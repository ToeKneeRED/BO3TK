#pragma once
#include <Winternl.h>
#include <dbghelp.h>
#include <d3d11.h>

#include "HookEvent.h"
#include "MinHook.h"
#include "ImGuiService.h"
#include "Log.h"
#include "Com_PrintMessage.h"
#include "structs.h"

#pragma comment(lib, "dbghelp.lib")

#define OFFSET(offset) (*(uint64_t*)((uint64_t)(NtCurrentTeb()->ProcessEnvironmentBlock) + 0x10) + (uint64_t)(offset))

namespace
{
#define FUNC_HOOK(name, target)                                           \
    {                                                                     \
        target, { (uintptr_t) & Hooks::h##name, (void**)&Hooks::o##name } \
    }
#define LIB_HOOK(libName, funcName)                                                        \
    {                                                                                      \
        reinterpret_cast<uintptr_t>(GetProcAddress(GetModuleHandleA(libName), #funcName)), \
        {                                                                                  \
            (uintptr_t)&Hooks::h##funcName, (void**)&Hooks::o##funcName                    \
        }                                                                                  \
    }
} // namespace

struct Hook
{
    Hook() = default;
    Hook(const uintptr_t& acDetour, void** apOriginal)
        : Original(apOriginal)
        , Detour(acDetour)
    {
    }

    void** Original{};
    uintptr_t Detour{};
};

namespace Exe
{
inline HANDLE BaseModule = GetModuleHandleA(nullptr);
inline LPVOID EntryPoint{};
inline DWORD Size{};
inline uintptr_t End{};
inline HWND WindowHandle{};
} // namespace Exe

inline MH_STATUS g_status{};
inline HWND g_window = nullptr;
inline ID3D11Device* g_pDevice = nullptr;
inline ID3D11DeviceContext* g_pContext = nullptr;
inline ID3D11RenderTargetView* g_mainRenderTargetView;
inline ImGuiService* g_imguiService = nullptr;

// Need to find base ptr
struct Player
{
    const char* Name = nullptr;
    uint32_t* Points = nullptr;
    uint16_t* Kills = nullptr;
    uint16_t* Ping = nullptr;
};

// BO3Enhanced
namespace BO3E
{
static bool* IsInGame = reinterpret_cast<bool*>(OFFSET(0x31E1930)); // scoreboard value?
static uint32_t* ZombiesAlive = reinterpret_cast<uint32_t*>(OFFSET(0x9A1B6AC));
static Player LocalPlayer = {
    .Name = reinterpret_cast<const char*>(OFFSET(0x3B07498)),
    .Points = reinterpret_cast<uint32_t*>(OFFSET(0x3B074CC)),
    .Kills = reinterpret_cast<uint16_t*>(OFFSET(0x3B074D0)),
    .Ping = reinterpret_cast<uint16_t*>(OFFSET(0x3B074C0))};
static Player Player1;
static Player Player2;
static Player Player3;
static Player Player4;
static scrVmPub_t* gScrVmPub = reinterpret_cast<scrVmPub_t*>(OFFSET(0x3F66B50));
static dvar_t* s_dvarHashTable = reinterpret_cast<dvar_t*>(OFFSET(0x19B42530)); // array size: 1350
static dvar_t* s_dvarPool = reinterpret_cast<dvar_t*>(OFFSET(0x19A00D30));      // array size: 5400
static bool* s_isRunningUILevel = reinterpret_cast<bool*>(OFFSET(0x148FD0EF));
static void* s_playerData_ptr = reinterpret_cast<void*>(OFFSET(0x33614D0));
static ugcinfo_entry_wstor* currently_loaded_mod = reinterpret_cast<ugcinfo_entry_wstor*>(OFFSET(0x18A585E0));
static lua_State* luaVM = nullptr;

} // namespace BO3E

namespace Hooks
{
inline uint64_t* MethodsTable = nullptr;

static bool BindVTable(const uint16_t _index, void** _original, void* _function)
{
    assert(_original != NULL && _function != NULL);

    void* target = reinterpret_cast<void*>(MethodsTable[_index]);

    MH_STATUS status = MH_CreateHook(target, _function, _original);
    if (status != MH_OK)
    {
        LOG_ERROR("Failed to create hook: {} : {}", _function, MH_StatusToString(status));
        return false;
    }

    status = MH_EnableHook(target);
    if (status != MH_OK)
    {
        LOG_ERROR("Failed to enable hook: {} : {}", _function, MH_StatusToString(status));
        return false;
    }

    return true;
}

typedef int(WINAPI* MessageBoxA_t)(HWND, LPCSTR, LPCSTR, UINT);
inline MessageBoxA_t oMessageBoxA = nullptr;
inline int WINAPI hMessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT type)
{
    LOG_PRINT("[MessageBoxA] {}", lpText);

    if (!strcmp(lpCaption, "Safe mode")) return IDNO; // i am lazy

    return oMessageBoxA(hWnd, lpText, lpCaption, type);
}

typedef const char*(__fastcall* lua_pcall_t)(lua_State*, long, long);
inline lua_pcall_t olua_pcall = nullptr;
inline const char* hlua_pcall(lua_State* state, long nArgs, long nResults)
{
    LOG_PRINT("[lua_pcall] 0x{:X}", (uintptr_t)state);

    return olua_pcall(state, nArgs, nResults);
}

typedef const char*(__fastcall* Dvar_RegisterNew_t)(
    unsigned int, dvarType_t, unsigned int, unsigned int, void*, void*, __int64);
inline Dvar_RegisterNew_t oDvar_RegisterNew = nullptr;
inline const char* hDvar_RegisterNew(
    unsigned int dvarName, dvarType_t type, unsigned int a3, unsigned int a4, void* a5, void* a6, __int64 a7)
{
    LOG_PRINT("[Dvar_RegisterNew] 0x{:X} {}", dvarName, type);

    return oDvar_RegisterNew(dvarName, type, a3, a4, a5, a6, a7);
}

typedef dvar_t*(__fastcall* Dvar_SetFromStringByName_t)(const char*, const char*, uint64_t, uint64_t, uint64_t);
inline Dvar_SetFromStringByName_t oDvar_SetFromStringByName = nullptr;
inline dvar_t* hDvar_SetFromStringByName(
    const char* dvarName, const char* string, uint64_t source, uint64_t flags, uint64_t createIfMissing)
{
    LOG_PRINT("[Dvar_SetFromStringByName] Name: {}\t Value: {}", dvarName, string);

    auto result = oDvar_SetFromStringByName(dvarName, string, source, flags, createIfMissing);
    LOG_PRINT("dvar_t @ 0x{:X}", (uintptr_t)result);

    return result;
}

typedef int32_t(__fastcall* Com_HashString_t)(const char*);
inline Com_HashString_t oCom_HashString = nullptr;
inline int32_t hCom_HashString(const char* name)
{
    LOG_PRINT("[Com_HashString] {}", name);

    return oCom_HashString(name);
}

typedef size_t(__fastcall* PLmemcpy_t)(char*, size_t, const char*, size_t);
inline PLmemcpy_t oPLmemcpy = nullptr;
inline size_t hPLmemcpy(char* dst, size_t len, const char* src, size_t dstSize)
{
    LOG_PRINT("[PLmemcpy] Copy: {} to 0x{:X}", src, (uintptr_t)dst);

    return oPLmemcpy(dst, len, src, dstSize);
}

typedef __int64(__fastcall* RegisterLuaEnums_t)(lua_State*);
inline RegisterLuaEnums_t oRegisterLuaEnums = nullptr;
inline __int64 hRegisterLuaEnums(lua_State* luaVM)
{
    if (!BO3E::luaVM)
    {
        BO3E::luaVM = luaVM;
        LOG_PRINT("[RegisterLuaEnums] Set luaVM: 0x{:X}", (uintptr_t)BO3E::luaVM);
    }

    return oRegisterLuaEnums(luaVM);
}

//#define Com_Error(code, fmt, ...)                                                              \
//    ((void(__fastcall*)(const char*, uint32_t, uint32_t, const char*, ...))OFFSET(0x2210B90))( \
//        "", 0, code, fmt, __VA_ARGS__)
typedef void(__fastcall* Com_Error_t)(const char*, int, unsigned int, const char*, ...);
inline Com_Error_t oCom_Error = nullptr;
inline void hCom_Error(const char* a1, int a2, unsigned int a3, const char* a4, ...)
{
    return oCom_Error(a1, a2, a3, a4);
}

// yoinked from https://github.com/shiversoftdev/BO3Enhanced
#define LUA_REGISTRYINDEX (-10000)
#define LUA_ENVIRONINDEX (-10001)
#define LUA_GLOBALSINDEX (-10002)

inline HksObject getObjectForIndex(lua_State* s, int index)
{
    HksNumber result;
    HksObject* object;
    int failure;

    if (index <= LUA_REGISTRYINDEX)
    {
        failure = 0;
        switch (index)
        {
            case LUA_REGISTRYINDEX: object = &s->m_global->m_registry; break;
            case LUA_GLOBALSINDEX: object = &s->globals; break;
            case LUA_ENVIRONINDEX:
                s->m_cEnv.v.cClosure = (hks::cclosure*)s->m_apistack.base[-1].v.cClosure->m_env;
                s->m_cEnv.t = 5;
                object = &s->m_cEnv;
                break;
            default:
                object =
                    (HksObject*)(&s->m_apistack.base[-1].v.cClosure->m_numUpvalues + 8 * (LUA_GLOBALSINDEX - index));
                break;
        }
    }
    else if (index <= 0)
    {
        if (index >= 0)
        {
            failure = 1;
            object = 0LL;
        }
        else if (&s->m_apistack.top[index] < s->m_apistack.base)
        {
            failure = 1;
            object = 0LL;
        }
        else
        {
            failure = 0;
            object = &s->m_apistack.top[index];
        }
    }
    else if (&s->m_apistack.base[index - 1] >= s->m_apistack.top)
    {
        failure = 1;
        object = 0LL;
    }
    else
    {
        failure = 0;
        object = &s->m_apistack.base[index - 1];
    }

    return object ? *object : HksObject();
}

inline int
hksi_hksL_loadbuffer(lua_State* s, HksCompilerSettings* options, char const* buff, size_t sz, char const* name)
{
    typedef int hksi_hksL_loadbuffer_t(lua_State*, HksCompilerSettings*, char const*, size_t, char const*);
    auto* f = (hksi_hksL_loadbuffer_t*)(hksi_hksL_loadbuffer);
    return f(s, options, buff, sz, name);
}

inline void hksI_openlib(lua_State* s, const char* libname, const luaL_Reg l[], int nup, int isHksFunc)
{
    typedef void hksI_openlib_t(lua_State*, const char*, const luaL_Reg[], int, int);
    auto* f = (hksI_openlib_t*)(hksI_openlib);
    f(s, libname, l, nup, isHksFunc);
}

inline void
hks_pushnamedcclosure(lua_State* s, lua_CFunction fn, int n, const char* functionName, int treatClosureAsFuncForProf)
{
    typedef void hks_pushnamedcclosure_t(lua_State*, lua_CFunction, int, const char*, int);
    auto* f = (hks_pushnamedcclosure_t*)(hks_pushnamedcclosure);
    f(s, fn, n, functionName, treatClosureAsFuncForProf);
}

inline const char* hksi_lua_pushvfstring(lua_State* s, const char* fmt, va_list* argp)
{
    typedef const char* hksi_lua_pushvfstring_t(lua_State*, const char*, va_list*);
    auto f = (hksi_lua_pushvfstring_t*)(hksi_lua_pushvfstring);
    return f(s, fmt, argp);
}

inline const char* hks_obj_tolstring(lua_State* s, HksObject* obj, size_t* len)
{
    typedef const char* hks_obj_tolstring_t(lua_State*, HksObject*, size_t*);
    auto f = (hks_obj_tolstring_t*)(hks_obj_tolstring);
    return f(s, obj, len);
}

inline char Com_Error_(const char* file, int line, ErrorCode code, const char* fmt, ...)
{
    typedef char Com_Error_t(char const*, int, int, char const*, ...);
    auto f = (Com_Error_t*)(Com_Error_);
    return f(file, line, code, fmt);
}

inline void hksi_luaL_error(lua_State* s, const char* fmt, ...)
{
    typedef void hksi_luaL_error_t(lua_State*, const char*, ...);
    auto f = (hksi_luaL_error_t*)(hksi_luaL_error);
    f(s, fmt);
}

inline void luaL_argerror(lua_State* s, int narg, const char* extramsg)
{
    typedef void luaL_argerror_t(lua_State*, int, const char*);
    auto f = (luaL_argerror_t*)(luaL_argerror);
    f(s, narg, extramsg);
}

inline void Lua_CoD_LuaStateManager_Error(const char* error, lua_State* luaVM)
{
    typedef void Lua_CoD_LuaStateManager_Error_t(const char*, lua_State*);
    auto f = (Lua_CoD_LuaStateManager_Error_t*)(Lua_CoD_LuaStateManager_Error);
    f(error, luaVM);
}

inline int lua_pcall(lua_State* s, long nargs, long nresults)
{
    typedef int lua_pcall_t(lua_State * luaState, long, long);
    lua_pcall_t* f2 = (lua_pcall_t*)(lua_pcall);
    return f2(s, nargs, nresults);
}

inline void luaL_register(lua_State* s, const char* libname, const luaL_Reg* l) { hksI_openlib(s, libname, l, 0, 1); }

inline void lua_setfield(lua_State* s, int index, const char* k)
{
    typedef void lua_setfield_t(lua_State * luaState, int, const char*);
    auto f = (lua_setfield_t*)(lua_setfield);
    f(s, index, k);
}

inline void lua_setglobal(lua_State* s, const char* k) { lua_setfield(s, LUA_GLOBALSINDEX, k); }

inline void lua_pop(lua_State* s, int n) { s->m_apistack.top -= n; }

inline HksNumber lua_tonumber(lua_State* s, int index)
{
    auto object = getObjectForIndex(s, index);
    return object.v.number;
}

inline const char* lua_tostring(lua_State* s, int index)
{
    auto object = getObjectForIndex(s, index);
    return hks_obj_tolstring(s, &object, 0);
}

inline const void* lua_topointer(lua_State* s, int index)
{
    typedef const void* lua_topointer_t(lua_State * luaState, int);
    auto f = (lua_topointer_t*)(lua_topointer);
    return f(s, index);
}

inline int lua_toboolean(lua_State* s, int index)
{
    typedef int lua_toboolean_t(lua_State * luaState, int);
    auto f = (lua_toboolean_t*)(lua_toboolean);
    return f(s, index);
}

inline hksUint64 lua_toui64(lua_State* s, int index)
{
    typedef hksUint64 lua_toui64_t(lua_State * luaState, int);
    auto f = (lua_toui64_t*)(lua_toui64);
    return f(s, index);
}

inline void lua_pushnumber(lua_State* s, HksNumber n)
{
    auto top = s->m_apistack.top;
    top->v.number = n;
    top->t = TNUMBER;
    s->m_apistack.top = top + 1;
}

inline void lua_pushinteger(lua_State* s, int n)
{
    auto top = s->m_apistack.top;
    top->v.number = float(n);
    top->t = TNUMBER;
    s->m_apistack.top = top + 1;
}

inline void lua_pushnil(lua_State* s)
{
    auto top = s->m_apistack.top;
    top->v.number = 0;
    top->t = TNIL;
    s->m_apistack.top = top + 1;
}

inline void lua_pushboolean(lua_State* s, int b)
{
    auto top = s->m_apistack.top;
    top->v.boolean = b;
    top->t = TBOOLEAN;
    s->m_apistack.top = top + 1;
}

inline void lua_pushvalue(lua_State* s, int index)
{
    HksObject* st;
    auto object = getObjectForIndex(s, index);
    st = s->m_apistack.top;
    *st = object;
    s->m_apistack.top = st + 1;
}

inline void lua_pushlstring(lua_State* s, const char* str, size_t l)
{
    typedef void lua_pushlstring_t(lua_State * luaState, const char*, size_t);
    auto f = (lua_pushlstring_t*)(lua_pushlstring);
    f(s, str, l);
}

inline void lua_pushfstring(lua_State* s, const char* fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    hksi_lua_pushvfstring(s, fmt, &va);
}

inline void lua_pushvfstring(lua_State* s, const char* fmt, va_list* argp) { hksi_lua_pushvfstring(s, fmt, argp); }

inline void lua_getfield(lua_State* s, int index, const char* k)
{
    auto object = getObjectForIndex(s, index);

    constexpr HksRegister v16{};

    auto top = --s->m_apistack.top;
    top->v.cClosure = object.v.cClosure;
    s->m_apistack.top = top++;
}

inline void lua_getglobal(lua_State* s, const char* k) { lua_getfield(s, LUA_GLOBALSINDEX, k); }

typedef const char*(__fastcall* hks_obj_tolstring_t)(lua_State*, HksObject*, size_t*);
inline hks_obj_tolstring_t ohks_obj_tolstring = nullptr;
inline const char* hhks_obj_tolstring(lua_State* state, HksObject* object, size_t* size)
{
    return ohks_obj_tolstring(state, object, size);
}

typedef HRESULT(__stdcall* Present_t)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
inline Present_t OriginalPresent = nullptr;
inline bool init = false;
inline HRESULT __stdcall HookPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
    if (!init)
    {
        if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&g_pDevice))))
        {
            g_pDevice->GetImmediateContext(&g_pContext);

            DXGI_SWAP_CHAIN_DESC swapChainDesc;
            HRESULT result = pSwapChain->GetDesc(&swapChainDesc);
            if (FAILED(result))
            {
                LOG_ERROR("Failed to get swapchain description");
                return result;
            }
            g_window = swapChainDesc.OutputWindow;

            ID3D11Texture2D* pBackBuffer;
            result = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&pBackBuffer));
            if (FAILED(result))
            {
                LOG_ERROR("Failed to get swapchain back buffer");
                return result;
            }

            result = g_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
            if (FAILED(result))
            {
                LOG_ERROR("Failed to create render target view");
                return result;
            }
            pBackBuffer->Release();

            g_imguiService = ImGuiService::Init(g_window, g_pDevice, g_pContext);
            init = true;
        }
        else
        {
            LOG_ERROR("Failed to get device from swapchain");
            return OriginalPresent(pSwapChain, SyncInterval, Flags);
        }
    }

    g_imguiService->OnDraw();

    g_pContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
    return OriginalPresent(pSwapChain, SyncInterval, Flags);
}
} // namespace Hooks

const static std::unordered_map<uintptr_t, Hook> g_hookLookupMap{
    FUNC_HOOK(Com_PrintMessage, 0x220F4F0), FUNC_HOOK(Com_Error, 0x13DF170),
    FUNC_HOOK(Com_HashString, 0x2210B90),   FUNC_HOOK(Dvar_SetFromStringByName, 0x23ABA90),
    FUNC_HOOK(Dvar_RegisterNew, 0x23A6870), FUNC_HOOK(RegisterLuaEnums, 0x200BCC0),
    FUNC_HOOK(lua_pcall, 0x1E54EF0),        FUNC_HOOK(PLmemcpy, 0x23B46F0),
    LIB_HOOK("user32.dll", MessageBoxA),
};
static std::vector<uintptr_t> g_createdHookTargets{};

static bool HookFunction(const std::unique_ptr<IHook>& acpHook)
{
    if (const auto cIter = std::ranges::find(g_createdHookTargets, acpHook->Target);
        cIter == g_createdHookTargets.end())
    {
        g_status = MH_CreateHook(
            reinterpret_cast<void*>((OFFSET(acpHook->Target))), reinterpret_cast<void*>(acpHook->Detour),
            acpHook->Original);
        if (g_status != MH_OK)
        {
            LOG_ERROR(
                "{}Failed to create hook for 0x{:X} {}\t(0x{:X})", NarrowText::Foreground::Red, acpHook->Target,
                MH_StatusToString(g_status), OFFSET(acpHook->Target));
            return false;
        }

        g_createdHookTargets.push_back(acpHook->Target);
    }

    g_status = acpHook->Enabled ? MH_EnableHook(reinterpret_cast<void*>((OFFSET(acpHook->Target))))
                                : MH_DisableHook(reinterpret_cast<void*>((OFFSET(acpHook->Target))));
    if (g_status != MH_OK)
    {
        LOG_ERROR(
            "{}Failed to {} hook for 0x{:X} {}", NarrowText::Foreground::Red, acpHook->Enabled ? "enable" : "disable",
            acpHook->Target, MH_StatusToString(g_status));
        return false;
    }

    return true;
}
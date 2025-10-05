#pragma once
#include <d3d11.h>

#include "ImGuiService.h"
#include "Log.h"
#include "MinHook.h"
#include "structs.h"

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

#define ADDRESS(offset) (uintptr_t)((uintptr_t)Exe::BaseModule + (offset))

#define CREATE_HOOK(offset, detour, original)                                                  \
    g_status = MH_CreateHook((void*)(ADDRESS(offset)), (LPVOID) & detour, (LPVOID*)&original); \
    if (g_status != MH_OK)                                                                     \
    {                                                                                          \
        Log::Get()->Error(                                                                     \
            "{}Failed to create hook for 0x{:X} {}", NarrowText::Foreground::Red, offset,      \
            MH_StatusToString(g_status));                                                      \
    }                                                                                          \
    else                                                                                       \
    {                                                                                          \
        Log::Get()->Print(L"{}Hook created: 0x{:X}", WideText::Foreground::Green, offset);     \
    }

#define ENABLE_HOOK(offset)                                                                \
    g_status = MH_EnableHook((LPVOID)(ADDRESS(offset)));                                   \
    if (g_status != MH_OK)                                                                 \
    {                                                                                      \
        Log::Get()->Error(                                                                 \
            "{}Failed to enable hook for 0x{:X} {}", NarrowText::Foreground::Red, offset,  \
            MH_StatusToString(g_status));                                                  \
    }                                                                                      \
    else                                                                                   \
    {                                                                                      \
        Log::Get()->Print(L"{}Hook enabled: 0x{:X}", WideText::Foreground::Green, offset); \
    }

#define FUNC_HOOK(offset, funcName)                             \
    CREATE_HOOK(offset, Hooks::h##funcName, Hooks::o##funcName) \
    ENABLE_HOOK(offset)

#define LIB_HOOK(lib, function, detour, original)                                                \
    {                                                                                            \
        HMODULE module = GetModuleHandleA(lib);                                                  \
        if (!module)                                                                             \
            module = LoadLibraryA(lib);                                                          \
        if (module)                                                                              \
        {                                                                                        \
            void* target = GetProcAddress(module, #function);                                    \
            if (target)                                                                          \
            {                                                                                    \
                if (MH_CreateHook(target, detour, reinterpret_cast<LPVOID*>(original)) == MH_OK) \
                {                                                                                \
                    MH_EnableHook(target);                                                       \
                    Log::Get()->Print("[Hook] {}", #function);                                   \
                }                                                                                \
                else                                                                             \
                {                                                                                \
                    Log::Get()->Error("[Hook] Failed to create {}", #function);                  \
                }                                                                                \
            }                                                                                    \
            else                                                                                 \
            {                                                                                    \
                Log::Get()->Error("[Hook] Failed to find {}", #function);                        \
            }                                                                                    \
        }                                                                                        \
        else                                                                                     \
        {                                                                                        \
            Log::Get()->Error("[Hook] Failed to find {}", lib);                                  \
        }                                                                                        \
    }

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
static bool* IsInGame = reinterpret_cast<bool*>(ADDRESS(0x31E1930)); // better one somewhere else?
static uint32_t* ZombiesAlive = reinterpret_cast<uint32_t*>(ADDRESS(0x9A1B6AC));
static Player LocalPlayer = {
    .Name = reinterpret_cast<const char*>(ADDRESS(0x3B07498)),
    .Points = reinterpret_cast<uint32_t*>(ADDRESS(0x3B074CC)),
    .Kills = reinterpret_cast<uint16_t*>(ADDRESS(0x3B074D0)),
    .Ping = reinterpret_cast<uint16_t*>(ADDRESS(0x3B074C0))};
static Player Player1;
static Player Player2;
static Player Player3;
static Player Player4;

} // namespace BO3E

template <typename T> MH_STATUS MH_CreateHook(LPVOID pTarget, LPVOID pDetour, T** ppOriginal)
{
    return MH_CreateHook(pTarget, pDetour, reinterpret_cast<LPVOID*>(ppOriginal));
}

namespace Hooks
{
inline uint64_t* MethodsTable = nullptr;

static bool BindVTable(const uint16_t _index, void** _original, void* _function)
{
    Log* log = Log::Get();

    assert(_original != NULL && _function != NULL);

    void* target = reinterpret_cast<void*>(MethodsTable[_index]);

    MH_STATUS status = MH_CreateHook(target, _function, _original);
    if (status != MH_OK)
    {
        log->Error("Failed to create hook: {} : {}", _function, MH_StatusToString(status));
        return false;
    }

    status = MH_EnableHook(target);
    if (status != MH_OK)
    {
        log->Error("Failed to enable hook: {} : {}", _function, MH_StatusToString(status));
        return false;
    }

    return true;
}

typedef int(WINAPI* MessageBoxA_t)(HWND, LPCSTR, LPCSTR, UINT);
inline MessageBoxA_t OriginalMessageBoxA = nullptr;
inline int WINAPI HookMessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT type)
{
    if (!strcmp(lpCaption, "Safe mode"))
        return IDNO; // i am lazy

    return OriginalMessageBoxA(hWnd, lpText, lpCaption, type);
}

typedef void(__fastcall* Com_PrintMessage_t)(int, consoleLabel_e, const char*, int);
inline Com_PrintMessage_t oCom_PrintMessage = nullptr;
inline void __fastcall hCom_PrintMessage(int channel, consoleLabel_e label, const char* msg, int error)
{
    Log::Get()->Print("[Com_PrintMessage] {}", msg);

    return oCom_PrintMessage(channel, label, msg, error);
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
                Log::Get()->Error("Failed to get swapchain description");
                return result;
            }
            g_window = swapChainDesc.OutputWindow;

            ID3D11Texture2D* pBackBuffer;
            result = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&pBackBuffer));
            if (FAILED(result))
            {
                Log::Get()->Error("Failed to get swapchain back buffer");
                return result;
            }

            result = g_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
            if (FAILED(result))
            {
                Log::Get()->Error("Failed to create render target view");
                return result;
            }
            pBackBuffer->Release();

            g_imguiService = ImGuiService::Init(g_window, g_pDevice, g_pContext);
            init = true;
        }
        else
        {
            Log::Get()->Error("Failed to get device from swapchain");
            return OriginalPresent(pSwapChain, SyncInterval, Flags);
        }
    }

    g_imguiService->OnDraw();

    g_pContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
    return OriginalPresent(pSwapChain, SyncInterval, Flags);
}
} // namespace Hooks
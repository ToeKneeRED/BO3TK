#pragma once
#include <d3d11.h>

#include "ImGuiService.h"
#include "Log.h"
#include "MinHook.h"

namespace Exe
{
inline HANDLE BaseModule = GetModuleHandleA(nullptr);
inline LPVOID EntryPoint{};
inline DWORD Size{};
inline uintptr_t End{};
inline HWND WindowHandle{};
}

inline MH_STATUS g_status{};
inline HWND g_window = nullptr;
inline ID3D11Device* g_pDevice = nullptr;
inline ID3D11DeviceContext* g_pContext = nullptr;
inline ID3D11RenderTargetView* g_mainRenderTargetView;

namespace
{
ImGuiService* g_imguiService = nullptr;
}

#define CREATE_HOOK(target, detour, original)\
    g_status = MH_CreateHook((void*)(target), (LPVOID) &detour, (LPVOID*)&original);\
    if (g_status != MH_OK)\
    {\
        Log::Get()->Error("{}Failed to create hook for {} {}", NarrowText::Foreground::Red, #target, MH_StatusToString(g_status));\
    }\
    else\
    {\
        Log::Get()->Print(L"{}Hook created: {}", WideText::Foreground::Green, L#target);\
    }\

#define ENABLE_HOOK(target)\
    g_status = MH_EnableHook((LPVOID)(target));\
    if (g_status != MH_OK)\
        {\
            Log::Get()->Error("{}Failed to enable hook for {} {}", NarrowText::Foreground::Red, #target, MH_StatusToString(g_status));\
        }\
        else\
        {\
            Log::Get()->Print(L"{}Hook enabled: {}", WideText::Foreground::Green, L#target);\
        }\

#define ADDRESS(offset) (uintptr_t)((uintptr_t)Exe::BaseModule + (offset))

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
    .Ping = reinterpret_cast<uint16_t*>(ADDRESS(0x3B074C0)) };
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
            DXGI_SWAP_CHAIN_DESC sd;
            pSwapChain->GetDesc(&sd);
            g_window = sd.OutputWindow;
            ID3D11Texture2D* pBackBuffer;
            pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&pBackBuffer));
            g_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
            pBackBuffer->Release();
            g_imguiService = ImGuiService::Init(g_window, g_pDevice, g_pContext);
            init = true;
        }
        else
        {
            Log::Get()->Error("Failed to retrieve ID3D11Device from IDXGISwapChain");
            return OriginalPresent(pSwapChain, SyncInterval, Flags);
        }
    }

    g_imguiService->OnDraw();

    g_pContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
    return OriginalPresent(pSwapChain, SyncInterval, Flags);
}
} // namespace Hooks
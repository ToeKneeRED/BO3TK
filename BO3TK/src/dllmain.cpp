#include <any>
#include <filesystem>
#include <d3d11.h>
#include <tlhelp32.h>
#include <Psapi.h>
#include <windows.h>

#include "Hooks.h"
#include "Log.h"
#include "ImGuiService.h"

#include <Event/EventHandler.h>
#include "HookEvent.h"

std::atomic g_stop = false;
static std::shared_ptr<EventHandler> g_pEventHandler = nullptr;

typedef LONG(NTAPI* NtResumeThread_t)(HANDLE, PULONG);
static void ResumeProcess(const DWORD acProcessId)
{
    const HANDLE cThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

    if (cThreadSnap == INVALID_HANDLE_VALUE) return;

    THREADENTRY32 te32;
    te32.dwSize = sizeof(THREADENTRY32);

    if (Thread32First(cThreadSnap, &te32))
    {
        do {
            if (te32.th32OwnerProcessID == acProcessId)
            {
                if (const HANDLE cHThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te32.th32ThreadID))
                {
                    if (const HMODULE cHNtDll = GetModuleHandleA("ntdll.dll"))
                    {
                        if (const auto cNtResumeThread =
                                reinterpret_cast<NtResumeThread_t>(GetProcAddress(cHNtDll, "NtResumeThread")))
                        {
                            ULONG suspendCount = 0;
                            cNtResumeThread(cHThread, &suspendCount);
                        }
                    }
                    CloseHandle(cHThread);
                }
            }
        } while (Thread32Next(cThreadSnap, &te32));
    }
    CloseHandle(cThreadSnap);
}

static void MainThread()
{
    MODULEINFO modInfo;
    if (GetModuleInformation(GetCurrentProcess(), static_cast<HMODULE>(Exe::BaseModule), &modInfo, sizeof(modInfo)))
    {
        Exe::Size = modInfo.SizeOfImage;
        Exe::EntryPoint = modInfo.EntryPoint;
        Exe::End = reinterpret_cast<uintptr_t>(modInfo.lpBaseOfDll) + static_cast<uintptr_t>(modInfo.SizeOfImage);
    }
    else
    {
        LOG_ERROR("Failed to retrieve module info");
        return;
    }

    // need to wait for exe to get far enough along so it doesnt grab garbage
    ResumeProcess(GetCurrentProcessId());

    if (ImGuiService::Test())
    {
        if (const MH_STATUS cStatus = MH_Initialize(); cStatus != MH_OK)
        {
            LOG_ERROR("Failed minhook initialize: {}", MH_StatusToString(cStatus));
            return;
        }

        while (!Hooks::BindVTable(8, reinterpret_cast<void**>(&Hooks::OriginalPresent), &Hooks::HookPresent))
        {
            if (g_stop) return;

            LOG_INFO("{}Waiting for Present hook...", NarrowText::Foreground::Yellow);
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
        }

        LOG_INFO("{}Present hooked", NarrowText::Foreground::Green);
    }
}
static BOOL APIENTRY DllMain(const HMODULE hModule, const DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        {
            DisableThreadLibraryCalls(hModule);

            std::thread(MainThread).detach();
            std::thread(
                [=]
                {
                    while (!g_stop)
                    {
                        if (GetAsyncKeyState(VK_END) & 0x01)
                        {
                            spdlog::default_logger()->flush();
                            LOG_PRINT("Exiting...");

                            g_stop = true;
                        }

                        std::this_thread::sleep_for(std::chrono::milliseconds(300));
                    }
                })
                .detach();
            std::thread(
                [=]()
                {
                    g_pEventHandler = std::make_shared<EventHandler>("BO3TK_exe", "BO3TK_dll");
                    g_pEventHandler->AddCallback<HookEvent>(
                        [](const HookData& acData)
                        {
                            const std::unique_ptr<IHook> cpHook = std::make_unique<IHook>(
                                acData.Type, acData.FuncName, acData.Target, acData.Detour, nullptr, acData.Enabled);
                            switch (acData.Type)
                            {
                                case HookType::Function:
                                    LOG_PRINT("[HookEvent<FuncHook>] {}: {}", acData.FuncName, acData.Enabled);
                                    break;

                                case HookType::Library:
                                    LOG_PRINT(
                                        "[HookEvent<LibHook>] {}\t{}: {}", acData.LibName, acData.FuncName,
                                        acData.Enabled);

                                    cpHook->Target = reinterpret_cast<uintptr_t>(
                                        GetProcAddress(GetModuleHandleA(acData.LibName), acData.FuncName));

                                    break;
                                case HookType::None: return;
                            }

                            if (const auto cIter = g_hookLookupMap.find(cpHook->Target); cIter != g_hookLookupMap.end())
                            {
                                if (cpHook->Target > reinterpret_cast<uintptr_t>(Exe::BaseModule))
                                {
                                    cpHook->Target -= reinterpret_cast<uintptr_t>(Exe::BaseModule);
                                }
                                cpHook->Detour = cIter->second.Detour;
                                cpHook->Original = cIter->second.Original;

                                if (HookFunction(std::move(cpHook)))
                                {
                                    LOG_PRINT(
                                        "{}: {}", acData.FuncName, acData.Enabled ? "enabled" : "disabled");
                                }
                            }
                            else { LOG_ERROR("Target function not found in lookup map"); }
                        });
                    while (!g_pEventHandler->Run())
                    {
                        LOG_ERROR("Disconnected from EventHandler...");
                        std::this_thread::sleep_for(std::chrono::milliseconds(g_pEventHandler->SleepDuration));
                    }
                })
                .detach();
        }
        break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            if (g_stop)
            {
                MH_DisableHook(MH_ALL_HOOKS);
                MH_Uninitialize();
            }
            break;
        default: break;
    }

    return TRUE;
}

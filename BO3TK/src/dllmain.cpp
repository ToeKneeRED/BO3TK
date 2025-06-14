#include <any>
#include <filesystem>
#include <d3d11.h>
#include <tlhelp32.h>
#include <Psapi.h>

#include "Hooks.h"
#include "ImGuiService.h"
#include "Log.h"

auto* g_log = Log::Get();
std::atomic g_stop = false;

typedef LONG(NTAPI* NtResumeThread_t)(HANDLE, PULONG);
static void ResumeProcess(const DWORD acProcessId)
{
    const HANDLE cThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

    if (cThreadSnap == INVALID_HANDLE_VALUE)
        return;

    THREADENTRY32 te32;
    te32.dwSize = sizeof(THREADENTRY32);

    if (Thread32First(cThreadSnap, &te32))
    {
        do
        {
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

// function hook test
typedef __int64(__fastcall* Sub_141E3FD80_t)(__int64 a1, __int64 a2);
Sub_141E3FD80_t OriginalSub = nullptr;
__int64 __fastcall HookSub_141E3FD80(__int64 a1, __int64 a2)
{
    g_log->Print("called thing");

    return OriginalSub(a1, a2);
}

static DWORD WINAPI MainThread(const std::atomic<bool>& aStop)
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
        g_log->Print("mod info failed");
        return 0;
    }

    if (ImGuiService::Test())
    {
        // need to wait for exe to get far enough along so it doesnt grab garbage
        ResumeProcess(GetCurrentProcessId());

        if (const MH_STATUS cStatus = MH_Initialize(); cStatus != MH_OK)
        {
            g_log->Error("Failed minhook initialize: {}", MH_StatusToString(cStatus));
            return FALSE;
        }

        do
        {
            if (aStop)
                return FALSE;

            g_log->Print("{}Waiting for Present hook...", NarrowText::Foreground::Yellow);
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
        } while (!Hooks::BindVTable(8, reinterpret_cast<void**>(&Hooks::OriginalPresent), &Hooks::HookPresent));

        g_log->Print("{}Present hooked", NarrowText::Foreground::Green);
    }

    CREATE_HOOK(0x1E3FD80, HookSub_141E3FD80, OriginalSub)
    ENABLE_HOOK(0x1E3FD80)

    return TRUE;
}

static BOOL APIENTRY DllMain(const HMODULE hModule, const DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        DisableThreadLibraryCalls(hModule);

        std::thread(MainThread, std::ref(g_stop)).detach();
        std::thread(
            [&]()
            {
                while (!g_stop.load())
                {
                    if (GetAsyncKeyState(VK_END) & 0x01)
                    {
                        spdlog::default_logger()->flush();
                        g_log->Print("Exiting...");

                        g_stop.store(true);
                    }

                    std::this_thread::sleep_for(std::chrono::milliseconds(300));
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

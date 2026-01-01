#include <any>
#include <filesystem>
#include <d3d11.h>
#include <tlhelp32.h>
#include <Psapi.h>
#include <fstream>
#include <windows.h>

#include "Hooks.h"
#include "Log.h"
#include "ImGuiService.h"

//#include "CommandDispatcher.h"
//#include "CommandEvent.h"
#include <Event/EventHandler.h>

#include "HookEvent.h"


auto* g_log = Log::Get();
std::atomic g_stop = false;
std::unordered_map<dvarStrHash_t, std::string> g_dvarHashMap{};

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

static DWORD WINAPI MainThread(const std::atomic<bool>& acStop)
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
            if (acStop)
                return FALSE;

            g_log->Print("{}Waiting for Present hook...", NarrowText::Foreground::Yellow);
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
        } while (!Hooks::BindVTable(8, reinterpret_cast<void**>(&Hooks::OriginalPresent), &Hooks::HookPresent));

        g_log->Print("{}Present hooked", NarrowText::Foreground::Green);
    }

    // safe mode
    LIB_HOOK("user32.dll", MessageBoxA, &Hooks::hMessageBoxA, &Hooks::oMessageBoxA)
    FUNC_HOOK(0x200BCC0, RegisterLuaEnums)
    //FUNC_HOOK(0x2210B90, Com_Error)
    FUNC_HOOK(0x23ABA90, Dvar_SetFromStringByName)
    //FUNC_HOOK(0x13DF170, Com_HashString)
    FUNC_HOOK(0x23B46F0, PLmemcpy)
    FUNC_HOOK(0x220F4F0, Com_PrintMessage)
    //FUNC_HOOK(0x1E54EF0, lua_pcall)
    FUNC_HOOK(0x23A6870, Dvar_RegisterNew)
    //MH_EnableHook(nullptr);

    return 0;
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
            [&]
            {
                while (!g_stop.load())
                {
                    if (GetAsyncKeyState(VK_END) & 0x01)
                    {
                        spdlog::default_logger()->flush();
                        g_log->Print("Exiting...");

                        g_stop.store(true);
                    }
                    else if (GetAsyncKeyState(VK_HOME) & 0x01)
                    {
                        Com_Error(ERROR_UI, "Something :)");
                        //Hooks::oCom_Error("", 0, ERROR_UI, "Something :)");
                    }

                    std::this_thread::sleep_for(std::chrono::milliseconds(300));
                }
            })
            .detach();
        std::thread([&]()
        {
            std::unique_ptr<EventHandler> eventHandler{ new EventHandler("BO3TK_dll") };

            eventHandler->RegisterEvent<HookEvent>([](const HookEvent& acEvent)
            {
                auto data = static_cast<const FuncHook*>(acEvent.GetData());
                Log::Get()->Print("[HookEvent] {}: {}", data->Name, data->Enabled);
            });
            eventHandler->Start();
        }).detach();
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

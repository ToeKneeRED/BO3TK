#include <any>
#include <filesystem>
#include <d3d11.h>
#include <tlhelp32.h>
#include <Psapi.h>
#include <bcrypt.h>
#include <windows.h>

#include "Hooks.h"
#include "Log.h"
#include "ImGuiService.h"
#include "CommandEvent.h"
#include <Handler.h>

#include "CommandDispatcher.h"

#pragma comment(lib, "bcrypt.lib")

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

typedef int(WINAPI* MessageBoxA_t)(HWND, LPCSTR, LPCSTR, UINT);
MessageBoxA_t OriginalMessageBoxA = nullptr;
inline int WINAPI HookMessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT type)
{
    if (!strcmp(lpCaption, "Safe mode"))
        return IDNO; // i am lazy

    return OriginalMessageBoxA(hWnd, lpText, lpCaption, type);
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

    LIB_HOOK("user32.dll", MessageBoxA, &HookMessageBoxA, &OriginalMessageBoxA)
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

                    std::this_thread::sleep_for(std::chrono::milliseconds(300));
                }
            })
            .detach();
        /*std::thread([&]
        {
            auto* eventHandler = new EventHandler<CommandEvent>{"BO3TK_dll"};
            CommandDispatcher dispatcher;

            eventHandler->Listen([&](const std::string& acData)
            {
                const std::string& cPrefix = dispatcher.Prefix;
                if (acData.starts_with(cPrefix))
                {
                    const auto cStart = acData.find('(');
                    const auto cEnd = acData.find(')');

                    if (cStart == std::string::npos || cEnd == std::string::npos || cEnd <= cStart)
                        return;

                    const std::string cCommandName = acData.substr(cPrefix.size(), cStart - cPrefix.size());
                    const std::string cArgString = acData.substr(cStart + 1, cEnd - cStart - 1);

                    std::vector<std::string> args;
                    std::stringstream ss(cArgString);
                    std::string token;
                    while (std::getline(ss, token, ',')) {
                        token.erase(0, token.find_first_not_of(" \t"));
                        token.erase(token.find_last_not_of(" \t") + 1);
                        args.push_back(token);
                    }

                    Log::Get()->Print("{} {}", cCommandName.c_str(), cArgString.c_str());
                    for (const auto& cArg : args)
                    {
                        Log::Get()->Print("arg: {}", cArg.c_str());
                    }

                    try {
                        dispatcher.Execute(acData);
                    } catch (const std::exception& cException) {
                        Log::Get()->Error("Command execution failed: {}", cException.what());
                    }
                }
            });
        }).detach();*/
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

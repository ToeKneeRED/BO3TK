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

#include "CommandDispatcher.h"
#include <Event/EventHandler.h>

#include "CommandEvent.h"

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

    LIB_HOOK("user32.dll", MessageBoxA, &Hooks::HookMessageBoxA, &Hooks::OriginalMessageBoxA)
    FUNC_HOOK(0x220F4F0, Com_PrintMessage)
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
        std::thread([&]
        {
            const auto cEventHandler = new EventHandler("BO3TK_dll");
            std::shared_ptr<CommandDispatcher> commandDispatcher = std::make_shared<CommandDispatcher>();
            const std::string& cPrefix = commandDispatcher->GetPrefix();

            cEventHandler->AddCallback([&, commandDispatcher](CommandEvent::DataType acData)
            {
                if (acData.starts_with(cPrefix))
                {
                    const std::string cNoPrefix = acData.starts_with(cPrefix) ? 
                        acData.substr(cPrefix.length()) : acData;
                    std::istringstream stream(cNoPrefix);
                    CommandName commandName;
                    stream >> commandName;
                    std::ranges::transform(commandName, commandName.begin(),
                          [](unsigned char c){ return std::tolower(c); });

                    CommandArgs args;
                    std::string arg;
                    while (stream >> arg)
                        args.push_back(arg);

                    const auto* cpCommand = commandDispatcher->FindCommand(cNoPrefix.substr(cNoPrefix.find_first_of(' ') + 1));
                    auto* command = commandDispatcher->StringToCommand(cNoPrefix);

                    if (commandName == "addcommand")
                    {
                        if (!cpCommand)
                        {
                            std::string newCommand = cNoPrefix.substr(cNoPrefix.find_first_of(' ') + 1);
                            CommandArgs newArgs{newCommand.substr(newCommand.find_first_of(' ') + 1)};
                            newCommand = newCommand.erase(newCommand.find_first_of(' '));
                            Log::Get()->Print("AddCommand: {}", newCommand);
                            commandDispatcher->AddCommand(new Command(newCommand, newArgs, CommandCallbacks()));
                            commandDispatcher->AddCallback(newCommand, [&](const CommandArgs& acArgs)
                            {
                                for (const auto& cCmdArg : acArgs)
                                {
                                    Log::Get()->Print("arg: {}", cCmdArg.c_str());
                                }
                            });
                        }
                    }
                    else
                    {
                        std::string newCommand = cNoPrefix.substr(cNoPrefix.find_first_of(' ') + 1);
                        CommandArgs newArgs{newCommand.substr(newCommand.find_first_of(' ') + 1)};
                        newCommand = newCommand.erase(newCommand.find_first_of(' '));

                        if (cpCommand)
                            commandDispatcher->Call(newCommand, newArgs);
                    }
                }
            });

            cEventHandler->Start();
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

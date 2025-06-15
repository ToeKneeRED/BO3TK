#include "Dashboard.h"
#include "Log.h"
#include <filesystem>
#include "Runner.h"

#include "BrowseButton.h"
#include "Button.h"
#include "InputField.h"
#include "shellapi.h"

int APIENTRY wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ const LPWSTR lpCmdLine, _In_ int)
{
    int argc;
    LPWSTR* argvW = CommandLineToArgvW(lpCmdLine, &argc);

    std::vector<std::string> narrowArgs;
    std::vector<char*> argv;
    for (int i = 0; i < argc; ++i)
    {
        if (const int cLen = WideCharToMultiByte(CP_UTF8, 0, argvW[i], -1, nullptr, 0, nullptr, nullptr); cLen > 0)
        {
            std::string narrow(cLen, '\0');
            WideCharToMultiByte(CP_UTF8, 0, argvW[i], -1, narrow.data(), cLen, nullptr, nullptr);
            narrowArgs.push_back(std::move(narrow));
            argv.push_back(narrowArgs.back().data());
        }
    }
    Dashboard::Init(argc, argv.data());
    LocalFree(argvW);

    Runner::CreateDashboardComponents();

    return Dashboard::Run();
}

BOOL Runner::Inject(const DWORD acProcessId, LPCSTR apDllPath)
{
    if (acProcessId == 0 || !std::filesystem::exists(apDllPath))
    {
        Log::Get()->Error("{} does not exist or invalid process ID", apDllPath);
        return FALSE;
    }

    const HANDLE cProcess = OpenProcess(
        PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ,
        FALSE, acProcessId);
    if (!cProcess)
    {
        char buf[256];
        FormatMessageA(
            FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, GetLastError(),
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, (sizeof(buf) / sizeof(char)), nullptr);
        Log::Get()->Error("Failed to open process: {}", buf);
        return FALSE;
    }

    const HMODULE cKernel32 = GetModuleHandleA("kernel32.dll");
    if (!cKernel32)
    {
        Log::Get()->Error("Failed to get kernel32.dll handle");
        CloseHandle(cProcess);
        return FALSE;
    }

    const LPVOID cLoadLibraryAddr = reinterpret_cast<LPVOID>(GetProcAddress(cKernel32, "LoadLibraryA"));
    if (!cLoadLibraryAddr)
    {
        Log::Get()->Error("Failed to get LoadLibraryA address");
        CloseHandle(cProcess);
        return FALSE;
    }

    const SIZE_T cDllPathLen = strlen(apDllPath) + 1;
    const LPVOID cRemoteString =
        VirtualAllocEx(cProcess, nullptr, cDllPathLen, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!cRemoteString)
    {
        Log::Get()->Error("VirtualAllocEx failed");
        CloseHandle(cProcess);
        return FALSE;
    }

    BOOL result = FALSE;
    if (WriteProcessMemory(cProcess, cRemoteString, apDllPath, cDllPathLen, nullptr))
    {
        if (const HANDLE cRemoteThread = CreateRemoteThread(
                cProcess, nullptr, 0, static_cast<LPTHREAD_START_ROUTINE>(cLoadLibraryAddr), cRemoteString, 0, nullptr))
        {
            WaitForSingleObject(cRemoteThread, INFINITE);
            CloseHandle(cRemoteThread);
            result = TRUE;
        }
        else
        {
            Log::Get()->Error("CreateRemoteThread failed");
        }
    }
    else
    {
        Log::Get()->Error("WriteProcessMemory failed");
    }

    VirtualFreeEx(cProcess, cRemoteString, 0, MEM_RELEASE);
    CloseHandle(cProcess);

    return result;
}

void Runner::OnLaunchButtonPress(Button* apButton)
{
    STARTUPINFO sInfo{};
    PROCESS_INFORMATION pInfo{};

    if (const auto cPath = Dashboard::GamePath.c_str(); // TODO: deal with the disgusting \\\\ in paths with spaces,
                                                        // should still work otherwise (-4 hours)
        CreateProcessA(cPath, nullptr, nullptr, nullptr, FALSE, CREATE_SUSPENDED, nullptr, nullptr, &sInfo, &pInfo))
    {
        if (Runner::Inject(pInfo.dwProcessId, DLL_PATH))
        {
            apButton->AnimateColors(QColor("#1e5e3d"), QColor("#e0f4e9"));

            std::thread(
                [hProcess = pInfo.hProcess, hThread = pInfo.hThread, apButton]
                {
                    DWORD exitCode = STILL_ACTIVE;

                    while (exitCode == STILL_ACTIVE)
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds(2500));
                        if (!GetExitCodeProcess(hProcess, &exitCode))
                        {
                            break;
                        }
                    }

                    QMetaObject::invokeMethod(
                        apButton,
                        [=]
                        {
                            if (!apButton->IsAnimating())
                            {
                                apButton->AnimateToOriginal();
                            }
                        },
                        Qt::QueuedConnection);

                    CloseHandle(hProcess);
                    CloseHandle(hThread);
                })
                .detach();
        }
        else
        {
            apButton->AnimateColors(QColor("#6e1e1e"), QColor("#ff8e8e"));

            QTimer::singleShot(
                3000, apButton,
                [=]
                {
                    if (!apButton->IsAnimating())
                    {
                        apButton->AnimateToOriginal();
                    }
                });
        }
    }
    else
    {
        char buf[256];
        FormatMessageA(
            FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, GetLastError(),
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, (sizeof(buf) / sizeof(char)), nullptr);
        Log::Get()->Error("{}{}", buf, cPath);

        apButton->AnimateColors(QColor("#6e1e1e"), QColor("#ff8e8e"));

        QTimer::singleShot(
            3000, apButton,
            [=]
            {
                if (!apButton->IsAnimating())
                {
                    apButton->AnimateToOriginal();
                }
            });
    }
}

void Runner::CreateDashboardComponents()
{
    Dashboard::SetIcon(QIcon(":/BO3TK.ico"));

    Dashboard::GamePath = Dashboard::Settings->value("GamePath", GAME_PATH).toString().toUtf8().constData();
    Dashboard::DllPath = Dashboard::Settings->value("DllPath", DLL_PATH).toString().toUtf8().constData();

    Button* launchButton = Dashboard::CreateComponent<Button>("BO3Enhanced", Dashboard::Window);
    launchButton->OnPress += [=]
    {
        Runner::OnLaunchButtonPress(launchButton);
    };
    Dashboard::Layout->addWidget(launchButton);

    QHBoxLayout* rowLayout = new QHBoxLayout(Dashboard::Window);
    QVBoxLayout* leftColumnLayout = new QVBoxLayout(Dashboard::Window);
    QVBoxLayout* rightColumnLayout = new QVBoxLayout(Dashboard::Window);
    QHBoxLayout* rightRowLayout = new QHBoxLayout(Dashboard::Window);

    QLabel* gamePathLabel = new QLabel;
    gamePathLabel->setText("Game Path");
    gamePathLabel->setFont(QFont("Jetbrains Mono NL Semibold", 10));
    gamePathLabel->setAlignment(Qt::AlignmentFlag::AlignHCenter);
    InputField* gamePathInputField =
        Dashboard::CreateComponent<InputField>(Dashboard::GamePath.c_str(), Dashboard::Window);
    gamePathInputField->OnSubmit += [=]
    {
        Dashboard::GamePath = gamePathInputField->text().toUtf8().constData();
        Dashboard::GamePath.erase(std::ranges::remove(Dashboard::GamePath, '\"').begin(), Dashboard::GamePath.end());

        gamePathInputField->setText(Dashboard::GamePath.c_str());
        gamePathInputField->update();

        Dashboard::Settings->setValue("GamePath", Dashboard::GamePath.c_str());

    };
    gamePathInputField->setTextMargins(0, 0, 32, 0);

    BrowseButton* gameBrowseButton = Dashboard::CreateComponent<BrowseButton>("Browse for game", gamePathInputField);
    gamePathInputField->OnResize += [=]
    {
        gameBrowseButton->move(
            gamePathInputField->width() - gameBrowseButton->width() - 4,
            (gamePathInputField->height() - gameBrowseButton->height()) / 2);
    };
    gameBrowseButton->OnFileSelect += [=](const QString& acPath)
    {
        gamePathInputField->setText(acPath);
    };

    QObject::connect(
        gamePathInputField, &QLineEdit::textChanged,
        [=](const QString&)
        {
            gameBrowseButton->move(
                gamePathInputField->width() - gameBrowseButton->width() - 4,
                (gamePathInputField->height() - gameBrowseButton->height()) / 2);
        });

    leftColumnLayout->addWidget(gamePathLabel);
    leftColumnLayout->addWidget(gamePathInputField);

    QLabel* dllPathLabel = new QLabel;
    dllPathLabel->setText("DLL Path");
    dllPathLabel->setFont(QFont("Jetbrains Mono NL Semibold", 10));
    dllPathLabel->setAlignment(Qt::AlignmentFlag::AlignHCenter);
    InputField* dllPathInputField =
        Dashboard::CreateComponent<InputField>(Dashboard::DllPath.c_str(), Dashboard::Window);
    dllPathInputField->OnSubmit += [=]
    {
        Dashboard::DllPath = dllPathInputField->text().toUtf8().constData();
        Dashboard::DllPath.erase(std::ranges::remove(Dashboard::DllPath, '\"').begin(), Dashboard::DllPath.end());

        dllPathInputField->setText(Dashboard::DllPath.c_str());
        dllPathInputField->update();

        Dashboard::Settings->setValue("DllPath", Dashboard::DllPath.c_str());
    };
    dllPathInputField->setTextMargins(0, 0, 32, 0);

    BrowseButton* dllBrowseButton = Dashboard::CreateComponent<BrowseButton>("Browse for dll", dllPathInputField);
    dllPathInputField->OnResize += [=]
    {
        dllBrowseButton->move(
            dllPathInputField->width() - dllBrowseButton->width() - 4,
            (dllPathInputField->height() - dllBrowseButton->height()) / 2);
    };
    dllBrowseButton->OnFileSelect += [=](const QString& acPath)
    {
        dllPathInputField->setText(acPath);
    };

    QObject::connect(
        dllPathInputField, &QLineEdit::textChanged,
        [=](const QString&)
        {
            dllBrowseButton->move(
                dllPathInputField->width() - dllBrowseButton->width() - 4,
                (dllPathInputField->height() - dllBrowseButton->height()) / 2);
        });

    rightColumnLayout->addWidget(dllPathLabel);
    rightColumnLayout->addWidget(dllPathInputField);

    rowLayout->addLayout(leftColumnLayout);
    rowLayout->addLayout(rightColumnLayout);
    Dashboard::Layout->addLayout(rowLayout);
    Dashboard::Layout->addStretch();
}

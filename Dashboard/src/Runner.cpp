#include <filesystem>
#include <windows.h>
#include <shellapi.h> // <-- randomly broke and now requires windows.h immediately before ???

#include "Runner.h"
#include "Dashboard.h"
#include "Log.h"
#include "Button.h"
#include "BrowseButton.h"
#include "InputField.h"
#include "Event.h"
#include "CommandEvent.h"

#include <QMenuBar>
#include <QApplication>
#include <QPointer>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

#include "NotificationManager.h"

static CommandEvent* g_commandEventSender = nullptr;

// something broke and now it doesnt like wWinMain, so now main
int main(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ const LPWSTR lpCmdLine, _In_ int)
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

    if (const auto cPath = Dashboard::GamePath.c_str();
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

            /*std::thread([&]
            {
                g_commandEventSender = new CommandEvent(IPC::Client, "BO3TK_dll");
            }).detach();*/
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
    // Icon
    Dashboard::SetIcon(QIcon(":/BO3TK.ico"));

    // Settings
    Dashboard::GamePath = Dashboard::Settings->value("GamePath", GAME_PATH).toString().toUtf8().constData();
    Dashboard::DllPath = Dashboard::Settings->value("DllPath", DLL_PATH).toString().toUtf8().constData();

    // Layouts
    const QPointer cRowLayout = new QHBoxLayout(Dashboard::Window);
    const QPointer cLeftColumnLayout = new QVBoxLayout(Dashboard::Window);
    const QPointer cRightColumnLayout = new QVBoxLayout(Dashboard::Window);

    // BO3E button
    const QPointer cLaunchButton = Dashboard::CreateComponent<Button>("BO3Enhanced", Dashboard::Window);
    cLaunchButton->OnPress += [=]
    {
        Runner::OnLaunchButtonPress(cLaunchButton);
    };
    Dashboard::Layout->addWidget(cLaunchButton);

    // Game Path
    const QPointer cGamePathLabel = new QLabel;
    cGamePathLabel->setText("Game Path");
    cGamePathLabel->setStyleSheet(R"(color: #ffffff)");
    cGamePathLabel->setFont(QFont("Jetbrains Mono NL Semibold", 10));
    cGamePathLabel->setAlignment(Qt::AlignmentFlag::AlignHCenter);
    QPointer gamePathInputField =
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

    const QPointer cGameBrowseButton = Dashboard::CreateComponent<BrowseButton>("Browse for game", gamePathInputField);
    gamePathInputField->OnResize += [=]
    {
        cGameBrowseButton->move(
            gamePathInputField->width() - cGameBrowseButton->width() - 4,
            (gamePathInputField->height() - cGameBrowseButton->height()) / 2);
    };
    cGameBrowseButton->OnFileSelect += [=](const QString& acPath)
    {
        gamePathInputField->setText(acPath);
    };

    QObject::connect(
        gamePathInputField, &QLineEdit::textChanged,
        [=](const QString&)
        {
            cGameBrowseButton->move(
                gamePathInputField->width() - cGameBrowseButton->width() - 4,
                (gamePathInputField->height() - cGameBrowseButton->height()) / 2);
        });

    cLeftColumnLayout->addWidget(cGamePathLabel);
    cLeftColumnLayout->addWidget(gamePathInputField);

    // DLL Path
    const QPointer cDllPathLabel = new QLabel;
    cDllPathLabel->setText("DLL Path");
    cDllPathLabel->setStyleSheet(R"(color: #ffffff)");
    cDllPathLabel->setFont(QFont("Jetbrains Mono NL Semibold", 10));
    cDllPathLabel->setAlignment(Qt::AlignmentFlag::AlignHCenter);
    QPointer dllPathInputField = Dashboard::CreateComponent<InputField>(Dashboard::DllPath.c_str(), Dashboard::Window);
    dllPathInputField->OnSubmit += [=]
    {
        Dashboard::DllPath = dllPathInputField->text().toUtf8().constData();
        Dashboard::DllPath.erase(std::ranges::remove(Dashboard::DllPath, '\"').begin(), Dashboard::DllPath.end());

        dllPathInputField->setText(Dashboard::DllPath.c_str());
        dllPathInputField->update();

        Dashboard::Settings->setValue("DllPath", Dashboard::DllPath.c_str());
    };
    dllPathInputField->setTextMargins(0, 0, 32, 0);

    const QPointer cDllBrowseButton = Dashboard::CreateComponent<BrowseButton>("Browse for dll", dllPathInputField);
    dllPathInputField->OnResize += [=]
    {
        cDllBrowseButton->move(
            dllPathInputField->width() - cDllBrowseButton->width() - 4,
            (dllPathInputField->height() - cDllBrowseButton->height()) / 2);
    };
    cDllBrowseButton->OnFileSelect += [=](const QString& acPath)
    {
        dllPathInputField->setText(acPath);
    };

    QObject::connect(
        dllPathInputField, &QLineEdit::textChanged,
        [=](const QString&)
        {
            cDllBrowseButton->move(
                dllPathInputField->width() - cDllBrowseButton->width() - 4,
                (dllPathInputField->height() - cDllBrowseButton->height()) / 2);
        });

    cRightColumnLayout->addWidget(cDllPathLabel);
    cRightColumnLayout->addWidget(dllPathInputField);

    cRowLayout->addLayout(cLeftColumnLayout);
    cRowLayout->addLayout(cRightColumnLayout);
    Dashboard::Layout->addLayout(cRowLayout);
    Dashboard::Layout->addStretch();

    const QPointer cSentEventsLayout = new QVBoxLayout(Dashboard::Window);
    Dashboard::Layout->addLayout(cSentEventsLayout);

    const QPointer cEventLayout = new QHBoxLayout(Dashboard::Window);
    QPointer sendEventInputField = Dashboard::CreateComponent<InputField>("", Dashboard::Window);
    sendEventInputField->setAlignment(Qt::AlignBottom);
    QObject::connect(
        sendEventInputField, &QLineEdit::returnPressed,
        [sendEventInputField, cSentEventsLayout]
        {
            if (sendEventInputField->text().isEmpty())
                return;

            const QPointer cEventText = new QLabel(sendEventInputField->text());
            cEventText->setStyleSheet(R"(color: #ffffff)");
            cEventText->setFont(QFont("Jetbrains Mono NL Semibold", 10));
            cEventText->setFixedHeight(16);
            cEventText->setAlignment(Qt::AlignBottom);
            cSentEventsLayout->addWidget(cEventText);

            auto* opacityEffect = new QGraphicsOpacityEffect(cEventText);
            cEventText->setGraphicsEffect(opacityEffect);
            opacityEffect->setOpacity(1.0);

            auto* fadeAnim = new QPropertyAnimation(opacityEffect, "opacity", cEventText);
            fadeAnim->setDuration(1000);
            fadeAnim->setStartValue(1.0);
            fadeAnim->setEndValue(0.0);
            fadeAnim->setEasingCurve(QEasingCurve::Linear);

            QTimer::singleShot(3000, [fadeAnim]() { fadeAnim->start(); });

            QObject::connect(fadeAnim, &QPropertyAnimation::finished, cEventText, &QWidget::deleteLater);

            if (g_commandEventSender)
            {
                const std::string cToSend = std::string(sendEventInputField->text().toUtf8().constData());
                g_commandEventSender->Send(*new std::string(cToSend));
            }

            NotificationManager::ShowNotification("Command sent: " + sendEventInputField->text(), Dashboard::Window);
            sendEventInputField->setText("");
        });
    const QPointer cSendEventButton = Dashboard::CreateComponent<Button>("Send", Dashboard::Window);
    cSendEventButton->setFixedWidth(50);
    cSendEventButton->OnPress += [sendEventInputField]
    {
        sendEventInputField->returnPressed();
    };

    cEventLayout->addWidget(sendEventInputField);
    cEventLayout->addWidget(cSendEventButton);
    Dashboard::Layout->addLayout(cEventLayout);
}

#include <filesystem>
#include <windows.h>
#include <shellapi.h>

#include "Runner.h"
#include "Dashboard.h"
#include "Log.h"
#include "Button.h"
#include "BrowseButton.h"
#include "InputField.h"
#include "Injector.h"
#include "NotificationManager.h"
#include "HookEvent.h"
#include "EventHandler.h"

#include <QMenuBar>
#include <QPointer>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QCheckbox>

int Runner::Start(const LPWSTR apcCmdLine)
{
    auto [argc, argv] = ParseCommandLine(apcCmdLine);
    Dashboard::Init(argc, argv.data());

    std::thread(
        [&]()
        {
            m_eventHandler = std::make_unique<EventHandler>("BO3TK_dll", "BO3TK_exe", true);
            while (!m_eventHandler->Run())
            {
                Log::Get()->Error("Disconnected from EventHandler...");
                std::this_thread::sleep_for(std::chrono::milliseconds(m_eventHandler->SleepDuration));
            }
        })
        .detach();

    CreateDashboardComponents();

    return Dashboard::Run();
}

CommandLineArgs Runner::ParseCommandLine(const LPWSTR apcCmdLine)
{
    CommandLineArgs cli{};

    LPWSTR* pArgvW = CommandLineToArgvW(apcCmdLine, &cli.Argc);

    std::vector<std::string> narrowArgs;
    narrowArgs.resize(cli.Argc);

    for (int i = 0; i < cli.Argc; ++i)
    {
        if (const int cLen = WideCharToMultiByte(CP_UTF8, 0, pArgvW[i], -1, nullptr, 0, nullptr, nullptr); cLen > 0)
        {
            std::string narrow(cLen, '\0');
            WideCharToMultiByte(CP_UTF8, 0, pArgvW[i], -1, narrow.data(), cLen, nullptr, nullptr);
            narrowArgs.push_back(std::move(narrow));
            cli.Argv.push_back(narrowArgs.back().data());
        }
    }

    LocalFree(pArgvW);

    return cli;
}

void Runner::OnLaunchButtonPress(Button* apButton)
{
    STARTUPINFO sInfo{};
    PROCESS_INFORMATION pInfo{};

    if (const auto cPath = Dashboard::GamePath.c_str();
        CreateProcessA(cPath, nullptr, nullptr, nullptr, FALSE, CREATE_SUSPENDED, nullptr, nullptr, &sInfo, &pInfo))
    {
        if (Injector::Inject(pInfo.dwProcessId, DLL_PATH))
        {
            apButton->AnimateColors(QColor("#1e5e3d"), QColor("#e0f4e9"));

            std::thread(
                [hProcess = pInfo.hProcess, hThread = pInfo.hThread, apButton]
                {
                    DWORD exitCode = STILL_ACTIVE;

                    while (exitCode == STILL_ACTIVE)
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds(2500));
                        if (!GetExitCodeProcess(hProcess, &exitCode)) { break; }
                    }

                    QMetaObject::invokeMethod(
                        apButton,
                        [=]
                        {
                            if (!apButton->IsAnimating()) { apButton->AnimateToOriginal(); }
                        },
                        Qt::QueuedConnection);

                    CloseHandle(hProcess);
                    CloseHandle(hThread);
                })
                .detach();

            NotificationManager::ShowNotification(QString("Successfully launched & injected"), Dashboard::Window);
        }
        else
        {
            apButton->AnimateColors(QColor("#6e1e1e"), QColor("#ff8e8e"));

            QTimer::singleShot(3000, apButton, [apButton]() { AnimateButton(apButton); });
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

        QTimer::singleShot(3000, apButton, [apButton]() { AnimateButton(apButton); });
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
    cLaunchButton->OnPress += [=] { OnLaunchButtonPress(cLaunchButton); };
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
    cGameBrowseButton->OnFileSelect += [=](const QString& acPath) { gamePathInputField->setText(acPath); };

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
    cDllBrowseButton->OnFileSelect += [=](const QString& acPath) { dllPathInputField->setText(acPath); };

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

    // Hooks
    const QPointer cHooksLabel = new QLabel("Function Hooks");
    cHooksLabel->setStyleSheet("color: #ffffff");
    cHooksLabel->setFont(QFont("Jetbrains Mono NL Semibold", 10));
    cHooksLabel->setAlignment(Qt::AlignHCenter);

    const QPointer cHooksLayout = new QVBoxLayout(Dashboard::Window);
    cHooksLayout->addWidget(cHooksLabel);

    m_hookButtons.push_back(new HookButton<>{new FuncHook("Com_PrintMessage", 0x220F4F0), nullptr});
    m_hookButtons.push_back(new HookButton<>{new FuncHook("Com_Error", 0x13DF170), nullptr});
    m_hookButtons.push_back(new HookButton<>{new FuncHook("Com_HashString", 0x2210B90), nullptr});
    m_hookButtons.push_back(new HookButton<>{new FuncHook("Dvar_SetFromStringByName", 0x23ABA90), nullptr});
    m_hookButtons.push_back(new HookButton<>{new FuncHook("Dvar_RegisterNew", 0x23A6870), nullptr});
    m_hookButtons.push_back(new HookButton<>{new FuncHook("RegisterLuaEnums", 0x200BCC0), nullptr});
    m_hookButtons.push_back(new HookButton<>{new FuncHook("lua_pcall", 0x1E54EF0), nullptr});
    m_hookButtons.push_back(new HookButton<>{new FuncHook("PLmemcpy", 0x23B46F0), nullptr});
    m_hookButtons.push_back(new HookButton<>{new LibHook("user32.dll", "MessageBoxA"), nullptr});

    for (HookButton<>* button : m_hookButtons)
    {
        button->Checkbox = new QCheckBox(button->Hook->Name.c_str(), Dashboard::Window);
        button->Checkbox->setChecked(button->Hook->Enabled);
        button->Checkbox->setStyleSheet(R"(
            QCheckBox {
                color: #ffffff;
                font-family: "JetBrains Mono NL";
                font-size: 14px;
                spacing: 8px;
            }
            QCheckBox:hover { color: #b0b0b0; }
            QCheckBox::indicator {
                width: 18px;
                height: 18px;
                border-radius: 4px;
                background-color: #2c2c2c;
                border: 1px solid #444444;
            }
            QCheckBox::indicator:hover {
                border: 1px solid #5a9bd5;
            }
            QCheckBox::indicator:checked {
                background-color: #5a9bd5;
            }
        )");

        QObject::connect(
            button->Checkbox, &QCheckBox::toggled,
            [this, button](bool aChecked)
            {
                if (!m_eventHandler || !m_eventHandler->WritePipe->Connected) return;

                HookPayload payload{.Type = button->Hook->Type, .Enabled = aChecked};

                strncpy_s(payload.FuncName, button->Hook->Name.data(), button->Hook->Name.length());

                if (payload.Type == HookType::Library)
                {
                    const LibHook* cpLib = dynamic_cast<LibHook*>(button->Hook);
                    strncpy_s(payload.LibName, cpLib->LibName.data(), cpLib->Name.length());
                }

                m_eventHandler->Send(HookEvent(payload));
            });

        cHooksLayout->addWidget(button->Checkbox);
    }

    Dashboard::Layout->addLayout(cHooksLayout);
    Dashboard::Layout->addStretch();

    RECT rect{};
    const auto cDashboardWindow = GetActiveWindow();
    GetWindowRect(cDashboardWindow, &rect);
    const auto cDashboardHeight = rect.bottom - rect.top;

    SetWindowPos(GetConsoleWindow(), nullptr, 0, cDashboardHeight, 0, 0, SWP_NOSIZE);
    SetWindowPos(cDashboardWindow, nullptr, 0, 0, 0, 0, SWP_NOSIZE);
}

void Runner::AnimateButton(Button* apButton)
{
    if (!apButton->IsAnimating()) { apButton->AnimateToOriginal(); }
}

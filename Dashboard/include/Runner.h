#pragma once
#include <qcheckbox.h>

#include "EventHandler.h"
#include "HookEvent.h"

class Button;
class QCheckBox;

struct CommandLineArgs
{
    int Argc;
    std::vector<char*> Argv;
};

struct HookButton
{
    HookButton() = default;
    explicit HookButton(FuncHook* apHook = nullptr, QCheckBox* apCheckbox = nullptr) : Hook(apHook), Checkbox(apCheckbox) {}

    FuncHook* Hook = nullptr;
    QCheckBox* Checkbox = nullptr;
};

struct Runner
{
    static CommandLineArgs ParseCommandLine(LPWSTR acpCmdLine = nullptr);
    int Start(LPWSTR acpCmdLine = nullptr);
    void CreateDashboardComponents();
    void OnLaunchButtonPress(Button* apButton = nullptr);
    static void AnimateButton(Button* apButton = nullptr);

    std::unique_ptr<EventHandler> m_eventHandler{ new EventHandler("BO3TK_dll") };
    std::vector<HookButton*> m_hookButtons{};
};
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

template <typename Type = IHook> struct HookButton
{
    HookButton() = default;
    explicit HookButton(Type* apHook, QCheckBox* apCheckbox = nullptr)
        : Hook(apHook)
        , Checkbox(apCheckbox)
    {
    }

    Type* Hook{nullptr};
    QCheckBox* Checkbox{nullptr};
};

struct Runner
{
    static CommandLineArgs ParseCommandLine(LPWSTR acpCmdLine = nullptr);
    int Start(LPWSTR acpCmdLine = nullptr);
    void CreateDashboardComponents();
    static void OnLaunchButtonPress(Button* apButton = nullptr);
    static void AnimateButton(Button* apButton = nullptr);

    std::unique_ptr<EventHandler> pEventHandler{};
private:
    std::vector<HookButton<IHook>*> m_hookButtons{};
};
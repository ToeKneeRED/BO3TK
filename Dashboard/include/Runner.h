#pragma once
#include "EventHandler.h"

class Button;

struct CommandLineArgs
{
    int Argc;
    std::vector<char*> Argv;
};

namespace Runner
{
static CommandLineArgs ParseCommandLine(LPWSTR apcCmdLine = nullptr);

void CreateDashboardComponents();
void OnLaunchButtonPress(Button* apButton = nullptr);

inline std::unique_ptr<EventHandler> g_eventHandler{ new EventHandler("BO3TK_dll") };
} // namespace Runner
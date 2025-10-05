#pragma once

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
} // namespace Runner
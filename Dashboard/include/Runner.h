#pragma once
#include "framework.h"

class Button;

namespace Runner
{

BOOL Inject(DWORD acProcessId, LPCSTR apDllPath);

void CreateDashboardComponents();
void OnLaunchButtonPress(Button* apButton);
} // namespace Runner

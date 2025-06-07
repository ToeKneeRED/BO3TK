#pragma once
#include <Button.h>
#include <QVBoxLayout>
#include <QTimer>

struct Dashboard
{
    static void Init(int32_t aArgc = 0, char** aArgv = nullptr);
    static int Run();
    static void AddButton(const std::string& acName, Button* apButton);

    static QApplication* App;
    static QWidget* Window;
    static QVBoxLayout* Layout;

    static std::unordered_map<std::string, Button*> Buttons;
};

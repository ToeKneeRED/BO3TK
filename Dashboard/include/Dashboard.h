#pragma once
#include "InputField.h"
#include <Button.h>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>

struct Dashboard
{
    static void Init(int32_t aArgc = 0, char** aArgv = nullptr);
    static int Run();

    static void AddButton(const char* acName, Button* apButton);
    static Button* CreateButton(const char* acName, const char* acText = "");
    static void AddInputField(const char* acName, InputField* apInputField);
    static InputField* CreateInputField(const char* acName, const char* acText = "");

    static QApplication* App;
    static QWidget* Window;
    static QVBoxLayout* Layout;
    static QIcon* Icon;

    static std::string GamePath;
    static std::string DllPath;

    static std::unordered_map<const char*, Button*> Buttons;
    static std::unordered_map<const char*, InputField*> InputFields;
};

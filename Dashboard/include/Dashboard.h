#pragma once
#include "InputField.h"
#include <Button.h>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QSettings>

struct Dashboard
{
    static void Init(int32_t aArgc = 0, char** aArgv = nullptr);
    static int Run();

    static void SetIcon(const QIcon& acIcon) { Window->setWindowIcon(acIcon); }
    static void AddButton(const char* acName, Button* apButton);
    static Button* CreateButton(const char* acName, const char* acText = "");
    static void AddInputField(const char* acName, InputField* apInputField);
    static InputField* CreateInputField(const char* acName, const char* acText = "");

    static inline QApplication* App;
    static inline QWidget* Window;
    static inline QVBoxLayout* Layout;
    static inline QIcon* Icon;
    static inline QSettings* Settings;

    static inline std::string GamePath;
    static inline std::string DllPath;

    static inline std::unordered_map<const char*, Button*> Buttons;
    static inline std::unordered_map<const char*, InputField*> InputFields;
};

#pragma once
#include <Component.h>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QSettings>

struct Dashboard
{
    static void Init(int32_t aArgc = 0, char** apArgv = nullptr);
    static int Run();

    static void SetIcon(const QIcon& acIcon) { Window->setWindowIcon(acIcon); }

    template <typename T, typename... Args> static T* CreateComponent(Args&&... aArgs)
    {
        T* component = new T(std::forward<Args>(aArgs)...);
        Components.push_back(component);

        return component;
    }

    static inline QApplication* App;
    static inline QWidget* Window;
    static inline QVBoxLayout* Layout;
    static inline QIcon* Icon;
    static inline QSettings* Settings;

    static inline std::string GamePath;
    static inline std::string DllPath;

    static inline std::vector<IComponent*> Components;
};

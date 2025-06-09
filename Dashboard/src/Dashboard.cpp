#include "Dashboard.h"
#include "Log.h"
#include <QApplication>
#include <QScreen>

void Dashboard::Init(int32_t aArgc, char** aArgv)
{
    App = new QApplication(aArgc, aArgv);

    Settings = new QSettings("Dashboard.ini", QSettings::IniFormat);

    Window = new QWidget;
    Window->setWindowTitle("Dashboard");
    Window->resize(900, 600);
    Window->setFocusPolicy(Qt::ClickFocus);
    if (const QScreen* primaryScreen = QGuiApplication::primaryScreen())
    {
        const QRect cScreenGeometry = primaryScreen->availableGeometry();
        const int& cX = cScreenGeometry.x() + (cScreenGeometry.width() - Window->width()) / 2;
        const int& cY = cScreenGeometry.y() + (cScreenGeometry.height() - Window->height()) / 2;

        Window->move(cX, cY);
    }

    Layout = new QVBoxLayout(Window);

    Window->setLayout(Layout);
    Window->show();
}

int Dashboard::Run()
{
    return QApplication::exec();
}

void Dashboard::AddButton(const char* acName, Button* apButton)
{
    Buttons.insert({acName, apButton});

    Layout->addWidget(apButton);
}

Button* Dashboard::CreateButton(const char* acName, const char* acText)
{
    Button* button = new Button(acText, Window);
    AddButton(acName, button);

    return button;
}

void Dashboard::AddInputField(const char* acName, InputField* apInputField)
{
    InputFields.insert({acName, apInputField});

    Layout->addWidget(apInputField);
}

InputField* Dashboard::CreateInputField(const char* acName, const char* acText)
{
    InputField* inputField = new InputField(acText, Window);
    AddInputField(acName, inputField);

    return inputField;
}
#include "Dashboard.h"
#include "Log.h"
#include <QApplication>
#include <QScreen>

void Dashboard::Init(int32_t aArgc, char** apArgv)
{
    App = new QApplication(aArgc, apArgv);

    Settings = new QSettings("Dashboard.ini", QSettings::IniFormat);

    Window = new QWidget;
    Window->setObjectName("Window");
    Window->setWindowTitle("Dashboard");
    Window->resize(500, 700);
    Window->setFocusPolicy(Qt::ClickFocus);
    Window->setStyleSheet(R"(
    QWidget#Window {
        background-color: #1e1e1e;
        color: #ffffff;
        font-family: "Jetbrains Mono NL";
        font-size: 14px;
    })");

    Layout = new QVBoxLayout(Window);

    Window->setLayout(Layout);
    Window->show();
}

int Dashboard::Run()
{
    return QApplication::exec();
}

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
    Window->resize(900, 750);
    Window->setFocusPolicy(Qt::ClickFocus);
    if (const QScreen* primaryScreen = QGuiApplication::primaryScreen())
    {
        const QRect& cScreenGeometry = primaryScreen->availableGeometry();
        const int& cX = cScreenGeometry.x() + (cScreenGeometry.width() - Window->width()) / 2;
        const int& cY = cScreenGeometry.y() + (cScreenGeometry.height() - Window->height()) / 2;

        Window->move(cX, cY);
    }
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

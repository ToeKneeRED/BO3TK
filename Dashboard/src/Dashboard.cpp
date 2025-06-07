#include "Dashboard.h"
#include "Log.h"
#include "framework.h"
#include <QApplication>
#include <QScreen>
#include "Button.h"

QApplication* Dashboard::App = nullptr;
QWidget* Dashboard::Window = nullptr;
QVBoxLayout* Dashboard::Layout = nullptr;
std::unordered_map<std::string, Button*> Dashboard::Buttons{};

void Dashboard::Init(int32_t aArgc, char** aArgv)
{
    App = new QApplication(aArgc, aArgv);

    Window = new QWidget;
    Window->setWindowTitle("Dashboard");
    Window->resize(900, 600);

    if (const QScreen* primaryScreen = QGuiApplication::primaryScreen())
    {
        const QRect cScreenGeometry = primaryScreen->availableGeometry();
        const int& cX = cScreenGeometry.x() + (cScreenGeometry.width() - Window->width()) / 2;
        const int& cY = cScreenGeometry.y() + (cScreenGeometry.height() - Window->height()) / 2;

        Window->move(cX, cY);
    }

    Layout = new QVBoxLayout(Window);
    AddButton("BO3Enhanced", new Button("BO3Enhanced", Window));
    Layout->addStretch();

    Window->setLayout(Layout);
    Window->show();
}

int Dashboard::Run()
{
    return QApplication::exec();
}

void Dashboard::AddButton(const std::string& acName, Button* apButton)
{
    Buttons.insert({acName, apButton});

    Layout->addWidget(apButton);
}


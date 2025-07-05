#pragma once
#include "Component.h"

#include <qwidget.h>

struct Notification : Component<QWidget>
{
    Q_OBJECT
public:
    Notification(const QString& acMessage, QWidget* apParent = nullptr);

    void Show(int acDuration = 3000);
};

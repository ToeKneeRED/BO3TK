#pragma once
#include <QVBoxLayout>
#include <qwidget.h>

#include "Component.h"
#include "Notification.h"

struct NotificationContainer final : Component<QWidget>
{
    static NotificationContainer* Instance(QWidget* apParent = nullptr);

    void Add(Notification* apNotification);

private:
    explicit NotificationContainer(QWidget* apParent = nullptr);
    QVBoxLayout* m_layout = nullptr;
};

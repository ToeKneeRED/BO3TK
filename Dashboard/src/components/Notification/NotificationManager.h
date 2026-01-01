#pragma once
#include <qstring.h>
#include <qwidget.h>
#include <QScreen>

#include "Notification.h"
#include "NotificationContainer.h"

struct NotificationManager
{
    static void ShowNotification(const QString& acMessage, QWidget* apParent = nullptr)
    {
        auto* notification = new Notification(acMessage);
        NotificationContainer::Instance(apParent)->Add(notification);
        notification->Show(1500);
    }

private:
    QList<Notification*> s_activeNotifications;
    int s_nextNotificationYOffset = 0;
    const int m_notificationSpacing = 8;
};

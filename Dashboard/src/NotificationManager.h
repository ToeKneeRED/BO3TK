#pragma once
#include <qstring.h>
#include <qwidget.h>
#include <QGuiApplication>
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
    static inline QList<Notification*> activeNotifications;
    static inline int nextNotificationYOffset = 0;
    static constexpr int notificationSpacing = 8;
};

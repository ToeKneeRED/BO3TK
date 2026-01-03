#include "NotificationContainer.h"
#include <QGuiApplication>
#include <QScreen>

NotificationContainer::NotificationContainer(QWidget* apParent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);
    setAttribute(Qt::WA_TransparentForMouseEvents);

    m_layout = new QVBoxLayout(this);
    m_layout->setSpacing(8);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setAlignment(Qt::AlignBottom | Qt::AlignRight);
    setLayout(m_layout);

    const QRect cScreenRect = QGuiApplication::primaryScreen()->availableGeometry();
    move(cScreenRect.right() - 300, cScreenRect.bottom() - 200);
    resize(300, 200);
}

NotificationContainer* NotificationContainer::Instance(QWidget* apParent)
{
    static NotificationContainer* instance = new NotificationContainer(apParent);

    if (!instance->isVisible()) instance->show();

    if (apParent)
    {
        const QRect cParentRect = apParent->geometry();
        instance->move(cParentRect.right() - instance->width() - 10, cParentRect.bottom() - instance->height() - 10);
    }

    return instance;
}

void NotificationContainer::Add(Notification* apNotification)
{
    m_layout->addWidget(apNotification);

    connect(
        apNotification, &QObject::destroyed, this, [this, apNotification] { m_layout->removeWidget(apNotification); });
}
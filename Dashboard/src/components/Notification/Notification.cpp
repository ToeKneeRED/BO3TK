#include "Notification.h"

#include <QVBoxLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsOpacityEffect>
#include <QLabel>
#include <QPropertyAnimation>
#include <QTimer>

Notification::Notification(const QString& acMessage, QWidget* apParent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 5, 10, 5);

    auto* label = new QLabel(acMessage);
    label->setStyleSheet(R"(color: white; background-color: rgba(50, 50, 50, 200); padding: 5px; border-radius: 4px;)");
    label->setWordWrap(true);

    layout->addWidget(label);

    setLayout(layout);
    adjustSize();
}

void Notification::Show(const int acDuration)
{
    auto* effect = new QGraphicsOpacityEffect(this);
    setGraphicsEffect(effect);
    effect->setOpacity(0);

    auto* fadeIn = new QPropertyAnimation(effect, "opacity", this);
    fadeIn->setDuration(500);
    fadeIn->setStartValue(0.0);
    fadeIn->setEndValue(1.0);

    auto* fadeOut = new QPropertyAnimation(effect, "opacity", this);
    fadeOut->setDuration(500);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.0);
    QTimer::singleShot(acDuration, fadeOut, [fadeOut] { fadeOut->start(QAbstractAnimation::DeleteWhenStopped); });

    fadeIn->start(QAbstractAnimation::DeleteWhenStopped);

    connect(fadeOut, &QPropertyAnimation::finished, this, &QWidget::deleteLater);

    show();
}
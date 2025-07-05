#include "Button.h"
#include <QPainter>
#include <QTimer>
#include <QPropertyAnimation>
#include <QStyleOptionButton>

Button::Button(const QString& acText, QWidget* apParent)
{
    setText(acText);
    setParent(apParent);
    setFont(QFont("JetBrains Mono NL Semibold", 14));
    setCursor(Qt::PointingHandCursor);
    setFlat(true);
    setAutoFillBackground(false);
    setObjectName("Button");
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    setStyleSheet(R"(
        QPushButton#Button {
            background-color: #2c2c2c;
            color: #ffffff;
            border: none;
            border-radius: 6px;
            padding: 8px 16px;
            font-size: 14px;
            font-family: "Jetbrains Mono NL Semibold";
        }
        QPushButton#Button:hover {
            background-color: #3c3c3c;
        }
        QPushButton#Button:pressed {
            background-color: #ffffff;
        }
    )");

    m_backgroundAnimation = new QPropertyAnimation(this, "backgroundColor");
    m_backgroundAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    connect(m_backgroundAnimation, &QAbstractAnimation::finished, [this] { this->m_isBackgroundAnimating = false; });

    m_textAnimation = new QPropertyAnimation(this, "textColor");
    m_textAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    connect(m_textAnimation, &QAbstractAnimation::finished, [this] { this->m_isTextAnimating = false; });

    show();
}

void Button::SetBackgroundColor(const QColor& acColor)
{
    if (acColor != m_backgroundColor)
    {
        m_backgroundColor = acColor;
        update();
    }
}

void Button::SetTextColor(const QColor& acColor)
{
    if (acColor != m_textColor)
    {
        m_textColor = acColor;
        update();
    }
}

void Button::AnimateColors(const QColor& acBgTarget, const QColor& acTextTarget, const int acDuration)
{
    AnimateBackground(acBgTarget, acDuration);
    AnimateText(acTextTarget, acDuration);
}

void Button::AnimateBackground(const QColor& acTarget, const int acDuration)
{
    m_isBackgroundAnimating = true;

    if (m_backgroundAnimation->state() == QAbstractAnimation::Running)
        m_backgroundAnimation->stop();

    m_backgroundAnimation->setDuration(acDuration);
    m_backgroundAnimation->setEndValue(acTarget);

    m_backgroundAnimation->start(QAbstractAnimation::KeepWhenStopped);
}

void Button::AnimateText(const QColor& acTarget, const int acDuration)
{
    m_isTextAnimating = true;

    if (m_textAnimation->state() == QAbstractAnimation::Running)
        m_textAnimation->stop();

    m_textAnimation->setDuration(acDuration);
    m_textAnimation->setEndValue(acTarget);

    m_textAnimation->start(QAbstractAnimation::KeepWhenStopped);
}

void Button::AnimateToOriginal(const int acDuration)
{
    AnimateColors(BUTTON_BACKGROUND_COLOR, BUTTON_TEXT_COLOR, acDuration);
}

void Button::paintEvent(QPaintEvent* apEvent)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setBrush(m_backgroundColor);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), 8, 8);

    painter.setPen(m_textColor);
    painter.setFont(font());
    painter.drawText(rect(), Qt::AlignCenter, text());
}

void Button::mousePressEvent(QMouseEvent* apEvent)
{
    QPushButton::mousePressEvent(apEvent);
    OnPress();
}
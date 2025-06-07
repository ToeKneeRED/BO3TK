#include "Button.h"
#include <QPainter>
#include <QTimer>
#include <QPropertyAnimation>
#include <QStyleOptionButton>

Button::Button(const QString& text, QWidget* parent)
    : QPushButton(text, parent)
{
    setFont(QFont("JetBrains Mono NL Semibold", 14));
    setCursor(Qt::PointingHandCursor);
    setFlat(true);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    setStyleSheet(R"(
        QPushButton {
            background-color: #2c2c2c;
            color: #ffffff;
            border: none;
            border-radius: 6px;
            padding: 8px 16px;
            font-size: 14px;
            font-family: "Jetbrains Mono NL Semibold";
        }
        QPushButton:hover {
            background-color: #3c3c3c;
        }
        QPushButton:pressed {
            background-color: #1e1e1e;
        }
    )");
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

void Button::AnimateBackground(const QColor& acTarget, int acDuration)
{
    if (m_isBackgroundAnimating)
        return;

    m_isBackgroundAnimating = true;
    auto* bgAnim = new QPropertyAnimation(this, "backgroundColor");
    bgAnim->setDuration(acDuration);
    bgAnim->setStartValue(m_backgroundColor);
    bgAnim->setEndValue(acTarget);
    bgAnim->setEasingCurve(QEasingCurve::InOutCubic);

    connect(bgAnim, &QPropertyAnimation::finished, this, [this]() {
        QTimer::singleShot(1000, this, [this]() {
            m_isBackgroundAnimating = false;
        });
    });

    bgAnim->start(QAbstractAnimation::DeleteWhenStopped);
}

void Button::AnimateText(const QColor& acTarget, const int acDuration)
{
    if (m_isTextAnimating)
        return;

    m_isTextAnimating = true;
    auto* textAnim = new QPropertyAnimation(this, "textColor");
    textAnim->setDuration(acDuration);
    textAnim->setStartValue(m_textColor);
    textAnim->setEndValue(acTarget);
    textAnim->setEasingCurve(QEasingCurve::InOutCubic);

    connect(textAnim, &QPropertyAnimation::finished, this, [this]() {
        QTimer::singleShot(1000, this, [this]() {
            m_isTextAnimating = false;
        });
    });

    textAnim->start(QAbstractAnimation::DeleteWhenStopped);
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
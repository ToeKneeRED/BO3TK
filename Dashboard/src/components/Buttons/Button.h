#pragma once
#include "Handler.h"
#include "Component.h"
#include <QPushButton>

#define BUTTON_BACKGROUND_COLOR QColor("#2c2c2c")
#define BUTTON_TEXT_COLOR QColor("#ffffff")

class QPropertyAnimation;

struct Button final : Component<QPushButton>
{
    Q_OBJECT
    Q_PROPERTY(QColor backgroundColor READ BackgroundColor WRITE SetBackgroundColor)
    Q_PROPERTY(QColor textColor READ TextColor WRITE SetTextColor)

public:
    Handler<> OnPress;

    [[nodiscard]] QColor BackgroundColor() const { return m_backgroundColor; }
    void SetBackgroundColor(const QColor& acColor);
    [[nodiscard]] QColor TextColor() const { return m_textColor; }
    void SetTextColor(const QColor& acColor);

    void AnimateColors(const QColor& acBgTarget, const QColor& acTextTarget, int acDuration = 500);
    void AnimateBackground(const QColor& acTarget, int acDuration = 500);
    void AnimateText(const QColor& acTarget, int acDuration = 500);
    void AnimateToOriginal(int acDuration = 500);
    [[nodiscard]] bool IsAnimating() const { return m_isBackgroundAnimating || m_isTextAnimating; }

    explicit Button(const QString& acText, QWidget* acParent = nullptr);

protected:
    void paintEvent(QPaintEvent* apEvent) override;
    void mousePressEvent(QMouseEvent* apEvent) override;

private:
    QColor m_backgroundColor = BUTTON_BACKGROUND_COLOR;
    QColor m_textColor = BUTTON_TEXT_COLOR;
    QPropertyAnimation* m_backgroundAnimation = nullptr;
    QPropertyAnimation* m_textAnimation = nullptr;
    bool m_isBackgroundAnimating = false;
    bool m_isTextAnimating = false;
};
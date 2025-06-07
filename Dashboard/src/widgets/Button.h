#pragma once
#include <QPushButton>

#define BUTTON_BACKGROUND_COLOR QColor("#2c2c2c")
#define BUTTON_TEXT_COLOR QColor("#ffffff")

struct Button final : QPushButton
{
    Q_OBJECT
    Q_PROPERTY(QColor backgroundColor READ BackgroundColor WRITE SetBackgroundColor)
    Q_PROPERTY(QColor textColor READ TextColor WRITE SetTextColor)

public:
    struct PressHandler
    {
	void operator+=(std::function<void()> func)
	{
	    m_funcs.push_back(std::move(func));
	}

	void operator()() const
        {
	    for (auto& cFunc : m_funcs)
	        if (cFunc) cFunc();
	}

    private:
	std::vector<std::function<void()>> m_funcs;
    };

    PressHandler OnPress;

    [[nodiscard]] QColor BackgroundColor() const { return m_backgroundColor; }
    void SetBackgroundColor(const QColor& acColor);
    [[nodiscard]] QColor TextColor() const { return m_textColor; }
    void SetTextColor(const QColor& acColor);
    void AnimateColors(const QColor& acBgTarget, const QColor& acTextTarget, int acDuration = 500);
    void AnimateBackground(const QColor& acTarget, int duration = 500);
    void AnimateText(const QColor& acTarget, int acDuration = 500);
    void AnimateToOriginal(int acDuration = 500);

    explicit Button(const QString& text, QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* apEvent) override;
    void mousePressEvent(QMouseEvent* apEvent) override;

private:
    QColor m_backgroundColor = BUTTON_BACKGROUND_COLOR;
    QColor m_textColor = BUTTON_TEXT_COLOR;

    bool m_isBackgroundAnimating = false;
    bool m_isTextAnimating = false;
};
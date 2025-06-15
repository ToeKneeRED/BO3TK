#include "InputField.h"

InputField::InputField(const QString& acText, QWidget* apParent)
{
    setText(acText);
    setParent(apParent);
    setMinimumWidth(600);
    setStyleSheet(R"(
        QLineEdit {
            background-color: #2c2c2c;
            color: #ffffff;
            border: none;
            border-radius: 6px;
            padding: 8px 16px;
            font-size: 14px;
            font-family: "Jetbrains Mono NL Semibold";
            selection-background-color: #5e5e5e;
        }

        QLineEdit:focus {
            border: 1px solid #5a9bd5;
            background-color: #333333;
        }
    )");

    connect(this, &InputField::focusLost, [this] { this->OnSubmit(); });
    connect(this, &QLineEdit::returnPressed, [this] { this->OnSubmit(); });

    show();
}

void InputField::focusOutEvent(QFocusEvent* event)
{
    emit focusLost();
    QLineEdit::focusOutEvent(event);

    OnSubmit();
}

void InputField::resizeEvent(QResizeEvent* event)
{
    QLineEdit::resizeEvent(event);

    OnResize();
}

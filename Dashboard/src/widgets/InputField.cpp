#include "InputField.h"

InputField::InputField(const QString& text, QWidget* parent)
{
    setMinimumWidth(600);
    setStyleSheet(R"(
        QLineEdit {
            background-color: #ffffff;
            color: #2c2c2c;
            border: 1px solid #444444;
            border-radius: 4px;
            padding: 8px 12px;
            font-size: 12px;
            font-family: "Jetbrains Mono NL Semibold";
        }

        QLineEdit:focus {
            border: 1px solid #888888;
            color: #3c3c3c;
            background-color: #aaaaaa;
        }
    )");

    setText(text);

    connect(this, &InputField::focusLost, [this] {
         this->OnSubmit();
    });

    connect(this, &QLineEdit::returnPressed, [this] {
        this->OnSubmit();
    });
}

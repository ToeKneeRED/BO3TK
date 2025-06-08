#pragma once
#include <QLineEdit>
#include "Handler.h"

struct InputField final : QLineEdit
{
     Q_OBJECT
public:
     explicit InputField(const QString& text, QWidget* parent = nullptr);

    Handler<> OnSubmit;

signals:
    void focusLost();

protected:
    void focusOutEvent(QFocusEvent* event) override
    {
        emit focusLost();
        QLineEdit::focusOutEvent(event);

        OnSubmit();
    }
};
#pragma once
#include "Handler.h"
#include "Component.h"
#include <QLineEdit>

struct InputField final : Component<QLineEdit>
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
#pragma once
#include "Handler.h"
#include "Component.h"
#include <QLineEdit>

struct InputField final : Component<QLineEdit>
{
    Q_OBJECT

public:
    explicit InputField(const QString& acText, QWidget* apParent = nullptr);

    Handler<> OnSubmit;
    Handler<> OnResize;

signals:
    void focusLost();

protected:
    void resizeEvent(QResizeEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
};
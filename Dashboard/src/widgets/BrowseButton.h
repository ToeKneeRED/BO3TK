#pragma once
#include "Component.h"
#include "InputField.h"
#include <QToolButton>

struct BrowseButton final : Component<QToolButton>
{
    Q_OBJECT

public:
    Handler<void, const QString&> OnFileSelect;

    explicit BrowseButton(const QString& acText, QWidget* parent = nullptr);

signals:
    void fileSelected(const QString& path);
};

#include "BrowseButton.h"
#include <QIcon>
#include <QFileDialog>

BrowseButton::BrowseButton(const QString& acText, QWidget* parent)
{
    setParent(parent);
    setText(acText);
    setToolTip("Browse for file...");
    setCursor(Qt::PointingHandCursor);
    setIcon(QIcon(":/icons/folder.svg"));
    setIconSize(QSize(16, 16));
    setStyleSheet(R"(
        QToolButton {
            border: none;
            padding: 4px;
            background-color: none;
        }
    )");

    connect(this, &QToolButton::clicked, [this] 
    {
        if (const QString cFilePath = QFileDialog::getOpenFileName(this, "Select DLL", QString(), 
            "DLL Files (*.dll);;All Files (*)"); !cFilePath.isEmpty()) 
        {
            emit fileSelected(cFilePath);
        }
    });
    connect(this, &BrowseButton::fileSelected, [this](const QString& acPath) {
        OnFileSelect(acPath);
    });
}
#include "BrowseButton.h"
#include <QIcon>
#include <QFileDialog>

BrowseButton::BrowseButton(const QString& acText, QWidget* parent)
{
    setParent(parent);
    setToolTip(acText);
    setCursor(Qt::PointingHandCursor);
    setIcon(QIcon(":/folder.ico"));
    setStyleSheet(R"(
        QToolButton {
            border: none;
            padding: 4px;
            border-radius: 4px;
            background-color: none;
        }
        QToolButton:hover {
            background-color: #3c3c3c;
        }
    )");

    connect(
        this, &QToolButton::clicked,
        [this]
        {
            if (const QString cFilePath =
                    QFileDialog::getOpenFileName(this, "Select DLL", QString(), "DLL Files (*.dll);;All Files (*)");
                !cFilePath.isEmpty())
            {
                emit fileSelected(cFilePath);
            }
        });
    connect(this, &BrowseButton::fileSelected, [this](const QString& acPath) { OnFileSelect(acPath); });

    show();
}
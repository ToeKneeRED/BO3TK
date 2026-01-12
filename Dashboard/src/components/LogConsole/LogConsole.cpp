#include "LogConsole.h"
#include "Button.h"

#include <QVBoxLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QPlainTextEdit>
#include <QTime>
#include <QScrollBar>

LogConsole::LogConsole(QWidget* apParent)
{
    setParent(apParent);

    QVBoxLayout* pLayout = new QVBoxLayout(this);
    QHBoxLayout* pToolbar = new QHBoxLayout();

    m_pConsole = new QPlainTextEdit();
    m_pConsole->setReadOnly(true);
    m_pConsole->setMaximumBlockCount(5000);
    m_pConsole->setFont(QFont("JetBrains Mono", 10));
    m_pConsole->setStyleSheet(R"(
        QPlainTextEdit {
            background-color: #1e1e1e;
            color: #ffffff;
            border: 1px solid #333333;
        }
        QComboBox {
            background-color: #1e1e1e;
            color: #ffffff;
            border: 1px solid #333333;
        }
    )");

    m_pFilter = new QComboBox();
    m_pFilter->addItems({"All", "Game", "Hook", "Info", "Warning", "Error"});

    m_pClear = new Button("Clear", this);
    m_pClear->OnPress += [&]()
    {
        m_pConsole->clear();
    };

    m_pAutoScroll = new QCheckBox();
    m_pAutoScroll->setCheckState(Qt::CheckState::Checked);
    m_pAutoScroll->setChecked(true);

    pToolbar->addWidget(m_pFilter);
    pToolbar->addWidget(m_pAutoScroll);
    pToolbar->addStretch();
    pToolbar->addWidget(m_pClear);

    pLayout->addLayout(pToolbar);
    pLayout->addWidget(m_pConsole);
}

void LogConsole::AddLog(const char* acpMessage, const LogLevel& acLevel)
{
    QTextCharFormat format;
    format.setForeground(LogColors[acLevel]);

    QTextCursor cursor(m_pConsole->document());
    cursor.movePosition(QTextCursor::End);
    cursor.insertText(
        QString("[%1] %2\n").arg(QTime::currentTime().toString("hh:mm:ss")).arg(acpMessage), format
    );

    if (m_pAutoScroll->isChecked())
    {
        QMetaObject::invokeMethod(m_pConsole, [this]()
        {
            QScrollBar* pBar = m_pConsole->verticalScrollBar();
            pBar->setValue(pBar->maximum());
        }, Qt::QueuedConnection);
    }
}
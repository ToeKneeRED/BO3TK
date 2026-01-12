#pragma once
#include "Component.h"

#include <QWidget>
#include <QMap>

struct Button;
class QComboBox;
class QPlainTextEdit;
class QCheckBox;

struct LogConsole final : Component<QWidget>
{
    Q_OBJECT

public:
    enum class LogLevel : uint8_t
    {
        Game,
        Hook,
        Info,
        Warning,
        Error
    };

    explicit LogConsole(QWidget* apParent = nullptr);

    void AddLog(const char* acpMessage = "", const LogLevel& acLevel = LogLevel::Info);

    QMap<LogLevel, QColor> LogColors = {
        { LogLevel::Game,    QColor("#aaaaaa") },
        { LogLevel::Hook,    QColor("#5a9bd5") },
        { LogLevel::Info,    QColor("#ffffff") },
        { LogLevel::Warning, QColor("#f0ad4e") },
        { LogLevel::Error,   QColor("#d9534f") },
    };

private:
    QPlainTextEdit* m_pConsole;
    QComboBox* m_pFilter;
    QCheckBox* m_pAutoScroll;
    Button* m_pClear;
};
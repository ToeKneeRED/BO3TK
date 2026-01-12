#pragma once
#include "Event.h"

enum class LogType : uint8_t
{
    Game,
    Hook,
    Info,
    Warning,
    Error
};

#pragma pack(push, 1)
struct LogData
{
    LogType Type{};
    char Message[256]{};
};
#pragma pack(pop)

struct LogEvent : Event<LogData>
{
    LogEvent(const LogData& acData)
        : Event(acData, "LogEvent")
    {
    }
};
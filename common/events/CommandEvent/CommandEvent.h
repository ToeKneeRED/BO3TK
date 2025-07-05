#pragma once
#include "Event.h"

struct CommandEvent : Event<std::string>
{
    CommandEvent(const IPC& acIpc, const std::string& acName = "", const size_t& acLength = 0)
        : Event(acIpc, acName, acLength)
    {
    }
};
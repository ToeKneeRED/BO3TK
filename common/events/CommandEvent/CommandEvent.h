#pragma once
#include "Event.h"

struct CommandEvent : Event<std::string>
{
    CommandEvent(const IPC& acIpc, const char* acName = "", const size_t& acLength = sizeof(CommandEvent::DataType))
        : Event(acIpc, acName)
    {
    }
};
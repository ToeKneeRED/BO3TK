#pragma once
#include <string>

#include "Event.h"

struct CommandEvent : Event<std::string>
{
    CommandEvent(const char* acName = "", const size_t& acLength = sizeof(CommandEvent::DataType))
        : Event(acName)
    {
    }
};
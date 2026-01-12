#pragma once
#include "EventHandler.h"
#include "LogEvent.h"
#include "structs.h"

extern std::shared_ptr<EventHandler> g_pEventHandler;

namespace Hooks
{
    typedef void(__fastcall* Com_PrintMessage_t)(int, consoleLabel_e, const char*, int);
    inline Com_PrintMessage_t oCom_PrintMessage = nullptr;
    inline void __fastcall hCom_PrintMessage(int channel, consoleLabel_e label, const char* msg, int error)
    {
        LogData data{.Type = LogType::Hook };
        strncpy_s(data.Message, sizeof(data.Message), msg, sizeof(data.Message));

        if (g_pEventHandler)
        {
            g_pEventHandler->Send<LogEvent>(data);
        }
        else
        {
            LOG_WARN("[{}] EventHandler not found", __FUNCTION__);
        }
    
        return oCom_PrintMessage(channel, label, msg, error);
    }
}
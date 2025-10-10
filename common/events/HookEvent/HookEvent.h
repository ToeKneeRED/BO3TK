#pragma once
#include "Event.h"

struct FuncHook
{
    std::string Name{};
    uintptr_t Target{};
    uintptr_t Detour{};
    void* Original{};
    bool Enabled;
};

struct HookEvent : Event<FuncHook>
{
    HookEvent() : Event("HookEvent") {}
};
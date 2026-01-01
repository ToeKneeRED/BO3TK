#pragma once
#include "Event.h"

struct FuncHook
{
    explicit FuncHook(const std::string& acName = "", const uintptr_t& acTarget = 0, const uintptr_t& acDetour = 0, 
        void* apOriginal = nullptr, const bool& acEnabled = false) : Name(acName), Target(acTarget),
        Detour(acDetour), Original(apOriginal), Enabled(acEnabled){}

    std::string Name{};
    uintptr_t Target{};
    uintptr_t Detour{};
    void* Original{};
    bool Enabled;
};

struct HookEvent : Event<FuncHook>
{
    HookEvent() : Event("HookEvent") {}
    HookEvent(const FuncHook& acHook) : Event(acHook, "HookEvent") {}
};
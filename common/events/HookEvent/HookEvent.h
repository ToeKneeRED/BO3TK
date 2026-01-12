#pragma once
#include <string>

#include "Event.h"

enum class HookType : uint8_t
{
    None,
    Function,
    Library
};

#pragma pack(push, 1)
struct HookData
{
    HookType Type{};
    bool Enabled{};
    uintptr_t Target{};
    uintptr_t Detour{};

    char FuncName[64]{};
    char LibName[64]{};
};
#pragma pack(pop)

struct HookEvent : Event<HookData>
{
    HookEvent(const HookData& acData)
        : Event(acData, "HookEvent")
    {
    }
};

struct IHook
{
    explicit IHook(
        const HookType& acType = HookType::None, const std::string& acName = "", const uintptr_t& acTarget = 0,
        const uintptr_t& acDetour = 0, void** apOriginal = nullptr, const bool& acEnabled = false)
        : Type(acType)
        , Name(acName)
        , Target(acTarget)
        , Detour(acDetour)
        , Original(apOriginal)
        , Enabled(acEnabled)
    {
    }
    IHook(const IHook&) = default;
    IHook(IHook&&) = default;
    virtual IHook& operator=(const IHook&) = default;
    virtual IHook& operator=(IHook&&) = default;
    virtual ~IHook() = default;

    HookType Type{};
    std::string Name{};
    uintptr_t Target{};
    uintptr_t Detour{};
    void** Original{};
    bool Enabled{};
};
struct FuncHook : IHook
{
    FuncHook(const std::string& acName, uintptr_t acTarget)
        : IHook(HookType::Function, acName, acTarget)
    {
    }
};

struct LibHook : IHook
{
    LibHook(const std::string& acLibName = "", const std::string& acFuncName = "", uintptr_t acTarget = 0)
        : IHook(HookType::Library, acFuncName, acTarget)
        , LibName(acLibName)
    {
    }

    std::string LibName;
};
#pragma once
#include <utility>

struct IComponent
{
    IComponent() = default;
    virtual ~IComponent() = default;
    IComponent(const IComponent&) = default;
    IComponent(IComponent&&) = default;
    IComponent& operator=(const IComponent&) = default;
    IComponent& operator=(IComponent&&) = default;
};

template<typename T, typename... Args>
struct Component : T, IComponent
{
    explicit Component(Args&&... aArgs) : T(std::forward<Args>(aArgs)...)
    {
        
    }
};
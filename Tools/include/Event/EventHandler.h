#pragma once
#include <typeindex>
#include <unordered_map>
#include <functional>
#include <thread>
#include <atomic>
#include <memory>

#include "Event.h"
#include "IEventBase.h"

struct EventHandler
{
    // funky stuff to extract argument type from lambda/function
    template <typename F> struct FuncArg;
    template <typename R, typename C, typename Arg> struct FuncArg<R (C::*)(Arg) const>
    {
        using Type = Arg;
    };
    template <typename F> using FuncArgType = typename FuncArg<decltype(&F::operator())>::Type;

    explicit EventHandler(std::string pipeName)
        : m_pipeName(std::move(pipeName))
    {
    }
    EventHandler(const EventHandler&) = delete;
    EventHandler(EventHandler&&) = delete;
    EventHandler& operator=(const EventHandler&) = delete;
    EventHandler& operator=(EventHandler&&) = delete;
    ~EventHandler() { Stop(); }

    template <typename Callback> void AddCallback(Callback&& aCallback)
    {
        using ArgType = FuncArgType<Callback>;
        auto event = std::make_unique<Event<ArgType>>(IPC::Server, m_pipeName.c_str());

        m_callbacks[typeid(ArgType)] = [callback = std::forward<Callback>(aCallback)](const void* acData)
        {
            callback(*static_cast<const ArgType*>(acData));
        };

        m_events.push_back(std::move(event));
    }

    void Start()
    {
        m_thread = std::thread([this]
        {
            while (m_running)
            {
                for (const auto& cEvent : m_events)
                {
                    auto& cType = cEvent->GetDataType();
                    if (auto iter = m_callbacks.find(cType); iter != m_callbacks.end())
                    {
                        cEvent->Receive(iter->second);
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        });
    }

    void Stop()
    {
        m_running = false;

        if (m_thread.joinable())
            m_thread.join();
    }

private:
    std::string m_pipeName;
    std::unordered_map<std::type_index, std::function<void(const void*)>> m_callbacks;
    std::vector<std::unique_ptr<IEventBase>> m_events;
    std::thread m_thread;
    std::atomic<bool> m_running{true};
};
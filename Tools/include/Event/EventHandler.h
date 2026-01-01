#pragma once
#include <typeindex>
#include <unordered_map>
#include <functional>
#include <thread>
#include <atomic>
#include <memory>

#include "Event.h"
#include "Log.h"
#include "IEventBase.h"

#pragma pack(push, 1)
struct EventHeader {
    size_t TypeHash;
    size_t DataSize;
};
#pragma pack(pop)

struct EventHandler
{
    // funky stuff to extract argument type from lambda/function
    template <typename F> struct FuncArg;
    template <typename R, typename C, typename Arg> struct FuncArg<R (C::*)(Arg) const>
    {
        using Type = Arg;
    };
    template <typename F> using FuncArgType = typename FuncArg<decltype(&F::operator())>::Type;

    explicit EventHandler(const std::string& acPipeName)
        : m_pipeName(std::move(R"(\\.\pipe\)" + acPipeName))
    {
    }
    EventHandler(const EventHandler&) = delete;
    EventHandler(EventHandler&&) = delete;
    EventHandler& operator=(const EventHandler&) = delete;
    EventHandler& operator=(EventHandler&&) = delete;
    ~EventHandler() { Stop(); }

    void Start()
    {
        if (Running)
            return;

        m_pipe = CreateNamedPipeA(
            m_pipeName.c_str(),
            PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
            1, 1024, 1024, 0, nullptr);

        if (m_pipe == INVALID_HANDLE_VALUE)
        {
            Log::Get()->Print("Pipe already created, creating file");
            m_pipe = CreateFileA(m_pipeName.c_str(), GENERIC_READ | GENERIC_WRITE, 
                    0, nullptr, OPEN_EXISTING, 0, nullptr);

            if (m_pipe)
            {
                Log::Get()->Print("Connected");
                Running = true;
            }
            else
            {
                char buf[256];
                FormatMessageA(
                    FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, GetLastError(),
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, (sizeof(buf) / sizeof(char)), nullptr);
                Log::Get()->Error("Failed pipe creation: {} {}", m_pipeName.c_str(), buf);
                return;
            }
        }

        Log::Get()->Print("Running ConnectNamedPipe");

        if (!ConnectNamedPipe(m_pipe, nullptr))
        {
            if (const DWORD cErr = GetLastError(); cErr == ERROR_PIPE_CONNECTED)
            {
                Log::Get()->Print("Connected");
                Running = true;
            }
        }
        else
        {
            Running = true;
            Log::Get()->Print("Connected");
        }

        Log::Get()->Print("Finished ConnectNamedPipe");
        Log::Get()->Print("Running: {}", Running ? "true" : "false");

        //m_thread = std::thread([this]
        //{
            while (Running)
            {
                Log::Get()->Print("running...");
                EventHeader header{};
                DWORD read = 0;

                Log::Get()->Print("reading file for header");
                if (!ReadFile(m_pipe, &header, sizeof(EventHeader), &read, nullptr))
                    continue;

                Log::Get()->Print("read file for header");
                std::vector<char> buffer(header.DataSize);
                Log::Get()->Print("reading file for data");
                ReadFile(m_pipe, buffer.data(), header.DataSize, &read, nullptr);
                Log::Get()->Print("read file for data");

                if (auto it = m_handlers.find(header.TypeHash); it != m_handlers.end())
                {
                    if (auto iter = std::ranges::find(buffer, '\0'); iter != buffer.end()) {
                        buffer.resize(std::distance(buffer.begin(), iter));
                    }
                    it->second(buffer.data(), buffer.size());
                }
                else
                {
                    Log::Get()->Warn("No handler registered for event type {}", header.TypeHash);
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(SleepDuration));
            }
        //});
    }

    void Stop()
    {
        Running = false;

        if (m_thread.joinable())
            m_thread.join();
    }

    template <typename T>
    void RegisterEvent(std::function<void(const T&)> aHandler)
    {
        const size_t cHash = std::hash<std::string>{}(typeid(T).name());
        m_handlers[cHash] = [aHandler](const void* apData, const size_t acSize)
        {
            if constexpr (std::is_trivially_copyable_v<T>)
            {
                T data{};
                memcpy(&data, apData, min(acSize, sizeof(T)));
                aHandler(data);
            }
            else
            {
                T data{};
                memcpy(&data, apData, min(acSize, sizeof(T)));
                aHandler(data);
            }
        };
    }

    template<typename T = IEventBase>
    void Send(const T& acEvent) const
    {
        const size_t typeHash = std::hash<std::string>{}(typeid(T).name());

        const void* pData = nullptr;
        size_t dataSize = 0;

        if constexpr (std::is_base_of_v<IEventBase, T>)
        {
            pData = acEvent.GetData();
            dataSize = acEvent.GetDataSize();
        }
        else
        {
            pData = &acEvent;
            dataSize = sizeof(T);
        }

        if (!pData || !dataSize)
            return;

        const EventHeader cHeader {.TypeHash = typeHash, .DataSize = static_cast<uint32_t>(dataSize) };
        DWORD written = 0;

        if(!m_pipe) 
        {
            Log::Get()->Error("Attempting to send without pipe connected");
        }

        Log::Get()->Print("writing");
        WriteFile(m_pipe, &cHeader, sizeof(EventHeader), &written, nullptr);
        Log::Get()->Print("wrote header");
        WriteFile(m_pipe, pData, (DWORD)dataSize, &written, nullptr);
        Log::Get()->Print("wrote data");
    }

    uint32_t SleepDuration = 250;
    std::atomic<bool> Running{false};

private:
    std::string m_pipeName;
    HANDLE m_pipe{};
    std::unordered_map<uint32_t, std::function<void(const void*, size_t)>> m_handlers;
    std::thread m_thread;
    std::vector<IEventBase*> m_eventsToRegister{};
};
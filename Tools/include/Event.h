#pragma once
#include <atomic>
#include <cstdint>
#include <mutex>

#include "Log.h"

enum class EventType : uint8_t
{
    None = 0,
    Notify,
    Data
};

struct WinEvent
{
    WinEvent()
    {
        m_event = ::CreateEvent(nullptr, TRUE, FALSE, nullptr);

        if (!m_event)
        {
            char buf[256];
            FormatMessageA(
                FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf,
                (sizeof(buf) / sizeof(char)), nullptr);
            Log::Get()->Error("Failed to create WinEvent: {}", buf);
        }
    }
    ~WinEvent()
    {
        if (m_event)
        {
            ::CloseHandle(m_event);
        }
    }
    WinEvent(const WinEvent&) = default;
    WinEvent(WinEvent&&) = default;
    WinEvent& operator=(const WinEvent&) = default;
    WinEvent& operator=(WinEvent&&) = default;

    [[nodiscard]] HANDLE Get() const { return m_event; }
    void Set() const { ::SetEvent(m_event); }
    void Reset() const { ::ResetEvent(m_event); }
    [[nodiscard]] bool Wait(const DWORD cMs = INFINITE) const { return ::WaitForSingleObject(m_event, cMs) == WAIT_OBJECT_0; }

private:
    HANDLE m_event;
};

#pragma pack(push, 1)
template <typename DataType, typename... Args> struct Event
{
    Event() = default;
    explicit Event(const DataType& acData, const EventType& acType)
        : m_type(acType), m_data(acData)
    {
    }

    static Event* Create(const DataType& acData, const EventType& acType = EventType::None) { return new Event(acData, acType); }
    void Read(const DataType& acData)
    {
        std::lock_guard lock(m_mutex);
        m_data = acData;
        m_isDirty.store(true);
    }
    DataType& Write()
    {
        std::lock_guard lock(m_mutex);
        m_isDirty.store(false);
        return m_data;
    }
    //void SetEvent() const { m_winEvent.Set(); }
    [[nodiscard]] bool IsDirty() const { return m_isDirty.load(); }

private:
    std::atomic<EventType> m_type = EventType::None;
    std::atomic<bool> m_isDirty = false;
    DataType m_data{};
    std::mutex m_mutex{};
    WinEvent m_winEvent{};
};
#pragma pack(pop)
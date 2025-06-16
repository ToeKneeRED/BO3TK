#pragma once
#include <cstdint>
#include <mutex>
#include <utility>

#include "Log.h"

enum class EventType : uint8_t
{
    None = 0,
    Notify,
    Data
};
enum class IPC : uint8_t
{
    None = 0,
    Create,
    Open
};

template <typename T>
struct Shared
{
    T buffer{};
};

template<typename T>
struct WinEvent
{
    explicit WinEvent(const IPC& acIpc = IPC::None, const LPCSTR acpEventName = nullptr, const LPCSTR acpMappingName = nullptr)
    {
        switch (acIpc)
        {
        case IPC::Create: Create(acpEventName, acpMappingName); break;
        case IPC::Open: Open(acpEventName, acpMappingName); break;
        case IPC::None: break;
        }
    } 
    ~WinEvent()
    {
        if (m_event)
        {
            CloseHandle(m_event);
        }
        if (m_mapping)
        {
            CloseHandle(m_mapping);
        }
    }
    WinEvent(const WinEvent&) = default;
    WinEvent(WinEvent&&) = default;
    WinEvent& operator=(const WinEvent&) = default;
    WinEvent& operator=(WinEvent&&) = default;
    
    [[nodiscard]] HANDLE GetEventHandle() const { return m_event; }
    [[nodiscard]] HANDLE GetMappingHandle() const { return m_mapping; }
    void Set() const { SetEvent(m_event); }
    void Reset() const { ResetEvent(m_event); }
    [[nodiscard]] bool Wait(const DWORD cMs = INFINITE) const
    {
        const bool cResult = WaitForSingleObject(m_event, cMs) == WAIT_OBJECT_0;
        ResetEvent(m_event);
        return cResult;
    }

    bool Create(const LPCSTR acpEventName = nullptr, const LPCSTR acpMappingName = nullptr)
    {
        m_event = CreateEventA(nullptr, TRUE, FALSE, acpEventName);
        m_mapping = CreateFileMappingA(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, sizeof(T), acpMappingName);

        while (!m_event || !m_mapping)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(2500));

            if (!m_event)
            {
                m_event = CreateEventA(nullptr, TRUE, FALSE, acpEventName);
                continue;
            }

            if (!m_mapping)
            {
                m_mapping = CreateFileMappingA(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, sizeof(T), acpMappingName);
                continue;
            }
        }

        return true;
    }
    bool Open(const LPCSTR acpEventName = nullptr, const LPCSTR acpMappingName = nullptr)
    {
        m_event = OpenEventA(EVENT_ALL_ACCESS, FALSE, acpEventName);
        m_mapping = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, acpMappingName);

        while (!m_event || !m_mapping)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(2500));

            if (!m_event)
            {
                m_event = OpenEventA(EVENT_ALL_ACCESS, FALSE, acpEventName);
                continue;
            }

            if (!m_mapping)
            {
                m_mapping = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, acpMappingName);
                continue;
            }
        }

        return true;
    }

private:
    HANDLE m_event;
    HANDLE m_mapping;
};

#pragma pack(push, 1)
template <typename DataType> struct Event
{
    Event() = default;
    Event(const LPCSTR acpEventName = nullptr, 
        const LPCSTR acpMappingName = nullptr, const IPC& acIpc = IPC::None, const EventType& acType = EventType::None)
        : m_type(acType)
        , m_ipc(acIpc)
    {
        switch (acIpc)
        {
        case IPC::Create: Create(acpEventName, acpMappingName); break;
        case IPC::Open: Open(acpEventName, acpMappingName); break;
        case IPC::None: break;
        }
    }
    ~Event()
    {
        if (m_data)
            UnmapViewOfFile(m_data);
        m_data = nullptr;
    }
    Event(Event&&) = default;
    Event(const Event&) = default;
    Event& operator=(Event&&) = default;
    Event& operator=(const Event&) = default;

    bool Create(const LPCSTR acpEventName, const LPCSTR acpMappingName)
    {
        if (!m_winEvent.Create(acpEventName, acpMappingName))
            return false;

        m_data = static_cast<DataType*>(MapViewOfFile(m_winEvent.GetMappingHandle(), FILE_MAP_WRITE, 0, 0, 0));

        return true;
    }
    bool Open(const LPCSTR acpEventName, const LPCSTR acpMappingName)
    {

        if (!m_winEvent.Open(acpEventName, acpMappingName))
            return false;

        m_data = static_cast<DataType*>(MapViewOfFile(m_winEvent.GetMappingHandle(), FILE_MAP_READ, 0, 0, 0));

        return true;
    }

    [[nodiscard]] DataType Read()
    {
        return *m_data;
    }
    void Write(const DataType& acData)
    {
        memcpy_s(m_data, sizeof(DataType), &acData, sizeof(DataType));
    }

    void SetEvent() const { m_winEvent.Set(); }
    [[nodiscard]] bool WaitForEvent(const DWORD cMs = INFINITE) const { return m_winEvent.Wait(cMs); }

private:
    EventType m_type = EventType::None;
    IPC m_ipc = IPC::None;
    DataType* m_data{};
    WinEvent<DataType> m_winEvent;
};
#pragma pack(pop)
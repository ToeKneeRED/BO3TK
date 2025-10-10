#pragma once
#include <windows.h>
#include <string>
#include <functional>
#include "Log.h"
#include "IEventBase.h"

template <typename Type> struct Event : IEventBase
{
    using DataType = Type;

    using IEventBase::IEventBase;
    Event(const char* acName = "")
        : IEventBase(acName) {}
    Event() : IEventBase() {}
    Event(const Event&) = default;
    Event(Event&&) = default;
    ~Event() override
    {
        if (m_pipe != INVALID_HANDLE_VALUE)
        {
            CloseHandle(m_pipe);
            m_pipe = INVALID_HANDLE_VALUE;
        }
    }
    Event& operator=(const Event&) = default;
    Event& operator=(Event&&) = default;

    bool Send(const DataType& acData)
    {
        if constexpr (std::is_same_v<DataType, std::string>)
        {
            const DWORD cLength = static_cast<DWORD>(acData.size());
            DWORD written = 0;

            if (!WriteFile(m_pipe, &cLength, sizeof(cLength), &written, nullptr))
                return false;

            return WriteFile(m_pipe, acData.data(), cLength, &written, nullptr);
        }
        else if constexpr (std::is_trivially_copyable_v<DataType>)
        {
            DWORD written = 0;
            return WriteFile(m_pipe, &acData, sizeof(DataType), &written, nullptr);
        }
        else
        {
            static_assert(std::is_trivially_copyable_v<DataType>, "Cannot WriteFile with non-trivially-copyable type.");
            return false;
        }
    }

    bool Send(const Event<DataType>* acpEvent)
    {
        const DataType cData = acpEvent->GetData();

        if constexpr (std::is_same_v<DataType, std::string>)
        {
            const DWORD cLength = static_cast<DWORD>(cData.size());
            DWORD written = 0;

            if (!WriteFile(m_pipe, &cLength, sizeof(cLength), &written, nullptr))
                return false;

            return WriteFile(m_pipe, cData.data(), cLength, &written, nullptr);
        }
        else if constexpr (std::is_trivially_copyable_v<DataType>)
        {
            DWORD written = 0;
            return WriteFile(m_pipe, &cData, sizeof(DataType), &written, nullptr);
        }
        else
        {
            static_assert(std::is_trivially_copyable_v<DataType>, "Cannot WriteFile with non-trivially-copyable type.");
            return false;
        }
    }

    bool Receive(const void* acData) override
    {
        DataType buffer{};
        return Receive(&buffer);
    }

    bool Receive(DataType& aBuffer)
    {
        if (!m_connected)
        {
            Log::Get()->Warn("Attempted to receive data while not connected");
            return false;
        }

        DWORD read = 0;

        if constexpr (std::is_same_v<DataType, std::string>) // string (should switch to stringstream for non trivial copy?)
        {
            DWORD length = 0;
            if (!ReadFile(m_pipe, &length, sizeof(length), &read, nullptr) || length == 0 || length > 8192)
                return false;

            aBuffer.resize(length);
            return ReadFile(m_pipe, aBuffer.data(), length, &read, nullptr);
        }
        else if constexpr (std::is_base_of_v<IEventBase, DataType>) // event
        {
            return ReadBuffer(&aBuffer, sizeof(DataType));
        }
        else if constexpr (std::is_trivially_copyable_v<DataType>) // trivial copy
        {
            return ReadFile(m_pipe, &aBuffer, sizeof(DataType), &read, nullptr);
        }
        return false;
    }

    bool Receive(const std::function<void(DataType&)>& acHandler)
    {
        if (DataType data{}; Receive(data))
        {
            acHandler(data);
            return true;
        }
        return false;
    }

    bool Receive(const std::function<void(const void*)>& acHandler) override
    {
        DataType data;

        if (Receive([&](DataType& val) { acHandler(static_cast<const void*>(&val)); }))
            return true;
        return false;
    }

    [[nodiscard]] HANDLE GetHandle() const { return m_pipe; }
    [[nodiscard]] bool IsValid() const { return m_pipe != INVALID_HANDLE_VALUE; }
    [[nodiscard]] DataType& GetData() const { return m_data; }
    [[nodiscard]] const std::type_info& GetDataType() const override { return typeid(DataType); }
    void SetData(const DataType& acData) { m_data = acData; }

private:
    DataType m_data{};
    size_t m_length = sizeof(DataType);
};
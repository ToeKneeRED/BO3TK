#pragma once
#include <functional>

struct IEventBase
{
    IEventBase() = delete;
    explicit IEventBase(const std::string& acName)
        : m_name(R"(\\.\pipe\)" + acName)
    {
    }
    IEventBase(const IEventBase&) = default;
    IEventBase(IEventBase&&) = default;
    IEventBase& operator=(const IEventBase&) = default;
    IEventBase& operator=(IEventBase&&) = default;
    virtual ~IEventBase()
    {
        if (m_pipe != INVALID_HANDLE_VALUE)
        {
            CloseHandle(m_pipe);
            m_pipe = INVALID_HANDLE_VALUE;
        }
    }

    virtual bool Receive(const void*) = 0;
    virtual bool Receive(const std::function<void(const void*)>&) = 0;
    [[nodiscard]] virtual const std::type_info& GetDataType() const = 0;

    bool Create()
    {
        if (m_connected)
            return false;

        m_pipe = CreateNamedPipeA(
            m_name.c_str(), PIPE_ACCESS_DUPLEX, PIPE_READMODE_MESSAGE | PIPE_TYPE_MESSAGE | PIPE_WAIT,
            PIPE_UNLIMITED_INSTANCES, sizeof(*this), sizeof(*this), NMPWAIT_WAIT_FOREVER, nullptr);

        if (m_pipe == INVALID_HANDLE_VALUE)
        {
            char buf[256];
            FormatMessageA(
                FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, GetLastError(),
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, (sizeof(buf) / sizeof(char)), nullptr);
            Log::Get()->Error("Failed to create named pipe {} {}", m_name.c_str(), buf);
            return false;
        }

        if (!ConnectNamedPipe(m_pipe, nullptr))
        {
            if (const DWORD cErr = GetLastError(); cErr == ERROR_PIPE_CONNECTED)
            {
                Log::Get()->Print("Client connected early");
                m_connected = true;
            }
        }
        else
        {
            m_connected = true;
            Log::Get()->Print("Server connected to pipe");
        }

        return true;
    }
    virtual bool Open(const char* acPipeName)
    {
        Log::Get()->Print("Attempting to connect to pipe...");

        while (true)
        {
            m_pipe = CreateFileA(
                acPipeName, GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);

            if (m_pipe != INVALID_HANDLE_VALUE)
                break;

            if (const DWORD cErr = GetLastError(); cErr == ERROR_PIPE_BUSY || cErr == ERROR_FILE_NOT_FOUND)
            {
                if (cErr == ERROR_FILE_NOT_FOUND)
                {
                    while (!WaitNamedPipeA(acPipeName, NMPWAIT_WAIT_FOREVER))
                    {
                        if (GetLastError() != ERROR_FILE_NOT_FOUND)
                        {
                            char buf[256];
                            FormatMessageA(
                                FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, GetLastError(),
                                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, (sizeof(buf) / sizeof(char)), nullptr);
                            Log::Get()->Error("WaitNamedPipe failed {}", buf);
                            return false;
                        }
                    }

                    m_pipe = CreateFileA(acPipeName, GENERIC_ALL, 0, nullptr, OPEN_EXISTING, 0, nullptr);
                }
                else
                {
                    char buf[256];
                    FormatMessageA(
                        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, GetLastError(),
                        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, (sizeof(buf) / sizeof(char)), nullptr);
                    Log::Get()->Error("CreateFile failed {}", buf);
                    return false;
                }
            }
        }

        DWORD mode = PIPE_READMODE_MESSAGE;
        mode = PIPE_READMODE_BYTE;
        m_connected = SetNamedPipeHandleState(m_pipe, &mode, nullptr, nullptr);

        m_connected = true;
        Log::Get()->Print("Connected to pipe");
        return true;
    }

    [[nodiscard]] const std::string& GetName() const { return m_name; }

protected:
    HANDLE m_pipe = INVALID_HANDLE_VALUE;
    bool m_connected = false;
    std::string m_name{};
};
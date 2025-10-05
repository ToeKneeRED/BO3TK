#pragma once
#include <functional>
#include <ranges>
#include <string>

#include "Log.h"

struct Command;

using CommandName = std::string;
using CommandArgs = std::vector<std::string>;
using CommandMap = std::unordered_map<CommandName, Command*>;
using CommandCallback = std::function<void(const CommandArgs&)>;
using CommandCallbacks = std::vector<CommandCallback>;

struct Command
{
    Command() = delete;
    Command(const CommandName& acName, const CommandArgs& acArgs, const CommandCallbacks& acCallbacks)
        : Name(acName), Args(acArgs), Callbacks(acCallbacks) {}

    CommandName Name{};
    CommandArgs Args{};
    CommandCallbacks Callbacks{};
};

struct CommandDispatcher
{
    explicit CommandDispatcher(const char* acPrefix = "/") : m_prefix(acPrefix) {}
    ~CommandDispatcher()
    {
        for (const auto& cCommand : m_commands | std::views::values)
        {
            delete cCommand;
        }
    };

    template <typename Func>
    void AddCallback(const std::string& acName, Func&& aFunc)
    {
        m_commands[acName]->Callbacks.emplace_back(std::forward<Func>(aFunc));
    }

    [[nodiscard]] Command* StringToCommand(const std::string& acCommandString)
    {
        const std::string cNoPrefix = acCommandString.starts_with(m_prefix) ? 
            acCommandString.substr(m_prefix.size()) : acCommandString;

        std::istringstream stream(cNoPrefix);
        CommandName name;
        stream >> name;
        
        CommandArgs args;
        std::string arg;
        while (stream >> arg)
            args.push_back(arg);

        Command* command = new Command(name, args, CommandCallbacks());

        return command;
    }

    static void Call(const Command* acCommand) noexcept
    {
        for (const auto& cCallback : acCommand->Callbacks)
        {
            cCallback(acCommand->Args);
        }
    }
    void Call(const CommandName& acName, const CommandArgs& acArgs) noexcept
    {
        for (const auto& cCallback : m_commands[acName]->Callbacks)
        {
            cCallback(acArgs);
        }
    }

    void AddCommand(Command* aCommand) { m_commands[aCommand->Name] = aCommand; }
    [[nodiscard]] Command* FindCommand(const CommandName& acName) const noexcept
    {
        const auto cCommand = m_commands.find(acName);
        return cCommand != m_commands.end() ? cCommand->second : nullptr;
    }

    [[nodiscard]] const std::string& GetPrefix() const noexcept { return m_prefix; }
    void SetPrefix(const char* acPrefix) noexcept { m_prefix = acPrefix; }
    void SetPrefix(const char acPrefix) noexcept { m_prefix = acPrefix; }

private:
    CommandMap m_commands{};
    std::string m_prefix{};
};
#pragma once
#include <functional>
#include <iostream>
#include <string>

#include "Log.h"

struct CommandDispatcher
{
    //    template<typename Func>
    //    void Register(const std::string& acName, Func&& aFunc)
    //    {
    //        m_funcs[acName] = [func = std::forward<Func>(aFunc)](const std::vector<std::string>& acArgs) {
    //            CallWithArgs(func, acArgs);
    //        };
    //    }
    //
    //    void Execute(const std::string& acName, const std::vector<std::string>& acArgs)
    //    {
    //        if (const auto cIter = m_funcs.find(acName); cIter != m_funcs.end())
    //            cIter->second(acArgs);
    //        else
    //            Log::Get()->Warn("Command not found: ", acName);
    //    }
    //
    //    void Execute(const std::string& commandLine)
    //    {
    //        size_t openParen = commandLine.find('(');
    //        size_t closeParen = commandLine.rfind(')');
    //        if (openParen == std::string::npos || closeParen == std::string::npos || closeParen <= openParen) {
    //            std::cerr << "Invalid command format.\n";
    //            return;
    //        }
    //
    //        std::string funcName = commandLine.substr(0, openParen);
    //        std::string argsStr = commandLine.substr(openParen + 1, closeParen - openParen - 1);
    //        std::vector<std::string> args = SplitArgs(argsStr);
    //
    //        Dispatch(funcName, args);
    //    }
    //
    //    // string -> type
    //    template<typename T>
    //    static T Convert(const std::string& acString)
    //    {
    //        std::istringstream iss(acString);
    //        T val;
    //        iss >> val;
    //        if (iss.fail())
    //            throw std::runtime_error("Failed to convert argument: " + acString);
    //        return val;
    //    }
    //    template<>
    //    inline int Convert<int>(const std::string& arg)
    //    {
    //        return std::stoi(arg);
    //    }
    //    template<>
    //    inline const char* Convert<const char*>(const std::string& arg)
    //    {
    //        return arg.c_str();
    //    }
    //    template<>
    //    inline UINT Convert<UINT>(const std::string& arg)
    //    {
    //        if (arg == "MB_OK") return MB_OK;
    //        if (arg == "MB_ICONERROR") return MB_ICONERROR;
    //        return MB_OK; // default
    //    }
    //    template<>
    //    inline HWND Convert<HWND>(const std::string& arg)
    //    {
    //        size_t idx = 0;
    //        uintptr_t val = std::stoull(arg, &idx, 0);
    //        if (idx != arg.length())
    //            throw std::runtime_error("Failed to fully parse HWND from: " + arg);
    //        return reinterpret_cast<HWND>(val);
    //    }
    //    template<typename... Args>
    //    static std::tuple<Args...> ConvertAll(const std::vector<std::string>& strings)
    //    {
    //        if (strings.size() < sizeof...(Args))
    //            throw std::runtime_error("Not enough arguments.");
    //
    //        return ConvertAllImpl<Args...>(strings, std::index_sequence_for<Args...>{});
    //    }
    //    template<typename... Args, size_t... Is>
    //    static std::tuple<Args...> ConvertAllImpl(const std::vector<std::string>& strings, std::index_sequence<Is...>)
    //    {
    //        return std::make_tuple(Convert<Args>(strings[Is])...);
    //    }
    //
    //    template<typename Func, typename... Args>
    //    static void CallWithArgs(Func&& aFunc, const std::vector<std::string>& acArgs)
    //    {
    //        if (acArgs.size() < sizeof...(Args))
    //            throw std::runtime_error("Not enough arguments.");
    //
    //        CallWithArgsImpl<Func, Args...>(
    //            std::forward<Func>(aFunc),
    //            acArgs,
    //            std::index_sequence_for<Args...>{}
    //        );
    //    }
    //
    //    std::string Prefix{'/'};
    //
    // private:
    //    static void Dispatch(const std::string& acName, const std::vector<std::string>& acArgs)
    //    {
    //        if (acName == "MessageBoxA")
    //        {
    //            if (acArgs.size() != 3 && acArgs.size() != 4) {
    //                std::cerr << "MessageBoxA requires 3 or 4 arguments.\n";
    //                return;
    //            }
    //
    //            // reversed :sdwut:
    //            const char* text = acArgs[2].c_str();
    //            const char* caption = acArgs[1].c_str();
    //
    //            UINT type = MB_OK;
    //            if (acArgs.size() == 4)
    //                type = ParseMessageBoxType(acArgs[3]);
    //
    //            MessageBoxA(nullptr, text, caption, type);
    //        }
    //        else {
    //            std::cerr << "Unknown function: " << acName << "\n";
    //        }
    //    }
    //
    //    static UINT ParseMessageBoxType(const std::string& acString)
    //    {
    //        if (acString == "MB_OK") return MB_OK;
    //        if (acString == "MB_ICONERROR") return MB_ICONERROR;
    //        if (acString == "MB_OKCANCEL") return MB_OKCANCEL;
    //        // Extend with more types as needed
    //        return MB_OK; // fallback
    //    }
    //
    //    std::vector<std::string> SplitArgs(const std::string& str)
    //    {
    //        std::vector<std::string> result;
    //        std::stringstream ss(str);
    //        std::string item;
    //
    //        bool inQuotes = false;
    //        std::string token;
    //        for (char ch : str) {
    //            if (ch == '"') {
    //                inQuotes = !inQuotes;
    //                continue;
    //            }
    //
    //            if (ch == ',' && !inQuotes) {
    //                result.push_back(token);
    //                token.clear();
    //            } else {
    //                token += ch;
    //            }
    //        }
    //        if (!token.empty())
    //            result.push_back(token);
    //
    //        return result;
    //    }
    //
    //    template<typename Func, typename... Args, size_t... Indices>
    //    static void CallWithArgsImpl(Func&& aFunc, const std::vector<std::string>& acArgs,
    //    std::index_sequence<Indices...>)
    //    {
    //        aFunc(Convert<std::decay_t<Args>>(acArgs[Indices])...);
    //    }

    std::unordered_map<std::string, std::function<void(const std::vector<std::string>&)>> m_funcs;
};
#pragma once
#include <filesystem>
#include <windows.h>
#include "Log.h"

struct Injector
{
    static BOOL Inject(const DWORD acProcessId, LPCSTR apDllPath)
    {
        if (acProcessId == 0 || !std::filesystem::exists(apDllPath))
        {
            Log::Get()->Error("{} does not exist or invalid process ID", apDllPath);
            return FALSE;
        }

        const HANDLE cProcess = OpenProcess(
            PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE |
                PROCESS_VM_READ,
            FALSE, acProcessId);
        if (!cProcess)
        {
            char buf[256];
            FormatMessageA(
                FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, GetLastError(),
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, (sizeof(buf) / sizeof(char)), nullptr);
            Log::Get()->Error("Failed to open process: {}", buf);
            return FALSE;
        }

        const HMODULE cKernel32 = GetModuleHandleA("kernel32.dll");
        if (!cKernel32)
        {
            Log::Get()->Error("Failed to get kernel32.dll handle");
            CloseHandle(cProcess);
            return FALSE;
        }

        const LPVOID cLoadLibraryAddr = reinterpret_cast<LPVOID>(GetProcAddress(cKernel32, "LoadLibraryA"));
        if (!cLoadLibraryAddr)
        {
            Log::Get()->Error("Failed to get LoadLibraryA address");
            CloseHandle(cProcess);
            return FALSE;
        }

        const SIZE_T cDllPathLen = strlen(apDllPath) + 1;
        const LPVOID cRemoteString =
            VirtualAllocEx(cProcess, nullptr, cDllPathLen, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        if (!cRemoteString)
        {
            Log::Get()->Error("VirtualAllocEx failed");
            CloseHandle(cProcess);
            return FALSE;
        }

        BOOL result = FALSE;
        if (WriteProcessMemory(cProcess, cRemoteString, apDllPath, cDllPathLen, nullptr))
        {
            if (const HANDLE cRemoteThread = CreateRemoteThread(
                    cProcess, nullptr, 0, static_cast<LPTHREAD_START_ROUTINE>(cLoadLibraryAddr), cRemoteString, 0,
                    nullptr))
            {
                WaitForSingleObject(cRemoteThread, INFINITE);
                CloseHandle(cRemoteThread);
                result = TRUE;
            }
            else
            {
                Log::Get()->Error("CreateRemoteThread failed");
            }
        }
        else
        {
            Log::Get()->Error("WriteProcessMemory failed");
        }

        VirtualFreeEx(cProcess, cRemoteString, 0, MEM_RELEASE);
        CloseHandle(cProcess);

        return result;
    }
};
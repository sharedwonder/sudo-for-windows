// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under MIT License.

#include "SudoFramework.h"
#include "SudoConfiguration.h"
#include "SudoRpcClient.h"

#include <signal.h>

int run(LPTSTR commandLine) {
    signal(SIGINT, SIG_IGN); // Disable Ctrl+C.

    TCHAR username[20];
    {
        DWORD size = 20;
        if (!GetUserName(username, &size)) {
            return GetLastError();
        }
    }

    HANDLE (*BaseGetConsoleReference)();
    {
        HMODULE KernelBaseDLL = GetModuleHandle(TEXT("KernelBase.dll"));
        if (KernelBaseDLL != NULL) {
            BaseGetConsoleReference = (HANDLE (*)()) GetProcAddress(KernelBaseDLL, "BaseGetConsoleReference");
        } else {
            return GetLastError();
        }
    }

    TCHAR currentDirectory[MAX_PATH + 1];
    GetCurrentDirectory(MAX_PATH, currentDirectory);

    HANDLE process = INVALID_HANDLE_VALUE;

    LPTCH environment = GetEnvironmentStrings();
    DWORD environmentSize;
    {
        LPTCH pointer = environment;
        while (true) {
            ++ pointer;
            if (*pointer == TEXT('\0')) {
                ++ pointer;
                if (*pointer == TEXT('\0')) {
                    break;
                }
            }
        }
        environmentSize = (DWORD) (pointer - environment + 1);
    }

    RpcTryExcept
        DWORD errorCode = LaunchElevatedProcess(
            GetCurrentProcessId(), username, commandLine,
            (ULONG_PTR) BaseGetConsoleReference(), environmentSize, environment,
            currentDirectory, TEXT("sudo"), &((ULONG_PTR) process)
        );
        if (errorCode != ERROR_SUCCESS) {
            return errorCode;
        }
    RpcExcept(RpcExceptionFilter(RpcExceptionCode()))
        FreeEnvironmentStrings(environment);
        return RpcExceptionCode();
    RpcEndExcept

    FreeEnvironmentStrings(environment);

    // Wait for the new process to exit.
    WaitForSingleObject(process, INFINITE);

    int exitCode;

    if (!GetExitCodeProcess(process, &exitCode)) {
        CloseHandle(process);
        return GetLastError();
    }

    CloseHandle(process);
    return exitCode;
}

int _tmain(int argc, LPTSTR *argv) {
    if (argc == 1) {
        _tprintf(TEXT("Usage: sudo <command-or-program> <arguments...>\r\n"));
        return -1;
    }

    LPTSTR commandLine = GetCommandLine();
    {
        bool inQuotes = false;
        bool flag = false;
        while (true) {
            if (*commandLine == TEXT('"') && inQuotes) {
                inQuotes = false;
            } else if (*commandLine == TEXT('"')) {
                inQuotes = true;
            }

            if (*commandLine != TEXT(' ') && flag) {
                break;
            }
            if (*commandLine == TEXT(' ') && !inQuotes) {
                flag = true;
            }
            ++ commandLine;
        }
    }

    size_t newCommandLineSize = _tcslen(commandLine) + 8;
    LPTSTR newCommandLine = (LPTSTR) malloc(sizeof(LPTSTR) * newCommandLineSize);
    if (newCommandLine == NULL) {
        return ERROR_NOT_ENOUGH_MEMORY;
    }
    _stprintf_s(newCommandLine, newCommandLineSize, TEXT("cmd /c %s"), commandLine);

    DWORD status = SudoRpcClientInit();
    if (status != RPC_S_OK) {
        return status;
    }

    status = run(newCommandLine);

    SudoRpcClientFreeBinding();
    free(newCommandLine);

    return status;
}

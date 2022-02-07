// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under MIT License.

#include "SudoFramework.h"
#include "settings.h"

#include <signal.h>

_Must_inspect_result_ _Ret_maybenull_ _Post_writable_byte_size_(size)
void __RPC_FAR *__RPC_USER midl_user_allocate(_In_ size_t size) {
    return malloc(size);
}

void __RPC_USER midl_user_free(_Pre_maybenull_ _Post_invalid_ void __RPC_FAR *pointer) {
    free(pointer);
}

int run(LPTSTR commandLine) {
    ReadSudoSettings();

    signal(SIGINT, SIG_IGN); // Disable Ctrl+C.

    TCHAR userName[20];
    {
        DWORD size = 20;
        if (!GetUserName(userName, &size)) {
            return GetLastError();
        }
    }

    HANDLE (*BaseGetConsoleReferences)();
    {
        HMODULE KernelBaseDLL = GetModuleHandle(TEXT("KernelBase.dll"));
        if (KernelBaseDLL != NULL) {
            BaseGetConsoleReferences = (HANDLE (*)()) GetProcAddress(KernelBaseDLL, "BaseGetConsoleReference");
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
            GetCurrentProcessId(), userName, commandLine,
            (ULONG_PTR) BaseGetConsoleReferences(), environmentSize, environment,
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
        _tprintf(TEXT("Usage: sudo <program-or-command> [arguments...]\n"));
        return -1;
    }

    LPTSTR commandLine = GetCommandLine();
    LPTSTR newCommandLine;

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

    size_t newCommandLineLength = _tcslen(commandLine) + 8;
    newCommandLine = (LPTSTR) malloc(sizeof(LPTSTR) * newCommandLineLength);
    _stprintf_s(newCommandLine, newCommandLineLength, TEXT("cmd /c %s"), commandLine);

    _TUCHAR *stringBinding;

    RPC_STATUS status = RpcStringBindingCompose(NULL, (_TUCHAR *) TEXT("ncacn_np"), NULL,
                                                (_TUCHAR *) TEXT("\\pipe\\SudoService"), NULL, &stringBinding);
    if (status != RPC_S_OK) {
        return status;
    }

    status = RpcBindingFromStringBinding(stringBinding, &RpcBindingHandle);
    if (status != RPC_S_OK) {
        RpcStringFree(&stringBinding);
        return status;
    }

    int exitCode = run(newCommandLine);

    RpcStringFree(&stringBinding);
    RpcBindingFree(&RpcBindingHandle);

    return exitCode;
}

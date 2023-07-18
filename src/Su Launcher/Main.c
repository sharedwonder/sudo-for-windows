// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under MIT License.

#include "SudoFramework.h"
#include "SudoConfiguration.h"
#include "SudoRpcClient.h"

#include <signal.h>
#include <conio.h>

int run() {
    signal(SIGINT, SIG_IGN); // Ignore Ctrl+C.

    TCHAR username[USERNAME_MAX_LENGTH + 1];
    {
        DWORD size = USERNAME_MAX_LENGTH + 1;
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
            GetCurrentProcessId(), username, TEXT("cmd /k"),
            (ULONG_PTR) BaseGetConsoleReference(), environmentSize, environment,
            currentDirectory, TEXT("su"), &((ULONG_PTR) process)
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
    DWORD status = SudoRpcClientInit();
    if (status != RPC_S_OK) {
        return status;
    }

    status = run();

    SudoRpcClientFreeBinding();

    return status;
}

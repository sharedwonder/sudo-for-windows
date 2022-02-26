// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under MIT License.

#include "SudoFramework.h"
#include "SudoConfiguration.h"
#include "SudoRpcClient.h"

#include <signal.h>
#include <conio.h>

_Must_inspect_result_ _Ret_maybenull_ _Post_writable_byte_size_(size)
void __RPC_FAR *__RPC_USER midl_user_allocate(_In_ size_t size) {
    return malloc(size);
}

void __RPC_USER midl_user_free(_Pre_maybenull_ _Post_invalid_ void __RPC_FAR *pointer) {
    free(pointer);
}

int run() {
    signal(SIGINT, SIG_IGN); // Disable Ctrl+C.

    TCHAR userName[20];
    {
        DWORD size = 20;
        if (!GetUserName(userName, &size)) {
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
            GetCurrentProcessId(), userName, TEXT("cmd /k"),
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
    /*_TUCHAR *stringBinding;

    RPC_STATUS status = RpcStringBindingCompose(NULL, (_TUCHAR *) TEXT("ncacn_np"), NULL,
                                                (_TUCHAR *) TEXT("\\pipe\\SudoForWindows"), NULL, &stringBinding);
    if (status != RPC_S_OK) {
        return status;
    }

    status = RpcBindingFromStringBinding(stringBinding, &SudoRpcBindingHandle);
    if (status != RPC_S_OK) {
        RpcStringFree(&stringBinding);
        return status;
    }

    int exitCode = run();

    RpcStringFree(&stringBinding);
    RpcBindingFree(&SudoRpcBindingHandle);

    return exitCode;*/

    DWORD status = SudoRpcClientInit();
    if (status != RPC_S_OK) {
        return status;
    }

    status = run();

    SudoRpcClientFreeBinding();

    return status;
}

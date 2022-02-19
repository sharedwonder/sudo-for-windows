// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under MIT License.

#include "SudoFramework.h"
#include "configuration.h"
#include "password.h"

_Must_inspect_result_ _Ret_maybenull_ _Post_writable_byte_size_(size)
void __RPC_FAR *__RPC_USER midl_user_allocate(_In_ size_t size) {
    return malloc(size);
}

void __RPC_USER midl_user_free(_Pre_maybenull_ _Post_invalid_ void __RPC_FAR *pointer) {
    free(pointer);
}

int _tmain(int argc, LPTSTR *argv) {
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

    LPTSTR userName = argv[1];
    LPTSTR launcher = argv[2];

    // Try to get the password of the user and create a new process.
    for (DWORD attempt = 0; attempt < SudoConfig.AttemptLimit; ++ attempt) {
        _tprintf(TEXT("[%s] password for %s: "), launcher, userName);
        TCHAR password[PASSWORD_MAX_LENGTH + 1] = {TEXT('\0')};

        // Get the password of the user.
        if (!ReadPassword(password)) {
            if (GetLastError() == ERROR_BUFFER_OVERFLOW) { // The password is too long.
                _ftprintf(stderr, TEXT("[%s] password too long\n"), launcher);
                continue;
            } else if (GetLastError() == ERROR_CANCELLED) { // The operation cancelled by the user.
                // Operation cancelled.
                _ftprintf(stderr, TEXT("[%s] operation cancelled\n"), launcher);
            }

            goto Exit;
        }

        if (!VerifyPassword(userName, password)) {
            // For security reasons, when the password is no longer needed, fill it with zero.
            ZeroMemory(password, PASSWORD_MAX_LENGTH + 1);

            if (GetLastError() == ERROR_LOGON_FAILURE) {
                _ftprintf(stderr, TEXT("[%s] wrong password\n"), launcher);
                continue;
            }

            _ftprintf(stderr, TEXT("[%s] sorry, try again\n"), launcher);

            goto Exit;
        }
        // For security reasons, when the password is no longer needed, fill it with zero.
        ZeroMemory(password, PASSWORD_MAX_LENGTH + 1);

        SetLastError(ERROR_SUCCESS);
        goto Exit;
    }

Exit:
    RpcStringFree(&stringBinding);
    RpcBindingFree(&RpcBindingHandle);

    return GetLastError();
}

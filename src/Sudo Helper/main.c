// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under MIT License.

#include "SudoFramework.h"
#include "SudoConfiguration.h"
#include "password.h"

int _tmain(int argc, LPTSTR *argv) {
    if (argc != 3) {
        return -1;
    }

    LPTSTR username = argv[1];

    // Try to get the password of the user and create a new process.
    for (DWORD attempt = 0; attempt < SudoConfig.AttemptLimit; ++ attempt) {
        TCHAR password[PASSWORD_MAX_LENGTH + 1];

        if (_tcscmp(argv[2], TEXT("su")) == 0) {
            _tprintf(TEXT("Password: "));
        } else if (_tcscmp(argv[2], TEXT("sudo")) == 0) {
            _tprintf(TEXT("[sudo] password for %s: "), username);
        } else {
            return -1;
        }

        // Get the password of the user.
        if (!ReadPassword(password)) {
            if (GetLastError() == ERROR_BUFFER_OVERFLOW) { // The password is too long.
                _ftprintf(stderr, TEXT("Password too long.\r\n"));
                continue;
            } else if (GetLastError() == ERROR_CANCELLED) { // The operation cancelled by the user.
                // Operation cancelled.
                _ftprintf(stderr, TEXT("Operation cancelled.\r\n"));
            }

            return GetLastError();
        }

        if (!VerifyPassword(username, password)) {
            // For security reasons, when the password is no longer needed, fill it with zero.
            ZeroMemory(password, PASSWORD_MAX_LENGTH + 1);

            if (GetLastError() == ERROR_LOGON_FAILURE) {
                _ftprintf(stderr, TEXT("Authentication failure.\r\n"));
                continue;
            }

            _ftprintf(stderr, TEXT("Unknown error.\r\n"));

            return GetLastError();
        }
        // For security reasons, when the password is no longer needed, fill it with zero.
        ZeroMemory(password, PASSWORD_MAX_LENGTH + 1);

        SetLastError(ERROR_SUCCESS);
        return GetLastError();
    }

    return GetLastError();
}

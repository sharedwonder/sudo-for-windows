// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under MIT License.

#include "SudoFramework.h"
#include "SudoConfiguration.h"
#include "password.h"

int _tmain(int argc, LPTSTR *argv) {
    LPTSTR userName = argv[1];
    LPTSTR launcher = argv[2];

    // Try to get the password of the user and create a new process.
    for (DWORD attempt = 0; attempt < SudoConfig.AttemptLimit; ++ attempt) {
        _tprintf(TEXT("[%s] password for %s: "), launcher, userName);
        TCHAR password[PASSWORD_MAX_LENGTH + 1];

        // Get the password of the user.
        if (!ReadPassword(password)) {
            if (GetLastError() == ERROR_BUFFER_OVERFLOW) { // The password is too long.
                _ftprintf(stderr, TEXT("[%s] password too long\n"), launcher);
                continue;
            } else if (GetLastError() == ERROR_CANCELLED) { // The operation cancelled by the user.
                // Operation cancelled.
                _ftprintf(stderr, TEXT("[%s] operation cancelled\n"), launcher);
            }

            return GetLastError();
        }

        if (!VerifyPassword(userName, password)) {
            // For security reasons, when the password is no longer needed, fill it with zero.
            ZeroMemory(password, PASSWORD_MAX_LENGTH + 1);

            if (GetLastError() == ERROR_LOGON_FAILURE) {
                _ftprintf(stderr, TEXT("[%s] wrong password\n"), launcher);
                continue;
            }

            _ftprintf(stderr, TEXT("[%s] sorry, try again\n"), launcher);

            return GetLastError();
        }
        // For security reasons, when the password is no longer needed, fill it with zero.
        ZeroMemory(password, PASSWORD_MAX_LENGTH + 1);

        SetLastError(ERROR_SUCCESS);
        return GetLastError();
    }

    return GetLastError();
}

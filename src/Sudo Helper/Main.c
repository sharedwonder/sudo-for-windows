// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under the MIT License.

#include "SudoFramework.h"
#include "SudoConfiguration.h"
#include "Password.h"

int _tmain(int argc, LPTSTR *argv) {
    if (argc != 3) {
        return -1;
    }

    LPTSTR username = argv[1];
    LPTSTR launcher = argv[2];

    for (DWORD attempt = 0; attempt < SudoConfig.AttemptLimit; ++attempt) {
        TCHAR password[PASSWORD_MAX_LENGTH + 1];

        if (_tcscmp(argv[2], TEXT("su")) == 0) {
            _tprintf(TEXT("Password: "));
        } else if (_tcscmp(argv[2], TEXT("sudo")) == 0) {
            _tprintf(TEXT("[sudo] password for %s: "), username);
        } else {
            return -1;
        }

        if (!ReadPassword(password)) {
            if (GetLastError() == ERROR_BUFFER_OVERFLOW) {
                _ftprintf(stderr, TEXT("%s: Password too long.\r\n"), launcher);
                continue;
            } else if (GetLastError() == ERROR_CANCELLED) {
                _ftprintf(stderr, TEXT("%s: Operation cancelled.\r\n"), launcher);
            }

            return GetLastError();
        }

        BOOL successed = VerifyPassword(username, password);

        // For security reasons, when the password is no longer needed, fill it with zero
        SecureZeroMemory(password, PASSWORD_MAX_LENGTH + 1);

        if (!successed) {
            if (GetLastError() == ERROR_LOGON_FAILURE) {
                _ftprintf(stderr, TEXT("%s: Authentication failure.\r\n"), argv[2]);
                continue;
            }

            LPTSTR message;
            FormatMessage(FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
                          NULL, GetLastError(), LANG_USER_DEFAULT, (LPTSTR) &message, 0, NULL);
            _ftprintf(stderr, TEXT("%s: (%d) %s"), launcher, GetLastError(), message);
            LocalFree(message);

            return GetLastError();
        }

        SetLastError(ERROR_SUCCESS);
        return GetLastError();
    }

    return GetLastError();
}

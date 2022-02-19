// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under MIT License.

#include "password.h"

_Success_(return) BOOL ReadPassword(_Out_writes_all_(PASSWORD_MAX_LENGTH + 1) LPTSTR password) {
    TCHAR ch;
    size_t size = 0;

    password[PASSWORD_MAX_LENGTH] = '\0';

    while (true) {
        // Read a character from the console (no echo).
        ch = _gettch();

        switch (ch) {
        case EOF: // Ctrl+Z
            // The operation cancelled by the user.
            SetLastError(ERROR_CANCELLED);
            _tprintf(TEXT("\r\n"));
            return FALSE;

        case TEXT('\x1a'): case TEXT('\x3'): // Ctrl+C
            // The operation cancelled by the user.
            SetLastError(ERROR_CANCELLED);
            _tprintf(TEXT("\r\n"));
            return FALSE;

        case TEXT('\r'): // Press enter.
            _tprintf(TEXT("\r\n"));
            return TRUE;

        case TEXT('\b'): // Press backspace.
            if (size != 0) {
                password[-- size] = '\0';
            }
            break;

        default:
            password[size ++] = ch;
            break;
        }

        // The password is too long.
        if (password[PASSWORD_MAX_LENGTH] != '\0') {
            SetLastError(ERROR_BUFFER_OVERFLOW);
            _tprintf(TEXT("\r\n"));
            return FALSE;
        }
    }
}

BOOL VerifyPassword(_In_ LPTSTR userName, _In_ LPTSTR password) {
    HANDLE token;
    return LogonUser(userName, NULL, password, LOGON32_LOGON_INTERACTIVE, LOGON32_PROVIDER_DEFAULT, &token);
}

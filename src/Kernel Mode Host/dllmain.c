// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under MIT License.

#include "KernelModeHost.h"
#include "KernelModeEntry.h"

HANDLE KME_Device;
static BOOL SeDebugPrivilegeFlag;

static BOOL EnableDebugPrivilege() {
    BOOL isSuccess;
    HANDLE token;

    if (isSuccess = OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &token)) {
        TOKEN_ELEVATION elevation;
        TOKEN_PRIVILEGES privileges;
        DWORD length;

        isSuccess = GetTokenInformation(token, TokenElevation, &elevation, sizeof(elevation), &length);
        if (!isSuccess || !elevation.TokenIsElevated) {
            return FALSE;
        }

        if (isSuccess = LookupPrivilegeValue(NULL, TEXT("SeDebugPrivilege"), &(privileges.Privileges[0].Luid))) {
            PRIVILEGE_SET requiredPrivileges;
            BOOL isEnableSeDebugPrivilege;

            requiredPrivileges.Control = PRIVILEGE_SET_ALL_NECESSARY;
            requiredPrivileges.PrivilegeCount = 1;
            requiredPrivileges.Privilege[0].Luid = privileges.Privileges[0].Luid;
            requiredPrivileges.Privilege[0].Attributes = SE_PRIVILEGE_ENABLED;

            PrivilegeCheck(token, &requiredPrivileges, &isEnableSeDebugPrivilege);

            if (!isEnableSeDebugPrivilege) {
                privileges.PrivilegeCount = 1;
                privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
                isSuccess = AdjustTokenPrivileges(token, FALSE, &privileges, sizeof(privileges), NULL, NULL);
                SeDebugPrivilegeFlag = TRUE;
            } else {
                SeDebugPrivilegeFlag = FALSE;
            }
        }

        CloseHandle(token);
    }

    return isSuccess;
}

static BOOL RestoreDebugPrivilege() {
    BOOL isSuccess;
    HANDLE token;

    if (SeDebugPrivilegeFlag) {

        if (isSuccess = OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &token)) {
            TOKEN_PRIVILEGES privileges;

            if (isSuccess = LookupPrivilegeValue(NULL, TEXT("SeDebugPrivilege"), &(privileges.Privileges[0].Luid))) {
                privileges.PrivilegeCount = 1;
                privileges.Privileges[0].Attributes = SE_PRIVILEGE_REMOVED;
                isSuccess = AdjustTokenPrivileges(token, FALSE, &privileges, 0, NULL, 0);
            }

            CloseHandle(token);
        }
    } else {
        isSuccess = TRUE;
    }

    return isSuccess;
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved) {
    switch (reason) {
    case DLL_PROCESS_ATTACH:
    {
        if (KME_Device != NULL) {
            break;
        }

        if (!EnableDebugPrivilege()) {
            return FALSE;
        }

        KME_Device = CreateFile(TEXT(KME_DEVICE_PATH), GENERIC_READ | GENERIC_WRITE, 0,
                                NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (KME_Device == INVALID_HANDLE_VALUE) {
            return FALSE;
        }
    }
        break;

    case DLL_PROCESS_DETACH:
    {
        if (KME_Device != NULL) {
            CloseHandle(KME_Device);
        }

        if (!RestoreDebugPrivilege()) {
            return FALSE;
        }
    }
        break;

    default:
        break;
    }

    return TRUE;
}

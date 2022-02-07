// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under MIT License.

#include "settings.h"

#include "framework.h"

#define ACCESS_CHECK_MODE 1
#define ALLOW_UNLOAD_DRIVER TRUE
#define ALLOW_OTHER_PROGRAMS_MODIFY_SETTINGS TRUE

static HANDLE keyHandle;

inline VOID SetDefaultValues(_Out_ KME_DriverSettings *driverSettings) {
    driverSettings->AccessCheckMode = ACCESS_CHECK_MODE;
    driverSettings->AllowUnloadDriver = ALLOW_UNLOAD_DRIVER;
    driverSettings->AllowOtherProgramsModifySettings = ALLOW_OTHER_PROGRAMS_MODIFY_SETTINGS;
}

VOID DriverInitSettings(_Out_ KME_DriverSettings *driverSettings) {
    OBJECT_ATTRIBUTES attrib;
    UNICODE_STRING keyPath;

    SetDefaultValues(driverSettings);

    RtlInitUnicodeString(&keyPath, L"\\Registry\\Machine\\SOFTWARE\\Sudo for Windows\\Kernel Mode\\Kernel Mode Entry\\Settings");

    InitializeObjectAttributes(&attrib, &keyPath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
    ZwOpenKey(&keyHandle, KEY_READ, &attrib);

    DriverLoadSettings(driverSettings);
}

VOID DriverLoadSettings(_Out_ KME_DriverSettings *driverSettings) {
    NTSTATUS status = STATUS_SUCCESS;

    UNICODE_STRING valueName;
    KEY_VALUE_PARTIAL_INFORMATION valueInfo;
    ULONG size;

#define GET_SETTING(Name, ConvertType, CType, KeyValueType) \
    if (keyHandle != NULL) { \
        PCWSTR src = L ## #Name; \
        RtlInitUnicodeString(&valueName, src); \
        status = ZwQueryValueKey(keyHandle, &valueName, KeyValuePartialInformation, &valueInfo, sizeof(valueInfo), &size); \
        if (NT_SUCCESS(status) && size != 0) { \
            status = ZwQueryValueKey(keyHandle, &valueName, KeyValuePartialInformation, &valueInfo, size, &size); \
            if (NT_SUCCESS(status) && valueInfo.Type == KeyValueType && valueInfo.DataLength == sizeof(CType)) { \
                driverSettings->Name = (ConvertType) *((CType *) valueInfo.Data); \
            } \
        } \
    } \
    KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, \
               "[KernelModeEntry: DriverLoadSettings] Settings:\n\t" #Name " (" #ConvertType "): %d.\n", \
               driverSettings->Name))

    GET_SETTING(AccessCheckMode, UCHAR, ULONG, REG_DWORD);
    GET_SETTING(AllowUnloadDriver, BOOLEAN, ULONG, REG_DWORD);
    GET_SETTING(AllowOtherProgramsModifySettings, BOOLEAN, ULONG, REG_DWORD);

    KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL,
               "[KernelModeEntry: DriverLoadSettings] The driver loaded settings.\n"));
}

VOID DriverSaveSettings(
    _In_ KME_DriverSettings *driverSettings
) {
    UNREFERENCED_PARAMETER(driverSettings);
}

VOID DriverResetSettings(_Out_opt_ KME_DriverSettings *driverSettings) {
    if (driverSettings != NULL) {
        SetDefaultValues(driverSettings);
    }
}

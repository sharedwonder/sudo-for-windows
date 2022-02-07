// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under MIT License.

#pragma once

#include "framework.h"

EXTERN_C_START

typedef struct _DriverSettings {
    BOOLEAN AllowUnloadDriver; // default: 1 (true)
    UCHAR AccessCheckMode; // default: 1 (Check SeDebugPrivilege)
    BOOLEAN AllowOtherProgramsModifySettings; // default: 1 (true)
} KME_DriverSettings;

VOID DriverInitSettings(
    _Out_ KME_DriverSettings *driverSettings
);

VOID DriverLoadSettings(
    _Out_ KME_DriverSettings *driverSettings
);

VOID DriverSaveSettings(
    _In_ KME_DriverSettings *driverSettings
);

VOID DriverResetSettings(
    _Out_opt_ KME_DriverSettings *driverSettings
);

EXTERN_C_END

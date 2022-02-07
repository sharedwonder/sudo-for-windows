// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under MIT License.

#pragma once

#include "SudoFramework.h"

EXTERN_C_START

typedef struct {
    DWORD ATTEMPT_LIMIT; // default: 3
} SUDO_HELPER_SETTINGS;

extern SUDO_HELPER_SETTINGS SudoHelperSettings;

void ReadSudoSettings();

EXTERN_C_END

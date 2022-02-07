// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under MIT License.

#pragma once

#include "settings.h"

EXTERN_C_START

SUDO_HELPER_SETTINGS SudoHelperSettings;

void ReadSudoSettings() {
    SudoHelperSettings.ATTEMPT_LIMIT = 3;
}

EXTERN_C_END

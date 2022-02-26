// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under MIT License.

#pragma once

#include "SudoFramework.h"

EXTERN_C_START

typedef struct SUDO_CONFIG {
    DWORD AttemptLimit;
} SUDO_CONFIG;

typedef SUDO_CONFIG *PSUDO_CONFIG;

#ifdef SUDOCONFIGURATION_EXPORTS
#define SUDO_CONFIG_API __declspec(dllexport)
#else
#define SUDO_CONFIG_API __declspec(dllimport)
#endif // SUDOCONFIGURATION_EXPORTS

SUDO_CONFIG_API extern SUDO_CONFIG SudoConfig;

SUDO_CONFIG_API _Success_(return) BOOL LoadSudoConfig(_Out_ PSUDO_CONFIG config);

SUDO_CONFIG_API _Success_(return) BOOL UnloadSudoConfig();

SUDO_CONFIG_API _Success_(return) BOOL UpdateSudoConfig(_In_ PSUDO_CONFIG config);

SUDO_CONFIG_API _Success_(return) BOOL ResetSudoConfig(_Out_ PSUDO_CONFIG config);

EXTERN_C_END

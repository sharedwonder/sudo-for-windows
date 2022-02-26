// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under MIT License.

#pragma once

#include "SudoFramework.h"

EXTERN_C_START

// The max length of a password in Windows.
#define PASSWORD_MAX_LENGTH 257

_Success_(return) BOOL ReadPassword(_Out_writes_z_(PASSWORD_MAX_LENGTH + 1) LPTSTR password);

BOOL VerifyPassword(_In_ LPTSTR userName, _In_ LPTSTR password);

EXTERN_C_END

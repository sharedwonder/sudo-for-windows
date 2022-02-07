// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under MIT License.

#pragma once

#include "SudoFramework.h"

// The max length of a password in Windows.
#define PASSWORD_MAX_LENGTH 257

_Success_(return) BOOL ReadPassword(_Out_writes_all_(PASSWORD_MAX_LENGTH + 1) LPTSTR password);

BOOL VerifyPassword(LPTSTR userName, LPTSTR password);

// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under MIT License.

#pragma once

#include "SudoFramework.h"

EXTERN_C_START

// The max length of a password in Windows.
#define PASSWORD_MAX_LENGTH 257

/*!
 * Reads a password from the console (no echo).
 *
 * @param password The answer.
 * @return If successful, returns the true, otherwise returns false.
 */
_Success_(return) BOOL ReadPassword(_Out_writes_z_(PASSWORD_MAX_LENGTH + 1) LPTSTR password);

/*!
 * Verifies that the password is valid.
 *
 * @param username The username.
 * @param password The password to verify.
 * @return BOOL The answer.
 */
BOOL VerifyPassword(_In_ LPTSTR username, _In_ LPTSTR password);

EXTERN_C_END

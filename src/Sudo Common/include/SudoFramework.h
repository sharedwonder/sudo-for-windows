// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under MIT License.

#pragma once

/*
 * In the C language, when you include the header file "windows.h",
 * and if the macro MICROSOFT_WINDOWS_WINBASE_H_DEFINE_INTERLOCKED_CPLUSPLUS_OVERLOADS
 * is not defined as 0, the compiler will display the warning C5105 in compiling.
 * But don't need to do this in C++ language.
 *
 * This problem appears in the Windows SDK 10.0.19041 and later.
 * I tried to reinstall the Windows SDK, but it didn't have any effect.
 */
#ifndef __cplusplus
#define MICROSOFT_WINDOWS_WINBASE_H_DEFINE_INTERLOCKED_CPLUSPLUS_OVERLOADS 0
#endif // !__cplusplus

#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// The maximum length of username in Windows.
#define USERNAME_MAX_LENGTH 20

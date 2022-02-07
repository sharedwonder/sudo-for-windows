// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under MIT License.

#pragma once

/*
 * In the C language, when you include the header file "windows.h",
 * if the macro MICROSOFT_WINDOWS_WINBASE_H_DEFINE_INTERLOCKED_CPLUSPLUS_OVERLOADS
 * is not defined as 0, it will display the warning C5105 in compiling.
 * And the code segment associated with the macro is compiled in the C++ language.
 * So you don't have to worry about what errors or warnings will cause.
 * In C++ language, you don't need to do this.
 *
 * This problem appears in the Windows SDK 10.0.19041 and 10.0.22000 versions,
 * and I don't know if there will be a problem in other versions.
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

#if defined(_M_X64) // x64
#include "SudoRpcX64.h"
#elif defined(_M_IX86) // x86
#include "SudoRpcX86.h"
#else // Unknown
#error Unknown platform. Supportted platfroms: x64, x86.
#endif // defined(_M_X64)

// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under MIT License.

#pragma once

/*
 * In the C language, when you include the header file "windows.h",
 * and if the macro MICROSOFT_WINDOWS_WINBASE_H_DEFINE_INTERLOCKED_CPLUSPLUS_OVERLOADS
 * is not defined as 0, the compiler will display the warning C5105 in compiling.
 * But don't need to do this in C++ language.
 */
#ifndef __cplusplus
    #define MICROSOFT_WINDOWS_WINBASE_H_DEFINE_INTERLOCKED_CPLUSPLUS_OVERLOADS 0
#endif // !__cplusplus

#include <Windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define USERNAME_MAX_LENGTH 20 // The maximum length of username in Windows.
#define PID_MAX_LENGTH 10
#define EXIT_CODE_MAX_LENGTH 10
#define ERROR_CODE_MAX_LENGTH 6

 // The undisclosed console reference attribute.
#define PROC_THREAD_ATTRIBUTE_CONSOLE_REFERENCE ProcThreadAttributeValue((PROC_THREAD_ATTRIBUTE_NUM) 10, FALSE, TRUE, FALSE)

// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under MIT License.

#pragma once

#ifdef _KERNEL_MODE
#include "framework.h"
#else
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
#endif // _KERNEL_MODE

#define KME_DEVICE_NAME "KernelModeEntry"

#ifdef _KERNEL_MODE
#define KME_DEVICE_PATH "\\Device\\KernelModeEntry"
#else
#define KME_DEVICE_PATH "\\\\.\\GLOBALROOT\\Device\\KernelModeEntry"
#endif // _KERNEL_MODE

#define CONVERT_TO_UNICODE_IMPL(string) L ## string
#define CONVERT_TO_UNICODE(string) CONVERT_TO_UNICODE_IMPL(string)

#define KME_CONTROL_CODE(x) CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800 + x, METHOD_BUFFERED, FILE_ANY_ACCESS)

typedef enum {
    FC_NT_OPEN_PROCESS,
    NT_OPEN_PROCESS_TOKEN_EX,
    FC_NT_TERMINAL_PROCESS,
    FC_NT_CLOSE,
} KME_FUNCTION_CODE;

typedef enum {
    NT_OPEN_PROCESS = KME_CONTROL_CODE(FC_NT_OPEN_PROCESS),
    NT_OPEN_PROCESS_TOKEN_EX = KME_CONTROL_CODE(NT_OPEN_PROCESS_TOKEN_EX),
    NT_TERMINATE_PROCESS = KME_CONTROL_CODE(FC_NT_TERMINAL_PROCESS),
    NT_CLOSE = KME_CONTROL_CODE(FC_NT_CLOSE)
} KME_CONTROL_CODE;

typedef struct {
    _Out_ PHANDLE processHandle;
    _In_ ACCESS_MASK desiredAccess;
    _In_ POBJECT_ATTRIBUTES objectAttributes;
    _In_opt_ PCLIENT_ID clientId;
} ARGS_NT_OPEN_PROCESS;

typedef struct {
    _In_ HANDLE processHandle;
    _In_ ACCESS_MASK desiredAccess;
    _In_ ULONG handleAttributes;
    _Out_ PHANDLE tokenHandle;
} ARGS_NT_OPEN_PROCESS_TOKEN_EX;

typedef struct {
    _In_opt_ HANDLE processHandle;
    _In_ NTSTATUS exitStatus;
} ARGS_NT_TERMINATE_PROCESS;

typedef struct {
    _In_ HANDLE handle;
} ARGS_NT_CLOSE;

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

#include "KMH_Framework.h"

#ifdef NTKERNELAPILIBRARY_EXPORTS
#define KERNEL_API __declspec(dllexport)
#else
#define KERNEL_API __declspec(dllimport)
#endif // NTKERNELAPILIBRARY_EXPORTS

#define WINNT_API KERNEL_API

#ifdef _DEBUG
#define KERNEL_MODE_HOST_DLL_NAME TEXT("KernelModeHost_d.dll")
#else
#define KERNEL_MODE_HOST_DLL_NAME TEXT("KernelModeHost.dll")
#endif // _DEBUG

EXTERN_C_START

typedef HMODULE KernelModeHostLibrary;

// Native NT kernel functions.

KERNEL_API NTSTATUS NTAPI NtOpenProcess(
    _Out_ PHANDLE processHandle,
    _In_ ACCESS_MASK desiredAccess,
    _In_ POBJECT_ATTRIBUTES objectAttributes,
    _In_opt_ PCLIENT_ID clientId
);

KERNEL_API NTSTATUS NTAPI NtOpenProcessTokenEx(
    _In_ HANDLE processHandle,
    _In_ ACCESS_MASK desiredAccess,
    _In_ ULONG handleAttributes,
    _Out_ PHANDLE tokenHandle
);

KERNEL_API NTSTATUS NTAPI NtTerminateProcess(
    _In_opt_ HANDLE processHandle,
    _In_ UINT exitStatus
);

KERNEL_API NTSTATUS NTAPI NtClose(
    _In_ HANDLE handle
);

EXTERN_C_END

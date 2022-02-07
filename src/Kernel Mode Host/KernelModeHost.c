// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under MIT License.

#include "KernelModeHost.h"

#include "KernelModeEntry.h"
#include "DriverControl.h"

NTSTATUS NTAPI NtOpenProcess(_Out_ PHANDLE processHandle, _In_ ACCESS_MASK desiredAccess,
                       _In_ POBJECT_ATTRIBUTES objectAttributes, _In_opt_ PCLIENT_ID clientId) {
    INLINE_CALL_KERNEL_API(
        NT_OPEN_PROCESS, {
            input.processHandle = processHandle;
            input.desiredAccess = desiredAccess;
            input.objectAttributes = objectAttributes;
            input.clientId = clientId;
        }
    );
}

NTSTATUS NTAPI NtOpenProcessTokenEx(_In_ HANDLE processHandle, _In_ ACCESS_MASK desiredAccess,
                                    _In_ ULONG handleAttributes, _Out_ PHANDLE tokenHandle) {
    INLINE_CALL_KERNEL_API(
        NT_OPEN_PROCESS_TOKEN_EX, {
            input.processHandle = processHandle;
            input.desiredAccess = desiredAccess;
            input.handleAttributes = handleAttributes;
            input.tokenHandle = tokenHandle;
        }
    )
}

NTSTATUS NTAPI NtTerminateProcess(_In_opt_ HANDLE processHandle, _In_ UINT exitStatus) {
    INLINE_CALL_KERNEL_API(
        NT_TERMINATE_PROCESS, {
            input.processHandle = processHandle;
            input.exitStatus = exitStatus;
        }
    );
}

NTSTATUS NTAPI NtClose(_In_ HANDLE handle) {
    INLINE_CALL_KERNEL_API(
        NT_CLOSE, {
            input.handle = handle;
        }
    );
}

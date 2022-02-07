// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under MIT License.

#include "ioControl.h"

#include "framework.h"

NTSTATUS KME_DispatchCreate(_In_ PDEVICE_OBJECT driver, _In_ PIRP irp) {
    UNREFERENCED_PARAMETER(driver);

    NTSTATUS status = STATUS_SUCCESS;
    PRIVILEGE_SET requiredPrivileges;
    PIO_STACK_LOCATION irpStackLocation = IoGetCurrentIrpStackLocation(irp);
    PIO_SECURITY_CONTEXT securityContext = irpStackLocation->Parameters.Create.SecurityContext;

    requiredPrivileges.Control = PRIVILEGE_SET_ALL_NECESSARY;
    requiredPrivileges.PrivilegeCount = 1;
    requiredPrivileges.Privilege[0].Luid.HighPart = 0;
    requiredPrivileges.Privilege[0].Luid.LowPart = SE_DEBUG_PRIVILEGE;
    requiredPrivileges.Privilege[0].Attributes = 0;

    if (!SePrivilegeCheck(&requiredPrivileges,
        &(securityContext->AccessState->SubjectSecurityContext), irp->RequestorMode)) {
        status = STATUS_PRIVILEGE_NOT_HELD;
        KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
                   "[KernelModeEntry: KME_DispatchCreate] The client have no required privileges (SE_DEBUG_PRIVILEGE).\n"));
        KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
                   "[KernelModeEntry: KME_DispatchCreate] The client unable connect.\n"));
        goto End;
    }

    KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL,
               "[KernelModeEntry: KME_DispatchCreate] The client is successfully connected.\n"));

End:
    irp->IoStatus.Status = status;
    irp->IoStatus.Information = 0;
    IoCompleteRequest(irp, IO_NO_INCREMENT);

    return status;
}

NTSTATUS KME_DispatchClose(_In_ PDEVICE_OBJECT driver, _In_ PIRP irp) {
    UNREFERENCED_PARAMETER(driver);

    NTSTATUS status = STATUS_SUCCESS;

    irp->IoStatus.Status = status;
    irp->IoStatus.Information = 0;
    IoCompleteRequest(irp, IO_NO_INCREMENT);

    KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL,
               "[KernelModeEntry: KME_DispatchClose] The client is successfully disconnected.\n"));

    return status;
}

NTSTATUS KME_DispatchDeviceControl(_In_ PDEVICE_OBJECT driver, _In_ PIRP irp) {
    UNREFERENCED_PARAMETER(driver);

    NTSTATUS status = STATUS_SUCCESS;
    PIO_STACK_LOCATION irpStackLocation = IoGetCurrentIrpStackLocation(irp);
    ULONG controlCode = irpStackLocation->Parameters.DeviceIoControl.IoControlCode;
    ULONG inputLength = irpStackLocation->Parameters.DeviceIoControl.InputBufferLength;
    PVOID originalInput = irp->AssociatedIrp.SystemBuffer;

    if (inputLength != 0 && originalInput == NULL) {
        status = STATUS_INVALID_BUFFER_SIZE;
        goto End;
    }

#define CTRL_CASE(Name, Function) \
    case Name: \
    { \
        ARGS_ ## Name *input = (ARGS_ ## Name *) originalInput; \
        if (inputLength > 0 && input == NULL) { \
            status = STATUS_INVALID_PARAMETER; \
            goto End; \
        } \
        if (inputLength != sizeof(*input)) { \
            status = STATUS_INFO_LENGTH_MISMATCH; \
            goto End; \
        } \
        status = Function; \
    } \
        break
    ZwOpenProcessTokenEx;
    switch (controlCode) {
        CTRL_CASE(NT_OPEN_PROCESS, ZwOpenProcess(input->processHandle, input->desiredAccess, input->objectAttributes, input->clientId));
        CTRL_CASE(NT_TERMINATE_PROCESS, ZwTerminateProcess(input->processHandle, input->exitStatus));
        CTRL_CASE(NT_OPEN_PROCESS_TOKEN_EX,
                  ZwOpenProcessTokenEx(input->processHandle, input->desiredAccess, input->handleAttributes, input->tokenHandle));
        CTRL_CASE(NT_CLOSE, ZwClose(input->handle));

    default: // Invalid control code.
        status = STATUS_INVALID_DEVICE_REQUEST;
        break;
    }

#undef CTRL_CASE

End:
    irp->IoStatus.Status = status;
    irp->IoStatus.Information = status;
    IoCompleteRequest(irp, IO_NO_INCREMENT);

    return status;
}

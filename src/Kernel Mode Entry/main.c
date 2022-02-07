// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under MIT License.

#include "framework.h"
#include "ioControl.h"
#include "settings.h"

/* Driver loading funtion. */
DRIVER_INITIALIZE DriverEntry;

/* Driver unloading function. */
DRIVER_UNLOAD DriverUnload;

/* The device of this driver. */
PDEVICE_OBJECT KME_Device;

/* The path of the device. */
UNICODE_STRING KME_DevicePath;

KME_DriverSettings driverSettings;

NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT driver, _In_ PUNICODE_STRING registryPath) {
    UNREFERENCED_PARAMETER(registryPath);

    NTSTATUS status = STATUS_SUCCESS;

    // Create a device of this driver.
    RtlInitUnicodeString(&KME_DevicePath, CONVERT_TO_UNICODE(KME_DEVICE_PATH));
    status = IoCreateDevice(driver, 0, &KME_DevicePath, FILE_DEVICE_UNKNOWN,
        FILE_DEVICE_SECURE_OPEN, FALSE, &KME_Device);
    if (!NT_SUCCESS(status)) {
        return status;
    }

    DriverInitSettings(&driverSettings);

    // Set I/O control functions.
    driver->MajorFunction[IRP_MJ_CREATE] = KME_DispatchCreate;
    driver->MajorFunction[IRP_MJ_CLOSE] = KME_DispatchClose;
    driver->MajorFunction[IRP_MJ_DEVICE_CONTROL] = KME_DispatchDeviceControl;

    // Set DriverUnload function.
    if (driverSettings.AllowUnloadDriver) {
        driver->DriverUnload = DriverUnload;
    }

    KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "[KernelModeEntry: DriverEntry] Driver loaded.\n"));

    return status;
}

VOID DriverUnload(_In_ PDRIVER_OBJECT driver) {
    UNREFERENCED_PARAMETER(driver);

    // Delete the device.
    IoDeleteDevice(KME_Device);

    KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "[KernelModeEntry: DriverUnload] Driver unloaded.\n"));
}

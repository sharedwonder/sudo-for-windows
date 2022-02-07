// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under MIT License.

#pragma once

#include "KMH_Framework.h"

EXTERN_C HANDLE KME_Device;

#define KME_DEVICE_CONTROL(ControlCode, Input, StatusRef) \
    DeviceIoControl(KME_Device, ControlCode, &Input, sizeof(Input), NULL, 0, (LPDWORD) StatusRef, NULL)

#define INLINE_CALL_KERNEL_API(Name, SetupInput) \
    ARGS_ ## Name input; \
    SetupInput \
    NTSTATUS status;\
    KME_DEVICE_CONTROL(Name, input, &status); \
    return status; \

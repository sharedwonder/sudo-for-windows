// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under MIT License.

#pragma once

#include "framework.h"

EXTERN_C_START

NTSTATUS KME_DispatchCreate(
	_In_ PDEVICE_OBJECT driver,
	_In_ PIRP irp
);

NTSTATUS KME_DispatchClose(
	_In_ PDEVICE_OBJECT driver,
	_In_ PIRP irp
);

NTSTATUS KME_DispatchDeviceControl(
	_In_ PDEVICE_OBJECT driver,
	_In_ PIRP irp
);

EXTERN_C_END

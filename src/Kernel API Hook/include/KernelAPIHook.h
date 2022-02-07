// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under MIT License.

#pragma once

#include "KernelModeHost.h"

#ifdef _DEBUG
#define KRNLAPI_HOOK_DLL_NAME TEXT("KernelAPIHook_d.dll")
#else
#define KRNLAPI_HOOK_DLL_NAME TEXT("KernelAPIHook.dll")
#endif // _DEBUG

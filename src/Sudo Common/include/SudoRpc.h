// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under MIT License.

#include "SudoFramework.h"

typedef _TUCHAR *RPC_TSTR;

#if defined(_M_X64) // x64
#include "SudoRpcX64.h"
#elif defined(_M_IX86) // x86
#include "SudoRpcX86.h"
#elif defined(_M_ARM) // ARM
#include "SudoRpcARM.h"
#elif defined(_M_ARM64)
#include "SudoRpcARM64.h"
#else
#error Unknown platform. Supported platfroms: x64, x86, ARM, ARM64.
#endif // defined(_M_X64)

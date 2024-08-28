// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under the MIT License.

#include "SudoFramework.h"

typedef _TUCHAR *RPC_TSTR;

#if defined(_M_X64)
    #include "SudoRpcX64.h"
#elif defined(_M_IX86)
    #include "SudoRpcX86.h"
#elif defined(_M_ARM)
    #include "SudoRpcARM.h"
#elif defined(_M_ARM64)
    #include "SudoRpcARM64.h"
#else
    #error Unsupported architecture (supported architectures: x64, x86, ARM, ARM64)
#endif

// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under MIT License.

#include "SudoRpc.h"

EXTERN_C_START

DWORD WINAPI SudoRpcServerRun();

_Success_(return == ERROR_SUCCESS) RPC_STATUS WINAPI SudoRpcServerShutdown();

EXTERN_C_END

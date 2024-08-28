// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under the MIT License.

#include "SudoRpc.h"

EXTERN_C_START

/*!
 * Listens for remote procedure calls.
 *
 * @return DWORD The exit code.
 */
DWORD WINAPI SudoRpcServerListen();

/*!
 * Stops listening for remote procedure calls.
 *
 * @return DWORD The return code, if successful, returns ERROR_SUCCESS (0).
 */
_Success_(return == ERROR_SUCCESS) RPC_STATUS WINAPI SudoRpcServerShutdown();

EXTERN_C_END

// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under the MIT License.

#include "SudoRpc.h"

#ifdef SUDORPCCLIENT_EXPORTS
    #define SUDO_RPC_CLIENT_API __declspec(dllexport)
#else
    #define SUDO_RPC_CLIENT_API __declspec(dllimport)
#endif

EXTERN_C_START

/*!
 * Initializes the remote procedure calls client.
 *
 * @return DWORD The return code, if successful, returns ERROR_SUCCESS (0).
 */
SUDO_RPC_CLIENT_API _Success_(return == ERROR_SUCCESS) DWORD WINAPI SudoRpcClientInit();

/*!
 * Frees the binding to the server.
 */
SUDO_RPC_CLIENT_API void WINAPI SudoRpcClientFreeBinding();

EXTERN_C_END

// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under MIT License.

#include "SudoRpc.h"

#ifdef SUDORPCCLIENT_EXPORTS
#define SUDO_RPC_CLIENT_API __declspec(dllexport)
#else
#define SUDO_RPC_CLIENT_API __declspec(dllimport)
#endif // SUDORPCCLIENT_EXPORTS

EXTERN_C_START

SUDO_RPC_CLIENT_API _Success_(return == ERROR_SUCCESS) DWORD WINAPI SudoRpcClientInit();

SUDO_RPC_CLIENT_API void WINAPI SudoRpcClientFreeBinding();

EXTERN_C_END

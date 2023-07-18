// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under MIT License.

#include "SudoRpcClient.h"

_Success_(return == ERROR_SUCCESS) DWORD WINAPI SudoRpcClientInit() {
    RPC_TSTR stringBinding;

    RPC_STATUS status = RpcStringBindingCompose(NULL, (RPC_TSTR) TEXT("ncacn_np"), NULL,
                                                (RPC_TSTR) TEXT("\\pipe\\SudoForWindows"), NULL, &stringBinding);
    if (status != RPC_S_OK) {
        return status;
    }

    status = RpcBindingFromStringBinding(stringBinding, &SudoRpcBindingHandle);
    RpcStringFree(&stringBinding);

    return status;
}

void WINAPI SudoRpcClientFreeBinding() {
    RpcBindingFree(&SudoRpcBindingHandle);
}

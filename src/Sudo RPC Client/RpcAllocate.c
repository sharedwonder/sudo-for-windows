// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under the MIT License.

#include "SudoRpc.h"

_Must_inspect_result_ _Ret_maybenull_ _Post_writable_byte_size_(size)
void __RPC_FAR *__RPC_USER midl_user_allocate(_In_ size_t size) {
    return malloc(size);
}

void __RPC_USER midl_user_free(_Pre_maybenull_ _Post_invalid_ void __RPC_FAR *pointer) {
    free(pointer);
}

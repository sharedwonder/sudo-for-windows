// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under MIT License.

#include "KernelApiHook.h"

#include <detours.h>

static HMODULE NTDLL;
static HMODULE KERNEL_MODE_HOST;
static LPSTR APIName[] = {"NtOpenProcess", "NtOpenProcessTokenEx", "NtTerminateProcess", "NtClose"};

static VOID HooksAttach() {
    DetourRestoreAfterWith();
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    for (size_t index = 0; index < sizeof(APIName) / sizeof(APIName[0]); ++ index) {
        DetourAttach(GetProcAddress(NTDLL, APIName[index]), GetProcAddress(KERNEL_MODE_HOST, APIName[index]));
    }

    DetourTransactionCommit();
}

static VOID HooksDetach() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    for (size_t index = 0; index < sizeof(APIName) / sizeof(APIName[0]); ++ index) {
        DetourDetach(GetProcAddress(NTDLL, APIName[index]), GetProcAddress(KERNEL_MODE_HOST, APIName[index]));
    }

    DetourTransactionCommit();
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved) {
    static BOOLEAN attached;

    switch (reason) {
    case DLL_PROCESS_ATTACH:
    {
        if (attached) {
            break;
        }

        NTDLL = GetModuleHandle(TEXT("ntdll.dll"));
        if (NTDLL == NULL) {
            return FALSE;
        }
        KERNEL_MODE_HOST = LoadLibrary(KERNEL_MODE_HOST_DLL_NAME);
        if (NTDLL == NULL) {
            return FALSE;
        }

        HooksAttach();
        attached = TRUE;
    }
        break;

    case DLL_PROCESS_DETACH:
    {
        HooksDetach();
        FreeLibrary(KERNEL_MODE_HOST);
        attached = FALSE;
    }
        break;

    default:
        break;
    }

    return TRUE;
}

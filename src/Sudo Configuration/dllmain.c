// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under MIT License.

#include "SudoFramework.h"
#include "SudoConfiguration.h"

SUDO_CONFIG SudoConfig;

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved) {
    switch (reason) {
    case DLL_PROCESS_ATTACH:
        return LoadSudoConfig(&SudoConfig);

    case DLL_THREAD_ATTACH: case DLL_THREAD_DETACH:
        break;

    case DLL_PROCESS_DETACH:
        return UnloadSudoConfig();
    }

    return TRUE;
}

// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under the MIT License.

#include "SudoConfiguration.h"

static HKEY configKey;

_Success_(return) BOOL LoadSudoConfig(_Out_ PSUDO_CONFIG config) {
#define GET_CONFIG(name) \
    { \
        DWORD size = sizeof(config->name); \
        if (RegQueryValueEx(configKey, TEXT(#name), NULL, NULL, (LPBYTE) &config->name, &size) != ERROR_SUCCESS) { \
            config->name = SudoConfigDefaultValue ## name; \
        } \
    }

#define GET_CONFIG_EX(name, cType, valueType) \
    { \
        cType value; \
        DWORD size = sizeof(config->name); \
        if (RegQueryValueEx(configKey, TEXT(#name), NULL, NULL, (LPBYTE) &config->name, &size) != ERROR_SUCCESS) { \
            config->name = SudoConfigDefaultValue ## name; \
        } \
        config->name = (valueType) value; \
    }

    if (RegOpenKey(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Sudo for Windows\\config"), &configKey) != ERROR_SUCCESS) {
        if (RegCreateKey(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Sudo for Windows\\config"), &configKey) != ERROR_SUCCESS) {
            return FALSE;
        }
    }

    GET_CONFIG(AttemptLimit);

#undef GET_CONFIG
#undef GET_CONFIG_EX

    return TRUE;
}

_Success_(return) BOOL UnloadSudoConfig() {
    return RegCloseKey(configKey);
}

_Success_(return) BOOL UpdateSudoConfig(_In_ PSUDO_CONFIG config) {
#define SET_CONFIG(name, regType) \
    if (RegSetKeyValue(configKey, NULL, TEXT(#name), regType, &config->name, sizeof(config->name)) != 0) { \
        return FALSE; \
    }

    SET_CONFIG(AttemptLimit, REG_DWORD);

#undef SET_CONFIG

    return TRUE;
}

_Success_(return) BOOL ResetSudoConfig(_Out_ PSUDO_CONFIG config) {
    SUDO_CONFIG defaultConfig = {
        .AttemptLimit = SudoConfigDefaultValueAttemptLimit
    };

    UpdateSudoConfig(&defaultConfig);

    if (config != NULL) {
        config = &defaultConfig;
    }

    return TRUE;
}

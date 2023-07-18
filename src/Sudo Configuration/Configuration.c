// Copyright (c) Liu Baihao. All rights reserved.
// This software is licensed under the MIT License.

#include "SudoConfiguration.h"

#define GET_CONFIG(name) \
    { \
        DWORD size = sizeof(config->name); \
        if (RegQueryValueEx(configKey, TEXT(#name), NULL, NULL, (LPBYTE) &config->name, &size) != ERROR_SUCCESS) { \
            config->name = DefaultValue ## name; \
        } \
    }

#define GET_CONFIG_EX(name, cType, valueType) \
    { \
        cType value; \
        DWORD size = sizeof(config->name); \
        if (RegQueryValueEx(configKey, TEXT(#name), NULL, NULL, (LPBYTE) &config->name, &size) != ERROR_SUCCESS) { \
            config->name = DefaultValue ## name; \
        } \
        config->name = (valueType) value; \
    }

#define DefaultValueAttemptLimit 3

static HKEY configKey;

_Success_(return) BOOL LoadSudoConfig(_Out_ PSUDO_CONFIG config) {
    if (RegOpenKey(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Sudo for Windows\\config"), &configKey) != ERROR_SUCCESS) {
        if (RegCreateKey(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Sudo for Windows\\config"), &configKey) != ERROR_SUCCESS) {
            return FALSE;
        }
    }

    // Read configuration from the registry.
    GET_CONFIG(AttemptLimit);

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
        .AttemptLimit = DefaultValueAttemptLimit
    };

    UpdateSudoConfig(&defaultConfig);

    if (config != NULL) {
        config = &defaultConfig;
    }

    return TRUE;
}

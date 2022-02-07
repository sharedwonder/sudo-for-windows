// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under MIT License.

#include "SudoFramework.h"

EXTERN_C_START

// The undisclosed console reference attribute.
#define PROC_THREAD_ATTRIBUTE_CONSOLE_REFERENCE ProcThreadAttributeValue((PROC_THREAD_ATTRIBUTE_NUM) 10, FALSE, TRUE, FALSE)

enum LOG_TYPE {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_DEBUG
};

_Success_(return) BOOL QueryTokenInformation(_In_ HANDLE token, _In_ TOKEN_INFORMATION_CLASS tokenInfoClass, _Out_opt_ LPVOID tokenInfo);

BOOL TokenIsElevated(_In_ HANDLE token);

void WriteToLogFile(LPTSTR message, enum LOG_TYPE logType);

void WINAPI __callback ControlHandler(DWORD request);

DWORD WINAPI ServiceRun();

void WINAPI ServiceMain(DWORD argc, LPTSTR *argv);

#define LogWriteLine(message, logType, ...) \
    WriteToLogFile((message), (logType))

#define LogWriteLineEx(format, size, logType, ...) \
    { \
        LPTSTR message = (LPTSTR) malloc((size) * sizeof(TCHAR)); \
        if (message != NULL) { \
            _stprintf_s((message), (size), (format), __VA_ARGS__); \
            WriteToLogFile((message), (logType)); \
            free(message); \
        } \
    } \
    (void) 0

#ifdef _DEBUG
#define LogWriteLineDbg(message) \
    WriteToLogFile((message), LOG_DEBUG)

#define LogWriteLineDbgEx(format, size, ...) \
    { \
        LPTSTR message = (LPTSTR) malloc(size * sizeof(TCHAR)); \
        if (message != NULL) { \
            _stprintf_s((message), (size), (format), __VA_ARGS__); \
            WriteToLogFile((message), LOG_DEBUG); \
            free(message); \
        } \
    } \
    (void) 0
#else
#define LogWriteLineDbg(content) (void) 0
#define LogWriteLineEbgEx(format, size, ...) (void) 0
#endif // _DEBUG

EXTERN_C_END

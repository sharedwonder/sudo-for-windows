// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under the MIT License.

#include "SudoFramework.h"

EXTERN_C_START

/*
 * The message types of log.
 */
enum LOG_MESSAGE_TYPE {
    LOG_MESSAGE_INFO,
    LOG_MESSAGE_WARNING,
    LOG_MESSAGE_ERROR,
    LOG_MESSAGE_DEBUG
};

/*!
 * Queries the token information.
 *
 * @param token The token to query.
 * @param tokenInfoClass The information class to query.
 * @param tokenInfo The answer.
 */
_Success_(return) BOOL QueryTokenInformation(_In_ HANDLE token, _In_ TOKEN_INFORMATION_CLASS tokenInfoClass, _Out_opt_ LPVOID tokenInfo);

/*!
 * Detects whether the token is elevated.
 *
 * @param token The token to detect.
 * @return BOOL The answer.
 */
BOOL TokenIsElevated(_In_ HANDLE token);

DWORD LaunchElevatedProcess(DWORD clientProcessId, LPTSTR username, LPTSTR commandLine,
                            ULONG_PTR consoleReference, DWORD environmentSize, LPTCH environment,
                            LPTSTR workingDirectory, LPTSTR launcher, PULONG_PTR newProcess);

/*!
 * Writes a message to the log file.
 *
 * @param message The message to write to the log file.
 * @param messageType The message type.
 */
void ServiceLog(LPTSTR message, enum LOG_MESSAGE_TYPE messageType);

/*!
 * Handles service control requests.
 *
 * @param request The service control request.
 */
void WINAPI __callback ServiceControlHandler(DWORD request);

/*!
 * Runs the service.
 *
 * @return DWORD The exit code.
 */
DWORD WINAPI ServiceRun();

/*!
 * Sets up the service and runs the service.
 *
 * @param argc
 * @param argv
 */
void WINAPI ServiceMain(DWORD argc, LPTSTR *argv);

EXTERN_C_END

// Logging Macros

#define WriteLog(message, messageType) \
    ServiceLog((message), (messageType))

#define WriteLogEx(format, size, messageType, ...) \
    { \
        LPTSTR _MESSAGE = (LPTSTR) malloc((size) * sizeof(TCHAR)); \
        if (_MESSAGE != NULL) { \
            _stprintf_s(_MESSAGE, (size), (format), __VA_ARGS__); \
            ServiceLog(_MESSAGE, (messageType)); \
            free(_MESSAGE); \
        } \
    } \
    (void) 0

#ifdef _DEBUG
    #define WriteLogDbg(message) \
        ServiceLog((message), LOG_MESSAGE_DEBUG)

    #define WriteLogDbgEx(format, size, ...) \
        { \
            LPTSTR __MESSAGE = (LPTSTR) malloc(size * sizeof(TCHAR)); \
            if (__MESSAGE != NULL) { \
                _stprintf_s(__MESSAGE, (size), (format), __VA_ARGS__); \
                ServiceLog(__MESSAGE, LOG_MESSAGE_DEBUG); \
                free(__MESSAGE); \
            } \
        } \
        (void) 0
#else
    #define WriteLogDbg(content) (void) 0
    #define WriteLogDbgEx(format, size, ...) (void) 0
#endif

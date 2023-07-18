// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under MIT License.

#include "Service.h"
#include "SudoConfiguration.h"
#include "SudoRpcServer.h"

#include <direct.h>
#include <time.h>

static SERVICE_STATUS ServiceStatus;
static SERVICE_STATUS_HANDLE ServiceStatusHandle;
static HANDLE LogFileHandle;
static TCHAR ProgramDirectory[MAX_PATH + 1] = {TEXT('\0')};

_Success_(return) BOOL QueryTokenInformation(_In_ HANDLE token, _In_ TOKEN_INFORMATION_CLASS tokenInfoClass, _Out_opt_ LPVOID tokenInfo) {
    DWORD size;
    GetTokenInformation(token, tokenInfoClass, NULL, 0, &size); // Get the size of information.
    return GetTokenInformation(token, tokenInfoClass, tokenInfo, size, &size);
}

BOOL TokenIsElevated(_In_ HANDLE token) {
    TOKEN_ELEVATION tokenElevation;

    if (!QueryTokenInformation(token, TokenElevation, &tokenElevation)) {
        return FALSE;
    }

    return tokenElevation.TokenIsElevated;
}

DWORD LaunchElevatedProcess(DWORD clientProcessId, LPTSTR username, LPTSTR commandLine,
                            ULONG_PTR consoleReference, DWORD environmentSize, LPTCH environment,
                            LPTSTR workingDirectory, LPTSTR launcher, PULONG_PTR newProcess) {
    // Unused parameters.
    UNREFERENCED_PARAMETER(environmentSize);

    WriteLogEx(TEXT("New elevating request:\r\n"
                    "Client process ID: %u\r\n"
                    "User: %s"),
               PID_MAX_LENGTH + USERNAME_MAX_LENGTH + 50,
               LOG_MESSAGE_INFO, clientProcessId, username);

    HANDLE clientProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, clientProcessId);
    if (clientProcess == INVALID_HANDLE_VALUE) {
        goto InitFailed;
    }

    // Using to set the new process attributes.
    STARTUPINFOEX startupInfoEx = {
        .StartupInfo.cb = sizeof(STARTUPINFOEX)
    };

    // Setting the new process attributes.
    {
        size_t returnLength = 0;

        InitializeProcThreadAttributeList(NULL, 2, 0, &returnLength);
        startupInfoEx.lpAttributeList = (LPPROC_THREAD_ATTRIBUTE_LIST) malloc(returnLength);
        if (startupInfoEx.lpAttributeList == NULL) {
            SetLastError(ERROR_NOT_ENOUGH_MEMORY);
            goto Failed;
        }

        // Initialize the attribute list.
        if (!InitializeProcThreadAttributeList(startupInfoEx.lpAttributeList, 2, 0, &returnLength)) {
            goto Failed;
        }

        if (!UpdateProcThreadAttribute(startupInfoEx.lpAttributeList, 0, PROC_THREAD_ATTRIBUTE_PARENT_PROCESS,
                                       &clientProcess, sizeof(HANDLE), NULL, NULL)) {
            goto Failed;
        }

        if (!UpdateProcThreadAttribute(startupInfoEx.lpAttributeList, 0, PROC_THREAD_ATTRIBUTE_CONSOLE_REFERENCE,
                                       &((HANDLE) consoleReference), sizeof(HANDLE), NULL, NULL)) {
            goto Failed;
        }
    }

    HANDLE clientToken;
    HANDLE elevatedToken;
    if (!OpenProcessToken(clientProcess, TOKEN_ALL_ACCESS, &clientToken)) {
        goto Failed;
    }

    if (!TokenIsElevated(clientToken)) {
        // Launch the helper program to authentication the request.

        TCHAR helperCommandLine[MAX_PATH + 2 + USERNAME_MAX_LENGTH + 2 + 4 + 1];
        _stprintf_s(helperCommandLine, sizeof(helperCommandLine) / sizeof(TCHAR),
                    TEXT("\"%sSudoHelper.exe\" \"%s\" %s"), ProgramDirectory, username, launcher);

        HANDLE newToken;
        {
            HANDLE token;
            if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &token)) {
                goto Failed;
            }

            if (!DuplicateTokenEx(token, TOKEN_ALL_ACCESS, NULL, SecurityDelegation, TokenPrimary, &newToken)) {
                CloseHandle(token);
                goto Failed;
            }

            CloseHandle(token);

            DWORD sessionId;
            if (!ProcessIdToSessionId(clientProcessId, &sessionId)) {
                CloseHandle(newToken);
                goto Failed;
            }

            if (!SetTokenInformation(newToken, TokenSessionId, &sessionId, sizeof(DWORD))) {
                CloseHandle(newToken);
                goto Failed;
            }
        }

        PROCESS_INFORMATION helperProcessInfo;
        if (!CreateProcessAsUser(newToken, NULL, helperCommandLine, NULL, NULL, TRUE,
                                 CREATE_UNICODE_ENVIRONMENT | EXTENDED_STARTUPINFO_PRESENT, NULL,
                                 ProgramDirectory, &startupInfoEx.StartupInfo, &helperProcessInfo)) {
            CloseHandle(newToken);
            goto Failed;
        }

        CloseHandle(newToken);

        WaitForSingleObject(helperProcessInfo.hProcess, INFINITE);

        DWORD errorCode;

        if (!GetExitCodeProcess(helperProcessInfo.hProcess, &errorCode)) {
            errorCode = GetLastError();
        }

        CloseHandle(helperProcessInfo.hProcess);
        CloseHandle(helperProcessInfo.hThread);
        if (errorCode != EXIT_SUCCESS) {
            SetLastError(errorCode);
            goto Failed;
        }

        // Get the elevated token of the user.

        TOKEN_LINKED_TOKEN linkedToken;
        if (!QueryTokenInformation(clientToken, TokenLinkedToken, &linkedToken)) {
            goto Failed;
        }
        if (!DuplicateTokenEx(linkedToken.LinkedToken, TOKEN_ALL_ACCESS, NULL, SecurityDelegation, TokenPrimary, &elevatedToken)) {
            goto Failed;
        }
    } else {
        elevatedToken = clientToken;
    }

    PROCESS_INFORMATION processInfo;
    if (!CreateProcessAsUser(elevatedToken, NULL, commandLine, NULL, NULL, TRUE,
                             CREATE_UNICODE_ENVIRONMENT | EXTENDED_STARTUPINFO_PRESENT,
                             environment, workingDirectory, &startupInfoEx.StartupInfo, &processInfo)) {
        goto Failed;
    }

    if (!DuplicateHandle(GetCurrentProcess(), processInfo.hProcess, clientProcess,
                         (PHANDLE) newProcess, 0, FALSE, DUPLICATE_SAME_ACCESS)) {
        CloseHandle(processInfo.hProcess);
        CloseHandle(processInfo.hThread);
        goto Failed;
    }

    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);
    CloseHandle(clientProcess);
    free(startupInfoEx.lpAttributeList);

    WriteLogEx(TEXT("Elevated successfully:\r\n"
                    "Client process ID: %u\r\n"
                    "User: %s"),
               PID_MAX_LENGTH + USERNAME_MAX_LENGTH + 50,
               LOG_MESSAGE_INFO, clientProcessId, username);
    return ERROR_SUCCESS;

Failed:
    CloseHandle(clientProcess);
    free(startupInfoEx.lpAttributeList);

InitFailed:
    LPTSTR message;
    FormatMessage(FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
                  NULL, GetLastError(), LANG_USER_DEFAULT, (LPTSTR) &message, 0, NULL);
    size_t messageSize = _tcslen(message);
    message[messageSize -= 2] = '\0';

    WriteLogEx(TEXT("Failed to elevate:\r\n"
                    "Client process ID: %u\r\n"
                    "User: %s\r\n"
                    "Error code: %u\r\n"
                    "Error message: %s"),
               PID_MAX_LENGTH + USERNAME_MAX_LENGTH + ERROR_CODE_MAX_LENGTH + 76 + messageSize,
               LOG_MESSAGE_INFO, clientProcessId, username, GetLastError(), message);

    LocalFree(message);

    return GetLastError();
}

DWORD WINAPI SudoRpcServerListen() {
    RPC_STATUS status = RpcServerUseProtseqEp((RPC_TSTR) TEXT("ncacn_np"), RPC_C_LISTEN_MAX_CALLS_DEFAULT,
                                              (RPC_TSTR) TEXT("\\pipe\\SudoForWindows"), NULL);
    if (status != RPC_S_OK) {
        return status;
    }

    status = RpcServerRegisterIf(SudoForWindows_v0_0_s_ifspec, NULL, NULL);
    if (status != RPC_S_OK) {
        return status;
    }

    return RpcServerListen(1, RPC_C_LISTEN_MAX_CALLS_DEFAULT, FALSE);
}

_Success_(return == ERROR_SUCCESS) RPC_STATUS WINAPI SudoRpcServerShutdown() {
    RPC_STATUS status;

    status = RpcMgmtStopServerListening(NULL);
    if (status != ERROR_SUCCESS) {
        return status;
    }

    return RpcServerUnregisterIf(NULL, NULL, FALSE);
}

void ServiceLog(LPTSTR message, enum LOG_MESSAGE_TYPE messageType) {
    TCHAR formattedTime[128];
    DWORD formattedTimeSize;

    // Get the formatted time.
    {
        time_t nowTime;
        struct tm timeStruct;

        time(&nowTime);
        localtime_s(&timeStruct, &nowTime);
        formattedTimeSize = (DWORD) _tcsftime(formattedTime, 128, TEXT("[%Y-%m-%d %H:%M:%S] "), &timeStruct);
    }

    WriteFile(LogFileHandle, formattedTime, formattedTimeSize * sizeof(TCHAR), NULL, NULL);

    switch (messageType) {
    case LOG_MESSAGE_INFO:
        WriteFile(LogFileHandle, TEXT("\tINFO\t"), 6 * sizeof(TCHAR), NULL, NULL);
        break;

    case LOG_MESSAGE_WARNING:
        WriteFile(LogFileHandle, TEXT("\tWARNING\t"), 9 * sizeof(TCHAR), NULL, NULL);
        break;

    case LOG_MESSAGE_ERROR:
        WriteFile(LogFileHandle, TEXT("\tERROR\t"), 7 * sizeof(TCHAR), NULL, NULL);
        break;

    case LOG_MESSAGE_DEBUG:
        WriteFile(LogFileHandle, TEXT("\tDEBUG\t"), 7 * sizeof(TCHAR), NULL, NULL);
        break;
    }

    WriteFile(LogFileHandle, message, ((DWORD) _tcslen(message)) * sizeof(TCHAR), NULL, NULL);
    WriteFile(LogFileHandle, TEXT("\r\n"), 2 * sizeof(TCHAR), NULL, NULL);
}

void WINAPI __callback ServiceControlHandler(DWORD control) {
    switch (control) {
    case SERVICE_CONTROL_STOP: case SERVICE_CONTROL_SHUTDOWN:
        ServiceStatus.dwWin32ExitCode = SudoRpcServerShutdown();

        WriteLogEx(TEXT("Sudo for Windows Service stopped. Exit code: %u"),
                   EXIT_CODE_MAX_LENGTH + 46, LOG_MESSAGE_INFO, ServiceStatus.dwWin32ExitCode);
        CloseHandle(LogFileHandle);

        ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        SetServiceStatus(ServiceStatusHandle, &ServiceStatus);
    }
}

DWORD WINAPI ServiceRun() {
    return SudoRpcServerListen();
}

void WINAPI ServiceMain(DWORD argc, LPTSTR *argv) {
    // Unused parameters.
    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    ServiceStatus.dwServiceType = SERVICE_WIN32;
    ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
    ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    ServiceStatus.dwWin32ExitCode = 0;
    ServiceStatus.dwServiceSpecificExitCode = 0;
    ServiceStatus.dwCheckPoint = 0;
    ServiceStatus.dwWaitHint = 0;
    ServiceStatusHandle = RegisterServiceCtrlHandler(TEXT("SudoService"), ServiceControlHandler);

    // Open the log file.
    {
        TCHAR logFile[MAX_PATH + 1] = {TEXT('\0')};
        TCHAR tempDirectory[MAX_PATH + 1] = {TEXT('\0')};

        GetEnvironmentVariable(TEXT("TEMP"), tempDirectory, MAX_PATH); // Get the path of the temporary directory.

        if (_taccess(tempDirectory, 0) == -1) {
            if (!CreateDirectory(tempDirectory, NULL)) {
                return;
            }
        }

        _tcscat_s(logFile, MAX_PATH, tempDirectory);
        _tcscat_s(logFile, MAX_PATH, TEXT("\\SudoService.log"));

        if (_waccess(logFile, 0) == -1) {
            // The log file is not existing.
            LogFileHandle = CreateFile(logFile, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ,
                                       NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
            // Check whether the handle is valid.
            if (LogFileHandle == INVALID_HANDLE_VALUE) {
                return;
            }

            // Set the file encoding to UTF-16.
        #ifdef UNICODE
            // Set the file encoding to Unicode (UTF-16).
            WORD header = 0xfeff;
            WriteFile(LogFileHandle, &header, sizeof(WORD), NULL, NULL);
        #endif // UNICODE
        } else {
            // The log file is already existing.
            LogFileHandle = CreateFile(logFile, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ,
                                       NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
            // Check whether the handle is valid.
            if (LogFileHandle == INVALID_HANDLE_VALUE) {
                return;
            }

        #ifdef UNICODE
            WORD header = 0;
            if (!ReadFile(LogFileHandle, &header, sizeof(WORD), NULL, NULL)) {
                return;
            }
            // Check whether the file encoding is UTF-16.
            if (header != 0xfeff) {
                // If not, set it to UTF-16.
                header = 0xfeff;
                WriteFile(LogFileHandle, &header, sizeof(WORD), NULL, NULL);
            }
        #endif // UNICODE

            // Move the pointer to the end of the log file.
            SetFilePointer(LogFileHandle, 0, NULL, FILE_END);
        }
    }

    if (ServiceStatusHandle == NULL) {
        WriteLogEx(TEXT("Failed to start Sudo for Windows Service. Error code: %u."),
                   ERROR_CODE_MAX_LENGTH + 56, LOG_MESSAGE_ERROR, GetLastError());
        return;
    }

    ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(ServiceStatusHandle, &ServiceStatus);

    WriteLog(TEXT("Sudo for Windows Service started."), LOG_MESSAGE_INFO);

    GetModuleFileName(NULL, ProgramDirectory, MAX_PATH);
    _tcsrchr(ProgramDirectory, TEXT('\\'))[1] = '\0';

    ServiceStatus.dwWin32ExitCode = ServiceRun();

    WriteLogEx(TEXT("Sudo for Windows Service stopped. Exit code: %d"),
               EXIT_CODE_MAX_LENGTH + 46, LOG_MESSAGE_INFO, ServiceStatus.dwWin32ExitCode);
    CloseHandle(LogFileHandle);

    ServiceStatus.dwCurrentState = SERVICE_STOPPED;
    SetServiceStatus(ServiceStatusHandle, &ServiceStatus);
}

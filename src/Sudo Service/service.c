// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under MIT License.

#include "service.h"

#include <direct.h>
#include <time.h>

static SERVICE_STATUS ServiceStatus;
static SERVICE_STATUS_HANDLE ServiceStatusHandle;
static HANDLE LogFileHandle;
static TCHAR ProgramDirectory[MAX_PATH + 1] = {TEXT('\0')};

_Must_inspect_result_ _Ret_maybenull_ _Post_writable_byte_size_(size)
void __RPC_FAR *__RPC_USER midl_user_allocate(_In_ size_t size) {
    return malloc(size);
}

void __RPC_USER midl_user_free(_Pre_maybenull_ _Post_invalid_ void __RPC_FAR *pointer) {
    free(pointer);
}

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

DWORD LaunchElevatedProcess(DWORD clientProcessId, LPTSTR userName, LPTSTR commandLine,
                            ULONG_PTR consoleReference, DWORD environmentSize, LPTCH environment,
                            LPTSTR workingDirectory, LPTSTR launcher, PULONG_PTR newProcess) {
    // Unused parameters.
    UNREFERENCED_PARAMETER(environmentSize);

    LogWriteLineEx(TEXT("New elevating permissions request, client process ID: %u, user: %s"), 93,
                   LOG_INFO, clientProcessId, userName);

    HANDLE clientProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, clientProcessId);
    if (clientProcess == INVALID_HANDLE_VALUE) {
        LogWriteLineEx(TEXT("Failed to elevated permission, client process ID: %u, user: %s"), 89,
                       LOG_INFO, clientProcessId, userName);
        return GetLastError();
    }

    // Using to set the new process attributes.
    STARTUPINFOEX startupInfoEx;

    ZeroMemory(&startupInfoEx, sizeof(STARTUPINFOEX));
    startupInfoEx.StartupInfo.cb = sizeof(STARTUPINFOEX);

    // Setting the new process attributes.
    {
        size_t returnLength = 0;

        InitializeProcThreadAttributeList(NULL, 2, 0, &returnLength);
        startupInfoEx.lpAttributeList = (LPPROC_THREAD_ATTRIBUTE_LIST) malloc(returnLength);
        if (startupInfoEx.lpAttributeList == NULL) {
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

        TCHAR helperCommandLine[MAX_PATH + 27];
        _stprintf_s(helperCommandLine, MAX_PATH + 27, TEXT("\"%sSudoHelper.exe\" %s %s"), ProgramDirectory, userName, launcher);

        HANDLE newToken;
        {
            HANDLE token;
            OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &token);

            if (!DuplicateTokenEx(token, TOKEN_ALL_ACCESS, NULL, SecurityDelegation, TokenPrimary, &newToken)) {
                CloseHandle(token);
                goto Failed;
            }

            CloseHandle(token);

            DWORD sessionId;
            ProcessIdToSessionId(clientProcessId, &sessionId);

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
        goto Failed;
    }

    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);

    CloseHandle(clientProcess);

    LogWriteLineEx(TEXT("Elevated permission successfully, client process ID: %u, user: %s"), 92,
                   LOG_INFO, clientProcessId, userName);
    return ERROR_SUCCESS;

Failed:
    LogWriteLineEx(TEXT("Failed to elevated permission, client process ID: %u, user: %s"), 89,
                   LOG_INFO, clientProcessId, userName);

    CloseHandle(clientProcess);
    free(startupInfoEx.lpAttributeList);

    return GetLastError();
}

void WriteToLogFile(LPTSTR message, enum LOG_TYPE logType) {
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

    switch (logType) {
    case LOG_INFO:
        WriteFile(LogFileHandle, TEXT("INFO    "), 8 * sizeof(TCHAR), NULL, NULL);
        break;

    case LOG_WARNING:
        WriteFile(LogFileHandle, TEXT("WARNING "), 8 * sizeof(TCHAR), NULL, NULL);
        break;

    case LOG_ERROR:
        WriteFile(LogFileHandle, TEXT("ERROR   "), 8 * sizeof(TCHAR), NULL, NULL);
        break;

    case LOG_DEBUG:
        WriteFile(LogFileHandle, TEXT("DEBUG   "), 8 * sizeof(TCHAR), NULL, NULL);
        break;
    }

    WriteFile(LogFileHandle, message, ((DWORD) _tcslen(message)) * sizeof(TCHAR), NULL, NULL);
    WriteFile(LogFileHandle, TEXT("\r\n"), 2 * sizeof(TCHAR), NULL, NULL);
}

DWORD WINAPI ServiceRun() {
    RPC_STATUS status = RpcServerUseProtseqEp((_TUCHAR *) TEXT("ncacn_np"), RPC_C_LISTEN_MAX_CALLS_DEFAULT,
                                              (_TUCHAR *) TEXT("\\pipe\\SudoService"), NULL);
    if (status != RPC_S_OK) {
        return status;
    }

    status = RpcServerRegisterIf(SudoForWindows_v1_0_s_ifspec, NULL, NULL);
    if (status != RPC_S_OK) {
        return status;
    }

    status = RpcServerListen(1, RPC_C_LISTEN_MAX_CALLS_DEFAULT, FALSE);
    return status;
}

void WINAPI __callback ControlHandler(DWORD control) {
    switch (control) {
    case SERVICE_CONTROL_STOP: case SERVICE_CONTROL_SHUTDOWN:
        CloseHandle(LogFileHandle);
        LogWriteLine(TEXT("Sudo for Windows Service stopped, Exit code: 0"), LOG_INFO);
        ServiceStatus.dwWin32ExitCode = 0;
        ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        SetServiceStatus(ServiceStatusHandle, &ServiceStatus);
    }
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
    ServiceStatusHandle = RegisterServiceCtrlHandler(TEXT("SudoService"), ControlHandler);

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

            // Set the file encoding to Unicode (UTF-16).
            WORD header = 0xfeff;
            WriteFile(LogFileHandle, &header, sizeof(WORD), NULL, NULL);
        } else {
            // The log file is already existing.
            LogFileHandle = CreateFile(logFile, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ,
                                       NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
            // Check whether the handle is valid.
            if (LogFileHandle == INVALID_HANDLE_VALUE) {
                return;
            }

            WORD header = 0;
            if (!ReadFile(LogFileHandle, &header, sizeof(WORD), NULL, NULL)) {
                return;
            }
            // Check whether the file encoding is Unicode (UTF-16).
            if (header != 0xfeff) {
                // If not, set it to Unicode (UTF-16).
                header = 0xfeff;
                WriteFile(LogFileHandle, &header, sizeof(WORD), NULL, NULL);
            }
            // Move the pointer to the end of the log file.
            SetFilePointer(LogFileHandle, 0, NULL, FILE_END);
        }
    }

    if (ServiceStatusHandle == NULL) {
        LogWriteLineEx(TEXT("Failed to start Sudo for Windows Service. Error code: %u."), 60, LOG_ERROR, GetLastError());
        return;
    }

    ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(ServiceStatusHandle, &ServiceStatus);

    LogWriteLine(TEXT("Sudo for Windows Service started successfully."), LOG_INFO);

    GetModuleFileName(NULL, ProgramDirectory, MAX_PATH);
    _tcsrchr(ProgramDirectory, TEXT('\\'))[1] = '\0';

    ServiceStatus.dwWin32ExitCode = ServiceRun();
    LogWriteLineEx(TEXT("Sudo for Windows Service stopped. Exit code: %d"), 56, LOG_INFO, ServiceStatus.dwWin32ExitCode);
    ServiceStatus.dwCurrentState = SERVICE_STOPPED;
    SetServiceStatus(ServiceStatusHandle, &ServiceStatus);
}

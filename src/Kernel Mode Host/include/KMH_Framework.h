// Copyright (c) 2022 Liu Baihao. All rights reserved.
// This software is licensed under MIT License.

#pragma once

#include "KernelModeHost.h"

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
#ifdef MIDL_PASS
    [size_is(MaximumLength / 2), length_is((Length) / 2)] USHORT *Buffer;
#else // MIDL_PASS
    _Field_size_bytes_part_opt_(MaximumLength, Length) PWCH Buffer;
#endif // MIDL_PASS
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct _OBJECT_ATTRIBUTES {
    ULONG Length;
    HANDLE RootDirectory;
    PUNICODE_STRING ObjectName;
    ULONG Attributes;
    PVOID SecurityDescriptor;
    PVOID SecurityQualityOfService;
} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;

#define OBJ_INHERIT                         0x00000002L
#define OBJ_PERMANENT                       0x00000010L
#define OBJ_EXCLUSIVE                       0x00000020L
#define OBJ_CASE_INSENSITIVE                0x00000040L
#define OBJ_OPENIF                          0x00000080L
#define OBJ_OPENLINK                        0x00000100L
#define OBJ_KERNEL_HANDLE                   0x00000200L
#define OBJ_FORCE_ACCESS_CHECK              0x00000400L
#define OBJ_IGNORE_IMPERSONATED_DEVICEMAP   0x00000800L
#define OBJ_DONT_REPARSE                    0x00001000L
#define OBJ_VALID_ATTRIBUTES                0x00001FF2L

#define InitializeObjectAttributes(objectAttributes, objectName, attributes, rootDirectory, securityDescriptor) { \
    (objectAttributes)->Length = sizeof(OBJECT_ATTRIBUTES); \
    (objectAttributes)->RootDirectory = rootDirectory; \
    (objectAttributes)->Attributes = attributes; \
    (objectAttributes)->ObjectName = objectName; \
    (objectAttributes)->SecurityDescriptor = securityDescriptor; \
    (objectAttributes)->SecurityQualityOfService = NULL; \
}

typedef struct _CLIENT_ID {
    HANDLE UniqueProcess;
    HANDLE UniqueThread;
} CLIENT_ID, *PCLIENT_ID;

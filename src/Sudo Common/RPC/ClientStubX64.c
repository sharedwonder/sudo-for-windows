

/* this ALWAYS GENERATED file contains the RPC client stubs */


 /* File created by MIDL compiler version 8.01.0626 */
/* at Tue Jan 19 11:14:07 2038
 */
/* Compiler settings for ..\SudoService.idl, ..\SudoService.acf:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0626 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#if defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/

#include <string.h>

#include "SudoRpcX64.h"

#define TYPE_FORMAT_STRING_SIZE   33                                
#define PROC_FORMAT_STRING_SIZE   87                                
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _SudoService_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } SudoService_MIDL_TYPE_FORMAT_STRING;

typedef struct _SudoService_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } SudoService_MIDL_PROC_FORMAT_STRING;

typedef struct _SudoService_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } SudoService_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};

#if defined(_CONTROL_FLOW_GUARD_XFG)
#define XFG_TRAMPOLINES(ObjectType)\
static unsigned long ObjectType ## _UserSize_XFG(unsigned long * pFlags, unsigned long Offset, void * pObject)\
{\
return  ObjectType ## _UserSize(pFlags, Offset, pObject);\
}\
static unsigned char * ObjectType ## _UserMarshal_XFG(unsigned long * pFlags, unsigned char * pBuffer, void * pObject)\
{\
return ObjectType ## _UserMarshal(pFlags, pBuffer, pObject);\
}\
static unsigned char * ObjectType ## _UserUnmarshal_XFG(unsigned long * pFlags, unsigned char * pBuffer, void * pObject)\
{\
return ObjectType ## _UserUnmarshal(pFlags, pBuffer, pObject);\
}\
static void ObjectType ## _UserFree_XFG(unsigned long * pFlags, void * pObject)\
{\
ObjectType ## _UserFree(pFlags, pObject);\
}
#define XFG_TRAMPOLINES64(ObjectType)\
static unsigned long ObjectType ## _UserSize64_XFG(unsigned long * pFlags, unsigned long Offset, void * pObject)\
{\
return  ObjectType ## _UserSize64(pFlags, Offset, pObject);\
}\
static unsigned char * ObjectType ## _UserMarshal64_XFG(unsigned long * pFlags, unsigned char * pBuffer, void * pObject)\
{\
return ObjectType ## _UserMarshal64(pFlags, pBuffer, pObject);\
}\
static unsigned char * ObjectType ## _UserUnmarshal64_XFG(unsigned long * pFlags, unsigned char * pBuffer, void * pObject)\
{\
return ObjectType ## _UserUnmarshal64(pFlags, pBuffer, pObject);\
}\
static void ObjectType ## _UserFree64_XFG(unsigned long * pFlags, void * pObject)\
{\
ObjectType ## _UserFree64(pFlags, pObject);\
}
#define XFG_BIND_TRAMPOLINES(HandleType, ObjectType)\
static void* ObjectType ## _bind_XFG(HandleType pObject)\
{\
return ObjectType ## _bind((ObjectType) pObject);\
}\
static void ObjectType ## _unbind_XFG(HandleType pObject, handle_t ServerHandle)\
{\
ObjectType ## _unbind((ObjectType) pObject, ServerHandle);\
}
#define XFG_TRAMPOLINE_FPTR(Function) Function ## _XFG
#else
#define XFG_TRAMPOLINES(ObjectType)
#define XFG_TRAMPOLINES64(ObjectType)
#define XFG_BIND_TRAMPOLINES(HandleType, ObjectType)
#define XFG_TRAMPOLINE_FPTR(Function) Function
#endif


extern const SudoService_MIDL_TYPE_FORMAT_STRING SudoService__MIDL_TypeFormatString;
extern const SudoService_MIDL_PROC_FORMAT_STRING SudoService__MIDL_ProcFormatString;
extern const SudoService_MIDL_EXPR_FORMAT_STRING SudoService__MIDL_ExprFormatString;

#define GENERIC_BINDING_TABLE_SIZE   0            


/* Standard interface: SudoForWindows, ver. 1.0,
   GUID={0x479faa79,0x6f61,0x1499,{0x50,0x6f,0xb4,0xa5,0x4c,0x73,0xf9,0x6f}} */

handle_t RpcBindingHandle;


static const RPC_CLIENT_INTERFACE SudoForWindows___RpcClientInterface =
    {
    sizeof(RPC_CLIENT_INTERFACE),
    {{0x479faa79,0x6f61,0x1499,{0x50,0x6f,0xb4,0xa5,0x4c,0x73,0xf9,0x6f}},{1,0}},
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    0,
    0,
    0,
    0,
    0,
    0x00000000
    };
RPC_IF_HANDLE SudoForWindows_v1_0_c_ifspec = (RPC_IF_HANDLE)& SudoForWindows___RpcClientInterface;

extern const MIDL_STUB_DESC SudoForWindows_StubDesc;

static RPC_BINDING_HANDLE SudoForWindows__MIDL_AutoBindHandle;


DWORD LaunchElevatedProcess( 
    /* [in] */ DWORD clientProcessId,
    /* [string][in] */ LPTSTR userName,
    /* [string][in] */ LPTSTR commandLine,
    /* [in] */ ULONG_PTR consoleReference,
    /* [in] */ DWORD environmentSize,
    /* [length_is][size_is][in] */ TCHAR *environment,
    /* [string][in] */ LPTSTR workingDirectory,
    /* [string][in] */ LPTSTR launcher,
    /* [out] */ PULONG_PTR newProcess)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&SudoForWindows_StubDesc,
                  (PFORMAT_STRING) &SudoService__MIDL_ProcFormatString.Format[0],
                  clientProcessId,
                  userName,
                  commandLine,
                  consoleReference,
                  environmentSize,
                  environment,
                  workingDirectory,
                  launcher,
                  newProcess);
    return ( DWORD  )_RetVal.Simple;
    
}


#if !defined(__RPC_WIN64__)
#error  Invalid build platform for this stub.
#endif

static const SudoService_MIDL_PROC_FORMAT_STRING SudoService__MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure LaunchElevatedProcess */

			0x32,		/* FC_BIND_PRIMITIVE */
			0x48,		/* Old Flags:  */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x0 ),	/* 0 */
/*  8 */	NdrFcShort( 0x50 ),	/* X64 Stack size/offset = 80 */
/* 10 */	NdrFcShort( 0x18 ),	/* 24 */
/* 12 */	NdrFcShort( 0x24 ),	/* 36 */
/* 14 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0xa,		/* 10 */
/* 16 */	0xa,		/* 10 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 18 */	NdrFcShort( 0x0 ),	/* 0 */
/* 20 */	NdrFcShort( 0x1 ),	/* 1 */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */
/* 24 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter clientProcessId */

/* 26 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 28 */	NdrFcShort( 0x0 ),	/* X64 Stack size/offset = 0 */
/* 30 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter userName */

/* 32 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 34 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 36 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter commandLine */

/* 38 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 40 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 42 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter consoleReference */

/* 44 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 46 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 48 */	0xb9,		/* FC_UINT3264 */
			0x0,		/* 0 */

	/* Parameter environmentSize */

/* 50 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 52 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 54 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter environment */

/* 56 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 58 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 60 */	NdrFcShort( 0xa ),	/* Type Offset=10 */

	/* Parameter workingDirectory */

/* 62 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 64 */	NdrFcShort( 0x30 ),	/* X64 Stack size/offset = 48 */
/* 66 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter launcher */

/* 68 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 70 */	NdrFcShort( 0x38 ),	/* X64 Stack size/offset = 56 */
/* 72 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter newProcess */

/* 74 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 76 */	NdrFcShort( 0x40 ),	/* X64 Stack size/offset = 64 */
/* 78 */	0xb9,		/* FC_UINT3264 */
			0x0,		/* 0 */

	/* Return value */

/* 80 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 82 */	NdrFcShort( 0x48 ),	/* X64 Stack size/offset = 72 */
/* 84 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const SudoService_MIDL_TYPE_FORMAT_STRING SudoService__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/*  4 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/*  6 */	
			0x11, 0x0,	/* FC_RP */
/*  8 */	NdrFcShort( 0x2 ),	/* Offset= 2 (10) */
/* 10 */	
			0x1c,		/* FC_CVARRAY */
			0x1,		/* 1 */
/* 12 */	NdrFcShort( 0x2 ),	/* 2 */
/* 14 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x56,		/* FC_MULT_2 */
/* 16 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 18 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 20 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 22 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 24 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 26 */	0x5,		/* FC_WCHAR */
			0x5b,		/* FC_END */
/* 28 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 30 */	0xb9,		/* FC_UINT3264 */
			0x5c,		/* FC_PAD */

			0x0
        }
    };

static const unsigned short SudoForWindows_FormatStringOffsetTable[] =
    {
    0
    };


static const MIDL_STUB_DESC SudoForWindows_StubDesc = 
    {
    (void *)& SudoForWindows___RpcClientInterface,
    MIDL_user_allocate,
    MIDL_user_free,
    &RpcBindingHandle,
    0,
    0,
    0,
    0,
    SudoService__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x8010272, /* MIDL Version 8.1.626 */
    0,
    0,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* defined(_M_AMD64)*/


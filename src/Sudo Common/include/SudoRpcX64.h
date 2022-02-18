

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0626 */
/* at Tue Jan 19 11:14:07 2038
 */
/* Compiler settings for SudoService.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0626 
    protocol : all , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */


#ifndef __SudoRpcX64_h__
#define __SudoRpcX64_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef DECLSPEC_XFGVIRT
#if _CONTROL_FLOW_GUARD_XFG
#define DECLSPEC_XFGVIRT(base, func) __declspec(xfg_virtual(base, func))
#else
#define DECLSPEC_XFGVIRT(base, func)
#endif
#endif

/* Forward Declarations */ 

/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __SudoForWindows_INTERFACE_DEFINED__
#define __SudoForWindows_INTERFACE_DEFINED__

/* interface SudoForWindows */
/* [implicit_handle][version][uuid] */ 

DWORD LaunchElevatedProcess( 
    /* [in] */ DWORD clientProcessId,
    /* [string][in] */ LPTSTR userName,
    /* [string][in] */ LPTSTR commandLine,
    /* [in] */ ULONG_PTR consoleReference,
    /* [in] */ DWORD environmentSize,
    /* [length_is][size_is][in] */ TCHAR *environment,
    /* [string][in] */ LPTSTR workingDirectory,
    /* [string][in] */ LPTSTR launcher,
    /* [out] */ PULONG_PTR newProcess);


extern handle_t RpcBindingHandle;


extern RPC_IF_HANDLE SudoForWindows_v1_0_c_ifspec;
extern RPC_IF_HANDLE SudoForWindows_v1_0_s_ifspec;
#endif /* __SudoForWindows_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif



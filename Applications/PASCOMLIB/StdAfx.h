/*******************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name		: StdAfx.h
 *  Overview		:
 *  Last Modified	: [6/6/2008]
 *******************************************************************************
 *                       Revision History
 *******************************************************************************
 *  Revision	Author		Date & Time			Changes
 *==============================================================================
 *   1.0		Patni       [6/6/2008]			Added file header
 *
 ******************************************************************************/

#ifndef __SM_PASCOM_STDAFX_H__
#define __SM_PASCOM_STDAFX_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <ErrorDisp/ErrorDisplay.h>     // For MPlus Try catch method
#import "msxml4.dll" rename_namespace("MSXML4") named_guids
using namespace MSXML4;

//+Patni-MSN/2009Mar22/Added/PSP1#Warnings
#pragma warning(disable:4786)
#pragma warning(disable:4003)
//-Patni-MSN/2009Mar22/Added/PSP1#Warnings
// for libWNP integration
#include <comdef.h>
#include <iostream>
#include <winsock2.h>
#include <tami/libWNP/WinNativePorting.h>
// for libWNP integration

//+Patni-AA/2010Oct20/Modified/V2.00#IR-171
#include "PASTrace.h"                   // For PAS_TRACE()
#include "ErrorManager.h"               // For CErrorManager class 
//-Patni-AA/2010Oct20/Modified/V2.00#IR-171

#define _PASCOM_AUTO_LIB_


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #ifndef __SM_PASCOM_STDAFX_H__

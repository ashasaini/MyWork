/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: stdafx.h
 *  Overview: Source file that includes just the standard includes.
 *  Last Modified: 2007/8/10 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2007/6/15 12:00:00     PQM5 Baselined code
 *   2.0          Patni            2007/8/10 12:00:00     Implemented PQM6 requirements & Formatted
 *
 *****************************************************************************/
// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently

#ifndef __SM_IPCLIB_STDAFX_H__
#define __SM_IPCLIB_STDAFX_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers

#define _WIN32_WINNT 0x0400

//#pragma warning(disable: 4251)
#pragma warning(disable: 4003)
#pragma warning(disable: 4786)
#pragma warning(disable: 4005)

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>       // MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>         // MFC support for Windows Common Controls
#include <afxtempl.h>
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <comdef.h>
#include <winsock2.h>

#include <iostream>
#include <vector>
#include <map>
using std::vector;

#include <tami/libWNP/WinNativePorting.h>

#include <tami/common/basicdefs.h>
#include <tami/vfStudy/vf_basictypes.h>
#include <tami/vfStudy/vf_apptypes.h>
#include <tami/vfStudy/vf_appnames.h>

#include <PDataTypes/PTString.h>
#include <PQM/PQMCommon.h>
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // #ifndef __SM_IPCLIB_STDAFX_H__

// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__F2ACDBC5_C8F5_4DF9_B8BB_31F4ECF31009__INCLUDED_)
#define AFX_STDAFX_H__F2ACDBC5_C8F5_4DF9_B8BB_31F4ECF31009__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxole.h>         // MFC OLE classes
#include <afxdisp.h>        // MFC Automation classes
#include <afxdocob.h>
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#define WM_INIT_KTRECON         (WM_USER + 100)
#define WM_CMD_CLOSE            (WM_USER + 101)
#define WM_CMD_SHOW             (WM_USER + 102)
#define WM_CMD_HIDE             (WM_USER + 103)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__F2ACDBC5_C8F5_4DF9_B8BB_31F4ECF31009__INCLUDED_)

// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__156EC6F5_083E_4001_AC73_D124FEE97D51__INCLUDED_)
#define AFX_STDAFX_H__156EC6F5_083E_4001_AC73_D124FEE97D51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define message(ignore) //To suppress INFO messages //to see the messages comment out this line

#include <MPlus/begin_new_iostream.h>
//#include <MPlus/end_new_iostream.h>

#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxole.h>         // MFC OLE classes
#include <afxdisp.h>        // MFC Automation classes
#include <afxdocob.h>
#include <afxdtctl.h>       // MFC support for Internet Explorer 4 Common Controls
#include <afx.h>
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>         // MFC support for Windows Common Controls
//#include <afxsock.h>        // MFC socket extensions


#define STL_USING_MAP
#define STL_USING_VECTOR
#define STL_USING_STRING
#define STL_USING_STREAM
#define STL_USING_SET
#define STL_USING_LIST
#include "GLCommon/MPlusSTL.h"



#include "C:\MRMPlus\include\tami\common\errors.h"
#include <comdef.h>

#include <atlbase.h>

#include "CommonDefinitions.h"


class MRSExeModule : public CComModule
{
public:
    MRSExeModule();
    virtual ~MRSExeModule();
    virtual LONG Unlock();
    DWORD dwThreadID;
};
extern MRSExeModule _Module;
#include <atlcom.h>
#include <atlwin.h>
#include <comdef.h>
#endif // _AFX_NO_AFXCMN_SUPPORT

#define WM_INIT_TIPREP          (WM_USER + 100)
#define WM_CMD_CLOSE            (WM_USER + 101)
#define WM_CMD_SHOW             (WM_USER + 102)
#define WM_CMD_HIDE             (WM_USER + 103)
#define WM_CMD_DRAW_ROI         (WM_USER + 104)
#define WM_CMD_GET_SELIMG       (WM_USER +105)
#define WM_PORT_NO       (WM_USER +106)
#define WM_UPDATE_PROGRESS       (WM_USER +107)


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__156EC6F5_083E_4001_AC73_D124FEE97D51__INCLUDED_)

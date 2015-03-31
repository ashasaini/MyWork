/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: IPCCom.cpp
 *  Overview: Implementation of CIPCCom class.
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
#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "IPCCom.h"
#include "IPCCom_i.c"
#include "PqmIpcCom.h"

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_PqmIpcCom, CPqmIpcCom)
END_OBJECT_MAP()

// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL,
//      run nmake -f IPCComps.mk in the project directory.

BOOL DisplayError(CString f_error_message, CString f_file_name,
                  UINT f_line_number, int f_error_level,
                  DWORD f_message_id , CString f_event_source, bool f_detail_error_log);
class CIPCComApp : public CWinApp
{
public:

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CIPCComApp)
public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
    //}}AFX_VIRTUAL

    //{{AFX_MSG(CIPCComApp)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CIPCComApp, CWinApp)
    //{{AFX_MSG_MAP(CIPCComApp)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

CIPCComApp theApp;

//****************************Method Header************************************

//Method Name   :InitInstance()

//Author        :PATNI / HAR

//Purpose       :

//*****************************************************************************
BOOL CIPCComApp::InitInstance(
)
{
    _Module.Init(ObjectMap, m_hInstance, &LIBID_IPCCOMLib);
    return CWinApp::InitInstance();
}

//****************************Method Header************************************

//Method Name   :ExitInstance()

//Author        :PATNI / HAR

//Purpose       :

//*****************************************************************************
int CIPCComApp::ExitInstance(
)
{
    _Module.Term();
    return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE
//****************************Method Header************************************

//Method Name   :DllCanUnloadNow()

//Author        :PATNI / HAR

//Purpose       :

//*****************************************************************************
STDAPI DllCanUnloadNow(
    void
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return (AfxDllCanUnloadNow() == S_OK && _Module.GetLockCount() == 0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type
//****************************Method Header************************************

//Method Name   :DllGetClassObject()

//Author        :PATNI / HAR

//Purpose       :

//*****************************************************************************
STDAPI DllGetClassObject(
    REFCLSID rclsid,
    REFIID riid,
    LPVOID* ppv
)
{
    return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry
//****************************Method Header************************************

//Method Name   :DllRegisterServer()

//Author        :PATNI / HAR

//Purpose       :

//*****************************************************************************
STDAPI DllRegisterServer(
    void
)
{
    // registers object, typelib and all interfaces in typelib
    return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry
//****************************Method Header************************************

//Method Name   :DllUnregisterServer()

//Author        :PATNI / HAR

//Purpose       :

//*****************************************************************************
STDAPI DllUnregisterServer(
    void
)
{
    return _Module.UnregisterServer(TRUE);
}



// TiPrep.cpp : Defines the class behaviors for the application.
//
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#include "stdafx.h"
#include "TiPrep.h"
#include <afxsock.h>
#include "MainFrm.h"
#include "IpFrame.h"
#include "TiPrepDoc.h"
#include "TiPrepView.h"
#include "TiPrepLogHelper.h" //AS-AN

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTiPrepApp

BEGIN_MESSAGE_MAP(CTiPrepApp, CWinApp)
    //{{AFX_MSG_MAP(CTiPrepApp)

    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG_MAP
    // Standard file based document commands
    ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
    ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTiPrepApp construction

CTiPrepApp::CTiPrepApp():m_bMouseActive(false)
{
    // Place all significant initialization in InitInstance
    //Patni-SS/2009Mar14/Added/PSP1#ZOrder
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTiPrepApp object

CTiPrepApp theApp;

// This identifier was generated to be statistically unique for your app.
// You may change it if you prefer to choose a specific identifier.

// {5A4AECCA-C1EB-4AEC-9D64-0043265A27A4}
static const CLSID clsid =
{ 0x5a4aecca, 0xc1eb, 0x4aec, { 0x9d, 0x64, 0x0, 0x43, 0x26, 0x5a, 0x27, 0xa4 } };

/////////////////////////////////////////////////////////////////////////////
// CTiPrepApp initialization

BOOL CTiPrepApp::InitInstance()
{
    ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

    // Initialize OLE libraries
    if(!AfxOleInit()) {
//      AfxMessageBox(IDP_OLE_INIT_FAILED);
        return FALSE;
    }

    if (!AfxSocketInit()) //AS-AN
    {
        CTiPrepLogHelper::WriteToErrorLog
            (_T("Socket initialization failed.Please restart the applilcation "), __LINE__, __FILE__);
        //return FALSE; //do not return from here else application will exit
	} //AS-AN
    AfxEnableControlContainer();
    // Standard initialization
    // If you are not using these features and wish to reduce the size
    //  of your final executable, you should remove from the following
    //  the specific initialization routines you do not need.
#ifdef _AFXDLL
    Enable3dControls();         // Call this when using MFC in a shared DLL
#else
    Enable3dControlsStatic();   // Call this when linking to MFC statically
#endif
    // Change the registry key under which our settings are stored.
    // TODO: You should modify this string to be something appropriate
    // such as the name of your company or organization.
    SetRegistryKey(_T("Local AppWizard-Generated Applications"));
    LoadStdProfileSettings();  // Load standard INI file options (including MRU)
    // Register the application's document templates.  Document templates
    //  serve as the connection between documents, frame windows and views.
    CSingleDocTemplate* pDocTemplate;
    pDocTemplate = new CSingleDocTemplate(
        IDR_MAINFRAME,
        RUNTIME_CLASS(CTiPrepDoc),
        RUNTIME_CLASS(CMainFrame),       // main SDI frame window
        RUNTIME_CLASS(CTiPrepView));
    pDocTemplate->SetServerInfo(
        IDR_SRVR_EMBEDDED, IDR_SRVR_INPLACE,
        RUNTIME_CLASS(CInPlaceFrame));
    AddDocTemplate(pDocTemplate);
    // Connect the COleTemplateServer to the document template.
    //  The COleTemplateServer creates new documents on behalf
    //  of requesting OLE containers by using information
    //  specified in the document template.
    m_server.ConnectTemplate(clsid, pDocTemplate, TRUE);
    // Note: SDI applications register server objects only if /Embedding
    //   or /Automation is present on the command line.
    // Enable DDE Execute open
    EnableShellOpen();
    RegisterShellFileTypes(TRUE);
    // Parse command line for standard shell commands, DDE, file open
    CCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);

    // Check to see if launched as OLE server
    if(cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated) {
        // Register all OLE server (factories) as running.  This enables the
        //  OLE libraries to create objects from other applications.
        COleTemplateServer::RegisterAll();
        // Application was run with /Embedding or /Automation.  Don't show the
        //  main window in this case.
        return TRUE;
    }

    // When a server application is launched stand-alone, it is a good idea
    //  to update the system registry in case it has been damaged.
    m_server.UpdateRegistry(OAT_DOC_OBJECT_SERVER);

    // Dispatch commands specified on the command line
    if(!ProcessShellCommand(cmdInfo)) {
        return FALSE;
    }

    // The one and only window has been initialized, so show and update it.
    m_pMainWnd->ShowWindow(SW_HIDE);
    m_pMainWnd->UpdateWindow();
    // Enable drag/drop open
    m_pMainWnd->DragAcceptFiles();
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CTiPrepApp message handlers


/*************************************************************
* Method:    	ExitInstance
* Description: 	This function is used 
* Returns:   	int
*************************************************************/
int CTiPrepApp::ExitInstance()
{
    CoUninitialize();
    return CWinApp::ExitInstance();
}

/*************************************************************
* Method:    	PreTranslateMessage
* Description: 	This function is used 
* Parameter: 	MSG * pMsg :
* Returns:   	BOOL
*************************************************************/
BOOL CTiPrepApp::PreTranslateMessage(MSG* pMsg)
{
    if(!m_pMainWnd) {
        return FALSE;
    }

    return CWinApp::PreTranslateMessage(pMsg);
}

///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////

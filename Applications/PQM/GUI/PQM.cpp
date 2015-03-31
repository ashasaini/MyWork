//REDMINE_782_CPP_Test_Tool
//TMSC-REDMINE-782
//TMSC-REDMINE-781
//TMSC-REDMINE-780
//Redmine-775
//TMSC-REDMINE-777
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQM.cpp
 *  Overview: Implementation of CPQM class.
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
#include "PQM.h"
#include <PDataParser/INIReader.h>
#include <libStudyManager/NonCopyable.h>
#include "MainFrm.h"
#include "IpFrame.h"
#include "PQMDoc.h"
#include "PQMView.h"
//TMSC-REDMINE-774
#include "PqmGuiConstants.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//+Patni-PP/2009Jun01/Modified/SU14 CPP test initialization corrections
//HINSTANCE hInstance;
HINSTANCE hInstance = NULL;
//-Patni-PP/2009Jun01/Modified/SU14 CPP test initialization corrections

BEGIN_MESSAGE_MAP(CPQMApp, CWinApp)
    //{{AFX_MSG_MAP(CPQMApp)
    ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// The one and only CPQMApp object

CPQMApp theApp;

// This identifier was generated to be statistically unique for your app.
// You may change it if you prefer to choose a specific identifier.

// {D480948E-DEC8-4C36-975F-D12A84814CD1}
static const CLSID clsidpqm = { 0xd480948e, 0xdec8, 0x4c36, { 0x97, 0x5f, 0xd1, 0x2a, 0x84, 0x81, 0x4c, 0xd1 } };


// CPQMApp construction


CPQMApp::CPQMApp(
):  m_localize(NULL),
    m_bMouseActive(false),
    m_run_automated(0),
    m_run_embadded(0),
    m_mode(-1)
{

    //+Patni-PP/2009Jun01/Modified/SU14 CPP test initialization corrections
    //-Patni-PP/2009Jun01/Modified/SU14 CPP test initialization corrections

    // TODO: add construction code here,
    // Place all significant initialization in InitInstance

}

//+Patni-AMT/2009May15/Added/JFT# Unregister
//************************************Method Header*****************************
//Method Name : ProcessShellCommand
//Author      : PATNI/AMT
//Purpose     : Dont show message while unregestering application thro
//              /unregserver
//              This function is called internally by OS.
//*****************************************************************************/
BOOL CPQMApp::ProcessShellCommand(
    CCommandLineInfo& f_cmd_info
)
{
    BOOL l_result = TRUE;

    switch (f_cmd_info.m_nShellCommand) {
        case CCommandLineInfo::AppUnregister: {
            BOOL l_unregistered = Unregister();
            l_result = FALSE;

            if (m_pCmdInfo == NULL) {
                m_pCmdInfo = new CCommandLineInfo;
                m_pCmdInfo->m_nShellCommand = CCommandLineInfo::AppUnregister;
            }
        }
        break;
        default:
            l_result = CWinApp::ProcessShellCommand(f_cmd_info);
            break;
    }

    return l_result;
}
//-Patni-AMT/2009May15/Added/JFT# Unregister

int CPQMApp::IsRunAutomated()
{
    return m_run_automated;
}


int CPQMApp::IsRunEmbadded()
{
    return m_run_embadded;
}

bool CPQMApp::GetIsMouseActive()
{
    return m_bMouseActive;
}

void CPQMApp::SetIsMouseActive(bool bFlag)
{
    m_bMouseActive = bFlag;
}

void CPQMApp::SetVoiceMenuMode(const int mode)
{
    m_mode = mode;
}

/////////////////////////////////////////////////////////////////////////////
//************************************Method Header**************************************

//Method Name:  InitInstance

//Author:  PATNI/AC

//Purpose: CPQMApp initialization

//**************************************************************************************/
BOOL CPQMApp::InitInstance(
)
{
    CoInitialize(NULL);
    m_localize = new MPlus::I18n::CResourceLoader(this); //Language usha

    // Initialize OLE libraries
    if (!AfxOleInit())  {

        AfxMessageBox(IDP_OLE_INIT_FAILED);
        return FALSE;
    }

    AfxEnableControlContainer();
    //+TMSC-Tanoue/2009Oct04/ADDED MVC5087
    AfxOleGetMessageFilter()->EnableNotRespondingDialog(FALSE);
    AfxOleGetMessageFilter()->EnableBusyDialog(FALSE);
    //+TMSC-Tanoue/2009Oct04/ADDED MVC5087

    //    SetTranceInfo();
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
    SetRegistryKey(_T("Toshiba Medical Systems Corporation"));

    LoadStdProfileSettings();  // Load standard INI file options (including MRU)

    // Register the application's document templates.  Document templates
    //  serve as the connection between documents, frame windows and views.

    CSingleDocTemplate* pDocTemplate = NULL;
    pDocTemplate = new CSingleDocTemplate(
        IDR_MAINFRAME,
        RUNTIME_CLASS(CPQMDoc),
        RUNTIME_CLASS(CMainFrame),       // main SDI frame window
        RUNTIME_CLASS(CPQMView));
    pDocTemplate->SetServerInfo(

        IDR_SRVR_EMBEDDED, IDR_SRVR_INPLACE,
        RUNTIME_CLASS(CInPlaceFrame));
    AddDocTemplate(pDocTemplate);

    // Connect the COleTemplateServer to the document template.
    //  The COleTemplateServer creates new documents on behalf
    //  of requesting OLE containers by using information
    //  specified in the document template.
    m_server.ConnectTemplate(clsidpqm, pDocTemplate, TRUE);
    // Note: SDI applications register server objects only if /Embedding
    //   or /Automation is present on the command line.

    // Enable DDE Execute open
    EnableShellOpen();
    RegisterShellFileTypes(TRUE);

    // Parse command line for standard shell commands, DDE, file open
    CCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);

    // Check to see if launched as OLE server
    // + SM4 Register App AMT
    m_run_automated = cmdInfo.m_bRunAutomated;
    m_run_embadded  = cmdInfo.m_bRunEmbedded;

    if (m_run_embadded || m_run_automated) {
        // - SM4 Register App AMT
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
    if (!ProcessShellCommand(cmdInfo))
        return FALSE;

    hInstance = m_hInstance;

    // The one and only window has been initialized, so show and update it.
    m_pMainWnd->ShowWindow(SW_SHOW);

    CRect main_rect ;
    m_pMainWnd->GetWindowRect(&main_rect);

    const CPoint offset = CPoint(258, 581);
    m_pMainWnd->MoveWindow(CRect(offset.x, offset.y , offset.x + main_rect.Width(), offset.y + main_rect.Height()), TRUE);

    const int border_width = GetSystemMetrics(SM_CXBORDER) + 1;
    const int border_Height = GetSystemMetrics(SM_CYBORDER) + 1;

    CRgn rgn;
    rgn.CreateRectRgn(main_rect.left + border_width, main_rect.top + border_Height, main_rect.left + main_rect.Width() - border_width, main_rect.top + main_rect.Height() - border_Height);
    m_pMainWnd->SetWindowRgn((HRGN)rgn, FALSE);
    rgn.DeleteObject();
    //-------------------------------------------------------------------------

    m_pMainWnd->UpdateWindow();

    // Enable drag/drop open
    m_pMainWnd->DragAcceptFiles();

    // For application start
    //  CPQMLogMgr::GetInstance()->WriteOpeLog(OP_ID_APP_START);
    return TRUE;
}

int CPQMApp::ExitInstance()
{
    DEL_PTR(m_localize)

    CoUninitialize();
    return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog,
    private MR::ACQ::SM::COMMON::NonCopyable<CAboutDlg>
{
public:
    // Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    //}}AFX_DATA

    CAboutDlg();

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
    // No message handlers
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(
    CDataExchange* pDX
)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}

// App command to run the dialog
void CPQMApp::OnAppAbout(
)
{
    CAboutDlg aboutDlg;
    aboutDlg.DoModal();
}




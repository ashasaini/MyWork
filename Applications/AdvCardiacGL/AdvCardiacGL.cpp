// AdvCardiacGL.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "AdvCardiacGL.h"
#include "MainFrm.h"
#include "IpFrame.h"
#include "AdvCardiacGLDoc.h"
#include "AdvCardiacGLView.h"
//#include "ACGAppController.h"
#pragma warning(disable:4786)

//#pragma comment(compiler,_T("/IGNORE:4786"));
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CAdvCardiacGLApp

BEGIN_MESSAGE_MAP(CAdvCardiacGLApp, CWinApp)
//{{AFX_MSG_MAP(CAdvCardiacGLApp)
ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
// Standard file based document commands
ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdvCardiacGLApp construction

CAdvCardiacGLApp::CAdvCardiacGLApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAdvCardiacGLApp object

CAdvCardiacGLApp theApp;

// This identifier was generated to be statistically unique for your app.
// You may change it if you prefer to choose a specific identifier.

// {A59D947A-19FD-431C-BDC2-3981BB01BB11}
static const CLSID clsid =
{ 0xa59d947a, 0x19fd, 0x431c, { 0xbd, 0xc2, 0x39, 0x81, 0xbb, 0x1, 0xbb, 0x11 } };

/////////////////////////////////////////////////////////////////////////////
// CAdvCardiacGLApp initialization

BOOL CAdvCardiacGLApp::InitInstance()
{
	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		//		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	
	AfxEnableControlContainer();
	
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
	
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
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
		RUNTIME_CLASS(CAdvCardiacGLDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CAdvCardiacGLView));
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
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
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
	
	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_HIDE);
	
	//((CAdvCardiacGLView*)m_pMainWnd)->StartGUI();	
	
    m_pMainWnd->UpdateWindow();
	
    // Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();
	
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog 
{
public:
	CAboutDlg();
	
	// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//{{AFX_MSG_MAP(CAboutDlg)
// No message handlers
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CAdvCardiacGLApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

CAdvCardiacGLApp::~CAdvCardiacGLApp()
{
	
}

BOOL CAdvCardiacGLApp::RegWinZOrder(HWND hWnd_i)
{
    BOOL l_result = FALSE;
    
    CDPSComManager *l_server = CDPSComManager::GetInstance();
    
    if( l_server != NULL ){
        
        IMplusMessage *l_msg = NULL;
        
        CString l_sparent(_T(""));
        l_sparent.Format(_T("%s"),(LPCTSTR)(l_server->GetParentName()));
        l_server->Connect(l_sparent);
		
        l_msg = l_server->CreateMessage(FW_RegisterWindowZOrder,1,l_server->GetParentName()); 
        
        CString l_parameter = _T("");
        l_parameter.Format(_T("%ld/%s"), hWnd_i,_T("Z_ORDER_FLOAT_GL_LAYER"));
        l_parameter += _T("/");
        l_parameter += _T("Top");
        
        l_msg->SetParameter( _bstr_t(l_parameter));
        if (!l_server->SendMsg(l_msg) ) {
            l_result = FALSE;
        }
        
        DEL_MSG(l_msg);
        l_msg = NULL;
        l_server->Disconnect();
    }		
    return l_result;	
}
///////////////////////////////// END OF FILE /////////////////////////////////
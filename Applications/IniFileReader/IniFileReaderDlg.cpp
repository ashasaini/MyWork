// IniFileReaderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IniFileReader.h"
#include "IniFileReaderDlg.h"
#include <shlwapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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

/////////////////////////////////////////////////////////////////////////////
// CIniFileReaderDlg dialog

CIniFileReaderDlg::CIniFileReaderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIniFileReaderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIniFileReaderDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_csFilepath.Empty();
    CString l_ini_file_name(_T(""));
    m_csFilepath = getenv("MRMP_DATA");
    m_csFilepath += _T("\\KtRecon");

    m_csFileName = _T("KtRecon_Plan.ini");

    m_csFilePathWithName = m_csFilepath + _T("\\") + m_csFileName;
}

void CIniFileReaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIniFileReaderDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIniFileReaderDlg, CDialog)
	//{{AFX_MSG_MAP(CIniFileReaderDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIniFileReaderDlg message handlers

BOOL CIniFileReaderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CIniFileReaderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CIniFileReaderDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CIniFileReaderDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CIniFileReaderDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CIniFileReaderDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CIniFileReaderDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
    CString l_csStudy(_T("[/gp/site/site5000/study/Runxxx"));
    CString l_csProtocol(_T("1000_1"));
    CString l_csKtReconFlag(_T("ON"));
    DeleteProtocol(l_csStudy,l_csProtocol);
	
}

void CIniFileReaderDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
    CString l_csStudy(_T("[/gp/site/site5000/study/Runxxx"));
    CString l_csProtocol(_T("1000_1"));
    CString l_csKtReconFlag(_T("ON"));
    AddOrUpdateProtocol(l_csStudy,l_csProtocol,l_csKtReconFlag);
	
}

void CIniFileReaderDlg::OnButton3() 
{
	// TODO: Add your control notification handler code here
    CString l_csStudy(_T("[/gp/site/site5000/study/Runxxx"));
    CString l_csProtocol(_T("1000_1"));
    CString l_csKtReconFlag(_T("OFF"));
    AddOrUpdateProtocol(l_csStudy,l_csProtocol,l_csKtReconFlag);
	
}

void CIniFileReaderDlg::OnButton4() 
{
	// TODO: Add your control notification handler code here
    CString l_csStudy(_T("[/gp/site/site5000/study/Runxxx"));
    CString l_csProtocol(_T("1000_1"));
    CString l_csKtReconFlag(_T(""));
    BOOL l_bKtReconFlag = FALSE;
    ReadProtocol(l_csStudy,l_csProtocol,l_csKtReconFlag,l_bKtReconFlag);
    AfxMessageBox(l_csKtReconFlag);
	
}

void CIniFileReaderDlg::OnButton5() 
{
	// TODO: Add your control notification handler code here
    CString l_csStudy(_T("[/gp/site/site5000/study/Runxxx"));
    DeleteSection(l_csStudy);
	
}


long CIniFileReaderDlg::AddOrUpdateProtocol( CString &f_csStudy,CString &f_csProtocol,CString &f_csKtReconFlag )
{
//     if(!PathFileExists(m_csFilePathWithName)) { //if file does not exists create it
//         
//         TCHAR destFldr[100] = {'\0'};
//        _stprintf(destFldr,_T("%s"),m_csFilepath);
//         
//         if(ERROR_PATH_NOT_FOUND != CreateDirectory(destFldr, NULL)) {
//             HANDLE file = CreateFile(
//                 m_csFilePathWithName, GENERIC_WRITE, 0, 0,
//                 CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);
//         }else{
//             return E_ERROR;
//         }        
//     }
    
    long l_lRetValue = E_NO_ERROR;
    l_lRetValue = WritePrivateProfileString (f_csStudy, f_csProtocol, 
        f_csKtReconFlag, m_csFilePathWithName);
    
    return l_lRetValue;
}

long CIniFileReaderDlg::DeleteProtocol( CString &f_csStudy,CString &f_csProtocol )
{
    if(!PathFileExists(m_csFilePathWithName)) {
        return E_ERROR;
    }
    
    long l_lRetValue = E_NO_ERROR;
    l_lRetValue = WritePrivateProfileString (f_csStudy, f_csProtocol, 
        NULL, m_csFilePathWithName);
    
    return l_lRetValue;
}

long CIniFileReaderDlg::DeleteSection( CString &f_csStudy )
{
    if(!PathFileExists(m_csFilePathWithName)) {
        return E_ERROR;
    }
    
    long l_lRetValue = E_NO_ERROR;
    l_lRetValue = WritePrivateProfileString (f_csStudy, NULL, 
        NULL, m_csFilePathWithName);
    
    return l_lRetValue;
}

long CIniFileReaderDlg::ReadProtocol( CString &f_csStudy,CString &f_csPtotocol,CString &f_csKtReconFlag,BOOL &f_bKtReconFlag )
{
    f_bKtReconFlag = FALSE;
    if(!PathFileExists(m_csFilePathWithName)) {
        f_bKtReconFlag = FALSE;
        return E_ERROR;
    }
    
    TCHAR l_cKeyVal[255];
    
    long l_lRetValue = E_NO_ERROR;	
    l_lRetValue = GetPrivateProfileString(f_csStudy,f_csPtotocol,
        _T(""),l_cKeyVal, 255, m_csFilePathWithName);	
    
    f_csKtReconFlag.Empty();
    f_csKtReconFlag.Format(_T("%s"),l_cKeyVal);

    if(f_csKtReconFlag.CompareNoCase(_T("ON")) == 0) {
        f_bKtReconFlag = TRUE;
    } else {
        f_bKtReconFlag = FALSE;
    }

    return l_lRetValue;
}

long CIniFileReaderDlg::AddSection( CString &f_csStudy )
{
    if(!PathFileExists(m_csFilePathWithName)) {//if file does not exists create it
        //return E_ERROR;
    }
    
    long l_lRetValue = E_NO_ERROR;
    l_lRetValue = WritePrivateProfileString (f_csStudy, NULL, 
        NULL, m_csFilePathWithName);
    
    return l_lRetValue;
}

void CIniFileReaderDlg::OnButton6() 
{
	// TODO: Add your control notification handler code here
    CString l_csStudy(_T("/gp/site/site5000/study/Runxxx"));
    AddSection(l_csStudy);	
}

int CIniFileReaderDlg::GetKtReconLicense( BOOL &f_nLicense )
{
    CString l_ini_file_name(_T(""));
    l_ini_file_name = getenv("MRMP_Config");
    l_ini_file_name += _T("\\PQM");
    l_ini_file_name += _T("\\PQM_KtRecon_License.ini");
    
    if(PathFileExists(l_ini_file_name)) {

        TCHAR ac_Result[255]={'\0'};
        CString l_csSection(_T("KtRecon_License"));
        CString l_csKey(_T("KtRecon"));

        long l_lRetValue = E_NO_ERROR;
        l_lRetValue = GetPrivateProfileString(l_csSection,l_csKey,
            _T(""),ac_Result, 255, l_ini_file_name);	

        CString l_csLicense(_T(""));
        l_csLicense.Format(_T("%s"),ac_Result);    
        
        
        if(l_csLicense.CompareNoCase(_T("ON")) == 0) {
            f_nLicense = TRUE;
        } else {
            f_nLicense = FALSE;
        }
    }else{
        f_nLicense = FALSE;
    }
    
    return E_NO_ERROR;
}

void CIniFileReaderDlg::OnButton7() 
{
	// TODO: Add your control notification handler code here
    BOOL l_bLicense = FALSE;
    GetKtReconLicense(l_bLicense);
    
    if(TRUE == l_bLicense){
        AfxMessageBox(_T("ON"));
    }else{
        AfxMessageBox(_T("OFF"));
    }	
}

// Used to retrieve all key/value pairs of a given section.  
CStringList* CIniFileReaderDlg::GetAllProtocolsData(CString &f_csStudy)  
{
    char ac_Result[2000];  //change size depending on needs
    
    CStringList *l_sectionDataList;
    l_sectionDataList->RemoveAll();

    long l_lRetValue = GetPrivateProfileSection((LPCTSTR)f_csStudy, ac_Result, 2000, (LPCTSTR)m_strFileName);
    CString strSectionData;

    for(int i=0; i<l_lRetValue; i++)
    {
        if(ac_Result[i] != '\0') {
            strSectionData = strSectionData + ac_Result[i];
        } else {
            if(strSectionData != "") {
                l_sectionDataList->InsertAfter(l_sectionDataList->GetTailPosition(),strSectionData);
            }
            strSectionData = "";
        }
    }
    
    return l_sectionDataList;
}

void CIniFileReaderDlg::ReadAllProtocols()
{
    CString f_csStudy(_T("123"));
    CStringList* l_csDataList = GetAllProtocolsData(f_csStudy);
    
    for(){

    }
    
}


// int ReadKtReconPlan(char *f_cInStudy,char *f_cInProtocol,char * f_cOutKtReconFlag, bool f_bOutKtReconFlag)
// {
//     //KtRecon_Plan.ini
//     //file handle
//     //open file
//     SearchStudy();
//     //if study is found then search for protocol
//     //if protocol is found then read the KtRecon flag
// }
// 
// int SearchStudy(FILE *fp,char *f_cInStudy,bool f_bOutIsFound,int f_nOutLineNum)
// {
// 
// }
// 
// int SerachProtocol(FILE *fp,char *f_cInProtocol,int f_nInLineNum,bool f_bOutIsFound,int f_nOutLineNum)
// {
// 
// }
// 
// int GetKtReconFlag(FILE *fp,char *f_cInProtocol,int f_nInLineNum)
// {
// 
// }

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include "../ini.h"
// 
// typedef struct
// {
//     int version;
//     const char* name;
//     const char* email;
// } configuration;
// 
// static int handler(void* user, const char* section, const char* name,
//                    const char* value)
// {
//     configuration* pconfig = (configuration*)user;
//     
// #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
//     if (MATCH("protocol", "version")) {
//         pconfig->version = atoi(value);
//     } else if (MATCH("user", "name")) {
//         pconfig->name = strdup(value);
//     } else if (MATCH("user", "email")) {
//         pconfig->email = strdup(value);
//     } else {
//         return 0;  /* unknown section/name, error */
//     }
//     return 1;
// }
// 
// int main(int argc, char* argv[])
// {
//     configuration config;
//     
//     if (ini_parse("test.ini", handler, &config) < 0) {
//         printf("Can't load 'test.ini'\n");
//         return 1;
//     }
//     printf("Config loaded from 'test.ini': version=%d, name=%s, email=%s\n",
//         config.version, config.name, config.email);
//     return 0;
// }
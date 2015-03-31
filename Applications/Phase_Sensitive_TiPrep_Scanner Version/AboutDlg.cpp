// AboutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tiprep.h"
#include "AboutDlg.h"

#include "TiPrepTrace.h"
#include "TiPrepLogHelper.h"
#include "TiPrepView.h"
#include <tami/common/machdefs.h>
#include <tami/libinit/Init.h>
#include <SYS/STAT.H>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog


const CString EXCEPTION_ABOUTDLG = L"Exception occurred in About dialog";

CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
: CPopupDlg(CAboutDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_hIcon = AfxGetApp()->LoadIcon(IDI_TIPREP_ICON2); //Inside the dialog's constructor.
    m_csMpowerVersion = _T("MPOWER ");
}

// copy constructor
CAboutDlg::CAboutDlg(CAboutDlg& f_aboutdlg): CPopupDlg(CAboutDlg::IDD)
{
//     //TIPREP_TRACE(CAboutDlg::CAboutDlg);      
	m_hIcon = f_aboutdlg.m_hIcon;
 }

//assignment operator
// CAboutDlg& CAboutDlg::operator = (CAboutDlg& f_aboutdlg)
// {
//     //TIPREP_TRACE(CAboutDlg::CAboutDlg);
//     
//     if(this == &f_aboutdlg) {
//         return(*this);
//     }
//     
//     return (*this);
// }

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CPopupDlg::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    DDX_Control(pDX, IDOK, m_AboutOk);
    DDX_Control(pDX, IDC_STATIC_ABOUT_ICON, m_StaticAbout3);
    DDX_Control(pDX, IDC_STATIC_ABOUT2, m_StaticAbout2);
    DDX_Control(pDX, IDC_STATIC_ABOUT1, m_StaticAbout1);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutDlg, CPopupDlg)
//{{AFX_MSG_MAP(CAboutDlg)
ON_WM_CTLCOLOR()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
CAboutDlg& CAboutDlg::operator = (CAboutDlg& f_aboutdlg)
{
    if(this == &f_aboutdlg) {
        return(*this);
    }
	    m_hIcon = f_aboutdlg.m_hIcon;
    return (*this);
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg message handlers

HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
    HBRUSH hbr;
    try {
        hbr= CPopupDlg::OnCtlColor(pDC, pWnd, nCtlColor);
        
        if(nCtlColor == CTLCOLOR_STATIC) {
            pDC->SetBkColor(BKCOLOR);
            pDC->SetTextColor(TEXT_BKCOLOR);
            pDC->SetBkMode(OPAQUE);
            return (HBRUSH)m_list_brush;
        } else {
            //
        }
        
        return hbr;
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(_T("Exception About Dlg"), __LINE__, __FILE__);
        return NULL;
    }
    
    // TODO: Change any attributes of the DC here
    
    // TODO: Return a different brush if the default is not desired
    return hbr;
}

BOOL CAboutDlg::OnInitDialog() 
{
    TIPREP_TRACE(CAboutDlg::OnInitDialog,MRTraceMarker3);
    CPopupDlg::OnInitDialog();
    
    WINDOW_COLORS_t l_window_color;
    GetWindowColors(l_window_color);
    SetWindowTextA(GetSafeHwnd(),"About TiPrep");
    
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon
    
                                    /*SendMessageA(
                                    GetDlgItem(IDC_STATIC_ABOUT_ICON)->GetSafeHwnd(),
                                    WM_SETICON,
                                    ICON_BIG,
    reinterpret_cast<LPARAM>(loadIcon()));*/
    
    SendMessageA(
        GetDlgItem(IDC_STATIC_ABOUT_ICON)->GetSafeHwnd(),
        STM_SETICON,
        reinterpret_cast<WPARAM>(
        loadIcon(
        64,
        64)),
        0);
    
    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    
    m_list_brush.CreateSolidBrush(l_window_color.crClientColor);
    SetRibbonHeight(0);
    
    this->SetFont(_T("Arial"), TEXT_SIZE, true);
    m_AboutOk.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
    
    SetBkMode((m_StaticAbout3.GetDC())->GetSafeHdc(),TRANSPARENT);
    m_StaticAbout3.SetFont(&m_font, true);
    SetBkMode((m_StaticAbout1.GetDC())->GetSafeHdc(),TRANSPARENT);
    m_StaticAbout1.SetFont(&m_font, true);
    SetBkMode((m_StaticAbout2.GetDC())->GetSafeHdc(),TRANSPARENT);
    m_StaticAbout2.SetFont(&m_font, true);
    
    if(FALSE == ReadMpowerVersion()){
        CTiPrepLogHelper::WriteToErrorLog(_T("ReadMpowerVersion failed"), __LINE__, __FILE__);
    }
    SetDlgItemText(IDC_STATIC_ABOUT1,m_csMpowerVersion);
    
    // TODO: Add extra initialization here
    CTiPrepView::GetView()->AddZOrderLayer();
    CTiPrepView::GetView()->RegisterWindowZOrder(m_hWnd);
    
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

// void CAboutDlg::OnOK() 
// {
// 	// TODO: Add extra validation here
// 	TIPREP_TRACE(CAboutDlg::OnOK);
// 	CPopupDlg::OnOK();
// }

HICON CAboutDlg::loadIcon(int x, int y)
{
    bool use_system_icon = false;
    const int oic_winlogo = 32517; // OIC_WINLOGO
    
    
    HINSTANCE hInstance = use_system_icon ? NULL : GetModuleHandleW(NULL);
    
    HICON hIcon = static_cast<HICON>(
        LoadImageW(
        hInstance,
        reinterpret_cast<LPCWSTR>(
        use_system_icon ? oic_winlogo : IDI_TIPREP_ICON),
        IMAGE_ICON,
        x,
        y,
        // We specify LR_SHARED regardless of whether we are loading a
        // system icon, because we otherwise would need to check whether we
        // need to free the icon or not (as we mustn't, for system icons).
        // With LR_SHARED, the system takes care of releasing the resource.
        LR_SHARED));
    //assert(hIcon);
    
    if (use_system_icon
        && x == 48
        && y == 48)
    {
        // To get an 48x48 version of the system icon OIC_WINLOGO, we may need
        // to resample it, as we may not have been able to load an 48x48
        // version of it. Resampling may be a bit ugly, but since this system
        // icon is only used as a placeholder in the about dialog (shown next
        // to the about dialog text), it's okay.
        hIcon = static_cast<HICON>(CopyImage(hIcon, IMAGE_ICON, 48, 48, LR_COPYDELETEORG));
        //assert(hIcon);
    }
    
    return hIcon;
}

HICON CAboutDlg::loadIcon()
{
    return loadIcon(
        GetSystemMetrics(SM_CXICON),
        GetSystemMetrics(SM_CYICON));
}

void CAboutDlg::OnCancel()
{
    TIPREP_TRACE(CAboutDlg::OnCancel,MRTraceMarker3);
    
    try {
        CTiPrepView::GetView()->UnRegisterWindowZOrder(m_hWnd);
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_ABOUTDLG, __LINE__, __FILE__);
    }
    
    CPopupDlg::OnCancel();
}
void CAboutDlg::OnOK()
{
    TIPREP_TRACE(CAboutDlg::OnOK,MRTraceMarker3);
    
    try {
        CTiPrepView::GetView()->UnRegisterWindowZOrder(m_hWnd);
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_ABOUTDLG, __LINE__, __FILE__);
    }
    
    CPopupDlg::OnOK();
}

BOOL CAboutDlg::ReadMpowerVersion()
{
    FILE *fp = NULL;
    Common_t	common_p;// = NULL;
    char filename[100];
    CString csEtcPath(_T(""));
    char sm_version[INIT_VERSION_SIZE] = {'\0'};
    char rm_version[INIT_VERSION_SIZE] = {'\0'};
    
    try{
        if(FALSE == GetEtcPath(csEtcPath)){//etc fails
            CTiPrepLogHelper::WriteToErrorLog(_T("GetEtcPath failed"), __LINE__, __FILE__);
            if(E_NO_ERROR == InitGetVersion(sm_version,rm_version)){
                m_csMpowerVersion += sm_version;
                return TRUE;
            }else{
                CTiPrepLogHelper::WriteToErrorLog(_T("SM_VERSION Read failed"), __LINE__, __FILE__);
                return FALSE;
            }
        }else{
            csEtcPath += _T("/"); 
            csEtcPath += _T(SYS_SM_CF);
            sprintf(filename ,"%S",csEtcPath);
            if((fp = fopen(filename,"r")) == NULL){//fopen fails
                CString lcs(csEtcPath);
                lcs += _T(":SM_CF file open failed");
                CTiPrepLogHelper::WriteToErrorLog(lcs, __LINE__, __FILE__);
                if(E_NO_ERROR == InitGetVersion(sm_version,rm_version)){
                    m_csMpowerVersion += sm_version;
                    return TRUE;
                }else{
                    CTiPrepLogHelper::WriteToErrorLog(_T("SM_VERSION Read failed"), __LINE__, __FILE__);
                    return FALSE;
                }
            }else{
                if(E_NO_ERROR == InitReadSmConf(fp,&common_p)){//SM_CF file read fails
                        if(0 == strcmp(common_p.sm_version,"")){
                            CTiPrepLogHelper::WriteToErrorLog(_T("common_p->sm_version is NULL"), __LINE__, __FILE__);
                            if(E_NO_ERROR == InitGetVersion(sm_version,rm_version)){
                                m_csMpowerVersion += sm_version;
                                if(NULL != fp){
                                    fclose(fp);
                                }
                                return TRUE;
                            }else{
                                CTiPrepLogHelper::WriteToErrorLog(_T("SM_VERSION Read failed"), __LINE__, __FILE__);
                                if(NULL != fp){
                                    fclose(fp);
                                }
                                return FALSE;
                            }
                        }else{
                            m_csMpowerVersion += common_p.sm_version;
                        }
                }else{
                    CTiPrepLogHelper::WriteToErrorLog(_T("InitReadSmConf failed"), __LINE__, __FILE__);
                    if(E_NO_ERROR == InitGetVersion(sm_version,rm_version)){
                        m_csMpowerVersion += sm_version;
                        if(NULL != fp){
                            fclose(fp);
                        }
                        return TRUE;
                    }else{
                        CTiPrepLogHelper::WriteToErrorLog(_T("SM_VERSION Read failed"), __LINE__, __FILE__);
                        if(NULL != fp){
                            fclose(fp);
                        }
                        return FALSE;
                    }                
                }                
            }            
        }
        if(0 != strcmp(common_p.sm_version,"")){
            if(E_NO_ERROR == InitGetVersion(sm_version,rm_version)){//CrossCheck with engine            
                if(strcmp(common_p.sm_version , sm_version) != 0){
                    CTiPrepLogHelper::WriteToErrorLog(_T("SM_Version Mismatch"), __LINE__, __FILE__);
                }
            }
        }
        if(NULL != fp){
            fclose(fp);
        }
    }catch(...){
        if(NULL != fp){
            fclose(fp);
        }
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_ABOUTDLG, __LINE__, __FILE__);
        return FALSE;
    }
    return TRUE;
}

BOOL CAboutDlg::GetEtcPath( CString &csEtcPath )
{
    char        *env = NULL;
    char        path[100] = {'\0'};
    struct _stat stBuf;
    try{
        env = getenv(ENV_GP_ETC);
        if(env == NULL) {
            csEtcPath = (_T(ETC_DEF_PATH));
        }
        else{
            strcpy(path,env);
            csEtcPath = env;
            if(_stat(path,&stBuf) || (stBuf.st_mode & _S_IFDIR)) {
                csEtcPath = (_T(ETC_DEF_PATH));
            }
        }
    }catch(...){
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_ABOUTDLG, __LINE__, __FILE__);
        return FALSE;
    }
    return TRUE;
    
}

CAboutDlg::~CAboutDlg()
{
	try{
		m_list_brush.DeleteObject();
	}catch(...)
	{
		CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_ABOUTDLG, __LINE__, __FILE__);

	}
}

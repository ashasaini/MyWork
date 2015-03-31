// OpenRawDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CommonDefinations.h"
#include "epi.h"
#include "EPIController.h"
#include "OpenRawDlg.h"
#include "VFCReader.h"
#include "FileReader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COpenRawDlg dialog


COpenRawDlg::COpenRawDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COpenRawDlg::IDD, pParent),m_csRawFilePath(_T(""))
{
	//{{AFX_DATA_INIT(COpenRawDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void COpenRawDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COpenRawDlg)
	DDX_Control(pDX, IDC_COMBO_RAW_FORM, m_ComboRawForm);
	DDX_Control(pDX, IDC_EDIT_RAW_WIDHT, m_EditRawWidth);
	DDX_Control(pDX, IDC_EDIT_RAW_HEIGHT, m_EditRawHeight);
	DDX_Control(pDX, IDC_COMBO_RAW_TYPE, m_ComboRawType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COpenRawDlg, CDialog)
	//{{AFX_MSG_MAP(COpenRawDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpenRawDlg message handlers

void COpenRawDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void COpenRawDlg::OnOK() 
{
    double * l_dImageBuff = NULL;
    double *l_dDataDumpBuff = NULL;

    try{
    CString l_csHeight(_T(""));
    CString l_csWidth(_T(""));
    CString l_csDatatype(_T(""));
    CString l_csDataForm(_T(""));

    m_EditRawHeight.GetWindowText(l_csHeight);
    m_EditRawWidth.GetWindowText(l_csWidth);

    int l_nSelPos = m_ComboRawType.GetCurSel();
    int l_nBuffCnt = m_ComboRawType.GetLBTextLen(l_nSelPos);    
    if(l_nSelPos < 0 || l_nBuffCnt < 0) {
        return;
    }    
    m_ComboRawType.GetLBText(l_nSelPos, l_csDatatype.GetBuffer(l_nBuffCnt));
    l_csDatatype.ReleaseBuffer();

     l_nSelPos = m_ComboRawForm.GetCurSel();
     l_nBuffCnt = m_ComboRawForm.GetLBTextLen(l_nSelPos);    
    if(l_nSelPos < 0 || l_nBuffCnt < 0) {
        return;
    }    
    m_ComboRawForm.GetLBText(l_nSelPos, l_csDataForm.GetBuffer(l_nBuffCnt));
    l_csDataForm.ReleaseBuffer();


    if(l_csDataForm.IsEmpty()||l_csDatatype.IsEmpty()||l_csHeight.IsEmpty()||l_csWidth.IsEmpty()||m_csRawFilePath.IsEmpty()){
        return;
    }

    char l_cpC[150] = {'\0'};
    wcstombs(l_cpC, l_csHeight.AllocSysString(), 100);
     int l_nHeight = atoi(l_cpC);

     wcstombs(l_cpC,l_csWidth.AllocSysString(), 100);
     int l_nWidth = atoi(l_cpC);

     if(l_nWidth <= 0 || l_nHeight <= 0){
         return;
     }

    int newImageData = l_nHeight*l_nWidth*2;

    
    CFileReader l_filreader;
    int l_nStatus = l_filreader.ReadImage(m_csRawFilePath, newImageData, l_dImageBuff);
    
    if(0 != l_nStatus) {
        DELARR(l_dImageBuff);
        return ;
    }
    int DataForm = 0;//Magnitude
    
    if(l_csDataForm.CollateNoCase(sMAGNITUDE) == 0) {
        DataForm = MAGNITUDE;
    }
    
    if(l_csDataForm.CollateNoCase(sPHASE) == 0) {
        DataForm = PHASE;
    }
    
    if(l_csDataForm.CollateNoCase(sREAL) == 0) {
        DataForm = REAL;
    }
    
    if(l_csDataForm.CollateNoCase(sIMAGINARY) == 0) {
        DataForm = IMAGINARY;
    }
    CVFCReader l_vcfHandle;
    switch(DataForm) {
    case MAGNITUDE:
        l_vcfHandle.MakeMagnitudeData(l_dImageBuff, newImageData, l_dDataDumpBuff);
        break;
    case PHASE:
        l_vcfHandle.MakePhaseData(l_dImageBuff, newImageData, l_dDataDumpBuff);
        break;
    case REAL:
        l_vcfHandle.MakeRealData(l_dImageBuff, newImageData, l_dDataDumpBuff);
        break;
    case IMAGINARY:
        l_vcfHandle.MakeImaginaryData(l_dImageBuff, newImageData, l_dDataDumpBuff);
        break;
    default:
        DELARR(l_dImageBuff);
        MEMFREE(l_dDataDumpBuff);
        return;
    }    
    CEPIController::GetInstance()->displayImage(l_dDataDumpBuff, l_nHeight, l_nWidth);
    DELARR(l_dImageBuff);
    MEMFREE(l_dDataDumpBuff);
    }
    catch(...){
        DELARR(l_dImageBuff);
       MEMFREE(l_dDataDumpBuff);
    }
     
    CDialog::OnOK();
}

void COpenRawDlg::SetRawFilePath( CString &csFilepath )
{
    m_csRawFilePath = csFilepath;
}

BOOL COpenRawDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetWindowText(m_csRawFilePath);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

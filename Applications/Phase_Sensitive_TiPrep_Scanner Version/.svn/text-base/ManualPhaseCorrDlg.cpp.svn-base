// ManualPhaseCorrDlg.cpp : implementation file
//
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/

#include "stdafx.h"
#include "tiprep.h"
#include "ManualPhaseCorrDlg.h"
#include "TiPrepView.h"
#include "TiPrepLogHelper.h"
#include "TiPrepController.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CManualPhaseCorrDlg dialog


/*************************************************************
* Method:    	CManualPhaseCorrDlg
* Description: 	This function is used 
* Parameter: 	CWnd * pParent :
* Returns:   	
*************************************************************/
CManualPhaseCorrDlg::CManualPhaseCorrDlg(CWnd* pParent /*=NULL*/)
    : CPopupDlg(CManualPhaseCorrDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CManualPhaseCorrDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_hIcon = AfxGetApp()->LoadIcon(IDI_TIPREP_ICON2); //Inside the dialog's constructor.
}
CManualPhaseCorrDlg& CManualPhaseCorrDlg::operator = (CManualPhaseCorrDlg& f_manualphasecorr)
{
    if(this == &f_manualphasecorr) {
        return(*this);
    } 
	m_hIcon = f_manualphasecorr.m_hIcon;
    return (*this);
}

CManualPhaseCorrDlg::CManualPhaseCorrDlg(CManualPhaseCorrDlg & f_manual) : CPopupDlg(CManualPhaseCorrDlg::IDD)
{
	m_hIcon = f_manual.m_hIcon;
}

void CManualPhaseCorrDlg::DoDataExchange(CDataExchange* pDX)
{
    CPopupDlg::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CManualPhaseCorrDlg)
    DDX_Control(pDX, IDC_BUTTON_SELECT_REAL, m_Button_Select_Real);
    DDX_Control(pDX, IDC_BUTTON_SELECT_MAGNITUDE, m_Button_Select_Magnitude);
    DDX_Control(pDX, IDC_BUTTON_SELECT_IMAGINARY, m_Button_Select_Imaginary);
    DDX_Control(pDX, IDOK, m_Button_Ok);
    DDX_Control(pDX, IDC_EDIT_REAL, m_Edit_Real);
    DDX_Control(pDX, IDC_EDIT_MAGNITUDE, m_Edit_Magnitude);
    DDX_Control(pDX, IDC_EDIT_IMAGINARY, m_Edit_Imaginary);
    DDX_Control(pDX, IDC_STATIC_LABEL_REAL, m_Label_Real);
    DDX_Control(pDX, IDC_STATIC_LABEL_MAGNITUDE, m_Label_Magnitude);
    DDX_Control(pDX, IDC_STATIC_LABEL_IMAGINARY, m_Label_Imaginary);
    DDX_Control(pDX, IDC_BUTTON_CORRECT_PHASE_MANUAL, m_Button_Phase_Corr);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CManualPhaseCorrDlg, CPopupDlg)
    //{{AFX_MSG_MAP(CManualPhaseCorrDlg)
    ON_BN_CLICKED(IDC_BUTTON_CORRECT_PHASE_MANUAL, OnButtonCorrectPhaseManual)
    ON_BN_CLICKED(IDC_BUTTON_SELECT_IMAGINARY, OnButtonSelectImaginary)
    ON_BN_CLICKED(IDC_BUTTON_SELECT_MAGNITUDE, OnButtonSelectMagnitude)
    ON_BN_CLICKED(IDC_BUTTON_SELECT_REAL, OnButtonSelectReal)
    ON_WM_CTLCOLOR()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CManualPhaseCorrDlg message handlers

/*************************************************************
* Method:    	OnInitDialog
* Description: 	This function is used 
* Returns:   	BOOL
*************************************************************/
BOOL CManualPhaseCorrDlg::OnInitDialog()
{
    CPopupDlg::OnInitDialog();
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    WINDOW_COLORS_t l_window_color;
    GetWindowColors(l_window_color);
    m_list_brush.CreateSolidBrush(l_window_color.crClientColor);
    InitializeManualPhaseCorrDlgControls();
    CTiPrepView::GetView()->AddZOrderLayer();
    CTiPrepView::GetView()->RegisterWindowZOrder(m_hWnd);
    return TRUE;  
}

/*************************************************************
* Method:       InitializeAddOffsetDllgControls
* Description:  This function is used
* Returns:      void
*************************************************************/
void CManualPhaseCorrDlg::InitializeManualPhaseCorrDlgControls()
{
    CFont l_font;
    SetRibbonHeight(0);
    m_Label_Real.SetFont(&l_font, true);
    m_Edit_Real.SetFont(_T("Arial"), 10, true);
    m_Edit_Real.SetBkColor(BKCOLOR);
    m_Button_Select_Real.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
    m_Label_Magnitude.SetFont(&l_font, true);
    m_Edit_Magnitude.SetFont(_T("Arial"), 10, true);
    m_Edit_Magnitude.SetBkColor(BKCOLOR);
    m_Button_Select_Magnitude.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
    m_Label_Imaginary.SetFont(&l_font, true);
    m_Edit_Imaginary.SetFont(_T("Arial"), 10, true);
    m_Edit_Imaginary.SetBkColor(BKCOLOR);
    m_Button_Select_Imaginary.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
    m_Button_Phase_Corr.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
    m_Button_Ok.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
}

/*************************************************************
* Method:    	OnButtonCorrectPhaseManual
* Description: 	This function is used 
* Returns:   	void
*************************************************************/
void CManualPhaseCorrDlg::OnButtonCorrectPhaseManual()
{
    try{
    CTiPrepView::GetView()->SetWaitCursor(true);
    CString l_protocol_value_check = _T("");
    int l_nErrorCode = I_NO_ERROR;
    m_Edit_Magnitude.GetWindowText(l_protocol_value_check);
    BOOL l_protocol_value = l_protocol_value_check.IsEmpty();

    if(FALSE != l_protocol_value) {

        l_nErrorCode = E_SELECT_ALL_SERIES;
        CTiPrepView::GetView()->DisplayErr(l_nErrorCode,PHASE_CORRECTION_DLG);
        return;
    }

    m_Edit_Imaginary.GetWindowText(l_protocol_value_check);
    l_protocol_value = l_protocol_value_check.IsEmpty();

    if(FALSE != l_protocol_value) {

        l_nErrorCode = E_SELECT_ALL_SERIES;
        CTiPrepView::GetView()->DisplayErr(l_nErrorCode,PHASE_CORRECTION_DLG);
        return;
    }

    m_Edit_Real.GetWindowText(l_protocol_value_check);
    l_protocol_value = l_protocol_value_check.IsEmpty();

    if(FALSE != l_protocol_value) {

        l_nErrorCode = E_SELECT_ALL_SERIES;
        CTiPrepView::GetView()->DisplayErr(l_nErrorCode,PHASE_CORRECTION_DLG);
        return;
    }


    l_nErrorCode = CTiPrepController::GetInstance()->ValidateSelectedSeries();
    if(E_NO_ERROR == l_nErrorCode) {
        l_nErrorCode = CTiPrepController::GetInstance()->SeriesPhaseCorrection();
        if (E_NO_ERROR != l_nErrorCode)
        {
          //  l_nErrorCode = E_PHASE_CORRECTION;
            CTiPrepView::GetView()->DisplayErr(l_nErrorCode,PHASE_CORRECTION_DLG);
        }
    }else{
        CTiPrepView::GetView()->DisplayErr(l_nErrorCode,PHASE_CORRECTION_DLG);
        return;
    }

    CTiPrepView::GetView()->SetWaitCursor(false);
    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog
            (_T("Exception occurred in OnButtonCorrectPhaseManual()"),__LINE__,__FILE__);
    }
}

/*************************************************************
* Method:    	OnButtonSelectImaginary
* Description: 	This function is used 
* Returns:   	void
*************************************************************/
void CManualPhaseCorrDlg::OnButtonSelectImaginary()
{
    try{
        (CTiPrepView::GetView())->SetIselEvent(EVENT_IMG_SELECT);
        (CTiPrepView::GetView())->SetWaitCursor(true);
        BOOL l_bselImg = (CTiPrepView::GetView())->GetSelectedImages();

        if(FALSE == l_bselImg){
            (CTiPrepView::GetView())->SetWaitCursor(false);
        }
    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog
            (_T("Exception occurred in OnButtonSelectImaginary()"),__LINE__,__FILE__);
    }
}
/*************************************************************
* Method:    	OnButtonSelectMagnitude
* Description: 	This function is used 
* Returns:   	void
*************************************************************/
void CManualPhaseCorrDlg::OnButtonSelectMagnitude()
{
    try{
        (CTiPrepView::GetView())->SetIselEvent(EVENT_MG_SELECT);
        (CTiPrepView::GetView())->SetWaitCursor(true);
        BOOL l_bselImg = (CTiPrepView::GetView())->GetSelectedImages();

        if(FALSE == l_bselImg){
            (CTiPrepView::GetView())->SetWaitCursor(false);
        }
    }catch(...){
        CTiPrepLogHelper::WriteToErrorLog(
            (_T("Exception occured in OnButtonSelectMagnitude()")),__LINE__,__FILE__);
    }
}
/*************************************************************
* Method:    	OnButtonSelectReal
* Description: 	This function is used 
* Returns:   	void
*************************************************************/
void CManualPhaseCorrDlg::OnButtonSelectReal()
{
    try{
        (CTiPrepView::GetView())->SetIselEvent(EVENT_RE_SELECT);
        (CTiPrepView::GetView())->SetWaitCursor(true);
        BOOL l_bselImg = (CTiPrepView::GetView())->GetSelectedImages();

        if(FALSE == l_bselImg){
            (CTiPrepView::GetView())->SetWaitCursor(false);
        }
    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog(
            (_T("Exception occured in OnButtonSelectReal()")),__LINE__,__FILE__);
    }
}

/*************************************************************
* Method:    	OnOK
* Description: 	This function is used 
* Returns:   	void
*************************************************************/
void CManualPhaseCorrDlg::OnOK()
{
    CPopupDlg::OnOK();
}

/*************************************************************
* Method:    	DisplaySeriesSelectedForManualPhaseCorr
* Description: 	This function is used 
* Parameter: 	const CString & f_csSeriesName :
* Parameter: 	const int f_nSelectedDataType :
* Returns:   	void
*************************************************************/
void CManualPhaseCorrDlg::DisplaySeriesSelectedForManualPhaseCorr(
    const CString &f_csSeriesName,
    const int f_nSelectedDataType)
{
    switch(f_nSelectedDataType) {
        case MAGNITUDE:
            m_Edit_Magnitude.SetWindowText(f_csSeriesName);
            break;
        case REAL:
            m_Edit_Real.SetWindowText(f_csSeriesName);
            break;
        case IMGN:
            m_Edit_Imaginary.SetWindowText(f_csSeriesName);
            break;
        default:
            CTiPrepLogHelper::WriteToErrorLog(_T("DisplaySeriesSelectedForManualPhaseCorr(): Unknown data type"), __LINE__, __FILE__);
            break;
    }
}

/*************************************************************
* Method:    	OnCtlColor
* Description: 	This function is used 
* Parameter: 	CDC * pDC :
* Parameter: 	CWnd * pWnd :
* Parameter: 	UINT nCtlColor :
* Returns:   	HBRUSH
*************************************************************/
HBRUSH CManualPhaseCorrDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CPopupDlg::OnCtlColor(pDC, pWnd, nCtlColor);

    if(nCtlColor == CTLCOLOR_STATIC) {
        pDC->SetBkColor(BKCOLOR);
        pDC->SetTextColor(TEXT_BKCOLOR);
        pDC->SetBkMode(OPAQUE);
        return (HBRUSH)m_list_brush;
    } else {
        //
    }

    return hbr;
}

/*************************************************************
* Method:    	OnCancel
* Description: 	This function is used 
* Returns:   	void
*************************************************************/
void CManualPhaseCorrDlg::OnCancel()
{
    CTiPrepView::GetView()->UnRegisterWindowZOrder(m_hWnd);
    CPopupDlg::OnCancel();
}

CManualPhaseCorrDlg::~CManualPhaseCorrDlg()
{
	try{
		m_list_brush.DeleteObject();
	}catch(...)
	{
		CString l_csmsg(_T("Exception occurred in Manual Phase Correction dialog"));
        CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
		
	}
}

///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////
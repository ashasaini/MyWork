// PolarityCorrectionDlg.cpp : implementation file
//
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#include "stdafx.h"
#include "tiprep.h"
#include "TiPrepDlg.h"
#include "PolarityCorrectionDlg.h"
#include "TiPrepTrace.h"
#include "TiPrepLogHelper.h"
#include "TiPrepView.h"
#include "TiPrepController.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//****************************Method Header***************************
//Method Name   : CPolarityCorrectionDlg
//Purpose       : Constructor
//********************************************************************
CPolarityCorrectionDlg::CPolarityCorrectionDlg(CWnd* pParent /*=NULL*/)
: CPopupDlg(CPolarityCorrectionDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CPolarityCorrectionDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    /*TIPREP_TRACE(CPolarityCorrectionDlg::CPolarityCorrectionDlg);*/
    m_hIcon = AfxGetApp()->LoadIcon(IDI_TIPREP_ICON2); //Inside the dialog's constructor.
}
CPolarityCorrectionDlg& CPolarityCorrectionDlg::operator = (CPolarityCorrectionDlg& f_polarity_correctionDlg_ptr)
{
    /*TIPREP_TRACE(CPolarityCorrectionDlg::operator);*/
    
    if(this == &f_polarity_correctionDlg_ptr) {
        return(*this);
    }
    
    return (*this);
}
CPolarityCorrectionDlg::CPolarityCorrectionDlg(CPolarityCorrectionDlg & f_polaritycorrectiondlg) : CPopupDlg(CPolarityCorrectionDlg::IDD)
{
	m_hIcon = f_polaritycorrectiondlg.m_hIcon;
	m_selected_series_no = f_polaritycorrectiondlg.m_selected_series_no;
}
//****************************Method Header***************************
//Method Name   : CPolarityCorrectionDlg
//Purpose       : Destructor
//********************************************************************


//Add destructor

/*************************************************************
* Method:       DoDataExchange
* Description:  This function is used
* Parameter:    CDataExchange * pDX
* Returns:      void
*************************************************************/
void CPolarityCorrectionDlg::DoDataExchange(CDataExchange* pDX)
{
    CPopupDlg::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPolarityCorrectionDlg)
    DDX_Control(pDX, IDCANCEL, m_exit_polarity_correction);
    DDX_Control(pDX, IDC_MAGNITUDE_SELECT_BTN, m_magnitude_select_btn);
    DDX_Control(pDX, IDC_EDIT_MAGNITUDE, m_edit_magnitude);
    DDX_Control(pDX, IDC_STATIC_MAGNITUDE, m_static_magnitude);
    DDX_Control(pDX, IDC_POLARITY_CORRECTION_BTN, m_polarity_correction_btn);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPolarityCorrectionDlg, CPopupDlg)
//{{AFX_MSG_MAP(CPolarityCorrectionDlg)
// NOTE: the ClassWizard will add message map macros here
//ON_WM_PAINT()
ON_WM_CTLCOLOR()
ON_BN_CLICKED(IDC_MAGNITUDE_SELECT_BTN, OnMagnitudeSeriesSelectBtn)
ON_BN_CLICKED(IDC_POLARITY_CORRECTION_BTN, OnPolarityCorrectionBtnClick)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPolarityCorrectionDlg message handlers

/*************************************************************
* Method:       OnInitDialog
* Description:  This function is used
* Returns:      BOOL
*************************************************************/
BOOL CPolarityCorrectionDlg::OnInitDialog()
{
    TIPREP_TRACE(CPolarityCorrectionDlg::OnInitDialog,MRTraceMarker3);
    CPopupDlg::OnInitDialog();
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    WINDOW_COLORS_t l_window_color;
    GetWindowColors(l_window_color);
    m_list_brush.CreateSolidBrush(l_window_color.crClientColor);
    SetRibbonHeight(0);
    InitializeControls();
    CTiPrepView::GetView()->AddZOrderLayer();
    CTiPrepView::GetView()->RegisterWindowZOrder(m_hWnd);
    return TRUE;  
}

/*************************************************************
* Method:       OnCtlColor
* Description:  This function is used
* Parameter:    CDC * pDC
* Parameter:    CWnd * pWnd
* Parameter:    UINT nCtlColor
* Returns:      HBRUSH
*************************************************************/
HBRUSH CPolarityCorrectionDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
* Method:       InitializeControls
* Description:  This function is used
* Returns:      void
*************************************************************/
void CPolarityCorrectionDlg::InitializeControls()
{
    TIPREP_TRACE(CPolarityCorrectionDlg::InitializeControls,MRTraceMarker3);
    m_edit_magnitude.SetFont(_T("Arial"), 10, true);
    m_edit_magnitude.SetReadOnly(TRUE);
    m_magnitude_select_btn.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
    m_exit_polarity_correction.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
    m_polarity_correction_btn.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
    m_static_magnitude.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_static_magnitude.SetBkColor(BKCOLOR);
    m_static_magnitude.SetWindowText(_T("Mg"));
}

/*************************************************************
* Method:       OnMagnitudeSeriesSelectBtn
* Description:  This function is used to display the selected
*               image series when the select button of polarity
*               correction is clicked.
* Returns:      void
*************************************************************/
void CPolarityCorrectionDlg::OnMagnitudeSeriesSelectBtn()
{
    TIPREP_TRACE(CPolarityCorrectionDlg::OnMagnitudeSeriesSelectBtn,MRTraceMarker3);
    CTiPrepView::GetView()->SetIselEvent(EVENT_POLARITY_CORRECTION);
    BOOL l_error = (CTiPrepView::GetView())->GetSelectedImages();
    
    if(TRUE != l_error) {
        CTiPrepLogHelper::WriteToErrorLog(_T("image loid not properly obtained"), __LINE__, __FILE__);
    }
}

/*************************************************************
* Method:       OnPolarityCorrectionBtnClick
* Description:  This function is used
* Returns:      void
*************************************************************/
void CPolarityCorrectionDlg::OnPolarityCorrectionBtnClick()
{
    CTiPrepView::GetView()->SetWaitCursor(true);
    CString l_protocol_value_check = _T("");
    m_edit_magnitude.GetWindowText(l_protocol_value_check);
    BOOL l_protocol_value = l_protocol_value_check.IsEmpty();
    
    if(FALSE != l_protocol_value) {
        
        int l_nMsagCode = I_PROPER_IMG;
        CTiPrepView::GetView()->DisplayErr(l_nMsagCode, POLARITY_CORR_DLG);
        return;
    }
    
    int l_error_status = (CTiPrepController::GetInstance())->PolarityCorrectionCalculation();
    
    if(E_NO_ERROR != l_error_status) {
        CTiPrepView::GetView()->DisplayErr(l_error_status); 
        return;
    }
    CTiPrepView::GetView()->SetWaitCursor(false);
}

/*************************************************************
* Method:       RefreshSelectedSeries
* Description:  This function is used
* Parameter:    CString f_csSeriesNumber
* Returns:      void
*************************************************************/
void CPolarityCorrectionDlg::RefreshSelectedSeries(CString &f_csSeriesNumber)
{
    TIPREP_TRACE(CPolarityCorrectionDlg::RefreshSelectedSeries,MRTraceMarker3);
    m_edit_magnitude.SetWindowText(f_csSeriesNumber);
}


/*************************************************************
* Method:    	OnCancel
* Description: 	This function is used 
* Returns:   	void
*************************************************************/
void CPolarityCorrectionDlg::OnCancel()
{
    CTiPrepView::GetView()->UnRegisterWindowZOrder(m_hWnd);
    CPopupDlg::OnCancel();
}

CPolarityCorrectionDlg::~CPolarityCorrectionDlg()
{
	try{
		m_list_brush.DeleteObject();
	}catch(...)
	{
		CString l_csmsg(_T("Exception occurred in Polarity Correction dialog"));
        CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
		
	}
}

///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////
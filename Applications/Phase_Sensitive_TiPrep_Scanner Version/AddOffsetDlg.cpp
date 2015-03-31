// AddOffsetDlg.cpp : implementation file
//
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/

#include "stdafx.h"
#include "tiprep.h"
#include "AddOffsetDlg.h"
#include "TiPrepTrace.h"
#include "TiPrepLogHelper.h"
#include "TiPrepView.h"
#include "TiPrepController.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const CString EXCEPTION_ADDOFFSETDLG = L"Exception occurred in Add offset dialog";

/////////////////////////////////////////////////////////////////////////////
// CAddOffsetDlg dialog


CAddOffsetDlg::CAddOffsetDlg(CWnd* pParent /*=NULL*/)
: CPopupDlg(CAddOffsetDlg::IDD, pParent),
m_add_offset_check_box_status(0)
{
    //{{AFX_DATA_INIT(CAddOffsetDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_hIcon = AfxGetApp()->LoadIcon(IDI_TIPREP_ICON2); //Inside the dialog's constructor.

}

// copy constructor
CAddOffsetDlg::CAddOffsetDlg(CAddOffsetDlg& f_offsetdlg): CPopupDlg(CAddOffsetDlg::IDD)
{
//     //TIPREP_TRACE(CAddOffsetDlg::CAddOffsetDlg);      
	m_hIcon = f_offsetdlg.m_hIcon;
}

//assignment operator
CAddOffsetDlg& CAddOffsetDlg::operator = (CAddOffsetDlg& f_offsetdlg)
{
//     //TIPREP_TRACE(CAddOffsetDlg::operator =);
//     
    if(this == &f_offsetdlg) {
        return(*this);
    }
	m_hIcon = f_offsetdlg.m_hIcon;
	m_add_offset_check_box_status = f_offsetdlg.m_add_offset_check_box_status;
//     
    return (*this);
}

/*************************************************************
* Method:       DoDataExchange
* Description:  This function is used
* Parameter:    CDataExchange * pDX
* Returns:      void
*************************************************************/
void CAddOffsetDlg::DoDataExchange(CDataExchange* pDX)
{
    CPopupDlg::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAddOffsetDlg)
    DDX_Control(pDX, IDOK, m_exit_add_offset_btn);
    DDX_Control(pDX, IDC_BUTTON_APPLY_OFFSET, m_apply_offset_btn);
    DDX_Control(pDX, IDC_BUTTON_PROT_SELECT, m_protocol_select_btn);
    DDX_Control(pDX, IDC_EDIT_PROTOCOL, m_edit_protocol);
    DDX_Control(pDX, IDC_EDIT_OFFSET, m_edit_offset);
    DDX_Control(pDX, IDC_CHECK_ADD_ROI_AVG, m_add_roi_avg_chkbox);
    DDX_Control(pDX, IDC_STATIC_PROTOCOL, m_label_protocol);
    DDX_Control(pDX, IDC_STATIC_OFFSET, m_label_offset);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddOffsetDlg, CPopupDlg)
//{{AFX_MSG_MAP(CAddOffsetDlg)
ON_BN_CLICKED(IDC_BUTTON_PROT_SELECT, OnButtonProtSelect)
ON_BN_CLICKED(IDC_BUTTON_APPLY_OFFSET, OnButtonApplyOffset)
ON_BN_CLICKED(IDC_CHECK_ADD_ROI_AVG, OnCheckAddRoiAvg)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddOffsetDlg message handlers

/*************************************************************
* Method:       OnButtonProtSelect
* Description:  This function is used
* Returns:      void
*************************************************************/
void CAddOffsetDlg::OnButtonProtSelect()
{
    TIPREP_TRACE(CAddOffsetDlg::OnButtonProtSelect,MRTraceMarker3);
    
    try {
        CTiPrepView::GetView()->SetIselEvent(EVENT_ADD_OFFSET);
        BOOL l_error = (CTiPrepView::GetView())->GetSelectedImages();
        
        if(TRUE != l_error) {
            CTiPrepLogHelper::WriteToErrorLog(_T("Error in messaging with iSelector"), __LINE__, __FILE__);
            return;
        }
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_ADDOFFSETDLG, __LINE__, __FILE__);
        return;
    }
}

/*************************************************************
* Method:       OnButtonApplyOffset
* Description:  This function is used
* Returns:      void
*************************************************************/
void CAddOffsetDlg::OnButtonApplyOffset()
{
    CTiPrepView::GetView()->SetWaitCursor(true);
    try {
        OnCheckAddRoiAvg();
        //create new series after adding offset
        //add series to study
        CString l_offset_value = _T("--");
        CString l_protocol_value_check = _T("");
        m_edit_offset.GetWindowText(l_offset_value);
        BOOL l_empty_string_status = l_offset_value.IsEmpty();
        m_edit_protocol.GetWindowText(l_protocol_value_check);
        BOOL l_protocol_value = l_protocol_value_check.IsEmpty();
        
        if(FALSE != l_protocol_value) {
            int l_nMsagCode = I_PROPER_IMG;
            CTiPrepView::GetView()->DisplayErr(l_nMsagCode, ADD_OFFSET_DLG);
            return;
        }
        
        char l_string_in_char[128] = {'\0'};
        wcstombs(l_string_in_char, l_offset_value, 128);
        float l_compare_value =atof(l_string_in_char);
        if( l_compare_value>500000.0f) {
            l_offset_value =_T("500000");
        }
        
        for(int l_string_parsing = 0; 
        l_string_parsing < (int)strlen(l_string_in_char); 
        l_string_parsing++) {
            if(isdigit(l_string_in_char[l_string_parsing]) == 0) {
                int l_nMsagCode = I_VAL_OFFSET;
                CTiPrepView::GetView()->DisplayErr(l_nMsagCode, ADD_OFFSET_DLG);
                m_edit_offset.SetWindowText(_T("--"));
                return;
            }
        }
        
		if(TRUE == l_empty_string_status || l_offset_value.CompareNoCase(_T("0")) == 0 
			||l_compare_value <0.0f/*|| l_compare_value>10000.0f*/) {
            int l_nMsagCode = I_VAL_OFFSET;
            CTiPrepView::GetView()->DisplayErr(l_nMsagCode, ADD_OFFSET_DLG);
            m_edit_offset.SetWindowText(_T("--"));
            return;
        }
        int l_actual_expected_offset_value =0;
        int l_error_status = 
            (CTiPrepController::GetInstance())->AddOffsetCalculation
            (l_offset_value, m_add_offset_check_box_status,&l_actual_expected_offset_value);
        CString l_correct_offset_value = _T("");
        l_correct_offset_value.Format(_T("%d"),l_actual_expected_offset_value);
        m_edit_offset.SetWindowText(l_correct_offset_value);
        CTiPrepView::GetView()->SetWaitCursor(false);
        
        if(E_NO_ERROR != l_error_status && E_OFFSET_RANGE_ERROR != l_error_status) {
            CTiPrepView::GetView()->DisplayErr(l_error_status,ADD_OFFSET_DLG);
            return;
        } else if (E_OFFSET_RANGE_ERROR == l_error_status){
            CString l_error_msg =_T("");
            l_error_msg.Format(_T("Please enter offset within range 0 to %d"),l_actual_expected_offset_value);         
            CErrorDlg l_error_dlg(this);
            l_error_dlg.SetErrorMessage(l_error_msg);
            l_error_dlg.DoModal();
            return;
        } else {
        }
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_ADDOFFSETDLG, __LINE__, __FILE__);
        CTiPrepView::GetView()->SetWaitCursor(false);
        return;
    }
    //CTiPrepView::GetView()->SetWaitCursor(false);
}

/*************************************************************
* Method:       OnCheckAddRoiAvg
* Description:  This function is used
* Returns:      void
*************************************************************/
void CAddOffsetDlg::OnCheckAddRoiAvg()
{
    int l_add_offset_check_box_status = 0;
    l_add_offset_check_box_status = m_add_roi_avg_chkbox.GetCheck();
    m_add_offset_check_box_status = l_add_offset_check_box_status;
}

/*************************************************************
* Method:       OnOK
* Description:  This function is used
* Returns:      void
*************************************************************/
void CAddOffsetDlg::OnOK()
{
    try {
        CTiPrepView::GetView()->UnRegisterWindowZOrder(m_hWnd);
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_ADDOFFSETDLG, __LINE__, __FILE__);
        return;
    }
    
    CPopupDlg::OnOK();
}

/*************************************************************
* Method:       OnInitDialog
* Description:  This function is used
* Returns:      BOOL
*************************************************************/
BOOL CAddOffsetDlg::OnInitDialog()
{
    CPopupDlg::OnInitDialog();
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

	WINDOW_COLORS_t l_window_color;
    GetWindowColors(l_window_color);
    m_list_brush.CreateSolidBrush(l_window_color.crClientColor);

    try {
        InitializeAddOffsetDlgControls();
        CTiPrepView::GetView()->AddZOrderLayer();
        CTiPrepView::GetView()->RegisterWindowZOrder(m_hWnd);
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_ADDOFFSETDLG, __LINE__, __FILE__);
        return FALSE;
    }
    
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// Member Functions
///////////////////

/*************************************************************
* Method:       RefreshSelectedSeries
* Description:  This function is used
* Parameter:    CString f_csSeriesNumber
* Returns:      void
*************************************************************/
void CAddOffsetDlg::RefreshSelectedSeries(CString &f_csSeriesNumber)
{
    m_edit_protocol.SetWindowText(f_csSeriesNumber);
}

/*************************************************************
* Method:       InitializeAddOffsetDllgControls
* Description:  This function is used
* Returns:      void
*************************************************************/
void CAddOffsetDlg::InitializeAddOffsetDlgControls()
{
    TIPREP_TRACE(CAddOffsetDlg::InitializeAddOffsetDlgControls,MRTraceMarker9);
    SetRibbonHeight(0);
    m_edit_protocol.SetFont(_T("Arial"), 10, true);
    m_edit_protocol.SetReadOnly(TRUE);
    m_edit_offset.SetFont(_T("Arial"), 10, true);
    m_label_protocol.SetFont(_T("Arial"), 10, true);
    m_label_offset.SetFont(_T("Arial"), 10, true);
    m_label_protocol.SetReadOnly();
    m_label_offset.SetReadOnly();
    m_label_protocol.SetBkColor(BKCOLOR);
    m_label_offset.SetBkColor(BKCOLOR);
    m_apply_offset_btn.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
    m_protocol_select_btn.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
    m_exit_add_offset_btn.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
    m_add_roi_avg_chkbox.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_add_roi_avg_chkbox.SetBkColor(BKCOLOR);
}

/*************************************************************
* Method:    	OnCancel
* Description: 	This function is used 
* Returns:   	void
*************************************************************/
void CAddOffsetDlg::OnCancel()
{
    TIPREP_TRACE(CAddOffsetDlg::OnCancel,MRTraceMarker3);
    
    try {
        CTiPrepView::GetView()->UnRegisterWindowZOrder(m_hWnd);
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_ADDOFFSETDLG, __LINE__, __FILE__);
    }
    
    CPopupDlg::OnCancel();
}

/*************************************************************
* Method:       OnCtlColor
* Description:  This function is used
* Parameter:    CDC * pDC
* Parameter:    CWnd * pWnd
* Parameter:    UINT nCtlColor
* Returns:      HBRUSH
*************************************************************/
HBRUSH CAddOffsetDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    try {
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
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_ADDOFFSETDLG, __LINE__, __FILE__);
        return NULL;
    }
}

CAddOffsetDlg::~CAddOffsetDlg()
{
	try{
		m_list_brush.DeleteObject();
	}catch(...)
	{
		CString l_csmsg(_T("Exception occurred in Add Offset dialog"));
        CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
		
	}
}


///////////////////////////////////////////////////////////////////////////////
// End of file
//////////////

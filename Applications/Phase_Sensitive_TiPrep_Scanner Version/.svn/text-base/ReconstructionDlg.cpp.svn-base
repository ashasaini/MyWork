/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#include "stdafx.h"
#include "tiprep.h"
#include "ReconstructionDlg.h"

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
//Method Name   : CReconstructionDlg
//Purpose       : Constructor
//********************************************************************
/**
*   A constructor.
*   This function is used for initialization of class variables.
*/
CReconstructionDlg::CReconstructionDlg(CWnd* pParent /*=NULL*/)
: CPopupDlg(CReconstructionDlg::IDD, pParent)
{
    /*TIPREP_TRACE(CReconstructionDlg::CReconstructionDlg);*/
    //{{AFX_DATA_INIT(CReconstructionDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_hIcon = AfxGetApp()->LoadIcon(IDI_TIPREP_ICON2); //Inside the dialog's constructor.
}

CReconstructionDlg& CReconstructionDlg::operator = (CReconstructionDlg& f_Rcondlg_ptr)
{
    /*TIPREP_TRACE(CReconstructionDlg::operator);*/
    
    if(this == &f_Rcondlg_ptr) {
        return(*this);
    }
    m_hIcon = AfxGetApp()->LoadIcon(IDI_TIPREP_ICON2);
    return (*this);
}
CReconstructionDlg::CReconstructionDlg(CReconstructionDlg & f_recondlg) : CPopupDlg(CReconstructionDlg::IDD)
{
  m_hIcon = f_recondlg.m_hIcon;
}
//****************************Method Header***************************
//Method Name   : CReconstructionDlg
//Purpose       : Destructor
//********************************************************************
//Add destructor


/*************************************************************
* Method:       DoDataExchange
* Description:  This function is used
* Parameter:    CDataExchange * pDX
* Returns:      void
*************************************************************/
void CReconstructionDlg::DoDataExchange(CDataExchange* pDX)
{
    /*TIPREP_TRACE(CReconstructionDlg::DoDataExchange);*/
    CPopupDlg::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CReconstructionDlg)
    DDX_Control(pDX, IDC_STATIC_PROT_FOR_RECON, m_static_recon_prot);
    DDX_Control(pDX, IDC_COMBO_DATA_FORM, m_combo_data_form);
    DDX_Control(pDX, IDCANCEL, m_button_exit);
    DDX_Control(pDX, IDC_BUTTON_SELECT, m_button_select);
    DDX_Control(pDX, IDC_BUTTON_RECONSTRUCTION, m_button_reconstruction);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReconstructionDlg, CPopupDlg)
//{{AFX_MSG_MAP(CReconstructionDlg)
ON_WM_PAINT()
ON_WM_CTLCOLOR()
ON_BN_CLICKED(IDC_BUTTON_RECONSTRUCTION, OnButtonReconstruction)
ON_BN_CLICKED(IDC_BUTTON_SELECT, OnButtonSelectRecon)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReconstructionDlg message handlers

/*************************************************************
* Method:       OnInitDialog
* Description:  This function is used
* Returns:      BOOL
*************************************************************/
BOOL CReconstructionDlg::OnInitDialog()
{
    TIPREP_TRACE(CReconstructionDlg::OnInitDialog,MRTraceMarker3);
    CPopupDlg::OnInitDialog();
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    // TODO: Add extra initialization here
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
* Method:    	OnCtlColor
* Description: 	This function is used 
* Parameter: 	CDC * pDC :
* Parameter: 	CWnd * pWnd :
* Parameter: 	UINT nCtlColor :
* Returns:   	HBRUSH
*************************************************************/
HBRUSH CReconstructionDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CPopupDlg::OnCtlColor(pDC, pWnd, nCtlColor);
    
    // TODO: Change any attributes of the DC here
    if(nCtlColor == CTLCOLOR_STATIC) {
        pDC->SetBkColor(BKCOLOR);
        pDC->SetTextColor(TEXT_BKCOLOR);
        pDC->SetBkMode(OPAQUE);
        return (HBRUSH) m_list_brush;
    } else {
        //
    }
    
    return hbr;
}
/*************************************************************
* Method:       OnButtonReconstruction
* Description:  This function is used
* Returns:      void
*************************************************************/
void CReconstructionDlg::OnButtonReconstruction()
{
    try{
        int l_nDataForm = m_combo_data_form.GetCurSel();
        CString l_protocol_value_check = _T("");
        m_static_recon_prot.GetWindowText(l_protocol_value_check);
        if (l_protocol_value_check.IsEmpty())
        {
            int err_code = I_PROPER_IMG;
            CTiPrepView::GetView()->DisplayErr(err_code,RECON_DLG);
            return;
            
        }
        AfxGetApp()->DoWaitCursor(1);
        m_button_reconstruction.EnableWindow(FALSE);

        int l_nStatus = CTiPrepController::GetInstance()->doRawChkAndRecon(l_nDataForm);
        
        if(E_NO_ERROR != l_nStatus) {
            CTiPrepView::GetView()->DisplayErr(l_nStatus);
            m_button_reconstruction.EnableWindow(TRUE);
            AfxGetApp()->DoWaitCursor(-1);
            return;
        }
        m_button_reconstruction.EnableWindow(TRUE);
        AfxGetApp()->DoWaitCursor(-1);
    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog
            (_T("Exception occurred in Reconstruction Class"), __LINE__, __FILE__);
    }
}

/*************************************************************
* Method:    	OnButtonSelectRecon
* Description: 	This function is used 
* Returns:   	void
*************************************************************/
void CReconstructionDlg::OnButtonSelectRecon()
{
    try{
        CTiPrepView::GetView()->SetIselEvent(EVENT_RECON_IMG);
        CTiPrepView::GetView()->SetWaitCursor(true);
        BOOL l_bselImg = CTiPrepView::GetView()->GetSelectedImages();

        if(FALSE == l_bselImg){
            (CTiPrepView::GetView())->SetWaitCursor(false);
        }

    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog
            (_T("Exception occurred in Reconstruction Class"), __LINE__, __FILE__);
    }
}

/*************************************************************
* Method:    	OnCancel
* Description: 	This function is used 
* Returns:   	void
*************************************************************/
void  CReconstructionDlg::OnCancel()
{
    CTiPrepView::GetView()->UnRegisterWindowZOrder(m_hWnd);
    CPopupDlg::OnCancel();
}
///////////////////////////////////////////////////////////////////////////////
// Member Functions
///////////////////


/*************************************************************
* Method:       InitializeControls
* Description:  This function is used to initialize dialog controls.
* Returns:      void
*************************************************************/
void CReconstructionDlg::InitializeControls()
{
    TIPREP_TRACE(CReconstructionDlg::InitializeControls,MRTraceMarker3);

    SetBkMode((m_static_recon_prot.GetDC())->GetSafeHdc(),TRANSPARENT);
    m_static_recon_prot.SetFont(&m_font, true);

    m_button_select.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
    m_button_exit.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
    m_button_reconstruction.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
    m_combo_data_form.SetCurSel(0);
    m_combo_data_form.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_combo_data_form.SetBkColor(BKCOLOR);
}

/*************************************************************
* Method:    	setSeriseName
* Description: 	This function is used 
* Parameter: 	const CString & f_csSeriseName :
* Returns:   	void
*************************************************************/
void CReconstructionDlg::setSeriseName(const CString &f_csSeriseName)
{
    m_static_recon_prot.SetWindowText(f_csSeriseName);
}

///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////

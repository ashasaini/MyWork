/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/

#include "stdafx.h"
#include "tiprep.h"
#include "ErrorDlg.h"

#include "TiPrepTrace.h"
#include "TiPrepLogHelper.h"
#include "TiPrepErrors.h"
#include "TiPrepView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const CString EXCEPTION_ERRORDLG = L"Exception occurred in Error Dialog";

/**
*   A constructor.
*   This function is used for initialization of class variables.
*/
CErrorDlg::CErrorDlg(CWnd* pParent /*=NULL*/)
: CPopupDlg(CErrorDlg::IDD, pParent),m_csTitle(_T("")) 
{
    /*TIPREP_TRACE(CErrorDlg::CErrorDlg);*/
    m_baseHwnd = NULL;
    //{{AFX_DATA_INIT(CErrorDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_hIcon = AfxGetApp()->LoadIcon(IDI_TIPREP_ICON2); //Inside the dialog's constructor.
}

// copy constructor
// CErrorDlg::CErrorDlg(CErrorDlg& f_ErrorDlg): CPopupDlg(CErrorDlg::IDD)
// {
//     //TIPREP_TRACE(CErrorDlg::CErrorDlg);  
// }

//assignment operator
CErrorDlg& CErrorDlg::operator = (CErrorDlg& f_ErrorDlg)
{
//     //TIPREP_TRACE(CErrorDlg::operator =);
//     
    if(this == &f_ErrorDlg) {
        return(*this);
    } 
	m_error_msg = f_ErrorDlg.m_error_msg;
	m_csTitle = f_ErrorDlg.m_csTitle;
// 
    return (*this);
}

/*************************************************************
* Method:       DoDataExchange
* Description:  This function is used
* Parameter:    CDataExchange * pDX
* Returns:      void
*************************************************************/
void CErrorDlg::DoDataExchange(CDataExchange* pDX)
{
    /*TIPREP_TRACE(CErrorDlg::DoDataExchange);*/
    CPopupDlg::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CErrorDlg)
    DDX_Control(pDX, IDC_STATIC_ERROR, m_errorctrl);
    DDX_Control(pDX, IDOK, m_buttonOk);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CErrorDlg, CPopupDlg)
//{{AFX_MSG_MAP(CErrorDlg)
ON_WM_SHOWWINDOW()
ON_WM_CTLCOLOR()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CErrorDlg message handlers
/////////////////////////////


/*************************************************************
* Method:       OnInitDialog
* Description:  This function is used
* Returns:      BOOL
*************************************************************/
BOOL CErrorDlg::OnInitDialog()
{
    TIPREP_TRACE(CErrorDlg::OnInitDialog,MRTraceMarker9);
    try{
        CPopupDlg::OnInitDialog();
        CFont l_lableFont;
//        CSize bitmapSize;
        WINDOW_COLORS_t l_window_color;
        GetWindowColors(l_window_color);
        m_list_brush.CreateSolidBrush(l_window_color.crClientColor);
        SetRibbonHeight(0);
        m_buttonOk.SetPrimaryTextFont(_T("Arial"), 12, true);
        m_errorctrl.SetFont(&l_lableFont, true);
        m_errorctrl.SetWindowText(m_error_msg);
        if(!(m_csTitle.IsEmpty())){
            SetWindowText(m_csTitle);
        }
        HWND hwndOwner;
        
        if(m_baseHwnd == NULL) {
            hwndOwner = GetParent()->GetSafeHwnd();
            if(NULL == hwndOwner) {
                hwndOwner = GetDesktopWindow()->GetSafeHwnd();
            }
        } else {
            hwndOwner = m_baseHwnd;
        }
        
        CRect rc, rcDlg, rcOwner;
        ::GetWindowRect(hwndOwner, &rcOwner);
        GetWindowRect(&rcDlg);
        CopyRect(&rc, &rcOwner);
        OffsetRect(&rcDlg, -rcDlg.left, -rcDlg.top);
        OffsetRect(&rc, -rc.left, -rc.top);
        OffsetRect(&rc, -rcDlg.right, -rcDlg.bottom);
        MoveWindow(rcOwner.left + (rc.right / 2),
            rcOwner.top + (rc.bottom / 2), rcDlg.Width(), rcDlg.Height());
        CTiPrepView::GetView()->AddZOrderLayer();
        
        CTiPrepView::GetView()->RegisterWindowZOrder(m_hWnd);
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_ERRORDLG, __LINE__, __FILE__);
    }
    
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
HBRUSH CErrorDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    /*TIPREP_TRACE(CErrorDlg::OnCtlColor,MRTraceMarker9);*/
    HBRUSH hbr = DialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
    
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
* Method:    	OnShowWindow
* Description: 	This function is used 
* Parameter: 	BOOL bShow :
* Parameter: 	UINT nStatus :
* Returns:   	void
*************************************************************/
void CErrorDlg::OnShowWindow(/*BOOL bShow, UINT nStatus*/)
{
    SetFocus();
}


///////////////////////////////////////////////////////////////////////////////
// Member Functions
///////////////////


/*************************************************************
* Method:       SetErrorMessage
* Description:  This is setter method for m_error_msg.
* Parameter:    CString f_csError
*               Contains error message to display on dialog.
* Returns:      void
*************************************************************/
void CErrorDlg::SetErrorMessage(CString& f_csError)
{
    TIPREP_TRACE(CErrorDlg::SetErrorMessage,MRTraceMarker3);
    m_error_msg = f_csError;
}
/*************************************************************
* Method:       SetDlgTitle
* Description:  This is setter method for dialog title.
* Parameter:    CString f_csTitle
*               Contains title of the dialog.
* Returns:      void
*************************************************************/
void CErrorDlg::SetDlgTitle(CString& f_csTitle)
{
    TIPREP_TRACE(CErrorDlg::SetDlgTitle,MRTraceMarker3);
    m_csTitle = f_csTitle;
}

/*************************************************************
* Method:    	SetErrorValue
* Description: 	This function is used 
* Parameter: 	int ErrorValue :
* Returns:   	void
*************************************************************/
void CErrorDlg::SetErrorValue(int ErrorValue)
{
    TIPREP_TRACE(CErrorDlg::SetErrorValue,MRTraceMarker3);
    if (ErrorValue > I_NO_ERROR && ErrorValue < E_LAST_VAL)
    {
        if (ErrorValue == E_MEM_ALLOC ||
            ErrorValue == E_ERROR_NULL_FUNCPARAMS ||
            ErrorValue == E_ERROR ||
            ErrorValue == E_ERROR_NULL_FUNCPARAMS ||
            ErrorValue == E_ERR_EXCEPTION)
        {
            m_error_msg = ERRORSTRING[E_COMON_ERROR];
        } else {
            m_error_msg = ERRORSTRING[ErrorValue];
        }
    } else {
        m_error_msg = ERRORSTRING[E_COMON_ERROR];
    }
}

/*************************************************************
* Method:    	SetHWnd
* Description: 	This function is used 
* Parameter: 	HWND h :
* Returns:   	void
*************************************************************/
void CErrorDlg::SetHWnd(HWND h)
{
    m_baseHwnd = h;
}

/*************************************************************
* Method:    	OnCancel
* Description: 	This function is used 
* Returns:   	void
*************************************************************/
void CErrorDlg::OnCancel()
{
    CTiPrepView::GetView()->UnRegisterWindowZOrder(m_hWnd);
    CPopupDlg::OnCancel();
}
/*************************************************************
* Method:    	OnOK
* Description: 	This function is used 
* Returns:   	void
*************************************************************/
void CErrorDlg::OnOK()
{
    CTiPrepView::GetView()->UnRegisterWindowZOrder(m_hWnd);
    CPopupDlg::OnOK();
}
/*************************************************************
* Method:    	ShowErrorMessage
* Description: 	This function is used 
* Returns:   	void
*************************************************************/
void CErrorDlg::ShowErrorMessage()
{
    if (m_error_msg.IsEmpty())
        return;
    CString lcsmsg;
    lcsmsg = ERRORSTRING[E_COMON_ERROR];
    if (m_error_msg.CompareNoCase(lcsmsg) == 0)
    {
        CTiPrepLogHelper::WriteToErrorLog(m_error_msg,0,_T(""),ERRORVIEW_DISPLAY); 
    } else {
        DoModal();
    }
}

CErrorDlg::~CErrorDlg()
{
	try{
		m_list_brush.DeleteObject();
	}catch(...)
	{
		CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_ERRORDLG, __LINE__, __FILE__);
		
	}
}

///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////

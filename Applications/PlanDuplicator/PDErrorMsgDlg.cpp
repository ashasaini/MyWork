// PDErrorMsgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "planduplicator.h"
#include "PDErrorMsgDlg.h"
#include "dataTypes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPDErrorMsgDlg dialog

CPDErrorMsgDlg::CPDErrorMsgDlg(CWnd* pParent /*=NULL*/)
    : DialogEx(CPDErrorMsgDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CPDErrorMsgDlg)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}

void CPDErrorMsgDlg::DoDataExchange(CDataExchange* pDX)
{
    DialogEx::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPDErrorMsgDlg)
    DDX_Control(pDX, IDOK, m_buttonOk);
    DDX_Control(pDX, IDC_STATIC_ERROR, m_errorctrl);
        // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPDErrorMsgDlg, DialogEx)
    //{{AFX_MSG_MAP(CPDErrorMsgDlg)
        // NOTE: the ClassWizard will add message map macros here
        ON_WM_CTLCOLOR()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPDErrorMsgDlg message handlers

BOOL CPDErrorMsgDlg::OnInitDialog()
{
    DialogEx::OnInitDialog();

    CFont l_lableFont;
    CSize bitmapSize;
    WINDOW_COLORS_t l_window_color;
    GetWindowColors(l_window_color);
    m_list_brush.CreateSolidBrush(l_window_color.crClientColor);
    SetRibbonHeight( 0 );
    m_buttonOk.SetPrimaryTextFont(_T("Arial"), FONT_SIZE_12, TRUE);
    m_buttonOk.SetButtonType(TUSBUTTON_ROUND_ALL);
    CEdit* l_pName = (CEdit*)GetDlgItem(IDC_STATIC_ERROR);  
    m_errorctrl.SetFont(&l_lableFont, TRUE);
    m_errorctrl.SetWindowText(m_error_msg);
    return(TRUE);  
}

/**
* This is setter method for m_error_msg.
* @param f_csError  Contains error message to display on dialog.
*/
void CPDErrorMsgDlg::SetErrorMessage(CString &f_csError)
{
    m_error_msg = f_csError;
}

/**
*   A constructor.
*   This function is used to set color of dialog.
*/
HBRUSH CPDErrorMsgDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = DialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    if(CTLCOLOR_STATIC == nCtlColor){
        pDC->SetBkColor(BKCOLOR);
        pDC->SetTextColor(TEXT_BKCOLOR);
        pDC->SetBkMode(OPAQUE);
        return(m_list_brush);
    }else{
        //
    }

    return(hbr);
}

////////////////////////    End Of File     /////////////////////////////////

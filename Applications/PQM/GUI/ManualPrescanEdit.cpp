//TMSC-REDMINE-781
// ManualPrescanEdit.cpp : implementation file
//

#include "stdafx.h"
#include "ManualPrescanEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(CManualPrescanEdit, EditCtrlEx)
    //{{AFX_MSG_MAP(CManualPrescanEdit)
    ON_WM_CHAR()
    ON_WM_CONTEXTMENU()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CManualPrescanEdit

CManualPrescanEdit::CManualPrescanEdit()
{
}

CManualPrescanEdit::~CManualPrescanEdit()
{
}

// CManualPrescanEdit message handlers
//****************************Method Header************************************
//Module         : OnContextMenu
//Author         : IGATE/SS
//Purpose        : only numeric data accepted
//*****************************************************************************

void CManualPrescanEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // TODO: Add your message handler code here and/or call default

    if ((nChar >= '0' && nChar <= '9') ||
        (nChar == VK_BACK) ||
        (nChar == '.')
       ) {

        CString current_string(_T("")) ;
        GetWindowText(current_string);

        if (current_string.Find(_T(".")) != -1) {

            //dot is already present in string..
            int nStartChar = 0, nEndChar = 0;	//Patni-PP/2009May28/Modified/SU14 CPP test initialization corrections
            GetSel(nStartChar, nEndChar);

            if (nStartChar == nEndChar) {
                //there is no selected string..and dot is present in current string..

                if (nChar == '.')
                    return ;

            } else {

                CString selected_string = current_string.Mid(nStartChar, nEndChar);

                if (selected_string.Find(_T(".")) == -1) {
                    //dot is not present in selected string..

                    if (nChar == '.') {
                        //if input char is decimal, make wParam NULL..
                        return ;
                    }
                }
            }
        }

        EditCtrlEx::OnChar(nChar, nRepCnt, nFlags);
    }
}

//****************************Method Header************************************
//Module         : OnContextMenu
//Author         : IGATE/SS
//Purpose        : Right mouse button pressed
//*****************************************************************************
void CManualPrescanEdit::OnContextMenu(CWnd* pNewWnd, CPoint point)
{
}

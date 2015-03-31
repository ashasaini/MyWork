//TMSC-REDMINE-781
//TMSC-Redmine-780
//TMSC-REDMINE-777
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMEdit.cpp
 *  Overview: Implementation of CPQMEdit class.
 *  Last Modified: 2007/8/10 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2007/6/15 12:00:00     PQM5 Baselined code
 *   2.0          Patni            2007/8/10 12:00:00     Implemented PQM6 requirements & Formatted
 *
 *****************************************************************************/
#include "stdafx.h"
#include "PQMEdit.h"
#include "PQMView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COLOR_EDITBOX_UPPER_BORDER      RGB(172,168,153)
#define COLOR_EDITBOX_LOWER_BORDER      RGB(255,255,255)
#define EDITBOX_TEXT_COLOR              RGB(255, 255, 255)
#define EDITBOX_BACKGROUND_COLOR        RGB(0, 0, 0)
#define EDITBOX_FOCUS_BORDER_COLOR      RGB(255, 255, 255)
#define EDITBOX_NOT_FOCUS_BORDER_COLOR  RGB(0, 0, 0)


BEGIN_MESSAGE_MAP(CPQMEdit, EditCtrlEx)
    //{{AFX_MSG_MAP(CPQMEdit)
    ON_WM_CHAR()
    ON_WM_RBUTTONDOWN()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


//************************************Method Header**************************************

//Method Name:  CPQMEdit

//Author:  PATNI/SVP

//Purpose: CPQMEdit.

//**************************************************************************************/
CPQMEdit::CPQMEdit(
)
{
}


//************************************Method Header**************************************
//Method Name:  CPQMEdit

//Author:  PATNI/SVP

//Purpose: Destructor.

//**************************************************************************************/
CPQMEdit::~CPQMEdit(
)
{
}


//************************************Method Header**************************************

//Method Name:  ValidateStringWithDotAndMinuschar

//Author:  PATNI/SVP

//Purpose: To validate str with '.' & '-' chars.

//**************************************************************************************/
bool CPQMEdit::ValidateStringWithDotAndMinusChars(
    const CString& str,
    const int valid_dot_pos
) const
{
    const int len = str.GetLength();

    if (len == 0) {
        return false;
    }

    int count_dot = 0;
    int pos_dot = 0;
    int count_minus = 0;
    int pos_minus = 0;

    for (int i = 0; i < len; i++) {

        if (str[i] == '-') {

            pos_minus = i;

            if ((len == 1) ||       //Length is one & first char is '-'
                (pos_minus > 0) ||  //Position of '-' not at zeroth
                (++count_minus > 1) //Number of '-' chars more than one
               ) {

                return false;
            }

        } else if (str[i] == '.') {

            pos_dot = i;

            if ((len == 1) ||        //Length is one & first char is '.'
                //   (pos_dot> valid_dot_pos) ||      //Position of '.' is next to 2 nd
                (++count_dot > 1)) { //Number of '.' chars more than one

                return false;
            }
        }
    }

    return true;
}


//************************************Method Header**************************************

//Method Name:  OnChar

//Author:  PATNI/SVP

//Purpose: To validate characters entered.

//**************************************************************************************/
void CPQMEdit::OnChar(
    UINT chara,
    UINT rep_cnt,
    UINT flags
)
{

    int ctrl_id = GetDlgCtrlID();

    if (ctrl_id == IDC_EDIT_ID) { //This is for the ID edit box on comment dlg
        if (__iscsym(chara) ||
            (chara == VK_BACK) ||
            (chara == ' ')) {

            EditCtrlEx::OnChar(chara, rep_cnt, flags);
        }

    } else if ((ctrl_id == IDC_EDIT_COMMENT) || (ctrl_id == IDC_PASSWD)) { // the function is for comment edit box on the comment dlg.

        // if (__iscsym(chara) ||
        //   (chara == VK_BACK) ||
        //   (chara == ' ')) { // the function is used for checking the alphabets and underscore

        EditCtrlEx::OnChar(chara, rep_cnt, flags);

        //}

    } else if ((::GetKeyState(VK_CONTROL) < 0) ||   // The function is used for checking the alphabets, numbers and underscore
               (chara >= '0' && chara <= '9') ||
               (chara == VK_BACK) ||
               (chara == '.') ||
               (chara == '-')) {


        EditCtrlEx::OnChar(chara, rep_cnt, flags);

    }

    if (chara == VK_RETURN) {
        if (ctrl_id == IDC_EDIT_CFA) { // Handling enter on Adjust edit box

            CPQMView* p_parent = (CPQMView*)GetParent();
            ASSERT(p_parent != NULL);

            if (p_parent) {
                p_parent->DisplayOffset();
                EditCtrlEx::OnChar(chara, rep_cnt, flags);
            }
        }
    }
}




//****************************Method Header************************************
//Module         : OnRButtonDown
//Author         : PATNI/MN
//Purpose        : Right mouse button pressed
//*****************************************************************************
void CPQMEdit::OnRButtonDown(
    UINT nFlags,
    CPoint point
)
{

    //we are restricting right click and hence right click menu display
    //  CEdit::OnRButtonDown(nFlags, point);
}
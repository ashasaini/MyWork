/*****************************************************************************
 *
 *  (c) Copyright 2011 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PasswordDlg.cpp
 *  Overview:
 *  Created: 2011/3/09 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni/SUDHIR     2011/3/09 12:00:00     Created
 *
 *
 *****************************************************************************/

#include "stdafx.h"
#include "PasswordDlg.h"
#include <libStudyManager/UI/ZOrderLayerFactory.h>
#include "DPSManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using MR::ACQ::SM::COMMON::UI::CZOrderDialog;
using MR::ACQ::SM::COMMON::UI::CZOrderLayerFactory;

const int RIBBON_GAP = 5;

const int MAX_ATTEMPT = 3;
const int MAX_SIZE = 256;

/////////////////////////////////////////////////////////////////////////////
// CPasswordDlg dialog

BEGIN_MESSAGE_MAP(CPasswordDlg, CZOrderDialog)
    //{{AFX_MSG_MAP(CPasswordDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

CPasswordDlg::CPasswordDlg(CWnd* pParent /*=NULL*/)
    : CZOrderDialog(CZOrderLayerFactory::GetIselLayer(),
                    CDPSManager::GetInstance(), IDD_PASSWORD_DLG, pParent),
    m_totalattempts(0),
    m_pFilepath("\\PQM\\AppliPas\\AppliPAS.bin")
{
    //{{AFX_DATA_INIT(CPasswordDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}
//****************************Method Header********************
//Method Name   : OpenPasswordDlg
//Author        : PATNI/SUDHIR
//Purpose       : Opens Password Dialog
//*************************************************************
bool CPasswordDlg::OpenPasswordDlg()
{

    return (IDCANCEL != DoModal()) ;
}

//******************************************************************************
// Function Name    : OnOK
// Author           : PATNI/SUDHIR
// Purpose          : Perform on click OK
//******************************************************************************
void CPasswordDlg::OnCancel()
{
    //+Patni-Hemant/2011Apr20/Added/IR185 A-PAS re-open
    if (m_nFlags & (WF_MODALLOOP | WF_CONTINUEMODAL)) {
        EndModalLoop(IDCANCEL);
    }

    //DialogEx::OnCancel();
    //-Patni-Hemant/2011Apr20/Added/IR185 A-PAS re-open
}

void CPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
    CZOrderDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPasswordDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    DDX_Control(pDX, IDCANCEL, m_cancel_button);
    DDX_Control(pDX, IDC_PWD_EDIT, m_pwd_edit);
    DDX_Control(pDX, IDC_ENTER_PWD_STATIC, m_pwd_static);
    DDX_Control(pDX, IDC_ERR_MSG_STATIC, m_Err_msg_static);
    DDX_Text(pDX, IDC_PWD_EDIT, m_pwd_val);
    //}}AFX_DATA_MAP
}


/////////////////////////////////////////////////////////////////////////////
// CPasswordDlg message handlers

//****************************Method Header********************
//Method Name   : OnInitDialogImpl
//Author        : PATNI/SUDHIR
//Purpose       : Loads the string for Password Dialog
//*************************************************************
BOOL CPasswordDlg::OnInitDialogImpl()
{


    m_totalattempts = 0;

    SetMultilingual();
    SetButtonAppearence();
    SetDialogRibbonHeight();

    return CZOrderDialog::OnInitDialogImpl();
}


//******************************************************************************
// Function Name    : PreTranslateMessage
// Author           : PATNI/SUDHIR
// Purpose          :
//******************************************************************************
BOOL CPasswordDlg::PreTranslateMessageImpl(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class


    if ((WM_KEYDOWN == pMsg->message) && (VK_RETURN == pMsg->wParam)) {
        if (pMsg->hwnd == m_pwd_edit.m_hWnd) {

            checkPWD();

            pMsg->wParam = NULL;
            return TRUE;
        }
    }

    //+Patni-Hemant/2011Apr20/Added/IR185 A-PAS re-open
    if (pMsg && pMsg->message == WM_RBUTTONDOWN) {
        return TRUE;
    }

    //-Patni-Hemant/2011Apr20/Added/IR185 A-PAS re-open

    return CZOrderDialog::PreTranslateMessageImpl(pMsg);
}
//******************************************************************************
// Function Name    : checkPWD
// Author           : PATNI/SUDHIR
// Purpose          : Perform on click OK
//******************************************************************************
void CPasswordDlg::checkPWD()
{
    // TODO: Add extra validation here
    UpdateData(TRUE);//)

    if (validatePassword()) { //!m_pwd_val.CompareNoCase(_T("Sudhir")))
        m_Err_msg_static.ShowWindow(SW_HIDE);

        //+Patni-Hemant/2011Apr20/Added/IR185 A-PAS re-open
        if (m_nFlags & (WF_MODALLOOP | WF_CONTINUEMODAL)) {
            EndModalLoop(IDOK);
        }

        //DialogEx::OnOK();
        //-Patni-Hemant/2011Apr20/Added/IR185 A-PAS re-open

    } else {
        m_totalattempts++;

        if (m_totalattempts == MAX_ATTEMPT) {
            OnCancel();

        } else {

            m_Err_msg_static.ShowWindow(SW_SHOW);

            m_pwd_edit.SetSel(0, -1);
            m_pwd_edit.Clear();

            //m_pwd_edit.SetFocus();

            return;
        }
    }

    UpdateData(FALSE);

    //DialogEx::OnOK();
}

//****************************Method Header********************
//Method Name   : validatePassword
//Author        : PATNI/SUDHIR
//Purpose       : validates the entered password
//*************************************************************
bool CPasswordDlg::validatePassword()
{
    bool isValid = false;

    if (!m_pwd_val.IsEmpty()) {

        char l_FullPath[256] = {0};
        sprintf(l_FullPath, "%s%s", getenv("MRMP_CONFIG"), m_pFilepath);
        char l_pwdBuff[256] = {0};
        FILE* l_pFile = NULL;

        if (NULL != (l_pFile = fopen(l_FullPath, "rb+"))) {
            fread(l_pwdBuff, sizeof(char), MAX_SIZE, l_pFile);
            fclose(l_pFile);

            CString l_filepwd(l_pwdBuff);
            TCHAR l_chsz_password[buf_size] = {'\0'};
            CPQMUtility::GetInstance()->AsciiToString((LPCTSTR)l_filepwd, l_chsz_password);

            const unsigned int l_nActual = _tcslen(l_chsz_password);
            const unsigned int l_password_length = m_pwd_val.GetLength();

            if (l_password_length == l_nActual) {
                //int l_nChkVal = 0;
                isValid = true;

                for (unsigned int l_nCount = 0; l_nCount < l_nActual; l_nCount++) {
                    if (l_chsz_password[l_nCount] == m_pwd_val.GetAt(l_nCount)) {
                        //l_nChkVal = 1;
                        continue;
                    }

                    //if(1 == l_nChkVal)
                    //{
                    //	break;
                    //}
                    isValid = false;
                    break;
                }

                //if(0 == l_nChkVal) {
                //isValid = true;
                //}
            }

        } else {
            isValid = false;
        }
    }

    return isValid;
}

//******************************************************************************
// Function Name    : SetAppearance
// Author           : PATNI/SUDHIR
// Purpose          : Modify appearance of controls.
//******************************************************************************
void CPasswordDlg::SetButtonAppearence(
)
{
    LPCTSTR button_font_name = _T("System") ;
    const int size = 12;

    m_pwd_edit.SetFont(button_font_name, size);
    m_pwd_edit.SetFocus();

    m_cancel_button.SetPrimaryTextFont(button_font_name, size);
    m_cancel_button.SetCurveType(CURVE_SMALL_3);

    WINDOW_COLORS_t stColors_o;
    GetWindowColors(stColors_o);
    COLORREF bk_color = stColors_o.crClientColor;

    m_pwd_static.LoadDeafultStaticCtrlColorsWithBKColor(bk_color);
    m_pwd_static.SetFontAttributes(button_font_name, size);
    m_Err_msg_static.LoadDeafultStaticCtrlColorsWithBKColor(bk_color);
    m_Err_msg_static.SetFontAttributes(button_font_name, size);
    m_Err_msg_static.ShowWindow(SW_HIDE);

}
//******************************************************************************
// Function Name    : SetDialogRibbonHeight
// Author           : PATNI/SUDHIR
// Purpose          : Set Dialog appearance
//******************************************************************************

void CPasswordDlg::SetDialogRibbonHeight()
{
    CRect client_rect;
    GetClientRect(&client_rect);

    CRect cancel_button_rect ;
    m_cancel_button.GetWindowRect(&cancel_button_rect);
    ScreenToClient(&cancel_button_rect);

    SetRibbonHeight(client_rect.bottom - cancel_button_rect.top + RIBBON_GAP);
}


//******************************************************************************
// Function Name    : SetMultiLingual
// Author           : PATNI/SUDHIR
// Purpose          : Set language strings
//******************************************************************************
void CPasswordDlg::SetMultilingual()
{

    // other controls
    CString l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_PASSWORD_DLG_TITLE"));

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        CWnd ::SetWindowText(l_chOutput);
    }

    l_chOutput = _T("");

    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_CANCEL_BUTTON"));

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        m_cancel_button.SetWindowText(l_chOutput);
    }

    l_chOutput = _T("");

    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_STATIC_DBDT_PWD"));

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        m_pwd_static.SetWindowText(l_chOutput);
    }

    l_chOutput = _T("");

    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_INVALID_PASSWORD"));

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        m_Err_msg_static.SetWindowText(l_chOutput);
    }

    l_chOutput = _T("");
}




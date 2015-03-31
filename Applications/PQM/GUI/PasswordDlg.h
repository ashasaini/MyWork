/*****************************************************************************
 *
 *  (c) Copyright 2011 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PasswordDlg.h
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

#ifndef __SM_PASSWORD_H__
#define __SM_PASSWORD_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PasswordDlg.h : header file
//

#include <MPlusUICommon/ButtonEx.h>
#include <MPlusUICommon/DialogEx.h>
#include <MPlusUICommon/EditCtrlEx.h>
#include <libStudyManager/NonCopyable.h>
#include <libStudyManager/UI/ZOrderWindow.h>
#include <PQM/resource.h>
#include "CustomStatic.h"

using namespace MPlus::UICommon;


/////////////////////////////////////////////////////////////////////////////
// CPasswordDlg dialog

class CPasswordDlg : public MR::ACQ::SM::COMMON::UI::CZOrderDialog,
    private MR::ACQ::SM::COMMON::NonCopyable<CPasswordDlg>
{
    // Construction
public:

    CPasswordDlg(CWnd* pParent = NULL);   // standard constructor

    bool OpenPasswordDlg();

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CPasswordDlg)
    virtual void OnCancel();
    //}}AFX_MSG
private:
    // Dialog Data
    //{{AFX_DATA(CPasswordDlg)
    enum { IDD = IDD_PASSWORD_DLG };
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA


    int				m_totalattempts;
    const char*		m_pFilepath;
    ButtonEx		m_cancel_button;
    EditCtrlEx		m_pwd_edit;
    CCustomStatic	m_pwd_static;
    CString			m_pwd_val;
    CCustomStatic	m_Err_msg_static;

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPasswordDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialogImpl();
    //}}AFX_VIRTUAL

    virtual BOOL PreTranslateMessageImpl(MSG* pMsg);
    void checkPWD();
    bool validatePassword();

    void SetButtonAppearence();
    void SetDialogRibbonHeight();
    void SetMultilingual();

    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #ifndef __SM_PASSWORD_H__

//Redmine-782
//Redmine-775
//Redmine-777
#ifndef __SM_PQM_DBDT_CAUTION_DIALOG_H__
#define __SM_PQM_DBDT_CAUTION_DIALOG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PqmdBdtCautionDialog.h : header file
//
#include <MPlusUICommon/DialogEx.h>
#include <MPlusUICommon/ButtonEx.h>

//Patni-PJS/2010Jun30/Added/TMSC-REDMINE-491
#include <MPlusConfigToolCommon/MPlusDBdtManager/dBdtAuthenticate.h>
#include <libStudyManager/NonCopyable.h>
#include "PQMEdit.h"
#include "CustomStatic.h"

using namespace MPlus::UICommon ;
using namespace MPlus::Srv::PasswordAuthenticate;
//-Patni-PJS/2010Jun30/Added/TMSC-REDMINE-491

/////////////////////////////////////////////////////////////////////////////
// CPqmdBdtCautionDialog dialog

class CPqmdBdtCautionDialog : public DialogEx,
    private MR::ACQ::SM::COMMON::NonCopyable<CPqmdBdtCautionDialog>
{
public:
    explicit CPqmdBdtCautionDialog(CWnd* pParent = NULL);   // standard constructor

protected:
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPqmdBdtCautionDialog)
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Generated message map functions
    //{{AFX_MSG(CPqmdBdtCautionDialog)
    virtual void OnOK();
    virtual void OnCancel();
    virtual BOOL OnInitDialog();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    virtual void OnDestroy();
    //}}AFX_MSG

private :
    // Dialog Data
    //{{AFX_DATA(CPqmdBdtCautionDialog)
    enum { IDD = IDD_CAUTION_DBDT };
    CCustomStatic	m_dbdt_pwd;
    CCustomStatic	m_dbtdt_text;
    CCustomStatic	m_error;
    CPQMEdit	m_passwd;
    ButtonEx	m_cancel;
    ButtonEx	m_ok;
    //}}AFX_DATA

    CdBdtAuthenticate m_dbdt_auth_passwd;
    int m_fail_cnt;
    CBrush m_bkbrush;

    void SetMultilingual();
    void SetDialogRibbonHeight();

    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #ifndef __SM_PQM_DBDT_CAUTION_DIALOG_H__

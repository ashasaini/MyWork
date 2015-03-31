//TMSC-REDMINE-783
//TMSC-REDMINE-780
//Redmine-775
//Redmine-777
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQM.h
 *  Overview: Structure of CPQM class.
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
#ifndef __SM_PQM_H__
#define __SM_PQM_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include <MPlusI18n/MPlusI18nResourceLoader.h> //Language usha
#include <tami/common/basicdefs.h>
#include <libStudyManager/NonCopyable.h>
#include "PQMGlobal.h"


class CPQMApp : public CWinApp,
    private MR::ACQ::SM::COMMON::NonCopyable<CPQMApp>
{
public:
    CPQMApp();
    BOOL ProcessShellCommand(CCommandLineInfo& f_cmd_info); //Patni-AMT/2009May15/Added/JFT# Unregister
    int IsRunAutomated();
    int IsRunEmbadded();

    bool GetIsMouseActive();
    void SetIsMouseActive(bool bFlag);

    void SetVoiceMenuMode(const int mode);

private:
    MPlus::I18n::CResourceLoader*   m_localize;//Language usha
    bool m_bMouseActive;
    COleTemplateServer m_server;

    int m_run_automated;
    int m_run_embadded;
    int m_mode;

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPQMApp)
    virtual BOOL InitInstance();
    virtual int ExitInstance();
    //}}AFX_VIRTUAL

    //{{AFX_MSG(CPQMApp)
    afx_msg void OnAppAbout();
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #ifndef __SM_PQM_H__

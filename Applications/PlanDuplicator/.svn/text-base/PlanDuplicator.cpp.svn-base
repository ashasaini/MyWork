// PlanDuplicator.cpp : Defines the class behaviors for the application.
//
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: PlanDuplicator
    Author: PATNI-PlanDuplicator Team
*******************************************************************************/
#include "stdafx.h"
#include "common.h"
#include "PlanDuplicator.h"
#include "PlanDuplicatorDlg.h"
//#include "PDController.h"
#include "PDLogHelper.h"
#include "PDTrace.h"
#include "PDErrorMsgDlg.h"
#include "PDProcessExec.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlanDuplicatorApp

BEGIN_MESSAGE_MAP(CPlanDuplicatorApp, CWinApp)
//{{AFX_MSG_MAP(CPlanDuplicatorApp)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG
ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlanDuplicatorApp construction
//****************************Method Header************************************

//Method Name   :CPlanDuplicatorApp

//Author        :PATNI

//Purpose       :Constructor
//*****************************************************************************
CPlanDuplicatorApp::CPlanDuplicatorApp()
{
    PD_TRACE(CPlanDuplicatorApp::CPlanDuplicatorApp);
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPlanDuplicatorApp object

CPlanDuplicatorApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPlanDuplicatorApp initialization

BOOL CPlanDuplicatorApp::InitInstance()
{
    PD_TRACE(CPlanDuplicatorApp::InitInstance);
    AfxEnableControlContainer();
    
#ifdef _AFXDLL
    Enable3dControls();         // Call this when using MFC in a shared DLL
#else
    Enable3dControlsStatic();   // Call this when linking to MFC statically
#endif
    //Mpower 047/Sep-2012//AN/Added : Start
    //Only single instance of PDTool is allowed to run at a time
    CPDProcessExec l_pdexec;
    char * l_csExe_name = PD_APPLICATION;
    bool l_flag = false;
    if_no_error(l_pdexec.GetProcessList(l_csExe_name,l_flag ))
    {
        if (l_flag)
        {
          return FALSE;
            
        }
    }
    ////Mpower 047/Sep-2012/AN/Added :End
    int l_nstatus = CPDLogHelper::CreateInstance();
    if(E_NO_ERROR != l_nstatus)
    {
        CPDErrorMsgDlg l_errmsg_dlg;
        CString l_cserrmsg (_T("failed to create logger!!!!"));
        l_errmsg_dlg.SetErrorMessage(l_cserrmsg);
        l_errmsg_dlg.DoModal();
    }
    CPlanDuplicatorDlg dlg;
    m_pMainWnd = &dlg;
    
    int nResponse = dlg.DoModal();
    
    CCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);
    
    //if (!ProcessShellCommand(cmdInfo))
    
    if (nResponse == IDOK){
        // TODO: Place code here to handle when the dialog is
        //  dismissed with OK
    }else if (nResponse == IDCANCEL){
        // TODO: Place code here to handle when the dialog is
        //  dismissed with Cancel
    }
    
    // Since the dialog has been closed, return FALSE so that we exit the
    //  application, rather than start the application's message pump.
    return(FALSE);
}

////////////////////////////////    End of File     /////////////////////////////
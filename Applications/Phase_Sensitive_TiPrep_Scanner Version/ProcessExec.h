// PDProcessExec.h: interface for the CProcessExec class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#if !defined(AFX_PDPROCESSEXEC_H__FE4E6EB9_BF54_430D_B55F_5B71CAF4512D__INCLUDED_)
#define AFX_PDPROCESSEXEC_H__FE4E6EB9_BF54_430D_B55F_5B71CAF4512D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CProcessExec
{
public:
	int FindChildHWND(HWND &hwnd);
    CProcessExec();
    virtual ~CProcessExec();
    int GetProcessList(const char * process_name, bool &f_flag);
    int CreateProc(CString &l_csProcessNm, CString &l_csParams, HWND &handle);
    int ExitProc(void);
private:
    PROCESS_INFORMATION m_pinfo;
};

#endif // !defined(AFX_PDPROCESSEXEC_H__FE4E6EB9_BF54_430D_B55F_5B71CAF4512D__INCLUDED_)

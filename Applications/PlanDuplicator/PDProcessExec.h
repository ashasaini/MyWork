// PDProcessExec.h: interface for the CPDProcessExec class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PDPROCESSEXEC_H__FE4E6EB9_BF54_430D_B55F_5B71CAF4512D__INCLUDED_)
#define AFX_PDPROCESSEXEC_H__FE4E6EB9_BF54_430D_B55F_5B71CAF4512D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPDProcessExec  
{
public:
    CPDProcessExec();
    virtual ~CPDProcessExec();
    int CPDProcessExec::GetProcessList( const char * process_name,bool &f_flag);
    int CreateProc(CString &l_csProcessNm);
    int ExitProc(void);
private:
    PROCESS_INFORMATION m_pinfo;
};

#endif // !defined(AFX_PDPROCESSEXEC_H__FE4E6EB9_BF54_430D_B55F_5B71CAF4512D__INCLUDED_)

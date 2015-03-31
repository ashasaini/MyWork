// KtReconExecProc.h: interface for the CKtReconExecProc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KTRECONEXECPROC_H__BD4796C6_9DC3_4A8C_A1D7_6B0AD8EDB02B__INCLUDED_)
#define AFX_KTRECONEXECPROC_H__BD4796C6_9DC3_4A8C_A1D7_6B0AD8EDB02B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KtReconErrors.h"
class CKtReconExecProc  
{
public:
	CKtReconExecProc();
	virtual ~CKtReconExecProc();

    int InvokeExec(int f_nExe, CString f_csParams, bool f_bIsKtReconButton );
    int ExitProc(void);

private:
    int CKtReconExecProc::GetProcessList(const char * f_process_name, bool &f_flag, int f_nExe);
    int CreateProc(CString &f_csProcessNm, CString &f_csParams, PROCESS_INFORMATION &f_pid);
    bool IsProcessRunning(const char * l_csProc_name,int f_nExe);
    
	PROCESS_INFORMATION m_pinfoRawCopy;
	PROCESS_INFORMATION m_pinfoVROMProc;
	PROCESS_INFORMATION m_pinfoReconViewer;    


};

#endif // !defined(AFX_KTRECONEXECPROC_H__BD4796C6_9DC3_4A8C_A1D7_6B0AD8EDB02B__INCLUDED_)
/////////////////////////////////END OF FILE /////////////////////////////////////
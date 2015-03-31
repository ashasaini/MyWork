// KtReconTrace.cpp: implementation of the CKtReconTrace class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KtReconTrace.h"
#include "KtReconLogHelper.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CKtReconTrace::CKtReconTrace()
{
}

CKtReconTrace::CKtReconTrace(const CString& f_func_name)
{
    CString    l_trace_line = _T("");
    m_func_name = f_func_name;
    l_trace_line.Format(_T("[0x%8lx] Entering %s()\n"), GetCurrentThreadId(),
        m_func_name);
    
    CKtReconLogHelper::WriteToTraceLog
        (_T("MRI_KTRECON"), MRTraceMarker1, l_trace_line,
        _T("CKtReconTrace::CKtReconTrace"));
    
}
/**
*   A destructor.
*   This function is used for deleting the memory if used.
*/
CKtReconTrace::~CKtReconTrace()
{
    CString     l_trace_line = _T("");
    l_trace_line.Format(_T("[0x%8lx] Exiting %s()\n"), GetCurrentThreadId(),
        m_func_name);
    
    CKtReconLogHelper::WriteToTraceLog
        (_T("MRI_KTRECON"), MRTraceMarker1, l_trace_line,
        _T("CKtReconTrace::~CKtReconTrace"));
}

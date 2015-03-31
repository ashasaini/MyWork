// PDTrace.cpp: implementation of the CPDTrace class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PDTrace.h"
#include "PDLogHelper.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*************************************************************
* Method:    	CPDTrace
* Description: 	This function is used 
* Parameter: 	const CString & f_func_name :
* Returns:   	
*************************************************************/
CPDTrace::CPDTrace(const CString& f_func_name)
{
    CString    l_trace_line = _T("");
    m_func_name = f_func_name;
    l_trace_line.Format(_T("[0x%8lx] Entering %s()\n"), GetCurrentThreadId(),
        m_func_name);
    CPDLogHelper::WriteToTraceLog(_T("MRI_PD"),MRTraceMarker5,
        l_trace_line,m_func_name);
}
/**
*   A destructor.
*   This function is used for deleting the memory if used.
*/

CPDTrace::~CPDTrace()
{
    CString     l_trace_line = _T("");
    l_trace_line.Format(_T("[0x%8lx] Exiting %s()\n"), GetCurrentThreadId(),
        m_func_name);
    CPDLogHelper::WriteToTraceLog(_T("MRI_PD"),MRTraceMarker5,
        l_trace_line,m_func_name);
}

////////////////////////    End Of File     /////////////////////////////////



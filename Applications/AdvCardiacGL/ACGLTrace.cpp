/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: ACGL
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#include "stdafx.h"
#include "ACGLTrace.h"
#include "ACGLLogHelper.h"

/**
*   A constructor.
*   This function is used for initialization of class variables.
*/
CACGLTrace::CACGLTrace()
{
}
/**
*   A constructor.
*   This function is used for initialization of class variables.
*/
CACGLTrace::CACGLTrace(const CString& f_func_name)
{
    CString    l_trace_line = _T("");
    m_func_name = f_func_name;
    l_trace_line.Format(_T("[0x%8lx] Entering %s()\n"), GetCurrentThreadId(),
                        m_func_name);

    CACGLLogHelper::WriteToTraceLog
        (_T("MRI_ACGL"), MRTraceMarker3, l_trace_line,
                    _T("CACGLTrace::CACGLTrace"));

}
/**
*   A destructor.
*   This function is used for deleting the memory if used.
*/
CACGLTrace::~CACGLTrace()
{
    CString     l_trace_line = _T("");
    l_trace_line.Format(_T("[0x%8lx] Exiting %s()\n"), GetCurrentThreadId(),
                        m_func_name);

    CACGLLogHelper::WriteToTraceLog
        (_T("MRI_ACGL"), MRTraceMarker3, l_trace_line,
                    _T("CACGLTrace::~CACGLTrace"));
}
///////////////////////////////// END OF FILE /////////////////////////////////
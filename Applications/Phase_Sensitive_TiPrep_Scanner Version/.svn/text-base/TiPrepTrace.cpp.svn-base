/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#include "stdafx.h"
#include "TiPrepTrace.h"
#include "TiPrepLogHelper.h"

/**
*   A constructor.
*   This function is used for initialization of class variables.
*/
CTiPrepTrace::CTiPrepTrace()
{
}
/**
*   A constructor.
*   This function is used for initialization of class variables.
*/
CTiPrepTrace::CTiPrepTrace(const CString& f_func_name,unsigned long f_ulMarker)
{
    CString    l_trace_line = _T("");
    m_func_name = f_func_name;
    l_trace_line.Format(_T("[0x%8lx] Entering %s()\n"), GetCurrentThreadId(),
                        m_func_name);

    CTiPrepLogHelper::WriteToTraceLog
        (_T("MRI_TIPREP"), f_ulMarker, l_trace_line,
                    _T("CTiPrepTrace::CTiPrepTrace"));

}
/**
*   A destructor.
*   This function is used for deleting the memory if used.
*/
CTiPrepTrace::~CTiPrepTrace()
{
    CString     l_trace_line = _T("");
    l_trace_line.Format(_T("[0x%8lx] Exiting %s()\n"), GetCurrentThreadId(),
                        m_func_name);

    CTiPrepLogHelper::WriteToTraceLog
        (_T("MRI_TIPREP"), MRTraceMarker14, l_trace_line,
                    _T("CTiPrepTrace::~CTiPrepTrace"));
}

///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////

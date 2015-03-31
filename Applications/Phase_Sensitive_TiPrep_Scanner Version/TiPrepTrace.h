/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#if !defined(AFX_TIPREPTRACE_H__1DDF7C4C_BA9A_4D73_B8DD_1A8B5195657B__INCLUDED_)
#define AFX_TIPREPTRACE_H__1DDF7C4C_BA9A_4D73_B8DD_1A8B5195657B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/**
    This micro is used trace entry and exit in particular function.
*/
#define TIPREP_TRACE( func_name, marker )          \
    CTiPrepTrace trace_msg( _T(#func_name),marker );
/**
    This class is used for trace.
*/
class CTiPrepTrace
{
public:
    CTiPrepTrace();
    CTiPrepTrace(const CString&, unsigned long);
    virtual ~CTiPrepTrace();

private:
    CString m_func_name;
};

#endif // !defined(AFX_TIPREPTRACE_H__1DDF7C4C_BA9A_4D73_B8DD_1A8B5195657B__INCLUDED_)

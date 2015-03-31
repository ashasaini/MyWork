// PDTrace.h: interface for the CPDTrace class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PDTRACE_H__5F9CD47B_1E3B_45BC_AF28_B1CCF9495A81__INCLUDED_)
#define AFX_PDTRACE_H__5F9CD47B_1E3B_45BC_AF28_B1CCF9495A81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PD_TRACE( func_name )          \
CPDTrace trace_msg( _T(#func_name) );

class CPDTrace  
{
public:
    CPDTrace();
    explicit CPDTrace(const CString&);
    virtual ~CPDTrace();

private:
    CString m_func_name;

};

#endif // !defined(AFX_PDTRACE_H__5F9CD47B_1E3B_45BC_AF28_B1CCF9495A81__INCLUDED_)

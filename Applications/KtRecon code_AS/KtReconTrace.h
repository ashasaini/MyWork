// KtReconTrace.h: interface for the CKtReconTrace class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KTRECONTRACE_H__89EA683E_F6B8_4893_9F05_11571F12B3F1__INCLUDED_)
#define AFX_KTRECONTRACE_H__89EA683E_F6B8_4893_9F05_11571F12B3F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CKtReconTrace  
{
public:
	CKtReconTrace();
	virtual ~CKtReconTrace();
    CKtReconTrace(const CString&);
private:
        CString m_func_name;

};

#endif // !defined(AFX_KTRECONTRACE_H__89EA683E_F6B8_4893_9F05_11571F12B3F1__INCLUDED_)

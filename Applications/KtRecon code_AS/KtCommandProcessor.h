// KtCommandProcessor.h: interface for the CKtCommandProcessor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KTCOMMANDPROCESSOR_H__23DD4FB9_6334_4B81_8A70_2475E8D1D11E__INCLUDED_)
#define AFX_KTCOMMANDPROCESSOR_H__23DD4FB9_6334_4B81_8A70_2475E8D1D11E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CKtCommandProcessor  
{
public:
	virtual int GetCommand(CString f_csStudyName, int f_nProtIndx, CString &f_csCommand);
	CKtCommandProcessor();
	virtual ~CKtCommandProcessor();
};

#endif // !defined(AFX_KTCOMMANDPROCESSOR_H__23DD4FB9_6334_4B81_8A70_2475E8D1D11E__INCLUDED_)
/////////////////////////////////END OF FILE /////////////////////////////////////
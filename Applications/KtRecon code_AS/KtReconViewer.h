// KtReconViewer.h: interface for the CKtReconViewer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KTRECONVIEWER_H__BF6A2926_62DB_4823_B4C6_18743055BB1D__INCLUDED_)
#define AFX_KTRECONVIEWER_H__BF6A2926_62DB_4823_B4C6_18743055BB1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KtCommandProcessor.h"

class CKtReconViewer : public CKtCommandProcessor  
{
public:
	CKtReconViewer();
	virtual ~CKtReconViewer();
    int GetCommand(CString f_csStudyName, int f_nProtIndx, CString &f_csCommand);
    void SetIsCalledFromKtReconButton(bool f_bflag);
private:
    bool IsCalledFromKtReconButton;

};

#endif // !defined(AFX_KTRECONVIEWER_H__BF6A2926_62DB_4823_B4C6_18743055BB1D__INCLUDED_)
/////////////////////////////////END OF FILE /////////////////////////////////////
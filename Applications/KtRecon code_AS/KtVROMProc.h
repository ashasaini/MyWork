// KtVROMProc.h: interface for the CKtVROMProc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KTVROMPROC_H__BF67D1B1_9E91_4B6F_8A2E_7924A4CE7EE7__INCLUDED_)
#define AFX_KTVROMPROC_H__BF67D1B1_9E91_4B6F_8A2E_7924A4CE7EE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KtCommandProcessor.h"

class CKtVROMProc : public CKtCommandProcessor  
{
public:
	CKtVROMProc();
	virtual ~CKtVROMProc();
    int GetCommand(CString f_csStudyName, int f_nProtIndx, CString &f_csCommand);
    int GetMaxNumOfThreadsAllowed(int &f_NumOfThreads);
};

#endif // !defined(AFX_KTVROMPROC_H__BF67D1B1_9E91_4B6F_8A2E_7924A4CE7EE7__INCLUDED_)
/////////////////////////////////END OF FILE /////////////////////////////////////
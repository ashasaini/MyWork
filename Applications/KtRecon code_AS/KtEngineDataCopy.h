// KtEngineDataCopy.h: interface for the CKtEngineDataCopy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KTENGINEDATACOPY_H__D4E9EA85_5F60_4813_84C5_D8F3C015931A__INCLUDED_)
#define AFX_KTENGINEDATACOPY_H__D4E9EA85_5F60_4813_84C5_D8F3C015931A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KtCommandProcessor.h"

class CKtEngineDataCopy : public CKtCommandProcessor  
{
public:
	int GetCommand(CString f_csStudyName, int f_nProtIndx, CString &f_csCommand);
	CKtEngineDataCopy();
	virtual ~CKtEngineDataCopy();
};

#endif // !defined(AFX_KTENGINEDATACOPY_H__D4E9EA85_5F60_4813_84C5_D8F3C015931A__INCLUDED_)
/////////////////////////////////END OF FILE /////////////////////////////////////
// KtICMMgr.h: interface for the CKtICMMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KTICMMGR_H__5BF7AFFB_8B18_46F6_B069_DD8F53BDEF0A__INCLUDED_)
#define AFX_KTICMMGR_H__5BF7AFFB_8B18_46F6_B069_DD8F53BDEF0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <vector>
using namespace std;

class CKtICMMgr  
{
public:
	CKtICMMgr();
	virtual ~CKtICMMgr();
    BOOL GetSelectedImages();
    int GetLOIDSelectedImagesFromIsel(const CString &f_csICID,/*out*/vector <CString> &f_csImgLOID);    
    int GetStudyFileName(const CString &f_csICID,CString &f_csStudyFileName,/*in*/vector <CString> & f_csImgLOID);
private:
    bool m_is_wait_request_isel;

};

#endif // !defined(AFX_KTICMMGR_H__5BF7AFFB_8B18_46F6_B069_DD8F53BDEF0A__INCLUDED_)
/////////////////////////////////END OF FILE /////////////////////////////////////
// TiProlimUpdater.h: interface for the CTiProlimUpdater class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIPROLIMUPDATER_H__45F0EA52_6D8C_4A17_8863_1139E54C7BE3__INCLUDED_)
#define AFX_TIPROLIMUPDATER_H__45F0EA52_6D8C_4A17_8863_1139E54C7BE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTiProlimUpdater  
{
public:
	CString m_csstudyfile;
	void SetData(int prot,float ti,CString &);
	int ProlimTi();
	float m_ftival;
	int m_nprotocol;
	CTiProlimUpdater();
	virtual ~CTiProlimUpdater();

};

#endif // !defined(AFX_TIPROLIMUPDATER_H__45F0EA52_6D8C_4A17_8863_1139E54C7BE3__INCLUDED_)

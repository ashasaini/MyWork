// ReconProcess.h: interface for the CReconProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECONPROCESS_H__1D045531_A58E_4A3E_BF3A_5F6C6B84FBA4__INCLUDED_)
#define AFX_RECONPROCESS_H__1D045531_A58E_4A3E_BF3A_5F6C6B84FBA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CReconProcess  
{
public:
	CReconProcess();
	virtual ~CReconProcess();
    int performReconstruction(CString &f_csStudyFileName, int f_nProtocolNum, int f_nDataForm);
    void setProtocolNumber(const int &f_nProtNo, const CString &f_csStudyFileName); // { m_csloids = val; }
    int getProtocolNumber(int &f_nProtNo, CString &f_csStudyFileName);

private:
    CString m_csProcessName;
    int m_nProtocolNumber;
    CString m_csStudyFileName;

};

#endif // !defined(AFX_RECONPROCESS_H__1D045531_A58E_4A3E_BF3A_5F6C6B84FBA4__INCLUDED_)

// KtReconController.h: interface for the CKtReconController class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KTRECONCONTROLLER_H__852D1D90_5A2A_453A_9912_40C5C36811B9__INCLUDED_)
#define AFX_KTRECONCONTROLLER_H__852D1D90_5A2A_453A_9912_40C5C36811B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ktReconExecProc.h"

#include <vector>
using namespace std;

class CKtReconDlg;
class CKtReconController  
{
public:
	int StartGUI(CWnd * parent);
	void SetProtocolNumber(int prot_no);
	int SetStudyFileLoid(CString &Studyloid);
	int GetStudyLoidFromPager();
    static CKtReconController * GetKtController();
	int OnExit();
	int OnView();
	int OnKtRecon();
	int OnGetData();
	CKtReconController();
	virtual ~CKtReconController();
    void SetDlg( CKtReconDlg* f_dlg );
//    int OnRecvMsgFromIsel(const CString &f_csICID);
    int SetImgLoidInController(/*in*/vector <CString> & f_csImgLOID);
	int GetDataForDisplayingScannedProtocol(/*in*///vector <CString> & f_csImgLOID,
                                                           // vector<CString> & f_csSeriesLoidVector,
                                                           // vector<long> & f_nSeriesNumberVector,
                                                            vector<long> & f_nProtocolNumberVector,
                                                            //CString & f_source_series,
                                                            int & f_nTotalProtocolCount
                                                            //int & f_nTotalSeriesCount
															);
	//+Mpower/20130812/PN/added
	void SetWaitCursor(bool f_flag);
	bool IsWaitCursor();
	//-Mpower/20130812/PN/added
		CString m_csstudyloid;

private:
    vector <CString> m_csImageLoid;
    CString m_csStudyFileName; //Remove it
    int m_nSelectedProtIndex;
    vector <CString> m_csSeriesLoid;
    vector <long> m_nsSeriesNumberArray;
    CKtReconExecProc *m_ExecProc;
    CKtReconDlg *m_KtReconDlg;
	bool m_is_wait_cursor; //+Mpower/20130812/PN/added

};

#endif // !defined(AFX_KTRECONCONTROLLER_H__852D1D90_5A2A_453A_9912_40C5C36811B9__INCLUDED_)
/////////////////////////////////END OF FILE /////////////////////////////////////
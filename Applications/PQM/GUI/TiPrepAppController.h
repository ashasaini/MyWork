// TiPrepAppController.h: interface for the CTiPrepAppController class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIPREPAPPCONTROLLER_H__C3902663_067F_4AE8_AECB_137DCA4215DF__INCLUDED_)
#define AFX_TIPREPAPPCONTROLLER_H__C3902663_067F_4AE8_AECB_137DCA4215DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <libStudyManager/NonCopyable.h>

class CTiPrepAppController  : private MR::ACQ::SM::COMMON::NonCopyable<CTiPrepAppController> 
{
public:
	void CloseConnectionToTiPrep();
	void GetPQMStudyLoid();
	int SelectImagesForTiPrep();
	int ProcessUpdateRequest(const CString& studyloid, const CString &cstemplate);
	int OnRecvConnectionClose(const CString &studyloid,const CString &cstemplate);
	int CanTiPrepUpdate(int protocol,const CString &studyloid, const CString &cstemplate);
	void SetTiPrepInvoked(bool flag);
	int OnRecvTiPrepRegister(int pid, const CString &csstudyloid,const CString &cstemplate);
	int TiPrepConnectionClose();
	int TiPrepConnected(const CString &cstemplate);
	int ProcessPageChange(const CString & csParam);
	int SendLoadTiPrep(const CString & csParam = _T(""));
	int ChangeStudyInEmptyTemplate(const CString &cstemplate );
	int ExtractPagerInfo(const CString &param);
	int GetTiPrepTemplate();
	void SetTiPrepLoadWait(bool);
	bool GetTiPrepLoadWait();
	bool GetTiPrepRunningState();
	void SetTiPrepRunning(bool isrunning);
	bool m_tiprep_running;
	bool m_tiprep_loadgl_wait;
	bool CanTiPrepEnable();
	bool CanTiPrepInvoke();
	int InvokeTiPrep(bool bymenu);
    HRESULT IsProtocolforTiPrep();
    HRESULT GetTiPrepLicense(const int tipreptype);
    int SendRefreshMsgToTiPrep();
	CTiPrepAppController();
	virtual ~CTiPrepAppController();

private:
	int m_nprotocolSelected;
	CString m_tiprep_template;  //redudnant
	CString m_cs_tiprep_template; //in which it was invoked
		CString m_study_loid; //acquire study
			bool m_bisTiPrepInvoked; // whether it was invoked from pqm or not
	int m_isconnected_tiprep;  //



};

#endif // !defined(AFX_TIPREPAPPCONTROLLER_H__C3902663_067F_4AE8_AECB_137DCA4215DF__INCLUDED_)

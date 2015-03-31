// IRSListner.h: interface for the CIRSListner class.
//
//////////////////////////////////////////////////////////////////////

/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#if !defined(AFX_IRSLISTNER_H__C91DD82A_3211_47FA_8527_70AD90DB5B32__INCLUDED_)
#define AFX_IRSLISTNER_H__C91DD82A_3211_47FA_8527_70AD90DB5B32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//const int INVALID_FD = -1;
class CIRSListner
{
public:
    CIRSListner();
    virtual ~CIRSListner();
    CIRSListner& operator=(CIRSListner &f_irsListner);
    CIRSListner(CIRSListner &f_irsListner);

    // Initialize Create Listener thread and starts listening
    void ForceExitIRS();
    BOOL WaitForIRSDone();
    BOOL InitializeIRS();
    void CloseIrsNotifier();
    int GetIRSSocketFD();
    int getReconButtonClk() const {
        return m_nReconButtonClk;
    }
    void setReconButtonClk(int val) {
        m_nReconButtonClk = val;
    }
    int getIRSMsgCnt() const {
        return m_nIRSMsgCnt;
    }
    void setIRSMsgCnt(int val) {
        m_nIRSMsgCnt = val;
    }
    int getSeriseCnt() const {
        return m_nSeriseCnt;
    }
    void setSeriseCnt(int val) {
        m_nSeriseCnt = val;
    }
    void incrementSeriseCnt() {
        m_nSeriseCnt++;
    }
    int getReconCalled() const {
        return m_nReconCalled;
    }
    void setReconCalled(int val) {
        m_nReconCalled = val;
    }
    CString GetStudyLoidFromCFWIRSMsg(const char* f_irs_msg) const;
    

private:
    // Thread function which will continously listening
    static DWORD WINAPI IRSListnerThread(LPVOID f_parameter);
    
    status_t IRSRegisterReconDone();
    
    // call back function when IRS starts communicating with PQM
    void IRSEventHandler();
    void IrsTerminate();
    

private:
	//bool m_recon_response;
    int                     m_bIndexToPass;
    int                     m_irs_socket;
    HANDLE                  m_irslisner_thread;
    int                     m_nReconCalled;
    int                     m_nReconButtonClk;
    int                     m_nSeriseCnt;
    int                     m_process_node;
    int                     m_IrsInitCount;
    int                     m_IrsTerminateF;
    int                     m_nIRSMsgCnt;

};

#endif // !defined(AFX_IRSLISTNER_H__C91DD82A_3211_47FA_8527_70AD90DB5B32__INCLUDED_)

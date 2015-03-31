#if !defined __SM_IPCLIB_PQMPMSARMANAGER__H_
#define __SM_IPCLIB_PQMPMSARMANAGER__H_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>
#include <libSARManager/LibSARManager.h>
#include "libSARMgrClientAppInterface.h"
#include "Pqm.h"



class CPqm;
class CPqmProtocol;

class CPqmPmSARManager : private MR::ACQ::SM::COMMON::NonCopyable<CPqmPmSARManager>
{
    friend class CPqm;
public:


    BOOL ConnectToSARManager();
    void OnSARMgrDisconnect();

    void SendSessionStartMsgToSARMgr()const;
    void SendSessionEndMsgToSARMgr() const;
    void SendRTSARInfoForLastDoneProtocol();

    void ReInvokeSARManager() ;
    void HandleAbortFailforSARControl() ;

    void CalcSARControlAndPutVFForAll(BOOL f_is_scanning = FALSE) ;

    void CreateScanEditBuffer(const int32_t f_current_acq_order);
    void UpdateScanEditBuffer();
    void ClearScanEditBuffer();
    int  GetMaxTimeForPagerTimer() const {
        return m_max_pager_timer_time;
    };

    void ResetSarPreScanParam(sarInfo_p f_sar_info) const;

private:

    CPqm*						  	 m_pqmptr;
    LIBSARMGR::CLibSARManager*			m_lib_sar_manager;
    CLibSARMgrClientAppInterface*	m_libsar_mgr_client_interface;


    HMODULE								m_module;
    int									m_max_pager_timer_time;


    explicit CPqmPmSARManager(CPqm* f_pqmptr);
    virtual ~CPqmPmSARManager();

    bool CanProcessSARManager() const;
    int CalcHistoryTimeDiff(const LPSYSTEMTIME f_rt_sar_time) const;

};

#endif // __SM_IPCLIB_PQMPMSARMANAGER__H_


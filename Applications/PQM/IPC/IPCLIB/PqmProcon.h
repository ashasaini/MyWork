//Redmine-783
//Redmine-774
//Redmine-780
//Redmine-782
// PqmProcon.h: interface for the CPqmProcon class.
//
//////////////////////////////////////////////////////////////////////
#ifndef __SM_IPCLIB_PQMPROCON_H__
#define __SM_IPCLIB_PQMPROCON_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tami/libvf/varfields.h>
#include <libStudyManager/NonCopyable.h>
#include <libStudyManager/Mutex/ThreadMutex.h>
#include <PQM/PQMCommanMessages.h>
#include "pqmmessagestructures.h"






#define PROMSG_HEAD_LEN		(sizeof(int32_t)+sizeof(clock_t)+sizeof(int32_t))

/******************************************************************************
 * Error return codes
 *****************************************************************************/

#define PROMSG_E_IO		        (E_ERROR+1)
#define PROMSG_E_EOF		    (E_ERROR+2)
#define PROMSG_E_MALLOC		    (E_ERROR+3)

#define K_PM_TYPE_PDET			0x00010000
#define K_PM_PROLIM_EDIT		(K_PM_TYPE_PROLIM + 1)
#define K_PM_PROLIM_GLOC		(K_PM_TYPE_PROLIM + 2)
#define K_PM_PROLIM_QUEUE_NEXT	(K_PM_TYPE_PROLIM + 3)
#define K_PM_PROLIM_NEXT_COPY	(K_PM_TYPE_PROLIM + 4)
#define K_PM_PROLIM_QUEUE_EXIT	(K_PM_TYPE_PROLIM + 5)
#define K_PM_PROLIM_CANCEL		(K_PM_TYPE_PROLIM + 6)
#define K_PM_PROLIM_RESET		(K_PM_TYPE_PROLIM + 7)
#define PROCON_IDENT_PROEDIT	"proEdit"
#define PROCON_IDENT_LOCATOR	"Locator"
#define PROCON_IDENT_PQM		"PQM"
#define PROCON_IDENT_WSE		"WSE"
#define PROCON_IDENT_PROWFDA	"proWFDA"
//Patni-MSN/2009Apr13/Added/PSP- Internal Defect Fix
#define PROMSG_PQM_PROBE_PLAN_EXIT 556
//Patni-PJS/2010Jan2/JaFT/TMSC_REDMINE-111
#define PROMSG_PQM_PROBE_ABORT     555

typedef enum AUTO_GATE_INTERVAL_STATUS {
    AUTO_GATE_INTERVAL_NONE = 0,
    AUTO_GATE_INVOKEPL_REQUEST,
    GATE_INTERVAL_REQUEST
};

class CSocketMsg;
class CPqm;
class CPqmProtocol;
class CPQMSocket;
class CPqmProbe;


class CPqmProcon : private MR::ACQ::SM::COMMON::NonCopyable<CPqmProcon>
{

public:
    //Constrcutor / Destructor
    explicit CPqmProcon(CPqm* f_pqmptr);
    virtual ~CPqmProcon();

    //Patni-Hemant/2010Nov13/Added/IR-167_REOPEN_ISSUE
    void OnProbeClosed();
    //Patni-PJS/2010Jan2/JaFT/TMSC_REDMINE-111
    void AbortProbeScan();
    //Patni-SS/13-Jan-10/Modified/MVC005961
    bool pqmPmLocatorControl(CPqmProtocol* f_protocol, const int f_index);

    //+Patni-Ajay/61850/Added/TMSC-REDMINE-443
    int ProconRequestUiCB(int f_tag, int f_acq_order = -1, bool f_from_server_config = true , int f_maskValue = -1);
    //Patni-HEMANT/ADDED On 3/31/2009 1:19:09 PM/ PSP / IDS#202, IDS#192
    bool ConnectToProcon(bool f_only_try_to_connect = false);
    bool OnProconMessage(const CSocketMsg& f_msg);
    //Patni-HEMANT/ADDED On 3/31/2009 1:16:15 PM/ PSP / IDS#202, IDS#192
    bool OnProconDisconnect(bool f_create_thread_for_new_connection = true);
    bool SendMessageToProcon(const int f_code, const int f_length, void* f_data_p);
    int  SeverConfigToProCon(int, const int, int*, const int);
    bool StopProlim() ;
    int  PqmGetParentIndexFplanning(const int	protocol, bool f_bGetModeScanNum = false); 	//Patni-RSG-MN/2009Sept11/Modified/MVC3227/Added default param
    void ProlimGateInterval(const int f_gate_method);
    //SM4 LOCATOR_IMAGE Himanshu 3 Dec 2008
    void SearchParentLocatorImage(const int f_protocol, VfPathElem_t* f_loc_images,
                                  VfPathElem_t* f_loc_images2, VfPathElem_t* f_loc_images3);	// pqmPmGetLocatorImage in vantage

    bool_t PqmPqIsGlocOk(const int pos , bool f_bGetModeScanNum = false);  //Patni-RSG-MN/2009Sept11/Modified/MVC3227/Added default param,changed to public
    //Patni-RUP/2010Jan08/Close Porbe defect fix
    void CloseProbe(void);
    //Patni-KSS/2010Oct08/Added/V1.35#IR-167
    bool ProcessProbeMsgForIRS(const pqm_msg_tag f_msg) const;


    //+Patni-RUP/2010Aug06/Added/To fix PQM exit crash
    bool GetPQMExitFlag()const;
    void SetPQMExitFlag(const bool& f_pqm_exit_flag);
    //-Patni-RUP/2010Aug06/Added/To fix PQM exit crash

    HANDLE GetProbeSockEventHandle();

    int   GetCurrentEditProt()const;
    int   GetDataForProbe(void**);
    CPqm* GetPQM() const;
    CPqmProtocol* GetCurrentProtocol() const;

    CPqmProbe* GetPQMProbe() const ;
    PqmPmProlimT* GetPqmPmProlim(void);
    bool GetIsProconActive() const;

    void SetPqmPmDelay(delayInfo_t& f_delayinfo);
    AUTO_GATE_INTERVAL_STATUS GetAutoIntervalStatus() const;

    bool IsPlanDynamicExecutedForCurrent() const;
    CPqmProtocol* GetMasterProtocolForPlanDynimc();
    CPQMSocket* GetProconSocket();

private:

    bool m_probe_invoked;
    bool m_scan_editing;
    int  m_wse_or_locator;
    int  m_gui_event;
    CPQMSocket*   m_procon_socket;
    CPqm*		  m_pqmptr;
    char*         m_scratchpad_file;
    CPqmProtocol* m_curr_protocol;
    int  m_selectCount;
    int* m_selectList;
    CPqmProbe*    m_probe;
    PqmPmProlimT* m_pqm_procon;
    bool          m_pqm_exit;   //Patni-RUP/2010Aug06/Added/To fix PQM exit crash
    HANDLE        m_proconreconnect_thread ;
    HANDLE		  e_pqm_req_to_create_probesock;

    bool	m_is_plan_dynamic_executed;
    bool	m_is_locator_invoked;
    AUTO_GATE_INTERVAL_STATUS    m_auto_intervalstatus;

    bool	m_result_pmLocator;
    bool	m_reset_flag;
    bool	m_can_process_reset;

    bool m_inscan_switch_recv_flag;

    MR::ACQ::SM::COMMON::MUTEX::CThreadMutex m_pqmGetParentIndex_lock;

    inline char* ConvertToChar(const CString& f_data);

    status_t CreateProconScratchPad(const bool f_fluoro = false);
    status_t SendServerConfigToProcon(const bool f_fluoro = false);

    bool SendMsg2InvokeUi(char* f_exe_name);
    // thread to poll proCon server connection
    void PqmPmSetNextProtocol(CPqmProtocol* f_current_protocol);
    void PqmSendProConInitMsgs(CPqmProtocol*	f_protocol);
    TCHAR* ProMsgErrcodeStr(int	code);
    int    NegotiationToProCon();
    void   StartProbeClientThread() const;
    void   CloseProbePlanDlg();

    static DWORD WINAPI ProConReConnectThread(LPVOID f_parameter);

    bool InvokeProbeDialog();
    status_t SetNextProconCurrent(CPqmProtocol* f_current_protocol);

    CString GetMsgStrFromCode(const int f_msg_code);

    void SetAutoIntervalStatus(const AUTO_GATE_INTERVAL_STATUS f_auto_intervalstatus);
    status_t MaintainScratchPadForProcon(CPqmProtocol* f_current_protocol);

};
#endif // #ifndef __SM_IPCLIB_PQMPROCON_H__ 

//Redmine-783
//Redmine-774
//Redmine-780
//Redmine-782
// PqmProbe.h: interface for the CPqmProbe class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_PQMPROBE_H__
#define __SM_IPCLIB_PQMPROBE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tami/libproMsg/proMsg.h>
#include <libStudyManager/NonCopyable.h>
//Patni-RUP/2010Feb09/Modified/To fix Issue no 6 reported by TMSC
#include <Probe/Common.h>


class CSocketMsg;
class CPqmProcon;
class CPqmPmAcqman;
//Patni-RUP/2009Dec15/Internal defect fix
class CPqmProbeServer;


typedef struct thres_hold {
    float l_min_thres_hold;
    float l_max_thres_hold;

    thres_hold() : l_min_thres_hold(0), l_max_thres_hold(0) { }

} thres_hold_t;


class CPqmProbe : private MR::ACQ::SM::COMMON::NonCopyable<CPqmProbe>
{
public:

    //Constrcutor / Destrcutor
    CPqmProbe();
    explicit CPqmProbe(CPqmProcon*);
    virtual ~CPqmProbe();


    //Patni-Hemant/10Nov2010/Modified/TMSC-REDMINE-946
    bool RefProbeScanStart()const;
    void RefProbeScanEnd()const;
    void StartProbeClientThread();
    void OnProbeMessage(const CSocketMsg&);
    void OnProbeDisconnect()const;
    bool SendMsgToProbe(proMsg_t*)const;
    void StudyPlanInit(char* f_file_name, const int prot)const;
    //Patni-RUP/2010Apr21/Added/To Fix TMSC-REDMINE-68 defect
    void ShowHideProbeWindow(const BOOL& f_show_flag);

    void StopProbeOnError(const bool f_stop_explicit = false);

    static DWORD GetThreadID();
    int  GetProbeMode() const;
    void SetProbeMode(const int f_mode);

    bool GetProbeFlagFromShMemory(BOOL& f_probe_flag)const; //Patni-RUP/2010Jun09/Added/MVC007947

    CPqmProcon*	 const GetPqmProCon()const;


private:

    static DWORD  m_dThreadId;
    static HANDLE m_hThread;
    //Patni-MSN/2009Mar10/Modified/Optimization
    static CPqmProcon* m_procon;
    static int m_refprobe_mode;

    CPqmPmAcqman* m_acqman_ptr;
    //Patni-RUP/2009Dec15/Internal defect fix
    CPqmProbeServer* m_server;
    bool m_show_probe_dlg;  //Patni-RUP/2010Jun09/Added/MVC007947

    void OnRefProbeDisconnect()const;
    void CreatePQMServer(void);

    static DWORD WINAPI ThreadFunc(LPVOID f_lpparam);

    CString GetMsgStrFromCode(const int f_msg_code)const;
    //Patni-RUP/2010Feb09/Modified/To fix Issue no 6 reported by TMSC
    bool    GetShrMemSegDataForProbe(shared_mem_data_t* const f_shared_mem_data)const;

    BOOL SetAcqmanPointer(CPqmPmAcqman* f_acqman_ptr);
};

#endif // #ifndef __SM_IPCLIB_PQMPROBE_H__ 

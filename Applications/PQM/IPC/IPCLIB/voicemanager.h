//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-780
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: voicemanager.h
 *  Overview: Structure of CVoiceManager class.
 *  Last Modified: 2008/5/20 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2007/6/15 12:00:00     PQM5 Baselined code
 *   2.0          Patni            2007/8/10 12:00:00     Implemented PQM6 requirements & Formatted
 *   3.0          Patni/LK         2008/5/10 12:00:00     Code restructured
 *****************************************************************************/
#ifndef __SM_IPCLIB_VOICEMANAGER_H__
#define __SM_IPCLIB_VOICEMANAGER_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include <Pcommlib/SocketMsg.h>
#include <libStudyManager/NonCopyable.h>
//Patni-Sribanta/2011Jan14/Added/TMSC-REDMINE-1026
#include <PQM/VoiceMgr/VoiceMgr.h>

//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"

const int VOICE_FREQUENCY = 10;

typedef enum {
    VM_TIME_OUT = -1,
    VM_NOT_INITIALIZED = 0,
    VM_INITIALIZED,
    VM_ERROR,
    VM_NO_ERROR
} VM_STATUS;

class CPqm;
class CPQMSocket;

class CVoiceManager : private MR::ACQ::SM::COMMON::NonCopyable<CVoiceManager>
{
public:

    static CVoiceManager* GetInstance();

    virtual ~CVoiceManager();

    //Patni-HAR/2010Jun30/Added/V1.30#TMSC-REDMINE-385
    //Patni-Sribanta/2011Jan14/Modified/TMSC-REDMINE-1026
    VM_STATUS CmdVolume(int f_volume);
    //Patni-PJS/2009Sep23/Added/MVC4941
    //Patni-Sribanta/2011Jan14/Commented/TMSC-REDMINE-1026
    //VM_STATUS InitializeTicks(const int f_dynamic_delay, const int f_tp1_delay);
    //Patni-PJS/2009Sep23/Added/MVC4941

    VM_STATUS InitializeVoiceMgr(const int f_mode, const int f_volume, const int f_dynamic_delay, const int f_tp1_delay, CPqm* f_ppqm = NULL);  //Patni-Raj/2010Jan14/Added/PH#3#/Debug Window

    // To send the Play command to VoiceMgr
    VM_STATUS CmdPlay(const int f_voice_param, const BOOL f_sync_cmd = FALSE);

    // To send the Delay command to VoiceMgr
    VM_STATUS CmdDelay(const int f_av_interval, const int f_pln_autovoice, const BOOL f_sync_cmd = FALSE);

    // To send the Trap command to VoiceMgr
    VM_STATUS CmdTrap(const int f_tp1_delay, const int f_tp1_duration, const int f_parameter, const int f_pln_autovoice, const BOOL f_sync_cmd = FALSE);

    // To send the PDelay command to VoiceMgr
    VM_STATUS CmdPDelay(const int f_acqtime, const int f_period, const int f_paramater, const int f_pln_autovoice, const BOOL f_sync_cmd = FALSE);

    // To send the Prog command to VoiceMgr
    VM_STATUS CmdProg(const int f_offset, const int f_delaytime, const int f_pln_autovoice, const BOOL f_sync_cmd = FALSE);

    //Patni-DKH/2009Nov11/Added/Auto Voice Defect Fix - MVC004942,MVC004366
    // To send the quit command to VoiceMgr
    VM_STATUS CmdQuit();

    //Patni-Sribanta/2011Jan11/Added/TMSC-REDMINE-1026
    static void DestroyVoiceInstance();

    // get the length of voice of index id
    int GetVoiceLength(const int f_index)const;

    void QuitVoiceManager();


    int GetAvDynamicDelay() const;
    int GetAvTp1Delay()	const;
    void InitializeTicks();
    // call back function for receiving Voice Manager msgs
    //+Patni-Sribanta/2011Jan14/Commented/TMSC-REDMINE-1026
    //void OnVoiceMgrMsg(CSocketMsg msg);
    //int SafeExit();
    //-Patni-Sribanta/2011Jan14/Commented/TMSC-REDMINE-1026

protected:

private:

    static CVoiceManager*   m_voicemgr;

    CPqm* m_ppqm;  //Patni-Raj/2010Jan14/Added/PH#3#/Debug Window
    //Patni-Sribanta/2011Jan14/Commented/TMSC-REDMINE-1026
    CVoiceMgr               m_voice_mgr;
    int                     m_ticks[PQM_AV_NUM_VOICES];
    int	m_av_dynamic_delay;
    int	m_av_tp1_delay;

    CVoiceManager();

    BOOL AvAction(VOICEMGR_CMD cmd, ...);
    bool UpdateVoiceInfoFile();

    void setAvDynamicDelay(const int f_av_dynamic_delay);
    void SetAvTp1Delay(const int f_av_tp1_delay);

    //+Patni-Sribanta/2011Jan14/Commented/TMSC-REDMINE-1026
    /*
    VM_STATUS InitializeProcess();
    int WaitforDoneMsg(int f_timeout);

    //Patni-PJS/2009Sep3/Added/Code Review
    CRITICAL_SECTION m_voice_lock;

    //+Patni-PJS/2009Oct9/Added/MVC05123
    VM_STATUS CreateChildProcess(HANDLE f_hchildstd_out_wr);
    VM_STATUS ReadFromPipe(HANDLE f_hchildstd_out_wr, HANDLE f_hchildstd_out_rd);
    //-Patni-PJS/2009Oct9/Added/MVC05123
    */
    //BOOL                    m_is_voicemgr_initialized;
    //int                     m_mode;
    //int                     m_volume;
    //CPQMSocket*             m_voicemgr_socket;
    //-Patni-Sribanta/2011Jan14/Commented/TMSC-REDMINE-1026

    //Patni-Sribanta/2011Jan14/Commented/TMSC-REDMINE-1026
    //BOOL                    m_is_done_msg_rcvd;
};
#endif // #ifndef __SM_IPCLIB_VOICEMANAGER_H__

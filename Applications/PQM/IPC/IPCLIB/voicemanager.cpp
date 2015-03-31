//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-778
//Redmine-779
//Redmine-780
//Redmine-781
//Redmine-782
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: voicemanager.cpp
 *  Overview: Implementation of CVoiceManager class.
 *  Last Modified: 2007/8/10 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2007/6/15 12:00:00     PQM5 Baselined code
 *   2.0          Patni            2007/8/10 12:00:00     Implemented PQM6 requirements & Formatted
 *   3.0          Patni/LK         2008/5/10 12:00:00     Code restructured
 *****************************************************************************/
#include "StdAfx.h"
#include "VoiceManager.h"
#include <PQM/PQMLogMgr.h>
#include <PQM/PqmUtilities.h>
#include "pqmconfig.h"
#include "PQMDebugInfo.h"
#include "Pqm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CVoiceManager* CVoiceManager::m_voicemgr = NULL;

//********************************Method Header********************************
//Method Name   :GetInstance()
//Author        :PATNI/LK
//Purpose       :Get Single Instance of CVoiceManager
//*****************************************************************************
CVoiceManager* CVoiceManager::GetInstance(
)
{
    if (m_voicemgr == NULL) {
        m_voicemgr = new CVoiceManager;
    }

    return m_voicemgr;
}

//***************************Method Header*************************************
//Method Name    : ~CVoiceManager()
//Author         : PATNI/HAR
//Purpose        : Destruction
//*****************************************************************************
CVoiceManager::~CVoiceManager(
)
{
    LPCTSTR FUNC_NAME = _T("CVoiceManager::~CVoiceManager");
    PQM_TRACE1(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of CVoiceManager::~CVoiceManager"));
}

//+Patni-HAR/2010Jun30/Added/V1.30#TMSC-REDMINE-385
//********************************Method Header********************************
//Method Name   :CmdVolume()
//Author        :PATNI/DKH
//Purpose       :sends PQM_AV_CMD_QUIT command to voicemanager
//*****************************************************************************
VM_STATUS CVoiceManager::CmdVolume(
    int f_volume		//Patni-Sribanta/2011Jan14/Added/TMSC-REDMINE-1026
)
{
    //Patni-PJS/2010Jul09/Added/V1.30#TMSC-REDMINE-385/Code Review
    LPCTSTR FUNC_NAME = _T("CVoiceManager::CmdVolume");
    CString l_log_string(_T(""));

    //Patni-Sribanta/2011Jan14/Modified/TMSC-REDMINE-1026
    l_log_string.Format(_T("[Volume : %d]"), f_volume);

    //Patni-PJS/2010Jul09/Modified/V1.30#TMSC-REDMINE-385
    //Patni-PJS/2010Jul09/Added/V1.30#TMSC-REDMINE-385/Code Review
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_string);

    //Patni-Sribanta/2011Jan14/Modified/TMSC-REDMINE-1026
    if (!AvAction(PQM_AV_CMD_VOLUME, f_volume)) {
        return VM_ERROR;
    }

    return VM_NO_ERROR;
}
//-Patni-HAR/2010Jun30/Added/V1.30#TMSC-REDMINE-385

//***************************Method Header*************************************
//Method Name    : InitializeVoiceMgr
//Author         : PATNI/LK
//Purpose        : Spawns VoiceMgr application and connects it with socket
//*****************************************************************************
VM_STATUS CVoiceManager::InitializeVoiceMgr(
    const int f_mode,
    const int f_volume,
    const int f_dynamic_delay,
    const int f_tp1_delay,
    CPqm* f_ppqm  //Patni-Raj/2010Jan14/Added/PH#3#/Debug Window
)
{
    //Patni-PJS/2010Jul09/Added/V1.30#TMSC-REDMINE-385/Code Review
    LPCTSTR FUNC_NAME = _T("CVoiceManager::InitializeVoiceMgr");
    //+Patni-HAR/2010Jun30/Added/V1.30#TMSC-REDMINE-385
    CString l_log_string(_T(""));
    l_log_string.Format(_T("[Mode : %d][Volume : %d][Dynamic Delay : %d]\
        [TP1 Delay : %d]"), f_mode, f_volume, f_dynamic_delay, f_tp1_delay);
    //Patni-PJS/2010Jul09/Added/V1.30#TMSC-REDMINE-385/Code Review
    PQM_TRACE(COMMHANDLER_MARKER_PQM_VOICE, FUNC_NAME, l_log_string);
    //-Patni-HAR/2010Jun30/Added/V1.30#TMSC-REDMINE-385

    VM_STATUS l_status = VM_INITIALIZED;
    m_ppqm = f_ppqm;//Patni-Raj/2010Jan14/Added/PH#3#/Debug Window

    //Patni-Sribanta/2011Jan14/Added/TMSC-REDMINE-1026
    m_voice_mgr.InitVoiceFileData(); //Initilize Voice File Data
    m_voice_mgr.GetVoiceTicks(m_ticks, PQM_AV_NUM_VOICES);

    setAvDynamicDelay(f_dynamic_delay);
    SetAvTp1Delay(f_tp1_delay);
    CmdVolume(f_volume);

    if (!UpdateVoiceInfoFile()) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_VOICE, FUNC_NAME, _T("Updation to Info file failed."));
    }

    return l_status;
}

//********************************Method Header********************************
//Method Name   :CmdPlay()
//Author        :PATNI/LK
//Purpose       :sends PQM_AV_CMD_PLAY command  if not sync and sends
//                  PQM_AV_CMD_PLAY_W command in sync mode
//*****************************************************************************
VM_STATUS CVoiceManager::CmdPlay(
    const int f_voice_param,
    const BOOL f_sync_cmd /* = FALSE */
)
{
    //Patni-PJS/2010Jul09/Added/V1.30#TMSC-REDMINE-385/Code Review
    LPCTSTR FUNC_NAME = _T("CVoiceManager::CmdPlay");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_VOICE, FUNC_NAME, _T("Trace of CmdPlay"));

    //+Patni-Sribanta/2011Jan11/Modified/TMSC-REDMINE-1026
    if (!AvAction(f_sync_cmd ? PQM_AV_CMD_PLAY_W : PQM_AV_CMD_PLAY , f_voice_param)) {
        return VM_ERROR;
    }

    //-Patni-Sribanta/2011Jan11/Modified/TMSC-REDMINE-1026

    return VM_NO_ERROR;
}

//********************************Method Header********************************
//Method Name   :CmdDelay()
//Author        :PATNI/LK
//Purpose       :sends PQM_AV_CMD_DELAY command if not sync and sends
//                  PQM_AV_CMD_DELAY_W command in sync mode
//*****************************************************************************
VM_STATUS CVoiceManager::CmdDelay(
    const int f_av_interval,
    const int f_pln_autovoice,
    const BOOL f_sync_cmd /* = FALSE */
)
{
    //+Patni-PJS/2010Jul09/Added/V1.30#TMSC-REDMINE-385/Code Review
    LPCTSTR FUNC_NAME = _T("CVoiceManager::CmdDelay");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_VOICE, FUNC_NAME, _T("Trace of CmdDelay"));
    //-Patni-PJS/2010Jul09/Added/V1.30#TMSC-REDMINE-385/Code Review

    //+Patni-Sribanta/2011Jan11/Modified/TMSC-REDMINE-1026
    if (!AvAction(f_sync_cmd ? PQM_AV_CMD_DELAY_W : PQM_AV_CMD_DELAY, f_av_interval, f_pln_autovoice)) {
        return VM_ERROR;
    }

    //-Patni-Sribanta/2011Jan11/Modified/TMSC-REDMINE-1026

    return VM_NO_ERROR;
}

//********************************Method Header********************************
//Method Name   :CmdTrap()
//Author        :PATNI/LK
//Purpose       :sends PQM_AV_CMD_TRAP command  if not sync and sends
//                  PQM_AV_CMD_TRAP_W command in sync mode
//*****************************************************************************
VM_STATUS CVoiceManager::CmdTrap(
    const int f_tp1_delay,
    const int f_tp1_duration,
    const int f_parameter,
    const int f_pln_autovoice,
    const BOOL f_sync_cmd /* = FALSE */
)
{
    //+Patni-PJS/2010Jul09/Added/V1.30#TMSC-REDMINE-385/Code Review
    LPCTSTR FUNC_NAME = _T("CVoiceManager::CmdTrap");
    //Patni-Dhanesh/2009Nov04/MOdified/MVC004746-Code Review
    PQM_TRACE(COMMHANDLER_MARKER_PQM_VOICE, FUNC_NAME, _T("Trace of CmdTrap"));

    //-Patni-PJS/2010Jul09/Added/V1.30#TMSC-REDMINE-385/Code Review

    //+Patni-Sribanta/2011Jan11/Modified/TMSC-REDMINE-1026
    if (!AvAction(f_sync_cmd ? PQM_AV_CMD_TRAP_W : PQM_AV_CMD_TRAP,
                  f_tp1_delay,
                  f_tp1_duration,
                  f_parameter,
                  f_pln_autovoice)) {

        return VM_ERROR;
    }

    //-Patni-Sribanta/2011Jan11/Modified/TMSC-REDMINE-1026


    return VM_NO_ERROR;
}

//********************************Method Header********************************
//Method Name   :CmdPDelay()
//Author        :PATNI/LK
//Purpose       :sends PQM_AV_CMD_PDELAY command  if not sync and sends
//                  PQM_AV_CMD_PDELAY_W command in sync mode
//*****************************************************************************
VM_STATUS CVoiceManager::CmdPDelay(
    const int f_acqtime,
    const int f_period,
    const int f_paramater,
    const int f_pln_autovoice,
    const BOOL f_sync_cmd /* = FALSE */
)
{
    //+Patni-PJS/2010Jul09/Added/V1.30#TMSC-REDMINE-385/Code Review
    LPCTSTR FUNC_NAME = _T("CVoiceManager::CmdPDelay");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_VOICE, FUNC_NAME, _T("Trace of CmdPDelay"));
    //-Patni-PJS/2010Jul09/Added/V1.30#TMSC-REDMINE-385/Code Review

    //+Patni-Sribanta/2011Jan11/Modified/TMSC-REDMINE-1026

    if (! AvAction(f_sync_cmd ? PQM_AV_CMD_PDELAY_W : PQM_AV_CMD_PDELAY,
                   f_acqtime,
                   f_period,
                   f_paramater,
                   f_pln_autovoice)) {
        return VM_ERROR;
    }

    //-Patni-Sribanta/2011Jan11/Modified/TMSC-REDMINE-1026

    return VM_NO_ERROR;
}

//********************************Method Header********************************
//Method Name   :CmdProg()
//Author        :PATNI/LK
//Purpose       :sends PQM_AV_CMD_PROG command  if not sync and sends
//                  PQM_AV_CMD_PROG_W command in sync mode
//*****************************************************************************
VM_STATUS CVoiceManager::CmdProg(
    const int f_offset,
    const int f_delaytime,
    const int f_pln_autovoice,
    const BOOL f_sync_cmd /* = FALSE */
)
{
    //+Patni-PJS/2010Jul09/Added/V1.30#TMSC-REDMINE-385/Code Review
    LPCTSTR FUNC_NAME = _T("CVoiceManager::CmdProg");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_VOICE, FUNC_NAME, _T("Trace of CmdProg"));
    //-Patni-PJS/2010Jul09/Added/V1.30#TMSC-REDMINE-385/Code Review

    //+Patni-Sribanta/2011Jan11/Modified/TMSC-REDMINE-1026
    if (! AvAction(f_sync_cmd ? PQM_AV_CMD_PROG_W : PQM_AV_CMD_PROG,
                   f_offset,
                   f_delaytime,
                   f_pln_autovoice)) {
        return VM_ERROR;
    }

    //-Patni-Sribanta/2011Jan11/Modified/TMSC-REDMINE-1026

    return VM_NO_ERROR;
}

//+Patni-DKH/2009Nov11/Added/Added method, it may be used later
//********************************Method Header********************************
//Method Name   :CmdQuit()
//Author        :PATNI/DKH
//Purpose       :sends PQM_AV_CMD_QUIT command to voicemanager
//*****************************************************************************
VM_STATUS CVoiceManager::CmdQuit()
{
    //+Patni-PJS/2010Jul09/Added/V1.30#TMSC-REDMINE-385/Code Review
    LPCTSTR FUNC_NAME = _T("CVoiceManager::CmdQuit");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_VOICE, FUNC_NAME, _T("Trace of CmdQuit"));

    //-Patni-PJS/2010Jul09/Added/V1.30#TMSC-REDMINE-385/Code Review
    if (!AvAction(PQM_AV_CMD_QUIT)) {
        return VM_ERROR;
    }

    return VM_NO_ERROR;
}
//+Patni-DKH/2009Nov11/Added/Added method, it may be used later

//+Patni-Sribanta/2011Jan11/Added/TMSC-REDMINE-1026
void CVoiceManager::DestroyVoiceInstance()
{

    DEL_PTR(m_voicemgr);
}
//-Patni-Sribanta/2011Jan11/Added/TMSC-REDMINE-1026

//***************************Method Header*************************************
//Method Name    : GetVoiceLength()
//Author         : PATNI/HAR
//Purpose        :
//*****************************************************************************
int CVoiceManager::GetVoiceLength(
    const int f_index
)const
{
    //Patni-PJS/2010Jul09/Added/V1.30#TMSC-REDMINE-385/Code Review
    LPCTSTR FUNC_NAME = _T("CVoiceManager::GetVoiceLength");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of AvGetLength"));

    if (f_index < 1 || f_index > PQM_AV_NUM_VOICES) {
        return -1;

    } else {
        return (m_ticks[f_index - 1] * VOICE_FREQUENCY);
    }
}


//***************************Method Header*************************************
//Method Name    : QuitVoiceManager()
//Author         : PATNI/
//Purpose        : Calling for Termenating Thread of VoiceManager
//*****************************************************************************
void CVoiceManager::QuitVoiceManager()
{
    LPCTSTR FUNC_NAME = _T("CVoiceManager::QuitVoiceManager");
    PQM_TRACE1(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    m_voice_mgr.QuitCommand();
}
//***************************Method Header*************************************
//Method Name    : GetAvDynamicDelay()
//Author         : PATNI/Sribanta
//Purpose        :
//*****************************************************************************
int CVoiceManager::GetAvDynamicDelay(
)const
{
    return m_av_dynamic_delay;
}
//***************************Method Header*************************************
//Method Name    : GetAvTp1Delay()
//Author         : PATNI/Sribanta
//Purpose        :
//*****************************************************************************
int CVoiceManager::GetAvTp1Delay(
)const
{
    return m_av_tp1_delay;
}

//***************************Method Header*************************************
//Method Name    : CVoiceManager()
//Author         : PATNI/HAR
//Purpose        : Construction
//*****************************************************************************
CVoiceManager::CVoiceManager(
):	m_ppqm(NULL)
{
    LPCTSTR FUNC_NAME = _T("CVoiceManager::CVoiceManager");
    PQM_TRACE1(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of CVoiceManager::CVoiceManager"));

    for (int i = 0; i < PQM_AV_NUM_VOICES; i++) {
        m_ticks[i] = 0;
    }
}

//***************************Method Header*************************************
//Method Name    : AvAction()
//Author         : PATNI/HAR
//Purpose        :
//*****************************************************************************
BOOL CVoiceManager::AvAction(
    VOICEMGR_CMD cmd,
    ...
)
{
    //+Patni-PJS/2010Jul09/Added/V1.30#TMSC-REDMINE-385/Code Review
    LPCTSTR FUNC_NAME = _T("CVoiceManager::AvAction");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_VOICE, FUNC_NAME,
              _T("Trace of AvAction"));
    //-Patni-PJS/2010Jul09/Added/V1.30#TMSC-REDMINE-385/Code Review

    char        buf[VOICEMANAGER_BUFFER] = {0};
    BOOL        status = TRUE;
    va_list     args = NULL;
    CString     l_tracemsg;
    va_start(args, cmd);

    //+Patni-Sribanta/2011Jan14/Added/TMSC-REDMINE-1026
    int l_sleep1 = 0;
    int l_sleep2 = 0;
    int l_interval = 0;
    int l_voice_file1_num = 0;
    int l_voice_file2_num = 0;
    //-Patni-Sribanta/2011Jan14/Commented/TMSC-REDMINE-1026

    try {
        m_ppqm->WriteToUI(PQM_MSG_BEGINWAIT_CURSOR_PQM_BUTTONS);

        switch (cmd) {
            case PQM_AV_CMD_QUIT:
                sprintf(buf, "quit\n");
                m_voice_mgr.QuitCommand();
                l_tracemsg.Format(_T("quit command"));
                break;

            case PQM_AV_CMD_PLAY:
                vsprintf(buf, "play %d\n", args);
                l_tracemsg.Format(_T("play command"));
                //Patni-Sribanta/2011Jan14/Added/Modified/TMSC-REDMINE-1026
                m_voice_mgr.PlayCommandNoWait(va_arg(args , int));
                break;

            case PQM_AV_CMD_PLAY_W:
                vsprintf(buf, "playw %d\n", args);
                l_tracemsg.Format(_T("play command"));
                //Patni-Sribanta/2011Jan14/Added/Modified/TMSC-REDMINE-1026
                m_voice_mgr.PlayCommand(va_arg(args , int));
                break;

            case PQM_AV_CMD_DELAY:
                vsprintf(buf, "delay %d %d\n", args);
                l_tracemsg.Format(_T("delay command"));
                //+Patni-Sribanta/2011Jan14/Added/Modified/TMSC-REDMINE-1026
                l_sleep1 = va_arg(args , int);
                l_voice_file1_num = va_arg(args , int);
                m_voice_mgr.DelayCommandNoWait(l_sleep1, l_voice_file1_num);
                //-Patni-Sribanta/2011Jan14/Added/Modified/TMSC-REDMINE-1026
                break;

            case PQM_AV_CMD_DELAY_W:
                vsprintf(buf, "delayw %d %d\n", args);
                l_tracemsg.Format(_T("delayw command"));
                //+Patni-Sribanta/2011Jan14/Added/Modified/TMSC-REDMINE-1026
                l_sleep1 = va_arg(args , int);
                l_voice_file1_num = va_arg(args , int);
                m_voice_mgr.DelayCommand(l_sleep1, l_voice_file1_num);
                //-Patni-Sribanta/2011Jan14/Added/Modified/TMSC-REDMINE-1026
                break;

            case PQM_AV_CMD_VOLUME:
                vsprintf(buf, "volume %d\n", args);
                l_tracemsg.Format(_T("volume command"));
                //Patni-Sribanta/2011Jan14/Added/Modified/TMSC-REDMINE-1026
                m_voice_mgr.SetVolume(va_arg(args , int));
                break;

            case PQM_AV_CMD_VOLUME_W:
                vsprintf(buf, "volumew %d\n", args);
                l_tracemsg.Format(_T("volumew command"));
                //Patni-Sribanta/2011Jan14/Added/Modified/TMSC-REDMINE-1026
                m_voice_mgr.SetVolume(va_arg(args , int));
                break;

            case PQM_AV_CMD_PROG:
                vsprintf(buf, "prog %d %d %d\n", args);
                l_tracemsg.Format(_T("prog command"));
                //+Patni-Sribanta/2011Jan14/Added/Modified/TMSC-REDMINE-1026
                l_interval = va_arg(args , int);
                l_voice_file1_num = va_arg(args , int);
                l_voice_file2_num = va_arg(args , int);
                m_voice_mgr.ProgCommandNoWait(l_interval, l_voice_file1_num, l_voice_file2_num);
                //-Patni-Sribanta/2011Jan14/Added/Modified/TMSC-REDMINE-1026
                break;

            case PQM_AV_CMD_PROG_W:
                vsprintf(buf, "progw %d %d %d\n", args);
                l_tracemsg.Format(_T("progw command"));
                //+Patni-Sribanta/2011Jan14/Added/Modified/TMSC-REDMINE-1026
                l_interval = va_arg(args , int);
                l_voice_file1_num = va_arg(args , int);
                l_voice_file2_num = va_arg(args , int);
                m_voice_mgr.ProgCommand(l_interval, l_voice_file1_num, l_voice_file2_num);
                //-Patni-Sribanta/2011Jan14/Added/Modified/TMSC-REDMINE-1026
                break;

            case PQM_AV_CMD_PDELAY:
                vsprintf(buf, "pdelay %d %d %d %d\n", args);
                l_tracemsg.Format(_T("pdelay command"));
                //+Patni-Sribanta/2011Jan14/Added/Modified/TMSC-REDMINE-1026
                l_sleep1 = va_arg(args , int);
                l_sleep2 = va_arg(args , int);
                l_voice_file1_num = va_arg(args , int);
                l_voice_file2_num = va_arg(args , int);
                m_voice_mgr.ProgDelayCommandNoWait(l_sleep1, l_sleep2, l_voice_file1_num, l_voice_file2_num);
                //-Patni-Sribanta/2011Jan14/Added/Modified/TMSC-REDMINE-1026
                break;

            case PQM_AV_CMD_PDELAY_W:
                vsprintf(buf, "pdelayw %d %d %d %d\n", args);
                l_tracemsg.Format(_T("pdelayw command"));
                //+Patni-Sribanta/2011Jan14/Added/Modified/TMSC-REDMINE-1026
                l_sleep1 = va_arg(args , int);
                l_sleep2 = va_arg(args , int);
                l_voice_file1_num = va_arg(args , int);
                l_voice_file2_num = va_arg(args , int);
                m_voice_mgr.ProgDelayCommand(l_sleep1, l_sleep2, l_voice_file1_num, l_voice_file2_num);
                //-Patni-Sribanta/2011Jan14/Added/Modified/TMSC-REDMINE-1026
                break;

            case PQM_AV_CMD_INTERVAL:
                vsprintf(buf, "interval %d %d %d\n", args);
                l_tracemsg.Format(_T("interval command"));
                //+Patni-Sribanta/2011Jan14/Added/Modified/TMSC-REDMINE-1026
                l_voice_file1_num = va_arg(args , int);
                l_sleep1 = va_arg(args , int);
                l_voice_file2_num = va_arg(args , int);
                m_voice_mgr.IntervalCommandNoWait(l_voice_file1_num, l_sleep1, l_voice_file2_num);
                //-Patni-Sribanta/2011Jan14/Added/Modified/TMSC-REDMINE-1026
                break;

            case PQM_AV_CMD_INTERVAL_W:
                vsprintf(buf, "intervalw %d %d %d\n", args);
                l_tracemsg.Format(_T("intervalw command"));
                //+Patni-Sribanta/2011Jan14/Added/Modified/TMSC-REDMINE-1026
                l_voice_file1_num = va_arg(args , int);
                l_sleep1 = va_arg(args , int);
                l_voice_file2_num = va_arg(args , int);
                m_voice_mgr.IntervalCommand(l_voice_file1_num, l_sleep1, l_voice_file2_num);
                //-Patni-Sribanta/2011Jan14/Added/Modified/TMSC-REDMINE-1026
                break;

            case PQM_AV_CMD_TRAP:
                vsprintf(buf, "trap %d %d %d %d\n", args);
                l_tracemsg.Format(_T("trap command"));
                //+Patni-Sribanta/2011Jan14/Added/Modified/TMSC-REDMINE-1026
                l_sleep1 = va_arg(args , int);
                l_interval = va_arg(args , int);
                l_voice_file1_num = va_arg(args , int);
                l_voice_file2_num = va_arg(args , int);
                m_voice_mgr.TrapCommandNoWait(l_sleep1, l_interval, l_voice_file1_num, l_voice_file2_num);
                //-Patni-Sribanta/2011Jan14/Added/Modified/TMSC-REDMINE-1026
                break;

            case PQM_AV_CMD_TRAP_W:
                vsprintf(buf, "trapw %d %d %d\n", args);
                l_tracemsg.Format(_T("trapw command"));
                //+Patni-Sribanta/2011Jan14/Added/Modified/TMSC-REDMINE-1026
                l_sleep1 = va_arg(args , int);
                l_interval = va_arg(args , int);
                l_voice_file1_num = va_arg(args , int);
                l_voice_file2_num = va_arg(args , int);
                m_voice_mgr.TrapCommand(l_sleep1, l_interval, l_voice_file1_num, l_voice_file2_num);
                //-Patni-Sribanta/2011Jan14/Added/Modified/TMSC-REDMINE-1026
                break;

            default:
                status = FALSE;
                break;
        }

        va_end(args);

        m_ppqm->WriteToUI(PQM_MSG_ENDWAIT_CURSOR_PQM_BUTTONS);

        if (!status) {
            return status;
        }

        //+Patni-PJS/2009Sep2/Added/MVC004366 Code Review
        CString l_cs_buffer = CString(buf);
        //+Patni-MP/2010Jul20/Added/REDMINE-385 ITT Defect
        l_cs_buffer = l_cs_buffer.Left(l_cs_buffer.GetLength() - 1);
        l_tracemsg += _T(" VoiceManagerBuf---->") + l_cs_buffer;

        //+Patni-Raj/2010Jan14/Added/PH#3#/Debug Window
        CPQMDebugInfo::GetInstance(m_ppqm)->PqmDebugAcqMonitor(_T("AutoVoice : %s"), l_cs_buffer);
        //-Patni-Raj/2010Jan14/Added/PH#3#/Debug Window

        //-Patni-PJS/2009Sep2/Added/MVC004366 Code Review
        //Patni-PJS/2010Jul09/Added/V1.30#TMSC-REDMINE-385/Code Review
        PQM_TRACE(COMMHANDLER_MARKER_PQM_VOICE, FUNC_NAME, l_tracemsg);

    } catch (...) {
        m_ppqm->WriteToUI(PQM_MSG_ENDWAIT_CURSOR_PQM_BUTTONS);
        PQM_TRACE(COMMHANDLER_MARKER_PQM_VOICE, FUNC_NAME, _T("Exception occured. "));
        throw;
    }

    return TRUE;
}
//***************************Method Header*************************************
//Method Name    : UpdateVoiceInfoFile()
//Author         : PATNI/Sribanta
//Purpose        :
//*****************************************************************************
bool CVoiceManager::UpdateVoiceInfoFile()
{
    char* l_path = NULL;
    char l_filepath[128] = {0};
    int l_cnt = 0;
    FILE* fp = NULL;

    if ((l_path = getenv("MRMP_GP_DATA")) != NULL) {
        sprintf(l_filepath, "%s\\Voice\\Info", l_path);

        if ((fp = fopen(l_filepath, "w")) != NULL) {
            for (l_cnt = 0; l_cnt < 5; l_cnt++) {
                fprintf(fp, "voice%d\t%d\n", l_cnt + 1, m_ticks[l_cnt] * 10);
            }

            fprintf(fp, "Dynamic\t%d\n", m_av_dynamic_delay);
            fprintf(fp, "TPCTRL\t%d\n", m_av_tp1_delay);
            fclose(fp);

        } else {
            return false;
        }

    } else {
        return false;
    }

    return true;
}
//***************************Method Header*************************************
//Method Name    : setAvDynamicDelay()
//Author         : PATNI/Sribanta
//Purpose        :
//*****************************************************************************
void CVoiceManager::setAvDynamicDelay(const int f_av_dynamic_delay
                                     )
{
    m_av_dynamic_delay = f_av_dynamic_delay;
}
//***************************Method Header*************************************
//Method Name    : SetAvTp1Delay()
//Author         : PATNI/Sribanta
//Purpose        :
//*****************************************************************************
void CVoiceManager::SetAvTp1Delay(const int f_av_tp1_delay
                                 )
{
    m_av_tp1_delay = f_av_tp1_delay;
}
//***************************Method Header*************************************
//Method Name    : InitializeTicks()
//Author         : PATNI/Sribanta
//Purpose        :
//*****************************************************************************
void CVoiceManager::InitializeTicks()
{

    try {

        m_voice_mgr.UpdateVoiceFileData();
        m_voice_mgr.GetVoiceTicks(m_ticks, PQM_AV_NUM_VOICES);

        if (!UpdateVoiceInfoFile()) {
            PQM_TRACE(COMMHANDLER_MARKER_PQM_VOICE, _T("InitializeTicks"), _T("Updation to Info file failed."));
        }

    } catch (...) {
        CString	l_print_string = _T("");
        l_print_string.Format(_T("Exception occurred while calling voice functions"));

        PQM_TRACE(COMMHANDLER_MARKER_PQM_VOICE,
                  _T("CVoiceManager::InitializeTicks"), l_print_string);
        throw;
    }
}
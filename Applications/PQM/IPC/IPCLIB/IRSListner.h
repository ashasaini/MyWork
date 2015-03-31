
//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-780
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: IRSListner.h
 *  Overview: Structure of CIRSListner class.
 *  Last Modified: 2008/5/30 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2007/6/15 12:00:00     PQM5 Baselined code
 *   2.0          Patni            2007/8/10 12:00:00     Implemented PQM6 requirements & Formatted
 *  3.0          Patni/LK          2008/5/30 12:00:00     Restructured after
                                                            TA comments
 *****************************************************************************/
#ifndef __SM_IPCLIB_IRSLISTNER_H__
#define __SM_IPCLIB_IRSLISTNER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <tami/libvf/varfields.h>
extern "C" {
#include <tami/libIrs/Irs.h>
}
#include <libStudyManager/NonCopyable.h>
#include <PQM/PQMCommanMessages.h>
#include "pqmmessagestructures.h"


const int INVALID_FD = -1;

class CPqm;

class CIRSListner : private MR::ACQ::SM::COMMON::NonCopyable<CIRSListner>
{
public:
    //Constructor / Destructor
    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    //CIRSListner(CPqm* f_pqmptr);
    explicit CIRSListner(CPqm* f_pqm);  //Patni-KSS/2011Feb17/Modified/CPP Test tool Execution
    virtual ~CIRSListner();

    // Initialize Create Listener thread and starts listening
    BOOL InitializeIRS();

    //Patni-DKH/2010Jan27/Added/PH#3#/IR86/DebugWindow
    int GetIRSSocketFD()const;

private:

    //Patni-AMT/2009Oct16/Added/Locator button issue
    int                     m_bIndexToPass;
    int                     m_process_node;
    CPqm*                   m_parent_pqmptr;
    int                     m_irs_socket;
    HANDLE                  m_irslisner_thread;


    status_t IRSRegisterReconDone()const;
    // call back function when IRS starts communicating with PQM
    void IRSEventHandler();
    void pqmIrsBDMHandler(IRS_EventT*)const;
    // functions are not being used but are ported from base code
    status_t IrsRegisterButtons()const;

    //Patni-KSS/2010Oct08/Added/V1.35#IR-167
    void ProcessProbeMsgForIRS(const pqm_msg_tag f_msg)const;
    CString PrintPath(VfPathElem_t*	pathP, int depth)const;

    void IrsNotifyScanStart(study_id_t* f_study_id)const;

    //void IrsNotifyImageAdded(study_id_t *f_study_id, VfPathElem_t *f_path, int f_depth);
    //void IrsNotifyLastImageDone(study_id_t  *f_study_id, VfPathElem_t *f_path, int f_depth);

    // Thread function which will continously listening
    static DWORD WINAPI IRSListnerThread(LPVOID f_parameter);
    //static void CALLBACK TimerProcedure(void* hwnd,  uint msg, uint id_prt, ulong dwTime);

    CString GetStudyLoidFromCFWIRSMsg(const char* f_irs_msg) const;

};

#endif // #ifndef __SM_IPCLIB_IRSLISTNER_H__ 

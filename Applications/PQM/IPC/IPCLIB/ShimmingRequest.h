//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-780
//Redmine-782
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: ShimmingRequest.h
 *  Overview: Structure of CShimmingRequest class.
 *  Last Modified:
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2008/6/02 12:00:00     New class Implemented
                                                            After Restructure
 *
 *****************************************************************************/

#ifndef __SM_IPCLIB_SHIMMINGREQUEST_H__
#define __SM_IPCLIB_SHIMMINGREQUEST_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef enum {
    SHIMMING_ADJUST_DONE = 0,
    SHIMMING_CANCEL
} SR_STATUS;

extern "C" {
#include <tami/libvf/varfields.h>
}

#include <tami/libmsintf/msintf.h>
#include <libStudyManager/NonCopyable.h>
#include "ShimmingRequestStructures.h"

class CPQMStudy;

class CShimmingRequest : private MR::ACQ::SM::COMMON::NonCopyable<CShimmingRequest>
{
public:

    //Constrcutor / Destrcutor
    CShimmingRequest();
    explicit CShimmingRequest(CPQMStudy* f_pqmstudy);
    virtual ~CShimmingRequest();

    SR_STATUS ProcessShimmingRequest(const SShimReqInfo_t* f_shimreq_info);
    void SendLocalShimReqTomsShim(msInteractiveShim_t* f_shim_data);


    static void ShimmingTimerReset(bool f_Timeout = false);
    static void ShimmingTimerSetup();


private:

    static unsigned m_shimming_timeout;

    CPQMStudy*      m_parent_study;
    //Patni-HAR/2010Feb18/Added/MSA0251-00176

    // Initialize Shim Req structure by default values
    void InitializeShimReqInfo(/*SShimGlobal_t* f_shimglobal*/);

    // Prepare Shimming Request Info structure before communicating with msShim
    BOOL PrepareShimReqInfo(const SShimReqInfo_t* f_shimreq_info/*, SShimGlobal_t* f_shimglobal*/);

    // requests msShim to communicate
    int msShimReq(/*SShimGlobal_t* f_shimglobal*/)const;

    // starts msShim
    BOOL msShimRun(/*SShimGlobal_t* f_shimglobal*/)const;

    // after getting the reply from msShim, puts Shim offset and FSY Value in study file
    BOOL msShimEndProc(const msInteractiveShim_t& f_shimdata/*, SShimGlobal_t* f_shimglobal*/)const;

    // fills VfPathElem_t structure and quits Shimming Request operations
    void QuitShimmingRequest(int f_flag, /*SShimGlobal_t* f_shim_global,*/ VfPathElem_t* f_path_elem)const;

    // VfPathElem_t structure is assigned with msShim reply
    BOOL ComposeAdjustDoneMsg(/*SShimGlobal_t* f_shim_global,*/ VfPathElem_t* f_path_elem)const;

    // Process Shimming Request Response and updates shim shared data
    BOOL ProcessResponse(const SR_STATUS f_shimreq_status, VfPathElem_t* f_path_elem, const int f_shim_flag)const;


    BOOL CheckShimDataFiles()const;


    static void ShimmingTimeOut(int sig);
};


#endif // #ifndef __SM_IPCLIB_SHIMMINGREQUEST_H__

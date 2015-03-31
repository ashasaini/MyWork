/*****************************************************************************
*
*  (c) Copyright 2011 TOSHIBA Corporation. All Rights Reserved
*
*  File Name: DPSManager.h
*  Overview: Structure of DPSManager class.
*  Last Modified:
*****************************************************************************
*                       Revision History
*****************************************************************************
*  Revision      Author           Date & Time            Changes
*  ==========================================================================
*
 *****************************************************************************/
// DPSManager.h: interface for the CDPSManager class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_DPS_MANAGER_H__
#define __SM_DPS_MANAGER_H__
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/Singleton.h>
#include <libStudyManager/CFW/CFWHandler.h>
#include <libStudyManager/CFW/CFWListener.h>

class CDPSManager :
    public MR::ACQ::SM::COMMON::Singleton<CDPSManager>
    , public MR::ACQ::SM::COMMON::CFW::ICFWHandler
    , public MR::ACQ::SM::COMMON::CFW::CCFWListener
{

    friend MR::ACQ::SM::COMMON::Singleton<CDPSManager>;

public:
    bool Initialize();
    bool HaveToShowSR() const;

private:
    bool m_is_recv_GL_SR_ShowPQM;

    CDPSManager();
    virtual ~CDPSManager();

    static BOOL GetFBIDelayInfo(const CString f_param,
                                CString& f_stuydyloid,
                                int& f_delay1,
                                int& f_delay2);


    BOOL RecvCallBack(const MR::ACQ::SM::COMMON::CFW::CCFWReceiveMsg& recvMsg,
                      MR::ACQ::SM::COMMON::CFW::CCFWReplyMsg* const replyMsg,
                      MR::ACQ::SM::COMMON::CFW::CCFWListener* const listener);

    void GLParamSetReadyOpenEvent()const;

};


#endif // #ifndef __SM_DPS_MANAGER_H__

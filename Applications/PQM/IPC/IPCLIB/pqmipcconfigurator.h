//Remine-780
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: pqmipcconfigurator.h
 *  Overview: Structure of CPQMIPCConfigurator class.
 *  Last Modified: 2007/8/10 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2007/6/15 12:00:00     PQM5 Baselined code
 *   2.0          Patni            2007/8/10 12:00:00     Implemented PQM6 requirements & Formatted
 *
 *****************************************************************************/
#ifndef __SM_IPCLIB_PQMIPC_CONFIGURATOR_H__
#define __SM_IPCLIB_PQMIPC_CONFIGURATOR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>

class CPQMIPCConfigurator : private MR::ACQ::SM::COMMON::NonCopyable<CPQMIPCConfigurator>
{
public:
    // Default constructor of CPQMIPCConfigurator
    CPQMIPCConfigurator();
    virtual ~CPQMIPCConfigurator();

    bool IsSkipRGN() const;
    void SetSkipRGN(const bool set);

    CString GetAcqmanChannelName() const;
    CString GetPQMServerChannelName() const;

    float GetRxGainOffset() const;
    void  SetRxGainOffset(const float offset);

    static CString GetConfigFileName();

    //Patni-ARD/2010Mar10/Commented/Ph-3# IR-100 code review
    //static CString GetMapFileName();

private:
    bool m_skip_rgn;
    float m_rxgain_offset;

    CString m_acq_channel_name;
    CString m_pqm_channel_name;

    static CString m_config_file_name;
    static CString m_map_file_name;
};
#endif // #ifndef __SM_IPCLIB_PQMIPC_CONFIGURATOR_H__

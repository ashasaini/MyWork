//TMSC-REDMINE-783
//TMSC-REDMINE-780
//Redmine-775
//Redmine-777
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMGlobal.h
 *  Overview: Structure of CPQMGlobal class.
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
#ifndef __SM_PQM_GLOBAL_H__
#define __SM_PQM_GLOBAL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>

//! The class is used to store global variable for Application

class CPQMGlobal : private MR::ACQ::SM::COMMON::NonCopyable<CPQMGlobal>
{
public:
    CPQMGlobal();
    virtual ~CPQMGlobal();

    void ReSetUtilityMenuDefaultItems();

    bool GetAutoPlanFlag()const;
    void SetAutoPlanFlag(const bool auto_plan);

    bool GetPlanScanFlag()const ;
    void SetPlanScanFlag(const bool plan_scan);

    bool GetSkipRgnFlag()const ;
    void SetSkipRgnFlag(const bool skip_rgn);

    bool GetAllCoverageRgnFlag()const;
    void SetAllCoverageRgnFlag(const bool all_coverage_rgn);

    bool GetSpeedermapAutoVoiceFlag()const;
    void SetSpeedermapAutoVoiceFlag(const bool speedermap_auto_voice);

    bool GetMultiCovCautionFlag()const;
    void SetMultiCovCautionFlag(const bool multi_cov_caution);

    bool GetPrepWbCoilFlag()const;
    void SetPrepWbCoilFlag(const bool prep_wb_coil);

    bool GetShimmWbCoilFlag()const;
    void SetShimmWbCoilFlag(const bool shimm_wb_coil);

    bool GetBatchSettingFlag() const;
    void SetBatchSettingFlag(const bool batch_setting_flag);

    bool GetLocatorMoveFlag()const;
    void SetLocatorMoveFlag(const bool locator_move_flag);

    int GetPQMMode()const;
    void SetPQMMode(const int mode);

    void SetUtilityMenuDefaultItems(const SIPCConfigInfo* f_sIPCConfigInfo);

    bool GetWirelessSystemTypeFlag()const;
    void SetWirelessSystemTypeFlag(const bool mode);

private:
    S_DefaultUtilityMenu m_s_default_utility_menu;

    bool m_locator_move_flag;
    bool m_auto_plan;
    bool m_plan_scan;

    bool m_skip_rgn;
    bool m_all_coverage_rgn;
    bool m_speedermap_auto_voice;

    bool m_multi_cov_caution;


    bool m_prep_wb_coil;

    bool m_shimm_wb_coil;

    bool m_batch_setting_flag ;

    int m_mode;

    bool m_wireless_sys_flag;
};


#endif // #ifndef __SM_PQM_GLOBAL_H__

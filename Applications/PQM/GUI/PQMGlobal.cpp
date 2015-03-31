//TMSC-REDMINE-782
//TMSC-REDMINE-780
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMGlobal.cpp
 *  Overview: Implementation of CPQMGlobal class.
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
#include "stdafx.h"
#include "PQMGlobal.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//************************************Method Header**************************************

//Method Name:  CPQMGlobal

//Author:  PATNI/SVP

//Purpose: Constructor

//**************************************************************************************/
CPQMGlobal::CPQMGlobal(
):	m_locator_move_flag(false),
    m_auto_plan(false),
    m_plan_scan(false),
    m_skip_rgn(false),
    m_all_coverage_rgn(false),
    m_speedermap_auto_voice(false),
    m_multi_cov_caution(false),
    m_prep_wb_coil(false),
    m_shimm_wb_coil(false),
    m_batch_setting_flag(false),
    m_mode(0),
    m_wireless_sys_flag(false)
{
}

//************************************Method Header**************************************

//Method Name:  CPQMGlobal

//Author:  PATNI/SVP

//Purpose: Destructor

//**************************************************************************************/
CPQMGlobal::~CPQMGlobal(
)
{
}

//************************************Method Header**********************************
// Method Name  : ReSetUtilityMenuDefaultItems
// Author       : PATNI/ ARD
// Purpose      : Reset the Utility menu items on selction of study
//***********************************************************************************
void CPQMGlobal::ReSetUtilityMenuDefaultItems()
{
    m_auto_plan = m_s_default_utility_menu.auto_plan;
    m_plan_scan = m_s_default_utility_menu.plan_scan;
    m_prep_wb_coil = m_s_default_utility_menu.prep_WB_coil;
    m_shimm_wb_coil = m_s_default_utility_menu.shimming_WB_coil;
    m_skip_rgn = m_s_default_utility_menu.skip_rgn;
    m_all_coverage_rgn = m_s_default_utility_menu.all_cov_rgn;
    m_batch_setting_flag = m_s_default_utility_menu.batch_setting;
    m_multi_cov_caution = m_s_default_utility_menu.multi_cov_caution;

    //Patni-Hemant/2010Jun7/Added/MaFT/MVC008198
    m_speedermap_auto_voice = m_s_default_utility_menu.speedermap_auto_voice;
}
//-Patni-ARD/2010Mar18/Added/DeFT# IR-100 Default Utility menu items


//************************************Method Header**************************************

//Method Name:  GetAutoPlanFlag

//Author:  PATNI/SVP

//Purpose: Returns auto plan flag

//**************************************************************************************/
bool CPQMGlobal::GetAutoPlanFlag(
)const
{
    return m_auto_plan;
}

//************************************Method Header**************************************

//Method Name:  SetAutoPlanFlag

//Author:  PATNI/SVP

//Purpose: Sets auto plan flag

//**************************************************************************************/
void CPQMGlobal::SetAutoPlanFlag(
    const bool auto_plan
)
{
    m_auto_plan = auto_plan;
}

//************************************Method Header**************************************

//Method Name:  GetPlanScanFlag

//Author:  PATNI/SVP

//Purpose: Returns plan scan flag

//**************************************************************************************/
bool CPQMGlobal::GetPlanScanFlag(
)const
{
    return m_plan_scan;
}

//************************************Method Header**************************************

//Method Name:  SetPlanScanFlag

//Author:  PATNI/SVP

//Purpose: Sets plan scan flag

//**************************************************************************************/
void CPQMGlobal::SetPlanScanFlag(
    const bool plan_scan
)
{
    m_plan_scan = plan_scan;
}

//************************************Method Header**************************************

//Method Name:  GetSkipRgnFlag

//Author:  PATNI/SVP

//Purpose: Returns skip rgn flag

//**************************************************************************************/
bool CPQMGlobal::GetSkipRgnFlag(
)const
{
    return m_skip_rgn;
}

//************************************Method Header**************************************

//Method Name:  SetSkipRgnFlag

//Author:  PATNI/SVP

//Purpose: Sets skip rgn flag

//**************************************************************************************/
void CPQMGlobal::SetSkipRgnFlag(
    const bool skip_rgn
)
{
    m_skip_rgn = skip_rgn;
}

//************************************Method Header**************************************

//Method Name:  GetAllCoverageRgnFlag

//Author:  PATNI/SVP

//Purpose: Returns all coverage rgn flag

//**************************************************************************************/
bool CPQMGlobal::GetAllCoverageRgnFlag(
)const
{
    return m_all_coverage_rgn;
}

//************************************Method Header**************************************

//Method Name:  SetAllCoverageRgnFlag

//Author:  PATNI/SVP

//Purpose: Sets all coverage rgn flag

//**************************************************************************************/
void CPQMGlobal::SetAllCoverageRgnFlag(
    const bool all_coverage_rgn
)
{
    m_all_coverage_rgn = all_coverage_rgn;
}

//************************************Method Header**************************************

//Method Name:  GetSpeedermapAutoVoiceFlag

//Author:  PATNI/SVP

//Purpose: Returns speedermap autovoice flag

//**************************************************************************************/
bool CPQMGlobal::GetSpeedermapAutoVoiceFlag(
)const
{
    return m_speedermap_auto_voice;
}

//************************************Method Header**************************************

//Method Name:  SetSpeedermapAutoVoiceFlag

//Author:  PATNI/SVP

//Purpose: Sets speedermap autovoice flag

//*************************************************************************************/
void CPQMGlobal::SetSpeedermapAutoVoiceFlag(
    const bool speedermap_auto_voice
)
{
    m_speedermap_auto_voice = speedermap_auto_voice;
}

//************************************Method Header**************************************

//Method Name:  GetMultiCovCautionFlag

//Author:  PATNI/SVP

//Purpose: Returns multi cov caution flag

//**************************************************************************************/
bool CPQMGlobal::GetMultiCovCautionFlag(
)const
{
    return m_multi_cov_caution;
}

//************************************Method Header**************************************

//Method Name:  SetMultiCovCautionFlag

//Author:  PATNI/SVP

//Purpose: Sets multi cov caution flag

//**************************************************************************************/
void CPQMGlobal::SetMultiCovCautionFlag(
    const bool multi_cov_caution
)
{
    m_multi_cov_caution = multi_cov_caution;
}


//************************************Method Header**************************************

//Method Name:  GetPrepWbCoilFlag

//Author:  PATNI/SVP

//Purpose: Returns prep wb coil flag

//**************************************************************************************/
bool CPQMGlobal::GetPrepWbCoilFlag(
)const
{
    return m_prep_wb_coil;
}

//************************************Method Header**************************************

//Method Name:  SetPrepWbCoilFlag

//Author:  PATNI/SVP

//Purpose: Sets prep wb coil flag

//**************************************************************************************/
void CPQMGlobal::SetPrepWbCoilFlag(
    const bool prep_wb_coil
)
{
    m_prep_wb_coil = prep_wb_coil;
}

//************************************Method Header**************************************

//Method Name:  GetShimmWbCoilFlag

//Author:  PATNI/SVP

//Purpose: Returns shimm wb coil flag

//**************************************************************************************/
bool CPQMGlobal::GetShimmWbCoilFlag(
)const
{
    return m_shimm_wb_coil;
}

//************************************Method Header**************************************

//Method Name:  SetShimmWbCoilFlag

//Author:  PATNI/SVP

//Purpose: Sets shimm wb coil flag

//**************************************************************************************/
void CPQMGlobal::SetShimmWbCoilFlag(
    const bool shimm_wb_coil
)
{
    m_shimm_wb_coil = shimm_wb_coil;
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPQMGlobal::GetBatchSettingFlag(
) const	//Patni-Hemant/2010May6/Added/JaFT/SM_PH3_CDR-CDS_CDR_17
{
    return m_batch_setting_flag ;
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMGlobal::SetBatchSettingFlag(
    const bool batch_setting_flag
)
{
    m_batch_setting_flag = batch_setting_flag;
}

//************************************Method Header************************************
// Method Name  : GetLocatorMoveFlag
// Author       : PATNI/ MSN
// Purpose      :
//***********************************************************************************
bool CPQMGlobal::GetLocatorMoveFlag(
)const
{
    return m_locator_move_flag;
}

//************************************Method Header************************************
// Method Name  : SetLocatorMoveFlag
// Author       : PATNI/ MSN
// Purpose      :
//***********************************************************************************
void CPQMGlobal::SetLocatorMoveFlag(
    const bool f_locator_move_flag
)
{
    m_locator_move_flag = f_locator_move_flag;
}

//************************************Method Header**************************************

//Method Name:  GetPQMMode

//Author:  PATNI/SVP

//Purpose: Returns PQM mode

//**************************************************************************************/
int CPQMGlobal::GetPQMMode(
)const
{
    return m_mode;
}

//************************************Method Header**************************************

//Method Name:  SetPQMMode

//Author:  PATNI/SVP

//Purpose: Sets PQM mode

//**************************************************************************************/
void CPQMGlobal::SetPQMMode(
    const int mode
)
{
    m_mode = mode;
}


//************************************Method Header**********************************
// Method Name  : SetUtilityMenuDefaultItems
// Author       : PATNI/ ARD
// Purpose      : Set the Utility menu items on invocation of PQM
//***********************************************************************************
void CPQMGlobal::SetUtilityMenuDefaultItems(const SIPCConfigInfo* f_sIPCConfigInfo)
{
#define SET_UTIL_MENU(n) (m_s_default_utility_menu.##n = f_sIPCConfigInfo->s_DefaultUtilityMenu.##n)

    SET_UTIL_MENU(auto_plan);
    SET_UTIL_MENU(plan_scan);
    SET_UTIL_MENU(prep_WB_coil);
    SET_UTIL_MENU(shimming_WB_coil);

    SET_UTIL_MENU(skip_rgn);
    SET_UTIL_MENU(all_cov_rgn);
    SET_UTIL_MENU(batch_setting);
    SET_UTIL_MENU(multi_cov_caution);

    //Patni-Hemant/2010Jun7/Added/MaFT/MVC008198
    SET_UTIL_MENU(speedermap_auto_voice);

#undef SET_UTIL_MENU
}

//************************************Method Header**************************************
//Method Name:  GetWirelessSystemTypeFlag
//Author:  PATNI
//Purpose:
//**************************************************************************************/
bool CPQMGlobal::GetWirelessSystemTypeFlag()const
{
    return m_wireless_sys_flag;
}

//************************************Method Header**************************************
//Method Name:  SetWirelessSystemTypeFlag
//Author:  PATNI
//Purpose:
//**************************************************************************************/
void CPQMGlobal::SetWirelessSystemTypeFlag(
    const bool mode
)
{
    m_wireless_sys_flag = mode;
}


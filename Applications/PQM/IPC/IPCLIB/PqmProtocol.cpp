//Redmine-783
//Redmine-774
//Redmine-778
//Redmine-779
//Redmine-780
//Redmine-782
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PqmProtocol.cpp
 *  Overview: Implementation of CPqmProtocol class.
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
#include "PqmProtocol.h"
#include <tami/vfStudy/vf_appcodes.h>
#include <libStudyManager/constants.h> //Tanoue/MVC5849
#include <PQM/PQMLogMgr.h>
#include "PqmPrMode.h"
#include "PqmPrTRSC.h"

using namespace MR::ACQ::SM::COMMON; //Tanoue/MVC5849

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//****************************Method Header************************************
//Method Name   : CPqmProtocol()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
CPqmProtocol::CPqmProtocol():
    m_auto_target_ptr(),
    m_protocolkind(ProtocolKind_Normal),
    m_map_required(-1),
    m_map_post_proc_done(false),//Patni-MRP/2010Aug06/Added/TMSC-REDMINE498
    m_require_shimming_flag(-1),
    m_is_series_num_indb(FALSE),    //Patni-PJS/2009Dec15/Added/MVC006256
    m_app_code(0),
    m_tr(0),                    //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    m_prot(0),
    m_pos(0),
    m_acq_status(0),
    m_hold_scan(VFC_HOLD_PRE_SCAN),
    m_auto_hold(0),
    m_auto_proedit(0),
    m_auto_transfer(0),
    m_gating_method(0),
    m_dynamic(FALSE),
    m_intermit_flag(0),
    m_intermit_time(0),
    m_time(0L),
    m_pqm_pr_mode(new CPqmPrMode()),
    m_pqm_pr_trsc(new CPqmPrTRSC()),
    m_sar_init(FALSE),              //+Patni-HAR/Added/PSP-Defect#358
    m_processing(FALSE),
    m_shimming(0),
    m_shimm_ready(FALSE),
    m_fluoro(FALSE),
    m_has_image(FALSE),             //Patni-PJS/2009Dec15/Added/MVC006256
    m_has_parent(FALSE),
    m_prep_scan(0),
    m_moving_couch(0),
    m_acq_split_mode(VFC_ACQ_SPLIT_MODE_NONE),
    m_pln_auto_voice(0),
    m_exe_auto_voice(0),
    m_data_kind(0),
    m_speeder_type(VFC_SPEEDER_TYPE_NONE),
    m_speeder_mapprot(-1),
    m_intensity_type(VFC_INTENSITY_MAP_TYPE_NONE),
    m_probe_scan(VFC_PROBE_NONE),
    m_retro_scan(0),
    m_dbdt_mode(VFC_DBDT_CALC_MODE_CSR),
    m_dbdt_operation(VFC_DBDT_IEC_OPERATING_NORMAL),
    m_num_of_coverages(0),
    m_num_of_scan(0),
    m_retro_mode(VFC_RETRO_MODE_NONE),
    m_num_changed_receive_coil(0),
    m_cur_dbdt_mode(0),
    m_dbdt_mode_2002(0),
    m_dbdt_control(VFC_DBDT_CONTROLLED1),
    m_local_cf(0),
    m_couch_offset_flag(FALSE),
    m_probe_correction(0),
    m_pln_status(0),
    m_amb_enable_flag(0),
    m_amb_mode_flag(0),
    m_sarinfo(NULL),
    m_master_protocol(NULL),
    m_saturn_coil(new PqmSaturnCoil_t()),
    m_coil_DB_tbl(new PqmSaturnRcvCoil_t()),
    m_child_thread_status(CT_NONE),	//Patni-HAR/2010Jan18/Added/DeFT#MVC006364
    m_rflPos(0.0f),
    m_cfaPos(0.0f),
    m_rflProt(-1),
    m_cfaProt(-1),
    m_shimProt(-1),
    m_mrs_eddy_axis(0),              //Patni-HAR/2010Apr26/Added/IR-134
    m_bFlagWaitForVoice(TRUE),	    //Patni-Manish//2009Apr04//Added//PSP1-352
    m_copySourceProt(-1),           //Patni-Manish/2010Feb11/Added/MCM0229-0022/Parent Image
    m_couchMethodFlg(0),
    m_cala_enable_flg(false),
    m_hala_enable_flg(false),
    m_cala_invoke_mode(INVOKE_MODE_NONE),
    m_hala_invoke_mode(INVOKE_MODE_NONE),
    m_invoke_cala_app(FALSE),
    m_cala_planned_status(PLANNED_STATUS_NONE),
    m_hala_planned_status(PLANNED_STATUS_NONE),
    m_intensity_corr_status(0),
    m_coil_channel_mode(-1),
    m_delay_time(_T("")),
    m_group_id(0),
    m_master_flag(0),
    m_multi_series_enable_flag(VFC_MULTI_SERIES_DYNAMIC_ENABLE_FLAG_OFF),
    m_auto_get_interval_flag(-1),
    m_inscanswitchflag(false),
    m_inscantypetochange(0),
    m_numchannels(-1),
    m_automap_autoshim_protocol_added(false)

{
    //+Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    for (int j = 0; j < 32; j++) {
        m_scan_id[j] = '\0';
    }

    //+Patni-RUP/2009Aug25/Modified/cpp test corrections
    //memset(m_loc_images,0,sizeof(m_loc_images)*4);
    memset(m_loc_images, 0, sizeof(VfPathElem_t) * 4);
    //-Patni-RUP/2009Aug25/Modified/cpp test corrections

    //-Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections

    /* For scan offset. */
    memset(m_comment, 0, sizeof(m_comment));
    m_sarinfo = new sarInfo_t;
    memset(m_sarinfo, 0, sizeof(*m_sarinfo));

    // V9.25 Coil Support@AMT
    memset(m_saturn_coil, 0, sizeof(PqmSaturnCoil_t));
    memset(m_coil_DB_tbl, 0, sizeof(PqmSaturnRcvCoil_t));

    for (int i = 0; i < NUM_2; i++) {
        m_img_gr[i] = 0;
    }

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    for (i = 0; i < 32; i++) {
        m_contrast_name[i] = 0;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    memset(m_scan_id, '\0', SCAN_ID_LENGTH * 2 + 1); //Tanoue/MVC5849

    memset(m_comment, '\0', SCAN_COMMENT_LENGTH * 2 + 1); //Tanoue/MVC5849

    for (i = 0; i < PQM_MAX_MOVINGBED_SCAN; i++) {
        m_rcv_coil_tbl[i] = 0;
    }

    for (i = 0; i < NUM_2; i++) {
        m_dbdt_value[i] = 0L;
    }

    //MRP sar init
    m_sarStat = sarStatTsuccess;
    m_offset.x = 0.0;
    m_offset.y = 0.0;
    m_offset.z = 0.0;

}

//****************************Method Header************************************
//Method Name   : ~CPqmProtocol()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
CPqmProtocol::~CPqmProtocol(
)
{
    RemoveCrossRefForTargetAndAutoInsert();

    DEL_PTR(m_pqm_pr_mode);
    DEL_PTR(m_pqm_pr_trsc);
    DEL_PTR(m_sarinfo);

    // V9.25 Coil Support@AMT
    DEL_PTR(m_saturn_coil);
    DEL_PTR(m_coil_DB_tbl);
}

//****************************Method Header************************************
//Method Name   : GetCoilDBTable()
//Author        : PATNI\AMT
//Purpose       : Get coil database table
//*****************************************************************************
PqmSaturnRcvCoil_t* CPqmProtocol::GetCoilDBTable(
)
{
    return m_coil_DB_tbl;
}

//****************************Method Header************************************
//Method Name   : SetCoilDBTable()
//Author        : PATNI\AMT
//Purpose       : Set coil database table
//*****************************************************************************
bool CPqmProtocol::SetCoilDBTable(
    PqmSaturnRcvCoil_t* f_saturn_rcv_coil
)
{
    bool l_ret_value = false;

    if (NULL != f_saturn_rcv_coil) {
        memcpy(m_coil_DB_tbl, f_saturn_rcv_coil, sizeof(PqmSaturnRcvCoil_t));
        l_ret_value = true;
    }

    return l_ret_value;
}

//****************************Method Header************************************
//Method Name   : GetSaturnCoil()
//Author        : PATNI\AMT
//Purpose       : Get Saturn coil information
//*****************************************************************************
PqmSaturnCoil_t* CPqmProtocol::GetSaturnCoil(
)
{
    return m_saturn_coil;
}


//****************************Method Header************************************
//Method Name   : SetSaturnCoil()
//Author        : PATNI\AMT
//Purpose       : Set Saturn coil to protocol
//*****************************************************************************
bool CPqmProtocol::SetSaturnCoil(
    PqmSaturnCoil_t* f_saturn_coil
)
{
    bool l_ret_value = false ;

    if (NULL != f_saturn_coil) {
        memcpy(m_saturn_coil, f_saturn_coil, sizeof(PqmSaturnCoil_t));
        l_ret_value = true;
    }

    return l_ret_value;
}

//****************************Method Header************************************
//Method Name   : GetIntermitTime()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetIntermitTime(
) const
{
    return m_intermit_time;
}

//****************************Method Header************************************
//Method Name   : SetIntermitTime()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetIntermitTime(
    const int intermit_time
)
{
    m_intermit_time = intermit_time;
}

//****************************Method Header************************************
//Method Name   : GetAppCode()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetAppCode(
) const
{
    return m_app_code;
}

//****************************Method Header************************************
//Method Name   : SetAppCode()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetAppCode(
    int app_code
)
{
    m_app_code = app_code;
}

//****************************Method Header************************************
//Method Name   : GetProtocol()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetProtocol(
) const
{
    return m_prot;
}

//****************************Method Header************************************
//Method Name   : SetProtocol()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetProtocol(
    const int prot
)
{
    m_prot = prot;
}

//****************************Method Header************************************
//Method Name   : GetPosition()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetPosition(
) const
{
    return m_pos;
}

//****************************Method Header************************************
//Method Name   : SetPosition()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetPosition(
    const int pos
)
{
    m_pos = pos;
}

//****************************Method Header************************************
//Method Name   : GetImageGr()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int* CPqmProtocol::GetImageGr(
)
{
    return m_img_gr;
}

//****************************Method Header************************************
//Method Name   : GetAcqStatus()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetAcqStatus(
) const
{
    return m_acq_status ;
}

//****************************Method Header************************************
//Method Name   : SetAcqStatus()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetAcqStatus(
    int acq_status
)
{
    m_acq_status = acq_status;
}

//****************************Method Header************************************
//Method Name   : GetHoldScan()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetHoldScan(
) const
{
    return m_hold_scan;
}

//****************************Method Header************************************
//Method Name   : SetHoldScan()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetHoldScan(
    int hold_scan
)
{
    m_hold_scan = hold_scan;
}

//****************************Method Header************************************
//Method Name   : GetAutoHold()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetAutoHold(
) const
{
    return m_auto_hold;
}

//****************************Method Header************************************
//Method Name   : SetAutoHold()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetAutoHold(
    int auto_hold
)
{
    m_auto_hold = auto_hold;
}

//****************************Method Header************************************
//Method Name   : GetAutoProEdit()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetAutoProEdit(
) const
{
    return m_auto_proedit;
}

//****************************Method Header************************************
//Method Name   : SetAutoProEdit()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetAutoProEdit(
    int auto_pro_edit
)
{
    m_auto_proedit = auto_pro_edit;
}

//****************************Method Header************************************
//Method Name   : GetAutoTransfer()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetAutoTransfer(
) const
{
    return m_auto_transfer;
}

//****************************Method Header************************************
//Method Name   : SetAutoTransfer()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetAutoTransfer(
    int auto_transfer
)
{
    m_auto_transfer = auto_transfer;
}

//****************************Method Header************************************
//Method Name   : GetCouchOffsetFlag()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetCouchOffsetFlag(
) const
{
    return m_couch_offset_flag;
}

//****************************Method Header************************************
//Method Name   : SetCouchOffsetFlag()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetCouchOffsetFlag(
    int couch_offset_flag
)
{
    m_couch_offset_flag = couch_offset_flag;
}

//****************************Method Header************************************
//Method Name   : GetGatingMethod()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetGatingMethod(
) const
{
    return m_gating_method;
}

//****************************Method Header************************************
//Method Name   : SetGatingMethod()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetGatingMethod(
    int gating_method
)
{
    m_gating_method = gating_method;
}

//****************************Method Header************************************
//Method Name   : GetDynamic()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetDynamic(
) const
{
    return m_dynamic;
}

//****************************Method Header************************************
//Method Name   : SetDynamic()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetDynamic(
    BOOL dynamic
)
{
    m_dynamic = dynamic;
}

//****************************Method Header************************************
//Method Name   : GetIntermitFlag()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetIntermitFlag(
) const
{
    return m_intermit_flag;
}

//****************************Method Header************************************
//Method Name   : SetIntermitFlag()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetIntermitFlag(
    int intermit_flag
)
{
    m_intermit_flag = intermit_flag;
}

//****************************Method Header************************************
//Method Name   : GetScanID()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
char* CPqmProtocol::GetScanID(
)
{
    return m_scan_id;
}

//****************************Method Header************************************
//Method Name   : SetScanID()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetScanID(
    const char* scan_id
)
{
    strcpy(m_scan_id, scan_id);
}

//****************************Method Header************************************
//Method Name   : GetScanComment()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
char* CPqmProtocol::GetScanComment(
)
{
    return m_comment;
}

//****************************Method Header************************************
//Method Name   : SetScanComment()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetScanComment(
    const char* comment
)
{
    strcpy(m_comment, comment);
}

//****************************Method Header************************************
//Method Name   : GetSARInit()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
BOOL CPqmProtocol::GetSARInit(
) const
{
    return m_sar_init;
}

//****************************Method Header************************************
//Method Name   : SetSARInit()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetSARInit(
    const BOOL sar_init
)
{
    m_sar_init = sar_init;
}
//+Patni-PJS/Added/2011-Mar-11/REDMINE-1529
//****************************Method Header************************************
//Method Name   : GetProcessing()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetProcessing(
) const
{
    return m_processing;
}

//****************************Method Header************************************
//Method Name   : SetProcessing()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetProcessing(
    const int processing
)
{
    m_processing = processing;
}
//-Patni-PJS/Added/2011-Mar-11/REDMINE-1529
//****************************Method Header************************************
//Method Name   : GetShimming()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetShimming(
) const
{
    return m_shimming;
}

//****************************Method Header************************************
//Method Name   : SetShimming()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetShimming(
    const int shimming
)
{
    m_shimming = shimming;
}

//****************************Method Header************************************
//Method Name   : GetShimmingReady()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
BOOL CPqmProtocol::GetShimmingReady(
) const
{
    return m_shimm_ready;
}

//****************************Method Header************************************
//Method Name   : SetShimmingReady()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetShimmingReady(
    const BOOL shimm_ready
)
{
    m_shimm_ready = shimm_ready;
}

//****************************Method Header************************************
//Method Name   : GetFluoro()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
BOOL CPqmProtocol::GetFluoro(
) const
{
    return m_fluoro;

}

//****************************Method Header************************************
//Method Name   : SetFluoro()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetFluoro(
    BOOL fluoro
)
{
    m_fluoro = fluoro;
}


//****************************Method Header************************************
//Method Name   : GetHasImage()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
BOOL CPqmProtocol::GetHasImage(
) const
{
    return m_has_image;
}

//****************************Method Header************************************
//Method Name   : SetHasImage()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetHasImage(
    BOOL has_image
)
{
    m_has_image = has_image;
}

//****************************Method Header************************************
//Method Name   : GetHasParent()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
BOOL CPqmProtocol::GetHasParent(
) const
{
    return m_has_parent;
}

//****************************Method Header************************************
//Method Name   : SetHasParent()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetHasParent(
    BOOL has_parent
)
{
    m_has_parent = has_parent;
}

//****************************Method Header************************************
//Method Name   : GetPrepScan()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetPrepScan(
) const
{
    return m_prep_scan;
}

//****************************Method Header************************************
//Method Name   : SetPrepScan()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetPrepScan(
    const int prep_scan
)
{
    m_prep_scan = prep_scan;
}

//****************************Method Header************************************
//Method Name   : GetMovingCouch()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetMovingCouch(
) const
{
    return m_moving_couch;
}

//****************************Method Header************************************
//Method Name   : SetMovingCouch()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetMovingCouch(
    const int moving_couch
)
{
    m_moving_couch = moving_couch;
}

//****************************Method Header************************************
//Method Name   : GetAcqSplitMode()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetAcqSplitMode(
) const
{
    return m_acq_split_mode;
}

//****************************Method Header************************************
//Method Name   : SetAcqSplitMode()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetAcqSplitMode(
    int acq_split_mode
)
{
    m_acq_split_mode = acq_split_mode;
}

//****************************Method Header************************************
//Method Name   : GetPlnAutoVoice()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetPlnAutoVoice(
) const
{
    return m_pln_auto_voice;
}

//****************************Method Header************************************
//Method Name   : SetPlnAutoVoice()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetPlnAutoVoice(
    const int pln_auto_voice
)
{
    m_pln_auto_voice = pln_auto_voice;
}

//****************************Method Header************************************
//Method Name   : GetExeAutoVoice()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetExeAutoVoice(
) const
{
    return m_exe_auto_voice;
}

//****************************Method Header************************************
//Method Name   : SetExeAutoVoice()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetExeAutoVoice(
    int exe_auto_voice
)
{
    m_exe_auto_voice = exe_auto_voice;
}

//****************************Method Header************************************
//Method Name   : GetDataKind()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int const CPqmProtocol::GetDataKind(
)
{
    return m_data_kind;
}

//****************************Method Header************************************
//Method Name   : SetDataKind()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetDataKind(
    const int data_kind
)
{
    m_data_kind = data_kind;
}

//****************************Method Header************************************
//Method Name   : GetSpeederType()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetSpeederType(
) const
{
    return m_speeder_type;
}

//****************************Method Header************************************
//Method Name   : SetSpeederType()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetSpeederType(
    const int speeder_type
)
{
    m_speeder_type = speeder_type;
}

//****************************Method Header************************************
//Method Name   : GetSpeederMapProtocol()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetSpeederMapProtocol(
) const
{
    return m_speeder_mapprot;
}

//****************************Method Header************************************
//Method Name   : SetSpeederMapProtocol()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetSpeederMapProtocol(
    const int speeder_mapprot
)
{
    m_speeder_mapprot = speeder_mapprot;
}

//****************************Method Header************************************
//Method Name   : GetIntensityType()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetIntensityType(
) const
{
    return m_intensity_type;
}

//****************************Method Header************************************
//Method Name   : SetIntensityType()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetIntensityType(
    int intensity_type
)
{
    m_intensity_type = intensity_type;
}

//****************************Method Header************************************
//Method Name   : GetProbeScan()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetProbeScan(
) const
{
    return m_probe_scan;
}

//****************************Method Header************************************
//Method Name   : SetProbeScan()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetProbeScan(
    const int probe_scan
)
{
    m_probe_scan = probe_scan;
}

//****************************Method Header************************************
//Method Name   : GetRetroScan()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetRetroScan(
) const
{
    return m_retro_scan;
}

//****************************Method Header************************************
//Method Name   : GetdBdtMode()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetdBdtMode(
) const
{
    return m_dbdt_mode;
}

//****************************Method Header************************************
//Method Name   : SetdBdtMode()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetdBdtMode(
    int dBdt_mode
)
{
    m_dbdt_mode = dBdt_mode;
}

//****************************Method Header************************************
//Method Name   : GetdBdtModeString()
//Author        : PATNI\MSN
//Purpose       :  Gets the current dBdt value
//*****************************************************************************
CString CPqmProtocol::GetdBdtModeString()const
{
    return m_dbdtstring;
}

//****************************Method Header************************************
//Method Name   : SetdBdtModeString()
//Author        : PATNI\MSN
//Purpose       :  Sets the current dBdt value
//*****************************************************************************
void CPqmProtocol::SetdBdtModeString(const CString& f_dbdtstring)
{
    m_dbdtstring = f_dbdtstring;
}

//****************************Method Header************************************
//Method Name   : GetdBdtOperation()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetdBdtOperation(
) const
{
    return m_dbdt_operation;
}

//****************************Method Header************************************
//Method Name   : SetdBdtOperation()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetdBdtOperation(
    int dBdt_operation
)
{
    m_dbdt_operation = dBdt_operation;
}

//****************************Method Header************************************
//Method Name   : GetNumOfCoverages()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetNumOfCoverages(
) const
{
    return m_num_of_coverages;
}

//****************************Method Header************************************
//Method Name   : SetNumOfCoverages()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetNumOfCoverages(
    const int num_of_coverages
)
{
    m_num_of_coverages = num_of_coverages;
}

//****************************Method Header************************************
//Method Name   : GetNumOfScan()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetNumOfScan(
) const
{
    return m_num_of_scan;
}

//****************************Method Header************************************
//Method Name   : SetNumOfScan()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetNumOfScan(
    const int num_of_scan
)
{
    m_num_of_scan = num_of_scan;
}

//****************************Method Header************************************
//Method Name   : GetProlimSaveFlag()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
BOOL CPqmProtocol::GetProlimSaveFlag(
) const
{
    return m_prolim_saveflg;
}

//****************************Method Header************************************
//Method Name   : SetProlimSaveFlag()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetProlimSaveFlag(
    const BOOL prolim_saveflg
)
{
    m_prolim_saveflg = prolim_saveflg;
}

//****************************Method Header************************************
//Method Name   : GetRetroMode()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetRetroMode(
) const
{
    return m_retro_mode;
}

//****************************Method Header************************************
//Method Name   : GetNumChangedReceiveCoil()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetNumChangedReceiveCoil(
) const
{
    return m_num_changed_receive_coil;
}

//****************************Method Header************************************
//Method Name   : SetNumChangedReceiveCoil()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetNumChangedReceiveCoil(
    const int num_changed_receive_coil
)
{
    m_num_changed_receive_coil = num_changed_receive_coil;
}

//****************************Method Header************************************
//Method Name   : GetReceiveCoilTable()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int* CPqmProtocol::GetReceiveCoilTable(
)
{
    return m_rcv_coil_tbl;
}

//****************************Method Header************************************
//Method Name   : SetReceiveCoilTable()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetReceiveCoilTable(
    const int index,
    const int receive_coil_table
)
{
    m_rcv_coil_tbl[index] = receive_coil_table;
}

//****************************Method Header************************************
//Method Name   : GetCurrentdBdtMode()
//Author        : PATNI\MSN
//Purpose       :  Gets the current dBdt value
//*****************************************************************************
int CPqmProtocol::GetCurrentdBdtMode(
) const
{
    return m_cur_dbdt_mode;
}

//****************************Method Header************************************
//Method Name   : SetCurrentdBdtMode()
//Author        : PATNI\MSN
//Purpose       :  Sets the current dBdt value
//*****************************************************************************
void CPqmProtocol::SetCurrentdBdtMode(
    const int f_cur_dbdt_mode
)
{
    m_cur_dbdt_mode = f_cur_dbdt_mode;
}

//****************************Method Header************************************
//Method Name   : GetdBdtMode2002()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetdBdtMode2002(
) const
{
    return m_dbdt_mode_2002;
}

//****************************Method Header************************************
//Method Name   : SetdBdtMode2002()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetdBdtMode2002(
    int dBdt_mode_2002
)
{
    m_dbdt_mode_2002 = dBdt_mode_2002;
}

//****************************Method Header************************************
//Method Name   : GetdBdtControl()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetdBdtControl(
) const
{
    return m_dbdt_control;
}

//****************************Method Header************************************
//Method Name   : SetdBdtControl()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetdBdtControl(
    int dBdt_control
)
{
    m_dbdt_control = dBdt_control;
}

//****************************Method Header************************************
//Method Name   : GetLocalCF()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetLocalCF(
) const
{
    return m_local_cf;
}

//****************************Method Header************************************
//Method Name   : SetLocalCF()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetLocalCF(
    const int local_cf
)
{
    m_local_cf = local_cf;
}

//****************************Method Header************************************
//Method Name   : GetProbeCorrection()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetProbeCorrection(
) const
{
    return m_probe_correction;
}

//****************************Method Header************************************
//Method Name   : SetProbeCorrection()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetProbeCorrection(
    const int probe_correction
)
{
    m_probe_correction = probe_correction;
}

//****************************Method Header************************************
//Method Name   : GetPlnStatus()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetPlnStatus(
) const
{
    return m_pln_status;
}

//****************************Method Header************************************
//Method Name   : SetPlnStatus()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetPlnStatus(
    const int pln_status
)
{
    m_pln_status = pln_status;
}

//****************************Method Header************************************
//Method Name   : GetAMBEnableFlag()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetAMBEnableFlag(
) const
{
    return m_amb_enable_flag;
}

//****************************Method Header************************************
//Method Name   : GetAMBModeFlag()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetAMBModeFlag(
) const
{
    return m_amb_mode_flag;
}

//****************************Method Header************************************
//Method Name   : SetAMBModeFlag()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetAMBModeFlag(
    int amb_mode_flag
)
{
    m_amb_mode_flag = amb_mode_flag;
}

//****************************Method Header************************************
//Method Name   : GetScanTime()
//Author        : PATNI\HAR - JS
//Purpose       :
//*****************************************************************************
time_t CPqmProtocol::GetScanTime(
) const
{
    return m_time;
}

//****************************Method Header************************************
//Method Name   : SetScanTime()
//Author        : PATNI\HAR - JS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetScanTime(
    const time_t f_scan_time
)
{
    m_time = f_scan_time;
}

//****************************Method Header************************************
//Method Name   : GetOffset()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
f32vec3_t CPqmProtocol::GetOffset(
)const
{
    /*    if (xyz == 0) {
            return m_offset_x;
        } else if (xyz == 1) {
            return m_offset_y;
        } else {
            return m_offset_z;
        }*/

    return m_offset;
}

//****************************Method Header************************************
//Method Name   : SetOffset()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetOffset(
    const int xyz,
    const float value
)
{
    if (xyz == 0) {
        m_offset.x = value;

    } else if (xyz == 1) {
        m_offset.y = value;

    } else {
        m_offset.z = value;
    }
}

//****************************Method Header************************************
//Method Name   : GetPqmPrMode()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
CPqmPrMode* const CPqmProtocol::GetPqmPrMode(
)const
{
    return m_pqm_pr_mode;
}

//****************************Method Header************************************
//Method Name   : GetPqmPrTRSC()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
CPqmPrTRSC* const CPqmProtocol::GetPqmPrTRSC(
)const
{
    return m_pqm_pr_trsc;
}

//****************************Method Header************************************
//Method Name   : GetSarInfo()
//Author        : PATNI\LK
//Purpose       :  This will return the pointer of CPqmSarInfo class
//*****************************************************************************
sarInfo_t* CPqmProtocol::GetSarInfo(
)
{
    return m_sarinfo;
}

//****************************Method Header************************************
//Method Name   : GetTrValue()
//Author        : PATNI\HAR - JS
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetTrValue(
) const
{
    return m_tr;
}

//****************************Method Header************************************
//Method Name   : SetTrValue()
//Author        : PATNI\HAR - JS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetTrValue(
    const int f_tr
)
{
    m_tr = f_tr;
}

//****************************Method Header************************************
//Method Name   : GetContrastName()
//Author        : PATNI\HAR - JS
//Purpose       :
//*****************************************************************************
char* CPqmProtocol::GetContrastName(
)
{
    return m_contrast_name;
}

//****************************Method Header************************************
//Method Name   : SetContrastName()
//Author        : PATNI\HAR - JS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetContrastName(
    const char* f_contrastname
)
{
    strcpy(m_contrast_name, f_contrastname);
}

//****************************Method Header************************************
//Method Name   : GetSeriesLOID()
//Author        : PATNI\MRP
//Purpose       :  Returns the Series LOID
//*****************************************************************************
CString CPqmProtocol::GetSeriesLOID(
)const
{
    return m_series_loid;
}

//****************************Method Header************************************
//Method Name   : SetSeriesLOID()
//Author        : PATNI\MRP
//Purpose       :  Sets the Series LOID
//*****************************************************************************
void CPqmProtocol::SetSeriesLOID(
    const CString& f_series_loid
)
{
    m_series_loid = f_series_loid;
}

//****************************Method Header************************************
//Method Name   : GetSARStat()
//Author        : PATNI\MRP
//Purpose       :  Gets the SARStat
//*****************************************************************************
sarStat_t CPqmProtocol::GetSARStat(
)const
{
    return m_sarStat;
}

//****************************Method Header************************************
//Method Name   : SetSARStat()
//Author        : PATNI\MRP
//Purpose       :  Sets the SARStat
//*****************************************************************************
void CPqmProtocol::SetSARStat(
    const sarStat_t f_sarStat
)
{
    LPCTSTR FUNC_NAME = _T("CPqmProtocol::SetSARStat");
    CString l_print_string(_T(""));
    l_print_string.Format(_T("sarStatus = %d"), f_sarStat);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_print_string);

    m_sarStat = f_sarStat;
}

//****************************Method Header************************************
//Method Name   : GetLocatorImages()
//Author        : PATNI\MSN
//Purpose       :  Returns the Locator images
//*****************************************************************************
VfPathElem_t CPqmProtocol::GetLocatorImages(
    const int f_loc_img_cnt
)
{
    return m_loc_images[f_loc_img_cnt];
}

//****************************Method Header************************************
//Method Name   : SetLocatorImages()
//Author        : PATNI\MSN
//Purpose       :  Sets the Locator images
//*****************************************************************************
//+Patni-SS/2009Mar28/Moddified/PSP1#32,62 MoveCouch
void CPqmProtocol::SetLocatorImages(
    //int cnt,
    VfPathElem_t* f_locator_image
)
{
    if (f_locator_image != NULL) {
        memcpy(m_loc_images, f_locator_image, sizeof(m_loc_images));

    } else {
        memset(m_loc_images, NULL, sizeof(m_loc_images));
    }

    //m_loc_images[cnt] = f_locator_image;
}

//****************************Method Header************************************
//Method Name   : GetdBdtValue()
//Author        : PATNI\MSN
//Purpose       :  Gets the dBdt value
//*****************************************************************************
float CPqmProtocol::GetdBdtValue(
    const int f_index
)const
{
    if (f_index < NUM_2) {		//Patni-MJC/2009Aug17/Modified/cpp test corrections
        return m_dbdt_value[f_index];

    } else {
        return -1;
    }
}

//****************************Method Header************************************
//Method Name   : SetdBdtValue()
//Author        : PATNI\MSN
//Purpose       :  Sets the dBdt value
//*****************************************************************************
void CPqmProtocol::SetdBdtValue(
    const int f_index, const float f_dbdtvalue
)
{
    m_dbdt_value[f_index] = f_dbdtvalue;
}

//****************************Method Header************************************
//Method Name   : GetWaitForVoice()
//Parameter     : None
//Author        : PATNI\Manishkumar
//Purpose       : Return the variable which holds the value to play auto voice.
//*****************************************************************************
bool CPqmProtocol::GetFlagWaitForVoice()const
{
    return m_bFlagWaitForVoice;
}

//****************************Method Header************************************
//Method Name   : SetWaitForVoice(bool f_bWaitForVoice)
//Parameter     :  f_bWaitForVoice - The variable which holds the value
//Author        : PATNI\Manishkumar
//Purpose       :  Set the value of variable
//*****************************************************************************
void CPqmProtocol::SetFlagWaitForVoice(bool f_bFlagWaitForVoice)
{
    m_bFlagWaitForVoice = f_bFlagWaitForVoice;
}

//****************************Method Header************************************
//Method Name   : GetSeriesNumInDbFlag()
//Author        : PATNI\PJS
//Purpose       :  This function get the Series Num Flag of the class
//*****************************************************************************
BOOL CPqmProtocol::GetSeriesNumInDbFlag(
)const
{
    return m_is_series_num_indb;
}

//****************************Method Header************************************
//Method Name   : SetSeriesNumFlag()
//Author        : PATNI\PJS
//Purpose       :  This function Set the Series Num Flag of the class
//*****************************************************************************
void CPqmProtocol::SetSeriesNumInDbFlag(
    const BOOL f_set_series_num_indb
)
{
    m_is_series_num_indb = f_set_series_num_indb;
}

//****************************Method Header************************************
//Method Name   : GetChildThreadStatus()
//Author        : PATNI\HAR
//Purpose       :
//*****************************************************************************
CHILD_THREAD_STATUS CPqmProtocol::GetChildThreadStatus()const
{
    return m_child_thread_status;
}

//****************************Method Header************************************
//Method Name   : SetChildThreadStatus()
//Author        : PATNI\HAR
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetChildThreadStatus(
    const int f_child_thread_status)
{
    switch (f_child_thread_status) {
        case 0:
            m_child_thread_status = CT_NONE;
            break;

        case 1:
            m_child_thread_status = CT_DELETE;
            break;
    }
}

//**************************************************************************
//Method Name   : GetCopySourceProt
//Author        : PATNI/Manish
//Purpose       :
//**************************************************************************
int CPqmProtocol::GetCopySourceProt()const
{
    return m_copySourceProt;
}
//**************************************************************************
//Method Name   : SetCopySourceProt
//Author        : PATNI/Manish
//Purpose       :
//**************************************************************************
void CPqmProtocol::SetCopySourceProt(const int f_copySourceProt)
{
    m_copySourceProt = f_copySourceProt;
}
//**************************************************************************
//Method Name   : GetcouchMethodFlg
//Author        : PATNI/Manish
//Purpose       :
//**************************************************************************
int CPqmProtocol::GetcouchMethodFlg()const
{
    return m_couchMethodFlg;
}
//**************************************************************************
//Method Name   : SetcouchMethodFlg
//Author        : PATNI/Manish
//Purpose       :
//**************************************************************************
void CPqmProtocol::SetcouchMethodFlg(const int f_couchMethodFlg)
{
    m_couchMethodFlg = f_couchMethodFlg;
}

//**************************************************************************
//Method Name   : GetRflPos
//Author        : PATNI/DKH
//Purpose       : Function to get RFL position
//**************************************************************************
float CPqmProtocol::GetRflPos()const
{
    return m_rflPos;
}
//**************************************************************************
//Method Name   : SetRflPos
//Author        : PATNI/DKH
//Purpose       : Function to set RFL position
//**************************************************************************
void CPqmProtocol::SetRflPos(const float f_rflpos)
{
    m_rflPos = f_rflpos;
}
//**************************************************************************
//Method Name   : GetCfaPos
//Author        : PATNI/DKH
//Purpose       : Function to get CFA position
//**************************************************************************
float CPqmProtocol::GetCfaPos()const
{
    return m_cfaPos;
}
//**************************************************************************
//Method Name   : SetCfaPos
//Author        : PATNI/DKH
//Purpose       : Function to set CFA position
//**************************************************************************
void CPqmProtocol::SetCfaPos(const float f_cfapos)
{
    m_cfaPos = f_cfapos;
}

//**************************************************************************
//Method Name   : GetRflProt
//Author        : PATNI/DKH
//Purpose       : Function to get RFL protocol
//**************************************************************************
int CPqmProtocol::GetRflProt()const
{
    return m_rflProt;
}
//**************************************************************************
//Method Name   : SetRflProt
//Author        : PATNI/DKH
//Purpose       : Function to set RFL protocol
//**************************************************************************
void CPqmProtocol::SetRflProt(const int f_rflprot)
{
    m_rflProt = f_rflprot;
}
//**************************************************************************
//Method Name   : GetCfaProt
//Author        : PATNI/DKH
//Purpose       : Function to get CFA protocol
//**************************************************************************
int CPqmProtocol::GetCfaProt()const
{
    return m_cfaProt ;
}
//**************************************************************************
//Method Name   : SetCfaProt
//Author        : PATNI/DKH
//Purpose       : Function to set CFA protocol
//**************************************************************************
void CPqmProtocol::SetCfaProt(const int f_cfaprot)
{
    m_cfaProt = f_cfaprot;
}

//**************************************************************************
//Method Name   : GetShimProt
//Author        : PATNI/DKH
//Purpose       : Function to get Shim protocol
//**************************************************************************
int CPqmProtocol::GetShimProt()const
{
    return m_shimProt;
}

//**************************************************************************
//Method Name   : SetShimProt
//Author        : PATNI/DKH
//Purpose       : Function to set Shim protocol
//**************************************************************************
void CPqmProtocol::SetShimProt(const int f_shimprot)
{
    m_shimProt = f_shimprot;
}

//************************************Method Header************************************
// Method Name  : GetCDSInfo
// Author       : PATNI/ HEMANT
// Purpose      : Returns the protocol CDS information structure
//***********************************************************************************
CpqmCDSInfo* CPqmProtocol::GetCDSInfo(
)
{
    return &m_cds_info;
}
//+Patni-Hemant/2010Feb5/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : GetASCOMPASSFlag
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
int CPqmProtocol::GetASCOMPASSFlag(
) const  		//Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_20
{
    return m_cds_info.m_cds_flag ;
}

//************************************Method Header************************************
// Method Name  : SetASCOMPASSFlag
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPqmProtocol::SetASCOMPASSFlag(
    const int f_as_compass_flag
)
{
    m_cds_info.m_cds_flag = f_as_compass_flag ;
}
//-Patni-Hemant/2010Feb5/Added/ACE-Phase#3/CDS Requirement

//****************************Method Header************************************
//Method Name   : GetMRSEddyAxis()
//Author        : PATNI\HAR
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetMRSEddyAxis(
)const
{
    return m_mrs_eddy_axis;
}

//****************************Method Header************************************
//Method Name   : SetMRSEddyAxis()
//Author        : PATNI\HAR
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetMRSEddyAxis(
    const int f_mrs_eddy_axis
)
{
    m_mrs_eddy_axis = f_mrs_eddy_axis;
}

//****************************Method Header************************************
//Method Name   : GetRequireShimming
//Author        : PATNI/AM/NFD001
//Purpose       :
//*****************************************************************************
int CPqmProtocol:: GetRequireShimming()const
{
    return m_require_shimming_flag;
}

//****************************Method Header************************************
//Method Name   : SetRequireShimming
//Author        : PATNI/AM/NFD001
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetRequireShimming(const int l_require_shimmimg_flag)
{
    m_require_shimming_flag = l_require_shimmimg_flag;
}

//****************************Method Header************************************
//Method Name   : GetIsMapPostProcDone()
//Author        : PATNI\MRP
//Purpose       :
//*****************************************************************************
bool CPqmProtocol::GetIsMapPostProcDone(
)const
{
    return m_map_post_proc_done;

}
//****************************Method Header************************************
//Method Name   : SetIsMapPostProcDone()
//Author        : PATNI\MRP
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetIsMapPostProcDone(const bool f_is_map_postproc_done)
{
    m_map_post_proc_done = f_is_map_postproc_done;
}


int CPqmProtocol::GetProtcolCorrStatus(
)const
{
    return m_intensity_corr_status;
}


void CPqmProtocol::SetProtcolCorrStatus(const int f_intensity_corr_status)
{
    m_intensity_corr_status = f_intensity_corr_status;
}


//****************************Method Header************************************
//Method Name   : GetAutoInsertedMapProtocol()
//Author        : PATNI
//Purpose       :
//*****************************************************************************
CPqmProtocol* CPqmProtocol::GetAutoInsertedMapProtocol(
)const
{
    return m_auto_target_ptr.m_map_autoinsert_protocol;
}

//****************************Method Header************************************
//Method Name   : SetAutoInsertedMapProtocol()
//Author        : PATNI
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetAutoInsertedMapProtocol(
    CPqmProtocol* f_protocol
)
{
    m_auto_target_ptr.m_map_autoinsert_protocol = f_protocol;
}

//****************************Method Header************************************
//Method Name   : GetAutoInsertedShimProtocol()
//Author        : PATNI
//Purpose       :
//*****************************************************************************
CPqmProtocol* CPqmProtocol::GetAutoInsertedShimProtocol(
)const
{
    return m_auto_target_ptr.m_shim_autoinsert_protocol;
}

//****************************Method Header************************************
//Method Name   : SetAutoInsertedShimProtocol()
//Author        : PATNI
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetAutoInsertedShimProtocol(
    CPqmProtocol* f_protocol
)
{
    m_auto_target_ptr.m_shim_autoinsert_protocol = f_protocol;
}

//****************************Method Header************************************
//Method Name   : GetTargetProtocol()
//Author        : PATNI
//Purpose       :
//*****************************************************************************
CPqmProtocol* CPqmProtocol::GetTargetProtocol(
)const
{
    return m_auto_target_ptr.m_target_protocol;
}

//****************************Method Header************************************
//Method Name   : SetTargetProtocol()
//Author        : PATNI
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetTargetProtocol(
    CPqmProtocol* f_protocol
)
{
    m_auto_target_ptr.m_target_protocol = f_protocol;
}


//****************************Method Header************************************
//Method Name   : GetMapRequired
//Author        :
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetMapRequired(
)const
{
    return m_map_required;
}

//****************************Method Header************************************
//Method Name   : SetMapRequired
//Author        :
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetMapRequired(
    const int f_map_required
)
{
    m_map_required = f_map_required;
}
//****************************Method Header************************************
//Method Name   : SetOriginalParameters
//Author        :
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetOriginalParameters(
)
{
    LPCTSTR FUNC_NAME = _T("CPqmProtocol::SetOriginalParameters");
    memcpy(&(m_auto_target_ptr.m_original_params->m_saturn_coil), m_saturn_coil, sizeof(PqmSaturnCoil_t));
    m_auto_target_ptr.m_original_params->SetAnatomy(m_pqm_pr_mode->GetAnatomy());
    CString tmpStr(_T(""));
    tmpStr.Format(_T("m_auto_target_ptr.m_original_params->SetAnatomy(m_pqm_pr_mode->GetAnatomy()) = %d"), m_pqm_pr_mode->GetAnatomy());
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, tmpStr);
    m_auto_target_ptr.m_original_params->SetScanOffset(m_offset);
    m_auto_target_ptr.m_original_params->SetEditingFlag(false);
}
//****************************Method Header************************************
//Method Name   : GetOriginalParameters
//Author        :
//Purpose       :
//*****************************************************************************
OriginalParameters* CPqmProtocol::GetOriginalParameters(
)const
{
    return m_auto_target_ptr.m_original_params;
}

int CPqmProtocol::GetcoilChannelMode() const
{
    return m_coil_channel_mode;
}

void CPqmProtocol::SetcoilChannelMode(const int f_coil_channel_mode)
{
    m_coil_channel_mode = f_coil_channel_mode;
}

int CPqmProtocol::GetSystemChannels() const
{
    return m_numchannels;
}

void CPqmProtocol::SetSystemChannels(const int f_numchannel)
{
    m_numchannels = f_numchannel;
}


//****************************Method Header************************************
//Method Name   : GetMapInsertionCounter
//Author        :
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetMapInsertionCounter()const
{
    return m_auto_target_ptr.GetMapInsertionCounter();
}
//****************************Method Header************************************
//Method Name   : IncrementMapInsertionCounter
//Author        :
//Purpose       :
//*****************************************************************************
void CPqmProtocol::IncrementMapInsertionCounter(const int f_counter)
{
    m_auto_target_ptr.IncrementMapInsertionCounter(f_counter);

}
//****************************Method Header************************************
//Method Name   : GetShimInsertionCounter
//Author        :
//Purpose       :
//*****************************************************************************
int CPqmProtocol::GetShimInsertionCounter()const
{
    return m_auto_target_ptr.GetShimInsertionCounter();
}
//****************************Method Header************************************
//Method Name   : IncrementShimInsertionCounter
//Author        :
//Purpose       :
//*****************************************************************************
void CPqmProtocol::IncrementShimInsertionCounter(const int f_counter)
{
    m_auto_target_ptr.IncrementShimInsertionCounter(f_counter);

}


const ProtocolKindEnum CPqmProtocol::GetProtocolKind(
)const
{
    return m_protocolkind;
}


void CPqmProtocol::SetProtocolKind(
    const ProtocolKindEnum f_protocol_kind
)
{
    m_protocolkind = f_protocol_kind;
}


//****************************Method Header************************************
//Method Name   : RemoveAutoInsertProtRefFromTarget
//Author        :
//Purpose       :
//*****************************************************************************
void CPqmProtocol::RemoveAutoInsertProtRefFromTarget(CPqmProtocol* const f_auto_insertprot)
{

    if (NULL != m_auto_target_ptr.m_map_autoinsert_protocol) {

        if (f_auto_insertprot == m_auto_target_ptr.m_map_autoinsert_protocol) {
            m_auto_target_ptr.m_map_autoinsert_protocol = NULL;
            return;
        }
    }

    if (NULL != this->m_auto_target_ptr.m_shim_autoinsert_protocol) {

        if (f_auto_insertprot == m_auto_target_ptr.m_shim_autoinsert_protocol) {
            m_auto_target_ptr.m_shim_autoinsert_protocol = NULL;
            return;
        }
    }
}
//****************************Method Header************************************
//Method Name   : RemoveTargetRefFromAutoInsertProt
//Author        :
//Purpose       :
//*****************************************************************************
void CPqmProtocol::RemoveTargetRefFromAutoInsertProt(
    CPqmProtocol* const f_targetprot
)
{
    if (f_targetprot == m_auto_target_ptr.m_target_protocol) {
        m_auto_target_ptr.m_target_protocol = NULL ;
    }
}

//****************************Method Header************************************
//Method Name   : GetDelayTime()
//Author        : PATNI\AKR
//Purpose       : Get the new delay time from protocol
//*****************************************************************************
CString CPqmProtocol::GetDelayTime() const
{
    return m_delay_time;
}
//****************************Method Header************************************
//Method Name   : SetDelayTime()
//Author        : PATNI\AKR
//Purpose       : Set the new delay time in protocol
//*****************************************************************************
void CPqmProtocol::SetDelayTime(const CString& f_delay_time)
{
    m_delay_time = f_delay_time;
}

//****************************Method Header************************************
//Method Name   : GetMasterFlag()
//Author        : PATNI\AKR
//Purpose       : Get master flag information
//*******************************************
BOOL CPqmProtocol::GetMasterFlag() const
{
    return m_master_flag;
}
//****************************Method Header************************************
//Method Name   : SetMasterFlag()
//Author        : PATNI\AKR
//Purpose       : Set Master loid information
//*****************************************************************************
void CPqmProtocol::SetMasterFlag(const BOOL f_master_flag)
{
    m_master_flag = f_master_flag;
}
//****************************Method Header************************************
//Method Name   : GetGroupID()
//Author        : PATNI\AKR
//Purpose       : Get group id
//*****************************************************************************
int CPqmProtocol::GetGroupID() const
{
    return m_group_id;
}
//****************************Method Header************************************
//Method Name   : SetGroupID()
//Author        : PATNI\AKR
//Purpose       : Set group id of protocol
//*****************************************************************************
void CPqmProtocol::SetGroupID(const int f_group_id)
{
    m_group_id = f_group_id;
}
//****************************Method Header************************************
//Method Name   : GetMultiSeriesDynamicFlag()
//Author        : PATNI\AKR
//Purpose       : Get MultiSeries enable flag for master slave protocols
//*****************************************************************************
int CPqmProtocol::GetMultiSeriesDynamicFlag() const
{
    return m_multi_series_enable_flag;
}
//****************************Method Header************************************
//Method Name   : SetMultiSeriesDynamicFlag()
//Author        : PATNI\AKR
//Purpose       : Set MultiSeries enable flag for master slave protocols
//*****************************************************************************
void CPqmProtocol::SetMultiSeriesDynamicFlag(const int f_multi_series_enable_flag)
{
    m_multi_series_enable_flag = f_multi_series_enable_flag;
}

//****************************Method Header************************************
//Method Name   : GetMasterProtocol()
//Author        : PATNI\AKR
//Purpose       : Get Master protocol pointer for slave
//*****************************************************************************
CPqmProtocol* CPqmProtocol::GetMasterProtocol() const
{
    return m_master_protocol;
}
//****************************Method Header************************************
//Method Name   : SetMasterProtocol()
//Author        : PATNI\AKR
//Purpose       : Every slave will have master's protocol information, this is set at
//				  slave transtfer from memory, slave duplicate, on queue & exit and at master delete.
//*****************************************************************************
void CPqmProtocol::SetMasterProtocol(CPqmProtocol* const f_master_protocol)
{
    m_master_protocol = f_master_protocol;
}

//****************************Method Header************************************
//Method Name   : RemoveCrossRefForTargetAndAutoInsert
//Author        :
//Purpose       :
//*****************************************************************************
void CPqmProtocol::RemoveCrossRefForTargetAndAutoInsert(
)
{
    if (NULL != m_auto_target_ptr.m_target_protocol) {
        m_auto_target_ptr.m_target_protocol->RemoveAutoInsertProtRefFromTarget(this);
        m_auto_target_ptr.m_target_protocol = NULL;
        return;
    }

    if (NULL != m_auto_target_ptr.m_map_autoinsert_protocol) {
        m_auto_target_ptr.m_map_autoinsert_protocol->RemoveTargetRefFromAutoInsertProt(this);
        m_auto_target_ptr.m_map_autoinsert_protocol = NULL;
    }

    if (NULL != m_auto_target_ptr.m_shim_autoinsert_protocol) {
        m_auto_target_ptr.m_shim_autoinsert_protocol->RemoveTargetRefFromAutoInsertProt(this);
        m_auto_target_ptr.m_shim_autoinsert_protocol = NULL;
    }

}



//****************************Method Header************************************
//Method Name   : SetImageGr()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetImageGr(
    const int index,
    const int img_gr
)
{
    m_img_gr[index] = img_gr;
}

//****************************Method Header************************************
//Method Name   : SetRetroScan()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetRetroScan(
    const int retro_scan
)
{
    m_retro_scan = retro_scan;
}

//****************************Method Header************************************
//Method Name   : SetRetroMode()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetRetroMode(
    const int retro_mode
)
{
    m_retro_mode = retro_mode;
}

//****************************Method Header************************************
//Method Name   : SetAMBEnableFlag()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetAMBEnableFlag(
    int amb_enable_flag
)
{
    m_amb_enable_flag = amb_enable_flag;
}

//****************************Method Header************************************
//Method Name   : IsProtocolCALAEnable()
//Author        : PATNI\PJS
//Purpose       :
//*****************************************************************************
bool CPqmProtocol::IsProtocolCALAEnable(
)
{
    return m_cala_enable_flg;
}

//****************************Method Header************************************
//Method Name   : SetProtcolCALAEnabled()
//Author        : PATNI\PJS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetProtcolCALAEnabled(
    const bool f_cala_enable_flg
)
{
    m_cala_enable_flg = f_cala_enable_flg;
}

//****************************Method Header************************************
//Method Name   : GetCALAInvokeMode()
//Author        : PATNI\PJS
//Purpose       :
//*****************************************************************************
AL_INVOKE_MODE CPqmProtocol::GetCALAInvokeMode(
)
{
    return m_cala_invoke_mode;
}

//****************************Method Header************************************
//Method Name   : SetCALAInvokeMode()
//Author        : PATNI\PJS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetCALAInvokeMode(
    AL_INVOKE_MODE f_cala_invoke_mode
)
{
    m_cala_invoke_mode = f_cala_invoke_mode;
}

void CPqmProtocol::SetHALAInvokeMode(const AL_INVOKE_MODE f_hala_invoke_mode)
{
    m_hala_invoke_mode = f_hala_invoke_mode;
}
//****************************Method Header************************************
//Method Name   : CanInvokeAutoLocator()
//Author        : PATNI\PJS
//Purpose       :
//*****************************************************************************
BOOL CPqmProtocol::CanInvokeAutoLocator(
)
{
    return m_invoke_cala_app;
}

//****************************Method Header************************************
//Method Name   : SetCanInvokeAutoLocator()
//Author        : PATNI\PJS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetCanInvokeAutoLocator(
    BOOL f_invoke_cala_app
)
{
    m_invoke_cala_app = f_invoke_cala_app;
}

//****************************Method Header************************************
//Method Name   : SetCalaPlannedStatus()
//Author        : PATNI\PJS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetCalaPlannedStatus(
    AL_PLANNED_STATUS f_cala_planned_status
)
{
    m_cala_planned_status = f_cala_planned_status;
}
void CPqmProtocol::SetHalaPlannedStatus(
    AL_PLANNED_STATUS f_hala_planned_status
)
{
    m_hala_planned_status = f_hala_planned_status;
}
//****************************Method Header************************************
//Method Name   : GetCalaPlannedStatus()
//Author        : PATNI\PJS
//Purpose       :
//*****************************************************************************
AL_PLANNED_STATUS CPqmProtocol::GetCalaPlannedStatus(
)
{
    return m_cala_planned_status;
}
AUTOLOCATOR_TYPE CPqmProtocol::GetAutoLocatorType()
{
    if (TRUE == m_cala_enable_flg) {
        return AUTOLOCATOR_CARDIAC;

    } else if (TRUE == m_hala_enable_flg) {
        return AUTOLOCATOR_NEURO;

    } else {
        return AUTOLOCATOR_NONE;
    }
}
AL_INVOKE_MODE CPqmProtocol::GetAutoLocatorInvokeMode(const AUTOLOCATOR_TYPE f_autolocator_type)
{
    if (AUTOLOCATOR_CARDIAC == f_autolocator_type) {
        return GetCALAInvokeMode();

    } else if (AUTOLOCATOR_NEURO == f_autolocator_type) {
        return GetHALAInvokeMode();

    } else {
        return INVOKE_MODE_NONE ;
    }
}
AL_PLANNED_STATUS CPqmProtocol::GetHALAPlannedStatus()
{
    return m_hala_planned_status;
}
//****************************Method Header************************************
//Method Name   : GetAutogetIntervalflag()
//Author        : PATNI
//Purpose       :
//*****************************************************************************
BOOL CPqmProtocol::GetAutoGetIntervalFlag()const
{
    return m_auto_get_interval_flag;
}

//****************************Method Header************************************
//Method Name   : SetAutogetIntervalflag()
//Author        : PATNI
//Purpose       :
//*****************************************************************************

void CPqmProtocol::SetAutoGetIntervalFlag(
    const BOOL f_auto_get_interval_flag
)
{
    m_auto_get_interval_flag = f_auto_get_interval_flag;
}


//****************************Method Header************************************
//Method Name   : IsProtocolHALAEnable()
//Author        : iGATE
//Purpose       :
//*****************************************************************************
bool CPqmProtocol::IsProtocolHALAEnable(
)
{
    return m_hala_enable_flg;
}


//****************************Method Header************************************
//Method Name   : SetProtcolHALAEnabled()
//Author        : PATNI\PJS
//Purpose       :
//*****************************************************************************
void CPqmProtocol::SetProtcolHALAEnabled(
    const bool f_hala_enable_flg
)
{
    m_hala_enable_flg = f_hala_enable_flg;
}
//****************************Method Header************************************
//Method Name   : GetCALAInvokeMode()
//Author        : PATNI\PJS
//Purpose       :
//*****************************************************************************
AL_INVOKE_MODE CPqmProtocol::GetHALAInvokeMode(
)
{
    return m_hala_invoke_mode;
}

bool CPqmProtocol::GetInscanSwitchFlag()const
{
    return m_inscanswitchflag;
}
void CPqmProtocol::SetInscanSwitchFlag(const bool f_inscanswitchflag)
{
    m_inscanswitchflag = f_inscanswitchflag ;
}
int CPqmProtocol::GetInscanSwitchType()const
{
    return m_inscantypetochange;
}
void CPqmProtocol::SetInscanSwitchType(const int f_inscanswitchtype)
{
    m_inscantypetochange =  f_inscanswitchtype;
}
bool CPqmProtocol::GetAtuomapAutoShimAdded()const
{
    return m_automap_autoshim_protocol_added;
}
void CPqmProtocol::SetAtuomapAutoShimAdded(const bool f_automap_autoshim_protocol_added)
{
    m_automap_autoshim_protocol_added =  f_automap_autoshim_protocol_added;
}

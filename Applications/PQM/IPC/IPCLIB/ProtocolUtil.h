/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: ProtocolUtil.h
 *  Overview: Structure of CProtocolUtil class.
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
//This class will contain all the study related information like the patient name etc.
//It will also hold the map of the CSeriesPQM objects

#ifndef __SM_IPCLIB_PROTOCOLUTIL_H__
#define __SM_IPCLIB_PROTOCOLUTIL_H__

#if _MSC_VER > 1000
#pragma once
#endif
#define MAX_COM 512

#include <tami/vfStudy/vfstudy.h>
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"
/* pqmPrMaintainStudy Mode */
#define MAINTAIN_NORMAL         0
#define MAINTAIN_PREP           1
#define MAINTAIN_CF         2
#define MAINTAIN_MOTION_PROBING     3
#define MAINTAIN_MOTION_CORRECT     4
#define VFC_LOCAL_CF_NONE  0

typedef struct {
    int     tag;    /* Tag number in pqm protocols. */
    char*    str;   /* Tag name */
    bool_t  flag;   /* TRUE : Store in pr_data_t.
               FALSE: Only get data.    */
    VfName_t    name;   /* SVN_.... */
    VfType_t    type;   /* SVT_.... */
    int     depth;  /* SVD_.... */
    VfSize_t    size;   /* value size */
} SPqm_pr_init_t;

/******************************************************************
 If size == 0, PQM will not get the data. Only label.
******************************************************************/
#define INIPP(n,b,v,s)  {K_PR_##n,#n,b,SVN_PLN_##v,SVT_PLN_##v,SVD_ST_PROTOCOL,s}
#define INIPR(n,b,v,s)  {K_PR_##n,#n,b,SVN_PRS_##v,SVT_PRS_##v,SVD_ST_PROTOCOL,s}
#define INIPC(n,b,v,s)  {K_PR_##n,#n,b,SVN_SCH_##v,SVT_SCH_##v,SVD_ST_PROTOCOL,s}
#define INIPE(n,b,v,s)  {K_PR_##n,#n,b,SVN_EXE_##v,SVT_EXE_##v,SVD_ST_PROTOCOL,s}
#define INIPS(n,b,v,s)  {K_PR_##n,#n,b,SVN_SEQ_##v,SVT_SEQ_##v,SVD_ST_PROTOCOL,s}
#define INIPG(n,b,v,s)  {K_PR_##n,#n,b,SVN_GEN_##v,SVT_GEN_##v,SVD_ST_PROTOCOL,s}
#define INIPPR(n,b,v,s) {K_PR_##n,#n,b,SVN_PRC_##v,SVT_PRC_##v,SVD_ST_PROTOCOL,s}
#define INISS(n,b,v,s)  {K_PR_##n,#n,b,SVN_SEQ_##v,SVT_SEQ_##v,SVD_ST_SUBPROTOCOL,s}

#define PRSCN(n,b,v,s)  {K_PR_PRESCAN_##n,#n,b,SVN_EXE_PRESCAN_##v,SVT_EXE_PRESCAN_##v,SVD_ST_PROTOCOL,s}

static  SPqm_pr_init_t   pr_init[] = {
    {K_PR_DUMMY, FALSE, 0, 0, 0, 0},

    INIPP(APP_CODE,             TRUE,   APP_CODE,           sizeof(int32_t)),
    INIPP(SEQUENCE_NAME,        FALSE,  TEMPLATE_SEQ,       0),
    INIPP(IMAGING_MODE,         FALSE,  IMAGING_MODE,       sizeof(int32_t)),

    INIPP(SCAN_ID,              TRUE,   COMMENTS,           0),
    INIPP(SCAN_COMMENTS,        TRUE,   SCAN_COMMENTS,      0),
    INIPP(TOTAL_TIME,           TRUE,   TOTAL_TIME,         sizeof(time_t)),
    INIPP(ACQ_TIME,             FALSE,  SUBPROTOCOL_TIME,   sizeof(time_t)),
    INIPE(ACQ_STATUS,           TRUE,   PROTOCOL_ACQ_STATUS, sizeof(int32_t)),
    INIPG(IMG_GR,               FALSE,  VF_GROUP,           0),

    INIPP(CONTRAST,             TRUE,   CONTR_AGENT_FL,     sizeof(bool_t)),
    INIPC(CONTRAST_NAME,        FALSE,  N_CONTRAST,         0),
    INIPP(PATIENT_ORIENT,       TRUE,   PATIENT_ORIENT,     sizeof(VftPatori_t)),
    INIPP(RC_COIL,              TRUE,   RECVR_COIL,         sizeof(int32_t)),
    INIPP(TR_COIL,              FALSE,  TRANSMIT_COIL,      sizeof(int32_t)),
    INIPR(RC_COIL_NAME,         FALSE,  RECVR_COIL,         0),
    INIPR(TR_COIL_NAME,         FALSE,  TRANSMIT_COIL,      0),

    //Patni/SS/2010July20/Added/NFD001-AutoShim
    INIPP(SHIM_REQUIRED,        TRUE,   SHIM_REQUIRED,		sizeof(int32_t)),
    INIPP(HOLD,                 TRUE,   HOLD,               sizeof(int32_t)),
    INIPP(AUTO_HOLD,            TRUE,   AUTO_HOLD,          sizeof(int32_t)),
    INIPP(AUTO_POSTPROC_FLG,    TRUE,   AUTO_POSTPROC_FLG,  sizeof(int32_t)),
    //+Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement	(V9.50)
    INIPP(CDS_FLAG,				TRUE,	CDS_FLAG,			sizeof(int32_t)),
    //-Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement
    INIPP(ANATOMY,              TRUE,   SCAN_ANATOMY,       sizeof(int32_t)),
    INIPP(GATE,                 TRUE,   GATING_METHOD,      sizeof(int32_t)),
    INIPP(DYNAMIC,              TRUE,   DYNAMIC_SCAN,       sizeof(int32_t)),
    INIPP(INTERMIT_FLAG,        FALSE,  INTERMIT_FLAG,      sizeof(int32_t)),
    INIPP(INTERMIT_TIME,        FALSE,  INTERMIT_TIME,      sizeof(int32_t)),
    INIPE(COUCH,                TRUE,   COUCH,              sizeof(flt32_t)),
    INIPE(HOME_POSITION,        FALSE,  HOME_POSITION,      sizeof(flt32_t)),
    INIPE(COUCH_OFFSET,         FALSE,  COUCH_OFFSET,       sizeof(flt32_t)),
    INIPE(PARENT_COUCH_OFFSET,  FALSE,  PARENT_COUCH_OFFSET, sizeof(flt32_t)),
    INIPE(SCAN_NUMBER,          FALSE,  ACQ_CONDITION,      sizeof(int32_t)),

    PRSCN(FSY,                  FALSE,  FSY_ACTION,         sizeof(int32_t)),
    PRSCN(RFL,                  FALSE,  RFL_ACTION,         sizeof(int32_t)),
    PRSCN(RGN,                  FALSE,  RGN_ACTION,         sizeof(int32_t)),

    PRSCN(FSY_VALUE,            FALSE,  FSY_VALUE,          sizeof(flt64_t)),
    PRSCN(RFL_GAIN,             FALSE,  RFL_GAIN_VALUE,     sizeof(flt32_t)),
    PRSCN(RFL_SCALER,           FALSE,  RFL_SCALE_VALUE,    sizeof(flt32_t)),
    PRSCN(RGN_VALUE,            FALSE,  RGN_VALUE,          sizeof(flt32_t)),
    PRSCN(RGN_OFFSET,           FALSE,  RGN_OFFSET,         sizeof(flt32_t)),
    INIPS(DEFAULT_RGN_VALUE,    FALSE,  DEFAULT_RGN_VALUE,  sizeof(flt32_t)),

    INIPP(LOCATOR_IMAGE,        FALSE,  LOCATOR_IMAGES,     0),

    INIPP(CALC_SAR,             FALSE,  CALC_SAR_FLAG,      sizeof(bool_t)),
    INIPS(SEQ_SHIM,             FALSE,  SHIM_FLAG,          sizeof(bool_t)),
    INIPS(SEQ_FLUORO,           FALSE,  FLUORO_FLAG,        sizeof(bool_t)),

    INISS(SAR,                  TRUE,   SAR,                sizeof(flt32_t)),

    INIPP(STATUS,               FALSE,  STATUS,             sizeof(int32_t)),
    INIPP(VIEW_DIR,             TRUE,   VIEWING_DIR,        sizeof(int32_t)),

    INIPP(PLN_AUTO_VOICE,       TRUE,   AUTO_VOICE,         sizeof(int32_t)),
    INIPE(EXE_AUTO_VOICE,       TRUE,   AUTO_VOICE,         sizeof(int32_t)),

    INIPP(TIME_CTRL_PARAM,      FALSE,  TIME_CTRL_PARAM,    sizeof(VftTimeCtrlParam_t)),
    INIPP(TPCTRL,               FALSE,  AUTO_VOICE_TPCTRL,  sizeof(int32_t)),
    INIPP(MIN_TR,               FALSE,  MIN_TR,             sizeof(flt32_t)),
    INIPP(TR,                   FALSE,  CONTRAST_TR,        sizeof(flt32_t)),

    INIPE(SCAN_AGENT,           FALSE,  SCAN_AGENT,         0),
    INIPP(PREP_SCAN,            FALSE,  PREP_SCAN,          sizeof(int32_t)),
    INIPP(MOVING_COUCH,         FALSE,  MOVING_COUCH_FLG,   sizeof(int32_t)),
    INIPP(POST_ACQ_MODE,        FALSE,  POST_ACQ_MODE,      sizeof(int32_t)),
    INIPS(FLY_FLAG,             FALSE,  FLY_FLAG,           sizeof(int32_t)),
    INIPE(PAC_PATTERN,          FALSE,  PAC_PATTERN,        sizeof(int32_t)),
    INIPE(NUM_PAC_SCAN,         FALSE,  NUM_PAC_SCAN,       sizeof(int32_t)),

    INIPP(SPEEDER_TYPE,         FALSE,  SPEEDER_TYPE,       sizeof(int32_t)),
    INIPE(CHANNEL_PER_VFEP,     FALSE,  CHANNEL_PER_VFEP,   sizeof(int32_t)),
    INIPPR(INTENSITY_CORR_FLAG, TRUE,   INTENSITY_CORR_FLAG, sizeof(int32_t)),

    INIPP(ACQ_SPLIT_MODE,       FALSE,  ACQ_SPLIT_MODE,     sizeof(int32_t)),
    INIPP(AUTO_PROEDIT_FLAG,    FALSE,  AUTO_PROEDIT_FLAG,  sizeof(int32_t)),
    INIPP(PROBE_FLAG,           TRUE,   PROBE_FLAG,         sizeof(int32_t)),
    INIPP(GATE_PARAM,           FALSE,  GATE_PARAM,         sizeof(VftGatingParam_t)),
    INIPS(DBDT_MODE,            FALSE,  DBDT_MODE,          sizeof(int32_t)),

    INISS(DBDT_OPERATING_MODE,  TRUE,   DBDT_IEC_OPERATING_MODE, sizeof(int32_t)),

    INISS(DBDT_VALUE,           TRUE,   DBDT_VALUE,         sizeof(flt32_t) * 2),
    INIPPR(DRKS_FLAG,           FALSE,  DRKS_FLAG,          sizeof(int32_t)),

    INIPPR(RECON_START_AFTER_REFVIEW, TRUE, RECON_START_AFTER_REFVIEW, sizeof(int32_t)),

    INIPP(REPETITIONS,          FALSE,  REPETITIONS,        sizeof(int32_t)),
    INIPP(COUCH_TABLE,          FALSE,  COUCH_TABLE,        0),

    INIPE(CHANGED_RECEIVE_COIL,     TRUE,   CHANGED_RECEIVE_COIL,   sizeof(int32_t)),
    INIPE(RECVR_COIL_TBL,           TRUE,   RECVR_COIL_TBL,     0),
    INIPE(RECVR_COILNAME_TBL,       TRUE,   RECVR_COILNAME_TBL, 0),
    INIPE(PAC_PATTERN_TBL,          TRUE,   PAC_PATTERN_TBL,    0),
    INIPE(CHANNEL_GAIN_RATIO_TBL,   TRUE,  CHANNEL_GAIN_RATIO_TBL, 0),
    INIPS(DBDT_MODE_2002,           FALSE,  DBDT_MODE_2002,     sizeof(int32_t)),
    INIPE(PAC_SCAN_FLAG,            FALSE,  PAC_SCAN_FLAG,      sizeof(bool_t)),    /* V6.20 */
    INIPP(LOCAL_CF_FLAG,            VFC_LOCAL_CF_NONE,  LOCAL_CF_FLAG,      sizeof(int32_t)),   /* V6.25 */
    INIPP(PROBE_CORRECTION,         FALSE,  PROBE_CORRECTION,       sizeof(int32_t)),        /* V6.50 */

    INIPP(AMB_ENABLE_FLG, FALSE,          AMB_ENABLE_FLG,     sizeof(int32_t)),        /* V7.10 */
    INIPP(AMB_MODE_FLG,   VFC_AMB_MODE_NONE, AMB_MODE_FLG,       sizeof(int32_t)),       /* V7.10 */
    INIPP(NUM_SECTION_RECVR_COIL,   FALSE,  NUM_SECTION_RECVR_COIL, sizeof(int32_t)),   /* V9.00 */
    INIPP(LDB_MAPPED_ID,             TRUE,   LDB_MAPPED_ID,           0),
    INIPP(LOCATOR_IMAGE_ID,             TRUE,   LOCATOR_IMAGE_ID,           0),//Meghana to read the new tag for parent Image

    //+Patni-HEMANT/ADDED On 6/17/2009 4:22:38 PM/ ACE-2 / WFDA
    INIPP(INSCAN_FLAG,				FALSE,   INSCAN_FLAG,           sizeof(int32_t)),
    INIPP(INSCAN_PREFERENCE_GUID,	FALSE,   INSCAN_PREFERENCE_GUID,           0),
    //-Patni-HEMANT/ADDED On 6/17/2009 4:22:38 PM/ ACE-2 / WFDA
    //+Patni-MP/ADDED On 6/17/2009 4:22:38 PM/ ACE-2 / CR MANUAL PRESCAN
    PRSCN(RFL_GAIN_VALUE_LIMIT, FALSE,  RFL_GAIN_VALUE_LIMIT,     sizeof(flt32_t)),
    //-Patni-MP/ADDED On 6/17/2009 4:22:38 PM/ ACE-2 / CR MANUAL PRESCAN

    //+Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image
    INIPP(MOVE_COUCH_METHOD_FLAG,	TRUE,	MOVE_COUCH_METHOD_FLAG,	sizeof(int32_t)),
    INIPP(COPY_SOURCE_PROTOCOL,		TRUE,	COPY_SOURCE_PROTOCOL_NO, sizeof(int32_t)),
    //-Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image
    INIPE(RFL_COUCH,					TRUE,				RFL_COUCH,				sizeof(flt32_t)),	/* V9.50 */
    INIPE(CFA_COUCH,					TRUE,				CFA_COUCH,				sizeof(flt32_t)),	/* V9.50 */
    //Patni-HAR/2010Apr26/Added/IR-134
    INIPS(MRS_EDDY_AXIS,            TRUE,  MRS_EDDY_AXIS,         sizeof(int32_t)),
    //Patni-HAR/2010Jun29/Added/V1.30#IR-158
    INIPE(PC_VAP_MODE,                  FALSE,  PC_VAP_MODE,                sizeof(int32_t)),
    //Patni-PJS/2011Feb09/Added/MVC009736
    INIPP(COVERAGES,            FALSE,   COVERAGES,             sizeof(int32_t)),
    INIPP(MAP_REQUIRED,        TRUE,   MAP_REQUIRED,		sizeof(int32_t)),
    INIPP(CALA_ENABLE_FLAG,         FALSE,  CALA_ENABLE_FLAG,      sizeof(bool_t)),
    INIPP(CALA_INVOKE_MODE,         FALSE,  CALA_INVOKE_MODE,      sizeof(int32_t)),
    INIPP(CALA_PLANNED_STATUS,      FALSE,  CALA_PLANNED_STATUS,   sizeof(int32_t)),
    INIPPR(INTENSITY_CORR_STATUS,      FALSE,  INTENSITY_CORR_STATUS,   sizeof(int32_t)),
    INIPP(MULTI_SERIES_DYNAMIC_ENABLE_FLAG,      FALSE,  MULTI_SERIES_DYNAMIC_ENABLE_FLAG,   sizeof(int32_t)),
    INIPP(MAX_COIL_CHANNEL_MODE,      FALSE,  MAX_COIL_CHANNEL_MODE,   sizeof(int32_t)),
    INIPP(AUTO_GET_INTERVAL_FLAG,      FALSE,  AUTO_GET_INTERVAL_FLAG,   sizeof(int32_t)),
    INIPP(HALA_ENABLE_FLAG,         FALSE,  HALA_ENABLE_FLAG,      sizeof(bool_t)),
    INIPP(HALA_INVOKE_MODE,         FALSE,  HALA_INVOKE_MODE,      sizeof(int32_t)),
    INIPP(HALA_PLANNED_STATUS,      FALSE,  HALA_PLANNED_STATUS,   sizeof(int32_t)),
    INIPP(CALC_SAR_FLAG,			FALSE,  CALC_SAR_FLAG,   sizeof(bool_t)),
    INIPP(SAR_RATIO_PROLIM_LIMIT,	FALSE,  SAR_RATIO_PROLIM_LIMIT,   sizeof(int32_t)),
    INIPP(RTSAR_CTRL_FLAG,			FALSE,  RTSAR_CTRL_FLAG,   sizeof(bool_t))
};  /* end of pr_init[] table. */

/* == K_PR_ALL */
static int pr_init_count = sizeof(pr_init) / sizeof(pr_init[0]);

#endif // #ifndef __SM_IPCLIB_PROTOCOLUTIL_H__ 
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: pqmconstants.h
 *  Overview: Structure of CPQMConstants class.
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
// This header file contains all the defined constants which is used in PQM-IPC.
#ifndef __SM_IPCLIB_PQMCONSTANTS_H__
#define __SM_IPCLIB_PQMCONSTANTS_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include <tami/acqman/AM_ClientMessages.h>
#include <PQM/PQMCommanMessages.h>
#include "pqmmessagestructures.h"
#define PQM_IPC_INI_SEC             _T("PQMIPC")
#define PQM_IPC_LOG_FILE            _T("LOG_FILE")

#define FIRST                       _T("FIRST ")
#define ANATOMY                     _T("ANATOMY ")
#define COIL                        _T("COIL ")
#define TX_TABLE                    _T("TX_TABLE ")
#define RFL_TABLE                   _T("RFL_TABLE ")
//Patni-DKH/2010Feb18/Added/Phase3#MCM0229-00187
#define CFA_TABLE                   _T("CFA_TABLE")
#define COUCH                       _T("COUCH ")

//Patni-ARD/2010Apr08/Commented/JaFT# IR-100
//#define DEFAULT_CONTRASTAGENT       _T("Gadolinium") //SM3_Mangesh
#define DEFAULT_CONTRASTAGENTINDEX       _T("1") //SM3_Mangesh
#define CDS                         _T("CDS")//+Patni-HAR/2010Feb10/Modified/CDS Requirement

#define AUTO_SEP					_T("\\PQM\\AutoSepDynamic.ini")
#define EARLY_SCAN_TIME_SECTION		_T("Early Scan Time")
#define EARLY_SCAN_TIME_KEY			_T("Time")
const int DEFAULT_DELAY_TIME		= 3;

const int ONE_SEC = 1000;
const int SIXTY_SEC = 60;
const int MAX_NEW_DELAY = 5999;
const int ZERO_ASCII = 48;
const int MIN_TIMER_TIME	= 0;

const int PQM_INVALID_PROTOCOL	= -1;
const int PQM_NORMAL_PROTOCOL	= 0;
const int PQM_SLAVE_PROTOCOL	= 1;
const int PQM_MASTER_PROTOCOL	= 2;

const int FIRST_ROW_INDEX		= 0;
const int SECOND_ROW_INDEX		= 1;
const int THIRD_ROW_INDEX		= 2;
const int FOURTH_ROW_INDEX		= 3;


//+Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
/***** cds_step *********/
#define	CDS_STEP0_NONE					0
#define	CDS_STEP1_RFINTERLOCK_LEVEL		1
#define	CDS_STEP2_MOVECOUCH_LEVEL		2
#define	CDS_STEP3_SHIMCTRL_LEVEL		3
#define	CDS_STEP4_SCAN					4
#define	CDS_STEP5_SCANDONE				5
#define	CDS_STEP6_DONE					6



/* for CDS */
#define PQM_CDS_NONE			0x0000
#define PQM_CDS_SETTING			0x0001
#define PQM_CDS_SCAN			0x0002
#define PQM_CDS_PREV			0x0004
#define PQM_CDS_NO_CONFIRM      0x0008


#define	CDS_TIMEOUT_DEFAULT	(30*1)	 /* default timeout 30sec */

//unsigned int Cds_timeout ;                 /* TIMER time */


#define PQM_CDS_MAX_DYNAMIC		16
//-Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774

#define PQM_MAX_PROTOCOLS           64
//#define K_PM_TYPE_AMB               0x000B0000
#define PQM_PQ_BREAK                (1<<3)

#define TO_STRING(x)                #x

//+Patni-PJS/Added/2011-Mar-11/REDMINE-1529
const int PROCESSING_VALUE_NONE     = 0;

const DWORD ERR_ID = 111;
//const int VFC_HOLD_COUCH = (1L<<10);
const unsigned long DEFAULT_TRACE_MARKER = 0xFFFFFFFF;

const int PQM_INFO_BIT_COILID       = 0x0001;
const int PQM_INFO_BIT_COUCH        = 0x0002;
const int PQM_INFO_BIT_GATE         = 0x0004;
const int PQM_INFO_BIT_RM           = 0x0008;
const int PQM_INFO_BIT_TEMP         = 0x0010;

const int PQM_ACQ_BIT_CONTROL       = 0x0001;
const int PQM_ACQ_BIT_GATEMODE      = 0x0002;
const int PQM_ACQ_BIT_ACQINFO       = 0x0004;
const int PQM_ACQ_BIT_RFINTERLOCK   = 0x0008;
const int PQM_ACQ_BIT_SHIMCTRL      = 0x0010;
const int PQM_ACQ_BIT_MOVECOUCH     = 0x0020;
const int PQM_ACQ_BIT_REMOTE_MOVECOUCH = 0x0040;
const int PQM_ACQ_BIT_PRESCAN       = 0x0100;
const int PQM_ACQ_BIT_ACQUISITION   = 0x0200;
const int PQM_ACQ_BIT_SCAN_PROC     = 0x1000;
const int PQM_ACQ_BIT_PRESCAN_HOOK  = 0x4000;

const int PQM_ACQ_BIT_OLPTEMP		= 0x8000;	//+Patni-Ravindra Acharya/2010June09/Added/IR-153
const int PQM_ACQ_BIT_SAEINFO		= 0x00010000;
const float  PQM_MIN_B1RMS          = 0.01f;
#define E_NO_ERROR      0
#define E_ERROR         1
#define E_ERROR_STAT    -1

//const int VFC_PRESCAN_GAIN_MEASURE = 0;
//const int VFC_PRESCAN_GAIN_CALCULATE = 3;
//const int VFC_PRESCAN_LOAD_VARFIELD = 4;
//const int VFC_ACQ_STATUS_WAIT = 0;
//const int VFC_ACQ_STATUS_FAIL = 2;
//const int VFC_ACQ_STATUS_SUCCESS = 1;
//const int VFC_ACQ_STATUS_CURRENT = 3;

//const int VFC_PROBE_RTMC = 1;
//const int VFC_ACQ_SPLIT_MODE_PREP = 2;

#define C_PR_STATUS_WAIT            VFC_ACQ_STATUS_WAIT
#define C_PR_STATUS_DONE            VFC_ACQ_STATUS_SUCCESS
#define C_PR_STATUS_FAIL            VFC_ACQ_STATUS_FAIL
#define C_PR_STATUS_CURRENT         VFC_ACQ_STATUS_CURRENT

//const int VFC_HOLD_MAIN_SCAN = (1L<<1);

#define PQM_PQ_BREAK                (1<<3)
//+Patni-Ravindra Acharya/2010June09/Added/IR-153
#define PQM_TEMP_PRE	0x0001	/* Measurement at time of first prescan */
#define PQM_TEMP_SHIM	0x0002	/* Measurement after shimming scan */

//-Patni-Ravindra Acharya/2010June09/Added/IR-153

/* for CFA Retry */
const int PQM_CFA_NONE              = 0;
const int PQM_CFA_RETRY_AND_SCAN    = 2;
const int PQM_CFA_RETRY             = 1;
const int PQM_START_BTN_NORMAL      = 0;
const int PQM_START_BTN_APC         = 1;

//const int PQM_CFA_RETRY_AND_SCAN = 2;

/* pqm type */
const int K_SCHEDULING      = 0;
const int K_IMAGING         = 1;
//Patni-PJS/2010Feb24/Added/DeFT# PROPOSAL_08-Remove EZScan
const int K_INIT = 99;


/* VarField:    SVN_PLN_MOVING_COUCH_FLG
 */
//const int VFC_MOVING_COUCH_NONE = 0;
//const int VFC_MOVING_COUCH_CONTINUOUSLY = 1;
//const int VFC_MOVING_COUCH_MANUAL = 2;
//const int VFC_MOVING_COUCH_AUTOMATIC = 3;

//const int VFC_HOLD_COVERAGE = (1L<<2);
//const int VFC_HOLD_MOVE_BED = (1L<<4);
//const int VFC_HOLD_APC = (1L<<16);
//const int VFC_HOLD_CFA = (1L<<17);
//const int VFC_HOLD_RGN = (1L<<18);

/* Set Scan Mode */
const int K_MODE_SET_IT	    = 0;
const int K_MODE_SET_ALL    = 1;

/* For Scan Start button. */
//const int PQM_START_BTN_NORMAL = 0;
//const int PQM_START_BTN_APC    = 1;

/* For Pause/Resume button */
const int PQM_PAUSE_BTN     = 0;
const int PQM_RESUME_BTN    = 1;

/* sys_type := (Magnetic Intensity) << 16 | (Grade)
 * Grade := (Type << 8) | (Index)
 */

// commented MAX_BITFLD as the value had already been defined some where in VfLib
//const int MAX_BITFLD = 512;
const int MAX_FIELDS    = 20;
const int TRY_MAX       = 5;
//27 Feb 2006
//const int SVN_OFF_EXECUTION = 5000;
//const int VFN_STUDY_BASE = 0;
//const int SVN_MIN_PROTOCOL = VFN_STUDY_BASE + 40000;
//const int SVN_BASE_PROTOCOL_EXECUTION = SVN_MIN_PROTOCOL + SVN_OFF_EXECUTION;
//const int SVN_EXE_PARENT_COUCH_OFFSET = SVN_BASE_PROTOCOL_EXECUTION + 31;

#define ABSF(a) ((a) < 0.0 ? (-(a)) : (a))

//const long int VFC_HOLD_PRE_SCAN = (1L<<0);
//const int VFC_ACQ_STATUS_SUCCESS = 1;
//const int VFC_ACQ_STATUS_CURRENT = 3;
//const int VFC_PLN_STATUS_GLOC = (1<<9);

//const int C_PR_STATUS_DONE = VFC_ACQ_STATUS_SUCCESS;
//const int C_PR_STATUS_CURRENT = VFC_ACQ_STATUS_CURRENT;
const int PQ_FILLER        = -1;
const int PQ_FILLER_DONE   = -2;
const int PQ_FILLER_CURR   = -3;
const int PQ_FILLER_WAIT   = -4;

#define PQM_STS_LIST                (1<<PQM_LIST_ID_STS)
#define PQM_LIST_ID_STS             1
#define VFC_HOLD_PRE_SCAN           (1L<<0)
#define VFC_MODE_AUTO_VIEW_CS       (1L << 2)   // Display Center Slice via Auto View
#define VFC_AFTER_REFVIEW_AUTO      0
#define VFC_AFTER_REFVIEW_MANUAL    1

#define APPLICATION  _T("PQM-IPC APPLICATION")

////Ashish SM3
//#define IS_SCANOFFSET_DIALOG	1
//#define IS_CFA_DIALOG	2
//#define IS_GATE_INFO_DIALOG	3
//#define IS_COIL_INFO_DIALOG	4
//#define IS_COIL_SELECTION_DIALOG	5
////Ashish SM3

/* protocol tags */
typedef	enum	_pr_tag_e {
    K_PR_DUMMY = 0,
    K_PR_APP_CODE,                  /* Application Code */
    K_PR_SEQUENCE_NAME,             /* Sequence Name */
    K_PR_IMAGING_MODE,              /* 2D/3D */
    K_PR_SCAN_ID,                   /* Scan Comment     */
    K_PR_SCAN_COMMENTS,             /* Scan Comment     */
    K_PR_TOTAL_TIME,                /* Total time for scan  */
    K_PR_ACQ_TIME,                  /* Subprotocol time */
    K_PR_ACQ_STATUS,                /* Acquire Status   */
    K_PR_IMG_GR,                    /* Image Group ID   */
    K_PR_CONTRAST,                  /* Contrast Agent use   */
    K_PR_CONTRAST_NAME,             /* Contrast Agent name  */
    K_PR_PATIENT_ORIENT,            /* Patient Orientation  */
    K_PR_RC_COIL,                   /* Reciver Coil     */
    K_PR_TR_COIL,                   /* Transrate Coil   */
    K_PR_RC_COIL_NAME,              /* Reciver Coil     */
    K_PR_TR_COIL_NAME,              /* Transrate Coil   */
    K_PR_HOLD,                      /* Hold Scan        */
    K_PR_AUTO_HOLD,                 /* Auto Hold Scan   */
    K_PR_AUTO_POSTPROC_FLG ,        /* Auto TRANSFER    */
    K_PR_CDS_FLAG ,					/* Flags whether the automatic settings*/
    K_PR_ANATOMY,                   /* Anatomy      */
    K_PR_GATE,                      /* for Gate Scan    */
    K_PR_DYNAMIC,                   /* for Dynamic Scan */
    K_PR_INTERMIT_FLAG,             /* Intermit Flag */
    K_PR_INTERMIT_TIME,             /* Intermit time */
    K_PR_COUCH,                     /* Couch position   */
    K_PR_HOME_POSITION,             /* Couch Home Position  */
    K_PR_COUCH_OFFSET,              /* Couch Offset     */
    K_PR_PARENT_COUCH_OFFSET,       /* Parent Couch Offset  */
    K_PR_SCAN_NUMBER,               /* scan number.     */

    K_PR_PRESCAN_FSY,               /* PreScan FSY Action flag. */
    K_PR_PRESCAN_RFL,               /* PreScan RFL Action flag. */
    K_PR_PRESCAN_RGN,               /* PreScan RGN Action flag. */

    K_PR_PRESCAN_FSY_VALUE,         /* PreScan FSY Value */
    K_PR_PRESCAN_RFL_GAIN,          /* PreScan RFL Gain */
    K_PR_PRESCAN_RFL_SCALER,        /* PreScan RFL Scaler */
    K_PR_PRESCAN_RGN_VALUE,         /* PreScan RGN Value */
    K_PR_PRESCAN_RGN_OFFSET,        /* PreScan RGN Offset */
    K_PR_DEFAULT_RGN_VALUE,         /* Default RGN Value */

    K_PR_LOCATOR_IMAGE,             /* for parent image.        */

    K_PR_CALC_SAR,                  /* for SAR calc.        */
    K_PR_SEQ_SHIM,                  /* Shimming sequence flag.  */
    K_PR_SEQ_FLUORO,                /* Fluoro sequence flag.    */

    K_PR_SAR,                       /* SAR value            */

    K_PR_STATUS,                    /* Protocol Status      */
    K_PR_VIEW_DIR,                  /* Viewing Direction        */

    K_PR_PLN_AUTO_VOICE,            /* AUTO VOICE */
    K_PR_EXE_AUTO_VOICE,            /* AUTO VOICE */

    K_PR_TIME_CTRL_PARAM,           /* AUTO VOICE (Dynamic) */
    K_PR_TPCTRL,                    /* AUTO VOICE (TPCTRL) */
    K_PR_MIN_TR,                    /* AUTO VOICE (TPCTRL) */
    K_PR_TR,                        /* AUTO VOICE (TPCTRL) */

    K_PR_SCAN_AGENT,                /* Scan Agent */
    K_PR_PREP_SCAN,                 /* Prep Scan */
    K_PR_MOVING_COUCH,              /* Moving Bed */
    K_PR_POST_ACQ_MODE,             /* Reconstruction Mode */
    K_PR_FLY_FLAG,                  /* OnTheFly Flag */
    K_PR_PAC_PATTERN,
    K_PR_NUM_PAC_SCAN,
    K_PR_SPEEDER_TYPE,              /* V5.00 Speeder */
    K_PR_CHANNEL_PER_VFEP,          /* Channel Per VFEP */
    K_PR_INTENSITY_CORR_FLAG,       /* V5.00 Intensity */
    K_PR_ACQ_SPLIT_MODE,            /* acquisition Split mode */
    K_PR_AUTO_PROEDIT_FLAG,         /* V5.00 AutoProEdit  */
    K_PR_PROBE_FLAG,                /* V5.00 PROBE FLAG */
    K_PR_GATE_PARAM,                /* V5.10 GATE PARAM */
    K_PR_DBDT_MODE,                 /* V5.00 dBdt mode */
    K_PR_DBDT_OPERATING_MODE,       /* V5.00 dBdt Operation */
    K_PR_DBDT_VALUE,                /* V5.00 dBdt Operation */
    K_PR_DRKS_FLAG,                 /* V5.10 DRKS flag */
    K_PR_RECON_START_AFTER_REFVIEW, /* V5.10 DRKS Recon mode */
    K_PR_REPETITIONS,               /* V5.30 MovingBed for PVC */
    K_PR_COUCH_TABLE,               /* V5.30 MovingBed for PVC */
    K_PR_CHANGED_RECEIVE_COIL,      /* V5.30 MovingBed for PVC */
    K_PR_RECVR_COIL_TBL,            /* V5.30 MovingBed for PVC */
    K_PR_RECVR_COILNAME_TBL,        /* V5.30 MovingBed for PVC */
    K_PR_PAC_PATTERN_TBL,           /* V5.30 MovingBed for PVC */
    K_PR_CHANNEL_GAIN_RATIO_TBL,    /* V5.30 MovingBed for PVC */
    K_PR_DBDT_MODE_2002,            /* V6.00 dBdt 2002 mode */
    K_PR_PAC_SCAN_FLAG,             /* V6.20 for MRS */
    K_PR_LOCAL_CF_FLAG,             /* V6.25 for Local CF */
    K_PR_PROBE_CORRECTION,          /* V6.50 RefProbe */

    K_PR_AMB_ENABLE_FLG,            /* V7.10 AMB Enable flag */
    K_PR_AMB_MODE_FLG,              /* V7.10 AMB Mode flag */
    K_PR_NUM_SECTION_RECVR_COIL,    /* V9.00 Number of Section  for Saturn */
    K_PR_LDB_MAPPED_ID,             /* For reading the series loid*/
    K_PR_LOCATOR_IMAGE_ID,			/* For reading the Parent Image LOID*///+PATNI-MP added 08-07-09/ACE-2

    //+Patni-HEMANT/ADDED On 6/17/2009 4:22:38 PM/ ACE-2 / WFDA
    K_PR_INSCAN_FLAG,
    K_PR_INSCAN_PREFERENCE_GUID,
    //-Patni-HEMANT/ADDED On 6/17/2009 4:22:38 PM/ ACE-2 / WFDA
    //Patni-MP/ADDED On 6/17/2009 4:22:38 PM/ ACE-2 / CR MANUAL PRESCAN
    K_PR_PRESCAN_RFL_GAIN_VALUE_LIMIT,

    //+Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image
    K_PR_MOVE_COUCH_METHOD_FLAG,
    K_PR_COPY_SOURCE_PROTOCOL,
    //-Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image
    //Patni-DKH/2010Feb18/Added/Phase3#MCM0229-00187
    K_PR_RFL_COUCH,				    /* V9.50 RFLêQë‰à íu */
    K_PR_CFA_COUCH,				    /* V9.50 CFAêQë‰à íu */
    //Patni-DKH/2010Feb18/Added/Phase3#MCM0229-00187
    //Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement

    K_PR_MRS_EDDY_AXIS,             //Patni-HAR/2010Apr26/Added/IR-134
    K_PR_PC_VAP_MODE,   //Patni-HAR/2010Jun29/Added/V1.30#IR-158
    //Patni/SS/2010July20/Added/NFD001-AutoShim
    K_PR_SHIM_REQUIRED,
    K_PR_MAP_REQUIRED,
    //Patni-PJS/2011Feb09/Added/MVC009736
    K_PR_COVERAGES,

    K_PR_CALA_ENABLE_FLAG,
    K_PR_CALA_INVOKE_MODE,
    K_PR_CALA_PLANNED_STATUS,
    K_PR_INTENSITY_CORR_STATUS,
    K_PR_MAX_COIL_CHANNEL_MODE,
    K_PR_MULTI_SERIES_DYNAMIC_ENABLE_FLAG,
    K_PR_AUTO_GET_INTERVAL_FLAG,
    K_PR_HALA_ENABLE_FLAG,
    K_PR_HALA_INVOKE_MODE,
    K_PR_HALA_PLANNED_STATUS,
    K_PR_CALC_SAR_FLAG,
    K_PR_SAR_RATIO_PROLIM_LIMIT,
    K_PR_RTSAR_CTRL_FLAG,
    K_ALL_PR                        /* All Protocols */
} pr_tag_e;

//+Patni-DKH/2010Apr12/Modified/Phase3#MCM0229-00187
typedef enum {
    K_PRESCAN_MODE_1ST_SCAN = 0,
    K_PRESCAN_MODE_CFA_RFL,
    K_PRESCAN_MODE_CFA_TABLE,
    K_PRESCAN_MODE_RFL,
    K_PRESCAN_MODE_EVERY,
    K_PRESCAN_MODE_APC,
    K_PRESCAN_MODE_CFA,
    K_PRESCAN_MODE_AMB,	/* V7.10 */
    //Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
    K_PRESCAN_MODE_CDS

} PqmPreScanKinds;
//-Patni-DKH/2010Apr12/Modified/Phase3#MCM0229-00187

/*typedef enum {
    AMCM_FUNC_KIND_REQUEST = 1,
    AMCM_FUNC_KIND_REPLY,
    AMCM_FUNC_KIND_ANSWER,
    AMCM_FUNC_KIND_STATUS,
    AMCM_FUNC_KIND_UNKNOWN
} func_kind_t;*/


//const int   VFC_PATORI_HEAD_FIRST = 0;
//const int   VFC_PATORI_FEET_FIRST = 1;
//const int   VFC_PATORI_PRONE = 0;
//const int   VFC_PATORI_SUPINE = 1;
//const int   VFC_PATORI_LEFT_UP = 2;
//const int   VFC_PATORI_RIGHT_UP = 3;
//
//const int   C_PR_ORIENT_HD = VFC_PATORI_HEAD_FIRST;
//const int   C_PR_ORIENT_FT = VFC_PATORI_FEET_FIRST;
//const int   C_PR_ORIENT_PR = VFC_PATORI_PRONE;
//const int   C_PR_ORIENT_SP = VFC_PATORI_SUPINE;
//const int   C_PR_ORIENT_LUP = VFC_PATORI_LEFT_UP;
//const int   C_PR_ORIENT_RUP = VFC_PATORI_RIGHT_UP;

const int   PQM_NUM_COIL_PORT       = ACQ_COILINFO_PORT_CNT;
const int   COIL_CHANNEL_MASK       = 0x00FF0000;
const float COUCH_MIN_VALUE         = ((float)0.00001f);
const float COUCH_ACCURACY_RANGE_MM = ((float)1.0);


#define VUI_SHIFT_MODE_VG           (1<<0)
#define VUI_SHIFT_MODE_SG           (1<<1)
#define VUI_SHIFT_X                 (1<<0)
#define VUI_SHIFT_Y                 (1<<1)
#define VUI_SHIFT_Z                 (1<<2)

#define IS_PLANNED_FOR_NON_AMB(obj) (obj->GetHasParent() || obj->GetPlnStatus() & VFC_PLN_STATUS_GLOC)
#define IS_AMB(obj) ((VFC_AMB_MODE_NONE != obj->GetAMBModeFlag()) && (obj->GetAMBEnableFlag() || VFC_AMB_MODE_SLAVE == obj->GetAMBModeFlag()))
#define PLAN_CHECK(obj) (IS_PLANNED_FOR_NON_AMB(obj) || (!IS_PLANNED_FOR_NON_AMB(obj) && IS_AMB(obj)))

#define COUCH_OFFSET(obj)	(((obj)->GetPqmPrMode()->GetPatoriEndIn()==VFC_PATORI_HEAD_FIRST) ? \
                             ((obj)->GetPqmPrMode()->GetHomePos() - (obj)->GetPqmPrMode()->GetCouchPos()) : \
                             ((obj)->GetPqmPrMode()->GetCouchPos() - (obj)->GetPqmPrMode()->GetHomePos()))

const int   TRANS_ACQINFO       = 1;
const int   TRANS_RFINTERLOCK   = 2;
const int   TRANS_PRESCAN       = 3;
const int   TRANS_SCAN          = 4;

const int   SCAN_START_BTN = 1;
const int   SCAN_PAUSE_BTN = 1;
const int   SCAN_ABORT_BTN = 1;

const int	TIME_OUT_VAL_FOR_RECON = 120000;
const int	AL_PROTOCOL_SLICE_NUM_LIMIT	= 3;
const int	CALA_PROTOCOL_SLAB_LIMIT		= 1;
const int	HALA_PROTOCOL_SLAB_LIMIT		= 2;

#define VFC_SPEEDER_TYPE_SENSITIVITY 1

typedef enum _sar_status_e {
    PQM_SAR_OK = 0,		    /* SAR is good for Scan. */
    PQM_SAR_OK_BUT_SAR,		/* SAR is NG of SAR, but Scan ok. */
    PQM_SAR_OK_BUT_COIL,	/* SAR is NG of COIL, but Scan ok. */
    PQM_SAR_OK_BUT_RF,	    /* SAR is NG of RF, but Scan ok. */
    PQM_SAR_OK_BUT_LIB,		/* SAR is NG of SAR, but Scan ok. */
    PQM_SAR_ERROR,		    /* SAR is NG of COIL, but Scan ok. */
    PQM_SAR_LIMIT_OVER,		/* SAR is NG */
    PQM_SAR_COIL_PROTECT,	/* SAR is NG for coil protect. */
    PQM_SAR_RF_PROTECT,	    /* SAR is NG for RF protect. */
    PQM_SAR_LIB_ERROR,	    /* libSAR Error V9.25 */
    PQM_SAR_SAE_ERROR,		/* SAR is NG for SAE, Scan not ok. */
    PQM_SAR_SAE_WARN,		/* SAR is NG for SAE, but Scan ok with warning. */
    PQM_SAR_COIL_PROTECT_WAIT,							/*SAR is NG, Scan after wait time*/
    PQM_SAR_LIMIT_OVER_WAIT,							/*SAR is NG of COIL, Scan after wait time*/
    PQM_SAR_COIL_PROTECT_6MIN_WAIT_FOR_AUTO_START,		/*SAR is NG of COIL, Auto Scan after wait time*/
    PQM_SAR_LIMIT_OVER_6MIN_WAIT_FOR_AUTO_START,			/*SAR is NG, Scan after wait time*/
    PQM_SAR_COIL_PROTECT_10SEC_WAIT_FOR_AUTO_START,		/*SAR is NG of COIL, resume scan after 1 sec time */
    PQM_SAR_LIMIT_OVER_10SEC_WAIT_FOR_AUTO_START,		/*SAR is NG, resume scan after 1 sec time*/

    PQM_SAR_ALL_STS			/* This should be last enum always*/

} sar_status_e;

typedef enum CHECK_BIT {
    PQM_CHK_BIT_FIRST = 0x0001,
    PQM_CHK_BIT_COIL =  0x0020,
    PQM_CHK_BIT_ANATOMY = 0x0040,
    PQM_CHK_BIT_COUCH = 0x0080,
    PQM_CHK_BIT_TX_TABLE = 0x0100,
    PQM_CHK_BIT_RFL_TABLE = 0x0200,
    PQM_CHK_BIT_CFA_TABLE =	0x0400, //Patni-DKH/2010Feb18/Added/Phase3#MCM0229-00187
    PQM_CHK_BIT_MOVE_COUCH = 0x1000,
    PQM_CHK_BIT_AMB_SCAN = 0x2000,
    //+Patni-Himanshu/2010Feb10/Added/ACE-Phase#3/CDS Requirement
    //PQM_CHK_BIT_PHCOR = 0x4000,
    PQM_CHK_BIT_CDS = 0x4000,
    //-Patni-Himanshu/2010Feb10/Added/ACE-Phase#3/CDS Requirement
};

#define AGENT_KEY1                  RG
#define AGENT_KEY2                  CF
//#define K_PM_TYPE_AMB               0x000B0000

const int PQM_SAR_RECALC_AND_CHECK	=	-1;

//DICITONARY related constant values
#define PAS_COL_WD          150
#define DEFAULT_COL_WIDTH   40

#define CENTER               2
#define RIGHT                1
#define LEFT                 0
#define DEFAULT_APP_DIC_ALIGNMENT CENTER

typedef enum {
    PQM_AV_VOICE_1 = 1,
    PQM_AV_VOICE_2,
    PQM_AV_VOICE_3,
    PQM_AV_VOICE_4,
    PQM_AV_VOICE_5
} VOICEMGR_PARAM;

const int PQM_AV_NUM_VOICES         =   5;
typedef enum {
    PQM_AV_CMD_QUIT = 0,
    PQM_AV_CMD_PLAY = 1,
    PQM_AV_CMD_PLAY_W,
    PQM_AV_CMD_DELAY,
    PQM_AV_CMD_DELAY_W,
    PQM_AV_CMD_VOLUME,
    PQM_AV_CMD_VOLUME_W,
    PQM_AV_CMD_PROG,
    PQM_AV_CMD_PROG_W,
    PQM_AV_CMD_PDELAY,
    PQM_AV_CMD_PDELAY_W,
    PQM_AV_CMD_INTERVAL ,
    PQM_AV_CMD_INTERVAL_W,
    PQM_AV_CMD_TRAP,
    PQM_AV_CMD_TRAP_W
} VOICEMGR_CMD;


//DICITONARY related constant values


//  For Shimming State
const int SHIM_STATE_WAIT           =   0;
const int SHIM_STATE_READY          =   1;
const int SHIM_STATE_ACTIVE         =   2;


//const int VFC_LANG_ENGLISH          =   0;
//const int VFC_LANG_JAPANESE         =   1;
const int PQM_MAX_MOVINGBED_SCAN    =   128;
const int PQM_LOID_LEN				=   26;


/* Force prescan message. */
const int PQM_WARN_PRESCAN_NUMBER           = 0;
const int PQM_WARN_PRESCAN_COUCH            = 1;
const int PQM_WARN_PRESCAN_ANATOMY          = 2;
const int PQM_WARN_PRESCAN_COIL             = 3;
const int PQM_WARN_PRESCAN_DIFF_COIL        = 4;
const int PQM_WARN_PRESCAN_MOVING_COUCH     = 5;
const int PQM_WARN_PRESCAN_PUMP             = 6;
const int PQM_WARN_PRESCAN_FREEZER          = 7;
const int PQM_WARN_PRESCAN_PUMP_AND_FREEZER = 8;
const int PQM_WARN_PRESCAN_SPEEDER_COUCH    = 9;
const int PQM_WARN_PRESCAN_SPEEDER_COIL     = 10;
const int PQM_WARN_PRESCAN_INTENSITY_COUCH  = 11;
const int PQM_WARN_PRESCAN_PVC_COUCH_POSI   = 12;
const int PQM_WARN_PRESCAN_HOME_POS         = 13;
const int PQM_WARN_PRESCAN_PROJECTION       = 14;

/* Force scan message */
const int PQM_WARN_SCAN_AUTO_VOICE          = 100;
const int PQM_WARN_SCAN_MULTI_COVERAGES     = 101;
const int PQM_WARN_SCAN_INTENSITY_COIL      = 102;
const int PQM_WARN_SCAN_INTENSITY_COUCH     = 103;
const int PQM_WARN_SCAN_INTENSITY_MAP       = 104;
const int PQM_WARN_SCAN_INTENSITY_AREA      = 105;
const int PQM_WARN_SCAN_PREP_DELAY_SKIP     = 106;
const int PQM_WARN_SCAN_PREP_STUDY          = 107;
const int PQM_WARN_SCAN_DRKS_RECON_MODE     = 108;
const int PQM_WARN_SCAN_PV_MOVINGBED_SPEEDER = 109;
const int PQM_WARN_SCAN_PV_MOVINGBED_INTEN  = 110;
const int PQM_WARN_SCAN_HR_SHIMMING         = 111;
const int PQM_WARN_TX_INTENSITY_NOTFOUND    = 113;
const int PQM_WARN_INTENSITY_NOTFOUND       = 114;
const int PQM_WARN_SCAN_TX_INTENSITY_AREA	= 115;
const int PQM_WARN_SCAN_TX_DEGENERACY_MAP 	= 116;
const int MSG_SET_RCV_COIL                  = 200;
const int MSG_SET_SAR_ANATOMY               = 201;
const int ERR_ACQ_LOCK_FILE                 = 202;
/* enable receviced WholeBodyCoil V5.00   */
//const int RWB_SPEEDER = 1;
//const int RWB_PREP = 2;
//const int RWB_SHIMMING = 4;

const int ezscan_retry = 0; /* V5.00 */
const int NUM_ACQ_FLAG = 3;
//const int ACQ_RMSTATUS_VACUME_LOW = 0x00000001;             /* vacume level low             */
//const int ACQ_RMSTATUS_FREEZER_STOP =   0x00000002;             /* freezer stop             */
//const int VFC_AMB_MODE_NONE = 0;    /*Å@í èÌéBëú */
//const int VFC_AMB_MODE_MASTER = 1;  /* AMB Mater protocol */
//const int VFC_AMB_MODE_SLAVE = 2;   /* AMB SLAVE protocol */
const int PQM_ACQ_BIT_WARNING = 0x2000;

const int PQM_MAX_STRING = 512;
const int PQM_NUM_VFEP_CHANNEL = 8;
enum {
    PQM_SCAN_START = 1,
    PQM_SCAN_RESTART,
    PQM_APC_START,
    PQM_SCAN_ABORT,
    PQM_SCAN_PAUSE,
    PQM_SCAN_RESUME
};
const int PQM_SOCKET_ACQMAN     = 100;
const int PQM_SOCKET_VOICEMGR   = 101;
const int PQM_SOCKET_IRS        = 102;
const int PQM_SOCKET_REFPROBE   = 103;

const int PQM_FREEZE_FOR_PDET       = (1L << 0);
const int PQM_FREEZE_FOR_MEMORY     = (1L << 1);
const int PQM_FREEZE_FOR_PROLIM     = (1L << 2);
const int PQM_FREEZE_FOR_SHIMMING   = (1L << 3);
const int PQM_FREEZE_FOR_SCAN       = (1L << 4);
const int PQM_FREEZE_FOR_VUICOPY    = (1L << 5);
const int PQM_FREEZE_FOR_SCAN_PROC  = (1L << 6);
const int PQM_FREEZE_FOR_SFT        = (1L << 7);
const int PQM_FREEZE_FOR_SPEEDER    = (1L << 8);

//Patni-MSN/2009Mar19/Added/PSP-Warning Removal
const float PQM_COUCH_INIT_POSITION = 99999.0;

const int NUM_0             = 0;		//Patni/2011Mar31/Added/V2.0/IR-181_NFD1_Prod_Req_Part1
const int NUM_1             = 1;
const int NUM_2             = 2;
const int NUM_3             = 3;
const int NUM_4             = 4;
const int NUM_5             = 5;
const int NUM_6             = 6;
const int NUM_7             = 7;
const int NUM_16            = 16;
const int NUM_10            = 10;
const int NUM_11            = 11;
const int NUM_30            = 30;
const int NUM_32            = 32;
const int NUM_64            = 64;
const int NUM_100           = 100;
const int NUM_1000          = 1000;
const int NUM_126           = 126;
const int NUM_128           = 128;
const int READ_BUFFER       = 512;
const int PORT_MIN_NUMBER   = 1024;
const int PORT_MAX_NUMBER   = 65535;
const int COUCH_POSITION    = 99999;
const int PATH_BUFFER       = 8;
const int AUTO_VOICE_VALUE  = 0x0F;
const int FILE_BUFFER       = 1024;
const int AV_ACQ_TIME       = 1000;
const int STUDY_BUFFER      = 256;
const int SLEEP_TIME        = 5000;
const int COUCH_INLIMIT     = -2050;
const int COUCH_MAXIMUM_MOVEMENT        = 1200;
const int COUCH_MAXIMUM_PARENT_OFFSET   = 150;
const int COUCH_SLOW_SPEED_RANGE        = 50;
const int COUCH_OFFSET                  = 0;
const int COUCH_HOME_POSITION   = -2000;
const int WEIGHT_CLASS_ZERO     = 20;
const int WEIGHT_CLASS_ONE      = 40;
const int WEIGHT_CLASS_TWO      = 60;
const int WEIGHT_CLASS_THREE    = 80;
const int WEIGHT_CLASS_FOUR     = 100;
const double PROTOCOL_HOME_POS  = 0.00001;
const int FILE_MODE             = 0;
const int INTERMIT_TIME         = 1000;
const int TP_DURATION           = 1000;
const int PROTOCOL_OFFSET       = 1000;
const int YEAR_OFFSET           = 1900;
const int TIME_OFFSET           = -200;
const double DIVISION_FACTOR_COUCH_POSTION = 1000.0;
const double DIVISION_FACTOR_HOME_POSTION  = 1000.0;
const int MAXPROTOCOL_DEFAULT   = 64;
const int INVALID_COLOR_VALUE   = -10;
const int INVALID_XML_FILE      = -20;
const int INVALID_PROTOCOL_STATUS = -30;
const int INVALID_TRACE_PATH    = -40;
const int INVALID_EVENT_PATH    = -50;
//SM3_Mangesh_Start
const int MAXCONTRAST_DEFAULT   = 5;
const int MAXSCANANATOMY_DEFAULT = 32;
const int MAXSARANATOMY_DEFAULT = 11;
//SM3_Mangesh_End


// file names
const CPTString COILDB_FILENAME                     = L"CoilDb.xml";
const CPTString INTERLOCK_FILENAME                  = L"interlock.xml";
const CPTString WEIGHT_RATIO_FILENAME               = L"Weight_Ratio.xml";
const CPTString APPCONFIG_FILENAME                  = L"PQM_AppConfig.xml";
const CPTString SHARED_MEMORY_COUCH_INFO_FILENAME   = L"sharedmemorycouchinfo.xml";
const CPTString SHARED_MEMORY_TUNE_INFO_FILENAME    = L"sharedmemorytuneinfo.xml";
const CPTString SHARED_MEMORY_SHIM_STATE_FILENAME   = L"sharedmemoryshimstate.xml";
const CPTString COLOR_SETTING_FILENAME              = L"PQM_ColorSetting.xml";
//SM3_Mangesh_Start
const CPTString CONTRAST_AGENTS_FILENAME            = L"PQM_ContrastAgent.xml";
const CPTString SCAN_ANATOMY_FILENAME               = L"PQM_ScanAnatomy.xml";
//SM3_Mangesh_End
//Patni-ARD/2009Oct28/Added/IR 85 Auto Transfer
const CPTString SCAN_APP_SITE_FILENAME              = L"MPlusMRSrvScanAPPSite.xml";
//Patni-RSG/2009Nov11/Added/IR-116
const CPTString MRS_SHIM_CONFIG_FILE_NAME           = L"MRSShimConfig.xml";


//+Patni-Ajay/2010July23/Added/PlanDuplicate/REDMINE-443
const CPTString PRESET_ITEM_FILENAME                = L"PQM_PresetSetting.xml";
//-Patni-Ajay/2010July23/Added/PlanDuplicate/REDMINE-443
//Patni-AJS/2010Sep3/Added/ScanCommentDialog/IR-163
const CPTString SCAN_COMMENT_FILENAME                = L"pqm.scan.comment.xml";
//Patni/2011FEB17/Added/V2.0/IR-181_NFD1_Prod_Req_Part1
const CPTString AUTO_INSERT_FILENAME                 = L"AutoInsertProtocol.xml";
// Tag Names for Coil DB
const CPTString NODE_TAG_NAME_COIL_INFORMATION      = L"CoilInformation";
const CPTString CHILD_TAG_NAME_COIL_ID              = L"CoilId";
const CPTString CHILD_TAG_NAME_WBCOIL               = L"WBCoil";
const CPTString CHILD_TAG_NAME_CHANNEL              = L"Channel";
const CPTString CHILD_TAG_NAME_TYPE                 = L"Type";
const CPTString CHILD_TAG_NAME_PORT                 = L"Port";
const CPTString CHILD_TAG_NAME_LABEL                = L"Label";
const CPTString CHILD_TAG_NAME_PACPATTERN           = L"PacPattern";
const CPTString CHILD_TAG_NAME_SPEEDER_ENABLE       = L"SpeederEnable";
const CPTString CHILD_TAG_NAME_RECEIVER_ENABLE      = L"ReceiveWBEnable";
const CPTString CHILD_TAG_NAME_ENABLE               = L"Enable";
const CPTString CHILD_TAG_NAME_OUTSIDELIMIT         = L"OutsideLimit";
const CPTString CHILD_TAG_NAME_INSIDELIMIT          = L"InsideLimit";
const CPTString CHILD_TAG_NAME_TUNELIMIT            = L"TuneLimit";
const CPTString CHILD_TAG_NAME_RFLIMIT              = L"RFLimit";
const CPTString CHILD_TAG_NAME_COIL_DESCRIPTION     = L"CoilDescription";

// Tag Names for Interlock
const CPTString CHILD_TAG_NAME_ANATOMY              = L"anatomy";
const CPTString CHILD_TAG_NAME_WGT_CLASS1           = L"weight_class1";
const CPTString CHILD_TAG_NAME_WGT_CLASS2           = L"weight_class2";
const CPTString CHILD_TAG_NAME_WGT_CLASS3           = L"weight_class3";
const CPTString CHILD_TAG_NAME_WGT_CLASS4           = L"weight_class4";
const CPTString CHILD_TAG_NAME_WGT_CLASS5           = L"weight_class5";
const CPTString CHILD_TAG_NAME_WGT_CLASS6           = L"weight_class5";

// Tag Names for App Config
const CPTString NODE_TAG_NAME_PROTOCOL_VALUE        = L"ProtocolValue";
const CPTString CHILD_TAG_NAME_MAXLIMIT             = L"MaxLimit";
const CPTString NODE_TAG_NAME_UNICODE               = L"UNIASCII";
const CPTString CHILD_TAG_NAME_UNI_APP              = L"APP";
const CPTString CHILD_TAG_NAME_UNI_FORMAT           = L"Format";
const CPTString NODE_TAG_NAME_TRACELOG_VALUE        = L"TraceLog";
const CPTString CHILD_TAG_NAME_USESDK               = L"UseCSSDK";
const CPTString CHILD_TAG_NAME_TRACELOGPATH         = L"TraceLogPath";
const CPTString NODE_TAG_NAME_EVENTLOG_VALUE        = L"EventLog";
const CPTString CHILD_TAG_NAME_EVENTLOGPATH         = L"EventLogPath";
const CPTString APP_VAL                             = L"Acqman";
const CPTString UNICODE_SUPPORTED                   = L"UNICODE";
//SM3_Mangesh_Start
const CPTString NODE_TAG_NAME_CONTRASTAGENTS_VALUE  = L"ContrastAgentsValue";

const CPTString NODE_TAG_NAME_SCANANATOMY_VALUE     = L"ScanAnatomyValue";
const CPTString CHILD_TAG_NAME_SCANANATOMY_MAXLIMIT = L"ScanAnatomyMaxLimit";
const CPTString CHILD_TAG_NAME_SARANATOMY_MAXLIMIT  = L"SarAnatomyMaxLimit";

//SM3_Mangesh_End

//SM3_Mangesh_Start
//Tag Names for PQM Contrast Agent config.
const CPTString NODE_TAG_NAME_CONTRAST_AGENT        = L"ContrastAgent";
const CPTString CHILD_TAG_NAME_INDEX		        = L"Index";
const CPTString CHILD_TAG_NAME_VFINDEX		        = L"VFIndex";

//+Patni-Hemant/2010May20/Modified/MaFT/MVC007915
//Patni-Hemant/2010Apr26/Added/JaFT/IR-137
const CPTString CHILD_TAG_NAME_LITERALITY		    = L"Laterality";
//-Patni-Hemant/2010May20/Modified/MaFT/MVC007915


//Tag Names for PQM Scan Anatomy config.
const CPTString NODE_TAG_NAME_ANATOMY				= L"Anatomy";
const CPTString CHILD_TAG_NAME_SAR_ANATOMY          = L"SARAnatomy";
const CPTString NODE_TAG_NAME_SCAN_ANATOMY          = L"ScanAnatomy";
const CPTString NODE_TAG_NAME_SCAN_ANATOMY_COUNT    = L"ScanAnatomyCount";

//SM3_Mangesh_End

// Tag Names for Couch Info
const CPTString NODE_TAG_NAME_COUCH_INFO            = L"CouchInformation";
const CPTString CHILD_TAG_NAME_SCANNO               = L"Scanno";
const CPTString CHILD_TAG_NAME_COUCH_POS            = L"couchpos";
const CPTString CHILD_TAG_NAME_HOME_POS             = L"homepos";

// Tag Names for Tune Info
const CPTString NODE_TAG_NAME_TUNE_INFO             = L"TuneInfo";
const CPTString CHILD_TAG_NAME_TXTUNEPOS            = L"txtunepos";
const CPTString CHILD_TAG_NAME_RFLPOS               = L"rflpos";

// Tag Names for Shim State
const CPTString NODE_TAG_NAME_SHIM                  = L"Shim";
const CPTString CHILD_TAG_NAME_SHIM_STATE           = L"shimstate";


//Patni-RSG/2009Nov11/Added/IR-116
// Tag Names for Shim State
const CPTString LOCAL_SHIM                          = L"Local_Shim";
const CPTString CHILD_TAG_NAME_lOCAL_SHIM_LIMIT     = L"localShimLimit";
//Patni-RSG/2009Nov11/Added/IR-116


// Tag Names for Color Info
const CPTString NODE_TAG_NAME_STATUS_CONFIG         = L"StatusConfig";
const CPTString CHILD_TAG_NAME_NAME                 = L"Name";
const CPTString CHILD_TAG_NAME_FORMAT               = L"Format";
const CPTString NODE_TAG_NAME_COLOR_INFO            = L"ColorInfo";
const CPTString CHILD_TAG_NAME_BACKGROUND           = L"Background";
const CPTString CHILD_TAG_NAME_FOREGROUND           = L"Foreground";

// Attrib Name
const CPTString ATTRIBUTE_NAME_1                    = L"value";
const CPTString ATTRIBUTE_NAME_2                    = L"value_type";

// Status
const CString STATUS_WAIT                         = L"WAIT";
const CString STATUS_CURRENT                      = L"CURRENT";
const CString STATUS_FAILED                       = L"FAILED";
const CString STATUS_DONE                         = L"DONE";

//+Patni-ARD/2009Oct28/Added/IR 85 Auto Transfer
const CPTString NODE_TAG_NAME_AUTO_JOB_SETTING    = L"AutoJobSetting";
const CPTString CHILD_TAG_NAME_AUTOJOB            = L"AutoJob";
//-Patni-ARD/2009Oct28/Added/IR 85 Auto Transfer
const CPTString PQM_DEFAULT_PGUID				  = L"PQM_Default_PGUID.xml";


// For Event Log

const _TCHAR USER_NAME[]                    =   _T("TMSC");

const _TCHAR APP_NAME[]                     =   _T("PQM");

// For Operation Log

const BYTE SUBSYSTEM_ID                     =    0x41;
const BYTE COMPONENT_ID                     =    0x02;
const WORD OP_ID_APP_START = 0x0000;
#define OP_ID_SCAN_STG                      0x00AA

const WORD PQMSTG_ACQINFO                   =    OP_ID_SCAN_STG + 0x0001;
const WORD PQMSTG_INTERLOCK                 =    OP_ID_SCAN_STG + 0x0002;
const WORD PQMSTG_SHIMCTRL                  =    OP_ID_SCAN_STG + 0x0003;
const WORD PQMSTG_PRESCAN                   =    OP_ID_SCAN_STG + 0x0004;
const WORD PQMSTG_ACQUISITION               =    OP_ID_SCAN_STG + 0x0005;
const WORD PQMSTG_PAUSE                     =    OP_ID_SCAN_STG + 0x0006;
const WORD PQMSTG_RESUME                    =    OP_ID_SCAN_STG + 0x0007;
const WORD PQMSTG_ABORT                     =    OP_ID_SCAN_STG + 0x0008;

//const WORD PQMSTG_STATUS                  =    OP_ID_SCAN_STG + 0x0009;
#define OP_ID_SCAN_STATUS                   0x00C8

const WORD STATUS_AMCMS_WAIT_FOR_START      =    OP_ID_SCAN_STATUS + 0x0001;
const WORD STATUS_AMCMS_SCAN_BEGIN          =    OP_ID_SCAN_STATUS + 0x0002;
const WORD STATUS_AMCMS_SCAN_PAUSE          =    OP_ID_SCAN_STATUS + 0x0003;
const WORD STATUS_AMCMS_SCAN_RESUME         =    OP_ID_SCAN_STATUS + 0x0004;
const WORD STATUS_AMCMS_SCAN_END            =    OP_ID_SCAN_STATUS + 0x0005;
const WORD STATUS_AMCMS_RMSCAN_END          =    OP_ID_SCAN_STATUS + 0x0006;
const WORD STATUS_AMCMS_PRESCAN_CFA_END     =    OP_ID_SCAN_STATUS + 0x0007;
const WORD STATUS_AMCMS_PRESCAN_RFL_END     =    OP_ID_SCAN_STATUS + 0x0008;
const WORD STATUS_AMCMS_PRESCAN_RXGAIN_END  =    OP_ID_SCAN_STATUS + 0x0009;
const WORD STATUS_AMCMS_DYNAMIC_SCAN_BEGIN  =    OP_ID_SCAN_STATUS + 0x000A;
const WORD STATUS_AMCMS_VISUALPREP_END      =    OP_ID_SCAN_STATUS + 0x000B;
const WORD STATUS_AMCMS_AMB_PROTOCOL_END    =    OP_ID_SCAN_STATUS + 0x000C;

const int INVALIDVAL = 4294967295;
///////////////// DBSA Lib Constants
// + LOKESH 14 May 2007
#define PQM_SVN_PLN_SELECTED_SLICE_THK      0x001910BA
#define PQM_SVN_PLN_SELECTED_SLICE_GAP      0x001910BB
#define PQM_SVN_PLN_FOV                     0x001910BC
#define PQM_SVN_PLN_TRANSMIT_COIL           0x001910BD
#define PQM_SVN_PLN_TOTAL_TIME              0x001910BE
#define PQM_SVN_PLN_RECVR_COIL              0x001910C1
#define PQM_SVN_PLN_SELECTED_CONTRAST_TE    0x001910C6
#define PQM_SVN_PLN_TEMPLATE_SEQ            0x001910CA
#define PQM_SVN_PLN_STATUS                  0x001910CC
#define PQM_SVN_PLN_HOLD                    0x001910CD
#define PQM_SVN_EXE_PROTOCOL_ACQ_STATUS     0x001910CF
#define PQM_SVN_PLN_DYNAMIC_SCAN            0x001910DA
#define PQM_SVN_PLN_APP_CODE                0x001910E9
#define PQM_SVN_EXE_COUCH                   0x001910FD
#define PQM_SVN_SEQ_DBDT_VALUE              0x0019112F
#define PQM_SVN_PLN_SCAN_ANATOMY            0x00191132
#define PQM_SVN_SEQ_SAR_TOTAL_TIME          0x0019113A
#define PQM_SVN_PLN_VIEWING_DIR             0x0019115D
#define PQM_SVN_PLN_SELECTED_TAG_THK        0x0019116F
#define PQM_SVN_PLN_MOTION_SUPPRESSION      0x0019119B
#define PQM_SVN_PLN_APP_OPTION              0x001911C0
#define PQM_SVN_PLN_SLICE_ORIENTATION       0x00211002
#define PQM_SVN_PLN_PHASE_DIRECTION         0x00211003
#define PQM_SVN_PLN_WRAPAROUND_REDUCT       0x00211004
#define PQM_SVN_PLN_GRID_TAG                0x001911A5
#define PQM_SVN_PRC_REFINE_FILTER           0x00191050
#define PQM_SVN_PRC_K_SPACE_FILTER          0x00191051
#define PQM_SVN_PLN_COVERAGES               0x001910B6
#define PQM_SVN_STD_PROTOCOL_NAME           0x00181030
//Patni-RSG/Added/2009Nov25/MSA0248-00033/MVC005786
#define PQM_SVN_STD_SHORT_SERIES_INFO       0xE3011000
//+ Study Manager
#define PQM_DCM_MR_STUDY_HOME_POSITION      0x001910A1
#define PQM_DCM_MR_STUDY_ACQ_CONDITION      0x001910FB

//SM4 Himanshu 5 Dec 2008 TEMP Remove Later
//#define PQM_SVN_EXE_SERIES_HOME_POSITION 0x001911E4
#define PQM_DCM_MR_SERIES_HOME_POSITION     0x001911E4
#define PQM_DCM_MR_SERIES_ACQ_CONDITION     0x0019108C

//+ Study Manager 2
//Patni - HAR / 2009Mar18 / Added / PSP Defect Sheet : Defect#212
#define PQM_DCM_MR_STUDY_NODE_PATH          0x00291050 // 0x00200010 
#define PQM_DCM_MR_SERIES_NODE_PATH         0x00291051
#define PQM_DCM_MR_PROTOCOL_NODE_PATH       0x00291054
#define PQM_DCM_MR_SCH_NUM_PROTOCOLS        0x00111003
#define PQM_DCM_ST_SERIES_NUMBER            0x00200011
#define PQM_DCM_MR_SCH_PROTOCOL_GROUP       0x00111004
#define PQM_DCM_MR_SCH_NUM_SERIES_IN_PROTOCOL 0x00111005
#define PQM_DCM_MR_SERIES_NUMBER_BIAS       0x00111006
//- Study Manager 2

#define DCM_MR_WFDA_CAPP_INITIAL_PARAMETER			0x0019128F
#define DCM_MR_WFDA_CAPP_ANALYSIS_PARAMETERS		0x00191276
#define DCM_MR_WFDA_CAPP_PREFERENCE_GUID			0x00191286
#define DCM_MR_WFDA_CAPP_INSCAN_FLAG				0x00191285
#define DCM_MR_WFDA_CAPP_INSCAN_CALC_MODE			0x001912B4
//+Patni-Sribanta/2010July21/Added/TMSC-REDMINE-452
#define PQM_DCM_MR_STUDY_LEAF_5          0x00200010
//-Patni-Sribanta/2010July21/Added/TMSC-REDMINE-452

//+Patni-Sribanta/2010July13/Added/IR-149
#define PQM_DCM_EXE_RTSAR_MAX_STANDARD_IN_SUBPROTOCOL 0x001912AA
#define PQM_DCM_EXE_RTSAR_MAX_SHORT_IN_SUBPROTOCOL 0x001912AB
#define PQM_DCM_EXE_RTSAR_PARAMS_WB 0x001912AC
#define PQM_DCM_EXE_RTSAR_PARAMS_HEAD 0x001912AD
#define PQM_DCM_EXE_RTSAR_PARAMS_PARTIAL 0x001912AE
#define PQM_DCM_EXE_RTSAR_PEAK_POWER_RFL 0x001912AF
//-Patni-Sribanta/2010July13/Added/IR-149

#define DCM_MR_PLN_SAE_TOTAL				0x001912D4
#define DCM_MR_SEQ_ACQ_COVERAGE				0x001911E6
#define DCM_MR_EXE_RTSAR_AVERAGE_STANDARD_IN_SUBPROTOCOL	0x001912D6

//-Patni-HAR/2010Mar26/Added/MVC007422
#define PQM_DCM_MR_SEQ_IEC_INFO             0x00111007
//Patni-MP/2009Jul25/Added/ACE-2 IR LOCATOR_IMAGE_ID to LOCATOR_IMAGES
#define PQM_DCM_MR_SERIES_IMAGE_LOID        0x00080018

//+Patni-PJS/2010May10/Added/JaFT# IR-141
#define PQM_DCM_MR_PATIENT_ID               0x00100020
#define PQM_DCM_MR_STUDY_UID                0x00100020
//-Patni-PJS/2010May10/Added/JaFT# IR-141

#define PQM_DCM_MR_STUDY_CHARSET			0x00080005

#define DCM_MR_PLN_RTSAR_MAX_STANDARD_IN_SUBPROTOCOL	0x001912E1
#define DCM_MR_PLN_RTSAR_MAX_SHORT_IN_SUBPROTOCOL		0x001912E2
#define DCM_MR_EXE_N_RTSAR_END_POINT_TIME				0x001912E3

const int MAX_NODES     = 256;
const int MAX_BUFFER    = 256;
const int BUFFER_ERR_PROCESS = 500;
const int TAG_STRING    = 20;
const int VALUE_BW      = 244;
const int VALUE_FLIP    = 40;
const int VALUE_FLOP    = 160;
const int VALUE_OBDATA  = 3;
const int VALUE_MULTIPLIER = 2;
const int IRS_MAX_BUFFER = 256;
const int MAX_SEQUENCE  = 10;

const int PROMSG_SHIMM_BEGIN = 5100;    /* PQM->shimming with path */
const int PROMSG_SHIMM_ADJUST = 5101;   /* shimming->PQM with path */
const int PROMSG_SHIMM_END = 5101;      /* for old version */
const int PROMSG_SHIMM_CANCEL = 5102;   /* shimming->PQM */
const int PROMSG_SHIMM_DONE = 5103;     /* shimming->PQM */
const int PROMSG_SHIMM_ADJUST_DONE = 5104;  /* PQM->shimming */
const int SHIMMING_TIMEOUT_DEFAULT = 60;     /* default timeout 60sec */
const int DEFAULT_DATA_VALUE = 0x7FFFFFFF;

const int STEP_LEVEL_1 = 1;
const int STEP_LEVEL_2 = 2;
const int STEP_LEVEL_3 = 3;
const int STEP_LEVEL_4 = 4;
const int STEP_LEVEL_5 = 5;
const int ACQ_TIME = 1000;
const float INTERLOCK_WATT = 9000.00f;
const int IRS_SLEEP_TIME = 2000;
const int VOICEMANAGER_BUFFER = 256;
const int RGB_255 = 255;
const int BASE_10 = 10;
const int BASE_11 = 11;
const int BASE_12 = 12;
const int BASE_13 = 13;
const int BASE_14 = 14;
const int BASE_15 = 15;



const int PQM_SHIM_RESET = 0;
const int PQM_SHIM_USE_DEFAULT = 1;
const int PQM_SHIM_USE_VARFIELD = 2;

/* preScanStatus */
const int STATUS_IEC_CHECK = 0x0001;
const int STATUS_PRESCANHOOK = 0x0002;

const int PQM_ACQ_ERROR_PVCOIL_FILE = 120;


#define LOCK_FILE                       "scan_lock_file"

#define PQM_SVN_PLN_AUTO_POSTPROC_FLG 0x001911B7
#define PQM_SVN_PLN_AUTO_VOICE  0x001910CE
#define PQM_SVN_EXE_AUTO_VOICE 0x001910C3
#define PQM_SVN_PLN_INTERMIT_FLAG 0x001910E4

const int COILKEY = 1094909952;//1178730659;//
#define ANATOMY_GUI VFC_ANATOMY_HEAD

//+Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
enum {
    SCAN_ERR_NOTIFY = 3,    //Blue
    SCAN_ERR_WARNING = 4,   //Yellow 4 = 5
    SCAN_ERR_CRUCIAL = 6    //Red
};
//-Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView

enum {
    SCAN_WARN_INFORMATION = 3,
    SCAN_WARN_WARNING = 4,
    SCAN_WARN_CRUCIAL = 5

};

enum {
    SCAN_WARN_SYSMODAL = 1,
    SCAN_WARN_APPMODAL = 2,
    SCAN_WARN_MODELESS = 3,
};

enum {
    SCAN_WARN_OKBUTTON = 1,
    SCAN_WARN_CANCELBUTTON = 2,
    SCAN_WARN_RETRYBUTTON = 4,
    SCAN_WARN_YESBUTTON = 8,
    SCAN_WARN_NOBUTTON = 10,
    SCAN_WARN_OKONLYBUTTON = SCAN_WARN_OKBUTTON,
    SCAN_WARN_OK_CANCEL_BUTTONS = SCAN_WARN_OKBUTTON | SCAN_WARN_CANCELBUTTON,
    SCAN_WARN_OK_CANCEL_RETRY_BUTTONS = SCAN_WARN_OKBUTTON | SCAN_WARN_CANCELBUTTON | SCAN_WARN_RETRYBUTTON,
    SCAN_WARN_YES_NO_BUTTONS = SCAN_WARN_YESBUTTON | SCAN_WARN_NOBUTTON
};

const int COIL_PORT_MASK = 0xFF000000;
const int BREAST_SPDR_COIL = 0x4600003b;
const int COIL_PHYSICALID_MASK = 0x0000FFFF;
const int WBCOIL_PHYSICAL_ID = 0x0000;
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
//const int MAXIMUM_COIL_IN_PORT = 16;

const int P	= -1;
const int V	= 1;

const int OS_ERROR = -1;

const int PQM_SHM_KEY	= 0x10010001;
const int PQM_SEM_KEY	= 0x10010001;
const int PQM_SEM_NUM	= 1;
const int PQM_SEM_NO	= 0;
const int PQM_SEM_DEF	= 1;

const int CNTLUNIT = 1;
const int E_BUSY = E_ERROR + 1;
const int FIELDPROFILEIMAGES = 15;
const int PQM_K_PR_PRESCAN_RFL = 1;
const int PQM_VFC_PRESCAN_CLINICAL = 0;

// + V9.25 coil support@AMT
//********************************************
//	  Return Code
//********************************************
// Pls use E_NO_ERROR instead of NO_ERROR as NO_ERROR is defined in some
// windows header file. @AMT
//#define NO_ERROR				    E_NO_ERROR
#define ERROR_DISCONNECT		    0x01    // Not used...
#define ERROR_RESELECT			    0x02
#define	ERR_OFF_GROUP		        0x03
#define	ERR_COILDATA		        0x04
#define	ERR_NO_COILDATA		        0x05

//Patni-ARD/20090226/Added/ACE-1# Enhance the withstand load of couch Start
#define	PQM_OVER_LOG		"pqm/weight_over.log"
#define PQM_MAX_COIL_WEIGHT		300
//Patni-ARD/20090226/Added/ACE-1# Enhance the withstand load of couch End

//********************************************
//  defines for Combine
//********************************************
//+Patni-MRP/2010APR26/Modified/COMMON_CDR_06
/*#define COMBINE_NOT_TOGETHER		0x00
#define COMBINE_PERMISSION_CONNECT	0x01
#define COMBINE_SWITCH_SELECT		0x05
#define COMBINE_PERMISSION_ALL		0x07
#define COMBINE_WHOLE_BODY			0xF5
// + V9.25 coil support@AMT
*/
//-Patni-MRP/2010APR26/Modified/COMMON_CDR_06
typedef enum dbdt_status {
    PQM_CALC_ERROR = 0,
    PQM_1ST_CONTROLLED,
    PQM_2ND_CONTROLLED,
    PQM_CONTROLLED1,
    PQM_CONTROLLED1_PLAN_ERR,
    PQM_CONTROLLED1_FOV_ERR,
    PQM_CONTROLLED2_RO_ERR,
    PQM_LICENSE_ERR,
    //	PQM_2ND_CONTROLLED_ERR,
    //	PQM_OVER_ERR,
    PQM_SAR_1ST_CONTROLLED,
    PQM_SAR_2ND_CONTROLLED,
    PQM_SAR_LICENSE_ERR,
} dbdt_status_e;

/* Force dBdt message. */
#define	PQM_DBDT_NORMAL				        0x00000000
#define	PQM_DBDT_1ST_CONTROLLED			    0x00000001
#define	PQM_DBDT_2ND_CONTROLLED			    0x00000002
#define	PQM_DBDT_CONTROLLED1			    0x00000003
#define	PQM_DBDT_CALC_ERROR			        (0x00000004 | PQM_DBDT_ERR)
#define PQM_DBDT_CONTROLLED1_PLAN_ERR		(0x00000005 | PQM_DBDT_ERR)
#define PQM_DBDT_CONTROLLED1_FOV_ERR		(0x00000006 | PQM_DBDT_ERR)
#define PQM_DBDT_CONTROLLED2_RO_ERR		    (0x00000007 | PQM_DBDT_ERR)
#define PQM_DBDT_LICENSE_ERR			    (0x00000008 | PQM_DBDT_ERR)
#define PQM_DBDT_2ND_CONTROLLED_ERR		    (0x00000009 | PQM_DBDT_ERR)
#define PQM_DBDT_OVER_ERR			        (0x0000000a | PQM_DBDT_ERR)
#define	PQM_DBDT_ERR				        0x00008000
#define	PQM_DBDT_MASK				        0x0000ffff

/* Force SAR message. */
#define	PQM_IEC_SAR_NORMAL			        0x00000000
#define	PQM_IEC_SAR_1ST_CONTROLLED		    0x00010000
#define	PQM_IEC_SAR_2ND_CONTROLLED		    0x00020000
#define PQM_IEC_SAR_LICENSE_ERR			    (0x00030000 | PQM_IEC_SAR_ERR)
#define	PQM_IEC_SAR_ERR				        0x80000000
#define	PQM_IEC_SAR_MASK			        0xffff0000

/* dBdt LEVEL value */
#define	CONTROLLED_1ST_VAL	20
#define	CONTROLLED_2ND_VAL	50

//SM3_Mangesh_Start
//+Patni-Hemant/2010Dec06/Commented/REDMINE-774
//typedef struct SContrastAgent {
//    CString  m_strname;
//    int  m_index;
//
//    //+Patni-RUP/2009Aug21/Modified/cpp test corrections
//    SContrastAgent() {
//        m_strname	= _T("");
//        m_index = 0 ;
//    }
//    //-Patni-RUP/2009Aug21/Modified/cpp test corrections
//
//} ContrastAgent;
//-Patni-Hemant/2010Dec06/Commented/REDMINE-774


//+Patni-Hemant/2010Apr27/Modified/JaFT/IR-137
#include "PQM\PQMCommonStructures.h"

/*
struct SScanAantomy {

    int m_scan_anatomy_vf;
    CString m_scan_anatomy_name;

    SScanAantomy() {

        m_scan_anatomy_vf = -1 ;
        m_scan_anatomy_name	= _T("");
    }

    SScanAantomy(int scan_anatomy_vf, CString scan_anatomy_name) {

        m_scan_anatomy_vf = scan_anatomy_vf ;
        m_scan_anatomy_name	= scan_anatomy_name ;
    }

    bool operator == (const SScanAantomy& scanaantomy_data) {

        return ((m_scan_anatomy_name.CompareNoCase(scanaantomy_data.m_scan_anatomy_name) == 0)
                &&
                (m_scan_anatomy_vf == scanaantomy_data.m_scan_anatomy_vf)
               )  ;
    }
};
*/
//-Patni-Hemant/2010Apr27/Modified/JaFT/IR-137

//Patni-Hemant/2010Dec06/Commented/REDMINE-774
//typedef CList<ContrastAgent, ContrastAgent> CContrastAgentData;

typedef CList<SScanAantomy, SScanAantomy> CScanAnatomyData;
//SM3_Mangesh_End

// + SM4 LOCATOR_IMAGE Himanshu 3 Dec 2008
const int PROLIM_REF_LAST_IMAGE			= 0;
const int PROLIM_REF_LAST_PLANED_IMAGE	= 1;
const int PROLIM_REF_NO_IMAGE			= 2;
// - SM4 LOCATOR_IMAGE Himanshu 3 Dec 2008

//+Patni-DKH/2010Mar03/Added/Phase3#MCM0229-00187
const int GLOBAL_RFL_LIMIT = 50;
const int GLOBAL_TUNE_LIMIT = 500;
//-Patni-DKH/2010Mar03/Added/Phase3#MCM0229-00187

const int LOCATOR_INVOCATION_DELAY = 300;   //Patni-RUP/2010Jul19/Added/TMSC-REDMINE-215
//PATNI-Sribanta/2010Nov09/Added/IR-170
#define GP_PAS_PATH  "\\gp\\PAS\\";		//Patni-Sribanta/2011Mar28/Modified/IR-170_Part3

const TCHAR THRESHOLD_CFA[]		= _T("Threshold_CFA:");
const TCHAR THRESHOLD[]			= _T("Threshold:");
const TCHAR TEMP_COEFF[]		= _T("TempCoeff:");
const TCHAR GIF[]               = _T("GIF:");
const TCHAR EOL[]				= _T("\n"); // End of line
const TCHAR TRIM_SPACE[]		= _T(" ");
const TCHAR FIND_COMMA[]		= _T(",");
const int MAX_TEMP				= 16;
const float OLPTEMP_NOTUSE 		= -100.0;
const int CFA_REQUIRED			= 1;
const int CFA_NOT_REQUIRED		= 0;
const int CFA_SHIFT_ERROR		= 2;

#endif // #ifndef __SM_IPCLIB_PQMCONSTANTS_H__ 

typedef enum {
    CHANNEL_MODE_NOT_REQUIRED = -1,
    CHANNEL_MODE_NORMAL = 0,
    CHANNEL_MODE_MAX = 1
} CHANNEL_MODE;

typedef enum {
    PQM_PROTOCOL_SCAN = 0,
    PQM_APC_SCAN,
    PQM_CFA_SCAN,
    PQM_RMC_SCAN,
    PQM_GATE_SCAN
} PQM_SCAN_MODE;
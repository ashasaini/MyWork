// pqmAMBStructs.h: interface for the CpqmAMBStructs class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_PQMAMBSTRUCTS_H__
#define __SM_IPCLIB_PQMAMBSTRUCTS_H__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tami/pqm/pqmAmbMsg.h>
#include "VarFieldHandle.h"

typedef	struct	Entrytbl_s {
    int		num;				/* Number of Entry */
    int		prot[AMB_STATION_MAX];	/*  */
} Entrytbl_t;

typedef	struct	Entrytbl_s_CB {
    Entrytbl_t& entry;
    CPQMStudy* l_study;
} Entrytbl_t_CB;

class CPQMAMBcallback : private MR::ACQ::SM::COMMON::NonCopyable<CPQMAMBcallback>
{
public:
    explicit CPQMAMBcallback(CPQMStudy* f_study) {
        m_study = f_study;
    }
    CPQMStudy* m_study;
    Entrytbl_t* m_entry;
};

typedef	struct	PqmAmbScanTbl_s {
    int		prot ;
    int32_t	type ;
} PqmAmbScanTbl_t, *PqmAmbScanTbl_p;

typedef	struct	PqmAmbScanProt_s {
    int					num;
    long				RemainScanTime;
    PqmAmbScanTbl_t*	PqmScanTbl;
} PqmAmbScanProt_t, *PqmAmbScanProt_p;

typedef struct PqmPmAmbS {

    bool_t	debug;
    int		retry;
    int		Amb_status;				/* AMB Propcess Status */
    bool_t	SetAllCoilSAR_backup;	/* SetAllCoilSAR backup*/

    int		numStation;
    float	offset[AMB_STATION_MAX];		/* Zoffset value */
    float	NewPosi[AMB_STATION_MAX];		/* 移動を想定した場合の寝台位置 */
    float	MoveCouchVal[AMB_STATION_MAX];/* 移動を想定した場合に必要な移動距離 */
    int		ParentOff[AMB_STATION_MAX];	/*　位置決め親子関係を切る必要有無 */
    Entrytbl_t	Master;						/* Master protocol */
    Entrytbl_t	Master_org;					/* Master protocol */
    Entrytbl_t	shimming;					/* Shimming protocol */
    Entrytbl_t	map;						/* Speeder MAP protocol */
    int		numSlave ;						/* Number of Slave protocol without Shimming/MAP */
    int*		SlaveProt;						/* Slave protocol */
    int		PrepFlag;		/* Visual Prep ON/OFF */
    int		Speeder;		/* SPEEDER type  */
    int		Intensity;		/* 輝度補正 ON/OFF */
    int		stationOrder;	/* デフォルト寝台移動　0: outside  */

    int		AmbEditStatus ;	/* AMBEditとの通信status */
    void*	userData;		/* AMB editメッセージ通知したコールバックで使うData */

    amb_t	AmbPlan;		/*  */

} PqmPmAmbT ;

/** For scan mode. **/
typedef struct  _PqmPrMode_s {
    int         rcvCoilId;
    float       couchPos;
    float	    homePos;
    float	    couchOffset;
    int         anatomy;
    //char        bodypart[PQM_BODYPART_LEN];
    int         contrast;
    int		    scanNumber;
    int		    view_dir;
    VftPatori_t	patori;		/* Patient orientation. */
} PqmPrMode_t, * PqmPrMode_p;


typedef struct PqmAmbProtCheckDataS {
    flt64_t	fsy_value;
    flt32_t	rfl_gain_value;
    flt32_t	rfl_scale_value;
    flt32_t	rgn_value;
    flt32_t	TGCRFoutRatio;
    flt32_t	inter_lock;


    int32_t	shim_flag ;
    VfPathElem_t	activeShimPath[SVD_ST_PROCESSED];
    int32_t	shimFsyFlag ;

    VfPathElem_t        mapPath[4];
    int32_t	sar ;
    int32_t	rcvCoil ;
    int32_t	trnCoil ;
    flt32_t	couchPosi ;
    f32vec3_t	offVecVG ;
    f32vec3_t	offVec ;
} PqmAmbProtCheckDataT, PqmAmbProtCheckDataP ;

typedef enum {
    A__PRESCAN_FSY_VALUE = 0,
    A_PRESCAN_RFL_GAIN_VALUE,
    A_PRESCAN_RFL_SCALE_VALUE,
    A_PRESCAN_RGN_VALUE,
    A_PRESCAN_TGC_RFOUT_RATIO,
    A_RF_INTERLOCK_VALUE,
    A_SHIM_ENABLE_FLAG,
    A_SHIM_PATH ,
    A_SHIM_FSY_FLAG ,
    A_MAP_PATH ,
    A_SAR_ANATOMY ,
    A_RECVR_COIL ,
    A_TRANSMIT_COIL ,
    A_EXE_COUCH ,
    A_COUCH_OFFSET,
    A_VG_OFFSET_VECTOR,
    A_OFFSET_VECTOR ,
    A_ALL_CHECK
} PqmAMBAcqCheckKinds;

#endif // #ifndef __SM_IPCLIB_PQMAMBSTRUCTS_H__

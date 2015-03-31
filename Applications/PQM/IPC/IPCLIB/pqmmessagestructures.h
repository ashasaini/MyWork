/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: pqmmessagestructures.h
 *  Overview: Structure of Cpqmmessagestructures class.
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
#ifndef __SM_IPCLIB_PQMMESSAGE_STRUCTURE_H__
#define __SM_IPCLIB_PQMMESSAGE_STRUCTURE_H__
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tami/acqman/AM_ClientMessages.h>
#include <tami/acqman/AM_ClientMessages_unicode.h>

#define PQM_SHIM_LEN            8
#define PQM_GRAD_LEN            3



typedef struct {
    //flt64_t   fsyValue;
    double  fsyValue;
    //int32_t   gradOffset[PQM_GRAD_LEN];
    int gradOffset[PQM_GRAD_LEN];
    //int32_t   shimOffset[PQM_SHIM_LEN];
    int shimOffset[PQM_SHIM_LEN];
    //int32_t   fsyFlag;
    int fsyFlag;
} PqmShim_t;

//Patni-DKH/2010Jan29/Added/PH#3#/FBI
typedef struct	delayInfo_s {
    study_id_t	study_id;
    int32_t     delay1;
    int32_t     delay2;

    delayInfo_s() :
        delay1(0),
        delay2(0) {
        memset(&study_id, 0, sizeof(study_id));
    }

} delayInfo_t;
//Patni-DKH/2010Jan29/Added/PH#3#/FBI

typedef struct PqmPmProlimS {

    int		gate_req;
    bool_t	debug;
    int		retry;

    char*	scheduleTmp;
    char*	imagingTmp;
    char*	fluoroTmp;

    int		planWidth;	/* Plan Width */
    int		planComRightPos;/* Plan Comment Right Position */
    int		planStsRightPos;/* Plan Status Right Position */
    int		referenceImage;	/* Reference Image */
    int		treeId;		/* TREE ID for prolim */
    bool_t	multiSelect;	/* Multi Select */

    int		selectCount;	/* V7.00 */
    int*		selectList;		/* V7.00 */

    //Patni-DKH/2010Jan29/Added/PH#3#/FBI
    delayInfo_t delayInfo;

    PqmPmProlimS(): gate_req(0),
        debug(false),
        retry(0),
        scheduleTmp(NULL),
        imagingTmp(NULL),
        fluoroTmp(NULL),
        planWidth(0),
        planComRightPos(0),
        planStsRightPos(0),
        referenceImage(0),
        treeId(0),
        multiSelect(false),
        selectCount(0),
        selectList(NULL) {
        memset(&delayInfo, 0, sizeof(delayInfo));
    }

} PqmPmProlimT, *PqmPmProlimP;

typedef struct	ProtocolScanTimeData_s {
    int32_t scan_total_time;
    int32_t num_temp_max_scan_time;
    flt32_t temp_max_scan_time_ratio;

    ProtocolScanTimeData_s():
        scan_total_time(0),
        num_temp_max_scan_time(0),
        temp_max_scan_time_ratio(1.0) {}
} ProtocolScanTimeData_t;
#endif // #ifndef __SM_IPCLIB_PQMMESSAGE_STRUCTURE_H__ 

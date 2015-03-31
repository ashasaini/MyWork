// PosSlabInfo.h: interface for the PosSlabInfo class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_POSSLABINFO_H__
#define __SM_IPCLIB_POSSLABINFO_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "Vector2D.h"
#include "Vector3D.h"


typedef struct msPosSlabInfo_s : private MR::ACQ::SM::COMMON::NonCopyable<msPosSlabInfo_s> {
    CVector2D    plnFov;
    flt32_t      sliceThickness;
    flt32_t      sliceGap;
    int32_t      sliceNum;
    CVector3D    offsetVec, sliceVec, peVec, roVec;

    /* check info */
    int32_t		 orientationId;
    CVector3D    eachPos[8];/* all 8 points */
    CVector3D    slabCenter;/* slab center points */
    CVector3D    sliceCenter[2];/* slice center points(max,min) 2 points */

    msPosSlabInfo_s();

    static void makeCheckPointsInfo(msPosSlabInfo_s* pos);
    const CVector3D ProductOrientationLength(const CVector3D& f_length);
} msPosSlabInfo_t;


#endif // #ifndef __SM_IPCLIB_POSSLABINFO_H__

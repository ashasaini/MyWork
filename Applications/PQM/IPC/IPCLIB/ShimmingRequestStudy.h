/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: ShimmingRequestStudy.h
 *  Overview: Structure of CShimmingRequestStudy class.
 *  Last Modified: 2007/8/10 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2008/3/15 12:00:00     Study Manager Phase 1
 *
 *****************************************************************************/

#ifndef __SM_IPCLIB_SHIMMINGREQUEST_STUDY_H__
#define __SM_IPCLIB_SHIMMINGREQUEST_STUDY_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>

class CShimmingRequestStudy : private MR::ACQ::SM::COMMON::NonCopyable<CShimmingRequestStudy>
{
public:
    CShimmingRequestStudy();
    virtual ~CShimmingRequestStudy();

    BOOL ShimPutOffset(const int f_typeformat_depth);
    BOOL ShimDataKindCheck();
    BOOL ShimFlagRead();
    BOOL ShimStudyOpen();
    BOOL ShimGetZ2(int* f_z2);
    BOOL ShimPutFsyVal(const int f_typeformatdepth);
    BOOL ShimInfoRead();
    BOOL ShimPositionRead();
    int Direction(const float x, const float y, const float z);

    void ShimStudyClose();
protected:

private:
    void ShimReadTotalSlices();
};
#endif // #ifndef __SM_IPCLIB_SHIMMINGREQUEST_STUDY_H__

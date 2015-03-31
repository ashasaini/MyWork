//Redmine-783
//Redmine-780
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: pqmshim.h
 *  Overview: Structure of CPqmShim class.
 *  Last Modified:
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2008/3/15 12:00:00     Study Manager Phase 1
 *
 *****************************************************************************/

#ifndef __SM_IPCLIB_PQMSHIM_SHARE_DATA_H__
#define __SM_IPCLIB_PQMSHIM_SHARE_DATA_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tami/vfStudy/vfs_structure.h>
#include <tami/libvf/varfields.h>
#include <PASCOMLIB/SequenceParameter.h>
#include <libStudyManager/NonCopyable.h>

// This class is mainly responsible for the shimming values in shared memory
class CPqmShimSharedData : private MR::ACQ::SM::COMMON::NonCopyable<CPqmShimSharedData>
{
public:
    static CPqmShimSharedData* GetInstance();
    //Destructor
    virtual ~CPqmShimSharedData();

    BOOL InitShim();
    BOOL CopyCouchInfo(int* f_scannumber, float*	f_couchpos, float* f_homepos);
    BOOL CopyBackCouchInfo(const int f_scannumber, const float f_couchpos, const float	f_homepos);
    BOOL CopyTuneInfo(float*	f_txtunepos, float*	f_rflpos, float*	f_cfapos);
    BOOL ShimRegistCouchInfo(int* f_scannumber, float* f_couchpos, float* f_homepos);
    BOOL ShimRegistTuneInfo(float*	f_txtunepos, float*	f_rflpos , float*   f_cfapos);
    BOOL ShimRegistStudy(const BOOL f_isimaging, study_id_t* f_study_id, study_id_t* f_another_study, bool* f_conflict, bool* f_samestudy);
    BOOL ShimRegistProtocol(const int	f_protocol);
    BOOL CopyShimPos(float*	f_shimpos);

    study_id_t  GetScheduleActive() const;
    study_id_t  GetImagingActive() const;

    int   GetCurrentProtocolNumber() const;    // Patni - HAR / 2009Mar18 / Added / PSP Defect Sheet : Defect#212
    int   GetScanNumber() const;
    float GetCouchPos() const;
    float GetHomePos() const;
    float Gettxtunepos() const;
    float Getrflpos() const;

    BOOL GetShimState(int* f_shimstate);
    BOOL SetShimState(const int f_shimstate);

    BOOL GetShimFsyFlag(int*	f_shimfsyflag);
    BOOL SetShimFsyFlag(const int	f_shimfsyflag);

    BOOL GetShimPath(VfPathElem_t* const f_shimpath);
    BOOL SetShimPath(VfPathElem_t*	const f_shimpath);

    BOOL GetHRShimPath(VfPathElem_t* const	f_shimpath);
    BOOL SetHRShimPath(VfPathElem_t* const	f_shimpath);

    BOOL GetCfaPos(float* f_cfapos);
    BOOL SetCfaPos(float f_cfapos);

    BOOL GetShimPos(float* f_shimpos);
    BOOL SetShimPos(const float f_shimpos);

private:

    study_id_t                      m_schedule_study;
    study_id_t                      m_imaging_study;
    bool                            m_scheduleactive;
    bool                            m_imagingactive;
    int                             m_currentprotocol;// current scanning protocol
    int                             m_scannumber;
    float                           m_couchpos;
    float                           m_homepos;
    float                           m_txtunepos;
    float                           m_rflpos;
    VfPathElem_t                    m_activeshimpath[SVD_ST_PROCESSED];
    VfPathElem_t                    m_activehrshimpath[SVD_ST_PROCESSED];
    int                             m_shimstate;
    int                             m_shimfsyflag;
    static CPqmShimSharedData*      m_pqmshimshareddata;
    //Patni-DKH/2010Feb18/Added/Phase3#MCM0229-00187
    float							m_cfaPos;
    float							m_shimPos;

    //Constructor
    CPqmShimSharedData();

    BOOL ShimClear(const bool f_isimaging);
};

#endif // #ifndef __SM_IPCLIB_PQMSHIM_SHARE_DATA_H__

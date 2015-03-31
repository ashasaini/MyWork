//Redmine-783
//Redmine-780
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PqmPrMode.h
 *  Overview: Structure of CPqmPrMode class.
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
#ifndef __SM_IPCLIB_PQMPRMODE_H__
#define __SM_IPCLIB_PQMPRMODE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tami/common/machdefs.h>
#include <tami/common/basicdefs.h>
#include <tami/vfStudy/vf_apptypes.h>
#include <libStudyManager/NonCopyable.h>

//+Patni-Hemant/2010Apr27/Added/JaFT/IR-137
#include <PQM/PQMCommonStructures.h>
//-Patni-Hemant/2010Apr27/Added/JaFT/IR-137

class CPqmPrMode : private MR::ACQ::SM::COMMON::NonCopyable<CPqmPrMode>
{
public:
    //Constructor / Destructor
    CPqmPrMode();
    virtual ~CPqmPrMode();


    void Copy(const CPqmPrMode* pqmprmode);
    int  Compare(CPqmPrMode* mode1);//0 if the values are identical 1 if not same


    VftPatori_t GetPatientOrientation()const;

    //+Patni-MP/2009Mar17/Added/ACE-1/MCM0213-00131
    CString GetBodyPart() const;
    void    SetBodyPart(const CString& f_bodyPart);
    //-Patni-MP/2009Mar17/Added/ACE-1/MCM0213-00131

    int  GetRecvCoilId()const;
    void SetRecvCoilId(const int rcv_coil_id);

    int  GetAnatomy() const;
    void SetAnatomy(const int anatomy);

    int  GetContrast() const;
    void SetContrast(const int contrast);

    int  GetScanNumber() const;
    void SetScanNumber(const int scan_number);

    int  GetViewDir() const;
    void SetViewDir(const int view_dir);

    float GetCouchPos() const;
    void  SetCouchPos(const float couch_pos);

    float GetHomePos() const;
    void  SetHomePos(const float home_pos);

    float GetCouchOffset() const;
    void  SetCouchOffset(const float couch_offset);

    int  GetPatoriEndIn() const;
    void SetPatoriEndIn(const int f_end_in);

    int  GetPatoriSideUp() const;
    void SetPatoriSideUp(const int f_side_up);

    //+Patni-Hemant/2010Apr27/Added/JaFT/IR-137
    LateralityEnum GetLaterality() const;
    void SetLaterality(const LateralityEnum f_laterality);
    void SetLaterality(const int f_laterality);
    //-Patni-Hemant/2010Apr27/Added/JaFT/IR-137

private:

    int         m_rcv_coil_id;
    int         m_anatomy;
    int         m_contrast;
    int         m_scan_number;
    int         m_view_dir;
    float       m_couch_pos;
    float       m_home_pos;
    float       m_couch_offset;
    CString     m_bodypart;

    VftPatori_t    m_patori;       // Patient orientation.
    LateralityEnum m_laterality ;

};
#endif // #ifndef __SM_IPCLIB_PQMPRMODE_H__ 

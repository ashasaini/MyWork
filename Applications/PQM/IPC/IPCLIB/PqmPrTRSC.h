//Redmine-783
//Redmine-774
//Redmine-780
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PqmPrTRSC.h
 *  Overview: Structure of CPqmPrTRSC class.
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

#ifndef __SM_IPCLIB_PQMPRTRSC_H__
#define __SM_IPCLIB_PQMPRTRSC_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>

class CPqmPrTRSC : private MR::ACQ::SM::COMMON::NonCopyable<CPqmPrTRSC>
{
public:
    //Constrcutror / Destrcutor

    CPqmPrTRSC();
    virtual ~CPqmPrTRSC();

    BOOL GetAnatomy() const;
    void SetAnatomy(const BOOL anatomy);

    BOOL GetCoil() const;
    void SetCoil(const BOOL coil);

    BOOL GetCouch() const;
    void SetCouch(const BOOL couch);

    BOOL GetContrast() const;
    void SetContrast(const BOOL contrast);

    void SetSARChanged(const BOOL sar_changed);
    void SetChanged(const BOOL changed);

private:

    /* for PqmPqReMakeList */
    BOOL  m_changed;
    BOOL  m_sar_changed;
    /* Changed Flag from Previous Protocol */
    BOOL  m_anatomy;
    BOOL  m_coil;
    BOOL  m_contrast;
    BOOL  m_couch;


    BOOL GetChanged() const;

    BOOL GetSARChanged() const;
};
#endif // #ifndef __SM_IPCLIB_PQMPRTRSC_H__ 

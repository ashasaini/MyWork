//Redmine-783
//Redmine-780
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: couch.h
 *  Overview: Structure of CCouch class.
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
#ifndef __SM_IPCLIB_COUCH_H__
#define __SM_IPCLIB_COUCH_H__
#if _MSC_VER > 1000
#pragma once
#endif

#include <libStudyManager/NonCopyable.h>

class CCouch : private MR::ACQ::SM::COMMON::NonCopyable<CCouch>
{
public:
    // Default contructor for CCouch class
    CCouch();
    ~CCouch();

private:

    int m_couch_postion;                // Couch position
    int m_home_postion;                 // Home position
    int m_couch_offset;                 // Couch offset

    // Set and Get couch postion
    int  GetCouchPostion() const;
    void SetCouchPostion(const int couch_postion);

    // Set and Get couch home postion
    int  GetHomePostion() const;
    void SetHomePostion(const int home_postion);

    // Set and Get couch offset
    int  GetCouchOffset() const;
    void SetCouchOffset(const int couch_offset);


};

#endif // #ifndef __SM_IPCLIB_COUCH_H__ 

//Redmine-783
//Redmine-780
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: couchconfig.h
 *  Overview: Structure of CCouchConfig class.
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
#ifndef __SM_IPCLIB_COUCHCONFIG_H__
#define __SM_IPCLIB_COUCHCONFIG_H__
#if _MSC_VER > 1000
#pragma once
#endif

#include <libStudyManager/NonCopyable.h>

class CCouchConfig : private MR::ACQ::SM::COMMON::NonCopyable<CCouchConfig>
{
public:
    // Default contructor and destructor of CCouchConfig class
    CCouchConfig();
    ~CCouchConfig();


    // Set and Get Couch In limit value
    double GetCouchInLimit() const;
    void   SetCouchInLimit(const double inlimit);

    // Set and Get Couch Out limit value
    double GetCouchOutLimit() const;
    void   SetCouchOutLimit(const double outlimit);

    // Set and Get Couch minimum movement value
    double GetCouchMinimumMovement() const;
    void   SetCouchMinimumMovement(const double min_movement);

    // Set and Get Couch maximum movement value
    double GetCouchMaximumMovement() const;
    void   SetCouchMaximumMovement(const double max_movement);

    // Set and Get Couch maximum parent offset value
    double GetCouchMaximumParentOffset() const;
    void   SetCouchMaximumParentOffset(const double max_parent_offset);

    // Set and Get Couch range value
    //+Patni-SS/20091014/Modified/TMSC review comments
    double GetCouchSpeedRange() const;
    void   SetCouchSpeedRange(const double slow_speed_rangeint);
    //-Patni-SS/20091014/Modified/TMSC review comments

private:

    double m_inlimit;                              // In Limit
    double m_outlimit;                             // Out Limit
    double m_minimum_movement;                     // Minimum Movement
    double m_maximum_movement;                     // Maximum Movement
    double m_maximum_parent_offset;                // Maximum parent offset
    double m_slow_speed_range;                     // Slow speed range
};

#endif // #ifndef __SM_IPCLIB_COUCHCONFIG_H__ 

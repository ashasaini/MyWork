//Redmine-783
//Redmine-780
//Redmine-782
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: couchconfig.cpp
 *  Overview: Implementation of CCouchConfig class.
 *  Last Modified: 2007/8/10 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2007/6/15 12:00:00     PQM5 Baselined code
 *   2.0          Patni            2007/8/10 12:00:00     Implemented PQM6 requ-
                                                            sirements & Formatted
 *   3.0          Patni/LK         2008/5/15 12:00:00     Restructured after TA
                                                            comments
 *****************************************************************************/
#include "stdafx.h"
#include "CouchConfig.h"
#include <PDataParser/INIReader.h>
#include <PQM/PQMLogMgr.h>
#include "PQMConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//****************************Method Header************************************
//Method Name   :CCouchConfig
//Author        :PATNI/JCM
//Purpose       :Constructor
//*****************************************************************************
CCouchConfig::CCouchConfig(
):  m_inlimit(COUCH_INLIMIT),
    m_outlimit(0),
    m_minimum_movement(1),
    m_maximum_movement(COUCH_MAXIMUM_MOVEMENT),
    m_maximum_parent_offset(COUCH_MAXIMUM_PARENT_OFFSET),
    m_slow_speed_range(COUCH_SLOW_SPEED_RANGE)
{
}

//****************************Method Header************************************
//Method Name   :~CCouchConfig
//Author        :PATNI/JCM
//Purpose       :Destructor
//*****************************************************************************
CCouchConfig::~CCouchConfig(
)
{
}

//****************************Method Header************************************
//Method Name   :GetCouchInLimit
//Author        :PATNI/JCM
//Purpose       :This method retrieves the inside limit of the couch
//*****************************************************************************
double CCouchConfig::GetCouchInLimit(
) const
{
    LPCTSTR FUNC_NAME = _T("CCouchConfig::GetCouchInLimit");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    return m_inlimit;
}

//****************************Method Header************************************
//Method Name   :SetCouchInLimit
//Author        :PATNI/JCM
//Purpose       :This method sets the couch inside limit
//*****************************************************************************
void CCouchConfig::SetCouchInLimit(
    const double inlimit
)
{
    LPCTSTR FUNC_NAME = _T("CCouchConfig::SetCouchInLimit");
    CString str_limit(_T(""));
    str_limit.Format(_T("%d"), inlimit);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_limit);

    m_inlimit = inlimit;
}

//****************************Method Header************************************
//Method Name   :GetCouchOutLimit
//Author        :PATNI/JCM
//Purpose       :This method gets couch out limit
//*****************************************************************************
double CCouchConfig::GetCouchOutLimit(
) const
{
    LPCTSTR FUNC_NAME = _T("CCouchConfig::GetCouchOutLimit");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    return m_outlimit;
}

//****************************Method Header************************************
//Method Name   :SetCouchOutLimit
//Author        :PATNI/JCM
//Purpose       :This method sets the couch out limit
//*****************************************************************************
void CCouchConfig::SetCouchOutLimit(
    const double outlimit
)
{
    LPCTSTR FUNC_NAME = _T("CCouchConfig::SetCouchOutLimit");
    CString str_msg(_T(""));
    str_msg.Format(_T("%d"), outlimit);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    m_outlimit = outlimit;
}

//****************************Method Header************************************
//Method Name   :GetCouchMinimumMovement
//Author        :PATNI/JCM
//Purpose       :This method gets the  minimum movement of the couch
//*****************************************************************************
double CCouchConfig::GetCouchMinimumMovement(
) const
{
    LPCTSTR FUNC_NAME = _T("CCouchConfig::GetCouchMinimumMovement");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    return m_minimum_movement;
}

//****************************Method Header************************************
//Method Name   :SetCouchMinimumMovement
//Author        :PATNI/JCM
//Purpose       :This method sets the couch minimum movement
//*****************************************************************************
void CCouchConfig::SetCouchMinimumMovement(
    const double min_movement
)
{
    LPCTSTR FUNC_NAME = _T("CCouchConfig::SetCouchMinimumMovement");
    CString str_msg(_T(""));
    str_msg.Format(_T("%d"), min_movement);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    m_minimum_movement = min_movement;
}

//****************************Method Header************************************
//Method Name   :GetCouchMaximumMovement
//Author        :PATNI/JCM
//Purpose       : This method gets the maximum movement of the couch
//*****************************************************************************
double CCouchConfig::GetCouchMaximumMovement(
) const
{
    LPCTSTR FUNC_NAME = _T("CCouchConfig::GetCouchMaximumMovement");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    return m_maximum_movement;
}

//****************************Method Header************************************
//Method Name   :SetCouchMaximumMovement
//Author        :PATNI/JCM
//Purpose       :This method sets the couch maximum movement
//*****************************************************************************
void CCouchConfig::SetCouchMaximumMovement(
    const double max_movement
)
{
    LPCTSTR FUNC_NAME = _T("CCouchConfig::SetCouchMaximumMovement");
    CString str_msg(_T(""));
    str_msg.Format(_T("%d"), max_movement);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    m_maximum_movement = max_movement;
}

//****************************Method Header************************************
//Method Name   :GetCouchMaximumParentOffset
//Author        :PATNI/LK
//Purpose       :This method gets the couch parent offset from IPCConfig.ini file
//*****************************************************************************
double CCouchConfig::GetCouchMaximumParentOffset(
) const
{
    return m_maximum_parent_offset;
}

//****************************Method Header************************************
//Method Name   :SetCouchParentOffset
//Author        :PATNI/JCM
//Purpose       :This method sets the couch parent offset
//*****************************************************************************
void CCouchConfig::SetCouchMaximumParentOffset(
    const double max_parent_offset
)
{
    CString str_msg(_T(""));
    str_msg.Format(_T("%d"), max_parent_offset);

    m_maximum_parent_offset = max_parent_offset;
}

//+Patni-SS/20091014/Modified/TMSC review comments
//GetCouchRange method name is not correct as per function so it has changed to GetCouchSpeedRange
//****************************Method Header************************************
//Method Name   :GetCouchSpeedRange
//Author        :PATNI/JCM
//Purpose       :This method gets couch range
//*****************************************************************************
double CCouchConfig::GetCouchSpeedRange(
) const
{
    LPCTSTR FUNC_NAME = _T("CCouchConfig::GetCouchSpeedRange");
    CString str_range(_T(""));
    str_range.Format(_T("%d"), m_slow_speed_range);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_range);

    return m_slow_speed_range;
}

//+Patni-SS/20091014/Modified/TMSC review comments
//SetCouchRange method name is not correct as per function so it has changed to SetCouchSpeedRange
//****************************Method Header************************************
//Method Name   :SetCouchSpeedRange
//Author        :PATNI/JCM
//Purpose       :This method sets the couch range
//*****************************************************************************
void CCouchConfig::SetCouchSpeedRange(
    const double slow_speed_rangeint
)
{
    LPCTSTR FUNC_NAME = _T("CCouchConfig::SetCouchSpeedRange");
    CString str_msg(_T(""));
    str_msg.Format(_T("%d"), slow_speed_rangeint);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    m_slow_speed_range = slow_speed_rangeint;
}

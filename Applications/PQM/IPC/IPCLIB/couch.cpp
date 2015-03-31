//Redmine-783
//Redmine-780
//Redmine-782
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: couch.cpp
 *  Overview: Implementation of CCouch class.
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
#include "StdAfx.h"
#include "Couch.h"
#include <PQM/PQMLogMgr.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//****************************Method Header************************************
//Method Name   :CCouch
//Author        :PATNI/JCM
//Purpose       :Constructor
//*****************************************************************************
CCouch::CCouch(
):	m_couch_postion(NULL),
    m_home_postion(NULL),
    m_couch_offset(NULL)
{
}

//****************************Method Header************************************
//Method Name   :~CCouch
//Author        :PATNI/JCM
//Purpose       :Destructor
//*****************************************************************************
CCouch::~CCouch(
)
{
}

//****************************Method Header************************************
//Method Name   :GetCouchPosition
//Author        :PATNI/JCM
//Purpose       :This method gets the couch postion
//*****************************************************************************
int CCouch::GetCouchPostion(
) const
{
    LPCTSTR FUNC_NAME = _T("CCouch::GetCouchPostion");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    return m_couch_postion;
}

//****************************Method Header************************************
//Method Name   :SetCouchPostion
//Author        :PATNI/JCM
//Purpose       :This method sets the couch postion
//*****************************************************************************
void CCouch::SetCouchPostion(
    const int couch_postion
)
{
    LPCTSTR FUNC_NAME = _T("CCouch::SetCouchPostion");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    m_couch_postion = couch_postion;
}

//****************************Method Header************************************
//Method Name   :GetHomePostion
//Author        :PATNI/JCM
//Purpose       :This method gets the couch home postion
//*****************************************************************************
int CCouch::GetHomePostion(
) const
{
    LPCTSTR FUNC_NAME = _T("CCouch::GetHomePostion");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    return m_home_postion;
}

//****************************Method Header************************************
//Method Name   :SetHomePostion
//Author        :PATNI/JCM
//Purpose       :This method sets the home postion of the couch
//*****************************************************************************
void CCouch::SetHomePostion(
    const int home_postion
)
{
    LPCTSTR FUNC_NAME = _T("CCouch::SetHomePostion");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    m_home_postion = home_postion;
}

//****************************Method Header************************************
//Method Name   :GetCouchOffset
//Author        :PATNI/JCM
//Purpose       :This method gets the couch offset
//*****************************************************************************
int CCouch::GetCouchOffset(
) const
{
    LPCTSTR FUNC_NAME = _T("CCouch::GetCouchOffset");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    return  m_couch_offset;
}

//****************************Method Header************************************
//Method Name   :SetCouchOffset
//Author        :PATNI/JCM
//Purpose       :This method sets the couch offset
//*****************************************************************************
void CCouch::SetCouchOffset(
    const int couch_offset
)
{
    LPCTSTR FUNC_NAME = _T("CCouch::SetCouchOffset");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    m_couch_offset = couch_offset;
}

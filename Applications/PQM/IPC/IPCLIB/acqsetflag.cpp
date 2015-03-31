//Redmine-783
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: acqsetflag.cpp
 *  Overview: Implementation of CPSAcqSetFlag class.
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
#include "stdafx.h"
#include "AcqSetFlag.h"
#include <PQM/PQMLogMgr.h>
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//*******************************Method Header*********************************
//Method Name    :CPSAcqSetFlag()
//Author         :PATNI/HAR
//Purpose        :Constrcutor
//*****************************************************************************
CPSAcqSetFlag::CPSAcqSetFlag():
    m_tag(K_PR_PRESCAN_NONE),
    m_data(PQM_VFC_PRESCAN_NONE
          )
{
}

//*******************************Method Header*********************************
//Method Name    :CPSAcqSetFlag()
//Author         :PATNI/HAR
//Purpose        :Overload constructor
//*****************************************************************************
CPSAcqSetFlag::CPSAcqSetFlag(
    const CPSAcqSetFlag::TAG tag,
    const CPSAcqSetFlag::DATA data
): m_tag(tag), m_data(data)
{

}

//*******************************Method Header*********************************
//Method Name    :~CPSAcqSetFlag()
//Author         :PATNI/HAR
//Purpose        :Destructor
//*****************************************************************************
CPSAcqSetFlag::~CPSAcqSetFlag(
)
{
}

//*******************************Method Header*********************************
//Method Name    :GetTag()
//Author         :PATNI/HAR
//Purpose        :Gets the value of tag
//*****************************************************************************
CPSAcqSetFlag::TAG CPSAcqSetFlag::GetTag(
)const
{
    return m_tag;
}

//*******************************Method Header*********************************
//Method Name    :SetTag()
//Author         :PATNI/HAR
//Purpose        :This method sets the value for the tag
//*****************************************************************************
void CPSAcqSetFlag::SetTag(
    const TAG tag
)
{
    LPCTSTR FUNC_NAME = _T("CPSAcqSetFlag::SetTag");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    m_tag = tag;
}

//*******************************Method Header*********************************
//Method Name    :GetData()
//Author         :PATNI/HAR
//Purpose        :Gets the value of data
//*****************************************************************************
CPSAcqSetFlag::DATA CPSAcqSetFlag::GetData(
)const
{
    LPCTSTR FUNC_NAME = _T("CPSAcqSetFlag::GetData");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    return m_data;
}

//*******************************Method Header*********************************
//Method Name    :SetData()
//Author         :PATNI/HAR
//Purpose        :This method sets the value for the tag
//*****************************************************************************
void CPSAcqSetFlag::SetData(
    const DATA data
)
{
    LPCTSTR FUNC_NAME = _T("CPSAcqSetFlag::SetData");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    m_data = data;
}
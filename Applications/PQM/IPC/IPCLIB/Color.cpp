/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: Color.cpp
 *  Overview: Implementation of CColor class.
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
#include "Color.h"
#include <PQM/PQMLogMgr.h>
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CColor, CObject, 1)

//***************************Method Header*************************************

//Method Name    : CColor()

//Author         : PATNI/MSN

//Purpose        : Default Constructor

//*****************************************************************************
CColor::CColor(
)
{
}

//***************************Method Header*************************************

//Method Name    : ~CColor()

//Author         : PATNI/MSN

//Purpose        : Destructor

//*****************************************************************************
CColor::~CColor(
)
{
}

//***************************Method Header*************************************

//Method Name    : Serialize()

//Author         : PATNI/MSN

//Purpose        : Serialize Deserialize

//*****************************************************************************
void CColor::Serialize(
    CArchive& ar
)
{
    LPCTSTR FUNC_NAME = _T("CColor::Serialize");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    CObject::Serialize(ar);

    if (ar.IsLoading()) {
        m_color_list.RemoveAll();
    }

    m_color_list.Serialize(ar);
}
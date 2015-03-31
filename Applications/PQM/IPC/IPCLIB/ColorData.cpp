//Redmine-783
//Redmine-780
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: ColorData.cpp
 *  Overview: Implementation of CColorData class.
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
#include "ColorData.h"
#include <PQM/PQMLogMgr.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CColorData, CObject, 1)

//************************************Method Header**************************************
//Method Name:  CColorData()
//Author:  PATNI/MSN
//Purpose: Constructor
//**************************************************************************************/
CColorData::CColorData(
)
{
    for (int i = 0; i < NUM_4; i++) {
        m_status[i] = L"";
        m_foregroundcolor[i] = 0L;
        m_backgroundcolor[i] = 0L;
    }
}

//************************************Method Header**************************************
//Method Name:  ~CColorData()
//Author:  PATNI/MSN
//Purpose: Destructor
//**************************************************************************************/
CColorData::~CColorData(
)
{
}

//***************************Method Header*************************************
//Method Name    : CColorData()
//Author         : PATNI/MSN
//Purpose        : Copy Constructor
//*****************************************************************************
CColorData::CColorData(
    const CColorData& obj
)
{
    for (int i = 0; i < NUM_4; i++) {
        m_status[i] = obj.m_status[i];
        m_foregroundcolor[i] = obj.m_foregroundcolor[i];
        m_backgroundcolor[i] = obj.m_backgroundcolor[i];
    }
}

//***************************Method Header*************************************
//Method Name    : operator = ()
//Author         : PATNI/MSN
//Purpose        : Overloading the = operator
//*****************************************************************************
CColorData& CColorData::operator = (
    const CColorData& obj
)
{
    if (this == &obj) {
        return *this;
    }

    for (int i = 0; i < NUM_4; i++) {
        m_status[i] = obj.m_status[i];
        m_foregroundcolor[i] = obj.m_foregroundcolor[i];
        m_backgroundcolor[i] = obj.m_backgroundcolor[i];
    }

    return *this;
}

//************************************Method Header**************************************
//Method Name:  SetColorStatus()
//Author:  PATNI/MSN
//Purpose: Sets the status i.e WAIT/CURRENT/FAILED/DONE
//**************************************************************************************/
void CColorData::SetColorStatus(
    const int pos,
    const CString& status
)
{
    m_status[pos] = status;
}

//************************************Method Header**************************************
//Method Name:  SetFcolor()
//Author:  PATNI/MSN
//Purpose: Sets the foreground color
//**************************************************************************************/
void CColorData::SetFcolor(
    const int pos,
    const COLORREF fcolor
)
{
    m_foregroundcolor[pos] = fcolor;
}

//************************************Method Header**************************************
//Method Name:  SetBcolor()
//Author:  PATNI/MSN
//Purpose: Sets the background color
//**************************************************************************************/
void CColorData::SetBcolor(
    const int pos,
    const COLORREF bcolor
)
{
    m_backgroundcolor[pos] = bcolor;
}

//***************************Method Header*************************************
//Method Name    : CColorCopy()
//Author         : PATNI/MSN
//Purpose        : Making the single copy
//*****************************************************************************
void CColorData::CColorCopy(
    const CColorData& obj
)
{
    for (int i = 0; i < NUM_4; i++) {
        m_status[i] = obj.m_status[i];
        m_foregroundcolor[i] = obj.m_foregroundcolor[i];
        m_backgroundcolor[i] = obj.m_backgroundcolor[i];
    }
}

//************************************Method Header**************************************
//Method Name:  Serialize()
//Author:  PATNI/MSN
//Purpose: serializes/ deserializes data
//**************************************************************************************/
void CColorData::Serialize(
    CArchive& ar
)
{
    LPCTSTR FUNC_NAME = _T("CColorData::Serialize");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    CObject::Serialize(ar);

    if (ar.IsStoring()) {
        for (int i = 0; i < NUM_4; i++) {
            ar << m_status[i];
            ar << m_foregroundcolor[i];
            ar << m_backgroundcolor[i];
        }

    } else {

        for (int i = 0; i < NUM_4; i++) {
            ar >> m_status[i];
            ar >> m_foregroundcolor[i];
            ar >> m_backgroundcolor[i];
        }
    }
}


//************************************Method Header**************************************
//Method Name:  GetStatus()
//Author:  PATNI/MSN
//Purpose: Returns the status i.e WAIT/CURRENT/FAILED/DONE
//**************************************************************************************/
CString CColorData::GetStatus(
    const int pos
) const
{
    return m_status[pos];
}

//************************************Method Header**************************************
//Method Name:  GetFcolor()
//Author:  PATNI/MSN
//Purpose: Returns the foreground color
//**************************************************************************************/
DWORD CColorData::GetFcolor(
    const int pos
) const
{
    return m_foregroundcolor[pos];
}


//************************************Method Header**************************************
//Method Name:  GetBcolor()
//Author:  PATNI/MSN
//Purpose: Gets the background color
//**************************************************************************************/
DWORD CColorData::GetBcolor(
    const int pos
) const
{
    return m_backgroundcolor[pos];
}



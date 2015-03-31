//Redmine-783
//Redmine-774
//Redmine-780
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PqmPrTRSC.cpp
 *  Overview: Implementation of CPqmPrTRSC class.
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
#include "PqmPrTRSC.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//****************************Method Header************************************
//Method Name   : CPqmPrTRSC()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
CPqmPrTRSC::CPqmPrTRSC():
    m_changed(FALSE),
    m_sar_changed(FALSE),
    m_anatomy(TRUE),
    m_coil(FALSE),
    m_contrast(FALSE),
    m_couch(FALSE)
{

}

//****************************Method Header************************************
//Method Name   : ~CPqmPrTRSC()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
CPqmPrTRSC::~CPqmPrTRSC(
)
{

}

//****************************Method Header************************************
//Method Name   : GetAnatomy()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
BOOL CPqmPrTRSC::GetAnatomy(
) const
{
    return m_anatomy;
}

//****************************Method Header************************************
//Method Name   : SetAnatomy()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmPrTRSC::SetAnatomy(
    const BOOL anatomy
)
{
    m_anatomy = anatomy;
}

//****************************Method Header************************************
//Method Name   : GetCoil()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
BOOL CPqmPrTRSC::GetCoil(
) const
{
    return m_coil;
}

//****************************Method Header************************************
//Method Name   : SetCoil()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmPrTRSC::SetCoil(
    const BOOL coil
)
{
    m_coil = coil;
}

//****************************Method Header************************************
//Method Name   : GetCouch()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
BOOL CPqmPrTRSC::GetCouch(
) const
{
    return m_couch;
}

//****************************Method Header************************************
//Method Name   : SetCouch()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmPrTRSC::SetCouch(
    const BOOL couch
)
{
    m_couch = couch;
}

//****************************Method Header************************************
//Method Name   : GetContrast()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
BOOL CPqmPrTRSC::GetContrast(
) const
{
    return m_contrast;
}

//****************************Method Header************************************
//Method Name   : SetContrast()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmPrTRSC::SetContrast(
    const BOOL contrast
)
{
    m_contrast = contrast;
}

//****************************Method Header************************************
//Method Name   : SetSARChanged()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmPrTRSC::SetSARChanged(
    const BOOL sar_changed
)
{
    m_sar_changed = sar_changed;
}

//****************************Method Header************************************
//Method Name   : SetChanged()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmPrTRSC::SetChanged(
    const BOOL changed
)
{
    m_changed = changed;
}

//****************************Method Header************************************
//Method Name   : GetChanged()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
BOOL CPqmPrTRSC::GetChanged(
) const
{
    return m_changed;
}

//****************************Method Header************************************
//Method Name   : GetSARChanged()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
BOOL CPqmPrTRSC::GetSARChanged(
) const
{
    return m_sar_changed;
}



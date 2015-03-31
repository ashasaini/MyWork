//Redmine-783
//Redmine-780
//Redmine-782
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PqmPrMode.cpp
 *  Overview: Implementation of CPqmPrMode class.
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
// PqmPrMode.cpp: implementation of the CPqmPrMode class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "PqmPrMode.h"
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//****************************Method Header************************************
//Method Name   : CPqmPrMode()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
CPqmPrMode::CPqmPrMode():
    m_rcv_coil_id(-1),
    m_anatomy(0),
    m_contrast(0),
    m_scan_number(0),
    m_view_dir(0),
    m_couch_pos((float)COUCH_POSITION),
    m_home_pos((float)COUCH_HOME_POSITION),
    m_couch_offset((float)COUCH_OFFSET),
    m_bodypart(_T(""))
{
    m_patori.end_in = -1;
    m_patori.side_up = -1;
}

//****************************Method Header************************************
//Method Name   : ~CPqmPrMode()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
CPqmPrMode::~CPqmPrMode(
)
{

}

void CPqmPrMode::Copy(
    const CPqmPrMode* pqmprmode
)
{
    m_rcv_coil_id = pqmprmode->m_rcv_coil_id;
    m_anatomy = pqmprmode->m_anatomy;
    m_contrast = pqmprmode->m_contrast;
    m_scan_number = pqmprmode->m_scan_number;
    m_view_dir = pqmprmode->m_view_dir;
    m_couch_pos = pqmprmode->m_couch_pos;
    m_home_pos = pqmprmode->m_home_pos;
    m_couch_offset = pqmprmode->m_couch_offset;
    //+Patni-MP/2009Mar17/Added/ACE-1/MCM0213-00131
    m_bodypart = pqmprmode->m_bodypart;
    //-Patni-MP/2009Mar17/Added/ACE-1/MCM0213-00131

    m_patori.end_in = pqmprmode->m_patori.end_in;
    m_patori.side_up = pqmprmode->m_patori.side_up;
    //+Patni-Hemant/2010Apr28/Added/JaFT/IR-137
    m_laterality = pqmprmode->m_laterality;
    //-Patni-Hemant/2010Apr28/Added/JaFT/IR-137
    //return this;
}

int CPqmPrMode::Compare(
    CPqmPrMode* mode1
)
{
    if (m_rcv_coil_id != mode1->m_rcv_coil_id)
        return 1;

    if (m_contrast != mode1->m_contrast)
        return 1;

    if (m_scan_number != mode1->m_scan_number)
        return 1;

    if (m_view_dir != mode1->m_view_dir)
        return 1;

    if (m_couch_pos != mode1->m_couch_pos)
        return 1;

    if (m_home_pos != mode1->m_home_pos)
        return 1;

    if (m_couch_offset != mode1->m_couch_offset)
        return 1;

    if (m_anatomy != mode1->m_anatomy)
        return 1;

    if (m_patori.end_in != mode1->m_patori.end_in)
        return 1;

    if (m_patori.side_up != mode1->m_patori.side_up)
        return 1;

    //+Patni-MP/2009Mar17/Added/ACE-1/MCM0213-00131
    if (m_bodypart.CompareNoCase(mode1->GetBodyPart()) != 0)
        return 1;

    //-Patni-MP/2009Mar17/Added/ACE-1/MCM0213-00131

    //+Patni-Hemant/2010Apr27/Added/JaFT/IR-137
    if (GetLaterality() != mode1->GetLaterality())
        return 1;

    //-Patni-Hemant/2010Apr27/Added/JaFT/IR-137

    return 0;
}

//****************************Method Header************************************
//Method Name   : GetPatientOrientation()
//Author        : PATNI/AMT
//Purpose       : Returns Patient orientation structure.
//*****************************************************************************
VftPatori_t CPqmPrMode::GetPatientOrientation()const
{
    return m_patori;
}

//+Patni-MP/2009Mar17/Added/ACE-1/MCM0213-00131
//****************************Method Header************************************
//Method Name   : GetBodyPart()
//Author        : PATNI/MRP
//Purpose       : Returns Body Part.
//*****************************************************************************
CString CPqmPrMode::GetBodyPart(
) const
{
    return m_bodypart;
}

//****************************Method Header************************************
//Method Name   : SetBodyPart()
//Author        : PATNI/MRP
//Purpose       : Sets the Body Part
//*****************************************************************************
void CPqmPrMode::SetBodyPart(
    const CString& f_bodyPart
)
{
    m_bodypart = f_bodyPart;
}
//-Patni-MP/2009Mar17/Added/ACE-1/MCM0213-00131

//****************************Method Header************************************
//Method Name   : GetReceiveCoilID()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmPrMode::GetRecvCoilId(
) const
{
    return m_rcv_coil_id;
}

//****************************Method Header************************************
//Method Name   : SetReceiveCoilID()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmPrMode::SetRecvCoilId(
    const int rcv_coil_id
)
{
    m_rcv_coil_id = rcv_coil_id;
}

//****************************Method Header************************************
//Method Name   : GetAnatomy()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmPrMode::GetAnatomy(
) const
{
    return m_anatomy;
}

//****************************Method Header************************************
//Method Name   : SetAnatomy()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmPrMode::SetAnatomy(
    const int anatomy
)
{
    m_anatomy = anatomy;
}

//****************************Method Header************************************
//Method Name   : GetContrast()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmPrMode::GetContrast(
) const
{
    return m_contrast;
}

//****************************Method Header************************************
//Method Name   : SetContrast()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmPrMode::SetContrast(
    const int contrast
)
{
    m_contrast = contrast;
}

//****************************Method Header************************************
//Method Name   : GetScanNumber()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmPrMode::GetScanNumber(
) const
{
    return m_scan_number;
}

//****************************Method Header************************************
//Method Name   : SetScanNumber()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmPrMode::SetScanNumber(
    const int scan_number
)
{
    m_scan_number = scan_number;
}

//****************************Method Header************************************
//Method Name   : GetViewDir()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmPrMode::GetViewDir(
) const
{
    return m_view_dir;
}

//****************************Method Header************************************
//Method Name   : SetViewDir()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmPrMode::SetViewDir(
    const int view_dir
)
{
    m_view_dir = view_dir;
}


//****************************Method Header************************************
//Method Name   : GetCouchPos()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
float CPqmPrMode::GetCouchPos(
) const
{
    return m_couch_pos;
}

//****************************Method Header************************************
//Method Name   : SetCouchPos()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmPrMode::SetCouchPos(
    const float couch_pos
)
{
    m_couch_pos = couch_pos;
}

//****************************Method Header************************************
//Method Name   : GetHomePos()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
float CPqmPrMode::GetHomePos(
) const
{
    return m_home_pos;
}

//****************************Method Header************************************
//Method Name   : SetHomePos()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmPrMode::SetHomePos(
    const float home_pos
)
{
    m_home_pos = home_pos;
}

//****************************Method Header************************************
//Method Name   : GetCouchOffset()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
float CPqmPrMode::GetCouchOffset(
) const
{
    return m_couch_offset;
}

//****************************Method Header************************************
//Method Name   : SetCouchOffset()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmPrMode::SetCouchOffset(
    const float couch_offset
)
{
    m_couch_offset = couch_offset;
}

//****************************Method Header************************************
//Method Name   : GetViewDir()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmPrMode::GetPatoriEndIn(
)const
{
    return m_patori.end_in;
}

//****************************Method Header************************************
//Method Name   : SetViewDir()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmPrMode::SetPatoriEndIn(
    const int f_end_in
)
{
    m_patori.end_in = f_end_in;
}

//****************************Method Header************************************
//Method Name   : GetPatoriSideUp()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
int CPqmPrMode::GetPatoriSideUp(
) const
{
    return m_patori.side_up;
}

//****************************Method Header************************************
//Method Name   : SetPatoriSideUp()
//Author        : PATNI\HAR - SS
//Purpose       :
//*****************************************************************************
void CPqmPrMode::SetPatoriSideUp(
    const int f_side_up
)
{
    m_patori.side_up = f_side_up;
}

//+Patni-Hemant/2010Apr27/Added/JaFT/IR-137
//************************************Method Header************************************
// Method Name  : GetLaterality
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
LateralityEnum CPqmPrMode::GetLaterality() const
{
    return m_laterality;
}

//************************************Method Header************************************
// Method Name  :SetLaterality
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPqmPrMode::SetLaterality(
    const LateralityEnum f_laterality
)
{
    m_laterality = f_laterality;
}

//************************************Method Header************************************
// Method Name  :SetLaterality
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPqmPrMode::SetLaterality(
    const int f_laterality
)
{
    m_laterality = (LateralityEnum)f_laterality;
}

//-Patni-Hemant/2010Apr27/Added/JaFT/IR-137

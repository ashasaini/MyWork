// RoiParams.cpp: implementation of the CRoiParams class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#include "stdafx.h"
#include "CircleROIParams.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCircleROIParams::CCircleROIParams() : m_fCenter_Y(0),
m_fCenter_X(0),
m_fRadius(0)
{
    //
}

CCircleROIParams::~CCircleROIParams()
{
    //
}

///////////////////////////////////////////////////////////////////////////////
// Member Functions
///////////////////

/*************************************************************
* Method:       GetRadius
* Description:  This function is used
* Returns:      float
*************************************************************/
float CCircleROIParams::GetRadius() const
{
    return m_fRadius;
}
/*************************************************************
* Method:       SetRadius
* Description:  This function is used
* Parameter:    float val
* Returns:      void
*************************************************************/
void CCircleROIParams::SetRadius(float val)
{
    m_fRadius = val;
}
/*************************************************************
* Method:       GetCenter_Y
* Description:  This function is used
* Returns:      float
*************************************************************/
float CCircleROIParams::GetCenter_Y() const
{
    return m_fCenter_Y;
}
/*************************************************************
* Method:       SetCenter_Y
* Description:  This function is used
* Parameter:    float val
* Returns:      void
*************************************************************/
void CCircleROIParams::SetCenter_Y(float val)
{
    m_fCenter_Y = val;
}
/*************************************************************
* Method:       GetCenter_X
* Description:  This function is used
* Returns:      float
*************************************************************/
float CCircleROIParams::GetCenter_X() const
{
    return m_fCenter_X;
}
/*************************************************************
* Method:       SetCenter_X
* Description:  This function is used
* Parameter:    float val
* Returns:      void
*************************************************************/
void CCircleROIParams::SetCenter_X(float val)
{
    m_fCenter_X = val;
}

///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////
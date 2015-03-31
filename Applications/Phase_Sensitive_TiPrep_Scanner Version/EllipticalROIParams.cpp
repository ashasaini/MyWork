// EllipticalROIParams.cpp: implementation of the CEllipticalROIParams class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
// Class for ellipse coordinate saving.
#include "stdafx.h"
#include "EllipticalROIParams.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEllipticalROIParams::CEllipticalROIParams() :m_fCenter_Y(0.0), m_fCenter_X(0.0), m_fHeight(0.0),
    m_fWidth(0.0)    
{
}

CEllipticalROIParams::~CEllipticalROIParams()
{
    //
}

///////////////////////////////////////////////////////////////////////////////
// Member Functions
///////////////////


float CEllipticalROIParams::GetHeight() const
{
    return m_fHeight;
}
void CEllipticalROIParams::SetHeight(float val)
{
    m_fHeight = val;
}
float CEllipticalROIParams::GetWidth() const
{
    return m_fWidth;
}
void CEllipticalROIParams::SetWidth(float val)
{
    m_fWidth = val;
}
float CEllipticalROIParams::GetCenter_Y() const
{
    return m_fCenter_Y;
}
void CEllipticalROIParams::SetCenter_Y(float val)
{
    m_fCenter_Y = val;
}
float CEllipticalROIParams::GetCenter_X() const
{
    return m_fCenter_X;
}
void CEllipticalROIParams::SetCenter_X(float val)
{
    m_fCenter_X = val;
}

///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////

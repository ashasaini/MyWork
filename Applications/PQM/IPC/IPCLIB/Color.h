/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: Color.h
 *  Overview: Structure of CColor class.
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
#ifndef __SM_IPCLIB_COLOR_H__
#define __SM_IPCLIB_COLOR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include <libStudyManager/NonCopyable.h>
#include "ColorData.h"

class CColor : public CObject
    , private MR::ACQ::SM::COMMON::NonCopyable<CColor>
{
public:

    CList <CColorData, CColorData> m_color_list;

    CColor();
    virtual ~CColor();

    void Serialize(CArchive& ar);

private:
    DECLARE_SERIAL(CColor);
};

#endif // #ifndef __SM_IPCLIB_COLOR_H__ 

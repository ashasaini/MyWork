//Redmine-783
//Redmine-780
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: ColorSetting.h
 *  Overview: Structure of CColorSetting class.
 *  Last Modified: 2007/8/10 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2007/6/15 12:00:00     PQM5 Baselined code
 *   2.0          Patni            2007/8/10 12:00:00     Implemented PQM6 requirements & Formatted
 *   3.0          Patni/LK         2008/5/10  12:00:00    Code Restructured after
                                                            TA comments
 *****************************************************************************/
#ifndef __SM_IPCLIB_COLOR_SETTING_H__
#define __SM_IPCLIB_COLOR_SETTING_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>

class CColor;
class CColorData;

class CColorSetting : private MR::ACQ::SM::COMMON::NonCopyable<CColorSetting>
{
public:
    //Constructor / Destrcutor
    CColorSetting();
    virtual ~CColorSetting();

    // Read the color settings info from config
    BOOL ReadColorSetting();
    CColor* const GetColorSetting()const;

private:

    CColor* m_color;
    CColorData* m_colordata;

    // Initialize the color for the given protocol status
    int InitColor(const CString& f_protocol_status, const int f_index);

    void SetColorDataList(const CColorData& colorobj);
};

#endif // #ifndef __SM_IPCLIB_COLOR_SETTING_H__ 

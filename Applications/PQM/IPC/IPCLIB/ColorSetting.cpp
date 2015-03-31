//Redmine-783
//Redmine-780
//Redmine-782
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: ColorSetting.cpp
 *  Overview: Implementation of CColorSetting class.
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
#include "stdafx.h"
#include "ColorSetting.h"
#include <math.h>
#include <PQM/Resource.h>
#include <PQM/PQMLogMgr.h>
#include <PQM/PqmUtilities.h>		// Added By KT/ACE-2/15-06-2009/Multilingualisation
#include "pqmconfig.h"
#include "Color.h"



//************************************Method Header**************************************
//Method Name:  CColorSetting()
//Author:  PATNI/MSN
//Purpose: Constructor
//**************************************************************************************/
CColorSetting::CColorSetting():
    m_color(NULL),
    m_colordata(NULL)
{
}

//************************************Method Header**************************************
//Method Name:  CColorSetting()
//Author:  PATNI/MSN
//Purpose: Destructor
//**************************************************************************************/
CColorSetting::~CColorSetting(
)
{
    DEL_PTR(m_colordata);
    DEL_PTR(m_color);
}

//************************************Method Header**************************************
//Method Name:  ReadColorSetting()
//Author:  PATNI/LK
//Purpose: Read Color Setting File and store it in color data
//**************************************************************************************/
BOOL CColorSetting::ReadColorSetting(
)
{
    m_colordata = new CColorData;
    m_color = new CColor;

    int l_index = 0;

    // store values in color data for WAIT status
    int l_status = InitColor(STATUS_WAIT, l_index++);
    // store values in color data for CURRENT status
    l_status = InitColor(STATUS_CURRENT, l_index++);
    // store values in color data for FAILED status
    l_status = InitColor(STATUS_FAILED, l_index++);
    // store values in color data for DONE status
    l_status = InitColor(STATUS_DONE, l_index++);

    SetColorDataList(*m_colordata);

    if (l_status == INVALID_XML_FILE) {
        CPQMLogMgr::GetInstance()->WriteEventLog(
            _T("Unable to load the XML File"),
            ERROR_LOG_INFORMATION1,
            _T("ColorSetting.cpp"),
            __LINE__);//Patni-KSS/2010May19/ADDED/IR-82

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_LOAD_XML_FILE_ERROR")),
            _T("ColorSetting.cpp"),
            __LINE__,
            ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return FALSE;

    } else {
        return TRUE;
    }

    return TRUE;
}

//****************************Method Header************************************
//Method Name   :GetColorSetting()
//Author        :PATNI/HAR
//Purpose       :Return the CColor Pointer
//*****************************************************************************
CColor* const CColorSetting::GetColorSetting(
)const
{
    return m_color;
}

//************************************Method Header**************************************
//Method Name:  InitColor()
//Author:  PATNI/LK
//Purpose: Read Color Setting File and store it in color data
//**************************************************************************************/
int CColorSetting::InitColor(
    const CString& f_protocol_status,
    const int f_index
)
{
    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    //COLORREF l_foreground_colorref, l_background_colorref;
    COLORREF l_foreground_colorref = 0, l_background_colorref = 0;

    int l_status1 = CPQMConfig::GetInstance()->GetBackGroundColor(f_protocol_status,
                    l_background_colorref);
    int l_status2 = CPQMConfig::GetInstance()->GetForeGroundColor(f_protocol_status,
                    l_foreground_colorref);

    // store values in color data for given protocol status
    if (m_colordata) {
        m_colordata->SetColorStatus(f_index, f_protocol_status);
        m_colordata->SetFcolor(f_index, l_foreground_colorref);
        m_colordata->SetBcolor(f_index, l_background_colorref);
    }

    if (l_status1 == INVALID_COLOR_VALUE || l_status2 == INVALID_COLOR_VALUE) {
        CPQMLogMgr::GetInstance()->WriteEventLog(
            _T("Invalid color values present in XML File"),
            ERROR_LOG_INFORMATION1,
            _T("ColorSetting.cpp"),
            __LINE__);//Patni-KSS/2010May19/ADDED/IR-82

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_INVALID_COLOR_VALUES_IN_XML_FILE")),
            _T("ColorSetting.cpp"),
            __LINE__,
            ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);

        return INVALID_COLOR_VALUE;

    } else if (l_status1 == INVALID_PROTOCOL_STATUS || l_status2 == INVALID_PROTOCOL_STATUS) {
        CPQMLogMgr::GetInstance()->WriteEventLog(
            _T("Unable to get the protocol status from XML File"),
            ERROR_LOG_INFORMATION1,
            _T("ColorSetting.cpp"),
            __LINE__);//Patni-KSS/2010May19/ADDED/IR-82


        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_PROTOCOL_STATUS_FROM_XML_FILE_ERROR")),
            _T("ColorSetting.cpp"),
            __LINE__,
            ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return INVALID_PROTOCOL_STATUS;

    } else if (l_status1 == INVALID_XML_FILE || l_status2 == INVALID_XML_FILE) {
        return INVALID_XML_FILE;
    }

    return E_NO_ERROR;
}

//****************************Method Header************************************
//Method Name   :SetColorDataList()
//Author        :PATNI/HAR
//Purpose       :Added Color Object into CList
//*****************************************************************************
void CColorSetting::SetColorDataList(
    const CColorData& colorobj
)
{
    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_color) {
        m_color->m_color_list.AddTail(colorobj);
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}



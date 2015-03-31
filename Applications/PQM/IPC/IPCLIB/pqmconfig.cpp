//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-779
//Redmine-780
//Redmine-781
//Redmine-782
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: pqmconfig.cpp
 *  Overview: Implementation of CPQMConfig class.
 *  Last Modified: 2008/5/20 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2007/6/15 12:00:00     PQM5 Baselined code
 *   2.0          Patni            2007/8/10 12:00:00     Implemented PQM6 requ-
                                                            irements & Formatted
.*   3.0          Patni/LK         2007/8/10 12:00:00     Restructured after
                                                            TA comments
 *****************************************************************************/
#include "stdafx.h"
#include "PQMConfig.h"
#include <math.h>
#include <PDataParser/INIReader.h>
#include <libStudyManager/strcnv.h> //Tanoue/MVC5849
#include <PQM/PQMLogMgr.h>
#include <PQM/PqmUtilities.h> // Added By KT/ACE-2/15-06-2009/Multilingualisation
#include <PQM/resource.h>
#include "PQMXMLParser.h"
#include "ScanAnatomyReader.h"
#include "ScanCommentXMLReader.h"

//+Patni-Ajay/2010July23/Added/TMSC-REDMINE-443
#include "PlanDuplicateXMLReader.h"
//Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
#include "SFTMenuXMLReader.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CPQMConfig* CPQMConfig::m_pqmconfig = NULL;
BOOL CPQMConfig::m_is_pqm_app_settings_loaded = FALSE;
BOOL CPQMConfig::m_is_color_settings_loaded = FALSE;
BOOL CPQMConfig::m_is_scan_anatomy_loaded = FALSE;

CIniReader* CPQMConfig::m_ipcconfig_reader = NULL;
CIniReader* CPQMConfig::m_map_reader = NULL;
CPQMXMLParser* CPQMConfig::m_colorsettings_reader = NULL;
CPQMXMLParser* CPQMConfig::m_pqmappconfig_reader = NULL;

//+Patni-ARD/2009Oct28/Added/IR 85 Auto Transfer
CPQMXMLParser* CPQMConfig::m_pqm_auto_transfer_reader = NULL;
BOOL CPQMConfig::m_is_auto_transfer_reader = FALSE;
//-Patni-ARD/2009Oct28/Added/IR 85 Auto Transfer
CScanAnatomyReader*	CPQMConfig::m_scan_anatomy_reader = NULL ;	//SM3_Mangesh
//+Patni-Ajay/2010July23/Added/TMSC-REDMINE-443
CPlanDuplicateXMLReader*	CPQMConfig::m_preset_item_reader = NULL ;
CScanCommentXMLReader*	CPQMConfig::m_scan_comment_reader = NULL ;

using namespace MR::ACQ::SM::COMMON; //Tanoue/MVC5849

//********************************Method Header********************************
//Method Name   :GetInstance()
//Author        :PATNI/LK
//Purpose       :Get Single Instance of CPQMConfig
//*****************************************************************************
CPQMConfig* CPQMConfig::GetInstance(
)
{
    if (m_pqmconfig == NULL) {
        m_pqmconfig = new CPQMConfig;

        if (!m_pqmconfig->InitializeConfig()) {
            //+ MEITEC/2010Aug10/Commented/REDMINE-144
            //delete m_pqmconfig;
            //m_pqmconfig = NULL;
            //- MEITEC/2010Aug10/Commented/REDMINE-144
        }

        //Patni-ARD/2009Oct28/Added/IR 85 Auto Transfer
        m_pqmconfig->ReadAndSetAutoJobTagNumber();
    }

    return m_pqmconfig;
}

//********************************Method Header********************************
//Method Name   :~CPQMConfig()
//Author        :PATNI/LK
//Purpose       :Destructor
//*****************************************************************************
CPQMConfig::~CPQMConfig(
)
{
    DEL_PTR(m_ipcconfig_reader);
    DEL_PTR(m_map_reader);
    DEL_PTR(m_colorsettings_reader);
    DEL_PTR(m_pqmappconfig_reader);
    DEL_PTR(m_scan_anatomy_reader);

    //Patni-ARD/2009Oct28/Added/IR 85 Auto Transfer
    DEL_PTR(m_pqm_auto_transfer_reader);

    //+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
    DEL_PTR(m_sft_menu_parser);
    DEL_PTR(m_tool_menu_parser);
    //-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu

    //Patni-AJS/2010Sep3/Added/ScanCommentDialog/IR-163
    DEL_PTR(m_scan_comment_reader);
}

//********************************Method Header********************************
//Method Name   :IsSkipRGN()
//Author        :PATNI/LK
//Purpose       :To get the value for SkipRGN
//*****************************************************************************
bool CPQMConfig::IsSkipRGN(
) const
{
    LPCTSTR FUNC_NAME = _T("CPQMConfig::IsSkipRGN");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    return m_skip_rgn;
}

//********************************Method Header********************************
//Method Name   :ReadMaxProtocolLimitfromFile()
//Author        :PATNI/LK
//Purpose       :To Retrieve the MAX Protocol Limit from PQMApp_Config.xml file
//*****************************************************************************
long CPQMConfig::ReadMaxProtocolLimitfromFile(
)
{
    if (m_is_pqm_app_settings_loaded) {
        CPTString l_maxprotocol;
        l_maxprotocol = m_pqmappconfig_reader->GetAttributeValue(0,
                        NODE_TAG_NAME_PROTOCOL_VALUE,
                        CHILD_TAG_NAME_MAXLIMIT,
                        ATTRIBUTE_NAME_1);
        return m_pqmappconfig_reader->ReadLongValue(l_maxprotocol);

    } else {
        return MAXPROTOCOL_DEFAULT;
    }
}

//********************************Method Header********************************
//Method Name   :IsUniCodeSupportedAcqMan()
//Author        :PATNI/LK
//Purpose       :Retrieves whether MPlus AcqMan had been
//                                used or not by reading PQMApp_Config.xml file
//*****************************************************************************
bool CPQMConfig::IsUniCodeSupportedAcqMan(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMConfig::IsUniCodeSupportedAcqMan");

    if (m_is_pqm_app_settings_loaded) {
        CPTString l_acqmantype;

        l_acqmantype = m_pqmappconfig_reader->GetAttributeValue(1,
                       NODE_TAG_NAME_UNICODE,
                       CHILD_TAG_NAME_UNI_FORMAT,
                       ATTRIBUTE_NAME_1);

        if (l_acqmantype.CompareNoCase(UNICODE_SUPPORTED) == 0) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Unicode AcqMan"));
            return true;

        } else {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Non-Unicode/ASCII AcqMan"));
            return false;
        }

    } else {
        return false;
    }
}

//********************************Method Header********************************
//Method Name   :CanEnableAutoTransfer()
//Author        :PATNI/ARD
//Purpose       :Get the number of AutoJob tag in MplusMRSrvScanAPPSite.xml
//*****************************************************************************
bool CPQMConfig::CanEnableAutoTransfer() const
{
    return (m_nAuto_job_number > 0);
}

//********************************Method Header********************************
//Method Name   :ReadAndSetAutoJobTagNumber()
//Author        :PATNI/ARD
//Purpose       :Read and set auto job tag number present in MplusMRSrvScanAPPSite.xml
//*****************************************************************************
void CPQMConfig::ReadAndSetAutoJobTagNumber()
{
    LPCTSTR FUNC_NAME = _T("CPQMConfig::ReadAndSetAutoJobTagNumber");
    int l_auto_job_number = 0;

    if (m_is_auto_transfer_reader && m_pqm_auto_transfer_reader) {
        int l_nodecnt = m_pqm_auto_transfer_reader->GetNodeCnt();

        if (l_nodecnt > 0) {
            for (int l_index = 0; l_index < l_nodecnt; l_index++) {

                int listpos = m_pqm_auto_transfer_reader->GetTagPosition(l_index,
                              NODE_TAG_NAME_AUTO_JOB_SETTING,
                              CHILD_TAG_NAME_AUTOJOB,
                              ATTRIBUTE_NAME_1);

                if (listpos != -1) {
                    l_auto_job_number++;
                }
            }
        }
    }

    m_nAuto_job_number = l_auto_job_number;

    CString l_log_string(_T(""));
    l_log_string.Format(_T("NO_OF_AUTO_JOB Tag = %d "), m_nAuto_job_number);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_string);


}
//-Patni-ARD/2009Oct28/Added/IR 85 Auto Transfer

void CPQMConfig::LoadToolMenu(
    const bool f_flag
)
{
    m_load_tool_menu = f_flag ;
}

//+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
void CPQMConfig::LoadSFTConfigMenu()
{
    LPCTSTR FUNC_NAME = _T("CPQMConfig::LoadSFTConfigMenu");

    //+Patni-RUP/2010Jul29/Modified/Added Try Catch
    try {
        //-Patni-RUP/2010Jul29/Modified/Added Try Catch
        m_sft_menu_parser = new CSFTMenuXMLReader();

        if (m_sft_menu_parser) {
            if (!m_sft_menu_parser->LoadXMLFile(CPQMConfig::GetSFTMenuFileName())) {

                //+Patni-Hemant/2010July21/Commented/IR-152_Part_2
                //l_error = CPqmUtilities::GetMultiLingualString(_T("IDS_SFT_MENU_CONFIG_FILE_MISSING"));
                //CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
                //	l_error, MPlus::EH::Crucial, APP_NAME, APP_NAME);
                //-Patni-Hemant/2010July21/Commented/IR-152_Part_2
            }
        }

        if (m_load_tool_menu) {
            m_tool_menu_parser = new CSFTMenuXMLReader();

            if (m_tool_menu_parser) {
                if (!m_tool_menu_parser->LoadXMLFile(CPQMConfig::GetToolMenuFileName())) {

                    CString l_error = CPqmUtilities::GetMultiLingualString(_T("IDS_TOOL_MENU_CONFIG_FILE_MISSING"));
                    CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
                        l_error, MPlus::EH::Crucial, APP_NAME, APP_NAME);
                }
            }
        }

        //+Patni-RUP/2010Jul29/Modified/Added Try Catch

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception occured"));
    }

    //-Patni-RUP/2010Jul29/Modified/Added Try Catch
}
//-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu

//********************************Method Header********************************
//Method Name   :GetJetChannel_V()
//Author        :PATNI/MRP
//Purpose       :Get GetJetChannel_V from IPCConfig.ini
//*****************************************************************************
int CPQMConfig::GetJetChannel_V(
) const
{
    //+Patni-ARD/2010Mar3/Modified/DeFT# IR-100 remove IPCConfig.ini
    /*
    int l_JetChannel_V = 0;
    CString l_jetvalue = _T("");

    if (m_ipcconfig_reader->SectionExists(_T("JETSCAN"))) {
        l_jetvalue = m_ipcconfig_reader->GetKeyValue(_T("JETChannel_V"), _T("JETSCAN"));
        l_JetChannel_V = _ttoi(l_jetvalue);

        if (l_JetChannel_V < 0) {
            CString l_err_str(_T(""));
            l_err_str.LoadString(IDS_JETCHANNEL_V_ILLEGAL_VALUE_FOUND);

            CPQMLogMgr::GetInstance()->WriteEventLog(IDS_JETCHANNEL_V_ILLEGAL_VALUE_FOUND,
                    ERROR_LOG_CRUCIAL1,
                    _T("PQMConfig.cpp"));
            l_JetChannel_V = 0;
        }

        return l_JetChannel_V;

    } else {
        CString l_err_str(_T(""));
        // l_err_str.LoadString(IDS_JETCHANNEL_V_NOT_FOUND);

        CPQMLogMgr::GetInstance()->WriteEventLog(IDS_JETCHANNEL_V_NOT_FOUND,
                ERROR_LOG_CRUCIAL1,
                _T("PQMConfig.cpp"));

        l_err_str = CPqmUtilities::GetMultiLingualString(_T("IDS_JETCHANNEL_V_NOT_FOUND"));	// Added By KT/ACE-2/15-06-2009/Multilingualisation

        CPQMLogMgr::GetInstance()->DisplayError(l_err_str,
                                                _T("PQMConfig.cpp"), __LINE__, ERROR_LOG_CRUCIAL1 ,
                                                ERR_ID, APP_NAME);

        return l_JetChannel_V;
    }
    */
    return m_JetChannel_V;
    //-Patni-ARD/2010Mar3/Modified/DeFT# IR-100 remove IPCConfig.ini
}

//+Patni-ARD/2010Mar3/Added/DeFT# IR-100 remove IPCConfig.ini
//********************************Method Header********************************
//Method Name   :SetJetChannel_V()
//Author        :PATNI/ARD
//Purpose       :Set Jet channel value in PQMConfig.ini
//*****************************************************************************
void CPQMConfig::SetJetChannel_V(const int f_jet_channel_v)
{
    m_JetChannel_V = f_jet_channel_v;
}
//-Patni-ARD/2010Mar3/Added/DeFT# IR-100 remove IPCConfig.ini

//********************************Method Header********************************
//Method Name   :GetRxGainOffset()
//Author        :PATNI/MRP
//Purpose       :To get the RxGainOffset
//*****************************************************************************
float CPQMConfig::GetRxGainOffset(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMConfig::GetRxGainOffset");
    CString str_msg(_T(""));
    str_msg.Format(_T("%f"), m_rxgain_offset);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    return m_rxgain_offset;
}

//********************************Method Header********************************
//Method Name   :SetRxGainOffset()
//Author        :PATNI/MRP
//Purpose       :To set the value for RxGainOffset
//*****************************************************************************
void CPQMConfig::SetRxGainOffset(
    const float offset
)
{
    LPCTSTR FUNC_NAME = _T("CPQMConfig::SetRxGainOffset");
    CString str_msg(_T(""));
    str_msg.Format(_T("%f"), offset);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    m_rxgain_offset = offset;
}

//********************************Method Header********************************
//Method Name   :GetCouchMaxParentOffset()
//Author        :PATNI/LK
//Purpose       :Get Couch Maximum Parent Offset value from IPCConfig.ini
//*****************************************************************************
CString CPQMConfig::GetCouchMaxParentOffset(
)const
{
    if (m_ipcconfig_reader->SectionExists(_T("TESTING"))) {
        return m_ipcconfig_reader->GetKeyValue(
                   _T("CouchMaximumParentOffset"),
                   _T("TESTING"));

    } else {
        CString l_str(_T(""));
        // l_err_str.LoadString(IDS_STUDY_NOT_FOUND); // Added By KT/ACE-2/15-06-2009/Multilingualisation

        //+Patni-PJS/2009May10/Modify/IR-82 review comment
        l_str.LoadString(IDS_NO_READ_VALUE);
        CPQMLogMgr::GetInstance()->WriteEventLog(l_str,
                ERROR_LOG_CRUCIAL1,
                _T("PQMConfig.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82
        /*
        CPQMLogMgr::GetInstance()->WriteEventLog(IDS_NO_READ_VALUE,
                ERROR_LOG_CRUCIAL1,
                _T("PQMConfig.cpp"));

        */
        //-Patni-PJS/2009May10/Modify/IR-82 review comment

        CPQMLogMgr::GetInstance()->DisplayError(CPqmUtilities::GetMultiLingualString(_T("IDS_STUDY_NOT_FOUND")),
                                                _T("PQMConfig.cpp"), __LINE__, ERROR_LOG_CRUCIAL1 ,
                                                ERR_ID, APP_NAME);

        return _T("");
    }

}

//********************************Method Header********************************
//Method Name   :GetBackGroundColor()
//Author        :PATNI/LK
//Purpose       :To Retrieve the Back Ground color at the given index
//*****************************************************************************
int CPQMConfig::GetBackGroundColor(
    const CString& f_protocol_status,
    COLORREF& f_backgroundcolor
)
{
    int l_status = E_NO_ERROR;
    BOOL l_is_status_found = TRUE;

    if (m_is_color_settings_loaded) {
        CPTString  l_backgroundcolor;
        CString l_str_bkground(_T(""));
        int l_nodecnt = m_colorsettings_reader->GetNodeCnt();
        int listpos = 0;

        for (int l_index = 0; l_index < l_nodecnt; l_index++) {
            CPTString l_protocolstatus = m_colorsettings_reader->GetAttributeValue(l_index,
                                         NODE_TAG_NAME_STATUS_CONFIG,
                                         CHILD_TAG_NAME_NAME,
                                         ATTRIBUTE_NAME_1);

            if (!(l_is_status_found  = l_protocolstatus.CompareNoCase(f_protocol_status))) {
                listpos = m_colorsettings_reader->GetTagPosition(l_index,
                          NODE_TAG_NAME_STATUS_CONFIG,
                          CHILD_TAG_NAME_NAME,
                          ATTRIBUTE_NAME_1);

                l_backgroundcolor = m_colorsettings_reader->GetAttributeValue(NUM_1,
                                    NODE_TAG_NAME_COLOR_INFO,
                                    CHILD_TAG_NAME_BACKGROUND,
                                    ATTRIBUTE_NAME_1,
                                    (listpos + 1));


                l_str_bkground = m_colorsettings_reader->ReadStringValue(l_backgroundcolor);

                if (!l_str_bkground.GetLength()) {
                    l_status = INVALID_COLOR_VALUE;
                    l_backgroundcolor = L"000000";
                }

                if (!GetRGBValueFromString(l_str_bkground, f_backgroundcolor)) {
                    f_backgroundcolor = RGB(0, 0, 0);
                    l_status = INVALID_COLOR_VALUE;
                }

                break;
            }
        }

    } else {
        f_backgroundcolor = RGB(255, 0, 0);
        return INVALID_XML_FILE;
    }

    if (l_is_status_found) {
        f_backgroundcolor = RGB(0, 0, 0);
        l_status = INVALID_PROTOCOL_STATUS;
    }

    return l_status;
}

//********************************Method Header********************************
//Method Name   :GetForeGroundColor()
//Author        :PATNI/LK
//Purpose       :To Retrieve the foreground color for a given protocol status
//*****************************************************************************
int CPQMConfig::GetForeGroundColor(
    const CString& f_protocol_status,
    COLORREF& f_foregroundcolor
)
{
    int l_status = E_NO_ERROR;
    BOOL l_status_not_found = TRUE;

    if (m_is_color_settings_loaded) {

        int l_nodecnt = m_colorsettings_reader->GetNodeCnt();
        int listpos = 0;
        CString l_str_frground(_T(""));
        CPTString l_foregroundcolor;

        for (int l_index = 0; l_index < l_nodecnt; l_index++) {

            CPTString l_protocolstatus = m_colorsettings_reader->GetAttributeValue(l_index,
                                         NODE_TAG_NAME_STATUS_CONFIG,
                                         CHILD_TAG_NAME_NAME,
                                         ATTRIBUTE_NAME_1);

            if (!(l_status_not_found = l_protocolstatus.CompareNoCase(f_protocol_status))) {

                listpos = m_colorsettings_reader->GetTagPosition(l_index,
                          NODE_TAG_NAME_STATUS_CONFIG,
                          CHILD_TAG_NAME_NAME,
                          ATTRIBUTE_NAME_1);

                l_foregroundcolor = m_colorsettings_reader->GetAttributeValue(NUM_1,
                                    NODE_TAG_NAME_COLOR_INFO,
                                    CHILD_TAG_NAME_FOREGROUND,
                                    ATTRIBUTE_NAME_1,
                                    (listpos + 2)) ;

                l_str_frground = m_colorsettings_reader->ReadStringValue(l_foregroundcolor);

                if (l_str_frground.GetLength() != 6) {
                    l_status = INVALID_COLOR_VALUE;
                    l_str_frground = L"ffffff";
                }

                if (!GetRGBValueFromString(l_str_frground, f_foregroundcolor)) {
                    f_foregroundcolor = RGB(RGB_255, RGB_255, RGB_255);
                    l_status = INVALID_COLOR_VALUE;
                }

                break;
            }
        }

    } else {
        f_foregroundcolor = RGB(RGB_255, RGB_255, RGB_255);
        return INVALID_XML_FILE;
    }

    if (l_status_not_found) {

        f_foregroundcolor = RGB(RGB_255, RGB_255, RGB_255);
        l_status = INVALID_PROTOCOL_STATUS;
    }

    return l_status;
}

//********************************Method Header********************************
//Method Name   :GetTraceLogCSSDKValue()
//Author        :PATNI/LK
//Purpose       :Retrieves TraceLog CSSDK value by reading PQMApp_Config.xml file
//*****************************************************************************
BOOL CPQMConfig::GetTraceLogCSSDKValue(
    int& f_tracelog_cssdk_value
)const
{
    if (m_is_pqm_app_settings_loaded) {

        CPTString l_tracelog_cssdk_str = m_pqmappconfig_reader->GetAttributeValue(NUM_2,
                                         NODE_TAG_NAME_TRACELOG_VALUE,
                                         CHILD_TAG_NAME_USESDK,
                                         ATTRIBUTE_NAME_1);

        int l_tracelog_cssdk_value = m_pqmappconfig_reader->ReadLongValue(l_tracelog_cssdk_str);

        if (l_tracelog_cssdk_value == 0 || l_tracelog_cssdk_value == 1) {
            f_tracelog_cssdk_value = l_tracelog_cssdk_value;

        } else {
            f_tracelog_cssdk_value = 0;
        }

        return TRUE;

    } else {
        f_tracelog_cssdk_value = 0;
        return FALSE;
    }
}
//********************************Method Header********************************
//Method Name   :GetTraceLogPath()
//Author        :PATNI/LK
//Purpose       :Retrieves TraceLog path by reading PQMApp_Config.xml file
//*****************************************************************************
BOOL CPQMConfig::GetTraceLogPath(
    CString& f_tracelog_path
)const
{
    wchar_t l_buff[MAX_PATH] = {0};	//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections

    if (m_is_pqm_app_settings_loaded) {

        CPTString l_tracelog_path = m_pqmappconfig_reader->GetAttributeValue(NUM_2,
                                    NODE_TAG_NAME_TRACELOG_VALUE,
                                    CHILD_TAG_NAME_TRACELOGPATH,
                                    ATTRIBUTE_NAME_1);

        struct _stat l_tracelog_status_struct;

        if (_wstat(l_tracelog_path, &l_tracelog_status_struct)) {
            ::GetEnvironmentVariable(_T("MRMP_LOGDIR"), l_buff, MAX_PATH);
            f_tracelog_path  = l_buff;
            f_tracelog_path += _T("\\Trace");
            return INVALID_TRACE_PATH;

        } else {
            f_tracelog_path = l_tracelog_path;
            return TRUE;
        }

    } else {
        ::GetEnvironmentVariable(_T("MRMP_LOGDIR"), l_buff, MAX_PATH);
        f_tracelog_path = l_buff;
        f_tracelog_path += _T("\\Trace");
        return INVALID_XML_FILE;
    }
}
//********************************Method Header********************************
//Method Name   :GetEventLogCSSDKValue()
//Author        :PATNI/LK
//Purpose       :Retrieves EventLog CSSDK value by reading PQMApp_Config.xml file
//*****************************************************************************
BOOL CPQMConfig::GetEventLogCSSDKValue(
    int& f_eventlog_cssdk_value
)const
{
    if (m_is_pqm_app_settings_loaded) {

        CPTString l_eventlog_cssdk_str = m_pqmappconfig_reader->GetAttributeValue(NUM_3,
                                         NODE_TAG_NAME_EVENTLOG_VALUE,
                                         CHILD_TAG_NAME_USESDK,
                                         ATTRIBUTE_NAME_1);

        int l_eventlog_cssdk_value = m_pqmappconfig_reader->ReadLongValue(l_eventlog_cssdk_str);

        if (l_eventlog_cssdk_value == 0 || l_eventlog_cssdk_value == 1) {
            f_eventlog_cssdk_value = l_eventlog_cssdk_value;

        } else {
            f_eventlog_cssdk_value = 0;
        }

        return TRUE;

    } else {
        f_eventlog_cssdk_value = 0;
        return FALSE;
    }

}
//********************************Method Header********************************
//Method Name   :GetEventLogPath()
//Author        :PATNI/LK
//Purpose       :Retrieves EventLog path by reading PQMApp_Config.xml file
//*****************************************************************************
int CPQMConfig::GetEventLogPath(
    CString& f_eventlog_path
)const
{
    wchar_t l_tracebuff[MAX_PATH] = {0};	//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections

    if (m_is_pqm_app_settings_loaded) {

        CPTString l_eventlog_path = m_pqmappconfig_reader->GetAttributeValue(NUM_3,
                                    NODE_TAG_NAME_EVENTLOG_VALUE,
                                    CHILD_TAG_NAME_EVENTLOGPATH,
                                    ATTRIBUTE_NAME_1);

        struct _stat l_eventlog_status_struct;

        if (_wstat(l_eventlog_path, &l_eventlog_status_struct)) {
            ::GetEnvironmentVariable(_T("MRMP_LOGDIR"), l_tracebuff, MAX_PATH);
            f_eventlog_path = l_tracebuff;
            f_eventlog_path += _T("\\Error");

            return INVALID_EVENT_PATH;

        } else {
            f_eventlog_path = l_eventlog_path;
            return TRUE;
        }

    } else {
        ::GetEnvironmentVariable(_T("MRMP_LOGDIR"), l_tracebuff, MAX_PATH);
        f_eventlog_path = l_tracebuff;
        f_eventlog_path += _T("\\Error");
        return INVALID_XML_FILE;
    }
}

//+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
bool CPQMConfig::GetSFTMenuXMLData(
    VARIANT** f_scan_anatomy_data
)const
{
    if (m_sft_menu_parser) {
        m_sft_menu_parser->GetSFTMenuXMLData(f_scan_anatomy_data);
        return true;
    }

    return false;
}

bool CPQMConfig::GetToolMenuXMLData(
    VARIANT** f_scan_anatomy_data
)const
{
    if (m_tool_menu_parser) {
        m_tool_menu_parser->GetSFTMenuXMLData(f_scan_anatomy_data);
        return true;
    }

    return false;
}
//-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu

//+Patni-AJS/2010Sep3/Added/ScanCommentDialog/IR-163
//********************************Method Header********************************
//Method Name   :GetScanCommentXMLData()
//Author        : PATNI\AJS
//Purpose       :
//*****************************************************************************
bool CPQMConfig::GetScanCommentXMLData(
    VARIANT** f_scan_comment_data
)
{
    if (NULL == m_scan_comment_reader) {
        m_scan_comment_reader = new CScanCommentXMLReader();
    }

    if (m_scan_comment_reader && m_scan_comment_reader->GetScanCommentXMLData(f_scan_comment_data))	{
        return true;
    }

    CString l_error = CPqmUtilities::GetMultiLingualString(_T("IDS_SCANCOMMENT_CONFIG_FILE_MISSING"));
    CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
        l_error, MPlus::EH::Crucial, APP_NAME, APP_NAME);

    return false;
}
//-Patni-AJS/2010Sep3/Added/ScanCommentDialog/IR-163

//********************************Method Header********************************
//Method Name   :GetScanAnatomy()
//Author        :PATNI/Mangesh
//Purpose       :To Retrieve the Scan Anatomy data from config file
//*****************************************************************************
bool CPQMConfig::GetScanAnatomy(VARIANT** f_scan_anatomy_data)const
{
    //AfxMessageBox(L"testing");
    long l_max_saranatomy_limit = ReadMaxSarAnatomyLimitfromFile();
    long l_max_scananatomy_limit = ReadMaxScanAnatomyLimitfromFile();

    //CScanAnatomyReader obj_scan_anatomy_reader ;
    if (m_scan_anatomy_reader->GetScanAnatomyData(f_scan_anatomy_data, l_max_saranatomy_limit, l_max_scananatomy_limit))	{
        return true;
    }

    return false;

    /*int l_status =E_NO_ERROR;
    SScanAantomy l_scan_anatomy;
    CPTString l_scan_anatomy_name = L"";
    CPTString l_scan_anatomy_index = L"";


     CScanAnatomyData** l_scananatomy_data = ( (CScanAnatomyData**)(*f_scan_anatomy_data)->byref ) ;

      if(m_is_scan_anatomy_loaded) {


     	int l_nodecnt = m_scananatomy_reader->GetNodeCnt();
     	long l_max_saranatomy_limit = ReadMaxSarAnatomyLimitfromFile();
     	long l_max_scananatomy_limit = ReadMaxScanAnatomyLimitfromFile();

     	if(l_max_saranatomy_limit < l_nodecnt){
     	  //show error message
     		CPQMLogMgr::GetInstance()->DisplayScanError(IDS_MAX_SARANATOMY,
                     SCAN_ERR_CRUCIAL,
                     _T("pqmconfig.cpp"));
     		l_status =E_ERROR;

     		return l_status;
     	}

     	for(int l_index = 0; l_index < l_nodecnt; l_index++) {

     		CPTString l_sar_anatomy_name = m_scananatomy_reader->GetAttributeValue(l_index,
     											NODE_TAG_NAME_ANATOMY,
     											CHILD_TAG_NAME_SAR_ANATOMY,
     											ATTRIBUTE_NAME_1);

     		CPTString l_scan_anatomy_count = m_scananatomy_reader->GetAttributeValue(l_index,
     											NODE_TAG_NAME_ANATOMY,
     											NODE_TAG_NAME_SCAN_ANATOMY_COUNT,
     											ATTRIBUTE_NAME_1);


     		int l_sar_seq = GetSarAnatomyIndex(l_sar_anatomy_name);
     		if(l_sar_seq == -1) {

     			ASSERT(FALSE);
     			return -1;
     		}

     		int l_scananatomy_count = 0;

     		if(l_scan_anatomy_count){

     		  	l_scananatomy_count = m_scananatomy_reader->ReadLongValue(l_scan_anatomy_count);
     		}

     		int l_currentlistpos = m_scananatomy_reader->GetTagPosition(l_index,
     																	NODE_TAG_NAME_ANATOMY,
     																	NODE_TAG_NAME_SCAN_ANATOMY_COUNT,
     																	ATTRIBUTE_NAME_1);
     		for(int count = 0;count < l_scananatomy_count;count++){


     			l_scan_anatomy_name = m_scananatomy_reader->GetAttributeValue(NUM_1,
     																		NODE_TAG_NAME_SCAN_ANATOMY,
     																		CHILD_TAG_NAME_NAME,
     																		ATTRIBUTE_NAME_1,
     																		++l_currentlistpos);

     			l_scan_anatomy_index = m_scananatomy_reader->GetAttributeValue(NUM_1,
     																		NODE_TAG_NAME_SCAN_ANATOMY,
     																		CHILD_TAG_NAME_VFINDEX,
     																		ATTRIBUTE_NAME_1,
     																		++l_currentlistpos );
     			if(l_scan_anatomy_index){

     				l_scan_anatomy.m_scan_anatomy_vf = m_scananatomy_reader->ReadLongValue(l_scan_anatomy_index);
     			}
     			char* l_scananatomy_name = NULL;


     			if(l_scan_anatomy_name && l_scan_anatomy_index){
     				l_scananatomy_name = ReadStringValue(l_scan_anatomy_name);

     			}

     			if((l_scananatomy_name!=NULL)&& !(strcmp(l_scananatomy_name,"")==0)){
     				l_scan_anatomy.m_scan_anatomy_name = l_scananatomy_name;

     				l_scananatomy_data[l_sar_seq]->AddTail(l_scan_anatomy);
     			}

     		}//end for

     	} //end for

     } //end if
    */

}

//+Patni-Ajay/2010July23/Added/TMSC-REDMINE-443
//********************************Method Header********************************
//Method Name   :GetPresetItem()
//Author        :
//Purpose       :
//*****************************************************************************
bool CPQMConfig::GetPresetItem(VARIANT** f_preset_item_data
                              )const
{

    if (m_preset_item_reader->GetPresetItem(f_preset_item_data))	{
        return true;
    }

    return false;
}
//-Patni-Ajay/2010July23/Added/TMSC-REDMINE-443


//********************************Method Header********************************
//Method Name   :SetSkipRGN()
//Author        :PATNI/LK
//Purpose       :To set the value for SkipRGN
//*****************************************************************************
void CPQMConfig::SetSkipRGN(
    const bool set
)
{
    LPCTSTR FUNC_NAME = _T("CPQMConfig::SetSkipRGN");
    CString str_msg(_T(""));
    str_msg.Format(_T("%d"), set);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);
    m_skip_rgn = set;
}

//********************************Method Header********************************
//Method Name   :SetStudyUID()
//Author        :PATNI/LK
//Purpose       :Set Study UID file name in the ipcconfig.ini
//*****************************************************************************
void CPQMConfig::SetStudyUID(
    const CString& f_studyuid
)
{
    m_ipcconfig_reader->SetKey(f_studyuid, _T("Study Uid"), _T("STUDY"));
}

//********************************Method Header********************************
//Method Name   :CPQMConfig()
//Author        :PATNI/LK
//Purpose       :Constructor
//*****************************************************************************
CPQMConfig::CPQMConfig():
    m_nAuto_job_number(0),    //Patni-ARD/2009Oct28/Added/IR 85 Auto Transfer
    m_skip_rgn(false),
    m_rxgain_offset(0.0),
    m_JetChannel_V(0),
    m_sft_menu_parser(NULL),//+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
    m_tool_menu_parser(NULL),
    m_load_tool_menu(false)//-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
{
}

//********************************Method Header********************************
//Method Name   :InitializeConfig()
//Author        :PATNI/LK
//Purpose       :Initializes all the readers. it returns FALSE if it fails.
//*****************************************************************************
BOOL CPQMConfig::InitializeConfig(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMConfig::InitializeConfig");
    //+Patni-ARD/2010Mar10/Modified/Ph-3# IR-100 code review

    //m_ipcconfig_reader = new CIniReader(CPQMConfig::GetIPCConfigFileName());
    m_ipcconfig_reader = new CIniReader(CPqmUtilities::GetIPCConfigFileName());

    //+Patni-ARD/2010Feb23/Modified/PQM crash
    try {
        // Check for file/folder is exist or not !
        //if (!m_ipcconfig_reader || E_ERROR_STAT == _wstat(CPQMConfig::GetIPCConfigFileName(), &buf)) {
        struct _stat buf;

        if (!m_ipcconfig_reader || E_ERROR_STAT == _wstat(CPqmUtilities::GetIPCConfigFileName(), &buf)) {
            return FALSE;
        }

        // Check for file/folder is exist or not !
        //m_map_reader = new CIniReader(CPQMConfig::GetMapFileName());
        m_map_reader = new CIniReader(CPqmUtilities::GetMapFileName());

        //if (!m_map_reader || E_ERROR_STAT == _wstat(CPQMConfig::GetIPCConfigFileName(), &buf)) {
        if (!m_map_reader || E_ERROR_STAT == _wstat(CPqmUtilities::GetIPCConfigFileName(), &buf)) {
            return FALSE;
        }

        m_colorsettings_reader = new CPQMXMLParser();
        //m_is_color_settings_loaded = m_colorsettings_reader->LoadXMLFile(CPQMConfig::GetColorSettingsFileName());
        m_is_color_settings_loaded = m_colorsettings_reader->LoadXMLFile(CPqmUtilities::GetColorSettingsFileName());

        m_pqmappconfig_reader = new CPQMXMLParser();
        //m_is_pqm_app_settings_loaded = m_pqmappconfig_reader->LoadXMLFile(CPQMConfig::GetPqmAppConfigFileName());
        m_is_pqm_app_settings_loaded = m_pqmappconfig_reader->LoadXMLFile(CPqmUtilities::GetPqmAppConfigFileName());

        //+Patni-ARD/2010Apr08/Commented/JaFT# IR-100 code review
        //SM3_Mangesh_Start
        //m_contrastagents_reader = new CPQMXMLParser();
        //m_is_contrast_agents_loaded = m_contrastagents_reader->LoadXMLFile(CPQMConfig::GetContrastAgentsFileName());
        //m_is_contrast_agents_loaded = m_contrastagents_reader->LoadXMLFile(CPqmUtilities::GetContrastAgentsFileName());
        //-Patni-ARD/2010Apr08/Commented/JaFT# IR-100 code review

        //m_scananatomy_reader = new CPQMXMLParser();
        //m_is_scan_anatomy_loaded = m_scananatomy_reader->LoadXMLFile(CPQMConfig::GetScanAnatomyFileName());

        m_scan_anatomy_reader = new CScanAnatomyReader();
        //m_is_scan_anatomy_loaded = 	m_scan_anatomy_reader->LoadXMLFile(CPQMConfig::GetScanAnatomyFileName());
        m_is_scan_anatomy_loaded = 	m_scan_anatomy_reader->LoadXMLFile(CPqmUtilities::GetScanAnatomyFileName());

        //+Patni-Ajay/2010July23/Added/TMSC-REDMINE-443
        m_preset_item_reader = new CPlanDuplicateXMLReader();

        if (FALSE == m_preset_item_reader->LoadXMLFile(CPqmUtilities::GetPresetItemFileName())) {

            CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
                CPqmUtilities::GetMultiLingualString(_T("IDS_PLAN_DUPLICATOR_CONFIG_MISSING")),
                MPlus::EH::Crucial, APP_NAME, APP_NAME);
        }

        //-Patni-Ajay/2010July23/Added/TMSC-REDMINE-443

        //SM3_Mangesh_End

        //+Patni-ARD/2009Oct28/Added/IR 85 Auto Transfer
        m_pqm_auto_transfer_reader = new CPQMXMLParser();

        if (m_pqm_auto_transfer_reader) {
            //m_is_auto_transfer_reader = m_pqm_auto_transfer_reader->LoadXMLFile(CPQMConfig::GetScanAppSiteFileName());
            m_is_auto_transfer_reader = m_pqm_auto_transfer_reader->LoadXMLFile(CPqmUtilities::GetScanAppSiteFileName());
        }

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Unknown exception occured"));

        return FALSE;
    }

    //-Patni-ARD/2010Feb23/Modified/PQM crash
    //-Patni-ARD/2009Oct28/Added/IR 85 Auto Transfer
    //-Patni-ARD/2010Mar10/Modified/Ph-3# IR-100 code review

    return TRUE;
}

//************************************Method Header**************************************
//Method Name:  ConvertHexStringToInt()
//Author:  PATNI/MSN
//Purpose: Convert HexString To Integer value
//**************************************************************************************/
int CPQMConfig::ConvertHexStringToInt(
    LPCSTR str
)const
{
    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (str == NULL) {
        return -1;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    int sum = 0;
    int base = 0;

    for (int i = strlen(str) - 1; i >= 0; i--) {
        switch (str[i]) {
            case 'a':
                base = BASE_10;
                break;
            case 'b':
                base = BASE_11;
                break;
            case 'c':
                base = BASE_12;
                break;
            case 'd':
                base = BASE_13;
                break;
            case 'e':
                base = BASE_14;
                break;
            case 'f':
                base = BASE_15;
                break;
            default:
                char buff[NUM_2] = {0};	//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
                buff[0] = str[i];

                if ((buff[0] >= '0') && (buff[0] <= '9')) {
                    base = atoi(buff);
                }

                break;
        }

        sum = sum + base * (int)pow(NUM_16, i);
    }

    return (sum);
}

//********************************Method Header*****************************************
//Method Name   :ReadMaxContrastAgentsLimitfromFile()
//Author        :PATNI/Mangesh
//Purpose       :To Retrieve the MAX Contrast Agents Limit from PQMApp_Config.xml file
//**************************************************************************************
long CPQMConfig::ReadMaxContrastAgentsLimitfromFile(
)const
{
    if (m_is_pqm_app_settings_loaded) {

        CPTString l_max_contrast_agents = m_pqmappconfig_reader->GetAttributeValue(NUM_4,
                                          NODE_TAG_NAME_CONTRASTAGENTS_VALUE,
                                          CHILD_TAG_NAME_MAXLIMIT,
                                          ATTRIBUTE_NAME_1);
        return m_pqmappconfig_reader->ReadLongValue(l_max_contrast_agents);

    } else {
        return MAXCONTRAST_DEFAULT;
    }
}

//********************************Method Header********************************
//Method Name   :ReadMaxScanAnatomyLimitfromFile()
//Author        :PATNI/Mangesh
//Purpose       :To Retrieve the MAX Scan Anatomy Limit from PQMApp_Config.xml file
//*****************************************************************************
long CPQMConfig::ReadMaxSarAnatomyLimitfromFile(
)const
{
    if (m_is_pqm_app_settings_loaded) {

        CPTString l_max_sar_anatomy = m_pqmappconfig_reader->GetAttributeValue(NUM_5,
                                      NODE_TAG_NAME_SCANANATOMY_VALUE,
                                      CHILD_TAG_NAME_SARANATOMY_MAXLIMIT,
                                      ATTRIBUTE_NAME_1);
        return m_pqmappconfig_reader->ReadLongValue(l_max_sar_anatomy);

    } else {
        return MAXSARANATOMY_DEFAULT;
    }
}

//********************************Method Header********************************
//Method Name   :ReadMaxScanAnatomyLimitfromFile()
//Author        :PATNI/Mangesh
//Purpose       :To Retrieve the MAX Scan Anatomy Limit from PQMApp_Config.xml file
//*****************************************************************************
long CPQMConfig::ReadMaxScanAnatomyLimitfromFile(
)const
{
    if (m_is_pqm_app_settings_loaded) {

        CPTString l_max_scan_anatomy = m_pqmappconfig_reader->GetAttributeValue(NUM_5,
                                       NODE_TAG_NAME_SCANANATOMY_VALUE,
                                       CHILD_TAG_NAME_SCANANATOMY_MAXLIMIT,
                                       ATTRIBUTE_NAME_1);
        return m_pqmappconfig_reader->ReadLongValue(l_max_scan_anatomy);

    } else {
        return MAXSCANANATOMY_DEFAULT;
    }
}

//************************************Method Header**************************************
//Method Name:  GetRGBValueFromString()
//Author:  PATNI/MSN
//Purpose: Get RGB value from the Hex string
//**************************************************************************************/
BOOL CPQMConfig::GetRGBValueFromString(
    const CString& f_rgbstr,
    COLORREF& f_rgbcolor
)const
{
    if (f_rgbstr.GetLength() == NUM_6) {
        CString rstr = f_rgbstr.Mid(0, NUM_2);
        CString gstr = f_rgbstr.Mid(NUM_2, NUM_2);
        CString bstr = f_rgbstr.Mid(NUM_4, NUM_2);

        //+Patni-ARD/2010Mar10/Modified/Ph-3# IR-100 code review
        LPCSTR red = CPqmUtilities::ReadStringValue(rstr.GetBuffer(NUM_2));
        LPCSTR green = CPqmUtilities::ReadStringValue(gstr.GetBuffer(NUM_2));
        LPCSTR blue = CPqmUtilities::ReadStringValue(bstr.GetBuffer(NUM_2));
        //-Patni-ARD/2010Mar10/Modified/Ph-3# IR-100 code review

        int intred = ConvertHexStringToInt(red);
        int intgreen = ConvertHexStringToInt(green);
        int intblue = ConvertHexStringToInt(blue);

        if ((intred < 0) || (intgreen < 0) || (intblue < 0)) {
            return FALSE;
        }

        f_rgbcolor = RGB(intred, intgreen, intblue);
        return TRUE;

    } else {
        return FALSE;
    }
}

//+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
CPTString CPQMConfig::GetSFTMenuFileName(
) const
{

    CPTString l_file_name;
    wchar_t l_buffer[MAX_PATH] = {0};	//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    ::GetEnvironmentVariable(_T("MRMP_Config"), l_buffer, MAX_PATH);
    l_file_name = l_buffer;
    l_file_name += L"\\";

    //Patni-Hemant/2010July22/Modified/IR-152-Part 3
    l_file_name += L"PQM\\";

    l_file_name += _T("PQM_SFTMenu.xml");

    return l_file_name;
}

CPTString CPQMConfig::GetToolMenuFileName(
) const
{

    CPTString l_file_name;
    wchar_t l_buffer[MAX_PATH] = {0};	//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    ::GetEnvironmentVariable(_T("MRMP_Config"), l_buffer, MAX_PATH);
    l_file_name = l_buffer;
    l_file_name += L"\\";

    //Patni-Hemant/2010July22/Modified/IR-152-Part 3
    l_file_name += L"PQM\\";

    l_file_name += _T("PQM_ToolMenu.xml");

    return l_file_name;
}
//-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu

















//********************************Method Header********************************
//Method Name   :GetIPCConfigFileName()
//Author        :PATNI/MRP
//Purpose       :Get the config file name
//*****************************************************************************
//+Patni-ARD/2010Mar10/Deleted/Ph-3# IR-100 code review
/*
CString CPQMConfig::GetIPCConfigFileName(
)
{
    CString l_file_name;
    wchar_t l_buffer[MAX_PATH] = {0};	//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    ::GetEnvironmentVariable(_T("MRMP_Config"), l_buffer, MAX_PATH);
    l_file_name = l_buffer;
    l_file_name += L"\\";
    l_file_name += L"PQM\\IPCCONFIG.INI";
    return l_file_name;
}
*/
//********************************Method Header********************************
//Method Name   :GetMapFileName()
//Author        :PATNI/HAR
//Purpose       :Get the map file name
//*****************************************************************************
/*
CString CPQMConfig::GetMapFileName(
)
{
    CString l_file_name;
    wchar_t l_buffer[MAX_PATH] = {0};	//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    ::GetEnvironmentVariable(_T("MRMP_Config"), l_buffer, MAX_PATH);
    l_file_name = l_buffer;
    l_file_name += L"\\";
    l_file_name += L"MEMORY\\MAP.INI";

    return l_file_name;
}
*/
//-Patni-ARD/2010Mar10/Commented/Ph-3# IR-100 code review

//********************************Method Header********************************
//Method Name   :GetStudyUID()
//Author        :PATNI/LK
//Purpose       :Get Study UID file name
//*****************************************************************************
//+Patni-AMT/2010Mar10/Modified/Ph-3#IR-100
/*
CString CPQMConfig::GetStudyUID(
)
{
    if (m_ipcconfig_reader->SectionExists(_T("Study"))) {
        return m_ipcconfig_reader->GetKeyValue(_T("Study Uid"), _T("Study"));

    } else {
        CString l_err_str(_T(""));
        // l_err_str.LoadString(IDS_STUDY_NOT_FOUND);  / Added By KT/ACE-2/15-06-2009/Multilingualisation

        CPQMLogMgr::GetInstance()->WriteEventLog(l_err_str,
                ERROR_LOG_CRUCIAL1 , _T("PQMConfig.cpp"));

        CString temp_string = _T("");													// Added By KT/ACE-2/15-06-2009/Multilingualisation
        temp_string = CPqmUtilities::GetMultiLingualString(_T("IDS_STUDY_NOT_FOUND"));  // Added By KT/ACE-2/15-06-2009/Multilingualisation

        CPQMLogMgr::GetInstance()->DisplayError(temp_string,  // Added By KT/ACE-2/15-06-2009/Multilingualisation
                                                _T("PQMConfig.cpp"), __LINE__, ERROR_LOG_CRUCIAL1 ,
                                                ERR_ID, APP_NAME);

        return _T("");
    }
}
*/
//-Patni-AMT/2010Mar10/Modified/Ph-3#IR-100
//********************************Method Header********************************
//Method Name   :GetStudyPath()
//Author        :PATNI/LK
//Purpose       :Get Study Path file name from IPCConfig.ini
//*****************************************************************************
//+Patni-ARD/2010Mar10/Commented/Ph-3# IR-100 code review
/*
CString CPQMConfig::GetStudyPath(
)
{

    /*if ( m_ipcconfig_reader->SectionExists(_T("Study")) ) {
        return m_ipcconfig_reader->GetKeyValue(_T("Study Path"),_T("Study"));
    } else {
        CString l_err_str(_T(""));
        l_err_str.LoadString(IDS_STUDY_NOT_FOUND);

        CPQMLogMgr::GetInstance()->WriteEventLog(l_err_str,
            ERROR_LOG_CRUCIAL1 ,_T("PQMConfig.cpp"));

        CPQMLogMgr::GetInstance()->DisplayError(l_err_str,
            _T("PQMConfig.cpp"),__LINE__,ERROR_LOG_CRUCIAL1 ,
            ERR_ID,APP_NAME);

        return _T("");
    }*/
/*
    //Nitin changes ipcconfig.ini dependency
    CString l_file_name;
    wchar_t l_buffer[MAX_PATH] = {0};	//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    ::GetEnvironmentVariable(_T("SITE"), l_buffer, MAX_PATH);
    l_file_name = l_buffer;
    l_file_name += L"/";
    l_file_name += L"study/";

    return l_file_name;
}
*/

//********************************Method Header********************************
//Method Name   :SetStudyPath()
//Author        :PATNI/LK
//Purpose       :Set Study Path file name in the IPCConfig.ini
//*****************************************************************************
/*
void CPQMConfig::SetStudyPath(
    CString  f_study_path
)
{
    //  m_ipcconfig_reader->SetKey(f_study_path,_T("Study Path"),_T("STUDY"));
}
*/
//********************************Method Header********************************
//Method Name   :GetIRSPath()
//Author        :PATNI/LK
//Purpose       :Get IRS Path from IPCConfig.ini
//*****************************************************************************
/*
CString CPQMConfig::GetIRSPath(
)
{
    if (m_ipcconfig_reader->SectionExists(_T("SOCKET"))) {
        return m_ipcconfig_reader->GetKeyValue(_T("Irs Path"), _T("SOCKET"));

    } else {
        CString l_err_str(_T(""));
        //  l_err_str.LoadString(IDS_STUDY_NOT_FOUND); // Added By KT/ACE-2/15-06-2009/Multilingualisation

        CPQMLogMgr::GetInstance()->WriteEventLog(IDS_NO_READ_VALUE,
                ERROR_LOG_CRUCIAL1,
                _T("PQMConfig.cpp"));

        l_err_str = CPqmUtilities::GetMultiLingualString(_T("IDS_STUDY_NOT_FOUND"));	// Added By KT/ACE-2/15-06-2009/Multilingualisation												 // Added By KT/ACE-2/15-06-2009/Multilingualisation

        CPQMLogMgr::GetInstance()->DisplayError(l_err_str,
                                                _T("PQMConfig.cpp"), __LINE__, ERROR_LOG_CRUCIAL1 ,
                                                ERR_ID, APP_NAME);

        return _T("");
    }
}
*/

//********************************Method Header********************************
//Method Name   :GetIRSNotifierPath()
//Author        :PATNI/LK
//Purpose       :Get IRS Notifier Path from IPCConfig.ini
//*****************************************************************************
/*
CString CPQMConfig::GetIRSNotifierPath(
)
{
    if (m_ipcconfig_reader->SectionExists(_T("SOCKET"))) {
        return m_ipcconfig_reader->GetKeyValue(_T("IrsNotifier Path"), _T("SOCKET"));

    } else {
        CString l_err_str(_T(""));
        //   l_err_str.LoadString(IDS_STUDY_NOT_FOUND); // Added By KT/ACE-2/15-06-2009/Multilingualisation

        CPQMLogMgr::GetInstance()->WriteEventLog(IDS_NO_READ_VALUE,
                ERROR_LOG_CRUCIAL1,
                _T("PQMConfig.cpp"));

        l_err_str = CPqmUtilities::GetMultiLingualString(_T("IDS_STUDY_NOT_FOUND"));     // Added By KT/ACE-2/15-06-2009/Multilingualisation

        CPQMLogMgr::GetInstance()->DisplayError(l_err_str,
                                                _T("PQMConfig.cpp"), __LINE__, ERROR_LOG_CRUCIAL1 ,
                                                ERR_ID, APP_NAME);

        return _T("");
    }
}
*/
//-Patni-ARD/2010Mar10/Commented/Ph-3# IR-100 code review


//********************************Method Header********************************
//Method Name   :GetAcqManIP()
//Author        :PATNI/LK
//Purpose       :Get AcqMan IP address from IPCConfig.ini
//*****************************************************************************
//+Patni-ARD/2010Mar24/Commented/Ph-3# IR-100
/*
CString CPQMConfig::GetAcqManIP(
)
{
    if (m_ipcconfig_reader->SectionExists(_T("SOCKET"))) {
        return m_ipcconfig_reader->GetKeyValue(_T("Acqman Ip"), _T("SOCKET"));

    } else {
        CString l_err_str(_T(""));
        //  l_err_str.LoadString(IDS_STUDY_NOT_FOUND);  // Added By KT/ACE-2/15-06-2009/Multilingualisation

        CPQMLogMgr::GetInstance()->WriteEventLog(IDS_NO_READ_VALUE,
                ERROR_LOG_CRUCIAL1,
                _T("PQMConfig.cpp"));

        l_err_str = CPqmUtilities::GetMultiLingualString(_T("IDS_STUDY_NOT_FOUND"));   // Added By KT/ACE-2/15-06-2009/Multilingualisation

        CPQMLogMgr::GetInstance()->DisplayError(l_err_str,
                                                _T("PQMConfig.cpp"), __LINE__, ERROR_LOG_CRUCIAL1 ,
                                                ERR_ID, APP_NAME);

        return _T("");
    }
}
*/
//-Patni-ARD/2010Mar24/Commented/Ph-3# IR-100

//********************************Method Header********************************
//Method Name   :GetAcqManPort()
//Author        :PATNI/LK
//Purpose       :Get AcqMan IP address from IPCConfig.ini
//*****************************************************************************
//+Patni-ARD/2010Mar24/Commented/Ph-3# IR-100
/*
CString CPQMConfig::GetAcqManPort(
)
{
    if (m_ipcconfig_reader->SectionExists(_T("SOCKET"))) {
        return  m_ipcconfig_reader->GetKeyValue(_T("Acqman Port"), _T("SOCKET"));

    } else {
        CString l_err_str(_T(""));
        //  l_err_str.LoadString(IDS_STUDY_NOT_FOUND);

        CPQMLogMgr::GetInstance()->WriteEventLog(IDS_NO_READ_VALUE,
                ERROR_LOG_CRUCIAL1,
                _T("PQMConfig.cpp"));

        l_err_str = CPqmUtilities::GetMultiLingualString(_T("IDS_STUDY_NOT_FOUND"));  // Added By KT/ACE-2/15-06-2009/Multilingualisation

        CPQMLogMgr::GetInstance()->DisplayError(l_err_str,
                                                _T("PQMConfig.cpp"), __LINE__, ERROR_LOG_CRUCIAL1 ,
                                                ERR_ID, APP_NAME);

        return _T("");
    }
}
*/
//-Patni-ARD/2010Mar24/Commented/Ph-3# IR-100


//********************************Method Header********************************
//Method Name   :GetVoiceMgrIP()
//Author        :PATNI/LK
//Purpose       :Get Voice Manager IP address from IPCConfig.ini
//*****************************************************************************
//+Patni-ARD/2010Mar24/Commented/Ph-3# IR-100
/*
CString CPQMConfig::GetVoiceMgrIP(
)
{
    if (m_ipcconfig_reader->SectionExists(_T("SOCKET"))) {
        return m_ipcconfig_reader->GetKeyValue(_T("VoiceMgr IP"), _T("SOCKET"));

    } else {
        CString l_err_str(_T(""));
        // l_err_str.LoadString(IDS_STUDY_NOT_FOUND);

        CPQMLogMgr::GetInstance()->WriteEventLog(IDS_NO_READ_VALUE,
                ERROR_LOG_CRUCIAL1,
                _T("PQMConfig.cpp"));

        l_err_str = CPqmUtilities::GetMultiLingualString(_T("IDS_STUDY_NOT_FOUND"));  // Added By KT/ACE-2/15-06-2009/Multilingualisation

        CPQMLogMgr::GetInstance()->DisplayError(l_err_str,
                                                _T("PQMConfig.cpp"), __LINE__, ERROR_LOG_CRUCIAL1 ,
                                                ERR_ID, APP_NAME);

        return _T("");
    }
}
*/


//********************************Method Header********************************
//Method Name   :GetVoiceMgrPort()
//Author        :PATNI/LK
//Purpose       :Get Voice Manager Port from IPCConfig.ini
//*****************************************************************************
/*
CString CPQMConfig::GetVoiceMgrPort(
)
{
    if (m_ipcconfig_reader->SectionExists(_T("SOCKET"))) {
        return m_ipcconfig_reader->GetKeyValue(_T("VoiceMgr Port"), _T("SOCKET"));

    } else {
        CString l_err_str(_T(""));
        // l_err_str.LoadString(IDS_STUDY_NOT_FOUND);

        CPQMLogMgr::GetInstance()->WriteEventLog(IDS_NO_READ_VALUE,
                ERROR_LOG_CRUCIAL1,
                _T("PQMConfig.cpp"));

        l_err_str = CPqmUtilities::GetMultiLingualString(_T("IDS_STUDY_NOT_FOUND"));  // Added By KT/ACE-2/15-06-2009/Multilingualisation

        CPQMLogMgr::GetInstance()->DisplayError(l_err_str,
                                                _T("PQMConfig.cpp"), __LINE__, ERROR_LOG_CRUCIAL1 ,
                                                ERR_ID, APP_NAME);

        return _T("");
    }
}
*/
//-Patni-ARD/2010Mar24/Commented/Ph-3# IR-100

//********************************Method Header********************************
//Method Name   :GetSystemName()
//Author        :PATNI/LK
//Purpose       :Get System Name from IPCConfig.ini
//*****************************************************************************
//+Patni-ARD/2010Mar3/Commented/DeFT# IR-100 remove IPCConfig.ini
/*
CString CPQMConfig::GetSystemName(
)
{
    if (m_ipcconfig_reader->SectionExists(_T("Study"))) {
        return  m_ipcconfig_reader->GetKeyValue(_T("System Name"), _T("Study"));

    } else {
        CString l_err_str(_T(""));
        //  l_err_str.LoadString(IDS_STUDY_NOT_FOUND);

        CPQMLogMgr::GetInstance()->WriteEventLog(IDS_NO_READ_VALUE,
                ERROR_LOG_CRUCIAL1,
                _T("PQMConfig.cpp"));

        l_err_str = CPqmUtilities::GetMultiLingualString(_T("IDS_STUDY_NOT_FOUND"));  // Added By KT/ACE-2/15-06-2009/Multilingualisation

        CPQMLogMgr::GetInstance()->DisplayError(l_err_str,
                                                _T("PQMConfig.cpp"), __LINE__, ERROR_LOG_CRUCIAL1 ,
                                                ERR_ID, APP_NAME);

        return _T("");
    }
}
*/
//-Patni-ARD/2010Mar3/Commented/DeFT# IR-100 remove IPCConfig.ini

//********************************Method Header********************************
//Method Name   :GetPrepStudy()
//Author        :PATNI/LK
//Purpose       :Get Prep Study Name from IPCConfig.ini
//*****************************************************************************
//+Patni-ARD/2010Mar10/Commented/Ph-3# IR-100 code review
/*
CString CPQMConfig::GetPrepStudy(
)
{
    if (m_ipcconfig_reader->SectionExists(_T("Study"))) {
        return m_ipcconfig_reader->GetKeyValue(_T("PrepStudy"), _T("Study"));

    } else {
        CString l_err_str(_T(""));
        //  l_err_str.LoadString(IDS_STUDY_NOT_FOUND);

        CPQMLogMgr::GetInstance()->WriteEventLog(IDS_NO_READ_VALUE,
                ERROR_LOG_CRUCIAL1,
                _T("PQMConfig.cpp"));

        l_err_str = CPqmUtilities::GetMultiLingualString(_T("IDS_STUDY_NOT_FOUND"));  // Added By KT/ACE-2/15-06-2009/Multilingualisation

        CPQMLogMgr::GetInstance()->DisplayError(l_err_str,
                                                _T("PQMConfig.cpp"), __LINE__, ERROR_LOG_CRUCIAL1 ,
                                                ERR_ID, APP_NAME);

        return _T("");
    }
}
*/
//-Patni-ARD/2010Mar10/Commented/Ph-3# IR-100 code review
/*

//********************************Method Header********************************
//Method Name   :GetDataBaseSelection()
//Author        :PATNI/LK
//Purpose       :Get Data Base Selection from Map.ini
//*****************************************************************************
CString CPQMConfig::GetDataBaseSelection(
)
{
    if (m_map_reader->SectionExists(_T("DATABASE_SELECTION"))) {
        return m_map_reader->GetKeyValue(_T("DB"), _T("DATABASE_SELECTION"));

    } else {
        CString l_err_str(_T(""));
        // l_err_str.LoadString(IDS_STUDY_NOT_FOUND);

        //+Patni-PJS/2009May10/Modify/IR-82 review comment
        CString string = _T("");
        string.LoadString(IDS_NO_READ_VALUE);
        CPQMLogMgr::GetInstance()->WriteEventLog(string,
                ERROR_LOG_CRUCIAL1,
                _T("PQMConfig.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82


        //CPQMLogMgr::GetInstance()->WriteEventLog(IDS_NO_READ_VALUE,
        //        ERROR_LOG_CRUCIAL1,
        //        _T("PQMConfig.cpp"));
        //
        //-Patni-PJS/2009May10/Modify/IR-82 review comment

        l_err_str = CPqmUtilities::GetMultiLingualString(_T("IDS_STUDY_NOT_FOUND"));  // Added By KT/ACE-2/15-06-2009/Multilingualisation

        CPQMLogMgr::GetInstance()->DisplayError(l_err_str,
                                                _T("PQMConfig.cpp"), __LINE__, ERROR_LOG_CRUCIAL1 ,
                                                ERR_ID, APP_NAME);

        return _T("");
    }
}
*/

//********************************Method Header********************************
//Method Name   :GetColorSettingsFileName()
//Author        :PATNI/LK
//Purpose       :Get PQMAPP_colorsettings file path
//*****************************************************************************
//+Patni-ARD/2010Mar10/Commented/Ph-3# IR-100 code review
/*
CPTString CPQMConfig::GetColorSettingsFileName(
)
{
    CPTString l_file_name;
    wchar_t l_buffer[MAX_PATH] = {0};	//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    ::GetEnvironmentVariable(_T("MRMP_Config"), l_buffer, MAX_PATH);
    l_file_name = l_buffer;
    l_file_name += L"\\";
    l_file_name += L"PQM\\";
    l_file_name += COLOR_SETTING_FILENAME;

    return l_file_name;

}
*/

//********************************Method Header********************************
//Method Name   :GetPqmAppConfigFileName()
//Author        :PATNI/LK
//Purpose       :Get PQMAPP_config file path
//*****************************************************************************
/*
CPTString CPQMConfig::GetPqmAppConfigFileName(
)
{
    CPTString l_file_name;

    wchar_t l_buff[MAX_PATH] = {0};	//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    ::GetEnvironmentVariable(_T("MRMP_Config"), l_buff, MAX_PATH);
    l_file_name = l_buff;
    l_file_name += L"\\";
    l_file_name += L"PQM\\";
    l_file_name += APPCONFIG_FILENAME;

    return l_file_name;
}
*/
//-Patni-ARD/2010Mar10/Commented/Ph-3# IR-100 code review



//************************************Method Header**************************************
//Method Name:  ReadStringValue()
//Author:  PATNI/LK
//Purpose: Converts CPTString to char*
//**************************************************************************************/
//+Patni-ARD/2010Mar10/Commented/Ph-3# IR-100 code review
/*
char* CPQMConfig::ReadStringValue(
    CPTString f_string
)
{
    CPQMLogMgr::GetInstance()->WriteTraceLog(DOMAIN_NAME,
            USER_FUNC_MARKER,
            _T("CPQMConfig::ReadStringValue"),
            _T("Trace of ReadStringValue"), true);
    //+Patni-HAR/2009Aug31/Modified/Corrected call for wcstombs()
    int n = f_string.GetLength() + 1;
    char* char_string = new char[n];
    memset(char_string, 0, sizeof(char) *(n));
    //+Patni-HAR/2009Aug31/Modified/Corrected call for wcstombs()
    wcstombs(char_string, f_string, n);
    //char_string[n] = '\0';
    return char_string;
}
*/
//-Patni-ARD/2010Mar10/Commented/Ph-3# IR-100 code review
//********************************Method Header********************************

//Method Name   :GetSeriesBiasValue()

//Author        :PATNI/MRP

//Purpose       :Get the map file name

//*****************************************************************************
//+Patni-ARD/2010Mar3/Commented/DeFT# IR-100 remove IPCConfig.ini
/*
int CPQMConfig::GetSeriesBiasValue(
)
{
    int l_seriesbias = 0;
    CString l_seriesBiasString;

    if (m_ipcconfig_reader->SectionExists(_T("Study"))) {
        l_seriesBiasString = m_ipcconfig_reader->GetKeyValue(_T("Series Bias"), _T("Study"));
        l_seriesbias = _wtoi(l_seriesBiasString);

    } else {

        l_seriesbias = 1000;
    }

    return l_seriesbias;
}
*/
//-Patni-ARD/2010Mar3/Commented/DeFT# IR-100 remove IPCConfig.ini

//SM3_Mangesh_Start
//********************************Method Header********************************

//Method Name   :GetContrastAgentsFileName()

//Author        :PATNI/Mangesh

//Purpose       :Get the Contrast Agent's file name

//*****************************************************************************
//+Patni-ARD/2010Mar10/Commented/Ph-3# IR-100 code review
/*
CPTString CPQMConfig::GetContrastAgentsFileName(
)
{
    CPTString l_file_name = L"";
    wchar_t l_buffer[MAX_PATH] = {0}; //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    ::GetEnvironmentVariable(_T("MRMP_Config"), l_buffer, MAX_PATH);
    l_file_name = l_buffer;
    l_file_name += L"\\";
    l_file_name += L"PQM\\";
    l_file_name += CONTRAST_AGENTS_FILENAME;

    return l_file_name;
}
*/
//********************************Method Header********************************

//Method Name   :GetScanAnatomyFileName()

//Author        :PATNI/Mangesh

//Purpose       :Get the Scan Anatomy file name

//*****************************************************************************
/*
CPTString CPQMConfig::GetScanAnatomyFileName(
)
{
    CPTString l_file_name;
    wchar_t l_buffer[MAX_PATH] = {0};	//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    ::GetEnvironmentVariable(_T("MRMP_Config"), l_buffer, MAX_PATH);
    l_file_name = l_buffer;
    l_file_name += L"\\";
    l_file_name += L"PQM\\";
    l_file_name += SCAN_ANATOMY_FILENAME;

    return l_file_name;
}
*/
//-Patni-ARD/2010Mar10/Commented/Ph-3# IR-100 code review
//********************************Method Header********************************
//Method Name   :GetContrastAgent()

//Author        :PATNI/Mangesh

//Purpose       :To Retrieve the Contrast Agent data from the config file
//*****************************************************************************
//+Patni-ARD/2010Apr08/Commented/JaFT# IR-100 code review
/*
int CPQMConfig::GetContrastAgent(VARIANT** f_contrast_agent_data
                                )
{

    int l_status = E_NO_ERROR;
    CPTString l_contrast_agent_name = L"";
    CPTString l_contrast_agent_index = L"";
    SContrastAgent l_contrastagent;
    CContrastAgentData* l_contrast_agent_data = NULL;

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (f_contrast_agent_data == NULL) {
        CPQMLogMgr::GetInstance()->WriteTraceLog(DOMAIN_NAME,
                USER_FUNC_MARKER,
                _T("CPQMConfig::GetContrastAgent"),
                _T("Contrast Agent data pointer is NULL"), true);
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    l_contrast_agent_data = (CContrastAgentData*)(*f_contrast_agent_data)->byref;

    if (m_is_contrast_agents_loaded) {

        int l_nodecnt = m_contrastagents_reader->GetNodeCnt();
        long l_max_contrastagent_limit = ReadMaxContrastAgentsLimitfromFile();

        if (l_max_contrastagent_limit < l_nodecnt) {
            //show error message and use default value Gadolieum
            //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
            CPQMLogMgr::GetInstance()->DisplayScanError(IDS_MAX_CONTRASTAGENTS,
                    _T("IDS_MAX_CONTRASTAGENTS"),
                    SCAN_ERR_NOTIFY, _T("pqmconfig.cpp"));

            l_contrastagent.m_strname = DEFAULT_CONTRASTAGENT;
            l_contrastagent.m_index = m_contrastagents_reader->ReadLongValue(DEFAULT_CONTRASTAGENTINDEX);
            l_contrast_agent_data->AddTail(l_contrastagent);
            return l_status;
        }

        for (int l_index = 0; l_index < l_nodecnt; l_index++) {

            l_contrast_agent_name = m_contrastagents_reader->GetAttributeValue(l_index,
                                    NODE_TAG_NAME_CONTRAST_AGENT,
                                    CHILD_TAG_NAME_NAME,
                                    ATTRIBUTE_NAME_1);

            UTIL::Pstr2Wcs(&l_contrastagent.m_strname, l_contrast_agent_name); //Tanoue/MVC5849


            l_contrast_agent_index = m_contrastagents_reader->GetAttributeValue(l_index,
                                     NODE_TAG_NAME_CONTRAST_AGENT,
                                     CHILD_TAG_NAME_INDEX,
                                     ATTRIBUTE_NAME_1);

            if (l_contrast_agent_index) {

                l_contrastagent.m_index = m_contrastagents_reader->ReadLongValue(l_contrast_agent_index);
            }

            l_contrast_agent_data->AddTail(l_contrastagent);

        }//end for

    } else {
        //if file is not loaded then add default contrast agent
        l_contrastagent.m_strname = DEFAULT_CONTRASTAGENT;
        l_contrastagent.m_index = m_contrastagents_reader->ReadLongValue(DEFAULT_CONTRASTAGENTINDEX);
        l_contrast_agent_data->AddTail(l_contrastagent);
    }

    return l_status;
}
*/
//-Patni-ARD/2010Apr08/Commented/JaFT# IR-100  code review



//SM3_Mangesh_End

//Patni-AMT/2009Nov06/Deleted/MSA0248-00100-Code Review
// GetVAPChannel() and GetVEFPChannel() functions are removed as now its reading
// from sm.cf file.

//********************************Method Header********************************
//Method Name   :GetMachineType()
//Author        :PATNI/ARP
//Purpose       :Get the type of machine(One path or Using vantage)
//*****************************************************************************
//+Patni-ARD/2010Mar3/Commented/DeFT# IR-100 remove IPCConfig.ini
/*
CString CPQMConfig::GetMachineType(
)
{
    if (m_ipcconfig_reader->SectionExists(_T("Monitor"))) {
        return m_ipcconfig_reader->GetKeyValue(_T("MachineType"), _T("Monitor"));

    } else {
        //  CString l_err_str(_T("Machine type not found"));									// Added By KT/ACE-2/15-06-2009/Multilingualisation
        CString l_err_str = _T("");														// Added By KT/ACE-2/15-06-2009/Multilingualisation
        //l_err_str.LoadString(IDS_STUDY_NOT_FOUND);

        CPQMLogMgr::GetInstance()->WriteEventLog(IDS_NO_READ_VALUE,
                ERROR_LOG_CRUCIAL1,
                _T("PQMConfig.cpp"));

        l_err_str = CPqmUtilities::GetMultiLingualString(_T("IDS_MACHINE_TYPE_ERROR"));		// Added By KT/ACE-2/15-06-2009/Multilingualisation

        CPQMLogMgr::GetInstance()->DisplayError(
            l_err_str,																		// Added By KT/ACE-2/15-06-2009/Multilingualisation
            _T("PQMConfig.cpp"), __LINE__, ERROR_LOG_CRUCIAL1 ,
            ERR_ID, APP_NAME);

        return _T("");
    }
}
*/
//-Patni-ARD/2010Mar3/Commented/DeFT# IR-100 remove IPCConfig.ini

// + SM4 Himanshu 5 Dec 2008 TEMP Remove Later
//********************************Method Header********************************
//Method Name   :GetHomePosition()
//Author        :PATNI / HAR
//Purpose       :
//*****************************************************************************
//+Patni-ARD/2010Mar10/Commented/Ph-3# IR-100 code review
/*
CString CPQMConfig::GetHomePosition(
)
{
    if (m_ipcconfig_reader->SectionExists(_T("Testing"))) {
        return m_ipcconfig_reader->GetKeyValue(_T("Home_Position"), _T("Testing"));

    } else {
        //CString l_err_str(_T("Home Position not found"));				// Added By KT/ACE-2/15-06-2009/Multilingualisation
        CString l_err_str = _T("");									// Added By KT/ACE-2/15-06-2009/Multilingualisation
        //l_err_str.LoadString(_T("Home Position not found"));

        CPQMLogMgr::GetInstance()->WriteEventLog(l_err_str,
                ERROR_LOG_CRUCIAL1 , _T("PQMConfig.cpp"));

        l_err_str = CPqmUtilities::GetMultiLingualString(_T("IDS_HOME_POSITION_ERROR"));    // Added By KT/ACE-2/15-06-2009/Multilingualisation

        CPQMLogMgr::GetInstance()->DisplayError(
            l_err_str,																		// Added By KT/ACE-2/15-06-2009/Multilingualisation
            _T("PQMConfig.cpp"), __LINE__, ERROR_LOG_CRUCIAL1 ,
            ERR_ID, APP_NAME);

        return _T("");
    }
}
*/
//-Patni-ARD/2010Mar10/Commented/Ph-3# IR-100 code review


//********************************Method Header********************************
//Method Name   :GetAcqCondition()
//Author        :PATNI / HAR
//Purpose       :
//*****************************************************************************
//+Patni-ARD/2010Mar10/Commented/Ph-3# IR-100 code review
/*
CString CPQMConfig::GetAcqCondition(
)
{
    if (m_ipcconfig_reader->SectionExists(_T("Testing"))) {
        return m_ipcconfig_reader->GetKeyValue(_T("Acq_Condition"), _T("Testing"));

    } else {
        // CString l_err_str(_T("Acquisition Condition not found"));    // Added By KT/ACE-2/15-06-2009/Multilingualisation
        //sl_err_str.LoadString(_T("Acquisition Condition not fo//und"));
        CString l_err_str = _T("");									// Added By KT/ACE-2/15-06-2009/Multilingualisation

        CPQMLogMgr::GetInstance()->WriteEventLog(l_err_str,
                ERROR_LOG_CRUCIAL1 , _T("PQMConfig.cpp"));

        l_err_str = CPqmUtilities::GetMultiLingualString(_T("IDS_ACQUISITION_ERROR"));   // Added By KT/ACE-2/15-06-2009/Multilingualisation

        CPQMLogMgr::GetInstance()->DisplayError(
            l_err_str,																	// Added By KT/ACE-2/15-06-2009/Multilingualisation
            _T("PQMConfig.cpp"), __LINE__, ERROR_LOG_CRUCIAL1 ,
            ERR_ID, APP_NAME);

        return _T("");
    }
}
*/
//-Patni-ARD/2010Mar10/Commented/Ph-3# IR-100 code review
// - SM4 Himanshu 5 Dec 2008 TEMP Remove Later

//+Patni-MP/2009Mar12/Added/ACE-1/JETSCAN Requirement
//-Patni-MP/2009Mar12/Added/ACE-1/JETSCAN Requirement

//+Patni-ARD/2009Oct28/Added/IR 85 Auto Transfer

//********************************Method Header********************************
//Method Name   :GetScanAppSiteFileName()
//Author        :PATNI/ARD
//Purpose       :Get the number of AutoJob tag in MplusMRSrvScanAPPSite.xml
//*****************************************************************************
/*
CPTString CPQMConfig::GetScanAppSiteFileName(
)
{
    CPTString l_file_name = L"";
    wchar_t l_buffer[MAX_PATH] = {0};

    if ((::GetEnvironmentVariable(_T("MPLUSCONFDIR"), l_buffer, MAX_PATH)) == NULL) {
        wsprintf(l_buffer, _T("C:\\MPlusPlatform\\Config"));
    }

    l_file_name = l_buffer;
    l_file_name += _T("\\MPlusConfig\\Site\\") + SCAN_APP_SITE_FILENAME;

    return l_file_name;
}
*/








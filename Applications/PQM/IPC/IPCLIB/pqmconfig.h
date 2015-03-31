//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-780
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMConfig.h
 *  Overview: Structure of CPQMConfig class.
 *  Last Modified: 2008/5/20 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2007/6/15 12:00:00     PQM5 Baselined code
 *   2.0          Patni            2007/8/10 12:00:00     Implemented PQM6
                                                            requirements & Formatted
 *   3.0          Patni/LK         2007/8/10 12:00:00     Restructured after
                                                            TA comments
 *****************************************************************************/
#ifndef __SM_IPCLIB_PQMCONFIG_H__
#define __SM_IPCLIB_PQMCONFIG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>
//SM3_Mangesh_Start
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"
//SM3_Mangesh_End

class CIniReader;
class CPQMXMLParser;
class CScanAnatomyReader;

//Patni-Ajay/2010July23/Added/TMSC-REDMINE-443
class CPlanDuplicateXMLReader;
//Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
class CSFTMenuXMLReader ;
//Patni-AJS/2010Sep3/Added/ScanCommentDialog/IR-163
class CScanCommentXMLReader;

class CPQMConfig : private MR::ACQ::SM::COMMON::NonCopyable<CPQMConfig>
{
public:

    static CPQMConfig* GetInstance();

    virtual ~CPQMConfig();

    bool IsSkipRGN() const;
    long ReadMaxProtocolLimitfromFile();
    bool IsUniCodeSupportedAcqMan()const;
    bool CanEnableAutoTransfer() const;
    void ReadAndSetAutoJobTagNumber();
    void LoadToolMenu(const bool f_flag);
    void LoadSFTConfigMenu();

    int   GetJetChannel_V() const;
    void  SetJetChannel_V(const int f_jet_channel_v);

    float GetRxGainOffset()const;
    void  SetRxGainOffset(const float offset);

    CString GetCouchMaxParentOffset() const;
    //CString GetDataBaseSelection() const;
    int  GetBackGroundColor(const CString& f_protocol_status, COLORREF& f_backgroundcolor);
    int  GetForeGroundColor(const CString& f_protocol_status, COLORREF& f_foregroundcolor);
    BOOL GetTraceLogCSSDKValue(int& f_tracelog_cssdk_value)const;
    BOOL GetTraceLogPath(CString& f_tracelog_path)const;
    BOOL GetEventLogCSSDKValue(int& f_eventlog_cssdk_value)const;
    int  GetEventLogPath(CString& f_eventlog_path)const;
    bool GetSFTMenuXMLData(VARIANT** f_scan_anatomy_data)const;
    bool GetToolMenuXMLData(VARIANT** f_scan_anatomy_data)const;
    bool GetScanCommentXMLData(VARIANT** f_scan_comment_data);
    bool GetScanAnatomy(VARIANT** f_scan_anatomy_data)const;
    bool GetPresetItem(VARIANT** f_preset_item_data)const;

    void SetSkipRGN(const bool set);
    void SetStudyUID(const CString& f_studyuid);

private:

    static CPQMConfig*              m_pqmconfig; // singleton instance
    static BOOL                     m_is_pqm_app_settings_loaded;
    static BOOL                     m_is_color_settings_loaded;
    static BOOL                     m_is_scan_anatomy_loaded;
    static CIniReader*              m_ipcconfig_reader;  // INI reader for IPCConfig.ini
    static CIniReader*              m_map_reader;        // INI reader for map.ini
    static CPQMXMLParser*           m_colorsettings_reader;// XML Parser for PQM_ColorSetting file
    static CPQMXMLParser*           m_pqmappconfig_reader;// XML Parser for PQM_AppConfig file
    static CPQMXMLParser*           m_pqm_auto_transfer_reader;// XML Parser for MPlusMRSrvScanAPPSite.xml file
    static BOOL                     m_is_auto_transfer_reader;
    static CScanAnatomyReader*		m_scan_anatomy_reader ;
    static CPlanDuplicateXMLReader*	m_preset_item_reader ;
    static CScanCommentXMLReader*	m_scan_comment_reader ;


    int                m_nAuto_job_number;
    bool               m_skip_rgn;
    float              m_rxgain_offset;
    int				   m_JetChannel_V;
    CSFTMenuXMLReader* m_sft_menu_parser;
    CSFTMenuXMLReader* m_tool_menu_parser;
    bool               m_load_tool_menu ;


    CPQMConfig();

    BOOL InitializeConfig();
    int  ConvertHexStringToInt(LPCSTR str)const;
    long ReadMaxContrastAgentsLimitfromFile()const;
    long ReadMaxSarAnatomyLimitfromFile()const;
    long ReadMaxScanAnatomyLimitfromFile()const;


    BOOL GetRGBValueFromString(const CString& f_rgbstr, COLORREF& f_rgbcolor)const;

    CPTString GetSFTMenuFileName() const;
    CPTString GetToolMenuFileName() const;
};

#endif // #ifndef __SM_IPCLIB_PQMCONFIG_H__ 
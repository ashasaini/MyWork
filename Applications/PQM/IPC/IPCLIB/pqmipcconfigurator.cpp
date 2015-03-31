//Redmine-778
//Redmine-780
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: pqmipcconfigurator.cpp
 *  Overview: Implementation of CPQMIPCConfigurator class.
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

#include "PQMIPCConfigurator.h"
#include <WIN32DEP/INIReader.h>
#include <PQM/PQMLogMgr.h>

#include "PQMIPC.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


CString CPQMIPCConfigurator::m_config_file_name = _T("");
CString CPQMIPCConfigurator::m_map_file_name = _T("");
#define PQM_IPC_CONFIG_FILE_NAME    L"IPCCONFIG.INI"

//********************************Method Header********************************

//Method Name   :CPQMIPCConfigurator()

//Author        :PATNI/MRP

//Purpose       :Constructor

//*****************************************************************************
CPQMIPCConfigurator::CPQMIPCConfigurator():
    m_skip_rgn(true),
    m_rxgain_offset(0.0)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCConfigurator::CPQMIPCConfigurator");
    PQM_TRACE1(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    CString pqm_section(_T("Channels"));

    // { Prasad 12-May-2006 , VSS code reusability optimisation
    CIniReader ini_reader(GetConfigFileName());
    // } Prasad 12-May-2006 , VSS code reusability optimisation

    time_t ltime = 0L;
    time(&ltime);

    if (ini_reader.SectionExists(pqm_section)) {
        m_acq_channel_name =
            ini_reader.GetKeyValue(_T("Acq_Channel_Name"), pqm_section);
        CString key_value =
            ini_reader.GetKeyValue(_T("PQM_Channel_Name"), pqm_section);

        //      m_trace_level=
        //          (CTraceInfo::TRACE_LEVEL)
        //                  _ttoi(ini_reader.GetKeyValue(_T("TraceLevel"), pqm_section));

        m_pqm_channel_name = key_value;

    } else {
        m_acq_channel_name = L"PQMServer";
        m_pqm_channel_name = L"PQMComm";

    }

    m_pqm_channel_name.TrimRight(_T("\n"));
}

//********************************Method Header********************************

//Method Name   :~CPQMIPCConfigurator()

//Author        :PATNI/MRP

//Purpose       :Destructor

//*****************************************************************************
CPQMIPCConfigurator::~CPQMIPCConfigurator(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCConfigurator::~CPQMIPCConfigurator");
    PQM_TRACE1(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace"));
}

//********************************Method Header********************************

//Method Name   :IsSkipRGN()

//Author        :PATNI/MRP

//Purpose       :To get the value for SkipRGN

//*****************************************************************************
bool CPQMIPCConfigurator::IsSkipRGN(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCConfigurator::IsSkipRGN");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));
    return m_skip_rgn;
}

//********************************Method Header********************************

//Method Name   :SetSkipRGN()

//Author        :PATNI/MRP

//Purpose       :To set the value for SkipRGN

//*****************************************************************************
void CPQMIPCConfigurator::SetSkipRGN(
    bool set
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCConfigurator::SetSkipRGN");

    CString str_msg(_T(""));
    str_msg.Format(_T("%d"), set);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);
    m_skip_rgn = set;
}

//********************************Method Header********************************

//Method Name   :GetAcqmanChannelName()

//Author        :PATNI/MRP

//Purpose       :To get the channel name for communicating with Acqman

//*****************************************************************************
CString CPQMIPCConfigurator::GetAcqmanChannelName(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCConfigurator::GetAcqmanChannelName");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, m_acq_channel_name);

    return m_acq_channel_name;
}

//********************************Method Header********************************

//Method Name   :GetPQMServerChannelName()

//Author        :PATNI/MRP

//Purpose       :To get the Channel name which PQM IPC will listen too.

//*****************************************************************************
CString CPQMIPCConfigurator::GetPQMServerChannelName(
) const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCConfigurator::GetPQMServerChannelName");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, m_pqm_channel_name);
    return m_pqm_channel_name;
}

//********************************Method Header********************************

//Method Name   :GetRxGainOffset()

//Author        :PATNI/MRP

//Purpose       :To get the RxGainOffset

//*****************************************************************************
float CPQMIPCConfigurator::GetRxGainOffset(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCConfigurator::GetRxGainOffset");

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
void CPQMIPCConfigurator::SetRxGainOffset(
    float offset
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCConfigurator::SetRxGainOffset");

    CString str_msg(_T(""));
    str_msg.Format(_T("%f"), offset);


    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    m_rxgain_offset = offset;
}

//********************************Method Header********************************

//Method Name   :GetConfigFileName()

//Author        :PATNI/MRP

//Purpose       :Get the config file name

//*****************************************************************************
CString CPQMIPCConfigurator::GetConfigFileName(
) const
{
    wchar_t buffer[MAX_PATH];
    ::GetEnvironmentVariable(_T("MRMP_Config"), buffer, MAX_PATH);
    m_config_file_name  = buffer;
    m_config_file_name += L"\\";
    m_config_file_name += L"PQM\\IPCCONFIG.INI";

    return m_config_file_name;
}

//********************************Method Header********************************

//Method Name   :GetMapFileName()

//Author        :PATNI/HAR

//Purpose       :Get the map file name

//*****************************************************************************
//-Patni-ARD/2010Mar10/Commented/Ph-3# IR-100 code review
/*
CString CPQMIPCConfigurator::GetMapFileName(
)
{
    wchar_t buffer[MAX_PATH];
    ::GetEnvironmentVariable(_T("MRMP_Config"), buffer, MAX_PATH);
    m_map_file_name  = buffer;
    m_map_file_name += L"\\";
    m_map_file_name += L"MEMORY\\MAP.INI";

    return m_map_file_name;
}
*/
//-Patni-ARD/2010Mar10/Commented/Ph-3# IR-100 code review

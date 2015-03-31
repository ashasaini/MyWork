//TMSC-REDMINE-783
//TMSC-REDMINE-780
//Redmine-775
//Redmine-777
//TMSC-REDMINE-774
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMUtility.h
 *  Overview: Structure of CPQMUtility class.
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
#ifndef __SM_PQM_UTILITY_H__
#define __SM_PQM_UTILITY_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>

class CIniReader;   //Patni/2010Apr07/Added/JaFT# IR-100

class CPQMUtility  : private MR::ACQ::SM::COMMON::NonCopyable<CPQMUtility>
{
public:
    static unsigned int __stdcall WaitForCALAApplcationProcess(void* f_parameter);
    static unsigned int __stdcall WaitForGLParamSetReady(void* f_parameter);
    static CPQMUtility* GetInstance();

    void ParseLine(const CString& f_strline, CString& f_name,
                   CString& f_key, CString& f_value)const;


    void ConvertDoubleToString(const double d_val, CString& str, const CString& format_spec)const;
    double ConvertStringToDouble(const CString& str) const;
    void GetScanOffsetValues(const CString& f_strline, CString& x_offset,
                             CString& y_offset, CString& z_offset)const;

    static void CloseUtility();

    DWORD AsciiToString(LPCTSTR f_chInputChar, LPTSTR f_chOutputChar) const;

    static CString GetPQMStatusString();
    static BOOL GetConfigFilePath(CString& f_config_file_path);

    static int GetPQMStatus();
    static void SetPQMStatus(const int state);

    static CString GetPQMParentName();
    static void SetPQMParentName(const CString& parent_name);

    static CString GetApplicationName();
    static CString GetMultiLingualString(const CString& f_str); // Added By KT/ACE-2/15-06-2009/Multilingualisation

    static bool GetBoolKeyValue(
        CIniReader& f_reader,
        const CString& f_key,
        const CString& f_section
    );
    static CString GetSenderName();

    void SetSenderParam(const CString& f_sender_param);
    void SetSenderChannel(const CString& f_channel);

private:

    static CPQMUtility* m_utility;
    static int m_status;
    static CString m_parent_name;
    static CString m_parent_channel;
    static CString m_sender_param;

    CPQMUtility();
    virtual ~CPQMUtility();

    CString GetSenderChannel()const;
};

#endif // #ifndef __SM_PQM_UTILITY_H__

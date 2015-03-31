/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: ACGL
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#if !defined(AFX_ACGLLOGHELPER_H__55864EE1_DA9F_4A37_A047_BA9952C3ADF7__INCLUDED_)
#define AFX_ACGLLOGHELPER_H__55864EE1_DA9F_4A37_A047_BA9952C3ADF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "LogLibrary\MRErrorLog.h"
#include "LogLibrary\MRTraceLog.h"

using namespace MR::ERRORLOG;
using namespace MR::TRACELOG;
typedef enum MSG_OPTIONS {
    ERROR_LOGONLY = 1,
    ERRORVIEW_DISPLAY
};
/**
*  This class is a wrapper for logging information in a file.
*/
class CACGLLogHelper
{

//functions
private:
    CACGLLogHelper();
    static bool CreateErrorLogInstance(BSTR lParam);
    static bool CreateTraceLogInstance(BSTR lParam);

public:

    static bool CreateInstance(BSTR);

    static void WriteToErrorLog(CString f_smsg, int f_Line_No = 0, CString f_csFile_Name = _T(""), int f_nmsgoption = ERROR_LOGONLY);
    static void WriteToTraceLog(LPCTSTR l_domain_name,
                                unsigned long l_marker_value,
                                LPCTSTR l_log_text,
                                LPCTSTR l_function_name = _T(""));
    static void DeleteInstance();

//variables
private:
    /**
    *  Pointer to CMRErrorLog class.
    */
    static CMRErrorLog *m_errorlog_instance;

    /**
    *  Pointer to CMRTraceLog class.
    */
    static CMRTraceLog *m_tracelog_instance;


    /**
    *  Pointer to CACGLLogHelper class.
    */
    static CACGLLogHelper* m_pACGLLogger;

    /**
    *  Represents the path of ACGL application ini file.
    */
    static CString m_config_path;

};


#endif // !defined(AFX_AFX_ACGLLOGHELPER_H__55864EE1_DA9F_4A37_A047_BA9952C3ADF7__INCLUDED_)
///////////////////////////////// END OF FILE /////////////////////////////////
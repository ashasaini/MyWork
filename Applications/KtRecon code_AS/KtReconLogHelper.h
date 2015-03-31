// KtReconLogHelper.h: interface for the CKtReconLogHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KTRECONLOGHELPER_H__DC351DFD_ACD4_4463_8EF3_210E9191C331__INCLUDED_)
#define AFX_KTRECONLOGHELPER_H__DC351DFD_ACD4_4463_8EF3_210E9191C331__INCLUDED_

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
class CKtReconLogHelper
{

//functions
private:
    CKtReconLogHelper();
    
    static bool CreateErrorLogInstance(BSTR lParam);
    static bool CreateTraceLogInstance(BSTR lParam);

public:
    virtual ~CKtReconLogHelper();
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
    *  Pointer to CKtReconLogHelper class.
    */
    static CKtReconLogHelper* m_pKtReconLogger;

    /**
    *  Represents the path of TiPrep application ini file.
    */
    static CString m_config_path;

};

#endif // !defined(AFX_KTRECONLOGHELPER_H__DC351DFD_ACD4_4463_8EF3_210E9191C331__INCLUDED_)

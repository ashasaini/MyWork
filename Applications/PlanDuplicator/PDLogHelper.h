// PDLogHelper.h: interface for the CPDLogHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PDLOGHELPER_H__82C132DB_5474_4A6F_9500_6D821BCC0C06__INCLUDED_)
#define AFX_PDLOGHELPER_H__82C132DB_5474_4A6F_9500_6D821BCC0C06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "dataTypes.h"
#include "LogLibrary\MRErrorLog.h"
#include "LogLibrary\MRTraceLog.h"
#include "PDErrorMsgDlg.h"

using namespace MR::ERRORLOG;
using namespace MR::TRACELOG;

/**
*  This class is a wrapper for logging information in a file.
*/

class CPDLogHelper  
{
public:
    
    //functions
private:
    CPDLogHelper();
    virtual ~CPDLogHelper();
    
    static int CreateErrorLogInstance(void);
    static int CreateTraceLogInstance(void);
    
public:
    
    static int CreateInstance(void);
    
    static void WriteToErrorLog(CString &f_smsg, int f_Line_No, Msg_Option f_nmsgoption = ERROR_LOGONLY);
    static void WriteToTraceLog(LPCTSTR l_domain_name,
                                unsigned long l_marker_value,
                                LPCTSTR l_log_text, 
                                LPCTSTR l_function_name = _T(""));
private:
    CPDLogHelper(CPDLogHelper& f_calc);
    CPDLogHelper& operator = (CPDLogHelper& f_calc);

/**
*  Pointer to CMRErrorLog class.
    */
    static CMRErrorLog *m_errorlog_instance;
    
    /**
    *  Pointer to CMRTraceLog class.
    */
    static CMRTraceLog *m_tracelog_instance;
    
    /**
    *  Pointer to CTiPrepLogHelper class.
    */
    static CPDLogHelper* m_pPDLogger;
    
    /**
    *  Represents the path of TiPrep application ini file.
    */
    static CString m_config_path;
};

#endif // !defined(AFX_PDLOGHELPER_H__82C132DB_5474_4A6F_9500_6D821BCC0C06__INCLUDED_)

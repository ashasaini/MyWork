#include "stdafx.h"
#include "RelimitLogHelper.h"
#include "PDTrace.h"
#include "PDLogHelper.h"

/*************************************************************
* Method:    	WriteToErrorLog
* Description: 	This function is used 
* Parameter: 	char * f_smsg :
* Parameter: 	int f_Line_No :
* Parameter: 	Msg_Option f_nmsgoption :
* Returns:   	void
*************************************************************/
void WriteToErrorLog(char *f_smsg, int f_Line_No, Msg_Option f_nmsgoption)
{
    CString l_cserrmsg(f_smsg);
    CPDLogHelper::WriteToErrorLog(l_cserrmsg,f_Line_No,f_nmsgoption);
}

/*************************************************************
* Method:    	TraceFuncEnter
* Description: 	This function is used 
* Parameter: 	char * f_function_name :
* Returns:   	void
*************************************************************/
void TraceFuncEnter(char * f_function_name)
{
         CString l_csfunc_name(f_function_name);
         CString    l_trace_line = _T("");
         l_trace_line.Format(_T("[0x%8lx] Entering %s()\n"), GetCurrentThreadId(),
             l_csfunc_name);
         CPDLogHelper::WriteToTraceLog(_T("MRI_PD"),MRTraceMarker5,
             l_trace_line,l_csfunc_name);
}

/*************************************************************
* Method:    	TraceFuncExit
* Description: 	This function is used 
* Parameter: 	char * f_function_name :
* Returns:   	void
*************************************************************/
void TraceFuncExit(char * f_function_name)
{
    CString l_csfunc_name(f_function_name);
    CString     l_trace_line = _T("");
    l_trace_line.Format(_T("[0x%8lx] Exiting %s()\n"), GetCurrentThreadId(),
        l_csfunc_name);
    CPDLogHelper::WriteToTraceLog(_T("MRI_PD"),MRTraceMarker5,
        l_trace_line,l_csfunc_name);
}
/*************************************************************
* Method:    	WriteToTraceLog
* Description: 	This function is used 
* Parameter: 	char * f_smsg :
* Parameter: 	int f_Line_No :
* Parameter: 	Msg_Option f_nmsgoption :
* Returns:   	void
*************************************************************/
void WriteToTraceLog(char *f_smsg,char * func_name)
{
    CString l_cserrmsg(f_smsg);
    CString l_csfuncname (func_name);
    CPDLogHelper::WriteToTraceLog(_T("MRI_PD"),MRTraceMarker6,l_cserrmsg,l_csfuncname);
}

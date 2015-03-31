// ReconRawCheck.cpp: implementation of the CReconRawCheck class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ReconRawCheck.h"
#include "CommonDefinations.h"
#include "EPILogHelper.h"


#define BUFSIZE 512 


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*************************************************************
* Method:    	CReconRawCheck
* Description: 	This function is constructor 
* Returns:   	
*************************************************************/
CReconRawCheck::CReconRawCheck():
m_hChildStd_IN_Rd(NULL),
m_hChildStd_IN_Wr(NULL),
m_hChildStd_OUT_Rd(NULL),
m_hChildStd_OUT_Wr( NULL),
m_hInputFile (NULL)

{
    /*EPI_TRACE(CReconRawCheck::CReconRawCheck);*/
}

CReconRawCheck& CReconRawCheck::operator = (CReconRawCheck& f_RawCheck_ptr)
{
    /*EPI_TRACE(CReconRawCheck::operator);*/
    
    if(this == &f_RawCheck_ptr) {
        return(*this);
    }
    RawCheckCleanUp();
    m_hChildStd_IN_Rd = f_RawCheck_ptr.m_hChildStd_IN_Rd;
    m_hChildStd_IN_Wr = f_RawCheck_ptr.m_hChildStd_IN_Wr;
    m_hChildStd_OUT_Rd = f_RawCheck_ptr.m_hChildStd_OUT_Rd;
    m_hChildStd_OUT_Wr = f_RawCheck_ptr.m_hChildStd_OUT_Wr;
    m_hInputFile = f_RawCheck_ptr.m_hInputFile;

    return (*this);
}

/*************************************************************
* Method:    	~CReconRawCheck
* Description: 	This function is destructor 
* Returns:   	
*************************************************************/
CReconRawCheck::~CReconRawCheck()
{
    /*EPI_TRACE(CReconRawCheck::~CReconRawCheck);*/

// The remaining open handles are cleaned up when this process terminates. 
// To avoid resource leaks in a larger application, close handles explicitly.

    RawCheckCleanUp();
    
}

/*************************************************************
* Method:    	RawCheck
* Description: 	This function is interface for rawcheck with 
                other class
* Parameter: 	CString & f_engineHost :engine name mrengine00
* Parameter: 	CString & f_csStudyFileName :study file name 
                Run-xxx
* Parameter: 	int f_nProtocolNo : protocol index 1.x
* Parameter: 	CString f_RawCheckStatusFilePath : status file 
                path with file name
* Returns:   	BOOL
*************************************************************/
BOOL CReconRawCheck::RawCheck( CString& f_engineHost , 
                              CString& f_csStudyFileName , 
                              int f_nProtocolNo , 
                              CString &f_RawCheckStatusFilePath )
{
    /*EPI_TRACE(CReconRawCheck::RawCheck,MRTraceMarker3);*/
    
    try{        
        SECURITY_ATTRIBUTES saAttr;     
        
        // Set the bInheritHandle flag so pipe handles are inherited. 
        
        saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
        saAttr.bInheritHandle = TRUE; 
        saAttr.lpSecurityDescriptor = NULL; 
        
        // Create a pipe for the child process's STDOUT. 
        
        m_hChildStd_OUT_Rd = NULL;
        m_hChildStd_OUT_Wr = NULL;
        if ( ! CreatePipe(&m_hChildStd_OUT_Rd, &m_hChildStd_OUT_Wr, &saAttr, 0) ){
            CString l_csRawCheckErr =_T("CreatePipe Failed");
            CEPILogHelper::WriteToErrorLog(l_csRawCheckErr, __LINE__,__FILE__);
            RawCheckCleanUp();
            return FALSE;    
        }
        if(NULL == m_hChildStd_OUT_Rd || NULL == m_hChildStd_OUT_Wr || m_hChildStd_OUT_Rd == INVALID_HANDLE_VALUE ){
            CString l_csRawCheckErr =_T("OUT pipes are NULL");
            CEPILogHelper::WriteToErrorLog(l_csRawCheckErr, __LINE__,__FILE__);
            RawCheckCleanUp();
            return FALSE;
        }

        // Ensure the read handle to the pipe for STDOUT is not inherited.
        if ( ! SetHandleInformation(m_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0) ){
            CString l_csRawCheckErr =_T("SetHandleInformation Failed");
            CEPILogHelper::WriteToErrorLog(l_csRawCheckErr, __LINE__,__FILE__);
            RawCheckCleanUp();
            return FALSE;    
            
        }    
        // Create a pipe for the child process's STDIN. 
        m_hChildStd_IN_Rd = NULL;
        m_hChildStd_IN_Wr = NULL;
        if (! CreatePipe(&m_hChildStd_IN_Rd, &m_hChildStd_IN_Wr, &saAttr, 0)) {
            CString l_csRawCheckErr =_T("CreatePipe Failed");
            CEPILogHelper::WriteToErrorLog(l_csRawCheckErr, __LINE__,__FILE__);
            RawCheckCleanUp();
            return FALSE; 
        }        
        if(NULL == m_hChildStd_IN_Rd || NULL == m_hChildStd_IN_Wr){
            CString l_csRawCheckErr =_T("IN pipes are NULL");
            CEPILogHelper::WriteToErrorLog(l_csRawCheckErr, __LINE__,__FILE__);
            RawCheckCleanUp();
            return FALSE;
        }

        // Ensure the write handle to the pipe for STDIN is not inherited. 
        
        if ( ! SetHandleInformation(m_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0) ){
            CString l_csRawCheckErr =_T("SetHandleInformation Failed");
            CEPILogHelper::WriteToErrorLog(l_csRawCheckErr, __LINE__,__FILE__);
            RawCheckCleanUp();
            return FALSE;   
        }
        
        // Get a handle to an input file for the parent. 
        // This example assumes a plain text file and uses string output to verify data flow. 
        if(TRUE == f_RawCheckStatusFilePath.IsEmpty()){
            f_RawCheckStatusFilePath = RAWCHECK_STATUS_FILEPATH;
        }
        m_hInputFile = NULL;
        m_hInputFile = CreateFile((LPCTSTR)f_RawCheckStatusFilePath, 
            GENERIC_ALL, 
            FILE_SHARE_READ  | FILE_SHARE_WRITE, 
            NULL, 
            CREATE_ALWAYS, 
            FILE_ATTRIBUTE_NORMAL, 
            NULL); 
        
        if ( m_hInputFile == INVALID_HANDLE_VALUE || NULL == m_hInputFile) {
            CString l_csRawCheckErr =_T("CreateFile = INVALID_HANDLE_VALUE ");
            CEPILogHelper::WriteToErrorLog(l_csRawCheckErr, __LINE__,__FILE__);
            RawCheckCleanUp();
            return FALSE; 
        }
        
        // Create the child process.
        if(TRUE != RawCheckCreateProcess(f_engineHost , f_csStudyFileName ,  f_nProtocolNo)){
            CString l_csRawCheckErr =_T("RawCheckCreateProcess Failed");
            CEPILogHelper::WriteToErrorLog(l_csRawCheckErr, __LINE__,__FILE__);
            RawCheckCleanUp();
            return FALSE;
        }    
        // The remaining open handles are cleaned up when this process terminates. 
        // To avoid resource leaks in a larger application, close handles explicitly.
        RawCheckCleanUp();
    }catch(...){
        CEPILogHelper::WriteToErrorLog(_T("Exception occurred in RawCheck"), __LINE__, __FILE__);
        RawCheckCleanUp();
        return FALSE;   
    }

    return TRUE;
}

/*************************************************************
* Method:    	RawCheckCreateProcess
* Description: 	This function is used to create child process 
* Parameter: 	CString & f_engineHost :engine name
* Parameter: 	CString & f_csStudyFileName :study name
* Parameter: 	int f_nProtocolNo :index number
* Returns:   	BOOL
*************************************************************/
BOOL CReconRawCheck::RawCheckCreateProcess( CString&f_engineHost , CString& f_csStudyFileName , int f_nProtocolNo )
{
    /*EPI_TRACE(CReconRawCheck::RawCheckCreateProcess,MRTraceMarker3);*/
    try
    {
        UNREFERENCED_PARAMETER(f_engineHost);
        CString szCmdline("");
        BOOL RawCheckFileReadStatus = TRUE;
        //gprexec host command    
        //for vantage to work ----------
        CString l_csStudyName;
        int l_ncnt = f_csStudyFileName.Find(':');
        int l_nLength = f_csStudyFileName.GetLength();
        l_csStudyName = f_csStudyFileName.Right(l_nLength - (l_ncnt + 1));
        szCmdline.Format(_T("c:/gp/bin/rawchk.exe -e %s:/1.%d"), l_csStudyName, f_nProtocolNo);
        //szCmdline.Format(_T("\"C:\\gp\\bin\\gprexec\" -o %s /gp/bin/rawchk -e %s:/1.%d"),f_engineHost, f_csStudyFileName, f_nProtocolNo);

        if(TRUE == szCmdline.IsEmpty()){
            CString l_csRawCheckErr =_T("Command Line is Empty");
            CEPILogHelper::WriteToErrorLog(l_csRawCheckErr, __LINE__,__FILE__);
            return FALSE;
        }
        PROCESS_INFORMATION piProcInfo; 
        STARTUPINFO siStartInfo;
        BOOL bSuccess = FALSE; 
    
        // Set up members of the PROCESS_INFORMATION structure. 
    
        ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );
    
        // Set up members of the STARTUPINFO structure. 
        // This structure specifies the STDIN and STDOUT handles for redirection.
    
        ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
        siStartInfo.cb = sizeof(STARTUPINFO); 
        siStartInfo.hStdError = m_hChildStd_OUT_Wr;
        siStartInfo.hStdOutput = m_hChildStd_OUT_Wr;
        siStartInfo.hStdInput = m_hChildStd_IN_Rd;
        siStartInfo.dwFlags |= STARTF_USESTDHANDLES;
        
        CEPILogHelper::WriteToErrorLog(_T("Create process for Raw check succeeded") , __LINE__,__FILE__);
        
        // Create the child process. 
        bSuccess = CreateProcess(NULL, //app name is kept NULL as command line.
            (LPTSTR)(LPCTSTR)szCmdline,     // command line 
            NULL,          // process security attributes 
            NULL,          // primary thread security attributes 
            TRUE,          // handles are inherited 
            CREATE_NO_WINDOW,             // creation flags 
            NULL,          // use parent's environment 
            NULL,          // use parent's current directory 
            &siStartInfo,  // STARTUPINFO pointer 
            &piProcInfo);  // receives PROCESS_INFORMATION 
    
        // If an error occurs, exit the application. 
        if ( ! bSuccess ){ 
            CString l_csRawCheckErr =_T("CreateProcess for raw check failed");
            CEPILogHelper::WriteToErrorLog(l_csRawCheckErr, __LINE__,__FILE__);
            return FALSE;
        }
        else 
        {
            //-------------------------------------------------------------------
        
            DWORD resumeThreadResult = ResumeThread(piProcInfo.hThread);
            if (-1 == resumeThreadResult)
            {
                CString temp;
                temp.Format(_T("GetExitCodeProcess failed (%d).\n"), GetLastError());
                CEPILogHelper::WriteToErrorLog(temp, __LINE__, __FILE__);
                return FALSE;
            }

            WaitForSingleObject( piProcInfo.hProcess, INFINITE );
                
            //-------------------------------------------------------------------------
            // Read from pipe that is the standard output for child process. 
            int result = -1;
            if(!GetExitCodeProcess(piProcInfo.hProcess,(LPDWORD)&result))
            {  
                //printf("GetExitCodeProcess() failed (%d)\n", GetLastError() );
                CString l_csRawCheckErr(_T(""));
                l_csRawCheckErr.Format(_T("RawCheckCreateProcess() GetExitCodeProcess failed ERRORNO = %d"),GetLastError());
                CEPILogHelper::WriteToErrorLog(l_csRawCheckErr, __LINE__,__FILE__);
            }
        
            // Close handles to the child process and its primary thread.
            RawCheckFileReadStatus = RawCheckReadFromPipe(); 
            if(piProcInfo.hProcess){
                CloseHandle(piProcInfo.hProcess);
            }
            if(piProcInfo.hThread){
                CloseHandle(piProcInfo.hThread);
            }
        }
        if(RawCheckFileReadStatus != TRUE){
            return FALSE;
        }
        return TRUE;
    }catch (...){
        CEPILogHelper::WriteToErrorLog(_T("Exception Occurred in RawCheckCreateProcess"), __LINE__,__FILE__);
        return FALSE;
    }    

}

/*************************************************************
* Method:    	RawCheckWriteToPipe
* Description: 	This function is used to write on child's end
* Parameter: 	void  
* Returns:   	void
*************************************************************/
void CReconRawCheck::RawCheckWriteToPipe( void )
// Read from a file and write its contents to the pipe for the child's STDIN.
// Stop when there is no more data. 
{ 
    /*EPI_TRACE(CReconRawCheck::RawCheckWriteToPipe,MRTraceMarker3);*/

    DWORD dwRead, dwWritten; 
    CHAR chBuf[BUFSIZE];
    BOOL bSuccess = FALSE;
    
    //    for (;;) 
    //    { 
    bSuccess = ReadFile(m_hInputFile, chBuf, BUFSIZE, &dwRead, NULL);
    if ( ! bSuccess || dwRead == 0 ) return; 
    
    bSuccess = WriteFile(m_hChildStd_IN_Wr, chBuf, dwRead, &dwWritten, NULL);
    if ( ! bSuccess || dwWritten == 0) return; 
    //    } 
    
    // Close the pipe handle so the child process stops reading. 
    
    if ( m_hChildStd_IN_Wr ){
        CloseHandle(m_hChildStd_IN_Wr);
    }
        
} 

/*************************************************************
* Method:    	RawCheckReadFromPipe
* Description: 	This function is used read from pipe at parents end 
* Parameter: 	void  
* Returns:   	void
*************************************************************/
BOOL CReconRawCheck::RawCheckReadFromPipe( void )
// Read output from the child process's pipe for STDOUT
// and write to the parent process's pipe for STDOUT. 
// Stop when there is no more data. 
{ 
    /*EPI_TRACE(CReconRawCheck::RawCheckReadFromPipe,MRTraceMarker3);*/
    try {
        DWORD dwRead, dwWritten; 
        CHAR chBuf[BUFSIZE]; 
        BOOL bSuccess = FALSE;
        if(m_hInputFile){
            HANDLE hParentStdOut = m_hInputFile;//GetStdHandle(STD_OUTPUT_HANDLE);
            
            if (hParentStdOut == NULL || hParentStdOut== INVALID_HANDLE_VALUE || m_hChildStd_OUT_Rd == NULL || m_hChildStd_OUT_Rd == INVALID_HANDLE_VALUE) //AN Revw
            {
                CEPILogHelper::WriteToErrorLog(_T("handle value invalid for RawCheckReadFromPipe()"),__LINE__,__FILE__);
                return FALSE;
            }
            char peekBuf[BUFSIZE+1] = {0};
            DWORD peekRead = 0,peekAvail = 0,peekLeftMessage= 0;
            BOOL bPeek = PeekNamedPipe(m_hChildStd_OUT_Rd, peekBuf, BUFSIZE, &peekRead, &peekAvail, &peekLeftMessage);
            if (peekAvail == 0 || bPeek == FALSE) { // Using peekAvail
                CEPILogHelper::WriteToErrorLog(_T("PeekNamedPipe return fail, unable to read from pipe"),__LINE__,__FILE__);
                return FALSE;
            }
            bSuccess = ReadFile( m_hChildStd_OUT_Rd, chBuf, BUFSIZE, &dwRead, NULL);
            CString trace = _T("ReadFile() executed");
            
            CEPILogHelper::WriteToErrorLog(_T("ReadFile for RawCheckReadFromPipe() succeeded"),__LINE__,__FILE__);
            if( ! bSuccess || dwRead == 0 ){
                CEPILogHelper::WriteToErrorLog(_T("ReadFile() failed in RawCheckReadFromPipe(). Unable to get ouput of raw check"),__LINE__,__FILE__);
                return FALSE; 
            }            
            bSuccess = WriteFile(hParentStdOut, chBuf, 
                dwRead, &dwWritten, NULL);
            trace = _T("WriteFile() executed");
            CEPILogHelper::WriteToErrorLog(_T("WriteFile for RawCheckReadFromPipe() succeeded"),__LINE__,__FILE__);
            
            if (! bSuccess ) {
                CEPILogHelper::WriteToErrorLog(_T("WriteFile() failed in RawCheckReadFromPipe(). Unable to get ouput of raw check"),__LINE__,__FILE__);
                return FALSE; 
            }
            return TRUE;
        }else{
            CEPILogHelper::WriteToErrorLog(_T("handle value invalid RawCheckReadFromPipe()"),__LINE__,__FILE__);
            return FALSE;
        }
    } catch (...){        
        CEPILogHelper::WriteToErrorLog(_T("Exception occurred in RawCheckReadFromPipe()"),__LINE__,__FILE__);
        return FALSE;
    }
} 

/*************************************************************
* Method:    	RawCheckCleanUp
* Description: 	This function is used as cleanup procedure
* Returns:   	void
*************************************************************/
void CReconRawCheck::RawCheckCleanUp()
{
    try {
    if(m_hInputFile){
        CloseHandle(m_hInputFile);
        m_hInputFile = NULL;
    }
    if(m_hChildStd_OUT_Rd){
        CloseHandle(m_hChildStd_OUT_Rd);
        m_hChildStd_OUT_Rd = NULL;
    }
    if(m_hChildStd_OUT_Wr){
        CloseHandle(m_hChildStd_OUT_Rd);
        m_hChildStd_OUT_Rd = NULL;
    }
    if(m_hChildStd_IN_Rd){
        CloseHandle(m_hChildStd_IN_Rd);
        m_hChildStd_IN_Rd = NULL;
    }
    if(m_hChildStd_IN_Wr){
        CloseHandle(m_hChildStd_IN_Wr);
        m_hChildStd_IN_Wr = NULL;
    }

    } catch (...)
    {
        CEPILogHelper::WriteToErrorLog(_T("Exception occurred in RawCheckCleanUp()"),__LINE__,__FILE__);

    }
}
///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////

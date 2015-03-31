// KtReconExecProc.cpp: implementation of the CKtReconExecProc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KtReconExecProc.h"
#include "CommonDefinitions.h"
#include "KtReconLogHelper.h"
#include <tlhelp32.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKtReconExecProc::CKtReconExecProc()
{
    memset(&m_pinfoRawCopy, 0, sizeof(PROCESS_INFORMATION));
    memset(&m_pinfoVROMProc, 0, sizeof(PROCESS_INFORMATION));
    memset(&m_pinfoReconViewer, 0, sizeof(PROCESS_INFORMATION));
}

CKtReconExecProc::~CKtReconExecProc()
{
    
}

int CKtReconExecProc::InvokeExec( int f_nExe, CString f_csParams, bool f_bIsKtReconButton )
{
    try{
        if(0 > f_nExe){
			CString l_error_string(_T("Function parameter incorrect"));
			CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
            return(E_ERROR);
        }
        
        if((true != f_bIsKtReconButton) && (KTRECONVIEWER_INIT != f_nExe)){// because when ktReconViewer is invoked from View button,
                                                                           //f_csParams parameter will be empty
            if(TRUE == f_csParams.IsEmpty()){   //for rest of the cases if f_csParams is empty then its an error condition
				CString l_error_string(_T("f_csParams is empty"));
			    CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
                return(E_ERROR_NULL_FUNCPARAMS);
            }
        }
        
        int l_nStatus = E_NO_ERROR;
        CString l_csProcessPath(_T(""));
        bool l_bIsProcRunning = false;
        char l_csProc_name[BUFFER_SIZE_128] = {'\0'};        
        
        switch(f_nExe) {
        case RAWCOPY_INIT:
            memset(l_csProc_name,0,BUFFER_SIZE_128*sizeof(char));
            strcpy(l_csProc_name,RAWCOPY_EXEC_NAME);            
            
            l_bIsProcRunning = IsProcessRunning(l_csProc_name,f_nExe);
            if(true == l_bIsProcRunning){
				CString l_error_string(_T("Rawcopy is already running"));
			    CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
                return(E_NO_ERROR); //should be E_NO_ERROR
            }
            
            l_csProcessPath = RAWCOPY_EXEC_PATH;
            l_nStatus = CreateProc(l_csProcessPath, f_csParams, m_pinfoRawCopy);
            
            if(E_NO_ERROR != l_nStatus) {
				CString l_error_string(_T("Error occured in CreateProc"));
			    CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
                return(E_ERROR);
            }
            break;
            
        case VROMPROCESSOR_INIT:
            memset(l_csProc_name,0,BUFFER_SIZE_128*sizeof(char));
            strcpy(l_csProc_name,VROMPROC_EXEC_NAME);
            
            l_bIsProcRunning = IsProcessRunning(l_csProc_name,f_nExe);
            if(true == l_bIsProcRunning){
				CString l_error_string(_T("VROMProcessor is already running"));
			    CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
                return(E_NO_ERROR); //should be E_NO_ERROR
            }           
            
            l_csProcessPath = VROMPROC_EXEC_PATH;
            l_nStatus = CreateProc(l_csProcessPath,f_csParams, m_pinfoVROMProc);
            
            if(E_NO_ERROR != l_nStatus) {
				CString l_error_string(_T("Error occured in CreateProc"));
			    CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
                return(E_ERROR);                
            }           
            break;
            
        case KTRECONVIEWER_INIT:
            memset(l_csProc_name,0,BUFFER_SIZE_128*sizeof(char));
            strcpy(l_csProc_name,RECONVIEWER_EXEC_NAME);            
            
            if(true == f_bIsKtReconButton){ //i.e k-t Recon button was clicked
                
                l_bIsProcRunning = IsProcessRunning(l_csProc_name,VROMPROCESSOR_INIT);
                if(true == l_bIsProcRunning){
					CString l_error_string(_T("KtReconViewer is already running on k-t Recon button"));
			        CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
                    return(E_NO_ERROR); //should be E_NO_ERROR
                }           
                
                l_csProcessPath = RECONVIEWER_EXEC_PATH;
                l_nStatus = CreateProc(l_csProcessPath,f_csParams,m_pinfoVROMProc);//if the button is k-t Recon then PID of VROM should be used
            }
            else{   //i.e View button was clicked
                l_bIsProcRunning = IsProcessRunning(l_csProc_name,KTRECONVIEWER_INIT);
                if(true == l_bIsProcRunning){
					CString l_error_string(_T("KtReconViewer is already running on View button"));
			        CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
                    return(E_NO_ERROR);
                }           
                
                l_csProcessPath = RECONVIEWER_EXEC_PATH;
                l_nStatus = CreateProc(l_csProcessPath,f_csParams,m_pinfoReconViewer);
            }
            
            if(E_NO_ERROR != l_nStatus) {
				CString l_error_string(_T("Error occured in CreateProc"));
			    CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
                return(E_ERROR);
            }            
            break;
            
        default:
			CString l_error_string(_T("Default case executed"));
			    CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
            break;
            
        }//switch ends
    }catch(...)
    {
		CString l_error_string(_T("Exception occured in InvokeExec"));
		CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
        return(E_ERR_EXCEPTION);
    }
    return(E_NO_ERROR);
}

bool CKtReconExecProc::IsProcessRunning(const char * l_csProc_name,int f_nExe)
{
    if((0 == strcmp(l_csProc_name,"")) || (NULL == l_csProc_name) || (0 > f_nExe)){
		CString l_error_string(_T("Function parameter NULL"));
		CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
        return(true);//should be true so that new process or multiple processes are not invoked
    }
    
    int l_nStatus = E_NO_ERROR;
    bool l_flag = false;
    
    l_nStatus = GetProcessList(l_csProc_name, l_flag, f_nExe);
    
    if(E_NO_ERROR != l_nStatus) {
        CString l_csErrMsg(_T("InvokeExec(): Error in getting process list ."));
		CKtReconLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__);
        //CKtReconLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
        return(true);   //new process should not get invoked hence true
    }
    
    if(true == l_flag) {
        //implies process already running
		CString l_error_string(_T("Process is running"));
		CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
        return(true);
    }
    return(false);
}

int CKtReconExecProc::GetProcessList( const char * process_name, bool &f_flag, int f_nExe )
{
    try{
        if((0 == strcmp(process_name,"")) || (NULL == process_name) || (0 > f_nExe)){
			CString l_error_string(_T("PIncorrect function parameter"));
		    CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
            return(E_ERROR);
        }
        
        HANDLE hProcessSnap = NULL;
        HANDLE hProcess = NULL;
        PROCESSENTRY32 pe32;
        DWORD dwPriorityClass = 0l;
        int l_nproccount = 0;
        // Take a snapshot of all processes in the system.
        hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        
        if(INVALID_HANDLE_VALUE == hProcessSnap) {
            CString l_cserrmsg = _T("CreateToolhelp32Snapshot(of processes)failed");
            CKtReconLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__);
            return(E_ERROR);
        }
        
        // Set the size of the structure before using it.
        pe32.dwSize = sizeof(PROCESSENTRY32);
        // Retrieve information about the first process,
        // and exit if unsuccessful
        
        if(!Process32First(hProcessSnap, &pe32)) {
            CString l_cserrmsg = _T("Process32First");
			CKtReconLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__);
            CloseHandle(hProcessSnap);          // clean the snapshot object
            return(E_ERROR);
        }
        
        char *l_cpC = new char[BUFFER_SIZE_256];
        
        if(NULL == l_cpC) {
            CString l_cserrmsg = _T("Memory allocation failed in GetProcessList() for char *l_cpC");
            CKtReconLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__);
            return(E_ERROR);
        }
        
        // Now walk the snapshot of processes, and
        // display information about each process in turn
        do {
            wcstombs(l_cpC, (LPCTSTR)pe32.szExeFile, 128);
            
            if(strcmp(process_name, l_cpC) == 0) {                  
                // Retrieve the priority class.
                dwPriorityClass = 0;
                hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
                
                if(NULL == hProcess) {
                    CString l_cserrmsg = _T("OpenProcess returns NULL");
                    CKtReconLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__);
                } else {
                    dwPriorityClass = GetPriorityClass(hProcess);
                    
                    if(!dwPriorityClass) {
                        CString l_cserrmsg = _T("GetPriorityClass returns NULL");
                        CKtReconLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__);
                    } else {
                        //dummy
                    }
                    
                    CloseHandle(hProcess);
                    f_flag = false;
                    switch(f_nExe){
                    case RAWCOPY_INIT:
                        if(m_pinfoRawCopy.dwProcessId == pe32.th32ProcessID) {
                            f_flag = true;                                
                        }
                        break;
                        
                    case VROMPROCESSOR_INIT:
                        if(m_pinfoVROMProc.dwProcessId == pe32.th32ProcessID) {
                            f_flag = true;                                
                        }
                        break;
                        
                    case KTRECONVIEWER_INIT:
                        if(m_pinfoReconViewer.dwProcessId == pe32.th32ProcessID) {
                            f_flag = true;                                
                        }
                        break;
                    default:
                        //f_flag = true;//just to be safe
                        //log error//recheck the message to be written to the log file
						CString l_error_string = _T("");
							CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
                        break;
                    }
                    if(false == f_flag){
                        break;
                    }
                    
                    TerminateProcess(hProcess, 0);
                }
                
                l_nproccount++;
            }
        } while(Process32Next(hProcessSnap, &pe32));
        
        if(NULL != l_cpC) {
            delete []l_cpC;
            l_cpC = NULL;
        }
        
        CloseHandle(hProcessSnap);
        
    }catch(...)
    {
        CString lcsmsg(_T("Exception occurred in GetProcessList"));
         CKtReconLogHelper::WriteToErrorLog(lcsmsg, __LINE__);
        return(E_ERR_EXCEPTION);
    }
    return(E_NO_ERROR);
}

int CKtReconExecProc::CreateProc( CString &f_csProcessNm, CString &f_csParams, PROCESS_INFORMATION &f_pid )
{
    try{
        if((TRUE == f_csProcessNm.IsEmpty()) /*|| (TRUE == f_csParams.IsEmpty())*/){//f_csParams can be empty in case of ktReconViewer
                                                                                    //when invoked from View button
            //log error
		CString lcsmsg(_T("f_csProcessNm is empty"));
         CKtReconLogHelper::WriteToErrorLog(lcsmsg, __LINE__);
            return(E_ERROR);
        }
        
        STARTUPINFO sinfo;
        memset(&sinfo, 0, sizeof(STARTUPINFO));
        memset(&f_pid, 0, sizeof(PROCESS_INFORMATION));
        sinfo.cb = sizeof(STARTUPINFO);
        
        if(!CreateProcess((LPCTSTR)f_csProcessNm, (LPTSTR)(LPCTSTR)f_csParams, NULL, NULL, FALSE, 0, NULL, NULL, &sinfo, &f_pid)) {
            CString l_cserrmsg = _T("Could not open process");
               CKtReconLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__);
            //log error
            return E_ERROR; 
        } else {
            
        }
    }catch(...)
    {
        CString lcsmsg(_T("Exception occurred in CreateProc"));
          CKtReconLogHelper::WriteToErrorLog(lcsmsg, __LINE__);
        //log error
        return E_ERR_EXCEPTION;        
    }
    return(E_NO_ERROR);
}

int CKtReconExecProc::ExitProc( void )
{
    DWORD dwExitCode = 0;
    
    try {
        //Raw Copy
        if(NULL != m_pinfoRawCopy.hProcess) {
            ::GetExitCodeProcess(m_pinfoRawCopy.hProcess, &dwExitCode);
        } else {
            //dummy
        }
        
        if(dwExitCode == STILL_ACTIVE) {
            // Process did not terminate -> force it
            ::TerminateProcess(m_pinfoRawCopy.hProcess, 0);  // Zero is the exit code
        } else {
            //dummy
        }
        
        //VROMProcessor
        if(NULL != m_pinfoVROMProc.hProcess) {
            ::GetExitCodeProcess(m_pinfoVROMProc.hProcess, &dwExitCode);
        } else {
            //dummy
        }
        
        if(dwExitCode == STILL_ACTIVE) {
            // Process did not terminate -> force it
            ::TerminateProcess(m_pinfoVROMProc.hProcess, 0);  // Zero is the exit code
        } else {
            //dummy
        }
        
        //KTReconViewer
        if(NULL != m_pinfoReconViewer.hProcess) {
            ::GetExitCodeProcess(m_pinfoReconViewer.hProcess, &dwExitCode);
        } else {
            //dummy
        }
        
        if(dwExitCode == STILL_ACTIVE) {
            // Process did not terminate -> force it
            ::TerminateProcess(m_pinfoReconViewer.hProcess, 0);  // Zero is the exit code
        } else {
            //dummy
        }
    } catch(...) {
        CString lcsmsg(_T("Exception occurred in ExitProc"));
        CKtReconLogHelper::WriteToErrorLog(lcsmsg, __LINE__);
        //log errro
        return E_ERROR;
    }    
    return(E_NO_ERROR);
}
/////////////////////////////////END OF FILE /////////////////////////////////////
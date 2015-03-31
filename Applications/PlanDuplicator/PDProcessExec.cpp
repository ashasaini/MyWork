// PDProcessExec.cpp: implementation of the CPDProcessExec class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PDProcessExec.h"
#include "Winsock2.h"
#include <tlhelp32.h>
#include "PDErrorMsgDlg.h"
#include "PDTrace.h"
#include "PDLogHelper.h"
#include "Common.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPDProcessExec::CPDProcessExec() 
{
    PD_TRACE(CPDProcessExec::CPDProcessExec);
}

CPDProcessExec::~CPDProcessExec()
{
    PD_TRACE(CPDProcessExec::~CPDProcessExec);
}

//****************************Method Header************************************

//Method Name   :GetProcessList

//Author        :PATNI

//Purpose       :Take a snapshot of all processes in the system and look for 
//               "SmartMPR.exe" if found set the flag.
//*****************************************************************************
int CPDProcessExec::GetProcessList( const char * process_name,bool &f_flag)
{
    PD_TRACE(CPDProcessExec::GetProcessList);
    HANDLE hProcessSnap=NULL;
    HANDLE hProcess=NULL;
    PROCESSENTRY32 pe32;
    DWORD dwPriorityClass=0l;
    char *l_cpC=new char[BUFFER_SIZE_160];
    int l_nproccount = 0;
    if(NULL == l_cpC){          //Mpower 040/2012-08-09/AS/Added
        CString l_cserrmsg = _T("Memory allocation failed in GetProcessList() for char *l_cpC");
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(E_ERROR);
    }

    
    // Take a snapshot of all processes in the system.
    hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
    
    if (INVALID_HANDLE_VALUE == hProcessSnap){
        CString l_cserrmsg = _T("CreateToolhelp32Snapshot(of processes)failed");
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        if(NULL != l_cpC){
            delete []l_cpC; //Mpower 030/2012-06-26/AS/Modified/cpp change
            l_cpC = NULL;
        }
        return(E_ERROR);
    }
    
    // Set the size of the structure before using it.
    pe32.dwSize = sizeof(PROCESSENTRY32);
    // Retrieve information about the first process,
    // and exit if unsuccessful
    
    if (!Process32First(hProcessSnap, &pe32)){
        CString l_cserrmsg = _T("Process32First");
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        CloseHandle(hProcessSnap);          // clean the snapshot object
        if(NULL != l_cpC){
            delete []l_cpC; //Mpower 030/2012-06-26/AS/Modified/cpp change
            l_cpC = NULL;
        }
        return(E_ERROR);
    }
    
    // Now walk the snapshot of processes, and
    // display information about each process in turn
    do{        
        wcstombs(l_cpC,(LPCTSTR)pe32.szExeFile,BUFFER_SIZE_128);    //AS was 100
        
        if(strcmp(process_name,l_cpC) == 0){					//Mpower 040/2012-08-24/AS/Modified
            // Retrieve the priority class.
            dwPriorityClass = 0;
            hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,pe32.th32ProcessID);
            
            if (NULL == hProcess){
                CString l_cserrmsg = _T("OpenProcess");
                CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
            }else{
                dwPriorityClass = GetPriorityClass(hProcess);
                
                if ( !dwPriorityClass ){
                    CString l_cserrmsg = _T("GetPriorityClass");
                    CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                }else{
                    //dummy
                }
                
                CloseHandle(hProcess);
            }
            l_nproccount++;
            f_flag=true;
            TerminateProcess(hProcess,0);
        }
                
    } while( Process32Next( hProcessSnap, &pe32 ) );
    ////Mpower 047/Sep-2012/AN/Added
    if (strcmp(process_name,PD_APPLICATION) == 0)
    {
        if (l_nproccount > 1)
        { 
            f_flag = true;
        } else
        {
            f_flag = false;
         }
    }
    ////Mpower 047/AN/Added :End
    if(NULL != l_cpC){      //Mpower 040/2012-08-09/AS/Modified/delete was inside do-while
        delete []l_cpC; //Mpower 030/2012-06-26/AS/Modified/cpp change
        l_cpC = NULL;
    }
    
    CloseHandle( hProcessSnap );
    return(E_NO_ERROR);
}

/*************************************************************
* Method:    	CreateProc
* Description: 	This function is used 
* Parameter: 	CString & l_csProcessNm :
* Returns:   	int
*************************************************************/
int CPDProcessExec::CreateProc(CString &l_csProcessNm)
{
    PD_TRACE(CPDProcessExec::CreateProc);
    STARTUPINFO sinfo;
    memset(&sinfo,0,sizeof(STARTUPINFO));       
    memset(&m_pinfo,0,sizeof(PROCESS_INFORMATION));     
    sinfo.cb = sizeof(STARTUPINFO);
    
    if (!CreateProcess(NULL,(LPTSTR)(LPCTSTR)l_csProcessNm, NULL, NULL, FALSE, 0, NULL, NULL, &sinfo, &m_pinfo)){
        CString l_cserrmsg = _T("Could not open process");
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
    }else{
        //dummy
    }
    
    return(E_NO_ERROR);
}

/*************************************************************
* Method:    	ExitProc
* Description: 	This function is used 
* Parameter: 	void  :
* Returns:   	int
*************************************************************/
int CPDProcessExec::ExitProc(void)
{
    PD_TRACE(CPDProcessExec::ExitProc);
    DWORD dwExitCode = 0;
    
    if(NULL != m_pinfo.hProcess){
        ::GetExitCodeProcess(m_pinfo.hProcess, &dwExitCode);
    }else{
        //dummy
    }
    
    if(dwExitCode == STILL_ACTIVE){
        // Process did not terminate -> force it
        ::TerminateProcess(m_pinfo.hProcess, 0);  // Zero is the exit code      
    }else{
        //dummy
    }
    
    return(E_NO_ERROR);
}   

////////////////////////    End Of File     /////////////////////////////////


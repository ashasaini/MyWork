// PDProcessExec.cpp: implementation of the CProcessExec class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#include "stdafx.h"
#include "ProcessExec.h"
#include <tlhelp32.h>
#include "TiPrepTrace.h"
#include "TiPrepLogHelper.h"
#include "CommonDefinitions.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//****************************Method Header***************************
//Method Name   : CProcessExec
//Purpose       : Constructor
//********************************************************************
CProcessExec::CProcessExec()
{
    /*TIPREP_TRACE(CProcessExec::CProcessExec);*/
}
//****************************Method Header***************************
//Method Name   : CProcessExec
//Purpose       : Destructor
//********************************************************************
CProcessExec::~CProcessExec()
{
    /*TIPREP_TRACE(CProcessExec::~CProcessExec);*/
}

HWND m_apphwnd = NULL;

/*************************************************************
* Method:    	EnumWindowsProc
* Description: 	This function is used 
* Parameter: 	HWND hwnd :
* Parameter: 	LPARAM param :
* Returns:   	int CALLBACK
*************************************************************/
int CALLBACK EnumWindowsProc(HWND hwnd, LPARAM param)
{
    try{
        DWORD pID;
        DWORD TpID = GetWindowThreadProcessId(hwnd, &pID);
        
        if(TpID == (DWORD)param) {
            m_apphwnd = hwnd;
            return false;
        }
        
        return true;
    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog
            (_T("Exception occurred in Node Create Class"), __LINE__, __FILE__);
        return false;
    }
}

/*************************************************************
* Method:       GetProcessList
* Description:  This function is used to take a snapshot of all
*               processes in the system and look for "SmartMPR.exe"
*               if found set the flag.
* Parameter:    const char * process_name
* Parameter:    bool & f_flag
* Returns:      int
*************************************************************/
int CProcessExec::GetProcessList(const char * process_name, bool &f_flag)
{
    TIPREP_TRACE(CProcessExec::GetProcessList,MRTraceMarker3);
    try{
        HANDLE hProcessSnap = NULL;
        HANDLE hProcess = NULL;
        PROCESSENTRY32 pe32;
        DWORD dwPriorityClass = 0l;
        int l_nproccount = 0;
        // Take a snapshot of all processes in the system.
        hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        
        if(INVALID_HANDLE_VALUE == hProcessSnap) {
            CString l_cserrmsg = _T("CreateToolhelp32Snapshot(of processes)failed");
            CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
            return(E_ERROR);
        }
        
        // Set the size of the structure before using it.
        pe32.dwSize = sizeof(PROCESSENTRY32);
        // Retrieve information about the first process,
        // and exit if unsuccessful
        
        if(!Process32First(hProcessSnap, &pe32)) {
            CString l_cserrmsg = _T("Process32First");
            CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
            CloseHandle(hProcessSnap);          // clean the snapshot object
            return(E_ERROR);
        }
        
        char *l_cpC = new char[160];
        
        if(NULL == l_cpC) {
            CString l_cserrmsg = _T("Memory allocation failed in GetProcessList() for char *l_cpC");
            CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
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
                    CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
                } else {
                    dwPriorityClass = GetPriorityClass(hProcess);
                    
                    if(!dwPriorityClass) {
                        CString l_cserrmsg = _T("GetPriorityClass");
                        CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
                    } else {
                        //dummy
                    }
                    
                    CloseHandle(hProcess);
                    
                    if(m_pinfo.dwProcessId == pe32.th32ProcessID) {
                        f_flag = true;
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
    }catch(...){
        CString lcsmsg(_T("Exception occurred in GetProcessList"));
        CTiPrepLogHelper::WriteToErrorLog(lcsmsg, __LINE__, __FILE__);
        E_ERR_EXCEPTION;
    }
    return(E_NO_ERROR);
}

/*************************************************************
* Method:       CreateProc
* Description:  This function is used
* Parameter:    CString & l_csProcessNm
* Parameter:    CString & l_csParams
* Returns:      int
*************************************************************/
int CProcessExec::CreateProc(CString &l_csProcessNm, CString &l_csParams, HWND &handle) 
{
    TIPREP_TRACE(CProcessExec::CreateProc,MRTraceMarker3);
    UNREFERENCED_PARAMETER(handle);
    try {
        STARTUPINFO sinfo;
        memset(&sinfo, 0, sizeof(STARTUPINFO));
        memset(&m_pinfo, 0, sizeof(PROCESS_INFORMATION));
        sinfo.cb = sizeof(STARTUPINFO);
        
        if(!CreateProcess((LPCTSTR)l_csProcessNm, (LPTSTR)(LPCTSTR)l_csParams, NULL, NULL, FALSE, 0, NULL, NULL, &sinfo, &m_pinfo)) {
            CString l_cserrmsg = _T("Could not open process");
            CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
            return E_ERROR; 
        } else {
//             Sleep(200); 
            //WaitForInputIdle(m_pinfo.hThread,2000);
//             ::EnumWindows(&EnumWindowsProc, m_pinfo.dwThreadId);
//             handle = m_apphwnd;
        }
    } catch(...) {
        CString lcsmsg(_T("Exception occurred in CreateProc"));
        CTiPrepLogHelper::WriteToErrorLog(lcsmsg, __LINE__, __FILE__);
        E_ERR_EXCEPTION;
    }
    
    return(E_NO_ERROR);
}

/*************************************************************
* Method:       ExitProc
* Description:  This function is used
* Parameter:    void
* Returns:      int
*************************************************************/
int CProcessExec::ExitProc(void)
{
    TIPREP_TRACE(CProcessExec::ExitProc,MRTraceMarker3);
    DWORD dwExitCode = 0;
    
    try {
        if(NULL != m_pinfo.hProcess) {
            ::GetExitCodeProcess(m_pinfo.hProcess, &dwExitCode);
        } else {
            //dummy
        }
        
        if(dwExitCode == STILL_ACTIVE) {
            // Process did not terminate -> force it
            ::TerminateProcess(m_pinfo.hProcess, 0);  // Zero is the exit code
        } else {
            //dummy
        }
    } catch(...) {
        CString lcsmsg(_T("Exception occurred in ExitProc"));
        CTiPrepLogHelper::WriteToErrorLog(lcsmsg, __LINE__, __FILE__);
        return E_ERROR;
    }
    
    return(E_NO_ERROR);
}

/*************************************************************
* Method:    	FindChildHWND
* Description: 	This function is used 
* Parameter: 	HWND & hwnd :
* Returns:   	int
*************************************************************/
int CProcessExec::FindChildHWND(HWND &hwnd)
{
    TIPREP_TRACE(CProcessExec::FindChildHWND,MRTraceMarker3);
    try{
    ::EnumWindows(&EnumWindowsProc, m_pinfo.dwThreadId); //Must call this function only after createproc
            hwnd = m_apphwnd;
            return E_NO_ERROR;
    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog
            (_T("Exception occurred in ProcessExecution Class"), __LINE__, __FILE__);
        return E_ERROR;
    }
}

///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////
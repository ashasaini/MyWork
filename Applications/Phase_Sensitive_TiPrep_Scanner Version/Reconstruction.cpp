// Reconstruction.cpp: implementation of the CReconstruction class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#include "stdafx.h"
#include "Reconstruction.h"
#include "TiPrepLogHelper.h"
#include "TiPrepTrace.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//****************************Method Header***************************
//Method Name   : CReconstruction
//Purpose       : Constructor
//********************************************************************
CReconstruction::CReconstruction():
m_csProcessName(_T("C:/gp/bin/reconf.exe")),
m_nProtocolNumber(-1),
m_csStudyFileName(_T(""))
{
}

//****************************Method Header***************************
//Method Name   : CReconstruction
//Purpose       : Destructor
//********************************************************************
CReconstruction::~CReconstruction()
{
}

/*************************************************************
* Method:       performReconstruction
* Description:  This function is used
* Parameter:    CString f_csStudyFileName
* Parameter:    int f_nProtocolNum
* Parameter:    int f_nDataForm
* Returns:      int
*************************************************************/
int CReconstruction::performReconstruction(CString &f_csStudyFileName, int f_nProtocolNum, int f_nDataForm)
{
    TIPREP_TRACE(CReconstruction::performReconstruction,MRTraceMarker3);
    
    try{
        STARTUPINFO l_starupInfo;
        PROCESS_INFORMATION l_processInfo;
        CString l_csProtocol;
        CString l_csDataForm;
        l_csProtocol.Format(_T("%d"), f_nProtocolNum);
        l_csDataForm.Format(_T("%d"), f_nDataForm);
        CString l_csProcessNm;
        l_csProcessNm.Format(_T("C:/gp/bin/reconf.exe"));
        CString l_csCmdLineParameter;
        l_csCmdLineParameter = "-study " + f_csStudyFileName + " -protocol " + l_csProtocol + " -form " + l_csDataForm;
        CString l_csCommandLine;
        l_csCommandLine += l_csProcessNm + " " + l_csCmdLineParameter;
        memset(&l_starupInfo, 0, sizeof(STARTUPINFO));
        memset(&l_processInfo, 0, sizeof(PROCESS_INFORMATION));
        l_starupInfo.cb = sizeof(STARTUPINFO);
        
        if(CreateProcess(NULL, (LPTSTR)(LPCTSTR)l_csCommandLine, NULL, NULL, false,CREATE_NO_WINDOW, NULL, NULL, &l_starupInfo, &l_processInfo)) {
            // Resume the external process thread.
            DWORD resumeThreadResult = ResumeThread(l_processInfo.hThread);
            // ResumeThread() returns 1 which is OK
            // (it means that the thread was suspended but then restarted)
            // Wait for the external process to finish.
            DWORD waitForSingelObjectResult = WaitForSingleObject(l_processInfo.hProcess, INFINITE);
            // WaitForSingleObject() returns 0 which is OK.
            // Get the exit code of the external process.
            DWORD exitCode;
            
            if(!GetExitCodeProcess(l_processInfo.hProcess, &exitCode)) {
                // Handle error.
                CString temp;
                temp.Format(_T("GetExitCodeProcess failed (%d).\n"), GetLastError());
                CTiPrepLogHelper::WriteToErrorLog(temp, __LINE__, __FILE__);
                return E_RECON_FAIL;
            } else {
               // CString temp;
               // temp.Format(_T("Application exitcode:%d.\n"), exitCode);
               // CTiPrepLogHelper::WriteToErrorLog(temp, __LINE__, __FILE__);
            }
        } else {
            CString temp;
            temp.Format(_T("Failed to create process\n"));
            CTiPrepLogHelper::WriteToErrorLog(temp, __LINE__, __FILE__);
		    return E_ERROR;
        }
        
        return E_NO_ERROR;
    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog
            (_T("Exception occurred in Reconstruction Class"), __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:    	setProtocolNumber
* Description: 	This function is used 
* Parameter: 	const int & f_nProtNo :
* Parameter: 	const CString & f_csStudyFileName :
* Returns:   	void
*************************************************************/
void CReconstruction::setProtocolNumber(const int &f_nProtNo, const CString &f_csStudyFileName) 
{
    m_nProtocolNumber = f_nProtNo;
    m_csStudyFileName.Format(_T("%s"), f_csStudyFileName);
    return;
}

/*************************************************************
* Method:    	getProtocolNumber
* Description: 	This function is used 
* Parameter: 	int & f_nProtNo :
* Parameter: 	CString & f_csStudyFileName :
* Returns:   	int
*************************************************************/
int CReconstruction::getProtocolNumber(int &f_nProtNo, CString &f_csStudyFileName)
{
    f_nProtNo = m_nProtocolNumber;
    f_csStudyFileName.Format(_T("%s"), m_csStudyFileName);
    return E_NO_ERROR;
}

///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////
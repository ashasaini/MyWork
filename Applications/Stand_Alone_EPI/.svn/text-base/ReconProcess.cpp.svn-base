// ReconProcess.cpp: implementation of the CReconProcess class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "epi.h"
#include "ReconProcess.h"
#include "EPILogHelper.h"
#include "CommonDefinations.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CReconProcess::CReconProcess():m_csProcessName(_T("C:/gp/bin/reconf.exe")),
                               m_nProtocolNumber(-1),
                               m_csStudyFileName(_T(""))

{

}

CReconProcess::~CReconProcess()
{

}

int CReconProcess::performReconstruction( CString &f_csStudyFileName, int f_nProtocolNum, int f_nDataForm )
{
    try{
        STARTUPINFO l_starupInfo;
        PROCESS_INFORMATION l_processInfo;
        CString l_csProcessNm;
        l_csProcessNm.Format(_T("C:/gp/bin/reconf.exe"));

        //To get proper path for study file.
        CString l_csStudyName;
        int l_ncnt = f_csStudyFileName.Find("Run");
        int l_nLength = f_csStudyFileName.GetLength();
        l_csStudyName = f_csStudyFileName.Right(l_nLength - (l_ncnt));        
        //-----------------
        
        CString l_csStudyFilename;
        l_csStudyFilename.Format("/study/%s",l_csStudyName);
        CString l_csFileCreatorCommandLine;// = "-study /study/Run40042.-1959 -protocol 3 -form 2";
        l_csFileCreatorCommandLine.Format("-study %s -protocol %d -form %d",l_csStudyFilename,f_nProtocolNum,f_nDataForm);
        CString l_csCommandLine;
        l_csCommandLine+=l_csProcessNm+" "+l_csFileCreatorCommandLine;

        memset(&l_starupInfo, 0, sizeof(STARTUPINFO));
        memset(&l_processInfo, 0, sizeof(PROCESS_INFORMATION));
        l_starupInfo.cb = sizeof(STARTUPINFO);
        
        if(CreateProcess(NULL, (LPTSTR)(LPCTSTR)l_csCommandLine, NULL, NULL, false,CREATE_NO_WINDOW, NULL, NULL, &l_starupInfo, &l_processInfo)) {
            // Resume the external process thread.
            DWORD resumeThreadResult = ResumeThread(l_processInfo.hThread);
            if (-1 == resumeThreadResult)
            {
                CString temp;
                temp.Format(_T("GetExitCodeProcess failed (%d).\n"), GetLastError());
                CEPILogHelper::WriteToErrorLog(temp, __LINE__, __FILE__);
                return EPI_RECON_FAIL;
            }
            // ResumeThread() returns 1 which is OK
            // (it means that the thread was suspended but then restarted)
            // Wait for the external process to finish.
            DWORD waitForSingelObjectResult = WaitForSingleObject(l_processInfo.hProcess, INFINITE);
            // WaitForSingleObject() returns 0 which is OK.
            // Get the exit code of the external process.
            if (WAIT_FAILED == waitForSingelObjectResult)
            {
                CString temp;
                temp.Format(_T("GetExitCodeProcess failed (%d).\n"), GetLastError());
                CEPILogHelper::WriteToErrorLog(temp, __LINE__, __FILE__);
                return EPI_RECON_FAIL;
            }
            DWORD exitCode;
            
            if(!GetExitCodeProcess(l_processInfo.hProcess, &exitCode)) {
                // Handle error.
                CString temp;
                temp.Format(_T("GetExitCodeProcess failed (%d).\n"), GetLastError());
                CEPILogHelper::WriteToErrorLog(temp, __LINE__, __FILE__);
                return EPI_RECON_FAIL;
            } else {
                CString temp;
                temp.Format(_T("Application exitcode:%d.\n"), exitCode);
                if (1 == exitCode)
                {
                    return EPI_RECON_FAIL;
                }
            }
        } else {
            CString temp;
            temp.Format(_T("Failed to create process\n"));
            CEPILogHelper::WriteToErrorLog(temp, __LINE__, __FILE__);
            return EPI_RECON_FAIL;
        }
        
        return 0;
    }
    catch(...){
        CEPILogHelper::WriteToErrorLog
            (_T("Exception occurred in performReconstruction()"), __LINE__, __FILE__);
        return EPI_ERR_EXCEPTION;
    }
}

void CReconProcess::setProtocolNumber( const int &f_nProtNo, const CString &f_csStudyFileName )
{
    m_nProtocolNumber = f_nProtNo;
    m_csStudyFileName.Format(_T("%s"), f_csStudyFileName);
    return;
}

int CReconProcess::getProtocolNumber( int &f_nProtNo, CString &f_csStudyFileName )
{
    f_nProtNo = m_nProtocolNumber;
    f_csStudyFileName.Format(_T("%s"), m_csStudyFileName);
    return 0;
}

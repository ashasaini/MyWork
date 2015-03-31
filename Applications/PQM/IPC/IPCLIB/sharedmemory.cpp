//Redmine-783
//Redmine-778
//Redmine-780
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: sharedmemory.cpp
 *  Overview: Implementation of CSharedMemory class.
 *  Last Modified: 2007/8/10 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2007/6/15 12:00:00     PQM5 Baselined code
 *   2.0          Patni            2007/8/10 12:00:00     Implemented PQM6 requirements & Formatted
 *
 *****************************************************************************/
#include "stdafx.h"
#include "SharedMemory.h"
#include <PQM/PQMLogMgr.h>
#include <PQM/Resource.h>
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSharedMemory
//*******************************Method Header*********************************
//Method Name    :CSharedMemory()
//Author         :PATNI/AC
//Purpose        :Constructor for initialisation
//*****************************************************************************
CSharedMemory::CSharedMemory(
    CString strStudyid
): m_filemapping(NULL),
    m_data(NULL
          )
{
    LPCTSTR FUNC_NAME = _T("CSharedMemory::CSharedMemory");
    PQM_TRACE1(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of CSharedMemory::CSharedMemory"));

    //+Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
    for (int i = 0; i < 256; i++) {
        m_sharedname[i] = 0;
    }

    //-Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
    m_studyid[0] = '\x0';

    TCHAR studyid[STUDY_BUFFER] = {0}; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
    wcscpy(studyid, strStudyid);

    wcscpy(m_sharedname, _T("PQMStudyID"));

    m_filemapping = CreateFileMapping(
                        INVALID_HANDLE_VALUE,
                        NULL,
                        PAGE_READWRITE,
                        0,
                        sizeof(SharedData),
                        (unsigned short*) m_sharedname);

    if (!m_filemapping) {
        // { Prasad 12-May-2006 , VSS code reusability optimisation
        //      PQM_ERROR2(IDS_NO_SHARE_MEM,
        //                      CErrorInfo::ERR_SEVERITY::EI_ERROR,
        //                      CErrorInfo::ERR_TYPE::ET_SYSTEM);

        //      CPQMLogMgr::GetInstance()->WriteEventLog(IDS_NO_SHARE_MEM,
        //          Error_Crucial,_T("SharedMemory.cpp"));

        // } Prasad 12-May-2006 , VSS code reusability optimisation
    }

    m_data = (SharedData*) MapViewOfFile(
                 m_filemapping,
                 FILE_MAP_READ | FILE_MAP_WRITE,
                 0,
                 0,
                 0);

    if (m_data) {
        if (m_data->m_no_instances >= NUM_2) {

            ReleaseResources(NULL);

        } else {

            if (m_data->m_studyid1[0] == '\x0') {
                wcscpy(m_data->m_studyid1, studyid);

            } else {
                wcscpy(m_data->m_studyid2, studyid);
            }

            m_data->m_no_instances++;
            wcscpy(m_studyid, studyid);
        }

    } else {
        // { Prasad 12-May-2006 , VSS code reusability optimisation
        //      PQM_ERROR2(IDS_NO_SHARE_MEM,
        //                      CErrorInfo::ERR_SEVERITY::EI_ERROR,
        //                      CErrorInfo::ERR_TYPE::ET_SYSTEM);

        //+Patni-ARD/2009-Nov-26/Modified/DeFT# MVC003149 + MVC005179
        /*
        CPQMLogMgr::GetInstance()->WriteEventLog(IDS_NO_SHARE_MEM,
                ERROR_LOG_CRUCIAL1 , _T("SharedMemory.cpp"));
        		*/
        PQM_TRACE(USER_FUNC_MARKER, _T("CSharedMemory::CSharedMemory"),
                  _T("Could not create shared Memory"));

        //+Patni-ARD/2009Jun25/Added/ACE# policy of ErrorView
        //CPQMLogMgr::GetInstance()->DisplayScanError(IDS_NO_SHARE_MEM, _T("IDS_NO_SHARE_MEM"),
        //        SCAN_ERR_NOTIFY, L"SharedMemory.cpp");
        //-Patni-ARD/2009Jun25/Added/ACE# policy of ErrorView
        //-Patni-ARD/2009-Nov-26/Modified/DeFT# MVC003149 + MVC005179
        // } Prasad 12-May-2006 , VSS code reusability optimisation
    }
}

//*******************************Method Header*********************************
//Method Name    :~CSharedMemory()
//Author         :PATNI/AC
//Purpose        :Destructor for releasing resources
//*****************************************************************************
CSharedMemory::~CSharedMemory(
)
{
    LPCTSTR FUNC_NAME = _T("CSharedMemory::~CSharedMemory");
    PQM_TRACE1(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of CSharedMemory::~CSharedMemory"));

    //    if (m_studyid[0] != '\x0') {
    //        ReleaseResources(m_studyid);
    //    }
}

//*******************************Method Header*********************************
//Method Name    :ReleaseResources()
//Author         :PATNI/AC
//Purpose        :Function to release resources
//*****************************************************************************
void CSharedMemory:: ReleaseResources(
    TCHAR* studyid
)
{

    LPCTSTR FUNC_NAME = _T("CSharedMemory:: ReleaseResources");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of ReleaseResources"));

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_data == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_data is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    if (studyid) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, studyid);


        if (_wcsnicmp(studyid, m_data->m_studyid1, wcslen(studyid)) == 0) {
            m_data->m_studyid1[0] = '\x0';

        } else {
            m_data->m_studyid2[0] = '\x0';
        }

        m_data->m_no_instances--;
    }

    if (!m_data->m_no_instances) {
        if (m_data) {
            UnmapViewOfFile(m_data);
            m_data = NULL;
        }

        if (m_filemapping) {
            CloseHandle(m_filemapping);
            m_filemapping = NULL;
        }
    }
}

bool CSharedMemory::IsStudyIDSame(
)const
{
    if (m_data) {
        return !wcscmp(m_data->m_studyid1, m_data->m_studyid2);
    }

    return false;
}

//*******************************Method Header*********************************
//Method Name    :IsSharedMemValid()
//Author         :PATNI/AC
//Purpose        :Function to check validity of shared memory
//*****************************************************************************
bool CSharedMemory::IsSharedMemValid(
)const
{
    LPCTSTR FUNC_NAME = _T("CSharedMemory::IsSharedMemValid");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of IsSharedMemValid"));

    bool result = false;

    if (m_data) {
        result = true;
    }

    CString str_msg(_T(""));
    str_msg.Format(_T("%d"), result);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    return result;
}

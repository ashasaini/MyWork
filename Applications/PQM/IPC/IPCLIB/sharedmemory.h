//Redmine-783
//Redmine-780
//Redmine-782
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: sharedmemory.h
 *  Overview: Structure of CSharedMemory class.
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

#ifndef __SM_IPCLIB_SHAREDMEMORY_H__
#define __SM_IPCLIB_SHAREDMEMORY_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SharedMemory.h : header file
//
#include <libStudyManager/NonCopyable.h>

/////////////////////////////////////////////////////////////////////////////
// CSharedMemory window
typedef struct SharedData {
    int m_no_instances;
    TCHAR m_studyid1[100];
    TCHAR m_studyid2[100];

    SharedData() :
        m_no_instances(0) {
        memset(m_studyid1, 0, sizeof(m_studyid1));
        memset(m_studyid2, 0, sizeof(m_studyid2));
    }

} shareddata;

class CSharedMemory : private MR::ACQ::SM::COMMON::NonCopyable<CSharedMemory>
{
public:

    explicit CSharedMemory(CString strStudyid);
    virtual ~CSharedMemory();

    void ReleaseResources(TCHAR* studyid);
    bool IsStudyIDSame()const;

private :

    HANDLE m_filemapping;
    TCHAR  m_sharedname[256];
    TCHAR  m_studyid[256];

    // keep shared memory data here
    shareddata* m_data;

    bool IsSharedMemValid()const;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // #ifndef __SM_IPCLIB_SHAREDMEMORY_H__

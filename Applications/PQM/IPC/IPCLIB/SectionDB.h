//Redmine-783
// SectionDB.h: interface for the CSectionDB class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_SECTIONDB_H__
#define __SM_IPCLIB_SECTIONDB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tami/libgpdb/section.h>
#include <libStudyManager/NonCopyable.h>
//Patni-Rajendra/2011Apr1/Added/TMSC-REDMINE-773
#include <libStudyManager/Mutex/ThreadMutex.h>

class ISectionData;

class CSectionDB : private MR::ACQ::SM::COMMON::NonCopyable<CSectionDB>
{

public:

    CSectionDB();

    virtual ~CSectionDB();

    bool Search(ISectionData* f_data, ...);

private:

    static ISectionData* m_data;
    //Patni-Rajendra/2011Apr1/Modified/TMSC-REDMINE-1559
    static MR::ACQ::SM::COMMON::MUTEX::CThreadMutex m_critical_sec;

    void FreeResources(char* f_li_fname, char* f_ld_fname, CString f_msg = _T(""));

    static void GPDBCallback(void* f_key, sedb_rec_p f_db_rec_p);
};

#endif // #ifndef __SM_IPCLIB_SECTIONDB_H__ 

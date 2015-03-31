//Redmine-783
// PathDB.h: interface for the CPathDB class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_PATHDB_H__
#define __SM_IPCLIB_PATHDB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tami/libgpdb/path.h>
#include <libStudyManager/NonCopyable.h>
//Patni-Rajendra/2011Apr1/Added/TMSC-REDMINE-773
#include <libStudyManager/Mutex/ThreadMutex.h>

class IPathData;

class CPathDB : private MR::ACQ::SM::COMMON::NonCopyable<CPathDB>
{

public:

    CPathDB();

    virtual ~CPathDB();

    bool Search(IPathData* f_data);

    static void GPDBCallback(void* f_key, path_db_rec_p f_db_rec_p);

private:

    //Patni-Rajendra/2011Apr1/Modified/TMSC-REDMINE-1559
    static MR::ACQ::SM::COMMON::MUTEX::CThreadMutex m_critical_sec;
    static IPathData* m_data;

    //Patni-AMT/2009Nov16/Added/DeFT#MVC004996
    void FreeResources(char* f_li_fname, char* f_ld_fname, CString f_msg = _T(""));

};
#endif // #ifndef __SM_IPCLIB_PATHDB_H__ 

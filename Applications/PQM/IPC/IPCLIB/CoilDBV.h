//Redmine-783
// CoilDB1.h: interface for the CCoilDBV class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_COILDB_H__
#define __SM_IPCLIB_COILDB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tami/libgpdb/coil.h>
#include <libStudyManager/NonCopyable.h>
//Patni-Rajendra/2011Apr1/Added/TMSC-REDMINE-773
#include <libStudyManager/Mutex/ThreadMutex.h>
class ICoilData;

class CCoilDBV : private MR::ACQ::SM::COMMON::NonCopyable<CCoilDBV>
{
public:

    //Constrcutor / Destrcutor
    CCoilDBV();
    virtual ~CCoilDBV();

    bool Search(ICoilData* f_data, ...);

private:

    static ICoilData* m_data;
    //Patni-Rajendra/2011Apr1/Modified/TMSC-REDMINE-1559
    static MR::ACQ::SM::COMMON::MUTEX::CThreadMutex m_critical_sec;

    //Patni-AMT/2009Nov16/Added/DeFT#MVC004996
    void FreeResources(char* f_li_fname, char* f_ld_fname, CString  f_msg_id = _T(""));

    static void GPDBCallback(void* f_key, cdb_rec_p f_db_rec_p);
};

#endif // #ifndef __SM_IPCLIB_COILDB_H__ 

/**
 * Copyright(c) 2010 TOSHIBA Medical Systems Corporation, All Rights Reserved.
 * @file   VarFieldHandle.h
 *
 * declaration
 * - CVarFieldHandle
 * - CVFHandleScopedLockAndLog
 *
 * @author K. Tanoue
 */
#ifndef __SM_IPCLIB_VARFIELDHANDLE_H__
#define __SM_IPCLIB_VARFIELDHANDLE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tami/libvf/varfields.h>
#include <libStudyManager/NonCopyable.h>
//Patni-Rajendra/2011Mar29/Modified/TMSC-REDMINE-773
#include <libStudyManager/Mutex/ScopedLock.h>
#include "pqmstudy.h"

// forward declaration
namespace MR
{
namespace ACQ
{
namespace SM
{
namespace COMMON
{
class IMutex;
}
}
}
}

/**
 * CVarFieldHandle
 *
 * Access VarField.
 *
 * To use gp libraries functions which access varfield,
 * use Adapter classes which declared in GPLibAdapter.h.
 * Those Adapter classes can be friend to use CVarFieldHandle::Get().
 *
 * If vf name is passed to constructor, vf handle is automatically closed with destructor.
 * If VfDBHandle_t is passed to constructor, vf handle is not close with destructor.
 */
class CVarFieldHandle : private MR::ACQ::SM::COMMON::NonCopyable<CVarFieldHandle>
{
    friend class CLibPLTreeWrapper; // for GetLock()
    friend class CVFHandleScopedLockAndLog; // for GetLock()
    // for Get()
    friend class CPQMDebugInfo; // FIXME this design is not good...
    friend class ClibVfToUiAdapter;
    friend class ClibSARAdapter;
    friend class ClibmsVfAdapter;
    friend class ClibmsAdapter;

public:
    CVarFieldHandle(const CString& vf_name, const int vf_db_type = VFFT_FILE,
                    const int vf_opt = O_RDWR, const bool is_threadsafe = true);
    CVarFieldHandle(const char* const vf_name, const int vf_db_type = VFFT_FILE,
                    const int vf_opt = O_RDWR, const bool is_threadsafe = true);
    CVarFieldHandle(VfDBHandle_t vf_handle, const bool is_threadsafe = true);
    ~CVarFieldHandle();

    void Log(LPCTSTR func_name, LPCTSTR log) const;

    status_t Open(const int max_retry = 10, const int interval_millisec = 500);
    status_t Close();
    status_t ReOpen();
    status_t VuiOpen(const int max_retry = 10, const int interval_millisec = 500);
    status_t VuiClose();
    status_t GetFields(const VfPathElem_t* const path, const int depth,
                       VfFieldSpec_t field_list[], const int field_list_len,
                       int* const num_found);

    status_t PutFields(const VfPathElem_t* const path, const int depth,
                       VfFieldSpec_t field_list[], const int field_list_len,
                       int* const vf_num_put, time_t* const mod_timestamp);

    status_t CreateSubtree(const VfPathElem_t* const path,
                           const int nodeDepth, const VfName_t nodeName,
                           const int initialSize);
    status_t AddBranch(const VfPathElem_t* const path, const int nodeDepth);
    status_t ScanSubtree(const VfPathElem_t* const path,
                         const int nodeDepth, const VfName_t nodeName,
                         bitfield_t* const mask, int* const total,
                         int* const active_num);
    std::string GetName() const;

private:

    VfDBHandle_t m_vf_handle;
    std::string  m_vf_name;
    const int m_vf_db_type;
    const int m_vf_opt;

    bool m_is_opened;
    bool m_is_vui_open;

    const bool m_is_scoped_close; // RAII flag
    //Patni-Rajendra/2011Mar29/Modified/TMSC-REDMINE-1559
    MR::ACQ::SM::COMMON::MUTEX::IMutex* m_mutex;


    status_t OpenImpl(const int max_retry, const int interval_millisec);
    status_t CloseImpl();
    VfDBHandle_t Get();
    //Patni-Rajendra/2011Mar29/Modified/TMSC-REDMINE-1559
    MR::ACQ::SM::COMMON::MUTEX::IMutex* GetLock();


};

/**
 * CVFHandleScopedLockAndLog
 * scoped lock and log for vf handle.
 */
class CVFHandleScopedLockAndLog : private MR::ACQ::SM::COMMON::NonCopyable<CVFHandleScopedLockAndLog>
{
public:
    CVFHandleScopedLockAndLog(CVarFieldHandle* const handle, LPCTSTR log = _T("Scoped Log"));
    ~CVFHandleScopedLockAndLog();

private:
    CVarFieldHandle* const m_handle;
    const LPCTSTR m_log;
    //Patni-Rajendra/2011Mar29/Modified/TMSC-REDMINE-1559
    MR::ACQ::SM::COMMON::MUTEX::CScopedLock m_lock;
};
#endif // #ifndef __SM_IPCLIB_VARFIELDHANDLE_H__

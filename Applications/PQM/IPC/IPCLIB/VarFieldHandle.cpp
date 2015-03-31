/**
 * Copyright(c) 2010 TOSHIBA Medical Systems Corporation, All Rights Reserved.
 * @file   VarFieldHandle.cpp
 * implementation
 * - CVarFieldHandle
 * - CVFHandleScopedLockAndLog
 *
 * @author K. Tanoue
 */
//Redmine-781
#include "stdafx.h"
#include "VarFieldHandle.h"
#include <afxwin.h>
#include <libStudyManager/destroy.h>
#include <libStudyManager/NonCopyable.h>
#include <libStudyManager/Singleton.h>
#include <libStudyManager/Mutex/MutexFactory.h>
//Patni-Rajendra/2011Mar29/Modified/TMSC-REDMINE-773
#include <libStudyManager/Mutex/Mutex.h>
#include <libStudyManager/strcnv.h>
#include <PQM/PQMLogMgr.h>
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"

//+Patni-Rajendra/2011Mar29/Modified/TMSC-REDMINE-1559
using namespace MR::ACQ::SM::COMMON;
using namespace MR::ACQ::SM::COMMON::MUTEX;
//-Patni-Rajendra/2011Mar29/Modified/TMSC-REDMINE-1559

extern "C" {
    status_t VuiDeleteProtocol(VfDBHandle_t, int);
}

#define SCOPED_LOCK_AND_TRACE(log) CVFHandleScopedLockAndLog logobj(this, log)

namespace
{
class CVarFieldHandleMutexFactory : public MR::ACQ::SM::COMMON::Singleton<CVarFieldHandleMutexFactory>
{
    friend class MR::ACQ::SM::COMMON::Singleton<CVarFieldHandleMutexFactory>;
public:
    MR::ACQ::SM::COMMON::MUTEX::IMutex* GetMutex(const bool is_threadsafe) {
        return is_threadsafe
               ? &m_thread_mutex
               : MR::ACQ::SM::COMMON::MUTEX::CMutexFactory::GetNullMutex();
    }

private:
    MR::ACQ::SM::COMMON::MUTEX::CThreadMutex m_thread_mutex;

    CVarFieldHandleMutexFactory()
        : m_thread_mutex()
    {}

    ~CVarFieldHandleMutexFactory()
    {}
};
} //namespace

/**
 * Constructor for CString
 */
CVarFieldHandle::CVarFieldHandle(const CString& vf_name, const int vf_db_type,
                                 const int vf_opt, const bool is_threadsafe)
    : m_vf_handle(NULL)
    , m_vf_name("")
    , m_vf_db_type(vf_db_type)
    , m_vf_opt(vf_opt)
    , m_is_opened(false)
    , m_is_vui_open(false)
    , m_is_scoped_close(true)
    , m_mutex(NULL)
{
    UTIL::Wcs2Mbs(&m_vf_name, vf_name);

    m_mutex = CVarFieldHandleMutexFactory::GetInstance()->GetMutex(is_threadsafe);
}

/**
 * Constructor for char
 */
CVarFieldHandle::CVarFieldHandle(const char* vf_name, const int vf_db_type,
                                 const int vf_opt, const bool is_threadsafe)
    : m_vf_handle(NULL)
    , m_vf_name(vf_name)
    , m_vf_db_type(vf_db_type)
    , m_vf_opt(vf_opt)
    , m_is_opened(false)
    , m_is_vui_open(false)
    , m_is_scoped_close(true)
    , m_mutex(NULL)
{
    m_mutex = CVarFieldHandleMutexFactory::GetInstance()->GetMutex(is_threadsafe);
}

/**
 * Constructor for VfDBHandle_t
 */
CVarFieldHandle::CVarFieldHandle(VfDBHandle_t vf_handle, const bool is_threadsafe)
    : m_vf_handle(vf_handle)
    , m_vf_name("")
    , m_vf_db_type(-1)
    , m_vf_opt(-1)
    , m_is_opened(true)
    , m_is_vui_open(false)
    , m_is_scoped_close(false)
    , m_mutex(NULL)
{
    m_mutex = CVarFieldHandleMutexFactory::GetInstance()->GetMutex(is_threadsafe);
}

/**
 * Destructor
 */
CVarFieldHandle::~CVarFieldHandle()
{
    if (m_is_scoped_close) {
        if (m_is_vui_open) {
            this->VuiClose();

        } else {
            this->Close();
        }
    }
}

/**
 * Output Log with handle address and error no
 * @param func_name [in]
 * @param log [in]
 */
void CVarFieldHandle::Log(LPCTSTR func_name, LPCTSTR log) const
{
    CString l_strlog(_T(""));
    l_strlog.Format(_T("[handle:0x%x][errno:%d][GetLastError:%d]"),
                    m_vf_handle, errno, GetLastError());
    PQM_TRACE(USER_FUNC_MARKER, func_name, l_strlog + log);
}

/**
 * Open
 * if open is failed, retry max_retry times with interval.
 * @param max_retry [in]
 * @param interval_millisec [in]
 * @return E_NO_ERROR or E_ERROR
 */
status_t CVarFieldHandle::Open(const int max_retry, const int interval_millisec)
{
    m_is_vui_open = false;
    return this->OpenImpl(max_retry, interval_millisec);
}

/**
 * Close()
 * @return E_NO_ERROR or E_ERROR
 */
status_t CVarFieldHandle::Close()
{
    return this->CloseImpl();
}

/**
 * ReOpen()
 * @return E_NO_ERROR or E_ERROR
 */
status_t CVarFieldHandle::ReOpen()
{
    LPCTSTR FUNC_NAME = _T("CVarFieldHandle::ReOpen");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    //Reset errno
    SET_ERRNO(0);
    SetLastError(0);

    status_t sts = this->Close();

    if (sts != E_NO_ERROR) {
        return sts;
    }

    return this->Open();
}

/**
 * VuiOpen
 * if open is failed, retry max_retry times with interval.
 * @param max_retry [in]
 * @param interval_millisec [in]
 * @return E_NO_ERROR or E_ERROR
 */
status_t CVarFieldHandle::VuiOpen(const int max_retry, const int interval_millisec)
{
    m_is_vui_open = true;
    return this->OpenImpl(max_retry, interval_millisec);
}

/**
 * VuiClose()
 * @return E_NO_ERROR or E_ERROR
 */
status_t CVarFieldHandle::VuiClose()
{
    return this->CloseImpl();
}

/**
 * GetFields
 *
 * Return the VarField data of specified tags.
 *
 * @param path [in]
 * @param depth [in]
 * @param field_list [inout]
 * @param field_list_len [in]
 * @param num_found [out]
 * @return E_NO_ERROR or E_ERROR
 */
status_t CVarFieldHandle::GetFields(const VfPathElem_t* const  path, const int depth,
                                    VfFieldSpec_t* const field_list, const int field_list_len,
                                    int* const num_found)
{
    LPCTSTR FUNC_NAME = _T("CVarFieldHandle::GetFields");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {
        return ::VfGetFields(m_vf_handle, const_cast<VfPathElem_t*>(path), depth,
                             field_list, field_list_len, num_found);

    } catch (...) {
        this->Log(FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * PutFields
 *
 * Write VarField tag to the VarField file.
 *
 * @param path [in]
 * @param depth [in]
 * @param field_list [inout]
 * @param field_list_len [in]
 * @param vf_num_put [out]
 * @param mod_timestamp [out]
 * @return E_NO_ERROR or E_ERROR
 */
status_t CVarFieldHandle::PutFields(const VfPathElem_t* const path, const int depth,
                                    VfFieldSpec_t* const field_list, const int field_list_len,
                                    int* const vf_num_put, time_t* const mod_timestamp)
{
    LPCTSTR FUNC_NAME = _T("CVarFieldHandle::PutFields");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {
        return ::VfPutFields(m_vf_handle, const_cast<VfPathElem_t*>(path), depth,
                             field_list, field_list_len, vf_num_put, mod_timestamp);

    } catch (...) {
        this->Log(FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * CreateSubtree
 *
 * Returns no. of nodes present under specified parent node.
 *
 * @param path [in]
 * @param nodeDepth [in]
 * @param nodeName [in]
 * @param mask [out]
 * @param total [out]
 * @param active_num [out]
 * @return E_NO_ERROR or E_ERROR
 */
status_t CVarFieldHandle::CreateSubtree(const VfPathElem_t* const path,
                                        const int nodeDepth, const VfName_t nodeName,
                                        const int initialSize)
{
    LPCTSTR FUNC_NAME = _T("CVarFieldHandle::CreateSubtree");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {
        return ::VfCreateSubtree(m_vf_handle, const_cast<VfPathElem_t*>(path),
                                 nodeDepth, nodeName, initialSize);

    } catch (...) {
        this->Log(FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * AddBranch
 *
 * Add branch
 *
 * @param path [in]
 * @param nodeDepth [in]
 * @return E_NO_ERROR or E_ERROR
 */
status_t CVarFieldHandle::AddBranch(const VfPathElem_t* const path, const int nodeDepth)
{
    LPCTSTR FUNC_NAME = _T("CVarFieldHandle::AddBranch");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    try {
        return ::VfAddBranch(m_vf_handle, const_cast<VfPathElem_t*>(path), nodeDepth);

    } catch (...) {
        this->Log(FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * ScanSubtree
 *
 * Returns no. of nodes present under specified parent node.
 *
 * @param path [in]
 * @param nodeDepth [in]
 * @param nodeName [in]
 * @param mask [out]
 * @param total [out]
 * @param active_num [out]
 * @return E_NO_ERROR or E_ERROR
 */
status_t CVarFieldHandle::ScanSubtree(const VfPathElem_t* const path,
                                      const int nodeDepth, const VfName_t nodeName,
                                      bitfield_t* const mask, int* const total,
                                      int* const active_num)
{
    LPCTSTR FUNC_NAME = _T("CVarFieldHandle::ScanSubtree");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);
	

	
    try {
        return ::VfScanSubtree(m_vf_handle, const_cast<VfPathElem_t*>(path),
                               nodeDepth, nodeName,
                               mask, total, active_num);

    } catch (...) {
        this->Log(FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * GetName
 *
 * Get study name
 * @return study name
 */
std::string CVarFieldHandle::GetName() const
{
    return this->m_vf_name;
}

/**
 * OpenImpl
 * if open is failed, retry max_retry times with interval.
 * @param max_retry [in]
 * @param interval_millisec [in]
 * @return E_NO_ERROR or E_ERROR
 */
status_t CVarFieldHandle::OpenImpl(const int max_retry, const int interval_millisec)
{
    LPCTSTR FUNC_NAME = m_is_vui_open ?
                        _T("CVarFieldHandle::VuiOpen") : _T("CVarFieldHandle::Open");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    if (m_is_opened) {
        this->Log(FUNC_NAME, _T("Already Opened."));
        return E_NO_ERROR;
    }

    VfDBName_t name = {0};
    name.filename = const_cast<char*>(m_vf_name.c_str());

    status_t sts = E_NO_ERROR;

    CString l_strlog(_T(""));

    for (int i = 0; i < max_retry; i++) {
        try {
            if (m_is_vui_open) {
                m_vf_handle = ::VuiOpen(name.filename, m_vf_opt);
                sts = m_vf_handle ? E_NO_ERROR : E_ERROR;

            } else {
                sts = ::VfOpen(m_vf_db_type, name, m_vf_opt, &m_vf_handle);
            }

        } catch (...) {
            this->Log(FUNC_NAME, _T("EXCEPTION"));
            throw;
        }


        l_strlog.Format(_T("[count:%d/%d][status:%d] "),
                        i + 1, max_retry, sts);
        this->Log(FUNC_NAME, l_strlog + CString(m_vf_name.c_str()));

        if (sts == E_NO_ERROR) {
            m_is_opened = true;
            return sts;
        }

        Sleep(interval_millisec);
    }

    return sts;
}

/**
 * CloseImpl()
 * @return E_NO_ERROR or E_ERROR
 */
status_t CVarFieldHandle::CloseImpl()
{
    LPCTSTR FUNC_NAME = m_is_vui_open ?
                        _T("CVarFieldHandle::VuiClose") : _T("CVarFieldHandle::Close");
    SCOPED_LOCK_AND_TRACE(FUNC_NAME);

    if (!m_is_opened) {
        this->Log(FUNC_NAME, _T("Already Closed."));
        return E_NO_ERROR;
    }

    try {
        status_t sts = E_NO_ERROR;

        if (m_is_vui_open) {
            sts = ::VuiClose(m_vf_handle);

        } else {
            sts = ::VfClose(m_vf_handle);
        }

        m_is_opened = (sts == E_NO_ERROR) ? false : true;

        if (m_is_opened) {
            this->Log(FUNC_NAME, _T("Close Failed"));

        } else {
            m_vf_handle = NULL;
        }

        return sts;

    } catch (...) {
        this->Log(FUNC_NAME, _T("EXCEPTION"));
        throw;
    }
}

/**
 * Get(). Only GPLibAdapter classes can use it.
 * @return VfDBHandle
 */
VfDBHandle_t CVarFieldHandle::Get()
{
    return m_vf_handle;
}

/**
 * GetLock(). Only CVFHandleScopedLockAndLog can use it.
 * @return sync object.
 */
//Patni-Rajendra/2011Mar29/Modified/TMSC-REDMINE-1559
MR::ACQ::SM::COMMON::MUTEX::IMutex* CVarFieldHandle::GetLock()
{
    return m_mutex;
}

/**
 * Constructor
 * @param handle [in]
 * @param log [in]
 */
CVFHandleScopedLockAndLog::CVFHandleScopedLockAndLog(CVarFieldHandle* const handle, LPCTSTR log)
    : m_handle(handle)
    , m_log(log)
    , m_lock(handle ? handle->GetLock() : NULL, CPQMLogMgr::GetInstance())
{
    if (m_handle) {
        m_handle->Log(m_log, _T("[Enter]"));
    }
}

/**
 * Destructor
 */
CVFHandleScopedLockAndLog::~CVFHandleScopedLockAndLog()
{
    if (m_handle) {
        m_handle->Log(m_log, _T("[Leave]"));
    }
}

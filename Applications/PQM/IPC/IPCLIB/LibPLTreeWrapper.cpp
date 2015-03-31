// LibPLTreeWrapper.cpp: implementation of the CLibPLTreeWrapper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LibPLTreeWrapper.h"
#include <libStudyManager/Mutex/ThreadMutex.h>
#include <PQM/PQMLogMgr.h>
#include "PQMStudy.h"
#include "VarFieldHandle.h"

#define MAX_SIZE 256
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//***************************Method Header*************************************
//Method Name    : CLibPLTreeWrapper
//Author         : PATNI
//Purpose        :
//*****************************************************************************
CLibPLTreeWrapper::CLibPLTreeWrapper(CVarFieldHandle* const f_vfdb_handle) :
    m_module(0),
    m_prolim_private_wrapper(NULL),
    m_mutex(f_vfdb_handle->GetLock())
{
    m_mutex->Lock();
}

//***************************Method Header*************************************
//Method Name    : ~CLibPLTreeWrapper
//Author         : PATNI
//Purpose        :
//*****************************************************************************
CLibPLTreeWrapper::~CLibPLTreeWrapper()
{
    UnLoadProlimLibrary();
    m_mutex->Unlock();
}

//***************************Method Header*************************************
//Method Name    : ExecutePlUpdate
//Author         : PATNI
//Purpose        :
//*****************************************************************************
bool CLibPLTreeWrapper::ExecutePlUpdate(CPQMStudy* const f_pqmstudy, const int f_protocol, char* const f_scratch_pad
                                       )
{
    LPCTSTR FUNC_NAME = _T("CLibPLTreeWrapper::ExecutePlUpdate");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    m_module = LoadLibrary(_T("libPlTreePvtWrapper.dll"));

    bool l_status = false;

    if (INVALID_HANDLE_VALUE == m_module || NULL == m_module) {
        m_prolim_private_wrapper = NULL;
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed to load Prolim Library"));
        return l_status;
    }

    typedef CLibPlTreePvtWrapper* (*FUNC_PTR_INSTANCE)(void);
    FUNC_PTR_INSTANCE l_instance_ptr = (FUNC_PTR_INSTANCE)GetProcAddress(m_module, "GetInstance");

    if (l_instance_ptr != NULL) {
        m_prolim_private_wrapper = l_instance_ptr();

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Could not get the instance of Prolim class..!!"));
        return l_status;
    }

    CString l_sequence_name = f_pqmstudy->GetSequenceName(f_protocol);
    wchar_t* l_seq_name = l_sequence_name.GetBuffer(MAX_SIZE);

    CString l_seq_arch =  _T("");
    f_pqmstudy->GetSequenceArchitecture(f_protocol, l_seq_arch);
    wchar_t* l_seq_archtre = l_seq_arch.GetBuffer(MAX_SIZE);



    std::wstring l_path;
    l_status = m_prolim_private_wrapper->ExecutePlUpdate(l_seq_name, l_seq_archtre, &l_path, f_scratch_pad);


    if (l_status) {
        CString l_exe_path(l_path.c_str());
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Path of the spawned exe is ") + l_exe_path);

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Error occurred in spawning the PlUpdate EXE or loading libplPvt.dl"));
    }

    l_sequence_name.ReleaseBuffer();
    l_seq_arch.ReleaseBuffer();

    return l_status;
}

//***************************Method Header*************************************
//Method Name    : UnLoadProlimLibrary
//Author         : PATNI
//Purpose        :
//*****************************************************************************
void CLibPLTreeWrapper::UnLoadProlimLibrary(
)
{
    LPCTSTR FUNC_NAME = _T("CLibPLTreeWrapper::UnLoadProlimLibrary");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    typedef void(*PFUNC_DelInstance)(void);
    PFUNC_DelInstance delinstance_ptr = (PFUNC_DelInstance)GetProcAddress(m_module, "DeleteInstance");

    if (delinstance_ptr != NULL) {
        delinstance_ptr();
    }

    FreeLibrary(m_module);
    m_prolim_private_wrapper = NULL;
    m_module = NULL;
}

bool CLibPLTreeWrapper::ExecutePlUpdateWrapper(CPQMStudy* const f_pqm_study, const int f_protocol, char* const f_scratch_pad)
{
    return ExecutePlUpdate(f_pqm_study, f_protocol, f_scratch_pad);
}


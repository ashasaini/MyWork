// LibPLTreeWrapper.h: interface for the CLibPLTreeWrapper class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_LIBPLTREE_WRAPPER_H__
#define __SM_IPCLIB_LIBPLTREE_WRAPPER_H__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <libStudyManager/NonCopyable.h>
#include <LibPlTreePvtWrapper\LibPlTreePvtWrapper.h>

namespace MR
{
namespace ACQ
{
namespace SM
{
namespace COMMON
{
namespace MUTEX
{
class IMutex;
}
}
}
}
}

class CLibPlTreePvtWrapper;
class CVarFieldHandle;
class CPQMStudy;


class CLibPLTreeWrapper :
    private MR::ACQ::SM::COMMON::NonCopyable<CLibPLTreeWrapper>
{
public:
    explicit CLibPLTreeWrapper(CVarFieldHandle* const f_vfdb_handle);
    virtual ~CLibPLTreeWrapper();
    bool ExecutePlUpdateWrapper(CPQMStudy* const f_pqm_study, const int f_protocol, char* const f_scratch_pad);
private :
    HMODULE m_module;
    CLibPlTreePvtWrapper* m_prolim_private_wrapper;

    MR::ACQ::SM::COMMON::MUTEX::IMutex* m_mutex;

    bool ExecutePlUpdate(CPQMStudy* const f_pqmstudy, const int f_protocol, char* const f_scratch_pad);
    void UnLoadProlimLibrary();
};

#endif // #ifndef __SM_IPCLIB_LIBPLTREE_WRAPPER_H__

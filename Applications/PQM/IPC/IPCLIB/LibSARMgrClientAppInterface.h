// LibSARMgrClientAppInterface.h: interface for the CApplicationManageInterface class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_LIBSARMGRCLIENTAPPINTERFACE_H_
#define __SM_IPCLIB_LIBSARMGRCLIENTAPPINTERFACE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>
#include <libSARManager/LibSARMgrClientApp.h>

class CPqmPmSARManager;

class CLibSARMgrClientAppInterface  : public LIBSARMGR::CLibSARMgrClientApp
    , private MR::ACQ::SM::COMMON::NonCopyable<CLibSARMgrClientAppInterface>
{

public:
    explicit CLibSARMgrClientAppInterface(CPqmPmSARManager* f_pqm_pm_sar_manager);
    virtual ~CLibSARMgrClientAppInterface();

    virtual void OnSARMgrDisconnect()const;

private:
    CPqmPmSARManager* m_pqm_pm_sar_manager;

};

#endif // __SM_IPCLIB_LIBSARMGRCLIENTAPPINTERFACE_H_

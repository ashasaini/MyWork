// LibSARMgrClientAppInterface.cpp: implementation of the CLibSARMgrClientAppInterface class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LibSARMgrClientAppInterface.h"
#include "PqmPmSARManager.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLibSARMgrClientAppInterface::CLibSARMgrClientAppInterface(CPqmPmSARManager* f_pqm_pm_sar_manager)
    : m_pqm_pm_sar_manager(f_pqm_pm_sar_manager)
{
}

CLibSARMgrClientAppInterface::~CLibSARMgrClientAppInterface()
{
}


void CLibSARMgrClientAppInterface::OnSARMgrDisconnect()const
{

    m_pqm_pm_sar_manager->OnSARMgrDisconnect();
}
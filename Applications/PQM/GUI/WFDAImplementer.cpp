// WFDAImplementer.cpp: implementation of the CWFDAImplementer class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "WFDAImplementer.h"
#include "WFDAHandler.h"
#include "WFDAHandlerInterface.h"




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//************************************Method Header************************************
// Method Name  : CWFDAImplementer
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CWFDAImplementer::CWFDAImplementer()
{
    //m_pqm = NULL ;
    m_wfda_handler = NULL ;
}


//************************************Method Header************************************
// Method Name  : ~CWFDAImplementer
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CWFDAImplementer::~CWFDAImplementer()
{
    DEL_PTR(m_wfda_handler);
}

//************************************Method Header************************************
// Method Name  : CWFDAImplementer
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CWFDAImplementer::CWFDAImplementer(
    CPQMView* f_pqm_view
)
{
    m_wfda_handler = new CWFDAHandler(f_pqm_view);
}

//************************************Method Header************************************
// Method Name  : NewSequencesAdded
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CWFDAImplementer::NewSequencesAdded(
    const int f_count
)const
{

    if (m_wfda_handler) {

        return m_wfda_handler->NewSequencesAdded(f_count);
    }

    ASSERT(FALSE);
    return false;
}

//************************************Method Header************************************
// Method Name  : DuplicateProtocol
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CWFDAImplementer::DuplicateProtocol(
    const int f_duplicate_index
)const
{

    if (m_wfda_handler) {

        return m_wfda_handler->DuplicateProtocol(f_duplicate_index);
    }

    ASSERT(FALSE);
    return false;
}

//************************************Method Header************************************
// Method Name  : ScanStatusChanged
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CWFDAImplementer::ScanStatusChanged(
    const int f_acq_order,
    const WFDA_Protocol_Status f_status
)const
{
    if (m_wfda_handler) {

        return m_wfda_handler->ScanStatusChanged(f_acq_order, f_status);
    }

    ASSERT(FALSE);
    return false;
}

bool CWFDAImplementer::IsProtocolInscan(
    const int f_acq_order
)const
{
    if (m_wfda_handler) {
        return m_wfda_handler->IsProtocolInScan(f_acq_order);
    }

    return false;
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CWFDAHandlerInterface* CWFDAImplementer::GetWFDAHandlerInterface(
) const
{

    return (CWFDAHandlerInterface*) m_wfda_handler;
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CWFDAImplementer::OnScanEditStatusChangedForProtocol(
    const int f_acq_order,
    const WFDA_Protocol_Status f_wfda_protocol_status
)const
{

    if (m_wfda_handler) {

        return m_wfda_handler->OnScanEditStatusChangedForProtocol(f_acq_order, f_wfda_protocol_status);
    }

    ASSERT(FALSE);
    return false;
}

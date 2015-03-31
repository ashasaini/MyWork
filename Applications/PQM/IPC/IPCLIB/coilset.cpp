//Redmine-783
//Redmine-780
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: coilset.cpp
 *  Overview: Implementation of CCoilSet class.
 *  Last Modified: 2007/8/10 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2007/6/15 12:00:00     PQM5 Baselined code
 *   2.0          Patni            2007/8/10 12:00:00     Implemented PQM6 requirements & Formatted
 *
 *****************************************************************************/
#include "StdAfx.h"
#include "CoilSet.h"
#include <PQM/PQMLogMgr.h>
#include "Coil.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//****************************Method Header************************************
//Method Name   :CCoilSet
//Author        :PATNI/MRP
//Purpose       :Default Constructor
//*****************************************************************************
CCoilSet::CCoilSet(
): m_tx_coil(NULL),
    m_rcv_coil(NULL)
{
}

//****************************Method Header************************************
//Method Name   :~CCoilSet
//Author        :PATNI/MRP
//Purpose       :Default Destructor
//*****************************************************************************
CCoilSet::~CCoilSet(
)
{
}

//****************************Method Header************************************
//Method Name   :GetTransmitCoil
//Author        :PATNI/MRP
//Purpose       :Getting the value of transmit coil
//*****************************************************************************
CCoil* const CCoilSet::GetTransmitCoil(
)const
{
    LPCTSTR FUNC_NAME = _T("CCoilSet::GetTransmitCoil");

    //  GetCoilUId() is replaced by GetCoilKey()
    if (m_tx_coil) {
        CString str_msg(_T(""));
        str_msg.Format(_T("%ld"), (m_tx_coil) ? m_tx_coil->GetCoilKey() : 0);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

        return m_tx_coil;

    } else {
        return NULL;
    }
}

//****************************Method Header************************************
//Method Name   :SetTransmitCoil
//Author        :PATNI/MRP
//Purpose       :Setting the value of transmit coil
//*****************************************************************************
void CCoilSet::SetTransmitCoil(
    CCoil* f_tx_coil
)
{
    LPCTSTR FUNC_NAME = _T("CCoilSet::SetTransmitCoil");
    //  GetCoilUId() is replaced by GetCoilKey()

    CString str_msg(_T(""));
    str_msg.Format(_T("%ld"), (f_tx_coil) ? f_tx_coil->GetCoilKey() : 0);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    m_tx_coil = f_tx_coil;
}

//****************************Method Header************************************
//Method Name   :GetReceiveCoil
//Author        :PATNI/MRP
//Purpose       :Getting the value of receive coil
//*****************************************************************************
CCoil* const CCoilSet::GetReceiveCoil(
)const
{
    LPCTSTR FUNC_NAME = _T("CCoilSet::GetReceiveCoil");
    //  GetCoilUId() is replaced by GetCoilKey()

    if (m_rcv_coil) {
        CString str_msg(_T(""));
        str_msg.Format(_T("%ld"), (m_rcv_coil) ? m_rcv_coil->GetCoilKey() : 0);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

        return m_rcv_coil;

    } else  {
        return NULL;
    }
}

//****************************Method Header************************************
//Method Name   :SetReceiveCoil
//Author        :PATNI/MRP
//Purpose       :Setting the value of receive coil
//*****************************************************************************
void CCoilSet::SetReceiveCoil(
    CCoil* f_rcv_coil
)
{
    LPCTSTR FUNC_NAME = _T("CCoilSet::SetReceiveCoil");
    //  GetCoilUId() is replaced by GetCoilKey()

    CString str_msg(_T(""));
    str_msg.Format(_T("%ld"), (f_rcv_coil) ? f_rcv_coil->GetCoilKey() : 0);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    m_rcv_coil = f_rcv_coil;
}


//TMSC-REDMINE-782
//TMSC-REDMINE-781
//TMSC-REDMINE-780
//TMSC-REDMINE-778
//TMSC-REDMINE-777
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: IPCComManager.cpp
 *  Overview: Implementation of CIPCComManager class.
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
#include "stdafx.h"
#include "IPCComManager.h"
#include "PQMSequence.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

Data_decl_list  g_sequence_param_name_list;

//************************************Method Header**************************************
//Method Name:  CIPCComManager
//Author:  PATNI
//Purpose: Constructor, initialize the total no. of sequence to zero.
//**************************************************************************************/
CIPCComManager::CIPCComManager(
)
{

}

//************************************Method Header**************************************
//Method Name:  ~CIPCComManager
//Author:  PATNI
//Purpose: Destructor.
//**************************************************************************************/
CIPCComManager::~CIPCComManager(
)
{
}

//************************************Method Header**************************************
//Method Name:  GetList
//Author:  PATNI/DJ
//Purpose: Copy the local seq. list object to the input reference parameter list object.
//**************************************************************************************/
bool CIPCComManager::GetList(
    CSeqList* objlist, const long maxlimit
)
{
    LPCTSTR FUNC_NAME = _T("CIPCComManager::GetList");

    CString str;
    str.Format(L"Series Count = %d", m_rec_seq_list.GetCount());

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str);

    bool max_limit_status = true;
    POSITION pos = m_rec_seq_list.GetHeadPosition();

    while (pos) {

        if (objlist->GetCount() < maxlimit) {
            CPASCOMSequence* pseq = m_rec_seq_list.GetNext(pos);
            objlist->AddTail(new CSequence(pseq));

        } else {
            max_limit_status = false;
            break;
        }
    }

    // Sarjeet ++
    m_rec_seq_list.RemoveAll();
    // Sarjeet --

    return max_limit_status;
}

//************************************Method Header**************************************
//Method Name:  GetSendSeqList()
//Author:  Rajendra
//Purpose: Get send sequence list.
//**************************************************************************************/
SequenceList& CIPCComManager::GetSendSeqList()
{

    return m_snd_seq_list;
}

//************************************Method Header**************************************
//Method Name:  GetRecSeqList()
//Author:  Rajendra
//Purpose: Get receive sequence list.
//**************************************************************************************/
SequenceList& CIPCComManager::GetRecSeqList()
{

    return m_rec_seq_list;
}

//************************************Method Header**************************************
//Method Name:  SetSndSequence
//Author:  PATNI/DJ
//Purpose: Set the send sequence list.
//**************************************************************************************/
void CIPCComManager::SetSndSequence(
    CSequence* pseq
)
{
    LPCTSTR FUNC_NAME = _T("CIPCComManager::SetSndSequence");


    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("Trace of SetSndSequence"));


    CPASCOMSequence* passcom = new CPASCOMSequence(*(pseq->GetCPASCOMSequence()));
    pseq->SetCPASCOMSequence(passcom);
    m_snd_seq_list.AddTail(passcom);
}



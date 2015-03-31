//REDMINE-1213_Update_29_APR
//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-780
//Redmine-781
//Redmine-782
// PqmAuditLog.cpp: implementation of the CPqmAuditLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PqmAuditLog.h"
#include <MPlusSec/MPlusAuditClient.h>
#include <PQM/PQMLogMgr.h>
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"

using namespace MPlus::Audit;
using namespace IHEYr5;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//+Patni-PJS/2010May09/Added/JaFT# IR-141
CPqmAuditLog::CPqmAuditLog() :
    m_audit_client(NULL),
    m_patient_id(_T("")),
    m_study_id(_T("")),
    m_mplus_user_name(_T(""))
{

    LPCTSTR FUNC_NAME = _T("CPqmAuditLog::CPqmAuditLog");

    if (!StartupAuditClient()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Audit client start up failed"));
    }
}

CPqmAuditLog::~CPqmAuditLog()
{
    LPCTSTR FUNC_NAME = _T("CPqmAuditLog::~CPqmAuditLog");

    if (!ShutDownAuditClient()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("ShutDownAuditClient failed"));
    }
}

//*****************************************************************************
//Method Name   : RaiseOrderRecordAuditEvent
//Author        : Patni/PJS
//Purpose       : This function is used to raise OrderRecord Audit log event
//*****************************************************************************
bool CPqmAuditLog::RaiseOrderRecordAuditEvent(
)
{
    LPCTSTR FUNC_NAME = _T("CPqmAuditLog::RaiseOrderRecordAuditEvent");

    CPhiAdpSimple l_phiAdaptor(m_patient_id, //PatientId
                               _T(""),   //PatientName
                               _T(""),   //UID
                               _T(""),   //AccessionNumber
                               0,//NumberOfInstances,
                               _T(""));
    //Creating object of COptionMessageCommon
    COptionMessageCommon l_optionMsg_cmn(_T(""));

    //Create Order event
    CEvOrderRecordEvent l_order_event(m_mplus_user_name,
                                      ((IPhiAdaptor*)&l_phiAdaptor),
                                      _T("R"));

    if (m_audit_client == NULL) {
        //Trace log
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_audit_client pointer is NULL"));
        return false;
    }

    //Raise Event
    return m_audit_client->RaiseAuditEvent(&l_order_event);
}

//*******************************************************************************
//Method Name   : RaiseInstancesAuditEvent
//Author        : Patni/PJS
//Purpose       : This function is used to raise event for DICOM instance created
//*******************************************************************************
bool CPqmAuditLog::RaiseInstancesAuditEvent(
)
{
    LPCTSTR FUNC_NAME = _T("CPqmAuditLog::RaiseOrderRecordAuditEvent");

    CPhiAdpSimple l_phiAdaptor(m_patient_id, //PatientId
                               _T(""),   //PatientName
                               m_study_id, //UID
                               _T(""),   //AccessionNumber
                               0,//NumberOfInstances,
                               _T(""));
    //Creating object of COptionMessageCommon
    COptionMessageCommon l_optionMsg_cmn(_T("Message"));

    //Create Order event
    CEvInstancesUsed l_instances_event(m_mplus_user_name,
                                       ((IPhiAdaptor*)&l_phiAdaptor),
                                       _T(""),
                                       _T("U"));

    if (m_audit_client == NULL) {
        //Trace log
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_audit_client pointer is NULL"));
        return false;
    }

    //Raise Event
    return m_audit_client->RaiseAuditEvent(&l_instances_event);
}

//*****************************************************************************
//Method Name   : SetUsername
//Author        : Patni/PJS
//Purpose       :
//*****************************************************************************
void CPqmAuditLog::SetUsername(
    const CString& f_usr_name
)
{
    m_mplus_user_name = f_usr_name;
}

//*****************************************************************************
//Method Name   : SetStudyDetails
//Author        : Patni/PJS
//Purpose       : This function is used to set patient and Study ID
//*****************************************************************************
void CPqmAuditLog::SetStudyDetails(
    const CString& f_patient_id, const CString& f_study_id
)
{
    m_patient_id = f_patient_id;
    m_study_id = f_study_id;
}

//*****************************************************************************
//Method Name   : StartupAuditClient
//Author        : Patni/PJS
//Purpose       : This function is used to start up the Audit server and client
//*****************************************************************************
bool CPqmAuditLog::StartupAuditClient(
)
{
    LPCTSTR FUNC_NAME = _T("CPqmAuditLog::StartupAuditClient");
    //Initializing COM object
    CoInitializeEx(0, COINIT_MULTITHREADED);

    m_audit_client = CAuditClient::Instance();

    if (m_audit_client == NULL) {
        //Trace log
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_audit_client pointer is NULL"));
        return false;
    }

    //Start up client
    return m_audit_client->StartupClient();
}

//*****************************************************************************
//Method Name   : ShutDownAuditClient
//Author        : Patni/PJS
//Purpose       : This function is used to shutdown the Audit cllient
//*****************************************************************************
bool CPqmAuditLog::ShutDownAuditClient(
)const
{
    LPCTSTR FUNC_NAME = _T("CPqmAuditLog::ShutDownAuditClient");

    if (m_audit_client == NULL) {
        //Trace log
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_audit_client pointer is NULL"));
        return false;
    }

    return m_audit_client->ShutdownClient();
}

//*****************************************************************************
//Method Name   : GetUserName
//Author        : Patni/PJS
//Purpose       :
//*****************************************************************************
CString CPqmAuditLog::GetUserName(
) const
{
    return m_mplus_user_name;
}
//-Patni-PJS/2010May09/Added/JaFT# IR-141
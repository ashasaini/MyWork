//Redmine-783
//Redmine-780
// PqmAuditLog.h: interface for the CPqmAuditLog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_PQMAUDITLOG_H__
#define __SM_IPCLIB_PQMAUDITLOG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>

namespace MPlus
{
namespace Audit
{
class CAuditClient;
}
}

//+Patni-PJS/2010May09/Added/JaFT# IR-141
class CPqmAuditLog : private MR::ACQ::SM::COMMON::NonCopyable<CPqmAuditLog>
{

public:

    //Constructor / Destructor
    CPqmAuditLog();
    virtual ~CPqmAuditLog();

    bool  RaiseOrderRecordAuditEvent();
    bool  RaiseInstancesAuditEvent();

    void  SetUsername(const CString& f_usr_name);
    void  SetStudyDetails(const CString& f_patient_id, const CString& f_study_id);

private:

    MPlus::Audit::CAuditClient* m_audit_client;
    CString      m_patient_id;
    CString      m_study_id;
    CString      m_mplus_user_name;

    bool  StartupAuditClient();
    bool  ShutDownAuditClient()  const;

    CString GetUserName() const;

};
//-Patni-PJS/2010May09/Added/JaFT# IR-141
#endif // #ifndef __SM_IPCLIB_PQMAUDITLOG_H__ 

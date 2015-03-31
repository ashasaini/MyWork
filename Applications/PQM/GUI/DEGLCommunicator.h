//Redmine-783
//Redmine-777
#ifndef __SM_DEGL_COMMUNICATOR_H__
#define __SM_DEGL_COMMUNICATOR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>

#include <libStudyManager/NonCopyable.h>

// pre-declaration
namespace MPLUS
{
namespace FW
{
namespace COMM
{
class IMplusBroadcastConnector;
}
}
}


class  CDEGLCommunicator: private MR::ACQ::SM::COMMON::NonCopyable<CDEGLCommunicator>
{
public:

    static CDEGLCommunicator* GetInstance();

    bool SendDeleteReserve(const wchar_t* const f_studyoid);
    bool SendDeleteComplete(const wchar_t* const f_studyoid);
    //Patni-DKH/2011Feb08/Added/REDMINE-1434
    bool SendCurrentExamUpdate(const wchar_t* const f_patient_oid,
                               const wchar_t* const f_studyoid);

    static void DeleteCommunicatorIntance();

private:

    static CDEGLCommunicator* m_connector;

    //TMSC-Tanoue/2011Feb14/Modified/REDMINE-1434
    typedef std::map<DWORD, MPLUS::FW::COMM::IMplusBroadcastConnector*> CFW_BC_Map_t;
    CFW_BC_Map_t m_cfw_connectors;
    bool m_is_connected;

    CDEGLCommunicator();
    virtual ~CDEGLCommunicator();

    //TMSC-Tanoue/2011Feb14/Modified/REDMINE-1434
    MPLUS::FW::COMM::IMplusBroadcastConnector* CreateConnector();
    //+Patni-DKH/2011Feb08/Modified/REDMINE-1434
    bool SendEvent(const wchar_t* const f_patient_oid,
                   const wchar_t* const f_studyoid,
                   const wchar_t* const custom);

};

#endif /* end of include guard: __SM_DEGL_COMMUNICATOR_H__ */

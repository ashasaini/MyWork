//TMSC-REDMINE-781
//TMSC-REDMINE-777
#include "stdafx.h"
#include "DEGLCommunicator.h"

CDEGLCommunicator* CDEGLCommunicator::m_connector = NULL;

//TMSC-Tanoue/2011Feb14/Modified/REDMINE-1434
using MPLUS::FW::COMM::IMplusBroadcastConnector;

namespace CFW_BC
{
static const wchar_t CHANNEL[] = _T("BC_MR_IRS");
static const wchar_t SENDER[] = _T("Acquire.PQM");
static const wchar_t CMD[] = _T("IRS_EF_EXAM_UPDATE");
namespace CUSTOM
{
static const wchar_t DEL_RESERVE[] = _T("Delete_reserve");
static const wchar_t DEL_COMPLETE[] = _T("Delete_complete");
//Patni-DKH/2011Feb08/Added/REDMINE-1434
static const wchar_t UPDATE_CORRECTEXAM[] = _T("Correct");
}
}

//************************************Method Header**************************************
//Method Name   :  GetInstance
//Author        :  PATNI/Sribanta
//Purpose       :  Creates CDEGLCommunicator instance if it is not created before.
//**************************************************************************************/
CDEGLCommunicator* CDEGLCommunicator::GetInstance(
)
{
    if (!m_connector) {
        m_connector = new CDEGLCommunicator();
    }

    return m_connector;
}

//************************************Method Header**************************************
//Method Name:  SendDeleteReserve
//Author:  PATNI/Sribanta
//Purpose: Send DeleteReserve to CFW BroadCast.It should be called before delete protocols.
//**************************************************************************************/

bool CDEGLCommunicator::SendDeleteReserve(const wchar_t* const f_studyoid)
{
    //Patni-DKH/2011Feb08/Modified/REDMINE-1434
    return this->SendEvent(_T(""), f_studyoid, CFW_BC::CUSTOM::DEL_RESERVE);
}


//************************************Method Header**************************************
//Method Name:  SendDeleteComplete
//Author:  PATNI/Sribanta
//Purpose: Send DeleteComplete to CFW BroadCast.It should be called after delete protocols.
//**************************************************************************************/

bool CDEGLCommunicator::SendDeleteComplete(const wchar_t* const f_studyoid)
{
    //Patni-DKH/2011Feb08/Modified/REDMINE-1434
    return this->SendEvent(_T(""), f_studyoid, CFW_BC::CUSTOM::DEL_COMPLETE);
}

//+Patni-DKH/2011Feb08/Added/REDMINE-1434
//************************************Method Header**************************************

//Method Name:  SendCurrentExamUpdate

//Author:  PATNI/DKH

//Purpose: Send Update current Exam data broadcast message.

//**************************************************************************************/

bool CDEGLCommunicator::SendCurrentExamUpdate(const wchar_t* const f_patient_oid,
        const wchar_t* const f_studyoid)
{
    return this->SendEvent(f_patient_oid, f_studyoid, CFW_BC::CUSTOM::UPDATE_CORRECTEXAM);
}
//-Patni-DKH/2011Feb08/Added/REDMINE-1434

//************************************Method Header**************************************
//Method Name:  DeleteCommunicatorIntance
//Author     :  PATNI/Hemant
//Purpose    :  Deletes the static communicator instance of this class
//**************************************************************************************/
void CDEGLCommunicator::DeleteCommunicatorIntance()
{

    if (m_connector) {

        DEL_PTR(m_connector);
    }
}

//************************************Method Header**************************************
//Method Name:  CDEGLCommunicator
//Author:  PATNI/Sribanta
//Purpose: Constructor
//**************************************************************************************/

CDEGLCommunicator::CDEGLCommunicator()
    : m_cfw_connectors(), //TMSC-Tanoue/2011Feb14/Modified/REDMINE-1434
      m_is_connected(false)
{
}



//************************************Method Header************************************
//Method Name:  ~CDEGLCommunicator
//Author:  PATNI/Sribanta
//Purpose: Destructor
//**************************************************************************************/

CDEGLCommunicator::~CDEGLCommunicator()
{
    //TMSC-Tanoue/2011Feb14/Modified/REDMINE-1434
    for (CFW_BC_Map_t::iterator itr = m_cfw_connectors.begin();
         itr != m_cfw_connectors.end(); ++itr) {

        DEL_CONNECTOR((*itr).second);
        (*itr).second = NULL;
    }

    m_cfw_connectors.clear();
}

//************************************Method Header**************************************

//Method Name:  CreateConnector

//Author:  TMSC/Tanoue

//Purpose: Create CFW BroadCast Connector.
//         It create and connect connector for each threads,
//         because CFW BroadCast Connector is not able to re-use with different threads.
//         TODO : Try re-use one-instance with COM MTA

//**************************************************************************************/

//+TMSC-Tanoue/2011Feb14/Modified/REDMINE-1434
IMplusBroadcastConnector* CDEGLCommunicator::CreateConnector()
{
    LPCTSTR FUNC_NAME = _T("CDEGLCommunicator::CreateConnector");
    const DWORD thread_id = ::GetCurrentThreadId();

    CFW_BC_Map_t::const_iterator itr = m_cfw_connectors.find(thread_id);

    if (itr != m_cfw_connectors.end()) {
        return (*itr).second;
    }

    IMplusBroadcastConnector* connector = NEW_BCONNECTOR();

    if (!connector) {
        return NULL;
    }

    const HRESULT hr = connector->Connect(_bstr_t(CFW_BC::CHANNEL));

    if (FAILED(hr)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("[ERROR] Connect to CFW Broadcast failed.\n"));

        DEL_CONNECTOR(connector);
        return NULL;
    }

    m_cfw_connectors.insert(std::make_pair(thread_id, connector));
    return connector;
}
//-TMSC-Tanoue/2011Feb14/Modified/REDMINE-1434

//************************************Method Header**************************************
//Method Name:  SendEvent
//Author:  PATNI/Sribanta
//Purpose: Send Command with param and custom to CFW BroadCast
//**************************************************************************************/
//+Patni-DKH/2011Feb08/Modified/REDMINE-1434
bool CDEGLCommunicator::SendEvent(const wchar_t* const f_patient_oid,
                                  const wchar_t* const f_studyoid,
                                  const wchar_t* const custom)
{
    LPCTSTR FUNC_NAME = _T("CDEGLCommunicator::SendEvent");

    if (NULL == f_patient_oid || NULL == f_studyoid || NULL == custom) {
        return false;
    }

    //+TMSC-Tanoue/2011Feb14/Modified/REDMINE-1434
    IMplusBroadcastConnector* connector = this->CreateConnector();

    if (!connector) {
        return false;
    }

    //-TMSC-Tanoue/2011Feb14/Modified/REDMINE-1434

    IMplusMessage* pMsg = NEW_MSG();

    if (NULL == pMsg) {
        return false;
    }

    pMsg->SetSender(_bstr_t(CFW_BC::SENDER));
    pMsg->SetCommand(_bstr_t(CFW_BC::CMD));
    pMsg->SetCustom(_bstr_t(custom));

    // param format is Local/patientOID/f_studyoid/seriesOID/imageOID
    std::wstring param(_T("Local/"));
    param += f_patient_oid;
    param += _T("/");
    param += f_studyoid ;
    pMsg->SetParameter(_bstr_t(param.c_str()));


    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("Calling SendMessage"));
    //TMSC-Tanoue/2011Feb14/Modified/REDMINE-1434
    HRESULT hr = connector->SendMessage(pMsg, _bstr_t(CFW_BC::CMD));
    DEL_MSG(pMsg);

    //+Patni-Ravindra Acharya/2010Oct04/Added/IR-165
    pMsg = NULL ;
    //-Patni-Ravindra Acharya/2010Oct04/Added/IR-165

    return FAILED(hr) ? false : true;
}
//-Patni-DKH/2011Feb08/Modified/REDMINE-1434


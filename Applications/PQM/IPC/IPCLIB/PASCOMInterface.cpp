//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-774 : Unnecessary functions removal
//Redmine-780
//Redmine-781
//Redmine-782
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PASCOMInterface.cpp
 *  Overview: Implementation of CPASCOMInterface class.
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
#include "PASCOMInterface.h"
#include <PASCOMLib/PASCOMLayerFactory.h>

#include <PQM/PQMLogMgr.h>
#include <PQM/PqmUtilities.h> // Added By KT/ACE-2/15-06-2009/Multilingualisation
#include "Pqm.h"
#include "PQMStudyMgr.h"
#include <PQM/PQMIPCManager.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace MR::ACQ::SM::COMMON::MUTEX;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//*****************************************************************************
//Method Name   : CPASCOMInterface
//Author        : PATNI/DJ
//Purpose       : Construct the object.
//*****************************************************************************
CPASCOMInterface::CPASCOMInterface(const int f_mode):
    m_ptr_pqm(NULL),	//Patni-AD/2009Jun01/Added/SU14 //cpp test corrections
    m_study_null_msg_displayed(false),
    m_mode(f_mode)
{
    if (m_mode == K_IMAGING) {
        CString acquire_app_name = _T("PQM");
        CString acquire_channel_name = _T("Acquire");
        //		m_pascom = CPASCOMLayer::GetInstance(acquire_app_name,acquire_channel_name); // USHA TAREVIEW
        m_pascom = CPASCOMLayerFactory::GetPASCOMLayerInstance(acquire_app_name, acquire_channel_name);

        if (m_pascom) {	//Patni-MJC/2009Aug17/Modified/cpp test corrections
            m_pascom->RegisterListener(this);
        }

    } else if (m_mode == K_SCHEDULING) {
        CString schedule_app_name = _T("PQM");
        CString schedule_channel_name = _T("Schedule");
        // m_pascom = CPASCOMLayer::GetInstance(schedule_app_name,schedule_channel_name); //USHA TAREVIEW
        m_pascom = CPASCOMLayerFactory::GetPASCOMLayerInstance(schedule_app_name, schedule_channel_name);

        if (m_pascom) {	//Patni-MJC/2009Aug17/Modified/cpp test corrections
            m_pascom->RegisterListener(this);
        }
    }
}

//*****************************************************************************
//Method Name : CPASCOMInterface
//Author      : PATNI/AMT
//Purpose     : Destruct the object.
//*****************************************************************************
CPASCOMInterface::~CPASCOMInterface()
{
    if (m_pascom != NULL) {
        //        CPASCOMLayer::DestroyInstance(); //USHA TAREVIEW
        CPASCOMLayerFactory::DestroyPASCOMLayerInstance();

        m_pascom = NULL;
    }
}

//****************************Method Header********************
//Method Name   : ReadSequenceParameterXML
//Author        : PATNI/DJ
//Purpose       : This will read sequence parameter information.
//*************************************************************
void CPASCOMInterface::ReadSequenceParameterXML(
    Data_decl_list& f_data_decl_list
)
{
    LPCTSTR FUNC_NAME = _T("CPASCOMInterface::ReadSequenceParameterXML");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pascom) {
        m_pascom->ReadSequenceParameterXML(f_data_decl_list);

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pascom pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//****************************Method Header***************************
//Method Name   : SendSequence
//Author        : PATNI/DJ
//Purpose       : This method sends Sequences from PQM to Memory.
//********************************************************************
void CPASCOMInterface::SendSequence(
    SequenceList& f_seq_to_send
)
{

    //	AfxMessageBox(_T("CPASCOMInterface::SendSequence"));
    CString l_target = _T("");

    if (m_mode == K_SCHEDULING) {
        //		AfxMessageBox(_T("Sending to Memory.Schedule"),MB_SYSTEMMODAL);
        l_target = SCHEDULE_MEMORY;

    } else if (m_mode == K_IMAGING) {
        //		AfxMessageBox(_T("Sending to Memory.Acquire"),MB_SYSTEMMODAL);
        l_target = ACQUIRE_MEMORY;
    }

    m_pascom->Communicate
    ((WPARAM)&f_seq_to_send, PASCOM_SEQ_SENDING, l_target, m_ptr_pqm->GetMode());
}

//+Patni-Sribanta/2010Nov09/Added/IR-170
//******************************************************************************
// Function Name    : CheckLicense
// Author           : PATNI-Sribanta
// Purpose          : Function to check licence of PAS with PAS path.
//******************************************************************************
bool CPASCOMInterface::CheckLicense(
    const CString& f_pas_path
) const
{
    if (!m_pascom) {
        return false ;
    }

    return m_pascom->CheckLicense(f_pas_path);
}
//-Patni-Sribanta/2010Nov09/Added/IR-170

//+Patni-Sribanta/2011Feb25/Added/IR-170_Part3
//******************************************************************************
// Function Name    : IsValidLicenseInLabelFile
// Author           : PATNI-Sribanta
// Purpose          :
//******************************************************************************
bool CPASCOMInterface::IsValidLicenseInLabelFile(
    const CString& f_pas_path
)
{
    return m_pascom->IsValidLicenseInLabelFile(f_pas_path);
}
//-Patni-Sribanta/2011Feb25/Added/IR-170_Part3

//*****************************************************************************
//Method Name : GetAllSequences
//Author      : PATNI-DJ
//Purpose     : Get all the sequences.
//*****************************************************************************
bool CPASCOMInterface::GetAllSequences(
    SequenceList& seq_list,
    int& mode
)
{
    LPCTSTR FUNC_NAME = _T("CPASCOMInterface::GetAllSequences");
    bool l_ret = false;
    seq_list.RemoveAll();

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pascom) {
        m_pascom->GetAllSequences(seq_list, mode);
        l_ret = true;
        //	CString str_mode;
        //    str_mode.Format(_T("GetAllSequences Mode: %d"),mode);
        //	AfxMessageBox(str_mode,MB_SYSTEMMODAL);
        SetMode(mode);

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pascom pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    return l_ret;
}


//*****************************************************************************
//Method Name : GetAllSequences
//Author      : PATNI-DJ
//Purpose     : Retrieves all the existing sequences under the selected PAS in
//               the database by calling the PASCOM library's GetAllSequneces()
//               function.
//*****************************************************************************
bool CPASCOMInterface::GetAllSequences(
    SequenceList& f_sequence_list,
    const CString&       f_container_path,
    const PasUniqueId&   f_pas_id
)
{
    LPCTSTR FUNC_NAME = _T("CPASCOMInterface::GetAllSequences");
    f_sequence_list.RemoveAll();

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pascom) {
        m_pascom->GetAllSequences(f_sequence_list, f_container_path, f_pas_id);
        return true;

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pascom pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    return false;
}

//+Patni-Sribanta/2010Dec06/Added/IR-170_Reopen
//****************************Method Header************************************
//Method Name   :GetLicensedAcqOrder()
//Author        :PATNI/Sribanta
//Purpose       :
//*****************************************************************************
void CPASCOMInterface::GetLicensedAcqOrder(
    VuiAcqOrder_t& f_acq_order,
    const CString& f_pas_path
)
{
    MemInitSeqDB();

    int l_total_licensedseq = 0;

    for (int i = 0; i < f_acq_order.SzAcqOrder; i++) {
        if (IsValidLicenseFor(f_pas_path, static_cast<long>(f_acq_order.VuiAcqOrder[i]))) {
            f_acq_order.VuiAcqOrder[l_total_licensedseq] = f_acq_order.VuiAcqOrder[i];
            ++l_total_licensedseq;
        }
    }

    f_acq_order.SzAcqOrder = l_total_licensedseq;
    f_acq_order.NumAcqOrder = l_total_licensedseq;

}
//-Patni-Sribanta/2010Dec06/Added/IR-170_Reopen

//ADD SEQUENCE FIX 25 June 08
//******************************************************************************
// Function Name    :
// Author           : PATNI -
// Purpose          :
//******************************************************************************
CPASCOMSequence* CPASCOMInterface::GetSingleSequence(
    const CString& f_path,
    long& f_acqordr
)
{
    LPCTSTR FUNC_NAME = _T("CPASCOMInterface::GetSingleSequence");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pascom) {
        return m_pascom->GetSingleSequence(f_path, f_acqordr);

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pascom pointer is NULL"));
        return NULL;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//****************************Method Header************************************
//Method Name   : GetMode()
//Author        : PATNI/MSN
//Purpose       : This method give the value of the mode
//*****************************************************************************
int CPASCOMInterface::GetMode(
) const
{
    //This is not Required
    //CString str_mode(_T(""));
    //str_mode.Format(_T("%d"), m_mode);

    return m_mode;
}

//****************************Method Header************************************
//Method Name   : SetMode()
//Author        : PATNI/MSN
//Purpose       : This method set the value of the mode
//*****************************************************************************
void CPASCOMInterface::SetMode(
    const int mode
)
{
    m_mode = mode;
}

//****************************Method Header********************
//Method Name   : SetPqm
//Author        : PATNI/DJ
//Purpose       : Set the current PQM pointer.
//*************************************************************
void CPASCOMInterface::SetPqm(
    CPqm* p_pqm
)
{
    m_ptr_pqm = p_pqm;
}

//+Patni-Sribanta/2010Dec06/Added/IR-170_Reopen
//******************************************************************************
// Function Name    : MemInitSeqDB
// Author           : PATNI-Sribanta
// Purpose          :
//******************************************************************************
void CPASCOMInterface::MemInitSeqDB(
)const
{
    m_pascom->MemInitSeqDB();
}

//******************************************************************************
// Function Name    : IsValidLicenseFor
// Author           : PATNI-Sribanta
// Purpose          :
//******************************************************************************
bool CPASCOMInterface::IsValidLicenseFor(
    const CString& f_pas_path,
    const long& f_acqorder
)
{
    return m_pascom->IsValidLicenseFor(f_pas_path, f_acqorder);
}
//-Patni-Sribanta/2010Dec06/Added/IR-170_Reopen

//****************************Method Header***************************
//Method Name   : SequenceSending_Callback
//Author        : PATNI/DJ
//Purpose       : This will send message to GUI that, MEMORY is ready
//                Ready to recevie sequences from PQM.
//********************************************************************
bool CPASCOMInterface::SequenceSending_Callback(
    const PASCOM_COMM_MSG& f_pascom_msg
)
{
    LPCTSTR FUNC_NAME = _T("CPASCOMInterface::SequenceSending_Callback");
    //+Patni-HAR/2010Feb04/Added/PQM->Memory Sequence Transfer
    DWORD timeout = (1000 * 60);
    HANDLE hPQMStateEvent = NULL;

    if (NULL != (hPQMStateEvent = OpenEvent(SYNCHRONIZE, FALSE, PQM_CHILD_EVENT))) {
        if (WAIT_TIMEOUT == WaitForSingleObject(hPQMStateEvent, timeout)) {
            // Error through for time out
        }

        CloseHandle(hPQMStateEvent);
    }

    //-Patni-HAR/2010Feb04/Added/PQM->Memory Sequence Transfer

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_ptr_pqm) {
        m_ptr_pqm->WriteToUI(PQM_MSR_RCV_SEQ); //  Ready to receive sequences

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pascom pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    return true;
}

//****************************Method Header********************
//Method Name   : SequenceReceiving_Callback
//Author        : PATNI/SS
//Purpose       : This function is a sort of callback function.
//                Called when PASCOM receives sequence receive msg
//*************************************************************
bool CPASCOMInterface::SequenceReceiving_Callback(
    const PASCOM_COMM_MSG& f_pascom_msg
)
{
    LPCTSTR FUNC_NAME = _T("CPASCOMInterface::SequenceReceiving_Callback");
    CScopedLock l_scoped_lock(m_ptr_pqm->GetStudy()->GetSeqTransferMutex());
    //+Patni-SS/2009Apr07/Modi/PSP1#12
    // this is to check is PQM is busy in updating data if PQM is busy than wait.
    DWORD timeout = (1000 * 60);
    HANDLE hPQMStateEvent = NULL;

    if (NULL != (hPQMStateEvent = OpenEvent(SYNCHRONIZE, FALSE, PQM_CHILD_EVENT))) {
        if (WAIT_TIMEOUT == WaitForSingleObject(hPQMStateEvent, timeout)) {
            // Error through for time out
        }

        //Patni-HAR/2010Feb04/Added/PQM->Memory Sequence Transfer
        CloseHandle(hPQMStateEvent);
    }

    //-Patni-SS/2009Apr07/Modi/PSP1#12

    bool l_result = false;	//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections

    if (m_ptr_pqm->GetDbsaStudyMgr()->GetStudy() == NULL) {

        if (m_study_null_msg_displayed == false) {
            //CString temp_string = CPqmUtilities::GetMultiLingualString(_T("IDS_STUDY_LOAD_ERROR"));    // Added By KT/ACE-2/15-06-2009/Multilingualisation

            CPQMLogMgr::GetInstance()->DisplayError(
                CPqmUtilities::GetMultiLingualString(_T("IDS_STUDY_LOAD_ERROR")),
                _T("PASCOMInterface.cpp"), __LINE__,
                ERROR_LOG_CRUCIAL1 ,
                ERR_ID, APP_NAME);

            m_study_null_msg_displayed = true;
        }

    } else {

        if (f_pascom_msg.no_of_sequence_sent > 0) {

            //Patni-SS/MODI/2009-May-27/JFT#73
            if (m_ptr_pqm->GetNoOfSequence() == 0) {
                m_ptr_pqm->WriteToUI(PQM_MSG_BEGINWAIT_CURSOR);
            }

            //+Patni-Sribanta/2010Nov15/Modified/IR-170
            /*
            CString l_acq_order(_T(""));
            l_acq_order.Format(_T("%d"), f_pascom_msg.acquisition_order);
            m_ptr_pqm->SetNoOfSequence(f_pascom_msg.no_of_sequence_sent);


            m_ptr_pqm->ReceiveSequences(f_pascom_msg.sequence_path, l_acq_order);


            m_ptr_pqm->SendSequencesToGUI();

            //+Patni-HEMANT/ ADDED On6/12/2009 1:14:13 PM/ ACE-2 / WFDA
            //m_ptr_pqm->NewSequencesAdded(f_pascom_msg.no_of_sequence_sent);
            m_ptr_pqm->NewSequencesAdded(1);
            */
            //IR-170_Impact_Scenario
            if (!m_ptr_pqm->ShowSequenceOnGUI(&(f_pascom_msg.acquisition_order), f_pascom_msg.sequence_path, f_pascom_msg.no_of_sequence_sent)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ShowSequenceOnGUI() failed"));
                l_result = false;
            }

            //-Patni-Sribanta/2010Nov15/Modified/IR-170

            l_result = true;
        }

        m_study_null_msg_displayed = false;
    }

    return l_result;
}

//****************************Method Header********************
//Method Name   : HandlePASCOMMsg
//Author        : PATNI/DJ
//Purpose       : This listener function will receive
//              : sequences from Memory and send it to GUI.
//*************************************************************
bool CPASCOMInterface::HandlePASCOMMsg(
    const PASCOM_COMM_MSG& f_pascom_msg
)
{
    LPCTSTR FUNC_NAME = _T("CPASCOMInterface::HandlePASCOMMsg");
    bool l_ret = false;

    //+ Patni-RJ/2009Apr02/Added/PSP- Performance code
#ifdef __SM_PERFORMANCE_CHECK__
    __int64 l_freq = 0, l_stop = 0, l_start = 0;
#endif
    //- Patni-RJ/2009Apr02/Added/PSP- Performance code

    CString l_target = _T("");

    if (m_mode == K_SCHEDULING) {
        l_target = SCHEDULE_MEMORY;

    } else if (m_mode == K_IMAGING) {
        l_target = ACQUIRE_MEMORY;
    }

    MSG_TYPE l_msg = f_pascom_msg.msg;
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PASCOM_COMM_MSG"));

    switch (l_msg) {
        case PASCOM_SEQ_SENDING: { //When sequences will come from Memory.
            //+ Patni-RJ/2009Apr02/Added/PSP- Performance code
#ifdef __SM_PERFORMANCE_CHECK__

            QueryPerformanceCounter((LARGE_INTEGER*)&l_start);
#endif
            //- Patni-RJ/2009Apr02/Added/PSP- Performance code

            l_ret = SequenceReceiving_Callback(f_pascom_msg);
            CPQMIPCManager* l_ipcmgr = m_ptr_pqm->GetManagerPtr() ;
            int l_no_of_sequence_recd_from_memory = l_ipcmgr->GetNoOfSeqRecvdfromMemory();

            if (l_no_of_sequence_recd_from_memory >= f_pascom_msg.no_of_sequence_sent) {

                m_pascom->Communicate
                ((WPARAM)NULL, PASCOM_PQM_SEQ_RECEIVED, l_target, m_ptr_pqm->GetMode());

                l_ipcmgr->SetNoOfSeqRecvdfromMemory(1);

            } else {
                l_no_of_sequence_recd_from_memory++;
                l_ipcmgr->SetNoOfSeqRecvdfromMemory(l_no_of_sequence_recd_from_memory);
            }

            //+ Patni-RJ/2009Apr02/Added/PSP- Performance code
#ifdef __SM_PERFORMANCE_CHECK__
            QueryPerformanceFrequency((LARGE_INTEGER*)&l_freq);
            QueryPerformanceCounter((LARGE_INTEGER*)&l_stop);

            CPQMLogMgr::GetInstance()->TracePerformance(l_start, l_stop, l_freq, _T("{HandlePASCOMMsg} Receiving sequence from Memory"));
#endif
            //- Patni-RJ/2009Apr02/Added/PSP- Performance code
        }
        break;

        case PASCOM_SEQ_RECEIVING: //When "Add sequences" will be selected at memory side.
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PASCOM_SEQ_RECEIVING"));

            //+Patni-MJC/2009Aug17/Modified/cpp test corrections
            if (m_ptr_pqm) {
                m_ptr_pqm->WriteToUI(PQM_MSG_BEGINWAIT_CURSOR);

            } else {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_ptr_pqm pointer is NULL"));
            }

            //-Patni-MJC/2009Aug17/Modified/cpp test corrections
            l_ret = SequenceSending_Callback(f_pascom_msg);

            break;

        case PASCOM_REGIST_FINISH:
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PASCOM_REGIST_FINISH"));

            //+Patni-MJC/2009Aug17/Modified/cpp test corrections
            if (m_ptr_pqm) {
                m_ptr_pqm->WriteToUI(PQM_MSG_PASCOM_REGIST_DONE);
                m_ptr_pqm->WriteToUI(PQM_MSG_ENDWAIT_CURSOR);

            } else {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_ptr_pqm pointer is NULL"));
            }

            //-Patni-MJC/2009Aug17/Modified/cpp test corrections
            break;

        default:
            // SKP throw;
            break;
    }

    //l_ret = true;
    return true; //l_ret;
}

//****************************Method Header********************
//Method Name   : ConvertSequenceToXML
//Author        : PATNI/DJ
//Purpose       : This fun. will convert the row sequences to XML.
//*************************************************************
void CPASCOMInterface::ConvertSequenceToXML(
    CPASCOMSequence* f_seq,
    TCHAR* l_seq_str
)
{
    LPCTSTR FUNC_NAME = _T("CPASCOMInterface::ConvertSequenceToXML");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pascom) {
        m_pascom->ConvertSequenceToXML(f_seq, l_seq_str);

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pascom pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//****************************Method Header********************
//Method Name   : CreateSequenceformString
//Author        : PATNI/DJ
//Purpose       : This will create a sequence from CString value
//*************************************************************
void CPASCOMInterface::CreateSequenceformString(
    const CString& f_seq_data
)
{
    LPCTSTR FUNC_NAME = _T("CPASCOMInterface::CreateSequenceformString");

    //Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(f_seq_data);


    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pascom) {
        //+Patni-Sudhir/2010Dec03/Modified/TMSC_REDMINE-774
        m_pascom->CreateSequenceformString();
        //-Patni-Sudhir/2010Dec03/Modified/TMSC_REDMINE-774

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pascom pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//****************************Method Header********************
//Method Name   : AddReceivedSequence
//Author        : PATNI/DJ
//Purpose       : This will add the receive sequences.
//*************************************************************
void CPASCOMInterface::AddReceivedSequence(
    SequenceList& f_seq_list
)
{
    LPCTSTR FUNC_NAME = _T("CPASCOMInterface::AddReceivedSequence");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pascom) {
        m_pascom->AddReceivedSequence(f_seq_list);

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pascom pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

///////////////////////////////////////////////////////////////////////////////
//                          PAS Related Operations.
///////////////////////////////////////////////////////////////////////////////
//*****************************************************************************
//Method Name : DuplicatePAS
//Author      : PATNI-DJ
//Purpose     : Duplicate PAS information in database by calling the
//               PASCOM library's DuplicatePAS() function.
//*****************************************************************************
bool CPASCOMInterface::DuplicatePAS(
    PAS& f_pas,
    const CString& f_container_path
)const
{
    LPCTSTR FUNC_NAME = _T("CPASCOMInterface::DuplicatePAS");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pascom) {
        m_pascom->DuplicatePAS(f_pas, f_container_path);
        return true;

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pascom pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    return false;
}

//*****************************************************************************
//Method Name : DeletePAS
//Author      : PATNI-DJ
//Purpose     : Delete one or more PAS information from database by calling the
//               PASCOM library's DeletePAS() function.
//*****************************************************************************
bool CPASCOMInterface::DeletePAS(
    const PasUniqueId& f_pas_id,
    const CString& f_container_path
)const
{
    LPCTSTR FUNC_NAME = _T("CPASCOMInterface::DeletePAS");

    const PasUniqueId& l_pas_id = f_pas_id;

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pascom) {
        m_pascom->DeletePAS(f_container_path, l_pas_id);
        return true;

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pascom pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    return false;
}

//*****************************************************************************
//Method Name : SortPAS
//Author      : PATNI-DJ
//Purpose     : Sorts PAS in database for that it calls the PASCOM library's
//                  SortPAS() function.
//*****************************************************************************
bool CPASCOMInterface::SortPAS(
    PasUniqueIdList& f_pas_id_list,
    const CString& f_container_path
)const
{
    LPCTSTR FUNC_NAME = _T("CPASCOMInterface::SortPAS");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pascom) {
        m_pascom->SortPAS(f_pas_id_list, f_container_path);
        return true;

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pascom pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    return false;
}

//*****************************************************************************
//Method Name : CreatePASDB
//Author      : PATNI-DJ
//Purpose     : This will create a PAS database.
//*****************************************************************************
void CPASCOMInterface :: CreatePASDB(
    const CString& PASDBpath,
    const CString& f_mapfilePath,
    CPASDBMetaData* f_metadata
)const
{
    LPCTSTR FUNC_NAME = _T("CPASCOMInterface::CreatePASDB");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pascom) {
        m_pascom->CreatePASDB(PASDBpath, f_mapfilePath, f_metadata);

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pascom pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//*****************************************************************************
//Method Name : CreatePAS
//Author      : PATNI-DJ
//Purpose     : Create PAS in database for that it calls the PASCOM library's
//                  CreatePAS() function.
//*****************************************************************************
bool CPASCOMInterface::CreatePAS(
    PAS& f_pas,
    const CString& f_container_path
)
{
    LPCTSTR FUNC_NAME = _T("CPASCOMInterface::CreatePAS");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pascom) {
        m_pascom->CreatePAS(f_pas, f_container_path);
        return true;

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pascom pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    return false;
}

//*****************************************************************************
//Method Name : RenamePAS
//Author      : PATNI-DJ
//Purpose     : Rename PAS in database for that it calls the PASCOM library's
//              RenamePAS() function.
//*****************************************************************************
bool CPASCOMInterface::RenamePAS(
    const PasUniqueId& f_pas_id,
    const CString& f_container_path,
    const CString& f_new_name
)const
{
    LPCTSTR FUNC_NAME = _T("CPASCOMInterface::RenamePAS");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pascom) {
        m_pascom->RenamePAS(f_pas_id, f_container_path, f_new_name);
        return true;

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pascom pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    return false;
}

//*****************************************************************************
//Method Name : GetAllPAS
//Author      : PATNI-DJ
//Purpose     : Retrieve all the PAS of currently selected container from the
//               database by calling the PASCOM library's GetAllPAS()
//               function.
//*****************************************************************************
bool CPASCOMInterface::GetAllPAS(
    PasList& f_pas_list,
    const CString& f_container_path
)
{
    LPCTSTR FUNC_NAME = _T("CPASCOMInterface::GetAllPAS");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pascom) {
        m_pascom->GetAllPas(f_pas_list, f_container_path);
        return true;

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pascom pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    return false;
}

///////////////////////////////////////////////////////////////////////////////
//                          Sequence Related Operations.
///////////////////////////////////////////////////////////////////////////////

//*****************************************************************************
//Method Name : PasteSequences
//Author      : PATNI-DJ
//Purpose     : Retrieve all the PAS of currently selected container from the
//               database by calling the PASCOM library's GetAllPAS()
//               function.
//*****************************************************************************
bool CPASCOMInterface::PasteSequences(
    SequenceList& f_sequence_list,
    const CString&       f_container_path,
    const PasUniqueId&   f_pas_id
)const
{
    LPCTSTR FUNC_NAME = _T("CPASCOMInterface::PasteSequences");

    //+Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(f_sequence_list);
    UNREFERENCED_PARAMETER(f_pas_id);
    //-Patni-KSS/2011Feb18/Added/CPP Test tool Execution



    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pascom) {
        //+Patni-Sudhir/2010Dec03/Modified/TMSC_REDMINE-774
        m_pascom->PasteSequence(f_container_path);
        //-Patni-Sudhir/2010Dec03/Modified/TMSC_REDMINE-774
        return true;

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pascom pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    return false;
}

//*****************************************************************************
//Method Name : DeleteSequences
//Author      : PATNI-DJ
//Purpose     : Delete the sequences specified by the f_sequence_id_list from
//               selected PAS in database by calling the PASCOM library's
//               DeleteSequence() function.
//*****************************************************************************
bool CPASCOMInterface::DeleteSequences(
    const CString&               f_container_path,
    SequenceUniqueIdList& f_sequence_id_list,
    const PasUniqueId&           f_pas_id
)const
{
    LPCTSTR FUNC_NAME = _T("CPASCOMInterface::DeleteSequences");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pascom) {
        m_pascom->DeleteSequences(f_container_path, f_sequence_id_list, f_pas_id);
        return true;

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pascom pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    return false;
}

//*****************************************************************************
//Method Name : SortSequences
//Author      : PATNI-DJ
//Purpose     : Change the order of sequence of currently selected PAS in the
//               database by calling the PASCOM library's SortSequnece()
//               function.
//*****************************************************************************
bool CPASCOMInterface::SortSequence(
    SequenceUniqueIdList& f_sequence_id_list,
    const CString&               f_container_path,
    const PasUniqueId&           f_pas_id
)const
{
    LPCTSTR FUNC_NAME = _T("CPASCOMInterface::SortSequence");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pascom) {
        m_pascom->SortSequence(f_sequence_id_list, f_pas_id, f_container_path);
        return true;

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pascom pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    return false;
}


//*****************************************************************************
//Method Name : RenameSequences
//Author      : PATNI-DJ
//Purpose     : Rename one or more existing sequences under the selected PAS in
//               the database by calling the PASCOM library's RenameSequneces()
//               function.
//*****************************************************************************
bool CPASCOMInterface::RenameSequences(
    SequenceUniqueIdList& f_sequence_id_list,
    const CString&               f_pas_path,
    const CString&               f_new_name
)
{
    //+Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(f_sequence_id_list);
    UNREFERENCED_PARAMETER(f_pas_path);
    UNREFERENCED_PARAMETER(f_new_name);
    //-Patni-KSS/2011Feb18/Added/CPP Test tool Execution

    return true;
}



//******************************************************************************
// End of file

//ADD SEQUENCE FIX 25 June 08
//******************************************************************************



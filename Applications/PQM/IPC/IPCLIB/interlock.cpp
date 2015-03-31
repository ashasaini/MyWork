//REDMINE-1213
//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-779
//Redmine-780
//Redmine-781
//Redmine-782
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: interlock.cpp
 *  Overview: Implementation of CInterLock class.
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
#include "InterLock.h"
#include <tami/libAcq/AcqCoilSignal.h>
#include <PQM/resource.h>
#include <PQM/PQMLogMgr.h>
#include <PQM/PqmUtilities.h> // Added By KT/ACE-2/15-06-2009/Multilingualisation
#include "pqm.h"
#include "PqmPmAcqman.h"
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
//#include "coildb.h"
#include "pqmstudy.h"
#include "PqmPrMode.h"
#include "PQMXMLParser.h"
#include "scanprocedure.h"
#include "PqmCoilData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//****************************Method Header************************************
//Method Name   :CInterLock()
//Author        :PATNI/ASG
//Purpose       :Constructor
//*****************************************************************************
CInterLock::CInterLock(
    CScanProcedure* scan_proc
): CScanTransaction(scan_proc)
{
    LPCTSTR FUNC_NAME = _T("CInterLock::CInterLock");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ScanTransaction Created"));
}

//****************************Method Header************************************
//Method Name   :~CInterLock()
//Author        :PATNI/ASG
//Purpose       :Destructor
//*****************************************************************************
CInterLock::~CInterLock(
)
{
    LPCTSTR FUNC_NAME = _T("CInterLock::~CInterLock");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ScanTransaction Destroyed"));
}

//****************************Method Header************************************
//Method Name   :ErrReport()
//Author        :PATNI/ASG
//Purpose       :This method does the processing after an error message is
//              received from Acqman
//*****************************************************************************
bool CInterLock::ErrReport(
    const int type,
    const int errorId,
    const CString& err_msg,				// Added By KT/ACE-2/15-06-2009/Multilingualisation
    AM_AcqManToClient_t* const body
)
{
    LPCTSTR FUNC_NAME = _T("CInterLock::ErrReport");
    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool executions
    UNREFERENCED_PARAMETER(body);
    CScanProcedure* scan_proc = GetScanProcedure();
    CPqmPmAcqman* acqman = scan_proc->GetAcqManager();
    CPqm* pqm =  acqman->GetPqm();

    CPqmProtocol* curr_protocol = scan_proc->GetScanProcProtocol();

    CString str_msg(_T(""));
    str_msg.Format(_T("%s %d"), acqman->GetStr(type), errorId);

    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, str_msg);

    //  CString l_error_str = L"";		 // Added By KT/ACE-2/15-06-2009/Multilingualisation
    //  l_error_str.LoadString(errorId);   // Added By KT/ACE-2/15-06-2009/Multilingualisation
    //str_msg = CPqmUtilities::GetMultiLingualString(err_msg); // Added By KT/ACE-2/15-06-2009/Multilingualisation
    //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
    CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
        CPqmUtilities::GetMultiLingualString(err_msg),
        MPlus::EH::Information, _T("CInterLock.cpp"), _T("PQM"));


    pqm->GetStudy()->SetAcqStatus(curr_protocol, C_PR_STATUS_WAIT);
    pqm->WriteToUI(PQM_MSG_ABORT_WAIT);

    if (curr_protocol != NULL) {
        //+Patni-PJS/Added/2011-Mar-11/REDMINE-1529
        curr_protocol->SetProcessing(PROCESSING_VALUE_NONE);
        pqm->GetStudy()->SetScanModeOfHead();
        scan_proc->ResetCurrentRequest();
    }

    m_scan_procedure->SetIsGateExecuting(false);
    m_scan_procedure->SetIsCoilOrGateInfoExecuting(false);
    return true;
}

//****************************Method Header************************************
//Method Name   :BeforePrescan()
//Author        :PATNI/ASG
//Purpose       :This method will do the processing required after receiving the
//                reply for the Interlock request made by PQM IPC
//*****************************************************************************
int CInterLock::BeforePrescan(
    const int type,
    AM_AcqManToClient_t* body
)
{
    LPCTSTR FUNC_NAME = _T("CInterLock::BeforePrescan");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace"));

    CPqmProtocol* curr_protocol = NULL;

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_scan_procedure) {
        curr_protocol = m_scan_procedure->GetScanProcProtocol();
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    CScanProcedure* scan_proc = GetScanProcedure();

    if (type == AMCM_FUNC_KIND_REQUEST) {
        if (m_header.function == AMCMR_REMOTE_MOVECOUCH) {
            scan_proc->SetScanAbort(PQM_SCAN_ABORT);
            //PqmAcqGlobal.scanAbort = PqmAcqScanAbort;
            //PqmUiSetSensitive(W_SCAN_ABORT_BTN,TRUE);
            //PqmDebugAcqMonitor("BeforePreScan Callback: Unfreeze Scan");
            //PqmUiUnfreeze(PQM_FREEZE_FOR_SCAN);

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace"));

        }

        return E_NO_ERROR;
    }

    if (curr_protocol != NULL) {
        //+Patni-PJS/Added/2011-Mar-11/REDMINE-1529
        curr_protocol->SetProcessing(PROCESSING_VALUE_NONE);
    }

    if (scan_proc->GetIsScanning()) {
        if (!PostProcess(
                body->couchInfo.position,
                body->couchInfo.couchStatusFlag,
                m_header.function)) {

            //+Patni-PJS/2009May10/Modify/IR-82 review comment
            CString  string = _T("");
            string.LoadString(IDS_CANNOT_PROCEED);
            CPQMLogMgr::GetInstance()->WriteEventLog(string,
                    ERROR_LOG_CRUCIAL1 ,
                    _T("Interlock.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82

            /*
            CPQMLogMgr::GetInstance()->WriteEventLog(IDS_CANNOT_PROCEED,
                    ERROR_LOG_CRUCIAL1 ,
                    _T("Interlock.cpp"));
            */

            //-Patni-PJS/2009May10/Modify/IR-82 review comment
            return E_ERROR;
        }
    }

    return E_NO_ERROR;
}

//****************************Method Header************************************
//Method Name   :ProcessResponse()
//Author        :PATNI/ASG
//Purpose       :This method will be called to process response received from
//              Acqman
//*****************************************************************************
void CInterLock::ProcessResponse(
    AM_ClientMessageHeader_t* hdr,
    AM_AcqManToClient_t* body
)
{
    LPCTSTR FUNC_NAME = _T("CInterLock::ProcessResponse");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace"));

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (hdr == NULL) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("hdr is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    CScanProcedure* scan_proc = GetScanProcedure();
    m_header = *hdr;
    CPqmPmAcqman* acqman = scan_proc->GetAcqManager();

    int type = 0;
    int funcBit = acqman->GetFunctionBit(hdr->function);

    CString str_msg(_T(""));
    str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                   acqman->GetStr(body->status),
                   acqman->GetStr(type),
                   acqman->GetStr(hdr->function));

    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, str_msg);

    if ((body->status == AMCM_SUCCESS) ||
        (body->status == AMCM_RECONST)) {

        if (hdr->function == AMCMR_RFINTERLOCK) {

            type = AMCM_FUNC_KIND_REQUEST;
            scan_proc->SetProcessing(scan_proc->GetProcessing() | funcBit);

            if (E_NO_ERROR != BeforePrescan(type, body)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("BeforePrescan() failed."));
            }

        } else if (hdr->function == AMCMA_RFINTERLOCK_DONE) {

            type = AMCM_FUNC_KIND_ANSWER;

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                      _T("[Response type: ANSWER]"));
            scan_proc->SetProcessing(scan_proc->GetProcessing() ^ funcBit);
            scan_proc->SetRequest(scan_proc->GetRequest() ^ funcBit);

            if (E_NO_ERROR != BeforePrescan(type, body)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("BeforePrescan() failed."));
            }
        }

        str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                       acqman->GetStr(body->status),
                       acqman->GetStr(type),
                       acqman->GetStr(hdr->function));

        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, str_msg);

    } else if (body->status == AMCM_NACK) {

        if (hdr->function == AMCMR_RFINTERLOCK) {

            type = AMCM_FUNC_KIND_REQUEST;
            scan_proc->SetRequest(scan_proc->GetRequest() ^ funcBit);

        } else if (hdr->function == AMCMA_RFINTERLOCK_DONE) {

            type = AMCM_FUNC_KIND_ANSWER;

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                      _T("[Response type: ANSWER]"));

            scan_proc->SetProcessing(scan_proc->GetProcessing() ^ funcBit);
            scan_proc->SetRequest(scan_proc->GetRequest() ^ funcBit);
        }

        str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                       acqman->GetStr(body->status),
                       acqman->GetStr(type),
                       acqman->GetStr(hdr->function));

        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, str_msg);

        if (hdr->function == AMCMR_RFINTERLOCK) {
            ErrReport(type, IDS_ERR_ACQMAN_RFINTERLOCK, _T("IDS_ERR_ACQMAN_RFINTERLOCK"), body);			// Added By KT/ACE-2/15-06-2009/Multilingualisation

        } else if (hdr->function == AMCMA_RFINTERLOCK_DONE) {
            ErrReport(type, IDS_ERR_ACQMAN_RFINTERLOCK_DONE, _T("IDS_ERR_ACQMAN_RFINTERLOCK_DONE"), body); // Added By KT/ACE-2/15-06-2009/Multilingualisation
        }

    } else {

        if (hdr->function == AMCMR_RFINTERLOCK) {

            type = AMCM_FUNC_KIND_REQUEST;
            int result = scan_proc->GetRequest();
            result ^= funcBit;
            //MRPdefect812
            scan_proc->SetRequest(result);
            //MRPdefect812

        } else if (hdr->function == AMCMA_RFINTERLOCK_DONE) {

            type = AMCM_FUNC_KIND_ANSWER;

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                      _T("[Response type: ANSWER]"));

            scan_proc->SetProcessing(scan_proc->GetProcessing() ^ funcBit);
            scan_proc->SetRequest(scan_proc->GetRequest() ^ funcBit);
        }

        if (hdr->function == AMCMR_RFINTERLOCK) {
            ErrReport(type, IDS_ERR_ACQMAN_RFINTERLOCK, _T("IDS_ERR_ACQMAN_RFINTERLOCK"), body);				// Added By KT/ACE-2/15-06-2009/Multilingualisation

        } else if (hdr->function == AMCMA_RFINTERLOCK_DONE) {
            ErrReport(type, IDS_ERR_ACQMAN_RFINTERLOCK_DONE, _T("IDS_ERR_ACQMAN_RFINTERLOCK_DONE"), body);		// Added By KT/ACE-2/15-06-2009/Multilingualisation
        }

        str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                       acqman->GetStr(body->status),
                       acqman->GetStr(type),
                       acqman->GetStr(hdr->function));

        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, str_msg);

        if (scan_proc->GetProcessing()) {
            //  scan_proc->ResetCurrentRequest();
        }
    }
}


//****************************Method Header************************************
//Method Name   :PrepareRequest()
//Author        :PATNI/ASG
//Purpose       :This method prepares the InterLock request that will be sent
//              to Acqman
//*****************************************************************************
bool CInterLock::PrepareRequest(
)
{
    LPCTSTR FUNC_NAME = _T("CInterLock::PrepareRequest");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace"));

    CScanProcedure* scan_proc = GetScanProcedure();
    CPqm* l_pqmmanager = scan_proc->GetAcqManager()->GetPqm();
    CPQMStudy* l_study = l_pqmmanager->GetStudy();
    CPqmProtocol* curr_protocol = scan_proc->GetScanProcProtocol();

    memset(&m_header, 0, sizeof(AM_ClientMessageHeader_t));
    memset(&m_data, 0, sizeof(AM_ClientToAcqMan_unicode_t));

    m_header.function = AMCMM_RFINTERLOCK;

    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
              _T("[Request: AMCMM_RFINTERLOCK]"));

    int anatomy = -1;

    if (curr_protocol != NULL) {

        m_data.protocol_node = curr_protocol->GetProtocol();
        anatomy = curr_protocol->GetPqmPrMode()->GetAnatomy();

    } else {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                  _T("curr_protocol pointer is NULL !!!"));
        return false;
    }

    // + V9.25 coil support@AMT
    int     l_coil_id = -1;

    if (l_pqmmanager->IsSaturnCoilSystem()) {
        //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
        //PqmCoil_t *l_coil;
        PqmCoil_t* l_coil = 0;
        l_study->GetTxCoilInSaturn(curr_protocol->GetCoilDBTable(), &l_coil);

        //+Patni-Hemant/2010Mar15/Added/ACE-Phase#3/CDS Requirement
        if (NULL == l_coil) {

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                      _T("GetTxCoilInSaturn failed."));

            return false ;
        }

        //-Patni-Hemant/2010Mar15/Added/ACE-Phase#3/CDS Requirement


        l_coil_id = l_coil->coil_id;

    } else {
        l_coil_id = curr_protocol->GetPqmPrMode()->GetRecvCoilId();
    }

    float	inter_lock = 0.0f;
    BOOL return_val = GetInterLock(l_coil_id, anatomy,
                                   l_pqmmanager->GetStudy()->GetWeight(), &inter_lock);

    if (return_val == E_ERROR || inter_lock == 0.0) {


        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
            CPqmUtilities::GetMultiLingualString(_T("IDS_INTERLOCK_CALC_ERROR")),
            MPlus::EH::Information,
            _T("PQM"),
            _T("PQM"));

        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("GetInterLock ERROR."));
        return false;
    }



    m_data.param.interlock.watt = inter_lock;

    bool l_ret = SendMessage();

    if (l_ret == false) {

        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
            CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_CONNECT_ACQMAN")),
            MPlus::EH::Information,
            _T("PQM"),
            _T("PQM"));

        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Cannot Connect to AcqMan"));
    }

    return l_ret;
}

//****************************Method Header************************************
//Method Name   :GetInterLock()
//Author        :PATNI/ASG
//Purpose       :This method does the calculation of the inter-lock value
//               depending on the weight of the patient
//*****************************************************************************
BOOL CInterLock::GetInterLock(
    const int receiver_coilId,
    const int anatomy,
    const float weight,
    float*	inter_lock
)const
{
    LPCTSTR FUNC_NAME = _T("CInterLock::GetInterLock");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    //- Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    //DB_FUNC_ENTER("[PqmGetInterLock]:");
    if (m_scan_procedure == NULL) {	//Patni-MJC/2009Aug17/Modified/cpp test corrections Need to Check
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_scan_procedure pointer is NULL !!!"));
        return E_ERROR;
    }

    if (weight <= 0 || anatomy < 0) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Invalid Weight/Anatomy..."));
        return	E_ERROR;
    }

    int weight_class = 0;

    if (weight < 20.0)	{
        weight_class = 0;

    } else if (weight < 40.0) {
        weight_class = 1;

    } else if (weight < 60.0) {
        weight_class = 2;

    } else if (weight < 80.0) {
        weight_class = 3;

    } else if (weight < 100.0) {
        weight_class = 4;

    } else 	{/* max */
        weight_class = 5;
    }

    InterlockDB_t*	lock_coil = NULL, *idbCoilList = NULL;
    CPqm* pqm = m_scan_procedure->GetAcqManager()->GetPqm();
    CInterlockData* l_interlock_data = pqm->GetDataManager()->GetInterlockData();

    for (int i = 0; i < l_interlock_data->GetCoilCount(); i++) {

        idbCoilList = l_interlock_data->GetCoilList();

        if (idbCoilList[i].key == receiver_coilId) {

            lock_coil = &idbCoilList[i];
            break;
        }
    }

    if (lock_coil == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("lock_coil is NULL !!!"));
        return E_ERROR;
    }

    int lock_val = 0;
    lock_val = (int)lock_coil->value[weight_class];
    CPqmCoilData* l_coil_data = CDataManager::GetInstance()->GetPQMCoilData();

    CCoil l_coil;
    l_coil_data->PqmCoilGetTxCoil(receiver_coilId, l_coil);

    /** For WB coil **/
    InterlockDB_t*	idbWeightList = NULL;

    if ((l_coil.IsWBCoil()) || (l_coil.GetPort() == CPqmCoilData::GetWholeBodyPort())) {

        InterlockDB_t* lock_anat = NULL;

        for (i = 0; i < l_interlock_data->GetWeightCount(); i++) {

            idbWeightList = l_interlock_data->GetWeightList();

            if (anatomy == idbWeightList[i].key) {

                lock_anat = &idbWeightList[i];
                break;
            }
        }

        if (lock_anat == NULL) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("lock_anat is NULL !!!"));
            return E_ERROR;
        }

        *inter_lock =
            (float)(lock_val * lock_anat->value[weight_class]);
        *inter_lock /= 100;

        CString l_trace_str;
        l_trace_str.Format(_T("interlock = %d * %d /100"), lock_val, lock_anat->value[weight_class]);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_str);

    } else {
        *inter_lock = (float)lock_val;
    }


    CString l_trace_string;
    l_trace_string.Format(_T("*inter_lock=%.3f"), *inter_lock);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_string);

    return E_NO_ERROR;
}

//****************************Method Header************************************
//Method Name   :GetLockValue()
//Author        :PATNI/HAR
//Purpose       :
//*****************************************************************************
int CInterLock::GetLockValue(
    const int f_receiver_coil_id,
    const int f_weight_class
)
{
    LPCTSTR FUNC_NAME = _T("CInterLock::GetLockValue");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    CPTString file_name_interlock;
    CPQMXMLParser* ipcparser = new CPQMXMLParser();
    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    //wchar_t buffer[MAX_PATH];
    wchar_t buffer[MAX_PATH] = {0};
    ::GetEnvironmentVariable(_T("MRMP_Config"), buffer, MAX_PATH);
    file_name_interlock = buffer;
    file_name_interlock += L"\\";
    file_name_interlock += L"PQM\\";
    file_name_interlock += INTERLOCK_FILENAME;

    int weight_value = 0;

    // load xml file
    if (ipcparser->LoadXMLFile(file_name_interlock)) {
        int nodecnt = ipcparser->GetNodeCnt();
        CPTString weight_value_str;
        int coilId = 0;

        for (int index = 0; index < nodecnt; index++) {

            CPTString coilIdstr = ipcparser->GetAttributeValue(
                                      index,
                                      NODE_TAG_NAME_COIL_INFORMATION,
                                      CHILD_TAG_NAME_COIL_ID,
                                      ATTRIBUTE_NAME_1);

            coilId = ipcparser->ReadLongValue(coilIdstr);

            if (coilId != f_receiver_coil_id) {
                continue;

            } else {
                switch (f_weight_class) {
                    case 0:
                        weight_value_str = ipcparser->GetAttributeValue(
                                               index,
                                               NODE_TAG_NAME_COIL_INFORMATION,
                                               CHILD_TAG_NAME_WGT_CLASS1,
                                               ATTRIBUTE_NAME_1);
                        break;

                    case 1:
                        weight_value_str = ipcparser->GetAttributeValue(
                                               index,
                                               NODE_TAG_NAME_COIL_INFORMATION,
                                               CHILD_TAG_NAME_WGT_CLASS2,
                                               ATTRIBUTE_NAME_1);
                        break;

                    case 2:
                        weight_value_str = ipcparser->GetAttributeValue(
                                               index,
                                               NODE_TAG_NAME_COIL_INFORMATION,
                                               CHILD_TAG_NAME_WGT_CLASS3,
                                               ATTRIBUTE_NAME_1);
                        break;

                    case 3:
                        weight_value_str = ipcparser->GetAttributeValue(
                                               index,
                                               NODE_TAG_NAME_COIL_INFORMATION,
                                               CHILD_TAG_NAME_WGT_CLASS4,
                                               ATTRIBUTE_NAME_1);
                        break;

                    case 4:
                        weight_value_str = ipcparser->GetAttributeValue(
                                               index,
                                               NODE_TAG_NAME_COIL_INFORMATION,
                                               CHILD_TAG_NAME_WGT_CLASS5,
                                               ATTRIBUTE_NAME_1);
                        break;

                    case 5:
                        weight_value_str = ipcparser->GetAttributeValue(
                                               index,
                                               NODE_TAG_NAME_COIL_INFORMATION,
                                               CHILD_TAG_NAME_WGT_CLASS6,
                                               ATTRIBUTE_NAME_1);
                        break;

                    default:
                        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Invalid Weight"));
                }

                weight_value = ipcparser->ReadLongValue(weight_value_str);
            }
        }

        DEL_PTR(ipcparser);

    } else {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Unable to Load Interlock.xml"));

        return -1;
    }

    return weight_value;
}

//****************************Method Header************************************
//Method Name   :GetLockAnatomy()
//Author        :PATNI/HAR
//Purpose       :
//*****************************************************************************
int CInterLock::GetLockAnatomy(
    const int f_anatomy,
    const int f_weight_class
)
{
    LPCTSTR FUNC_NAME = _T("CInterLock::GetLockAnatomy");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));


    CPTString file_name_weight_ratio;
    CPQMXMLParser* ipcparser = new CPQMXMLParser();
    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    //wchar_t buffer[MAX_PATH];
    wchar_t buffer[MAX_PATH] = {0};
    ::GetEnvironmentVariable(_T("MRMP_Config"), buffer, MAX_PATH);
    file_name_weight_ratio = buffer;
    file_name_weight_ratio += L"\\";
    file_name_weight_ratio += L"PQM\\";
    file_name_weight_ratio += WEIGHT_RATIO_FILENAME;

    int weight_value = 0;

    // load xml file
    if (ipcparser->LoadXMLFile(file_name_weight_ratio)) {
        int nodecnt =  ipcparser->GetNodeCnt();
        int anatomy = 0;
        CPTString weight_value_str;

        for (int index = 0; index < nodecnt; index++) {

            CPTString l_anatomy_str = ipcparser->GetAttributeValue(
                                          index,
                                          NODE_TAG_NAME_COIL_INFORMATION,
                                          CHILD_TAG_NAME_ANATOMY,
                                          ATTRIBUTE_NAME_1);

            anatomy = ipcparser->ReadLongValue(l_anatomy_str);

            if (anatomy != f_anatomy) {
                continue;

            } else {
                switch (f_weight_class) {
                    case 0:
                        weight_value_str = ipcparser->GetAttributeValue(
                                               index,
                                               NODE_TAG_NAME_COIL_INFORMATION,
                                               CHILD_TAG_NAME_WGT_CLASS1,
                                               ATTRIBUTE_NAME_1);
                        break;

                    case 1:
                        weight_value_str = ipcparser->GetAttributeValue(
                                               index,
                                               NODE_TAG_NAME_COIL_INFORMATION,
                                               CHILD_TAG_NAME_WGT_CLASS2,
                                               ATTRIBUTE_NAME_1);
                        break;

                    case 2:
                        weight_value_str = ipcparser->GetAttributeValue(
                                               index,
                                               NODE_TAG_NAME_COIL_INFORMATION,
                                               CHILD_TAG_NAME_WGT_CLASS3,
                                               ATTRIBUTE_NAME_1);
                        break;

                    case 3:
                        weight_value_str = ipcparser->GetAttributeValue(
                                               index,
                                               NODE_TAG_NAME_COIL_INFORMATION,
                                               CHILD_TAG_NAME_WGT_CLASS4,
                                               ATTRIBUTE_NAME_1);
                        break;

                    case 4:
                        weight_value_str = ipcparser->GetAttributeValue(
                                               index,
                                               NODE_TAG_NAME_COIL_INFORMATION,
                                               CHILD_TAG_NAME_WGT_CLASS5,
                                               ATTRIBUTE_NAME_1);
                        break;

                    case 5:
                        weight_value_str = ipcparser->GetAttributeValue(
                                               index,
                                               NODE_TAG_NAME_COIL_INFORMATION,
                                               CHILD_TAG_NAME_WGT_CLASS6,
                                               ATTRIBUTE_NAME_1);
                        break;

                    default:
                        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Invalid Weight"));
                }

                weight_value = ipcparser->ReadLongValue(weight_value_str);
            }
        }

        DEL_PTR(ipcparser);

    } else {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Unable to Load Interlock.xml"));

        return -1;
    }

    return weight_value;
}



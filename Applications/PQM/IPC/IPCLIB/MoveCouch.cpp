//REDMINE-1213_Update_03_MAY
//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-780
//Redmine-781
//Redmine-782
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: Movecouch.cpp
 *  Overview: Structure of CMoveCouch class.
 *  Last Modified: 2008/8/11 15:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2008/8/11 15:00:00     Implemented SM3 requirements & Formatted
 *
 *****************************************************************************/

//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MoveCouch.h"
#include <math.h>
#include <PQM/PQMLogMgr.h>
//Patni-PJS/2010Dec21/Added/REDMINE-1098/Part-I
#include <PQM/PqmUtilities.h>
#include <PQM/resource.h>
#include "pqm.h"
#include "pqmpmacqman.h"
#include "PqmPrMode.h"
#include "couchconfig.h"
#include "scanprocedure.h"
#include "VarFieldHandle.h" //TMSC-Tanoue/2010Aug25/MVC008345
#include "PQMDebugInfo.h"
#include "pqmAMB.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//*******************************Method Header*********************************
//Method Name    : CMoveCouch()
//Author         : PATNI/MSN
//Purpose        : Constructor
//*****************************************************************************
CMoveCouch::CMoveCouch(
    CScanProcedure* f_scan_proc
): CScanTransaction(f_scan_proc),
    m_function(0),
    m_position(0.0)
{
    LPCTSTR FUNC_NAME = _T("CMoveCouch::CMoveCouch");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ScanTransaction Created"));
}


//*******************************Method Header*********************************
//Method Name    : ~CMoveCouch()
//Author         : PATNI/MSN
//Purpose        : Destructor
//*****************************************************************************
CMoveCouch::~CMoveCouch()
{
    LPCTSTR FUNC_NAME = _T("CMoveCouch::~CMoveCouch");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ScanTransaction Destroyed"));
}

//****************************Method Header************************************
//Method Name   : PrepareRequest()
//Author        : PATNI/MSN
//Purpose       : Prepare request by setting the action and request before sending it to Acqman.
//*****************************************************************************
bool CMoveCouch::PrepareRequest()
{
    LPCTSTR FUNC_NAME = _T("CMoveCouch::PrepareRequest");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    //+Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    time_t              ts = 0L;
    VfFieldSpec_t       fieldSpec[2] = {0};
    VfPathElem_t        path[5] = {0};
    //-Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_scan_procedure == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_scan_procedure pointer is NULL"));
        return false;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    CPqmProtocol* l_protocol = m_scan_procedure->GetScanProcProtocol();
    CPqmPmAcqman* l_acqman = m_scan_procedure->GetAcqManager();//Patni-MJC/2009Aug17/Modified/cpp test corrections
    CPqm* l_pqm = l_acqman->GetPqm();
    CPQMStudy* l_study = l_pqm->GetStudy();
    // CCouchConfig* l_couch_config = m_scan_procedure->GetAcqManager()->GetPqm()->GetCouchConfigPtr();//Patni-MJC/2009Aug17/Modified/cpp test corrections Need to Check
    CCouchConfig* l_couch_config  = l_pqm->GetCouchConfigPtr();//Patni-MJC/2009Aug17/Modified/cpp test corrections

    int32_t	mode = 0 ;
    mode = VFC_MOVE_COUCH_METHOD_PLAN_CENTER_POSI ;

    //+Patni-DKH/2010Apr12/Modified/Phase3#MCM0229-00187
    //if (l_pqm->GetMoveLocatorMode()) {
    if (l_protocol->GetcouchMethodFlg() == VFC_MOVE_COUCH_METHOD_PARENT_POSI) {
        //-Patni-DKH/2010Apr12/Modified/Phase3#MCM0229-00187
        if (l_protocol->GetLocatorImages(0).subtree_name == SVN_ST_PROTOCOL) {
            if (l_study->GetItemFromProtocol(l_protocol->GetLocatorImages(0).index)) {	/* e‰æ‘œˆÊ’u */
                mode = VFC_MOVE_COUCH_METHOD_PARENT_POSI ;
            }
        }
    }

    bzero(path,     sizeof(path));
    bzero(fieldSpec,     sizeof(fieldSpec));
    path[0].subtree_name = SVN_ST_PROTOCOL;
    path[0].index = l_protocol->GetProtocol();
    int n = 0 ;
    VFF_SET_ARGS(fieldSpec[n], SVN_EXE_MOVE_COUCH_METHOD, VFT_INT32,
                 sizeof(int32_t) , &mode, VFO_REPLACE);
    n++;

    //+Patni-SS/2009Apr01/PSP1#VFLOCK
    CString str = _T("");
    str.Format(_T("Protocol = %d, mode = %d,"), l_protocol->GetProtocol(), mode);
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, str);

    CVarFieldHandle* vf_handle = l_study->GetVfDBHandle();

    if (!vf_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("vf_handle is NULL"));
        return false;
    }

    int numDone = 0  ;

    if (E_NO_ERROR != vf_handle->PutFields(path, SVD_ST_PROTOCOL, fieldSpec, n, (int*) & numDone, &ts)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
        return false;
    }

    if (n != numDone) {
        CPQMLogMgr::GetInstance()->WriteEventLog(_T("VfPutFields(SVN_EXE_MOVE_COUCH_METHOD) error"), ERROR_LOG_INFORMATION1 ,
                _T("CScanTransaction.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82
        //	ERROR_LOG("VfPutFields(SVN_EXE_MOVE_COUCH_METHOD) error " ) ;
    }

    // bzero(&acqRequest,sizeof(PqmAcqRequest_t));

    /*   PqmDebugAcqMonitor("Send MoveCouch %g -> %g\n",
    PqmAcqGlobal.curObj->mode.couchPos,
    position);*/

    CString str_msg(_T(""));
    str_msg.Format(_T("Send MoveCouch %g -> %g\n"),
                   l_protocol->GetPqmPrMode()->GetCouchPos() ,
                   m_position);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    //+Patni-Raj/2010Jan14/Added/PH#3#/Debug Window
    CPQMDebugInfo::GetInstance(l_pqm)->PqmDebugAcqMonitor(_T("Send MoveCouch %g -> %g"),
            l_protocol->GetPqmPrMode()->GetCouchPos(),
            m_position);

    //-Patni-Raj/2010Jan14/Added/PH#3#/Debug Window
    /* Set function */
    m_header.function = (func_t)m_function;

    if (m_function == AMCMM_MOVECOUCH) {
        m_data.param.moveCouch.position = m_position;

    } else {
        m_data.param.remoteMoveCouch.position = m_position;

        //+Patni-SS/20091014/Modified/TMSC review comments
        //GetCouchRange method name is not correct as per function so it has changed to GetCouchSpeedRange
        if (((l_pqm->GetHiSpeedCouch()) && fabs(m_position) > fabs(l_couch_config->GetCouchSpeedRange()))
            || (l_pqm->PqmPmIsActive(K_PM_TYPE_AMB) && l_protocol->GetAMBModeFlag() != VFC_AMB_MODE_NONE)) {
            m_data.param.remoteMoveCouch.speed = ACQ_REMOTE_MOVECOUCH_FAST;

        } else {
            m_data.param.remoteMoveCouch.speed = ACQ_REMOTE_MOVECOUCH_SLOW;
        }

        //PqmAcqGlobal.scanAbort = PqmAcqScanAbort;
        m_scan_procedure->SetScanAbort(PQM_SCAN_ABORT);
    }

    //+Patni-SS/2010Mar31/Added/MSA00251-00284
    /*  if (l_protocol) {
          m_scan_procedure->SetProcessing(PROCESSNG_MOVE_COUCH);
      }*/
    //-Patni-SS/2010Mar31/Added/MSA00251-00284

    if (!l_acqman->SendMessage(this)) {

        return false;
    }

    return true;
}

void CMoveCouch::SetFuntionAndPosition(
    const int f_function, const float f_position
)
{
    m_function = f_function;
    m_position = f_position;
}

//****************************Method Header************************************
//Method Name   : ProcessResponse()
//Author        : PATNI/MSN
//Purpose       : Process the incoming response coming from Acqman
//*****************************************************************************
void CMoveCouch::ProcessResponse(
    AM_ClientMessageHeader_t* const f_header,
    AM_AcqManToClient_t* const f_data
)
{
    LPCTSTR FUNC_NAME = _T("CMoveCouch::ProcessResponse");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    CScanProcedure* scan_proc = GetScanProcedure();

    if (f_header) {
        m_header = *f_header;
    }

    //+Patni-RUP/2009Aug21/Modified/cpp test corrections
    else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_header pointer is NULL"));
        return;
    }

    //-Patni-RUP/2009Aug21/Modified/cpp test corrections

    CPqmPmAcqman* acqman = scan_proc->GetAcqManager();
    CPqm* l_pqm = acqman->GetPqm();

    int type = 0;

    int funcBit = acqman->GetFunctionBit(f_header->function);

    CString str_msg(_T(""));
    str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                   acqman->GetStr(f_data->status),
                   acqman->GetStr(type),
                   acqman->GetStr(f_header->function));

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    if ((f_data->status == AMCM_SUCCESS) ||
        (f_data->status == AMCM_RECONST)) {

        if (f_header->function == AMCMA_REMOTE_MOVECOUCH_DONE) {

            type = AMCM_FUNC_KIND_ANSWER;


            scan_proc->SetProcessing(scan_proc->GetProcessing() ^ funcBit);
            scan_proc->SetRequest(scan_proc->GetRequest() ^ funcBit);

            if (!PostProcess(
                    f_data->couchInfo.position,
                    f_data->couchInfo.couchStatusFlag,
                    AMCMA_REMOTE_MOVECOUCH_DONE)) {
                //+Patni-PJS/2009May10/Modify/IR-82 review comment
                str_msg.Empty();
                str_msg.LoadString(IDS_CANNOT_PROCEED);
                CPQMLogMgr::GetInstance()->WriteEventLog(
                    str_msg,
                    ERROR_LOG_CRUCIAL1 , _T("MoveCouch.cpp"),
                    __LINE__);//Patni-KSS/2010May19/ADDED/IR-82
                /*CPQMLogMgr::GetInstance()->WriteEventLog(
                    IDS_CANNOT_PROCEED,
                    ERROR_LOG_CRUCIAL1 , _T("MoveCouch.cpp"));*/
                //-Patni-PJS/2009May10/Modify/IR-82 review comment
                //Patni-PJS+HEMANT/COMMENTED On 9/15/2009 10:21:21 PM/ MVC2336
                //l_pqm->WriteToUI(PQM_MSG_ABORT_WAIT);

                return;
            }

            //	PreScanGo(AMCMA_REMOTE_MOVECOUCH_DONE);
            //+Patni-SS/2010Mar31/Added/MSA00251-00284

        } else if (f_header->function == AMCMR_REMOTE_MOVECOUCH) {

            type = AMCM_FUNC_KIND_REQUEST;
            scan_proc->SetProcessing(scan_proc->GetProcessing() | funcBit);
            str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                           acqman->GetStr(f_data->status),
                           acqman->GetStr(type),
                           acqman->GetStr(f_header->function));

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);
        }

        //-Patni-SS/2010Mar31/Added/MSA00251-00284

    } else if (f_data->status == AMCM_NACK) {

        if (f_header->function == AMCMA_REMOTE_MOVECOUCH_DONE) {

            type = AMCM_FUNC_KIND_ANSWER;

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("[Response type: ANSWER]"));

            scan_proc->SetProcessing(scan_proc->GetProcessing() ^ funcBit);
            scan_proc->SetRequest(scan_proc->GetRequest() ^ funcBit);
            // + LOKESH Intgr 5 Bug Fix
            //m_pqm_ptr->WriteToUI(PQM_MSG_GATE_SCAN_END);
            // - LOKESH Intgr 5 Bug Fix
        }//+Patni-PJS/2010Dec21/Added/REDMINE-1098/Part-I

        else if (f_header->function == AMCMR_REMOTE_MOVECOUCH) {
            //Display message in error view
            CPQMStudy* l_study = l_pqm->GetStudy();

            //CString l_error_string = CPqmUtilities::GetMultiLingualString(_T("IDS_MOVE_COUCH_CONFIRM_COUCH_ERROR"));
            CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
                CPqmUtilities::GetMultiLingualString(_T("IDS_MOVE_COUCH_CONFIRM_COUCH_ERROR")),
                MPlus::EH::Information, _T("Trace CMoveCouch::ProcessResponse"), _T("PQM"));

            l_study->SetScanModeOfHead();
            scan_proc->ResetCurrentRequest();
            l_pqm->WriteToUI(PQM_MSG_ABORT_WAIT);
            l_pqm->GetAMB()->PqmPmAbortAmb();
        }

        //-Patni-PJS/2010Dec21/Added/REDMINE-1098/Part-I

        str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                       acqman->GetStr(f_data->status),
                       acqman->GetStr(type),
                       acqman->GetStr(f_header->function));

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    } else {

        if (f_header->function == AMCMA_REMOTE_MOVECOUCH_DONE) {

            type = AMCM_FUNC_KIND_ANSWER;

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("[f_header->function = AMCMA_REMOTE_MOVECOUCH_DONE && Response type: ANSWER]"));

            scan_proc->SetProcessing(scan_proc->GetProcessing() ^ funcBit);
            scan_proc->SetRequest(scan_proc->GetRequest() ^ funcBit);
        }

        str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                       acqman->GetStr(f_data->status),
                       acqman->GetStr(type),
                       acqman->GetStr(f_header->function));

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);
    }
}



//REDMINE1213
//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-774
//Redmine-778
//Redmine-780
//Redmine-781
//Redmine-782
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: ShimmingRequest.cpp
 *  Overview: Structure of CShimmingRequest class.
 *  Last Modified:
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2008/6/02 12:00:00     New class Implemented
                                                            After restructure
 *****************************************************************************/

#include "stdafx.h"
#include "ShimmingRequest.h"
//Patni-HAR/2010Feb18/Added/MSA0251-00176
#include <PQM/PqmUtilities.h>
#include <PQM/PQMLogMgr.h>
#include <PQM/resource.h>
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"
#include "pqmstudy.h"
#include "Pqm.h"    // Patni - HAR / 2009Mar18 / Added / PSP Defect Sheet : Defect#212
#include "pqmpmacqman.h"
#include "scanprocedure.h"
#include "PQMDebugInfo.h"
#include "pqmAMB.h"

VfPathElem_t s_shimimagepath[] = {
    {SVN_ST_PROTOCOL, 0},
    {SVN_ST_SUBPROTOCOL, 0},
    {SVN_ST_SLICEGROUP, 0},
    {SVN_ST_SLICE, 0},
    {SVN_ST_IMAGE, 0}
};

VfPathElem_t s_shimechopath[] = {
    {SVN_ST_PROTOCOL, 0},
    {SVN_ST_PROCESSED, 0},
    {SVN_ST_ECHO, 0},
    {SVN_ST_POSITION, 0}
};

static int l_execflag = 0;
static int l_makeimage = 0;
SShimGlobal_t*   g_shim_global = NULL; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections

//+Patni-HAR/2010Feb18/Added/MSA0251-00176
// MEITEC/2010Jun07/Modified/90 -> 120
#define	SHIMMING_TIMEOUT_DEFAULT		(120*1)
unsigned CShimmingRequest::m_shimming_timeout = 0L;
//-Patni-HAR/2010Feb18/Added/MSA0251-00176

//*******************************Method Header*********************************
//Method Name    :CShimmingRequest
//Author         :PATNI/LK
//Purpose        :default constructor
//*****************************************************************************
CShimmingRequest::CShimmingRequest():
    m_parent_study(NULL) //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
{
}

//*******************************Method Header*********************************
//Method Name    :CShimmingRequest
//Author         :PATNI/LK
//Purpose        :constructor
//*****************************************************************************
CShimmingRequest::CShimmingRequest(
    CPQMStudy* f_pqmstudy
): m_parent_study(f_pqmstudy
                     )
{

}

//*******************************Method Header*********************************
//Method Name    :~CShimmingRequest
//Author         :PATNI/LK
//Purpose        :destructor
//*****************************************************************************
CShimmingRequest::~CShimmingRequest(
)
{

}

//*******************************Method Header*********************************
//Method Name    :ProcessShimmingRequest
//Author         :PATNI/LK
//Purpose        :Prepares shim request structure and communicates with msShim
//*****************************************************************************
SR_STATUS CShimmingRequest::ProcessShimmingRequest(
    const SShimReqInfo_t* f_shimreq_info
)
{
    LPCTSTR FUNC_NAME = _T("CShimmingRequest::ProcessShimmingRequest");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CShimmingRequest::ProcessShimmingRequest"));

    SR_STATUS       status = SHIMMING_CANCEL;
    int             l_msshim_status    = E_ERROR;
    int             l_shim_quit_status = E_ERROR;
    g_shim_global = new SShimGlobal_t();


    if (PrepareShimReqInfo(f_shimreq_info/*, g_shim_global*/)) {

        int l_shiminfo_mode = g_shim_global->shimStudyP->shimInfo_mode;
        int l_shimflag = g_shim_global->shimInfoP->shim_flag;

        switch (l_shimflag) {
            case VFC_SHIM_HIGH_RESOLUTION:

                // for HRS sequence scan return the shimming status as TRUE
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("High Resolution Shimming for MRS. normal return"));

                g_shim_global->shimInfoP->adjust_processed = 1;
                l_shim_quit_status = SHIM_NORMAL_END;
                status = SHIMMING_ADJUST_DONE;
                break;

            case VFC_SHIM_NONE:

                // for normal sequence, return error status
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Shimming none"));

                l_shim_quit_status = SHIM_CANCEL;
                status = SHIMMING_CANCEL;
                break;

            case VFC_SHIM_STANDARD:

                switch (l_shiminfo_mode) {
                        // if the shimming mode is Auto then communicate with msShim application
                    case SHIM_AUTO_MODE:

                        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Shimming mode is Auto Mode"));

                        //+Patni-PJS/2010Jan14/Added/MVC006466
                        if (!CheckShimDataFiles()) {
                            CString l_cserror_string = _T("Shim setup data is not present");
                            CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(l_cserror_string,
                                    MPlus::EH::Information, _T("ShimmingRequest.cpp"), _T("PQM"));

                            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Cannot get shim default"));

                            l_shim_quit_status = SHIM_MS_ERROR;
                            status = SHIMMING_CANCEL;
                            break;
                        }

                        //-Patni-PJS/2010Jan14/Added/MVC006466
                        l_msshim_status = msShimReq(/*g_shim_global*/);

                        if (l_msshim_status == E_NO_ERROR) {
                            l_shim_quit_status = SHIM_NORMAL_END;
                            status = SHIMMING_ADJUST_DONE;

                        } else if (l_msshim_status == E_SHIM_SLICE_ORI_ERROR) {
                            l_shim_quit_status = SHIM_SLICE_ORI_ERROR;
                            status = SHIMMING_CANCEL;

                        } else {
                            l_shim_quit_status = SHIM_MS_ERROR;
                            status = SHIMMING_CANCEL;
                        }

                        break;

                        // if the mode is Manual then throw an error and return the status
                    case SHIM_MANUAL_MODE:
                        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Shimming mode is Manual Mode"));
                        l_shim_quit_status = SHIM_CANCEL;
                        status = SHIMMING_CANCEL;
                        break;

                    default:
                        l_shim_quit_status = SHIM_FATAL_ERROR;
                        status = SHIMMING_CANCEL;
                        break;
                }

                break;

            default:
                l_shim_quit_status = SHIM_FATAL_ERROR;
                status = SHIMMING_CANCEL;
                break;
        }

    } else {
        l_shim_quit_status = SHIM_FATAL_ERROR;
    }

    VfPathElem_t    l_vfpath[SVD_ST_PROCESSED] = {0};
    ::memset(l_vfpath, 0, sizeof(VfPathElem_t) * SVD_ST_PROCESSED);

    QuitShimmingRequest(l_shim_quit_status/*, g_shim_global*/, l_vfpath);

    ProcessResponse(status, l_vfpath, g_shim_global->shimInfoP->shim_flag);

    return status;

}

void CShimmingRequest::SendLocalShimReqTomsShim(
    msInteractiveShim_t* f_shim_data
)
{
    // + Patni - HAR / 2009 Mar 06 / Added / MRS
    LPCTSTR FUNC_NAME = _T("CShimmingRequest::SendLocalShimReqTomsShim");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SendLocalShimReqTomsShim"));
    msCalcRequest_t  l_ms_reply;
    InteractiveRequestParams_t l_req_p;
    int32_t  nSocId = 0; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
    int l_count = 0;
    int l_status = E_NO_ERROR;
    memset(&l_ms_reply, 0, sizeof(msCalcRequest_t));
    memset(&l_req_p, 0, sizeof(InteractiveRequestParams_t));

    CString l_str(_T(""));

    while (l_count < 10) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Before msInteractiveProcOpen"));

        if ((l_status  = msInteractiveProcOpen(MS_TYPE_SHIMMING, &nSocId))) { /* msShimプロセス要求 */

            f_shim_data->status = E_ERROR;
            break;
        }

        l_str.Format(_T("Socket id = %d"), nSocId);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
        l_req_p.msType = MS_TYPE_SHIMMING;
        l_req_p.InteractiveParams.InteractiveShim.shimming = f_shim_data ;

        /* msShimへ処理要求を送付する */
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Before msInteractiveProcRequest"));
        l_status  = msInteractiveProcRequest(&l_req_p , nSocId);
        if_error(l_status) {
            msInteractiveProcClose(nSocId);
            f_shim_data->status = E_ERROR;
            break;
        }

        /* msShimからの終了応答を待つ */
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Before msInteractiveProcSync"));
        l_status  = msInteractiveProcSync(&l_ms_reply , nSocId);

        if ((l_status = msInteractiveProcClose(nSocId)) != E_NO_ERROR) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("msInteractiveProcClose() - Error"));
        }

        // - Patni - HAR / 2009 Mar 06 / Added / MRS
        /*if_error(l_status ) {
        	//shimPrintf("%s : %d. msInteractiveProcSync() error\n",__FILE__,__LINE__);

        	msInteractiveProcClose(nSocId);

        	if(l_count++ >= 10 )
        		f_shim_data->status = E_ERROR;

        } else */{
            f_shim_data->patient[0] = l_ms_reply.CalcParams.ShimFA.patient[0];
            f_shim_data->patient[1] = l_ms_reply.CalcParams.ShimFA.patient[1];
            f_shim_data->patient[2] = l_ms_reply.CalcParams.ShimFA.patient[2];
            f_shim_data->patient[3] = l_ms_reply.CalcParams.ShimFA.patient[3];

            if (f_shim_data->patient[1] < 1) {

                f_shim_data->status = E_ERROR;

            }

            f_shim_data->status = E_NO_ERROR;
            break;
        }
    }

}

//*******************************Method Header*********************************
//Method Name    :ShimmingTimerReset
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
void CShimmingRequest::ShimmingTimerReset(
    bool f_Timeout /* = false */
)
{
    LPCTSTR FUNC_NAME = _T("CShimmingRequest::ShimmingTimerReset");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
              _T("CShimmingRequest::ShimmingTimerReset"));

    WNP_alarm(0);

    if (f_Timeout) {

        CPqm* l_pqm_ptr = CPqmUtilities::GetPqmPtr();

        if (l_pqm_ptr == NULL) {

            PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                      _T("CPqm pointer is NULL"));

            return;
        }

        CPqmPmAcqman* acq_mgr = l_pqm_ptr->GetAcqManager();

        if (acq_mgr == NULL) {
            PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                      _T("CPqmPmAcqman pointer is NULL"));
        }

        CScanProcedure* scan_proc = acq_mgr->GetScanProcedure();

        if (scan_proc == NULL) {
            PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                      _T("CScanProcedure pointer is NULL"));
        }

        CPQMLogMgr::GetInstance()->DisplayScanError(IDS_MSG_SHIMMING_TIMEOUT,
                _T("IDS_MSG_SHIMMING_TIMEOUT"),
                SCAN_ERR_NOTIFY, _T("ShimmingRequest.cpp"));

        //+Patni-HAR/2010Mar03/Added/MSA0251-00176-00
        const int l_protocol_no = scan_proc->GetScanProcProtocol()->GetProtocol();
        CPqmProtocol* l_protocol = l_pqm_ptr->GetStudy()->GetProtocol(l_protocol_no);

        if (l_protocol != NULL) {
            PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                      _T("SetShimmingReady(FALSE)"));
            l_protocol->SetShimmingReady(FALSE);
            scan_proc->GetScanProcProtocol()->SetShimmingReady(FALSE);
        }

        //-Patni-HAR/2010Mar03/Added/MSA0251-00176-00

        l_pqm_ptr->GetStudy()->SetScanModeOfHead();
        scan_proc->ResetCurrentRequest();

        //+Patni-HAR/2010Mar08/Added/MSA0251-00275
        if (l_pqm_ptr->GetPostProcessFlag()) {
            l_pqm_ptr->SetPostProcessFlag(FALSE);
        }

        //-Patni-HAR/2010Mar08/Added/MSA0251-00275

        if (scan_proc->GetScanProcProtocol()->GetShimming() == VFC_SHIM_STANDARD) {
            //Patni-HAR/2010Mar03/Added/MSA0251-00176-00
            l_pqm_ptr->WriteToUI(PQM_MSG_SCANDONE_SHIMMING); //Patni-HAR/2010Feb22/Added/MSA0251-00176

        } else {
            l_pqm_ptr->WriteToUI(PQM_MSG_POST_PROCESS_END);
        }

        l_pqm_ptr->WriteToUI(PQM_MSG_REMOVE_AUTO_SCAN);
        l_pqm_ptr->WriteToUI(PQM_MSG_ENDWAIT_CURSOR);


    }

    WNP_signal(SIGALRM, SIG_DFL);
}

//*******************************Method Header*********************************
//Method Name    :ShimmingTimerSetup
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
void CShimmingRequest::ShimmingTimerSetup(
)
{
    LPCTSTR FUNC_NAME = _T("CShimmingRequest::ShimmingTimerSetup");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of CShimmingRequest::ShimmingTimerSetup"));

    char*	timebuf = getenv("SHIMMING_TIMEOUT");

    if (timebuf != NULL) {
        m_shimming_timeout = atoi(timebuf);

    } else {
        m_shimming_timeout = SHIMMING_TIMEOUT_DEFAULT;

    }

    CString l_trace_string(_T(""));
    l_trace_string.Format(_T("Shimming Timeout = %d"), m_shimming_timeout);

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);

    WNP_signal(SIGALRM, &CShimmingRequest::ShimmingTimeOut);
    WNP_alarm(CShimmingRequest::m_shimming_timeout) ;
}
//-Patni-HAR/2010Feb18/Added/MSA0251-00176

//*******************************Method Header*********************************
//Method Name    :InitializeShimReqInfo
//Author         :PATNI/LK
//Purpose        :
//*****************************************************************************
void CShimmingRequest::InitializeShimReqInfo(
    /*    SShimGlobal_t* g_shim_global*/
)
{

    LPCTSTR FUNC_NAME = _T("CShimmingRequest::InitializeShimReqInfo");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CShimmingRequest::InitializeShimReqInfo"));


    g_shim_global->shimStudyP = new SShimStudy_t();
    g_shim_global->shimStudyP->study = NULL;
    g_shim_global->shimStudyP->nodes = s_shimimagepath;
    g_shim_global->shimStudyP->imageNodes   = s_shimechopath;
    g_shim_global->shimInfoP = new SShimInfo_t();
    g_shim_global->shimStudyP->shimInfo_mode = 0; // Auto Mode is set by default
    g_shim_global->posiDataP = new position_t();
    g_shim_global->shimInfoP->calc_processed = -1;
    g_shim_global->shimInfoP->adjust_processed = -1;
    g_shim_global->shimInfoP->totalSlice = -1;
    g_shim_global->shimInfoP->shim_flag = -1;
    g_shim_global->wait_time = 0.0;
    g_shim_global->shimStudyP->B0ShiftLimit = B0_SHIFT_LIMIT;
    g_shim_global->shimStudyP->WaitTimeMax  = WAIT_TIME_MAX;

    char*    l_buff = getenv("SHIMMING_B0_SHIFT_LIMIT");

    if (l_buff != NULL) {
        g_shim_global->shimStudyP->B0ShiftLimit = atoi(l_buff);
    }

    l_buff = getenv("SHIMMING_WAIT_TIME_MAX");

    if (l_buff != NULL) {
        g_shim_global->shimStudyP->WaitTimeMax  = atoi(l_buff);
    }
}


//*******************************Method Header*********************************
//Method Name    :PrepareShimReqInfo
//Author         :PATNI/LK
//Purpose        :
//*****************************************************************************
BOOL CShimmingRequest::PrepareShimReqInfo(
    const SShimReqInfo_t* f_shimreq_info/*,
    SShimGlobal_t* g_shim_global*/
)
{
    LPCTSTR FUNC_NAME = _T("CShimmingRequest::PrepareShimReqInfo");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CShimmingRequest::PrepareShimReqInfo"));

    BOOL            l_status = FALSE;

    s_shimimagepath[SVD_ST_PROTOCOL - 1].index    = 0;
    s_shimimagepath[SVD_ST_SUBPROTOCOL - 1].index = 0;
    s_shimimagepath[SVD_ST_SLICEGROUP - 1].index  = 0;
    s_shimimagepath[SVD_ST_SLICE - 1].index           = 0;
    s_shimimagepath[SVD_ST_IMAGE - 1].index          = 0;

    s_shimechopath[SVD_ST_PROTOCOL - 1].index     = 0;
    s_shimechopath[SVD_ST_PROCESSED - 1].index    = 0;
    s_shimechopath[SVD_ST_ECHO - 1].index         = 0;
    s_shimechopath[SVD_ST_POSITION - 1].index     = 0;

    if (g_shim_global) {
        InitializeShimReqInfo(/*g_shim_global*/);

        CString l_studyname =  f_shimreq_info->study; /*m_parent_study->GetStudyName()*/;
        const int l_studynamelen = l_studyname.GetLength();
        /* char *l_str = NULL;
         l_str = new char[l_studyname.GetLength()];
         g_shim_global->shimStudyP->study = new CHAR[150];
         wcstombs(l_str, l_studyname, l_studyname.GetLength());*/

        char* l_str = m_parent_study->ConvertCHAR(l_studyname);
        g_shim_global->shimStudyP->study = new CHAR[l_studynamelen];
        strcpy(g_shim_global->shimStudyP->study, l_str);
        g_shim_global->shimStudyP->protocol = f_shimreq_info->protocol;

        if (g_shim_global->shimStudyP->protocol < 0) {
            return l_status;
        }

        g_shim_global->shimStudyP->nodes[SVD_ST_PROTOCOL - 1].index       = g_shim_global->shimStudyP->protocol;
        g_shim_global->shimStudyP->imageNodes[SVD_ST_PROTOCOL - 1].index  = g_shim_global->shimStudyP->protocol;
        g_shim_global->shimInfoP->adjust_protocol                       = g_shim_global->shimStudyP->protocol;

        VfPathElem_t*   l_vfpath_elem = s_shimechopath;
        // Read the total slices
        g_shim_global->shimInfoP->totalSlice    = m_parent_study->ReadTotalSlicesForShimming(l_vfpath_elem);

        // Read the Shim Flag
        g_shim_global->shimInfoP->shim_flag     = m_parent_study->GetShimFlag(l_vfpath_elem);

        // check for Mode of the Shim Global struct.
        if (g_shim_global->shimStudyP->shimInfo_mode != SHIM_MANUAL_MODE &&
            g_shim_global->shimStudyP->shimInfo_mode != SHIM_AUTO_MODE) {

            CString l_tracemsg = _T("");
            l_tracemsg.Format(_T("Failed to set Mode --> %d"), g_shim_global->shimStudyP->shimInfo_mode);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_tracemsg);

            return l_status;
        }

        if (f_shimreq_info->path_len == SVD_ST_POSITION * sizeof(VfPathElem_t)) {

            const int l_cal_processed = f_shimreq_info->path[SVD_ST_PROCESSED - 1].index;

            g_shim_global->shimInfoP->calc_processed = l_cal_processed ;

            if (l_cal_processed == -1) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("calc_processed Error"));
                return l_status;
            }

            if (!(l_status = m_parent_study->ShimDataKindCheck(l_vfpath_elem, l_cal_processed))) {
                return l_status;
            }

            if (l_execflag) {
                l_makeimage ++;

            } else {
                l_execflag = 1;
                l_makeimage ++;
            }

            if ((f_shimreq_info->path[2].index == 2) &&
                (f_shimreq_info->path[3].index == (g_shim_global->shimInfoP->totalSlice - 1))) {
                Sleep(3);

            } else {
                return TRUE;
            }

        } else {
            g_shim_global->shimInfoP->calc_processed = 0;
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Recon End message"));

            l_status = TRUE;
        }

    } else {
        return l_status;
    }

    return l_status;
}


//****************************Method Header************************************
//Method Name   :msShimReq
//Author        :PATNI/LK
//Purpose       :
//*****************************************************************************
int CShimmingRequest::msShimReq(
    /*    SShimGlobal_t* g_shim_global*/
)const
{

    LPCTSTR FUNC_NAME = _T("CShimmingRequest::msShimReq");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of msShimReq"));

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_parent_study == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_parent_study is NULL"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    // Read the shim Position
    const int l_status = m_parent_study->ShimPositionRead(g_shim_global->shimStudyP, g_shim_global->posiDataP);

    if (l_status != E_NO_ERROR) {
        //+Patni-PJS/2010Jan29/Added/MVC006955
        CString l_cserror_string = _T("Please retry shimming plan");
        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(l_cserror_string,
                MPlus::EH::Information, _T("ShimmingRequest.cpp"), _T("PQM"));
        //-Patni-PJS/2010Jan29/Added/MVC006955
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("shimPositionRead Error"));
        return l_status ;
    }

    // Run the msShim application
    if (!msShimRun(/*g_shim_global*/)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("msShimRun Error"));
        return E_ERROR;
    }

    return E_NO_ERROR;
}


//****************************Method Header************************************
//Method Name   :msShimRun
//Author        :PATNI/LK
//Purpose       :
//*****************************************************************************
BOOL CShimmingRequest::msShimRun(
    /*    SShimGlobal_t* g_shim_global*/
)const
{

    LPCTSTR FUNC_NAME = _T("CShimmingRequest::msShimRun");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of msShimRun"));

    BOOL                            l_status   = TRUE;
    long                            l_msShim_socket_id   = -1;
    InteractiveRequestParams_t      l_interactive_reqparam;
    msCalcRequest_t                 l_replyfrom_msshim  = {0};
    msInteractiveShim_t             l_msinteractive_shimdata = {0};

    memset(&l_msinteractive_shimdata, NULL, sizeof(l_msinteractive_shimdata));

    if (sizeof(l_msinteractive_shimdata.study_name) < strlen(g_shim_global->shimStudyP->study)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("strlen Error"));
        return FALSE;
    }

    // Initialize the structure with current study name and current protocol number
    strncpy(l_msinteractive_shimdata.study_name, g_shim_global->shimStudyP->study, strlen(g_shim_global->shimStudyP->study));
    l_msinteractive_shimdata.protocolNode  = g_shim_global->shimStudyP->protocol;
    l_msinteractive_shimdata.processedNode = g_shim_global->shimInfoP->calc_processed;
    l_msinteractive_shimdata.startPosi     = g_shim_global->shimStudyP->startPosi;
    l_msinteractive_shimdata.endPosi       = g_shim_global->shimStudyP->endPosi;
    l_msinteractive_shimdata.centerPosi    = g_shim_global->posiDataP->offsetVec;

    l_msinteractive_shimdata.shimming_Channels = -1;

    // checks the AMB flag of the study file and if its value is VFC_AMB_MODE_SLAVE then returns TRUE
    if (m_parent_study->ShimCheckAMB(g_shim_global->shimStudyP->imageNodes)) {
        l_msinteractive_shimdata.shimming_Channels = 3;
    }

    CString l_tracemsg = _T("");

    //+Patni-PJS/2009Dec31/Added + Modified/MVC006466
    for (int l_count = 0; l_count < 10; l_count++) {
        // libMsIntf library function retrieves the socket id
        if ((l_status = msInteractiveProcOpen(MS_TYPE_SHIMMING, &l_msShim_socket_id))) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("msInteractiveProcOpen function fails "));
            return FALSE;
        }

        l_tracemsg.Format(_T("msShim socket FD --> %d"), l_msShim_socket_id);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_tracemsg);

        l_interactive_reqparam.msType = MS_TYPE_SHIMMING;
        l_interactive_reqparam.InteractiveParams.InteractiveShim.shimming = &l_msinteractive_shimdata;

        // MSINTF Library function calls
        if ((l_status = msInteractiveProcRequest(&l_interactive_reqparam, l_msShim_socket_id)) != E_NO_ERROR) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("msBatchRequest() - Error"));

            // MSINTF Library function calls
            msInteractiveProcClose(l_msShim_socket_id);
            return FALSE;
        }

        // MSINTF Library function call
        if ((l_status = msInteractiveProcSync(&l_replyfrom_msshim, l_msShim_socket_id)) != E_NO_ERROR) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("msInteractiveProcSync() - Error"));

            // MSINTF Library function call
            msInteractiveProcClose(l_msShim_socket_id);
            // + Not implemented for retry msShim communication

            //        if (l_count > 10) {
            //            //shimPrintf("%s : %d Retry Count Over. \n", __FILE__, __LINE__);
            //            return FALSE;
            //        } else {
            //            l_count++;
            //            Sleep(5);
            //            goto RETRY;
            //        }

        } else {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("msInteractiveProcSync() - Sucess"));
            break;
        }
    }

    if (l_count == 10) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Retry count exceded more than 10 time"));
        return FALSE;
    }

    //-Patni-PJS/2009Dec31/Added + Modified/MVC006466
    // MSINTF Library function call
    if ((l_status = msInteractiveProcClose(l_msShim_socket_id)) != E_NO_ERROR) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("msInteractiveProcClose() - Error"));
        return FALSE;
    }

    l_msinteractive_shimdata.patient[0] = l_replyfrom_msshim.CalcParams.ShimFA.patient[0];
    l_msinteractive_shimdata.patient[1] = l_replyfrom_msshim.CalcParams.ShimFA.patient[1];
    l_msinteractive_shimdata.patient[2] = l_replyfrom_msshim.CalcParams.ShimFA.patient[2];
    l_msinteractive_shimdata.patient[3] = l_replyfrom_msshim.CalcParams.ShimFA.patient[3];

    if (l_msinteractive_shimdata.patient[1] < 1) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("processed node < 1 error"));
        return FALSE;
    }

    l_msinteractive_shimdata.status = E_NO_ERROR;

    if (!msShimEndProc(l_msinteractive_shimdata/*, g_shim_global*/)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("msShimEndProc failed."));
        return FALSE;
    }

    return TRUE;
}

//****************************Method Header************************************
//Method Name   :msShimEndProc
//Author        :PATNI/LK
//Purpose       :
//*****************************************************************************
BOOL CShimmingRequest::msShimEndProc(
    const msInteractiveShim_t& f_shimdata/*,
    SShimGlobal_t* g_shim_global*/
)const
{

    LPCTSTR FUNC_NAME = _T("CShimmingRequest::msShimEndProc");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of msShimEndProc"));

    BOOL l_err_status = FALSE;

#ifdef PQM_MODE
    int    z2 = 0, z2_base = 0; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
    double d_shift = 0.0; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
#endif

    g_shim_global->shimInfoP->adjust_processed = f_shimdata.patient[1];

    CString l_tracemsg(_T(""));
    l_tracemsg.Format(_T("msShimEndProc() - adjust_index --> %d"), g_shim_global->shimInfoP->adjust_processed);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_tracemsg);

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_parent_study == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_parent_study is NULL"));
        return FALSE;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
#ifdef PQM_MODE

    if (m_parent_study->ShimGetZ2OffSet(&z2_base) != E_NO_ERROR) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("shimGetZ2() - Error (z2_base=0)"));
        z2_base = 0;
    }

#endif

    if (m_parent_study->ShimReqPutOffset(SVD_ST_PROCESSED, g_shim_global->shimStudyP->imageNodes, g_shim_global->shimInfoP->adjust_processed)) {
        l_err_status = TRUE;
    }

    if (m_parent_study->ShimReqPutFsyVal(g_shim_global->shimStudyP->imageNodes, SVD_ST_PROCESSED, g_shim_global->shimInfoP->adjust_processed)) {
        l_err_status = TRUE;
    }

#ifdef PQM_MODE

    // V4.10
    if (m_parent_study->ShimGetZ2OffSet(&z2) != E_NO_ERROR) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ShimGetZ2OffSet() - Error"));
        l_err_status = TRUE;
    }

#endif

    if (!l_err_status) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("msShimEndProc Error "));
        return FALSE;
    }

#ifdef PQM_MODE
    l_tracemsg.Format(_T("msShimEndProc() - z2 = %d z2_base --> %d"), z2, z2_base);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_tracemsg);

    d_shift = (double)(z2 - z2_base);
    g_shim_global->wait_time = d_shift;
#endif

    return TRUE;
}

//*******************************Method Header*********************************
//Method Name    :QuitShimmingRequest
//Author         :PATNI/LK
//Purpose        :Closes Shimming Request operations
//*****************************************************************************
void CShimmingRequest::QuitShimmingRequest(
    int f_flag,
    /*    SShimGlobal_t* g_shim_global,*/
    VfPathElem_t* f_path_elem
)const
{
    LPCTSTR FUNC_NAME = _T("CShimmingRequest::QuitShimmingRequest");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of QuitShimmingRequest"));

    if (g_shim_global) {
        if (f_flag != SHIM_NORMAL_END)  {
            g_shim_global->shimInfoP->adjust_protocol  = -1;
            g_shim_global->shimInfoP->adjust_processed = -1;
        }

        if (ComposeAdjustDoneMsg(/*g_shim_global,*/ f_path_elem) != TRUE) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ComposeAdjustDoneMsg() - Error"));
            f_flag = SHIM_TO_PQM_ERROR;
        }

        g_shim_global->end_flag = f_flag;
    }
}

//*******************************Method Header*********************************
//Method Name    :ComposeAdjustDoneMsg
//Author         :PATNI/LK
//Purpose        :
//*****************************************************************************
BOOL CShimmingRequest::ComposeAdjustDoneMsg(
    /*    SShimGlobal_t* g_shim_global,*/
    VfPathElem_t* f_path_elem
)const
{
    LPCTSTR FUNC_NAME = _T("CShimmingRequest::ComposeAdjustDoneMsg");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of ComposeAdjustDoneMsg"));

    if (g_shim_global) {
        if (g_shim_global->shimInfoP->adjust_protocol  == -1 ||
            g_shim_global->shimInfoP->adjust_processed  == -1) {

            CString l_tracemsg(_T(""));
            l_tracemsg.Format(_T("After Shimming Request No More Adjusted protocol and Adjusted Processed elements"));

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_tracemsg);

        } else {
            if (f_path_elem) {
                f_path_elem[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
                f_path_elem[SVD_ST_PROTOCOL - 1].index = g_shim_global->shimInfoP->adjust_protocol;
                f_path_elem[SVD_ST_PROCESSED - 1].subtree_name = SVN_ST_PROCESSED;
                f_path_elem[SVD_ST_PROCESSED - 1].index = g_shim_global->shimInfoP->adjust_processed;

            } else {
                return FALSE;
            }
        }

        return TRUE;

    } else {
        return FALSE;
    }
}

//*******************************Method Header*********************************
//Method Name    :ProcessResponse
//Author         :PATNI/LK
//Purpose        :updates new path element in Shim Shared Data
//*****************************************************************************
BOOL CShimmingRequest::ProcessResponse(
    const SR_STATUS       f_shimreq_status,
    VfPathElem_t*   f_path_elem,
    const int             f_shim_flag
)const
{
    LPCTSTR FUNC_NAME = _T("CShimmingRequest::ProcessResponse");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of ProcessResponse"));

    BOOL status = FALSE;

    CPqmShimSharedData*   l_pqmshimshareddata = CPqmShimSharedData::GetInstance();

    if (f_shimreq_status == SHIMMING_ADJUST_DONE) {
        //+Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow
        if (m_parent_study) {
            CPqm* l_pqmptr = m_parent_study->GetPqm();

            if (NULL != l_pqmptr) {
                CPQMDebugInfo::GetInstance(l_pqmptr)->PqmDebugProMonitor(_T("SHIM: PROMSG_SHIMM_ADJUST code (%d).\n")
                        , f_shimreq_status);
            }
        }

        //-Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow
        if (f_path_elem) {
            if (f_shim_flag == VFC_SHIM_STANDARD) {
                //  if(sizeof(f_path_elem) == sizeof(VfPathElem_t) * SVD_ST_PROCESSED) {
                if (f_path_elem[SVD_ST_PROCESSED - 1].subtree_name == SVN_ST_PROCESSED) {
                    l_pqmshimshareddata->SetShimState(SHIM_STATE_ACTIVE);
                    l_pqmshimshareddata->SetShimPath(f_path_elem);

                } else {
                    l_pqmshimshareddata->SetShimState(SHIM_STATE_READY);
                    l_pqmshimshareddata->SetShimPath(NULL);
                }

            } else if (f_shim_flag == VFC_SHIM_HIGH_RESOLUTION) {
                //if( sizeof(f_path_elem) == sizeof(VfPathElem_t) * SVD_ST_PROCESSED) {
                if (f_path_elem[SVD_ST_PROCESSED - 1].subtree_name == SVN_ST_PROCESSED) {
                    l_pqmshimshareddata->SetHRShimPath(f_path_elem);

                } else {
                    l_pqmshimshareddata->SetHRShimPath(NULL);
                }

                //+Patni-HEMANT/ADDED On 3/29/2009 9:39:19 PM/  PSP#212, IDS#198, IDS#199
                int list_index_to_pas = -1;

                if (l_pqmshimshareddata->GetCurrentProtocolNumber() != -1) {

                    //list_index_to_pas = m_parent_study->GetPositionFromProtocol (
                    //	l_pqmshimshareddata->GetCurrentProtocolNumber()
                    //	);
                    //+Patni-HAR-RG/2009June09/Modified/JFT#50
                    CPqmProtocol* l_protocol = m_parent_study->GetProtocol(l_pqmshimshareddata->GetCurrentProtocolNumber());

                    if (l_protocol != NULL) {
                        list_index_to_pas = l_protocol->GetProtocol();

                        if (list_index_to_pas == -1) {
                            return FALSE;
                        }
                    }

                    //-Patni-HAR-RG/2009June09/Modified/JFT#50
                }

                m_parent_study->GetPqm()->GetAcqManager()->GetScanProcedure()->CheckNextProtocol(list_index_to_pas);
                //
                //-Patni-HEMANT/ ADDED On3/29/2009 9:39:27 PM/  PSP#212, IDS#198, IDS#199
            }

            m_parent_study->WriteToShimData();
            status = TRUE;
        }

    } else if (f_shimreq_status == SHIMMING_CANCEL) {

        int l_shimstate = 0; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections

        //+Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow
        if (m_parent_study) {
            CPqm* l_pqmptr = m_parent_study->GetPqm();

            if (NULL != l_pqmptr) {
                CPQMDebugInfo::GetInstance(l_pqmptr)->PqmDebugProMonitor(_T("SHIM: PROMSG_SHIMM_CANCEL"));
            }
        }

        //-Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow
        if (f_shim_flag == VFC_SHIM_STANDARD &&
            CPqmShimSharedData::GetInstance()->GetShimState(&l_shimstate) == TRUE &&
            l_shimstate == SHIM_STATE_WAIT
           ) {
            m_parent_study->SetShimmingOffset(PQM_SHIM_RESET, -1);
        }

    } else {

        //+Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow
        if (m_parent_study) {
            CPqm* l_pqmptr = m_parent_study->GetPqm();

            if (NULL != l_pqmptr) {
                CPQMDebugInfo::GetInstance(l_pqmptr)->PqmDebugProMonitor(_T("SHIM: Unknown Message (%d)"), f_shimreq_status);
            }
        }

        //-Patni-DKH/2010Feb03/Added/PH#3#/IR86/DebugWindow
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Unknown message from Shimming Request"));

        //CPQMLogMgr::GetInstance()->DisplayScanError(IDS_ERR_PM_SHIMMING,
        //  SCAN_ERR_CRUCIAL,
        //_T("ShimmingRequest.cpp"));

        // CPQMLogMgr::GetInstance()->WriteEventLog(
        //     IDS_ERR_PM_SHIMMING,
        //     ERROR_LOG_ERROR1 ,_T("ShimmingRequest.cpp"));
    }

    return status;
}


//***************************Method Header*************************************
//Method Name    :CheckShimDataFiles()
//Author         :PATNI/
//Purpose        :
//*****************************************************************************
//+Patni-PJS/2010Jan14/Added/MVC006466
BOOL CShimmingRequest::CheckShimDataFiles(
)const
{
    LPCTSTR FUNC_NAME = _T("CShimmingRequest::CheckShimDataFiles");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CheckShimDataFiles"));

    char	buf[128] = "", magfile[64] = "";	//Patni-MJC/2009Aug17/Modified/cpp test corrections

    char* path = getenv("GP_SHIMDT_DIR");


    if (path == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Failed to get value of GP_SHIMDT_DIR environment variable"));
        return FALSE;
    }


    sprintf(buf, "%s/setupShimming", path);

    FILE*	fp = NULL;												//Patni-MJC/2009Aug17/Modified/cpp test corrections

    if ((fp = fopen(buf, "r")) == NULL) {

        CString str(buf);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  str);
        return FALSE;
    }

    fscanf(fp, "%s", magfile);
    fclose(fp);

    sprintf(buf, "%s/magDB/%s", path, magfile);

    if ((fp = fopen(buf, "r")) == NULL) {

        CString filepath(buf);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  filepath);
        return FALSE;
    }

    fclose(fp);

    return TRUE;
}
//+Patni-PJS/2010Jan14/Added/MVC006466

//+Patni-HAR/2010Feb18/Added/MSA0251-00176
//*******************************Method Header*********************************
//Method Name    :ShimmingTimeOut
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
void CShimmingRequest::ShimmingTimeOut(
    int sig
)
{
    LPCTSTR FUNC_NAME = _T("CShimmingRequest::ShimmingTimeOut");

    //Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(sig);

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
              _T("CShimmingRequest::ShimmingTimeOut"));

    CShimmingRequest::ShimmingTimerReset(true) ;		/* TIMER RESET */

    CPqmUtilities::GetPqmPtr()->GetAMB()->PqmPmAbortAmb();
}


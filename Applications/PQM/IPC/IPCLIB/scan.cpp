//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-774
//Redmine-778
//Redmine-779
//Redmine-780
//Redmine-781
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: scan.cpp
 *  Overview: Implementation of CScan class.
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
//Patni-PJS/2011Mar2/Modified/TMSC_REDMINE-1480-Review Comments
#include <process.h>
#include "StdAfx.h"
#include "Scan.h"
#include <ErrorDisp/ErrorDispConsts.h>
#include <libStudyManager/strcnv.h>
#include <libStudyManager/Log/ScopedFuncInOutLog.h>
#include <PQM/PQMIPCManager.h>
#include <PQM/PQMLogMgr.h>
#include <PQM/PqmUtilities.h>
#include "PqmPmAcqman.h"
#include "Pqm.h"
#include "ProtocolUtil.h"
#include "ShimCtrl.h"
#include "PqmSpeeder.h"
#include "PQMStudyMgr.h"
#include "scanprocedure.h"
#include "PqmMRSScan.h"
#include "PqmProbe.h"
//Patni-PJS/2009Nov10/Added/MVC04957
#include "VarFieldHandle.h" //TMSC-Tanoue/2010Aug25/MVC008345
#include "VoiceManager.h"
#include "pqmAMB.h"
#include "PqmPmSARManager.h"

using namespace MR::ACQ::SM::COMMON::MUTEX;


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
extern VfPathElem_t s_shimechopath[];
//Patni-ARD/20090305/Added/ACE-1# handling of stabilization start
extern "C" {
    extern int AcqFileCopy(char* infile, char* outfile);
}

using namespace MPlus::EH;

//Patni-ARD/20090305/Added/ACE-1# handling of stabilization End
//***************************Method Header*************************************
//Method Name    : CScan()
//Author         : PATNI/AG
//Purpose        : Construction
//*****************************************************************************
CScan::CScan(
    CScanProcedure* scan_proc
) : CScanTransaction(scan_proc),
    //Patni-PJS/2011Mar2/Modified/TMSC_REDMINE-1480-Review Comments
    m_mrs_thread_id(0),
    m_mrs_thread_handle(NULL)
    //-Patni-PJS/2011Mar2/Modified/TMSC_REDMINE-1480-Review Comments

{
    LPCTSTR FUNC_NAME = _T("CScan::CScan");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ScanTransaction Created"));
}

//***************************Method Header*************************************
//Method Name    : ~CScan()
//Author         : PATNI/AG
//Purpose        : Destruction
//*****************************************************************************
CScan::~CScan(
)
{
    LPCTSTR FUNC_NAME = _T("CScan::~CScan");

    //+Patni-PJS/2011Feb24/Added/TMSC_REDMINE-1480
    if (m_mrs_thread_handle) {
        CloseHandle(m_mrs_thread_handle);
        m_mrs_thread_handle = NULL;
    }

    //-Patni-PJS/2011Feb24/Added/TMSC_REDMINE-1480

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ScanTransaction Destroyed"));
}


status_t CScan::AcqProbingThresHold(const float f_max, const float f_min)
{
    LPCTSTR FUNC_NAME = _T("CScan::AcqProbingThresHold");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of AcqProbingThresHold"));
    //    bzero(&acqRequest,sizeof(PqmAcqRequest_t));

    //    PqmDebugAcqMonitor("AMCMM_MO_MONITOR_ABORT : abort ");

    /* Set function */
    m_header.function = AMCMM_PROBE_CHANGE_THRESH;
    m_data.param.ThreshHold.max_threshold = f_max;
    m_data.param.ThreshHold.min_threshold = f_min;

    return SendMessage() ;
}


// + SM4 Himanshu VISUAL_PREP 13 JAN 2009
//*******************************Method Header*********************************
//Method Name    : MaintainPrepStudy()
//Author         : PATNI/HAR
//Purpose        :
//*****************************************************************************
int CScan::MaintainPrepStudy(
    const CString& f_file_name,
    CPqmProtocol* f_current_protocol
)const
{

    //+Patni-DKH/2010Oct15/Modified/MVC009192
    LPCTSTR FUNC_NAME = _T("CScan::MaintainPrepStudy");

    char		prepfile[256] = {'\0'};
    CPqmUtilities::GetPrepStudyName(prepfile);

    //Patni-ARD/20090306/Added/ACE-1# handling of stabilization start
    int l_status = -1;

    //MEITEC/2010May17/Modified/IR-100

    if (strlen(prepfile) != 0) {
        std::string l_str_filename("");
        UTIL::Wcs2Mbs(&l_str_filename, f_file_name);

        l_status = AcqFileCopy(prepfile, const_cast<char*>(l_str_filename.c_str()));
    }

    //Patni-ARD/20090306/Added/ACE-1# handling of stabilization End
    if (l_status != 0) {	//Patni-ARD/20090402/Modified/ACE-1# handling of stabilization
        CString l_wcs_prepfilename(_T(""));
        UTIL::Mbs2Wcs(&l_wcs_prepfilename, prepfile);

        CString l_errmsg = _T("");
        l_errmsg.Format(_T("AcqFileCopy(%s, %s) function FAILED.Error Value : %d."),
                        l_wcs_prepfilename, f_file_name, l_status);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_errmsg);
        return PQM_WARN_SCAN_PREP_STUDY;

    } else {
        CScanProcedure* scanproc = GetScanProcedure();

        if (scanproc == NULL) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Scan Procedure pointer is NULL"));
            return E_ERROR;
        }

        CPqmPmAcqman* acqman = scanproc->GetAcqManager();
        CPqm* pqm = acqman->GetPqm();
        l_status = pqm->GetStudy()->MaintainStudy(f_file_name,
                   f_current_protocol, MAINTAIN_PREP);

        if (l_status != E_NO_ERROR) {
            return  PQM_WARN_SCAN_PREP_STUDY;
        }

        return E_NO_ERROR;
    }

    //-Patni-DKH/2010Oct15/Modified/MVC009192
}
// - SM4 Himanshu VISUAL_PREP 13 JAN 2009

//*******************************Method Header*********************************
//Method Name    : MaintainMotionCorrectStudy()
//Author         : PATNI/MSN
//Purpose        : Maintain the probe study required for RMC scanning
//*****************************************************************************
int CScan::MaintainMotionCorrectStudy(
    const CString& f_file_name,
    CPqmProtocol* f_current_protocol
)const
{
    LPCTSTR FUNC_NAME = _T("CScan::MaintainMotionCorrectStudy");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace of MaintainMotionCorrectStudy"));

    CScanProcedure* scanproc = GetScanProcedure();

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (scanproc == NULL) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Scan Procedure pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    CPqm* pqm = scanproc->GetAcqManager()->GetPqm();

    BITFLD_DECL(mask, MAX_NODES) = {0}; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
    memset(mask, 0, sizeof(mask));

    //+Patni-DKH/2010Dec15/Modified/TMSC-REDMINE-655
    char		l_probestudyfile[256] = {'\0'};

    CPqmUtilities::GetProbeStudyName(l_probestudyfile);
    int l_status = E_NO_ERROR;

    if (strlen(l_probestudyfile) != 0) {
        std::string l_str_filename("");
        UTIL::Wcs2Mbs(&l_str_filename, f_file_name);

        l_status = AcqFileCopy(l_probestudyfile, const_cast<char*>(l_str_filename.c_str()));
    }

    if (l_status != E_NO_ERROR) {
        CString l_wcs_probefilename(_T(""));
        UTIL::Mbs2Wcs(&l_wcs_probefilename, l_probestudyfile);

        CString l_errmsg = _T("");
        l_errmsg.Format(_T("AcqFileCopy(%s, %s) function FAILED.Error Value : %d."),
                        l_wcs_probefilename, f_file_name, l_status);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_errmsg);
        return E_ERROR;

    } else {

        //+Patni-MSN/2009Mar12/Modified/Optimization
        int l_status = pqm->GetStudy()->MaintainStudy(f_file_name,
                       f_current_protocol,
                       MAINTAIN_MOTION_CORRECT);
        //-Patni-MSN/2009Mar12/Modified/Optimization

        if (l_status == E_NO_ERROR) {

            /* Get Thresh Hold from PROBING_FILE_NAME */
            pqm->GetStudy()->SetProbeThresHold(f_current_protocol->GetProtocol()) ;

            CVarFieldHandle probe_handle(f_file_name);
            l_status = probe_handle.Open();

            if (l_status != E_NO_ERROR) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_vf_db_name handle null"));
                return E_ERROR;
            }

            int32_t l_prob_flag = VFC_PROBE_RTMC;
            pqm->GetStudy()->SetProtValues(&probe_handle, 0, mask,
                                           K_PR_PROBE_FLAG, &l_prob_flag, sizeof(l_prob_flag),
                                           NULL);
            //Patni-AMT/2009Jul06/Modified/ ManualPrescan TMSC review comment
            pqm->GetStudy()->PutRflParamsSAR(&probe_handle, 0);//MRP
        }
    }

    //Patni-DKH/2010Dec15/Modified/TMSC-REDMINE-655
    return l_status;
}


//*******************************Method Header*********************************
//Method Name    : AcquisitionDone
//Author         : PATNI/MRP
//Purpose        : This function will set the protocol status to done and will update
//                 the study file with the scan end date and time.
//*****************************************************************************
int CScan::AcquisitionDone(
)
{
    LPCTSTR FUNC_NAME = _T("CScan::AcquisitionDone");
    PQM_SCOPED_TRACE();


    CScanProcedure* scanproc = GetScanProcedure();

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (scanproc == NULL) {
        PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME,
                  _T("Scan procedure pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    CPqmPmAcqman* acqman_manager = scanproc->GetAcqManager();
    CPqm* pqmptr = acqman_manager->GetPqm();
    CPqmProtocol* curr_protocol = scanproc->GetScanProcProtocol();
    CPQMStudy* l_pqm_study = pqmptr->GetStudy();

    if (NULL == l_pqm_study) {
        PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME, _T("Study pointer is NULL"));
        return E_ERROR;
    }

    CScopedLock l_scoped_lock(l_pqm_study->GetScanMutex());
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_scan_mutex acquired"));

    CVoiceManager::GetInstance()->QuitVoiceManager();

    //+Patni-Ravindra Acharya/Added/22March2010/IR-133

    //+MEITEC/2010May25/Added Comment/Patni MVC7300_CDR
    CString l_risDictionary = pqmptr->GetRisDictionary() ;
    CString l_risName = pqmptr->GetRisName() ;
    //-MEITEC/2010May25/Added Comment/Patni MVC7300_CDR
    int iProtocol  = curr_protocol->GetProtocol() ;

    //+MEITEC/2010May25/Added Comment/Patni MVC7300_CDR
    //if ( (l_ptr_risDictionary != NULL) && (l_ptr_risName != NULL))
    if ((l_risDictionary.CompareNoCase(_T("")) != 0) && (l_risName.CompareNoCase(_T("")) != 0)) {
        //-MEITEC/2010May25/Added Comment/Patni MVC7300_CDR
        PROCESS_INFORMATION m_proList_proc_info ;
        STARTUPINFO si ;
        ZeroMemory(&si, sizeof(si)) ;
        si.cb = sizeof(si) ;
        ZeroMemory(&m_proList_proc_info, sizeof(m_proList_proc_info)) ;

        CString l_szEnv2 = pqmptr->GetStudy()->GetStudyName();

        //+MEITEC/2010May25/Added Comment/Patni MVC7300_CDR
        CString l_protocol_num(_T(""));
        l_protocol_num.Format(_T(" -protocol %d"), iProtocol);
        CString l_szEnv = _T("proList -study ") + l_szEnv2 + l_protocol_num + _T(" -dic ") + l_risDictionary + _T(" -name ") + l_risName + _T(" -csv -save -study_id on ") ;
        //-MEITEC/2010May25/Added Comment/Patni MVC7300_CDR

        char* l_inst_type_env = getenv("MPLUSINSTTYPE") ;
        char* l_mrmp_home_env = getenv("MRMP_HOME") ;

        CString l_tmp_path(l_inst_type_env) ;
        CString l_proList_path(l_mrmp_home_env) ;

        l_proList_path += _T("/bin") + l_tmp_path + _T("proList.exe") ;

        if (!CreateProcess(l_proList_path,
                           l_szEnv.GetBuffer(l_szEnv.GetLength()),
                           NULL,
                           NULL,
                           FALSE,
                           CREATE_NO_WINDOW,
                           NULL,
                           NULL,
                           &si,
                           &m_proList_proc_info)) {


            //+Patni-NP/2010May26/Modified/Redmine-347

            CPQMLogMgr::GetInstance()->WriteEventLog(
                CPqmUtilities::GetMultiLingualString(_T("IDS_PROLIST_NOT_INVOKED")),
                ERROR_LOG_INFORMATION2, _T("scan.cpp"), __LINE__
            ) ;
            //-Patni-NP/2010May26/Modified/Redmine-347
            //return PROLIST_NOT_INITIALIZED ;
        }

    }

    //-Patni-Ravindra Acharya/Added On 22March2010//IR-133


    //Patni-HEMANT/ADDED On 6/30/2009 6:56:43 PM/ ACE-2 / WFDA
    //QTS#37 :sending scan done message to InScan server just before changing the GUI status.
    //Patni-PJS/2011Jan25/Modified/IR-97
    //pqmptr->ProcessScanRelatedMsgForWFDA(curr_protocol, PQM_MSG_STATUS_DONE);

    BOOL has_image = l_pqm_study->IsHasImage(curr_protocol);

    //+Patni-HAR/2010Apr26/Added/MVC006285
    if (!has_image) {
        //    time_t  t = 0L, s_time = 0L; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
        //    t = time(&s_time);
        //    l_pqm_study->SetScanDateTime(t);
        //+Patni-PJS/2010May09/Added/JaFT# IR-141
        CScanProcedure* scan_proc = GetScanProcedure();
        CPqmPmAcqman* acqman = scan_proc->GetAcqManager();
        CPqm* pqm = acqman->GetPqm();

        if (!pqm->RaiseOrderRecordAuditEvent()) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("RaiseOrderRecordAuditEvent failed"));
        }

        //-Patni-PJS/2010May09/Added/JaFT# IR-141
    }

    //-Patni-HAR/2010Apr26/Added/MVC006285
    if (!scanproc->GetCFARetry()) {
        l_pqm_study->SetAcqStatus(curr_protocol, C_PR_STATUS_DONE);
        //+Patni-NP/2009Mar09/Added/PSP1 Changes For VF Lock
        BOOL l_postmsg_status = FALSE;
        CString l_series_loid = curr_protocol->GetSeriesLOID();

        //TMSC/2009Nov12/Modified/Code Changes
        //+Patni-PJS/2009Dec15/Added/MVC006256-tbt
        if (!curr_protocol->GetSeriesNumInDbFlag()) {
            l_postmsg_status = pqmptr->GetDbsaStudyMgr()->SetSeriesNoInDB((LPCTSTR)curr_protocol->GetSeriesLOID(), curr_protocol->GetAcqStatus());

            if (l_postmsg_status == E_ERROR) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("Failed to set series in DB"));
                //+Patni-RSG/2009Dec15/Added/MVC006555
                //return E_ERROR;
            }

            curr_protocol->SetSeriesNumInDbFlag(TRUE);
        }

        //-Patni-PJS/2009Dec15/Added/MVC006256
        //l_postmsg_status = pqmptr->m_dbsa_study_mgr->SetSeriesNoInDB((LPCTSTR)curr_protocol->GetSeriesLOID(), curr_protocol->GetAcqStatus());
        //TMSC/2009Nov12/Modified/Code Changes
        l_postmsg_status = pqmptr->GetDbsaStudyMgr()->SetAcquisitionStatusInLocalDB((BSTR)l_series_loid.AllocSysString(),
                           curr_protocol->GetAcqStatus());

        //+Patni-HAR/2010Mar26/Added/MVC007422

        CPqmUtilities::UpdateIECInfo(curr_protocol);

        pqmptr->GetStudy()->SetNewSAR(curr_protocol->GetPosition() + 1);

        pqmptr->GetSARManager()->SendRTSARInfoForLastDoneProtocol();
        pqmptr->GetSARManager()->CalcSARControlAndPutVFForAll();


        //+Patni-Manish/Uncommented-Added/2009-Sept-07/MVC4625
        if (curr_protocol->GetAppCode() == VFC_APP_CODE_MRS ||
            curr_protocol->GetAppCode() == VFC_APP_CODE_MRSCSI) {

            const int l_acq_order = curr_protocol->GetProtocol();

            if (l_pqm_study->IsProtocolInScan(l_acq_order)) {
                PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME,
                          _T("MRS/MRSCSI Protocol is InScan Kind"));

            } else {


                CPqmMRSScan* l_mrs_scan = (CPqmMRSScan*)scanproc->GetCurrentTransaction();

                if (l_mrs_scan != NULL) {
                    //Patni-PJS/2011Feb24/Modified/TMSC_REDMINE-1480
                    if (FALSE == CPqmUtilities::InvokeMRSProcessTool(curr_protocol)) {
                        PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME,
                                  _T("Failed to Invoke MRS Process Tool."));

                    } else {
                        //+Patni-PJS/2011Feb24/Added/TMSC_REDMINE-1480
                        //+Patni-PJS/2011Mar2/Modified/TMSC_REDMINE-1480-Review Comments
                        /*m_mrs_processtool_thread = CreateThread(
                                 NULL,                           // default security attributes
                                 NULL,                           // use default stack size
                                 CPqmUtilities::IsMRSProcessToolAlive,  // thread function
                                 (void*)this,                    // argument to thread function
                                 NULL,                           // use default creation flags
                                 NULL);*/
                        if (m_mrs_thread_handle) {
                            CloseHandle(m_mrs_thread_handle);
                            m_mrs_thread_handle = NULL;
                        }

                        m_mrs_thread_handle = (HANDLE)_beginthreadex(0,
                                              0,
                                              CPqmUtilities::IsMRSProcessToolAlive,
                                              this,
                                              0,
                                              &m_mrs_thread_id);

                        if (!m_mrs_thread_handle) {
                            PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME,
                                      _T(" failed to create thread handle"));
                        }

                        //Patni-PJS/2011Mar2/Modified/TMSC_REDMINE-1480-Review Comments
                        //-Patni-PJS/2011Feb24/Added/TMSC_REDMINE-1480
                    }
                }
            }
        } //-Patni-Manish/Uncommented-Added/2009-Sept-07/MVC4625


        if (curr_protocol->GetShimming() != VFC_SHIM_NONE || curr_protocol->GetSpeederType() == VFC_SPEEDER_TYPE_SENSITIVITY) {
            if ((curr_protocol->GetSpeederType() == VFC_SPEEDER_TYPE_SENSITIVITY && !curr_protocol->GetIsMapPostProcDone()) ||
                (curr_protocol->GetShimming() != VFC_SHIM_NONE && (!curr_protocol->GetShimmingReady() || VFC_SHIM_STANDARD == curr_protocol->GetShimming()))) {

                pqmptr->SetPostProcessFlag(TRUE);
                //+Patni-SS/UnCOMMENTED/2009-May-15/JFT#139
                pqmptr->WriteToUI(PQM_MSG_BEGINWAIT_CURSOR);
            }

            //+Patni-HAR-RG/2009June09/Modified/JFT#50
            PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME,
                      _T(" 1. if==> PQM_MSG_STATUS_DONE"));

            //Patni-HAR/2010Mar04/Commented/MSA00251-00176-00
            pqmptr->WriteToUI(PQM_MSG_STATUS_DONE);

        } else {
            PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME, _T(" else PQM_MSG_STATUS_DONE"));
            //-Patni-HAR-RG/2009June09/Modified/JFT#50

            //check wether protocol is autolocator enabled and can invoke AL application
            if (pqmptr->CheckForAutoLocator(curr_protocol)) {
                pqmptr->WriteToUI(PQM_MSG_STATUS_DONE);
            }
        }

        //-Patni-HAR-SS/2009Apr20/Modified/Defect#372
        //-Patni-Manish/Added On 20March2009//PSP1#253

    }

    pqmptr->GetAMB()->PqmAmbEachAcquisitionDone(curr_protocol, false);

    CPqmProbe* pqm_probe = scanproc->GetPqmProbe();         //Patni-MJC/2009Aug17/Modified/cpp test corrections

    if (pqm_probe) {                                        //Patni-MJC/2009Aug17/Modified/cpp test corrections
        pqm_probe->RefProbeScanEnd() ;   //RMC
    }

    CPqmProtocol* next_protocol = l_pqm_study->GetNextItem(curr_protocol);

    if (curr_protocol->GetShimming()) {

        if (curr_protocol->GetShimmingReady()) {
            // + Restructured code
            // create Shimming Request object
            CShimmingRequest l_shimming_request(l_pqm_study);
            // input parameter
            SShimReqInfo_t l_shimreq_info ;

            CString l_studyname = l_pqm_study->GetStudyName();
            int l_del_place = l_studyname.ReverseFind(_T('/'));
            int l_diff = l_studyname.GetLength() - l_del_place;
            CString l_del_str = l_studyname.Left(l_studyname.GetLength() - (l_diff - 1));
            l_studyname.Delete(0, l_del_str.GetLength());

            CString l_new_str = _T("/study/");
            l_new_str += l_studyname;
            //Patni-HAR/2009Aug31/Modified/Corrected call for wcstombs()
            int l_len = l_new_str.GetLength() + 1;
            char* l_str = new char[l_len];
            memset(l_str, 0, sizeof(char) * (l_len));
            wcstombs(l_str, l_new_str, l_len);

            strcpy(l_shimreq_info.study, l_str);

            // - Sarjeet
            l_shimreq_info.protocol =  curr_protocol->GetProtocol();
            l_shimreq_info.path_len =  0;
            // Patni - SS, RSG / ADDED/2009June12/ Added / JFT#50
            m_scan_procedure->ResetCurrentRequest();
            // Process the Shimming Request
            SR_STATUS l_shim_req_status = l_shimming_request.ProcessShimmingRequest(&l_shimreq_info);

            // get the Shim flag of the current protocol
            int l_shim_flag = l_pqm_study->GetShimFlag(s_shimechopath);

            DEL_PTR_ARY(l_str);    //+Patni-HAR/2009Aug11/Added/Memory Leaks

            // for a Shimming sequence, send the shim offset value to AcqMan and send ShimCtrl msg
            if (l_shim_flag == VFC_SHIM_STANDARD) {
                if (l_shim_req_status == SHIMMING_ADJUST_DONE) {
                    PqmShim_t l_shim;
                    PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME,
                              _T("Before Sending SendShimOffset from AcquisitionDone"));
                    l_pqm_study->GetShimmingOffset(&l_shim);
                    scanproc->GetShimCtrl()->SendShimOffset(&l_shim);

                    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                              _T("2. if==> PQM_MSG_SCANDONE_SHIMMING"));

                    //+Patni-HAR-SS/MODDIFIED/2009-June-05/JFT#266
                    pqmptr->WriteToUI(PQM_MSG_SCANDONE_SHIMMING);
                    //pqmptr->WriteToUI(PQM_MSG_ENDWAIT_CURSOR);
                    //-Patni-MP/2010-Sep-13/REDMINE-500

                    pqmptr->SetPostProcessFlag(TRUE);

                } else {
                    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                              _T("2. else ==> PQM_MSG_SCANDONE_SHIMMING"));

                    //Patni-HAR/2010Mar04/Modified/MSA00251-00176-00
                    pqmptr->WriteToUI(PQM_MSG_SCANDONE_SHIMMING);
                    //-Patni-MP/2010-Sep-13/REDMINE-500
                    pqmptr->SetPostProcessFlag(FALSE);

                    if (!pqmptr->CanDisplayCFAGraph(curr_protocol)) {
                        pqmptr->WriteToUI(PQM_MSG_AUTO_SHIM_CFA_PRESCAN_END);

                    } else {
                        pqmptr->WriteToUI(PQM_MSG_CFA_PRESCAN_END);
                    }

                    pqmptr->WriteToUI(PQM_MSG_REMOVE_AUTO_SCAN);
                }
            }

            //-Patni-HAR-SS/MODDIFIED/2009-June-05/JFT#266
            PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("SetShimmingReady(FALSE)"));

            curr_protocol->SetShimmingReady(FALSE);

        } else {
            //+Patni-Manishkumar/2009Sept15/Added/MVC004760/Set wait cursor
            if (curr_protocol->GetShimming() == VFC_SHIM_HIGH_RESOLUTION) {
                PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME, _T("Begin Wait Cursor"));
                //Patni-HAR/2010Mar08/Added/MSA0251-00275

                //Patni-PJS/2010Jan19/Added/MVC006827
                pqmptr->WriteToUI(PQM_MSG_POST_PROCESS_START);
            }

            //-Patni-Manishkumar/2009Sept15/Added/MVC004760

            curr_protocol->SetShimmingReady(TRUE);
            // + Patni - SS, RSG / ADDED/ 2009June12/ Added / JFT#50
            //Patni-HAR/2010Feb18/Added/MSA0251-00176
            //m_scan_procedure->ResetCurrentRequest();

            CShimmingRequest::ShimmingTimerSetup();
        }

    } else if (curr_protocol->GetSpeederType() == VFC_SPEEDER_TYPE_SENSITIVITY) {
        //-Patni-MRP/2010Aug6/Added/REDMINE-498
        if (!curr_protocol->GetIsMapPostProcDone()) {
            CPqmSpeeder* l_speeder = pqmptr->GetSpeederPtr();
            //Patni-HAR/2010Mar08/Added/MSA0251-00275

            //Patni-PJS/2010Jan19/Added/MVC006827
            pqmptr->WriteToUI(PQM_MSG_POST_PROCESS_START);
            l_speeder->SpeederTimerSetup();
            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                      _T("Trace of l_protocol->GetSpeederType() == VFC_SPEEDER_TYPE_SENSITIVITY"));
            //scanproc->SetIsScanning(false);
            scanproc->ResetCurrentRequest();
        }

        //+Patni-MP/DKH/2011Feb23/Added/TMSC-REDMINE-1470
        else {
            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                      _T("Speeder done before scandone"));

            if (next_protocol != NULL &&
                !(next_protocol->GetHoldScan() & VFC_HOLD_PRE_SCAN)) {
                l_pqm_study->CheckNextAutoProt(next_protocol);

            } else {
                scanproc->ResetCurrentRequest();
            }
        }

        //-Patni-MP/DKH/2011Feb23/Added/TMSC-REDMINE-1470
        //+Patni-ARD/2010Mar3/Commented/DeFT# IR-100 remove IPCConfig.ini
        /*
        if (CPQMConfig::GetInstance()->GetMachineType() != _T("ONEPATH")) {
            if (l_pqm_study->SpeederMAPExist(curr_protocol->GetProtocol()) == E_NO_ERROR) {
                l_speeder->SpeederDone(curr_protocol, E_NO_ERROR) ;

            } else {
                l_speeder->SpeederDone(curr_protocol, E_ERROR) ;
            }
        }
        */
        //-Patni-ARD/2010Mar3/Commented/DeFT# IR-100 remove IPCConfig.ini

    } else if (next_protocol != NULL &&
               next_protocol->GetAutoProEdit()) {  //Patni-PJS/2010Feb24/Added/DeFT# PROPOSAL_08-Remove EZScan

        //Patni-HEMANT/ADDED On 3/29/2009 9:40:32 PM/ IDS#198, IDS#199
        scanproc->ResetCurrentRequest();
        //PqmPmActionTools(K_PM_PROLIM_EDIT,NULL);            PQM_PROEDIT_NAVI;

        //Patni-PJS/2010Feb24/Added/DeFT# PROPOSAL_08-Remove EZScan

    } else if (next_protocol != NULL &&
               !(next_protocol->GetHoldScan() & VFC_HOLD_PRE_SCAN)) {

        //+Patni-MP/DKH/2011Feb23/Modified/TMSC-REDMINE-1470
        if (E_ERROR == l_pqm_study->CheckNextAutoProt(next_protocol)) {
            return E_ERROR;
        }

        //-Patni-MP/DKH/2011Feb23/Modified/TMSC-REDMINE-1470

    } else {
        scanproc->ResetCurrentRequest();
    }

    if (scanproc->GetAPCHold()) {
        scanproc->SetAPCHold(0);
    }

    //+ Patni-PJS/2009Jun05/Added/JFT# 251,251,251,IR(5)

    return E_NO_ERROR;

}


//*******************************Method Header*********************************
//Method Name    : ProcessResponse()
//Author         : PATNI/MRP
//Purpose        :This method will be called after a response that Scan
//                 is done successfully is received .This method does the
//                 setting of the last scan done values with the values of
//                 the current protocol.
//*****************************************************************************
void CScan::ProcessResponse(
    AM_ClientMessageHeader_t* hdr,
    AM_AcqManToClient_t* body
)
{
    LPCTSTR FUNC_NAME = _T("CScan::ProcessResponse");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace of ProcessResponse"));

    CScanProcedure* scan_proc = GetScanProcedure();
    CPqmPmAcqman* acqman = scan_proc->GetAcqManager();
    CPqm* pqm = acqman->GetPqm();
    int type = 0;

    int funcBit = acqman->GetFunctionBit(hdr->function);
    CString str_msg(_T(""));

    if ((body->status == AMCM_SUCCESS) ||
        (body->status == AMCM_RECONST)) {
        //Saurabh@RMC
        if ((hdr->function == AMCMR_PROBE_CHANGE_THRESH) || (hdr->function == AMCMA_PROBE_CHANGE_THRESH_DONE)) {
            str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                           acqman->GetStr(body->status),
                           acqman->GetStr(type),
                           acqman->GetStr(hdr->function));

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, str_msg);

        }

        //Saurabh@RMC

        else if ((hdr->function == AMCMR_ACQUISITION) ||
                 (hdr->function == AMCMR_ACQUISITION_CONTINUE) ||
                 (hdr->function == AMCMR_PREP_SCAN) ||
                 (hdr->function == AMCMR_MO_CORRECT) ||
                 (hdr->function == AMCMR_AMB_SCAN)
                ) {

            type = AMCM_FUNC_KIND_REQUEST;
            scan_proc->SetProcessing(scan_proc->GetProcessing() | funcBit);

            AcquisitionCallback(type, body);
            str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                           acqman->GetStr(body->status),
                           acqman->GetStr(type),
                           acqman->GetStr(hdr->function));

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, str_msg);

        } else if ((hdr->function == AMCMA_ACQUISITION_DONE) ||
                   (hdr->function == AMCMA_ACQUISITION_CONTINUE_DONE) ||
                   (hdr->function == AMCMA_PREP_SCAN_DONE) ||
                   (hdr->function == AMCMA_MO_CORRECT_DONE) ||
                   (hdr->function == AMCMA_AMB_SCAN_DONE)) {

            type = AMCM_FUNC_KIND_ANSWER;

            pqm->SetRefProbeNotAlive(false);

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                      _T("[Response type: ANSWER]"));

            scan_proc->SetProcessing(scan_proc->GetProcessing() ^ funcBit);
            scan_proc->SetRequest(scan_proc->GetRequest() ^ funcBit);

            AcquisitionCallback(type, body);
            str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                           acqman->GetStr(body->status),
                           acqman->GetStr(type),
                           acqman->GetStr(hdr->function));

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, str_msg);

        } else if ((hdr->function & AM_OFFSET_AMCMS) == AM_OFFSET_AMCMS) {
            if (AcquisitionStatusCallback(type, hdr, body) == E_NO_ERROR) {
                return;
            }
        }

    } else if (body->status == AMCM_NACK) {

        if ((hdr->function == AMCMR_ACQUISITION) ||
            (hdr->function == AMCMR_ACQUISITION_CONTINUE) ||
            (hdr->function == AMCMR_PREP_SCAN)) {

            type = AMCM_FUNC_KIND_REQUEST;
            scan_proc->SetRequest(scan_proc->GetRequest() ^ funcBit);

        } else if ((hdr->function == AMCMA_ACQUISITION_DONE) ||
                   (hdr->function == AMCMA_ACQUISITION_CONTINUE_DONE) ||
                   (hdr->function == AMCMA_PREP_SCAN_DONE) ||
                   (hdr->function == AMCMA_MO_CORRECT_DONE)) {

            type = AMCM_FUNC_KIND_ANSWER;

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                      _T("[Response type: ANSWER]"));

            pqm->SetRefProbeNotAlive(false);
            scan_proc->SetProcessing(scan_proc->GetProcessing() ^ funcBit);
            scan_proc->SetRequest(scan_proc->GetRequest() ^ funcBit);
        }

        // errorId = body->status;
        //-Patni-MP/2009Dec17/Added/MVC006301,MVC006324
        if (hdr->function == AMCMA_ACQUISITION_DONE) {
            ErrAcquisition(type, _T("IDS_ERR_ACQMAN_ACQUISITION_DONE"), body);

        } else if (hdr->function == AMCMA_ACQUISITION_CONTINUE_DONE) {
            ErrAcquisition(type, _T("IDS_ERR_ACQMAN_ACQUISITION_CONTINUE_DONE"), body);

        } else if (hdr->function == AMCMR_ACQUISITION) {
            ErrAcquisition(type, _T("IDS_ERR_ACQMAN_ACQUISITION"), body);

        } else if (hdr->function == AMCMR_ACQUISITION_CONTINUE) {
            //+Patni-PJS-AP/2009Sep21/Modified/MVC004818, MVC004938
            if (!acqman->GetIsContinueScan()) {
                ErrAcquisition(type, _T("IDS_ERR_ACQMAN_ACQUISITION_CONTINUE"), body);
            }

            acqman->SetIsContinueScan(FALSE);
            //-Patni-PJS-AP/2009Sep21/Added/MVC004818, MVC004938

        } else if (hdr->function == AMCMR_PREP_SCAN) {

            ErrAcquisition(type, _T("IDS_ERR_ACQMAN_PREP_SCAN"), body);

        } else if (hdr->function == AMCMA_PREP_SCAN_DONE) {

            ErrAcquisition(type, _T("IDS_ERR_ACQMAN_PREP_SCAN_DONE"), body);

        } else if (hdr->function == AMCMR_MO_CORRECT) {

            ErrAcquisition(type, _T("IDS_ERR_ACQMAN_MO_CORRECT"), body);

        } else if (hdr->function == AMCMA_MO_CORRECT_DONE) {

            ErrAcquisition(type, _T("IDS_ERR_ACQMAN_MO_CORRECT_DONE"), body);
        }

        //-Patni-MP/2009Dec17/Added/MVC006301,MVC006324

    } else {
        if ((hdr->function == AMCMR_ACQUISITION) ||
            (hdr->function == AMCMR_ACQUISITION_CONTINUE) ||
            (hdr->function == AMCMR_PREP_SCAN)) {

            type = AMCM_FUNC_KIND_REQUEST;
            int result = 0;
            result = scan_proc->GetRequest();
            result ^= funcBit;
            //MRPdefect812
            scan_proc->SetRequest(result);
            //MRPdefect812

        } else if ((hdr->function == AMCMA_ACQUISITION_DONE) ||
                   (hdr->function == AMCMA_ACQUISITION_CONTINUE_DONE) ||
                   (hdr->function == AMCMA_PREP_SCAN_DONE)) {

            type = AMCM_FUNC_KIND_ANSWER;

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                      _T("[Response type: ANSWER]"));

            scan_proc->SetProcessing(scan_proc->GetProcessing() ^ funcBit);
            scan_proc->SetRequest(scan_proc->GetRequest() ^ funcBit);
        }

        //-Patni-MP/2009Dec17/Added/MVC006301,MVC006324
        if (hdr->function == AMCMA_ACQUISITION_DONE) {
            ErrAcquisition(type, _T("IDS_ERR_ACQMAN_ACQUISITION_DONE"), body);

        } else if (hdr->function == AMCMA_ACQUISITION_CONTINUE_DONE) {
            ErrAcquisition(type, _T("IDS_ERR_ACQMAN_ACQUISITION_CONTINUE_DONE"), body);

        } else if (hdr->function == AMCMR_ACQUISITION) {
            ErrAcquisition(type, _T("IDS_ERR_ACQMAN_ACQUISITION"), body);

        } else if (hdr->function == AMCMR_ACQUISITION_CONTINUE) {

            ErrAcquisition(type, _T("IDS_ERR_ACQMAN_ACQUISITION_CONTINUE"), body);

        } else if (hdr->function == AMCMR_PREP_SCAN) {

            ErrAcquisition(type, _T("IDS_ERR_ACQMAN_PREP_SCAN"), body);

        } else if (hdr->function == AMCMA_PREP_SCAN_DONE) {

            ErrAcquisition(type, _T("IDS_ERR_ACQMAN_PREP_SCAN_DONE"), body);

        } else if (hdr->function == AMCMR_MO_CORRECT) {

            ErrAcquisition(type, _T("IDS_ERR_ACQMAN_MO_CORRECT"), body);

        } else if (hdr->function == AMCMA_MO_CORRECT_DONE) {

            ErrAcquisition(type, _T("IDS_ERR_ACQMAN_MO_CORRECT_DONE"), body);

        } else if (hdr->function == AMCMA_AMB_SCAN_DONE && body->status == AMCM_SEQGEN) {

            ErrAcquisition(type, _T("IDS_ERR_ACQMAN_PREP_SCAN_DONE"), body);
        }

        //-Patni-MP/2009Dec17/Added/MVC006301,MVC006324

        if (scan_proc->GetProcessing()) {
            //          scan_proc->ResetCurrentRequest();
        }
    }
}

//*******************************Method Header*********************************
//Method Name    : AcquisitionCallback
//Author         : PATNI/GP
//Purpose        : Callback for Acquisition
//*****************************************************************************
int CScan::AcquisitionCallback(
    const int type,
    AM_AcqManToClient_t* data
)
{
    LPCTSTR FUNC_NAME = _T("CScan::AcquisitionCallback");

    //+Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(data);

    CScanProcedure* scan_proc = GetScanProcedure();

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (scan_proc == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Scan procedure pointer is NULL"));
        return FALSE;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    CPqmPmAcqman* acqman_manager = scan_proc->GetAcqManager();	//Patni-MJC/2009Aug17/Modified/cpp test corrections

    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
              acqman_manager->GetStr(type));

    CPqm* pqmptr = acqman_manager->GetPqm();


    if (type == AMCM_FUNC_KIND_REQUEST) {
        //PqmUiSetSensitive(W_SCAN_START_BTN,FALSE);
        //PqmUiSetSensitive(W_SCAN_PAUSE_BTN,FALSE);
        //PqmUiSetSensitive(W_SCAN_ABORT_BTN,TRUE);

        scan_proc->SetScanStart(NULL);
        scan_proc->SetScanPause(NULL);
        scan_proc->SetScanAbort(PQM_SCAN_ABORT);

        //PQM_ACQ_STATUS();
        return E_NO_ERROR;
    }

    if (!scan_proc->GetScanProc(NUM_2).IsEmpty()) {
        //+Patni-HAR/2009Aug11/Added/Memory Leaks
        // Here, we are not using cb_data outside this function,
        //as we are not calling pqmAcqScanProcAfterScan() function
        //so no need to create the object
        //Please remove commented code when K_PM_SCAN_PROC_3 will
        //implement
        //AM_AcqManToClient_t *cb_data = new AM_AcqManToClient_t;

        //if (cb_data == NULL) {
        //    return FALSE;
        //}

        //memcpy(cb_data,data,sizeof(AM_AcqManToClient_t));
        //-Patni-HAR/2009Aug11/Added/Memory Leaks

        //DB_PRINTF("A",("K_PM_SCAN_PROC_3\n"));
        //PqmPmActionTools(K_PM_SCAN_PROC_3,(void *)PqmAcqGlobal.curObj,
        //  PqmAcqGlobal.scanProc[2],
        //  pqmAcqScanProcAfterScan,cb_data);
        scan_proc->SetProcessing(scan_proc->GetProcessing() | PQM_ACQ_BIT_SCAN_PROC);
        return E_NO_ERROR;

    } else {
        //Patni-PJS/2010Feb24/Added/DeFT# PROPOSAL_08-Remove EZScan
        if (scan_proc->GetIsScanning()) {
            return AcquisitionDone();

        } else {
            return E_NO_ERROR;
        }
    }

    pqmptr->SeriesScanDone();
    return E_NO_ERROR;
}

//*******************************Method Header*********************************
//Method Name    : ErrAcquisition
//Author         : PATNI/GP
//Purpose        : To check error in qcuisition
//*****************************************************************************
int CScan::ErrAcquisition(
    const int type,
    //-Patni-MP/2009Dec17/Modified/MVC006301,MVC006324
    const CString& error_str,
    AM_AcqManToClient_t* data
)
{
    LPCTSTR FUNC_NAME = _T("CScan::ErrAcquisition");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace of ErrAcquisition"));

    CScanProcedure* scan_proc = GetScanProcedure();
    CPqmPmAcqman* acqman = scan_proc->GetAcqManager();
    CPqm* pqm = acqman->GetPqm();
    CPqmProtocol* curr_protocol = scan_proc->GetScanProcProtocol();
    CPqmProcon* l_procon = pqm->GetPqmProCon();

    //+Patni-RUP/2009Aug25/Modified/cpp test corrections
    if (curr_protocol == NULL) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                  _T("curr_protocol pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-RUP/2009Aug25/Modified/cpp test corrections

    //+Patni-HEMANT/ADDED On 6/30/2009 6:56:43 PM/ ACE-2 / WFDA
    //QTS#38 :sending scan fail message to InScan server just before changing the GUI status.
    //
    //Patni-PJS/2011Jan25/Modified/IR-97
    //pqm->ProcessScanRelatedMsgForWFDA(curr_protocol, PQM_MSG_ABORT_FAIL);
    //-Patni-HEMANT/ ADDED On6/30/2009 6:57:26 PM/ ACE-2 / WFDA

    //+Patni-RUP/2009Aug25/Modified/cpp test corrections
    if (type != AMCM_FUNC_KIND_REQUEST) {
        acqman->AcqDeleteLockFile();
    }

    if (curr_protocol) {
        pqm->GetStudy()->SetAcqStatus(curr_protocol, C_PR_STATUS_FAIL);

        //-Patni-MP/2009Dec17/Added/MVC006301
        if (curr_protocol->GetPrepScan() ||
            (curr_protocol->GetAcqSplitMode() == VFC_ACQ_SPLIT_MODE_PREP)) {
            pqm->WriteToUI(PQM_MSG_INJECT_TIME_INVISIBLE);

        }

        //Patni-MP-DKH/2010Mar18/Commented/PH#3#/IR126
        //pqm->WriteToUI(PQM_MSG_ABORT_FAIL);  // L"ABORT_FAIL"
        //-Patni-MP/2009Dec17/Added/MVC006301

    }

    //-Patni-RUP/2009Aug25/Modified/cpp test corrections

    //////////////////////////////////////////////////////////////////////////

    //	PostThreadMessage(CPQMIPCManager::dThreadId,MSG_SETSERISENO_IN_DB,(WPARAM)(LPCTSTR)curr_protocol->GetSeriesLOID(),(LPARAM)curr_protocol->GetAcqStatus());
    //  WaitForSingleObject(CPQMIPCManager::m_ThreadEvent,INFINITE);
    //+Patni-NP/2009Mar09/Added/PSP1 Changes For VF Lock
    //Patni-PJS/2009Dec15/Added/MVC006256
    //    BOOL l_postmsg_status = pqm->m_dbsa_study_mgr->SetSeriesNoInDB((LPCTSTR)curr_protocol->GetSeriesLOID(), curr_protocol->GetAcqStatus());
    //-Patni-NP/2009Mar09/Added/PSP1 Changes For VF Lock

    //	// - Sarjeet
    //////////////////////////////////////////////////////////////////////////////
    /*  V5.00 for RGN SKIP */
    //+Patni-PJS/2009Dec22/Added/MVC006256
    int l_status = E_ERROR;

    if (!curr_protocol->GetSeriesNumInDbFlag()) {
        l_status = pqm->GetDbsaStudyMgr()->SetSeriesNoInDB((LPCTSTR)curr_protocol->GetSeriesLOID(), curr_protocol->GetAcqStatus());

        if (l_status == E_ERROR) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("Failed to set series in DB"));

        } else {
            curr_protocol->SetSeriesNumInDbFlag(TRUE);
        }
    }

    //Patni-HAR/2010Mar26/Added/MVC007422

    //-Patni-PJS/2009Dec22/Added/MVC006256
    CString l_errorMsg = _T("");
    //Patni-Ravindra Acharya/2010April12/Added/PH#3#/IR-126
    int l_error_level = SCAN_WARN_INFORMATION ;

    if (data->status == AMCM_SAR) {
        //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
        //+Patni-MP/2009Dec04/Modified/MVC004957
        l_errorMsg = CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_SAR"));

    } else if (data->status == AMCM_OLPOVERTEMP) {

        l_errorMsg = CPqmUtilities::GetMultiLingualString(_T("ERR_OLPOVERTEMP"));

        //-Patni-MP/2009Dec04/Modified/MVC004957
    }//+Patni-DKH/2010Mar10/Added/PH#3#/IR-126

    else if (data->status == AMCM_GAMPTEMP) {
        l_errorMsg = CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_GAMPTEMP"));

        //+Patni-Ravindra Acharya/2010April12/Added/PH#3#/IR-126
        l_error_level = SCAN_WARN_WARNING ;
        //-Patni-Ravindra Acharya/2010April12/Added/PH#3#/IR-126
    }

    //-Patni-DKH/2010Mar10/Added/PH#3#/IR-126
    else {
        //PqmUiErrorDialog(PqmUiFetchLiteral(cb_p->literal),ERR_ERROR);
        //-Patni-MP/2009Dec17/Added/MVC006301,MVC006324
        l_errorMsg = CPqmUtilities::GetMultiLingualString(error_str);
    }

    //+Patni-MP/2009Dec04/Modified/MVC004957
    MPLUSERRORUSERRESPONSE_e    l_response = MPlusErrResponse_Error;
    CString l_warnapp_name = _T("");
    l_warnapp_name.LoadString(IDS_WARNING_APP_NAME);
    l_response = (MPLUSERRORUSERRESPONSE_e) CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(
                     l_errorMsg,
                     l_warnapp_name,
                     l_error_level,					//+Patni-Ravindra Acharya/2010April12/Modified/PH#3#/IR-126
                     SCAN_WARN_SYSMODAL,
                     SCAN_WARN_OKBUTTON);

    //-Patni-MP/2009Dec04/Modified/MVC004957
    //Patni-MP-DKH/2010Mar18/Added/PH#3#/IR126
    pqm->WriteToUI(PQM_MSG_ABORT_FAIL);  // L"ABORT_FAIL"

    pqm->GetAMB()->PqmPmAbortAmb();

    if (scan_proc->GetProbing()) {

        //pqmAcqErrProbing(NULL,NULL,NULL) ;
        l_procon->StopProlim();
        scan_proc->SetProbing(FALSE);

    } else {
        CPqmProbe* pqm_probe = scan_proc->GetPqmProbe();

        if (pqm_probe) {
            pqm_probe->RefProbeScanEnd(); //RMC
        }
    }

    //+Patni-RUP/2009Aug25/Modified/cpp test corrections
    //if(curr_protocol == NULL) {
    //    return E_ERROR;
    //}
    //-Patni-RUP/2009Aug25/Modified/cpp test corrections

    scan_proc->ResetCurrentRequest();
    pqm->GetStudy()->SetScanModeOfHead();

    return E_NO_ERROR;
}

//*******************************Method Header*********************************
//Method Name    :PrepareRequest()
//Author         :PATNI/HAR
//Purpose        :This method prepares the Prescan request that will be sent to Acqman
//*****************************************************************************
bool CScan::PrepareRequest(
)
{
    LPCTSTR FUNC_NAME = (_T("CScan::PrepareRequest"));
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, FUNC_NAME);

    CScanProcedure* scanproc = GetScanProcedure();

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (scanproc == NULL) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Scan Procedure pointer is NULL"));

        return E_ERROR;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    CPqmPmAcqman* acqman = scanproc->GetAcqManager();		//Patni-MJC/2009Aug17/Modified/cpp test corrections
    CPqmProtocol* curr_protocol = scanproc->GetScanProcProtocol();
    CPqm* pqm = acqman->GetPqm();
    CPqmProbe* l_refprobeptr = scanproc->GetPqmProbe();

    //Patni-DKH/2010Dec03/Added/TMSC-REDMINE-979
    pqm->WriteToUI(PQM_MSG_SRGL_SCANSTART);

    //+Patni-HAR/2010Oct28/Added/V1.30#TMSC-REDMINE-860
    if (!curr_protocol->GetSeriesNumInDbFlag()) {

        pqm->GetDbsaStudyMgr()->WriteSeriesInformationAtSeriesLevel(
            curr_protocol->GetSeriesLOID());

        int l_postmsg_status = pqm->GetDbsaStudyMgr()->SetSeriesNoInDB(
                                   (LPCTSTR)curr_protocol->GetSeriesLOID(), curr_protocol->GetAcqStatus());

        if (E_ERROR == l_postmsg_status) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed to set series in DB"));

            //+Patni-PJS/2010Nov9/Added/V1.30#TMSC-REDMINE-860 error scenario

            MRERROR_LOG_DATA l_error_log_data;
            l_error_log_data.error_message = CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_DATABASE_UPDATE_FAILED"));
            l_error_log_data.error_source = _T("PQM");
            CPQMLogMgr::GetInstance()->DisplayMessageinErrorDialog(l_error_log_data, MPlus::EH::Information);
            //-Patni-PJS/2010Nov9/Added/V1.30#TMSC-REDMINE-860 error scenario

            return false;
        }

        curr_protocol->SetSeriesNumInDbFlag(TRUE);
    }

    if (!pqm->GetStudyManagerPtr()->UpdateAnatomyValueInMPlusDB(curr_protocol)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("UpdateAnatomyValueInMPlusDB failed."));
    }

    if (pqm->GetStudy()->IsProtocolInScan(curr_protocol->GetProtocol())) {

        pqm->WriteToUI(PQM_MSG_INSCAN_SCAN_START);

        if (!pqm->GetStudy()->IsValidWFDAScan()) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("Inscan scan is not valid"));
            pqm->GetStudy()->SetScanModeOfHead();
            scanproc->ResetCurrentRequest();

            pqm->GetStudy()->SetAcqStatus(curr_protocol, C_PR_STATUS_FAIL);
            pqm->WriteToUI(PQM_MSG_ABORT_FAIL);

            return E_ERROR;
        }
    }


    //Patni-HAR/2010Oct28/Added/V1.30#TMSC-REDMINE-860

    memset(&m_header, 0, sizeof(AM_ClientMessageHeader_t));
    //memset(&m_data, 0, sizeof(AM_ClientToAcqMan_t));
    memset(&m_data, 0, sizeof(AM_ClientToAcqMan_unicode_t));

    // RefProbe

    //+Patni-MSN/2009Mar10/Modified/Modified according to Base Code condition
    if (curr_protocol->GetProbeCorrection()/*&&(pqm->GetRefProbeNotAlive() == false)*/) {
        //-Patni-MSN/2009Mar10/Modified/Modified according to Base Code condition

        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                  _T("*****invoke REFPROBE *****"));
        //saurabh
        //pqm->WriteToUI(PQM_MSG_INVOKE_REFPROBE);
        l_refprobeptr->StartProbeClientThread();
        //+Patni-MSN/2009Mar12/Modified/Optimization
        l_refprobeptr->SetProbeMode(1);
        PostThreadMessage(CPqmProbe::GetThreadID(), MSG_PROBE, (WPARAM)NULL, (LPARAM)NULL);
        //+Patni-RUP/2009Dec15/Internal defect fix
        //WaitForSingleObject(CPqmProbe::m_probe_event, INFINITE);
        //-Patni-RUP/2009Dec15/Internal defect fix
        //+ Patni-MSN/2009Apr17/Modified/PSP-Code Review
        //+Patni-RUP/2009Dec15/Internal defect fix
        //l_refprobeptr->ConnectToProbe();
        //-Patni-RUP/2009Dec15/Internal defect fix
        //- Patni-MSN/2009Apr17/Modified/PSP-Code Review
        //  return true;
        //-Patni-MSN/2009Mar12/Modified/Optimization
    }

    CPqm* l_pqm = acqman->GetPqm();

    if (l_pqm->GetAMB()->PqmAMBScanSeg2Active()) {

        m_header.function = AMCMM_AMB_SCAN;
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                  _T("***** AMB_SCAN *****"));

    } else if ((curr_protocol->GetPrepScan()  &&
                l_pqm->GetAMB()->CanPerformPrep()
               ) ||
               curr_protocol->GetAcqSplitMode() == VFC_ACQ_SPLIT_MODE_PREP) {

        m_header.function = AMCMM_PREP_SCAN;

        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                  _T("***** PREP_SCAN *****"));

    } else if ((curr_protocol->GetProbeScan() == VFC_PROBE_RTMC)/*&&(!pqm->GetRefProbeNotAlive())*/) {
        m_header.function = AMCMM_MO_CORRECT;

        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                  _T("***** AMCMM_MO_CORRECT *****"));

    } else {

        if (!(scanproc->GetScanProc(1).IsEmpty()) ||
            curr_protocol->GetFluoro() ||
            curr_protocol->GetPrepScan() ||
            (curr_protocol->GetAcqSplitMode() == VFC_ACQ_SPLIT_MODE_PREP)
           ) {

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                      _T("***** ACQUISITION *****"));
            m_header.function = AMCMM_ACQUISITION;

        } else {

            m_header.function = AMCMM_ACQUISITION_CONTINUE;

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,                    _T("***** ACQUISITION CONTINUE *****"));
        }
    }

    //    l_study_path = CPQMConfig::GetInstance()->GetStudyPath();
    //    if ( l_study_path.GetLength() <= 0) {
    //        CPQMLogMgr::GetInstance()->WriteEventLog(IDS_STUDY_NOT_FOUND,ERROR_LOG_CRUCIAL1 ,_T("Scan.cpp"));
    //        return false;
    //    }
    CString l_study_path = pqm->GetStudyUidPath();

    if (l_study_path.GetLength() <= 0) {
        //+Patni-PJS/2009May10/Modify/IR-82 review comment
        CString string = _T("");
        string.LoadString(IDS_STUDY_NOT_FOUND);
        CPQMLogMgr::GetInstance()->WriteEventLog(string, ERROR_LOG_CRUCIAL1 , _T("Scan.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82
        /*CPQMLogMgr::GetInstance()->WriteEventLog(IDS_STUDY_NOT_FOUND, ERROR_LOG_CRUCIAL1 , _T("Scan.cpp"));*/
        //-Patni-PJS/2009May10/Modify/IR-82 review comment
        return false;
    }

    //    for ( int index = 0; index < l_study_path.GetLength(); index++) {
    //      m_data.study_file[index] = l_study_path.GetAt(index);
    //  }
    //  m_data.study_file[index] = '\0';

    _tcscpy(m_data.study_file, l_study_path);
    m_data.protocol_node = curr_protocol->GetProtocol();

    if (curr_protocol->GetPrepScan() ||
        curr_protocol->GetAcqSplitMode() == VFC_ACQ_SPLIT_MODE_PREP) {

        CString l_prep_study = _T("/gp/tmp/prepStudy");

        //+Patni-DKH/2010Oct15/Modified/MVC009192
        if (PQM_WARN_SCAN_PREP_STUDY == MaintainPrepStudy(l_prep_study, curr_protocol)) {	// + SM4 Himanshu VISUAL_PREP 13 JAN 2009

            CString l_warnapp_name = _T("");
            l_warnapp_name.LoadString(IDS_WARNING_APP_NAME);

            CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(
                CPqmUtilities::GetMultiLingualString(_T("IDS_WARN_SCAN_PREP_STUDY")),
                l_warnapp_name,
                SCAN_WARN_INFORMATION,
                SCAN_WARN_SYSMODAL, SCAN_WARN_OKBUTTON);

            //+Patni-DKH/2010Oct15/Commented/TMSC-REDMINE-860
            //scanproc->ResetCurrentRequest();

            //if (NULL != pqm) {
            //    pqm->GetStudy()->SetAcqStatus(scanproc->GetScanProcProtocol(), C_PR_STATUS_WAIT);
            //    pqm->WriteToUI(PQM_MSG_ABORT_WAIT);
            //}
            //-Patni-DKH/2010Oct15/Commented/TMSC-REDMINE-860
            return false;
        }

        //-Patni-DKH/2010Oct15/Modified/MVC009192
        _tcscpy(m_data.param.visualPrep.prep_study, l_prep_study);
        m_data.param.visualPrep.prep_protocol = 0;
    }

    if (curr_protocol->GetProbeScan() == VFC_PROBE_RTMC) {
        //+Patni-MSN/2009Mar12/Modified/Optimization
        //+Patni-DKH/2010Dec15/Modified/TMSC-REDMINE-655
        CString l_probe_study = _T("/gp/tmp/probeStudy");

        if (MaintainMotionCorrectStudy(l_probe_study, curr_protocol) == E_ERROR) {

            CString l_warnapp_name = _T("");
            l_warnapp_name.LoadString(IDS_WARNING_APP_NAME);

            CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(
                CPqmUtilities::GetMultiLingualString(_T("IDS_WARN_SCAN_PROBE_STUDY")),
                l_warnapp_name,
                SCAN_WARN_INFORMATION,
                SCAN_WARN_SYSMODAL, SCAN_WARN_OKBUTTON);
            return false;
        }

        //+Patni-DKH/2010Dec15/Modified/TMSC-REDMINE-655
        //-Patni-MSN/2009Mar12/Modified/Optimization
        _tcscpy(m_data.param.motionCorrect.probe_study, _T("/gp/tmp/probeStudy"));
        m_data.param.motionCorrect.probe_protocol = 0;
    }

    // PqmIrsNotifyScanStart(&PqmGlobal.pqmStudy.studyId);
    if (!(scanproc->GetScanProc(1).IsEmpty())) {
        // Not in this phase
        scanproc->SetProcessing(scanproc->GetProcessing() | PQM_ACQ_BIT_SCAN_PROC);
        return true;

    } else {
        return SendMessage();
    }
}


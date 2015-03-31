//REDMINE-1213_Update_03_MAY
//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-774
//Redmine-778
//Redmine-779
//Redmine-780
//Redmine-781
//Redmine-782
/***********************************************************************************
*
*    Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
*
*    Module: PQM-IPC
*    Author: PATNI
*************************************************************************************

                         Revision History
*******************************************************************************
*  Revision      Author           Date & Time            Changes
*  ==========================================================================
*   1.0           PATNI      2008/6/30 13:00:00     Created
*
******************************************************************************/

#include "stdafx.h"
#include "PqmShimStruct.h"
#include <libStudyManager/Log/ScopedFuncInOutLog.h>
#include <PQM/PQMLogMgr.h>
#include <PQM/resource.h>
#include <PQM/PqmUtilities.h> // Added By KT/ACE-2/15-06-2009/Multilingualisation
#include "shimcommon.h"
#include "PqmMRSScan.h"
#include "Pqm.h"
#include "prescan.h"
#include "scanprocedure.h"
#include "pqmpmacqman.h"
//Patni-RSG/2009Nov11/Added/IR-116
#include "PQMXMLParser.h"
#include "VarFieldHandle.h" //TMSC-Tanoue/2010Aug25/MVC008345
#include "PqmPrMode.h"		//Patni-Hemant/03May2011/Modified/IR-168_Part3

//#define SHOW_SPEC_SELECTION 2 //Patni-Rajeesh/25Jan2011/Commented/IR-168-part2
//****************************Method Header************************************
//Method Name   : CPqmMRSScan
//Author        : PATNI
//Purpose       : Construction
//*****************************************************************************
CPqmMRSScan::CPqmMRSScan(CScanProcedure* scan_proc):
    CScanTransaction(scan_proc),
    m_prescan(NULL)
{
    LPCTSTR FUNC_NAME = _T("CPqmMRSScan::CPqmMRSScan");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ScanTransaction Created"));

    if (scan_proc)m_localshimming.SetPqmToLocalShim(scan_proc->GetPqmPtr());
}

//****************************Method Header************************************
//Method Name   : ~CPqmMRSScan
//Author        : PATNI
//Purpose       : Destruction
//*****************************************************************************
CPqmMRSScan::~CPqmMRSScan()
{
    LPCTSTR FUNC_NAME = _T("CPqmMRSScan::~CPqmMRSScan");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ScanTransaction Destroyed"));
}

//****************************Method Header************************************
//Method Name   : InitMRS
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int CPqmMRSScan::InitMRS()
{
    LPCTSTR FUNC_NAME = _T("CPqmMRSScan::InitMRS");
    PQM_SCOPED_TRACE();

    //+Patni-Hemant/03May2011/Added/IR-168_Part3
    CPqmProtocol* l_scan_proc_prot = m_scan_procedure->GetScanProcProtocol();

    if (NULL == l_scan_proc_prot) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("scan_proc_prot is NULL"));
        return FALSE;
    }

    //-Patni-Hemant/03May2011/Added/IR-168_Part3

    char systime[MAX_BUFFER] = {0};
    SYSTEMTIME lpSystemTime ;
    // Get current system time
    GetSystemTime(&lpSystemTime);  // system time

    // + Patni - HAR / 2009 Mar 23 / Added / Defect#245
    char* l_tmp_file = new char[256];
    memset(l_tmp_file, 0 , sizeof(char) * 256);
    // - Patni - HAR / 2009 Mar 23 / Added / Defect#245
    sprintf(systime, "%d%d%d%d%d%d",
            lpSystemTime.wYear,
            lpSystemTime.wMonth,
            lpSystemTime.wDay,
            lpSystemTime.wHour,
            lpSystemTime.wMinute,
            lpSystemTime.wSecond);

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_scan_procedure) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("m_scan_procedure is NULL"));
        return FALSE;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    // get study file name
    CString l_stdy_file_name = m_scan_procedure->GetAcqManager()->GetPqm()->GetStudy()->GetStudyName();
    int l_searchpos = l_stdy_file_name.ReverseFind('/');
    l_searchpos = l_searchpos + 1;
    int	l_ret = l_stdy_file_name.Delete(0, l_searchpos);


    // + MEM_INI AMIT 20081230
    //CHAR* l_study_file = new char(l_stdy_file_name.GetLength()+1);
    CHAR* l_study_file = new char[l_stdy_file_name.GetLength() + 1];
    memset(l_study_file, 0, sizeof(char) * (l_stdy_file_name.GetLength() + 1));
    // - MEM_INI AMIT 20081230
    wcstombs(l_study_file , l_stdy_file_name, l_stdy_file_name.GetLength() + 1);//-Patni-HAR/2009Aug31/Modified/Corrected call for wcstombs()
    // Get Scratchpad file path

    //+Patni-Hemant/03May2011/Modified/IR-168_Part3
    //+ Patni-MSN/2009Apr17/Modified/PSP#381
    char*	path = NULL;

    if ((path = getenv("GP_TMP_DIR")) != NULL) {
        sprintf(l_tmp_file, "%s%s%s_%d_%s", path, "/PQM.ScanProc", l_study_file,
                l_scan_proc_prot->GetProtocol(), systime);

    } else {
        sprintf(l_tmp_file, "%s%s%s_%d_%s", "/gp/tmp", "/PQM.ScanProc", l_study_file,
                l_scan_proc_prot->GetProtocol(), systime);
    }

    //-Patni-Hemant/03May2011/Modified/IR-168_Part3

    //- Patni-MSN/2009Apr17/Modified/PSP#381
    // Scratchpad file path/name should be in below format:
    // [/gp/tmp/PQM.Scanproc<Stusy Run number>_<protocol number>_<time stamp>]

    /** make empty file **/

    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, CString(_T("Scratpad File Name :")) + l_tmp_file);

    FILE* fp = fopen(l_tmp_file, "a");

    if (fp) {
        fclose(fp);
    }

    //Patni-Hemant/03May2011/Modified/IR-168_Part3
    m_scan_procedure->GetAcqManager()->GetPqm()->GetStudy()->MaintainScratchPadforMRS(l_scan_proc_prot->GetProtocol(), l_tmp_file, SP_CREATE_MODE_PRIVATE_PROLIM);

    CString l_stdy_org_file_name = m_scan_procedure->GetAcqManager()->GetPqm()->GetStudy()->GetStudyName();
    // + MEM_INI AMIT 20081230
    //CHAR* l_study_org_file = new char(l_stdy_org_file_name.GetLength()+1);
    CHAR* l_study_org_file = new char[l_stdy_org_file_name.GetLength() + 1];
    memset(l_study_org_file, 0, sizeof(char) * (l_stdy_org_file_name.GetLength() + 1));
    // + MEM_INI AMIT 20081230
    wcstombs(l_study_org_file , l_stdy_org_file_name, l_stdy_org_file_name.GetLength() + 1);//-Patni-HAR/2009Aug31/Modified/Corrected call for wcstombs()
    // Get Scratchpad file path

    //Patni-Hemant/03May2011/Modified/IR-168_Part3
    if (ShimGlobalInit(l_study_org_file,
                       l_tmp_file,
                       l_scan_proc_prot->GetProtocol()) != E_NO_ERROR) {
        //+Patni-SS/2009Sep30/added/MVC005060
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Error ShimGlobalInit"));

        m_localshimming.ShimQuit(SHIM_FATAL_ERROR);
        return false;
    }

    if (!globalP->shimStudyP->study) {
        //+Patni-SS/2009Sep30/added/MVC005060
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("shimStudyP->study is NULL"));
        m_localshimming.ShimQuit(SHIM_FATAL_ERROR);
        return FALSE;
    }

    if (globalP->shimStudyP->protocol == -1) {
        //+Patni-SS/2009Sep30/added/MVC005060
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("protocol == -1"));
        m_localshimming.ShimQuit(SHIM_FATAL_ERROR);
        return FALSE;
    }

    if (MrsShimStudyOpen() != E_NO_ERROR) {
        //+Patni-SS/2009Sep30/added/MVC005060
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Error MrsShimStudyOpen"));
        m_localshimming.ShimQuit(SHIM_FATAL_ERROR);
        return FALSE;
    }

    if (MrsPositionRead() != E_NO_ERROR) {

        //+Patni-SS/2009Sep30/added/MVC005060
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Error mrsPositionRead"));

        m_localshimming.ShimQuit(SHIM_FATAL_ERROR);
        return FALSE;
    }

    if (MrsCheckVoxelMatrixInside() != MrsSliceInside) {
        //+Patni-HAR/2009April09/Added/MRSCSI Scan Fix
        MrsPopErrorDialog(3, IDS_OP_ERROR_MRS_PARENT, _T("IDS_OP_ERROR_MRS_PARENT"));	 // Added By KT/ACE-2/11-06-2009/Multilingualisation
        //-Patni-HAR/2009April09/Added/MRSCSI Scan Fix
        return FALSE;
    }

    if (globalP->shimStudyP->hrShimNodes[SVD_ST_PROTOCOL - 1].subtree_name == 0) {
        //+Patni-HAR/2009April09/Added/MRSCSI Scan Fix
        MrsPopErrorDialog(3, IDS_OP_ERROR_HR, _T("IDS_OP_ERROR_HR"));				 // Added By KT/ACE-2/11-06-2009/Multilingualisation
        //-Patni-HAR/2009April09/Added/MRSCSI Scan Fix
        return FALSE;
    }

    if (ShimHrInfoRead() != E_NO_ERROR) {
        //+Patni-SS/2009Sep30/added/MVC005060
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Error shimHrInfoRead"));

        m_localshimming.ShimQuit(SHIM_FATAL_ERROR);
        return FALSE;
    }

    if (m_scan_procedure->GetPrescan() == K_PRESCAN_MODE_EVERY) {
        m_scan_procedure->AddTransaction(this);

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("MRS scan skipped for first (clinical) prescan"));
        m_scan_procedure->AddTransaction((CScanTransaction*)m_prescan);
        m_prescan->SendMessage();
        return TRUE;
    }

    if (globalP->shimStudyP->acq_hold_mode & VFC_HOLD_PRE_SCAN) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("MRS in Non-AutoScan Mode"));

        /* 対話モード */
        globalP->exec_mode = MRS_EXE_MANUAL;
        /* メイン画面の初期化 */
        SetEddyAxisExecute();
        // globalP->ui = MrsUICreate(globalP->app,globalP->topLevel,(MrsUIProc_t)mrsCB,globalP);
        m_scan_procedure->GetAcqManager()->GetPqm()->WriteToUI(PQM_MSG_MRS_SCAN); // message to PQM GUI to display MRS UI

    } else {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("MRS in AutoScan Mode"));

        /* 自動モード */
        globalP->exec_mode = MRS_EXE_AUTO;
        globalP->wsatInfoP->show_spect = 0;
        globalP->mrsStateP->local = MRS_STATE_CURR;

        if (((globalP->mrsInfoP->matrixPE != 1) ||
             (globalP->mrsInfoP->matrixRO != 1)) &&
            globalP->mrsInfoP->PacFlag != ON) {

            //globalP->mrsStateP->ref = MRS_STATE_SKIP;
            //+Patni-MP/2009Nov26/Modified/MSA248-00019
            globalP->mrsStateP->ref = VFC_PRESCAN_MRS_EXE;
            //-Patni-MP/2009Nov26/Modified/MSA248-00019

        }

        SetEddyAxisExecute();

        if (m_scan_procedure->GetAcqManager()->GetPqm()->GetMRS_BRST_License()) {
            //Patni-Hemant/03May2011/Modified/IR-168_Part3
            globalP->mrsStateP->f0 = CanDisplaySpectrumGraph(l_scan_proc_prot) ? VFC_PRESCAN_MRS_SHOW_GRAPH : globalP->mrsStateP->f0 ;
        }

        // Set MRS prescan value in study file
        m_scan_procedure->GetAcqManager()->GetPqm()->GetStudy()->SetMRSExecutation(m_scan_procedure->GetScanProcProtocol()->GetProtocol(),
                globalP->mrsStateP->f0, globalP->mrsStateP->ref, globalP->mrsStateP->wsat);

        int status = 0; //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections

        if ((status = m_localshimming.ShimLocalRun(globalP)) != E_NO_ERROR) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Error shimHrInfoRead"));

            if (status == SHIM_AREA_ERROR) {
                MrsPopErrorDialog(3, IDS_OP_ERROR_MRS_ROI, _T("IDS_OP_ERROR_MRS_ROI"));

            } else {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Calling m_localshimming.ShimQuit(SHIM_FATAL_ERROR)"));
                m_localshimming.ShimQuit(SHIM_FATAL_ERROR);
            }

            return FALSE;

        } else {
            // Do post process of Local shimming
            LocalShimDone();
        }
    }

    return TRUE;
}


//****************************Method Header************************************
//Method Name   : GetDefaultValue
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int CPqmMRSScan::GetDefaultValue(
    SMRSScan_t* f_mrs_default_value
)const
{
    //+Patni-MP/2009Dec01/Added/MSA248-00019
    f_mrs_default_value->ref = globalP->mrsStateP->ref;
    f_mrs_default_value->ref_change_allow = TRUE;

    //-Patni-MP/2009Dec01/Added/MSA248-00019
    if (((globalP->mrsInfoP->matrixPE != 1) ||
         (globalP->mrsInfoP->matrixRO != 1)) &&
        globalP->mrsInfoP->PacFlag != 1) {
        //+Patni-MP/2009Dec01/Modified/MSA248-00019
        f_mrs_default_value->ref = VFC_PRESCAN_MRS_EXE;

        f_mrs_default_value->ref_change_allow = TRUE;
        //+Patni-MP/2009Dec01/Modified/MSA248-00019
    }

    //+Patni-MP/2009Dec01/Modified/MSA248-00019
    if (globalP->mrsInfoP->PacFlag == 1) {
        f_mrs_default_value->ref = MRS_STATE_WAIT;
        f_mrs_default_value->ref_change_allow = FALSE;

    }

    //+Patni-MP/2009Dec01/Modified/MSA248-00019
    f_mrs_default_value->shim = globalP->mrsStateP->local;				//Patni-Ravindra Acharya/09Dec2010/Added/IR-168
    f_mrs_default_value->wsat = globalP->mrsStateP->wsat;
    f_mrs_default_value->shim_change_allow = TRUE ;						//Patni-Ravindra Acharya/02Dec2010/Added/IR-168
    f_mrs_default_value->wsat_change_allow = TRUE;

    //Patni-Ravindra Acharya/14Dec2010/Added/IR-168
    MrsWSATSelectionRead(f_mrs_default_value) ;

    return TRUE;
}

//****************************Method Header************************************
//Method Name   : SetPrescan
//Author        : PATNI
//Purpose       :
//*****************************************************************************
void CPqmMRSScan::SetPrescan(
    CScanTransaction* f_prescan
)
{
    m_prescan = (CPreScan*)f_prescan;
}

//****************************Method Header************************************
//Method Name   : SetScanKind
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int CPqmMRSScan::SetScanKind(
    SMRSScan_t* f_mrs_kind
)
{
    // Local shimming will be always performed
    //+Patni-Ravindra Acharya/22Nov2010/Added/IR-168

    if (f_mrs_kind->shim) {
        globalP->mrsStateP->local = MRS_STATE_WAIT;

    } else {
        globalP->mrsStateP->local = MRS_STATE_SKIP;
    }

    if (f_mrs_kind->f0) {
        if (f_mrs_kind->spec_graph) {
            globalP->mrsStateP->f0 = VFC_PRESCAN_MRS_SHOW_GRAPH;//Patni-Rajeesh/25Jan2011/Modified/IR-168-part2
            f_mrs_kind->f0 = VFC_PRESCAN_MRS_SHOW_GRAPH;//Patni-Rajeesh/25Jan2011/Modified/IR-168-part2

        } else {
            globalP->mrsStateP->f0 = MRS_STATE_WAIT;
        }

        //-Patni-Ravindra Acharya/22Nov2010/Added/IR-168

    } else {
        globalP->mrsStateP->f0 = MRS_STATE_SKIP;
    }

    if (f_mrs_kind->ref) {
        globalP->mrsStateP->ref = MRS_STATE_WAIT;

    } else {
        globalP->mrsStateP->ref = MRS_STATE_SKIP;
    }

    if (f_mrs_kind->wsat) {
        globalP->mrsStateP->wsat = MRS_STATE_WAIT;

    } else {
        globalP->mrsStateP->wsat = MRS_STATE_SKIP;
    }

    if (NULL == m_scan_procedure) {
        //Patni-AMT/2010Jan05/Modified/DeFT# Review cmt FN_MRS-83
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, _T("CPqmMRSScan::SetScanKind"),
                  _T("m_scan_procedure is NULL"));
        return FALSE;
    }

    if (m_scan_procedure->GetAcqManager()->GetPqm()->

        GetStudy()->SetMRSExecutation(m_scan_procedure->GetScanProcProtocol()
                                      ->GetProtocol(), f_mrs_kind->f0, f_mrs_kind->ref,
                                      f_mrs_kind->wsat) != E_NO_ERROR) {

        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
            CPqmUtilities::GetMultiLingualString(_T("IDS_WRITE_MRS_SCAN_TO_STUDY_FILE_ERROR")),
            MPlus::EH::Crucial, _T("pqmMrsScan.cpp"), _T("PQM"));

        m_scan_procedure->GetAcqManager()->GetPqm()->GetStudy()->SetAcqStatus(m_scan_procedure->GetScanProcProtocol(), C_PR_STATUS_WAIT);
        //+Patni-HAR/2009April09/Added/MRSCSI Scan Fix
        //m_scan_procedure->GetAcqManager()->GetPqm()->WriteToUI(PQM_MSG_ABORT_WAIT);
        //+Patni-HAR/2009April09/Added/MRSCSI Scan Fix
        return  FALSE;
    }

    int status = TRUE;

    //execuet Local Shimming
    //+Patni-Ravindra Acharya/14Dec2010/Modified/IR-168
    if (MRS_STATE_WAIT == globalP->mrsStateP->local) {
        //-Patni-Ravindra Acharya/14Dec2010/Modified/IR-168
        // Send msg to GUI Local shimming is begin
        m_scan_procedure->GetAcqManager()->GetPqm()->WriteToUI(PQM_MSG_MRS_LOCALSHIM_BEGIN);

        if ((status = m_localshimming.ShimLocalRun(globalP)) != E_NO_ERROR) {
            if (status == SHIM_AREA_ERROR) {
                //+Patni-HAR/2009April09/Added/MRSCSI Scan Fix
                MrsPopErrorDialog(3, IDS_OP_ERROR_MRS_ROI, _T("IDS_OP_ERROR_MRS_ROI"));
                return FALSE;
                //-Patni-HAR/2009April09/Added/MRSCSI Scan Fix

            } else {
                m_localshimming.ShimQuit(SHIM_FATAL_ERROR);
                return FALSE;
            }

        } else {
            // Do post process of Local shimming
            LocalShimDone();
            //Patni-Ravindra Acharya/03Jan2011/Added/IR-168
            m_scan_procedure->GetAcqManager()->GetPqm()->WriteToUI(PQM_MSG_MRS_LOCALSHIM_DONE); // message to PQM GUI to update MRS UI
        }

    } else {
        status = m_localshimming.CalcShimandHRShimArea(globalP);

        if (E_NO_ERROR == status) {
            LocalShimDone();

        } else if (status == SHIM_AREA_ERROR) {
            //		if (status == SHIM_AREA_ERROR) {
            MrsPopErrorDialog(3, IDS_OP_ERROR_MRS_ROI, _T("IDS_OP_ERROR_MRS_ROI"));
            return FALSE;

        } else {
            m_localshimming.ShimQuit(SHIM_FATAL_ERROR);
            return FALSE;
        }
    }

    //-Patni-Ravindra Acharya/14Dec2010/Modified/IR-168
    return TRUE;
}

//****************************Method Header************************************
//Method Name   : ShimSetupScratchPad
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int CPqmMRSScan::ShimSetupScratchPad(
    char* f_study,
    char* f_scratchpad,
    const int f_index,
    VfPathElem_t* f_shimpath
)
{
    LPCTSTR FUNC_NAME = _T("CPqmMRSScan::ShimSetupScratchPad");

    CVarFieldHandle src_handle(f_study);

    if (src_handle.VuiOpen() != E_NO_ERROR) {
        return E_ERROR;
    }

    CVarFieldHandle dst_handle(f_scratchpad);

    if (dst_handle.VuiOpen() != E_NO_ERROR) {
        return E_ERROR;
    }

    //SHIMINGUPASU is SVN_SD_PROCESSED
    VfPathElem_t	scratchPath[SVD_ST_PROTOCOL] = {0};
    VfPathElem_t	studyPath[SVD_ST_PROTOCOL] = {0};	//Patni-MJC/2009Aug17/Modified/cpp test corrections

    memset(scratchPath, 0, sizeof(VfPathElem_t) * SVD_ST_PROTOCOL);	//Patni-MJC/2009Aug17/Modified/cpp test corrections
    memset(studyPath, 0, sizeof(VfPathElem_t) * SVD_ST_PROTOCOL);	//Patni-MJC/2009Aug17/Modified/cpp test corrections
    int shimPathSize = sizeof(VfPathElem_t) * SVD_ST_PROCESSED;

    /* The index is 0 ScratchPad protocol has been fixed.
    */
    scratchPath[0].subtree_name = SVN_ST_PROTOCOL;
    scratchPath[0].index = 0;
    studyPath[0].subtree_name = SVN_ST_PROTOCOL;
    studyPath[0].index = f_index;

    /* SHIMUPASU new to replace.
    */
    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    VfFieldSpec_t	vfReq[8] = {0};
    memset(vfReq, 0, sizeof(VfFieldSpec_t) * 8);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    int  i = 0, n = 0;
    VFF_SET_ARGS(vfReq[n], SVN_SHIM_PATH, SVT_SHIM_PATH,
                 shimPathSize, f_shimpath, VFO_REPLACE);
    n++;

    //+Patni-SS/2009Apr01/PSP1#VFLOCK
    CString l_log_msg = _T("");
    l_log_msg.Format(_T("SVN_SHIM_PATH=%d"), f_shimpath);
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_log_msg);
    //-Patni-SS/2009Apr01/PSP1#VFLOCK

    int num = 0;
    time_t		tm = 0L;
    status_t vfStat = src_handle.PutFields(studyPath, SVD_ST_PROTOCOL, vfReq, n, &num, &tm);

    if (vfStat != E_NO_ERROR) {
        goto vfError;
    }

    /* SHIMINGUDETA acquisition
    */
    for (n = 0; n < sizeof(shimTbl) / sizeof(*shimTbl); n++) {
        VFF_SET_ARGS(vfReq[n], shimTbl[n].name, shimTbl[n].type,
                     0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);
    }

    vfStat = src_handle.GetFields(f_shimpath, SVD_ST_PROCESSED, vfReq, n, &num);

    if (vfStat != E_NO_ERROR) {
        goto vfError;
    }

    for (n = 0; n < sizeof(shimTbl) / sizeof(*shimTbl); n++) {
        if (vfReq[n].status == VFE_NO_ERROR) {
            shimTbl[n].shim = vfReq[n].buffer;
            shimTbl[n].shimSize = vfReq[n].size;
        }
    }

    /* add a protocol to specify the index.
    * Usually SHIMINGU: 1
    * HR SHIMINGU: 2
    * ? PUROSESUDONODO create a fixed 0.
    */
    f_shimpath[0].index = SHIM_PROCESSED_INDEX;
    f_shimpath[1].index = 0;

    /* ScratchPad nodes already have a protocol to AddBranch
    * To add a node.
    */
    vfStat = dst_handle.AddBranch(f_shimpath, SVD_ST_PROTOCOL);

    if (vfStat != E_NO_ERROR) goto vfError;

    /* PUROSESUDONODO to create.
    */
    vfStat = dst_handle.CreateSubtree(f_shimpath, SVD_ST_PROTOCOL,
                                      SVN_ST_PROCESSED, 1);

    if (vfStat != E_NO_ERROR) {
        goto vfError;
    }

    /* SHIMUPASU new to replace.
    */
    n = 0;
    VFF_SET_ARGS(vfReq[n], SVN_SHIM_PATH, SVT_SHIM_PATH,
                 shimPathSize, f_shimpath, VFO_REPLACE);
    n++;

    //+Patni-SS/2009Apr01/PSP1#VFLOCK
    l_log_msg.Format(_T("SVN_SHIM_PATH=%d"), f_shimpath);
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_log_msg);
    //-Patni-SS/2009Apr01/PSP1#VFLOCK
    vfStat = dst_handle.PutFields(scratchPath, SVD_ST_PROTOCOL, vfReq, n, &num, &tm);

    if (vfStat != E_NO_ERROR) {
        goto vfError;
    }

    for (i = n = 0; i < sizeof(shimTbl) / sizeof(*shimTbl); i++) {
        if (shimTbl[i].shim != NULL) {
            VFF_SET_ARGS(vfReq[n], shimTbl[i].name, shimTbl[i].type,
                         shimTbl[i].shimSize, shimTbl[i].shim,
                         VFO_REPLACE);
            n++;
        }
    }

    if (n > 0) {
        //+Patni-SS/2009Apr01/PSP1#VFLOCK
        CString l_log_msg = _T("");
        l_log_msg.Format(_T("SVN_SHIM_PATH=%d"), f_shimpath);
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_log_msg);
        //-Patni-SS/2009Apr01/PSP1#VFLOCK
        vfStat = dst_handle.PutFields(f_shimpath, SVD_ST_PROCESSED,
                                      vfReq, n, &num, &tm);

        if (vfStat != E_NO_ERROR) {
            goto vfError;
        }
    }

vfError:
    DB_PRINTF("M", ("vfStat = %d\n", vfStat));

    for (n = 0; n < sizeof(shimTbl) / sizeof(*shimTbl); n++) {
        FREE_PTR(shimTbl[n].shim);
    }

    return vfStat;

}

//****************************Method Header************************************
//Method Name   : MrsShimStudyOpen
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int CPqmMRSScan::MrsShimStudyOpen()
{
    DEL_PTR(globalP->shimStudyP->vfHandle);
    globalP->shimStudyP->vfHandle = new CVarFieldHandle(globalP->shimStudyP->study);
    status_t status = globalP->shimStudyP->vfHandle->Open();

    if (status != E_NO_ERROR) {
        return E_ERROR;
    }

    return E_NO_ERROR;
}

//****************************Method Header************************************
//Method Name   : MrsRefModeRead
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int CPqmMRSScan::MrsRefModeRead(int* f_iref_mode_array)const
{
    //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    char		  buf[256] = {0};

    memset(buf, NULL, sizeof(buf));
    sprintf(buf, "%s/%s", getenv("GP_SHIMDT_DIR"), "ref_mode");

    FILE*	  	fp = fopen(buf, "r");

    if (!fp) {
        return E_ERROR;
    }

    char*	  	p0 = NULL;
    char*	  	token = NULL;
    int		  type = 0;

    while (fgets(buf, 256, fp)) {
        p0 = strrchr(buf, '\n');

        if (p0) {
            *p0 = '\0';
        }

        p0 = strtok(buf, "#");

        if ((p0 == buf) && (strlen(p0) > 1)) {
            token = strtok(p0, " \t");

            if (token) {
                if (!strcmp("PAC", token)) {
                    type = 1;

                } else if (!strcmp("SINGLE", token)) {
                    type = 2;
                }

                else {
                    type = 0;
                }
            }

            token = strtok(NULL, " \t");

            if (!token) {
                type = 0;
            }

            switch (type) {
                case 1:
                    sscanf(token, "%d", f_iref_mode_array);
                    break;

                case 2:
                    sscanf(token, "%d", f_iref_mode_array + 1);
                    break;

                case 0:
                default:
                    break;
            }
        }
    }

    fclose(fp);
    return E_NO_ERROR;
}

//****************************Method Header************************************
//Method Name   : MrsRefInfoRead
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int CPqmMRSScan::MrsRefInfoRead(
)const
{
    //+Patni-MP/UnCommented On 02-DEC-2009 / SEFT /MSA0248-00019/MVC005784
    //Partially uncommented for reading the SVN_EXE_NUM_PAC_SCAN tag from the study file

    //int iRefModeArray[2] = {0}; //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    //int32_t iRefMode = 0; //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections

    //TMSC-Tanoue/2010Jun18/Modified/MVC008345. Use CVarFieldHandle
    CVarFieldHandle vf_handle(globalP->shimStudyP->study);
    status_t l_open_status = vf_handle.Open();

    //Patni-RAJ/ADDED On 10/09/2009
    if (l_open_status != E_NO_ERROR) {
        return E_ERROR;
    }

    VfFieldSpec_t reqFlds[10] = {0}; //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(reqFlds, 0, sizeof(VfFieldSpec_t) * 10);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008

    int n = 0, numDone = 0; //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    int typeFormatDepth = SVD_ST_PROTOCOL;
    int16_t iPacScanFlag = 0;

    VFF_SET_ARGS(reqFlds[n], SVN_EXE_NUM_PAC_SCAN, SVT_EXE_NUM_PAC_SCAN,
                 sizeof(int16_t), &iPacScanFlag, 0);
    n++;
    int32_t iMatrix[3] = {0};
    VFF_SET_ARGS(reqFlds[n], SVN_PLN_MRS_MATRIX, SVT_PLN_MRS_MATRIX,
                 sizeof(int32_t) * 3, iMatrix, 0);
    n++;

    int l_status = vf_handle.GetFields(s_shimImagePath, typeFormatDepth, reqFlds, n, &numDone);

    if ((l_status != E_NO_ERROR) || numDone != n) {
        return E_ERROR;
    }

    if (iPacScanFlag > ON) {
        iPacScanFlag = ON;

    } else if (iPacScanFlag == ON) {
        iPacScanFlag = OFF;
    }

    if (iPacScanFlag == ON) {
        globalP->mrsInfoP->PacFlag = ON;

    } else if (iPacScanFlag == OFF) { /* No PAC */
        globalP->mrsInfoP->PacFlag = OFF;

    }

    //+Patni-MP/UnCommented On 02-DEC-2009 / SEFT /MSA0248-00019/MVC005784
    return E_NO_ERROR;
}

//****************************Method Header************************************
//Method Name   : MrsWsatInfoRead
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int CPqmMRSScan::MrsWsatInfoRead(
)const
{
    //+Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    char		  buf[256] = {0};
    //-Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections

    memset(buf, NULL, sizeof(buf));
    sprintf(buf, "%s/%s", getenv("GP_SHIMDT_DIR"), "watersat_data");

    FILE*	  	fp = fopen(buf, "r");

    if (!fp) {
        return E_ERROR;
    }

    char*	  	p0 = NULL;
    char*	  	token = NULL;
    int		  type = 0;

    while (fgets(buf, 256, fp)) {
        p0 = strrchr(buf, '\n');

        if (p0) {
            *p0 = '\0';
        }

        p0 = strtok(buf, "#");

        if ((p0 == buf) && (strlen(p0) > 1)) {
            token = strtok(p0, " \t");

            if (token) {
                if (!strcmp("num_point", token)) {
                    type = 1;

                } else if (!strcmp("start_angle", token)) {
                    type = 2;

                } else if (!strcmp("step_angle", token)) {
                    type = 3;

                } else if (!strcmp("minVolume_watersat", token)) {
                    type = 4;

                } else if (!strcmp("F0c", token)) {
                    type = 5;

                } else if (!strcmp("d_F0c", token)) {
                    type = 6;

                } else if (!strcmp("show_graph", token)) {
                    type = 7;

                } else if (!strcmp("show_spect", token)) {
                    type = 8;

                } else if (!strcmp("pln_nex", token)) {
                    type = 9;

                } else if (!strcmp("DC_point", token)) {
                    type = 10;

                } else if (!strcmp("dummy_shot", token)) {
                    type = 11;

                } else {
                    type = 0;
                }
            }

            token = strtok(NULL, " \t");

            if (!token) {
                type = 0;
            }

            switch (type) {
                case 1:
                    sscanf(token, "%d", &globalP->wsatInfoP->num_point);
                    break;

                case 2:
                    sscanf(token, "%d", &globalP->wsatInfoP->start_angle);
                    break;

                case 3:
                    sscanf(token, "%d", &globalP->wsatInfoP->step_angle);
                    break;

                case 4:
                    sscanf(token, "%f", &globalP->wsatInfoP->minVolume);
                    break;

                case 5:
                    sscanf(token, "%f", &globalP->wsatInfoP->F0c);
                    break;

                case 6:
                    sscanf(token, "%f", &globalP->wsatInfoP->d_F0c);
                    break;

                case 7:
                    sscanf(token, "%d", &globalP->wsatInfoP->show_graph);
                    break;

                case 8:
                    sscanf(token, "%d", &globalP->wsatInfoP->show_spect);
                    break;

                case 9:
                    sscanf(token, "%f", &globalP->wsatInfoP->pln_nex);
                    break;

                case 10:
                    sscanf(token, "%d", &globalP->wsatInfoP->DC_point);
                    break;

                case 11:
                    sscanf(token, "%d", &globalP->wsatInfoP->dummy_shot);
                    break;

                case 0:
                default:
                    break;
            }

        }
    }

    fclose(fp);

    return E_NO_ERROR;
}

//****************************Method Header************************************
//Method Name   : MrsLocalShimInfoRead
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int CPqmMRSScan::MrsLocalShimInfoRead(
)
{
    LPCTSTR FUNC_NAME = _T("CPqmMRSScan::MrsLocalShimInfoRead");
    //+Patni-RSG/Added On 11/11/2009 / SEFT / IR116
    CPQMXMLParser* l_mrsshimconfig_parser = new  CPQMXMLParser();
    CPTString file_name;
    wchar_t buffer[MAX_PATH] = {0};
    ::GetEnvironmentVariable(_T("MRMP_Config"), buffer, MAX_PATH);
    file_name  = buffer;
    file_name += L"\\MRS\\";
    file_name += MRS_SHIM_CONFIG_FILE_NAME;
    bool l_val_set = false;

    //+Patni-MP/Added On 24/11/2009 / SEFT / IR116
    if (globalP == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                  _T("globalP structure is NULL"));
        return E_ERROR;
    }

    if (globalP->shimInfoP == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                  _T("globalP->shimInfoP structure is NULL"));
        return E_ERROR;
    }

    //-Patni-MP/Added On 24/11/2009 / SEFT / IR116
    // load xml file
    //+Patni-RSG/Added On 20/11/2009 / SEFT / IR116

    bool flag = false;

    try {
        if (l_mrsshimconfig_parser->LoadXMLFile(file_name)) {
            flag = true;
            CPTString shimStatestr;
            CString l_trace_string = _T("");
            long l_local_shim_val = 0;

            for (int l_node_pos = 0; l_node_pos <= l_mrsshimconfig_parser->GetNodeCnt(); l_node_pos++) {
                int node_pos = l_mrsshimconfig_parser->GetTagPosition(l_node_pos, LOCAL_SHIM, CHILD_TAG_NAME_lOCAL_SHIM_LIMIT, ATTRIBUTE_NAME_1);

                if (node_pos != -1) {
                    shimStatestr.Empty();
                    shimStatestr = l_mrsshimconfig_parser->GetAttributeValue(l_node_pos, LOCAL_SHIM, CHILD_TAG_NAME_lOCAL_SHIM_LIMIT, ATTRIBUTE_NAME_1);

                    if (!shimStatestr.IsEmpty()) {

                        l_local_shim_val = 0;
                        l_local_shim_val = l_mrsshimconfig_parser->ReadLongValue(shimStatestr);
                        //+Patni-RSG/Added On 20/11/2009 / SEFT / IR116

                        l_trace_string.Format(_T("%d"), l_local_shim_val);
                        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                                  _T("Read MRSShimConfig.xml file Local Shim Limit Value found : ") + l_trace_string);
                        //+Patni-RSG/Added On 20/11/2009 / SEFT / IR116
                        globalP->shimInfoP->localShimLimit = l_local_shim_val;
                        l_val_set = true;

                    } else {
                        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                                  _T("Local Shim Limit Value Not Found in MRSShimConfig.xml"));
                    }

                    break;
                }
            }

        }

    } catch (...) {

        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                  _T("Exception Occurred :Failed to Read MRSShimConfig.xml file"));
    }

    if (flag == false) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                  _T("Failed to Read MRSShimConfig.xml file"));
        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
            CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_XMLFILE_LOADING_FAILED")),
            MPlus::EH::Information, _T("PqmMRSScan.cpp"), _T("PQM"));
        return E_ERROR;
    }

    //+Patni-RSG/Added On 20/11/2009 / SEFT / IR116
    //+Patni-MP/Modified On 24/11/2009 / SEFT / IR116
    if ((!l_val_set) || (globalP->shimInfoP->localShimLimit <= 0)) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                  _T("Setting local Shim Default value 500"));

        globalP->shimInfoP->localShimLimit = 500;
    }

    DEL_PTR(l_mrsshimconfig_parser);

    return E_NO_ERROR;
    //-Patni-RSG/Added On 11/11/2009 / SEFT / IR116
}

//****************************Method Header************************************
//Method Name   : ShimGlobalInit
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int CPqmMRSScan::ShimGlobalInit(
    char* f_study_file,
    char* f_scratchpad,
    const int f_protocol_index
)
{
    DEL_PTR(globalP);

    globalP = new shimGlobal_t();

    memset(globalP, NULL, sizeof(shimGlobal_t));

    if (!globalP) {

        return E_ERROR;
    }

    globalP->shimStudyP = &s_shimStudy;
    globalP->shimStudyP->nodes = s_shimImagePath;
    globalP->shimStudyP->imageNodes = s_shimEchoPath;
    globalP->shimStudyP->hrShimNodes = s_hr_shimPath;
    globalP->shimStudyP->shimNodes = s_shimPath;
    globalP->shimStudyP->presatNodes = s_presatPath;
    globalP->shimInfoP = &s_shimInfo;
    globalP->mrsInfoP = &s_mrsInfo;
    globalP->hr_shimInfoP = &s_hr_shimInfo;
    globalP->wsatInfoP = &s_wsatInfo;
    globalP->refInfoP = &s_refInfo;
    globalP->f0InfoP = &s_f0Info;
    globalP->mrsStateP = &s_mrsState;
    globalP->mrsSelectedP = &s_mrsSelected;
    globalP->shimComm.shimAcqRequest_id = -1;
    globalP->shimComm.wsatAcqRequest_id = -1;
    globalP->posiDataP = &s_position;

    //shimInfoInit();
    globalP->shimStudyP->shimInfo_mode = 0; /* Auto */

    globalP->shimInfoP->calc_processed = -1;
    globalP->shimInfoP->adjust_processed = -1;
    globalP->shimInfoP->totalSlice = -1;

    memset(globalP->tmp, NULL, sizeof(globalP->tmp));
    //TlGetTmpPath(globalP->tmp); // we have to add temp path in  globalP->tmp

    // Initialize study,scratch file and protocol number in to global struct
    globalP->shimStudyP->study = f_study_file;
    globalP->shimStudyP->scratchPad = f_scratchpad;
    globalP->shimStudyP->protocol = f_protocol_index;
    //////////////////////////////////////////////////////////////////////////


    globalP->shimStudyP->nodes[SVD_ST_PROTOCOL - 1].index
    = globalP->shimStudyP->protocol;
    globalP->shimStudyP->imageNodes[SVD_ST_PROTOCOL - 1].index
    = globalP->shimStudyP->protocol;
    globalP->shimInfoP->adjust_protocol = globalP->shimStudyP->protocol;

    /* local shim limit をセットする */
    if (MrsLocalShimInfoRead() != E_NO_ERROR) {
        return E_ERROR;
    }

    /* globalP->wsatInfoP をセットする */

    //+Patni-RSG/Commented On 11/11/2009 / SEFT / IR116
    //if(MrsWsatInfoRead() != E_NO_ERROR){
    //  return E_ERROR;
    //}
    //+Patni-RSG/Commented On 11/11/2009 / SEFT / IR116


    /* 収集に必要なパラメータ */
    globalP->wsatInfoP->seq_link_rcvrgain = strdup("linkE1WsaRG");
    globalP->wsatInfoP->seq_link_image = strdup("linkE1Wsa");

    /* globalP->refInfoP をセットする */

    //+Patni-MP/UnCommented On 02-DEC-2009 / SEFT /MSA0248-00019/MVC005784
    //The logic for reading file data from gp is commented
    //partially uncommented for reading the PAC flag from the study file
    if (MrsRefInfoRead() != E_NO_ERROR) {
        return E_ERROR;
    }

    //+Patni-MP/UnCommented On 02-DEC-2009 / SEFT /MSA0248-00019/MVC005784

    /* globalP->mrsStateP をセットする。
       とりあえず全部やるようにセット */
    globalP->mrsStateP->local = MRS_STATE_WAIT;
    globalP->mrsStateP->f0 = MRS_STATE_WAIT;
    globalP->mrsStateP->spec_graph = MRS_STATE_SKIP ;			//Patni-Hemant/03May2011/Modified/IR-168_Reopen
    globalP->mrsStateP->ref = MRS_STATE_WAIT;
    globalP->mrsStateP->wsat = MRS_STATE_WAIT;

    return E_NO_ERROR;
}

//****************************Method Header************************************
//Method Name   : PrepareRequest
//Author        : PATNI
//Purpose       :
//*****************************************************************************
bool CPqmMRSScan::PrepareRequest()
{
    LPCTSTR FUNC_NAME = _T("CPqmMRSScan::PrepareRequest");
    PQM_SCOPED_TRACE();

    //Patni-RSG/2009Nov11/Modified/MSA0248-00005
    //CScanProcedure* scan_procedure = GetScanProcedure();
    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_scan_procedure) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("m_scan_procedure is NULL"));
        return false;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    CPqmProtocol* curr_protocol = m_scan_procedure->GetScanProcProtocol();

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == curr_protocol) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("curr_protocol is NULL"));
        return false;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    memset(&m_header, 0, sizeof(AM_ClientMessageHeader_t));
    memset(&m_data, 0, sizeof(AM_ClientToAcqMan_unicode_t)); 		// - Patni - HAR / 2009 Mar 06 / Added / MRS


    m_header.function = AMCMM_MRS_PRESCAN;

    CString l_study = m_scan_procedure->GetAcqManager()->GetPqm()->GetStudyUidPath();

    bool l_result = TRUE;

    if (l_study.GetLength() <= 0) {
        //+Patni-PJS/2009May10/Modify/IR-82 review comment
        CString string = _T("");
        string.LoadString(IDS_STUDY_NOT_FOUND);
        CPQMLogMgr::GetInstance()->WriteEventLog(string,
                ERROR_LOG_CRUCIAL1 , _T("PreScan.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82
        /*
        CPQMLogMgr::GetInstance()->WriteEventLog(IDS_STUDY_NOT_FOUND,
                ERROR_LOG_CRUCIAL1 , _T("PreScan.cpp"));
        */
        //-Patni-PJS/2009May10/Modify/IR-82 review comment
        return false;

    }

    //+Patni-RSG/2009Nov06/Commented/MSA0248-00005
    //	m_scan_procedure->GetAcqManager()->GetPqm()->GetStudy()->SetAcqStatus(curr_protocol,C_PR_STATUS_WAIT);
    //-Patni-RSG/2009Nov06/Commented/MSA0248-00005
    _tcscpy(m_data.study_file, l_study);

    m_data.protocol_node = curr_protocol->GetProtocol();

    if (m_data.protocol_node < 0) {
        m_data.protocol_node = m_scan_procedure->GetProt();
    }

    mbstowcs(m_data.param.SCPath.SctrachPad_file, globalP->shimStudyP->scratchPad, strlen(globalP->shimStudyP->scratchPad));

    // Trace log study file and scratchpad file name
    CString str(_T(""));
    str.Format(L"Function : %s \n Study File : [%s] \n Scratchpad file :[%s]",
               m_scan_procedure->GetAcqManager()->GetStr(m_header.function),
               m_data.study_file, m_data.param.SCPath.SctrachPad_file);

    PQM_TRACE1(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, str);

    if (!SendMessage()) {

        PQM_TRACE1(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("SendMessage() Failed"));
        l_result = false;
    }

    /*	m_scan_procedure->GetAcqManager()->GetPqm()->WriteToUI(PQM_MSG_MRS_F0_BEGIN);
    	Sleep(1000);
    	m_scan_procedure->GetAcqManager()->GetPqm()->WriteToUI(PQM_MSG_MRS_F0_DONE);
    	Sleep(1000);
    	m_scan_procedure->GetAcqManager()->GetPqm()->WriteToUI(PQM_MSG_MRS_REF_BEGIN);
    	Sleep(1000);
    	m_scan_procedure->GetAcqManager()->GetPqm()->WriteToUI(PQM_MSG_MRS_REF_DONE);
    	Sleep(1000);
    	m_scan_procedure->GetAcqManager()->GetPqm()->WriteToUI(PQM_MSG_MRS_WSAT_BEGIN);
    	Sleep(1000);
    	m_scan_procedure->GetAcqManager()->GetPqm()->WriteToUI(PQM_MSG_MRS_WSAT_DONE);
    	Sleep(1000);
    	m_scan_procedure->GetAcqManager()->GetPqm()->WriteToUI(PQM_MSG_MRS_PRESCAN_DONE);
    */
    return l_result;
}


//****************************Method Header************************************
//Method Name   : ShimSetupF0ScratchPad
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int CPqmMRSScan::ShimSetupF0ScratchPad(
)
{
    VfPathElem_t path[SVD_ST_PROCESSED] = {0}; //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections

    path[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    path[SVD_ST_PROTOCOL - 1].index = globalP->shimStudyP->hrShimNodes[SVD_ST_PROTOCOL - 1].index;
    path[SVD_ST_PROCESSED - 1].subtree_name = SVN_ST_PROCESSED;
    path[SVD_ST_PROCESSED - 1].index = globalP->shimInfoP->hr_shim_processed;

    globalP->shimStudyP->hrShimNodes[SVD_ST_PROCESSED - 1].index =
        globalP->shimInfoP->hr_shim_processed;

    return ShimSetupScratchPad(globalP->shimStudyP->study,
                               globalP->shimStudyP->scratchPad,
                               globalP->shimStudyP->protocol,
                               path);
}

//****************************Method Header************************************
//Method Name   : ShimHrInfoRead
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int CPqmMRSScan::ShimHrInfoRead()const
{
    /* SHIMING high-resolution information to load  */
    shimStudy_t*		l_study_p  = globalP->shimStudyP;
    VfPathElem_t*      	l_image_path_p  = l_study_p ->nodes;
    VfPathElem_t*       l_hr_shim_path_p  = l_study_p ->hrShimNodes;
    hr_shimInfo_t*      l_hr_info_p = globalP->hr_shimInfoP;

    l_image_path_p [SVD_ST_PROTOCOL - 1].index = l_hr_shim_path_p [SVD_ST_PROTOCOL - 1].index;
    l_image_path_p [SVD_ST_SUBPROTOCOL - 1].index = 0;
    l_image_path_p [SVD_ST_SLICEGROUP - 1].index = 0;
    l_image_path_p [SVD_ST_SLICE - 1].index = 0;
    l_image_path_p [SVD_ST_IMAGE - 1].index = 0;

    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    VfFieldSpec_t	l_req_flds [8] = {0}; //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    memset(l_req_flds, 0, sizeof(VfFieldSpec_t) * 8);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    int			l_n = 0, type_format_depth = -1;
    type_format_depth = SVD_ST_PROTOCOL; /* Protocol Node Data FOV & l_total Slice Read */
    VFF_SET_ARGS(l_req_flds [l_n], SVN_PLN_FOV, SVT_PLN_FOV,
                 sizeof(f32vec2_t), &l_hr_info_p->fovVec, 0);
    l_n++;
    VFF_SET_ARGS(l_req_flds [l_n], SVN_PLN_TOTAL_SLICES, SVT_PLN_TOTAL_SLICES,
                 sizeof(int32_t), &l_hr_info_p->sliceNum, 0);
    l_n++;
    VFF_SET_ARGS(l_req_flds [l_n], SVN_PLN_SELECTED_SLICE_GAP, SVT_PLN_SELECTED_SLICE_GAP,
                 sizeof(flt32_t), &l_hr_info_p->sliceGap, 0);
    l_n++;
    VFF_SET_ARGS(l_req_flds [l_n], SVN_SEQ_ACQ_MATRIX, SVT_SEQ_ACQ_MATRIX,
                 sizeof(f32vec2_t), &l_hr_info_p->acq_matrix, 0);
    l_n++;

    int			l_num_done = -1;
    CVarFieldHandle* const vf_handle = globalP->shimStudyP->vfHandle;
    int l_status = vf_handle->GetFields(l_image_path_p , type_format_depth, l_req_flds ,
                                        l_n, &l_num_done);

    if (l_status != E_NO_ERROR ||
        (l_num_done != l_n)) {
        return E_ERROR;
    }

    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(l_req_flds, 0, sizeof(VfFieldSpec_t) * 8);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    l_n = 0;
    type_format_depth = SVD_ST_SLICEGROUP;
    VFF_SET_ARGS(l_req_flds [l_n], SVN_PLN_OFFSET_VECTOR, SVT_PLN_OFFSET_VECTOR,
                 sizeof(f32vec3_t), &l_hr_info_p->offsetVec, 0);
    l_n++;
    VFF_SET_ARGS(l_req_flds [l_n], SVN_PLN_SLICE_ORI_VECTOR, SVT_PLN_SLICE_ORI_VECTOR,
                 sizeof(f32vec3_t), &l_hr_info_p->sliceVec , 0);
    l_n++;
    VFF_SET_ARGS(l_req_flds [l_n], SVN_PLN_THICKNESS, SVT_PLN_THICKNESS,
                 sizeof(flt32_t), &l_hr_info_p->thickness, 0);
    l_n++;
    VFF_SET_ARGS(l_req_flds [l_n], SVN_PLN_PHASE_ORI_VECTOR, SVT_PLN_PHASE_ORI_VECTOR,
                 sizeof(f32vec3_t), &l_hr_info_p->peVec, 0);
    l_n++;

    l_status = vf_handle->GetFields(l_image_path_p , type_format_depth, l_req_flds ,
                                    l_n, &l_num_done);

    if (l_status != E_NO_ERROR ||
        (l_num_done != l_n)) {
        return E_ERROR;
    }

    return E_NO_ERROR;
}


//****************************Method Header************************************
//Method Name   : MrsCheckVoxelMatrixInside
//Author        : PATNI
//Purpose       :
//*****************************************************************************
MrsSliceCkeckRes_e CPqmMRSScan::MrsCheckVoxelMatrixInside()const
{
    MrsSliceCkeckRes_e l_result = MrsSliceInside; // Removal of warning@AMT

    //+Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections

    BITFLD_DECL(mask,   MAX_BITFLD) = {0};

    VfFieldSpec_t       l_req_flds[12] = {0};
    //-Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    f32vec3_t           l_par_offset_vec;
    f32vec3_t           l_par_slice_vec;
    flt32_t             l_tmp = 0.0f, l_max = 0.0f, l_min = 0.0f; //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    VfPathElem_t        l_par_path[4] = {0};

    if (globalP->mrsInfoP->matrixSO == 1) {
        return MrsSliceInside;
    }

    float l_slice_vec[3] = {0.0f};
    float l_voxel_offset_vec[3] = {0.0f};
    l_slice_vec [0] = (float)globalP->mrsInfoP->sliceVec.x;
    l_slice_vec [1] = (float)globalP->mrsInfoP->sliceVec .y;
    l_slice_vec [2] = (float)globalP->mrsInfoP->sliceVec .z;
    l_voxel_offset_vec [0] = (float)globalP->mrsInfoP->voxelOffsetVec.x;
    l_voxel_offset_vec [1] = (float)globalP->mrsInfoP->voxelOffsetVec .y;
    l_voxel_offset_vec [2] = (float)globalP->mrsInfoP->voxelOffsetVec .z;
    float l_voxel_so = (float)globalP->mrsInfoP->voxelSO;

    for (int l_idx = 0; l_idx < 4; l_idx++) {
        l_par_path[l_idx].subtree_name = globalP->mrsInfoP->locImages[l_idx].subtree_name;
        l_par_path[l_idx].index = globalP->mrsInfoP->locImages[l_idx].index;
    }

    int l_active = 0;
    int l_total = MAX_BITFLD;
    BITFLD_INIT(mask, MAX_BITFLD, 0);
    //l_vf_stat = VfScanSubtree(globalP->shimStudyP->vfHandle,
    //l_par_path, SVD_ST_ECHO, SVN_ST_POSITION,
    //mask,&l_total,&l_active);


    CVarFieldHandle* const vf_handle = globalP->shimStudyP->vfHandle;
    status_t l_vf_stat = vf_handle->ScanSubtree(l_par_path, SVD_ST_ECHO, SVN_ST_POSITION,
                         mask, &l_total, &l_active);

    if_error(l_vf_stat) {
        return MrsSliceOutside;
    }
    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(l_req_flds, 0, sizeof(VfFieldSpec_t) * 12);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    int l_num = 0;

    int type_format_depth = SVD_ST_ECHO;
    VFF_SET_ARGS(l_req_flds [l_num], SVN_PLN_OFFSET_VECTOR, SVT_PLN_OFFSET_VECTOR,
                 sizeof(f32vec3_t), &l_par_offset_vec, 0);
    l_num++;
    VFF_SET_ARGS(l_req_flds [l_num], SVN_PLN_SLICE_ORI_VECTOR, SVT_PLN_SLICE_ORI_VECTOR,
                 sizeof(f32vec3_t), &l_par_slice_vec, 0);
    l_num++;

    int l_status = -1;
    int l_num_done = 0;
    l_status = vf_handle->GetFields(l_par_path, type_format_depth, l_req_flds ,
                                    l_num, &l_num_done);

    if (l_status != E_NO_ERROR ||
        l_num_done != l_num) {
        return MrsSliceOutside;
    }


    l_max = l_min = 0.;

    for (l_idx = 0; l_idx < l_total; l_idx++) {
        if (BITFLD_ON(mask, l_idx)) {
            l_par_path[SVD_ST_POSITION - 1].index = l_idx;
            // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
            memset(l_req_flds, 0, sizeof(VfFieldSpec_t) * 12);
            // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
            l_num = 0;
            type_format_depth = SVD_ST_POSITION;
            VFF_SET_ARGS(l_req_flds [l_num], SVN_PRC_SLICE_POSITION, SVT_PRC_SLICE_POSITION,
                         sizeof(flt32_t), &l_tmp, 0);
            l_num++;

            status_t l_status = vf_handle->GetFields(l_par_path, type_format_depth, l_req_flds ,
                                l_num, &l_num_done);

            if (l_status != E_NO_ERROR ||
                l_num_done != l_num) {
                return MrsSliceOutside;
            }

            if (l_idx == 0) l_max = l_min = l_tmp;

            if (l_tmp < l_min) l_min = l_tmp;

            if (l_tmp > l_max) l_max = l_tmp;
        }
    }

    float l_first_par_offset_vec[3] = {0.0f};
    float l_last_par_offset_vec[3] = {0.0f};
    l_first_par_offset_vec [0] = l_par_offset_vec.x + l_par_slice_vec.x * l_min;
    l_first_par_offset_vec [1] = l_par_offset_vec.y + l_par_slice_vec.y * l_min;
    l_first_par_offset_vec [2] = l_par_offset_vec.z + l_par_slice_vec.z * l_min;
    l_last_par_offset_vec [0] = l_par_offset_vec.x + l_par_slice_vec.x * l_max;
    l_last_par_offset_vec [1] = l_par_offset_vec.y + l_par_slice_vec.y * l_max;
    l_last_par_offset_vec [2] = l_par_offset_vec.z + l_par_slice_vec.z * l_max;

    /*I have in the region of interest check
      End  slice vector direction */

    float l_child_offset_vec[3] = {0.0f};
    l_child_offset_vec[0] = (float)(globalP->mrsSelectedP->offsetVec.x +
                                    l_slice_vec [0] * globalP->mrsSelectedP->fovVec.z / 2.0);
    l_child_offset_vec[1] = (float)(globalP->mrsSelectedP->offsetVec.y +
                                    l_slice_vec [1] * globalP->mrsSelectedP->fovVec.z / 2.0);
    l_child_offset_vec[2] = (float)(globalP->mrsSelectedP->offsetVec.z +
                                    l_slice_vec [2] * globalP->mrsSelectedP->fovVec.z / 2.0);

    mrsCeckSliceInside(l_slice_vec ,
                       l_first_par_offset_vec ,
                       l_last_par_offset_vec ,
                       l_child_offset_vec,
                       &l_result);

    if (l_result != MrsSliceInside) return l_result ;

    /* Area of interest I have in check
       slice vector the opposite end */

    l_child_offset_vec[0] = (float)(globalP->mrsSelectedP->offsetVec.x -
                                    l_slice_vec [0] * globalP->mrsSelectedP->fovVec.z / 2.0);
    l_child_offset_vec[1] = (float)(globalP->mrsSelectedP->offsetVec.y -
                                    l_slice_vec [1] * globalP->mrsSelectedP->fovVec.z / 2.0);
    l_child_offset_vec[2] = (float)(globalP->mrsSelectedP->offsetVec.z -
                                    l_slice_vec [2] * globalP->mrsSelectedP->fovVec.z / 2.0);

    mrsCeckSliceInside(l_slice_vec ,
                       l_first_par_offset_vec ,
                       l_last_par_offset_vec ,
                       l_child_offset_vec,
                       &l_result);
    return l_result;
}

//****************************Method Header************************************
//Method Name   : MrsPositionRead
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int CPqmMRSScan::MrsPositionRead(
)const
{
    shimStudy_t*		l_study_p = globalP->shimStudyP;
    VfPathElem_t*      	l_image_path_p = l_study_p->nodes;
    VfPathElem_t*       l_hr_shim_path_p = l_study_p->hrShimNodes;
    VfPathElem_t*       l_shim_path_p = l_study_p->shimNodes;
    VfPathElem_t*       l_presat_path_p = l_study_p->presatNodes;

    mrsInfo_t*		l_pos_p = globalP->mrsInfoP;
    refInfo_t*      l_ref_p = globalP->refInfoP;
    mrsSelected_t*  l_mrs_select_p = globalP->mrsSelectedP;

    l_image_path_p[SVD_ST_SUBPROTOCOL - 1].index = 0;
    l_image_path_p[SVD_ST_SLICEGROUP - 1].index = 0;
    l_image_path_p[SVD_ST_SLICE - 1].index = 0;
    l_image_path_p[SVD_ST_IMAGE - 1].index = 0;

    l_presat_path_p[SVD_ST_PROTOCOL - 1].index = l_image_path_p[SVD_ST_PROTOCOL - 1].index;
    l_presat_path_p[SVD_ST_SUBPROTOCOL - 1].index = 0;
    l_presat_path_p[SVD_ST_PRESAT - 1].index = 0;
    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    VfFieldSpec_t	l_reqFlds[12] = {0};
    memset(l_reqFlds, 0, sizeof(VfFieldSpec_t) * 12);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    int			l_n = 0;
    int l_type_format_depth = SVD_ST_PROTOCOL; /* Protocol Node Data Fov & l_total Slice Read */
    VFF_SET_ARGS(l_reqFlds[l_n], SVN_PLN_FOV, SVT_PLN_FOV,
                 sizeof(f32vec2_t), &l_pos_p->fovVec, 0);
    l_n++;
    VFF_SET_ARGS(l_reqFlds[l_n], SVN_PLN_TOTAL_SLICES,
                 SVT_PLN_TOTAL_SLICES, sizeof(int32_t),
                 &l_pos_p->sliceNum, 0);
    l_n++;
    VFF_SET_ARGS(l_reqFlds[l_n], SVN_PLN_SELECTED_SLICE_GAP,
                 SVT_PLN_SELECTED_SLICE_GAP, sizeof(flt32_t),
                 &l_pos_p->sliceGap, 0);
    l_n++;

    VFF_SET_ARGS(l_reqFlds[l_n], SVN_SEQ_HR_SHIM_PATH,
                 SVT_SEQ_HR_SHIM_PATH, 0,
                 NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);
    l_n++;
    VFF_SET_ARGS(l_reqFlds[l_n], SVN_SEQ_SHIM_PATH,
                 SVT_SEQ_SHIM_PATH, 0,
                 NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);
    l_n++;

    VFF_SET_ARGS(l_reqFlds[l_n], SVN_PLN_HOLD,
                 SVT_PLN_HOLD, sizeof(int32_t),
                 &globalP->shimStudyP->acq_hold_mode, 0);
    l_n++;
    int32_t              l_matrix[3] = {0};
    VFF_SET_ARGS(l_reqFlds[l_n], SVN_PLN_MRS_MATRIX,
                 SVT_PLN_MRS_MATRIX, sizeof(int32_t) * 3,
                 l_matrix, 0);
    l_n++;
    VFF_SET_ARGS(l_reqFlds[l_n], SVN_SEQ_ACQ_MATRIX,
                 SVT_SEQ_ACQ_MATRIX, sizeof(f32vec2_t),
                 &l_ref_p->main_acq_matrix, 0);
    l_n++;
    VFF_SET_ARGS(l_reqFlds[l_n], SVN_PLN_LOCATOR_IMAGES,
                 SVT_PLN_LOCATOR_IMAGES, sizeof(l_pos_p->locImages),
                 l_pos_p->locImages, 0);
    l_n++;
    flt32_t              l_voxel_size[3] = {0};

    VFF_SET_ARGS(l_reqFlds[l_n], SVN_PLN_MRS_VOXCEL,
                 SVT_PLN_MRS_VOXCEL, sizeof(flt32_t) * 3,
                 l_voxel_size, 0);
    l_n++;

    int			l_num_done = 0;
    CVarFieldHandle* const vf_handle = globalP->shimStudyP->vfHandle;
    int l_status = vf_handle->GetFields(l_image_path_p, l_type_format_depth,
                                        l_reqFlds, l_n, &l_num_done);

    if (l_status != E_NO_ERROR || (l_num_done != l_n) ||
        (!l_reqFlds[3].buffer) ||
        (!l_reqFlds[4].buffer)) {

        FREE_PTR(l_reqFlds[3].buffer)

        FREE_PTR(l_reqFlds[4].buffer);

        return E_ERROR;
    }

    memcpy(l_hr_shim_path_p, l_reqFlds[3].buffer, sizeof(VfPathElem_t) * 2);
    FREE_PTR(l_reqFlds[3].buffer);
    memcpy(l_shim_path_p, l_reqFlds[4].buffer, sizeof(VfPathElem_t) * 2);
    FREE_PTR(l_reqFlds[4].buffer);

    l_pos_p->matrixPE = l_matrix[0];
    l_pos_p->matrixRO = l_matrix[1];
    l_pos_p->matrixSO = l_matrix[2];
    l_pos_p->voxelSO = l_voxel_size[2];
    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(l_reqFlds, 0, sizeof(VfFieldSpec_t) * 12);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    l_n = 0;
    l_type_format_depth = SVD_ST_SLICEGROUP;
    VFF_SET_ARGS(l_reqFlds[l_n], SVN_PLN_OFFSET_VECTOR, SVT_PLN_OFFSET_VECTOR,
                 sizeof(f32vec3_t), &l_pos_p->offsetVec, 0);
    l_n++;
    VFF_SET_ARGS(l_reqFlds[l_n], SVN_PLN_SLICE_ORI_VECTOR, SVT_PLN_SLICE_ORI_VECTOR,
                 sizeof(f32vec3_t), &l_pos_p->sliceVec , 0);
    l_n++;
    VFF_SET_ARGS(l_reqFlds[l_n], SVN_PLN_THICKNESS, SVT_PLN_THICKNESS,
                 sizeof(flt32_t), &l_pos_p->thickness, 0);
    l_n++;
    VFF_SET_ARGS(l_reqFlds[l_n], SVN_PLN_PHASE_ORI_VECTOR, SVT_PLN_PHASE_ORI_VECTOR,
                 sizeof(f32vec3_t), &l_pos_p->peVec, 0);
    l_n++;

    l_status = vf_handle->GetFields(l_image_path_p, l_type_format_depth, l_reqFlds,
                                    l_n, &l_num_done);

    if (l_status != E_NO_ERROR ||
        l_num_done != l_n) {
        return E_ERROR;
    }

    l_pos_p->voxelOffsetVec.x = l_pos_p->offsetVec.x;
    l_pos_p->voxelOffsetVec.y = l_pos_p->offsetVec.y;
    l_pos_p->voxelOffsetVec.z = l_pos_p->offsetVec.z;


    if ((l_pos_p->matrixPE != 1) || (l_pos_p->matrixRO != 1)) {
        /* MARUCHIBOKUSERU*/
        // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
        memset(l_reqFlds, 0, sizeof(VfFieldSpec_t) * 12);
        // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
        l_n = 0;
        l_type_format_depth = SVD_ST_PROTOCOL; /* Protocol Node Data */
        VFF_SET_ARGS(l_reqFlds[l_n], SVN_PLN_MRS_SELECTED, SVT_PLN_MRS_SELECTED,
                     sizeof(f32vec3_t), &l_mrs_select_p->fovVec, 0);
        l_n++;

        status_t l_status = vf_handle->GetFields(l_presat_path_p, l_type_format_depth, l_reqFlds, l_n, &l_num_done);

        if (l_status != E_NO_ERROR ||
            l_num_done != l_n) {

            return E_ERROR;
        }

        while (1) {
            int32_t presatType = -1;

            /*Select the ROI excited to find PURISATTONODO */
            // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
            memset(l_reqFlds, 0, sizeof(VfFieldSpec_t) * 12);
            // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
            l_n = 0;
            l_type_format_depth = SVD_ST_PRESAT; /* Presat Node Data */
            VFF_SET_ARGS(l_reqFlds[l_n], SVN_PLN_PRESAT_TYPE, SVT_PLN_PRESAT_TYPE,
                         sizeof(int32_t), &presatType, 0);
            l_n++;
            VFF_SET_ARGS(l_reqFlds[l_n], SVN_PLN_OFFSET_VECTOR, SVT_PLN_OFFSET_VECTOR,
                         sizeof(f32vec3_t), &l_mrs_select_p->offsetVec, 0);
            l_n++;

            status_t l_status = vf_handle->GetFields(l_presat_path_p, l_type_format_depth, l_reqFlds, l_n, &l_num_done);

            if (l_status != E_NO_ERROR ||
                l_num_done != l_n) {

                return E_ERROR;
            }

            if (presatType == VFC_PRESAT_TYPE_MRS_SELECTED) {
                /* MITSUKATTA */

                /* Replacing the value*/
                l_pos_p->offsetVec.x = l_mrs_select_p->offsetVec.x;
                l_pos_p->offsetVec.y = l_mrs_select_p->offsetVec.y;
                l_pos_p->offsetVec.z = l_mrs_select_p->offsetVec.z;
                l_pos_p->fovVec.x = l_mrs_select_p->fovVec.x;
                l_pos_p->fovVec.y = l_mrs_select_p->fovVec.y;
                l_pos_p->thickness = l_mrs_select_p->fovVec.z;
                l_pos_p->sliceNum = 1;
                l_pos_p->sliceGap = 0.0;
                break;

            } else {
                /* Next to see PURISATTONODO */
                l_presat_path_p[SVD_ST_PRESAT - 1].index += 1;
            }
        }
    }

    return E_NO_ERROR;
}

// + Patni - HAR / 2009 Mar 06 / Modified / MRS
void CPqmMRSScan::MrsPopErrorDialog(
    const int type,
    const int f_error_id,
    const CString& f_errmsg
)const
{
    LPCTSTR FUNC_NAME = _T("CPqmMRSScan::MrsPopErrorDialog");

    //+Patni-KSS/2011Feb17/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(type);
    UNREFERENCED_PARAMETER(f_error_id);
    //-Patni-KSS/2011Feb17/Added/CPP Test tool Execution

    CString temp_string = CPqmUtilities::GetMultiLingualString(f_errmsg);

    //+Patni-HAR/2009April09/Added/MRSCSI Scan Fix
    MRERROR_LOG_DATA error_log_data;

    error_log_data.error_message = temp_string;
    //Patni-AMT/2009Dec4/Modified/DeFT# MSA0248-00139-Code Review
    error_log_data.error_source = _T("PQM");



    CPQMLogMgr::GetInstance()->DisplayMessageinErrorDialog(error_log_data, MPlus::EH::Information);
    //-Patni-HAR/2009April09/Added/MRSCSI Scan Fix

    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, temp_string);
}
// - Patni - HAR / 2009 Mar 06 / Modified / MRS

//+Patni-Ravindra Acharya/15Dec2010/Added/IR-168
int CPqmMRSScan::MrsWSATSelectionRead(
    SMRSScan_t* f_mrs_default_value
)const
{
    //Partially uncommented for reading the SVN_PLN_WSAT_MODE tag from the study file
    LPCTSTR FUNC_NAME = _T("CPqmMRSScan::MrsWSATSelectionRead");
    VfFieldSpec_t reqFlds[1] = {0};
    VfPathElem_t search_node_path[] = {
        { SVN_ST_PROTOCOL,      0 },
        { SVN_ST_SUBPROTOCOL,   0 },
        { SVN_ST_SLICEGROUP,    0 },
        { SVN_ST_SLICE,     0 },
        { SVN_ST_IMAGE,     0 },
    };

    search_node_path[SVN_ST_PROTOCOL - 1].index = m_scan_procedure->GetScanProcProtocol()->GetProtocol();

    CVarFieldHandle vf_handle(globalP->shimStudyP->study);

    status_t l_open_status = vf_handle.Open();

    if (l_open_status != E_NO_ERROR) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  CPqmUtilities::GetLibVfErrorStatusString(l_open_status));
        return E_ERROR;
    }

    memset(reqFlds, 0, sizeof(VfFieldSpec_t) * 1);

    int n = 0, numDone = 0;
    int l_Selection = -1, l_status = -1;

    VFF_SET_ARGS(reqFlds[n], SVN_MRS_PRESCAN_WSAT_MODE, SVT_MRS_PRESCAN_WSAT_MODE,	//Patni-Rajeesh/25Jan2011/Modified/IR-168-part2
                 sizeof(int), &l_Selection, 0);
    n++;


    l_status = vf_handle.GetFields(search_node_path, SVD_ST_PROTOCOL, reqFlds, n, &numDone);

    //+Patni-Hemant/03May2011/Modified/IR-168_Reopen + IR-168_Part3
    if (E_NO_ERROR == l_status) {

        f_mrs_default_value->spec_graph_change_allow = TRUE ;
        f_mrs_default_value->spec_graph = CanDisplaySpectrumGraph(m_scan_procedure->GetScanProcProtocol()) ? TRUE : FALSE;

        f_mrs_default_value->shim_change_allow = (f_mrs_default_value->spec_graph > 0) ? FALSE : TRUE;
        f_mrs_default_value->shim = TRUE ;

        switch (l_Selection) {

            case VFC_MRS_PRESCAN_WSAT_MODE_PARTIAL: {

                f_mrs_default_value->f0_change_allow = FALSE ;
                f_mrs_default_value->f0 = TRUE ;
            }
            break ;

            case VFC_MRS_PRESCAN_WSAT_MODE_FULL:
            default : {

                f_mrs_default_value->f0_change_allow = TRUE ;
                f_mrs_default_value->f0 = TRUE ;
            }
            break ;
        }

        return E_NO_ERROR;

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, CPqmUtilities::
                  GetLibVfErrorStatusString(l_status));
        return l_status;
    }

    //-Patni-Hemant/03May2011/Modified/IR-168_Reopen + IR-168_Part3
}
//-Patni-Ravindra Acharya/15Dec2010/Added/IR-168

//+Patni-HAR/2010Apr26/Added/IR-134
//****************************Method Header************************************
//Method Name   : SetEddyAxisExecute
//Author        : PATNI/HAR
//Purpose       :
//*****************************************************************************
void CPqmMRSScan::SetEddyAxisExecute(
)const
{
    if (0 != m_scan_procedure->GetScanProcProtocol()->GetMRSEddyAxis()) {
        globalP->mrsStateP->f0      = VFC_PRESCAN_MRS_EXE;
        globalP->mrsStateP->ref     = VFC_PRESCAN_MRS_EXE;
        globalP->mrsStateP->wsat    = VFC_PRESCAN_MRS_SKIP;
    }
}
//-Patni-HAR/2010Apr26/Added/IR-134

//****************************Method Header************************************
//Method Name   : LocalShimDone
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int CPqmMRSScan::LocalShimDone(
)
{
    LPCTSTR FUNC_NAME = _T("CPqmMRSScan::LocalShimDone");
    PQM_SCOPED_TRACE();

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_scan_procedure) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("m_scan_procedure is NULL"));
        return -1;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    //Patni-Ravindra Acharya/03Jan2011/Commented/IR-168
    //m_scan_procedure->GetAcqManager()->GetPqm()->WriteToUI(PQM_MSG_MRS_LOCALSHIM_DONE); // message to PQM GUI to update MRS UI

    VfPathElem_t    l_shimpath[SVD_ST_PROCESSED] = {0}; //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    l_shimpath[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_shimpath[SVD_ST_PROTOCOL - 1].index = globalP->shimStudyP->hrShimNodes[SVD_ST_PROTOCOL - 1].index;
    l_shimpath[SVD_ST_PROCESSED - 1].subtree_name = SVN_ST_PROCESSED;
    l_shimpath[SVD_ST_PROCESSED - 1].index = globalP->shimInfoP->hr_shim_processed;

    CPqmShimSharedData::GetInstance()->SetHRShimPath(l_shimpath); // to set the shim path
    m_scan_procedure->GetAcqManager()->GetPqm()->GetStudy()->WriteToShimData(); // to write the updated value in shim shared data file.

    /* Scratchpad to add information to SHIMINGU */
    if (ShimSetupF0ScratchPad() != E_NO_ERROR) {

        m_localshimming.ShimQuit(SHIM_FATAL_ERROR);
    }

    // prepare and send MRS request to Acqman
    return PrepareRequest();
}

//****************************Method Header************************************
//Method Name   : ProcessResponse
//Author        : PATNI
//Purpose       :
//*****************************************************************************
void CPqmMRSScan::ProcessResponse(
    AM_ClientMessageHeader_t* f_hdr,
    AM_AcqManToClient_t* f_body
)
{
    LPCTSTR FUNC_NAME = _T("CPqmMRSScan::ProcessResponse");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_scan_procedure) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("m_scan_procedure is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    int type = 0;
    int funcBit = m_scan_procedure->GetAcqManager()->GetFunctionBit(f_hdr->function);

    CString str_msg(_T(""));

    if ((f_body->status == AMCM_SUCCESS) ||
        (f_body->status == AMCM_RECONST)) {

        if (f_hdr->function == AMCMR_MRS_PRESCAN) {

            type = AMCM_FUNC_KIND_REQUEST;
            m_scan_procedure->SetProcessing(m_scan_procedure->GetProcessing() | funcBit);

        } else if ((f_hdr->function == AMCMA_MRS_PRESCAN_DONE)) {

            type = AMCM_FUNC_KIND_ANSWER;

            m_scan_procedure->SetProcessing(m_scan_procedure->GetProcessing() ^ funcBit);
            m_scan_procedure->SetRequest(m_scan_procedure->GetRequest() ^ funcBit);

            m_scan_procedure->GetAcqManager()->GetPqm()->WriteToUI(PQM_MSG_MRS_PRESCAN_DONE);
            // Set prescan as last transaction
            m_scan_procedure->AddTransaction((CScanTransaction*)m_prescan);
            m_prescan->SendMessage(); // Send normal pre scan request to Acqman

        } else {						//STAUS messages and unknown message
            type = AMCM_FUNC_KIND_STATUS;

            if (f_hdr->function == AMCMS_MRS_PRESCAN_F0_BEGIN) {
                m_scan_procedure->GetAcqManager()->GetPqm()->WriteToUI(PQM_MSG_MRS_F0_BEGIN);

            } else if (f_hdr->function == AMCMS_MRS_PRESCAN_F0_END) {
                m_scan_procedure->GetAcqManager()->GetPqm()->WriteToUI(PQM_MSG_MRS_F0_DONE);

            } else if (f_hdr->function == AMCMS_MRS_PRESCAN_REF_BEGIN) {
                m_scan_procedure->GetAcqManager()->GetPqm()->WriteToUI(PQM_MSG_MRS_REF_BEGIN);

            } else if (f_hdr->function == AMCMS_MRS_PRESCAN_REF_END) {
                m_scan_procedure->GetAcqManager()->GetPqm()->WriteToUI(PQM_MSG_MRS_REF_DONE);

            } else if (f_hdr->function == AMCMS_MRS_PRESCAN_WSAT_BEGIN) {
                m_scan_procedure->GetAcqManager()->GetPqm()->WriteToUI(PQM_MSG_MRS_WSAT_BEGIN);

            } else if (f_hdr->function == AMCMS_MRS_PRESCAN_WSAT_END) {
                m_scan_procedure->GetAcqManager()->GetPqm()->WriteToUI(PQM_MSG_MRS_WSAT_DONE);

            } else {
                PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Unknown status Message !"));
            }
        }

        str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                       m_scan_procedure->GetAcqManager()->GetStr(f_body->status),
                       m_scan_procedure->GetAcqManager()->GetStr(type),
                       m_scan_procedure->GetAcqManager()->GetStr(f_hdr->function));

        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, str_msg);

    } else if (f_body->status == AMCM_NACK) {

        if ((f_hdr->function == AMCMR_MRS_PRESCAN)) {

            type = AMCM_FUNC_KIND_REQUEST;

        } else if (f_hdr->function == AMCMA_MRS_PRESCAN_DONE) {

            type = AMCM_FUNC_KIND_ANSWER;

        } else {
            type = AMCM_FUNC_KIND_STATUS;
        }

        m_scan_procedure->SetProcessing(m_scan_procedure->GetProcessing() ^ funcBit);
        m_scan_procedure->SetRequest(m_scan_procedure->GetRequest() ^ funcBit);

        str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                       m_scan_procedure->GetAcqManager()->GetStr(f_body->status),
                       m_scan_procedure->GetAcqManager()->GetStr(type),
                       m_scan_procedure->GetAcqManager()->GetStr(f_hdr->function));

        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, str_msg);

        //+Patni-PJS/2010Dec29/Added/TMSC-REDMINE-1098-Part2

        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
            CPqmUtilities::GetMultiLingualString(_T("IDS_ACQUISITION_MGR_ERROR")),
            MPlus::EH::Information, _T("PqmMRSScan.cpp"), _T("PQM"));
        //-Patni-PJS/2010Dec29/Added/TMSC-REDMINE-1098-Part2
        m_scan_procedure->GetAcqManager()->GetPqm()->GetStudy()->SetAcqStatus(m_scan_procedure->GetScanProcProtocol(), C_PR_STATUS_WAIT);
        m_scan_procedure->GetAcqManager()->GetPqm()->GetStudy()->SetScanModeOfHead();

        m_scan_procedure->GetAcqManager()->GetPqm()->WriteToUI(PQM_MSG_ABORT_WAIT); // L"ABORT_WAIT"

        m_scan_procedure->ResetCurrentRequest();

    } else { // Unknown message

        str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                       m_scan_procedure->GetAcqManager()->GetStr(f_body->status),
                       m_scan_procedure->GetAcqManager()->GetStr(type),
                       m_scan_procedure->GetAcqManager()->GetStr(f_hdr->function));

        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, str_msg);

        m_scan_procedure->GetAcqManager()->GetPqm()->GetStudy()->SetAcqStatus(m_scan_procedure->GetScanProcProtocol(), C_PR_STATUS_WAIT);
        m_scan_procedure->GetAcqManager()->GetPqm()->GetStudy()->SetScanModeOfHead();

        m_scan_procedure->GetAcqManager()->GetPqm()->WriteToUI(PQM_MSG_ABORT_WAIT); // L"ABORT_WAIT"

        m_scan_procedure->ResetCurrentRequest();
    }
}


//+Patni-Hemant/03May2011/Added/IR-168_Part3
bool CPqmMRSScan::CanDisplaySpectrumGraph(
    const CPqmProtocol* f_protocol
) const
{

    LPCTSTR FUNC_NAME = _T("CPqmMRSScan::CanDisplaySpectrumGraph");

    if (NULL == f_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Protocol is NULL"));
        return false ;
    }

    const int l_is_not_eddy_axis_present = 0;

    if (l_is_not_eddy_axis_present != f_protocol->GetMRSEddyAxis()) {
        return false;
    }

    return (VFC_ANATOMY_HEAD != f_protocol->GetPqmPrMode()->GetAnatomy());
}
//-Patni-Hemant/03May2011/Added/IR-168_Part3
//REDMINE-1213_Update_29_APR
//Redmine-1213
//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-774
//Redmine-779
//Redmine-780
//Redmine-781
//Redmine-782
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: acqinfo.cpp
 *  Overview: Implementation of CAcqInfo class.
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
#include "AcqInfo.h"
#include <ErrorDisp/ErrorDispConsts.h>
#include <MPlusI18n/MPlusI18nCatalog.h>  // Added By KT/ACE-2/15-06-2009/Multilingualisation
#include <libStudyManager/Log/ScopedFuncInOutLog.h>
#include <PQM/resource.h>
#include <PQM/PQMLogMgr.h>
#include <PQM/PqmUtilities.h>
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"
#include "PQMIPC.h"
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774

#include "Masterslave.h"
//#include "CoilDB.h"
#include "Coil.h"
#include "Pqm.h"
#include "PqmSpeeder.h"
#include "scanprocedure.h"
#include "pqmpmacqman.h"
#include "PqmCoilData.h"
#include "PqmPrMode.h"
#include "GateMode.h"
#include "pqmconfig.h"
//Patni-DKH/2010Jan25/Added/PH#3#/IR86/DebugWindow
#include "PQMDebugInfo.h"
//+Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
#include "CDSUtilities.h"
#include "pqmCDS.h"
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
//#include "CDSConstants.h"
//-Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
#include "VarFieldHandle.h" //TMSC-Tanoue/2010Aug25/MVC008345
#include "CFShiftCheck.h"
#include "AutoInsertConstant.h"
#include "pqmAMB.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace MPlus::EH;
using namespace MR::ACQ::SM::COMMON::MUTEX;

//***************************Method Header*************************************
//Method Name    : CAcqInfo()
//Author         : PATNI/NJK
//Purpose        : Default constructor
//*****************************************************************************
CAcqInfo::CAcqInfo(
    CScanProcedure* f_scan_proc
) : CScanTransaction(f_scan_proc), m_acqinfo_mode(NULL)
{
    LPCTSTR FUNC_NAME = _T("CAcqInfo::CAcqInfo");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ScanTransaction Created"));
}

//***************************Method Header*************************************
//Method Name    : ~CAcqInfo()
//Author         : PATNI/NJK
//Purpose        : Destructor
//*****************************************************************************
CAcqInfo::~CAcqInfo(
)
{
    LPCTSTR FUNC_NAME = _T("CAcqInfo::~CAcqInfo");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ScanTransaction Destroyed"));
}

//****************************Method Header************************************
//Method Name   :PrepareRequest
//Author        :PATNI/MRP
//Purpose       :This method prepares the Acqinfo request that will be sent to Acqman
//****************************************************************************
bool CAcqInfo::PrepareRequest(
)
{
    //Patni-AMT/2009Sep09/Added/MVC004983 -  Code Review
    CString l_func_name = _T("CAcqInfo::PrepareRequest");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, l_func_name,
              _T("Trace of PrepareRequest"));

    CScanProcedure* scan_proc = GetScanProcedure();

    //+Patni-Manish/2009Sept30/Added/MVC004983/Coil dlg not display
    if (NULL == scan_proc) {
        PQM_TRACE(USER_FUNC_MARKER, l_func_name,
                  _T("CScanProcedure object is NULL"));
        return FALSE;
    }

    //+Patni-HAR/2010Apr13/Added/Phase#3 Code Review
    //CPqmProtocol* protocol = scan_proc->GetScanProcProtocol();

    //if (NULL == protocol) {
    //    PQM_TRACE(USER_FUNC_MARKER, l_func_name,
    //              _T("CPqmProtocol object is NULL"));
    //    return FALSE;
    //}
    //-Patni-HAR/2010Apr13/Added/Phase#3 Code Review

    CPqmPmAcqman* acqman = scan_proc->GetAcqManager();

    if (NULL == acqman) {
        PQM_TRACE(USER_FUNC_MARKER, l_func_name,
                  _T("CPqmPmAcqman object is NULL"));
        return FALSE;
    }

    CPqm* pqm = acqman->GetPqm();

    if (NULL == pqm) {
        PQM_TRACE(USER_FUNC_MARKER, l_func_name,
                  _T("CPqm object is NULL"));
        return FALSE;
    }

    //-Patni-Manish/2009Sept30/Added/MVC004983/Coil dlg not display

    scan_proc->SetProcessing(0); // temporarily add to check the Abort functionality by  Sarjeet
    PrepareAcqInfo();

    if (!SendMessage()) {

        pqm->GetStudy()->SetScanModeOfHead();
        scan_proc->ResetCurrentRequest();

        //+Patni-PJS/2009May10/Modify/IR-82 review comment
        CString string = _T("");
        string.LoadString(IDS_NOSENDMESSAGE);
        CPQMLogMgr::GetInstance()->WriteEventLog(
            string,
            ERROR_LOG_CRUCIAL1 , _T("AcqInfo.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82

        /*
        CPQMLogMgr::GetInstance()->WriteEventLog(
            IDS_NOSENDMESSAGE,
            ERROR_LOG_CRUCIAL1 , _T("AcqInfo.cpp"));
        */
        //-Patni-PJS/2009May10/Modify/IR-82 review comment

        PQM_TRACE(USER_FUNC_MARKER, l_func_name,
                  _T("Sending AMCMM_ACQINFO message to AcqMan failed"));
        return false;
    }

    return true;
}

//+Patni-Hemant/2010Feb19/Modified/ACE-Phase#3/CDS Requirement
//+Patni-ARD/2010Feb16/Added/CDS code review
//****************************Method Header************************************
//Method Name   :GetMovingBedInSaturn()
//Author        :PATNI/ARD
//Purpose       :check for moving bed in saturn coil
//*****************************************************************************
status_t CAcqInfo::GetMovingBedInSaturn(VftCoilListInfo_t* f_coillist)
{
    LPCTSTR FUNC_NAME = _T("CAcqInfo::GetMovingBedInSaturn");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("Trace of GetMovingBedInSaturn"));

    //+Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_02
    //
    //	if(!f_coillist){
    //		return E_NO_ERROR;
    //	}
    //
    //    int	i=0 , mbf=0 ;
    //    PqmCoil_t* p = NULL;
    //	CPqmCoilData *l_coil_data = CDataManager::GetInstance()->GetPQMCoilData();
    //
    //	for(i = 0; i < f_coillist->num; i++)
    //	{
    //		CCoil l_saturn_coil;
    //		l_coil_data->PqmCoilidGetSaturnCoil(f_coillist->coillist[i].coilID, f_coillist->coillist[i].portID[0], NULL, l_saturn_coil);
    //		p = l_saturn_coil.GetCoil();
    //
    //		if(p != NULL)
    //		{
    //			if(i == 0) {
    //				mbf = p->movingBed ;
    //			}
    //
    //			if( mbf != p->movingBed) {
    //			    return E_ERROR;
    //			}
    //
    //		}
    //	}
    //
    //	if(mbf){
    //	    return E_NO_ERROR;
    //	}else{
    //		return E_ERROR;
    //	}

    if (!f_coillist) {
        return E_NO_ERROR;
    }

    if (f_coillist->num <= 0) {
        return E_ERROR ;
    }

    CPqmCoilData* l_coil_data = CDataManager::GetInstance()->GetPQMCoilData();

    //get the first coil.
    CCoil l_saturn_coil;
    l_coil_data->PqmCoilidGetSaturnCoil(f_coillist->coillist[0].coilID, f_coillist->coillist[0].portID[0], NULL, l_saturn_coil);
    PqmCoil_t* l_coil = l_saturn_coil.GetCoil();

    //get the mbf for first coil
    const int l_mbf = l_coil->movingBed ;

    if (l_mbf) {

        for (int l_coil_index = 0; l_coil_index < f_coillist->num; l_coil_index++) {
            l_coil_data->PqmCoilidGetSaturnCoil(f_coillist->coillist[l_coil_index].coilID, f_coillist->coillist[l_coil_index].portID[0], NULL, l_saturn_coil);
            l_coil = l_saturn_coil.GetCoil();

            if (l_coil != NULL) {
                //mbf for all the coils should be same
                if (l_mbf != l_coil->movingBed) {
                    return E_ERROR;
                }
            }
        }

        return E_NO_ERROR;
    }

    return E_ERROR ;
    //-Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_02

}
//-Patni-ARD/2010Feb16/Added/CDS code review
//-Patni-Hemant/2010Feb19/Modified/ACE-Phase#3/CDS Requirement

//****************************Method Header************************************
//Method Name   :SetAcqInfoMode()
//Author        :PATNI/HAR-LK
//Purpose       :This method prepares the Acqinfo request that will be sent to Acqman
//*****************************************************************************
void CAcqInfo::SetAcqInfoMode(
    const int f_acqinfo_mode
)
{
    m_acqinfo_mode = f_acqinfo_mode;
}

//****************************Method Header************************************
//Method Name   :AcqInfoCallback
//Author        :PATNI/HAR
//Purpose       :This method will handle the call back procedure.   This method
//               decides which is to be done depending upon the type of call back
//               like calling error handler class if the return value is false.
//****************************************************************************
int CAcqInfo::AcqInfoCallback(
    const int f_type,
    AM_AcqManToClient_t* const f_data
)
{
    //+Patni-ARD/2010Feb18/Modified/Code Review -
    //multiple calls of GetStudy() replaced by local variable
    //Patni-AMT/2010Apr16/Modified/JaFT# COMMON_CDR_07
    LPCTSTR FUNC_NAME = _T("CAcqInfo::AcqInfoCallback");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace"));

    CScanProcedure* l_scan_proc = GetScanProcedure();

    //+Patni-RUP/2010Jul29/Modified/Internal Review Comment
    if (NULL == l_scan_proc) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("l_scan_proc is NULL"));
        return E_ERROR;
    }

    //-Patni-RUP/2010Jul29/Modified/Internal Review Comment
    CPqmPmAcqman* l_acqman = l_scan_proc->GetAcqManager();

    //+Patni-RUP/2010Jul29/Modified/Internal Review Comment
    if (NULL == l_acqman) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("l_acqman is NULL"));
        return E_ERROR;
    }

    //-Patni-RUP/2010Jul29/Modified/Internal Review Comment
    CPqm* l_pqm = l_acqman->GetPqm();

    //+Patni-RUP/2010Jul29/Modified/Internal Review Comment
    if (NULL == l_pqm) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("l_pqm is NULL"));
        return E_ERROR;
    }

    //-Patni-RUP/2010Jul29/Modified/Internal Review Comment

    //+Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
    CpqmCDS* l_pqm_cds = l_pqm->GetCDS();

    //+Patni-RUP/2010Jul29/Modified/Internal Review Comment
    if (NULL == l_pqm_cds) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("l_pqm_cds is NULL"));
        return E_ERROR;
    }

    //-Patni-RUP/2010Jul29/Modified/Internal Review Comment
    CDataManager* l_data_mgr = CDataManager::GetInstance();

    //+Patni-RUP/2010Jul29/Modified/Internal Review Comment
    if (NULL == l_data_mgr) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("l_data_mgr is NULL"));
        return E_ERROR;
    }

    //-Patni-RUP/2010Jul29/Modified/Internal Review Comment
    CPqmCoilData* l_pqm_coil_data = l_data_mgr ->GetPQMCoilData();

    //+Patni-RUP/2010Jul29/Modified/Internal Review Comment
    if (NULL == l_pqm_coil_data) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("l_pqm_coil_data is NULL"));
        return E_ERROR;
    }

    //-Patni-RUP/2010Jul29/Modified/Internal Review Comment
    //-Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement

    CPqmProtocol* l_protocol = l_scan_proc->GetScanProcProtocol();
    //+Patni-HAR/2010Apr13/Added/Phase#3 Code Review
    int receiver_coilid = -1;

    if (l_protocol) {
        receiver_coilid = l_protocol->GetPqmPrMode()->GetRecvCoilId();
    }

    //-Patni-HAR/2010Apr13/Added/Phase#3 Code Review
    status_t l_sts(0);	//Patni-AD/2009May28/Modified/SU14 //cpp test corrections


    //+Patni-MP/2009Mar16/Added/ACE-1/JETSCAN
    CPqmCoilData* l_coil_data = CDataManager::GetInstance()->GetPQMCoilData();

    CPQMStudy* l_pqm_study = l_pqm->GetStudy();

    if (NULL == l_pqm_study) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqm_study is NULL"));
        return E_ERROR;
    }

    //int l_errorid = 0;

    if (f_type == AMCM_FUNC_KIND_REQUEST) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("f_type --> AMCM_FUNC_KIND_REQUEST"));

        return E_NO_ERROR;
    }

    l_scan_proc->SetIsCoilOrGateInfoExecuting(false);
    CCFShiftCheck* l_cfshift_check = l_pqm->GetCFShiftCheck();

    if (NULL != l_cfshift_check) {
        l_cfshift_check->SetcfShiftTemp(f_data);
    }

    if (!l_scan_proc->GetIsScanning()) {


        if (m_acqinfo_mode & PQM_INFO_BIT_GATE) {
            //+Patni-RUP/2009Sep10/Modified/cpp test corrections
            CGateMode l_gateMode(l_scan_proc);
            l_gateMode.AcqSetInfoGate(&f_data->gateInfo);

            l_pqm->WriteToUI(PQM_MSG_GATE_DIALOG_SHOW);
            //-Patni-RUP/2009Sep10/Modified/cpp test corrections
        }

        if (m_acqinfo_mode & PQM_INFO_BIT_COILID) {

            if (l_pqm->IsDialogActive(IS_COIL_SELECTION_DIALOG)) {

                if (l_pqm->GetCoilSelectionTool()->SetConnectCoil(
                        f_data->coilInfo)) {

                    return E_NO_ERROR;

                } else {
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetConnectCoil() failed."));
                }
            }

            if (!l_pqm->GetCoilSelectionTool()->CoilSelSaveConnectCoil(
                    f_data->coilInfo)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CoilSelSaveConnectCoil() failed."));
            }

            if (SetInfoCoil(f_data) == E_ERROR) {
                return E_ERROR;
            }

            if (l_pqm->GetGUICoilInformationDisplay()) {
                l_pqm->WriteToUI(PQM_MSG_COILINFO_SHOW);
            }

            l_pqm->SetGUICoilInformationDisplay(false);

        }

        // Later Phase
        // if(tableMoving == TRUE &&
        //  PqmAcqGlobal.info_mode & PQM_INFO_BIT_COUCH)
        //      PqmUiTableDispPos((float)(data->couchInfo.position));

        CPqmShimSharedData::GetInstance()->ShimRegistCouchInfo(&f_data->couchInfo.couchStatusFlag,
                &f_data->couchInfo.position, NULL);
        l_pqm_study->WriteToShimData();
        //+Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
        CString l_print_str(_T(""));
        l_print_str.Format(_T("[couchInfo.autoInDistance : %f]"), f_data->couchInfo.autoInDistance);
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, l_print_str);
        l_pqm_cds->PqmCdsSetAutoIn(f_data->couchInfo.autoInDistance);
        //-Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement

        //+Patni-HAR-RG/2009June09/Modified/JFT#50
        if (l_scan_proc->IsSingleProcRunning()) {
            l_scan_proc->ResetCurrentRequest();
            l_pqm->WriteToUI(PQM_MSG_ABORT_WAIT);
            l_pqm->GetAMB()->PqmPmAbortAmb();
        }

        //-Patni-HAR-RG/2009June09/Modified/JFT#50
        return E_NO_ERROR;
    }

    if (m_acqinfo_mode & PQM_INFO_BIT_COILID) {


        //+Patni-MP/2009Mar16/Added/ACE-1
        //CPqmCoilData *l_coil_data = CDataManager::GetInstance()->GetPQMCoilData();
        if (!l_pqm->GetCoilSelectionTool()->CoilSelSaveConnectCoil(f_data->coilInfo)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CoilSelSaveConnectCoil() failed."));
        }

        // - V9.25 Coil support@AMT

        if (SetInfoCoil(f_data) == E_ERROR) {
            return E_ERROR;
        }

        // + V9.25 coil support@AMT
        const int l_rec_coil_id = (l_pqm->IsSaturnCoilSystem()) ?
                                  l_protocol->GetSaturnCoil()->RcvCoil[0].coilID :
                                  l_protocol->GetPqmPrMode()->GetRecvCoilId() ;
        // - V9.25 coil support@AMT

        CString l_str_recv_id(_T(""));
        l_str_recv_id.Format(_T("[receive coil id: %d]"), l_rec_coil_id);

        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, l_str_recv_id);

        CCoil l_tx_coil ;
        CCoil l_rcv_coil ;

        // + V9.25 Coil support@AMT
        if (l_pqm->IsSaturnCoilSystem()) {

            if (l_protocol->GetCoilDBTable()->numCoil == 0) {
                l_rcv_coil.SetCoil(NULL);

            } else {
                // l_rcv_coil_p = l_protocol->GetCoilDBTable()->coiltbl_p[0];
                PqmSaturnRcvCoil_t* l_Coil_Saturn = l_protocol->GetCoilDBTable();
                l_rcv_coil.SetCoil(l_Coil_Saturn->coiltbl_p[0]);
            }

            PqmCoil_t* l_tmp_tx_coil = l_tx_coil.GetCoil();
            l_pqm_study->GetTxCoilInSaturn(l_protocol->GetCoilDBTable(), &l_tmp_tx_coil);
            l_tx_coil.SetCoil(l_tmp_tx_coil);

        } else {

            l_coil_data->PqmCoilGetTxCoil(l_rec_coil_id, l_tx_coil);
            l_coil_data->PqmCoilGetTxCoil(l_rec_coil_id, l_rcv_coil);
        }

        if (l_tx_coil.GetCoil() == NULL || l_rcv_coil.GetCoil() == NULL) {
            //+Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
            if (!l_protocol->GetCDSInfo()->m_cds_flag) {
                //-Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
                // - V9.25 Coil support@AMT
                l_scan_proc->ResetCurrentRequest();

                CString l_error_str = L"";
                l_error_str.LoadString(IDS_COIL_INFO_ABSENT);

                //+Patni-ARD/2009-Nov-27/Commented/DeFT# MVC003149 + MVC005179
                /*
                CPQMLogMgr::GetInstance()->WriteEventLog(l_error_str,
                        ERROR_LOG_CRUCIAL1 ,
                        _T("AcqInfo.cpp"));
                CPQMLogMgr::GetInstance()->WriteTraceLog(DOMAIN_NAME,
                        COMMHANDLER_MARKER_PQM_ACQMAN,
                        _T("CAcqInfo::AcqInfoCallback"),
                        l_error_str);
                */
                //-Patni-ARD/2009-Nov-27/Commented/DeFT# MVC003149 + MVC005179

                //+Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
                CPQMLogMgr::GetInstance()->DisplayScanError(IDS_COIL_INFO_ABSENT,			// Added By KT/ACE-2/15-06-2009/Multilingualisation
                        _T("IDS_COIL_INFO_ABSENT"),
                        SCAN_ERR_NOTIFY, L"AcqInfo.cpp");
                /*
                CPQMLogMgr::GetInstance()->DisplayError(l_error_str,
                _T("AcqInfo.cpp"),
                __LINE__,
                ERROR_LOG_CRUCIAL1 ,
                ERR_ID,APP_NAME);
                */
                //-Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
                l_pqm_study->SetAcqStatus(l_protocol , C_PR_STATUS_WAIT);
                l_pqm->WriteToUI(PQM_MSG_ABORT_WAIT);
                l_pqm->GetAMB()->PqmPmAbortAmb();

                return E_ERROR;
                //+Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
            }

            //-Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
        }

        if (l_pqm->IsSaturnCoilSystem()) {

            //+Patni-AMT/2009Jun25/Added/JFT# TMSC review comment for SAR calculation
            CPqmProtocol* l_next_protocol = NULL;

            for (l_next_protocol = l_pqm_study->GetNextWaitProtocol(l_protocol);
                 l_next_protocol != NULL;
                 l_next_protocol = l_pqm_study->GetNextWaitProtocol(l_next_protocol)) {
                l_next_protocol->GetSarInfo()->couchPosition =
                    f_data->couchInfo.position;
            }

            //-Patni-AMT/2009Jun25/Added/JFT# TMSC review comment for SAR calculation
            PqmSaturnCoil_t*    l_saturn_coil   = l_protocol->GetSaturnCoil();

            VftCoilListInfo_t*  l_last_acq_coil = l_pqm->GetCoilSelectionTool()->CoilSelGetConnectCoil();

            l_sts = AcqCheckSaturnCoil(l_last_acq_coil, l_saturn_coil, TRUE);

            if (l_sts == E_NO_ERROR) {
                //TMSC-Tanoue/2010Aug25/MVC008345
                CVarFieldHandle* l_vf_handle = l_pqm_study->GetVfDBHandle();

                if (!l_vf_handle) {
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_vf_handle is NULL"));
                    return E_ERROR;
                }

                int l_prot = l_protocol->GetProtocol();
                l_pqm_study->PutSaturnCoil(l_vf_handle, l_prot, l_saturn_coil);
                l_pqm_study->PutConnectedCoil(l_vf_handle, l_prot, l_protocol);

                //+Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
                if ((l_protocol->GetCDSInfo()->m_cds_flag)		&&
                    (l_protocol->GetCDSInfo()->m_cds_step != CDS_STEP6_DONE)) {
                    l_protocol->GetCDSInfo()->m_cds_status =
                        l_pqm_cds->PqmCdsCheckCoil(&(l_protocol->GetSaturnCoil()->CoilList));

                    if (l_protocol->GetCDSInfo()->m_cds_status & VFC_CDS_STATUS_AUTO_SELECT) {
                        if (!l_pqm_cds->PqmCdsSetListCoil(l_protocol)) {
                            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PqmCdsSetListCoil() failed."));
                        }

                        CCDSUtilities::PutCdsCoordinate(l_vf_handle, l_protocol->GetProtocol(), l_protocol->GetCDSInfo()->m_coordinatetbl);

                        if (l_protocol->GetCDSInfo()->m_cds_step == CDS_STEP0_NONE)
                            l_protocol->GetCDSInfo()->m_cds_step = CDS_STEP1_RFINTERLOCK_LEVEL;	/* Skip ACQINFO_LEVEL */

                    } else {
                        if ((l_protocol->GetSaturnCoil()->numSection == 0)		||
                            (l_protocol->GetCoilDBTable()->numCoil == 0)			||
                            (l_pqm_coil_data->PqmInstallCheckSaturn(&(l_protocol->GetSaturnCoil()->CoilList)) != E_NO_ERROR)) {
                            /* The coil can not be set automatically if not set the receiver coil */
                            if (l_pqm->GetCoilSelectionTool()->CoilSelGetConnectCoil()->num <= 1) {
                                //+Patni-Ravindra Acharya/2010May14/Modified/Redmine275
                                //PqmUiErrorDialog(PqmUiFetchLiteral("MSG_COIL_CDS_NONE"),ERR_NOTIFY);
                                CPQMLogMgr::GetInstance()->DisplayScanError(IDS_CDS_MSG_COIL_CDS_NONE, _T("IDS_CDS_MSG_COIL_CDS_NONE"),
                                        SCAN_ERR_NOTIFY, _T("AcqInfo.cpp"));
                                //-Patni-Ravindra Acharya/2010May14/Modified/Redmine275

                            } else {
                                //PqmUiErrorDialog(PqmUiFetchLiteral("MSG_COIL_CDS_COIL"),ERR_NOTIFY);
                                CPQMLogMgr::GetInstance()->DisplayScanError(IDS_CDS_MSG_COIL_CDS_COIL, _T("IDS_CDS_MSG_COIL_CDS_COIL"),
                                        SCAN_ERR_NOTIFY, _T("AcqInfo.cpp")); //+Patni-NP/2010May26/Modified/Redmine-347
                            }

                            l_pqm->GetStudy()->SetScanModeOfHead();
                            m_scan_procedure->ResetCurrentRequest();

                            //l_pqm->WriteToUI(PQM_MSG_CDS_APPLY_CMPS_COIL_CUR_PROT);

                            l_pqm->WriteToUI(PQM_MSG_ABORT_WAIT);
                            l_pqm->GetAMB()->PqmPmAbortAmb();

                            return E_ERROR;
                        }

                        //About the flag they COMPASS, CDS is set if the coil is covered
                        //When you are satisfied to continue scanning. However, the output flag to clear message
                        l_protocol->SetASCOMPASSFlag(0);

                        BITFLD_DECL(mask, K_ALL_PR) = {0};

                        //PqmPqSetProtValue(	l_vfd, l_protocol,
                        //	K_PR_CDS_FLAG, &(l_protocol->GetCDSInfo()->m_cds_flag),
                        //	sizeof(int32_t),
                        //	TRUE,PQM_STS_LIST);

                        if (E_NO_ERROR != l_pqm->GetStudy()->SetProtocolValues(l_vf_handle,
                                l_protocol->GetProtocol(),
                                mask,
                                K_PR_CDS_FLAG,
                                &(l_protocol->GetCDSInfo()->m_cds_flag),
                                sizeof(l_protocol->GetCDSInfo()->m_cds_flag),
                                NULL)) {
                            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetProtocolValues() failed."));
                        }

                        //Here.. inform GUI to update the data..
                        l_pqm->WriteToUI(PQM_MSG_CDS_UPDATE_AS_CMPS_FLAG_CUR_PROT);

                        //PqmUiErrorDialog(PqmUiFetchLiteral("MSG_COIL_CDS_COIL"),ERR_NOTIFY);
                        CPQMLogMgr::GetInstance()->DisplayScanError(IDS_CDS_MSG_COIL_CDS_COIL, _T("IDS_CDS_MSG_COIL_CDS_COIL"),
                                SCAN_ERR_NOTIFY, _T("AcqInfo.cpp")); //+Patni-NP/2010May26/Modified/Redmine-347
                    }
                }

                //-Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement

            } else {
                // + SM4 Himanshu CORRECT_COIL 15 Jan 2009
                //MRERROR_LOG_DATA error_log_data; //TMSC-Tanoue/2010Mar05/MSA0251-00247
                CString l_error_string(_T(""));
                int i = 0;						//Patni-MJC/2009Aug17/Modified/cpp test corrections

                //+Patni-Hemant/2010Mar24/Added/ACE-Phase#3/CDS Requirement
                bool l_reset_request = true ;

                //-Patni-Hemant/2010Mar24/Added/ACE-Phase#3/CDS Requirement
                switch (l_sts) {
                    case ERROR_RESELECT:
                        //+Patni-Hemant/2010Feb18/Modified/ACE-Phase#3/CDS Requirement
                        //
                        //						//	l_error_string.LoadString(IDS_ERR_COIL_RESELECT);
                        //                        l_error_string = CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_COIL_RESELECT"));			// Added By KT/ACE-2/15-06-2009/Multilingualisation(_T("IDS_ERR_COIL_RESELECT"));	   // Added By KT/ACE-2/15-06-2009/Multilingualisation															// Added By KT/ACE-2/15-06-2009/Multilingualisation
                        //                        error_log_data.error_message = l_error_string;
                        //                        error_log_data.error_source = _T("PQM");
                        //                        CPQMLogMgr::GetInstance()->DisplayMessageinErrorDialog(error_log_data);
                        //                        l_pqm->GetStudy()->SetScanModeOfHead();
                        //                        m_scan_procedure->ResetCurrentRequest();
                        //                        l_pqm->WriteToUI(PQM_MSG_ABORT_WAIT);
                        //                        l_pqm->WriteToUI(PQM_MSG_CORRECT_COIL);

                        if (l_protocol->GetCDSInfo()->m_cds_flag) {
                            l_protocol->GetCDSInfo()->m_cds_status =
                                l_pqm_cds->PqmCdsCheckCoil(l_pqm->GetCoilSelectionTool()->CoilSelGetConnectCoil());

                            if (l_protocol->GetCDSInfo()->m_cds_status != PQM_CDS_NONE) {
                                CString warning_message(_T(""));

                                //PqmListSelectPos(PqmPqGetPosFromProt(PqmAcqGlobal.curObj->prot),FALSE,0);
                                //PqmPqSetVisible(PqmPqGetPosFromProt(PqmAcqGlobal.curObj->prot));
                                l_pqm->WriteToUI(PQM_MSG_CDS_SELECT_CURRENT_PROTOCOL);

                                char buf[128];
                                sprintf(buf, "\n%s -> AS-COMPASS", l_protocol->GetSaturnCoil()->CoilName);

                                //xms = XmStringCreateSimple(buf);
                                CString xms = buf;

                                if (!xms.IsEmpty()) {
                                    CString l_coil_cds_error = CPqmUtilities::GetMultiLingualString(_T("IDS_CDS_ERR_COIL_CDS_CONTINUE"));
                                    //msg = XmStringConcat(PqmUiFetchLiteral("ERR_COIL_CDS_CONTINUE"),xms);
                                    warning_message = l_coil_cds_error + xms ;

                                    //XmStringFree(xms);

                                } else {

                                    //PqmUiWarningDialog(	PqmUiFetchLiteral("ERR_COIL_CDS_CONTINUE"),
                                    //	(XtCallbackProc)pqmAcqCDSContinueCB,
                                    //	(XtCallbackProc)PqmPmCoilSelCB,
                                    //	NULL);

                                    warning_message = CPqmUtilities::GetMultiLingualString(_T("IDS_CDS_ERR_COIL_CDS_CONTINUE"));
                                }

                                if (!warning_message.IsEmpty()) {

                                    MPLUSERRORUSERRESPONSE_e l_response = (MPLUSERRORUSERRESPONSE_e) CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(
                                            warning_message,
                                            _T("PQM"),
                                            SCAN_WARN_INFORMATION,
                                            SCAN_WARN_SYSMODAL,
                                            SCAN_WARN_OK_CANCEL_BUTTONS);

                                    if (l_response == MPlusErrResponse_OK) {

                                        //contiue scan...
                                        l_scan_proc->pqmAcqCDSContinueCB();
                                        l_reset_request = false ;

                                    } else if (l_response == MPlusErrResponse_CANCEL) {


                                        //we need to have abort the protocol befre coil reselect
                                        l_pqm_study->SetAcqStatus(l_scan_proc->GetScanProcProtocol(), C_PR_STATUS_WAIT);
                                        l_pqm_study->SetScanModeOfHead();
                                        m_scan_procedure->ResetCurrentRequest();
                                        l_pqm->WriteToUI(PQM_MSG_ABORT_WAIT);
                                        l_pqm->GetAMB()->PqmPmAbortAmb();
                                        l_pqm->WriteToUI(PQM_MSG_CORRECT_COIL);
                                    }
                                }

                            } else {
                                if (l_pqm->GetCoilSelectionTool()->CoilSelGetConnectCoil()->num <= 1) {
                                    //PqmUiErrorDialog(PqmUiFetchLiteral("MSG_COIL_CDS_NONE"),ERR_NOTIFY);
                                    CPQMLogMgr::GetInstance()->DisplayScanError(IDS_CDS_MSG_COIL_CDS_NONE,
                                            _T("IDS_CDS_MSG_COIL_CDS_NONE"), SCAN_ERR_NOTIFY, L"AcqInfo.cpp");

                                } else {
                                    //PqmUiErrorDialog(PqmUiFetchLiteral("MSG_COIL_CDS_COIL"),ERR_NOTIFY);
                                    CPQMLogMgr::GetInstance()->DisplayScanError(IDS_CDS_MSG_COIL_CDS_COIL,
                                            _T("IDS_CDS_MSG_COIL_CDS_COIL"), SCAN_ERR_NOTIFY, L"AcqInfo.cpp");
                                }

                                l_pqm_study->SetAcqStatus(l_scan_proc->GetScanProcProtocol(), C_PR_STATUS_WAIT);
                                l_pqm_study->SetScanModeOfHead();
                                m_scan_procedure->ResetCurrentRequest();
                                l_pqm->WriteToUI(PQM_MSG_ABORT_WAIT);
                                l_pqm->GetAMB()->PqmPmAbortAmb();
                            }

                        } else {

                            //PqmListSelectPos(PqmPqGetPosFromProt(l_protocol->prot),FALSE,0);
                            //PqmPqSetVisible(PqmPqGetPosFromProt(l_protocol->prot));
                            //PqmUiWarningDialog(PqmUiFetchLiteral("ERR_COIL_RESELECT"), (XtCallbackProc)PqmPmCoilSelCB, NULL, NULL);
                            l_pqm->WriteToUI(PQM_MSG_CDS_SELECT_CURRENT_PROTOCOL);

                            l_error_string = CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_COIL_RESELECT"));
                            //+TMSC-Tanoue/2010Mar05/MSA0251-00247
                            /*
                            error_log_data.error_message = l_error_string;
                            error_log_data.error_source = _T("PQM");
                            CPQMLogMgr::GetInstance()->DisplayMessageinErrorDialog(error_log_data);
                            */
                            CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(l_error_string, _T("PQM"),
                                    SCAN_WARN_INFORMATION, SCAN_WARN_SYSMODAL, SCAN_WARN_OKBUTTON);
                            //-TMSC-Tanoue/2010Mar05/MSA0251-00247

                            //we need to have abort the protocol befre coil reselect
                            l_pqm->WriteToUI(PQM_MSG_ABORT_WAIT);
                            l_pqm->WriteToUI(PQM_MSG_CORRECT_COIL);
                        }

                        if (l_reset_request) {
                            l_pqm_study->SetAcqStatus(l_scan_proc->GetScanProcProtocol(), C_PR_STATUS_WAIT);
                            l_pqm_study->SetScanModeOfHead();
                            m_scan_procedure->ResetCurrentRequest();
                        }

                        //-Patni-Hemant/2010Feb18/Modified/ACE-Phase#3/CDS Requirement
                        break;

                    case ERROR_DISCONNECT:
                        //	l_error_string.LoadString(IDS_ERR_COIL_DISCONNECT); // Added By KT/ACE-2/15-06-2009/Multilingualisation
                        l_error_string = CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_COIL_DISCONNECT"));    // Added By KT/ACE-2/15-06-2009/Multilingualisation
                        //MRPdefect122
                        //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
                        //error_log_data.error_source = _T("PQM");
                        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
                            l_error_string, MPlus::EH::Information,
                            //MPlus::EH::Crucial,
                            //+ Patni-SS/Added-2009May23/JFT - Internal Defect Fix
                            _T("PQM"),
                            //- Patni-SS/Added-2009May23/JFT - Internal Defect Fix
                            _T("PQM"));
                        //MRPdefect122
                        l_pqm_study->SetScanModeOfHead();
                        m_scan_procedure->ResetCurrentRequest();
                        l_pqm->WriteToUI(PQM_MSG_ABORT_WAIT);
                        l_pqm->GetAMB()->PqmPmAbortAmb();
                        break;

                    case ERR_COILDATA:
                    case ERR_NO_COILDATA:
                        //l_error_string.LoadString(IDS_ERR_COIL_DATA);
                        //MRPdefect122
                        //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
                        //error_log_data.error_source = _T("PQM");
                        //CPQMLogMgr::GetInstance()->DisplayMessageinErrorDialog(error_log_data);
                        l_error_string = CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_COIL_DATA"));    // Added By KT/ACE-2/15-06-2009/Multilingualisation
                        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
                            l_error_string, MPlus::EH::Information,
                            //MPlus::EH::Crucial,
                            //+ Patni-SS/Added-2009May23/JFT - Internal Defect Fix
                            _T("PQM"),
                            //- Patni-SS/Added-2009May23/JFT - Internal Defect Fix
                            _T("PQM"));
                        //MRPdefect122
                        l_pqm_study->SetScanModeOfHead();
                        m_scan_procedure->ResetCurrentRequest();
                        l_pqm->WriteToUI(PQM_MSG_ABORT_WAIT);
                        l_pqm->GetAMB()->PqmPmAbortAmb();
                        break;

                        //Patni-ARD/20090226/Added/ACE-1# Enhance the withstand load of couch Start
                    case ERR_OVER_COILWEIGHT: {
                        std::string l_coilname("");

                        for (i = 0; i < l_last_acq_coil->num; i++) {
                            bool b_IsCoilidGetSaturnCoil = false;

                            CCoil			l_saturn_coil;

                            b_IsCoilidGetSaturnCoil = l_coil_data->PqmCoilidGetSaturnCoil(l_last_acq_coil->coillist[i].coilID, l_last_acq_coil->coillist[i].portID[0], NULL, l_saturn_coil);

                            if (b_IsCoilidGetSaturnCoil) {
                                PqmCoil_t*				l_coil_p = l_saturn_coil.GetCoil();

                                //MRP for short_name
                                if (l_coil_p) {
                                    l_coilname += " ";
                                    l_coilname += l_coil_p->shortName;
                                }
                            }
                        }

                        l_pqm_study->pqmAcqOverErrLog(const_cast<char*>(l_coilname.c_str()));

                        l_pqm_study->SetScanModeOfHead();
                        m_scan_procedure->ResetCurrentRequest();
                        l_pqm->WriteToUI(PQM_MSG_ABORT_WAIT);
                        l_pqm->GetAMB()->PqmPmAbortAmb();

                    }

                    break;

                    //Patni-ARD/20090226/Added/ACE-1# Enhance the withstand load of couch End
                    default:
                        // + SM4 Himanshu COIL_DEFECT_PQM_HANG 20090126
                        l_pqm_study->SetAcqStatus(l_scan_proc->GetScanProcProtocol(), C_PR_STATUS_WAIT);
                        l_pqm_study->SetScanModeOfHead();
                        m_scan_procedure->ResetCurrentRequest();
                        l_pqm->WriteToUI(PQM_MSG_ABORT_WAIT);
                        l_pqm->GetAMB()->PqmPmAbortAmb();
                        // - SM4 Himanshu COIL_DEFECT_PQM_HANG 20090126
                        break;
                }

                // - SM4 Himanshu CORRECT_COIL 15 Jan 2009
                return E_ERROR;
            }

        } else {

#if NORMAL_COIL
            // This section is used for normal type of coil. In future we might
            // need to support Normal mode.@AMT@20080730
            CCoil* l_new_coil_ptr;
            l_new_coil_ptr = &l_new_coil;

            if (CheckCoil(f_data,
                          // l_tx_coil_ptr->GetCoilKey(),    //V9.25 Coil support@AMT
                          l_tx_coil.GetCoilId(),
                          l_rec_coil_id,
                          &l_new_coil_ptr) != E_NO_ERROR) {

                if (l_new_coil_ptr == NULL) {
                    CString l_error_str = L"";
                    l_error_str.LoadString(IDS_ERR_ILLEGAL_COIL_CONNECT);
                    //CString err_string(_T(""));
                    CString err_string =  CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_ILLEGAL_COIL_CONNECT"));    // Added By KT/ACE-2/15-06-2009/Multilingualisation

                    //+Patni-ARD/2009-Nov-27/Commented/DeFT# MVC003149 + MVC005179
                    /*
                    CPQMLogMgr::GetInstance()->WriteEventLog(
                        l_error_str,
                        ERROR_LOG_CRUCIAL1,
                        _T("AcqInfo.cpp"));
                    	*/
                    //-Patni-ARD/2009-Nov-27/Commented/DeFT# MVC003149 + MVC005179

                    //+Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
                    CPQMLogMgr::GetInstance()->DisplayScanError(IDS_ERR_ILLEGAL_COIL_CONNECT,
                            _T("IDS_ERR_ILLEGAL_COIL_CONNECT"), SCAN_ERR_NOTIFY, L"AcqInfo.cpp");

                    /*
                    CPQMLogMgr::GetInstance()->DisplayError(l_error_str,
                    _T("AcqInfo.cpp"),
                    __LINE__,
                    ERROR_LOG_CRUCIAL1,
                    ERR_ID,APP_NAME);
                    */
                    //-Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView

                    l_pqm_study->SetScanModeOfHead();
                    l_scan_proc->ResetCurrentRequest();
                    l_pqm_study->SetAcqStatus(l_scan_proc->GetScanProcProtocol(), C_PR_STATUS_WAIT);
                    l_pqm->WriteToUI(PQM_MSG_ABORT_WAIT);
                    l_pqm->GetAMB()->PqmPmAbortAmb();

                } else {
                    l_scan_proc->WarningPreScan(
                        IDS_PQM_WARN_PRESCAN_DIFF_COIL,
                        _T("IDS_PQM_WARN_PRESCAN_DIFF_COIL"),	// Added By KT/ACE-2/11-06-2009/Multilingualisation
                        &l_new_coil);

                    //+Patni-PJS/2009May10/Modify/IR-82 review comment
                    CString string = _T("");
                    string.LoadString(IDS_PQM_WARN_PRESCAN_DIFF_COIL);
                    CPQMLogMgr::GetInstance()->WriteEventLog(
                        string,
                        ERROR_LOG_WARNING1,
                        _T("AcqInfo.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82
                    //-Patni-PJS/2009May10/Modify/IR-82 review comment
                    /*
                    CPQMLogMgr::GetInstance()->WriteEventLog(
                        IDS_PQM_WARN_PRESCAN_DIFF_COIL,
                        ERROR_LOG_WARNING1,
                        _T("AcqInfo.cpp"));
                    */
                }

                l_pqm_study->SetAcqStatus(l_scan_proc->GetScanProcProtocol(), C_PR_STATUS_WAIT);
                l_pqm->WriteToUI(PQM_MSG_ABORT_WAIT);
                l_pqm->GetAMB()->PqmPmAbortAmb();
                return E_ERROR;
            }

#endif // NORMAL_COIL
            //+Patni+MP/2009Mar12/Added/ACE-1/JETSCAN
            CPqmProtocol* l_curr_prot = l_scan_proc->GetScanProcProtocol();
            CCoil l_coil_class;
            l_coil_data->PqmCoilGetCoil(l_curr_prot->GetPqmPrMode()->GetRecvCoilId(), l_coil_class);
            PqmCoil_t* l_coil = l_coil_class.GetCoil();

            /* JET撮像でチャネル数がオーバーしている場合 V9.26 */
            if (l_pqm_study->IsProtocolJETEnable(l_curr_prot->GetProtocol())) {
                int l_JetChannel_V = CPQMConfig::GetInstance()->GetJetChannel_V();

                if (l_coil != NULL && l_coil->maxChannel > l_JetChannel_V) {
                    char* l_channel = new char[3];
                    memset(l_channel, 0, 3 * sizeof(char));
                    l_channel = _itoa(l_JetChannel_V, l_channel, 10);
                    char pstring[512] = {0};
                    //CString l_warningString(_T(""));
                    CString l_warningString = CPqmUtilities::GetMultiLingualString(_T("IDS_WARN_JET_CHANNEL"));	// Added By KT/ACE-2/15-06-2009/Multilingualisation
                    wcstombs(pstring, l_warningString, l_warningString.GetLength() + 1);//+Patni-HAR/2009Aug31/Modified/Corrected call for wcstombs()
                    CPqmUtilities::PQMUiErrorCat(pstring, l_channel, _T("acqinfo.cpp"));

                    if (l_channel != NULL) {
                        DEL_PTR_ARY(l_channel);	//+Patni-HAR/2009Aug11/Added/Memory Leaks
                    }

                }
            }

            //-Patni+MP/2009Mar12/Added/ACE-1/JETSCAN
            //Patni-ARD/20090226/Added/ACE-1# Enhance the withstand load of couch start
            CCoil l_couch_coil; //new
            l_coil_data->PqmCoilGetCoil(l_rec_coil_id, l_couch_coil);//new

            PqmCoil_t*	l_coil_p = l_couch_coil.GetCoil();

            if (l_coil_p != NULL && (l_pqm_study->GetWeight() > l_coil_p->weight)) {
                char wt[6] = "";	//Patni-MJC/2009Aug17/Modified/cpp test corrections
                sprintf(wt, "%d", l_coil_p->weight);

                CString l_str(_T(""));
                //	l_error_string.LoadString(ERR_OVER_COILWEIGHT);											 // Added By KT/ACE-2/15-06-2009/Multilingualisation
                CString l_error_string = CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_OVER_COILWEIGHT"));    // Added By KT/ACE-2/15-06-2009/Multilingualisation

                int l_size = sizeof(wt) + 1;
                wchar_t* l_weight = new wchar_t[l_size];

                mbstowcs(l_weight, wt, l_size);
                l_str.Format(l_error_string, l_weight);

                CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
                    l_str,
                    MPlus::EH::Crucial,
                    _T("PQM"),
                    _T("PQM"));

                DEL_PTR_ARY(l_weight);

                l_pqm_study->pqmAcqOverErrLog(l_coil_p->label);
            }

            l_pqm_study->SetScanModeOfHead();
            m_scan_procedure->ResetCurrentRequest();
            l_pqm->WriteToUI(PQM_MSG_ABORT_WAIT);
            l_pqm->GetAMB()->PqmPmAbortAmb();
            //Patni-ARD/20090226/Added/ACE-1# Enhance the withstand load of couch End

        }

        l_protocol->SetNumChangedReceiveCoil(0);

        //The below commented code implemented in Later phase
        if (l_protocol->GetMovingCouch()) {

            //+Patni-ARD/2010Feb16/Added/CDS code review
            if (l_pqm->IsSaturnCoilSystem()) {
                //+Patni-Hemant/2010Feb22/Modified/ACE-Phase#3/CDS Requirement
                //PqmSaturnRcvCoil_t* l_coil_table = l_protocol->GetCoilDBTable();
                //
                //if( GetMovingBedInSaturn( l_coil_table ) != E_NO_ERROR )
                if (GetMovingBedInSaturn(&(l_protocol->GetSaturnCoil()->CoilList)) != E_NO_ERROR)
                    //-Patni-Hemant/2010Feb22/Modified/ACE-Phase#3/CDS Requirement
                {
                    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("GetMovingBedInSaturn failed."));

                    //+Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_01
                    //+Patni-NP/2010May26/Modified/Redmine-347
                    //CString l_str_err_moving_couch = CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_MOVING_COUCH"));
                    //int l_error_level = SCAN_WARN_INFORMATION;
                    //-Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_01

                    /* MRERROR_LOG_DATA l_error_log_data;
                     l_error_log_data.error_message = l_str_err_moving_couch ;
                     l_error_log_data.error_source = _T("PQM");

                     CPQMLogMgr::GetInstance()->DisplayMessageinErrorDialog(l_error_log_data);*/

                    MPLUSERRORUSERRESPONSE_e    l_response = MPlusErrResponse_Error;
                    CString l_warnapp_name = _T("");
                    l_warnapp_name.LoadString(IDS_WARNING_APP_NAME);

                    l_response = (MPLUSERRORUSERRESPONSE_e) CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(
                                     CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_MOVING_COUCH")),
                                     l_warnapp_name, SCAN_WARN_INFORMATION ,
                                     SCAN_WARN_SYSMODAL, SCAN_WARN_OKBUTTON);
                    //-Patni-NP/2010May26/Modified/Redmine-347
                    //PqmUiErrorDialog(PqmUiFetchLiteral("ERR_MOVING_COUCH"),ERR_ERROR);
                    //PqmPqSetScanModeOfHead();
                    l_pqm_study->SetScanModeOfHead();
                    //PqmAcqResetCurrentReq();
                    m_scan_procedure->ResetCurrentRequest();
                    l_pqm_study->SetAcqStatus(l_scan_proc->GetScanProcProtocol(), C_PR_STATUS_WAIT);
                    l_pqm->WriteToUI(PQM_MSG_ABORT_WAIT);
                    l_pqm->GetAMB()->PqmPmAbortAmb();

                    return E_ERROR;
                }

            } else {
                // AcqGetMovingCouchInfo is a function of libacq lib so will be
                // implemented in later phase
                mCouchInfo_t movingCouchInfo;
                CCoil l_coil;
                AcqGetMovingCouchInfo(receiver_coilid, &movingCouchInfo) ;

                //+Patni+MP/2009Mar12/Added/ACE-1/JETSCAN
                //CPqmCoilData* l_coil_data = CDataManager::GetInstance()->GetPQMCoilData();

                l_coil_data->PqmCoilGetTxCoil(receiver_coilid, l_coil);

                if (l_coil.GetCoil() != NULL) {

                    if (l_coil.IsWBCoil() == 0 && movingCouchInfo.moveMode == ACQ_MOVE_MODE_NORMAL) {

                        //+Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_01
                        //+Patni-NP/2010May26/Modified/Redmine-347
                        CString l_str_err_moving_couch =
                            CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_MOVING_COUCH"));
                        int l_error_level = SCAN_WARN_INFORMATION;

                        MPLUSERRORUSERRESPONSE_e    l_response = MPlusErrResponse_Error;
                        CString l_warnapp_name = _T("");
                        l_warnapp_name.LoadString(IDS_WARNING_APP_NAME);

                        l_response = (MPLUSERRORUSERRESPONSE_e) CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(
                                         l_str_err_moving_couch, l_warnapp_name, l_error_level ,
                                         SCAN_WARN_SYSMODAL, SCAN_WARN_OKBUTTON);

                        //-Patni-NP/2010May26/Modified/Redmine-347
                        //-Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_01

                        // MRERROR_LOG_DATA l_error_log_data;
                        // l_error_log_data.error_message = l_str_err_moving_couch ;
                        //  l_error_log_data.error_source = _T("PQM");

                        // CPQMLogMgr::GetInstance()->DisplayMessageinErrorDialog(l_error_log_data);

                        ///                 PqmUiErrorDialog(PqmUiFetchLiteral("ERR_MOVING_COUCH"),ERR_ERROR);
                        //                 PqmPqSetScanModeOfHead();   //  V6.00
                        //                 PqmAcqResetCurrentReq();    //  V6.00
                        return E_ERROR;

                    } else if (movingCouchInfo.moveMode == ACQ_MOVE_MODE_PVCOIL) {  // PV Coil
                        if (AcqMovingPVcheck(l_protocol, l_protocol->GetPqmPrMode()->GetCouchPos(), &movingCouchInfo) != E_NO_ERROR) {
                            PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("AcqMovingPVcheck() failed."));
                            return E_ERROR ;
                        }

                    }
                }
            }

            //-Patni-ARD/2010Feb16/Added/CDS code review
        }
    }

    int l_status = -1;

    if (l_pqm->IsSaturnCoilSystem()) {	// Saturn
        // l_status = pqmAcqCheckBreastImagingInSaturn(&PqmAcqGlobal.curObj->coilDBtbl, &PqmAcqGlobal.curObj->mode.patori) ;
        //+Patni-Hemant/2010Feb18/Modified/ACE-Phase#3/CDS Requirement
        //PqmSaturnRcvCoil_t* l_coil_table = l_protocol->GetCoilDBTable();
        VftCoilListInfo_t* l_coil_table = &(l_protocol->GetSaturnCoil()->CoilList);
        //-Patni-Hemant/2010Feb18/Modified/ACE-Phase#3/CDS Requirement
        VftPatori_t l_pat_ori = l_protocol->GetPqmPrMode()->GetPatientOrientation();
        l_status = CheckBreastImagingInSaturn(l_coil_table, &l_pat_ori);

    } else	{
        int l_patori_side_up = l_protocol->GetPqmPrMode()->GetPatoriSideUp();
        int l_patori_end_in = l_protocol->GetPqmPrMode()->GetPatoriEndIn();
        l_status = CheckBreastImaging(receiver_coilid, l_patori_side_up,
                                      l_patori_end_in);
    } // l_pqm->IsSaturnCoilSystem

    if (l_status  != E_NO_ERROR) {
        //+Patni-HAR/2009April01/Added/PSP-Defect#341
        l_pqm->WriteToUI(PQM_MSG_ABORT_WAIT);
        l_pqm->GetAMB()->PqmPmAbortAmb();
        return E_ERROR;
    }

    //  //for SPEEDER PQM7
    if (l_protocol->GetSpeederType() != VFC_SPEEDER_TYPE_NONE) {
        //Patni-PJS/2010Oct13/MVC009193
        int l_num_channels = 0 ;

        // m_pqm_speeder.SetPqmPtr( m_scan_procedure->GetAcqManager()->GetPqm());
        //Meghana for Speeder
        if (l_pqm->IsSaturnCoilSystem()) {
            // Saturn
            // SPEEDER support in SM3@AMT
            //Meghana here for SPEEDER
            l_status = l_pqm->GetSpeederPtr()->MapCoilCheckSaturn(l_protocol->GetCoilDBTable()) ;
            //+Patni-PJS/2010Oct13/MVC009193
            CPqmProtocol* l_curr_prot = l_scan_proc->GetScanProcProtocol();

            if (l_curr_prot) {
                PqmSaturnCoil_t* l_saturn_coil = l_curr_prot->GetSaturnCoil();

                if (l_saturn_coil) {
                    for (int l_count = 0 ; l_count < l_saturn_coil->numSection ; l_count++) {
                        l_num_channels += l_saturn_coil->Signal[l_count].num ;
                    }
                }
            }

            //-Patni-PJS/2010Oct13/MVC009193

        } else {
            l_status = l_pqm->GetSpeederPtr()->MapCoilCheck(receiver_coilid);
        }

        if (l_status == VFC_INTENSITY_MAP_TYPE_NONE || NUM_1 == l_num_channels) {//Patni-PJS/2010Oct13/MVC009193

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Intensity type of Map is None"));

            l_scan_proc->WarningPreScan(IDS_PQM_WARN_PRESCAN_SPEEDER_COIL, _T("IDS_PQM_WARN_PRESCAN_SPEEDER_COIL"), 0); // Added By KT/ACE-2/11-06-2009/Multilingualisation
            //l_pqm_study->SetAcqStatus(l_scan_proc->GetScanProcProtocol(), C_PR_STATUS_WAIT);
            // l_pqm->WriteToUI(PQM_MSG_ABORT_WAIT);



            return E_ERROR ;
        }
    }

    //we will not perform AutoInsert procedure when Scan is restarted...
    if (!l_acqman->GetIsScanRetryFlag() ||
        ((l_protocol->GetMapRequired() || l_protocol->GetRequireShimming()) && !l_protocol->GetAtuomapAutoShimAdded())) {
        int l_AutoMap_AutoShimming_status = AutoMapAutoShimming(l_protocol, l_pqm, f_data);

        if (-1 != l_AutoMap_AutoShimming_status) {
            return l_AutoMap_AutoShimming_status;
        }
    }

#ifdef ACQINFO_RM

    //+Patni-ARD/2009Sep15/Added/Pump freezer confirmation
    if (m_acqinfo_mode & PQM_INFO_BIT_RM) {
        if ((f_data->statusList.lines & ACQ_RMSTATUS_VACUME_LOW) &&
            (f_data->statusList.lines & ACQ_RMSTATUS_FREEZER_STOP)) {

            l_scan_proc->WarningPreScan(IDS_PQM_WARN_PRESCAN_PUMP_AND_FREEZER, _T("IDS_PQM_WARN_PRESCAN_PUMP_AND_FREEZER"), 0); // Added By KT/ACE-2/11-06-2009/Multilingualisation
            //l_pqm->GetStudy()->SetAcqStatus(l_scan_proc->GetScanProcProtocol(),C_PR_STATUS_WAIT);
            //l_pqm->WriteToUI(PQM_MSG_ABORT_WAIT);
            return E_ERROR;

        } else if (f_data->statusList.lines & ACQ_RMSTATUS_VACUME_LOW) {

            l_scan_proc->WarningPreScan(IDS_PQM_WARN_PRESCAN_PUMP, _T("IDS_PQM_WARN_PRESCAN_PUMP"), 0);
            //l_pqm->GetStudy()->SetAcqStatus(l_scan_proc->GetScanProcProtocol(),C_PR_STATUS_WAIT);
            //l_pqm->WriteToUI(PQM_MSG_ABORT_WAIT);
            return E_ERROR;

        } else if (f_data->statusList.lines & ACQ_RMSTATUS_FREEZER_STOP) {

            l_scan_proc->WarningPreScan(IDS_PQM_WARN_PRESCAN_FREEZER, _T("IDS_PQM_WARN_PRESCAN_FREEZER"), 0);
            //l_pqm->GetStudy()->SetAcqStatus(l_scan_proc->GetScanProcProtocol(),C_PR_STATUS_WAIT);
            //l_pqm->WriteToUI(PQM_MSG_ABORT_WAIT);
            return E_ERROR;
        }

        //-Patni-ARD/2009Sep15/Added/Pump freezer confirmation
    }

#endif

    if (l_scan_proc->IsHasParent() || l_protocol->GetCalaPlannedStatus() != PLANNED_STATUS_NONE
        || l_protocol->GetHALAPlannedStatus() != PLANNED_STATUS_NONE) {

        CPqmPrMode l_parent_info;
        BITFLD_DECL(mask, K_ALL_PR) = {0};	//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections

        if ((l_protocol->GetCalaPlannedStatus() != PLANNED_STATUS_NONE &&
             l_protocol->GetCalaPlannedStatus() != PLANNED_STATUS_PLANNED) ||
            (l_protocol->GetHALAPlannedStatus() != PLANNED_STATUS_NONE &&
             l_protocol->GetHALAPlannedStatus() != PLANNED_STATUS_PLANNED)) {
            l_pqm->GetRefParentInfo(l_protocol, &l_parent_info);

        } else {
            l_pqm_study->GetParentInfo(l_protocol->GetProtocol(),
                                       &l_parent_info);
        }

        //scan_proc->GetParentInfo(&l_parent_info);
        //+Patni-MP/2009Mar07/Added/ACE-1#MCM0213-00154
        if (l_parent_info.GetScanNumber() != f_data->couchInfo.couchStatusFlag) {

            //+Patni-MP+Hemant/2009SEPT02/Commented/Internal defect found during MVC#4545
            //following is commented as per base code
            //l_pqm->GetStudy()->SetAcqStatus(l_scan_proc->GetScanProcProtocol(),C_PR_STATUS_WAIT);
            //l_pqm->WriteToUI(PQM_MSG_ABORT_WAIT);
            //-Patni-MP+Hemant/2009SEPT02/Commented/Internal defect found during MVC#4545

            l_scan_proc->WarningPreScan(IDS_PQM_WARN_PRESCAN_PROJECTION, _T("IDS_PQM_WARN_PRESCAN_PROJECTION"), 0);		// Added By KT/ACE-2/11-06-2009/Multilingualisation
            return E_ERROR;
        }

        //+Patni-MP/2009Mar07/Added/ACE-1#MCM0213-00154

        if (!(l_protocol->GetHoldScan() & VFC_HOLD_COUCH)) {
            if (l_scan_proc->CheckParentCouchOffset(f_data->couchInfo.position) != E_NO_ERROR) {

                //Patni-HAR/2010Jan15/Added/DeFT#MVC006728
                //Here, we have to comment this Warning Display because CheckParentCouchOffset()
                //is already displaying same warning message
                //So there is no need to display same message 2 times.
                //This code is also correct as per vantage but in vantage there is a difference between
                //vantage and Mplus architecure so to avoid displaying same message twice we
                //are commenting display warning here
                //l_scan_proc->WarningPreScan(IDS_PQM_WARN_PRESCAN_COUCH, _T("IDS_PQM_WARN_PRESCAN_COUCH"), 0);				// Added By KT/ACE-2/11-06-2009/Multilingualisation

                //+Patni-MP+Hemant/2009SEPT02/Commented/Internal defect found during MVC#4545
                //following is commented as per base code
                //l_pqm->GetStudy()->SetAcqStatus(l_scan_proc->GetScanProcProtocol(),C_PR_STATUS_WAIT);
                //l_pqm->WriteToUI(PQM_MSG_ABORT_WAIT);
                //-Patni-MP+Hemant/2009SEPT02/Commented/Internal defect found during MVC#4545

                //+Patni-PJS/2009May10/Modify/IR-82 review comment
                CString string = _T("");
                string.LoadString(IDS_PQM_WARN_PRESCAN_COUCH);
                CPQMLogMgr::GetInstance()->WriteEventLog(
                    string,
                    ERROR_LOG_WARNING1,
                    _T("AcqInfo.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82

                /*
                CPQMLogMgr::GetInstance()->WriteEventLog(
                    IDS_PQM_WARN_PRESCAN_COUCH,
                    ERROR_LOG_WARNING1,
                    _T("AcqInfo.cpp"));
                */
                //-Patni-PJS/2009May10/Modify/IR-82 review comment
                //+Patni-RAJ/2010Jan14/Added/PH#3# -IR-86/Debug Window

                CPQMDebugInfo::GetInstance(l_pqm)->PqmDebugAcqMonitor(_T("Parent : %d Current : %d"),
                        l_parent_info.GetScanNumber(),
                        l_protocol->GetPqmPrMode()->GetScanNumber());

                //-Patni-RAJ/2010Jan14/Added/PH#3# -IR-86/Debug Window

                return E_ERROR;
            }
        }

        // Removed In Vantage 9 Ver
        //if (!IS_COIL_SAME(
        //    l_parent_info.GetRecvCoilId(),
        //    l_pqm_pr_ptr->GetRecvCoilId())) {
        //
        //    l_scan_proc->WarningPreScan(IDS_PQM_WARN_PRESCAN_COIL,0);
        //    return E_ERROR;
        //}

        //+Patni-MP/2009Mar07/Commented/ACE-1#MCM0213-00154
        /* if (l_parent_info.GetScanNumber() != f_data->couchInfo.couchStatusFlag) {
         	l_pqm->GetStudy()->SetAcqStatus(l_scan_proc->GetScanProcProtocol(),C_PR_STATUS_WAIT);
             l_pqm->WriteToUI(PQM_MSG_ABORT_WAIT);
             l_scan_proc->WarningPreScan(IDS_PQM_WARN_PRESCAN_PROJECTION,0);
             return E_ERROR;
         }*/

        float couch_offset = l_parent_info.GetCouchOffset();

        if (E_NO_ERROR != l_pqm_study->SetProtocolValues(l_pqm_study->GetVfDBHandle(),
                l_protocol->GetProtocol(),
                mask,
                K_PR_PARENT_COUCH_OFFSET,
                &couch_offset,
                sizeof(float),
                NULL)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetProtocolValues() failed."));
        }

        l_parent_info.SetCouchOffset(couch_offset);

    } else {
        float l_couch_offset = 0.0f;
        BITFLD_DECL(mask, K_ALL_PR) = {0}; //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections


        if (E_NO_ERROR != l_pqm_study->SetProtocolValues(l_pqm_study->GetVfDBHandle(),
                l_protocol->GetProtocol(),
                mask,
                K_PR_PARENT_COUCH_OFFSET,
                &l_couch_offset,
                sizeof(flt32_t),
                NULL)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetProtocolValues() failed."));
        }
    }

    // check for master and slave protocol info
    if (E_ERROR == l_pqm->GetMasterSlavePtr()->CheckMasterSlaveProtocol(l_protocol, f_data->couchInfo.position, f_data->couchInfo.couchStatusFlag)) {
        return E_ERROR;
    }

    if (!PostProcess(
            f_data->couchInfo.position,
            f_data->couchInfo.couchStatusFlag,
            AMCMA_ACQINFO_DONE)) {

        //+Patni-PJS/2009May10/Modify/IR-82 review comment
        CString string = _T("");
        string.LoadString(IDS_CANNOT_PROCEED);
        CPQMLogMgr::GetInstance()->WriteEventLog(
            string,
            ERROR_LOG_CRUCIAL1 , _T("AcqInfo.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82
        /*
        CPQMLogMgr::GetInstance()->WriteEventLog(
            IDS_CANNOT_PROCEED,
            ERROR_LOG_CRUCIAL1 , _T("AcqInfo.cpp"));
        */
        //-Patni-PJS/2009May10/Modify/IR-82 review comment
        l_pqm_study->SetAcqStatus(l_scan_proc->GetScanProcProtocol(), C_PR_STATUS_WAIT);
        l_pqm->WriteToUI(PQM_MSG_ABORT_WAIT);

        return E_ERROR;
    }

    return E_NO_ERROR;
}

//****************************Method Header************************************
//Method Name   :ErrAcqInfo
//Author        :PATNI/HAR
//Purpose       :This method does the processing after an error message is
//               received from Acqman
//****************************************************************************
bool CAcqInfo::ErrAcqInfo(
    const int f_type,
    const int f_error_id,
    const CString& err_msg,
    AM_AcqManToClient_t* const f_response
)
{
    LPCTSTR FUNC_NAME = _T("CAcqInfo::ErrAcqInfo");

    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    UNREFERENCED_PARAMETER(f_response);
    CString l_str_error_id(_T(""));
    CString l_str_type = TO_STRING(f_type);
    l_str_error_id.Format(_T("%d"), f_error_id);
    CString l_str_msg = l_str_type + l_str_error_id + _T("response");

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_str_msg);

    CScanProcedure* l_scan_proc = GetScanProcedure();
    CPqm* l_pqmptr = l_scan_proc->GetAcqManager()->GetPqm();

    if (!err_msg.IsEmpty()) {
        CPQMLogMgr::GetInstance()->DisplayScanError(f_error_id, err_msg,
                SCAN_ERR_NOTIFY, _T("AcqInfo.cpp"));
    }

    if (f_type == AMCM_FUNC_KIND_REQUEST) {

        if (l_scan_proc->GetIsScanning()) {

            if (l_scan_proc->IsSingleProcRunning()) {
                l_pqmptr->GetStudy()->SetScanModeOfHead();
                l_scan_proc->ResetCurrentRequest();
            }
        }

    } else {

        if (l_scan_proc->IsSingleProcRunning()) {
            l_pqmptr->GetStudy()->SetScanModeOfHead();
            l_scan_proc->ResetCurrentRequest();
        }
    }

    l_scan_proc->SetIsCoilOrGateInfoExecuting(false);

    SetInfoCoil(NULL);


    return true;
}

//****************************Method Header************************************
//Method Name   :CheckCoil()
//Author        :PATNI/HAR
//Purpose       :THis method checks the coil received from Acqman
//****************************************************************************
int CAcqInfo::CheckCoil(
    AM_AcqManToClient_t* const data,
    const int tc_coil_key,
    const int rcv_coil_key,
    CCoil** newcoil
)const
{
    LPCTSTR FUNC_NAME = _T("CAcqInfo::CheckCoil");

    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    UNREFERENCED_PARAMETER(tc_coil_key);
    UNREFERENCED_PARAMETER(data);

    CString str_recv_coil_key(_T(""));
    str_recv_coil_key.Format(_T("%d"), rcv_coil_key);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_recv_coil_key);

    CScanProcedure* scan_proc = GetScanProcedure();
    CPqmPmAcqman* acqman_manager = scan_proc->GetAcqManager();
    CPqm* pqmmanager = acqman_manager->GetPqm();

    typedef std::map<int, CCoil*> CoilMap;
    CoilMap coilmap;

    CCoil* coilpt = NULL;
    //+Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
    //CCoil* coil_p = NULL;
    CCoil coil_p;
    //-Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
    int coilct = 1;
    int status = E_ERROR;

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (newcoil == NULL) {
        return status;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    *newcoil = NULL;
    //+Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
    CPqmCoilData* l_coil_data = CDataManager::GetInstance()->GetPQMCoilData();

    if (NULL == l_coil_data) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("l_coil_data is NULL"));
        return status;
    }

    //-Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774

    for (int i = 0; i < PQM_NUM_COIL_PORT; i++) {

        if (data->coilInfo.port[i].coilId != -1
            && data->coilInfo.port[i].coilKey != 0) {

            if (data->coilInfo.port[i].coilKey & COIL_CHANNEL_MASK) {
                //+Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
                /*coil_p =
                    pqmmanager->GetCoilDB()->GetCoil(data->coilInfo.port[i].coilKey);*/
                l_coil_data->PqmCoilGetCoil(data->coilInfo.port[i].coilKey, coil_p);
                //-Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774

            } else {
                //+Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
                /*coil_p =
                    pqmmanager->GetCoilDB()->GetEnableCoil(data->coilInfo.port[i].coilKey);*/
                l_coil_data->PqmCoilGetEnableCoil(data->coilInfo.port[i].coilKey, coil_p);
                //-Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
            }

            //+Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774

            /*if (coil_p == NULL) {

                CPQMLogMgr::GetInstance()->WriteTraceLog(DOMAIN_NAME,
                        USER_FUNC_MARKER,
                        _T("CAcqInfo::CheckCoil"),
                        _T("Coil(coil_p) Pointer is Null"));

                return E_ERROR;
            }*/
            // + V9.25 coil support@AMT
            //if (coil_p->GetWBCoil()) {
            //if (coil_p->IsWBCoil()) {
            if (coil_p.IsWBCoil()) {
                // - V9.25 coil support@AMT
                coilmap[0] = &coil_p;

            } else {
                coilmap[coilct++] = &coil_p;
            }

            //-Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
            //+Patni-Raj/2010Jan14/Added/PH#3#/Debug Window

            CPQMDebugInfo::GetInstance(pqmmanager)->PqmDebugAcqMonitor(_T("[%d] = %x"), coilct, coil_p);

            //-Patni-Raj/2010Jan14/Added/PH#3#/Debug Window
        }
    }

    CoilMap::iterator it = coilmap.find(0);

    if (it == coilmap.end()) {
        CString l_error_str = L"No Coil found";
        CString temp_string = CPqmUtilities::GetMultiLingualString(_T("IDS_NO_COIL_ERROR"));	// Added By KT/ACE-2/15-06-2009/Multilingualisation

        CPQMLogMgr::GetInstance()->DisplayError(
            //l_error_str,															    // Added By KT/ACE-2/15-06-2009/Multilingualisation
            temp_string,																// Added By KT/ACE-2/15-06-2009/Multilingualisation
            _T("AcqInfo.cpp"), __LINE__, ERROR_LOG_CRUCIAL1 ,
            ERR_ID, APP_NAME);

        pqmmanager->GetStudy()->SetScanModeOfHead();

        CPQMLogMgr::GetInstance()->WriteEventLog(
            l_error_str,
            ERROR_LOG_CRUCIAL1,
            _T("AcqInfo.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82


        return E_ERROR;
    }

    coilpt = it->second;
    status = E_NO_ERROR;

    //  GetCoilUId() is replaced by GetCoilKey() and added MACRO IS_COIL_SAME
    if (IS_COIL_SAME(coilpt->GetCoilKey(), rcv_coil_key)) {

        if (coilct > 1) {

            it = coilmap.find(1);

            if (it != coilmap.end()) {
                *newcoil = it->second;
            }

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("(coilct > 1) Condition Satisfied"));

            status = E_ERROR;
        }

    } else if (coilct == 1)  {

        it = coilmap.find(0);

        if (it != coilmap.end()) {
            *newcoil = it->second;
            status = E_ERROR;

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("(coilct == 1) Condition Satisfied"));
        }

    } else {

        for (i = 1; i < coilct; i++) {

            CCoil* l_coil = NULL;

            it = coilmap.find(i);

            if (it != coilmap.end()) {
                l_coil = it->second;
            }

            //  GetCoilUId() is replaced by GetCoilKey()
            if (l_coil->GetCoilKey() & COIL_CHANNEL_MASK) {

                if (l_coil->GetCoilKey() != (unsigned long)rcv_coil_key) {
                    *newcoil = l_coil;
                }

            } else if (!IS_COIL_SAME(l_coil->GetCoilKey(), rcv_coil_key)) {

                *newcoil = l_coil;
            }
        }

        if (*newcoil != NULL) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("(*newcoil != NULL) Condition Satisfied"));

            status = E_ERROR;
        }
    }

    if (*newcoil != NULL && !((*newcoil)->GetEnable())) {
        //+Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
        CCoil l_coil_obj;
        l_coil_data->PqmCoilGetEnableCoil((*newcoil)->GetCoilKey(), l_coil_obj);
        (*newcoil)->SetCoil(l_coil_obj.GetCoil());
        //-Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
    }

    if (status == E_ERROR) {
        //+Patni-ARD/2009-Nov-25/Modified/DeFT# MVC003149 + MVC005179
        /*
        CPQMLogMgr::GetInstance()->WriteEventLog(
            IDS_NOCOIL,
            ERROR_LOG_CRUCIAL1 , _T("AcqInfo.cpp"));
        	*/
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("No Coils present in list"));

        //+Patni-ARD/2009Jun25/Added/ACE# policy of ErrorView
        //CPQMLogMgr::GetInstance()->DisplayScanError(IDS_NOCOIL, _T("IDS_NOCOIL"),
        //        SCAN_ERR_NOTIFY, L"AcqInfo.cpp");
        //-Patni-ARD/2009Jun25/Added/ACE# policy of ErrorView
        //-Patni-ARD/2009-Nov-25/Modified/DeFT# MVC003149 + MVC005179
    }

    return status;
}

//****************************Method Header************************************
//Method Name   :PrepareAcqInfo
//Author        :PATNI/MRP
//Purpose       :To prepare acquisition info.
//****************************************************************************
void CAcqInfo::PrepareAcqInfo(
)
{
    LPCTSTR FUNC_NAME = _T("CAcqInfo::PrepareAcqInfo");
    CString l_str(_T(""));
    l_str.Format(_T("[AcqInfo Mode--> %d]"), m_acqinfo_mode);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);

    int req_cnt = 0;

    memset(&m_header, 0, sizeof(AM_ClientMessageHeader_t));
    memset(&m_data, 0, sizeof(AM_ClientToAcqMan_unicode_t));

    m_header.function = AMCMM_ACQINFO;

    if (m_acqinfo_mode & PQM_INFO_BIT_COILID) {
        m_data.param.acqInfo.kind[req_cnt++] = ACQ_ACQINFO_COILID;
    }

    if (m_acqinfo_mode & PQM_INFO_BIT_COUCH) {
        m_data.param.acqInfo.kind[req_cnt++] = ACQ_ACQINFO_COUCH;
    }

    if (m_acqinfo_mode & PQM_INFO_BIT_GATE) {
        m_data.param.acqInfo.kind[req_cnt++] = ACQ_ACQINFO_GATE;
    }

    if (m_acqinfo_mode & PQM_INFO_BIT_RM) {
        m_data.param.acqInfo.kind[req_cnt++] = ACQ_ACQINFO_RMSTATUS;
    }

    if (m_acqinfo_mode & PQM_INFO_BIT_TEMP) {
        m_data.param.acqInfo.kind[req_cnt++] = ACQ_ACQINFO_TEMP;
    }

    m_data.param.acqInfo.reqCnt = req_cnt;

    l_str = _T("");
    l_str.Format(_T("m_data.param.acqInfo.reqCnt = %d"), req_cnt);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
}

status_t	CAcqInfo::AcqMovingPVcheck(CPqmProtocol* const	curObj,
                                       const float couchPos,
                                       mCouchInfo_t*	CouchInfo
                                   )
{
    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    UNREFERENCED_PARAMETER(curObj);
    int		couchtbl[PQM_MAX_MOVINGBED_SCAN] = {0} ;
    int     loop = 0;
    int     RcvCoilTbl[PQM_MAX_MOVINGBED_SCAN] = {0};
    int	i = 0;
    //- Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    CScanProcedure* scan_proc = GetScanProcedure();
    CPqmProtocol* protocol = scan_proc->GetScanProcProtocol();
    CPqm* pqm = scan_proc->GetAcqManager()->GetPqm();

    BITFLD_DECL(mask, K_ALL_PR) = {0};	//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    DB_FUNC_ENTER("[pqmAcqMovingPVcheck]:");

    /* SVN_PLN_COUCH_TABLE */
    memset(mask, 0, sizeof(mask));
    int size = sizeof(couchtbl);
    memset(couchtbl, 0, sizeof(couchtbl));
    memset(RcvCoilTbl, 0, sizeof(RcvCoilTbl));
    status_t status = pqm->GetStudy()->GetProtocolValues(pqm->GetStudy()->GetVfDBHandle() ,
                      protocol->GetProtocol() , mask,
                      K_PR_COUCH_TABLE, couchtbl, &size,
                      K_PR_REPETITIONS, &loop, NULL,
                      NULL);
#if	0
    DB_PRINTF("M", ("%d = PqmPrVaGetProtValues(size=%d) %s:%d\n", status , size, __FILE__, __LINE__));

    for (i = 0 ; i <= loop ; i++)
        DB_PRINTF("M", ("COUCH_TABLE[%d]=%d \n", i , couchtbl[i]));

    DB_PRINTF("M", ("%s:%d = CouchInfo(moveDistance=%f) \n (moveCount=%d)\n (couchPosition=%d)\n (couchOffset=%d)\n",
                    __FILE__, __LINE__,
                    CouchInfo->moveDistance,
                    CouchInfo->moveCount,
                    CouchInfo->couchPosition,
                    CouchInfo->couchOffset
                   ));
#endif

    int NumRcvCoilTbl = 0 ;
    int coiltbl[PQM_MAX_MOVINGBED_SCAN] = {0};
    AcqGetPVcoilIDTable(coiltbl , &NumRcvCoilTbl) ;

#if	0
    i = NumRcvCoilTbl ;

    while (i--)
        DB_PRINTF("M", ("%s:%d  COILTBL[%d] = [0x%x] \n",
                        __FILE__, __LINE__, i, coiltbl[i]));

#endif

    if (protocol->GetPqmPrMode()->GetPatoriEndIn() == VFC_PATORI_HEAD_FIRST) {
        //     PqmUiErrorDialog(PqmUiFetchLiteral("ERR_PVC_ILLEGAL_PATIENT_DIR"),ERR_ERROR);
        //	  	PqmPqSetScanModeOfHead();	/*	V6.00 */
        //		PqmAcqResetCurrentReq();	/*	V6.00 */
    }

    /* 移動量が CouchInfo->couchOffset の倍数であること
       Float->intへの誤差を考慮して小数点以下を四捨五入することにした。(V6.00)
    */
    int maxMove = 0 ;
    CouchInfo->moveDistance *= 1000 ;
    CouchInfo->moveDistance += 0.5 ;
    //int moveDistancei = 0 ;
    int moveDistancei = (int)(CouchInfo->moveDistance) ;

    for (i = 1 ; i <= loop ; i++) {
        if (abs(couchtbl[i] % moveDistancei) != 0) {
            /* The couch position is Illegal. */
            DB_PRINTF("M", ("%s:%d = couchtbl[%d]=%d  moveDistancei=%d \n", __FILE__, __LINE__,
                            i , couchtbl[i], moveDistancei));
            //	PqmUiErrorDialog(PqmUiFetchLiteral("ERR_PVC_ILLEGAL_COUCH_VALUE"),ERR_ERROR);
            //	PqmPqSetScanModeOfHead();	/*	V6.00 */
            //	PqmAcqResetCurrentReq();	/*	V6.00 */
            return  E_ERROR;
        }

        maxMove = MAX((couchtbl[i] / moveDistancei), maxMove) ;
    }

    /* 移動量を分類する。 これにより移動回数を求める。  */

    if (CouchInfo->moveCount == 0 ||
        maxMove  > CouchInfo->moveCount) {
        /* The number of times of movement is Illegal. */
        DB_PRINTF("M", ("%s:%d = maxMove=%d moveDistancei=%d\n",
                        __FILE__, __LINE__, maxMove, moveDistancei));
        //   	PqmUiErrorDialog(PqmUiFetchLiteral("ERR_PVC_ILLEGAL_NUM_COUCH"),ERR_ERROR);
        //		PqmPqSetScanModeOfHead();	/*	V6.00 */
        //		PqmAcqResetCurrentReq();	/*	V6.00 */
        return  E_ERROR;
    }


    int RcvCoilTbloffset = 0 ;

    for (i = 0 ; i <= NumRcvCoilTbl ; i++) {
        if (protocol->GetPqmPrMode()->GetRecvCoilId() == coiltbl[i]) {
            RcvCoilTbloffset = i ;
            break ;
        }
    }

    for (i = 1 ; i <= loop ; i++) {
        protocol->SetReceiveCoilTable(i - 1, coiltbl[(couchtbl[i] / moveDistancei) + RcvCoilTbloffset]);

#if	0
        DB_PRINTF("M", ("%s:%d =RcvCoilTbl[]=0X%x couchtbl=%d moveDistancei=%d\n",
                        __FILE__, __LINE__, RcvCoilTbl[i], couchtbl[i], moveDistancei));
#endif

    }

    protocol->SetNumChangedReceiveCoil(loop);

    /* 現在の寝台位置求める。受信コイル位置に適しているかチェック */
    int couchPosi = (int)couchPos ;

    if (CouchInfo->couchOffset < abs(couchPosi - CouchInfo->couchPosition)) {
        /* The couch position is Illegal. */
        DB_PRINTF("M", ("%s:%d = couchPosi=%d CouchInfo(couchOffset=%d,couchPosition=%d)\n",
                        __FILE__, __LINE__, couchPosi, CouchInfo->couchOffset, CouchInfo->couchPosition));

        if ((moveDistancei / 2) <= abs(couchPosi - CouchInfo->couchPosition)) {
            //		PqmUiErrorDialog(PqmUiFetchLiteral("ERR_PVC_ILLEGAL_COUCH_POSI"),ERR_ERROR);
            //		PqmPqSetScanModeOfHead();	/*	V6.00 */
            //		PqmAcqResetCurrentReq();	/*	V6.00 */
        } else
            //	PqmAcqWarningPreScan(IDS_PQM_WARN_PRESCAN_PVC_COUCH_POSI,0);
            return  E_ERROR;
    }

    return E_NO_ERROR ;
}

//****************************Method Header************************************
//Method Name   :ProcessResponse
//Author        :PATNI/MRP
//Purpose       :This method will be called to process response received from Acqman
//*****************************************************************************
void CAcqInfo::ProcessResponse(
    AM_ClientMessageHeader_t* f_hdr,
    AM_AcqManToClient_t* f_body
)
{
    LPCTSTR FUNC_NAME = _T("CAcqInfo::ProcessResponse");
    CScanProcedure* l_scan_proc = GetScanProcedure();

    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace"));

    CPqmPmAcqman* l_acqman = l_scan_proc->GetAcqManager();
    CPqm* l_pqm = l_acqman->GetPqm();

    int l_type = 0;

    int l_func_bit = l_acqman->GetFunctionBit(f_hdr->function);

    //+Patni-Sribanta/2011Jan24/Commented/TMSC-REDMINE-1026
    ////+Patni-Raj/2010Jan14/Added/PH#3#/Debug Window
    //
    //CString l_strfunction = l_acqman->GetStr(f_hdr->function);
    //
    //if ((l_strfunction.CompareNoCase(_T("Unknown Message !!")) == 0)) {
    //    l_strfunction.Format(_T("Function (%d)"), f_hdr->function);
    //
    //}
    //
    //CString l_strstatus = l_acqman->GetStatusString(f_body->status);
    //CPQMDebugInfo::GetInstance(l_pqm)->PqmDebugAcqMonitor(_T("%-32s : %s [0x%x]"), l_strfunction, l_strstatus, l_func_bit);
    //
    ////-Patni-Raj/2010Jan14/Added/PH#3#/Debug Window
    //-Patni-Sribanta/2011Jan24/Commented/TMSC-REDMINE-1026


    if ((f_body->status == AMCM_SUCCESS) &&
        (f_hdr->function == AMCMS_COUCH_POSITION)) {

        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("[Response type: STATUS]"));

    } else if ((f_body->status == AMCM_SUCCESS) ||
               (f_body->status == AMCM_RECONST)) {

        if (f_hdr->function == AMCMR_ACQINFO) {

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("[Response type: REQUEST]"));

            l_type = AMCM_FUNC_KIND_REQUEST;
            l_scan_proc->SetProcessing(l_scan_proc->GetProcessing() | l_func_bit);

            if (E_NO_ERROR != AcqInfoCallback(l_type, f_body)) {
                PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("AcqInfoCallback() failed."));
            }

        } else if (f_hdr->function == AMCMA_ACQINFO_DONE) {

            l_type = AMCM_FUNC_KIND_ANSWER;

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("[Response type: ANSWER]"));

            l_scan_proc->SetProcessing(l_scan_proc->GetProcessing() ^ l_func_bit);
            l_scan_proc->SetRequest(l_scan_proc->GetRequest() ^ l_func_bit);

            if (E_NO_ERROR != AcqInfoCallback(l_type, f_body)) {
                PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("AcqInfoCallback() failed."));
            }

        }

        CString l_str_msg(_T(""));
        l_str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                         l_acqman->GetStr(f_body->status),
                         l_acqman->GetStr(l_type),
                         l_acqman->GetStr(f_hdr->function));

        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, l_str_msg);



    } else if (f_body->status == AMCM_NACK) {

        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("[Response type: AMCM_NACK]"));

        if (f_hdr->function == AMCMR_ACQINFO) {

            l_type = AMCM_FUNC_KIND_REQUEST;
            l_scan_proc->SetRequest(l_scan_proc->GetRequest() ^ l_func_bit);

            //+Patni-AD/2009Jun10/Added/SU14 IR#72
            if (m_acqinfo_mode & PQM_INFO_BIT_COILID) {
                if (l_pqm->GetCoilSelectionTool()->GetCoilSelectionFlagActive()) {
                    AM_AcqManCoilInfo_t l_coil_info;
                    memset(&l_coil_info, -1, sizeof(AM_AcqManCoilInfo_t));

                    if (!l_pqm->GetCoilSelectionTool()->SetConnectCoil(l_coil_info)) {
                        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetConnectCoil() failed."));
                    }
                }
            }

            //-Patni-AD/2009Jun10/Added/SU14 IR#72

        } else if (f_hdr->function == AMCMA_ACQINFO_DONE) {

            l_type = AMCM_FUNC_KIND_ANSWER;

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("[Response type: ANSWER]"));

            l_scan_proc->SetProcessing(l_scan_proc->GetProcessing() ^ l_func_bit);
            l_scan_proc->SetRequest(l_scan_proc->GetRequest() ^ l_func_bit);
        }

        CString l_str_msg(_T(""));
        l_str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                         l_acqman->GetStr(f_body->status),
                         l_acqman->GetStr(l_type),
                         l_acqman->GetStr(f_hdr->function));

        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, l_str_msg);

        if (f_hdr->function == AMCMR_ACQINFO) {
            ErrAcqInfo(l_type, IDS_ERR_ACQMAN_ACQINFO, _T("IDS_ERR_ACQMAN_ACQINFO"), f_body);			 // Added By KT/ACE-2/15-06-2009/Multilingualisation

        } else if (f_hdr->function == AMCMA_ACQINFO_DONE) {
            ErrAcqInfo(l_type, IDS_ERR_ACQMAN_ACQINFO_DONE, _T("IDS_ERR_ACQMAN_ACQINFO_DONE"), f_body);	  // Added By KT/ACE-2/15-06-2009/Multilingualisation
        }

        l_pqm->GetCoilSelectionTool()->ResetAcqInfoFlags();
        l_pqm->WriteToUI(PQM_MSG_ACQINFO_ERROR);

        l_pqm->GetStudy()->SetAcqStatus(l_scan_proc->GetScanProcProtocol(), C_PR_STATUS_WAIT);
        l_pqm->WriteToUI(PQM_MSG_ABORT_WAIT); // To Active Scan Start Button

        if (l_scan_proc->GetIsScanning()) {
            l_scan_proc->SetIsScanning(false);
        }

        l_pqm->GetAMB()->PqmPmAbortAmb();


    } else {
        if (f_hdr->function == AMCMR_ACQINFO) {

            l_type = AMCM_FUNC_KIND_REQUEST;
            int l_result = l_scan_proc->GetRequest();
            l_result ^= l_func_bit;
            //MRPdefect812
            l_scan_proc->SetRequest(l_result);
            //MRPdefect812

        } else if (f_hdr->function == AMCMA_ACQINFO_DONE) {

            l_type = AMCM_FUNC_KIND_ANSWER;

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("[Response type: ANSWER]"));

            l_scan_proc->SetProcessing(l_scan_proc->GetProcessing() ^ l_func_bit);
            l_scan_proc->SetRequest(l_scan_proc->GetRequest() ^ l_func_bit);
        }

        CString l_str_msg(_T(""));
        l_str_msg.Format(_T("[Status: %s, Type: %s, Function: %s]"),
                         l_acqman->GetStr(f_body->status),
                         l_acqman->GetStr(l_type),
                         l_acqman->GetStr(f_hdr->function));

        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, l_str_msg);

        if (f_hdr->function == AMCMR_ACQINFO) {
            ErrAcqInfo(l_type, IDS_ERR_ACQMAN_ACQINFO, _T("IDS_ERR_ACQMAN_ACQINFO"), f_body); // Added By KT/ACE-2/15-06-2009/Multilingualisation

        } else if (f_hdr->function == AMCMA_ACQINFO_DONE) {
            ErrAcqInfo(l_type, IDS_ERR_ACQMAN_ACQINFO_DONE, _T("IDS_ERR_ACQMAN_ACQINFO_DONE"), f_body);					// Added By KT/ACE-2/15-06-2009/Multilingualisation
        }

        if (l_scan_proc->GetProcessing()) {
            //  scan_proc->ResetCurrentRequest();
        }



        l_pqm->GetStudy()->SetAcqStatus(l_scan_proc->GetScanProcProtocol(), C_PR_STATUS_WAIT);
        l_pqm->WriteToUI(PQM_MSG_ABORT_WAIT); // To Active Scan Start Button

        if (l_scan_proc->GetIsScanning()) {
            l_scan_proc->SetIsScanning(false);
        }

        l_pqm->GetAMB()->PqmPmAbortAmb();
    }
}

//+Patni/AKR/2010Aug12/Added/NFD001
//****************************Method Header************************************
//Method Name   :AutoMapAutoShimming()
//Author        :PATNI
//Purpose       :Implementation of AutoShimming functionality
//*****************************************************************************
int CAcqInfo::AutoMapAutoShimming(CPqmProtocol* const f_protocol, CPqm* f_pqm, AM_AcqManToClient_t* const f_data)const
{
    CAutoInsertProcedure* l_auto_insert_proc = f_pqm->GetAutoInsertProc();
    return l_auto_insert_proc->AutoMapAutoShimming(f_protocol, AutoInsertData(f_data->couchInfo.position, f_data->couchInfo.couchStatusFlag));
}



//****************************Method Header************************************
//Method Name   :SetInfoCoil
//Author        :PATNI/HAR
//Purpose       :This method reset the old coil information and set the new
//               coil information from Acqman
//****************************************************************************
int CAcqInfo::SetInfoCoil(
    AM_AcqManToClient_t* const f_data
)
{
    LPCTSTR FUNC_NAME = _T("CAcqInfo::SetInfoCoil");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Enter"));

    // + V9.25@AMT

    //+Patni-MP/2009Apr30/Commented/ACE1 - defect
    //CCoil l_coil;
    //-Patni-MP/2009Apr30/Commented/ACE1 - defect

    // - V9.25@AMT

    CPqm* l_pqm = GetScanProcedure()->GetAcqManager()->GetPqm();
    l_pqm->DeleteAllCoilData();

    if (f_data == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_data =  NULL"));

        return E_ERROR;
    }

    int l_coil_exist = 0;
    CPqmCoilData* l_coil_data = CDataManager::GetInstance()->GetPQMCoilData();
    CCoil l_enable_coil;
    CString l_str_msg(_T(""));
    VftCoilListInfo_t* l_coil_connect = NULL;

    for (int i = 0; i < ACQ_COILINFO_PORT_CNT; i++) {

        l_str_msg = _T("");
        l_str_msg.Format(_T("[%d] coil : 0x%x ID : %d"),
                         i, f_data->coilInfo.port[i].coilKey, f_data->coilInfo.port[i].coilId);

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str_msg);

        //+Patni-RAJ/2010Jan14/Added/PH#3# -IR-86/Debug Window

        CPQMDebugInfo::GetInstance(l_pqm)->PqmDebugAcqMonitor(l_str_msg);

        //-Patni-RAJ/2010Jan14/Added/PH#3# -IR-86/Debug Window
        if (f_data->coilInfo.port[i].coilId != -1 &&
            f_data->coilInfo.port[i].coilKey != 0) {
            //+Patni-MP/2009Apr30/Added/ACE1 - defect
            CCoil l_coil;

            //-Patni-MP/2009Apr30/Added/ACE1 - defect
            //+Patni-AMT/2009Mar30/Added/ACE1#MCM0228-00017 Linear Coil defect
            if (i == ACQ_COILINFO_PORT_D || i == ACQ_COILINFO_PORT_E) {

                l_coil_connect = l_pqm->GetCoilSelectionTool()->CoilSelGetConnectCoil();

                for (int j = 0; j < l_coil_connect->num; j++) {
                    if (l_coil_connect->coillist[j].coilID ==
                        f_data->coilInfo.port[i].coilId) {
                        if (i == ACQ_COILINFO_PORT_D) {
                            l_coil_data->PqmCoilidGetSaturnCoil(
                                l_coil_connect->coillist[j].coilID,
                                l_coil_connect->coillist[j].portID[0], CDB_CHANNEL_A,
                                l_coil);

                        } else {
                            l_coil_data->PqmCoilidGetSaturnCoil(
                                l_coil_connect->coillist[j].coilID,
                                l_coil_connect->coillist[j].portID[0], CDB_CHANNEL_B,
                                l_coil);
                        }

                        break;
                    }
                }
            }


            if (l_coil.GetCoil() == NULL) {
                l_coil_data->PqmCoilGetCoilGr(f_data->coilInfo.port[i].coilKey,
                                              l_coil);
            }

            //-Patni-AMT/2009Mar30/Added/ACE1#MCM0228-00017 Linear Coil defect

            // - V9.25@SMT
            l_coil_exist++;
            //+Patni-Raj/2010Jan14/Added/PH#3#/Debug Window

            CPQMDebugInfo::GetInstance(l_pqm)->PqmDebugAcqMonitor(_T("[%d] name : %s"), i,
                    l_coil.GetCoil() ? l_coil.GetLabel() : "NULL");

            //-Patni-Raj/2010Jan14/Added/PH#3#/Debug Window


            // + V9.25@AMT
            if (l_coil.GetCoil() != NULL) {

                if (l_coil.GetEnable()) {
                    // - V9.25@AMT

                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("[enabled coil]"));

                    l_pqm->AddCoilToList(l_coil.GetLabel());

                } else {
                    //  GetCoilUId() is replaced by GetCoilKey()
                    l_coil_data->PqmCoilGetEnableCoil(l_coil.GetCoilId(),
                                                      l_enable_coil);

                    if (l_enable_coil.GetCoil() == NULL) {
                        l_pqm->AddCoilToList(l_coil.GetLabel());

                    } else {
                        l_pqm->AddCoilToList(l_enable_coil.GetLabel());
                    }
                }

            } else {

                l_str_msg = _T("");
                l_pqm->AddCoilToList(l_str_msg);
            }
        }
    }

    if (l_coil_exist == 0) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_coil_exist == 0"));

        // l_coil_msg.LoadString(IDS_ERR_NO_COIL_IN_PORT);  // Added By KT/ACE-2/15-06-2009/Multilingualisation
        l_str_msg =  CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_NO_COIL_IN_PORT"));    // Added By KT/ACE-2/15-06-2009/Multilingualisation
        l_pqm->AddCoilToList(l_str_msg);
    }

    return E_NO_ERROR;
}


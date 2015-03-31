// pqmAMB.cpp: implementation of the CpqmAMB class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pqmAMB.h"

#include <libStudyManager/Log/ScopedFuncInOutLog.h>
#include <PQM/PQMIPCManager.h>

#include "pqmAMBConstants.h"
#include "PqmIpcLibConstants.h"
#include "ScanProcedure.h"
#include "pqmpmacqman.h"
#include "pqm.h"
#include "PqmPrMode.h"
#include "scan.h"
#include "VarFieldHandle.h"
#include "pqmAMBUtilities.h"
#include "InterLock.h"
#include "MasterSlave.h"
#include "CheckSequence.h"
#include "math.h"
#include "couchconfig.h"
#include "GPLibAdapter.h"
#include "PASCOMInterface.h"
#include "PQMStudyMgr.h"
#include "PQMDebugInfo.h"
#include "AcqInfo.h"
#include "PqmPmSARManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CpqmAMB::CpqmAMB(CPqmPmAcqman* f_acq_manager)
{
    if (f_acq_manager) {
        m_acqman_ptr = f_acq_manager;
        m_pqmptr = f_acq_manager->GetPqm();
    }

    m_PqmAmbScanProt.PqmScanTbl = NULL;
    m_PqmAmbScanProt.num = 0;
    CpqmAMBUtilities::SetPQM(m_pqmptr);
    m_acq_order_for_ui = -1;
}

CpqmAMB::~CpqmAMB()
{
    DEL_PTR_ARY(m_PqmAmbScanProt.PqmScanTbl);
    DEL_PTR_ARY(m_pqmpm_amb.SlaveProt);
}


void CpqmAMB::InitParameters()
{
    m_PqmAmbScanProt.num = 0;
    DEL_PTR_ARY(m_PqmAmbScanProt.PqmScanTbl);
    DEL_PTR_ARY(m_pqmpm_amb.SlaveProt);
    DEL_PTR_ARY(m_shim_map_list.m_seq_pos);
    DEL_PTR_ARY(m_shim_map_list.m_seq_list);
    memset(&m_pqmpm_amb, 0, sizeof(m_pqmpm_amb));
}

//****************************Method Header************************************
//Method Name   :CheckAMBMenuClickConditions()
//Author        :iGATE
//Purpose       :
//*****************************************************************************
BOOL CpqmAMB::CheckAMBMenuClickConditions(const int* f_selected_items, const int f_count)const
{
    LPCTSTR FUNC_NAME = _T("CpqmAMB::CheckAmbInitialPlan");
    PQM_SCOPED_TRACE();
    int l_status = 0;
    bool_t l_benable = false;
    CPqmProtocol* l_pqmprotocol = NULL;
    CPQMStudy* l_pstudy =  m_pqmptr->GetStudy();

    for (int i = 0 ; i < f_count ; i++) {

        l_pqmprotocol = l_pstudy->GetItemFromPosition(f_selected_items[i] + 1);

        if (m_pqmptr->IsSaturnCoilSystem()) {
            /* Saturn Coil */
            l_benable =  CAcqInfo::GetMovingBedInSaturn(&l_pqmprotocol->GetSaturnCoil()->CoilList);

        } else {
            //l_benable = pqmAmbReciveCoilCheck(obj->mode.rcvCoilId);
        }

        if (l_pqmprotocol->GetAMBEnableFlag() != TRUE) {
            l_status = PQM_AMB_START_DISABLE_SEQ ;

        } else if (l_pqmprotocol->GetCDSInfo()->m_cds_flag) {
            l_status = PQM_AMB_START_WITH_CDS;

        } else if (l_pqmprotocol->GetAMBModeFlag() == VFC_AMB_MODE_SLAVE) {
            l_status = PQM_AMB_START_DISABLE_SEQ ;

        } else if (l_pqmprotocol->GetAcqStatus() != C_PR_STATUS_WAIT) {
            l_status = PQM_AMB_START_BAD_PARAMETER;

        } else if (l_benable) {
            l_status = PQM_AMB_START_RECIVED_COIL;

        } else if (l_pqmprotocol->GetMultiSeriesDynamicFlag() && (l_pqmprotocol->GetGroupID() > 0)) {
            l_status = PQM_AMB_START_BAD_PARAMETER;
        }

        int l_protocol_limit = PQM_MAX_PROTOCOLS - l_pstudy->GetAllItems() ;

        if (l_protocol_limit < f_count) {
            l_status = PQM_AMB_START_PROTOCOL_OVER;
        }

        if (l_status) {
            CpqmAMBUtilities::pqmAmbStartError(l_status) ;
            return FALSE;
        }
    }

    return TRUE;

}

//****************************Method Header************************************
//Method Name   :CheckAmbInitialPlan()
//Author        :iGATE
//Purpose       :Check AMB initial plan and does all init dialog validations.
//*****************************************************************************
BOOL CpqmAMB::CheckAmbInitialPlan(const int* f_selected_items, const int f_count)
{

    LPCTSTR FUNC_NAME = _T("CpqmAMB::CheckAmbInitialPlan");
    PQM_SCOPED_TRACE();

    int		l_arr_pos_tbl[AMB_STATION_MAX] = {0}, l_arr_scan_num[AMB_STATION_MAX] = {0}, l_arr_port_tbl[AMB_STATION_MAX] = {0};
    float	l_arr_couchtbl[AMB_STATION_MAX] = {0} ;

    // memset(&m_pqmpm_amb, 0, sizeof(m_pqmpm_amb));
    InitParameters();
    memset(l_arr_scan_num, 0, sizeof(l_arr_scan_num));
    memset(l_arr_pos_tbl, 0, sizeof(l_arr_pos_tbl));
    memset(l_arr_couchtbl, 0, sizeof(l_arr_couchtbl));

    int l_status = 0;
    bool_t l_benable = false;
    CPQMStudy* l_pstudy =  m_pqmptr->GetStudy();
    CPqmProtocol* l_pqmprotocol = NULL;

    for (int i = 0 ; i < f_count ; i++) {
        l_pqmprotocol = l_pstudy->GetItemFromPosition(f_selected_items[i] + 1);

        if (NULL == l_pqmprotocol) {

            CpqmAMBUtilities::pqmAmbStartError(PQM_AMB_SYSTEM_ERROR) ;
            return FALSE;
        }

        l_arr_port_tbl[i] = l_pqmprotocol->GetProtocol();

        if (m_pqmptr->IsSaturnCoilSystem()) {
            /* Saturn Coil */
            l_benable =  CAcqInfo::GetMovingBedInSaturn(&l_pqmprotocol->GetSaturnCoil()->CoilList);

        } else {
            //l_benable = pqmAmbReciveCoilCheck(obj->mode.rcvCoilId); <<TBD>>
        }

        if (l_pqmprotocol->GetAMBModeFlag() == VFC_AMB_MODE_SLAVE) {
            l_status = PQM_AMB_START_DISABLE_SEQ ;

        } else if (l_pqmprotocol->GetCDSInfo()->m_cds_flag) {
            l_status = PQM_AMB_START_WITH_CDS;

        } else if (l_pqmprotocol->GetHasParent() != TRUE)	{
            l_status = PQM_AMB_START_NONPLAN;

        } else if (l_pqmprotocol->GetProbeScan() != VFC_PROBE_NONE) {
            l_status = PQM_AMB_START_BAD_PARAMETER;

        } else if (l_pqmprotocol->GetDynamic() == TRUE) {
            l_status = PQM_AMB_START_BAD_PARAMETER;

        } else if (l_pqmprotocol->GetMovingCouch() != VFC_MOVING_COUCH_NONE) {
            l_status = PQM_AMB_START_BAD_PARAMETER;

        } else if (l_pqmprotocol->GetAcqStatus() != C_PR_STATUS_WAIT) {
            l_status = PQM_AMB_START_BAD_PARAMETER;

        } else if (CpqmAMBUtilities::pqmAmbMatrixCheck(l_pstudy->GetVfDBHandle(), l_pqmprotocol->GetProtocol())) {
            l_status = PQM_AMB_START_MATRIX_OVER;

        } else if (l_benable) {
            l_status = PQM_AMB_START_RECIVED_COIL;
        }

        //<<TBD>> messages to be checked againts SR
        else if (l_pqmprotocol->GetMultiSeriesDynamicFlag() && (l_pqmprotocol->GetGroupID() > 0)) {
            l_status = PQM_AMB_START_BAD_PARAMETER;

        } else if ((VFC_MAP_REQUIRED_TO_BEFORE == l_pqmprotocol->GetMapRequired()) || (VFC_MAP_REQUIRED_TO_AFTER == l_pqmprotocol->GetMapRequired())) {
            l_status = PQM_AMB_START_BAD_PARAMETER;

        } else if ((VFC_SHIM_REQUIRED_TO_BEFORE == l_pqmprotocol->GetRequireShimming()) || (VFC_SHIM_REQUIRED_TO_AFTER == l_pqmprotocol->GetRequireShimming())) {
            l_status = PQM_AMB_START_BAD_PARAMETER;

        } else if (VFC_INSCAN_NONE != l_pstudy->GetInscanFlagForProtocolIndex(l_pqmprotocol->GetProtocol())) {
            l_status = PQM_AMB_START_BAD_PARAMETER;

        } else if (VFC_INTENSITY_CORR_STATUS_TX_RX == l_pqmprotocol->GetProtcolCorrStatus()) {
            l_status = PQM_AMB_START_BAD_PARAMETER;

        } else if ((VFC_ALA_ENABLE_FLAG_ON == l_pqmprotocol->IsProtocolHALAEnable()) && (INVOKE_MODE_NONE != l_pqmprotocol->GetHALAInvokeMode())) {
            l_status = PQM_AMB_START_BAD_PARAMETER;

        } else if ((VFC_CALA_ENABLE_FLAG_ON == l_pqmprotocol->IsProtocolCALAEnable()) && (VFC_ALA_INVOKE_MODE_NONE != l_pqmprotocol->GetCALAInvokeMode())) {
            l_status = PQM_AMB_START_BAD_PARAMETER;

        } else if (l_pqmprotocol->GetAutoGetIntervalFlag()) {
            l_status = PQM_AMB_START_BAD_PARAMETER;

        } else if (l_pqmprotocol->GetAMBEnableFlag() != TRUE) {
            l_status = PQM_AMB_START_DISABLE_SEQ ;

        } else if (l_pqmprotocol->GetCalaPlannedStatus() == PLANNED_STATUS_NOVECTOR || l_pqmprotocol->GetCalaPlannedStatus() == PLANNED_STATUS_STARTED
                   || l_pqmprotocol->GetHALAPlannedStatus() == PLANNED_STATUS_NOVECTOR || l_pqmprotocol->GetHALAPlannedStatus() == PLANNED_STATUS_STARTED) {
            l_status = PQM_AMB_START_BAD_PARAMETER;

        } else if (3 == CpqmAMBUtilities::GetGDCValue(l_pstudy->GetVfDBHandle(), l_pqmprotocol->GetProtocol())) { //3D(VMC) GDC Value
            l_status = PQM_AMB_START_BAD_PARAMETER;

        } else if (0 != CpqmAMBUtilities::GetAutoMipPreview(l_pstudy->GetVfDBHandle(), l_pqmprotocol->GetProtocol())) { //check Auto Mip Preview
            l_status = PQM_AMB_START_BAD_PARAMETER;
        }


        l_arr_couchtbl[i] = l_pqmprotocol->GetOffset().z;
        l_arr_pos_tbl[i]  = l_pqmprotocol->GetPosition();
        l_arr_scan_num[i] = l_pqmprotocol->GetPqmPrMode()->GetScanNumber();

        int l_protocol_limit = PQM_MAX_PROTOCOLS - l_pstudy->GetAllItems() ;

        if (l_protocol_limit  < f_count) {
            l_status = PQM_AMB_START_PROTOCOL_OVER;
        }

        if (l_status)
            break ;

        m_pqmpm_amb.PrepFlag |= l_pqmprotocol->GetPrepScan();
        m_pqmpm_amb.Speeder |= l_pqmprotocol->GetSpeederType();
        m_pqmpm_amb.Intensity |= l_pqmprotocol->GetIntensityType();

    }

    float l_offset = l_arr_couchtbl[f_count - 1] - l_arr_couchtbl[0] ;

    for (i = 0; (0 == l_status) && i < f_count - 1; i++) {

        float	l_sub = l_arr_couchtbl[i + 1] - l_arr_couchtbl[i]  ;

        if ((l_arr_pos_tbl[i] + 1) != l_arr_pos_tbl[i + 1]) {
            l_status = PQM_AMB_START_MOVE_COUCH ;

        } else if (0 == l_offset) {
            l_status = PQM_AMB_START_MOVE_COUCH ;

        } else if (l_offset < 0  && l_sub > 0) {
            l_status = PQM_AMB_START_MOVE_COUCH ;

        } else if (l_offset > 0  && l_sub < 0) {
            l_status = PQM_AMB_START_MOVE_COUCH ;

        } else if (l_arr_scan_num[0] != l_arr_scan_num[i + 1]) {
            l_status = PQM_AMB_START_ILLEGAL_PARENT ;
        }
    }

    if (m_pqmpm_amb.PrepFlag) {
        char		prepfile[256] = {'\0'};
        CPqmUtilities::GetPrepStudyName(prepfile);

        if (strlen(prepfile) && l_pstudy->StudyCheckPlan(prepfile) !=  E_NO_ERROR) {
            l_status = PQM_AMB_START_PREP_NONPLAN ;
        }

    }

    if (l_status) {
        CpqmAMBUtilities::pqmAmbStartError(l_status) ;
        return FALSE;

    } else {
        if (l_pqmprotocol->GetPqmPrMode()->GetPatoriEndIn() == VFC_PATORI_HEAD_FIRST) {
            if (l_offset > 0)
                m_pqmpm_amb.stationOrder =  AMB_COUCH_OUTSIDE;

            else if (l_offset < 0)
                m_pqmpm_amb.stationOrder =  AMB_COUCH_INSIDE ;

        } else {
            if (l_offset > 0)
                m_pqmpm_amb.stationOrder =  AMB_COUCH_INSIDE;

            else if (l_offset < 0)
                m_pqmpm_amb.stationOrder =  AMB_COUCH_OUTSIDE;
        }

        m_pqmpm_amb.numStation = f_count ;
        memcpy(m_pqmpm_amb.offset , l_arr_couchtbl , sizeof(int)*f_count);
        m_pqmpm_amb.Master.num = f_count ;
        memcpy(&m_pqmpm_amb.Master.prot , l_arr_port_tbl , sizeof(int)*f_count);

        m_pqmptr->WriteToUI(PQM_MSG_AMB_SET_AUTO_SCAN);

        return TRUE;
    }
}

//****************************Method Header************************************
//Method Name   :pqmCheckAMBplan()
//Author        :iGATE
//Purpose       :Check plan on scan start and Calculate no. of slave protocols required.
//*****************************************************************************
status_t CpqmAMB::pqmCheckAMBplan(amb_t* const f_ambplan)
{
    LPCTSTR FUNC_NAME = _T("CpqmAMB::pqmCheckAMBplan");
    PQM_SCOPED_TRACE();

    if (f_ambplan->numStation != m_pqmpm_amb.numStation) {
        CString str;
        str.Format(_T("Error Occured. PQM vs AMBedit (%d vs %d)") , f_ambplan->numStation, m_pqmpm_amb.numStation);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str);

        /* Station数が突然変わる事はないので異常！！ エラーとする  */
        CpqmAMBUtilities::pqmAmbStartError(PQM_AMB_ILLEGAL_INFOMATION);
        pqmSendAMBScanStart_Error();
        return	E_ERROR ;
    }

    /** プロトコル複製可能数のチェック **/
    int l_num = 0;

    for (int i = 1 ; f_ambplan->numSegment > i ; i++) {
        for (int j = 0; f_ambplan->numStation > j ; j++) {
            l_num += f_ambplan->cell[i][j].repeat ;
        }
    }

    m_pqmpm_amb.numSlave = l_num ;

    if (f_ambplan->shimming)
        l_num  += f_ambplan->numStation;

    if (f_ambplan->map)
        l_num += f_ambplan->numStation;

    const int l_ProtocolLimit = PQM_MAX_PROTOCOLS - m_pqmptr->GetStudy()->GetAllItems() ;

    if (l_num > l_ProtocolLimit) {
        CString str;
        str.Format(_T("Error Occured. Protocol Over (Enable %d vs Plan %d)") , l_ProtocolLimit, l_num);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str);

        CpqmAMBUtilities::pqmAmbStartError(PQM_AMB_START_PROTOCOL_OVER);
        pqmSendAMBScanStart_Error();
        return	E_ERROR ;
    }

    memcpy((void*)&m_pqmpm_amb.AmbPlan, (void*)f_ambplan , sizeof(amb_t)) ;

    return E_NO_ERROR ;
}
//****************************Method Header************************************
//Method Name   :pqmGetAMBPlan()
//Author        :iGATE
//Purpose       :Get AMB plan by GUI.
//*****************************************************************************
status_t CpqmAMB::pqmGetAMBPlan(amb_t* const f_ambpaln, const int f_prot) const
{
    LPCTSTR FUNC_NAME = _T("CpqmAMB::pqmGetAMBPlan");
    PQM_SCOPED_TRACE();

    memset(f_ambpaln, 0, sizeof(amb_t)) ;

    int i = 0 , j = 0 ;

    /* 指定されたプロトコルからAMB計画情報を取得 */
    if (E_NO_ERROR == CpqmAMBUtilities::pqmAmbParamGetStudy(m_pqmptr->GetStudy()->GetVfDBHandle() , f_ambpaln, m_pqmpm_amb.Master.prot[0])) {
        f_ambpaln->numStation = m_pqmpm_amb.numStation;
        f_ambpaln->stationOrder = m_pqmpm_amb.stationOrder;

        if ((VFC_SPEEDER_TYPE_SPEEDER & m_pqmpm_amb.Speeder) ||
            (m_pqmpm_amb.Intensity & VFC_INTENSITY_CORR_ON))
            f_ambpaln->map = TRUE;

        else
            f_ambpaln->map = FALSE;

        for (i = 0; AMB_SEGMENT_MAX > i ; i++) {
            for (j = 0; AMB_STATION_MAX > j ; j++) {
                if (m_pqmpm_amb.PrepFlag) {
                    f_ambpaln->cell[i][j].prep =  TRUE;
                    f_ambpaln->cell[i][j].prepEnable =  TRUE;

                } else {
                    f_ambpaln->cell[i][j].prep =  AMB_DEF_PREP;
                    f_ambpaln->cell[i][j].prepEnable =  FALSE;
                }

                if (0 == f_ambpaln->cell[i][j].repeat) {
                    /* PAS更新するとNumStation,NumSegment超える領域は
                       ゼロが入ってしまう。ambEdit的には1でないといけない
                     */
                    f_ambpaln->cell[i][j].repeat = 1 ;
                }
            }
        }

        return E_NO_ERROR;
    }

    /* もし存在しなければデフォルト値を返す */
    f_ambpaln->numStation = m_pqmpm_amb.numStation;
    f_ambpaln->numSegment = AMB_DEF_SEG;
    f_ambpaln->stationOrder = m_pqmpm_amb.stationOrder;

    for (i = 0; AMB_SEGMENT_MAX > i ; i++)
        f_ambpaln->direction[i] = AMB_DEF_DIRECTION;

    f_ambpaln->shimming = AMB_DEF_SHIMMING;

    if ((VFC_SPEEDER_TYPE_SPEEDER & m_pqmpm_amb.Speeder) ||
        (m_pqmpm_amb.Intensity & VFC_INTENSITY_CORR_ON))
        f_ambpaln->map = TRUE;

    else
        f_ambpaln->map = FALSE;

    for (j = 0; f_ambpaln->numStation > j ; j++)
        f_ambpaln->mapAutoVoice[j] = AMB_DEF_VOICE;

    for (i = 0; AMB_SEGMENT_MAX > i ; i++) {
        for (j = 0; AMB_STATION_MAX > j ; j++) {
            if (m_pqmpm_amb.PrepFlag) {
                f_ambpaln->cell[i][j].prep =  TRUE;
                f_ambpaln->cell[i][j].prepEnable =  TRUE;

            } else {
                f_ambpaln->cell[i][j].prep =  AMB_DEF_PREP;
                f_ambpaln->cell[i][j].prepEnable =  FALSE;
            }

            f_ambpaln->cell[i][j].repeat =  AMB_DEF_REPEAT;
        }
    }

    return(E_NO_ERROR);
}

//****************************Method Header************************************
//Method Name   :pqmAMBScanstart()
//Author        :iGATE
//Purpose       :AMB scan start by GUI.
//*****************************************************************************
status_t CpqmAMB::pqmAMBScanstart(amb_t*	const f_ambplan)
{
    LPCTSTR FUNC_NAME = _T("CpqmAMB::pqmAMBScanstart");
    PQM_SCOPED_TRACE();

    if (PqmAMBCheckScanStatus(AMB_GO_SCAN))
        return E_NO_ERROR;

    if (pqmCheckAMBplan(f_ambplan) == E_NO_ERROR)
        return pqmAmbCouchCheck();

    return E_ERROR	;
}

//****************************Method Header************************************
//Method Name   :pqmAMBScanAbort()
//Author        :iGATE
//Purpose       :Abort AMB scan.
//*****************************************************************************
status_t CpqmAMB::pqmAMBScanAbort()
{
    LPCTSTR FUNC_NAME = _T("CpqmAMB::pqmAMBScanAbort");
    PQM_SCOPED_TRACE();

    CScanProcedure* l_scan_procedure = m_pqmptr->GetAcqManager()->GetScanProcedure();

    CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("[**AMB ABORT**] PqmAcqGlobal.processing = 0x%x \n"), l_scan_procedure->GetProcessing());
    CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("[**AMB ABORT**] m_pqmpm_amb.Amb_status = 0x%x \n"), m_pqmpm_amb.Amb_status);
    CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("[**AMB ABORT**] PqmAcqGlobal.request = 0x%x \n"), l_scan_procedure->GetRequest());

    if (l_scan_procedure->GetIsWaitingFlagForSARCtrl()) {
        l_scan_procedure->WaitTimeTimerReset(l_scan_procedure->GetScanProcProtocol());
    }

    /* Scan Abort処理 */
    if (l_scan_procedure->GetProcessing() || l_scan_procedure->GetRequest()) {
        m_acqman_ptr->AcqmanAbortFromnMainThread();

    } else if (PqmAMBCheckScanStatus(AMB_GO_SCAN))
        PqmPmAbortAmb();

    return 1;
}
//****************************Method Header************************************
//Method Name   :pqmAmbCouchCheck()
//Author        :iGATE
//Purpose       :Check couch offset and show warning message.
//*****************************************************************************
status_t CpqmAMB::pqmAmbCouchCheck(void)
{
    LPCTSTR FUNC_NAME = _T("CpqmAMB::pqmAmbCouchCheck");
    PQM_SCOPED_TRACE();

    if (!m_pqmptr->PqmPmIsActive(K_PM_TYPE_AMB)) {
        pqmAmbSystemError();
        return E_ERROR;
    }

    int	l_msg = 0;

    switch (pqmAmbCheckMoveCouch(&l_msg)) {

        case COUCH_ERR_LEVEL3 : {

            MPLUSERRORUSERRESPONSE_e l_response = (MPLUSERRORUSERRESPONSE_e)CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(
                    CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_AMB_COUCH_POSI_ILLEGAL")),
                    _T("PQM"),
                    SCAN_WARN_INFORMATION,
                    SCAN_WARN_SYSMODAL,
                    SCAN_WARN_CANCELBUTTON);

            if (MPlusErrResponse_CANCEL == l_response) {
                pqmAmbPopupNormalCancel();
            }

            return	E_ERROR ;

        }

        case COUCH_ERR_LEVEL1 : {
            MPLUSERRORUSERRESPONSE_e l_response = (MPLUSERRORUSERRESPONSE_e) CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(
                    CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_AMB_START_COUCH_OVER")),
                    _T("PQM"),
                    SCAN_WARN_CRUCIAL,
                    SCAN_WARN_SYSMODAL,
                    SCAN_WARN_OK_CANCEL_BUTTONS);

            if (MPlusErrResponse_OK == l_response) {
                return pqmAmbPopupCheckParent(NULL);

            } else {
                pqmAmbPopupNormalCancel();
            }

            return E_ERROR ;

        }

        case COUCH_ERR_NON: {

            MPLUSERRORUSERRESPONSE_e l_response = (MPLUSERRORUSERRESPONSE_e) CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(
                    CPqmUtilities::GetMultiLingualString(_T("IDS_WARN_AMB_COUCH_MOVE_NORMAL")),
                    _T("PQM"),
                    SCAN_WARN_CRUCIAL,
                    SCAN_WARN_SYSMODAL,
                    SCAN_WARN_OK_CANCEL_BUTTONS);

            if (MPlusErrResponse_OK == l_response) {
                return pqmAmbPopupCheckParent(NULL);

            } else {
                pqmAmbPopupNormalCancel();
                return E_ERROR ;
            }
        }
    }


    return E_ERROR ;
}


//****************************Method Header************************************
//Method Name   :pqmAmbCheckMoveCouch()
//Author        :iGATE
//Purpose       :Check couch offset.
//*****************************************************************************
status_t CpqmAMB::pqmAmbCheckMoveCouch(int*	const f_msg)
{
    LPCTSTR FUNC_NAME = _T("CpqmAMB::pqmAmbCheckMoveCouch");
    PQM_SCOPED_TRACE();

    CPQMStudy* l_study = m_pqmptr->GetStudy();
    CPqmProtocol* l_obj  = l_study->GetItemFromProtocol(m_pqmpm_amb.Master.prot[0]);

    status_t	l_sts = COUCH_ERR_NON ;

    if ((NULL == l_study) || (NULL == l_obj)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_study or l_obj pointer is NULL"));
        return l_sts;
    }


    int	 l_msg_id = PQM_AMB_COUCH_MOVE_NORMAL ;
    status_t l_error_level  = COUCH_ERR_NON ;
    int l_error_msg = PQM_AMB_COUCH_MOVE_NORMAL ;

    CPqmPrMode l_modeEntry ;

    l_modeEntry.Copy(l_obj ->GetPqmPrMode());

    for (int i = 0 ; i < m_pqmpm_amb.numStation ; i++) {
        l_obj  = l_study->GetItemFromProtocol(m_pqmpm_amb.Master.prot[i]);
        l_sts = CpqmAMBUtilities::pqmAmbCheckMovingCouch(l_obj  , &l_modeEntry, &m_pqmpm_amb.NewPosi[i], &m_pqmpm_amb.MoveCouchVal[i], &l_msg_id) ;

        if (l_sts > l_error_level) {
            l_error_level = l_sts ;
            l_error_msg = l_msg_id ;
        }

        l_modeEntry.SetCouchPos(m_pqmpm_amb.NewPosi[i]);
    }

    *f_msg = l_error_msg ;

    return l_error_level;
}

//****************************Method Header************************************
//Method Name   :pqmAmbPopupCheckParent()
//Author        :iGATE
//Purpose       :Check Paraent offset and show warning message.
//*****************************************************************************
status_t CpqmAMB::pqmAmbPopupCheckParent(void*	f_data)
{
    LPCTSTR FUNC_NAME = _T("CpqmAMB::pqmAmbPopupCheckParent");
    PQM_SCOPED_TRACE();

    if (!m_pqmptr->PqmPmIsActive(K_PM_TYPE_AMB)) {
        pqmAmbSystemError();
        return E_ERROR;
    }

    if (pqmAmbCouchParent() != E_NO_ERROR) {
        /* 位置決め画像の親子関係問題あり */

        MPLUSERRORUSERRESPONSE_e l_response = (MPLUSERRORUSERRESPONSE_e) CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(
                CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_AMB_START_LOCATOR_PATH_DELETE")),
                _T("PQM"),
                SCAN_WARN_INFORMATION,
                SCAN_WARN_SYSMODAL,
                SCAN_WARN_OK_CANCEL_BUTTONS);

        if (MPlusErrResponse_OK == l_response) {
            pqmAmbPopupParentClear();
            return E_NO_ERROR;

        } else {
            pqmAmbPopupNormalCancel();
            return E_ERROR;

        }

    } else {
        /* 位置決め画像の親子関係問題無し
           ここでやっと撮像可能な状態になった */
        return pqmAmbPopupScanGo(NULL);

    }
}

//****************************Method Header************************************
//Method Name   :pqmAmbCouchParent()
//Author        :iGATE
//Purpose       :Check Parent couch offset for all AMB protocols.
//*****************************************************************************
int CpqmAMB::pqmAmbCouchParent(void)
{
    LPCTSTR FUNC_NAME = _T("CpqmAMB::pqmAmbCouchParent");
    PQM_SCOPED_TRACE();

    CPQMStudy* l_study = m_pqmptr->GetStudy();
    int l_return = E_NO_ERROR;

    CPqmProtocol*	l_obj = NULL;

    for (int i = 0 ; i < m_pqmpm_amb.numStation ; i++) {
        m_pqmpm_amb.ParentOff[i] = FALSE ;
        l_obj = l_study->GetItemFromProtocol(m_pqmpm_amb.Master.prot[i]);

        if (NULL == l_obj) {
            return E_ERROR;
        }

        if (pqmAmbCheckParentCouchOffset(l_obj, m_pqmpm_amb.NewPosi[i]) != E_NO_ERROR) {
            m_pqmpm_amb.ParentOff[i] = TRUE ;
            l_return = E_ERROR ;
        }
    }

    return l_return ;
}
//****************************Method Header************************************
//Method Name   :pqmAmbCheckParentCouchOffset()
//Author        :iGATE
//Purpose       :Check Parent couch offset for given protocol.
//*****************************************************************************
status_t CpqmAMB::pqmAmbCheckParentCouchOffset(CPqmProtocol* const f_obj, const float	f_ccp)const
{
    LPCTSTR FUNC_NAME = _T("CpqmAMB::pqmAmbCheckParentCouchOffset");
    PQM_SCOPED_TRACE();

    if (f_obj->GetHasParent()) {
        CPQMStudy* l_study = m_pqmptr->GetStudy();
        CPqmPrMode l_parent_info;

        l_study->GetParentInfo(f_obj->GetProtocol(), &l_parent_info);

        const float l_pcp = l_parent_info.GetCouchPos() ;
        float l_couch = l_pcp - f_ccp;
        l_couch = ABSF(l_couch);

        const float z = (float)(f_obj->GetOffset().z * 1000.);
        float l_scp = 0;

        if (f_obj->GetPqmPrMode()->GetPatoriEndIn() == VFC_PATORI_HEAD_FIRST)
            l_scp = (f_obj->GetPqmPrMode()->GetHomePos() + z);

        else
            l_scp = (f_obj->GetPqmPrMode()->GetHomePos() - z);

        /* 寝台位置が親画像と１５cm以上離れている場合あるいは、
        親画像の寝台位置と位置決めした撮像位置が１５cm以上異なる場合で且つ親画像と寝台位置が異なる場合 */
        const double l_maximumParentOffset = m_pqmptr->GetCouchConfigPtr()->GetCouchMaximumParentOffset();

        if ((l_couch > l_maximumParentOffset) ||
            (ABSF(l_pcp - f_ccp) > COUCH_ACCURACY_RANGE_MM && ABSF(l_pcp - l_scp) > l_maximumParentOffset)
           ) {
            return E_ERROR;
        }
    }

    return	E_NO_ERROR;
}

//****************************Method Header************************************
//Method Name   :pqmAmbPopupNormalCancel()
//Author        :iGATE
//Purpose       :Handle popup cancel.
//*****************************************************************************
void CpqmAMB::pqmAmbPopupNormalCancel()
{
    LPCTSTR FUNC_NAME = _T("CpqmAMB::pqmAmbPopupNormalCancel");
    PQM_SCOPED_TRACE();

    if (!m_pqmptr->PqmPmIsActive(K_PM_TYPE_AMB)) {
        pqmAmbSystemError();
        return ;
    }

    pqmSendAMBScanStart_Error();
}

//****************************Method Header************************************
//Method Name   :pqmSendAMBScanStart_Error()
//Author        :iGATE
//Purpose       :Send AMB scan start error to gui.
//*****************************************************************************
inline void CpqmAMB::pqmSendAMBScanStart_Error()
{
    m_pqmptr->WriteToUI(PQM_MSG_AMB_SCAN_START_ERROR);
}

//****************************Method Header************************************
//Method Name   :pqmAmbSystemError()
//Author        :iGATE
//Purpose       :Abort AMB scan if any error found.
//*****************************************************************************
void CpqmAMB::pqmAmbSystemError(void)
{
    LPCTSTR FUNC_NAME = _T("CpqmAMB::pqmAmbSystemError");
    PQM_SCOPED_TRACE();

    /* Scan Abort処理 */
    if (m_acqman_ptr->GetScanProcedure()->GetProcessing() || m_acqman_ptr->GetScanProcedure()->GetRequest()) {
        m_acqman_ptr->AcqmanAbortFromnMainThread();

    } else {
        PqmPmAbortAmb() ;
    }
}
//****************************Method Header************************************
//Method Name   :pqmAmbPopupParentClear()
//Author        :iGATE
//Purpose       :To reset the parent information.
//*****************************************************************************
status_t CpqmAMB::pqmAmbPopupParentClear()
{
    LPCTSTR FUNC_NAME = _T("CpqmAMB::pqmAmbPopupParentClear");
    PQM_SCOPED_TRACE();

    CPqmProtocol*	l_obj = NULL;
    CPQMStudy* l_study = m_pqmptr->GetStudy();

    for (int i = 0 ; i < m_pqmpm_amb.numStation ; i++) {
        if (TRUE == m_pqmpm_amb.ParentOff[i]) {
            /* 親子関係をクリアする */
            l_obj =  l_study->GetItemFromProtocol(m_pqmpm_amb.Master.prot[i]);
            m_acq_order_for_ui = m_pqmpm_amb.Master.prot[i];
            pqmAmbInitPatientPath(l_obj);
        }
    }

    return pqmAmbPopupScanGo(NULL);
}
//****************************Method Header************************************
//Method Name   :pqmAmbInitPatientPath()
//Author        :iGATE
//Purpose       :To reset the parent information for the protocol.
//*****************************************************************************
void CpqmAMB::pqmAmbInitPatientPath(CPqmProtocol* f_obj) const
{
    LPCTSTR FUNC_NAME = _T("CpqmAMB::pqmAmbInitPatientPath");
    PQM_SCOPED_TRACE();

    if (NULL == f_obj) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_obj pointer is NULL"));
        return;
    }

    BITFLD_DECL(mask, K_ALL_PR);
    memset(mask, 0, sizeof(mask));

    CPQMStudy* l_study = m_pqmptr->GetStudy();
    VfPathElem_t    l_locImages[4] = {0};
    int32_t     l_prot_status = 0;
    CString l_series_loid = _T("");
    flt32_t		l_parent_couchoffset = 0.0 ;

    status_t l_status =  l_study->SetProtValues(l_study->GetVfDBHandle(), f_obj->GetProtocol(), mask,
                         K_PR_LOCATOR_IMAGE_ID, &l_series_loid, l_series_loid.GetLength(),
                         K_PR_LOCATOR_IMAGE, l_locImages, sizeof(l_locImages),
                         K_PR_STATUS,   &l_prot_status,   sizeof(int32_t),
                         K_PR_PARENT_COUCH_OFFSET, &l_parent_couchoffset, sizeof(l_parent_couchoffset),
                         NULL);

    if (E_NO_ERROR != l_status) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("SetProtValues failed"));
    }


    if (PLAN_CHECK(f_obj)) {
        f_obj->SetHasParent(false);

        int l_pl_status = f_obj->GetPlnStatus();
        l_pl_status &= ~VFC_PLN_STATUS_GLOC;
        f_obj->SetPlnStatus(l_pl_status);

        m_pqmptr->WriteToUI(PQM_MSG_AMB_PARENT_IMAGE_DELETED);

        f_obj->SetLocatorImages(NULL);

        /*** But to convert the coordinate system coordinate system HH CC from here would otherwise,
        If the AMB, you need to move the bed even when you turn off the parent-child relationship.
        Leave the coordinate system HH So, in the same state as ScanOffset.　***/
        f_obj->SetCouchOffsetFlag(FALSE);
    }
}

//****************************Method Header************************************
//Method Name   :pqmAmbPopupScanGo()
//Author        :iGATE
//Purpose       :AMB scan go after couch movement message.
//*****************************************************************************
status_t CpqmAMB::pqmAmbPopupScanGo(void* f_data)
{
    LPCTSTR FUNC_NAME = _T("CpqmAMB::pqmAmbPopupScanGo");
    PQM_SCOPED_TRACE();

    CPQMStudy* l_study = m_pqmptr->GetStudy();


    /******************************************************************
        V7.21 MCM0160-00026
        位置決め画像登録有りの場合は強制的に位置決め画像登録無しにする。
        自動差分処理等が行われると膨大な数の画像が出来てしまうから。
    *******************************************************************/
    CpqmAMBUtilities::pqmAmbCheckGDC_LocatorImage(l_study->GetVfDBHandle(), &m_pqmpm_amb.Master);

    if (E_NO_ERROR == CpqmAMBUtilities::pqmAmbParamPutStudy(l_study->GetVfDBHandle(), &m_pqmpm_amb.AmbPlan , &m_pqmpm_amb.Master)) {
        if (pqmAMBmakeProtocol() == E_NO_ERROR) {
            /* Scan 開始する */
            /* AMB Scan中フラグをONにする */
            m_pqmpm_amb.Amb_status |= AMB_GO_SCAN;
            float l_couch = PQM_COUCH_INIT_POSITION;
            CPqmShimSharedData::GetInstance()->ShimRegistTuneInfo(&l_couch, &l_couch, &l_couch);
            CPqmShimSharedData::GetInstance()->InitShim();
            //AMBMasterScanStart();
            return E_NO_ERROR;
        }
    }

    return E_ERROR;
}

//****************************Method Header************************************
//Method Name   :pqmAMBmakeProtocol()
//Author        :iGATE
//Purpose       :Make AMB protocols ready for AMB scan.
//*****************************************************************************
status_t CpqmAMB::pqmAMBmakeProtocol(void)
{

    LPCTSTR FUNC_NAME = _T("CpqmAMB::pqmAMBmakeProtocol");
    PQM_SCOPED_TRACE();

    CPQMStudy* l_study = m_pqmptr->GetStudy();

    /* Masterは撮像順に並びを変えるので、オリジナルの順はコピーしておく */
    memcpy(&m_pqmpm_amb.Master_org, &m_pqmpm_amb.Master, sizeof(Entrytbl_t));

    /**** ここで連動処理情報を入れる ****/
    //  pqmAmbSetPostProc(PqmGlobal.pqmStudy.vfHandle, &m_pqmpm_amb.AmbPlan , &m_pqmpm_amb.Master); <<TBD>>

    /**** Slave Protocol情報を入れる器を確保 ****/

    m_pqmpm_amb.SlaveProt = new int[m_pqmpm_amb.numSlave];

    if (NULL == m_pqmpm_amb.SlaveProt) {
        pqmAmbSystemError();
        return E_ERROR ;
    }

    for (int i = 0 ; i < m_pqmpm_amb.numSlave ; i++) {
        *(m_pqmpm_amb.SlaveProt + i) = -1 ;
    }

    int* l_prot_list = m_pqmpm_amb.Master.prot ;
    int l_count = m_pqmpm_amb.Master.num ;

    /* SEG1並び替え処理 ここで、Masterは撮像順に並びが変わる */

    pqmAmbSortSeg1Proc(l_prot_list, l_count) ;

    memset(&m_pqmpm_amb.shimming, NULL, sizeof(Entrytbl_t)) ;
    memset(&m_pqmpm_amb.map, NULL, sizeof(Entrytbl_t)) ;

    CPQMAMBcallback* l_pAMBcallbk = NULL;

    if (m_pqmpm_amb.AmbPlan.shimming || m_pqmpm_amb.AmbPlan.map) {

        l_pAMBcallbk = new CPQMAMBcallback(l_study);
        memset(l_pAMBcallbk, NULL, sizeof(CPQMAMBcallback));
        l_pAMBcallbk->m_study = l_study;
    }


    /* Shimming/MAPプロトコル複製 */
    if (m_pqmpm_amb.AmbPlan.shimming) {
        l_pAMBcallbk->m_entry = &m_pqmpm_amb.shimming;
        CpqmAMBUtilities::pqmAmbGetOtherProtocols((void*)l_pAMBcallbk , 0, l_count, AMB_SHIMMING_STUDY);

        if (m_pqmpm_amb.shimming.num != l_count) {	/* 作成エラー*/

            CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(CPqmUtilities::GetMultiLingualString(_T("IDS_FAIL_ADD_SHIM_PROTOCOL")),
                    MPlus::EH::Information, _T("CpqmAMB.cpp"), _T("PQM"));

            PqmAmbDeleteAutoShimMapFromVarfieldOnly();
            pqmAmbSystemError();
            return E_ERROR ;
        }
    }

    if (m_pqmpm_amb.AmbPlan.map) {
        l_pAMBcallbk->m_entry = &m_pqmpm_amb.map;
        CpqmAMBUtilities::pqmAmbGetOtherProtocols((void*)l_pAMBcallbk, 0, l_count, AMB_MAP_STUDY);

        if (m_pqmpm_amb.map.num != l_count) {	/* 作成エラー*/

            CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(CPqmUtilities::GetMultiLingualString(_T("IDS_FAIL_ADD_MAP_PROTOCOL")),
                    MPlus::EH::Information, _T("CpqmAMB.cpp"), _T("PQM"));

            PqmAmbDeleteAutoShimMapFromVarfieldOnly();
            pqmAmbSystemError();
            return E_ERROR ;
        }
    }

    DEL_PTR(l_pAMBcallbk);

    /* SEG2以降のプロトコル複製リスト作成処理 */
    if (pqmAmbMakeDuplicateList(m_pqmpm_amb.Master_org.prot, l_count) != E_NO_ERROR) {
        /* SYSTEMエラー */

        PqmAmbDeleteAutoShimMapFromVarfieldOnly();
        pqmAmbSystemError();
        return E_ERROR ;
    }

    /** ここでShimming/MAPの並び順を変更する **/
    pqmAmbSortShim_MAP(l_prot_list, l_count);
    l_study->SetScanModeOfHead();

    //pqmAmbRedrawProtocol() ;

    /*　SEG1を自動撮像にする。*/
    CPqmProtocol* l_protocol = NULL;

    for (i = 0; i < m_pqmpm_amb.Master.num; i++) {
        l_protocol = l_study->GetItemFromProtocol(m_pqmpm_amb.Master.prot[i]);
        CpqmAMBUtilities::pqmAmbAutoScanSet(l_study->GetVfDBHandle(), m_pqmpm_amb.Master.prot[i], l_protocol, 0);
    }

    /** AMB Scan table 作成 **/
    if (pqmAmbMakeScanTbl() != E_NO_ERROR) {
        PqmAmbDeleteAutoShimMapFromVarfieldOnly();
        pqmAmbSystemError();
        return E_ERROR ;
    }

    if (m_pqmpm_amb.AmbPlan.map || m_pqmpm_amb.AmbPlan.shimming) {
        if (!CreateMPlusSeriesAndGUIProtcol(l_count)) {
            PqmAmbDeleteAutoShimMapFromVarfieldOnly();
            pqmAmbSystemError();
            return E_ERROR ;
        }
    }

    UpdatePrepForAMBMaster();

    /*** ここで全てのプロトコルの撮像可能有無をチェックする ***/

    m_pqmptr->WriteToUI(PQM_MSG_AMB_SET_AUTO_SCAN);

    SetAMBScanLabel();

    return pqmAbmDiskSpaceCheck() ;
}

//****************************Method Header************************************
//Method Name   :pqmAmbSortSeg1Proc()
//Author        :iGATE
//Purpose       :Sort Master protocols.
//*****************************************************************************
status_t CpqmAMB::pqmAmbSortSeg1Proc(int* const f_prot_list, const int f_num)
{
    LPCTSTR FUNC_NAME = _T("CpqmAMB::pqmAmbSortSeg1Proc");
    PQM_SCOPED_TRACE();

    if (NULL == f_prot_list) {
        return E_ERROR ;
    }

    int* l_pos_list = new int[f_num];

    CpqmAMBUtilities::pqmAmbConvProtToPos(f_prot_list , f_num , l_pos_list) ;

    if (m_pqmpm_amb.AmbPlan.stationOrder != m_pqmpm_amb.AmbPlan.direction[0]) {
        for (int i = 0 ; i < f_num ; i++) {
            m_master_rearrange_struct.m_master_pos_org = l_pos_list[f_num - 1] ;
            m_master_rearrange_struct.m_master_pos_new = l_pos_list[i] ;

            m_pqmptr->WriteToUI(PQM_MSG_AMB_REARRANGE);
        }

        m_pqmpm_amb.AmbPlan.stationOrder = 0 ;	/* Normal */

        for (i = 0; i < (int)(f_num / 2) ; i++) {
            int	work = 	f_prot_list[i] ;
            f_prot_list[i] = f_prot_list[(f_num - 1) - i] ;
            f_prot_list[(f_num - 1) - i] = work ;
        }
    }

    DEL_PTR(l_pos_list);

    return E_NO_ERROR;
}
//****************************Method Header************************************
//Method Name   :AMBMasterScanStart()
//Author        :iGATE
//Purpose       :Start scan of first wait protocol.
//*****************************************************************************
inline void CpqmAMB::AMBMasterScanStart() const
{
    LPCTSTR FUNC_NAME = _T("CpqmAMB::AMBMasterScanStart");
    PQM_SCOPED_TRACE();

    if (!m_pqmptr->GetAcqManager()->GetScanProcedure()->GetIsScanning() &&
        !m_pqmptr->PqmPmIsActive(IS_CFA_DIALOG) &&
        !m_pqmptr->PqmPmIsActive(IS_COIL_INFO_DIALOG) &&
        !m_pqmptr->GetPostProcessFlag()
       ) {

        //m_pqmptr->WriteToUI(AUTO_SCAN_START_FOR_FIRST_WAIT_PROTOCOL);
    }
}
//****************************Method Header************************************
//Method Name   :CreateMPlusSeriesAndGUIProtcol()
//Author        :iGATE
//Purpose       :Create mplus series for SHIM/MAP protocol and add it to GUI.
//*****************************************************************************
bool CpqmAMB::CreateMPlusSeriesAndGUIProtcol(
    const int f_count
)
{
    LPCTSTR FUNC_NAME = _T("CpqmAMB::CreateMPlusSeriesAndGUIProtcol");
    PQM_SCOPED_TRACE();
    CPQMStudy* l_study = m_pqmptr->GetStudy();

    int l_auto_protocol_count = 0 ;
    bool l_create_series_success = true;

    if (m_pqmpm_amb.AmbPlan.shimming)
        l_auto_protocol_count += f_count;

    if (m_pqmpm_amb.AmbPlan.map)
        l_auto_protocol_count += f_count;

    m_shim_map_list.m_list_count = l_auto_protocol_count;
    m_shim_map_list.m_seq_pos = new int[l_auto_protocol_count];
    SequenceList  l_shim_map_list_for_gui;
    CPASCOMSequence* l_seq = NULL;
    int l_counter = 0;

    for (int i = 0; i < f_count  ; i++) {
        if (m_pqmpm_amb.AmbPlan.shimming) {
            if (NULL == (l_seq = CreateMPlusSeriesAndGUIProtcol(m_pqmpm_amb.shimming.prot[i], AMB_SHIMMING_STUDY))) {
                l_create_series_success = false;
                break;
            }

            l_shim_map_list_for_gui.AddTail(l_seq);
            CPqmProtocol* l_protocol = l_study->GetItemFromProtocol(m_pqmpm_amb.shimming.prot[i]);
            m_shim_map_list.m_seq_pos[l_counter++] = l_protocol->GetPosition();
        }

        if (m_pqmpm_amb.AmbPlan.map) {
            if (NULL == (l_seq = CreateMPlusSeriesAndGUIProtcol(m_pqmpm_amb.map.prot[i], AMB_MAP_STUDY))) {
                l_create_series_success = false;
                break;
            }

            l_shim_map_list_for_gui.AddTail(l_seq);
            CPqmProtocol* l_protocol = l_study->GetItemFromProtocol(m_pqmpm_amb.map.prot[i]);
            m_shim_map_list.m_seq_pos[l_counter++] = l_protocol->GetPosition();
        }
    }

    if (l_create_series_success) {
        m_shim_map_list.m_seq_list = &l_shim_map_list_for_gui;
        m_pqmptr->WriteToUI(PQM_MSG_AMB_ADD_SHIM_MAP);

        for (i = 0; i < f_count  ; i++) {
            CPqmProtocol* l_master_prot = l_study->GetItemFromProtocol(m_pqmpm_amb.Master.prot[i]);

            if (!l_master_prot->GetHasParent()) {
                if (m_pqmpm_amb.AmbPlan.shimming) {
                    m_acq_order_for_ui = m_pqmpm_amb.shimming.prot[i];
                    pqmAmbInitPatientPath(l_study->GetItemFromProtocol(m_pqmpm_amb.shimming.prot[i]));
                }

                if (m_pqmpm_amb.AmbPlan.map) {

                    m_acq_order_for_ui = m_pqmpm_amb.map.prot[i];
                    pqmAmbInitPatientPath(l_study->GetItemFromProtocol(m_pqmpm_amb.map.prot[i]));
                }
            }
        }
    }

    DEL_PTR_ARY(m_shim_map_list.m_seq_pos);
    l_shim_map_list_for_gui.RemoveAll();
    return l_create_series_success;
}
//****************************Method Header************************************
//Method Name   :CreateMPlusSeriesAndGUIProtcol()
//Author        :iGATE
//Purpose       :Create mplus series for SHIM/MAP protocol and add it to GUI.
//*****************************************************************************
CPASCOMSequence* CpqmAMB::CreateMPlusSeriesAndGUIProtcol(
    const int f_acq_order,
    const CString& f_file_path
)
{

    LPCTSTR FUNC_NAME = _T("CpqmAMB::CreateMPlusSeriesAndGUIProtcol");
    PQM_SCOPED_TRACE();

    CPQMStudy* l_study = m_pqmptr->GetStudy();
    CPASCOMInterface* l_passcominterface = m_pqmptr->GetPascomInterface();

    CPqmProtocol* l_protocol = l_study->GetItemFromProtocol(f_acq_order); // (f_entry_table.prot[i]) ;

    long l_prot = 0;
    CPASCOMSequence* l_seq_temp = l_passcominterface->GetSingleSequence(f_file_path, l_prot);

    SequenceList list ;
    list.AddTail(l_seq_temp);

    MakeAutoSeries(&list, l_protocol->GetPosition()) ;


    if (list.IsEmpty() || NULL == l_protocol->GetPqmPrMode()) {
        return NULL;
    }

    list.RemoveAll();

    return l_seq_temp;

}

//****************************Method Header************************************
//Method Name   :MakeAutoSeries()
//Author        :iGATE
//Purpose       :Create mplus series for SHIM/MAP protocol.
//*****************************************************************************
void CpqmAMB::MakeAutoSeries(SequenceList* const f_sequence_list,
                             const int f_target_position)const
{
    LPCTSTR FUNC_NAME = _T("CpqmAMB::MakeAutoSeries");
    PQM_SCOPED_TRACE();

    CPQMStudyMgr*  l_study_manager = m_pqmptr->GetStudyManagerPtr();

    if (NULL == l_study_manager) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_study_manager pointer is NULL"));
        return;
    }

    if (NULL == f_sequence_list) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_map_sequence_list points to NULL"));
        return;
    }

    LPCTSTR l_loid = _T("");
    l_study_manager->MakeSeries(f_sequence_list, l_loid , MAP_AUTO_MODE, f_target_position);
    CString seqorder(_T(""));
    seqorder.Format(_T("%d"), l_study_manager->GetSeriesCount());
    BSTR l_bstrseqorder = seqorder.AllocSysString();
    ::SysFreeString(l_bstrseqorder);
}

//****************************Method Header************************************
//Method Name   :GetAMBShimMapArray()
//Author        :iGATE
//Purpose       :Give SHIM/MAP insexes to GUI.
//*****************************************************************************
void CpqmAMB::GetAMBShimMapArray(VARIANT**  f_data)
{
    (*f_data)->byref = &m_shim_map_list;
}

//****************************Method Header************************************
//Method Name   :GetAMBMasterProtocols()
//Author        :iGATE
//Purpose       :Give Master protocol list to GUI.
//*****************************************************************************
void CpqmAMB::GetAMBMasterProtocols(VARIANT**  f_data)
{
    (*f_data)->byref = &m_slave_list;
}

//****************************Method Header************************************
//Method Name   :GetAMBAutoScanItems()
//Author        :iGATE
//Purpose       :Give protocol indexes to UI to set auto scan.
//*****************************************************************************
void CpqmAMB::GetAMBAutoScanItems(int** f_arr_items, int* f_count, BOOL* f_is_auto_scan_on) const
{
    *f_count = m_pqmpm_amb.Master.num;
    *f_is_auto_scan_on = TRUE;
    *f_arr_items = new int[*f_count];
    memcpy(*f_arr_items, &m_pqmpm_amb.Master.prot, (sizeof(int)* m_pqmpm_amb.Master.num));
}
//****************************Method Header************************************
//Method Name   :pqmAbmDiskSpaceCheck()
//Author        :iGATE
//Purpose       :Check disk space for total AMB protocols.
//*****************************************************************************
status_t CpqmAMB::pqmAbmDiskSpaceCheck()
{

    LPCTSTR FUNC_NAME = _T("CpqmAMB::pqmAbmDiskSpaceCheck");
    PQM_SCOPED_TRACE();

    const int n = m_pqmpm_amb.numSlave + m_pqmpm_amb.Master.num + m_pqmpm_amb.shimming.num + m_pqmpm_amb.map.num ;
    int32_t* prot_tbl =  new int32_t[n * sizeof(int32_t)];

    if (NULL == prot_tbl)
        return E_ERROR;

    PqmAmbScanTbl_t* pqmScanTbl = m_PqmAmbScanProt.PqmScanTbl ;

    for (int i = 0 ; i < n ; i++)
        *(prot_tbl + i) = (pqmScanTbl + i)->prot ;

    CCheckSequence l_checksequence(m_pqmptr);
    uint32_t l_amb_image_totalsize = 0;
    status_t l_status = l_checksequence.CheckMultiProtocolDiskSpace(prot_tbl, n, l_amb_image_totalsize) ;

    if (E_NO_ERROR != l_status) {
        char* l_msg = NULL;

        switch (l_status) {
            case PQM_ERROR_HOSTDISK_SPACE:
                l_msg = "IDS_MSG_NO_HOST_SPACE";
                break;

            case PQM_ERROR_ENGINEDISK_SPACE:
                l_msg = "IDS_MSG_NO_ENGINE_SPACE";
                break;

            case PQM_ERROR_OVER_2GB:
                l_msg = "IDS_MSG_OVER_2GB";
                break;

            case PQM_ERROR_IMGSRV_MEMORY:
                l_msg = "IDS_MSG_IMGSRV_MEMORY";
                break;

            case PQM_ERROR_VAP_STUDY:
            default:
                l_msg = "IDS_MSG_VAP_STUDY";
                break;
        }

        CString l_error_String =  CPqmUtilities::GetMultiLingualString(l_msg);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Error Display:") + l_error_String);
        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(l_error_String,
                MPlus::EH::Information,
                _T("pqmAMB.cpp"),
                _T("PQM"));
        pqmAmbSystemError();
        return E_ERROR ;
    }

    DEL_PTR_ARY(prot_tbl);
    l_status = l_checksequence.CheckAMBDiskSpace(l_amb_image_totalsize);

    if (E_ERROR == l_status) {

        CString l_error_String =  CPqmUtilities::GetMultiLingualString(_T("IDS_MSG_NO_LOCALDB_SPACE"));
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Error Display:") + l_error_String);
        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(l_error_String,
                MPlus::EH::Information,
                _T("pqmAMB.cpp"),
                _T("PQM"));
        pqmAmbSystemError();

    }

    return(l_status) ;
}

//****************************Method Header************************************
//Method Name   :GetRearrangeIndex()
//Author        :iGATE
//Purpose       : Give rearrage index to UI.
//*****************************************************************************

void CpqmAMB::GetRearrangeIndex(int* f_protocol_index_to_move, int* f_target_index)const
{
    *f_protocol_index_to_move  =  m_master_rearrange_struct.m_master_pos_org;
    *f_target_index = m_master_rearrange_struct.m_master_pos_new;
}


//****************************Method Header************************************
//Method Name   :pqmAmbMakeDuplicateList()
//Author        :iGATE
//Purpose       :Make slave protocol index list.
//*****************************************************************************
status_t CpqmAMB ::pqmAmbMakeDuplicateList(int* const f_prot_list, const int f_count) const
{
    LPCTSTR FUNC_NAME = _T("CpqmAMB::pqmAmbMakeDuplicateList");
    PQM_SCOPED_TRACE();

    int l_num = 0 ;
    int l = 0, j = 0;
    int* l_slave_protlist = m_pqmpm_amb.SlaveProt ;

    for (int i = 1 ; i < m_pqmpm_amb.AmbPlan.numSegment; i++) {
        if (m_pqmpm_amb.stationOrder == m_pqmpm_amb.AmbPlan.direction[i]) {
            for (j = 0; j < m_pqmpm_amb.AmbPlan.numStation; j++) {
                for (l = 0 ; l < m_pqmpm_amb.AmbPlan.cell[i][j].repeat ; l++)
                    l_slave_protlist[l_num++] = f_prot_list[j] ;
            }

        } else {
            for (j = m_pqmpm_amb.AmbPlan.numStation - 1; j >= 0 ; j--) {
                for (l = 0 ; l < m_pqmpm_amb.AmbPlan.cell[i][j].repeat ; l++)
                    l_slave_protlist[l_num++] = f_prot_list[j] ;

            }
        }
    }

    return E_NO_ERROR;
}

//****************************Method Header************************************
//Method Name   :pqmAmbSortShim_MAP()
//Author        :iGATE
//Purpose       :Sort SHIM/MAP protocols.
//*****************************************************************************
status_t CpqmAMB::pqmAmbSortShim_MAP(int* const f_prot_list, const int f_num) const
{
    LPCTSTR FUNC_NAME = _T("CpqmAMB::pqmAmbSortShim_MAP");
    PQM_SCOPED_TRACE();

    CPQMStudy* l_study = m_pqmptr->GetStudy();
    CVarFieldHandle*	l_vf_handle = l_study->GetVfDBHandle();

    int l_to_pos = 0;
    CPqmProtocol* l_protocol = NULL;
    int	index = 0;  /* MCM0160-00049 */

    for (int i = f_num - 1 ; i >= 0  ; i--) {
        if (m_pqmpm_amb.AmbPlan.shimming) {
            CpqmAMBUtilities::pqmAmbConvProtToPos(&f_prot_list[i] , 1 , &l_to_pos) ;  /*protからposに変換 */
            l_protocol = l_study->GetItemFromProtocol(m_pqmpm_amb.shimming.prot[i]);

            if (NULL == l_protocol)
                return E_ERROR;

            l_study->AMBReArrange(l_protocol->GetPosition() - 1, l_to_pos - 1);
            l_protocol->SetProtocolKind(ProtocolKind_AutoShim);

            if (!l_study->AMBCopyScanModeFrom(l_vf_handle, f_prot_list[i], l_protocol->GetProtocol(), false))
                return E_ERROR;

            /******* ここでScanID情報/AMB Mode をセットする ********/
            CpqmAMBUtilities::pqmAmbModeSet(l_vf_handle, m_pqmpm_amb.shimming.prot[i], VFC_AMB_MODE_SLAVE);
        }

        if (m_pqmpm_amb.AmbPlan.map) {

            if (m_pqmpm_amb.Master_org.prot[0] == m_pqmpm_amb.Master.prot[0]) {
                index = i ;

            } else {
                index = (f_num - 1) - i ;
            }

            CpqmAMBUtilities::pqmAmbConvProtToPos(&f_prot_list[i] , 1 , &l_to_pos) ;  /*protからposに変換 */
            l_protocol = l_study->GetItemFromProtocol(m_pqmpm_amb.map.prot[i]);

            if (NULL == l_protocol)
                return E_ERROR;

            l_study->AMBReArrange(l_protocol->GetPosition() - 1, l_to_pos - 1);

            /*** Copy ScanMode ***/
            if (!l_study->AMBCopyScanModeFrom(l_vf_handle, f_prot_list[i], l_protocol->GetProtocol(), false))
                return E_ERROR;

            /******* ここでScanID情報/AMB Mode をセットする ********/
            l_protocol->SetExeAutoVoice(m_pqmpm_amb.AmbPlan.mapAutoVoice[index]);
            CpqmAMBUtilities::pqmAmbModeSet(l_vf_handle, m_pqmpm_amb.map.prot[i], VFC_AMB_MODE_SLAVE);

        }
    }

    return E_NO_ERROR;
}


//****************************Method Header************************************
//Method Name   :pqmAmbMakeScanTbl()
//Author        :iGATE
//Purpose       : Make scan table.
//*****************************************************************************
status_t CpqmAMB::pqmAmbMakeScanTbl(void)
{
    LPCTSTR FUNC_NAME = _T("CpqmAMB::pqmAmbMakeScanTbl");
    PQM_SCOPED_TRACE();

    CPQMStudy* l_study = m_pqmptr->GetStudy();
    CVarFieldHandle* l_vf_handle = l_study->GetVfDBHandle();

    if ((NULL == l_study) || (NULL == l_vf_handle)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("((NULL == l_study) || (NULL == l_vf_handle))"));
        return E_ERROR;
    }

    int n = (m_pqmpm_amb.numSlave + m_pqmpm_amb.Master.num + m_pqmpm_amb.shimming.num + m_pqmpm_amb.map.num) ;
    PqmAmbScanTbl_t* l_pqmScanTbl = new PqmAmbScanTbl_t[n];

    if (NULL == l_pqmScanTbl) {
        return E_ERROR;
    }

    DEL_PTR_ARY(m_PqmAmbScanProt.PqmScanTbl);

    memset(l_pqmScanTbl, -1 , sizeof(PqmAmbScanTbl_t)*n);

    m_PqmAmbScanProt.PqmScanTbl = l_pqmScanTbl ;
    n = m_pqmpm_amb.Master.num + m_pqmpm_amb.shimming.num + m_pqmpm_amb.map.num ;
    m_PqmAmbScanProt.num = n ;
    m_PqmAmbScanProt.RemainScanTime  = 0 ;
    n = 0 ;
    int	l_prep_flg = 0 ;
    int	l_station_val = 0;
    int	l_scantime = 0;

    amb_t* l_ambplan = &m_pqmpm_amb.AmbPlan ;
    int* l_slavep = m_pqmpm_amb.SlaveProt ;

    for (int seg = 0 ; seg < l_ambplan->numSegment; seg++) {
        for (int station = 0; station < l_ambplan->numStation; station++) {
            if (m_pqmpm_amb.stationOrder != m_pqmpm_amb.AmbPlan.direction[seg])
                l_station_val = (l_ambplan->numStation - 1) - station ;

            else
                l_station_val = station ;

            if (0 == seg) {
                /* Shimming/MAP 処理を入れる */
                if (l_ambplan->shimming) {

                    int l_master_prot = m_pqmpm_amb.Master.prot[station];
                    (l_pqmScanTbl + n)->prot = m_pqmpm_amb.shimming.prot[station];
                    (l_pqmScanTbl + n)->type = PUT_TYPE((AMB_SHIMMING | AMB_SLAVE)) |
                                               PUT_SEG((seg + 1)) |
                                               PUT_STATION((l_station_val + 1)) |
                                               PUT_LOOP(1) ;

                    int l_shim_protocol = m_pqmpm_amb.shimming.prot[station];

                    if (E_ERROR == CpqmAMBUtilities::pqmAmbSetChannelModeForMapShim(l_vf_handle, l_master_prot, l_shim_protocol)) {
                        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                                  _T("pqmAmbSetChannelModeForMapShim failed to set coil channel mode"));
                    }

                    PqmAmbSetMovingCouch((l_pqmScanTbl + n)->prot, l_master_prot);
                    l_scantime = (l_study->GetItemFromProtocol((l_pqmScanTbl + n)->prot))->GetScanTime(); //get scan time for that protocol
                    m_PqmAmbScanProt.RemainScanTime += l_scantime ;
                    n++;

                }

                if (l_ambplan->map) {

                    int l_master_prot = m_pqmpm_amb.Master.prot[station];
                    (l_pqmScanTbl + n)->prot = m_pqmpm_amb.map.prot[station];
                    (l_pqmScanTbl + n)->type = PUT_TYPE((AMB_SPEEDER_MAP | AMB_SLAVE)) |
                                               PUT_SEG((seg + 1)) |
                                               PUT_STATION((l_station_val + 1)) |
                                               PUT_LOOP(1) ;

                    int l_map_protocol = m_pqmpm_amb.map.prot[station];

                    if (E_ERROR == CpqmAMBUtilities::pqmAmbSetChannelModeForMapShim(l_vf_handle, l_master_prot, l_map_protocol)) {
                        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                                  _T("pqmAmbSetChannelModeForMapShim failed to set coil channel mode"));
                    }

                    PqmAmbSetMovingCouch((l_pqmScanTbl + n)->prot, l_master_prot);
                    l_scantime = (l_study->GetItemFromProtocol((l_pqmScanTbl + n)->prot))->GetScanTime();
                    m_PqmAmbScanProt.RemainScanTime += l_scantime ;
                    n++;
                }

                (l_pqmScanTbl + n)->prot = m_pqmpm_amb.Master.prot[station];
                (l_pqmScanTbl + n)->type = PUT_TYPE((AMB_SCAN | AMB_MASTER)) |
                                           PUT_SEG((seg + 1)) |
                                           PUT_STATION((l_station_val + 1)) |
                                           PUT_LOOP(1) ;
                PqmAmbSetMovingCouch((l_pqmScanTbl + n)->prot);
                CpqmAMBUtilities::pqmAmbScanIDSetScan(m_pqmpm_amb.AmbPlan.numSegment, l_vf_handle, (l_pqmScanTbl + n)->prot, (l_pqmScanTbl + n)->type);
                l_scantime = (l_study->GetItemFromProtocol((l_pqmScanTbl + n)->prot))->GetScanTime();
                m_PqmAmbScanProt.RemainScanTime += l_scantime ;
                n++;

                CpqmAMBUtilities::pqmAmbModeSet(l_vf_handle, m_pqmpm_amb.Master.prot[station], VFC_AMB_MODE_MASTER);

            } else {
                /** Slave  **/
                for (int i = 0 ; i < l_ambplan->cell[seg][l_station_val].repeat ; i++) {
                    if (l_ambplan->cell[seg][l_station_val].prep)
                        l_prep_flg = AMB_PREP ;

                    (l_pqmScanTbl + n)->prot = *l_slavep++;
                    (l_pqmScanTbl + n)->type = PUT_TYPE((AMB_SCAN | AMB_SLAVE | l_prep_flg)) |
                                               PUT_SEG((seg + 1)) |
                                               PUT_STATION((l_station_val	+ 1)) |
                                               PUT_LOOP((i + 1)) ;
                    l_scantime = (l_study->GetItemFromProtocol((l_pqmScanTbl + n)->prot))->GetScanTime();
                    m_PqmAmbScanProt.RemainScanTime += l_scantime ;
                    n++;
                    l_prep_flg = 0 ;
                }
            }
        }
    }

    return E_NO_ERROR;
}
//****************************Method Header************************************
//Method Name   :PqmAmbSetMovingCouch()
//Author        :iGATE
//Purpose       :Set couch method flag.
//*****************************************************************************
void CpqmAMB::PqmAmbSetMovingCouch(const int f_prot, const int f_master_prot /* = -1*/)
{
    LPCTSTR FUNC_NAME = _T("CpqmAMB::PqmAmbSetMovingCouch");
    PQM_SCOPED_TRACE();

    if (!m_pqmptr->IsDialogActive(IS_AMB_DIALOG)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("AMB Dialog is Not Active"));
        return ;
    }

    CPQMStudy* l_study = m_pqmptr->GetStudy();
    CPqmProtocol* l_master_protocol = NULL;
    CPqmProtocol* l_protocol = l_study->GetItemFromProtocol(f_prot);

    if (-1 != f_master_prot)
        l_master_protocol = l_study->GetItemFromProtocol(f_master_prot);

    if (NULL == l_protocol) {

        CString str;
        str.Format(_T("Protocol is NULL. Protocol Acqorder %d"), f_prot);

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str);
        return ;
    }

    PqmAmbScanTbl_t* l_prot_tbl = pqmAMBGetScantblFromProt(l_protocol->GetProtocol());

    const int l_seg = GET_SEG((l_prot_tbl->type));
    const int l_station = GET_STATION((l_prot_tbl->type));
    const int l_type = GET_TYPE((l_prot_tbl->type));

    if (1 == l_seg) {
        if ((l_type & AMB_SCAN) == AMB_SCAN) {
            int l_holdScan = l_protocol->GetHoldScan() | VFC_HOLD_COUCH;
            l_protocol->SetHoldScan(l_holdScan);
            l_study->SetProtocolValue(l_study->GetVfDBHandle(),
                                      l_protocol,
                                      K_PR_HOLD, &l_holdScan , sizeof(int32_t),
                                      TRUE, PQM_STS_LIST);



            int l_couchMethodFlg = l_protocol->GetcouchMethodFlg();

            if (VFC_MOVE_COUCH_METHOD_NONE == l_couchMethodFlg)
                l_couchMethodFlg = VFC_MOVE_COUCH_METHOD_PLAN_CENTER_POSI;

            l_protocol->SetcouchMethodFlg(l_couchMethodFlg);
            l_study->SetProtocolValue(l_study->GetVfDBHandle(), l_protocol,
                                      K_PR_MOVE_COUCH_METHOD_FLAG,
                                      &l_couchMethodFlg ,
                                      sizeof(int32_t),
                                      TRUE, PQM_STS_LIST);

        } else {
            const float l_offset = floorf(m_pqmpm_amb.NewPosi[l_station - 1] - l_protocol->GetPqmPrMode()->GetHomePos());
            CpqmAMBUtilities::pqmAmbSetScanOff(l_master_protocol, l_protocol, l_offset);
        }
    }
}

//****************************Method Header************************************
//Method Name   :pqmAMBGetScantblFromProt()
//Author        :iGATE
//Purpose       : Get scan table from protocol acq order.
//*****************************************************************************
PqmAmbScanTbl_t* CpqmAMB::pqmAMBGetScantblFromProt(const int	f_prot)
{
    PqmAmbScanProt_t* l_ScanProtTbl = pqmAmbGetScanProtbl();
    PqmAmbScanTbl_t* l_ScanTbl = l_ScanProtTbl->PqmScanTbl ;

    for (int i = 0; i < l_ScanProtTbl->num ; i++) {
        if ((l_ScanTbl + i)->prot == f_prot) {
            return ((l_ScanTbl + i)) ;
        }
    }

    return NULL ;
}

//****************************Method Header************************************
//Method Name   :pqmAMBGetScantblFromProt()
//Author        :iGATE
//Purpose       :Get AMB scan table .
//*****************************************************************************
inline PqmAmbScanProt_t* CpqmAMB::pqmAmbGetScanProtbl()
{
    return	&m_PqmAmbScanProt;
}


//****************************Method Header************************************
//Method Name   :PqmAmbEachAcquisitionDone()
//Author        :iGATE
//Purpose       :Call on each AMB protocol acquisition done.
//1.Duplicate Masters , 2.Call PqmAmbEachAcquisitionStart for slave protocol.
//3.reset data on last protocol scan done.
//*****************************************************************************
void CpqmAMB::PqmAmbEachAcquisitionDone(CPqmProtocol*	const f_obj, const bool_t	f_seg2_flag)
{
    LPCTSTR FUNC_NAME = _T("CpqmAMB::PqmAmbEachAcquisitionDone");
    PQM_SCOPED_TRACE();

    if (!m_pqmptr->IsDialogActive(IS_AMB_DIALOG)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("IS_AMB_DIALOG is Not Active"));
        return ;
    }

    int	l_type = 0, l_seg = 0, l_station = 0, l_loop = 0;

    //pmObj = PqmPmGetObjectFromTag(K_PM_TYPE_AMB);
    PqmAmbScanProt_t* l_ScanProtTbl = pqmAmbGetScanProtbl();
    PqmAmbScanTbl_t* l_ScanTbl = l_ScanProtTbl->PqmScanTbl ;

    if (0 == l_ScanProtTbl->num || NULL == l_ScanTbl)
        return;


    for (int i = 0; i < l_ScanProtTbl->num ; i++) {

        if ((l_ScanTbl + i)->prot == f_obj->GetProtocol()) {
            (l_ScanTbl + i)->type  |= PUT_TYPE(AMB_SCAN_DONE);
            l_type = (l_ScanTbl + i)->type ;
            l_loop = GET_LOOP(l_type) ;
            l_station = GET_STATION(l_type) ;
            l_seg = GET_SEG(l_type) ;
            break;
        }
    }

    if (i == l_ScanProtTbl->num) {
        /* AMB_SYSERR */
        //			fprintf(stderr, " %s  %d : Can't find protocol(%d)\n", __FILE__,__LINE__,obj->prot);
        /* AMB suspended by the SYSTEM error. */
        CString str;
        str.Format(_T("Can't find protocol Acq_index= %d, Position = %d"), f_obj->GetProtocol(), f_obj->GetPosition());
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str);
    }


    /* End or imaging SEG1? */
    if (m_pqmpm_amb.Master.prot[m_pqmpm_amb.Master.num - 1] == f_obj->GetProtocol()) {
        /*  Subsequent replication protocol SEG2*/
        if (pqmAmbDuplicate(m_pqmpm_amb.SlaveProt, m_pqmpm_amb.numSlave) == E_NO_ERROR) {
            pqmAmbSEG2ScanSetup() ;

        } else
            PqmPmAbortAmb();

    } else if (i == l_ScanProtTbl->num - 1) {
        /*AMB has completed all imaging... */
        PqmAMBScanEndSeg2();
        pqmPmAmbEnd();

    } else if (f_seg2_flag) {
        /*************************************************
        Subsequent processing SEG2
        PQM is ScanDone Status on the Protocol to the individual.
        LOG output the imaging
        	**************************************************/
        CPQMStudy* l_study = m_pqmptr->GetStudy();
        l_study->SetAcqStatus(f_obj, C_PR_STATUS_DONE);
        m_pqmptr->WriteToUI(PQM_MSG_STATUS_DONE);
        CPqmUtilities::UpdateIECInfo(f_obj);
        /**********Check the following imaging **********/
        CPqmProtocol* l_nextObj = l_study->GetNextItem(f_obj);

        if (l_nextObj) {
            l_study->SetWaitToCurrent(l_nextObj);
            m_pqmptr->WriteToUI(PQM_MSG_AMB_SLAVE_SCAN_START);
            m_pqmptr->WriteToUI(PQM_MSG_SRGL_SCANSTART);
            PqmAmbEachAcquisitionStart(l_nextObj, 1);
            m_acqman_ptr->GetScanProcedure()->SetScanProcProtocol(l_nextObj);
        }
    }
}

//****************************Method Header************************************
//Method Name   :UpdateSecondOnwardsSlaveData()
//Author        :iGATE
//Purpose       :
//*****************************************************************************
status_t CpqmAMB::UpdateSecondOnwardsSlaveData()
{
    LPCTSTR FUNC_NAME = _T("CpqmAMB::UpdateSecondOnwardsSlaveData");
    PQM_SCOPED_TRACE();

    PqmAmbScanTbl_t* l_pqmScanTbl = m_PqmAmbScanProt.PqmScanTbl ;
    int l_acq_order = -1;
    CPqmProtocol* l_protocol = NULL ;
    CCheckSequence l_check_seq(m_pqmptr);
    status_t l_return = E_ERROR;
    CPQMStudy* l_study = m_pqmptr->GetStudy();
    int	l_type = 0;
    bool l_b_first_slave = true;

    for (int i = 0 , n = 0 ; i < m_PqmAmbScanProt.num ; i++) {
        l_acq_order = (l_pqmScanTbl + i)->prot;
        l_protocol = l_study->GetItemFromProtocol(l_acq_order);
        l_type = GET_TYPE(((l_pqmScanTbl + i)->type));

        if (!(l_type & AMB_SCAN_DONE)) {
            if (!l_b_first_slave)
                l_return = l_check_seq.UpdateCoilRelatedData(l_protocol);

            l_b_first_slave = false;
        }
    }

    return l_return;
}
//****************************Method Header************************************
//Method Name   :PqmAMBScanSeg2Active()
//Author        :iGATE
//Purpose       :Get seg2 flag TRUE/FALSE
//*****************************************************************************
bool_t	CpqmAMB::PqmAMBScanSeg2Active(void) const
{
    return (m_pqmpm_amb.Amb_status & AMB_SEG2_SCAN) ? TRUE : FALSE;
}

//****************************Method Header************************************
//Method Name   :pqmAmbSEG2ScanSetup()
//Author        :iGATE
//Purpose       :Copy each master prescan data to its corresponding slave protocol.
//*****************************************************************************
status_t CpqmAMB::pqmAmbSEG2ScanSetup(void)
{
    LPCTSTR FUNC_NAME = _T("CpqmAMB::pqmAmbSEG2ScanSetup");
    PQM_SCOPED_TRACE();

    CPQMStudy* l_study = m_pqmptr->GetStudy();
    CVarFieldHandle* l_vfd = l_study->GetVfDBHandle();
    PqmAmbScanTbl_t* l_pqmScanTbl = m_PqmAmbScanProt.PqmScanTbl ;

    int* l_couch_tbl = new int[m_PqmAmbScanProt.num + 1];
    int* l_prot_tbl = new int[m_PqmAmbScanProt.num + 1] ;

    if ((NULL == l_couch_tbl) || (NULL == l_prot_tbl)) {
        return E_ERROR;
    }

    memset(l_couch_tbl, NULL, sizeof(int)*m_PqmAmbScanProt.num + 1);
    memset(l_prot_tbl, NULL, sizeof(int)*m_PqmAmbScanProt.num + 1);

    int	l_from_prot = -1, l_to_prot = -1 ;
    int l_numStation = 0;
    int l_prePosi = -9999 ;
    int l_start_prot = -1 ;
    int	l_seg = 0, l_type = 0;

    VfPathElem_t	l_path[4];
    l_path[0].subtree_name = SVN_ST_PROTOCOL;
    l_path[1].subtree_name = SVN_ST_SUBPROTOCOL;
    l_path[1].index = 0;
    VfFieldSpec_t       l_vfReq[8] = {0};
    CString l_amb_image_label = _T("");
    int l_indx = 0, l_length = 0, l_num = 0, l_loop = 0;
    time_t l_ts;
    status_t l_status;
    char* l_image_label_buffer;

    CPqmProtocol* l_target_protocol = NULL ;
    CPqmProtocol* l_start_obj = NULL;

    for (int i = 0 , n = 0 ; i < m_PqmAmbScanProt.num ; i++) {
        l_to_prot = (l_pqmScanTbl + i)->prot;
        l_target_protocol = l_study->GetItemFromProtocol(l_to_prot);
        // assert(obj) ;

        l_loop = GET_LOOP(((l_pqmScanTbl + i)->type));
        l_numStation = GET_STATION(((l_pqmScanTbl + i)->type)) - 1 ;
        l_seg = GET_SEG(((l_pqmScanTbl + i)->type));
        l_type = GET_TYPE(((l_pqmScanTbl + i)->type));

        if (!(l_type & AMB_SPEEDER_MAP) && !(l_type & AMB_SHIMMING)) {
            l_amb_image_label.Format(_T("AMB%d(%d)/%d"), l_seg, l_loop, m_pqmpm_amb.AmbPlan.numSegment);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_amb_image_label);

            l_length = l_amb_image_label.GetLength() + 1;
            l_image_label_buffer = new char[l_length];
            l_indx = 0;

            memset(l_image_label_buffer, 0, sizeof(char)*l_length);
            wcstombs(l_image_label_buffer, l_amb_image_label.GetBuffer(l_length) , l_length);

            VFF_SET_ARGS(l_vfReq[l_indx], SVN_PRC_AMB_SCAN_LABEL, SVT_PRC_AMB_SCAN_LABEL,
                         l_length, l_image_label_buffer, VFO_REPLACE);

            l_indx++;

            l_path[0].index = l_to_prot;

            l_status = l_vfd->PutFields(l_path, SVD_ST_PROTOCOL, l_vfReq, 1, &l_num, &l_ts);

            if (l_status != E_NO_ERROR) {
                CString l_print_str = _T("");
                l_print_str = CPqmUtilities::GetLibVfErrorStatusString(l_status);
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_print_str);
            }

            DEL_PTR_ARY(l_image_label_buffer);
        }

        if (!(l_type & AMB_SCAN_DONE)) {
            /** 1.preScan等をMasterプロトコルからコピー **/
            l_from_prot = m_pqmpm_amb.Master_org.prot[l_numStation];
            CpqmAMBUtilities::pqmAmbPreScanCopy(l_vfd, l_from_prot, (l_pqmScanTbl + i)->prot);

            CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("PreScan Copy  SEG1Prot[%d] NewProt=[%d]"), l_from_prot, (l_pqmScanTbl + i)->prot);

            /** 2.Plan座標変換 **/
            /** 3.寝台情報更新 **/
            /*** 但し、先頭のプロトコルは通常撮像と同様の処理を
            行うため、座標変換や寝台情報更新等は行ってはいけない。
            ***/
            n++ ;
            l_prot_tbl[n] = l_to_prot ;

            if (-9999 == l_prePosi) {
                l_start_prot = l_to_prot ;
                l_start_obj = l_target_protocol ;
                l_prePosi = (int)m_pqmpm_amb.NewPosi[l_numStation];
                l_couch_tbl[n] = 0 ;

            } else {
                CpqmAMBUtilities::pqmAmbSetOffset(l_vfd, l_target_protocol, m_pqmpm_amb.NewPosi[l_numStation]);
                l_couch_tbl[n] = (int)m_pqmpm_amb.NewPosi[l_numStation] - l_prePosi ;

                CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("AMB n=%d  NewPosi[%d]= %f  prePosi=%d"),
                        n, l_numStation, m_pqmpm_amb.NewPosi[l_numStation], l_prePosi);
            }
        }
    }

    UpdateSecondOnwardsSlaveData();
    /** 5.テーブル作成 **/
    l_prot_tbl[0] = n ;
    l_couch_tbl[0] = n ;
    CpqmAMBUtilities::pqmAmbSetAcqmanTbl(l_vfd, l_prot_tbl[1], l_prot_tbl, l_couch_tbl);

    //Check in Vantage here...
    /*if (NULL == l_couch_tbl || NULL == l_prot_tbl) {*/
    DEL_PTR_ARY(l_couch_tbl);
    DEL_PTR_ARY(l_prot_tbl)
    //return E_ERROR;
    //}

    /***********************
    AMBのSEG2Scan開始
    ************************/
    PqmAMBScanStartSeg2(l_start_obj);

    return E_NO_ERROR ;
}	/* pqmAmbSEG2ScanSetup */



//****************************Method Header************************************
//Method Name   :PqmAMBScanStartSeg2()
//Author        :iGATE
//Purpose       :Set seg2 scan start flag.
//*****************************************************************************
void	CpqmAMB::PqmAMBScanStartSeg2(CPqmProtocol*	const f_prObj)
{

    m_pqmpm_amb.Amb_status |= AMB_SEG2_SCAN;
}

//****************************Method Header************************************
//Method Name   :PqmAMBScanEndSeg2()
//Author        :iGATE
//Purpose       :Set AMB scan end flag.
//*****************************************************************************
inline void	CpqmAMB::PqmAMBScanEndSeg2(void)
{
    m_pqmpm_amb.Amb_status = 0 ;
}

//****************************Method Header************************************
//Method Name   :pqmAmbAcquisitionCheck()
//Author        :iGATE
//Purpose       :Compare current slave protocol data with its master protocol.
//*****************************************************************************
status_t CpqmAMB::pqmAmbAcquisitionCheck(PqmAmbScanTbl_t* const f_ScanTbl)
{
    LPCTSTR FUNC_NAME = _T("CpqmAMB::pqmAmbAcquisitionCheck");
    PQM_SCOPED_TRACE();

    status_t l_sts = E_NO_ERROR;

    if ((GET_TYPE(f_ScanTbl->type) & AMB_MASTER) == NULL) {
        /* Gets the protocol information corresponding SEGMENT1 */
        int l_curr_prot  = f_ScanTbl->prot ;

        int l_Num_station = GET_STATION(f_ScanTbl->type) - 1 ;

        int l_seg1_prot = m_pqmpm_amb.Master_org.prot[l_Num_station] ;

        PqmAmbProtCheckDataT l_seq1Data, l_curData;
        memset(&l_seq1Data , NULL , sizeof(PqmAmbProtCheckDataT)) ;
        memset(&l_curData , NULL , sizeof(PqmAmbProtCheckDataT)) ;

        /* SEGMENT1 Information */
        VfFieldSpec_t	l_seg1Req[20] ;
        CPQMStudy* l_study = m_pqmptr->GetStudy();

        l_sts = CpqmAMBUtilities::pqmAmbGetProtCheckData(l_study->GetVfDBHandle() , l_seg1_prot, l_seg1Req, &l_seq1Data);
        CPqmProtocol* l_obj = l_study->GetItemFromProtocol(l_seg1_prot);
        assert(l_obj);
        CInterLock l_cinterlock(m_acqman_ptr->GetScanProcedure());;

        /* again ask again InterLock information does not exist in the SEG1*/
        bool_t l_enable = false;

        if (m_pqmptr->IsSaturnCoilSystem()) {
            /* Saturn Coil */
            PqmCoil_t*		coil_p;
            l_study->GetTxCoilInSaturn(l_obj->GetCoilDBTable(), &coil_p);
            l_enable = l_cinterlock.GetInterLock(coil_p->coil_id, l_obj->GetPqmPrMode()->GetAnatomy(), m_pqmptr->GetStudy()->GetWeight(), &l_seq1Data.inter_lock)  ;

        } else {
            l_enable = l_cinterlock.GetInterLock(l_obj->GetPqmPrMode()->GetRecvCoilId(), l_obj->GetPqmPrMode()->GetAnatomy(), m_pqmptr->GetStudy()->GetWeight(), &l_seq1Data.inter_lock);

        }

        if (l_enable || (0.0 == l_seq1Data.inter_lock)) {
            l_sts =  E_ERROR;
        }

        /*Here to get the current information. */
        VfFieldSpec_t  l_curReq[20] ;
        l_sts |= CpqmAMBUtilities::pqmAmbGetProtCheckData(l_study->GetVfDBHandle(), l_curr_prot , l_curReq, &l_curData);

        /* Check if the same as current SEG1 */
        if (E_NO_ERROR == l_sts) {
            l_sts = 0 ;

            if (l_seq1Data.fsy_value != l_curData.fsy_value)		{
                l_sts = 0x0001 ;
            }

            if (l_seq1Data.rfl_gain_value != l_curData.rfl_gain_value)	{
                l_sts = 0x0002 ;
            }

            if (l_seq1Data.rfl_scale_value != l_curData.rfl_scale_value) {
                l_sts = 0x0004 ;
            }

            if (l_seq1Data.rgn_value != l_curData.rgn_value)		{
                l_sts = 0x0008 ;
            }

            if (l_seq1Data.TGCRFoutRatio != l_curData.TGCRFoutRatio)	{
                l_sts = 0x0010 ;
            }

            if (l_seq1Data.inter_lock != l_curData.inter_lock)		{
                l_sts = 0x0020 ;
            }

            if (l_seq1Data.shim_flag != l_curData.shim_flag)		{
                l_sts = 0x0040 ;
            }

            if (l_seq1Data.shimFsyFlag != l_curData.shimFsyFlag)	{
                l_sts = 0x0080 ;
            }

            if (l_seq1Data.sar != l_curData.sar)			{
                l_sts = 0x0100 ;
            }

            if (l_seq1Data.rcvCoil != l_curData.rcvCoil)		{
                l_sts = 0x0200 ;
            }

            if (l_seq1Data.trnCoil != l_curData.trnCoil)		{
                l_sts = 0x0400 ;
            }

            /* See spare minute 1mm error because there is an error hard sleeper position */
            if (ABSF(l_seq1Data.couchPosi - l_curData.couchPosi) > 1.0) {
                l_sts = 0x0800 ;
            }

            if (memcmp(l_seq1Data.activeShimPath , l_curData.activeShimPath, sizeof(VfPathElem_t))) {
                l_sts = 0x1000;
            }

            if (memcmp(l_seq1Data.mapPath , l_curData.mapPath, sizeof(VfPathElem_t))) {
                l_sts = 0x2000;
            }

            if (memcmp(&l_seq1Data.offVecVG , &l_curData.offVecVG, sizeof(f32vec3_t))) {
                l_sts = 0x4000;
            }

            if ((l_seq1Data.offVec.x != l_curData.offVec.x) ||
                (l_seq1Data.offVec.y != l_curData.offVec.y) ||
                (ABSF(l_seq1Data.offVec.z - l_curData.offVec.z) > 0.001)) {	/* For offset in the Z direction is considered hard errors */
                l_sts = 0x8000 ;
            }

        } else {
            l_sts = 0xffff;
        }

        if (l_sts) {
            /* Navi にLOG出力する */
            //char	buf[1024] ;
            //XmString	msg;

            /* Make an error is displayed here*/
            CString err_msg = _T("IDS_ERR_AMB_SCAN_COMDITION_ERROR");
            CString l_error_String = CPqmUtilities::GetMultiLingualString(err_msg);
            CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(l_error_String,
                    MPlus::EH::Information, _T("pqmAMB.cpp"), _T("PQM"));

            err_msg.Format(_T("AMB_SCAN error code [0x%x]"), l_sts);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CheckImgGr return E_ERROR"));

            err_msg.Format(_T("AMB_SCAN error code [0x%x]"), l_sts);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, err_msg);

            pqmAmbSystemError();
            l_sts =  E_ERROR;
        }
    }

    return l_sts ;
}

//****************************Method Header************************************
//Method Name   :pqmAmbDuplicate()
//Author        :iGATE
//Purpose       :Make duplicate slave protocols from Masters.
//*****************************************************************************
status_t CpqmAMB::pqmAmbDuplicate(int* f_prot_list, const int f_count)
{

    LPCTSTR FUNC_NAME = _T("CpqmAMB::pqmAmbDuplicate");
    PQM_SCOPED_TRACE();

    int* l_pos_list = new int[sizeof(int)*f_count];

    if (NULL == l_pos_list)
        return E_ERROR ;

    CpqmAMBUtilities::pqmAmbConvProtToPos(f_prot_list , f_count , l_pos_list) ;

    //l_study->GetTopWaitPos(&wait_obj);
    //to_pos = wait_obj->GetPosition();

    PqmAmbScanTbl_t* l_pqmScanTbl = m_PqmAmbScanProt.PqmScanTbl ;
    PqmAmbScanTbl_t* l_slave_protlist = (PqmAmbScanTbl_t*)(l_pqmScanTbl + m_PqmAmbScanProt.num);

    HANDLE  slave_duplicate_event_handle = CreateEvent(NULL, TRUE, TRUE, PQM_AMB_SLAVE_DUPLICATE);
    ResetEvent(slave_duplicate_event_handle);

    m_slave_list.m_master_acqorders_to_duplicate = new int[f_count];
    m_slave_list.m_duplicated_slave_acqorders = new int[f_count];

    memset(m_slave_list.m_duplicated_slave_acqorders, 0, (sizeof(int) * f_count));
    memcpy(m_slave_list.m_master_acqorders_to_duplicate, f_prot_list, (sizeof(int)* f_count));

    m_slave_list.m_count = f_count;

    m_pqmptr->WriteToUI(PQM_MSG_AMB_DUPLICATE_MASTERS);
    WaitForSingleObject(slave_duplicate_event_handle, INFINITE) ;

    for (int i = 0 ; i < f_count ; i++) {
        if (-1 == m_slave_list.m_duplicated_slave_acqorders[i]) {
            DEL_PTR_ARY(l_pos_list);
            DEL_PTR_ARY(m_slave_list.m_duplicated_slave_acqorders);
            DEL_PTR_ARY(m_slave_list.m_master_acqorders_to_duplicate);
            return E_ERROR;
        }

        f_prot_list[i] = m_slave_list.m_duplicated_slave_acqorders[i];
        (l_slave_protlist + i)->prot = f_prot_list[i];
        m_PqmAmbScanProt.num += 1;
    }


    DEL_PTR_ARY(l_pos_list);

    int	l_prep = OFF;
    CPQMStudy* l_study = m_pqmptr->GetStudy();
    CVarFieldHandle* l_vfd = l_study->GetVfDBHandle();
    int l_slave_prep_scan = OFF;

    for (int l = 0 ; l < f_count ; l++) {

        CpqmAMBUtilities::pqmAmbModeSet(l_vfd , (l_slave_protlist + l)->prot, VFC_AMB_MODE_SLAVE);
        CpqmAMBUtilities::pqmAmbScanIDSetScan(m_pqmpm_amb.AmbPlan.numSegment, l_vfd , (l_slave_protlist + l)->prot, (l_slave_protlist + l)->type);

        l_prep = (GET_TYPE(((l_slave_protlist + l)->type)) & AMB_PREP) ? ON : OFF;

        CpqmAMBUtilities::pqmAmbPrepSet(l_vfd , (l_slave_protlist + l)->prot, l_prep) ;

        if (!l_slave_prep_scan && l_prep) {
            l_slave_prep_scan = ON;
        }
    }

    CPqmProtocol* l_protocol = NULL;

    CPqmPmSARManager* l_sar_mgr_ptr = m_pqmptr->GetSARManager();

    if (NULL == l_sar_mgr_ptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_sar_mgr_ptr is NULL"));
        return E_ERROR ;
    }

    sarInfo_t* l_curr_sarinfo = NULL;

    for (int l_index = 0 ; l_index < f_count ; l_index++) {

        l_protocol =  l_study->GetItemFromProtocol((l_slave_protlist + l_index)->prot);

        if (NULL == l_protocol) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_protocol is NULL"));
            break;
        }

        l_curr_sarinfo = l_protocol->GetSarInfo();

        if (NULL == l_curr_sarinfo) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_curr_sarinfo is NULL"));
            break;
        }

        l_curr_sarinfo->fixed_ope_mode = l_slave_prep_scan;


        l_sar_mgr_ptr->ResetSarPreScanParam(l_curr_sarinfo);

        CpqmAMBUtilities::DeleteTagsFromAMBSlaveProt(l_study->GetVfDBHandle(), l_protocol->GetProtocol());
        CpqmAMBUtilities::SetValForSlaveProt(l_protocol);
    }

    l_sar_mgr_ptr->CalcSARControlAndPutVFForAll();

    m_pqmptr->WriteToUI(PQM_MSG_AMB_SLAVE_UPDATE);

    DEL_PTR_ARY(m_slave_list.m_duplicated_slave_acqorders);
    DEL_PTR_ARY(m_slave_list.m_master_acqorders_to_duplicate);

    l_study->GetVfDBHandle()->ReOpen();

    return E_NO_ERROR;
}

//****************************Method Header************************************
//Method Name   :PqmAmbEachAcquisitionStart()
//Author        :iGATE
//Purpose       : Each slave protocol scan start.
//*****************************************************************************
void CpqmAMB::PqmAmbEachAcquisitionStart(CPqmProtocol*	const f_obj, const bool_t	f_pro_check)
{
    LPCTSTR FUNC_NAME = _T("CpqmAMB::PqmAmbEachAcquisitionStart");
    PQM_SCOPED_TRACE();

    PqmAmbScanProt_t*	l_ScanProtTbl = NULL;
    PqmAmbScanTbl_t*		l_prot_tbl = NULL;

    if (m_pqmptr->PqmPmIsActive(K_PM_TYPE_AMB)) {
        l_prot_tbl = pqmAMBGetScantblFromProt(f_obj->GetProtocol());

        /**  Type mechanism put safety check here**/
        if (f_pro_check) {
            if (pqmAmbAcquisitionCheck(l_prot_tbl) != E_NO_ERROR) {
                /***AMB immediately disrupt mismatch conditions because***/
                return ;
            }
        }

        if (l_prot_tbl) {
            l_ScanProtTbl = pqmAmbGetScanProtbl();

            //pmObj = PqmPmGetObjectFromTag(K_PM_TYPE_AMB);
            if (f_obj->GetAcqStatus() == C_PR_STATUS_WAIT ||
                f_obj->GetAcqStatus() == C_PR_STATUS_CURRENT)
                /* The case of a multi-protocol led Acqman, depending on the timing
                  There may be times when I have been to Current */
            {
                /*if( 0L < ScanProtTbl->RemainScanTime )
                	ScanProtTbl->RemainScanTime -= (long)obj->time;
                pqmSendAMBProgress(pmObj, prot_tbl->type, ON) ;*/ //<<TBD>>
                return ;

            } else {
                /* AMB_SYSERR  */
                //fprintf(stderr, " %s  %d PqmAmbEachAcquisitionStart() error\n", __FILE__,__LINE__);
            }
        }

        //}
    }

}
//****************************Method Header************************************
//Method Name   :PqmPmAbortAmb()
//Author        :iGATE
//Purpose       :Handle AMB abort functionality.
//*****************************************************************************
void CpqmAMB::PqmPmAbortAmb(BOOL f_is_dialog_end/* = TRUE*/)
{
    LPCTSTR FUNC_NAME = _T("CpqmAMB::PqmPmAbortAmb");
    PQM_SCOPED_TRACE();

    if (!m_pqmptr->PqmPmIsActive(K_PM_TYPE_AMB)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("K_PM_TYPE_AMB is Not Active"));
        return ;
    }

    CPQMStudy* l_study = m_pqmptr->GetStudy();
    CVarFieldHandle* l_vfd = l_study->GetVfDBHandle();

    PqmAmbScanProt_t* l_ScanProtTbl = pqmAmbGetScanProtbl();
    PqmAmbScanTbl_t* l_prot_tbl = l_ScanProtTbl->PqmScanTbl ;

    if ((0 < l_ScanProtTbl->num) && (NULL != l_prot_tbl)) {

        int	l_type = 0;
        CPqmProtocol* l_protocol = NULL;

        for (int i = 0; i < l_ScanProtTbl->num ; i++) {
            l_type = GET_TYPE(((l_prot_tbl + i)->type)) ;
            l_protocol = l_study->GetProtocol((l_prot_tbl + i)->prot);

            if (l_protocol && !(l_type & AMB_SCAN_DONE)) {
                if (l_protocol->GetAcqStatus() == C_PR_STATUS_CURRENT) {
                    l_study->SetAcqStatus(l_protocol, C_PR_STATUS_FAIL);
                    m_pqmptr->WriteToUI(PQM_MSG_ABORT_FAIL);

                    SetSeriesNoInDB(l_protocol);

                } else if (l_protocol->GetAcqStatus() == C_PR_STATUS_WAIT) {
                    if ((l_type & AMB_MASTER)) {

                        if (l_protocol && l_protocol->GetAcqStatus() == C_PR_STATUS_WAIT) {
                            /* ここではStatusを変換するだけでなく、
                            FAILの場合HCになっていないといけないので
                                座標変換(HH->HC)を行う */
                            if (!l_protocol->GetCouchOffsetFlag()) {
                                CpqmAMBUtilities::pqmAmbSetOffset(l_vfd, l_protocol, (l_protocol->GetPqmPrMode()->GetCouchPos()));

                                CPQMDebugInfo::GetInstance(m_pqmptr)->PqmDebugAcqMonitor(_T("[%2d] %f %f %f "),
                                        l_protocol->GetProtocol(),
                                        l_protocol->GetPqmPrMode()->GetCouchPos(), l_protocol->GetPqmPrMode()->GetHomePos(),
                                        l_protocol->GetPqmPrMode()->GetCouchOffset()/*,obj->couchOffsetflag   %5d */);
                            }

                            l_study->SetAcqStatus(l_protocol, C_PR_STATUS_FAIL);
                            SetAMBIndexToUpdateOrDelete(l_protocol->GetPosition() - 1);

                            SetSeriesNoInDB(l_protocol);
                            m_pqmptr->WriteToUI(PQM_MSG_AMB_UPDATE_STATUS_FAIL);


                        }


                    } else {
                        /** Slave PROTOCOL **/
                        if (l_protocol->GetAcqStatus() == C_PR_STATUS_WAIT) {
                            SetAMBIndexToUpdateOrDelete(l_protocol->GetPosition() - 1);
                            m_pqmptr->WriteToUI(PQM_MSG_AMB_SLAVE_INDEX_DELETE);
                        }
                    }
                }
            }
        }

    } else {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_ScanProtTbl or l_prot_tbl is not yet prepared."));
    }


    if (PqmAMBCheckScanStatus(AMB_GO_SCAN)) {

        CPqmProtocol* l_top_wait_protocol = l_study->GetTopWaitProtocol();

        if (l_top_wait_protocol) {
            CPqmProtocol* l_top_master_protocol = l_study->GetProtocol(m_pqmpm_amb.Master.prot[0]);

            if (l_top_master_protocol && (l_top_wait_protocol->GetPosition() == l_top_master_protocol->GetPosition() - 1)) {
                m_master_rearrange_struct.m_master_pos_org = l_top_wait_protocol->GetPosition();
                m_master_rearrange_struct.m_master_pos_new = m_master_rearrange_struct.m_master_pos_org + m_pqmpm_amb.Master.num ;
                m_pqmptr->WriteToUI(PQM_MSG_AMB_REARRANGE);
            }
        }

        if (!f_is_dialog_end)
            f_is_dialog_end = TRUE;

    }

    m_pqmptr->GetSARManager()->CalcSARControlAndPutVFForAll();

    if (f_is_dialog_end) {
        pqmPmAmbEnd();
        l_study->GetVfDBHandle()->ReOpen();
    }

}

//****************************Method Header************************************
//Method Name   :pqmPmAmbEnd()
//Author        :iGate
//Purpose       :Handle AMB end.
//*****************************************************************************
void CpqmAMB::pqmPmAmbEnd()
{
    LPCTSTR FUNC_NAME = _T("CpqmAMB::pqmPmAmbEnd");
    PQM_SCOPED_TRACE();

    m_pqmptr->WriteToUI(PQM_MSG_AMB_EXIT);
    PqmAMBScanEndSeg2();

    float couch = PQM_COUCH_INIT_POSITION;
    CPqmShimSharedData::GetInstance()->ShimRegistTuneInfo(&couch, &couch, &couch);
    CPqmShimSharedData::GetInstance()->InitShim();

    m_pqmptr->WriteToUI(PQM_MSG_INJECT_TIME_INVISIBLE);

    m_pqmptr->WriteToUI(PQM_MSG_SELECT_FIRST_WAIT_PROTOCOL);
    m_pqmptr->WriteToUI(PQM_MSG_REMOVE_AUTO_SCAN);
}

//****************************Method Header************************************
//Method Name   :PqmAMBCheckScanStatus()
//Author        :iGATE
//Purpose       :Get AMB scan status.
//*****************************************************************************
bool CpqmAMB::PqmAMBCheckScanStatus(const int	f_sts) const
{
    return (m_pqmpm_amb.Amb_status & f_sts) ? true : false ;
}

//****************************Method Header************************************
//Method Name   :PqmAmbDeleteSlave()
//Author        :iGATE
//Purpose       :Delete AMB wait protocols on load study .
//*****************************************************************************
void CpqmAMB::PqmAmbDeleteSlave()
{
    LPCTSTR FUNC_NAME = _T("CpqmAMB::PqmAmbDeleteSlave");
    PQM_SCOPED_TRACE();

    protocolvec l_protocolvector = m_pqmptr->GetStudy()->GetProtocolvector();
    protocolvec::iterator v1_Iter  = NULL;

    CPqmProtocol*	l_protocol = NULL;

    for (v1_Iter = l_protocolvector.begin(); v1_Iter != l_protocolvector.end(); v1_Iter++) {

        l_protocol = *v1_Iter;

        if (VFC_AMB_MODE_SLAVE == l_protocol->GetAMBModeFlag() && C_PR_STATUS_WAIT == l_protocol->GetAcqStatus()) {
            l_protocol = *v1_Iter;
            SetAMBIndexToUpdateOrDelete(l_protocol->GetPosition() - 1);
            m_pqmptr->WriteToUI(PQM_MSG_AMB_SLAVE_INDEX_DELETE);
        }
    }
}

//****************************Method Header************************************
//Method Name   :PqmAmbDeleteAutoShimMapFromVarfieldOnly()
//Author        :iGATE
//Purpose       :Delete AMB Slave wait protocols from Varfield Study
//*****************************************************************************
void CpqmAMB::PqmAmbDeleteAutoShimMapFromVarfieldOnly()
{
    LPCTSTR FUNC_NAME = _T("CpqmAMB::PqmAmbDeleteAutoShimMapFromVarfieldOnly");
    PQM_SCOPED_TRACE();

    CPQMStudy* l_study = m_pqmptr->GetStudy();
    CPqmProtocol*	l_protocol = NULL;

    if (m_pqmpm_amb.AmbPlan.shimming) {
        for (int i = 0; i < m_pqmpm_amb.shimming.num; i++) {
            l_protocol = l_study->GetProtocol(m_pqmpm_amb.shimming.prot[i]);

            if (l_protocol) {
                m_pqmptr->GetStudy()->DeleteProt(l_protocol->GetProtocol());
            }
        }
    }

    if (m_pqmpm_amb.AmbPlan.map) {
        for (int i = 0; i < m_pqmpm_amb.map.num; i++) {
            l_protocol = l_study->GetProtocol(m_pqmpm_amb.map.prot[i]);

            if (l_protocol) {
                m_pqmptr->GetStudy()->DeleteProt(l_protocol->GetProtocol());
            }
        }
    }
}

//****************************Method Header************************************
//Method Name   :GetAMBIndexToUdpate()
//Author        :iGATE
//Purpose       :Get index to update protocol by GUI.
//*****************************************************************************
int CpqmAMB::GetAMBIndexToUdpate() const
{
    return m_index_to_update_or_delete;
}
//****************************Method Header************************************
//Method Name   :SetAMBIndexToUpdateOrDelete()
//Author        :iGATE
//Purpose       :Set protocol index for GUI..
//*****************************************************************************
inline void CpqmAMB::SetAMBIndexToUpdateOrDelete(const int f_index)
{
    m_index_to_update_or_delete = f_index;
}
//****************************Method Header************************************
//Method Name   :PqmAmbPrepStart()
//Author        :iGATE
//Purpose       :Set prep start.
//*****************************************************************************
void CpqmAMB::PqmAmbPrepStart(void)
{
    /* ここでAMB処理のStatusを中にする */
    if (m_pqmptr->PqmPmIsActive(K_PM_TYPE_AMB) && (PqmAMBCheckScanStatus(AMB_GO_SCAN))) {
        m_pqmpm_amb.Amb_status |= AMB_PREP_SCAN;
        m_pqmptr->WriteToUI(PQM_MSG_AMB_SCAN_WAIT);
    }

}
//****************************Method Header************************************
//Method Name   :PqmAmbPrepEnd()
//Author        :iGATE
//Purpose       :Set prep end status.
//*****************************************************************************
void CpqmAMB::PqmAmbPrepEnd(void)
{
    /* ここでAMB処理のStatusからPrepをはずす */
    if (m_pqmptr->PqmPmIsActive(K_PM_TYPE_AMB)) {
        m_pqmpm_amb.Amb_status ^= AMB_PREP_SCAN;
    }
}

//****************************Method Header************************************
//Method Name   :PqmAMBVisualPrepEnd()
//Author        :iGATE
//Purpose       :Set prep end flag to scan table.
//*****************************************************************************
void CpqmAMB::PqmAMBVisualPrepEnd(const int f_prot)
{
    LPCTSTR FUNC_NAME = _T("CpqmAMB::PqmAMBVisualPrepEnd");
    PQM_SCOPED_TRACE();

    PqmAmbScanTbl_t* l_ScanTbl = pqmAMBGetScantblFromProt(f_prot);

    if (l_ScanTbl) {
        int l_type = GET_TYPE(l_ScanTbl->type) ;
        l_type ^= AMB_PREP ;
        l_type = PUT_TYPE(l_type) | (l_ScanTbl->type & ~TYPE_MASK) ;
        //pqmSendAMBProgress(pmObj, type, ON) ;
    }
}


//****************************Method Header************************************
//Method Name   :GetAMBSlaveCount()
//Author        :iGATE
//Purpose       :Get slave count by GUI.
//*****************************************************************************
void CpqmAMB::GetAMBSlaveCount(int* f_count) const
{
    *f_count = m_pqmpm_amb.numSlave;
}

//****************************Method Header************************************
//Method Name   :PqmPqAMBSlaveLost()
//Author        :iGATE
//Purpose       :Filter AMB slave protocol from give list.
//*****************************************************************************
status_t CpqmAMB::PqmPqAMBSlaveLost(int* const f_selected_list , int* const f_selected_count) const
{
    LPCTSTR FUNC_NAME = _T("CpqmAMB::PqmPqAMBSlaveLost");
    PQM_SCOPED_TRACE();

    if ((NULL == f_selected_list) || (0 == (*f_selected_count))) return FALSE;

    CPQMStudy* l_study = m_pqmptr->GetStudy();

    CPqmProtocol*	 l_protocol = NULL;
    status_t	l_sts = FALSE ;

    for (int i = 0 , j = 0 ; i < *f_selected_count ; i++) {

        l_protocol = l_study->GetItemFromPosition(*(f_selected_list + i) + 1);
        assert(obj != NULL);

        if (VFC_AMB_MODE_SLAVE == l_protocol->GetAMBModeFlag()) {
            l_sts = TRUE ;

        } else {
            *(f_selected_list + j) = *(f_selected_list + i);
            j++;
        }
    }

    *f_selected_count = j ;

    return l_sts ;
}

//****************************Method Header************************************
//Method Name   :SetSeriesNoInDB()
//Author        :iGATE
//Purpose       :Set series no in DB.
//*****************************************************************************
void CpqmAMB::SetSeriesNoInDB(CPqmProtocol* f_protocol) const
{
    LPCTSTR FUNC_NAME = _T("CpqmAMB::SetSeriesNoInDB");
    PQM_SCOPED_TRACE();

    if (!f_protocol->GetSeriesNumInDbFlag()) {

        if (E_ERROR == m_pqmptr->GetDbsaStudyMgr()->SetSeriesNoInDB((LPCTSTR)f_protocol->GetSeriesLOID(), f_protocol->GetAcqStatus())) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed to set series in DB"));

        } else {
            f_protocol->SetSeriesNumInDbFlag(TRUE);
        }
    }
}

int CpqmAMB::GetIndexForParentImageRemoved() const
{
    return m_acq_order_for_ui;
}
//****************************Method Header************************************
//Method Name   :CanPerformPrep()
//Author        :iGATE
//Purpose       :
//*****************************************************************************
bool CpqmAMB::CanPerformPrep(
) const
{

    LPCTSTR FUNC_NAME = _T("CpqmAMB::CanPerformPrep");
    PQM_SCOPED_TRACE();

    if (!m_pqmptr->PqmPmIsActive(K_PM_TYPE_AMB) || (m_pqmpm_amb.Master.num <= 0)) {
        return true;
    }

    if ((!PqmAMBCheckScanStatus(AMB_GO_SCAN))) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Allow prep as AMB scan has not started"));
        return true;
    }

    CScanProcedure* l_scan_procedure = m_pqmptr->GetAcqManager()->GetScanProcedure();
    CPqmProtocol* curr_protocol = l_scan_procedure->GetScanProcProtocol();

    if (NULL == curr_protocol) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Error. No current protocol"));
        return false;
    }

    CPQMStudy* l_study = m_pqmptr->GetStudy();
    CPqmProtocol* l_first_master  = l_study->GetItemFromProtocol(m_pqmpm_amb.Master.prot[0]);

    const bool l_return_val = (curr_protocol->GetPosition() < l_first_master->GetPosition());

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_return_val ? _T("Allow Prep") : _T("Don't Allow"));

    return l_return_val;
}
//****************************Method Header************************************
//Method Name   :SetAMBScanLabel()
//Author        :iGATE
//Purpose       :Set AMB scan label for master protocol.
//*****************************************************************************
void CpqmAMB::SetAMBScanLabel()
{

    LPCTSTR FUNC_NAME = _T("CpqmAMB::SetAMBScanLabel");
    PQM_SCOPED_TRACE();

    CPQMStudy* l_study = m_pqmptr->GetStudy();
    CVarFieldHandle* l_vfd = l_study->GetVfDBHandle();
    PqmAmbScanTbl_t* l_pqmScanTbl = m_PqmAmbScanProt.PqmScanTbl ;


    int	l_from_prot = -1, l_to_prot = -1 ;
    int l_numStation = 0;
    int	l_seg = 0, l_type = 0;

    VfPathElem_t	l_path[4];
    l_path[0].subtree_name = SVN_ST_PROTOCOL;
    l_path[1].subtree_name = SVN_ST_SUBPROTOCOL;
    l_path[1].index = 0;
    VfFieldSpec_t       l_vfReq[8] = {0};
    CString l_amb_image_label = _T("");
    int l_indx = 0, l_length = 0, l_num = 0, l_loop = 0;
    time_t l_ts;
    status_t l_status;
    char* l_image_label_buffer;

    CPqmProtocol* l_target_protocol = NULL ;


    for (int i = 0 ; i < m_PqmAmbScanProt.num ; i++) {
        l_to_prot = (l_pqmScanTbl + i)->prot;
        l_target_protocol = l_study->GetItemFromProtocol(l_to_prot);


        l_loop = GET_LOOP(((l_pqmScanTbl + i)->type));
        l_numStation = GET_STATION(((l_pqmScanTbl + i)->type)) - 1 ;
        l_seg = GET_SEG(((l_pqmScanTbl + i)->type));
        l_type = GET_TYPE(((l_pqmScanTbl + i)->type));

        if ((l_type & AMB_MASTER) && !(l_type & AMB_SPEEDER_MAP) && !(l_type & AMB_SHIMMING)) {
            l_amb_image_label.Format(_T("AMB%d(%d)/%d"), l_seg, l_loop, m_pqmpm_amb.AmbPlan.numSegment);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_amb_image_label);

            l_length = l_amb_image_label.GetLength() + 1;
            l_image_label_buffer = new char[l_length];
            l_indx = 0;

            memset(l_image_label_buffer, 0, sizeof(char)*l_length);
            wcstombs(l_image_label_buffer, l_amb_image_label.GetBuffer(l_length) , l_length);

            VFF_SET_ARGS(l_vfReq[l_indx], SVN_PRC_AMB_SCAN_LABEL, SVT_PRC_AMB_SCAN_LABEL,
                         l_length, l_image_label_buffer, VFO_REPLACE);

            l_indx++;

            l_path[0].index = l_to_prot;

            l_status = l_vfd->PutFields(l_path, SVD_ST_PROTOCOL, l_vfReq, 1, &l_num, &l_ts);

            if (l_status != E_NO_ERROR) {
                CString l_print_str = _T("");
                l_print_str = CPqmUtilities::GetLibVfErrorStatusString(l_status);
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_print_str);
            }

            DEL_PTR_ARY(l_image_label_buffer);
        }

    }
}
//****************************Method Header************************************
//Method Name   :UpdatePrepForAMBMaster()
//Author        :iGATE
//Purpose       :Check and update PREP_SCAN tag for master protocols.
//*****************************************************************************
void CpqmAMB::UpdatePrepForAMBMaster()
{
    LPCTSTR FUNC_NAME = _T("CpqmAMB::UpdatePrepForAMBMaster");
    PQM_SCOPED_TRACE();

    amb_t* l_ambplan = &m_pqmpm_amb.AmbPlan;
    CPQMStudy* l_study = m_pqmptr->GetStudy();
    CVarFieldHandle* l_vfd = l_study->GetVfDBHandle();

    const int l_segment = 2;

    for (int l_station = 1; l_station <= l_ambplan->numStation; l_station++) {

        if (l_ambplan->cell[l_segment - 1][l_station - 1].prep) {
            return;
        }
    }

    CPqmProtocol* l_master_protocol = NULL;

    for (l_station = 1; l_station <= l_ambplan->numStation; l_station++) {

        l_master_protocol  = l_study->GetItemFromProtocol(m_pqmpm_amb.Master.prot[l_station - 1]);

        if (NULL == l_master_protocol) {
            break;
        }

        if (l_master_protocol->GetPrepScan()) {
            CString l_log_msg(_T(""));
            l_log_msg.Format(_T("Master Protocol - Prot Index(%d) Prep Scan changing to OFF"));
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_msg);

            CpqmAMBUtilities::pqmAmbPrepSet(l_vfd, m_pqmpm_amb.Master.prot[l_station - 1], OFF);
            l_master_protocol->GetSarInfo()->fixed_ope_mode = 0;
        }
    }
}
// pqmAMBUtilities.cpp: implementation of the CpqmAMBUtilities class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pqmAMBUtilities.h"

#include <libStudyManager/Log/ScopedFuncInOutLog.h>
#include <libStudyManager/strcnv.h>
#include <tami/pqm/pqmAmbMsg.h>
#include <tami/libms/msFtMatrix.h>
#include <PQM/PQMLogMgr.h>
#include <PQM/PqmUtilities.h>

#include "pqm.h"
#include "PQMStudyMgr.h"
#include "ScanProcedure.h"
#include "pqmpmacqman.h"
#include "VarFieldHandle.h"
#include "PqmProtocol.h"
#include "pqmAMBConstants.h"
#include "GPLibAdapter.h"
#include "PQMDebugInfo.h"
#include "InterLock.h"
#include "PqmCoilData.h"
#include "math.h"
#include "couchconfig.h"


using namespace MR::ACQ::SM::COMMON;

CPqm* CpqmAMBUtilities::m_pqm = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CpqmAMBUtilities::CpqmAMBUtilities()
{

}

CpqmAMBUtilities::~CpqmAMBUtilities()
{

}
/*****************************************************************
AMBパラメータをStudyに格納する。
以下の３種類に分ける。
　VftAmbParam_t:AMBパラメータ
　VftAmbCell_t: Cell情報
　VftAmbPostProc_t: 連動処理
******************************************************************/
status_t	CpqmAMBUtilities::pqmAmbParamPutStudy(CVarFieldHandle*	const f_vf_handle, amb_t* const	f_amb_plan, Entrytbl_t* const f_prot_tbl)
{
    LPCTSTR FUNC_NAME = _T("CpqmAMBUtilities::pqmAmbParamPutStudy");
    PQM_SCOPED_TRACE();

    VftAmbParam_t		l_VftAmbParam;
    VftAmbCell_t		l_VftAmbCell;
    VftAmbPostProc_t	l_VftAmbPostProc;
    VfPathElem_t	l_path[4];
    VfFieldSpec_t	l_vfReq[6];
    time_t	ts;

    l_path[0].subtree_name = SVN_ST_PROTOCOL;
    l_path[0].index = f_prot_tbl->prot[0] ;
    l_path[1].subtree_name = SVN_ST_SUBPROTOCOL;
    l_path[1].index = 0;

    memset(&l_VftAmbParam , NULL , sizeof(l_VftAmbParam)) ;
    memset(&l_VftAmbCell , NULL , sizeof(l_VftAmbCell)) ;
    memset(&l_VftAmbPostProc , NULL , sizeof(l_VftAmbPostProc)) ;

    /*** VftAmbParam copy ***/
    l_VftAmbParam.numStation = f_amb_plan->numStation ;
    l_VftAmbParam.numSegment = f_amb_plan->numSegment ;
    l_VftAmbParam.stationOrder = f_amb_plan->stationOrder ;
    l_VftAmbParam.shimming = f_amb_plan->shimming ;
    l_VftAmbParam.map = f_amb_plan->map ;
    memcpy(l_VftAmbParam.direction, f_amb_plan->direction, sizeof(l_VftAmbParam.direction)) ;
    memcpy(l_VftAmbParam.mapAutoVoice, f_amb_plan->mapAutoVoice, sizeof(l_VftAmbParam.mapAutoVoice)) ;

    /*** VftAmbCell Copy ***/
    memcpy(&l_VftAmbCell , f_amb_plan->cell , sizeof(l_VftAmbCell));

    /*** VftAmbPostProc Copy ***/
    memcpy(&l_VftAmbPostProc , &f_amb_plan->postProcessing , sizeof(l_VftAmbPostProc));

    int n = 0 , l_numdone = 0;
    status_t	status = E_ERROR;

    for (int i = 0 ; i < f_prot_tbl->num ; i++) {
        n = 0 ;
        memset(l_vfReq , NULL , sizeof(l_vfReq)) ;
        /************* AUTO MIP **************/
        VFF_SET_ARGS(l_vfReq[n], SVN_PLN_AMB_PARAM, SVT_PLN_AMB_PARAM,
                     sizeof(l_VftAmbParam), &l_VftAmbParam , VFO_REPLACE);
        n++;

        VFF_SET_ARGS(l_vfReq[n], SVN_PLN_AMB_CELL, SVT_PLN_AMB_CELL,
                     sizeof(l_VftAmbCell), &l_VftAmbCell , VFO_REPLACE);
        n++;

        VFF_SET_ARGS(l_vfReq[n], SVN_PLN_AMB_POSTPROC, SVT_PLN_AMB_POSTPROC,
                     sizeof(l_VftAmbPostProc), &l_VftAmbPostProc, VFO_REPLACE);
        n++;

        l_path[0].index = f_prot_tbl->prot[i] ;

        status =  f_vf_handle->PutFields(l_path, SVD_ST_PROTOCOL, l_vfReq, n, &l_numdone, &ts);

        //status = VfPutFields(f_vf_handle,path,SVD_ST_PROTOCOL,vfReq,n,&numDone, &ts);
        if (status != E_NO_ERROR && n != l_numdone) {
            return E_ERROR;
        }

    }

    return E_NO_ERROR;
}
//****************************Method Header************************************
//Method Name   :pqmAmbParamGetStudy()
//Author        :iGATE
//Purpose       :Read AMB Parameters from study.
//*****************************************************************************
status_t CpqmAMBUtilities::pqmAmbParamGetStudy(CVarFieldHandle* const f_vf_handle, amb_t* const f_amb_plan, const int prot)
{

    LPCTSTR FUNC_NAME = _T("CpqmAMBUtilities::pqmAmbParamGetStudy");
    PQM_SCOPED_TRACE();

    VftAmbParam_t		l_VftAmbParam;
    VftAmbCell_t		l_VftAmbCell;
    VftAmbPostProc_t	l_VftAmbPostProc;

    VfPathElem_t	l_path[4];
    VfFieldSpec_t	l_vfReq[6];

    l_path[0].subtree_name = SVN_ST_PROTOCOL;
    l_path[0].index = prot ;
    l_path[1].subtree_name = SVN_ST_SUBPROTOCOL;
    l_path[1].index = 0;

    memset(f_amb_plan , NULL , sizeof(*f_amb_plan)) ;
    memset(&l_VftAmbParam , NULL , sizeof(l_VftAmbParam)) ;
    memset(&l_VftAmbCell , NULL , sizeof(l_VftAmbCell)) ;
    memset(&l_VftAmbPostProc , NULL , sizeof(l_VftAmbPostProc)) ;

    int n = 0 , l_numdone = 0 ;
    memset(l_vfReq , NULL , sizeof(l_vfReq)) ;
    /************* AUTO MIP **************/
    VFF_SET_ARGS(l_vfReq[n], SVN_PLN_AMB_PARAM, SVT_PLN_AMB_PARAM,
                 sizeof(l_VftAmbParam), &l_VftAmbParam , NULL);
    n++;

    VFF_SET_ARGS(l_vfReq[n], SVN_PLN_AMB_CELL, SVT_PLN_AMB_CELL,
                 sizeof(l_VftAmbCell), &l_VftAmbCell , NULL);
    n++;

    VFF_SET_ARGS(l_vfReq[n], SVN_PLN_AMB_POSTPROC, SVT_PLN_AMB_POSTPROC,
                 sizeof(l_VftAmbPostProc), &l_VftAmbPostProc, NULL);
    n++;

    //status = VfGetFields(f_vf_handle,path,SVD_ST_PROTOCOL,vfReq,n,&numDone);
    status_t l_status = f_vf_handle->GetFields(l_path, SVD_ST_PROTOCOL, l_vfReq, n, &l_numdone);

    if (l_status != E_NO_ERROR ||
        l_vfReq[0].status != E_NO_ERROR ||
        l_vfReq[1].status != E_NO_ERROR ||
        l_vfReq[2].status != E_NO_ERROR ||
        n != l_numdone) {

        return E_ERROR;
    }

    /*** VftAmbParam copy ***/
    f_amb_plan->numStation = l_VftAmbParam.numStation ;
    f_amb_plan->numSegment = l_VftAmbParam.numSegment ;
    f_amb_plan->stationOrder = l_VftAmbParam.stationOrder ;
    f_amb_plan->shimming = l_VftAmbParam.shimming ;
    f_amb_plan->map = l_VftAmbParam.map ;
    memcpy(f_amb_plan->direction, l_VftAmbParam.direction, sizeof(l_VftAmbParam.direction)) ;
    memcpy(f_amb_plan->mapAutoVoice, l_VftAmbParam.mapAutoVoice, sizeof(l_VftAmbParam.mapAutoVoice)) ;

    /*** VftAmbCell Copy ***/
    memcpy(f_amb_plan->cell , &l_VftAmbCell ,  sizeof(l_VftAmbCell));

    /*** VftAmbPostProc Copy ***/
    memcpy(&f_amb_plan->postProcessing , &l_VftAmbPostProc , sizeof(l_VftAmbPostProc));

    return E_NO_ERROR;
}
//****************************Method Header************************************
//Method Name   :pqmAmbMatrixCheck()
//Author        :iGATE
//Purpose       :checking of FOV range.
//*****************************************************************************
status_t CpqmAMBUtilities::pqmAmbMatrixCheck(CVarFieldHandle* const handle, const int32_t protocol_index)
{
    LPCTSTR FUNC_NAME = _T("CpqmAMBUtilities::pqmAmbMatrixCheck");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("trace"));

    int32_t	peFtMatrix = 0;
    int32_t	roFtMatrix = 0;

    try {
        status_t sts = handle->ReOpen();

        if (E_NO_ERROR != sts) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ReOpen failed"));
            return sts;
        }

        return ::msCheckFtMatrix(const_cast<char*>(handle->GetName().c_str()),
                                 protocol_index, &peFtMatrix, &roFtMatrix);

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("EXCEPTION"));
        throw;
    }

    return	0 ;
}
//****************************Method Header************************************
//Method Name   :pqmAmbCheckGDC_LocatorImage()
//Author        :iGATE
//Purpose       :Write Locate image flag for each master.
//*****************************************************************************
void CpqmAMBUtilities::pqmAmbCheckGDC_LocatorImage(CVarFieldHandle*	const f_vf_handle, Entrytbl_t* const f_prot_tbl)
{
    LPCTSTR FUNC_NAME = _T("CpqmAMBUtilities::pqmAmbCheckGDC_LocatorImage");
    PQM_SCOPED_TRACE();

    VfPathElem_t	l_path[4];

    l_path[0].subtree_name = SVN_ST_PROTOCOL;
    l_path[0].index = f_prot_tbl->prot[0] ;
    l_path[1].subtree_name = SVN_ST_SUBPROTOCOL;
    l_path[1].index = 0;

    int l_numDone = 0 ;
    status_t  l_status = E_ERROR ;
    VfFieldSpec_t	l_vfReq[3];
    time_t	l_ts = 0;
    int32_t	l_locator_image_flg = 0 ;

    for (int i = 0 ; i < f_prot_tbl->num ; i++) {
        l_locator_image_flg = 0 ;	/* 位置決め画像登録しない */
        l_path[0].index = f_prot_tbl->prot[i] ;

        VFF_SET_ARGS(l_vfReq[0], SVN_PRC_GDC_LOCATOR_IMAGE, SVT_PRC_GDC_LOCATOR_IMAGE,
                     sizeof(l_locator_image_flg), &l_locator_image_flg , VFO_REPLACE);

        l_status = f_vf_handle->PutFields(l_path, SVD_ST_PROTOCOL, l_vfReq, 1, &l_numDone, &l_ts);

        if (l_status != E_NO_ERROR && 1 != l_numDone) {
            CString str;
            str.Format(_T("[AMB] VfPutFields(SVN_PRC_GDC_LOCATOR_IMAGE) error /1.%d "), f_prot_tbl->prot[i]);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str);
        }

    }
}

//****************************Method Header************************************
//Method Name   :pqmAmbAutoScanSet()
//Author        :iGATE
//Purpose       :Set auto scan flag to study for given protocol.
//*****************************************************************************
void CpqmAMBUtilities::pqmAmbAutoScanSet(CVarFieldHandle* const f_vf_handle, const int f_prot, CPqmProtocol* const f_protocol , int f_hold_scan)
{
    LPCTSTR FUNC_NAME = _T("CpqmAMBUtilities::pqmAmbAutoScanSet");
    PQM_SCOPED_TRACE();

    VfPathElem_t	l_path[4];
    VfFieldSpec_t	l_vfReq[4];
    int			l_num = 0 ;
    time_t		l_ts = 0;

    l_path[0].subtree_name = SVN_ST_PROTOCOL;
    l_path[0].index = f_prot;
    l_path[1].subtree_name = SVN_ST_SUBPROTOCOL;
    l_path[1].index = 0;

    f_protocol->SetHoldScan(f_hold_scan);
    /******* ここでセットする ********/
    VFF_SET_ARGS(l_vfReq[0], SVN_PLN_HOLD, SVT_PLN_HOLD,
                 sizeof(f_hold_scan), &f_hold_scan, VFO_REPLACE);
    f_vf_handle->PutFields(l_path, SVD_ST_PROTOCOL, l_vfReq, 1, &l_num, &l_ts);
}

//****************************Method Header************************************
//Method Name   :pqmAmbStartError()
//Author        :iGATE
//Purpose       :Display error message in error view based on error code.
//*****************************************************************************
void CpqmAMBUtilities::pqmAmbStartError(const int	f_code)
{
    LPCTSTR FUNC_NAME = _T("CpqmAMBUtilities::pqmAmbStartError");

#define PROC(n) {PQM_AMB_##n, "IDS_PQM_AMB_" # n}

    static struct {
        int             code;
        char*           literal;
    } dialogTbl[] = {
        PROC(SEQ_COPY),
        PROC(PAS_REGI),
        PROC(START_NONPLAN),
        PROC(START_MOVE_COUCH),
        PROC(START_DISABLE_SEQ),
        PROC(START_BAD_PARAMETER),
        PROC(START_SELECT_SEQ),
        PROC(START_SELECT_SEQ_OVER),
        PROC(START_ILLEGAL_PARENT),
        PROC(START_MATRIX_OVER),
        PROC(START_COUCH_OVER),
        PROC(START_PREP_NONPLAN),
        PROC(START_PROTOCOL_OVER),
        PROC(START_RECIVED_COIL),
        PROC(START_WITH_CDS),
        PROC(ILLEGAL_INFOMATION),
        PROC(SYSTEM_ERROR),
    };
#undef PROC
    DB_FUNC_ENTER("[pqmAmbError]:");

    DB_PRINTF("A", ("code=%d\n", f_code));

    for (register int i = 0; i < sizeof(dialogTbl) / sizeof(*dialogTbl); i++) {
        if (f_code == dialogTbl[i].code) {

            CString l_error_String =  CPqmUtilities::GetMultiLingualString(CString(dialogTbl[i].literal));
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Error Display:") + l_error_String);

            CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(l_error_String,
                    MPlus::EH::Information,
                    _T("pqmAMB.cpp"),
                    _T("PQM"));

            break ;
        }
    }
}


//****************************Method Header************************************
//Method Name   :pqmAmbGetProtCheckData()
//Author        :iGATE
//Purpose       :Get protocol check dada from study.
//*****************************************************************************
status_t CpqmAMBUtilities::pqmAmbGetProtCheckData(CVarFieldHandle* const f_vfd, const int f_prot, VfFieldSpec_t* const fvfReq, PqmAmbProtCheckDataT* const f_tbl)
{
    LPCTSTR FUNC_NAME = _T("CpqmAMBUtilities::pqmAmbGetProtCheckData");
    PQM_SCOPED_TRACE();

    VfPathElem_t	l_path[4];
    l_path[0].subtree_name = SVN_ST_PROTOCOL;
    l_path[0].index = f_prot;
    l_path[1].subtree_name = SVN_ST_SUBPROTOCOL;
    l_path[1].index = 0;

    if (!f_vfd)	return E_ERROR;

    int n = 0 ;
    /*********** PRESCAN ************/
    VFF_SET_ARGS(fvfReq[A__PRESCAN_FSY_VALUE], SVN_EXE_PRESCAN_FSY_VALUE, SVT_EXE_PRESCAN_FSY_VALUE,
                 sizeof(f_tbl->fsy_value), &f_tbl->fsy_value, 0L);
    n++;
    VFF_SET_ARGS(fvfReq[A_PRESCAN_RFL_GAIN_VALUE], SVN_EXE_PRESCAN_RFL_GAIN_VALUE, SVT_EXE_PRESCAN_RFL_GAIN_VALUE,
                 sizeof(f_tbl->rfl_gain_value), &f_tbl->rfl_gain_value, 0L);
    n++;
    VFF_SET_ARGS(fvfReq[A_PRESCAN_RFL_SCALE_VALUE], SVN_EXE_PRESCAN_RFL_SCALE_VALUE, SVT_EXE_PRESCAN_RFL_SCALE_VALUE,
                 sizeof(f_tbl->rfl_scale_value), &f_tbl->rfl_scale_value, 0L);
    n++;
    VFF_SET_ARGS(fvfReq[A_PRESCAN_RGN_VALUE], SVN_EXE_PRESCAN_RGN_VALUE, SVT_EXE_PRESCAN_RGN_VALUE,
                 sizeof(f_tbl->rgn_value), &f_tbl->rgn_value, 0L);
    n++;
    VFF_SET_ARGS(fvfReq[A_PRESCAN_TGC_RFOUT_RATIO], SVN_EXE_PRESCAN_TGC_RFOUT_RATIO, SVT_EXE_PRESCAN_TGC_RFOUT_RATIO,
                 sizeof(f_tbl->TGCRFoutRatio), &f_tbl->TGCRFoutRatio, 0L);
    n++;
    VFF_SET_ARGS(fvfReq[A_RF_INTERLOCK_VALUE], SVN_PLN_RF_INTERLOCK_VALUE, SVT_PLN_RF_INTERLOCK_VALUE,
                 sizeof(f_tbl->inter_lock), &f_tbl->inter_lock, 0L);
    n++;

    /******* Shimming path ********/
    VFF_SET_ARGS(fvfReq[A_SHIM_ENABLE_FLAG], SVN_PLN_SHIM_ENABLE_FLAG, SVT_PLN_SHIM_ENABLE_FLAG,
                 sizeof(f_tbl->shim_flag), &f_tbl->shim_flag, 0L);
    n++;
    VFF_SET_ARGS(fvfReq[A_SHIM_PATH], SVN_SEQ_SHIM_PATH, SVT_SEQ_SHIM_PATH,
                 sizeof(f_tbl->activeShimPath), f_tbl->activeShimPath, 0L);
    n++;
    VFF_SET_ARGS(fvfReq[A_SHIM_FSY_FLAG], SVN_SEQ_SHIM_FSY_FLAG, SVT_SEQ_SHIM_FSY_FLAG,
                 sizeof(f_tbl->shimFsyFlag), &f_tbl->shimFsyFlag , 0L);
    n++;

    /******* SPEEDER MAP PATH ******/
    VFF_SET_ARGS(fvfReq[A_MAP_PATH], SVN_SEQ_MAP_PATH , VFT_PATHELEM,
                 sizeof(f_tbl->mapPath) , f_tbl->mapPath , 0L);
    n++;

    /******* SAR ANATOMY ******/
    VFF_SET_ARGS(fvfReq[A_SAR_ANATOMY], SVN_PLN_SAR_ANATOMY , SVT_PLN_SAR_ANATOMY,
                 sizeof(f_tbl->mapPath) , &f_tbl->sar , 0L);
    n++;

    /*******  RECIVER/TRANSMIT  COIL  ******/
    VFF_SET_ARGS(fvfReq[A_RECVR_COIL], SVN_PLN_RECVR_COIL , SVT_PLN_RECVR_COIL,
                 sizeof(f_tbl->rcvCoil) , &f_tbl->rcvCoil , 0L);
    n++;
    VFF_SET_ARGS(fvfReq[A_TRANSMIT_COIL], SVN_PLN_TRANSMIT_COIL , SVT_PLN_TRANSMIT_COIL,
                 sizeof(f_tbl->trnCoil) , &f_tbl->trnCoil , 0L);
    n++;

    /******* COUCH POSITION  ******/
    VFF_SET_ARGS(fvfReq[A_EXE_COUCH], SVN_EXE_COUCH , SVT_EXE_COUCH,
                 sizeof(f_tbl->couchPosi) , &f_tbl->couchPosi , 0L);
    n++;

    int l_numDone = 0 ;

    status_t l_status = f_vfd->GetFields(l_path, SVD_ST_PROTOCOL, fvfReq, n, &l_numDone);

    if (E_NO_ERROR == l_status) {
        l_path[1].subtree_name = SVN_ST_VISUALGROUP;
        l_path[1].index = 0;
        /******* VG OFFSET VECTOR  ******/
        VFF_SET_ARGS(fvfReq[A_VG_OFFSET_VECTOR], SVN_PLN_VG_OFFSET_VECTOR , SVT_PLN_VG_OFFSET_VECTOR,
                     sizeof(f_tbl->offVecVG) , &f_tbl->offVecVG , 0L);
        n++;
        l_status |= f_vfd->GetFields(l_path, SVD_ST_VISUALGROUP, &fvfReq[A_VG_OFFSET_VECTOR], 1, &l_numDone);
    }

    if (E_NO_ERROR == l_status) {
        l_path[1].subtree_name = SVN_ST_SUBPROTOCOL;
        l_path[1].index = 0;
        l_path[2].subtree_name = SVN_ST_SLICEGROUP;
        l_path[2].index = 0;
        /******* PLN OFFSET VECTOR  ******/
        VFF_SET_ARGS(fvfReq[A_OFFSET_VECTOR], SVN_PLN_OFFSET_VECTOR , SVT_PLN_OFFSET_VECTOR,
                     sizeof(f_tbl->offVec) , &f_tbl->offVec , 0L);
        n++;
        l_status = f_vfd->GetFields(l_path, SVD_ST_SLICEGROUP, &fvfReq[A_OFFSET_VECTOR], 1, &l_numDone);
    }


    return l_status ;
}


//****************************Method Header************************************
//Method Name   :pqmAmbSetOffset()
//Author        :iGATE
//Purpose       :Set couch offset for Slave protocols.
//*****************************************************************************
status_t CpqmAMBUtilities::pqmAmbSetOffset(CVarFieldHandle* const f_vfd , CPqmProtocol* const f_protocol, const flt32_t	f_couchPos)
{
    LPCTSTR FUNC_NAME = _T("CpqmAMBUtilities::pqmAmbSetOffset");
    PQM_SCOPED_TRACE();

    flt32_t	l_offset = 0.0;
    BITFLD_DECL(mask, K_ALL_PR);
    CPQMStudy* l_study = m_pqm->GetStudy();
    CPqmPrMode* l_mode = f_protocol->GetPqmPrMode() ;

    /* 本来ならば、現在の寝台位置を使うのだが、、
       ここでは(AMBの場合)寝台移動を想定してScanOffsetをセットする
       移動先の寝台位置とHomeポジションとでOffsetを求める
     */
    if (l_mode->GetPatoriEndIn()/*(obj)->mode.patori.end_in*/ == VFC_PATORI_HEAD_FIRST)
        l_offset = l_mode->GetHomePos() - f_couchPos;

    else
        l_offset = f_couchPos - l_mode->GetHomePos();

    l_offset /= 1000.0 ;

    BITFLD_INIT(mask, K_ALL_PR, 0);
    l_mode->SetCouchOffset(l_offset);
    l_mode->SetCouchPos(f_couchPos);
    l_study->SetProtocolValues(f_vfd , f_protocol->GetProtocol(), mask,
                               K_PR_COUCH_OFFSET,	&l_offset,	sizeof(flt32_t),
                               K_PR_COUCH,		&f_couchPos,   sizeof(flt32_t),
                               NULL);

    CScanProcedure* l_scan_procedure = m_pqm->GetAcqManager()->GetScanProcedure();

    if (!f_protocol->GetFluoro()) {
        if (f_protocol->GetCouchOffsetFlag()) {
            /*  ここはHHもしくはCCでなければばらばい、
            既にHCの状態であることはありえない
            もしあったらそれは異常状態であるのでエラーとする
            */
            l_study->SetScanModeOfHead();
            l_scan_procedure->ResetCurrentRequest();
            MPLUSERRORUSERRESPONSE_e l_response = (MPLUSERRORUSERRESPONSE_e)CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(
                    _T("WARN_PRESCAN_HOME_POS"),
                    _T("PQM"),
                    SCAN_WARN_CRUCIAL,
                    SCAN_WARN_SYSMODAL,
                    SCAN_WARN_OKBUTTON);

            return E_ERROR ;
        }

        ClibVfToUiAdapter vuiAdapter(l_study->GetVfDBHandle());
        int l_status = -1;

        if (PLAN_CHECK(f_protocol)) {
            CPQMDebugInfo::GetInstance(m_pqm)->PqmDebugAcqMonitor(_T("Change SG Offset to Current Position (%f)"), l_offset);

            /* SGをカレントポジション座標系に変換  HH->HC*/
            l_status = vuiAdapter.VuiShiftOffsetVector(f_protocol->GetProtocol(), VUI_SHIFT_MODE_SG, VUI_SHIFT_Z, l_offset);

        } else {
            /*	通常の撮像の場合、
            	VGをホームポジション座標系に変換  CC->HC　だが、
            	AMBの場合、位置決めされていないことはないが、
            　　親画像との関係が１５ｃｍ以上ある場合親子関係を切る場合がある
            　　その時はCCなにでHC変換する */

            CPQMDebugInfo::GetInstance(m_pqm)->PqmDebugAcqMonitor(_T("Change VG Offset to Home Position (%f)"), -l_offset);

            /* VGをホームポジション座標系に変換  CC->HC
             */
            l_status = vuiAdapter.VuiShiftOffsetVector(f_protocol->GetProtocol(), VUI_SHIFT_MODE_VG, VUI_SHIFT_Z, -l_offset);
        }

        f_protocol->SetCouchOffsetFlag(TRUE);
    }

    return E_NO_ERROR ;
}

//****************************Method Header************************************
//Method Name   :pqmAmbSetAcqmanTbl()
//Author        :iGATE
//Purpose       :Set couch and protocol table into study for Acqman.
//*****************************************************************************
status_t  CpqmAMBUtilities::pqmAmbSetAcqmanTbl(CVarFieldHandle* const f_vfd , const int f_prot, int* const f_prot_tbl, int* const f_couch_tbl)
{

    LPCTSTR FUNC_NAME = _T("CpqmAMBUtilities::pqmAmbSetAcqmanTbl");
    PQM_SCOPED_TRACE();

    VfPathElem_t	l_path[4];

    l_path[0].subtree_name = SVN_ST_PROTOCOL;
    l_path[0].index = f_prot;
    l_path[1].subtree_name = SVN_ST_SUBPROTOCOL;
    l_path[1].index = 0;

    /** このテーブルはプロトコル番号以外にも先頭には格納個数が入っている。
    　　このテーブル数に１加算値がprot_tbl数になる　　　　**/
    int l_len = *f_prot_tbl + 1 ;
    int n = 0 ;
    VfFieldSpec_t	l_vfReq[4];
    VFF_SET_ARGS(l_vfReq[n], SVN_PLN_AMB_COUCH_TABLE, SVT_PLN_AMB_COUCH_TABLE,
                 sizeof(int)*l_len, f_couch_tbl, VFO_REPLACE);
    n++;
    VFF_SET_ARGS(l_vfReq[n], SVN_PLN_AMB_PROTOCOL_TABLE, SVT_PLN_AMB_PROTOCOL_TABLE,
                 sizeof(int)*l_len, f_prot_tbl, VFO_REPLACE);
    n++;
    int	 l_numDone ;
    status_t        l_status  ;
    time_t	l_ts;
    l_status = f_vfd->PutFields(l_path, SVD_ST_PROTOCOL, l_vfReq, n, &l_numDone, &l_ts);

    if (l_status != E_NO_ERROR && n != l_numDone) {
        return E_ERROR;
    }

    return E_NO_ERROR ;
}



//****************************Method Header************************************
//Method Name   :pqmAmbPreScanCopy()
//Author        :iGATE
//Purpose       :Copies prescan tags of Master protocol to Slave protocol.
//*****************************************************************************
status_t CpqmAMBUtilities::pqmAmbPreScanCopy(CVarFieldHandle* f_vfd , const int f_from_prot, const int f_to_prot)
{
    LPCTSTR FUNC_NAME = _T("CpqmAMBUtilities::pqmAmbPreScanCopy");
    PQM_SCOPED_TRACE();

    VfPathElem_t	l_activeShimPath[SVD_ST_PROCESSED];
    VfPathElem_t    l_mapPath[4];

    VfPathElem_t	l_path[4];
    memset(l_mapPath , NULL , sizeof(l_mapPath));
    memset(l_activeShimPath , NULL , sizeof(l_activeShimPath));
    memset(l_path , NULL , sizeof(l_path));

    l_path[0].subtree_name = SVN_ST_PROTOCOL;
    l_path[0].index = f_from_prot;
    l_path[1].subtree_name = SVN_ST_SUBPROTOCOL;
    l_path[1].index = 0;

    if (!f_vfd) {
        return E_ERROR;
    }

    struct VfMap_s_prot {
        VfName_t m_name;
        VfName_t m_type;
    } const l_vf_map_prot [] = {

        { SVN_EXE_PRESCAN_FSY_VALUE, SVT_EXE_PRESCAN_FSY_VALUE},
        { SVN_EXE_PRESCAN_RFL_GAIN_VALUE, SVT_EXE_PRESCAN_RFL_GAIN_VALUE},
        { SVN_EXE_PRESCAN_RFL_SCALE_VALUE, SVT_EXE_PRESCAN_RFL_SCALE_VALUE},
        { SVN_EXE_PRESCAN_RGN_VALUE, SVT_EXE_PRESCAN_RGN_VALUE},
        { SVN_EXE_PRESCAN_TGC_RFOUT_RATIO, SVT_EXE_PRESCAN_TGC_RFOUT_RATIO},
        { SVN_SEQ_MAP_PATH , VFT_PATHELEM},
        { SVN_PLN_PHASE_CORRECTION_PRESCAN_FLAG  , SVT_PLN_PHASE_CORRECTION_PRESCAN_FLAG},
        { SVN_SEQ_PHASE_CORRECTION_PRESCAN_METHOD , SVT_SEQ_PHASE_CORRECTION_PRESCAN_METHOD},
        { SVN_EXE_PRESCAN_B1_SHIM_AMP , SVT_EXE_PRESCAN_B1_SHIM_AMP},
        { SVN_EXE_PRESCAN_B1_SHIM_PHASE , SVT_EXE_PRESCAN_B1_SHIM_PHASE},
        { SVN_EXE_G_COIL_BASE_OLP_VALUE , SVT_EXE_G_COIL_BASE_OLP_VALUE},
        { SVN_EXE_RTSAR_PEAK_POWER_RFL, SVT_EXE_RTSAR_PEAK_POWER_RFL},
        { SVN_EXE_PRESCAN_NUM_MTGC, SVT_EXE_PRESCAN_NUM_MTGC},
        { SVN_EXE_PRESCAN_RFL_GAIN_VALUE_LIMIT, SVT_EXE_PRESCAN_RFL_GAIN_VALUE_LIMIT},
        { SVN_EXE_PRESCAN_MTGC_INDEX, SVT_EXE_PRESCAN_MTGC_INDEX},
        { SVN_EXE_PRESCAN_MTGC_RFOUT_RATIO, SVT_EXE_PRESCAN_MTGC_RFOUT_RATIO},
        { SVN_EXE_PRESCAN_MTGC_RFOUT_RATIO_MAX, SVT_EXE_PRESCAN_MTGC_RFOUT_RATIO_MAX},
        { SVN_EXE_PRESCAN_STGC_RFOUT_RATIO, SVT_EXE_PRESCAN_STGC_RFOUT_RATIO},
        { SVN_EXE_PRESCAN_NUM_RFL_SHOT, SVT_EXE_PRESCAN_NUM_RFL_SHOT},
        { SVN_EXE_PRESCAN_STGC_RFOUT_RATIO1, SVT_EXE_PRESCAN_STGC_RFOUT_RATIO1 },
        { SVN_EXE_PRESCAN_STGC_RFOUT_RATIO2, SVT_EXE_PRESCAN_STGC_RFOUT_RATIO2},
        { SVN_EXE_PRESCAN_MTGC_REFLECT1, SVT_EXE_PRESCAN_MTGC_REFLECT1},
        { SVN_EXE_PRESCAN_MTGC_REFLECT2, SVT_EXE_PRESCAN_MTGC_REFLECT2},
        { SVN_EXE_PRESCAN_STGC_REFLECT1, SVT_EXE_PRESCAN_STGC_REFLECT1},
        { SVN_EXE_PRESCAN_STGC_REFLECT2, SVT_EXE_PRESCAN_STGC_REFLECT2},
        { SVN_EXE_CFA_COUCH, SVT_EXE_CFA_COUCH},
        { SVN_EXE_RFL_COUCH, SVT_EXE_RFL_COUCH}
    };

    const int	l_vf_map_size = sizeof(l_vf_map_prot) / sizeof(*l_vf_map_prot);

    VfFieldSpec_t l_vfreq_get[l_vf_map_size] = {0};

    int	l_index = 0;

    for (l_index = 0; l_index < l_vf_map_size; l_index++) {
        VFF_SET_ARGS(l_vfreq_get[l_index], l_vf_map_prot[l_index].m_name, l_vf_map_prot[l_index].m_type, 0,
                     NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);

    }

    int	l_num_done = 0;
    status_t   l_status = f_vfd->GetFields(l_path, SVD_ST_PROTOCOL, l_vfreq_get, l_vf_map_size, &l_num_done);

    if (l_status != E_NO_ERROR ||  5 >= l_num_done ||
        l_vfreq_get[0].status != VFE_NO_ERROR ||
        l_vfreq_get[1].status != VFE_NO_ERROR ||
        l_vfreq_get[2].status != VFE_NO_ERROR ||
        l_vfreq_get[3].status != VFE_NO_ERROR ||
        l_vfreq_get[4].status != VFE_NO_ERROR) {
        l_status = E_ERROR ;
        return E_ERROR;
    }

    int l_num = 0;
    VfFieldSpec_t	l_vfreq_put[l_vf_map_size] = {0};

    for (l_index = 0, l_num = 0 ; l_index < l_vf_map_size ; l_index ++) {
        if (E_NO_ERROR == l_vfreq_get[l_index].status) {

            VFF_SET_ARGS(l_vfreq_put[l_num], l_vf_map_prot[l_index].m_name , l_vf_map_prot[l_index].m_type,
                         l_vfreq_get[l_index].size, l_vfreq_get[l_index].buffer, VFO_REPLACE);
            l_num++;
        }
    }

    VfPathElem_t l_put_path[2] = {0};
    l_put_path[0].subtree_name = SVN_ST_PROTOCOL;
    l_put_path[0].index = f_to_prot;

    if (l_num) {
        time_t l_time_stamp;
        l_num_done = 0;
        l_status = f_vfd->PutFields(l_put_path, SVD_ST_PROTOCOL, l_vfreq_put, l_num, &l_num_done, &l_time_stamp);

        if (E_NO_ERROR != l_status || l_num_done != l_num) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
        }
    }

    for (l_index = 0; l_index < l_vf_map_size; l_index++) {
        if (l_vfreq_get[l_index].status == E_NO_ERROR && l_vfreq_get[l_index].size > 0)
            FREE_PTR(l_vfreq_get[l_index].buffer) ;
    }

    //subprotocol level tags

    memset(l_path , NULL , sizeof(l_path));
    memset(l_vfreq_get , NULL , sizeof(l_vfreq_get));
    memset(l_vfreq_put , NULL , sizeof(l_vfreq_put));

    l_path[0].subtree_name = SVN_ST_PROTOCOL;
    l_path[0].index = f_from_prot;
    l_path[1].subtree_name = SVN_ST_SUBPROTOCOL;
    l_path[1].index = 0;


    struct VfMap_s_subprot {
        VfName_t m_name;
        VfName_t m_type;
    } const l_vf_map_subprot [] = {
        {SVN_EXE_PHASE_CORRECTION_PRESCAN_STATUS , SVT_EXE_PHASE_CORRECTION_PRESCAN_STATUS},
        { SVN_EXE_ANGLE_OFFSET , SVT_EXE_ANGLE_OFFSET},
        { SVN_EXE_RO_TUNE_VALUE , SVT_EXE_RO_TUNE_VALUE},
        { SVN_GEN_PHASE_CORRECTION_DATA , VFT_INT16}
    };

    const int l_vf_map_size_subprot = sizeof(l_vf_map_subprot) / sizeof(*l_vf_map_subprot);

    for (l_index = 0; l_index < l_vf_map_size_subprot; l_index++) {
        VFF_SET_ARGS(l_vfreq_get[l_index], l_vf_map_subprot[l_index].m_name, l_vf_map_subprot[l_index].m_type, 0,
                     NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);

    }

    l_num_done = 0;

    l_status = f_vfd->GetFields(l_path, SVD_ST_SUBPROTOCOL, l_vfreq_get, l_vf_map_size_subprot, &l_num_done);

    for (l_index = 0, l_num = 0 ; l_index < l_vf_map_size_subprot ; l_index ++) {
        if (E_NO_ERROR == l_vfreq_get[l_index].status) {

            VFF_SET_ARGS(l_vfreq_put[l_num], l_vf_map_subprot[l_index].m_name , l_vf_map_subprot[l_index].m_type,
                         l_vfreq_get[l_index].size, l_vfreq_get[l_index].buffer, VFO_REPLACE);
            l_num++;
        }
    }

    if (l_num) {
        time_t l_ts;
        l_num_done = 0;
        memset(l_put_path , NULL , sizeof(l_put_path));
        l_put_path[0].subtree_name = SVN_ST_PROTOCOL;
        l_put_path[0].index = f_to_prot;
        l_put_path[1].subtree_name = SVN_ST_SUBPROTOCOL;
        l_put_path[1].index = 0;

        l_status = f_vfd->PutFields(l_put_path, SVD_ST_SUBPROTOCOL, l_vfreq_put, l_num, &l_num_done, &l_ts);

        if (E_NO_ERROR != l_status || l_num_done != l_num) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
        }
    }

    for (l_index = 0; l_index < l_vf_map_size_subprot; l_index++) {
        if (l_vfreq_get[l_index].status == E_NO_ERROR && l_vfreq_get[l_index].size > 0)
            FREE_PTR(l_vfreq_get[l_index].buffer) ;
    }

    memset(l_path , NULL , sizeof(l_path));
    memset(l_vfreq_get , NULL , sizeof(l_vfreq_get));
    memset(l_vfreq_put , NULL , sizeof(l_vfreq_put));

    l_path[0].subtree_name = SVN_ST_PROTOCOL;
    l_path[0].index = f_from_prot;
    l_path[1].subtree_name = SVN_ST_SUBPROTOCOL;
    l_path[1].index = 0;
    int n = 0 ;

    int32_t	l_shim_flag = 0 ;

    VFF_SET_ARGS(l_vfreq_get[0], SVN_PLN_SHIM_ENABLE_FLAG, SVT_PLN_SHIM_ENABLE_FLAG,
                 sizeof(l_shim_flag), &l_shim_flag, 0L);
    n++;
    VFF_SET_ARGS(l_vfreq_get[1], SVN_SEQ_SHIM_PATH, SVT_SEQ_SHIM_PATH,
                 sizeof(l_activeShimPath), l_activeShimPath, 0L);
    n++;
    int  l_shimFsyFlag = 0;
    VFF_SET_ARGS(l_vfreq_get[2], SVN_SEQ_SHIM_FSY_FLAG, SVT_SEQ_SHIM_FSY_FLAG,
                 sizeof(l_shimFsyFlag), &l_shimFsyFlag, 0L);
    n++;

    int l_numDone = 0;
    l_status = f_vfd->GetFields(l_path, SVD_ST_PROTOCOL, l_vfreq_get, n, &l_numDone);

    n = 0 ;

    if (l_vfreq_get[0].status == VFE_NO_ERROR &&
        l_vfreq_get[1].status == VFE_NO_ERROR &&
        l_vfreq_get[2].status == VFE_NO_ERROR) {
        VFF_SET_ARGS(l_vfreq_put[n], SVN_PLN_SHIM_ENABLE_FLAG, SVT_PLN_SHIM_ENABLE_FLAG,
                     sizeof(l_shim_flag), &l_shim_flag, VFO_REPLACE);
        n++;
        VFF_SET_ARGS(l_vfreq_put[n], SVN_SEQ_SHIM_PATH, SVT_SEQ_SHIM_PATH,
                     sizeof(l_activeShimPath), l_activeShimPath, VFO_REPLACE);
        n++ ;
        VFF_SET_ARGS(l_vfreq_put[n], SVN_SEQ_SHIM_FSY_FLAG, SVT_SEQ_SHIM_FSY_FLAG,
                     sizeof(l_shimFsyFlag), &l_shimFsyFlag, VFO_REPLACE);
        n++;
    }

    CPqmProtocol*	l_obj = m_pqm->GetStudy()->GetItemFromProtocol(f_to_prot);
    assert(l_obj) ;

    /************* RF Interlock情報のセットアップ *************/
    flt32_t l_inter_lock  = 0.0 ;
    CInterLock l_cinterlock(m_pqm->GetAcqManager()->GetScanProcedure());
    BOOL l_enable = FALSE;

    if (m_pqm->IsSaturnCoilSystem()) {
        /* Saturn Coil */
        PqmCoil_t*		coil_p = NULL;
        m_pqm->GetStudy()->GetTxCoilInSaturn(l_obj->GetCoilDBTable(), &coil_p);
        l_enable = l_cinterlock.GetInterLock(coil_p->coil_id, l_obj->GetPqmPrMode()->GetAnatomy(), m_pqm->GetStudy()->GetWeight(), &l_inter_lock)  ;

    } else {
        l_enable = l_cinterlock.GetInterLock(l_obj->GetPqmPrMode()->GetRecvCoilId(), l_obj->GetPqmPrMode()->GetAnatomy(), m_pqm->GetStudy()->GetWeight(), &l_inter_lock);
    }

    if (l_enable  || (0.0 == l_inter_lock))
        return E_ERROR;

    VFF_SET_ARGS(l_vfreq_put[n], SVN_PLN_RF_INTERLOCK_VALUE, SVT_PLN_RF_INTERLOCK_VALUE,
                 sizeof(l_inter_lock), &l_inter_lock , VFO_REPLACE);
    n++;

    int32_t	l_fsy_action, l_rfl_action, l_rgn_action;

    l_fsy_action = l_rfl_action = l_rgn_action = VFC_PRESCAN_USE_VARFIELD ;
    VFF_SET_ARGS(l_vfreq_put[n], SVN_EXE_PRESCAN_FSY_ACTION, SVT_EXE_PRESCAN_FSY_ACTION,
                 sizeof(l_fsy_action), &l_fsy_action, VFO_REPLACE);
    n++;
    VFF_SET_ARGS(l_vfreq_put[n], SVN_EXE_PRESCAN_RFL_ACTION, SVT_EXE_PRESCAN_RFL_ACTION,
                 sizeof(l_rfl_action), &l_rfl_action, VFO_REPLACE);
    n++;
    VFF_SET_ARGS(l_vfreq_put[n], SVN_EXE_PRESCAN_RGN_ACTION, SVT_EXE_PRESCAN_RGN_ACTION,
                 sizeof(l_rgn_action), &l_rgn_action, VFO_REPLACE);
    n++;



    time_t	l_ts;
    l_path[0].index = f_to_prot;
    l_numDone = 0;
    l_status = f_vfd->PutFields(l_path, SVD_ST_PROTOCOL, l_vfreq_put, n, &l_numDone, &l_ts);

    if (l_status != E_NO_ERROR && n != l_numDone) {
        return E_ERROR;
    }

    return l_status;

}

//****************************Method Header************************************
//Method Name   :pqmAmbPrepSet()
//Author        :iGATE
//Purpose       : Set visual prep ON/OFF.
//*****************************************************************************
void CpqmAMBUtilities::pqmAmbPrepSet(CVarFieldHandle* const f_vfd, const int f_prot, int f_data)
{

    LPCTSTR FUNC_NAME = _T("CpqmAMBUtilities::pqmAmbPrepSet");
    PQM_SCOPED_TRACE();

    VfPathElem_t	l_path[4];
    VfFieldSpec_t	l_vfReq[4];

    CPqmProtocol* l_protocol =  m_pqm->GetStudy()->GetItemFromProtocol(f_prot);
    l_protocol->SetPrepScan(f_data);
    l_path[0].subtree_name = SVN_ST_PROTOCOL;
    l_path[0].index = f_prot;
    /******* ここでセットする ********/
    VFF_SET_ARGS(l_vfReq[0], SVN_PLN_PREP_SCAN, SVT_PLN_PREP_SCAN,
                 sizeof(f_data), &f_data, VFO_REPLACE);

    int			l_num = 0 ;
    time_t		l_ts = 0;

    status_t l_status =  f_vfd->PutFields(l_path, SVD_ST_PROTOCOL, l_vfReq, 1, &l_num, &l_ts);

}
//****************************Method Header************************************
//Method Name   :pqmAmbCheckMovingCouch()
//Author        :iGATE
//Purpose       :Check in/out limit for the given protocol and calulate new couch position.
//*****************************************************************************
status_t CpqmAMBUtilities::pqmAmbCheckMovingCouch(CPqmProtocol*  const f_obj , CPqmPrMode* const f_mode,
        float* const f_newposition, float* const f_moveval, int* const f_msg)
{
    LPCTSTR FUNC_NAME = _T("CpqmAMBUtilities::pqmAmbCheckMovingCouch");
    PQM_SCOPED_TRACE();

    CPqmCoilData* l_coil_data = m_pqm->GetDataManager()->GetPQMCoilData();
    CCoil l_coil;
    l_coil_data->PqmCoilGetCoil(f_obj->GetPqmPrMode()->GetRecvCoilId(), l_coil);

    int		l_inLimit = 0, l_outLimit = 0;

    if (m_pqm->IsSaturnCoilSystem()) {
        /* Saturn Coil */
        l_coil_data->GetInOutSideLimit(f_obj->GetCoilDBTable(),
                                       &l_inLimit, &l_outLimit);

    } else {
        l_inLimit = l_coil.GetInsideLimit();
        l_outLimit = l_coil.GetOutsideLimit();
    }

    float l_distance = (f_mode->GetHomePos() - f_mode->GetCouchPos());
    float z = (float)(f_obj->GetOffset().z * 1000.);
    float	l_offset = 0.0f;

    if (VFC_PATORI_HEAD_FIRST == f_mode->GetPatoriEndIn())
        l_offset = floorf(l_distance + z);

    else
        l_offset = floorf(l_distance - z);

    CPQMDebugInfo::GetInstance(m_pqm)->PqmDebugAcqMonitor(_T("distance = %g\n"), l_distance);
    CPQMDebugInfo::GetInstance(m_pqm)->PqmDebugAcqMonitor(_T("Z Offset = %g m --> %g mm\n"),
            f_obj->GetOffset().z, z);
    CPQMDebugInfo::GetInstance(m_pqm)->PqmDebugAcqMonitor(_T("homePos = %g offset = %g couchPos = %g"),
            f_mode->GetHomePos(), l_offset,
            f_mode->GetCouchPos());

    float l_newPos = f_mode->GetCouchPos() + l_offset;

    CPQMStudy* l_study = m_pqm->GetStudy();
    CPqmProtocol* l_prObj = NULL	;

    /* もし位置決め画像の寝台位置に移動する設定になっていた場合、
       位置決め画像の寝台位置を使用する　V9.00 */
    if (VFC_MOVE_COUCH_METHOD_PARENT_POSI == f_obj->GetcouchMethodFlg()) {
        if (SVN_ST_PROTOCOL == f_obj->GetLocatorImages(0).subtree_name) {
            l_prObj   =   l_study->GetItemFromProtocol(f_obj->GetLocatorImages(0).index);

            if (NULL != l_prObj) {
                l_newPos  = l_prObj->GetPqmPrMode()->GetCouchPos();
                l_offset  = l_prObj->GetPqmPrMode()->GetCouchPos() - f_mode->GetCouchPos();
            }
        }
    }

    float l_inLimitPos  = f_mode->GetHomePos() - (float)l_inLimit;
    float l_outLimitPos = f_mode->GetHomePos() + (float)l_outLimit;

    CCouchConfig* l_couch_config =  m_pqm->GetCouchConfigPtr();

    if (l_inLimitPos < (float)l_couch_config->GetCouchInLimit())
        l_inLimitPos = (float)l_couch_config->GetCouchInLimit();

    if (l_outLimitPos > (float)l_couch_config->GetCouchOutLimit())
        l_outLimitPos = (float)l_couch_config->GetCouchOutLimit();

    CPQMDebugInfo::GetInstance(m_pqm)->PqmDebugAcqMonitor(_T("Enable range %f < %f < %f"), l_inLimitPos, l_newPos, l_outLimitPos);

    if (f_mode->GetCouchPos() < l_inLimitPos || f_mode->GetCouchPos() > l_outLimitPos) {
        *f_msg = PQM_AMB_COUCH_POSI_ILLEGAL ;
        *f_newposition = f_mode->GetCouchPos() ;
        *f_moveval = 0 ;
        return COUCH_ERR_LEVEL3;

    } else if (((f_mode->GetCouchPos() == l_inLimitPos) && (l_newPos < l_inLimitPos)) ||
               ((f_mode->GetCouchPos() == l_outLimitPos) && (l_newPos > l_outLimitPos))) {
        *f_msg = PQM_AMB_START_COUCH_OVER ;
        *f_newposition = f_mode->GetCouchPos() ;
        *f_moveval = 0 ;
        return COUCH_ERR_LEVEL1;
    }

    *f_msg = PQM_AMB_COUCH_MOVE_NORMAL;

    if (l_newPos < l_inLimitPos) {
        l_offset = l_inLimitPos - f_mode->GetCouchPos();

        CPQMDebugInfo::GetInstance(m_pqm)->PqmDebugAcqMonitor(_T("New offset = %f"), l_offset);
        *f_msg = PQM_AMB_START_COUCH_OVER ;
        *f_newposition =  l_inLimitPos;
        *f_moveval = l_offset ;
        return COUCH_ERR_LEVEL1;

    } else if (l_newPos > l_outLimitPos) {
        l_offset = l_outLimitPos - f_mode->GetCouchPos();

        CPQMDebugInfo::GetInstance(m_pqm)->PqmDebugAcqMonitor(_T("New offset = %f"), l_offset);
        *f_msg = PQM_AMB_START_COUCH_OVER ;
        *f_newposition =  l_outLimitPos;
        *f_moveval = l_offset ;
        return COUCH_ERR_LEVEL1;
    }

    if (l_offset > (float)l_couch_config->GetCouchMaximumMovement()) {
        l_offset = (float)l_couch_config->GetCouchMaximumMovement();

        CPQMDebugInfo::GetInstance(m_pqm)->PqmDebugAcqMonitor(_T("New offset = %f"), l_offset);
        *f_msg = PQM_AMB_START_COUCH_OVER ;
        *f_newposition =  f_mode->GetCouchPos() + l_offset;
        *f_moveval = l_offset ;
        return COUCH_ERR_LEVEL1;

    } else if (l_offset < - (float)l_couch_config->GetCouchMaximumMovement()) {
        l_offset = - (float)l_couch_config->GetCouchMaximumMovement();

        CPQMDebugInfo::GetInstance(m_pqm)->PqmDebugAcqMonitor(_T("New offset = %f"), l_offset);
        *f_msg = PQM_AMB_START_COUCH_OVER ;
        *f_newposition =  f_mode->GetCouchPos() + l_offset;
        *f_moveval = l_offset ;
        return COUCH_ERR_LEVEL1;
    }

    *f_newposition =  l_newPos;
    *f_moveval = l_offset ;
    return COUCH_ERR_NON;

}

//****************************Method Header************************************
//Method Name   :pqmAmbSetScanOff()
//Author        :iGATE
//Purpose       :Set scan offset.
//*****************************************************************************
void CpqmAMBUtilities::pqmAmbSetScanOff(CPqmProtocol* const f_master_obj, CPqmProtocol* const f_obj , const float f_offset)
{
    LPCTSTR FUNC_NAME = _T("CpqmAMBUtilities::pqmAmbSetScanOff");
    PQM_SCOPED_TRACE();

    f32vec3_t	l_vec;
    l_vec.x = l_vec.y = 0;
    l_vec.z = f_offset;

    f32vec3_t l_offset_vec = f_obj->GetOffset();
    CPQMStudy* l_study = m_pqm->GetStudy();
    l_study->PqmUiConvertVector(&(f_obj->GetPqmPrMode()->GetPatientOrientation()), FALSE, &l_vec, &l_offset_vec);

    l_offset_vec.x /= OFFSET_RATE_AMB;
    l_offset_vec.y /= OFFSET_RATE_AMB;
    l_offset_vec.z /= OFFSET_RATE_AMB;
    f_obj->SetOffset(0, l_offset_vec.x);
    f_obj->SetOffset(1, l_offset_vec.y);
    f_obj->SetOffset(2, l_offset_vec.z);

    ClibVfToUiAdapter vuiAdapter(l_study->GetVfDBHandle());
    const status_t l_status = vuiAdapter.VuiSetOffsetVector(f_obj->GetProtocol(), &l_offset_vec);

    //DB_PRINTF("c",("status=%d\n",status));

    CPqmCoilData* l_coil_data = m_pqm->GetDataManager()->GetPQMCoilData();
    CCoil l_coil;
    l_coil_data->PqmCoilGetCoil(f_obj->GetPqmPrMode()->GetRecvCoilId(), l_coil);
    int l_inLimit, l_outLimit = 0;

    bool_t	l_couch_enable = FALSE;

    if (m_pqm->IsSaturnCoilSystem()) {
        /* Saturn Coil */
        l_couch_enable = (l_coil_data->GetInOutSideLimit(f_obj->GetCoilDBTable(), &l_inLimit, &l_outLimit) == E_NO_ERROR && l_inLimit + l_outLimit > 0);

    } else {

        l_inLimit = l_coil.GetInsideLimit();
        l_outLimit = l_coil.GetOutsideLimit();
        l_couch_enable = (l_inLimit + l_outLimit > 0);
    }

    /* V7.00  Wide Area Scan スキャンオフセット設定したら寝台移動をＯＮにする */

    if (l_couch_enable) {
        /*  ここで位置決めした様に見せかける必要があるので
            PLN_STATUSのGLOCをONにする  */
        int32_t hold_scan = f_obj->GetHoldScan() | VFC_HOLD_COUCH;
        f_obj->SetHoldScan(hold_scan);

        l_study->SetProtocolValue(l_study->GetVfDBHandle(), f_obj, K_PR_HOLD,
                                  &hold_scan, sizeof(int32_t),
                                  TRUE, PQM_STS_LIST);


        int l_couch_method = f_master_obj->GetcouchMethodFlg();

        if (VFC_MOVE_COUCH_METHOD_NONE == l_couch_method)
            l_couch_method = VFC_MOVE_COUCH_METHOD_PLAN_CENTER_POSI;

        f_obj->SetcouchMethodFlg(l_couch_method);
        l_study->SetProtocolValue(l_study->GetVfDBHandle(), f_obj,
                                  K_PR_MOVE_COUCH_METHOD_FLAG,
                                  &l_couch_method,
                                  sizeof(int32_t),
                                  TRUE, PQM_STS_LIST);
    }

    //obj->pln_status   |= VFC_PLN_STATUS_GLOC;
    int32_t l_pln_status = f_obj->GetPlnStatus() | VFC_PLN_STATUS_GLOC ;
    f_obj->SetPlnStatus(l_pln_status);

    l_study->SetProtocolValue(l_study->GetVfDBHandle(), f_obj, K_PR_STATUS,
                              &l_pln_status, sizeof(int32_t), TRUE, PQM_STS_LIST);

    //PqmListSelectPos(PqmPqGetPosFromProt(obj->prot),FALSE,0);

    /* 結果を表示する。オフセットを設定するプロトコルが選択中
       のはず */
    //l_study->PqmUiSetScanOffset(obj);
}

//****************************Method Header************************************
//Method Name   :pqmAmbConvProtToPos()
//Author        :iGATE
//Purpose       :To Convert Acq order to position.
//*****************************************************************************
status_t CpqmAMBUtilities::pqmAmbConvProtToPos(int* const f_prot_list, const int f_n, int* const f_pos_list)
{
    CPqmProtocol* l_protocol = NULL;
    CPQMStudy* l_study = m_pqm->GetStudy();
    int l_sts = E_NO_ERROR;

    for (int i = 0 ; i < f_n ; i++) {
        l_protocol = l_study->GetItemFromProtocol(f_prot_list[i]);

        if (NULL == l_protocol)
            l_sts =  E_ERROR ;

        else
            f_pos_list[i] = l_protocol->GetPosition();
    }

    return l_sts ;
}
//****************************Method Header************************************
//Method Name   :pqmAmbModeSet()
//Author        :iGATE
//Purpose       :Set Mode of AMB protocol  as VFC_AMB_MODE_MASTER or VFC_AMB_MODE_SLAVE
//*****************************************************************************
status_t CpqmAMBUtilities::pqmAmbModeSet(CVarFieldHandle*	const f_vf_handle,
        const int		f_prot,
        int		f_mode)
{

    LPCTSTR FUNC_NAME = _T("CpqmAMBUtilities::pqmAmbModeSet");
    PQM_SCOPED_TRACE();


    VfPathElem_t	l_path[4];

    l_path[0].subtree_name = SVN_ST_PROTOCOL;
    l_path[0].index = f_prot;
    l_path[1].subtree_name = SVN_ST_SUBPROTOCOL;
    l_path[1].index = 0;

    CPQMStudy* l_study = m_pqm->GetStudy();
    CPqmProtocol* l_obj = l_study->GetItemFromProtocol(f_prot);

    if (l_obj) {
        l_obj->SetAMBModeFlag(f_mode);

        if (VFC_AMB_MODE_MASTER == f_mode) {
            l_obj->SetExeAutoVoice(0x0) ;
        }
    }

    /******* ここでStudyに書く ********/
    int n = 0 ;
    VfFieldSpec_t	l_vfReq[4];
    VFF_SET_ARGS(l_vfReq[n], SVN_PLN_AMB_MODE_FLG, SVT_PLN_AMB_MODE_FLG,
                 sizeof(f_mode), &f_mode, VFO_REPLACE);
    n++;

    int l_auto_voice = 0;

    if (VFC_AMB_MODE_MASTER == f_mode) {
        /* Master ProtocolのAutoVoiceは無効にする */
        VFF_SET_ARGS(l_vfReq[n], SVN_EXE_AUTO_VOICE, SVT_EXE_AUTO_VOICE,
                     sizeof(int32_t), &l_auto_voice, VFO_REPLACE);
        n++;
    }

    int num = 0;
    time_t		l_ts;

    if (E_NO_ERROR == f_vf_handle->PutFields(l_path, SVD_ST_PROTOCOL, l_vfReq, n, &num, &l_ts)) {

        return E_NO_ERROR;
    }

    return(E_ERROR);
}


//****************************Method Header************************************
//Method Name   :pqmAmbScanIDSetScan()
//Author        :iGATE
//Purpose       :Set scan id for slave protocols.
//*****************************************************************************
status_t CpqmAMBUtilities::pqmAmbScanIDSetScan(int f_numSegment, CVarFieldHandle* f_vfd, int	f_prot, int f_type)
{

    LPCTSTR FUNC_NAME = _T("CpqmAMBUtilities::pqmAmbScanIDSetScan");
    PQM_SCOPED_TRACE();

    VfPathElem_t	l_path[4];
    l_path[0].subtree_name = SVN_ST_PROTOCOL;
    l_path[0].index = f_prot;
    l_path[1].subtree_name = SVN_ST_SUBPROTOCOL;
    l_path[1].index = 0;

    VfFieldSpec_t	l_vfReq[4];

    const int l_scan_id_length = MR::ACQ::SM::COMMON::SCAN_ID_LENGTH;
    char	l_ScanID[l_scan_id_length * 2 + 1];
    memset(l_ScanID , 0, sizeof(l_ScanID)) ;
    memset(l_vfReq , 0, sizeof(l_vfReq)) ;

    VFF_SET_ARGS(l_vfReq[0], SVN_PLN_COMMENTS, SVT_PLN_COMMENTS,
                 sizeof(l_ScanID), l_ScanID, VFO_VARIABLE_LEN);
    int l_num = 0;

    CString scan_id_wcs(_T(""));
    status_t l_status = f_vfd->GetFields(l_path, SVD_ST_PROTOCOL, l_vfReq, 1, &l_num);

    if (E_NO_ERROR != l_status || E_NO_ERROR != l_vfReq[0].status || l_num != 1) {
        return E_ERROR;
    }

    /******* ここでScanID情報をセットする ********/

    int	l_loop = GET_LOOP(f_type) ;
    int	l_numStation = GET_STATION(f_type) ;
    int	l_numseg = GET_SEG(f_type) ;
    int l_prot_type =  GET_TYPE(f_type) ;

    int	l_loop_ID[3];
    l_loop_ID[0] = l_numseg ;
    l_loop_ID[1] = l_loop;
    l_loop_ID[2] = f_numSegment;
    int i = 0 ;

    std::string scan_id_mbs("");

    if ((l_prot_type & AMB_MASTER) == NULL) {
        CPQMStudy* l_study = m_pqm->GetStudy();
        CPqmProtocol* l_obj = l_study->GetItemFromProtocol(f_prot);

        if (!l_obj) {
            return E_ERROR;
        }

        if ((l_prot_type & AMB_SCAN) == NULL)
            scan_id_wcs.Format(_T("AMB %d  %s"),
                               l_numStation, CString(l_ScanID));

        else if (1 == l_loop)
            scan_id_wcs.Format(_T("AMB %d:%d/%d  %s"),
                               l_numStation, l_numseg, f_numSegment, CString(l_ScanID));

        else
            scan_id_wcs.Format(_T("AMB %d:%d(%d)/%d  %s"),
                               l_numStation, l_numseg, l_loop, f_numSegment, CString(l_ScanID));

        const int l_min_length = MIN(scan_id_wcs.GetLength(), l_scan_id_length);
        scan_id_wcs = scan_id_wcs.Left(l_min_length);

        UTIL::Wcs2Mbs(&scan_id_mbs, scan_id_wcs);

        l_obj->SetScanID(scan_id_mbs.c_str());
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, CString(l_obj->GetScanID()));

        VFF_SET_ARGS(l_vfReq[i], SVN_PLN_COMMENTS, SVT_PLN_COMMENTS, scan_id_mbs.length() + 1, const_cast<char*>(scan_id_mbs.c_str()), VFO_REPLACE);
        i++ ;

        BSTR l_bstr_loid = l_obj->GetSeriesLOID().AllocSysString();
        VARIANT l_pdata;
        BSTR l_bstr_wcs = scan_id_wcs.AllocSysString();
        l_pdata.byref = l_bstr_wcs;
        l_pdata.wReserved1 = SCAN_ID;
        m_pqm->GetStudyManagerPtr()->UpdateGUIControlsInDB(&l_pdata, l_bstr_loid);
        ::SysFreeString(l_bstr_wcs);
        ::SysFreeString(l_bstr_loid);
    }

    VFF_SET_ARGS(l_vfReq[i], SVN_PRC_AMB_SCAN_LOOP_ID, SVT_PRC_AMB_SCAN_LOOP_ID,
                 sizeof(l_loop_ID), &l_loop_ID, VFO_REPLACE);
    i++;
    time_t l_ts;

    l_status = f_vfd->PutFields(l_path, SVD_ST_PROTOCOL, l_vfReq, i, &l_num, &l_ts);

    if (E_NO_ERROR != l_status || l_num != i)
        return E_ERROR;

    return E_NO_ERROR;
}

//****************************Method Header************************************
//Method Name   :pqmAmbGetOtherProtocols()
//Author        :iGATE
//Purpose       :Add SHIM/MAP protocols to study.
//*****************************************************************************
status_t  CpqmAMBUtilities::pqmAmbGetOtherProtocols(void* f_data, const int f_pos, int f_count, char*	const f_filename)
{

    LPCTSTR FUNC_NAME = _T("CpqmAMBUtilities::pqmAmbGetOtherProtocols");
    PQM_SCOPED_TRACE();

    status_t	status = E_NO_ERROR;

    CPQMStudy* l_study = m_pqm->GetStudy();
    l_study->GetVfDBHandle()->ReOpen();

    DB_PRINTF("M", ("filename = %s\n", f_filename));

    ClibVfToUiAdapter vuiAdapter(l_study->GetVfDBHandle());

    for (; f_count ; f_count--) {
        status = vuiAdapter.VuiCopyProtocolsFromFile(
                     f_filename,
                     &f_pos, 1, 0,
                     CpqmAMBUtilities::pqmAmbAfterOtherProtocols,
                     f_data);

        if (status != E_NO_ERROR) {
            /* Put error to log file. */
            /*UIErrorNLog(E_VF_NO_COPY,UIERROR_ERROR,"%s %s",
            	"pqmPmMemoryHandle",filename);*/
            DB_FUNC_EXIT();
            return E_ERROR;
        }


        l_study->GetVfDBHandle()->ReOpen();

        /* Protocol Index を更新する V5.00 */

        l_study->SetScanModeOfHead();
    }

    return E_NO_ERROR;
}



//****************************Method Header************************************
//Method Name   :pqmAmbAfterOtherProtocols()
//Author        :iGATE
//Purpose       :Callback function by ClibVfToUiAdapter
//*****************************************************************************
status_t  CpqmAMBUtilities::pqmAmbAfterOtherProtocols(VfDBHandle_t f_db_handle, int f_protocol, void* f_data)
{
    LPCTSTR FUNC_NAME = _T("CpqmAMB::pqmAmbAfterOtherProtocols");
    PQM_SCOPED_TRACE();

    CPQMAMBcallback*	l_pAMBcallbk = (CPQMAMBcallback*)f_data ;
    CPQMStudy* l_study  = l_pAMBcallbk->m_study;

    CVarFieldHandle l_vf_handle(f_db_handle, false);

    if (l_study->CheckImgGr(&l_vf_handle, f_protocol) != E_NO_ERROR) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CheckImgGr return E_ERROR"));
        return E_ERROR;
    }

    if (l_study->ResetOffset(&l_vf_handle, f_protocol)
        != E_NO_ERROR) {
        return E_ERROR;
    }

    CPqmProtocol* l_protocol = l_study->ListCopyProt(l_study->GetVfDBHandle(), f_protocol, 0);

    if (NULL == l_protocol) {
        return E_ERROR;
    }

    /* AutoScan */
    CpqmAMBUtilities::pqmAmbAutoScanSet(&l_vf_handle, f_protocol, l_protocol, 0);

    l_pAMBcallbk->m_entry->prot[l_pAMBcallbk->m_entry->num] = f_protocol;
    l_pAMBcallbk->m_entry->num++;

    return E_NO_ERROR;
}
//****************************Method Header************************************
//Method Name   :SetPQM()
//Author        :iGATE
//Purpose       :Set PQM pointer for CpqmAMBUtilities functions.
//*****************************************************************************
void CpqmAMBUtilities::SetPQM(CPqm* f_pqm)
{
    m_pqm = f_pqm;
}


//************************************Method Header************************************
// Method Name  : GetGDCValue
// Author       : iGate
// Purpose      :
//***********************************************************************************
int CpqmAMBUtilities::GetGDCValue(CVarFieldHandle* const f_vf_handle,
                                  const int f_prot_index)
{
    LPCTSTR FUNC_NAME = _T("CpqmAMBUtilities::GetGDCValue");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, FUNC_NAME);

    if (!f_vf_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_vf_handle is NULL"));
        return 0;
    }

    int                 l_n = 0;
    VfFieldSpec_t       l_reqflds[2] = {0};
    int                 l_numdone = -1;
    int                 l_status = E_ERROR;
    int                 l_gdc_value = 0;

    VfPathElem_t    l_pelm[2] = {0};
    memset(l_pelm, 0, sizeof(VfPathElem_t));
    l_pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_pelm[SVD_ST_PROTOCOL - 1].index = f_prot_index ;
    memset(l_reqflds, 0, sizeof(VfFieldSpec_t) * 2);


    VFF_SET_ARGS(l_reqflds[l_n], SVN_PRC_GDC_DIM, SVT_PRC_GDC_DIM,
                 sizeof(int), &l_gdc_value, 0);
    l_n++;

    l_status = f_vf_handle->GetFields(l_pelm, SVD_ST_PROTOCOL, l_reqflds, l_n, &l_numdone);

    if (l_status != E_NO_ERROR) {
        return 0;
    }

    return l_gdc_value;
}

//************************************Method Header************************************
// Method Name  : GetGDCValue
// Author       : iGate
// Purpose      :
//***********************************************************************************
int CpqmAMBUtilities::GetAutoMipPreview(CVarFieldHandle* const f_vf_handle,
                                        const int f_prot_index)
{

    LPCTSTR FUNC_NAME = _T("CpqmAMBUtilities::GetAutoMipPreview");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Inside GetAutoMipPreview"));

    if (f_prot_index < 0) {

        return 0;
    }

    VfPathElem_t        l_nodePath[SVD_ST_SLICE] = {
        {SVN_ST_PROTOCOL, 0},
        {SVN_ST_SUBPROTOCOL, 0},
        {SVN_ST_SLICEGROUP, 0},
        {SVN_ST_SLICE, 0}
    };
    VfFieldSpec_t       l_vfReq[16] = {0};

    int n = 0;
    l_nodePath[SVD_ST_PROTOCOL - 1].index = f_prot_index;

    int l_Auto_MIP_preview[3] = {0};
    VFF_SET_ARGS(l_vfReq[n], SVN_PRC_AUTO_MIP_PREVIEW,
                 SVT_PRC_AUTO_MIP_PREVIEW, sizeof(l_Auto_MIP_preview),
                 l_Auto_MIP_preview, 0);
    n++;
    int l_num = 0;
    status_t l_vfStatus = f_vf_handle->GetFields(l_nodePath, SVD_ST_PROTOCOL, l_vfReq, n, &l_num);

    if_error(l_vfStatus) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("VfAppGetFields error."));
    }

    return l_Auto_MIP_preview[0];
}

//*****************************************************************************
//Method Name    : DeleteTagsFromAMBSlaveProt()
//Author         : iGATE
//Purpose        : To delete tags from slave protocols for AMB.
//                 Add your required tags below
//*****************************************************************************
void CpqmAMBUtilities::DeleteTagsFromAMBSlaveProt(CVarFieldHandle* const f_vf_handle,
        const int f_protocol)
{

    LPCTSTR FUNC_NAME = _T("CpqmAMBUtilities::DeleteTagsFromAMBSlaveProt");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("trace"));

    struct field_map {
        VfName_t name;
        VfType_t type;
    } const l_tag_tables[] = {
        {SVN_PLN_INJECT_TIME,   SVT_PLN_INJECT_TIME},
        //Add your required tags below
    };

    const int l_tag_tables_len = sizeof(l_tag_tables) / sizeof(*l_tag_tables);
    VfFieldSpec_t   l_vfreq_get[l_tag_tables_len] = {0};
    VfPathElem_t    l_path[1] = {0};

    l_path[0].subtree_name = SVN_ST_PROTOCOL;
    l_path[0].index = f_protocol;

    for (int i = 0; i < l_tag_tables_len; i++) {

        VFF_SET_ARGS(l_vfreq_get[i], l_tag_tables[i].name, l_tag_tables[i].type, 0,
                     NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);
    }

    int l_num_done = 0, n = 0;
    status_t l_status =  f_vf_handle->GetFields(l_path, SVD_ST_PROTOCOL, l_vfreq_get, l_tag_tables_len, &l_num_done);

    VfFieldSpec_t	l_vfreq_put[l_tag_tables_len] = {0};

    for (i = 0; i < l_tag_tables_len; i++) {

        if (E_NO_ERROR == l_status && E_NO_ERROR == l_vfreq_get[i].status) {

            VFF_SET_ARGS(l_vfreq_put[i], l_tag_tables[i].name, l_tag_tables[i].type, 0,
                         NULL, VFO_DELETE);
            n++;
        }
    }

    if (n > 0) {
        time_t	l_tmStamp = 0;
        l_num_done = 0;

        l_status = f_vf_handle->PutFields(l_path, SVD_ST_PROTOCOL, l_vfreq_put, n, &l_num_done, &l_tmStamp);

        for (i = 0; i < l_tag_tables_len; i++) {
            if (l_vfreq_get[i].status == E_NO_ERROR && l_vfreq_get[i].size > 0)
                FREE_PTR(l_vfreq_get[i].buffer) ;
        }

        if (E_NO_ERROR != l_status || l_num_done != n) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
        }
    }

}

//*****************************************************************************
//Method Name    : SetValForSlavProt()
//Author         : iGATE
//Purpose        : Set values for slave protocols in CPqmProtocol
//*****************************************************************************
void CpqmAMBUtilities::SetValForSlaveProt(CPqmProtocol* f_protocol)
{
    CString l_delay_time = m_pqm->GetStudy()->GetDelayTimeFromVf(f_protocol->GetProtocol());
    f_protocol->SetDelayTime(l_delay_time);
}
//****************************Method Header************************************
//Method Name   :pqmAmbSetChannelModeForMapShim()
//Author        :iGATE
//Purpose       :Set coil channel mode for shim and map protocols according to its masters.
//*****************************************************************************
status_t CpqmAMBUtilities::pqmAmbSetChannelModeForMapShim(CVarFieldHandle* f_vfd, int f_master_prot, int f_map_shim_prot)
{

    LPCTSTR FUNC_NAME = _T("CpqmAMBUtilities::pqmAmbSetChannelModeForMapShim");
    PQM_SCOPED_TRACE();

    VfPathElem_t	l_path[4];
    VfFieldSpec_t	l_vfReq[4];

    CPqmProtocol* l_master_prot = m_pqm->GetStudy()->GetItemFromProtocol(f_master_prot);
    int l_channel_mode = l_master_prot->GetcoilChannelMode();

    l_path[0].subtree_name = SVN_ST_PROTOCOL;
    l_path[0].index = f_map_shim_prot;

    VFF_SET_ARGS(l_vfReq[0], SVN_PLN_MAX_COIL_CHANNEL_MODE, SVT_PLN_MAX_COIL_CHANNEL_MODE,
                 sizeof(int), &l_channel_mode, VFO_REPLACE);

    int			l_num = 0 ;
    time_t		l_ts = 0;

    status_t l_status =  f_vfd->PutFields(l_path, SVD_ST_PROTOCOL, l_vfReq, 1, &l_num, &l_ts);

    if (l_status != E_NO_ERROR) {
        return E_ERROR;
    }

    CPqmProtocol* l_shim_map_prot = m_pqm->GetStudy()->GetItemFromProtocol(f_map_shim_prot);
    l_shim_map_prot->SetcoilChannelMode(l_channel_mode);

    return E_NO_ERROR ;

}
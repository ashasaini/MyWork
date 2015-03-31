//REDMINE-1213_Update_03_MAY
//REDMINE-1213_Update_29_APR
//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-774
//Redmine-778
//Redmine-779
//Redmine-780
//Redmine-781
//Redmine-782
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: pqmstudy.cpp
 *  Overview: Implementation of CPQMStudy class.
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
//This class will contain all the study related information like the patient name etc.
//It will also hold the map of the CSeriesPQM objects
#include "StdAfx.h"

#include <math.h>		//IR-171

#include "PQMStudy.h"
#include <algorithm>
#include <functional>
#include <tami/vfStudy/vfs_plan.h>
// + LibAcq AMT
// Both include need to be together.
#include <tami/libdll/dll.h>
extern "C" {
#include <tami/libAcq/AcqCoilSignal.h>
#include <tami/libAcq/checkSlicePosition.h>
#include <tami/libms/gdcModeDecision.h>
}
// - LibAcq AMT
#include <tami/proCon/proMsgCodes.h>
//Patni-Sudhir/2011Jan13/Added/Redmine-434
#include <tami/libmathserver/msgetnrcomment.h>
#include <DBSALib/DBSALib.h>

#include <libStudyManager/strcnv.h> //Tanoue/MVC5849
#include <libStudyManager/Log/ScopedFuncInOutLog.h>
#include <libStudyManager/Math.h>
#include <PQM/PQMLogMgr.h>
#include <PQM/PqmUtilities.h>
#include <PQM/resource.h>

//Patni-MP/2009June07/Added/ACE-2/Post Processing
#include "pqmPostProc.h"
#include "pqmipc.h"
#include "pqm.h"
#include "pqmpmacqman.h"
#include "scanprocedure.h"
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"
#include "PQMSAR.h"
#include "PQMStudyMgr.h"
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
//#include "coildb.h"
#include "PqmPrMode.h"
#include "PqmPrTRSC.h"
#include "PQMdBdt.h"
#include "pqmconfig.h"
#include "ProtocolUtil.h"
#include "PqmCoilData.h"
#include "PqmSectionData.h"
#include "PqmPathData.h"
#include "voicemanager.h"
#include "PqmProbe.h"
#include "PASCOMInterface.h"
#include "MasterSlave.h"

//Patni-Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement
#include "CDSUtilities.h"
#include "VarFieldHandle.h" //TMSC-Tanoue/2010Aug25/MVC008345
#include "GPLibAdapter.h" //TMSC-Tanoue/2010Aug25/MVC008345
#include "PQMDebugInfo.h"
#include "CoilChannelMode.h"
#include "pqmAMB.h"
//Patni/AM/2010July06/Added/NFD001-AutoMap
#include "PqmPmSARManager.h"

extern "C" {
    extern int AcqFileCopy(char* infile, char* outfile);
}

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

struct {
    VfName_t    name;
    VfType_t    type;
    void*   shim;
    int     shimSize;
} shimTbl[] = {
#define DEF(n)  {SVN_SEQ_##n,SVT_SEQ_##n,NULL,0}
    DEF(SHIM_GRAD_OFFSET),
    DEF(AXI_FSY_OFFSET),
    DEF(SAG_FSY_OFFSET),
    DEF(COR_FSY_OFFSET),
    /*DEF(SHIM_FSY_FLAG),*/
#undef DEF
};


//+Patni-PJS/2010Mar24/Added/IR-140
struct {
    VfName_t    name;
    VfType_t    type;
    void*       ctrl_value;
    int         ctrl_size;
} updateCtrls[] = {
    {SVN_PLN_COMMENTS, SVT_PLN_COMMENTS, NULL, 0},
    {SVN_PLN_SCAN_COMMENTS, SVT_PLN_SCAN_COMMENTS, NULL, 0},
    {SVN_PLN_HOLD, SVT_PLN_HOLD, NULL, 0},
    {SVN_PLN_AUTO_POSTPROC_FLG, SVT_PLN_AUTO_POSTPROC_FLG, NULL, 0},
    {SVN_PLN_INJECT_TIME, SVT_PLN_INJECT_TIME, NULL, 0},
    {SVN_SCH_N_CONTRAST, SVT_SCH_N_CONTRAST, NULL, 0},
    {SVN_PLN_SCAN_ANATOMY, SVT_PLN_SCAN_ANATOMY, NULL, 0},
    {SVN_PLN_MASTER_FLAG, SVT_PLN_MASTER_FLAG, NULL, 0},
    //+Patni-Hemant/2010Oct06/Added/MVC009119
    { SVN_PLN_BODY_PART,  SVT_PLN_BODY_PART, NULL, 0 },
    { SVN_PLN_LATERALITY, SVT_PLN_LATERALITY, NULL, 0 },

    { SVN_EXE_AUTO_VOICE, SVT_EXE_AUTO_VOICE, NULL, 0 },//Patni-Abhishek/2010Dec14/Added/IR-180
    { SVN_PLN_MAX_COIL_CHANNEL_MODE, SVT_PLN_MAX_COIL_CHANNEL_MODE, NULL, 0 }
    //-Patni-Hemant/2010Oct06/Added/MVC009119
};
//-Patni-PJS/2010Mar24/Added/IR-140

const int g_update_controls_size = (sizeof(updateCtrls) / sizeof(*updateCtrls));//Patni-Ravindra Acharya/2011Mar25/Added/TMSC-Review-Comments

//Patni-MP/2009Mar17/Added/ACE-1/MCM0213-00131
#define NO_FIELD            100
//Patni-AMT/2010Feb16/Modified/Ph-3# PROPOSAL_07
#define POS2INDEX(pos)pos - 1

using namespace MRMPlus::DBSALib;
using namespace MR::ACQ::SM::COMMON; //Tanoue/MVC5849
//Patni-Rajendra/2011Mar29/Added/TMSC-REDMINE-1559
using namespace MR::ACQ::SM::COMMON::MUTEX;
using MR::ACQ::SM::COMMON::MATH::RoundUp;

struct {
    VfName_t    name;
    VfType_t    type;
    void*       ctrl_value;
    int         ctrl_size;
} updatesavedtags[] = {
    {SVN_PLN_CURRENT_PAGE, SVT_PLN_CURRENT_PAGE, NULL, 0},
    {SVN_PLN_INSCAN_PREFERENCE_GUID, SVT_PLN_INSCAN_PREFERENCE_GUID, NULL, 0},
    {SVN_EXE_CHANNEL_PER_VFEP, SVT_EXE_CHANNEL_PER_VFEP, NULL, 0},
    {SVN_EXE_PAC_MATRIX, SVT_EXE_PAC_MATRIX, NULL, 0},
    {SVN_PLN_STATUS, SVT_PLN_STATUS, NULL, 0}
};
const int g_update_saved_tags = (sizeof(updatesavedtags) / sizeof(*updatesavedtags));
//****************************Method Header************************************
//Method Name   :CPQMStudy()
//Author        :PATNI/MRP
//Purpose       :Constructor
//*****************************************************************************
CPQMStudy::CPQMStudy(
    CPqm* pqm):
    m_pqm_ptr(pqm),
    m_wait_pos(0),
    m_study_loid(_T("")),
    m_study_name(_T("")),
    m_vfdb_handle(NULL),
    m_weight(0.0f),
    m_first_scan(TRUE),
    m_has_image(FALSE),
    m_total_time(0L),
    m_prot_count(0),
    m_gender(0),
    m_mode(0),
    m_height(0.0f),
    m_study_path(_T("")),
    m_current_hgst_dbdt(VFC_DBDT_IEC_OPERATING_NORMAL),
    m_current_hgst_sar(IEC_SAR_NORMAL_MODE), //Patni-PJS/2009Oct29/Added/IR-92
    m_studyreopen(false),//Patni-DKH/2010May03/Added/Phase3#IR105
    m_is_valid_wfda_scan(FALSE),//Patni-PJS/2011Feb21/Added/IR-97
    m_rm_sae_status_for_study(false)
{
    m_study_id.run_num  = -1;
    m_study_id.site_id  = -1;
}

//****************************Method Header************************************
//Method Name   :~CPQMStudy()
//Author        :PATNI/MRP
//Purpose       :Destructor
//*****************************************************************************
CPQMStudy::~CPQMStudy(
)
{
    //Patni-AMT/2009Apr22/Added/PSP1#001
    ClearProtocolVector();
    //Patni/2011Feb11/Added+Modified/V2.0/IR-181_NFD1_Prod_Req_Part1
    DEL_PTR(m_vfdb_handle);
}

//+Patni-PJS/2010Dec10/Modified/IR-97
//****************************Method Header************************************
//Method Name   :CheckPlnStatus()
//Author        :PATNI/PJS
//Purpose       :Check Pln Status of the protocol from varfiled.
//*****************************************************************************
status_t CPQMStudy::CheckPlnStatus(
    const int f_protocol
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::CheckPlnStatus");


    int l_plan_status = 0;

    VfFieldSpec_t l_vf_req[1] = {0};

    VfPathElem_t l_node_path[] = {
        {SVN_ST_PROTOCOL,       0},
    };

    memset(l_vf_req, 0, sizeof(VfFieldSpec_t) * 1);
    l_node_path[SVN_ST_PROTOCOL - 1].index = f_protocol;

    int l_n = 0, l_num = 0;

    VFF_SET_ARGS(l_vf_req[l_n],
                 SVN_PLN_STATUS,
                 SVT_PLN_STATUS,
                 sizeof(int),
                 &l_plan_status,
                 0);

    l_n++;

    status_t l_status = m_vfdb_handle->GetFields(l_node_path, SVD_ST_PROTOCOL, l_vf_req, l_n, &l_num);

    if ((l_status == E_NO_ERROR) && (l_num = 1)) {
        if ((l_plan_status & VFC_PLN_STATUS_PROEDIT) == VFC_PLN_STATUS_PROEDIT) {
            return E_NO_ERROR;

        } else {
            return E_ERROR;
        }

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetFields API Failed"));
        return E_ERROR;
    }
}
//-Patni-PJS/2010Dec10/Modified/IR-97

//+Patni-PJS/2011Feb21/Added/IR-97
//****************************Method Header************************************
//Method Name   :IsValidWFDAScan()
//Author        :PATNI/PJS
//Purpose       :Check whether the current WFDA scan is valid.
//*****************************************************************************
BOOL CPQMStudy::IsValidWFDAScan(
)
{
    return m_is_valid_wfda_scan;
}
//-Patni-PJS/2011Feb21/Added/IR-97

/****************************Method Header************************************
//Method Name   :UpdateGUIControlsInDB()
//Author        :PATNI/SG
//Purpose       :For updating controls values in Varfield DB
//*****************************************************************************/
BOOL CPQMStudy::UpdateGUIControlsInVarfieldDB(
    VARIANT* f_pdata,
    const int f_scan_index
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::UpdateGUIControlsInVarfieldDB");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMStudy::UpdateGUIControlsInVarfieldDB"));

    BOOL l_result = TRUE;

    //+Patni-MP/2009Mar17/Added/ACE-1/MCM0213-00131
    CString l_body_part_value = _T("");
    BSTR l_body_part_BSTR = NULL;           //Patni-MJC/2009Aug17/Modified/cpp test corrections
    //-Patni-MP/2009Mar17/Added/ACE-1/MCM0213-00131

    //+Patni-MP/2009Jun07/Added/ACE-2/Skip Prescan
    BSTR l_skip_prescan_BSTR = NULL;        //Patni-MJC/2009Aug17/Modified/cpp test corrections
    CString l_skip_prescan_value = _T("");
    //-Patni-MP/2009Jun07/Added/ACE-2/Skip Prescan
    long l_hold_value = 0;
    long l_autotransfer_value = 0;
    long l_viewdir_value = 0;
    long l_sar_region_value = 0;
    long l_autovoice_value = 0;

    BSTR l_comment = NULL;                  //Patni-MJC/2009Aug17/Modified/cpp test corrections
    CString l_scan_comment_lpt(_T(""));
    BSTR l_bstr_contrast_name = NULL;       //Patni-MJC/2009Aug17/Modified/cpp test corrections
    BSTR l_bstr_patient_orientation = NULL; //Patni-MJC/2009Aug17/Modified/cpp test corrections
    CString l_str_contrast_name(_T(""));
    CString l_str_patient_orient(_T(""));
    int l_contrast = 0;                 //Patni-MJC/2009Aug17/Modified/cpp test corrections
    CPqmCoilData* l_coil_data  = NULL;  //Patni-MJC/2009Aug17/Modified/cpp test corrections

    int l_length = -1;

    CString l_end_in(_T(""));
    CString l_side_up(_T(""));
    int l_count = 0;
    int l_int_end = 0, l_int_side = 0;  //Patni-MJC/2009Aug17/Modified/cpp test corrections
    VftPatori_t l_patori;

    int         n = 0, num_put = 0; //Patni-MJC/2009Aug17/Modified/cpp test corrections
    time_t      tmStamp = 0;        //Patni-MJC/2009Aug17/Modified/cpp test corrections
    VfFieldSpec_t   vfReq[8] = {0}; //Patni-MJC/2009Aug17/Modified/cpp test corrections

    //+Patni-DKH/2010Apr12/Added/Phase3#MCM0229-00187
    long l_move_couch = 0;

    BITFLD_DECL(l_mask, K_ALL_PR);
    BITFLD_INIT(l_mask, K_ALL_PR, 0);

    CPqmProtocol* l_current_protocol = GetProtocol(f_scan_index);

    if (NULL == l_current_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_current_protocol is NULL !!!"));
        return FALSE;
    }

    CPqmPrMode* l_pqm_pr_mode =  l_current_protocol->GetPqmPrMode();
    status_t l_status = E_ERROR;
    CString tmpStr(_T(""));

    switch ((e_control_id)f_pdata->wReserved1) { // Control ID

        case AUTO_VOICE:
            l_autovoice_value = long(f_pdata->intVal);

            if (E_NO_ERROR != SetProtocolValues(m_vfdb_handle,
                                                l_current_protocol->GetProtocol(),
                                                l_mask,
                                                K_PR_EXE_AUTO_VOICE,
                                                &l_autovoice_value,
                                                sizeof(long),
                                                NULL)) l_result = FALSE;

            l_current_protocol->SetExeAutoVoice(l_autovoice_value);
            break;

        case HOLD_VALUE:
            l_hold_value = long(f_pdata->intVal);
            l_coil_data = m_pqm_ptr->GetDataManager()->GetPQMCoilData();

            //+Patni-PJS/2010Sep22/Added/MVC008989

            /*if (f_pdata->wReserved2 == MOVE_COUCH &&  !(l_coil_data->GetInOutSideLimit(
                l_current_protocol->GetCoilDBTable(), &in_limit, &out_limit) == E_NO_ERROR
            && in_limit + out_limit > 0)) {
                l_result = false;
                break;
            }*/
            //-Patni-PJS/2010Sep16/Added/MVC008989
            if (E_NO_ERROR != SetProtocolValues(m_vfdb_handle,
                                                l_current_protocol->GetProtocol(),
                                                l_mask,
                                                K_PR_HOLD,
                                                &l_hold_value,
                                                sizeof(long),
                                                NULL)) l_result = FALSE;

            l_current_protocol->SetHoldScan(l_hold_value);
            break;

        case AUTO_TRANSFER:
            l_autotransfer_value = long(f_pdata->intVal);

            if (E_NO_ERROR != SetProtocolValues(m_vfdb_handle,
                                                l_current_protocol->GetProtocol(),
                                                l_mask,
                                                K_PR_AUTO_POSTPROC_FLG,
                                                &l_autotransfer_value,
                                                sizeof(long),
                                                NULL)) l_result = FALSE;

            l_current_protocol->SetAutoTransfer(l_autotransfer_value);
            break;

        case CONTRAST_AGENT: {
            //Tanoue/MVC5849
            l_bstr_contrast_name = (BSTR)f_pdata->byref;
            l_str_contrast_name = l_bstr_contrast_name;
            std::string l_mbs_contrast("");
            UTIL::Wcs2Mbs(&l_mbs_contrast, l_str_contrast_name);
            l_length = l_mbs_contrast.length() + 1;

            //SM4_Mangesh_Start
            l_contrast = f_pdata->wReserved3 ;
            //SM4_Mangesh_End

            if (E_NO_ERROR != SetProtocolValues(m_vfdb_handle,
                                                l_current_protocol->GetProtocol(),
                                                l_mask,
                                                K_PR_CONTRAST_NAME, const_cast<char*>(l_mbs_contrast.c_str()), l_length,
                                                K_PR_CONTRAST, &l_contrast, sizeof(int),
                                                NULL))l_result = FALSE;

            //        path[0].subtree_name = SVN_ST_STUDY;
            //        path[0].index = l_current_protocol->GetProtocol();
            n = 0;

            VFF_SET_ARGS(vfReq[n], SVN_SCH_CONTRAST, SVT_SCH_CONTRAST, l_length,
                         const_cast<char*>(l_mbs_contrast.c_str()), VFO_REPLACE);

            n++;

            l_status = m_vfdb_handle->PutFields(NULL, SVD_ST_STUDY, vfReq, n, &num_put, &tmStamp);

            if (E_NO_ERROR != l_status || num_put != n) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
            }

            l_current_protocol->SetContrastName(l_mbs_contrast.c_str());

            //Patni-Hemant/2010Jun8/Added/MaFT/MVC008201
            l_current_protocol->GetPqmPrMode()->SetContrast(l_contrast > 0);

            break;
        }


        case SAR_REG:
            l_sar_region_value = long(f_pdata->intVal);

            /*if(E_NO_ERROR !=SetProtocolValues(m_vf_handle,
             l_current_protocol->GetProtocol(),
             l_mask,
             K_PR_ANATOMY,
             &l_sar_region_value,
             sizeof(long),
             NULL))l_result = false;*/
            if (E_NO_ERROR != SetProtocolValue(m_vfdb_handle,
                                               l_current_protocol,
                                               K_PR_ANATOMY,
                                               &l_sar_region_value,
                                               sizeof(long),
                                               0, 0))l_result = FALSE;

            l_pqm_pr_mode->SetAnatomy(l_sar_region_value);
            tmpStr.Format(_T("l_pqm_pr_mode->SetAnatomy(l_sar_region_value) = %d"), l_sar_region_value);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, tmpStr);

            break;

        case PATIENT_ORIENTATION:
            l_bstr_patient_orientation = (BSTR) f_pdata->byref;
            l_str_patient_orient = l_bstr_patient_orientation;

            l_count = l_str_patient_orient.Find(_T(","), 0);
            l_end_in = l_str_patient_orient.Mid(0, l_count);
            l_side_up = l_str_patient_orient.Mid((l_count + 1), l_str_patient_orient.GetLength() + 1);

            l_int_end = _ttoi((LPCTSTR)l_end_in);
            l_int_side = _ttoi((LPCTSTR)l_side_up);

            l_pqm_pr_mode->SetPatoriEndIn(l_int_end);
            l_pqm_pr_mode->SetPatoriSideUp(l_int_side);
            l_patori.side_up = l_pqm_pr_mode->GetPatoriSideUp();
            l_patori.end_in = l_pqm_pr_mode->GetPatoriEndIn();

            if (E_NO_ERROR != SetProtocolValues(m_vfdb_handle,
                                                l_current_protocol->GetProtocol(),
                                                l_mask,
                                                K_PR_PATIENT_ORIENT, &l_patori,
                                                sizeof(VftPatori_t),
                                                NULL))l_result = FALSE;

            break;


        case VIEW_DIRECTION:
            l_viewdir_value = long(f_pdata->intVal);

            if (E_NO_ERROR != SetProtocolValues(m_vfdb_handle,
                                                l_current_protocol->GetProtocol(),
                                                l_mask,
                                                K_PR_VIEW_DIR,
                                                &l_viewdir_value,
                                                sizeof(long),
                                                NULL))l_result = FALSE;

            l_pqm_pr_mode->SetViewDir(l_viewdir_value);
            break;

            //+Patni-MP/2009Mar17/Added/ACE-1/MCM0213-00131
        case BODY_PART:
            l_body_part_BSTR = BSTR(f_pdata->byref);
            l_body_part_value = l_body_part_BSTR;

            if (E_NO_ERROR != BodyPartSetVF(l_current_protocol->GetProtocol(),
                                            l_body_part_value)) l_result = FALSE;

            //+Patni-HEMANT/ADDED On 9/10/2009 5:26:34 PM/ MVC003979
            if (l_result) {
                l_pqm_pr_mode->SetBodyPart(l_body_part_value);
            }

            //-Patni-HEMANT/ADDED On 9/10/2009 5:26:34 PM/ MVC003979

            break;
            //-Patni-MP/2009Mar17/Added/ACE-1/MCM0213-00131

            //+Patni-Hemant/2010Apr27/Added/JaFT/IR-137
        case LATERALITY:

            l_result = FALSE;

            if (SaveLaterality(l_current_protocol->GetProtocol(),
                               f_pdata->intVal)) {
                l_pqm_pr_mode->SetLaterality(f_pdata->intVal);

                l_result = TRUE ;
            }

            break ;
            //-Patni-Hemant/2010Apr27/Added/JaFT/IR-137

            //+Patni-MP/2009Jun07/Added/ACE-2/Skip Prescan
        case SKIP_PRESCAN:
            l_skip_prescan_BSTR = BSTR(f_pdata->byref);
            l_skip_prescan_value = l_skip_prescan_BSTR;

            if (E_NO_ERROR != SetAgentValueVF(l_current_protocol->GetProtocol(),
                                              l_skip_prescan_value)) l_result = FALSE;

            break;

            //-Patni-MP/2009Jun07/Added/ACE-2/Skip Prescan
        case SCAN_COMMENT:
        case SCAN_ID: {
            //Tanoue/MVC5849
            l_comment = (BSTR)f_pdata->byref;
            l_scan_comment_lpt = l_comment;
            std::string l_mbs_comment("");
            UTIL::Wcs2Mbs(&l_mbs_comment, l_scan_comment_lpt);
            l_length = l_mbs_comment.length() + 1;

            if (E_NO_ERROR != SetProtocolValues(m_vfdb_handle,
                                                l_current_protocol->GetProtocol(),
                                                l_mask,
                                                ((e_control_id)f_pdata->wReserved1 == SCAN_COMMENT) ?  K_PR_SCAN_COMMENTS : K_PR_SCAN_ID,
                                                const_cast<char*>(l_mbs_comment.c_str()),
                                                l_length,
                                                NULL)) l_result = FALSE;

            //+Patni-MP/Modified/2009Mar12/DefectNo#PSP239

            if (l_result) {
                //+ Patni-MSN/2009Sept02/Added/Comment Not Updated - Internal defect
                if ((e_control_id)f_pdata->wReserved1 == SCAN_COMMENT) {
                    l_current_protocol->SetScanComment(l_mbs_comment.c_str());

                } else {
                    l_current_protocol->SetScanID(l_mbs_comment.c_str());
                    m_pqm_ptr->UpdateSARInfoOnPage();
                }

                //- Patni-MSN/2009Sept02/Added/Comment Not Updated - Internal defect
            }

            //-Patni-MP/Modified/2009Mar12/DefectNo#PSP239

            if (l_current_protocol->GetAcqStatus() == C_PR_STATUS_DONE) {
                //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
                CPQMLogMgr::GetInstance()->DisplayScanError(IDS_INFORM_COMMENT_DONE, _T("IDS_INFORM_COMMENT_DONE"),
                        SCAN_ERR_NOTIFY, _T("pqmstudy.cpp"));
            }

            //+ Patni-MSN/2009Sept02/Added/Comment Not Updated - Internal defect

            //- Patni-MSN/2009Sept02/Added/Comment Not Updated - Internal defect
            break ;
        }

        //+Patni-DKH/2010Apr12/Added/Phase3#MCM0229-00187
        case MOVE_COUCH_METHOD:
            l_move_couch = long(f_pdata->intVal);

            if (E_NO_ERROR != SetProtocolValues(m_vfdb_handle,
                                                l_current_protocol->GetProtocol(),
                                                l_mask,
                                                K_PR_MOVE_COUCH_METHOD_FLAG,
                                                &l_move_couch,
                                                sizeof(long),
                                                NULL)) l_result = FALSE;

            l_current_protocol->SetcouchMethodFlg(l_move_couch);

            if (l_move_couch == VFC_MOVE_COUCH_METHOD_PLAN_CENTER_POSI || l_move_couch == VFC_MOVE_COUCH_METHOD_NONE) {
                int l_copy_source_prot = -1;

                if (E_NO_ERROR != SetProtocolValues(m_vfdb_handle,
                                                    l_current_protocol->GetProtocol(),
                                                    l_mask,
                                                    K_PR_COPY_SOURCE_PROTOCOL,
                                                    &l_copy_source_prot,
                                                    sizeof(long),
                                                    NULL)) l_result = FALSE;
            }

            break;
            //-Patni-DKH/2010Apr12/Added/Phase3#MCM0229-00187
            //-------------------------------------------------

        default:
            break;

    }

    return l_result;
}

//***************************Method Header*************************************
//Method Name    :ListCopyProt()
//Author         :PATNI/SG
//Purpose        :
//*****************************************************************************
CPqmProtocol* CPQMStudy::ListCopyProt(
    CVarFieldHandle* const f_vf_handle,
    const int f_to_prot,
    const int f_to_pos
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::ListCopyProt");
    //+Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(f_vf_handle);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of ListCopyProt"));

    BITFLD_DECL(mask, MAX_NODES);

    int l_status = CheckImgGr(m_vfdb_handle, f_to_prot);
    if_error(l_status) {

        //CopyError(f_to_prot,NULL,__FILE__,__LINE__);
        return NULL;
    }

    int l_acq_status = C_PR_STATUS_WAIT;
    memset(mask, 0, sizeof(mask));

    if (E_NO_ERROR != SetProtocolValues(m_vfdb_handle, f_to_prot, mask, K_PR_ACQ_STATUS,
                                        &l_acq_status, sizeof(int32_t), NULL)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetProtocolValues() failed."));
    }

    /** For SVN_PLN_HOLD **/
    memset(mask, 0, sizeof(mask));
    int32_t l_hold_scan = 0;
    GetProtocolValues(m_vfdb_handle,
                      f_to_prot,
                      mask,
                      K_PR_HOLD,
                      &l_hold_scan,
                      NULL,
                      NULL);

    if (!(l_hold_scan & VFC_HOLD_PRE_SCAN)) {
        l_hold_scan |= VFC_HOLD_PRE_SCAN;
    }

    if (l_hold_scan & VFC_HOLD_APC)
        l_hold_scan ^= VFC_HOLD_APC;

    memset(mask, 0, sizeof(mask));

    if (E_NO_ERROR != SetProtocolValues(m_vfdb_handle, f_to_prot, mask, K_PR_HOLD, &l_hold_scan,
                                        sizeof(int32_t), NULL)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetProtocolValues() failed."));
    }

    CPqmProtocol* l_obj = NULL;
    if_no_error(AddProtocol(m_vfdb_handle, f_to_prot, f_to_pos, TRUE, &l_obj)) {
        return l_obj;
    }
    return NULL;
}
//***************************Method Header*************************************
//Method Name    :AMBReArrange()
//Author         :iGATE
//Purpose        :
//*****************************************************************************
int CPQMStudy::AMBReArrange(const int f_drag_indx, const int f_drop_index)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::AMBRearrange");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of AMBRearrange"));

    CString l_print_str(_T(""));
    l_print_str.Format(_T("Drag Index = %d : Drop Index = %d"), f_drag_indx, f_drop_index);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_print_str);

    CScopedLock l_scoped_lock(&m_protocol_vec_mutex);

    ClibVfToUiAdapter vuiAdapter(m_vfdb_handle);
    int l_status = vuiAdapter.VuiSetAcqOrder(f_drag_indx, f_drop_index);

    protocolvec::iterator l_start = NULL, l_middle = NULL, l_end = NULL; //Patni-MJC/2009Aug17/Modified/cpp test corrections
    protocolvec::iterator l_iterator = m_protocol_vec.begin();

    if (f_drag_indx < f_drop_index) {
        l_start = l_iterator + f_drag_indx;
        l_middle = l_start + 1;
        l_end = l_iterator + f_drop_index + 1;

    } else  if (f_drag_indx > f_drop_index) {
        l_start = l_iterator + f_drop_index;
        l_middle = l_iterator + f_drag_indx;
        l_end = l_iterator + f_drag_indx + 1;

    } else {
        return S_FALSE;
    }

    std::rotate(l_start, l_middle, l_end);

    RenumberPositionsInVector();
    SearchScanMode(m_vfdb_handle, K_MODE_SET_IT);

    return l_status;
}


//****************************Method Header************************************
//Method Name   :ReArrange()
//Author        :PATNI/SG
//Purpose       :
//*****************************************************************************
int CPQMStudy::ReArrange(
    const int f_drag_indx,
    const int f_drop_index
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::ReArrange");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of ReArrange"));

    CString l_print_str(_T(""));
    l_print_str.Format(_T("Drag Index = %d : Drop Index = %d"), f_drag_indx, f_drop_index);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_print_str);

    //Patni-DKH/2011Mar04/Added/TMSC-REDMINE-1486
    CScopedLock l_scoped_lock(&m_protocol_vec_mutex);

    ClibVfToUiAdapter vuiAdapter(m_vfdb_handle);
    int l_status = vuiAdapter.VuiSetAcqOrder(f_drag_indx, f_drop_index);

    protocolvec::iterator l_start = NULL, l_middle = NULL, l_end = NULL; //Patni-MJC/2009Aug17/Modified/cpp test corrections
    protocolvec::iterator l_iterator = m_protocol_vec.begin();

    if (f_drag_indx < f_drop_index) {
        l_start = l_iterator + f_drag_indx;
        l_middle = l_start + 1;
        l_end = l_iterator + f_drop_index + 1;

    } else  if (f_drag_indx > f_drop_index) {
        l_start = l_iterator + f_drop_index;
        l_middle = l_iterator + f_drag_indx;
        l_end = l_iterator + f_drag_indx + 1;

    } else {
        return S_FALSE;
    }

    std::rotate(l_start, l_middle, l_end);

    /////Added by Meghana On 8/2/2008 3:51:35 PM
    RenumberPositionsInVector();
    //Send +1 coz the CopyScanMode function assumes the index position from 1
    CopyScanMode(m_vfdb_handle, (f_drop_index + 1), TRUE);
    SearchScanMode(m_vfdb_handle, K_MODE_SET_IT);

    if (f_drag_indx < f_drop_index)
        SetNewSAR(f_drag_indx);

    else
        SetNewSAR(f_drop_index);

    //////End Of addition By Meghana On8/2/2008 3:51:48 PM
    //+ Patni-PJS/2009Jun05/Added/JFT# 251,251,251,IR(5)
    //+Patni-MJC/2009Aug27/Modified/cpp test corrections
    if (m_pqm_ptr == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return S_FALSE;
    }

    //-Patni-MJC/2009Aug27/Modified/cpp test corrections
    return l_status;
}

//+Patni-SS/2010July6/NFD001AutoMap
//***************************Method Header*************************************
//Method Name    :CopyScanModeFrom
//Author         :PATNI/SS/NFD001AutoMap
//Purpose        :
//
//*****************************************************************************
bool CPQMStudy::CopyScanModeFrom(CVarFieldHandle*	const f_vfd,
                                 const int		from_pos,
                                 const int		to_pos,
                                 const bool_t	copyContrast)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::CopyScanModeFrom");
    CString l_trace_msg(_T(""));
    l_trace_msg .Format(_T("Inside CopyScanModeFrom, from_pos = %d, to_pos = %d, copyContrast = %d"), from_pos, to_pos, copyContrast);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              l_trace_msg);

    CCoil	l_coil_p;
    CCoil           l_coil;
    int		l_obj_trCoil, l_trCoil;
    CPqmPrMode	l_modeEntry;

    bool_t		l_enable;
    CPqmCoilData*    l_coil_data = CDataManager::GetInstance()->GetPQMCoilData();


    BITFLD_DECL(mask, MAX_NODES);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMStudy::CopyScanModeFrom"));

    CPqmProtocol* l_obj = GetItemFromPosition(to_pos);

    if (NULL == l_obj  || PQ_FILLER == l_obj->GetProtocol()) {
        return false;
    }

    memset(mask, 0, sizeof(mask));
    status_t l_status = GetProtocolValues
                        (m_vfdb_handle, l_obj->GetProtocol(), mask,
                         K_PR_TR_COIL,  &l_obj_trCoil, NULL,
                         NULL);


    CPqmProtocol* l_from_obj = GetItemFromPosition(from_pos);

    if (NULL == l_from_obj  || PQ_FILLER == l_from_obj->GetProtocol()) {
        return false;
    }

    l_modeEntry.Copy(l_from_obj->GetPqmPrMode());
    PqmSaturnCoil_t* l_saturnEntry = l_from_obj->GetSaturnCoil();

    if (NULL == l_saturnEntry) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_saturnEntry pointer is NULL"));
        return false;
    }

    int l_prot = l_from_obj->GetProtocol();

    if (m_pqm_ptr->IsSaturnCoilSystem()) {
        /* Saturn Coil */
        PqmCoil_t* l_tmp_coil = NULL;

        if (!GetTxCoilInSaturn(l_from_obj->GetCoilDBTable(), &l_tmp_coil)) {
            l_tmp_coil = NULL;
        }

        l_coil.SetCoil(l_tmp_coil);

    } else {
        if (NULL != l_coil_data) {
            l_coil_data->PqmCoilGetTxCoil(l_modeEntry.GetRecvCoilId(), l_coil_p);

        } else {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_coil_data pointer is NULL"));
        }
    }

    if (NULL != l_coil_p.GetCoil()) {
        l_trCoil = l_coil_p.GetCoilId();

    } else {
        l_trCoil = 0;
    }

    if (!copyContrast)
        l_modeEntry.SetContrast(l_obj->GetPqmPrMode()->GetContrast());

    if (m_pqm_ptr->IsSaturnCoilSystem()) {

        l_modeEntry.SetRecvCoilId(l_obj->GetPqmPrMode()->GetRecvCoilId());
        int l_return = memcmp(l_saturnEntry, l_obj->GetSaturnCoil(), sizeof(PqmSaturnCoil_t));
        l_enable = (l_modeEntry.Compare(l_obj->GetPqmPrMode())) ||
                   (l_trCoil != l_obj_trCoil) || l_return ;

    } else {
        l_enable = (l_modeEntry.Compare(l_obj->GetPqmPrMode()) || l_trCoil != l_obj_trCoil);
    }

    if (l_enable) {
        CPqmPrMode* copyToMode = l_obj->GetPqmPrMode();

        if (NULL == copyToMode) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("copyToMode points to Null."));
            return false;
        }

        copyToMode->SetAnatomy(l_modeEntry.GetAnatomy());
        CString tmpStr(_T(""));
        tmpStr.Format(_T("copyToMode->SetAnatomy(l_modeEntry.GetAnatomy()) = %d"), l_modeEntry.GetAnatomy());
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, tmpStr);
        BodyPartSetVF(l_obj->GetProtocol(), l_modeEntry.GetBodyPart());
        copyToMode->SetBodyPart(l_modeEntry.GetBodyPart());
        SaveLaterality(l_obj->GetProtocol(), l_modeEntry.GetLaterality());
        copyToMode->SetLaterality(l_modeEntry.GetLaterality());

        copyToMode->SetContrast(l_modeEntry.GetContrast());
        copyToMode->SetCouchOffset(l_modeEntry.GetCouchOffset());
        copyToMode->SetCouchPos(l_modeEntry.GetCouchPos());
        copyToMode->SetHomePos(l_modeEntry.GetHomePos());
        copyToMode->SetPatoriEndIn(l_modeEntry.GetPatoriEndIn());
        copyToMode->SetPatoriSideUp(l_modeEntry.GetPatoriSideUp());
        copyToMode->SetRecvCoilId(l_modeEntry.GetRecvCoilId());
        copyToMode->SetScanNumber(l_modeEntry.GetScanNumber());
        copyToMode->SetViewDir(l_modeEntry.GetViewDir());

        l_obj->SetSaturnCoil(l_saturnEntry);




        if (m_pqm_ptr->IsSaturnCoilSystem()) {
            /* Saturn Coil */
            int i;
            PqmSaturnRcvCoil_t* l_coil_db = l_obj->GetCoilDBTable();

            if (NULL == l_coil_db) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_coil_db points to Null."));
                return false;
            }

            for (i = 0; i < l_coil_db->numCoil; i++) {
                (l_obj->GetSarInfo())->receiverCoil[i]  = l_coil_db->coiltbl_p[0]->coil_id;
            }

        } else {

            (l_obj->GetSarInfo())->receiverCoil[0] = l_obj->GetPqmPrMode()->GetRecvCoilId();
        }

        l_obj->GetSarInfo()->transmiterCoil = l_trCoil;

        CString l_str(_T(""));
        l_str.Format(_T("rcvCoilId = %d\n l_trCoil    = %d\n contrast  = %d\n  couchPos  = %f\n anatomy  = %d\n view_dir  = %d\n "),
                     l_obj->GetPqmPrMode()->GetRecvCoilId(), l_trCoil, l_obj->GetPqmPrMode()->GetContrast(),
                     l_obj->GetPqmPrMode()->GetCouchPos(), l_obj->GetPqmPrMode()->GetAnatomy(), l_obj->GetPqmPrMode()->GetViewDir());
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);


        memset(mask, 0, sizeof(mask));

        CPqmPrMode* l_ptr_pqmmode = l_obj->GetPqmPrMode();

        if (NULL == l_ptr_pqmmode) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_ptr_pqmmode pointer is NULL"));
            return false;
        }

        int l_rcvCoilId = l_ptr_pqmmode->GetRecvCoilId();
        int l_lanatomy = l_ptr_pqmmode->GetAnatomy();
        int l_scanNumber = l_ptr_pqmmode->GetScanNumber();
        float l_couch_Pos = l_ptr_pqmmode->GetCouchPos();
        float l_home_Pos = l_ptr_pqmmode->GetHomePos();
        float l_couch_offset = l_ptr_pqmmode->GetCouchOffset();
        int l_contrast = l_ptr_pqmmode->GetContrast();
        int l_view_dir = l_ptr_pqmmode->GetViewDir();

        VftPatori_t l_patori;
        l_patori.side_up = l_ptr_pqmmode->GetPatoriSideUp();
        l_patori.end_in = l_ptr_pqmmode->GetPatoriEndIn();
        l_status = SetProtocolValues
                   (m_vfdb_handle, l_obj->GetProtocol(), mask,
                    K_PR_TR_COIL, &l_trCoil,      sizeof(int32_t),
                    K_PR_RC_COIL, &l_rcvCoilId,   sizeof(int32_t),
                    K_PR_ANATOMY, &l_lanatomy,    sizeof(int32_t),
                    K_PR_SCAN_NUMBER, &l_scanNumber,  sizeof(int32_t),
                    K_PR_COUCH,   &l_couch_Pos,   sizeof(flt32_t),
                    K_PR_HOME_POSITION, &l_home_Pos,  sizeof(flt32_t),
                    K_PR_COUCH_OFFSET,    &l_couch_offset,    sizeof(flt32_t),
                    K_PR_CONTRAST,    &l_contrast,    sizeof(int32_t),
                    K_PR_VIEW_DIR,    &l_view_dir,    sizeof(int32_t),
                    K_PR_PATIENT_ORIENT,  &(l_patori),    sizeof(VftPatori_t),
                    NULL);

        if (E_NO_ERROR != l_status) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetProtocolValues failed."));
        }

        BodyPartSetVF(l_obj->GetProtocol(), l_obj->GetPqmPrMode()->GetBodyPart());

        if (m_pqm_ptr->IsSaturnCoilSystem()) {

            int l_prot = l_obj->GetProtocol();

            PutSaturnCoil(m_vfdb_handle, l_prot, l_obj->GetSaturnCoil());

            PutConnectedCoil(m_vfdb_handle, l_prot, l_obj);

            if (E_NO_ERROR != l_coil_data->PqmAllCoilGetSaturn(l_obj->GetSaturnCoil(),
                    l_obj->GetCoilDBTable())) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PqmAllCoilGetSaturn failed"));
            }

            PutRecvrCoil(m_vfdb_handle, l_prot, l_obj);

            SetProtValues(m_vfdb_handle, l_prot, mask,
                          K_PR_RC_COIL_NAME,
                          (l_obj->GetSaturnCoil())->CoilName,
                          strlen((l_obj->GetSaturnCoil())->CoilName) + 1,
                          NULL);
            SetTxCoilInSaturn(f_vfd, l_obj, l_obj->GetCoilDBTable()) ;
        }

    }

    if (l_prot > -1 && copyContrast) {
        char		buf[1024];
        int		size;

        memset(mask, 0, sizeof(mask));
        l_status = GetProtocolValues
                   (m_vfdb_handle, l_prot, mask,
                    K_PR_CONTRAST_NAME, buf, &size,
                    NULL);

        if (E_NO_ERROR == l_status  && !BITFLD_ON(mask, K_PR_CONTRAST_NAME)) {
            memset(mask, 0, sizeof(mask));
            SetProtocolValues
            (f_vfd, l_obj->GetProtocol(), mask,
             K_PR_CONTRAST_NAME, buf, size,
             NULL);
        }

    }

    return true;
}
//-Patni-SS/2010July6/NFD001AutoMap


//+Patni-DH+ARD/2009Dec03/Added/DeFT# MVC005787+MSA0248-00087
//****************************Method Header************************************
//Method Name   : PqmUiSetScanOffset()
//Author        : PATNI\DHANESH+ARD
//Purpose       :
//*****************************************************************************
BOOL CPQMStudy::PqmUiSetScanOffset(
    CPqmProtocol* f_obj,
    VARIANT** f_scanoffset
)
{
    //+Patni-PJS/2010Feb18/Added/DeFT# PROPOSAL_01-MEM_LEAK-COMMON_CDR_07
    LPCTSTR FUNC_NAME = _T("CPQMStudy::PqmUiSetScanOffset");

    if (NULL == (*f_scanoffset)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("(*f_scanoffset) is NULL"));
        return FALSE;
    }

    if (f_obj == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_obj is NULL"));
        return FALSE;
    }

    //-Patni-PJS/2010Feb18/Added/DeFT# PROPOSAL_01-MEM_LEAK
    f32vec3_t*   l_objvec = new f32vec3_t;

    //+Patni-PJS/2010Feb18/Added/DeFT# PROPOSAL_01-MEM_LEAK
    if (NULL == l_objvec) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_objvec is NULL"));
        return FALSE;
    }

    //-Patni-PJS/2010Feb18/Added/DeFT# PROPOSAL_01-MEM_LEAK
    memset(l_objvec, 0, sizeof(f32vec3_t));
    //+Patni-SS/Modified/20091217/MVC005787+MSA0248-00087
    PqmUiConvertVector(&(f_obj->GetPqmPrMode()->GetPatientOrientation()),
                       TRUE, &(f_obj->GetOffset()), l_objvec); // TRUE: when to display offset values on GUI & scanoffset dialog.
    //-Patni-SS/Modified/20091217/MVC005787+MSA0248-00087

    if (NULL == l_objvec) {
        //+Patni-PJS/2010Feb18/Added/DeFT# PROPOSAL_01-MEM_LEAK
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_objvec is NULL"));
        return FALSE ;
    }

    l_objvec->x *= OFFSET_RATE;
    l_objvec->y *= OFFSET_RATE;
    l_objvec->z *= OFFSET_RATE;

    if (*f_scanoffset) {

        (*f_scanoffset)->byref = l_objvec;

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,  _T("(*f_scanoffset) is NULL"));
        return FALSE;
    }

    return TRUE;
}
//-Patni-DH+ARD/2009Dec03/Added/DeFT# MVC005787+MSA0248-00087


//+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : OnOffAsCompass
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPQMStudy::OnOffAsCompass(
    const int f_protocol,
    const bool f_OnOff
)
{
    BOOL l_return_value = E_ERROR;

    CPqmProtocol* l_protocol = GetProtocol(f_protocol);

    if (l_protocol) {

        BITFLD_DECL(mask, K_ALL_PR);
        memset(mask, 0, sizeof(mask));

        int l_OnOff = f_OnOff ? VFC_CDS_FLAG_ON : VFC_CDS_FLAG_OFF ;

        l_return_value = SetProtocolValues(m_vfdb_handle, f_protocol, mask,
                                           K_PR_CDS_FLAG, &l_OnOff, sizeof(int), NULL);

        if (l_return_value == E_NO_ERROR) {
            l_protocol->SetASCOMPASSFlag(f_OnOff);
        }
    }

    return (l_return_value == E_NO_ERROR) ;
}
//-Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement

//****************************Method Header************************************
//Method Name   :PutSaturnCoil()
//Author        :PATNI/AMT
//Purpose       :Writes Saturn coil information to Study File.
//*****************************************************************************
int CPQMStudy::PutSaturnCoil(
    CVarFieldHandle* const f_vf_handle,
    const int             f_prot,
    PqmSaturnCoil_t* f_saturn_Coil
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::PutSaturnCoil");
    int             n = 0, s = 0;
    VfPathElem_t    l_pelm[2] = {0};
    VfFieldSpec_t   l_vfReq[6] = {0};

    CVarFieldHandle* l_use_vf_handle = f_vf_handle ? f_vf_handle : m_vfdb_handle;

    BITFLD_DECL(sub_mask, K_ALL_PR);

    PqmSaturnCoil_t l_saturn_Coil ;

    if (f_saturn_Coil == NULL) {
        //
        return E_ERROR;
    }

    l_saturn_Coil = *(f_saturn_Coil);

    //DB_FUNC_ENTER("[PqmPrPutSaturnCoil]:");

    l_pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_pelm[SVD_ST_PROTOCOL - 1].index = f_prot;

    // Set Number of Section
    BITFLD_INIT(sub_mask, K_ALL_PR, 0);

    //+Patni-AMT/2009Mar08/Added/PSP1#5 VFLOCK Hang issues
    CString l_log_msg = _T("");
    l_log_msg.Format(_T("K_PR_NUM_SECTION_RECVR_COIL = %d"), f_saturn_Coil->numSection);

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_log_msg);

    //-Patni-AMT/2009Mar08/Added/PSP1#5 VFLOCK Hang issues

    SetProtValues(f_vf_handle, f_prot, sub_mask,
                  K_PR_NUM_SECTION_RECVR_COIL, &l_saturn_Coil.numSection,
                  sizeof(l_saturn_Coil.numSection),
                  NULL);
    //Added by Meghana On 8/29/2008 4:05:15 PM

    if (l_saturn_Coil.numSection > 0) {
        n = 0;
        s = l_saturn_Coil.numSection * sizeof(VftSectionRecvrCoil_t) ;
        VFF_SET_ARGS(l_vfReq[n], SVN_PLN_SECTION_RECVR_COIL,
                     SVT_PLN_SECTION_RECVR_COIL,
                     s, &l_saturn_Coil.RcvCoil, VFO_REPLACE);
        n++;

        s = l_saturn_Coil.numSection * sizeof(VftSectionSignal_t) ;
        VFF_SET_ARGS(l_vfReq[n], SVN_PLN_SECTION_RECVR_COIL_SIGNAL,
                     SVT_PLN_SECTION_RECVR_COIL_SIGNAL,
                     s, &l_saturn_Coil.Signal, VFO_REPLACE);
        n++;
        //+Patni-HAR/2010Feb10/Modified/CDS Requirement/Compatible to V9.50
    }

    //+Patni-Hemant/2010Mar25/Added/ACE-Phase#3/CDS Requirement
    //The following is not as per Vantage.
    //But it is also valid that we remove all the coils..
    //if (l_saturn_Coil.CoilList.num) {
    if (l_saturn_Coil.CoilList.num >= 0) {
        //-Patni-Hemant/2010Mar25/Added/ACE-Phase#3/CDS Requirement

        VFF_SET_ARGS(l_vfReq[n], SVN_PLN_RECVR_COIL_SHOW_LIST,
                     SVT_PLN_RECVR_COIL_SHOW_LIST,
                     sizeof(l_saturn_Coil.CoilList), &l_saturn_Coil.CoilList,
                     VFO_REPLACE);
        n++;

        l_saturn_Coil.CoilListcoordinate.num = l_saturn_Coil.CoilList.num;
        VFF_SET_ARGS(l_vfReq[n], SVN_PLN_RECVR_COIL_COORDINATE_LIST,
                     SVT_PLN_RECVR_COIL_COORDINATE_LIST,
                     sizeof(l_saturn_Coil.CoilListcoordinate), &l_saturn_Coil.CoilListcoordinate,
                     VFO_REPLACE);
        n++;
    }

    status_t        l_sts = E_ERROR;

    if (n > 0) {
        //-Patni-HAR/2010Feb10/Modified/CDS Requirement/Compatible to V9.50
        time_t          l_tm = {0};
        int l_num = 0;
        l_sts = f_vf_handle->PutFields(l_pelm, SVD_ST_PROTOCOL, l_vfReq, n, &l_num, &l_tm);

        if (l_sts != E_NO_ERROR || n != l_num) {    // Error Handling
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
            l_sts = E_ERROR ;
        }
    }

    return l_sts ;
}

//****************************Method Header************************************
//Method Name   : PutConnectedCoil
//Author        : PATNI/AMT
//Purpose       : Writes connected coil information to study file
//*****************************************************************************
status_t CPQMStudy::PutConnectedCoil(
    CVarFieldHandle* const f_vf_handle,
    const int f_prot,
    CPqmProtocol* f_pr_obj
)
{

    LPCTSTR FUNC_NAME = _T("CPQMStudy::PutConnectedCoil");

    //+Patni-MJC/2009Aug27/Modified/cpp test corrections
    if (m_pqm_ptr == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug27/Modified/cpp test corrections
    CDataManager*    l_data_mgr = m_pqm_ptr->GetDataManager();

    VfFieldSpec_t   l_vfReq[1] = {0};
    int32_t         l_connected_coil[MAXIMUM_COIL_IN_PORT] = {0};
    bool            l_wb_flg = false;
    CCoil           l_coil ;

    BITFLD_DECL(sub_mask, K_ALL_PR);

    // DB_FUNC_ENTER("[PqmPrPutConnectedCoil]:");
    VfPathElem_t    l_pelm[2] = {0};
    memset(l_pelm, 0, sizeof(VfPathElem_t) * 2);    //Patni-MJC/2009Aug17/Modified/cpp test corrections
    l_pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_pelm[SVD_ST_PROTOCOL - 1].index = f_prot;

    memset(l_vfReq, 0, sizeof(VfFieldSpec_t) * 1);  //Patni-MJC/2009Aug17/Modified/cpp test corrections

    memset(l_connected_coil, 0, sizeof(l_connected_coil));
    //Modified by Meghana On 8/29/2008 1:31:35 PM
    sarInfo_t* coil_sar_info = f_pr_obj->GetSarInfo();
    memset(coil_sar_info->connectedCoil, 0, sizeof(coil_sar_info->connectedCoil));
    ///////////////////

    PqmSaturnCoil_t* l_satrun_coil = f_pr_obj->GetSaturnCoil();

    for (int i = 0; i < l_satrun_coil->CoilList.num; i++) {
        if (l_satrun_coil->CoilList.coillist[i].coilID == WBCOIL_PHYSICAL_ID) {
            l_wb_flg = true;
        }

        l_data_mgr->GetPQMCoilData()->PqmCoilidGetSaturnCoil(
            l_satrun_coil->CoilList.coillist[i].coilID,
            l_satrun_coil->CoilList.coillist[i].portID[0],
            l_satrun_coil->CoilList.coillist[i].channel,
            l_coil);

        if (l_coil.GetCoil() != NULL) {
            l_connected_coil[i] = l_coil.GetCoilId();
            (f_pr_obj->GetSarInfo())->connectedCoil[i] = l_coil.GetCoilId();
        }
    }

    if (!l_wb_flg) {
        if (l_coil.GetCoil() != NULL) {

            l_data_mgr->GetPQMCoilData()->PqmCoilidGetSaturnCoil(
                WBCOIL_PHYSICAL_ID,  'A', NULL, l_coil);

            l_connected_coil[i] = l_coil.GetCoilId() ;
            (f_pr_obj->GetSarInfo())->connectedCoil[i] = l_coil.GetCoilId();
            i++;
        }
    }

    // Set Number of Section
    BITFLD_INIT(sub_mask, K_ALL_PR, 0);
    status_t        l_sts   = E_NO_ERROR;

    if (i) {
        int n = 0;

        int s = i * sizeof(int32_t) ;
        VFF_SET_ARGS(l_vfReq[n], SVN_EXE_CONNECTED_COIL, SVT_EXE_CONNECTED_COIL,
                     s, l_connected_coil, VFO_REPLACE);
        n++;

        //+Patni-SS/2009Apr01/PSP1#VFLOCK
        CString l_log_msg = _T("");
        l_log_msg.Format(_T("SVN_EXE_CONNECTED_COIL =%d "), l_connected_coil);
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_log_msg);

        //-Patni-SS/2009Apr01/PSP1#VFLOCK
        time_t          l_tm    = {0};
        int l_num = 0;
        l_sts = f_vf_handle->PutFields(l_pelm, SVD_ST_PROTOCOL, l_vfReq, n,
                                       &l_num, &l_tm);

        if (l_sts != E_NO_ERROR || n != l_num) {    // Error Handling
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
            l_sts = E_ERROR ;
        }
    }

    return l_sts ;
}

//****************************Method Header************************************
//Method Name   : PutRecvrCoil
//Author        : PATNI/AMT
//Purpose       : Writes receiver coil information to study file
//*****************************************************************************
status_t CPQMStudy::PutRecvrCoil(
    CVarFieldHandle* const f_vf_handle,
    const int f_prot,
    CPqmProtocol* f_pr_obj
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::PutRecvrCoil");

    //+Patni-MJC/2009Aug27/Modified/cpp test corrections
    if (f_pr_obj == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("f_pr_obj Pointer is null"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug27/Modified/cpp test corrections

    VfPathElem_t    l_pelm[2] = {0};
    VfFieldSpec_t   l_vfReq[1] = {0};
    int32_t         l_rcver_coil[MAXIMUM_COIL_IN_PORT] = {0};

    BITFLD_DECL(sub_mask, K_ALL_PR);

    //DB_FUNC_ENTER("[PqmPrPutRecvrCoil]:");

    memset(l_pelm, 0, sizeof(VfPathElem_t) * 2);    //Patni-MJC/2009Aug17/Modified/cpp test corrections
    memset(l_vfReq, 0, sizeof(VfFieldSpec_t) * 1);  //Patni-MJC/2009Aug17/Modified/cpp test corrections
    l_pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_pelm[SVD_ST_PROTOCOL - 1].index = f_prot;

    memset(l_rcver_coil, 0, sizeof(l_rcver_coil));
    sarInfo_t* coil_sar_info = f_pr_obj->GetSarInfo();
    memset(coil_sar_info->receiverCoil, 0, sizeof(coil_sar_info->receiverCoil));

    PqmSaturnRcvCoil_t* l_rcv_coil_tbl = f_pr_obj->GetCoilDBTable();

    for (int i = 0; i < l_rcv_coil_tbl->numCoil; i++) {
        l_rcver_coil[i]  = l_rcv_coil_tbl->coiltbl_p[i]->coil_id;
        (f_pr_obj->GetSarInfo())->receiverCoil[i] =
            (f_pr_obj->GetCoilDBTable())->coiltbl_p[i]->coil_id;
    }

    // Set Number of Section
    BITFLD_INIT(sub_mask, K_ALL_PR, 0);
    status_t        l_sts = E_NO_ERROR;

    if (i) {

        int n = 0;

        int s = i * sizeof(int32_t);
        VFF_SET_ARGS(l_vfReq[n], SVN_PLN_RECVR_COIL, SVT_PLN_RECVR_COIL,
                     s, l_rcver_coil, VFO_REPLACE);
        n++;

        // NEED_MODI Study file handle got NULL
        //l_sts = CPQMStudy::PutFields(m_vf_handle, l_pelm, SVD_ST_PROTOCOL, l_vfReq, n,  // SM4 Himanshu CFA Fix

        //+Patni-SS/2009Apr01/PSP1#VFLOCK
        CString l_log_msg = _T("");
        l_log_msg.Format(_T("SVN_PLN_RECVR_COIL=%d"), l_rcver_coil);
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_log_msg);
        //-Patni-SS/2009Apr01/PSP1#VFLOCK
        time_t          l_tm = {0};
        int l_num = 0;
        l_sts = f_vf_handle->PutFields(l_pelm, SVD_ST_PROTOCOL, l_vfReq, n,
                                       &l_num, &l_tm);

        if (l_sts != E_NO_ERROR || n != l_num) {    // Error Handling
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
            l_sts = E_ERROR ;
        }
    }

    return l_sts ;
}

//****************************Method Header************************************
//Method Name   :SpeederMAPExist()
//Author        :PATNI /
//Purpose       :
//*****************************************************************************
status_t CPQMStudy::SpeederMAPExist(const int prot)const
{

    VfPathElem_t        path[5] = {0};
    memset(path, 0, sizeof(VfPathElem_t) * 5);          //Patni-MJC/2009Aug17/Modified/cpp test corrections
    path[0].subtree_name = SVN_ST_PROTOCOL;
    path[0].index = prot;
    path[1].subtree_name = SVN_ST_PROCESSED;
    path[1].index = 0;

    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    VfFieldSpec_t       fieldSpec[2] = {0};
    memset(fieldSpec, 0, sizeof(VfFieldSpec_t) * 2);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008

    int n = 0;
    int32_t             map_flag = 0;
    VFF_SET_ARGS(fieldSpec[n], SVN_PRC_MAP_EXIST_FLAG, VFT_BOOL,
                 sizeof map_flag , &map_flag, 0L);
    n++;

    int numDone = 0;
    status_t status = m_vfdb_handle->GetFields(path, SVD_ST_PROCESSED, fieldSpec, n, (int*) & numDone);
    //DB_PRINTF("S",("MAP_EXIST_FLAG  %s:%d  sts=%d, n=%d, numDone=%d, map_flag=%d \n",
    //          __FILE__,__LINE__, status, n , numDone, map_flag));

    if (status == E_NO_ERROR && n == numDone && map_flag == TRUE) {
        status = E_NO_ERROR ;

    } else {
        status = E_ERROR;
    }

    return(status) ;
}

//****************************Method Header************************************
//Method Name   : PutdBdtMode()
//Author        : PATNI\MSN
//Purpose       :
//*****************************************************************************
int CPQMStudy::PutdBdtMode(CVarFieldHandle* const vf_handle , const int  port,
                           int32_t mode , flt32_t* value)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::PutdBdtMode");

    if (m_pqm_ptr == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return E_ERROR;
    }

    DB_FUNC_ENTER("[pqmPutdBdtMode]:");

    VfPathElem_t        path[SVD_ST_SUBPROTOCOL] = {0};//Patni-MJC/2009Aug17/Modified/cpp test corrections
    memset(path, 0, sizeof(VfPathElem_t) * SVD_ST_SUBPROTOCOL);         //Patni-MJC/2009Aug17/Modified/cpp test corrections
    path[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    path[SVD_ST_PROTOCOL - 1].index = port ;
    path[SVD_ST_SUBPROTOCOL - 1].subtree_name = SVN_ST_SUBPROTOCOL;
    path[SVD_ST_SUBPROTOCOL - 1].index = 0 ;

    VfFieldSpec_t       vfReq[3] = {0};
    memset(vfReq, 0, sizeof(VfFieldSpec_t) * 3);                        //Patni-MJC/2009Aug17/Modified/cpp test corrections
    int n = 0 ;
    VFF_SET_ARGS(vfReq[n], SVN_SEQ_DBDT_IEC_OPERATING_MODE,
                 SVT_SEQ_DBDT_IEC_OPERATING_MODE, sizeof(flt32_t),
                 &mode, VFO_REPLACE);
    n++ ;

    //+Patni-SS/2009Apr01/PSP1#VFLOCK
    CString l_log_msg = _T("");
    l_log_msg.Format(_T("SVN_SEQ_DBDT_IEC_OPERATING_MODE =%d "), mode);
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_log_msg);

    //-Patni-SS/2009Apr01/PSP1#VFLOCK
    int num = 0;
    time_t tm = 0;
    status_t status = vf_handle->PutFields(path, SVD_ST_PROTOCOL, vfReq, n, &num, &tm);

    if (status != E_NO_ERROR && num != n) {
        DB_PRINTF("", ("Put Data Error. (%s:%d) \n", __FILE__, __LINE__));
        DB_FUNC_EXIT();
        return E_ERROR;
    }

    n = 0 ;
    VFF_SET_ARGS(vfReq[n], SVN_SEQ_DBDT_VALUE,
                 SVT_SEQ_DBDT_VALUE, sizeof(flt32_t) * 2,
                 value, VFO_REPLACE);
    n++ ;

    //+Patni-SS/2009Apr01/PSP1#VFLOCK
    l_log_msg.Format(_T("SVN_SEQ_DBDT_VALUE =%d "), *value);

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_log_msg);

    status = vf_handle->PutFields(path, SVD_ST_SUBPROTOCOL, vfReq, n, &num, &tm);

    if (status != E_NO_ERROR && num != n) {
        DB_PRINTF("", ("Put Data Error. (%s:%d) \n", __FILE__, __LINE__));
        DB_FUNC_EXIT();
        return E_ERROR;
    }

    //+ Patni-MSN/2009Sept02/Added/MVC004505

    //- Patni-MSN/2009Sept02/Added/MVC004505

    DB_FUNC_EXIT();
    return E_NO_ERROR;
}

//****************************Method Header************************************
//Method Name   :CloseStudy()
//Author        :PATNI /SS
//Purpose       : Close Study file on PQM close
//*****************************************************************************
void CPQMStudy::CloseStudy()
{
    //+TMSC-Tanoue/2010Nov10/REDMINE-958
    CScopedLock guard(&m_cs);

    //+Patni-Hemant/2010Dec04/If study is not created then m_vfdb_handle is NULL
    //So if condition is added
    if (m_vfdb_handle) {
        m_vfdb_handle->Close();
    }

    //-Patni-Hemant/2010Dec04

    //-TMSC-Tanoue/2010Nov10/REDMINE-958

    //Patni-Ss/2009June07/JFT#166, #IDS 531
    m_prot_count = 0;
    m_study_id.run_num = -1;
    m_study_id.site_id = -1;
    SetStudyLoid(_T(""));
    m_weight = 0.0f;
    m_has_image = FALSE;
    m_total_time = 0L;
    m_first_scan = TRUE;
    m_wait_pos = 0;
    //Patni-Ss/2009June07/JFT#166, #IDS 531

    m_pqm_ptr->GetSARManager()->SendSessionEndMsgToSARMgr();

}

//+Patni-HEMANT/ADDED On 8/20/2009 10:06:52 PM/ MVC002838
//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPQMStudy::CreateOrUpdateVFStudyFile(
    CString f_patient_loid,
    CString f_study_loid,
    CString& f_vf_study_name
)
{

    CPdetApp l_pdet;
    TCHAR l_err_string[256] = {0};
    TCHAR* l_patient_loid = f_patient_loid.GetBuffer(f_patient_loid.GetLength());
    TCHAR* l_study_loid = f_study_loid.GetBuffer(f_study_loid.GetLength());
    TCHAR* l_vf_study_name = f_vf_study_name.GetBuffer(f_vf_study_name.GetLength());

    //Note : l_err_string will contain the error string, if error occured.
    //we still not have any error to be displayed according to current code..
    CString l_message = _T("");
    CString l_filename = l_pdet.WriteToVarfieldDb(l_patient_loid, l_study_loid, l_vf_study_name, l_err_string, &l_message);

    f_patient_loid.ReleaseBuffer();
    f_study_loid.ReleaseBuffer();
    f_vf_study_name.ReleaseBuffer();

    //+Patni-HEMANT/ADDED On 9/7/2009 3:02:10 PM/ MVC002838
    //as per PDET team discussion, they will send operation error
    //i.e. database reading writting problem
    //other warning message will come in l_message.
    //previously we had decided to have only l_message but as per latest discussion,
    //we have done like this.
    CString l_operation_error = l_err_string;

    if (l_operation_error.IsEmpty()) {

        if (!l_message.IsEmpty()) {
            CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(l_message,
                    _T("pdet"),
                    SCAN_WARN_WARNING,
                    SCAN_WARN_SYSMODAL,
                    SCAN_WARN_OKONLYBUTTON);

        } else {
            l_filename = l_filename.Mid(l_filename.Find(_T("Run")));
        }

    } else {

        CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(l_operation_error,
                _T("PQM"),
                SCAN_WARN_WARNING,
                SCAN_WARN_SYSMODAL,
                SCAN_WARN_OKONLYBUTTON);
    }

    //-Patni-HEMANT/ADDED On 9/7/2009 3:02:10 PM/ MVC002838

    f_vf_study_name = l_filename;

    return (f_vf_study_name.IsEmpty() == TRUE ? false : true);
}

//****************************Method Header************************************
//Method Name   :CreateStudyFile()
//Author        :PATNI/MSN
//Purpose       :Create Study File using Study Loid of MPlus Local DB
//*****************************************************************************
bool CPQMStudy::CreateStudyFile(
    const CString& f_patient_loid,
    const CString& f_study_loid
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::CreateStudyFile");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, f_study_loid);

    // TCHAR l_err_string[256] = {0};
    CString l_filename = _T("");
    // CString l_run_num = _T(""); //Patni-AMT/2010Mar10/Modified/Ph-3#IR-100
    // CString l_site_id = _T(""); //Patni-AMT/2010Mar10/Modified/Ph-3#IR-100

    m_study_loid = f_study_loid;

    if (!f_patient_loid.IsEmpty() && !f_study_loid.IsEmpty()) {


        //+Patni-HEMANT/ADDED On 8/20/2009 10:06:52 PM/ MVC002838

        if (!CreateOrUpdateVFStudyFile(f_patient_loid, f_study_loid, l_filename)) {
            return false;

        } else {
            //+Patni-Sribanta/2010July21/Added/TMSC-REDMINE-452
            OnCreateVFStudyFile(l_filename);
            //+Patni-Sribanta/2010July21/Added/TMSC-REDMINE-452
            //+ MEITEC watanabe ADDED 10/01/14 //.pdb_header file copy
            PdbFileCopy();
            //- MEITEC watanabe ADDED 10/01/14 //.pdb_header file copy
        }

        //-Patni-HEMANT/ADDED On 8/20/2009 10:06:52 PM/ MVC002838

    } else {
        return false; //Patni-AMT/2009May22/Added/JFT Code Review.
    }

    //CString l_temp_str = _T("Loaded file name - ");
    //l_temp_str += l_filename;
    //l_temp_str.Format(_T("%s %s"), l_err_string, l_filename);
    //l_err_string +=  l_filename;

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_filename);
    //+Patni-AMT/2009May22/Added/JFT Code Review
    //Patni-PJS/2009Sep16/Modified/TMSC Code review comment

    char*   l_site_path_env = getenv("SITE") ;
    CString l_site_path(l_site_path_env);

    if (l_site_path.IsEmpty()) {

        //+Patni-ARD/2009-Nov-25/Modified/DeFT# MVC003149 + MVC005179
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Error: Environment variable GP_SITES_ROOT is not set."));
        //-Patni-ARD/2009-Nov-25/Modified/DeFT# MVC003149 + MVC005179

        return false;
    }

    if (PathIsDirectory(l_site_path)) {

        CString l_newstudypath  = _T("");
        //+Patni-AMT/2010Mar10/Modified/Ph-3#IR-100
        /*
        CString l_keyname       = _T("Study Uid");
        CString l_sectionname   = _T("STUDY");
        */

        l_newstudypath.Format(_T("%s/%s/%s"), l_site_path, STUDY_FOLDER, l_filename);

        m_study_path = l_newstudypath;
        m_study_path.MakeLower();
        m_study_path.Replace(_T("run"), _T("Run"));

        // Set study id into PQM config file
        /*
        CString l_studyuid = CPQMConfig::GetInstance()->GetStudyUID();
        int l_searchpos = 0, l_lentodelete = 0, l_startpos = 0, l_ret = 0;

        l_searchpos = l_studyuid.Find(_T("\\"));
        l_lentodelete = l_studyuid.GetLength() - l_searchpos - 1;
        l_startpos = l_searchpos + 1;
        l_ret = l_studyuid.Delete(l_startpos, l_lentodelete);

        CString l_newstudyuid = l_studyuid + l_filename;
        CPQMConfig::GetInstance()->SetStudyUID(l_newstudyuid);

        if (CPQMConfig::GetInstance()->GetMachineType() == _T("NORMAL")) {
            BOOL l_result = CopyFile(l_newstudypath, l_newstudyuid, FALSE);

            if (l_result == 0) {
                //Means could not copy the study file to the Mapped drive
                //Write the error to the error log
                //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
                CPQMLogMgr::GetInstance()->DisplayScanError(IDS_ERR_MAP_DRIVE, _T("IDS_ERR_MAP_DRIVE"),
                SCAN_ERR_NOTIFY, L"PQMStudy.cpp");
                m_pqm_ptr->WriteToUI(PQM_MSG_MAP_DRIVE_NOT_FOUND);
                return false;
            }
        }

        l_newstudyuid.Replace(_T("\\"), _T("/"));

        if (CPQMConfig::GetInstance()->GetMachineType() == _T("NORMAL")) {
            m_pqm_ptr->SetStudyUid(l_newstudyuid);

        } else if (CPQMConfig::GetInstance()->GetMachineType() == _T("ONEPATH")) {
            m_pqm_ptr->SetStudyUid(l_newstudypath);
        }

        l_searchpos = l_newstudyuid.Find(_T("/"));
        l_searchpos = l_searchpos + 1;
        l_ret = l_newstudyuid.Delete(0, l_searchpos);
        */
        m_pqm_ptr->SetStudyUid(l_newstudypath);
        m_pqm_ptr->SetStudyUidPath(l_newstudypath);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_newstudypath);
    }

    //-Patni-AMT/2010Mar10/Modified/Ph-3#IR-100
    //-Patni-AMT/2009May22/Added/JFT Code Review

    return true;
}

//***************************Method Header*************************************
//Method Name    :IsHasImage()
//Author         :PATNI/HAR-SS
//Purpose        :
//*****************************************************************************
BOOL CPQMStudy::IsHasImage(
    CPqmProtocol* curr_protocol
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::IsHasImage");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of IsHasImage"));

    //+Patni-SS/MODIFIED/2009-Jun-04/CRASH TMSC reported
    if (curr_protocol == NULL)
        return false;

    BOOL result = FALSE;
    CPqmProtocol* result_acqSeries = NULL;
    //Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    protocolvec::const_iterator v1_Iter = NULL;

    //Patni-DKH/2011Mar04/Added/TMSC-REDMINE-1486
    CScopedLock l_scoped_lock(&m_protocol_vec_mutex);

    for (v1_Iter = m_protocol_vec.begin(); v1_Iter != m_protocol_vec.end(); v1_Iter++) {
        result_acqSeries = *v1_Iter;

        //+Patni-SS/MODIFIED/2009-Jun-04/CRASH TMSC reported
        if ((result_acqSeries  != NULL) && result_acqSeries->GetProtocol() == curr_protocol->GetProtocol()) {
            break;
        }

        //+Patni-SS/MODIFIED/2009-Jun-04/CRASH TMSC reported
        if (result_acqSeries != NULL && result_acqSeries->GetAcqStatus() == C_PR_STATUS_DONE) {
            result = TRUE;
            break;

        } else {
            continue;
        }
    }

    return result;
}

//***************************Method Header*************************************
//Method Name    :PutInjectTimeInDB()
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
void CPQMStudy::PutInjectTimeInDB(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::PutInjectTimeInDB");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of PutInjectTimeInDB"));

    //+Patni-MJC/2009Aug27/Modified/cpp test corrections
    if (m_pqm_ptr == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug27/Modified/cpp test corrections
    //+Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    CScanProcedure* l_scan_procedure = m_pqm_ptr->GetAcqManager()->GetScanProcedure();

    if (l_scan_procedure == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("l_scan_procedure pointer is NULL"));
        return;
    }

    CPqmProtocol* l_current_protocol = l_scan_procedure->GetScanProcProtocol();

    if (l_current_protocol == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("l_current_protocol pointer is NULL"));
        return;
    }

    //-Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    if (l_current_protocol != NULL) {
        time_t l_now = time(0);

        if (l_now == -1) {
            return;
        }

        VfPathElem_t l_path[] = {
            SVN_ST_PROTOCOL,
            0,
            SVN_ST_SUBPROTOCOL,
            0,
            SVN_ST_ECHO,
            0,
            SVN_ST_POSITION,
            0
        };

        int l_n = 0;
        VfFieldSpec_t l_vf_req[NUM_7] = {0};

        VFF_SET_ARGS(l_vf_req[l_n],
                     SVN_EXE_INJECT_TIME,
                     VFT_TIME,
                     sizeof(time_t),
                     &l_now,
                     VFO_REPLACE);

        l_n++;
        //+Patni-SS/2009Apr01/PSP1#VFLOCK
        CString l_log_msg = _T("");
        l_log_msg.Format(_T(" SVN_EXE_INJECT_TIME =%d"), l_now);
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_log_msg);

        //-Patni-SS/2009Apr01/PSP1#VFLOCK
        int l_num = 0;
        time_t l_tm = 0;

        //Note: To put current time in root level
        status_t l_status = E_NO_ERROR;

        if (m_pqm_ptr->GetMulitScanDynamic_License()) {									//REDMINE-2187-PRUTHVIRAJ
            l_status = m_vfdb_handle->PutFields(NULL, SVD_ST_STUDY, l_vf_req,
                                                l_n, &l_num, &l_tm);

            if ((l_status != E_NO_ERROR) || (l_num != l_n)) {

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed. at root level"));
                return ;
            }

            m_pqm_ptr->GetAcqManager()->SetInjectTimePresent(true);
        }

        l_path[SVD_ST_PROTOCOL - 1].index = l_current_protocol->GetProtocol();

        l_num = 0;
        l_tm = 0;
        l_status = m_vfdb_handle->PutFields(l_path, SVD_ST_PROTOCOL, l_vf_req,
                                            l_n, &l_num, &l_tm);

        if ((l_status != E_NO_ERROR) || (l_num != l_n)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed. at protocol level"));

            //vfGetFuncError("PqmUiInjectTimeCB:", status);
            return ;
        }

    }
}

//***************************Method Header*************************************
//Method Name    :DuplicateProtocol()
//Author         :PATNI/SG
//Purpose        :
//*****************************************************************************
int CPQMStudy::DuplicateProtocol(
    int f_from_pos,
    int f_to_pos,
    const int f_group_id /*= 0*/
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::DuplicateProtocol");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of DuplicateProtocol"));

    if (!m_vfdb_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_vfdb_handle is NULL"));
        return -1;
    }

    int f_from_pos_1based = f_from_pos + 1;

    CPqmProtocol*    l_from_obj = GetItemFromPosition(f_from_pos_1based);
    ASSERT(l_from_obj != NULL);

    if (l_from_obj->GetAMBModeFlag() == VFC_AMB_MODE_SLAVE) {
        return -1;

    }

    long l_maxprotocol_limit = CPQMConfig::GetInstance()->ReadMaxProtocolLimitfromFile();

    CString lPosition(_T(""));
    lPosition.Format(_T("From Pos = %d : To Pos = %d"), f_from_pos, f_to_pos);
    VuiAcqOrder_t l_acq_order;
    memset(&l_acq_order, NULL, sizeof(VuiAcqOrder_t));
    //Patni-RAJ/ADDED On 10/09/2009
    CString l_str = _T("");
    //-20091006 Tanoue-san
    l_str.Format(_T("Handle to close - 0x%x"), m_vfdb_handle);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,  l_str);
    ClibVfToUiAdapter vuiAdapter(m_vfdb_handle);

    if (E_NO_ERROR != vuiAdapter.VuiGetVfAcqOrder(&l_acq_order)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("VuiGetVfAcqOrder() Failed."));
        return -1;
    }

    CString l_branches_str(_T(""));
    l_branches_str.Format(_T("Branches = %d"), l_acq_order.SzAcqOrder);

    CString l_series_count(_T(""));
    l_series_count.Format(_T("Branches = %d"), l_acq_order.SzAcqOrder);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_series_count);

    const int l_difference = l_maxprotocol_limit - l_acq_order.SzAcqOrder;

    if (l_difference == 0) {


        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
            CPqmUtilities::GetMultiLingualString(_T("IDS_NO_SEQ_ACCEPATANCE")),
            MPlus::EH::Information, _T("PQMStudy.cpp"), _T("PQM"));

        return -1;

    }


    if (l_maxprotocol_limit < GetAllItems() + 1) {
        return -1 ;
    }

    //Added by Meghana On 8/6/2008 2:22:50 PM

    if (l_from_obj->GetAcqStatus() == C_PR_STATUS_CURRENT &&
        !m_pqm_ptr->GetAcqManager()->GetScanProcedure()->GetIsWaitingFlagForSARCtrl()) {

        //ListCopyCurrent(m_vf_handle,0,&f_to_pos);
        // + SM4 DEF#814, 60 Himanshu 20090120
        m_vfdb_handle->ReOpen();
        if_error(ListCopyCurrent(m_vfdb_handle, f_to_pos, &f_to_pos)) {
            m_vfdb_handle->ReOpen();
            return -1;
        }
        m_vfdb_handle->ReOpen();
        // - SM4 DEF#814, 60 Himanshu 20090120

    } else if_error(ListCopyItem(/*m_vf_handle,*/f_from_pos, f_to_pos)) {

        //      PqmPqResetPosition(from_pos);
        return -1 ;
    }

    //Added by Meghana On 8/6/2008 2:41:08 PM
    CPqmProtocol*    l_obj = NULL;

    if (f_to_pos) {
        l_obj = GetItemFromPosition(f_to_pos + 1);

    } else {
        l_obj = GetItemFromPosition(m_prot_count);
    }

    ASSERT(l_obj != NULL);

    if (f_group_id) {
        l_obj->SetMasterProtocol(m_pqm_ptr->GetMasterSlavePtr()->GetMasterProtocolForGroupId(f_group_id));
        SetGroupIDToVF(l_obj, f_group_id);
    }

    //ListAddItemUnselected(l_obj->prxms,f_to_pos);
    f_from_pos = l_obj->GetProtocol();
    CString l_string_acq_order(_T(""));
    l_string_acq_order.Format(_T("*** DuplicateProtocol = %d"), m_prot_count);
    //+ Patni-PJS/2009Jun05/Added/JFT# 251,251,251,IR(5)

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_string_acq_order);
    return f_from_pos;
}

//***************************Method Header*************************************
//Method Name    :DuplicateSequence()
//Author         :PATNI/SG
//Purpose        :
//*****************************************************************************
void CPQMStudy::DuplicateSequence(
    int* f_pos_list
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::DuplicateSequence");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of DuplicateSequence"));

    if (f_pos_list == NULL) {
        return;
    }

    CPqm* l_pqm = GetPqm();
    long l_maxprotocol_limit = CPQMConfig::GetInstance()->ReadMaxProtocolLimitfromFile();

    int l_count = 0, l_to_pos = 0;
    int l_all_items = GetAllItems() + l_count;

    if (l_maxprotocol_limit < l_all_items) {
    } else {

        //      for(i = l_count - 1;i >= 0;i--)
        //      {
        CPqmProtocol* l_obj = GetItemFromPosition(f_pos_list[0]);
        ASSERT(l_obj != NULL);

        //+Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
        if (l_obj == NULL) {
            return;
        }

        //-Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
        if (l_obj->GetAcqStatus() == C_PR_STATUS_CURRENT) {
            l_to_pos = 0;

        } else {
            if (l_obj->GetAcqStatus() == C_PR_STATUS_WAIT) {
                l_to_pos = f_pos_list[0] + 1;

            } else {
                l_to_pos = m_wait_pos;
                CPqmProtocol* l_to_obj = GetItemFromPosition(l_to_pos);

                //+Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
                if (l_to_obj == NULL) {
                    return;
                }

                //-Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
                if ((l_pqm->GetAcqManager()->GetProtocol() != NULL) &&
                    (l_to_obj->GetProtocol() == l_pqm->GetAcqManager()->GetProtocol()->GetProtocol()))
                    l_to_pos++;
            }
        }

        f_pos_list[0] = DuplicateProtocol(f_pos_list[0] , l_to_pos);
        //}
        //      for(i = 0;i < l_count;i++)
        //      {
        l_obj = GetProtocol(f_pos_list[0]);

        if (l_obj) {
            f_pos_list[0] = l_obj->GetPosition();
        }
    }

    SetScanModeOfHead();
}

//****************************Method Header************************************
//Method Name   :DeleteProt()
//Author        :PATNI/HAR-JS
//Purpose       :
//*****************************************************************************
void CPQMStudy::DeleteProt(
    const int f_index
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::DeleteProt");
    // Time take by Varfield to delete a protocol %d, f_index
    //+ Patni-RJ/2009Apr02/Added/PSP- Performance code
#ifdef __SM_PERFORMANCE_CHECK__
    __int64 l_freq = 0, l_stop = 0, l_start = 0;    //Patni-MJC/2009Aug17/Modified/cpp test corrections

    QueryPerformanceCounter((LARGE_INTEGER*)&l_start);
#endif
    //- Patni-RJ/2009Apr02/Added/PSP- Performance code

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of DeleteProt"));

    //Patni-DKH/2010Nov19/Added/TMSC-REDMINE-1486
    try {

        VuiAcqOrder_t l_acq_order;
        //Patni-RAJ/ADDED On 10/09/2009
        CString l_str = _T("");
        //-20091006 Tanoue-san
        l_str.Format(_T("Handle to close - 0x%x"), m_vfdb_handle);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,  l_str);
        ClibVfToUiAdapter vuiAdapter(m_vfdb_handle);

        if (E_NO_ERROR != vuiAdapter.VuiGetVfAcqOrder(&l_acq_order)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("VuiGetVfAcqOrder() failed."));
            return ;
        }

        l_str.Format(_T("Branches = %d"), l_acq_order.SzAcqOrder);

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Before Delete: ") + l_str);

        l_str.Format(_T("Index = %d"), f_index);

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);

        if (E_NO_ERROR != vuiAdapter.VuiDeleteProtocol(f_index)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("VuiDeleteProtocol Error"));
            return;
        }

        //Patni-DKH/2010Nov19/Added/TMSC-REDMINE-1486
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Protocol deleted from Varfield"));



        //+ Patni-RJ/2009Apr02/Added/PSP- Performance code
#ifdef __SM_PERFORMANCE_CHECK__
        QueryPerformanceFrequency((LARGE_INTEGER*)&l_freq);
        QueryPerformanceCounter((LARGE_INTEGER*)&l_stop);
        CString l_temp(_T(""));
        l_temp.Format(_T("{CPQMStudy::DeleteProt} Time take by Varfield to delete a protocol %d."), f_index);
        CPQMLogMgr::GetInstance()->TracePerformance(l_start, l_stop, l_freq, l_temp);
#endif

#ifdef __SM_PERFORMANCE_CHECK__
        l_freq = 0;
        l_stop = 0;
        l_start = 0;

        QueryPerformanceCounter((LARGE_INTEGER*)&l_start);
#endif
        //- Patni-RJ/2009Apr02/Added/PSP- Performance code

        //Patni-DKH/2011Mar04/Added/TMSC-REDMINE-1486
        CScopedLock l_scoped_lock(&m_protocol_vec_mutex);

        protocolvec::iterator v1_Iter = NULL;   //Patni-MJC/2009Aug17/Modified/cpp test corrections

        CPqmProtocol* l_protocol = NULL;

        for (v1_Iter = m_protocol_vec.begin();
             v1_Iter != m_protocol_vec.end();
             v1_Iter++) {
            l_protocol = *v1_Iter;

            //+Patni-SS/MODIFIED/2009-Jun-04/CRASH TMSC reported
            if (l_protocol != NULL && l_protocol->GetProtocol() == f_index) {
                //Patni-HAR/2009Aug11/Added/Memory Leaks
                DEL_PTR(l_protocol);
                m_protocol_vec.erase(v1_Iter);
                //+Patni-SS/COMMENTED/2009-May-20/AcqOrder Defect
                m_prot_count--;
                break;

            } else {
                continue;
            }
        }

        //Patni-DKH/2010Nov19/Added/TMSC-REDMINE-1486
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Deleted protocol from protocol vector"));


        RenumberPositionsInVector();

        //Patni-DKH/2010Nov19/Added/TMSC-REDMINE-1486
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Renumbered protocols in protocol vector"));

        //Patni-RAJ/ADDED On 10/09/2009
        //-20091006 Tanoue-san
        l_str.Format(_T("Handle to close - 0x%x"), m_vfdb_handle);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,  l_str);

        if (E_NO_ERROR != vuiAdapter.VuiGetVfAcqOrder(&l_acq_order)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("VuiGetVfAcqOrder() failed."));
            return;
        }

        l_str.Format(_T("Branches = %d"), l_acq_order.SzAcqOrder);

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("After Delete: ") + l_str);

        //+ Patni-RSG/2009Oct14/Modified/MVC005134, MVC004938
        CPqmProtocol* wait_protocol = NULL;
        SetTopWaitPos(GetTopWaitPos(&wait_protocol));
        //+ Patni-RSG/2009Oct14/Modified/MVC005134, MVC004938

        SearchScanMode(m_vfdb_handle, K_MODE_SET_IT);

        //+ Patni-PJS/2009Jun05/Added/JFT# 251,251,251,IR(5)
        //+Patni-MJC/2009Aug27/Modified/cpp test corrections
        if (m_pqm_ptr == NULL) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
            return;
        }

        //-Patni-MJC/2009Aug27/Modified/cpp test corrections

        //Patni-DKH/2010Nov19/Added/TMSC-REDMINE-1486
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                  _T("Protocol Deletion Successfull from Varfield"));

        //+ Patni-RJ/2009Apr02/Added/PSP- Performance code
#ifdef __SM_PERFORMANCE_CHECK__
        QueryPerformanceFrequency((LARGE_INTEGER*)&l_freq);
        QueryPerformanceCounter((LARGE_INTEGER*)&l_stop);

        CPQMLogMgr::GetInstance()->TracePerformance(l_start, l_stop, l_freq, _T("{CPQMStudy::DeleteProt} Time take by PQM in reordering protocols"));
#endif
        //- Patni-RJ/2009Apr02/Added/PSP- Performance code

        //+Patni-DKH/2010Nov19/Added/TMSC-REDMINE-1486

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Unhandled Exception Occurred in DeleteProt"));
        throw;
    }

    //-Patni-DKH/2010Nov19/Added/TMSC-REDMINE-1486
}

//+Patni-HAR/2010Mar26/Added/MVC007422
//****************************Method Header************************************
//Method Name   : ReaddBdtOpMode()
//Author        : PATNI \ HAR
//Purpose       : Reads dB/dt operating from varfield study file at subprotocol level
//*****************************************************************************
//Patni-PJS/2010June1/Modified/MVC007434 CR
int CPQMStudy::ReaddBdtOpMode(
    CPqmProtocol* f_protocol,
    int& f_dbdt_op_mode,
    flt32_t& f_curr_dbdt_value
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::ReaddBdtOpMode");

    if (!f_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("[Protocol pointer is NULL]"));
        return E_ERROR;
    }

    int l_acq_status = f_protocol->GetAcqStatus();
    CString l_log_str(_T(""));
    l_log_str.Format(_T("[Protocol No : %d][Protocol Status : %d]"),
                     f_protocol->GetProtocol(), l_acq_status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);

    int l_num_done = -1;
    //+Patni-PJS/2010June1/Modified + Added/MVC007434 CR
    flt32_t l_dbdt_value[2] = {0.0};
    f_curr_dbdt_value = 0.0f;
    VfPathElem_t l_search_node_path[] = {
        { SVN_ST_PROTOCOL,		0 },
        { SVN_ST_SUBPROTOCOL,	0 },
        { SVN_ST_SLICEGROUP,	0 },
        { SVN_ST_SLICE,		0 },
        { SVN_ST_IMAGE,		0 },
    };

    int active = 0;
    int l_status    = E_NO_ERROR;
    VfFieldSpec_t l_vf_req[2] = {0};
    memset(&l_vf_req, 0, sizeof(VfFieldSpec_t) * 2);
    //-Patni-PJS/2010June1/Modified + Added/MVC007434 CR

    VfPathElem_t    vf_dst_path[MAX_DEPTH] = {0};
    memset(vf_dst_path , 0 , sizeof(VfPathElem_t) * MAX_DEPTH);

    BITFLD_DECL(mask,   MAX_BITFLD);

    vf_dst_path[0].subtree_name = SVN_ST_PROTOCOL;
    vf_dst_path[0].index		= f_protocol->GetProtocol();

    int total = MAX_BITFLD;
    BITFLD_INIT(mask, MAX_BITFLD, 0);

    if (m_vfdb_handle) {
        l_status = m_vfdb_handle->ScanSubtree(vf_dst_path,
                                              SVD_ST_PROTOCOL, SVN_ST_SUBPROTOCOL, mask, &total, &active);

        f_dbdt_op_mode = 0;

        for (int i = 0; i < total; i++) {

            if (BITFLD_ON(mask, i)) {

                if (l_status != E_NO_ERROR) {

                    l_log_str.Format(_T("[Error Status : %d]"), l_status);
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);
                    return E_ERROR;
                }

                int l_cur_dbdt_op_mode = 0;
                int n = 0;
                //+Patni-PJS/2010June1/Modified+Added/MVC007434 CR
                VFF_SET_ARGS(l_vf_req[n], SVN_SEQ_DBDT_IEC_OPERATING_MODE,
                             SVT_SEQ_DBDT_IEC_OPERATING_MODE, sizeof(int),
                             &l_cur_dbdt_op_mode, 0);
                n++;

                //+Patni-PJS/2010Jun09/Added/MVC007434
                if ((C_PR_STATUS_CURRENT == l_acq_status) || (C_PR_STATUS_FAIL == l_acq_status)) {
                    VFF_SET_ARGS(l_vf_req[n], SVN_SEQ_DBDT_VALUE,
                                 SVT_SEQ_DBDT_VALUE, sizeof(flt32_t), l_dbdt_value,
                                 VFO_REPLACE);

                } else {
                    VFF_SET_ARGS(l_vf_req[n], SVN_SEQ_DBDT_VALUE,
                                 SVT_SEQ_DBDT_VALUE, sizeof(flt32_t) * 2, l_dbdt_value,
                                 VFO_REPLACE);
                }

                //-Patni-PJS/2010Jun09/Added/MVC007434
                n++ ;
                //-Patni-PJS/2010June1/Modified+Added/MVC007434 CR
                l_search_node_path[SVD_ST_PROTOCOL - 1].subtree_name    = SVN_ST_PROTOCOL;
                l_search_node_path[SVD_ST_PROTOCOL - 1].index           = f_protocol->GetProtocol();
                l_search_node_path[SVD_ST_SUBPROTOCOL - 1].subtree_name = SVN_ST_SUBPROTOCOL;
                l_search_node_path[SVD_ST_SUBPROTOCOL - 1].index        = i;

                if ((C_PR_STATUS_WAIT == l_acq_status) //||
                    /*(C_PR_STATUS_CURRENT == l_acq_status)*/) {
                    //Patni-PJS/2010June1/Modified/MVC007434 CR
                    l_status = m_vfdb_handle->GetFields(l_search_node_path,
                                                        SVD_ST_PROTOCOL, l_vf_req, n, &l_num_done);

                } else {
                    //Patni-PJS/2010June1/Added/MVC007434 CR
                    l_status = m_vfdb_handle->GetFields(l_search_node_path,
                                                        SVD_ST_SUBPROTOCOL, l_vf_req, n, &l_num_done);
                }

                if (l_status != E_NO_ERROR) {

                    l_log_str.Format(_T("[Error Status : %d]"), l_status);
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);
                    return E_ERROR;
                }

                //+Patni-PJS/2010June1/Modified + Added/MVC007434 CR
                float l_dbdt_val = (l_acq_status == C_PR_STATUS_DONE)
                                   ? l_dbdt_value[1] : l_dbdt_value[0];

                if (l_cur_dbdt_op_mode > f_dbdt_op_mode) {
                    f_dbdt_op_mode = l_cur_dbdt_op_mode;
                    f_curr_dbdt_value = l_dbdt_val;

                } else if (l_cur_dbdt_op_mode == f_dbdt_op_mode) {
                    if (l_dbdt_val > f_curr_dbdt_value) {
                        f_curr_dbdt_value = l_dbdt_val;
                    }
                }

                //-Patni-PJS/2010June1/Modified + Added/MVC007434 CR
            }
        }

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_vfdb_handle pointer is NULL"));
    }

    return l_status;
}
//-Patni-HAR/2010Mar26/Added/MVC007422

//+Patni-ARD/2009Sep7/Added/Prototype 2
//************************************Method Header**********************************
// Method Name  : CheckAllForSequenseParam
// Author       : PATNI/ ARD
// Purpose      : check sequence params for all sequences present on GUI and IPC side
//***********************************************************************************
bool CPQMStudy::CheckAllForSequenseParam(VARIANT* f_pData, const int f_count_gui_list)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::CheckAllForSequenseParam");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T(""));

    //Patni-DKH/2011Mar04/Added/TMSC-REDMINE-1486
    CScopedLock l_scoped_lock(&m_protocol_vec_mutex);

    bool l_is_study_same = false;
    bool l_is_count_mismatch = true;

    if (f_pData == NULL) return l_is_study_same;

    if (f_count_gui_list == (int)m_protocol_vec.size()) {
        l_is_count_mismatch = false;
        l_is_study_same = true;
    }

    protocol_Info_Vec* l_list_protocol_info_vec;
    l_list_protocol_info_vec = (protocol_Info_Vec*) f_pData->byref;

    protocolvec::iterator v1_Iter = NULL;
    protocol_Info_Vec::iterator v_list_protcol_info_Iter = NULL;

    v1_Iter = m_protocol_vec.begin();
    v_list_protcol_info_Iter = l_list_protocol_info_vec->begin();

    if (l_is_study_same) {
        for (; v1_Iter != m_protocol_vec.end(),
             v_list_protcol_info_Iter != l_list_protocol_info_vec->end(); v1_Iter++, v_list_protcol_info_Iter++) {

            //Patni-MSN/2009Nov04/Modified/Code Review - Method name change
            if ((l_is_study_same = m_pqm_ptr->IsMatchSequenceParam(
                                       (CPqmProtocol*) * v1_Iter, (ProtocoInfo*) * v_list_protcol_info_Iter)) == false) break;

        }
    }

    if (!l_is_study_same) {

        v_list_protcol_info_Iter = l_list_protocol_info_vec->begin();
        v1_Iter = m_protocol_vec.begin();

        CString l_strErrMsg(_T("*************************Protocol Mismatch**************************"));

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  l_strErrMsg);

        if (l_is_count_mismatch) {
            l_strErrMsg.Format(_T("Protocol count mismatch GUI count: %d IPC count: %d"),
                               f_count_gui_list, (int)m_protocol_vec.size());

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_strErrMsg);
        }

        for (; v1_Iter != m_protocol_vec.end(),
             v_list_protcol_info_Iter != l_list_protocol_info_vec->end(); v1_Iter++, v_list_protcol_info_Iter++) {

            m_pqm_ptr->LogProtocolInfo((CPqmProtocol*)*v1_Iter, (ProtocoInfo*)*v_list_protcol_info_Iter);
        }


        //+TMSC-Tanoue/2010Mar05/MSA0251-00247
        /*
        MRERROR_LOG_DATA l_error_log_data;
        l_error_log_data.error_message = l_strErrMsg;
        l_error_log_data.error_source = _T("PQM");

        CPQMLogMgr::GetInstance()->DisplayMessageinErrorDialog(l_error_log_data);
        */
        CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(
            CPqmUtilities::GetMultiLingualString(_T("IDS_PQM_LIST_STUDY_PROTOCOL_MISMATCH")), _T("PQM"),
            SCAN_WARN_INFORMATION, SCAN_WARN_SYSMODAL, SCAN_WARN_OKBUTTON);
    }

    return l_is_study_same;
}
//-Patni-ARD/2009Sep7/Added/Prototype 2

//***************************Method Header*************************************
//Method Name    :WriteToShimData()
//Author         :PATNI/
//Purpose        :
//*****************************************************************************
BOOL CPQMStudy::WriteToShimData(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::WriteToShimData");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of WriteToShimData"));

    //Patni-SS/20091013/Modified/MSA0248-00056
    char* l_env = getenv("GP_SHIMDT_DIR");

    if (l_env == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Failed to get value of GP_SHIMDT_DIR environment variable"));
        return FALSE;
    }

    int             l_scannumber = 0;
    float           l_couchpos = 0.0f;
    float           l_homepos = 0.0f;
    float           l_txtunepos = 0.0f;
    float           l_rflpos = 0.0f;
    int             l_shimstate = 0;
    int             l_shimfsyflag = 0;
    CString         l_strdata;
    //Patni-DKH/2010Feb18/Added/Phase3#MCM0229-00187
    float           l_cfapos = 0.0f ;

    CPqmShimSharedData::GetInstance()->GetShimFsyFlag(&l_shimfsyflag);
    CPqmShimSharedData::GetInstance()->GetShimState(&l_shimstate);
    CPqmShimSharedData::GetInstance()->CopyTuneInfo(&l_txtunepos, &l_rflpos , &l_cfapos);//Patni-DKH/2010Feb18/Added/Phase3#MCM0229-00187
    CPqmShimSharedData::GetInstance()->CopyCouchInfo(&l_scannumber, &l_couchpos, &l_homepos);

    l_strdata.Format(_T("Scan Number=%d \nCouch Position=%f \nHome Position=%f \nTxTune Position=%f \nRflPosition=%f \nShim State=%d \nShim Flag=%d"), l_scannumber, l_couchpos, l_homepos, l_txtunepos, l_rflpos, l_shimstate, l_shimfsyflag);

    char            l_shimlogpath [256] = "";
    strcpy(l_shimlogpath, l_env);

    strcat(l_shimlogpath, "/ShimData.txt");

    FILE* l_fp = fopen(l_shimlogpath, "w+");

    if (l_fp) {
        wchar_t* l_strbuff = l_strdata.GetBuffer(l_strdata.GetLength());
        fwprintf(l_fp, l_strbuff);
        fclose(l_fp);

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ShimData file pointer is NULL"));
        return FALSE;
    }

    return TRUE;
}

//+Patni-Hemant/08Sep2010/Added/IR-152 Reopen
bool CPQMStudy::CreateScratchPadForSFTTool(
    const int f_acq_order,
    CString* f_tmp_file,
    const SCRATCHPAD_CREATION_MODE f_sp_creation_mode
)
{

    char*	path = NULL;
    char l_tmp_file[256] = {0};
    memset(l_tmp_file, 0 , sizeof(char) * 256);

    if ((path = getenv("GP_TMP_DIR")) != NULL) {
        sprintf(l_tmp_file, "%s%s", path, "/PQM.sftTool");

    } else {
        sprintf(l_tmp_file, "%s", "/gp/tmp/PQM.sftTool");
    }

    /** make empty file **/

    FILE* fp = fopen(l_tmp_file, "a");

    if (fp) {
        fclose(fp);
    }

    if (CreateScratchPad(f_acq_order, l_tmp_file, f_sp_creation_mode) == E_NO_ERROR) {

        MaintainShimming(l_tmp_file);


        if (f_tmp_file) {
            (*f_tmp_file) = l_tmp_file;
        }

        return true ;
    }

    return false;
}
//-Patni-Hemant/08Sep2010/Added/IR-152 Reopen

//***************************Method Header*************************************
//Method Name    :MaintainShimming()
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
int CPQMStudy::MaintainShimming(
    const CString& f_file
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::MaintainShimming");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of MaintainShimming"));

    PqmShim_t l_shim;
    int l_shim_state = 0;

    GetShimmingOffset(&l_shim);
    CPqmShimSharedData::GetInstance()->GetShimState(&l_shim_state);

    CVarFieldHandle l_maintain_vf_handle(f_file);

    if (l_maintain_vf_handle.VuiOpen() != E_NO_ERROR) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, f_file);
        return E_ERROR;
    }

    PutShimmingOffset(&l_maintain_vf_handle, NULL, SVD_ST_STUDY, &l_shim);

    VfFieldSpec_t l_vf_req[NUM_4] = {0};
    VFF_SET_ARGS(l_vf_req[0],
                 SVN_SCH_WEIGHT,
                 SVT_SCH_WEIGHT,
                 sizeof(float),
                 &m_weight,
                 VFO_REPLACE);

    VFF_SET_ARGS(l_vf_req[1],
                 SVN_SCH_GENDER,
                 SVT_SCH_GENDER,
                 sizeof(int),
                 &m_gender,
                 VFO_REPLACE);

    VFF_SET_ARGS(l_vf_req[2],
                 SVN_SCH_HEIGHT,
                 SVT_SCH_HEIGHT,
                 sizeof(float),
                 &m_height,
                 VFO_REPLACE);
    //+Patni-SS/2009Apr01/PSP1#VFLOCK
    CString l_log_msg = _T("");
    l_log_msg.Format(_T(" Patient WEIGHT=%d,GENDER = %d,HEIGHT = %d"), m_weight, m_gender, m_height);

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_log_msg);

    //-Patni-SS/2009Apr01/PSP1#VFLOCK
    int l_num = 0;
    time_t  l_tm = 0;
    int l_vf_stat = l_maintain_vf_handle.PutFields(NULL, SVD_ST_STUDY,
                    l_vf_req, NUM_3, &l_num, &l_tm);

    if (l_vf_stat != E_NO_ERROR) {
        //DB_PRINTF("M",("vfStat = %d\n",vfStat));
    }

    int l_hold = 0;
    VFF_SET_ARGS(l_vf_req[0],
                 SVN_PLN_HOLD,
                 SVT_PLN_HOLD,
                 sizeof(int),
                 &l_hold, VFO_REPLACE);

    VFF_SET_ARGS(l_vf_req[1],
                 SVN_PLN_AUTO_HOLD,
                 SVT_PLN_AUTO_HOLD,
                 sizeof(int),
                 &l_hold, VFO_REPLACE);

    VfPathElem_t    l_path[SVD_ST_PROTOCOL] = {0};
    l_path[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_path[SVD_ST_PROTOCOL - 1].index = 0;
    //+Patni-SS/2009Apr01/PSP1#VFLOCK
    l_log_msg.Format(_T(" SVN_PLN_HOLD =%d,SVN_PLN_AUTO_HOLD = %d"), l_hold, l_hold);

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_log_msg);
    //-Patni-SS/2009Apr01/PSP1#VFLOCK
    l_vf_stat = l_maintain_vf_handle.PutFields(l_path, SVD_ST_PROTOCOL, l_vf_req,
                2, &l_num, &l_tm);

    if (l_vf_stat  != E_NO_ERROR) {
        //DB_PRINTF("M",("vfStat = %d\n",vfStat));
    }

    if (l_shim_state == SHIM_STATE_ACTIVE) {
        // + SM4 Himanshu CFA Fix
        l_log_msg.Format(_T("VF Handle = %x"), &l_maintain_vf_handle);

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_msg);
        // + SM4 Himanshu CFA Fix
        SetupShimming(&l_maintain_vf_handle);
    }

    return E_NO_ERROR;
}

//****************************Method Header************************************
//Method Name   :PqmPmSaveProtocol
//Author        :PATNI/
//Purpose       :
//*****************************************************************************
status_t CPQMStudy::PqmPmSaveProtocol(
    CPqmProtocol* f_protocol,
    char* f_tmp_file
)
{
    //Patni-AMT/2010Mar22/Added/Ph-3# IR-86-TC34
    LPCTSTR FUNC_NAME = _T("CPQMStudy::PqmPmSaveProtocol");
    PQM_SCOPED_TRACE();

    CScopedLock l_scoped_lock(&m_scan_mutex);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_scan_mutex acquired"));

    if (f_protocol == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_protocol pointer is NULL"));
        return E_ERROR;
    }

    //+Patni-PJS/2010Mar24/Added/IR-140
    if (E_ERROR == PopulateGUIControls(f_protocol->GetProtocol())) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("PopulateGUIControls failed to populate updateCtrls structure"));
    }

    //-Patni-PJS/2010Mar24/Added/IR-140
    //+Patni-MJC/2009Aug27/Modified/cpp test corrections
    if (m_pqm_ptr == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return E_ERROR;
    }

    CString l_str(_T(""));
    l_str.Format(_T("Acq Order = %d"), f_protocol->GetProtocol());
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);

    int* Imge = f_protocol->GetImageGr();

    //+TMSC-REDMINE-1596

    //+Patni-SS/2009Apr01/PSP1#VFLOCK
    l_str.Format(_T(" :VuiCopyProtocolFromScratchPad :"));
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_str);
    //-Patni-SS/2009Apr01/PSP1#VFLOCK

    //Patni-AMT/2010Mar22/Added/Ph-3# IR-86-TC36
    CPQMDebugInfo::GetInstance(m_pqm_ptr)->PqmDebugProMonitor(
        _T("VuiCopyProtocolFromScratchPad %s(%d) -> %d[%d]\n"),
        CString(f_tmp_file), 0,
        f_protocol->GetProtocol(), Imge[1]);

    ClibVfToUiAdapter vuiAdapter(m_vfdb_handle);
    status_t l_status = vuiAdapter.VuiCopyProtocolFromScratchPad(f_protocol->GetProtocol(), f_tmp_file, 0,
                        Imge[1], NULL);

    if_error(l_status) {

        //ProMsgSend(pmObj->core.wrFd,PROMSG_PQM_QUEUE_DONE,0,NULL);
        //DBPTR(("Send to proCon: PROMSG_PQM_QUEUE_DONE\n"));
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("VuiCopyProtocolFromScratchPad() failed."));
        return E_ERROR;
    }
    //-TMSC-REDMINE-1596
    CPqmProcon* l_pqm_procon = m_pqm_ptr->GetPqmProCon();

    if (NULL == l_pqm_procon) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqm_procon is NULL"));
        return E_ERROR;
    }

    bool l_is_plan_dynamic_executed = l_pqm_procon->IsPlanDynamicExecutedForCurrent();

    //+Patni-PJS/2010Mar24/Added/IR-140
    if (E_ERROR == RestoreGUICtrlValInVarfield(f_protocol->GetProtocol(), l_is_plan_dynamic_executed)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("RestoreGUICtrlValInVarfield failed to restore updateCtrls value in varfield"));
    }

    //-Patni-PJS/2010Mar24/Added/IR-140

    int l_scannumber = f_protocol->GetPqmPrMode()->GetScanNumber();
    float l_couchpos   = f_protocol->GetPqmPrMode()->GetCouchPos();


    BITFLD_DECL(mask, K_ALL_PR);

    //+Patni-HAR/2010Apr05/Modifiied/ACE-Phase#3/CDS Requirement
    //+Patni-Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement //CDS Others


    PqmSaturnCoil_t l_saturn_coil; //SaturnCoil information
    int l_selected_channel_mode = -1;

    /* 	At COMPASS, ProEdit it could have changed the coil inside
    Study this information is to use a coil that contains the information */
    if (!l_is_plan_dynamic_executed && m_pqm_ptr->IsSaturnCoilSystem()) {
        memcpy(&l_saturn_coil, f_protocol->GetSaturnCoil(), sizeof(PqmSaturnCoil_t));
        l_selected_channel_mode = f_protocol->GetcoilChannelMode();
    }

    //-Patni-Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement //CDS Others
    //-Patni-HAR/2010Apr05/Modifiied/ACE-Phase#3/CDS Requirement
    GetProtocolItemValues(m_vfdb_handle, f_protocol);

    //Patni-AMT/2010Feb18/Modified/MSA0251-00125
    status_t l_dbdt_status =  m_pqm_ptr->GetdBdt()->dBdtCalc(m_vfdb_handle, f_protocol);

    /* プラン中にscanNumber,couchPosが変更になっている場合が
    * あるのでスクラッチパッド内の値ではなく、pqm内の値を使用する。
    */
    f_protocol->GetPqmPrMode()->SetScanNumber(l_scannumber);
    f_protocol->GetPqmPrMode()->SetCouchPos(l_couchpos);
    memset(mask, 0, sizeof(mask));

    if (E_ERROR != SetProtocolValues(m_vfdb_handle, f_protocol->GetProtocol(), mask,
                                     K_PR_SCAN_NUMBER, &l_scannumber,  sizeof(int32_t),
                                     K_PR_COUCH,       &l_couchpos,    sizeof(flt32_t),
                                     NULL)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetProtocolValues() failed."));
    }

    //+Patni-HAR/2010Apr05/Modifiied/ACE-Phase#3/CDS Requirement
    //+Patni-Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement	//CDS Others
    //Here we will not send message to GUi for coil refresh..
    //that will be done for all the database values later

    /* Overwrite information Coil */
    if (!l_is_plan_dynamic_executed && m_pqm_ptr->IsSaturnCoilSystem()) {
        /* CopySourceProt ProEdit information is valid if the change in coil
        Study effect of this process because the coil does not return the information */

        if (f_protocol->GetCopySourceProt() < 0) {
            CCoilChannelMode* l_coilchannelmode = m_pqm_ptr->GetCoilChannelMode();

            if ((l_coilchannelmode != NULL) && l_coilchannelmode->IsCoilChannelModeApplicable() && (l_selected_channel_mode != -1)) {
                l_coilchannelmode->SetChannelModeSelection(f_protocol->GetProtocol() , l_selected_channel_mode);
            }

            memcpy(f_protocol->GetSaturnCoil(), &l_saturn_coil, sizeof(PqmSaturnCoil_t));
            PutSaturnCoil(m_vfdb_handle, f_protocol->GetProtocol(), f_protocol->GetSaturnCoil());
            CPqmCoilData* l_coil_data = m_pqm_ptr->GetDataManager()->GetPQMCoilData();

            if (l_coil_data) {
                if (E_NO_ERROR != l_coil_data->PqmAllCoilGetSaturn(f_protocol->GetSaturnCoil(),
                        f_protocol->GetCoilDBTable())) {
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PqmAllCoilGetSaturn failed"));
                }
            }

            PutConnectedCoil(m_vfdb_handle, f_protocol->GetProtocol(), f_protocol);
            PutRecvrCoil(m_vfdb_handle, f_protocol->GetProtocol(), f_protocol);

            if (!m_pqm_ptr->GetCoilSelectionTool()->CSSetCoilName(f_protocol, m_vfdb_handle)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CSSetCoilName failed."));
            }

            //+Patni-Hemant/2010Apr26/Added/JaFT/CDS Requirement/Vantage Error
            if (!SetTxCoilInSaturn(m_vfdb_handle, f_protocol,
                                   f_protocol->GetCoilDBTable())) {

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("SetTxCoilInSaturn failed"));
            }

            //-Patni-Hemant/2010Apr26/Added/JaFT/CDS Requirement/Vantage Error
        }
    }

    //-Patni-Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement //CDS Others
    //-Patni-HAR/2010Apr05/Modifiied/ACE-Phase#3/CDS Requirement

    if (f_protocol->GetHasParent()) {
        SetScanOffsetModeFlag(f_protocol, 0);
    }

    CPqmProtocol* l_pCurProtocol = GetPrevItemFromPos(f_protocol);

    if (l_pCurProtocol != NULL) {
        AddPrescanTagsForNextProtocol(m_vfdb_handle, NULL, l_pCurProtocol->GetProtocol());
    }

    UpdateSARControlInfoOnProtocolUpdate(f_protocol);

    /* Get SAR Informations */
    //PqmSarGetValueFromVF(vfd,pmObj->core.prObj);
    //Meghana for the SAR function here..
    m_pqm_ptr->GetStudy()->GetValueFromVfSAR(f_protocol);
    RecalculateSAR(f_protocol);



    if (m_pqm_ptr->GetSARManager() != NULL) {
        m_pqm_ptr->GetSARManager()->CalcSARControlAndPutVFForAll();
    }

    f_protocol->SetHoldScan(f_protocol->GetHoldScan() | VFC_HOLD_PRE_SCAN);

    SetBreathHoldValue(f_protocol, m_vfdb_handle);

    //+Patni-HAR/2009July/Modified/JFT#425
    //Patni-HAR-SS/2010Feb09/Modified/MSA0251-00125-l_dbdt_status == E_NO_ERROR added to
    //  if condition
    if (m_pqm_ptr->GetPlanAndScan() && l_dbdt_status == E_NO_ERROR) { //Patni-PJS/2010Feb24/Added/DeFT# PROPOSAL_08-Remove EZScan
        f_protocol->SetHoldScan(f_protocol->GetHoldScan() ^ VFC_HOLD_PRE_SCAN);
    }

    //+ Patni-PJS/2009Jun10/Added/IDS#293
    /*  if(f_protocol->GetOffset().z){
            f_protocol->SetHoldScan(f_protocol->GetHoldScan() | VFC_HOLD_COUCH);
        }*/
    //+ Patni-PJS/2009Jun10/Added/IDS#293
    memset(mask, 0, sizeof(mask));

    int l_holdscan = f_protocol->GetHoldScan();

    if (E_NO_ERROR != SetProtocolValues(m_vfdb_handle, f_protocol->GetProtocol(), mask, K_PR_HOLD,
                                        &l_holdscan, sizeof(int32_t),
                                        NULL)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetProtocolValues failed."));
    }

    SearchScanMode(m_vfdb_handle, K_MODE_SET_IT);
    //PqmListReplaceItemsPos(pmObj->core.prObj->prxms,
    //         pmObj->core.prObj->pos,0);

    //PqmUiUpdateTime();

    PqmPrCheckVisualGr(f_tmp_file, "FromProlim", 0,
                       __FILE__, __LINE__);

    // + SM4 Defect#737 Himanshu 20090130
    l_str.Format(_T("Acq Order = %d"), f_protocol->GetProtocol());
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
    //  SetPascomSequenceObject(f_protocol);
    //m_pqm_ptr->WriteToUI(PQM_MSG_UPDATE_SEQUENCE_INFO);
    // - SM4 Defect#737 Himanshu 20090130

    UpdateInscanFlag(f_protocol);

    //Patni-Hemant+HAR/2010May21/Added/MaFT/MVC007927
    m_pqm_ptr->WriteToUI(PQM_MSG_PROCON_QUEUE_DONE);
    CMasterSlave* l_master_slave_ptr = m_pqm_ptr->GetMasterSlavePtr();

    if (l_master_slave_ptr) {
        const int l_master_slave = l_master_slave_ptr->IsMasterSlaveProtocol(f_protocol);

        if (PQM_SLAVE_PROTOCOL == l_master_slave) {
            CPqmProtocol* l_master_protocol_for_slave = f_protocol->GetMasterProtocol();
            CPqmProtocol* l_master_protocol = l_master_slave_ptr->GetMasterProtocolForGroupId(f_protocol->GetGroupID());

            if (NULL == l_master_protocol || ((NULL == l_master_protocol_for_slave) && !l_is_plan_dynamic_executed)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("This slave has no master"));
                l_master_slave_ptr->ChangeToNormalProtocol(f_protocol, false);

            } else {
                f_protocol->SetMasterProtocol(l_master_protocol);
            }

        } else if (PQM_MASTER_PROTOCOL == l_master_slave) {
            AutoCopyParameter_type l_parameter;
            l_parameter.m_scratchpad = f_tmp_file;

            if (E_NO_ERROR != l_master_slave_ptr->PqmPmSaveForMasterSlaveProtocol(f_protocol, /*f_tmp_file*/l_parameter, AUTO_COPY_PROCON)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PqmPmSaveForMasterSlaveProtocol failed for AUTO_COPY_PROCON"));
            }
        }
    }

    //+Patni-PJS+HAR/2010Jul09/Modified/V1.20#TMSC-REDMINE-441

    l_pqm_procon->SendMessageToProcon(PROMSG_PQM_QUEUE_DONE, 0, NULL);
    //-Patni-PJS+HAR/2010Jul09/Modified/V1.20#TMSC-REDMINE-441


    m_pqm_ptr->ProtocolEditingCompleted(f_protocol);

    //Patni-PJS/2010Feb24/Added/DeFT# PROPOSAL_08-Remove EZScan
    //Patni-HAR-SS/2010Feb09/Modified/MSA0251-00125-l_dbdt_status == E_NO_ERROR added to if cond.
    if (m_pqm_ptr->GetPlanAndScan() &&  l_dbdt_status == E_NO_ERROR &&
        //+Patni-HAR/2010Mar08/Added/MSA0251-00275
        !m_pqm_ptr->GetProcessing() &&
        !m_pqm_ptr->GetPostProcessFlag()) {
        //-Patni-HAR/2010Mar08/Added/MSA0251-00275
        CPqmProtocol* obj = NULL;

        GetTopWaitPos(&obj);
        CPqmProtocol* currobj = GetCurrentProtocol();

        //+Patni-HAR/2010May27/Added/MSA00251-00275
        CScanProcedure* l_scan_proc = m_pqm_ptr->GetAcqManager()->GetScanProcedure();

        if (NULL != l_scan_proc &&
            (PQM_CFA_RETRY_AND_SCAN == l_scan_proc->GetCFARetry())
           ) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("In Scan procedure CFA Retry is PQM_CFA_RETRY_AND_SCAN"));
            return E_NO_ERROR;
        }

        //-Patni-HAR/2010May27/Added/MSA00251-00275
        if (obj != NULL &&
            (currobj == NULL) &&
            (obj->GetPosition() == f_protocol->GetPosition())) {
            //PqmAcqScanStart();

            //+Patni-PJS/Added/2011-Mar-11/REDMINE-1529
            obj->SetProcessing(PROCESSING_VALUE_NONE);

            //m_pqm_ptr->WriteToUI(PQM_MSG_PROCON_PLANED_SCAN);
            //m_pqm_ptr->ScanSeries(obj->GetPosition()); // to start scan
            m_pqm_ptr->GetAcqManager()->CheckDelayAndScanStartForFirstWaitProtocol(obj);
        }
    }

    return E_NO_ERROR;
}

//****************************Method Header************************************
//Method Name   :PqmPmMaintainScratchPad
//Author        :PATNI/
//Purpose       :
//*****************************************************************************
status_t CPQMStudy::PqmPmMaintainScratchPad(
    CPqmProtocol* f_protocol,
    char* tmp_file,
    const SCRATCHPAD_CREATION_MODE f_sp_creation_mode
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::PqmPmMaintainScratchPad");
    //+Patni-RSG Modified and Added /2009Jun15/JFT-324

    if (f_protocol->GetProtocol() < 0) {
        return E_ERROR;
    }

    if (!(f_protocol->GetHoldScan() & VFC_HOLD_PRE_SCAN)) {

        f_protocol->SetHoldScan(f_protocol->GetHoldScan() | VFC_HOLD_PRE_SCAN);
        BITFLD_DECL(mask, MAX_NODES);
        memset(mask, 0, sizeof(mask));
        int l_hold_scan = f_protocol->GetHoldScan();
        SetProtocolValue(m_vfdb_handle, f_protocol, K_PR_HOLD, &l_hold_scan,
                         sizeof(int32_t), TRUE, PQM_STS_LIST);
    }

    if (E_NO_ERROR != CreateScratchPad(f_protocol->GetProtocol(), tmp_file, f_sp_creation_mode)) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("ScratchPad Cration Failed"));
        return E_ERROR;
    }

    //+Patni-RSG Modified and Added /2009Jun15/JFT-324

    /** 950718 : DEBUG for visual group. **/
    PqmPrCheckVisualGr(tmp_file, "ToProlim", 0, __FILE__, __LINE__);
    PqmPrSetPatientWeight(tmp_file);

    return E_NO_ERROR;
}

//+Patni+MP/2009Jun19/Added/ACE-2/B1 Shimming
//****************************Method Header************************************
//Method Name   :AddPrescanTagsForNextProtocol()
//Author        :PATNI / MRP
//Purpose       :
//*****************************************************************************
//+Patni-PJS/2010Apr21/Added/JaFT# CFA RFL Part-II
void CPQMStudy::AddPrescanTagsForNextProtocol(
    CVarFieldHandle* const f_vf_read_handle,
    CVarFieldHandle* const f_vf_write_handle,
    const int f_current_protocol,
    const bool f_write_scratchpad,
    const bool f_apply_to_current_only /*= false*/
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::AddPrescanTagsForNextProtocol");
    PQM_SCOPED_TRACE();


    //Patni-PJS/2010Apr21/Added/JaFT# CFA RFL Part-II
    if (f_current_protocol < 0) {
        return ;
    }

    CVarFieldHandle* l_vf_write_handle = (f_vf_write_handle) ? f_vf_write_handle : f_vf_read_handle;

    CPqmProtocol* l_prot = GetProtocol(f_current_protocol);

    //+TMSC-Tanoue/2010Sep03/MVC008324
    if (!l_prot) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_prot is NULL."));
        return;
    }

    //-TMSC-Tanoue/2010Sep03/MVC008324

    CString l_trace_string = _T("");
    l_trace_string.Format(_T("Protocol (Position, Acqorder) = (%d,%d)"), l_prot->GetPosition(), l_prot->GetProtocol());
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_string);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME , _T(""));
    VfPathElem_t        l_nodePath[SVD_ST_SLICE] = {
        {SVN_ST_PROTOCOL, 0},
        {SVN_ST_SUBPROTOCOL, 0},
        {SVN_ST_SLICEGROUP, 0},
        {SVN_ST_SLICE, 0}
    };

    const int l_total_elements = 14;
    VfFieldSpec_t f_vfReq[l_total_elements] = {0};

    float              l_prescan_stgc_fout_ratio1 = 0.0f;
    float              l_prescan_stgc_fout_ratio2 = 0.0f;
    float              l_prescan_stgc_fout_reflect1 = 0.0f;
    float              l_prescan_stgc_fout_reflect2 = 0.0f;
    float              l_prescan_b1_shim_amp = 0.0f;
    float              l_prescan_b1_shim_phase = 0.0f;
    f32vec2_t          l_prescan_mtgc_fout_ratio1;
    f32vec2_t          l_prescan_mtgc_fout_ratio2;
    f32vec2_t          l_prescan_mtgc_fout_reflect1;
    f32vec2_t          l_prescan_mtgc_fout_reflect2;
    float              l_prescan_stgc_fout_ratio = 0.0f;
    float              l_prescan_tgc_fout_ratio = 0.0f;
    int                l_num_mtgc = 0;

    int                l_num = 0, n = 0;

    memset(&l_prescan_mtgc_fout_ratio1, 0, sizeof(f32vec2_t));
    memset(&l_prescan_mtgc_fout_ratio2, 0, sizeof(f32vec2_t));
    memset(&l_prescan_mtgc_fout_reflect1, 0, sizeof(f32vec2_t));
    memset(&l_prescan_mtgc_fout_reflect2, 0, sizeof(f32vec2_t));

    int l_source_prot = f_current_protocol;

    if (f_apply_to_current_only && (l_prot->GetRflProt() != -1)) {

        CPqmProtocol* l_rfl_prot = GetProtocol(l_prot->GetRflProt());

        if (l_rfl_prot) {
            l_trace_string.Format(_T("Using RFL Prot for Prescan Tags. RFL Protocol (Position, Acqorder) = (%d,%d)"), l_rfl_prot->GetPosition(), l_rfl_prot->GetProtocol());
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_string);

            l_source_prot = l_prot->GetRflProt();

        } else {
            l_trace_string.Format(_T("RFL Prot (Acqorder = %d) is NULL. Error Occured..."), l_prot->GetRflProt());
        }
    }

    l_nodePath[SVD_ST_PROTOCOL - 1].index = l_source_prot;

    n = 0;
    VFF_SET_ARGS(f_vfReq[n], SVN_EXE_PRESCAN_STGC_RFOUT_RATIO1,
                 SVT_EXE_PRESCAN_STGC_RFOUT_RATIO1, sizeof(float),
                 &l_prescan_stgc_fout_ratio1, 0);
    n++;
    VFF_SET_ARGS(f_vfReq[n], SVN_EXE_PRESCAN_STGC_RFOUT_RATIO2,
                 SVT_EXE_PRESCAN_STGC_RFOUT_RATIO2, sizeof(float),
                 &l_prescan_stgc_fout_ratio2, 0);
    n++;
    VFF_SET_ARGS(f_vfReq[n], SVN_EXE_PRESCAN_STGC_REFLECT1,
                 SVT_EXE_PRESCAN_STGC_REFLECT1, sizeof(float),
                 &l_prescan_stgc_fout_reflect1, 0);
    n++;
    VFF_SET_ARGS(f_vfReq[n], SVN_EXE_PRESCAN_STGC_REFLECT2,
                 SVT_EXE_PRESCAN_STGC_REFLECT2, sizeof(float),
                 &l_prescan_stgc_fout_reflect2, 0);
    n++;
    VFF_SET_ARGS(f_vfReq[n], SVN_EXE_PRESCAN_B1_SHIM_AMP,
                 SVT_EXE_PRESCAN_B1_SHIM_AMP, sizeof(float),
                 &l_prescan_b1_shim_amp, 0);
    n++;
    VFF_SET_ARGS(f_vfReq[n], SVN_EXE_PRESCAN_B1_SHIM_PHASE,
                 SVT_EXE_PRESCAN_B1_SHIM_PHASE, sizeof(float),
                 &l_prescan_b1_shim_phase, 0);
    n++;
    VFF_SET_ARGS(f_vfReq[n], SVN_EXE_PRESCAN_MTGC_RFOUT_RATIO1,
                 SVT_EXE_PRESCAN_MTGC_RFOUT_RATIO1, sizeof(f32vec2_t),
                 &l_prescan_mtgc_fout_ratio1, 0);
    n++;
    VFF_SET_ARGS(f_vfReq[n], SVN_EXE_PRESCAN_MTGC_RFOUT_RATIO2,
                 SVT_EXE_PRESCAN_MTGC_RFOUT_RATIO2, sizeof(f32vec2_t),
                 &l_prescan_mtgc_fout_ratio2, 0);
    n++;
    VFF_SET_ARGS(f_vfReq[n], SVN_EXE_PRESCAN_MTGC_REFLECT1,
                 SVT_EXE_PRESCAN_MTGC_REFLECT1, sizeof(f32vec2_t),
                 &l_prescan_mtgc_fout_reflect1, 0);
    n++;
    VFF_SET_ARGS(f_vfReq[n], SVN_EXE_PRESCAN_MTGC_REFLECT2,
                 SVT_EXE_PRESCAN_MTGC_REFLECT2, sizeof(f32vec2_t),
                 &l_prescan_mtgc_fout_reflect2, 0);
    n++;
    VFF_SET_ARGS(f_vfReq[n], SVN_EXE_PRESCAN_STGC_RFOUT_RATIO,
                 SVT_EXE_PRESCAN_STGC_RFOUT_RATIO, sizeof(float),
                 &l_prescan_stgc_fout_ratio, 0);
    n++;
    VFF_SET_ARGS(f_vfReq[n], SVN_EXE_PRESCAN_TGC_RFOUT_RATIO,
                 SVT_EXE_PRESCAN_TGC_RFOUT_RATIO, sizeof(float),
                 &l_prescan_tgc_fout_ratio, 0);
    n++;

    VFF_SET_ARGS(f_vfReq[n], SVN_EXE_PRESCAN_NUM_MTGC,
                 SVT_EXE_PRESCAN_NUM_MTGC, sizeof(l_num_mtgc),
                 &l_num_mtgc, 0);
    n++;

    //Keep this as always LAST...
    VFF_SET_ARGS(f_vfReq[n], SVN_EXE_PRESCAN_MTGC_RFOUT_RATIO, SVT_EXE_PRESCAN_MTGC_RFOUT_RATIO,
                 0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);
    n++;

    status_t l_vfStatus = m_vfdb_handle->GetFields(l_nodePath, SVD_ST_PROTOCOL, f_vfReq, n, &l_num);

    if ((l_vfStatus != E_NO_ERROR) || (l_num <= 0)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetFields API Failed"));
        return;
    }

    l_trace_string.Format(_T("SVN_EXE_PRESCAN_STGC_RFOUT_RATIO1 = %f, status = %ld"), l_prescan_stgc_fout_ratio1, f_vfReq[0].status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_string);
    l_trace_string.Format(_T("SVT_EXE_PRESCAN_STGC_RFOUT_RATIO2 = %f, status = %ld"), l_prescan_stgc_fout_ratio2, f_vfReq[1].status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_string);
    l_trace_string.Format(_T("SVN_EXE_PRESCAN_STGC_REFLECT1 = %f, status = %ld"), l_prescan_stgc_fout_reflect1, f_vfReq[2].status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_string);
    l_trace_string.Format(_T("SVN_EXE_PRESCAN_STGC_REFLECT2 = %f, status = %ld"), l_prescan_stgc_fout_reflect2, f_vfReq[3].status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_string);
    l_trace_string.Format(_T("SVN_EXE_PRESCAN_B1_SHIM_AMP = %f, status = %ld"), l_prescan_b1_shim_amp, f_vfReq[4].status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_string);
    l_trace_string.Format(_T("SVN_EXE_PRESCAN_B1_SHIM_PHASE = %f, status = %ld"), l_prescan_b1_shim_phase, f_vfReq[5].status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_string);
    l_trace_string.Format(_T("SVN_EXE_PRESCAN_MTGC_RFOUT_RATIO1 = %f, status = %ld"), l_prescan_mtgc_fout_ratio1.x, f_vfReq[6].status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_string);
    l_trace_string.Format(_T("SVN_EXE_PRESCAN_MTGC_RFOUT_RATIO2 = %f, status = %ld"), l_prescan_mtgc_fout_ratio2.x, f_vfReq[7].status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_string);
    l_trace_string.Format(_T("SVN_EXE_PRESCAN_MTGC_REFLECT1 = %f, status = %ld"), l_prescan_mtgc_fout_reflect1.x, f_vfReq[8].status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_string);
    l_trace_string.Format(_T("SVT_EXE_PRESCAN_MTGC_REFLECT2 = %f, status = %ld"), l_prescan_mtgc_fout_reflect2.x, f_vfReq[9].status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_string);
    l_trace_string.Format(_T("SVN_EXE_PRESCAN_STGC_RFOUT_RATIO = %f, status = %ld"), l_prescan_stgc_fout_ratio, f_vfReq[10].status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_string);
    l_trace_string.Format(_T("SVN_EXE_PRESCAN_TGC_RFOUT_RATIO = %f, status = %ld"), l_prescan_tgc_fout_ratio, f_vfReq[11].status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_string);

    l_trace_string.Format(_T("SVN_EXE_PRESCAN_NUM_MTGC = %d, status = %ld"), l_num_mtgc, f_vfReq[12].status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_string);
    l_trace_string.Format(_T("SVN_EXE_PRESCAN_MTGC_RFOUT_RATIO status = %ld"), f_vfReq[13].status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_string);

    if ((E_NO_ERROR == f_vfReq[12].status) && (E_NO_ERROR == f_vfReq[13].status)) {
        const int l_total_vector_elements = (f_vfReq[13].size) / (sizeof(f32vec2_t));

        if (l_num_mtgc > l_total_vector_elements) {

            //validated SVN_EXE_PRESCAN_NUM_MTGC...
            l_num_mtgc = l_total_vector_elements ;
            l_trace_string.Format(_T("Updated SVN_EXE_PRESCAN_NUM_MTGC = %d"), l_num_mtgc);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_string);
        }
    }

    int l_start_pos = -1;
    int l_end_pos = -1;

    if (!f_write_scratchpad) {

        if (f_apply_to_current_only) {

            //we will apply for provided protocol only.
            l_start_pos = l_end_pos = l_prot->GetPosition();

        } else {

            //as per current code, we will not incldue l_prot and start apply from next
            //That is why +1 will be there...

            l_end_pos = GetAllItems();
            l_start_pos = l_prot->GetPosition() + 1;

            if (l_start_pos > l_end_pos) {

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("There is no Protocol Below."));
                return;
            }
        }
    }

    if (!WritePrescanTagsForProtocol(
            l_vf_write_handle, f_vfReq,
            l_total_elements,
            l_start_pos,
            l_end_pos,
            f_write_scratchpad)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("WritePrescanTagsForProtocol failed!"));
    }

    if (f_vfReq[l_total_elements - 1].status == E_NO_ERROR && f_vfReq[l_total_elements - 1].size > 0)
        FREE_PTR(f_vfReq[l_total_elements - 1].buffer) ;

}
//-Patni+MP/2009Jun19/Added/ACE-2/B1 Shimming

//****************************Method Header************************************
//Method Name   : PqmPrCheckSlicePosition()
//Author        : PATNI\MSN
//Purpose       :
//*****************************************************************************
bool_t CPQMStudy::PqmPrCheckSlicePosition(
    CVarFieldHandle* const f_vf_handle,
    CPqmProtocol* f_obj
)
{
    //Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(f_vf_handle);

    VfPathElem_t    pelm[] = {
        {SVN_ST_PROTOCOL,   0},
        {SVN_ST_SUBPROTOCOL,    0},
        {SVN_ST_SLICEGROUP, 0},
        {0,         0}
    };

    struct {
        flt32_t     gap;
        flt32_t     thickness;
        f32vec3_t   offVec;
        f32vec3_t   oriVec;
        int32_t     nSlices;
        double      distance;
        double      threshold;
    } vfData;
    VfFieldSpec_t   vfReq[5] = {0};//Patni-MJC/2009Aug17/Modified/cpp test corrections
    BITFLD_DECL(mask, MAX_NODES);
    bool_t  ret = FALSE;
    DB_FUNC_ENTER("[PqmPrCheckSlicePosition]:");

    pelm[SVD_ST_PROTOCOL - 1].index = f_obj->GetProtocol();

    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(vfReq, 0, sizeof(VfFieldSpec_t) * 5);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008

    int n = 0;
    VFF_SET_ARGS(vfReq[n], SVN_PLN_SELECTED_SLICE_GAP,
                 SVT_PLN_SELECTED_SLICE_GAP,
                 sizeof(vfData.gap),
                 &vfData.gap, 0);
    n++;
    int num = 0;
    status_t l_status = m_vfdb_handle->GetFields(pelm, SVD_ST_PROTOCOL, vfReq, n, &num);

    if (l_status == E_NO_ERROR) {
        if (vfReq[0].status != E_NO_ERROR) {
            vfData.gap = 0.0;
        }

    } else {
        DB_PRINTF("A", ("CPQMStudy::GetFields() error [SVN_PLN_SELECTED_SLICE_GAP] [%s] %d\n",
                        __FILE__, __LINE__));
        return TRUE;
    }

    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(vfReq, 0, sizeof(VfFieldSpec_t) * 5);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    n = 0 ;
    VFF_SET_ARGS(vfReq[n], SVN_PLN_THICKNESS,
                 SVT_PLN_THICKNESS,
                 sizeof(vfData.thickness),
                 &vfData.thickness, 0);
    n++;
    l_status = m_vfdb_handle->GetFields(pelm, SVD_ST_SLICEGROUP, vfReq, n, &num);

    if (l_status == E_NO_ERROR) {
        if (vfReq[0].status != E_NO_ERROR) {
            DB_PRINTF("A", ("CPQMStudy::GetFields() error [SVN_PLN_THICKNESS] [%s] %d\n",
                            __FILE__, __LINE__));
            return TRUE;
        }

    } else {
        DB_PRINTF("A", ("CPQMStudy::GetFields() error [SVN_PLN_THICKNESS] [%s] %d\n",
                        __FILE__, __LINE__));
        return TRUE;
    }

    pelm[SVD_ST_VISUALGROUP - 1].subtree_name = SVN_ST_VISUALGROUP ;
    pelm[SVD_ST_SLICEGROUP - 1].subtree_name = 0 ;

    memset(mask, 0, sizeof(mask));
    int     total = MAX_NODES;
    int     active = 0;
    l_status = m_vfdb_handle->ScanSubtree(pelm, SVD_ST_PROTOCOL,
                                          SVN_ST_VISUALGROUP,
                                          mask, &total, &active);

    if (l_status == E_NO_ERROR && active > 0) {
        int i = 0;//Patni-MJC/2009Aug17/Modified/cpp test corrections
        float   offset = 0.0;
        float   couch_offset = 0.0;

        if (PLAN_CHECK(f_obj)) {
            couch_offset = (float)(COUCH_OFFSET(f_obj) / 1000.0);
        }

        for (i = 0; i < total; i++) {
            if (BITFLD_ON(mask, i)) {
                pelm[SVD_ST_VISUALGROUP - 1].index = i;
                // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
                memset(vfReq, 0, sizeof(VfFieldSpec_t) * 5);
                // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
                n = 0;
                VFF_SET_ARGS(vfReq[n],
                             SVN_PLN_VG_OFFSET_VECTOR,
                             SVT_PLN_VG_OFFSET_VECTOR,
                             sizeof(f32vec3_t), &vfData.offVec, 0);
                n++;
                VFF_SET_ARGS(vfReq[n],
                             SVN_PLN_VG_SLICE_ORI_VECTOR,
                             SVT_PLN_VG_SLICE_ORI_VECTOR,
                             sizeof(f32vec3_t), &vfData.oriVec, 0);
                n++;
                VFF_SET_ARGS(vfReq[n],
                             SVN_PLN_SLICES_IN_VG,
                             SVT_PLN_SLICES_IN_VG,
                             sizeof(int32_t), &vfData.nSlices, 0);
                n++;
                l_status = m_vfdb_handle->GetFields(pelm, SVD_ST_VISUALGROUP,
                                                    vfReq, n, &num);

                if (l_status == E_NO_ERROR &&
                    vfReq[0].status == E_NO_ERROR &&
                    vfReq[1].status == E_NO_ERROR &&
                    vfReq[2].status == E_NO_ERROR) {
                    //+Patni-Raj/2010Jan15/Added/PH#3#/Debug Window
                    CPQMDebugInfo::GetInstance(m_pqm_ptr)->PqmDebugAcqMonitor(_T("couch_offset : %f = %f + %f  "), (vfData.offVec.z + couch_offset), vfData.offVec.z, couch_offset);
#if 0
                    CPQMDebugInfo::GetInstance(m_pqm_ptr)->PqmDebugAcqMonitor(_T("offset : %f "), offset);
                    CPQMDebugInfo::GetInstance(m_pqm_ptr)->PqmDebugAcqMonitor(_T("vfData.thickness: %f "), vfData.thickness);
                    CPQMDebugInfo::GetInstance(m_pqm_ptr)->PqmDebugAcqMonitor(_T("vfData.gap : %f "), vfData.gap);
                    CPQMDebugInfo::GetInstance(m_pqm_ptr)->PqmDebugAcqMonitor(_T("vfData.nSlices : %d "), vfData.nSlices);
#endif
                    //-Patni-Raj/2010Jan15/Added/PH#3#/Debug Window

                    //+ Patni-MSN/Added On 22March2009//PSP1#33
                    if (AcqCheckSlicePosition((double)offset,
                                              (double)(vfData.offVec.z + couch_offset),
                                              (double)vfData.oriVec.z,
                                              (double)vfData.thickness,
                                              (double)vfData.gap,
                                              vfData.nSlices,
                                              &vfData.distance,
                                              &vfData.threshold) == ACQ_ERROR_DISTANCE) {
                        char    buf[512] = "";//Patni-MJC/2009Aug17/Modified/cpp test corrections

                        //                      sprintf( buf, " [%s:/1.%d] Slab[%d],couch_offset[%f=%f+%f],ST[%f],Gap[%f],NS[%d]",
                        //                          m_pqm_ptr->GetStudy()->GetStudyName(), f_obj->GetProtocol(), i,
                        //                          (vfData.offVec.z+couch_offset),vfData.offVec.z,couch_offset,
                        //                          vfData.thickness,vfData.gap,vfData.nSlices ) ;
                        //                      ERROR_LOG(buf)

                        CPQMLogMgr::GetInstance()->WriteEventLog(_T("CPQMStudy::PqmPrCheckSlicePosition"), ERROR_LOG_CRUCIAL1 ,
                                buf, __LINE__);  // MEITEC/2010May24/IR-82
                        ret = TRUE;
                        break;
                    }

                    //- Patni-MSN/Added On 22March2009//PSP1#33
                }
            }
        }
    }

    return ret;
}

//****************************Method Header************************************
//Method Name   :PutSaturnCoilDegeneracy()
//Author        : PATNI/HAR-AMT
//Purpose       : Put Saturn coil degeneracy into Database file
//*****************************************************************************
int CPQMStudy::PutSaturnCoilDegeneracy(
    const int             f_prot,
    PqmSaturnCoil_t* f_saturn_coil
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::PutSaturnCoilDegeneracy");
    PqmSaturnCoil_t l_saturn_coil;
    BITFLD_DECL(sub_mask, K_ALL_PR);

    BITFLD_INIT(sub_mask, K_ALL_PR, 0);

    //+Patni-MRP/2009Sep08/Modified/MVC004380
    if (f_saturn_coil == NULL) {
        return E_ERROR;
    }

    //-Patni-MRP/2009Sep08/Modified/MVC004380

    l_saturn_coil = *f_saturn_coil;
    status_t        l_sts   = E_NO_ERROR;

    if (l_saturn_coil.numSection) {

        int n = 0, l_num = 0;
        // SignalDegeneracy
        int s = sizeof(int16_t) ;
        VfFieldSpec_t   l_vfReq[2] = {0};
        VFF_SET_ARGS(l_vfReq[n], SVN_EXE_NUM_PAC_DEGENERACY,
                     SVT_EXE_NUM_PAC_DEGENERACY,  s, &l_saturn_coil.numSignalDegeneracy,
                     VFO_REPLACE);
        n++;

        //  If degeneracy is present then only save information
        if (l_saturn_coil.numSignalDegeneracy) {

            s = l_saturn_coil.numSection * sizeof(VftSectionSignal_t) ;
            VFF_SET_ARGS(l_vfReq[n], SVN_PLN_SECTION_RECVR_COIL_SIGNAL_DEGENERACY,
                         SVT_PLN_SECTION_RECVR_COIL_SIGNAL_DEGENERACY, s,
                         &l_saturn_coil.SignalDegeneracy, VFO_REPLACE);
            n++;
        }

        time_t          l_tm    = {0};
        VfPathElem_t    l_pelm[2] = {0};
        l_pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
        l_pelm[SVD_ST_PROTOCOL - 1].index = f_prot;
        l_sts = m_vfdb_handle->PutFields(l_pelm, SVD_ST_PROTOCOL, l_vfReq, n, &l_num, &l_tm);

        if (l_sts != E_NO_ERROR || n != l_num) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
            l_sts = E_ERROR ;
        }
    }

    return l_sts ;
}

//+Patni-Hemant/2010Feb16/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : PqmSetWBCoilCDS
// Author       : PATNI/ HEMANT
// Purpose      : Coil Set CDS WB
//***********************************************************************************
status_t CPQMStudy::PqmSetWBCoilCDS(
    CVarFieldHandle* const f_vf_handle,
    int f_protocol,
    PqmSaturnCoil_t* f_saturnCoil,
    PqmCoil_t* f_coil_p
)
{

    LPCTSTR FUNC_NAME = _T("CPQMStudy::PqmSetWBCoilCDS");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CPQMStudy::PqmSetWBCoilCDS"));

    path_db_rec_t pathP ;
    BITFLD_DECL(mask, K_ALL_PR);

    CPqmProtocol* l_protocol = GetItemFromProtocol(f_protocol);

    //DB_FUNC_ENTER("[PqmSetWBCoilCDS]:");

    if (!f_saturnCoil || !f_coil_p || !l_protocol) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_saturnCoil or f_coil_p or l_protocol is NULL"));

        return E_ERROR ;
    }

    CDataManager* l_data_mgr = m_pqm_ptr->GetDataManager();
    CPqmCoilData* l_coil_data = l_data_mgr ->GetPQMCoilData();

    /* 	Target new coils and coil in either conventional RF coils,
    Coil of the coil will WholeBody traditional information	*/
    if ((f_coil_p->mview & 0x80000000)) {
        /*PATHDB conventional coil so that the information set */

        BOOL flg = FALSE;

        memset(&f_saturnCoil->Signal , NULL , sizeof(f_saturnCoil->Signal)) ;
        memset(&f_saturnCoil->RcvCoil , NULL , sizeof(f_saturnCoil->RcvCoil)) ;
        memset(&f_saturnCoil->CoilName , NULL , sizeof(f_saturnCoil->CoilName)) ;

        if (l_coil_data->PqmCoilGetPathDB(f_coil_p->mview & COIL_PHYSICALID_MASK, &pathP) == FALSE) {
            return E_ERROR ;
        }

        f_saturnCoil->numSection = 1 ;

        /** Mview PathDB set from here to get information about **/
        int signal = pathP.path_data[0];
        int i = 0, j = 0;

        for (j = i = 0 ; i < MAX_SIGNAL_COIL ; i++) {
            if (signal & (0x0001 << i)) {
                f_saturnCoil->Signal[j].signal_no[j] = signal & (0x0001 << i) ;
                f_saturnCoil->Signal[j].num++ ;
            }
        }

        f_saturnCoil->RcvCoil[0].portID[0] =  f_coil_p->port;
        f_saturnCoil->RcvCoil[0].portID[1] =  ' ';	/*  Space */
        f_saturnCoil->RcvCoil[0].coilID    =  f_coil_p->coil_id & COIL_PHYSICALID_MASK ;
        f_saturnCoil->RcvCoil[0].channel   =  f_coil_p->channel;

        for (i = 0; i < f_saturnCoil->CoilList.num; i++) {
            if ((f_saturnCoil->CoilList.coillist[i].portID[0] ==  f_coil_p->port) &&
                (f_saturnCoil->CoilList.coillist[i].coilID    == (f_coil_p->coil_id & COIL_PHYSICALID_MASK)) &&
                (f_saturnCoil->CoilList.coillist[i].channel   ==  f_coil_p->channel)) {
                flg = TRUE;
            }
        }

        if (!flg) {
            f_saturnCoil->CoilList.coillist[f_saturnCoil->CoilList.num].portID[0] =  f_coil_p->port;
            f_saturnCoil->CoilList.coillist[f_saturnCoil->CoilList.num].portID[1] =  ' ';	/*  Space  */
            f_saturnCoil->CoilList.coillist[f_saturnCoil->CoilList.num].coilID    =  f_coil_p->coil_id & COIL_PHYSICALID_MASK ;
            f_saturnCoil->CoilList.coillist[f_saturnCoil->CoilList.num].channel   =  f_coil_p->channel;
            f_saturnCoil->CoilList.num++;
        }

        PutSaturnCoil(f_vf_handle, f_protocol , f_saturnCoil);

        /* Receiver coil to create a table name */
        if (E_NO_ERROR != l_coil_data->PqmAllCoilGetSaturn(f_saturnCoil, l_protocol->GetCoilDBTable())) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PqmAllCoilGetSaturn failed"));
        }

        PutRecvrCoil(f_vf_handle, f_protocol, l_protocol);

        memcpy(f_saturnCoil->CoilName , f_coil_p->label, (strlen(f_coil_p->label) + 1));

        BITFLD_INIT(mask, K_ALL_PR, 0);

        //		PqmPrVaSetProtValues(f_vfhandle,f_protocol,mask,
        //				K_PR_RC_COIL_NAME, f_saturnCoil->CoilName,	strlen(f_saturnCoil->CoilName)+1,
        //				K_PR_NUM_PAC_SCAN, &f_coil_p->num_channels ,sizeof(int16_t),
        //				NULL );

        SetProtValues(
            f_vf_handle, f_protocol, mask,
            K_PR_RC_COIL_NAME, f_saturnCoil->CoilName,	strlen(f_saturnCoil->CoilName) + 1,
            K_PR_NUM_PAC_SCAN, &f_coil_p->num_channels , sizeof(int16_t),
            NULL);

    }




    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exiting CPQMStudy::PqmSetWBCoilCDS"));

    return E_NO_ERROR ;
}
//-Patni-Hemant/2010Feb16/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : IsProtocolInScan
// Author       : PATNI/ HEMANT
// Purpose      : returns true if given protocol index is of kind inscan.
//***********************************************************************************
bool CPQMStudy::IsProtocolInScan(
    const int f_protocol_acq_order
)
{

    const int l_inscan_flag = GetInscanFlagForProtocolIndex(f_protocol_acq_order);

    if (l_inscan_flag == -1) {

        //ASSERT(FALSE);
        return false ;
    }

    return (IsThisFlagInScanKind(l_inscan_flag));
}

//****************************Method Header************************************
//Method Name   :SpeederSetDynamic()
//Author        :PATNI /MRP
//Purpose       :コイル情報から撮影可能なMAP種を取得する
//*****************************************************************************
status_t CPQMStudy::SpeederSetDynamic(const int prot, int* CoilTbl)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SpeederSetDynamic");
    VfPathElem_t        path[5] = {0};//Patni-MJC/2009Aug17/Modified/cpp test corrections

    VfFieldSpec_t       fieldSpec[8] = {0};//Patni-MJC/2009Aug17/Modified/cpp test corrections
    VftTimeCtrlParam_t  timeCtrl;

    DB_FUNC_ENTER("[PqmSpeederCopySubProtocol]:");

    memset(&timeCtrl , NULL , sizeof(timeCtrl)) ;

    memset(path, 0, sizeof(VfPathElem_t) * 5);//Patni-MJC/2009Aug17/Modified/cpp test corrections
    path[0].subtree_name = SVN_ST_PROTOCOL;
    path[0].index = prot;
    path[1].subtree_name = SVN_ST_SUBPROTOCOL;
    path[1].index = 0;

    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(fieldSpec, 0, sizeof(VfFieldSpec_t) * 8);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    int n = 0;
    int total_time = 0;
    VFF_SET_ARGS(fieldSpec[n], SVN_PLN_SUBPROTOCOL_TIME, VFT_INT32,
                 sizeof(total_time), &total_time, 0L);
    n++;

    int numDone = 0;
    status_t status = m_vfdb_handle->GetFields(path, SVD_ST_SUBPROTOCOL, fieldSpec, n, (int*) & numDone);

    //+Patni-NP/2010Jul27/Added/IR-161
    CPQMStudy* l_study = m_pqm_ptr->GetStudy();

    if (l_study == NULL) {
        return E_ERROR;
    }

    // CPqmProtocol* l_curr_prot = GetCurrentProtocol();
    CPqmProtocol* l_curr_prot = l_study->GetProtocol(prot);//IR-161
    //-Patni-NP/2010Jul27/Added/IR-161

    //+Patni-RUP/2009Aug21/Modified/cpp test corrections
    if (l_curr_prot == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("l_curr_prot pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-RUP/2009Aug21/Modified/cpp test corrections
    int voice_id = 0;

    if (n == numDone) {
        if ((m_pqm_ptr->GetAvSpdrMap() == TRUE) &&
            (l_curr_prot->GetExeAutoVoice() & 0x0F)) {

            /* のりしろ用に100msを追加 */
            voice_id = (l_curr_prot->GetExeAutoVoice() & 0x0F) ;
            CVoiceManager* voice_manager = CVoiceManager::GetInstance();
            int length =  voice_manager->GetVoiceLength(voice_id) +
                          voice_manager->GetVoiceLength(PQM_AV_VOICE_5) +
                          voice_manager->GetAvDynamicDelay() + m_pqm_ptr->GetAvSpdrMapDelay() + 100;


            l_curr_prot->SetPlnAutoVoice(voice_id);


            memset(&timeCtrl , NULL , sizeof(timeCtrl)) ;
            timeCtrl.totalTime = ((total_time) * 2 * 1000) + length ;
            timeCtrl.numSegment = 1;
            timeCtrl.segment[0].period = ((total_time) * 1000) + length ;
            timeCtrl.segment[0].repeat = 2 ;
            total_time *= 2 ;
            total_time += (length / 1000) ;

        } else {
            memset(&timeCtrl , NULL , sizeof(timeCtrl)) ;
            timeCtrl.totalTime = ((total_time) * 2 * 1000) ;
            timeCtrl.numSegment = 1;
            timeCtrl.segment[0].period = ((total_time) * 1000) ;
            timeCtrl.segment[0].repeat = 2 ;
            total_time *= 2 ;
        }

    } else {
        DB_PRINTF("S", ("%s:%d  CPQMStudy::GetFields ERROR num_Req=%d sts=%d numDone=%d\n",
                        __FILE__, __LINE__, n , status, numDone));
        DB_FUNC_EXIT() ;
        return(E_ERROR) ;
    }

    if (CoilTbl[0]  != NULL) {
        if (SetPVCoilTbl(prot, CoilTbl , 2) != E_NO_ERROR) {
            DB_FUNC_EXIT() ;
            return(E_ERROR) ;
        }
    }

    int i = TRUE ;
    n = 0;
    VFF_SET_ARGS(fieldSpec[n], SVN_PLN_TIME_CTRL_PARAM, VFT_TIME_CTRL_PARAM,
                 sizeof(timeCtrl), &timeCtrl, VFO_REPLACE);
    n++;
    VFF_SET_ARGS(fieldSpec[n], SVN_PLN_TOTAL_TIME, VFT_INT32,
                 sizeof(total_time), &total_time, VFO_REPLACE);
    n++;
    VFF_SET_ARGS(fieldSpec[n], SVN_PLN_DYNAMIC_SCAN, VFT_INT32,
                 sizeof(i), &i, VFO_REPLACE);
    n++;
    VFF_SET_ARGS(fieldSpec[n], SVN_PLN_TIME_DIM_SCAN, VFT_INT32,
                 sizeof(i), &i, VFO_REPLACE);
    n++;
    int ii = 2 ;
    VFF_SET_ARGS(fieldSpec[n], SVN_PLN_REPETITIONS, VFT_INT32,
                 sizeof(i), &ii, VFO_REPLACE);
    n++;

    VFF_SET_ARGS(fieldSpec[n], SVN_PLN_AUTO_VOICE, VFT_INT32,
                 sizeof(voice_id), &voice_id, VFO_REPLACE);
    n++;

    //+Patni-SS/2009Apr01/PSP1#VFLOCK
    CString l_log_msg = _T("");
    l_log_msg.Format(_T("SVN_PLN_TIME_CTRL_PARAM =%d,SVN_PLN_TOTAL_TIME = %d, SVN_PLN_DYNAMIC_SCAN = %d,SVN_PLN_TIME_DIM_SCAN= %d,SVN_PLN_REPETITIONS = %d,SVN_PLN_AUTO_VOICE=%d"),
                     timeCtrl, total_time, i, i, voice_id);
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_log_msg);

    time_t              ts = 0;
    status = m_vfdb_handle->PutFields(path, SVD_ST_PROTOCOL, fieldSpec, n, (int*) & numDone, &ts);
    CPqmPmAcqman* pqm_pm_acqman = m_pqm_ptr->GetAcqManager();
    CScanProcedure* scan_proc = pqm_pm_acqman->GetScanProcedure();

    scan_proc->SetDynamicVoice();

    /* 書き込んだ情報を読み込み内部情報と同じにする */
    GetProtocolItemValues(m_vfdb_handle, l_curr_prot);

    if (n != numDone) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
        return(E_ERROR) ;
    }

    else
        return(E_NO_ERROR) ;
}

//****************************Method Header************************************
//Method Name   :SpeederCoilMode()
//Author        :PATNI /MRP
//Purpose       :
//*****************************************************************************
int32_t CPQMStudy::SpeederCoilMode(const int prot)const
{
    int             n = 0, num = 0, s = sizeof(int32_t);//Patni-MJC/2009Aug17/Modified/cpp test corrections
    int32_t         switch_coil_type = 0;
    VfPathElem_t    pelm[2] = {0};//Patni-MJC/2009Aug17/Modified/cpp test corrections
    VfFieldSpec_t   vfReq[1] = {0};//Patni-MJC/2009Aug17/Modified/cpp test corrections

    DB_FUNC_ENTER("[pqmSpeederCoilMode]:");

    memset(pelm, 0, sizeof(VfPathElem_t) * 2);  //Patni-MJC/2009Aug17/Modified/cpp test corrections
    pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    pelm[SVD_ST_PROTOCOL - 1].index = prot;
    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(vfReq, 0, sizeof(VfFieldSpec_t));
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    // n = num = 0;//Patni-MJC/2009Aug17/Modified/cpp test corrections
    // s = sizeof(int32_t) ;//Patni-MJC/2009Aug17/Modified/cpp test corrections
    VFF_SET_ARGS(vfReq[n], SVN_PLN_SWITCH_COIL_TYPE, SVT_PLN_SWITCH_COIL_TYPE,
                 s, &switch_coil_type, NULL);
    n++;

    status_t sts = m_vfdb_handle->GetFields(pelm, SVD_ST_PROTOCOL, vfReq, n, &num);

    if (sts != E_NO_ERROR || n != num) {
        switch_coil_type = VFC_SWITCH_COIL_TYPE_NONE ;
    }

    return switch_coil_type;
}

//****************************Method Header************************************
//Method Name   :SpeederClearMapPath()
//Author        :PATNI /MRP
//Purpose       :コイル情報から撮影可能なMAP種を取得する
//*****************************************************************************
status_t CPQMStudy::SpeederClearMapPath(const int seqProt)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SpeederClearMapPath");


    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    VfFieldSpec_t       fieldSpec[2] = {0};//Patni-MJC/2009Aug17/Modified/cpp test corrections
    VfPathElem_t        path[5] = {0};//Patni-MJC/2009Aug17/Modified/cpp test corrections

    memset(fieldSpec, 0, sizeof(VfFieldSpec_t) * 2);//Patni-MJC/2009Aug17/Modified/cpp test corrections
    memset(path, 0, sizeof(VfPathElem_t) * 5);      //Patni-MJC/2009Aug17/Modified/cpp test corrections

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Inside SpeederClearMapPath"));

    //DB_PRINTF("S",("dist_Prot=%d \n",seqProt));
    CString l_trace_string = _T("");
    l_trace_string.Format(_T("dist_Prot=%d "), seqProt);


    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);

    path[0].subtree_name = SVN_ST_PROTOCOL;
    path[0].index = seqProt;

    int n = 0;
    VFF_SET_ARGS(fieldSpec[n], SVN_SEQ_MAP_PATH , VFT_PATHELEM,
                 0 , NULL , VFO_DELETE);
    n++;

    //+Patni-SS/2009Apr01/PSP1#VFLOCK
    CString l_log_msg = _T("");
    l_log_msg.Format(_T("SVN_SEQ_MAP_PATH =%d "), 0);
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_log_msg);

    int numDone = 0;
    time_t              ts = 0;
    status_t l_status = m_vfdb_handle->PutFields(path, SVD_ST_PROTOCOL, fieldSpec, n, (int*) & numDone, &ts);

    if (l_status != E_NO_ERROR || n != numDone) {
        l_trace_string.Format(_T("CPQMStudy::PutFields ERROR sts=%d numDone=%d\n"),
                              l_status, numDone);
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);
        return E_ERROR ;
    }

    return E_NO_ERROR;
}

//****************************Method Header************************************
//Method Name   :SpeederSetMapPath()
//Author        :PATNI /MRP
//Purpose       :コイル情報から撮影可能なMAP種を取得する
//*****************************************************************************
status_t CPQMStudy::SpeederSetMapPath(const int seqProt, const int mapProt)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SpeederSetMapPath");

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    VfFieldSpec_t       fieldSpec[2] = {0};//Patni-MJC/2009Aug17/Modified/cpp test corrections
    VfPathElem_t        path[5] = {0};//Patni-MJC/2009Aug17/Modified/cpp test corrections
    VfPathElem_t        mapPath[4] = {0};//Patni-MJC/2009Aug17/Modified/cpp test corrections

    memset(fieldSpec, 0, sizeof(VfFieldSpec_t) * 2);    //Patni-MJC/2009Aug17/Modified/cpp test corrections
    memset(path, 0, sizeof(VfPathElem_t) * 5);          //Patni-MJC/2009Aug17/Modified/cpp test corrections
    memset(mapPath, 0, sizeof(VfPathElem_t) * 4);       //Patni-MJC/2009Aug17/Modified/cpp test corrections

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Inside PqmSpeederSetMapPath"));

    CString l_trace_string = _T("");
    l_trace_string.Format(_T("dist_Prot=%d   map_Prot=%d \n"), seqProt, mapProt);

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);

    memset(mapPath , NULL , sizeof(mapPath));
    path[0].subtree_name = SVN_ST_PROTOCOL;
    path[0].index = seqProt;
    memset(mapPath , NULL , sizeof(mapPath)) ;
    mapPath[0].subtree_name = SVN_ST_PROTOCOL;
    mapPath[0].index = mapProt;
    mapPath[1].subtree_name = SVN_ST_PROCESSED;
    mapPath[1].index = 0;

    int n = 0;
    VFF_SET_ARGS(fieldSpec[n], SVN_SEQ_MAP_PATH , VFT_PATHELEM,
                 sizeof(mapPath) , mapPath , VFO_REPLACE);
    n++;

    //+Patni-SS/2009Apr01/PSP1#VFLOCK
    l_trace_string.Format(_T("SVN_SEQ_MAP_PATH =%d "), mapPath);
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);

    int numDone = 0;
    time_t ts = 0;
    status_t status = m_vfdb_handle->PutFields(path, SVD_ST_PROTOCOL, fieldSpec, n, (int*) & numDone, &ts);

    if (status != E_NO_ERROR || n != numDone) {
        l_trace_string.Format(_T("CPQMStudy::PutFields ERROR sts=%d numDone=%d\n"),
                              status, numDone);
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);
        return E_ERROR ;
    }

    return E_NO_ERROR;
}

//****************************Method Header************************************
//Method Name   :SpeederSetMapType()
//Author        :PATNI /MRP
//Purpose       :コイル情報から撮影可能なMAP種を取得する
//*****************************************************************************
status_t CPQMStudy::SpeederSetMapType(const int prot, int type)const
{
    LPCTSTR FUNC_NAME = _T(" CPQMStudy::SpeederSetMapType");

    VfFieldSpec_t       fieldSpec[2] = {0};//Patni-MJC/2009Aug17/Modified/cpp test corrections
    VfPathElem_t        path[5] = {0};//Patni-MJC/2009Aug17/Modified/cpp test corrections

    memset(fieldSpec, 0, sizeof(VfFieldSpec_t) * 2);//Patni-MJC/2009Aug17/Modified/cpp test corrections
    memset(path, 0, sizeof(VfPathElem_t) * 5);      //Patni-MJC/2009Aug17/Modified/cpp test corrections

    DB_FUNC_ENTER("[PqmSpeederSetMapType]:");
    DB_PRINTF("S", ("Prot=%d Type=%d \n", prot, type));

    path[0].subtree_name = SVN_ST_PROTOCOL;
    path[0].index = prot;

    int n = 0;
    VFF_SET_ARGS(fieldSpec[n], SVN_PRC_INTENSITY_MAP_TYPE, VFT_BITFIELD,
                 sizeof(type) , &type , VFO_REPLACE);
    n++;

    //+Patni-SS/2009Apr01/PSP1#VFLOCK
    CString l_log_msg = _T("");
    l_log_msg.Format(_T("SVN_PRC_INTENSITY_MAP_TYPE =%d "), type);
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_log_msg);
    time_t              ts = 0;
    int numDone = 0;
    status_t status = m_vfdb_handle->PutFields(path, SVD_ST_PROTOCOL, fieldSpec, n, (int*) & numDone, &ts);

    if (status != E_NO_ERROR || n != numDone) {
        DB_PRINTF("S", ("CPQMStudy::PutFields ERROR sts=%d numDone=%d\n", status, numDone));
        return E_ERROR ;
    }

    DB_FUNC_EXIT() ;
    return E_NO_ERROR;
}

//+MEITEC/2010Aug03/Added Comment/IR-161
//*****************************************************************************
//Method Name   : SpeederResetDynamic
//Author        : Patni/NP
//Purpose       :
//*****************************************************************************
status_t CPQMStudy::SpeederResetDynamic(CVarFieldHandle* const vf_handle, CPqmProtocol* f_current_protocol /*PqmPrData_t *obj*/)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SpeederResetDynamic");

    VfPathElem_t    l_path[5] = {0} ;
    time_t         	l_ts = 0L;
    int				l_total_time = 0;
    int             l_num = 0 , l_i = 0 , l_ii = 0, l_numDone = 0 ;
    VfFieldSpec_t   l_fieldSpec[8] = {0};


    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));
    //    DB_FUNC_ENTER("[PqmSpeederResetDynamic]:");

    if (f_current_protocol->GetSpeederType() != VFC_SPEEDER_TYPE_SENSITIVITY) {
        /* MAPプロトコルだけダイナミック情報を削除する */
        return(E_NO_ERROR) ;
    }

    l_path[0].subtree_name = SVN_ST_PROTOCOL;
    l_path[0].index = f_current_protocol->GetProtocol();
    l_path[1].subtree_name = SVN_ST_SUBPROTOCOL;
    l_path[1].index = 0;

    /* SVN_PLN_SUBPROTOCOL_TIMEを取得する */
    VFF_SET_ARGS(l_fieldSpec[l_num], SVN_PLN_SUBPROTOCOL_TIME, VFT_INT32,
                 sizeof(l_total_time), &l_total_time, 0L);
    l_num++;
    status_t l_status = vf_handle->GetFields(l_path, SVD_ST_SUBPROTOCOL, l_fieldSpec, l_num, (int*)&l_numDone);

    if (l_num != l_numDone) {
        /*DB_PRINTF("S",("%s:%d  VfGetFields ERROR num_Req=%d sts=%d numDone=%d\n",
        	__FILE__,__LINE__, n ,status, numDone));
        DB_FUNC_EXIT() ;
        return( E_ERROR ) ;*/
    }

    l_i = FALSE ;
    l_num = 0;
    /* SVN_PLN_TOTAL_TIMEにSVN_PLN_SUBPROTOCOL_TIMEを上書きする */
    VFF_SET_ARGS(l_fieldSpec[l_num], SVN_PLN_TOTAL_TIME, VFT_INT32,
                 sizeof(l_total_time), &l_total_time, VFO_REPLACE);
    l_num++;
    /* 以下のDynamic情報をデフォルトにする。 */
    VFF_SET_ARGS(l_fieldSpec[l_num], SVN_PLN_DYNAMIC_SCAN, VFT_INT32,
                 sizeof(l_i), &l_i, VFO_REPLACE);
    l_num++;
    VFF_SET_ARGS(l_fieldSpec[l_num], SVN_PLN_TIME_DIM_SCAN, VFT_INT32,
                 sizeof(l_i), &l_i, VFO_REPLACE);
    l_num++;
    l_ii = 1 ;
    VFF_SET_ARGS(l_fieldSpec[l_num], SVN_PLN_REPETITIONS, VFT_INT32,
                 sizeof(l_i), &l_ii, VFO_REPLACE);
    l_num++;

    l_status = vf_handle->PutFields(l_path, SVD_ST_PROTOCOL, l_fieldSpec, l_num, (int*)&l_numDone, &l_ts);

    /* total timeの内部情報を書き込んだ情報と同期 */
    f_current_protocol->SetScanTime(l_total_time);// = total_time;

    //	PqmPqReMakeList(vfd,PqmPqGetPosFromProt(f_current_protocol->GetProtocol()),PQM_DAT_LIST,TRUE);

    if (l_num != l_numDone) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
        return(E_ERROR) ;

    } else {
        return(E_NO_ERROR) ;
    }
}

//****************************Method Header************************************
//Method Name   : PqmPrMaintainGdc()
//Author        : PATNI\HAR
//Purpose       :
//*****************************************************************************
status_t  CPQMStudy::PqmPrMaintainGdc(
    CVarFieldHandle* const f_vf_handle,
    const int f_prot
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::PqmPrMaintainGdc");

    //Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(f_vf_handle);

    //+Patni-MJC/2009Aug27/Modified/cpp test corrections
    if (m_pqm_ptr == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return E_ERROR;
    }

    VfPathElem_t    pelm[1] = {0};
    memset(pelm, 0, sizeof(VfPathElem_t) * 1);          //Patni-MJC/2009Aug17/Modified/cpp test corrections
    pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    pelm[SVD_ST_PROTOCOL - 1].index = f_prot;

    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    VfFieldSpec_t   vfReq[6] = {0};
    memset(vfReq, 0, sizeof(VfFieldSpec_t) * 6);
    VfFieldSpec_t   vfChkReq[6] = {0};
    memset(vfChkReq, 0, sizeof(VfFieldSpec_t) * 6);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    int req = 0;
    char* l_system_type = m_pqm_ptr->GetSystemType();

    VFF_SET_ARGS(vfReq[req],
                 SVN_GEN_SYSTEM_TYPE,
                 SVT_GEN_SYSTEM_TYPE,
                 strlen(l_system_type) + 1,
                 l_system_type,
                 VFO_REPLACE);

    req++;
    int n = 0;
    gdcMode_t       gdcMode;

    VFF_SET_ARGS(vfChkReq[n],
                 SVN_PRC_GDC_FLAG,
                 SVT_PRC_GDC_FLAG,
                 sizeof(gdcMode.gdcFlag),
                 &gdcMode.gdcFlag,
                 0);
    n++;

    VFF_SET_ARGS(vfChkReq[n],
                 SVN_PRC_GDC_LOCATOR_IMAGE,
                 SVT_PRC_GDC_LOCATOR_IMAGE,
                 sizeof(gdcMode.locatorImageFlag),
                 &gdcMode.locatorImageFlag,
                 0);
    n++;

    VFF_SET_ARGS(vfChkReq[n],
                 SVN_PRC_GDC_LOCATOR_MASK,
                 SVT_PRC_GDC_LOCATOR_MASK,
                 sizeof(gdcMode.maskImageFlag),
                 &gdcMode.maskImageFlag,
                 0);
    n++;

    VFF_SET_ARGS(vfChkReq[n],
                 SVN_PRC_AUTO_GDC_ACTION,
                 SVT_PRC_AUTO_GDC_ACTION,
                 sizeof(gdcMode.autoGDCFlag),
                 &gdcMode.autoGDCFlag,
                 0);
    n++;
    /* V9.20 チャネル縮退対応 */
    bool_t          degeneracyFlg = FALSE;
    VFF_SET_ARGS(vfChkReq[n],
                 SVN_PLN_DEGENERACY_FLAG,
                 SVT_PLN_DEGENERACY_FLAG,
                 sizeof(degeneracyFlg),
                 &degeneracyFlg,
                 0);
    n++;

    gdcMode_t l_gdc_mode;
    memcpy(&l_gdc_mode, &m_pqm_ptr->GetGDCMode(), sizeof(gdcMode_t));
    int l_gdc_flag = l_gdc_mode.gdcFlag;
    int l_locator_image_flag = l_gdc_mode.locatorImageFlag;
    int l_mask_image_flag = l_gdc_mode.maskImageFlag;
    int l_auto_gdc_flag = l_gdc_mode.autoGDCFlag;
    int l_harmonization_flag = m_pqm_ptr->GetHarmonizationMode().harmonizationFlag;

    int num = 0;
    time_t tm = 0;
    status_t l_status = m_vfdb_handle->GetFields(pelm, SVD_ST_PROTOCOL, vfChkReq, n, &num);

    if (l_status == E_NO_ERROR) {
        if (vfChkReq[0].status == E_NO_ERROR
            && l_gdc_flag != -1) {

            VFF_SET_ARGS(vfReq[req],
                         SVN_PRC_GDC_FLAG,
                         SVT_PRC_GDC_FLAG,
                         sizeof(gdcMode.gdcFlag),
                         &l_gdc_flag,
                         VFO_REPLACE);
            req++;
        }

        if (vfChkReq[1].status == E_NO_ERROR
            && l_locator_image_flag != -1) {

            VFF_SET_ARGS(vfReq[req],
                         SVN_PRC_GDC_LOCATOR_IMAGE,
                         SVT_PRC_GDC_LOCATOR_IMAGE,
                         sizeof(gdcMode.locatorImageFlag),
                         &l_locator_image_flag,
                         VFO_REPLACE);
            req++;
        }

        if (vfChkReq[2].status == E_NO_ERROR
            && l_mask_image_flag != -1) {

            VFF_SET_ARGS(vfReq[req],
                         SVN_PRC_GDC_LOCATOR_MASK,
                         SVT_PRC_GDC_LOCATOR_MASK,
                         sizeof(gdcMode.maskImageFlag),
                         &l_mask_image_flag,
                         VFO_REPLACE);
            req++;
        }

        if (vfChkReq[3].status == E_NO_ERROR
            && l_auto_gdc_flag != -1) {

            VFF_SET_ARGS(vfReq[req],
                         SVN_PRC_AUTO_GDC_ACTION,
                         SVT_PRC_AUTO_GDC_ACTION,
                         sizeof(gdcMode.autoGDCFlag),
                         &l_auto_gdc_flag,
                         VFO_REPLACE);
            req++;
        }

        /* V9.20 チャネル縮退対応 */
        if (vfChkReq[4].status == E_NO_ERROR
            && l_harmonization_flag != -1) {

            VFF_SET_ARGS(vfReq[req],
                         SVN_PLN_DEGENERACY_FLAG,
                         SVT_PLN_DEGENERACY_FLAG,
                         sizeof(degeneracyFlg),
                         &l_harmonization_flag,
                         VFO_REPLACE);
            req++;
        }
    }

    //+Patni-SS/2009Apr01/PSP1#VFLOCK

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T(""));

    l_status = m_vfdb_handle->PutFields(pelm, SVD_ST_PROTOCOL, vfReq, req, &num, &tm);
    return E_NO_ERROR;
}

//*****************************************************************************
//Method Name   : ConvertCHAR()
//Author        : PATNI/SG
//Purpose       : Convert CString object to char*
//*****************************************************************************
char* CPQMStudy::ConvertCHAR(
    CString f_input
) const		//Patni-Sribanta/2010Oct25/Added/IR-170
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::ConvertCHAR");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMStudy::ConvertCHAR"));

    const int l_inlen = f_input.GetLength();
    char* l_retptr = (char*) calloc(l_inlen + 1, sizeof(char));

    //+Patni-RUP/2009Aug25/Modified/cpp test corrections
    //for(int l_counter = 0;l_counter<l_inlen;l_counter++) {
    //    l_retptr[l_counter] = (char)f_input.GetAt(l_counter);
    //}
    //l_retptr[l_counter] = 0;

    if (l_retptr != NULL) {
        for (int l_counter = 0; l_counter < l_inlen; l_counter++) {
            l_retptr[l_counter] = (char)f_input.GetAt(l_counter);
        }

        l_retptr[l_counter] = 0;
    }

    //-Patni-RUP/2009Aug25/Modified/cpp test corrections
    return l_retptr;
}

//***************************Method Header*************************************
//Method Name    :ReadTotalSlicesForShimming()
//Author         :PATNI/LK
//Purpose        :Returns the total number of slices and if it fails to collect
//                    the information it will return 15
//*****************************************************************************
int CPQMStudy::ReadTotalSlicesForShimming(
    VfPathElem_t* f_vfpath_elem
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::ReadTotalSlicesForShimming");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of ReadTotalSlicesForShimming"));

    int                 n = 0;
    VfFieldSpec_t       l_reqflds[5] = {0};//Patni-MJC/2009Aug17/Modified/cpp test corrections
    int                 l_typeformatdepth = SVD_ST_PROTOCOL;
    int                 l_numdone = -1;
    VfPathElem_t*        l_processpathp = f_vfpath_elem;
    int                 l_status = E_ERROR;
    int                 l_total_slices = -1;

    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(l_reqflds, 0, sizeof(VfFieldSpec_t) * 5);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008

    VFF_SET_ARGS(l_reqflds[n], SVN_PLN_TOTAL_SLICES, SVT_PLN_TOTAL_SLICES,
                 sizeof(int), &l_total_slices, 0);
    n++;

    l_status = m_vfdb_handle->GetFields(l_processpathp, l_typeformatdepth, l_reqflds, n, &l_numdone);

    if (l_status != E_NO_ERROR || l_numdone != n) {
        l_total_slices = FIELDPROFILEIMAGES;
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed to read SVN_PLN_TOTAL_SLICES from shim study"));
    }

    CString l_tracemsg(_T(""));
    l_tracemsg.Format(_T("Total Slices Read from Study file --> %d"), l_total_slices);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_tracemsg);

    return l_total_slices;
}

//****************************Method Header************************************
//Method Name   :ShimPositionRead
//Author        :PATNI/
//Purpose       :
//*****************************************************************************
int CPQMStudy::ShimPositionRead(
    SShimStudy_t* f_shimstudy_struct,
    position_t* f_position_struct
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::ShimPositionRead");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of ShimPositionRead"));

    VfFieldSpec_t   l_reqflds[5] = {0};//Patni-MJC/2009Aug17/Modified/cpp test corrections
    int             l_typeformatdepth =  SVD_ST_PROTOCOL; // Protocol Node Data Fov & Total Slice Read ;
    SShimStudy_t*    l_studyp = f_shimstudy_struct;
    position_t*      l_posp = f_position_struct;

    VfPathElem_t* l_imagepathp = l_studyp->nodes;
    VfPathElem_t* l_processpathp = l_studyp->imageNodes;

    l_imagepathp[SVD_ST_SUBPROTOCOL - 1].index = 0;
    l_imagepathp[SVD_ST_SLICEGROUP - 1].index = 0;
    l_imagepathp[SVD_ST_SLICE - 1].index = 0;
    l_imagepathp[SVD_ST_IMAGE - 1].index = 0;

    l_processpathp[SVD_ST_PROCESSED - 1].index = 0;
    l_processpathp[SVD_ST_ECHO - 1].index = 0;
    l_processpathp[SVD_ST_POSITION - 1].index = 0;

    //    shimPrintf("%s : %d shimPositionRead() - image /1.%d,process:/1.%d/7.%d/8.%d \n",
    //      __FILE__, __LINE__,
    //          imagePathP[SVD_ST_PROTOCOL-1].index,
    //          processPathP[SVD_ST_PROTOCOL-1].index,
    //          processPathP[SVD_ST_PROCESSED-1].index,
    //          processPathP[SVD_ST_ECHO-1].index);
    // + Crash Modification HIMANSHU 20081227
    memset(l_reqflds, NULL, sizeof(VfFieldSpec_t) * 5);
    // - Crash Modification HIMANSHU 20081227
    int             n = 0;
    VFF_SET_ARGS(l_reqflds[n], SVN_PLN_FOV, SVT_PLN_FOV, sizeof(f32vec2_t), &l_posp->fovVec, 0);
    n++;

    VFF_SET_ARGS(l_reqflds[n], SVN_PLN_TOTAL_SLICES, SVT_PLN_TOTAL_SLICES, sizeof(int),
                 &l_posp->sliceNum, 0);
    n++;

    VFF_SET_ARGS(l_reqflds[n], SVN_PLN_SELECTED_SLICE_GAP, SVT_PLN_SELECTED_SLICE_GAP, sizeof(float),
                 &l_posp->sliceGap, 0);
    n++;

    int             l_numdone = -1;
    status_t l_status = m_vfdb_handle->GetFields(l_imagepathp, l_typeformatdepth, l_reqflds, n, &l_numdone);

    if (l_status != E_NO_ERROR || l_numdone != n) {
        //shimPrintf("%s : %d shimPositionRead() - FOV Read Error \n", __FILE__, __LINE__);
        return E_ERROR;
    }

    n = 0;
    l_typeformatdepth = SVD_ST_SLICEGROUP;
    memset(l_reqflds, NULL, sizeof(VfFieldSpec_t) * 5); // + Crash Modification HIMANSHU 20081227

    VFF_SET_ARGS(l_reqflds[n], SVN_PLN_OFFSET_VECTOR, SVT_PLN_OFFSET_VECTOR, sizeof(f32vec3_t),
                 &l_posp->offsetVec, 0);
    n++;

    VFF_SET_ARGS(l_reqflds[n], SVN_PLN_SLICE_ORI_VECTOR, SVT_PLN_SLICE_ORI_VECTOR, sizeof(f32vec3_t),
                 &l_posp->sliceVec, 0);
    n++;

    VFF_SET_ARGS(l_reqflds[n], SVN_PLN_THICKNESS, SVT_PLN_THICKNESS, sizeof(float),
                 &l_posp->thickness, 0);
    n++;

    VFF_SET_ARGS(l_reqflds[n], SVN_PLN_PHASE_ORI_VECTOR, SVT_PLN_PHASE_ORI_VECTOR, sizeof(f32vec3_t),
                 &l_posp->peVec, 0);
    n++;

    l_status = m_vfdb_handle->GetFields(l_imagepathp, l_typeformatdepth, l_reqflds, n, &l_numdone);

    if (l_status != E_NO_ERROR || l_numdone != n) {
        //shimPrintf("%s : %d shimPositionRead() - OFFSET & SLICE_ORI Read Error \n", __FILE__, __LINE__);
        // return E_ERROR;
    }

    memset(l_reqflds, NULL, sizeof(VfFieldSpec_t) * 5); // + Crash Modification HIMANSHU 20081227
    n = 0;
    l_typeformatdepth = SVD_ST_ECHO;
    VFF_SET_ARGS(l_reqflds[n], SVN_PRC_CEN_TO_CEN_DIST, SVT_PRC_CEN_TO_CEN_DIST, sizeof(float),
                 &l_posp->cen_to_cen_dist, 0);
    n++;

    l_status = m_vfdb_handle->GetFields(l_processpathp, l_typeformatdepth, l_reqflds, n, &l_numdone);

    if (l_status != E_NO_ERROR || l_numdone != n) {
        //shimPrintf("%s : %d shimPositionRead() - CEN_TO_CEN_DIST Read Error \n", __FILE__, __LINE__);
        // return E_ERROR;
    }

    float l_domain = (l_posp->sliceNum - 1) / 2.0f * l_posp->cen_to_cen_dist + (l_posp->thickness / 2.0f);

    switch (Direction(l_posp->sliceVec.x, l_posp->sliceVec.y, l_posp->sliceVec.z)) {

        case SHIMAXICAL:
            l_studyp->startPosi.z   = l_posp->offsetVec.z - l_domain;
            l_studyp->endPosi.z     = l_posp->offsetVec.z + l_domain;
            l_studyp->startPosi.x   = l_posp->offsetVec.x - l_posp->fovVec.x / 2.0f;
            l_studyp->endPosi.x     = l_posp->offsetVec.x + l_posp->fovVec.x / 2.0f;
            l_studyp->startPosi.y   = l_posp->offsetVec.y - l_posp->fovVec.y / 2.0f;
            l_studyp->endPosi.y     = l_posp->offsetVec.y + l_posp->fovVec.y / 2.0f;
            break;

        case SHIMSAGITAL:
            l_studyp->startPosi.x   = l_posp->offsetVec.x - l_domain;
            l_studyp->endPosi.x     = l_posp->offsetVec.x + l_domain;
            l_studyp->startPosi.z   = l_posp->offsetVec.z - l_posp->fovVec.y / 2.0f;
            l_studyp->endPosi.z     = l_posp->offsetVec.z + l_posp->fovVec.y / 2.0f;
            l_studyp->startPosi.y   = l_posp->offsetVec.y - l_posp->fovVec.x / 2.0f;
            l_studyp->endPosi.y     = l_posp->offsetVec.y + l_posp->fovVec.x / 2.0f;
            break;

        case SHIMCORONAL:
            l_studyp->startPosi.y   = l_posp->offsetVec.y - l_domain;
            l_studyp->endPosi.y     = l_posp->offsetVec.y + l_domain;
            l_studyp->startPosi.z   = l_posp->offsetVec.z - l_posp->fovVec.y / 2.0f;
            l_studyp->endPosi.z     = l_posp->offsetVec.z + l_posp->fovVec.y / 2.0f;
            l_studyp->startPosi.x   = l_posp->offsetVec.x - l_posp->fovVec.x / 2.0f;
            l_studyp->endPosi.x     = l_posp->offsetVec.x + l_posp->fovVec.x / 2.0f;
            break;

        default:
            return E_SHIM_SLICE_ORI_ERROR;
    }

    //    shimPrintf("\t offsetVec = %f %f %f \n", posP->offsetVec.x, posP->offsetVec.y, posP->offsetVec.z);
    //    shimPrintf("\t sliceVec  = %f %f %f \n", posP->sliceVec.x,  posP->sliceVec.y,  posP->sliceVec.z);
    //    shimPrintf("\t fovVec    = %f %f \n",    posP->fovVec.x,    posP->fovVec.y);
    //    shimPrintf("\t sliceNum  = %d \n",       posP->sliceNum);
    //    shimPrintf("\t thicknes  = %f \n",       posP->thickness);
    //    shimPrintf("\t cen_to_cen_dist = %f \n", posP->cen_to_cen_dist);
    //    shimPrintf("\t start - %f %f %f \n",     studyP->startPosi.x, studyP->startPosi.y, studyP->startPosi.z);
    //    shimPrintf("\t end   - %f %f %f \n",     studyP->endPosi.x,   studyP->endPosi.y,   studyP->endPosi.z);
    return E_NO_ERROR;
}

//****************************Method Header************************************
//Method Name   :ShimCheckAMB
//Author        :PATNI/LK
//Purpose       :
//*****************************************************************************
BOOL CPQMStudy::ShimCheckAMB(
    VfPathElem_t* f_vfpath_elem
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::ShimCheckAMB");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of ShimCheckAMB"));

    VfFieldSpec_t       l_reqflds[5] = {0};//Patni-MJC/2009Aug17/Modified/cpp test corrections
    int                 l_amb_flg = -1;
    VfPathElem_t*        l_processpathp = f_vfpath_elem;

    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(l_reqflds, 0, sizeof(VfFieldSpec_t) * 5);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    int                 n = 0;
    VFF_SET_ARGS(l_reqflds[n], SVN_PLN_AMB_MODE_FLG, SVT_PLN_AMB_MODE_FLG,
                 sizeof(int), &l_amb_flg, 0);
    n++;

    int                 l_typeformatdepth = SVD_ST_PROTOCOL;
    int                 l_numdone = -1;
    status_t l_status = m_vfdb_handle->GetFields(l_processpathp, l_typeformatdepth, l_reqflds, n, &l_numdone);

    if (l_status != E_NO_ERROR || l_numdone != n) {
        l_amb_flg = VFC_AMB_MODE_NONE;
    }

    if (l_amb_flg == VFC_AMB_MODE_SLAVE) {
        return TRUE;

    } else {
        return FALSE;
    }
}

//****************************Method Header************************************
//Method Name   :ShimDataKindCheck
//Author        :PATNI/
//Purpose       :
//*****************************************************************************
BOOL CPQMStudy::ShimDataKindCheck(
    VfPathElem_t* f_processpathp,
    const int f_processed_nodes
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::ShimDataKindCheck");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of ShimDataKindCheck"));

    VfFieldSpec_t       l_reqflds[5] = {0};//Patni-MJC/2009Aug17/Modified/cpp test corrections
    int                 l_datakind = -1;
    VfPathElem_t*        l_processpathp = f_processpathp;

    l_processpathp [SVD_ST_PROCESSED - 1].index = f_processed_nodes;
    l_processpathp [SVD_ST_ECHO - 1].index = 0;
    l_processpathp [SVD_ST_POSITION - 1].index = 0;

    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(l_reqflds, 0, sizeof(VfFieldSpec_t) * 5);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    int             n = 0;
    VFF_SET_ARGS(l_reqflds[n], SVN_PRC_DATA_KIND, SVT_PRC_DATA_KIND,
                 sizeof(int), &l_datakind, 0);
    n++;
    int                 l_typeformatdepth  = SVD_ST_PROCESSED;
    int                 l_numdone = -1;
    status_t l_status = m_vfdb_handle->GetFields(l_processpathp , l_typeformatdepth, l_reqflds, n, &l_numdone);

    if (l_status  != E_NO_ERROR || l_numdone != n) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Data Kind Read Error"));
        return FALSE;
    }

    if (l_datakind != VFC_PROC_SHIM_CALC) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("data Not CALC"));
        return FALSE;
    }

    return TRUE;
}

//****************************Method Header************************************
//Method Name   :ShimReqPutOffset
//Author        :PATNI/
//Purpose       :
//*****************************************************************************
BOOL CPQMStudy::ShimReqPutOffset(
    int f_typeformat_depth,
    VfPathElem_t* f_vfpath_element,
    const int f_adjust_processed
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::ShimReqPutOffset");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of ShimReqPutOffset"));

    VfFieldSpec_t   l_reqflds[5] = {0};//Patni-MJC/2009Aug17/Modified/cpp test corrections
    int             n = 0;
    int             l_shim_coil[SHIM_COIL] = {0};
    int             l_grad_coil[GRAD_CHANNEL] = {0};

    VfPathElem_t* l_processpathp = f_vfpath_element;
    l_processpathp[SVD_ST_PROCESSED - 1].index = f_adjust_processed;

    f_typeformat_depth = SVD_ST_PROCESSED;

    l_processpathp[SVD_ST_ECHO - 1].index = 0;
    l_processpathp[SVD_ST_POSITION - 1].index = 0;

    //    shimPrintf("%s : %d shimPutOffset() - GetOffsetPath = /1.%d/7.%d \n", __FILE__, __LINE__,
    //                processPathP[SVD_ST_PROTOCOL-1].index,
    //                processPathP[SVD_ST_PROCESSED-1].index);
    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(l_reqflds, 0, sizeof(VfFieldSpec_t) * 5);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008

    VFF_SET_ARGS(l_reqflds[n], SVN_SEQ_SHIM_OFFSET, SVT_SEQ_SHIM_OFFSET,
                 sizeof(int)*SHIM_COIL, l_shim_coil, 0);
    n++;
    VFF_SET_ARGS(l_reqflds[n], SVN_SEQ_SHIM_GRAD_OFFSET, SVT_SEQ_SHIM_GRAD_OFFSET,
                 sizeof(int)*GRAD_CHANNEL, l_grad_coil, 0);
    n++;
    int             l_numdone = 0;
    status_t l_status = m_vfdb_handle->GetFields(l_processpathp, f_typeformat_depth, l_reqflds, n, &l_numdone);

    if (l_status != E_NO_ERROR || l_numdone != n) {
        //shimPrintf("%s : %d shimPutOffset() - CPQMStudy::GetFields Error \n", __FILE__, __LINE__);
        return FALSE;
    }

    //    {
    //  for(int i = 0; i< SHIM_COIL;i++)
    //            shimPrintf("\t shimCoil[%d] = %d \n", i, shim_coil[i]);
    //  for(i = 0; i< GRAD_CHANNEL;i++)
    //            shimPrintf("\t gradCoil[%d] = %d \n", i, grad_coil[i]);
    //    }

    n = 0;
    f_typeformat_depth = SVD_ST_STUDY;
    VFF_SET_ARGS(l_reqflds[n], SVN_SEQ_SHIM_OFFSET, SVT_SEQ_SHIM_OFFSET,
                 sizeof(int)*SHIM_COIL, l_shim_coil, VFO_REPLACE);
    n++;
    VFF_SET_ARGS(l_reqflds[n], SVN_SEQ_SHIM_GRAD_OFFSET, SVT_SEQ_SHIM_GRAD_OFFSET,
                 sizeof(int)*GRAD_CHANNEL, l_grad_coil, VFO_REPLACE);
    n++;

    //+Patni-SS/2009Apr01/PSP1#VFLOCK
    CString l_log_msg = _T("");
    l_log_msg.Format(_T("SVN_SEQ_SHIM_GRAD_OFFSET =%d,SVN_SEQ_SHIM_OFFSET = %d "), l_shim_coil, l_grad_coil);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_msg);
    //-Patni-SS/2009Apr01/PSP1#VFLOCK
    l_status = m_vfdb_handle->PutFields(NULL, f_typeformat_depth, l_reqflds, n, &l_numdone, NULL);

    if (l_status != E_NO_ERROR || l_numdone != n) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
        //shimPrintf("%s : %d shimPutOffset() - CPQMStudy::PutFields Error \n", __FILE__, __LINE__);
        return FALSE;
    }

    return TRUE;
}

//****************************Method Header************************************
//Method Name   :ShimGetZ2OffSet
//Author        :PATNI/
//Purpose       :
//*****************************************************************************
BOOL CPQMStudy::ShimGetZ2OffSet(
    int* f_z2_offset
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::ShimGetZ2OffSet");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of ShimGetZ2OffSet"));

    VfFieldSpec_t   l_reqflds[5] = {0}; //Patni-MJC/2009Aug17/Modified/cpp test corrections
    int             n = 0;
    int             l_shim_coil[SHIM_COIL] = {0};

    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(l_reqflds, 0, sizeof(VfFieldSpec_t) * 5);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008

    VFF_SET_ARGS(l_reqflds[n], SVN_SEQ_SHIM_OFFSET, SVT_SEQ_SHIM_OFFSET,
                 sizeof(int)*SHIM_COIL, l_shim_coil, 0);
    n++;
    int             l_numdone = 0;
    int             l_typeformatdepth = SVD_ST_STUDY;
    status_t l_status = m_vfdb_handle->GetFields(NULL, l_typeformatdepth, l_reqflds, n, &l_numdone);

    if (l_status != E_NO_ERROR || l_numdone != n) {
        //shimPrintf("%s : %d shimGetZ2() - Result Read Error \n", __FILE__, __LINE__);
        return FALSE;
    }

    if (f_z2_offset) {      //Patni-MJC/2009Aug17/Modified/cpp test corrections
        *f_z2_offset = l_shim_coil[CHANNEL_Z2];

    } else {                //Patni-MJC/2009Aug17/Modified/cpp test corrections
        return FALSE;       //Patni-MJC/2009Aug17/Modified/cpp test corrections
    }

    return TRUE;
}

//****************************Method Header************************************
//Method Name   :ShimReqPutFsyVal
//Author        :PATNI/
//Purpose       :
//*****************************************************************************
BOOL CPQMStudy::ShimReqPutFsyVal(
    VfPathElem_t* f_vfpath_element,
    int f_typeformatdepth,
    const int f_adjust_processed
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::ShimReqPutFsyVal");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of ShimReqPutFsyVal"));

    VfFieldSpec_t   l_reqflds[5] = {0};//Patni-MJC/2009Aug17/Modified/cpp test corrections
    int             n = 0;
    double          l_fsy_value = 0.0;

    VfPathElem_t* l_processpathp = f_vfpath_element;

    l_processpathp[SVD_ST_PROCESSED - 1].index = f_adjust_processed;

    f_typeformatdepth = SVD_ST_PROCESSED;

    l_processpathp[SVD_ST_ECHO - 1].index = 0;
    l_processpathp[SVD_ST_POSITION - 1].index = 0;

    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(l_reqflds, 0, sizeof(VfFieldSpec_t) * 5);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008

    VFF_SET_ARGS(l_reqflds[n], SVN_EXE_SHIM_FSY_VALUE, SVT_EXE_SHIM_FSY_VALUE,
                 sizeof(double), &l_fsy_value, 0);
    n++;

    //    shimPrintf("%s : %d shimPutFsyVal() - GetFsytPath = /1.%d/7.%d \n", __FILE__, __LINE__,
    //      processPathP[SVD_ST_PROTOCOL-1].index,
    //      processPathP[SVD_ST_PROCESSED-1].index);
    int             l_numdone = -1;
    status_t l_status = m_vfdb_handle->GetFields(l_processpathp, f_typeformatdepth, l_reqflds, n, &l_numdone);

    if (l_status != E_NO_ERROR || l_numdone != n) {
        //shimPrintf("%s : %d shimPutFsyVal() - fsy_value CPQMStudy::GetFields Error \n", __FILE__, __LINE__);
        return FALSE;
    }

    //shimPrintf("%s : %d shimPutFsyVal() - B0 Default: %f \n", __FILE__, __LINE__, fsy_value);

    n = 0;
    f_typeformatdepth = SVD_ST_STUDY;

    VFF_SET_ARGS(l_reqflds[n], SVN_EXE_SHIM_FSY_VALUE, SVT_EXE_SHIM_FSY_VALUE,
                 sizeof(double), &l_fsy_value, VFO_REPLACE);
    n++;
    //+Patni-SS/2009Apr01/PSP1#VFLOCK
    CString l_log_msg = _T("");
    l_log_msg.Format(_T("SVN_EXE_SHIM_FSY_VALUE =%d "), l_fsy_value);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_msg);
    //-Patni-SS/2009Apr01/PSP1#VFLOCK
    l_status = m_vfdb_handle->PutFields(NULL, f_typeformatdepth, l_reqflds, n, &l_numdone, NULL);

    if (l_status != E_NO_ERROR || l_numdone != n) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
        //shimPrintf("%s : %d shimPutFsyVal() - fsy_value CPQMStudy::PutFields Error \n", __FILE__, __LINE__);
        return FALSE;
    }

    return TRUE;

}

//****************************Method Header************************************
//Method Name   : PutSaturnCoil2()
//Author        : PATNI\MRP
//Purpose       :
//*****************************************************************************
status_t CPQMStudy::PutSaturnCoil2(int prot,
                                   int              numSection, /* number of Section */
                                   VftSectionRecvrCoil_t*   RcvCoil,   /* Recver Coil */
                                   VftSectionSignal_t*      Signal,   /* Signal */
                                   char*    CoilName  /* Coil label name  */)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::PutSaturnCoil2");
    status_t        sts  = E_NO_ERROR;
    BITFLD_DECL(mask, K_ALL_PR);

    if (numSection) {
        int mode = TRUE ;
        int n = 0;
        VfFieldSpec_t   vfReq[5] = {0};
        memset(vfReq, 0, sizeof(VfFieldSpec_t) * 5);
        VFF_SET_ARGS(vfReq[n], SVN_PLN_NUM_SECTION_RECVR_COIL2, SVT_PLN_NUM_SECTION_RECVR_COIL2,
                     sizeof(numSection), &numSection, VFO_REPLACE);
        n++;
        int s = numSection * sizeof(VftSectionRecvrCoil_t) ;
        VFF_SET_ARGS(vfReq[n], SVN_PLN_SECTION_RECVR_COIL2, SVT_PLN_SECTION_RECVR_COIL2,
                     s, RcvCoil, VFO_REPLACE);
        n++;
        s = numSection * sizeof(VftSectionSignal_t) ;
        VFF_SET_ARGS(vfReq[n], SVN_PLN_SECTION_RECVR_COIL_SIGNAL2, SVT_PLN_SECTION_RECVR_COIL_SIGNAL2,
                     s, Signal, VFO_REPLACE);
        n++;

        //sts = VfAppPutFields(m_vf_handle,pelm,SVD_ST_PROTOCOL,vfReq,n,&num,&tm);

        //+Patni-SS/2009Apr01/PSP1#VFLOCK
        CString l_log_msg = _T("");
        l_log_msg.Format(_T("SVN_PLN_NUM_SECTION_RECVR_COIL2 =%d ,SVN_PLN_SECTION_RECVR_COIL2 = %d, SVN_PLN_SECTION_RECVR_COIL_SIGNAL2 = %d"), numSection, RcvCoil, Signal);

        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_log_msg);

        time_t          tm = 0;
        int num = 0;

        VfPathElem_t    pelm[2] = {0};
        memset(pelm, 0, sizeof(VfPathElem_t) * 2);          //Patni-MJC/2009Aug17/Modified/cpp test corrections
        pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
        pelm[SVD_ST_PROTOCOL - 1].index = prot;

        sts = m_vfdb_handle->PutFields(pelm, SVD_ST_PROTOCOL, vfReq, n, &num, &tm);

        if (sts != E_NO_ERROR || n != num) {    /* エラー処理は？ */
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
            sts = E_ERROR ;
        }

        /* ここでコイル切り替え撮像ModeをTRUE、コイル名テーブルを登録する */
        SetProtValues(m_vfdb_handle, prot, mask,
                      K_PR_CHANGED_RECEIVE_COIL,    &mode,      sizeof(mode),
                      K_PR_RECVR_COILNAME_TBL,  CoilName, (strlen(CoilName) + 1),
                      NULL) ;
    }

    return sts ;
}

//+Patni-Manishkumar/2Nov09/ADDED/IR88/Chk acq. status in varfield
//************************************Method Header************************************
// Method Name  : IsMatchAcqStatusOfVarField
// Author       : PATNI/ Manishkumar
// Purpose      : Check the acqstatus from internal memory and varfield
//***********************************************************************************
bool CPQMStudy::IsMatchAcqStatusOfVarField(ProtocoInfo* f_protocol_info)
{
    int l_acqstatus = f_protocol_info->acq_status;
    int acqstatusofvar =  -1;
    BITFLD_DECL(mask, K_ALL_PR) = {0};
    memset(mask, 0, sizeof(mask));

    GetProtocolValues(m_vfdb_handle,
                      f_protocol_info->acq_order,
                      mask,
                      K_PR_ACQ_STATUS,
                      &acqstatusofvar,
                      NULL,
                      NULL);

    if (l_acqstatus != acqstatusofvar) {
        return false;
    }

    return true;
}
//-Patni-Manishkumar/2Nov09/ADDED/IR88/Chk acq. status in varfield

//****************************Method Header************************************
//Method Name   :LoadStudy()
//Author        :PATNI/MRP
//Purpose       :This method Loads the instance of CStudy and reads the information
//               related to the series in the respective data members of the Series
//*****************************************************************************
//+PATNI-Sribanta/2010Nov09/Modified/IR-170
BOOL CPQMStudy::LoadStudy(
    const CString& f_study_name,
    const int& f_mode,
    study_id_t& f_study_id_t,
    bool& f_same_study
)
//-PATNI-Sribanta/2010Nov09/Modified/IR-170
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::LoadStudy");
    //+Patni-AMT/2009Apr20/Modified/PSP1#001 Code Review
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, f_study_name);

    if (f_study_name.IsEmpty()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Study Name is blank"));
        return PQM_ERROR;
    }


    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, f_study_name);

    m_study_name = f_study_name;
    //-Patni-PJS/2009Nov3/Added/MSA0248-00140


    ClearProtocolVector();

    if (!ReadPatientInfoFromVarField(f_study_name)) {


        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                  _T("Failed to read Patient Information from Varfield"));
        return PQM_ERROR;
    }

    m_pqm_ptr->GetSARManager()->SendSessionStartMsgToSARMgr();


    //*************************************************************************


    bool            l_conflict = false;

    if (!CPqmShimSharedData::GetInstance()->ShimRegistStudy(f_mode, &m_study_id,
            &f_study_id_t, &l_conflict, &f_same_study)) {


        return PQM_ERROR;

    } else if (l_conflict) {

        CPQMLogMgr::GetInstance()->DisplayScanError(
            IDS_ERR_SAME_STUDY, _T("IDS_ERR_SAME_STUDY"), SCAN_ERR_CRUCIAL, L"PQMStudy.cpp");
        return PQM_ERROR;
    }


    return PQM_ERROR_NORMAL;
}
//-Patni-Sribanta/2010Nov18/Modified/REDMINE-772
//+Patni-Sribanta/2010Nov18/Added/REDMINE-772
//****************************Method Header************************************
//Method Name   :FillProtocolVector()
//Author        :PATNI/HAR-Sribanta
//Purpose       :
//*****************************************************************************
BOOL CPQMStudy::FillProtocolVector(
    const CString& f_pas_path
)
{
    CString FUNC_TRACE(_T("CPQMStudy::FillProtocolVector"));
    PQM_TRACE(USER_FUNC_MARKER, FUNC_TRACE, FUNC_TRACE);

    //+Patni-DKH/2010Sep24/Added/TMSC-REDMINE-756
    DBSAIndex_t l_seriesindex = 0u ;

    WORD* l_wacqorder = new WORD[MAX_BUFFER];

    if (NULL == l_wacqorder) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_TRACE,
                  _T("l_wacqorder pointer is NULL"));
        return FALSE ;
    }

    memset(l_wacqorder, 0, sizeof(WORD)*MAX_BUFFER);
    //-Patni-DKH/2010Sep24/Added/TMSC-REDMINE-756


    ClibVfToUiAdapter vuiAdapter(m_vfdb_handle);
    int l_branches		= -1;
    BOOL l_message_display_flag = DisplayProtocolMismatchMessage(!CheckProtocolsInDB(l_branches));

    //+Patni-Sribanta/2010Oct25/Modified/IR-170
    if (0 == l_branches && l_message_display_flag) {
        if (!LoadPasSequences(f_pas_path)) {

            m_pqm_ptr->WriteToUI(PQM_MSG_INVOKE_MEMORY);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_TRACE,
                      _T("Message sent to invoke Memory Application"));
        }
    }

    //-Patni-Sribanta/2010Oct25/Modified/IR-170

    else {
        int protocol_index = 0;
        CPASCOMSequence*    l_seq_param     = NULL;
        CPASParamStore*     l_objParamStore = NULL ;
        SequenceList        l_sequence_list;
        int index = 0;

        for (int i = 0; i < l_branches; i++) {

            protocol_index = vuiAdapter.VuiProtNum(index);
            CString l_prot_index(_T(""));
            l_prot_index.Format(_T("[Protocol Index = %d], [VF Handle = 0x%x]"),
                                protocol_index, m_vfdb_handle);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_TRACE, l_prot_index);

            if (protocol_index >= 0) {
                CPqmProtocol* l_pqm_protocol = new CPqmProtocol();
                m_pqm_ptr->GetSAR()->ResetSARInfo(l_pqm_protocol);
                l_pqm_protocol->SetProtocol(protocol_index);
                l_pqm_protocol->SetSystemChannels(m_pqm_ptr->GetNumChannels());
                if_error(SetProtocolObject(m_vfdb_handle, l_pqm_protocol)) {
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_TRACE,
                              _T("SetProtocolObject ERROR"));
                    ClearProtocolVector();
                    ClearSequenceList(l_sequence_list);
                    return FALSE;
                }

                m_protocol_vec.push_back(l_pqm_protocol);
                m_prot_count++;

                l_pqm_protocol->SetPosition(m_protocol_vec.size());
                l_seq_param		= new CPASCOMSequence();
                l_objParamStore = new CPASParamStore();

                CString l_series_loid = l_pqm_protocol->GetSeriesLOID();
                PQM_TRACE(USER_FUNC_MARKER, FUNC_TRACE, l_series_loid);
                BOOL l_isvalid = m_pqm_ptr->GetStudyManagerPtr()->
                                 CheckSeriesInMPlusDB(l_series_loid, l_seriesindex);

                if ((l_series_loid.IsEmpty() || (FALSE == l_isvalid)) &&
                    (C_PR_STATUS_WAIT == l_pqm_protocol->GetAcqStatus())) {

                    //-Patni-DKH/2010Sep24/Added/TMSC-REDMINE-756
                    if (C_PR_STATUS_WAIT != l_pqm_protocol->GetAcqStatus()) {
                        ClearProtocolVector();
                        ClearSequenceList(l_sequence_list);

                        CString l_err_msg(_T(""));
                        l_err_msg.Format(_T(
                                             "Series LOID is not found in MPlus DB or Series Load is NULL in VarfieldDB. Series LOID = "));
                        PQM_TRACE(USER_FUNC_MARKER, FUNC_TRACE,
                                  l_err_msg + l_series_loid);

                        m_pqm_ptr->WriteToUI(PQM_MSG_MDB_VDB_NOT_SYNC);
                        DEL_PTR_ARY(l_wacqorder);
                        return FALSE;
                    }

                    //-Patni-DKH/2010Sep24/Added/TMSC-REDMINE-756

                    //+Patni-Sribanta-HAR/2010Sep21/Modified/TMSC-REDMINE-756
                    if (l_message_display_flag &&
                        (!DisplayProtocolMismatchMessage(l_message_display_flag))
                       ) {
                        l_message_display_flag = FALSE;
                    }

                    CString l_err_msg(_T(""));
                    l_err_msg.Format(_T(
                                         "Series LOID is not found in MPlus DB or Series Load is NULL in VarfieldDB. Series LOID = "));
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_TRACE, l_err_msg + l_series_loid);

                    if (E_NO_ERROR != vuiAdapter.VuiDeleteProtocol(l_pqm_protocol->GetProtocol())) {
                        PQM_TRACE(USER_FUNC_MARKER, FUNC_TRACE, _T("VuiDeleteProtocol() failed."));
                        ClearProtocolVector();
                        ClearSequenceList(l_sequence_list);
                        m_pqm_ptr->WriteToUI(PQM_MSG_MDB_VDB_NOT_SYNC);
                        DEL_PTR_ARY(l_wacqorder);
                        return FALSE;
                    }

                    m_vfdb_handle->ReOpen();

                    CPqmProtocol* l_protocol = m_protocol_vec.back();
                    DEL_PTR(l_protocol);

                    m_protocol_vec.erase(m_protocol_vec.end() - 1);
                    m_prot_count--;

                    //Patni-DKH/2010Sep30/Added/TMSC-REDMINE-756
                    //DEL_PTR(l_pqm_protocol);

                    //-Patni-Sribanta-HAR/2010Sep21/Modified/TMSC-REDMINE-756

                } else {
                    //-Patni-Sribanta-HAR/2010Sep21/Modified/TMSC-REDMINE-756
                    l_seq_param->SetSequenceID(l_series_loid);
                    FillObjectParam(protocol_index, *l_pqm_protocol, *l_objParamStore);

                    //+Patni-Hemant+ARD/2009Oct30/Commented/IR 85 Auto Transfer
                    if (m_pqm_ptr->GetStudyManagerPtr()) {
                        if (!m_pqm_ptr->GetStudyManagerPtr()->UpdateAutoTransferValueInMPlusDB(l_pqm_protocol)) {

                            PQM_TRACE(USER_FUNC_MARKER, FUNC_TRACE,
                                      _T("UpdateAutoTransferValueInMPlusDB() : \
							Failed to update auto transfer tag in MPLUSDB"));
                        }
                    }

                    //-Patni-Hemant+ARD/2009Oct30/Commented/IR 85 Auto Transfer

                    l_seq_param->SetSequenceParam(l_objParamStore);
                    l_sequence_list.AddTail(l_seq_param);
                    DEL_PTR(l_objParamStore);
                    //Patni-DKH/2010Sep24/Added/TMSC-REDMINE-756
                    l_wacqorder[index] = l_seriesindex ;
                    //Patni-Sribanta-HAR/2010Sep21/Modified/TMSC-REDMINE-756
                    index++;
                }//Patni-Sribanta-HAR/2010Sep21/Modified/TMSC-REDMINE-756
            }
        }

        //+Patni-DKH/2010Sep24/Modified/TMSC-REDMINE-756
        if (!l_message_display_flag) {
            m_pqm_ptr->GetStudyManagerPtr()->UpdateAcqOrderInLocalDB(l_wacqorder, index);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_TRACE,
                      _T("Acqorder Updated in Local DB"));
        }

        DEL_PTR_ARY(l_wacqorder);
        //-Patni-DKH/2010Sep24/Modified/TMSC-REDMINE-756

        //+Patni-Sribanta-HAR/2010Sep21/Modified/TMSC-REDMINE-756
        if (!CheckProtocolsInDB(l_branches)) {
            ClearSequenceList(l_sequence_list);
            ClearProtocolVector();
            m_pqm_ptr->WriteToUI(PQM_MSG_MDB_VDB_NOT_SYNC);
            return FALSE;
        }

        //-Patni-Sribanta-HAR/2010Sep21/Modified/TMSC-REDMINE-756

        m_pqm_ptr->GetMasterSlavePtr()->SetMasterProtocolForAllSlaves();

        SetNewRflSARForProtocol(l_sequence_list);
        CopyBackCouchInfoForlastDoneProtocol();

        if (K_SCHEDULING == m_pqm_ptr->GetMode()) {
            m_pqm_ptr->GetSendScheduleSeqList().RemoveAll();
            m_pqm_ptr->GetSendScheduleSeqList().AddTail(&l_sequence_list);

        } else {
            m_pqm_ptr->GetSendAcquireSeqList().RemoveAll();
            m_pqm_ptr->GetSendAcquireSeqList().AddTail(&l_sequence_list);
        }

        if (m_pqm_ptr->GetSARManager()) {
            m_pqm_ptr->GetSARManager()->SendRTSARInfoForLastDoneProtocol();
        }

        m_pqm_ptr->WriteToUI(PQM_MSG_SERIES_LOAD);

    }

    //+Patni-Manish/2009Sept02/VFLOG/Added
    try {
        WriteStudyLOIDinVarfield();

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_TRACE,
                  _T("Crashed on writing the study LOID in varfeild"));
    }

    //-Patni-Manish/2009Sept02/VFLOG/Added
    return TRUE;
}

//***************************Method Header*************************************
//Method Name    : WriteToStudy()
//Author         : PATNI/SG
//Purpose        : Gets the Minimum and Maximum Threshhold value from the study file
//*****************************************************************************
status_t CPQMStudy::WriteToStudy(
    CString f_sequence_path,
    int* f_acqorder,
    const int f_seq_count
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::WriteToStudy");

    if (!m_vfdb_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_vfdb_handle is NULL"));
        return E_ERROR;
    }

    char* l_memory_path = ConvertCHAR(f_sequence_path);

    BITFLD_DECL(mask, MAX_NODES);
    int l_tmp_branch = MAX_NODES;
    ClibVfToUiAdapter vuiAdapter(m_vfdb_handle);
    vuiAdapter.VuiGetAllProt(mask, &l_tmp_branch);




    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of WriteToStudy"));

    //+Patni-MJC/2009Aug27/Modified/cpp test corrections
    if (m_pqm_ptr == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug27/Modified/cpp test corrections
    m_mode = m_pqm_ptr->GetMode();

    //Patni-DKH/2011Mar04/Added/TMSC-REDMINE-1486
    CScopedLock l_prot_loc(&m_protocol_vec_mutex);

    CPQMChildstudy* l_childstudy = new CPQMChildstudy(this);
    int l_status = E_ERROR;
    void* data = (void*) l_childstudy;
    m_vfdb_handle->ReOpen();

    try {
        //+Patni-SS/2009Apr01/PSP1#VFLOCK
        CString l_log_msg = _T("");
        //-20091006 Tanoue-san
        l_log_msg.Format(_T("m_vfdb_handle = 0x%x,CPQMStudy::GetProtocolFromStudy - %d"),
                         m_vfdb_handle, CPQMStudy::GetProtocolFromStudy);
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_log_msg);

        //-Patni-SS/2009Apr01/PSP1#VFLOCK
        //+Patni-Manish/2009Sept02/VFLOG/Added
        try {
            l_status = vuiAdapter.VuiCopyProtocolsFromFile(l_memory_path,
                       f_acqorder,
                       f_seq_count,
                       0,
                       CPQMStudy::GetProtocolFromStudy,
                       data);

        } catch (...) {
            CString l_print_string = _T("");
            l_print_string.Format(_T("VuiCopyProtocolsFromFile Crashed with handle=%x/f_seq_count=%d"),
                                  m_vfdb_handle, f_seq_count);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_print_string);
        }

        //-Patni-Manish/2009Sept02/VFLOG/Added

        //-Patni-RSG/2009Aug07/ACE Performence Code

        DEL_PTR(l_childstudy); //TMSC-Tanoue/2010Feb10/Memory Leaks

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception in PQM"));
    }

    m_vfdb_handle->ReOpen();
    return l_status;
}

//***************************Method Header*************************************
//Method Name    :ReadShimValuesFromFile()
//Author         :PATNI/
//Purpose        :
//*****************************************************************************
BOOL CPQMStudy::ReadShimValuesFromFile(
    int*    f_scannumber,
    float*  f_couchpos,
    float*  f_homepos,
    float*  f_txtunepos,
    float*  f_rflpos,
    int*    f_shimstate,
    int*    f_shimsfyflag
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::ReadShimValuesFromFile");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of ReadShimValuesFromFile"));

    //Patni-SS/20091013/Modified/MSA0248-00056
    char* l_env = getenv("GP_SHIMDT_DIR");

    //+Patni-PJS/2009Sep16/Modified/Internal Code review comment
    if (l_env == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Failed to get value of GP_SHIMDT_DIR environment variable"));
        return FALSE;
    }

    //-Patni-PJS/2009Sep16/Modified/Internal Code review comment

    char l_shimdatapath[256] = "";
    strcpy(l_shimdatapath, l_env);
    strcat(l_shimdatapath, "/ShimData.txt");

    FILE* l_shimfileptr = fopen(l_shimdatapath, "r");

    if (l_shimfileptr) {
        wchar_t l_filebuf[512] = _T("");
        fgetws(l_filebuf, 512, l_shimfileptr);

        for (wchar_t* l_token = wcstok(l_filebuf, _T("\n")); (l_token != NULL && !feof(l_shimfileptr)); l_token = wcstok(l_filebuf, _T("\n"))) {

            fgetws(l_filebuf, 512, l_shimfileptr) ;


            if ((wcsstr(l_token, _T("Scan Number")) != NULL)) {
                if (swscanf(l_token, _T("Scan Number=%d"), f_scannumber) == 0) {
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("unable to read scan number from ShimData file"));

                    return FALSE;
                }

            } else if ((wcsstr(l_token, _T("Couch Position")) != NULL)) {
                if (swscanf(l_token, _T("Couch Position=%f"), f_couchpos) == 0) {
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("unable to read couch position from ShimData file"));
                    return FALSE;
                }

            } else if ((wcsstr(l_token, _T("Home Position")) != NULL)) {
                if (swscanf(l_token, _T("Home Position=%f"), f_homepos) == 0) {
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("unable to read Home position from ShimData file"));

                    return FALSE;
                }

            } else if ((wcsstr(l_token, _T("TxTune Position")) != NULL)) {
                if (swscanf(l_token, _T("TxTune Position=%f"), f_txtunepos) == 0) {
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("unable to read TxTune Position from ShimData file"));

                    return FALSE;
                }

            } else if ((wcsstr(l_token, _T("RflPosition")) != NULL)) {
                if (swscanf(l_token, _T("RflPosition=%f"), f_rflpos) == 0) {
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("unable to read Rfl Position from ShimData file"));

                    return FALSE;
                }

            } else if ((wcsstr(l_token, _T("Shim State")) != NULL)) {
                if (swscanf(l_token, _T("Shim State=%d"), f_shimstate) == 0) {
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("unable to read Shim Statefrom ShimData file"));

                    return FALSE;
                }

            } else if ((wcsstr(l_token, _T("Shim Flag")) != NULL)) {
                if (swscanf(l_token, _T("Shim Flag=%d"), f_shimsfyflag) == 0) {
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("unable to read Shim flag from ShimData file"));
                    return FALSE;
                }
            }

        }

        return TRUE;

    } else {
        CString str(l_shimdatapath);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str);
        return FALSE;
    }

}

//*****************************************************************************
//Method Name   : ReadMaxValueFromSubProtocol
//Author        : Patni/Sribanta
//Purpose       : This function is used to read the maximum value for a tag
//				  from varfield.
//*****************************************************************************
int CPQMStudy::ReadMaxValueFromSubProtocol(
    const int f_protocol,
    const int f_tag_name,
    f32vec3_t& f_curr_value
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::ReadMaxValueFromSubProtocol");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMStudy::ReadMaxValueFromSubProtocol"));

    f32vec3_t l_max_value, l_tag_values;
    ::ZeroMemory(&l_max_value, sizeof(l_max_value));
    ::ZeroMemory(&l_tag_values, sizeof(l_tag_values));

    VfFieldSpec_t  l_vf_req;
    ::ZeroMemory(&l_vf_req, sizeof(l_vf_req));

    VfPathElem_t search_node_path[] = {
        { SVN_ST_PROTOCOL,      0 },
        { SVN_ST_SUBPROTOCOL,   0 },
        { SVN_ST_SLICEGROUP,    0 },
        { SVN_ST_SLICE,     0 },
        { SVN_ST_IMAGE,     0 },
    };


    VfPathElem_t    vf_dst_path[MAX_DEPTH] = {0};
    memset(vf_dst_path , 0 , sizeof(VfPathElem_t) * MAX_DEPTH);

    BITFLD_DECL(mask,   MAX_BITFLD);

    if (NULL == m_vfdb_handle) {
        return NULL;
    }

    vf_dst_path[0].subtree_name = SVN_ST_PROTOCOL;
    vf_dst_path[0].index		= f_protocol;

    int l_total = MAX_BITFLD, l_active = 0;
    BITFLD_INIT(mask, MAX_BITFLD, 0);

    status_t l_status = m_vfdb_handle->ScanSubtree(vf_dst_path, SVD_ST_PROTOCOL,
                        SVN_ST_SUBPROTOCOL, mask, &l_total, &l_active);

    int l_count = 0;
    int l_num_done  = -1;

    if (E_NO_ERROR == l_status) {
        for (int i = 0; i < l_total; i++) {

            if (BITFLD_ON(mask, i)) {

                l_count = 0;

                VFF_SET_ARGS(l_vf_req, f_tag_name, VFT_FLTVEC3,
                             sizeof(l_tag_values), &l_tag_values, VFO_VARIABLE_LEN);
                l_count++;

                search_node_path[SVD_ST_PROTOCOL - 1].subtree_name	    = SVN_ST_PROTOCOL;
                search_node_path[SVN_ST_PROTOCOL - 1].index             = f_protocol;
                search_node_path[SVD_ST_SUBPROTOCOL - 1].subtree_name   = SVN_ST_SUBPROTOCOL;
                search_node_path[SVD_ST_SUBPROTOCOL - 1].index		    = i;


                l_status = m_vfdb_handle->GetFields(search_node_path,
                                                    SVD_ST_SUBPROTOCOL, &l_vf_req, l_count, &l_num_done);

                if (E_ERROR == l_status) {
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed to get the required field"));
                }

                if (0 == i) {
                    l_max_value = l_tag_values;

                } else if (i > 0) {
                    if ((l_max_value.x) < l_tag_values.x) {
                        l_max_value.x = l_tag_values.x;
                    }

                    if ((l_max_value.y) < l_tag_values.y) {
                        l_max_value.y = l_tag_values.y;
                    }

                    if ((l_max_value.z) < l_tag_values.z) {
                        l_max_value.z = l_tag_values.z;
                    }
                }
            }
        }

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  CPqmUtilities::GetLibVfErrorStatusString(l_status));
    }

    f_curr_value = l_max_value;
    return 0;
}

//+Patni-Sribanta/2010July08/Added/IR-149
//*****************************************************************************
//Method Name   : ReadRealtimeSAR
//Author        : Patni/Sribanta
//Purpose       :
//*****************************************************************************
int CPQMStudy::ReadRealtimeSAR(
    const int f_acq_order,
    std::vector<f32vec3_t>* f_tag_values
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::ReadRealtimeSAR");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CPQMStudy::ReadRealtimeSAR"));

    VfPathElem_t    pelm[2] = {
        {SVN_ST_PROTOCOL,       0},
        {SVN_ST_SUBPROTOCOL,    0}
    };

    pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    pelm[SVD_ST_PROTOCOL - 1].index = f_acq_order;

    const int l_tag_count = 3;
    f_tag_values->reserve(l_tag_count);

    VfFieldSpec_t  vf_req[l_tag_count];
    memset(vf_req, 0, sizeof(VfFieldSpec_t) * l_tag_count);

    f32vec3_t l_tag_values[l_tag_count];
    memset(l_tag_values, 0, sizeof(f32vec3_t) * l_tag_count);


    int cnt = 0;
    unsigned long l_tags[l_tag_count] = {
        SVN_EXE_RTSAR_PARAMS_WB,
        SVN_EXE_RTSAR_PARAMS_HEAD,
        SVN_EXE_RTSAR_PARAMS_PARTIAL
    };

    for (int index = 0; index < l_tag_count ; index++) {

        VFF_SET_ARGS(vf_req[index], l_tags[index], VFT_FLTVEC3,
                     sizeof(l_tag_values[index]), &l_tag_values[index], VFO_VARIABLE_LEN);
        cnt++;
    }

    int  num = 0;
    status_t l_status = m_vfdb_handle->GetFields(pelm, SVD_ST_SUBPROTOCOL, vf_req, cnt, &num);

    if (E_NO_ERROR != l_status || cnt != num) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetField failed to read the subprotocol tag"));
        return FALSE;
    }

    f_tag_values->assign(l_tag_values, l_tag_values + l_tag_count);

    return static_cast<int>(f_tag_values->size());
}

//+Patni-Sribanta/2010July08/Added/IR-149
//*****************************************************************************
//Method Name   : ReadProtocolLevelTagValue
//Author        : Patni/Sribanta
//Purpose       :
//*****************************************************************************
int CPQMStudy::ReadProtocolLevelTagValue(
    const int f_acq_order,
    const int f_tag_name,
    f32vec3_t& f_var_field_value)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::ReadProtocolLevelTagValue");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CPQMStudy::ReadProtocolLevelTagValue"));

    VfPathElem_t    pelm[2] = {
        {SVN_ST_PROTOCOL,       0},
        {SVN_ST_SUBPROTOCOL,    0}
    };

    pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    pelm[SVD_ST_PROTOCOL - 1].index = f_acq_order;

    VfFieldSpec_t  vf_req = {0};
    ::ZeroMemory(&vf_req, sizeof(vf_req));

    f32vec3_t l_tag_values = {0};
    ::ZeroMemory(&l_tag_values, sizeof(l_tag_values));

    int l_count = 0;

    VFF_SET_ARGS(vf_req, f_tag_name, VFT_FLTVEC3,
                 sizeof(l_tag_values), &l_tag_values, 0);
    l_count++;
    int  l_numDone = 0;
    status_t l_status = m_vfdb_handle->GetFields(pelm, SVD_ST_PROTOCOL, &vf_req, l_count, &l_numDone);

    if ((E_NO_ERROR != l_status) || (l_count != l_numDone)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed GetFields()"));
        return l_numDone;
    }

    f_var_field_value = l_tag_values;

    return l_numDone;
}
//-Patni-Sribanta/2010July08/Added/IR-149

//*****************************************************************************
//Method Name   : ReadSubProtocolLevelTagValue
//Author        : iGATE
//Purpose       : Reads tags at Sub-protocol
//*****************************************************************************
int CPQMStudy::ReadSubProtocolLevelTagValue(
    const int f_acq_order,
    const int f_tag_name,
    std::vector<f32vec3_t>* f_tag_value)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::ReadSubProtocolLevelTagValue");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMStudy::ReadSubProtocolLevelTagValue"));

    VfPathElem_t    pelm[4] = {
        {SVN_ST_PROTOCOL,       0},
        {SVN_ST_SUBPROTOCOL,    0}
    };

    pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    pelm[SVD_ST_PROTOCOL - 1].index = f_acq_order;
    pelm[SVD_ST_SUBPROTOCOL - 1].subtree_name = SVN_ST_SUBPROTOCOL;
    pelm[SVD_ST_SUBPROTOCOL - 1].index = 0;

    BITFLD_DECL(mask, MAX_BITFLD) = {0};
    BITFLD_INIT(mask, MAX_BITFLD, 0);
    int l_active = 0;
    int l_total = MAX_BITFLD;

    status_t l_vf_stat = m_vfdb_handle->ScanSubtree(pelm, SVD_ST_PROTOCOL, SVN_ST_SUBPROTOCOL,
                         mask, &l_total, &l_active);

    if (E_NO_ERROR != l_vf_stat) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ScanSubtree failed to calculate the subprotocol nodes"));
        return 0;
    }

    VfFieldSpec_t  vf_req;
    ::ZeroMemory(&vf_req, sizeof(vf_req));

    f_tag_value->reserve(l_active);

    int l_numDone = 0;

    for (int index = 0; index < l_total; index++) {
        if (BITFLD_ON(mask, index)) {

            pelm[SVD_ST_SUBPROTOCOL - 1].index = index;

            int l_count = 0;
            f32vec3_t l_tag_values;
            ::ZeroMemory(&l_tag_values, sizeof(l_tag_values));

            VFF_SET_ARGS(vf_req, f_tag_name, VFT_FLTVEC3,
                         sizeof(l_tag_values), &l_tag_values, 0);
            l_count++;

            l_numDone = 0;
            status_t l_status = m_vfdb_handle->GetFields(pelm, SVD_ST_SUBPROTOCOL, &vf_req, l_count, (int*)&l_numDone);

            if (E_NO_ERROR != l_status || l_count != l_numDone) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetField failed to read the subprotocol tag"));
                return FALSE;
            }

            f_tag_value->push_back(l_tag_values);
        }
    }

    return static_cast<int>(f_tag_value->size());
}

//****************************Method Header************************************
//Method Name   :DeleteParentIndexFplanning()
//Author        :PATNI / MRP
//Purpose       :To delete the Parent Index from Study for the protocol
//*****************************************************************************
int CPQMStudy::DeleteParentIndexFplanning(const int   f_protocol)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::DeleteParentIndexFplanning");

    VfPathElem_t    l_path[1] = {0};//Patni-MJC/2009Aug17/Modified/cpp test corrections
    int             l_parent_index = 0;
    VfFieldSpec_t   l_vfReq[1] = {0};//Patni-MJC/2009Aug17/Modified/cpp test corrections

    memset(l_path, 0, sizeof(VfPathElem_t) * 1);

    l_path[0].subtree_name = SVN_ST_PROTOCOL;
    l_path[0].index = f_protocol;

    memset(l_vfReq, 0, sizeof(VfFieldSpec_t) * 1);

    VFF_SET_ARGS(l_vfReq[0],
                 SVN_PRC_PARENT_INDEX_FOR_PLANNING,
                 SVT_PRC_PARENT_INDEX_FOR_PLANNING,
                 sizeof(VFT_INT32),
                 &l_parent_index,
                 0);

    int             l_num = 0;
    status_t l_status = m_vfdb_handle->GetFields(l_path, SVD_ST_PROTOCOL, l_vfReq,
                        1, &l_num);

    //If Parent Index for Planning field is not found in the study
    //It is OK - MRP
    if (l_status != E_NO_ERROR) {
        return E_NO_ERROR;
    }

    l_parent_index = -1;
    VFF_SET_ARGS(l_vfReq[0],
                 SVN_PRC_PARENT_INDEX_FOR_PLANNING,
                 SVT_PRC_PARENT_INDEX_FOR_PLANNING,
                 sizeof(VFT_INT32),
                 &l_parent_index,
                 VFO_REPLACE);

    time_t          l_ts = 0;//Patni-MJC/2009Aug17/Modified/cpp test corrections
    l_status = m_vfdb_handle->PutFields(l_path, SVN_ST_PROTOCOL, l_vfReq,
                                        1, &l_num, &l_ts);

    if (l_num != 1) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
        l_status = E_ERROR;
    }

    return l_status;
}

//+Patni+MP/2009Jun07/Added/ACE-2/Missing Vantage Skip Prescan
//****************************Method Header************************************
//Method Name   :PostProcessMk()
//Author        :PATNI / MRP
//Purpose       :For checking and setting Auto Post Processing tags and values
//*****************************************************************************
void CPQMStudy::PostProcessMk(CPqmProtocol* f_curObj)
{

    LPCTSTR FUNC_NAME = _T("CPQMStudy::PostProcessMk");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of PostProcessMk"));

    if (f_curObj == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                  _T("Protocol Pointer f_curObj is NULL"));
        return;
    }

    CString l_trace_string = _T("");
    l_trace_string.Format(_T("Protocol Index = %d"), f_curObj->GetProtocol());
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);

    //+Patni+MP/2009Jun23/Modified/ACE-2/TA-Review comments

    int size = PROMPTERSCRIPT_MAXSIZE;

    /*************************************************************
      大きなエリアを確保して SVN_PRC_PROMPTER_SCRIPT形式に編集して
      Study File へ書き込む   VFT_STRING
    ***************************************************************/
    //size = PROMPTERSCRIPT_MAXSIZE ;
    char* script = new char[size];

    if (!script) {
        return ;
    }

    /****************************
      Recon & 自動処理部分の編集
     *****************************/
    if (CheckPostProcessing(f_curObj->GetProtocol()) == TRUE) {
        sprintf(script, "%s\n\n%s\n\n", RECON_SCRIPT, POSTPROC_SCRIPT) ;

        //Not in Scope of ACE-2
        /*if ( pqmPrCheckAutoFilm(pqmstudy->vfHandle,curObj->prot) == TRUE &&  AUTOFILM )
        {
            sprintf( buf , "%s\n%s\n\n" , AUTO_FILM2,FILM_MODE) ;
            strcat(script, buf);
            sprintf( buf , "%s\n" , BACKGROUND_SCRIPT4) ;
            n = 4 ;
        }
        else
        {
            sprintf( buf , "%s\n", BACKGROUND_SCRIPT3 ) ;
            n = 3 ;
        }*/

    } else {
        sprintf(script , "%s\n\n" , RECON_SCRIPT) ;
        /*if ( pqmPrCheckAutoFilm(pqmstudy->vfHandle,curObj->prot) == TRUE &&  AUTOFILM )
        {
            sprintf( buf , "%s\n%s\n\n" , AUTO_FILM1,FILM_MODE);
            strcat(script, buf);
            sprintf( buf , "%s\n" , BACKGROUND_SCRIPT3) ;
            n = 3 ;
        }
        else
        {
            sprintf( buf , "%s\n", BACKGROUND_SCRIPT2 ) ;
            n = 2 ;
           }*/
    }

    /****************************
      撮影連動部分の編集
     *****************************/
    /* if( f_curObj->GetAutoTransfer() == TRUE )
     {
        if((path = getenv("GP_DATA")) != NULL)
        {
            sprintf( work , "%s/%s" , path, SCANAUTO_FILE ) ;
        }
        else
        {
            sprintf( work , "./%s" , SCANAUTO_FILE ) ;
        }
      #ifdef __NUTC__
        fp = fopen(work, "rt");
      #else
        fp = fopen(work, "r");
      #endif
        if ( fp )
        {
            strcat(script, buf);
            if ( EZSCAN )
            {
                sprintf( work , "%s\n" , EZSCAN_MODE) ;
                strcat(script, work);
            }
            else
            {
                   sprintf( work , "%s\n" , NORMAL_MODE) ;
                strcat(script, work);
            }
                while (fgets(buf, sizeof(buf)-1, fp)) {
                if ( (size-64) < (int)((strlen(buf) + strlen(script))) ) {
                    size += PROMPTERSCRIPT_MAXSIZE;
                    realloc(script, size);
                    }
                    sprintf( work , SCANAUTO_FILE_FORM , buf) ;
                strcat(script, work);
            }
            n++ ;
            fclose( fp );
      }
      else
      {
        printf( "Can't Open  =[%s]\n", work ) ;
      }
      sprintf( work , "\n" ) ;
      strcat(script, work);
     }
      */
    /*********** END SCRIPT **********/
    //if ( EZSCAN )
    //{
    /****************************
       簡易撮影部分の編集
    *****************************/
    //sprintf( work , "%s\n\n" , EZSCAN_END) ;
    //strcat(script, work);
    //}
    //else
    //{
    /******************
      Normal scan
    *******************/
    int n = 0;
    char    work[1024] = {0} ;

    sprintf(work , "%d%s\n\n" , n, SCRIPT_END) ;

    strcat(script, work);
    //}

    PutPostscript(f_curObj->GetProtocol(), script) ;

    //+Patni+MP/2009Jun23/Modified/ACE-2/TA-Review comments
    //if ( script ) free( script ) ;
    DEL_PTR_ARY(script);

    //-Patni+MP/2009Jun23/Modified/ACE-2/TA-Review comments
}

//***************************Method Header*************************************
//Method Name    :NoiseReductionProcessCheck()
//Author         :PATNI/LK
//Purpose        :
//*****************************************************************************
void CPQMStudy::NoiseReductionProcessCheck(
    const int f_protocolnumber
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::NoiseReductionProcessCheck");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of NoiseReductionProcessCheck"));


    CPqm* l_pqm = GetPqm();

    if (l_pqm == NULL) {
        return;
    }

    //+ Patni-NP/2009Aug17/Added/MVC003302 , IR#91

    //+ Onsite 20090820
    if (l_pqm->GetNRFilter_License() == TRUE)//false) // it should be true.

    {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("NoiseReduction License is OK"));
        return;
    }

    //+ Onsite 20090820
    //-Patni-NP/2009Aug17/Added/MVC003302, IR#91

    VfPathElem_t        node_path[SVD_ST_SLICE] = {
        {SVN_ST_PROTOCOL, 0},
        {SVN_ST_SUBPROTOCOL, 0},
        {SVN_ST_SLICEGROUP, 0},
        {SVN_ST_SLICE, 0}
    };
    //+Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    VfFieldSpec_t   vf_req[8] = {0};
    int             n = 0;
    int             nr_flag = 0, automip_filter = -1;

    node_path[SVD_ST_PROTOCOL - 1].index = f_protocolnumber;

    VFF_SET_ARGS(vf_req[n],
                 SVN_PRC_RECON_NR_FLAG,
                 SVT_PRC_RECON_NR_FLAG,
                 sizeof(int),
                 &nr_flag,
                 VFO_REPLACE);

    n++;

    VFF_SET_ARGS(vf_req[n],
                 SVN_PRC_AUTO_MIP_GOP_FILTER,
                 SVT_PRC_AUTO_MIP_GOP_FILTER,
                 sizeof(int),
                 &automip_filter,
                 VFO_REPLACE);
    n++;

    //+Patni-SS/2009Apr01/PSP1#VFLOCK
    CString l_log_msg = _T("");
    l_log_msg.Format(_T(" SVN_PRC_AUTO_MIP_GOP_FILTER =%d"), automip_filter);

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_log_msg);
    //-Patni-SS/2009Apr01/PSP1#VFLOCK
    time_t tm  = 0;
    int num = 0;
    status_t vf_status = m_vfdb_handle->PutFields(node_path, SVD_ST_PROTOCOL,
                         vf_req, n, &num, &tm);
    if_error(vf_status) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("VfAppPutFields error"));
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("VfAppPutFields end"));
}

//***************************Method Header*************************************
//Method Name    :EMTONE_LicenseCheck()
//Author         :PATNI
//Purpose        :
//*****************************************************************************
void CPQMStudy::EMTONE_LicenseCheck(
    const int f_protocolnumber
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::EMTONE_LicenseCheck");

    if (m_pqm_ptr->GetEMTONE_License() == TRUE) //License is OK return

    {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("EMTONE_License is OK"));
        return;
    }

    VfFieldSpec_t	vf_req[1] = {0};

    VfPathElem_t    l_pelm[] = {
        {SVN_ST_PROTOCOL,       0},
        {SVN_ST_SUBPROTOCOL,    0}
    };

    l_pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_pelm[SVD_ST_PROTOCOL - 1].index = f_protocolnumber;


    int				n = 0, num_put = 0;
    int             l_emtflag = 0;

    VFF_SET_ARGS(vf_req[n],
                 SVN_PRC_EMTONE_FLAG,
                 SVT_PRC_EMTONE_FLAG,
                 sizeof(l_emtflag),
                 &l_emtflag,
                 VFO_REPLACE);

    n++;

    time_t tm  = 0;
    status_t vf_status = m_vfdb_handle->PutFields(l_pelm, SVD_ST_PROTOCOL,
                         vf_req, n, &num_put, &tm);
    if_error(vf_status) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("VfAppPutFields error"));
    }


}

//****************************Method Header************************************
//Method Name   :CopyCurrentProtocol()
//Author        :PATNI/HAR-JS
//Purpose       :
//*****************************************************************************
void CPQMStudy::CopyCurrentProtocol(
    const int f_protocol
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::CopyCurrentProtocol");
    //+Patni-AMT/2010Mar22/Modified/Ph-3# IR-100
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("Trace of CopyCurrentProtocol"));

    char l_current_file_name[256] = {0};
    CPqmUtilities::GetCurrScatchpadFileName(l_current_file_name);

    if (m_vfdb_handle) {
        ClibVfToUiAdapter vuiAdapter(m_vfdb_handle);
        status_t l_status = vuiAdapter.VuiCopyProtocolToFile(f_protocol, l_current_file_name, 0, 0, NULL);

        if (l_status != E_NO_ERROR) {

            CPQMLogMgr::GetInstance()->DisplayScanError(
                IDS_ERR_COPY_TO_TMP_FILE, _T("IDS_ERR_COPY_TO_TMP_FILE"),
                SCAN_ERR_NOTIFY, L"PQMStudy.cpp");
            m_pqm_ptr->WriteToUI(PQM_MSG_CLOSE_WSE);
        }

    } else {
        CPQMLogMgr::GetInstance()->DisplayScanError(
            IDS_ERR_COPY_TO_TMP_FILE, _T("IDS_ERR_COPY_TO_TMP_FILE"),
            SCAN_ERR_NOTIFY,
            L"PQMStudy.cpp");
    }

    //-Patni-AMT/2010Mar22/Modified/Ph-3# IR-100
}

//+Patni/2011Jan31/Added/V1.35/MVC009666
//****************************Method Header************************************
//Method Name   :DelPCVAPMode()
//Author        :PATNI
//Purpose       :
//*****************************************************************************
void CPQMStudy::DelPCVAPMode(const int f_protocol)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::DelPCVAPMode");

    if (NULL == m_vfdb_handle) {
        return ;
    }

    int				n = 0, num_put = 0;
    time_t			tmStamp = 0;
    VfFieldSpec_t   vfReq[1] = {0};
    VfPathElem_t    path[1] = {0};

    memset(vfReq, 0, sizeof(VfFieldSpec_t));
    memset(path, 0, sizeof(VfPathElem_t));
    path[0].subtree_name = SVN_ST_PROTOCOL;
    path[0].index = f_protocol;
    n = 0;

    status_t l_status = m_vfdb_handle->GetFields(path, SVD_ST_PROTOCOL, vfReq, n, (int*)&num_put);



    CString l_strLog(_T(""));
    l_strLog.Format(L"Status of m_vfdb_handle->GetFields() is:- %d", l_status);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_strLog);

    if (E_NO_ERROR  == vfReq[0].status) {
        VFF_SET_ARGS(vfReq[n],
                     SVN_EXE_PC_VAP_MODE ,
                     SVT_EXE_PC_VAP_MODE ,
                     0,
                     NULL,
                     VFO_DELETE);
        n++;
    }

    if (n) {
        m_vfdb_handle->PutFields(path, SVD_ST_PROTOCOL, vfReq,
                                 n, &num_put, &tmStamp);
    }
}
//-Patni/2011Jan31/Added/V1.35/MVC009666

//***************************Method Header*************************************
//Method Name    :DRKSCheck()
//Author         :PATNI
//Purpose        :
//*****************************************************************************
int CPQMStudy::DRKSCheck(
    CPqmProtocol* f_current_protocol
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::DRKSCheck");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of DRKSCheck"));

    int l_post_acq_mode = 0;
    int l_drks_flag = 0;

    BITFLD_DECL(mask, K_ALL_PR);

    memset(mask, 0, sizeof(mask));

    GetProtocolValues(m_vfdb_handle,
                      f_current_protocol->GetProtocol(),
                      mask,
                      K_PR_POST_ACQ_MODE,
                      &l_post_acq_mode,
                      NULL,
                      K_PR_DRKS_FLAG,
                      &l_drks_flag,
                      NULL,
                      NULL);
    //  DB_PRINTF("D",("postAcqMode = %d DRKSflag = %d DRKSreconMode=%d\n", postAcqMode, DRKSflag, PqmGlobal.DRKSreconMode ));
    status_t l_status = E_NO_ERROR;
    int l_mode = 0;

    if (l_drks_flag &&
        (l_post_acq_mode & VFC_MODE_AUTO_VIEW_CS) &&
        (!m_pqm_ptr->GetDRKSreconMode())) {

        l_mode = VFC_AFTER_REFVIEW_MANUAL;
        l_status = E_ERROR;

    } else {

        l_mode = VFC_AFTER_REFVIEW_AUTO;
        l_status = E_NO_ERROR;
    }

    memset(mask, 0, sizeof(mask));
    SetProtocolValues(m_vfdb_handle,
                      f_current_protocol->GetProtocol(),
                      mask,
                      K_PR_RECON_START_AFTER_REFVIEW,
                      &l_mode,
                      sizeof(int),
                      NULL);
    return l_status ;
}

//***************************Method Header*************************************
//Method Name    :ProtocolShimmingStudy()
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
BOOL CPQMStudy::ProtocolShimmingStudy(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::ProtocolShimmingStudy");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("Trace of ProtocolShimmingStudy"));
    //+Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
    //CCoil* rcvcoil_p = NULL;

    //-Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774

    if (m_pqm_ptr == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                  _T("m_pqm_ptr pointer is NULL"));
        return FALSE;
    }

    if (!m_pqm_ptr->GetShimmWBCoil()) {
        return TRUE;
    }

    CScanProcedure* l_scan_proc = m_pqm_ptr->GetAcqManager()->GetScanProcedure();
    CPqmProtocol* l_cur_protocol = l_scan_proc->GetScanProcProtocol();
    CPqmCoilData* l_coil_data = m_pqm_ptr->GetDataManager()->GetPQMCoilData();

    if (m_pqm_ptr->IsSaturnCoilSystem()) {

        CCoil l_WB_coil;

        if (!l_coil_data->GetRWBInSaturn(l_cur_protocol->GetCoilDBTable(),
                                         RWB_SHIMMING, l_WB_coil)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetRWBInSaturn failed"));
        }

        if (l_WB_coil.GetCoil() != NULL) {
            //Patni-AMT/2009Oct31/Modified/DeFT/MCM0229-00153
            SetWBCoilSaturn(m_vfdb_handle, l_cur_protocol->GetProtocol(),
                            l_cur_protocol->GetSaturnCoil(), l_WB_coil.GetCoil(),
                            l_cur_protocol->GetSaturnCoil()->RcvCoil[0].portID[1]) ;
            //Meghana here
            m_pqm_ptr->WriteToUI(PQM_MSG_COIL_CHANGE_FOR_CURRENT_PROTOCOL);
        }

    } else {
        //+Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
        CCoil rcvcoil_p;
        l_coil_data->PqmCoilGetCoilRWB(l_scan_proc->GetScanProcProtocol()->GetPqmPrMode()->GetRecvCoilId(), rcvcoil_p);

        /*rcvcoil_p = m_pqm_ptr->GetCoilDB()->GetCoilRWB(
                    l_scan_proc->GetScanProcProtocol()->GetPqmPrMode()->GetRecvCoilId());*/
        //if (rcvcoil_p == NULL) {
        if (NULL == rcvcoil_p.GetCoil()) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("Invalid coil!"));
            return FALSE;
        }

        //-Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
        int l_coil_key = l_scan_proc->GetScanProcProtocol()->GetPqmPrMode()->
                         GetRecvCoilId();
        int l_rcv_coil_key = l_scan_proc->GetScanProcProtocol()->GetPqmPrMode()->
                             GetRecvCoilId();
        //+Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
        //CString l_str_label = rcvcoil_p->GetLabel();
        CString l_str_label = rcvcoil_p.GetLabel();
        //-Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
        LPTSTR l_label = l_str_label.GetBuffer(l_str_label.GetLength() + 1);
        //+Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
        //CCoil* l_wbcoil = m_pqm_ptr->GetCoilDB()->GetWBCoil();
        CCoil l_wbcoil;
        l_coil_data->GetWBCoil(l_wbcoil);
        //int l_wb_coil_key = l_wbcoil->GetCoilId();
        int l_wb_coil_key = l_wbcoil.GetCoilId();

        //if (rcvcoil_p && (rcvcoil_p->GetRecieveWBEnable() & RWB_SHIMMING)) {
        //-Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
        if (rcvcoil_p.GetRecieveWBEnable() & RWB_SHIMMING) {

            BITFLD_DECL(mask, MAX_NODES);
            memset(mask, 0, sizeof(mask));

            if (E_NO_ERROR != SetProtocolValues(m_vfdb_handle,
                                                l_scan_proc->GetScanProcProtocol()->GetProtocol(),
                                                mask,
                                                K_PR_TR_COIL,
                                                &l_wb_coil_key,
                                                sizeof(int32_t),
                                                K_PR_RC_COIL,
                                                &l_rcv_coil_key,
                                                sizeof(int32_t),
                                                K_PR_RC_COIL_NAME,
                                                l_label,
                                                l_str_label.GetLength() + 1,
                                                NULL)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetProtocolValues failed."));
            }

            l_scan_proc->GetScanProcProtocol()->GetPqmPrMode()->SetRecvCoilId(
                //Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
                /*rcvcoil_p->GetCoilKey());*/rcvcoil_p.GetCoilKey());
        }
    }

    return TRUE;
}

//*****************************************************************************
//Method Name   : ClearFieldsOnScanStart
//Author        : Patni/PJS
//Purpose       : This function is used to clear fields on scar start
//*****************************************************************************
status_t CPQMStudy::ClearFieldsOnScanStart(
    const int f_protocol
)
{
    LPCTSTR			FUNC_NAME = _T("CPQMStudy::ClearFieldsOnScanStart");

    int				n = 0, num_put = 0;
    time_t			tmStamp = 0;
    VfFieldSpec_t   vfReq[1] = {0};
    VfPathElem_t    path[1] = {0};

    path[0].subtree_name = SVN_ST_PROTOCOL;
    path[0].index = f_protocol;
    n = 0;
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("Trace of CPQMStudy::ClearFieldsOnScanStart"));

    VFF_SET_ARGS(vfReq[n],
                 SVN_EXE_PROCESSED_FWS ,
                 SVT_EXE_PROCESSED_FWS ,
                 0,
                 NULL,
                 VFO_DELETE);

    n++;

    status_t l_status = m_vfdb_handle->PutFields(path, SVD_ST_PROTOCOL, vfReq,
                        n, &num_put, &tmStamp);

    if (n != num_put || E_NO_ERROR != l_status) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  CPqmUtilities::GetLibVfErrorStatusString(l_status));
        l_status = E_ERROR;
    }

    return l_status;
}
//-Patni-PJS/2010Jun17/Added/MVC008293 CR
//****************************Method Header************************************
//Method Name   :SearchScanMode()
//Author        :PATNI/HAR
//Purpose       :This method will set the TRSC values of the series.
//*****************************************************************************
void CPQMStudy::SearchScanMode(
    CVarFieldHandle* const f_handle,
    const int flag
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SearchScanMode");

    //Patni-KSS/2011Feb17/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(flag);


    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SearchScanMode"));

    //+Patni-MJC/2009Aug27/Modified/cpp test corrections
    if (m_pqm_ptr == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug27/Modified/cpp test corrections
    bool l_saturn_system = m_pqm_ptr->IsSaturnCoilSystem();
    //+Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    CPqmCoilData*       l_coil_data = m_pqm_ptr->GetDataManager()->GetPQMCoilData();

    if (l_coil_data == NULL) {
        return;
    }

    CPqmPrTRSC*         trsc_save_ptr = NULL;
    CPqmPrMode          pr_mode_ptr;
    float               couch_pos = 0.0f, tmp_couch = 0.0f;
    int                 count = 0, prot_for_contrast = 0, rcv_coil_id = 0;
    int                 anatomy = 0, contrast = 0;
    BOOL				l_enable = FALSE;
    //-Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    PqmSaturnCoil_t*        l_saturn_coil = NULL;
    PqmSaturnCoil_t*        l_pr_saturn_coil = NULL;
    PqmSaturnRcvCoil_t* l_saturn_table = NULL;
    PqmSaturnRcvCoil_t* l_pr_saturn_table = NULL;

    CPqmPrTRSC          trsc_ptr;
    trsc_ptr.SetAnatomy(FALSE);
    trsc_ptr.SetCoil(FALSE);
    trsc_ptr.SetContrast(FALSE);
    trsc_ptr.SetCouch(FALSE);
    protocolvec::iterator v1_Iter = NULL;
    CPqmProtocol*       pqm_protocol_ptr = NULL;
    CString tmpStr(_T(""));
    //Patni-DKH/2011Mar04/Added/TMSC-REDMINE-1486
    CScopedLock l_scoped_lock(&m_protocol_vec_mutex);

    for (v1_Iter = m_protocol_vec.begin(); v1_Iter != m_protocol_vec.end(); v1_Iter++) {

        pqm_protocol_ptr = *v1_Iter;

        //+Patni-SS/MODIFIED/2009-Jun-04/CRASH TMSC reported
        if (pqm_protocol_ptr != NULL && (pqm_protocol_ptr->GetProtocol() != PQ_FILLER)) {

            if (l_saturn_system) {

                l_saturn_coil = pqm_protocol_ptr->GetSaturnCoil();
                l_saturn_table = pqm_protocol_ptr->GetCoilDBTable();

            } else {
                rcv_coil_id = pqm_protocol_ptr->GetPqmPrMode()->GetRecvCoilId();
            }

            couch_pos = pqm_protocol_ptr->GetPqmPrMode()->GetCouchPos();
            anatomy = pqm_protocol_ptr->GetPqmPrMode()->GetAnatomy();
            contrast = pqm_protocol_ptr->GetPqmPrMode()->GetContrast();

            trsc_save_ptr = pqm_protocol_ptr->GetPqmPrTRSC();

            //+Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
            if (trsc_save_ptr == NULL) {
                return;
            }

            //-Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
            if (count == 0) {

                if (l_saturn_system) {
                    l_pr_saturn_coil = l_saturn_coil;
                    l_pr_saturn_table = l_saturn_table;

                } else {
                    pr_mode_ptr.SetRecvCoilId(rcv_coil_id);
                }

                pr_mode_ptr.SetCouchPos(couch_pos);
                pr_mode_ptr.SetAnatomy(anatomy);
                tmpStr.Format(_T("when count==0 AND pr_mode_ptr.SetAnatomy(anatomy) = %d"), anatomy);
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, tmpStr);
                pr_mode_ptr.SetContrast(contrast);
                prot_for_contrast = pqm_protocol_ptr->GetProtocol();
                count++;
            }

            tmp_couch = pr_mode_ptr.GetCouchPos() - couch_pos;

            l_enable = FALSE;

            if (l_saturn_system) {
                l_enable = l_coil_data->PqmIsSameCoilInSaturn(l_saturn_coil,
                           l_pr_saturn_coil, l_saturn_table, l_pr_saturn_table);

            } else {
                l_enable = !IS_COIL_SAME(pr_mode_ptr.GetRecvCoilId(),
                                         rcv_coil_id);
            }

            /* RcvCoil check. */
            if (l_enable) {

                if (l_saturn_system) {
                    l_pr_saturn_coil = l_saturn_coil;
                    l_pr_saturn_table = l_saturn_table;

                } else {
                    pr_mode_ptr.SetRecvCoilId(rcv_coil_id);
                }

                if (trsc_ptr.GetCoil()) {
                    trsc_ptr.SetCoil(FALSE);

                } else {
                    trsc_ptr.SetCoil(TRUE);
                }
            }

            pqm_protocol_ptr->GetPqmPrTRSC()->SetCoil(trsc_ptr.GetCoil());

            /* Couch position check. */
            if (ABSF(tmp_couch) > m_pqm_ptr->GetCouchLimitAPC()) {

                pr_mode_ptr.SetCouchPos(couch_pos);

                if (trsc_ptr.GetCouch()) {
                    trsc_ptr.SetCouch(FALSE);

                } else {
                    trsc_ptr.SetCouch(TRUE);
                }
            }

            pqm_protocol_ptr->GetPqmPrTRSC()->SetCouch(trsc_ptr.GetCouch());

            /* Anatomy check. */
            if (pr_mode_ptr.GetAnatomy() != anatomy) {

                pr_mode_ptr.SetAnatomy(anatomy);
                tmpStr = _T("");
                tmpStr.Format(_T(" Anatomy check AND pr_mode_ptr.SetAnatomy(anatomy) = %d"), anatomy);
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, tmpStr);

                if (trsc_ptr.GetAnatomy()) {
                    trsc_ptr.SetAnatomy(FALSE);

                } else {
                    trsc_ptr.SetAnatomy(TRUE);
                }
            }

            pqm_protocol_ptr->GetPqmPrTRSC()->SetAnatomy(trsc_ptr.GetAnatomy());
            tmpStr = _T("");
            tmpStr.Format(_T(" pqm_protocol_ptr->GetPqmPrTRSC()->SetAnatomy(trsc_ptr.GetAnatomy()) = %d"), trsc_ptr.GetAnatomy());
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, tmpStr);

            /* Contrast Agent check. */
            if (pr_mode_ptr.GetContrast() != contrast) {

                pr_mode_ptr.SetContrast(contrast);
                prot_for_contrast = pqm_protocol_ptr->GetProtocol();

                if (trsc_ptr.GetContrast()) {
                    trsc_ptr.SetContrast(FALSE);

                } else {
                    trsc_ptr.SetContrast(TRUE);
                }

            } else if (contrast &&
                       !(CompareContrastAgent(f_handle,
                                              prot_for_contrast,
                                              pqm_protocol_ptr->GetProtocol()))) {

                pr_mode_ptr.SetContrast(contrast);
                prot_for_contrast = pqm_protocol_ptr->GetProtocol();

                if (trsc_ptr.GetContrast()) {
                    trsc_ptr.SetContrast(FALSE);

                } else {
                    trsc_ptr.SetContrast(TRUE);
                }
            }

            pqm_protocol_ptr->GetPqmPrTRSC()->SetContrast(trsc_ptr.GetContrast());
            pqm_protocol_ptr->GetPqmPrTRSC()->SetChanged(FALSE);

            if (trsc_save_ptr->GetAnatomy() != pqm_protocol_ptr->GetPqmPrTRSC()->GetAnatomy() ||
                trsc_save_ptr->GetCoil() != pqm_protocol_ptr->GetPqmPrTRSC()->GetCoil() ||
                trsc_save_ptr->GetContrast() != pqm_protocol_ptr->GetPqmPrTRSC()->GetContrast() ||
                trsc_save_ptr->GetCouch() != pqm_protocol_ptr->GetPqmPrTRSC()->GetCouch()
               ) {
                pqm_protocol_ptr->GetPqmPrTRSC()->SetChanged(TRUE);
            }
        }
    }
}

//+Patni+MP/2009Jun07/Added/ACE-2/Missing Vantage func-Couch Distortion
//****************************Method Header************************************
//Method Name   :InitPatientPath()
//Author        :PATNI / MRP
//Purpose       :To reset the parent information for the protocol
//*****************************************************************************
void CPQMStudy::InitPatientPath(

    CPqmProtocol* f_prot

)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::InitPatientPath");

    if (f_prot == NULL)
        return;

    CString l_str(_T(""));
    l_str.Format(_T("Protocol Index  = %d"), f_prot->GetProtocol());
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_str);

    VfPathElem_t        l_path = {0};
    BITFLD_DECL(mask, K_ALL_PR);
    int32_t     l_prot_status = 0;
    flt32_t     l_parent_couchoffset = 0.0;
    //+Patni-MP/2009Jul25/Modified/ACE-2/IR for TAG conversion
    VfPathElem_t    locImages[4] = {0};
    //+Patni-MP/2009Jul25/Modified/ACE-2/IR for TAG conversion
    CString l_series_loid = _T("");

    l_path.subtree_name = 0;
    l_path.index  = 0;

    memset(mask, 0, sizeof(mask));
    int l_protocol_index = f_prot->GetProtocol();
    //+Patni-MP/2009Jul25/Modified/ACE-2/IR for TAG conversion
    int l_status = SetProtValues(
                       m_vfdb_handle, l_protocol_index, mask,
                       K_PR_LOCATOR_IMAGE_ID, &l_series_loid, l_series_loid.GetLength(),
                       K_PR_LOCATOR_IMAGE, locImages, sizeof(locImages),
                       K_PR_STATUS,       &l_prot_status,   sizeof(int32_t),
                       K_PR_PARENT_COUCH_OFFSET, &l_parent_couchoffset, sizeof(flt32_t),
                       NULL);

    //+Patni-MP/2009Jul25/Modified/ACE-2/IR for TAG conversion
    if (l_status != E_NO_ERROR) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("SetProtValues failed"));
    }

    if (PLAN_CHECK(f_prot)) {
        float   offset = 0.0f;
        f_prot->SetHasParent(false);

        int l_pl_status = f_prot->GetPlnStatus();

        l_pl_status &= ~VFC_PLN_STATUS_GLOC;

        f_prot->SetPlnStatus(l_pl_status);

        //Patni-Hemant/2009Sept01/Added/MVC#004545
        //this is to be called after SetPlnStatus only..
        m_pqm_ptr->WriteToUI(PQM_MSG_PARENT_IMAGE_DELETED_FOR_CURRENT);


        /* ホームポジション座標系をカレントポジション座標系にする */
        /* ここでは寝台移動したばかりなのでobjのcouchOffset情報は
           正しくないので、HomeとCurrent からOffset情報を求める */

        offset = (float)(COUCH_OFFSET(f_prot) / 1000.0);

        /* VG,SGともにカレントポジション座標系に変換する。
         * pqmAcqでの記述と異なる点に注意。
         */
        //VuiShiftOffsetVector(l_study_handle,l_protocol_index,
        //       VUI_SHIFT_MODE_SG,VUI_SHIFT_Z,offset);
        //VuiShiftOffsetVector(l_study_handle,l_protocol_index,
        //       VUI_SHIFT_MODE_VG,VUI_SHIFT_Z,offset);

        ClibVfToUiAdapter vuiAdapter(m_vfdb_handle);
        status_t l_status = vuiAdapter.VuiShiftOffsetVector(l_protocol_index,
                            VUI_SHIFT_MODE_SG, VUI_SHIFT_Z, offset);
        l_status = vuiAdapter.VuiShiftOffsetVector(l_protocol_index,
                   VUI_SHIFT_MODE_VG, VUI_SHIFT_Z, offset);

        f_prot->SetCouchOffsetFlag(false);
    }
}
//-Patni-MP/2009Jun07/Added/ACE-2/Missing Vantage func-Couch Distortion
//***************************Method Header*************************************
//Method Name    :IsHasLocatorImage()
//Author         :PATNI/HAR-SS
//Purpose        :This module returns judge the protocol is planed or not.
//Return Value      TRUE  : Indicated protocol has a parent image.
//                  FALSE : Indicated protocol dosn't have a parent image.

//*****************************************************************************
BOOL CPQMStudy::IsHasLocatorImage(
    const int prot
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::IsHasLocatorImage");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of IsHasLocatorImage"));

    //+Patni-MP /2010Jan22/Modified/MVC006820
    if (prot < 0) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("prot is less than zero"));
        return FALSE;
    }

    //-Patni-MP /2010Jan22/Modified/MVC006820
    //+Patni-PJS /2010Jan22/Modified/MVC006820
    if (m_vfdb_handle == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_vfdb_handle is NULL"));
        return FALSE;
    }

    //-Patni-PJS /2010Jan22/Modified/MVC006820
    VfPathElem_t path[PATH_BUFFER] = {0};
    BITFLD_DECL(mask, K_ALL_PR);

    int size = sizeof(path);
    memset(mask, 0, sizeof(mask));
    BOOL status = GetProtocolValues(m_vfdb_handle,
                                    prot,
                                    mask,
                                    K_PR_LOCATOR_IMAGE,
                                    path,
                                    &size,
                                    NULL);

    if (status == E_NO_ERROR &&
        !BITFLD_ON(mask, K_PR_LOCATOR_IMAGE) &&
        path[0].subtree_name > 0) {

        return TRUE;
    }

    return FALSE;
}

//***************************Method Header*************************************
//Method Name    :StudyCheckPlan()
//Author         :PATNI/HAR-SS
//Purpose        :
//*****************************************************************************
BOOL CPQMStudy::StudyCheckPlan(
    const CString& file_name
)
{
    int l_status = E_ERROR;
    VfFieldSpec_t l_vf_req[NUM_3] = {0};

    static VfPathElem_t l_node_path[] = {
        {SVN_ST_PROTOCOL,       0},
        {SVN_ST_SUBPROTOCOL,    0},
        {SVN_ST_SLICEGROUP,     0},
        {SVN_ST_SLICE,          0}
    };

    CVarFieldHandle vf_handle(file_name);
    l_status = vf_handle.Open();

    if (l_status != E_NO_ERROR) {
        return PQM_WARN_SCAN_PREP_STUDY;
    }

    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(l_vf_req, 0, sizeof(VfFieldSpec_t) * 3);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    int l_n = 0;
    int l_plan_status = 0;
    VFF_SET_ARGS(l_vf_req[l_n],
                 SVN_PLN_STATUS,
                 SVT_PLN_STATUS,
                 sizeof(int),
                 &l_plan_status,
                 0);

    l_n++;
    int l_num = 0;
    l_status = vf_handle.GetFields(l_node_path, SVD_ST_PROTOCOL, l_vf_req, l_n, &l_num);

    if (l_n == l_num) {

        //DB_PRINTF("p",("SVN_PLN_STATUS = [0x%x]  (%s:%d) \n",
        //  plan_status, __FILE__,__LINE__));

        if (l_plan_status & VFC_PLN_STATUS_GLOC) {
            l_status = E_NO_ERROR;

        } else { //+Patni-PJS/2009Aug28/Added/MVC002490(Reopened)defect solved
            l_status = E_ERROR;
        }

        //-Patni-PJS/2009Aug28/Added/MVC002490(Reopened)defect solved
    }

    return l_status;
}

//*****************************************************************************
//Method Name   : UpdateSpectrumData
//Author        : Patni/DKH/NFD001-CFA
//Purpose       :
//*****************************************************************************
void CPQMStudy::UpdateSpectrumData(CPqmProtocol* f_curpqmprotocol)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::UpdateSpectrumData");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMStudy::UpdateSpectrumData"));

    if (-1 == f_curpqmprotocol->GetCfaProt()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CFA protocol returned -1"));
        return;

    }

    VfFieldSpec_t       vfReq[2];
    VfFieldSpec_t       vfReq1[1];
    time_t              ts = 0L;
    VfPathElem_t        path[1], path1[1];

    path[0].subtree_name = SVN_ST_PROTOCOL;
    path[0].index = f_curpqmprotocol->GetCfaProt();

    path1[0].subtree_name = SVN_ST_PROTOCOL;
    path1[0].index = f_curpqmprotocol->GetProtocol();

    int n = 0;
    CString l_str_cfa_prot(_T(""));
    l_str_cfa_prot.Format(_T("Use CFA from = %d to = %d"), f_curpqmprotocol->GetCfaProt(), f_curpqmprotocol->GetProtocol());
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str_cfa_prot);

    memset(vfReq, NULL, sizeof(VfFieldSpec_t) * 2);

    VFF_SET_ARGS(vfReq[n], SVN_EXE_SHIM_FSY_FLAG, SVT_EXE_SHIM_FSY_FLAG,
                 0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);
    n++;


    VFF_SET_ARGS(vfReq[n], SVN_EXE_PRESCAN_FSY_SPECTRUM, SVT_EXE_PRESCAN_FSY_SPECTRUM,
                 0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);
    n++;
    int numDone = 0;
    status_t l_status = m_vfdb_handle->GetFields(path, SVD_ST_PROTOCOL,
                        vfReq, n, (int*) &numDone);

    if (E_NO_ERROR != l_status && n != numDone) {
        return;
    }

    n = 0;

    memset(vfReq1, NULL, sizeof(VfFieldSpec_t) * 1);

    if (vfReq[n].status == E_NO_ERROR) {
        VFF_SET_ARGS(vfReq1[n], SVN_EXE_SHIM_FSY_FLAG, SVT_EXE_SHIM_FSY_FLAG,
                     vfReq[n].size, vfReq[n].buffer, VFO_ALLOCATE | VFO_REPLACE);
        n++;
    }

    int l_length = 0;
    char*        l_link = NULL;

    if (NO_ERROR == vfReq[n].status) {
        l_length = vfReq[n].size;
        l_link = (char*)vfReq[n].buffer;
    }

    if (n) {
        l_status = m_vfdb_handle->PutFields(path1, SVD_ST_PROTOCOL, vfReq1,
                                            n, (int*)&numDone, &ts);
    }

    FILE*	l_fp = NULL;
    CString    l_fname = _T("CF.curveData");
    //unsigned char l_read;
    CString l_path(_T(""));
    l_path.Format(_T("%s/%s/%s"), _tgetenv(_T("GP_DATA")), _T("Prescan"), l_fname);

    if (NULL == (l_fp = _tfopen(l_path, _T("w+")))) {
        const DWORD l_Result = GetLastError();
        return ;
    }

    fwrite(l_link, l_length, 1, l_fp);
    fclose(l_fp);


    if (vfReq[1].size > 0 && NULL != vfReq[1].buffer) {
        FREE_PTR(vfReq[1].buffer);
    }

}

// + SHIMMING Lokesh 13 March 2008
//***************************Method Header*************************************
//Method Name    :PutShimmingOffset()
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
int CPQMStudy::PutShimmingOffset(
    CVarFieldHandle* const f_vf_handle,
    VfPathElem_t*   f_path,
    const int             f_depth,
    PqmShim_t*       f_shim
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::PutShimmingOffset");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of PutShimmingOffset"));

    VfFieldSpec_t l_vf_req[4] = {0};

    VFF_SET_ARGS(l_vf_req[0],
                 SVN_SEQ_SHIM_OFFSET,
                 SVT_SEQ_SHIM_OFFSET,
                 sizeof(f_shim->shimOffset),
                 f_shim->shimOffset,
                 VFO_REPLACE);

    VFF_SET_ARGS(l_vf_req[1],
                 SVN_SEQ_SHIM_GRAD_OFFSET,
                 SVT_SEQ_SHIM_GRAD_OFFSET,
                 sizeof(f_shim->gradOffset),
                 f_shim->gradOffset,
                 VFO_REPLACE);

    VFF_SET_ARGS(l_vf_req[2],
                 SVN_EXE_SHIM_FSY_VALUE,
                 SVT_EXE_SHIM_FSY_VALUE,
                 sizeof(f_shim->fsyValue),
                 &(f_shim->fsyValue),
                 VFO_REPLACE);

    VFF_SET_ARGS(l_vf_req[3],
                 SVN_SEQ_SHIM_FSY_FLAG,
                 SVT_SEQ_SHIM_FSY_FLAG,
                 sizeof(int),
                 &(f_shim->fsyFlag),
                 VFO_REPLACE);
    //+Patni-SS/2009Apr01/PSP1#VFLOCK

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of PutShimmingOffset"));

    //-Patni-SS/2009Apr01/PSP1#VFLOCK
    int l_num = 0;
    time_t l_tm = time(0);

    int l_status = f_vf_handle->PutFields(f_path, f_depth, l_vf_req, 4, &l_num, &l_tm);

    if (l_status != E_NO_ERROR ||
        l_vf_req[0].status != E_NO_ERROR ||
        l_vf_req[1].status != E_NO_ERROR ||
        l_vf_req[2].status != E_NO_ERROR) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Unable to Write Shimming Information in Study File"));
    }

    return l_status;
}
// - SHIMMING Lokesh 13 March 2008
//+Patni-MP/2010Oct19/Added/NFD001_AT_6
//****************************Method Header************************************
//Method Name   :CopySpectrumDataFromShimFile()
//Author        :PATNI/MRP
//Purpose       :Copy SpectrumData from CfDaily_Current.vfs file to the study file
//*****************************************************************************
bool CPQMStudy::CopySpectrumDataFromShimFile(CPqmProtocol* f_pqmprotocol)const
{

    LPCTSTR FUNC_NAME = _T("CPQMStudy::CopySpectrumDataFromShimFile");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Entering GetAllProtAcqStatus"));


    if (NULL == f_pqmprotocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Protocol Object is NULL"));
        return false;
    }

    char* l_path = NULL, l_file[256] = {0};

    if ((l_path = getenv("PRESCAN")) != NULL) {
        strcpy(l_file, l_path);

    } else {
        strcpy(l_file, ".");
    }

    //+Patni/2011Feb11/Modified/V2.0/IR-181_NFD1_Prod_Req_Part1
    //
    CScanProcedure* l_scan_proc = m_pqm_ptr->GetAcqManager()->GetScanProcedure();

    if (l_scan_proc &&
        (PQM_CFA_RETRY_AND_SCAN == l_scan_proc->GetCFARetry()) &&
        (FALSE == m_pqm_ptr->CanDisplayCFAGraph(f_pqmprotocol))
       ) {
        strcat(l_file, "CfDaily_2Slice_Current.vfs");

    } else {
        strcat(l_file, "CfDaily_Current.vfs");
    }

    //-Patni/2011Feb11/Modified/V2.0/IR-181_NFD1_Prod_Req_Part1

    CVarFieldHandle l_shim_vf_handle(l_file);
    status_t l_status = l_shim_vf_handle.Open();

    if (l_status == E_NO_ERROR) {
        VfFieldSpec_t       vfReq[1] = {0};
        VfFieldSpec_t       vfReq1[1] = {0};
        int                 n = 0 , numDone = 0;
        time_t				ts = 0l;
        VfPathElem_t        path0[1] = {0};
        VfPathElem_t        path1[1] = {0};

        path0[0].subtree_name = SVN_ST_PROTOCOL;
        path0[0].index = 0;

        path1[0].subtree_name = SVN_ST_PROTOCOL;
        path1[0].index = f_pqmprotocol->GetProtocol();

        n = 0;
        VFF_SET_ARGS(vfReq[n], SVN_EXE_PRESCAN_FSY_SPECTRUM, SVT_EXE_PRESCAN_FSY_SPECTRUM,
                     0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);
        n++;

        l_status = l_shim_vf_handle.GetFields(path0, SVD_ST_PROTOCOL, vfReq, n, (int*)&numDone);

        n = 0;

        if (vfReq[0].status == E_NO_ERROR) {
            VFF_SET_ARGS(vfReq1[n], SVN_EXE_PRESCAN_FSY_SPECTRUM, SVT_EXE_PRESCAN_FSY_SPECTRUM,
                         vfReq[0].size, vfReq[0].buffer, VFO_REPLACE);
            n++;
        }

        if (n) {
            l_status = m_vfdb_handle->PutFields(path1, SVD_ST_PROTOCOL, vfReq1,
                                                n, (int*)&numDone, &ts);
        }

        if (vfReq[0].status == E_NO_ERROR && vfReq[n]. size > 0) {
            FREE_PTR(vfReq[0].buffer) ;
        }

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Could not open CfDaily_Current.vfs"));
        return false;
    }

    return true;
}
//-Patni-MP/2010Oct19/Added/NFD001_AT_6
//****************************Method Header************************************
//Method Name   :Check1stScan()
//Author        :PATNI/ASG
//Purpose       :This method returns if this is the first series being scanned in
//               the study
//*****************************************************************************
bool CPQMStudy::Check1stScan(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::Check1stScan");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of Check1stScan"));

    //Patni-DKH/2011Mar04/Added/TMSC-REDMINE-1486
    CScopedLock l_scoped_lock(&m_protocol_vec_mutex);

    bool result = true;
    CPqmProtocol*   result_acqSeries = NULL;
    //Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    protocolvec::const_iterator v1_Iter = NULL;

    for (v1_Iter = m_protocol_vec.begin(); v1_Iter != m_protocol_vec.end(); v1_Iter++) {
        result_acqSeries = *v1_Iter;

        //+Patni-SS/MODIFIED/2009-Jun-04/CRASH TMSC reported
        if (result_acqSeries != NULL && result_acqSeries->GetAcqStatus() == C_PR_STATUS_DONE) {
            result =  false;
            break;

        } else {
            continue;
        }
    }

    return result;
}

//***************************Method Header*************************************
//Method Name    :MaintainStudy()
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
int CPQMStudy::MaintainStudy(
    CString f_file,
    CPqmProtocol* f_current_protocol,
    const int f_mode
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::MaintainStudy");
    // + SM4 Himanshu CFA Fix
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of MaintainStudy"));

    if (m_pqm_ptr == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return E_ERROR;
    }

    CScanProcedure* l_scan_procedure = m_pqm_ptr->GetAcqManager()->GetScanProcedure();

    if (l_scan_procedure == NULL) {
        return E_ERROR;
    }

    CPqmCoilData*   l_coil_data = CDataManager::GetInstance()->GetPQMCoilData();

    if (l_coil_data == NULL) {
        return E_ERROR;
    }

    //-Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of MaintainStudy"));

    BITFLD_DECL(mask, MAX_NODES);
    memset(mask, 0, sizeof(mask));

    double l_fsy = 0.0;
    GetProtocolValues(m_vfdb_handle,
                      f_current_protocol->GetProtocol(),
                      mask,
                      K_PR_PRESCAN_FSY_VALUE,
                      &l_fsy,
                      NULL,
                      NULL);
    CString l_print_string(_T(""));
    l_print_string.Format(_T("K_PR_PRESCAN_FSY_VALUE = %lf"), l_fsy);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_print_string);

    MaintainShimming(f_file);

    CVarFieldHandle l_maintain_vf_handle(f_file);

    if (l_maintain_vf_handle.VuiOpen() != E_NO_ERROR) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, f_file);
        return E_ERROR;
    }

    PqmCoil_t* l_coil = NULL;

    if (m_pqm_ptr->IsSaturnCoilSystem()) {

        if (!GetTxCoilInSaturn(f_current_protocol->GetCoilDBTable(), &l_coil)) {

            l_coil = NULL;

        }

    } else {
        //+Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
        /*l_coil_ptr = m_pqm_ptr->GetCoilDB()->GetTxCoil(
                         f_current_protocol->GetPqmPrMode()->GetRecvCoilId());*/
        CCoil l_coil_obj;
        l_coil_data->PqmCoilGetTxCoil(f_current_protocol->GetPqmPrMode()->GetRecvCoilId(), l_coil_obj);
        //-Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
    }

    int l_tr_coil = 0;

    if (l_coil != NULL) {

        l_tr_coil = l_coil->coil_id;

    } else {
        l_tr_coil = 0;
    }

    int l_rx_coil = f_current_protocol->GetPqmPrMode()->GetRecvCoilId();

    CCoil l_receive_coil;

    if (m_pqm_ptr->IsSaturnCoilSystem()) {  // Saturn Coil

        if (!l_coil_data->GetRWBInSaturn(f_current_protocol->GetCoilDBTable(),
                                         RWB_PREP, l_receive_coil)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetRWBInSaturn failed"));
        }

    } else {

        l_coil_data->PqmCoilGetCoilRWB(
            f_current_protocol->GetPqmPrMode()->GetRecvCoilId(), l_receive_coil);
    }

    BOOL l_use_wb = FALSE ;

    if ((f_mode == MAINTAIN_PREP) && (l_receive_coil.GetCoil() != NULL) &&
        (m_pqm_ptr->GetPrepWbCoil() == TRUE)) {
        l_use_wb = TRUE ;

        if (l_receive_coil.GetRecieveWBEnable() & RWB_PREP) {
            CCoil l_wb_coil;
            l_coil_data->GetWBCoil(l_wb_coil);
            // + SM4 AMIT+HImanshu Visual Prep Fix 20090126
            l_tr_coil = l_wb_coil.GetCoilId();
            l_rx_coil = l_receive_coil.GetCoilId() ;
            // - SM4 AMIT+HImanshu Visual Prep Fix 20090126
        }
    }

    if (m_pqm_ptr->IsSaturnCoilSystem()) {  // Saturn Coil
        if (l_use_wb == TRUE) {
            PqmSaturnCoil_t l_saturn_coil;
            memset(&l_saturn_coil, 0, sizeof(PqmSaturnCoil_t));
            //Patni-AMT/2009Oct31/Modified/DeFT/MCM0229-00153-Added port
            SetWBCoilSaturn(&l_maintain_vf_handle, 0, &l_saturn_coil,
                            l_receive_coil.GetCoil(), f_current_protocol->GetSaturnCoil()->RcvCoil[0].portID[1]) ;

        } else {
            PutSaturnCoil(&l_maintain_vf_handle, 0,
                          f_current_protocol->GetSaturnCoil());
            PutRecvrCoil(&l_maintain_vf_handle, 0, f_current_protocol);
        }

        PutConnectedCoil(&l_maintain_vf_handle, 0, f_current_protocol);

    } else {
        SetProtValues(&l_maintain_vf_handle, 0, mask,
                      K_PR_RC_COIL,     &l_rx_coil,     sizeof(int32_t),
                      NULL);
    }

    //+Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
    //l_rcv_coil_ptr = m_pqm_ptr->GetCoilDB()->GetCoilRWB(
    //                   f_current_protocol->GetPqmPrMode()->GetRecvCoilId());
    CCoil l_rcv_coil_obj;
    l_coil_data->PqmCoilGetCoilRWB(f_current_protocol->GetPqmPrMode()->GetRecvCoilId(), l_rcv_coil_obj);
    //-Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
    int l_anatomy = f_current_protocol->GetPqmPrMode()->GetAnatomy();
    float l_couch_pos = f_current_protocol->GetPqmPrMode()->GetCouchPos();
    float l_home_pos = f_current_protocol->GetPqmPrMode()->GetHomePos();
    float l_couch_offset = f_current_protocol->GetPqmPrMode()->GetCouchOffset();
    int l_view_dir = f_current_protocol->GetPqmPrMode()->GetViewDir();

    VftPatori_t   l_patori;
    l_patori.side_up = f_current_protocol->GetPqmPrMode()->GetPatoriSideUp();
    l_patori.end_in = f_current_protocol->GetPqmPrMode()->GetPatoriEndIn();
    int l_scan_number = f_current_protocol->GetPqmPrMode()->GetScanNumber();
    int l_contrast = f_current_protocol->GetPqmPrMode()->GetContrast();

    if (f_mode == MAINTAIN_MOTION_PROBING) {

        SetProtValues(&l_maintain_vf_handle,
                      0,
                      mask,
                      K_PR_TR_COIL,
                      &l_tr_coil,
                      sizeof(int),
                      K_PR_ANATOMY,
                      &l_anatomy,
                      sizeof(int),
                      K_PR_COUCH,
                      &l_couch_pos,
                      sizeof(float),
                      K_PR_HOME_POSITION,
                      &l_home_pos,
                      sizeof(float),
                      K_PR_COUCH_OFFSET,
                      &l_couch_offset,
                      sizeof(float),
                      K_PR_VIEW_DIR,
                      &l_view_dir,
                      sizeof(int),
                      K_PR_PATIENT_ORIENT,
                      &l_patori,
                      sizeof(VftPatori_t),
                      NULL);

    } else {

        SetProtValues(&l_maintain_vf_handle,
                      0,
                      mask,
                      K_PR_TR_COIL,
                      &l_tr_coil,
                      sizeof(int),
                      K_PR_ANATOMY,
                      &l_anatomy,
                      sizeof(int),
                      K_PR_SCAN_NUMBER,
                      &l_scan_number,
                      sizeof(int),
                      K_PR_COUCH,
                      &l_couch_pos,
                      sizeof(float),
                      K_PR_HOME_POSITION,
                      &l_home_pos,
                      sizeof(float),
                      K_PR_COUCH_OFFSET,
                      &l_couch_offset,
                      sizeof(float),
                      K_PR_CONTRAST,
                      &l_contrast,
                      sizeof(int),
                      K_PR_VIEW_DIR,
                      &l_view_dir,
                      sizeof(int32_t),
                      K_PR_PATIENT_ORIENT,
                      &l_patori,
                      sizeof(VftPatori_t),
                      K_PR_PRESCAN_FSY_VALUE,
                      &l_fsy,
                      sizeof(l_fsy),
                      NULL);
        MaintainBaseTempToCFStudy(f_file, f_current_protocol);
    }


    //Patni-AMT/2009Jul06/Modified/ ManualPrescan TMSC review comment
    if (E_NO_ERROR != PutRflParamsSAR(&l_maintain_vf_handle, 0)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutRflParamsSAR() failed."));
        return E_ERROR;
    }

    //+Patni+MP/2009Jun19/Added/ACE-2/B1 Shimming
    if (f_current_protocol != NULL) {
        //+Patni+MP/2010Feb26/Modified/B1 Shimming
        CPqmProtocol* l_prev_protocol = GetPrevItemFromPos(f_current_protocol);

        int l_prev_acq_order = 0;

        if (l_prev_protocol != NULL) {
            l_prev_acq_order = l_prev_protocol->GetProtocol();

        } else {
            l_prev_protocol =  Get1stScanDone();

            if (l_prev_protocol != NULL) {
                l_prev_acq_order = l_prev_protocol->GetProtocol();

            } else {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("B1-Shim data will be copied from 0th AcqOrder Protocol"));
            }
        }

        if (!m_pqm_ptr->GetAMB()->PqmAMBScanSeg2Active() && f_current_protocol->GetRflProt() == -1) {
            AddPrescanTagsForNextProtocol(m_vfdb_handle,
                                          &l_maintain_vf_handle,
                                          l_prev_acq_order,
                                          true);
        }
    }

    //+Patni+MP/2009Jun19/Added/ACE-2/B1 Shimming
    //DBDT OPERATING MODE Default set
    if (f_mode == MAINTAIN_MOTION_PROBING ||
        f_mode == MAINTAIN_MOTION_CORRECT ||
        f_mode == MAINTAIN_PREP) {

        PutDBDT_NormalMode(&l_maintain_vf_handle, 0) ;
    }

    float l_offset = 0.0f;
    int l_shim_state = 0;
    CPqmShimSharedData::GetInstance()->GetShimState(&l_shim_state);

    //PqmDebugAcqMonitor("pqmPrMaintainStudy1: mode=%d, cfaRetry=%d objPrep=%d",mode,PqmAcqGlobal.cfaRetry,obj->prepScan);

    int l_cfa_retry = l_scan_procedure->GetCFARetry();
    //Patni-Raj/2010Jan15/Added/PH#3#/Debug Window
    CPQMDebugInfo::GetInstance(m_pqm_ptr)->PqmDebugAcqMonitor(_T("pqmPrMaintainStudy1: mode=%d, cfaRetry=%d objPrep=%d"), f_mode, l_cfa_retry, l_scan_procedure->GetPrescan());

    if (f_mode == MAINTAIN_PREP ||
        f_mode == MAINTAIN_MOTION_CORRECT ||
        ((f_mode == MAINTAIN_CF) && (l_cfa_retry == PQM_CFA_RETRY_AND_SCAN)) ||
        ((f_mode == MAINTAIN_CF) && ((l_cfa_retry == PQM_CFA_RETRY) &&
                                     (l_shim_state == SHIM_STATE_ACTIVE)))) {

        l_offset = f_current_protocol->GetPqmPrMode()->GetCouchOffset();

    } else if (f_mode == MAINTAIN_MOTION_PROBING) {

        //+ Patni-MSN/2009Dec16/Modify/As per Base Code
        l_offset = (float)(COUCH_OFFSET(f_current_protocol) / 1000.0);
        //- Patni-MSN/2009Dec16/Modify/As per Base Code
    }

    //Patni-Raj/2010Jan15/Added/PH#3#/Debug Window
    CPQMDebugInfo::GetInstance(m_pqm_ptr)->PqmDebugAcqMonitor(_T("pqmPrMaintainStudy2: offset=%f"), l_offset);

    if (fabs(l_offset) >= COUCH_MIN_VALUE) {
        ClibVfToUiAdapter vuiAdapter(&l_maintain_vf_handle);
        status_t l_status = vuiAdapter.VuiShiftOffsetVector(0, VUI_SHIFT_MODE_SG, VUI_SHIFT_Z, l_offset);
        l_print_string.Format(L"Status of vuiAdapter.VuiShiftOffsetVector() is:- %d", l_status);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_print_string);


    }

    return E_NO_ERROR;
    // - SM4 Himanshu CFA Fix
}

//+Patni-AMT/2009Jul06/Modified/ ManualPrescan TMSC review comment
//***************************Method Header*************************************
//Method Name    :PutRflParamsSAR()
//Author         :PATNI/MRP
//Purpose        :This function calls the PutRflParams function of the
//                CPQMSAR class
//*****************************************************************************
status_t CPQMStudy::PutRflParamsSAR(CVarFieldHandle* const f_vf_handle, const int f_prot)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::PutRflParamsSAR");

    if (f_vf_handle == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_vf_handle is NULL => E_ERROR returned"));
        return E_ERROR;
    }

    //+Patni-MJC/2009Aug27/Modified/cpp test corrections
    if (m_pqm_ptr == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug27/Modified/cpp test corrections

    return m_pqm_ptr->GetSAR()->PutRflParams(f_vf_handle, f_prot);
}
//+Patni-MP/2009Mar12/Added/ACE1-JET SCAN requirement
//****************************Method Header************************************
//Method Name   :pqmAcqJETEnable()
//Author        :PATNI / MRP
//Purpose       :To check if the protocol is JET Enabled
//*****************************************************************************
int CPQMStudy::IsProtocolJETEnable(const int f_prot)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::IsProtocolJETEnable");

    VfPathElem_t    l_pelm[2] = {0};//Patni-MJC/2009Aug17/Modified/cpp test corrections
    VfFieldSpec_t   l_vfReq[1] = {0};//Patni-MJC/2009Aug17/Modified/cpp test corrections

    //DB_FUNC_ENTER("[pqmAcqJETEnable]:");
    //Write Trace Log here

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("IsProtocolJETEnable Entered"));

    memset(l_pelm, 0, sizeof(VfPathElem_t) * 2);    //Patni-MJC/2009Aug17/Modified/cpp test corrections
    l_pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_pelm[SVD_ST_PROTOCOL - 1].index = f_prot;

    memset(l_vfReq, 0, sizeof(VfFieldSpec_t) * 1);

    int l_n = 0, l_num = 0, l_trajectory_type = 0;
    int l_s = sizeof(int) ;
    VFF_SET_ARGS(l_vfReq[l_n], SVN_PLN_K_SPACE_TRAJECTORY_TYPE, SVT_PLN_K_SPACE_TRAJECTORY_TYPE,
                 l_s, &l_trajectory_type, NULL);
    l_n++;

    status_t l_sts = m_vfdb_handle->GetFields(l_pelm, SVD_ST_PROTOCOL, l_vfReq, l_n, &l_num);

    if (l_sts != E_NO_ERROR || l_n != l_num) {
        l_trajectory_type = VFC_K_SPACE_TRAJECTORY_TYPE_NONE ;
    }

    //if(l_vfReq.buffer!=NULL){ Please check mem allocated for buffer & l_trajectory_type is same [AoA]
    //VfFree(l_vfReq.buffer);
    //l_vfReq.buffer = NULL;
    //}
    return l_trajectory_type;
}
//+Patni-MP/2009Mar12/Added/ACE1-JET SCAN requirement

//Patni-ARD/20090226/Added/ACE-1# Enhance the withstand load of couch Start
/**********************************************
//Method Name   :pqmAcqOverErrLog()
//Author        :PATNI / ARD
//Purpose       :Log the error to file
***********************************************/
void CPQMStudy::pqmAcqOverErrLog(char* f_coilname)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::pqmAcqOverErrLog");

    char*   path = NULL, buf[128] = {0};

    //memset(buf,0,sizeof(buf));
    //memset(path,0,sizeof(path));

    //DB_FUNC_ENTER("[pqmAcqOverErrLog]:");

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Inside pqmAcqOverErrLog"));

    if ((path = getenv("GP_DATA")) != NULL) {
        sprintf(buf, "%s/%s", path, PQM_OVER_LOG);

    } else {
        sprintf(buf, "/gp/data/%s", PQM_OVER_LOG);
    }

    FILE*   fp = NULL;

    if ((fp = fopen(buf, "a")) != NULL) {
        time_t      now = 0;//Patni-MJC/2009Aug17/Modified/cpp test corrections
        struct tm*  tms = NULL;
        char    date[16] = {0};

        time(&now);
        tms = localtime(&now);

        strftime(date, sizeof(char) * 15, "%Y%m%d %H:%M", tms);

        fprintf(fp, "%s Run%d.%d %f %s\n", date,
                GetRumNum(),
                GetSiteId(),
                GetWeight(),
                f_coilname);
        fclose(fp);
    }
}
//Patni-ARD/20090226/Added/ACE-1# Enhance the withstand load of couch End

//***************************Method Header*************************************
//Method Name    :MaintainCfStudy()
//Author         :PATNI / HAR
//Purpose        :
//*****************************************************************************
int CPQMStudy::MaintainCfStudy(
    CPqmProtocol* f_protocol,
    wchar_t* f_study_name
)
{
    //+Patni-DKH/2010Dec15/Modified/TMSC-REDMINE-655-Part-I
    LPCTSTR FUNC_NAME = _T("CPQMStudy::MaintainCfStudy");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace of MaintainCfStudy"));
    //-Patni-DKH/2010Dec15/Modified/TMSC-REDMINE-655-Part-I

    f32vec3_t   slVec, phVec, ofVec;
    memset(&slVec , 0 , sizeof(f32vec3_t));
    memset(&phVec , 0 , sizeof(f32vec3_t));
    memset(&ofVec , 0 , sizeof(f32vec3_t));

    flt32_t thickness = 0.0, fov[2] = {0.0, 0.0};               //Patni-MJC/2009Aug17/Modified/cpp test corrections
    VfPathElem_t    activeShimPath[SVD_ST_PROCESSED] = {0}; //Patni-MJC/2009Aug17/Modified/cpp test corrections

    char*   path = NULL , buf[256] = {0}, file[256] = {0};

    if ((path = getenv("PRESCAN")) != NULL) {
        strcpy(buf, path);
        strcpy(file, path);

    } else {
        strcpy(buf, ".");
        strcpy(file, ".");
    }

    //+Patni/2011Feb11/Added + Modified/V2.0/IR-181_NFD1_Prod_Req_Part1
    //
    CScanProcedure* l_scan_proc = m_pqm_ptr->GetAcqManager()->GetScanProcedure();

    if (l_scan_proc &&
        (PQM_CFA_RETRY_AND_SCAN == l_scan_proc->GetCFARetry()) &&
        (FALSE == m_pqm_ptr->CanDisplayCFAGraph(f_protocol))
       ) {
        strcat(buf, "CfDaily_2Slice.vfs ");
        strcat(file, "CfDaily_2Slice_Current.vfs");

    } else {
        strcat(buf, "CfDaily.vfs ");
        strcat(file, "CfDaily_Current.vfs");
    }

    //-Patni/2011Feb11/Modified/V2.0/IR-181_NFD1_Prod_Req_Part1

    if (f_study_name != NULL) {
        mbstowcs(f_study_name, file, sizeof(char*) * 256);
        //strcpy(f_study_name,file);;
    }

    //+Patni-DKH/2010Dec15/Modified/TMSC-REDMINE-655-Part-I
    int l_status = AcqFileCopy(buf, file);

    if (l_status != E_NO_ERROR) {
        CString l_wcs_cfdaily(_T("")), l_wcs_currentcfdaily(_T(""));
        UTIL::Mbs2Wcs(&l_wcs_cfdaily, buf);
        UTIL::Mbs2Wcs(&l_wcs_currentcfdaily, file);

        CString l_errmsg = _T("");
        l_errmsg.Format(_T("AcqFileCopy(%s, %s) function FAILED.Error Value : %d."),
                        l_wcs_cfdaily, l_wcs_currentcfdaily, l_status);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_errmsg);
        CPQMLogMgr::GetInstance()->DisplayScanError(IDS_ERR_MSG_STUDY_WRITE_ERROR, _T("IDS_ERR_MSG_STUDY_WRITE_ERROR"),
                SCAN_ERR_NOTIFY, FUNC_NAME);
        return E_ERROR;

    }

    //-Patni-DKH/2010Dec15/Modified/TMSC-REDMINE-655-Part-I

    int     shimState = -1;

    if (CPqmShimSharedData::GetInstance()->GetShimState(&shimState) == TRUE &&
        CPqmShimSharedData::GetInstance()->GetShimPath(activeShimPath) == TRUE &&
        shimState == SHIM_STATE_ACTIVE) {

        int     index = -1;
        flt64_t     fsy = 0.00;
        VfPathElem_t    vfpath[4] = {0};    //Patni-MJC/2009Aug17/Modified/cpp test corrections
        memset(vfpath , 0, sizeof(VfPathElem_t) * 4);
        VfFieldSpec_t   vfReq[3] = {0};     //Patni-MJC/2009Aug17/Modified/cpp test corrections
        memset(vfReq , 0, sizeof(VfFieldSpec_t) * 3);
        int     num = -1;
        BITFLD_DECL(mask, K_ALL_PR);

        vfpath[0].subtree_name = SVN_ST_PROTOCOL;
        vfpath[0].index = 0;
        vfpath[1].subtree_name = SVN_ST_SUBPROTOCOL;
        vfpath[1].index = 0;
        vfpath[2].subtree_name = SVN_ST_SLICEGROUP;
        vfpath[2].index = 0;

        index = activeShimPath[0].index;

        ClibVfToUiAdapter vuiAdapter(m_vfdb_handle);
        vuiAdapter.VuiGetOffsetVector(index, &ofVec);
        vuiAdapter.VuiGetSliceVector(index, &slVec);
        vuiAdapter.VuiGetPhaseVector(index, &phVec);
        //PqmDebugAcqMonitor("Active Shim index = %d",index);
        //PqmDebugAcqMonitor("Active Shim Slice Vecotr  [%g,%g,%g]",
        //  slVec.x,slVec.y,slVec.z);
        //PqmDebugAcqMonitor("Active Shim Phase Vecotr  [%g,%g,%g]",
        //  phVec.x,phVec.y,phVec.z);
        //PqmDebugAcqMonitor("Active Shim Offset Vecotr [%g,%g,%g]",
        //  ofVec.x,ofVec.y,ofVec.z);
        //+Patni-Raj/2010Jan15/Added/PH#3#/Debug Window
        CPQMDebugInfo::GetInstance(m_pqm_ptr)->PqmDebugAcqMonitor(_T("Active Shim index = %d"), index);
        CPQMDebugInfo::GetInstance(m_pqm_ptr)->PqmDebugAcqMonitor(_T("Active Shim Slice Vecotr  [%g,%g,%g]"),
                slVec.x, slVec.y, slVec.z);
        CPQMDebugInfo::GetInstance(m_pqm_ptr)->PqmDebugAcqMonitor(_T("Active Shim Phase Vecotr  [%g,%g,%g]"),
                phVec.x, phVec.y, phVec.z);
        CPQMDebugInfo::GetInstance(m_pqm_ptr)->PqmDebugAcqMonitor(_T("Active Shim Offset Vecotr [%g,%g,%g]"),
                ofVec.x, ofVec.y, ofVec.z);
        //-Patni-Raj/2010Jan15/Added/PH#3#/Debug Window

        vfpath[0].index = index;

        VFF_SET_ARGS(vfReq[0], SVN_PLN_FOV, VFT_FLTVEC2, sizeof(fov), fov, NULL);

        status_t l_status = m_vfdb_handle->GetFields(vfpath, SVD_ST_PROTOCOL, vfReq, 1, &num);

        if (num == 1) {
            //Patni-Raj/2010Jan15/Added/PH#3#/Debug Window
            CPQMDebugInfo::GetInstance(m_pqm_ptr)->PqmDebugAcqMonitor(_T("Active Shim FOV [%g,%g]"), fov[0], fov[1]);

        } else {
            fov[0] = fov[0] = 0.0 ;
        }

        VFF_SET_ARGS(vfReq[0], SVN_PLN_THICKNESS, VFT_FLT32, sizeof(thickness),
                     &thickness, NULL);

        l_status = m_vfdb_handle->GetFields(vfpath, SVD_ST_SLICEGROUP, vfReq, 1, &num);

        if (num == 1) {
            //Patni-Raj/2010Jan15/Added/PH#3#/Debug Window
            CPQMDebugInfo::GetInstance(m_pqm_ptr)->PqmDebugAcqMonitor(_T("Active Shim Thickness [%g]"), thickness);

        } else {
            thickness = 0.0 ;
        }

        memset(mask, 0, sizeof(mask));
        GetProtocolValues(m_vfdb_handle, index, mask, K_PR_PRESCAN_FSY_VALUE, &fsy,
                          NULL, NULL);

        CVarFieldHandle current_vf_handle(file);
        l_status = current_vf_handle.VuiOpen();

        //Patni-RAJ/ADDED On 10/09/2009
        if (l_status == E_NO_ERROR) {
            time_t  tm = -1;
            int n = -1;
            char* cmd = NULL;

            vfpath[0].index = 0 ;

            if (f_protocol->GetLocalCF() == VFC_LOCAL_CF_ON) {
                cmd = "linkCfIRLoc";

            } else {
                cmd = "linkCfIR";
            }

            n = 0 ;

            VFF_SET_ARGS(vfReq[n], SVN_SEQ_LINK_CFA, SVT_SEQ_LINK_CFA,
                         strlen(cmd) + 1  , cmd , VFO_REPLACE);
            n++;

            VFF_SET_ARGS(vfReq[n], SVN_PLN_FOV, VFT_FLTVEC2,
                         sizeof(fov)  , fov , VFO_REPLACE);
            n++;
            status_t l_status = current_vf_handle.PutFields(vfpath, SVD_ST_PROTOCOL,
                                vfReq, n, &num, &tm);
            VFF_SET_ARGS(vfReq[0], SVN_SEQ_LINK_IMAGE, SVT_SEQ_LINK_IMAGE,
                         strlen(cmd) + 1, cmd, VFO_REPLACE);

            l_status = current_vf_handle.PutFields(vfpath, SVD_ST_SUBPROTOCOL,
                                                   vfReq, 1, &num, &tm);
            VFF_SET_ARGS(vfReq[0], SVN_PLN_THICKNESS, VFT_FLT32,
                         sizeof(thickness)  , &thickness , VFO_REPLACE);

            l_status = current_vf_handle.PutFields(vfpath, SVD_ST_SLICEGROUP,
                                                   vfReq, 1, &num, &tm);
            memset(mask, 0, sizeof(mask));

            if (E_NO_ERROR != SetProtocolValues(&current_vf_handle, 0, mask, K_PR_PRESCAN_FSY_VALUE, &fsy,
                                                sizeof(fsy), NULL)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetProtocolValues failed."));
            }

            MaintainBaseTempToCFStudy(file, f_protocol);
            //Patni-RAJ/ADDED On 10/09/2009
            ClibVfToUiAdapter vuiAdapter(&current_vf_handle);
            vuiAdapter.VuiSetOffsetVector(0, &ofVec);
            vuiAdapter.VuiSetSliceVector(0, &slVec);
            vuiAdapter.VuiSetPhaseVector(0, &phVec);
        }
    }

    return  MaintainStudy(file, f_protocol, MAINTAIN_CF);
}

//Patni-DKH/2011Feb23/Added/TMSC-REDMINE-1470
//****************************Method Header************************************
//Method Name   :CheckNextAutoProt()
//Author        :PATNI
//Purpose       : To start scan of next autoscan protocol
//*****************************************************************************
int CPQMStudy::CheckNextAutoProt(CPqmProtocol* f_prot)
{

    LPCTSTR FUNC_NAME = _T("CPQMStudy::CheckNextAutoProt");
    PQM_SCOPED_TRACE();

    if (NULL == f_prot) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("f_prot pointer is NULL"));
        return E_ERROR;
    }

    CPqmPmAcqman* acq_mgr = m_pqm_ptr->GetAcqManager();

    if (NULL == acq_mgr) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("acq_mgr pointer is NULL"));
        return E_ERROR;
    }

    CScanProcedure* scan_proc = acq_mgr->GetScanProcedure();

    if (NULL == scan_proc) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("scan_proc pointer is NULL"));
        return E_ERROR;
    }

    scan_proc->ResetCurrentRequest();

    if (scan_proc->GetIsScanning()) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Scan Is Still Running..."));
        return E_ERROR;
    }

    CString l_trace_string ;
    int sar_status = 0;
    l_trace_string.Format(_T("%s:%d  Auto Scan Start.\n"), __FILE__, __LINE__);
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);

    //+Patni-HAR/2010Jan18/Added/DeFT#MVC006364
    if (f_prot->GetChildThreadStatus() == CT_DELETE) {
        return E_ERROR;
    }

    //-Patni-HAR/2010Jan18/Added/DeFT#MVC006364
    //Patni-SS/ADDED/2009-June-01/ JFT#242
    scan_proc->SetScanProcProtocol(f_prot);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Starting Auto Scan"));
    acq_mgr->AutoScanStart();

    return E_NO_ERROR;
}


void CPQMStudy::CheckMRSValueInVF(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::CheckMRSValueInVF");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CheckMRSValueInVF"));

    CPqmProtocol* l_protocol = NULL;

    for (l_protocol  = GetFirstProtocol();
         l_protocol != NULL;
         l_protocol = GetNextItem(l_protocol)) {

        CheckMRSValueInVF(l_protocol);
    }

}

void CPQMStudy::CheckMRSValueInVF(const CPqmProtocol* f_protocol)const
{
    int l_Value = GetMRSValueFromVF(f_protocol);

    if ((!GetPqm()->GetMRS_BRST_License()) && (l_Value == VFC_PRESCAN_MRS_SHOW_GRAPH)) {
        SetMrsValueInVF(VFC_PRESCAN_MRS_EXE, f_protocol);
    }
}

//+Patni-Sribanta/2011Mar07/Added/IR-184
//************************************Method Header************************************
// Method Name  : UpdateIntensityCorrStatus()
// Author       : PATNI/Sribanta
// Purpose      :
//***********************************************************************************
bool CPQMStudy::UpdateIntensityCorrStatus(
    const int f_protocol,
    int f_intensity_corr_status_value
)
{
    VfFieldSpec_t vf_req[1] = {0};
    VfPathElem_t path[SVD_ST_PROTOCOL] = {0};

    memset(path , NULL, sizeof(path)) ;
    memset(vf_req, NULL, sizeof(vf_req)) ;

    path[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    path[SVD_ST_PROTOCOL - 1].index = f_protocol ;

    VFF_SET_ARGS(vf_req[0],
                 SVN_PRC_INTENSITY_CORR_STATUS,
                 SVT_PRC_INTENSITY_CORR_STATUS,
                 sizeof(int),
                 &f_intensity_corr_status_value,
                 VFO_REPLACE);

    int num = 0;
    time_t tm = 0;
    int status = m_vfdb_handle->PutFields(path, SVD_ST_PROTOCOL, vf_req, 1, &num, &tm);

    if ((E_NO_ERROR == status) && (1 == num)) {
        CPqmProtocol* l_protocol = GetProtocol(f_protocol);

        if (l_protocol) {
            l_protocol->SetProtcolCorrStatus(f_intensity_corr_status_value);
        }

        return true;
    }

    return false ;
}
//************************************Method Header************************************
// Method Name  : UpdateIntensityCorrMethod()
// Author       : PATNI/Sribanta
// Purpose      :
//***********************************************************************************
bool CPQMStudy::UpdateIntensityCorrMethod(
    const int f_protocol,
    int f_intensity_corr_method_value
)
{
    VfFieldSpec_t vf_req[1] = {0};
    VfPathElem_t path[SVD_ST_PROTOCOL] = {0};

    memset(path , NULL, sizeof(path)) ;
    memset(vf_req, NULL, sizeof(vf_req)) ;

    path[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    path[SVD_ST_PROTOCOL - 1].index = f_protocol ;

    VFF_SET_ARGS(vf_req[0],
                 SVN_PRC_INTENSITY_CORR_METHOD,
                 SVT_PRC_INTENSITY_CORR_METHOD,
                 sizeof(int),
                 &f_intensity_corr_method_value,
                 VFO_REPLACE);

    int num = 0;
    time_t tm = 0;
    int status = m_vfdb_handle->PutFields(path, SVD_ST_PROTOCOL, vf_req, 1, &num, &tm);

    if ((E_NO_ERROR == status) && (1 == num)) {
        return true;
    }

    return false ;
}
//************************************Method Header************************************
// Method Name  : UpdateIntensityCorrFlag()
// Author       : PATNI/Sribanta
// Purpose      :
//***********************************************************************************
bool CPQMStudy::UpdateIntensityCorrFlag(
    const int f_protocol,
    int f_intensity_corr_flag_value
)
{
    VfFieldSpec_t vf_req[1] = {0};
    VfPathElem_t path[SVD_ST_PROTOCOL] = {0};

    memset(path , NULL, sizeof(path)) ;
    memset(vf_req, NULL, sizeof(vf_req)) ;

    path[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    path[SVD_ST_PROTOCOL - 1].index = f_protocol ;

    VFF_SET_ARGS(vf_req[0],
                 SVN_PRC_INTENSITY_CORR_FLAG,
                 SVT_PRC_INTENSITY_CORR_FLAG,
                 sizeof(int),
                 &f_intensity_corr_flag_value,
                 VFO_REPLACE);

    int num = 0;
    time_t tm = 0;
    int status = m_vfdb_handle->PutFields(path, SVD_ST_PROTOCOL, vf_req, 1, &num, &tm);

    if ((E_NO_ERROR == status) && (1 == num)) {
        CPqmProtocol* l_protocol = GetProtocol(f_protocol);

        if (l_protocol) {
            l_protocol->SetIntensityType(f_intensity_corr_flag_value);
        }

        return true;
    }

    return false ;
}


//************************************Method Header************************************
// Method Name  : CheckIntensityCorrFlag()
// Author       : PATNI/Sribanta
// Purpose      :
//***********************************************************************************
bool CPQMStudy::CheckIntensityCorrFlag(
    const int f_protocol
)const
{
    VfPathElem_t    path[] = {
        {SVN_ST_PROTOCOL,       0},
        {SVN_ST_SUBPROTOCOL,    0}
    };

    VfFieldSpec_t       vf_req[1];
    memset(vf_req, 0, sizeof(VfFieldSpec_t));
    path[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    path[SVD_ST_PROTOCOL - 1].index = f_protocol;

    int cnt = 0;
    int	 l_intensity_corr_flag = 0;
    VFF_SET_ARGS(vf_req[cnt], SVN_PRC_INTENSITY_CORR_TX_RX_ENABLE_FLAG, SVT_PRC_INTENSITY_CORR_TX_RX_ENABLE_FLAG,
                 sizeof(l_intensity_corr_flag) , &l_intensity_corr_flag, 0);
    cnt++;

    int numDone = 0;
    status_t status = m_vfdb_handle->GetFields(path, SVD_ST_PROTOCOL, vf_req, cnt, &numDone);

    return ((E_NO_ERROR == status) && (cnt == numDone) && (1 == l_intensity_corr_flag)) ;

}

//************************************Method Header************************************
// Method Name  : CheckIntensityCorrMapStatus()
// Author       : PATNI/Sribanta
// Purpose      :
//***********************************************************************************
bool CPQMStudy::CheckIntensityCorrMapStatus(
    const int f_protocol
)const
{
    VfPathElem_t    path[] = {
        {SVN_ST_PROTOCOL,       0},
        {SVN_ST_PROCESSED,      0}
    };

    VfFieldSpec_t       vf_req[1];
    memset(vf_req, 0, sizeof(VfFieldSpec_t));
    path[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    path[SVD_ST_PROTOCOL - 1].index = f_protocol;

    int cnt = 0;
    int	 l_intensity_map_status = -1;
    VFF_SET_ARGS(vf_req[cnt], SVN_PRC_INTENSITY_CORR_TX_RX_MAP_STATUS, SVT_PRC_INTENSITY_CORR_TX_RX_MAP_STATUS,
                 sizeof(l_intensity_map_status) , &l_intensity_map_status, 0);
    cnt++;

    int numDone = 0;
    status_t status = m_vfdb_handle->GetFields(path, SVD_ST_PROCESSED, vf_req, cnt, &numDone);

    return((E_NO_ERROR == status) && (cnt == numDone) && (0 == l_intensity_map_status)) ;

}
//****************************Method Header************************************
//Method Name   :CopyPCVAPmodetoMRSScratchPad()
//Author        :PATNI
//Purpose       :
//*****************************************************************************
void CPQMStudy::CopyPCVAPmodetoMRSScratchPad(const int f_acq_order, const CString& f_file)
{

    LPCTSTR FUNC_NAME = _T("CPQMStudy::CopyPCVAPmodeinMRSScratchPad");

    int l_pcvapmode = -1;

    BITFLD_DECL(mask, K_ALL_PR) = {0};
    status_t l_status = GetProtocolValues(m_vfdb_handle, f_acq_order, mask, K_PR_PC_VAP_MODE,
                                          &l_pcvapmode, NULL, NULL);

    if (E_ERROR == l_status) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed to get SVN_EXE_PC_VAP_MODE."));
        return;
    }

    CVarFieldHandle l_vf_handle(f_file);

    if (l_vf_handle.VuiOpen() != E_NO_ERROR) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("MRS scratch pad Vf_handle.VuiOpen() failed.!!"));
        return;
    }

    VfFieldSpec_t	l_vf_req[1] = {0};
    int				l_num = 0;

    VFF_SET_ARGS(l_vf_req[0],
                 SVN_EXE_PC_VAP_MODE,
                 SVT_EXE_PC_VAP_MODE,
                 sizeof(l_pcvapmode),
                 &l_pcvapmode,
                 VFO_REPLACE | VFO_ALLOCATE);
    l_num++;

    int	 			l_num_done = 0;
    time_t         	l_ts = 0;
    VfPathElem_t	l_path[1] = {SVN_ST_PROTOCOL, 0};

    l_status = l_vf_handle.PutFields(l_path, SVD_ST_PROTOCOL, l_vf_req,
                                     l_num, &l_num_done, &l_ts);

    if ((E_ERROR == l_status) || (l_num != l_num_done)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed to Set SVN_EXE_PC_VAP_MODE."));
    }

    l_vf_handle.VuiClose();
}
//************************************MethodHeader************************************
// Method Name  : UpdateSARInfo()
// Author       : PATNI/Priyanka
// Purpose      :
//***********************************************************************************
BOOL CPQMStudy::UpdateSARInfo()
{

    LPCTSTR FUNC_NAME = _T("CPQMStudy::UpdateSARInfo");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (m_pqm_ptr == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return FALSE;
    }

    if (K_IMAGING != m_pqm_ptr->GetMode()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("not acquire mode, return"));
        return FALSE;
    }

    CPqmProtocol* l_protocol = NULL;
    const int l_nsize = GetAllItems();
    bool IsSarcalculated = false;

    {
        CScopedLock l_scoped_lock(&m_protocol_vec_mutex);

        for (int l_nindex = 1; l_nindex <= l_nsize; ++l_nindex) {
            l_protocol = GetItemFromPosition(l_nindex);

            if (NULL == l_protocol) {
                break;
            }

            if (l_protocol->GetAcqStatus() != C_PR_STATUS_WAIT) {
                IsSarcalculated = true;
            }

            if (l_protocol->GetAcqStatus() == C_PR_STATUS_CURRENT || l_protocol->GetAcqStatus() == C_PR_STATUS_WAIT) {
                break;
            }
        }
    }

    CString l_sar_info = _T("");

    if (NULL == l_protocol) {
        l_sar_info.Format(_T("0x07\\               \\                    \\               "), _T(""));
        m_pqm_ptr->SetCurrentSarInfo(l_sar_info);
        m_pqm_ptr->WriteToUI(PQM_MSG_UPDATE_PAGER_AREA);
        return FALSE;
    }

    m_pqm_ptr->GetSAR()->GetSARValue(l_protocol);

    CString l_sarstr = _T("---");

    sarInfo_t* l_sarinfo = l_protocol->GetSarInfo();

    if (IsSarcalculated && l_sarinfo) {

        if (SARCTRL_LICENCE_ON == m_pqm_ptr->GetSARControlLicense()) {

            if (m_pqm_ptr->GetManagerPtr() && m_pqm_ptr->GetSARManager()
               ) {


                const flt32_t l_active_sar = l_sarinfo->sarControl.tsarControl[l_sarinfo->sarControl.active].sar_6min;

                if (l_active_sar > MIN_SAR) {

                    l_sarstr.Format(_T("%0.3f"), l_active_sar);
                    l_sarstr += _T(" W/kg");
                    l_sarstr += CPQMSAR::FindSarMode(l_sarinfo->sarControl.ope_mode);
                }
            }

        } else if (l_sarinfo->tsar[l_sarinfo->active].sar > MIN_SAR) {
            l_sarstr.Format(_T("%0.3f"), l_sarinfo->tsar[l_sarinfo->active].sar);
            l_sarstr += _T(" W/kg");
            l_sarstr += CPQMSAR::FindSarMode(l_sarinfo->tsar[l_sarinfo->active].ope_mode);
        }
    }

    const CString l_scan_id = l_protocol->GetScanID();

    l_sar_info.Format(_T("0x07\\ScanID = %s      \\SAR = %s       \\dB/dt %s"), l_scan_id,  l_sarstr, l_protocol->GetdBdtModeString());

    CString l_trace_str = _T("");

    l_trace_str.Format(_T("SAR= %s"), l_sarstr);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_str);

    if (m_pqm_ptr->GetCurrentSarInfo().Compare(l_sar_info)) {
        m_pqm_ptr->SetCurrentSarInfo(l_sar_info);
        m_pqm_ptr->WriteToUI(PQM_MSG_UPDATE_PAGER_AREA);
    }

    return TRUE;
}

//****************************Method Header************************************
//Method Name   :SetRMSAEStatusForStudy
//Author        :iGATE/AKR
//Purpose       :
//*****************************************************************************
void CPQMStudy::SetRMSAEStatusForStudy(const bool f_status)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetRMSAEStatusForStudy");
    CString l_trace_str(_T("Set: SAE info sent to RM : "));

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, f_status ? (l_trace_str + _T("TRUE")) : (l_trace_str + _T("FALSE")));

    m_rm_sae_status_for_study = f_status;
}

//****************************Method Header************************************
//Method Name   :GetRMSAEStatusForStudy()
//Author        :iGATE/AKR
//Purpose       :
//*****************************************************************************
bool CPQMStudy::GetRMSAEStatusForStudy() const
{
    return m_rm_sae_status_for_study;
}

int CPQMStudy::GetTargetProtocolsDegeneracyFlag(const int f_index)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetTargetProtocolsDegeneracyFlag");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, FUNC_NAME);

    int             num = 0, n = 0;

    VfFieldSpec_t   l_vfreq[1] = {0};

    VfPathElem_t    l_pelm[SVD_ST_PROTOCOL] = {0};
    status_t l_status;

    memset(l_vfreq, 0, sizeof(VfFieldSpec_t) * 1);
    memset(l_pelm, 0, sizeof(VfPathElem_t) * SVD_ST_PROTOCOL);
    l_pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_pelm[SVD_ST_PROTOCOL - 1].index = f_index;

    int degeneracyFlag = 0;

    //Set varfield arguement
    VFF_SET_ARGS(l_vfreq[0], SVN_PLN_DEGENERACY_FLAG, SVT_PLN_DEGENERACY_FLAG,
                 sizeof(int), &degeneracyFlag,
                 0);
    n++;

    //Get required tag from varfield
    l_status = m_vfdb_handle->GetFields(l_pelm, SVD_ST_PROTOCOL, l_vfreq, n, &num);

    //Check for error
    if (num != n || l_status == E_ERROR) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed to get the required field"));
        return -1;

    } else
        return degeneracyFlag;
}

//****************************Method Header************************************
//Method Name   :SetBreathHoldValue
//Author        :PATNI/AKR/NFD002_Demo_CR-1
//Purpose       :This method will Set the breath hold value
//*****************************************************************************
void CPQMStudy::SetBreathHoldValue(CPqmProtocol* f_protocol, CVarFieldHandle* const f_vf_handle)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetBreathHoldValue");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL == f_protocol || NULL == f_vf_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_protocol or f_vf_handle is NULL"));
        return;
    }

    const bool l_breath_hold = (f_protocol->GetDynamic() || f_protocol->GetMovingCouch()) ? true : false;

    int l_hold_scan = f_protocol->GetHoldScan();

    if (PQM_SLAVE_PROTOCOL == m_pqm_ptr->IsMasterSlaveProtocol(f_protocol)) {

        l_hold_scan &= ~VFC_HOLD_COVERAGE;

        f_protocol->SetHoldScan(l_hold_scan);

        BITFLD_DECL(sub_mask, K_ALL_PR);
        BITFLD_INIT(sub_mask, K_ALL_PR, 0);

        l_hold_scan = f_protocol->GetHoldScan();

        SetProtocolValues(f_vf_handle,
                          f_protocol->GetProtocol(),
                          sub_mask,
                          K_PR_HOLD,
                          &l_hold_scan,
                          sizeof(int),
                          NULL);

    } else if (l_breath_hold && !(f_protocol->GetHoldScan() & VFC_HOLD_COVERAGE)
               && f_protocol->GetSpeederType() != VFC_SPEEDER_TYPE_SENSITIVITY) {

        f_protocol->SetHoldScan(f_protocol->GetHoldScan() | VFC_HOLD_COVERAGE);

        BITFLD_DECL(sub_mask, K_ALL_PR);
        BITFLD_INIT(sub_mask, K_ALL_PR, 0);

        l_hold_scan = f_protocol->GetHoldScan();
        SetProtocolValues(f_vf_handle,
                          f_protocol->GetProtocol(), sub_mask,
                          K_PR_HOLD,	&l_hold_scan,	sizeof(int),
                          NULL);
    }
}
//*******************************Method Header*********************************
//Method Name    :DisableMoveCouchMethodFlagFromScratchPad()
//Author         :PATNI-AKR/NFD002_2-ProductReq-DemoCR
//Purpose        :This function will disable move couch flag
//*****************************************************************************
status_t CPQMStudy::DisableMoveCouchMethodFlagFromScratchPad(CVarFieldHandle* const f_vf_handle)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::DisableMoveCouchMethodFlagFromScratchPad");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL == f_vf_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_vf_handle is NULL "));
        return E_ERROR;
    }

    int				l_num = 0;
    int l_move_couch_method_flag = VFC_MOVE_COUCH_METHOD_NONE;

    VfFieldSpec_t	l_vf_req[1] = {0};

    VFF_SET_ARGS(l_vf_req[l_num],
                 SVN_PLN_MOVE_COUCH_METHOD_FLAG,
                 VFT_INT32,
                 sizeof(VFT_INT32),
                 &l_move_couch_method_flag,
                 VFO_REPLACE | VFO_ALLOCATE);
    l_num++;

    int	 			l_num_done = 0;
    time_t         	l_ts = 0;
    VfPathElem_t    l_pelm[1] = {{SVN_ST_PROTOCOL, 0}};
    status_t l_status = f_vf_handle->PutFields(l_pelm, SVD_ST_PROTOCOL, l_vf_req,
                        l_num, &l_num_done, &l_ts);

    if ((E_ERROR == l_status) || (l_num != l_num_done)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed to Put fields "));
        return E_ERROR;
    }

    return l_status;
}
//****************************Method Header************************************
//Method Name   :SetMasterRGNValueToSlave
//Author        :PATNI/AKR/NFD002
//Purpose       :This method will Set the Master's RGN value to slave plotocol
//*****************************************************************************
status_t CPQMStudy::SetMasterRGNValueToSlave(const int f_slave_acq_order, const int f_master_acq_order)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetMasterRGNValueToSlave");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    BITFLD_DECL(mask, K_ALL_PR);
    BITFLD_INIT(mask, K_ALL_PR, 0);

    float l_rgn_value = 0.0f;

    if (E_NO_ERROR != GetProtocolValues(m_vfdb_handle,
                                        f_master_acq_order,
                                        mask,
                                        K_PR_PRESCAN_RGN_VALUE,
                                        &l_rgn_value,
                                        NULL,
                                        NULL)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetProtocolValues() failed."));
        return E_ERROR;
    }

    CString l_trace("");
    l_trace.Format(_T("Master's rgn value is %f"), l_rgn_value);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace);
    BITFLD_INIT(mask, K_ALL_PR, 0);

    if (E_NO_ERROR != SetProtocolValues(m_vfdb_handle,
                                        f_slave_acq_order,
                                        mask,
                                        K_PR_PRESCAN_RGN_VALUE,
                                        &l_rgn_value,
                                        sizeof(l_rgn_value),
                                        NULL)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Error Copying K_PR_PRESCAN_RGN_VALUE => SetProtocolValues() failed."));
        return E_ERROR;
    }

    l_trace.Format(_T("Copied RGN value from Master AcqOrder= %d to Slave AcqOrder= %d, Slave RGN Value= %f"),
                   f_master_acq_order,
                   f_slave_acq_order,
                   l_rgn_value);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace);

    return E_NO_ERROR;
}
//**************************************************************************
//Method Name   : SetGroupIDToVF
//Author        : PATNI/AKR
//Purpose       : Sets the Group Id in VFC
//**************************************************************************
void CPQMStudy::SetGroupIDToVF(CPqmProtocol* f_protocol, int f_group_id)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetGroupIDToVF");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL == f_protocol || NULL == m_vfdb_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_protocol or m_vfdb_handle is NULL"));
        return;
    }

    VfPathElem_t	l_path[1] = {0};
    l_path[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_path[SVD_ST_PROTOCOL - 1].index = f_protocol->GetProtocol();

    int				l_num = 0;
    VfFieldSpec_t	l_vf_req[1] = {0};
    VFF_SET_ARGS(l_vf_req[0],
                 SVN_PLN_GROUP_ID,
                 VFT_INT32,
                 sizeof(VFT_INT32),
                 &f_group_id,
                 VFO_REPLACE | VFO_ALLOCATE);
    l_num++;


    int	 			l_num_done = 0;
    time_t         	l_ts = 0;
    status_t l_status = m_vfdb_handle->PutFields(l_path, SVD_ST_PROTOCOL, l_vf_req,
                        l_num, &l_num_done, &l_ts);

    if ((E_ERROR == l_status) || (l_num != l_num_done)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Failed to Set SVN_PLN_GROUP_ID."));
        return;
    }

    f_protocol->SetGroupID(f_group_id);

    if (!f_group_id) {
        //This is normal protocol, delete master flag.
        SetMasterFlagToVF(f_protocol, FALSE, true);
    }
}
//**************************************************************************
//Method Name   : SetGroupNumToVFC
//Author        : PATNI/AKR--NFD002
//Purpose       : Sets the group num to VFC
//**************************************************************************
void CPQMStudy::SetGroupNumToVF(int f_group_num)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetGroupNumToVFC");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, FUNC_NAME);

    if (NULL == m_vfdb_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_vfdb_handle pointer is NULL"));
        return ;
    }

    int	 			l_num_done = 0;
    int				l_status = E_ERROR;
    time_t         	l_ts = 0;
    int				l_num = 0;


    VfFieldSpec_t	l_vf_req[1] = {0};
    VFF_SET_ARGS(l_vf_req[l_num],
                 SVN_PLN_GROUP_NUM,
                 VFT_INT32,
                 sizeof(VFT_INT32),
                 &f_group_num,
                 VFO_REPLACE | VFO_ALLOCATE);
    l_num++;

    l_status = m_vfdb_handle->PutFields(NULL, SVD_ST_STUDY, l_vf_req,
                                        l_num, &l_num_done, &l_ts);

    if ((E_ERROR == l_status) || (l_num != l_num_done)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Failed to Set SVN_PLN_GROUP_NUM."));
    }
}
//****************************Method Header************************************
//Method Name   :GetDelayTimeFromVf()
//Author        :PATNI/AKR--NFD002
//Purpose       :This method will get the delay time from VF DB and return as time string
//*****************************************************************************
CString CPQMStudy::GetDelayTimeFromVf(const int f_protocol)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetDelayTimeFromVf");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, FUNC_NAME);

    if (NULL == m_vfdb_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_vfdb_handle pointer is NULL"));
        return _T("");
    }

    int				l_num_done_t = 0;
    int				l_status = E_ERROR;
    int				l_delay_time_vfc = 0;
    int				l_n = 0;

    VfFieldSpec_t	l_vf_req[NUM_1] = {0};
    VfPathElem_t	l_path[NUM_1] = {0};
    CString			l_delay_time_str(_T(""));

    l_path[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_path[SVD_ST_PROTOCOL - 1].index = f_protocol;

    VFF_SET_ARGS(l_vf_req[0],
                 SVN_PLN_INJECT_TIME,
                 VFT_INT32,
                 sizeof(VFT_INT32),
                 &l_delay_time_vfc,
                 VFO_ALLOCATE | VFO_VARIABLE_LEN);
    l_n++;


    l_status = m_vfdb_handle->GetFields(l_path, SVD_ST_PROTOCOL, l_vf_req,
                                        l_n, &l_num_done_t);

    if ((E_ERROR == l_status) || (l_num_done_t != l_n)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Delay Time is not present for this protocol."));
        return l_delay_time_str;
    }

    if (l_delay_time_vfc > MAX_NEW_DELAY) {
        l_delay_time_vfc = 0 ;	//If delay time is set to more than 99:59, it should not display on GUI.
    }

    if (l_delay_time_vfc) {
        int l_min = l_delay_time_vfc / SIXTY_SEC;
        int l_sec = l_delay_time_vfc % SIXTY_SEC;
        l_delay_time_str.Format(_T("%02d:%02d"), l_min, l_sec);
    }

    return l_delay_time_str;
}
//****************************Method Header************************************
//Method Name   :SetNewDelayTimeToVf()
//Author        :PATNI/AKR/NFD002-Part2-2 Demo CR
//Purpose       :This method will set the updated delay time in VFC
//*****************************************************************************
void CPQMStudy::SetNewDelayTimeToVf(int f_delay_time, const int f_protocol)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetNewDelayTimeToVf");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, FUNC_NAME);

    if (NULL == m_vfdb_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_vfdb_handle pointer is NULL"));
        return ;
    }

    int	 			l_num_done = 0;
    int				l_status = E_ERROR;
    time_t         	l_ts = 0;
    int				l_num = 0;

    VfFieldSpec_t	l_vf_req[1] = {0};
    VfPathElem_t	l_path[1] = {0};

    l_path[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_path[SVD_ST_PROTOCOL - 1].index = f_protocol;

    VFF_SET_ARGS(l_vf_req[0],
                 SVN_PLN_INJECT_TIME,
                 VFT_INT32,
                 sizeof(VFT_INT32),
                 &f_delay_time,
                 VFO_REPLACE | VFO_ALLOCATE);
    l_num++;


    l_status = m_vfdb_handle->PutFields(l_path, SVD_ST_PROTOCOL, l_vf_req,
                                        l_num, &l_num_done, &l_ts);

    if ((E_ERROR == l_status) || (l_num != l_num_done)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed to Set SVN_PLN_INJECT_TIME."));
        return;
    }

    SetDelayTimeForProtocol(f_protocol, f_delay_time);
}

//****************************Method Header************************************
//Method Name   :CalculateTimeDiff()
//Author        :PATNI/AKR/NFD002-Part2-1
//Purpose       :This method will calculate the time difference of injected time from now.
//*****************************************************************************
void CPQMStudy::CalculateTimeDiff(VARIANT* f_pdata)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::CalculateTimeDiff");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, FUNC_NAME);

    if (NULL == m_vfdb_handle || NULL == f_pdata) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_vfdb_handle or f_pdata pointer is NULL"));
        return ;
    }

    int				l_num_done_t = 0;
    int				l_status_timer = E_ERROR;
    time_t			l_time_injected = 0;
    time_t			l_now = time(0);
    int				l_n = 0;

    VfFieldSpec_t	l_vf_req_inject_time[NUM_1] = {0};

    VFF_SET_ARGS(l_vf_req_inject_time[0],
                 SVN_EXE_INJECT_TIME,
                 VFT_TIME,
                 sizeof(time_t),
                 &l_time_injected,
                 0);
    l_n++;


    l_status_timer = m_vfdb_handle->GetFields(NULL, SVD_ST_STUDY, l_vf_req_inject_time,
                     l_n, &l_num_done_t);

    if (E_ERROR == l_status_timer || l_num_done_t != l_n) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Failed to Get inject time from VFC"));
    }

    time_t l_diff = l_now - l_time_injected;

    if (l_diff > 0) {
        f_pdata->intVal = l_diff;

    } else {
        f_pdata->intVal = 0;
    }
}
//****************************Method Header************************************
//Method Name   :CheckForInjectedTime()
//Author        :PATNI/AKR/NFD002-Part2-1
//Purpose       :This method will check for injected time present or not.
//*****************************************************************************
bool CPQMStudy::CheckForInjectedTime(int& f_injected_time_diff)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::CheckForInjectedTime");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, FUNC_NAME);

    int	 			l_num_done_t = 0;
    int				l_status_timer = E_ERROR;
    time_t			l_time_injected = 0;
    time_t			l_now_t = time(0);
    int				l_n = 0;
    bool			l_return_status = true;
    VfFieldSpec_t	l_vf_req_inject_time[1] = {0};
    VfPathElem_t	l_path_inject_time[1] = {0};

    VFF_SET_ARGS(l_vf_req_inject_time[0],
                 SVN_EXE_INJECT_TIME,
                 VFT_TIME,
                 sizeof(time_t),
                 &l_time_injected,
                 0);
    l_n++;

    if (NULL == m_vfdb_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_vfdb_handle pointer is NULL"));
        return false;
    }

    l_status_timer = m_vfdb_handle->GetFields(NULL, SVD_ST_STUDY, l_vf_req_inject_time,
                     l_n, &l_num_done_t);

    if ((E_ERROR == l_status_timer) || (l_n != l_num_done_t)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Inject Time is not present for this protocol."));
        return false;
    }

    CString l_trace_str(_T(""));

    if (0 != l_time_injected) {

        f_injected_time_diff = l_now_t -  l_time_injected;

        l_trace_str.Format(_T("SVN_EXE_INJECT_TIME tag present, time difference is %d"), f_injected_time_diff);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_str);

        if (MAX_TIMER_TIME < f_injected_time_diff || MIN_TIMER_TIME > f_injected_time_diff)  {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Delay time has crossed its limit"));

            f_injected_time_diff = 0;

            l_return_status = false;

        }

        m_pqm_ptr->GetAcqManager()->SetInjectTimePresent(true);

        return l_return_status;
    }

    return false;
}
//**************************************************************************
//Method Name   : SetMasterFlagToVF
//Author        : PATNI/AKR
//Purpose       : Sets the master flag to VFC
//**************************************************************************
void CPQMStudy::SetMasterFlagToVF(CPqmProtocol* f_protocol, BOOL f_master_flag, bool f_can_delete_tag)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetMasterFlagToVFC");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL == f_protocol || NULL == m_vfdb_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_protocol or m_vfdb_handle is NULL"));
        return;
    }

    int	 			l_num_done = 0;
    int				l_status = E_ERROR;
    time_t         	l_ts = 0;
    int				l_num = 0;

    VfFieldSpec_t	l_vf_req[1] = {0};
    VfPathElem_t	l_path[1] = {0};

    l_path[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_path[SVD_ST_PROTOCOL - 1].index = f_protocol->GetProtocol();

    unsigned long l_option_flag = VFO_REPLACE | VFO_ALLOCATE;

    if (!f_master_flag && f_can_delete_tag) {
        l_option_flag = VFO_DELETE;
    }

    VFF_SET_ARGS(l_vf_req[0],
                 SVN_PLN_MASTER_FLAG,
                 VFT_INT32,
                 sizeof(VFT_INT32),
                 &f_master_flag,
                 l_option_flag);
    l_num++;

    l_status = m_vfdb_handle->PutFields(l_path, SVD_ST_PROTOCOL, l_vf_req,
                                        l_num, &l_num_done, &l_ts);

    if ((E_ERROR == l_status) || (l_num != l_num_done)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed to Set SVN_PLN_MASTER_FLAG."));
        return;
    }

    f_protocol->SetMasterFlag(f_master_flag);
}


//**************************************************************************
//Method Name   : CheckPixelAndRGNCorrFlag
//Author        : PATNI/AKR
//Purpose       : Checks Pixel Normalization mode and RGN correction value from VF for master slave protocols
//**************************************************************************
bool CPQMStudy::CheckPixelAndRGNCorrFlag(const int f_current_protocol)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::CheckPixelAndRGNCorrFlag");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL == m_vfdb_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_vfdb_handle pointer is NULL"));
        return false;
    }

    bool			l_return_value = false;
    int				l_num_done = 0;
    int				l_status = E_ERROR;
    int				l_pixel_normalization_mode = 0;
    int				l_rgn_corr_flag = 0;
    int				l_n = 0;


    VfFieldSpec_t	l_vf_req[NUM_2] = {0};
    VfPathElem_t	l_path[NUM_1] = {0};
    CString			l_delay_time_str(_T(""));

    l_path[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_path[SVD_ST_PROTOCOL - 1].index = f_current_protocol;


    VFF_SET_ARGS(l_vf_req[l_n],
                 SVN_PRC_PIXEL_NORMALIZE_MODE,
                 SVT_PRC_PIXEL_NORMALIZE_MODE,
                 sizeof(VFT_INT32),
                 &l_pixel_normalization_mode,
                 0);
    l_n++;

    VFF_SET_ARGS(l_vf_req[l_n],
                 SVN_PRC_RGN_CORR_FLAG,
                 SVT_PRC_RGN_CORR_FLAG,
                 sizeof(VFT_INT32),
                 &l_rgn_corr_flag,
                 0);
    l_n++;

    l_status = m_vfdb_handle->GetFields(l_path, SVD_ST_PROTOCOL, l_vf_req,
                                        l_n, &l_num_done);

    if ((E_ERROR == l_status) || (l_num_done != l_n)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Tags are not present in study file"));
        return l_return_value;
    }

    if (VFC_PIXEL_NORMALIZE_CLIPPING == l_pixel_normalization_mode &&
        l_rgn_corr_flag) {

        l_return_value = true;
    }

    return l_return_value;
}
//*******************************Method Header*********************************
//Method Name    :IsTagPresentInFile
//Author         :PATNI/Abhishek/Redmine-2187
//Purpose        :This function will check the given tag is present in the given file or not.
//*****************************************************************************
bool CPQMStudy::IsTagPresentInFile(CVarFieldHandle* f_vf_handle, int f_tag, int f_type
                                   , const int f_acq_order)
{
    LPCTSTR FUNC_NAME = _T("CMasterSlave::IsTagPresentInFile");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    int                 l_num = 0, l_numdone = 0;
    VfFieldSpec_t       l_vfreq[1] = {0};
    VfPathElem_t        l_pelm[1] = {0};
    status_t            l_status = E_ERROR;

    if (NULL == f_vf_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_vf_handle is NULL"));
        return false;
    }

    l_pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_pelm[SVD_ST_PROTOCOL - 1].index = f_acq_order;

    VFF_SET_ARGS(l_vfreq[0], f_tag, f_type, 0,
                 NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);
    l_num++;

    l_status = f_vf_handle->GetFields(l_pelm, SVD_ST_PROTOCOL, l_vfreq, l_num, &l_numdone);

    FREE_PTR(l_vfreq[0].buffer);

    return l_status ? false : (l_num == l_numdone) ? true : false;
}

//***************************Method Header*************************************
//Method Name    :SendSAEInfo()
//Author         :PATNI/AKR
//Purpose        :This function will get the last completed protocol's total sae and send to acqman
//*****************************************************************************
bool CPQMStudy::SendSAEInfo()
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SendSAEInfo");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    bool l_status = false;

    if (K_IMAGING != m_pqm_ptr->GetMode()) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PQM schedule mode"));
        return l_status;
    }

    if (!m_study_loid.CompareNoCase(_T(""))) {


        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_study_loid is null"));
        return l_status;
    }

    if (GetRMSAEStatusForStudy()) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Total SAE info is already set to RM"));
        return l_status;
    }

    int32_t l_total_sae = GetTotalSAE(GetLastCompletedProtocol());

    CScanProcedure* l_scan_proc = m_pqm_ptr->GetAcqManager()->GetScanProcedure();

    if (NULL == l_scan_proc) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_scan_proc is NULL"));
        return l_status;
    }

    if (!(l_status = l_scan_proc->SendSAEInfo(l_total_sae))) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Total SAE value is not set on Hardware"));

        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
            CPqmUtilities::GetMultiLingualString(_T("IDS_ACQUISITION_MGR_SAE_ERROR")),
            MPlus::EH::Information, _T("PQM"), _T("PQM"));
    }

    return l_status;
}
//***************************Method Header*************************************
//Method Name    :GetTotalSAE()
//Author         :PATNI/AKR
//Purpose        :Return the total SAE value of the given protocol
//					1. if current/wait protocol => return sarInfo_t -> sae -> total_sae
//					2. if dane/fail protocol => Get SVN_SEQ_TOTAL_SEQ, if not present => return 0
//												if SVN_SEQ_TOTAL_SEQ [1] == 0 ,
//													return SVN_SEQ_TOTAL_SEQ [0]
//												else
//													return SVN_SEQ_TOTAL_SEQ [1]
//*****************************************************************************
int32_t CPQMStudy::GetTotalSAE(CPqmProtocol* f_pqm_protocol)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetTotalSAE");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    int32_t l_total_sae = 0;

    if (NULL == f_pqm_protocol) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_pqm_protocol is NULL"));
        return l_total_sae;
    }

    const int l_protocol_status = f_pqm_protocol->GetAcqStatus();

    if (C_PR_STATUS_WAIT == l_protocol_status || C_PR_STATUS_CURRENT == l_protocol_status) {

        l_total_sae = f_pqm_protocol->GetSarInfo()->sae.sae_total;


    } else {
        int32_t l_total_sae_arr[NUM_2] = {0};

        if (E_NO_ERROR == GetTotalSAEValueFromVarfield(f_pqm_protocol, l_total_sae_arr)) {

            l_total_sae = l_total_sae_arr[1] > 0 ? l_total_sae_arr[1] : l_total_sae_arr[0];
        }
    }

    CString l_trace_str(_T(""));
    l_trace_str.Format(_T("Total SAE value = %d"), l_total_sae);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_str);

    return l_total_sae;
}
//***************************Method Header*************************************
//Method Name    :GetLastCompletedProtocol()
//Author         :PATNI/AKR
//Purpose        :Return Last done or Fail protocol
//*****************************************************************************
CPqmProtocol* CPQMStudy::GetLastCompletedProtocol()
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetLastCompletedProtocol");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    CScopedLock l_scoped_lock(&m_protocol_vec_mutex);

    const int l_total_protocols = GetAllItems();
    CPqmProtocol* l_curr_protocol = NULL, *l_last_complete_protocol = NULL;

    for (int l_index = 0; l_index < l_total_protocols; l_index++) {

        l_curr_protocol = GetProtocolForIndex(l_index);

        if (NULL == l_curr_protocol) {
            break;
        }

        if (C_PR_STATUS_DONE == l_curr_protocol->GetAcqStatus() ||
            C_PR_STATUS_FAIL == l_curr_protocol->GetAcqStatus()) {

            l_last_complete_protocol = l_curr_protocol;

        } else {
            break;
        }
    }

    return l_last_complete_protocol;
}


//***************************Method Header*************************************
//Method Name    :GetVfDBHandle()
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
CVarFieldHandle* CPQMStudy::GetVfDBHandle()
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetVfDBHandle");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetVfDBHandle"));
    return m_vfdb_handle;
}

//****************************Method Header************************************
//Method Name   :GetProtocol()
//Author        :PATNI/MRP
//Purpose       :This method returns protocol based on AcqOrder specified by
//                  protocol_number
//*****************************************************************************
CPqmProtocol* CPQMStudy::GetProtocol(
    const int protocol_number
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetProtocol");
    //Patni-AMT/2009Oct27/Added/ Code Review
    CString l_acq_order = _T("");
    l_acq_order.Format(_T("GetProtocol with AcqOrder=%d"), protocol_number);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_acq_order);
    //-Patni-AMT/2009Oct27/Added/ Code Review

    //Patni-DKH/2011Mar04/Added/TMSC-REDMINE-1486
    CScopedLock l_scoped_lock(&m_protocol_vec_mutex);

    CPqmProtocol* result_acqSeries = NULL;
    protocolvec::const_iterator v1_Iter = NULL;

    for (v1_Iter = m_protocol_vec.begin(); v1_Iter != m_protocol_vec.end(); v1_Iter++) {

        ASSERT(v1_Iter != NULL) ;

        result_acqSeries = *v1_Iter;

        //Patni-SS/MODIFIED/2009-Jun-02/JFT#213
        if (result_acqSeries != NULL && result_acqSeries->GetProtocol() == protocol_number) {
            break;

        } else {
            //Patni-AMT/2009Oct27/Added/ Code Review
            result_acqSeries = NULL;
        }
    }

    if (!result_acqSeries) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("WARNING!! Protocol is NULL."));
    }

    return result_acqSeries;
}

//***************************Method Header*************************************
//Method Name    :GetItemFromPosition()
//Author         :PATNI/HAR-SS
//Purpose        :
//*****************************************************************************
CPqmProtocol* CPQMStudy::GetItemFromPosition(
    const int position
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetItemFromPosition");
    CString str(_T(""));
    str.Format(_T("position = %d"), position);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str);

    // + SM4 Himanshu 9 Dec 2008
    if (position < 1) {
        return NULL;
    }

    //Patni-DKH/2011Mar04/Added/TMSC-REDMINE-1486
    CScopedLock l_scoped_lock(&m_protocol_vec_mutex);

    // - SM4 Himanshu 9 Dec 2008
    CPqmProtocol*   result_acqSeries = NULL;
    //Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    protocolvec::const_iterator v1_Iter = NULL;

    int counter = 1;

    for (v1_Iter = m_protocol_vec.begin(); v1_Iter != m_protocol_vec.end(); v1_Iter++) {

        result_acqSeries = *v1_Iter;

        if (counter == position) {
            break;
        }

        counter++;
    }

    return result_acqSeries;
}

//***************************Method Header*************************************
//Method Name    :GetItemFromProtocol()
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
CPqmProtocol* CPQMStudy::GetItemFromProtocol(
    const int f_protocol
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetItemFromProtocol");
    CString str(_T(""));
    str.Format(_T("f_protocol = %d"), f_protocol);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str);

    CPqmProtocol*   result_acqSeries = NULL;

    //Patni-DKH/2011Mar04/Added/TMSC-REDMINE-1486
    CScopedLock l_scoped_lock(&m_protocol_vec_mutex);

    if (f_protocol < 0) {
        return result_acqSeries ;
    }

    //Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    protocolvec::const_iterator v1_Iter = NULL;
    int counter = 1;

    for (v1_Iter = m_protocol_vec.begin(); v1_Iter != m_protocol_vec.end(); v1_Iter++, counter++) {

        result_acqSeries = *v1_Iter;

        //+Patni-SS/MODIFIED/2009-Jun-04/CRASH TMSC reported
        if (result_acqSeries != NULL && result_acqSeries->GetProtocol() == f_protocol) {
            break;
        }
    }

    return result_acqSeries;
}

//****************************Method Header************************************
//Method Name   :GetPqm()
//Author        :PATNI/MRP
//Purpose       :This method will return the pqm pointer
//*****************************************************************************
CPqm* CPQMStudy::GetPqm(
)const
{
    return m_pqm_ptr;
}

//***************************Method Header*************************************
//Method Name    :GetStudyName()
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
CString CPQMStudy::GetStudyName(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetStudyName");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetStudyName"));
    return m_study_name;
}

//***************************Method Header*************************************
//Method Name    :GetRumNum()
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
int CPQMStudy::GetRumNum(
) const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetRumNum");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetRumNum"));

    return m_study_id.run_num;
}

//***************************Method Header*************************************
//Method Name    :GetSiteId()
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
int CPQMStudy::GetSiteId(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetSiteId");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetSiteId"));
    return m_study_id.site_id;
}

//+Patni-DKH/2010Jan29/Added/PH#3#/FBI
//**************************************************************************
//Method Name   : GetStudyID
//Author        : PATNI\DKH
//Purpose       :
//**************************************************************************
study_id_t CPQMStudy::GetStudyID()const
{
    return m_study_id ;
}
//-Patni-DKH/2010Jan29/Added/PH#3#/FBI

//****************************Method Header************************************
//Method Name   :GetShimFlag
//Author        :PATNI/
//Purpose       :
//*****************************************************************************
int CPQMStudy::GetShimFlag(
    VfPathElem_t* f_vfpath_element
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetShimFlag");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetShimFlag"));

    int             n = 0, l_numdone = -1;
    VfFieldSpec_t   l_reqflds[5] = {0};//Patni-MJC/2009Aug17/Modified/cpp test corrections
    int             l_typeformatdepth = SVD_ST_PROTOCOL;
    VfPathElem_t*   l_processpathp = f_vfpath_element;
    int             l_shim_flag = -1;
    int             l_status = E_ERROR;

    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(l_reqflds, 0, sizeof(VfFieldSpec_t) * 5);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    VFF_SET_ARGS(l_reqflds[n], SVN_SEQ_SHIM_FLAG, SVT_SEQ_SHIM_FLAG,
                 sizeof(int), &l_shim_flag, 0);
    n++;

    // opens the study file
    l_status = m_vfdb_handle->GetFields(l_processpathp, l_typeformatdepth, l_reqflds, n, &l_numdone);

    if (l_status != E_NO_ERROR || l_numdone != n) {
        l_shim_flag = VFC_SHIM_STANDARD;
        CString l_tracemsg(_T(""));
        l_tracemsg.Format(_T("Failed to read Shim Flag SVN_SEQ_SHIM_FLAG and default is set to VFC_SHIM_STANDARD"));
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_tracemsg);
    }

    // close the study file
    CString l_shimflg_trace(_T(""));

    if (l_shim_flag == VFC_SHIM_NONE) {
        l_shimflg_trace.Format(_T("Shim Flag --> VFC_SHIM_NONE"));

    } else if (l_shim_flag == VFC_SHIM_STANDARD) {
        l_shimflg_trace.Format(_T("Shim Flag --> VFC_SHIM_STANDARD"));

    } else if (l_shim_flag == VFC_SHIM_HIGH_RESOLUTION) {
        l_shimflg_trace.Format(_T("Shim Flag --> VFC_SHIM_HIGH_RESOLUTION"));
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_shimflg_trace);
    return l_shim_flag;
}

//***************************Method Header*************************************
//Method Name    :GetShimmingOffset()
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
int CPQMStudy::GetShimmingOffset(
    PqmShim_t* f_shim
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetShimmingOffset");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetShimmingOffset"));

    //Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    VfFieldSpec_t l_vf_req[NUM_4] = {0};

    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(l_vf_req, 0, sizeof(VfFieldSpec_t) * NUM_4);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    VFF_SET_ARGS(l_vf_req[0],
                 SVN_SEQ_SHIM_OFFSET,
                 SVT_SEQ_SHIM_OFFSET,
                 sizeof(int) * PQM_SHIM_LEN,
                 f_shim->shimOffset,
                 VFO_VARIABLE_LEN);

    VFF_SET_ARGS(l_vf_req[1],
                 SVN_SEQ_SHIM_GRAD_OFFSET,
                 SVT_SEQ_SHIM_GRAD_OFFSET,
                 sizeof(int) * PQM_GRAD_LEN,
                 f_shim->gradOffset,
                 VFO_VARIABLE_LEN);

    VFF_SET_ARGS(l_vf_req[NUM_2],
                 SVN_EXE_SHIM_FSY_VALUE,
                 SVT_EXE_SHIM_FSY_VALUE,
                 sizeof(flt64_t),
                 &f_shim->fsyValue,
                 0);

    int l_num = 0;
    int l_status = m_vfdb_handle->GetFields(NULL, SVD_ST_STUDY,
                                            l_vf_req, NUM_3, &l_num);

    if (l_status != E_NO_ERROR ||
        l_num != NUM_3 ||
        l_vf_req[0].status != E_NO_ERROR ||
        l_vf_req[1].status != E_NO_ERROR ||
        l_vf_req[NUM_2].status != E_NO_ERROR) {

        //      DB_PRINTF("",("Get Data Error. (%d/3) [%s,%s,%s]\n",
        //          num,
        //          VfFieldErrorStr(vfReq[0].status,NULL,NULL),
        //          VfFieldErrorStr(vfReq[1].status,NULL,NULL),
        //          VfFieldErrorStr(vfReq[2].status,NULL,NULL)));
        memset(f_shim, 0, sizeof(PqmShim_t));
        l_status = E_ERROR;
    }

    return l_status;
}

//***************************Method Header*************************************
//Method Name    :GetImageDataKind()
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
int CPQMStudy::GetImageDataKind(
    int protocol,
    int proc
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetImageDataKind");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetImageDataKind"));

    VfPathElem_t pelm[] = {{SVN_ST_PROTOCOL, 0}, {SVN_ST_PROCESSED, 0}};
    pelm[SVD_ST_PROTOCOL - 1].index = protocol;
    pelm[SVD_ST_PROCESSED - 1].index = proc;
    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    VfFieldSpec_t vf_req[1] = {0};
    memset(vf_req, 0, sizeof(VfFieldSpec_t));
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008

    int data = 0;
    VFF_SET_ARGS(vf_req[0],
                 SVN_PRC_DATA_KIND,
                 0,
                 sizeof(int),
                 &data,
                 0);

    int num = 0;
    int status = m_vfdb_handle->GetFields(pelm, SVD_ST_PROCESSED, vf_req, 1, &num);

    if (status == E_NO_ERROR &&
        vf_req[0].status == E_NO_ERROR) {
        return data;
    }

    return -1;
}

//*****************************Method Header**********************************
//Method Name    :GetTopWaitPos()
//Author         :PATNI/VB
//Purpose        :This method returns 1 based position of the topmost protocol
//                having the status as wait
//****************************************************************************
int CPQMStudy::GetTopWaitPos(
    CPqmProtocol** result_acqSeries
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetTopWaitPos");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetTopWaitPos"));

    //Patni-DKH/2011Mar04/Added/TMSC-REDMINE-1486
    CScopedLock l_scoped_lock(&m_protocol_vec_mutex, CPQMLogMgr::GetInstance()); //Redmine-2520

    int position = 0;

    if (*result_acqSeries != NULL) {
        *result_acqSeries = NULL;
    }

    //Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    protocolvec::const_iterator v1_Iter = NULL;
    CPqmProtocol* protocol = NULL;

    for (v1_Iter = m_protocol_vec.begin(); v1_Iter != m_protocol_vec.end(); v1_Iter++) {
        position++;
        protocol = *v1_Iter;

        //+Patni-SS/MODIFIED/2009-Jun-04/CRASH TMSC reported
        if ((protocol != NULL) && protocol->GetAcqStatus() == C_PR_STATUS_WAIT) {
            *result_acqSeries = *v1_Iter;
            return position;
            break;

        } else {
            continue;
        }
    }

    //Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part2
    PQM_TRACE(USER_FUNC_MARKER, _T("CPQMStudy::GetTopWaitPos"), _T("Topwait protocol not found"));

    return -1;
}

//****************************Method Header************************************
//Method Name   :GetCurrentProtocol()
//Author        :PATNI/MRP
//Purpose       :This method returns the current protocol which is in the "Current"
//               condition
//****************************************************************************
CPqmProtocol* CPQMStudy::GetCurrentProtocol(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetCurrentProtocol");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetCurrentProtocol"));

    CPqmProtocol*   result_acqSeries = NULL;
    //Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    protocolvec::const_iterator v1_Iter = NULL;

    //Patni-DKH/2011Mar04/Added/TMSC-REDMINE-1486
    CScopedLock l_scoped_lock(&m_protocol_vec_mutex);

    for (v1_Iter = m_protocol_vec.begin(); v1_Iter != m_protocol_vec.end(); v1_Iter++) {
        CPqmProtocol* acqSeries = NULL;
        acqSeries = *v1_Iter;

        //+Patni-SS/MODIFIED/2009-Jun-04/CRASH TMSC reported
        if (acqSeries != NULL && acqSeries->GetAcqStatus() == C_PR_STATUS_CURRENT) {
            result_acqSeries  = acqSeries ;
            break;

        } else {
            continue;
        }
    }

    return result_acqSeries;
}

//****************************Method Header************************************
//Method Name   :GetWeight()
//Author        :PATNI/MRP
//Purpose       :This method returns the weight of the patient from the Study File
//*****************************************************************************
float CPQMStudy::GetWeight(
) const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetWeight");
    CString l_weight_str(_T(""));
    l_weight_str.Format(_T("Patient Weight = %f"), m_weight);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_weight_str);

    return m_weight;
}

//***************************Method Header*************************************
//Method Name    :GetHeight()
//Author         :PATNI/MRP
//Purpose        :This function returns the patient height
//*****************************************************************************
float CPQMStudy::GetHeight(
)const
{
    return m_height;
}

// MEITEC/2010Apr23/Added/IR86
int CPQMStudy::GetGender(
)const
{
    return m_gender;
}

//***************************Method Header*************************************
//Method Name    :GetAllItems()
//Author         :PATNI/SG
//Purpose        :
//*****************************************************************************
int CPQMStudy::GetAllItems(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetAllItems");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetAllItems"));
    return m_protocol_vec.size();
}

//+Patni-AMT/2010Apr12/Added/JaFT# IR-86 Code Optimization
//*****************************************************************************
//Method Name   : GetProtCountFromVec
//Author        : Patni/AMT
//Purpose       : Returns a protocol count from Vector size.
//*****************************************************************************
int CPQMStudy::GetProtCountFromVec(
)const
{
    return m_protocol_vec.size();
}
//-Patni-AMT/2010Apr12/Added/JaFT# IR-86 Code Optimization

// MEITEC/2010Apr23/Added/IR86
protocolvec CPQMStudy::GetProtocolvector()const
{
    return m_protocol_vec ;
}

//***************************Method Header*************************************
//Method Name    :GetFirstScan()
//Author         :PATNI/HAR-SS
//Purpose        :
//*****************************************************************************
BOOL CPQMStudy::GetFirstScan(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetFirstScan");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetFirstScan"));
    return m_first_scan;
}

//+Patni-DKH/2010Sep13/Added/TMSC-REDMINE-666
//****************************Method Header************************************
//Method Name   :GetAllProtAcqStatus()
//Author        :PATNI/DKH
//Purpose       :This method will log the acq status for all the protocols
//*****************************************************************************
void CPQMStudy::GetAllProtAcqStatus()
{

    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetAllProtAcqStatus");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CPQMStudy::GetAllProtAcqStatus"));

    //Patni-DKH/2011Mar04/Added/TMSC-REDMINE-1486
    CScopedLock l_scoped_lock(&m_protocol_vec_mutex);

    CPqmProtocol* l_pqmprot = NULL;
    protocolvec::iterator v1_Iter = NULL;
    int l_protacqstatus =  -1;
    BITFLD_DECL(mask, K_ALL_PR) = {0};
    CString l_allprotacqstatus(_T(""));
    CString l_logstr = _T("Acquisition Status of protocol ");

    memset(mask, 0, sizeof(mask));

    //Loop through the protocols
    for (v1_Iter = m_protocol_vec.begin(); v1_Iter != m_protocol_vec.end(); v1_Iter++) {

        l_allprotacqstatus = _T("");

        l_pqmprot = *v1_Iter;

        if (NULL == l_pqmprot) {
            continue;
        }

        //Get AcqStastus from varfield

        // MEITEC/2010Sep14/Modified/MVC008345
        GetProtocolValues(m_vfdb_handle,
                          l_pqmprot->GetProtocol(),
                          mask,
                          K_PR_ACQ_STATUS,
                          &l_protacqstatus,
                          NULL,
                          NULL);

        CString l_sts = _T("");

        switch (l_protacqstatus) {
            case C_PR_STATUS_WAIT:
                l_sts = _T("WAIT");
                break;

            case VFC_ACQ_STATUS_SUCCESS:
                l_sts = _T("DONE");
                break;

            case VFC_ACQ_STATUS_FAIL:
                l_sts = _T("FAIL");
                break;

            case VFC_ACQ_STATUS_CURRENT:
                l_sts = _T("CURR");
                break;

            default:
                l_sts = _T("UNKNOWN");
                break;

        }

        l_allprotacqstatus += l_logstr ;
        l_allprotacqstatus += l_pqmprot->GetScanID();
        l_allprotacqstatus += _T(" = ");
        l_allprotacqstatus += l_sts ;

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_allprotacqstatus);

    }
}
//-Patni-DKH/2010Sep13/Added/TMSC-REDMINE-666

CString CPQMStudy::GetCoilNameFromAcqOrder(
    const int f_protocol_no
)const
{
    LPCTSTR FUNC_NAME = _T(" CPQMStudy::GetCoilNameFromAcqOrder");
    CPqmProtocol* l_protocol = GetProtocol(f_protocol_no);

    //+Patni-RUP/2009Aug20/Modified/cpp test corrections
    //CString l_name(l_protocol->GetSaturnCoil()->CoilName);
    CString l_name = _T("");

    if (l_protocol == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("l_protocol pointer is NULL"));
        return l_name;
    }

    PqmSaturnCoil_t* l_pqm_saturn_coil = l_protocol->GetSaturnCoil();

    if (l_pqm_saturn_coil == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("l_pqm_saturn_coil pointer is NULL"));
        return l_name;
    }

    l_name = l_pqm_saturn_coil->CoilName;
    //-Patni-RUP/2009Aug20/Modified/cpp test corrections
    return l_name;
}

//************************************Method Header**********************************
// Method Name  : GetReceiverCoilName
// Author       : PATNI/
// Purpose      : Get the reciever coil name
//***********************************************************************************
CString CPQMStudy::GetReceiverCoilName(const int f_acq_order)const
{
    char l_recvr_coil_name[32] = {0};

    //CPqmProtocol *l_prot = GetProtocol(f_acq_order);
    //if(l_prot != NULL){
    //  if(l_prot->GetSaturnCoil() != NULL)
    {
        ClibVfToUiAdapter vuiAdapter(m_vfdb_handle);
        vuiAdapter.VuiSaturnCoilName(f_acq_order, l_recvr_coil_name);
    }
    CString l_recv_coil(l_recvr_coil_name);
    //l_recv_coil = ;

    return l_recv_coil;
    //}
}

/****************************Method Header************************************
//Method Name   :GetControlValueFromStudy()
//Author        :PATNI/
//Purpose       :For Get controls values from VarField DB
//*****************************************************************************/
BOOL CPQMStudy::GetControlValueFromStudy(
    VARIANT* f_pdata,
    const int f_scan_index
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetControlValueFromStudy");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetControlValueFromStudy"));

    CPqmProtocol* l_current_protocol = GetProtocol(f_scan_index);

    //+Patni-RUP/2009Aug21/Modified/cpp test corrections
    if (l_current_protocol == NULL) {
        PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("l_current_protocol pointer is NULL"));
        return false;
    }

    //-Patni-RUP/2009Aug21/Modified/cpp test corrections

    if ((e_control_id)f_pdata->wReserved1 == HOLD_VALUE) {
        f_pdata->intVal = l_current_protocol->GetHoldScan();
        //added on 19/123/08
        //Note: If group id is present then Breath hold menu item should be disbaled.
        f_pdata->wReserved2 = (l_current_protocol->GetDynamic() || l_current_protocol->GetMovingCouch() ||
                               l_current_protocol->GetGroupID()) ;

    } else if ((e_control_id)f_pdata->wReserved1 == SCAN_TIME) {
        f_pdata->llVal = l_current_protocol->GetScanTime();


    }

    //Added by Sarjeet and Hemant, now allowing GUI to read the updated scan id also
    //defect #760
    else if ((e_control_id)f_pdata->wReserved1 == SCAN_ID) {

        CString* scan_id = (CString*)(f_pdata->byref) ;
        (*scan_id) = l_current_protocol->GetScanID();
        //MRPdefect615

    } else if ((e_control_id)f_pdata->wReserved1 == PLAN_STATUS) {

        f_pdata->intVal = l_current_protocol->GetPlnStatus();

    }

    //MRPdefect615

    //end of addition for //defect #760

    else if (MASTER_SLAVE_HOLD == (e_control_id)f_pdata->wReserved1) {
        f_pdata->intVal = l_current_protocol->GetGroupID();
        f_pdata->wReserved1 = l_current_protocol->GetMasterFlag();
        f_pdata->wReserved2 = l_current_protocol->GetHoldScan();
        //Note: On delete protocol set the breath hold menu item to disbale/enable.
        f_pdata->wReserved3 = (l_current_protocol->GetDynamic() || l_current_protocol->GetMovingCouch() ||
                               (l_current_protocol->GetGroupID() && !l_current_protocol->GetMasterFlag()));
    }

    else {
        PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Unknown Control"));
    }


    return true;
}

//+Patni+MP/2009Jun07/Added/ACE-2/Missing Vantage Skip Prescan
//****************************Method Header************************************
//Method Name   :GetScanAgent()
//Author        :PATNI / MRP
//Purpose       :To read the Scan Agent from the Study
//*****************************************************************************
int CPQMStudy::GetScanAgent(const int f_protocol_index,
                            CString& f_scan_agent
                           )
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetScanAgent");

    CString l_str(_T(""));
    l_str.Format(_T("Protocol Index  = %d"), f_protocol_index);

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_str);

    //+Patni+MP/2009Jun23/Modified/ACE-2/TA-Review comments
    VfFieldSpec_t   l_vfField[1] = {0};
    VfPathElem_t    l_path[2] = {0};
    //-Patni+MP/2009Jun23/Modified/ACE-2/TA-Review comments

    //+Patni+MP/2009Jun23/commented/ACE-2/TA-Review comments
    //memset(l_path,0,sizeof(VfPathElem_t) *2);
    //memset(l_vfField,0,sizeof(VfFieldSpec_t));
    //+Patni+MP/2009Jun23/Modified/ACE-2/TA-Review comments

    l_path[0].subtree_name = SVN_ST_PROTOCOL;
    l_path[0].index = f_protocol_index;

    VFF_SET_ARGS(l_vfField[0],
                 SVN_EXE_SCAN_AGENT,
                 SVT_EXE_SCAN_AGENT,
                 0,
                 NULL,
                 VFO_ALLOCATE | VFO_VARIABLE_LEN);

    int             l_numDone = 0;
    status_t l_status = m_vfdb_handle->GetFields(l_path, SVN_ST_PROTOCOL, &l_vfField[0],
                        1, &l_numDone);

    if (l_status != E_NO_ERROR) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("CPQMStudy::GetFields failed"));
        f_scan_agent = _T("");
        FREE_PTR(l_vfField[0].buffer);
        return E_ERROR;
    }

    if ((l_vfField[0].buffer == NULL) || (l_vfField[0].size == 0)) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                  _T("CPQMStudy::GetScanAgent - Value not found in study"));
        f_scan_agent = _T("");
        return NO_FIELD;

    } else {
        char* scanagent = new char[l_vfField[0].size];

        //+Patni-RUP/2009Aug25/Modified/cpp test corrections
        if (scanagent == NULL) {
            PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("scanagent pointer is NULL"));
            f_scan_agent = _T("");
            return E_ERROR;
        }

        //-Patni-RUP/2009Aug25/Modified/cpp test corrections
        strncpy(scanagent, (char*)l_vfField[0].buffer, l_vfField[0].size);
        l_str = scanagent;
        f_scan_agent = l_str;
        FREE_PTR(l_vfField[0].buffer);

        //Patni+MP/2009Jun23/Modified/ACE-2/TA-Review comments
        DEL_PTR_ARY(scanagent);

    }


    return l_status;
}

//+Patni-Sudhir/2011Jan13/Added/Redmine-434
//****************************Method Header************************************
//Method Name   :GetNRFilterStringForProtocol()
//Author        :PATNI/Sudhir
//Purpose       : To get the Filter ID of the selected protocol
//*****************************************************************************
int CPQMStudy::GetNRFilterStringForProtocol(const int f_acq_order, const int f_NRFLG, VARIANT* f_NRVal)const
{

    int		kspaceNo = 0; // K-SPACE Filter
    int		filterNo = 0; // Refine Filter or GOP Filter

    VfFieldSpec_t	vf_req[2] = {0};

    if (f_NRFLG) {
        VFF_SET_ARGS(vf_req[0],
                     SVN_PRC_K_SPACE_FILTER_NR,
                     SVT_PRC_K_SPACE_FILTER_NR,
                     sizeof(int32_t), &kspaceNo, 0);
        VFF_SET_ARGS(vf_req[1],
                     SVN_PRC_GOP_FILTER,
                     SVT_PRC_GOP_FILTER,
                     sizeof(int32_t), &filterNo, 0);

    } else {
        VFF_SET_ARGS(vf_req[0],
                     SVN_PRC_K_SPACE_FILTER,
                     SVT_PRC_K_SPACE_FILTER,
                     sizeof(int32_t), &kspaceNo, 0);
        VFF_SET_ARGS(vf_req[1],
                     SVN_PRC_REFINE_FILTER,
                     SVT_PRC_REFINE_FILTER,
                     sizeof(int32_t), &filterNo, 0);

    }

    VfPathElem_t    l_pelm[] = {
        {SVN_ST_PROTOCOL,       0},
        {SVN_ST_SUBPROTOCOL,    0}
    };


    l_pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_pelm[SVD_ST_PROTOCOL - 1].index = f_acq_order;
    int			num_done = 0;

    status_t l_status = m_vfdb_handle->GetFields(l_pelm, PROTOCOLLEVEL, vf_req,
                        2, &num_done);

    if (num_done != 2 || l_status != E_NO_ERROR || vf_req[0].status || vf_req[1].status) {
        return E_ERROR;
    }

    char l_comment[256] = "";
    char* commentP = l_comment;

    if (f_NRFLG) {
        l_status = msGetNRcomment(kspaceNo, filterNo, &commentP);

    } else {
        l_status = msGetRefineComment(kspaceNo, filterNo, &commentP);
    }

    if (l_status == E_NO_ERROR) {
        f_NRVal->pcVal = commentP;

    } else {
        return E_ERROR;
    }

    return E_NO_ERROR;
}
//-Patni-Sudhir/2011Jan13/Added/Redmine-434

//****************************Method Header************************************
//Method Name   :GetEMTFilterStringForProtocol()
//Author        :
//Purpose       :
//*****************************************************************************
int CPQMStudy::GetEMTFilterStringForProtocol(const int f_acq_order, VARIANT* f_EMTVal)const
{

    const int l_total_values = 2;
    int	l_filterNo = 0;
    int l_emtflag = 0;

    VfFieldSpec_t	vf_req[l_total_values] = {0};

    VFF_SET_ARGS(vf_req[0],
                 SVN_PRC_EMTONE_ID,
                 SVT_PRC_EMTONE_ID,
                 sizeof(l_filterNo), &l_filterNo, 0);

    VFF_SET_ARGS(vf_req[1],
                 SVN_PRC_EMTONE_FLAG,
                 SVT_PRC_EMTONE_FLAG,
                 sizeof(l_emtflag), &l_emtflag, 0);


    VfPathElem_t    l_pelm[] = {
        {SVN_ST_PROTOCOL,       0},
        {SVN_ST_SUBPROTOCOL,    0}
    };


    l_pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_pelm[SVD_ST_PROTOCOL - 1].index = f_acq_order;

    int num_done = 0;
    status_t l_status = m_vfdb_handle->GetFields(l_pelm, PROTOCOLLEVEL, vf_req,
                        l_total_values, &num_done);

    if ((num_done != l_total_values)
        || (E_NO_ERROR != l_status)
        || (E_NO_ERROR != vf_req[0].status)
        || (E_NO_ERROR != vf_req[1].status)
       ) {
        return E_ERROR;
    }

    char l_comment[256] = "";
    char* commentP = l_comment;
    l_status = msGetEMToneComment(l_filterNo, &commentP);

    if (l_status != E_NO_ERROR) {
        return E_ERROR;
    }

    CString* l_str = reinterpret_cast<CString*>(f_EMTVal->byref);
    (*l_str) = commentP;

    f_EMTVal->intVal = l_emtflag;

    return E_NO_ERROR;
}
CPqmProtocol* CPQMStudy:: GetFirstProtocol(
)
{
    //Patni-DKH/2011Mar04/Added/TMSC-REDMINE-1486
    CScopedLock l_scoped_lock(&m_protocol_vec_mutex);

    return (m_protocol_vec.begin() != m_protocol_vec.end() ? (*m_protocol_vec.begin()) : NULL);
}

//+Patni-AMT/2010Feb16/Modified/Ph-3# PROPOSAL_07
//****************************Method Header************************************
//Method Name   : GetNextItemFromPos()
//Author        : PATNI/HAR
//Purpose       : This method returns the next item found after the given position
//*****************************************************************************
CPqmProtocol* CPQMStudy::GetNextItemFromPos(
    CPqmProtocol* f_obj
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetNextItemFromPos");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetNextItemFromPos"));

    if (NULL == f_obj) {
        return NULL;
    }

    CPqmProtocol* l_next_prot = NULL;
    CString       l_log_msg = _T("");
    const int l_cur_pos = f_obj->GetPosition();

    //+Patni-AMT/2010Apr19/Modified/JaFT# PROPOSAL7_CDR_01
    //+Patni-PJS/2010Feb18/Added/DeFT# PROPOSAL_07
    try {
        l_next_prot = m_protocol_vec.at(l_cur_pos);
        l_log_msg.Format(_T("Cur. Pos=%d, Next Pos=%d"), l_cur_pos, l_next_prot->GetPosition());

    } catch (std::out_of_range&) {
        l_log_msg.Format(_T("out of range. Cur. Pos=%d"), l_cur_pos);
    }

    //-Patni-PJS/2010Feb18/Added/DeFT# PROPOSAL_07
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_msg);
    return l_next_prot;
    //-Patni-AMT/2010Apr19/Modified/JaFT# PROPOSAL7_CDR_01
}
//-Patni-AMT/2010Feb16/Modified/Ph-3# PROPOSAL_07

//****************************Method Header************************************
//Method Name   :GetPrevItemFromPos()
//Author        :PATNI/HAR
//Purpose       :This method returns the previous series from the given object
//*****************************************************************************
//+Patni-AMT/2010Feb16/Modified/Ph-3# PROPOSAL_07
CPqmProtocol* CPQMStudy::GetPrevItemFromPos(
    CPqmProtocol* f_obj
)const
{
    //Patni-AMT/2010Apr16/Modified/JaFT# COMMON_CDR_07
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetPrevItemFromPos");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetPrevItemFromPos"));

    if (NULL == f_obj) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_obj is NULL"));
        return NULL;
    }

    //Patni-DKH/2011Mar04/Added/TMSC-REDMINE-1486
    CScopedLock l_scoped_lock(&m_protocol_vec_mutex);

    const int l_cur_pos = f_obj->GetPosition();

    // l_cur_pos is position which is 1 based while vector is 0 based. To get
    // previous item from position, we have to 1st subtract 1 to convert it to 0
    // based. Again we have to subtract 1 to get previous item

    //+Patni-AMT/2010Apr19/Modified/JaFT# PROPOSAL7_CDR_02
    //+Patni-PJS/2010Feb18/Added/DeFT# PROPOSAL_07
    CString       l_log_msg = _T("");
    CPqmProtocol* l_prev_obj = NULL;

    try {
        l_prev_obj = m_protocol_vec.at(POS2INDEX(l_cur_pos) - 1);
        l_log_msg.Format(_T("Cur Pos=%d, Prev Pos=%d"), l_cur_pos, l_prev_obj->GetPosition());

    } catch (std::out_of_range&) {
        l_log_msg.Format(_T("out of range. Cur. Pos=%d"), l_cur_pos);
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_msg);
    //-Patni-PJS/2010Feb18/Added/DeFT# PROPOSAL_07
    //-Patni-AMT/2010Apr19/Modified/JaFT# PROPOSAL7_CDR_02


    return l_prev_obj;
}
//-Patni-AMT/2010Feb16/Modified/Ph-3# PROPOSAL_07

//***************************Method Header*************************************
//Method Name    :GetNextItem()
//Author         :PATNI/HAR-SS
//Purpose        :
//*****************************************************************************
CPqmProtocol* CPQMStudy::GetNextItem(
    CPqmProtocol* protocol
)const
{
    //+ MEITEC/2010Jun23/Modified/MVC008324_1
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetNextItem");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetNextItem"));
    //- MEITEC/2010Jun23/Modified/MVC008324_1

    if (protocol == NULL) {
        return NULL;
    }

    //Patni-DKH/2011Mar04/Added/TMSC-REDMINE-1486
    CScopedLock l_scoped_lock(&m_protocol_vec_mutex);

    CPqmProtocol* result_protocol = NULL;
    //Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    protocolvec::const_iterator v1_Iter = NULL;

    for (v1_Iter = m_protocol_vec.begin(); v1_Iter != m_protocol_vec.end(); v1_Iter++) {
        result_protocol = *v1_Iter;

        //+Patni-SS/MODIFIED/2009-Jun-04/CRASH TMSC reported
        if (result_protocol != NULL && result_protocol->GetProtocol() == protocol->GetProtocol()) {
            v1_Iter++;

            for (; v1_Iter != m_protocol_vec.end(); v1_Iter++) {
                result_protocol = *v1_Iter;

                //+Patni-SS/MODIFIED/2009-Jun-04/CRASH TMSC reported
                if (result_protocol != NULL  && result_protocol->GetProtocol() != PQ_FILLER) {
                    return result_protocol;
                }
            }

            break;
        }
    }

    return NULL;
}

//***************************Method Header*************************************
//Method Name    :GetNextWaitProtocol()
//Author         :PATNI/HAR-SS
//Purpose        :
//*****************************************************************************
CPqmProtocol* CPQMStudy::GetNextWaitProtocol(
    CPqmProtocol* protocol
)const
{
    //+ MEITEC/2010Jun23/Modified/MVC008324_1
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetNextWaitProtocol");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetNextWaitProtocol"));
    //+ MEITEC/2010Jun23/Modified/MVC008324_1

    //+Patni-SS/MODIFIED/2009-Jun-04/CRASH TMSC reported
    if (protocol == NULL)
        return NULL;

    //Patni-DKH/2011Mar04/Added/TMSC-REDMINE-1486
    CScopedLock l_scoped_lock(&m_protocol_vec_mutex);

    CPqmProtocol* result_protocol = NULL;
    //Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    protocolvec::const_iterator v1_Iter = NULL;

    for (v1_Iter = m_protocol_vec.begin(); v1_Iter != m_protocol_vec.end(); v1_Iter++) {
        result_protocol = *v1_Iter;

        //+Patni-SS/MODIFIED/2009-Jun-04/CRASH TMSC reported
        if (result_protocol != NULL && result_protocol->GetProtocol() == protocol->GetProtocol()) {
            v1_Iter++;

            for (; v1_Iter != m_protocol_vec.end(); v1_Iter++) {
                result_protocol = *v1_Iter;

                //+Patni-SS/MODIFIED/2009-Jun-04/CRASH TMSC reported
                if (result_protocol != NULL && result_protocol->GetAcqStatus() == C_PR_STATUS_WAIT) {
                    return result_protocol;
                }
            }

            break;
        }
    }

    return NULL;
}

//****************************Method Header************************************
//Method Name   :GetSubProtValues()
//Author        :PATNI/HAR - SS
//Purpose       :Get sub-protocol data. If buffer is NULL,this subroutine
//                  allocate the area.
//*****************************************************************************
BOOL CPQMStudy::GetSubProtValues(
    int     prot,
    int     sub_prot,
    bitfield_t*  mask,

    ...
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetSubProtValues");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetSubProtValues"));

    VfPathElem_t    pelm[] = {
        {SVN_ST_PROTOCOL,   0},
        {SVN_ST_SUBPROTOCOL,    0}
    };
    va_list     args;   /* variable argument list */
    //+Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    unsigned long vf_parm = 0, vf_parm2 = 0;
    int         tag = 0, n = 0;
    VfFieldSpec_t   vfReq[K_ALL_PR] = {0};
    VfSize_t*        vfSize[K_ALL_PR] = {0};
    int         tagList[K_ALL_PR] = {0};
    //-Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution

    pelm[SVD_ST_PROTOCOL - 1].index = prot;
    pelm[SVD_ST_SUBPROTOCOL - 1].index = sub_prot;

    vf_parm2 = VFO_ALLOCATE | VFO_VARIABLE_LEN;

    /* get list of fields to be retrieved */
    va_start(args, mask);

    while (tag = va_arg(args, int)) {
        VfName_t    vf_name = 0;
        VfType_t    vf_type = 0;
        VfSize_t    vf_size = 0;
        //+Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
        void**        vf_ap = va_arg(args, void**);
        int     prot_tag = 0;
        //-Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution

        VfSize_t* vf_size_p = va_arg(args, VfSize_t*);

        if ((prot_tag = GetInitNumber(tag)) >= 0) {
            vf_name = pr_init[prot_tag].name;
            vf_type = pr_init[prot_tag].type;
            vf_size = pr_init[prot_tag].size;

        } else {
            //+MEITEC/2010Sep29/Modified output format.
            CString str(_T(""));
            str.Format(L"Illegal request!!! -> prot_tag=%d\n", prot_tag);
            //-MEITEC/2010Sep29/Modified output format.
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str);
            continue;
        }

        if (vf_size_p != NULL) {
            vf_size = *vf_size_p;
            VFF_SET_ARGS(vfReq[n], vf_name, vf_type, vf_size, vf_ap, vf_parm2);

        } else {
            VFF_SET_ARGS(vfReq[n], vf_name, vf_type, vf_size, vf_ap, vf_parm);
        }

        vfSize[n] = vf_size_p;
        tagList[n] = tag;
        n++;

        if (n > MAX_FIELDS) {
            return E_ERROR;
        }
    }

    va_end(args);
    int num = 0, i = 0;
    status_t        status  = E_NO_ERROR;

    if (n) {
        status = m_vfdb_handle->GetFields(pelm, SVD_ST_SUBPROTOCOL, vfReq, n, &num);

        for (i = 0; i < n; i++) {
            if (vfReq[i].status == VFE_NO_ERROR) {
                if (vfSize[i] != NULL) {
                    *(vfSize[i]) = vfReq[i].size;
                }
            }
        } /* for(;;) */
    }

    if (num < n) {
        for (i = 0; i < n; i++) {
            if (vfReq[i].status != VFE_NO_ERROR) {
                BITFLD_SET(mask, tagList[i]);

            } else {
                BITFLD_RESET(mask, tagList[i]);
            }
        }
    }

    return(status == E_NO_ERROR && num < n ? E_ERROR : status);
}

//****************************Method Header************************************
//Method Name   :GetProtocolItemValues()
//Author        :PATNI/HAR - SS
//Purpose       : Get some values i.e., Acq Status,Scan Time,Scan Mode.
//*****************************************************************************
BOOL CPQMStudy::GetProtocolItemValues(
    CVarFieldHandle* const f_vf_handle,
    CPqmProtocol*    pr_obj_p
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetProtocolItemValues");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetProtocolItemValues"));

    BITFLD_DECL(mask, K_ALL_PR);

    if (pr_obj_p == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, L"pr_obj_p is NULL\n");
        return  FALSE;
    }

    //+MEITEC/2010Sep29/Modified output format.
    CString str(_T(""));
    str.Format(L"prot=%d \n", pr_obj_p->GetProtocol());
    //-MEITEC/2010Sep29/Modified output format.
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str);

    if (pr_obj_p->GetProtocol() == PQ_FILLER) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, L"Protocol is FILLER.\n");
        return  FALSE;
    }

    const int com_size = SCAN_COMMENT_LENGTH * 2; //Tanoue/MVC5849

    const int id_size = SCAN_ID_LENGTH * 2; //Tanoue/MVC5849


    VfPathElem_t    path[PATH_BUFFER] = {0};

    int imggr_size = sizeof(int32_t) * 2;

    int path_size = sizeof(path);

    memset(path, 0, sizeof(path));

    memset(mask, 0, sizeof(mask));

    //+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
    int l_as_compass_flag = 0 ;

    int l_as_compass_flag_size = sizeof(l_as_compass_flag);

    //-Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement

    int     l_app_code = 0, l_acq_status = 0;

    time_t  l_time = 0;

    int     l_hold_scan = VFC_HOLD_PRE_SCAN, l_auto_hold = 0, l_auto_transfer = 0;

    int     l_image_gr[2] = {0, 0};

    int     l_receive_coil_id = 0, l_anatomy = 0, l_scan_number = 0, l_contrast = 0;

    float   l_couch_position = PQM_COUCH_INIT_POSITION;

    float   l_home_position = PQM_COUCH_INIT_POSITION;

    float   l_couch_offset = 0.0f;

    int     l_view_dir = 0, l_gating_method = 0;

    BOOL    l_dynamic = FALSE, l_fluoro = FALSE;

    int     l_intermit_flag = 0, l_intermit_time = 0, l_visual_prep = 0;

    int     l_moving_couch = 0, l_shimming_sequence = 0;

    //Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    char    l_scan_id[id_size + 1] = "", l_scan_comment[com_size + 1] = "";

    int     l_dBdt_mode_2002 = 0, l_pln_auto_voice = 0, l_exe_auto_voice = 0;

    int     l_speeder_type = 0, l_acq_split_mode = 0, l_intensity_type = 0;

    int     l_auto_pro_edit = 0, l_probe_scan = 0, l_dBdt_mode = 0;

    int     l_local_cf = 0, l_probe_correction = 0, l_pln_status = 0;

    int     l_amb_enable_flag = 0, l_amb_mode_flag = 0;


    //Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    char    l_contrast_name[com_size + 1] = "";

    float   l_sar = 0.0f;

    //Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    int     l_tr = 0;

    VftPatori_t l_patori;

    int         l_num_section = -1;

    int loid_size = PQM_LOID_LEN;

    //Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    char    l_series_loid_var[PQM_LOID_LEN] = "";


    //Patni-DKH/2010Feb18/Added/Phase3#MCM0229-00187
    float   l_rflcouch = 0.0f;

    float   l_cfacouch = 0.0f;

    //Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image
    int     l_couchMethodFlg = 0 , l_copySourceProt = 0;

    //Patni-HAR/2010Apr26/Added/IR-134
    int l_mrs_eddy_axis = 0;    //Patni-HAR/2010May07/Modified/IR-134

    //Patni/AM/2010July20/Added/NFD001-AutoShim
    int l_shimm_require_flag = -1;

    int l_map_required = -1;

    //Patni-PJS/2011Feb09/Added/MVC009736
    int l_num_of_coverages = 0;

    memset(l_scan_comment, 0, com_size + 1);

    memset(l_contrast_name, 0, com_size + 1);

    memset(l_scan_id, 0, id_size + 1);

    memset(l_series_loid_var, 0, PQM_LOID_LEN);

    bool l_cala_enable_flag = false;

    int l_cala_invoke_mode = 0;

    int l_cala_planned_status = 0;

    bool l_hala_enable_flag = false;

    int l_hala_invoke_mode = 0;

    int l_hala_planned_status = 0;

    int l_intensity_corr_status = 0;

    int l_coil_channel_mode = -1;

    CString		l_delay_time(_T(""));

    int			l_group_id = 0;

    int			l_group_num = 0;

    BOOL		l_master_flag = 0;

    BOOL		l_multi_series_enable_flag = 0;

    BOOL        l_auto_get_interval_flag = 0;


    GetProtocolValues(f_vf_handle,
                      pr_obj_p->GetProtocol(),
                      mask,
                      K_PR_APP_CODE,            &l_app_code,                NULL,
                      K_PR_ACQ_STATUS,          &l_acq_status,              NULL,
                      K_PR_TOTAL_TIME,          &l_time,                    NULL,
                      K_PR_HOLD,                &l_hold_scan,               NULL,
                      K_PR_AUTO_HOLD,           &l_auto_hold,               NULL,
                      K_PR_AUTO_POSTPROC_FLG,   &l_auto_transfer,           NULL,
                      K_PR_IMG_GR,              l_image_gr,                 &imggr_size,
                      K_PR_RC_COIL,             &l_receive_coil_id,         NULL,
                      K_PR_ANATOMY,             &l_anatomy,                 NULL,
                      K_PR_SCAN_NUMBER,         &l_scan_number,             NULL,
                      K_PR_COUCH,               &l_couch_position,          NULL,
                      K_PR_HOME_POSITION,       &l_home_position,           NULL,
                      K_PR_COUCH_OFFSET,        &l_couch_offset,            NULL,
                      K_PR_CONTRAST,            &l_contrast,                NULL,
                      K_PR_VIEW_DIR,            &l_view_dir,                NULL,
                      K_PR_GATE,                &l_gating_method,           NULL,
                      K_PR_DYNAMIC,             &l_dynamic,                 NULL,
                      K_PR_INTERMIT_FLAG,       &l_intermit_flag,           NULL,
                      K_PR_INTERMIT_TIME,       &l_intermit_time,           NULL,
                      K_PR_PREP_SCAN,           &l_visual_prep,             NULL,
                      K_PR_MOVING_COUCH,        &l_moving_couch,            NULL,
                      K_PR_SEQ_SHIM,            &l_shimming_sequence,       NULL,
                      K_PR_SEQ_FLUORO,          &l_fluoro,                  NULL,
                      K_PR_SCAN_ID,             l_scan_id,                  &id_size,
                      K_PR_SCAN_COMMENTS,       l_scan_comment,             &com_size,
                      K_PR_LOCATOR_IMAGE,       path,                       &path_size,
                      K_PR_DBDT_MODE_2002,      &l_dBdt_mode_2002,          NULL,
                      K_PR_PLN_AUTO_VOICE,      &l_pln_auto_voice,          NULL,
                      K_PR_EXE_AUTO_VOICE,      &l_exe_auto_voice,          NULL,
                      K_PR_SPEEDER_TYPE,        &l_speeder_type,            NULL,
                      K_PR_ACQ_SPLIT_MODE,      &l_acq_split_mode,          NULL,
                      K_PR_INTENSITY_CORR_FLAG, &l_intensity_type,          NULL,
                      K_PR_AUTO_PROEDIT_FLAG,   &l_auto_pro_edit,           NULL,
                      K_PR_PROBE_FLAG,          &l_probe_scan,              NULL,
                      K_PR_DBDT_MODE,           &l_dBdt_mode,               NULL,
                      K_PR_LOCAL_CF_FLAG,       &l_local_cf,                NULL,
                      K_PR_PROBE_CORRECTION,    &l_probe_correction,        NULL,
                      K_PR_STATUS,              &l_pln_status,              NULL,
                      K_PR_AMB_ENABLE_FLG,    &l_amb_enable_flag,         NULL,
                      K_PR_AMB_MODE_FLG,      &l_amb_mode_flag,           NULL,
                      K_PR_NUM_SECTION_RECVR_COIL, &l_num_section,          NULL,
                      K_PR_SAR,                 &l_sar,                     NULL,
                      K_PR_TR,                  &l_tr,                      NULL,
                      K_PR_CONTRAST_NAME,       l_contrast_name,            &com_size,
                      K_PR_PATIENT_ORIENT,      &l_patori,                  NULL,
                      K_PR_LDB_MAPPED_ID,       &l_series_loid_var,         &loid_size,
                      //+Patni-Manish/2010Feb11/Added/MCM0229-00122/ParentImage
                      K_PR_MOVE_COUCH_METHOD_FLAG,  &l_couchMethodFlg,      NULL,
                      K_PR_COPY_SOURCE_PROTOCOL,    &l_copySourceProt,      NULL,
                      K_PR_RFL_COUCH,           &l_rflcouch,                NULL,   /* V9.50 */
                      K_PR_CFA_COUCH,           &l_cfacouch,                NULL,   /* V9.50 */
                      //+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
                      K_PR_CDS_FLAG,          &l_as_compass_flag,    &l_as_compass_flag_size,
                      //-Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
                      //Patni-HAR/2010Apr26/Added/IR-134
                      K_PR_MRS_EDDY_AXIS,       &l_mrs_eddy_axis,       NULL,
                      //Patni/AM/2010July20/Added/NFD001-AutoShim
                      K_PR_SHIM_REQUIRED,       &l_shimm_require_flag,       NULL,
                      K_PR_MAP_REQUIRED,        &l_map_required,            NULL,
                      //Patni-PJS/2011Feb09/Added/MVC009736
                      K_PR_COVERAGES,           &l_num_of_coverages,        NULL,
                      K_PR_CALA_ENABLE_FLAG,       &l_cala_enable_flag,     NULL,
                      K_PR_CALA_INVOKE_MODE,       &l_cala_invoke_mode,     NULL,
                      K_PR_CALA_PLANNED_STATUS,    &l_cala_planned_status,  NULL,
                      K_PR_INTENSITY_CORR_STATUS, &l_intensity_corr_status, NULL,
                      K_PR_MULTI_SERIES_DYNAMIC_ENABLE_FLAG, &l_multi_series_enable_flag, NULL,
                      K_PR_MAX_COIL_CHANNEL_MODE, &l_coil_channel_mode, NULL,
                      K_PR_AUTO_GET_INTERVAL_FLAG, &l_auto_get_interval_flag, NULL,
                      K_PR_HALA_ENABLE_FLAG,       &l_hala_enable_flag,     NULL,
                      K_PR_HALA_INVOKE_MODE,       &l_hala_invoke_mode,     NULL,
                      K_PR_HALA_PLANNED_STATUS,    &l_hala_planned_status,  NULL,
                      NULL);

    //Patni/AM/2010July20/Added/NFD001-AutoShim
    pr_obj_p->SetRequireShimming(l_shimm_require_flag);

    //-Patni/AM/2010July20/Added/NFD001-AutoShim

    pr_obj_p->SetMapRequired(l_map_required);

    //set the  protocols data
    pr_obj_p->GetPqmPrMode()->SetPatoriEndIn(l_patori.end_in);

    pr_obj_p->GetPqmPrMode()->SetPatoriSideUp(l_patori.side_up);

    pr_obj_p->SetAppCode(l_app_code);

    pr_obj_p->SetAcqStatus(l_acq_status);

    pr_obj_p->SetHoldScan(l_hold_scan);

    pr_obj_p->SetAutoHold(l_auto_hold);

    pr_obj_p->SetAutoTransfer(l_auto_transfer);

    pr_obj_p->GetPqmPrMode()->SetRecvCoilId(l_receive_coil_id);

    pr_obj_p->GetPqmPrMode()->SetAnatomy(l_anatomy);

    CString tmpStr(_T(""));

    tmpStr.Format(_T("pr_obj_p->GetPqmPrMode()->SetAnatomy(l_anatomy) = %d"), l_anatomy);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, tmpStr);

    pr_obj_p->GetPqmPrMode()->SetScanNumber(l_scan_number);

    pr_obj_p->GetPqmPrMode()->SetCouchPos(l_couch_position);

    pr_obj_p->GetPqmPrMode()->SetHomePos(l_home_position);

    pr_obj_p->GetPqmPrMode()->SetCouchOffset(l_couch_offset);

    pr_obj_p->GetPqmPrMode()->SetContrast(l_contrast);

    pr_obj_p->GetPqmPrMode()->SetViewDir(l_view_dir);

    //MRP for BodyPart
    //+Patni-MP/2009Mar17/Added/ACE-1/MCM0213-00131
    CString l_bodypart = _T("");

    BodyPartGetVF(pr_obj_p->GetProtocol(), l_bodypart);

    pr_obj_p->GetPqmPrMode()->SetBodyPart(l_bodypart);

    //-Patni-MP/2009Mar17/Added/ACE-1/MCM0213-00131

    //+Patni-Hemant/2010Apr27/Added/JaFT/IR-137
    int l_laterality_val = -1;

    GetLaterality(pr_obj_p->GetProtocol(), l_laterality_val);

    pr_obj_p->GetPqmPrMode()->SetLaterality(l_laterality_val);

    //-Patni-Hemant/2010Apr27/Added/JaFT/IR-137

    pr_obj_p->SetGatingMethod(l_gating_method);

    pr_obj_p->SetDynamic(l_dynamic);

    pr_obj_p->SetIntermitFlag(l_intermit_flag);

    pr_obj_p->SetIntermitTime(l_intermit_time);

    pr_obj_p->SetPrepScan(l_visual_prep);

    pr_obj_p->SetMovingCouch(l_moving_couch);

    pr_obj_p->SetShimming(l_shimming_sequence);

    pr_obj_p->SetFluoro(l_fluoro);

    pr_obj_p->SetScanID(l_scan_id);

    pr_obj_p->SetScanComment(l_scan_comment);

    pr_obj_p->SetdBdtMode2002(l_dBdt_mode_2002);

    pr_obj_p->SetPlnAutoVoice(l_pln_auto_voice);

    pr_obj_p->SetExeAutoVoice(l_exe_auto_voice);

    pr_obj_p->SetSpeederType(l_speeder_type);

    pr_obj_p->SetAcqSplitMode(l_acq_split_mode);

    pr_obj_p->SetIntensityType(l_intensity_type);

    pr_obj_p->SetAutoProEdit(l_auto_pro_edit);

    pr_obj_p->SetProbeScan(l_probe_scan);

    pr_obj_p->SetdBdtMode(l_dBdt_mode);

    pr_obj_p->SetLocalCF(l_local_cf);

    pr_obj_p->SetProbeCorrection(l_probe_correction);

    pr_obj_p->SetPlnStatus(l_pln_status);

    pr_obj_p->SetScanTime(l_time);

    l_delay_time = GetDelayTimeFromVf(pr_obj_p->GetProtocol());

    pr_obj_p->SetDelayTime(l_delay_time);

    pr_obj_p->SetMultiSeriesDynamicFlag(l_multi_series_enable_flag);

    l_master_flag = GetMasterFlagfromVF(pr_obj_p->GetProtocol());

    pr_obj_p->SetMasterFlag(l_master_flag);

    l_group_id = GetGroupIDfromVF(pr_obj_p->GetProtocol());

    if (l_group_id && !l_master_flag) {
        SetMasterFlagToVF(pr_obj_p, FALSE);
    }

    pr_obj_p->SetGroupID(l_group_id);

    l_group_num = GetGroupNumFromVF();

    m_pqm_ptr->GetMasterSlavePtr()->SetGroupNum(l_group_num);

    pr_obj_p->SetAMBEnableFlag(l_amb_enable_flag);
    pr_obj_p->SetAMBModeFlag(l_amb_mode_flag);
    pr_obj_p->GetSaturnCoil()->numSection = l_num_section;

    pr_obj_p->GetSarInfo()->tsar[0].sar = l_sar;

    pr_obj_p->SetTrValue(l_tr);

    pr_obj_p->SetContrastName(l_contrast_name);

    //Added by Meghana On 8/11/2008 10:54:45 AM
    wchar_t* serloid = new wchar_t[PQM_LOID_LEN];

    mbstowcs(serloid, l_series_loid_var, PQM_LOID_LEN);

    CString l_seriesloid(serloid);

    //AfxMessageBox(l_seriesloid);
    pr_obj_p->SetSeriesLOID(l_seriesloid);

    //+Patni-Manish/2010Feb11/Added/MCM0229-00122/ParentImage
    pr_obj_p->SetcouchMethodFlg(l_couchMethodFlg);

    pr_obj_p->SetCopySourceProt(l_copySourceProt);

    //-Patni-Manish/2010Feb11/Added/MCM0229-00122/ParentImage
    //+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
    pr_obj_p->SetASCOMPASSFlag(l_as_compass_flag);

    //-Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
    //Patni-DKH/2010Feb18/Added/Phase3#MCM0229-00187
    pr_obj_p->SetRflPos(l_rflcouch);

    pr_obj_p->SetCfaPos(l_cfacouch);

    pr_obj_p->SetProtcolCALAEnabled(l_cala_enable_flag);
    pr_obj_p->SetProtcolHALAEnabled(l_hala_enable_flag);
    pr_obj_p->SetProtcolCorrStatus(l_intensity_corr_status);
    pr_obj_p->SetcoilChannelMode(l_coil_channel_mode);

    pr_obj_p->SetAutoGetIntervalFlag(l_auto_get_interval_flag);

    switch (l_cala_invoke_mode) {

        case VFC_CALA_INVOKE_MODE_NONE:
            pr_obj_p->SetCALAInvokeMode(INVOKE_MODE_NONE);
            break;

        case VFC_CALA_INVOKE_MODE_FOREGROUND:
            pr_obj_p->SetCALAInvokeMode(INVOKE_MODE_FOREGROUND);
            break;

        case VFC_CALA_INVOKE_MODE_BACKGROUND:
            pr_obj_p->SetCALAInvokeMode(INVOKE_MODE_BACKGROUND);
            break;

        default:
            pr_obj_p->SetCALAInvokeMode(INVOKE_MODE_NONE);

    }

    switch (l_hala_invoke_mode) {

        case VFC_ALA_INVOKE_MODE_NONE:
            pr_obj_p->SetHALAInvokeMode(INVOKE_MODE_NONE);
            break;

        case VFC_ALA_INVOKE_MODE_FOREGROUND:
            pr_obj_p->SetHALAInvokeMode(INVOKE_MODE_FOREGROUND);
            break;

        case VFC_ALA_INVOKE_MODE_BACKGROUND:
            pr_obj_p->SetHALAInvokeMode(INVOKE_MODE_BACKGROUND);
            break;

        default:
            pr_obj_p->SetHALAInvokeMode(INVOKE_MODE_NONE);

    }

    switch (l_hala_planned_status) {

        case VFC_ALA_PLANNED_STATUS_NONE:
            pr_obj_p->SetHalaPlannedStatus(PLANNED_STATUS_NONE);
            break;

        case VFC_ALA_PLANNED_STATUS_NOVECTOR:
            pr_obj_p->SetHalaPlannedStatus(PLANNED_STATUS_NOVECTOR);
            break;

        case VFC_ALA_PLANNED_STATUS_SCAN_STARTED:
            pr_obj_p->SetHalaPlannedStatus(PLANNED_STATUS_STARTED);
            break;

        case VFC_ALA_PLANNED_STATUS_SCAN_PLANNED:
            pr_obj_p->SetHalaPlannedStatus(PLANNED_STATUS_PLANNED);
            break;

        case VFC_ALA_PLANNED_STATUS_SCAN_EDITED:
            pr_obj_p->SetHalaPlannedStatus(PLANNED_STATUS_EDITED);
            break;

        default:
            pr_obj_p->SetHalaPlannedStatus(PLANNED_STATUS_NONE);

    }

    switch (l_cala_planned_status) {

        case VFC_CALA_PLANNED_STATUS_NONE:
            pr_obj_p->SetCalaPlannedStatus(PLANNED_STATUS_NONE);
            break;

        case VFC_CALA_PLANNED_STATUS_NOVECTOR:
            pr_obj_p->SetCalaPlannedStatus(PLANNED_STATUS_NOVECTOR);
            break;

        case VFC_CALA_PLANNED_STATUS_SCAN_STARTED:
            pr_obj_p->SetCalaPlannedStatus(PLANNED_STATUS_STARTED);
            break;

        case VFC_CALA_PLANNED_STATUS_SCAN_PLANNED:
            pr_obj_p->SetCalaPlannedStatus(PLANNED_STATUS_PLANNED);
            break;

        case VFC_CALA_PLANNED_STATUS_SCAN_EDITED:
            pr_obj_p->SetCalaPlannedStatus(PLANNED_STATUS_EDITED);
            break;

        default:
            pr_obj_p->SetCalaPlannedStatus(PLANNED_STATUS_NONE);

    }



    //Patni-PJS/2011Feb09/Added/MVC009736
    pr_obj_p->SetNumOfCoverages(l_num_of_coverages);

    //+Patni-HAR/2010Apr26/Added/IR-134
    if (l_mrs_eddy_axis) {
        pr_obj_p->SetMRSEddyAxis(l_mrs_eddy_axis);
    }

    //-Patni-HAR/2010Apr26/Added/IR-134

    // + Patni - HAR / 2009Mar11 / Memory Leak
    DEL_PTR_ARY(serloid);

    // - Patni - HAR / 2009Mar11 / Memory Leak
    //End of modification
    BITFLD_INIT(mask, K_ALL_PR, 0);
    //Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    int dBdtOperation = 0;

    if (pr_obj_p->GetAcqStatus() == C_PR_STATUS_DONE) {

        GetSubProtValues(pr_obj_p->GetProtocol(),
                         0,
                         mask,
                         K_PR_DBDT_OPERATING_MODE,
                         &dBdtOperation,
                         NULL,
                         NULL);
        pr_obj_p->SetdBdtOperation(dBdtOperation);

        //+Patni-NP/2009Sep6/Added/MVC004766
        switch (dBdtOperation) {
            case 0:
                pr_obj_p->SetdBdtModeString(DBDT_NORMAL_LABEL);
                break;

            case 1:
                pr_obj_p->SetdBdtModeString(DBDT_MODE1_LABEL);
                break;

            case 2:
                pr_obj_p->SetdBdtModeString(DBDT_MODE2_LABEL);
                break;

            default:
                pr_obj_p->SetdBdtModeString(DBDT_NORMAL_LABEL);
                break;
        }

        //-Patni-NP/2009Sep6/Added/MVC004766

    }

    //+Patni-PJS/2009Oct29/Added/IR-92
    if (m_pqm_ptr == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return FALSE;
    }

    if (m_pqm_ptr->IsSameStudy()) {
        SetCurrentSarDbdt(pr_obj_p, true);
    }

    //-Patni-PJS/2009Oct29/Added/IR-92
    str.Format(_T("dBdtOperation[%d] mode[%d]\n"), pr_obj_p->GetdBdtOperation(), pr_obj_p->GetdBdtMode());

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str);

    if (!BITFLD_ON(mask, K_PR_LOCATOR_IMAGE) &&
        path[0].subtree_name > 0) {
        //+Patni-SS/2009Mar28/Moddified/PSP1#32,62 MoveCouch
        pr_obj_p->SetHasParent(TRUE);
        pr_obj_p->SetLocatorImages(path);

    } else {
        pr_obj_p->SetHasParent(FALSE) ;
        pr_obj_p->SetLocatorImages(NULL);
        //-Patni-SS/2009Mar28/Moddified/PSP1#32,62 MoveCouch

    }

    if (!BITFLD_ON(mask, K_PR_ACQ_STATUS)) {

        if (pr_obj_p->GetAcqStatus() != VFC_ACQ_STATUS_WAIT &&
            pr_obj_p->GetAcqStatus() != VFC_ACQ_STATUS_FAIL &&
            pr_obj_p->GetAcqStatus() != VFC_ACQ_STATUS_SUCCESS &&
            pr_obj_p->GetAcqStatus() != VFC_ACQ_STATUS_CURRENT) {

            //Patni-Manish/2010Feb11/Added/MCM0229-00122/ParentImage
            BodyPartGetVF(pr_obj_p->GetProtocol(), l_bodypart);
            BITFLD_DECL(sub_mask, K_ALL_PR);
            pr_obj_p->SetAcqStatus(VFC_ACQ_STATUS_FAIL);

            BITFLD_INIT(sub_mask, K_ALL_PR, 0);
            l_acq_status = VFC_ACQ_STATUS_FAIL;

            if (E_NO_ERROR != SetProtocolValues(f_vf_handle,
                                                pr_obj_p->GetProtocol(),
                                                sub_mask,
                                                K_PR_ACQ_STATUS,
                                                &l_acq_status,
                                                sizeof(int),
                                                NULL)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetProtocolValues failed."));
            }

            pr_obj_p->SetAcqStatus(l_acq_status);
        }
    }


    if (BITFLD_ON(mask, K_PR_HOLD)) {

        BITFLD_DECL(sub_mask, K_ALL_PR);

        pr_obj_p->SetHoldScan(VFC_HOLD_PRE_SCAN);
        l_hold_scan = VFC_HOLD_PRE_SCAN;

        DeleteHoldMode(pr_obj_p->GetProtocol());
        BITFLD_INIT(sub_mask, K_ALL_PR, 0);

        if (E_NO_ERROR != SetProtocolValues(f_vf_handle,
                                            pr_obj_p->GetProtocol(),
                                            sub_mask,
                                            K_PR_HOLD,
                                            &l_hold_scan,
                                            sizeof(int),
                                            NULL)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetProtocolValues failed."));
        }
    }

    /** For Viewing direction **/
    if (BITFLD_ON(mask, K_PR_VIEW_DIR) ||
        (pr_obj_p->GetPqmPrMode()->GetViewDir() != VFC_VIEW_FROM_HEAD &&
         pr_obj_p->GetPqmPrMode()->GetViewDir() != VFC_VIEW_FROM_FEET)) {

        BITFLD_DECL(sub_mask, K_ALL_PR);
        pr_obj_p->GetPqmPrMode()->SetViewDir(VFC_VIEW_FROM_FEET);
        int view_dir = VFC_VIEW_FROM_FEET;

        BITFLD_INIT(sub_mask, K_ALL_PR, 0);

        pr_obj_p->GetPqmPrMode()->SetViewDir(view_dir);
    }

    SetBreathHoldValue(pr_obj_p, f_vf_handle);

    /* For scan offset. 951031 */
    f32vec3_t l_offset;

    ClibVfToUiAdapter vuiAdapter(f_vf_handle);
    vuiAdapter.VuiGetOffsetVector(pr_obj_p->GetProtocol(), &l_offset);

    pr_obj_p->SetOffset(0, l_offset.x);
    pr_obj_p->SetOffset(1, l_offset.y);
    pr_obj_p->SetOffset(2, l_offset.z);

    /* Speeder MAP path Port */
    //+Patni-PJS/2010May20/Modified/JaFT/TMSC-REDMINE-285
    //pr_obj_p->SetSpeederMapProtocol(-1);
    VfPathElem_t	l_activemappath[2] = {0};
    GetMapPathFromVF(pr_obj_p->GetProtocol(), l_activemappath);
    pr_obj_p->SetSpeederMapProtocol(l_activemappath[0].index);

    //-Patni-PJS/2010May20/Modified/JaFT/TMSC-REDMINE-285
    if (pr_obj_p->GetAcqStatus() != VFC_ACQ_STATUS_WAIT) {
        //+Patni-Hemant/2010Feb12/Modified/ACE-Phase#3/CDS Requirement
        //pr_obj_p->SetCouchOffsetFlag(TRUE);
        if (!(pr_obj_p->GetASCOMPASSFlag()) || (pr_obj_p->GetCDSInfo()->m_cds_step == CDS_STEP6_DONE)) {

            pr_obj_p->SetCouchOffsetFlag(TRUE);
        }

        //-Patni-Hemant/2010Feb12/Modified/ACE-Phase#3/CDS Requirement
    }

    CPqmCoilData* l_coil_data = m_pqm_ptr->GetDataManager()->GetPQMCoilData();

    if (m_pqm_ptr->IsSaturnCoilSystem()) {

        const int l_status = GetSaturnCoil(m_vfdb_handle,
                                           pr_obj_p->GetProtocol(), pr_obj_p->GetSaturnCoil());

        GetConnectedCoil(m_vfdb_handle, pr_obj_p->GetProtocol(), pr_obj_p);

        if (E_NO_ERROR != l_coil_data->PqmAllCoilGetSaturn(pr_obj_p->GetSaturnCoil(),
                pr_obj_p->GetCoilDBTable())) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PqmAllCoilGetSaturn failed"));
        }

        if (pr_obj_p->GetSaturnCoil()->CoilName[0] == NULL) {

            //+Patni-PJS/2010Jan20/Added/MVC006796
            CCoilSelectionTool* l_coil_sel_tool = m_pqm_ptr->GetCoilSelectionTool();

            if (l_coil_sel_tool  == NULL) {
                PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                          _T("CCoilSelectionTool pointer l_coil_sel_tool is NULL"));
                return FALSE;
            }

            if (!l_coil_sel_tool->CSSetCoilName(pr_obj_p, m_vfdb_handle)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CSSetCoilName failed."));
            }
        }

        bool l_is_coil_channel_applicable = false;

        if (VFC_ACQ_STATUS_WAIT == l_acq_status) {

            l_is_coil_channel_applicable = UpdateNumchannelforCoilChannelMode(l_coil_channel_mode, pr_obj_p);
        }

        /* Signal情報設定
         * PAS選択時にSignal情報をセットする */
        if ((pr_obj_p->GetSaturnCoil()->numSection > 0
             && pr_obj_p->GetCoilDBTable()->numCoil > 0)
            && (l_coil_data->PqmInstallCheckSaturn(
                    &pr_obj_p->GetSaturnCoil()->CoilList) == E_NO_ERROR)) {

            //AcqGetcoilSignal(PqmGlobal.numChannels,
            //	pr_obj_p->saturnCoil.numSection,
            //	pr_obj_p->saturnCoil.RcvCoil,
            //	pr_obj_p->saturnCoil.Signal,
            //	PqmCoilGetcoilHead(),
            //	PqmCoilGetsectionHead());

            // + LibAcq AMT
            PqmSaturnCoil_t* l_saturn_coil = pr_obj_p->GetSaturnCoil();
            CDataManager* l_data_mgr = CDataManager::GetInstance();
            DllT* l_coil_head    = l_data_mgr->GetPQMCoilData()->GetHead();
            DllT* l_section_head = l_data_mgr->GetPQMSectionData()->GetHead();

            AcqGetcoilSignal(pr_obj_p->GetSystemChannels(), l_saturn_coil->numSection,
                             l_saturn_coil->RcvCoil, l_saturn_coil->Signal,
                             l_coil_head, l_section_head);

            if (/*l_is_coil_channel_applicable &&*/ TRUE == MinimumChannelCheck(l_saturn_coil->numSection, l_saturn_coil->CoilList.num, l_saturn_coil->RcvCoil, l_saturn_coil->Signal)) { // Pavan-10-feb-12
                /* Studyに書き込み */
                PutSaturnCoil(m_vfdb_handle, pr_obj_p->GetProtocol(), l_saturn_coil);
                // - LibAcq AMT
            }
        }

        //+Patni-Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement
        /* V9.50 CDS */
        CCDSUtilities::GetCdsCoordinate(m_vfdb_handle, pr_obj_p->GetProtocol(), pr_obj_p->GetCDSInfo()->m_coordinatetbl);
        CCDSUtilities::GetCdsLocation(m_vfdb_handle, pr_obj_p->GetProtocol(), pr_obj_p->GetCDSInfo()->m_locationtbl);
        CCDSUtilities::GetCdsStatus(m_vfdb_handle, pr_obj_p->GetProtocol(), &(pr_obj_p->GetCDSInfo()->m_cds_status));
        //-Patni-Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement
    }

    return  TRUE;
}
//************************************Method Header************************************
// Method Name  : MinimumChannelCheck
// Author       : PATNI/
// Purpose      :
//***********************************************************************************
int CPQMStudy::MinimumChannelCheck(const int f_num_Section, const int f_coil_num,
                                   VftSectionRecvrCoil_t   f_recvr_Coil[],
                                   const VftSectionSignal_t f_signal_Coil[]) const
{

    LPCTSTR FUNC_NAME = _T("CPQMStudy::MinimumChannelCheck");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    int l_sec_cnt = -1;


    sedb_rec_t*  l_sec_p[PQM_NUM_COIL_ELEMENT] = {0};

    CPqmCoilData* l_coil_data = NULL;
    CDataManager* l_data_mgr = CDataManager::GetInstance();

    if (l_data_mgr != NULL) {
        l_coil_data = l_data_mgr->GetPQMCoilData();

        if (NULL == l_coil_data) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_coil_data data is NULL, return -1"));
            return -1;
        }
    }

    for (int i = 0; i < f_num_Section; i++) {
        if (E_NO_ERROR != l_coil_data->PqmCoilGetSaturnSection(&f_recvr_Coil[i], &l_sec_cnt, l_sec_p)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PqmCoilGetSaturnSection failed"));
            return -1;
        }

        if (l_sec_cnt > 0) {
            if (f_signal_Coil[i].num < l_sec_p[0]->minChannel) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("return FALSE"));
                return FALSE;
            }
        }
    }

    int l_total_num_signal = 0;

    for (i = 0 ; i < f_num_Section; i++) {
        l_total_num_signal += f_signal_Coil[i].num;
    }

    int	l_channel = m_pqm_ptr->GetChannelPerVFEP() * m_pqm_ptr->GetVAPChannels();

    if (l_total_num_signal > l_channel) {
        CString l_str = _T("");
        l_str.Format(_T("return FALSE as  l_total_num_signal[%d] > l_channel[%d]"), l_total_num_signal, l_channel);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
        return FALSE;
    }

    return TRUE;
}
//************************************Method Header************************************
// Method Name  : GetPGUIDForProtocol
// Author       : PATNI/ HEMANT
// Purpose      : This function expects as f_protocol_index as valid protocol index
//                and returns the inscan flag value for this protocol index
//***********************************************************************************
CString CPQMStudy::GetPGUIDForProtocol(
    const int f_protocol_acq_order
)
{
    BITFLD_DECL(mask, K_ALL_PR);
    memset(mask, 0, sizeof(mask));

    int inscan_preference_guid_length = 120 ;
    char l_inscan_preference_guid[120] = {0} ;

    int l_inscan_flag = -1;

    //  K_PR_INSCAN_FLAG,               &l_inscan_flag,                 NULL,
    //  K_PR_INSCAN_PREFERENCE_GUID,    &l_inscan_preference_guid,      inscan_preference_guid_length,

    try {
        GetProtocolValues(m_vfdb_handle,
                          f_protocol_acq_order,
                          mask,
                          K_PR_INSCAN_PREFERENCE_GUID,
                          l_inscan_preference_guid,
                          &inscan_preference_guid_length,
                          NULL);

    } catch (...) {

        ASSERT(FALSE);
    }

    CString l_pguid = l_inscan_preference_guid;

    return l_pguid;
}


//+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
CString CPQMStudy::GetSequnceDirectory(
    const int f_acq_order
)
{
    VfPathElem_t    pelm[] = {
        {SVN_ST_PROTOCOL,       0},
        {SVN_ST_SUBPROTOCOL,    0}
    };

    VfFieldSpec_t       vf_req[1];
    pelm[SVD_ST_PROTOCOL - 1].index = f_acq_order;

    int cnt = 0;
    VFF_SET_ARGS(vf_req[0], SVN_PLN_TEMPLATE_SEQ, 0,
                 0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);

    cnt++;

    // MEITEC/2010Sep02/Modified/MVC008345
    int  num = 0;
    status_t l_status = m_vfdb_handle->GetFields(pelm, SVD_ST_PROTOCOL, vf_req, cnt, &num);
    CString str(_T(""));

    //+Patni-AJS/2010Aug23/Modified/IR-152_Bug2
    if (E_NO_ERROR == l_status) {
        char* path = getenv("SEQDIR");
        str = path ;
        str = str + _T("/") + (char*)vf_req[0].buffer;

        FREE_PTR(vf_req[0].buffer);
    }

    //-Patni-AJS/2010Aug23/Modified/IR-152_Bug2

    return str ;
}


//+Patni-Hemant/2010Feb23/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : GetASCompassFlag
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
BOOL CPQMStudy::GetASCompassFlag(
    const int f_acqorder,
    BOOL* f_compass_flag
)const
{
    CPqmProtocol* l_protocol = GetItemFromProtocol(f_acqorder) ;

    if (l_protocol) {
        (*f_compass_flag) = l_protocol->GetASCOMPASSFlag();

        return TRUE ;
    }

    return FALSE ;
}
//-Patni-Hemant/2010Feb23/Added/ACE-Phase#3/CDS Requirement

//************************************Method Header************************************
// Method Name  : GetProtocolForIndex
// Author       : PATNI/ HEMANT
// Purpose      : returns the CPqmProtocol pointer stored at zero based index = f_index
//                If the f_index is not valid, the function should reutrn NULL.
//***********************************************************************************
CPqmProtocol* CPQMStudy::GetProtocolForIndex(
    const int f_index
)const
{
    //Note : f_index should be zero based index

    //Patni-DKH/2011Mar04/Added/TMSC-REDMINE-1486
    CScopedLock l_scoped_lock(&m_protocol_vec_mutex);

    const int l_total_protocols = m_protocol_vec.size();

    if ((f_index < 0) || (f_index >= l_total_protocols)) {

        ASSERT(FALSE);
        return NULL;
    }

    CPqmProtocol* protocol_to_return = m_protocol_vec.at(f_index);

    return protocol_to_return;
}
//-Patni-HEMANT/ADDED On 5/28/2009 2:54:39 PM/ ACE-2 / WFDA

//+Patni-Sribanta/2010June25/Added/IR-156_2
CString CPQMStudy::GetIDENTValueFor(
    const int f_acq_order
)const
{

    VfPathElem_t    pelm[] = {
        {SVN_ST_PROTOCOL,       0},
        {SVN_ST_SUBPROTOCOL,    0}
    };

    char l_ident[60] = {0};
    VfFieldSpec_t       vf_req[1];
    memset(vf_req, 0, sizeof(VfFieldSpec_t));
    pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    pelm[SVD_ST_PROTOCOL - 1].index = f_acq_order;


    int cnt = 0;
    VFF_SET_ARGS(vf_req[0], SVN_PLN_SEQ_IDENT, SVT_PLN_SEQ_IDENT,
                 60, l_ident, VFO_ALLOCATE | VFO_VARIABLE_LEN);

    cnt++;

    int  num = 0;
    status_t l_status = m_vfdb_handle->GetFields(pelm, SVD_ST_PROTOCOL, vf_req, cnt, &num);

    //path = getenv("SEQDIR");
    //str = path ;
    CString str = l_ident;

    //free(vf_req[0].buffer);

    return str ;

}
//-Patni-Sribanta/2010June25/Added/IR-156_2

//+Patni-RSG/Added/2009Nov25/MSA0248-0033
//****************************Method Header************************************
//Method Name   : GetSeriesLOIDUnderProtocol
//Author        : PATNI/ RSG
//Purpose       : This function creates VFPath for All Active Branches in VF.(CR-23)
//*****************************************************************************
void CPQMStudy::GetSeriesLOIDUnderProtocol(
    const int f_prot_index,
    std::vector<char*>* f_vect_series_loid
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetSeriesLOIDUnderProtocol");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("Trace of CPQMStudy::GetSeriesLOIDUnderProtocol"));

    if (f_prot_index < 0 && f_vect_series_loid == NULL) {

        return;
    }

    _int32 bit_one = 1;
    int total_node_branch = 0;
    int index = 0;

    VfPathElem_t* vfpath = new VfPathElem_t[4];
    memset(vfpath, NULL, sizeof(VfPathElem_t) * 4);
    InitializeVFPath(vfpath);
    vfpath[SVD_ST_PROTOCOL - 1].index = f_prot_index;
    unsigned int node_name = SVN_ST_PROCESSED;
    bitfield_t* active_mask = GetActiveNodeBranches(vfpath, &total_node_branch, node_name);

    if (NULL == active_mask) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Trace of CPQMStudy::GetSeriesLOIDUnderProtocol"));
        return;

    }

    VfPathElem_t* act_path = NULL;
    //int shift_count = 0;

    for (int count = 0; count < total_node_branch; count++) {
        act_path = new VfPathElem_t[4];
        memset(act_path, 0, sizeof(VfPathElem_t) * 4);
        InitializeVFPath(act_path);
        memcpy(act_path, vfpath, sizeof(VfPathElem_t) * 4);

        if (BITFLD_ON(active_mask, count)) {
            act_path[SVD_ST_PROCESSED - 1].index = count;

        } else {
            DEL_PTR_ARY(act_path);
        }


        if (act_path) {
            char* l_series_id = 0;
            VfFieldSpec_t vf_req;
            int num_found = -1;
            VFF_SET_ARGS(vf_req, SVN_PLN_LDB_MAPPED_ID, SVT_PLN_LDB_MAPPED_ID, 0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);
            status_t st = m_vfdb_handle->GetFields(act_path, SVD_ST_PROCESSED, &vf_req, 1, &num_found);

            if (!st && vf_req.buffer) {
                l_series_id = (char*)(vf_req.buffer);
                f_vect_series_loid->push_back(l_series_id);
            }

            DEL_PTR_ARY(act_path);
        }
    }


    DEL_PTR_ARY(vfpath);

    DEL_PTR_ARY(active_mask);
}
//-Patni-RSG/Added/2009Nov25/MSA0248-0033
//*****************************Method Header**********************************
//Method Name    :GetTopWaitProtocol()
//Author         :PATNI/VB
//Purpose        :This method returns the topmost protocol having the status
//                  as wait
//****************************************************************************
CPqmProtocol* CPQMStudy::GetTopWaitProtocol(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetTopWaitProtocol");
    PQM_TRACE(USER_FUNC_MARKER,	FUNC_NAME, _T("Trace of GetTopWaitProtocol"));

    //Patni-DKH/2011Mar04/Added/TMSC-REDMINE-1486
    CScopedLock l_scoped_lock(&m_protocol_vec_mutex);

    protocolvec::const_iterator v1_Iter = NULL;

    for (v1_Iter = m_protocol_vec.begin(); v1_Iter != m_protocol_vec.end(); v1_Iter++) {
        CPqmProtocol* result_acqSeries = *v1_Iter;

        //+Patni-SS/MODIFIED/2009-Jun-04/CRASH TMSC reported
        if ((result_acqSeries != NULL) && result_acqSeries->GetAcqStatus() == C_PR_STATUS_WAIT) {
            return result_acqSeries;

        } else {
            continue;
        }
    }

    return NULL;
}

//*****************************Method Header**********************************
//Method Name    :GetTopWaitOrCurrentProtocol()
//Author         :
//Purpose        :
//****************************************************************************
CPqmProtocol* CPQMStudy::GetTopWaitOrCurrentProtocol() const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetTopWaitOrCurrentProtocol");
    PQM_TRACE(USER_FUNC_MARKER,	FUNC_NAME, _T("Trace"));

    CScopedLock l_scoped_lock(&m_protocol_vec_mutex);

    protocolvec::const_iterator v1_Iter = NULL;

    for (v1_Iter = m_protocol_vec.begin(); v1_Iter != m_protocol_vec.end(); v1_Iter++) {
        CPqmProtocol* result_acqSeries = *v1_Iter;

        if ((result_acqSeries != NULL) && ((result_acqSeries->GetAcqStatus() == C_PR_STATUS_WAIT) ||
                                           (result_acqSeries->GetAcqStatus() == C_PR_STATUS_CURRENT))) {

            return result_acqSeries;

        } else {
            continue;
        }
    }

    return NULL;
}

//+Patni+MP/2009Jun07/Added/ACE-2/Display DBDT Mode and Value
//****************************Method Header************************************
//Method Name   :GetDBDTValueFromStudy()
//Author        :PATNI / MRP
//Purpose       :To Get DB/DT value from Study File
//*****************************************************************************
CString CPQMStudy::GetDBDTValueFromStudy(const int f_protocol
                                        )const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetDBDTValueFromStudy");
    CString l_dbdtStr = _T("");
    int                 n = 0, num = 0;
    VfFieldSpec_t       vfReq[1] = {0};
    VfPathElem_t        path[SVD_ST_SUBPROTOCOL] = {0};
    flt32_t value[2] = {0};
    //+Patni+MP/2009Sep05/Added/MVC4505

    if (f_protocol < 0) {
        return l_dbdtStr;
    }

    //+Patni+MP/2009Sep05/Added/MVC4505

    path[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    path[SVD_ST_PROTOCOL - 1].index = f_protocol ;
    path[SVD_ST_SUBPROTOCOL - 1].subtree_name = SVN_ST_SUBPROTOCOL;
    path[SVD_ST_SUBPROTOCOL - 1].index = 0 ;

    n = 0 ;
    //+Patni+MP/2009Sep05/Added/MVC4505
    CPqmProtocol* l_protocol = GetItemFromProtocol(f_protocol);
    int l_acq_status = C_PR_STATUS_WAIT;

    if (l_protocol != NULL) {

        l_acq_status = l_protocol->GetAcqStatus();
    }

    if ((l_acq_status == C_PR_STATUS_WAIT) || (l_acq_status == C_PR_STATUS_CURRENT)) {

        VFF_SET_ARGS(vfReq[n], SVN_SEQ_DBDT_VALUE,
                     SVT_SEQ_DBDT_VALUE, sizeof(flt32_t),
                     value, VFO_REPLACE);
        n++ ;

    } else {
        VFF_SET_ARGS(vfReq[n], SVN_SEQ_DBDT_VALUE,
                     SVT_SEQ_DBDT_VALUE, sizeof(flt32_t) * 2,
                     value, VFO_REPLACE);
        n++ ;
    }

    status_t status = m_vfdb_handle->GetFields(path, SVD_ST_SUBPROTOCOL, vfReq, n, &num);

    if (status != E_NO_ERROR && num != n) {
        return l_dbdtStr;
    }


    //+Patni+MP/2009Sep05/Added/MVC4505

    if ((l_acq_status == C_PR_STATUS_WAIT) || (l_acq_status == C_PR_STATUS_CURRENT)) {
        if (value[0] == 0) {
            //Patni-NP/2009Sep15/Added/MVC004766
            l_dbdtStr = _T("");

        } else {
            l_dbdtStr.Format(_T(" %0.3f T/s"), value[0]);
        }

    } else { //C_PR_STATUS_DONE ||C_PR_STATUS_FAIL
        if (value[1] == 0) {
            //Patni-NP/2009Sep15/Added/MVC004766
            l_dbdtStr = _T("");

        } else {
            l_dbdtStr.Format(_T(" %0.3f T/s"), value[1]);
        }

    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_dbdtStr);

    //-Patni+MP/2009Sep05/Added/MVC4505

    return l_dbdtStr;

}
//-Patni+MP/2009Jun07/Added/ACE-2/Display DBDT Mode and Value

//***************************Method Header*************************************
//Method Name    :GetParentInfo()
//Author         :PATNI/HAR-SS
//Purpose        :
//*****************************************************************************
void CPQMStudy::GetParentInfo(
    int prot,
    CPqmPrMode* pqm_pr_mode
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetParentInfo");
    //+Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    VfPathElem_t path[PATH_BUFFER] = {0};
    //-Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    BITFLD_DECL(mask, K_ALL_PR);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetParentInfo"));

    memset(mask, 0, sizeof(mask));
    int size = sizeof(path);

    int l_status = GetProtocolValues(m_vfdb_handle,
                                     prot,
                                     mask,
                                     K_PR_LOCATOR_IMAGE,
                                     path,
                                     &size,
                                     NULL);
    CString l_strLog(_T(""));
    l_strLog.Format(L"Status of GetProtocolValues is:- %d", l_status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_strLog);

    l_strLog.Format(L"parent=%d.%d\n", path[0].subtree_name, path[0].index);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_strLog);

    int parent = -1;

    if (!BITFLD_ON(mask, K_PR_LOCATOR_IMAGE)) {
        if (path[0].subtree_name == 1) {
            parent = path[0].index;
        }
    }

    if (parent < 0) {
        return;
    }

    memset(mask, 0, sizeof(mask));
    //+Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    int rcv_coil_id = 0, anatomy = 0, scan_number = 0;
    float couch_pos = 0.0f, home_pos = 0.0f, couch_offset = 0.0f;
    //-Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    //+Patni-SS/2009Mar28/Moddified/PSP1#32,62 MoveCouch
    rcv_coil_id = anatomy = scan_number = 0;
    couch_pos = home_pos = 0.0;
    //-Patni-SS/2009Mar28/Moddified/PSP1#32,62 MoveCouch

    l_status = GetProtocolValues(m_vfdb_handle,
                                 parent,
                                 mask,
                                 K_PR_SCAN_NUMBER,   &scan_number,   NULL,
                                 K_PR_COUCH,     &couch_pos, NULL,
                                 K_PR_HOME_POSITION, &home_pos,  NULL,
                                 K_PR_COUCH_OFFSET,  &couch_offset,  NULL,
                                 K_PR_ANATOMY,       &anatomy,   NULL,
                                 K_PR_RC_COIL,       &rcv_coil_id,   NULL,
                                 NULL);

    l_strLog.Format(L"Status of GetProtocolValues is:- %d", l_status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_strLog);

    pqm_pr_mode->SetScanNumber(scan_number);
    pqm_pr_mode->SetCouchPos(couch_pos);
    pqm_pr_mode->SetHomePos(home_pos);
    pqm_pr_mode->SetCouchOffset(couch_offset);
    pqm_pr_mode->SetAnatomy(anatomy);
    pqm_pr_mode->SetRecvCoilId(rcv_coil_id);

    if (BITFLD_ON(mask, K_PR_SCAN_NUMBER)) {
        pqm_pr_mode->SetScanNumber(0);
    }

    if (BITFLD_ON(mask, K_PR_COUCH)) {
        pqm_pr_mode->SetCouchPos(0.0);
    }

    if (BITFLD_ON(mask, K_PR_HOME_POSITION)) {
        pqm_pr_mode->SetHomePos(0.0);
    }

    if (BITFLD_ON(mask, K_PR_COUCH_OFFSET)) {
        pqm_pr_mode->SetCouchOffset(0.0);
    }

    if (BITFLD_ON(mask, K_PR_ANATOMY)) {
        pqm_pr_mode->SetAnatomy(-1);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("pqm_pr_mode->SetAnatomy = -1"));
    }

    if (BITFLD_ON(mask, K_PR_RC_COIL)) {
        pqm_pr_mode->SetRecvCoilId(0);
    }
}

//+Patni-DKH/2010May03/Added/Phase3#IR105
//*****************************************************************************
//Method Name   : GetIsStudyReopened
//Author        : Patni/PJS
//Purpose       : This function is used to get the study reopen flag
//*****************************************************************************
bool CPQMStudy::GetIsStudyReopened() const
{
    return m_studyreopen;
}

//************************************Method Header************************************
// Method Name  : GetShimEnableFlagFromVF
// Author       : PATNI/ PJS
// Purpose      : This function is used to get the shim enable flag from varfield
//***********************************************************************************
BOOL CPQMStudy::GetShimEnableFlagFromVF(
    const int f_protocol
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetShimEnableFlagFromVF");
    //Trace log
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetShimEnableFlagFromVF"));
    BOOL l_shim_enable_flg = FALSE;
    VfFieldSpec_t   l_vfreq[1];
    VfPathElem_t    l_pelm[SVD_ST_PROTOCOL] = {0};
    int             num = 0, n = 0;

    memset(l_vfreq, 0, sizeof(VfFieldSpec_t));
    memset(l_pelm, 0, sizeof(VfPathElem_t) * SVD_ST_PROTOCOL);

    l_pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_pelm[SVD_ST_PROTOCOL - 1].index = f_protocol;

    //Set varfield arguement
    VFF_SET_ARGS(l_vfreq[0], SVN_PLN_SHIM_ENABLE_FLAG, SVT_PLN_SHIM_ENABLE_FLAG,
                 sizeof(l_shim_enable_flg), &l_shim_enable_flg,
                 VFO_ALLOCATE | VFO_VARIABLE_LEN);
    n++;
    status_t l_status = m_vfdb_handle->GetFields(l_pelm, SVD_ST_PROTOCOL, l_vfreq, n, &num);

    //Check for error
    if ((num != n) || (l_status == E_ERROR)) {
        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME, _T("Failed to get the required field"));
    }

    return l_shim_enable_flg;
}
//************************************Method Header************************************
// Method Name  : GetShimPathFromVF
// Author       : PATNI/ PJS
// Purpose      : This function is used to get the shim path from varfield
//***********************************************************************************
bool CPQMStudy::GetShimPathFromVF(
    const int f_protocol,
    VfPathElem_t* f_shim_path
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetShimPathFromVF");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetShimPathFromVF"));
    VfFieldSpec_t   l_vfreq[1] = {0};
    VfPathElem_t    l_pelm[SVD_ST_PROTOCOL] = {0};
    int             num = 0, n = 0;
    time_t          tm = 0;

    memset(l_vfreq, 0, sizeof(VfFieldSpec_t) * 1);
    memset(l_pelm, 0, sizeof(VfPathElem_t) * SVD_ST_PROTOCOL);
    l_pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_pelm[SVD_ST_PROTOCOL - 1].index = f_protocol;

    const int l_size = sizeof(VfPathElem_t) * 2;
    //Set varfield arguement
    VFF_SET_ARGS(l_vfreq[0], SVN_SEQ_SHIM_PATH, SVT_SEQ_SHIM_PATH,
                 l_size, f_shim_path,
                 VFO_ALLOCATE | VFO_VARIABLE_LEN);
    n++;
    //Get required tag from varfield
    status_t l_status = m_vfdb_handle->GetFields(l_pelm, SVD_ST_PROTOCOL, l_vfreq, n, &num);

    //Check for error
    if (num != n || l_status == E_ERROR) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed to get the required field"));
        return false;
    }

    return true;
}

//****************************Method Header************************************
//Method Name   :GetLastDone()
//Author        :PATNI/ASG
//Purpose       :This method will return the series object which was processed last.
//*****************************************************************************
CPqmProtocol* CPQMStudy::GetLastDone(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetLastDone");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetLastDone"));

    //Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    CPqmProtocol*   result_acqSeries = NULL, *last_done = NULL;
    //Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    protocolvec::const_iterator v1_Iter = NULL;

    //Patni-DKH/2011Mar04/Added/TMSC-REDMINE-1486
    CScopedLock l_scoped_lock(&m_protocol_vec_mutex);

    for (v1_Iter = m_protocol_vec.begin(); v1_Iter != m_protocol_vec.end(); v1_Iter++) {

        result_acqSeries = *v1_Iter;

        //+Patni-SS/MODIFIED/2009-Jun-04/CRASH TMSC reported
        if (result_acqSeries != NULL && result_acqSeries->GetAcqStatus() == C_PR_STATUS_DONE) {
            last_done = *v1_Iter;;

        } else {
            //////
        }
    }

    return last_done;
}

//***************************Method Header*************************************
//Method Name    :GetNewRflSAR()
//Author         :PATNI/MRP
//Purpose        :This function calls the GetNewRfl function of the
//                CPQMSAR class
//*****************************************************************************
void CPQMStudy::GetNewRflSAR(
    const int f_protocol_id
)const
{
    //If the Study handle is NULL get the handle

    if (m_vfdb_handle == NULL) {

    }

    m_pqm_ptr->GetSAR()->GetNewRfl(m_vfdb_handle, f_protocol_id);
}
//***************************Method Header*************************************
//Method Name    :GetValueFromVfSAR()
//Author         :PATNI/MRP
//Purpose        :This function calls the GetValueFromVf function of the
//                CPQMSAR class
//*****************************************************************************
void CPQMStudy::GetValueFromVfSAR(
    CPqmProtocol* f_protocol
)const
{
    m_pqm_ptr->GetSAR()->GetValueFromVF(m_vfdb_handle, f_protocol);
}

//***************************Method Header*************************************
//Method Name    :GetPreviousAgent()
//Author         :PATNI / MRP
//Purpose        :Function returning the Previous Prescan value
//*****************************************************************************
//+Patni+MP/2009Jun07/Modified/ACE-2/Missing Vantage Skip Prescan
int CPQMStudy::GetPreviousAgent(
    CPqmProtocol* protocol,
    int nos,
    float* rgn
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetPreviousAgent");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetPreviousAgent"));

    if (protocol == NULL)
        return E_ERROR;

    BITFLD_DECL(mask, K_ALL_PR);

    if ((int)m_protocol_vec.size() < nos) {
        return E_ERROR;
    } //else {

    //Patni-DKH/2011Mar04/Added/TMSC-REDMINE-1486
    CScopedLock l_scoped_lock(&m_protocol_vec_mutex);

    protocolvec::iterator v1_Iter = (m_protocol_vec.begin() + (protocol->GetPosition() - 1));
    CPqmProtocol* l_protocol = *v1_Iter;
    BOOL    status = false;

    if (l_protocol == protocol) {

        for (; v1_Iter != NULL ; v1_Iter--) {

            l_protocol = *v1_Iter;

            if (l_protocol->GetAcqStatus() == C_PR_STATUS_DONE ||
                l_protocol->GetAcqStatus() == C_PR_STATUS_FAIL) {

                if (--nos == 0) {

                    memset(mask, 0, sizeof(mask));
                    status = GetProtocolValues(m_vfdb_handle,
                                               l_protocol->GetProtocol(),
                                               mask,
                                               K_PR_PRESCAN_RGN_VALUE,
                                               rgn,
                                               NULL,
                                               NULL);

                    if (BITFLD_ON(mask, K_PR_PRESCAN_RGN_VALUE)) {
                        status = E_ERROR;
                        return status;
                    }

                }

                break;
            }
        }
    }

    //}
    if (nos == 0)
        return E_NO_ERROR;

    else
        return E_ERROR;

}
//-Patni+MP/2009Jun07/Modified/ACE-2/Missing Vantage Skip Prescan

//+Patni-Sribanta/2010Aug11/Added/TMSC-REDMINE-557
//****************************Method Header************************************
//Method Name   :GetCFStudyProtocolLastDone()
//Author        :PATNI/Sribanta
//Purpose       :This method will return the protocol rewuired for CF study
//*****************************************************************************
CPqmProtocol* CPQMStudy::GetCFStudyProt(
)const
{

    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetCFStudyProt");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CPQMStudy::GetCFStudyProt"));

    //Patni-DKH/2011Mar04/Added/TMSC-REDMINE-1486
    CScopedLock l_scoped_lock(&m_protocol_vec_mutex);


    CPqmProtocol* l_cfstudy_prot = NULL;
    protocolvec::const_iterator v1_Iter = NULL;

    for (v1_Iter = m_protocol_vec.begin(); v1_Iter != m_protocol_vec.end(); v1_Iter++) {

        if (NULL == v1_Iter) {
            continue;
        }

        if ((C_PR_STATUS_DONE == (*v1_Iter)->GetAcqStatus())) {
            l_cfstudy_prot = *v1_Iter;

        } else if ((l_cfstudy_prot == NULL) && (C_PR_STATUS_WAIT == (*v1_Iter)->GetAcqStatus())) {
            l_cfstudy_prot = *v1_Iter;
            break;
        }
    }

    return l_cfstudy_prot;
}
//-Patni-Sribanta/2010Aug11/Added/TMSC-REDMINE-557

//****************************Method Header************************************
//Method Name   :GetProtocolValues()
//Author        :PATNI/HAR - SS
//Purpose       :Get protocol data. If buffer is NULL,this subroutine allocate
//                  the area.
//*****************************************************************************
BOOL CPQMStudy::GetProtocolValues(
    CVarFieldHandle* const f_vf_handle,
    int        prot,
    bitfield_t* mask,
    ...
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetProtocolValues");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetProtocolValues"));

    if (f_vf_handle == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_vf_handle is NULL !!!!!\n"));
        return  E_ERROR;
    }

    //+Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    unsigned long vf_parm = 0, vf_parm2 = 0;
    int         tag = 0, n = 0;
    va_list     args;   /* variable argument list */
    VfFieldSpec_t   vfReq[K_ALL_PR] = {0};
    VfSize_t*        vfSize[K_ALL_PR] = {0};
    int         tagList[K_ALL_PR] = {0};
    //-Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(vfReq, 0, sizeof(VfFieldSpec_t) * K_ALL_PR);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008

    /* get list of fields to be retrieved */
    va_start(args, mask);

    vf_parm  = 0;
    vf_parm2 = VFO_VARIABLE_LEN;

    VfName_t    vf_name = 0;
    VfType_t    vf_type = 0;
    VfSize_t	vf_size = 0;
    void**      vf_ap = NULL;
    int     prot_tag = 0;
    VfSize_t* vf_size_p = NULL;
    CString str(_T(""));

    while (tag = va_arg(args, int)) {
        vf_ap = NULL;
        vf_ap = va_arg(args, void**);

        vf_size_p = NULL;
        vf_size_p = va_arg(args, VfSize_t*);

        if ((prot_tag = GetInitNumber(tag)) >= 0) {
            vf_name = pr_init[prot_tag].name;
            vf_type = pr_init[prot_tag].type;
            vf_size = pr_init[prot_tag].size;

        } else {
            //+MEITEC/2010Sep29/Modified output format.

            str.Format(L"Illegal request!!! -> prot_tag=%d\n", prot_tag);
            //-MEITEC/2010Sep29/Modified output format.
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str);
            continue;
        }

        if (vf_size_p != NULL) {
            vf_size = *vf_size_p;
            VFF_SET_ARGS(vfReq[n], vf_name, vf_type, vf_size, vf_ap, vf_parm2);

        } else {
            VFF_SET_ARGS(vfReq[n], vf_name, vf_type, vf_size, vf_ap, vf_parm);
        }

        vfSize[n] = vf_size_p;
        tagList[n] = tag;
        n++;

        if (n > K_ALL_PR) {
            return  E_ERROR;
        }
    }

    va_end(args);
    int num = 0, i = 0;
    status_t        status  = E_NO_ERROR;
    VfPathElem_t    pelm[] = {{SVN_ST_PROTOCOL, 0}};
    pelm[SVD_ST_PROTOCOL - 1].index = prot;

    if (n) {
        status = f_vf_handle->GetFields(pelm, SVD_ST_PROTOCOL, vfReq, n, &num);

        for (i = 0; i < n; i++) {
            if (vfReq[i].status == VFE_NO_ERROR) {
                if (vfSize[i] != NULL) {
                    *(vfSize[i]) = vfReq[i].size;
                }
            }
        } /* for(;;) */
    }

    if (num < n) {
        for (i = 0; i < n; i++) {
            if (vfReq[i].status != VFE_NO_ERROR) {
                BITFLD_SET(mask, tagList[i]);

            } else {
                BITFLD_RESET(mask, tagList[i]);
            }
        }
    }

    return(status == E_NO_ERROR && num < n ? E_ERROR : status);
}
//***************************Method Header*************************************
//Method Name    :SetProtocolValues()
//Author         :PATNI/SG
//Purpose        :
//*****************************************************************************
status_t CPQMStudy::SetProtocolValues(
    CVarFieldHandle* const f_vf_handle,
    int     prot,
    bitfield_t*  mask,
    ...
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetProtocolValues");
    //+Patni-SS/2009Apr01/PSP1#VFLOCK
    CString l_print_string(_T(""));
    l_print_string.Format(_T(" = %x"), f_vf_handle);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, (_T("Trace of SetProtocolValues") + l_print_string));
    //-Patni-SS/2009Apr01/PSP1#VFLOCK

    VfPathElem_t    pelm[] = {{SVN_ST_PROTOCOL,    0}};
    va_list        args;   /* variable argument list */

    unsigned long vf_parm = VFO_REPLACE;
    VfFieldSpec_t  vfReq[K_ALL_PR] = {0};
    int            tagList[K_ALL_PR] = {0};
    int            n = 0, tag = 0;
    int        prot_tag = 0;
    void*       vf_ap = NULL;

    pelm[SVD_ST_PROTOCOL - 1].index = prot;

    /* get list of fields to be updated */
    va_start(args, mask);

    while (tag = va_arg(args, int)) {
        VfName_t   vf_name = 0;
        VfType_t   vf_type = 0;
        VfSize_t   vf_size = 0;
        prot_tag = 0;
        vf_ap = NULL;

        vf_ap   = va_arg(args, void*);

        //+Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
        if (vf_ap == NULL) {
            return E_ERROR;
        }

        //-Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
        vf_size = va_arg(args, VfSize_t);

        /* get pr_init data number. */
        if ((prot_tag = GetInitNumber(tag)) >= 0) {
            vf_name = pr_init[prot_tag].name;
            vf_type = pr_init[prot_tag].type;

        } else {
            CString str(L"Illegal request!!! -> tag=%d \n", prot_tag);

            continue;
        }

        VFF_SET_ARGS(vfReq[n], vf_name,
                     vf_type, vf_size, vf_ap, vf_parm);
        tagList[n] = tag;
        n++;

        if (n > K_ALL_PR) {
            return E_ERROR;
        }
    }

    va_end(args);
    int num = 0;
    status_t       status  = E_NO_ERROR;

    if (n) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Before VfPutFields") + l_print_string);
        time_t     time_stamp = 0;
        status = f_vf_handle->PutFields(pelm, SVD_ST_PROTOCOL, vfReq, n,
                                        &num, &time_stamp);
    }

    if (num <= n) {
        /* Return bits as ON corresponding to arguments not done */
        for (int i = 0; i < n; i++) {
            if (vfReq[i].status != VFE_NO_ERROR) {
                BITFLD_SET(mask, tagList[i]);

            } else {
                BITFLD_RESET(mask, tagList[i]);
            }
        }
    }

    return status;
}


//+Patni-MP&Sribanta/2011Apr08/Added/REDMINE-1610
//****************************Method Header************************************
//Method Name   :GetMappedIDForProcessedNode()
//Author        :PATNI /
//Purpose       :To Get Mapped ID value for protocol.
//*****************************************************************************
CString CPQMStudy::GetMappedIDForProcessedNode(const int f_acq_order, const int f_processed_node_index)const
{

    VfPathElem_t    pelm[] = {
        {SVN_ST_PROTOCOL,       0},
        {SVN_ST_PROCESSED,    0}
    };

    VfFieldSpec_t   vf_req[1] = {0};

    memset(vf_req, 0, sizeof(VfFieldSpec_t));
    pelm[SVD_ST_PROTOCOL - 1].index = f_acq_order;
    pelm[SVD_ST_PROCESSED - 1].index = f_processed_node_index;

    int cnt = 0;
    VFF_SET_ARGS(vf_req[0], SVN_PLN_LDB_MAPPED_ID, SVT_PLN_LDB_MAPPED_ID,
                 0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);

    cnt++;

    int  num = 0;
    status_t l_status = m_vfdb_handle->GetFields(pelm, SVD_ST_PROCESSED, vf_req, cnt, &num);

    CString l_mapped_id = _T("");

    if (E_NO_ERROR == l_status && (1 == num) && (vf_req[0].size > 0) && (NULL != vf_req[0].buffer)) {
        l_mapped_id = (char*)(vf_req[0].buffer);
        FREE_PTR(vf_req[0].buffer);
    }

    return l_mapped_id;
}
//-Patni-MP&Sribanta/2011Apr08/Added/REDMINE-1610


int CPQMStudy::GetMRSValueFromVF(const CPqmProtocol* f_protocol)
const
{

    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetMRSValueFromVF");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, FUNC_NAME);

    if (NULL == f_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_protocol pointer is NULL"));
        return -1;
    }

    status_t l_status = E_ERROR;
    int             num = 0, n = 0, l_Data = 0;

    if (f_protocol->GetAppCode() == VFC_APP_CODE_MRS ||
        f_protocol->GetAppCode() == VFC_APP_CODE_MRSCSI) {
        VfFieldSpec_t   l_vfreq[1] = {0};

        VfPathElem_t    l_pelm[SVD_ST_PROTOCOL] = {0};
        int             num = 0, n = 0;
        status_t l_status;

        memset(l_vfreq, 0, sizeof(VfFieldSpec_t) * 1);
        memset(l_pelm, 0, sizeof(VfPathElem_t) * SVD_ST_PROTOCOL);
        l_pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
        l_pelm[SVD_ST_PROTOCOL - 1].index = f_protocol->GetProtocol();

        const int l_size = sizeof(int);

        //Set varfield arguement
        VFF_SET_ARGS(l_vfreq[0], SVN_EXE_PRESCAN_MRS_F0_ACTION, VFT_INT32,
                     l_size, &l_Data,
                     VFO_ALLOCATE | VFO_VARIABLE_LEN);
        n++;
        //Get required tag from varfield
        l_status = m_vfdb_handle->GetFields(l_pelm, SVD_ST_PROTOCOL, l_vfreq, n, &num);

        //Check for error
        if (num != n || l_status == E_ERROR) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed to get the required field"));
            return -1;

        } else
            return l_Data;

    }

    return -1;
}

void CPQMStudy::SetMrsValueInVF(int f_Value, const CPqmProtocol* f_protocol)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetMrsValueInVF");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, FUNC_NAME);

    VfFieldSpec_t	vfReq;
    memset(&vfReq, 0, sizeof(VfFieldSpec_t));

    VFF_SET_ARGS(vfReq,
                 SVN_EXE_PRESCAN_MRS_F0_ACTION,
                 SVT_EXE_PRESCAN_MRS_F0_ACTION,
                 sizeof(f_Value),
                 &f_Value,
                 VFO_REPLACE);

    int	l_num = 0;

    time_t		tm = 0l;
    VfPathElem_t	pelm[2] = {0};
    pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    pelm[SVD_ST_PROTOCOL - 1].index = f_protocol->GetProtocol();

    status_t l_sts = m_vfdb_handle->PutFields(pelm, SVD_ST_PROTOCOL, &vfReq, 1, &l_num, &tm);

    if (l_sts != E_NO_ERROR || l_num != 1) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetMrsValueInVF() failed"));
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exiting CPQMStudy::SetMrsValueInVF"));

}


//***************************Method Header*************************************
//Method Name    :GetHasImageInStudy()
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
BOOL CPQMStudy::GetHasImageInStudy(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetHasImageInStudy");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetHasImageInStudy"));
    return m_has_image;
}

//***************************Method Header*************************************
//Method Name    :SetHasImageInStudy()
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
void CPQMStudy::SetHasImageInStudy(
    const BOOL study_has_image
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetHasImageInStudy");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetHasImageInStudy"));

    m_has_image = study_has_image;
}

//************************************Method Header**********************************
// Method Name  : GetCurrentHighestDbdt
// Author       : PATNI/PJS
// Purpose      : Get current highest dbdt mode member variable m_current_hgst_dbdt
//***********************************************************************************
int CPQMStudy::GetCurrentHighestDbdt(
) const
{
    return m_current_hgst_dbdt;
}

//************************************Method Header**********************************
// Method Name  : SetCurrentHighestDbdt
// Author       : PATNI/PJS
// Purpose      : Set current highest dbdt mode member variable m_current_hgst_dbdt
//***********************************************************************************
void CPQMStudy::SetCurrentHighestDbdt(
    const int f_current_hgst_dbdt
)
{
    m_current_hgst_dbdt = f_current_hgst_dbdt;
}

// + SM4 LOCATOR_IMAGE Himanshu 5 Dec 2008
//****************************Method Header************************************
//Method Name   :GetStudyLOID()
//Author        :PATNI / HAR
//Purpose       :
//*****************************************************************************
CString CPQMStudy::GetStudyLoid(
)const
{
    return m_study_loid;
}
// - SM4 LOCATOR_IMAGE Himanshu 5 Dec 2008
//***************************Method Header*************************************
//Method Name    :SetStudyLoid()
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
void CPQMStudy::SetStudyLoid(
    const CString& f_study_loid
)
{
    m_study_loid = f_study_loid;
}

//***************************Method Header*************************************
//Method Name    :GetStudyPath()
//Author         :PATNI/
//Purpose        :
//*****************************************************************************
CString CPQMStudy::GetStudyPath(
)const
{
    return m_study_path;

}
//***************************Method Header*************************************
//Method Name    :SetStudyPath()
//Author         :PATNI/
//Purpose        :
//*****************************************************************************
void CPQMStudy::SetStudyPath(
    const CString& f_study_path
)
{
    m_study_path = f_study_path;
}

//****************************Method Header************************************
//Method Name   : GetTxCoilInSaturn
//Author        : PATNI/AMT
//Purpose       : Get Transmitter coil from Saturn coil list.
//*****************************************************************************
status_t CPQMStudy::GetTxCoilInSaturn(
    PqmSaturnRcvCoil_t* f_coil,
    PqmCoil_t** f_coil_p
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetTxCoilInSaturn");

    //    AfxMessageBox(_T("CPQMStudy::GetTxCoilInSaturn-Start")); // Test@AMT20080806
    if (f_coil->numCoil == 0) {
        //Patni-Raj/2010Jan15/Added/PH#3#/Debug Window
        CPQMDebugInfo::GetInstance(m_pqm_ptr)->PqmDebugAcqMonitor(_T("PqmGetTxCoilInSaturn NO COIL!!"));
        return 0 ;
    }

    //+Patni-RUP/2009Aug25/Modified/cpp test corrections
    //l_coil_data = CDataManager::GetInstance()->GetPQMCoilData();
    CDataManager* l_data_Manager = CDataManager::GetInstance();

    if (l_data_Manager == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("l_data_Manager pointer is NULL"));
        *f_coil_p = NULL;
        return 0;
    }

    CPqmCoilData* l_coil_data = l_data_Manager->GetPQMCoilData();

    if (l_coil_data == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("PQM_TRACE pointer is NULL"));
        *f_coil_p = NULL;
        return 0;
    }

    //-Patni-RUP/2009Aug25/Modified/cpp test corrections
    PqmCoil_t* l_p = f_coil->coiltbl_p[0] ;
    int l_coil_type = l_p->type ;

    for (int i = 0 ; i < f_coil->numCoil ; i++) {

        l_p = f_coil->coiltbl_p[i] ;

        if (l_coil_type != l_p->type) {

            *f_coil_p = NULL;
            //Patni-Raj/2010Jan15/Added/PH#3#/Debug Window
            CPQMDebugInfo::GetInstance(m_pqm_ptr)->PqmDebugAcqMonitor(_T("PqmGetTxCoilInSaturn DIFFERENT TYPE!!"));
            return 0 ;
        }
    }

    if (l_coil_type  == CDB_TRANSMIT_N_RCV) {

        *f_coil_p = f_coil->coiltbl_p[0] ;

    } else {
        CCoil           l_coil;
        l_coil_data->GetWBCoil(l_coil);
        *f_coil_p = l_coil.GetCoil();
    }

    return l_coil_type ;
}

//****************************Method Header************************************
//Method Name   : SetTxCoilInSaturn
//Author        : PATNI/AMT
//Purpose       : Writes tx coil into Saturn coil
//*****************************************************************************
bool CPQMStudy::SetTxCoilInSaturn(
    CVarFieldHandle* const f_vf_handle,
    CPqmProtocol*        f_obj,
    PqmSaturnRcvCoil_t*  f_coil
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetTxCoilInSaturn");

    //Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(f_vf_handle);


    //+Patni-MJC/2009Aug27/Modified/cpp test corrections
    if (m_pqm_ptr == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return FALSE;
    }

    //-Patni-MJC/2009Aug27/Modified/cpp test corrections

    CPqmCoilData* l_coil_data = m_pqm_ptr->GetDataManager()->GetPQMCoilData();

    if (f_coil->numCoil == 0)   return FALSE ;

    PqmCoil_t*  l_tmp_coil = NULL ;

    if (!GetTxCoilInSaturn(f_coil, &l_tmp_coil)
        || (l_tmp_coil == NULL)) {
        return FALSE ;
    }

    SetProtocolValue(m_vfdb_handle, f_obj, K_PR_TR_COIL,
                     &l_tmp_coil->coil_id, sizeof(int32_t), FALSE, PQM_STS_LIST);

    const int l_d_size = strlen(l_tmp_coil->label) + 1;

    SetProtocolValue(m_vfdb_handle, f_obj, K_PR_TR_COIL_NAME,
                     l_tmp_coil->label, l_d_size, FALSE, PQM_STS_LIST);

    return TRUE;
}

CPASCOMSequence* CPQMStudy::GetPascomSequenceObject(
    VARIANT* pData,
    const int f_acq_order
)
{
    return (SetPascomSequenceObject((CPASCOMSequence*)pData->byref, f_acq_order));
}

//****************************Method Header****************************
// Method Name  :SetPascomSequenceObject
// Author       :PATNI/
// Purpose      :
//*********************************************************************
// + SM4 Defect#737 Himanshu 20090130
CPASCOMSequence* CPQMStudy::SetPascomSequenceObject(
    CPASCOMSequence* f_pascom_sequence,
    const int f_acq_order
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetPascomSequenceObject");

    CString l_str(_T(""));
    l_str.Format(_T("Acq Order  = %d"), f_acq_order);

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_str);

    CPqmProtocol* l_protocol = GetProtocol(f_acq_order);

    //+Patni-RUP/2009Aug20/Modified/cpp test corrections
    if (l_protocol == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("l_protocol pointer is NULL"));
        return f_pascom_sequence;
    }

    //-Patni-RUP/2009Aug20/Modified/cpp test corrections

    //m_pascom_sequence_object->SetSequenceID(l_protocol->GetSeriesLOID());

    CPASParamStore* l_objParamStore = f_pascom_sequence->GetSequenceParam();

    l_objParamStore->SetString(_T("COMM"),
                               l_protocol->GetScanID());

    l_str = l_protocol->GetScanComment();
    l_objParamStore->SetString(_T("MEMO"), l_str);

    l_objParamStore->SetFloat(_T("TRSC"),
                              l_protocol->GetPqmPrMode()->GetCouchPos());

    //+ Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display
    //+Patni-NP/2009Sep15/Added/MVC004766
    //Patni-HAR/2010May18/Modified/MVC007434
    //+Patni-PJS/2010June1/Modified + Added/MVC007434 CR
    //l_objParamStore->SetString(_T("DBDT"), GetDBDTValueFromStudy(l_protocol->GetProtocol()));
    flt32_t l_curr_dbdt_val = 0.0;
    int l_dbdt_ope_mode = 0;
    l_str = _T(""); //Patni/2011Apr1/Added/V2.0/Redmine-781 Bug Fix

    if (E_ERROR != ReaddBdtOpMode(l_protocol, l_dbdt_ope_mode, l_curr_dbdt_val)) {
        if (l_curr_dbdt_val != 0.0) {
            l_str.Format(_T("%f"), l_curr_dbdt_val);
        }

        l_objParamStore->SetString(_T("DBDT"), l_str);
    }

    if (l_dbdt_ope_mode == VFC_DBDT_IEC_OPERATING_NORMAL) {
        l_protocol->SetdBdtModeString(DBDT_NORMAL_LABEL);

    } else if (l_dbdt_ope_mode == VFC_DBDT_IEC_OPERATING_1ST_CONTROLLED) {
        l_protocol->SetdBdtModeString(DBDT_MODE1_LABEL);

    } else if (l_dbdt_ope_mode == VFC_DBDT_IEC_OPERATING_2ND_CONTROLLED) {
        l_protocol->SetdBdtModeString(DBDT_MODE2_LABEL);
    }

    //-Patni-PJS/2010June1/Modified + Added/MVC007434 CR
    //- Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display
    //+Patni-PJS/2010May20/Added/JaFT/TMSC-REDMINE-285
    l_objParamStore->SetString(_T("REUSESHIM"), GetReuseShimValue(l_protocol->GetProtocol()));
    l_objParamStore->SetString(_T("REUSEMAP"), GetReuseMapValue(l_protocol->GetProtocol()));
    //-Patni-PJS/2010May20/Added/JaFT/TMSC-REDMINE-285
    //+ Patni-PJS/2009Sep25/Added/MVC4865

    l_str = _T("----------");



    if (SARCTRL_LICENCE_ON != m_pqm_ptr ->GetSARControlLicense()) {
        l_str = _T("----------");

        //-20091006 Tanoue-san

        if (l_protocol->GetSarInfo()->tsar[sarTypeWholeBody].sar > 0.000001) {
            l_str.Format(_T("%0.3f W/kg"), l_protocol->GetSarInfo()->tsar[sarTypeWholeBody].sar);
            l_str += CPQMSAR::FindSarMode(l_protocol->GetSarInfo()->tsar[sarTypeWholeBody].ope_mode);

            if (l_protocol->GetSarInfo()->active == sarTypeWholeBody) {
                l_str += _T("*");
            }
        }

        l_objParamStore->SetString(_T("SAR"), l_str);

        l_str = _T("----------");

        //-20091006 Tanoue-san
        if (l_protocol->GetSarInfo()->tsar[sarTypeHead].sar > 0.000001) {
            l_str.Format(_T("%0.3f W/kg"), l_protocol->GetSarInfo()->tsar[sarTypeHead].sar);
            l_str += CPQMSAR::FindSarMode(l_protocol->GetSarInfo()->tsar[sarTypeHead].ope_mode);

            if (l_protocol->GetSarInfo()->active == sarTypeHead) {
                l_str += _T("*");
            }
        }

        l_objParamStore->SetString(_T("HEADSAR"), l_str);

        l_str = _T("----------");

        //-20091006 Tanoue-san
        if (l_protocol->GetSarInfo()->tsar[sarTypePartial].sar > 0.000001) {
            l_str.Format(_T("%0.3f W/kg"), l_protocol->GetSarInfo()->tsar[sarTypePartial].sar);
            l_str += CPQMSAR::FindSarMode(l_protocol->GetSarInfo()->tsar[sarTypePartial].ope_mode);

            if (l_protocol->GetSarInfo()->active == sarTypePartial) {
                l_str += _T("*");
            }
        }

        l_objParamStore->SetString(_T("PARTIALBODYSAR"), l_str);

    } else {

        if (K_IMAGING == m_pqm_ptr->GetMode() || (l_protocol->GetAcqStatus() == C_PR_STATUS_DONE) || (l_protocol->GetAcqStatus() == C_PR_STATUS_FAIL)) {

            bool l_is_scan_completed = false;

            if ((l_protocol->GetAcqStatus() == C_PR_STATUS_DONE) ||
                (l_protocol->GetAcqStatus() == C_PR_STATUS_FAIL)) {

                l_is_scan_completed = true;

            }

            l_str = GetSarControlParamStoreSarInfoFor(l_protocol, sarTypeWholeBody, l_is_scan_completed);
            l_objParamStore->SetString(_T("SAR"), l_str);

            l_str =	GetSarControlParamStoreSarInfoFor(l_protocol, sarTypeHead, l_is_scan_completed);
            l_objParamStore->SetString(_T("HEADSAR"), l_str);

            l_str =	GetSarControlParamStoreSarInfoFor(l_protocol, sarTypePartial, l_is_scan_completed);
            l_objParamStore->SetString(_T("PARTIALBODYSAR"), l_str);

        } else {

            l_str = _T("----------");

            l_objParamStore->SetString(_T("SAR"), l_str);
            l_objParamStore->SetString(_T("HEADSAR"), l_str);
            l_objParamStore->SetString(_T("PARTIALBODYSAR"), l_str);
        }
    }

    l_str = _T("----------");

    if (l_protocol->GetSarInfo()->B1rms >= PQM_MIN_B1RMS) {
        l_str.Format(_T("%.2f uT"), RoundUp(l_protocol->GetSarInfo()->B1rms, 2));
    }

    l_objParamStore->SetString(_T("B1RMS"), l_str);


    //- Patni-PJS/2009Sep25/Added/MVC4865
    l_objParamStore->SetFloat(_T("SCANTIME"),
                              (float)l_protocol->GetScanTime());

    l_objParamStore->SetFloat(_T("IMGTECH"),
                              (float)l_protocol->GetAppCode());

    GetPasParamObject(*l_objParamStore, f_acq_order);

    l_objParamStore->SetFloat(_T("HOLD"),
                              (float)l_protocol->GetHoldScan());

    l_objParamStore->SetFloat(_T("ACQSTATUS"),
                              (float)l_protocol->GetAcqStatus());

    int l_anatomy = l_protocol->GetPqmPrMode()->GetAnatomy();
    l_objParamStore->SetFloat(_T("SA"),
                              (float)l_anatomy);

    //+Patni-AMT/2009Mar05/Added/PSP1#85 Code Review
    l_objParamStore->SetFloat(_T("COUCH"),
                              l_protocol->GetPqmPrMode()->GetCouchPos());
    //-Patni-AMT/2009Mar05/Added/PSP1#85 Code Review

    l_objParamStore->SetInt(_T("AUTOVOICE"),
                            l_protocol->GetExeAutoVoice());


    l_str.Format(_T("%d,%d"), l_protocol->GetPqmPrMode()->GetPatoriEndIn(),
                 l_protocol->GetPqmPrMode()->GetPatoriSideUp());
    l_objParamStore->SetString(_T("PORI"), l_str);

    f32vec3_t l_offset = l_protocol->GetOffset();

    l_str.Format(_T("%f,%f,%f"), l_offset.x * 100, l_offset.y * 100, l_offset.z * 100);
    l_objParamStore->SetString(_T("SCANOFFSET"), l_str);

    l_objParamStore->SetFloat(_T("VDIR"), (float)l_protocol->
                              GetPqmPrMode()->GetViewDir());

    l_str = l_protocol->GetContrastName();

    l_objParamStore->SetString(_T("CONTRASTNAME"), l_str);

    //+Patni-Hemant/2010May7/Added/JaFT/TMSC-REDMINE226_225
    l_objParamStore->SetInt(_T("AS-COMPASS"), l_protocol->GetASCOMPASSFlag());
    l_objParamStore->SetInt(_T("AMB-FLAG"), l_protocol->GetAMBEnableFlag());
    l_objParamStore->SetInt(_T("AMB-MODE-FLAG"), l_protocol->GetAMBModeFlag());
    //-Patni-Hemant/2010May7/Added/JaFT/TMSC-REDMINE226_225

    //Patni-RUP/2010Jul27/Added/TMSC-REDMINE-466
    l_objParamStore->SetInt(_T("COUCHMETHOD"), l_protocol->GetcouchMethodFlg());

    l_objParamStore->SetString(_T("DELAYTIME"), l_protocol->GetDelayTime());
    l_objParamStore->SetInt(_T("GROUPID"), l_protocol->GetGroupID());
    l_objParamStore->SetInt(_T("MASTERFLAG"), l_protocol->GetMasterFlag());
    l_objParamStore->SetInt(_T("MSDENABLEFLAG"), l_protocol->GetMultiSeriesDynamicFlag());

    l_objParamStore->SetInt(_T("INSERTMAP"), l_protocol->GetMapRequired());
    l_objParamStore->SetInt(_T("INSERTSHIM"), l_protocol->GetRequireShimming());
    return f_pascom_sequence;
}

//+Patni-HAR/2010May06/Added/MVC006285
//*****************************************************************************
//Method Name   : GetScanDateTime()
//Author        : Patni / HAR
//Purpose       :
//*****************************************************************************
char* CPQMStudy::GetScanDateTime(
    const int f_protocol
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetScanDateTime");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));
    VfFieldSpec_t   l_vf_req[1] = {0};
    VfPathElem_t search_node_path[] = {
        { SVN_ST_PROTOCOL,      0 },
        { SVN_ST_SUBPROTOCOL,   0 },
        { SVN_ST_SLICEGROUP,    0 },
        { SVN_ST_SLICE,     0 },
        { SVN_ST_IMAGE,     0 },
    };

    VfPathElem_t    vf_dst_path[MAX_DEPTH] = {0};
    memset(vf_dst_path , 0 , sizeof(VfPathElem_t) * MAX_DEPTH);

    BITFLD_DECL(mask,   MAX_BITFLD);


    if (NULL == m_vfdb_handle) {
        return NULL;
    }

    vf_dst_path[0].subtree_name = SVN_ST_PROTOCOL;
    vf_dst_path[0].index		= 0;

    int total = MAX_BITFLD, active = 0;
    BITFLD_INIT(mask, MAX_BITFLD, 0);

    status_t l_status = m_vfdb_handle->ScanSubtree(vf_dst_path, SVD_ST_PROTOCOL,
                        SVN_ST_SUBPROTOCOL, mask, &total, &active);
    int n = 0;
    int l_num_done  = -1;

    if (E_NO_ERROR == l_status) {
        for (int i = 0; i < total; i++) {

            if (BITFLD_ON(mask, i)) {
                n = 0;
                VFF_SET_ARGS(l_vf_req[n], SVN_EXE_N_SCAN_START, SVT_EXE_N_SCAN_START,
                             0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);

                search_node_path[SVD_ST_PROTOCOL - 1].subtree_name	    = SVN_ST_PROTOCOL;
                search_node_path[SVN_ST_PROTOCOL - 1].index             = f_protocol;
                search_node_path[SVD_ST_SUBPROTOCOL - 1].subtree_name   = SVN_ST_SUBPROTOCOL;
                search_node_path[SVD_ST_SUBPROTOCOL - 1].index		    = i;
                n++;

                l_status = m_vfdb_handle->GetFields(search_node_path,
                                                    SVD_ST_SUBPROTOCOL, l_vf_req, n, &l_num_done);

                if (E_NO_ERROR == l_status) {
                    break;

                } else {
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                              CPqmUtilities::GetLibVfErrorStatusString(l_status));
                }
            }
        }

        //+TMSC-Tanoue/2010Sep03/MVC008324
        if (E_NO_ERROR == l_status && l_vf_req[0].buffer) {
            char* l_time = new char [l_vf_req[0].size + 1];
            memcpy(l_time, l_vf_req[0].buffer, l_vf_req[0].size + 1);
            FREE_PTR(l_vf_req[0].buffer);
            return l_time;

        } else {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      CPqmUtilities::GetLibVfErrorStatusString(l_status));
            return NULL;
        }

        //-TMSC-Tanoue/2010Sep03/MVC008324

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  CPqmUtilities::GetLibVfErrorStatusString(l_status));
        return NULL;
    }

    return NULL;
}

//*****************************************************************************
//Method Name   : SetScanDateTime()
//Author        : Patni / HAR
//Purpose       :
//*****************************************************************************
status_t CPQMStudy::SetScanDateTime(
    char* f_scan_time
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetScanDateTime");

    CString l_print_str(f_scan_time);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_print_str);

    VfFieldSpec_t   l_vf_req[1] = {0};
    VfPathElem_t search_node_path[] = {
        { SVN_ST_PROTOCOL,      0 },
        { SVN_ST_SUBPROTOCOL,   0 },
        { SVN_ST_SLICEGROUP,    0 },
        { SVN_ST_SLICE,     0 },
        { SVN_ST_IMAGE,     0 },
    };
    time_t          l_temp_time     = 0;
    int             n               = 0;
    int             num             = 0;

    if (NULL == m_vfdb_handle) {
        return E_ERROR;
    }

    VFF_SET_ARGS(l_vf_req[n], SVN_SCH_N_SCAN_DATE_TIME, SVT_SCH_N_SCAN_DATE_TIME,
                 strlen(f_scan_time) + 1, f_scan_time, VFO_REPLACE);

    search_node_path[SVD_ST_PROTOCOL - 1].subtree_name	= SVN_ST_PROTOCOL;
    search_node_path[SVN_ST_PROTOCOL - 1].index         = 0;

    n++;

    return m_vfdb_handle->PutFields(NULL, SVD_ST_STUDY, l_vf_req, n, &num, &l_temp_time);
    //return l_status;
}
//-Patni-HAR/2010May06/Added/MVC006285

//************************************Method Header**********************************
// Method Name  : GetCurrentHighestSar
// Author       : PATNI/PJS
// Purpose      : Get current highest sar mode member variable m_current_hgst_sar
//***********************************************************************************
int CPQMStudy::GetCurrentHighestSar(
) const
{
    return m_current_hgst_sar;
}

//************************************Method Header**********************************
// Method Name  : SetCurrentHighestSar
// Author       : PATNI/PJS
// Purpose      : Set current highest sar mode member variable m_current_hgst_sar
//***********************************************************************************
void CPQMStudy::SetCurrentHighestSar(
    const int f_current_hgst_sar
)
{
    m_current_hgst_sar = f_current_hgst_sar;
}
//************************************Method Header**********************************
// Method Name  : SetAutoMapProtocolsDegeneracyFlag
// Author       : PATNI/
// Purpose      :
//***********************************************************************************
void CPQMStudy::SetAutoMapProtocolsDegeneracyFlag(int f_degeneracy_flag, const int f_index)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetAutoMapProtocolsDegeneracyFlag");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, FUNC_NAME);

    VfFieldSpec_t	vfReq;
    memset(&vfReq, 0, sizeof(VfFieldSpec_t));

    int n = 0;
    int	l_num = 0;
    time_t		tm = 0l;
    VfPathElem_t	pelm[2] = {0};

    VFF_SET_ARGS(vfReq,
                 SVN_PLN_DEGENERACY_FLAG,
                 SVT_PLN_DEGENERACY_FLAG,
                 sizeof(f_degeneracy_flag),
                 &f_degeneracy_flag,
                 VFO_REPLACE);
    n++;

    pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    pelm[SVD_ST_PROTOCOL - 1].index = f_index;

    status_t l_sts = m_vfdb_handle->PutFields(pelm, SVD_ST_PROTOCOL, &vfReq, n, &l_num, &tm);

    if (l_sts != E_NO_ERROR || l_num != n) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetAutoMapProtocolsDegeneracyFlag() failed"));
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exiting CPQMStudy::SetAutoMapProtocolsDegeneracyFlag"));
}
//+Patni-PJS/2011Feb21/Added/IR-97
//****************************Method Header************************************
//Method Name   :SetIsValidWFDAScan()
//Author        :PATNI/PJS
//Purpose       :
//*****************************************************************************
void CPQMStudy::SetIsValidWFDAScan(
    BOOL f_is_valid_wfda_scan
)
{
    m_is_valid_wfda_scan = f_is_valid_wfda_scan;
}
//-Patni-PJS/2011Feb21/Added/IR-97


//***************************Method Header*************************************
//Method Name    :SetAcqStatus()
//Author         :PATNI / AC
//Purpose        :This method sets the Acquistion status of the given series
//                to the passed status
//*****************************************************************************
void CPQMStudy::SetAcqStatus(
    CPqmProtocol* curr_protocol,
    const int acq_status
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetAcqStatus");
    CString str_status(_T(""));
    str_status.Format(_T("%d"), acq_status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetAcqStatus: ") + str_status);

    BITFLD_DECL(mask, K_ALL_PR);

    if (curr_protocol == NULL) {
        return;
    }

    //+Patni-SS/MODIFIED/2009-Jun-02/JFT#213
    //+Patni-MP/Added/2010-Sep-10/TMSC-REDMINE-790
    if ((C_PR_STATUS_DONE == curr_protocol->GetAcqStatus()) || (C_PR_STATUS_FAIL == curr_protocol->GetAcqStatus())) {
        return;
    }

    //-Patni-MP/Added/2010-Sep-10/TMSC-REDMINE-790
    curr_protocol->SetAcqStatus(acq_status);
    CPqmProtocol* wait_protocol = NULL;
    SetTopWaitPos(GetTopWaitPos(&wait_protocol));

    //-Patni-SS/MODIFIED/2009-Jun-02/JFT#213
    if (acq_status != C_PR_STATUS_WAIT) {
        if ((wait_protocol != NULL) && (wait_protocol->GetProtocol() != PQ_FILLER)
            && wait_protocol != curr_protocol) {
            if (!(wait_protocol->GetHoldScan() & VFC_HOLD_PRE_SCAN) &&
                ((curr_protocol->GetAcqStatus() != VFC_ACQ_STATUS_SUCCESS)
                 || ((wait_protocol->GetProcessing() != PROCESSNG_PROEDIT) && (wait_protocol->GetProcessing() != PROCESSING_VALUE_NONE)))) {

                SetScanModeOfHead();

                wait_protocol->SetHoldScan(
                    wait_protocol->GetHoldScan() | VFC_HOLD_PRE_SCAN);

                CString str(_T(""));
                str.Format(L"Reset Auto Scan wait_protocol->holdScan = 0x%x, wait_protocol->acqStatus = %d, wait_protocol->Processing = %d \n",
                           wait_protocol->GetHoldScan(),
                           curr_protocol->GetAcqStatus(),
                           wait_protocol->GetProcessing());

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str);
                memset(mask, 0, sizeof(mask));
                int hold_scan = wait_protocol->GetHoldScan();
                int auto_hold = wait_protocol->GetAutoHold();


                if (E_ERROR != SetProtocolValues(m_vfdb_handle,
                                                 wait_protocol->GetProtocol(),
                                                 mask,
                                                 K_PR_HOLD,
                                                 &(hold_scan),
                                                 sizeof(int),
                                                 K_PR_AUTO_HOLD,
                                                 &(auto_hold),
                                                 sizeof(int),
                                                 NULL)) {
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetProtocolValues failed."));
                }

                wait_protocol->SetHoldScan(hold_scan);
                wait_protocol->SetAutoHold(auto_hold);

                GetProtocolItemValues(m_vfdb_handle, wait_protocol);

            }
        }

        //For SAR
        if (curr_protocol->GetAcqStatus() == C_PR_STATUS_DONE) {
            GetNewRflSAR(curr_protocol->GetProtocol());
        }

        GetValueFromVfSAR(curr_protocol);

    } else if (acq_status == C_PR_STATUS_WAIT) {
        ConvCouchOffset(curr_protocol);

        //+Patni-NP/2010Jul27/Added/IR-161
        SpeederResetDynamic(GetVfDBHandle(), curr_protocol);
        //-Patni-NP/2010Jul27/Added/IR-161
    }

    if (curr_protocol == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("curr_protocol pointer is NULL"));
        return;
    }

    //-Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    int acq_curr_status = curr_protocol->GetAcqStatus();
    memset(mask, 0, sizeof(mask));
    status_t    status = SetProtocolValues(m_vfdb_handle,
                                           curr_protocol->GetProtocol(),
                                           mask ,
                                           K_PR_ACQ_STATUS,
                                           &acq_curr_status,
                                           sizeof(int),
                                           NULL);

    //+Patni-MJC/2009Aug27/Modified/cpp test corrections
    if (m_pqm_ptr == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug27/Modified/cpp test corrections
    //+Patni-RSG/2009May20/JFT-IR-17
    if (curr_protocol->GetSeriesLOID()) {
        m_pqm_ptr->GetStudyManagerPtr()->SetAcquisitionStatusInLocalDB((BSTR)(curr_protocol->GetSeriesLOID().AllocSysString()), acq_curr_status);
    }

    //-Patni-RSG/2009May20/JFT-IR-17

    curr_protocol->SetAcqStatus(acq_curr_status);

    if (status != E_NO_ERROR) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetProtocolValues ERROR! \n"));
        SetScanModeOfHead();
        return;
    }

    GetProtocolItemValues(m_vfdb_handle, curr_protocol);

    if (curr_protocol->GetAcqStatus() == C_PR_STATUS_WAIT) {
        SetScanModeOfHead();
    }

    //Commented by Meghana On 8/5/2008 2:57:24 PM


    //    for(v1_Iter = m_protocol_vec.begin();v1_Iter != m_protocol_vec.end(); v1_Iter++  ) {
    //        result_acqSeries = *v1_Iter;
    //        result_acqSeries->SetPosition(result_acqSeries->GetPosition() + 1);
    //    }
}





//***************************Method Header*************************************
//Method Name    :SetScanOffsetValue()
//Author         :PATNI/MSN
//Purpose        :

//*****************************************************************************
bool CPQMStudy::SetScanOffsetValue(
    const int index,
    const float f_scanoffset_x,
    const float f_scanoffset_y,
    const float f_scanoffset_z,
    const BOOL f_set_mode
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetScanOffsetValue");

    //   Widget w_offset = PqmUiGetWidget(W_OFFSET_DB);
    //   Widget x_scale  = PqmUiGetWidget(W_OFFSET_X);
    //   Widget      y_scale  = PqmUiGetWidget(W_OFFSET_Y);
    //   Widget      z_scale  = PqmUiGetWidget(W_OFFSET_Z);
    if (m_pqm_ptr == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return false;
    }

    CPqmProtocol* l_obj = GetProtocol(index);

    if (l_obj == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_obj pointer is NULL"));
        return false;
    }

#ifdef DEBUG
    status_t    status = E_NO_ERROR;
#endif

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of SetScanOffsetValue"));

    //+Patni-Hemant/2010May25/Modified/MaFT/TMSC-REDMINE-MVC007976
    CScanProcedure* l_scan_proc = m_pqm_ptr->GetAcqManager()->GetScanProcedure();

    if (l_obj->GetAcqStatus() != C_PR_STATUS_WAIT || l_obj->GetHasParent()) {
        //-Patni-Hemant/2010May25/Modified/MaFT/TMSC-REDMINE-MVC007976

        l_obj->SetProcessing(FALSE);
        //PqmUiErrorDialog(PqmUiFetchLiteral("ERR_SET_OFFSET"),ERR_ERROR);
        CPQMLogMgr::GetInstance()->DisplayScanError(IDS_ERR_SET_OFFSET, _T("IDS_ERR_SET_OFFSET"), SCAN_ERR_CRUCIAL, L"PQMStudy.cpp");
        //MEITEC/2010May27/Modified/TMSC-REDMINE-MVC007976
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Vector not converted..."));
        return false;
    }

    f32vec3_t   vec;
    vec.x = f_scanoffset_x;
    vec.y = f_scanoffset_y;
    vec.z = f_scanoffset_z;

    DB_PRINTF("c", ("x=%g,y=%g,z=%g\n", vec.x, vec.y, vec.z));

    //Patni-AMT/2009Dec4/Modified/DeFT# MVC005787+MSA0248-00087
    VftPatori_t l_patori = (l_obj->GetPqmPrMode()->GetPatientOrientation());
    f32vec3_t offset = l_obj->GetOffset();
    //+Patni-SS/Modified/20091217/MVC005787+MSA0248-00087
    PqmUiConvertVector(&l_patori, FALSE, &vec, &offset); // FALSE: GUI display value need to save in DB
    //-Patni-SS/Modified/20091217/MVC005787+MSA0248-00087

    offset.x /= OFFSET_RATE;
    offset.y /= OFFSET_RATE;
    offset.z /= OFFSET_RATE;

    l_obj->SetOffset(0, offset.x);
    l_obj->SetOffset(1, offset.y);
    l_obj->SetOffset(2, offset.z);
    //+Patni-MP /2010Jan22/Modified/MVC006820
    ClibVfToUiAdapter vuiAdapter(m_vfdb_handle);
    int sts = vuiAdapter.VuiSetOffsetVector(l_obj->GetProtocol(), &offset);
    //-Patni-MP /2010Jan22/Modified/MVC006820
    l_obj->SetProcessing(FALSE);

    if (f_set_mode) {

        bool_t  couch_enable = FALSE;
        PqmCoil_t*  coil_p = NULL;

        if (m_pqm_ptr->IsSaturnCoilSystem()) {

            int inLimit = 0, outLimit = 0;
            PqmSaturnRcvCoil_t* l_coiltable = l_obj->GetCoilDBTable();
            couch_enable = (m_pqm_ptr->GetDataManager()->GetPQMCoilData()->GetInOutSideLimit(l_coiltable, &inLimit, &outLimit) == E_NO_ERROR
                            &&  inLimit + outLimit > 0);

        } else {
            CCoil coil;
            m_pqm_ptr->GetDataManager()->GetPQMCoilData()->PqmCoilGetCoil(l_obj->GetPqmPrMode()->GetRecvCoilId(), coil);
            couch_enable = (coil_p != NULL && coil_p->inSideLimit + coil_p->outSideLimit > 0);
        }

        // V7.00  Wide Area Scan スキャンオフセット設定したら寝台移動をＯＮにする
        if (offset.z) {

            if (couch_enable && !l_obj->GetFluoro() &&
                (l_obj->GetAppCode() != VFC_APP_CODE_MRS) &&
                (l_obj->GetAppCode() != VFC_APP_CODE_MRSCSI) &&
                (l_obj->GetShimming() != VFC_SHIM_HIGH_RESOLUTION)) {

                int l_holdscan = l_obj->GetHoldScan() | VFC_HOLD_COUCH;
                l_obj->SetHoldScan(l_holdscan);

                SetProtocolValue(m_vfdb_handle, l_obj, K_PR_HOLD,
                                 &l_holdscan, sizeof(int32_t),
                                 TRUE, PQM_STS_LIST);

                l_obj->SetcouchMethodFlg(VFC_MOVE_COUCH_METHOD_PLAN_CENTER_POSI);
                int l_couchMethodFlg = l_obj->GetcouchMethodFlg();
                SetProtocolValue(m_vfdb_handle, l_obj,
                                 K_PR_MOVE_COUCH_METHOD_FLAG,
                                 &l_couchMethodFlg,
                                 sizeof(int32_t),
                                 TRUE, PQM_STS_LIST);

            }

            l_obj->SetPlnStatus(l_obj->GetPlnStatus() | VFC_PLN_STATUS_GLOC);

            int l_pln_status = l_obj->GetPlnStatus();
            SetProtocolValue(m_vfdb_handle, l_obj, K_PR_STATUS,
                             &l_pln_status, sizeof(int32_t),
                             TRUE, PQM_STS_LIST);

        }

        SetScanOffsetModeFlag(l_obj, 1);
    }

    //  PQM_SET_WIDGET_STATUS(obj->pos);
    return true;
}

//***************************Method Header*************************************
//Method Name    :SetShimmingInfo()
//Author         :PATNI/
//Purpose        :
//*****************************************************************************
void CPQMStudy::SetShimmingInfo(
    const int     f_protocolnum,
    BOOL    f_isactiveshim
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetShimmingInfo");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetShimmingInfo"));

    VfFieldSpec_t   l_vfreq[5] = {0};
    VfPathElem_t    l_pelm[SVD_ST_PROTOCOL] = {0};              //Patni-MJC/2009Aug17/Modified/cpp test corrections
    VfPathElem_t    l_activeshimpath[SVD_ST_PROCESSED] = {0};   //Patni-MJC/2009Aug17/Modified/cpp test corrections
    VfPathElem_t    l_activehrshimpath[SVD_ST_PROCESSED] = {0}; //Patni-MJC/2009Aug17/Modified/cpp test corrections
    int             l_shimfsyflag = 0;                 //Patni-MJC/2009Aug17/Modified/cpp test corrections

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    memset(l_vfreq, 0, sizeof(VfFieldSpec_t) * 4);
    memset(l_pelm, 0, sizeof(VfPathElem_t) * SVD_ST_PROTOCOL);
    memset(l_activeshimpath, 0, sizeof(VfPathElem_t) * SVD_ST_PROCESSED);
    memset(l_activehrshimpath, 0, sizeof(VfPathElem_t) * SVD_ST_PROCESSED);
    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    l_pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_pelm[SVD_ST_PROTOCOL - 1].index = f_protocolnum;

    CPqmShimSharedData::GetInstance()->GetShimFsyFlag(&l_shimfsyflag);
    CPqmShimSharedData::GetInstance()->GetShimPath(l_activeshimpath);
    CPqmShimSharedData::GetInstance()->GetHRShimPath(l_activehrshimpath);
    int l_shim_exe_fsy_flag = VFC_SHIM_FSY_NONE;

    if (TRUE == f_isactiveshim) {
        l_shim_exe_fsy_flag  = l_shimfsyflag;
    }

    VFF_SET_ARGS(l_vfreq[0], SVN_PLN_SHIM_ENABLE_FLAG, SVT_PLN_SHIM_ENABLE_FLAG,
                 sizeof(f_isactiveshim), &f_isactiveshim,
                 VFO_REPLACE);
    VFF_SET_ARGS(l_vfreq[1], SVN_SEQ_SHIM_PATH, SVT_SEQ_SHIM_PATH,
                 sizeof(l_activeshimpath), l_activeshimpath,
                 VFO_REPLACE);
    VFF_SET_ARGS(l_vfreq[2], SVN_SEQ_HR_SHIM_PATH, SVT_SEQ_HR_SHIM_PATH,
                 sizeof(l_activehrshimpath), l_activehrshimpath,
                 VFO_REPLACE);
    VFF_SET_ARGS(l_vfreq[3], SVN_SEQ_SHIM_FSY_FLAG, SVT_SEQ_SHIM_FSY_FLAG,
                 sizeof(int), &l_shimfsyflag,
                 VFO_REPLACE);
    //NFD001-CFA
    VFF_SET_ARGS(l_vfreq[4], SVN_EXE_SHIM_FSY_FLAG, SVT_EXE_SHIM_FSY_FLAG,
                 sizeof(int), &l_shim_exe_fsy_flag,
                 VFO_REPLACE);
    time_t          tm = 0;
    int				num = 0;
    status_t l_status = m_vfdb_handle->PutFields(l_pelm, SVD_ST_PROTOCOL, l_vfreq, 5, &num, &tm);

#ifdef DEBUG

    if (num != 5 ||
        l_vfreq[0].status != E_NO_ERROR || l_vfreq[1].status != E_NO_ERROR ||
        l_vfreq[2].status != E_NO_ERROR || l_vfreq[3].status != E_NO_ERROR ||
        l_vfreq[4].status != E_NO_ERROR) {
        DB_PRINTF("", ("Error in VfAppPutFields()\n"));
    }

#endif
}

//****************************Method Header************************************
//  Method Name   :SetScanModeOfHead()
//  Author        :PATNI/VB
//  Purpose       :This method sets the hold status of the series
//*****************************************************************************
void CPQMStudy::SetScanModeOfHead(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetScanModeOfHead");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetScanModeOfHead"));

    //Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    CPqmProtocol* protocol_ptr = NULL;
    BITFLD_DECL(mask, K_ALL_PR);

    if (GetTopWaitPos(&protocol_ptr) == -1 || protocol_ptr == NULL) {
        //+Patni-Manish/2009Sept30/Added/MVC004983/Coil dlg not display
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CPqmProtocol object is NULL"));
        //-Patni-Manish/2009Sept30/Added/MVC004983/Coil dlg not display
        return;
    }

    if (!((protocol_ptr->GetHoldScan()) & VFC_HOLD_PRE_SCAN)) {

        int hold_scan = protocol_ptr->GetHoldScan();

        CScanProcedure* l_scan_proc = m_pqm_ptr->GetAcqManager()->GetScanProcedure();

        if (l_scan_proc && PQM_PROTOCOL_SCAN != l_scan_proc->GetScanMode()) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Scan mode not PQM_PROTOCOL_MODE"));

        } else {
            hold_scan |= VFC_HOLD_PRE_SCAN;
        }

        int auto_hold = 0;

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Reset Auto Scan\n"));
        memset(mask, 0, sizeof(mask));

        int l_status = SetProtocolValues(m_vfdb_handle,
                                         protocol_ptr->GetProtocol(),
                                         mask,
                                         K_PR_HOLD,
                                         &hold_scan,
                                         sizeof(int),
                                         K_PR_AUTO_HOLD,
                                         &auto_hold,
                                         sizeof(int),
                                         NULL);
        protocol_ptr->SetAutoHold(auto_hold);
        protocol_ptr->SetHoldScan(hold_scan);

        CString l_strLog(_T(""));
        l_strLog.Format(L"Status of SetProtocolValues is:- %d", l_status);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_strLog);


    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("!((protocol_ptr->GetHoldScan()) & VFC_HOLD_PRE_SCAN) Condition Failed"));
    }
}
//Patni-PJS/2009Oct29/Added/IR-92
//************************************Method Header**********************************
// Method Name  : SetCurrentSarDbdt
// Author       : PATNI/PJS
// Purpose      : Set current highest sar mode and dbdt mode
//***********************************************************************************
void CPQMStudy::SetCurrentSarDbdt(
    CPqmProtocol* f_pqm_prot,
    const bool f_everyprotocol_flag /*= false*/
)
{
    if (f_pqm_prot != NULL) {
        status_t l_sarmode = IEC_SAR_NORMAL_MODE;
        sarInfo_t* l_sar_info = f_pqm_prot->GetSarInfo();
        l_sarmode = m_pqm_ptr->GetSARControlLicense() ? l_sar_info->sarControl.ope_mode : l_sar_info->tsar[l_sar_info->active].ope_mode;

        int dbdt_opern = f_pqm_prot->GetdBdtOperation();

        if (!f_everyprotocol_flag || (f_everyprotocol_flag && (VFC_DBDT_IEC_OPERATING_NORMAL != GetCurrentHighestDbdt()))) {

            if (dbdt_opern > GetCurrentHighestDbdt()) {
                SetCurrentHighestDbdt(dbdt_opern);
            }
        }

        if (!f_everyprotocol_flag || (f_everyprotocol_flag && (IEC_SAR_NORMAL_MODE != GetCurrentHighestSar()))) {
            if (l_sarmode > GetCurrentHighestSar()) {
                SetCurrentHighestSar(l_sarmode);
            }
        }
    }
}

/***************************Method Header*************************************
Method Name    :SetScanAnatomy()
Author         :PATNI/HAR
Purpose        :
*****************************************************************************/
void CPQMStudy::SetScanAnatomy(
    int f_scan_anatomy
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetScanAnatomy");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetScanAnatomy"));

    CPqmProtocol* l_protocol = NULL;

    if (GetTopWaitPos(&l_protocol) > 0) {
        if (f_scan_anatomy != -1) {
            // default value was set as HEAD
            f_scan_anatomy = 1;
        }

        l_protocol->GetPqmPrMode()->SetAnatomy(f_scan_anatomy);
        CString tmpStr(_T(""));
        tmpStr.Format(_T("SetAnatomy(f_scan_anatomy) = %d"), f_scan_anatomy);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, tmpStr);
    }
}

//***************************Method Header*************************************
//Method Name    :SetReceiverCoilID()
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
void CPQMStudy::SetReceiverCoilID(
    long f_receiver_coilid
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetReceiverCoilID");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetReceiverCoilID"));

    CPqmProtocol* l_protocol = NULL;

    if (GetTopWaitPos(&l_protocol) > 0) {
        if (f_receiver_coilid != -1) {
            // default value was set as QD Whole Body
            f_receiver_coilid = COILKEY;
        }

        l_protocol->GetPqmPrMode()->SetRecvCoilId(f_receiver_coilid);
    }
}

//****************************Method Header************************************
//Method Name   :SetMRSExecutation
//Author        :PATNI/
//Purpose       :
//*****************************************************************************
int CPQMStudy::SetMRSExecutation(
    const int f_pro_index,
    int f_f0_value,
    int f_ref_value,
    int f_wsat_value
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetMRSExecutation");

    //+ Patni-MP/2009Mar11/Modified/Warning Removal
    //BITFLD_DECL(mask,MAX_NODES);
    VfFieldSpec_t reqFlds[3] = {0};//Patni-MJC/2009Aug17/Modified/cpp test corrections
    VfPathElem_t        path[1] = {0, 0};
    int n = 0;
    /*status = SetProtocolValues
        (m_vf_handle,f_pro_index,mask,
        SVN_EXE_PRESCAN_MRS_F0_ACTION , &f_f0_value,    sizeof(int32_t),
        SVN_EXE_PRESCAN_MRS_REF_ACTION ,    &f_ref_value,   sizeof(int32_t),
        SVN_EXE_PRESCAN_MRS_WSAT_ACTION ,   &f_wsat_value,  sizeof(int32_t),
        NULL); */

    memset(&reqFlds, NULL, sizeof(VfFieldSpec_t) * 3);
    n = 0;
    VFF_SET_ARGS(reqFlds[n], SVN_EXE_PRESCAN_MRS_F0_ACTION , SVT_EXE_PRESCAN_MRS_F0_ACTION ,
                 sizeof(int), &f_f0_value, VFO_REPLACE | VFO_ALLOCATE);
    n++;
    VFF_SET_ARGS(reqFlds[n], SVN_EXE_PRESCAN_MRS_REF_ACTION , SVT_EXE_PRESCAN_MRS_REF_ACTION ,
                 sizeof(int), &f_ref_value, VFO_REPLACE | VFO_ALLOCATE);
    n++;
    VFF_SET_ARGS(reqFlds[n], SVN_EXE_PRESCAN_MRS_WSAT_ACTION , SVT_EXE_PRESCAN_MRS_WSAT_ACTION ,
                 sizeof(int), &f_wsat_value, VFO_REPLACE | VFO_ALLOCATE);
    n++;

    path[0].subtree_name = SVN_ST_PROTOCOL;
    path[0].index = f_pro_index;

    //+Patni-SS/2009Apr01/PSP1#VFLOCK
    CString l_log_msg = _T("");
    l_log_msg.Format(_T("SVN_EXE_PRESCAN_MRS_F0_ACTION =%d , SVN_EXE_PRESCAN_MRS_REF_ACTION = %d, SVN_EXE_PRESCAN_MRS_WSAT_ACTION = %d "), f_f0_value, f_ref_value, f_wsat_value);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_msg);

    //-Patni-SS/2009Apr01/PSP1#VFLOCK
    int numDone = 0;
    status_t l_status = m_vfdb_handle->PutFields(path, SVD_ST_PROTOCOL, reqFlds, n, &numDone, NULL);

    if ((l_status != E_NO_ERROR) ||
        numDone != n) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
        return E_ERROR;

    }

    return l_status;
}

//*****************************************************************************
//Method Name   : SetstudyReopened
//Author        : Patni/PJS
//Purpose       : This function is used to set the study reopen flag
//*****************************************************************************
CPQMStudy::SetstudyReopened(const bool f_istudyreopened)
{
    m_studyreopen = f_istudyreopened;
}

//***************************Method Header*************************************
//Method Name    :SetSeriesLOIDReference()
//Author         :PATNI/MRP
//Purpose        :To Write the Series LOId at the series level
//*****************************************************************************
void CPQMStudy::SetSeriesLOIDReference(
    const int     f_protocolnum,
    CString f_series_loid
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetSeriesLOIDReference");

    //+Patni-HAR/2010Jun17/Modified/V1.20#MVC008324_1
    CString l_print_str(_T(""));
    l_print_str.Format(_T("[Protocol No: %d]"), f_protocolnum);
    l_print_str += _T("[Series LOID = ");
    l_print_str += f_series_loid;
    l_print_str += _T("]");

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_print_str);
    //-Patni-HAR/2010Jun17/Modified/V1.20#MVC008324_1

    VfFieldSpec_t   l_vfreq[2] = {0};   // MEM_INI AMIT 20081230
    VfPathElem_t    l_pelm[SVD_ST_PROTOCOL] = {0}; // MEM_INI AMIT 20081230

    memset(l_pelm, 0, sizeof(VfPathElem_t) * SVD_ST_PROTOCOL);  //Patni-MJC/2009Aug17/Modified/cpp test corrections
    memset(l_vfreq, 0, sizeof(VfFieldSpec_t) * 2);              //Patni-MJC/2009Aug17/Modified/cpp test corrections

    l_pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_pelm[SVD_ST_PROTOCOL - 1].index = f_protocolnum;
    int l_length = f_series_loid.GetLength() + 1;
    char* buffer = new char[l_length];

    memset(buffer, 0, sizeof(char)*l_length);
    wcstombs(buffer, f_series_loid.GetBuffer(l_length) , l_length);

    VFF_SET_ARGS(l_vfreq[0], SVN_PLN_LDB_MAPPED_ID, SVT_PLN_LDB_MAPPED_ID,
                 l_length, buffer ,
                 VFO_ALLOCATE | VFO_REPLACE);

    //+Patni-HAR/2010Jun17/Commented/V1.20#MVC008324_1
    //+Patni-SS/2009Apr01/PSP1#VFLOCK
    //CString l_log_msg = _T("");
    //l_log_msg.Format(_T("SVN_PLN_LDB_MAPPED_ID =%d "), *buffer);
    //CPQMLogMgr::GetInstance()->WriteTraceLog(DOMAIN_NAME,
    //        SYSTEM_FUNC_MARKER,_T("CPQMStudy::SetSeriesLOIDReference"),
    //        l_log_msg);
    //-Patni-HAR/2010Jun17/Commented/V1.20#MVC008324_1
    int             num = 0;//Patni-MJC/2009Aug17/Modified/cpp test corrections
    time_t          tm = 0;//Patni-MJC/2009Aug17/Modified/cpp test corrections

    status_t l_status = m_vfdb_handle->PutFields(l_pelm, SVD_ST_PROTOCOL, l_vfreq, 1, &num, &tm);

    if (l_status != E_NO_ERROR) {
        //+Patni-HAR/2010Jun17/Commented/V1.20#MVC008324_1
        l_print_str = CPqmUtilities::GetLibVfErrorStatusString(l_status);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_print_str);
        //-Patni-HAR/2010Jun17/Commented/V1.20#MVC008324_1
    }

    DEL_PTR_ARY(buffer);

    if ((num != 1) || (l_vfreq[0].status != E_NO_ERROR)) {
        //+Patni-HAR/2010Jun17/Commented/V1.20#MVC008324_1
        l_print_str = CPqmUtilities::GetLibVfErrorStatusString(l_vfreq[0].status);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_print_str);
        //-Patni-HAR/2010Jun17/Commented/V1.20#MVC008324_1
    }
}

//***************************Method Header*************************************
//Method Name    :SetRGNCoverageMode()
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
void CPQMStudy::SetRGNCoverageMode(
    const int port,
    const int mode
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetRGNCoverageMode");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetRGNCoverageMode"));
    //+Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution

    int rgn_type = 0;

    //-Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    if (mode) {
        rgn_type = VFC_RGN_ALL_COVERAGE ;

    } else {
        rgn_type = VFC_RGN_CENTER_COVERAGE ;
    }

    VfFieldSpec_t vf_req[NUM_3] = {0};
    VfPathElem_t path[SVD_ST_SUBPROTOCOL] = {0};

    memset(path , NULL, sizeof(path)) ;
    memset(vf_req, NULL, sizeof(vf_req)) ;

    path[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    path[SVD_ST_PROTOCOL - 1].index = port ;

    VFF_SET_ARGS(vf_req[0],
                 SVN_EXE_RGN_COVERAGE_TYPE,
                 SVT_EXE_RGN_COVERAGE_TYPE,
                 sizeof(int),
                 &rgn_type,
                 VFO_REPLACE);

    //+Patni-SS/2009Apr01/PSP1#VFLOCK
    CString l_log_msg = _T("");
    l_log_msg.Format(_T("SVN_EXE_RGN_COVERAGE_TYPE =%d "), rgn_type);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_msg);
    //-Patni-SS/2009Apr01/PSP1#VFLOCK
    int num = 0;
    time_t tm = 0;
    int status = m_vfdb_handle->PutFields(path, SVD_ST_PROTOCOL, vf_req, 1, &num, &tm);

    if (status != E_NO_ERROR && num != 1) {

        l_log_msg.Format(L"(status=%d) != E_NO_ERROR && (num = %d) != 1 ", status, num);

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_msg);
    }

    return ;
}

// + SHIMMING Lokesh 13 March 2008
//***************************Method Header*************************************
//Method Name    :SetShimmingOffset()
//Author         :PATNI/
//Purpose        :
//*****************************************************************************
PqmShim_t* CPQMStudy::SetShimmingOffset(
    const int f_behavior,
    const int f_protocol
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetShimmingOffset");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetShimmingOffset"));

    PqmShim_t*      l_shim = new PqmShim_t;
    int             l_shimstate = 0;    //Patni-MJC/2009Aug17/Modified/cpp test corrections
    VfPathElem_t    l_pelm[SVD_ST_PROTOCOL] = {0};  //Patni-MJC/2009Aug17/Modified/cpp test corrections

    CString l_tracemsg(_T(""));
    l_tracemsg .Format(_T("f_behavior = %d"), f_behavior);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_tracemsg);

    memset(l_shim, 0, sizeof(PqmShim_t));
    memset(l_pelm, 0, sizeof(VfPathElem_t) * SVD_ST_PROTOCOL);  //Patni-MJC/2009Aug17/Modified/cpp test corrections

    CPqmShimSharedData::GetInstance()->GetShimState(& l_shimstate);

    if (l_shimstate == SHIM_STATE_ACTIVE && f_behavior == PQM_SHIM_USE_VARFIELD) {

        CPqmShimSharedData::GetInstance()->GetShimFsyFlag(&(l_shim->fsyFlag));
        GetShimmingOffset(l_shim);

    } else if ((f_behavior == PQM_SHIM_USE_DEFAULT) || (f_behavior == PQM_SHIM_RESET)) {

        CPqmShimSharedData::GetInstance()->SetShimState(SHIM_STATE_WAIT);
        CPqmShimSharedData::GetInstance()->SetShimPath(NULL);

        WriteToShimData();
        // reset values
        l_shim->fsyValue = 0;
        l_shim->fsyFlag = VFC_SHIM_FSY_WATER;

        if (!GetShimDefault(l_shim)) {
            //DB_PRINTF("",("Cannot get Shim Defailt.\n"));
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Cannot get Shim Defailt"));
        }

    } else {

        //DB_PRINTF("",("Status error. shimState=%d , behavior=%d \n",shimState,behavior));
        CPqmShimSharedData::GetInstance()->SetShimState(SHIM_STATE_WAIT);
        CPqmShimSharedData::GetInstance()->SetShimPath(NULL);
        // reset values
        memset(l_shim, 0, sizeof(PqmShim_t));
        l_shim->fsyValue = 0;
        l_shim->fsyFlag = VFC_SHIM_FSY_WATER;
    }

    if (f_behavior == PQM_SHIM_RESET || f_behavior == PQM_SHIM_USE_DEFAULT) {
        PutShimmingOffset(m_vfdb_handle, NULL, SVD_ST_STUDY, l_shim);
    }

    if (f_behavior == PQM_SHIM_USE_DEFAULT || f_behavior == PQM_SHIM_USE_VARFIELD) {
        l_pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
        l_pelm[SVD_ST_PROTOCOL - 1].index = f_protocol;

        PutShimmingOffset(m_vfdb_handle, l_pelm, SVD_ST_PROTOCOL, l_shim);
    }

    CPqmShimSharedData::GetInstance()->SetHRShimPath(NULL);

    if (!WriteToShimData()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Unable to write updated shim values in shimdata file"));
    }

    return l_shim;
}


//****************************Method Header************************************
//Method Name   : SetProtocolValue
//Author        : PATNI/AMT
//Purpose       : Get Transmitter coil from Saturn coil list.
//*****************************************************************************
status_t CPQMStudy::SetProtocolValue(
    CVarFieldHandle* const f_vf_handle,
    CPqmProtocol*    f_obj,
    const int             f_tag,
    void*            f_data,
    const int             f_size,
    const BOOL            f_remake_item,
    const int             f_remake_part
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetProtocolValue");

    //Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(f_remake_part);

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of SetProtocolValue"));

    int     l_hold_scan = 0;
    BITFLD_DECL(mask, K_ALL_PR);

    if (f_tag == K_PR_HOLD) {

        l_hold_scan = *((int*)(f_data));

        if (f_obj->GetDynamic()) {
            /* V6.20 Speeder map
            SpeederMAPはDynamic方式になったがBreathHoldは任意に設定したい。
                */
            if (!(l_hold_scan & VFC_HOLD_COVERAGE)
                && (f_obj->GetSpeederType() != VFC_SPEEDER_TYPE_SENSITIVITY)) {
                l_hold_scan |= VFC_HOLD_COVERAGE;
            }
        }

        //+Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image
        int l_couchMethodFlg = f_obj->GetcouchMethodFlg();

        if (f_obj->GetFluoro() ||
            (f_obj->GetAppCode() == VFC_APP_CODE_MRS) ||
            (f_obj->GetAppCode() == VFC_APP_CODE_MRSCSI)) {
            if (l_hold_scan & VFC_HOLD_COUCH) {
                l_hold_scan ^= VFC_HOLD_COUCH;

                f_obj->SetcouchMethodFlg(VFC_MOVE_COUCH_METHOD_NONE);
                SetProtocolValue(m_vfdb_handle,
                                 f_obj,
                                 VFC_MOVE_COUCH_METHOD_PLAN_CENTER_POSI,
                                 &l_couchMethodFlg,
                                 sizeof(int32_t),
                                 TRUE, PQM_STS_LIST);
                //-Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image
            }
        }

        memset(mask, 0, sizeof(mask));
        SetProtocolValues(f_vf_handle,
                          f_obj->GetProtocol(),
                          mask,
                          K_PR_HOLD,
                          &l_hold_scan,
                          f_size,
                          NULL);

    } else {

        memset(mask, 0, sizeof(mask));
        SetProtocolValues(f_vf_handle,
                          f_obj->GetProtocol(),
                          mask,
                          f_tag,
                          f_data,
                          f_size,
                          NULL);
    }

    /*** For SAR --->> 950220 ***/
    if (f_tag == K_PR_RC_COIL || f_tag == K_PR_ANATOMY || f_tag == K_PR_TR_COIL) {
        if (f_tag == K_PR_RC_COIL) {

            f_obj->GetSarInfo()->receiverCoil[0] = *((int32_t*)(f_data));
            f_obj->GetPqmPrMode()->SetRecvCoilId(*((int32_t*)(f_data)));

        } else if (f_tag == K_PR_ANATOMY) {

            sarInfo_t* l_sar = (f_obj->GetSarInfo());
            l_sar->scanAnatomy = *((int32_t*)(f_data));
            f_obj->GetPqmPrMode()->SetAnatomy(*((int32_t*)(f_data)));
            CString tmpStr(_T(""));
            tmpStr.Format(_T("f_obj->GetPqmPrMode()->SetAnatomy(*((int32_t*)(f_data))) = %d"), *((int32_t*)(f_data)));
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, tmpStr);

        } else {

            /* f_tag== K_PR_TR_COIL */
            sarInfo_t* l_sar = (f_obj->GetSarInfo());
            l_sar->transmiterCoil = *((int32_t*)(f_data));
        }

        RecalculateSAR(f_obj);
    }

    //*** <<--- For SAR ***

    GetProtocolItemValues(m_vfdb_handle, f_obj);

    if (f_remake_item) {    // redraw list item flag.
        //DB_PRINTF("u",("PqmPrMakeList() %s:%d\n",__FILE__,__LINE__));
        //PqmPrMakeList(vfd,obj,remakePart);
        //PqmListReplaceItemsPos(obj->prxms,obj->pos,remakePart);
    }

    return E_NO_ERROR;
}

//****************************Method Header************************************
//Method Name   :SetIntensityFlagInStudy()
//Author        :PATNI /MRP
//Purpose       :
//*****************************************************************************
bool CPQMStudy::SetIntensityFlagInStudy(
    bool f_value, CPqmProtocol* f_protocol /*=NULL*/
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetIntensityFlagInStudy");
    CPqmProtocol* l_protocol = GetTopWaitProtocol();

    if (NULL != f_protocol) {
        l_protocol = f_protocol;
    }

    //+Patni-RUP/2009Aug25/Modified/cpp test corrections
    if (NULL == l_protocol) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("l_protocol pointer is NULL"));
        return false;
    }

    //-Patni-RUP/2009Aug25/Modified/cpp test corrections
    l_protocol->SetIntensityType(f_value);
    BITFLD_DECL(mask, K_ALL_PR);
    int size = sizeof(int32_t);
    memset(mask, 0, sizeof(mask));
    SetProtocolValues(m_vfdb_handle, l_protocol->GetProtocol(), mask, K_PR_INTENSITY_CORR_FLAG, &f_value, size, NULL);
    return true;
}

//****************************Method Header************************************
//Method Name   : SetWBCoilSaturn()
//Author        : PATNI\HAR
//Purpose       :
//*****************************************************************************
//+Patni-AMT/2009Oct31/Modified/DeFT/MCM0229-00153
int CPQMStudy::SetWBCoilSaturn(
    CVarFieldHandle* const f_vf_handle,
    const int f_protocol,
    PqmSaturnCoil_t* f_saturn_coil,
    PqmCoil_t* f_coil,
    const char f_port
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetWBCoilSaturn");

    //+Patni-MJC/2009Aug27/Modified/cpp test corrections
    if (m_pqm_ptr == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                  _T("m_pqm_ptr pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug27/Modified/cpp test corrections
    CDataManager* l_data_mgr = m_pqm_ptr->GetDataManager();
    CPqmCoilData* l_coil_data = l_data_mgr ->GetPQMCoilData();
    path_db_rec_t   l_pathP;    // + 578 AMIT PATEL Shimming WB Onsite
    BITFLD_DECL(mask, K_ALL_PR);

    //DB_FUNC_ENTER("[PqmSetWBCoilSaturn]:");

    if (!f_saturn_coil || !f_coil) {
        return E_ERROR;
    }

    /*  ターゲットコイルは従来コイルや新RFコイルどちらでも、
    WholeBodyは従来コイルのコイル情報になる */
    if ((f_coil->mview & 0x80000000)) { /* 従来コイルなのでPATHDB情報を設定する */

        int i = 0;

        memset(&f_saturn_coil->Signal, NULL, sizeof(f_saturn_coil->Signal));
        memset(&f_saturn_coil->RcvCoil, NULL, sizeof(f_saturn_coil->RcvCoil));
        memset(&f_saturn_coil->CoilName, NULL, sizeof(f_saturn_coil->CoilName));
        memset(&f_saturn_coil->CoilList, NULL, sizeof(f_saturn_coil->CoilList));

        // + 578 AMIT PATEL Shimming WB Onsite
        memset(&l_pathP, 0, sizeof(l_pathP));
        CPqmPathData* l_path_db = CDataManager::GetInstance()->GetPqmPathData();

        //+Patni-RUP/2009Aug21/Modified/cpp test corrections
        if (l_path_db == NULL) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("l_path_db pointer is NULL"));
            return E_ERROR ;
        }

        //-Patni-RUP/2009Aug21/Modified/cpp test corrections

        if (l_path_db->GetPathDataFromKey(f_coil->mview & COIL_PHYSICALID_MASK, &l_pathP) == false) {
            return E_ERROR ;
        }


        f_saturn_coil->numSection = 1 ;
        /**  ここでMviewから　PathDB情報を取得して情報を設定する　**/
        int signal = l_pathP.path_data[0];

        for (int j = i = 0 ; i < MAX_SIGNAL_COIL ; i++) {

            if (signal & (0x0001 << i)) {

                f_saturn_coil->Signal[j].signal_no[j] =
                    signal & (0x0001 << i) ;
                f_saturn_coil->Signal[j].num++ ;
            }
        }

        // - 578 AMIT PATEL Shimming WB Onsite

        f_saturn_coil->RcvCoil[0].portID[0] =  f_coil->port;

        //+Patni-AMT/2009Oct31/Modified/DeFT/MCM0229-00153
        if (f_coil->port == CDB_PORT_G) {
            f_saturn_coil->RcvCoil[0].portID[1] = f_port;

        } else {
            f_saturn_coil->RcvCoil[0].portID[1] =  ' ';     /*  Space */
        }

        //-Patni-AMT/2009Oct31/Modified/DeFT/MCM0229-00153
        f_saturn_coil->RcvCoil[0].coilID    =  f_coil->coil_id & COIL_PHYSICALID_MASK ;
        f_saturn_coil->RcvCoil[0].channel   =  f_coil->channel;

        f_saturn_coil->CoilList.num = 1;
        f_saturn_coil->CoilList.coillist[0].portID[0] =  f_coil->port;

        //+Patni-AMT/2009Oct31/Modified/DeFT/MCM0229-00153
        if (f_coil->port == CDB_PORT_G) {
            f_saturn_coil->CoilList.coillist[0].portID[1] = f_port;

        } else {
            f_saturn_coil->CoilList.coillist[0].portID[1] =  ' ';   /*  Space  */
        }

        //-Patni-AMT/2009Oct31/Modified/DeFT/MCM0229-00153
        f_saturn_coil->CoilList.coillist[0].coilID    =  f_coil->coil_id & COIL_PHYSICALID_MASK ;
        f_saturn_coil->CoilList.coillist[0].channel   =  f_coil->channel;

        PutSaturnCoil(f_vf_handle, f_protocol, f_saturn_coil)   ;

        /* 受信コイル名のテーブル作成を行う */
        if (E_NO_ERROR != l_coil_data->PqmAllCoilGetSaturn(f_saturn_coil,
                (GetItemFromProtocol(f_protocol)->GetCoilDBTable()))) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PqmAllCoilGetSaturn failed"));
        }

        PutRecvrCoil(f_vf_handle, f_protocol, (GetItemFromProtocol(f_protocol)));
        memcpy(f_saturn_coil->CoilName , f_coil->label,
               (strlen(f_coil->label) + 1));

        BITFLD_INIT(mask, K_ALL_PR, 0);
        //MRP here on 25 Dec 2008
        SetProtocolValues(f_vf_handle,
                          f_protocol,
                          mask,
                          K_PR_RC_COIL_NAME, f_saturn_coil->CoilName,
                          strlen(f_saturn_coil->CoilName) + 1,
                          K_PR_NUM_PAC_SCAN,
                          &f_coil->num_channels,
                          sizeof(int16_t),
                          NULL);
    }

    return E_NO_ERROR ;
}
//-Patni-AMT/2009Oct31/Modified/DeFT/MCM0229-00153

//***************************Method Header*************************************
//Method Name    :SetNewSAR()
//Author         :PATNI/MRP
//Purpose        :This function calls the SetNewSAR function of the
//                CPQMSAR class
//*****************************************************************************
void CPQMStudy::SetNewSAR(
    const int f_protocol_id
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetNewSAR");

    CString l_print_str(_T(""));
    l_print_str.Format(_T("Handle = %x : Position = %d"), m_vfdb_handle, f_protocol_id);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_print_str);

    CScopedLock l_scoped_lock(&m_protocol_vec_mutex);

    CPqmProtocol* l_obj = GetItemFromPosition(f_protocol_id);

    if (l_obj != NULL) {
        AddPrescanTagsForNextProtocol(GetVfDBHandle(), NULL, l_obj->GetProtocol());
    }


    for (int i = 1; i <= GetAllItems(); i++) {
        l_obj = GetItemFromPosition(i);

        if (l_obj->GetAcqStatus() == C_PR_STATUS_WAIT) {
            RecalculateSAR(l_obj);
        }
    }
}

//***************************Method Header*************************************
//Method Name    :SetFirstScan()
//Author         :PATNI/HAR-SS
//Purpose        :
//*****************************************************************************
void CPQMStudy::SetFirstScan(
    const BOOL first_scan
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetFirstScan");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetFirstScan"));
    m_first_scan = first_scan;
}

//***************************Method Header*************************************
//Method Name    :SetProbeThresHold()
//Author         :PATNI/MSN
//Purpose        :Sets the Minimum and Maximum Threshhold value in the study file
//*****************************************************************************
status_t CPQMStudy::SetProbeThresHold(const int  f_protocol)
{

    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetProbeThresHold");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetProbeThresHold"));

    flt32_t             l_max = 0L, l_min = 0L;

    if (GetProbeData(&l_max, &l_min) == E_NO_ERROR) {

        int l_n = 0;
        VfFieldSpec_t       l_vfreq[3] = {0};
        VFF_SET_ARGS(l_vfreq[l_n], SVN_EXE_PROBE_THRESHOLD_MAX,
                     SVT_EXE_PROBE_THRESHOLD_MAX, sizeof(flt32_t),
                     &l_max, VFO_REPLACE);
        l_n++ ;

        VFF_SET_ARGS(l_vfreq[l_n], SVN_EXE_PROBE_THRESHOLD_MIN,
                     SVT_EXE_PROBE_THRESHOLD_MIN, sizeof(flt32_t),
                     &l_min, VFO_REPLACE);
        l_n++ ;

        if (m_vfdb_handle == NULL) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_vfdb_handle is NULL !!!!!\n"));
            return  E_ERROR;
        }

        //+Patni-SS/2009Apr01/PSP1#VFLOCK
        CString l_log_msg = _T("");
        l_log_msg.Format(_T(" SVN_EXE_PROBE_THRESHOLD_MAX =%d,SVN_EXE_PROBE_THRESHOLD_MIN = %d"), l_max, l_min);

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_msg);
        //-Patni-SS/2009Apr01/PSP1#VFLOCK
        int l_num = 0;
        time_t l_tm = 0;
        VfPathElem_t        l_path[SVD_ST_PROTOCOL] = {0};
        l_path[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
        l_path[SVD_ST_PROTOCOL - 1].index = f_protocol ;

        status_t l_status = m_vfdb_handle->PutFields(l_path, SVD_ST_PROTOCOL, l_vfreq, l_n, &l_num, &l_tm);

        if (l_status != E_NO_ERROR && l_num != l_n) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Put Data Error"));
            return E_ERROR;
        }
    }

    return E_NO_ERROR;
}

/************************************************************************
* Set protocol data.
* And remake info data and list data.
* status_t PqmPrVaSetProtValues(vfd,prot,*mask,...)
*  VfDBHandle_t    vfd,     Study Handler.
*  int     prot,        Protocol number.
*  bitfield_t  *mask,
*  ... )
*  valiable datas are......
*  int     tag,
*  void        *buffer
*  VfSize_t    size
*************************************************************************/
BOOL CPQMStudy::SetProtValues(
    CVarFieldHandle* const vf_handle,
    int     prot,
    bitfield_t*  mask,
    ...
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetProtValues");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetProtValues"));

    va_list        args;   /* variable argument list */
    VfFieldSpec_t  vfReq[K_ALL_PR] = {0};
    int            tagList[K_ALL_PR] = {0};

    //-Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    /* get list of fields to be updated */
    va_start(args, mask);

    /* Over write mode. */
    unsigned long vf_parm  = VFO_REPLACE;

    int  n = 0, tag = 0;
    CString str(_T(""));

    while (tag = va_arg(args, int)) {
        VfName_t   vf_name = 0;
        VfType_t   vf_type = 0;
        VfSize_t   vf_size = 0;
        //+Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
        void*       vf_ap = NULL;
        int        prot_tag = 0;
        //-Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
        vf_ap   = va_arg(args, void*);
        vf_size = va_arg(args, VfSize_t);

        /* get pr_init data number. */
        if ((prot_tag = GetInitNumber(tag)) >= 0) {
            vf_name = pr_init[prot_tag].name;
            vf_type = pr_init[prot_tag].type;

        } else {
            //+MEITEC/2010Sep29/Modified output format.
            str.Format(L"Illegal request!!! -> prot_tag=%d\n", prot_tag);
            //-MEITEC/2010Sep29/Modified output format.
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str);
            continue;
        }



        VFF_SET_ARGS(vfReq[n], vf_name,
                     vf_type, vf_size, vf_ap, vf_parm);
        tagList[n] = tag;
        n++;

        if (n > K_ALL_PR) {
            return E_ERROR;
        }

    }

    va_end(args);
    int num = 0;
    status_t       status  = E_NO_ERROR;

    if (n) {
        //+Patni-SS/2009Apr01/PSP1#VFLOCK
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Before VfPutFields"));

        //+Patni-SS/2009Apr01/PSP1#VFLOCK
        VfPathElem_t    pelm[] = {{SVN_ST_PROTOCOL,    0}};
        pelm[SVD_ST_PROTOCOL - 1].index = prot;
        time_t     time_stamp = 0;
        status = vf_handle->PutFields(pelm, SVD_ST_PROTOCOL, vfReq, n, &num, &time_stamp);

        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("After VfPutFields"));
        //-Patni-SS/2009Apr01/PSP1#VFLOCK
    }

    if (num <= n) {
        /* Return bits as ON corresponding to arguments not done */
        for (int i = 0; i < n; i++) {
            if (vfReq[i].status != VFE_NO_ERROR) {
                BITFLD_SET(mask, tagList[i]);

            } else {
                BITFLD_RESET(mask, tagList[i]);
            }
        }
    }

    return status;
}

//****************************Method Header************************************
//  Method Name   : SetWaitToCurrent()
//  Author        : PATNI/VB
//  Purpose       : This method will set the status of given protocol from
//                  wait to current.
//******************************************************************************
void CPQMStudy::SetWaitToCurrent(
    CPqmProtocol* protocol
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetWaitToCurrent");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (protocol && protocol->GetProtocol() >= 0) {

        float l_val_ret = protocol->GetPqmPrMode()->GetCouchPos();

        if (m_pqm_ptr == NULL) {
            return;
        }

        m_pqm_ptr->SetCouchPos(l_val_ret);
        m_pqm_ptr->WriteToUI(PQM_MSG_UPDATE_COUCH_VAL);


        int current_status = VFC_ACQ_STATUS_CURRENT;
        BITFLD_DECL(mask, K_ALL_PR);
        protocol->SetAcqStatus(C_PR_STATUS_CURRENT);
        memset(mask, 0, sizeof(mask));

        SetProtocolValues(m_vfdb_handle,
                          protocol->GetProtocol(),
                          mask,
                          K_PR_ACQ_STATUS,
                          &current_status,
                          sizeof(int),
                          NULL);

        //+ Patni-RSG/2009Oct14/Modified/MVC005134, MVC004938

        SetTopWaitPos(protocol->GetPosition());
        //- Patni-RSG/2009Oct14/Modified/MVC005134, MVC004938

        //- Patni-PJS/2009Jul29/Modified/MVC0003961
        //+Patni-RSG/2009May20/JFT-IR-17
        if (protocol->GetSeriesLOID()) {
            m_pqm_ptr->GetStudyManagerPtr()->SetAcquisitionStatusInLocalDB((BSTR)(protocol->GetSeriesLOID().AllocSysString()), C_PR_STATUS_CURRENT);
        }

        //-Patni-RSG/2009May20/JFT-IR-17
    }

}

//***************************Method Header*************************************
//Method Name    :SetDefaultRGNValue()
//Author         :PATNI/HAR-SS
//Purpose        :
//*****************************************************************************
void CPQMStudy::SetDefaultRGNValue(
    const int node
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetDefaultRGNValue");
    BITFLD_DECL(mask, K_ALL_PR);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetDefaultRGNValue"));

    memset(mask, 0, sizeof(mask));
    float rgn = 0.0f;
    GetProtocolValues(m_vfdb_handle,
                      node,
                      mask,
                      K_PR_DEFAULT_RGN_VALUE,
                      &rgn,
                      NULL,
                      NULL);

    if (BITFLD_ON(mask, K_PR_DEFAULT_RGN_VALUE)) {
        rgn = m_pqm_ptr->GetDefaultRGN();
    }

    BITFLD_INIT(mask, K_ALL_PR, 0);

    SetProtocolValues(m_vfdb_handle,
                      node,
                      mask,
                      K_PR_PRESCAN_RGN_VALUE,
                      &rgn,
                      sizeof(rgn),
                      NULL);
}



//***************************Method Header*************************************
//Method Name    :SetDelayTimeForProtocol()
//Author         :PATNI/AKR
//Purpose        :
//*****************************************************************************
void CPQMStudy::SetDelayTimeForProtocol(const int f_acq_order, const int f_delay_time)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetDelayTimeForProtocol");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    CPqmProtocol* l_protocol = GetProtocol(f_acq_order);

    if (NULL == l_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_protocol is NULL"));
        return;
    }

    CString l_delay_time_str(_T(""));

    if (f_delay_time) {
        int l_minute = f_delay_time / SIXTY_SEC;
        int l_second = f_delay_time % SIXTY_SEC;
        l_delay_time_str.Format(_T("%02d:%02d"), l_minute, l_second);
    }

    l_protocol->SetDelayTime(l_delay_time_str);
    CString l_trace_msg(_T(""));
    l_trace_msg.Format(_T("Delay time is updated on CPqmProtocol as %s"), l_delay_time_str);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_msg);
}


void CPQMStudy::CheckAutoLocLicensenSetStatus(CPqmProtocol* l_protocol)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::CheckCALALicensenSetStatus");

    if (NULL == l_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_protocol pointer is NULL."));
        return;
    }

    BITFLD_DECL(mask,   MAX_BITFLD) = {0};
    BITFLD_INIT(mask, K_ALL_PR, 0);
    memset(mask, 0, sizeof(mask));

    int l_al_planned_status = VFC_ALA_PLANNED_STATUS_NONE;

    if (!m_pqm_ptr->GetAutoLocatorLicense(AUTOLOCATOR_CARDIAC)) {

        if (E_NO_ERROR != SetProtocolValues(m_vfdb_handle,
                                            l_protocol->GetProtocol(), mask, K_PR_CALA_PLANNED_STATUS, &l_al_planned_status,
                                            sizeof(l_al_planned_status), NULL)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetProtocolValues() failed."));
        }

        l_protocol->SetCalaPlannedStatus(PLANNED_STATUS_NONE);

    }

    if (!m_pqm_ptr->GetAutoLocatorLicense(AUTOLOCATOR_NEURO)) {
        if (E_NO_ERROR != SetProtocolValues(m_vfdb_handle,
                                            l_protocol->GetProtocol(), mask, K_PR_HALA_PLANNED_STATUS, &l_al_planned_status,
                                            sizeof(l_al_planned_status), NULL)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetProtocolValues() failed."));
        }

        l_protocol->SetHalaPlannedStatus(PLANNED_STATUS_NONE);

    }
}


//*******************************Method Header*********************************
//Method Name    :CreatScratchPadFromStudy()
//Author         :PATNI/AKR
//Purpose        :
//*****************************************************************************
status_t CPQMStudy::CreatScratchPadFromStudy(
    const int f_protocol_acq_order,
    const char* const f_scratchpad,
    const SCRATCHPAD_CREATION_MODE f_sp_creation_mode)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::CreatScratchPadFromStudy");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));


    FILE* l_fp = NULL;
    l_fp = fopen(f_scratchpad, "a");

    if (NULL == l_fp) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_fp is NULL"));
        return E_ERROR;
    }

    fclose(l_fp);
    l_fp = NULL;

    if (E_NO_ERROR != MaintainScratchPadForPrivateProlim(f_protocol_acq_order, f_scratchpad, f_sp_creation_mode)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("MaintainScratchPadForPrivateProlim failed...."));
        return E_ERROR;
    }

    return E_NO_ERROR;
}

//****************************Method Header************************************
//Method Name   :Direction
//Author        :PATNI/
//Purpose       :
//*****************************************************************************
int CPQMStudy::Direction(
    const float x,
    const float y,
    const float z
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::Direction");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of Direction"));


    double fx = fabs(x);
    double fy = fabs(y);
    double fz = fabs(z);

    if ((0.0 < fx && 1.0 > fx) || (0.0 < fy && 1.0 > fy) ||
        (0.0 < fz && 1.0 > fz))
        return SHIM_SURFACE_ERROR;

    if (fx >= fy && fx >= fz)
        return SHIMSAGITAL;

    if (fy >= fx && fy >= fz)
        return SHIMCORONAL;

    if (fz >= fx && fz >= fy)
        return SHIMAXICAL;

    return FALSE;
}

//***************************Method Header*************************************
//Method Name    :WriteStudyLOIDinVarfield()
//Author         :PATNI/MRP
//Purpose        :To Write the Study LOID in the Varfield Database
//*****************************************************************************
bool CPQMStudy::WriteStudyLOIDinVarfield(
)
{

    LPCTSTR FUNC_NAME = _T("CPQMStudy::WriteStudyLOIDinVarfield");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of WriteStudyLOIDinVarfield"));

    VfFieldSpec_t   l_vfreq[1] = {0}; //Patni-MJC/2009Aug17/Modified/cpp test corrections

    CString l_StudyLOID = m_study_loid;
    //__asm int 3;

    int l_length =  l_StudyLOID.GetLength() + 1;
    char* buffer = new char[l_length];

    memset(&l_vfreq, NULL, sizeof(VfFieldSpec_t));

    memset(buffer, 0, sizeof(char) * l_length);
    wcstombs(buffer, l_StudyLOID.GetBuffer(l_length) , l_length);

    VFF_SET_ARGS(l_vfreq[0], SVN_PLN_LDB_MAPPED_ID,
                 SVT_PLN_LDB_MAPPED_ID,
                 l_length,
                 buffer,
                 VFO_ALLOCATE | VFO_REPLACE);


    //Commented by Meghana
    //  VfPathElem_t *path = new VfPathElem_t [4];
    //
    //  path[0].subtree_name = SVN_ST_PROTOCOL;
    //    path[0].index = 0;
    //    path[1].subtree_name = SVN_ST_SUBPROTOCOL;
    //    path[1].index = 0;
    //    path[2].subtree_name = SVN_ST_ECHO;
    //    path[2].index = 0;
    //    path[3].subtree_name = SVN_ST_POSITION;
    //    path[3].index = 0;

    //+Patni-SS/2009Apr01/PSP1#VFLOCK
    CString l_log_msg = _T("");
    l_log_msg.Format(_T("SVN_PLN_LDB_MAPPED_ID =%d "), *buffer);

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_log_msg);
    //-Patni-SS/2009Apr01/PSP1#VFLOCK

    int             num = 0;
    time_t          tm = 0;
    status_t l_status = m_vfdb_handle->PutFields(NULL, SVD_ST_STUDY, l_vfreq, 1, &num, &tm);

    DEL_PTR_ARY(buffer);

    if (l_status != E_NO_ERROR || num != 1) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
        return false;
    }


    return true;
}

//************************************Method Header**********************************
// Method Name  : BOOL CPQMStudy::PdbFileCopy()
// Author       : MEITEC
// Purpose      : コンソール側の.pdb_headerファイルをエンジン側にコピーする。
//***********************************************************************************
void CPQMStudy::PdbFileCopy()
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::PdbFileCopy");
    //ENGINEが接続されているかチェックする。
    CString strEngine = getenv(ENV_ENGINE);

    if (strEngine.Compare(ENGINE_CONNECT_MODE))
        return;

    CString strMsg = _T("");
    CString strFromPath = _T("");   //コピー元のパス

    STARTUPINFO si;                 //ウィンドウ表示設定
    PROCESS_INFORMATION pi;         //プロセス・スレッドの情報

    int Handle_stat[2] = {0, 0};

    DWORD ExitCode = 0;             //プロセスの終了ステータス

    //コピー元のパスを作成する。
    strFromPath.Format(_T("/%s/%s"), STUDY_FOLDER, FNAME_PDBHEADER);

    //コピー先のパスを作成する。
    CString strToPath = _T("//");
    strToPath += getenv(ENV_ENGINE_HOST);
    strToPath += strFromPath;

    //ucp実行コマンドを作成する。
    CString strCmd = _T("ucp ") + strFromPath + _T(" ") + strToPath;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    //ucpを起動させる。
    if (FALSE == CreateProcess(NULL, (LPTSTR)(LPCTSTR)strCmd, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        //ucp起動失敗。ErrorLogとErrorViewにメッセージを出力する。
        strMsg.Format(_T("Command execution failed. [cmd : %s ]."), strCmd);
        CPQMLogMgr::GetInstance()->WriteEventLog(strMsg ,
                ERROR_LOG_CRUCIAL1,
                _T("pqmstudy.cpp"), __LINE__); // MEITEC/2010May24/IR-82

        strMsg.Format(_T("Failed to back up a .pdb_header file to Recon Engine."));
        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
            strMsg,
            MPlus::EH::Information,
            _T("pqmstudy.cpp"),
            _T("PQM"));

        return;
    }

    //プロセス終了を待つ。
    if (WAIT_OBJECT_0 == WaitForSingleObject(pi.hProcess, INFINITE)) {

        //プロセスの終了ステータスを取得する。
        if (GetExitCodeProcess(pi.hProcess, &ExitCode)) {

            if (!ExitCode) {
                //ファイルコピー成功。TraceLogにメッセージを出力する。
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Succeed : back up a .pdb_header file to Recon Engine."));

            } else {
                //ucp異常終了。ErrorLogとErrorViewにメッセージを出力する。
                strMsg.Format(_T("Failed to back up a .pdb_header file to Recon Engine."));
                CPQMLogMgr::GetInstance()->WriteEventLog(strMsg ,
                        ERROR_LOG_CRUCIAL1,
                        _T("pqmstudy.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82

                CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
                    strMsg,
                    MPlus::EH::Information,
                    _T("pqmstudy.cpp"),
                    _T("PQM"));

            }

        } else {
            //プロセス終了ステータス取得失敗。ErrorLogにメッセージを出力する。
            strMsg.Format(_T("GetExitCodeProcess() ERROR."));
            CPQMLogMgr::GetInstance()->WriteEventLog(strMsg ,
                    ERROR_LOG_CRUCIAL1,
                    _T("pqmstudy.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82

        }
    }

    Handle_stat[0] = CloseHandle(pi.hThread);
    Handle_stat[1] = CloseHandle(pi.hProcess);

    if (!Handle_stat[0] || !Handle_stat[1]) {
        strMsg.Format(_T("Failed to Handle close."));
        CPQMLogMgr::GetInstance()->WriteEventLog(strMsg ,
                ERROR_LOG_CRUCIAL1,
                _T("pqmstudy.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82
    }

    return;
}
//- MEITEC watanabe ADDED 10/01/14 //.pdb_header file copy

//***************************Method Header*************************************
//Method Name    :RenumberPositionsInVector()
//Author         :PATNI/MRP
//Purpose        :Set the position of all the protocols in the vector
//*****************************************************************************
void CPQMStudy::RenumberPositionsInVector(
)
{
    int pos = 1;
    CPqmProtocol*   result_acqseries = NULL;
    protocolvec::iterator v1_Iter  = NULL;//Patni-MJC/2009Aug17/Modified/cpp test corrections

    //Patni-DKH/2011Mar04/Added/TMSC-REDMINE-1486
    CScopedLock l_scoped_lock(&m_protocol_vec_mutex);

    for (v1_Iter = m_protocol_vec.begin();
         v1_Iter != m_protocol_vec.end();
         v1_Iter++) {

        result_acqseries = *v1_Iter;

        //+Patni-SS/MODIFIED/2009-Jun-04/CRASH TMSC reported
        if (result_acqseries)
            result_acqseries->SetPosition(pos);

        pos++;
    }
}

//***************************Method Header*************************************
//Method Name    :PutDBDT_NormalMode()
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
void CPQMStudy::PutDBDT_NormalMode(
    CVarFieldHandle* const f_maintain_vf_handle,
    const int f_protocol_num
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::PutDBDT_NormalMode");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of PutDBDT_NormalMode"));

    VfFieldSpec_t l_vf_req[1] = {0};
    int l_mode = VFC_DBDT_IEC_OPERATING_NORMAL ;

    VFF_SET_ARGS(l_vf_req[0],
                 SVN_SEQ_DBDT_IEC_OPERATING_MODE,
                 SVT_SEQ_DBDT_IEC_OPERATING_MODE,
                 sizeof(l_mode),
                 &l_mode,
                 VFO_REPLACE);

    VfPathElem_t l_pelm[1] = {0};
    l_pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_pelm[SVD_ST_PROTOCOL - 1].index = f_protocol_num;

    //+Patni-SS/2009Apr01/PSP1#VFLOCK
    CString l_log_msg = _T("");
    l_log_msg.Format(_T(" SVT_SEQ_DBDT_IEC_OPERATING_MODE =%d"), l_mode);

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_log_msg);
    //-Patni-SS/2009Apr01/PSP1#VFLOCK
    time_t l_tm = 0;
    int l_num = 0;
    status_t l_status = f_maintain_vf_handle->PutFields(l_pelm, SVD_ST_PROTOCOL,
                        l_vf_req, 1, &l_num, &l_tm);

    if (E_NO_ERROR != l_status || l_num != 1) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
    }

    CString l_strLog(_T(""));
    l_strLog.Format(L"Status of f_maintain_vf_handle->PutFields() is:- %d", l_status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_strLog);

}

//***************************Method Header*************************************
//Method Name    :DeleteHoldMode()
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
void CPQMStudy::DeleteHoldMode(
    const int f_protocol
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::DeleteHoldMode");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of DeleteHoldMode"));

    VfFieldSpec_t l_vf_req[1] = {0};
    VFF_SET_ARGS(l_vf_req[0],
                 SVN_PLN_HOLD,
                 SVT_PLN_HOLD,
                 sizeof(int),
                 NULL,
                 VFO_DELETE);

    VfPathElem_t l_pelm[1] = {0};
    l_pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_pelm[SVD_ST_PROTOCOL - 1].index = f_protocol;

    time_t l_tm = 0;
    int l_num = 0;
    status_t l_status = m_vfdb_handle->PutFields(l_pelm, SVD_ST_PROTOCOL, l_vf_req,
                        1, &l_num, &l_tm);

    if (E_NO_ERROR != l_status || l_num != 1) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
    }

    CString l_strLog(_T(""));
    l_strLog.Format(L"Status of m_vfdb_handle->GetFields() is:- %d", l_status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_strLog);

}

//***************************Method Header*************************************
//Method Name    :ConvCouchOffset()
//Author         :PATNI / AC
//Purpose        :This method calculates the couch offset and shifts the offset
//                vector
//*****************************************************************************
void CPQMStudy::ConvCouchOffset(
    CPqmProtocol* protocol
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::ConvCouchOffset");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of ConvCouchOffset"));

    if (protocol->GetCouchOffsetFlag() == TRUE) {

        float offset = 0.0f;
        BITFLD_DECL(mask, K_ALL_PR);

        if ((fabs(protocol->GetPqmPrMode()->GetCouchOffset())) >= COUCH_MIN_VALUE) {

            offset = - protocol->GetPqmPrMode()->GetCouchOffset();

            ClibVfToUiAdapter vuiAdapter(m_vfdb_handle);

            //+Patni-ARD+SS/2009Sep25/Added/Defect Fix MVC4934
            if (PLAN_CHECK(protocol)) {
                vuiAdapter.VuiShiftOffsetVector(protocol->GetProtocol(),
                                                VUI_SHIFT_MODE_SG,
                                                VUI_SHIFT_Z,
                                                offset);

            } else {
                vuiAdapter.VuiShiftOffsetVector(protocol->GetProtocol(),
                                                VUI_SHIFT_MODE_VG,
                                                VUI_SHIFT_Z,
                                                -offset);
            }

            //-Patni-ARD+SS/2009Sep25/Added/Defect Fix MVC4934
        }

        offset = 0.0f;
        protocol->GetPqmPrMode()->SetCouchOffset(0.0f);

        BITFLD_INIT(mask, K_ALL_PR, 0);

        SetProtocolValues(m_vfdb_handle,
                          protocol->GetProtocol(),
                          mask,
                          K_PR_COUCH_OFFSET,
                          &offset,
                          sizeof(float),
                          NULL);
        protocol->SetCouchOffsetFlag(FALSE);
    }
}

//***************************Method Header*************************************
//Method Name    :CheckImgGr()
//Author         :PATNI/SG
//Purpose        :
//*****************************************************************************
int CPQMStudy::CheckImgGr(
    CVarFieldHandle* const f_vf_handle,
    const int f_protocol
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::CheckImgGr");

    if (!f_vf_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_vf_handle is NULL"));
        return E_ERROR;
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMStudy::CheckImgGr"));

    ClibVfToUiAdapter vuiAdapter(f_vf_handle);
    int imgGr[2] = {0}; // SM4 Defect#60 AMIT + Himanshu
    status_t l_status = vuiAdapter.VuiGetProtocolImgGr(f_protocol, imgGr, 2);
    // For Group Number
    if_error(l_status) {
        // + SM4 Defect#60 AMIT + Himanshu
        // This condition is not in Vantage source code.
        // Condition put here after discussion with TMSC for following reason
        // There are 2 conditions
        // 1) Error occured after adding new protocol
        // 2) Error occured before adding new protocol
        //     if error has occured before adding protocol, then its not required to
        //     delete protocol. Hence below condition is added to prevent it.
        // below code change will take care for both the condition.
        if (!(imgGr[0] == 0 && imgGr[1] == 0)) {
            if (E_NO_ERROR != vuiAdapter.VuiDeleteProtocol(f_protocol)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("VuiDeleteProtocol() failed."));
                return E_ERROR;
            }

            f_vf_handle->ReOpen();
        }

        // + SM4 Defect#60 AMIT + Himanshu

        CString l_error_string(_T(""));
        l_error_string.Format(_T("VuiGetProtocolImgGr error. prot=%d"), f_protocol);

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_error_string);


        if (l_error_string.LoadString(IDS_MAX_PROTOCOL) != TRUE) {

            //+Patni-ARD/2009-Nov-25/Added/DeFT# MVC003149 + MVC005179
            CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
                CPqmUtilities::GetMultiLingualString(_T("IDS_NO_SEQ_ACCEPATANCE")),
                MPlus::EH::Information, _T("PQMStudy.cpp"), _T("PQM"));
            //-Patni-ARD/2009-Nov-25/Added/DeFT# MVC003149 + MVC005179
        }

        //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(l_error_string,
                MPlus::EH::Information, _T("pqmstudy.cpp"), _T("PQM"));

        return E_ERROR;
    }
    return E_NO_ERROR;
}


//***************************Method Header*************************************
//Method Name    :CopyError()
//Author         :PATNI/SG
//Purpose        :
//*****************************************************************************
int CPQMStudy::CopyError(
    const int f_protocol,
    char* f_msg,
    char* f_file_name,
    const int f_line_no
)const
{
    //+Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(f_protocol);
    UNREFERENCED_PARAMETER(f_msg);
    UNREFERENCED_PARAMETER(f_line_no);
    //-Patni-KSS/2011Feb18/Added/CPP Test tool Execution

    CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
        CPqmUtilities::GetMultiLingualString(_T("IDS_COPY_ERROR")),
        MPlus::EH::Crucial,
        f_file_name,
        _T("PQM"));
    return E_NO_ERROR;
}

//***************************Method Header*************************************
//Method Name    :ClearFields()
//Author         :PATNI/SG
//Purpose        :
//*****************************************************************************
int CPQMStudy::ClearFields(CVarFieldHandle* const f_vf_handle,
                           const int f_protocol
                          )const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::ClearFields");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMStudy::ClearFields"));

    VfFieldSpec_t   vfReq[8] = {0};
    int n = 0;

    VFF_SET_ARGS(vfReq[n],
                 SVN_EXE_PRESCAN_RFL_GAIN_VALUE,
                 SVT_EXE_PRESCAN_RFL_GAIN_VALUE,
                 0,
                 NULL,
                 VFO_DELETE);

    n++;

    VFF_SET_ARGS(vfReq[n],
                 SVN_EXE_PRESCAN_RFL_SCALE_VALUE,
                 SVT_EXE_PRESCAN_RFL_SCALE_VALUE,
                 0,
                 NULL,
                 VFO_DELETE);

    n++;

    VFF_SET_ARGS(vfReq[n],
                 SVN_EXE_COUCH,
                 SVT_EXE_COUCH,
                 0,
                 NULL,
                 VFO_DELETE);

    n++;

    VFF_SET_ARGS(vfReq[n],
                 SVN_EXE_ACQ_CONDITION,
                 SVT_EXE_ACQ_CONDITION,
                 0,
                 NULL,
                 VFO_DELETE);

    n++;
    int32_t     protect = 0 ;
    VFF_SET_ARGS(vfReq[n],
                 SVN_PRC_PROTECT_FLAG,
                 SVT_PRC_PROTECT_FLAG,
                 sizeof(int32_t),
                 &protect,
                 VFO_REPLACE);

    n++;
    //Patni-MRP/2009Aug16/Commented/MVC002872 Reset Plan info
    VfPathElem_t    l_locImages[4] = {0};
    VFF_SET_ARGS(vfReq[n],
                 SVN_PLN_LOCATOR_IMAGES,
                 SVT_PLN_LOCATOR_IMAGES,
                 sizeof(l_locImages),
                 &l_locImages,
                 VFO_REPLACE);
    n++;


    //+Patni-SS/2009Apr01/PSP1#VFLOCK
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMStudy::ClearFields"));
    //-Patni-SS/2009Apr01/PSP1#VFLOCK
    int num_put = 0;
    time_t      tmStamp = 0;
    VfPathElem_t    path[5] = {0};
    path[0].subtree_name = SVN_ST_PROTOCOL;
    path[0].index = f_protocol;

    status_t l_status = f_vf_handle->PutFields(path, SVD_ST_PROTOCOL, vfReq,
                        n, &num_put, &tmStamp);

    if (E_NO_ERROR != l_status || num_put != n) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
    }

    return E_NO_ERROR;


}

//***************************Method Header*************************************
//Method Name    :ResetProtInfo()
//Author         :PATNI/SG
//Purpose        :
//*****************************************************************************
int CPQMStudy::ResetProtInfo(
    CVarFieldHandle* const f_vf_handle,
    const int f_prot,
    void*   f_data
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::ResetProtInfo");
    PQM_FUNC_TRACE(_T("CPQMStudy::ResetProtInfo"));

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMStudy::ResetProtInfo"));

    VfPathElem_t path[4] = {0};
    bool_t delImgGr = false;
    delImgGr = (bool_t)f_data;
    VfPathElem_t    pelm[] = {{SVN_ST_PROTOCOL,    0}};
    BITFLD_DECL(mask, MAX_NODES);
    f32vec3_t vec;
    vec.x = vec.y = vec.z = .0;
    memset(path, 0, sizeof(path));
    memset(mask, 0, sizeof(mask));
    //+Patni-ARD/2009Jul15/Modified/Defect #MVC3315
    char*            l_buffer = new char[sizeof(char)];
    memset(l_buffer, 0, sizeof(char));
    int32_t         l_image_id_len = 1;
    wcstombs(l_buffer, _T("\0"), l_image_id_len);
    //-Patni-ARD/2009Jul15/Modified/Defect #MVC3315
    bool_t  fluoro = false;
    status_t status = GetProtocolValues(f_vf_handle,
                                        f_prot,
                                        mask,
                                        K_PR_SEQ_FLUORO,
                                        &fluoro,
                                        NULL,
                                        NULL);

    if (status != E_NO_ERROR || BITFLD_ON(mask, K_PR_SEQ_FLUORO)) {

        fluoro = FALSE;
    }

    int mode = 0;
    VfFieldSpec_t fieldSpec[7] = {0};
    memset(fieldSpec, 0, sizeof(fieldSpec));

    path[0].subtree_name = SVN_ST_PROTOCOL;
    path[0].index = f_prot;
    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(fieldSpec, 0, sizeof(VfFieldSpec_t) * 4);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008

    VFF_SET_ARGS(fieldSpec[0],
                 SVN_PLN_SCANOFFSET_SET_FLAG,
                 SVT_PLN_SCANOFFSET_SET_FLAG,
                 sizeof(mode),
                 &mode,
                 0L);
    int numDone = 0;
    int sts = f_vf_handle->GetFields(path, SVD_ST_PROTOCOL, fieldSpec, 1, &numDone);

    ClibVfToUiAdapter vuiAdapter(f_vf_handle);
    int32_t prot_status = 0, scan_offset_flag = 0;

    if ((sts == E_NO_ERROR) && (numDone == 1) &&
        (fieldSpec[0].status != VFE_NOT_FOUND)) {

        if (mode == 1) {

            scan_offset_flag = 1;
            vuiAdapter.VuiGetOffsetVector(f_prot, &vec);

            if (vec.z != 0) {

                prot_status |= VFC_PLN_STATUS_GLOC;
            }

            //+Patni-ARD/2010APR15/Added/JaFT# MVC007178

        } else {
            prot_status = 0;
        }

        //-Patni-ARD/2010APR15/Added/JaFT# MVC007178
    }

    //    if (PqmGlobal.SliceNumSarchSlab == 0 ) {
    mode = VFC_SLICE_NUMBER_METHOD_SLICES;
    //    } else {
    //      mode = VFC_SLICE_NUMBER_METHOD_SLAB;
    //    }

    memset(path, 0, sizeof(path));
    memset(mask, 0, sizeof(mask));
    memset(fieldSpec , 0, sizeof(fieldSpec)) ;

    path[0].subtree_name = SVN_ST_PROTOCOL;
    path[0].index = f_prot;

    VFF_SET_ARGS(fieldSpec[0],
                 SVN_PLN_SLICE_NUMBER_METHOD,
                 SVT_PLN_SLICE_NUMBER_METHOD,
                 sizeof(mode),
                 &mode,
                 VFO_REPLACE);

    time_t ts = 0;
    CString l_log_msg = _T("");

    for (int rcnt = 0; rcnt < 3; rcnt++) {

        //+Patni-SS/2009Apr01/PSP1#VFLOCK
        l_log_msg.Format(_T("SVN_PLN_SLICE_NUMBER_METHOD = %d"), mode);
        //Patni-ARD/2009Jul15/Modified/Code review
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_msg);
        //-Patni-SS/2009Apr01/PSP1#VFLOCK

        status = f_vf_handle->PutFields(path, SVD_ST_PROTOCOL, fieldSpec, 1, &numDone, &ts);

        if (status == E_NO_ERROR && numDone == 1) {
            break;
        }
    }

    ///******   Clear field for store protocol. *****/
    memset(mask, 0, sizeof(mask));

    pelm[SVD_ST_PROTOCOL - 1].index = f_prot;
    memset(path, 0, sizeof(path));
    bool_t  calc_sar_flag = false;
    int32_t autoHold = 0, prescan = VFC_PRESCAN_CLINICAL;
    int32_t acqStatus = C_PR_STATUS_WAIT;

    SetProtocolValues(f_vf_handle,
                      f_prot,
                      mask,
                      K_PR_ACQ_STATUS,	&acqStatus,	sizeof(int32_t),
                      K_PR_AUTO_HOLD,	&autoHold,	sizeof(int32_t),
                      K_PR_CALC_SAR,		&calc_sar_flag,	sizeof(bool_t),
                      K_PR_STATUS,		&prot_status,	sizeof(int32_t),
                      K_PR_PRESCAN_FSY,	&prescan,	sizeof(int32_t),
                      K_PR_PRESCAN_RFL,	&prescan,	sizeof(int32_t),
                      NULL);

    if (!fluoro) {

        memset(mask, 0, sizeof(mask));
        SetProtocolValues(f_vf_handle,
                          f_prot,
                          mask,
                          K_PR_PRESCAN_RGN,
                          &prescan,
                          sizeof(int32_t),
                          NULL);
    }

    memset(path, 0, sizeof(path));
    //+Patni-ARD/2009Jul15/Added/Defect #MVC3315
    memset(fieldSpec, 0, sizeof(VfFieldSpec_t) * 7);

    int n = 0;
    VFF_SET_ARGS(fieldSpec[n],
                 SVN_PLN_LOCATOR_IMAGE_ID,
                 SVT_PLN_LOCATOR_IMAGE_ID,
                 l_image_id_len,
                 l_buffer,
                 VFO_ALLOCATE | VFO_REPLACE);
    n++;

    VFF_SET_ARGS(fieldSpec[n],
                 SVN_PLN_LOCATOR_IMAGE_ID_2,
                 SVT_PLN_LOCATOR_IMAGE_ID_2,
                 l_image_id_len,
                 l_buffer,
                 VFO_ALLOCATE | VFO_REPLACE);
    n++;

    VFF_SET_ARGS(fieldSpec[n],
                 SVN_PLN_LOCATOR_IMAGE_ID_3,
                 SVT_PLN_LOCATOR_IMAGE_ID_3,
                 l_image_id_len,
                 l_buffer,
                 VFO_ALLOCATE | VFO_REPLACE);
    n++;
    VFF_SET_ARGS(fieldSpec[n],
                 SVN_PLN_LOCATOR_IMAGE_ID_4,
                 SVT_PLN_LOCATOR_IMAGE_ID_4,
                 l_image_id_len,
                 l_buffer,
                 VFO_ALLOCATE | VFO_REPLACE);
    n++;
    VFF_SET_ARGS(fieldSpec[n],
                 SVN_PLN_LOCATOR_IMAGE_ID_5,
                 SVT_PLN_LOCATOR_IMAGE_ID_5,
                 l_image_id_len,
                 l_buffer,
                 VFO_ALLOCATE | VFO_REPLACE);
    n++;

    VFF_SET_ARGS(fieldSpec[n],
                 SVN_PLN_LOCATOR_IMAGE_ID_6,
                 SVT_PLN_LOCATOR_IMAGE_ID_6,
                 l_image_id_len,
                 l_buffer,
                 VFO_ALLOCATE | VFO_REPLACE);
    n++;
    //Patni-AMT/2009Aug14/Modified/ACE-2 MVC002872
    status = f_vf_handle->PutFields(pelm, SVD_ST_PROTOCOL, fieldSpec, n, &numDone, &ts);

    //Patni-PJS/2010Feb24/Added/DeFT# PROPOSAL_08-Remove EZScan
    if (E_NO_ERROR != status || numDone != n) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
    }

    memset(mask, 0, sizeof(mask));
    SetProtocolValues(f_vf_handle,
                      f_prot,
                      mask,
                      K_PR_LOCATOR_IMAGE, path,   4 * sizeof(VfPathElem_t),
                      NULL);

    //+Patni-ARD/2010Jan6/Added/DeFT#29 IR 100
    if (m_pqm_ptr->GetOffsetVecReset() && scan_offset_flag == 0) {
        vuiAdapter.VuiSetOffsetVector(f_prot, &vec);
    }

    //-Patni-ARD/2010Jan6/Added/DeFT#29 IR 100

    //+Patni-MJC/2009Aug27/Modified/cpp test corrections
    DEL_PTR_ARY(l_buffer);

    //-Patni-MJC/2009Aug27/Modified/cpp test corrections
    //Patni-ARD/2009July21/Modified/Defect #MVC3315

    return E_NO_ERROR;

}

//***************************Method Header*************************************
//Method Name    :AddProtocol()
//Author         :PATNI/SG
//Purpose        :
//*****************************************************************************
int CPQMStudy::AddProtocol(CVarFieldHandle* const f_vf_handle,
                           const int f_to_prot,
                           int f_to_pos,
                           const int f_copyContrast,
                           CPqmProtocol** f_obj
                          )
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::AddProtocol");

    CString str = _T("");

    str.Format(_T("f_to_prot = %d \n f_to_pos = %d \n f_copyContrast = %d"), f_to_prot, f_to_pos, f_copyContrast);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str);

    //Patni-DKH/2011Mar04/Added/TMSC-REDMINE-1486
    CScopedLock l_scoped_lock(&m_protocol_vec_mutex);

    int l_pos = AddProtocolObject(f_vf_handle, f_to_prot, f_to_pos);

    if (l_pos == 0) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("MSG_NO_MORE_PROTOCOL"));
    }


    CPqmProtocol* l_obj = GetProtocol(f_to_prot);

    //+Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    if (l_obj == NULL) {
        return E_ERROR;
    }


    CheckMRSValueInVF(l_obj);

    //-Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    int l_to_order = 0, l_zero_based = 0;

    if (f_to_pos == 0) {
        f_to_pos = l_pos;
        l_to_order = 0;

    } else {
        l_to_order = 1;
        l_zero_based = 1;
    }

    //Shift the Item in the vector here
    //Added by Meghana On 8/6/2008 9:17:51 AM
    //  __asm int 3;
    if (l_pos != f_to_pos) {
        //-1 because the position calculated is 1 based and in the vector it is 0 based
        MoveProtObjInVector((l_pos - 1), f_to_pos);
    }

    // Add protocol index to AcqOrder
    ClibVfToUiAdapter vuiAdapter(f_vf_handle);
    int l_status = vuiAdapter.VuiAddToAcqOrder(f_to_prot);

    if_error(l_status) {
        return CopyError(f_to_prot, NULL, __FILE__, __LINE__);
    }

    //set the acq order
    int l_from_order = vuiAdapter.VuiAcqNum(f_to_prot);


    l_pos = 0;

    CPqmProtocol*   result_acqseries = NULL;
    //Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    protocolvec::iterator v1_Iter = NULL;

    for (v1_Iter = m_protocol_vec.begin();
         v1_Iter != m_protocol_vec.end();
         v1_Iter++) {

        result_acqseries = *v1_Iter;

        if (++l_pos == f_to_pos) {
            break;
        }

        //+Patni-SS/MODIFIED/2009-Jun-04/CRASH TMSC reported
        if (result_acqseries != NULL && result_acqseries->GetProtocol() == PQ_FILLER) {
            continue;
        }

        l_to_order++;
    }

    l_status = vuiAdapter.VuiSetAcqOrder(l_from_order, l_to_order);

    if_error(l_status) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("VuiSetAcqOrder Error"));
        return CopyError(f_to_prot, NULL, __FILE__, __LINE__);
    }

    if (l_zero_based == 1) {
        f_to_pos ++;
    }

    CopyScanMode(f_vf_handle, f_to_pos, f_copyContrast);
    SearchScanMode(f_vf_handle, K_MODE_SET_IT);

    DeleteCFASpectrum(l_obj->GetProtocol());

    CPqmProtocol* l_prev_protocol = GetPrevItemFromPos(l_obj);
    int l_prev_acq_order = 0;

    if (l_prev_protocol != NULL) {
        l_prev_acq_order = l_prev_protocol->GetProtocol();

    } else {
        l_prev_protocol =  Get1stScanDone();

        if (l_prev_protocol != NULL) {
            l_prev_acq_order = l_prev_protocol->GetProtocol();

        } else {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("B1-Shim data will be copied from 0th AcqOrder Protocol"));
        }
    }

    if (l_obj->GetRflProt() == -1) {
        AddPrescanTagsForNextProtocol(m_vfdb_handle, NULL, l_prev_acq_order);
    }

    RecalculateSAR(l_obj);
    flt32_t l_curr_dbdt = 0.0;
    int l_dbdt_mode = 0;

    if (E_NO_ERROR == ReaddBdtOpMode(
            l_obj, l_dbdt_mode, l_curr_dbdt)) {
        if (l_dbdt_mode == VFC_DBDT_IEC_OPERATING_NORMAL) {
            l_obj->SetdBdtModeString(DBDT_NORMAL_LABEL);

        } else if (l_dbdt_mode == VFC_DBDT_IEC_OPERATING_1ST_CONTROLLED) {
            l_obj->SetdBdtModeString(DBDT_MODE1_LABEL);

        } else if (l_dbdt_mode == VFC_DBDT_IEC_OPERATING_2ND_CONTROLLED) {
            l_obj->SetdBdtModeString(DBDT_MODE2_LABEL);
        }

    }

    //This is a temporary change added only for Phase-2 because
    //The scan comment of the earlier Done protocol is copied to the
    //second new protocol as soon as it is transferred. This is done by SR.
    //But if a blank space is there in Varfield database for the newly added protocol
    //DSLite copies back the blank space while conversion of the second protocol after scan is done
    //and hence indirectly we are able to maintain an empty comment for a protocol
    //If the user has not added any comment for it.
    CString l_str_comment = l_obj->GetScanComment();

    if (l_str_comment.IsEmpty()) {
        VARIANT* l_pvariant = new VARIANT;

        if (l_pvariant == NULL) {
            return E_ERROR;
        }

        l_pvariant->byref = _T(" ");
        l_pvariant->wReserved1 = SCAN_COMMENT;
        UpdateGUIControlsInVarfieldDB(l_pvariant, l_obj->GetProtocol());
        DEL_PTR(l_pvariant);
    }

    if (E_NO_ERROR != m_pqm_ptr->GetdBdt()->dBdtCalc(f_vf_handle, l_obj)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("dBdtCalc() failed."));
    }

    if (f_obj != NULL) {
        *f_obj = l_obj;
    }

    if (TRUE == CheckMulitScanDynamic_License(l_obj, f_to_prot)) { //Redmine2187
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CheckMulitScanDynamic_License() is not available"));
    }

    return E_NO_ERROR;
}
//*****************************************************************************
//Method Name    : UpdateSARControlInfoOnProtocolUpdate
//Author         : iGATE
//Purpose        :
//*****************************************************************************
void  CPQMStudy::UpdateSARControlInfoOnProtocolUpdate(CPqmProtocol* f_protocol_obj)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::UpdateSARControlInfoOnProtocolUpdate");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));


    if (NULL == f_protocol_obj) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_protocol_obj is NULL."));
        return;
    }

    BITFLD_DECL(mask, K_ALL_PR);
    memset(mask, 0, sizeof(mask));


    BOOL		l_sar_ctrl_flag = FALSE;

    if (m_pqm_ptr->GetSARControlLicense()) {

        l_sar_ctrl_flag = TRUE;

    }

    if (E_NO_ERROR != SetProtocolValues(m_vfdb_handle, f_protocol_obj->GetProtocol(), mask,
                                        K_PR_RTSAR_CTRL_FLAG, &l_sar_ctrl_flag, sizeof(l_sar_ctrl_flag), NULL)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetProtocolValues failed"));
        return;
    }

    UpdateFixedOpeModeForSARControl(f_protocol_obj);
}
//***************************Method Header*************************************
//Method Name    :ListCopyCurrent()
//Author         :PATNI/SG
//Purpose        :
//*****************************************************************************
status_t CPQMStudy::ListCopyCurrent(
    CVarFieldHandle* const f_vf_handle,
    const int f_pos,
    int*    f_to_pos
)
{
    //+Patni-AMT/2010Mar22/Modified/Ph-3# IR-100
    LPCTSTR FUNC_NAME = _T("CPQMStudy::ListCopyCurrent");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMStudy::ListCopyCurrent"));
    char l_current_file_name[256] = {0};
    CPqmUtilities::GetCurrScatchpadFileName(l_current_file_name);

    //+Patni-SS/2009Apr01/PSP1#VFLOCK
    CString l_log_msg = _T("");
    l_log_msg.Format(_T(" :VuiCopyProtocolFromFile :"));
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_log_msg);
    //-Patni-SS/2009Apr01/PSP1#VFLOCK
    ClibVfToUiAdapter vuiAdapter(f_vf_handle);
    int l_index = 0;
    status_t l_status = vuiAdapter.VuiCopyProtocolFromFile(-1, l_current_file_name, 0, 0, &l_index);
    if_error(l_status) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("VuiCopyProtocolFromFile() failed."));
        return E_ERROR;
    }

    //Patni-SS/MODIFIED/2009-Jun-02/JFT#213

    //+Patni-PJS/2010Jun7/Modified/MVC008019
    //l_obj = ListCopyProt(f_vf_handle, l_index, (f_pos == 0 && (m_wait_pos > 1)) ? (m_wait_pos - 1) : f_pos); //m_wait_pos is one base index and ListCopyProt need 0 base index
    //-Patni-PJS/2010Jun7/Modified/MVC008019
    CPqmProtocol*   l_obj = ListCopyProt(f_vf_handle, l_index, (f_pos == 0 ? (GetAllItems()) : f_pos));

    if (f_to_pos != NULL) {
        if (l_obj != NULL)
            *f_to_pos = (l_obj->GetPosition() - 1);//To be insynch with all other functions

        else
            *f_to_pos = 0;
    }

    if (l_obj == NULL) {
        return E_ERROR;
    }

    //CheckVisualGr(CurrFile,"FromCurr",0,__FILE__,__LINE__);
    return E_NO_ERROR;
    //-Patni-AMT/2010Mar22/Modified/Ph-3# IR-100
}


//***************************Method Header*************************************
//Method Name    :ListCopyItem()
//Author         :PATNI/SG
//Purpose        :
//*****************************************************************************
status_t CPQMStudy::ListCopyItem(
    /*VfDBHandle_t f_vf_handle,*/
    const int f_from_pos,
    const int f_to_pos
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::ListCopyItem");

    CString str(_T(""));
    str.Format(_T("f_from_pos = %d \n f_to_pos = %d"), f_from_pos,  f_to_pos);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str);

    if (!m_vfdb_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_vfdb_handle is NULL"));
        return E_ERROR;
    }

    //Added by Meghana On 8/6/2008 2:42:19 PM
    int f_from_pos_1based = f_from_pos + 1;
    CPqmProtocol* l_frobj = GetItemFromPosition(f_from_pos_1based);

    //ASSERT(l_frobj != NULL);
    if (l_frobj == NULL) return E_ERROR;

    int l_from_prot = l_frobj->GetProtocol();

    if (l_from_prot < 0) {/* PQ_FILLER */
        return E_ERROR;
    }

    int l_to_prot = -1;
    m_vfdb_handle->ReOpen();
    //+Patni-SS/2009Apr01/PSP1#VFLOCK
    CString l_log_msg = _T("");
    l_log_msg.Format(_T(" :VuiCopyProtocol :"));

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_msg);
    //-Patni-SS/2009Apr01/PSP1#VFLOCK
    ClibVfToUiAdapter vuiAdapter(m_vfdb_handle);
    int status = vuiAdapter.VuiCopyProtocol(l_from_prot, -1, 0, &l_to_prot);
    if_error(status) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("VuiCopyProtocol() failed."));
        return E_ERROR;
    }
    m_vfdb_handle->ReOpen();

    if_error(status) {

        if (l_to_prot != -1) {
            if (E_NO_ERROR != vuiAdapter.VuiDeleteProtocol(l_to_prot)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("VuiDeleteProtocol() failed."));
                return E_ERROR;
            }

            m_vfdb_handle->ReOpen();
        }

        //      PqmUiErrorDialog(PqmUiFetchLiteral("ERR_PROTOCOL_COPY"),ERR_ERROR);
        //      PqmPqInitList();
        return E_ERROR;
    }
    //+Patni-SS/COMMENTED/2009-May-20/AcqOrder Defect
    //m_prot_count = l_to_prot;

    CPqmProtocol* l_obj = ListCopyProt(m_vfdb_handle, l_to_prot, f_to_pos);

    if (l_obj != NULL) {

        if (l_frobj->GetPqmPrMode()->GetScanNumber() !=
            l_obj->GetPqmPrMode()->GetScanNumber()) {
            //Meghana modified here for defect fix.....
            InitPatientPathfromCopy(m_vfdb_handle, l_obj, l_frobj);
            GetProtocolItemValues(m_vfdb_handle, l_obj);
            SearchScanMode(m_vfdb_handle, K_MODE_SET_IT);
            //PqmPrMakeList(vfd,obj,0);

        } else if (l_frobj->GetAcqStatus() == C_PR_STATUS_DONE ||
                   l_frobj->GetAcqStatus() == C_PR_STATUS_FAIL) {

            BITFLD_DECL(mask, MAX_NODES);

            /*  if ((l_frobj->GetHasParent() || l_frobj->GetPlnStatus() &
            VFC_PLN_STATUS_GLOC)) { Meenakshi */
            if (PLAN_CHECK(l_frobj)) {

                vuiAdapter.VuiShiftOffsetVector(l_obj->GetProtocol(),
                                                VUI_SHIFT_MODE_SG, VUI_SHIFT_Z,
                                                -l_frobj->GetPqmPrMode()->GetCouchOffset());

            } else {
                vuiAdapter.VuiShiftOffsetVector(l_obj->GetProtocol(),
                                                VUI_SHIFT_MODE_VG, VUI_SHIFT_Z,
                                                l_frobj->GetPqmPrMode()->GetCouchOffset());
            }

            l_obj->GetPqmPrMode()->SetCouchOffset(0);
            float l_couch_offset = l_obj->GetPqmPrMode()->GetCouchOffset();
            memset(mask, 0, sizeof(mask));
            SetProtocolValues(m_vfdb_handle,
                              l_obj->GetProtocol(),
                              mask,
                              K_PR_COUCH_OFFSET,
                              &l_couch_offset,
                              sizeof(flt32_t),
                              NULL);
            l_obj->SetCouchOffsetFlag(FALSE);
        }

    } else { // + SM4 Def 60 HIMANSHU
        return E_ERROR;
    }

    // + SM4 Def 60 HIMANSHU
    if (l_obj) {
        ClearFieldsForCopy(m_vfdb_handle, l_obj->GetProtocol());
    }

    return E_NO_ERROR;
}

//***************************Method Header*************************************
//Method Name    :InitPatientPathfromCopy()
//Author         :PATNI/SG
//Purpose        :
//*****************************************************************************
void CPQMStudy::InitPatientPathfromCopy(
    CVarFieldHandle* const f_vf_handle,
    CPqmProtocol* l_obj,
    CPqmProtocol* l_fromobj
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::InitPatientPathfromCopy");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMStudy::InitPatientPathfromCopy"));
    VfPathElem_t        path = {0};
    BITFLD_DECL(mask, K_ALL_PR);
    //+Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    int32_t     prot_status = 0;
    flt32_t     parent_couchoffset = 0.0f ;
    //-Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    DB_FUNC_ENTER("[PqmPrInitPatientPath]:");

    path.subtree_name = 0;
    path.index  = 0;

    memset(mask, 0, sizeof(mask));
    SetProtocolValues
    (f_vf_handle, l_obj->GetProtocol(), mask,
     K_PR_LOCATOR_IMAGE, &path, sizeof(VfPathElem_t),
     K_PR_STATUS,       &prot_status,    sizeof(int32_t),
     K_PR_PARENT_COUCH_OFFSET, &parent_couchoffset, sizeof(flt32_t),
     NULL);

    if (l_fromobj != NULL) {
        //+Patni-PJS/2009Aug18/Modified/Cpp test tool execution.
        //-Patni-PJS/2009Aug18/Modified/Cpp test tool execution.
        /* ここではOffset情報が確定しているのでそのままそのまま使用する */
        float offset = l_fromobj->GetPqmPrMode()->GetCouchOffset();

        if (l_fromobj->GetAcqStatus() == C_PR_STATUS_DONE ||
            l_fromobj->GetAcqStatus() == C_PR_STATUS_FAIL) {
            BITFLD_DECL(mask, MAX_NODES);

            if (PLAN_CHECK(l_obj)) {
                l_obj->SetHasParent(FALSE);
                //memset(l_obj->locImages,0,sizeof(obj->locImages));
                int l_pln_status = l_obj->GetPlnStatus();
                l_pln_status &= ~VFC_PLN_STATUS_GLOC;
                l_obj->SetPlnStatus(l_pln_status);
            }

            /* VGをカレントポジション座標系に変換  HC->CC
               位置決め有り(Dp)でも、親Pathを切ったので位置決め無し(W)にする。
            */
            //VuiShiftOffsetVector(f_vf_handle,l_obj->GetProtocol(),
            //  VUI_SHIFT_MODE_VG, VUI_SHIFT_Z, offset);

            ClibVfToUiAdapter vuiAdapter(f_vf_handle);
            vuiAdapter.VuiShiftOffsetVector(l_obj->GetProtocol(),
                                            VUI_SHIFT_MODE_VG, VUI_SHIFT_Z, offset);
            l_obj->GetPqmPrMode()->SetCouchOffset(0);
            memset(mask, 0, sizeof(mask));
            float l_couch_offset = 0.0f;

            if (E_NO_ERROR != SetProtocolValues
                (f_vf_handle, l_obj->GetProtocol(), mask,
                 K_PR_COUCH_OFFSET, &l_couch_offset, sizeof(flt32_t),
                 NULL)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetProtocolValues() failed."));
            }

            l_obj->GetPqmPrMode()->SetCouchOffset(l_couch_offset);
            l_obj->SetCouchOffsetFlag(FALSE);
        }
    }

}
//***************************Method Header*************************************
//Method Name    :ClearFieldsForCopy()
//Author         :PATNI/SG
//Purpose        :
//*****************************************************************************
void CPQMStudy::ClearFieldsForCopy(
    CVarFieldHandle* const f_vf_handle,
    const int f_prot
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::ClearFieldsForCopy");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMStudy::ClearFieldsForCopy"));

    VfFieldSpec_t   vfReq[8] = {0};
    VfPathElem_t    path[5] = {0};
    int32_t     protect = 0 ;
    memset(path, 0, sizeof(VfPathElem_t) * 5);
    path[0].subtree_name = SVN_ST_PROTOCOL;
    path[0].index = f_prot;
    int n = 0;
    //Patni-PJS/2009Aug18/Added/Cpp test tool execution.
    memset(vfReq, 0, sizeof(VfFieldSpec_t) * 8);
    VFF_SET_ARGS(vfReq[n],
                 SVN_PRC_PROTECT_FLAG,
                 SVT_PRC_PROTECT_FLAG,
                 sizeof(int32_t), &protect, VFO_REPLACE);
    n++;

    //+Patni-SS/2009Apr01/PSP1#VFLOCK
    CString l_log_msg = _T("");
    l_log_msg.Format(_T(" SVT_PRC_PROTECT_FLAG =%d"), protect);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_msg);
    //-Patni-SS/2009Apr01/PSP1#VFLOCK
    int num_put = 0;
    time_t      tmStamp = 0;
    f_vf_handle->PutFields(path, SVD_ST_PROTOCOL, vfReq, n, &num_put, &tmStamp);
}

//***************************Method Header*************************************
//Method Name    :ResetOffset()
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
int CPQMStudy::ResetOffset(
    CVarFieldHandle* const f_vf_handle,
    const int f_protocol
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::ResetOffset");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of ResetOffset"));

    VfPathElem_t l_path[4] = {0};
    l_path[0].subtree_name = SVN_ST_PROTOCOL;
    l_path[0].index = f_protocol;
    l_path[1].subtree_name = SVN_ST_SUBPROTOCOL;
    l_path[1].index = 0;


    VfPathElem_t l_loc_images[4] = {0};
    memset(l_loc_images, 0, sizeof(l_loc_images)) ;

    VfFieldSpec_t l_vf_req[8] = {0};
    memset(l_vf_req, 0, sizeof(l_vf_req));
    int l_acq_status = VFC_ACQ_STATUS_WAIT;
    float l_couch_offset = PQM_COUCH_INIT_POSITION;

    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(l_vf_req, 0, sizeof(VfFieldSpec_t) * 8);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    VFF_SET_ARGS(l_vf_req[0],
                 SVN_EXE_PROTOCOL_ACQ_STATUS,
                 SVT_EXE_PROTOCOL_ACQ_STATUS,
                 sizeof(l_acq_status),
                 &l_acq_status,
                 0L);

    VFF_SET_ARGS(l_vf_req[1],
                 SVN_EXE_COUCH_OFFSET, VFT_FLT32,
                 sizeof(l_couch_offset),
                 &l_couch_offset,
                 0L);

    VFF_SET_ARGS(l_vf_req[2],
                 SVN_PLN_LOCATOR_IMAGES,
                 SVT_PLN_LOCATOR_IMAGES,
                 sizeof(l_loc_images),
                 &l_loc_images,
                 0L);
    int l_num = 0;
    status_t l_status = f_vf_handle->GetFields(l_path, SVD_ST_PROTOCOL, l_vf_req, 3, &l_num);

    if (l_status == E_NO_ERROR) {

        if (l_vf_req[0].status != E_NO_ERROR ||
            l_vf_req[1].status != E_NO_ERROR) {

            return(VarfieldInitOffset(f_vf_handle, l_path));
        }

        bool l_has_parent = FALSE;

        if (l_vf_req[2].status == E_NO_ERROR &&
            l_loc_images[0].subtree_name > 0) {

            l_has_parent = TRUE ;
        }

        if (fabs(l_couch_offset) > COUCH_MIN_VALUE) {

            ClibVfToUiAdapter vuiAdapter(f_vf_handle);

            if (l_has_parent) {

                if (l_acq_status == VFC_ACQ_STATUS_SUCCESS ||
                    l_acq_status == VFC_ACQ_STATUS_FAIL ||
                    l_acq_status == VFC_ACQ_STATUS_CURRENT) {

                    status_t l_status = vuiAdapter.VuiShiftOffsetVector(f_protocol, VUI_SHIFT_MODE_VG,
                                        VUI_SHIFT_Z, l_couch_offset);

                } else {
                    /* HH -> CC */
                    l_status = vuiAdapter.VuiShiftOffsetVector(f_protocol, VUI_SHIFT_MODE_SG,
                               VUI_SHIFT_Z, l_couch_offset);
                    l_status = vuiAdapter.VuiShiftOffsetVector(f_protocol, VUI_SHIFT_MODE_VG,
                               VUI_SHIFT_Z, l_couch_offset);
                }

            } else {
                if (l_acq_status == VFC_ACQ_STATUS_SUCCESS ||
                    l_acq_status == VFC_ACQ_STATUS_FAIL ||
                    l_acq_status == VFC_ACQ_STATUS_CURRENT) {

                    status_t l_status = vuiAdapter.VuiShiftOffsetVector(f_protocol, VUI_SHIFT_MODE_VG,
                                        VUI_SHIFT_Z, l_couch_offset);

                } else {
                    ;/* WAITなので、既にCCの状態になっている。変換不要 */
                }
            }
        }

        return(VarfieldInitOffset(f_vf_handle, l_path));

    } else {
        //+Patni-Manish/2009Sept02/VFLOG/Added
        try {
            VarfieldInitOffset(f_vf_handle, l_path);

        } catch (...) {
            CString l_print_string = _T("");
            l_print_string.Format(_T("VarfieldInitOffset Crashed with handle=%x/pelm=/1.%d"),
                                  f_vf_handle, l_path[0].index);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_print_string);
        }

        //-Patni-Manish/2009Sept02/VFLOG/Added
        return(E_ERROR);
    }
}

//***************************Method Header*************************************
//Method Name    :VarfieldInitOffset()
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
int CPQMStudy::VarfieldInitOffset(
    CVarFieldHandle* f_vf_handle,
    VfPathElem_t* f_path
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::VarfieldInitOffset");

    VfFieldSpec_t l_vf_req[8] = {0};
    VfPathElem_t l_loc_images[4] = {0};

    int i = 0;

    VFF_SET_ARGS(l_vf_req[i],
                 SVN_EXE_PARENT_COUCH_OFFSET,
                 VFT_FLT32,
                 0,
                 NULL,
                 VFO_DELETE);

    i++;

    VFF_SET_ARGS(l_vf_req[i],
                 SVN_EXE_COUCH_OFFSET,
                 VFT_FLT32,
                 0,
                 NULL,
                 VFO_DELETE);

    i++;

    VFF_SET_ARGS(l_vf_req[i],
                 SVN_EXE_PROTOCOL_ACQ_STATUS,
                 SVT_EXE_PROTOCOL_ACQ_STATUS,
                 0,
                 NULL,
                 VFO_DELETE);

    i++;

    memset(l_loc_images, 0, sizeof(l_loc_images));

    VFF_SET_ARGS(l_vf_req[i],
                 SVN_PLN_LOCATOR_IMAGES,
                 SVT_PLN_LOCATOR_IMAGES,
                 sizeof(l_loc_images),
                 &l_loc_images,
                 VFO_REPLACE);

    i++ ;

    //+Patni-SS/2009Apr01/PSP1#VFLOCK
    CString l_log_msg = _T("");
    l_log_msg.Format(_T(" SVN_PLN_LOCATOR_IMAGES Index=%d,subtree_name= %d"), l_loc_images->index, l_loc_images->subtree_name);

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_log_msg);
    //-Patni-SS/2009Apr01/PSP1#VFLOCK
    int l_num_done = 0;
    time_t l_ts = time(0);
    return f_vf_handle->PutFields(f_path, SVD_ST_PROTOCOL, l_vf_req,
                                  i, (int*) & l_num_done, &l_ts);
}

//***************************Method Header*************************************
//Method Name    :AddProtocolObject()
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
int CPQMStudy::AddProtocolObject(
    CVarFieldHandle* const f_vf_handle,
    const int f_protocol,
    const int f_to_pos
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::AddProtocolObject");
    //Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    //Patni-PJS/2009Nov1/Commented/Code Review
    //int   l_fill_status = 0;
    CPqmProtocol* l_protocol_object = NULL;
    //DllT  *dll;

    CString str(_T(""));
    str.Format(_T("f_protocol = %d \n f_to_pos = %d"), f_protocol, f_to_pos);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str);

    //Patni-DKH/2011Mar04/Added/TMSC-REDMINE-1486
    CScopedLock l_prot_loc(&m_protocol_vec_mutex);

    if ((l_protocol_object = CreateProtocolObject(f_vf_handle, NULL, f_protocol)) == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CreateProtocolObject ERROR"));

        return 0;
    }

    DB_PRINTF("Y", ("prot = %d,acqStatus=%d\n", prot, obj->acqStatus));

    if (l_protocol_object->GetAcqStatus() == C_PR_STATUS_WAIT) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("acqStatus => C_PR_STATUS_WAIT"));
        //+Patni-PJS/2009Nov1/Commented/Code Review
        /*l_fill_status = PQ_FILLER_WAIT;
        l_fill_status = C_PR_STATUS_WAIT; */
        //-Patni-PJS/2009Nov1/Commented/Code Review

        // For Total time
        m_total_time += l_protocol_object->GetScanTime();

    } else {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("acqStatus => NOT WAIT PROTOCOL"));
        //+Patni-PJS/2009Nov1/Commented/Code Review
        /*l_fill_status = PQ_FILLER_DONE;
        l_fill_status = C_PR_STATUS_DONE; */
        //-Patni-PJS/2009Nov1/Commented/Code Review
    }

    //l_position = 1;

    //      for (v1_Iter = m_protocol_vec.begin();
    //      v1_Iter != m_protocol_vec.end();
    //      v1_Iter++ ) {
    //
    //          ASSERT(v1_Iter!=NULL) ;
    //          result_acqSeries = *v1_Iter;
    //          l_position++;
    //  }

    //  if(f_to_pos == 0){

    //Modified by Meghana
    m_protocol_vec.push_back(l_protocol_object);
    m_prot_count += 1;
    //  }
    const int l_position = m_protocol_vec.size();
    //l_position ++;

    l_protocol_object->SetPosition(l_position);


    GetValueFromVfSAR(l_protocol_object);

    /* set scan mode number to all protocols. */
    //SearchScanMode(f_handle, K_MODE_SET_ALL);

    return l_position;
}

//***************************Method Header*************************************
//Method Name    :CreateProtocolObject()
//Author         :PATNI/SG
//Purpose        :
//*****************************************************************************
CPqmProtocol* CPQMStudy::CreateProtocolObject(
    CVarFieldHandle* const f_vf_handle,
    CPqmProtocol* f_protocol_object,
    const int f_protocol
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::CreateProtocolObject");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMStudy::CreateProtocolObject"));

    if (f_protocol_object == NULL) {
        f_protocol_object = new CPqmProtocol();
    }

    CString l_protocol(_T(""));
    l_protocol.Format(_T("Protocol = %d"), f_protocol);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_protocol);

    f_protocol_object->SetProtocol(f_protocol);
    f_protocol_object->SetAcqStatus(C_PR_STATUS_WAIT);
    f_protocol_object->SetHoldScan(VFC_HOLD_PRE_SCAN);
    f_protocol_object->SetAutoTransfer(0);
    f_protocol_object->GetPqmPrMode()->SetRecvCoilId(0);
    f_protocol_object->GetPqmPrMode()->SetHomePos(PQM_COUCH_INIT_POSITION);
    f_protocol_object->GetPqmPrMode()->SetCouchPos(PQM_COUCH_INIT_POSITION);
    f_protocol_object->GetPqmPrMode()->SetCouchOffset(0.0f);
    f_protocol_object->GetPqmPrMode()->SetContrast(0);
    f_protocol_object->GetPqmPrMode()->SetAnatomy(0);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_protocol_object->GetPqmPrMode()->SetAnatomy = 0"));
    f_protocol_object->GetPqmPrMode()->SetScanNumber(0);
    f_protocol_object->SetGatingMethod(0);
    f_protocol_object->SetDynamic(FALSE);
    f_protocol_object->SetPrepScan(0);
    f_protocol_object->SetShimming(0);
    f_protocol_object->SetFluoro(FALSE);
    f_protocol_object->SetHasParent(FALSE);
    f_protocol_object->GetPqmPrMode()->SetPatoriEndIn(-1);
    f_protocol_object->GetPqmPrMode()->SetPatoriSideUp(-1);
    f_protocol_object->SetScanTime(0);

    /* For scan offset. */
    f_protocol_object->SetOffset(0, 0.0f);
    f_protocol_object->SetOffset(1, 0.0f);
    f_protocol_object->SetOffset(2, 0.0f);
    //Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    char l_scan_comment[SCAN_COMMENT_LENGTH * 2 + 1] = {0}; //Tanoue/MVC5849
    memset(l_scan_comment, 0, SCAN_COMMENT_LENGTH * 2 + 1); //Tanoue/MVC5849
    f_protocol_object->SetScanComment(l_scan_comment);
    f_protocol_object->SetCouchOffsetFlag(FALSE);
    f_protocol_object->SetSystemChannels(m_pqm_ptr->GetNumChannels());

    //Add the function to reset SAR
    //+Patni-MJC/2009Aug27/Modified/cpp test corrections
    if (m_pqm_ptr == NULL) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));

    } else {
        m_pqm_ptr->GetSAR()->ResetSARInfo(f_protocol_object);
    }

    //-Patni-MJC/2009Aug27/Modified/cpp test corrections

    f_protocol_object->GetPqmPrMode()->SetViewDir(-1);


    UpdateSARControlInfoOnProtocolUpdate(f_protocol_object);

    int l_status = SetProtocolObject(f_vf_handle, f_protocol_object);

    if_error(l_status) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetProtocolObject ERROR."));
        DB_PRINTF("", ("pqmPrSetProtocolObject ERROR.\n"));
    }

    else {
        ///AfxMessageBox(L"Before push back !!!!");
        //m_protocol_vec.push_back(f_protocol_object);

    }

    return  f_protocol_object;
}

//****************************Method Header************************************
//Method Name   :CompareContrastAgent()
//Author        :PATNI/HAR
//Purpose       :Compare specified protocol's contrast agents.
//*****************************************************************************
BOOL CPQMStudy::CompareContrastAgent(
    CVarFieldHandle* const f_vf_handle,
    const int f_protocol1,
    const int f_protocol2
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::CompareContrastAgent");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CompareContrastAgent"));

    //+Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    bool_t  l_ret = FALSE;
    char l_buf1[128] = {0}, l_buf2[128] = {0};
    //-Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution

    BITFLD_DECL(mask, K_ALL_PR);

    memset(mask, 0, sizeof(mask));
    int l_size1 = sizeof(l_buf1);

    status_t l_status = GetProtocolValues(f_vf_handle,
                                          f_protocol1,
                                          mask,
                                          K_PR_CONTRAST_NAME,
                                          l_buf1,
                                          &l_size1,
                                          NULL);

    if (l_status != E_NO_ERROR || BITFLD_ON(mask, K_PR_CONTRAST_NAME)) {
        return  l_ret;
    }

    memset(mask, 0, sizeof(mask));
    int l_size2 = sizeof(l_buf2);

    l_status = GetProtocolValues(f_vf_handle,
                                 f_protocol2,
                                 mask,
                                 K_PR_CONTRAST_NAME,
                                 l_buf2,
                                 &l_size2,
                                 NULL);

    if (l_status != E_NO_ERROR || BITFLD_ON(mask, K_PR_CONTRAST_NAME)) {
        return  l_ret;
    }

    if (l_size2 != l_size1) {
        return  l_ret;
    }

    l_ret = !strcmp(l_buf1, l_buf2);
    return l_ret;
}

//****************************Method Header************************************
//Method Name   :CreateScratchPad
//Author        :PATNI/
//Purpose       :
//*****************************************************************************
int CPQMStudy::CreateScratchPad(
    const int f_protocol_index,
    const char* f_tmp_file,
    const SCRATCHPAD_CREATION_MODE f_sp_creation_mode
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::CreateScratchPad");

    if (!m_vfdb_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_vfdb_handle is NULL"));
        return E_ERROR;
    }

    m_vfdb_handle->ReOpen();
    //+Patni-SS/2009Apr01/PSP1#VFLOCK
    CString l_log_msg = _T("");

    //+Patni-RSG Modified and Added /2009Jun15/JFT-324
    status_t l_result = -1;
    int l_retry_cnt = 0;

    l_log_msg.Format(_T(" :VuiCopyProtocolToFile :"));

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_msg);
    //-Patni-SS/2009Apr01/PSP1#VFLOCK

    ClibVfToUiAdapter vuiAdapter(m_vfdb_handle);

    if (SP_CREATE_MODE_PRIVATE_PROLIM == f_sp_creation_mode) {
        l_result = vuiAdapter.VuiCopyProtocolToPrivateProlimScratchPad(f_protocol_index, f_tmp_file, 0, 0, NULL);

    } else
        l_result = vuiAdapter.VuiCopyProtocolToFile(f_protocol_index, f_tmp_file, 0, 0, NULL);

    if ((SP_CREATE_MODE_PRIVATE_PROLIM == f_sp_creation_mode) || (SP_CREATE_MODE_PRIVATE_PROLIM_MSD == f_sp_creation_mode)) {

        SaveTagsBeforeScratchpad(f_protocol_index);
    }


    if (l_result != E_NO_ERROR) {
        CPQMLogMgr::GetInstance()->DisplayScanError(
            IDS_ERR_COPY_TO_TMP_FILE, _T("IDS_ERR_COPY_TO_TMP_FILE"),
            SCAN_ERR_NOTIFY,
            L"PQMStudy.cpp");
        return l_result;
    }

    m_vfdb_handle->ReOpen();

    UpdateScratchPadForSARControl(f_tmp_file, f_sp_creation_mode);
    return l_result;
}


//***************************Method Header*************************************
//Method Name    :CopyScanMode()
//Author         :PATNI/
//Purpose        : Send +1 coz the CopyScanMode function assumes the index position from 1
//*****************************************************************************
void CPQMStudy::CopyScanMode(
    CVarFieldHandle* const f_vf_handle,
    const int f_to_pos,
    const int f_copyContrast
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::CopyScanMode");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    CPqmProtocol* l_obj = GetItemFromPosition(f_to_pos);

    if (l_obj == NULL || l_obj->GetProtocol() == PQ_FILLER) {
        return;
    }

    //+Patni-AMT/2009Nov24/Modified/DeFT/MVC005085-Code cleanup
    int             l_prot = -1;
    int             l_obj_trCoil = 0, l_trCoil = 0;
    CPqmPrMode      l_modeEntry;
    PqmSaturnCoil_t* l_saturn_entry     = {0};
    PqmSaturnRcvCoil_t* l_coilDB_tbl_entry = {0};
    CPqmCoilData*    l_coil_data = CDataManager::GetInstance()->GetPQMCoilData();
    int l_channel_mode = -1;

    bool_t          enable = false;
    //-Patni-AMT/2009Nov24/Modified/DeFT/MVC005085-Code cleanup

    BITFLD_DECL(mask, MAX_NODES);

    memset(mask, 0, sizeof(mask));
    status_t status = GetProtocolValues
                      (f_vf_handle, l_obj->GetProtocol(), mask,
                       K_PR_TR_COIL,  &l_obj_trCoil, NULL,
                       NULL);

    //    DB_PRINTF("l",("pos = %d\n",pos));
    //    DB_PRINTF("l",("rcvCoilId = 0x%x\n",obj->mode.rcvCoilId));
    //    DB_PRINTF("l",("trCoilId  = 0x%x\n",obj_trCoil));
    //    DB_PRINTF("l",("contrast  = %d\n",obj->mode.contrast));
    //    DB_PRINTF("l",("couchPos  = %g\n",obj->mode.couchPos));
    //    DB_PRINTF("l",("anatomy   = %d\n",obj->mode.anatomy));
    //    DB_PRINTF("l",("view_dir  = %d\n",obj->mode.view_dir));


    //Patni-AMT/2010Feb16/Modified/Ph-3# PROPOSAL_07
    CPqmProtocol* l_prev_obj = GetPrevItemFromPos(l_obj);
    CPqmProtocol* l_next_obj = GetNextItemFromPos(l_obj);
    CPqmProtocol*    l_Wait_obj = NULL;
    CPqmProtocol*    l_objEntry = NULL;

    CString tmpStr(_T(""));

    //    DB_DO_IF(prev_obj != NULL,"l",printf("Prev : %d\n",prev_obj->prot));
    //    DB_DO_IF(next_obj != NULL,"l",printf("Next : %d\n",next_obj->prot));
    if ((f_to_pos) == GetTopWaitPos(&l_Wait_obj)) { /* copy to head of wait. */
        if (l_next_obj != NULL) {

            l_modeEntry.Copy(l_next_obj->GetPqmPrMode());
            // + V9.25 coil support@AMT
            l_saturn_entry = l_next_obj->GetSaturnCoil();
            l_channel_mode = l_next_obj->GetcoilChannelMode();
            l_coilDB_tbl_entry =  l_next_obj->GetCoilDBTable();
            // - V9.25 coil support@AMT
            l_objEntry  = l_next_obj;
            l_prot = l_next_obj->GetProtocol();

        } else if (l_prev_obj != NULL) {


            l_modeEntry.Copy(l_prev_obj->GetPqmPrMode());

            // + V9.25 coil support@AMT
            l_saturn_entry = l_prev_obj->GetSaturnCoil();
            l_channel_mode = l_prev_obj->GetcoilChannelMode();
            l_coilDB_tbl_entry =  l_prev_obj->GetCoilDBTable();
            // - V9.25 coil support@AMT
            l_objEntry  = l_prev_obj;
            l_prot = l_prev_obj->GetProtocol();

        } else {

            /* no other wait protocol. */
            /* set default data. */
            int32_t     anatomy = 0;
            int32_t     view_dir = 0;
            status_t    status = E_NO_ERROR;

            //DB_PRINTF("l",("Nothing WAIT PROTOCOL.\n"));
            l_modeEntry.Copy(l_obj->GetPqmPrMode());
            // + V9.25 coil support@AMT
            l_saturn_entry = l_obj->GetSaturnCoil();
            l_channel_mode = l_obj->GetcoilChannelMode();
            l_coilDB_tbl_entry =  l_obj->GetCoilDBTable();
            // - V9.25 coil support@AMT
            l_objEntry  = l_obj;

            /** If Sequence has anatomy,don't change it. **/
            memset(mask, 0, sizeof(mask));
            status = GetProtocolValues
                     (f_vf_handle, l_obj->GetProtocol(), mask,
                      K_PR_ANATOMY,  &anatomy,  NULL,
                      K_PR_VIEW_DIR,    &view_dir,  NULL,
                      NULL);

            if (!BITFLD_ON(mask, K_PR_ANATOMY)) {
                l_modeEntry.SetAnatomy(anatomy);

                tmpStr.Format(_T("l_modeEntry.SetAnatomy(anatomy) = %d"), anatomy);
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, tmpStr);
                //Patni-MP/2009Mar17/Added/ACE-1/MCM0213-00131
                BodyPartGetVF(l_obj->GetProtocol(), l_modeEntry.GetBodyPart());

                //+Patni-Hemant/2010Apr27/Added/JaFT/IR-137
                int l_laterality = -1;
                GetLaterality(l_obj->GetProtocol(), l_laterality);
                l_modeEntry.SetLaterality(l_laterality);
                //-Patni-Hemant/2010Apr27/Added/JaFT/IR-137

            } else {
                l_modeEntry.SetAnatomy(0);
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_modeEntry.SetAnatomy = 0"));
                //Patni-MP/2009Mar17/Added/ACE-1/MCM0213-00131
                l_modeEntry.SetBodyPart(_T(""));

                //+Patni-Hemant/2010Apr27/Added/JaFT/IR-137
                l_modeEntry.SetLaterality(Laterality_Invalid);
                //-Patni-Hemant/2010Apr27/Added/JaFT/IR-137
            }

            /*** For V3.0 : 950213 ***/
            if (((status != E_NO_ERROR) && (anatomy != 0)) ||
                BITFLD_ON(mask, K_PR_VIEW_DIR) ||
                (view_dir != VFC_VIEW_FROM_HEAD &&
                 view_dir != VFC_VIEW_FROM_FEET))
                l_modeEntry.SetViewDir(VFC_VIEW_FROM_FEET);

            else
                l_modeEntry.SetViewDir(view_dir);
        }

    } else {

        if (l_prev_obj) {


            l_modeEntry.Copy(l_prev_obj->GetPqmPrMode());
            // + V9.25 Coil Support@AMT
            l_saturn_entry = l_prev_obj->GetSaturnCoil();
            l_channel_mode = l_prev_obj->GetcoilChannelMode();
            l_coilDB_tbl_entry =  l_prev_obj->GetCoilDBTable();
            // - V9.25 Coil Support@AMT

            l_objEntry  = l_prev_obj;
            l_prot = l_prev_obj->GetProtocol();

        } else {
            //+Patni-RUP/2009Sep10/Modified/cpp test corrections
            PQM_TRACE(USER_FUNC_MARKER,  FUNC_NAME,
                      _T("l_prev_obj pointer is NULL"));
            return;
        }

        //-Patni-RUP/2009Sep10/Modified/cpp test corrections
    }

    CCoil           l_coil;

    // + V9.25 Coil Support@AMT
    if (m_pqm_ptr->IsSaturnCoilSystem()) {

        PqmCoil_t* l_tmp_coil = NULL;

        if (!GetTxCoilInSaturn(l_objEntry->GetCoilDBTable(), &l_tmp_coil)) {
            l_tmp_coil = NULL;
        }

        l_coil.SetCoil(l_tmp_coil);
        enable = l_obj->GetCoilDBTable()->numCoil != 0;

    } else {

        l_coil_data->PqmCoilGetTxCoil(l_modeEntry.GetRecvCoilId(), l_coil);
        CPqmPrMode* l_pqm_mode = l_obj->GetPqmPrMode();
        enable = l_pqm_mode->GetRecvCoilId() != 0;
    }


    if (l_coil.GetCoil() != NULL) {
        l_trCoil = l_coil.GetCoilId();

    } else {
        l_trCoil = 0;
    }

    // - V9.25 Coil Support@AMT
    CPqmPrMode* l_mode = l_obj->GetPqmPrMode();

    /* 1997.06.10 by Nao */
    if (!f_copyContrast) {

        l_modeEntry.SetContrast(l_mode->GetContrast());
    }


    /* V7.00 */
    if (!m_pqm_ptr->GetAllCoilSAR()) {
        if (l_mode->GetAnatomy()) {
            l_modeEntry.SetAnatomy(l_mode->GetAnatomy()) ;
            tmpStr = _T("");
            tmpStr.Format(_T("l_modeEntry.SetAnatomy(l_mode->GetAnatomy()) = %d"), l_mode->GetAnatomy());
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, tmpStr);
            //Patni-MP/2009Mar17/Added/ACE-1/MCM0213-00131
            l_modeEntry.SetBodyPart(l_mode->GetBodyPart());

            //+Patni-Hemant/2010Apr27/Added/JaFT/IR-137
            l_modeEntry.SetLaterality(l_mode->GetLaterality());
            //-Patni-Hemant/2010Apr27/Added/JaFT/IR-137

        }

        if (enable) {
            l_modeEntry.SetRecvCoilId(l_mode->GetRecvCoilId());
            l_saturn_entry = l_obj->GetSaturnCoil();
            l_channel_mode = l_obj->GetcoilChannelMode();
            l_coilDB_tbl_entry  = l_obj->GetCoilDBTable() ;
            l_objEntry  = l_obj;
            l_trCoil = l_obj_trCoil;
        }
    }

    // else
    // {
    //   //if(modeEntry.anatomy == obj->mode.anatomy)
    //  //      strcpy(modeEntry.bodypart , obj->mode.bodypart);
    // }
    //------------------------------------------------------------------------------------
    // + V9.25 coil support@AMT
    //    __asm int 3;
    if (m_pqm_ptr->IsSaturnCoilSystem()) {  //Saturn Coil

        /**********************************************************************
         In case of Saturn System, Receiver coil id is different than rcvCoilId
          of mode. Compare these two coil ids at this point it might be same
         *********************************************************************/

        l_modeEntry.SetRecvCoilId(l_mode->GetRecvCoilId());
        //Patni-HAR/2009Nov24/Modifed/DeFT/MVC005085
        int l_return = memcmp(l_saturn_entry, l_obj->GetSaturnCoil(), sizeof(PqmSaturnCoil_t));
        enable = (l_modeEntry.Compare(l_mode)) ||
                 (l_trCoil != l_obj_trCoil) || l_return ;

    } else {

        CPqmPrMode* l_secMode = l_obj->GetPqmPrMode();

        enable = (l_modeEntry.Compare(l_mode)) || (l_trCoil != l_obj_trCoil);
    }

    // - V9.25 coil support@AMT

    if (enable) {
        //Setting the new values in the PQMPrMode
        CPqmPrMode* copyToMode = l_obj->GetPqmPrMode();
        copyToMode->SetAnatomy(l_modeEntry.GetAnatomy());
        tmpStr = _T("");
        tmpStr.Format(_T("copyToMode->SetAnatomy(l_modeEntry.GetAnatomy()) = %d"), l_modeEntry.GetAnatomy());
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, tmpStr);
        copyToMode->SetContrast(l_modeEntry.GetContrast());
        copyToMode->SetCouchOffset(l_modeEntry.GetCouchOffset());
        copyToMode->SetCouchPos(l_modeEntry.GetCouchPos());
        copyToMode->SetHomePos(l_modeEntry.GetHomePos());
        copyToMode->SetPatoriEndIn(l_modeEntry.GetPatoriEndIn());
        copyToMode->SetPatoriSideUp(l_modeEntry.GetPatoriSideUp());
        copyToMode->SetRecvCoilId(l_modeEntry.GetRecvCoilId());
        copyToMode->SetScanNumber(l_modeEntry.GetScanNumber());
        copyToMode->SetViewDir(l_modeEntry.GetViewDir());
        // V9.25 coil support@AMT
        l_obj->SetSaturnCoil(l_saturn_entry);
        l_obj->SetcoilChannelMode(l_channel_mode);
        l_obj->SetCoilDBTable(l_coilDB_tbl_entry) ;

        //Patni-MP/2009Mar17/Added/ACE-1/MCM0213-00131
        BodyPartSetVF(l_obj->GetProtocol(), l_modeEntry.GetBodyPart());

        //Patni-Hemant/2009SEPT10/Added/Defect found while fising MVC3979
        copyToMode->SetBodyPart(l_modeEntry.GetBodyPart());

        //+Patni-Hemant/2010Apr27/Added/JaFT/IR-137
        SaveLaterality(l_obj->GetProtocol(), l_modeEntry.GetLaterality());
        copyToMode->SetLaterality(l_modeEntry.GetLaterality());
        //-Patni-Hemant/2010Apr27/Added/JaFT/IR-137

        /*** For SAR --->> 950220 ***/
        //Added by Meghana On 9/23/2008 5:24:23 PM

        (l_obj->GetSarInfo())->scanAnatomy = l_modeEntry.GetAnatomy();

        if (m_pqm_ptr->IsSaturnCoilSystem()) {      //Saturn Coil

            PqmSaturnRcvCoil_t* l_coil_db = l_obj->GetCoilDBTable() ;

            for (int i = 0; i < l_coil_db->numCoil; i++) {

                (l_obj->GetSarInfo())->receiverCoil[i] =
                    l_coil_db->coiltbl_p[0]->coil_id;
            }

        } else {

            (l_obj->GetSarInfo())->receiverCoil[0] = l_mode->GetRecvCoilId();
        }

        l_obj->GetSarInfo()->transmiterCoil = l_trCoil;
        // V9.25 coil support@AMT

        /*************/
        //DB_PRINTF("l",("rcvCoilId = 0x%x\n",obj->mode.rcvCoilId));
        //DB_PRINTF("l",("trCoil    = 0x%x\n",trCoil));
        //DB_PRINTF("l",("contrast  = %d\n",obj->mode.contrast));
        //DB_PRINTF("l",("couchPos  = %g\n",obj->mode.couchPos));
        //DB_PRINTF("l",("anatomy   = %d\n",obj->mode.anatomy));
        //DB_PRINTF("l",("view_dir  = %d\n",obj->mode.view_dir));

        /* 1.COPYする内容をStudyに書き込む（Saturn以前） */
        memset(mask, 0, sizeof(mask));
        CPqmPrMode* l_ptr_pqmmode = l_obj->GetPqmPrMode();
        int l_rcvCoilId = l_ptr_pqmmode->GetRecvCoilId();
        int l_lanatomy = l_ptr_pqmmode->GetAnatomy();
        int l_scanNumber = l_ptr_pqmmode->GetScanNumber();
        float l_couch_Pos = l_ptr_pqmmode->GetCouchPos();
        float l_home_Pos = l_ptr_pqmmode->GetHomePos();
        float l_couch_offset = l_ptr_pqmmode->GetCouchOffset();
        int l_contrast = l_ptr_pqmmode->GetContrast();
        int l_view_dir = l_ptr_pqmmode->GetViewDir();

        VftPatori_t l_patori;
        l_patori.side_up = l_ptr_pqmmode->GetPatoriSideUp();
        l_patori.end_in = l_ptr_pqmmode->GetPatoriEndIn();
        //Meghana here......
        status = SetProtocolValues
                 (m_vfdb_handle, l_obj->GetProtocol(), mask,
                  K_PR_TR_COIL, &l_trCoil,      sizeof(int32_t),
                  K_PR_RC_COIL, &l_rcvCoilId,   sizeof(int32_t),
                  K_PR_ANATOMY, &l_lanatomy,    sizeof(int32_t),
                  K_PR_SCAN_NUMBER, &l_scanNumber,  sizeof(int32_t),
                  K_PR_COUCH,   &l_couch_Pos,   sizeof(flt32_t),
                  K_PR_HOME_POSITION, &l_home_Pos,  sizeof(flt32_t),
                  K_PR_COUCH_OFFSET,    &l_couch_offset,    sizeof(flt32_t),
                  K_PR_CONTRAST,    &l_contrast,    sizeof(int32_t),
                  K_PR_VIEW_DIR,    &l_view_dir,    sizeof(int32_t),
                  K_PR_PATIENT_ORIENT,  &(l_patori),    sizeof(VftPatori_t),
                  K_PR_MAX_COIL_CHANNEL_MODE, &(l_channel_mode), sizeof(l_channel_mode),
                  NULL);

        if (m_pqm_ptr->IsSaturnCoilSystem()) {       //Saturn Coil

            int l_prot = l_obj->GetProtocol();

            PutSaturnCoil(m_vfdb_handle, l_prot, l_obj->GetSaturnCoil());
            PutConnectedCoil(m_vfdb_handle, l_prot, l_obj);

            if (E_NO_ERROR != l_coil_data->PqmAllCoilGetSaturn(l_obj->GetSaturnCoil(),
                    l_obj->GetCoilDBTable())) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PqmAllCoilGetSaturn failed"));
            }

            PutRecvrCoil(m_vfdb_handle, l_prot, l_obj);
            SetProtValues(m_vfdb_handle, l_prot, mask,
                          K_PR_RC_COIL_NAME,
                          (l_obj->GetSaturnCoil())->CoilName,
                          strlen((l_obj->GetSaturnCoil())->CoilName) + 1,
                          NULL);
            SetTxCoilInSaturn(f_vf_handle, l_obj, l_obj->GetCoilDBTable()) ;
        }
    }

    if (l_prot > -1 && f_copyContrast) {
        char        buf[1024] = "";//Patni-MJC/2009Aug17/Modified/cpp test corrections
        int     size = 1024;//Patni-MJC/2009Aug17/Modified/cpp test corrections

        //size = 1024;      //Patni-MJC/2009Aug17/Modified/cpp test corrections
        memset(mask, 0, sizeof(mask));
        status = GetProtocolValues
                 (f_vf_handle, l_prot, mask,
                  K_PR_CONTRAST_NAME, buf, &size,
                  NULL);

        if ((status == E_NO_ERROR && !BITFLD_ON(mask, K_PR_CONTRAST_NAME))

            //Patni-Hemant/2010Jun18/Added/MaFT/MVC008201/Update
            || !(l_obj->GetPqmPrMode()->GetContrast())) {

            memset(mask, 0, sizeof(mask));
            SetProtocolValues
            (f_vf_handle, l_obj->GetProtocol(), mask,
             K_PR_CONTRAST_NAME, buf, size,
             NULL);

            //Patni-Hemant/2010Jun18/Added/MaFT/MVC008201/Update
            l_obj->SetContrastName(buf);
        }
    }

}

//****************************Method Header************************************
//Method Name   :CopyBackCouchInfoForlastDoneProtocol()
//Author        :PATNI/HAR-Sribanta
//Purpose       :
//*****************************************************************************
void CPQMStudy::CopyBackCouchInfoForlastDoneProtocol(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::CopyBackCouchInfoForlastDoneProtocol");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));
    CPqmProtocol* l_result_acq_series	= NULL;
    protocolvec::iterator v1_Iter	= NULL;
    CPqmProtocol* l_last_done_protocol = NULL;

    for (v1_Iter = m_protocol_vec.begin(); v1_Iter != m_protocol_vec.end(); v1_Iter++) {

        l_result_acq_series = *v1_Iter;

        if (NULL == l_result_acq_series) {
            continue;
        }

        if (C_PR_STATUS_WAIT == l_result_acq_series->GetAcqStatus()) {
            RecalculateSAR(l_result_acq_series);

            if (E_NO_ERROR != m_pqm_ptr->GetdBdt()->dBdtCalc(m_vfdb_handle, l_result_acq_series)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("dBdtCalc failed."));
            }

        } else if (C_PR_STATUS_DONE == l_result_acq_series->GetAcqStatus()) {
            m_has_image = TRUE;
            l_result_acq_series->SetHasImage(TRUE);
            l_last_done_protocol = l_result_acq_series;

        }
    }

    if (l_last_done_protocol) {
        m_pqm_ptr->GetAcqManager()->SetProtocolModeLastScan(l_last_done_protocol->GetPqmPrMode());

        if ((K_IMAGING == m_pqm_ptr->GetMode())) {
            int l_scan_number = l_last_done_protocol->GetPqmPrMode()->GetScanNumber();
            float l_couch_pos = l_last_done_protocol->GetPqmPrMode()->GetCouchPos();
            float l_home_pos = l_last_done_protocol->GetPqmPrMode()->GetHomePos();

            CPqmShimSharedData::GetInstance()->CopyBackCouchInfo(
                l_scan_number, l_couch_pos, l_home_pos);
        }
    }
}
//-Patni-Sribanta/2010Nov18/Added/REDMINE-772
//****************************Method Header************************************
//Method Name   :ReadPatientInfoFromVarField()
//Author        :PATNI/HAR-Sribanta
//Purpose       :
//*****************************************************************************
BOOL CPQMStudy::ReadPatientInfoFromVarField(
    const CString& f_study_name
)
{
    CString FUNC_TRACE(_T("CPQMStudy::ReadPatientInfoFromVarField"));
    PQM_TRACE(USER_FUNC_MARKER, FUNC_TRACE, f_study_name);

    if (m_pqm_ptr == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_TRACE,
                  _T("m_pqm_ptr pointer is NULL"));
        return FALSE;
    }

    m_weight  = 0.0f;
    m_height  = 0.0f;
    VfFieldSpec_t   l_vf_req[NUM_4] = {0};
    int             l_cnt           = 0;
    int             l_num           = 0;


    memset(l_vf_req, 0, sizeof(VfFieldSpec_t) * 4);

    VFF_SET_ARGS(l_vf_req[l_cnt], SVN_SCH_STUDY_ID, SVT_SCH_STUDY_ID,
                 sizeof(study_id_t), &m_study_id, 0L);

    l_cnt++;

    VFF_SET_ARGS(l_vf_req[l_cnt], SVN_SCH_WEIGHT, SVT_SCH_WEIGHT,
                 sizeof(float), &m_weight, 0L);

    l_cnt++;

    if (m_pqm_ptr->IsSaturnCoilSystem()) {

        VFF_SET_ARGS(l_vf_req[l_cnt], SVN_SCH_HEIGHT, SVT_SCH_HEIGHT,
                     sizeof(float), &m_height, 0L);

        l_cnt++;
    }

    VFF_SET_ARGS(l_vf_req[l_cnt], SVN_SCH_GENDER, SVT_SCH_GENDER,
                 sizeof(int), &m_gender , 0L);

    l_cnt++;

    DEL_PTR(m_vfdb_handle);

    m_vfdb_handle = new CVarFieldHandle(f_study_name);
    status_t        l_status        = E_ERROR;

    if (E_NO_ERROR != (l_status = m_vfdb_handle->Open())) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_TRACE,
                  CPqmUtilities::GetLibVfErrorStatusString(l_status));
        return FALSE;

    } else {
        if (E_NO_ERROR != (l_status = m_vfdb_handle->GetFields(NULL, SVD_ST_STUDY,
                                      l_vf_req, l_cnt, &l_num))) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_TRACE,
                      CPqmUtilities::GetLibVfErrorStatusString(l_status));
            return FALSE;
        }
    }

    return TRUE;
}
//-Patni-Sribanta/2010Nov18/Added/REDMINE-772

//+Patni-Sribanta/2010Oct25/Added/IR-170
//****************************Method Header************************************
//Method Name   :LoadPasSequences()
//Author        :PATNI/Sribanta
//Purpose       :
//*****************************************************************************
bool CPQMStudy::LoadPasSequences(
    const CString& f_pas_path
)const
{

    LPCTSTR FUNC_NAME = _T("CPQMStudy::LoadPasSequences");

    if (f_pas_path.IsEmpty()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Pas Path is Empty."));
        return false;
    }

    if (!CheckPasValidity(f_pas_path)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed to check PAS Validity."));

        //+Patni-Sudhir/2010Dec09/Added/IR-170
        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
            CPqmUtilities::GetMultiLingualString(_T("IDS_REQ_PAS_NOT_FOUND")),
            MPlus::EH::Information, _T("PQM"), _T("PQM"));
        //-Patni-Sudhir/2010Dec09/Added/IR-170

        return false;
    }

    if (!CheckPasLicense(f_pas_path)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed to check PAS License."));

        //+Patni-Sudhir/2010Dec09/Added/IR-170
        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
            CPqmUtilities::GetMultiLingualString(_T("IDS_REQ_PAS_NOT_FOUND")),
            MPlus::EH::Information, _T("PQM"), _T("PQM"));
        //-Patni-Sudhir/2010Dec09/Added/IR-170

        return false;
    }

    if (!LoadPasAndDisplay(f_pas_path)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed to Load PAS and Display."));

        //+Patni-Sribanta/2011Feb18/Added/IR-170_Part3
        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
            CPqmUtilities::GetMultiLingualString(_T("IDS_REQ_PAS_NOT_FOUND")),
            MPlus::EH::Information,
            _T("PQM"), _T("PQM"));
        //-Patni-Sribanta/2011Feb18/Added/IR-170_Part3

        return false;
    }

    return true;
}

//****************************Method Header************************************
//Method Name   :CheckPASinPASDB()
//Author        :PATNI/Sribanta
//Purpose       :This function reads the file from PAS path and check whether \
//				 this file is present or not.
//*****************************************************************************
bool CPQMStudy::CheckPasValidity(
    const CString& f_pas_path
) const
{


    //+Patni-Sudhir/2010Dec09/Modified/IR-170
    //return (_taccess(f_pas_path,00)==0);
    if (0 == _taccess(f_pas_path, 00)) {
        return true;

    } else {
        return false;
    }

    //-Patni-Sudhir/2010Dec09/Modified/IR-170

}

//****************************Method Header************************************
//Method Name   :CheckPasLicense()
//Author        :PATNI/Sribanta
//Purpose       :This function will check the License for PAS.This function takes\
//				 PAS path as argument.
//*****************************************************************************
bool CPQMStudy::CheckPasLicense(
    CString f_pas_path
) const
{

    LPCTSTR FUNC_NAME = _T("CPQMStudy::CheckPasLicense");
    //Patni-Sribanta/2011Mar28/Added/IR-170_Part3
    f_pas_path.Replace(_T("/"), _T("\\"));

    //+Patni-Sribanta/2011Feb17/Modified/IR-170_Part3
    CPASCOMInterface* l_passcominterface = m_pqm_ptr->GetPascomInterface();

    if (!(l_passcominterface->IsValidLicenseInLabelFile(f_pas_path))) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("IsValidLicenseInLabelFile() Failed."));
        return false ;
    }

    CString l_path = (CString)GP_PAS_PATH;

    if (-1 == f_pas_path.Find(l_path)) {
        return true ;
    }

    if (!GetLicenseStringForPath(f_pas_path)) {
        return false ;
    }

    //-Patni-Sribanta/2011Feb17/Modified/IR-170_Part3

    return l_passcominterface->CheckLicense(f_pas_path);
}

//****************************Method Header************************************
//Method Name   :LoadPasAndDisplay()
//Author        :PATNI/Sribanta
//Purpose       :This function will take pas path and acq order as argument and \
//				 display the corresponding PAS in PQM.
//*****************************************************************************
bool CPQMStudy::LoadPasAndDisplay(
    const CString& f_pas_path
)const
{

    LPCTSTR FUNC_NAME = _T("CPQMStudy::LoadPasAndDisplay");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Entering LoadPaSandDisplay"));

    CVarFieldHandle l_pasdb_handle(f_pas_path);

    if (l_pasdb_handle.VuiOpen() != E_NO_ERROR) {

        const CString log_str = _T("Could Not Load PAS For: ") + f_pas_path ;
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, log_str);

        return false;
    }

    ClibVfToUiAdapter vuiAdapter(&l_pasdb_handle);
    VuiAcqOrder_t l_acq_order;

    if (E_NO_ERROR != vuiAdapter.VuiGetVfAcqOrder(&l_acq_order)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("VuiGetVfAcqOrder() failed."));
        return false;
    }

    if (l_acq_order.SzAcqOrder < 1) {

        const CString log_str = _T("No Sequence Present in PAS : ") + f_pas_path;
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, log_str);

        return false;
    }

    //+Patni-Sribanta/2010Dec06/Added/IR-170_Reopen
    m_pqm_ptr->GetPascomInterface()->GetLicensedAcqOrder(l_acq_order, f_pas_path);


    if (0 == l_acq_order.SzAcqOrder) {
        const CString log_str = _T("Invalid License for all Sequences in PAS: ") + f_pas_path;
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, log_str);

        //+Patni-Sribanta/2011Feb18/Commented/IR-170_Part3
        ////+Patni-Sudhir/2010Dec10/Added/IR-170
        //CString l_error_string =  CPqmUtilities::GetMultiLingualString(_T("IDS_REQ_PAS_NOT_FOUND"));
        //CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(l_error_string, MPlus::EH::Information, _T("PQM"),_T("PQM"));
        ////-Patni-Sudhir/2010Dec10/Added/IR-170
        //-Patni-Sribanta/2011Feb18/Commented/IR-170_Part3


        return false;
    }

    //-Patni-Sribanta/2010Dec06/Added/IR-170_Reopen

    //IR-170_Impact_Scenario
    if (!m_pqm_ptr->ShowSequenceOnGUI(l_acq_order.VuiAcqOrder, f_pas_path, l_acq_order.SzAcqOrder, l_acq_order.SzAcqOrder)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ShowSequenceOnGUI() failed"));
        return false;
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exiting LoadPaSandDisplay"));

    return true;
}
//-Patni-Sribanta/2010Oct25/Added/IR-170

//+Patni-Sribanta-HAR/2010Sep21/Modified/TMSC-REDMINE-756
//****************************Method Header************************************
//Method Name   :CheckProtocolsInDB()
//Author        :PATNI/HAR-Sribanta
//Purpose       :This function checks number of protocols in Varfield study file
//               and local db. Protocols in study file and Local DB are same
//               function returns TRUE. Protocols in study file and Local DB are
//               not same function returns FALSE
//*****************************************************************************
BOOL CPQMStudy::CheckProtocolsInDB(int& f_varfield_protocol_count)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::CheckProtocolsInDB");

    VuiAcqOrder_t    l_vf_acq_order;
    //Get AcqOrder from VF Study file
    memset(&l_vf_acq_order, NULL, sizeof(VuiAcqOrder_t));

    ClibVfToUiAdapter vuiAdapter(m_vfdb_handle);

    if (E_NO_ERROR != vuiAdapter.VuiGetVfAcqOrder(&l_vf_acq_order)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("VuiGetVfAcqOrder() failed."));
        return FALSE;
    }

    f_varfield_protocol_count = l_vf_acq_order.SzAcqOrder;

    const int l_series_count_local_db = m_pqm_ptr->GetStudyManagerPtr()->GetSeriesCountInLocalDB();

    CString l_err_msg(_T(""));
    l_err_msg.Format(_T("Acqorder Size in Varfield = %d ,Local DB = %d"),
                     f_varfield_protocol_count, l_series_count_local_db);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME , l_err_msg);

    if (l_series_count_local_db != f_varfield_protocol_count) {
        return FALSE;

    } else {
        return TRUE;
    }
}
//****************************Method Header************************************
//Method Name   :DisplayProtocolMismatchMessage()
//Author        :PATNI/HAR-Sribanta
//Purpose       :Function will display error message in error view in case passing
//               parameter is TRUE. After displaying error message function returns
//               FALSE.
//*****************************************************************************
BOOL CPQMStudy::DisplayProtocolMismatchMessage(const BOOL f_message_display_flag)const
{

    LPCTSTR FUNC_NAME = _T("CPQMStudy::DisplayProtocolMismatchMessage");

    if (f_message_display_flag) {

        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
            CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_PROTOCOL_LOCALDB")),
            MPlus::EH::Warning, _T("PQM"), _T("PQM"));

        return FALSE;

    } else {
        CString log_str = _T("[Message Display Flag : ");
        log_str = log_str + (f_message_display_flag ? _T("TRUE]") : _T("FALSE]"));
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, log_str);
        return TRUE;
    }
}
//****************************Method Header************************************
//Method Name   :FillObjectParam()
//Author        :PATNI/HAR-Sribanta
//Purpose       :Function fills data in PASParam object
//*****************************************************************************
void CPQMStudy::FillObjectParam(
    int& f_protocol_index,
    CPqmProtocol& f_pqm_protocol,
    CPASParamStore& f_pasparam_object
)
{
    CString strCnv(_T("")); //Tanoue/MVC5849
    //+Tanoue/MVC5849
    UTIL::Mbs2Wcs(&strCnv, f_pqm_protocol.GetScanID());

    f_pasparam_object.SetString(_T("COMM"), strCnv);

    UTIL::Mbs2Wcs(&strCnv, f_pqm_protocol.GetScanComment());
    f_pasparam_object.SetString(_T("MEMO"), strCnv);
    //-Tanoue/MVC5849

    //+Patni-MP/2009Jun07/Added/ACE-2/Skip Prescan
    //For Skip Prescan
    CString l_scan_agent(_T(""));

    GetScanAgent(f_pqm_protocol.GetProtocol(), l_scan_agent);
    f_pasparam_object.SetString(_T("SKIP_PRESCAN"), l_scan_agent);
    //-Patni-MP/2009Jun07/Added/ACE-2/Skip Prescan
    f_pasparam_object.SetFloat(_T("TRSC"),
                               f_pqm_protocol.GetPqmPrMode()->GetCouchPos());

    f_pasparam_object.SetFloat(_T("ACQSTATUS2"),
                               (float)f_pqm_protocol.GetAcqStatus());

    //+Patni-NP/2009Sep15/Added/MVC004766
    //+Patni-HAR/2010May18/Modified/MVC007434
    f_pasparam_object.SetString(_T("DBDT"),
                                GetDBDTValueFromStudy(f_pqm_protocol.GetProtocol()));
    //+Patni-NP/2009Sep15/Added/MVC004766
    CString l_str_whole_sar(_T(""));
    CString l_str_partial_sar(_T(""));
    CString l_str_head_sar(_T(""));

    //-20091006 Tanoue-san SAR Enum
    if (SARCTRL_LICENCE_ON != m_pqm_ptr->GetSARControlLicense()) {
        l_str_whole_sar.Format(_T("%.3f W/kg"), f_pqm_protocol.GetSarInfo()->tsar[sarTypeWholeBody].sar);
        l_str_partial_sar.Format(_T("%.3f W/kg"), f_pqm_protocol.GetSarInfo()->tsar[sarTypePartial].sar);
        l_str_head_sar.Format(_T("%.3f W/kg"), f_pqm_protocol.GetSarInfo()->tsar[sarTypeHead].sar);

    }	else {

        l_str_whole_sar.Format(_T("%.3f W/kg"), f_pqm_protocol.GetSarInfo()->sarControl.tsarControl[sarTypeWholeBody].sar_6min);
        l_str_partial_sar.Format(_T("%.3f W/kg"), f_pqm_protocol.GetSarInfo()->sarControl.tsarControl[sarTypePartial].sar_6min);
        l_str_head_sar.Format(_T("%.3f W/kg"), f_pqm_protocol.GetSarInfo()->sarControl.tsarControl[sarTypeHead].sar_6min);
    }

    //-20091006 Tanoue-san SAR Enum
    f_pasparam_object.SetString(_T("SAR"),
                                l_str_whole_sar);
    f_pasparam_object.SetString(_T("PARTIALBODYSAR"),
                                l_str_partial_sar);
    f_pasparam_object.SetString(_T("HEADSAR"),
                                l_str_head_sar);
    //- Patni-PJS/2009Jul23/Modify/IDS#442,MVC002699

    CString l_str_b1_rms = _T("----------");

    if (f_pqm_protocol.GetSarInfo()->B1rms >= PQM_MIN_B1RMS) {
        l_str_b1_rms.Format(_T("%.2f uT"), f_pqm_protocol.GetSarInfo()->B1rms);
    }

    f_pasparam_object.SetString(_T("B1RMS"),
                                l_str_b1_rms);

    f_pasparam_object.SetFloat(_T("SCANTIME"),
                               (float)f_pqm_protocol.GetScanTime());

    f_pasparam_object.SetFloat(_T("IMGTECH"),
                               (float)f_pqm_protocol.GetAppCode());

    GetPasParamObject(f_pasparam_object, f_protocol_index);

    f_pasparam_object.SetFloat(_T("HOLD"),
                               (float)f_pqm_protocol.GetHoldScan());

    f_pasparam_object.SetFloat(_T("ACQSTATUS"),
                               (float)f_pqm_protocol.GetAcqStatus());

    int l_anatomy = f_pqm_protocol.GetPqmPrMode()->GetAnatomy();
    f_pasparam_object.SetFloat(_T("SA"),
                               (float)l_anatomy);
    //+Patni-MP/2009Jun07/Added/ACE-1/Body Part

    CString l_bodyPart = f_pqm_protocol.GetPqmPrMode()->GetBodyPart();
    f_pasparam_object.SetString(_T("BODYPART"),
                                l_bodyPart);
    //+Patni-MP/2009Jun07/Added/ACE-1/Body Part

    //+Patni-Hemant/2010Apr27/Added/JaFT/IR-137
    f_pasparam_object.SetInt(_T("LATERALITY"),
                             (int)f_pqm_protocol.GetPqmPrMode()->GetLaterality());
    //-Patni-Hemant/2010Apr27/Added/JaFT/IR-137

    f_pasparam_object.SetInt(_T("AUTOVOICE"),
                             f_pqm_protocol.GetExeAutoVoice());

    f_pasparam_object.SetFloat(_T("COUCH"),
                               f_pqm_protocol.GetPqmPrMode()->GetCouchPos());

    f_pasparam_object.SetInt(_T("ACQORDER"),
                             f_pqm_protocol.GetProtocol());

    CString l_str_pori(_T(""));
    l_str_pori.Format(_T("%d,%d"), f_pqm_protocol.GetPqmPrMode()->GetPatoriEndIn(),
                      f_pqm_protocol.GetPqmPrMode()->GetPatoriSideUp());
    f_pasparam_object.SetString(_T("PORI"), l_str_pori);

    CString l_str_scanoffset(_T(""));
    f32vec3_t l_offset = f_pqm_protocol.GetOffset();

    l_str_scanoffset.Format(_T("%f,%f,%f"), l_offset.x * 100, l_offset.y * 100, l_offset.z * 100);
    f_pasparam_object.SetString(_T("SCANOFFSET"), l_str_scanoffset);

    f_pasparam_object.SetFloat(_T("VDIR"), (float)f_pqm_protocol.
                               GetPqmPrMode()->GetViewDir());

    //+Tanoue/MVC005849
    UTIL::Mbs2Wcs(&strCnv, f_pqm_protocol.GetContrastName());
    f_pasparam_object.SetString(_T("CONTRASTNAME"), strCnv);
    //-Tanoue/MVC005849


    f_pasparam_object.SetInt(_T("APOST"), f_pqm_protocol.GetAutoTransfer());

    //+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
    f_pasparam_object.SetInt(_T("AS-COMPASS"), f_pqm_protocol.GetASCOMPASSFlag());
    f_pasparam_object.SetInt(_T("AMB-FLAG"), f_pqm_protocol.GetAMBEnableFlag());
    f_pasparam_object.SetInt(_T("AMB-MODE-FLAG"), f_pqm_protocol.GetAMBModeFlag());
    //-Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
    //Patni-HAR/2010Apr26/Added/IR-134
    f_pasparam_object.SetInt(_T("EDDY"), f_pqm_protocol.GetMRSEddyAxis());

    //Patni-RUP/2010Jul27/Added/TMSC-REDMINE-466
    f_pasparam_object.SetInt(_T("COUCHMETHOD"), f_pqm_protocol.GetcouchMethodFlg());

    f_pasparam_object.SetString(_T("DELAYTIME"), f_pqm_protocol.GetDelayTime());
    f_pasparam_object.SetInt(_T("GROUPID"), f_pqm_protocol.GetGroupID());
    f_pasparam_object.SetInt(_T("MASTERFLAG"), f_pqm_protocol.GetMasterFlag());
    f_pasparam_object.SetInt(_T("MSDENABLEFLAG"), f_pqm_protocol.GetMultiSeriesDynamicFlag());

    f_pasparam_object.SetInt(_T("INSERTMAP"), f_pqm_protocol.GetMapRequired());
    f_pasparam_object.SetInt(_T("INSERTSHIM"), f_pqm_protocol.GetRequireShimming());

}
//-Patni-Sribanta-HAR/2010Sep21/Modified/TMSC-REDMINE-756

//+Patni-Sribanta/2010July21/Added/TMSC-REDMINE-452
//***************************Method Header*************************************
//Method Name    :OnCreateVFStudyFile()
//Author         :PATNI/ Sribanta
//Purpose        :
//*****************************************************************************
bool CPQMStudy::OnCreateVFStudyFile(CString f_file_name)
{

    LPCTSTR FUNC_NAME = _T("CPQMStudy::OnCreateVFStudyFile");
    f_file_name.Delete(0, 3);
    const int l_dot_position = f_file_name.Find(_T("."));

    if (-1 == l_dot_position) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("l_dot_position is -1"));
        return false;
    }

    const CString l_study_id = f_file_name.Left(l_dot_position);

    if (l_study_id.IsEmpty()) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_study_id is empty"));
        return false;
    }

    if (TRUE == m_pqm_ptr->GetStudyManagerPtr()->WriteStudyIDInlocalDB(l_study_id)) {
        m_pqm_ptr->WriteToUI(PQM_MSG_GL_EXAMUPDATE);
    }

    return true;
}
//-Patni-Sribanta/2010July21/Added/TMSC-REDMINE-452


//+Patni-PJS/2010May25/Modifed+Added/TMSC-REDMINE-234
//*****************************************************************************
//Method Name   : IsValidCopySrcProtNum
//Author        : Patni/PJS
//Purpose       : This function is used to validate copy source protocol number
//*****************************************************************************
status_t CPQMStudy::IsValidCopySrcProtNum(
    const int f_prot_num
)const
{
    int                 n  = 0, numDone = 0;
    int32_t				l_cpy_src_prot = 0;
    VfPathElem_t        path[1] = {0};
    VfFieldSpec_t		vfReq[1] = {0};
    memset(path, 0, sizeof(VfPathElem_t));
    path[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    path[SVD_ST_PROTOCOL - 1].index = f_prot_num;
    memset(vfReq, 0, sizeof(VfFieldSpec_t));

    n = 0;
    VFF_SET_ARGS(vfReq[n], SVN_PLN_COPY_SOURCE_PROTOCOL_NO, SVT_PLN_COPY_SOURCE_PROTOCOL_NO,
                 sizeof(l_cpy_src_prot), &l_cpy_src_prot, VFO_ALLOCATE | VFO_VARIABLE_LEN);
    n++;

    status_t status = m_vfdb_handle->GetFields(path, SVD_ST_PROTOCOL, vfReq, n, & numDone);

    if (status == E_NO_ERROR && n == numDone && l_cpy_src_prot >= 0) {
        status = E_NO_ERROR ;

    } else {
        status = E_ERROR;
    }

    return(status) ;
}
//-Patni-PJS/2010May25/Modifed+Added/TMSC-REDMINE-234

//+Patni-PJS/2010Mar24/Added/IR-140
//**************************************************************************
//Method Name   :  PopulateGUIControls
//Author        :  PATNI/PJS
//Purpose       :  This function populates the structure updateCtrls from
//                 varfield for the tags whose scratchpad values are to be
//                 overwritten. This function should be called before tag
//                 values are from scratch pad are overwritten in varfield
//**************************************************************************
status_t CPQMStudy::PopulateGUIControls(
    const int f_acq_order
)
{
    LPCTSTR       FUNC_NAME = _T("CPQMStudy::PopulateGUIControls");

    VfFieldSpec_t*      l_vfreq = new VfFieldSpec_t[g_update_controls_size];//Patni-Rajendra/2011Mar10/modified/TMSC-Review-Comments
    VfPathElem_t        l_pelm[1] = {{SVN_ST_PROTOCOL, 0}};

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of PopulateGUIControls"));
    //Initialize VfFieldSpec_t array

    memset(l_vfreq, 0, sizeof(VfFieldSpec_t) * g_update_controls_size);	//Patni-Rajendra/2011Mar10/modified/TMSC-Review-Comments
    //set the protocol index.
    l_pelm[0].index = f_acq_order;


    for (int l_num = 0; l_num < g_update_controls_size; l_num++) {	//Patni-Rajendra/2011Mar10/modified//TMSC-Review-Comments
        VFF_SET_ARGS(l_vfreq[l_num], updateCtrls[l_num].name, updateCtrls[l_num].type, 0,
                     NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);
    }

    int l_numdone = 0;
    status_t l_status = m_vfdb_handle->GetFields(l_pelm, SVD_ST_PROTOCOL, l_vfreq, l_num, &l_numdone);

    //set the field value and size of the tags in the gui control structure

    for (l_num = 0; l_num < g_update_controls_size; l_num++) {//Patni-Rajendra/2011Mar10/modified/TMSC-Review-Comments

        if (l_vfreq[l_num].status == VFE_NO_ERROR) {
            updateCtrls[l_num].ctrl_value = l_vfreq[l_num].buffer;
            updateCtrls[l_num].ctrl_size = l_vfreq[l_num].size;
        }

        //+Patni-Hemant/2010Jun1/Added/MaFT/TMSC-REDMINE-8035
        else {
            updateCtrls[l_num].ctrl_value = 0;
            updateCtrls[l_num].ctrl_size = 0;
        }

        //-Patni-Hemant/2010Jun1/Added/MaFT/TMSC-REDMINE-8035
    }

    DEL_PTR_ARY(l_vfreq);

    return l_status;
}

//*******************************************************************************
//Method Name   :  RestoreGUICtrlValInVarfield
//Author        :  PATNI/PJS
//Purpose       :  This function is used to restore the values of the gui control
//                 structure to varfield provided the structure is populated.
//*******************************************************************************
status_t CPQMStudy::RestoreGUICtrlValInVarfield(
    const int f_acq_order, const bool f_is_plan_dynamic_executed /*=false*/
)
{
    LPCTSTR		FUNC_NAME = _T("CPQMStudy::RestoreGUICtrlValInVarfield");

    int             l_num = 0;
    int             l_count = 0;

    VfFieldSpec_t*  l_vfreq = new VfFieldSpec_t[g_update_controls_size];//Patni-Rajendra/2011Mar10/modified/TMSC-Review-Comments
    VfPathElem_t    l_pelm[1] = {{SVN_ST_PROTOCOL, 0}};

    memset(l_vfreq, 0, sizeof(VfFieldSpec_t) * g_update_controls_size);//Patni-Rajendra/2011Mar10/modified/TMSC-Review-Comments
    l_pelm[0].index = f_acq_order;
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("Trace of RestoreGUICtrlValInVarfield"));

    //Patni-Hemant/2010Oct06/Added/MVC009119
    const bool f_copy_src = (E_NO_ERROR == IsValidCopySrcProtNum(f_acq_order)) ;


    //Initializing VfFieldSpec_t array to be filled with values from updateCtrls struct
    for (l_count = l_num = 0; l_count < g_update_controls_size; l_count++) {//Patni-Rajendra/2011Mar10/modified/TMSC-Review-Comments
        if ((updateCtrls[l_count].ctrl_value != NULL) && (updateCtrls[l_count].ctrl_size != 0)) {
            //+Patni-PJS/2010May25/Modifed+Added/TMSC-REDMINE-234

            //+Patni-Hemant/2010Oct06/Modifed/MVC009119
            //if ((updateCtrls[l_count].name ==  SVN_PLN_SCAN_ANATOMY) && (E_NO_ERROR == IsValidCopySrcProtNum(f_acq_order))) {
            //    continue;
            //}

            if ((
                    (updateCtrls[l_count].name ==  SVN_PLN_SCAN_ANATOMY) ||
                    (updateCtrls[l_count].name ==  SVN_PLN_BODY_PART)    ||
                    (updateCtrls[l_count].name ==  SVN_PLN_LATERALITY)   ||
                    (updateCtrls[l_count].name ==  SVN_PLN_MAX_COIL_CHANNEL_MODE)) &&
                (f_copy_src || f_is_plan_dynamic_executed)) {

                continue ;

            }

            //-Patni-Hemant/2010Oct06/Modifed/MVC009119

            //-Patni-PJS/2010May25/Modifed+Added/TMSC-REDMINE-234
            //Patni-PJS/2010Apr16/Added/JaFT# IR-140 Internal defect fix
            VFF_SET_ARGS(l_vfreq[l_num], updateCtrls[l_count].name,
                         updateCtrls[l_count].type, updateCtrls[l_count].ctrl_size,
                         updateCtrls[l_count].ctrl_value, VFO_REPLACE);
            l_num++;

        }

        //+Patni-Hemant/2010Oct06/Added/MVC009119
        else {

            if (updateCtrls[l_count].name ==  SVN_PLN_LATERALITY) {
                SaveLaterality(f_acq_order, -1);
            }
        }

        //-Patni-Hemant/2010Oct06/Added/MVC009119
    }

    status_t l_status = E_NO_ERROR;

    //Restoring VfFieldSpec_t value in the varfield
    if (l_num) {
        int l_numDone = 0;
        time_t ts = 0L;
        l_status = m_vfdb_handle->PutFields(l_pelm,
                                            SVD_ST_PROTOCOL, l_vfreq, l_num, (int*) & l_numDone, &ts);
    }

    DEL_PTR_ARY(l_vfreq);

    return l_status;
}
//-Patni-PJS/2010Mar24/Added/IR-140

//+Patni-ARD/2010Mar10/Added/Ph-3# IR-100 code review
//********************************Method Header********************************
//Method Name   : PrepPlanInit()
//Author        : PATNI/PJS
//Purpose       : Initializes prep study
//*****************************************************************************
void CPQMStudy::PrepPlanInit()const
{
    char            l_file[512] = {0};
    //+Patni-ARD/2010Mar3/Modified/DeFT# IR-100 remove IPCConfig.ini
    CPqmUtilities::GetPrepStudyName(l_file);

    //GetPrepStudyName(l_file);
    //-Patni-ARD/2010Mar3/Modified/DeFT# IR-100 remove IPCConfig.ini

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections

    if (m_pqm_ptr) {
        CPqmProcon* pqm_procon = m_pqm_ptr->GetPqmProCon();

        if ((NULL != pqm_procon) && pqm_procon->GetPQMProbe()) {
            pqm_procon->GetPQMProbe()->StudyPlanInit(l_file, 0);
        }
    }
}
//- Patni-PJS/2009Aug14/Modify/MVC002490

//+ Patni-MSN/2009Dec16/Modify/RMC Positioning
//********************************Method Header********************************
//Method Name   : ProbePlanInit()
//Author        : PATNI/MSN
//Purpose       : Initializes probe study
//*****************************************************************************
void CPQMStudy::ProbePlanInit()const
{
    char        l_file[512] = {0};

    CPqmUtilities::GetProbeStudyName(l_file);


    if (m_pqm_ptr) {
        CPqmProcon* pqm_procon = m_pqm_ptr->GetPqmProCon();


        if ((NULL != pqm_procon) && pqm_procon->GetPQMProbe()) {
            pqm_procon->GetPQMProbe()->StudyPlanInit(l_file, 0);
        }
    }
}
//-Patni-ARD/2010Mar10/Added/Ph-3# IR-100 code review

//+Patni-RSG/Added/2009Nov25/MSA0248-0033
//****************************Method Header************************************
//Method Name   :InitializeVFPath()
//Author        :PATNI/RSG
//Purpose       :Initialize VfPath_Element up to depth SVD_ST_IMAGE
//*****************************************************************************
void CPQMStudy::InitializeVFPath(
    VfPathElem_t* f_vf_path
)const
{
    if (!f_vf_path) {
        return;
    }

    f_vf_path[0].subtree_name = SVN_ST_PROTOCOL;
    f_vf_path[0].index = -1;

    f_vf_path[1].subtree_name = SVN_ST_PROCESSED;
    f_vf_path[1].index = -1;

    f_vf_path[2].subtree_name = SVN_ST_ECHO;
    f_vf_path[2].index = -1;

    f_vf_path[3].subtree_name = SVN_ST_IMAGE;
    f_vf_path[3].index = -1;

}
//-Patni-RSG/Added/2009Nov25/MSA0248-0033

//+Patni+MP/2009Jun07/Added/ACE-2/Missing Vantage Auto Post Proc
//****************************Method Header************************************
//Method Name   :CheckPostProcessing()
//Author        :PATNI / MRP
//Purpose       :For checking Auto Post Processing tags and values
//*****************************************************************************
bool CPQMStudy::CheckPostProcessing(const int f_protocol_index)
{

    LPCTSTR FUNC_NAME = _T("CPQMStudy::CheckPostProcessing");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Inside CheckPostProcessing"));

    if (f_protocol_index < 0) {

        return false;
    }

    VfPathElem_t        l_nodePath[SVD_ST_SLICE] = {
        {SVN_ST_PROTOCOL, 0},
        {SVN_ST_SUBPROTOCOL, 0},
        {SVN_ST_SLICEGROUP, 0},
        {SVN_ST_SLICE, 0}
    };
    VfFieldSpec_t       l_vfReq[16] = {0};

    int n = 0;
    l_nodePath[SVD_ST_PROTOCOL - 1].index = f_protocol_index;

    n = 0;
    int l_Auto_Diffusion_adc = 0;
    VFF_SET_ARGS(l_vfReq[n], SVN_PRC_AUTO_DIFFUSION_ADC,
                 SVT_PRC_AUTO_DIFFUSION_ADC, sizeof(int),
                 &l_Auto_Diffusion_adc, 0);
    n++;
    int l_Auto_Diffusion_iso = 0;
    VFF_SET_ARGS(l_vfReq[n], SVN_PRC_AUTO_DIFFUSION_ISOTROPIC,
                 SVT_PRC_AUTO_DIFFUSION_ISOTROPIC, sizeof(int),
                 &l_Auto_Diffusion_iso, 0);
    n++;
    int l_Auto_Perfusion = 0;
    VFF_SET_ARGS(l_vfReq[n], SVN_PRC_AUTO_PERFUSION,
                 SVT_PRC_AUTO_PERFUSION, sizeof(int),
                 &l_Auto_Perfusion, 0);
    n++;
    int l_Auto_Perfusion_map = 0;
    VFF_SET_ARGS(l_vfReq[n], SVN_PRC_AUTO_PERFUSION_MAPPING,
                 SVT_PRC_AUTO_PERFUSION_MAPPING, sizeof(int),
                 &l_Auto_Perfusion_map, 0);
    n++;
    int l_Auto_Dynamic_sub = 0;
    VFF_SET_ARGS(l_vfReq[n], SVN_PRC_AUTO_DYNAMIC_SUBTRACTION,
                 SVT_PRC_AUTO_DYNAMIC_SUBTRACTION, sizeof(int),
                 &l_Auto_Dynamic_sub, 0);
    n++;
    int l_Auto_MIP_preview[3] = {0};
    VFF_SET_ARGS(l_vfReq[n], SVN_PRC_AUTO_MIP_PREVIEW,
                 SVT_PRC_AUTO_MIP_PREVIEW, sizeof(l_Auto_MIP_preview),
                 l_Auto_MIP_preview, 0);
    n++;
    int l_Auto_MovingBed_sub = 0;
    VFF_SET_ARGS(l_vfReq[n], SVN_PLN_AUTO_MOVINGBED_SUBTRACTION,
                 SVT_PLN_AUTO_MOVINGBED_SUBTRACTION, sizeof(int),
                 &l_Auto_MovingBed_sub, 0);
    n++;
    int l_Auto_Dynamic_sub_image = 0;
    VFF_SET_ARGS(l_vfReq[n], SVN_PRC_AUTO_DYNAMIC_SUBTRACTION_IMAGES,
                 SVT_PRC_AUTO_DYNAMIC_SUBTRACTION_IMAGES, sizeof(int),
                 &l_Auto_Dynamic_sub_image, 0);
    n++;
    int l_Auto_Geometric = 0;
    VFF_SET_ARGS(l_vfReq[n], SVN_PRC_AUTO_GDC_ACTION,		/* V6.10　磁場歪処理 */
                 SVT_PRC_AUTO_GDC_ACTION, sizeof(int),
                 &l_Auto_Geometric, 0);
    n++;
    int l_Auto_AMB_sub = 0;
    VFF_SET_ARGS(l_vfReq[n], SVN_PLN_AMB_SUBTRACTION,		/* V7.10　AMB自動差分 */
                 SVT_PLN_AMB_SUBTRACTION, sizeof(int),
                 &l_Auto_AMB_sub, 0);
    n++;
    int l_num = 0;
    status_t l_vfStatus = m_vfdb_handle->GetFields(l_nodePath, SVD_ST_PROTOCOL, l_vfReq, n, &l_num);

    if_error(l_vfStatus) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("VfAppGetFields error."));
    }
    //Write trace logs here
    CString l_trace_string = _T("");
    l_trace_string.Format(_T("true = %d, false = %d"), TRUE, FALSE);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_string);

    l_trace_string.Format(_T("Auto_Diffusion_adc = %d, sts(%d)"), l_Auto_Diffusion_adc, l_vfReq[0].status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_string);

    l_trace_string.Format(_T("Auto_Diffusion_iso = %d, sts(%d)"), l_Auto_Diffusion_iso, l_vfReq[1].status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_string);

    l_trace_string.Format(_T("Auto_Perfusion = %d, sts(%d)"), l_Auto_Perfusion, l_vfReq[2].status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_string);

    l_trace_string.Format(_T("Auto_Perfusion_map = %d, sts(%d)"), l_Auto_Perfusion_map, l_vfReq[3].status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_string);

    l_trace_string.Format(_T("Auto_Dynamic_sub = %d, sts(%d)"), l_Auto_Dynamic_sub, l_vfReq[4].status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_string);

    l_trace_string.Format(_T("Auto_MIP_preview = %d, sts(%d)"), l_Auto_MIP_preview, l_vfReq[5].status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_string);

    l_trace_string.Format(_T("Auto_MovingBed_sub = %d, sts(%d)"), l_Auto_MovingBed_sub, l_vfReq[6].status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_string);

    l_trace_string.Format(_T("Auto_Dynamic_sub_image = %d, sts(%d)"), l_Auto_Dynamic_sub_image, l_vfReq[7].status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_string);

    l_trace_string.Format(_T("Auto_Geometric = %d, sts(%d)"), l_Auto_Geometric, l_vfReq[8].status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_string);

    l_trace_string.Format(_T("Auto_AMB_sub = %d, sts(%d)"), l_Auto_AMB_sub, l_vfReq[9].status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_string);

    if ((l_Auto_Dynamic_sub_image & VFC_SUBTRACTION_COMPLEX) != 0) {
        l_Auto_Dynamic_sub = FALSE ;

    }

    /* AMB 自動差分処理はAMB撮像時以外は処理対象としない V7.10 */
    //if( !PqmPmIsActive(K_PM_TYPE_AMB))    Auto_AMB_sub = FALSE ;
    bool l_postproc_flg = false;

    if ((l_vfReq[0].status == E_NO_ERROR && l_Auto_Diffusion_adc == TRUE) ||
        (l_vfReq[1].status == E_NO_ERROR && l_Auto_Diffusion_iso == TRUE) ||
        (l_vfReq[2].status == E_NO_ERROR && l_Auto_Perfusion == TRUE) ||
        (l_vfReq[3].status == E_NO_ERROR && l_Auto_Perfusion_map) ||
        (l_vfReq[4].status == E_NO_ERROR && l_Auto_Dynamic_sub == TRUE) ||
        (l_vfReq[5].status == E_NO_ERROR && l_Auto_MIP_preview[0]) ||
        (l_vfReq[6].status == E_NO_ERROR && l_Auto_MovingBed_sub == TRUE) ||
        (l_vfReq[8].status == E_NO_ERROR && l_Auto_Geometric == TRUE) ||    /* V6.10　磁場歪処理 */
        (l_vfReq[9].status == E_NO_ERROR && l_Auto_AMB_sub == TRUE)) {  /* V7.10　AMB Sub */
        l_postproc_flg = TRUE ;

    } else {
        /* system call -> 関数に変更した。　MARS V6.00 */
        char* f_study_name_char = {0};
        int l_length = m_study_name.GetLength() ;
        f_study_name_char = new char[l_length + 1];
        memset(f_study_name_char, 0, (l_length + 1)*sizeof(char));
        wcstombs(f_study_name_char, m_study_name, l_length + 1);

        ClibMathServerAdapter MathServerAdapter(m_vfdb_handle);

        if (MathServerAdapter.msCheckPostProc(f_study_name_char, f_protocol_index) > 0) {
            l_postproc_flg = true;

        } else {
            l_postproc_flg = false;
        }

        DEL_PTR_ARY(f_study_name_char);

        //-Patni-HAR/2009Aug11/Added/Memory Leaks
    }

    return l_postproc_flg;
}

//+Patni+MP/2009Jun07/Added/ACE-2/Missing Vantage Auto Post Proc
//****************************Method Header************************************
//Method Name   :PutPostscript()
//Author        :PATNI / MRP
//Purpose       :
//*****************************************************************************
bool CPQMStudy::PutPostscript(const int f_protocol_index, char* script)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::PutPostscript");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Inside PutPostscript"));

    if (f_protocol_index < 0) {

        return false;
    }

    if (script == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Script NULL"));
        return false;         //Patni-RUP/2009Aug25/Modified/cpp test corrections
    }

    VfPathElem_t        l_nodePath[SVD_ST_SLICE] = {
        {SVN_ST_PROTOCOL, 0},
        {SVN_ST_SUBPROTOCOL, 0},
        {SVN_ST_SLICEGROUP, 0},
        {SVN_ST_SLICE, 0}
    };

    VfFieldSpec_t       l_vfReq[8] = {0};
    CString l_trace_string = _T("");

    l_nodePath[SVD_ST_PROTOCOL - 1].index = f_protocol_index;

    CString l_scriptString(script);
    l_trace_string.Format(_T("script = [\n%s]\n"), l_scriptString);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_string);

    int n = 0;
    VFF_SET_ARGS(l_vfReq[n], SVN_PRC_PROMPTER_SCRIPT,
                 SVT_PRC_PROMPTER_SCRIPT, strlen(script),
                 script, VFO_REPLACE);
    n++;

    time_t tm  = 0;
    int l_num = 0;
    status_t l_vfStatus = m_vfdb_handle->PutFields(l_nodePath, SVD_ST_PROTOCOL, l_vfReq, n, &l_num, &tm);

    if_error(l_vfStatus) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("VfAppPutFields error."));

    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("VfAppPutFields end"));
    return TRUE;
}
//-Patni+MP/2009Jun07/Added/ACE-2/Missing Vantage Auto Post Proc


//****************************Method Header************************************
//Method Name   :WritePrescanTagsForProtocol()
//Author        :PATNI / MRP
//Purpose       :
//*****************************************************************************
bool CPQMStudy::WritePrescanTagsForProtocol(
    CVarFieldHandle* f_vf_handle,
    VfFieldSpec_t* f_vfReq,
    const int f_total_elements,
    const int f_start_pos,
    const int f_end_pos,
    const bool f_write_to_scratchpad
)const
{

    LPCTSTR FUNC_NAME = _T("CPQMStudy::WritePrescanTagsForProtocol");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    VfFieldSpec_t* l_vfReq_next_prot = new VfFieldSpec_t[f_total_elements];

    VfPathElem_t        l_nodePath[SVD_ST_SLICE] = {
        {SVN_ST_PROTOCOL, 0},
        {SVN_ST_SUBPROTOCOL, 0},
        {SVN_ST_SLICEGROUP, 0},
        {SVN_ST_SLICE, 0}
    };

    int l_fields_for_next_prot = 0;

    for (int tag_index = 0; tag_index < f_total_elements ; tag_index++) {

        if (f_vfReq[tag_index].status != E_NO_ERROR) {
            continue;
        }

        VFF_SET_ARGS(l_vfReq_next_prot[l_fields_for_next_prot], f_vfReq[tag_index].name,
                     f_vfReq[tag_index].type, f_vfReq[tag_index].size,
                     f_vfReq[tag_index].buffer, VFO_REPLACE);
        l_fields_for_next_prot++;

    }

    time_t tm = 0;
    int l_num = 0;

    if (f_write_to_scratchpad) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Applying Prescan Tags in Scratchpad..."));

        l_nodePath[SVD_ST_PROTOCOL - 1].index = 0;

        status_t l_vfStatus = f_vf_handle->PutFields(l_nodePath, SVD_ST_PROTOCOL, l_vfReq_next_prot, l_fields_for_next_prot, &l_num, &tm);

        if (l_vfStatus != E_NO_ERROR) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Could not update Prescan Tag in ScratchPad. Error Occured..."));

            DEL_PTR_ARY(l_vfReq_next_prot);
            return false;
        }

    } else {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Applying Prescan Tags in Study File..."));

        CString l_trace_string = _T("");

        const int l_total_prototcols = GetAllItems();

        if (((f_start_pos < 1) || (f_start_pos > l_total_prototcols)) ||
            ((f_end_pos < 1) || (f_end_pos > l_total_prototcols))
           ) {

            l_trace_string.Format(_T("Invalid Input(f_start_pos, f_end_pos) = (%d,%d). Error Occured... "), f_start_pos, f_end_pos);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_string);

            DEL_PTR_ARY(l_vfReq_next_prot);
            return false;
        }

        CPqmProtocol* l_obj = NULL;

        for (int i = f_start_pos; i <= f_end_pos; i++) {

            l_obj = GetItemFromPosition(i);

            if (l_obj == NULL) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_obj is NULL. Error Occured.."));

                DEL_PTR_ARY(l_vfReq_next_prot);
                return false;
            }

            l_trace_string.Format(_T("Protocol (Pos, Acqorder, Status) = (%d, %d, %d)"), i, l_obj->GetProtocol() , l_obj->GetAcqStatus());
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_string);

            if (!((l_obj->GetAcqStatus() == C_PR_STATUS_WAIT) || (l_obj->GetAcqStatus() == C_PR_STATUS_CURRENT))) {

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Protocol status should be WAIT or Current. Error Occured.."));
                continue;
            }

            l_nodePath[SVD_ST_PROTOCOL - 1].index = l_obj->GetProtocol();
            status_t l_vfStatus = f_vf_handle->PutFields(l_nodePath, SVD_ST_PROTOCOL, l_vfReq_next_prot, l_fields_for_next_prot, &l_num, &tm);

            if ((l_vfStatus != E_NO_ERROR) || (l_num != l_fields_for_next_prot)) {
                l_trace_string.Format(_T("Could not update B1 Shim Tags for protocol. Status = %d, l_num = %d, l_fields_for_next_prot = %d"), l_vfStatus, l_num, l_fields_for_next_prot);
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_string);
            }
        }
    }

    DEL_PTR_ARY(l_vfReq_next_prot);
    return true;
}


//+Patni-AMT/2009Apr22/Added/PSP1#001
//****************************Method Header************************************
//Method Name   : ClearProtocolVector()
//Author        : PATNI/AMT
//Purpose       : Free all the objects from Protocol Vector and delete the vector
//*****************************************************************************
BOOL CPQMStudy::ClearProtocolVector()
{
    //Patni-DKH/2011Mar04/Added/TMSC-REDMINE-1486
    CScopedLock l_scoped_lock(&m_protocol_vec_mutex);

    //Patni-MJC/2009Aug17/Modified/cpp test corrections Need to Check
    UTIL::DeleteAndClearContainer(&m_protocol_vec);
    return TRUE;
}

//****************************Method Header************************************
//Method Name   : ClearSequenceList()
//Author        : PATNI/AMT
//Purpose       : Free all the objects from Sequence List and delete Sequence List
//*****************************************************************************
BOOL CPQMStudy::ClearSequenceList(
    SequenceList& f_seq_list
)
{
    UTIL::DeleteAndClearMFCList<CPASCOMSequence>(&f_seq_list);
    return TRUE;
}
//-Patni-AMT/2009Apr22/Added/PSP1#001

/****************************Method Header************************************
//Method Name   :PqmPrStoreFile
//Author        :PATNI/
//Purpose       :
//*****************************************************************************/
void CPQMStudy::PqmPrStoreFile(char*    store_file,
                               char*    to_file,
                               char*    comment
                              )
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::PqmPrStoreFile");
    char    date[64] = "";//Patni-MJC/2009Aug17/Modified/cpp test corrections
    char    command[512] = ""; //Patni-MJC/2009Aug17/Modified/cpp test corrections
    time_t  now = 0;//Patni-MJC/2009Aug17/Modified/cpp test corrections
    char    buff[BUFSIZ] = "";//Patni-MJC/2009Aug17/Modified/cpp test corrections


    time(&now);
    struct tm*           tms = localtime(&now); //Patni-MJC/2009Aug17/Modified/cpp test corrections

    //TlGetTmpPath(path);
    char*   path = getenv("GP_TMP_DIR"/*"GP_TMP"*/);    //Patni-MJC/2009Aug17/Modified/cpp test corrections

    //+Patni-PJS/2009Sep16/Modified/Internal Code review comment
    if (path == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Failed to get value of GP_TMP_DIR environment variable"));
        return;
    }

    //-Patni-PJS/2009Sep16/Modified/Internal Code review comment
    strcat(path, "/PQM.ChkVG.");
    strcat(path, to_file);
    strcat(path, "_");
    strcat(path, date);

    sprintf(command, "cp %s %s & ", store_file, path);
    system(command);

    sprintf(buff, "%s\nLOG FILE[%s]", comment, path);
    sprintf(comment, "%s !", buff);

}

/****************************Method Header************************************
//Method Name   :PqmPrCountNode
//Author        :PATNI/
//Purpose       :
//*****************************************************************************/
status_t CPQMStudy::PqmPrCountNode(CVarFieldHandle* const vf_handle,
                                   VfPathElem_t*    nodePath,
                                   const int      depth,
                                   int*     numNode
                                  )
{
    BITFLD_DECL(mask, MAX_NODES);

    int name = nodePath[depth].subtree_name;
    int total = MAX_NODES;
    int active = 0;

    status_t l_status = vf_handle->ScanSubtree(nodePath, depth, name,
                        mask, &total, &active);

    if (!l_status) {
        if (name == SVN_ST_SLICE) {
            *numNode += active;

        } else {
            for (int index = 0; index < total; index++) {
                if (BITFLD_ON(mask, index)) {
                    nodePath[depth].index = index;
                    if_error(PqmPrCountNode(vf_handle, nodePath,
                                            depth + 1, numNode))
                    return E_ERROR;
                }
            }
        }

    } else {
        return E_ERROR;
    }

    return E_NO_ERROR;
}

/****************************Method Header************************************
//Method Name   :PqmPrCheckVisualGr
//Author        :PATNI/
//Purpose       :
//*****************************************************************************/
void    CPQMStudy::PqmPrCheckVisualGr(char*     file_name,
                                      char*     to_file,
                                      const int       prot,
                                      char*     file,
                                      const int       line
                                     )
{
    /* VfPathElem_t for sub-protocol. */
    VfPathElem_t    pelm[] = {
        {SVN_ST_PROTOCOL,   0},
        {SVN_ST_VISUALGROUP,    0},
        {0,         0}
    };
    char    buff[BUFSIZ] = "";//Patni-MJC/2009Aug17/Modified/cpp test corrections
    BITFLD_DECL(mask, MAX_NODES);
    DB_FUNC_ENTER("[PqmPrCheckVisualGr]:");

    CVarFieldHandle vf_handle(file_name);
    status_t status = vf_handle.Open();

    if_error(status) {
        sprintf(buff, "Open ERROR. [%s] : (%s:%d) (%s:%d)",
                file_name, file, line, __FILE__, __LINE__);

        DB_PRINTF("w", ("%s \n", buff));
        return;
    }

    memset(mask, 0, sizeof(mask));

    int     total = MAX_NODES;
    int     active = 0;
    status = vf_handle.ScanSubtree(pelm, SVD_ST_PROTOCOL, SVN_ST_VISUALGROUP,
                                   mask, &total, &active);

    if (status != E_NO_ERROR) {
        sprintf(buff, "NO VISUAL Gr.(%s:%d)", file, line);

        PqmPrStoreFile(file_name, to_file, buff);
        //PqmUiErrorMessage(buff,ERR_ERROR);
        return;

    } else if (!active) {
        sprintf(buff, "NO Active node in the Visual Group.(%s:%d)",
                file, line);

        PqmPrStoreFile(file_name, to_file, buff);
        //PqmUiErrorMessage(buff,ERR_ERROR);

        return;

    } else {
        //  DB_PRINTF("w",("The study [%s] has a Visual group node "
        //             "for protocol [%d] !\n",
        //             file_name,prot));
    }

    /* Check Slice number. */
    PqmPrCheckSliceNumber(file_name, to_file, prot, file, line);
}

/****************************Method Header************************************
//Method Name   :PqmPrCheckSliceNumber
//Author        :PATNI/
//Purpose       :
//*****************************************************************************/
void CPQMStudy::PqmPrCheckSliceNumber(
    char*   file_name,
    char*   to_file,
    const int prot,
    char*   file,
    const int line
)
{
    VfPathElem_t    nodePath[SVD_ST_SLICE] = {
        {SVN_ST_PROTOCOL, 0},
        {SVN_ST_SUBPROTOCOL, 0},
        {SVN_ST_SLICEGROUP, 0},
        {SVN_ST_SLICE, 0}
    };
    VfFieldSpec_t   vfReq[5] = {0};//Patni-MJC/2009Aug17/Modified/cpp test corrections
    int         slice = 0;//Patni-MJC/2009Aug17/Modified/cpp test corrections
    int         img_mode = 0, app_code = 0;//Patni-MJC/2009Aug17/Modified/cpp test corrections
    int         num = 0, n = 0;//Patni-MJC/2009Aug17/Modified/cpp test corrections
    DB_FUNC_ENTER("[pqmPrCheckSliceNumber]:");

    CVarFieldHandle vf_handle(file_name);
    status_t vfStatus = vf_handle.Open();

    if_error(vfStatus) {
        //DB_PRINTF("",("CPQMStudy::Open error [%s]\n",file_name));
        return;
    }
    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(vfReq, 0, sizeof(VfFieldSpec_t) * 5);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    n = 0;
    VFF_SET_ARGS(vfReq[n], SVN_PLN_APP_CODE,
                 SVT_PLN_APP_CODE, sizeof(int),
                 &app_code, 0);
    n++;
    VFF_SET_ARGS(vfReq[n], SVN_PLN_IMAGING_MODE,
                 SVT_PLN_IMAGING_MODE, sizeof(int),
                 &img_mode, 0);
    n++;
    VFF_SET_ARGS(vfReq[n], SVN_PLN_TOTAL_SLICES,
                 SVT_PLN_TOTAL_SLICES, sizeof(int),
                 &slice, 0);
    n++;

    vfStatus = vf_handle.GetFields(nodePath, SVD_ST_PROTOCOL, vfReq, n, &num);

    if_error(vfStatus) {
        //DB_PRINTF("",("VfAppGetFields error.\n"));
        if (vfReq[0].status != E_NO_ERROR)
            // PqmUiErrorMessage("Get IMAGING_MODE error.",ERR_ERROR);

            if (vfReq[1].status != E_NO_ERROR)
                //   PqmUiErrorMessage("Get TOTAL_SLICES error.",ERR_ERROR);

                //DB_PRINTF("",("VfAppGetFields error. [%s] %d\n",
                //        __FILE__,__LINE__));
                return;
    }



    if (app_code == VFC_APP_CODE_QUAD_SE ||
        app_code == VFC_APP_CODE_QUAD_FE) {
        int mux = 0;//Patni-MJC/2009Aug17/Modified/cpp test corrections
        // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
        memset(vfReq, 0, sizeof(VfFieldSpec_t) * 5);
        // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
        n = 0;
        VFF_SET_ARGS(vfReq[n], SVN_PLN_QUAD_MUX,
                     SVT_PLN_QUAD_MUX, sizeof(int),
                     &mux, 0);
        n++;

        vfStatus = vf_handle.GetFields(nodePath, SVD_ST_PROTOCOL,
                                       vfReq, n, &num);

        if_error(vfStatus) {
            //DB_PRINTF("",("VfAppGetFields error.\n"));
            if (vfReq[0].status != E_NO_ERROR)
                //PqmUiErrorMessage("Get IMAGING_MODE error.",ERR_ERROR);

                if (vfReq[1].status != E_NO_ERROR)
                    //PqmUiErrorMessage("Get TOTAL_SLICES error.",ERR_ERROR);

                    //      DB_PRINTF("",("VfAppGetFields error. [%s] %d\n",
                    //            __FILE__,__LINE__));
                    return;
        }

        slice /= mux;
    }

    if (img_mode == VFC_IMAGING_MODE_2D) {
        char    tmp_buff[512] = "";//Patni-MJC/2009Aug17/Modified/cpp test corrections
        int total_slice = 0;

        nodePath[0].index = prot;

        if (!PqmPrCountNode(&vf_handle, nodePath, SVD_ST_PROTOCOL, &total_slice)) {
            if (total_slice != slice) {
                sprintf(tmp_buff,
                        "Illegal Slice Group. (%s:%d)\n"
                        "[file=%s,prot=%d][slice=%d,total_slice=%d]",
                        file, line, file_name, prot, slice, total_slice);

                PqmPrStoreFile(file_name, to_file, tmp_buff);
                //PqmUiErrorMessage(tmp_buff,ERR_ERROR);
            }

            //      DB_PRINTF("w",("=======>>> total_slice=%d,slice=%d\n",
            //             total_slice,slice));

        } else {

            sprintf(tmp_buff,
                    "VfScanSubtree error.\nfile,prot=[%s,%d],%s %d",
                    file_name, prot, __FILE__, __LINE__);
        }
    }
}

/****************************Method Header************************************
//Method Name   :PqmPrSetPatientWeight
//Author        :PATNI/
//Purpose       :
//*****************************************************************************/
status_t CPQMStudy::PqmPrSetPatientWeight(const char* file)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::PqmPrSetPatientWeight");
    int         n = 0;
    VfFieldSpec_t   vfReq[4] = {0}; //Patni-MJC/2009Aug17/Modified/cpp test corrections
    DB_FUNC_ENTER("[PqmPrSetScanDateTime]:");
    //Patni-RAJ/ADDED On 10/09/2009

    CVarFieldHandle vf_handle(file);

    if (vf_handle.VuiOpen() != E_NO_ERROR) {
        return E_ERROR;
    }

    memset(vfReq, 0, sizeof(VfFieldSpec_t) * 4);//Patni-MJC/2009Aug17/Modified/cpp test corrections

    n = 0;
    VFF_SET_ARGS(vfReq[n], SVN_SCH_WEIGHT, SVT_SCH_WEIGHT,
                 sizeof(float), &m_weight,
                 VFO_REPLACE);
    n++;
    VFF_SET_ARGS(vfReq[n], SVN_SCH_HEIGHT, SVT_SCH_HEIGHT,
                 sizeof(float), &m_height,
                 VFO_REPLACE);
    n++;
    VFF_SET_ARGS(vfReq[n], SVN_SCH_GENDER, SVT_SCH_GENDER,
                 sizeof(int), &m_gender,
                 VFO_REPLACE);
    n++;

    //+Patni-SS/2009Apr01/PSP1#VFLOCK
    CString l_log_msg = _T("");
    l_log_msg.Format(_T("SVN_SCH_WEIGHT =%d , SVN_SCH_HEIGHT = %d, SVN_SCH_GENDER = %d "), m_weight, m_height, m_gender);
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_log_msg);
    //-Patni-SS/2009Apr01/PSP1#VFLOCK
    time_t      tm = 0;             //Patni-MJC/2009Aug17/Modified/cpp test corrections
    int num = 0;
    status_t l_status = vf_handle.PutFields(NULL, SVD_ST_STUDY, vfReq, n, &num, &tm);

    l_log_msg.Format(L"Status of GetProtocolValues is:- %d", l_status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_msg);

    return E_NO_ERROR;
}   /* PqmPrSetPatientWeight() */

//***************************Method Header*************************************
//Method Name    :CheckForMultiCoverageScan()
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
BOOL CPQMStudy::CheckForMultiCoverageScan(
    CPqmProtocol* protocol
)const
{

    LPCTSTR FUNC_NAME = _T("CPQMStudy::CheckForMultiCoverageScan");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CheckForMultiCoverageScan"));

    static VfPathElem_t   path[] = {
        SVN_ST_PROTOCOL,  0,
        SVN_ST_PROCESSED, 0,
        SVN_ST_ECHO,      0,
        SVN_ST_POSITION,  0
    };

    //+Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    if (protocol == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("protocol pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    protocol->SetNumOfCoverages(0);

    //+Patni-MJC/2009Aug27/Modified/cpp test corrections
    if (m_pqm_ptr == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug27/Modified/cpp test corrections

    //Patni-PJS/2010Feb24/Added/DeFT# PROPOSAL_08-Remove EZScan
    if (!(protocol->GetHoldScan() & VFC_HOLD_PRE_SCAN)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("!(protocol->GetHoldScan() & VFC_HOLD_PRE_SCAN)"));

        return E_NO_ERROR;
    }

    path[SVD_ST_PROTOCOL - 1].index = protocol->GetProtocol();
    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    VfFieldSpec_t vfReq[NUM_2] = {0};
    memset(vfReq, 0, sizeof(VfFieldSpec_t) * NUM_2);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008

    int n = 0;
    int coverage = 0;
    VFF_SET_ARGS(vfReq[n],
                 SVN_PLN_COVERAGES,
                 VFT_INT32,
                 sizeof(coverage),
                 &coverage,
                 0L);

    n++;
    int num = 0;
    BOOL status = m_vfdb_handle->GetFields(path, SVD_ST_PROTOCOL, vfReq, n, &num);
    CString l_print_string(_T(""));

    if (status != E_NO_ERROR &&  num != n) {

        l_print_string.Format(_T("Status of CPQMStudy::GetFields = %d"), status);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_print_string);
        return E_ERROR;
    }

    protocol->SetNumOfCoverages(coverage);

    if (coverage != 1) {
        l_print_string.Format(_T("Coverage = %d"), coverage);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("coverage != 1") + l_print_string);
        return E_ERROR;
    }

    l_print_string.Format(L"COVERAGES %d\n", coverage);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_print_string);
    return E_NO_ERROR;
}

//***************************Method Header*************************************
//Method Name    :RecalculateSAR()
//Author         :PATNI/MRP
//Purpose        :This function calls the RecalculateSAR function of the
//                CPQMSAR class
//*****************************************************************************
void CPQMStudy::RecalculateSAR(
    CPqmProtocol* f_protocol
)const
{
    m_pqm_ptr->GetSAR()->RecalculateSAR(m_vfdb_handle, f_protocol);
}

//****************************Method Header************************************
//Method Name   : PqmUiConvertVector()
//Author        : PATNI\MSN
//Purpose       :
//*****************************************************************************
void CPQMStudy::PqmUiConvertVector(VftPatori_t* patori,
                                   bool_t       toMachine,
                                   f32vec3_t*   fr,
                                   f32vec3_t*   to
                                  )
{
#define SET_VEC(vx,vy,vz)   to->x=(vx),to->y=(vy),to->z=(vz)

    if (toMachine) {
        if (patori->end_in != VFC_PATORI_HEAD_FIRST) {
            switch (patori->side_up) {
                case VFC_PATORI_SUPINE:
                    SET_VEC(fr->x, fr->y, -fr->z);
                    break;

                case VFC_PATORI_PRONE:
                    SET_VEC(-fr->x, -fr->y, -fr->z);
                    break;

                case VFC_PATORI_LEFT_UP:
                    SET_VEC(fr->y, -fr->x, -fr->z);
                    break;

                case VFC_PATORI_RIGHT_UP:
                    SET_VEC(-fr->y, fr->x, -fr->z);
                    break;
            }

        } else {    /* VFC_PATORI_FEET_FIRST */
            switch (patori->side_up) {
                case VFC_PATORI_SUPINE:
                    SET_VEC(-fr->x, fr->y, fr->z);
                    break;

                case VFC_PATORI_PRONE:
                    SET_VEC(fr->x, -fr->y, fr->z);
                    break;

                case VFC_PATORI_LEFT_UP:
                    SET_VEC(-fr->y, -fr->x, fr->z);
                    break;

                case VFC_PATORI_RIGHT_UP:
                    SET_VEC(fr->y, fr->x, fr->z);
                    break;
            }
        }

    } else {
        if (patori->end_in != VFC_PATORI_HEAD_FIRST) {
            switch (patori->side_up) {
                case VFC_PATORI_SUPINE:
                    SET_VEC(fr->x, fr->y, -fr->z);
                    break;

                case VFC_PATORI_PRONE:
                    SET_VEC(-fr->x, -fr->y, -fr->z);
                    break;

                case VFC_PATORI_LEFT_UP:
                    SET_VEC(-fr->y, fr->x, -fr->z);
                    break;

                case VFC_PATORI_RIGHT_UP:
                    SET_VEC(fr->y, -fr->x, -fr->z);
                    break;
            }

        } else {    /* VFC_PATORI_FEET_FIRST */
            switch (patori->side_up) {
                case VFC_PATORI_SUPINE:
                    SET_VEC(-fr->x, fr->y, fr->z);
                    break;

                case VFC_PATORI_PRONE:
                    SET_VEC(fr->x, -fr->y, fr->z);
                    break;

                case VFC_PATORI_LEFT_UP:
                    SET_VEC(-fr->y, -fr->x, fr->z);
                    break;

                case VFC_PATORI_RIGHT_UP:
                    SET_VEC(fr->y, fr->x, fr->z);
                    break;
            }
        }
    }
}

//************************************Method Header************************************
// Method Name  : IsThisFlagInScanKind
// Author       : PATNI/ HEMANT
// Purpose      : This function checks weather the inscan flag value is OK with inscan protocols
//***********************************************************************************
bool CPQMStudy::IsThisFlagInScanKind(
    const int f_inscan_flag_value
)const
{

    switch (f_inscan_flag_value) {

        case VFC_INSCAN_MRS :
        case VFC_INSCAN_PWI :
        case VFC_INSCAN_FMRI:

            //Patni/2011Mar24/Modified/WFDA_Offshore
        case VFC_INSCAN_DWI :
        case VFC_INSCAN_MR3D :
            return true ;
    }

    return false ;
}

//+Patni-Hemant/2010Feb16/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CPqmProtocol* CPQMStudy::pqmAcqTopProtocol(void)
{
    CPqmProtocol* prObj = NULL;

    GetTopWaitPos(&prObj);	/* WAIT:O FILLER_WAIT:X */
    CPqmProtocol* curObj = GetCurrentProtocol();

    if (prObj == NULL || prObj->GetProtocol() < 0) {
        //PqmUiErrorDialog(PqmUiFetchLiteral("ERR_ILLEGAL_PROTOCOL"),ERR_ERROR);
        CPQMLogMgr::GetInstance()->DisplayScanError(
            IDS_ERR_ILLEGAL_PROTOCOL, _T("IDS_ERR_ILLEGAL_PROTOCOL"), SCAN_ERR_NOTIFY, _T("PQMStudy.cpp"));

        SetScanModeOfHead();

        //PqmUiSetSensitive(W_SCAN_START_BTN,FALSE);

        CPqmPmAcqman* pqm_pm_acqman = m_pqm_ptr->GetAcqManager();
        CScanProcedure* scan_proc = pqm_pm_acqman->GetScanProcedure();
        scan_proc->ResetCurrentRequest();
        return NULL;
    }

    /* Prolim scan will not run when in use*/
    if (prObj->GetProcessing() > 0) {
        return NULL;
    }

    /* If the protocol is not run during the imaging scan */
    if (curObj != NULL) {
        return NULL;
    }

    return prObj;
}
//-Patni-Hemant/2010Feb16/Added/ACE-Phase#3/CDS Requirement

//+Patni-Hemant/2010Apr27/Added/JaFT/IR-137
//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPQMStudy::SaveLaterality(
    const int& f_protocol_index,
    const int& f_laterality
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SaveLaterality");

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, FUNC_NAME);

    time_t          ts = 0;//Patni-MJC/2009Aug17/Modified/cpp test corrections
    VfFieldSpec_t   l_vfField[1] = {0};//Patni-MJC/2009Aug17/Modified/cpp test corrections
    VfPathElem_t    l_path[2] = {0};//Patni-MJC/2009Aug17/Modified/cpp test corrections
    int             l_numDone = 0;

    memset(l_path, 0, sizeof(VfPathElem_t) * 2);
    memset(l_vfField, 0, sizeof(VfFieldSpec_t));

    l_path[0].subtree_name = SVN_ST_PROTOCOL;
    l_path[0].index = f_protocol_index;

    std::string l_laterality_str("");

    //+Patni-HAR/2010Jun30/Added/V1.30#TMSC-REDMINE-382
    int l_laterality = -2;

    if ((f_laterality == -1) &&
        (false == GetLaterality(f_protocol_index, l_laterality))) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("No Laterality"));
        return true;
    }

    //-Patni-HAR/2010Jun30/Added/V1.30#TMSC-REDMINE-382

    if (f_laterality == -1) {

        VFF_SET_ARGS(l_vfField[0],
                     SVN_PLN_LATERALITY,
                     SVT_PLN_LATERALITY,
                     0,
                     NULL,
                     VFO_DELETE);

    } else {

        CString l_str = CPqmUtilities::GetStrForLaterality(f_laterality);

        UTIL::Wcs2Mbs(&l_laterality_str, l_str);
        VFF_SET_ARGS(l_vfField[0],
                     SVN_PLN_LATERALITY,
                     SVT_PLN_LATERALITY,
                     l_laterality_str.length() + 1,
                     const_cast<char*>(l_laterality_str.c_str()),
                     VFO_REPLACE);
    }

    status_t l_status = m_vfdb_handle->PutFields(l_path, SVN_ST_PROTOCOL, &l_vfField[0],
                        1, &l_numDone, &ts);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exiting CPQMStudy::SaveLaterality"));

    return (l_status == E_NO_ERROR);
}
//-Patni-Hemant/2010Apr27/Added/JaFT/IR-137

//Added by Meghana On 8/6/2008 9:23:14 AM
//***************************Method Header*************************************
//Method Name    :MoveProtObjInVector()
//Author         :PATNI/MRP
//Purpose        :Rearrange the protocol objects in the vector as per the
//                position on the GUI
//*****************************************************************************
void CPQMStudy::MoveProtObjInVector(
    const int f_from_pos,
    const int f_to_pos
)
{
    //  __asm int 3;
    protocolvec::iterator l_iterator = NULL, l_start = NULL, l_middle = NULL, l_end = NULL;//Patni-MJC/2009Aug17/Modified/cpp test corrections

    //Patni-DKH/2011Mar04/Added/TMSC-REDMINE-1486
    CScopedLock l_scoped_lock(&m_protocol_vec_mutex);

    l_iterator = m_protocol_vec.begin();

    if (f_from_pos < f_to_pos) {
        l_start = l_iterator + f_from_pos;
        l_middle = l_start + 1;
        l_end = l_iterator + f_to_pos + 1;

    } else  if (f_from_pos > f_to_pos) {
        l_start = l_iterator + f_to_pos;
        l_middle = l_iterator + f_from_pos;
        l_end = l_iterator + f_from_pos + 1;

    } else {
        return;
    }

    std::rotate(l_start, l_middle, l_end);
    //Added by Meghana On 8/6/2008 11:37:24 AM
    //Now set the positions of all the elememnts in the vector
    RenumberPositionsInVector();

}

//+Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image
//**************************************************************************
//Method Name   : pqmPmInitCouchAndSourceProt
//Author        : PATNI\Manish
//Purpose       :
//**************************************************************************
status_t CPQMStudy::pqmPmInitCouchAndSourceProt(CVarFieldHandle* const vf_handle, int prot)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::pqmPmInitCouchAndSourceProt");

    int32_t         flg  = 0;
    int32_t         hold = 0;
    VfPathElem_t    path[2];
    VfFieldSpec_t   vfReq[2];
    path[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    path[SVD_ST_PROTOCOL - 1].index = prot;

    int n = 0, num = 0;
    int s = sizeof(int32_t) ;
    VFF_SET_ARGS(vfReq[n], SVN_PLN_MOVE_COUCH_METHOD_FLAG, SVT_PLN_MOVE_COUCH_METHOD_FLAG,
                 s, &flg, NULL);
    n++;
    s = sizeof(int32_t) ;
    VFF_SET_ARGS(vfReq[n], SVN_PLN_HOLD, SVT_PLN_HOLD,
                 s, &hold, NULL);
    n++;

    status_t sts = vf_handle->GetFields(path, SVD_ST_PROTOCOL, vfReq, n, &num);

    if (sts != E_NO_ERROR || n != num) {
        if (vfReq[0].status == VFE_NOT_FOUND) {
            flg = VFC_MOVE_COUCH_METHOD_NONE;
        }
    }

    time_t          tmStamp;

    if ((flg  == VFC_MOVE_COUCH_METHOD_NONE) && (hold & VFC_HOLD_COUCH)) {
        flg = VFC_MOVE_COUCH_METHOD_PLAN_CENTER_POSI;
        n = num = 0;
        s = sizeof(int32_t) ;
        VFF_SET_ARGS(vfReq[n], SVN_PLN_MOVE_COUCH_METHOD_FLAG, SVT_PLN_MOVE_COUCH_METHOD_FLAG,
                     s, &flg, VFO_REPLACE);
        n++;
        sts = vf_handle->PutFields(path, SVD_ST_PROTOCOL, vfReq, n, &num, &tmStamp);

        if (E_NO_ERROR != sts || num != n) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
            return E_ERROR;
        }
    }

    int32_t no = -1;
    n = num = 0;
    s = sizeof(int32_t) ;
    VFF_SET_ARGS(vfReq[n], SVN_PLN_COPY_SOURCE_PROTOCOL_NO, SVT_PLN_COPY_SOURCE_PROTOCOL_NO,
                 s, &no, VFO_REPLACE);
    n++;
    sts = vf_handle->PutFields(path, SVD_ST_PROTOCOL, vfReq, n, &num, &tmStamp);

    if (E_NO_ERROR != sts || num != n) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
    }

    return sts;
}

//**************************************************************************
//Method Name   : PqmPmMemoryResetProtInfo
//Author        : PATNI\Manish
//Purpose       :
//**************************************************************************
status_t CPQMStudy::PqmPmMemoryResetProtInfo(CVarFieldHandle* const vf_handle,
        const int  f_protocol,
        //Patni-PJS+DKH/2010Nov5/Modified/REDMINE-927
        void* f_mode, void* f_data)
{
    //+Patni-PJS+DKH/2010Nov5/Modified/REDMINE-927
    LPCTSTR FUNC_NAME = _T("CPQMStudy::PqmPmMemoryResetProtInfo");
    status_t    status = E_ERROR;
    //-Patni-PJS+DKH/2010Nov5/Modified/REDMINE-927
    BITFLD_DECL(mask, MAX_NODES);

    //+Patni-PJS+DKH/2010Nov5/Added/REDMINE-927
    if (NULL == f_data) {
        //Trace Log
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,  _T("f_data is NULL"));
        return status;
    }

    CPQMChildstudy* l_child_study = (CPQMChildstudy*)f_data;

    if (NULL == l_child_study) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,  _T("l_child_study is NULL"));
        return status;
    }

    //-Patni-PJS+DKH/2010Nov5/Added/REDMINE-927
    l_child_study->m_study->ClearFields(vf_handle, f_protocol);
    l_child_study->m_study->ResetProtInfo(vf_handle, f_protocol, (void*)FALSE);

    VftPatori_t l_patori;
    //+Patni-PJS+DKH/2010Nov5/Modified/REDMINE-927
    //CPqmProtocol* l_tempprotocol = NULL;
    //l_tempprotocol = l_child_study->m_study->GetProtocol(f_protocol);
    //if (NULL == l_tempprotocol) {
    //  return status;
    //}
    protocolvec::const_iterator v1_Iter = NULL;
    /* For Orientation */
    /*for (l_protocol = l_child_study->m_study->GetNextItemFromPos(l_tempprotocol);
         l_protocol != NULL;
         l_protocol = l_child_study->m_study->GetNextItemFromPos(l_tempprotocol)) {*/
    //-Patni-PJS+DKH/2010Nov5/Modified/REDMINE-927
    CPqmProtocol*    l_protocol = NULL;

    for (v1_Iter = l_child_study->m_study->m_protocol_vec.begin();
         v1_Iter != l_child_study->m_study->m_protocol_vec.end(); v1_Iter++) {//Patni-PJS+DKH/2010Nov5/Added/REDMINE-927
        l_protocol = *v1_Iter;//Patni-PJS+DKH/2010Nov5/Added/REDMINE-927

        if (l_protocol  == NULL) continue;

        if (l_protocol->GetAcqStatus() == C_PR_STATUS_DONE ||
            l_protocol->GetAcqStatus() == C_PR_STATUS_CURRENT ||
            l_protocol->GetAcqStatus() == C_PR_STATUS_WAIT) {
            memset(mask, 0, sizeof(mask));
            l_patori.side_up = l_protocol->GetPqmPrMode()->GetPatoriSideUp();
            l_patori.end_in = l_protocol->GetPqmPrMode()->GetPatoriEndIn();
            status = l_child_study->m_study->SetProtocolValues(vf_handle,
                     l_protocol->GetProtocol(),
                     mask,
                     K_PR_PATIENT_ORIENT,
                     &l_patori,
                     sizeof(VftPatori_t),
                     NULL);
            break;
        }
    }

    pqmPmInitCouchAndSourceProt(vf_handle, f_protocol);
    return status;
}
//-Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image
//**************************************************************************
//Method Name   : UpdateNumchannelforCoilChannelMode
//Author        : PATNI\Pavan
//Purpose       :
//**************************************************************************
bool CPQMStudy::UpdateNumchannelforCoilChannelMode(const BOOL f_coil_channel_mode, CPqmProtocol* f_pqm_prot)
{
    LPCTSTR FUNC_NAME = _T("CPqm::UpdateNumchannelforCoilChannelMode");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL == f_pqm_prot) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Protocol pointer is NULL"));
        return false;
    }

    PqmSaturnCoil_t* l_saturn_coil = f_pqm_prot->GetSaturnCoil();

    if (NULL == l_saturn_coil) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Saturn coil pointer is NULL"));
        return false;
    }

    CCoilChannelMode* l_obj_channel_mode = m_pqm_ptr->GetCoilChannelMode();

    if (l_obj_channel_mode == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("l_obj_channel_mode is NULL"));
        return false;
    }

    if (!l_obj_channel_mode->IsCoilChannelModeApplicable() || (l_saturn_coil->numSection == -1) || (l_saturn_coil->CoilName == "")) { // 21-Oct-11
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("coil channel mode functionlity is not applicable"));
        return false;
    }

    int l_numchannel = -1;
    int l_total_num_signal = 0;

    if (CHANNEL_MODE_MAX == f_coil_channel_mode) {
        l_numchannel = l_obj_channel_mode->GetSystemEnableChannel();

    } else {
        // If channel mode is Normal or not supported then default channel would be used as normal channel
        //check selected section num signal count,
        //if num count is more then limited channel then set the channel mode as MAX

        l_numchannel = l_obj_channel_mode->GetLimitedChannel();

        if (-1 == f_coil_channel_mode) { // If channel mode is not while reload study
            l_obj_channel_mode->SetChannelModeSelection(f_pqm_prot->GetProtocol(), CHANNEL_MODE_NORMAL, f_pqm_prot);
        }
    }

    f_pqm_prot->SetSystemChannels(l_numchannel);
    CString msg = _T("");
    msg.Format(_T("PQM is using channel %d"), l_numchannel);
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, msg);

    return true;
}


//*****************************************************************************/
//Method Name    :DeleteCFASpectrum()
//Author         :PATNI/SS
//Purpose        :
//*****************************************************************************/
void CPQMStudy::DeleteCFASpectrum(
    const int f_protocol
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::DeleteCFASpectrum");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    VfPathElem_t l_pelm[1] = {0};
    time_t l_tm = 0;
    VfFieldSpec_t l_vf_req[2] = {0};

    //Patni-PVN/05Aug2010/Modified/NFD001-AutoMap-Shimm/Review
    memset(l_vf_req, NULL, sizeof(VfFieldSpec_t) * 2);

    int n = 0;
    VFF_SET_ARGS(l_vf_req[n], SVN_EXE_PRESCAN_FSY_SPECTRUM , SVT_EXE_PRESCAN_FSY_SPECTRUM, NULL, NULL, VFO_DELETE);
    n++;

    VFF_SET_ARGS(l_vf_req[n], SVN_EXE_SHIM_FSY_FLAG , SVT_EXE_SHIM_FSY_FLAG, sizeof(int), NULL, VFO_DELETE);
    n++;

    l_pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_pelm[SVD_ST_PROTOCOL - 1].index = f_protocol;

    int l_num = 0;
    m_vfdb_handle->PutFields(l_pelm, SVD_ST_PROTOCOL, l_vf_req, n, &l_num, &l_tm);
}

void CPQMStudy::SetScanOffsetModeFlag(
    CPqmProtocol* const f_protocol, const int f_value
) const
{

    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetScanOffsetModeFlag");

    if (NULL == f_protocol) {

        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Can not set SVN_PLN_SCANOFFSET_SET_FLAG for NULL protocol"));
        return ;
    }

    VfPathElem_t        path[4] = {0};
    memset(path, 0, sizeof(VfPathElem_t) * 4);          //Patni-MJC/2009Aug17/Modified/cpp test corrections
    VfFieldSpec_t       fieldSpec[4] = {0};
    memset(fieldSpec, 0, sizeof(VfFieldSpec_t) * 4);    //Patni-MJC/2009Aug17/Modified/cpp test corrections

    path[0].subtree_name = SVN_ST_PROTOCOL;
    path[0].index = f_protocol->GetProtocol();

    int l_mode_value = f_value;
    int n = 0;
    VFF_SET_ARGS(fieldSpec[n], SVN_PLN_SCANOFFSET_SET_FLAG, SVT_PLN_SCANOFFSET_SET_FLAG,
                 sizeof(l_mode_value), &l_mode_value, VFO_REPLACE);
    n++;

    int numDone = 0;
    time_t              ts = 0;
    status_t l_status = m_vfdb_handle->PutFields(path, SVD_ST_PROTOCOL, fieldSpec, n, &numDone, &ts);

    if_error(l_status) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Error While PutFields->SVN_PLN_SCANOFFSET_SET_FLAG"));
    }

    else {

        CString l_log_msg = _T("");
        l_log_msg.Format(_T("Updated SVN_PLN_SCANOFFSET_SET_FLAG = %d "), l_mode_value);

        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_log_msg);

    }
}

//***************************Method Header*************************************
//Method Name    :UpdateRTSAEValue()
//Author         :iGATE/AKR
//Purpose        :Read software SAE value and put in RT SAE value
//*****************************************************************************
status_t CPQMStudy::UpdateRTSAEValue(CPqmProtocol* f_protocol)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::UpdateRTSAEValue");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    status_t l_status = E_ERROR;

    if (NULL == f_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_protocol"));
        return l_status;
    }

    int32_t			l_sae_info[NUM_2] = {0};

    if (E_NO_ERROR != GetTotalSAEValueFromVarfield(f_protocol, l_sae_info)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetTotalSAEValueFromVarfield failed!!"));
        return E_ERROR;
    }

    if (l_sae_info[NUM_1] > 0) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Hardware SAE value already present"));
        return E_NO_ERROR;
    }

    if (l_sae_info[NUM_0] <= 0) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("LibSAR SAE value is not present"));
        return E_ERROR;
    }

    l_sae_info[NUM_1] = l_sae_info[NUM_0];

    if (E_NO_ERROR != PutTotalSAEValueToVarfield(f_protocol, l_sae_info)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutTotalSAEValueToVarfield failed"));
        return E_ERROR;
    }

    return E_NO_ERROR ;
}

//***************************Method Header*************************************
//Method Name    :PutTotalSAEValueToVarfield()
//Author         :iGATE/AKR
//Purpose        :Put software total SAE value to varfield
//*****************************************************************************
status_t CPQMStudy::PutTotalSAEValueToVarfield(CPqmProtocol* f_protocol, int32_t f_total_sae[])
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::PutTotalSAEValueToVarfield");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL == f_protocol || NULL == m_vfdb_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_protocol or m_vfdb_handle is NULL"));
        return E_ERROR;
    }

    VfPathElem_t    l_pelm[2] = {0};
    VfFieldSpec_t   l_vf_required[NUM_1] = {0};

    l_pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_pelm[SVD_ST_PROTOCOL - 1].index = f_protocol->GetProtocol();

    int	l_num = 0, l_num_done = 0;
    time_t l_time_stamp = 0;

    VFF_SET_ARGS(l_vf_required[l_num], SVN_PLN_SAE_TOTAL,
                 SVT_PLN_SAE_TOTAL, sizeof(int32_t)*NUM_2, f_total_sae, VFO_REPLACE);

    l_num++;

    status_t l_status = m_vfdb_handle->PutFields(l_pelm, SVD_ST_PROTOCOL, l_vf_required, l_num, &l_num_done, &l_time_stamp);

    if (E_NO_ERROR != l_status || l_num != l_num_done) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields failed"));
        return E_ERROR;
    }

    return E_NO_ERROR;

}

//***************************Method Header*************************************
//Method Name    :GetProtocolFromStudy()
//Author         :PATNI/SG
//Purpose        :
//*****************************************************************************
status_t CPQMStudy::GetProtocolFromStudy(VfDBHandle_t f_db_handle,
        int f_protocol,
        void* f_data)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetProtocolFromStudy");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetProtocolFromStudy"));

    CPQMChildstudy* l_childstudy = (CPQMChildstudy*)f_data;
    //+MEITEC/2010Sep21/Modified/REDMINE-751
    CVarFieldHandle l_vf_handle(f_db_handle, false);

    //-MEITEC/2010Sep21/Modified/REDMINE-751
    if (l_childstudy->m_study->CheckImgGr(&l_vf_handle, f_protocol) !=
        E_NO_ERROR) {
        // + SM4 Defect#60 AMIT+Himanshu
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CheckImgGr return E_ERROR"));
        // - SM4 Defect#60 AMIT+Himanshu
        return E_ERROR;
    }

    //Patni-PJS/2010Feb24/Added/DeFT# PROPOSAL_08-Remove EZScan
    if (l_childstudy->m_study->ResetOffset(&l_vf_handle, f_protocol)
        != E_NO_ERROR) {
        return E_ERROR;
    }

    // For Orientation
    //for(dll = PqmGlobal.pqmStudy.prObjectNode;dll != NULL;dll = dll->next)
    //{
    //if((obj = dll->item) == NULL) continue;
    //
    //if(obj->acqStatus == C_PR_STATUS_DONE ||
    //    obj->acqStatus == C_PR_STATUS_CURRENT ||
    //    obj->acqStatus == C_PR_STATUS_WAIT)
    //{
    //    memset(mask,0,sizeof(mask));
    //    status = PqmPrVaSetProtValues
    //        (vfd,prot,mask,
    //        K_PR_PATIENT_ORIENT,&obj->mode.patori,sizeof(VftPatori_t),
    //        NULL);
    //    break;
    //}
    //}
    //+Patni-PJS+DKH/2010Nov5/Modified/REDMINE-927
    //l_childstudy->m_study->ClearFields(&l_vf_handle, f_protocol);
    //l_childstudy->m_study->ResetProtInfo(&l_vf_handle, f_protocol, (void*)FALSE);
    PqmPmMemoryResetProtInfo(&l_vf_handle, f_protocol, (void*)FALSE, f_data);
    //-Patni-PJS+DKH/2010Nov5/Modified/REDMINE-927
    CPqmProtocol* l_protocol = NULL;
    status_t l_status = l_childstudy->m_study->AddProtocol(&l_vf_handle,
                        f_protocol, 0, FALSE, &l_protocol);

    l_childstudy->m_study->CheckAutoLocLicensenSetStatus(l_protocol);

    CMasterSlave* l_master_slave_ptr = l_childstudy->m_study->GetPqm()->GetMasterSlavePtr();

    if (l_master_slave_ptr) {
        l_master_slave_ptr->SetGroupIdForTransferredProtocol(l_protocol);
    }

    if_error(l_status) {

        return E_ERROR;
    }
    return E_NO_ERROR;
}

//****************************Method Header************************************
//Method Name   :GetInitNumber()
//Author        :PATNI/HAR - SS
//Purpose       :Return number for initialize.
//*****************************************************************************
int CPQMStudy::GetInitNumber(
    int tag
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetInitNumber");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetInitNumber"));

    for (int i = 0; i < K_ALL_PR; i++) {
        if (pr_init[i].tag == tag) {
            return i;
        }
    }

    return -1;
}

//***************************Method Header*************************************
//Method Name    :GetShimDefault()
//Author         :PATNI/
//Purpose        :
//*****************************************************************************
BOOL CPQMStudy::GetShimDefault(
    PqmShim_t*  f_shim
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetShimDefault");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetShimDefault"));

    DB_FUNC_ENTER("[pqmPrGetShimDefault]:");

    static char*    grad_key[] = {
        "Gx_Initial_Current [Normalized Value]",
        "Gy_Initial_Current [Normalized Value]",
        "Gz_Initial_Current [Normalized Value]"
    };

    static char*    shim_key[] = {
        "Shim_X_Initial_Current [Normalized Value]",
        "Shim_Y_Initial_Current [Normalized Value]",
        "Shim_Z_Initial_Current [Normalized Value]",
        "Shim_ZX_Initial_Current [Normalized Value]",
        "Shim_XY_Initial_Current [Normalized Value]",
        "Shim_ZY_Initial_Current [Normalized Value]",
        "Shim_Z2_Initial_Current [Normalized Value]",
        "Shim_X2Y2_Initial_Current [Normalized Value]",
    };

    register int    i = 0;  //Patni-MJC/2009Aug17/Modified/cpp test corrections
    static BOOL initialize = FALSE;
    static int  def_shimoffset[PQM_SHIM_LEN];
    static int  def_gradoffset[PQM_GRAD_LEN];

    if (initialize) {
        memcpy(f_shim->shimOffset, def_shimoffset, sizeof(def_shimoffset));
        memcpy(f_shim->gradOffset, def_gradoffset, sizeof(def_gradoffset));
        return E_NO_ERROR;
    }

    //Patni-SS/20091013/Modified/MSA0248-00056
    char* path = getenv("GP_SHIMDT_DIR");

    //+Patni-PJS/2009Sep16/Modified/Internal Code review comment
    if (path == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Failed to get value of GP_SHIMDT_DIR environment variable"));
        return FALSE;
    }

    //-Patni-PJS/2009Sep16/Modified/Internal Code review comment
    char buf[128] = "";
    sprintf(buf, "%s/setupShimming", path);

    FILE*   fp = NULL;

    if ((fp = fopen(buf, "r")) == NULL) {
        DB_PRINTF("", ("Cannot open %s.\n", buf));
        CString str(buf);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str);
        return FALSE;
    }

    char magfile[64] = "";
    fscanf(fp, "%s", magfile);
    fclose(fp);

    sprintf(buf, "%s/magDB/%s", path, magfile);

    if ((fp = fopen(buf, "r")) == NULL) {
        DB_PRINTF("", ("Cannot open %s.\n", buf));
        CString filepath(buf);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Can Not Open File :") + filepath);
        return E_ERROR;
    }

    memset(def_gradoffset, 0, sizeof(def_gradoffset));
    memset(def_shimoffset, 0, sizeof(def_shimoffset));

    char*   pt = NULL;

    while (fgets(buf, sizeof(buf), fp)) {

        if ((pt = strchr(buf, ':')) == NULL) continue;

        *pt++ = '\0';

        for (i = 0; i < PQM_GRAD_LEN; i++) {
            if (!strncmp(grad_key[i], buf, strlen(grad_key[i]))) {
                sscanf(pt, "%d", &(def_gradoffset[i]));
                break;
            }
        }

        for (i = 0; i < PQM_SHIM_LEN; i++) {
            if (!strncmp(shim_key[i], buf, strlen(shim_key[i]))) {
                sscanf(pt, "%d", &(def_shimoffset[i]));
                break;
            }
        }
    }

    fclose(fp);
    initialize = TRUE;
#ifdef DEBUG

    for (i = 0; i < PQM_GRAD_LEN; i++)
        DB_PRINTF("N", ("%s : %d\n", grad_key[i], def_gradoffset[i]));

    for (i = 0; i < PQM_SHIM_LEN; i++)
        DB_PRINTF("N", ("%s : %d\n", shim_key[i], def_shimoffset[i]));

#endif
    memcpy(f_shim->shimOffset, def_shimoffset, sizeof(def_shimoffset));
    memcpy(f_shim->gradOffset, def_gradoffset, sizeof(def_gradoffset));

    return TRUE;
}

//***************************Method Header*************************************
//Method Name    :GetParentIndexFplanning()
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
int CPQMStudy::GetParentIndexFplanning(
    const int protocol
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetParentIndexFplanning");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetParentIndexFplanning"));

    VfPathElem_t loc_images[NUM_6] = {0};

    loc_images[0].subtree_name = SVN_ST_PROTOCOL;
    loc_images[1].subtree_name = SVN_ST_PROCESSED;
    loc_images[NUM_2].subtree_name = SVN_ST_ECHO;
    loc_images[NUM_3].subtree_name = SVN_ST_POSITION;
    loc_images[0].index = protocol;
    loc_images[1].index = 0;
    loc_images[NUM_2].index = 0;
    loc_images[NUM_3].index = 0;
    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    VfFieldSpec_t vf_req[1] = {0};
    memset(vf_req, 0, sizeof(VfFieldSpec_t) * 1);   //Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008

    int parent_index = 0;
    VFF_SET_ARGS(vf_req[0],
                 SVN_PRC_PARENT_INDEX_FOR_PLANNING,
                 SVT_PRC_PARENT_INDEX_FOR_PLANNING,
                 sizeof(VFT_INT32),
                 &parent_index,
                 0);
    int num = 0;
    status_t l_status = m_vfdb_handle->GetFields(loc_images, SVD_ST_PROTOCOL, vf_req, 1, &num);

    if (l_status != E_NO_ERROR) {
        parent_index = 0;
    }

    return  parent_index ;
}

//***************************Method Header*************************************
//Method Name    :GetProbeData()
//Author         :PATNI/MSN
//Purpose        :Gets the Minimum and Maximum Threshhold value from the study file
//*****************************************************************************
status_t CPQMStudy::GetProbeData(flt32_t* f_max, flt32_t*   f_min)const
{
    //+Patni-MSN/2009Mar12/Modified/Optimization
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetProbeData");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetProbeData"));

    FILE*       l_fp = NULL;
    TCHAR       l_tmp_path[MAX_PATH] = {0};
    TCHAR       l_buf[MAX_PATH] = {0};


    if (::GetEnvironmentVariable(_T("GP_TMP_DIR"), l_tmp_path, MAX_PATH) != 0) {
        wsprintf(l_buf, _T("%s%s"), l_tmp_path, PROBING_DATAFILE);

    } else {
        wsprintf(l_buf, _T("%s%s"), _T("/gp/tmp"), PROBING_DATAFILE);
    }

    l_fp = _wfopen(l_buf, _T("r"));
    status_t    l_status = E_ERROR;

    if (l_fp != NULL)   {
        SProbeHeader  l_probehead;

        if (fread(&l_probehead, sizeof(l_probehead), sizeof(l_probehead), l_fp)) {
            *f_max = (float)l_probehead.threshold_max ;
            *f_min = (float)l_probehead.threshold_min ;
            l_status =  E_NO_ERROR ;

        }


        //+Patni-MSN/2009May31/Modified/RMC-Hang Issue
        fclose(l_fp);
        //-Patni-MSN/2009May31/Modified/RMC-Hang Issue
    }

    //-Patni-MSN/2009Mar12/Modified/Optimization
    return l_status ;
}

//****************************Method Header************************************
//Method Name   :GetPasParamObject()
//Author        :PATNI / HAR - SG
//Purpose       :
//*****************************************************************************
// + SM4 Defect#737 AMIT HIMANSHU
void CPQMStudy::GetPasParamObject(
    CPASParamStore& l_object_param,
    int f_protocol_index
)
{
    // - SM4 Defect#737 AMIT HIMANSHU
    //+Patni-HAR/20091103/Modified/IR-84 Code Review
    VfPathElem_t search_node_path[] = {
        { SVN_ST_PROTOCOL,      0 },
        { SVN_ST_SUBPROTOCOL,   0 },
        { SVN_ST_SLICEGROUP,    0 },
        { SVN_ST_SLICE,     0 },
        { SVN_ST_IMAGE,     0 },
    };

    // + Patni-Sribanta/2010June25/Modified/IR-156_2
    char l_bdy_prt[32], l_trans_coil[32], l_rcvr_coil[32], l_temp_seq[32], l_temp_ident[32];
    // - Patni-Sribanta/2010June25/Modified/IR-156_2


    memset(l_bdy_prt,       0, sizeof(char) * 32);
    memset(l_trans_coil,    0, sizeof(char) * 32);
    memset(l_trans_coil,    0, sizeof(char) * 32);
    memset(l_temp_seq,      0, sizeof(char) * 32);

    // + Patni-Sribanta/2010June25/Added/IR-156_2
    memset(l_temp_ident,	0, sizeof(char) * 32);
    // - Patni-Sribanta/2010June25/Added/IR-156_2

    //+Patni-Hemant/2010Apr27/Added/JaFT/IR-137
    char l_lateralitystr[32];
    memset(l_lateralitystr, 0, sizeof(char) * 32);
    //-Patni-Hemant/2010Apr27/Added/JaFT/IR-137

    //+Patni-Sribanta/2010June25/Modified/IR-156_2
    //+Patni-Hemant/2010Feb9/Modified/ACE-Phase#3/CDS Requirement
    //Patni-HAR/2010Apr26/Added/IR-134
    VfFieldSpec_t   vf_req[40];
    //-Patni-Hemant/2010Feb9/Modified/ACE-Phase#3/CDS Requirement
    //-Patni-Sribanta/2010June25/Modified/IR-156_2

    //Patni-HAR/2010Apr26/Added/IR-134
    //+Patni-Sribanta/2010June25/Modified/IR-156_2
    memset(vf_req, 0, sizeof(VfFieldSpec_t) * 40);
    //-Patni-Sribanta/2010June25/Modified/IR-156_2
    int i = 0;

    //+Patni-HAR/2010Jun02/Modified/MVC008075
    VFF_SET_ARGS(vf_req[i], SVN_PLN_SELECTED_CONTRAST_TE, SVT_PLN_SELECTED_CONTRAST_TE,
                 0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);
    //-Patni-HAR/2010Jun02/Modified/MVC008075
    i++;
    f32vec2_t  fov;
    VFF_SET_ARGS(vf_req[i], SVN_PLN_FOV, SVT_PLN_FOV,
                 sizeof(f32vec2_t), &fov, 0);
    i++;
    f32vec2_t  mtx;
    VFF_SET_ARGS(vf_req[i], SVN_PLN_DISPLAYED_MATRIX, SVT_PLN_DISPLAYED_MATRIX,
                 sizeof(f32vec2_t), &mtx, 0);
    i++;
    int l_flip_angle = 0;
    VFF_SET_ARGS(vf_req[i], SVN_PLN_FLIP_ANGLE, SVT_PLN_FLIP_ANGLE,
                 sizeof(int), &l_flip_angle, 0);
    i++;
    int l_flop_angle = 0;
    VFF_SET_ARGS(vf_req[i], SVN_PLN_FLOP_ANGLE, SVT_PLN_FLOP_ANGLE,
                 sizeof(int), &l_flop_angle, 0);
    i++;
    int l_wrap = 0;
    VFF_SET_ARGS(vf_req[i], SVN_PLN_WRAPAROUND_REDUCT, SVT_PLN_WRAPAROUND_REDUCT,
                 sizeof(int), &l_wrap, 0);
    i++;
    int l_img_mode = 0;
    VFF_SET_ARGS(vf_req[i], SVN_PLN_IMAGING_MODE, SVT_PLN_IMAGING_MODE,
                 sizeof(int), &l_img_mode, 0);
    i++;
    int l_rflt = 0;
    VFF_SET_ARGS(vf_req[i], SVN_PRC_REFINE_FILTER, SVT_PRC_REFINE_FILTER,
                 sizeof(int), &l_rflt, 0);
    i++;
    int l_space_flt = 0;
    VFF_SET_ARGS(vf_req[i], SVN_PRC_K_SPACE_FILTER, SVT_PRC_K_SPACE_FILTER,
                 sizeof(int), &l_space_flt, 0);
    i++;
    int l_coverages = 0;
    VFF_SET_ARGS(vf_req[i], SVN_PLN_COVERAGES, SVT_PLN_COVERAGES,
                 sizeof(int), &l_coverages, 0);
    i++;
    int l_total_slices = 0;
    VFF_SET_ARGS(vf_req[i], SVN_PLN_TOTAL_SLICES, SVT_PLN_TOTAL_SLICES,
                 sizeof(int), &l_total_slices, 0);
    i++;
    int l_transmit_coil = 0;
    VFF_SET_ARGS(vf_req[i], SVN_PLN_TRANSMIT_COIL, SVT_PLN_TRANSMIT_COIL,
                 sizeof(int), &l_transmit_coil, 0);
    i++;
    int l_receiver_coil = 0;
    VFF_SET_ARGS(vf_req[i], SVN_PLN_RECVR_COIL, SVT_PLN_RECVR_COIL,
                 sizeof(int), &l_receiver_coil, 0);
    i++;

    int  l_pln_status = 0;
    VFF_SET_ARGS(vf_req[i], SVN_PLN_STATUS, SVT_PLN_STATUS,
                 sizeof(int), &l_pln_status, 0);
    i++;
    int l_dynamic_scan = 0; //Patni-KSS/2011March10/Modified/TMSC-Review-Comments
    VFF_SET_ARGS(vf_req[i], SVN_PLN_DYNAMIC_SCAN, SVT_PLN_DYNAMIC_SCAN,
                 sizeof(int), &l_dynamic_scan, 0);
    i++;
    int l_motion_supression = 0;
    VFF_SET_ARGS(vf_req[i], SVN_PLN_MOTION_SUPPRESSION, SVT_PLN_MOTION_SUPPRESSION,
                 sizeof(int), &l_motion_supression, 0);
    i++;
    int l_app_option = 0;
    VFF_SET_ARGS(vf_req[i], SVN_PLN_APP_OPTION, SVT_PLN_APP_OPTION,
                 sizeof(int), &l_app_option, 0);
    i++;
    int l_ph_drctn = 0;
    VFF_SET_ARGS(vf_req[i], SVN_PLN_PHASE_DIRECTION, SVT_PLN_PHASE_DIRECTION,
                 sizeof(int), &l_ph_drctn, 0);
    i++;
    int l_gm = 0;
    VFF_SET_ARGS(vf_req[i], SVN_PLN_GATING_METHOD, SVT_PLN_GATING_METHOD,
                 sizeof(int), &l_gm, 0);
    i++;
    int l_recon_nr = 0;
    VFF_SET_ARGS(vf_req[i], SVN_PRC_RECON_NR_FLAG, SVT_PRC_RECON_NR_FLAG,
                 sizeof(int), &l_recon_nr, 0);
    i++;
    int l_sors = 0;
    VFF_SET_ARGS(vf_req[i], SVN_PLN_SORS_PULSE, SVT_PLN_SORS_PULSE,
                 sizeof(int), &l_sors, 0);
    i++;
    int l_mtcsat_pulse = 0;
    VFF_SET_ARGS(vf_req[i], SVN_PLN_MTCSAT_PULSE, SVT_PLN_MTCSAT_PULSE,
                 sizeof(int), &l_mtcsat_pulse, 0);
    i++;
    int l_auto_post_proc = 0; //Patni-KSS/2011March10/Modified/TMSC-Review-Comments
    VFF_SET_ARGS(vf_req[i], SVN_PLN_AUTO_POSTPROC_FLG, SVT_PLN_AUTO_POSTPROC_FLG,
                 sizeof(int), &l_auto_post_proc, 0);
    i++;
    int l_slice_orientation = 0;
    VFF_SET_ARGS(vf_req[i], SVN_PLN_SLICE_ORIENTATION, SVT_PLN_SLICE_ORIENTATION,
                 sizeof(int), &l_slice_orientation, 0);
    i++;
    float l_nex = 0.0f;
    VFF_SET_ARGS(vf_req[i], SVN_PLN_NEX, SVT_PLN_NEX,
                 sizeof(float), &l_nex, 0);
    i++;
    float l_contrast_tr = 0.0f;
    VFF_SET_ARGS(vf_req[i], SVN_PLN_CONTRAST_TR, SVT_PLN_CONTRAST_TR,
                 sizeof(float), &l_contrast_tr, 0);
    i++;
    float l_sel_slice_thk  = 0.0f;
    VFF_SET_ARGS(vf_req[i], SVN_PLN_SELECTED_SLICE_THK, SVT_PLN_SELECTED_SLICE_THK,
                 sizeof(float), &l_sel_slice_thk, 0);
    i++;
    float l_sel_slice_gap = 0.0f;
    VFF_SET_ARGS(vf_req[i], SVN_PLN_SELECTED_SLICE_GAP, SVT_PLN_SELECTED_SLICE_GAP,
                 sizeof(float), &l_sel_slice_gap, 0);
    i++;
    float l_seq_bandwidth = 0.0f;
    VFF_SET_ARGS(vf_req[i], SVN_SEQ_BAND_WIDTH, SVT_SEQ_BAND_WIDTH,
                 sizeof(float), &l_seq_bandwidth, 0);
    i++;
    float l_couch = 0.0f;
    VFF_SET_ARGS(vf_req[i], SVN_EXE_COUCH, SVT_EXE_COUCH,
                 sizeof(float), &l_couch, 0);
    i++;
    int l_fatsat_pulse = 0;
    VFF_SET_ARGS(vf_req[i], SVN_PLN_FATSAT_PULSE, SVT_PLN_FATSAT_PULSE,
                 sizeof(int), &l_fatsat_pulse, 0);
    i++;

    VFF_SET_ARGS(vf_req[i], SVN_PLN_TEMPLATE_SEQ, SVT_PLN_TEMPLATE_SEQ,
                 sizeof(char) * 32, &l_temp_seq[0], VFO_VARIABLE_LEN);
    i++;

    VFF_SET_ARGS(vf_req[i], SVN_PLN_BODY_PART, SVT_PLN_BODY_PART,
                 sizeof(char) * 32, &l_bdy_prt[0], VFO_VARIABLE_LEN);
    i++;

    //+Patni-Hemant/2010May27/Commented/MaFT/TMSC-REDMINE-MVC007915
    //	//+Patni-Hemant/2010Apr27/Added/JaFT/IR-137
    //    VFF_SET_ARGS(vf_req[i], SVN_PLN_LATERALITY, SVT_PLN_LATERALITY,
    //                 sizeof(char) * 32, &l_lateralitystr[0], VFO_VARIABLE_LEN);
    //    i++;
    //	//-Patni-Hemant/2010Apr27/Added/JaFT/IR-137
    //-Patni-Hemant/2010May27/Commented/MaFT/TMSC-REDMINE-MVC007915

    VFF_SET_ARGS(vf_req[i], SVN_PRS_TRANSMIT_COIL, SVT_PRS_TRANSMIT_COIL,
                 sizeof(char) * 32, &l_trans_coil[0], VFO_VARIABLE_LEN);
    i++;

    VFF_SET_ARGS(vf_req[i], SVN_PRS_RECVR_COIL, SVT_PRS_RECVR_COIL,
                 sizeof(char) * 32, &l_rcvr_coil[0], VFO_VARIABLE_LEN);
    i++;

    //+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
    int l_as_compass_flag = 0 ;
    VFF_SET_ARGS(vf_req[i], SVN_PLN_CDS_FLAG, SVT_PLN_CDS_FLAG,
                 sizeof(int), &l_as_compass_flag, 0);
    i++;
    //-Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
    int l_amb_enable_flag = 0 ;
    VFF_SET_ARGS(vf_req[i], SVN_PLN_AMB_ENABLE_FLG, SVT_PLN_AMB_ENABLE_FLG,
                 sizeof(int), &l_amb_enable_flag, 0);
    i++;


    int l_amb_mode_flag = 0 ;
    VFF_SET_ARGS(vf_req[i], SVN_PLN_AMB_MODE_FLG, SVT_PLN_AMB_MODE_FLG,
                 sizeof(int), &l_amb_mode_flag, 0);
    i++;

    //+Patni-HAR/2010Apr26/Added/IR-134
    int l_mrs_edddy_axis = 0;
    VFF_SET_ARGS(vf_req[i], SVN_SEQ_MRS_EDDY_AXIS, SVT_SEQ_MRS_EDDY_AXIS,
                 sizeof(int), &l_mrs_edddy_axis, 0);
    i++;
    //-Patni-HAR/2010Apr26/Added/IR-134

    // +Patni-Sribanta/2010June25/Modified/IR-156_2

    VFF_SET_ARGS(vf_req[i], SVN_PLN_SEQ_IDENT, SVT_PLN_SEQ_IDENT,
                 sizeof(char) * 32, &l_temp_ident[0], VFO_VARIABLE_LEN);
    i++;

    // - Patni-Sribanta/2010June25/Modified/IR-156_2
    int l_num_done = -1;
    search_node_path[SVN_ST_PROTOCOL - 1].index = f_protocol_index;

    status_t l_status = m_vfdb_handle->GetFields(search_node_path,
                        SVD_ST_PROTOCOL, vf_req, i, &l_num_done);

    CString l_temp_str(_T(""));

    if (fov.x != fov.y) {
        l_temp_str.Format(_T("%.3f,%.3f"), fov.x, fov.y);

    } else {
        l_temp_str.Format(_T("%.3f"), fov.x);
    }

    l_object_param.SetVector(_T("FOV"), l_temp_str);

    if (mtx.x != mtx.y) {
        l_temp_str.Format(_T("%f,%f"), (float)mtx.x, (float)mtx.y);

    } else {
        l_temp_str.Format(_T("%f"), (float)mtx.x);
    }

    l_object_param.SetVector(_T("MTX"), l_temp_str);
    //+Patni-PJS/2010June04/Modified/JaFT/MVC008075
    //+Patni-HAR/2010Jun02/Modified/MVC008075
    flt32_t*    te = (flt32_t*)vf_req[0].buffer;

    if (vf_req[0].size == 4) {
        l_temp_str.Format(_T("%f"), (te[0]));

    } else {
        l_temp_str.Format(_T("%f,%f"), (te[0]), (te[1]));
    }

    l_object_param.SetVector(_T("TE"), l_temp_str);
    //-Patni-HAR/2010Jun02/Modified/MVC008075
    //-Patni-PJS/2010June04/Modified/JaFT/MVC008075

    l_object_param.SetInt(_T("FLIP"), (l_flip_angle == -1 ? 0 : l_flip_angle));
    l_object_param.SetInt(_T("FLOP"), (l_flop_angle == -1 ? 0 : l_flop_angle));
    l_object_param.SetInt(_T("WRAP"), l_wrap);
    l_object_param.SetInt(_T("IMGMODE"), l_img_mode);
    l_object_param.SetInt(_T("RFLT"), l_rflt);
    l_object_param.SetInt(_T("COV"), l_coverages);
    l_object_param.SetInt(_T("KFLT"), l_space_flt);
    l_object_param.SetInt(_T("NS"), l_total_slices);
    l_object_param.SetInt(_T("TC"), l_transmit_coil);
    l_object_param.SetInt(_T("RC"), l_receiver_coil);
    l_object_param.SetInt(_T("PLANSTATUS"), l_pln_status);
    l_object_param.SetInt(_T("DYN"), l_dynamic_scan);
    l_object_param.SetInt(_T("MOTION"), l_motion_supression);
    l_object_param.SetInt(_T("APPOPT"), l_app_option);
    l_object_param.SetInt(_T("PD"), l_ph_drctn);
    l_object_param.SetInt(_T("NRFLG"), l_recon_nr);
    l_object_param.SetInt(_T("SORS"), l_sors);
    l_object_param.SetInt(_T("MTC"), l_mtcsat_pulse);
    l_object_param.SetInt(_T("APOST"), l_auto_post_proc);
    l_object_param.SetInt(_T("PLN"), l_slice_orientation);
    //+Patni-Ajay/Added/09Aug2010/TMSC-REDMINE-560
    l_object_param.SetInt(_T("GATE"), l_gm);

    l_object_param.SetFloat(_T("NAQ"), l_nex);
    l_object_param.SetFloat(_T("TR"), l_contrast_tr);
    //Patni-HAR/2010May31/Commented/MVC008075
    //l_object_param.SetVector(_T("TE"), l_contrast_te);
    l_object_param.SetFloat(_T("THK"), l_sel_slice_thk);
    l_object_param.SetFloat(_T("GAP"), l_sel_slice_gap);
    l_object_param.SetFloat(_T("BW"), l_seq_bandwidth);
    l_object_param.SetFloat(_T("COUCH"), l_couch);
    //+MIETEC/2010Jun21/Modified/MVC008389/Change variable tyue from float to integer.
    //l_object_param.SetFloat(_T("FAT"), l_fatsat_pulse);
    l_object_param.SetInt(_T("FAT"), l_fatsat_pulse);
    //-MIETEC/2010Jun21/Modified/MVC008389/Change variable tyue from float to integer.

    //+ Tanoue/MVC005849
    UTIL::Mbs2Wcs(&l_temp_str, l_bdy_prt);
    l_object_param.SetString(_T("BODYPART"), l_temp_str);
    //- Tanoue/MVC005849

    //+Patni-Hemant/2010May27/Modified/MaFT/TMSC-REDMINE-MVC007915
    //	//+Patni-Hemant/2010Apr27/Added/JaFT/IR-137
    //	UTIL::Mbs2Wcs(&l_temp_str, l_lateralitystr);
    //	int l_laterality_val = -1;
    //	l_laterality_val = CPqmUtilities::GetvalForLateralityStr(l_temp_str);
    //	l_object_param.SetInt(_T("LATERALITY"), l_laterality_val);
    //	//-Patni-Hemant/2010Apr27/Added/JaFT/IR-137
    int l_laterality_val = -1;
    GetLaterality(f_protocol_index, l_laterality_val);
    l_object_param.SetInt(_T("LATERALITY"), l_laterality_val);
    //-Patni-Hemant/2010May27/Modified/MaFT/TMSC-REDMINE-MVC007915


    l_object_param.SetString(_T("TRANSMIT"), l_trans_coil);
    l_object_param.SetString(_T("RECEIVER"), l_rcvr_coil);
    //+ Tanoue/MVC005849
    UTIL::Mbs2Wcs(&l_temp_str, l_temp_seq);
    l_object_param.SetString(_T("SEQ"), l_temp_str);
    //- Tanoue/MVC005849

    // + Patni-Sribanta/2010June25/Added/IR-156_2

    UTIL::Mbs2Wcs(&l_temp_str, l_temp_ident);
    l_object_param.SetString(_T("IDENT"), l_temp_str);

    // - Patni-Sribanta/2010June25/Added/IR-156_2

    //+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
    l_object_param.SetInt(_T("AS-COMPASS"), l_as_compass_flag);
    //-Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
    l_object_param.SetInt(_T("AMB-FLAG"), l_amb_enable_flag);
    l_object_param.SetInt(_T("AMB-MODE-FLAG"), l_amb_mode_flag);
    //Patni-HAR/2010Apr26/Added/IR-134
    l_object_param.SetInt(_T("EDDY"), l_mrs_edddy_axis);
    VfFieldSpec_t   l_vf_req[1] = {0};
    i = 0;
    long l_temp_time = 0;
    VFF_SET_ARGS(l_vf_req[i], SVN_EXE_SCAN_START, SVT_EXE_SCAN_START,
                 sizeof(int), &l_temp_time, 0);
    i++;

    l_num_done = -1;
    search_node_path[SVN_ST_PROTOCOL - 1].index = f_protocol_index;

    l_status = m_vfdb_handle->GetFields(search_node_path,
                                        SVD_ST_SUBPROTOCOL, l_vf_req, i, &l_num_done);
    char time[32] = {0};

    if (i == l_num_done) {

        struct tm* tmtime = localtime((&l_temp_time));

        if (tmtime) {
            sprintf(time, "%04d-%02d-%02d %02d:%02d:%02d", ((tmtime->tm_year) + 1900),
                    ((tmtime->tm_mon) + 1),
                    (tmtime->tm_mday), (tmtime->tm_hour), (tmtime->tm_min), (tmtime->tm_sec));
        }

        l_object_param.SetString(_T("SCANSTART"), time);

    } else {
        l_object_param.SetString(_T("SCANSTART"), time);
    }

    //+Patni-PJS/2010June04/Added/JaFT/MVC008075
    if ((vf_req[0].buffer != NULL) && (vf_req[0].size != 0)) {
        FREE_PTR(vf_req[0].buffer);
    }

    //-Patni-PJS/2010June04/Added/JaFT/MVC008075

    //-Patni-NP/2009Sep21/Added/MVC004865
    //-Patni-HAR/2009July27/Added/MVC#3827
    // - SM4 Defect#737 AMIT + HIMANSHU -> Added f_protocol_index
    //-Patni-HAR/20091103/Modified/IR-84 Code Review
}

//****************************Method Header*************************************
//Method Name   :GetINTValues()
//Author        :PATNI/HAR
//Purpose       :
//******************************************************************************
// + SM4 Defect#737 AMIT + HIMANSHU -> Added f_protocol_index
int CPQMStudy::GetINTValues(
    const unsigned long f_vftype,
    const unsigned long f_vfname,
    const int f_depth,
    const int f_protocol_index
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetINTValues");
    // + SM4 Defect#737 AMIT + HIMANSHU
    //Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    VfPathElem_t search_node_path[] = {
        { SVN_ST_PROTOCOL,      0 },
        { SVN_ST_SUBPROTOCOL,   0 },
        { SVN_ST_SLICEGROUP,    0 },
        { SVN_ST_SLICE,     0 },
        { SVN_ST_IMAGE,     0 },
    };

    // SM4 Defect#737 AMIT + HIMANSHU
    search_node_path[SVN_ST_PROTOCOL - 1].index = f_protocol_index;
    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    VfFieldSpec_t   vf_req[1] = {0};
    memset(vf_req, 0, sizeof(VfFieldSpec_t));
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    int l_return = -1;
    VFF_SET_ARGS(vf_req[0], f_vftype, f_vfname, sizeof(int),
                 &l_return, 0);

    int l_num_done = -1;
    status_t l_status = m_vfdb_handle->GetFields(search_node_path, f_depth, vf_req,
                        1, &l_num_done);
    CString l_strLog(_T(""));
    l_strLog.Format(L"Status of m_vfdb_handle->GetFields() is:- %d", l_status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_strLog);
    return l_return;
}
//+Patni-NP/2009Sep21/Added/MVC004865
//****************************Method Header*************************************
//Method Name   :GetLongValues()
//Author        :PATNI/NP
//Purpose       :
//******************************************************************************
int CPQMStudy::GetLONGValues(
    const unsigned long f_vftype,
    const unsigned long f_vfname,
    const int f_depth,
    const int f_protocol_index
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetLONGValues");
    VfPathElem_t    search_node_path[] = {
        { SVN_ST_PROTOCOL,      0 },
        { SVN_ST_SUBPROTOCOL,   0 },
        { SVN_ST_SLICEGROUP,    0 },
        { SVN_ST_SLICE,     0 },
        { SVN_ST_IMAGE,     0 },
    };
    search_node_path[SVN_ST_PROTOCOL - 1].index = f_protocol_index;
    VfFieldSpec_t   vf_req[1] = {0};
    memset(vf_req, 0, sizeof(VfFieldSpec_t));
    int l_return = -1;
    VFF_SET_ARGS(vf_req[0], f_vftype, f_vfname, sizeof(int),
                 &l_return, 0);
    int l_num_done = -1;
    status_t l_status = m_vfdb_handle->GetFields(search_node_path, f_depth, vf_req,
                        1, &l_num_done);
    CString l_strLog(_T(""));
    l_strLog.Format(L"Status of m_vfdb_handle->GetFields() is:- %d", l_status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_strLog);
    return l_return;
} //-Patni-NP/2009Sep21/Added/MVC004865

//****************************Method Header*************************************
//Method Name   :GetFLOATValues()
//Author        :PATNI/HAR
//Purpose       :
//******************************************************************************
// + SM4 Defect#737 AMIT + HIMANSHU -> Added f_protocol_index
float CPQMStudy::GetFLOATValues(
    const unsigned long f_vftype,
    const unsigned long f_vfname,
    const int f_depth,
    const int f_protocol_index
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetFLOATValues");
    // + SM4 Defect#737 AMIT + HIMANSHU
    VfPathElem_t search_node_path[] = {
        { SVN_ST_PROTOCOL,      0 },
        { SVN_ST_SUBPROTOCOL,   0 },
        { SVN_ST_SLICEGROUP,    0 },
        { SVN_ST_SLICE,     0 },
        { SVN_ST_IMAGE,     0 },
    };

    // SM4 Defect#737 AMIT + HIMANSHU
    search_node_path[SVN_ST_PROTOCOL - 1].index = f_protocol_index;
    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    VfFieldSpec_t   vf_req[1];
    memset(vf_req, 0, sizeof(VfFieldSpec_t));
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    float l_return = 0.0f;
    VFF_SET_ARGS(vf_req[0], f_vftype, f_vfname, sizeof(float), &l_return, 0);
    int l_num_done = -1;
    status_t l_status = m_vfdb_handle->GetFields(search_node_path, f_depth,
                        vf_req, 1, &l_num_done);
    CString l_strLog(_T(""));
    l_strLog.Format(L"Status of m_vfdb_handle->GetFields() is:- %d", l_status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_strLog);
    return l_return;
}

//****************************Method Header*************************************
//Method Name   :GetSTRValues()
//Author        :PATNI/HAR
//Purpose       :
//******************************************************************************
// + SM4 Defect#737 AMIT + HIMANSHU -> Added f_protocol_index
CString CPQMStudy::GetSTRValues(
    const unsigned long f_vftype,
    const unsigned long f_vfname,
    const int f_depth,
    const int f_protocol_index
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetSTRValues");
    // + SM4 Defect#737 AMIT + HIMANSHU -> Added f_protocol_index
    VfPathElem_t search_node_path[] = {
        { SVN_ST_PROTOCOL,      0 },
        { SVN_ST_SUBPROTOCOL,   0 },
        { SVN_ST_SLICEGROUP,    0 },
        { SVN_ST_SLICE,     0 },
        { SVN_ST_IMAGE,     0 },
    };
    // SM4 Defect#737 AMIT + HIMANSHU
    search_node_path[SVN_ST_PROTOCOL - 1].index = f_protocol_index;
    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    VfFieldSpec_t   vf_req[1] = {0};
    memset(vf_req, 0, sizeof(VfFieldSpec_t));
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    char buff[32] = "";
    memset(buff, 0, 32);
    VFF_SET_ARGS(vf_req[0], f_vftype, f_vfname, 32, &buff[0], VFO_VARIABLE_LEN);

    //+Patni-Manish/2009Sept02/VFLOG/Added
    int l_num_done = -1;
    status_t l_status = m_vfdb_handle->GetFields(search_node_path, f_depth, vf_req,
                        1, &l_num_done);
    CString l_strLog(_T(""));
    l_strLog.Format(L"Status of m_vfdb_handle->GetFields() is:- %d", l_status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_strLog);

    CString l_retstr((CHAR*)vf_req[0].buffer);
    return l_retstr;
}

//+Patni-HAR/2009Mar06/Modify/PSP1#85
//****************************Method Header*************************************
//Method Name   :GetVectorValues()
//Author        :PATNI/HAR
//Purpose       :
//******************************************************************************
CString CPQMStudy::GetVectorValues(
    const unsigned long f_vfname,
    const unsigned long f_vftype,
    const int f_depth,
    const int f_protocol_index
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetVectorValues");
    f32vec2_t l_return;
    memset(&l_return, 0, sizeof(f32vec2_t));
    VfPathElem_t search_node_path[] = {
        { SVN_ST_PROTOCOL,      0 },
        { SVN_ST_SUBPROTOCOL,   0 },
        { SVN_ST_SLICEGROUP,    0 },
        { SVN_ST_SLICE,     0 },
        { SVN_ST_IMAGE,     0 },
    };

    search_node_path[SVN_ST_PROTOCOL - 1].index = f_protocol_index;
    VfFieldSpec_t   vf_req[1] = {0};
    memset(vf_req, 0, sizeof(VfFieldSpec_t));
    VFF_SET_ARGS(vf_req[0], f_vfname, f_vftype, sizeof(f32vec2_t), &l_return, 0);
    int l_num_done = -1;
    status_t l_status = m_vfdb_handle->GetFields(search_node_path, f_depth, vf_req,
                        1, &l_num_done);

    CString l_strLog(_T(""));
    l_strLog.Format(L"Status of m_vfdb_handle->GetFields() is:- %d", l_status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_strLog);

    CString l_retstr(_T(""));

    if (f_vfname == SVN_PLN_DISPLAYED_MATRIX) {

        if (l_return.x != l_return.y) {
            l_retstr.Format(_T("%f,%f"), (float)l_return.x, (float)l_return.y);

        } else {
            l_retstr.Format(_T("%f"), (float)l_return.x);
        }

    } else {

        l_retstr.Format(_T("%f"), (float)l_return.x);
    }

    return l_retstr;
}

//+Patni-Sribanta/2010Oct25/Added/IR-170
//****************************Method Header************************************
//Method Name   :GetLicenseStringForPath()
//Author        :PATNI/Sribanta
//Purpose       :
//*****************************************************************************
bool CPQMStudy::GetLicenseStringForPath(
    CString& f_pas_path
) const
{

    CString l_path = (CString)GP_PAS_PATH;
    //+Patni-Sribanta/2011Mar09/Modified/IR-170_Part3
    f_pas_path.Delete(0, (l_path.GetLength() + f_pas_path.Find(l_path)));
    const int l_pos = f_pas_path.Find(_T("\\"));
    //-Patni-Sribanta/2011Mar09/Modified/IR-170_Part3
    f_pas_path = f_pas_path.Left(l_pos);
    f_pas_path = l_path + f_pas_path;

    return true;
}
//-Patni-Sribanta/2010Oct25/Added/IR-170

//+Patni-RSG/Added/2009Nov25/MSA0248-0033
//****************************Method Header************************************
//Method Name   :GetActiveNodeBranches ()
//Author        :PATNI/KNK
//Purpose       :Gives the active branches,total branches in a node.    //CR-23
//*****************************************************************************
bitfield_t* CPQMStudy::GetActiveNodeBranches(
    VfPathElem_t* path,
    int* tot_node_branch,
    unsigned int& node_name
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetActiveNodeBranches");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMStudy::GetActiveNodeBranches"));

    if (node_name == SVD_ST_STUDY) {
        return NULL;
    }

    int alloc_size = BITFLD_SIZE(256) * sizeof(bitfield_t);

    bitfield_t* active_mask = new bitfield_t[alloc_size];
    BITFLD_INIT(active_mask, alloc_size, 0);

    int l_maskbits = BITFLD_SIZE(alloc_size) * BITFLD_WORD_NBITS;
    int total_node_count = l_maskbits;
    *tot_node_branch = 0;
    int active_node_count = 0;

    int status = m_vfdb_handle->ScanSubtree(path, SVD_ST_PROTOCOL, node_name, active_mask,
                                            &total_node_count, &active_node_count);

    if ((total_node_count > l_maskbits)) {
        int temp = total_node_count / l_maskbits;

        if (temp && (total_node_count % l_maskbits)) {
            temp++;
        }

        //size is greater than allocated so deleted old one.
        DEL_PTR_ARY(active_mask);

        try {
            active_mask = new bitfield_t[(l_maskbits * temp)]; // reallocation with new size

        } catch (std::bad_alloc bad_allocation) {
            active_mask = NULL;
        }


        if (active_mask) {
            BITFLD_INIT(active_mask, (l_maskbits * temp), 0);

            status = m_vfdb_handle->ScanSubtree(path, (SVD_ST_PROTOCOL - 1), node_name,
                                                active_mask, &total_node_count, &active_node_count);
        }
    }

    if (status != E_NO_ERROR && status != 11) {
        if (status != E_BAD_PATH) {
            return NULL;
        }
    }

    *tot_node_branch = total_node_count;


    return(active_mask);
}




//****************************Method Header************************************
//Method Name   :MaintainBaseTempToCFStudy()
//Author        :PATNI / Pavan
//Purpose       :To copy base temperature from previous protocol to current protocol
//*****************************************************************************
int CPQMStudy::MaintainBaseTempToCFStudy(
    const CString& f_file,
    const CPqmProtocol* f_current_protocol
) const
{

    LPCTSTR FUNC_NAME = _T("CPQMStudy::MaintainBaseTempToCFStudy");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL == f_current_protocol) {
        return FALSE;
    }

    CVarFieldHandle l_maintain_vf_handle(f_file);

    if (l_maintain_vf_handle.VuiOpen() != E_NO_ERROR) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, f_file);
        return E_ERROR;
    }

    const int l_total_fields = 2;

    VfPathElem_t path_elem[l_total_fields] = {0};

    path_elem[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;

    path_elem[SVD_ST_PROTOCOL - 1].index = f_current_protocol->GetProtocol();

    int i = 0;

    float l_base_temp[MAX_TEMP] = {0.0};

    VfFieldSpec_t   fieldSpec[l_total_fields] = {0};

    VFF_SET_ARGS(fieldSpec[i], SVN_EXE_G_COIL_BASE_OLP_VALUE, SVT_EXE_G_COIL_BASE_OLP_VALUE,
                 sizeof(float)*MAX_TEMP, &l_base_temp, 0);

    i++;

    int numDone;

    BOOL status = m_vfdb_handle->GetFields(path_elem, SVD_ST_PROTOCOL, fieldSpec, i, &numDone);

    if (status != E_NO_ERROR) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Error in reading base temperature"));
        l_maintain_vf_handle.VuiClose();
        return FALSE;
    }

    path_elem[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    path_elem[SVD_ST_PROTOCOL - 1].index = 0;

    numDone = 0;
    i = 0;
    VfFieldSpec_t   fieldSpec1[l_total_fields] = {0};
    VFF_SET_ARGS(fieldSpec1[i], SVN_EXE_G_COIL_BASE_OLP_VALUE, SVT_EXE_G_COIL_BASE_OLP_VALUE,
                 fieldSpec[i].size, fieldSpec[i].buffer, VFO_ALLOCATE);
    i++;

    time_t l_tm = 0;
    status = l_maintain_vf_handle.PutFields(path_elem, SVD_ST_PROTOCOL, fieldSpec1,
                                            i, &numDone, &l_tm);

    l_maintain_vf_handle.VuiClose();

    if (status != E_NO_ERROR) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Error in writting SVN_EXE_G_COIL_BASE_OLP_VALUE"));
        return FALSE;
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exit Copying base temperature from previous protocol"));
    return TRUE;
}


//****************************Method Header************************************
//Method Name   :GetLastProtocol()
//Author        :PATNI/HAR
//Purpose       :This method returns the last protocol in the study
//*****************************************************************************
CPqmProtocol* CPQMStudy::GetLastProtocol(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetLastProtocol");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetLastProtocol"));

    //Patni-DKH/2011Mar04/Added/TMSC-REDMINE-1486
    CScopedLock l_scoped_lock(&m_protocol_vec_mutex);

    CPqmProtocol*   result_acqSeries = NULL;
    //Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    protocolvec::const_iterator v1_Iter = NULL;

    for (v1_Iter = m_protocol_vec.begin(); v1_Iter != m_protocol_vec.end(); v1_Iter++) {
        result_acqSeries = *v1_Iter;
    }

    return result_acqSeries;
}

//****************************Method Header************************************
//Method Name   :Get1stScanDone()
//Author        :PATNI/HAR
//Purpose       :This method returns the Series object which was scanned first
//*****************************************************************************
CPqmProtocol* CPQMStudy::Get1stScanDone(
)const
{

    LPCTSTR FUNC_NAME = _T("CPQMStudy::Get1stScanDone");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of Get1stScanDone"));

    //Patni-DKH/2011Mar04/Added/TMSC-REDMINE-1486
    CScopedLock l_scoped_lock(&m_protocol_vec_mutex);

    CPqmProtocol*   result_acqSeries = NULL;
    //Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    protocolvec::const_iterator v1_Iter = NULL;

    for (v1_Iter = m_protocol_vec.begin(); v1_Iter != m_protocol_vec.end(); v1_Iter++) {
        result_acqSeries = *v1_Iter;

        //+Patni-SS/MODIFIED/2009-Jun-04/CRASH TMSC reported
        if (result_acqSeries != NULL && result_acqSeries->GetAcqStatus() == C_PR_STATUS_DONE) {
            break;

        } else {
            continue;
        }
    }

    return result_acqSeries;
}

//***************************Method Header*************************************
//Method Name    :GetProtocolCountFromDB()
//Author         :PATNI/MRP
//Purpose        :
//**************************************************************************
int CPQMStudy::GetProtocolCountFromDB(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetProtocolCountFromDB ");

    if (!m_vfdb_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_vfdb_handle is NULL"));
        return -1;
    }

    int branches = MAX_NODES;
    BITFLD_DECL(mask, MAX_NODES);
    memset(mask, 0, sizeof(mask));

    m_vfdb_handle->ReOpen();
    //-Patni-AMT/2010Apr08/Commented/JaFT# IR-86 Study issue
    ClibVfToUiAdapter vuiAdapter(m_vfdb_handle);
    vuiAdapter.VuiGetAllProt(mask, &branches);

    return branches;
}

//****************************Method Header************************************
//Method Name   : GetSaturnCoil()
//Author        : PATNI\HAR
//Purpose       :
//*****************************************************************************
int CPQMStudy::GetSaturnCoil(
    CVarFieldHandle* const f_vf_handle,
    const int f_protocol,
    PqmSaturnCoil_t* f_saturn_coil
)const
{
    VfPathElem_t    l_pelm[2] = {0};//Patni-MJC/2009Aug17/Modified/cpp test corrections
    VfFieldSpec_t   l_vf_required[8] = {0};//Patni-MJC/2009Aug17/Modified/cpp test corrections

    //DB_FUNC_ENTER("[PqmPrGetSaturnCoil]:");

    memset(&f_saturn_coil->CoilName[0], 0, sizeof(f_saturn_coil->CoilName));
    memset(&f_saturn_coil->RcvCoil, 0, sizeof(f_saturn_coil->RcvCoil));
    memset(&f_saturn_coil->Signal, 0, sizeof(f_saturn_coil->Signal));
    memset(&f_saturn_coil->CoilList, 0, sizeof(f_saturn_coil->CoilList));
    memset(&f_saturn_coil->CoilListcoordinate, 0, sizeof(f_saturn_coil->CoilListcoordinate));
    memset(&f_saturn_coil->SignalDegeneracy, 0, sizeof(f_saturn_coil->SignalDegeneracy));
    memset(&f_saturn_coil->DegeneracyFlg, 0, sizeof(f_saturn_coil->DegeneracyFlg));

    //+Patni-HAR/2010Feb10/Commented/CDS Requirement/Compatible to V9.50
    //if (f_saturn_coil->numSection == 0) {
    //    return l_status;
    //}
    //-Patni-HAR/2010Feb10/Commented/CDS Requirement/Compatible to V9.50

    memset(l_pelm, 0, sizeof(VfPathElem_t) * 2);    //Patni-MJC/2009Aug17/Modified/cpp test corrections
    l_pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_pelm[SVD_ST_PROTOCOL - 1].index = f_protocol;
    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(l_vf_required, 0, sizeof(VfFieldSpec_t) * 8);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    int n = 0;
    int l_size = sizeof(f_saturn_coil->CoilName);

    //+Patni-HAR/2010Feb10/Modified/CDS Requirement/Compatible to V9.50
    if (f_saturn_coil->numSection > 0) {//+Patni-HAR/2010Feb10/Added/CDS Requirement/Compatible to V9.50
        VFF_SET_ARGS(
            l_vf_required[n],
            SVN_PRS_RECVR_COIL,
            SVT_PRS_RECVR_COIL,
            l_size,
            &f_saturn_coil->CoilName[0],
            VFO_VARIABLE_LEN);
        n++;

        l_size = f_saturn_coil->numSection * sizeof(VftSectionRecvrCoil_t);

        VFF_SET_ARGS(
            l_vf_required[n],
            SVN_PLN_SECTION_RECVR_COIL,
            SVT_PLN_SECTION_RECVR_COIL,
            l_size,
            &f_saturn_coil->RcvCoil,
            0);
        n++;

        l_size = f_saturn_coil->numSection * sizeof(VftSectionSignal_t);

        VFF_SET_ARGS(
            l_vf_required[n],
            SVN_PLN_SECTION_RECVR_COIL_SIGNAL,
            SVT_PLN_SECTION_RECVR_COIL_SIGNAL,
            l_size,
            &f_saturn_coil->Signal,
            0);

        n++;

        /* SignalDegeneracy */
        l_size = sizeof(int16_t) ;

        VFF_SET_ARGS(
            l_vf_required[n],
            SVN_EXE_NUM_PAC_DEGENERACY,
            SVT_EXE_NUM_PAC_DEGENERACY,
            l_size,
            &f_saturn_coil->numSignalDegeneracy,
            0);
        n++;
    }

    VFF_SET_ARGS(
        l_vf_required[n],
        SVN_PLN_RECVR_COIL_SHOW_LIST,
        SVT_PLN_RECVR_COIL_SHOW_LIST,
        sizeof(f_saturn_coil->CoilList),
        &f_saturn_coil->CoilList,
        0);

    n++;

    VFF_SET_ARGS(
        l_vf_required[n],
        SVN_PLN_RECVR_COIL_COORDINATE_LIST,
        SVT_PLN_RECVR_COIL_COORDINATE_LIST,
        sizeof(f_saturn_coil->CoilListcoordinate),
        &f_saturn_coil->CoilListcoordinate,
        0);

    n++;

    //-Patni-HAR/2010Feb10/Modified/CDS Requirement/Compatible to V9.50
    int num = 0;
    int l_status = f_vf_handle->GetFields(l_pelm, SVD_ST_PROTOCOL, l_vf_required, n, &num);

    if (l_status != E_NO_ERROR || n != num) {
        l_status = E_ERROR;
    }

    //+Patni-HAR/2010Feb10/Added/CDS Requirement/Compatible to V9.50
    if (n == 1) {
        l_status = E_NO_ERROR;
    }

    //-Patni-HAR/2010Feb10/Added/CDS Requirement/Compatible to V9.50
    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(l_vf_required, 0, sizeof(VfFieldSpec_t) * 7);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008

    /* SignalDegeneracy */
    //+Patni-MP/2009Mar16/Modified/ACE1-MCM0228-00102
    //l_size = f_saturn_coil->numSignalDegeneracy * sizeof(VftSectionSignal_t) ;
    //+Patni-HAR/2010Feb10/Modified/CDS Requirement/Compatible to V9.50
    if (f_saturn_coil->numSection > 0) {//+Patni-HAR/2010Feb10/Added/CDS Requirement/Compatible to V9.50

        n = num = 0;
        l_size = f_saturn_coil->numSection * sizeof(VftSectionSignal_t) ;
        //-Patni-MP/2009Mar16/Modified/ACE1-MCM0228-00102

        VFF_SET_ARGS(
            l_vf_required[n],
            SVN_PLN_SECTION_RECVR_COIL_SIGNAL_DEGENERACY,
            SVT_PLN_SECTION_RECVR_COIL_SIGNAL_DEGENERACY,
            l_size,
            &f_saturn_coil->SignalDegeneracy,
            0);
        n++;

        l_size = sizeof(bool_t) ;

        VFF_SET_ARGS(
            l_vf_required[n],
            SVN_PLN_DEGENERACY_FLAG,
            SVT_PLN_DEGENERACY_FLAG,
            l_size,
            &f_saturn_coil->DegeneracyFlg,
            0);
        n++;
        l_status = f_vf_handle->GetFields(l_pelm, SVD_ST_PROTOCOL, l_vf_required, n, &num);

        if (l_status != E_NO_ERROR || n != num) {
            l_status = E_ERROR ;
        }
    }

    //-Patni-HAR/2010Feb10/Added/CDS Requirement/Compatible to V9.50
    return l_status;
}

//****************************Method Header************************************
//Method Name   : GetConnectedCoil()
//Author        : PATNI\HAR
//Purpose       :
//*****************************************************************************
int CPQMStudy::GetConnectedCoil(
    CVarFieldHandle* const f_vf_handle,
    const int f_protocol,
    CPqmProtocol* f_protocol_object
)
{
    if (f_protocol_object == NULL) {    //Patni-MJC/2009Aug17/Modified/cpp test corrections Need to Check
        return E_ERROR;             //Patni-MJC/2009Aug17/Modified/cpp test corrections Need to Check
    }                               //Patni-MJC/2009Aug17/Modified/cpp test corrections Need to Check

    VfPathElem_t    l_pelm[2] = {0};
    memset(l_pelm, 0, sizeof(VfPathElem_t) * 2); //Patni-MJC/2009Aug17/Modified/cpp test corrections

    //DB_FUNC_ENTER("[PqmPrPutConnectedCoil]:");

    l_pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_pelm[SVD_ST_PROTOCOL - 1].index = f_protocol;

    VfFieldSpec_t   l_vf_required = {0};
    memset(f_protocol_object->GetSarInfo()->connectedCoil, 0,
           sizeof(f_protocol_object->GetSarInfo()->connectedCoil));

    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(&l_vf_required, 0, sizeof(VfFieldSpec_t));
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008

    VFF_SET_ARGS(
        l_vf_required,
        SVN_EXE_CONNECTED_COIL,
        SVT_EXE_CONNECTED_COIL,
        0,
        NULL,
        VFO_ALLOCATE | VFO_VARIABLE_LEN);

    int             num = 0;
    int l_status = f_vf_handle->GetFields(l_pelm, SVN_ST_PROTOCOL, &l_vf_required, 1, &num);

    if ((l_vf_required.buffer != NULL) && (l_vf_required.size != 0)) {

        memcpy(f_protocol_object->GetSarInfo()->connectedCoil,
               l_vf_required.buffer, l_vf_required.size);

        FREE_PTR(l_vf_required.buffer);
    }

    return l_status;
}

//************************************Method Header************************************
// Method Name  : GetInscanFlagForProtocolIndex
// Author       : PATNI/ HEMANT
// Purpose      : This function expects as f_protocol_index as valid protocol index
//                and returns the inscan flag value for this protocol index
//***********************************************************************************
int CPQMStudy::GetInscanFlagForProtocolIndex(
    const int f_protocol_acq_order
)
{
    BITFLD_DECL(mask, K_ALL_PR);
    memset(mask, 0, sizeof(mask));

    int l_inscan_flag = -1;

    //  K_PR_INSCAN_FLAG,               &l_inscan_flag,                 NULL,
    //  K_PR_INSCAN_PREFERENCE_GUID,    &l_inscan_preference_guid,      inscan_preference_guid_length,
    try {
        GetProtocolValues(m_vfdb_handle,
                          f_protocol_acq_order,
                          mask,
                          K_PR_INSCAN_FLAG,
                          &l_inscan_flag,
                          NULL ,
                          NULL);

    } catch (...) {

    }

    return l_inscan_flag;
}

//+Patni-Hemant/2010Apr28/Added/JaFT/IR-137
//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPQMStudy::GetLaterality(
    const int& f_protocol_index,
    int& f_laterality_value
)const
{

    VfFieldSpec_t   l_vfField[1] = {0};
    VfPathElem_t    l_path[2] = {0};
    int             l_numDone = 0;

    CString l_laterality_str(_T(""));

    memset(l_path, 0, sizeof(VfPathElem_t) * 2);
    memset(l_vfField, 0, sizeof(VfFieldSpec_t));

    l_path[0].subtree_name = SVN_ST_PROTOCOL;
    l_path[0].index = f_protocol_index;

    VFF_SET_ARGS(l_vfField[0],
                 SVN_PLN_LATERALITY,
                 SVT_PLN_LATERALITY,
                 0,
                 NULL,
                 VFO_ALLOCATE | VFO_VARIABLE_LEN);

    status_t l_status = m_vfdb_handle->GetFields(l_path, SVN_ST_PROTOCOL,
                        &l_vfField[0], 1, &l_numDone);

    if ((l_vfField[0].buffer == NULL) || (l_vfField[0].size == 0)) {
        //bodypart[0] = '\0';   /* 取れなくても異常ではない。存在しない場合もある。 */
        l_laterality_str = _T("");
        return false;

    } else {
        //Tanoue/MVC5849
        UTIL::Mbs2Wcs(&l_laterality_str, reinterpret_cast<char*>(l_vfField[0].buffer), l_vfField[0].size);
        FREE_PTR(l_vfField[0].buffer);
    }

    f_laterality_value = CPqmUtilities::GetvalForLateralityStr(l_laterality_str);

    return true;
}
//-Patni-Hemant/2010Apr28/Added/JaFT/IR-137

//+Patni-PJS/2010May20/Added/JaFT/TMSC-REDMINE-285
//************************************Method Header************************************
// Method Name  : GetReuseShimValue
// Author       : PATNI/ PJS
// Purpose      : This function is used to get the reuse shim value depending upond
//				  shim enable flag.
//***********************************************************************************
CString CPQMStudy::GetReuseShimValue(
    const int f_protocol
)const
{
    CString l_reuse_shim = _T("");
    CPqm* l_pqm_ptr = GetPqm();

    //Check for NULL
    if (!l_pqm_ptr || !GetShimEnableFlagFromVF(f_protocol)) {
        return l_reuse_shim;
    }

    //Get Shim enable flag from varfield
    VfPathElem_t	l_activeshimpath[2] = {0};
    GetShimPathFromVF(f_protocol, l_activeshimpath);

    CPqmProtocol* l_prot = GetProtocol(l_activeshimpath[0].index);

    if (l_prot) {
        CPQMStudyMgr* l_study_mgr = l_pqm_ptr->GetStudyManagerPtr();
        unsigned short l_series_bias = 0;
        //Get Series bias number
        l_study_mgr->GetSeriesBias(&l_series_bias);
        l_reuse_shim.Format(_T("%d"), (l_series_bias * l_prot->GetPosition()));
    }

    return l_reuse_shim;
}


//************************************Method Header************************************
// Method Name  : GetReuseMapValue
// Author       : PATNI/ PJS
// Purpose      : This function is used to get the reuse map value depending upond
//				  map enable flag.
//***********************************************************************************
CString CPQMStudy::GetReuseMapValue(
    const int f_protocol
)const
{
    CString l_reuse_map = _T("");
    CPqm* l_pqm_ptr = GetPqm();
    CPqmProtocol* l_curr_protocol = GetProtocol(f_protocol);

    //+TMSC-Tanoue/2010Sep03/MVC008324
    if (!l_curr_protocol) {
        return l_reuse_map;
    }

    //-TMSC-Tanoue/2010Sep03/MVC008324

    const int l_map_prot_num = l_curr_protocol->GetSpeederMapProtocol();

    if (l_map_prot_num < 0) {
        return l_reuse_map;
    }

    CPqmProtocol* l_map_obj = GetProtocol(l_map_prot_num);

    //Check for NULL
    if (!l_pqm_ptr || !l_map_obj || (E_NO_ERROR != SpeederMAPExist(l_map_obj->GetProtocol()))) {
        return l_reuse_map;
    }

    //Set Map protocol
    CPQMStudyMgr* l_study_mgr = l_pqm_ptr->GetStudyManagerPtr();
    unsigned short l_series_bias = 0;
    //Get Series bias number
    l_study_mgr->GetSeriesBias(&l_series_bias);
    l_reuse_map.Format(_T("%d"), (l_series_bias * l_map_obj->GetPosition()));


    return l_reuse_map;
}

//************************************Method Header************************************
// Method Name  : GetMapPathFromVF
// Author       : PATNI/ PJS
// Purpose      : This function is used to get the map path from varfield
//***********************************************************************************
bool CPQMStudy::GetMapPathFromVF(
    const int f_protocol,
    VfPathElem_t* f_map_path
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetMapPathFromVF");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetMapPathFromVF"));
    VfFieldSpec_t   l_vfreq[1] = {0};
    VfPathElem_t    l_pelm[SVD_ST_PROTOCOL] = {0};
    int             num = 0, n = 0;

    memset(l_vfreq, 0, sizeof(VfFieldSpec_t) * 1);
    memset(l_pelm, 0, sizeof(VfPathElem_t) * SVD_ST_PROTOCOL);
    l_pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_pelm[SVD_ST_PROTOCOL - 1].index = f_protocol;

    const int l_size = sizeof(VfPathElem_t) * 2;
    //Set varfield arguement
    VFF_SET_ARGS(l_vfreq[0], SVN_SEQ_MAP_PATH, VFT_PATHELEM,
                 l_size, f_map_path,
                 VFO_ALLOCATE | VFO_VARIABLE_LEN);
    n++;
    //Get required tag from varfield
    status_t l_status = m_vfdb_handle->GetFields(l_pelm, SVD_ST_PROTOCOL, l_vfreq, n, &num);

    //Check for error
    if (num != n || l_status == E_ERROR) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed to get the required field"));
        return false;
    }

    return true;
}
//-Patni-PJS/2010May20/Added/JaFT/TMSC-REDMINE-285

//****************************Method Header************************************
//Method Name   :BodyPartGetVF()
//Author        :PATNI / MRP
//Purpose       :To get the protocol from the study
//*****************************************************************************
int CPQMStudy::BodyPartGetVF(const int f_protocol_index,
                             CString& f_body_part_value
                            )
{

    VfFieldSpec_t   l_vfField[1] = {0};//Patni-MJC/2009Aug17/Modified/cpp test corrections
    VfPathElem_t    l_path[2] = {0};//Patni-MJC/2009Aug17/Modified/cpp test corrections

    //DB_FUNC_ENTER("PqmBodyPartGetVF]:");
    memset(l_path, 0, sizeof(VfPathElem_t) * 2);
    memset(l_vfField, 0, sizeof(VfFieldSpec_t));

    l_path[0].subtree_name = SVN_ST_PROTOCOL;
    l_path[0].index = f_protocol_index;

    VFF_SET_ARGS(l_vfField[0],
                 SVN_PLN_BODY_PART,
                 SVT_PLN_BODY_PART,
                 0,
                 NULL,
                 VFO_ALLOCATE | VFO_VARIABLE_LEN);

    int l_numDone = 0;
    status_t l_status = m_vfdb_handle->GetFields(l_path, SVN_ST_PROTOCOL,
                        &l_vfField[0], 1, &l_numDone);

    if ((l_vfField[0].buffer == NULL) || (l_vfField[0].size == 0)) {
        //bodypart[0] = '\0';   /* 取れなくても異常ではない。存在しない場合もある。 */
        f_body_part_value = _T("");
        return NO_FIELD;

    } else {
        //Tanoue/MVC5849
        UTIL::Mbs2Wcs(&f_body_part_value, reinterpret_cast<char*>(l_vfField[0].buffer), l_vfField[0].size);
        FREE_PTR(l_vfField[0].buffer);
    }


    return l_status;
}
//-Patni-MP/2009Mar17/Added/ACE-1/MCM0213-00131

//+Patni-MP/2009Mar16/Added/ACE-1/MCM0213-00181
//****************************Method Header************************************
//Method Name   :BodyPartSetVF()
//Author        :PATNI / MRP
//Purpose       :To set the BodyPart for the protocol in the study
//*****************************************************************************
int CPQMStudy::BodyPartSetVF(
    const int f_protocol_index,
    CString f_body_part_value
)
{

    VfFieldSpec_t   l_vfField[1] = {0};//Patni-MJC/2009Aug17/Modified/cpp test corrections
    VfPathElem_t    l_path[2] = {0};//Patni-MJC/2009Aug17/Modified/cpp test corrections

    //DB_FUNC_ENTER("PqmBodyPartSetVF]:");
    memset(l_path, 0, sizeof(VfPathElem_t) * 2);
    memset(l_vfField, 0, sizeof(VfFieldSpec_t));

    l_path[0].subtree_name = SVN_ST_PROTOCOL;
    l_path[0].index = f_protocol_index;

    std::string mbs_body_part(""); //Tanoue/MVC5849

    if (f_body_part_value.GetLength() == 0) {
        if (BodyPartGetVF(f_protocol_index, f_body_part_value) != NO_FIELD) {
            VFF_SET_ARGS(l_vfField[0],
                         SVN_PLN_BODY_PART,
                         SVT_PLN_BODY_PART,
                         0,
                         NULL,
                         VFO_DELETE);

        } else {
            return E_NO_ERROR;
        }


    } else {
        //Tanoue/MVC5849
        UTIL::Wcs2Mbs(&mbs_body_part, f_body_part_value);
        VFF_SET_ARGS(l_vfField[0],
                     SVN_PLN_BODY_PART,
                     SVT_PLN_BODY_PART,
                     mbs_body_part.length() + 1,
                     const_cast<char*>(mbs_body_part.c_str()),
                     VFO_REPLACE);
    }

    time_t          ts = 0;//Patni-MJC/2009Aug17/Modified/cpp test corrections
    int             l_numDone = 0;
    status_t        l_status = m_vfdb_handle->PutFields(l_path, SVN_ST_PROTOCOL, &l_vfField[0],
                               1, &l_numDone, &ts);

    return l_status;
}

//****************************Method Header************************************
//Method Name   :SetNewRflSARForProtocol()
//Author        :PATNI/HAR-Sribanta
//Purpose       :
//*****************************************************************************
void CPQMStudy::SetNewRflSARForProtocol(
    SequenceList& f_sequence_list
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetNewRflSARForProtocol");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));


    CPqmProtocol* l_result_acq_series	= NULL;
    CPqmProtocol* l_rfl_object		= NULL;
    protocolvec::iterator v1_Iter	= NULL;

    POSITION pos = f_sequence_list.GetHeadPosition();
    CPASCOMSequence* pseq = NULL;

    for (v1_Iter = m_protocol_vec.begin(); v1_Iter != m_protocol_vec.end(); v1_Iter++) {

        l_result_acq_series = *v1_Iter;
        pseq = f_sequence_list.GetNext(pos);

        if (NULL == l_result_acq_series) {
            continue;
        }

        if (C_PR_STATUS_WAIT == l_result_acq_series->GetAcqStatus()) {
            l_rfl_object = l_result_acq_series;
            break;

        } else if ((C_PR_STATUS_DONE == l_result_acq_series->GetAcqStatus()) ||
                   (C_PR_STATUS_FAIL == l_result_acq_series->GetAcqStatus())) {
            l_rfl_object = l_result_acq_series;

        } else if (C_PR_STATUS_CURRENT == l_result_acq_series->GetAcqStatus()) {

            BITFLD_DECL(sub_mask, MAX_NODES);

            SetAcqStatus(l_result_acq_series, C_PR_STATUS_FAIL);
            pseq->GetSequenceParam()->SetFloat(_T("ACQSTATUS2"), (float)l_result_acq_series->GetAcqStatus());

            int l_acq_status = C_PR_STATUS_FAIL;
            int l_postmsg_status = E_ERROR;

            if (!l_result_acq_series->GetSeriesNumInDbFlag()) {
                l_postmsg_status = m_pqm_ptr->GetDbsaStudyMgr()->SetSeriesNoInDB(
                                       (LPCTSTR)l_result_acq_series->GetSeriesLOID(), C_PR_STATUS_FAIL,
                                       l_result_acq_series->GetProtocol());

                if (l_postmsg_status == E_ERROR) {
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                              _T("Failed to set series in DB"));

                } else {
                    l_result_acq_series->SetSeriesNumInDbFlag(TRUE);
                }
            }

            CPqmUtilities::UpdateIECInfo(l_result_acq_series);

            if (E_NO_ERROR != SetProtocolValues(m_vfdb_handle, l_result_acq_series->GetProtocol(), sub_mask,
                                                K_PR_ACQ_STATUS, &l_acq_status, sizeof(int), NULL)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetProtocolValues failed."));
            }

            if (E_NO_ERROR != UpdateRTSAEValue(l_result_acq_series)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed to update Hardware SAE Value"));
            }
        }

        if (l_rfl_object) {
            GetNewRflSAR(l_rfl_object->GetProtocol());
        }
    }
}

//****************************Method Header************************************
//Method Name   :SetProtocolObject()
//Author        :PATNI/HAR - SS
//Purpose       :
//*****************************************************************************
BOOL CPQMStudy::SetProtocolObject(
    CVarFieldHandle* const f_vf_handle,
    CPqmProtocol* pr_obj_p
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetProtocolObject");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetProtocolObject"));

    if (pr_obj_p == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("No pr_obj_p ERROR !!!!\n"));
        return  FALSE;
    }

    BITFLD_DECL(sub_mask, K_ALL_PR);

    CString str(_T(""));
    str.Format(L"prot = %d, status = %d \n", pr_obj_p->GetProtocol(),
               pr_obj_p->GetAcqStatus());

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str);

    status_t    status = E_NO_ERROR;

    // Get protocol values from study file.
    if (!GetProtocolItemValues(f_vf_handle, pr_obj_p)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetProtocolItemValues ERROR.\n"));

        status = E_ERROR;

    } else if (pr_obj_p->GetAcqStatus() == C_PR_STATUS_WAIT) {
        //Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution

        SetBreathHoldValue(pr_obj_p, f_vf_handle);


        //+Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
        int l_scannumber = 0;
        float   l_couchpos = 0.0f, l_homepos = 0.0f;
        //-Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
        CPqmShimSharedData::GetInstance()->CopyCouchInfo(&l_scannumber, &l_couchpos, &l_homepos);
        pr_obj_p->GetPqmPrMode()->SetCouchPos(l_couchpos/*PqmAcqGetCurrentCouch()*/);

        /* Set couch position */
        BITFLD_INIT(sub_mask, K_ALL_PR, 0);

        if (E_NO_ERROR != SetProtocolValues(f_vf_handle,
                                            pr_obj_p->GetProtocol(),
                                            sub_mask,
                                            K_PR_COUCH,
                                            &l_couchpos,
                                            sizeof(float),
                                            NULL)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetProtocolValues failed."));
        }

        //+Patni-ARD/2009Nov03/Added/IR 85 Auto Transfer
        int l_autotransfer_value = 0;

        if (CPQMConfig::GetInstance()->CanEnableAutoTransfer()) {
            l_autotransfer_value = pr_obj_p->GetAutoTransfer();
        }

        if (E_NO_ERROR != SetProtocolValues(m_vfdb_handle,
                                            pr_obj_p->GetProtocol(),
                                            sub_mask,
                                            K_PR_AUTO_POSTPROC_FLG,
                                            &l_autotransfer_value,
                                            sizeof(long),
                                            NULL)) status = false;

        pr_obj_p->SetAutoTransfer(l_autotransfer_value);

        //-Patni-ARD/2009Nov03/Added/IR 85 Auto Transfer
    }

    CheckAutoLocLicensenSetStatus(pr_obj_p);

    return  status;
}

//****************************Method Header************************************
//Method Name   :SetTopWaitPos()
//Author        :PATNI/MRP
//Purpose       :Sets the position of the topmost wait protocol
//*****************************************************************************
void CPQMStudy::SetTopWaitPos(
    int waitpos
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetTopWaitPos");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetTopWaitPos"));

    m_wait_pos = waitpos;
}

//***************************Method Header*************************************
//Method Name    :SetScanDateTime()
//Author         :PATNI/HAR-SS
//Purpose        :
//*****************************************************************************
int CPQMStudy::SetScanDateTime(
    long f_time
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetScanDateTime");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetScanDateTime"));

    //Patni-PJS/2009Aug19/Modified/ACE# CPP Test Tool Execution
    VfFieldSpec_t vfReq[2] = {0};

    VFF_SET_ARGS(
        vfReq[0],
        SVN_SCH_N_SCAN_DATE_TIME,   //Patni-HAR/2010May06/Modified/MVC006285
        SVT_SCH_N_SCAN_DATE_TIME,   //Patni-HAR/2010May06/Modified/MVC006285
        sizeof(long),
        &f_time,
        VFO_REPLACE);

    //+Patni-SS/2009Apr01/PSP1#VFLOCK
    CString l_log_msg = _T("");
    l_log_msg.Format(_T("SVN_SCH_SCAN_DATE_TIME =%d "), f_time);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_msg);
    //-Patni-SS/2009Apr01/PSP1#VFLOCK

    //+Patni+MP/2009May02/added/ACE-1
    struct tm*  pTms = localtime((time_t*)(&f_time));
    char cTime[128] = {0};
    strftime(cTime, sizeof(cTime), "%d/%m/%Y,%H:%M:%S", pTms);
    VFF_SET_ARGS(vfReq[1], SVN_SCH_N_SCAN_DATE_TIME, SVT_SCH_N_SCAN_DATE_TIME,
                 strlen(cTime), cTime, VFO_REPLACE);

    //+Patni-MP/2009May02/added/ACE-1
    //+Patni-MP/2009May02/Modified/ACE-1
    int num = 0;
    time_t tm = 0;
    int status = m_vfdb_handle->PutFields(NULL, SVD_ST_STUDY, vfReq, 2, &num, &tm);

    if (status == E_NO_ERROR && num == 2) {
        l_log_msg.Format(L"(status = %d) == E_NO_ERROR && (num = %d) == 1", status, num);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_msg);
        return vfReq[0].status;
    }

    //-Patni-MP/2009May02/modified/ACE-1
    return status;
}

//***************************Method Header*************************************
//Method Name    :SetAnatomyInDB()
//Author         :PATNI/HAR-SS
//Purpose        :
//*****************************************************************************
BOOL CPQMStudy::SetAnatomyInDB(
    int anatomy,
    const int protocol
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetAnatomyInDB");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetAnatomyInDB"));
    BOOL status = FALSE;

    BITFLD_DECL(mask, K_ALL_PR);

    status = SetProtocolValues(m_vfdb_handle,
                               protocol,
                               mask,
                               K_PR_ANATOMY,
                               &anatomy,
                               sizeof(int),
                               NULL);
    return status;
}

//***************************Method Header*************************************
//Method Name    :SetPreScanValue()
//Author         :
//Purpose        :
//*****************************************************************************
void CPQMStudy::SetPreScanValue(
    const int f_acq_order
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetPreScanValue");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetPreScanValue"));



    BITFLD_DECL(mask, K_ALL_PR);

    float gain_value = 55;
    float scaling_value = 29209.9f;
    float rgn_offset = 0.00f;

    const BOOL status = SetProtocolValues(m_vfdb_handle,
                                          f_acq_order,
                                          mask,
                                          K_PR_PRESCAN_RFL_GAIN,
                                          &gain_value,
                                          sizeof(float),
                                          K_PR_PRESCAN_RFL_SCALER,
                                          &scaling_value,
                                          sizeof(float),
                                          /*  K_PR_PRESCAN_RGN_VALUE,
                                              rgn_gain,
                                              sizeof(float),*/
                                          K_PR_PRESCAN_RGN_OFFSET,
                                          rgn_offset,
                                          sizeof(float),
                                          NULL);
}

//Patni-AMT/2010Feb16/Commented/Ph-3# PROPOSAL_07-GetPositionFromProtocol()
//***************************Method Header*************************************
//Method Name    :SetSiteId()
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
void CPQMStudy::SetSiteId(
    const int site_id
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetSiteId");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetSiteId"));

    m_study_id.site_id = site_id;
}

//***************************Method Header*************************************
//Method Name    :SetRumNum()
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
void CPQMStudy::SetRumNum(
    const int rum_num
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetRumNum");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetRumNum"));
    m_study_id.run_num = rum_num;
}

//***************************Method Header*************************************
//Method Name    :SetupShimming()
//Author         :PATNI/
//Purpose        :
//*****************************************************************************
void CPQMStudy::SetupShimming(
    CVarFieldHandle* const f_vf_handle
)
{
    // + SM4 Himanshu CFA Fix
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetupShimming");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetupShimming"));

    CString l_print_str(_T(""));
    l_print_str.Format(_T("VF Handle = %x"), f_vf_handle);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_print_str);

    int             l_shimstate = -1, l_shimfsyflag = -1;
    VfFieldSpec_t   l_vfreq[8] = {0};                   //Patni-MJC/2009Aug17/Modified/cpp test corrections
    VfPathElem_t    l_path[SVD_ST_PROTOCOL] = {0};      //Patni-MJC/2009Aug17/Modified/cpp test corrections
    VfPathElem_t    l_newpath[SVD_ST_PROCESSED] = {0};  //Patni-MJC/2009Aug17/Modified/cpp test corrections

    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(l_vfreq, 0, sizeof(VfFieldSpec_t) * 8);
    memset(l_path, 0, sizeof(VfPathElem_t) * SVD_ST_PROTOCOL);
    memset(l_newpath, 0, sizeof(VfPathElem_t) * SVD_ST_PROCESSED);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008

    l_path[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_path[SVD_ST_PROTOCOL - 1].index = 0;

    CPqmShimSharedData::GetInstance()->GetShimPath(l_newpath);
    CPqmShimSharedData::GetInstance()->GetShimState(&l_shimstate);
    CPqmShimSharedData::GetInstance()->GetShimFsyFlag(&l_shimfsyflag);

    for (int n = 0; n < sizeof(shimTbl) / sizeof(*shimTbl); n++) {

        VFF_SET_ARGS(l_vfreq[n], shimTbl[n].name, shimTbl[n].type, 0,
                     NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);
    }

    CString l_path_str = _T("");

    for (int index = 0; index < SVD_ST_PROCESSED; index ++) {
        l_path_str.Format(_T("//%d.%d"), l_newpath[index].subtree_name, l_newpath[index].index);
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_path_str);

    int num = 0;
    status_t l_status = m_vfdb_handle->GetFields(l_newpath, SVD_ST_PROCESSED, l_vfreq, n, &num);
    l_print_str.Format(L"Status of m_vfdb_handle->GetFields() is:- %d", l_status);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_print_str);

    int vfstat = -1;

    if (vfstat != E_NO_ERROR) {
        DB_PRINTF("M", ("vfstat = %d\n", vfstat));

        for (n = 0; n < sizeof(shimTbl) / sizeof(*shimTbl); n++) {
            FREE_PTR(shimTbl[n].shim);
        }
    }

    for (n = 0; n < sizeof(shimTbl) / sizeof(*shimTbl); n++) {

        //+Patni-Raj/2010Jan15/Added/PH#3#/Debug Window
        CPQMDebugInfo::GetInstance(m_pqm_ptr)->PqmDebugAcqMonitor(_T("[%d] VfName = %d status = %d"),
                n, shimTbl[n].name, l_vfreq[n].status);
        //-Patni-Raj/2010Jan15/Added/PH#3#/Debug Window

        if (l_vfreq[n].status == VFE_NO_ERROR) {
            shimTbl[n].shim = l_vfreq[n].buffer;
            shimTbl[n].shimSize = l_vfreq[n].size;
        }
    }

    l_newpath[0].index = 1;
    l_newpath[1].index = 0;

    vfstat = f_vf_handle->AddBranch(l_newpath, SVD_ST_PROTOCOL);

    if (vfstat != E_NO_ERROR) {
        DB_PRINTF("M", ("vfstat = %d\n", vfstat));

        for (n = 0; n < sizeof(shimTbl) / sizeof(*shimTbl); n++) {
            FREE_PTR(shimTbl[n].shim);
        }

        return;

    }

    vfstat = f_vf_handle->CreateSubtree(l_newpath, SVD_ST_PROTOCOL, SVN_ST_PROCESSED, 1);

    if (vfstat != E_NO_ERROR) {
        DB_PRINTF("M", ("vfstat = %d\n", vfstat));

        for (n = 0; n < sizeof(shimTbl) / sizeof(*shimTbl); n++) {
            FREE_PTR(shimTbl[n].shim);
        }

        return;
    }

    BOOL l_isactiveshim = (l_shimstate == SHIM_STATE_ACTIVE);
    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(l_vfreq, 0, sizeof(VfFieldSpec_t) * 8);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    n = 0;
    VFF_SET_ARGS(l_vfreq[n], SVN_SEQ_SHIM_PATH, SVT_SEQ_SHIM_PATH,
                 sizeof(l_newpath), l_newpath, VFO_REPLACE);
    n++;

    VFF_SET_ARGS(l_vfreq[n], SVN_PLN_SHIM_ENABLE_FLAG,
                 SVT_PLN_SHIM_ENABLE_FLAG,
                 sizeof(l_isactiveshim), &l_isactiveshim, VFO_REPLACE);
    n++;

    VFF_SET_ARGS(l_vfreq[n], SVN_SEQ_SHIM_FSY_FLAG, SVT_SEQ_SHIM_FSY_FLAG,
                 sizeof(int), &l_shimfsyflag,
                 VFO_REPLACE);
    n++;
    time_t          tm = -1;
    vfstat = f_vf_handle->PutFields(l_path, SVD_ST_PROTOCOL, l_vfreq, n, &num, &tm);

    if (vfstat != E_NO_ERROR) {
        DB_PRINTF("M", ("vfstat = %d\n", vfstat));

        for (n = 0; n < sizeof(shimTbl) / sizeof(*shimTbl); n++) {
            FREE_PTR(shimTbl[n].shim);
        }

        return;
    }

    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(l_vfreq, 0, sizeof(VfFieldSpec_t) * 8);

    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    for (int i = n = 0; i < sizeof(shimTbl) / sizeof(*shimTbl); i++) {
        if (shimTbl[i].shim != NULL) {
            VFF_SET_ARGS(l_vfreq[n], shimTbl[i].name, shimTbl[n].type,
                         shimTbl[i].shimSize, shimTbl[i].shim,
                         VFO_REPLACE);
            n++;
        }
    }

    //Patni-Raj/2010Jan15/Added/PH#3#/Debug Window
    CPQMDebugInfo::GetInstance(m_pqm_ptr)->PqmDebugAcqMonitor(_T("Shimming Data n=%d"), n);

    if (n > 0) {

        vfstat = f_vf_handle->PutFields(l_newpath, SVD_ST_PROCESSED, l_vfreq, n, &num, &tm);

        if (vfstat != E_NO_ERROR) {
            DB_PRINTF("M", ("vfstat = %d\n", vfstat));

            for (n = 0; n < sizeof(shimTbl) / sizeof(*shimTbl); n++) {
                FREE_PTR(shimTbl[n].shim);
            }

            return;
        }
    }

    // - SM4 Himanshu CFA Fix
}

//***************************Method Header*************************************
//Method Name    :SetStudyName()
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
void CPQMStudy::SetStudyName(
    const CString& f_study_file_name
)
{
    m_study_name = f_study_file_name;
}

//****************************Method Header************************************
//Method Name   : SetPVCoilTbl()
//Author        : PATNI\MRP
//Purpose       :
//*****************************************************************************
status_t CPQMStudy::SetPVCoilTbl(
    const int     port,
    int*        rcvCoilTbl,
    const int     n)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetPVCoilTbl");
    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    PqmCoil_t*  coil_p = NULL; //Patni-MJC/2009Aug17/Modified/cpp test corrections Need to check
    int     d_size = 0, i = 0, total = 0;
    int     gain_data[PQM_NUM_VFEP_CHANNEL * 16] = {0};
    int     pac_pattern[16] = {0};
    char    CoilNameTbl[64 * 16] = "";
    int     mode = 0;
    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    BITFLD_DECL(mask, K_ALL_PR);

    DB_FUNC_ENTER("[PqmSetPVCoilTbl]:");

    if (n == 0)
        mode = FALSE ;

    else
        mode = TRUE ;

    memset(gain_data, 0 , sizeof(gain_data)) ;
    memset(pac_pattern, 0 , sizeof(pac_pattern)) ;
    memset(CoilNameTbl, 0 , sizeof(CoilNameTbl)) ;

    for (i = 0 ; i < n ; i++) {

        //Patni-MJC/2009Aug17/Modified/cpp test corrections Need to check
        if (rcvCoilTbl == NULL) {
            DB_PRINTF("rcvCoilTbl in PqmSetPVCoilTbl is NULL");
            return E_ERROR;
        }

        //Patni-MJC/2009Aug17/Modified/cpp test corrections Need to check
        DB_PRINTF("c", ("port=%d , total=%d,[0X%x]\n", port, n , *(rcvCoilTbl + i)));
        CPqmCoilData*   l_coil_data = CDataManager::GetInstance()->GetPQMCoilData();

        //+Patni-RUP/2009Aug21/Modified/cpp test corrections
        if (l_coil_data == NULL) {
            PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("l_coil_data pointer is NULL"));
            return E_ERROR;
        }

        //-Patni-RUP/2009Aug21/Modified/cpp test corrections
        CCoil dummy_coil;
        l_coil_data->PqmCoilGetCoil(*(rcvCoilTbl + i), dummy_coil) ;
        coil_p = dummy_coil.GetCoil();

        if (coil_p == NULL) {
            continue ;
            //return E_ERROR ;          //Patni-RUP/2009Aug21/Modified/cpp test corrections Need to check
        }

        memcpy(&pac_pattern[i], &coil_p->pac_pattern ,
               sizeof(coil_p->pac_pattern)) ;
        memcpy(&gain_data[PQM_NUM_VFEP_CHANNEL * i], coil_p->gain_data ,
               sizeof(coil_p->gain_data)) ;

        if (i != 0) {
            if ((total - 1) < 1024) {       //Patni-RUP/2009Aug21/Modified/cpp test corrections Need to check
                CoilNameTbl[total - 1] = ';' ;
                //+Patni-RUP/2009Aug21/Modified/cpp test corrections Need to check

            } else {
                PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("CoilNameTbl array out of Bound"));
                return E_ERROR;
            }

            //-Patni-RUP/2009Aug21/Modified/cpp test corrections Need to check
        }

        d_size = strlen(coil_p->label) + 1 ;

        if ((total + d_size) < 1024) {      //Patni-RUP/2009Aug21/Modified/cpp test corrections Need to check
            memcpy(&CoilNameTbl[total], coil_p->label, d_size) ;
            //+Patni-RUP/2009Aug21/Modified/cpp test corrections Need to check

        } else {
            PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("CoilNameTbl array out of Bound"));
            return E_ERROR;
        }

        //-Patni-RUP/2009Aug21/Modified/cpp test corrections Need to check
        total += d_size ;
    }

    DB_PRINTF("c", ("total=%d,[%s]\n", total, CoilNameTbl));
    SetProtValues(m_vfdb_handle, port, mask,
                  K_PR_CHANGED_RECEIVE_COIL,    &mode,      sizeof(mode),
                  K_PR_RECVR_COIL_TBL,      rcvCoilTbl, sizeof(int)*n,
                  K_PR_CHANNEL_GAIN_RATIO_TBL,  &gain_data, sizeof(flt32_t)*PQM_NUM_VFEP_CHANNEL * n,
                  K_PR_PAC_PATTERN_TBL,     &pac_pattern,   sizeof(int)*n,
                  K_PR_RECVR_COILNAME_TBL,  &CoilNameTbl,   total,
                  NULL);
    return E_NO_ERROR;
}

//+Patni-MP/2009Mar16/Added/ACE-1/MCM0213-00181
/**********************************************
  Studyフィールドからprotに対応した
  SVN_PRC_PARENT_INDEX_FOR_PLANNINGを取得する。
  取得できた場合、無効にするため-1をセットする。
***********************************************/
//****************************Method Header************************************
//Method Name   :SetAgentValueVF()
//Author        :PATNI / MRP
//Purpose       :To set the Scan Agent for the protocol in the study
//*****************************************************************************
int CPQMStudy::SetAgentValueVF(
    const int f_protocol_index,
    const CString& f_scan_agent_value
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetAgentValueVF");

    CString l_str(_T(""));
    l_str.Format(_T("Protocol Index  = %d and Agent %s"), f_protocol_index, f_scan_agent_value);
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_str);

    VfFieldSpec_t   l_vfField[1] = {0};
    VfPathElem_t    l_path[2] = {0};

    memset(l_path, 0, sizeof(VfPathElem_t) * 2);
    memset(l_vfField, 0, sizeof(VfFieldSpec_t));

    l_path[0].subtree_name = SVN_ST_PROTOCOL;
    l_path[0].index = f_protocol_index;

    //+Patni-DKH/2010Dec10/Modified/TMSC-REDMINE-1074
    //if (f_scan_agent_value.GetLength() == 0) {
    //    return E_NO_ERROR;

    //} else {
    //-Patni-DKH/2010Dec10/Modified/TMSC-REDMINE-1074
    int l_length = f_scan_agent_value.GetLength() ;
    char* f_scan_agent_value_char = new char[l_length + 1];

    //+Patni-RUP/2009Aug25/Modified/cpp test corrections
    if (f_scan_agent_value_char == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("f_scan_agent_value_char pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-RUP/2009Aug25/Modified/cpp test corrections

    memset(f_scan_agent_value_char, 0, (l_length + 1)*sizeof(f_scan_agent_value_char[0]));
    wcstombs(f_scan_agent_value_char, f_scan_agent_value, l_length + 1);
    VFF_SET_ARGS(l_vfField[0],
                 SVN_EXE_SCAN_AGENT,
                 SVT_EXE_SCAN_AGENT,
                 l_length + 1,
                 f_scan_agent_value_char,
                 VFO_REPLACE);
    //Patni-DKH/2010Dec10/Modified/TMSC-REDMINE-1074
    //}
    time_t              ts = 0;//Patni-MJC/2009Aug17/Modified/cpp test corrections
    int             l_numDone = 0;
    status_t l_status = m_vfdb_handle->PutFields(l_path, SVN_ST_PROTOCOL, &l_vfField[0],
                        1, &l_numDone, &ts);

    DEL_PTR_ARY(f_scan_agent_value_char);

    if (l_status == E_NO_ERROR) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("CPQMStudy::PutFields succsessfull"));

    } else {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("CPQMStudy::PutFields Failed"));
    }

    return l_status;
}
//-Patni+MP/2009Jun07/Added/ACE-2/Missing Vantage Skip Prescan


status_t CPQMStudy::DeleteCALAFields(
    const int f_acq_order
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetAgentValueVF");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));
    VfPathElem_t l_pelm[1] = {0};
    time_t l_tm = 0;
    VfFieldSpec_t l_vf_req[5] = {0};


    memset(l_vf_req, NULL, sizeof(VfFieldSpec_t) * 5);

    int n = 0;
    VFF_SET_ARGS(l_vf_req[n], SVN_PLN_CALA_OFFSET_VEC_TABLE , SVT_PLN_CALA_OFFSET_VEC_TABLE, sizeof(f32vec3_t), NULL, VFO_DELETE);
    n++;

    VFF_SET_ARGS(l_vf_req[n], SVN_PLN_CALA_PHASE_VEC_TABLE , SVT_PLN_CALA_PHASE_VEC_TABLE, sizeof(f32vec3_t), NULL, VFO_DELETE);
    n++;

    VFF_SET_ARGS(l_vf_req[n], SVN_PLN_CALA_SLICE_VEC_TABLE , SVT_PLN_CALA_SLICE_VEC_TABLE, sizeof(f32vec3_t), NULL, VFO_DELETE);
    n++;

    VFF_SET_ARGS(l_vf_req[n], SVN_PLN_CALA_CALCULATED_STATUS , SVT_PLN_CALA_CALCULATED_STATUS, sizeof(int), NULL, VFO_DELETE);
    n++;

    VFF_SET_ARGS(l_vf_req[n], SVN_PLN_CALA_INPUT_PROTOCOL_INDEX , SVT_PLN_CALA_INPUT_PROTOCOL_INDEX, sizeof(int), NULL, VFO_DELETE);
    n++;

    int l_num = 0;

    status_t l_status = m_vfdb_handle->PutFields(l_pelm, SVD_ST_STUDY, l_vf_req,
                        n, &l_num, &l_tm);

    BITFLD_DECL(mask,   MAX_BITFLD) = {0};
    BITFLD_INIT(mask, K_ALL_PR, 0);
    memset(mask, 0, sizeof(mask));
    int l_cala_planned_status = VFC_CALA_PLANNED_STATUS_NOVECTOR;

    if (E_NO_ERROR != SetProtocolValues(m_vfdb_handle,
                                        f_acq_order, mask, K_PR_CALA_PLANNED_STATUS, &l_cala_planned_status,
                                        sizeof(l_cala_planned_status), NULL)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetProtocolValues() failed."));
        l_status = E_ERROR;

    } else {
        CPqmProtocol* l_prot = GetProtocol(f_acq_order);

        if (l_prot) {
            l_prot->SetCalaPlannedStatus(PLANNED_STATUS_NOVECTOR);
        }
    }

    return l_status;
}

//****************************Method Header************************************
//Method Name   :MaintainScratchPadForPrivateProlim()
//Author        :PATNI
//Purpose       :To create scratchpad for private prolim
//*****************************************************************************
status_t CPQMStudy::MaintainScratchPadForPrivateProlim(const int f_acq_order, const char* tmp_file, const SCRATCHPAD_CREATION_MODE f_sp_creation_mode)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::MaintainScratchPadForPrivateProlim");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of MaintainScratchPadForPrivateProlim"));

    if (f_acq_order < 0) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Protocol Acqorder is not correct."));
        return E_ERROR;
    }

    if (NULL == tmp_file) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("tmp_file pointer is NULL"));
        return E_ERROR;
    }

    if (E_NO_ERROR != CreateScratchPad(f_acq_order, tmp_file, f_sp_creation_mode)) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("ScratchPad Cration Failed"));
        return E_ERROR;
    }

    PqmPrSetPatientWeight(tmp_file);

    return E_NO_ERROR;
}
//****************************Method Header************************************
//Method Name   :MaintainScratchPadforMRS()
//Author        :PATNI
//Purpose       :To create scratchpad for private prolim for MRS
//*****************************************************************************
void CPQMStudy::MaintainScratchPadforMRS(const int f_acq_order, char* tmp_file,
        const SCRATCHPAD_CREATION_MODE f_sp_creation_mode)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::MaintainScratchPadforMRS");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of MaintainScratchPadforMRS"));

    if (f_acq_order < 0) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Protocol Acqorder is not correct."));
        return ;
    }

    if (NULL == tmp_file) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("tmp_file pointer is NULL"));
        return ;
    }

    if (CreateScratchPad(f_acq_order, tmp_file, f_sp_creation_mode) == E_NO_ERROR) {
        MaintainShimming(CString(tmp_file));
        CopyPCVAPmodetoMRSScratchPad(f_acq_order, static_cast<CString>(tmp_file));
    }
}
//**************************************************************************
//Method Name   : GetMasterFlagfromVF
//Author        : PATNI/AKR--NFD002
//Purpose       : Gets the master flag of given protocol's acq_order
//**************************************************************************
BOOL CPQMStudy::GetMasterFlagfromVF(const int f_acq_order)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetMasterFlagfromVF");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    BOOL	l_master_flag = 0;

    if (NULL == m_vfdb_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_vfdb_handle pointer is NULL"));
        return l_master_flag;
    }

    int				l_num_done = 0;
    int				l_status = E_ERROR;
    int				l_n = 0;


    VfFieldSpec_t	l_vf_req[NUM_1] = {0};
    VfPathElem_t	l_path[NUM_1] = {0};

    l_path[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_path[SVD_ST_PROTOCOL - 1].index = f_acq_order;

    VFF_SET_ARGS(l_vf_req[0],
                 SVN_PLN_MASTER_FLAG,
                 VFT_INT32,
                 sizeof(VFT_INT32),
                 &l_master_flag,
                 NULL);
    l_n++;

    l_status = m_vfdb_handle->GetFields(l_path, SVD_ST_PROTOCOL, l_vf_req,
                                        l_n, &l_num_done);

    if ((E_ERROR == l_status) || (l_num_done != l_n)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_master_flag is Not Present."));
    }

    return l_master_flag;
}

//**************************************************************************
//Method Name   : GetGroupIDfromVF
//Author        : PATNI/AKR--NFD002
//Purpose       : Gets thye group id of given protocol's acq_order
//**************************************************************************
int CPQMStudy::GetGroupIDfromVF(const int f_acq_order)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetGroupIDfromVF");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    int		l_group_id = 0;

    if (NULL == m_vfdb_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_vfdb_handle pointer is NULL"));
        return l_group_id;
    }

    int				l_num_done = 0;
    int				l_status = E_ERROR;
    int				l_n = 0;

    VfFieldSpec_t	l_vf_req[NUM_1] = {0};
    VfPathElem_t	l_path[NUM_1] = {0};

    l_path[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_path[SVD_ST_PROTOCOL - 1].index = f_acq_order;

    VFF_SET_ARGS(l_vf_req[0],
                 SVN_PLN_GROUP_ID,
                 VFT_INT32,
                 sizeof(VFT_INT32),
                 &l_group_id,
                 NULL);
    l_n++;

    l_status = m_vfdb_handle->GetFields(l_path, SVD_ST_PROTOCOL, l_vf_req,
                                        l_n, &l_num_done);

    if ((E_ERROR == l_status) || (l_num_done != l_n)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Group ID is Not Present."));
    }

    return l_group_id;
}
//**************************************************************************
//Method Name   : GetGroupNumFromVF
//Author        : PATNI/AKR--NFD002
//Purpose       : Gets the Group Num from VFC
//**************************************************************************
int CPQMStudy::GetGroupNumFromVF()
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetGroupNumFromVFC");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, FUNC_NAME);

    int		l_group_num = 0;

    if (NULL == m_vfdb_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_vfdb_handle pointer is NULL"));
        return 0;
    }

    int				l_num_done = 0;
    int				l_status = E_ERROR;
    int				l_n = 0;
    VfFieldSpec_t	l_vf_req[NUM_1] = {0};


    VFF_SET_ARGS(l_vf_req[0],
                 SVN_PLN_GROUP_NUM,
                 VFT_INT32,
                 sizeof(VFT_INT32),
                 &l_group_num,
                 NULL);
    l_n++;

    l_status = m_vfdb_handle->GetFields(NULL, SVD_ST_STUDY, l_vf_req,
                                        l_n, &l_num_done);

    if ((E_ERROR == l_status) || (l_num_done != l_n)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Group num tag Not Present."));
    }

    return l_group_num;
}

//****************************Method Header************************************
//Method Name   :CheckMulitScanDynamic_License()
//Author        :PATNI/Abhishek/Redmine-2187
//Purpose       :This method will check for mulit scan dynamic licsence & if
//				license not found it will delete the required tags from study
//*****************************************************************************
BOOL CPQMStudy::CheckMulitScanDynamic_License(CPqmProtocol* f_obj, const int f_protocol)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::CheckMulitScanDynamic_License");

    if ((!m_pqm_ptr->GetMulitScanDynamic_License())) {

        if (NULL == m_vfdb_handle) {
            return FALSE;
        }

        VfFieldSpec_t   vfReq[2] = {0};
        VfPathElem_t    path[1] = {0};

        memset(vfReq, 0, sizeof(VfFieldSpec_t));
        memset(path, 0, sizeof(VfPathElem_t));
        path[0].subtree_name = SVN_ST_PROTOCOL;
        path[0].index = f_protocol;

        int n = 0, num_put = 0;

        CString l_strLog(_T(""));

        if (true == IsTagPresentInFile(m_vfdb_handle, SVN_PLN_GROUP_ID, SVT_PLN_GROUP_ID, f_protocol)) {

            VFF_SET_ARGS(vfReq[n], SVN_PLN_GROUP_ID, SVT_PLN_GROUP_ID, 0, NULL, VFO_DELETE);
            n++;

        }

        if (true == IsTagPresentInFile(m_vfdb_handle, SVN_PLN_MASTER_FLAG, SVT_PLN_MASTER_FLAG, f_protocol)) {

            VFF_SET_ARGS(vfReq[n], SVN_PLN_MASTER_FLAG, SVT_PLN_MASTER_FLAG, 0, NULL, VFO_DELETE);
            n++;

        }

        if (n) {
            time_t tmStamp = 0;
            status_t l_status = m_vfdb_handle->PutFields(path, SVD_ST_PROTOCOL, vfReq,
                                n, &num_put, &tmStamp);

            if (l_status != E_NO_ERROR || n != num_put) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
                l_status = E_ERROR ;
            }

            f_obj->SetGroupID(0);
            f_obj->SetMasterFlag(FALSE);
        }

        return TRUE;
    }

    return FALSE;
}
//*****************************Method Header**********************************
//Method Name    :GetTopWaitGateProt()
//Author         :
//Purpose        :
//****************************************************************************
BOOL CPQMStudy::GetTopWaitGateProt(
    CPqmProtocol** result_acqSeries
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetTopWaitGateProt");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetTopWaitGateProt"));


    CScopedLock l_scoped_lock(&m_protocol_vec_mutex);

    int position = 0;

    if (*result_acqSeries != NULL) {
        *result_acqSeries = NULL;
    }

    protocolvec::const_iterator v1_Iter = NULL;
    CPqmProtocol* protocol = NULL;

    for (v1_Iter = m_protocol_vec.begin(); v1_Iter != m_protocol_vec.end(); v1_Iter++) {
        position++;
        protocol = *v1_Iter;


        if ((protocol != NULL) && (protocol->GetAcqStatus() == C_PR_STATUS_WAIT) && (protocol->GetGatingMethod() != 0)) {
            *result_acqSeries = *v1_Iter;
            return TRUE;
            break;

        } else {
            continue;
        }
    }

    return FALSE;
}
status_t CPQMStudy::DeleteHALAFields(
    const int f_acq_order
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetAgentValueVF");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));
    VfPathElem_t l_pelm[1] = {0};
    time_t l_tm = 0;
    VfFieldSpec_t l_vf_req[5] = {0};


    memset(l_vf_req, NULL, sizeof(VfFieldSpec_t) * 5);

    int n = 0;
    VFF_SET_ARGS(l_vf_req[n], SVN_PLN_HALA_OFFSET_VEC_TABLE , SVT_PLN_HALA_OFFSET_VEC_TABLE, sizeof(f32vec3_t), NULL, VFO_DELETE);
    n++;

    VFF_SET_ARGS(l_vf_req[n], SVN_PLN_HALA_PHASE_VEC_TABLE , SVT_PLN_HALA_PHASE_VEC_TABLE, sizeof(f32vec3_t), NULL, VFO_DELETE);
    n++;

    VFF_SET_ARGS(l_vf_req[n], SVN_PLN_HALA_SLICE_VEC_TABLE , SVT_PLN_HALA_SLICE_VEC_TABLE, sizeof(f32vec3_t), NULL, VFO_DELETE);
    n++;

    VFF_SET_ARGS(l_vf_req[n], SVN_PLN_HALA_CALCULATED_STATUS , SVT_PLN_HALA_CALCULATED_STATUS, sizeof(int), NULL, VFO_DELETE);
    n++;

    VFF_SET_ARGS(l_vf_req[n], SVN_PLN_HALA_INPUT_PROTOCOL_INDEX , SVT_PLN_HALA_INPUT_PROTOCOL_INDEX, sizeof(int), NULL, VFO_DELETE);
    n++;

    int l_num = 0;

    status_t l_status = m_vfdb_handle->PutFields(l_pelm, SVD_ST_STUDY, l_vf_req,
                        n, &l_num, &l_tm);

    BITFLD_DECL(mask,   MAX_BITFLD) = {0};
    BITFLD_INIT(mask, K_ALL_PR, 0);
    memset(mask, 0, sizeof(mask));
    int l_hala_planned_status = VFC_CALA_PLANNED_STATUS_NOVECTOR;

    if (E_NO_ERROR != SetProtocolValues(m_vfdb_handle,
                                        f_acq_order, mask, K_PR_HALA_PLANNED_STATUS, &l_hala_planned_status,
                                        sizeof(l_hala_planned_status), NULL)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetProtocolValues() failed."));
        l_status = E_ERROR;

    } else {
        CPqmProtocol* l_prot = GetProtocol(f_acq_order);

        if (l_prot) {
            l_prot->SetHalaPlannedStatus(PLANNED_STATUS_NOVECTOR);
        }
    }

    return l_status;
}

//***************************Method Header*************************************
//Method Name    :GetTotalSAEValueFromVarfield()
//Author         :iGATE/AKR
//Purpose        :Read software total SAE value from varfield
//*****************************************************************************
status_t CPQMStudy::GetTotalSAEValueFromVarfield(CPqmProtocol* f_protocol, int32_t f_total_sae[])
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetTotalSAEValueFromVarfield");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL == f_protocol || NULL == m_vfdb_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_protocol or m_vfdb_handle is NULL"));
        return E_ERROR;
    }

    VfPathElem_t    l_pelm[2] = {0};
    VfFieldSpec_t   l_vf_required[NUM_1] = {0};

    l_pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_pelm[SVD_ST_PROTOCOL - 1].index = f_protocol->GetProtocol();

    int	l_num = 0, l_num_done = 0;

    VFF_SET_ARGS(l_vf_required[l_num], SVN_PLN_SAE_TOTAL,
                 SVT_PLN_SAE_TOTAL, sizeof(int32_t)*NUM_2, f_total_sae, NULL);

    l_num++;

    status_t l_status = m_vfdb_handle->GetFields(l_pelm, SVN_ST_PROTOCOL, l_vf_required, l_num, &l_num_done);

    if (E_NO_ERROR != l_status || l_num_done != l_num) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetFields is failed!!"));
        return E_ERROR;
    }

    return E_NO_ERROR;

}
//*****************************************************************************
//Method Name    :CheckCFAValueinScratchPad()
//Author         :PATNI
//Purpose        :
//*****************************************************************************
void CPQMStudy::CheckCFAValueinScratchPad(CPqmProtocol* f_current_prot, const int f_cfa_retry)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::WriteCFAValuetoCfaFile");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CheckCFAValueinScratchPad"));

    int l_shim_state = 0;
    CPqmShimSharedData::GetInstance()->GetShimState(&l_shim_state);

    if (l_shim_state != SHIM_STATE_ACTIVE) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Shimming is not active"));
        return;
    }

    int n = 0;
    int num = 0;
    double l_fsy = 0.0;
    time_t          tm = -1;

    CString l_msg = _T("");

    VfFieldSpec_t   l_vfreq[8] = {0};
    VfPathElem_t    l_new_path[SVD_ST_PROTOCOL] = {0};

    l_new_path[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_new_path[SVD_ST_PROTOCOL - 1].index = f_current_prot->GetProtocol();


    memset(l_vfreq, 0, sizeof(VfFieldSpec_t) * 8);

    VFF_SET_ARGS(l_vfreq[n], SVN_EXE_PRESCAN_FSY_VALUE, SVT_EXE_PRESCAN_FSY_VALUE, sizeof(l_fsy),
                 &l_fsy, VFO_ALLOCATE | VFO_VARIABLE_LEN);
    n++;

    status_t l_status = m_vfdb_handle->GetFields(l_new_path, SVD_ST_PROTOCOL, l_vfreq, n, &num);

    l_msg.Format(_T("FSY Value  = %f read from protocol index = %d"), l_fsy, f_current_prot->GetProtocol());

    if ((l_status == E_ERROR) || (n != num)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SVN_EXE_PRESCAN_FSY_VALUE value is not read from Study file"));
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_msg);

    if (l_fsy != 0.0) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("fys value is already in protocol"));
        return;
    }

    char*   path = NULL , buf[256] = {0}, file[256] = {0};

    if ((path = getenv("PRESCAN")) != NULL) {
        strcpy(file, path);

    } else {
        strcpy(file, ".");
    }

    if ((PQM_CFA_RETRY_AND_SCAN == f_cfa_retry) &&
        (FALSE == m_pqm_ptr->CanDisplayCFAGraph(f_current_prot))
       ) {
        strcat(buf, "CfDaily_2Slice.vfs ");
        strcat(file, "CfDaily_2Slice_Current.vfs");

    } else {
        strcat(buf, "CfDaily.vfs ");
        strcat(file, "CfDaily_Current.vfs");
    }

    CVarFieldHandle current_vf_handle(file);
    l_status = current_vf_handle.VuiOpen();

    if (E_NO_ERROR != l_status) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("current_vf_handle.VuiOpen() failed.!!"));
        return;
    }

    memset(l_new_path, 0, sizeof(VfPathElem_t) * SVD_ST_PROTOCOL);

    CPqmProtocol* l_cfa_protocol = GetCFStudyProt();

    if (l_cfa_protocol != NULL) {
        l_new_path[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
        l_new_path[SVD_ST_PROTOCOL - 1].index = l_cfa_protocol->GetProtocol();

        VfPathElem_t    l_cfa_path[SVD_ST_PROTOCOL] = {0};

        l_cfa_path[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
        l_cfa_path[SVD_ST_PROTOCOL - 1].index = 0; // Write CFA value to current protocol instead of CF scratch pad

        n = 0;
        num = 0;
        memset(l_vfreq, 0, sizeof(VfFieldSpec_t) * 8);

        VFF_SET_ARGS(l_vfreq[n], SVN_EXE_PRESCAN_FSY_VALUE, SVT_EXE_PRESCAN_FSY_VALUE, sizeof(l_fsy),
                     &l_fsy, VFO_VARIABLE_LEN);
        n++;

        l_status = m_vfdb_handle->GetFields(l_new_path, SVD_ST_PROTOCOL, l_vfreq, n, &num);

        if ((E_NO_ERROR == l_status) && (n == num)) {

            n = 0;
            num = 0;

            VFF_SET_ARGS(l_vfreq[n], SVN_EXE_PRESCAN_FSY_VALUE, SVT_EXE_PRESCAN_FSY_VALUE, sizeof(l_fsy),
                         &l_fsy, VFO_REPLACE);
            n++;
            l_status = current_vf_handle.PutFields(l_cfa_path, SVD_ST_PROTOCOL, l_vfreq, n, &num, &tm);

            if ((E_ERROR == l_status) && (n != num)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("current_vf_handle.PutFields() failed.!!"));
            }
        }
    }


}


void CPQMStudy::GetSequenceArchitecture(
    const int f_protocol, CString& f_seq_architecture
)const
{

    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetProtocolArchitecture");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, FUNC_NAME);

    if (NULL == m_vfdb_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_vfdb_handle pointer is NULL"));
        return ;
    }

    int				l_num_done = 0;
    int				l_status = E_ERROR;
    int				l_n = 0;
    VfFieldSpec_t	l_vf_req[NUM_1] = {0};

    VfPathElem_t    l_new_path[SVD_ST_PROTOCOL] = {0};

    l_new_path[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_new_path[SVD_ST_PROTOCOL - 1].index = f_protocol;

    char* l_sequence_architecture;

    VFF_SET_ARGS(l_vf_req[0], SVN_SEQ_ARCHITECTURE, SVT_SEQ_ARCHITECTURE, 0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);
    l_status = m_vfdb_handle->GetFields(l_new_path, SVD_ST_PROTOCOL, &l_vf_req[0], 1, &l_num_done);

    l_n++;

    if (!l_status && l_vf_req[0].buffer && E_NO_ERROR == l_status && (1 == l_num_done)) {
        l_sequence_architecture = (char*)(l_vf_req[0].buffer);
        f_seq_architecture = l_sequence_architecture;
    }
}


CString CPQMStudy::GetSequenceName(
    const int f_acq_order
)const
{

    VfPathElem_t    pelm[] = {
        {SVN_ST_PROTOCOL,       0},
        {SVN_ST_SUBPROTOCOL,    0}
    };

    char l_ident[60] = {0};
    VfFieldSpec_t       vf_req[1];
    memset(vf_req, 0, sizeof(VfFieldSpec_t));
    pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    pelm[SVD_ST_PROTOCOL - 1].index = f_acq_order;


    int cnt = 0;
    VFF_SET_ARGS(vf_req[0], SVN_PLN_TEMPLATE_SEQ, SVT_PLN_TEMPLATE_SEQ,
                 60, l_ident, VFO_ALLOCATE | VFO_VARIABLE_LEN);

    cnt++;

    int  num = 0;
    status_t l_status = m_vfdb_handle->GetFields(pelm, SVD_ST_PROTOCOL, vf_req, cnt, &num);

    //path = getenv("SEQDIR");
    //str = path ;
    CString str = l_ident;

    //free(vf_req[0].buffer);

    return str ;

}

CString CPQMStudy::GetDefaultPGUID(const int f_inscantype)
{

    switch (f_inscantype) {
        case VFC_INSCAN_MRS :
            //m_defaultpxmldatamgr->GetDefaultPGUID(f_inscantype)
            return _T("MRS_INITIAL_PREFERENCE_FOR_LEGACY_PROTOCOL");

        case VFC_INSCAN_PWI :
            return _T("PWI_INITIAL_PREFERENCE_FOR_LEGACY_PROTOCOL");

        case VFC_INSCAN_FMRI:
            return _T("fMRI_INITIAL_PREFERENCE_FOR_LEGACY_PROTOCOL");

        case VFC_INSCAN_DWI :
            return _T("DWI_INITIAL_PREFERENCE_FOR_LEGACY_PROTOCOL");

        case VFC_INSCAN_MR3D :
            return _T("MR3D_TPAS_ON_PROTOCOL");

        case VFC_INSCAN_NONE :
            return _T("NULL");

        default:
            return _T("");
    }

}

void CPQMStudy::SetPGUIDForProtocol(
    const int f_protocol_acq_order, CString& f_pguid)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetPGUIDForProtocol");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    BITFLD_DECL(mask, K_ALL_PR);
    memset(mask, 0, sizeof(mask));

    int inscan_preference_guid_length = 120 ;
    char l_inscan_preference_guid[120] = {0} ;

    if (NULL == l_inscan_preference_guid) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Memory allocation failed for PGUID"));
        return;
    }

    wcstombs(l_inscan_preference_guid, f_pguid.GetBuffer(f_pguid.GetLength()), f_pguid.GetLength());
    f_pguid.ReleaseBuffer();

    try {
        if (E_NO_ERROR != SetProtocolValues(m_vfdb_handle, f_protocol_acq_order, mask,
                                            K_PR_INSCAN_PREFERENCE_GUID,
                                            &l_inscan_preference_guid, inscan_preference_guid_length, NULL)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetProtocolValues() failed."));
        }

    } catch (...) {

        ASSERT(FALSE);
    }

}

void CPQMStudy::UpdateInscanFlag(const CPqmProtocol* f_pqmProtocol)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::UpdateInscanFlag");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL == f_pqmProtocol) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_PROCON, FUNC_NAME, _T("f_pqmProtocol pointer is NULL"));
        return;
    }

    int l_inscanflag = GetInscanFlagForProtocolIndex(f_pqmProtocol->GetProtocol());

    if (-1 == l_inscanflag) {
        //Non Inscan protocol
        return ;
    }

    //if inscan flag is present and not NONE
    if (l_inscanflag != VFC_INSCAN_NONE) {
        CString l_pguid = _T("");
        l_pguid = GetPGUIDForProtocol(f_pqmProtocol->GetProtocol());

        //if current PGUID is blank or different
        //then update the PGUID as per INSCAN flag with default PGUID
        if (l_pguid == _T("") || !CheckInscanPguidPrefix(l_pguid, l_inscanflag)) {


            CString l_defpguid = _T("");

            if (GetPqm()->IsPGUIDDataXmlFilePresent()) {
                l_defpguid =
                    GetPqm()->GetDefaultPGUIDDataManager()->GetPGUIDFromXML(l_inscanflag, f_pqmProtocol->GetProtocol(), this);

            } else {
                l_defpguid = GetDefaultPGUID(l_inscanflag);
            }

            SetPGUIDForProtocol(f_pqmProtocol->GetProtocol(), l_defpguid);
        }

    } else if (l_inscanflag == VFC_INSCAN_NONE) {
        //inscan flag is present and is NONE set the PGUID to NONE
        DeleteInscanPGUID(f_pqmProtocol->GetProtocol());
    }
}
bool CPQMStudy::CheckInscanPguidPrefix(const CString& f_pguid, const int l_inscanflag)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::CheckInscanPguidPrefix");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (f_pguid == _T("")) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PGUID is Blank"));
        return false;
    }

    int l_prefixpos = -1;
    CString l_pguidprefix = _T("");

    //get the INSCAN PGUID prefix
    l_prefixpos = f_pguid.Find(_T("_"));

    if (-1 == l_prefixpos) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PGUID Format is not proper"));
        return false;
    }

    l_pguidprefix = f_pguid.Left(l_prefixpos);

    switch (l_inscanflag) {
        case VFC_INSCAN_FMRI: {
            if (l_pguidprefix.CompareNoCase(_T("fmri")) != 0) {
                return false;
            }
        }
        break;

        case VFC_INSCAN_DWI: {
            if (l_pguidprefix.CompareNoCase(_T("dwi")) != 0) {
                return false;
            }
        }
        break;

        case VFC_INSCAN_PWI: {
            if (l_pguidprefix.CompareNoCase(_T("pwi")) != 0) {
                return false;
            }
        }
        break;

        case VFC_INSCAN_MRS: {
            if (l_pguidprefix.CompareNoCase(_T("mrs")) != 0) {
                return false;
            }
        }
        break;

        case VFC_INSCAN_CARDIAC: {
            if (l_pguidprefix.CompareNoCase(_T("cardiac")) != 0) {
                return false;
            }
        }
        break;

        case VFC_INSCAN_MR3D: {
            if (l_pguidprefix.CompareNoCase(_T("mr3d")) != 0) {
                return false;
            }
        }
        break;

        default:
            return false;
    }

    return true;
}
status_t CPQMStudy::DeleteInscanPGUID(
    const int f_acq_order
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::DeleteInscanPGUID");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));
    VfPathElem_t l_pelm[1] = {0};
    time_t l_tm = 0;
    VfFieldSpec_t l_vf_req[1] = {0};

    l_pelm[0].subtree_name = SVN_ST_PROTOCOL;
    l_pelm[0].index = f_acq_order;


    memset(l_vf_req, NULL, sizeof(VfFieldSpec_t));

    int n = 0;
    VFF_SET_ARGS(l_vf_req[n], SVN_PLN_INSCAN_PREFERENCE_GUID , SVT_PLN_INSCAN_PREFERENCE_GUID, sizeof(f32vec3_t), NULL, VFO_DELETE);
    n++;

    int l_num = 0;

    status_t l_status = m_vfdb_handle->PutFields(l_pelm, SVD_ST_PROTOCOL, l_vf_req,
                        n, &l_num, &l_tm);

    if (E_NO_ERROR != l_status || l_num != n) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
    }

    return l_status;
}

bool CPQMStudy::IsEditedInAcquirePage(
    const int f_protocol
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::IsEditedInAcquirePage");

    int l_edited_page = 0;

    VfFieldSpec_t l_vf_req[1] = {0};

    VfPathElem_t l_node_path[] = {
        {SVN_ST_PROTOCOL, 0}
    };

    memset(l_vf_req, 0, sizeof(VfFieldSpec_t) * 1);
    l_node_path[SVN_ST_PROTOCOL - 1].index = f_protocol;

    int l_n = 0, l_num = 0;
    VFF_SET_ARGS(l_vf_req[l_n],
                 SVN_PLN_CURRENT_PAGE,
                 SVT_PLN_CURRENT_PAGE,
                 sizeof(int),
                 &l_edited_page,
                 0);

    l_n++;

    status_t l_status = m_vfdb_handle->GetFields(l_node_path, SVD_ST_PROTOCOL, l_vf_req, l_n, &l_num);

    if ((l_status == E_NO_ERROR) && (l_num = 1)) {
        if ((l_edited_page == VFC_CURRENT_PAGE_ACQUIRE)) {
            return true;

        } else {
            return false;
        }

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetFields API Failed"));
        return false;
    }
}

bool CPQMStudy::CheckSlaveProtocolForDuplicate(int* f_selected_items, int f_count)
{

    for (int i = 0; i < f_count; i++) {
        int l_protocol_index = f_selected_items[i];
        CPqmProtocol*    l_from_obj = GetItemFromPosition(l_protocol_index + 1);
        ASSERT(l_from_obj != NULL);

        if (l_from_obj->GetAMBModeFlag() == VFC_AMB_MODE_SLAVE) {
            return true;
        }
    }

    return false;
}
//***************************Method Header*************************************
//Method Name    :AMBCopyScanModeFrom()
//Author         :iGATE
//Purpose        :
//*****************************************************************************

bool CPQMStudy::AMBCopyScanModeFrom(CVarFieldHandle*	const f_vfd,
                                    const int		from_protocol,
                                    const int		to_protocol,
                                    const bool_t	copyContrast)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::AMBCopyScanModeFrom");
    PQM_SCOPED_TRACE();

    CString l_trace_msg(_T(""));
    l_trace_msg .Format(_T("Inside CopyScanModeFrom, from_protocol = %d, to_protocol = %d, copyContrast = %d"), from_protocol, to_protocol, copyContrast);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              l_trace_msg);

    CCoil	l_coil_p;
    CCoil           l_coil;
    int		l_obj_trCoil, l_trCoil;
    CPqmPrMode	l_modeEntry;

    bool_t		l_enable;
    CPqmCoilData*    l_coil_data = CDataManager::GetInstance()->GetPQMCoilData();


    BITFLD_DECL(mask, MAX_NODES);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMStudy::CopyScanModeFrom"));

    CPqmProtocol* l_obj = GetItemFromProtocol(to_protocol);

    if (NULL == l_obj  || PQ_FILLER == l_obj->GetProtocol()) {
        return false;
    }

    memset(mask, 0, sizeof(mask));
    status_t l_status = GetProtocolValues
                        (m_vfdb_handle, l_obj->GetProtocol(), mask,
                         K_PR_TR_COIL,  &l_obj_trCoil, NULL,
                         NULL);


    CPqmProtocol* l_from_obj = GetItemFromProtocol(from_protocol);

    if (NULL == l_from_obj  || PQ_FILLER == l_from_obj->GetProtocol()) {
        return false;
    }

    l_modeEntry.Copy(l_from_obj->GetPqmPrMode());
    PqmSaturnCoil_t* l_saturnEntry = l_from_obj->GetSaturnCoil();

    if (NULL == l_saturnEntry) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_saturnEntry pointer is NULL"));
        return false;
    }

    int l_prot = l_from_obj->GetProtocol();

    if (m_pqm_ptr->IsSaturnCoilSystem()) {
        /* Saturn Coil */
        PqmCoil_t* l_tmp_coil = NULL;

        if (!GetTxCoilInSaturn(l_from_obj->GetCoilDBTable(), &l_tmp_coil)) {
            l_tmp_coil = NULL;
        }

        l_coil.SetCoil(l_tmp_coil);

    } else {
        if (NULL != l_coil_data) {
            l_coil_data->PqmCoilGetTxCoil(l_modeEntry.GetRecvCoilId(), l_coil_p);

        } else {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_coil_data pointer is NULL"));
        }
    }

    if (NULL != l_coil_p.GetCoil()) {
        l_trCoil = l_coil_p.GetCoilId();

    } else {
        l_trCoil = 0;
    }

    if (!copyContrast)
        l_modeEntry.SetContrast(l_obj->GetPqmPrMode()->GetContrast());

    if (m_pqm_ptr->IsSaturnCoilSystem()) {

        l_modeEntry.SetRecvCoilId(l_obj->GetPqmPrMode()->GetRecvCoilId());
        int l_return = memcmp(l_saturnEntry, l_obj->GetSaturnCoil(), sizeof(PqmSaturnCoil_t));
        l_enable = (l_modeEntry.Compare(l_obj->GetPqmPrMode())) ||
                   (l_trCoil != l_obj_trCoil) || l_return ;

    } else {
        l_enable = (l_modeEntry.Compare(l_obj->GetPqmPrMode()) || l_trCoil != l_obj_trCoil);
    }

    if (l_enable) {
        CPqmPrMode* copyToMode = l_obj->GetPqmPrMode();

        if (NULL == copyToMode) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("copyToMode points to Null."));
            return false;
        }

        copyToMode->SetAnatomy(l_modeEntry.GetAnatomy());
        CString tmpStr(_T(""));
        tmpStr.Format(_T("copyToMode->SetAnatomy(l_modeEntry.GetAnatomy()) = %d"), l_modeEntry.GetAnatomy());
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, tmpStr);
        BodyPartSetVF(l_obj->GetProtocol(), l_modeEntry.GetBodyPart());
        copyToMode->SetBodyPart(l_modeEntry.GetBodyPart());
        SaveLaterality(l_obj->GetProtocol(), l_modeEntry.GetLaterality());
        copyToMode->SetLaterality(l_modeEntry.GetLaterality());

        copyToMode->SetContrast(l_modeEntry.GetContrast());
        copyToMode->SetCouchOffset(l_modeEntry.GetCouchOffset());
        copyToMode->SetCouchPos(l_modeEntry.GetCouchPos());
        copyToMode->SetHomePos(l_modeEntry.GetHomePos());
        copyToMode->SetPatoriEndIn(l_modeEntry.GetPatoriEndIn());
        copyToMode->SetPatoriSideUp(l_modeEntry.GetPatoriSideUp());
        copyToMode->SetRecvCoilId(l_modeEntry.GetRecvCoilId());
        copyToMode->SetScanNumber(l_modeEntry.GetScanNumber());
        copyToMode->SetViewDir(l_modeEntry.GetViewDir());

        l_obj->SetSaturnCoil(l_saturnEntry);




        if (m_pqm_ptr->IsSaturnCoilSystem()) {
            /* Saturn Coil */
            int i;
            PqmSaturnRcvCoil_t* l_coil_db = l_obj->GetCoilDBTable();

            if (NULL == l_coil_db) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_coil_db points to Null."));
                return false;
            }

            for (i = 0; i < l_coil_db->numCoil; i++) {
                (l_obj->GetSarInfo())->receiverCoil[i]  = l_coil_db->coiltbl_p[0]->coil_id;
            }

        } else {

            (l_obj->GetSarInfo())->receiverCoil[0] = l_obj->GetPqmPrMode()->GetRecvCoilId();
        }

        l_obj->GetSarInfo()->transmiterCoil = l_trCoil;

        CString l_str(_T(""));
        l_str.Format(_T("rcvCoilId = %d\n l_trCoil    = %d\n contrast  = %d\n  couchPos  = %f\n anatomy  = %d\n view_dir  = %d\n "),
                     l_obj->GetPqmPrMode()->GetRecvCoilId(), l_trCoil, l_obj->GetPqmPrMode()->GetContrast(),
                     l_obj->GetPqmPrMode()->GetCouchPos(), l_obj->GetPqmPrMode()->GetAnatomy(), l_obj->GetPqmPrMode()->GetViewDir());
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);


        memset(mask, 0, sizeof(mask));

        CPqmPrMode* l_ptr_pqmmode = l_obj->GetPqmPrMode();

        if (NULL == l_ptr_pqmmode) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_ptr_pqmmode pointer is NULL"));
            return false;
        }

        int l_rcvCoilId = l_ptr_pqmmode->GetRecvCoilId();
        int l_lanatomy = l_ptr_pqmmode->GetAnatomy();
        int l_scanNumber = l_ptr_pqmmode->GetScanNumber();
        float l_couch_Pos = l_ptr_pqmmode->GetCouchPos();
        float l_home_Pos = l_ptr_pqmmode->GetHomePos();
        float l_couch_offset = l_ptr_pqmmode->GetCouchOffset();
        int l_contrast = l_ptr_pqmmode->GetContrast();
        int l_view_dir = l_ptr_pqmmode->GetViewDir();

        VftPatori_t l_patori;
        l_patori.side_up = l_ptr_pqmmode->GetPatoriSideUp();
        l_patori.end_in = l_ptr_pqmmode->GetPatoriEndIn();
        l_status = SetProtocolValues
                   (m_vfdb_handle, l_obj->GetProtocol(), mask,
                    K_PR_TR_COIL, &l_trCoil,      sizeof(int32_t),
                    K_PR_RC_COIL, &l_rcvCoilId,   sizeof(int32_t),
                    K_PR_ANATOMY, &l_lanatomy,    sizeof(int32_t),
                    K_PR_SCAN_NUMBER, &l_scanNumber,  sizeof(int32_t),
                    K_PR_COUCH,   &l_couch_Pos,   sizeof(flt32_t),
                    K_PR_HOME_POSITION, &l_home_Pos,  sizeof(flt32_t),
                    K_PR_COUCH_OFFSET,    &l_couch_offset,    sizeof(flt32_t),
                    K_PR_CONTRAST,    &l_contrast,    sizeof(int32_t),
                    K_PR_VIEW_DIR,    &l_view_dir,    sizeof(int32_t),
                    K_PR_PATIENT_ORIENT,  &(l_patori),    sizeof(VftPatori_t),
                    NULL);

        if (E_NO_ERROR != l_status) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetProtocolValues failed."));
        }

        BodyPartSetVF(l_obj->GetProtocol(), l_obj->GetPqmPrMode()->GetBodyPart());

        if (m_pqm_ptr->IsSaturnCoilSystem()) {

            int l_prot = l_obj->GetProtocol();

            PutSaturnCoil(m_vfdb_handle, l_prot, l_obj->GetSaturnCoil());

            PutConnectedCoil(m_vfdb_handle, l_prot, l_obj);

            if (E_NO_ERROR != l_coil_data->PqmAllCoilGetSaturn(l_obj->GetSaturnCoil(),
                    l_obj->GetCoilDBTable())) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PqmAllCoilGetSaturn failed"));
            }

            PutRecvrCoil(m_vfdb_handle, l_prot, l_obj);

            SetProtValues(m_vfdb_handle, l_prot, mask,
                          K_PR_RC_COIL_NAME,
                          (l_obj->GetSaturnCoil())->CoilName,
                          strlen((l_obj->GetSaturnCoil())->CoilName) + 1,
                          NULL);
            SetTxCoilInSaturn(f_vfd, l_obj, l_obj->GetCoilDBTable()) ;
        }

    }

    if (l_prot > -1 && copyContrast) {
        char		buf[1024];
        int		size;

        memset(mask, 0, sizeof(mask));
        l_status = GetProtocolValues
                   (m_vfdb_handle, l_prot, mask,
                    K_PR_CONTRAST_NAME, buf, &size,
                    NULL);

        if (E_NO_ERROR == l_status  && !BITFLD_ON(mask, K_PR_CONTRAST_NAME)) {
            memset(mask, 0, sizeof(mask));
            SetProtocolValues
            (f_vfd, l_obj->GetProtocol(), mask,
             K_PR_CONTRAST_NAME, buf, size,
             NULL);
        }

    }

    return true;
}
//*****************************************************************************
//Method Name    :GetRTSARTime()
//Author         :iGATE
//Purpose        : NFD010
//*****************************************************************************
int CPQMStudy::GetRTSARTime(const int& f_protocol,
                            SYSTEMTIME& f_rtsar_time
                           )const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::GetRTSARTime");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    VfPathElem_t    pelm[] = {
        {SVN_ST_PROTOCOL,       0},
        {SVN_ST_SUBPROTOCOL,    0}
    };

    char l_system_time[20] = {0};
    VfFieldSpec_t       vf_req[1];

    pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    pelm[SVD_ST_PROTOCOL - 1].index = f_protocol;


    status_t l_status;
    int cnt = 0;
    VFF_SET_ARGS(vf_req[0], SVN_EXE_N_RTSAR_END_POINT_TIME, SVT_EXE_N_RTSAR_END_POINT_TIME,
                 sizeof(l_system_time), &l_system_time, NULL);

    cnt++;

    if (cnt != 0) {
        int  num = 0;
        l_status = m_vfdb_handle->GetFields(pelm, SVD_ST_PROTOCOL, vf_req, cnt, &num);

        if (cnt != num) {
            PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("GetFields() Failed."));
            return E_ERROR ;
        }
    }

    if (l_status != E_NO_ERROR) {

        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("PutFields()Failed."));
        return E_ERROR ;
    }

    f_rtsar_time = CPqmUtilities::ConvertLocalTimeStringToSystemTime(l_system_time);

    return E_NO_ERROR;
}



//*****************************************************************************
//Method Name    : UpdateScratchPadForSARControl()
//Author         : iGATE
//Purpose        :
//*****************************************************************************
void CPQMStudy::UpdateScratchPadForSARControl(const char* f_tmp_file,
        const SCRATCHPAD_CREATION_MODE f_sp_creation_mode)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::UpdateScratchPadForSARControl");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL == f_tmp_file) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_tmp_file is NULL"));
        return;
    }

    CVarFieldHandle l_vf_handle((CString)f_tmp_file);


    BOOL		l_sar_control_flag = FALSE;
    int32_t		l_sar_control_limit[NUM_2] = {100, 0};

    if (SARCTRL_LICENCE_ON == m_pqm_ptr->GetSARControlLicense() &&
        !m_pqm_ptr->IsSARControlLimitFileExist()) {

        switch (f_sp_creation_mode) {
            case SP_CREATE_MODE_WSE:
                l_sar_control_limit[0] = 200 - SAR_LIMIT_10SEC_MARGIN;
                l_sar_control_flag  = TRUE;
                break;

            case SP_CREATE_MODE_WSE_AMB:
            case SP_CREATE_MODE_WSE_MRS:
                l_sar_control_limit[0] = 100;
                l_sar_control_flag  = TRUE;
                break;

            case SP_CREATE_MODE_PRIVATE_PROLIM:
                l_sar_control_limit[0] = 200 - SAR_LIMIT_10SEC_MARGIN;
                l_sar_control_flag  = FALSE;
                break;

            case SP_CREATE_MODE_PRIVATE_PROLIM_MSD:
                l_sar_control_limit[0] = 200 - SAR_LIMIT_10SEC_MARGIN;
                l_sar_control_flag  = FALSE;
                break;

            case OTHER:
                return;

        }

    }

    status_t l_status = l_vf_handle.VuiOpen();

    if (E_NO_ERROR != l_status) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_vf_handle.VuiOpen() failed.!!"));
        return ;
    }

    BITFLD_DECL(mask, K_ALL_PR);
    memset(mask, 0, sizeof(mask));

    if (E_NO_ERROR != SetProtocolValues(&l_vf_handle, 0, mask,
                                        K_PR_SAR_RATIO_PROLIM_LIMIT,	&l_sar_control_limit,	sizeof(l_sar_control_limit),
                                        K_PR_RTSAR_CTRL_FLAG,			&l_sar_control_flag,	sizeof(l_sar_control_flag),
                                        NULL)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetProtocolValues() failed"));

        l_vf_handle.VuiClose();
        return;
    }

    if (SP_CREATE_MODE_PRIVATE_PROLIM_MSD == f_sp_creation_mode) {

        memset(mask, 0, sizeof(mask));

        BOOL		l_calc_sar_flag		= FALSE;

        SetProtocolValues(&l_vf_handle, 0, mask,
                          K_PR_CALC_SAR_FLAG,			&l_calc_sar_flag,	sizeof(l_calc_sar_flag),
                          NULL);
    }

    l_vf_handle.VuiClose();
}



//*****************************************************************************
//Method Name    : UpdateFixedOpeModeForSARControl
//Author         : iGATE
//Purpose        :
//*****************************************************************************
void  CPQMStudy::UpdateFixedOpeModeForSARControl(CPqmProtocol* f_protocol_obj)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::UpdateFixedOpeModeForSARControl");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));


    if (NULL == f_protocol_obj) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_protocol_obj is NULL."));
        return;
    }

    int32_t l_fixed_ope_mode = 0;

    ClibSARAdapter sarAdapter(m_vfdb_handle);
    sarAdapter.SarGetVfRTSARFixedOpeMode(f_protocol_obj->GetProtocol(), &l_fixed_ope_mode);

    f_protocol_obj->GetSarInfo()->fixed_ope_mode = l_fixed_ope_mode;
}

//*****************************************************************************
//Method Name    : GetSarControlParamStoreSarInfoFor()
//Author         : iGATE/Nilesh
//Purpose        :
//*****************************************************************************
const CString CPQMStudy::GetSarControlParamStoreSarInfoFor(CPqmProtocol* f_protocol_obj,
        const sarType_t f_sar_control_type, const bool f_is_scan_completed) const
{
    CString l_str(_T("----------"));
    sarInfo_t* l_sar_info = f_protocol_obj->GetSarInfo();

    if (l_sar_info && l_sar_info->sarControl.tsarControl[f_sar_control_type].sar_6min > 0.000001) {
        l_str.Format(_T("%0.3f W/kg"), l_sar_info->sarControl.tsarControl[f_sar_control_type].sar_6min);

        //for 6m ope_mode
        const int32_t   l_sar_ctrl_6m_ope_mode = l_sar_info->sarControl.tsarControl[f_sar_control_type].ope_mode;

        //for 10s ope_mode
        const int32_t   l_sar_ctrl_10sec_ope_mode = l_sar_info->sarControl.tsarControl[f_sar_control_type].ope_mode_short;


        int32_t l_sar_mode = IEC_SAR_NORMAL_MODE;

        if (f_is_scan_completed &&
            (f_sar_control_type == l_sar_info->sarControl.active)) {

            l_sar_mode = l_sar_info->sarControl.ope_mode;

        } else if (l_sar_ctrl_6m_ope_mode == IEC_SAR_OVER || l_sar_ctrl_10sec_ope_mode == IEC_SAR_OVER) {
            l_sar_mode = IEC_SAR_OVER;

        } else if (l_sar_ctrl_6m_ope_mode == IEC_SAR_1ST_MODE || l_sar_ctrl_10sec_ope_mode == IEC_SAR_1ST_MODE) {
            l_sar_mode = IEC_SAR_1ST_MODE;
        }

        l_str += CPQMSAR::FindSarMode(l_sar_mode);

        if (f_sar_control_type == l_sar_info->sarControl.active) {
            l_str += _T("*");
        }
    }

    return l_str;
}

//****************************Method Header************************************

//Method Name   :SaveTagsBeforeScratchpad()

//Author        :PATNI /SP

//Purpose       : This function is used to save some SVN tags from map sturcture to study in case of private prolim

//*****************************************************************************
status_t CPQMStudy::SaveTagsBeforeScratchpad(
    const int f_acq_order
)
{
    LPCTSTR       FUNC_NAME = _T("CPQMStudy::SaveTagsBeforeScratchpad");

    VfFieldSpec_t*      l_vfreq = new VfFieldSpec_t[g_update_saved_tags];
    VfPathElem_t        l_pelm[1] = {{SVN_ST_PROTOCOL, 0}};

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SaveTagsBeforeScratchpad"));
    //Initialize VfFieldSpec_t array

    memset(l_vfreq, 0, sizeof(VfFieldSpec_t) * g_update_saved_tags);
    //set the protocol index.
    l_pelm[0].index = f_acq_order;


    for (int l_num = 0; l_num < g_update_saved_tags; l_num++) {
        VFF_SET_ARGS(l_vfreq[l_num], updatesavedtags[l_num].name, updatesavedtags[l_num].type, 0,
                     NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);
    }

    int l_numdone = 0;
    status_t l_status = m_vfdb_handle->GetFields(l_pelm, SVD_ST_PROTOCOL, l_vfreq, l_num, &l_numdone);

    //set the field value and size of the tags in the gui control structure

    for (l_num = 0; l_num < g_update_saved_tags; l_num++) {

        if (l_vfreq[l_num].status == VFE_NO_ERROR) {
            updatesavedtags[l_num].ctrl_value = l_vfreq[l_num].buffer;
            updatesavedtags[l_num].ctrl_size = l_vfreq[l_num].size;

        } else {
            updatesavedtags[l_num].ctrl_value = 0;
            updatesavedtags[l_num].ctrl_size = 0;
        }
    }

    DEL_PTR_ARY(l_vfreq);

    return l_status;
}

//*******************************************************************************
//Method Name   :  RestoreSavedTagsAfterScratchpad
//Author        :  PATNI/SP
//Purpose       :  This function is used to restore the values of the some SVN tags from map sturcture to study
//                 after copying from scratchpad in private prolim cases.
//*******************************************************************************
status_t CPQMStudy::RestoreSavedTagsAfterScratchpad(
    const int f_acq_order
)
{
    LPCTSTR		FUNC_NAME = _T("CPQMStudy::RestoreSavedTagsAfterScratchpad");

    int             l_num = 0;
    int             l_count = 0;

    VfFieldSpec_t*  l_vfreq = new VfFieldSpec_t[g_update_saved_tags];//Patni-Rajendra/2011Mar10/modified/TMSC-Review-Comments
    VfPathElem_t    l_pelm[1] = {{SVN_ST_PROTOCOL, 0}};

    memset(l_vfreq, 0, sizeof(VfFieldSpec_t) * g_update_saved_tags);//Patni-Rajendra/2011Mar10/modified/TMSC-Review-Comments
    l_pelm[0].index = f_acq_order;
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("Trace of RestoreSavedTagsAfterScratchpad"));


    //Initializing VfFieldSpec_t array to be filled with values from updateCtrls struct
    for (l_count = l_num = 0; l_count < g_update_saved_tags; l_count++) {//Patni-Rajendra/2011Mar10/modified/TMSC-Review-Comments
        if ((updatesavedtags[l_count].ctrl_value != NULL) && (updatesavedtags[l_count].ctrl_size != 0)) {

            VFF_SET_ARGS(l_vfreq[l_num], updatesavedtags[l_count].name,
                         updatesavedtags[l_count].type, updatesavedtags[l_count].ctrl_size,
                         updatesavedtags[l_count].ctrl_value, VFO_REPLACE);
            l_num++;

        }
    }

    status_t l_status = E_NO_ERROR;

    //Restoring VfFieldSpec_t value in the varfield
    if (l_num) {
        int l_numDone = 0;
        time_t ts = 0L;
        l_status = m_vfdb_handle->PutFields(l_pelm,
                                            SVD_ST_PROTOCOL, l_vfreq, l_num, (int*) & l_numDone, &ts);
    }

    DEL_PTR_ARY(l_vfreq);

    return l_status;
}
//Anulekha
int CPQMStudy::GetGDCSeriesLOIDForProtocol(int protocol, VARIANT *seriesloid)
{
	try {
	if (seriesloid == NULL) return 1;
	int gdcseriesno = 0; //prot traverse and get this
	status_t l_nstatus = E_NO_ERROR;
	int l_nret = 0;
	int l_ndataKind = 1;
	
	VfPathElem_t    l_ImgPath[4] = {
		{SVN_ST_PROTOCOL,   0},
		{SVN_ST_PROCESSED,  0},
		{SVN_ST_ECHO,       0},
		{SVN_ST_POSITION,   0}
	};
	int l_nTotProcessNodes = MAX_BITFLD;
	int l_nProcessCnt;
	
    VfPathElem_t    vf_dst_path[MAX_DEPTH] = {0};
    memset(vf_dst_path , 0 , sizeof(VfPathElem_t) * MAX_DEPTH);

    BITFLD_DECL(mask,   MAX_BITFLD);

    if (NULL == m_vfdb_handle) {
        return NULL;
    }

    vf_dst_path[0].subtree_name = SVN_ST_PROTOCOL;
    vf_dst_path[0].index		= protocol;

    int l_total = MAX_BITFLD, l_active = 0;
    BITFLD_INIT(mask, MAX_BITFLD, 0);

    l_nstatus = m_vfdb_handle->ScanSubtree(vf_dst_path, SVD_ST_PROTOCOL,
                       SVN_ST_PROCESSED, mask, &l_nTotProcessNodes, &l_active);

	if(E_NO_ERROR != l_nstatus || 0 == l_nTotProcessNodes) {
		return(E_ERROR);
	}
	
	for(l_nProcessCnt = 0; l_nProcessCnt < l_nTotProcessNodes; l_nProcessCnt++) {
		if (!BITFLD_ON(mask, l_nProcessCnt)) {
			continue;
		}
		l_ImgPath[0].index = protocol;
		l_ImgPath[0].subtree_name = SVN_ST_PROTOCOL;
		l_ImgPath[1].subtree_name = SVN_ST_PROCESSED;
		l_ImgPath[1].index = l_nProcessCnt;
		l_ImgPath[2].index = 0;
		l_ImgPath[3].index = 0;
		
		VfFieldSpec_t   l_vfreq[2];
		
		int l_nCnt = 0,l_num  =0;
		l_vfreq[l_nCnt].status = E_NO_ERROR;
		VFF_SET_ARGS(l_vfreq[l_nCnt], SVN_PRC_DATA_KIND, SVT_PRC_DATA_KIND,
			sizeof(l_ndataKind), &l_ndataKind, 0);
		l_nCnt++;
		l_nstatus  = m_vfdb_handle->GetFields(l_ImgPath, SVD_ST_PROCESSED, l_vfreq, l_nCnt, &l_num);
		if(E_NO_ERROR != l_nstatus) {
			CString l_csErrMsg = _T("Error occured while getting Data kind");
			// CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
			return l_nstatus;
		}
		
		if(VFC_PROC_RECON != l_ndataKind) {
		/* printf("data kind : %d for path : [%d.%d/%d.%d]\n",
		l_ndataKind,
		l_ImgPath[0].subtree_name,
		l_ImgPath[0].index,
		l_ImgPath[1].subtree_name,
            l_ImgPath[1].index);*/
			continue;
		}
		gdcseriesno = l_nProcessCnt;
		break;
	}
				
	seriesloid->bstrVal = (GetMappedIDForProcessedNode(protocol,gdcseriesno)).AllocSysString();
	} catch (...)
	{
		return E_ERROR;
	}
	return 0;
}

/*************************************************************
* Method:    	IsProtocolForTiPrep
* Description: 	This function is used 
* Parameter: 	int f_Protocol_index :
* Returns:   	bool
*               Function returns false in following conditions:
*               1. protocol is not scanned
*               2. recon is not performed on the protocol
*               3. protocol is not tiprep sequence
*************************************************************/
bool CPQMStudy::IsProtocolForTiPrep(int f_acq_order)
{
    //check whether protocol is scanned
    // check whether recon is done on selected protocol
    CPqmProtocol* l_protocol = m_pqm_ptr->GetStudy()->GetProtocol(f_acq_order);

    if (!l_protocol) {
        /*PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("protcol object is NULL"));*/
        return(false);
    }
    
    if (l_protocol->GetHasImage()) {
        if (/*(l_protocol->IsProtocolCALAEnable() == VFC_CALA_ENABLE_FLAG_ON) &&*/
            (l_protocol->GetAcqStatus() != C_PR_STATUS_DONE)) {
            return(false);
        }
    }else{
        return(false);
    }    

    //check sequence
    int l_Protocol_index = l_protocol->GetProtocol();
    VftGatingParam_s l_gating_param;
    VfPathElem_t l_gatepath[1] = {0};
    VfFieldSpec_t l_gatefieldSpec[1] = {0};
    int l_gatenum = 0, l_gatenumDone = 0;
    
    l_gatepath[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_gatepath[SVD_ST_PROTOCOL - 1].index = l_Protocol_index;
    l_gatepath[SVD_ST_SUBPROTOCOL - 1].subtree_name = SVN_ST_SUBPROTOCOL;
    l_gatepath[SVD_ST_SUBPROTOCOL - 1].index = 0;
    
    
    VFF_SET_ARGS(l_gatefieldSpec[l_gatenum], SVN_PLN_GATE_PARAM, SVT_PLN_GATE_PARAM,
        sizeof(l_gating_param), &(l_gating_param), 0);
    l_gatenum++;
    
    if (NULL == m_vfdb_handle) {
        return(false);
    } 

    status_t l_nstatus = m_vfdb_handle->GetFields(l_gatepath, SVD_ST_PROTOCOL, l_gatefieldSpec, l_gatenum, &l_gatenumDone);
    
    if ((E_NO_ERROR != l_nstatus) || (l_gatenum != l_gatenumDone)) {
        /*PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("VfGetFields failed to get fields from varfield"));*/
        return(false);
    }

    if(l_gating_param.gatingMode != VFC_GAT_MODE_TI_PREP){
        return(false);
    }

    return(true);
}

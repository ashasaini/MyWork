//REDMINE-1213
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
 *  File Name: scantransaction.cpp
 *  Overview: Implementation of CScanTransaction class.
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
#include "ScanTransaction.h"
#include <math.h>
#include <tami/vfStudy/vfs_plan.h>
//MRPdefect122
extern "C" {
#include <tami/libAcq/SaturnCoilCombinChk.h>
}
//MRPdefect122
#include <ErrorDisp/ErrorDispConsts.h>
#include <libStudyManager/strcnv.h>
#include <PQM/PQMLogMgr.h>
#include <PQM/pqmcommon.h>
#include <PQM/PqmUtilities.h>
#include <PQM/resource.h>
#include "pqm.h"
#include "PqmPmAcqman.h"
#include "PqmPrMode.h"
#include "scanprocedure.h"
#include "voicemanager.h"
//Patni-DKH/2010Feb18/Added/Phase3#MCM0229-00187
#include "scan.h"
#include "PqmCoilData.h"
#include "PqmSectionData.h"
#include "PQMSAR.h"
#include "PqmSpeeder.h"
#include "PQMdBdt.h"
#include "PQMSocket.h"
#include "PQMStudyMgr.h"
#include "couchconfig.h"


//+Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
#include "CDSUtilities.h"
#include "OlpTemp.h"			//+Patni-Ravindra Acharya/2010July09/Added/IR-153
//-Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
#include "VarFieldHandle.h"
#include "GPLibAdapter.h"
#include "PQMDebugInfo.h"
#include "pqmAMBConstants.h"
#include "pqmAMB.h"
#include "PqmPmSARManager.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace MPlus::EH;
using namespace MR::ACQ::SM::COMMON;

//***************************Method Header*************************************
//Method Name    : CScanTransaction()
//Author         : PATNI/MRP
//Purpose        : Construction
//*****************************************************************************
CScanTransaction::CScanTransaction(CScanProcedure* scan_proc):
    m_check_bit(0),
    m_check_str(_T("")),
    m_couch_position(0.0f),
    m_couch_status(0),
    m_acq_info_retry(0),
    m_fsyvalue(0.0f),
    m_scan_procedure(scan_proc),
    m_lastfunction(0)
{
    LPCTSTR FUNC_NAME = _T("CScanTransaction::CScanTransaction");
    PQM_TRACE1(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of CScanTransaction::CScanTransaction"));
}

//***************************Method Header*************************************
//Method Name    : ~CScanTransaction()
//Author         : PATNI/MRP
//Purpose        : Destruction
//*****************************************************************************
CScanTransaction::~CScanTransaction(
)
{
    LPCTSTR FUNC_NAME = _T("CScanTransaction::~CScanTransaction");
    PQM_TRACE1(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of CScanTransaction::~CScanTransaction"));
    m_scan_procedure = NULL;
}

//****************************Method Header************************************
//Method Name   :SendControl()
//Author        :PATNI/HAR-SS
//Purpose       :
//*****************************************************************************
bool CScanTransaction::SendControl(
    const int control_kind
)
{
    LPCTSTR FUNC_NAME = _T("CScanTransaction::SendControl");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SendControl"));

    CScanProcedure* scan_proc = GetScanProcedure();

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (scan_proc == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("scan_proc pointer is NULL"));
        return false;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    CPqmPmAcqman* acqman = scan_proc->GetAcqManager();

    // Set function
    m_header.function = AMCMM_CONTROL;

    m_data.param.control.kind = (AM_ControlKind_t)control_kind;
    scan_proc->SetControlKind(control_kind);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              control_kind == ACQ_ABORT_BUTTON ? L"ABORT" :
              control_kind == ACQ_START_BUTTON ? L"START" :
              control_kind == ACQ_PAUSE_BUTTON ? L"PAUSE" :
              control_kind == ACQ_RESUME_BUTTON ? L"RESUME" :
              L"unknown");

    bool result = true;

    if (!acqman->SendMessage(this)) {

        result = false;
    }

    return result;
}

//****************************Method Header************************************
//Method Name   : AcqCheckSaturnCoil()
//Author        : PATNI\HAR
//Purpose       : Coiled coil connected to compare and choose whether or imaging
//				  Returned. Port coil connected to the information reflected in
//				  the selection coil.
//*****************************************************************************
int CScanTransaction::AcqCheckSaturnCoil(
    VftCoilListInfo_t* f_coil_connect,
    PqmSaturnCoil_t* f_saturn_coil,
    bool_t f_change_under_prot
)
{
    LPCTSTR FUNC_NAME = _T("CScanTransaction::AcqCheckSaturnCoil");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections Need to Check
    if (m_scan_procedure == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_scan_procedure pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    CPqm*			l_pqm_ptr = m_scan_procedure->GetPqmPtr();
    CDataManager*	l_data_mgr_ptr = l_pqm_ptr->GetDataManager();
    coilSelect_t	l_coil_select;

    int				l_recvr_sec_flg[MAX_SELECT_COIL] = {0}; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
    int				l_list_flg[MAXIMUM_COIL_IN_PORT] = {0}; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
    int				l_concect_flg[MAXIMUM_COIL_IN_PORT] = {0}; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
    int				l_combine[MAXIMUM_COIL_IN_PORT] = {0}; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
    //PqmCoil_t		*l_coil;
    CCoil			l_saturn_coil;
    bool            l_ret = false;
    //DB_FUNC_ENTER("[PqmAcqCheckSaturnCoil]:");

    // Initialize
    memset(l_concect_flg, 0, sizeof(l_concect_flg));
    memset(l_recvr_sec_flg, 0, sizeof(l_recvr_sec_flg));
    memset(l_list_flg, 0, sizeof(l_list_flg));
    memset(l_combine, 0, sizeof(l_combine));
    memset(&l_coil_select, 0, sizeof(coilSelect_t));
    bzero(&l_saturn_coil, sizeof(CCoil));

    if (f_coil_connect->num == 0) {
        return ERR_NO_COILDATA;
    }

    // One misses (Spine) check
    // GroupId coil is defined in the search for the error if they can not find one

    CString l_error_string(_T(""));
    CString l_str(_T(""));
    CString temp_str(_T(""));
    int l_size  = 0;

    for (int i = 0; i < f_coil_connect->num; i++) {

        l_ret = l_data_mgr_ptr->GetPQMCoilData()->PqmCoilidGetSaturnCoil(
                    f_coil_connect->coillist[i].coilID,
                    f_coil_connect->coillist[i].portID[0],
                    NULL,
                    l_saturn_coil);

        if (l_saturn_coil.GetCoil() != NULL) {
            PqmCoil_t*	l_coil_p = l_saturn_coil.GetCoil();//Patni-MJC/2009Aug17/Modified/cpp test corrections

            //Patni-ARD/20090226/Added/ACE-1# Enhance the withstand load of couch Start
            if (l_pqm_ptr->GetStudy()->GetWeight() > l_coil_p->weight) {
                char wt[6] = "";	//Patni-MJC/2009Aug17/Modified/cpp test corrections
                sprintf(wt, "%d", l_coil_p->weight);

                //	l_error_string.LoadString(ERR_OVER_COILWEIGHT);														// Added By KT/ACE-2/15-06-2009/Multilingualisation
                l_error_string = CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_OVER_COILWEIGHT"));				// Added By KT/ACE-2/15-06-2009/Multilingualisation

                wchar_t* l_weight = NULL;
                l_size = sizeof(wt) + 1;
                l_weight = new wchar_t[l_size];

                mbstowcs(l_weight, wt, l_size);
                // Added By KT/ACE-2/15-06-2009/Multilingualisation
                temp_str = CPqmUtilities::GetMultiLingualString(_T("IDS_KG_STRING"));								// Added By KT/ACE-2/15-06-2009/Multilingualisation

                // l_str.Format(l_error_string, l_weight);																// Added By KT/ACE-2/15-06-2009/Multilingualisation
                l_str.Format(_T("%s %s %s"), l_error_string, l_weight, temp_str);										// Added By KT/ACE-2/15-06-2009/Multilingualisation

                CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
                    l_str,
                    MPlus::EH::Crucial,//not confirm for categary of error
                    _T("PQM"),
                    _T("PQM"));

                DEL_PTR_ARY(l_weight);
                //PqmUiErrorCat(PqmUiFetchLiteral("ERR_OVER_COILWEIGHT"),ERR_ERROR, wt);
                return ERR_OVER_COILWEIGHT;
            }

            //Patni-ARD/20090226/Added/ACE-1# Enhance the withstand load of couch End

            //+Patni-Hemant/2009Mar02/Added/ACE-1/Venus Coil
            int	portbit = -1;

            /* ポートチェック V9.30 */
            if (f_coil_connect->coillist[i].portID[0] == CDB_PORT_G) {	/* Aポート */

                //Modifying the vantage code..
                //portbit = 1 << (atoi(&f_coil_connect->coillist[i].portID[1]) - 1);
                char value = f_coil_connect->coillist[i].portID[1];
                portbit = 1 << (UTIL::ConvertCharToInt(value) - 1);
                //Patni-AMT/2009Oct14/Added/MSA0248-00051-Coil Log
                l_str.Format(_T("coil_id[%d]=%d,port_id=G[%d],portbit=%d"), i,
                             f_coil_connect->coillist[i].coilID, UTIL::ConvertCharToInt(value), portbit);


            } else if (f_coil_connect->coillist[i].portID[0] == CDB_PORT_F) {	/* Gポート */
                portbit = 1 << 7;
                //Patni-AMT/2009Oct14/Added/MSA0248-00051-Coil Log
                l_str.Format(_T("coil_id[%d]=%d,port_id=F,portbit=%d"), i,
                             f_coil_connect->coillist[i].coilID, portbit);
            }

            //Patni-AMT/2009Oct14/Added/MSA0248-00051-Coil Log
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);

            if ((f_coil_connect->coillist[i].portID[0] == CDB_PORT_G)
                || (f_coil_connect->coillist[i].portID[0] == CDB_PORT_F)) {
                PqmCoil_t* l_temp_saturn_coil = l_saturn_coil.GetCoil();

                if ((l_temp_saturn_coil->portDef & portbit) == NULL) {
                    //Patni-AMT/2009Oct14/Added/MSA0248-00051-Coil Log
                    l_str.Format(_T("portDef=%d,portBit=%d") ,
                                 l_temp_saturn_coil->portDef, portbit, l_temp_saturn_coil->coil_id);
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);

                    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
                    char l_out_string[512] = {'\0'}, l_port_string[128] = {'\0'};
                    char l_error_load_string[512] = {'\0'};
                    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

                    bzero(l_out_string, sizeof(l_out_string));
                    bzero(l_port_string, sizeof(l_port_string));
                    bzero(l_error_load_string, sizeof(l_error_load_string));

                    l_error_string =  CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_DIFFERENT_PORT"));

                    //+Patni-HAR/2009Aug31/Modified/Corrected call for wcstombs()
                    UTIL::Wcs2Mbs(l_error_load_string, l_error_string, 511);

                    CCoil l_saturn_coil_2;
                    l_data_mgr_ptr->GetPQMCoilData()->PqmCoilidGetSaturnCoil(f_coil_connect->coillist[i].coilID, f_coil_connect->coillist[i].portID[0], NULL, l_saturn_coil_2);

                    if (l_saturn_coil_2.GetCoil()) {

                        CPqmUtilities::PQMUiStringCat(l_error_load_string, l_saturn_coil_2.GetCoil()->label , l_out_string);

                        PQMAcqPortString(l_temp_saturn_coil, l_port_string);
                        //Patni-PJS/2010Mar26/Added/DeFT# MVC006228
                        //CPqmUtilities::PQMUiErrorCat(l_out_string, l_port_string, _T("scantransaction.cpp");
                        CPqmUtilities::PQMUiErrorCat(l_out_string, l_port_string, _T("scantransaction.cpp"), MPlus::EH::Information);

                        return ERR_OFF_GROUP;
                    }
                }
            }

            //-Patni-Hemant/2009Mar02/Added/ACE-1/Venus Coil
            for (int j = 0; j < 4; j++) {	//GroupID to search

                if (l_saturn_coil.GetCoil()->groupId[j] == 0xffff) {	//Group is no end
                    break;
                }

                for (int k = 0; k < f_coil_connect->num; k++) {

                    if (l_saturn_coil.GetCoil()->groupId[j] ==
                        f_coil_connect->coillist[k].coilID) {
                        break;
                    }
                }

                if (k == f_coil_connect->num) {
                    // NEED_MODI Error Display required.
                    // + SM4 Himanshu COIL_DEFECT_PQM_HANG 20090126

                    l_error_string =  CPqmUtilities::GetMultiLingualString(_T("IDS_THE_STRING"));			// Added By KT/ACE-2/15-06-2009/Multilingualisation

                    temp_str =  CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_OFF_GROUP_COIL"));		// Added By KT/ACE-2/15-06-2009/Multilingualisation

                    l_str.Format(_T("%s %s %s."), l_error_string, l_saturn_coil.GetLabel(), temp_str);		// Added By KT/ACE-2/15-06-2009/Multilingualisation

                    //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
                    CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(l_str,
                            MPlus::EH::Information, _T("ScanTransaction.cpp"),
                            _T("PQM"));
                    // - SM4 Himanshu COIL_DEFECT_PQM_HANG 20090126
                    //                    PqmUiErrorCat(PqmUiFetchLiteral("ERR_OFF_GROUP_COIL"),ERR_ERROR,
                    //					PqmCoilidGetSaturnCoil(coil_p->groupId[0], coilconnect->coillist[i].portID[0], NULL)->label);
                    return ERR_OFF_GROUP;
                }
            }
        }
    }

    // Use multiple search coil (select side coil )
    // Multiple coil in the list selected in the search coils and
    //	multiple selected in the coil connection information
    // Put up a flag.
    //Coil connection list is unable to connect multiple coils and when the error.

    for (i = 0; i < f_saturn_coil->numSection; i++) {

        for (int j = 0; j < f_saturn_coil->CoilList.num; j++) {

            if (f_saturn_coil->RcvCoil[i].coilID ==
                f_saturn_coil->CoilList.coillist[j].coilID &&
                f_saturn_coil->RcvCoil[i].portID[0] ==
                f_saturn_coil->CoilList.coillist[j].portID[0]) {

                l_ret = l_data_mgr_ptr->GetPQMCoilData()->
                        PqmCoilidGetSaturnCoil(
                            f_saturn_coil->RcvCoil[i].coilID,
                            f_saturn_coil->RcvCoil[i].portID[0], NULL, l_saturn_coil);

                l_recvr_sec_flg[i]++;

                if (l_recvr_sec_flg[i] >= 2 && l_saturn_coil.GetCoil()->numSCoil <= 1) {
                    //Connect multiple coil coil is not connected to the
                    // multiple was an error
                    return ERROR_RESELECT;
                }
            }
        }

        if (l_recvr_sec_flg[i] == 0) {

            //ERROR_LOG("Contradiction is caused in RcvCoil and coillist. ");
            //sprintf(msg, "coilID = %x portID = %s",saturnCoil->RcvCoil[i].coilID, saturnCoil->RcvCoil[i].portID);
            //ERROR_LOG(msg);
            return ERR_COILDATA;
        }
    }

    for (i = 0; i < f_saturn_coil->CoilList.num; i++) {

        for (int j = 0; j < f_saturn_coil->CoilList.num; j++) {

            if (f_saturn_coil->CoilList.coillist[i].coilID ==
                f_saturn_coil->CoilList.coillist[j].coilID &&
                f_saturn_coil->CoilList.coillist[i].portID[0] ==
                f_saturn_coil->CoilList.coillist[j].portID[0] && i != j) {

                l_ret = l_data_mgr_ptr->GetPQMCoilData()->
                        PqmCoilidGetSaturnCoil(
                            f_saturn_coil->CoilList.coillist[i].coilID,
                            f_saturn_coil->CoilList.coillist[i].portID[0],
                            NULL, l_saturn_coil);

                if (l_saturn_coil.GetCoil() != NULL && l_saturn_coil.GetCoil()->numSCoil > 1) {

                    l_list_flg[i] = 1;

                } else {
                    // Coil selection
                    return ERROR_RESELECT;
                }
            }
        }
    }

    int k = 0;
    // Use multiple search coil (coil connection side )
    // Coil in the multiple connections that are connected to the search coil
    //	and multiple coil connected to the information in a list Stored.
    // You can not connect multiple coil connection is more than that if the error.

    for (i = 0; i < f_coil_connect->num; i++) {

        for (int j = 0; j < f_coil_connect->num; j++) {

            l_ret = l_data_mgr_ptr->GetPQMCoilData()->
                    PqmCoilidGetSaturnCoil(
                        f_coil_connect->coillist[i].coilID,
                        f_coil_connect->coillist[i].portID[0],
                        NULL,
                        l_saturn_coil);

            if (f_coil_connect->coillist[i].coilID ==
                f_coil_connect->coillist[j].coilID &&
                f_coil_connect->coillist[i].portID[0] ==
                f_coil_connect->coillist[j].portID[0] && i != j) {

                if (l_saturn_coil.GetCoil() != NULL &&
                    l_saturn_coil.GetCoil()->numSCoil > 1) {

                    l_concect_flg[i] = 1;
                    k++;
                    break;

                } else {
                    //Connect multiple coil coil is not connected to the
                    //multiple was an error
                    return ERROR_DISCONNECT;
                }
            }
        }
    }


    // Lack of checks coil
    // The search of the coil can be used to connect coil short list to check
    //whether or not there. Multiple coil is connected

    for (i = 0; i < f_saturn_coil->numSection; i++) {

        for (int j = 0; j < f_coil_connect->num; j++) {

            //Connect coil inside the coil has been selected to check whether there are any

            if (f_saturn_coil->RcvCoil[i].coilID ==
                f_coil_connect->coillist[j].coilID &&
                f_saturn_coil->RcvCoil[i].portID[0] ==
                f_coil_connect->coillist[j].portID[0]) {

                if (f_saturn_coil->RcvCoil[i].portID[0] == CDB_PORT_D) {

                    if ((f_coil_connect->coillist[j].portID[1] == '1'
                         && f_saturn_coil->RcvCoil[i].channel == 'A')
                        || (f_coil_connect->coillist[j].portID[1] == '2'
                            && f_saturn_coil->RcvCoil[i].channel == 'B')) {

                        break;
                    }

                } else if ((l_recvr_sec_flg[i] <= 1 && !l_concect_flg[j]) ||
                           f_saturn_coil->RcvCoil[i].portID[1] ==
                           f_coil_connect->coillist[j].portID[1]) {
                    //Port or non-use of multiple ID (number) match
                    break;
                }
            }
        }

        if (j == f_coil_connect->num) {
            //Connect coil in the coil, so there is no shortage
            return ERROR_RESELECT;
        }
    }

    // Port additional information
    // Multiple coil connected to that port is a non-ID (number) to override.
    // Multiple coils that are connected to the list of those still in use.

    // List coil
    for (i = 0; i < f_saturn_coil->CoilList.num; i++) {

        for (int j = 0; j < f_coil_connect->num; j++) {

            if (f_saturn_coil->CoilList.coillist[i].coilID ==
                f_coil_connect->coillist[j].coilID &&
                f_saturn_coil->CoilList.coillist[i].portID[0] ==
                f_coil_connect->coillist[j].portID[0]) {

                if (!l_list_flg[i] && !l_concect_flg[j]) {

                    //List of their multiple coil connected to the port's ID (number) to override.
                    f_saturn_coil->CoilList.coillist[i].portID[1] =
                        f_coil_connect->coillist[j].portID[1];
                }
            }
        }
    }

    // Selected sections
    for (i = 0; i < f_saturn_coil->numSection; i++) {

        for (int j = 0; j < f_coil_connect->num; j++) {

            if (f_saturn_coil->RcvCoil[i].coilID ==
                f_coil_connect->coillist[j].coilID &&
                f_saturn_coil->RcvCoil[i].portID[0] ==
                f_coil_connect->coillist[j].portID[0]) {

                if (l_recvr_sec_flg[i] <= 1 && !l_concect_flg[j]) {

                    //Receiver coil (section) connection if the coils are not multi-port ID (number) to override.
                    f_saturn_coil->RcvCoil[i].portID[1] =
                        f_coil_connect->coillist[j].portID[1];
                }

                if (((f_saturn_coil->RcvCoil[i].portID[0] == CDB_PORT_G)
                     || (f_saturn_coil->RcvCoil[i].portID[0] == CDB_PORT_D))
                    && (f_saturn_coil->RcvCoil[i].portID[1] == ' ')) {

                    //This is to prevent a recurrence, but do not go through (if I can have bugs) for the check.
                    //ERROR_LOG("Space in portID.");
                    //sprintf(msg, "coilID = %x portID = %s",saturnCoil->RcvCoil[i].coilID, saturnCoil->RcvCoil[i].portID);
                    //ERROR_LOG(msg);
                    return ERR_COILDATA;
                }
            }
        }
    }

    // Combine check
    l_coil_select.num = f_saturn_coil->numSection;

    for (i = 0; i < f_saturn_coil->numSection; i++) {
        l_coil_select.select[i] = f_saturn_coil->RcvCoil[i];
    }

    if (l_data_mgr_ptr->GetPQMCoilData()->PqmCoilGetSaturnCombine(
            f_coil_connect, l_combine) != E_NO_ERROR) {

        //PqmUiErrorDialog(PqmUiFetchLiteral("ERR_ILLEGAL_COIL_DB"),ERR_ERROR);
        //+Patni-Hemant/Added On 3/4/2009 6:32:17 PM .
        //Illegal coil da base connected message dispaly
        //+Patni-MP/2009Mar16/Added/ACE-1 RM Hardware

        //l_error_string.LoadString(IDS_CS_ERR_ILLEGAL_COIL_ID);												// Added By KT/ACE-2/15-06-2009/Multilingualisation
        //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
            CPqmUtilities::GetMultiLingualString(_T("IDS_CS_ERR_ILLEGAL_COIL_ID")),
            MPlus::EH::Information, _T("ScanTransaction.cpp"), _T("PQM"));
        //-Patni-Hemant/Added On 3/4/2009 6:32:17 PM .

        return ERR_OFF_GROUP;
    }

    // liobAcq Call
    //MRPdefect122
    int l_return = SaturnCoilCombinChk(f_coil_connect, l_combine, &l_coil_select);
    //l_return = E_NO_ERROR;
    //MRPdefect122


    //+Patni-Hemant/2010Feb19/Added/ACE-Phase#3/CDS Requirement
    if ((l_coil_select.num == 0) && (l_return == ERROR_DISCONNECT) && (f_coil_connect->num == 2)) {
        // COMPASS, and this would imaging if you try to connect multiple non-connected coils.
        // If not, I'd like to MSG_COIL_CDS_NONE coil is chosen, the return value
        // E_NO_ERROR rewritten.

        l_return = E_NO_ERROR;
    }

    //-Patni-Hemant/2010Feb19/Added/ACE-Phase#3/CDS Requirement
    if (l_return == E_NO_ERROR) {
        // Coil edit list
        // A list of connection information to replace the coil.

        for (i = 0; i < f_coil_connect->num; i++) {

            for (int j = 0; j < f_saturn_coil->CoilList.num; j++) {

                if (f_saturn_coil->CoilList.coillist[j].coilID ==
                    f_coil_connect->coillist[i].coilID &&
                    f_saturn_coil->CoilList.coillist[j].portID[0] ==
                    f_coil_connect->coillist[i].portID[0] &&
                    f_saturn_coil->CoilList.coillist[j].portID[1] ==
                    f_coil_connect->coillist[i].portID[1]) {

                    break;
                }
            }

            if (j == f_saturn_coil->CoilList.num &&
                f_coil_connect->coillist[i].coilID != WBCOIL_PHYSICAL_ID) {

                //If you are not on the list coil coil connection information to add, however, except for WB
                f_saturn_coil->CoilList.num++;
                f_saturn_coil->CoilList.coillist[j] = f_coil_connect->coillist[i];
                f_saturn_coil->CoilList.coillist[j].coordinate = 0;
                f_saturn_coil->CoilListcoordinate.coordinate_list[j].coordinate = 0;

                //+Patni-Hemant/2010Feb19/Added/ACE-Phase#3/CDS Requirement

                /* Channel Information */
                if (f_saturn_coil->CoilList.coillist[j].portID[0]  == CDB_PORT_D) {
                    if (f_saturn_coil->CoilList.coillist[j].portID[1] == '1') {
                        f_saturn_coil->CoilList.coillist[j].channel = CDB_CHANNEL_A;

                    } else {
                        f_saturn_coil->CoilList.coillist[j].channel = CDB_CHANNEL_B;
                    }

                } else {
                    CCoil l_saturn_coil_temp;
                    l_data_mgr_ptr->GetPQMCoilData()->PqmCoilidGetSaturnCoil(f_coil_connect->coillist[i].coilID,
                            f_coil_connect->coillist[i].portID[0], NULL, l_saturn_coil_temp);

                    f_saturn_coil->CoilList.coillist[j].channel = l_saturn_coil_temp.GetCoil()->channel;
                }

                //-Patni-Hemant/2010Feb19/Added/ACE-Phase#3/CDS Requirement
            }

        }

        for (int j = 0; j < f_saturn_coil->CoilList.num;) {

            for (i = 0; i < f_coil_connect->num; i++) {

                if (f_saturn_coil->CoilList.coillist[j].coilID ==
                    f_coil_connect->coillist[i].coilID &&
                    f_saturn_coil->CoilList.coillist[j].portID[0] ==
                    f_coil_connect->coillist[i].portID[0] &&
                    f_saturn_coil->CoilList.coillist[j].portID[1] ==
                    f_coil_connect->coillist[i].portID[1]) {

                    j++; //Match is found coiled coil Next list (j)
                    break;
                }
            }

            if (i == f_coil_connect->num) {
                //If you do not have information connecting the coiled coil to remove the list
                f_saturn_coil->CoilList.num--;

                for (k = j; k < f_saturn_coil->CoilList.num; k++) {

                    f_saturn_coil->CoilList.coillist[k] =
                        f_saturn_coil->CoilList.coillist[k + 1];
                }

                //+Patni-HAR/Hemant/2010June17/Modified/V1.20#MVC008295
                memset(&f_saturn_coil->CoilList.coillist[k + 1], 0,
                       sizeof(f_saturn_coil->CoilList.coillist[k + 1]));
                // memset(&f_saturn_coil->CoilList.coillist[k], 0,
                // sizeof(f_saturn_coil->CoilList.coillist[k]));
                //-Patni-HAR/Hemant/2010June17/Modified/V1.20#MVC008295
                //Coil compaction of the list, so keep the index j
            }
        }

        //This is the PQM coil in the list to update the information.
        //Study update this function outside.

        //Under the protocol
        if (f_change_under_prot) {
            AcqChangeCoilList(f_coil_connect, l_concect_flg);
        }
    }

    return l_return;
}

//*******************************Method Header*********************************
//Method Name    :AcquisitionStatusCallback()
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
int CScanTransaction::AcquisitionStatusCallback(
    int type,
    AM_ClientMessageHeader_t* header,
    AM_AcqManToClient_t* data
)
{
    LPCTSTR FUNC_NAME = _T("CScanTransaction::AcquisitionStatusCallback");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("Trace of AcquisitionStatusCallback"));

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (header == NULL) {
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("header pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    CScanProcedure* scan_proc = GetScanProcedure();

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (scan_proc == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("scan_proc pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    CPqmPmAcqman* acqman = scan_proc->GetAcqManager();

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (acqman == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("acqman pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    CPqm* pqmptr = acqman->GetPqm();

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (pqmptr == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("pqmptr pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    CPqmProtocol* curr_protocol = GetScanProcedure()->GetScanProcProtocol();

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (curr_protocol == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("curr_protocol pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    if (type == AMCM_FUNC_KIND_REQUEST) {
        return E_NO_ERROR;
    }

    //Patni-PJS/2009Dec15/Added/MVC006256
    int l_postmsg_status = E_ERROR;
    //Patni-Meghna-DKH/2010May03/Added/Phase3#IR105
    CPQMStudy* l_study = pqmptr->GetStudy(); //Patni-NP/2010May26/Added/IR-151

    switch (header->function) {
        case AMCMS_WAIT_FOR_START:

            CPQMLogMgr::GetInstance()->WriteOpeLog(AMCMS_WAIT_FOR_START);

            //+Patni-MP/2010Mar04/Modified/MSA00251-00251
            //+Patni-MP/2009Dec15/Added/MVC006256
            //Patni-Rajeesh/2010Dec10/commented/REDMINE-1095

            /* if (!curr_protocol->GetSeriesNumInDbFlag()) {
                 //Patni-HAR/2010Apr22/Added/MVC007503, MVC007564
                 pqmptr->m_dbsa_study_mgr->WriteSeriesInformationAtSeriesLevel(curr_protocol->GetSeriesLOID());
                 l_postmsg_status = pqmptr->m_dbsa_study_mgr->SetSeriesNoInDB((LPCTSTR)curr_protocol->GetSeriesLOID(), curr_protocol->GetAcqStatus());

                 if (l_postmsg_status == E_ERROR) {
                     PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed to set series in DB"));

                     //Patni-HAR/2010Mar10/Added/MSA0251-00279
                     AbortScan();
                     return E_ERROR;
                 }

                 curr_protocol->SetSeriesNumInDbFlag(TRUE);
             }
             */
            //+Patni-NP/2010May26/Added/IR-151
            if (NULL != l_study)
                l_study->SetstudyReopened(false);

            //-Patni-NP/2010May26/Added/IR-151

            AcquisitionWaitForStart(data);
            //-Patni-MP/2010Mar04/Modified/MSA00251-00251
            //-Patni-MP/2009Dec15/Added/MVC006256
            break;

        case AMCMS_SCAN_BEGIN:

            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, _T("AMCMS_SCAN_BEGIN"));

            //+Patni-HAR/2010May06/Added/MVC006285
            {
                CPQMStudy* l_varfield_study = pqmptr->GetStudy();
                CPQMStudyMgr* l_local_study = pqmptr->GetDbsaStudyMgr();

                if (l_varfield_study && l_local_study) {
                    //Patni-HAR/2010Jun02/Added/MVC006285 Defect Fix
                    if (curr_protocol->GetPosition() == 1) {
                        char* l_scan_date_time = l_varfield_study->GetScanDateTime(curr_protocol->GetProtocol());

                        if (l_scan_date_time) {

                            l_varfield_study->SetScanDateTime(l_scan_date_time);
                            l_local_study->SetScanDateTimeInLocalDB(l_scan_date_time);

                            DEL_PTR_ARY(l_scan_date_time);

                        } else {
                            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                                      _T("Scan Date Time is NULL"));
                        }
                    }

                } else {
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                              _T("CPQMStudy or CPQMStudyMgr Pointer is NULL"));
                }
            }
            //-Patni-HAR/2010May06/Added/MVC006285

            //+Patni-PJS/2009Dec15/Added/MVC006256
            if (!curr_protocol->GetSeriesNumInDbFlag()) {
                //Patni-HAR/2010Apr22/Added/MVC007503, MVC007564
                pqmptr->GetDbsaStudyMgr()->WriteSeriesInformationAtSeriesLevel(curr_protocol->GetSeriesLOID());
                l_postmsg_status = pqmptr->GetDbsaStudyMgr()->SetSeriesNoInDB((LPCTSTR)curr_protocol->GetSeriesLOID(), curr_protocol->GetAcqStatus());

                if (l_postmsg_status == E_ERROR) {
                    PQM_TRACE(USER_FUNC_MARKER, _T("CScanTransaction::AcquisitionStatusCallback"),
                              _T("Failed to set series in DB"));

                    //Patni-HAR/2010Mar10/Added/MSA0251-00279
                    AbortScan();

                    return E_ERROR;
                }

                curr_protocol->SetSeriesNumInDbFlag(TRUE);
            }

            //-Patni-PJS/2009Dec15/Added/MVC006256
            if (curr_protocol->GetProbeScan() || curr_protocol->GetRetroMode()
                || pqmptr->GetAMB()->PqmAMBCheckScanStatus(AMB_GO_SCAN)
               ) {
                pqmptr->WriteToUI(PQM_MSG_SCAN_STARTPAUSE_DISABLE);
                //          PqmAcqGlobal.scanPause = NULL ;

            } else {
                pqmptr->WriteToUI(PQM_MSG_SCAN_START);
            }

            //+Patni-NP/2010May26/Added/IR-151
            if (NULL != l_study)
                l_study->SetstudyReopened(false);

            //-Patni-NP/2010May26/Added/IR-151

            //  PqmAcqGlobal.scanAbort  = PqmAcqScanAbort;

            //  PqmUiSetSensitive(W_SCAN_START_BTN,FALSE);
            //  PqmUiSetSensitive(W_SCAN_PAUSE_BTN,PqmAcqGlobal.scanPause != NULL);
            //  PqmUiSetSensitive(W_SCAN_ABORT_BTN,TRUE);

            //  PQM_ACQ_STATUS();

            break;

        case AMCMS_SCAN_PAUSE:
            CPQMLogMgr::GetInstance()->WriteOpeLog(AMCMS_SCAN_PAUSE);

        case AMCMS_SCAN_RESUME:
            CPQMLogMgr::GetInstance()->WriteOpeLog(AMCMS_SCAN_RESUME);

            if (header->function == AMCMS_SCAN_PAUSE) {
                //Patni-NP/2009Oct06/Commented/MVC005135
                //CVoiceManager::GetInstance()->SafeExit();
                //Patni-DKH/2009Dec08/Added/Auto Voice Defect Fix - MVC005845

                //Patni-Sribanta/2011Jan14/Commented/TMSC-REDMINE-1026
                //CVoiceManager::GetInstance()->SafeExit();

                if (curr_protocol->GetDynamic() &&
                    curr_protocol->GetPlnAutoVoice()) {
                    CVoiceManager::GetInstance()->CmdQuit();
                }

                if (curr_protocol->GetExeAutoVoice() ||
                    curr_protocol->GetPlnAutoVoice()) {

                    CVoiceManager::GetInstance()->CmdPlay(PQM_AV_VOICE_5, TRUE);
                    m_scan_procedure->SetWaitForVoice(false);
                }

                if (curr_protocol->GetDynamic() &&
                    curr_protocol->GetPlnAutoVoice()) {

                    curr_protocol->SetPlnAutoVoice(0);
                }

                scan_proc->SetScanStart(NULL);
                scan_proc->SetScanPause(PQM_SCAN_PAUSE);
                scan_proc->SetScanAbort(PQM_SCAN_ABORT);

                pqmptr->WriteToUI(PQM_MSG_PAUSE_DONE);		// + SM4 Defect#679 Himanshu 14 Jan 2009 // added redmine-2574

            } else {

                scan_proc->SetScanStart(NULL);
                scan_proc->SetScanPause(PQM_SCAN_PAUSE);
                scan_proc->SetScanAbort(PQM_SCAN_ABORT);

                pqmptr->WriteToUI(PQM_MSG_RESUME_DONE);
            }

            break;

        case AMCMS_SCAN_END:
            CPQMLogMgr::GetInstance()->WriteOpeLog(AMCMS_SCAN_END);
            {


                int l_no_of_scan = -1;
                l_no_of_scan = curr_protocol->GetNumOfScan();
                l_no_of_scan++;
                curr_protocol->SetNumOfScan(l_no_of_scan);
                scan_proc->SetScanStart(PQM_SCAN_START);
                break;
            }

        case AMCMS_RMSCAN_END :
            CPQMLogMgr::GetInstance()->WriteOpeLog(AMCMS_RMSCAN_END);

            //+Patni-HAR/2010June03/Modified/MVC004927
            if ((curr_protocol->GetExeAutoVoice() & 0x0F) ||
                curr_protocol->GetPlnAutoVoice() ||
                ((curr_protocol->GetExeAutoVoice() & 0x0F) &&
                 curr_protocol->GetNumOfScan() == curr_protocol->GetNumOfCoverages())) {
                //-Patni-HAR/2010June03/Modified/MVC004927
                CVoiceManager::GetInstance()->CmdPlay(PQM_AV_VOICE_5, TRUE);
                m_scan_procedure->SetWaitForVoice(false);
            }

            break;

        case AMCMS_PRESCAN_CFA_END:
            CPQMLogMgr::GetInstance()->WriteOpeLog(AMCMS_PRESCAN_CFA_END);

        case AMCMS_PRESCAN_RFL_END:
            CPQMLogMgr::GetInstance()->WriteOpeLog(AMCMS_PRESCAN_RFL_END);

        case AMCMS_PRESCAN_RXGAIN_END:
            CPQMLogMgr::GetInstance()->WriteOpeLog(AMCMS_PRESCAN_RXGAIN_END);

            //+Patni-Hemant/2010Feb19/Added/ACE-Phase#3/CDS Requirement
        case AMCMS_PRESCAN_CDS_END:
            CPQMLogMgr::GetInstance()->WriteOpeLog(AMCMS_PRESCAN_CDS_END);
            //-Patni-Hemant/2010Feb19/Added/ACE-Phase#3/CDS Requirement
            {

                //+Patni-Manish/Modified On 11April2009//PSP1#301//Replaced AUTO_VOICE_VALUE by 0xF0
                if ((curr_protocol->GetExeAutoVoice() & 0xF0) && m_scan_procedure->GetWaitForVoice()) {
                    CVoiceManager::GetInstance()->CmdPlay(PQM_AV_VOICE_5, TRUE);
                    m_scan_procedure->SetWaitForVoice(false);
                }

                //-Patni-Manish/Modified On 11April2009//PSP1#301//Replaced AUTO_VOICE_VALUE by 0xF0
                //+Patni-RAJ/2010Jan14/Added/PH#3# -IR-86/Debug Window
                CPQMDebugInfo::GetInstance(pqmptr)->PqmDebugAcqMonitor(_T("exeAutoVoice = 0x%x waitForVoice = %d"),
                        curr_protocol->GetExeAutoVoice(),
                        m_scan_procedure->GetWaitForVoice());
                //-Patni-RAJ/2010Jan14/Added/PH#3# -IR-86/Debug Window
                break;
            }

        case AMCMS_DYNAMIC_SCAN_BEGIN:

            CPQMLogMgr::GetInstance()->WriteOpeLog(AMCMS_DYNAMIC_SCAN_BEGIN);
            m_scan_procedure->SetCurrentDynamicRepeat(data->statusList.lines);

            //+Patni-RAJ/2010Jan14/Added/PH#3# -IR-86/Debug Window
            {
                // MEITEC /2010Sep21/Format is modified/IR-86/Debug Window
                CString l_str_func(_T(""));
                l_str_func.Format(_T("Status(%lu)"), header->function);

                CPQMDebugInfo::GetInstance(pqmptr)->PqmDebugAcqMonitor(_T("%-32s : %d"),
                        l_str_func,
                        data->statusList.lines);
            }

            //-Patni-RAJ/2010Jan14/Added/PH#3# -IR-86/Debug Window
            if (curr_protocol->GetDynamic() &&
                curr_protocol->GetPlnAutoVoice()
               ) {
                int length = -1, start = -1, period = -1;

                length = CVoiceManager::GetInstance()->GetVoiceLength(curr_protocol->GetPlnAutoVoice()) +
                         CVoiceManager::GetInstance()->GetVoiceLength(PQM_AV_VOICE_5);

                const VftTimeCtrlParam_t* l_avtime_ctrl = scan_proc->GetAvTimeCtrl();

                for (int i = 0; i < l_avtime_ctrl->numSegment; i++) {

                    start = l_avtime_ctrl->segment[i + 1].start -
                            CVoiceManager::GetInstance()->GetAvDynamicDelay();

                    period = l_avtime_ctrl->segment[i].period -
                             CVoiceManager::GetInstance()->GetAvDynamicDelay();

                    if (data->statusList.lines < l_avtime_ctrl->segment[i].repeat) {
                        //CPQMDebugAcqMonitor(_T("length = %d period = %d"), length, period);
                        //+Patni-RAJ/2010Jan14/Added/PH#3# -IR-86/Debug Window
                        CPQMDebugInfo::GetInstance(pqmptr)->PqmDebugAcqMonitor(_T("length = %d period = %d\n"),
                                length, period);

                        //-Patni-RAJ/2010Jan14/Added/PH#3# -IR-86/Debug Window
                        if (length < period) {
                            //+Patni-Manish/Added On 11April2009//PSP1#301
                            CVoiceManager::GetInstance()->CmdPDelay(
                                scan_proc->GetAvAcqTime(),
                                period,
                                PQM_AV_VOICE_5,
                                curr_protocol->GetPlnAutoVoice());
                            //-Patni-Manish/Added On 11April2009//PSP1#301

                            //+Patni-Manish/Commented On 11April2009//PSP1#301
                            /*
                            CVoiceManager::GetInstance()->CmdPDelay(
                              	pqmptr->GetMode(),
                                  scan_proc->GetAvAcqTime(),
                                  period,
                                  PQM_AV_VOICE_5,
                                  curr_protocol->GetPlnAutoVoice());
                            */
                            //-Patni-Manish/Commented On 11April2009//PSP1#301
                        }

                        break;

                    } else if (data->statusList.lines ==  l_avtime_ctrl->segment[i].repeat &&
                               i != l_avtime_ctrl->numSegment - 1) {

                        // CPQMDebugAcqMonitor(_T("length = %d start = %d"), length, start);
                        //+Patni-RAJ/2010Jan14/Added/PH#3# -IR-86/Debug Window
                        CPQMDebugInfo::GetInstance(pqmptr)->PqmDebugAcqMonitor(_T("length = %d start = %d\n"),
                                length, start);
                        //-Patni-RAJ/2010Jan14/Added/PH#3# -IR-86/Debug Window

                        if (length < start)	{
                            //+Patni-Manish/Added On 11April2009//PSP1#301
                            CString l_strlength = L"";
                            l_strlength.Format(L"%d", length);
                            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, (_T("Length = ") + l_strlength));

                            CString l_strStart = L"";
                            l_strStart.Format(L"%d", start);
                            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, (_T("Start = ") + l_strStart));


                            if (curr_protocol->GetAcqSplitMode() != VFC_ACQ_SPLIT_MODE_PREP || i != 0) { //Patni-Manishkumar/Added On 15/04/2009//PSP1-#249//Fixed for Visual Prep

                                CVoiceManager::GetInstance()->CmdPDelay(scan_proc->GetAvAcqTime(),
                                                                        start,
                                                                        PQM_AV_VOICE_5,
                                                                        curr_protocol->GetPlnAutoVoice());
                            }

                            //-Patni-Manish/Added On 11April2009//PSP1#301

                            //+Patni-Manish/Commented On 11April2009//PSP1#301
                            /*
                            CVoiceManager::GetInstance()->CmdPDelay(
                            	scan_proc->GetAvAcqTime(),
                                scan_proc->GetAvAcqTime(),
                                start,
                                PQM_AV_VOICE_5,
                                curr_protocol->GetPlnAutoVoice());
                            */
                            //-Patni-Manish/Commented On 11April2009//PSP1#301
                        }

                        break;
                    }
                }
            }

            break;

        case AMCMS_VISUALPREP_END:

            CPQMLogMgr::GetInstance()->WriteOpeLog(AMCMS_VISUALPREP_END);
            //Patni-RAJ/2010Jan14/Added/PH#3# -IR-86/Debug Window
            CPQMDebugInfo::GetInstance(pqmptr)->PqmDebugAcqMonitor(_T("AMCMS_VISUALPREP_ END  Prot=%d "), curr_protocol->GetProtocol());

            if (pqmptr->GetAMB()->PqmAMBScanSeg2Active()) {				//
                pqmptr->GetAMB()->PqmAMBVisualPrepEnd(l_study->GetCurrentProtocol()->GetProtocol());
            }

            break;

        case AMCMS_AMB_PROTOCOL_END:

            CPQMLogMgr::GetInstance()->WriteOpeLog(AMCMS_AMB_PROTOCOL_END);

            pqmptr->GetAMB()->PqmAmbEachAcquisitionDone(l_study->GetCurrentProtocol(), true);

            break;
    }

    return E_NO_ERROR;
}

//****************************Method Header************************************
//Method Name   :CheckBreastImaging()
//Author        :PATNI/HAR-JS
//Purpose       :
//*****************************************************************************
int CScanTransaction::CheckBreastImaging(const int f_recv_coil_id,
        const int f_patori_side_up,
        const int f_patori_end_in) const
{

    LPCTSTR FUNC_NAME = _T("CScanTransaction::CheckBreastImaging");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CheckBreastImaging"));

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections Need to Check
    if (m_scan_procedure == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_scan_procedure pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections


    CPQMStudy* l_study =  m_scan_procedure->GetAcqManager()->GetPqm()->GetStudy();

    if ((f_recv_coil_id & 0xff00ffff) == BREAST_SPDR_COIL) {
        if (f_patori_end_in != VFC_PATORI_HEAD_FIRST) {

            //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
            CPQMLogMgr::GetInstance()->DisplayScanError(
                IDS_ERR_BREAST_ILLEGAL_PATIENT_DIR, _T("IDS_ERR_BREAST_ILLEGAL_PATIENT_DIR"), SCAN_ERR_NOTIFY,
                _T("ScanTransaction.cpp"));

            l_study->SetScanModeOfHead();
            m_scan_procedure->ResetCurrentRequest();
            return E_ERROR;
        }

        if (f_patori_side_up != VFC_PATORI_PRONE) {

            //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
            CPQMLogMgr::GetInstance()->DisplayScanError(
                IDS_ERR_BREAST_ILLEGAL_PATIENT_POSI, _T("IDS_ERR_BREAST_ILLEGAL_PATIENT_POSI"), SCAN_ERR_NOTIFY,
                _T("ScanTransaction.cpp"));

            l_study->SetScanModeOfHead();
            m_scan_procedure->ResetCurrentRequest();

            return E_ERROR;
        }
    }

    return	E_NO_ERROR;
}

// + V9.25 coil support@AMT
//****************************Method Header************************************
//Method Name   : CheckBreastImagingInSaturn()
//Author        : PATNI/AMT
//Purpose       : Check breast imaging in satrun coil support.
//*****************************************************************************
status_t  CScanTransaction::CheckBreastImagingInSaturn(
    //+Patni-Hemant/2010Feb19/Modified/ACE-Phase#3/CDS Requirement
    //PqmSaturnRcvCoil_t *f_coil_t,
    VftCoilListInfo_t* f_coil_t,
    //-Patni-Hemant/2010Feb19/Modified/ACE-Phase#3/CDS Requirement
    VftPatori_t* f_pat_ori
)
{

    PqmCoil_t*   l_tmp_coil = NULL ;
    status_t	l_rtn = E_NO_ERROR;
    //+Patni-Hemant/2010Feb19/Added/ACE-Phase#3/CDS Requirement
    CDataManager* l_data_mgr_ptr = m_scan_procedure->GetPqmPtr()->GetDataManager();
    //-Patni-Hemant/2010Feb19/Added/ACE-Phase#3/CDS Requirement

    //+Patni-Hemant/2010Feb19/Commented/ACE-Phase#3/CDS Requirement
    //    if (f_coil_t->numCoil == 0) {
    //        return E_ERROR ;
    //    }
    //-Patni-Hemant/2010Feb19/Commented/ACE-Phase#3/CDS Requirement

    //+Patni-Hemant/2010Feb19/Modified/ACE-Phase#3/CDS Requirement
    //for (i = 0 ; i < f_coil_t->numCoil ; i++) {
    for (int i = 0 ; i < f_coil_t->num ; i++) {
        //-Patni-Hemant/2010Feb19/Modified/ACE-Phase#3/CDS Requirement

        //+Patni-Hemant/2010Feb19/Modified/ACE-Phase#3/CDS Requirement
        //l_tmp_coil = f_coil_t->coiltbl_p[i] ;
        CCoil l_saturn_coil_temp;
        l_data_mgr_ptr->GetPQMCoilData()->PqmCoilidGetSaturnCoil(f_coil_t->coillist[i].coilID, f_coil_t->coillist[i].portID[0], NULL, l_saturn_coil_temp);
        l_tmp_coil = l_saturn_coil_temp.GetCoil();
        //-Patni-Hemant/2010Feb19/Modified/ACE-Phase#3/CDS Requirement

        // If patient_orient if not equal to 0, perform BreastImaging
        if (l_tmp_coil->patient_orient != 0) {
            if (f_pat_ori->end_in == VFC_PATORI_HEAD_FIRST &&
                f_pat_ori->side_up == VFC_PATORI_RIGHT_UP) {

                if (l_tmp_coil->patient_orient & 0x0800) {			/* Head Right Up */
                    l_rtn = E_ERROR;
                }

            } else if (f_pat_ori->end_in == VFC_PATORI_HEAD_FIRST &&
                       f_pat_ori->side_up == VFC_PATORI_LEFT_UP) {

                if (l_tmp_coil->patient_orient & 0x0400) {			/* Head Left Up */
                    l_rtn = E_ERROR;
                }

            } else if (f_pat_ori->end_in == VFC_PATORI_HEAD_FIRST &&
                       f_pat_ori->side_up == VFC_PATORI_SUPINE) {

                if (l_tmp_coil->patient_orient & 0x0200) {			/* Head First Spine */
                    l_rtn = E_ERROR;
                }

            } else if (f_pat_ori->end_in == VFC_PATORI_HEAD_FIRST &&
                       f_pat_ori->side_up == VFC_PATORI_PRONE) {

                if (l_tmp_coil->patient_orient & 0x0100) {			/* Head First Pron */
                    l_rtn = E_ERROR;
                }

            } else if (f_pat_ori->end_in == VFC_PATORI_FEET_FIRST &&
                       f_pat_ori->side_up == VFC_PATORI_RIGHT_UP) {

                if (l_tmp_coil->patient_orient & 0x0008) {			/* Feet Right Up */
                    l_rtn = E_ERROR;
                }

            } else if (f_pat_ori->end_in == VFC_PATORI_FEET_FIRST &&
                       f_pat_ori->side_up == VFC_PATORI_LEFT_UP) {

                if (l_tmp_coil->patient_orient & 0x0004) {			/* Feet Left Up */
                    l_rtn = E_ERROR;
                }

            } else if (f_pat_ori->end_in == VFC_PATORI_FEET_FIRST &&
                       f_pat_ori->side_up == VFC_PATORI_SUPINE) {

                if (l_tmp_coil->patient_orient & 0x0002) {			/* Feet First Spine */
                    l_rtn = E_ERROR;
                }

            } else if (f_pat_ori->end_in == VFC_PATORI_FEET_FIRST &&
                       f_pat_ori->side_up == VFC_PATORI_PRONE) {

                if (l_tmp_coil->patient_orient & 0x0001) {			/* Feet First Pron */
                    l_rtn = E_ERROR;
                }
            }

            if (l_rtn != E_NO_ERROR) {
                CPqm* l_pqm = m_scan_procedure->GetAcqManager()->GetPqm();
                CPQMStudy* l_study = l_pqm->GetStudy();

                //+Patni-HAR/2009April01/PSP-Internal Defect Fix
                CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
                    CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_BREAST_ILLEGAL_PATIENT")),
                    MPlus::EH::Crucial,
                    _T("ScanTransaction.cpp"),
                    _T("PQM"));
                //-Patni-HAR/2009April01/PSP-Internal Defect Fix
                l_study->SetScanModeOfHead();
                m_scan_procedure->ResetCurrentRequest();
                return E_ERROR;
            }
        }
    }

    return	E_NO_ERROR;
}
// - V9.25 coil support@AMT

//***************************Method Header*************************************
//Method Name    : SendMessage()
//Author         : PATNI/MRP
//Purpose        : Send Message
//*****************************************************************************
bool CScanTransaction::SendMessage(
)
{
    LPCTSTR FUNC_NAME = _T("CScanTransaction::SendMessage");
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
              _T("Trace of CScanTransaction::SendMessage"));

    bool result = true;

    CPqmPmAcqman* acqman = GetScanProcedure()->GetAcqManager();

    if (!acqman->SendMessage(this)) {

        result = false;

        //+Patni-PJS/2009May10/Modify/IR-82 review comment
        CString string = _T("");
        string.LoadString(IDS_ERR_MSG_NOT_SENT);
        CPQMLogMgr::GetInstance()->WriteEventLog(string,
                ERROR_LOG_CRUCIAL1  , _T("ScanTransaction.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82

        /*CPQMLogMgr::GetInstance()->WriteEventLog(IDS_ERR_MSG_NOT_SENT,
                ERROR_LOG_CRUCIAL1  , _T("ScanTransaction.cpp"));*/
        //-Patni-PJS/2009May10/Modify/IR-82 review comment
    }

    return result;
}

//***************************Method Header*************************************
//Method Name    : GetHeader()
//Author         : PATNI/JCM
//Purpose        : This method returns the message header that will be send to Acqman
//*****************************************************************************
AM_ClientMessageHeader_t CScanTransaction::GetHeader(
)const
{
    LPCTSTR FUNC_NAME = _T("CScanTransaction::GetHeader");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetHeader"));

    return m_header;
}

//***************************Method Header*************************************
//Method Name    : SetHeader()
//Author         : PATNI/ASG
//Purpose        :
//*****************************************************************************
void CScanTransaction::SetHeader(
    const AM_ClientMessageHeader_t& header
)
{
    LPCTSTR FUNC_NAME = _T("CScanTransaction::SetHeader");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetHeader"));

    m_header = header;
}

//***************************Method Header*************************************
//Method Name    : GetData()
//Author         : PATNI/JCM
//Purpose        : This method sets the message header that will be send to Acqman
//*****************************************************************************
AM_ClientToAcqMan_unicode_t /*AM_ClientToAcqMan_t*/ CScanTransaction::GetData(
)const
{
    LPCTSTR FUNC_NAME = _T("CScanTransaction::GetData");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetData"));
    return m_data;
}

//*******************************Method Header*********************************
//Method Name    :SetData()
//Author         :PATNI/
//Purpose        :
//*****************************************************************************
void CScanTransaction::SetData(
    /*AM_ClientToAcqMan_t*/  const AM_ClientToAcqMan_unicode_t& body
)
{
    LPCTSTR FUNC_NAME = _T("CScanTransaction::SetData");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetData"));

    m_data = body;
}

//***************************Method Header*************************************
//Method Name    : GetScanProcedure()
//Author         : PATNI/MRP
//Purpose        : This method returns the scan procedure pointer
//*****************************************************************************
CScanProcedure* CScanTransaction::GetScanProcedure(
)const
{
    LPCTSTR FUNC_NAME = _T("CScanTransaction::GetScanProcedure");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CScanTransaction::GetScanProcedure"));
    return m_scan_procedure;
}

//***************************Method Header*************************************
//Method Name    :PostProcess()
//Author         :PATNI/MRP
//Purpose        :This method is called after the transaction processes the
//                response got from Acqman
//*****************************************************************************
bool CScanTransaction::PostProcess(
    const float couch_position,
    const int couch_status,
    const int lastfunction
)
{
    LPCTSTR FUNC_NAME = _T("CScanTransaction::PostProcess");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of PostProcess"));
    CScanProcedure* scan_procedure = GetScanProcedure();

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (scan_procedure == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("scan_procedure pointer is NULL"));
        return false;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    CString str_msg(_T(""));
    str_msg.Format(_T("[Couch Position: %f, Couch Status: %d, Last Function: %s]"),
                   couch_position,
                   couch_status,
                   scan_procedure->GetAcqManager()->GetStr(lastfunction));

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

    bool result = false;

    m_couch_position = couch_position;
    m_couch_status = couch_status;

    //+Patni-Hemant/2010Mar16/Modified/ACE-Phase#3/CDS Requirement
    //    if (SetCheckBit()) {
    //        result = PreScanGo(lastfunction);
    //    }

    bool l_process_prescango = true ;
    m_lastfunction = lastfunction;

    if (SetCheckBit(l_process_prescango)) {
        result = l_process_prescango ? PreScanGo(lastfunction) : true ;
    }

    //-Patni-Hemant/2010Mar16/Modified/ACE-Phase#3/CDS Requirement

    return result;
}

//*******************************Method Header*********************************
//Method Name    :CheckMovingCouch()
//Author         :PATNI/HAR-SS
//Purpose        :
//*****************************************************************************
BOOL CScanTransaction::CheckMovingCouch(
)
{
    LPCTSTR FUNC_NAME = _T("CScanTransaction::CheckMovingCouch");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CheckMovingCouch"));

    float offset = 0.0f; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
    int literal = 0; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
    bool	offsetflg	= true;

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections Need to Check
    if (m_scan_procedure == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_scan_procedure pointer is NULL"));

        //+Patni-SS/20091014/Modified/TMSC review comments
        // this function returns type is bool (true(1)= success, false(0)= fail) E_ERROR value is 1.
        //so return value should be false.
        return FALSE;
        //return E_ERROR;
        //-Patni-SS/20091014/Modified/TMSC review comments
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    CPqmProtocol* curr_protocol = m_scan_procedure->GetScanProcProtocol();
    CPqmPrMode* pr_mode = curr_protocol->GetPqmPrMode();
    CCouchConfig* l_couch_config =
        m_scan_procedure->GetAcqManager()->GetPqm()->GetCouchConfigPtr();
    CPqm* l_pqm = m_scan_procedure->GetPqmPtr();
    CPqmCoilData* l_coil_data = l_pqm->GetDataManager()->GetPQMCoilData();
    CPQMStudy* l_study = l_pqm->GetStudy();
    CPqmProtocol* l_protocol = NULL;

    int l_in_side_limit = -1, l_out_side_limit = -1;

    CString str_msg(_T(""));

    if (curr_protocol->GetHoldScan() & VFC_HOLD_COUCH) {
        //+Patni-DKH/2010Oct20/Added/MVC009233
        if (curr_protocol->GetcouchMethodFlg() != VFC_MOVE_COUCH_METHOD_PARENT_POSI
            && curr_protocol->GetcouchMethodFlg() != VFC_MOVE_COUCH_METHOD_PLAN_CENTER_POSI) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("Couch move method is not set."));
            return FALSE;
        }

        //-Patni-DKH/2009Jun02/Added/MVC009233
        float   new_pos = 0.0f, in_limit_pos = 0.0f, out_limit_pos = 0.0f; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
        CCoil l_coil;
        l_coil_data->PqmCoilGetCoil(pr_mode->GetRecvCoilId(), l_coil);

        if (l_pqm->IsSaturnCoilSystem()) {

            l_coil_data->GetInOutSideLimit(curr_protocol->GetCoilDBTable(),
                                           &l_in_side_limit, &l_out_side_limit);

        } else {
            l_coil_data->PqmCoilGetCoil(pr_mode->GetRecvCoilId(), l_coil);
            l_out_side_limit = l_coil.GetOutsideLimit() ;
            l_in_side_limit = l_coil.GetInsideLimit();
        }

        float distance = (pr_mode->GetHomePos() - pr_mode->GetCouchPos());

        f32vec3_t l_offset = curr_protocol->GetOffset();
        float z = l_offset.z * 1000.0f;

        if (pr_mode->GetPatoriEndIn() == VFC_PATORI_HEAD_FIRST) {
            offset = floorf(distance + z);

        } else {
            offset = floorf(distance - z);
        }

        str_msg.Format(_T("distance = %g\n, Z Offset = %g m --> %g mm\n, homePos = %g offset = %g couchPos = %g"),
                       distance,
                       l_offset.z, z,
                       curr_protocol->GetPqmPrMode()->GetHomePos(), offset,
                       curr_protocol->GetPqmPrMode()->GetCouchPos());

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

        //+Patni-Raj/2010Jan14/Added/PH#3#/Debug Window
        CPQMDebugInfo::GetInstance(l_pqm)->PqmDebugAcqMonitor(_T("distance = %g\n"), distance);
        CPQMDebugInfo::GetInstance(l_pqm)->PqmDebugAcqMonitor(_T("Z Offset = %g m --> %g mm\n"), offset, z);
        CPQMDebugInfo::GetInstance(l_pqm)->PqmDebugAcqMonitor(_T("homePos = %g offset = %g couchPos = %g"),
                curr_protocol->GetPqmPrMode()->GetHomePos(), offset, curr_protocol->GetPqmPrMode()->GetCouchPos());
        //-Patni-Raj/2010Jan14/Added/PH#3#/Debug Window

        new_pos = pr_mode->GetCouchPos() + offset;

        //+Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image
        if (curr_protocol->GetcouchMethodFlg() == VFC_MOVE_COUCH_METHOD_PARENT_POSI) {
            //        if (l_pqm->GetMoveLocatorMode()) {
            //-Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image
            if (curr_protocol->GetLocatorImages(0).subtree_name == SVN_ST_PROTOCOL) {
                l_protocol   = l_study->GetItemFromProtocol(curr_protocol->GetLocatorImages(0).index);

                if (l_protocol != NULL) {
                    new_pos  = l_protocol->GetPqmPrMode()->GetCouchPos();
                    offset  = l_protocol->GetPqmPrMode()->GetCouchPos() - curr_protocol->GetPqmPrMode()->GetCouchPos();
                    offsetflg = false;
                }
            }
        }


        in_limit_pos = pr_mode->GetHomePos() - (float)l_in_side_limit;
        out_limit_pos = pr_mode->GetHomePos() + (float)l_out_side_limit;

        //+Patni-SS/20091014/Modified/TMSC review comments
        if (in_limit_pos < (float)l_couch_config->GetCouchInLimit()) {
            in_limit_pos = (float)l_couch_config->GetCouchInLimit();
        }

        if (out_limit_pos > (float)l_couch_config->GetCouchOutLimit()) {
            out_limit_pos = (float)l_couch_config->GetCouchOutLimit();
        }

        //-Patni-SS/20091014/Modified/TMSC review comments

        //Patni-Raj/2010Jan14/Added/PH#3#/Debug Window
        CPQMDebugInfo::GetInstance(l_pqm)->PqmDebugAcqMonitor(_T("Enable range %f < %f < %f"), in_limit_pos, new_pos, out_limit_pos);
        str_msg = _T("");

        str_msg.Format(_T("Enable range %f < %f < %f"),
                       in_limit_pos,
                       new_pos,
                       out_limit_pos);

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_msg);

        if (pr_mode->GetCouchPos() < in_limit_pos ||
            pr_mode->GetCouchPos() > out_limit_pos) {

            literal = IDS_MOVE_COUCH_CONFIRM_OUT_RANGE;
            m_scan_procedure->SetProcessing(m_scan_procedure->GetProcessing() |
                                            PQM_ACQ_BIT_WARNING);


            DisplayMoveCouchMessageinWarningDialog(literal, _T("IDS_MOVE_COUCH_CONFIRM_OUT_RANGE"), SCAN_WARN_OKONLYBUTTON, offset);		// Added By KT/ACE-2/11-06-2009/Multilingualisation

            return TRUE;

        } else if (((pr_mode->GetCouchPos() == in_limit_pos) &&
                    (new_pos < in_limit_pos)) ||
                   ((pr_mode->GetCouchPos() == out_limit_pos) &&
                    (new_pos > out_limit_pos))) {

            literal = IDS_MOVE_COUCH_CONFIRM_JUST_LIMIT;
            m_scan_procedure->SetProcessing(m_scan_procedure->GetProcessing() |
                                            PQM_ACQ_BIT_WARNING);

            DisplayMoveCouchMessageinWarningDialog(literal, _T("IDS_MOVE_COUCH_CONFIRM_JUST_LIMIT"), SCAN_WARN_OKONLYBUTTON, offset);
            return TRUE;
        }

        str_msg = _T("");									// Added By KT/ACE-2/11-06-2009/Multilingualisation

        //+Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image
        if (curr_protocol->GetcouchMethodFlg() == VFC_MOVE_COUCH_METHOD_PARENT_POSI && offsetflg == FALSE) {
            //if (l_pqm->GetMoveLocatorMode()) {
            //-Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image
            literal = IDS_MOVE_COUCH_CONFIRM_PLAN;
            str_msg = _T("IDS_MOVE_COUCH_CONFIRM_PLAN");			// Added By KT/ACE-2/11-06-2009/Multilingualisation

        } else {
            literal = IDS_MOVE_COUCH_CONFIRM;
            str_msg = _T("IDS_MOVE_COUCH_CONFIRM");					// Added By KT/ACE-2/11-06-2009/Multilingualisation
        }

        if (new_pos < in_limit_pos) {

            offset = in_limit_pos - pr_mode->GetCouchPos();
            //Patni-RAJ/2010Jan14/Added/PH#3# -IR-86/Debug Window
            CPQMDebugInfo::GetInstance(l_pqm)->PqmDebugAcqMonitor(_T("New offset = %f"), offset);
            literal = IDS_MOVE_COUCH_CONFIRM_ROUND_LIMIT;
            str_msg = _T("IDS_MOVE_COUCH_CONFIRM_ROUND_LIMIT");		// Added By KT/ACE-2/11-06-2009/Multilingualisation

        } else if (new_pos > out_limit_pos) {

            offset = out_limit_pos - pr_mode->GetCouchPos();
            //Patni-RAJ/2010Jan14/Added/PH#3# -IR-86/Debug Window
            CPQMDebugInfo::GetInstance(l_pqm)->PqmDebugAcqMonitor(_T("New offset = %f"), offset);
            literal = IDS_MOVE_COUCH_CONFIRM_ROUND_LIMIT;
            str_msg = _T("IDS_MOVE_COUCH_CONFIRM_ROUND_LIMIT");		// Added By KT/ACE-2/11-06-2009/Multilingualisation
        }

        if (ABSF(offset) < l_couch_config->GetCouchMinimumMovement()) {
            return FALSE;
        }

        if (offset > l_couch_config->GetCouchMaximumMovement()) {

            offset = (float)l_couch_config->GetCouchMaximumMovement();
            //Patni-RAJ/2010Jan14/Added/PH#3# -IR-86/Debug Window
            CPQMDebugInfo::GetInstance(l_pqm)->PqmDebugAcqMonitor(_T("offset is limited : %f"), offset);
            literal = IDS_MOVE_COUCH_CONFIRM_ROUND_LIMIT;
            str_msg = _T("IDS_MOVE_COUCH_CONFIRM_ROUND_LIMIT");		// Added By KT/ACE-2/11-06-2009/Multilingualisation

        } else if (offset < -l_couch_config->GetCouchMaximumMovement()) {

            offset = (float) - l_couch_config->GetCouchMaximumMovement();
            //Patni-RAJ/2010Jan14/Added/PH#3# -IR-86/Debug Window
            CPQMDebugInfo::GetInstance(l_pqm)->PqmDebugAcqMonitor(_T("offset is limited : %f"), offset);
            literal = IDS_MOVE_COUCH_CONFIRM_ROUND_LIMIT;
            str_msg = _T("IDS_MOVE_COUCH_CONFIRM_ROUND_LIMIT");		// Added By KT/ACE-2/11-06-2009/Multilingualisation
        }

        if (l_pqm->PqmPmIsActive(K_PM_TYPE_AMB) &&
            (curr_protocol->GetAMBModeFlag() != VFC_AMB_MODE_NONE)) {

            AcqSendMoveCouch(AMCMM_REMOTE_MOVECOUCH, offset);

        } else {
            if (!PLAN_CHECK(curr_protocol)) {

                literal = IDS_MOVE_COUCH_CONFIRM_NO_PARENT;
                m_scan_procedure->SetProcessing(m_scan_procedure->GetProcessing() |
                                                PQM_ACQ_BIT_WARNING);

                DisplayMoveCouchMessageinWarningDialog(literal, _T("IDS_MOVE_COUCH_CONFIRM_NO_PARENT"), SCAN_WARN_OKONLYBUTTON, offset);		// Added By KT/ACE-2/11-06-2009/Multilingualisation

                return TRUE;
            }

            m_scan_procedure->SetProcessing(m_scan_procedure->GetProcessing() |
                                            PQM_ACQ_BIT_WARNING);

            DisplayMoveCouchMessageinWarningDialog(literal, str_msg, SCAN_WARN_OK_CANCEL_BUTTONS, offset);  // Added By KT/ACE-2/11-06-2009/Multilingualisation
        }

        return TRUE;
    }

    return FALSE;
}

//***************************Method Header*************************************
//Method Name    :PreScanGo()
//Author         :PATNI/MRP
//Purpose        :This function decides the which transaction will be send
//                 depending upon previous completed transaction
//*****************************************************************************
bool CScanTransaction::PreScanGo(
    const int f_last_function
)
{
    LPCTSTR FUNC_NAME = _T("CScanTransaction::PreScanGo");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of PreScanGo"));

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections Need to Check
    if (m_scan_procedure == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_scan_procedure pointer is NULL"));
        return false;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    bool l_result = false;
    CPqm* l_pqm = m_scan_procedure->GetAcqManager()->GetPqm();
    CPQMStudy* l_study = l_pqm->GetStudy();
    CPqmProtocol* l_current_protocol = m_scan_procedure->GetScanProcProtocol();
    int l_protocolnum = l_current_protocol->GetProtocol();
    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    //int l_shimstate;
    int l_shimstate = 0;

    //+Patni-Ravindra Achayra/2010Aug16/Added/IR-153-Part1
    CString l_err_msg = _T("IDS_ERR_CONNECT_ACQMAN") ;

    //-Patni-Ravindra Achayra/2010Aug16/Added/IR-153-Part1
    //+Patni-Raj/2010Jan14/Added/PH#3#/Debug Window
    CPQMDebugInfo::GetInstance(l_pqm)->PqmDebugAcqMonitor(_T("PreScanGo : last_function = %s"),
            (f_last_function == AMCMA_ACQINFO_DONE) ? _T("ACQINFO") :
            (f_last_function == AMCMR_OLPTEMP_MEASUREMENT) ? _T("OLPTEMP") :	//+Patni-Ravindra Acharya/2010Jun22/Added/IR-153
            (f_last_function == AMCMA_SHIMCTRL_DONE) ? _T("SHIMCTRL") :
            (f_last_function == AMCMA_REMOTE_MOVECOUCH_DONE) ? _T("MOVECOUCH") :
            (f_last_function == AMCMA_RFINTERLOCK_DONE) ? _T("RFINTERLOCK") :
            (f_last_function == AMCMM_REMOTE_MOVECOUCH) ? _T("MOVECOUCH CANCEL") :
            _T("UNKNOWN"));

    CPQMDebugInfo::GetInstance(l_pqm)->PqmDebugAcqMonitor(_T("CHK BIT = %X :"), m_check_bit);

    //-Patni-Raj/2010Jan14/Added/PH#3#/Debug Window
    switch (f_last_function) {

            //+Patni-Hemant/2010Aug06/Modified/REDMINE392_Update

        case AMCMA_ACQINFO_DONE: {
            m_scan_procedure->SetCouchInfo(l_current_protocol->GetProtocol(),
                                           /*l_current_protocol->GetPosition()*/
                                           m_couch_position,
                                           //l_current_protocol->GetPqmPrMode()->GetCouchOffset(), //commented
                                           m_couch_status);

            //+Patni-Ravindra Acharya/2010June22/Added/IR-153
            //Patni-DKH/2010Aug03/Modified/IR-153-Review Comment
            if (l_pqm->IsCFShiftSupportedSystem()) { //Patni-DKH/2010Jul23/Added/IR-153
                if (m_check_bit & (PQM_CHK_BIT_FIRST | PQM_CHK_BIT_COIL)) {
                    COlpTemp* l_olptemp = m_scan_procedure->CreateOLPTransaction() ;

                    if (!l_olptemp || !l_olptemp->pqmAcqSendOLPTemp(PQM_TEMP_PRE)) {
                        //+Patni-Ravindra Acharya/2010Aug16/Commented/IR-153-Part1
                        //	COlpTemp::DisplayMessage("IDS_ERR_CONNECT_ACQMAN") ;

                        //+Patni-Ravindra Achayra/2010Aug16/Added/IR-153-Part1
                        COlpTemp::DisplayMessage(l_err_msg) ;
                        //-Patni-Ravindra Achayra/2010Aug16/Added/IR-153-Part1

                        l_study->SetScanModeOfHead() ;
                        m_scan_procedure->ResetCurrentRequest() ;
                        //+Patni-Ravindra Acharya/Bug Fix - 13/07/2010
                        l_result = false;
                        break;
                    }

                    return TRUE;
                }
            }

            //+Patni-Ravindra Acharya/Bug Fix - 13/07/2010
            l_result = PreScanGo(AMCMR_OLPTEMP_MEASUREMENT);
        }
        break;
        //-Patni-Ravindra Acharya/2010June22/Added/IR-153

        //-Patni-Hemant/2010Aug06/Modified/REDMINE392_Update

        //+Patni-Ravindra Acharya/2010June10/Added/IR-153

        case AMCMR_OLPTEMP_MEASUREMENT: {
            m_scan_procedure->SetTempMode(0) ;
            m_scan_procedure->SetBreathHold(l_current_protocol->GetHoldScan() & VFC_HOLD_COVERAGE) ;

            if ((GP_MAGNET(l_pqm->GetSysType()) != GP_MAGNET_50)) {
                if (!m_scan_procedure->SendInterLock()) {

                    //+Patni-Ravindra Acharya/2010Aug16/Commented/IR-153-Part1
                    //	COlpTemp::DisplayMessage("IDS_ERR_CONNECT_ACQMAN") ;



                    l_study->SetScanModeOfHead();
                    m_scan_procedure->ResetCurrentRequest();
                    l_pqm->WriteToUI(PQM_MSG_ABORT_WAIT);

                    //+Patni-Ravindra Acharya/Bug Fix - 13/07/2010
                    l_result = false;
                    break;
                }

                l_result = true;

            } else {
                l_result = PreScanGo(AMCMA_RFINTERLOCK_DONE);
            }
        }
        break;

        //-Patni-Ravindra Acharya/2010June10/Added/IR-153
        case AMCMA_RFINTERLOCK_DONE: {
            if (l_current_protocol->GetCDSInfo()->m_cds_step == CDS_STEP1_RFINTERLOCK_LEVEL) {
                l_current_protocol->GetCDSInfo()->m_cds_step = CDS_STEP2_MOVECOUCH_LEVEL;	/* MOVECOUCH_LEVEL to fly to */
            }

            if (l_current_protocol->GetCDSInfo()->m_cds_step != CDS_STEP6_DONE) {
                /* CDS_STEP6_DONE fly because when you move the bed already */
                if (CheckMovingCouch()) {
                    l_pqm->WriteToUI(PQM_MSG_ENDWAIT_CURSOR);
                    l_result = false;
                    break;
                }
            }

            l_result = PreScanGo(AMCMA_REMOTE_MOVECOUCH_DONE);
            //-Patni-Hemant/2010Feb18/Modified/ACE-Phase#3/CDS Requirement

        }
        break;

        case AMCMA_REMOTE_MOVECOUCH_DONE:
        case AMCMM_REMOTE_MOVECOUCH: {
            l_result = PSGMoveCouchLevel();
        }
        break;

        case AMCMA_SHIMCTRL_DONE: {
            //+Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
            if (!CDSCheckShimCtrlLevel(l_current_protocol)) {
                l_result = false;
                break ;
            }

            //-Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
            //Patni-HAR/2010Feb26/Modified/MSA0251-00219
            // l_pqm->WriteToUI(PQM_MSG_STATUS_CURRENT);
            CPqmShimSharedData::GetInstance()->GetShimState(& l_shimstate);
            l_study->WriteToShimData();

            if (FALSE == l_pqm->GetAMB()->PqmAMBScanSeg2Active()) {
                if (l_shimstate == SHIM_STATE_ACTIVE) {
                    l_study->SetShimmingInfo(l_protocolnum, TRUE);

                } else {
                    l_study->SetShimmingInfo(l_protocolnum, FALSE);
                }
            }

            //+Patni-MP/2009Sep18/Added/MVC004774
            //+Patni-MP/2011Mar04/Modified/REDMINE-1515
            if (m_acq_info_retry) {
                m_scan_procedure->SendAcqInfoRetry(m_acq_info_retry);
                m_acq_info_retry = 0;
                l_result = false;
                break ;
            }

            //-Patni-MP/2009Sep18/Added/MVC004774
            l_result = PreScanGo(AMCMR_UNKNOWN);
        }
        break;

        case AMCMR_UNKNOWN: {
            l_result = PSGUnknownLevel();
        }
        break;
    }

    if ((l_current_protocol->GetHALAPlannedStatus() != PLANNED_STATUS_NONE) ||
        (l_current_protocol->GetCalaPlannedStatus() != PLANNED_STATUS_NONE)	||
        (l_current_protocol->GetGatingMethod() != 0)) {

        l_pqm->WriteToUI(PQM_MSG_CHANGE_SCAN_TIME);
    }

    return l_result;

}

//+Patni-Hemant/2009Mar03/Added/ACE-1/Venus Coil
//************************************Method Header************************************
// Method Name  : PQMAcqPortString
// Author       : PATNI/ HEMANT
// Purpose      : Wrapper funtion around the utilities class function
//***********************************************************************************
void CScanTransaction::PQMAcqPortString(PqmCoil_t* f_coil_p, char* f_port_string)
{
    /*************************************************************
     * 接続可能ポートのスペース区切り文字列を作成
     *************************************************************/
    CPqmUtilities::PQMAcqPortString(f_coil_p->port, f_coil_p->portDef, f_port_string);
}

//************************************Method Header************************************
// Method Name  : CDSCheckShimCtrlLevel
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CScanTransaction::CDSCheckShimCtrlLevel(
    CPqmProtocol* f_current_protocol
)
{
    LPCTSTR FUNC_NAME = _T("CScanTransaction::CDSCheckShimCtrlLevel");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("CScanTransaction::CDSCheckShimCtrlLevel"));

    if (!m_scan_procedure) {
        return false;
    }

    /* CDS decision on whether the scan V9.50 */
    /* CDS whether to go after the scan bed, and finally determine here */
    CPqm* l_pqm_ptr = m_scan_procedure->GetPqmPtr();

    if (!l_pqm_ptr) {
        return false;
    }

    CPQMStudy* l_study = l_pqm_ptr->GetStudy();
    CVarFieldHandle* const l_vf_handle = l_study->GetVfDBHandle();
    CpqmCDS* l_pqm_cds = l_pqm_ptr->GetCDS();

    CDataManager* l_data_mgr = CDataManager::GetInstance();
    CPqmCoilData* l_coil_data = l_data_mgr->GetPQMCoilData();

    DllT* l_coil_head    = l_data_mgr->GetPQMCoilData()->GetHead();
    DllT* l_section_head = l_data_mgr->GetPQMSectionData()->GetHead();

    PqmCoil_p l_rcvcoil_p = NULL;

    if (!f_current_protocol || !l_pqm_cds || !l_coil_data) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("f_current_protocol or l_pqm_cds is invalid"));
        return false ;
    }

    int l_status = PQM_CDS_NONE ;

    if ((f_current_protocol->GetCDSInfo()->m_cds_flag)								&&
        (f_current_protocol->GetCDSInfo()->m_cds_status & VFC_CDS_STATUS_SCAN)		&&
        !(f_current_protocol->GetCDSInfo()->m_cds_status & VFC_CDS_STATUS_USE_PREVIOUS) &&
        (f_current_protocol->GetCDSInfo()->m_cds_step   < CDS_STEP4_SCAN)) {	/* Since no pre-imaging imaging */
        /*Do not measured in the same sleeping position in the past, not been measured or search for all coils*/
        l_status = l_pqm_cds->PqmCdsScanLastCheck(f_current_protocol);
        /*Wrote the study valid information has been copied from the last minute imaging */
        CCDSUtilities::PutCdsCoordinate(l_vf_handle, f_current_protocol->GetProtocol(), f_current_protocol->GetCDSInfo()->m_coordinatetbl);

        if (l_status == PQM_CDS_SCAN) {
            /*CDS scan */
            f_current_protocol->GetCDSInfo()->m_cds_step = CDS_STEP4_SCAN;	/* UNKNOWN_LEVEL fly */

            /* Coil configuration information for all CDS*/
            if (E_NO_ERROR != l_pqm_cds->PqmCoilSetCDSCoil(l_vf_handle, f_current_protocol)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PqmCoilSetCDSCoil() failed."));
            }

            /* Since CFA CDS scan was performed in the WB, the information to set the WB coil information */
            l_rcvcoil_p  = l_coil_data->PqmGetCDSRWBInSaturn(&f_current_protocol->GetSaturnCoil()->CoilList, RWB_CDS) ;

            if (l_rcvcoil_p) {	/*Information coil NULL (no subject coil) to check if it is also */
                l_study->PqmSetWBCoilCDS(l_vf_handle, f_current_protocol->GetProtocol() , f_current_protocol->GetSaturnCoil(), l_rcvcoil_p) ;
            }

            if (E_NO_ERROR != l_coil_data->PqmAllCoilGetSaturn(f_current_protocol->GetSaturnCoil(), f_current_protocol->GetCoilDBTable())) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PqmAllCoilGetSaturn failed"));
            }

            /* Set the transmit coil */
            if (!l_study->SetTxCoilInSaturn(l_vf_handle, f_current_protocol, f_current_protocol->GetCoilDBTable())) {
                /* If no output message is sent to coil Eranabi*/
                //PqmUiErrorDialog(PqmUiFetchLiteral("ERR_ILLEGAL_COIL_ID"),ERR_ERROR);

                CPQMLogMgr::GetInstance()->DisplayScanError(
                    IDS_CS_ERR_ILLEGAL_COIL_ID,
                    _T("IDS_CS_ERR_ILLEGAL_COIL_ID"),
                    SCAN_ERR_CRUCIAL, L"ScanTransaction.cpp");
            }

            //update the GUI with WB coil
            l_pqm_ptr->WriteToUI(PQM_MSG_CDS_APPLY_CMPS_COIL_CUR_PROT);

            // Degenerate Information
            // Check whether you can degenerate
            if (l_coil_data->PqmCoilIsDegeneracy(f_current_protocol->GetSaturnCoil()->numSection,
                                                 f_current_protocol->GetSaturnCoil()->RcvCoil)) {
                f_current_protocol->GetSaturnCoil()->numSignalDegeneracy
                = l_coil_data->PqmCoilGetMinChannel(f_current_protocol->GetSaturnCoil()->numSection,
                                                    f_current_protocol->GetSaturnCoil()->RcvCoil);

                AcqGetcoilSignal(f_current_protocol->GetSaturnCoil()->numSignalDegeneracy,
                                 f_current_protocol->GetSaturnCoil()->numSection,
                                 f_current_protocol->GetSaturnCoil()->RcvCoil,
                                 f_current_protocol->GetSaturnCoil()->SignalDegeneracy,
                                 l_coil_head,
                                 l_section_head);

            } else {
                f_current_protocol->GetSaturnCoil()->numSignalDegeneracy = 0;
            }

            /*Study writing */
            l_study->PutSaturnCoilDegeneracy(f_current_protocol->GetProtocol(),
                                             f_current_protocol->GetSaturnCoil());

            /* Re-sent because you changed the coil interlocks */
            if (!m_scan_procedure->SendInterLock()) {
                //PqmUiErrorDialog(PqmUiFetchLiteral("ERR_CONNECT_ACQMAN"),ERR_ERROR);
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Cannot Connect to AcqMan"));

                l_study->SetScanModeOfHead();
                m_scan_procedure->ResetCurrentRequest();

                l_pqm_ptr->WriteToUI(PQM_MSG_ABORT_WAIT);
                l_pqm_ptr->GetAMB()->PqmPmAbortAmb();
            }

            return false;

        } else {
            /* CDS does not scan*/
            if (l_status == PQM_CDS_PREV) {
                f_current_protocol->GetCDSInfo()->m_cds_status |= VFC_CDS_STATUS_USE_PREVIOUS;
            }
        }

    } else if (CDSCheckCoil_Level2 == l_pqm_cds->GetCDSCheckCoilFlag()) {

        if (f_current_protocol->GetCDSInfo()->m_cds_flag && (f_current_protocol->GetCDSInfo()->m_cds_step  < CDS_STEP4_SCAN)) {
            l_status = l_pqm_cds->PqmCdsScanLastCheck(f_current_protocol, true);
            CCDSUtilities::PutCdsCoordinate(l_vf_handle, f_current_protocol->GetProtocol(), f_current_protocol->GetCDSInfo()->m_coordinatetbl);
        }
    }

    // Set automatic coil V9.50
    // CDS do not pass but only if you do not set auto scan coils

    if ((CDSCheckCoil_Level2 == l_pqm_cds->GetCDSCheckCoilFlag()) && (PQM_CDS_NO_CONFIRM == l_status)) {
        f_current_protocol->GetCDSInfo()->m_cds_step = CDS_STEP6_DONE;
    }


    if ((PQM_CDS_NO_CONFIRM != l_status) &&
        (f_current_protocol->GetCDSInfo()->m_cds_flag) &&
        (f_current_protocol->GetCDSInfo()->m_cds_status & VFC_CDS_STATUS_AUTO_SELECT)) {

        /* CDS_STEP6_DONE cds_step is not so configured for automatic coil */
        if (f_current_protocol->GetCDSInfo()->m_cds_step != CDS_STEP6_DONE) {
            /* Set automatic coil (coil information set Study) */
            m_scan_procedure->PqmAcqAutoCoilSet(l_vf_handle, f_current_protocol);

            return false ;
        }
    }


    //Check moving bed of magnetic field distortion compensation
    //Prohibit the shooting or if a certain distance away from the center of the magnetic field. V7.20
    //
    //Field distortion correction * SDM45-00792 Masking (P.30)
    //3.11.2 Process Flow
    //3. If it exceeds the threshold distance from the center of the magnetic field config files
    //Do not start the scan and display an error message. (Prep, MAP,
    //Shimming excluded)


    if (f_current_protocol->GetSpeederType() == VFC_SPEEDER_TYPE_SENSITIVITY ||
        f_current_protocol->GetShimming()) {
        ;

    } else {
        if (l_study->PqmPrCheckSlicePosition(l_vf_handle, f_current_protocol)) {
            //PqmUiErrorDialog(PqmUiFetchLiteral("ERR_GDC_MAXIMUM_DISTANCE"),ERR_ERROR) ;
            CPQMLogMgr::GetInstance()->DisplayScanError(
                IDS_TOO_LONG_DISTANCE_FROM_CENTER,
                _T("IDS_TOO_LONG_DISTANCE_FROM_CENTER"),
                SCAN_ERR_CRUCIAL, L"ScanTransaction.cpp");

            if (f_current_protocol->GetCDSInfo()->m_cds_status == (VFC_CDS_STATUS_AUTO_SELECT | VFC_CDS_STATUS_SCAN)) {
                /* CDS must be processed after the scan is aborted */
                m_scan_procedure->pqmAcqCdsAbort(l_vf_handle, f_current_protocol);
            }

            /* AMB I should stop the error */
            CPqm* l_pqm_ptr	= m_scan_procedure->GetPqmPtr();

            l_study->SetScanModeOfHead();
            m_scan_procedure->ResetCurrentRequest();

            l_pqm_ptr->WriteToUI(PQM_MSG_ABORT_WAIT);
            l_pqm_ptr->GetAMB()->PqmPmAbortAmb();


            return false;
        }
    }


    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("Exiting CDSCheckShimCtrlLevel successfully"));

    return true ;
}
//-Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement

//***************************Method Header*************************************
//Method Name    :PSGAcqInfoLevel()
//Author         :PATNI/MRP
//Purpose        :This methods handles the post processing after AcqInfo completed
//*****************************************************************************
bool CScanTransaction::PSGAcqInfoLevel(
)const
{
    LPCTSTR FUNC_NAME = _T("CScanTransaction::PSGAcqInfoLevel");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of PSGAcqInfoLevel"));
    bool result = true;

    CScanProcedure* scan_proc = GetScanProcedure();
    CPqmPmAcqman* acqman = scan_proc->GetAcqManager();
    CPqm* pqm = acqman->GetPqm();
    CPqmProtocol* curr_protocol = scan_proc->GetScanProcProtocol();

    return result;
}

//***************************Method Header*************************************
//Method Name    : PSGRFInterLockLevel()
//Author         : PATNI/MRP
//Purpose        :This methods handles the post processing after InterLock completed
//*****************************************************************************
bool CScanTransaction::PSGRFInterLockLevel(
)const
{
    LPCTSTR FUNC_NAME = _T("CScanTransaction::PSGRFInterLockLevel");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of PSGRFInterLockLevel"));

    bool result = true;
    CScanProcedure* scan_proc = GetScanProcedure();

    if (scan_proc->IsCouchMoving()) {
        result = false;
    }

    if (!scan_proc->SendShimCtrl(NULL)) {
        return false;
    }

    return result;
}

//***************************Method Header*************************************
//Method Name    :PSGMoveCouchLevel()
//Author         :PATNI/
//Purpose        :Dummy Function
//*****************************************************************************
bool CScanTransaction::PSGMoveCouchLevel(
)
{
    LPCTSTR FUNC_NAME = _T("CScanTransaction::PSGMoveCouchLevel");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections Need to Check
    if (m_scan_procedure == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_scan_procedure pointer is NULL"));
        return false;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    CPqm* l_pqm = m_scan_procedure->GetAcqManager()->GetPqm();
    CPQMStudy* l_study = l_pqm->GetStudy();
    CPqmProtocol* l_current_protocol = m_scan_procedure->GetScanProcProtocol();
    int l_protocolnum = l_current_protocol->GetProtocol();

    //+Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
    if (l_current_protocol->GetCDSInfo()->m_cds_step == CDS_STEP2_MOVECOUCH_LEVEL) {
        l_current_protocol->GetCDSInfo()->m_cds_step = CDS_STEP3_SHIMCTRL_LEVEL;	/* SHIMCTRL_LEVELへ飛ぶように */
    }

    //-Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
    //l_couchposition = l_current_protocol->GetPqmPrMode()->GetCouchPos();
    //+Patni-DKH/2010Feb22/Added/Phase3#MCM0229-00187
    float l_shimPos = 0.0f;
    CPqmShimSharedData* l_pqmshimshareddat = CPqmShimSharedData::GetInstance();

    if (!(l_pqmshimshareddat->CopyShimPos(&l_shimPos))) {
        return false;
    }

    //-Patni-DKH/2010Feb22/Added/Phase3#MCM0229-00187
    CString str_msg(_T(""));
    bool l_result = true;

    int l_shimstate = 0; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
    CPqmShimSharedData::GetInstance()->GetShimState(& l_shimstate);
    l_pqm->GetStudy()->WriteToShimData();

    m_scan_procedure->SetCouchInfo(l_current_protocol->GetProtocol(),
                                   /*l_current_protocol->GetPosition(),*/
                                   m_couch_position,//l_current_protocol->GetPqmPrMode()->GetCouchOffset(), //commented
                                   m_couch_status);

    if (m_scan_procedure->MoveCouchProhibitionCheck(l_current_protocol) == E_ERROR) {

        //+Patni/2011Mar16/Added/V2.0/Redmine-1548
        l_pqm->GetStudy()->SetScanModeOfHead();
        m_scan_procedure->ResetCurrentRequest();
        l_pqm->WriteToUI(PQM_MSG_ABORT_WAIT);
        l_pqm->GetAMB()->PqmPmAbortAmb();
        //-Patni/2011Mar16/Added/V2.0/Redmine-1548

        return false;
    }

    PqmShim_t* l_shim = NULL;
    bool_t	l_resetShim = FALSE;

    //Patni-DKH/2010Mar03/Added/Phase3#MCM0229-00187
    //TODO: As per vantage9.50 this code is to be moved to SHIMCTRL_LEVEl handler.
    // The below code will be moved to shim control
    if (l_current_protocol->GetSpeederType() == VFC_SPEEDER_TYPE_SENSITIVITY ||
        l_current_protocol->GetShimming()) {
        ;

    } else {
        if (l_study->PqmPrCheckSlicePosition(l_study->GetVfDBHandle(), l_current_protocol)) {

            // + MEITEC /2010Jan25/Added/MVC006871
            CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
                CPqmUtilities::GetMultiLingualString(_T("IDS_TOO_LONG_DISTANCE_FROM_CENTER")),
                MPlus::EH::Information,
                _T("PQM"),
                _T("PQM"));
            // - MEITEC /2010Jan25/Added/MVC006871

            //+Patni-NP/2010Jul27/Added/IR-161
            l_study->SpeederResetDynamic(l_study->GetVfDBHandle(), l_current_protocol);
            //-Patni-NP/2010Jul27/Added/IR-161

            l_study->SetScanModeOfHead();
            m_scan_procedure->ResetCurrentRequest();
            l_pqm->WriteToUI(PQM_MSG_ABORT_WAIT);
            l_pqm->GetAMB()->PqmPmAbortAmb();
            return false;

            //PqmUiErrorDialog(PqmUiFetchLiteral("ERR_GDC_MAXIMUM_DISTANCE"),ERR_ERROR) ;
            //l_pqm->GetAMB()->PqmPmAbortAmb();
            //  l_study->SetScanModeOfHead();
            // m_scan_procedure->ResetCurrentRequest();
            //	return false;
        }
    }

    //Patni-DKH/2010feb23/Added/Phase3#MCM0229-00187
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("MCM187 Step 1"));

    const int l_as_compass_flag = l_current_protocol->GetASCOMPASSFlag();
    CpqmCDS* l_pqm_cds = l_pqm->GetCDS();
    int l_cds_status = PQM_CDS_NONE;

    if (l_current_protocol->GetCDSInfo()->m_cds_status & VFC_CDS_STATUS_USE_PREVIOUS) {
        l_cds_status = PQM_CDS_PREV;
    }

    if ((m_check_bit & (PQM_CHK_BIT_RFL_TABLE | PQM_CHK_BIT_ANATOMY)) ||
        ((l_as_compass_flag) && (l_cds_status != PQM_CDS_PREV) && (l_current_protocol->GetCDSInfo()->m_cds_step == CDS_STEP6_DONE))
       ) {

        m_scan_procedure->PQMAcqSearchRFL(l_study->GetVfDBHandle(), l_current_protocol);


    } else if (!(m_check_bit & (PQM_CHK_BIT_FIRST | PQM_CHK_BIT_COIL))) {
        PQM_TRACE(USER_FUNC_MARKER,  FUNC_NAME, _T("MCM187 Step 2"));
        l_pqm->GetSAR()->SetNewRfl(l_study->GetVfDBHandle(), l_current_protocol);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("MCM187 Step 3"));
    }

    if (!l_pqm->PqmPmIsActive(K_PM_TYPE_AMB)) {
        if (m_check_bit & PQM_CHK_BIT_CFA_TABLE) {
            m_scan_procedure->PQMAcqSearchCFA(l_study->GetVfDBHandle(), l_current_protocol);
        }

    } else if (l_pqm->GetAMB()->PqmAMBScanSeg2Active()) {
        m_scan_procedure->PQMAcqSearchCFA(l_study->GetVfDBHandle(), l_current_protocol);
    }

    PQM_TRACE(USER_FUNC_MARKER,  FUNC_NAME, _T("MCM187 Step 4"));
    l_pqm->GetSAR()->RecalculateSAR(l_study->GetVfDBHandle(), l_current_protocol);

    if (!l_pqm->PqmPmIsActive(K_PM_TYPE_AMB)) {
        l_study->SetNewSAR(l_current_protocol->GetPosition());
    }

    l_pqm->GetSARManager()->CalcSARControlAndPutVFForAll(TRUE);

    //Patni-DKH/2010Feb23/Added/Phase3#MCM0229-00187

    //+Patni-ARD/2010Mar10/Commented/IR-100 code review
    //    if (CPQMConfig::GetInstance()->GetMachineType() == _T("ONEPATH")) {
    l_study->PqmPrMaintainGdc(l_study->GetVfDBHandle(), l_current_protocol->GetProtocol());
    //    }
    //-Patni-ARD/2010Mar10/Commented/IR-100 code review

    //+ Patni-PJS/2009Aug18/Added/MVC002336
    if (l_current_protocol->GetHoldScan() & VFC_HOLD_COUCH) {
        if (m_scan_procedure->CheckParentCouchOffset(m_couch_position) != E_NO_ERROR) {
            return false;
        }
    }

    //- Patni-PJS/2009Aug18/Added/MVC002336

    /******** for Speeder ********/
    int l_message = 0 ; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections

    if ((l_message = l_pqm->GetSpeederPtr()->SpeederCheck(l_current_protocol)) >= 0) {


        str_msg = l_pqm->GetSpeederPtr()->GetErrorString(l_message);							// Added By KT/ACE-2/15-06-2009/Multilingualisation

        //+ Patni-RSG/2009Nov4/Added/MVC004843
        if (l_message == ERR_CAN_NOT_SPEEDER_MAP) {
            //+TMSC-Tanoue/2010Mar05/MSA0251-00247
            /*
            CString error_message = CPqmUtilities::GetMultiLingualString(err_str);
            MRERROR_LOG_DATA error_log_data;
            error_log_data.error_message = error_message;
            error_log_data.error_source = _T("Sequence Queue");
            error_log_data.error_id = l_message;
            error_log_data.error_level = SCAN_ERR_CRUCIAL;
            CPQMLogMgr::GetInstance()->DisplayMessageinErrorDialog(error_log_data);
            */
            CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(
                CPqmUtilities::GetMultiLingualString(str_msg), _T("Sequence Queue"),
                SCAN_WARN_INFORMATION, SCAN_WARN_SYSMODAL, SCAN_WARN_OKBUTTON);
            //-TMSC-Tanoue/2010Mar05/MSA0251-00247
            //+Patni-Sribanta/2011Mar30/Added/IR-184

        } else if (ERR_TX_INTENSITY_NOTFOUND == l_message) {
            m_scan_procedure->WarningScan(PQM_WARN_TX_INTENSITY_NOTFOUND, L"ScanTransaction.cpp") ;
            return  false;

        } else if (ERR_INTENSITY_NOTFOUND == l_message) {
            m_scan_procedure->WarningScan(PQM_WARN_INTENSITY_NOTFOUND, L"ScanTransaction.cpp") ;
            return  false;

        } else {
            //-Patni-Sribanta/2011Mar30/Added/IR-184
            //+Patni-ARD/2009-Nov-25/Modified/DeFT# MVC003149 + MVC005179
            CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
                CPqmUtilities::GetMultiLingualString(str_msg),
                MPlus::EH::Information, _T("PQM"), _T("PQM"));
            //-Patni-ARD/2009-Nov-25/Modified/DeFT# MVC003149 + MVC005179
        }//-Patni-MP/2011Mar14/Added/IR-184

        //+ Patni-RSG/2009Nov4/Added/MVC004843
        l_study->SetScanModeOfHead();

        //+Patni-NP/2010Jul27/Added/IR-161
        l_study->SpeederResetDynamic(l_study->GetVfDBHandle(), l_current_protocol);
        //-Patni-NP/2010Jul27/Added/IR-161

        m_scan_procedure->ResetCurrentRequest();
        l_study->SetAcqStatus(l_current_protocol, C_PR_STATUS_WAIT);
        l_pqm->WriteToUI(PQM_MSG_ABORT_WAIT);
        l_pqm->GetAMB()->PqmPmAbortAmb();
        return  false;
    }

    int l_status = l_pqm->GetSpeederPtr()->IntensityCheck(l_current_protocol) ;

    if (l_status != E_NO_ERROR) {
        //+Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
        /*if (l_current_protocol->GetAcqStatus() == C_PR_STATUS_CURRENT) {
            m_scan_procedure->pqmAcqCdsAbort(l_study->GetVfDBHandle(), l_current_protocol);
        }*/

        //-Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement

        //+Patni-NP/2010Jul27/Added/IR-161
        l_study->SpeederResetDynamic(l_study->GetVfDBHandle(), l_current_protocol);
        //-Patni-NP/2010Jul27/Added/IR-161
        m_scan_procedure->WarningScan(l_status, L"ScanTransaction.cpp") ;

        return  false;
    }

    //+Patni-DKH/2010Mar03/Added/Phase3#MCM0229-00187
    CPqmCoilData* l_coil_data = CDataManager::GetInstance()->GetPQMCoilData();

    int l_rfl_limit = 0, l_tune_limit = 0;

    if (l_pqm->IsSaturnCoilSystem()) {	//Saturn Coil

        if (l_coil_data->PqmGetRflTuneInSaturn(
                l_current_protocol->GetCoilDBTable(),
                &l_rfl_limit, &l_tune_limit) != E_NO_ERROR) {

            l_rfl_limit = GLOBAL_RFL_LIMIT;
            l_tune_limit = GLOBAL_TUNE_LIMIT;
        }

    } else {
        CCoil coil;
        l_coil_data->PqmCoilGetCoil(l_current_protocol->GetPqmPrMode()->GetRecvCoilId(), coil);

        if (coil.GetCoil()) {

            l_rfl_limit = coil.GetRFLLimit();
            l_tune_limit = coil.GetTuneLimit();

        } else {

            l_tune_limit = GLOBAL_TUNE_LIMIT;
            l_rfl_limit = GLOBAL_RFL_LIMIT;
        }
    }

    const bool  l_is_positon_shim_active = ((ABSF(l_shimPos - m_couch_position) <= (float)l_rfl_limit) && (l_shimstate == SHIM_STATE_ACTIVE));

    //-Patni-DKH/2010Mar03/Added/Phase3#MCM0229-00187
    if (m_scan_procedure->GetScanProcProtocol()->GetShimming()) {
        // If this protocol is shimming sequence, shimming offset should be set before prescan
        // + SHIMMING Lokesh 13 March 2008

        //Patni-DKH/2010Mar03/Added/Phase3#MCM0229-00187
        if (l_shimstate == SHIM_STATE_WAIT ||
            m_check_bit & (PQM_CHK_BIT_FIRST | PQM_CHK_BIT_COIL | PQM_CHK_BIT_CFA_TABLE)) {
            l_shim = l_study->SetShimmingOffset(PQM_SHIM_USE_DEFAULT, l_protocolnum);

        } else {
            l_shim = l_study->SetShimmingOffset(PQM_SHIM_USE_VARFIELD, l_protocolnum);
        }

        l_result = m_scan_procedure->SendShimCtrl(l_shim);

        //+Patni-HAR/2009Aug11/Added/Memory Leaks
        DEL_PTR(l_shim);

        //-Patni-HAR/2009Aug11/Added/Memory Leaks
        return l_result;
    }

    //+Patni-DKH/2010Mar03/Commented/Phase3# MCM0229-00187
    /*else if ((m_check_bit & (PQM_CHK_BIT_FIRST | PQM_CHK_BIT_ANATOMY))
    || (m_check_bit && (l_shimstate == SHIM_STATE_ACTIVE))
    ) {

      l_shim = l_study->SetShimmingOffset(PQM_SHIM_RESET, -1);
      l_result = m_scan_procedure->SendShimCtrl(l_shim);
      l_pqm->GetAcqManager()->GetProtocolModeLastScan()->SetAnatomy(0);

    }*/
    //-Patni-DKH/2010Mar03/Commented/Phase3# MCM0229-00187
    //+Patni-DKH/2010Mar03/Added/Phase3#MCM0229-00187
    else if ((m_check_bit & (PQM_CHK_BIT_FIRST | PQM_CHK_BIT_COIL))) {
        l_resetShim = TRUE;

    } else if (m_check_bit  & (PQM_CHK_BIT_CFA_TABLE)) {
        if (l_current_protocol->GetCfaProt() != -1) {
            CPqmProtocol* l_pqmprotocol = l_study->GetItemFromProtocol(l_current_protocol->GetCfaProt());

            if (NULL == l_pqmprotocol) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("l_pqmprotocol pointer is NULL"));
                return false;
            }

            if (l_pqmprotocol->GetShimming()) {
                l_current_protocol->SetShimProt(l_pqmprotocol->GetProtocol());
                m_scan_procedure->PQMAcqReuseShim(l_study->GetVfDBHandle(),
                                                  l_current_protocol->GetShimProt());
                return true;
            }

            l_pqmprotocol = l_study->GetItemFromProtocol(l_pqmprotocol->GetShimProt());



            if (l_pqmprotocol  &&
                (ABSF(l_pqmprotocol->GetPqmPrMode()->GetCouchPos() - m_couch_position) > (float)l_rfl_limit)) {
                l_current_protocol->SetCfaProt(-1);
                l_resetShim = TRUE;

            } else if (!l_pqmprotocol && (l_shimstate == SHIM_STATE_ACTIVE)) {
                l_resetShim = TRUE;

            } else if (l_pqmprotocol) {
                l_current_protocol->SetShimProt(l_pqmprotocol->GetProtocol());
                m_scan_procedure->PQMAcqReuseShim(l_study->GetVfDBHandle(),
                                                  l_current_protocol->GetShimProt());
                return true;
            }

            if (l_current_protocol->GetShimProt() == -1) {
                m_scan_procedure->PQMAcqSearchShim(l_study->GetVfDBHandle(),
                                                   l_current_protocol);

                if (l_current_protocol->GetShimProt() != -1) {
                    m_scan_procedure->PQMAcqReuseShim(l_study->GetVfDBHandle(),
                                                      l_current_protocol->GetShimProt());
                    l_resetShim = FALSE;
                    return true;
                }
            }

        } else if (l_shimstate == SHIM_STATE_ACTIVE) {
            l_resetShim = TRUE;
        }

    }

    else if ((l_is_positon_shim_active && (!l_as_compass_flag)) ||
             (l_is_positon_shim_active && ((l_as_compass_flag) && (l_cds_status != PQM_CDS_PREV) && (l_current_protocol->GetCDSInfo()->m_cds_step != CDS_STEP6_DONE))) ||
             (l_is_positon_shim_active && ((l_as_compass_flag) && (l_cds_status == PQM_CDS_PREV) && (l_current_protocol->GetCDSInfo()->m_cds_step == CDS_STEP6_DONE)))
            ) {

        VfPathElem_t	l_activeShimPath[SVD_ST_PROCESSED];
        CPqmShimSharedData::GetInstance()->GetShimPath(l_activeShimPath);
        l_current_protocol->SetShimProt(l_activeShimPath[0].index);

    } else if (!l_pqm->PqmPmIsActive(K_PM_TYPE_AMB)) {

        float l_resetpos = PQM_COUCH_INIT_POSITION;
        m_scan_procedure->PQMAcqSearchShim(l_study->GetVfDBHandle(),
                                           l_current_protocol);

        if (l_current_protocol->GetShimProt() != -1) {
            if ((l_as_compass_flag) && (l_cds_status != PQM_CDS_PREV) && (l_current_protocol->GetCDSInfo()->m_cds_step == CDS_STEP6_DONE)) {
                if (l_current_protocol->GetRflProt() != -1) {
                    l_pqmshimshareddat->ShimRegistTuneInfo(NULL, &l_resetpos, NULL);
                }
            }

            l_pqmshimshareddat->ShimRegistTuneInfo(NULL, NULL, &l_resetpos);
            m_scan_procedure->PQMAcqReuseShim(l_study->GetVfDBHandle(),
                                              l_current_protocol->GetShimProt());

            DEL_PTR(l_shim);

            return true;

        } else if (l_shimstate == SHIM_STATE_ACTIVE) {
            l_pqmshimshareddat->ShimRegistTuneInfo(NULL, NULL, &l_resetpos);
            l_resetShim = TRUE;
        }
    }

    //+Patni-MP/2010Dec02/Modified/REDMINE-932

    if (l_resetShim == TRUE) {

        l_shim = l_study->SetShimmingOffset(PQM_SHIM_RESET, -1);
        l_result = m_scan_procedure->SendShimCtrl(l_shim);
        return l_result;
    }

    //-Patni-MP/2010Dec02/Modified/REDMINE-932
    //-Patni-DKH/2010Mar03/Added/Phase3#MCM0229-00187
    //+Patni-Meghna-DKH/2010Apr12/Commented/Phase3#MCM0229-00187
    /*else {
        l_result = PreScanGo(AMCMA_SHIMCTRL_DONE);
    }*/
    //-Patni-Meghna-DKH/2010Apr12/Commented/Phase3#MCM0229-00187
    l_result = PreScanGo(AMCMA_SHIMCTRL_DONE);

    //+Patni-HAR/2009Aug11/Added/Memory Leaks
    DEL_PTR(l_shim);

    //-Patni-HAR/2009Aug11/Added/Memory Leaks
    return l_result;

}

//***************************Method Header*************************************
//Method Name    : PSGShimCtrlLevel()
//Author         : PATNI/
//Purpose        : Dummy Function
//*****************************************************************************
bool CScanTransaction::PSGShimCtrlLevel(
)const
{
    LPCTSTR FUNC_NAME = _T("CScanTransaction::PSGShimCtrlLevel");

    bool result = true;

    CString str_msg(_T(""));
    str_msg.Format(_T("%d"), result);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CScanTransaction::PSGShimCtrlLevel"));

    return result;
}

//***************************Method Header*************************************
//Method Name    : PSGUnknownLevel()
//Author         : PATNI/
//Purpose        : This function will be called when an unknown request got from Acqman
//*****************************************************************************
bool CScanTransaction::PSGUnknownLevel(
)const
{
    LPCTSTR FUNC_NAME = _T("CScanTransaction::PSGUnknownLevel");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of PSGUnknownLevel"));

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections Need to Check
    if (m_scan_procedure == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_scan_procedure pointer is NULL"));
        return false;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    CPqmPmAcqman* acqman = m_scan_procedure->GetAcqManager();
    CPqm* pqm = acqman->GetPqm();
    CPQMStudy* l_study = pqm->GetStudy();//+Patni-NP/2010Jul27/Added/IR-161

    CPqmProtocol* curr_protocol = m_scan_procedure->GetScanProcProtocol();
    CPqmProtocol* last_protocol = pqm->GetStudy()->GetLastDone();

    CPqmPrMode* curr_prtotocol_mode = curr_protocol->GetPqmPrMode();
    CVarFieldHandle* const l_vf_handle = pqm->GetStudy()->GetVfDBHandle();
    CPqmPrMode* last_prtotocol_mode = NULL;

    if (last_protocol != NULL) {
        last_prtotocol_mode = last_protocol->GetPqmPrMode();
    }

    // + OLD_DB@AMT 20080905
    CPqmCoilData* l_coil_data = CDataManager::GetInstance()->GetPQMCoilData();
    //CCoil *coil = pqm->GetCoilDB()->GetCoil(curr_prtotocol_mode->GetRecvCoilId());
    CCoil coil ;
    l_coil_data->PqmCoilGetCoil(curr_prtotocol_mode->GetRecvCoilId(), coil);
    // - OLD_DB@AMT 20080905

    if (last_protocol != NULL) {

        CString l_str(_T(""));
        l_str.Format(_T("Last Protocol : Scan Number = %d, Home Position = %f \r\n \
            Current Protocol : Scan Number = %d, Home Position = %f "), last_prtotocol_mode->GetScanNumber(),
                     last_prtotocol_mode->GetHomePos(), curr_prtotocol_mode->GetScanNumber(), curr_prtotocol_mode->GetHomePos());

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);

        if (last_prtotocol_mode->GetScanNumber() == curr_prtotocol_mode->GetScanNumber() &&
            fabs(last_prtotocol_mode->GetHomePos() - curr_prtotocol_mode->GetHomePos()) > 0.00001) {

            //+Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
            if (curr_protocol->GetCDSInfo()->m_cds_status == (VFC_CDS_STATUS_AUTO_SELECT | VFC_CDS_STATUS_SCAN)) {
                /* CDS must be processed after the scan is aborted */
                m_scan_procedure->pqmAcqCdsAbort(l_vf_handle, curr_protocol);
            }

            //+Patni-NP/2010Jul27/Added/IR-161
            l_study->SpeederResetDynamic(l_study->GetVfDBHandle(), curr_protocol);
            //-Patni-NP/2010Jul27/Added/IR-161

            {
                char navi[256];
                //Patni-HAR/2010Jun01/Added/MVC008153
                sprintf(navi, "WARN_PRESCAN_HOME_POS1 LastHomePos = %f HomePos = %f",
                        last_prtotocol_mode->GetHomePos(), curr_prtotocol_mode->GetHomePos());

                //ERROR_LOG(navi);
                l_str = navi ;
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          l_str);
            }

            //-Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
            pqm->GetStudy()->SetScanModeOfHead();
            m_scan_procedure->ResetCurrentRequest();


            //+Patni-HAR/2010Jun04/Modified/MVC008153
            pqm->WriteToUI(PQM_MSG_ABORT_WAIT);
            pqm->GetAMB()->PqmPmAbortAmb();

            l_str = CPqmUtilities::GetMultiLingualString(_T("IDS_WARN_PRESCAN_HOME_POS")) ;
            CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(l_str, _T("PQM"),
                    SCAN_WARN_INFORMATION, SCAN_WARN_SYSMODAL, SCAN_WARN_OKBUTTON);

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      l_str);
            //-Patni-HAR/2010Jun04/Modified/MVC008153

            //-Patni-HAR/2010Jun04/Modified/MVC008153
            return false;
        }
    }

    if ((!(m_scan_procedure->GetPreScanStatus() & STATUS_IEC_CHECK)) &&
        (m_check_bit == 0)) {

        m_scan_procedure->SetPreScanStatus(
            m_scan_procedure->GetPreScanStatus() | STATUS_IEC_CHECK);

        const int sts = pqm->GetdBdt()->IECCheck(l_vf_handle, curr_protocol);
        //+Patni-HAR/2009Mar30/Modified/MRS SCAN Fix -
        // During T-Slip PAS scan Ok-Cancel messagebox display
        // Ok button handling is not proper
        // Try to handle OK button handling here but during duplicate protocol for
        //MRS scan was not working Need to change OK Button handling

        if (sts != E_NO_ERROR) {
            return false;
        }

        //+Patni-HAR/2009Mar28/Added/Actual Machine Test
        //if (sts == E_NO_ERROR) {
        //    bool l_return = false;
        //    l_return = PreScanGo(AMCMR_UNKNOWN);
        //    m_scan_procedure->SetProcessing(m_scan_procedure->GetProcessing() &
        //        ~PQM_ACQ_BIT_WARNING);
        //    return l_return;
        //} else {
        //-Patni-HAR/2009Mar28/Added/Actual Machine Test
        //	return false;
        //}
    }

    if (curr_protocol->GetProbeScan() == VFC_PROBE_RTMC ||
        curr_protocol->GetPrepScan() ||
        curr_protocol->GetAcqSplitMode() == VFC_ACQ_SPLIT_MODE_PREP) {

        if (m_scan_procedure->CheckPlan(curr_protocol) != E_NO_ERROR) {

            //+Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
            if (curr_protocol->GetCDSInfo()->m_cds_status == (VFC_CDS_STATUS_AUTO_SELECT | VFC_CDS_STATUS_SCAN)) {
                /* CDS must be processed after the scan is aborted */
                m_scan_procedure->pqmAcqCdsAbort(l_vf_handle, curr_protocol);
            }

            //-Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
            //+Patni-NP/2010Jul27/Added/IR-161
            l_study->SpeederResetDynamic(l_study->GetVfDBHandle(), curr_protocol);
            //-Patni-NP/2010Jul27/Added/IR-161

            pqm->GetStudy()->SetScanModeOfHead();
            m_scan_procedure->ResetCurrentRequest();
            //+ Patni-MSN/2009Dec16/Modify/RMC Positioning
            pqm->GetStudy()->SetAcqStatus(curr_protocol, C_PR_STATUS_WAIT);

            pqm->WriteToUI(PQM_MSG_ABORT_WAIT); //"ABORT_WAIT"
            //+ Patni-MSN/2009Dec16/Modify/RMC Positioning

            pqm->GetAMB()->PqmPmAbortAmb();
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CheckPlan E_ERROR"));
            //Patni-PJS/2009Aug12/Modify/MVC002490
            return false;
        }
    }

    if (curr_prtotocol_mode->GetScanNumber() > 0) {

        BITFLD_DECL(mask, K_ALL_PR) = {0}; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections

        float offset = COUCH_OFFSET(curr_protocol) / 1000.0f;

        BITFLD_INIT(mask, K_ALL_PR, 0);

        curr_prtotocol_mode->SetCouchOffset(offset);

        if (E_NO_ERROR != pqm->GetStudy()->SetProtocolValues(l_vf_handle,
                curr_protocol->GetProtocol(),
                mask,
                K_PR_COUCH_OFFSET,
                &offset,
                sizeof(float),
                NULL)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetProtocolValues failed."));
        }

        CPqm*		l_pqm_ptr	 = m_scan_procedure->GetPqmPtr();

        if ((!(curr_protocol->GetFluoro())) &&
            (m_scan_procedure->GetPreScanStatus() & STATUS_IEC_CHECK)
            || l_pqm_ptr->GetAMB()->PqmAMBScanSeg2Active() == TRUE) {

            if (curr_protocol->GetCouchOffsetFlag()) {
                //+Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement
                if (curr_protocol->GetCDSInfo()->m_cds_status == (VFC_CDS_STATUS_AUTO_SELECT | VFC_CDS_STATUS_SCAN)) {
                    /* CDS must be processed after the scan is aborted */
                    m_scan_procedure->pqmAcqCdsAbort(l_vf_handle, curr_protocol);
                }

                //-Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement

                CString l_str(_T(""));
                l_str.Format(_T("Current Protocol : Scan Number = %d, Home Position = %f "),
                             curr_prtotocol_mode->GetScanNumber(), curr_prtotocol_mode->GetHomePos());

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);

                //+Patni-NP/2010Jul27/Added/IR-161
                l_study->SpeederResetDynamic(l_study->GetVfDBHandle(), curr_protocol);
                //-Patni-NP/2010Jul27/Added/IR-161

                pqm->GetStudy()->SetScanModeOfHead();
                m_scan_procedure->ResetCurrentRequest();
                pqm->GetStudy()->SetAcqStatus(curr_protocol, C_PR_STATUS_WAIT);

                pqm->WriteToUI(PQM_MSG_ABORT_WAIT); //"ABORT_WAIT"
                l_pqm_ptr->GetAMB()->PqmPmAbortAmb();

                //+Patni-ARD/2009-Nov-26/Modified/DeFT# MVC003149 + MVC005179
                //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
                //CPQMLogMgr::GetInstance()->DisplayScanError(
                //    IDS_WARN_PRESCAN_HOME_POS, _T("IDS_WARN_PRESCAN_HOME_POS"), SCAN_ERR_NOTIFY,
                //    L"ScanTransaction.cpp");

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("UnKnown position of protocol"));
                //-Patni-ARD/2009-Nov-26/Modified/DeFT# MVC003149 + MVC005179

                return true;
            }

            if (PLAN_CHECK(curr_protocol)) {

                //Patni-Raj/2010Jan14/Added/PH#3#/Debug Window
                CPQMDebugInfo::GetInstance(pqm)->PqmDebugAcqMonitor(_T("Change SG Offset to Current Position (%f)"), offset);


                CString l_str(_T(""));
                l_str.Format(_T("Change SG Offset to Current Position (%f)"), offset);

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);

                ClibVfToUiAdapter vuiAdapter(l_vf_handle);
                vuiAdapter.VuiShiftOffsetVector(curr_protocol->GetProtocol(),
                                                VUI_SHIFT_MODE_SG,
                                                VUI_SHIFT_Z,
                                                offset);

            } else {
                //Patni-Raj/2010Jan14/Added/PH#3#/Debug Window
                CPQMDebugInfo::GetInstance(pqm)->PqmDebugAcqMonitor(_T("Change VG Offset to Home Position (%f)"), -offset);
                CString l_str(_T(""));
                l_str.Format(_T("Change VG Offset to Home Position (%f)"), -offset);

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);

                ClibVfToUiAdapter vuiAdapter(l_vf_handle);
                vuiAdapter.VuiShiftOffsetVector(curr_protocol->GetProtocol(),
                                                VUI_SHIFT_MODE_VG,  //Patni-SS/20091013/Modified/MSA0248-00025
                                                VUI_SHIFT_Z,
                                                -offset);
            }

            curr_protocol->SetCouchOffsetFlag(TRUE);
        }
    }

    //Patni-NP/20100311/Added/MCM0229-00208
    if (pqm->IsSaturnCoilSystem()) {
        /* Saturn Coil */
        //Patni-NP/20100528/Added/MVC007958
        //Patni-HAR/2010Sep10/Modified/V1.30#MVC008909
        if (curr_protocol->GetAcqStatus() == C_PR_STATUS_WAIT) {
            CPQMStudy* l_study = pqm->GetStudy();
            l_study->SetTxCoilInSaturn(l_study->GetVfDBHandle(), curr_protocol, curr_protocol->GetCoilDBTable());
        }

        //Patni-NP/20100528/Commented/MVC007958
        //curr_protocol->SetCouchOffsetFlag(FALSE);
        //Patni-NP/20100311/Added/MCM0229-00208

    } else {
        /* Vantage */
        // + OLD_DB@AMT 20080905
        m_scan_procedure->SetCoilName(l_vf_handle,
                                      curr_protocol, &coil);
        // + OLD_DB@AMT 20080905
    }

    if (!m_scan_procedure->SetPSData(l_vf_handle, m_check_bit)) {
        //Patni+Pavan/20011May11/Added/IR-187/
        // Before doing abort current scan should reset.
        pqm->GetStudy()->SetScanModeOfHead();
        m_scan_procedure->ResetCurrentRequest();
        pqm->GetStudy()->SetAcqStatus(curr_protocol, C_PR_STATUS_WAIT);

        pqm->WriteToUI(PQM_MSG_ABORT_WAIT); //"ABORT_WAIT"
        pqm->GetAMB()->PqmPmAbortAmb();
        //acqman->GetPQMSocket()->Close();//Patni+Pavan/20011May11/Commented/IR-187/


        //+Patni-ARD/2009-Nov-25/Modified/DeFT# MVC003149 + MVC005179
        //CString string = L"";
        //string.LoadString(IDS_ERR_CONNECT_ACQMAN);
        //CString temp_string;                   // Added By KT/ACE-2/15-06-2009/Multilingualisation
        //temp_string =  CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_CONNECT_ACQMAN"));    // Added By KT/ACE-2/15-06-2009/Multilingualisation
        //  CPQMLogMgr::GetInstance()->DisplayError(string, // Added By KT/ACE-2/15-06-2009/Multilingualisation
        //CPQMLogMgr::GetInstance()->DisplayError(temp_string, // Added By KT/ACE-2/15-06-2009/Multilingualisation
        //                                        _T("ScanTransaction.cpp"), __LINE__, ERROR_LOG_CRUCIAL1 ,
        //                                        ERR_ID, APP_NAME);

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Cannot Connect to AcqMan"));

        //CPQMLogMgr::GetInstance()->WriteEventLog(string, ERROR_LOG_CRUCIAL1  , _T("ScanTransaction.cpp"));
        //-Patni-ARD/2009-Nov-25/Modified/DeFT# MVC003149 + MVC005179
    }

    return true;
}

//****************************Method Header************************************
//Method Name   :AcqSndMoveCouch()
//Author        :PATNI/MSN
//Purpose       :
//*****************************************************************************
bool CScanTransaction::AcqSendMoveCouch(
    const int	f_function, const float f_position
)const
{
    //    PqmAcqRequest_t	acqRequest;
    if (m_scan_procedure) {				//Patni-MJC/2009Aug17/Modified/cpp test corrections
        m_scan_procedure->AcqSendMoveCouch(f_function, f_position);
        return true;

    } else {							//Patni-MJC/2009Aug17/Modified/cpp test corrections
        return false;
    }
}

//+Patni-HAR/2010Mar10/Added/MSA0251-00279
//********************************Method Header********************************
// Method Name  : AbortScan
// Author       : PATNI / HAR
// Purpose      :
//*****************************************************************************
void CScanTransaction::AbortScan(
)const
{
    m_scan_procedure->AcquisitionScanAbort();

    CString l_strErrMsg =  CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_DATABASE_UPDATE_FAILED"));

    MRERROR_LOG_DATA l_error_log_data;
    l_error_log_data.error_message = l_strErrMsg;
    l_error_log_data.error_source = _T("PQM");

    CPQMLogMgr::GetInstance()->DisplayMessageinErrorDialog(l_error_log_data, MPlus::EH::Information);
}
//-Patni-HAR/2010Mar10/Added/MSA0251-00279//+Patni-Hemant/2010Feb18/Added/ACE-Phase#3/CDS Requirement

//*******************************Method Header*********************************
//Method Name    :DisplayMoveCouchMessageinWarningDialog()
//Author         :PATNI/MSN
//Purpose        :
//*****************************************************************************
void CScanTransaction::DisplayMoveCouchMessageinWarningDialog(
    const int f_errorid,
    const CString& f_errmsg,
    const int buttontype,
    const float f_offset
)
{

    LPCTSTR FUNC_NAME = _T("CScanTransaction::DisplayMoveCouchMessageinWarningDialog");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections Need to Check
    if (m_scan_procedure == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_scan_procedure pointer is NULL"));
        return ;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    //+TMSC-Tanoue/2010Mar05/MSA0251-00247
    int l_warning_level = SCAN_WARN_INFORMATION;

    switch (f_errorid) {
        case IDS_MOVE_COUCH_CONFIRM:
        case IDS_MOVE_COUCH_CONFIRM_PLAN:
        case IDS_MOVE_COUCH_CONFIRM_ROUND_LIMIT:
            l_warning_level = SCAN_WARN_CRUCIAL;
            break;

        default:
            l_warning_level = SCAN_WARN_INFORMATION;
            break;
    }

    //-TMSC-Tanoue/2010Mar05/MSA0251-00247
    bool l_display_warn_flag = true;
    CPqmProtocol* l_curr_protocol = m_scan_procedure->GetScanProcProtocol();

    if (PQM_SLAVE_PROTOCOL == m_scan_procedure->GetPqmPtr()->IsMasterSlaveProtocol(l_curr_protocol) &&
        ((IDS_MOVE_COUCH_CONFIRM_PLAN == f_errorid) || (IDS_MOVE_COUCH_CONFIRM == f_errorid))) {

        l_display_warn_flag = false;
    }

    MPLUSERRORUSERRESPONSE_e    l_response = MPlusErrResponse_Error ; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections

    if (l_display_warn_flag) {
        CString l_warnapp_msg(_T(""));
        l_warnapp_msg.LoadString(f_errorid);
        l_response = (MPLUSERRORUSERRESPONSE_e) CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(
                         //l_warnapp_msg,		//Added By KT/ACE-2/11-06-2009/Multilingualisation
                         CPqmUtilities::GetMultiLingualString(f_errmsg),
                         _T("Sequence Queue"),
                         l_warning_level, //TMSC-Tanoue/2010Mar05/MSA0251-00247
                         SCAN_WARN_SYSMODAL,
                         buttontype
                     );
    }

    //+ Patni-MSN/2009Aug26/Modified/MVC004250


    if (((f_errorid == IDS_MOVE_COUCH_CONFIRM_OUT_RANGE) || (f_errorid == IDS_MOVE_COUCH_CONFIRM_JUST_LIMIT) || (f_errorid == IDS_MOVE_COUCH_CONFIRM_NO_PARENT)) && (l_response == MPlusErrResponse_OK)) {
        //- Patni-MSN/2009Aug26/Modified/MVC004250
        if (l_curr_protocol != NULL) {
            //+Patni-Meghna-DKH/2010Apr12/Modified/Phase3#MCM0229-00187
            //PreScanGo(AMCMM_REMOTE_MOVECOUCH);
            PostProcess(m_couch_position, m_couch_status, AMCMM_REMOTE_MOVECOUCH);

        } else {
            //PqmDebugAcqMonitor("curObj is NULL");
        }

        m_scan_procedure->SetProcessing(m_scan_procedure->GetProcessing() & ~PQM_ACQ_BIT_WARNING);
        return;
    }

    if (l_response == MPlusErrResponse_OK || false == l_display_warn_flag) { //Redmine-2153
        //+ Patni-SS/2009Jun10/Added/JFT#170
        //+Patni-Sribanta/2010Sep28/Commented/MVC009118
        //m_scan_procedure->GetAcqManager()->GetPqm()->WriteToUI(PQM_MSG_ENDWAIT_CURSOR);

        AcqSendMoveCouch(AMCMM_REMOTE_MOVECOUCH, f_offset);
        m_scan_procedure->SetProcessing(m_scan_procedure->GetProcessing() & ~PQM_ACQ_BIT_WARNING);

    } else if (l_response == MPlusErrResponse_CANCEL) {
        if (l_curr_protocol != NULL) {
            //+Patni-Meghna-DKH/2010Apr12/Modified/Phase3#MCM0229-00187
            //PreScanGo(AMCMM_REMOTE_MOVECOUCH);
            PostProcess(m_couch_position, m_couch_status, AMCMM_REMOTE_MOVECOUCH);
            //-Patni-Meghna-DKH/2010Apr12/Modified/Phase3#MCM0229-00187

        } else {
            //PqmDebugAcqMonitor("curObj is NULL");
        }

        m_scan_procedure->SetProcessing(m_scan_procedure->GetProcessing() & ~PQM_ACQ_BIT_WARNING);
    }
}

//*******************************Method Header*********************************
//Method Name    :AcquisitionWaitForStart()
//Author         :PATNI/HAR
//Purpose        :
//*****************************************************************************
void CScanTransaction::AcquisitionWaitForStart(
    AM_AcqManToClient_t* data
)
{
    LPCTSTR FUNC_NAME = _T("CScanTransaction::AcquisitionWaitForStart");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of AcquisitionWaitForStart"));

    CScanProcedure* scan_proc = GetScanProcedure();

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (scan_proc == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("scan_proc pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    CPqmPmAcqman* acqman = scan_proc->GetAcqManager();

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (acqman == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("acqman pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    CPqm* pqmptr = acqman->GetPqm();

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (pqmptr == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("pqmptr pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    CPqmProtocol* curr_protocol = scan_proc->GetScanProcProtocol();

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (curr_protocol == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("curr_protocol pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    CVoiceManager* voice_manager = CVoiceManager::GetInstance();

    //+ Patni-Dhanesh/2009Nov04/Added/Auto Voice Defect Fix - MVC004941
    if (voice_manager == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("voice_manager pointer is NULL"));
        return;
    }

    //- Patni-Dhanesh/2009Nov04/Added/Auto Voice Defect Fix - MVC004941

    //  data->acqman.hold = 8;
    scan_proc->SetScanStart(PQM_SCAN_RESTART); // will be used in PqmAcqStart and PqmAcqStartOrNext methods
    CString str(_T(""));
    str.Format(L"Acqman hold value = %d", data->acqman.hold);

    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, str);
    //Patni-Raj/2010Jan14/Added/PH#3#/Debug Window
    CPQMDebugInfo::GetInstance(pqmptr)->PqmDebugAcqMonitor(_T("WaitForStart HOLD = 0x%x"), data->acqman.hold);

    //Patni-PJS/2010Sep8/Modified/TMSC-REDMINE-685/As per base code
    //if (scan_proc->GetAPCHold() /*& (data->acqman.hold & VFC_HOLD_APC)*/) {
    if (scan_proc->GetAPCHold() && (data->acqman.hold & VFC_HOLD_APC)) {

        pqmptr->WriteToUI(PQM_MSG_APC_START);
        scan_proc->SetScanStart(PQM_APC_START); // will be used in PqmAcqStart and PqmAcqStartOrNext methods

    } else if (data->acqman.hold & VFC_HOLD_PREP) {

        scan_proc->SetWaitForPrepScan(TRUE);
        scan_proc->SetEnablePrepVoice(TRUE);

        //+Patni-Hemant/2010Jun4/Added/MaFT/TMSC-REDMINE-23/Visual Prep Case
        //
        //write detail into PQM debug window
        //PqmDebugAcqMonitor("waitForPrepChange = %d",
        //    PqmAcqGlobal.waitForPrepChange);
        CPQMDebugInfo::GetInstance(pqmptr)->PqmDebugAcqMonitor(_T("waitForPrepChange = %d"),
                scan_proc->GetWaitForPrepScan());
        //-Patni-Hemant/2010Jun4/Added/MaFT/TMSC-REDMINE-23/Visual Prep Case

        pqmptr->GetAMB()->PqmAmbPrepStart();
        pqmptr->WriteToUI(PQM_MSG_INJECT_TIME_VISIBLE); //   Make Inject time Button Visible

    } else if (data->acqman.hold & VFC_HOLD_COVERAGE) {

        if (scan_proc->GetBreathHold()) {
            //pqmptr->WriteToUI(PQM_MSG_SCAN_START_TRUE);
            pqmptr->WriteToUI(PQM_MSG_SCAN_START_BREATH_HOLD);
            pqmptr->WriteToUI(PQM_MSG_SCAN_PAUSE_FALSE);
            pqmptr->WriteToUI(PQM_MSG_SCAN_ABORT_TRUE);


            //Display Inject Time button, if required.
            if (curr_protocol->GetPrepScan()
                && ((curr_protocol->GetAcqSplitMode() != VFC_ACQ_SPLIT_MODE_PREP)
                    || (curr_protocol->GetDynamic() && (curr_protocol->GetAcqSplitMode() == VFC_ACQ_SPLIT_MODE_PREP)))) {
                pqmptr->WriteToUI(PQM_MSG_INJECT_TIME_VISIBLE);
            }

            return; // added by sarjeet as per base code
            // if we not add this, On breath hold and autovice option it will not work;
        }

        if (curr_protocol->GetPrepScan() && !(scan_proc->GetEnablePrepVoice())) {

            scan_proc->ScanContinue();

            return;
        }

        /* AUTO_VOICE */

        //+ Patni-Dhanesh/2009Nov04/Added/Auto Voice Defect Fix - Code Review
        if (curr_protocol->GetPlnAutoVoice()) {

            int l_length = voice_manager->GetVoiceLength(
                               curr_protocol->GetPlnAutoVoice());


            const VftTimeCtrlParam_t* l_avtime_ctrl = scan_proc->GetAvTimeCtrl();

            for (int segment = 0; segment < l_avtime_ctrl->numSegment; segment++) {
                if (data->statusList.lines < l_avtime_ctrl->segment[segment].repeat) {
                    break;
                }
            }

            if ((l_length > 0) && (l_avtime_ctrl->segment[segment].start > l_length)) {

                voice_manager ->CmdProg(l_avtime_ctrl->segment[segment].start,
                                        0, curr_protocol->GetPlnAutoVoice());

            } else {
                voice_manager->CmdDelay(pqmptr->GetAvInterval(),
                                        curr_protocol->GetPlnAutoVoice(),
                                        TRUE);
            }

            scan_proc->SetWaitForVoice(true);

            if (scan_proc->GetAvTp1Ctrl() && (scan_proc->GetAvTp1Margin() >= 0)) {
                //+ Patni-Dhanesh/2009Nov04/Added/Auto Voice Defect Fix - MVC004941
                voice_manager->CmdTrap(CVoiceManager::GetInstance()->GetAvTp1Delay(),
                                       scan_proc->GetAvTp1Duration(),
                                       PQM_AV_VOICE_5,
                                       curr_protocol->GetPlnAutoVoice());
                //- Patni-Dhanesh/2009Nov04/Added/Auto Voice Defect Fix - MVC004941

            }

            scan_proc->ScanContinue();
            return;

        } else if (curr_protocol->GetExeAutoVoice() & AUTO_VOICE_VALUE) {

            voice_manager->CmdDelay(pqmptr->GetAvInterval(),
                                    (curr_protocol->GetExeAutoVoice() & AUTO_VOICE_VALUE),
                                    TRUE);

            scan_proc->SetWaitForVoice(true);
            scan_proc->ScanContinue();
            return;
        }

    } else {
        /* AUTO_VOICE */

        //+Patni-Hemant/2010Jun4/Added/MaFT/TMSC-REDMINE-23/Visual Prep Case
        //+MEITEC/2010Jun07/Modified/MaFT/TMSC-REDMINE-23/Visual Prep Case/When the BreathHold PreScan AutoVoice be modified
        if (curr_protocol->GetExeAutoVoice() & 0xF0) {
            //if (curr_protocol->GetExeAutoVoice() & 0x0F) {
            //-MEITEC/2010Jun07/Modified/MaFT/TMSC-REDMINE-23/Visual Prep Case/When the BreathHold PreScan AutoVoice be modified

            if (scan_proc->GetAPCHold()) {
                //PqmAvAction(PQM_AV_CMD_PLAY_W,PQM_AV_VOICE_5);
                voice_manager->CmdPlay(PQM_AV_VOICE_5);
            }

            //-Patni-Hemant/2010Jun4/Added/MaFT/TMSC-REDMINE-23/Visual Prep Case

            voice_manager->CmdDelay(pqmptr->GetAvInterval(),
                                    (curr_protocol->GetExeAutoVoice() >> NUM_4),
                                    TRUE);

            scan_proc->SetWaitForVoice(true);
            scan_proc->ScanContinue();

            //Patni-Hemant/2010Jun4/Commented/MaFT/TMSC-REDMINE-23/Visual Prep Case
            //Its not in base code
            //scan_proc->SetAPCHold(0);

            return;
        }
    }

    //- Patni-Dhanesh/2009Nov04/Added/Auto Voice Defect Fix - Code Review

    //Patni-Hemant/2010Jun4/Commented/MaFT/TMSC-REDMINE-23/Visual Prep Case
    //Its not in base code
    //scan_proc->SetAPCHold(0);

    pqmptr->WriteToUI(PQM_MSG_SCAN_START_TRUE);
    pqmptr->WriteToUI(PQM_MSG_SCAN_PAUSE_FALSE);
    pqmptr->WriteToUI(PQM_MSG_SCAN_ABORT_TRUE);
}

//****************************Method Header************************************
//Method Name   : AcqChangeCoilList()
//Author        : PATNI\HAR
//Purpose       : Coiled coil connected to the list to reflect information. (Under the protocol)
//*****************************************************************************
void CScanTransaction::AcqChangeCoilList(
    VftCoilListInfo_t* f_coil_connect,
    int f_connect_flg[MAXIMUM_COIL_IN_PORT]
)
{
    LPCTSTR FUNC_NAME = _T("CScanTransaction::AcqChangeCoilList");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_scan_procedure == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_scan_procedure pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    CPqm*		l_pqm_ptr	 = m_scan_procedure->GetPqmPtr();
    CPQMStudy*	l_study_ptr = l_pqm_ptr->GetStudy();

    CPqmProtocol* l_protocol_ptr = NULL;
    //DB_FUNC_ENTER("[pqmAcqChangeCoilList]:");

    int i = 0, j = 0;

    for (l_protocol_ptr =
             l_study_ptr->GetNextItem(m_scan_procedure->GetScanProcProtocol());
         l_protocol_ptr != NULL;
         l_protocol_ptr = l_study_ptr->GetNextItem(l_protocol_ptr)) {

        //Connect to a list of information to replace the coil
        for (i = 0; i < f_coil_connect->num; i++) {

            for (j = 0; j < l_protocol_ptr->GetSaturnCoil()->CoilList.num;
                 j++) {

                if (l_protocol_ptr->GetSaturnCoil()->
                    CoilList.coillist[j].coilID
                    == f_coil_connect->coillist[i].coilID
                    && l_protocol_ptr->GetSaturnCoil()->
                    CoilList.coillist[j].portID[0]
                    == f_coil_connect->coillist[i].portID[0]) {

                    if (!f_connect_flg[i] ||
                        l_protocol_ptr->GetSaturnCoil()->
                        CoilList.coillist[j].portID[1]
                        == f_coil_connect->coillist[i].portID[1]) {
                        //Concect_flg standing on the port or not ID (number) is consistent with the
                        break;
                    }
                }
            }

            if (j == (l_protocol_ptr->GetSaturnCoil()->CoilList.num) &&
                f_coil_connect->coillist[i].coilID != WBCOIL_PHYSICAL_ID) {

                //Coil connected to the information
                l_protocol_ptr->GetSaturnCoil()->CoilList.num++;
                l_protocol_ptr->GetSaturnCoil()->CoilList.coillist[j] =
                    f_coil_connect->coillist[i];
                l_protocol_ptr->GetSaturnCoil()->
                CoilList.coillist[j].coordinate = 0;
                l_protocol_ptr->GetSaturnCoil()->
                CoilListcoordinate.coordinate_list[j].coordinate = 0;

                //+Patni-Hemant/2010Feb19/Added/ACE-Phase#3/CDS Requirement

                /* Channel Information */
                if (l_protocol_ptr->GetSaturnCoil()->CoilList.coillist[j].portID[0]  == CDB_PORT_D) {
                    if (l_protocol_ptr->GetSaturnCoil()->CoilList.coillist[j].portID[1] == '1') {
                        l_protocol_ptr->GetSaturnCoil()->CoilList.coillist[j].channel = CDB_CHANNEL_A;

                    } else {
                        l_protocol_ptr->GetSaturnCoil()->CoilList.coillist[j].channel = CDB_CHANNEL_B;
                    }

                } else {
                    CDataManager* l_data_mgr_ptr = l_pqm_ptr->GetDataManager();
                    CCoil l_saturn_coil_temp;
                    l_data_mgr_ptr->GetPQMCoilData()->PqmCoilidGetSaturnCoil(f_coil_connect->coillist[i].coilID,
                            f_coil_connect->coillist[i].portID[0], NULL, l_saturn_coil_temp);

                    l_protocol_ptr->GetSaturnCoil()->CoilList.coillist[j].channel = l_saturn_coil_temp.GetCoil()->channel;
                }

                //-Patni-Hemant/2010Feb19/Added/ACE-Phase#3/CDS Requirement
            }

            l_study_ptr->PutSaturnCoil(l_study_ptr->GetVfDBHandle(),
                                       l_protocol_ptr->GetProtocol(),
                                       l_protocol_ptr->GetSaturnCoil());

            l_study_ptr->PutConnectedCoil(l_study_ptr->GetVfDBHandle(),
                                          l_protocol_ptr->GetProtocol(),
                                          l_protocol_ptr);
        }
    }
}

//****************************Method Header************************************
//Method Name   :GetCFValue()
//Author        :PATNI/MSN
//Purpose       :Getter method to get the CF Value
//*****************************************************************************
double CScanTransaction::GetCFValue() const
{
    return m_fsyvalue;
}

//****************************Method Header************************************
//Method Name   :SetCFValue()
//Author        :PATNI/MSN
//Purpose       :Setter method to set the CF Value
//*****************************************************************************
void CScanTransaction::SetCFValue(const double fsyvalue)
{
    m_fsyvalue = fsyvalue;
}

//****************************Method Header************************************
//Method Name   :GetCheckBit()
//Author        :PATNI/MSN
//Purpose       :Returns the check bit
//*****************************************************************************
int CScanTransaction::GetCheckBit() const
{
    return m_check_bit;
}

//***************************Method Header*************************************
//Method Name    : SetCheckBit()
//Author         : PATNI/MRP
//Purpose        : This method sets the bit based on the parameter ans retuns
//*****************************************************************************
bool CScanTransaction::SetCheckBit(
    //+Patni-Hemant/2010Mar16/Added/ACE-Phase#3/CDS Requirement
    bool& f_process_prescango
    //-Patni-Hemant/2010Mar16/Added/ACE-Phase#3/CDS Requirement
)
{
    LPCTSTR FUNC_NAME = _T("CScanTransaction::SetCheckBit");

    //+Patni-Hemant/2010Mar16/Added/ACE-Phase#3/CDS Requirement
    f_process_prescango = true ;
    //-Patni-Hemant/2010Mar16/Added/ACE-Phase#3/CDS Requirement

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetCheckBit"));

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections Need to Check
    if (m_scan_procedure == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_scan_procedure pointer is NULL"));
        return false;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    CPqmPmAcqman* acqman = m_scan_procedure->GetAcqManager();
    CPqmProtocol* curr_protocol = m_scan_procedure->GetScanProcProtocol();
    CPqm* pqm = acqman->GetPqm();
    CPQMStudy* l_study = pqm->GetStudy(); 	//Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image
    CPqmPrMode* pqm_pr_mode = curr_protocol->GetPqmPrMode();
    CPqmPrMode* last_scan = acqman->GetProtocolModeLastScan();

    //Added by Meghana On 8/28/2008 12:51:00 PM
    PqmSaturnCoil_t* last_saturn_coil = NULL;
    PqmSaturnRcvCoil_t* last_coil_table = NULL;


    //+ Patni-MP/2009Aug13/Commented/MVC4208
    if (acqman != NULL) {
        last_saturn_coil = acqman->GetLastSaturnCoil();
        last_coil_table = acqman->GetLastSaturnCoilTbl();
    }

    //- Patni-MP/2009Aug13/Commented/MVC4208

    int l_rfl_limit = 0;
    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    //int l_tune_limit;
    int l_tune_limit = 0;
    m_acq_info_retry = m_scan_procedure->GetAcqInfoRetry();
    m_scan_procedure->SetAcqInfoRetry(0);

    // + V9.25 CoilSupport@AMT
    CPqmCoilData* l_coil_data = CDataManager::GetInstance()->GetPQMCoilData();

    if (pqm->IsSaturnCoilSystem()) {	//Saturn Coil

        if (l_coil_data->PqmGetRflTuneInSaturn(
                curr_protocol->GetCoilDBTable(),
                &l_rfl_limit, &l_tune_limit) != E_NO_ERROR) {

            l_rfl_limit = 50;
            l_tune_limit = 500;
        }

    } else {
        CCoil coil;
        l_coil_data->PqmCoilGetCoil(pqm_pr_mode->GetRecvCoilId(), coil);

        if (coil.GetCoil()) {

            l_rfl_limit = coil.GetRFLLimit();
            l_tune_limit = coil.GetTuneLimit();

        } else {

            l_tune_limit = 500;
            l_rfl_limit = 50;
        }
    }

    // - V9.25 CoilSupport@AMT

    ASSERT(curr_protocol != NULL);
    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    /*
    int sm_scan_number;
    float sm_couch_pos;
    float sm_home_pos;
    int   sm_shim_state;
    float sm_tx_tune_pos;
    float sm_rfl_pos;
    */
    int sm_scan_number = 0;
    float sm_couch_pos = 0.0f;
    float sm_home_pos = 0.0f;
    int   sm_shim_state = 0;
    float sm_tx_tune_pos = 0.0f;
    float sm_rfl_pos = 0.0f;
    //Patni-DKH/2010Feb18/Added/Phase3#MCM0229-00187
    float	    l_cfaPos = 0.0f;
    float	    l_shimPos = 0.0f ;
    //Patni-DKH/2010Feb18/Added/Phase3#MCM0229-00187
    //- Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    // + Shimming Lokesh 13 March 2008
    CPqmShimSharedData* l_pqmshimshareddat = CPqmShimSharedData::GetInstance();

    if (!(l_pqmshimshareddat->CopyCouchInfo(& sm_scan_number, &sm_couch_pos, &sm_home_pos)) ||
        //Patni-DKH/2010Feb18/Added/Phase3#MCM0229-00187
        !(l_pqmshimshareddat->CopyTuneInfo(&sm_tx_tune_pos, &sm_rfl_pos, &l_cfaPos)) ||
        //Patni-DKH/2010Feb18/Added/Phase3#MCM0229-00187
        !(l_pqmshimshareddat->CopyShimPos(&l_shimPos)) ||
        !(l_pqmshimshareddat->GetShimState(&sm_shim_state))
       ) {

        //+Patni-ARD/2009-Nov-26/Commented/DeFT# MVC003149 + MVC005179
        //+Patni-ARD/2009Jun25/Added/ACE# policy of ErrorView
        //CPQMLogMgr::GetInstance()->DisplayScanError(IDS_MISC_ERR, _T("IDS_MISC_ERR"),
        //        SCAN_ERR_NOTIFY, L"scantransaction.cpp");
        //-Patni-ARD/2009Jun25/Added/ACE# policy of ErrorView
        /*
        CPQMLogMgr::GetInstance()->WriteEventLog(IDS_MISC_ERR,
                ERROR_LOG_CRUCIAL1  , _T("ScanTransaction.cpp"));
        		*/
        //-Patni-ARD/2009-Nov-26/Commented/DeFT# MVC003149 + MVC005179
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Unable to copy Couch Information!!"));
        return false;

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("copid Couch Information!!"));
    }

    //+Patni-Raj/2010Jan14/Added/PH#3#/Debug Window
    //Patni-DKH/2010Feb18/Added/Phase3#MCM0229-00187
    CPQMDebugInfo::GetInstance(pqm)->PqmDebugAcqMonitor(_T("Couch Position : %f TunePosition : %f RflPosition : %f RflPosition : %f"),
            sm_couch_pos, sm_tx_tune_pos, sm_rfl_pos, l_cfaPos);
    //-Patni-Raj/2010Jan14/Added/PH#3#/Debug Window

    //    if (E_NO_ERROR != m_scan_procedure->CopyCouchInfo(sm_scan_number, sm_couch_pos, sm_home_pos) ||
    //        E_NO_ERROR != m_scan_procedure->CopyTuneInfo(sm_tx_tune_pos, sm_rfl_pos) ||
    //        E_NO_ERROR != m_scan_procedure->GetShimState(sm_shim_state)) {
    //
    //        CPQMLogMgr::GetInstance()->WriteEventLog(IDS_MISC_ERR,
    //            ERROR_LOG_CRUCIAL1  ,_T("ScanTransaction.cpp"));
    //
    //        return false;
    //    }

    // - Shimming Lokesh 13 March 2008

    m_check_bit = 0;
    m_check_str.Empty();

    const int l_as_compass_flag = curr_protocol->GetASCOMPASSFlag();
    CpqmCDS* l_pqm_cds = pqm->GetCDS();
    int l_cds_status = PQM_CDS_NONE;

    if (curr_protocol->GetCDSInfo()->m_cds_status & VFC_CDS_STATUS_USE_PREVIOUS) {
        l_cds_status = PQM_CDS_PREV;
    }

    bool l_CDS_Conditions_satisfied = ((l_as_compass_flag) && (l_cds_status != PQM_CDS_PREV) && (curr_protocol->GetCDSInfo()->m_cds_step == CDS_STEP6_DONE) && ((SHIM_STATE_ACTIVE != sm_shim_state) || curr_protocol->GetShimming()));
    bool l_coil_consider = true;

    if (l_as_compass_flag) {
        l_coil_consider = l_CDS_Conditions_satisfied;
    }

    if (sm_scan_number != m_couch_status) {
        //Patni-DKH/2010Feb18/Added/Phase3#MCM0229-00187
        float l_resetpos = PQM_COUCH_INIT_POSITION;
        l_pqmshimshareddat->ShimRegistTuneInfo(NULL, NULL, &l_resetpos);
        //Patni-DKH/2010Feb18/Added/Phase3#MCM0229-00187
        m_check_bit |= PQM_CHK_BIT_FIRST;
        m_check_str += FIRST;
        last_scan->SetAnatomy(0);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetAnatomy = 0"));

        //Patni-DKH/2010Feb18/Added/Phase3#MCM0229-00187
        //+Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image
        if (curr_protocol->GetAcqStatus() == C_PR_STATUS_WAIT) {
            acqman->pqmAcqResetCopySourceProt(l_study->GetVfDBHandle(), curr_protocol);
        }

        //-Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image


    }

    //+Patni-Meghna-DKH/2010Apr12/Commented/Phase3#MCM0229-00187
    /*else if (pqm_pr_mode->GetAnatomy() != last_acq->GetAnatomy()) {

        m_check_bit |= PQM_CHK_BIT_ANATOMY;
        m_check_str += ANATOMY;

    }*/ else if (pqm->IsSaturnCoilSystem() &&
                 (l_coil_data->PqmIsSameCoilInSaturn(curr_protocol->GetSaturnCoil(),
                         last_saturn_coil,
                         curr_protocol->GetCoilDBTable(),
                         last_coil_table) == FALSE) && (l_coil_consider)) {

        //pqm->GetCoilDB()->IsSameCoilInSaturn() == FALSE) {

        /* Saturn Coil */
        m_check_bit |= PQM_CHK_BIT_COIL;
        m_check_str += COIL;

        //+Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image
        if (curr_protocol->GetAcqStatus() == C_PR_STATUS_WAIT) {
            acqman->pqmAcqResetCopySourceProt(l_study->GetVfDBHandle(), curr_protocol);
        }

        //-Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image

    } else if (!pqm->IsSaturnCoilSystem() &&
               !IS_COIL_SAME(pqm_pr_mode->GetRecvCoilId(),
                             last_scan->GetRecvCoilId())) {

        m_check_bit |= PQM_CHK_BIT_COIL;
        m_check_str += COIL;

    } else if (l_CDS_Conditions_satisfied) {//3239
        if (m_lastfunction != AMCMR_UNKNOWN) {
            m_check_bit |= PQM_CHK_BIT_RFL_TABLE;
            m_check_str += RFL_TABLE;
            m_check_bit |= PQM_CHK_BIT_CFA_TABLE;
            m_check_str += CFA_TABLE;
        }

    } else if (curr_protocol->GetPqmPrMode()->GetAnatomy() != last_scan->GetAnatomy()) {
        m_check_bit |= PQM_CHK_BIT_ANATOMY;
        m_check_str += ANATOMY ;

        if (m_couch_position != l_cfaPos) {
            //+Patni-Raj/2010Jan14/Added/PH#3#/Debug Window
            CPQMDebugInfo::GetInstance(pqm)->PqmDebugAcqMonitor(_T("COIL *******  TuneLimit : %d RflLimit : %d"),
                    l_tune_limit, l_rfl_limit);

            //-Patni-Raj/2010Jan14/Added/PH#3#/Debug Window
            if (ABSF(m_couch_position - l_cfaPos) > (float)l_rfl_limit) {
                m_check_bit |= PQM_CHK_BIT_CFA_TABLE;
                m_check_str += CFA_TABLE;
            }
        }

    } else if (m_couch_position != sm_rfl_pos || m_couch_position != l_cfaPos) {
        //+Patni-Raj/2010Jan14/Added/PH#3#/Debug Window
        CPQMDebugInfo::GetInstance(pqm)->PqmDebugAcqMonitor(_T("COIL *******  TuneLimit : %d RflLimit : %d"),
                l_tune_limit, l_rfl_limit);

        //-Patni-Raj/2010Jan14/Added/PH#3#/Debug Window
        if ((ABSF(m_couch_position - l_cfaPos) > (float)l_rfl_limit) || (l_CDS_Conditions_satisfied && (m_lastfunction != AMCMR_UNKNOWN))) {
            /* CFA */
            m_check_bit |= PQM_CHK_BIT_CFA_TABLE;
            m_check_str += CFA_TABLE;
        }

        if ((ABSF(m_couch_position - sm_rfl_pos) > (float)l_rfl_limit) || (l_CDS_Conditions_satisfied && (m_lastfunction != AMCMR_UNKNOWN))) {
            /* RFL */
            m_check_bit |= PQM_CHK_BIT_RFL_TABLE;
            m_check_str += RFL_TABLE;
        }

    }

    if (!(m_check_bit & (PQM_CHK_BIT_FIRST | PQM_CHK_BIT_COIL))) {
        if (!(m_check_bit & PQM_CHK_BIT_CFA_TABLE)) {
            BITFLD_DECL(mask, K_ALL_PR);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CFAPOS VAlue set"));
            curr_protocol->SetCfaPos(l_cfaPos);

            if (E_NO_ERROR != pqm->GetStudy()->SetProtocolValues(pqm->GetStudy()->GetVfDBHandle(),
                    curr_protocol->GetProtocol(),
                    mask,
                    K_PR_CFA_COUCH,
                    &l_cfaPos,
                    sizeof(flt32_t),
                    NULL)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetProtocolValues failed."));
            }
        }

        if (!(m_check_bit & (PQM_CHK_BIT_RFL_TABLE | PQM_CHK_BIT_ANATOMY))) {
            BITFLD_DECL(mask, K_ALL_PR);
            curr_protocol->SetRflPos(sm_rfl_pos);

            if (E_NO_ERROR != pqm->GetStudy()->SetProtocolValues(pqm->GetStudy()->GetVfDBHandle(),
                    curr_protocol->GetProtocol(),
                    mask,
                    K_PR_RFL_COUCH,
                    &sm_rfl_pos,
                    sizeof(flt32_t),
                    NULL)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetProtocolValues failed."));
            }
        }
    }

    //+Patni-HAR/2010Feb10/Added/CDS Requirement
    //When set to automatic control coil cds_step
    if (curr_protocol->GetASCOMPASSFlag()) {
        if (curr_protocol->GetCDSInfo()->m_cds_step == CDS_STEP1_RFINTERLOCK_LEVEL) {
            f_process_prescango = false ;

            //+Patni-Hemant/2010Aug05/Modified/REDMINE-573
            //m_scan_procedure->SetCouchInfo(curr_protocol->GetPosition(), m_couch_position, m_couch_status);
            m_scan_procedure->SetCouchInfo(curr_protocol->GetProtocol(), m_couch_position, m_couch_status);
            //-Patni-Hemant/2010Aug05/Modified/REDMINE-573

            m_scan_procedure->SetBreathHold(curr_protocol->GetHoldScan() & VFC_HOLD_COVERAGE);
            //goto PRESCAN_RFINTERLOCK_LEVEL;     /* coilは後で変わるのでinterlock値は送らない */
            PreScanGo(AMCMA_RFINTERLOCK_DONE);    //coil is changed later, so do not send values interlock

        } else if (curr_protocol->GetCDSInfo()->m_cds_step == CDS_STEP2_MOVECOUCH_LEVEL) {

            f_process_prescango = false ;
            //goto PRESCAN_MOVECOUCH_LEVEL;       /* 寝台移動後はMOVECOUCH_LEVELへ */
            PreScanGo(AMCMA_REMOTE_MOVECOUCH_DONE);  //After moving to the sleeper MOVECOUCH_LEVEL

        } else if (curr_protocol->GetCDSInfo()->m_cds_step == CDS_STEP3_SHIMCTRL_LEVEL) {

            f_process_prescango = false ;

            //goto PRESCAN_SHIMCTRL_LEVEL;        /* After the normal SHIMCTRL */
            PreScanGo(AMCMA_SHIMCTRL_DONE);

        } else if (curr_protocol->GetCDSInfo()->m_cds_step == CDS_STEP4_SCAN) {

            f_process_prescango = false ;

            /* CDS chk_bit to make a*/
            m_check_bit |= PQM_CHK_BIT_CDS;
            m_check_str += CDS;
            PreScanGo(AMCMR_UNKNOWN);    //CDS of the CFA immediately sent one to scan for the value Interlock
        }
    }

    //-Patni-HAR/2010Feb10/Added/CDS Requirement
    CString l_tracemsg(_T(""));
    l_tracemsg.Format(_T("Check Bit Value is --> %d"), m_check_bit);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_tracemsg);

    return true;
}

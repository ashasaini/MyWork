// MasterSlave.cpp: implementation of the CMasterSlave class.
//
//////////////////////////////////////////////////////////////////////
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: MasterSlave.cpp
 *  Overview: Implementation of CMasterSlave class.
 *  Last Modified: 2011/01/11
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni				2011/01/11
 *
 *****************************************************************************/

#include "stdafx.h"
#include "MasterSlave.h"
#include <math.h>
#include <libStudyManager/Log/ScopedFuncInOutLog.h>
#include <PQM\PqmUtilities.h>
#include <PQM\resource.h>
#include "pqm.h"
#include "pqmstudy.h"
#include "PQMStudyMgr.h"
#include "PqmProtocol.h"
#include "VarFieldHandle.h"
#include "GPLibAdapter.h"
#include "LibPLTreeWrapper.h"
#include "PqmCoilData.h"
#include "PqmPmAcqman.h"
#include "PqmPmSARManager.h"
#include "ScanProcedure.h"


using namespace MPlus::EH;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
struct SDynamicParamCtrls {
    VfName_t    m_name;
    VfType_t    m_type;
    void*       m_ctrl_value;
    int         m_ctrl_size;
    status_t	m_status;
};


//**************************************************************************
//Method Name   : CMasterSlave
//Author        : PATNI/AKR
//Purpose       : Constructor, object pointer is on CPqm class.
//**************************************************************************
CMasterSlave::CMasterSlave(
    CPqm* f_pqm_ptr
) : m_pqm_ptr(f_pqm_ptr),
    m_pqm_study(f_pqm_ptr->GetStudy()),
    m_group_num(0),
    m_protocol_index_to_update(PQM_INVALID_PROTOCOL)
{
}
//**************************************************************************
//Method Name   : ~CMasterSlave
//Author        : PATNI/AKR
//Purpose       : Destructor
//**************************************************************************
CMasterSlave::~CMasterSlave()
{

}
//*******************************Method Header*********************************
//Method Name    :PqmPmSaveForMasterSlaveProtocol()
//Author         :PATNI/AKR
//Purpose        :Wrapper function of PqmPmSave for Auto copy and plan dynamic functionality.
//*****************************************************************************
status_t CMasterSlave::PqmPmSaveForMasterSlaveProtocol(
    CPqmProtocol* const f_master_protocol,
    AutoCopyParameter_type f_parameter,
    const overwrite_info_mode f_overwrite_info_mode
)
{

    LPCTSTR FUNC_NAME = _T("CMasterSlave::PqmPmSaveForMasterSlaveProtocol");
    PQM_SCOPED_TRACE();

    if (NULL == f_master_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Invalid parameteres passed."));
        return E_ERROR;
    }

    if (!m_pqm_ptr->GetMulitScanDynamic_License()) {
        return E_ERROR;
    }

    SDynamicParamCtrls l_dynamic_ctrls[] =  {
        {SVN_PLN_TIME_CTRL_PARAM, SVT_PLN_TIME_CTRL_PARAM, NULL, 0, 1},
        {SVN_PLN_ACQ_SPLIT_MODE, SVT_PLN_ACQ_SPLIT_MODE, NULL, 0, 1},
        {SVN_PLN_TOTAL_TIME, SVT_PLN_TOTAL_TIME, NULL, 0, 1},
        {SVN_PLN_REPETITIONS, SVT_PLN_REPETITIONS, NULL, 0, 1},
        {SVN_PLN_ACQ_SPLIT_TABLE, SVT_PLN_ACQ_SPLIT_TABLE, NULL, 0, 1},
        {SVN_PLN_LDB_MAPPED_ID, SVT_PLN_LDB_MAPPED_ID, NULL, 0, 1},
        {SVN_PLN_INJECT_TIME, SVT_PLN_INJECT_TIME, NULL, 0, 1},
        {SVN_EXE_PROTOCOL_ACQ_STATUS, SVT_EXE_PROTOCOL_ACQ_STATUS, NULL, 0, 1},
        {SVN_PLN_MASTER_FLAG, SVT_PLN_MASTER_FLAG, NULL, 0, 1},
        {SVN_PLN_HOLD, SVT_PLN_HOLD, NULL, 0, 1},
        {SVN_PLN_COMMENTS, SVT_PLN_COMMENTS, NULL, 0, 1},
        {SVN_PLN_SCAN_COMMENTS, SVT_PLN_SCAN_COMMENTS, NULL, 0, 1},
        {SVN_PLN_AUTO_POSTPROC_FLG, SVT_PLN_AUTO_POSTPROC_FLG, NULL, 0, 1},
        {SVN_PLN_PREP_SCAN, SVT_PLN_PREP_SCAN, NULL, 0, 1},
        {SVN_PLN_MULTI_SERIES_DYNAMIC_ENABLE_FLAG, SVT_PLN_MULTI_SERIES_DYNAMIC_ENABLE_FLAG, NULL, 0, 1},
        {SVN_PLN_SEGMENTATION_TYPE, SVT_PLN_SEGMENTATION_TYPE, NULL, 0, 1},
        {SVN_SCH_N_CONTRAST, SVT_SCH_N_CONTRAST, NULL, 0, 1},

    };


    const int l_dynamic_ctrls_length = sizeof(l_dynamic_ctrls) / sizeof(*l_dynamic_ctrls);

    status_t l_status = E_ERROR;

    switch (f_overwrite_info_mode) {

        case PLAN_DYNAMIC:

            l_status = PqmPmSaveForPlanDynamic(f_master_protocol, f_parameter.m_scratchpad, l_dynamic_ctrls, l_dynamic_ctrls_length);

            if (E_NO_ERROR != l_status) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PqmPmSaveForPlanDynamic failed."));
            }

            return l_status;

        case AUTO_COPY_PROCON:
        case AUTO_COPY_PQM:
        case AUTO_COPY_MEMORY:
            l_status = PqmPmSaveForAutoCopy(f_master_protocol, f_parameter, l_dynamic_ctrls, l_dynamic_ctrls_length, f_overwrite_info_mode);

            if (E_NO_ERROR != l_status) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PqmPmSaveForAutoCopy failed."));
            }

            return l_status;

        default:
            return E_ERROR;
    }

    return l_status;
}


//**************************************************************************
//Method Name   : AllocateGroupIdForDuplicateProtocols
//Author        : PATNI/AKR
//Purpose       : This is called from GUI, It will take input as seleted index
//				  which are going to be duplicated, and returns the group id's
//				  of the duplicated protocol. Caller of this function will have
//				  the responsibility to delete allocated memory.
//**************************************************************************
void CMasterSlave::AllocateGroupIdForDuplicateProtocols(
    VARIANT* f_pdata
)
{
    LPCTSTR FUNC_NAME = _T("CMasterSlave::AllocateGroupIdForDuplicateProtocols");
    PQM_SCOPED_TRACE();

    //l_selected_items is a 4 row array
    //1st row is having the selected index coming from GUI side
    //2nd row saves the information about master(2)/slave(1)/normal(0) protocol
    //3rd row save the group id of that protocol(older)
    //4th row will save the new allocated(to be) group id

    if (NULL == f_pdata || NULL == f_pdata->byref) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_pdata is NULL"));
        return;
    }

    int l_selected_count = f_pdata->wReserved1;

    if (0 == l_selected_count) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_selected_count is 0"));
        f_pdata->byref = NULL;
        return;
    }


    int* l_selected_items = NULL;

    try {
        l_selected_items = new int[l_selected_count * NUM_4];

    } catch (CMemoryException* e) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_selected_items new is failed."));
        f_pdata->byref = NULL;
        e->Delete();
        return;
    }

    //Initialize with zero
    memset(l_selected_items, 0, sizeof(int)*l_selected_count * NUM_4);


    //Saving first row as selected index.
    for (int l_index = 0; l_index < l_selected_count; l_index++) {
        l_selected_items[l_index] = ((int*)f_pdata->byref)[l_index] ;
    }

    //Saving second row of the array, i.e. recongnizing master/slave protocol &&
    //Saving third row of the array i.e. Selected protocol's group id
    IdentifiyMasterSlaveGroupID(l_selected_count, l_selected_items);

    //Saving 4th row of the array i.e.the new allocated(to be) group id
    //Reset group num only when new group is assigned to master
    if (true == DecideGroupIdForDuplicated(l_selected_count, l_selected_items)) {

        SetGroupNumToVFC();    //Set group num back to VFC
    }

    //caller of this function will have the responsibility to delete this allocation when done.
    int* l_new_group_id = NULL;

    try {
        l_new_group_id = new int[l_selected_count];

    } catch (CMemoryException* e) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_new_group_id is NULL"));
        DEL_PTR_ARY(l_selected_items);
        f_pdata->byref = NULL;
        e->Delete();
        return;
    }

    for (l_index = 0; l_index < l_selected_count; l_index++) {
        l_new_group_id[l_index] = l_selected_items[l_selected_count * FOURTH_ROW_INDEX + l_index];
    }

    DEL_PTR_ARY(l_selected_items);

    //Send back the data to GUI
    f_pdata->byref = (void*)l_new_group_id;
}
//**************************************************************************
//Method Name   : UpdateGroupNum
//Author        : PATNI/AKR
//Purpose       : This will get the group num from vector by getting maximum
//				  group id present in the PQM GUI, and reset it to vfc
//**************************************************************************
void CMasterSlave::UpdateGroupNum()
{
    LPCTSTR FUNC_NAME = _T("CMasterSlave::UpdateGroupNum");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));


    m_group_num = GetGroupNumFromVec();
    SetGroupNumToVFC();
}
//**************************************************************************
//Method Name   : IsMasterSlaveProtocol
//Author        : PATNI/AKR
//Purpose       : This function identifies the protocol as master/slave/normal/invalid
//				  Public funtion.
//**************************************************************************
int CMasterSlave::IsMasterSlaveProtocol(
    const CPqmProtocol* const f_protocol
)	const
{
    LPCTSTR FUNC_NAME = _T("CMasterSlave::IsMasterSlaveProtocol");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL == f_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Invalid protocol"));
        return PQM_INVALID_PROTOCOL;
    }

    if (!m_pqm_ptr->GetMulitScanDynamic_License()) {
        return PQM_NORMAL_PROTOCOL;
    }

    if (!f_protocol->GetGroupID()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Not a master or slave protocol"));
        return PQM_NORMAL_PROTOCOL;
    }

    int l_is_master_slave = f_protocol->GetMasterFlag() ? PQM_MASTER_PROTOCOL : PQM_SLAVE_PROTOCOL;

    CString l_trace_msg(_T(""));

    l_trace_msg.Format(_T("This is a %s protocol."),
                       (l_is_master_slave == PQM_MASTER_PROTOCOL) ? _T("Master") : _T("Slave"));
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_msg);

    return l_is_master_slave;
}
//**************************************************************************
//Method Name   : OnDeleteUpdateMasterSlave
//Author        : PATNI/AKR
//Purpose       : This function is called before deleting protocols from gui,
//				  this will reset the slaves group id to zero.
//**************************************************************************
void CMasterSlave::OnDeleteUpdateMasterSlave(
    VARIANT* f_pdata
)
{
    LPCTSTR FUNC_NAME = _T("CMasterSlave::OnDeleteUpdateMasterSlave");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL == f_pdata) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_pdata is NULL"));
        return;
    }

    int* l_selected_items = (int*)f_pdata->byref; //Indexs of selected protocols to delete
    int l_selected_count = f_pdata->wReserved1; //count of selected index to delete

    BOOL l_can_update_group_num = FALSE;

    CPqmProtocol* l_current_protocol = NULL;

    for (int l_index = 0; l_index < l_selected_count; l_index++) {
        l_current_protocol = m_pqm_study->GetProtocolForIndex(l_selected_items[l_index]);

        if (NULL == l_current_protocol) {
            break; //do not return, need to set flag to send back to gui.

        }

        if (PQM_MASTER_PROTOCOL == IsMasterSlaveProtocol(l_current_protocol)) {
            if (m_group_num == l_current_protocol->GetGroupID()) {
                l_can_update_group_num = TRUE;
            }

            //this function is called for every master
            DeleteSlaveGroupId(l_current_protocol->GetGroupID());
        }
    }

    //If updation needed, after deleting protocols, GUI will call update group num
    f_pdata->boolVal = l_can_update_group_num;
}
//*******************************Method Header*********************************
//Method Name    :CheckForSlaveScan
//Author         :PATNI/SS
//Purpose        :
//*****************************************************************************
int CMasterSlave::CheckForSlaveScan(
    CPqmProtocol* const f_current_prot
)
{
    LPCTSTR FUNC_NAME = _T("CMasterSlave::CheckForSlaveScan");
    PQM_TRACE(USER_FUNC_MARKER , FUNC_NAME, _T("Trace"));

    if (!f_current_prot) {
        PQM_TRACE(USER_FUNC_MARKER , FUNC_NAME, _T("f_current_prot is NULL"));
        return E_ERROR;
    }

    if (PQM_SLAVE_PROTOCOL != IsMasterSlaveProtocol(f_current_prot)) {
        return E_NO_ERROR;
    }

    CPqmProtocol* l_master_prtocol = f_current_prot->GetMasterProtocol();

    if (NULL == l_master_prtocol) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Abnormal Case :l_master_prtocol is NULL"));
        ChangeToNormalProtocol(f_current_prot);
        return E_NO_ERROR;
    }

    CString l_warning_msg = _T("");

    switch (l_master_prtocol->GetAcqStatus()) {

        case C_PR_STATUS_DONE:
            return E_NO_ERROR;

        case C_PR_STATUS_FAIL: {
            //Get Series bias number
            unsigned short l_series_bias = 0;
            m_pqm_ptr->GetStudyManagerPtr()->GetSeriesBias(&l_series_bias);

            CString l_series_num_str(_T(""));
            l_series_num_str.Format(_T("%d"), (l_series_bias * l_master_prtocol->GetPosition()));

            l_warning_msg.Format(CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_BASE_FAILED_FOR_SLAVE")) , l_series_num_str);
        }
        break;

        case C_PR_STATUS_WAIT: {
            l_warning_msg = CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_MASTER_NOT_DONE_FOR_SLAVE"));
        }
        break;

        default: {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Abnormal Case : l_master_prtocol is Current"));
            AbortScan(f_current_prot);
            return E_ERROR;
        }
    }

    if (!DisplayWarningMessage(l_warning_msg)) {
        AbortScan(f_current_prot);
        return E_ERROR;

    } else {

        switch (l_master_prtocol->GetAcqStatus()) {

            case C_PR_STATUS_FAIL: {
                return E_NO_ERROR;
            }
            break;

            case C_PR_STATUS_WAIT: {
                ChangeToNormalProtocol(f_current_prot);
                return E_NO_ERROR;
            }
            break;
        }
    }

    return E_NO_ERROR;
}
//*******************************Method Header*********************************
//Method Name    :CheckMasterSlaveProtocol
//Author         :PATNI/SS
//Purpose        :
//*****************************************************************************
int CMasterSlave::CheckMasterSlaveProtocol(
    CPqmProtocol* const f_current_prot,
    const float f_couch_pos,
    const int f_scan_number
)
{
    LPCTSTR FUNC_NAME = _T("CMasterSlave::CheckMasterSlaveProtocol");
    PQM_TRACE(USER_FUNC_MARKER , FUNC_NAME, _T("Trace"));

    if (!f_current_prot) return E_ERROR;

    bool l_pixle_check = true;

    if (PQM_SLAVE_PROTOCOL == IsMasterSlaveProtocol(f_current_prot)) {

        CPqmProtocol*   l_slave_prot = f_current_prot;
        CPqmPrMode* l_slave_pmpr_mode = l_slave_prot->GetPqmPrMode();

        float l_couch_pos = l_slave_pmpr_mode->GetCouchPos();
        l_slave_pmpr_mode->SetCouchPos(f_couch_pos);

        const int l_scan_number = l_slave_pmpr_mode->GetScanNumber();
        l_slave_pmpr_mode->SetScanNumber(f_scan_number);

        CPqmProtocol*   l_master_prot = f_current_prot->GetMasterProtocol();

        if (NOT_SAME == IsMasterSlaveInfoSame(l_slave_prot,  l_master_prot)) {
            //show warning message with ok and cancle button
            CString l_warn_msg = CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_MASTER_SLAVE_INFO_MISMATCH"));
            MPLUSERRORUSERRESPONSE_e l_response = (MPLUSERRORUSERRESPONSE_e) CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(l_warn_msg,
                                                  _T("PQM"),
                                                  SCAN_WARN_INFORMATION,
                                                  SCAN_WARN_SYSMODAL,
                                                  SCAN_WARN_OK_CANCEL_BUTTONS);

            if (l_response == MPlusErrResponse_CANCEL) {
                l_slave_pmpr_mode->SetCouchPos(l_couch_pos);
                l_slave_pmpr_mode->SetScanNumber(l_scan_number);

                AbortScan(f_current_prot);
                return E_ERROR;

            } else if (l_response == MPlusErrResponse_OK) {
                ChangeToNormalProtocol(f_current_prot);
            }
        }

        l_slave_pmpr_mode->SetCouchPos(l_couch_pos);
        l_slave_pmpr_mode->SetScanNumber(l_scan_number);

        if (IsMasterSlaveProtocol(f_current_prot)  == PQM_SLAVE_PROTOCOL
            && NOT_SAME == IsMasterSlaveVectorSame(l_slave_prot, l_master_prot)) {
            //show warning message with ok and cancle button
            CString l_warn_msg = CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_MASTER_SLAVE_VGN_VEC_MISMATCH"));
            MPLUSERRORUSERRESPONSE_e l_response = (MPLUSERRORUSERRESPONSE_e) CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(l_warn_msg,
                                                  _T("PQM"),
                                                  SCAN_WARN_INFORMATION,
                                                  SCAN_WARN_SYSMODAL,
                                                  SCAN_WARN_OK_CANCEL_BUTTONS);

            if (l_response == MPlusErrResponse_CANCEL) {
                AbortScan(f_current_prot);
                return E_ERROR;
            }

            l_pixle_check = false;
        }


    }

    if (l_pixle_check && PQM_NORMAL_PROTOCOL != IsMasterSlaveProtocol(f_current_prot)
        && (!f_current_prot->GetDynamic()
            || !m_pqm_ptr->GetStudy()->CheckPixelAndRGNCorrFlag(f_current_prot->GetProtocol()))
       ) {

        CString l_warn_msg = CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_DYNAMIC_ANALYSIS_NOT_SET"));
        MPLUSERRORUSERRESPONSE_e    l_response = MPlusErrResponse_Success ;
        l_response = (MPLUSERRORUSERRESPONSE_e) CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(l_warn_msg,
                     _T("PQM"),
                     SCAN_WARN_INFORMATION,
                     SCAN_WARN_SYSMODAL,
                     SCAN_WARN_OK_CANCEL_BUTTONS);

        if (l_response == MPlusErrResponse_CANCEL) {
            AbortScan(f_current_prot);
            return E_ERROR;
        }
    }

    return E_NO_ERROR;
}
//*******************************Method Header*********************************
//Method Name    :ChangeToNormalProtocol
//Author         :PATNI/SS
//Purpose        :
//*****************************************************************************
void  CMasterSlave::ChangeToNormalProtocol(
    CPqmProtocol* const f_current_prot,
    const bool f_update_gui /*=true*/
)
{
    LPCTSTR FUNC_NAME = _T("CMasterSlave::ChangeToNormalProtocol");
    PQM_TRACE(USER_FUNC_MARKER , FUNC_NAME, _T("Trace"));

    if (NULL == f_current_prot) {
        return;
    }

    f_current_prot->SetMasterProtocol(NULL);
    m_pqm_study->SetGroupIDToVF(f_current_prot, 0);
    m_pqm_study->SetBreathHoldValue(f_current_prot, m_pqm_study->GetVfDBHandle());

    if (f_update_gui) {
        m_protocol_index_to_update = f_current_prot->GetPosition() - 1;
        m_pqm_ptr->WriteToUI(PQM_MSG_MASTER_SLAVE_UPDATE_GUI);
        m_protocol_index_to_update = -1;
    }
}
//*******************************Method Header*********************************
//Method Name    :PerformAutoCopyForMasterSlave
//Author         :PATNI/AKR
//Purpose        :
//*****************************************************************************
void CMasterSlave::PerformAutoCopyForMasterSlave(VARIANT* const f_pdata)
{
    LPCTSTR FUNC_NAME = _T("CMasterSlave::PerformAutoCopyForMasterSlave");
    PQM_TRACE(USER_FUNC_MARKER , FUNC_NAME, _T("Trace"));

    if (NULL == f_pdata) {
        PQM_TRACE(USER_FUNC_MARKER , FUNC_NAME, _T("f_pdata is NULL"));
        return;
    }

    const int l_master_index = f_pdata->wReserved1;

    int l_slave_protocol_index = f_pdata->wReserved2;

    CPqmProtocol* l_master_protocol = m_pqm_study->GetProtocolForIndex(l_master_index);

    AutoCopyParameter_type l_parameter;

    l_parameter.m_protocol_index = &l_slave_protocol_index;



    if (E_NO_ERROR != PqmPmSaveForMasterSlaveProtocol(l_master_protocol, l_parameter, AUTO_COPY_PQM)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PqmPmSaveForMasterSlaveProtocol failed for AUTO_COPY_PQM"));
    }

    if (NULL != m_pqm_ptr->GetSARManager()) {
        m_pqm_ptr->GetSARManager()->CalcSARControlAndPutVFForAll();
    }
}
//*******************************Method Header*********************************
//Method Name    :UpdateSlaveForMaster
//Author         :PATNI/AKR
//Purpose        :
//*****************************************************************************
void CMasterSlave::UpdateSlaveForMaster(CPqmProtocol* const f_slave_protocol)
{
    LPCTSTR FUNC_NAME = _T("CMasterSlave::UpdateSlaveForMaster");
    PQM_TRACE(USER_FUNC_MARKER , FUNC_NAME, _T("Trace"));


    if (NULL == f_slave_protocol) {
        PQM_TRACE(USER_FUNC_MARKER , FUNC_NAME, _T("f_slave_protocol is NULL"));
        return;
    }

    CPqmProtocol* l_master_protocol = f_slave_protocol->GetMasterProtocol();

    if (l_master_protocol) {
        int l_slave_protocol_index = f_slave_protocol->GetPosition() - 1;
        AutoCopyParameter_type l_parameter;
        l_parameter.m_protocol_index = &l_slave_protocol_index;

        if (E_NO_ERROR != PqmPmSaveForMasterSlaveProtocol(l_master_protocol, l_parameter, AUTO_COPY_MEMORY)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PqmPmSaveForMasterSlaveProtocol failed for AUTO_COPY_MEMORY"));
        }

    }
}
//*******************************Method Header*********************************
//Method Name    :GetProtocolIndexToUpdate
//Author         :PATNI/AKR
//Purpose        :
//*****************************************************************************
int	CMasterSlave::GetProtocolIndexToUpdate()const
{
    return m_protocol_index_to_update;
}
//**************************************************************************
//Method Name   : GetSlavesProtocolIndex
//Author        : PATNI/AKR
//Purpose       : This function will take master group id and returns all the slaves of
//				  that group as out parameter, and slaves count as well. caller of this function
//				  will have the responsibility to deallocate memory for slaves index with delete [].
//**************************************************************************
int CMasterSlave::GetSlavesProtocolIndex(
    const int f_master_group_id,
    int** f_slaves_index
) const
{

    LPCTSTR FUNC_NAME = _T("CMasterSlave::GetSlavesProtocolIndex");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    int l_slaves_count = 0;
    const int l_total_protocols = m_pqm_study->GetProtCountFromVec();

    try {
        *f_slaves_index = new int[l_total_protocols]; //Caller of this function will have responsibility to delete

    } catch (CMemoryException* e) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Memory allocation fails"));
        e->Delete();
        return l_slaves_count;
    }

    //initialize this with -1 not with 0,as 0 is a valid index
    memset(*f_slaves_index, -1, sizeof(int)*l_total_protocols);

    CPqmProtocol* l_current_protocol = NULL;

    for (int l_index = 0; l_index < l_total_protocols; l_index++) {

        l_current_protocol = m_pqm_study->GetProtocolForIndex(l_index);

        if (NULL == l_current_protocol) {

            return l_slaves_count;

        }

        if (f_master_group_id == l_current_protocol->GetGroupID() &&
            PQM_SLAVE_PROTOCOL == IsMasterSlaveProtocol(l_current_protocol)) {

            (*f_slaves_index)[l_slaves_count] = l_index;
            l_slaves_count++;
        }
    }

    return l_slaves_count;	//returns slave count
}
//*******************************Method Header*********************************
//Method Name    :GetMasterListForPlanMark
//Author         :PATNI/AKR
//Purpose        :
//*****************************************************************************
void CMasterSlave::GetMasterListForPlanMark(VARIANT* f_pdata)
{
    LPCTSTR FUNC_NAME = _T("CMasterSlave::GetMasterListForPlanMark");
    PQM_SCOPED_TRACE();

    if (NULL == f_pdata) {
        return;
    }

    int l_selected_protocol_index = f_pdata->wReserved1;
    MasterListItemData* l_master_list_for_plan_mark = (MasterListItemData*)f_pdata->byref;

    if (NULL == l_master_list_for_plan_mark) {
        return;
    }

    int l_editing_protocol_postion = 	-1;
    CPqmProcon* l_pqm_procon = m_pqm_ptr->GetPqmProCon();

    if (l_pqm_procon) {
        CPqmProtocol* l_edting_protocol = l_pqm_procon->GetCurrentProtocol();

        if (l_edting_protocol) {
            l_editing_protocol_postion = l_edting_protocol->GetPosition();
        }
    }

    CPqmProtocol* l_master_protocol = m_pqm_study->GetProtocolForIndex(l_selected_protocol_index)->GetMasterProtocol();

    if (l_master_protocol) {
        f_pdata->wReserved1 = l_master_protocol->GetPosition();

    } else {
        f_pdata->wReserved1 = -1;
    }

    unsigned short l_series_bias = 0;
    m_pqm_ptr->GetStudyManagerPtr()->GetSeriesBias(&l_series_bias);

    CString l_master_series_num_str(_T(""));
    CString l_master_of_selected_index(_T(""));
    CPqmProtocol* l_current_protocol = NULL;

    for (int l_index = 0; l_index < l_selected_protocol_index; l_index++) {
        l_current_protocol = m_pqm_study->GetProtocolForIndex(l_index);

        if (NULL == l_current_protocol) {
            break;
        }

        if (l_current_protocol->GetMultiSeriesDynamicFlag() &&
            (l_editing_protocol_postion != l_current_protocol->GetPosition()) &&
            (PQM_SLAVE_PROTOCOL != IsMasterSlaveProtocol(l_current_protocol))  &&
            (VFC_AMB_MODE_SLAVE != l_current_protocol->GetAMBModeFlag()) &&
            (VFC_AMB_MODE_MASTER != l_current_protocol->GetAMBModeFlag())
           ) {



            l_master_series_num_str.Format(_T("%d"), (l_series_bias * l_current_protocol->GetPosition()));
            l_master_list_for_plan_mark->push_back(l_master_series_num_str);

            if (f_pdata->wReserved1 == l_current_protocol->GetPosition()) {
                l_master_of_selected_index = l_master_series_num_str;
            }
        }
    }

    if (-1 != f_pdata->wReserved1) {
        l_master_list_for_plan_mark->push_back(l_master_of_selected_index);
    }
}
//*******************************Method Header*********************************
//Method Name    :SetMasterProtocolForAllSlaves
//Author         :PATNI/AKR
//Purpose        :
//*****************************************************************************
void CMasterSlave::SetMasterProtocolForAllSlaves()
{
    LPCTSTR FUNC_NAME = _T("CMasterSlave::SetMasterProtocolForAllSlaves");
    PQM_TRACE(USER_FUNC_MARKER , FUNC_NAME, _T("Trace"));

    const int l_total_protocol = m_pqm_study->GetProtCountFromVec();
    CPqmProtocol* l_current_protocol = NULL;

    for (int l_index = 0; l_index < l_total_protocol; l_index++) {
        l_current_protocol = m_pqm_study->GetProtocolForIndex(l_index);

        if (l_current_protocol && l_current_protocol->GetGroupID()) {
            l_current_protocol->SetMasterProtocol(GetMasterProtocolForGroupId(l_current_protocol->GetGroupID()));
        }
    }
}
//**************************************************************************
//Method Name   : SetGroupIdForTransferredProtocol
//Author        : PATNI/AKR
//Purpose       : This function will set the group id for a transferred protocol
//**************************************************************************
void CMasterSlave::SetGroupIdForTransferredProtocol(
    CPqmProtocol* const f_current_protocol
)
{
    LPCTSTR FUNC_NAME = _T("CMasterSlave::SetGroupIdForTransferredProtocol");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL == f_current_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_current_protocol is NULL"));
        return;
    }

    if (!m_pqm_ptr->GetMulitScanDynamic_License()) {
        return;
    }

    bool l_can_update_group_num = false;
    const int l_is_master_slave = IsMasterSlaveProtocol(f_current_protocol);

    if (PQM_MASTER_PROTOCOL == l_is_master_slave) {

        l_can_update_group_num = true;
        m_pqm_study->SetGroupIDToVF(f_current_protocol, GetNewGroupId());

    } else if (PQM_SLAVE_PROTOCOL == l_is_master_slave) {

        m_pqm_study->SetGroupIDToVF(f_current_protocol, m_group_num);
        m_pqm_study->SetBreathHoldValue(f_current_protocol, m_pqm_study->GetVfDBHandle());

        if (0 == m_group_num) {
            l_can_update_group_num = true;

        } else {
            f_current_protocol->SetMasterProtocol(GetMasterProtocolForGroupId(f_current_protocol->GetGroupID()));
            UpdateSlaveForMaster(f_current_protocol);
        }
    }

    if (true == l_can_update_group_num) {
        SetGroupNumToVFC();
    }
}


//**************************************************************************
//Method Name   : GetMasterProtocolForGroupId
//Author        : PATNI/AKR
//Purpose       : This will return the master protocol for a given group id,
//
//**************************************************************************
CPqmProtocol* CMasterSlave::GetMasterProtocolForGroupId(
    const int f_group_id
) const
{
    LPCTSTR FUNC_NAME = _T("CMasterSlave::GetMasterProtocolForGroupId");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    const int l_total_protocol = m_pqm_study->GetProtCountFromVec();

    CPqmProtocol* l_current_protocol = NULL, *l_master_protocol = NULL;

    for (int l_index = l_total_protocol - 1; l_index >= 0; l_index--) {

        l_current_protocol = m_pqm_study->GetProtocolForIndex(l_index);

        if (NULL == l_current_protocol) {
            return NULL;
        }

        if (PQM_MASTER_PROTOCOL == IsMasterSlaveProtocol(l_current_protocol) &&
            f_group_id == l_current_protocol->GetGroupID()) {

            l_master_protocol = l_current_protocol;
            break;
        }
    }

    return l_master_protocol;
}

//*******************************Method Header*********************************
//Method Name    :SetGroupNum
//Author         :PATNI/AKR
//Purpose        :Set the group num value
//*****************************************************************************
void CMasterSlave::SetGroupNum(
    const int f_group_num
)
{
    m_group_num = f_group_num;
}


//**************************************************************************
//Method Name   : IdentifiyMasterSlaveGroupID
//Author        : PATNI/AKR
//Purpose       : Private: This protocol will identify the selected protocols's group id,
//				  and being master/slave or normal protocol.
//**************************************************************************
void CMasterSlave::IdentifiyMasterSlaveGroupID(
    const int f_selected_count,
    int* const f_selected_items
)
{
    LPCTSTR FUNC_NAME = _T("CMasterSlave::IdentifiyMasterSlaveGroupID");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    //Saving second row of the array, i.e. recongnizing master/slave protocol &&
    //Saving third row of the array, i.e. Selected protocol's group id
    CPqmProtocol* l_current_protocol = NULL;

    for (int l_index = 0; l_index < f_selected_count; l_index++) {
        l_current_protocol = m_pqm_study->GetProtocolForIndex(f_selected_items[l_index]);

        if (NULL == l_current_protocol) {
            return;
        }

        const int l_master_slave = IsMasterSlaveProtocol(l_current_protocol);

        if (PQM_MASTER_PROTOCOL == l_master_slave && C_PR_STATUS_CURRENT == l_current_protocol->GetAcqStatus() &&
            !m_pqm_ptr->GetAcqManager()->GetScanProcedure()->GetIsWaitingFlagForSARCtrl() &&
            !IsAlreadyMasterBeforeCurrent()) {

            f_selected_items[f_selected_count * SECOND_ROW_INDEX + l_index] = PQM_NORMAL_PROTOCOL;
            f_selected_items[f_selected_count * THIRD_ROW_INDEX + l_index] = 0;

        } else {
            f_selected_items[f_selected_count * SECOND_ROW_INDEX + l_index] = l_master_slave;
            f_selected_items[f_selected_count * THIRD_ROW_INDEX + l_index] = l_current_protocol->GetGroupID();
        }
    }
}

//**************************************************************************
//Method Name   : DecideGroupIdForDuplicated
//Author        : PATNI/AKR
//Purpose       : This is the function where group id is being decided.
//				  Returns if group num need to be updated or not and new group id
//				  by out parameter in the fourth row of the given array.
//**************************************************************************
bool CMasterSlave::DecideGroupIdForDuplicated(
    const int f_selected_count,
    int* const f_selected_items
)
{
    LPCTSTR FUNC_NAME = _T("CMasterSlave::DecideGroupIdForDuplicated");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    bool l_can_update_group_num = false;

    CPqmProtocol* l_current_protocol = NULL;

    for (int l_index = 0; l_index < f_selected_count; l_index++) {

        l_current_protocol = m_pqm_study->GetProtocolForIndex(f_selected_items[l_index]);

        if (NULL == l_current_protocol) {
            break;
        }

        if (PQM_MASTER_PROTOCOL == f_selected_items[f_selected_count * SECOND_ROW_INDEX + l_index]) {

            //Set the new allocated group id in the 4th row for master
            f_selected_items[f_selected_count * FOURTH_ROW_INDEX + l_index] = GetNewGroupId();
            l_can_update_group_num = true;

        } else if (PQM_SLAVE_PROTOCOL == f_selected_items[f_selected_count * SECOND_ROW_INDEX + l_index]) {

            //Set the new allocated group id in the 4th row for slave
            f_selected_items[f_selected_count * FOURTH_ROW_INDEX + l_index] = DecideGroupIdForDuplicatedSlave(f_selected_items, f_selected_count, l_index);
        }
    }

    //when we have generated new group id, then only need to update group num
    return l_can_update_group_num;
}
//**************************************************************************
//Method Name   : DecideGroupIdForDuplicatedSlave
//Author        : PATNI/AKR
//Purpose       : Private: This function will decide group id for slave
//**************************************************************************
int CMasterSlave::DecideGroupIdForDuplicatedSlave(
    const int* const f_selected_items,
    const int f_selected_count,
    const int f_selected_slave
) const
{
    LPCTSTR FUNC_NAME = _T("CMasterSlave::DecideGroupIdForDuplicatedSlave");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    int l_group_id = f_selected_items[f_selected_count * THIRD_ROW_INDEX + f_selected_slave]; //Initially group id will be its own

    //loop starts from one less then their own index to know master has been selected or not befor slave,
    for (int l_index = 0; l_index < f_selected_slave; l_index++) {
        if ((PQM_MASTER_PROTOCOL == f_selected_items[f_selected_count * SECOND_ROW_INDEX + l_index])) { //If any master present

            l_group_id = m_group_num; //Slave's new id will be same as recent group num.
            break;		//a slave can have only one master
        }
    }

    return l_group_id; //new group id or slaves older id(if master is not selected with slave to duplicate).
}
//**************************************************************************
//Method Name   : DeleteSlaveGroupId
//Author        : PATNI/AKR
//Purpose       : This function is called when a master protocol is going to delete
//				  this will reset group id for this master's slave
//**************************************************************************
void CMasterSlave::DeleteSlaveGroupId(
    const int f_master_group_id
)
{
    LPCTSTR FUNC_NAME = _T("CMasterSlave::DeleteSlaveGroupId");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    const int l_total_protocols = m_pqm_study->GetProtCountFromVec();
    CPqmProtocol* l_current_protocol = NULL;

    for (int l_index = 0; l_index < l_total_protocols; l_index++) {

        l_current_protocol = m_pqm_study->GetProtocolForIndex(l_index);

        if (NULL == l_current_protocol) {
            break;
        }

        if (f_master_group_id == l_current_protocol->GetGroupID()) {

            ChangeToNormalProtocol(l_current_protocol);
        }
    }
}
//*******************************Method Header*********************************
//Method Name    :PqmPmSaveForPlanDynamic()
//Author         :PATNI/AKR
//Purpose        :This function will be execute when click on plan dynamic
//*****************************************************************************
status_t CMasterSlave::PqmPmSaveForPlanDynamic(
    CPqmProtocol* const f_master_protocol,
    const char* const f_scratchpad_file,
    SDynamicParamCtrls* const f_plan_dynamic_ctrls,
    const int f_plan_dynamic_ctrls_length
)
{

    LPCTSTR FUNC_NAME = _T("CMasterSlave::PqmPmSaveForPlanDynamic");
    PQM_SCOPED_TRACE();

    if (NULL == f_plan_dynamic_ctrls) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_plan_dynamic_ctrls is NULL"));
        return E_ERROR;
    }

    //Plan dynamic menu item is enabled only for master or normal, but not for slave.
    //If selected protocol is not a master protocol, make it master protocol, even if on wse cancle, it will remain master.
    //Group Num is also need to reset.
    if (!f_master_protocol->GetMasterFlag()) {
        CreateNewMaster(f_master_protocol);
    }

    //Read parameteres from scratchpad.
    if (E_NO_ERROR != ReadDynamicParams(f_plan_dynamic_ctrls, f_plan_dynamic_ctrls_length, 0, f_scratchpad_file)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ReadDynamicPlanParams failed...."));
        return E_ERROR;
    }

    //Overwrite scratchpad with master protocol
    if (E_NO_ERROR != m_pqm_study->MaintainScratchPadForPrivateProlim(f_master_protocol->GetProtocol(), f_scratchpad_file, SP_CREATE_MODE_WSE)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("MaintainScratchPadForPrivateProlim failed...."));
        return E_ERROR;
    }

    //update scratchpad
    if (E_NO_ERROR != UpdateDynamicParams(f_scratchpad_file, f_plan_dynamic_ctrls, f_plan_dynamic_ctrls_length)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("UpdateDynamicPlanParams failed...break."));
        return E_ERROR;
    }

    return E_NO_ERROR;
}
//*******************************Method Header*********************************
//Method Name    :PqmPmSaveForAutoCopy()
//Author         :PATNI/Varun
//Purpose        :This function will autocopy master to slave or all slaves using scratch pad
//*****************************************************************************
status_t CMasterSlave::PqmPmSaveForAutoCopy(
    CPqmProtocol* const f_master_protocol,
    AutoCopyParameter_type f_parameter,
    SDynamicParamCtrls* const f_autocopy_dynamic_ctrls,
    const int f_autocopy_dynamic_ctrls_length,
    const overwrite_info_mode f_overwrite_info_mode
)
{

    LPCTSTR FUNC_NAME = _T("CMasterSlave::PqmPmSaveForAutoCopy");
    PQM_SCOPED_TRACE();

    if (NULL == f_autocopy_dynamic_ctrls) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_autocopy_dynamic_ctrls is NULL."));
        return E_ERROR;
    }

    int l_slave_protocol_count = 0;
    int* l_slave_protocols_index = NULL;
    CString l_pqm_procon_sp(_T(""));

    switch (f_overwrite_info_mode) {

        case AUTO_COPY_PQM:

            if (!f_master_protocol->GetMasterFlag()) {
                CreateNewMaster(f_master_protocol);
            } //no break

        case AUTO_COPY_MEMORY:
            l_slave_protocol_count = 1;
            l_slave_protocols_index = f_parameter.m_protocol_index;
            break;

        case AUTO_COPY_PROCON:
            l_slave_protocol_count = GetSlavesProtocolIndex(f_master_protocol->GetGroupID(), &l_slave_protocols_index);
            l_pqm_procon_sp = f_parameter.m_scratchpad;
            break;

        default:
            return E_ERROR;
    }

    if (!l_slave_protocol_count || NULL == l_slave_protocols_index) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("No Slave present for the master."));

        if (AUTO_COPY_PROCON == f_overwrite_info_mode) {
            DEL_PTR_ARY(l_slave_protocols_index);
        }

        return E_NO_ERROR;
    }

    int* l_imge = f_master_protocol->GetImageGr();
    CVarFieldHandle* l_vf_handle = m_pqm_study->GetVfDBHandle();
    ClibVfToUiAdapter l_vuiAdapter(l_vf_handle);

    char l_autocopy_sp_char[256] = {0};
    GetAutoCopyScratchPadPath(l_autocopy_sp_char);
    CString l_autocopy_sp(l_autocopy_sp_char);



    status_t l_status = E_NO_ERROR;

    CPqmProtocol* l_current_protocol = NULL;
    char* l_study_file_path = CPqmUtilities::ConvertCHAR(m_pqm_study->GetStudyPath());

    for (int l_protocol_index = 0; l_protocol_index < l_slave_protocol_count; l_protocol_index++) {

        l_current_protocol = m_pqm_study->GetProtocolForIndex(l_slave_protocols_index[l_protocol_index]);

        if (NULL == l_current_protocol) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_current_protocol is NULL"));
            l_status = E_ERROR;
            break;
        }


        //Read dynamic parameters of Slave's from VFC and sotre in to structure
        if (E_NO_ERROR != ReadDynamicParams(f_autocopy_dynamic_ctrls, f_autocopy_dynamic_ctrls_length,
                                            l_current_protocol->GetProtocol(), l_study_file_path)) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ReadDynamicPlanParams failed...break."));
            l_status = E_ERROR;
            break;
        }

        //Create a new scratch with master protocol information
        if (AUTO_COPY_PROCON == f_overwrite_info_mode) {
            if (CopyFile(l_pqm_procon_sp, l_autocopy_sp, FALSE) == 0) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CopyFile failed...break."));
                l_status = E_ERROR;
                break;
            }

            m_pqm_study->SaveTagsBeforeScratchpad(f_master_protocol->GetProtocol());

        } else if (E_NO_ERROR != m_pqm_study->CreatScratchPadFromStudy(f_master_protocol->GetProtocol(), l_autocopy_sp_char, SP_CREATE_MODE_PRIVATE_PROLIM_MSD)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CreatScratchPadFromStudy failed...."));
            l_status = E_ERROR;
            break;

        }

        //Update retained dynamic parameters to autocopy scratchpad
        if (E_NO_ERROR != UpdateDynamicParams(l_autocopy_sp_char, f_autocopy_dynamic_ctrls, f_autocopy_dynamic_ctrls_length)) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("UpdateDynamicPlanParams failed...break."));
            l_status = E_ERROR;
            break;
        }

        //Note: LibPLTreePvtWrapper is loaded and unloaded for each protocol,
        //because prolim pvt clears some of the used data at the time of unloading only.
        CLibPLTreeWrapper l_libpltreepvt_obj(l_vf_handle);

        if (!(l_libpltreepvt_obj.ExecutePlUpdateWrapper(m_pqm_study, f_master_protocol->GetProtocol(), l_autocopy_sp_char))) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Prolim wrapper library failed to load.."));
            l_status = E_ERROR;
            break;
        }

        //Copy updated Master protocol's information from autocopy scratchpad to study file

        if (E_NO_ERROR != l_vuiAdapter.VuiCopyProtocolFromScratchPad(l_current_protocol->GetProtocol(), l_autocopy_sp_char, 0, l_imge[1], NULL)) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("VuiCopyProtocolFromScratchPad failed...break."));
            l_status = E_ERROR;
            break;
        }

        if (E_ERROR == m_pqm_study->RestoreSavedTagsAfterScratchpad(l_current_protocol->GetProtocol())) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("RestoreSavedTagsAfterScratchpad failed to restore SVN tags value in varfield"));
        }

        if (m_pqm_study->IsProtocolInScan(f_master_protocol->GetProtocol()))
            m_pqm_ptr->GetStudyManagerPtr()->AddWFDACAPPValue(f_master_protocol->GetSeriesLOID(), l_current_protocol->GetSeriesLOID());



        //Delete the newly created scratchpad.
        if (!DeleteFile(l_autocopy_sp)) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("DeleteFile failed, as specified file not present."));
        }

        if (E_ERROR == UpdateProtocolInformation(l_vf_handle, l_current_protocol)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("UpdateProtocolInformation failed."));
            l_status = E_ERROR;
            break;
        }

    }//end of for loop



    if (E_ERROR == l_status) {
        CString l_err_msg =  CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_AUTO_COPY_FAILED"));
        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(l_err_msg, MPlus::EH::Information, _T("PQM"), _T("PQM"));

    }

    //Note: This message will update gui for all slave protocols of auto copy master.
    if (E_ERROR != l_status && AUTO_COPY_PROCON == f_overwrite_info_mode) {

        m_protocol_index_to_update = f_master_protocol->GetPosition() - 1;
        m_pqm_ptr->WriteToUI(PQM_MSG_AUTO_COPY_DONE);
        m_protocol_index_to_update  = -1;
        DEL_PTR_ARY(l_slave_protocols_index); //only this is dynamically allocated

    } else if (E_ERROR != l_status) {
        CPqmProtocol* l_slave_protocol = m_pqm_study->GetProtocolForIndex(l_slave_protocols_index[0]);

        if (l_slave_protocol) {
            l_slave_protocol->SetMasterProtocol(f_master_protocol);
        }
    }

    DEL_PTR_ARY(l_study_file_path);

    return l_status;
}

//*******************************Method Header*********************************
//Method Name    :ReadDynamicParams()
//Author         :PATNI/AKR
//Purpose        :This function reads the dynamic parameters of slave protocol from VFC to be retained
//*****************************************************************************
status_t CMasterSlave::ReadDynamicParams(
    SDynamicParamCtrls* const f_dynamic_ctrls,
    const int f_dynamic_ctrls_length,
    const int f_protocol_acq_order,
    const char* const f_file_path
)
{
    LPCTSTR FUNC_NAME = _T("CMasterSlave::ReadDynamicPlanParams");
    PQM_SCOPED_TRACE();

    if (NULL == f_dynamic_ctrls || NULL == f_file_path) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Invalid parameters"));
        return E_ERROR;
    }

    CVarFieldHandle l_vf_handle(f_file_path);

    if (E_NO_ERROR != l_vf_handle.VuiOpen()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_vf_handle.VuiOpen() failed.!!"));
        return E_ERROR;
    }

    VfFieldSpec_t*      l_vfreq = new VfFieldSpec_t[f_dynamic_ctrls_length];

    if (NULL == l_vfreq) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_vfreq is NULL..failed to allocate memory.!!"));
        return E_ERROR;
    }

    //Initialize VfFieldSpec_t array
    memset(l_vfreq, 0, sizeof(VfFieldSpec_t) *f_dynamic_ctrls_length);

    VfPathElem_t        l_pelm[1] = {0};
    l_pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_pelm[SVD_ST_PROTOCOL - 1].index = f_protocol_acq_order;


    int l_num = 0;

    for (l_num = 0; l_num < f_dynamic_ctrls_length; l_num++) {

        VFF_SET_ARGS(l_vfreq[l_num], f_dynamic_ctrls[l_num].m_name, f_dynamic_ctrls[l_num].m_type, 0,
                     NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);
    }

    int l_numdone = 0;
    status_t l_status = l_vf_handle.GetFields(l_pelm, SVD_ST_PROTOCOL, l_vfreq, l_num, &l_numdone);

    l_vf_handle.VuiClose();

    if (E_NO_ERROR != l_status) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetFields() failed......!!"));
        DEL_PTR_ARY(l_vfreq);
        return E_ERROR;
    }

    //set the field value and size of the tags in the structure
    for (l_num = 0; l_num < f_dynamic_ctrls_length; l_num++) {

        if (l_vfreq[l_num].status == VFE_NO_ERROR) {
            f_dynamic_ctrls[l_num].m_ctrl_value = l_vfreq[l_num].buffer;
            f_dynamic_ctrls[l_num].m_ctrl_size = l_vfreq[l_num].size;
            f_dynamic_ctrls[l_num].m_status = E_NO_ERROR;

        } else {
            f_dynamic_ctrls[l_num].m_ctrl_value = 0;
            f_dynamic_ctrls[l_num].m_ctrl_size = 0;
            f_dynamic_ctrls[l_num].m_status = E_ERROR;
        }
    }

    DEL_PTR_ARY(l_vfreq);

    return l_status;
}


//*******************************Method Header*********************************
//Method Name    :UpdateDynamicParams()
//Author         :PATNI/AKR
//Purpose        :This function update the Slave protocol's information in to the Varfield database
//*****************************************************************************
status_t CMasterSlave::UpdateDynamicParams(
    const char* const f_scratchpad_file,
    SDynamicParamCtrls* const f_dynamic_ctrls,
    const int f_dynamic_ctrls_length
)
{

    LPCTSTR FUNC_NAME = _T("CMasterSlave::UpdateDynamicParams");
    PQM_SCOPED_TRACE();

    if (NULL == f_dynamic_ctrls || NULL == f_scratchpad_file) {
        PQM_TRACE(USER_FUNC_MARKER , FUNC_NAME, _T("Invalid input parameters."));
        return E_ERROR;
    }

    CVarFieldHandle l_vf_handle(f_scratchpad_file);

    if (E_NO_ERROR != l_vf_handle.VuiOpen()) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("l_vf_handle.VuiOpen() failed.!!"));
        return E_ERROR;
    }

    VfFieldSpec_t*  l_vfreq = new VfFieldSpec_t[f_dynamic_ctrls_length];

    if (NULL == l_vfreq) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("l_vfreq is NULL..failed to allocate memory.!!"));
        l_vf_handle.VuiClose();
        return E_ERROR;
    }

    memset(l_vfreq, 0, sizeof(VfFieldSpec_t)*f_dynamic_ctrls_length);

    VfPathElem_t    l_pelm[1] = {0};
    l_pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    l_pelm[SVD_ST_PROTOCOL - 1].index = 0;


    int             l_num = 0;
    int             l_count = 0;

    //Initializing VfFieldSpec_t array to be filled with values from struct
    for (l_count = l_num = 0; l_count < f_dynamic_ctrls_length; l_count++) {
        if ((E_NO_ERROR == f_dynamic_ctrls[l_count].m_status)) {

            VFF_SET_ARGS(l_vfreq[l_num], f_dynamic_ctrls[l_count].m_name,
                         f_dynamic_ctrls[l_count].m_type, f_dynamic_ctrls[l_count].m_ctrl_size,
                         f_dynamic_ctrls[l_count].m_ctrl_value, VFO_REPLACE);
            l_num++;

        } else {

            if (m_pqm_study->IsTagPresentInFile(&l_vf_handle, f_dynamic_ctrls[l_count].m_name, f_dynamic_ctrls[l_count].m_type, 0)) {
                VFF_SET_ARGS(l_vfreq[l_num], f_dynamic_ctrls[l_count].m_name,
                             f_dynamic_ctrls[l_count].m_type, sizeof(int),
                             NULL, VFO_DELETE);
                l_num++;
            }

            CString l_trace_msg(_T(""));
            l_trace_msg.Format(_T("%d tag is absent in scratchpad...!!"), f_dynamic_ctrls[l_count].m_name);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      l_trace_msg);

        }

    }

    int l_numDone = 0;
    status_t        l_status = E_ERROR;
    time_t          ts = 0L;

    //Restoring VfFieldSpec_t value in the scratchpad
    if (l_num) {
        l_status = l_vf_handle.PutFields(l_pelm,
                                         SVD_ST_PROTOCOL, l_vfreq, l_num, (int*) & l_numDone, &ts);
    }


    //Disable the move couch method flag
    m_pqm_study->DisableMoveCouchMethodFlagFromScratchPad(&l_vf_handle);
    l_vf_handle.VuiClose();
    DEL_PTR_ARY(l_vfreq);
    DeleteBuffer(f_dynamic_ctrls, f_dynamic_ctrls_length);
    return l_status;
}

//*******************************Method Header*********************************
//Method Name    :DeleteBuffer
//Author         :PATNI/AKR
//Purpose        :Delete buffer allocated by vfc
//*****************************************************************************
void CMasterSlave::DeleteBuffer(
    SDynamicParamCtrls* const f_dynamic_ctrls,
    const int f_dynamic_ctrls_length
)
{
    for (int l_count = 0; l_count < f_dynamic_ctrls_length; l_count++) {
        FREE_PTR(f_dynamic_ctrls[l_count].m_ctrl_value);
    }
}

//*******************************Method Header*********************************
//Method Name    :DisplayWarningMessage
//Author         :PATNI/AKR
//Purpose        :
//*****************************************************************************
bool CMasterSlave::DisplayWarningMessage(const CString& f_warning_message)
{
    MPLUSERRORUSERRESPONSE_e l_response = (MPLUSERRORUSERRESPONSE_e) CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(
            f_warning_message,
            _T("PQM"),
            SCAN_WARN_INFORMATION,
            SCAN_WARN_SYSMODAL,
            SCAN_WARN_OK_CANCEL_BUTTONS
                                          );
    return (l_response == MPlusErrResponse_OK) ? true : false;
}


//*******************************Method Header*********************************
//Method Name    :IsMasterSlaveInfoSame
//Author         :PATNI/SS
//Purpose        :Redmine-2188// check following requirment
// Check#1  //Projection counter of Master and slave should be same.
// Check#2 //Home position of Master and slave should be same.
// Check#3 //Current Couch position of Master and slave should be almost same.
//If the protocol move couch is ON,
//PQM should calculate couch position after move couch, and then compare it.
// Check#4 //If the protocol is Cardiac or Head NOVECTOR/STARTED status
//PQM should set vector, and then compare it.
//Due to hardware accuracy, +-2mm margin can be allowed.
/*It means :
const int AllowableCouchDiff = 2;
if (abs(MasterCouchPosition - SlaveCouchPosition) <= AllowableCouchDiff) {
// OK
} else {
// NG. }*/
// Check#5	//Connected coil of Master and slave should be same.
// Check#6  //SAR anatomy of Master and slave should be same.
//*****************************************************************************
status_masterslave_s  CMasterSlave::IsMasterSlaveInfoSame(
    CPqmProtocol* const f_slave_prot,
    CPqmProtocol* const f_master_prot
)
{
    LPCTSTR FUNC_NAME = _T("CMasterSlave::IsMasterSlaveInfoSame");
    PQM_SCOPED_TRACE();

    // master and slave pointer should not NULL
    if (NULL == f_slave_prot || NULL ==  f_master_prot) {
        PQM_TRACE(USER_FUNC_MARKER , FUNC_NAME, _T("either f_slave_prot or f_master_prot is NULL!"));
        return STATUS_ERROR;
    }

    CString l_msg = _T("");

    // Check#1
    //Projection counter of Master and slave should be same
    if (f_slave_prot->GetPqmPrMode()->GetScanNumber() != f_master_prot->GetPqmPrMode()->GetScanNumber()) {
        l_msg.Format(_T("ScanNumber Slave(%d) != Master( %d)"), f_slave_prot->GetPqmPrMode()->GetScanNumber(), f_master_prot->GetPqmPrMode()->GetScanNumber());
        PQM_TRACE(USER_FUNC_MARKER , FUNC_NAME, l_msg);
        return NOT_SAME;
    }

    // Check#2
    //Home position of Master and slave should be same.
    if (f_slave_prot->GetPqmPrMode()->GetHomePos() != f_master_prot->GetPqmPrMode()->GetHomePos()) {
        l_msg.Format(_T("Couch home Position Slave(%f) != Master(%f)"), f_slave_prot->GetPqmPrMode()->GetHomePos(), f_master_prot->GetPqmPrMode()->GetHomePos());
        PQM_TRACE(USER_FUNC_MARKER , FUNC_NAME, l_msg);

        return NOT_SAME;
    }

    // Check#3
    //Current Couch position of Master and slave should be almost same.
    CPqmPrMode* l_pr_mode = f_slave_prot->GetPqmPrMode();;
    float   l_new_pos = l_pr_mode->GetCouchPos();

    //If the protocol move couch is ON,
    //PQM should calculate couch position after move couch, and then compare it.
    const int l_couch_move_method = f_slave_prot->GetcouchMethodFlg();

    if ((f_slave_prot->GetHoldScan() & VFC_HOLD_COUCH) && (VFC_MOVE_COUCH_METHOD_NONE != l_couch_move_method)) {

        float l_distance = (l_pr_mode->GetHomePos() - l_pr_mode->GetCouchPos());
        f32vec3_t l_offset = f_slave_prot->GetOffset();
        float z = l_offset.z * 1000.0f;

        if (VFC_PATORI_HEAD_FIRST == l_pr_mode->GetPatoriEndIn()) {
            l_new_pos = l_pr_mode->GetCouchPos() +  floorf(l_distance + z);

        } else {
            l_new_pos = l_pr_mode->GetCouchPos() +  floorf(l_distance - z);
        }

        if (VFC_MOVE_COUCH_METHOD_PARENT_POSI == l_couch_move_method) {
            if (SVN_ST_PROTOCOL == f_slave_prot->GetLocatorImages(0).subtree_name) {
                CPqmProtocol* l_protocol = m_pqm_study->GetItemFromProtocol(f_slave_prot->GetLocatorImages(0).index);

                if (NULL != l_protocol) {
                    l_new_pos  = l_protocol->GetPqmPrMode()->GetCouchPos();
                }
            }
        }
    }

    //Due to hardware accuracy, +-2mm margin can be allowed.
    const float l_allowable_couch_diff = 2.f;
    float l_master_couch_position = f_master_prot->GetPqmPrMode()->GetCouchPos();
    float l_slave_target_couch_position  = l_new_pos;

    if (fabs(l_master_couch_position - l_slave_target_couch_position) > l_allowable_couch_diff) {
        l_msg.Format(_T("Couch Position Slave(%f) and Master(%f)"), l_slave_target_couch_position, l_master_couch_position);
        PQM_TRACE(USER_FUNC_MARKER , FUNC_NAME, l_msg);

        return NOT_SAME;
    }

    CString l_str_msg(_T(""));
    l_str_msg.Format(_T("Calculated couch Position %f"), l_new_pos);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str_msg);
    // Check#4 //If the protocol is Cardiac or Head NOVECTOR/STARTED status
    //PQM should set vector, and then compare it.

    // TODO: Need to check with Tanoue-san, coz cardic check and vector set is all readly performed on current protocol.
    // so do we need to perform same again or not required.


    // Check#5 //Connected coil of Master and slave should be same.
    CPqmCoilData* l_coil_data = CDataManager::GetInstance()->GetPQMCoilData();

    if (m_pqm_ptr->IsSaturnCoilSystem() &&
        (l_coil_data->PqmIsSameCoilInSaturn(f_slave_prot->GetSaturnCoil(),
                                            f_master_prot->GetSaturnCoil(),
                                            f_slave_prot->GetCoilDBTable(),
                                            f_master_prot->GetCoilDBTable()) == FALSE)) {
        PQM_TRACE(USER_FUNC_MARKER , FUNC_NAME, _T("Coil mismatch "));
        return NOT_SAME;
    }

    // Check#6 //SAR anatomy of Master and slave should be same.
    if ((f_slave_prot->GetPqmPrMode()->GetAnatomy() != f_master_prot->GetPqmPrMode()->GetAnatomy())) {
        PQM_TRACE(USER_FUNC_MARKER , FUNC_NAME, _T("SAR Anatomy mismatch "));
        return NOT_SAME;
    }

    return SUCCESS;
}


//*******************************Method Header*********************************
//Method Name    :IsMasterSlaveVectorSame
//Author         :PATNI/SS
//Purpose        :// check requirment#2
//These each parameters of Master and Slave should be same.
//If the conditions are not satisfied, PQM will display error.
//protocol node
//SVN_PLN_FOV
//visual group node
//SVN_PLN_VG_SLICE_ORI_VECTOR,
//SVN_PLN_VG_PHASE_ORI_VECTOR
//SVN_PLN_VG_OFFSET_VECTOR
//*****************************************************************************
status_masterslave_s  CMasterSlave::IsMasterSlaveVectorSame(
    CPqmProtocol* const f_slave_prot,
    CPqmProtocol* const f_master_prot
)
{

    LPCTSTR FUNC_NAME = _T("CMasterSlave::IsMasterSlaveVectorSame");
    PQM_SCOPED_TRACE();

    // master and slave pointer should not NULL
    if (NULL == f_slave_prot || NULL ==  f_master_prot) {
        PQM_TRACE(USER_FUNC_MARKER , FUNC_NAME, _T("either f_slave_prot or f_master_prot is NULL!"));
        return STATUS_ERROR;
    }

    //protocol node
    // SVN_PLN_FOV
    f32vec2_t l_slave_fovVec, l_master_fovVec;
    memset(&l_slave_fovVec, 0, sizeof(f32vec2_t));
    memset(&l_master_fovVec, 0, sizeof(f32vec2_t));
    int             l_numdone = -1;
    VfFieldSpec_t   vfReq[1] = {0};

    VfPathElem_t pelm[SVD_ST_SLICEGROUP] = {0};
    pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;

    pelm[SVD_ST_PROTOCOL - 1].index        = f_slave_prot->GetProtocol();

    VFF_SET_ARGS(vfReq[0], SVN_PLN_FOV, VFT_FLTVEC2, sizeof(l_slave_fovVec), &l_slave_fovVec, 0);


    if (m_pqm_study->GetVfDBHandle()->GetFields(pelm, SVD_ST_PROTOCOL, vfReq, 1, &l_numdone) != E_NO_ERROR) {
        return STATUS_ERROR;
    }

    pelm[SVD_ST_PROTOCOL - 1].index        = f_master_prot->GetProtocol();

    VFF_SET_ARGS(vfReq[0], SVN_PLN_FOV, VFT_FLTVEC2,
                 sizeof(l_master_fovVec), &l_master_fovVec, 0);

    if (m_pqm_study->GetVfDBHandle()->GetFields(pelm, SVD_ST_PROTOCOL, vfReq, 1, &l_numdone) != E_NO_ERROR)
        return STATUS_ERROR;

    if (memcmp(&l_slave_fovVec, &l_master_fovVec, sizeof(f32vec2_t))) {
        PQM_TRACE(USER_FUNC_MARKER , FUNC_NAME, _T("FOV Mismatch"));
        return NOT_SAME;
    }


    f32vec3_t l_slave_offsetvec, l_slave_slicevec, l_slave_pevec;
    f32vec3_t l_master_offsetvec, l_master_slicevec, l_master_pevec;
    VfFieldSpec_t   l_slave_vfReq[3] = {0};
    VfFieldSpec_t   l_master_vfReq[3] = {0};

    VFF_SET_ARGS(l_slave_vfReq[0],
                 SVN_PLN_VG_OFFSET_VECTOR,
                 VFT_FLTVEC3,
                 sizeof(l_slave_offsetvec), &l_slave_offsetvec, 0);
    VFF_SET_ARGS(l_master_vfReq[0],
                 SVN_PLN_VG_OFFSET_VECTOR,
                 VFT_FLTVEC3,
                 sizeof(l_master_offsetvec), &l_master_offsetvec, 0);
    VFF_SET_ARGS(l_slave_vfReq[1],
                 SVN_PLN_VG_SLICE_ORI_VECTOR,
                 VFT_FLTVEC3,
                 sizeof(l_slave_slicevec), &l_slave_slicevec, 0);
    VFF_SET_ARGS(l_master_vfReq[1],
                 SVN_PLN_VG_SLICE_ORI_VECTOR,
                 VFT_FLTVEC3,
                 sizeof(l_master_slicevec), &l_master_slicevec, 0);
    VFF_SET_ARGS(l_slave_vfReq[2],
                 SVN_PLN_VG_PHASE_ORI_VECTOR,
                 VFT_FLTVEC3,
                 sizeof(l_slave_pevec), &l_slave_pevec, 0);
    VFF_SET_ARGS(l_master_vfReq[2],
                 SVN_PLN_VG_PHASE_ORI_VECTOR,
                 VFT_FLTVEC3,
                 sizeof(l_master_pevec), &l_master_pevec, 0);


    pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    pelm[SVD_ST_PROTOCOL - 1].index        = f_slave_prot->GetProtocol();
    pelm[SVD_ST_VISUALGROUP - 1].subtree_name = SVN_ST_VISUALGROUP ;
    pelm[SVD_ST_SLICEGROUP - 1].subtree_name = SVN_ST_SLICEGROUP ;

    memset(&l_slave_offsetvec, 0 , sizeof(f32vec3_t));
    memset(&l_slave_slicevec, 0 , sizeof(f32vec3_t));
    memset(&l_slave_pevec, 0 , sizeof(f32vec3_t));
    memset(&l_master_offsetvec, 0 , sizeof(f32vec3_t));
    memset(&l_master_slicevec, 0 , sizeof(f32vec3_t));
    memset(&l_master_pevec, 0 , sizeof(f32vec3_t));

    if (E_NO_ERROR != m_pqm_study->GetVfDBHandle()->GetFields(pelm, SVD_ST_VISUALGROUP,
            l_slave_vfReq, 3, &l_numdone)) {

        return STATUS_ERROR;
    }

    pelm[SVD_ST_PROTOCOL - 1].index  = f_master_prot->GetProtocol();

    if (E_NO_ERROR != m_pqm_study->GetVfDBHandle()->GetFields(pelm, SVD_ST_VISUALGROUP,
            l_master_vfReq, 3, &l_numdone)) {
        return STATUS_ERROR;
    }


    if (memcmp(&l_slave_offsetvec, &l_master_offsetvec, sizeof(f32vec3_t))
        || memcmp(&l_slave_slicevec, &l_master_slicevec, sizeof(f32vec3_t))
        || memcmp(&l_slave_pevec, &l_master_pevec, sizeof(f32vec3_t))) {
        PQM_TRACE(USER_FUNC_MARKER , FUNC_NAME, _T("Either Offset or Slice or Phase vector mismatch"));
        return NOT_SAME;
    }

    return SUCCESS;
}
//*******************************Method Header*********************************
//Method Name    :IsAlreadyMasterBeforeCurrent
//Author         :PATNI/AKR
//Purpose        :
//*****************************************************************************
bool CMasterSlave::IsAlreadyMasterBeforeCurrent() const
{
    LPCTSTR FUNC_NAME = _T("CMasterSlave::IsAlreadyMasterBeforeCurrent");
    PQM_TRACE(USER_FUNC_MARKER , FUNC_NAME, _T("Trace"));

    bool l_result = false;
    char l_current_file_name[256] = {0};

    if (!CPqmUtilities::GetCurrScatchpadFileName(l_current_file_name)) {
        PQM_TRACE(USER_FUNC_MARKER , FUNC_NAME, _T("Current Scratchpad path not found."));
        return l_result;
    }

    CVarFieldHandle l_vf_handle(l_current_file_name);

    if (E_NO_ERROR != l_vf_handle.VuiOpen()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_vf_handle.VuiOpen() failed.!!"));
        return l_result;
    }

    if (m_pqm_study->IsTagPresentInFile(&l_vf_handle, SVN_PLN_MASTER_FLAG, SVT_PLN_MASTER_FLAG, 0)) {
        l_result = true;
    }

    l_vf_handle.VuiClose();
    return l_result;
}
//*******************************Method Header*********************************
//Method Name    :AbortScan
//Author         :PATNI/SS
//Purpose        :
//*****************************************************************************
void CMasterSlave::AbortScan(
    CPqmProtocol* const f_current_prot
)
{
    LPCTSTR FUNC_NAME = _T("CMasterSlave::AbortScan");
    PQM_TRACE(USER_FUNC_MARKER , FUNC_NAME, _T("Trace"));

    if (!f_current_prot) {
        return ;
    }

    m_pqm_ptr->GetAcqManager()->GetScanProcedure()->ResetCurrentRequest();
    m_pqm_study->SetScanModeOfHead();
    m_pqm_study->SetAcqStatus(f_current_prot, C_PR_STATUS_WAIT);
    m_pqm_ptr->WriteToUI(PQM_MSG_ABORT_WAIT);

    CPQMLogMgr::GetInstance()->DisplayScanError(
        IDS_ERR_SCAN_ABORTED, _T("IDS_ERR_SCAN_ABORTED"),
        SCAN_ERR_NOTIFY, L"CMasterSlave.cpp");
}



//*******************************Method Header*********************************
//Method Name    :CreateNewMaster()
//Author         :PATNI/AKR
//Purpose        :
//*****************************************************************************
void CMasterSlave::CreateNewMaster(CPqmProtocol* const f_master_protocol)
{
    LPCTSTR FUNC_NAME = _T("CMasterSlave::CreateNewMaster");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL == f_master_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_master_protocol is NULL"));
        return;
    }

    m_pqm_study->SetGroupIDToVF(f_master_protocol, GetNewGroupId());
    m_pqm_study->SetMasterFlagToVF(f_master_protocol, TRUE);
    SetGroupNumToVFC();
    m_protocol_index_to_update = f_master_protocol->GetPosition() - 1;
    m_pqm_ptr->WriteToUI(PQM_MSG_MASTER_SLAVE_UPDATE_GUI);
    m_protocol_index_to_update = -1;
}
//*******************************Method Header*********************************
//Method Name    :UpdateProtocolInformation()
//Author         :PATNI/AKR
//Purpose        :
//*****************************************************************************
status_t CMasterSlave::UpdateProtocolInformation(
    CVarFieldHandle* f_vf_handle,
    CPqmProtocol* const f_current_protocol
)
{
    LPCTSTR FUNC_NAME = _T("CMasterSlave::UpdateProtocolInformation");
    PQM_SCOPED_TRACE();

    if (NULL == f_vf_handle || NULL == f_current_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_vf_handle or f_current_protocol is NULL"));
        return E_ERROR;
    }

    BITFLD_DECL(mask, K_ALL_PR);
    BITFLD_INIT(mask, K_ALL_PR, 0);


    int l_scannumber = f_current_protocol->GetPqmPrMode()->GetScanNumber();



    m_pqm_study->GetProtocolItemValues(f_vf_handle, f_current_protocol);

    f_current_protocol->GetPqmPrMode()->SetScanNumber(l_scannumber);

    memset(mask, 0, sizeof(mask));

    m_pqm_study->SetProtocolValues(f_vf_handle, f_current_protocol->GetProtocol(), mask,
                                   K_PR_SCAN_NUMBER, &l_scannumber,  sizeof(int32_t),
                                   NULL);

    m_pqm_study->UpdateSARControlInfoOnProtocolUpdate(f_current_protocol);

    m_pqm_study->GetValueFromVfSAR(f_current_protocol);

    m_pqm_study->RecalculateSAR(f_current_protocol);

    m_pqm_study->SetBreathHoldValue(f_current_protocol, f_vf_handle);

    return E_NO_ERROR;
}
//*******************************Method Header*********************************
//Method Name    :GetAutoCopyScratchPadPath
//Author         :PATNI/AKR
//Purpose        :
//*****************************************************************************
void CMasterSlave::GetAutoCopyScratchPadPath(
    char* const f_path
) const
{
    char*  l_buf = getenv("MRMP_GP_TMP_DIR");
    strcpy(f_path, l_buf);
    strcat(f_path, "//AutoCopyScratchPad.Imaging\0");
}
//**************************************************************************
//Method Name   : GetNewGroupId
//Author        : PATNI/AKR
//Purpose       : This will generate new group id.
//**************************************************************************
int CMasterSlave::GetNewGroupId()
{
    return ++m_group_num;
}
//**************************************************************************
//Method Name   : SetGroupNumToVFC
//Author        : PATNI/AKR
//Purpose       : This will set the group num in vfc
//**************************************************************************
void CMasterSlave::SetGroupNumToVFC()
{
    m_pqm_study->SetGroupNumToVF(m_group_num);
}
//**************************************************************************
//Method Name   : GetGroupNumFromVec
//Author        : PATNI/AKR
//Purpose       : Find the highest group id present in the protocol vector for group num calculation
//**************************************************************************
int CMasterSlave::GetGroupNumFromVec()
{
    LPCTSTR FUNC_NAME = _T("CMasterSlave::GetGroupNumFromVec");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    int l_max_grp_id = 0;
    const int l_total_protocols = m_pqm_study->GetProtCountFromVec();

    if (0 == l_total_protocols) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("no protocols in pqm"));
        return l_max_grp_id;
    }

    //Search whole protocol vector
    CPqmProtocol* l_current_protocol = NULL;
    int l_current_grp_id = 0;

    for (int l_index = 0; l_index < l_total_protocols; l_index++) {

        l_current_protocol = m_pqm_study->GetProtocolForIndex(l_index);

        if (NULL == l_current_protocol) {

            return 	l_max_grp_id;
        }

        l_current_grp_id = l_current_protocol->GetGroupID();

        if (l_max_grp_id < l_current_grp_id) {
            l_max_grp_id = l_current_grp_id; //saves maximum group id.
        }
    }

    return l_max_grp_id;
}

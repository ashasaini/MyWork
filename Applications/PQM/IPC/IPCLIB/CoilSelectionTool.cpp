//REDMINE-1213_Update_29_APR
//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-774
//Redmine-780
//Redmine-781
//Redmine-782
// CoilSelectionTool.cpp: implementation of the CCoilSelectionTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CoilSelectionTool.h"
// + Himanshu SM2 3rd Dec 2008
extern "C" {
#include <tami/libAcq/AcqCoilSignal.h>
}
// - Himanshu SM2 3rd Dec 2008
#include <PQM/PQMLogMgr.h>
#include <PQM/PqmUtilities.h>
#include "pqm.h"
#include "PqmCoilData.h"
#include "PqmSectionData.h"
#include "VarFieldHandle.h" //TMSC-Tanoue/2010Aug25/MVC008345
#include "GPLibAdapter.h" //TMSC-Tanoue/2010Aug25/MVC008345


//****************************Method Header************************************
//Method Name   : CCoilSelectionTool()
//Author        : PATNI/AMT
//Purpose       : C-tor
//*****************************************************************************
//+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
//CCoilSelectionTool::CCoilSelectionTool()
CCoilSelectionTool::CCoilSelectionTool():
    m_pqm(0),
    m_coil_selection_active(FALSE),
    m_acq_send_flag(FALSE)
{

}

//****************************Method Header************************************
//Method Name   : ~CCoilSelectionTool()
//Author        : PATNI/AMT
//Purpose       : D-tor
//*****************************************************************************
CCoilSelectionTool::~CCoilSelectionTool()
{

}

//****************************Method Header************************************
//Method Name   : CCoilSelectionTool()
//Author        : PATNI/AMT
//Purpose       : C-tor - Overloaded
//*****************************************************************************
CCoilSelectionTool::CCoilSelectionTool(CPqm* f_pqm):
    m_coil_selection_active(FALSE),
    m_acq_send_flag(FALSE)
{
    if (f_pqm != NULL) {
        m_pqm = f_pqm;
    }

    memset(&m_connect_coil, 0, sizeof(VftCoilListInfo_t));
    //Patni-Ravi/2010May05/Added/TMSC-REDMINE-111
    memset(&m_coil_sel_last_acq_coil, 0, sizeof(VftCoilListInfo_t));
}

//****************************Method Header************************************
//Method Name   : CSSetCoilName()
//Author        : PATNI/AMT
//Purpose       : Get Coil name from libVFtoUI and sets to protocol pass as
//					argument.
//*****************************************************************************
//+Patni-Hemant/2010Feb12/Modified/ACE-Phase#3/CDS Requirement
bool CCoilSelectionTool::CSSetCoilName(CPqmProtocol* const f_obj,
                                       CVarFieldHandle* const vf_handle)const
//-Patni-Hemant/2010Feb12/Modified/ACE-Phase#3/CDS Requirement
{
    LPCTSTR FUNC_NAME = _T("CCoilSelectionTool::CSSetCoilName ");

    if (f_obj == NULL) {
        return false;
    }

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqm == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("m_pqm object is NULL"));
        return false;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    CPQMStudy*   l_study = m_pqm->GetStudy();
    CVarFieldHandle* l_cur_vf   = l_study->GetVfDBHandle();
    //+Patni-Hemant/2010Mar3/Commented/ACE-Phase#3/CDS Requirement
    //Review comment - Sarjeet san
    //Not used
    //BOOL         l_is_flushed = FALSE;
    //-Patni-Hemant/2010Mar3/Added/ACE-Phase#3/CDS Requirement

    if (l_cur_vf == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_cur_vf is NULL"));
        return false;
    }

    //+Patni-SS/2009Apr01/PSP1#VFLOCK
    CString l_log_msg = _T("");
    l_log_msg.Format(_T(" :VuiSaturnCoilName : protocol no=%d, coilName = %s"), f_obj->GetProtocol(),
                     f_obj->GetSaturnCoil()->CoilName);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_msg);
    //-Patni-SS/2009Apr01/PSP1#VFLOCK

    ClibVfToUiAdapter vuiAdapter(l_cur_vf);
    vuiAdapter.VuiSaturnCoilName(f_obj->GetProtocol(),
                                 f_obj->GetSaturnCoil()->CoilName);
    //+Patni-HAR+Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement

    BITFLD_DECL(mask, K_ALL_PR);

    /* Study writing */
    BITFLD_INIT(mask, K_ALL_PR, 0);
    memset(mask, 0, sizeof(mask));

    CVarFieldHandle* vf_set_handle = vf_handle ? vf_handle : l_cur_vf;

    if (E_NO_ERROR != l_study->SetProtocolValues(vf_set_handle, f_obj->GetProtocol(),
            mask, K_PR_RC_COIL_NAME, f_obj->GetSaturnCoil()->CoilName,
            strlen(f_obj->GetSaturnCoil()->CoilName) + 1, NULL)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SetProtocolValues() failed."));
    }

    //-Patni-HAR+Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement
    return true;
}
// - CoilName AMT

//****************************Method Header************************************
//Method Name   : CoilSelConnectCoil()
//Author        : PATNI/AMT
//Purpose       : Get coil from Acqinfo data
//*****************************************************************************
VftCoilListInfo_t CCoilSelectionTool::CoilSelConnectCoil(
    const AM_AcqManCoilInfo_t& f_coil_Info
)const
{
    typedef struct porttbl_s {
        int		no;
        char	c[3];
    } porttbl_t;

    static porttbl_t l_port[]
    = {	{ACQ_COILINFO_PORT_A,  ACQ_COIL_PORT_A  },
        {ACQ_COILINFO_PORT_B,  ACQ_COIL_PORT_B  },
        {ACQ_COILINFO_PORT_C,  ACQ_COIL_PORT_C  },
        {ACQ_COILINFO_PORT_D,  ACQ_COIL_PORT_D1 },		// D1 on L2,Saturn
        {ACQ_COILINFO_PORT_E,  ACQ_COIL_PORT_D2 },		// D1 on L2,Saturn
        {ACQ_COILINFO_PORT_F,  ACQ_COIL_PORT_F  },
        {ACQ_COILINFO_PORT_G1, ACQ_COIL_PORT_G1 },		// set for Saturn only
        {ACQ_COILINFO_PORT_G2, ACQ_COIL_PORT_G2 },		// set for Saturn only
        {ACQ_COILINFO_PORT_G3, ACQ_COIL_PORT_G3 },		// set for Saturn only
        {ACQ_COILINFO_PORT_G4, ACQ_COIL_PORT_G4 },		// set for Saturn only
        {ACQ_COILINFO_PORT_G5, ACQ_COIL_PORT_G5 },		// set for Saturn only
        {ACQ_COILINFO_PORT_G6, ACQ_COIL_PORT_G6 },		// set for Saturn only
        {ACQ_COILINFO_PORT_G7, ACQ_COIL_PORT_G7 }
    };	// set for Saturn only

    // DB_FUNC_ENTER("[PqmPmCoilSelConnectCoil]:");

    VftCoilListInfo_t	l_coil_connect = {0};

    if (PQM_NUM_COIL_PORT != (sizeof(l_port) / sizeof(porttbl_t))) {
        // PqmUiErrorDialog(PqmUiFetchLiteral("ERR_ACQMAN_ACQINFO_DONE"),ERR_ERROR);
        //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
        //l_coil_connect.num  = 0;
        l_coil_connect.num  = 0l;
        return l_coil_connect;
    }

    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    int	j = 0, k = 0;

    for (int i = ACQ_COILINFO_PORT_A; i < PQM_NUM_COIL_PORT; i++) {

        if (f_coil_Info.port[i].coilId != -1) {

            l_coil_connect.coillist[j].coilID  = f_coil_Info.port[i].coilId;
            l_coil_connect.coillist[j].channel = ' ';

            for (k = 0; k < PQM_NUM_COIL_PORT; k++) {

                if (i == k) {

                    l_coil_connect.coillist[j].portID[0] = l_port[i].c[0];
                    l_coil_connect.coillist[j].portID[1] = l_port[i].c[1];
                }
            }

            j++;
        }
    }

    l_coil_connect.num  = j;
    //----------------------------------debug-----------------------------------
    // DB_PRINTF("B",("l_coil_connect.num =%d\n", l_coil_connect.num));
    // for(i = 0; i < l_coil_connect.num; i++){
    // 	DB_PRINTF("B",("[%d] CoilID =%#x, PortID=%c%c\n",
    //					i, l_coil_connect.coillist[i].coilID,
    //l_coil_connect.coillist[j].portID[0], l_coil_connect.coillist[j].portID[1]));
    //}
    //--------------------------------------------------------------------------
    return l_coil_connect;
}

//****************************Method Header************************************
//Method Name   : CoilSelSaveConnectCoil()
//Author        : PATNI/AMT
//Purpose       : Save last connected coil
//*****************************************************************************
bool CCoilSelectionTool::CoilSelSaveConnectCoil(
    const AM_AcqManCoilInfo_t& f_coil_Info
)
{
    m_coil_sel_last_acq_coil = CoilSelConnectCoil(f_coil_Info);
    //Patni-PJS/2009Oct23/Added/MVC005086
    m_connect_coil = m_coil_sel_last_acq_coil;

    return true;
}

//****************************Method Header************************************
//Method Name   : SaveSelectedListToStudy()
//Author        : PATNI/AMT
//Purpose       : Save selected list of coils to Study file
//*****************************************************************************
bool CCoilSelectionTool::SaveSelectedListToStudy(
    coilSelectListStruct_t* f_coil_listinfo,
    const int f_acq_order
)const
{
    LPCTSTR FUNC_NAME = _T("CCoilSelectionTool::SaveSelectedListToStudy");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqm == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm object is NULL"));
        return false;
    }

    if (f_coil_listinfo == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_coil_listinfo is NULL"));
        return false;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    CPQMStudy*       l_study = m_pqm->GetStudy();
    CVarFieldHandle* const l_vf_handle = l_study->GetVfDBHandle();

    if (!l_vf_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_vf_handle is NULL"));
        return false;
    }

    CPqmProtocol*    l_obj    = l_study->GetProtocol(f_acq_order);

    //+Patni-RUP/2009Aug20/Modified/cpp test corrections
    if (l_obj == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_obj object is NULL"));
        return false;
    }

    //-Patni-RUP/2009Aug20/Modified/cpp test corrections
    PqmSaturnCoil_p l_saturn_coil = l_obj->GetSaturnCoil();
    l_saturn_coil->CoilList.num = f_coil_listinfo->m_listInfo.num;

    for (int i = 0; i < f_coil_listinfo->m_listInfo.num; i++) {
        l_saturn_coil->CoilList.coillist[i] = f_coil_listinfo->m_listInfo.coillist[i];
        l_saturn_coil->CoilListcoordinate.coordinate_list[i] = f_coil_listinfo->m_listCoordinateListInfo.coordinate_list[i];
    }

    //    memcpy( (VftCoilListInfo_t )l_saturn_coil->CoilList, f_coil_list,
    //sizeof( VftCoilListInfo_t ));

    l_study->PutSaturnCoil(l_vf_handle, l_obj->GetProtocol(),
                           l_obj->GetSaturnCoil());
    l_study->PutConnectedCoil(l_vf_handle, l_obj->GetProtocol(), l_obj);
    //}
    return true;
}

//****************************Method Header************************************
//Method Name   : CoilSetProtocol()
//Author        : PATNI/AMT
//Purpose       : Save coil information to selected protocol & into study file
//*****************************************************************************
bool CCoilSelectionTool::CoilSetProtocol(
    CPqmProtocol* const f_obj,
    coilSelect_t* const f_selected_coil
)const
{
    LPCTSTR FUNC_NAME = _T("CCoilSelectionTool::CoilSetProtocol");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqm == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("m_pqm pointer is NULL"));
        return false;
    }

    if (f_obj == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("f_obj pointer is NULL"));
        return false;
    }

    if (f_selected_coil == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("f_selected_coil is NULL"));
        return false;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    BITFLD_DECL(mask, K_ALL_PR) = {0};
    PqmSaturnCoil_p l_saturn_coil = NULL;

    l_saturn_coil = f_obj->GetSaturnCoil();
    l_saturn_coil->numSection = f_selected_coil->num;

    for (int j = 0; j < f_selected_coil->num; j++) {
        l_saturn_coil->RcvCoil[j] = f_selected_coil->select[j];
    }

    CDataManager* l_data_mgr = m_pqm->GetDataManager();
    DllT* l_coil_head    = l_data_mgr->GetPQMCoilData()->GetHead();
    DllT* l_section_head = l_data_mgr->GetPQMSectionData()->GetHead();

    AcqGetcoilSignal(f_obj->GetSystemChannels(), l_saturn_coil->numSection,
                     l_saturn_coil->RcvCoil, l_saturn_coil->Signal,
                     l_coil_head, l_section_head);

    CPqmCoilData* l_coil_data = l_data_mgr ->GetPQMCoilData();


    if (E_NO_ERROR != l_coil_data->PqmAllCoilGetSaturn(f_obj->GetSaturnCoil(),
            f_obj->GetCoilDBTable())) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PqmAllCoilGetSaturn failed"));
    }

    // Write Coil Information to Study File
    m_pqm->GetStudy()->PutSaturnCoil(m_pqm->GetStudy()->GetVfDBHandle(),
                                     f_obj->GetProtocol(), f_obj->GetSaturnCoil());

    m_pqm->GetStudy()->PutConnectedCoil(m_pqm->GetStudy()->GetVfDBHandle(),
                                        f_obj->GetProtocol(), f_obj);

    m_pqm->GetStudy()->PutRecvrCoil(m_pqm->GetStudy()->GetVfDBHandle(),
                                    f_obj->GetProtocol(), f_obj);

    // Set Transmitter coil to study file.

    if (!m_pqm->GetStudy()->SetTxCoilInSaturn(m_pqm->GetStudy()->GetVfDBHandle(), f_obj,
            f_obj->GetCoilDBTable())) {
        // If Transmitter coil is not found, then show error.

        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
            CPqmUtilities::GetMultiLingualString(_T("IDS_CS_ERR_ILLEGAL_COIL_ID")),																		  // Added By KT/ACE-2/11-06-2009/Multilingualisation
            MPlus::EH::Information, _T("CoilSelectionTool.cpp"), _T("PQM"));
    }

    //+Patni-Hemant/2010Feb22/Modified/ACE-Phase#3/CDS Requirement
    /*
    CSSetCoilName(f_obj);	// CoilName AMT

    // Write to Study

    BITFLD_INIT(mask, K_ALL_PR, 0);	//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    memset(mask, 0, sizeof(mask));

    #ifdef VFUSERLIB_WRAPPER
    m_pqm->GetStudy()->SetProtValues(m_pqm->GetStudy()->GetVfDBHandle(),
                                     VF_SYNCHRONIZE,  f_obj->GetProtocol(), mask,
                                     K_PR_RC_COIL_NAME,	 f_obj->GetSaturnCoil()->CoilName, strlen(f_obj->GetSaturnCoil()->CoilName) + 1,
                                     NULL);
    #else
    m_pqm->GetStudy()->SetProtValues(m_pqm->GetStudy()->GetVfDBHandle(),
                                     f_obj->GetProtocol(), mask,
                                     K_PR_RC_COIL_NAME,	 f_obj->GetSaturnCoil()->CoilName, strlen(f_obj->GetSaturnCoil()->CoilName) + 1,
                                     NULL);
    #endif
      */

    if (!CSSetCoilName(f_obj, m_pqm->GetStudy()->GetVfDBHandle())) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CSSetCoilName failed."));
    }

    //-Patni-Hemant/2010Feb22/Commented/ACE-Phase#3/CDS Requirement
    return true;
}

//****************************Method Header************************************
//Method Name   : SaveSelectedCoilToStudy()
//Author        : PATNI/AMT
//Purpose       : Save sected coil information to study file
//*****************************************************************************
bool CCoilSelectionTool::SaveSelectedCoilToStudy(
    coilSelect_t*  f_selected_coil,
    const int f_acq_order
)const
{
    LPCTSTR FUNC_NAME = _T("CCoilSelectionTool::SaveSelectedCoilToStudy");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqm == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("m_pqm object is NULL"));
        return false;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    CPQMStudy* l_study = m_pqm->GetStudy();
    CPqmProtocol* l_obj = l_study->GetProtocol(f_acq_order);

    if (l_obj) {
        if (!CoilSetProtocol(l_obj, f_selected_coil)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CoilSetProtocol() failed."));
            return false;
        }
    }

    return true;
}

void CCoilSelectionTool::ResetAcqInfoFlags(
)
{
    m_coil_selection_active = FALSE;
    m_acq_send_flag			= TRUE;
}


//****************************Method Header************************************
//Method Name   : GetConnectCoilInformation()
//Author        : PATNI / HAR
//Purpose       :
//*****************************************************************************
VftCoilListInfo_t* CCoilSelectionTool::GetConnectCoilInformation(
)
{
    return &m_connect_coil;
}

//****************************Method Header************************************
//Method Name   : CoilSelGetConnectCoil()
//Author        : PATNI/AMT
//Purpose       : Return last saved acq coil
//*****************************************************************************
VftCoilListInfo_t* CCoilSelectionTool::CoilSelGetConnectCoil(
)
{
    return &m_coil_sel_last_acq_coil;
}

//****************************Method Header************************************
//Method Name   : SetConnectCoil()
//Author        : PATNI / HAR
//Purpose       :
//*****************************************************************************
BOOL CCoilSelectionTool::SetConnectCoil(
    const AM_AcqManCoilInfo_t& f_coil_info
)
{
    //DB_FUNC_ENTER("[PqmPmCoilSelSetConnectCoil]:")

    if (m_coil_selection_active) {

        m_connect_coil = CoilSelConnectCoil(f_coil_info);

        m_pqm->WriteToUI(PQM_MSG_PQM2COILSEL_CONNECT_COIL);
        //ProMsgSend(pmObj->core.wrFd,PROMSG_PQM2COILSEL_CONNECT_COIL,sizeof(coilconnect),&coilconnect);

        ResetAcqInfoFlags();

        return TRUE;

    } else {
        return FALSE;
    }
}

//****************************Method Header************************************
//Method Name   : GetCoilSelectionFlagActive()
//Author        : PATNI/HAR
//Purpose       :
//*****************************************************************************
BOOL CCoilSelectionTool::GetCoilSelectionFlagActive(
) const
{
    return m_coil_selection_active;
}

//****************************Method Header************************************
//Method Name   : SetCoilSelectionFlagActive()
//Author        : PATNI/HAR
//Purpose       :
//*****************************************************************************
void CCoilSelectionTool::SetCoilSelectionFlagActive(
    const BOOL f_flag
)
{
    m_coil_selection_active = f_flag;
}

//****************************Method Header************************************
//Method Name   : GetAcqSendFlag()
//Author        : PATNI/HAR
//Purpose       :
//*****************************************************************************
BOOL CCoilSelectionTool::GetAcqSendFlag(
) const
{
    return m_acq_send_flag;
}

// + CoilName AMT

//+Patni-PJS/2009Oct23/Added/MVC005086
//****************************Method Header************************************
//Method Name   : SetAcqSendFlag()
//Author        : PATNI/PJS
//Purpose       : Set the variable
//*****************************************************************************
void CCoilSelectionTool::SetAcqSendFlag(
    const BOOL f_acq_send
)
{
    m_acq_send_flag = f_acq_send;
}
//-Patni-PJS/2009Oct23/Added/MVC005086

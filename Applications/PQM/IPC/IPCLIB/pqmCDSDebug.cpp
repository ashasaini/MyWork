//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-780
//Redmine-782
// pqmCDSDebug.cpp: implementation of the CpqmCDSDebug class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pqmCDSDebug.h"
//Patni-Rajendra/2011Mar29/Modified/TMSC-REDMINE-773
#include <libStudyManager/Log/ScopedFuncInOutLog.h>
#include <PQM/PQMLogMgr.h>
#include "Pqm.h"
#include "CDSUtilities.h"
#include "VarFieldHandle.h" //TMSC-Tanoue/2010Aug25/MVC008345

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//************************************Method Header************************************
// Method Name  : CpqmCDSDebug
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CpqmCDSDebug::CpqmCDSDebug():
    m_cdsdebug_flag(false),
    m_currentCDSdebugprotocol(-1),
    m_pqm(NULL)

{
}

//************************************Method Header************************************
// Method Name  : ~CpqmCDSDebug
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CpqmCDSDebug::~CpqmCDSDebug()
{
    //No need to delete, it is shared
    if (m_pqm) {
        m_pqm = NULL ;
    }
}

//************************************Method Header************************************
// Method Name  : PqmUiCdsDebugWindowCB
// Author       : PATNI/ HEMANT
// Purpose      : This dialog will open the CDS debug location dialog
//***********************************************************************************
void CpqmCDSDebug::PqmUiCdsDebugWindowCB(
    CPqmProtocol* const f_protocol_obj
)
{
    LPCTSTR FUNC_NAME = _T("CpqmCDSDebug::PqmUiCdsDebugWindowCB");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (!f_protocol_obj || !m_pqm) {

        ASSERT(FALSE);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_protocol_obj or m_pqm is NULL"));

        return ;
    }

    //save the curent protocol acq order
    SetCurrentCDSDebugProtocol(f_protocol_obj->GetProtocol());

    //invoke CDS Debug dialog

    m_pqm->WriteToUI(PQM_MSG_CDS_DEBUG);
}

//************************************Method Header************************************
// Method Name  : pqmUiDebugSetCdsCB
// Author       : PATNI/ HEMANT
// Purpose      : This function is called when user pressed Set in CDS debug window.
//                f_compass_debug_param represents the dialog edited parameters
//                Because user has pressed SET button, the given argument will be saved as
//                coil location in CDSInfo data (l_curprot_cdsinfo->m_locationtbl) of the
//                currently running CDS protocol, then this location information will be
//                stored in database.
//***********************************************************************************
void CpqmCDSDebug::pqmUiDebugSetCdsCB(
    COMPASS_Debugg_Param* const f_compass_debug_param
)
{
    LPCTSTR FUNC_NAME = _T("CpqmCDSDebug::pqmUiDebugSetCdsCB");
    PQM_SCOPED_TRACE();

    CPqmProtocol* l_current_protocol = GetDebugProtocol();

    if (!l_current_protocol) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_current_protocol is NULL"));
        ASSERT(FALSE) ;
        return ;
    }

    //get the CDS info object from the protocol
    CpqmCDSInfo* l_curprot_cdsinfo = l_current_protocol->GetCDSInfo();

    //Note : Here we are going to udpate the CDSInfo->lcoationtbl for the current protocol
    //       Here only the coil location and detect flag will be updated, becasue other things will
    //       be same

    if (l_curprot_cdsinfo) {

        CString l_log_str = _T("");

        CPQMStudy* l_study = m_pqm->GetStudy();
        CVarFieldHandle* l_vf_handle = l_study->GetVfDBHandle();

        if (!l_vf_handle) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_vf_handle is NULL"));
            return;
        }

        PQM_TRACE(USER_FUNC_MARKER, _T("CpqmCDSDebug::pqmUiDebugSetCdsCB"), _T("Location Location Modified..."));

        typedef Vft_CdsCoilLocation_t* Vft_CdsCoilLocation_p ;
        Vft_CdsCoilLocation_p l_vft_cdscoillocation_p = NULL;
#define PRINT_LOCATION_DEBUG_INFO(l_cds_coil)													\
    if(l_cds_coil) {																		\
        l_log_str.Format(_T("Coil Id       : %X"), l_cds_coil->coilID) ;					\
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);		\
        \
        l_log_str.Format(_T("Coil Location : %d"), l_cds_coil->location) ;					\
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);		\
        \
        l_log_str.Format(_T("Coil Detect   : %d"), l_cds_coil->detectedFlag) ;				\
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);		\
    }																						\
     

        //first : for port G1 to G7
        for (int l_port_index = ACQ_COILINFO_PORT_G1 ; l_port_index <= ACQ_COILINFO_PORT_G7 ;  l_port_index++) {

            l_curprot_cdsinfo->m_locationtbl[l_port_index].location     = f_compass_debug_param[l_port_index].m_location ;
            l_curprot_cdsinfo->m_locationtbl[l_port_index].detectedFlag = f_compass_debug_param[l_port_index].m_detectedflag ;

            l_vft_cdscoillocation_p = &(l_curprot_cdsinfo->m_locationtbl[l_port_index]);
            PRINT_LOCATION_DEBUG_INFO(l_vft_cdscoillocation_p);
        }

        //second : for port ACQ_COILINFO_PORT_D
        l_curprot_cdsinfo->m_locationtbl[ACQ_COILINFO_PORT_D].location     = f_compass_debug_param[ACQ_COILINFO_PORT_D].m_location ;
        l_curprot_cdsinfo->m_locationtbl[ACQ_COILINFO_PORT_D].detectedFlag = f_compass_debug_param[ACQ_COILINFO_PORT_D].m_detectedflag ;

        l_vft_cdscoillocation_p = &(l_curprot_cdsinfo->m_locationtbl[ACQ_COILINFO_PORT_D]);
        PRINT_LOCATION_DEBUG_INFO(l_vft_cdscoillocation_p);


        //Third : for port ACQ_COILINFO_PORT_E
        l_curprot_cdsinfo->m_locationtbl[ACQ_COILINFO_PORT_E].location     = f_compass_debug_param[ACQ_COILINFO_PORT_E].m_location ;
        l_curprot_cdsinfo->m_locationtbl[ACQ_COILINFO_PORT_E].detectedFlag = f_compass_debug_param[ACQ_COILINFO_PORT_E].m_detectedflag ;

        l_vft_cdscoillocation_p = &(l_curprot_cdsinfo->m_locationtbl[ACQ_COILINFO_PORT_E]);
        PRINT_LOCATION_DEBUG_INFO(l_vft_cdscoillocation_p);


        CCDSUtilities::PutCdsLocation(l_vf_handle,
                                      GetCurrentCDSDebugProtocol(),
                                      l_curprot_cdsinfo->m_locationtbl
                                     );

        CCDSUtilities::GetCdsLocation(l_vf_handle,
                                      GetCurrentCDSDebugProtocol(),
                                      l_curprot_cdsinfo->m_locationtbl
                                     );

        //This will be called from the caller
        //PqmAcqAutoCoilSet(PqmGlobal.pqmStudy.vfHandle, prObj);
    }

#undef PRINT_LOCATION_DEBUG_INFO
}

//************************************Method Header************************************
// Method Name  : GetCDSDebugParam
// Author       : PATNI/ HEMANT
// Purpose      : This functions fills the input argument with the coil location values
//                for the protocol for which to debug with CDS debug dialog
//                This function is called through GUI application.
//                This function first finds the debug protocol index and thus the CPqmProtocol
//                object. and from that it will get the location information to be filled in
//                given argument.
//***********************************************************************************
void CpqmCDSDebug::GetCDSDebugParam(
    COMPASS_Debugg_Param* const f_compass_debug_param
)const   //Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_03
{
    LPCTSTR FUNC_NAME = _T("CpqmCDSDebug::GetCDSDebugParam");
    PQM_SCOPED_TRACE();

    CPqmProtocol* l_current_protocol = GetDebugProtocol();

    if (!l_current_protocol) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_current_protocol is NULL"));
        ASSERT(FALSE) ;
        return ;
    }

    //get the CDS info object from the protocol
    CpqmCDSInfo* l_curprot_cdsinfo = l_current_protocol->GetCDSInfo();

    if (l_curprot_cdsinfo) {

        char buf[128] = {0} ;

        const CString l_default_coil_id = _T("----");

        for (int l_index = 0 ; l_index < MAX_PARAM; l_index++) {
            f_compass_debug_param[l_index].m_coilid = l_default_coil_id ;
            f_compass_debug_param[l_index].m_detectedflag = 0 ;
            f_compass_debug_param[l_index].m_location = 0 ;
        }

        //first : for port G1 to G7
        for (int l_port_index = ACQ_COILINFO_PORT_G1 ; l_port_index <= ACQ_COILINFO_PORT_G7 ;  l_port_index++) {

            //get the coil id in 04x format
            if (l_curprot_cdsinfo->m_locationtbl[l_port_index].coilID != 0x0) {
                sprintf(buf, "%04x", l_curprot_cdsinfo->m_locationtbl[l_port_index].coilID);
                f_compass_debug_param[l_port_index].m_coilid = buf ;
            }

            f_compass_debug_param[l_port_index].m_location       = l_curprot_cdsinfo->m_locationtbl[l_port_index].location ;
            f_compass_debug_param[l_port_index].m_detectedflag   = (l_curprot_cdsinfo->m_locationtbl[l_port_index].detectedFlag > 0) ;

            //clearing the buffer...
            memset(buf, 0, sizeof(buf));
        }

        //second : for port ACQ_COILINFO_PORT_D

        if (l_curprot_cdsinfo->m_locationtbl[ACQ_COILINFO_PORT_D].coilID != 0x0) {
            sprintf(buf, "%04x", l_curprot_cdsinfo->m_locationtbl[ACQ_COILINFO_PORT_D].coilID);
            f_compass_debug_param[ACQ_COILINFO_PORT_D].m_coilid = buf ;
        }

        f_compass_debug_param[ACQ_COILINFO_PORT_D].m_location       = l_curprot_cdsinfo->m_locationtbl[ACQ_COILINFO_PORT_D].location ;
        f_compass_debug_param[ACQ_COILINFO_PORT_D].m_detectedflag   = (l_curprot_cdsinfo->m_locationtbl[ACQ_COILINFO_PORT_D].detectedFlag > 0);


        //Third : for port ACQ_COILINFO_PORT_E

        if (l_curprot_cdsinfo->m_locationtbl[ACQ_COILINFO_PORT_E].coilID != 0x0) {
            sprintf(buf, "%04x", l_curprot_cdsinfo->m_locationtbl[ACQ_COILINFO_PORT_E].coilID);
            f_compass_debug_param[ACQ_COILINFO_PORT_E].m_coilid = buf ;
        }

        f_compass_debug_param[ACQ_COILINFO_PORT_E].m_location       = l_curprot_cdsinfo->m_locationtbl[ACQ_COILINFO_PORT_E].location ;
        f_compass_debug_param[ACQ_COILINFO_PORT_E].m_detectedflag   = (l_curprot_cdsinfo->m_locationtbl[ACQ_COILINFO_PORT_E].detectedFlag > 0);
    }
}

//************************************Method Header************************************
// Method Name  : GetDebugProtocol
// Author       : PATNI/ HEMANT
// Purpose      : This will return the CPqmProtocol object corresponding the CDS debug
//                and is for acqorder stored in m_currentCDSdebugprotocol.
//***********************************************************************************
CPqmProtocol* CpqmCDSDebug::GetDebugProtocol(
)  const   //Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_03
{
    LPCTSTR FUNC_NAME = _T("CpqmCDSDebug::GetDebugProtocol");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (!m_pqm) {
        return NULL;
    }

    const int l_current_prot = GetCurrentCDSDebugProtocol();

    CPQMStudy* l_study = m_pqm->GetStudy();

    if (!l_study) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_study is NULL"));
        ASSERT(FALSE) ;
        return NULL;
    }

    CPqmProtocol* l_current_protocol = l_study->GetProtocol(l_current_prot);

    if (!l_current_protocol) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_current_protocol is NULL"));
        ASSERT(FALSE) ;
        return NULL;
    }

    return l_current_protocol ;
}

//************************************Method Header************************************
// Method Name  : GetCurrentCDSDebugProtocol
// Author       : PATNI/ HEMANT
// Purpose      : returns curent protocol acq order
//***********************************************************************************
int CpqmCDSDebug::GetCurrentCDSDebugProtocol(
) const  //Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_03
{
    return m_currentCDSdebugprotocol ;
}

//************************************Method Header************************************
// Method Name  : SetCurrentCDSDebugProtocol
// Author       : PATNI/ HEMANT
// Purpose      : set the curent protocol acq order
//***********************************************************************************
void CpqmCDSDebug::SetCurrentCDSDebugProtocol(
    const int f_debug_protocol
)
{
    m_currentCDSdebugprotocol = f_debug_protocol ;
}

//************************************Method Header************************************
// Method Name  : GetCDSDebugFlag
// Author       : PATNI/ HEMANT
// Purpose      : Returns true if CDS debug is ON, else false
//***********************************************************************************
bool CpqmCDSDebug::GetCDSDebugFlag(
) const  	//Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_03
{
    return m_cdsdebug_flag ;
}

//************************************Method Header************************************
// Method Name  : SetCDSDebugFlag
// Author       : PATNI/ HEMANT
// Purpose      : Sets the CDS debug On/OFF
//***********************************************************************************
void CpqmCDSDebug::SetCDSDebugFlag(
    const bool f_cdsdebug_flag
)
{
    m_cdsdebug_flag = f_cdsdebug_flag;
}

//************************************Method Header************************************
// Method Name  : SetPQM
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CpqmCDSDebug::SetPQM(
    CPqm* f_pqm
)
{
    m_pqm = f_pqm ;
}

//************************************Method Header************************************
// Method Name  : pqmUiDebugCancelCdsCB
// Author       : PATNI/ HEMANT
// Purpose      : This function is called when user pressed Cancel in CDS debug window.
//                f_compass_debug_param represents the dialog edited parameters.
//***********************************************************************************
void CpqmCDSDebug::pqmUiDebugCancelCdsCB(
    COMPASS_Debugg_Param* const f_compass_debug_param
)
{
    LPCTSTR FUNC_NAME = _T("CpqmCDSDebug::pqmUiDebugCancelCdsCB");

    //Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(f_compass_debug_param);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    //This will be called from the caller
    //PqmAcqAutoCoilSet(PqmGlobal.pqmStudy.vfHandle, prObj);
}


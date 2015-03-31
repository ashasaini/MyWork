//REDMINE-1213_Update_03_MAY
//REDMINE-1213
//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-780
//Redmine-781
// pqmCDS.cpp: implementation of the CpqmCDS class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pqmCDS.h"
extern "C" {
#include <tami/libAcq/AcqCDS.h>
}
#include <tami/libAcq/AcqError.h>
#include <tami/acqman/AM_ClientMessages.h>
#include <tami/vfStudy/vf_appcodes.h>
//Patni-Rajendra/2011Mar29/Modified/TMSC-REDMINE-773
#include <libStudyManager/Log/ScopedFuncInOutLog.h>
#include <PQM/resource.h>
#include <PQM/PQMLogMgr.h>
#include "pqm.h"
#include "pqmpmacqman.h"
#include "PqmPrMode.h"
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
//#include "CDSConstants.h"
#include "CDSUtilities.h"
#include "PqmCoilData.h"
#include "PqmSectionData.h"
#include "ScanProcedure.h"
#include "PQMDebugInfo.h"
#include "VarFieldHandle.h" //TMSC-Tanoue/2010Aug25/MVC008345


#define	LOCATION_MASK	0x000F
#define	LOCATION_UPPER	0x0001
#define	LOCATION_LOWER	0x0002

#define PQM_NUM_COIL_ELEMENT	16


typedef struct porttbl_s {
    int		no;
    char	c[3];
} porttbl_t;

static porttbl_t g_porttbl_object[]	= {
    {ACQ_COILINFO_PORT_A,  ACQ_COIL_PORT_A  },
    {ACQ_COILINFO_PORT_B,  ACQ_COIL_PORT_B  },
    {ACQ_COILINFO_PORT_C,  ACQ_COIL_PORT_C  },
    {ACQ_COILINFO_PORT_D,  ACQ_COIL_PORT_D1 },		/* D1 on L2,Saturn */
    {ACQ_COILINFO_PORT_E,  ACQ_COIL_PORT_D2 },		/* D1 on L2,Saturn */
    {ACQ_COILINFO_PORT_F,  ACQ_COIL_PORT_F  },
    {ACQ_COILINFO_PORT_G1, ACQ_COIL_PORT_G1 },		/* set for Saturn only */
    {ACQ_COILINFO_PORT_G2, ACQ_COIL_PORT_G2 },		/* set for Saturn only */
    {ACQ_COILINFO_PORT_G3, ACQ_COIL_PORT_G3 },		/* set for Saturn only */
    {ACQ_COILINFO_PORT_G4, ACQ_COIL_PORT_G4 },		/* set for Saturn only */
    {ACQ_COILINFO_PORT_G5, ACQ_COIL_PORT_G5 },		/* set for Saturn only */
    {ACQ_COILINFO_PORT_G6, ACQ_COIL_PORT_G6 },		/* set for Saturn only */
    {ACQ_COILINFO_PORT_G7, ACQ_COIL_PORT_G7 }
};	/* set for Saturn only */


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//************************************Method Header************************************
// Method Name  : CpqmCDS
// Author       : PATNI/ HEMANT
// Purpose      : Constrcutor
//***********************************************************************************
CpqmCDS::CpqmCDS(): m_pqm(NULL), m_cds_CheckCoil(CDSCheckCoil_None)
{
    SetCDSCoilSelStatus(CDS_NONE);
}

//************************************Method Header************************************
// Method Name  : ~CpqmCDS
// Author       : PATNI/ HEMANT
// Purpose      : Destrcutor
//***********************************************************************************
CpqmCDS::~CpqmCDS()
{
    //No need to delete..it is shared
    m_pqm = NULL ;
}

//************************************Method Header************************************
// Method Name  : PqmCdsInit
// Author       : PATNI/ HEMANT
// Purpose      : CDS load initialization parameter file
//***********************************************************************************
bool CpqmCDS::PqmCdsInit(
    CPqm* f_pqm
)
{
    LPCTSTR FUNC_NAME = _T("CpqmCDS::PqmCdsInit");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    m_pqm = f_pqm ;
    ASSERT(m_pqm);

    m_cds_debug.SetPQM(m_pqm);

    AcqCDS_Param_t l_cds_param;

    //Library function call for CDS - libAcq
    if (AcqCDS_readCDSParam(&l_cds_param) == ACQ_ERROR_NONE) {
        m_cds_param.m_moveCouchLimit		= l_cds_param.moveCouchLimit;
        m_cds_param.m_coilOverLapLimit	= l_cds_param.coilOverLapLimit;
        m_cds_param.m_couchLabelSize		= l_cds_param.couchLabelSize;
        m_cds_param.m_couchEndMargin		= l_cds_param.couchEndMargin;

        //+Patni-Hemant/2010Apr14/Added/ACE-Phase#3/CDS Requirement
        //According to Tanoue-san comment, check for Venus system first
        if (GP_SYS_ISVENUS((m_pqm->GetSysType()))) {
            /* Venus */
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Venus System"));

            m_cds_param.m_couchToProjDis	= l_cds_param.couchToProjDistVenus;
            m_cds_param.m_couchRangeIn	= l_cds_param.couchRangeInVenus;
            m_cds_param.m_couchRangeOut	= l_cds_param.couchRangeOutVenus;

        } else

            //-Patni-Hemant/2010Apr14/Added/ACE-Phase#3/CDS Requirement
            if (GP_SYS_ISTITAN(m_pqm->GetSysType())) {
                /* Titan */
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Titan System"));

                m_cds_param.m_couchToProjDis	= l_cds_param.couchToProjDistTitan;
                m_cds_param.m_couchRangeIn	= l_cds_param.couchRangeInTitan;
                m_cds_param.m_couchRangeOut	= l_cds_param.couchRangeOutTitan;

            } else {
                /* Atlas */
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Atlas System"));

                m_cds_param.m_couchToProjDis	= l_cds_param.couchToProjDistAtlas;
                m_cds_param.m_couchRangeIn	= l_cds_param.couchRangeInAtlas;
                m_cds_param.m_couchRangeOut	= l_cds_param.couchRangeOutAtlas;
            }

    } else {
        /* Error when the default value (or less) used */

        m_cds_param.m_moveCouchLimit =  100;
        m_cds_param.m_couchToProjDis		=   95;
        m_cds_param.m_couchLabelSize		=  127;
        m_cds_param.m_couchEndMargin		=  111;
        m_cds_param.m_couchRangeIn		= -100;
        m_cds_param.m_couchRangeOut		= 2500;
        m_cds_param.m_coilOverLapLimit	=  127;
    }


    CString l_log_str = _T("");
    l_log_str.Format(_T("CDS Param : moveCouchLimit   = %d "), m_cds_param.m_moveCouchLimit);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);

    l_log_str.Format(_T("CDS Param : couchToProjDis   = %d "), m_cds_param.m_couchToProjDis);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);

    l_log_str.Format(_T("CDS Param : couchLabelSize   = %d "), m_cds_param.m_couchLabelSize);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);

    l_log_str.Format(_T("CDS Param : couchEndMargin   = %d "), m_cds_param.m_couchEndMargin);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);

    l_log_str.Format(_T("CDS Param : couchRangeIn     = %d "), m_cds_param.m_couchRangeIn);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);

    l_log_str.Format(_T("CDS Param : couchRangeOut    = %d "), m_cds_param.m_couchRangeOut);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);

    l_log_str.Format(_T("CDS Param : coilOverLapLimit = %d "), m_cds_param.m_coilOverLapLimit);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);

    return true ;
}

//************************************Method Header************************************
// Method Name  : PqmCdsSetAutoIn
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CpqmCDS::PqmCdsSetAutoIn(
    const float f_dist
)
{
    m_cds_param.m_autoIn = (int)f_dist ;
}

//************************************Method Header************************************
// Method Name  : PqmCdsCheckCoil
// Author       : PATNI/ HEMANT
// Purpose      : The coils are set to check the parameter list CDS coil
//And, CDS return and the presence of auto-configuration. When this coil WholeBody
//Except, Combine 0x0007 when the coil is not included and CDS,
//Set is not automatic.
//Function returns followings :
//PQM_CDS_NONE			0x0000		not automatically set
//PQM_CDS_SETTING		0x0001		Only perform automatic configuration
//PQM_CDS_SCAN			0x0002		CDS pre-scan performed after autoconfiguration
//***********************************************************************************
int CpqmCDS::PqmCdsCheckCoil(
    VftCoilListInfo_t* const f_coillist
)const
{
    LPCTSTR FUNC_NAME = _T("CpqmCDS::PqmCdsCheckCoil");

    int l_cds = PQM_CDS_NONE;

    PQM_SCOPED_TRACE();

    if (NULL == f_coillist) {

        ASSERT(FALSE) ;
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_coillist is NULL"));
        return l_cds ;
    }

    CPqmCoilData* l_coil_data = CDataManager::GetInstance()->GetPQMCoilData();

    if (l_coil_data == NULL) {

        ASSERT(FALSE) ;
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetPQMCoilData() function returned NULL"));
        return l_cds ;
    }


    CCoil l_coil ;

    l_cds = 0;

    PqmCoil_t* l_coil_p = NULL ;

    for (int l_coil_counter = 0; l_coil_counter < f_coillist->num; l_coil_counter++) {

        //l_coil_p = PqmCoilidGetSaturnCoil(f_coillist->coillist[l_coil_counter].coilID, f_coillist->coillist[l_coil_counter].portID[0], NULL);
        l_coil_data->PqmCoilidGetSaturnCoil(
            f_coillist->coillist[l_coil_counter].coilID,
            f_coillist->coillist[l_coil_counter].portID[0],
            NULL,
            l_coil);

        l_coil_p = l_coil.GetCoil() ;

        if (l_coil_p == NULL) {
            continue;
        }

        if (l_coil_p->combine != 0xFFF5 && l_coil_p->combine != 0x0007) {

            return PQM_CDS_NONE;
        }

        l_cds |= l_coil_p->coilDetect;

    }

    return l_cds;
}

//************************************Method Header************************************
// Method Name  : PqmCdsSetListCoil
// Author       : PATNI/ HEMANT
// Purpose      : coil that is connected (coillist) that automatically sets the
//                CdsInfo.coordinatetbl coil to keep the store ID.
//***********************************************************************************
bool CpqmCDS::PqmCdsSetListCoil(
    CPqmProtocol* const f_pqmprotocol
)const
{
    LPCTSTR FUNC_NAME = _T("CpqmCDS::PqmCdsSetListCoil");
    PQM_SCOPED_TRACE();

    if (NULL == f_pqmprotocol) {

        ASSERT(FALSE) ;
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_pqmprotocol is NULL "));
        return false;
    }

    CPqmCoilData* l_coil_data = CDataManager::GetInstance()->GetPQMCoilData();

    if (l_coil_data == NULL) {

        ASSERT(FALSE) ;
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetPQMCoilData() function returned NULL"));
        return false;
    }

    PqmSaturnCoil_t* l_saturn_coil = f_pqmprotocol->GetSaturnCoil();

    if (l_saturn_coil == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Saturn Coil is not Present in Protocl"));
        return false;
    }


    //l_coillist = &prObj->saturnCoil.CoilList;	/*Information of the coil is connected */
    VftCoilListInfo_t* l_coillist = &l_saturn_coil->CoilList;	/*Information of the coil is connected */

    CCoil l_coil ;
    CpqmCDSInfo* l_pqmcdsinfo = f_pqmprotocol->GetCDSInfo();

    //+Patni-Hemant/2010Apr23/Added/ACE-Phase#3/CDS Requirement/Defect Fix - PQM/Vantage Crash
    //+TMSC-Tanoue/2010Sep03/MVC008324
    memset(l_pqmcdsinfo->m_coordinatetbl, 0, sizeof(l_pqmcdsinfo->m_coordinatetbl));
    memset(l_pqmcdsinfo->m_coil_tbl, 0, sizeof(l_pqmcdsinfo->m_coil_tbl));
    //-TMSC-Tanoue/2010Sep03/MVC008324
    //-Patni-Hemant/2010Apr23/Added/ACE-Phase#3/CDS Requirement/Defect Fix - PQM/Vantage Crash

    PqmCoil_t* l_coil_p = NULL;
    int l_port_counter = 0;

    for (int l_coil_counter = 0; l_coil_counter < l_coillist->num; l_coil_counter++) {
        //coil_p = PqmCoilidGetSaturnCoil(l_coillist->coillist[i].coilID, l_coillist->coillist[i].portID[0], l_coillist->coillist[i].channel);
        l_coil_data->PqmCoilidGetSaturnCoil(
            l_coillist->coillist[l_coil_counter].coilID,
            l_coillist->coillist[l_coil_counter].portID[0],
            l_coillist->coillist[l_coil_counter].channel,
            l_coil);

        l_coil_p = l_coil.GetCoil() ;


        if (l_coil_p == NULL) {
            continue;
        }

        if (l_coil_p->coilDetect & VFC_CDS_STATUS_AUTO_SELECT) {
            /*Set automatic coil*/

            for (l_port_counter = ACQ_COILINFO_PORT_A; l_port_counter < PQM_NUM_COIL_PORT; l_port_counter++) {
                if (memcmp(l_coillist->coillist[l_coil_counter].portID, g_porttbl_object[l_port_counter].c, sizeof(char) * 2) == 0) {
                    const int l_post_num = g_porttbl_object[l_port_counter].no ;
                    l_pqmcdsinfo->m_coil_tbl[l_post_num] = l_coil_p;
                    l_pqmcdsinfo->m_coordinatetbl[l_post_num].coilID   = l_coil_p->coil_id & COIL_PHYSICALID_MASK;
                }
            }
        }

    }

    return true;
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
void CpqmCDS::pqmUiDebugSetCdsCB(
    COMPASS_Debugg_Param f_compass_debug_param[]
)
{
    LPCTSTR FUNC_NAME = _T("CpqmCDS::pqmUiDebugSetCdsCB");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME , _T("Trace"));

    m_cds_debug.pqmUiDebugSetCdsCB(f_compass_debug_param);

    OnCDSDebugCompleted();
}



//************************************Method Header************************************
// Method Name  : pqmUiDebugCancelCdsCB
// Author       : PATNI/ HEMANT
// Purpose      : This function is called when user pressed Cancel in CDS debug window.
//                f_compass_debug_param represents the dialog edited parameters.
//***********************************************************************************
void CpqmCDS::pqmUiDebugCancelCdsCB(
    COMPASS_Debugg_Param f_compass_debug_param[]
)
{
    LPCTSTR FUNC_NAME = _T("CpqmCDS::pqmUiDebugCancelCdsCB");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    //This will do nothing.. so commented
    //m_cds_debug.pqmUiDebugCancelCdsCB(f_compass_debug_param);

    OnCDSDebugCompleted();
}

//************************************Method Header************************************
// Method Name  : OnOffCDSDebug
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CpqmCDS::OnOffCDSDebug(
    const BOOL f_OnOff
)
{
    m_cds_debug.SetCDSDebugFlag(f_OnOff > 0);
}

//************************************Method Header************************************
// Method Name  : PqmCdsReturnCoil
// Author       : PATNI/ HEMANT
// Purpose      : CDS for the WB was set to register before coil information
//                To save the information coil ,Returns the back up coil in given argument
//***********************************************************************************
void CpqmCDS::PqmCdsReturnCoil(
    PqmSaturnCoil_t* coil
)
{
    LPCTSTR FUNC_NAME = _T("CpqmCDS::PqmCdsReturnCoil");
    PQM_SCOPED_TRACE();

    memcpy(coil, &m_cdsbackup_coil, sizeof(PqmSaturnCoil_t));
}


//************************************Method Header************************************
// Method Name  : PqmCdsBackUpCoil
// Author       : PATNI/ HEMANT
// Purpose      : CDS for the WB was set to register before coil information
//                To save the information coil.
//***********************************************************************************
void CpqmCDS::PqmCdsBackUpCoil(
    PqmSaturnCoil_t* f_coil
)
{
    LPCTSTR FUNC_NAME = _T("CpqmCDS::PqmCdsBackUpCoil");
    PQM_SCOPED_TRACE();

    memcpy(&m_cdsbackup_coil, f_coil, sizeof(PqmSaturnCoil_t));
}

//************************************Method Header************************************
// Method Name  : PqmCdsChangeCouch
// Author       : PATNI/ HEMANT
// Purpose      : Start location sleeper berth - to convert the distance
//                between the centers of the magnetic field
//***********************************************************************************
int CpqmCDS::PqmCdsChangeCouch(
    const float f_couch
) const  	//Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_03
{
    return ((int)f_couch * -1) - m_cds_param.m_autoIn - m_cds_param.m_couchToProjDis;
}

//************************************Method Header************************************
// Method Name  : PqmCdsSetCoordinatetbl
// Author       : PATNI/ HEMANT
// Purpose      : Acqman calculated coil location (relative to the magnetic field center)
//                are converted to the cordinate relative to couch tip
//This value contains the unadjusted value, when used in error checking
//***********************************************************************************
void CpqmCDS::PqmCdsSetCoordinatetbl(
    Vft_CdsCoilLocation_t* const f_locationtbl,
    int32_t* f_coordinate,
    const float f_couch
)const
{
    LPCTSTR FUNC_NAME = _T("CpqmCDS::PqmCdsSetCoordinatetbl");

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    for (int l_port_counter = ACQ_COILINFO_PORT_A; l_port_counter < PQM_NUM_COIL_PORT; l_port_counter++) {
        if (f_locationtbl[l_port_counter].detectedFlag) {
            /* Calculated from measurements only valid */
            f_coordinate[l_port_counter] =
                pqmCdsChangeCoordinate(f_locationtbl[l_port_counter].location, f_couch);
        }
    }
}

//************************************Method Header************************************
// Method Name  : PqmCdsAdjustCoilPosition
// Author       : PATNI/ HEMANT
// Purpose      : Center coordinates overlapping coils seeking to adjust the position of the coil
//***********************************************************************************
void CpqmCDS::PqmCdsAdjustCoilPosition(
    CPqmProtocol* const f_curObj
)
{
    LPCTSTR FUNC_NAME = _T("CpqmCDS::PqmCdsAdjustCoilPosition");
    PQM_SCOPED_TRACE();

    if (!f_curObj) {
        return ;
    }

    CpqmCDSInfo* l_cdsInfo = NULL ;
    l_cdsInfo = f_curObj->GetCDSInfo();

    CPqmCoilData* l_coil_data = CDataManager::GetInstance()->GetPQMCoilData();

    if (l_coil_data == NULL) {

        ASSERT(FALSE) ;
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetPQMCoilData() function returned NULL"));
        return  ;
    }

    int				i = 0, j = 0, k = 0, u = 0, l = 0;
    int				p = 0;
    int				l_group[4] = { -1, -1, -1, -1};
    int				l_left = 0,  l_right = 0;

    //PqmcdsInfo_p	cdsInfo;
    PqmCoil_t*		coil_p = NULL;
    VftSectionRecvrCoil_t	recvrCoil;
    sedb_rec_t* l_sec_p[PQM_NUM_COIL_ELEMENT] = {0};						/*Section for information retrieval*/
    int l_sec_cnt = 0;											/* Section Number Information */

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Preparing Cordinate table"));

    int l_center = 0;

    /* acqman to calculate the center coordinates of the location-based measure (for measuring coil is not confirmed) */
    for (i = ACQ_COILINFO_PORT_A; i < PQM_NUM_COIL_PORT; i++) {
        if ((l_cdsInfo->m_coordinatetbl[i].coilID  != NULL)	&&
            !l_cdsInfo->m_coordinatetbl[i].detectedFlag) {
            /* Coil position is not confirmed */
            if (l_cdsInfo->m_coil_tbl[i]->coilDetect == VFC_CDS_STATUS_AUTO_SELECT) {
                /* Auto-only coil (now Head only)*/
                l_cdsInfo->m_coordinatetbl[i].location =
                    (l_cdsInfo->m_coil_tbl[i]->size) / 2;
                /* Determine the highest priority because*/
                l_cdsInfo->m_coordinatetbl[i].detectedFlag = TRUE;

            } else if (l_cdsInfo->m_locationtbl[i].detectedFlag) {
                /* Scan this was measured at - "to calculate the center coordinates */
                l_cdsInfo->m_org_coordinatetbl[i] =
                    pqmCdsChangeCoordinate(l_cdsInfo->m_locationtbl[i].location, f_curObj->GetPqmPrMode()->GetCouchPos());
                l_cdsInfo->m_coordinatetbl[i].location = l_cdsInfo->m_org_coordinatetbl[i];
                l_cdsInfo->m_used_locationtbl[i] = l_cdsInfo->m_locationtbl[i].location;
                l_cdsInfo->m_coordinatetbl[i].detectedFlag = FALSE;
                /* So if you adjust the position and the other coil overlapping Flag can not stand still */

                if (l_cdsInfo->m_coil_tbl[i]->coilLocation == 0) {
                    /* Coil does not consider the overlap (linear), so this case will set a Flag*/
                    l_cdsInfo->m_coordinatetbl[i].detectedFlag = TRUE;
                }

                if (l_cdsInfo->m_coil_tbl[i]->groupId[0] != 0xffff) {
                    /*Coil group*/
                    l_center = l_cdsInfo->m_coordinatetbl[i].location;

                    for (j = 0; j < 4; j++) {
                        /* Extract the coil group */
                        if (l_cdsInfo->m_coil_tbl[i]->groupId[j] == 0xffff) {
                            break;
                        }

                        for (k = ACQ_COILINFO_PORT_A; k < PQM_NUM_COIL_PORT; k++) {
                            if (l_cdsInfo->m_coil_tbl[i]->groupId[j] == l_cdsInfo->m_coordinatetbl[k].coilID) {
                                l_group[j] = k;
                            }
                        }
                    }
                }
            }
        }
    }

    for (i = ACQ_COILINFO_PORT_A; i < PQM_NUM_COIL_PORT; i++) {
        if ((l_cdsInfo->m_coordinatetbl[i].coilID  == NULL)	||
            (l_cdsInfo->m_coil_tbl[i]->groupId[0]  == 0xffff)) {
            continue;
        }

        /* Coil group */
        for (j = 0; j < 4; j++) {
            /* Extract the coil group */
            if (l_cdsInfo->m_coil_tbl[i]->groupId[j] == 0xffff) {
                break;
            }

            for (k = ACQ_COILINFO_PORT_A; k < PQM_NUM_COIL_PORT; k++) {
                if (l_cdsInfo->m_coil_tbl[i]->groupId[j] == l_cdsInfo->m_coordinatetbl[k].coilID) {
                    l_group[j] = k;
                }
            }
        }

        p = -1;

        for (j = 0; j < 3; j++) {
            if (l_group[j] == -1) {
                break;
            }

            if (l_cdsInfo->m_locationtbl[l_group[j]].detectedFlag) {
                if ((p == -1)	||
                    (ABSF(l_cdsInfo->m_locationtbl[l_group[j]].location) < ABSF(l_cdsInfo->m_locationtbl[l_group[p]].location))) {
                    p = j;
                }
            }
        }

        if (p == -1) {
            continue;
        }

        for (j = p - 1; j >= 0; j--) {
            l_cdsInfo->m_org_coordinatetbl[l_group[j]] =
                l_cdsInfo->m_coordinatetbl[l_group[j + 1]] .location
                - l_cdsInfo->m_coil_tbl[l_group[j + 1]]->size / 2
                - l_cdsInfo->m_coil_tbl[l_group[j]]->size / 2;
            l_cdsInfo->m_coordinatetbl[l_group[j]].location = l_cdsInfo->m_org_coordinatetbl[l_group[j]];
            l_cdsInfo->m_locationtbl[l_group[j]].detectedFlag = TRUE;
            l_cdsInfo->m_used_locationtbl[l_group[j]] = 9999;

        }

        for (j = p + 1; j < 4; j++) {
            if (l_group[j] == -1)	break;

            l_cdsInfo->m_org_coordinatetbl[l_group[j]] =
                l_cdsInfo->m_coordinatetbl[l_group[j - 1]].location
                + l_cdsInfo->m_coil_tbl[l_group[j - 1]]->size / 2
                + l_cdsInfo->m_coil_tbl[l_group[j]]->size / 2;
            l_cdsInfo->m_coordinatetbl[l_group[j]].location = l_cdsInfo->m_org_coordinatetbl[l_group[j]];
            l_cdsInfo->m_locationtbl[l_group[j]].detectedFlag = TRUE;
            l_cdsInfo->m_used_locationtbl[l_group[j]] = 9999;
        }
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Checking for overlapping coils"));

    //+Patni-Hemant/2010Aug02/Added/TMSC-REDMINE-520/CDS Abort Issue
    CString l_message_str(_T(""));

    l_message_str.Format(_T("Coil detection data initialized..."));
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message_str);

    for (i = ACQ_COILINFO_PORT_A; i < PQM_NUM_COIL_PORT; i++) {

        l_message_str.Format(_T("Coil id : %x, detectedFlag : %d, location : %d"),
                             l_cdsInfo->m_coordinatetbl[i].coilID,
                             l_cdsInfo->m_coordinatetbl[i].detectedFlag,
                             l_cdsInfo->m_coordinatetbl[i].location
                            );
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message_str);
    }

    //-Patni-Hemant/2010Aug02/Added/TMSC-REDMINE-520/CDS Abort Issue

    int32_t l_distL = 0, l_distR = 0;

    /*For overlapping coils and coils that are already confirmed to adjust the position (to be confirmed) */
    for (i = ACQ_COILINFO_PORT_A; i < PQM_NUM_COIL_PORT; i++) {
        if (!l_cdsInfo->m_coordinatetbl[i].detectedFlag || l_cdsInfo->m_coil_tbl[i] == NULL) {
            continue;
        }

        if (l_cdsInfo->m_coil_tbl[i]->coilLocation == 0) {
            continue;
        }


        //+Patni-Hemant/2010Aug02/Added/TMSC-REDMINE-520/CDS Abort Issue
        l_message_str.Format(_T("----Checking overlapping(Step-1) for port no:%d -----"), i);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message_str);
        //-Patni-Hemant/2010Aug02/Added/TMSC-REDMINE-520/CDS Abort Issue

        pqmCdsSearchOverlapCoil(l_cdsInfo, i, &l_left, &l_distL, &l_right, &l_distR);

        //+Patni-Hemant/2010Aug02/Added/TMSC-REDMINE-520/CDS Abort Issue
        l_message_str.Format(_T("Checking overlapping(Step-1) for port no:%d Completed"), i);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message_str);

        l_message_str.Format(_T("l_left = %d, l_distL = %d, l_right = %d, l_distR = %d"), l_left, l_distL, l_right, l_distR);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message_str);
        //-Patni-Hemant/2010Aug02/Added/TMSC-REDMINE-520/CDS Abort Issue

        if ((l_distL) < 0 && (l_distR < 0)) {
            /* Coil ends overlap in*/
            /*3 adjustable coil do I keep this up so */
            l_cdsInfo->m_coordinatetbl[l_left].location  = l_cdsInfo->m_coordinatetbl[l_left].location  + l_distL;
            l_cdsInfo->m_coordinatetbl[l_right].location = l_cdsInfo->m_coordinatetbl[l_right].location - l_distR;

        } else if ((l_distL < 0) || (l_distR < 0)) {
            /* Coil overlap with one*/
            if (l_distL < 0) {	/* Coil overlap in the left */
                p = l_left;
            }

            if (l_distR < 0) {	/*Coil overlap in right*/
                p = l_right;
            }


            //+Patni-Hemant/2010Aug02/Added/TMSC-REDMINE-520/CDS Abort Issue
            l_message_str.Format(_T("----Checking overlapping(Step-2) for port no :%d -----"), p);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message_str);
            //-Patni-Hemant/2010Aug02/Added/TMSC-REDMINE-520/CDS Abort Issue

            /* Determine the status of both ends of the coil to coil positioning */
            pqmCdsSearchOverlapCoil(l_cdsInfo, p, &l_left, &l_distL, &l_right, &l_distR);

            //+Patni-Hemant/2010Aug02/Added/TMSC-REDMINE-520/CDS Abort Issue
            l_message_str.Format(_T("Checking overlapping(Step-2) for port no :%d Completed"), p);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message_str);

            l_message_str.Format(_T("l_left = %d, l_distL = %d, l_right = %d, l_distR = %d"), l_left, l_distL, l_right, l_distR);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message_str);
            //-Patni-Hemant/2010Aug02/Added/TMSC-REDMINE-520/CDS Abort Issue


            if (((l_distL + l_distR) < 0)					&&
                (l_cdsInfo->m_coordinatetbl[l_left].detectedFlag)	&&
                (l_cdsInfo->m_coordinatetbl[l_right].detectedFlag)) {
                /*Confirmed that there is a coil located at both ends, not regulation */
                if ((l_cdsInfo->m_coil_tbl[l_left]->coilDetect == VFC_CDS_STATUS_AUTO_SELECT)	||
                    (l_cdsInfo->m_used_locationtbl[l_left] <= l_cdsInfo->m_used_locationtbl[l_right])) {
                    /* Coil with the right to adjust the right*/
                    l_cdsInfo->m_coordinatetbl[p].location     = l_cdsInfo->m_coordinatetbl[p].location - l_distL;
                    l_cdsInfo->m_coordinatetbl[l_right].location = l_cdsInfo->m_coordinatetbl[l_right].location - l_distL - l_distR;

                } else if ((l_cdsInfo->m_coil_tbl[l_right]->coilDetect == VFC_CDS_STATUS_AUTO_SELECT)	||
                           (l_cdsInfo->m_used_locationtbl[l_left] > l_cdsInfo->m_used_locationtbl[l_right])) {
                    /*Adjust the coil on the left with the left */
                    l_cdsInfo->m_coordinatetbl[p].location     = l_cdsInfo->m_coordinatetbl[p].location + l_distR;
                    l_cdsInfo->m_coordinatetbl[l_left].location  = l_cdsInfo->m_coordinatetbl[l_left].location + l_distR + l_distL;
                }

            } else if ((l_distR >= 0)	||
                       (l_cdsInfo->m_coordinatetbl[l_left].detectedFlag)) {
                /*Minutes in length that overlap on the left, right control*/
                l_cdsInfo->m_coordinatetbl[p].location = l_cdsInfo->m_coordinatetbl[p].location - l_distL;

            } else if ((l_distL >= 0)	||
                       (l_cdsInfo->m_coordinatetbl[l_right].detectedFlag)) {
                /* Minutes in length that overlap on the right, adjust the left */
                l_cdsInfo->m_coordinatetbl[p].location = l_cdsInfo->m_coordinatetbl[p].location + l_distR;
            }
        }

    }

    u = 0;
    l = 0;

    int				l_upper[3] = { -1, -1, -1};
    int				l_lower[3] = { -1, -1, -1};

    /* Coil position is not fixed to the upper index, divided into lower extraction */
    for (i = ACQ_COILINFO_PORT_A; i < PQM_NUM_COIL_PORT; i++) {
        if (l_cdsInfo->m_coordinatetbl[i].detectedFlag || l_cdsInfo->m_coil_tbl[i] == NULL) {
            continue;
        }

        if (!l_cdsInfo->m_locationtbl[i].detectedFlag) {
            continue;
        }

        if (l_cdsInfo->m_coil_tbl[i]->coilLocation & LOCATION_UPPER) {
            l_upper[u++] = i;

        } else if (LOCATION_LOWER) {
            l_lower[l++] = i;
        }

        if (u >= 3 || l >= 3) {
            break;	/* Impossible because it violates the constraints of three up*/
        }
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Sort the coils for priority"));

    /* Sorted by priority */
    pqmCdsSort(l_upper, l_cdsInfo->m_used_locationtbl, 3);
    pqmCdsSort(l_lower, l_cdsInfo->m_used_locationtbl, 3);

    /*Coils will have to confirm the position of high priority */
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 2; j++) {
            if (j == 0) {
                p = l_upper[i];

            } else {
                p = l_lower[i];
            }

            if (p == -1) {
                continue;
            }

            pqmCdsSearchOverlapCoil(l_cdsInfo, p, &l_left, &l_distL, &l_right, &l_distR);

            /* The coils are stacked on the left & & are left to determine the position of the coil */
            if ((l_distL < 0)	&&	(l_cdsInfo->m_coordinatetbl[l_left].detectedFlag)) {
                /*Shifted to the right */
                l_cdsInfo->m_coordinatetbl[p].location = l_cdsInfo->m_coordinatetbl[p].location - l_distL;
            }

            /*Coils are stacked on the right & & has the right to determine the position of the coil*/
            else if ((l_distR < 0)	&&	(l_cdsInfo->m_coordinatetbl[l_right].detectedFlag)) {
                /* Off to the left*/
                l_cdsInfo->m_coordinatetbl[p].location = l_cdsInfo->m_coordinatetbl[p].location + l_distR;
            }

            /* Coils are stacked on the left & & no firm position of the coil on the left */
            else if ((l_distL < 0)	&&	(!l_cdsInfo->m_coordinatetbl[l_left].detectedFlag)) {
                /* A good ?Kasanaku priority so high, it is likely that further confirmed the left*/
                pqmCdsSearchOverlapCoil(l_cdsInfo, l_left, &l_left, &l_distL, &l_right, &l_distR);

                if (l_cdsInfo->m_coordinatetbl[l_left].detectedFlag) {
                    if (l_distL < -l_distR) {
                        /*Coil on the left just off to the right lower priority is not adjusted only minutes*/
                        if (l_distR == 99999999) /*Special case that fits the position of the coil overlap was measured*/
                            l_cdsInfo->m_coordinatetbl[p].location = l_cdsInfo->m_coordinatetbl[p].location + l_distL;

                        else
                            l_cdsInfo->m_coordinatetbl[p].location = l_cdsInfo->m_coordinatetbl[p].location - (l_distR - l_distL);
                    }
                }
            }

            /* Coils are stacked on the right & & do not accept the position of the coil on the right */
            else if ((l_distR < 0)	&&	(!l_cdsInfo->m_coordinatetbl[l_right].detectedFlag)) {
                /* ?Kasanaku so good that a high priority, it is possible that the right is further confirmed */
                pqmCdsSearchOverlapCoil(l_cdsInfo, l_right, &l_left, &l_distL, &l_right, &l_distR);

                if (l_cdsInfo->m_coordinatetbl[l_right].detectedFlag) {
                    if (l_distR < -l_distL) {
                        /* Only low-priority right coil is shifted to the left can not control */
                        l_cdsInfo->m_coordinatetbl[p].location = l_cdsInfo->m_coordinatetbl[p].location + (l_distL + l_distR);
                    }
                }
            }

            l_cdsInfo->m_coordinatetbl[p].detectedFlag = TRUE;
        }
    }



    //+Patni-Hemant/2010Aug02/Added/TMSC-REDMINE-520/CDS Abort Issue
    l_message_str.Format(_T("Displaying Detection Status (Before Error handling)"));
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message_str);

    for (i = ACQ_COILINFO_PORT_A; i < PQM_NUM_COIL_PORT; i++) {

        l_message_str.Format(_T("Coil id : %x, detectedFlag : %d, location : %d"),
                             l_cdsInfo->m_coordinatetbl[i].coilID,
                             l_cdsInfo->m_coordinatetbl[i].detectedFlag,
                             l_cdsInfo->m_coordinatetbl[i].location
                            );
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message_str);
    }

    //-Patni-Hemant/2010Aug02/Added/TMSC-REDMINE-520/CDS Abort Issue


    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Perform error handling"));

    /* Error handling :
    * Or adjust the amount of tolerance for those sleeping outside the scope of the coil position
    * This information detectedFlag to False to disable and then measured
    */
    for (i = ACQ_COILINFO_PORT_A; i < PQM_NUM_COIL_PORT; i++) {
        if (!l_cdsInfo->m_coordinatetbl[i].detectedFlag) {
            continue;
        }

        if (l_cdsInfo->m_coil_tbl[i]->coilDetect == VFC_CDS_STATUS_AUTO_SELECT) {
            continue;	/* Head coil is covered so well on and off the error handling label */
        }

        if (l_cdsInfo->m_coil_tbl[i]->groupId[0] == 0xffff) {
            recvrCoil.coilID		= (uint16_t)l_cdsInfo->m_coordinatetbl[i].coilID;

            if (ACQ_COILINFO_PORT_D <= i && i <= ACQ_COILINFO_PORT_E) {
                recvrCoil.portID[0]	= CDB_PORT_D;

            } else if (ACQ_COILINFO_PORT_G1 <= i && i <= ACQ_COILINFO_PORT_G7) {
                recvrCoil.portID[0]	= CDB_PORT_G;
            }

            recvrCoil.channel	= CDB_CHANNEL_A;
            recvrCoil.SectionID	= 1;

            //PqmCoilGetSaturnSection(&recvrCoil, &l_sec_cnt, l_sec_p);
            if (E_NO_ERROR != l_coil_data->PqmCoilGetSaturnSection(&recvrCoil, &l_sec_cnt, l_sec_p)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PqmCoilGetSaturnSection failed"));
            }

            if (l_sec_cnt >  0) {
                l_left = l_cdsInfo->m_coordinatetbl[i].location + (int)(l_sec_p[0]->dist - l_sec_p[0]->elemXYZ[2] / 2.0);

            } else {
                /* No section (never comes true) */
                l_left  = l_cdsInfo->m_coordinatetbl[i].location - (l_cdsInfo->m_coil_tbl[i]->size / 2);
            }

            if (E_NO_ERROR != l_coil_data->PqmCoilGetSaturnSectionMax(&recvrCoil, &l_sec_cnt)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PqmCoilGetSaturnSectionMax failed"));
            }

            recvrCoil.SectionID	= l_sec_cnt;

            //PqmCoilGetSaturnSection(&recvrCoil, &l_sec_cnt, l_sec_p);
            if (E_NO_ERROR != l_coil_data->PqmCoilGetSaturnSection(&recvrCoil, &l_sec_cnt, l_sec_p)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PqmCoilGetSaturnSection failed"));
            }

            if (l_sec_cnt >  0) {
                l_right = l_cdsInfo->m_coordinatetbl[i].location + (int)(l_sec_p[0]->dist + l_sec_p[0]->elemXYZ[2] / 2.0);

            } else {
                /* No section (never comes true) */
                l_right = l_cdsInfo->m_coordinatetbl[i].location + (l_cdsInfo->m_coil_tbl[i]->size / 2);
            }

        } else {
            /*Coil group */
            l_left = l_cdsInfo->m_coordinatetbl[i].location;

            for (j = 0; j < 4; j++) {
                if (l_cdsInfo->m_coil_tbl[i]->groupId[j] == l_cdsInfo->m_coordinatetbl[i].coilID) {
                    /* When the coil is left half the size of location measurement */
                    recvrCoil.coilID		= (uint16_t)l_cdsInfo->m_coordinatetbl[i].coilID;

                    if (ACQ_COILINFO_PORT_D <= i && i <= ACQ_COILINFO_PORT_E) {
                        recvrCoil.portID[0]	= CDB_PORT_D;

                    } else if (ACQ_COILINFO_PORT_G1 <= i && i <= ACQ_COILINFO_PORT_G7) {
                        recvrCoil.portID[0]	= CDB_PORT_G;
                    }

                    recvrCoil.channel	= CDB_CHANNEL_A;
                    recvrCoil.SectionID	= 1;

                    //PqmCoilGetSaturnSection(&recvrCoil, &l_sec_cnt, l_sec_p);
                    if (E_NO_ERROR != l_coil_data->PqmCoilGetSaturnSection(&recvrCoil, &l_sec_cnt, l_sec_p)) {
                        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PqmCoilGetSaturnSection failed"));
                    }

                    if (l_sec_cnt >  0) {
                        l_left += (int)(l_sec_p[0]->dist - l_sec_p[0]->elemXYZ[2] / 2.0);

                    } else {
                        /* No section (never comes true) */
                        l_left -= l_cdsInfo->m_coil_tbl[i]->size / 2;
                    }

                    break;

                } else {
                    /* Advanced coil on the left just minutes from the size of the measurement coil location is to the left */
                    recvrCoil.coilID		= l_cdsInfo->m_coil_tbl[i]->groupId[j];

                    if (ACQ_COILINFO_PORT_D <= i && i <= ACQ_COILINFO_PORT_E) {
                        recvrCoil.portID[0]	= CDB_PORT_D;

                    } else if (ACQ_COILINFO_PORT_G1 <= i && i <= ACQ_COILINFO_PORT_G7) {
                        recvrCoil.portID[0]	= CDB_PORT_G;
                    }

                    recvrCoil.channel	= CDB_CHANNEL_A;
                    recvrCoil.SectionID	= 1;

                    if (E_NO_ERROR != l_coil_data->PqmCoilGetSaturnSection(&recvrCoil, &l_sec_cnt, l_sec_p)) {
                        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PqmCoilGetSaturnSection failed"));
                    }

                    if (l_sec_cnt >  0) {
                        //coil_p = PqmCoilidGetSaturnCoil(l_cdsInfo->m_coil_tbl[i]->groupId[j],
                        //	(char)l_cdsInfo->m_coil_tbl[i]->port, NULL);

                        CCoil l_coil ;
                        l_coil_data->PqmCoilidGetSaturnCoil(
                            l_cdsInfo->m_coil_tbl[i]->groupId[j],
                            (char)l_cdsInfo->m_coil_tbl[i]->port,
                            NULL,
                            l_coil);
                        coil_p = l_coil.GetCoil() ;

                        if (coil_p)
                            l_left += (int)(l_sec_p[0]->dist - l_sec_p[0]->elemXYZ[2] / 2.0 - coil_p->size / 2.0);

                    } else {
                        /* No section (never comes true) */
                        //coil_p = PqmCoilidGetSaturnCoil(l_cdsInfo->m_coil_tbl[i]->groupId[j],
                        //	(char)l_cdsInfo->m_coil_tbl[i]->port, NULL);

                        CCoil l_coil ;
                        l_coil_data->PqmCoilidGetSaturnCoil(
                            l_cdsInfo->m_coil_tbl[i]->groupId[j],
                            (char)l_cdsInfo->m_coil_tbl[i]->port,
                            NULL,
                            l_coil);

                        coil_p = l_coil.GetCoil() ;

                        if (coil_p)
                            l_left -= coil_p->size;
                    }
                }
            }

            l_right = l_cdsInfo->m_coordinatetbl[i].location;

            for (j = 3; j >= 0; j--) {
                if (l_cdsInfo->m_coil_tbl[i]->groupId[j] == l_cdsInfo->m_coordinatetbl[i].coilID) {
                    /* Measuring the right location at half the size of coil*/
                    recvrCoil.coilID		= (uint16_t)l_cdsInfo->m_coordinatetbl[i].coilID;

                    if (ACQ_COILINFO_PORT_D <= i && i <= ACQ_COILINFO_PORT_E) {
                        recvrCoil.portID[0]	= CDB_PORT_D;

                    } else if (ACQ_COILINFO_PORT_G1 <= i && i <= ACQ_COILINFO_PORT_G7) {
                        recvrCoil.portID[0]	= CDB_PORT_G;
                    }

                    recvrCoil.channel	= CDB_CHANNEL_A;

                    if (E_NO_ERROR != l_coil_data->PqmCoilGetSaturnSectionMax(&recvrCoil, &l_sec_cnt)) {
                        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PqmCoilGetSaturnSectionMax failed"));
                    }

                    recvrCoil.SectionID	= l_sec_cnt;

                    if (E_NO_ERROR != l_coil_data->PqmCoilGetSaturnSection(&recvrCoil, &l_sec_cnt, l_sec_p)) {
                        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PqmCoilGetSaturnSection failed"));
                    }

                    if (l_sec_cnt >  0) {
                        l_right += (int)(l_sec_p[0]->dist + l_sec_p[0]->elemXYZ[2] / 2.0);

                    } else {
                        /* No section (never comes true) */
                        l_right += l_cdsInfo->m_coil_tbl[i]->size / 2;
                    }

                    break;

                } else {
                    /* Only the tip of the minute size of coil to the right location is measured from the coil to the right */
                    recvrCoil.coilID		= l_cdsInfo->m_coil_tbl[i]->groupId[j];

                    if (ACQ_COILINFO_PORT_D <= i && i <= ACQ_COILINFO_PORT_E) {
                        recvrCoil.portID[0]	= CDB_PORT_D;

                    } else if (ACQ_COILINFO_PORT_G1 <= i && i <= ACQ_COILINFO_PORT_G7) {
                        recvrCoil.portID[0]	= CDB_PORT_G;
                    }

                    recvrCoil.channel	= CDB_CHANNEL_A;

                    if (E_NO_ERROR != l_coil_data->PqmCoilGetSaturnSectionMax(&recvrCoil, &l_sec_cnt)) {
                        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PqmCoilGetSaturnSectionMax failed"));
                    }

                    recvrCoil.SectionID	= l_sec_cnt;

                    if (E_NO_ERROR != l_coil_data->PqmCoilGetSaturnSection(&recvrCoil, &l_sec_cnt, l_sec_p)) {
                        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PqmCoilGetSaturnSection failed"));
                    }

                    if (l_sec_cnt >  0) {
                        //coil_p = PqmCoilidGetSaturnCoil(l_cdsInfo->m_coil_tbl[i]->groupId[j],
                        //	(char)l_cdsInfo->m_coil_tbl[i]->port, NULL);

                        CCoil l_coil ;
                        l_coil_data->PqmCoilidGetSaturnCoil(
                            l_cdsInfo->m_coil_tbl[i]->groupId[j],
                            (char)l_cdsInfo->m_coil_tbl[i]->port,
                            NULL,
                            l_coil);

                        coil_p = l_coil.GetCoil() ;

                        if (coil_p)
                            l_right += (int)(l_sec_p[0]->dist + l_sec_p[0]->elemXYZ[2] / 2.0 + coil_p->size / 2.0);

                    } else {
                        /* No section (never comes true)*/
                        //coil_p = PqmCoilidGetSaturnCoil(l_cdsInfo->m_coil_tbl[i]->groupId[j],
                        //	(char)l_cdsInfo->m_coil_tbl[i]->port, NULL);
                        CCoil l_coil ;
                        l_coil_data->PqmCoilidGetSaturnCoil(
                            l_cdsInfo->m_coil_tbl[i]->groupId[j],
                            (char)l_cdsInfo->m_coil_tbl[i]->port,
                            NULL,
                            l_coil);

                        coil_p = l_coil.GetCoil() ;

                        if (coil_p)
                            l_right += coil_p->size;
                    }
                }
            }
        }

        if ((m_cds_param.m_couchRangeIn > l_left) || (l_right > m_cds_param.m_couchRangeOut)) {
            /* Coils located outside the scope of the bed*/
            l_cdsInfo->m_coordinatetbl[i].detectedFlag = FALSE;

            //+Patni-Hemant/2010Aug02/Added/TMSC-REDMINE-520/CDS Abort Issue
            l_message_str = _T("coil is out of couch = coil is not detected");
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message_str);

            l_message_str.Format(_T("coil l_left = %d, coil l_right = %d "), l_left, l_right);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message_str);

            l_message_str.Format(_T("Coil id : %x, detectedFlag : %d, location : %d"),
                                 l_cdsInfo->m_coordinatetbl[i].coilID,
                                 l_cdsInfo->m_coordinatetbl[i].detectedFlag,
                                 l_cdsInfo->m_coordinatetbl[i].location
                                );

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message_str);
            //-Patni-Hemant/2010Aug02/Added/TMSC-REDMINE-520/CDS Abort Issue
        }

        if (l_cdsInfo->m_coil_tbl[i]->coilDetect & VFC_CDS_STATUS_SCAN) {
            if ((l_cdsInfo->m_coil_tbl[i]->groupId[0] == 0xffff)	||
                (l_cdsInfo->m_org_coordinatetbl[i]    != 0)) {
                /* Acqman coils found in */
                int32_t shift = ABSF(l_cdsInfo->m_org_coordinatetbl[i] - l_cdsInfo->m_coordinatetbl[i].location);

                if (shift > m_cds_param.m_coilOverLapLimit) {
                    /*Adjust the amount of tolerance or */
                    l_cdsInfo->m_coordinatetbl[i].detectedFlag = FALSE;


                    //+Patni-Hemant/2010Aug02/Added/TMSC-REDMINE-520/CDS Abort Issue
                    l_message_str = _T("Coil shift is more than coilOverLapLimit = coil is not detected");
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message_str);
                    l_message_str.Format(_T("Coil id : %x, detectedFlag : %d, location : %d"),
                                         l_cdsInfo->m_coordinatetbl[i].coilID,
                                         l_cdsInfo->m_coordinatetbl[i].detectedFlag,
                                         l_cdsInfo->m_coordinatetbl[i].location
                                        );

                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message_str);
                    //-Patni-Hemant/2010Aug02/Added/TMSC-REDMINE-520/CDS Abort Issue
                }
            }
        }
    }


    //+Patni-Hemant/2010Aug02/Added/TMSC-REDMINE-520/CDS Abort Issue
    l_message_str.Format(_T("Displaying Detection Status (After Error handling)"));
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message_str);

    for (i = ACQ_COILINFO_PORT_A; i < PQM_NUM_COIL_PORT; i++) {


        l_message_str.Format(_T("Coil id : %x, detectedFlag : %d, location : %d"),
                             l_cdsInfo->m_coordinatetbl[i].coilID,
                             l_cdsInfo->m_coordinatetbl[i].detectedFlag,
                             l_cdsInfo->m_coordinatetbl[i].location
                            );
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message_str);
    }

    //-Patni-Hemant/2010Aug02/Added/TMSC-REDMINE-520/CDS Abort Issue
}

//************************************Method Header************************************
// Method Name  : PqmCdsPositionForCoilSel
// Author       : PATNI/ HEMANT
// Purpose      : Position for calculating the coil selection screen information
//***********************************************************************************
void CpqmCDS::PqmCdsPositionForCoilSel(
    CPqmProtocol* const f_curObj
)
{
    LPCTSTR FUNC_NAME = _T("CpqmCDS::PqmCdsPositionForCoilSel");
    PQM_SCOPED_TRACE();
    VftSectionRecvrCoil_t	l_recvrCoil ;
    sedb_rec_t*			 l_sec_p[PQM_NUM_COIL_ELEMENT] = {0};						/* Section for information retrieval */
    int					l_sec_cnt = 0;												/* Section Number Information */
    CString l_log_str = _T("") ;

    CpqmCDSInfo* l_cdsInfo = f_curObj->GetCDSInfo();

    PqmSaturnCoil_t* l_saturn_coil = f_curObj->GetSaturnCoil();

    CPqmCoilData* l_coil_data = CDataManager::GetInstance()->GetPQMCoilData();

    for (int j = 0; j < l_saturn_coil->CoilList.num; j++) {
        /* All positional information to keep 0 */
        l_saturn_coil->CoilList.coillist[j].coordinate = 0;
        l_saturn_coil->CoilListcoordinate.coordinate_list[j].coordinate = 0;
    }

    flt32_t l_start = 0, l_end = 0, l_point = 0;

    int l_coil_width_to_consider = m_cds_param.m_couchLabelSize;

    for (int i = ACQ_COILINFO_PORT_A; i < PQM_NUM_COIL_PORT; i++) {
        if ((l_cdsInfo->m_coordinatetbl[i].coilID  == NULL)		||
            !l_cdsInfo->m_coil_tbl[i]) {
            /* Port is not connected to the coil is measured skip position */
            continue;
        }

        if (!l_cdsInfo->m_coil_tbl[i]->coilLocation) {
            /*Coil position skip Hide */
            continue;
        }

        if (!l_cdsInfo->m_coordinatetbl[i].detectedFlag) {
            /*Coil position was determined not to fly.*/
            continue;
        }

        if ((l_cdsInfo->m_coil_tbl[i]->groupId[0] != 0xffff)		&&
            (l_cdsInfo->m_coordinatetbl[i].coilID != l_cdsInfo->m_coil_tbl[i]->groupId[0])) {
            /*Groups except the top coil is not*/
            continue;
        }

        /* Coordinates of the top point of the coil */
        l_recvrCoil.coilID		= (uint16_t)l_cdsInfo->m_coordinatetbl[i].coilID;

        if (ACQ_COILINFO_PORT_D <= i && i <= ACQ_COILINFO_PORT_E) {
            l_recvrCoil.portID[0]	= CDB_PORT_D;

        } else if (ACQ_COILINFO_PORT_G1 <= i && i <= ACQ_COILINFO_PORT_G7) {
            l_recvrCoil.portID[0]	= CDB_PORT_G;
        }

        l_recvrCoil.channel	= CDB_CHANNEL_A;
        l_recvrCoil.SectionID	= 1;

        if (E_NO_ERROR != l_coil_data->PqmCoilGetSaturnSection(&l_recvrCoil, &l_sec_cnt, l_sec_p)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PqmCoilGetSaturnSection failed"));
        }

        int l_couchLabelSize;

        if (l_sec_cnt > 0 && l_sec_p[0]->elemXYZ[2] >= 240 && ((l_cdsInfo->m_coil_tbl[i]->coilLocation & LOCATION_HEAD_MASK) == LOCATION_HEAD_MASK)) {
            l_couchLabelSize = 2 * m_cds_param.m_couchLabelSize;

        } else {
            l_couchLabelSize = m_cds_param.m_couchLabelSize;
        }

        if (l_cdsInfo->m_coil_tbl[i]->coilDetect == VFC_CDS_STATUS_AUTO_SELECT) {

            l_coil_width_to_consider = l_couchLabelSize;

        } else {
            l_coil_width_to_consider = l_sec_p[0]->elemXYZ[2] ;
        }

        if (l_sec_cnt >  0) {
            l_point = (flt32_t)(l_cdsInfo->m_coordinatetbl[i].location + (l_sec_p[0]->dist - l_coil_width_to_consider / 2.0));

        } else {
            /* No section (never comes true)*/
            l_point = (flt32_t)(l_cdsInfo->m_coordinatetbl[i].location - (l_cdsInfo->m_coil_tbl[i]->size / 2.0));
        }

        //prepare log information
        l_log_str.Format(_T("Coil Id          : %X"), l_recvrCoil.coilID);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);

        l_log_str.Format(_T("No Of Sections   : %d"), l_sec_cnt);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);

        l_log_str.Format(_T("Coil Cordinate   : %d"), l_cdsInfo->m_coordinatetbl[i].location);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);

        if (l_sec_p[0] && (l_sec_cnt > 0)) {
            l_log_str.Format(_T("Section dist     : %f"), l_sec_p[0]->dist);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);
        }

        l_log_str.Format(_T("Section left pos : %f"), l_point);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);

        const double l_rgn_size = ((m_cds_param.m_coilsel_configparams.gui_sec_step * l_couchLabelSize) / 2.f);

        for (j = 0; j < l_saturn_coil->CoilList.num; j++) {
            if ((l_saturn_coil->CoilList.coillist[j].coilID == l_cdsInfo->m_coordinatetbl[i].coilID)	&&
                (memcmp(l_saturn_coil->CoilList.coillist[j].portID, g_porttbl_object[i].c, sizeof(char) * 2) == 0)) {

                for (flt32_t k = 1; k <= 15; k += (flt32_t)(m_cds_param.m_coilsel_configparams.gui_sec_step)) {
                    /*	Position the bed to be included as a central starting position in the size of Position To allow that position */
                    /* Position to start from the left edge of the range  */
                    l_start = (flt32_t)(((k - 1) * l_couchLabelSize) - l_rgn_size + m_cds_param.m_couchEndMargin);

                    /* Position within the range at the far right end of the range of */
                    l_end   = (flt32_t)(((k - 1) * l_couchLabelSize) + l_rgn_size + m_cds_param.m_couchEndMargin);

                    if (l_start <= l_point && l_point <= l_end) {
                        /*Position the coil have  */
                        l_saturn_coil->CoilList.coillist[j].coordinate = (uint8_t)(k + 0.005);
                        l_saturn_coil->CoilListcoordinate.coordinate_list[j].coordinate = k;

                        l_log_str.Format(_T("Coil Id: %X, Coil start pos: %d, Coil start pos in float: %f"),
                                         l_saturn_coil->CoilList.coillist[j].coilID,
                                         l_saturn_coil->CoilList.coillist[j].coordinate,
                                         l_saturn_coil->CoilListcoordinate.coordinate_list[j].coordinate
                                        );
                        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);

                        break;
                    }
                }
            }
        }
    }
}

//************************************Method Header************************************
// Method Name  : PqmCdsScanLastCheck
// Author       : PATNI/ HEMANT
// Purpose      : Thius function will take final decision whether to scan CDS or not
//                Execute CDS when any of following is true
//                - If connected port information has changed
//                - If couch has moved to more than max limit allowed
//                - If any previous protocol doesn't ave position information with the
//                  same connected coil
//                Copy at the same time that location information to confirm the position
//                of the coil.
//The possible return valeus are :
//PQM_CDS_NONE		0x0000		 Not scan
//PQM_CDS_SCAN		0x0002		 Scan
//PQM_CDS_PREV		0x0004		 Using the results of previous measurements without scanning
//***********************************************************************************
//Patni-MP/2010Dec13/Modified/REDMINE-932
int CpqmCDS::PqmCdsScanLastCheck(
    CPqmProtocol* const f_current_obj,
    const bool f_check_for_fixed_coils /*= false*/
)
{
    LPCTSTR FUNC_NAME = _T("CpqmCDS::PqmCdsScanLastCheck");
    PQM_SCOPED_TRACE();

    status_t	l_ret			= PQM_CDS_NONE;	/*  CDS whether the scan */

    if (NULL == f_current_obj) {

        ASSERT(FALSE);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_current_obj is NULL"));
        return l_ret;
    }

    CDataManager* l_data_manager = CDataManager::GetInstance();

    if (NULL == l_data_manager) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_data_manager is NULL"));
        return l_ret;
    }

    CPqmCoilData* l_coil_data = l_data_manager->GetPQMCoilData();

    if (NULL == l_coil_data) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_coil_data is NULL"));
        return l_ret;
    }

    CPQMStudy* l_study = m_pqm->GetStudy();
    CpqmCDSInfo* l_cur_cdsinfo = f_current_obj->GetCDSInfo();

    bool l_check_for_fixed_coils = f_check_for_fixed_coils && (VFC_CDS_STATUS_AUTO_SELECT == l_cur_cdsinfo->m_cds_status) ;

    if (CDSCheckCoil_Level2 == GetCDSCheckCoilFlag()) {
        if (f_current_obj->GetHasParent() && (VFC_MOVE_COUCH_METHOD_PARENT_POSI == f_current_obj->GetcouchMethodFlg())) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_current_obj Has Parent"));

            if (!l_check_for_fixed_coils) {
                f_current_obj->GetCDSInfo()->m_cds_status |= VFC_CDS_STATUS_USE_PREVIOUS;
                CCDSUtilities::PutCdsStatus(l_study->GetVfDBHandle(), f_current_obj->GetProtocol() , &f_current_obj->GetCDSInfo()->m_cds_status);
            }

            return PQM_CDS_NO_CONFIRM;
        }
    }


    int			i = 0 ;
    int			l_pos = 0;
    bool		l_couch_flg	= false;	/* Sleeping position within an acceptable range TRUE / FALSE */
    bool		l_prev_flg	= false;	/* Weather to consider previous protocol CDS information or not  */

    CPqmProtocol* prObj = f_current_obj;
    CPqmProtocol* l_src_prot = NULL;

    while (1) {
        if ((l_pos = prObj->GetPosition()) == 0) {
            break;
        }

        //pos is 1 base...
        if ((prObj = l_study->GetPrevItemFromPos(prObj)) == NULL) {
            break;
        }

        if (prObj->GetAcqStatus() != C_PR_STATUS_DONE) {
            continue;
        }

        if (false == prObj->GetCDSInfo()->m_cds_flag) {
            continue;
        }

        CpqmCDSInfo* l_proj_cdsinfo = prObj->GetCDSInfo();


        /* Coordinate to calculate the unadjusted (for error checking) */
        PqmCdsSetCoordinatetbl(l_proj_cdsinfo->m_locationtbl,
                               l_proj_cdsinfo->m_org_coordinatetbl,
                               prObj->GetPqmPrMode()->GetCouchPos());

        if (((l_proj_cdsinfo->m_cds_status & VFC_CDS_STATUS_SCAN)	&&
             !(l_proj_cdsinfo->m_cds_status & VFC_CDS_STATUS_USE_PREVIOUS))
            || (l_check_for_fixed_coils && (VFC_CDS_STATUS_AUTO_SELECT == l_proj_cdsinfo->m_cds_status))
           ) {
            /* CDS protocol scanned */
            /* Information coil */

            for (i = ACQ_COILINFO_PORT_A; i < PQM_NUM_COIL_PORT; i++) {
                if (l_proj_cdsinfo->m_coordinatetbl[i].coilID  !=
                    l_cur_cdsinfo->m_coordinatetbl[i].coilID) {
                    /* Information earlier Coil Coil is not so different from the information obtained */
                    break;
                }
            }

            if (l_proj_cdsinfo->m_coordinatetbl[i].coilID  !=
                l_cur_cdsinfo->m_coordinatetbl[i].coilID) {
                /* Twice to exit both the loop break */
                break;
            }

            if (prObj->GetPqmPrMode()->GetScanNumber() != f_current_obj->GetPqmPrMode()->GetScanNumber()) {
                /*Information that had previously
                been depressed since the machine does not get floodlight */

                break;
            }

            /*Sleeping position*/
            if (ABSF(f_current_obj->GetPqmPrMode()->GetCouchPos() - prObj->GetPqmPrMode()->GetCouchPos()) <= m_cds_param.m_moveCouchLimit) {
                /* Sleeping  position within the acceptable range */
                l_couch_flg = TRUE;		/* CDS does not scan */
            }

            /* Copy the information has been confirmed coil position */
            for (i = ACQ_COILINFO_PORT_A; i < PQM_NUM_COIL_PORT; i++) {
                if (!l_cur_cdsinfo->m_coordinatetbl[i].detectedFlag	&&
                    l_proj_cdsinfo->m_coordinatetbl[i].detectedFlag) {
                    l_cur_cdsinfo->m_coordinatetbl[i].detectedFlag = TRUE;
                    l_cur_cdsinfo->m_coordinatetbl[i].location  =
                        l_proj_cdsinfo->m_coordinatetbl[i].location;

                    /* location will need to adjust position if necessary later */
                    l_cur_cdsinfo->m_used_locationtbl[i] =
                        l_proj_cdsinfo->m_locationtbl[i].location;

                    l_cur_cdsinfo->m_org_coordinatetbl[i] = l_proj_cdsinfo->m_org_coordinatetbl[i];

                    l_prev_flg = TRUE;

                    if (l_src_prot == NULL)
                        l_src_prot = prObj;

                } else if (l_proj_cdsinfo->m_coordinatetbl[i].detectedFlag	&&
                           (l_cur_cdsinfo->m_coordinatetbl[i].location == l_proj_cdsinfo->m_coordinatetbl[i].location)) {

                    /* location will need to adjust position if necessary later */
                    l_cur_cdsinfo->m_used_locationtbl[i] =
                        l_proj_cdsinfo->m_locationtbl[i].location;

                    l_cur_cdsinfo->m_org_coordinatetbl[i] = l_proj_cdsinfo->m_org_coordinatetbl[i];
                }

            }
        }
    }

    /* Check if there is no information to confirm the position of coil */
    for (i = ACQ_COILINFO_PORT_A; i < PQM_NUM_COIL_PORT; i++) {
        if ((l_cur_cdsinfo->m_coordinatetbl[i].coilID  != NULL)	&&
            !l_cur_cdsinfo->m_coordinatetbl[i].detectedFlag) {
            l_ret = PQM_CDS_SCAN;
        }
    }

    if (l_ret) {
        /* Stock Information undetermined coil*/
        if (l_couch_flg) {
            /* If you have a position of tolerance in the imaging protocol of the past */
            l_ret = PQM_CDS_PREV;

        } else if (l_cur_cdsinfo->m_cds_rescan	&&
                   (ABSF(f_current_obj->GetPqmPrMode()->GetCouchPos() - GetPqmAcqStatic()->m_lastCdscouchPos) <= m_cds_param.m_moveCouchLimit)) {
            l_ret = PQM_CDS_NONE;
        }

    } else if (l_prev_flg) {
        l_ret = PQM_CDS_PREV;
    }

    //PqmDebugAcqMonitor("*-*-*-*-*-*-cds_rescan %d", l_cur_cdsinfo->m_cds_rescan);
    CPQMDebugInfo::GetInstance(m_pqm)->PqmDebugAcqMonitor("*-*-*-*-*-*-cds_rescan %d", l_cur_cdsinfo->m_cds_rescan);

    if (l_cur_cdsinfo->m_cds_rescan) {

        PqmSaturnCoil_t* l_saturn_coil = f_current_obj->GetSaturnCoil();

        if ((l_coil_data->PqmIsSameCoilInCDS(l_saturn_coil,
                                             &(GetPqmAcqStatic()->m_lastCdsCoil),
                                             f_current_obj->GetCoilDBTable(),
                                             &(GetPqmAcqStatic()->m_lastCdstbl)
                                            ) == FALSE)			||
            (f_current_obj->GetPqmPrMode()->GetScanNumber() != GetPqmAcqStatic()->m_lastCdsscanNumber)) {
            memset(l_cur_cdsinfo->m_coordinatetbl, 0, sizeof(Vft_CdsCoilLocation_t)*PQM_NUM_COIL_PORT);
            memset(l_cur_cdsinfo->m_locationtbl, 0, sizeof(Vft_CdsCoilLocation_t)*PQM_NUM_COIL_PORT);

            PqmCdsSetListCoil(f_current_obj);

            /* Wrote to study */
            CCDSUtilities::PutCdsCoordinate(l_study->GetVfDBHandle(),
                                            f_current_obj->GetProtocol(),
                                            l_cur_cdsinfo->m_coordinatetbl);

            l_ret = PQM_CDS_SCAN;
        }

        //PqmDebugAcqMonitor("lastCdsscanNumber %d", PqmAcqStatic.lastCdsscanNumber);
        //PqmDebugAcqMonitor("scanNumber %d", f_current_obj->GetPqmPrMode()->GetScanNumber());
        CPQMDebugInfo::GetInstance(m_pqm)->PqmDebugAcqMonitor("lastCdsscanNumber %d", GetPqmAcqStatic()->m_lastCdsscanNumber);
        CPQMDebugInfo::GetInstance(m_pqm)->PqmDebugAcqMonitor("scanNumber %d", f_current_obj->GetPqmPrMode()->GetScanNumber());
    }

    if (PQM_CDS_PREV == l_ret) {

        if (NULL == l_src_prot) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_src_prot is NULL"));
            return l_ret;
        }

        f_current_obj->GetCDSInfo()->m_src_protocol = l_src_prot->GetProtocol();

        int l_copy_coil_from = -1;

        if ((CDSCheckCoil_Level2 == GetCDSCheckCoilFlag()) && CanIgnoreCDSExecution(f_current_obj, l_src_prot, l_copy_coil_from, l_check_for_fixed_coils)) {

            CPqmProtocol* l_copy_from_protocol = l_study->GetProtocol(l_copy_coil_from);

            if ((l_copy_coil_from >= 0) && l_copy_from_protocol) {

                CPQMDebugInfo::GetInstance(m_pqm)->PqmDebugAcqMonitor(_T("Copying CDS coil info from Protocol Position = %d, Acqorder = %d"),
                        l_copy_from_protocol->GetPosition(), l_copy_from_protocol->GetProtocol());

                CopyCoilInfoFrom(l_copy_from_protocol, f_current_obj);

                if (!l_check_for_fixed_coils) {
                    f_current_obj->GetCDSInfo()->m_cds_status |= VFC_CDS_STATUS_USE_PREVIOUS;
                    CCDSUtilities::PutCdsStatus(l_study->GetVfDBHandle(), f_current_obj->GetProtocol() , &f_current_obj->GetCDSInfo()->m_cds_status);
                }

                m_pqm->WriteToUI(PQM_MSG_CDS_APPLY_CMPS_COIL_CUR_PROT);

                CopyCDSCCoilInfoAsPerBatchSetting(f_current_obj);

                l_ret = PQM_CDS_NO_CONFIRM;
            }
        }
    }

    return l_ret;
}

//************************************Method Header************************************
// Method Name  : CopyCoilInfoFrom
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CpqmCDS::CopyCDSCCoilInfoAsPerBatchSetting(
    CPqmProtocol* f_cds_protocol
) const
{

    if (m_pqm->GetAllCoilSAR()) {

        CPQMStudy* l_study_obj = m_pqm->GetStudy();

        for (CPqmProtocol* l_next_obj = l_study_obj->GetNextItemFromPos(f_cds_protocol);
             l_next_obj != NULL;
             l_next_obj = l_study_obj->GetNextItemFromPos(l_next_obj)) {
            if (!CopyCoilInfoFrom(f_cds_protocol, l_next_obj)) {
                return false;
            }
        }

        //send message to GUI only when its required
        m_pqm->WriteToUI(PQM_MSG_CDS_APPLY_COMPASS_COIL_TO_ALL_BELOW);
    }

    return true;
}

//************************************Method Header************************************
// Method Name  : CopyCoilInfoFrom
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CpqmCDS::CopyCoilInfoFrom(
    CPqmProtocol* src,
    CPqmProtocol* dest
) const
{
    LPCTSTR FUNC_NAME = _T("CpqmCDS::CopyCoilInfoFrom");
    PQM_SCOPED_TRACE();


    CPQMStudy* l_study_obj = m_pqm->GetStudy();

    CPqmCoilData* l_coil_data = CDataManager::GetInstance()->GetPQMCoilData();
    DllT* l_coil_head    = l_coil_data->GetHead();
    DllT* l_section_head = m_pqm->GetDataManager()->GetPQMSectionData()->GetHead();

    memcpy(dest->GetSaturnCoil(), src->GetSaturnCoil(), sizeof(PqmSaturnCoil_t));

    /* Signal information settings (Scan is also set at the start) */
    AcqGetcoilSignal(m_pqm->GetNumChannels(), dest->GetSaturnCoil()->numSection,
                     dest->GetSaturnCoil()->RcvCoil,
                     dest->GetSaturnCoil()->Signal,
                     l_coil_head,
                     l_section_head);

    if (E_NO_ERROR != l_coil_data->PqmAllCoilGetSaturn(dest->GetSaturnCoil(),
            dest->GetCoilDBTable())) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PqmAllCoilGetSaturn failed"));
    }

    /* Study writing */
    l_study_obj->PutSaturnCoil(l_study_obj->GetVfDBHandle(),
                               dest->GetProtocol(),
                               dest->GetSaturnCoil());

    l_study_obj->PutConnectedCoil(l_study_obj->GetVfDBHandle(),
                                  dest->GetProtocol(),
                                  dest);

    l_study_obj->PutRecvrCoil(l_study_obj->GetVfDBHandle(),
                              dest->GetProtocol(),
                              dest);

    /* Send to a coil */
    bool l_flag = l_study_obj->SetTxCoilInSaturn(l_study_obj->GetVfDBHandle(), dest, dest->GetCoilDBTable());

    if (!l_flag) {
        /* If no output message is sent to coil Eranabi */
        CPQMLogMgr::GetInstance()->DisplayScanError(
            IDS_CDS_ERR_ILLEGAL_COIL_ID,
            _T("IDS_CDS_ERR_ILLEGAL_COIL_ID"),
            SCAN_ERR_WARNING, L"ScanProcedure.cpp"); //+Patni-NP/2010May26/Modified/Redmine-347

    } else {
        /* Coil name*/
        if (!m_pqm->GetCoilSelectionTool()->CSSetCoilName(dest)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CSSetCoilName failed."));
            l_flag = false;
        }
    }

    return l_flag;
}

//************************************Method Header************************************
// Method Name  : PqmUiCdsDebug
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CpqmCDS::PqmUiCdsDebug(
) const
{
    return m_cds_debug.GetCDSDebugFlag();
}

//************************************Method Header************************************
// Method Name  : PqmUiCdsDebugWindowCB
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CpqmCDS::PqmUiCdsDebugWindowCB(
    CPqmProtocol* const f_protocol_obj
)
{
    m_cds_debug.PqmUiCdsDebugWindowCB(f_protocol_obj);
    //OnCDSDebugCompleted();
}

//************************************Method Header************************************
// Method Name  : PqmCoilSelEnd
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CpqmCDS::PqmCoilSelEnd(
    CPqmProtocol* const f_pmObj
)
{
    //Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(f_pmObj);

    //	if( f_pmObj->core.active )
    //	{
    //		ProMsgSend(f_pmObj->core.wrFd,PROMSG_PQM2COILSEL_EXIT,0,NULL);
    //		pqmPmResetCoilSel((void *)f_pmObj) ;
    //	}

    if (GetCDSCoilSelStatus() == CDS_WAIT) {
        pqmPmCoilSelCdsTimerReset();
        PqmPmCoilSelAfterAutoCoil(m_cds_debug.GetCurrentCDSDebugProtocol());
    }
}

//************************************Method Header************************************
// Method Name  : PqmPmCoilSelAfterAutoCoil
// Author       : PATNI/ HEMANT
// Purpose      : This function will invoke coil selection dialog for COMPASS confirmation
//***********************************************************************************
void CpqmCDS::PqmPmCoilSelAfterAutoCoil(
    const int f_prot
)
{
    LPCTSTR FUNC_NAME = _T("CpqmCDS::PqmPmCoilSelAfterAutoCoil");
    PQM_SCOPED_TRACE();

    if (!m_pqm) {
        return ;
    }

    m_cds_debug.SetCurrentCDSDebugProtocol(f_prot);

    if (m_pqm->IsDialogActive(IS_COIL_SELECTION_DIALOG)) {
        m_pqmPmCoilSelCDSFlg = CDS_WAIT;

        //pqmPmCoilSelCDSprot = prot;
        //m_cds_debug.SetCurrentCDSDebugProtocol(f_prot);

        //PqmUiErrorDialog(PqmUiFetchLiteral("MSG_AFTER_CDS"),ERR_NOTIFY);

        CPQMLogMgr::GetInstance()->DisplayScanError(
            IDS_CDS_MSG_AFTER_CDS,
            _T("IDS_CDS_MSG_AFTER_CDS"),
            SCAN_ERR_NOTIFY, L"pqmCDS.cpp"); //+Patni-NP/2010May26/Modified/Redmine-347

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Coil dialog is already open"));

        pqmPmCoilSelCdsTimerSetUp();

        return;
    }

    //PqmListSelectPos(PqmPqGetPosFromProt(prot),FALSE,0);
    //PqmPqSetVisible(PqmPqGetPosFromProt(prot));

    m_pqmPmCoilSelCDSFlg = CDS_ON;

    //The following call will do the above commented lines' work
    //That is, it will change the selection and then will visible
    //that protocol
    m_pqm->WriteToUI(PQM_MSG_CDS_COIL_CONFIRMATION);
}

//************************************Method Header************************************
// Method Name  :pqmPmCoilSelCdsTimeOut
// Author       : PATNI/ HEMANT
// Purpose      : Coil selection screen close-out process waiting time (Alarm interrupt handler)
//***********************************************************************************
void CpqmCDS::pqmPmCoilSelCdsTimeOut()
{
    LPCTSTR FUNC_NAME = _T("CpqmCDS::pqmPmCoilSelCdsTimeOut");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    //PqmPrData_t	*prObj;

    pqmPmCoilSelCdsTimerReset() ;		/* TIMER RESET */

    //DB_PRINTF("N",("<pqmPmCoilSelCdsTimeOut> [%s:%d]!!!!\n",__FILE__,__LINE__)) ;

    /* When the timeout to abort with a message to error dialog */

    //PqmUiErrorDialog(PqmUiFetchLiteral("MSG_AFTER_CDS_TIMEOUT"),ERR_NOTIFY);
    CPQMLogMgr::GetInstance()->DisplayScanError(
        IDS_CDS_MSG_AFTER_CDS_TIMEOUT,
        _T("IDS_CDS_MSG_AFTER_CDS_TIMEOUT"),
        SCAN_ERR_NOTIFY, L"pqmCDS.cpp"); //+Patni-NP/2010May26/Modified/Redmine-347
}

//************************************Method Header************************************
// Method Name  : PqmCoilCDSAutoCoilSet
// Author       : PATNI/ HEMANT
// Purpose      : Set automatic coil Stored in the field for imaging a section within a range imaging
//***********************************************************************************
status_t CpqmCDS::PqmCoilCDSAutoCoilSet(
    CPqmProtocol* const curObj
)const
{
    LPCTSTR FUNC_NAME = _T("CpqmCDS::PqmCoilCDSAutoCoilSet");
    PQM_SCOPED_TRACE();

    CDataManager* l_data_manager = CDataManager::GetInstance();

    if (NULL == l_data_manager) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_data_manager is NULL"));
        return E_ERROR ;
    }

    CPqmCoilData* l_coil_data = l_data_manager->GetPQMCoilData();

    if (NULL == l_coil_data) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_coil_data is NULL"));
        return E_ERROR ;
    }

    char port[PQM_NUM_COIL_PORT][3]
    =	{	ACQ_COIL_PORT_A ,
            ACQ_COIL_PORT_B ,
            ACQ_COIL_PORT_C ,
            ACQ_COIL_PORT_D1,
            ACQ_COIL_PORT_D2,
            ACQ_COIL_PORT_F ,
            ACQ_COIL_PORT_G1,
            ACQ_COIL_PORT_G2,
            ACQ_COIL_PORT_G3,
            ACQ_COIL_PORT_G4,
            ACQ_COIL_PORT_G5,
            ACQ_COIL_PORT_G6,
            ACQ_COIL_PORT_G7,
      };

    register DllT* dll = NULL;
    sedb_rec_t* l_sec_rec_p = NULL;

    DllT* sectionHead = l_data_manager->GetPQMSectionData()->GetHead();
    PqmSaturnCoil_t* l_saturn_coil = curObj->GetSaturnCoil() ;
    CpqmCDSInfo* cdsInfo = curObj->GetCDSInfo();

    if (!l_saturn_coil || !sectionHead || !cdsInfo) {

        ASSERT(FALSE) ;
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("cdsInfo or l_saturn_coil or sectionHead is NULL"));
        return E_ERROR ;
    }

    int	center = 0;							/* Position the bed field */
    center = PqmCdsChangeCouch(curObj->GetPqmPrMode()->GetCouchPos());

    /* Measuring range */
    int	start = 0, end = 0;						/* Range */
    start = center - GetCDSRange();
    end   = center + GetCDSRange();

    //PqmDebugAcqMonitor("CDS center position  = %d", center);
    //PqmDebugAcqMonitor("CDS start  position  = %d", start);
    //PqmDebugAcqMonitor("CDS end    position  = %d", end);
    CPQMDebugInfo::GetInstance(m_pqm)->PqmDebugAcqMonitor("CDS center position  = %d", center);
    CPQMDebugInfo::GetInstance(m_pqm)->PqmDebugAcqMonitor("CDS start  position  = %d", start);
    CPQMDebugInfo::GetInstance(m_pqm)->PqmDebugAcqMonitor("CDS end    position  = %d", end);


    //+Patni-Hemant/2010Aug02/Added/TMSC-REDMINE-520/CDS Abort Issue
    CString l_message_str(_T(""));

    l_message_str.Format(_T("CDS center position  = %d"), center);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message_str);

    l_message_str.Format(_T("CDS start position  = %d"), start);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message_str);

    l_message_str.Format(_T("CDS end position  = %d"), end);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message_str);

    l_message_str.Format(_T("Displaying CDS coil data before Auto coil set..."));
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message_str);

    int i = 0, j = 0;

    for (i = ACQ_COILINFO_PORT_A; i < PQM_NUM_COIL_PORT; i++) {

        l_message_str.Format(_T("Coil id : %x, detectedFlag : %d, location : %d"),
                             cdsInfo->m_coordinatetbl[i].coilID,
                             cdsInfo->m_coordinatetbl[i].detectedFlag,
                             cdsInfo->m_coordinatetbl[i].location
                            );
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message_str);
    }

    //-Patni-Hemant/2010Aug02/Added/TMSC-REDMINE-520/CDS Abort Issue

    int	sec_center = 0;						/* Center section */
    int	sec_limit = 0;						/* Position not to go into sections measuring range */
    int sec_size = 0;						/* Size sensitivity */
    unsigned int coil_id = 0;						/* Coil ID */
    unsigned int sect_id = 0;						/* Section ID */

    for (i = ACQ_COILINFO_PORT_A; i < PQM_NUM_COIL_PORT; i++) {
        if ((cdsInfo->m_coordinatetbl[i].coilID  != NULL)	&&
            cdsInfo->m_coordinatetbl[i].detectedFlag) {
            /* Position has been confirmed */
            if (cdsInfo->m_coil_tbl[i]->sectionDB) {
                for (dll = sectionHead; dll != NULL; dll = dll->next) {
                    l_sec_rec_p = (sedb_rec_t*)dll->item;

                    if ((l_sec_rec_p->coil_id      == (cdsInfo->m_coil_tbl[i]->coil_id & COIL_PHYSICALID_MASK))	&&
                        (l_sec_rec_p->port         == cdsInfo->m_coil_tbl[i]->port)		&&
                        (l_sec_rec_p->insectionPri == 1)) {
                        sec_size = l_sec_rec_p->elemXYZ[2];
                        coil_id  = l_sec_rec_p->coil_id;
                        sect_id  = l_sec_rec_p->section_id;
                        sec_center = cdsInfo->m_coordinatetbl[i].location + (int)l_sec_rec_p->dist;

                        if (sec_center < center) {
                            /* Left of center field */
                            sec_limit = (int)((sec_center + sec_size / 2.0) - (sec_size * (GetCDSRatio() / 100.0)));

                        } else {
                            /* From right-center field */
                            sec_limit = (int)((sec_center - sec_size / 2.0) + (sec_size * (GetCDSRatio() / 100.0)));
                        }

                        if (start <= sec_limit && sec_limit <= end) {
                            l_saturn_coil->RcvCoil[j].portID[0] = port[i][0];
                            l_saturn_coil->RcvCoil[j].portID[1] = port[i][1];
                            l_saturn_coil->RcvCoil[j].coilID    = coil_id;
                            l_saturn_coil->RcvCoil[j].SectionID = sect_id;
                            l_saturn_coil->RcvCoil[j].channel   = cdsInfo->m_coil_tbl[i]->channel;
                            j++;
                        }

                        //+Patni-Hemant/2010Aug02/Added/TMSC-REDMINE-520/CDS Abort Issue
                        else {
                            l_message_str.Format(_T("Could not consider the coil. Coild id : %x, sec_limit= %d"), coil_id, sec_limit);
                            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message_str);
                        }

                        //-Patni-Hemant/2010Aug02/Added/TMSC-REDMINE-520/CDS Abort Issue
                    }
                }

            } else {
                sec_size = cdsInfo->m_coil_tbl[i]->size;
                coil_id  = cdsInfo->m_coil_tbl[i]->coil_id & COIL_PHYSICALID_MASK;
                sect_id  = 0;
                sec_center = cdsInfo->m_coordinatetbl[i].location;

                if (sec_center < center) {
                    sec_limit = (int)((sec_center + sec_size / 2.0) - (sec_size * (GetCDSRatio() / 100.0)));

                } else {
                    sec_limit = (int)((sec_center - sec_size / 2.0) + (sec_size * (GetCDSRatio() / 100.0)));
                }

                if (start <= sec_limit && sec_limit <= end) {
                    l_saturn_coil->RcvCoil[j].portID[0] = port[i][0];
                    l_saturn_coil->RcvCoil[j].portID[1] = port[i][1];
                    l_saturn_coil->RcvCoil[j].coilID    = coil_id;
                    l_saturn_coil->RcvCoil[j].SectionID = sect_id;
                    l_saturn_coil->RcvCoil[j].channel   = cdsInfo->m_coil_tbl[i]->channel;
                    j++;
                }

                //+Patni-Hemant/2010Aug02/Added/TMSC-REDMINE-520/CDS Abort Issue
                else {
                    l_message_str.Format(_T("Could not consider the coil. Coild id : %x, sec_limit= %d"), coil_id, sec_limit);
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message_str);
                }

                //-Patni-Hemant/2010Aug02/Added/TMSC-REDMINE-520/CDS Abort Issue
            }

            if (j >= MAX_SELECT_COIL) {
                l_saturn_coil->numSection = j;
                return E_NO_ERROR;
            }
        }
    }

    CheckForLinkedSections(l_saturn_coil, j, cdsInfo);

    l_saturn_coil->numSection = j;

    if (l_saturn_coil->numSection == 0) {
        return E_ERROR;

    } else {
        return E_NO_ERROR;
    }
}

sedb_rec_t* CpqmCDS::GetSectionDataForSectionID(
    const int f_coilid,
    const int f_sectionid
) const
{
    register DllT* dll = NULL;
    sedb_rec_t* l_sec_rec_p = NULL;

    CDataManager* l_data_manager = CDataManager::GetInstance();
    DllT* sectionHead = l_data_manager->GetPQMSectionData()->GetHead();

    for (dll = sectionHead; dll != NULL; dll = dll->next) {

        l_sec_rec_p = (sedb_rec_t*)dll->item;

        if (NULL == l_sec_rec_p)
            continue;

        if ((f_coilid == l_sec_rec_p->coil_id) &&
            (f_sectionid == l_sec_rec_p->section_id)
           )
            return l_sec_rec_p;
    }

    return NULL;
}

int CpqmCDS::GetCoilindex(int f_coil_id, CpqmCDSInfo* cdsInfo) const
{
    for (int i = 0 ; i < PQM_NUM_COIL_PORT ; i++) {
        if (cdsInfo->m_coordinatetbl[i].coilID == (unsigned long)f_coil_id) {
            return i;
        }
    }

    return -1;
}

bool CpqmCDS::IsGroupedCoil(int f_coil_id, int f_linked_coil_id, CpqmCDSInfo* cdsInfo) const
{
    if (f_coil_id == f_linked_coil_id)
        return false;

    int l_coil_index = GetCoilindex(f_coil_id, cdsInfo);

    for (int gid = 0; gid < 4; gid++) {
        if ((cdsInfo->m_coil_tbl[l_coil_index]->groupId[gid] != f_coil_id) &&
            (cdsInfo->m_coil_tbl[l_coil_index]->groupId[gid] == f_linked_coil_id)) {
            return true;
        }
    }

    return false;

}
bool CpqmCDS::IsLinkedSectionOfGroupedCoil(sedb_rec_t* section_data, sedb_rec_t* linked_section_data, CpqmCDSInfo* cdsInfo) const
{
    if (section_data == NULL || linked_section_data == NULL || cdsInfo == NULL)
        return false;

    if (!IsGroupedCoil(section_data->coil_id, linked_section_data->coil_id, cdsInfo)) {
        return false;
    }

    if (section_data->link == linked_section_data->section_id) {
        if (section_data->link == section_data->section_id)
            return true;

    }

    return false;
}
bool CpqmCDS::IsLinkedSectionOfSameCoil(sedb_rec_t* section_data, sedb_rec_t* linked_section_data)const
{
    if (section_data == NULL || linked_section_data == NULL)
        return false;

    if ((section_data->coil_id == linked_section_data->coil_id) &&
        (section_data->link == linked_section_data->section_id))
        return true;

    else
        return false;

}

bool CpqmCDS::CheckForLinkedSections(
    PqmSaturnCoil_t* f_saturn_coil,
    int& f_total_coils, CpqmCDSInfo* cdsInfo
) const
{

    register DllT* dll = NULL;
    sedb_rec_t* l_sec_rec_p = NULL;

    bool l_found = false;
    int l_linked_section_id = 0;

    sedb_rec_t* linked_section_data = NULL ;
    sedb_rec_t* section_data = NULL ;

    int l_final_total_coils = f_total_coils ;

    for (int i = 0 ; i < f_total_coils ; i++) {

        section_data = GetSectionDataForSectionID(f_saturn_coil->RcvCoil[i].coilID, f_saturn_coil->RcvCoil[i].SectionID);

        if (NULL == section_data) {
            continue;
        }

        l_linked_section_id = section_data->link;

        if (l_linked_section_id <= 0)
            continue;

        l_found = false;

        for (int j = 0 ; j < f_total_coils ; j++) {

            if (i == j)
                continue;

            linked_section_data = GetSectionDataForSectionID(f_saturn_coil->RcvCoil[j].coilID, f_saturn_coil->RcvCoil[j].SectionID);

            if (IsLinkedSectionOfSameCoil(section_data, linked_section_data) ||
                IsLinkedSectionOfGroupedCoil(section_data, linked_section_data, cdsInfo)
               ) {

                l_found = true;
                break;
            }
        }

        if (!l_found) {

            linked_section_data = GetSectionDataForSectionID(f_saturn_coil->RcvCoil[i].coilID, l_linked_section_id);

            if (linked_section_data) {

                f_saturn_coil->RcvCoil[l_final_total_coils].portID[0] = f_saturn_coil->RcvCoil[i].portID[0];
                f_saturn_coil->RcvCoil[l_final_total_coils].portID[1] = f_saturn_coil->RcvCoil[i].portID[1];
                f_saturn_coil->RcvCoil[l_final_total_coils].coilID    = f_saturn_coil->RcvCoil[i].coilID;
                f_saturn_coil->RcvCoil[l_final_total_coils].SectionID = linked_section_data->section_id;
                f_saturn_coil->RcvCoil[l_final_total_coils].channel   = f_saturn_coil->RcvCoil[i].channel;
                l_final_total_coils++;
            }
        }
    }

    f_total_coils = l_final_total_coils ;

    return true;
}

//************************************Method Header************************************
// Method Name  : PqmCoilSetCDSCoil
// Author       : PATNI/ HEMANT
// Purpose      : Information set CDS scan coil Set position to measure all of the coil
//***********************************************************************************
status_t CpqmCDS::PqmCoilSetCDSCoil(
    CVarFieldHandle* const vf_handle,
    CPqmProtocol* const prObj
)const
{
    LPCTSTR FUNC_NAME = _T("CpqmCDS::PqmCoilSetCDSCoil");

    if (!vf_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("vf_handle is NULL"));
        return E_ERROR;
    }

    PQM_SCOPED_TRACE();

    CDataManager* l_data_manager = CDataManager::GetInstance();
    status_t				l_sts = E_ERROR;

    if (NULL == l_data_manager) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_data_manager is NULL"));
        return l_sts ;
    }

    CPqmCoilData* l_coil_data = l_data_manager->GetPQMCoilData();

    if (NULL == l_coil_data) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_coil_data is NULL"));
        return l_sts ;
    }

    int						i = 0 , j = 0;
    register DllT*			dll = NULL, *dll2 = NULL;
    PqmCoil_t*				l_coil_p = NULL;
    sedb_rec_t*				l_sec_rec_p = NULL, *l_sec_rec_p2 = NULL;
    int						l_num_sec = 0;							/* Imaging section 1 of the times */
    int						l_reps = 0;								/* Views imaging (if more than one dynamic) */
    int						l_num_sec_d[PQM_CDS_MAX_DYNAMIC] = {0};	/* For dynamic */
    unsigned int			l_prev_id = 0 ;
    int						l_sig_cnt = 0 ;
    int						l_sec_cnt = 0;

    DllT* sectionHead = l_data_manager->GetPQMSectionData()->GetHead();

    typedef struct porttbl_s {
        int		no;
        char	c[3];
    } porttbl_t;
    static porttbl_t port[]
    = {	{ACQ_COILINFO_PORT_A,  ACQ_COIL_PORT_A  },
        {ACQ_COILINFO_PORT_B,  ACQ_COIL_PORT_B  },
        {ACQ_COILINFO_PORT_C,  ACQ_COIL_PORT_C  },
        {ACQ_COILINFO_PORT_D,  ACQ_COIL_PORT_D1 },		/* D1 on L2,Saturn */
        {ACQ_COILINFO_PORT_E,  ACQ_COIL_PORT_D2 },		/* D1 on L2,Saturn */
        {ACQ_COILINFO_PORT_F,  ACQ_COIL_PORT_F  },
        {ACQ_COILINFO_PORT_G1, ACQ_COIL_PORT_G1 },		/* set for Saturn only */
        {ACQ_COILINFO_PORT_G2, ACQ_COIL_PORT_G2 },		/* set for Saturn only */
        {ACQ_COILINFO_PORT_G3, ACQ_COIL_PORT_G3 },		/* set for Saturn only */
        {ACQ_COILINFO_PORT_G4, ACQ_COIL_PORT_G4 },		/* set for Saturn only */
        {ACQ_COILINFO_PORT_G5, ACQ_COIL_PORT_G5 },		/* set for Saturn only */
        {ACQ_COILINFO_PORT_G6, ACQ_COIL_PORT_G6 },		/* set for Saturn only */
        {ACQ_COILINFO_PORT_G7, ACQ_COIL_PORT_G7 }
    };	/* set for Saturn only */

    //DB_FUNC_ENTER("[PqmCoilSetCDSCoil]:");
    VftCoilListInfo_t*		l_coilList = &(prObj->GetSaturnCoil()->CoilList);

    VftSectionRecvrCoil_t	sec[MAX_SELECT_COIL] = {0};				/* Section Information */
    VftSectionSignal_t		sig[MAX_SELECT_COIL] = {0};				/* Signal information */
    memset(sec, 0, sizeof(sec));
    memset(sig, 0, sizeof(sig));
    memset(l_num_sec_d, 0, sizeof(l_num_sec_d));

    CCoil l_coil ;
    const int l_num_of_channels = m_pqm->GetNumChannels();

    CString l_log_str = _T("");
    l_log_str.Format(_T("Number Of Channels = %d "), l_num_of_channels);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);


    for (i = 0; i < l_coilList->num; i++) {

        //l_coil_p = PqmCoilidGetSaturnCoil(l_coilList->coillist[i].coilID, l_coilList->coillist[i].portID[0], NULL);
        l_coil_data->PqmCoilidGetSaturnCoil(l_coilList->coillist[i].coilID, l_coilList->coillist[i].portID[0], NULL, l_coil);

        l_coil_p = l_coil.GetCoil() ;

        //Patni-Hemant/2010Mar3/Commented/ACE-Phase#3/CDS Requirement
        //Review comment - Sarjeet san
        if (!l_coil_p) {
            continue ;
        }

        if (l_coil_p->coilDetect & VFC_CDS_STATUS_SCAN) {
            for (j = ACQ_COILINFO_PORT_A; j < PQM_NUM_COIL_PORT; j++) {
                if (memcmp(l_coilList->coillist[i].portID, port[j].c, sizeof(char) * 2) == 0) {
                    break;
                }
            }

            if (l_coil_p->sectionDB != 0) {
                /* Section—L‚è */

                for (dll = sectionHead; dll != NULL; dll = dll->next) {
                    l_sec_rec_p = (sedb_rec_t*)dll->item;

                    if ((l_sec_rec_p->coil_id    == l_coilList->coillist[i].coilID)
                        && (l_sec_rec_p->port       == l_coilList->coillist[i].portID[0])
                        && (l_sec_rec_p->coilDetect)) {
                        sec[l_num_sec].portID[0] = l_coilList->coillist[i].portID[0];
                        sec[l_num_sec].portID[1] = l_coilList->coillist[i].portID[1];
                        sec[l_num_sec].coilID    = l_coilList->coillist[i].coilID;
                        sec[l_num_sec].channel   = l_coilList->coillist[i].channel;
                        sec[l_num_sec].SectionID = l_sec_rec_p->section_id;

                        for (dll2 = sectionHead; dll2 != NULL; dll2 = dll2->next) {
                            l_sec_rec_p2 = (sedb_rec_t*)dll2->item;

                            if ((l_sec_rec_p2->coil_id    == l_sec_rec_p->coil_id)
                                && (l_sec_rec_p2->port       == l_sec_rec_p->port)
                                && (l_sec_rec_p2->section_id  == l_sec_rec_p->section_id)
                                && (l_sec_rec_p2->coilDetect)) {
                                sig[l_num_sec].signal_no[sig[l_num_sec].num]          = l_sec_rec_p2->sig;
                                sig[l_num_sec].hybrid_signal[sig[l_num_sec].num]      = l_sec_rec_p2->hybSig;

                                sig[l_num_sec].num++;
                            }
                        }

                        l_num_sec++;
                    }
                }

            } else {
                /* Stock Section */
                sec[l_num_sec].portID[0] = l_coilList->coillist[i].portID[0];
                sec[l_num_sec].portID[1] = l_coilList->coillist[i].portID[1];
                sec[l_num_sec].coilID    = l_coilList->coillist[i].coilID;
                sec[l_num_sec].channel   = l_coilList->coillist[i].channel;
                sig[l_num_sec].num = l_coil_p->maxChannel;

                l_num_sec++;
            }
        }
    }

    /* Initialization */
    l_prev_id = sec[0].coilID;
    l_sig_cnt = 0;
    l_sec_cnt = 0;

    for (i = 0; i < l_num_sec; i++) {
        l_sig_cnt += sig[i].num;
        l_sec_cnt++;

        if ((sec[i].coilID    != sec[i + 1].coilID)	||
            (sec[i].portID[1] != sec[i + 1].portID[1])) {
            if (l_num_of_channels >= l_sig_cnt) {
                l_num_sec_d[l_reps] += l_sec_cnt;

            } else {
                l_sig_cnt = 0;
                l_reps++;
                i = i - l_sec_cnt;
            }

            l_sec_cnt = 0;
        }
    }


    if (l_num_of_channels >= l_sig_cnt) {
        l_num_sec_d[l_reps] += l_sec_cnt;
    }

    l_reps++;

    VfPathElem_t			pelm[2] = {0};
    pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    pelm[SVD_ST_PROTOCOL - 1].index = prObj->GetProtocol();

    int n = 0, num = 0;
    VfFieldSpec_t			vfReq[5] = {0};
    VFF_SET_ARGS(vfReq[n], SVN_PLN_REPETITIONS_CDS, SVT_PLN_REPETITIONS_CDS,
                 sizeof(int32_t), &l_reps, VFO_REPLACE);
    n++;

    l_log_str.Format(_T("SVN_PLN_REPETITIONS_CDS = %d "), l_reps);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);

    int s = 0;
    s = sizeof(int32_t) * l_reps;
    VFF_SET_ARGS(vfReq[n], SVN_PLN_NUM_SECTION_RECVR_COIL_CDS, SVT_PLN_NUM_SECTION_RECVR_COIL_CDS,
                 s, l_num_sec_d, VFO_REPLACE);
    n++;
    s = sizeof(VftSectionRecvrCoil_t) * l_num_sec;
    VFF_SET_ARGS(vfReq[n], SVN_PLN_SECTION_RECVR_COIL_CDS, SVT_PLN_SECTION_RECVR_COIL_CDS,
                 s, sec, VFO_REPLACE);
    n++;
    s = sizeof(VftSectionSignal_t) * l_num_sec;
    VFF_SET_ARGS(vfReq[n], SVN_PLN_SECTION_RECVR_COIL_SIGNAL_CDS, SVT_PLN_SECTION_RECVR_COIL_SIGNAL_CDS,
                 s, sig, VFO_REPLACE);
    n++;

    time_t					tm = 0L;    //Patni-KSS/2011Feb17/Modified/CPP Test tool Execution
    l_sts = vf_handle->PutFields(pelm, SVD_ST_PROTOCOL, vfReq, n, &num, &tm);

    if (l_sts != E_NO_ERROR || n != num) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
        /* Error handling */
        l_sts = E_ERROR ;
    }

    return l_sts;
}

//************************************Method Header************************************
// Method Name  : GetASCOMPASSParameter
// Author       : PATNI/ HEMANT
// Purpose      : This function will fil the AS-COMPASS setting dialog parameter in given arg
//***********************************************************************************
void CpqmCDS::GetASCOMPASSParameter(
    CDSCompassSettingParams* f_cdscompasssetting_param
) const  	//Patni-Hemant/2010May6/Added/JaFT/SM_PH3_CDR-CDS_CDR_18
{

    //Note : we will not check f_cdscompasssetting_param for NULL,  assuming it is checked before

    (*f_cdscompasssetting_param) = m_compass_setting_param;
}

void CpqmCDS::SetCSConfigParams(const sCSConfigParams& f_csconfigparams)
{

    m_cds_param.m_coilsel_configparams = f_csconfigparams ;
}

//************************************Method Header************************************
// Method Name  : SetASCOMPASSParameter
// Author       : PATNI/ HEMANT
// Purpose      : sets the as-compass setting dialog paramerter in class member variable
//***********************************************************************************
void CpqmCDS::SetASCOMPASSParameter(
    CDSCompassSettingParams* f_cdscompasssetting_param
)
{
    //Note : we will not check f_cdscompasssetting_param for NULL,  assuming it is checked before
    m_compass_setting_param = (*f_cdscompasssetting_param) ;
}

//************************************Method Header************************************
// Method Name  : GetCDSCheckCoilFlag
// Author       : PATNI/ HEMANT
// Purpose      : Returns the m_cds_CheckCoil member variable..
//***********************************************************************************
int CpqmCDS::GetCDSCheckCoilFlag(
) const
{
    return m_cds_CheckCoil ;
}

//************************************Method Header************************************
// Method Name  : SetCDSCheckCoilFlag
// Author       : PATNI/ HEMANT
// Purpose      : sets m_cds_CheckCoil member avriable
//***********************************************************************************
void CpqmCDS::SetCDSCheckCoilFlag(
    const int f_flag
)
{
    m_cds_CheckCoil = f_flag ;
}

//************************************Method Header************************************
// Method Name  : GetCurrentCDSProtocol
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CPqmProtocol* CpqmCDS::GetCurrentCDSProtocol(
)const
{
    return m_cds_debug.GetDebugProtocol();
}

//************************************Method Header************************************
// Method Name  : GetCDSCoilSelStatus
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CDS_COIL_SEL_STATUS CpqmCDS::GetCDSCoilSelStatus(
) const
{
    return m_pqmPmCoilSelCDSFlg ;
}

//************************************Method Header************************************
// Method Name  : GetPqmAcqStatic
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
PqmAcqStatic* CpqmCDS::GetPqmAcqStatic(
)
{
    return &m_PqmAcqStatic ;
}

//************************************Method Header************************************
// Method Name  : GetCDSDebugParam
// Author       : PATNI/ HEMANT
// Purpose      : Returns the coil location information for CDS protocol in scanning
//***********************************************************************************
void CpqmCDS::GetCDSDebugParam(
    COMPASS_Debugg_Param* f_compass_debug_param
) const  	//Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_03
{
    m_cds_debug.GetCDSDebugParam(f_compass_debug_param);
}

//************************************Method Header************************************
// Method Name  : GetMagneticCenterPosForCoilSel
// Author       : PATNI/ HEMANT
// Purpose      : This function will return the magnetic field center for the
//                CDS protocol currently in scanning
//***********************************************************************************
float CpqmCDS::GetMagneticCenterPosForCoilSel(
) const   //Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_03
{
    LPCTSTR FUNC_NAME = _T("CpqmCDS::GetMagneticCenterPosForCoilSel");
    CPqmProtocol* l_current_protocol = m_cds_debug.GetDebugProtocol();

    if (!l_current_protocol) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_current_protocol is NULL"));
        ASSERT(FALSE) ;
        return 0.0;
    }

    return PqmCdsGetCenterPos(l_current_protocol->GetPqmPrMode()->GetCouchPos());
}

//************************************Method Header************************************
// Method Name  : SetCDSCoilSelStatus
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CpqmCDS::SetCDSCoilSelStatus(
    const CDS_COIL_SEL_STATUS f_cds_coil_sel_status
)
{
    m_pqmPmCoilSelCDSFlg = f_cds_coil_sel_status;
}

int CpqmCDS::GetCoilLableSize()const
{

    return m_cds_param.m_couchLabelSize ;
}


//************************************Method Header************************************
// Method Name  : PqmCdsGetCenterPos
// Author       : PATNI/ HEMANT
// Purpose      : Sleeping position information from the Position field center obtained.
//                Position information is a float 1 / 2 to seek accuracy.
//                Return value is : The field center Positon (float, Accuracy: 1 / 2)
//***********************************************************************************
float CpqmCDS::PqmCdsGetCenterPos(
    float	f_couch
) const  	//Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_03
{
    LPCTSTR FUNC_NAME = _T("CpqmCDS::PqmCdsGetCenterPos");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    int		l_center = -1;
    /* Whether there is any field center position bed */
    l_center = PqmCdsChangeCouch(f_couch);

    if (l_center < 0) {
        return 0.0;			/* Field center is outside the scope of the sleeper */
    }

    /* If this value is less that 1*/
    float	l_pos1Lim = (float)m_cds_param.m_couchEndMargin + (float)(m_cds_param.m_couchLabelSize * (3.0 / 4.0));

    if (l_center < l_pos1Lim) {
        return 1.0;
    }

    /* Position to which you will find
       Position is 1-15
       1 and 15 and 15.5 are excluded because
       15 * 2-3-1 = 26
    */

    //Correcting Vantga error
    //for(int i = 0; i < 26; i++)
    float l_limit = 0.0f;

    for (int i = 0; i <= 26; i++) {
        l_limit = l_pos1Lim + (float)((m_cds_param.m_couchLabelSize / 2.0) * (i + 1));

        if (l_center < l_limit) {
            return (float)(1.5 + i / 2.0);
        }
    }

    /*If this value up to 15 */
    float l_pos15Lim = (float)(m_cds_param.m_couchLabelSize * 15.0) + (float)m_cds_param.m_couchEndMargin + (float)(m_cds_param.m_couchLabelSize / 4.0);

    if (l_center < l_pos15Lim) {
        return 15.0 ;
    }

    /* Field center is outside the scope of the sleeper*/
    return 0.0;
}

//************************************Method Header************************************
// Method Name  : OnCDSDebugCompleted
// Author       : PATNI/ HEMANT
// Purpose      : Start process after CDS debug completion
//***********************************************************************************
void CpqmCDS::OnCDSDebugCompleted(
)const
{
    LPCTSTR FUNC_NAME = _T("CpqmCDS::OnCDSDebugCompleted");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME , _T("Trace"));

    if (!m_pqm) {
        return ;
    }

    CPqmPmAcqman* l_pqmpmacqman = m_pqm->GetAcqManager();
    CPqmProtocol* l_current_protocol = m_cds_debug.GetDebugProtocol();

    if (l_pqmpmacqman && l_current_protocol) {
        l_pqmpmacqman->PqmAcqAutoCoilSet(l_current_protocol);
    }
}

//************************************Method Header************************************
// Method Name  : pqmCdsChangeCoordinate
// Author       : PATNI/ HEMANT
// Purpose      : Converting the f_location (relative to magnetic field center) to the
//                couch tip relative coordinate
//***********************************************************************************
int CpqmCDS::pqmCdsChangeCoordinate(
    const int f_location,
    const float f_couch
)const
{
    int l_center = PqmCdsChangeCouch(f_couch);
    return f_location + l_center;
}

//************************************Method Header************************************
// Method Name  : pqmCdsSearchOverlapCoil
// Author       : PATNI/ HEMANT
// Purpose      : Find and return the number of overlapping coils.
//                Port of coils that overlap ret1, ret2 put, respectively.
//                Add the length of the overlap in dist. Overlap even if this time
//                If the coil is put into the distance dist.
//***********************************************************************************
void CpqmCDS::pqmCdsSearchOverlapCoil(
    CpqmCDSInfo* f_cdsInfo,
    const int f_section_p,    /* Search for overlapping coil coil */
    int* f_left,        /* Port of overlapping coils (left) */
    int32_t* f_distl,       /* Distance that overlap */
    int* f_right,       /* Port of overlapping coils (right) */
    int32_t* f_distR         /* Distance that overlap */
)const
{
    LPCTSTR FUNC_NAME = _T("CpqmCDS::pqmCdsSearchOverlapCoil");
    PQM_SCOPED_TRACE();

    //+Patni-Hemant/2010Mar4/Added/ACE-Phase#3/CDS Requirement
    if (!f_left || !f_distl || !f_right || !f_distR || !f_cdsInfo) {
        return;
    }

    //-Patni-Hemant/2010Mar4/Added/ACE-Phase#3/CDS Requirement

    //+Patni-Hemant/2010Aug02/Added/TMSC-REDMINE-520/CDS Abort Issue
    CString l_message_str(_T(""));
    l_message_str.Format(_T("Check overlapping for port no : %d"), f_section_p);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message_str);

    l_message_str.Format(_T("Coil id : %x, detectedFlag : %d, location : %d"),
                         f_cdsInfo->m_coordinatetbl[f_section_p].coilID,
                         f_cdsInfo->m_coordinatetbl[f_section_p].detectedFlag,
                         f_cdsInfo->m_coordinatetbl[f_section_p].location
                        );
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message_str);
    //-Patni-Hemant/2010Aug02/Added/TMSC-REDMINE-520/CDS Abort Issue

    /* Initialization */
    *f_left	= -1;
    *f_distl	= 99999999;
    *f_right	= -1;
    *f_distR	= 99999999;

    float	l_point1 = 0.0f , l_point2 = 0.0f;
    int		l_dist = 0;

    for (int	i = ACQ_COILINFO_PORT_A; i < PQM_NUM_COIL_PORT; i++) {
        if (i == f_section_p || f_cdsInfo->m_coil_tbl[i] == NULL)	continue;

        if ((f_cdsInfo->m_coil_tbl[f_section_p]->coilLocation & 0x0f0) == 0x010 ||
            (f_cdsInfo->m_coil_tbl[i]->coilLocation & 0x0f0) == 0x010)
            continue;

        if (((f_cdsInfo->m_coil_tbl[f_section_p]->coilLocation & LOCATION_MASK)	==
             (f_cdsInfo->m_coil_tbl[i]->coilLocation & LOCATION_MASK))		&&	/* “¯‚¶ˆÊ’u */
            (f_cdsInfo->m_locationtbl[i].detectedFlag || f_cdsInfo->m_coordinatetbl[i].detectedFlag)) {
            if (f_cdsInfo->m_coordinatetbl[f_section_p].location >= f_cdsInfo->m_coordinatetbl[i].location) {
                /* ---i--- ---f_section_p--- */
                l_point1	= (float)f_cdsInfo->m_coordinatetbl[i].location + (float)(f_cdsInfo->m_coil_tbl[i]->size) / (float)2.0;
                l_point2	= (float)f_cdsInfo->m_coordinatetbl[f_section_p].location - (float)(f_cdsInfo->m_coil_tbl[f_section_p]->size) / (float)2.0;
                l_dist	= (int)(l_point2 - l_point1);

                if (*f_distl > l_dist) {
                    *f_distl	= l_dist;
                    *f_left	= i;
                }

            } else {
                /* ---f_section_p--- ---i--- */
                l_point1	= (float)f_cdsInfo->m_coordinatetbl[f_section_p].location + (float)(f_cdsInfo->m_coil_tbl[f_section_p]->size) / (float)2.0;
                l_point2	= (float)f_cdsInfo->m_coordinatetbl[i].location - (float)(f_cdsInfo->m_coil_tbl[i]->size) / (float)2.0;
                l_dist	= (int)(l_point2 - l_point1);

                if (*f_distR > l_dist) {
                    *f_distR	= l_dist;
                    *f_right	= i;
                }
            }
        }
    }

    l_message_str = _T("");
    l_message_str.Format(_T("Port of overlapping coils (left) = %d "), *f_left);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message_str);

    l_message_str.Format(_T("Distance that overlap (left) = %d "), *f_distl);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message_str);

    //Patni-Hemant/2010Aug02/Modified/TMSC-REDMINE-520/CDS Abort Issue
    l_message_str.Format(_T("Port of overlapping coils (right) = %d "), *f_right);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message_str);

    l_message_str.Format(_T("Distance that overlap (right) = %d "), *f_distR);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message_str);
}

//************************************Method Header************************************
// Method Name  : pqmPmCoilSelCdsTimerSetUp
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CpqmCDS::pqmPmCoilSelCdsTimerSetUp(
)const
{
    LPCTSTR FUNC_NAME = _T("CpqmCDS::pqmPmCoilSelCdsTimerSetUp");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    char* l_cds_timeout = getenv("CDS_TIMEOUT");

    int Cds_timeout = CDS_TIMEOUT_DEFAULT ;

    if (l_cds_timeout != NULL) {
        Cds_timeout =  30;
    }

    WNP_signal(SIGALRM, &CScanProcedure::pqmPmCoilSelCdsTimeOut);
    WNP_alarm(Cds_timeout) ;

    //	posixSignal(SIGALRM,pqmPmCoilSelCdsTimeOut);
    //	alarm( Cds_timeout ) ;
    //	DB_PRINTF("S",("Cds TIMER set(%d) \n",Cds_timeout));
    //	DB_FUNC_EXIT() ;
}

//************************************Method Header************************************
// Method Name  : pqmPmCoilSelCdsTimerReset
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CpqmCDS::pqmPmCoilSelCdsTimerReset(
)
{
    LPCTSTR FUNC_NAME = _T("CpqmCDS::pqmPmCoilSelCdsTimerReset");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    WNP_alarm(0) ;		/* TIMER RESET */
    WNP_signal(SIGALRM, SIG_DFL) ;

    //DB_PRINTF("S",("Cds TIMER Reset\n"));

    m_pqmPmCoilSelCDSFlg = CDS_NONE;

    //alarm( 0 ) ;		/* TIMER RESET */
    //posixSignal(SIGALRM,SIG_DFL) ;
    //DB_FUNC_EXIT() ;
}

//************************************Method Header************************************
// Method Name  : pqmCdsSort
// Author       : PATNI/ HEMANT
// Purpose      : Priority (closer to center field) in order to sort
//***********************************************************************************
void CpqmCDS::pqmCdsSort(
    int* f_index,               /* Array to sort (cdsinfo the index array) */
    int32_t* f_locationtbl,     /* This value is lower priority high */
    const int f_num                   /* Index number */
)
{
    if (!f_index) {
        return ;
    }

    int	i = 0, j = 0, w = 0;

    /*Sorted by priority */
    for (i = 0; i < f_num - 1; i++) {
        for (j = f_num - 1; j > i; j--) {
            if (f_index[j] != -1 && f_index[j - 1] != -1) {
                if (ABSF(f_locationtbl[f_index[j - 1]]) > ABSF(f_locationtbl[f_index[j]])) {
                    w			= f_index[j];
                    f_index[j]	= f_index[j - 1];
                    f_index[j - 1]	= w;
                }
            }
        }
    }
}


//************************************Method Header************************************
// Method Name  : GetCDSDebug
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CpqmCDS::CanIgnoreCDSExecution(
    CPqmProtocol* f_current_protocol,
    CPqmProtocol* f_src_protocol,
    int& f_copy_coil_from,
    const bool f_check_for_fixed_coils /*= false*/
) const
{


    LPCTSTR FUNC_NAME = _T("CpqmCDS::CanIgnoreCDSExecution");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    f_copy_coil_from = -1;

    CPQMStudy* l_study = m_pqm->GetStudy();
    CPqmProtocol* l_prev_protocol = l_study->GetPrevItemFromPos(f_current_protocol);

    if ((NULL == f_src_protocol) || (NULL == l_prev_protocol)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Error Occured. l_prev_protocol or f_src_protocol should not be NULL"));
        return false;
    }

    if (l_prev_protocol->GetCDSInfo()->m_cds_flag <= 0) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("prev_protocol COMPASS = OFF"));
        return false;
    }

    const float l_couch_pos_diff_with_prev = ABSF(f_current_protocol->GetPqmPrMode()->GetCouchPos() - l_prev_protocol->GetPqmPrMode()->GetCouchPos());

    if (0.0 != l_couch_pos_diff_with_prev) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Prev Protocol's couch diff is NOT ZERO"));
        return false;
    }

    CpqmCDSInfo* l_prev_prot_cdsinfo = l_prev_protocol->GetCDSInfo();
    CpqmCDSInfo* l_curr_prot_cdsinfo = f_current_protocol->GetCDSInfo();

    for (int i = ACQ_COILINFO_PORT_A; i < PQM_NUM_COIL_PORT; i++) {
        if (l_prev_prot_cdsinfo->m_coordinatetbl[i].coilID != l_curr_prot_cdsinfo->m_coordinatetbl[i].coilID) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Connected Coils are not same for Current Protocol and Previous Protocol"));
            CPQMDebugInfo::GetInstance(m_pqm)->PqmDebugAcqMonitor(_T("Current and Previous Protocol's Connected coils are not same !!!"));
            return false;
        }
    }


    CString str;
    str.Format(_T("Prev protocol (position=%d, Acqorder=%d)"),
               l_prev_protocol->GetPosition(), l_prev_protocol->GetProtocol());
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str);
    CPQMDebugInfo::GetInstance(m_pqm)->PqmDebugAcqMonitor(str);


    str.Format(_T("Reusable CDS protocol (position=%d, Acqorder=%d)"),
               f_src_protocol->GetPosition(), f_src_protocol->GetProtocol());
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str);
    CPQMDebugInfo::GetInstance(m_pqm)->PqmDebugAcqMonitor(str);

    if ((l_prev_prot_cdsinfo->m_cds_status & VFC_CDS_STATUS_SCAN)	&&
        !(l_prev_prot_cdsinfo->m_cds_status & VFC_CDS_STATUS_USE_PREVIOUS)) {

        //CDS was executed on prev protocol.
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CDS was executed on prev protocol."));

        if (f_src_protocol != l_prev_protocol) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_src_protocol != l_prev_protocol"));
            return false;
        }

    } else {
        //CDS was NOT executed on prev protocol.
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CDS was NOT executed on prev protocol."));

        if (f_src_protocol->GetProtocol() != l_prev_prot_cdsinfo->m_src_protocol) {

            if (!(f_check_for_fixed_coils && (f_src_protocol == l_prev_protocol))) {

                str.Format(_T("f_src_protocol Acqorder is not matching with Previous protocol's reused CDS protocol(Acqorder=%d)"), l_prev_prot_cdsinfo->m_src_protocol);
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str);
                return false;
            }
        }
    }

    f_copy_coil_from = l_prev_protocol->GetProtocol();

    return true;
}


//************************************Method Header************************************
// Method Name  : GetCDSDebug
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CpqmCDSDebug* CpqmCDS::GetCDSDebug(
)
{
    return &m_cds_debug;
}

//************************************Method Header************************************
// Method Name  : GetCDSRange
// Author       : PATNI/ HEMANT
// Purpose      : Returns the latest cds magnetic field range
//***********************************************************************************
int CpqmCDS::GetCDSRange(
) const
{
    //CDS selection coil (the distance from the center field) V9.50
    return m_compass_setting_param.m_dist_from_magnetic_center.m_value ;
}

//************************************Method Header************************************
// Method Name  : GetCDSRatio
// Author       : PATNI/ HEMANT
// Purpose      : Returns the CDS ratio / coverage for AS-COMPASs setting dialog
//                It is used while coil auto setting
//***********************************************************************************
int CpqmCDS::GetCDSRatio(
) const
{
    //CDS rate sensitivity of the coil to coil selection V9.50
    return m_compass_setting_param.m_min_coverage_for_section.m_value ;
}

//************************************Method Header************************************
// Method Name  : SetCDSDebugFlag
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CpqmCDS::SetCDSDebugFlag(
    const bool f_cdsdebug_flag
)
{
    m_cds_debug.SetCDSDebugFlag(f_cdsdebug_flag) ;
}






































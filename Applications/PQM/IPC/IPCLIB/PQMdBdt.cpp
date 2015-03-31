//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-780
//Redmine-781
//Redmine-782
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMAdapter.cpp
 *  Overview: Implementation of CPQMAdapter class.
 *  Last Modified: 2008/5/30 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2008/8/15 12:00:00     SM3 Baselined code

 *****************************************************************************/
#include "stdafx.h"
#include "PQMdBdt.h"

#include <math.h>		//IR-171
#include <ErrorDisp/ErrorDispConsts.h>
extern "C" {
#include <tami/libAcq/iecEdition.h>
    extern status_t gtLoadSysParams(GtSysParamT*);
}

//Patni-Rajendra/2011Mar29/Modified/TMSC-REDMINE-773
#include <libStudyManager/Log/ScopedFuncInOutLog.h>

#include <PQM/PqmUtilities.h> // Added By KT/ACE-2/15-06-2009/Multilingualisation
#include <PQM/PQMLogMgr.h>
#include <PQM/resource.h>
#include "ILicenseInfo.h"
#include "IObjectManager.h"
#include "pqmpmacqman.h"
#include "scanprocedure.h"
#include "VarFieldHandle.h" //TMSC-Tanoue/2010Aug25/MVC008345
#include "PqmProtocol.h"		//Patni/2011Apr07/Added/V2.0/IR-181_NFD1_Prod_Req_Part1
#include "pqm.h"
#include "pqmAMB.h"

using namespace MPlus::EH;

//****************************Method Header************************************
//Method Name   : CPQMdBdt()
//Author        : PATNI
//Purpose       :
//*****************************************************************************
CPQMdBdt::CPQMdBdt(
    const int f_mode,
    ILicenseInfo* f_licenseinfo,
    IObjectManager* f_objectmanager
): m_license_info(f_licenseinfo),
    m_obj_manager(f_objectmanager)
{
}

//****************************Method Header************************************
//Method Name   : ~CPQMdBdt()
//Author        : PATNI
//Purpose       :
//*****************************************************************************
CPQMdBdt::~CPQMdBdt(
)
{
}

//****************************Method Header************************************
//Method Name   : IECCheck
//Author        : PATNI
//Purpose       :
//*****************************************************************************
status_t CPQMdBdt::IECCheck(
    CVarFieldHandle* const f_vf_handle ,
    CPqmProtocol*  obj, const bool f_is_prescan_done
)
{
    LPCTSTR FUNC_NAME = _T("CPQMdBdt::IECCheck");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (!f_vf_handle || !obj) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("input argument is NULL"));
        return E_ERROR;
    }

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_obj_manager == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_obj_manager pointer is NULL"));
        return E_ERROR;
    }

    CPqmPmAcqman* l_pqm_pm_acqman = m_obj_manager->GetAcqManager();

    if (l_pqm_pm_acqman == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqm_pm_acqman pointer is NULL"));
        return E_ERROR;
    }



    if (!l_pqm_pm_acqman->CalcSARControlAndCheckSAR(l_pqm_pm_acqman->GetProtocol(), false, f_is_prescan_done)) {

        return E_ERROR;
    }

    char	dBdtMsg[1024] = {0} , sarMsg[1024] = {0};
    memset(dBdtMsg, NULL , sizeof(dBdtMsg)) ;
    memset(sarMsg, NULL , sizeof(sarMsg)) ;

    status_t	l_sts_dBdt = Check(f_vf_handle, obj, dBdtMsg);

    int32_t ver = 0, rev = 0;
    SarSdbGetSarVersion(&ver, &rev);

    /* Saturnの場合 */

    status_t l_sts_sar = PQM_SAR_OK;

    /* IEC SAR libsar 20060921 */
    if (ver == SAR_VERSION_VANTAGE) { /* PqmIsSaturnCoilSystem */
        l_sts_sar = IECCheckSAR(f_vf_handle, obj, sarMsg);

    } else if (ver == SAR_VERSION_ATLAS) {
    } else if (ver == SAR_VERSION_VENUS) {
    } else if (ver == SAR_VERSION_TITAN) {
    } else {
        //fxMessageBox(_T("ERR_VER_SARDB"),MB_SYSTEMMODAL);

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_VER_SARDB")),
            _T("PQMdBdt.cpp"), __LINE__, ERROR_LOG_CRUCIAL1 ,
            ERR_ID, APP_NAME);
        CString l_sarstat(_T(""));
        l_sarstat.Format(_T("libSAR Error Status = %d"), obj->GetSARStat());

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_sarstat);
    }

    CPQMStudy* l_study = m_obj_manager->GetStudy();

    if (l_study == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_study pointer is NULL"));
        return E_ERROR;
    }

    sarInfo_t* l_sar_info = obj->GetSarInfo();
    int l_sar_ope_mode = -1;

    if (l_sar_info) {
        l_sar_ope_mode = l_study->GetPqm()->GetSARControlLicense() ? l_sar_info->sarControl.ope_mode : l_sar_info->ope_mode;
    }

    if (l_sar_ope_mode != IEC_SAR_NORMAL_MODE) {
        if (l_study->GetPqm()->GetSARControlLicense()) {
            int l_active_for_sarctrl = obj->GetSarInfo()->sarControl.active;
            sprintf(sarMsg , "SAR   %.3f W/kg \n" , obj->GetSarInfo()->sarControl.tsarControl[l_active_for_sarctrl].sar_6min);

        } else {
            sprintf(sarMsg , "SAR   %.3f W/kg \n" , obj->GetSarInfo()->tsar[obj->GetSarInfo()->active].sar);
        }
    }

    //+Patni-PJS/2009Nov1/Commented/Code Review

    /*if (m_mode == K_EZSCAN) {*/	/* EasyScan*/
    /*if(l_sts_sar != PQM_IEC_SAR_NORMAL)
    {
        //	    PqmUiErrorDialog(PqmUiFetchLiteral("ERR_SAR"),ERR_ERROR);
        //AfxMessageBox(_T("ERR_SARDB"),MB_SYSTEMMODAL);
    	CString temp_string = CPqmUtilities::GetMultiLingualString(_T("IDS_ERR_SARDB"));    // Added By KT/ACE-2/15-06-2009/Multilingualisation
        CPQMLogMgr::GetInstance()->DisplayError(
           // _T("ERR_SARDB"), // Added By KT/ACE-2/15-06-2009/Multilingualisation
    	     temp_string,      // Added By KT/ACE-2/15-06-2009/Multilingualisation
            _T("PQMdBdt.cpp"),__LINE__,ERROR_LOG_CRUCIAL1 ,
            ERR_ID,APP_NAME);
        CString l_sarstat;
        l_sarstat.Format(_T("libSAR Error Status = %d"),obj->GetSARStat());

        CPQMLogMgr::GetInstance()->WriteTraceLog(DOMAIN_NAME,
            USER_FUNC_MARKER,
            _T("CPQMdBdt::IECCheck"),
            l_sarstat);

        PqmIECScanCancel();
        l_sts = E_ERROR;
        //return l_sts;
    }
    }*/
    /*  dBDt/SAR */
    /*else */
    //-Patni-PJS/2009Nov1/Commented/Code Review

    status_t l_sts = E_NO_ERROR ;

    if (l_sts_dBdt != PQM_DBDT_NORMAL
        ||   l_sar_ope_mode != IEC_SAR_NORMAL_MODE
        ||   l_sts_sar != PQM_IEC_SAR_NORMAL
       ) {
        if (l_sar_ope_mode != IEC_SAR_NORMAL_MODE) {
            /* Saturn pqmdBdtErrorInSaturn*/
            //+ Patni-PJS/2009Jun13/Added/JFT# 252/IR(5)
            //ErrorInSaturn( l_sts_dBdt, dBdtMsg, obj->GetSarInfo()->ope_mode, sarMsg );
            //l_sts = E_ERROR;
            //Patni-HAR/2010Feb08/Modified/MSA0251-00125
            l_sts = ErrorInSaturn(l_sts_dBdt, dBdtMsg, l_sar_ope_mode, sarMsg, obj);

            if (l_sts) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ErrorInSaturn return true"));
                l_sts = E_NO_ERROR;

            } else {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ErrorInSaturn return false"));
                l_sts = E_ERROR;
            }

            //- Patni-PJS/2009Jun13/Added/JFT# 252/IR(5)

        } else {
            //Patni-HAR/2010Feb08/Modified/MSA0251-00125
            bool ret = Error(l_sts_dBdt, dBdtMsg, l_sts_sar, sarMsg, obj);

            if (ret == true) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Error return true"));
                l_sts = E_NO_ERROR;

            } else {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Error return false"));
                l_sts = E_ERROR;
            }
        }
    }

    return l_sts ;
}

//****************************Method Header************************************
//Method Name   : dBdtCalc
//Author        : PATNI
//Purpose       :
//*****************************************************************************
//+Patni-HAR-SS/2010Feb09/Modified/MSA0251-00125
status_t CPQMdBdt::dBdtCalc(
    CVarFieldHandle* const f_vf_handle,
    CPqmProtocol*  f_obj
)
{
    LPCTSTR FUNC_NAME = _T("CPQMdBdt::dBdtCalc");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    flt32_t     l_dbdt_value[2] = {0.0};
    //Patni-HAR-SS/2010Feb09/Modified/MSA0251-00125
    status_t l_result = E_NO_ERROR;

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_obj_manager == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_obj_manager pointer is NULL"));
        //Patni-HAR-SS/2010Feb09/Modified/MSA0251-00125
        return E_ERROR ;
    }

    CPqmPmAcqman* l_pqm_pm_acqman = m_obj_manager->GetAcqManager();

    if (l_pqm_pm_acqman == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqm_pm_acqman pointer is NULL"));
        //Patni-HAR-SS/2010Feb09/Modified/MSA0251-00125
        return E_ERROR;
    }

    CScanProcedure*     l_scan_proc = l_pqm_pm_acqman->GetScanProcedure();

    if (l_scan_proc == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_scan_proc pointer is NULL"));
        //Patni-HAR-SS/2010Feb09/Modified/MSA0251-00125
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    CPQMStudy* l_study = m_obj_manager->GetStudy();

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (l_study == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_study pointer is NULL"));
        //Patni-HAR-SS/2010Feb09/Modified/MSA0251-00125
        return E_ERROR;
    }

    if (!f_vf_handle || !f_obj) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("input argument is NULL"));
        //Patni-HAR-SS/2010Feb09/Modified/MSA0251-00125
        return E_ERROR;
    }

    l_dbdt_value[0]  = 0.0;
    l_dbdt_value[1]  = 0.0;

    if (m_license_info->GetIECEdition() == VFC_DBDT_IEC_EDITION_1995) {
        /* dBdt mode check  */
        switch (f_obj->GetdBdtMode()) {
            case  VFC_DBDT_HIGH2_CONTROLLED1:/*dBdt 2nd mode + CONTROL1 V6.00 */
            case  VFC_DBDT_HIGH2_CONTROLLED2:/*dBdt 2nd mode + CONTROL2 V6.00 */
            case  VFC_DBDT_HIGH2:/*dBdt 2nd mode */

                f_obj->SetdBdtControl(VFC_DBDT_NORMAL);
                f_obj->SetCurrentdBdtMode(VFC_DBDT_HIGH2);
                //+ Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display
                f_obj->SetdBdtModeString(DBDT_NORMAL_LABEL);
                //+ Patni-PJS/2009Jun05/Modify/JFT# 251,251,251,IR(5)
                //m_obj_manager->WriteToUI(PQM_MSG_UPDATE_PAGER_AREA);
                //- Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display

                //Patni-HAR/2010Feb08/Modified/MSA0251-00125
                Error(PQM_DBDT_LICENSE_ERR , NULL, PQM_IEC_SAR_NORMAL, NULL, f_obj) ;
                //Patni-HAR-SS/2010Feb09/Modified/MSA0251-00125
                l_result = E_ERROR;
                break ;

            case  VFC_DBDT_HIGH1_CONTROLLED1:/*dBdt 1st mode + CONTROL1 V6.00 */
            case  VFC_DBDT_HIGH1_CONTROLLED2:/*dBdt 1st mode + CONTROL2 V6.00 */
            case  VFC_DBDT_HIGH1:/*dBdt 1st mode */

                f_obj->SetdBdtControl(VFC_DBDT_NORMAL);
                f_obj->SetCurrentdBdtMode(VFC_DBDT_HIGH1);
                //+ Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display
                f_obj->SetdBdtModeString(DBDT_NORMAL_LABEL);
                //+ Patni-PJS/2009Jun05/Modify/JFT# 251,251,251,IR(5)
                //m_obj_manager->WriteToUI(PQM_MSG_UPDATE_PAGER_AREA);
                //- Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display

                if (m_license_info->GetdBdtLicense() == FALSE) {
                    //Patni-HAR/2010Feb08/Modified/MSA0251-00125
                    Error(PQM_DBDT_LICENSE_ERR , NULL, PQM_IEC_SAR_NORMAL, NULL, f_obj) ;
                    //Patni-HAR-SS/2010Feb09/Modified/MSA0251-00125
                    l_result = E_ERROR;

                } else {
                    HighMode(l_study->GetVfDBHandle(), f_obj) ;
                }

                break ;

            case VFC_DBDT_CONTROLLED1:
            case VFC_DBDT_CONTROLLED2:
            default:

                f_obj->SetdBdtControl(VFC_DBDT_NORMAL);
                //+ Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display
                f_obj->SetdBdtModeString(DBDT_NORMAL_LABEL);
                //+ Patni-PJS/2009Jun05/Modify/JFT# 251,251,251,IR(5)
                //m_obj_manager->WriteToUI(PQM_MSG_UPDATE_PAGER_AREA);
                //- Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display

                f_obj->SetCurrentdBdtMode(f_obj->GetdBdtMode());
                l_study->PutdBdtMode(l_study->GetVfDBHandle(),  f_obj->GetProtocol(), VFC_DBDT_IEC_OPERATING_NORMAL, l_dbdt_value) ;
                f_obj->SetdBdtOperation(VFC_DBDT_IEC_OPERATING_NORMAL);
                f_obj->SetdBdtValue(0, 0.0);
                f_obj->SetdBdtValue(1, 0.0);

                break;
        }

    } else if (m_license_info->GetIECEdition() == VFC_DBDT_IEC_EDITION_2002) {
        //l_protocol->SetdBdtMode2002(0);
        switch (f_obj->GetdBdtMode2002()) {

            case VFC_DBDT_CONTROLLED1:
            case VFC_DBDT_HIGH1_CONTROLLED1:/*dBdt 1st mode + CONTROL1 V6.00 */
            case VFC_DBDT_HIGH2_CONTROLLED1:/*dBdt 2nd mode + CONTROL1 V6.00 */

                f_obj->SetdBdtControl(VFC_DBDT_CONTROLLED1);
                break;

            case VFC_DBDT_CONTROLLED2:/* 自主規制モード2 */
            case VFC_DBDT_HIGH1_CONTROLLED2:/*dBdt 1st mode + CONTROL2 V6.00 */
            case VFC_DBDT_HIGH2_CONTROLLED2:/*dBdt 2nd mode + CONTROL2 V6.00 */

                f_obj->SetdBdtControl(VFC_DBDT_CONTROLLED2);
                break;

            default:

                f_obj->SetdBdtControl(VFC_DBDT_NORMAL);
                //+ Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display
                f_obj->SetdBdtModeString(DBDT_NORMAL_LABEL);
                //+ Patni-PJS/2009Jun05/Modify/JFT# 251,251,251,IR(5)
                //+Patni-NP/2009Sep6/Added/MVC004766
                f_obj->SetdBdtOperation(VFC_DBDT_IEC_OPERATING_NORMAL);
                //-Patni-NP/2009Sep6/Added/MVC004766

                //m_obj_manager->WriteToUI(PQM_MSG_UPDATE_PAGER_AREA);
                //- Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display

                break;
        }

        /* dBdt mode Check */
        switch (f_obj->GetdBdtMode2002()) {

            case VFC_DBDT_HIGH2_CONTROLLED1:/*dBdt 2nd mode + CONTROL1 V6.00 */
            case VFC_DBDT_HIGH2_CONTROLLED2:/*dBdt 2nd mode + CONTROL2 V6.00 */
            case VFC_DBDT_HIGH2:/*dBdt 2nd Mode */

                f_obj->SetCurrentdBdtMode(VFC_DBDT_HIGH2);

                if (m_license_info->GetdBdt2002_2_License() == FALSE) {
                    //Patni-HAR/2010Feb08/Modified/MSA0251-00125
                    Error(PQM_DBDT_LICENSE_ERR , NULL, PQM_IEC_SAR_NORMAL, NULL, f_obj) ;
                    //Patni-HAR-SS/2010Feb09/Modified/MSA0251-00125
                    l_result = E_ERROR;

                } else {
                    l_study->PutdBdtMode(l_study->GetVfDBHandle(),  f_obj->GetProtocol(), VFC_DBDT_IEC_OPERATING_2ND_CONTROLLED, l_dbdt_value) ;
                    f_obj->SetdBdtOperation(VFC_DBDT_IEC_OPERATING_2ND_CONTROLLED);
                    f_obj->SetdBdtValue(0, f_obj->GetdBdtValue(0));
                    f_obj->SetdBdtValue(1, f_obj->GetdBdtValue(0));
                    //+ Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display
                    f_obj->SetdBdtModeString(DBDT_MODE2_LABEL);
                    //+ Patni-PJS/2009Jun05/Modify/JFT# 251,251,251,IR(5)
                    //m_obj_manager->WriteToUI(PQM_MSG_UPDATE_PAGER_AREA);
                    //- Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display
                }

                break ;

            case VFC_DBDT_HIGH1_CONTROLLED1:/*dBdt 1st mode + CONTROL1 V6.00 */
            case VFC_DBDT_HIGH1_CONTROLLED2:/*dBdt 1st mode + CONTROL2 V6.00 */
            case VFC_DBDT_HIGH1:/*dBdt 1st Mode */

                f_obj->SetCurrentdBdtMode(VFC_DBDT_HIGH1);

                if (m_license_info->GetdBdt2002_1_License() == FALSE &&
                    m_license_info->GetdBdt2002_2_License() == FALSE) {
                    //Patni-HAR/2010Feb08/Modified/MSA0251-00125
                    Error(PQM_DBDT_LICENSE_ERR , NULL, PQM_IEC_SAR_NORMAL, NULL, f_obj) ;
                    //Patni-HAR-SS/2010Feb09/Modified/MSA0251-00125
                    l_result = E_ERROR;

                } else {
                    l_study->PutdBdtMode(l_study->GetVfDBHandle(), f_obj->GetProtocol(), VFC_DBDT_IEC_OPERATING_1ST_CONTROLLED, l_dbdt_value) ;
                    f_obj->SetdBdtOperation(VFC_DBDT_IEC_OPERATING_1ST_CONTROLLED);
                    f_obj->SetdBdtValue(0, f_obj->GetdBdtValue(0));
                    f_obj->SetdBdtValue(1, f_obj->GetdBdtValue(0));
                    //+ Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display
                    f_obj->SetdBdtModeString(DBDT_MODE1_LABEL);
                    //+ Patni-PJS/2009Jun05/Modify/JFT# 251,251,251,IR(5)
                    //m_obj_manager->WriteToUI(PQM_MSG_UPDATE_PAGER_AREA);
                    //- Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display
                }

                break ;

            case VFC_DBDT_CONTROLLED1:/* 自主規制モード1 */
            case VFC_DBDT_CONTROLLED2:/* 自主規制モード2 */
            default:

                f_obj->SetCurrentdBdtMode(f_obj->GetdBdtMode2002());
                l_study->PutdBdtMode(l_study->GetVfDBHandle(), f_obj->GetProtocol(), VFC_DBDT_IEC_OPERATING_NORMAL, l_dbdt_value) ;
                f_obj->SetdBdtOperation(VFC_DBDT_IEC_OPERATING_NORMAL);
                f_obj->SetdBdtValue(0, 0.0);
                f_obj->SetdBdtValue(1, 0.0);

                break;
        }
    }

    DB_FUNC_EXIT() ;

    return l_result;
}

//****************************Method Header************************************
//Method Name   : PqmIECScanCancel
//Author        : PATNI
//Purpose       :
//*****************************************************************************
void	CPQMdBdt::PqmIECScanCancel()
{
    LPCTSTR FUNC_NAME = _T("CPQMdBdt::PqmIECScanCancel");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_obj_manager == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_obj_manager pointer is NULL"));
        return;
    }

    CPqmPmAcqman* l_pqm_pm_acqman = m_obj_manager->GetAcqManager();

    if (l_pqm_pm_acqman == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqm_pm_acqman pointer is NULL"));
        return ;
    }

    CScanProcedure*     l_scan_proc = l_pqm_pm_acqman->GetScanProcedure();

    if (l_scan_proc == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_scan_proc pointer is NULL"));
        return ;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    CPqmProtocol*       l_protocol = l_scan_proc->GetScanProcProtocol();

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (l_protocol == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_protocol pointer is NULL"));
        return ;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    CPqmPmAcqman*		l_acqman = l_scan_proc->GetAcqManager();

    if (l_acqman == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_acqman pointer is NULL"));
        return ;
    }

    if (l_protocol->GetAcqStatus() != C_PR_STATUS_WAIT) {

        //+ Patni-MSN/2009Mar26/Added/PSP- According to Base code
        l_scan_proc->AcquisitionScanAbort();
        //- Patni-MSN/2009Mar26/Added/PSP- According to Base code
        //Defect 84 - Fixed by Manishkumar
        //Set the value of ACQ_STATUS of protocol through object of PQMStudy.
        //l_protocol->SetAcqStatus(C_PR_STATUS_WAIT);
        m_obj_manager->GetStudy()->SetAcqStatus(l_protocol, C_PR_STATUS_WAIT);

        if (l_scan_proc->GetRequest())
            l_acqman->AcqDeleteLockFile();
    }

    m_obj_manager->GetStudy()->SetScanModeOfHead();
    l_scan_proc->ResetCurrentRequest();
    m_obj_manager->WriteToUI(PQM_MSG_ABORT_WAIT);
    l_acqman->GetPqm()->GetAMB()->PqmPmAbortAmb();
}

//****************************Method Header************************************
//Method Name   : ErrorInSaturn
//Author        : PATNI
//Purpose       :
//*****************************************************************************
//+ Patni-PJS/2009Jun13/Modify/JFT# 252/IR(5)
//void	CPQMdBdt::ErrorInSaturn(int	f_dbdtcode,
//Patni-HAR/2010Feb08/Modified/MSA0251-00125
bool 	CPQMdBdt::ErrorInSaturn(int	f_dbdtcode,
                                char* f_dbdtdata,
                                int	f_ope_mode,
                                char* f_sardata,
                                CPqmProtocol* f_protocol_object
                             )
{
    int l_sarcode = 0; //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    //+ Patni-PJS/2009Jun13/Modify/JFT# 252/IR(5)
    //DB_FUNC_ENTER("[ErrorInSaturn]:");
    //- Patni-PJS/2009Jun13/Modify/JFT# 252/IR(5)

    switch (f_ope_mode) {
        case IEC_SAR_NORMAL_MODE:
        default:
            l_sarcode = PQM_IEC_SAR_NORMAL;
            break;

        case IEC_SAR_1ST_MODE:
            l_sarcode = PQM_IEC_SAR_1ST_CONTROLLED;
            break;

        case IEC_SAR_2ND_MODE:
            l_sarcode = PQM_IEC_SAR_2ND_CONTROLLED;
            break;
    }

    //+ Patni-PJS/2009Jun13/Modify/JFT# 252/IR(5)
    //Error( f_dbdtcode, f_dbdtdata, l_sarcode, f_sardata );
    //Patni-HAR/2010Feb08/Modified/MSA0251-00125

    return Error(f_dbdtcode, f_dbdtdata, l_sarcode, f_sardata, f_protocol_object) ; //l_return_stat;
    //DB_FUNC_EXIT() ;
    //- Patni-PJS/2009Jun13/Modify/JFT# 252/IR(5)
}

//****************************Method Header************************************
//Method Name   : IECCheckSAR
//Author        : PATNI
//Purpose       :
//*****************************************************************************
status_t CPQMdBdt::IECCheckSAR(
    CVarFieldHandle* const f_vf_handle,
    CPqmProtocol* obj,
    char* sarMsg
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMdBdt::IECCheckSAR");

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Enter"));

    //+ Patni-PJS/2009Nov1/Modified/Code Review
    if (!f_vf_handle || !obj || !sarMsg) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("input argument is NULL"));
        return E_ERROR;
    }

    //-Patni-PJS/2009Nov1/Modified/Code Review

    status_t	rtn = 0 ;//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    status_t sar_mode = SarIECJudgement(obj->GetSarInfo(), NULL) ;

    if (sar_mode == IEC_SAR_NORMAL_MODE) {
        rtn = PQM_IEC_SAR_NORMAL;
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("IEC_SAR_NORMAL_MODE "));

    } else if (sar_mode == IEC_SAR_1ST_MODE) {
        rtn = PQM_IEC_SAR_1ST_CONTROLLED ;
        sprintf(sarMsg , "SAR   %.3f W/kg \n" , obj->GetSarInfo()->tsar[sarTypeWholeBody].sar);

        if (m_license_info->GetSAR02_01_License() != TRUE) {
            rtn = PQM_IEC_SAR_LICENSE_ERR;
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PQM_IEC_SAR_LICENSE_ERR"));
        }

    } else {
        rtn = PQM_IEC_SAR_2ND_CONTROLLED ;
        sprintf(sarMsg , "SAR   %.3f W/kg \n" , obj->GetSarInfo()->tsar[sarTypeWholeBody].sar);

        if (m_license_info->GetSAR02_02_License() != TRUE) {
            rtn = PQM_IEC_SAR_LICENSE_ERR ;
        }

        rtn = PQM_IEC_SAR_LICENSE_ERR;
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PQM_IEC_SAR_LICENSE_ERR"));
    }

    return rtn ;
}

//****************************Method Header************************************
//Method Name   : Check
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int CPQMdBdt::Check(
    CVarFieldHandle* const f_vf_handle,
    CPqmProtocol*  f_obj,
    char* buf
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMdBdt::Check");
    PQM_SCOPED_TRACE();

    if (!f_vf_handle || !f_obj) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("input argument is NULL"));
        return E_ERROR;
    }

    status_t l_status = PQM_DBDT_NORMAL ;

    if (f_obj->GetCurrentdBdtMode()  == VFC_DBDT_CONTROLLED1 ||
        f_obj->GetdBdtControl()  == VFC_DBDT_CONTROLLED1) {

        l_status = CheckMode1(f_vf_handle, f_obj, buf);

    } else if (f_obj->GetCurrentdBdtMode()  == VFC_DBDT_CONTROLLED2 ||
               f_obj->GetdBdtControl()  == VFC_DBDT_CONTROLLED2) {
        l_status = CheckMode2(f_vf_handle, f_obj) ;
    }

    if (l_status == PQM_DBDT_NORMAL) {

        switch (f_obj->GetCurrentdBdtMode()) {
            case  VFC_DBDT_HIGH2:/* dBdt 2nd Mode */

                //+Patni-PJS/2009Oct15/Modified/MSA0248-00023
                if (((m_license_info->GetdBdtLicense() == FALSE) ||
                     (m_license_info->GetdBdt2002_1_License() == FALSE) ||
                     (m_license_info->GetdBdt2002_2_License() == FALSE))) {
                    l_status = PQM_DBDT_LICENSE_ERR ;

                } else {
                    if ((m_license_info->GetIECEdition() == VFC_DBDT_IEC_EDITION_1995)) {
                        l_status = PQM_DBDT_2ND_CONTROLLED_ERR ;

                    } else {
                        l_status = Check2ndMode(f_vf_handle, f_obj) ;
                    }
                }

                break ;

            case  VFC_DBDT_HIGH1:/* dBdt 1st Mode */

                if ((m_license_info->GetdBdtLicense() == FALSE) ||
                    (m_license_info->GetdBdt2002_1_License() == FALSE)) {
                    l_status = PQM_DBDT_LICENSE_ERR ;

                } else {
                    l_status = CheckHighMode(f_vf_handle, f_obj) ;
                }

                //-Patni-PJS/2009Oct15/Modified/MSA0248-00023
                break ;
        }
    }

    return l_status ;
}

//****************************Method Header************************************
//Method Name   : CheckMode2_RO
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int CPQMdBdt::CheckMode2RO(
    CVarFieldHandle* const f_vf_handle,
    const int f_prot,
    const int f_mode
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMdBdt::CheckMode2RO");

    if (!f_vf_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_vf_handle is NULL"));
        return E_ERROR;
    }

    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    /*double		l_so[3],l_pe[3],l_ro[3];
    double		l_epi_vec ;
    int32_t		l_limit2[3] ;
    flt32_t		l_epi_angle[2];
    status_t            l_status;
    int                 l_n , l_numdone  ;
    VfFieldSpec_t       l_fieldspec[5];
    VfPathElem_t        l_path[5];*/
    double		l_so[3] = {0.0f}, l_pe[3] = {0.0f}, l_ro[3] = {0.0f};
    int32_t		l_limit2[3] = {0l};
    flt32_t		l_epi_angle[2] = {0.0f};
    VfFieldSpec_t       l_fieldspec[5] = {0};
    VfPathElem_t        l_path[5] = {0};
    //- Patni-PJS/2009Apr25/Modify/CPP Test tool execution

    PQM_SCOPED_TRACE();

    l_path[0].subtree_name = SVN_ST_PROTOCOL;
    l_path[0].index = f_prot;
    l_path[1].subtree_name = SVN_ST_VISUALGROUP;
    l_path[1].index = 0;
    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(l_fieldspec, 0, sizeof(VfFieldSpec_t) * 5);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    int l_n = 0 ;

    /* V7.20 EPI Oblique angle */
    VFF_SET_ARGS(l_fieldspec[l_n], SVN_PLN_OBLIQUE_EPI_ANGLE_TABLE, VFT_FLT32,
                 sizeof(l_epi_angle), l_epi_angle, 0L);
    l_n++;

    if (f_mode == VFC_DBDT_IEC_EDITION_1995) {
        VFF_SET_ARGS(l_fieldspec[l_n], SVN_SEQ_DBDT_RO_DIR_LIMIT2, VFT_INT32,
                     sizeof(l_limit2), l_limit2, 0L);
        l_n++;

    } else {
        VFF_SET_ARGS(l_fieldspec[l_n], SVN_SEQ_DBDT_RO_DIR_LIMIT2_2002, VFT_INT32,
                     sizeof(l_limit2), l_limit2, 0L);
        l_n++;
    }

    int l_numdone = 0 ;
    status_t l_status = f_vf_handle->GetFields(l_path, SVD_ST_PROTOCOL, l_fieldspec, l_n, (int*) & l_numdone);

    if (l_status != E_NO_ERROR) {
        DB_FUNC_EXIT() ;
        return l_status;
    }

    if (l_fieldspec[0].status != E_NO_ERROR) {
        l_epi_angle[0] = l_epi_angle[1] = 0.0 ;
    }

    double		l_epi_vec = cos((l_epi_angle[0] * M_PI / 180.0)) - 0.000001;
    l_epi_vec = fabs(l_epi_vec);
    DB_PRINTF("D", ("l_limit2=[%d,%d,%d]\n", l_limit2[0], l_limit2[1], l_limit2[2]));
    DB_PRINTF("D", ("l_epi_angle=[%f,%f]\n", l_epi_angle[0], l_epi_angle[1]));
    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(l_fieldspec, 0, sizeof(VfFieldSpec_t) * 5);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    l_n = 0 ;

    f32vec3_t		l_phasevec , l_slicevec ;

    VFF_SET_ARGS(l_fieldspec[l_n], SVN_PLN_VG_PHASE_ORI_VECTOR, VFT_FLTVEC3,
                 sizeof(f32vec3_t)  , &l_phasevec , 0L);
    l_n++;

    VFF_SET_ARGS(l_fieldspec[l_n], SVN_PLN_VG_SLICE_ORI_VECTOR, VFT_FLTVEC3,
                 sizeof(f32vec3_t)  , &l_slicevec , 0L);
    l_n++;

    l_status = f_vf_handle->GetFields(l_path, SVD_ST_VISUALGROUP, l_fieldspec, l_n, (int*) & l_numdone);

    if (l_status != E_NO_ERROR || l_n != l_numdone) {
        DB_FUNC_EXIT() ;
        return E_ERROR;
    }

    l_so[0] = l_slicevec.x;
    l_so[1] = l_slicevec.y;
    l_so[2] = l_slicevec.z;
    l_pe[0] = l_phasevec.x;
    l_pe[1] = l_phasevec.y;
    l_pe[2] = l_phasevec.z;

    for (l_n = 0 ; l_n < 3 ; l_n++) {
        DB_PRINTF("D", ("l_slicevec=[%f] l_phasevec(%f)\n", l_so[l_n], l_pe[l_n]));
    }

    InsetCrossProduct(l_pe, l_so, l_ro);

    for (l_n = 0 ; l_n < 3 ; l_n++) {
        DB_PRINTF("D", ("l_slicevec(%f) l_phasevec(%f), readVec(%f), l_limit2(%d)\n",
                        l_so[l_n], l_pe[l_n], l_ro[l_n], l_limit2[l_n]));
    }

    l_status = E_ERROR ;

    if (fabs(l_ro[2]) >= l_epi_vec && l_limit2[2] == 1) {
        l_status = E_NO_ERROR ;

    } else if (fabs(l_ro[1]) >= l_epi_vec && l_limit2[1] == 1) {
        l_status = E_NO_ERROR ;

    } else if (fabs(l_ro[0]) >= l_epi_vec && l_limit2[0] == 1) {
        l_status = E_NO_ERROR ;
    }

    return l_status;
}

//****************************Method Header************************************
//Method Name   : CheckMode2
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int CPQMdBdt::CheckMode2(
    CVarFieldHandle* const f_vf_handle,
    CPqmProtocol*  f_obj
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMdBdt::CheckMode2");
    PQM_SCOPED_TRACE();

    status_t l_sts = PQM_DBDT_NORMAL ;

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (!f_vf_handle || !f_obj) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("input argument is NULL"));
        return E_ERROR;
    }

    if (m_license_info == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_license_info pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    if (CheckMode2RO(f_vf_handle, f_obj->GetProtocol(), m_license_info->GetIECEdition())) {
        l_sts = PQM_DBDT_CONTROLLED2_RO_ERR ;
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PQM_DBDT_CONTROLLED2_RO_ERR "));
    }

    return l_sts ;
}

//****************************Method Header************************************
//Method Name   : CheckMode1_FOV
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int CPQMdBdt::CheckMode1FOV(
    CVarFieldHandle* const f_vf_handle,
    const int f_prot,
    flt32_t* f_lim,
    const int f_mode
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMdBdt::CheckMode1FOV");
    PQM_SCOPED_TRACE();

    f32vec2_t  FOV , l_matrix ;

    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    /*
    flt32_t l_limit1 ;
    status_t            l_status;
    int                 l_n , l_numdone  ;
    VfFieldSpec_t       l_fieldspec[5];
    VfPathElem_t        l_path[5];
    */
    int                 l_n = 0, l_numdone = 0 ;
    VfFieldSpec_t       l_fieldspec[5] = {0};
    VfPathElem_t        l_path[5] = {0};
    //- Patni-PJS/2009Apr25/Modify/CPP Test tool execution

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (!f_vf_handle || !f_lim) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("input argument is NULL"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    *f_lim = 0 ;
    l_path[0].subtree_name = SVN_ST_PROTOCOL;
    l_path[0].index = f_prot;
    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(l_fieldspec, 0, sizeof(VfFieldSpec_t) * 5);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    l_n = 0 ;
    VFF_SET_ARGS(l_fieldspec[l_n], SVN_PLN_FOV, VFT_FLTVEC2,
                 sizeof(f32vec2_t)  , &FOV , 0L);
    l_n++;
    VFF_SET_ARGS(l_fieldspec[l_n], SVN_SEQ_ACQ_MATRIX, VFT_FLTVEC2,
                 sizeof(l_matrix), &l_matrix, 0L);
    l_n++;
    flt32_t l_limit1 = 0.0f;

    if (f_mode == VFC_DBDT_IEC_EDITION_1995) {

        VFF_SET_ARGS(l_fieldspec[l_n], SVN_SEQ_DBDT_RO_RES_LIMIT1, VFT_FLT32,
                     sizeof(l_limit1), &l_limit1, 0L);
        l_n++;

    } else {
        VFF_SET_ARGS(l_fieldspec[l_n], SVN_SEQ_DBDT_RO_RES_LIMIT1_2002, VFT_FLT32,
                     sizeof(l_limit1), &l_limit1, 0L);
        l_n++;
    }

    status_t l_status = f_vf_handle->GetFields(l_path, SVD_ST_PROTOCOL, l_fieldspec, l_n, (int*) & l_numdone);

    CString l_print_string(_T(""));
    l_print_string.Format(_T("l_matrix[%f,%f], FOV[%f,%f] l_limit1=%f"),
                          l_matrix.x, l_matrix.y, FOV.x, FOV.y, l_limit1);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_print_string);

    if (l_status == E_NO_ERROR || l_n == l_numdone) {
        if ((FOV.y / l_matrix.y) > (l_limit1)) {
            l_status = E_NO_ERROR;

        } else {
            l_status = E_ERROR ;
            *f_lim = (l_limit1 * l_matrix.y) ;		/* BUG FIX V6.00 */

            if ((*f_lim - FOV.y) < 0.001) {
                l_status = E_NO_ERROR;
                DB_PRINTF("D", ("**** Adjust OK *****\n")) ;
            }

            *f_lim *= 100 ;		/* m -> cm */
        }

    } else {
        l_status = E_ERROR ;
    }

    return l_status ;
}

//****************************Method Header************************************
//Method Name   : CheckMode1_Plan
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int CPQMdBdt::CheckMode1Plan(
    CVarFieldHandle* const f_vf_handle,
    const int  f_prot
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMdBdt::CheckMode1Plan");

    if (!f_vf_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_vf_handle is NULL"));
        return E_ERROR;
    }

    static		f32vec3_t l_phasev = { 0 , 1 , 0 };
    static		f32vec3_t l_slicev = { 0 , 0 , 1 } ;
    f32vec3_t			l_phasevec, l_slicevec ;

    /*
    status_t            l_status;
    int                 l_n , l_numdone  ;
    VfFieldSpec_t       l_fieldspec[5];
    VfPathElem_t        l_path[5];
    */

    VfFieldSpec_t       l_fieldspec[5] = {0};
    VfPathElem_t        l_path[5] = {0};

    PQM_SCOPED_TRACE();

    l_path[0].subtree_name = SVN_ST_PROTOCOL;
    l_path[0].index = f_prot;
    l_path[1].subtree_name = SVN_ST_VISUALGROUP;
    l_path[1].index = 0;
    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(l_fieldspec, 0, sizeof(VfFieldSpec_t) * 5);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    int l_n = 0 ;
    VFF_SET_ARGS(l_fieldspec[l_n], SVN_PLN_VG_PHASE_ORI_VECTOR, VFT_FLTVEC3,
                 sizeof(f32vec3_t)  , &l_phasevec , 0L);
    l_n++;

    VFF_SET_ARGS(l_fieldspec[l_n], SVN_PLN_VG_SLICE_ORI_VECTOR, VFT_FLTVEC3,
                 sizeof(f32vec3_t)  , &l_slicevec , 0L);
    l_n++;

    int l_numdone = 0;

    status_t l_status = f_vf_handle->GetFields(l_path, SVD_ST_VISUALGROUP, l_fieldspec, l_n, (int*) & l_numdone);

    DB_PRINTF("D", ("l_slicevec=(%f,%f,%f) l_phasevec(%f,%f,%f)\n",
                    l_slicevec.x, l_slicevec.y, l_slicevec.z, l_phasevec.x, l_phasevec.y, l_phasevec.z));

    if (l_status != E_NO_ERROR || l_n != l_numdone) {

        l_status = E_ERROR;

    } else {
        if (VecCheck(&l_phasevec , &l_phasev) == E_NO_ERROR &&
            VecCheck(&l_slicevec , &l_slicev) == E_NO_ERROR) {
            l_status = E_NO_ERROR;

        } else {
            l_status =  E_ERROR ;
        }
    }

    return l_status ;
}

//****************************Method Header************************************
//Method Name   : VecCheck
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int CPQMdBdt::VecCheck(f32vec3_t* vec1, f32vec3_t* vec2
                      )const
{
    if (fabs(vec1->x) == fabs(vec2->x) &&
        fabs(vec1->y) == fabs(vec2->y) &&
        fabs(vec1->z) == fabs(vec2->z)) {
        return E_NO_ERROR ;
    }

    return E_ERROR ;
}

//****************************Method Header************************************
//Method Name   : CheckMode1
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int CPQMdBdt::CheckMode1(
    CVarFieldHandle* const f_vf_handle,
    CPqmProtocol*  obj,
    char* buf
)const
{
    //Patni-AMT/2010Apr16/Modified/JaFT# COMMON_CDR_07
    LPCTSTR FUNC_NAME = _T("CPQMdBdt::CheckMode1");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CheckMode1 Enter"));

    if (m_obj_manager == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_obj_manager pointer is NULL"));
        return E_ERROR;
    }

    CPqmPmAcqman* l_pqm_pm_acqman = m_obj_manager->GetAcqManager();

    if (l_pqm_pm_acqman == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqm_pm_acqman pointer is NULL"));
        return E_ERROR;
    }

    CScanProcedure*     l_scan_proc = l_pqm_pm_acqman->GetScanProcedure();

    if (l_scan_proc == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_scan_proc pointer is NULL"));
        return E_ERROR;
    }

    CPQMStudy* l_study = m_obj_manager->GetStudy();

    if (l_study == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_study pointer is NULL"));
        return E_ERROR;
    }

    if (!f_vf_handle || !obj) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("input argument is NULL"));
        return E_ERROR;
    }

    if (m_license_info == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_license_info pointer is NULL"));
        return E_ERROR;
    }

    flt32_t	l_lim1 = 0.0f;
    status_t l_sts = PQM_DBDT_NORMAL ;

    if (CheckMode1Plan(l_study->GetVfDBHandle(), obj->GetProtocol())) {
        l_sts = PQM_DBDT_CONTROLLED1_PLAN_ERR;
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PQM_DBDT_CONTROLLED1_PLAN_ERR"));

    } else if (CheckMode1FOV(l_study->GetVfDBHandle(), obj->GetProtocol(),
                             &l_lim1, m_license_info->GetIECEdition())) {

        sprintf(buf, "Min %.1f cm", l_lim1);
        l_sts = PQM_DBDT_CONTROLLED1_FOV_ERR ;
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PQM_DBDT_CONTROLLED1_FOV_ERR"));

    } else if (obj->GetCurrentdBdtMode() == VFC_DBDT_CONTROLLED1) {
        //Patni-PJS/2010Feb24/Added/DeFT# PROPOSAL_08-Remove EZScan

        l_sts = PQM_DBDT_CONTROLLED1 ;
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PQM_DBDT_CONTROLLED1"));
    }

    DB_FUNC_EXIT() ;
    return l_sts ;
}

//****************************Method Header************************************
//Method Name   : CheckHighMode
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int CPQMdBdt::CheckHighMode(
    CVarFieldHandle* const f_vf_handle,
    CPqmProtocol*  obj
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMdBdt::CheckHighMode");
    PQM_SCOPED_TRACE();

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (!f_vf_handle || !obj) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("input argument is NULL"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    status_t l_sts = PQM_DBDT_NORMAL ;

    if (obj->GetdBdtOperation() == VFC_DBDT_IEC_OPERATING_1ST_CONTROLLED) {
        //Patni-PJS/2010Feb24/Added/DeFT# PROPOSAL_08-Remove EZScan
        l_sts = PQM_DBDT_1ST_CONTROLLED;
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PQM_DBDT_1ST_CONTROLLED"));

        //Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display
        obj->SetdBdtModeString(DBDT_MODE1_LABEL);

    } else if (obj->GetdBdtOperation() == VFC_DBDT_IEC_OPERATING_2ND_CONTROLLED) {

        l_sts = PQM_DBDT_2ND_CONTROLLED_ERR ;
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PQM_DBDT_2ND_CONTROLLED_ERR"));
        //Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display
        obj->SetdBdtModeString(DBDT_MODE2_LABEL);

    } else if (obj->GetdBdtOperation() ==
               VFC_DBDT_IEC_OPERATING_OVER_CARDIAC_THRESHOLD) {

        l_sts = PQM_DBDT_OVER_ERR;
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PQM_DBDT_OVER_ERR"));

    } else if (obj->GetdBdtOperation() == VFC_DBDT_IEC_OPERATING_NORMAL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("VFC_DBDT_IEC_OPERATING_NORMAL"));
    }

    return l_sts ;
}

//****************************Method Header************************************
//Method Name   : HighMode
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int CPQMdBdt::HighMode(
    CVarFieldHandle* const f_vf_handle,
    CPqmProtocol*  f_obj
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMdBdt::HighMode");
    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    /*
    int		l_dbdt_mode ;
    flt32_t	l_dbdt_value[2] ;
    status_t	l_sts ;
    */
    //- Patni-PJS/2009Apr25/Modify/CPP Test tool execution

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_obj_manager == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_obj_manager pointer is NULL"));
        return E_ERROR;
    }

    if (!f_vf_handle || !f_obj) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("input argument is NULL"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    CPQMStudy* l_study = m_obj_manager->GetStudy();

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (l_study == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_study pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    PQM_SCOPED_TRACE();

    int		l_dbdt_mode = 0;
    flt32_t	l_dbdt_value[2]  = {0};

    memset(l_dbdt_value, 0, sizeof(l_dbdt_value));
    status_t l_sts = CalcdBdt(f_vf_handle, f_obj, &l_dbdt_value[0], &l_dbdt_mode) ;

    if (l_sts ==  E_NO_ERROR) {

        l_dbdt_value[1] = l_dbdt_value[0] ;
        CString l_print_string(_T(""));
        l_print_string.Format(_T("l_dbdt_mode=[%d] l_dbdt_value=[%f]  (%d)"),
                              l_dbdt_mode, l_dbdt_value[0], __LINE__);

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_print_string);

        /* put dBdtOperation/dBdt_value */
        f_obj->SetdBdtOperation(l_dbdt_mode);
        f_obj->SetdBdtValue(0, l_dbdt_value[0]);
        f_obj->SetdBdtValue(1, l_dbdt_value[1]);
        l_study->PutdBdtMode(f_vf_handle, f_obj->GetProtocol(), l_dbdt_mode, l_dbdt_value);
    }

    return l_sts ;
}

//****************************Method Header************************************
//Method Name   : Check2ndMode
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int CPQMdBdt::Check2ndMode(
    CVarFieldHandle* const f_vf_handle,
    CPqmProtocol*  f_obj
)const
{
    //Patni-AMT/2010Apr16/Modified/JaFT# COMMON_CDR_07
    LPCTSTR FUNC_NAME = _T("CPQMdBdt::Check2ndMode");

    PQM_SCOPED_TRACE();

    if (!f_vf_handle || !f_obj) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("input argument is NULL"));
        return E_ERROR;
    }

    status_t l_sts = PQM_DBDT_NORMAL;

    if (f_obj->GetdBdtOperation() == VFC_DBDT_IEC_OPERATING_1ST_CONTROLLED) {
        //Patni-PJS/2010Feb24/Added/DeFT# PROPOSAL_08-Remove EZScan
        l_sts = PQM_DBDT_1ST_CONTROLLED;
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PQM_DBDT_1ST_CONTROLLED"));

        // Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display
        f_obj->SetdBdtModeString(DBDT_MODE1_LABEL);

    } else if (f_obj->GetdBdtOperation() ==
               VFC_DBDT_IEC_OPERATING_2ND_CONTROLLED) {

        //Patni-PJS/2010Feb24/Added/DeFT# PROPOSAL_08-Remove EZScan
        l_sts = PQM_DBDT_2ND_CONTROLLED;
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PQM_DBDT_2ND_CONTROLLED"));

        //Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display
        f_obj->SetdBdtModeString(DBDT_MODE2_LABEL);

    } else if (f_obj->GetdBdtOperation() ==
               VFC_DBDT_IEC_OPERATING_OVER_CARDIAC_THRESHOLD) {

        l_sts = PQM_DBDT_OVER_ERR;
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PQM_DBDT_OVER_ERR"));

    } else if (f_obj->GetdBdtOperation() == VFC_DBDT_IEC_OPERATING_NORMAL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("VFC_DBDT_IEC_OPERATING_NORMAL"));
    }

    return l_sts ;
}

//****************************Method Header************************************
//Method Name   : Error
//Author        : PATNI
//Purpose       :
//*****************************************************************************
//Patni-HAR/2010Feb08/Modified/MSA0251-00125
bool	CPQMdBdt::Error(
    int	dBdtcode,
    char* dBdtdata,
    int	sarcode,
    char* sardata,
    CPqmProtocol* f_protocol_object
)
{
    LPCTSTR FUNC_NAME = _T("CPQMdBdt::Error");
#define PROC(n) {PQM_DBDT_##n,IDS_MSG_DBDT_##n}
#define PROCSAR(n) {PQM_IEC_SAR_##n,IDS_MSG_IEC_SAR_##n}


    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_obj_manager) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_obj_manager pointer is NULL"));
        return false;
    }

    CPqmPmAcqman* l_pqm_pm_acqman = m_obj_manager->GetAcqManager();

    if (NULL == l_pqm_pm_acqman) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqm_pm_acqman pointer is NULL"));
        return false;
    }

    //+Patni-HAR-SS/2010Feb09/Modified/MSA0251-00125
    if (NULL == f_protocol_object) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_protocol_object pointer is NULL"));
        return false;
    }

    //-Patni-HAR-SS/2010Feb09/Modified/MSA0251-00125

    CScanProcedure*     l_scan_proc = l_pqm_pm_acqman->GetScanProcedure();

    if (NULL == l_scan_proc) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_scan_proc pointer is NULL"));
        return false;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    //+Patni-PJS/2009Oct30/Added/IR-92
    CPQMStudy* l_pqmstudy = m_obj_manager->GetStudy();

    if (NULL == l_pqmstudy) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqmstudy pointer is NULL"));
        return false;
    }

    //-Patni-PJS/2009Oct30/Added/IR-92
    CPqmProtocol*       l_protocol = f_protocol_object;//l_scan_proc->GetScanProcProtocol();//Patni-HAR/2010Feb08/Modified/MSA0251-00125

    static struct {
        int code;
        int literal;
        //XtCallbackProc  okCB;
        //XtCallbackProc  cancelCB;
    } dialogTbl[] = {
        PROC(CALC_ERROR),
        PROC(1ST_CONTROLLED),
        PROC(2ND_CONTROLLED),
        PROC(CONTROLLED1),
        PROC(CONTROLLED1_PLAN_ERR),
        PROC(CONTROLLED1_FOV_ERR),
        PROC(CONTROLLED2_RO_ERR),
        PROC(LICENSE_ERR), /* このメッセージはCalcからのみ呼ばれる*/
        //	PROC(2ND_CONTROLLED_ERR),
        //	PROC(OVER_ERR),

        PROCSAR(1ST_CONTROLLED),
        PROCSAR(2ND_CONTROLLED),
        PROCSAR(LICENSE_ERR),
    };
#undef PROC
#undef PROCSAR
    DB_FUNC_ENTER("[pqmdBdtError]:");

    DB_PRINTF("A", ("dBdtcode=%d SARcode=%d \n", dBdtcode, sarcode));
    //Patni-NP/2009Aug29/Added/MVC004423
    CString tmp1(_T(""));
    CString tmp2(_T(""));
    /* まずはエラーコードに対応したメッセージとCallBack処理テーブルを取得 */
    int		dBdt_index = -1, sar_index = -1;

    if (dBdtcode != PQM_DBDT_NORMAL) {
        for (dBdt_index = 0; dBdt_index < sizeof(dialogTbl) / sizeof(*dialogTbl); dBdt_index++) {
            if (dBdtcode == dialogTbl[dBdt_index].code)
                break ;
        }

        if (dBdt_index >= (sizeof(dialogTbl) / sizeof(*dialogTbl)))
            dBdt_index = -1 ;
    }

    if (sarcode != PQM_IEC_SAR_NORMAL) {
        for (sar_index = 0; sar_index < sizeof(dialogTbl) / sizeof(*dialogTbl); sar_index++) {
            if (sarcode == dialogTbl[sar_index].code)
                break ;
        }

        if (sar_index >= (sizeof(dialogTbl) / sizeof(*dialogTbl)))
            sar_index = -1 ;
    }

    /* dBdt或いはSAR Operationでエラーの場合はエラーメッセージとする */
    if (dBdtcode & PQM_DBDT_ERR) {	/* ここではsarのOperationを無視するようにする */
        sarcode = PQM_IEC_SAR_NORMAL ;
        sar_index = -1 ;

    } else if (sarcode & PQM_IEC_SAR_ERR) {	/* ここではdBdtのOperationを無視するようにする */
        dBdtcode = PQM_DBDT_NORMAL ;
        dBdt_index = -1 ;
    }

    /*  どちらかが2ndOperationモードになった
    PASSWORD入力が必要なの場合 */
    else if ((dBdtcode == PQM_DBDT_2ND_CONTROLLED)) {

        //PqmUiPasswordDialog( err ) ;

        /*   CPQMLogMgr::GetInstance()->DisplayError(
        	   l_err_str,
        	   _T("PQMdBdt.cpp"),__LINE__,ERROR_LOG_CRUCIAL1 ,
        	   ERR_ID,APP_NAME);*/
        //+Patni-PJS/2009Oct29/Added/IR-92

        //Patni-HAR/2010Feb08/Modified/MSA0251-00125
        // Here, Protocol Object NULL should be tested first, otherwise if it is NULL
        // in case of "if loop" then application will crash or hang
        if (l_protocol != NULL) {
            if (l_protocol->GetdBdtOperation() > l_pqmstudy->GetCurrentHighestDbdt()) {
                m_obj_manager->WriteToUI(PQM_MSG_DBDT_CAUTION_DIALOG);

                if (m_obj_manager->GetDBDtCancelFlag() > 0) {
                    PqmIECScanCancel();
                    int l_processing = l_scan_proc->GetProcessing();
                    l_scan_proc->SetProcessing(l_processing |= PQM_ACQ_BIT_WARNING);
                    return false;

                } else {
                    //Patni-HAR/2010Feb08/Modified/MSA0251-00125
                    //if (l_protocol != NULL) {
                    int dbdt_opern = l_protocol->GetdBdtOperation();

                    if (dbdt_opern > l_pqmstudy->GetCurrentHighestDbdt()) {
                        l_pqmstudy->SetCurrentHighestDbdt(dbdt_opern);
                    }

                    //}

                    return true;
                }



            }

        }

        //-Patni-PJS/2009Oct29/Added/IR-92
    }

    /* ここではPASSWORDが必要ない警告メッセージ或いは
       エラーメッセージの処理を行う */
    //+Patni-NP/2009Aug27/Uncommented/
    CString msg(_T(""));
    CString msg1(_T(""));
    CString msg2(_T(""));

    if (dBdt_index != -1) {

        if (dBdtdata && strlen(dBdtdata)) {
            char  buf[512] = {0} ;	//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
            sprintf(buf, "\n%s", dBdtdata);
            tmp1 = (CString)buf;

            //+Patni-PJS/2009Nov1/Commented/Code Review
            if (NULL == l_protocol) {
                tmp2 = GetErrorStringFromInt(dBdt_index);

                if (tmp1.GetLength() > 0 && tmp2.GetLength() > 0) {
                    msg1 = tmp1 + tmp2;

                }

            } else {
                if ((dBdtcode == PQM_DBDT_1ST_CONTROLLED) || (dBdtcode == PQM_DBDT_2ND_CONTROLLED)) {

                    if (l_protocol->GetdBdtOperation() > l_pqmstudy->GetCurrentHighestDbdt()) {



                        tmp2 = GetErrorStringFromInt(dBdt_index);

                        if (tmp1.GetLength() > 0 && tmp2.GetLength() > 0) {
                            msg1 = tmp1 + tmp2;

                        }
                    }


                } else {
                    tmp2 = GetErrorStringFromInt(dBdt_index);

                    if (tmp1.GetLength() > 0 && tmp2.GetLength() > 0) {
                        msg1 = tmp1 + tmp2;

                    }
                }
            }

            //-Patni-PJS/2009Oct29/Added/IR-92

        } else {
            //+Patni-PJS/2009Nov1/Commented/Code Review
            //int l_literal = dialogTbl[dBdt_index].literal;
            //msg1.LoadString(l_literal);
            //Patni-MP/2009Aug29/Addeded/MVC004423
            //+Patni-PJS/2009Oct29/Added/IR-92
            if (NULL == l_protocol) {
                msg1 = GetErrorStringFromInt(dBdt_index);

            } else {
                if ((dBdtcode == PQM_DBDT_1ST_CONTROLLED) || (dBdtcode == PQM_DBDT_2ND_CONTROLLED)) {
                    if (l_protocol->GetdBdtOperation() > l_pqmstudy->GetCurrentHighestDbdt()) {

                        msg1 = GetErrorStringFromInt(dBdt_index);
                    }


                } else {
                    msg1 = GetErrorStringFromInt(dBdt_index);
                }
            }

            //-Patni-PJS/2009Oct29/Added/IR-92
        }
    }

    if (sar_index != -1) {

        //+Patni-PJS/2009Nov10/Added/IR-92 Code Review
        status_t l_sarmode = IEC_SAR_NORMAL_MODE;

        CPQMStudy* l_study = m_obj_manager->GetStudy();

        if (l_study == NULL) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_study pointer is NULL"));
            return E_ERROR;
        }

        sarInfo_t* l_sar_info = l_protocol->GetSarInfo();

        if (l_sar_info) {
            l_sarmode = l_study->GetPqm()->GetSARControlLicense() ? l_sar_info->sarControl.ope_mode :
                        l_sar_info->tsar[l_sar_info->active].ope_mode;
        }

        //-Patni-PJS/2009Nov10/Added/IR-92 Code Review

        if (sardata && strlen(sardata)) {
            char  buf[512] = {0} ;	//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections

            //tmp1.LoadString(dialogTbl[sar_index].literal);
            //Patni-MP/2009Aug29/Addeded/MVC004423
            //+Patni-PJS/2009Oct29/Added/IR-92
            if (l_sarmode > l_pqmstudy->GetCurrentHighestSar()) {
                tmp1 = GetErrorStringFromInt(sar_index);
                sprintf(buf, "%s", sardata);
                tmp2 = (CString)buf;

                if (tmp1.GetLength() > 0 && tmp2.GetLength() > 0) {
                    tmp1 += tmp2;
                    msg2 = tmp1;

                }
            }




            //-Patni-PJS/2009Oct29/Added/IR-92


        } else {
            //+Patni-PJS/2009Nov1/Commented/Code Review
            //int l_literal = dialogTbl[sar_index].literal;
            //msg2.LoadString(l_literal);
            //Patni-MP/2009Aug29/Addeded/MVC004423
            //+Patni-PJS/2009Oct29/Added/IR-92

            if (l_sarmode > l_pqmstudy->GetCurrentHighestSar()) {

                msg2 = GetErrorStringFromInt(sar_index);
            }



            //-Patni-PJS/2009Oct29/Added/IR-92
        }
    }

    int i = 0;	//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections

    //+ Patni-NP/2009Aug29/Modified/MVC004423
    if (msg1.GetLength() > 0 && msg2.GetLength() > 0) {

        char		buf[512] = {0} ;	//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
        sprintf(buf, "\n\n");
        tmp1 = buf;

        if (tmp1.GetLength() > 0) {
            tmp2 = msg1 + tmp1;

        }

        if (tmp2.GetLength() > 0) {
            msg = tmp2 + msg2;

        }

        i = dBdt_index ;

    } else if (msg1.GetLength() > 0) {
        i = dBdt_index ;
        msg = msg1;

    } else {
        i = sar_index ;
        msg = msg2;

    }

    //+ Patni-RSG/2009Apr27/Added/JFT- Default value set for button
    int l_buttontype = SCAN_WARN_CANCELBUTTON;

    int l_warnning_level = SCAN_WARN_INFORMATION;

    //- Patni-RSG/2009Apr27/Added/JFT- Default value set for button
    //Patni-NP/2009Aug29/Modified/MVC004423
    switch (i) {
        case PQM_CALC_ERROR :

            l_buttontype = SCAN_WARN_OKONLYBUTTON;
            l_warnning_level = SCAN_WARN_INFORMATION;
            break;

        case PQM_1ST_CONTROLLED:
            //Patni-MP/2009Aug29/Commented/MVC004423
            /*{
            	CString str1, str2;
            	str1 = CPqmUtilities::GetMultiLingualString(_T("IDS_DBDT_EXCEEDS_NORMAL_MODE1")); // Added By KT/ACE-2/15-06-2009/Multilingualisation
            	str2 = CPqmUtilities::GetMultiLingualString(_T("IDS_DBDT_EXCEEDS_NORMAL_MODE2")); // Added By KT/ACE-2/15-06-2009/Multilingualisation
            	msg = str1 + str2;
            }*/
            l_buttontype = SCAN_WARN_OK_CANCEL_BUTTONS;
            l_warnning_level = SCAN_WARN_CRUCIAL;
            break;

        case PQM_2ND_CONTROLLED:
            //Patni-MP/2009Aug29/Commented/MVC004423
            //msg = CPqmUtilities::GetMultiLingualString(_T("IDS_DBDT_ELEVATED_ERROR"));		 // Added By KT/ACE-2/15-06-2009/Multilingualisation
            l_buttontype = SCAN_WARN_OKONLYBUTTON;
            l_warnning_level = SCAN_WARN_INFORMATION;
            break;

        case PQM_CONTROLLED1:
            //Patni-MP/2009Aug29/Commented/MVC004423
            //msg = CPqmUtilities::GetMultiLingualString(_T("IDS_ELEVATED_DBDT_LEVEL_CAUTION"));											  // Added By KT/ACE-2/15-06-2009/Multilingualisation
            l_buttontype = SCAN_WARN_OK_CANCEL_BUTTONS;
            l_warnning_level = SCAN_WARN_CRUCIAL;
            break;

        case PQM_CONTROLLED1_PLAN_ERR:
            //Patni-MP/2009Aug29/Commented/MVC004423
            //msg = CPqmUtilities::GetMultiLingualString(_T("IDS_TRANSAXIAL_PROTOCOL")); // Added By KT/ACE-2/15-06-2009/Multilingualisation
            l_buttontype = SCAN_WARN_OKONLYBUTTON;
            l_warnning_level = SCAN_WARN_INFORMATION;
            break;

        case PQM_CONTROLLED1_FOV_ERR:
            //Patni-MP/2009Aug29/Commented/MVC004423
            //msg = CPqmUtilities::GetMultiLingualString(_T("IDS_SMALL_SPATIAL_RES")); // Added By KT/ACE-2/15-06-2009/Multilingualisation
            l_buttontype = SCAN_WARN_OKONLYBUTTON;
            l_warnning_level = SCAN_WARN_INFORMATION;
            break;

        case PQM_CONTROLLED2_RO_ERR:
            //Patni-MP/2009Aug29/Commented/MVC004423
            //msg = CPqmUtilities::GetMultiLingualString(_T("IDS_OBLIQUE_ANGLE_ERROR")); // Added By KT/ACE-2/15-06-2009/Multilingualisation
            l_buttontype = SCAN_WARN_OKONLYBUTTON;
            l_warnning_level = SCAN_WARN_INFORMATION;
            break;

        case PQM_LICENSE_ERR:
            //Patni-MP/2009Aug29/Commented/MVC004423
            //msg = CPqmUtilities::GetMultiLingualString(_T("IDS_DBDT_LICENSE_ERROR")); // Added By KT/ACE-2/15-06-2009/Multilingualisation
            l_buttontype = SCAN_WARN_OKONLYBUTTON;
            l_warnning_level = SCAN_WARN_INFORMATION;
            break;

            //	PQM_2ND_CONTROLLED_ERR,
            //	PQM_OVER_ERR,
        case PQM_SAR_1ST_CONTROLLED:
            //Patni-MP/2009Aug29/Commented/MVC004423
            //msg = CPqmUtilities::GetMultiLingualString(_T("IDS_SAR_EXCEEDS_NORMAL_MODE_CAUTION")); // Added By KT/ACE-2/15-06-2009/Multilingualisation
            //msg += tmp2;
            l_buttontype = SCAN_WARN_OK_CANCEL_BUTTONS;
            l_warnning_level = SCAN_WARN_CRUCIAL;
            break;

        case PQM_SAR_2ND_CONTROLLED:
            //Patni-MP/2009Aug29/Commented/MVC004423
            //msg = CPqmUtilities::GetMultiLingualString(_T("IDS_SAR_EXCEEDS_IST_MODE_CAUTION")); // Added By KT/ACE-2/15-06-2009/Multilingualisation
            //msg += tmp2;
            l_buttontype = SCAN_WARN_OK_CANCEL_BUTTONS;
            l_warnning_level = SCAN_WARN_CRUCIAL;
            break;

        case PQM_SAR_LICENSE_ERR:
            //Patni-MP/2009Aug29/Commented/MVC004423
            //msg = CPqmUtilities::GetMultiLingualString(_T("IDS_SAR_LICENSE_ERROR"));// Added By KT/ACE-2/15-06-2009/Multilingualisation
            //msg += tmp2;
            l_buttontype = SCAN_WARN_OKONLYBUTTON;
            l_warnning_level = SCAN_WARN_INFORMATION;
            break;
    }

    bool l_result = true;

    if (msg.GetLength() > 0) {
        //+TMSC-Tanoue/2010Mar05/MSA0251-00247




        MPLUSERRORUSERRESPONSE_e  l_response = (MPLUSERRORUSERRESPONSE_e) CPQMLogMgr::GetInstance()->
                                               DisplayMessageinWarningDialog(msg, _T("Sequence Queue"),
                                                       l_warnning_level, SCAN_WARN_SYSMODAL, l_buttontype);
        //-TMSC-Tanoue/2010Mar05/MSA0251-00247

        if (l_response == MPlusErrResponse_CANCEL) {
            PqmIECScanCancel();
            l_result = false;
        }

        if (((dBdt_index == PQM_SAR_LICENSE_ERR) ||
             (dBdt_index == PQM_CALC_ERROR) ||
             (dBdt_index == PQM_CONTROLLED1_PLAN_ERR) ||
             (dBdt_index == PQM_CONTROLLED1_FOV_ERR) ||
             (dBdt_index == PQM_CONTROLLED2_RO_ERR)) &&
            (l_response == MPlusErrResponse_OK)) {

            PqmIECScanCancel();
            l_result = false;
        }

        if ((/*(dBdt_index == PQM_LICENSE_ERR) || */
                (dBdt_index == PQM_1ST_CONTROLLED) ||
                (dBdt_index == PQM_CONTROLLED1)) &&
            (l_response == MPlusErrResponse_OK)) {
            //+Patni-PJS/2009Oct29/Added/IR-92
            if (l_protocol != NULL) {
                l_pqmstudy->SetCurrentSarDbdt(l_protocol);


            }

            //-Patni-PJS/2009Oct29/Added/IR-92
            l_result = true;
        }

        //+Patni-PJS/2009Oct29/Added/IR-92
        if (l_result && ((sar_index == PQM_SAR_1ST_CONTROLLED) || (sar_index == PQM_SAR_2ND_CONTROLLED))) {
            if (l_protocol != NULL) {
                l_pqmstudy->SetCurrentSarDbdt(l_protocol);
            }
        }

        //-Patni-PJS/2009Oct29/Added/IR-92
        //+Patni-PJS/2009Oct15/Modified/MSA0248-00023

        if ((dBdt_index == PQM_LICENSE_ERR)
            && ((l_response == MPlusErrResponse_OK) || (l_response == MPlusErrResponse_CANCEL))) {

            //- Patni-MSN/2009Mar26/Added/PSP- According to Base code
            //Defect 84 - Fixed by Manishkumar
            //Set the value of ACQ_STATUS of protocol through object of PQMStudy.
            //l_protocol->SetAcqStatus(C_PR_STATUS_WAIT);

            //+Patni-HAR/2010Feb09/Modified/MSA0251-00125


            if ((C_PR_STATUS_CURRENT == l_protocol->GetAcqStatus())) {
                m_obj_manager->GetStudy()->SetAcqStatus(l_protocol, C_PR_STATUS_WAIT);

            }

            if (l_protocol == l_scan_proc->GetScanProcProtocol()) {
                m_obj_manager->GetStudy()->SetScanModeOfHead();
                l_scan_proc->ResetCurrentRequest();
                m_obj_manager->WriteToUI(PQM_MSG_ABORT_WAIT);
                l_pqm_pm_acqman->GetPqm()->GetAMB()->PqmPmAbortAmb();

                if (l_scan_proc->GetRequest()) {
                    l_pqm_pm_acqman->AcqDeleteLockFile();

                }

            }

            l_result = false;
            //-Patni-HAR/2010Feb09/Modified/MSA0251-00125
        }

        //-Patni-PJS/2009Oct15/Modified/MSA0248-00023
    }

    if (l_result) {
        int l_processing = l_scan_proc->GetProcessing();
        l_scan_proc->SetProcessing(l_processing |= PQM_ACQ_BIT_WARNING);
    }

    DB_FUNC_EXIT() ;
    return l_result;
}

//****************************Method Header************************************
//Method Name   : Calc_dBdt
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int CPQMdBdt::CalcdBdt(
    CVarFieldHandle* const f_vf_handle,
    CPqmProtocol* f_protocol,
    flt32_t* f_val,
    int* f_mode
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMdBdt::CalcdBdt");
    PQM_SCOPED_TRACE();

    if (!f_vf_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_vf_handle is NULL"));
        return E_ERROR;
    }

    VfPathElem_t        l_pelm[] = {{SVN_ST_PROTOCOL, 0}};
    VfFieldSpec_t       l_vfreq[1] = {0};	//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    int                 l_num = 0; //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    f32vec3_t           l_sysparams[3] = {0};	//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(l_vfreq, 0, sizeof(VfFieldSpec_t));
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    l_pelm[SVD_ST_PROTOCOL - 1].index = f_protocol->GetProtocol();
    VFF_SET_ARGS(l_vfreq[0], SVN_SEQ_LOCAL_SYSPARAMS, 0,
                 sizeof(l_sysparams), l_sysparams, 0);

    status_t l_status = f_vf_handle->GetFields(l_pelm, SVD_ST_PROTOCOL, l_vfreq, 1, &l_num);

    if (l_status == E_NO_ERROR) {

        l_status = PqmCalcdBdt((flt32_t*)l_sysparams , f_val);

        if (*f_val >=  CONTROLLED_2ND_VAL) {
            *f_mode = VFC_DBDT_IEC_OPERATING_2ND_CONTROLLED ;
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("VFC_DBDT_IEC_OPERATING_2ND_CONTROLLED"));

            //+ Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display
            f_protocol->SetdBdtModeString(DBDT_MODE2_LABEL);
            //+ Patni-PJS/2009Jun05/Modify/JFT# 251,251,251,IR(5)
            //m_obj_manager->WriteToUI(PQM_MSG_UPDATE_PAGER_AREA);
            //- Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display

        } else if (*f_val >= CONTROLLED_1ST_VAL) {
            *f_mode = VFC_DBDT_IEC_OPERATING_1ST_CONTROLLED ;
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("VFC_DBDT_IEC_OPERATING_1ST_CONTROLLED"));
            //+ Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display
            f_protocol->SetdBdtModeString(DBDT_MODE1_LABEL);
            //+ Patni-PJS/2009Jun05/Modify/JFT# 251,251,251,IR(5)
            //m_obj_manager->WriteToUI(PQM_MSG_UPDATE_PAGER_AREA);
            //- Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display

        } else {
            *f_mode = VFC_DBDT_IEC_OPERATING_NORMAL ;
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("VFC_DBDT_IEC_OPERATING_NORMAL"));
        }
    }

    return l_status ;
}

//****************************Method Header************************************
//Method Name   : PqmCalcdBdt
//Author        : PATNI
//Purpose       :
//*****************************************************************************
status_t CPQMdBdt::PqmCalcdBdt(
    flt32_t* f_slew,
    flt32_t* f_val
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMdBdt::PqmCalcdBdt");
    PQM_SCOPED_TRACE();

    status_t		            l_status = 0;	//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    float			            l_dbdt = 0.0f;	//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    static int		            l_first		= 0;
    static GtSysParamT	        l_sysparams;
    static dBdtParamT	        l_dbdtparams;
    static RmDefSystemInfoT	    l_sysinfonow;

    if (!l_first) {
        l_status = sgGetSystemInfoFromfile(&l_sysinfonow,
                                           SG_SYSTEM_INFO_CTRL_ENV,
                                           SG_SYSTEM_INFO_FILE);

        if (l_status == E_ERROR) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("sgGetSystemInfoFromfile Fatal ERROR "));
            return E_ERROR ;
        }

        memset(&l_sysparams, 0x00, sizeof(l_sysparams));
        l_status = dBdt_Load_Params(&l_dbdtparams);

        if (l_status == E_ERROR) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("dBdt_Load_Params Fatal ERROR "));
            return E_ERROR ;
        }

        l_status = gtLoadSysParams(&l_sysparams);

        if (l_status == E_ERROR) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("gtLoadSysParams Fatal ERROR "));
            return E_ERROR ;
        }

        l_first = 1;
    }

    for (int l_i = 0; l_i < GT_NUM_CHANNELS; l_i++) {
        l_sysparams.param[l_i][0] = f_slew[l_i * 3];
        l_sysparams.param[l_i][1] = f_slew[1 + (l_i * 3)];
        l_sysparams.param[l_i][3] = f_slew[2 + (l_i * 3)];
    }

    int	l_checkmode	= DBDT_OPERATING_MODE_UNKNOWN;
    /*Z-AXIS Calculation*/
    l_status = dBdtCheckCSR(&l_sysparams,
                            &l_dbdtparams,
                            &l_dbdt,
                            &l_checkmode,
                            (float)l_sysinfonow.GradValueMax,
                            (float)l_sysinfonow.GradValueMax,
                            (float)l_sysinfonow.GradValueMax,
                            (float)l_sysinfonow.GradValueMin,
                            (float)l_sysinfonow.GradValueMin,
                            (float)l_sysinfonow.GradValueMin);

    if (l_checkmode == DBDT_SEQ_MODE_CSR) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("DBDT_SEQ_MODE_CSR"));
        DB_PRINTF("D", ("dBdt(CSR) : %8.2f [T/s]\n", l_dbdt));

    } else if (l_checkmode == DBDT_SEQ_MODE_CRT) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("DBDT_SEQ_MODE_CRT"));
        DB_PRINTF("D", ("dBdt(CRT) : %8.2f [T/s]\n", l_dbdt));

    } else if (l_checkmode == DBDT_SEQ_MODE_SIN_CRT) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("DBDT_SEQ_MODE_SIN_CRT"));
        DB_PRINTF("D", ("dBdt(SIN_CRT) : %8.2f [T/s]\n", l_dbdt));

    } else if (l_checkmode == DBDT_SEQ_MODE_SIN_CSR) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("DBDT_SEQ_MODE_SIN_CSR"));
        DB_PRINTF("D", ("dBdt(SIN_CSR) : %8.2f [T/s]\n", l_dbdt));

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Invalid Mode"));
        DB_PRINTF("D", (" invalid mode : %d\n", l_checkmode));
    }

    *f_val = l_dbdt;

    CString l_print_str(_T(""));
    l_print_str.Format(_T("*f_val = %f"), l_dbdt);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_print_str);

    return(E_NO_ERROR) ;
}

//****************************Method Header************************************
//Method Name   : InsetCrossProduct
//Author        : PATNI
//Purpose       :
//*****************************************************************************
void CPQMdBdt::InsetCrossProduct(double  p[],
                                 double  q[],
                                 double  crossProduct[])const
{

    crossProduct[0] = p[1] * q[2] - p[2] * q[1];
    crossProduct[1] = p[2] * q[0] - p[0] * q[2];
    crossProduct[2] = p[0] * q[1] - p[1] * q[0];

    InsetNormalize(crossProduct);
}

//****************************Method Header************************************
//Method Name   : InsetNormalize
//Author        : PATNI
//Purpose       :
//*****************************************************************************
void CPQMdBdt::InsetNormalize(double  v[])const
{
    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    /*
    double      length;
    int         l_i;
    */
    int         l_i = 0;
    //- Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    double length = sqrt(v[0] * v[0]  +  v[1] * v[1]  +  v[2] * v[2]);

    if (length != 0.) {
        for (l_i = 0; l_i < 3; l_i++) {
            v[l_i] = v[l_i] / length;
        }

    } else {
        for (l_i = 0; l_i < 3; l_i++) {
            v[l_i] = 0.;
        }
    }

    for (l_i = 0; l_i < 3; l_i++) {
        if (fabs(v[l_i]) < 0.00001) {
            v[l_i] = 0. ;
        }
    }
}

//****************************Method Header************************************
//Method Name   : GetErrorStringFromInt
//Author        : PATNI
//Purpose       :
//*****************************************************************************
//+ Patni-MP/2009Aug29/Added/MVC004423
CString CPQMdBdt::GetErrorStringFromInt(const int f_literal)const
{

    CString l_msg = _T("");

    switch (f_literal) {

        case PQM_CALC_ERROR :
            // Added By KT/ACE-2/15-06-2009/Multilingualisation
            l_msg = CPqmUtilities::GetMultiLingualString(_T("IDS_DBDT_CALC_ERROR"));	  // Added By KT/ACE-2/15-06-2009/Multilingualisation

            break;

        case PQM_1ST_CONTROLLED: {

            //+Patni-NP/2010Mar5/Commented/ML
            /*
            CString str1, str2;
            str1 = CPqmUtilities::GetMultiLingualString(_T("IDS_DBDT_EXCEEDS_NORMAL_MODE1")); // Added By KT/ACE-2/15-06-2009/Multilingualisation
            str2 = CPqmUtilities::GetMultiLingualString(_T("IDS_DBDT_EXCEEDS_NORMAL_MODE2")); // Added By KT/ACE-2/15-06-2009/Multilingualisation
            l_msg = str1 + str2;*/
            l_msg = CPqmUtilities::GetMultiLingualString(_T("IDS_DBDT_EXCEEDS_NORMAL_MODE1")); // Added By KT/ACE-2/15-06-2009/Multilingualisation

            //+Patni-NP/2010Mar5/Commented/ML
        }

        break;

        case PQM_2ND_CONTROLLED:

            l_msg = CPqmUtilities::GetMultiLingualString(_T("IDS_DBDT_ELEVATED_ERROR"));		 // Added By KT/ACE-2/15-06-2009/Multilingualisation

            break;

        case PQM_CONTROLLED1:

            l_msg = CPqmUtilities::GetMultiLingualString(_T("IDS_ELEVATED_DBDT_LEVEL_CAUTION"));											  // Added By KT/ACE-2/15-06-2009/Multilingualisation

            break;

        case PQM_CONTROLLED1_PLAN_ERR:

            l_msg = CPqmUtilities::GetMultiLingualString(_T("IDS_TRANSAXIAL_PROTOCOL")); // Added By KT/ACE-2/15-06-2009/Multilingualisation

            break;

        case PQM_CONTROLLED1_FOV_ERR:

            l_msg = CPqmUtilities::GetMultiLingualString(_T("IDS_SMALL_SPATIAL_RES")); // Added By KT/ACE-2/15-06-2009/Multilingualisation

            break;

        case PQM_CONTROLLED2_RO_ERR:

            l_msg = CPqmUtilities::GetMultiLingualString(_T("IDS_OBLIQUE_ANGLE_ERROR")); // Added By KT/ACE-2/15-06-2009/Multilingualisation

            break;

        case PQM_LICENSE_ERR:

            l_msg = CPqmUtilities::GetMultiLingualString(_T("IDS_DBDT_LICENSE_ERROR")); // Added By KT/ACE-2/15-06-2009/Multilingualisation
            break;

            //	PQM_2ND_CONTROLLED_ERR,
            //	PQM_OVER_ERR,
        case PQM_SAR_1ST_CONTROLLED:

            l_msg = CPqmUtilities::GetMultiLingualString(_T("IDS_SAR_EXCEEDS_NORMAL_MODE_CAUTION")); // Added By KT/ACE-2/15-06-2009/Multilingualisation
            break;

        case PQM_SAR_2ND_CONTROLLED:

            l_msg = CPqmUtilities::GetMultiLingualString(_T("IDS_SAR_EXCEEDS_IST_MODE_CAUTION")); // Added By KT/ACE-2/15-06-2009/Multilingualisation
            break;

        case PQM_SAR_LICENSE_ERR:

            l_msg = CPqmUtilities::GetMultiLingualString(_T("IDS_SAR_LICENSE_ERROR"));// Added By KT/ACE-2/15-06-2009/Multilingualisation
            break;

        default:

            l_msg =  CPqmUtilities::GetMultiLingualString(_T("IDS_NO_MATCHING_WARNING_ID"));   	// Added By KT/ACE-2/15-06-2009/Multilingualisation
    }

    return l_msg;
}
//- Patni-MP/2009Aug29/Added/MVC004423

//****************************Method Header************************************
//Method Name   : Get_IEC_Edition
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int	CPQMdBdt::GetIECEdition(
    long*	f_edition
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMdBdt::GetIECEdition");
    int32_t  l_status = FALSE;
    PQM_SCOPED_TRACE();

    CString l_print_str(_T(""));
    l_print_str.Format(_T("f_edition = %d"), f_edition);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_print_str);

    if (AcqGetIECEdition(f_edition) == ACQ_ERROR_NONE) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ACQ_ERROR_NONE"));

        if ((*f_edition >= VFC_DBDT_IEC_EDITION_1995)
            && (*f_edition <= VFC_DBDT_IEC_EDITION_2002)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("f_edition is > VFC_DBDT_IEC_EDITION_1995 and < \
                VFC_DBDT_IEC_EDITION_2002"));

            l_status = TRUE ;
        }
    }

    return l_status ;
}


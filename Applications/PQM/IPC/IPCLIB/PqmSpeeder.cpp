//REDMINE-1213_Update_03_MAY
//Redmine-783
//Redmine-774
//Redmine-778
//Redmine-780
//Redmine-781
//Redmine-782
// Speeder.cpp: implementation of the CSpeeder class.
//
//////////////////////////////////////////////////////////////////////
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: Speeder.cpp
 *  Overview: Implementation of CSpeeder class.
 *  Created:  2007/10/12 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2007/10/12 12:00:00    Created for PQM7 MAP
                                                            sequence scanning
 *
 *****************************************************************************/
#include "stdafx.h"
#include "PqmSpeeder.h"
#include <ErrorDisp/ErrorDispConsts.h>
#include <tami/libSpeeder/speederStatus.h>
#include <tami/libms/b1ICorrModeDecision.h>
extern "C" {
#include <tami/libSpeeder/speederProc.h>
}
#include <PQM/PQMLogMgr.h>
#include <PQM/resource.h>
#include <PQM/PqmUtilities.h>  // Added By KT/ACE-2/15-06-2009/Multilingualisation
#include "pqm.h"
#include "pqmpmacqman.h"
#include "scanprocedure.h"
#include "CheckFOVInMap.h"
#include "PqmCoilData.h"
#include "PqmPrMode.h"
#include "VarFieldHandle.h" //TMSC-Tanoue/2010Aug25/MVC008345
#include "GPLibAdapter.h" //TMSC-Tanoue/2010Aug25/MVC008345
#include "PQMDebugInfo.h"
#include "CoilChannelMode.h"
#include "pqmAMB.h"


//+Patni-MP/2009Mar07/Modified/ACE-MCM0213-00124
#define	SPEEDER_TIMEOUT_DEFAULT		(60*2)
//-Patni-MP/2009Mar07/Modified/ACE-MCM0213-00124

//Patni-Sribanta/2011Mar04/Added/IR-184

using namespace MPlus::EH;

//****************************Method Header************************************
//Method Name   :CPqmSpeeder()
//Author        :PATNI / JS
//Purpose       :Default Constructor
//*****************************************************************************
CPqmSpeeder* CPqmSpeeder::m_self_ptr = NULL;
CPqmSpeeder::CPqmSpeeder():
    m_pqm_ptr(NULL),
    m_num_speeder(0),
    m_speeder_timeout((unsigned)0),
    s_speederTbl(NULL)
{
    LPCTSTR FUNC_NAME = _T("CPqmSpeeder::CPqmSpeeder");
    PQM_TRACE1(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of CPqmSpeeder::CPqmSpeeder"));
    m_self_ptr = this;
    //+Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections


    //-Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
    WNP_ProcInit();
}

//****************************Method Header************************************
//Method Name   :~CPqmSpeeder()
//Author        :PATNI / JS
//Purpose       :Destructor
//*****************************************************************************
CPqmSpeeder::~CPqmSpeeder(
)
{
    LPCTSTR FUNC_NAME = _T("CPqmSpeeder::~CPqmSpeeder");
    PQM_TRACE1(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of CPqmSpeeder::~CPqmSpeeder"));

    m_self_ptr = NULL;
}

//****************************Method Header************************************
//Method Name   :MapCoilCheckSaturn()
//Author        :PATNI /MRP
//Purpose       :コイル情報から撮影可能なMAP種を取得する
//*****************************************************************************
int CPqmSpeeder::MapCoilCheckSaturn(
    PqmSaturnRcvCoil_t* coil_t
)
{

    LPCTSTR FUNC_NAME = _T("CPqmSpeeder::MapCoilCheckSaturn");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of MapCoilCheckSaturn"));
    int sts = VFC_INTENSITY_MAP_TYPE_NONE ;

    if (coil_t->numCoil == 0)	return sts ;

    CPqmCoilData*   l_coil_data = CDataManager::GetInstance()->GetPQMCoilData();
    //Added by Meghana On 9/1/2008 2:31:16 PM

    //+Patni-RUP/2009Aug20/Modified/cpp test corrections
    if (l_coil_data == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("l_coil_data pointer is NULL"));
        return sts ;
    }

    //-Patni-RUP/2009Aug20/Modified/cpp test corrections

    if (l_coil_data->GetSpeederInSaturn(coil_t , NULL)) {
        sts |= VFC_INTENSITY_MAP_TYPE_SPEEDER ;
    }

    CCoil dummy_coil;

    if (l_coil_data->GetRWBInSaturn(coil_t , RWB_SPEEDER , dummy_coil)) {
        sts |= VFC_INTENSITY_MAP_TYPE_INTENSITY ;

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetRWBInSaturn failed"));
    }

    return sts ;
}

//****************************Method Header************************************
//Method Name   :MapCoilCheck()
//Author        :PATNI /MRP
//Purpose       :コイル情報から撮影可能なMAP種を取得する
//*****************************************************************************
int CPqmSpeeder::MapCoilCheck(
    const int coilId
)
{
    LPCTSTR FUNC_NAME = _T("CPqmSpeeder::MapCoilCheck");

    CString l_trace_string(_T(""));
    l_trace_string.Format(_T("CoilID = 0x%08x"), coilId);
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);

    int sts = VFC_INTENSITY_MAP_TYPE_NONE ;


    //+Patni-RUP/2009Aug24/Modified/cpp test corrections
    //CPqmCoilData   *l_coil_data = CDataManager::GetInstance()->GetPQMCoilData();

    CDataManager* l_data_manager = CDataManager::GetInstance();

    if (l_data_manager == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,  _T("l_data_manager pointer is NULL"));
        return sts;
    }

    CPqmCoilData* l_coil_data = l_data_manager->GetPQMCoilData();

    if (l_coil_data == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,  _T("l_coil_data pointer is NULL"));
        return sts;
    }

    //-Patni-RUP/2009Aug24/Modified/cpp test corrections

    CCoil dummy_coil;
    l_coil_data->PqmCoilGetCoil(coilId, dummy_coil) ;
    PqmCoil_t* coil_p = dummy_coil.GetCoil();

    if (coil_p != NULL) {
        l_trace_string.Format(_T("SPEEDER Enable mode  %d.\n"), coil_p->speeder_enable);
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);

        if (coil_p->speeder_enable != 0) {
            sts |= VFC_INTENSITY_MAP_TYPE_SPEEDER ;

            if (SpeederWholeBodyCheck(coilId) == E_NO_ERROR)
                sts |= VFC_INTENSITY_MAP_TYPE_INTENSITY ;
        }

    } else {
        l_trace_string.Format(_T("Can't find CoilID 0x%08x."), coilId);
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);
    }

    //DB_FUNC_EXIT() ;
    return sts ;
}

//****************************Method Header************************************
//Method Name   :SpeederTimerSetup()
//Author        :PATNI / JS
//Purpose       :
//*****************************************************************************
void CPqmSpeeder::SpeederTimerSetup(
)
{
    //Complete this Meghana Uncomment this
    WNP_signal(SIGALRM, &CPqmSpeeder::SpeederTimeOut);
    WNP_alarm(m_speeder_timeout) ;
}

//****************************Method Header************************************
//Method Name   :SpeederDone()
//Author        :PATNI /MRP
//Purpose       :To read the Speeder Database
//*****************************************************************************
void CPqmSpeeder::SpeederDone(
    CPqmProtocol* const curObj,
    const int sts
)
{
    LPCTSTR FUNC_NAME = _T("CPqmSpeeder::SpeederDone");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of CPqmSpeeder::SpeederDone"));

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqm_ptr == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    CPQMStudy* l_study = m_pqm_ptr->GetStudy();
    CPqmPmAcqman* acq_mgr = m_pqm_ptr->GetAcqManager();
    CScanProcedure* scan_proc = acq_mgr->GetScanProcedure();

    CString l_trace_string(_T(""));

    SpeederTimerReset() ;		/* TIMER RESET */
    //PqmUiUnfreeze(PQM_FREEZE_FOR_SPEEDER);
    //Meghana for Wait Cursor
    m_pqm_ptr->WriteToUI(PQM_MSG_ENDWAIT_CURSOR);
    //Patni-PJS/2010Jan19/Added/MVC006827

    //Patni-HAR/2010Mar08/Added/MSA0251-00275
    m_pqm_ptr->SetPostProcessFlag(FALSE);

    m_pqm_ptr->WriteToUI(PQM_MSG_POST_PROCESS_END);




    if (sts == E_NO_ERROR) {
        {
            CPqmProtocol*	prObj = l_study->GetNextItem(curObj);

            //+Patni-HAR/2010Jan18/Added/DeFT#MVC006364
            if ((prObj == NULL) || prObj->GetChildThreadStatus() == CT_DELETE) {
                return;
            }

            //-Patni-HAR/2010Jan18/Added/DeFT#MVC006364

            if ((prObj != NULL)
                && !(prObj->GetHoldScan() & VFC_HOLD_PRE_SCAN)
                && (prObj->GetAcqStatus() == C_PR_STATUS_WAIT)) {
                //+Patni-MP/DKH/2011Feb23/Added/TMSC-REDMINE-1470
                if (!scan_proc->GetIsScanning())
                    l_study->CheckNextAutoProt(prObj);

                //-Patni-MP/DKH/2011Feb23/Added/TMSC-REDMINE-1470
            }
        }

    } else {
        //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
        //Patni-PJS/2009Dec10/Added/MVC005121
        m_pqm_ptr->WriteToUI(PQM_MSG_REMOVE_AUTO_SCAN);

        m_pqm_ptr->GetAMB()->PqmPmAbortAmb();

        CPQMLogMgr::GetInstance()->DisplayScanError(MSG_SET_SPEEDER_ERR, _T("MSG_SET_SPEEDER_ERR"),
                SCAN_ERR_NOTIFY, L"PQMSpeeder.cpp");

    }

}

//****************************Method Header************************************
//Method Name   :InitializeSpeeder()
//Author        :PATNI /MRP
//Purpose       :To read the Speeder Database
//*****************************************************************************
void CPqmSpeeder::InitializeSpeeder(
)
{

    LPCTSTR FUNC_NAME = _T("CPqmSpeeder::InitializeSpeeder");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of InitializeSpeeder"));

    /********* TIMEOUT時間の取得 *********/
    char* timebuf = getenv("SPEEDER_TIMEOUT");

    if (timebuf != NULL) {
        m_speeder_timeout = atoi(timebuf) ;

    } else {
        m_speeder_timeout = SPEEDER_TIMEOUT_DEFAULT;

    }

    //l_str_prot_no.Format(_T("%d"),f_protocol->GetProtocol());
    CString l_trace_string(_T(""));
    l_trace_string.Format(_T("Speeder Timeout = %d"), m_speeder_timeout);

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);

    {
        m_num_speeder = SpeederDB_read(&s_speederTbl);
        l_trace_string.Format(_T("SPEEDER DB  = %d"), m_num_speeder);
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);
    }
}

//****************************Method Header************************************
//Method Name   :IntensityCheck()
//Author        :PATNI /MRP
//Purpose       :To read the Speeder Database
//*****************************************************************************
int	CPqmSpeeder::IntensityCheck(
    CPqmProtocol* const obj,
    const mode_automap f_map_mode, /* NORMAL_MODE_M*/
    const float f_target_couch_pos /* 0.0f*/
)
{
    LPCTSTR FUNC_NAME = _T("CPqmSpeeder::IntensityCheck");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of IntensityCheck"));

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqm_ptr == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    CString l_trace_string(_T(""));
    CPqmCoilData*   l_coil_data = CDataManager::GetInstance()->GetPQMCoilData();

    //DB_FUNC_ENTER("[PqmIntensityCheck]:");
    l_trace_string.Format(_T("speeder_type = %d\n"), obj->GetSpeederType());

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);

    if (obj->GetMovingCouch() != VFC_MOVING_COUCH_NONE)
        return E_NO_ERROR ;

    //+Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
    status_t	sts = E_NO_ERROR;
    //-Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections


    /*************************************************
    3Dlocatorの場合はSPEEDER及び輝度補正ない
    そして再構成はMAP PATHあると困るらしい。　
    3Dlocatorの場合は無条件でMAP PATH削除する　
        V9.01   BugNo:00201
    **************************************************/
    if (obj->GetFluoro() ==  TRUE) {
        CPQMStudy* l_study = m_pqm_ptr->GetStudy();
        l_study->SpeederClearMapPath(obj->GetProtocol()) ;

    } else if (obj->GetSpeederType() != VFC_SPEEDER_TYPE_SPEEDER) {
        l_trace_string.Format(_T("Intensity_type = %d \n "), obj->GetIntensityType());

        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);

        CPqmProtocol* mapobj = NULL ;
        int speeder_mapProt_bak = obj->GetSpeederMapProtocol();

        //+NFD001/2010Jun30/AutoMap
        if (f_map_mode == MAP_AUTO_MODE) {
            sts = SpeederSearch(obj , &mapobj, f_map_mode, f_target_couch_pos) ;

        } else {
            sts = SpeederSearch(obj , &mapobj);
        }

        //-NFD001/2010Jun30/AutoMap
        bool_t	enable = 0;

        if (obj->GetIntensityType() == TRUE) {/* 輝度補正有りの場合 */
            /* MAPチェックの前に輝度補正可能コイルかどうかをチェックする */
            if (m_pqm_ptr->IsSaturnCoilSystem()) {	/* Saturn */

                enable = l_coil_data->GetSpeederInSaturn(obj->GetCoilDBTable(), MAP_INTENSITY_ENABLE);

            } else {

                CCoil dummy_coil;
                l_coil_data->PqmCoilGetCoil(obj->GetPqmPrMode()->GetRecvCoilId(), dummy_coil) ;
                PqmCoil_t*	coil_p = dummy_coil.GetCoil();

                if (coil_p) {
                    enable = coil_p->speeder_enable & MAP_INTENSITY_ENABLE;
                }
            }    /* 	Endif( PqmIsSaturnCoilSystem() ) */

            if (!enable) {	/* 輝度補正できないコイルの場合 */
                return PQM_WARN_SCAN_INTENSITY_COIL;
            }

            /* SpeederMAPが存在しない時は警告メッセージ表示する */
            if (sts != E_NO_ERROR) { /* Can't Find MAP seq. */
                l_trace_string = _T("Can't find INTENSITY MAP\n ");

                PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);

                if (sts == ERRCODE_TX_INTENSITY_AREA)
                    sts = PQM_WARN_SCAN_TX_INTENSITY_AREA ;

                else if (sts == ERRCODE_SPEEDER_AREA)
                    sts = PQM_WARN_SCAN_INTENSITY_AREA ;

                else if (sts == ERRCODE_SPEEDER_MAP)
                    sts = PQM_WARN_SCAN_INTENSITY_MAP ;

                else if (sts == ERRCODE_TX_DEGENERACY_MAP)
                    sts = PQM_WARN_SCAN_TX_DEGENERACY_MAP ;

                else if (ERRCODE_TX_INTENSITY_NOTFOUND == sts)
                    sts = PQM_WARN_TX_INTENSITY_NOTFOUND;

                else if (ERRCODE_INTENSITY_NOTFOUND == sts)
                    sts = PQM_WARN_INTENSITY_NOTFOUND;

                //-Patni-Sribanta/2011Mar07/Added/IR-184
                else
                    sts = PQM_WARN_SCAN_DEGENERACY_MAP;

            } else if (obj->GetSpeederType() == VFC_SPEEDER_TYPE_SPEEDER &&
                       speeder_mapProt_bak != obj->GetSpeederMapProtocol()) {
                l_trace_string.Format(_T("speeder_mapProt_bak = %d Intensity_mapProt = %d \n "),
                                      speeder_mapProt_bak, obj->GetSpeederMapProtocol());

                PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);

                obj->SetSpeederMapProtocol(speeder_mapProt_bak) ;
                sts = PQM_WARN_SCAN_INTENSITY_MAP ;
            }

        } else {	/* Speeder MAP PATHは常に設定する */
            sts = E_NO_ERROR ;
        }
    }

    //DB_FUNC_EXIT() ;
    return sts ;
}

//****************************Method Header************************************
//Method Name   :MovingBed_SpeederCheck()
//Author        :PATNI /MRP
//Purpose       :To read the Speeder Database
//*****************************************************************************
int	CPqmSpeeder::MovingBed_SpeederCheck(
    CPqmProtocol* const obj
)
{
    LPCTSTR FUNC_NAME = _T("CPqmSpeeder::MovingBed_SpeederCheck");

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of CPqmSpeeder::MovingBed_SpeederCheck"));

    if ((obj->GetMovingCouch() == VFC_MOVING_COUCH_NONE) ||
        ((obj->GetIntensityType() == FALSE) && (obj->GetSpeederType() == VFC_SPEEDER_TYPE_NONE)))
        return E_NO_ERROR ;

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqm_ptr == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    if (m_pqm_ptr->IsSaturnCoilSystem()) {	/* Saturn */
        /**	Saturnシステムの場合、PVコイルはサポートされない
        けれども、従来MovingBedとSPEEDERの併用は禁止しないといけない。
        **/
        if (obj->GetSpeederType() == VFC_SPEEDER_TYPE_NONE)
            return(PQM_WARN_SCAN_PV_MOVINGBED_INTEN) ;

        else
            return(PQM_WARN_SCAN_PV_MOVINGBED_SPEEDER) ;

    } else {

        int numPVcoil = 0, PVcoilTbl[PQM_MAX_MOVINGBED_SCAN] = {0}; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections;
        AcqGetPVcoilIDTable(PVcoilTbl, &numPVcoil);
        int l_recv_id = obj->GetPqmPrMode()->GetRecvCoilId();

        CString l_trace_string(_T(""));

        for (int j = 0 ; j < numPVcoil ; j++) {
            l_trace_string.Format(_T("[%d]:PVcoidId = 0x%x  vs 　RCVcoilID = 0x%x\n"), j, PVcoilTbl[j], obj->GetPqmPrMode()->GetRecvCoilId());

            PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);

            if ((IS_COIL_SAME(l_recv_id , PVcoilTbl[j]))) {
                /****************************************************
                   輝度補正の場合はOFFにしてそのまま撮像可能だが、
                   Speederの場合はエラーにする。　V6.20
                ****************************************************/
                if (obj->GetSpeederType() == VFC_SPEEDER_TYPE_NONE)
                    return(PQM_WARN_SCAN_PV_MOVINGBED_INTEN) ;

                else
                    return(PQM_WARN_SCAN_PV_MOVINGBED_SPEEDER) ;

                break ;
            }
        }
    }

    return E_NO_ERROR ;
}

//****************************Method Header************************************
//Method Name   :SpeederCheck()
//Author        :PATNI /MRP
//Purpose       :To read the Speeder Database
//*****************************************************************************
//Patni/AKR/2010July06/Modified/NFD001 AutoMap
//New parameters added with default values
int CPqmSpeeder::SpeederCheck(
    CPqmProtocol* const obj,
    const mode_automap map_mode,
    const float target_couch_pos
)
{
    LPCTSTR FUNC_NAME = _T("CPqmSpeeder::SpeederCheck");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of CPqmSpeeder::SpeederCheck"));

    status_t	sts = 0 ; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections

    int32_t	switch_coil_type = 0; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
    CString l_trace_string = _T("");
    CPqmCoilData*   l_coil_data = CDataManager::GetInstance()->GetPQMCoilData();

    l_trace_string.Format(_T("Intensity_type = %d, speeder_type=%d \n ")
                          , obj->GetIntensityType(), obj->GetSpeederType());
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqm_ptr == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return -1;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    CPQMStudy* l_study = m_pqm_ptr->GetStudy();
    int obj_receive_coil_id = obj->GetPqmPrMode()->GetRecvCoilId();

    int msg = -1;

    if ((obj->GetSpeederType() == VFC_SPEEDER_TYPE_NONE) &&
        (obj->GetIntensityType() == FALSE)) {
        //DB_FUNC_EXIT() ;
        return msg ;
    }

    int speeder_maptype = VFC_INTENSITY_MAP_TYPE_NONE ;

    CPqmProtocol* mapobj = NULL; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections

    if (obj->GetSpeederType() == VFC_SPEEDER_TYPE_SENSITIVITY) {

        l_trace_string.Format(_T("%s %d  = speeder_type SENSITIVITY"), __FILE__, __LINE__);

        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);

        if (l_study->GetVfDBHandle()) {
            /*  感度測定用撮影の場合、適応コイルの有無をチェックする */

            if (m_pqm_ptr->IsSaturnCoilSystem()) {	/* Saturn */
                speeder_maptype = MapCoilCheckSaturn(obj->GetCoilDBTable()) ;

            } else {
                speeder_maptype = MapCoilCheck(obj_receive_coil_id);
            }    /* 	Endif( PqmIsSaturnCoilSystem() ) */

            if (speeder_maptype == VFC_INTENSITY_MAP_TYPE_NONE) {
                msg =  ERR_ILLEGAL_SPEEDER_COIL ;
                return msg ;
            }

            if (speeder_maptype & VFC_INTENSITY_MAP_TYPE_INTENSITY) {
                PqmCoil_t*       coil_p = NULL; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
                int				coilTbl[4] = {0} ; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
                CCoil dummy_coil;

                if (m_pqm_ptr->IsSaturnCoilSystem()) {	/* Saturn */
                    if (!l_coil_data->GetRWBInSaturn(obj->GetCoilDBTable(), RWB_SPEEDER, dummy_coil)) {
                        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetRWBInSaturn failed"));
                    }

                    coil_p = dummy_coil.GetCoil();

                    if (coil_p == NULL) {	/* 対象のWBコイルが見つからない */
                        msg =  ERR_ILLEGAL_SPEEDER_COIL ;
                        return msg ;
                    }

                    memset(coilTbl , NULL , sizeof(coilTbl)) ;

                    if (SpeederSetWBCoilSaturn(obj, coil_p) == FALSE) {	/* 対象のWBコイルが見つからない */
                        msg =  ERR_ILLEGAL_SPEEDER_COIL ;
                        return msg ;
                    }

                } else {  /* 古いRFコイルの処理(古いとはVantageシステム) */

                    l_coil_data->PqmCoilGetCoilRWB(obj_receive_coil_id, dummy_coil);
                    coil_p = dummy_coil.GetCoil();

                    if (coil_p) {
                        /* V5.00    WholeBodyCoil での撮影も可能か？
                          もし可能ならDynamic Scanにして感度MAP撮影も同時に行うことにする。
                          SVN_PRC_INTENSITY_MAP_TYPE にMAP TYPE 設定する
                         */
                        /* V6.20以降、Speeder MAPの高速収集の為、　2003/09/19
                          SpeederMAP収集はカバレージ方式からDynamic方式へ変更 */
                        coilTbl[0] = obj_receive_coil_id ;
                        coilTbl[1] = coil_p->coil_id;

                    } else {	/* 対象のWBコイルが見つからない */
                        msg =  ERR_ILLEGAL_SPEEDER_COIL ;
                        return msg ;
                    }
                }	/* 	Endif( PqmIsSaturnCoilSystem() ) */

                /* 高速コイル切り替えの場合はダイナミック撮像しない　V9.25 */

                switch_coil_type = l_study->SpeederCoilMode(obj->GetProtocol());

                if (switch_coil_type == VFC_SWITCH_COIL_TYPE_NONE) {

                    //+Patni-NP/2010Jul27/Added/IR-161
                    if (coil_p->receiveWB_enable & RWB_MAP_DYNAMIC) {

                        sts = l_study->SpeederSetDynamic(obj->GetProtocol() , coilTbl) ;

                        if (sts != E_NO_ERROR) {
                            l_trace_string.Format(_T("%s %d  PqmSpeederCopySubProtocol Error !! \n"), __FILE__, __LINE__);
                            //+Patni-PJS/2010Mar11/Added/DeFT# MCM0229-00178
                            PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                                      l_trace_string);


                            msg = ERR_STUDY ;
                        }
                    }



                    else {
                        /* インライン方式でコイルを切り替える */
                        if (!m_pqm_ptr->IsSaturnCoilSystem()) {
                            /* Atlasでない場合はエラーとする（高速コイル切り替えはできない） */
                            //	msg = ERR_SPEEDER_SWITCHCOIL ;
                            //	ERROR_LOG("COILDB receiveWB_enable Error. system is different.");

                            //Need to confirm
                            CPQMLogMgr::GetInstance()->DisplayError(
                                CPqmUtilities::GetMultiLingualString(_T("IDS_SWITCH_COIL_TYPE_SYSTEM_ERROR")),
                                _T("Speeder.cpp"), __LINE__, ERROR_LOG_CRUCIAL1 ,
                                ERR_ID, APP_NAME);

                        } else {
                            /* Inline用に切り替える */
                            sts = SpeederOnSwitchCoil(l_study->GetVfDBHandle(), obj->GetProtocol());

                            if (sts != E_NO_ERROR) {

                                //Need to confirm
                                CPQMLogMgr::GetInstance()->DisplayError(
                                    CPqmUtilities::GetMultiLingualString(_T("IDS_SWITCH_COIL_TYPE_SYSTEM_ERROR")),
                                    _T("Speeder.cpp"), __LINE__, ERROR_LOG_CRUCIAL1 ,
                                    ERR_ID, APP_NAME);
                            }

                            sts = pqmSpeederChangeSwitchCoil(l_study->GetVfDBHandle(), obj->GetProtocol(),
                                                             coil_p->receiveWB_enable);

                            if (sts != E_NO_ERROR) {

                                CPQMLogMgr::GetInstance()->DisplayError(
                                    CPqmUtilities::GetMultiLingualString(_T("IDS_SWITCH_COIL_TYPE_SYSTEM_ERROR")),
                                    _T("Speeder.cpp"), __LINE__, ERROR_LOG_CRUCIAL1 ,
                                    ERR_ID, APP_NAME);
                            }
                        }
                    }

                    //-Patni-NP/2010Jul27/Added/IR-161

                } else if ((switch_coil_type == VFC_SWITCH_COIL_TYPE_MAP) || (switch_coil_type == VFC_SWITCH_COIL_TYPE_MAP_PAC)) { //+Patni-NP/2010Jul27/Modified/IR-161
                    //+Patni-NP/2010Jul27/Added/IR-161

                    if (coil_p->receiveWB_enable & RWB_MAP_DYNAMIC) {
                        /* ダイナミック方式でコイルを切り替える */
                        /* 高速コイル切り替え情報をOFFにする。 */
                        sts = SpeederOffSwitchCoil(l_study->GetVfDBHandle(), obj->GetProtocol());

                        if (sts != E_NO_ERROR) {

                            l_trace_string.Format(_T("%s %d  pqmSpeederOffSwitchCoil Error !! \n"), __FILE__, __LINE__);

                            PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);
                        }

                        sts = l_study->SpeederSetDynamic(obj->GetProtocol(), coilTbl) ;

                        if (sts != E_NO_ERROR) {

                            l_trace_string.Format(_T("%s %d  PqmSpeederCopySubProtocol Error !! \n"), __FILE__, __LINE__);

                            PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);
                        }
                    }

                    else {


                        if (!m_pqm_ptr->IsSaturnCoilSystem()) {
                            /* Atlasでない場合はエラーとする（高速コイル切り替えはできない） */
                            msg = ERR_PROTOCOL ;
                            CPQMLogMgr::GetInstance()->DisplayError(
                                CPqmUtilities::GetMultiLingualString(_T("IDS_SWITCH_COIL_TYPE_SYSTEM_ERROR")),
                                _T("Speeder.cpp"), __LINE__, ERROR_LOG_CRUCIAL1 ,
                                ERR_ID, APP_NAME);
                        }

                        else {
                            sts = pqmSpeederChangeSwitchCoil(l_study->GetVfDBHandle(), obj->GetProtocol(),
                                                             coil_p->receiveWB_enable);

                            if (sts != E_NO_ERROR) {
                                l_trace_string.Format(_T("%s %d  pqmSpeederChangeSwitchCoil Error !! \n"), __FILE__, __LINE__);

                                PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);
                            }

                        }
                    }

                } else {	/* フィールドの値が異常な場合 */
                    msg = ERR_PROTOCOL ;
                    CPQMLogMgr::GetInstance()->DisplayError(
                        CPqmUtilities::GetMultiLingualString(_T("IDS_SWITCH_COIL_TYPE_FIELD_ERROR")),
                        _T("Speeder.cpp"), __LINE__, ERROR_LOG_CRUCIAL1 ,
                        ERR_ID, APP_NAME);

                }//-Patni-NP/2010Jul27/Added/IR-161

            } else if (speeder_maptype == VFC_INTENSITY_MAP_TYPE_SPEEDER) {
                switch_coil_type = l_study->SpeederCoilMode(obj->GetProtocol());

                if (switch_coil_type) { //-Patni-NP/2010Jul27/Added/IR-161
                    sts = SpeederOffSwitchCoil(l_study->GetVfDBHandle(), obj->GetProtocol());

                    if (sts != E_NO_ERROR) {
                        msg = ERR_STUDY ;
                        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                                  _T("PqmSpeederOffSwitchCoil Error"));
                    }

                } else if ((switch_coil_type != VFC_SWITCH_COIL_TYPE_NONE)) {
                    msg = ERR_PROTOCOL ;
                    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("SWITCH_COIL_TYPE Error. Field is different."));
                }
            }

            //-Patni-PJS/2010Mar11/Added/DeFT# MCM0229-00178

        } else {
            l_trace_string.Format(_T("Can't open new study [%s]"), l_study->GetStudyName());
            PQM_TRACE(SYSTEM_FUNC_MARKER,
                      _T("CPqmSpeeder::SpeederCheck"),
                      l_trace_string);

            msg = ERR_STUDY ;
        }

        //-Patni-PJS/2010Mar11/Added/DeFT# MCM0229-00178

    } else if (obj->GetSpeederType() == VFC_SPEEDER_TYPE_SPEEDER) {
        /*  Speeder本撮影の場合、適応コイルの有無をチェック
        感度MAPデータを検索し、
        撮影領域が含まれてるか、寝台位置が同じかチェックする。
        感度MAPデータ検索方法:
        基本的には最新の感度測定撮影を使用するが、
        特殊なシーケンスに関しては対応したシーケンス種の物を使う */

        if (!IsValidSpeederCoil(obj)) {
            return ERR_ILLEGAL_SPEEDER_COIL ;
        }

        //+Patni/NFD001/2010Jun30/AutoMap
        sts = SpeederSearch(obj , &mapobj, map_mode, target_couch_pos) ;

        if (sts != E_NO_ERROR) {    /* Can't Find MAP seq. */
            l_trace_string = _T("Can't find SPEEDER MAP\n ");

            PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);

            if (sts == ERRCODE_SPEEDER_AREA)
                msg = ERR_OUT_RANGE_SPEEDER ;

            else if (sts == ERRCODE_SPEEDER_DEGENERACY)
                msg = ERR_SPEEDER_DEGENERACY ;

            //+Patni-Sribanta/2011Mar07/Added/IR-184
            else if (sts == ERRCODE_TX_INTENSITY_NOTFOUND)
                msg = ERR_TX_INTENSITY_NOTFOUND;

            else if (sts == ERRCODE_INTENSITY_NOTFOUND)
                msg = ERR_INTENSITY_NOTFOUND;

            else
                msg = ERR_CAN_NOT_SPEEDER_MAP;

        } else {
            l_trace_string = _T("SPEEDER MAP FIND\n ");
            PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);

            /********************************************************
                SPEEDER最大高速化率のチェックはSaturnシステムで
                より複雑になったので、従来のチェックと分ける
                PqmSpeederMaxRatioCheckの中で分ける。
            *********************************************************/

            /* SPEEDER最大高速化率のチェックを行う */
            if (SpeederMaxRatioCheck(obj)) {	/* Speeder Ratio error !!   V6.20 */
                msg = ERR_SPEEDER_MAX_RATIO ;
            }
        }
    }

    l_study->SpeederSetMapType(obj->GetProtocol(), speeder_maptype) ;
    //DB_FUNC_EXIT() ;
    return msg ;
}

// Added By KT/ACE-2/15-06-2009/Multilingualisation
//****************************Method Header************************************
//Method Name   :GetErrorString()
//Author        :PATNI /KT
//Purpose       :Get Error String
//*****************************************************************************
CString	CPqmSpeeder::GetErrorString(
    const int error_id
)
{
    CString err_str = _T("");

    switch (error_id) {
        case ERR_ILLEGAL_SPEEDER_COIL :
            err_str = _T("ERR_ILLEGAL_SPEEDER_COIL");
            break;

        case ERR_STUDY :
            err_str = _T("ERR_STUDY");
            break;

        case ERR_PROTOCOL :
            err_str = _T("ERR_PROTOCOL");
            break;

        case ERR_OUT_RANGE_SPEEDER :
            err_str = _T("ERR_OUT_RANGE_SPEEDER");
            break;

        case ERR_SPEEDER_DEGENERACY :
            err_str = _T("ERR_SPEEDER_DEGENERACY");
            break;

        case ERR_CAN_NOT_SPEEDER_MAP :
            err_str = _T("ERR_CAN_NOT_SPEEDER_MAP");
            break;

        case ERR_SPEEDER_MAX_RATIO :
            err_str = _T("ERR_SPEEDER_MAX_RATIO");
            break;

        default :
            err_str = _T("");
            break;
    }

    return err_str;
}

//****************************Method Header************************************
//Method Name   :SetPqmPtr()
//Author        :PATNI / JS
//Purpose       :
//*****************************************************************************
void CPqmSpeeder::SetPqmPtr(
    CPqm* f_pqm_ptr
)
{
    m_pqm_ptr = f_pqm_ptr;
}


bool CPqmSpeeder::IsValidSpeederCoil(
    CPqmProtocol* const f_protocol_obj
)
{

    LPCTSTR FUNC_NAME = _T("CPqmSpeeder::IsValidSpeederCoil");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (!m_pqm_ptr) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));
        return false;
    }

    if (m_pqm_ptr->IsSaturnCoilSystem()) {	/* Saturn */

        //How to get the receiver coil from the protocol
        CPqmCoilData* l_coil_data = CDataManager::GetInstance()->GetPQMCoilData();

        if (l_coil_data->GetSpeederInSaturn(f_protocol_obj->GetCoilDBTable(), MAP_SPEEDER_CORR_ENABLE) == FALSE) {
            return false;
        }

    } else {

        const int obj_receive_coil_id = f_protocol_obj->GetPqmPrMode()->GetRecvCoilId();

        if (SpeederCoilCheck(obj_receive_coil_id) != E_NO_ERROR) {
            return false;
        }
    }

    return true;
}

bool CPqmSpeeder::CanSupportB1Correction(
    CPqmProtocol* const f_protocol_obj
) const
{
    LPCTSTR FUNC_NAME = _T("CPqmSpeeder::CanSupportB1Correction");
    CPQMStudy* l_study = m_pqm_ptr->GetStudy();

    if (!l_study) {
        return false;
    }

    ClibmsAdapter l_libmsAdapter(l_study->GetVfDBHandle());
    status_t l_stsB1intensity = l_libmsAdapter.msCheckB1Intensity(f_protocol_obj->GetProtocol());

    if (E_ERROR == l_stsB1intensity) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("msCheckB1Intensity() returns E_ERROR"));

    } else {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("msCheckB1Intensity() returns E_NO_ERROR"));
    }

    return CanSupportB1Anatomy(f_protocol_obj) &&  CheckB1ConfigValue() &&
           l_study->CheckIntensityCorrFlag(f_protocol_obj->GetProtocol()) &&
           (E_NO_ERROR == l_stsB1intensity);
}

//****************************Method Header************************************
//Method Name   :PqmSpeederCoilCheck()
//Author        :PATNI /MRP
//Purpose       :コイル情報から撮影可能なMAP種を取得する
//*****************************************************************************
status_t CPqmSpeeder::SpeederCoilCheck(
    const int coilId
)
{

    LPCTSTR FUNC_NAME = _T("CPqmSpeeder::SpeederCoilCheck");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of CPqmSpeeder::SpeederCoilCheck"));

    CString l_trace_string(_T(""));
    l_trace_string.Format(_T("CoilID = 0x%08x"), coilId);

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);

    CCoil dummy_coil;
    CPqmCoilData*   l_coil_data = CDataManager::GetInstance()->GetPQMCoilData();

    //+Patni-RUP/2009Aug20/Modified/cpp test corrections
    if (l_coil_data == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("l_coil_data pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-RUP/2009Aug20/Modified/cpp test corrections

    l_coil_data->PqmCoilGetCoil(coilId , dummy_coil) ;

    PqmCoil_t* coil_p = dummy_coil.GetCoil();

    if (coil_p != NULL) {
        l_trace_string.Format(_T("SPEEDER Enable mode  %d"), coil_p->speeder_enable);
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);

        if ((coil_p->speeder_enable & MAP_SPEEDER_CORR_ENABLE) == MAP_SPEEDER_CORR_ENABLE) {
            //DB_FUNC_EXIT() ;
            return E_NO_ERROR ;
        }

    } else {
        //CString l_trace_string;
        l_trace_string.Format(_T("Can't find CoilID = 0x%08x"), coilId);
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);
    }

    //DB_FUNC_EXIT() ;
    return E_ERROR ;
}

//****************************Method Header************************************
//Method Name   :SpeederWholeBodyCheck()
//Author        :PATNI /MRP
//Purpose       :コイル情報から撮影可能なMAP種を取得する
//*****************************************************************************
status_t CPqmSpeeder::SpeederWholeBodyCheck(
    const int coilId
)
{

    LPCTSTR FUNC_NAME = _T("CPqmSpeeder::SpeederWholeBodyCheck");
    //DB_FUNC_ENTER("[SpeederWholeBodyCheck]:");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of CPqmSpeeder::SpeederWholeBodyCheck"));

    CString l_trace_string(_T(""));
    l_trace_string.Format(_T("CoilID = 0x%08x"), coilId);
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);

    CPqmCoilData*   l_coil_data = CDataManager::GetInstance()->GetPQMCoilData();

    //+Patni-RUP/2009Aug20/Modified/cpp test corrections
    if (l_coil_data == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("l_coil_data pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-RUP/2009Aug20/Modified/cpp test corrections

    CCoil dummy_coil;
    l_coil_data->PqmCoilGetCoilRWB(coilId, dummy_coil);
    PqmCoil_t* coil_p = dummy_coil.GetCoil();

    if (coil_p != NULL) {
        l_trace_string.Format(_T("receiveWB_enable mode  %d."), coil_p->receiveWB_enable);
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);

        if (coil_p->receiveWB_enable & RWB_SPEEDER) {
            //DB_FUNC_EXIT() ;
            return E_NO_ERROR ;
        }

    } else {
        CString l_trace_string(_T(""));
        l_trace_string.Format(_T("Can't find CoilID = 0x%08x"), coilId);
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);

    }

    //DB_FUNC_EXIT() ;
    return E_ERROR ;
}

//****************************Method Header************************************
//Method Name   :SpeederMAPCoilCheck()
//Author        :PATNI /MRP
//Purpose       :To read the Speeder Database
//*****************************************************************************
BOOL CPqmSpeeder::SpeederMAPCoilCheck(
    CPqmProtocol* const curObj ,
    CPqmProtocol* const mapObj
)const
{

    LPCTSTR FUNC_NAME = _T("CPqmSpeeder::SpeederMAPCoilCheck");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of CPqmSpeeder::SpeederMAPCoilCheck"));

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqm_ptr == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return FALSE;
    }

    CCoilChannelMode* l_coilchannle_mode = m_pqm_ptr->GetCoilChannelMode();

    if (l_coilchannle_mode != NULL && l_coilchannle_mode->IsCoilChannelModeApplicable()) {
        if (curObj->GetcoilChannelMode() != mapObj->GetcoilChannelMode()) {
            CString msg = _T("");
            msg.Format(_T("Current object channel mode = %d and map object channel mode = %d"), curObj->GetcoilChannelMode(), mapObj->GetcoilChannelMode());

            PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, msg);
            return FALSE;
        }
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    if (m_pqm_ptr->IsSaturnCoilSystem()) {	/* Saturn */

        //+Patni-RUP/2009Aug24/Modified/cpp test corrections
        //CPqmCoilData   *l_coil_data = CDataManager::GetInstance()->GetPQMCoilData();

        CDataManager* l_data_manager = CDataManager::GetInstance();

        if (l_data_manager == NULL) {
            PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("l_data_manager is NULLL"));
            return FALSE;
        }

        CPqmCoilData* l_coil_data = l_data_manager->GetPQMCoilData();

        if (l_coil_data == NULL) {
            PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("l_coil_data is NULLL"));
            return FALSE;
        }

        //-Patni-RUP/2009Aug24/Modified/cpp test corrections
        return l_coil_data->PqmIsSameCoilInSaturnforSPEEDER(curObj->GetSaturnCoil() , mapObj->GetSaturnCoil());

    } else {
        return (curObj->GetPqmPrMode()->GetRecvCoilId() == mapObj->GetPqmPrMode()->GetRecvCoilId());
    }

    // return E_NO_ERROR;   //-Patni-RUP/2009Aug24/Modified/cpp test corrections
}
//+Patni/AKR/2010July06/Modified/NFD001 AutoMap
//New parameters added with default values
//****************************Method Header************************************
//Method Name   : SpeederSearch()
//Author        : PATNI /MRP
//Purpose       : To read the Speeder Database
//*****************************************************************************
status_t CPqmSpeeder::SpeederSearch(
    CPqmProtocol* curObj,
    CPqmProtocol** mapObj,
    const mode_automap map_mode,
    const float target_couch_pos
)
{
    LPCTSTR   FUNC_NAME = _T("CPqmSpeeder::SpeederSearch");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of SpeederSearch"));

    if (mapObj == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("mapObj pointer is NULL"));
        return E_ERROR;
    }

    //+Patni-AMT/2010Feb16/Modified/Ph-3# PROPOSAL_07-Code Review
    bool_t          areaOB = 0 ;

    *mapObj = NULL;

    if (m_pqm_ptr == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return E_ERROR;
    }

    CPQMStudy* l_study = m_pqm_ptr->GetStudy();

    if (l_study == NULL) {
        return E_ERROR;
    }

    if (curObj->GetSpeederType() == VFC_SPEEDER_TYPE_SPEEDER &&
        curObj->GetSaturnCoil()->DegeneracyFlg == TRUE) {
        return ERRCODE_SPEEDER_DEGENERACY;
    }

    //-Patni-AMT/2010Feb16/Modified/Ph-3# PROPOSAL_07-Code Review

    /*******************************************************
      現在の位置からさかのぼって適応可能な感度撮影を探す。
      FOVが感度領域内か、感度MAPは存在するか？
    ********************************************************/

    CPqmProtocol*    prObj = curObj ;

    CString l_trace_string(_T(""));

    //+Patni-Sribanta/2011Mar24/Modified/IR-184

    //+Patni-Sribanta/2011Mar07/Added/IR-184
    const bool l_intensity_corr_mode = (curObj->GetProtcolCorrStatus() == VFC_INTENSITY_CORR_STATUS_TX_RX);
    //-Patni-Sribanta/2011Mar07/Added/IR-184

    bool l_intensity_corr_flag = l_intensity_corr_mode && CanSupportB1Correction(curObj) ;

    while (1) {
        //+Patni-AMT/2010Feb16/Modified/Ph-3# PROPOSAL_07
        l_trace_string.Format(_T("While %s:%d  %d\n"), __FILE__, __LINE__,
                              prObj->GetProtocol());
        PQM_TRACE1(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);

        if ((prObj = l_study->GetPrevItemFromPos(prObj)) == NULL) {
            //DB_PRINTF("S",("%s:%d \n",__FILE__,__LINE__));
            l_trace_string.Format(_T("%s:%d \n"), __FILE__, __LINE__);
            PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);
            break;
        }

        //-Patni-AMT/2010Feb16/Modified/Ph-3# PROPOSAL_07

        if (prObj->GetSpeederType() == VFC_SPEEDER_TYPE_SENSITIVITY &&
            SpeederMAPCoilCheck(prObj , curObj) &&
            prObj->GetPqmPrMode()->GetScanNumber() == curObj->GetPqmPrMode()->GetScanNumber()) {


            /***** 感度MAP撮影見つかった *****/
            //DB_PRINTF("S",("SPEEDER_TYPE_SENSITIVITY EXIST   %d\n",prObj->prot));
            if (l_study->SpeederMAPExist(prObj->GetProtocol()) == E_NO_ERROR) {
                /***** 感度MAPが本当に存在している *****/
                //+Patni/AKR/2010July06/Modified/NFD001 AutoMap
                //call by increased parameters
                //+Patni-Sribanta/2011Mar07/Added/IR-184
                if (l_intensity_corr_flag) {
                    if ((prObj->GetPqmPrMode()->GetAnatomy() != curObj->GetPqmPrMode()->GetAnatomy()) ||
                        (!l_study->CheckIntensityCorrMapStatus(prObj->GetProtocol()))) {
                        continue;
                    }
                }

                //-Patni-Sribanta/2011Mar07/Added/IR-184
                if (SpeederCouchPosiCheck(curObj->GetPqmPrMode()->GetCouchPos(), prObj->GetProtocol(), map_mode, target_couch_pos) == E_NO_ERROR) {	/*  寝台移動時の許容範囲チェックもOK  */
                    CCheckFOVInMap l_checkFov;

                    if (l_checkFov.PqmCheckFOVinMap(l_study->GetVfDBHandle(), curObj->GetProtocol(),
                                                    prObj->GetProtocol(), MS_CHECK_SLAB_CENTER_MODE) == E_NO_ERROR) {/***** 撮影領域も感度MAP撮影に含まれる これを使う！！　 *****/
                        status_t sts = l_study->SpeederSetMapPath(
                                           curObj->GetProtocol(), prObj->GetProtocol()) ;

                        if (sts == E_NO_ERROR) {
                            curObj->SetSpeederMapProtocol(prObj->GetProtocol()) ;
                            *mapObj = prObj ;

                            //+Patni-Sribanta/2011Mar07/Added/IR-184
                            if (l_intensity_corr_mode) {
                                if (!l_intensity_corr_flag) {
                                    sts = ERRCODE_TX_INTENSITY_NOTFOUND;
                                }
                            }

                            //-Patni-Sribanta/2011Mar07/Added/IR-184

                        } else {
                            l_study->SpeederClearMapPath(curObj->GetProtocol()) ;
                            sts =  ERRCODE_SPEEDER_AREA ;
                        }

                        //DB_FUNC_EXIT() ;
                        return sts  ;

                    } else {
                        areaOB = TRUE;
                    }
                }
            }
        }
    }

    //-Patni-Sribanta/2011Mar24/Modified/IR-184

    l_study->SpeederClearMapPath(curObj->GetProtocol()) ;

    //DB_FUNC_EXIT() ;
    if (curObj->GetSaturnCoil()->DegeneracyFlg == TRUE) {
        if ((curObj->GetProtcolCorrStatus() == VFC_INTENSITY_CORR_STATUS_RX) && (!l_intensity_corr_flag))
            return 	ERRCODE_SPEEDER_DEGENERACY_MAP;

        else
            return  ERRCODE_TX_DEGENERACY_MAP;

    } else if (areaOB == TRUE) {
        if ((curObj->GetProtcolCorrStatus() == VFC_INTENSITY_CORR_STATUS_RX) && (!l_intensity_corr_flag))
            return ERRCODE_SPEEDER_AREA ;

        else
            return 	ERRCODE_TX_INTENSITY_AREA;

        //+Patni-Sribanta/2011Mar24/Modified/IR-184



    } else if ((l_intensity_corr_flag || l_intensity_corr_mode)) {
        return  ERRCODE_TX_INTENSITY_NOTFOUND ;
        //-Patni-Sribanta/2011Mar24/Modified/IR-184

    } else if ((!l_intensity_corr_flag) && (curObj->GetProtcolCorrStatus() == VFC_INTENSITY_CORR_STATUS_RX)) {
        return 	ERRCODE_INTENSITY_NOTFOUND;

    } else {
        return ERRCODE_SPEEDER_MAP ;
    }
}

//****************************Method Header************************************
//Method Name   :SpeederSetWBCoilSaturn()
//Author        :PATNI /MRP
//Purpose       :To read the Speeder Database
//*****************************************************************************
status_t CPqmSpeeder::SpeederSetWBCoilSaturn(
    CPqmProtocol* const obj,
    PqmCoil_t* const coil_p
)
{
    LPCTSTR FUNC_NAME = _T("CPqmSpeeder::SpeederSetWBCoilSaturn");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (m_pqm_ptr == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return E_ERROR;
    }

    /* 	ターゲットコイルは従来コイルや新RFコイルどちらでも、
    WholeBodyは従来コイルのコイル情報になる	*/

    if ((coil_p->mview & 0x80000000)) {	/* 従来コイルなのでPATHDB情報を設定する */

        path_db_rec_t       pathP ;
        char                CoilName[32 * 2] = {0};
        VftSectionRecvrCoil_t   RcvCoil[MAX_SELECT_COIL] = {0}; /* Recver Coil */
        VftSectionSignal_t      Signal[MAX_SIGNAL_COIL] = {0};  /* Signal */
        CPqmCoilData*            l_coil_data = CDataManager::GetInstance()->GetPQMCoilData();

        memset(Signal , NULL , sizeof(Signal)) ;
        memset(RcvCoil , NULL , sizeof(RcvCoil)) ;
        memset(CoilName , NULL , sizeof(CoilName)) ;

        if (l_coil_data == NULL) {
            PQM_TRACE(SYSTEM_FUNC_MARKER,  FUNC_NAME, _T("l_coil_data pointer is NULL"));
            return FALSE;
        }

        if (l_coil_data->PqmCoilGetPathDB(coil_p->mview & COIL_PHYSICALID_MASK, &pathP) == FALSE) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PqmCoilGetPathDB failed"));
            return FALSE ;
        }

        /**  ここでMviewから　PathDB情報を取得して情報を設定する　**/
        int	i = 0, signal = 0 ; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
        signal = pathP.path_data[0];

        for (int j = i = 0 ; i < MAX_SIGNAL_COIL ; i++) {
            if (signal & (0x0001 << i)) {
                Signal[j].signal_no[j] = signal & (0x0001 << i) ;
                Signal[j].num++ ;
            }
        }

        /* 受信コイル名のテーブル作成を行う */
        PqmSaturnCoil_t* l_sat_coil = obj->GetSaturnCoil();
        memcpy(CoilName , l_sat_coil->CoilName, strlen(l_sat_coil->CoilName)) ;
        i = strlen(l_sat_coil->CoilName);
        CoilName[i++] =  ';' ;
        memcpy(&CoilName[i] , coil_p->label, (strlen(coil_p->label) + 1)) ;
        int numSection = 1 ;
        //+Patni-AMT/2009Oct31/Modified/DeFT/MCM0229-00153
        RcvCoil[0].portID[0] =  coil_p->port;

        if (RcvCoil[0].portID[0] == 'A') {
            RcvCoil[0].portID[1] =  ' ';    //Space

        } else {
            // Breast Coil Support
            RcvCoil[0].portID[1] = l_sat_coil->RcvCoil[0].portID[1];
        }

        //-Patni-AMT/2009Oct31/Modified/DeFT/MCM0229-00153

        RcvCoil[0].coilID =  coil_p->coil_id & COIL_PHYSICALID_MASK ;
        RcvCoil[0].channel =  coil_p->channel;

        /* SVN_PLN_SECTION_RECVR_COIL_SIGNAL2 はどうするのか？ */
        CPQMStudy* l_study = m_pqm_ptr->GetStudy();
        l_study->PutSaturnCoil2(obj->GetProtocol(), numSection, RcvCoil, Signal, CoilName);

        return TRUE ;
    }

    return FALSE ;
}
void CPqmSpeeder::TimeOut(int sig)
{
    //Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(sig);

}
//****************************Method Header************************************
//Method Name   :SpeederTimerReset()
//Author        :PATNI /MRP
//Purpose       :To read the Speeder Database
//*****************************************************************************
void CPqmSpeeder::SpeederTimerReset(
    bool f_Timeout
)
{

    LPCTSTR FUNC_NAME = _T("CPqmSpeeder::SpeederTimerReset");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of CPqmSpeeder::SpeederTimerReset"));

    WNP_alarm(0) ;		/* TIMER RESET */

    if (f_Timeout) {
        //+Patni-MJC/2009Aug17/Modified/cpp test corrections
        if (m_pqm_ptr == NULL) {
            PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
            return;
        }

        //-Patni-MJC/2009Aug17/Modified/cpp test corrections
        CPqmPmAcqman* acq_mgr = m_pqm_ptr->GetAcqManager();
        CScanProcedure* scan_proc = acq_mgr->GetScanProcedure();
        CPQMStudy* l_study = m_pqm_ptr->GetStudy();

        //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
        CPQMLogMgr::GetInstance()->DisplayScanError(MSG_SET_SPEEDER_TIMEOUT, _T("MSG_SET_SPEEDER_TIMEOUT"),
                SCAN_ERR_NOTIFY, L"PQMSpeeder.cpp");

        l_study->SetScanModeOfHead();
        scan_proc->ResetCurrentRequest();
        //+Patni-MP/2009Mar28/Added/PSP Internal Defect
        scan_proc->SendMessagrToUIForEndWaitCursor();
        //+Patni-MP/2009Mar28/Added/PSP Internal Defect

        //Patni-HAR/2010Mar08/Added/MSA0251-00275
        m_pqm_ptr->SetPostProcessFlag(FALSE);
        m_pqm_ptr->WriteToUI(PQM_MSG_POST_PROCESS_END);
        m_pqm_ptr->WriteToUI(PQM_MSG_REMOVE_AUTO_SCAN);
    }

    WNP_signal(SIGALRM, SIG_DFL) ;
}
//****************************Method Header************************************

//Method Name   :SpeederCouchPosiCheck()

//Author        :PATNI /MRP

//Purpose       :To read the Speeder Database

//*****************************************************************************
//Patni/AKR/2010July06/Modified/NFD001 AutoMap
//New parameters added with default values
status_t CPqmSpeeder::SpeederCouchPosiCheck(
    const float Current_couchPosi ,
    const int mapProt,
    const mode_automap map_mode,
    const float target_couch_pos
)
{
    LPCTSTR FUNC_NAME = _T("CPqmSpeeder::SpeederCouchPosiCheck");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of SpeederCouchPosiCheck"));

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections

    if (m_pqm_ptr == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections


    /* カレントのPROTOCOLを取得する */
    // + SM4 SPEEDER_HANG HIMANSHU 20090121
    //CurObj = l_study->GetCurrentProtocol() ;
    CPqmProtocol* CurObj = m_pqm_ptr->GetAcqManager()->GetScanProcedure()->GetScanProcProtocol();
    // - SM4 SPEEDER_HANG HIMANSHU 20090121


    /* SPEEDER 本スキャン有無をチェックする */
    if ((CurObj->GetIntensityType() == TRUE  ||
         CurObj->GetSpeederType()  == VFC_SPEEDER_TYPE_SPEEDER)) {

        CString l_trace_string(_T(""));
        l_trace_string.Format(_T("******* %s:%d **** Port=%d****\n"),
                              __FILE__, __LINE__, mapProt);
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);

        /* 感度MAP PORT取得 */
        CPQMStudy* l_study = m_pqm_ptr->GetStudy();
        CPqmProtocol* mapObj = l_study->GetItemFromProtocol(mapProt);

        if (mapObj == NULL) {
            DB_FUNC_EXIT() ;
            return E_ERROR ;
        }

        l_trace_string.Format(_T("******* %s:%d ****Current_Posi=%f Map_Posi=%f****\n"),
                              __FILE__, __LINE__, Current_couchPosi, mapObj->GetPqmPrMode()->GetCouchPos());
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);

        /* 対応する感度MAP撮影を求め、CouchPositionの差が許容範囲かチェックする */
        int		rfsLimit = 0; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections

        if (m_pqm_ptr->IsSaturnCoilSystem()) {	/* Saturn */
            rfsLimit = SpeederGetrfsLimitInSaturn(s_speederTbl, m_num_speeder, CurObj->GetCoilDBTable());

        } else {
            spdb_rec_p   speedertbl = SpeederGetCoil(s_speederTbl, m_num_speeder, CurObj->GetPqmPrMode()->GetRecvCoilId());

            if (speedertbl) {
                rfsLimit = speedertbl->rfsLimit ;
            }
        }

        l_trace_string.Format(_T("******* %s:%d ****Current_Posi=%f Map_Posi=%f Limit=%d ****\n"),
                              __FILE__, __LINE__, Current_couchPosi, mapObj->GetPqmPrMode()->GetCouchPos(), rfsLimit);
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);

        //+Patni/AKR/2010July06/Modified/NFD001-AutoMap
        //To check couch position
        if (map_mode == MAP_AUTO_MODE) {
            if (ABSF(target_couch_pos - mapObj->GetPqmPrMode()->GetCouchPos()) > rfsLimit) {
                return E_ERROR;
            }

        } else if (ABSF(Current_couchPosi - mapObj->GetPqmPrMode()->GetCouchPos()) > rfsLimit) {
            return E_ERROR ;
        }

    }

    //DB_FUNC_EXIT() ;
    return E_NO_ERROR ;
}
//****************************Method Header************************************

//Method Name   :SpeederMaxRatioCheck()

//Author        :PATNI /MRP

//Purpose       :To read the Speeder Database

//*****************************************************************************
status_t CPqmSpeeder::SpeederMaxRatioCheck(
    CPqmProtocol*  const prObj
)
{
    LPCTSTR FUNC_NAME = _T("CPqmSpeeder::SpeederMaxRatioCheck");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of SpeederMaxRatioCheck"));

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqm_ptr == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
#define SCALE  100

    /********************************************************
        ここでの処理はSaturnシステム以前のチェック機能であり
        Saturnシステムでは選択コイルによりインテリジェントな
        処理が必要になったので、別処理とする。
        よってここには入らない。
    ********************************************************/

    VfPathElem_t	path[5] = {0};
    VfFieldSpec_t	fieldSpec[4] = {0};
    spdb_rec_p	speedertbl = NULL ;
    int		n = 0 , numDone = 0 ;
    f32vec3_t		ratio ;
    float			Tratio = 0.0f ;//Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
    status_t	sts = E_NO_ERROR ;
    int			speeder_flag = 0 ;

    Boolean	enable = 0; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
    CPqmCoilData*   l_coil_data = CDataManager::GetInstance()->GetPQMCoilData();

    CString l_trace_string(_T(""));
    //DB_FUNC_ENTER("[PqmSpeederMaxRatioCheck]:");

    path[0].subtree_name = SVN_ST_PROTOCOL;
    path[0].index = prObj->GetProtocol() ;
    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(fieldSpec, 0, sizeof(VfFieldSpec_t) * 4);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    n = 0;
    VFF_SET_ARGS(fieldSpec[n], SVN_PLN_SPEEDER_RATIO_INPUT, VFT_FLTVEC3,
                 sizeof(ratio), &ratio, 0L);
    n++;
    VFF_SET_ARGS(fieldSpec[n], SVN_PLN_SPEEDER_FLAG, VFT_BITFIELD,
                 sizeof(speeder_flag), &speeder_flag, 0L);
    n++;

    CPQMStudy* l_study = m_pqm_ptr->GetStudy();
    CVarFieldHandle* vf_handle = l_study->GetVfDBHandle();

    if (!vf_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("vf_handle is NULL"));
        return E_ERROR;
    }

    status_t l_status = vf_handle->GetFields(path, SVD_ST_PROTOCOL, fieldSpec, n, (int*) & numDone);
    int Tmax = 0, PEmax = 0, SEmax = 0, PEexpand = 0, SEexpand = 0;	/* SpeederGetMaxRatioFromVF */ //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections

    if (sts != E_NO_ERROR || n != numDone) {
        sts = E_ERROR ;
        l_trace_string.Format(_T("%s:%d  VfGetFields ERROR num_Req=%d sts=%d numDone=%d\n"),
                              __FILE__, __LINE__, n , sts, numDone);

        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);

    } else {
        /* 対応するspeederDBチェックする */
        if (m_pqm_ptr->IsSaturnCoilSystem()) {
            /* libSpeeder関数を使用して取得する */
            CString l_study_name = l_study->GetStudyName();

            //+Patni-MJC/2009Aug17/Modified/cpp test corrections
            if (l_study_name.IsEmpty()) {
                PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("study_name pointer is NULL"));
                return E_ERROR;
            }

            //-Patni-MJC/2009Aug17/Modified/cpp test corrections

            ClibSpeederAdapter spdrAdapter(l_study_name, prObj->GetProtocol());
            spdrAdapter.SpeederGetMaxRatioFromVF(&Tmax, &PEmax, &SEmax, &PEexpand, &SEexpand);

            enable = TRUE;

        } else {

            /* 旧システム */
            CCoil dummy_coil;
            l_coil_data->PqmCoilGetCoil(prObj->GetPqmPrMode()->GetRecvCoilId(), dummy_coil);
            PqmCoil_t* coil_p = dummy_coil.GetCoil() ;
            speedertbl = SpeederGetCoil(s_speederTbl, m_num_speeder, prObj->GetPqmPrMode()->GetRecvCoilId());

            if (coil_p == NULL) {
                sts =  E_ERROR;
            }

            if (coil_p->speeder_enable & MAP_SPEEDER_SE_ENABLE) {
                enable = TRUE;

            } else {
                enable = FALSE;
            }

            if (speedertbl != NULL) {
                Tmax	= speedertbl->s_MaxRatio;
                PEmax	= speedertbl->s_PEMaxRatio;
                SEmax	= speedertbl->s_SEMaxRatio;

            } else {
                sts =  E_ERROR;
            }
        }

        if (sts  !=  E_ERROR) {
            l_trace_string.Format(_T("%s:%d   STUDY[%f,%f,%f]  DB[%d,%d,%d] \n"),
                                  __FILE__, __LINE__, ratio.x, ratio.y, ratio.z,
                                  Tmax, PEmax, SEmax);

            PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);

            if (((speeder_flag & VFC_SPEEDER_FLAG_BIT_PE) && (PEmax  < (int)ratio.x * SCALE)) ||
                /*   ( (speeder_flag & VFC_SPEEDER_FLAG_BIT_RO) && (speedertbl->s_ROMaxRatio  < (int)ratio.y*SCALE) ) ||  Dosen't check RO ration */
                ((enable) && (speeder_flag & VFC_SPEEDER_FLAG_BIT_SE) &&
                 (SEmax  < (int)ratio.z * SCALE))) {
                l_trace_string.Format(_T("%s:%d  MaxRatio  Error !!\n"), __FILE__, __LINE__);

                PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);

                sts =  E_ERROR ;
            }

            Tratio = 1 ;

            if (speeder_flag & VFC_SPEEDER_FLAG_BIT_PE)
                if ((int)ratio.x)		Tratio *= (ratio.x) ;

            /* if ( speeder_flag & VFC_SPEEDER_FLAG_BIT_RO )
            	if ( (int)ratio.y )		Tratio *= (ratio.y) ;	Dosen't check RO ration */
            if (speeder_flag & VFC_SPEEDER_FLAG_BIT_SE)
                if ((int)ratio.z)		Tratio *= (ratio.z) ;

            Tratio *= SCALE ;		/* Dec.12 2003 */
            l_trace_string.Format(_T("%s:%d  Total MaxRatio [ %d vs %f ]  !!\n"),
                                  __FILE__, __LINE__, Tmax, Tratio);

            PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);

            if (Tmax  < Tratio) {
                l_trace_string.Format(_T("%s:%d  Total MaxRatio [ %d vs %f ]  Error !!\n"),
                                      __FILE__, __LINE__, Tmax, Tratio);

                PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);

                sts =  E_ERROR ;
            }
        }
    }

    return sts ;
}
//****************************Method Header************************************

//Method Name   :SpeederGetCoil()

//Author        :PATNI /MRP

//Purpose       :To read the Speeder Database

//*****************************************************************************
spdb_rec_p CPqmSpeeder::SpeederGetCoil(
    spdb_rec_p* tbl ,
    const int n,
    const int coilid
)
{
    LPCTSTR FUNC_NAME = _T("CPqmSpeeder::SpeederGetCoil");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of CPqmSpeeder::SpeederGetCoil"));

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (tbl == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("tbl pointer is NULL"));
        return NULL;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    spdb_rec_p	rec_p = NULL ; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
    CString l_trace_string(_T(""));

    for (int i = 0; i < n; i++) {
        if (tbl[i] == NULL)	continue ;

        rec_p = tbl[i] ;
        l_trace_string.Format(_T("[%d]:coidId = 0x%x  vs 0x%x\n"), i, coilid, rec_p->speeder_id);

        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);

        //fflush(stdout);
        if (rec_p->speeder_id  == coilid) {
            //DB_FUNC_EXIT() ;
            return  rec_p;
        }
    }

    //DB_FUNC_EXIT() ;
    return NULL;
}

//****************************Method Header************************************
//Method Name   : SpeederGetrfsLimitInSaturn()
//Author        : PATNI /MRP
//Purpose       : To read the Speeder Database
//*****************************************************************************
int CPqmSpeeder::SpeederGetrfsLimitInSaturn(
    spdb_rec_p* tbl,
    const int num,
    PqmSaturnRcvCoil_t* coil_t
)
{
    LPCTSTR FUNC_NAME = _T("CPqmSpeeder::SpeederGetrfsLimitInSaturn");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("TRACE"));

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (coil_t == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                  _T("coil_t pointer is NULL"));
        return 0;
    }

    if (tbl == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                  _T("tbl pointer is NULL"));
        return 0;
    }

    //-Patni-MJC/2009Aug27/Modified/cpp test corrections

    int	flag = FALSE ;
    int rfsLimit = 9999 ;
    spdb_rec_p	rec_p = NULL;

    CString l_trace_string = _T("");
    l_trace_string.Format(_T("num: [%d], coil_t->numCoil: [%d]"), num, coil_t->numCoil);

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);

    for (int n = 0 ; n  < coil_t->numCoil ; n++) {
        for (int i = 0; i < num ; i++) {
            rec_p = tbl[i] ;

            if (NULL == rec_p) {
                PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("rec_p is NULL"));
                return 0;
            }

            if (NULL == coil_t->coiltbl_p[n]) {
                PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("coil_t->coiltbl_p[n] is NULL"));
                return 0;
            }

            if (rec_p->speeder_id == coil_t->coiltbl_p[n]->coil_id) {
                flag = TRUE ;

                if (rfsLimit > rec_p->rfsLimit)	rfsLimit = rec_p->rfsLimit ;

                break ;
            }
        }
    }

    l_trace_string.Format(_T("rfsLimit = %d"), rfsLimit);
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_trace_string);

    if (flag == TRUE) {
        return rfsLimit;

    } else {
        return 0 ;
    }
}

// Added By KT/ACE-2/15-06-2009/Multilingualisation

//+Patni-PJS/2010Mar11/Added/DeFT# MCM0229-00178
//**************************************************************************
//Method Name   :  SpeederOffSwitchCoil()
//Author        :  PATNI/PJS
//Purpose       :
//**************************************************************************
status_t CPqmSpeeder::SpeederOffSwitchCoil(CVarFieldHandle* const f_vf_handle, const int l_nprot)
{
    LPCTSTR FUNC_NAME = _T("CPqmSpeeder::SpeederOffSwitchCoil");

    if (!f_vf_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_vf_handle is NULL"));
        return E_ERROR;
    }

    VfPathElem_t		l_path[5] = {0};
    int32_t				l_ntotal_time = 0, l_num_coil = 0, l_switch_type = 0;
    int                 l_num = 0, l_numDone = 0;
    VfFieldSpec_t		l_fieldSpec[2] = {0};
    VftTimeCtrlParam_t	l_timeCtrl;

    memset(&l_timeCtrl , NULL , sizeof(l_timeCtrl)) ;

    l_path[0].subtree_name = SVN_ST_PROTOCOL;
    l_path[0].index = l_nprot;
    l_path[1].subtree_name = SVN_ST_SUBPROTOCOL;
    l_path[1].index = 0;

    l_num = 0;
    VFF_SET_ARGS(l_fieldSpec[l_num], SVN_PLN_SUBPROTOCOL_TIME, VFT_INT32,
                 sizeof(l_ntotal_time), &l_ntotal_time, 0L);
    l_num++;
    status_t l_status = f_vf_handle->GetFields(l_path, SVD_ST_SUBPROTOCOL, l_fieldSpec, l_num, (int*)&l_numDone);

    if (l_num != l_numDone) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("VfGetFields failed to get fields from varfield"));
        return	E_ERROR;
    }

    l_num = 0;
    VFF_SET_ARGS(l_fieldSpec[l_num], SVN_PLN_NUM_SWITCH_COIL, VFT_INT32,
                 sizeof(int32_t), &l_num_coil, NULL);
    l_num++;

    //Get SVD_ST_PROTOCOL field from varfield
    l_status = f_vf_handle->GetFields(l_path, SVD_ST_PROTOCOL, l_fieldSpec, l_num, (int*)&l_numDone);

    //If SVN_PLN_NUM_SWITCH_COIL is set in the varfiled change the scan time
    if (l_num == l_numDone) {
        l_ntotal_time /= l_num_coil;

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetFields failed"));
        return	E_ERROR;
    }

    //Set the changed scan time in the varfield.
    l_num = 0;
    l_switch_type = VFC_SWITCH_COIL_TYPE_NONE;
    VFF_SET_ARGS(l_fieldSpec[l_num], SVN_PLN_TOTAL_TIME, VFT_INT32,
                 sizeof(l_ntotal_time), &l_ntotal_time, VFO_REPLACE);
    l_num++;
    VFF_SET_ARGS(l_fieldSpec[l_num], SVN_PLN_SWITCH_COIL_TYPE, VFT_INT32,
                 sizeof(l_switch_type), &l_switch_type, VFO_REPLACE);
    l_num++;

    time_t l_ts = 0L;
    //Put SVD_ST_PROTOCOL fields from varfield
    l_status = f_vf_handle->PutFields(l_path, SVD_ST_PROTOCOL, l_fieldSpec,
                                      l_num, (int*)&l_numDone, &l_ts);
    if_error(l_status) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
        return E_ERROR;
    }

    CPQMStudy* l_study = m_pqm_ptr->GetStudy();

    if (NULL == l_study) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_study is NULL"));
        return	E_ERROR;
    }

    //get current scan protocol.
    CPqmProtocol* curObj = m_pqm_ptr->GetAcqManager()->GetScanProcedure()->GetScanProcProtocol();

    if (NULL == curObj) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("curObj is NULL"));
        return E_ERROR;
    }

    //Set protocol values
    l_study->GetProtocolItemValues(f_vf_handle, curObj);

    if (l_num != l_numDone) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields failed"));
        return	E_ERROR;
    }

    l_num = 0;
    VFF_SET_ARGS(l_fieldSpec[l_num], SVN_PLN_SUBPROTOCOL_TIME, VFT_INT32,
                 sizeof(l_ntotal_time), &l_ntotal_time, VFO_REPLACE);
    l_num++;

    //Put SVD_ST_PROTOCOL fields from varfield
    l_status = f_vf_handle->PutFields(l_path, SVD_ST_SUBPROTOCOL, l_fieldSpec,
                                      l_num, (int*)&l_numDone, &l_ts);
    if_error(l_status) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
        return E_ERROR;
    }

    if (l_num != l_numDone) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SpeederOffSwitchCoil failed"));
        return	E_ERROR;
    }

    //Send message to UI
    m_pqm_ptr->WriteToUI(PQM_MSG_CHANGE_SCAN_TIME);
    return	E_NO_ERROR;
}
//-Patni-PJS/2010Mar11/Added/DeFT# MCM0229-00178
//+Patni-NP/2010Jul27/Added/IR-161
//**************************************************************************
//Method Name   :  SpeederOnSwitchCoil()
//Author        :  PATNI/NP
//Purpose       :
//**************************************************************************
status_t CPqmSpeeder:: SpeederOnSwitchCoil(CVarFieldHandle* const vf_handle, const int prot)
{
    LPCTSTR FUNC_NAME = _T("CPqmSpeeder::SpeederOnSwitchCoil");

    if (!vf_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("vf_handle is NULL"));
        return E_ERROR;
    }

    VfPathElem_t		l_path[5]  = {0};
    int32_t				l_total_time = 0, l_num_coil = 0, l_switch_type = 0;
    int                 l_num = 0, l_numDone = 0;
    VfFieldSpec_t		l_fieldSpec[2] = {0};
    VftTimeCtrlParam_t	l_timeCtrl;

    memset(&l_timeCtrl , NULL , sizeof(l_timeCtrl)) ;

    l_path[0].subtree_name = SVN_ST_PROTOCOL;
    l_path[0].index = prot;
    l_path[1].subtree_name = SVN_ST_SUBPROTOCOL;
    l_path[1].index = 0;

    /* SVN_PLN_SUBPROTOCOL_TIMEを取得する */
    VFF_SET_ARGS(l_fieldSpec[l_num], SVN_PLN_SUBPROTOCOL_TIME, VFT_INT32,
                 sizeof(l_total_time), &l_total_time, 0L);
    l_num++;
    status_t l_status = vf_handle->GetFields(l_path, SVD_ST_SUBPROTOCOL, l_fieldSpec, l_num, (int*)&l_numDone);

    if (l_num != l_numDone) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("VfGetFields failed to get fields from varfield"));
        return	E_ERROR;
    }

    l_num = 0;
    /* SVN_PLN_NUM_SWITCH_COILを取得する */
    VFF_SET_ARGS(l_fieldSpec[l_num], SVN_PLN_NUM_SWITCH_COIL, VFT_INT32,
                 sizeof(int32_t), &l_num_coil, NULL);
    l_num++;


    l_status = vf_handle->GetFields(l_path, SVD_ST_PROTOCOL, l_fieldSpec, l_num, (int*)&l_numDone);

    if (l_num == l_numDone) {
        /* 撮像時間をInline用の撮像時間に再計算 */
        l_total_time *= l_num_coil;

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("GetFields failed"));
        return	E_ERROR;
    }

    l_num = 0;
    l_switch_type = VFC_SWITCH_COIL_TYPE_MAP;
    /* 再計算した時間をSVN_PLN_TOTAL_TIMEに格納 */
    VFF_SET_ARGS(l_fieldSpec[l_num], SVN_PLN_TOTAL_TIME, VFT_INT32,
                 sizeof(l_total_time), &l_total_time, VFO_REPLACE);
    l_num++;
    /* 時間と同期してSVN_PLN_SWITCH_COIL_TYPEもInlineに変更する */
    VFF_SET_ARGS(l_fieldSpec[l_num], SVN_PLN_SWITCH_COIL_TYPE, VFT_INT32,
                 sizeof(l_switch_type), &l_switch_type, VFO_REPLACE);
    l_num++;


    /************/
    time_t l_ts = 0;
    l_status = vf_handle->PutFields(l_path, SVD_ST_PROTOCOL, l_fieldSpec, l_num, (int*)&l_numDone, &l_ts);

    if (E_NO_ERROR != l_status || l_numDone != l_num) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
        return E_ERROR;
    }

    CPQMStudy* l_study = m_pqm_ptr->GetStudy();

    if (NULL == l_study) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_study is NULL"));
        return	E_ERROR;
    }


    CPqmProtocol* curObj = m_pqm_ptr->GetAcqManager()->GetScanProcedure()->GetScanProcProtocol();

    if (NULL == curObj) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("curObj is NULL"));
        return E_ERROR;
    }

    l_study->GetProtocolItemValues(vf_handle, curObj);

    if (l_num != l_numDone) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields failed"));
        return	E_ERROR;
    }

    l_num = 0;
    /* 再計算した時間をSVN_PLN_SUBPROTOCOL_TIMEにも格納 */
    VFF_SET_ARGS(l_fieldSpec[l_num], SVN_PLN_SUBPROTOCOL_TIME, VFT_INT32,
                 sizeof(l_total_time), &l_total_time, VFO_REPLACE);
    l_num++;

    l_status = vf_handle->PutFields(l_path, SVD_ST_SUBPROTOCOL, l_fieldSpec, l_num, (int*)&l_numDone, &l_ts);

    if (l_num != l_numDone) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields failed"));
        return	E_ERROR;
    }

    m_pqm_ptr->WriteToUI(PQM_MSG_CHANGE_SCAN_TIME); /*Need to Confirm*/
    return	E_NO_ERROR;
}





//**************************************************************************
//Method Name   :  pqmSpeederChangeSwitchCoil()
//Author        :  PATNI/NP
//Purpose       :
//**************************************************************************
status_t CPqmSpeeder:: pqmSpeederChangeSwitchCoil(CVarFieldHandle* const vf_handle, const int prot, const int receiveWB_enable)
{
    int32_t				l_switch_type = 0;

    /* 使用するコイルのreceiveWB_enableによってInline方式のコイル切替が
        PAC-PACとPAC-WBの２種類存在する */
    if (receiveWB_enable & RWB_MAP_PAC_PAC) {
        /* PAC-PAC切替 */
        l_switch_type = VFC_SWITCH_COIL_TYPE_MAP_PAC;

    } else {
        /* PAC-WB切替 */
        l_switch_type = VFC_SWITCH_COIL_TYPE_MAP;
    }

    return	CPqmUtilities::PqmPrPutSwitchCoilType(vf_handle, prot, &l_switch_type);
}
//-Patni-NP/2010Jul27/Added/IR-161

//+Patni-Sribanta/2011Mar04/Added/IR-184
//**************************************************************************
//Method Name   :  IsUniformityPossibleForB1()
//Author        :  PATNI/Sribanta
//Purpose       :
//**************************************************************************
bool CPqmSpeeder::CheckB1ConfigValue() const
{
    b1ICorrMode_t l_b1ICorrMode;
    memset(&l_b1ICorrMode, 0, sizeof(l_b1ICorrMode));
    status_t l_sts = b1ICorrModeDecision(NULL, &l_b1ICorrMode);
    return (E_NO_ERROR == l_sts) && (-1 == l_b1ICorrMode.iCorrMethod);
}

//**************************************************************************
//Method Name   :  CanSupportB1Anatomy()
//Author        :  PATNI/Sribanta
//Purpose       :
//**************************************************************************
bool CPqmSpeeder::CanSupportB1Anatomy(CPqmProtocol* const f_protocol_obj) const
{
    LPCTSTR FUNC_NAME = _T("CPqmSpeeder::CanSupportB1Anatomy");

    if (NULL == f_protocol_obj) {

        //+Patni-Sribanta/2011Mar24/Modified/IR-184
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_protocol_obj is NULL"));
        return false;
    }

    const int l_anatomy = f_protocol_obj->GetPqmPrMode()->GetAnatomy();

    const bool l_is_b1_supported_anatomy = (l_anatomy == VFC_ANATOMY_HEAD ||
                                            l_anatomy == VFC_ANATOMY_PELVIS ||
                                            l_anatomy == VFC_ANATOMY_ABDOMEN ||
                                            l_anatomy == VFC_ANATOMY_C_SPINE ||
                                            l_anatomy == VFC_ANATOMY_TL_SPINE
                                           );

    return l_is_b1_supported_anatomy;

}
//-Patni-Sribanta/2011Mar04/Added/IR-184

//****************************Method Header************************************
//Method Name   :SpeederTimeOut()
//Author        :PATNI /MRP
//Purpose       :To read the Speeder Database
//*****************************************************************************
void CPqmSpeeder::SpeederTimeOut(
    int sig //,int code
)
{
    LPCTSTR FUNC_NAME = _T("CPqmSpeeder::SpeederTimeOut");

    //Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(sig);

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of CPqmSpeeder::SpeederTimeOut"));

    if (m_self_ptr == NULL)
        return;

    m_self_ptr->SpeederTimerReset(true) ;		/* TIMER RESET */

    m_self_ptr->GetPqm()->GetAMB()->PqmPmAbortAmb();
    //	if(PqmPmIsActive(K_PM_TYPE_AMB))
    //	{/* AMBの場合はambEditにエラー通知する */
    //		PqmPmAbortAmb() ;
    //	}
}


CPqm* CPqmSpeeder::GetPqm()
{
    return m_pqm_ptr;
}
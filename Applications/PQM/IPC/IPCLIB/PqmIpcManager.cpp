//REDMINE-1213_Update_29_APR
//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-774
//Redmine-779
//Redmine-778
//Redmine-780
//Redmine-781
//Redmine-782
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PqmIpcManager.cpp
 *  Overview: Implementation of CPqmIpcManager class.
 *  Last Modified: 2007/8/10 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2007/6/15 12:00:00     PQM5 Baselined code
 *   2.0          Patni            2007/8/10 12:00:00     Implemented PQM6 requirements & Formatted
 *   3.0          Patni/LK         2007/8/10 12:00:00     Restructured after
                                                            TA comments
 *****************************************************************************/
//Patni-PJS/2011Mar2/Modified/TMSC_REDMINE-1480-Review Comments
#include <process.h>
#include "StdAfx.h"
#include <PQM/PQMIPCManager.h>
#include <objbase.h>
//Patni-Manish/2010Jan11/Added/Ph#3#IR-122/RemoteConsole
//MEITEC/2010Apr12/Modified/libRemote Change from libWNP to MRMPlus
#include <libremote/libremote.h>
//Patni-Rajendra/2011Mar29/Modified/TMSC-REDMINE-773
#include <libStudyManager/Mutex/ScopedLock.h>
#include <libStudyManager/Log/ScopedFuncInOutLog.h>
#include <PErrorHandler/ExceptionHandler.h>
#include <PQM/PQMLogMgr.h>
#include <PQM/PqmUtilities.h> // Added By KT/ACE-2/15-06-2009/Multilingualisation
#include "Pqm.h"
#include "PQMConfig.h"
#include "pqmpmacqman.h"
#include "scanprocedure.h"
#include "ColorSetting.h"
#include "PQMStudyMgr.h"
#include "IRSListner.h"
#include "voicemanager.h"
#include "PqmMRSScan.h"
#include "SelectionCoilData.h"
#include "SharedMemory.h"
#include "PQMSocket.h"
#include "PlanDuplicateSettings.h"
#include "PqmProbe.h"
#include "CoilSelectionTool.h"
//+Patni-HEMANT/ADDED On 6/19/2009 5:01:30 PM/ ACE-2 / WFDA
//+Patni-PJS/2010Feb2/Modified/IR-97
//#include "WFDAMsgReceiver.h"
//#include "WFDAImplementer.h"
//-Patni-PJS/2010Feb2/Modified/IR-97
//-Patni-HEMANT/ADDED On 6/19/2009 5:01:30 PM/ ACE-2 / WFDA
#include "VarFieldHandle.h" //TMSC-Tanoue/2010Aug25/MVC008345
#include "PASCOMInterface.h"
#include "CoilChannelMode.h"
#include "pqmAMB.h"
#include "PqmPmSARManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

Data_decl_list  g_sequence_param_name_list;
SequenceList m_dbsa_seq_list;

HANDLE CPQMIPCManager::hThread       = NULL;
DWORD  CPQMIPCManager::dThreadId     = 0;
HANDLE CPQMIPCManager::m_ThreadEvent = NULL;

//Patni-Rajendra/2011Mar29/Modified/TMSC-REDMINE-1559
using namespace MR::ACQ::SM::COMMON::MUTEX;

//****************************Method Header************************************
//Method Name   :CPQMIPCManager()
//Author        :PATNI / HAR
//Purpose       :Default Constructor
//*****************************************************************************
CPQMIPCManager::CPQMIPCManager():
    m_pqmptr(NULL),
    m_prescan_done(FALSE),
    m_pComObj(NULL),
    m_ptrCallback(NULL),
    //+Patni-PJS/2011Mar2/Modified/TMSC_REDMINE-1480-Review Comments
    m_mrs_thread_id(0),
    m_mrs_thread_handle(NULL),
    //-Patni-PJS/2011Mar2/Modified/TMSC_REDMINE-1480-Review Comments
    m_is_acqman_connected(false),
    m_sharedmem(NULL),
    m_selected_list(NULL),
    m_total_count(0),
    m_no_of_sequence_recvd_frm_memory(1),
    m_is_sarmanager_connected(FALSE)

{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::CPQMIPCManager");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace"));
}

//****************************Method Header************************************
//Method Name   :CPQMIPCManager()
//Author        :PATNI / HAR
//Purpose       :Parameterised Constructor
//*****************************************************************************
//+ MEITEC/2010Apr23/Added/IR86
CPQMIPCManager::CPQMIPCManager(
    bool (*callback)(pqm_msg_tag, void*), bool (*callbackdisplay)(BSTR, void*),
    CPqmIpcCom* pObj):
    m_pqmptr(NULL),
    m_prescan_done(FALSE),
    m_pComObj(pObj),
    m_ptrCallback(callback),
    m_ptrdisplayCallback(callbackdisplay),
    m_is_acqman_connected(false),
    m_sharedmem(NULL),
    m_no_of_sequence_recvd_frm_memory(1),
    m_is_sarmanager_connected(FALSE)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::CPQMIPCManager");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    SetIsAcqmanConnected(false);

}
//- MEITEC/2010Apr23/Added/IR86

//****************************Method Header************************************
//Method Name   :CPQMIPCManager()
//Author        :PATNI / HAR
//Purpose       :Destructor
//*****************************************************************************
CPQMIPCManager::~CPQMIPCManager(
)
{
    //+ MEITEC/2010Jun23/Modified/MVC008324_1
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::~CPQMIPCManager");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace"));
    //- MEITEC/2010Jun23/Modified/MVC008324_1

    if (m_pqmptr) {
        DEL_PTR(m_pqmptr);
    }


    //-Patni-RUP/2009Aug20/Modified/cpp test corrections
    if (m_pi.hProcess != NULL) {
        TerminateProcess(m_pi.hProcess, 0);

    } else {
    }

    //+Patni-HEMANT/ADDED On 6/19/2009 4:59:28 PM/ ACE-2 / WFDA
    //Creating the WFDA msg receiver...
    //Patni-PJS/2010Feb2/Modified/IR-97
    //CWFDAMsgReceiver::DeleteWFDAMsgReceiver();
    //-Patni-HEMANT/ADDED On 6/19/2009 4:59:28 PM/ ACE-2 / WFDA
    //+Patni-PJS/2011Feb24/Added/TMSC_REDMINE-1480
    //+Patni-PJS/2011Mar2/Modified/TMSC_REDMINE-1480-Review Comments
    if (m_mrs_thread_handle) {
        CloseHandle(m_mrs_thread_handle);
        m_mrs_thread_handle = NULL;
    }

    //-Patni-PJS/2011Mar2/Modified/TMSC_REDMINE-1480-Review Comments
    //-Patni-PJS/2011Feb24/Added/TMSC_REDMINE-1480
}

//+Patni-PJS/2011Jan25/Added/IR-97
//****************************Method Header************************************

//Method Name   :SendIndex()

//Author        :PATNI / HAR

//Purpose       :

//*****************************************************************************

void CPQMIPCManager::SendIndex(
    int f_index
)
{
    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqmptr == NULL) {
        CPQMLogMgr::GetInstance()->WriteTraceLog(DOMAIN_NAME,
                USER_FUNC_MARKER, _T("CPQMIPCManager::SendIndex"),
                _T("m_pqm_ptr pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    CPqmProtocol* l_current_protocol = NULL;
    m_pqmptr->GetStudy()->DeleteProt(f_index);
}
//-Patni-PJS/2011Jan25/Added/IR-97

//****************************Method Header************************************
//Method Name   :StartPQM()
//Author        :PATNI / HAR
//Purpose       :This method loads the coil information, study and series into
//               memory
//*****************************************************************************
//Patni-ARD/2010Mar29/Modified/Ph-3# IR-100
BOOL CPQMIPCManager::StartPQM(
    const int f_mode, VARIANT* f_config_data
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::StartPQM");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,  _T("Trace of StartPQM"));

    //+Patni-RUP/2009Sep17/Modified/Fixed Internal defect
    HINSTANCE curr_instance = NULL;
#ifdef _DEBUG
    curr_instance = ::GetModuleHandle(_T("PQMd.exe"));
#else
    curr_instance = ::GetModuleHandle(_T("PQM.exe"));
#endif
    AfxSetResourceHandle(curr_instance);
    //-Patni-RUP/2009Sep17/Modified/Fixed Internal defect

    m_pqmptr = new CPqm(f_mode);
    //Patni-HAR/2010Feb18/Added/MSA0251-00176
    CPqmUtilities::SetPqmPtr(m_pqmptr);
    //+Patni-PJS/2010May09/Added/JaFT# IR-141
    CString l_mplus_usr  = CPqmUtilities::GetMPlusUsrName();
    m_pqmptr->SetUsername(l_mplus_usr);
    //-Patni-PJS/2010May09/Added/JaFT# IR-141

    CPQMIPCManager::hThread = CreateThread(NULL, 0, CPQMIPCManager::ThreadProc,
                                           (void*)this, 0, &CPQMIPCManager::dThreadId);

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return FALSE;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    //+Patni-PJS/2009May10/Modify/IR-82 review comment
    if (!m_pqmptr->InitializePqm()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,  _T("InitializePqm() Failed."));
        return FALSE;
    }

    //-Patni-PJS/2009May10/Modify/IR-82 review comment
    if (!m_pqmptr->IsAcqmanValid()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("IsAcqManValid() returned FALSE"));
        return FALSE;
    }

    m_pqmptr->SetManagerPtr(this);

    //+Patni-ARD/2010Mar29/Modified/Ph-3# IR-100
    if (NULL != f_config_data) {
        SetIPCConfigInfo(f_config_data, f_mode);
    }

    //-Patni-ARD/2010Mar29/Modified/Ph-3# IR-100

    // Initialize VoiceMgr application

    //int l_av_dynamic_delay = m_pqmptr->GetAvDynamicDelay();
    //int l_av_tp1_delay = m_pqmptr->GetAvTp1Delay();

    //get the volume from resource
    if (f_mode == K_IMAGING) {
        //Patni-Raj/2010Jan14/Added/PH#3#/Debug Window
        //+Patni-PJS/2010Feb17/Added/DeFT# MVC006756
        //+Patni-Hemant/2010May18/Added/JaFT/TMSC-REDMINE-23
        //CVoiceManager::GetInstance()->InitializeVoiceMgr(f_mode, 80,
        //    l_av_dynamic_delay, l_av_tp1_delay, m_pqmptr);
        //-Patni-Hemant/2010May18/Added/JaFT/TMSC-REDMINE-23
        // Initialize IRS Listner
        if (FALSE == m_pqmptr->GetIRSListner()->InitializeIRS()) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("InitializeIRS failed."));
            return FALSE;
        }
    }

    // Initialize PQM IPC application.

    // Checks the Study section present in "IPCConfig.INI" file
    //+Patni-AMT/2010Mar10/Modified/Ph-3#IR-100

    /*
    l_study_uid = CPQMConfig::GetInstance()->GetStudyUID();

    if (l_study_uid.GetLength() <= 0) {
        CPQMLogMgr::GetInstance()->WriteTraceLog(DOMAIN_NAME,
                USER_FUNC_MARKER,
                _T("CPQMIPCManager::StartPQM"),
                _T("No Study UID got from the ini file"));

        CString l_err_string = L"";
        l_err_string .LoadString(IDS_STUDY_NOT_FOUND);

        CPQMLogMgr::GetInstance()->WriteEventLog(l_err_string ,
                ERROR_LOG_CRUCIAL1,
                _T("PQMIPCManager.cpp"));

        CString temp_string = CPqmUtilities::GetMultiLingualString(_T("IDS_STUDY_NOT_FOUND"));    // Added By KT/ACE-2/15-06-2009/Multilingualisation
        CPQMLogMgr::GetInstance()->DisplayError(
            //l_err_string,					// Added By KT/ACE-2/15-06-2009/Multilingualisation
            temp_string,					// Added By KT/ACE-2/15-06-2009/Multilingualisation
            _T("PQMIPCManager.cpp"),
            __LINE__, ERROR_LOG_CRUCIAL1 ,
            ERR_ID, APP_NAME);

        return FALSE;
    }
    */
    // Initialize PQM with Study UID
    //+ MEITEC/2010May24/IR-82
    /*if (!m_pqmptr->InitializePqm()) {
        return FALSE;
    }*/
    //- MEITEC/2010May24/IR-82

    //Patni-ARD/2010Mar31/Added/DeFT# IR-100
    SetStudyMgrSeriesBias(f_config_data);

    //-Patni-AMT/2010Mar10/Modified/Ph-3#IR-100

    if (!m_pqmptr->ConnectProCon()) {

        //Patni-ARD/2009-Nov-25/Added/DeFT# MVC003149 + MVC005179
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Connection to proCon server failed !"));

        SendMessageToGUI(PQM_MSG_PROCON_NOT_CONNECTED);

    } else {

        SendMessageToGUI(PQM_MSG_PROCON_CONNECTED);
    }

    if (m_pqmptr->GetMode() == K_IMAGING) {

        m_is_acqman_connected = m_pqmptr->GetAcqManager()->ConnectToAcqman();

        if (!m_is_acqman_connected) {

            CString string = L"";
            string.LoadString(IDS_ERR_CONNECT_ACQMAN);

            //+Patni-ARD/2009-Nov-25/Commented/DeFT# MVC003149 + MVC005179
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, string);
            //+Patni-HAR/2010Apr13/Added/Phase#3 CDS Requirement

        } else {
            m_pqmptr->GetAcqManager()->GetScanProcedure()->SendAcqInfoForCouch();
        }

        //-Patni-HAR/2010Apr13/Added/Phase#3 CDS Requirement
    }


    if (m_pqmptr->GetMode() == K_IMAGING) {

        m_is_sarmanager_connected = m_pqmptr->GetSARManager()->ConnectToSARManager();

        CString l_message(_T(""));
        l_message.Format(_T("SARManager connection %s."), m_is_sarmanager_connected ? _T("successfull") : _T("failed"));
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message);

    }

    //+Patni-ARD/2010Mar10/Commented/IR-100 code review
    //if (CPQMConfig::GetInstance()->GetMachineType() == _T("ONEPATH")) {
    gdcMode_t	l_gdc_mode;
    harmonizationMode_t l_harmonization_mode;

    memset(&l_gdc_mode, 0, sizeof(gdcMode_t));
    memset(&l_harmonization_mode, 0, sizeof(harmonizationMode_t));

    char*		l_system_type = getenv("GP_SYSTEM_TYPE");

    int			l_status = -1;

    //+Patni-RUP/2009Sep22/Modified/Added Try Catch
    try {
        l_status = gdcModeDecision(l_system_type, &l_gdc_mode) ;

    } catch (...) {
        CString	l_print_string = _T("");
        l_print_string.Format(_T("gdcModeDecision() crashed System Type : =%s "),
                              l_system_type);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_print_string);
    }

    //-Patni-RUP/2009Sep22/Modified/Added Try Catch
    if (l_system_type == NULL || l_status != E_NO_ERROR) {

        AfxMessageBox(_T("ERR_GDC_FILE_OPEN_ERROR"), MB_SYSTEMMODAL);
        //			PqmUiSafeExit(PQM_FATAL);
    }

    m_pqmptr->SetGDCMode(l_gdc_mode);//Patni-PJS/2009Sep16/Modified/internal Code review comment

    //+Patni-RUP/2009Sep22/Modified/Added Try Catch
    try {
        l_status = harmonizationModeDecision(l_system_type , &l_harmonization_mode);

    } catch (...) {
        CString	l_print_string = _T("");
        l_print_string.Format(_T("harmonizationModeDecision() crashed systemtype=%s"),
                              l_system_type);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_print_string);
    }

    //-Patni-RUP/2009Sep22/Modified/Added Try Catch

    if (l_system_type == NULL || l_status != E_NO_ERROR) {

        //			AfxMessageBox(_T("ERR_HARMONIZATION_FILE_OPEN_ERROR"), MB_SYSTEMMODAL);
        //			PqmUiErrorDialog(PqmUiFetchLiteral("ERR_HARMONIZATION_FILE_OPEN_ERROR"),ERR_ERROR);
        //			PqmUiSafeExit(PQM_FATAL);
    }

    //Patni-PJS/2009Sep16/Modified/Internal Code review comment
    m_pqmptr->SetHarmonizationMode(l_harmonization_mode);
    m_pqmptr->SetSystemType(_strdup(l_system_type));
    //}
    //-Patni-ARD/2010Mar10/Commented/IR-100 code review

    //+Patni-HEMANT/ADDED On 6/19/2009 4:59:28 PM/ ACE-2 / WFDA
    //Creating the WFDA msg receiver...

    //+Patni-PJS/2010Feb2/Modified/IR-97
    /*
    CWFDAImplementer* l_wfdaimplementer = m_pqmptr->GetWFDAImplementer();

    if (l_wfdaimplementer) {

        CWFDAMsgReceiver::CreateWFDAMsgReceiver(l_wfdaimplementer->GetWFDAHandlerInterface());

    } else {

        ASSERT(FALSE);
    }
    */
    //-Patni-PJS/2010Feb2/Modified/IR-97

    //-Patni-HEMANT/ADDED On 6/19/2009 4:59:28 PM/ ACE-2 / WFDA
    //Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
    CPQMConfig::GetInstance()->LoadSFTConfigMenu();
    return TRUE;
}

//***************************Method Header*************************************/
//Method Name    : StartScan()
//Author         : PATNI/MRP
//Purpose        : This function will be called from COM DLL when Scan start
//                  is Clicked
//*****************************************************************************/
//+Patni-ARD/2009Sept01/Modified/IR GUI IPC check SEQ
void CPQMIPCManager::StartScan(
    VARIANT* f_pData,
    const int f_selected_series
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::StartScan");
    PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return ;
    }

    if (m_pqmptr->GetMode() == K_IMAGING) {
        if (!m_is_acqman_connected) {
            if (!(m_is_acqman_connected = m_pqmptr->GetAcqManager()->ConnectToAcqman())) {

                CString string = L"";
                string.LoadString(IDS_ERR_CONNECT_ACQMAN);

                //+Patni-ARD/2009-Nov-25/Commented/DeFT# MVC003149 + MVC005179
                /*
                CPQMLogMgr::GetInstance()->WriteEventLog(string,
                        ERROR_LOG_CRUCIAL1,
                        _T("PQMIPCManager.cpp"));
                */

                //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
                //CPQMLogMgr::GetInstance()->DisplayScanError(
                //    IDS_ERR_CONNECT_ACQMAN, _T("IDS_ERR_CONNECT_ACQMAN"), SCAN_ERR_NOTIFY,
                //    L"PQMIPCManager.cpp");
                //-Patni-ARD/2009-Nov-25/Commented/DeFT# MVC003149 + MVC005179

                m_pqmptr->WriteToUI(PQM_MSG_ABORT_WAIT);//+I8_ITC60_saurabh_gupta

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, string);
                return;
            }
        }
    }

    //Patni-ARD/2009Sept01/Modified/IR GUI IPC check SEQ
    if (m_pqmptr->ScanSeries(f_pData, f_selected_series)) {

        CString str_msg(_T(""));
        str_msg.Format(_T("Series ID: %d"), f_selected_series);
        PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME, str_msg);


        CString l_currentseries_index(_T(""));
        l_currentseries_index.Format(_T("%d"), f_selected_series);

    } else {
        PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME, _T("Unable to Scan Series"));
    }
}

/***************************Method Header*************************************/
//Method Name    : AbortScan()
//Author         : PATNI/ASG
//Purpose        : This is called from COMDLL when abort button pressed
//*****************************************************************************/
void CPQMIPCManager::AbortScan(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::AbortScan");
    PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME, _T("Trace"));

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return ;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    CPqmPmAcqman* pqm_pm_acqman = m_pqmptr->GetAcqManager();
    CScanProcedure* scan_proc = pqm_pm_acqman->GetScanProcedure();

    pqm_pm_acqman->AcqmanAbortFromnMainThread();
}



/***************************Method Header*************************************/
//Method Name    : PauseScan()
//Author         : PATNI/ASG
//Purpose        : This is called from COMDLL when Pause button pressed
//*****************************************************************************/
void CPQMIPCManager::PauseScan(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::PauseScan");
    PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME, _T("Trace"));

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return ;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    CPqmPmAcqman* pqm_pm_acqman = m_pqmptr->GetAcqManager();
    CScanProcedure* scan_proc = pqm_pm_acqman->GetScanProcedure();

    //Patni-MP/ADDED/15Mar2009/PSP-Defect Fix #100
    SendMessageToGUI(PQM_MSG_SCAN_PAUSE_FALSE);

    scan_proc->AcquisitionScanPause();
    //Patni-MP/Commented/15Mar2009/PSP-Defect Fix #100
    //SendMessageToGUI(PQM_MSG_PAUSE_DONE );
}

/***************************Method Header*************************************/
//Method Name    : ResumeScan()
//Author         : PATNI/ASG
//Purpose        : This is called from COMDLL when Resume button pressed
//*****************************************************************************/
void CPQMIPCManager::ResumeScan(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::ResumeScan");
    PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME, _T("Trace"));

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return ;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    CPqmPmAcqman* pqm_pm_acqman = m_pqmptr->GetAcqManager();
    CScanProcedure* scan_proc = pqm_pm_acqman->GetScanProcedure();

    //Patni-MP/ADDED/15Mar2009/PSP-Defect Fix #100
    SendMessageToGUI(PQM_MSG_SCAN_PAUSE_FALSE);

    scan_proc->AcquisitionScanResume();
    //Patni-MP/Commented/15Mar2009/PSP-Defect Fix #100
    //SendMessageToGUI(PQM_MSG_RESUME_DONE );
}


/***************************Method Header*************************************/
//Method Name    : MRSScanAbort()
//Author         : PATNI/MAHESH
//Purpose        :
//*****************************************************************************/
void CPQMIPCManager::MrsScanClose()
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::MrsScanClose");
    PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return ;
    }

    CPqmPmAcqman* pqm_pm_acqman = m_pqmptr->GetAcqManager();

    pqm_pm_acqman->AcqmanAbort();
}



//+Patni-PJS/2010Oct26/Added/WFDA/IR-97
//************************************Method Header************************************
// Method Name  : CopyWFDAProtocolTags
// Author       : PATNI/ PJS
// Purpose      :
//***********************************************************************************
/*HRESULT CPQMIPCManager::CopyWFDAProtocolTags(
    int f_index_being_dup, int f_index_for_dup
)
{
    CPQMStudy* l_pqm_study = m_pqmptr->GetStudy();
    if(NULL == l_pqm_study) {
        return S_FALSE;
    }

    CPqmProtocol* f_from_obj = l_pqm_study->GetItemFromPosition(f_index_being_dup + 1);
    CPqmProtocol* f_to_obj = l_pqm_study->GetItemFromPosition(f_index_for_dup + 1);
    if(NULL == f_from_obj || NULL == f_to_obj) {
        return S_FALSE;
    }

    if(l_pqm_study->IsProtocolInScan(f_from_obj->GetProtocol())) {
        CPQMStudyMgr* l_pqm_studymgr = m_pqmptr->GetStudyManagerPtr();
        if(NULL == l_pqm_studymgr) {
            l_pqm_studymgr->CopyWFDAProtocolTags(f_from_obj, f_to_obj);
        }
        return S_OK;
    }
    return S_FALSE;
}*/
//-Patni-PJS/2010Oct26/Added/WFDA/IR-97

/***************************Method Header*************************************/
//Method Name    : LoadStudyLOID()
//Author         : PATNI/JS
//Purpose        : Set the Study LOID , which contains study loid information from GUI.
//*****************************************************************************/
//Patni-HAR/2010Apr21/Modified/MVC007534
int CPQMIPCManager::LoadStudyLOID(
    BSTR f_patientloid,
    BSTR f_studyloid,
    BSTR f_pas_path		//PATNI-Sribanta/2010Nov09/Added/IR-170
)
{
    //Patni-AMT/2010Apr16/Modified/JaFT# COMMON_CDR_07
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::LoadStudyLOID");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,  _T("Trace of LoadStudyLOID"));

    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return R_FALSE;
    }

    //CString l_str(_T(""));
    //Patni-NP/2009Sep30/Modified/ MVC005074
    //l_str.Format(_T("%s"), f_studyloid);

    //+Patni-RUP/2009Aug24/Modified/cpp test corrections
    //m_pqmptr->GetStudyManagerPtr()->SetLoid(l_str.GetBuffer(l_str.GetLength()));

    CPQMStudyMgr* l_pqm_studymgr = m_pqmptr->GetStudyManagerPtr();

    if (NULL == l_pqm_studymgr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,  _T("l_pqm_studymgr is NULL"));
        return R_FALSE;
    }

    //+ Patni-Tanoue san/2009Nov05/Added/Thread Synchronization

    // ThreadSafe for Study information with IRS msg.
    // To avoid update study info between IRS Study Check and Get Value.
    CScopedLock l_scoped_lock(m_pqmptr->GetStudy()->GetStudyLock(), CPQMLogMgr::GetInstance());
    //- Patni-Tanoue san/2009Nov05/Added/Thread Synchronization

    l_pqm_studymgr->SetLoid(f_studyloid);//Patni-Sribanta-MP/2011Jan18/Modified/Study Reload Issue

    //-Patni-RUP/2009Aug24/Modified/cpp test corrections

    // added by sarjeet
    //+Patni-AMT/2010Mar10/Modified/Ph-3#IR-100
    m_pqmptr->GetStudy()->SetStudyLoid(f_studyloid);
    CString l_study_uid = m_pqmptr->GetStudyManagerPtr()->GetStudyNameatStudyLevel();

    if (l_study_uid.Find(_T("Run")) != -1) {

        CString l_log_msg = CString(_T("Using Existing Study File")) + l_study_uid;
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_msg);

        //+Patni-HAR/2010Apr21/Added/MVC007534
        if (E_NO_ERROR != CheckFileExist(l_study_uid)) {
            return R_FILE_NOT_FOUND;
        }

        //-Patni-HAR/2010Apr21/Addded/MVC007534

        m_pqmptr->SetStudyUid(l_study_uid);
        m_pqmptr->SetStudyUidPath(l_study_uid);

        //Defect # 138 Sarjeet
        //+Patni-AMT/2009Mar28/Modified/PSP1#307 Code Review

        //Patni-PJS/2009Sep16/Modified/TMSC Code review comment
        //-Patni-AMT/2009Mar28/Modified/PSP1#307 Code Review
        l_study_uid.MakeLower();
        l_study_uid.Replace(_T("run"), _T("Run"));

        m_pqmptr->GetStudy()->SetStudyPath(l_study_uid);

        //+Patni-HEMANT/ADDED On 8/20/2009 10:06:52 PM/ MVC002838
        CString l_str = l_study_uid ;
        l_str = l_str.Mid(l_str.Find(_T("Run")));

        if (!m_pqmptr->GetStudy()->CreateOrUpdateVFStudyFile(f_patientloid, f_studyloid, l_str)) {
            return R_FALSE;
        }

        //-Patni-HEMANT/ADDED On 8/20/2009 10:06:52 PM/ MVC002838

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Creating new Study"));

        if (!m_pqmptr->GetStudy()->CreateStudyFile(f_patientloid, f_studyloid)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CreateStudyFile() failed."));
            return R_FALSE;
        }

        m_pqmptr->GetStudyManagerPtr()->WriteStudyNameatStudyLevel(m_pqmptr->GetStudyUid());
    }

    CString l_studyUID = m_pqmptr->GetStudy()->GetStudyPath();
    m_sharedmem = new CSharedMemory(l_studyUID);

    if (m_sharedmem->IsStudyIDSame()) {
        //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
        CPQMLogMgr::GetInstance()->DisplayScanError(IDS_STUDY_OPEN, _T("IDS_STUDY_OPEN"),
                SCAN_ERR_NOTIFY, _T("CPQMIPCManager::LoadStudyLOID"));

        SendMessageToGUI(PQM_MSG_SAME_UID);
        m_sharedmem->ReleaseResources(l_studyUID.GetBuffer(l_studyUID.GetLength()));

        //Patni-MN/2009Mar03/Modified/PSP1#138/Added the return value
        return R_SAME_FILE_EXIST;	//Patni-HAR/2010Apr21/Modified/MVC007534
    }

    //-Patni-AMT/2010Mar10/Modified/Ph-3#IR-100

    //Patni-Sribanta/2010Nov10/Modified/IR-170
    if (!m_pqmptr->LoadStudyLOID(f_patientloid, f_studyloid, f_pas_path)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("LoadStudyLOID() failed."));
        return R_FALSE;
    }

    return R_TRUE;
}

/***************************Method Header*************************************/
//Method Name    : DeleteAll()
//Author         : PATNI/PJP
//Purpose        : Set the Series LOID, which contains series loid information from GUI.
//*****************************************************************************/
BOOL CPQMIPCManager::DeleteAll(
    BSTR bStr
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::DeleteAll");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return FALSE;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    //+Patni-NP/2009Mar09/Added/PSP1 Changes For VF Lock
    //m_pqmptr->WriteToUI(PQM_MSG_BEGINWAIT_CURSOR);

    //    PostThreadMessage(dThreadId,MSG_DELETE,WPARAM(bStr),NULL);
    //   WaitForSingleObject(m_ThreadEvent,INFINITE);

    return m_pqmptr->DeleteAll(bStr);

    // m_pqmptr->WriteToUI(PQM_MSG_ENDWAIT_CURSOR);
    //+Patni-NP/2009Mar09/Added/PSP1 Changes For VF Lock
}

//****************************Method Header************************************
//Method Name    : ConnectVoiceManager()
//Author         : PATNI/HAR - MSN
//Purpose        :
//*****************************************************************************
void CPQMIPCManager::ConnectVoiceManager(
    const int voice_param
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::ConnectVoiceManager");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of ConnectVoiceManager"));

    // Play the voice cmd
    CVoiceManager::GetInstance()->CmdPlay(voice_param, TRUE);
}

//****************************Method Header************************************
//Method Name    : UpdateComment()
//Author         : PATNI/
//Purpose        :
//*****************************************************************************
void CPQMIPCManager::UpdateComment(
    BSTR bStr,
    BSTR scancomment,
    BSTR scanid
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::UpdateComment");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMIPCManager::UpdateComment"));

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    CString str_scancomment = scancomment;
    CString str_scanid = scanid;

    str_scancomment += ",";
    str_scancomment = str_scancomment + scanid;

    //+Patni-PJS/2009Sep10/Modified/TMSC CPP test review comment resolved
    //BSTR bstr_scancomment=NULL;
    //bstr_scancomment = _bstr_t(str_scancomment);
    //-Patni-PJS/2009Sep10/Modified/TMSC CPP test review comment resolved

    //scancomment
    //+Patni-NP/2009Mar09/Added/PSP1 Changes For VF Lock
    //m_pqmptr->WriteToUI(PQM_MSG_BEGINWAIT_CURSOR);
    //PostThreadMessage(dThreadId,MSG_UPDATE_COMMENT,WPARAM(bStr),LPARAM(bstr_scancomment));
    //WaitForSingleObject(m_ThreadEvent,INFINITE);

    m_pqmptr->UpdateComment(bStr, scancomment);

    //m_pqmptr->WriteToUI(PQM_MSG_ENDWAIT_CURSOR);
    //::SysFreeString(bstr_scancomment);    //Patni-PJS/2009Sep10/Modified/TMSC CPP test review comment resolved
}

//****************************Method Header************************************
//Method Name    : OnPQMCloseButton()
//Author         : PATNI/MSN
//Purpose        : To Disconnect voice manager
//*****************************************************************************
void CPQMIPCManager::OnPQMCloseButton(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::OnPQMCloseButton");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    //+Patni-RUP/2010Jan08/Close Porbe defect fix
    CPqmProcon* l_pqm_procon = m_pqmptr->GetPqmProCon();

    if (NULL != l_pqm_procon) {
        l_pqm_procon->CloseProbe();
    }

    //-Patni-RUP/2010Jan08/Close Porbe defect fix

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    //+ Patni-PJS/2009Jun05/Added/JFT# 251,251,251,IR(5)
    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
             _T("Trace of OnPQMCloseButton"));
    //+ Patni-PJS/2009Jun05/Added/JFT# 251,251,251,IR(5)
    m_pqmptr->SetCurrentSarInfo(_T(""));
    //Patni-DKH/2010Sep13/Added/TMSC-REDMINE-666
    m_pqmptr->GetStudy()->GetAllProtAcqStatus();


    m_pqmptr->GetStudy()->CloseStudy();

    m_pqmptr->GetSARManager()->SendSessionEndMsgToSARMgr();

    if (m_pqmptr->GetCoilSelectionTool()) {
        m_pqmptr->GetCoilSelectionTool()->ResetAcqInfoFlags();
    }

    if (m_sharedmem) {
        CString l_studyUID = m_pqmptr->GetStudy()->GetStudyPath();
        m_sharedmem->ReleaseResources(l_studyUID.GetBuffer(l_studyUID.GetLength()));
        DEL_PTR(m_sharedmem);
    }
}

//****************************Method Header************************************
//Method Name    : SetSerializeDuplicate()
//Author         : PATNI/PJP
//Purpose        : Set the SAFEARRAY pointer, which contains serialize data from GUI for Duplicate
//*****************************************************************************
BOOL CPQMIPCManager::DuplicateSequence(
    VARIANT* pData,
    BSTR bStr,
    const int f_index_duplicating_prot
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::DuplicateSequence");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL == m_ThreadEvent) {

        CPQMLogMgr::GetInstance()->WriteEventLog(
            _T("Unable to create Thread event"), ERROR_LOG_CRUCIAL1 , _T("CPQMIPCManager.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_THREAD_EVENT_ERROR")),
            _T("CPQMIPCManager.cpp"),
            __LINE__, ERROR_LOG_CRUCIAL1 ,
            ERR_ID, APP_NAME);
    }

    CPASCOMSequence* pSeq = (CPASCOMSequence*)pData->byref;
    m_pqmptr->SetWaitStatus(pSeq);

    return m_pqmptr->GetDbsaStudyMgr()->DuplicateProtocol(pSeq, _wtoi(bStr), f_index_duplicating_prot);

    //+Patni-NP/2009Mar09/Added/PSP1 Changes For VF Lock
}

//*******************************Method Header*********************************
//Method Name    :StartRFLPrescan()
//Author         :PATNI/MSN
//Purpose        :Starts the RFL Prescan done using APC Retry Menu
//*****************************************************************************
bool CPQMIPCManager::StartRFLPrescan(
    const int scanid
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::StartRFLPrescan");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return false;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    CScanProcedure* scanprocedure = m_pqmptr->GetAcqManager()->GetScanProcedure();
    //+Patni-Hemant/2011Jan11/Modified/REDMINE-1095_Reopen

    return scanprocedure->StartRFLPrescan(scanid);
    //-Patni-Hemant/2011Jan11/Modified/REDMINE-1095_Reopen
}

//*******************************Method Header*********************************
//Method Name    :StartCFAPrescan()
//Author         :PATNI/MSN
//Purpose        :Starts the CFA Prescan when done using CFA Retry Button
//*****************************************************************************
bool CPQMIPCManager::StartCFAPrescan(
    const int scanid
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::StartCFAPrescan");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return false;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    CString l_tracemsg(_T(""));
    l_tracemsg.Format(_T("Trace of StartCFAPrescan and the scan id is --> %d"), scanid);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_tracemsg);

    CPqmPmAcqman* l_acqman = m_pqmptr->GetAcqManager();
    CScanProcedure* l_scan_proc = l_acqman->GetScanProcedure();

    if (!l_scan_proc->RetryCFA(scanid, NULL)) {
        return false;
    }

    return true;
}

//*******************************Method Header*********************************
//Method Name    :ChangeShimCenterFrequency()
//Author         :PATNI/MSN
//Purpose        :Sends the Change Shim Center Frequency request to Acqman
//*****************************************************************************
void CPQMIPCManager::ChangeShimCenterFrequency(
    const double fsyvalue
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::ChangeShimCenterFrequency");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    CString l_tracemsg(_T(""));
    l_tracemsg.Format(_T("Trace of ChangeShimCenterFrequency and the fsyvalue is --> %f"), fsyvalue);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_tracemsg);

    CPqmPmAcqman* acqman = m_pqmptr->GetAcqManager();

    if (NULL == acqman) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("acqman is NULL"));
        return;
    }

    CScanProcedure* scanprocedure = acqman->GetScanProcedure();

    //+Patni-PJS/2010Mar10/Added/MCM0229-00036
    if (scanprocedure->GetCFARetry()) {
        CString str_msg(_T(""));
        str_msg.LoadString(IDS_ERR_ACQMAN_SHIMCTRL);
        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME, str_msg);
        return;
    }

    //-Patni-PJS/2010Mar10/Added/MCM0229-00036

    if (NULL == scanprocedure) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("scanprocedure is NULL"));
        return;
    }

    scanprocedure->SetCFValue(fsyvalue);
    //Patni-PJS/Modified/1Aug2009/Internal defect solved
    scanprocedure->SetScanProcProtocol(NULL);
    scanprocedure->SendShimCtrl(scanprocedure->GetPqmShimInfoptr());
}

/****************************Method Header************************************
//Method Name   :UpdateGUIControlsInDB()
//Author        :PATNI/JS
//Purpose       :For updating controls in PQM GUI
//*****************************************************************************/
//+Patni-RSG/Modified/2009Nov25/MSA0248-0033
BOOL CPQMIPCManager::UpdateGUIControlsInDB(
    VARIANT* pData,
    BSTR f_seriesid,
    const int f_scan_index
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::UpdateGUIControlsInDB");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of UpdateGUIControlsInDB"));

    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return FALSE;
    }

    //+Patni-RSG/Added/2009Nov25/MSA0248-0033

    if (FALSE == m_pqmptr->GetDbsaStudyMgr()->UpdateGUIControlsInDB(pData, f_seriesid, f_scan_index)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("UpdateGUIControlsInDB failed to update GUI controls in DB"));
        return FALSE;
    }

    return TRUE;

}

//****************************Method Header************************************
//Method Name    : SendGateModeInfo()
//Author         : PATNI/HAR
//Purpose        :
//*****************************************************************************
int CPQMIPCManager::SendGateModeInfo(
    const int f_gate_method
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SendGateModeInfo");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMIPCManager::SendGateModeInfo"));

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return FALSE;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections


    BOOL l_result = TRUE;
    CScanProcedure* l_scan_proc_ptr = m_pqmptr->GetAcqManager()->GetScanProcedure();
    CPQMStudy* l_pqm_study = m_pqmptr->GetStudy();
    CPqmProtocol* protocol = NULL;

    if (l_pqm_study->GetTopWaitPos(&protocol) < 0 || protocol == NULL) {

        CPQMLogMgr::GetInstance()->DisplayScanError(IDS_NO_WAIT_PROTOCOLS, _T("IDS_NO_WAIT_PROTOCOLS"), SCAN_ERR_NOTIFY, L"PqmIpcManager.cpp");

        l_result = FALSE;

    } else {

        //+Patni-HAR/2010Feb23/Modified/MSA0251-00183
        if (!l_scan_proc_ptr->GetIsScanning()) {

            l_scan_proc_ptr->SetScanProcProtocol(protocol);

            if (!l_scan_proc_ptr->SendGateModeInfo(f_gate_method, m_pqmptr->GetGateParamObj()->GetWirelessGateParam())) {

                PQM_TRACE(USER_FUNC_MARKER, _T("CPQMIPCManager::SendGateModeInfo"),
                          _T("Failed SendGateModeInfo"));
                l_result = FALSE;

            } else {
                PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Success SendGateModeInfo!"));
                l_result = TRUE;
            }

        } else {
            PQM_TRACE(USER_FUNC_MARKER, _T("CPQMIPCManager::SendGateModeInfo"),
                      _T("Scan is going on. During Scan GateMethod cannot be sent"));
            l_result = FALSE;
        }

        //-Patni-HAR/2010Feb23/Modified/MSA0251-00183
    }

    return l_result;
}

//****************************Method Header************************************
//Method Name    : PutInjectTimeInDB()
//Author         : PATNI/HAR-LK
//Purpose        :
//*****************************************************************************
void CPQMIPCManager::PutInjectTimeInDB(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::PutInjectTimeInDB");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return;

    } else {
        //-Patni-MJC/2009Aug17/Modified/cpp test corrections

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of PutInjectTimeInDB"));

        m_pqmptr->GetStudy()->PutInjectTimeInDB();
    }

    if (m_pqmptr->GetMulitScanDynamic_License()) {
        SendMessageToGUI(PQM_MSG_INJECT_TIMER_VISIBLE);

    } else {
        SendMessageToGUI(PQM_MSG_INJECT_TIME_INVISIBLE);
    }
}


//****************************Method Header************************************
//Module         : SendSequencesToMemory
//Author         : PATNI/JS
//Purpose        : method send sequences to memory
//*****************************************************************************
void CPQMIPCManager::SendSequencesToMemory(
    VARIANT* pdata
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SendSequencesToMemory");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return;

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SendSequencesToMemory"));
        m_pqmptr->SendSequencesToMemory(pdata);
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//****************************Method Header************************************
//Method Name   :DeleteProtocolFromVarField()
//Author        :PATNI / HAR
//Purpose       :
//*****************************************************************************
//Patni-HAR/2010Sep20/Modified/Rename function from SendIndex() to DeleteProtocolFromVarField() SM Refarctoring.
void CPQMIPCManager::DeleteProtocolFromVarField(
    const int f_index
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::DeleteProtocolFromVarField");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    m_pqmptr->GetStudy()->DeleteProt(f_index);
}

/****************************Method Header************************************
//Method Name   :UpdateGUIControlsInVarfieldDB()
//Author        :PATNI/SG
//Purpose       :For updating controls values in Varfield DB
//*****************************************************************************/
BOOL CPQMIPCManager::UpdateGUIControlsInVarfieldDB(
    VARIANT* f_pdata,
    const int f_scan_index
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::UpdateGUIControlsInVarfieldDB");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return false;

    } else {
        //-Patni-MJC/2009Aug17/Modified/cpp test corrections

        PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMIPCManager::UpdateGUIControlsInVarfieldDB"));

        return  m_pqmptr->GetStudy()->UpdateGUIControlsInVarfieldDB(f_pdata, f_scan_index);
    }

}

//****************************Method Header************************************
//Method Name   :DuplicateProtocol()
//Author        :PATNI / HAR
//Purpose       :
//*****************************************************************************
BOOL CPQMIPCManager::DuplicateProtocol(
    const int f_from_pos,
    const int f_to_pos,
    const int f_group_id /* =0 */
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::DuplicateProtocol");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return false;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    const int f_acq_order = m_pqmptr->GetStudy()->DuplicateProtocol(f_from_pos, f_to_pos, f_group_id);
    CString l_string_acq_order(_T(""));
    l_string_acq_order.Format(_T("*** CPQMIPCManager::DuplicateProtocol = %d"), f_acq_order);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_string_acq_order);
    return f_acq_order;
}

//****************************Method Header************************************
//Method Name   :DuplicateSequenceInVarfield()
//Author        :PATNI /SG
//Purpose       :
//*****************************************************************************
void CPQMIPCManager::DuplicateSequenceInVarfield(
    int f_pos_list
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::DuplicateSequenceInVarfield");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    m_pqmptr->GetStudy()->DuplicateSequence(&f_pos_list);
}

//****************************Method Header************************************
//Method Name   :DuplicateSequence()
//Author        :PATNI /SG
//Purpose       :
//*****************************************************************************
void CPQMIPCManager::ReArrange(
    const int f_drag_index,
    const int f_drop_index
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::ReArrange");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    m_pqmptr->GetStudy()->ReArrange(f_drag_index, f_drop_index);
}

//****************************Method Header************************************
//Module         : PQMManualPrescanClose
//Author         : PATNI/JS
//Purpose        :
//*****************************************************************************
void CPQMIPCManager::PQMManualPrescanClose(
)
{

    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::PQMManualPrescanClose");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return ;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    CPqmPmAcqman* pqm_pm_acqman = m_pqmptr->GetAcqManager();
    CScanProcedure* l_scan_proc = pqm_pm_acqman->GetScanProcedure();
    l_scan_proc->PQMManualPrescanClose();


}

//****************************Method Header************************************
//Module         : PQMManualPrescanApply
//Author         : PATNI/JS
//Purpose        :
//
//*****************************************************************************
void CPQMIPCManager::PQMManualPrescanApply(
    VARIANT* f_pdata
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::PQMManualPrescanApply");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return ;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    CPqmPmAcqman* pqm_pm_acqman = m_pqmptr->GetAcqManager();
    CScanProcedure* l_scan_proc = pqm_pm_acqman->GetScanProcedure();
    l_scan_proc->PQMManualPrescanApply(f_pdata);


}

int CPQMIPCManager::ProconReqUiCB(int f_tag, const int f_acq_order, const int f_maskValue)const
{
    if (m_pqmptr) {
        if (m_pqmptr->GetIsProconConnected() && m_pqmptr->GetPqmProCon()) {

            //+Patni-HEMANT/ADDED On 6/22/2009 10:40:38 AM/ ACE-2 / WFDA
            ////Modified by Ashish On 2/10/2009 3:46:44 PM
            ////return m_pqmptr->GetPqmProCon()->ProconRequestUiCB(f_tag, f_acq_order);
            //return m_pqmptr->GetPqmProCon()->ProconRequestUiCB(f_tag, f_acq_order);
            //Patni-Ajay/2010July23/Modified/TMSC-REDMINE-443
            return m_pqmptr->GetPqmProCon()->ProconRequestUiCB(f_tag, f_acq_order, false , f_maskValue);

        } else {
            return FALSE;
        }

    } else {
        //-Patni-HEMANT/ADDED On 6/22/2009 10:40:38 AM/ ACE-2 / WFDA
        return FALSE;
    }
}

//****************************Method Header************************************
//Module         : MrsScanStartUiCB
//Author         : PATNI/SS
//Purpose        :
//
//*****************************************************************************
int CPQMIPCManager::MrsScanStartUiCB(SMRSScan_s* f_mrs_prescan_data)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::MrsScanStartUiCB");

    int l_result = FALSE;

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return l_result;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    CScanProcedure* l_scan_proc = m_pqmptr->GetAcqManager()->GetScanProcedure();
    CPqmMRSScan* l_mrs_prescan = (CPqmMRSScan*)l_scan_proc->GetCurrentTransaction();

    if (l_mrs_prescan) {

        l_result = l_mrs_prescan->SetScanKind(f_mrs_prescan_data);

        //+Patni-HAR/2009April09/Added/MRS Scan Fix
        if (FALSE == l_result) {
            l_scan_proc->GetScanProcProtocol()->SetAcqStatus(C_PR_STATUS_WAIT);
            m_pqmptr->GetStudy()->SetAcqStatus(l_scan_proc->GetScanProcProtocol(),
                                               C_PR_STATUS_WAIT);
            m_pqmptr->GetStudy()->SetScanModeOfHead();
            l_scan_proc->ResetCurrentRequest() ;
            SendMessageToGUI(PQM_MSG_ABORT_WAIT);

        }

        //-Patni-HAR/2009April09/Added/MRS Scan Fix
    }

    return l_result;
}


//****************************Method Header************************************
//Module         : PqmSeverConfigToProCon
//Author         : PATNI/SS
//Purpose        :
//*****************************************************************************
int CPQMIPCManager::PqmSeverConfigToProCon(const int f_tag, const int f_index, int* f_list, const int f_size)const
{
    if (m_pqmptr) {
        if (m_pqmptr->GetPqmProCon() && m_pqmptr->GetIsProconConnected()) {
            return m_pqmptr->GetPqmProCon()->SeverConfigToProCon(f_tag, f_index, f_list, f_size);

        } else {
            return E_ERROR;
        }

    } else {
        //+Patni-RSG Modified and Added /2009Jun15/JFT-324
        return E_ERROR;
        //-Patni-RSG Modified and Added /2009Jun15/JFT-324
    }
}


//****************************Method Header************************************
//Module         : SendElectrodeType
//Author         : PATNI/ASHISH SM3
//Purpose        :
//*****************************************************************************
int CPQMIPCManager::SendElectrodeType(
    const int f_electrodetype
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SendElectrodeType");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqmptr) {
        CPqmPmAcqman* l_pqm_pm_acqman = m_pqmptr->GetAcqManager();

        if (l_pqm_pm_acqman) {
            CScanProcedure* l_scan_procedure = l_pqm_pm_acqman->GetScanProcedure();

            if (l_scan_procedure) {
                if (l_scan_procedure->SendElectrodeType(f_electrodetype)) {
                    return TRUE;

                } else {
                    return FALSE;
                }

            } else {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_scan_procedure pointer is NULL"));
                return FALSE;
            }

        } else {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqm_pm_acqman pointer is NULL"));
            return FALSE;
        }

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return FALSE;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//****************************Method Header************************************
//Module         : SendMessageToAcqManForCoil()
//Author         : PATNI / HAR
//Purpose        :
//*****************************************************************************
BOOL CPQMIPCManager::SendMessageToAcqManForCoil(
    const int f_mode,
    const int f_acq_order
)
{
    //Patni-KSS/2011Feb17/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(f_mode);

    //Patni-AMT/2009Sep09/Added/MVC004983 -  Code Review
    CString l_func_name = _T("CPQMIPCManager::SendMessageToAcqManForCoil");
    CString	l_msg_data = _T("");
    l_msg_data.Format(_T("Trace - Reqesting coil for AcqOrder = %d"), f_acq_order);
    PQM_TRACE(USER_FUNC_MARKER, l_func_name, l_msg_data);

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqmptr) {
        //+Patni-PJS/2009Oct23/Added/MVC005086
        CCoilSelectionTool* l_coil_sel_tool = m_pqmptr->GetCoilSelectionTool();

        if (l_coil_sel_tool != NULL) {
            l_coil_sel_tool->SetAcqSendFlag(FALSE);

        } else {
            PQM_TRACE(USER_FUNC_MARKER, l_func_name,
                      _T("CCoilSelectionTool obj is NULL-Can not reset m_acq_send_flag"));
        }

        //-Patni-PJS/2009Oct23/Added/MVC005086
        CPqmPmAcqman* l_acqman = m_pqmptr->GetAcqManager();

        if (l_acqman) {
            CScanProcedure* l_scan_proc = l_acqman->GetScanProcedure();

            if (l_scan_proc) {
                CPQMStudy* l_pqm_study = m_pqmptr->GetStudy();

                if (l_pqm_study) {
                    CPqmProtocol* l_protocol = l_pqm_study->GetProtocol(f_acq_order);
                    return l_scan_proc->SendAcqInfoForCoil(l_protocol);

                } else {
                    PQM_TRACE(USER_FUNC_MARKER, l_func_name,
                              _T("l_pqm_study pointer is NULL"));
                    return FALSE;
                }

            } else {
                PQM_TRACE(USER_FUNC_MARKER, l_func_name,
                          _T("l_scan_proc pointer is NULL"));
                return FALSE;
            }

        } else {
            PQM_TRACE(USER_FUNC_MARKER, l_func_name,
                      _T("l_acqman pointer is NULL"));
            return FALSE;
        }

    } else {
        PQM_TRACE(USER_FUNC_MARKER, l_func_name,
                  _T("m_pqmptr pointer is NULL"));
        return FALSE;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//****************************Method Header************************************
//Module         : GetConnectCoilInfo()
//Author         : PATNI / HAR
//Purpose        :
//*****************************************************************************
void CPQMIPCManager::SendCoilSelReadyToIPC(
    const int f_index
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SendCoilSelReadyToIPC");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqmptr) {
        m_pqmptr->SendCoilSelectionReady(f_index);

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    //return &l_coil_info;
}

//****************************Method Header************************************
//Module         : SendSelectedCoilListToIPC()
//Author         : PATNI / HAR
//Purpose        :
//*****************************************************************************
void CPQMIPCManager::SendSelectedCoilListToIPC(
    VARIANT* pdata,
    const int f_acq_order
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SendSelectedCoilListToIPC");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqmptr) {
        coilSelect_t* l_select_coil = (coilSelect_t*)pdata->byref;
        CCoilSelectionTool* l_coil_selection_tool = m_pqmptr->GetCoilSelectionTool();

        if (l_coil_selection_tool) {
            if (!l_coil_selection_tool->SaveSelectedCoilToStudy(l_select_coil, f_acq_order)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SaveSelectedCoilToStudy() failed."));
            }

        } else {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_coil_selection_tool pointer is NULL"));
        }

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}


//****************************Method Header************************************
//Module         : SendConnectedCoilListToIPC()
//Author         : PATNI / HAR
//Purpose        :
//*****************************************************************************
void CPQMIPCManager::SendConnectedCoilListToIPC(
    VARIANT* pdata,
    const int f_acq_order
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SendConnectedCoilListToIPC");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqmptr) {

        coilSelectListStruct_t*  coilSelectListStructObj = (coilSelectListStruct_t*) pdata->byref;
        CCoilSelectionTool* l_coil_selection_tool = m_pqmptr->GetCoilSelectionTool();

        if (l_coil_selection_tool) {
            if (!l_coil_selection_tool->SaveSelectedListToStudy(coilSelectListStructObj, f_acq_order)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("SaveSelectedListToStudy() failed."));
            }

        } else {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_coil_selection_tool pointer is NULL"));
        }

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

}

//****************************Method Header************************************
//Module         : OnPQMExit()
//Author         : PATNI
//Purpose        :
//*****************************************************************************
void CPQMIPCManager::OnPQMExit(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::OnPQMExit");

    //+Patni-RUP/2010Jul29/Modified/Added Try Catch
    try {
        //-Patni-RUP/2010Jul29/Modified/Added Try Catch
        //+Patni-PJS/2010May09/Added/JaFT# IR-141
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("OnPQMExit"));

        //-Patni-PJS/2010May09/Added/JaFT# IR-141
        //+Patni-MJC/2009Aug17/Modified/cpp test corrections
        if (m_pqmptr == NULL) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
            return;
        }


        m_pqmptr->DeleteIRSListnerInstance();
        //-Patni-MJC/2009Aug17/Modified/cpp test corrections

        //+Patni-RUP/2010Aug06/Added/To fix PQM exit crash
        CPqmProcon* l_procon = m_pqmptr->GetPqmProCon();

        if (NULL != l_procon) {
            l_procon->SetPQMExitFlag(true);
        }

        //-Patni-RUP/2010Aug06/Added/To fix PQM exit crash

        //+Patni-Sribanta/2011Jan14/Commented/TMSC-REDMINE-1026
        // Close voice manger
        //if (m_pqmptr->GetMode()  == K_IMAGING) {
        //CVoiceManager::GetInstance()->SafeExit();
        //}
        //-Patni-Sribanta/2011Jan14/Commented/TMSC-REDMINE-1026

        // Close Local Study file
        if (m_pqmptr) {

            //+Patni-RUP/2009Aug24/Modified/cpp test corrections
            //m_pqmptr->GetStudyManagerPtr()->FreeStudyLoid();
            CPQMStudyMgr* l_pqm_studymgr = m_pqmptr->GetStudyManagerPtr();

            if (l_pqm_studymgr != NULL) {
                l_pqm_studymgr->FreeStudyLoid();
            }

            //-Patni-RUP/2009Aug24/Modified/cpp test corrections

            // Close Study file
            m_pqmptr->GetStudy()->CloseStudy();
        }

        // Close thread.
        PostThreadMessage(CPQMIPCManager::dThreadId, MSG_CLOSE, NULL, NULL);

        if (CPQMIPCManager::hThread) {
            CloseHandle(CPQMIPCManager::hThread);
            CPQMIPCManager::hThread = NULL;
        }

        if (CPQMIPCManager::m_ThreadEvent) {
            CloseHandle(CPQMIPCManager::m_ThreadEvent);
            CPQMIPCManager::m_ThreadEvent = NULL;
        }

        if (m_pqmptr) {
            CPqmPmAcqman* l_pqm_acqman = m_pqmptr->GetAcqManager();

            if (l_pqm_acqman) {
                CPQMSocket* l_acqman_socket = l_pqm_acqman->GetPQMSocket();

                if (l_acqman_socket) {
                    if (!l_acqman_socket->Close()) {
                        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                                  _T("Unable to Close the Socket Connection"));
                    }
                }
            }

            CPqmProcon* l_procon = m_pqmptr->GetPqmProCon();

            if (l_procon) {
                CPQMSocket* l_socket = l_procon->GetProconSocket();

                if (l_socket) {
                    if (!l_socket->Close()) {
                        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                                  _T("Unable to Close the procon Socket Connection"));
                    }
                }
            }
        }

        //+Patni-RUP/2010Jul29/Modified/Added Try Catch

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception occured in OnPQMExit function"));
    }

    //-Patni-RUP/2010Jul29/Modified/Added Try Catch
}

void CPQMIPCManager::SendAcqInfoForGate(
)const
{
    if (m_pqmptr) {
        m_pqmptr->SendAcqInfoForGate();
    }
}

//****************************Method Header************************************
//Method Name   :ConnectToAcqMan()
//Author        :PATNI / SS
//Purpose       :Make a Sockect Connection with Acqman
//*****************************************************************************
void CPQMIPCManager::ConnectToAcqMan()
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::ConnectToAcqMan");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return ;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqmptr->GetMode() == K_IMAGING && m_pqmptr->GetAcqManager()) {
        m_is_acqman_connected = m_pqmptr->GetAcqManager()->ConnectToAcqman() ;
    }
}

//********************************Method Header********************************
//Method Name   : CheckNextProtocol()
//Author        : PATNI/MRP
//Purpose       : Checks the next protocol for Auto Scan
//*****************************************************************************
void CPQMIPCManager::CheckNextProtocol(
    const int f_protocol_no,
    const BOOL f_call_frm_gui	//+Patni-HAR-RG/2009June09/Modified/JFT#50
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::CheckNextProtocol");
    CString l_print_string(_T(""));
    l_print_string.Format(_T("Flag = %d"), f_call_frm_gui);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_print_string);

    if (m_pqmptr) {
        m_pqmptr->CheckNextProtocol(f_protocol_no, f_call_frm_gui);
    }
}


//Patni-ARD/20090306/Added/ACE-1# handling of stabilization Start
//********************************Method Header********************************
//Method Name   : FileCopy()
//Author        : PATNI/ARD
//Purpose       : Replacement of CopyFile() with library function AcqFileCopy()
//*****************************************************************************
int CPQMIPCManager::FileCopy(
    BSTR f_existingfilename,
    BSTR f_newfilename
)
{
    if (m_pqmptr) {
        return m_pqmptr->FileCopy(f_existingfilename, f_newfilename);
    }

    return -1;
}
//Patni-ARD/20090306/Added/ACE-1# handling of stabilization End

void CPQMIPCManager::ConnectToProconFromGUI()
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::ConnectToProconFromGUI");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (!m_pqmptr->ConnectProCon()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Unable to reconnect proCon "));
        SendMessageToGUI(PQM_MSG_PROCON_NOT_CONNECTED);

    } else {

        SendMessageToGUI(PQM_MSG_PROCON_CONNECTED);
    }

}

//************************************Method Header************************************
// Method Name  : RecvCallBack
// Author       : PATNI/ HEMANT
// Purpose      : processes received messages
//*************************************************************************************
BOOL CPQMIPCManager::RecvCallBackCFW(
    void* messageParam,
    void* messageRet,
    void* pCustom
)
{
    return RecvCallBack(messageParam, messageRet, pCustom);

    return TRUE ;
}

//************************************Method Header************************************
// Method Name  : DeleteProtocol
// Author       : PATNI/ HEMANT
// Purpose      :
//*************************************************************************************
bool CPQMIPCManager::DeleteProtocol_InScanServer(
    const int f_protocol_index
)const
{
    if (m_pqmptr) {

        return m_pqmptr->DeleteProtocol(f_protocol_index);
    }

    return true;
}



//************************************Method Header************************************
// Method Name  : DuplicateProtocol
// Author       : PATNI/ HEMANT
// Purpose      :
//*************************************************************************************
bool CPQMIPCManager::DuplicateProtocol_InScanServer(
    const int f_index_to_duplicate,
    const int f_duplicate_index
)const
{

    if (m_pqmptr) {

        return m_pqmptr->DuplicateProtocol(f_index_to_duplicate, f_duplicate_index);
    }

    return true;
}

//+Patni-ARD/2009Oct28/Added/Defect Fix# Auto Transfer
/***************************Method Header*************************************/
//Method Name    : CanEnableAutoTransfer()
//Author         : PATNI/ARD
//Purpose        :
//*****************************************************************************/
bool CPQMIPCManager::CanEnableAutoTransfer()const
{
    return CPQMConfig::GetInstance()->CanEnableAutoTransfer();
}
//-Patni-ARD/2009Oct28/Added/Defect Fix# Auto Transfer






/***************************Method Header*************************************/
//Method Name    : StartPQMDebug()
//Author         : PATNI/DKH
//Purpose        : Starts Debug option logging
//*****************************************************************************/
bool CPQMIPCManager::StartPQMDebug(const int f_debugOption, const UINT f_buttonmask)const
{
    if (NULL != m_pqmptr) {
        m_pqmptr->StartPQMDebug(f_debugOption, f_buttonmask);
        return true;
    }

    return false;
}

//+Patni-RAJ/2010Jan18/Added/PH#3#/IR86/DebugWindow
void CPQMIPCManager::SelectListandTotalCnt(int* f_selected_list, const int f_size)
{
    m_selected_list = f_selected_list;
    m_total_count = f_size;
}

//-Patni-RAJ/2010Jan18/Added/PH#3#/IR86/DebugWindow

/***************************Method Header*************************************/
//Method Name    : PqmDebugStatusLogOpen()
//Author         : PATNI/DKH
//Purpose        :
//*****************************************************************************/
bool CPQMIPCManager::PqmDebugStatusLogOpen()const
{
    if (NULL != m_pqmptr) {
        return m_pqmptr->PqmDebugStatusLogOpen();
    }

    return false;
}
//- Patni-DKH/2010Jan27/Added/PH#3#/IR86/DebugWindow

//+Patni-DKH/2010Jan12/Added/IR-87
//**************************************************************************
//Method Name   : InitWithDefaultSettings
//Author        : PATNI\DKH
//Purpose       :
//**************************************************************************
bool CPQMIPCManager::InitWithDefaultSettings()const
{
    if (m_pqmptr) {
        //Patni-AMT/2010Apr22/Modified/JaFT# IR87_CDR_13-Code Review
        return m_pqmptr->InitPlanDupSettingsFile();

    } else {
        return false;
    }

    return true;
}

//**************************************************************************
//Method Name   : ReadPlanDuplicateSettings
//Author        : PATNI\DKH
//Purpose       : To read plan duplicate settings
//**************************************************************************
bool CPQMIPCManager::ReadPlanDuplicateSettings(
    VARIANT** f_data
)const
{
    if (m_pqmptr) {
        CPlanDuplicateSettings* l_planDuplicateSettings = m_pqmptr->GetPlanDuplicateSettings();

        if (NULL != l_planDuplicateSettings) {
            l_planDuplicateSettings->ReadPlanDuplicateSettings(f_data);

        } else {
            return false;
        }

    } else {
        return false;
    }

    return true;
}

//**************************************************************************
//Method Name   : WritePlanDuplicateMask
//Author        : PATNI\DKH
//Purpose       : To write plan duplicate settings
//**************************************************************************
bool CPQMIPCManager::WritePlanDuplicateMask(const int32_t f_imask , BOOL f_bsaveflag)const
{
    if (m_pqmptr) {
        CPlanDuplicateSettings* l_planDuplicateSettings = m_pqmptr->GetPlanDuplicateSettings();

        if (NULL != l_planDuplicateSettings) {
            if (false == l_planDuplicateSettings->WritePlanDuplicateMask(f_imask, f_bsaveflag)) {
                return false;
            }

        } else {
            return false;
        }

    } else {
        return false;
    }

    return true;
}
//-Patni-DKH/2010Jan12/Added/IR-87

//+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : OnOffCDSDebug
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMIPCManager::OnOffCDSDebug(
    const BOOL f_OnOff
)const
{
    if (m_pqmptr) {
        m_pqmptr->OnOffCDSDebug(f_OnOff);
    }
}

//************************************Method Header************************************
// Method Name  : OnOffAsCompass
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPQMIPCManager::OnOffAsCompass(
    const int f_protocol,
    const bool f_OnOff
)const
{
    if (m_pqmptr) {
        return m_pqmptr->OnOffAsCompass(f_protocol, f_OnOff);
    }

    return false ;
}

//************************************Method Header************************************
// Method Name  : CDSDebugClosed
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPQMIPCManager::CDSDebugClosed(
    VARIANT** f_compass_debug_param,
    bool f_set
)
{
    if (m_pqmptr) {
        return m_pqmptr->CDSDebugClosed(f_compass_debug_param, f_set);
    }

    return false;
}
//-Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMIPCManager::CoilSelectionClosed(
    const BOOL f_ok_pressed
)const
{
    if (m_pqmptr) {
        m_pqmptr->CoilSelectionClosed(f_ok_pressed);
    }
}

//+Patni-RUP/2010Apr21/Added/To Fix TMSC-REDMINE-68 defect
//******************************************************************************
//Function Name:- OnShowHidePQM()
//Author       :- PATNI\RUP
//Purpose      :- This function is called when PQM is hide or shown.
//******************************************************************************
void CPQMIPCManager::OnShowHidePQM(const BOOL& f_show_flag)
{
    if (m_pqmptr) {
        CPqmProcon* l_pqm_procon = m_pqmptr->GetPqmProCon();

        if (l_pqm_procon) {
            CPqmProbe* l_pqm_probe = l_pqm_procon->GetPQMProbe();

            //Show or hide probe window
            //+Patni-RUP/2010Jun09/Added/MVC007947
            if (l_pqm_probe) {
                l_pqm_probe->ShowHideProbeWindow(f_show_flag);
            }

            //-Patni-RUP/2010Jun09/Added/MVC007947
        }
    }
}
//-Patni-RUP/2010Apr21/Added/To Fix TMSC-REDMINE-68 defect

//+Patni-Manish/2010Jan11/Added/Ph#3#IR-122/RemoteConsole
/***************************Method Header*************************************/
//Method Name    : RemoteCheckActiveFlag()
//Author         : PATNI/Manishkumar
//Purpose        : Check the remote flag from libRemote dll.
//*****************************************************************************/
bool CPQMIPCManager::RemoteCheckActiveFlag()const
{
    //MEITEC/2010Apr08/Modified/Ph#3 IR-122/RemoteConsole
    return remoteCheckActiveFlag() ? true : false;
}
//-Patni-Manish/2010Jan11/Added/Ph#3#IR-122/RemoteConsole

/***************************Method Header*************************************/
//Method Name    : SendMessageToGUI()
//Author         : PATNI/HAR
//Purpose        : This function call to a callback function
//*****************************************************************************/
void CPQMIPCManager::SendMessageToGUI(const pqm_msg_tag Msg
                                     )
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SendMessageToGUI");
    PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME, _T("Trace"));

    // Callback function
    m_ptrCallback(Msg, m_pComObj);
}

//+ Patni-DKH/2010Jan27/Added/PH#3#/IR86/DebugWindow
/***************************Method Header*************************************/
//Method Name    : SendDebugMessageToGUI()
//Author         : PATNI/DKH
//Purpose        : This function call to a callback function
//*****************************************************************************/
void CPQMIPCManager::SendDebugMessageToGUI(BSTR f_debugdata
                                          )
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SendDebugMessageToGUI");
    PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME,
              _T("Trace of SendDebugMessageToGUI"));

    // Callback function
    m_ptrdisplayCallback(f_debugdata, m_pComObj);

}

//+Patni-PJS/2011Feb24/Added/TMSC_REDMINE-1480
/***************************Method Header*************************************/
//Method Name    : InvokeMRSProcessTool()
//Author         : PJS
//Purpose        : This function is used invoke MRS ProcessTool
//*****************************************************************************/
BOOL CPQMIPCManager::InvokeMRSProcessTool(
    const int f_acq_order
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::InvokeMRSProcessTool");

    CPQMStudy* l_pqm_study = m_pqmptr->GetStudy();

    if (!l_pqm_study) {
        return FALSE;
    }

    CPqmProtocol* l_pqm_prot = l_pqm_study->GetProtocol(f_acq_order);

    if (l_pqm_prot->GetAppCode() == VFC_APP_CODE_MRS ||
        l_pqm_prot->GetAppCode() == VFC_APP_CODE_MRSCSI) {


        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME
                  , _T("Trace"));

        //Patni-PJS/2009Sep16/Modified/TMSC Code review comment
        if (NULL == l_pqm_prot) {
            //Patni-AMT/2010Jan05/Modified/DeFT# FN_MRS-86 Review cmt
            PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                      _T("CPqmMRSScan::InvokeMRSProcessTool->l_pqm_prot is NULL"));

            return FALSE;
        }

        if (FALSE == CPqmUtilities::InvokeMRSProcessTool(l_pqm_prot)) {
            return FALSE;
        }

        //+Patni-PJS/2011Mar2/Modified/TMSC_REDMINE-1480-Review Comments
        if (m_mrs_thread_handle) {
            CloseHandle(m_mrs_thread_handle);
            m_mrs_thread_handle = NULL;
        }

        m_mrs_thread_handle = (HANDLE)_beginthreadex(0,
                              0,
                              CPqmUtilities::IsMRSProcessToolAlive,
                              this,
                              0,
                              &m_mrs_thread_id);

        if (!m_mrs_thread_handle) {
            PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME,
                      _T(" failed to create thread handle"));
        }

        //-Patni-PJS/2011Mar2/Modified/TMSC_REDMINE-1480-Review Comments
        return TRUE;
    }

    return FALSE;
}

/***************************Method Header******************************************/
//Method Name    : IsSelectedSeqMRS()
//Author         : PJS
//Purpose        :
//*********************************************************************************/
BOOL CPQMIPCManager::IsSelectedSeqMRS(
    const int f_acq_order
)
{
    if (!m_pqmptr) {
        return FALSE;
    }

    CPqmProtocol* protocol = m_pqmptr->GetStudy()->GetProtocol(f_acq_order);

    if (!protocol) {
        return FALSE;
    }

    if (protocol->GetAppCode() == VFC_APP_CODE_MRS ||
        protocol->GetAppCode() == VFC_APP_CODE_MRSCSI) {
        return TRUE;
    }

    return FALSE;
}
//-Patni-PJS/2011Feb24/Added/TMSC_REDMINE-1480

//+Patni-PJS/2011Jan25/Modified/IR-97
//****************************Method Header************************************
//Method Name   :IsProtocolInScan()
//Author        :PATNI/PJS
//Purpose       :.
//*****************************************************************************
bool CPQMIPCManager::IsProtocolInScan(
    int f_acq_order
)
{
    CPQMStudy* l_pqm_study = m_pqmptr->GetStudy();

    if (NULL == l_pqm_study) {
        return false;
    }

    return l_pqm_study->IsProtocolInScan(f_acq_order);

}

//****************************Method Header************************************
//Method Name   :CheckPlnStatus()
//Author        :PATNI/PJS
//Purpose       :Check Pln Status of the protocol from varfiled.
//*****************************************************************************
bool CPQMIPCManager::CheckPlnStatus(
    int f_protocol
)
{
    CPQMStudy* l_pqm_study = m_pqmptr->GetStudy();

    if (NULL == l_pqm_study) {
        return false;
    }

    if (E_ERROR == l_pqm_study->CheckPlnStatus(f_protocol)) {
        CPQMLogMgr::GetInstance()->DisplayScanError(
            IDS_ERR_SCAN_EDIT_NOT_DONE, _T("IDS_ERR_SCAN_EDIT_NOT_DONE"), SCAN_ERR_NOTIFY, L"PqmPmAcqman.cpp");
        return false;
    }

    return true;
}

//****************************Method Header************************************
//Method Name   :GetProtocolInformation()
//Author        :PATNI/PJS
//Purpose       :.
//*****************************************************************************
bool CPQMIPCManager::GetProtocolInformation(
    VARIANT* pData,
    int f_acq_order
)
{
    CPQMStudy* l_pqm_study = m_pqmptr->GetStudy();

    if (NULL == l_pqm_study) {
        return false;
    }

    WFDAProtocolInfoStruct* l_wfdaprotocol_infostruct = new WFDAProtocolInfoStruct;
    CPqmProtocol* l_protocol = l_pqm_study->GetProtocol(f_acq_order);
    l_wfdaprotocol_infostruct->m_study_loid = l_pqm_study->GetStudyLoid();
    l_wfdaprotocol_infostruct->m_series_loid = l_protocol->GetSeriesLOID();
    l_wfdaprotocol_infostruct->m_pguid = l_pqm_study->GetPGUIDForProtocol(f_acq_order);

    if (l_protocol->GetInscanSwitchFlag()) {
        l_wfdaprotocol_infostruct->m_inscanswitchflag = true;
        l_wfdaprotocol_infostruct->m_inscanswitchtype = l_protocol->GetInscanSwitchType();

        if (m_pqmptr->IsPGUIDDataXmlFilePresent()) {
            l_wfdaprotocol_infostruct->m_defaultpguid =
                m_pqmptr->GetDefaultPGUIDDataManager()->GetPGUIDFromXML(l_protocol->GetInscanSwitchType(), f_acq_order, l_pqm_study);

        } else {
            l_wfdaprotocol_infostruct->m_defaultpguid = GetDefaultPGUID(l_protocol->GetInscanSwitchType());
        }
    }

    pData->byref = l_wfdaprotocol_infostruct;

    return true;

}
//-Patni-PJS/2011Jan25/Modified/IR-97

//**************************************************************************
//Method Name   :  CalculateTimeDiff()
//Author        :  PATNI/AKR
//Purpose       :  This function is used to get the inject time from study file.
//**************************************************************************
void CPQMIPCManager::CalculateTimeDiff(
    VARIANT* f_pdata
)
{
    if (m_pqmptr) {
        m_pqmptr->GetStudy()->CalculateTimeDiff(f_pdata);

    } else {
        PQM_TRACE(SYSTEM_FUNC_MARKER, _T("CPQMIPCManager::CalculateTimeDiff"),
                  _T("m_pqmptr is NULL"));
    }
}

//**************************************************************************
//Method Name   :  GetProtectedStatus()
//Author        :  IGATE/SS
//Purpose       :  This function is used to read value for protected tag.
//**************************************************************************
BOOL CPQMIPCManager::GetProtectedStatus(
    BSTR f_bStr
)
{
    if (m_pqmptr) {
        return (m_pqmptr->GetStudyManagerPtr()->ReadProtectedStatus(f_bStr));

    } else {
        PQM_TRACE(SYSTEM_FUNC_MARKER, _T("CPQMIPCManager::GetProtectedStatus"),
                  _T("m_pqmptr is NULL"));
        return FALSE;
    }
}

//**************************************************************************
//Method Name   :  SetDelayTimeToVf()
//Author        :  PATNI/AKR
//Purpose       :  This function is used to set delay time in vf DB
//**************************************************************************
void CPQMIPCManager::SetDelayTimeToVf(
    VARIANT* f_pdata
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SetDelayTimeToVf");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, FUNC_NAME);

    if (NULL == m_pqmptr) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr is NULL"));
        return;
    }

    int l_acq_order = -1;
    int l_delay_time = 0;

    if (f_pdata) {
        l_acq_order = f_pdata->intVal;
        l_delay_time = (int)f_pdata->wReserved1;
    }

    m_pqmptr->GetStudy()->SetNewDelayTimeToVf(l_delay_time, l_acq_order);
}

//**************************************************************************
//Method Name   :  ExecuteAutoScanStart()
//Author        :  PATNI/AKR
//Purpose       :
//**************************************************************************
void CPQMIPCManager::ExecuteAutoScanStart()
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::ExecuteAutoScanStart");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, FUNC_NAME);


    if (m_pqmptr->GetAcqManager()->GetInjectTimePresent() &&
        !m_pqmptr->GetProcessing() && !m_pqmptr->GetPostProcessFlag()) {
        m_pqmptr->GetAcqManager()->AutoScanStart();
    }
}

//**************************************************************************
//Method Name   :  AllocateGroupIdForDuplicateProtocols()
//Author        :  PATNI/AKR
//Purpose       :  This function is used to retrieve the scan time for
//				   currently selected protocol.
//**************************************************************************
void CPQMIPCManager::AllocateGroupIdForDuplicateProtocols(VARIANT* f_pdata)
{
    m_pqmptr->AllocateGroupIdForDuplicateProtocols(f_pdata);
}
//**************************************************************************
//Method Name   :  OnDeleteUpdateMasterSlave()
//Author        :  PATNI/AKR
//Purpose       :  This function will update master-slaves group id on deleting a protocol
//**************************************************************************
void CPQMIPCManager::OnDeleteUpdateMasterSlave(VARIANT* f_pdata)
{
    m_pqmptr->OnDeleteUpdateMasterSlave(f_pdata);
}
//**************************************************************************
//Method Name   :  UpdateGroupNum()
//Author        :  PATNI/AKR
//Purpose       :  This function will update group num tag with the maximum group id present
//**************************************************************************
void CPQMIPCManager::UpdateGroupNum()
{
    m_pqmptr->UpdateGroupNum();
}
//**************************************************************************
//Method Name   :  GetProtocolInfoForAutoCopy()
//Author        :  PATNI/Varun
//Purpose       :  This function is used to get the slave protocol count and slaves indexes
//**************************************************************************
BOOL CPQMIPCManager::GetProtocolInfoForAutoCopy(
    VARIANT* f_pdata
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetProtocolInfoForAutoCopy");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, FUNC_NAME);

    if (m_pqmptr) {
        m_pqmptr->GetProtocolInfoForAutoCopy(f_pdata);

    } else {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr is NULL"));
        return FALSE;
    }

    return TRUE;

}

int* CPQMIPCManager::GetSelectedList()const
{
    return  m_selected_list;
}

int CPQMIPCManager::GetTotalListCnt()const
{
    return  m_total_count;
}

/***************************Method Header*************************************/
//Method Name    : GetColorData()
//Author         : PATNI/MSN
//Purpose        : Gets the color data
//*****************************************************************************/
CColor* CPQMIPCManager::GetColorData(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetColorData");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return NULL;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    return m_pqmptr->GetColorInfo()->GetColorSetting();
}

/***************************Method Header*************************************/
//Method Name    : GetSeqParamList()
//Author         : PATNI/SS
//Purpose        : Return the g_sequence_param_name_list
//*****************************************************************************/
void* CPQMIPCManager::GetSeqParamList(
    int mode
) const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetSeqParamList");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return NULL;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    mode = m_pqmptr->GetMode();

    if (m_pqmptr->GetMode() == K_SCHEDULING) {
        return (void*)&m_pqmptr->GetSendScheduleSeqList();

    } else {
        return (void*)&m_pqmptr->GetSendAcquireSeqList();
    }
}

/***************************Method Header*************************************/
//Method Name    : GetSeqParamNameList()
//Author         : PATNI/SS
//Purpose        : Return the g_sequence_param_name_list
//*****************************************************************************/
void* CPQMIPCManager::GetSeqParamNameList() const
{
    return (PVOID) &g_sequence_param_name_list;
}

/***************************Method Header*************************************/
//Method Name    : GetMaxProtocolNumber()
//Author         : PATNI/HAR
//Purpose        : This function returns max number of protocols
//*****************************************************************************/
int CPQMIPCManager::GetMaxProtocolNumber(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetMaxProtocolNumber");
    PQM_TRACE(SCAN_OPERATION_MARKER, FUNC_NAME, _T("Trace"));
    return CPQMConfig::GetInstance()->ReadMaxProtocolLimitfromFile();
}

//********************************Method Header********************************
//Method Name   :GetSeriesCount
//Author        :PATNI/MSN
//Purpose       :returns the series count in the local DB
//*****************************************************************************
int CPQMIPCManager::GetSeriesCount(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetSeriesCount");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return 0;

    } else {
        //-Patni-MJC/2009Aug17/Modified/cpp test corrections
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetSeriesCount"));

        const int seriescount = m_pqmptr->GetDbsaStudyMgr()->GetSeriesCount();

        CString l_tracemsg(_T(""));
        l_tracemsg.Format(_T("The number of series present --> %d"), seriescount);

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_tracemsg);
        return seriescount;
    }
}

//************************************Method Header************************************
// Method Name  : GetDBDTCurrentMode
// Author       : PATNI/ NP
// Purpose      :
//*************************************************************************************
int CPQMIPCManager::GetDBDTCurrentMode(
    const int f_index,
    int* l_mode
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetDBDTCurrentMode");

    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_pqmptr pointer is NULL"));
        //Patni-NP/2009Sept22/Added/MVC#004914
        return S_FALSE;
    }

    CPqmProtocol* l_current_protocol = m_pqmptr->GetStudy()->GetProtocol(f_index);

    if (NULL == l_current_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("l_current_protocol pointer is NULL"));
        //Patni-NP/2009Sept22/Added/MVC#004914
        return S_FALSE;
    }

    //+Patni-HAR/2010May18/Modified/MVC007434
    //+Patni-PJS/2010June1/Modified + Added/MVC007434
    flt32_t l_curr_dbdt = 0.0f;

    if (E_NO_ERROR == m_pqmptr->GetStudy()->ReaddBdtOpMode(
            l_current_protocol, *l_mode, l_curr_dbdt)) {
        if (*l_mode == VFC_DBDT_IEC_OPERATING_NORMAL) {
            l_current_protocol->SetdBdtModeString(DBDT_NORMAL_LABEL);

        } else if (*l_mode == VFC_DBDT_IEC_OPERATING_1ST_CONTROLLED) {
            l_current_protocol->SetdBdtModeString(DBDT_MODE1_LABEL);

        } else if (*l_mode == VFC_DBDT_IEC_OPERATING_2ND_CONTROLLED) {
            l_current_protocol->SetdBdtModeString(DBDT_MODE2_LABEL);
        }

        //-Patni-PJS/2010June1/Modified + Added/MVC007434
        return S_OK;

    } else {
        return S_FALSE;
    }

    //    switch (l_current_protocol->GetdBdtOperation()) {
    //        case VFC_DBDT_IEC_OPERATING_1ST_CONTROLLED:
    //        case VFC_DBDT_IEC_OPERATING_2ND_CONTROLLED:
    //        case VFC_DBDT_IEC_OPERATING_NORMAL:
    //            *l_mode = l_current_protocol->GetdBdtOperation() ;
    //            break ;
    //        default:
    //            *l_mode = VFC_DBDT_IEC_OPERATING_2ND_CONTROLLED ;
    //    }
    //
    //    return S_OK;
    //-Patni-HAR/2010May18/Modified/MVC007434
}

//****************************Method Header************************************
//Module         : GetSeqDataofDisplaySequences
//Author         : PATNI/LK - JS
//Purpose        :
//*****************************************************************************
void* CPQMIPCManager::GetSeqDataofDisplaySequences(
    int f_mode,
    const int f_firstseq
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetSeqDataofDisplaySequences");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return NULL;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    f_mode = m_pqmptr->GetMode();

    SequenceList* l_display_sequences = new SequenceList;
    POSITION l_pos = NULL;

    CPASCOMSequence* l_pseq = NULL;

    if (m_pqmptr->GetMode() == K_SCHEDULING) {
        if (FALSE == m_pqmptr->GetSendScheduleSeqList().IsEmpty()) {
            l_pos = m_pqmptr->GetSendScheduleSeqList().GetHeadPosition();

            for (int i = 0; i < f_firstseq && l_pos != NULL; i++) { //Patni-AMT/2009Jun15/Modified/JFT# PQM Hang issue
                l_pseq = m_pqmptr->GetSendScheduleSeqList().GetNext(l_pos) ;
                l_display_sequences->AddTail(l_pseq);
                m_pqmptr->GetSendScheduleSeqList().RemoveAt(m_pqmptr->GetSendScheduleSeqList().Find(l_pseq));

            }
        }

    } else {
        if (FALSE == m_pqmptr->GetSendAcquireSeqList().IsEmpty()) {
            l_pos = m_pqmptr->GetSendAcquireSeqList().GetHeadPosition();

            for (int i = 0; i < f_firstseq && l_pos != NULL; i++) {
                l_pseq = m_pqmptr->GetSendAcquireSeqList().GetNext(l_pos) ;
                l_display_sequences->AddTail(l_pseq);
                m_pqmptr->GetSendAcquireSeqList().RemoveAt(m_pqmptr->GetSendAcquireSeqList().Find(l_pseq));

            }
        }
    }

    return (void*) l_display_sequences;
}

//****************************Method Header************************************
//Module         : GetMrsDefaultData
//Author         : PATNI/SS
//Purpose        :
//*****************************************************************************
int CPQMIPCManager::GetMrsDefaultData(VARIANT** f_data)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetMrsDefaultData");

    int l_result = FALSE;

    if (f_data) {
        SMRSScan_s* l_mrs_prescan_data = (SMRSScan_s*)(*f_data)->byref;

        if (m_pqmptr) {

            CScanProcedure* l_scan_proc = m_pqmptr->GetAcqManager()->GetScanProcedure();
            CPqmMRSScan* l_mrs_prescan = (CPqmMRSScan*)l_scan_proc->GetCurrentTransaction();


            if (l_mrs_prescan && l_mrs_prescan_data) {

                l_result = l_mrs_prescan->GetDefaultValue(l_mrs_prescan_data);
            }

            //+Patni-MJC/2009Aug17/Modified/cpp test corrections

        } else {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        }

        //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    }

    return l_result;
}

//****************************Method Header************************************
//Module         : PQMManualPrescanApply
//Author         : PATNI/JS
//Purpose        :
//*****************************************************************************
BOOL CPQMIPCManager::GetManualPreScanDefaultData(
    VARIANT** f_pdata
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetManualPreScanDefaultData");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    CPqmPmAcqman* l_pqm_pm_acqman = m_pqmptr->GetAcqManager();
    CScanProcedure* l_scan_proc = l_pqm_pm_acqman->GetScanProcedure();

    if (l_scan_proc->GetManualPreScanDefaultData(f_pdata) == 0) {

        return E_NO_ERROR;

    } else {
        return E_ERROR;
    }


}
//+Patni-Ajay/2010July23/Added/TMSC-REDMINE-443
/***************************Method Header*************************************/
//Method Name    : GetPresetItem()
//Author         :
//Purpose        :
//*****************************************************************************/
void CPQMIPCManager::GetPresetItem(VARIANT** pData
                                  )const
{

    CPQMConfig::GetInstance()->GetPresetItem(pData);
}
//-Patni-Ajay/2010July23/Added/TMSC-REDMINE-443

//****************************Method Header************************************
//Module         : GetCoilSelectionData
//Author         : PATNI / HAR
//Purpose        :
//*****************************************************************************
void* CPQMIPCManager::GetCoilSelectionData(
    int f_mode
) const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetCoilSelectionData");

    //Patni-KSS/2011Feb17/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(f_mode);

    //return (void*)&m_pqmptr->m_snd_schedule_seq_list;
    //CDataManager *l_data_manager = m_pqmptr->GetDataManager();
    //Dll *l_coil_selection_head = l_data_manager->GetSelectionCoilData()->GetHead();
    //+Patni-HEMANT/ADDED On 4/23/2009 12:42:40 PM/ ACE / ITC-2 Defect Fix
    //return (void *)m_pqmptr->GetDataManager()->GetSelectionCoilData()->GetHead();
    //+Patni-MJC/2009Aug17/Modified/cpp test corrections

    if (m_pqmptr) {
        CDataManager* l_data_manager = m_pqmptr->GetDataManager();

        if (l_data_manager) {
            CSelectionCoilData* l_selection_coil_data = l_data_manager->GetSelectionCoilData();

            if (l_selection_coil_data) {
                return (void*)l_selection_coil_data->GetHead();

            } else {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_selection_coil_data pointer is NULL"));
                return NULL;
            }

        } else {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_data_manager pointer is NULL"));
            return NULL ;
        }

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return NULL ;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    //-Patni-HEMANT/ADDED On 4/23/2009 12:42:40 PM/ ACE / ITC-2 Defect Fix
}

//****************************Method Header************************************
//Module         : GetConnectCoilInfo()
//Author         : PATNI / HAR
//Purpose        :
//*****************************************************************************
void* CPQMIPCManager::GetConnectCoilInfo(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetConnectCoilInfo");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqmptr) {
        CCoilSelectionTool* l_coil_selection_tool  = m_pqmptr->GetCoilSelectionTool();

        if (l_coil_selection_tool) {
            return (void*)l_coil_selection_tool->GetConnectCoilInformation();

        } else {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_coil_selection_tool pointer is NULL"));
            return NULL;
        }

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return NULL;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//****************************Method Header************************************
//Module         : GetShowCoilInfo()
//Author         : PATNI / HAR
//Purpose        :
//*****************************************************************************
void* CPQMIPCManager::GetShowCoilInfo(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetShowCoilInfo");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqmptr) {
        return (void*)m_pqmptr->GetShowCoilInfo();

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return NULL;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

void* CPQMIPCManager::GetCoordinateList(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetCoordinateList");

    if (m_pqmptr) {
        return (void*)m_pqmptr->GetCoordinateList();

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return NULL;
    }

}



//****************************Method Header************************************
//Module         : GetSelectCoilInfo()
//Author         : PATNI / HAR
//Purpose        :
//*****************************************************************************
void* CPQMIPCManager::GetSelectCoilInfo(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetSelectCoilInfo");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqmptr) {
        return (void*)(&m_pqmptr->GetSelectCoil());

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return NULL;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}


void CPQMIPCManager::GetCoilNameFromAcqOrder(
    const int f_acq_order,
    VARIANT**  coilnamefrom
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetCoilNameFromAcqOrder");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == coilnamefrom) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("coilnamefrom pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqmptr) {
        CString* l_string_value = (CString*)(*coilnamefrom)->byref;
        CPQMStudy* pqm_study = m_pqmptr->GetStudy();

        if (pqm_study) {
            *l_string_value = pqm_study->GetCoilNameFromAcqOrder(f_acq_order);

        } else {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("pqm_study pointer is NULL"));
        }

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//Nitin coil
void CPQMIPCManager::GetCoilNameFromTransmiter(
    const int f_acq_order,
    VARIANT**  coilnamefrom
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetCoilNameFromTransmiter");

    if (NULL == coilnamefrom) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("coilnamefrom pointer is NULL"));
        return;
    }

    CString* l_string_value = (CString*)(*coilnamefrom)->byref;
    *l_string_value = _T("");

    CPQMStudy* pqm_study = m_pqmptr->GetStudy();

    if (pqm_study) {
        CPqmProtocol* l_obj = pqm_study->GetProtocol(f_acq_order);

        if (l_obj) {

            if (l_obj->GetCoilDBTable() != NULL) {
                PqmCoil_t*	l_tmp_coil = NULL ;

                if (pqm_study->GetTxCoilInSaturn(l_obj->GetCoilDBTable(), &l_tmp_coil) != 0) {
                    *l_string_value = l_tmp_coil->label;
                }

            }
        }

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("pqm_study pointer is NULL"));
    }


}

//Nitin coil
void CPQMIPCManager::GetCoilNameFromReceiver(
    const int f_acq_order,
    VARIANT**  coilnamefrom
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetCoilNameFromReceiver");

    if (NULL == coilnamefrom) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("coilnamefrom pointer is NULL"));
        return;
    }

    CString* l_string_value = (CString*)(*coilnamefrom)->byref;
    *l_string_value = _T("");

    CPQMStudy* pqm_study = m_pqmptr->GetStudy();

    if (pqm_study) {
        *l_string_value = pqm_study->GetReceiverCoilName(f_acq_order);
    }


}


void CPQMIPCManager::GetInsideLimit(const int f_index,
                                    int& f_in_side_limit,
                                    int& f_out_side_limit,
                                    BOOL& enable)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetInsideLimit");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqmptr) {
        m_pqmptr->GetInsideLimit(f_index, f_in_side_limit, f_out_side_limit, enable);

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}


//****************************Method Header************************************
//Module         : GetConnectedCoilListForDisplay()
//Author         : PATNI/AMT
//Purpose        : This function gets connected coil list which is required to
//                  display at GUI
//*****************************************************************************
void* CPQMIPCManager::GetConnectedCoilListForDisplay(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetConnectedCoilListForDisplay");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqmptr) {
        return (void*)m_pqmptr->GetConnectedCoilList();

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return NULL;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

void CPQMIPCManager::GetConnectedCoilInformationForGUIDisplay(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetConnectedCoilInformationForGUIDisplay");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqmptr) {
        m_pqmptr->SetGUICoilInformationDisplay(true);

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

/****************************Method Header************************************
//Method Name   :GetControlValueFromDB()
//Author        :PATNI/SG
//Purpose       :For Get controls values From DB
//*****************************************************************************/
BOOL CPQMIPCManager::GetControlValueFromDB(
    VARIANT* f_pdata,
    const int f_scan_index
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetControlValueFromDB");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return false;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    return  m_pqmptr->GetStudy()->GetControlValueFromStudy(f_pdata, f_scan_index);

}

//****************************Method Header************************************
//Module         : GetCurrentWSEEditProt()
//Author         : PATNI/SS
//Purpose        :
//*****************************************************************************
int CPQMIPCManager::GetCurrentWSEEditProt(VARIANT* f_data)const
{
    if (m_pqmptr) {
        if (m_pqmptr->GetIsProconConnected() && m_pqmptr->GetPqmProCon()) {
            if (f_data) {
                f_data->intVal = m_pqmptr->GetPqmProCon()->GetCurrentEditProt();
                return TRUE;
            }
        }
    }

    return FALSE;

}

//****************************Method Header************************************
//Module         : GetGateInfoList()
//Author         : PATNI/MRP
//Purpose        :
//*****************************************************************************
void CPQMIPCManager::GetGateInfoList(
    VARIANT* pdata
)const
{
    if (m_pqmptr) {
        if (pdata) {
            pdata->byref = m_pqmptr->GetGateInfoList();
        }
    }
}

// + SM4 Defect#737 Himanshu 20090130
//****************************Method Header************************************
//Module         : GetSequenceInformation()
//Author         : PATNI / HAR
//Purpose        :
//*****************************************************************************
void* CPQMIPCManager::GetSequenceInformation(
    VARIANT* pData,
    const int f_acq_order
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetSequenceInformation");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMIPCManager::GetSequenceInformation"));

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqmptr) {
        return (void*)m_pqmptr->GetStudy()->GetPascomSequenceObject(pData, f_acq_order);

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return NULL;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}
// + SM4 Defect#737 Himanshu 20090130


//********************************Method Header********************************
//Method Name   : GetCurrentSarInfoToDisplay()
//Author        : PATNI/PJS
//Purpose       : Gets Current Scan Info
//*****************************************************************************
BOOL CPQMIPCManager ::GetCurrentSarInfoToDisplay(
    VARIANT** pdata)const//Patni-HAR/2010Jun23/Modified/MVC008324_2
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetCurrentSarInfoToDisplay");

    if (NULL == pdata) {
        return false;
    }


    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqmptr) {
        //+Patni-HAR/2010Jun23/Modified/MVC008324_2
        CString* l_string_value = (CString*)(*pdata)->byref;
        *l_string_value = m_pqmptr->GetCurrentSarInfo();
        //-Patni-HAR/2010Jun23/Modified/MVC008324_2
        return true;

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return false;
    }
}

void CPQMIPCManager::GetNewSeriesLoids(
    VARIANT* f_pdata
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetNewSeriesLoids");

    if (f_pdata) {
        //+Patni-MJC/2009Aug17/Modified/cpp test corrections
        if (m_pqmptr) {
            if (m_pqmptr->GetDbsaStudyMgr()) {
                f_pdata->byref =  m_pqmptr->GetDbsaStudyMgr()->GetNewSeriesLoids();

            } else {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("m_pqmptr->m_dbsa_study_mgr pointer is NULL"));
                return;
            }

        } else {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
            return;
        }

        //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    }
}

//********************************Method Header********************************
//Method Name   : GetTopMostWaitAcqOrder()
//Author        : PATNI/ARD
//Purpose       : Get Top most wait protocol acqorder.
//*****************************************************************************
void CPQMIPCManager::GetTopMostWaitAcqOrder(int* f_ipc_top_wait)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetTopMostWaitAcqOrder");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqmptr) {
        CPqmProtocol* l_top_most_wait = NULL;
        m_pqmptr->GetStudy()->GetTopWaitPos(&l_top_most_wait);

        if (l_top_most_wait != NULL) {
            *f_ipc_top_wait = l_top_most_wait->GetProtocol();
        }

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
    }
} //-Patni-ARD/2009Jun17/Added/JFT# AcqOrder Checking between GUI and IPC

//********************************Method Header********************************
//Method Name   : GetHasParent()
//Author        : PATNI/ARD
//Purpose       :
//*****************************************************************************
int CPQMIPCManager::GetHasParent(const int f_acq_order)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetHasParent");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqmptr) {
        return m_pqmptr->GetHasParent(f_acq_order);

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return 0;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}


//********************************Method Header********************************
//Method Name   : GetStudyPath()
//Author        : PATNI/HAR
//Purpose       :
//*****************************************************************************
HRESULT CPQMIPCManager::GetStudyPath(
    VARIANT* f_study_path
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetStudyPath");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqmptr) {
        CPQMStudy* l_pqm_study = m_pqmptr->GetStudy();

        if (l_pqm_study) {
            f_study_path->bstrVal = l_pqm_study->GetStudyPath().AllocSysString();

            if (f_study_path) {
                return S_OK;

            } else {
                return S_FALSE;
            }

        } else {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqm_study pointer is NULL"));
            return S_FALSE;
        }

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return S_FALSE;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//********************************Method Header********************************
//Method Name   : GetSarInfoPtr()
//Author        : PATNI/PJS
//Purpose       : Gets Scan Info ptr
//*****************************************************************************
HRESULT CPQMIPCManager ::GetSarInfoPtr(
    VARIANT* pdata)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetSarInfoPtr");

    if (NULL == pdata) {
        return false;
    }

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqmptr) {
        sarInfo_t* l_ptr_sarInfo = m_pqmptr->GetSarInfoPtr();

        if (l_ptr_sarInfo) {
            pdata->byref =  l_ptr_sarInfo;
            return S_OK;
        }

        return S_FALSE;

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return S_FALSE;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//********************************Method Header********************************
//Method Name   : GetSarInfoPtrForIndex()
//Author        : iGate
//Purpose       : Gets SAR Info ptr
//*****************************************************************************
HRESULT CPQMIPCManager::GetSarInfoPtrForIndex(VARIANT* f_pdata)const
{
    if (f_pdata) {
        f_pdata->byref = m_pqmptr->GetSarInfoPtrForIndex(f_pdata->intVal);
        return S_OK;
    }

    return S_FALSE;
}


//+ Patni-PJS/2009Jul29/Modified/MVC0003961
//************************************Method Header************************************
// Method Name  : GetCouchPos
// Author       : PATNI/ PJS
// Purpose      : Get Couch pos
//*************************************************************************************
void CPQMIPCManager::GetCouchPos(VARIANT* f_pData)const
{
    if (m_pqmptr) {
        f_pData->fltVal = m_pqmptr->GetCouchPos();
    }
}
//- Patni-PJS/2009Jul29/Modified/MVC0003961

//+Patni-Manishkumar/2009Jul29/Modified/MVC003316
//************************************Method Header************************************
// Method Name  : GetTotalSequence
// Author       : PATNI/ Manish
// Purpose      : Gets Total Sequence
//*************************************************************************************
void CPQMIPCManager::GetTotalSequence(VARIANT* f_pVariant)const
{
    if (m_pqmptr) {
        //Patni-PJS/2009Dec27/Modified+Added/MVC006364 Code review
        //long l_couch_pos = m_pqmptr->GetTotalSequence();
        f_pVariant->lVal = m_pqmptr->GetNoOfSequencesTransferred();
    }
}
//-Patni-Manishkumar/2009Jul29/Modified/MVC003316


//************************************Method Header************************************
// Method Name  : GetGUIListProtocolInfo
// Author       : PATNI/ ARD
// Purpose      :
//*************************************************************************************
bool CPQMIPCManager::GetGUIListProtocolInfo(
    VARIANT* f_pdata,
    const int f_count_gui_list
)const
{

    if (m_pqmptr) {
        return m_pqmptr->GetStudy()->CheckAllForSequenseParam(f_pdata, f_count_gui_list);
    }

    return false;
}

/****************************Method Header************************************
//Method Name   :GetPqmPqIsGlocOk()
//Author        :PATNI/Manishkumar
//Purpose       :Check the status of protocol and enable locator button
//*****************************************************************************/
BOOL CPQMIPCManager::GetPqmPqIsGlocOk()const
{

    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetPqmPqIsGlocOk");

    CPqmProcon* l_pPqmProcon = m_pqmptr->GetPqmProCon();

    if (NULL == l_pPqmProcon) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("CPqmProcon object NULLL"));
        return FALSE;
    }

    CPQMStudy* l_pqm_study  = m_pqmptr->GetStudy();

    if (NULL == l_pqm_study) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("CPQMStudy object NULLL"));
        return FALSE;
    }

    CPqmProtocol* obj = NULL;

    if (l_pPqmProcon->PqmPqIsGlocOk(l_pqm_study->GetTopWaitPos(&obj), true)) {
        m_pqmptr->WriteToUI(PQM_MSG_ON_FLAG_LOCATOR);
        return TRUE;
    }

    return TRUE;
}

/***************************Method Header*************************************/
//Method Name    : GetCoilSysSetting()
//Author         : PATNI/AMT
//Purpose        : Get system setting for coil
//*****************************************************************************/
bool CPQMIPCManager::GetCoilSysSetting(VARIANT* f_data)const
{
    if (m_pqmptr) {
        coil_sys_info_t l_coil_sys_info;
        l_coil_sys_info.VAP_channels = m_pqmptr->GetVAPChannels();
        l_coil_sys_info.channelPerVFEP = m_pqmptr->GetChannelPerVFEP();
        l_coil_sys_info.sft_mode = m_pqmptr->GetSFTMode();

        f_data->byref = &l_coil_sys_info;

    } else {
        return false;
    }

    return true;
}

//+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
BOOL CPQMIPCManager::GetSFTMenuXMLData(
    VARIANT** pData
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetSFTMenuXMLData");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetSFTMenuXMLData"));

    return CPQMConfig::GetInstance()->GetSFTMenuXMLData(pData);
}

BOOL CPQMIPCManager::GetToolMenuXMLData(
    VARIANT** pData
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetToolMenuXMLData");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetToolMenuXMLData"));

    return CPQMConfig::GetInstance()->GetToolMenuXMLData(pData);
}

void CPQMIPCManager::GetSFTParamValue(
    VARIANT** f_param, const int f_acq_order
)const
{
    m_pqmptr->GetSFTParamValue(f_param, f_acq_order);
}
//-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu

//+Patni-DH+ARD/2009Dec03/Added/DeFT# MVC005787+MSA0248-00087
/***************************Method Header*************************************/
//Method Name    : GetPatientScanoffset()
//Author         : PATNI/DH+ARD
//Purpose        : Returns the converted scan offset values for display
//*****************************************************************************/
BOOL CPQMIPCManager::GetPatientScanoffset(const int f_acq_order, VARIANT**  f_scanoffset)const
{
    CPQMStudy* l_study = m_pqmptr->GetStudy();

    if (NULL == l_study) {
        return FALSE;
    }

    CPqmProtocol* l_objProtocol = l_study->GetProtocol(f_acq_order);

    if (NULL == l_objProtocol) {
        return FALSE;
    }

    if (FALSE == l_study->PqmUiSetScanOffset(l_objProtocol, f_scanoffset)) {
        return FALSE ;
    }

    return TRUE;
}
//-Patni-DH+ARD/2009Dec03/Added/DeFT# MVC005787+MSA0248-00087


//+Patni-PJS/2010Mar11/Added/DeFT# MCM0229-00178
//**************************************************************************
//Method Name   :  GetScanTimeForCurrProt()
//Author        :  PATNI/PJS
//Purpose       :
//**************************************************************************
BOOL CPQMIPCManager::GetScanTimeForCurrProt(
    VARIANT* f_pdata
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetScanTimeForCurrProt");

    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME ,
                  _T("m_pqmptr pointer is NULL"));
        return FALSE;
    }

    CPqmPmAcqman* l_pqm_pm_acqman = m_pqmptr->GetAcqManager();

    if (NULL == l_pqm_pm_acqman) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("l_pqm_pm_acqman pointer is NULL"));
        return FALSE;
    }

    CScanProcedure* l_scan_proc = l_pqm_pm_acqman->GetScanProcedure();

    if (NULL == l_scan_proc) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("l_scan_proc pointer is NULL"));
        return FALSE;
    }

    CPqmProtocol* l_curr_protocol = l_scan_proc->GetScanProcProtocol();

    if (l_curr_protocol) {
        f_pdata->llVal = l_curr_protocol->GetScanTime();
        return TRUE;

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("l_curr_protocol pointer is NULL"));
        return FALSE;
    }
}
//-Patni-PJS/2010Mar11/Added/DeFT# MCM0229-00178

//+Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : GetMagneticCenterPosForCoilSel
// Author       : PATNI/ HEMANT
// Purpose      : Returns the center of magnetic field position wrt to GUI dialog
//***********************************************************************************
void CPQMIPCManager::GetMagneticCenterPosForCoilSel(
    VARIANT**  f_center_pos
)const  //Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_03
{
    if (m_pqmptr) {
        m_pqmptr->GetMagneticCenterPosForCoilSel(f_center_pos);
    }
}

//************************************Method Header************************************
// Method Name  : GetCDSDebugParam
// Author       : PATNI/ HEMANT
// Purpose      : Returns the coil location information for CDS protocol in scanning
//***********************************************************************************
void CPQMIPCManager::GetCDSDebugParam(
    VARIANT**  f_compass_debug_param
) const  	//Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_03
{
    if (m_pqmptr) {
        m_pqmptr->GetCDSDebugParam(f_compass_debug_param);
    }
}
//-Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement

//************************************Method Header************************************
// Method Name  : GetASCompassFlag
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
BOOL CPQMIPCManager::GetASCompassFlag(
    int f_acqorder,
    BOOL* f_compass_flag
) const  //Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_18
{
    if (m_pqmptr) {
        return m_pqmptr->GetASCompassFlag(f_acqorder, f_compass_flag);
    }

    return FALSE;
}

//*****************************************************************************
//Method Name   : GetCouchMethodForProt
//Author        : Patni/PJS
//Purpose       : This function is used to get the couch method for the given
//                protocol.
//*****************************************************************************
HRESULT CPQMIPCManager::GetCouchMethodForProt(
    const int f_acq_order, VARIANT* f_pdata
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetCouchMethodForProt");

    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_pqmptr pointer is NULL"));
        return S_FALSE;
    }

    if (NULL == f_pdata) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("f_pdata pointer is NULL"));
        return S_FALSE;
    }

    CPqmProtocol* l_prot = m_pqmptr->GetStudy()->GetProtocol(f_acq_order);

    if (NULL == l_prot) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("l_prot pointer is NULL"));
        return S_FALSE;
    }

    f_pdata->intVal = l_prot->GetcouchMethodFlg();
    return S_OK;
}

//****************************Method Header************************************
//Module         : GetAutoInsertedSeqDataToDisplay
//Author         : PATNI/DKH
//Purpose        :
//*****************************************************************************
BOOL CPQMIPCManager::GetAutoInsertedSeqDataToDisplay(
    VARIANT** f_autinsert_data,
    int* f_insertedmappos,
    int* f_target_pos,
    int* const f_is_shim_required,
    int* const f_is_map_required
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetAutoInsertedSeqDataToDisplay");
    PQM_SCOPED_TRACE();

    CAutoInsertProcedure* l_auto_insert_proc = m_pqmptr->GetAutoInsertProc();

    return l_auto_insert_proc->GetAutoInsertedSeqDataToDisplay(
               f_autinsert_data,
               f_insertedmappos,
               f_target_pos,
               f_is_shim_required,
               f_is_map_required);
}

void CPQMIPCManager::GetTargetProtInformation(
    int* f_target_pos
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetAutoInsertedSeqDataToDisplay");
    PQM_SCOPED_TRACE();

    CAutoInsertProcedure* l_auto_insert_proc = m_pqmptr->GetAutoInsertProc();
    l_auto_insert_proc->GetTargetProtInformation(f_target_pos);
}

//+Patni-AJS/2010Sep3/Added/ScanCommentDialog/IR-163
/***************************Method Header*************************************/
//Method Name    : GetScanComment()
//Author         : AJS
//Purpose        :
//*****************************************************************************/
void CPQMIPCManager::GetScanCommentXMLData(VARIANT** pData
                                          )const
{
    CPQMConfig::GetInstance()->GetScanCommentXMLData(pData);
}
//-Patni-AJS/2010Sep3/Added/ScanCommentDialog/IR-163

//+Patni-Abhishek/2010Dec18/Added/Redmine-1074_part1
/***************************Method Header*************************************/
//Method Name    : GetScanAgent()
//Author         : Abhishek
//Purpose        : Redmine-1074
//*****************************************************************************/
bool CPQMIPCManager::GetScanAgent(const int f_protocol_index, VARIANT* f_scan_agent)
{

    CPQMStudy* l_pqm_study = m_pqmptr->GetStudy();
    CString l_scan_agent = _T("");

    if (l_pqm_study) {

        if (E_NO_ERROR == l_pqm_study->GetScanAgent(f_protocol_index, l_scan_agent)) {
            f_scan_agent->byref = (&l_scan_agent);
            return true;
        }
    }

    return false;
}
//-Patni-Abhishek/2010Dec18/Added/Redmine-1074_part1

//+Patni-Sudhir/2011Jan13/Added/Redmine-434
/***************************Method Header*************************************/
//Method Name    : GetNRFilterStringForProtocol()
//Author         : Sudhir
//Purpose        : To get the Filter ID for the selected protocol
//*****************************************************************************/
bool CPQMIPCManager::GetNRFilterStringForProtocol(const int f_acq_order, const int f_NRFLG, VARIANT* f_NRVal)const
{

    CPQMStudy* l_pqm_study = m_pqmptr->GetStudy();

    if (l_pqm_study) {

        if (E_NO_ERROR == l_pqm_study->GetNRFilterStringForProtocol(f_acq_order, f_NRFLG, f_NRVal)) {
            return true;
        }
    }

    return false;
}
//-Patni-Sudhir/2011Jan13/Added/Redmine-434

/***************************Method Header*************************************/
//Method Name    : GetEMTFilterStringForProtocol()
//Author         :
//Purpose        :
//*****************************************************************************/
bool CPQMIPCManager::GetEMTFilterStringForProtocol(const int f_acq_order, VARIANT* f_EMTVal)const
{

    CPQMStudy* l_pqm_study = m_pqmptr->GetStudy();

    if (l_pqm_study) {

        if (E_NO_ERROR == l_pqm_study->GetEMTFilterStringForProtocol(f_acq_order, f_EMTVal)) {
            return true;
        }
    }

    return false;
}
/***************************Method Header******************************************/
//Method Name    : GetMRSLicense()
//Author         : PJS
//Purpose        : This function returns the status of the presence of MRS License
//*********************************************************************************/
BOOL CPQMIPCManager::GetMRSLicense(
)
{
    if (!m_pqmptr) {
        return FALSE;
    }

    return m_pqmptr->GetMrsLicense();
}

BOOL CPQMIPCManager::GetMRS_BRST_License(
)
{
    if (!m_pqmptr) {
        return FALSE;
    }

    return m_pqmptr->GetMRS_BRST_License();
}

/***************************Method Header******************************************/
//Method Name    : GetThreadEvent()
//Author         : PATNI/Sribanta
//Purpose        : This function returns Thread Handle.
//*********************************************************************************/
HANDLE& CPQMIPCManager::GetThreadEvent()
{
    return m_ThreadEvent;
}


BOOL CPQMIPCManager::GetPostProcessFlag(
) const
{
    if (!m_pqmptr) {
        return FALSE;
    }

    return m_pqmptr->GetPostProcessFlag();
}

//****************************Method Header************************************
//Module         : GetShimState
//Author         : PATNI/Sribanta
//Purpose        :
//*****************************************************************************
void CPQMIPCManager::GetShimState(int* f_shim_state)
{
    CPqmShimSharedData::GetInstance()->GetShimState(f_shim_state);
}

/***************************Method Header*************************************/
//Method Name    : GetScanAnatomy()
//Author         : PATNI/Mangesh
//Purpose        : This function returns the CScanAnatomyData object
//*****************************************************************************/
void CPQMIPCManager::GetScanAnatomy(VARIANT** pData
                                   )const
{

    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetScanAnatomy");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetScanAnatomy"));

    CPQMConfig::GetInstance()->GetScanAnatomy(pData);
}
//SM3_Mangesh_End


//****************************Method Header************************************
//Method Name    : SetScanAnatomy()
//Author         : PATNI/HAR-LK
//Purpose        :
//*****************************************************************************
void CPQMIPCManager::SetScanAnatomy(
    const int f_anatomy_id
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SetScanAnatomy");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return;

    } else {
        //-Patni-MJC/2009Aug17/Modified/cpp test corrections
        CString l_tracemsg(_T(""));
        l_tracemsg.Format(_T("Trace of  SetScanAnatomy and scan anatomy  is --> %d"), f_anatomy_id);

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_tracemsg);

        m_pqmptr->GetStudy()->SetScanAnatomy(f_anatomy_id);
    }
}

//+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : GetCDSDebugParam
// Author       : PATNI/ HEMANT
// Purpose      : Returns the coil location information for CDS protocol in scanning
//***********************************************************************************
void CPQMIPCManager::GetASCOMPASSParameter(
    VARIANT**  f_cdscompasssetting_param
) const  	//Patni-Hemant/2010May6/Added/JaFT/SM_PH3_CDR-CDS_CDR_18
{
    if (m_pqmptr) {
        m_pqmptr->GetASCOMPASSParameter(f_cdscompasssetting_param);

    } else {
        ASSERT(FALSE) ;
    }
}
//-Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement


//+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : GetCDSDebugParam
// Author       : PATNI/ HEMANT
// Purpose      : Returns the coil location information for CDS protocol in scanning
//***********************************************************************************
void CPQMIPCManager::SetASCOMPASSParameter(
    VARIANT**  f_cdscompasssetting_param
)
{

    if (m_pqmptr) {
        m_pqmptr->SetASCOMPASSParameter(f_cdscompasssetting_param);

    } else {

        ASSERT(FALSE) ;
    }
}
//-Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement

//****************************Method Header************************************
//Module         : GetIsAcqmanConnected
//Author         : PATNI/JS
//Purpose        : Getter method to check acqman is connected or not.
//*****************************************************************************
bool CPQMIPCManager::GetIsAcqmanConnected(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetIsAcqmanConnected");
    CString l_tracemsg(_T(""));

    if (m_is_acqman_connected) {
        l_tracemsg.Format(_T("acqman is connected"));

    } else {
        l_tracemsg.Format(_T("acqman is not connected"));
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_tracemsg);
    return m_is_acqman_connected;
}

//****************************Method Header************************************
//Module         : SetIsAcqmanConnected
//Author         : PATNI/JS
//Purpose        : Setter method to check acqman is connected or not.
//*****************************************************************************
void CPQMIPCManager::SetIsAcqmanConnected(
    const bool f_is_acqman_connected
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SetIsAcqmanConnected");
    CString l_tracemsg(_T(""));

    if (f_is_acqman_connected) {
        l_tracemsg.Format(_T("acqman is connected"));
        //	m_pqmptr->WriteToUI(PQM_MSG_SCANSTART_ENABLE);		//Ashish SM3

    } else {
        l_tracemsg.Format(_T("acqman is not connected"));
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_tracemsg);

    m_is_acqman_connected = f_is_acqman_connected;
}

//****************************Method Header************************************
//Module         : IsWireLessSystem
//Author         :
//Purpose        :
//*****************************************************************************
BOOL CPQMIPCManager::IsWireLessSystem()const
{

    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetGateMode");

    if ((NULL != m_pqmptr) && m_pqmptr->IsWireLessSystem()) {

        return TRUE;
    }

    return FALSE;

}

//+Patni-PJS/2011Feb21/Added/IR-97
void CPQMIPCManager::SetIsValidWFDAScan(
    BOOL f_is_valid_wfda_scan
)
{
    CPQMStudy* l_pqm_study = m_pqmptr->GetStudy();

    if (NULL == l_pqm_study) {
        return;
    }

    l_pqm_study->SetIsValidWFDAScan(f_is_valid_wfda_scan);
}
//-Patni-PJS/2011Feb21/Added/IR-97

//****************************Method Header************************************
//Method Name    : SetPrescanDone()
//Author         : PATNI/HAR - SS
//Purpose        :
//*****************************************************************************
void CPQMIPCManager::SetPrescanDone(
    const BOOL prescan_done
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SetPrescanDone");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetPrescanDone"));

    m_prescan_done = prescan_done;
}

/***************************Method Header*************************************/
//Method Name    : SetSeriesOrder()
//Author         : PATNI/PJP
//Purpose        : Set the index to series order
//*****************************************************************************/
void CPQMIPCManager::SetSeriesOrder(
    BSTR bStr,
    const int dropindex
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SetSeriesOrder");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    //+Patni-NP/2009Mar09/Added/PSP1 Changes For VF Lock
    //    m_pqmptr->WriteToUI(PQM_MSG_BEGINWAIT_CURSOR);
    //    PostThreadMessage(CPQMIPCManager::dThreadId,MSG_REARRANG,WPARAM(bStr),LPARAM(dropindex));
    //    WaitForSingleObject(m_ThreadEvent,INFINITE);

    m_pqmptr->GetDbsaStudyMgr()->SetSeriesOrder(bStr, dropindex);

    //  m_pqmptr->WriteToUI(PQM_MSG_ENDWAIT_CURSOR);
    //+Patni-NP/2009Mar09/Added/PSP1 Changes For VF Lock
}

//****************************Method Header************************************
//Method Name    : SetAutoVoiceFlag()
//Author         : PATNI/
//Purpose        :
//*****************************************************************************
void CPQMIPCManager::SetAutoVoiceFlag(
    const BOOL auto_voice
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SetAutoVoiceFlag");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return;

    } else {
        // Set the auto voice
        m_pqmptr->SetAutoVoice(auto_voice);
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//*******************************Method Header*********************************
//Method Name    :SetAPCHoldValue()
//Author         :PATNI/MSN
//Purpose        :Sets the APC Hold Value on pressing APC Pause menu
//*****************************************************************************
void CPQMIPCManager::SetAPCHoldValue(
    const int apcholdvalue
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SetAPCHoldValue");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    CString l_tracemsg(_T(""));
    l_tracemsg.Format(_T("Trace of SetAPCHoldValue and the apcholdvalue is --> %d"), apcholdvalue);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_tracemsg);

    CPqmPmAcqman* acqman = m_pqmptr->GetAcqManager();

    if (NULL == acqman) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("acqman is NULL"));
        return;
    }

    CScanProcedure* scanprocedure = acqman->GetScanProcedure();

    if (NULL == scanprocedure) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("scanprocedure is NULL"));
        return;
    }

    scanprocedure->SetAPCHold(apcholdvalue);
}

//****************************Method Header************************************
//Method Name    : SetReceiverCoilID()
//Author         : PATNI/HAR-LK
//Purpose        :
//*****************************************************************************
void CPQMIPCManager::SetReceiverCoilID(
    const long f_receiver_coilid
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SetReceiverCoilID");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return;

    } else {
        //-Patni-MJC/2009Aug17/Modified/cpp test corrections

        m_pqmptr->GetStudy()->SetReceiverCoilID(f_receiver_coilid);
        m_pqmptr->SetSelectedCoilKey(f_receiver_coilid);
    }
}


//****************************Method Header************************************
//Module         : SetExeAutoVoice
//Author         : PATNI/Ekta
//Purpose        : sets auto voice setting
//*****************************************************************************
void  CPQMIPCManager::SetExeAutoVoice(
    const int l_exe_auto_voice
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SetExeAutoVoice");
    //    CPQMLogMgr::GetInstance()->WriteTraceLog(DOMAIN_NAME,
    //        USER_FUNC_MARKER,
    //        _T("CPQMIPCManager::SetExeAutoVoice"),
    //        _T("CPQMIPCManager::SetExeAutoVoice"));

    CString l_exe_str(_T(""));
    l_exe_str.Format(_T("%d"), l_exe_auto_voice);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("ExeAutoVoice --> ") + l_exe_str);

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    CScanProcedure* l_scan_proc_ptr = m_pqmptr->GetAcqManager()->GetScanProcedure();
    CPqmProtocol* protocol = NULL;
    CPQMStudy* l_pqm_study = m_pqmptr->GetStudy();

    if (l_pqm_study->GetTopWaitPos(&protocol) < 0 || protocol == NULL) {
        //+Patni-ARD/2009-Nov-27/Commented/DeFT# MVC003149 + MVC005179
        /*
        CPQMLogMgr::GetInstance()->WriteEventLog(
            _T("No protocol in study file with wait status !"),
            ERROR_LOG_INFORMATION1 , _T("PQMIPCManager.cpp"));
        	*/
        //-Patni-ARD/2009-Nov-27/Commented/DeFT# MVC003149 + MVC005179
        //Removal of Messagebox
        //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
        CPQMLogMgr::GetInstance()->DisplayScanError(IDS_NO_WAIT_PROTOCOLS, _T("IDS_NO_WAIT_PROTOCOLS"),
                SCAN_ERR_NOTIFY, L"PqmIpcManager.cpp");

    } else {
        l_scan_proc_ptr->SetScanProcProtocol(protocol);

        //if (l_scan_proc_ptr->GetIsScanning()){
        BITFLD_DECL(mask, K_ALL_PR) = {0}; //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
        BITFLD_INIT(mask, K_ALL_PR, 0);
        protocol->SetExeAutoVoice(l_exe_auto_voice);

        l_pqm_study->SetProtocolValues(l_pqm_study->GetVfDBHandle(),
                                       protocol->GetProtocol(),
                                       mask,
                                       K_PR_EXE_AUTO_VOICE,
                                       &l_exe_auto_voice,
                                       sizeof(int),
                                       NULL);
    }
}

//********************************Method Header********************************
//Method Name   :SetPQMMode
//Author        :PATNI/MSN
//Purpose       :Sets the PQM Mode (Schedule/Acquire)
//*****************************************************************************
void CPQMIPCManager::SetPQMMode(const int f_mode)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SetPQMMode");

    CPQMLogMgr::GetInstance()->SetModeString(f_mode);

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return;

    } else {
        m_pqmptr->SetMode(f_mode);
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//****************************Method Header************************************
//Module         : SetShimmWBCoil
//Author         : PATNI/LK
//Purpose        : sets Shimming Scan WB menu item status
//*****************************************************************************
void CPQMIPCManager::SetShimmWBCoil(
    const BOOL f_shimscan_usewb
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SetShimmWBCoil");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetShimmWBCoil"));

    m_pqmptr->SetShimmWBCoil(f_shimscan_usewb);
}

//****************************Method Header************************************
//Module         : SetShimFsyFlag
//Author         : PATNI/LK
//Purpose        : sets FSY flag of shared memory of PQM as per user action on CFA
//                  dialog
//*****************************************************************************
void CPQMIPCManager::SetShimFsyFlag(
    const int f_shimfsy_flag
)
{

    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SetShimFsyFlag");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetShimFsyFlag"));

    CPqmShimSharedData::GetInstance()->SetShimFsyFlag(f_shimfsy_flag);

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return ;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    m_pqmptr->GetStudy()->WriteToShimData();

}


//****************************Method Header************************************
//Method         : SetPrescanHook
//Author         : PATNI/JS
//Purpose        :
//*****************************************************************************
void CPQMIPCManager::SetPrescanHook(
    const BOOL f_prescan_hook
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SetPrescanHook");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return ;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    CPqmPmAcqman* pqm_pm_acqman = m_pqmptr->GetAcqManager();
    CScanProcedure* l_scan_proc = pqm_pm_acqman->GetScanProcedure();
    l_scan_proc->SetPrescanHook(f_prescan_hook);

}

//****************************Method Header************************************
//Module         : SetAutoPlan
//Author         : PATNI/SS
//Purpose        :
//
//*****************************************************************************
int CPQMIPCManager::SetAutoPlan(
    const int f_auto_plan
)const
{

    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SetAutoPlan");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqmptr) {
        m_pqmptr->SetAutoPlan(f_auto_plan);
        return  TRUE;

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    return FALSE;
}

//****************************Method Header************************************
//Module         : SetPlanAndScan
//Author         : PATNI/SS
//Purpose        :
//
//*****************************************************************************
int CPQMIPCManager::SetPlanAndScan(
    const int f_plan_scan
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SetPlanAndScan");

    if (m_pqmptr) {
        m_pqmptr->SetPlanAndScan(f_plan_scan);
        return TRUE;

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return FALSE;
    }
}

//****************************Method Header************************************
//Module         : SetSkipRgn
//Author         : PATNI/SS
//Purpose        :
//
//*****************************************************************************
int CPQMIPCManager::SetSkipRgn(
    const int f_skip_rgn
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SetSkipRgn");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqmptr) {
        //+ Patni-MP/2009Mar11/Modified/Warning Removal
        if (f_skip_rgn <= 0) {
            m_pqmptr->SetSkipRgn(false);

        } else {
            m_pqmptr->SetSkipRgn(true);
        }

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    //m_pqmptr->SetSkipRgn((bool)f_skip_rgn);
    //int l_result = FALSE;

    //- Patni-MP/2009Mar11/Modified/Warning Removal
    return f_skip_rgn;
}

//****************************Method Header************************************
//Module         : SetAllCoverageRgn
//Author         : PATNI/SS
//Purpose        :
//
//*****************************************************************************
int CPQMIPCManager::SetAllCoverageRgn(
    const int f_all_cov_rgn
)const
{

    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SetAllCoverageRgn");

    /*m_pqmptr->SetAllCoverageRgn((bool)f_all_cov_rgn);
    int l_result = FALSE;
    return l_result;*/
    //+ Patni-MP/2009Mar11/Modified/Warning Removal
    //MRP for removing warnings
    if (m_pqmptr) {
        if (f_all_cov_rgn <= 0) {
            m_pqmptr->SetAllCoverageRgn(false);

        } else {
            m_pqmptr->SetAllCoverageRgn(true);
        }

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
    }

    //- Patni-MP/2009Mar11/Modified/Warning Removal

    return f_all_cov_rgn;
}

//****************************Method Header************************************
//Module         : SetPrepWbCoil
//Author         : PATNI/SS
//Purpose        :
//
//*****************************************************************************
int CPQMIPCManager::SetPrepWbCoil(
    const int f_prep_wb_coil
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SetPrepWbCoil");
    //+Patni-MJC/2009Aug17/Modified/cpp test corrections

    if (m_pqmptr) {
        m_pqmptr->SetPrepWbCoil(f_prep_wb_coil);
        return TRUE;

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqm_ptr pointer is NULL"));
        return FALSE;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}


//****************************Method Header************************************
//Module         : SetActiveDialog
//Author         : PATNI/ASHISH SM3
//Purpose        :
//*****************************************************************************
void CPQMIPCManager::SetActiveDialog(
    const int f_dialog_type, const BOOL f_bSet
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SetActiveDialog");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqmptr) {
        m_pqmptr->SetActiveDialog(f_dialog_type, f_bSet);

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

bool CPQMIPCManager::SetScanOffsetValue(const int index, const float f_scanoffset_x, const float f_scanoffset_y, const float f_scanoffset_z, const BOOL f_set_mode)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SetScanOffsetValue");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqmptr) {
        CPQMStudy* l_pqm_study = m_pqmptr->GetStudy();

        if (l_pqm_study) {
            return  l_pqm_study->SetScanOffsetValue(index, f_scanoffset_x, f_scanoffset_y, f_scanoffset_z, f_set_mode);

        } else {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqm_study pointer is NULL"));
            return false;
        }

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return false;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//****************************Method Header************************************
//Module         : SetMoveLocatorMode()
//Author         : PATNI
//Purpose        :
//*****************************************************************************
void CPQMIPCManager::SetMoveLocatorMode(const BOOL f_mode)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SetMoveLocatorMode");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqmptr) {
        m_pqmptr->SetMoveLocatorMode(f_mode ? true : false);

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//****************************Method Header************************************
//Module         : OnPQMExit()
//Author         : PATNI
//Purpose        :
//*****************************************************************************
void CPQMIPCManager::SetDBDtCancelFlag(
    const BOOL f_dbdt_flag
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SetDBDtCancelFlag");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqmptr) {
        m_pqmptr->SetDBDtCancelFlag(f_dbdt_flag);

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//****************************Method Header************************************
//Module         : SetBatchForSARNCoil()
//Author         : PATNI
//Purpose        :
//*****************************************************************************
void CPQMIPCManager::SetBatchForSARNCoil(const BOOL f_flag)const
{

    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SetBatchForSARNCoil");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqmptr) {
        m_pqmptr->SetAllCoilSAR(f_flag ? true : false);

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}

//**************************************************************************
//Method Name   : SetPqmPmDelay
//Author        : PATNI\DKH
//Purpose       :
//**************************************************************************
bool CPQMIPCManager::SetPqmPmDelay(const int f_delay1, const int f_delay2)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SetPqmPmDelay");

    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_pqmptr pointer is NULL"));

        return false;
    }

    CPqmProcon* l_pqm_procon = m_pqmptr->GetPqmProCon();

    if (NULL == l_pqm_procon) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME ,
                  _T("l_pqm_procon pointer is NULL"));

        return false;
    }

    CPQMStudy* l_pqmstudy = m_pqmptr->GetStudy();

    if (NULL == l_pqmstudy) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME ,
                  _T("l_pqmstudy pointer is NULL"));

        return false;
    }

    delayInfo_t l_delayinfo;
    l_delayinfo.delay1 = f_delay1;
    l_delayinfo.delay2 = f_delay2;
    l_delayinfo.study_id = l_pqmstudy->GetStudyID();
    l_pqm_procon->SetPqmPmDelay(l_delayinfo);
    return true;
}

//********************************Method Header********************************
//Method Name   : SetFirstWaitProtForAutoScan()
//Author        :
//Purpose       :
//*****************************************************************************
BOOL CPQMIPCManager::SetFirstWaitProtForAutoScan(
    const int f_acqorder
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SetFirstWaitProtForAutoScan");


    if (m_pqmptr) {

        m_pqmptr->SetFirstWaitProtForAutoScan(f_acqorder);

        return TRUE;


    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));

        return FALSE;
    }
}




//+Patni-HAR/2010Feb24/Added/MSA0251-00187
//****************************Method Header************************************
//Method Name   : SetScanProtocolInfoFromGUI()
//Author        : PATNI\HAR
//Purpose       :
//*****************************************************************************
BOOL CPQMIPCManager::SetScanProtocolInfoFromGUI(
    VARIANT* f_data
)const
{
    if (FALSE == m_pqmptr->SetScanProtocolInfoFromGUI(f_data)) {
        return FALSE;

    } else {
        return TRUE;
    }
}
//-Patni-HAR/2010Feb24/Added/MSA0251-00187

//+Patni-Hemant/2010Feb22/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : SetCDSCheckCoilFlag
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMIPCManager::SetCDSCheckCoilFlag(
    const int f_flag
)const
{
    if (m_pqmptr) {
        m_pqmptr->SetCDSCheckCoilFlag(f_flag);
    }
}
//-Patni-Hemant/2010Feb22/Added/ACE-Phase#3/CDS Requirement

//+Patni-Hemant/2010Jun7/Added/MaFT/MVC008166
//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMIPCManager::SetAvSpdrMap(
    const BOOL f_flag_val
)const
{
    if (m_pqmptr) {
        m_pqmptr->SetAvSpdrMap(f_flag_val);
    }
}
//-Patni-Hemant/2010Jun7/Added/MaFT/MVC008166


//+ Patni-PJS/2009Jul23/Modify/IDS#442,MVC002699
//********************************Method Header********************************
//Method Name   : SetProtocolIndex()
//Author        : PATNI/PJS
//Purpose       : Gets Scan Info ptr
//*****************************************************************************
HRESULT CPQMIPCManager ::SetProtocolIndex(
    const int f_index)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SetProtocolIndex");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqmptr) {
        m_pqmptr->SetProtocolIndex(f_index);
        return S_OK;

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return S_FALSE;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
}
//- Patni-PJS/2009Jul23/Modify/IDS#442,MVC002699

//+Patni-HAR/2010Jan18/Added/DeFT#MVC006364
//****************************Method Header************************************
//Method Name   : SetChildThreadStatus()
//Author        : PATNI\HAR
//Purpose       :
//*****************************************************************************
HRESULT CPQMIPCManager::SetChildThreadStatusToIPC(
    int* f_pData,
    const int f_count,
    const int f_child_thread_status
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SetChildThreadStatusToIPC");

    //Patni-KSS/2011Feb17/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(f_child_thread_status);

    CString l_str(_T(""));
    l_str.Format(_T("Count = %d"), f_count);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);

    CString l_s(_T(""));

    for (int j = 0; j < f_count; j++)  {
        l_str.Format(_T("%d "), f_pData[j]);
        l_s += l_str;
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_s);

    int* l_acq_order_array = f_pData;
    CPqmProtocol* l_protocol = NULL;
    CPQMStudy* l_study = m_pqmptr->GetStudy();

    for (int i = 0; i < f_count; i++) {
        l_protocol = l_study->GetProtocol(l_acq_order_array[i]);

        if (l_protocol)
            l_protocol->SetChildThreadStatus(CT_DELETE);
    }

    return S_OK;
}
//-Patni-HAR/2010Jan18/Added/DeFT#MVC006364
//****************************Method Header************************************
//Method Name   : SetChannelModeSelection()
//Author        : PATNI\Pavan
//Purpose       :
//*****************************************************************************
void CPQMIPCManager::SetChannelModeSelection(const int f_selected_item, BOOL f_selectedchannel_mode, const int f_system_channel)
{

    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SetChannelModeSelection");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));
    // Set the Channel mode setting


    CCoilChannelMode* l_coilchannel_mode = m_pqmptr->GetCoilChannelMode();

    if (NULL == l_coilchannel_mode) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Error in setting coil channel mode"));
        return;
    }

    l_coilchannel_mode->SetChannelModeSelection(f_selected_item, f_selectedchannel_mode);


    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace end"));
}

//****************************Method Header************************************
//Method Name   : SetNumChannels()
//Author        : PATNI\Pavan
//Purpose       :
//*****************************************************************************
void CPQMIPCManager::SetNumChannels(const int f_prot, const int f_channel) const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SetNumChannels");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));



    CCoilChannelMode* l_coilchannel_mode = m_pqmptr->GetCoilChannelMode();

    l_coilchannel_mode->UpdateCoil(f_prot, f_channel);
}
//****************************Method Header************************************
//Method Name   : GetChannelModeSelection()
//Author        : PATNI\Pavan
//Purpose       :
//*****************************************************************************
void CPQMIPCManager::GetChannelModeSelection(const int f_selected_item, VARIANT* f_pdata)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SetDefaultcoilChannelMode");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));


    CCoilChannelMode* l_coilchannel_mode = m_pqmptr->GetCoilChannelMode();

    if (NULL == l_coilchannel_mode) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Error in getting coil channel mode"));
        return;
    }

    l_coilchannel_mode->GetChannelModeSelection(f_selected_item, f_pdata);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace end"));
}

//****************************Method Header************************************
//Method Name   : GetNumSignalfromSelectedSection()
//Author        : PATNI\Pavan
//Purpose       :
//*****************************************************************************
void CPQMIPCManager::GetNumSignalfromSelectedSection(const int f_selected_item, VARIANT* f_pdata)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetNumSignalfromSelectedSection");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));


    CCoilChannelMode* l_coilchannel_mode = m_pqmptr->GetCoilChannelMode();

    if (NULL == l_coilchannel_mode) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Error in getting coil channel mode"));
        return;
    }

    l_coilchannel_mode->GetNumSignalfromSelectedSection(f_selected_item, f_pdata);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace end"));
}

//****************************Method Header************************************
//Method Name   : SetNumSignalfromSelectedSection()
//Author        : PATNI\Pavan
//Purpose       :
//*****************************************************************************
void CPQMIPCManager::SetNumChannelsforChannelMode(const BOOL f_use_coil_channel, const int f_system_enable_channel, const int f_limited_channel)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SetNumChannelsforChannelMode");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));


    CCoilChannelMode* l_coilchannel_mode = m_pqmptr->GetCoilChannelMode();

    if (NULL == l_coilchannel_mode) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Error in getting coil channel mode"));
        return;
    }

    l_coilchannel_mode->SetNumChannelsforChannelMode(f_use_coil_channel, f_system_enable_channel, f_limited_channel);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace end"));
}

/***************************Method Header*************************************/
//Method Name    : CheckAndGetWaitingForAutoScanStartAcqOrder()
//Author         : Patni/Nilesh
//Purpose        :
//*****************************************************************************/
void CPQMIPCManager::CheckAndGetWaitingForAutoScanStartAcqOrder(int* f_waiting_for_auto_scan_acq_order)
{
    m_pqmptr->CheckAndGetWaitingForAutoScanStartAcqOrder(f_waiting_for_auto_scan_acq_order);
}

//****************************Method Header************************************
//Method Name   : GetMasterPositionForSlave()
//Author        : PATNI\AKR
//Purpose       :
//*****************************************************************************
void CPQMIPCManager::GetMasterPositionForSlave(const int f_slave_index, int* f_master_position)const
{
    m_pqmptr->GetMasterPositionForSlave(f_slave_index, f_master_position);
}

/***************************Method Header*************************************/
//Method Name    : ChangeSlaveToNormalProtocol()
//Author         : Patni/AKR
//Purpose        :
//*****************************************************************************/
void CPQMIPCManager::ChangeSlaveToNormalProtocol(const int f_slave_index)
{
    m_pqmptr->ChangeSlaveToNormalProtocol(f_slave_index);
}
/***************************Method Header*************************************/
//Method Name    : GetProtocolIndexToUpdate()
//Author         : Patni/AKR
//Purpose        :
//*****************************************************************************/
void CPQMIPCManager::GetProtocolIndexToUpdate(int* const f_protocol_index)
{
    m_pqmptr->GetProtocolIndexToUpdate(f_protocol_index);
}
/***************************Method Header*************************************/
//Method Name    : GetMasterListForPlanMark()
//Author         : Patni/AKR
//Purpose        :
//*****************************************************************************/
void CPQMIPCManager::GetMasterListForPlanMark(VARIANT* f_pdata)
{
    m_pqmptr->GetMasterListForPlanMark(f_pdata);
}
/***************************Method Header*************************************/
//Method Name    : PerformAutoCopyForMasterSlave()
//Author         : Patni/AKR
//Purpose        :
//*****************************************************************************/
void CPQMIPCManager::PerformAutoCopyForMasterSlave(VARIANT* const f_pdata)
{
    m_pqmptr->PerformAutoCopyForMasterSlave(f_pdata);
}


/***************************Method Header*************************************/
//Method Name    : ResetAutoScanTimer()
//Author         : Patni/AKR
//Purpose        :
//*****************************************************************************/
void CPQMIPCManager::ResetAutoScanTimer()
{
    m_pqmptr->GetAcqManager()->SetInjectTimePresent(false);
    m_pqmptr->GetAcqManager()->AutoScanTimerReset();
}

//************************************Method Header************************************
// Method Name  : SendSAEInfo
// Author       : iGATE/ Amit
// Purpose      : processes received messages
//*************************************************************************************
void CPQMIPCManager::SendSAEInfo()
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SendSAEInfo");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr is NULL"));
        return;
    }

    CPQMStudy* l_pqm_study = m_pqmptr->GetStudy();

    if (NULL == l_pqm_study) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqm_study is NULL"));
        return;
    }

    l_pqm_study->SendSAEInfo();
}
/***************************Method Header*************************************/
//Method Name    : ReConnectSARManager()
//Author         : Patni-AKR
//Purpose        :
//*****************************************************************************/
BOOL CPQMIPCManager::ReConnectSARManager()
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::ReConnectSARManager");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, FUNC_NAME);

    CPqmPmSARManager* l_sar_manager = m_pqmptr->GetSARManager();

    if (NULL == l_sar_manager) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_sar_manager is NULL"));
        return FALSE;
    }

    m_is_sarmanager_connected = l_sar_manager->ConnectToSARManager();

    CString l_message(_T(""));
    l_message.Format(_T("SARManager connection %s."), m_is_sarmanager_connected ? _T("successfull") : _T("failed"));
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_message);


    if (m_is_sarmanager_connected) {
        l_sar_manager->ReInvokeSARManager();
    }

    return m_is_sarmanager_connected;
}
/***************************Method Header*************************************/
//Method Name    : HandleAbortFailForProtocol
//Author         : Patni-AKR
//Purpose        :
//*****************************************************************************/
BOOL CPQMIPCManager::HandleAbortFailForProtocol()
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::HandleAbortFailForProtocol");

    CPqmProtocol* l_last_failed_protocol = m_pqmptr->GetStudy()->GetLastCompletedProtocol();

    if (NULL == l_last_failed_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_last_failed_protocol is NULL"));
        return FALSE;
    }

    CPqmUtilities::UpdateIECInfo(l_last_failed_protocol);

    if (m_pqmptr->GetSARManager() && SARCTRL_LICENCE_ON == m_pqmptr->GetSARControlLicense()) {

        m_pqmptr->GetSARManager()->HandleAbortFailforSARControl();
    }

    return TRUE;
}
/***************************Method Header*************************************/
//Method Name    : CalcSARControlAndPutVFForAll
//Author         : Patni-AKR
//Purpose        :
//*****************************************************************************/
BOOL CPQMIPCManager::CalcSARControlAndPutVFForAll()
{
    if (m_pqmptr->GetSARManager()) {
        m_pqmptr->GetSARManager()->CalcSARControlAndPutVFForAll();
        return TRUE;
    }

    return FALSE;
}

//************************************MethodHeader************************************
// Method Name  : GetSARControlLicense()
// Author       : iGate/Jayanthi
// Purpose      :
//***********************************************************************************
BOOL CPQMIPCManager::GetSARControlLicense()
{
    if (m_pqmptr) {
        if (m_pqmptr->GetSARControlLicense()) {
            return TRUE;
        }
    }

    return FALSE;
}

//****************************Method Header************************************
//Method Name   : GetMaxTimeForPagerTimer()
//Author        : iGate/AKR
//Purpose       :
//*****************************************************************************
BOOL CPQMIPCManager::GetMaxTimeForPagerTimer(int* f_pager_timer_max_time) const
{
    const int l_max_pager_time = m_pqmptr->GetSARManager()->GetMaxTimeForPagerTimer();

    if (l_max_pager_time) {
        *f_pager_timer_max_time = l_max_pager_time;
        return TRUE;
    }

    return FALSE;
}
/***************************Method Header*************************************/
//Method Name    : GetIsSARMgrConnected()
//Author         : Patni-AKR
//Purpose        :
//*****************************************************************************/
BOOL CPQMIPCManager::GetIsSARMgrConnected() const
{
    return m_is_sarmanager_connected;
}
/***************************Method Header*************************************/
//Method Name    : SetIsSARMgrConnected()
//Author         : Patni-AKR
//Purpose        :
//*****************************************************************************/
void CPQMIPCManager::SetIsSARMgrConnected(BOOL const f_is_sarmanager_connected)
{
    m_is_sarmanager_connected = f_is_sarmanager_connected;
}

//************************************Method Header************************************
// Method Name  : RecvCallBack
// Author       : PATNI/ HEMANT
// Purpose      : processes received messages
//*************************************************************************************
BOOL CPQMIPCManager::RecvCallBack(
    void* messageParam,
    void* messageRet,
    PVOID pCustom
)
{
    //+TMSC-Tanoue/2010Sep03/MVC008324
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::RecvCallBack");

    try {
        //+Patni-PJS/2010Feb2/Modified/IR-97
        /*
        CWFDAMsgReceiver*  l_cwfdamsgreceiver = CWFDAMsgReceiver::GetWFDAMsgReceiver();

        if (l_cwfdamsgreceiver)
            return l_cwfdamsgreceiver->RecvCallBack(messageParam, messageRet, pCustom);

        */
        //-Patni-PJS/2010Feb2/Modified/IR-97

        return FALSE;

    } catch (CException* e) {
        const int MSG_SIZE = 1024;
        TCHAR errmsg[MSG_SIZE] = {_T('\0')};
        e->GetErrorMessage(errmsg, MSG_SIZE);
        CString strerr = _T("");
        strerr.Format(_T("[Exception] %s"), errmsg);

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, strerr);
        throw;

    } catch (...) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("[EXCEPTION] l_cwfdamsgreceiver->RecvCallBack()"));
        throw;
    }

    //-TMSC-Tanoue/2010Sep03/MVC008324
}

//+Patni-HAR/2010Apr21/Addded/MVC007534
//****************************Method Header************************************
//Method Name   : CheckFileExist()
//Author        : PATNI / HAR
//Purpose       :
//*****************************************************************************
int CPQMIPCManager::CheckFileExist(
    CString f_study_path
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::CheckFileExist");
    //TMSC-Tanoue/2010Jun18/Modified/MVC008345. Use CVarFieldHandle
    CVarFieldHandle l_vf_handle(f_study_path);
    status_t l_status = l_vf_handle.Open(1, 0); // not retry.

    if (E_NO_ERROR != l_status) {
        CString l_error_string = CPqmUtilities::GetLibVfErrorStatusString(l_status);

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_error_string);

        MRERROR_LOG_DATA error_log_data;
        error_log_data.error_message	= f_study_path + l_error_string;
        error_log_data.error_source		= _T("PQM");
        //CPQMLogMgr::GetInstance()->DisplayMessageinErrorDialog(error_log_data);
        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(error_log_data.error_message,
                MPlus::EH::Information, _T("PQMIPCManager.cpp"), _T("PQM"));
        //Patni-DKH/2010Apr21/Addded/MVC007534
        return l_status;
    }

    return l_status;
}
//-Patni-HAR/2010Apr21/Addded/MVC007534


/***************************Method Header*************************************/
//Method Name    : SetIPCConfigInfo()
//Author         : PATNI/ARD
//Purpose        : Set the ICP side configurable variables.
//*****************************************************************************/
void CPQMIPCManager::SetIPCConfigInfo(VARIANT* p_data, const int f_mode)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SetIPCConfigInfo");
    SIPCConfigInfo* l_sIPCConfiginfo;
    l_sIPCConfiginfo = (SIPCConfigInfo*)p_data->byref;

    //+Patni-ARD/2010Mar24/Modified/DeFT# IR-100 code review
    //+Patni-ARD/2010Apr08/Modified/JaFT# IR-100
    int l_av_volume			= 80;
    int l_av_Dynamic_Delay	= 500;
    int l_av_TP1_Delay		= 500;
    //+Patni-ARD/2010Apr08/Modified/JaFT# IR-100

    if (NULL != l_sIPCConfiginfo) {

        m_pqmptr->SetConfigInfo(l_sIPCConfiginfo);
        l_av_volume			= l_sIPCConfiginfo->av_Volume;
        l_av_Dynamic_Delay	= l_sIPCConfiginfo->av_Dynamic_Delay;
        l_av_TP1_Delay		= l_sIPCConfiginfo->av_TP1_Delay;

    } else {
        m_pqmptr->GetStudyManagerPtr()->SetSeriesBias(DEFAULT_SERIES_BIAS);

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("l_sIPCConfiginfo is NULL"));
    }

    if (f_mode == K_IMAGING) {
        //+Patni-HEMANT-HAR-SS/2010May20/Modified/MaFT#TMSC-REDMINE-23
        CVoiceManager::GetInstance()->InitializeVoiceMgr(f_mode,
                l_av_volume, l_av_Dynamic_Delay, l_av_TP1_Delay, m_pqmptr);
        //-Patni-HEMANT-HAR-SS/2010May20/Modified/MaFT#TMSC-REDMINE-23
        //Patni-HAR/2010Jun30/Added/V1.30#TMSC-REDMINE-385

        //Patni-Sribanta/2011Jan14/Commented/TMSC-REDMINE-1026
        //CVoiceManager::GetInstance()->CmdVolume();
    }

    //-Patni-ARD/2010Mar24/Modified/DeFT# IR-100 code review
}


//****************************Method Header************************************
//Method Name   :ThreadProc()
//Author        :PATNI
//Purpose       :
//*****************************************************************************
DWORD WINAPI CPQMIPCManager::ThreadProc(
    LPVOID lpParameter
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::ThreadProc");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));
    wchar_t l_buffer[MAX_PATH];

    __try {
        CPQMIPCManager* self = (CPQMIPCManager*)lpParameter;
        CoInitializeEx(NULL, COINIT_MULTITHREADED);

        if (self->m_pqmptr->GetMode() == K_IMAGING) {
            CPQMIPCManager::m_ThreadEvent = CreateEvent(NULL, false, false, _T("PQM_ACQUIRE_EVENT"));

        } else {
            CPQMIPCManager::m_ThreadEvent = CreateEvent(NULL, false, false, _T("PQM_SCHEDULE_EVENT"));
        }

        //+Patni-NP/2009Apr09/Added/PSP1 Changes For VF Lock
        MSG msg;

        int l_result = 0;

        while (1) {

            GetMessage(&msg, NULL, 0, 0);

            if (msg.message  == MSG_CLOSE) {
                break;
            }

            // - Patni - HAR / 2009Mar11 / Write Protocol Status In Local DB
            if (CPQMIPCManager::m_ThreadEvent)
                l_result = SetEvent(CPQMIPCManager::m_ThreadEvent);

            if (l_result == 0) {
                DWORD error_code = GetLastError();

                wsprintf(l_buffer, _T("%d"),  error_code);
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_buffer);

            }
        }

        //+Patni-NP/2009Apr09/Added/PSP1 Changes For VF Lock
        // 20091006 Tanoue-san forCoUninitialize
        CoUninitialize();

    } __except(RecordExceptionInfo(GetExceptionInformation(),
                                   _T("CPQMIPCManager::ThreadProc"))) {

    }

    return FALSE;
}

//*******************************Method Header*********************************
//Method Name    :GetAPCHoldValue()
//Author         :PATNI/MSN
//Purpose        :Gets the APC Hold Value set on pressing APC Pause menu
//*****************************************************************************
int CPQMIPCManager::GetAPCHoldValue(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetAPCHoldValue");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == m_pqmptr) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return 0;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetAPCHoldValue"));

    CPqmPmAcqman* acqman = m_pqmptr->GetAcqManager();

    if (NULL == acqman) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("acqman is NULL"));
        return false;
    }

    CScanProcedure* scanprocedure = acqman->GetScanProcedure();

    if (NULL == scanprocedure) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("scanprocedure is NULL"));
        return false;
    }

    return scanprocedure->GetAPCHold();
}

//+Patni-ARD/2010Mar31/Added/DeFT# IR-100
/***************************Method Header*************************************/
//Method Name    : SetStudyMgrSeriesBias()
//Author         : PATNI/ARD
//Purpose        : Set the bias value for study mgr.
//*****************************************************************************/
void CPQMIPCManager::SetStudyMgrSeriesBias(VARIANT* f_config_data)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SetStudyMgrSeriesBias");

    CPQMStudyMgr* l_pqm_studymgr = m_pqmptr->GetStudyManagerPtr();

    if (NULL != l_pqm_studymgr) {
        SIPCConfigInfo* l_sIPCConfiginfo = (SIPCConfigInfo*)f_config_data->byref;
        l_pqm_studymgr->SetSeriesBias(l_sIPCConfiginfo->series_bias);

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("l_pqm_studymgr pointer is NULL"));
    }
}
//-Patni-ARD/2010Mar31/Added/DeFT# IR-100

// :- No Declaration
//****************************Method Header************************************
//Method Name   :GetStudyUid()
//Author        :PATNI / HAR
//Purpose       :
//*****************************************************************************
//CString CPqm::GetStudyUid(
//) {
//    return m_study_uid;
//}

//****************************Method Header************************************
//Method Name   :SetStudyUid()
//Author        :PATNI / HAR
//Purpose       :
//*****************************************************************************
//void CPqm::SetStudyUid(
//	CString f_study_uid
//) {
//    m_study_uid = f_study_uid;
//}
//+Patni-PJS/2009Sep23/Added/MVC4941
/***************************Method Header*************************************/
//Method Name    : InitializeTicks()
//Author         : PATNI/PJS
//Purpose        : Initialize the ticks of the voice file to be played
//*****************************************************************************/
//+Patni-Sribanta/2011Jan11/Commented/TMSC-REDMINE-1026

void CPQMIPCManager::InitializeTicks(
)const
{
    if (m_pqmptr) {

        CVoiceManager::GetInstance()->InitializeTicks();
    }
}

//-Patni-Sribanta/2011Jan11/Modified/TMSC-REDMINE-1026


/***************************Method Header*************************************/
//Method Name    : GetMulitScanDynamic_License()
//Author         : iGATEPatni/Abhishek
//Purpose        : Redmine-2187 It checks for Multiscan Dynamic license
//*****************************************************************************/
BOOL CPQMIPCManager::GetMulitScanDynamic_License()const
{
    if (m_pqmptr) {
        return m_pqmptr->GetMulitScanDynamic_License();

    } else {
        return FALSE;
    }
}
//********************************Method Header********************************
//Method Name   : SetAutoLocatorRunning()
//Author        : PATNI/PJS
//Purpose       :
//*****************************************************************************
void CPQMIPCManager::SetAutoLocatorRunning(
    const BOOL f_cala_appln_runng
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SetAutoLocatorRunning");

    if (!m_pqmptr) {
        PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr is NULL"));
        return;
    }

    m_pqmptr->SetAutoLocatorRunning(f_cala_appln_runng);
}

//********************************Method Header********************************
//Method Name   : GetProtIndexAndWorkingMode()
//Author        : PATNI/PJS
//Purpose       :
//*****************************************************************************
HRESULT CPQMIPCManager::GetProtIndexAndWorkingMode(
    int f_acq_order,
    VARIANT* f_study_path
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetProtIndexAndWorkingMode");

    if (m_pqmptr) {
        CPQMStudy* l_pqm_study = m_pqmptr->GetStudy();

        if (l_pqm_study) {
            CString l_study_path = l_pqm_study->GetStudyPath();
            CPqmProtocol* l_pqm_prot = l_pqm_study->GetProtocol(f_acq_order);

            if (NULL == l_pqm_prot) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqm_prot pointer is NULL"));
                return S_FALSE;
            }

            AUTOLOCATOR_TYPE l_altype = l_pqm_prot->GetAutoLocatorType();
            AL_INVOKE_MODE l_work_mode =  l_pqm_prot->GetAutoLocatorInvokeMode(l_altype);
            CString l_prot_index, l_mode;
            l_prot_index.Format(_T("1.%d"), l_pqm_prot->GetProtocol());
            l_mode.Format(_T("%d"), l_work_mode);
            l_study_path +=  _T(":/") + l_prot_index + _T("\\") + l_mode;
            f_study_path->bstrVal = l_study_path.AllocSysString();

            if (f_study_path) {
                return S_OK;

            } else {
                return S_FALSE;
            }

        } else {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqm_study pointer is NULL"));
            return S_FALSE;
        }

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return S_FALSE;
    }
}

//********************************Method Header********************************
//Method Name   : IsValidSequenceToInvokeCALAAppln()
//Author        : PATNI/PJS
//Purpose       :
//*****************************************************************************
HRESULT CPQMIPCManager::GetProtIndexAndWorkingMode(
    VARIANT* f_study_path
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetProtIndexAndWorkingMode");

    if (m_pqmptr) {
        CPQMStudy* l_pqm_study = m_pqmptr->GetStudy();

        if (l_pqm_study) {
            CString l_study_path = l_pqm_study->GetStudyPath();
            CPqmProtocol* l_pqm_prot = l_pqm_study->GetLastDone();

            if (NULL == l_pqm_prot || l_study_path.IsEmpty()) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqm_prot pointer is NULL or l_study_path is empty"));
                return S_FALSE;
            }

            AUTOLOCATOR_TYPE l_al_type = l_pqm_prot->GetAutoLocatorType();

            AL_INVOKE_MODE l_work_mode =  l_pqm_prot->GetAutoLocatorInvokeMode(l_al_type);
            CString l_prot_index, l_mode;
            l_prot_index.Format(_T("1.%d"), l_pqm_prot->GetProtocol());
            l_mode.Format(_T("%d"), l_work_mode);
            l_study_path +=  _T(":/") + l_prot_index + _T("\\") + l_mode;
            f_study_path->bstrVal = l_study_path.AllocSysString();

            if (f_study_path) {
                return S_OK;

            } else {
                return S_FALSE;
            }

        } else {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqm_study pointer is NULL"));
            return S_FALSE;
        }

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr pointer is NULL"));
        return S_FALSE;
    }

}

//********************************Method Header********************************
//Method Name   : IsValidSequenceToInvokeCALAAppln()
//Author        : PATNI/PJS
//Purpose       :
//*****************************************************************************
BOOL CPQMIPCManager::IsValidSequenceToInvokeAutoLocator(
    const int f_acq_order, const int f_autolocatortype
) const
{

    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::IsValidSequenceToInvokeAutoLocator");

    if (!m_pqmptr) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr object is NULL"));
        return FALSE;
    }

    return m_pqmptr->IsValidSequenceToInvokeAutoLocator(f_acq_order, (AUTOLOCATOR_TYPE)f_autolocatortype);
}

//****************************Method Header************************************
//Method Name    : SendWirelessGateParam()
//Author         :
//Purpose        :
//*****************************************************************************
int CPQMIPCManager::SendWirelessGateParam(
    VARIANT* f_pdata
)
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::SendWirelessGateParam");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMIPCManager::SendWirelessGateParam"));


    if (NULL == m_pqmptr || NULL == f_pdata) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr or f_pdata pointer is NULL"));
        return FALSE;
    }

    BOOL l_result = TRUE;

    CPqmProtocol* l_top_wait_protocol = NULL;

    if (m_pqmptr->GetStudy()->GetTopWaitPos(&l_top_wait_protocol) < 0 || l_top_wait_protocol == NULL) {

        CPQMLogMgr::GetInstance()->DisplayScanError(IDS_NO_WAIT_PROTOCOLS, _T("IDS_NO_WAIT_PROTOCOLS"), SCAN_ERR_NOTIFY, _T("PqmIpcManager.cpp"));

        l_result = FALSE;

        return l_result;

    }

    CScanProcedure* l_scan_proc_ptr = m_pqmptr->GetAcqManager()->GetScanProcedure();

    Wireless_gating_t* l_wirelessgateinfo = (Wireless_gating_t*)f_pdata->byref;


    if (l_scan_proc_ptr && !l_scan_proc_ptr->GetIsScanning()) {

        l_scan_proc_ptr->SetScanProcProtocol(l_top_wait_protocol);

        if (!l_scan_proc_ptr->SendGateModeInfo(VFC_GAT_METHOD_BIT_ECG, l_wirelessgateinfo)) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed SendGateModeInfo()"));
            l_result = FALSE;

        } else {
            PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Success SendGateModeInfo()!"));
            l_result = TRUE;
        }

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("****Scan is going on.  cannot sent WirelessGateParam****"));
        l_result = FALSE;
    }

    return l_result;
}
//**************************************************************************
//Method Name   : GetWirelessGatingSelections
//Author        :
//Purpose       :
//**************************************************************************
bool CPQMIPCManager::GetWirelessGatingSelections(
    VARIANT* f_data
)const
{

    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetWirelessGatingSelections");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    Wireless_gating_t* l_wirelessgateparam = m_pqmptr->GetGateParamObj()->GetWirelessGateParam();

    if (NULL == l_wirelessgateparam) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("l_wirelessgateparam is NULL"));
        return false;
    }

    if (NULL != f_data) {
        f_data->byref = l_wirelessgateparam;

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("pointer fdata is NULL"));
        return false;
    }

    return true;

}

HRESULT CPQMIPCManager::GetAutoLocatorPlannedStatus(
    const int f_acq_order, int* f_cala_planned_status
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMIPCManager::GetAutoLocatorPlannedStatus");

    if (!m_pqmptr) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("m_pqmptr object is NULL"));
        return S_FALSE;
    }

    CPqmProtocol* protocol = m_pqmptr->GetStudy()->GetProtocol(f_acq_order);

    if (!protocol) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("protocol pointer is NULL"));
        return S_FALSE;
    }

    AL_PLANNED_STATUS l_al_planned_status = protocol->GetCalaPlannedStatus();

    if (l_al_planned_status == PLANNED_STATUS_NONE) {
        l_al_planned_status = protocol->GetHALAPlannedStatus();
    }

    *f_cala_planned_status = static_cast<int>(l_al_planned_status);

    return S_OK;
}

/***************************Method Header******************************************/
//Method Name    : GetCALALicense()
//Author         : PJS
//Purpose        : This function checks wether CALA License is present or not
//*********************************************************************************/
BOOL CPQMIPCManager::GetAutoLocatorLicense(const int f_autolocator_type
                                          )const
{
    if (!m_pqmptr) {
        return FALSE;
    }

    return m_pqmptr->GetAutoLocatorLicense((AUTOLOCATOR_TYPE)f_autolocator_type);
}
//****************************Method Header************************************
//Method Name   : GetScanMutex()
//Author        : PATNI/AKR
//Purpose       :
//*****************************************************************************
BOOL CPQMIPCManager::GetScanMutex(
    VARIANT** pData
)
{
    if (!m_pqmptr) {
        return FALSE;
    }

    return m_pqmptr->GetScanMutex(pData);
}
//****************************Method Header************************************
//Method Name   : GetStudyLock()
//Author        : iGATE
//Purpose       :
//*****************************************************************************
BOOL CPQMIPCManager::GetStudyLock(
    VARIANT** pData
)
{
    if (!m_pqmptr) {
        return FALSE;
    }

    return m_pqmptr->GetStudyLock(pData);
}

/***************************Method Header******************************************/
//Method Name    : SetWirelessParamtoProt()
//Author         : Patni
//Purpose        :
//*********************************************************************************/

void CPQMIPCManager::SetWirelessParamtoProt(
    VARIANT**  f_wireless_param
)
{

    if ((*f_wireless_param)) {

        Wireless_gating_t* l_wireless_param = (Wireless_gating_t*)(*f_wireless_param)->byref ;

        m_pqmptr->GetGateParamObj()->SetWirelessGateParam(l_wireless_param);

    }
}
/***************************Method Header******************************************/
//Method Name    : UpdateSARInfo()
//Author         : Patni
//Purpose        :
//*********************************************************************************/
void CPQMIPCManager::UpdateSARInfo()
{
    if (!m_pqmptr) {
        return;
    }

    m_pqmptr->UpdateSARInfo();
}
/***************************Method Header******************************************/
//Method Name    : GetDefaultPGUID()
//Author         : Patni
//Purpose        :
//*********************************************************************************/
CString CPQMIPCManager::GetDefaultPGUID(const int f_inscantype)
{

    switch (f_inscantype) {
        case VFC_INSCAN_MRS :
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
/***************************Method Header******************************************/
//Method Name    : GetRunningProcCount()
//Author         : iGatePatni
//Purpose        :
//*********************************************************************************/
void CPQMIPCManager::GetRunningProcCount(int* f_proc_count)
{
    if (f_proc_count) {
        *f_proc_count = m_pqmptr->GetAcqManager()->GetScanProcedure()->GetProcedureCount();
    }
}


//************************************Method Header************************************
// Method Name  : GetAMBStatusData
// Author       : iGATE
// Purpose      : Returns the AMB status data structure
//***********************************************************************************
BOOL CPQMIPCManager::GetAMBStatusData(int* f_selected_items, int count, VARIANT**  data)
{
    if (m_pqmptr) {
        return m_pqmptr->GetAMB()->CheckAmbInitialPlan(f_selected_items, count);
    }

    return false;
}
//************************************Method Header************************************
// Method Name  : GetAMBPlan
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMIPCManager::GetAMBPlan(VARIANT**  data)
{
    if (m_pqmptr) {
        m_pqmptr->GetAMBPlan(data);
    }
}
//************************************Method Header************************************
// Method Name  : AMBScanStart
// Author       : iGATE
// Purpose      :
//***********************************************************************************
BOOL CPQMIPCManager::AMBScanStart(VARIANT**  data)
{
    if (m_pqmptr) {
        if (data) {
            amb_t* l_amb = (amb_t*)(*data)->byref;

            if (l_amb) {
                if (E_ERROR == m_pqmptr->GetAMB()->pqmAMBScanstart(l_amb))
                    return FALSE;

                else
                    return TRUE;
            }
        }

    }

    return FALSE;
}
//************************************Method Header************************************
// Method Name  : AMBScanAbort
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMIPCManager::AMBScanAbort()
{
    if (m_pqmptr) {
        m_pqmptr->GetAMB()->pqmAMBScanAbort();
    }

}
//************************************Method Header************************************
// Method Name  : GetRearrangeIndex
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMIPCManager::GetRearrangeIndex(int* f_protocol_index_to_move, int* f_target_index)
{
    if (m_pqmptr) {
        m_pqmptr->GetAMB()->GetRearrangeIndex(f_protocol_index_to_move, f_target_index);
    }
}
//************************************Method Header************************************
// Method Name  : AMBReArrange
// Author       : iGATE
// Purpose      :
//***********************************************************************************
int CPQMIPCManager::AMBReArrange(const int f_drag_index, const int f_drop_index)
{
    if (m_pqmptr) {
        return m_pqmptr->GetStudy()->AMBReArrange(f_drag_index, f_drop_index);
    }

    return 0;

}
//************************************Method Header************************************
// Method Name  : GetAMBAutoScanItems
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMIPCManager::GetAMBAutoScanItems(int** f_arr_items, int* f_count, BOOL* l_is_auto_scan_on)
{
    if (m_pqmptr) {
        m_pqmptr->GetAMB()->GetAMBAutoScanItems(f_arr_items, f_count, l_is_auto_scan_on);
    }
}
//************************************Method Header************************************
// Method Name  : GetAMBMasterProtocols
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMIPCManager::GetAMBMasterProtocols(VARIANT** f_data)
{
    if (m_pqmptr) {
        m_pqmptr->GetAMB()->GetAMBMasterProtocols(f_data);
    }
}
//************************************Method Header************************************
// Method Name  : GetAMBLicense
// Author       : iGATE
// Purpose      :
//***********************************************************************************
BOOL CPQMIPCManager::GetAMBLicense()const
{
    if (m_pqmptr) {
        return m_pqmptr->GetAMBLicense();
    }

    return FALSE;
}
//************************************Method Header************************************
// Method Name  : GetAMBShimMapArray
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMIPCManager::GetAMBShimMapArray(VARIANT** f_data)
{
    if (m_pqmptr) {
        m_pqmptr->GetAMB()->GetAMBShimMapArray(f_data);
    }
}
//************************************Method Header************************************
// Method Name  : GetAMBIndexToUdpate
// Author       : iGATE
// Purpose      :
//***********************************************************************************
int CPQMIPCManager::GetAMBIndexToUdpate()
{
    if (m_pqmptr) {
        return m_pqmptr->GetAMB()->GetAMBIndexToUdpate();
    }

    return FALSE;
}
//************************************Method Header************************************
// Method Name  : CheckSlaveProtocolForDuplicate
// Author       : iGATE
// Purpose      :
//***********************************************************************************

bool CPQMIPCManager::CheckSlaveProtocolForDuplicate(int* f_selected_items, int f_count)
{

    return m_pqmptr->GetStudy()->CheckSlaveProtocolForDuplicate(f_selected_items, f_count);
}
//************************************Method Header************************************
// Method Name  : AMBSlaveMatchSequenceParam
// Author       : iGATE
// Purpose      :
//***********************************************************************************
bool CPQMIPCManager::AMBSlaveMatchSequenceParam(VARIANT* f_pData, const int f_start_index, const int f_end_index)
{
    return m_pqmptr->AMBSlaveMatchSequenceParam(f_pData, f_start_index, f_end_index);
}
//************************************Method Header************************************
// Method Name  : GetAMBSlaveCount
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMIPCManager::GetAMBSlaveCount(int* f_count)
{
    m_pqmptr->GetAMB()->GetAMBSlaveCount(f_count);
}
//************************************Method Header************************************
// Method Name  : PqmPqAMBSlaveLost
// Author       : iGATE
// Purpose      :
//***********************************************************************************
BOOL CPQMIPCManager::PqmPqAMBSlaveLost(int* f_selected_list , int* f_selected_count)
{
    return m_pqmptr->GetAMB()->PqmPqAMBSlaveLost(f_selected_list, f_selected_count);
}

//************************************Method Header************************************
// Method Name  : GetNoOfSeqRecvdfromMemory
// Author       : iGATE
// Purpose      :
//***********************************************************************************
int CPQMIPCManager::GetNoOfSeqRecvdfromMemory()const
{
    return m_no_of_sequence_recvd_frm_memory;
}

//************************************Method Header************************************
// Method Name  : SetNoOfSeqRecvdfromMemory
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMIPCManager::SetNoOfSeqRecvdfromMemory(int no_seq_recvd)
{
    m_no_of_sequence_recvd_frm_memory = no_seq_recvd;
}

void CPQMIPCManager::SetIsAMBPlanning(const bool f_is_amb_planning)
{
    m_pqmptr->SetIsAMBPlanning(f_is_amb_planning);
}

//************************************Method Header************************************
// Method Name  : GetIndexForParentImageRemoved
// Author       : iGATE
// Purpose      :
//***********************************************************************************
int CPQMIPCManager::GetIndexForParentImageRemoved()
{
    return m_pqmptr->GetAMB()->GetIndexForParentImageRemoved();
}
//************************************Method Header************************************
// Method Name  : CheckAMBMenuClickConditions
// Author       : iGATE
// Purpose      :
//***********************************************************************************
BOOL CPQMIPCManager::CheckAMBMenuClickConditions(const int* f_selected_items, const int f_count)const
{
    return m_pqmptr->GetAMB()->CheckAMBMenuClickConditions(f_selected_items, f_count);
}

//************************************Method Header************************************
// Method Name  : GetAPCRetry
// Author       : iGATE
// Purpose      :
//***********************************************************************************
BOOL CPQMIPCManager::GetAPCRetry()const
{
    return m_pqmptr->GetAcqManager()->GetScanProcedure()->GetAPCRetry();
}

/***************************Method Header******************************************/
//Method Name    : GetRunningProcCount()
//Author         : iGatePatni
//Purpose        :
//*********************************************************************************/
int CPQMIPCManager::UpdateTIinStudy(int * f_prot_idx, float * f_ti_val)
{
   
   return  m_pqmptr->UpdateTIinStudy(f_prot_idx,f_ti_val);
   
}
int CPQMIPCManager::GetGDCSeriesLOIDForProtocol(int protocol, VARIANT *seriesloid)
{
	   return  m_pqmptr->GetGDCSeriesLOIDForProtocol(protocol,seriesloid);

}

BOOL CPQMIPCManager::GetTiPrepLicense( const int f_tiprep_type ) const
{
    if (!m_pqmptr) {
        return FALSE;
    }
    
    return m_pqmptr->GetTiPrepLicense((TIPREP_TYPE)f_tiprep_type);

}

bool CPQMIPCManager::IsProtocolforTiPrep( int f_acq_order )
{
    if (!m_pqmptr) {
        return false;
    }
    
    return m_pqmptr->IsProtocolforTiPrep(f_acq_order);

}

BOOL CPQMIPCManager::CanTiPrepEnable()
{
    if (NULL == m_pqmptr) {
        return(FALSE);
    }
    
    return(m_pqmptr->CanTiPrepEnable());
}

//Redmine-779
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PqmIpcCom.cpp
 *  Overview: Implementation of CPqmIpcCom class.
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
#include "stdafx.h"
#include "IPCCom.h"
#include <libStudyManager/destroy.h>
#include "PqmIpcCom.h"
#include <PQM/PQMIPCManager.h>
#include <PQM/PQMLogMgr.h>

//*******************************Method Header*********************************

//Method Name    :CPqmIpcCom()

//Author         :PATNI/HAR

//Purpose        :Constructor of COM DLL class (CPqmIpcCom)

//*****************************************************************************
CPqmIpcCom::CPqmIpcCom(
)
{
    m_ptrManager = NULL;

    // MEITEC/2010Apr23/Added/IR86
    m_ptrManager = new CPQMIPCManager(WrapperRaiseEvent, WrapperRaiseDisplayEvent, this);
}


//*******************************Method Header*********************************

//Method Name    :~CPqmIpcCom()

//Author         :PATNI/HAR

//Purpose        :Destructor of COM DLL class (CPqmIpcCom)

//*****************************************************************************
CPqmIpcCom::~CPqmIpcCom(
)
{
    //+ MEITEC/2010Jun23/Added/MVC008324_1
    PQM_TRACE(SYSTEM_FUNC_MARKER, _T("CPqmIpcCom::~CPqmIpcCom"),
              _T("Trace of Destructor"));
    //- MEITEC/2010Jun23/Added/MVC008324_1

    // Deallocate memoru of PQM IPC Manager object
    DEL_PTR(m_ptrManager);
}

//*******************************Method Header*********************************

//Method Name    :Initialize()

//Author         :PATNI/HAR

//Purpose        :Initialize the PQM Lib

//*****************************************************************************
//Patni-ARD/2010Mar29/Modified/Ph-3# IR-100
STDMETHODIMP CPqmIpcCom::Initialize(
    int f_mode, VARIANT* f_config_data
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())


    if (m_ptrManager) {
        //Patni-ARD/2010Mar29/Modified/Ph-3# IR-100
        if (!m_ptrManager->StartPQM(f_mode, f_config_data)) {
            return S_FALSE;
        }

    } else {
        return S_FALSE;
    }

    return S_OK;
}

//*******************************Method Header*********************************

//Method Name    :StartScan()

//Author         :PATNI/HAR

//Purpose        :Send the message to lib to Start Scan

//*****************************************************************************
//+Patni-ARD/2009Sept01/Modified/IR GUI IPC check SEQ
STDMETHODIMP CPqmIpcCom::StartScan(
    VARIANT* f_pVariant,
    int f_selected_series
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())


    if (m_ptrManager) {
        m_ptrManager->StartScan(f_pVariant, f_selected_series);

    } else {
        return S_FALSE;
    }

    return S_OK;
}
//-Patni-ARD/2009Sept01/Modified/IR GUI IPC check SEQ

//*******************************Method Header*********************************

//Method Name    :AbortScan()

//Author         :PATNI/HAR

//Purpose        :Send the message to lib to Abort Scan

//*****************************************************************************
STDMETHODIMP CPqmIpcCom::AbortScan(
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())


    if (m_ptrManager) {

        m_ptrManager->AbortScan();

    } else {
        return S_FALSE;
    }

    return S_OK;
}

//*******************************Method Header*********************************

//Method Name    :AbortScan()

//Author         :PATNI/HAR

//Purpose        :Send the message to lib to Abort Scan

//*****************************************************************************
STDMETHODIMP CPqmIpcCom::MrsScanClose(
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())


    if (m_ptrManager) {

        m_ptrManager->MrsScanClose();

    } else {
        return S_FALSE;
    }

    return S_OK;
}

//*******************************Method Header*********************************

//Method Name    :PauseScan()

//Author         :PATNI/HAR

//Purpose        :Send the message to lib to Pause Scan

//*****************************************************************************
STDMETHODIMP CPqmIpcCom::PauseScan(
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        m_ptrManager->PauseScan();

    } else {
        return S_FALSE;
    }

    return S_OK;
}

//*******************************Method Header*********************************

//Method Name    :ResumeScan()

//Author         :PATNI/HAR

//Purpose        :Send the message to lib to Resume Scan

//*****************************************************************************
STDMETHODIMP CPqmIpcCom::ResumeScan(
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())


    if (m_ptrManager) {
        m_ptrManager->ResumeScan();

    } else {
        return S_FALSE;
    }

    return S_OK;
}

//*******************************Method Header*********************************

//Method Name    :WrapperRaiseEvent()

//Author         :PATNI/HAR

//Purpose        :This method get the message from the Lib

//*****************************************************************************
bool CPqmIpcCom::WrapperRaiseEvent(
    pqm_msg_tag Msg,
    void* ptrObj
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    bool bRet = true;
    CPqmIpcCom* mySelf = (CPqmIpcCom*) ptrObj;

    bRet = mySelf->RaiseEvent(Msg);

    return bRet;
}

//*******************************Method Header*********************************

//Method Name    :RaiseEvent()

//Author         :PATNI/HAR

//Purpose        :This method calls to the event sink

//*****************************************************************************
bool CPqmIpcCom::RaiseEvent(
    pqm_msg_tag Msg
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    bool bRet = true;

    Fire_SendMsgToUI(Msg);

    return bRet;
}


//*******************************Method Header*********************************

//Method Name    :RecieveLOID()

//Author         :PATNI/JS

//Purpose        :This function receives the Study LOID from PQM.

//*****************************************************************************
STDMETHODIMP CPqmIpcCom::ReceiveLOID(
    BSTR f_strpatientloid,
    BSTR f_strstudyloid,
    BSTR f_pas_path //Patni-Sribanta/2010Nov10/Modified/IR-170
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //+Patni-MN/2009Mar03/Modified/PSP1#138/Checked the return value
    BOOL l_bSameLoidCheck = TRUE;

    //+Patni-AD/2009Aug19/Modified/SU15 //CPP test corrections
    if (m_ptrManager != NULL) {

        //Patni-Sribanta/2010Nov10/Modified/IR-170
        l_bSameLoidCheck = m_ptrManager->LoadStudyLOID(f_strpatientloid, f_strstudyloid, f_pas_path);
        //+Patni-HAR/2010Apr21/Modified/MVC007534
        return l_bSameLoidCheck;
        //if (FALSE == l_bSameLoidCheck) {
        //    return E_FAIL;

        //else {
        //    return S_OK;
        //}
        //-Patni-HAR/2010Apr21/Modified/MVC007534
        //-Patni-MN/2009Mar03/Modified/PSP1#138/Checked the return value

    } else {

        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::ReceiveLOID"),
        //	_T("m_ptrManager is NULL"),false);
        return E_FAIL;
    }

    //-Patni-AD/2009Aug19/Modified/SU15 //CPP test corrections
    return S_OK;
}

//*******************************Method Header*********************************


//Method Name    :ReceiveSeriesAll()

//Author         :PATNI/PJP

//Purpose        :This function receives all the series ID from PQM.

//*****************************************************************************
STDMETHODIMP CPqmIpcCom::ReceiveSeriesAll(
    BSTR bStr
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //+Patni-AD/2009Aug19/Modified/SU15 //CPP test corrections
    if (m_ptrManager != NULL) {
        m_ptrManager->DeleteAll(bStr);

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::ReceiveSeriesAll"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-AD/2009Aug19/Modified/SU15 //CPP test corrections
    return S_OK;
}

/****************************Method Header************************************

//Method Name   :ReceiveSeriesOrder()

//Author        :PATNI/RD

//Purpose       :For Rearrange
//*****************************************************************************/
STDMETHODIMP CPqmIpcCom::ReceiveSeriesOrder(
    BSTR bStr,
    int dropindex
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //+Patni-AD/2009Aug19/Modified/SU15 //CPP test corrections
    if (m_ptrManager != NULL) {
        m_ptrManager->SetSeriesOrder(bStr, dropindex);

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::ReceiveSeriesOrder"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-AD/2009Aug19/Modified/SU15 //CPP test corrections
    return S_OK;
}

/****************************Method Header************************************

//Method Name   :ConnectVoiceManager()

//Author        :PATNI/MSN

//Purpose       :For Connecting to Voice Manager
//*****************************************************************************/
STDMETHODIMP CPqmIpcCom::ConnectVoiceManager(
    int voice_param
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //+Patni-AD/2009Aug19/Modified/SU15 //CPP test corrections
    if (m_ptrManager != NULL) {
        m_ptrManager->ConnectVoiceManager(voice_param);

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::ConnectVoiceManager"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-AD/2009Aug19/Modified/SU15 //CPP test corrections
    return S_OK;
}

/****************************Method Header************************************

//Method Name   :SetAutoVoice()

//Author        :PATNI/MSN

//Purpose       :Sets Auto Voice Flag
//*****************************************************************************/
STDMETHODIMP CPqmIpcCom::SetAutoVoice(
    BOOL flag
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //+Patni-AD/2009Aug19/Modified/SU15 //CPP test corrections
    if (m_ptrManager != NULL) {

        m_ptrManager->SetAutoVoiceFlag(flag);

    } else {

        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::SetAutoVoice"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-AD/2009Aug19/Modified/SU15 //CPP test corrections
    return S_OK;
}

/****************************Method Header************************************

//Method Name   :UpdateComment()

//Author        :PATNI/JS

//Purpose       :Updates the Comment in local DB
//*****************************************************************************/
STDMETHODIMP CPqmIpcCom::UpdateComment(
    BSTR bstr,
    BSTR scancomment,
    BSTR scanid
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //+Patni-AD/2009Aug19/Modified/SU15 //CPP test corrections
    if (m_ptrManager != NULL) {
        m_ptrManager->UpdateComment(bstr, scancomment, scanid);

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::UpdateComment"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-AD/2009Aug19/Modified/SU15 //CPP test corrections
    return S_OK;
}

/****************************Method Header************************************

//Method Name   :OnPQMCloseButton()

//Author        :PATNI/MSN

//Purpose       :Disconnects the Voice Manager
//*****************************************************************************/
STDMETHODIMP CPqmIpcCom::OnPQMCloseButton(
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //+Patni-AD/2009Aug19/Modified/SU15 //CPP test corrections
    if (m_ptrManager != NULL) {
        m_ptrManager->OnPQMCloseButton();

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::OnPQMCloseButton"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-AD/2009Aug19/Modified/SU15 //CPP test corrections
    return S_OK;
}

//*******************************Method Header*********************************

//Method Name    :GetColorDataEx()

//Author         :PATNI/

//Purpose        :

//*****************************************************************************
STDMETHODIMP CPqmIpcCom::GetColorDataEx(
    VARIANT* data
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //+Patni-AD/2009Aug19/Modified/SU15 //CPP test corrections
    if (m_ptrManager != NULL) {
        data->byref = (PVOID)m_ptrManager->GetColorData();

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::GetColorDataEx"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-AD/2009Aug19/Modified/SU15 //CPP test corrections
    return S_OK;
}

//*******************************Method Header*********************************

//Method Name    :GetCoilData()

//Author         :PATNI/

//Purpose        :

//*****************************************************************************
//+Patni-PJS/2010Nov23/TMSC-REDMINE-774
/*STDMETHODIMP CPqmIpcCom::GetCoilData(
    VARIANT* pData
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //+Patni-AD/2009Aug19/Modified/SU15 //CPP test corrections
    if (m_ptrManager != NULL) {
        pData->byref = (void*)m_ptrManager->GetCoilData();

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::GetCoilData"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-AD/2009Aug19/Modified/SU15 //CPP test corrections

    return S_OK;
}
*/
//-Patni-PJS/2010Nov23/TMSC-REDMINE-774
//*******************************Method Header*********************************

//Method Name    :GetSeqData()

//Author         :PATNI/

//Purpose        :

//*****************************************************************************
STDMETHODIMP CPqmIpcCom::GetSeqData(
    VARIANT* pdata,
    int mode
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //+Patni-AD/2009Aug19/Modified/SU15 //CPP test corrections
    if (m_ptrManager != NULL) {
        pdata->byref = (void*)m_ptrManager->GetSeqParamList(mode);

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::GetSeqData"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-AD/2009Aug19/Modified/SU15 //CPP test corrections

    return S_OK;
}

//*******************************Method Header*********************************

//Method Name    :DuplicateSequence()

//Author         :PATNI/

//Purpose        :

//*****************************************************************************
STDMETHODIMP CPqmIpcCom::DuplicateSequence(
    VARIANT* pData,
    BSTR bstr,
    const int f_index_duplicating_prot
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //+Patni-AD/2009Aug19/Modified/SU15 //CPP test corrections
    if (m_ptrManager != NULL) {

        if (m_ptrManager->DuplicateSequence(pData, bstr, f_index_duplicating_prot)) {
            return S_OK;

        } else {
            return S_FALSE;
        }

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::DuplicateSequence"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-AD/2009Aug19/Modified/SU15 //CPP test corrections
}

//*******************************Method Header*********************************

//Method Name    :GetSeqParamNameList()

//Author         :PATNI/HAR

//Purpose        :Constructor of COM DLL class (CPqmIpcCom)

//*****************************************************************************
STDMETHODIMP CPqmIpcCom::GetSeqParamNameList(
    VARIANT* pData
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //+Patni-AD/2009Aug19/Modified/SU15 //CPP test corrections
    if (m_ptrManager != NULL) {
        pData->byref = (PVOID)m_ptrManager->GetSeqParamNameList();

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::GetSeqParamNameList"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-AD/2009Aug19/Modified/SU15 //CPP test corrections
    return S_OK;
}

//*******************************Method Header*********************************

//Method Name    :DeleteSequence()

//Author         :PATNI/

//Purpose        :

//*****************************************************************************
STDMETHODIMP CPqmIpcCom::DeleteSequence(
    BSTR bstr
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //+Patni-AD/2009Aug19/Modified/SU15 //CPP test corrections
    if (m_ptrManager != NULL) {
        if (m_ptrManager->DeleteAll(bstr)) {

            return S_OK;

        } else {
            return S_FALSE;
        }

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::DeleteSequence"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-AD/2009Aug19/Modified/SU15 //CPP test corrections
}

//*******************************Method Header*********************************

//Method Name    :GetMaxProtocolLimit()

//Author         :PATNI/

//Purpose        :

//*****************************************************************************
STDMETHODIMP CPqmIpcCom::GetMaxProtocolLimit(
    long* MaxProtLimit
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //+Patni-AD/2009Aug19/Modified/SU15 //CPP test corrections
    if (m_ptrManager != NULL) {
        *MaxProtLimit = m_ptrManager->GetMaxProtocolNumber();

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::DeleteSequence"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-AD/2009Aug19/Modified/SU15 //CPP test corrections


    return S_OK;
}

//*******************************Method Header*********************************

//Method Name    :WriteTraceLog()

//Author         :PATNI/

//Purpose        :

//*****************************************************************************
STDMETHODIMP CPqmIpcCom::WriteTraceLog(
    LPCTSTR domain_name,
    unsigned long marker_val,
    LPCTSTR func_name,
    LPCTSTR log_text,
    int detail_trace_log
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    CPQMLogMgr::GetInstance()->WriteTraceLog(domain_name, marker_val, func_name, log_text, detail_trace_log ? true : false);

    return S_OK;
}

//*******************************Method Header*********************************

//Method Name    :WriteOpeLog()

//Author         :PATNI/

//Purpose        :

//*****************************************************************************
STDMETHODIMP CPqmIpcCom::WriteOpeLog(
    WORD opeation_id
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    CPQMLogMgr::GetInstance()->WriteOpeLog(opeation_id);

    return S_OK;
}

//*******************************Method Header*********************************

//Method Name    :WriteEventLog()

//Author         :PATNI/

//Purpose        :

//*****************************************************************************
STDMETHODIMP CPqmIpcCom::WriteEventLog(
    BSTR err_msg,
    int error_level,
    BSTR code_line,
    int error_line,	//Patni-MP/2010May24/Modified/IR-82
    int detail_error_log
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    CString str_err_msg(err_msg);
    CString str_line(code_line);
    //Patni-MP/2010May24/Modified/IR-82
    CPQMLogMgr::GetInstance()->WriteEventLog(str_err_msg, error_level, str_line, error_line, detail_error_log ? true : false);

    return S_OK;
}

//*******************************Method Header*********************************

//Method Name    :PQMDisplayError()

//Author         :PATNI/

//Purpose        :

//*****************************************************************************
STDMETHODIMP CPqmIpcCom::PQMDisplayError(
    BSTR error_msg,
    BSTR file_name,
    UINT line_Number,
    int error_level,
    DWORD msg_id,
    BSTR event_source,
    int f_detail_error_log
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    CString str_err_msg(error_msg);
    CString str_file_name(file_name);
    CString str_event_source(event_source);

    CPQMLogMgr::GetInstance()->DisplayError(str_err_msg, str_file_name, line_Number, (DWORD)error_level, msg_id, str_event_source, f_detail_error_log ? true : false);
    return S_OK;
}


/****************************Method Header************************************

//Method Name   :StartRFLPrescan()

//Author        :PATNI/MSN

//Purpose       :Starts the RFL Prescan done using APC Retry Menu

//*****************************************************************************/
STDMETHODIMP CPqmIpcCom::StartRFLPrescan(
    int scanindex
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //+Patni-Hemant/2011Jan11/Modified/REDMINE-1095_Reopen
    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager && m_ptrManager->StartRFLPrescan(scanindex)) {

        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
        return S_OK;

        //-Patni-Hemant/2011Jan11/Modified/REDMINE-1095_Reopen

    } else {

        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::StartRFLPrescan"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    return S_OK;
}

/****************************Method Header************************************

//Method Name   :StartCFAPrescan()

//Author        :PATNI/MSN

//Purpose       :Starts the CFA Prescan when done using CFA Retry Button
//*****************************************************************************/
STDMETHODIMP CPqmIpcCom::StartCFAPrescan(
    int scanindex
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        //+Patni-PJS/2010Aug10/Modified/TMSC-REDMINE-557
        if (false == m_ptrManager->StartCFAPrescan(scanindex)) {
            return S_FALSE;
        }

        //-Patni-PJS/2010Aug10/Modified/TMSC-REDMINE-557
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::StartCFAPrescan"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    return S_OK;
}

/****************************Method Header************************************

//Method Name   :ChangeShimCenterFrequency()

//Author        :PATNI/MSN

//Purpose       :Sends the Change Shim Center Frequency request to Acqman
//*****************************************************************************/
STDMETHODIMP CPqmIpcCom::ChangeShimCenterFrequency(
    double fsyvalue
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        m_ptrManager->ChangeShimCenterFrequency(fsyvalue);
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::ChangeShimCenterFrequency"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    return S_OK;
}

/****************************Method Header************************************

//Method Name   :SetAPCHoldValue()

//Author        :PATNI/MSN

//Purpose       :Sets the APC Hold Value
//*****************************************************************************/
STDMETHODIMP CPqmIpcCom::SetAPCHoldValue(
    BOOL apcholdvalue
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        m_ptrManager->SetAPCHoldValue(apcholdvalue);
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::SetAPCHoldValue"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    return S_OK;
}

//*******************************Method Header*********************************

//Method Name    :GetAPCHoldValue()

//Author         :PATNI/

//Purpose        :

//*****************************************************************************
STDMETHODIMP CPqmIpcCom::GetAPCHoldValue(
    BOOL* holdvalue
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    return S_OK;
}

/****************************Method Header************************************

//Method Name   :UpdateGUIControlsInDB()

//Author        :PATNI/JS

//Purpose       :For updating controls in PQM GUI
//*****************************************************************************/
STDMETHODIMP CPqmIpcCom::UpdateGUIControlsInDB(
    VARIANT* pData,
    BSTR f_seriesid
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        if (!m_ptrManager->UpdateGUIControlsInDB(pData, f_seriesid)) {
            return S_FALSE;

        } else {
            return S_OK;
        }

        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::UpdateGUIControlsInDB"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
}

//*******************************Method Header*********************************

//Method Name    :SendGateModeInfo()

//Author         :PATNI/HAR - LK

//Purpose        :

//*****************************************************************************
STDMETHODIMP CPqmIpcCom::SendGateModeInfo(
    int f_gate_mode
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        m_ptrManager->SendGateModeInfo(f_gate_mode);
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::SendGateModeInfo"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    return S_OK;
}

//*******************************Method Header*********************************

//Method Name    :PutInjectTimeInDB()

//Author         :PATNI/HAR

//Purpose        :

//*****************************************************************************
STDMETHODIMP CPqmIpcCom::PutInjectTimeInDB(
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        m_ptrManager->PutInjectTimeInDB();
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::PutInjectTimeInDB"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    return S_OK;
}

//*******************************Method Header*********************************

//Method Name    :SetMultiCovCaution()

//Author         :PATNI/ LK

//Purpose        :

//*****************************************************************************
// + Patni-Sribanta/2010Jun09/Modified/MVC8032
/*
STDMETHODIMP CPqmIpcCom::SetMultiCovCaution(
    BOOL f_multicovcaution
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        m_ptrManager->SetMultiCovCaution(f_multicovcaution);
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::SetMultiCovCaution"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    return S_OK;
}
*/

// + Patni-Sribanta/2010Jun09/Modified/MVC8032

//*******************************Method Header*********************************

//Method Name    :SetReceiverCoilID()

//Author         :PATNI/

//Purpose        :

//*****************************************************************************
STDMETHODIMP CPqmIpcCom::SetReceiverCoilID(
    long f_receiver_coilid
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        m_ptrManager->SetReceiverCoilID(f_receiver_coilid);
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::SetReceiverCoilID"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    return S_OK;
}

//*******************************Method Header*********************************

//Method Name    :SetAnatomy()

//Author         :PATNI/

//Purpose        :

//*****************************************************************************
STDMETHODIMP CPqmIpcCom::SetAnatomy(
    int f_anatomy_id
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        m_ptrManager->SetScanAnatomy(f_anatomy_id);
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::SetAnatomy"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    return S_OK;
}

//*******************************Method Header*********************************

//Method Name    :GetSeriesCount()

//Author         :PATNI/

//Purpose        :

//*****************************************************************************
STDMETHODIMP CPqmIpcCom::GetSeriesCount(
    int* seriescnt
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        *seriescnt = m_ptrManager->GetSeriesCount();
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::GetSeriesCount"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    return S_OK;
}

//*******************************Method Header*********************************

//Method Name    :PQMDisplayScanError()

//Author         :PATNI/

//Purpose        :

//*****************************************************************************
STDMETHODIMP CPqmIpcCom::PQMDisplayScanError(
    int f_errorid,
    BSTR f_errstr, // Added By KT/ACE-2/15-06-2009/Multilingualisation
    int f_errorlevel,
    BSTR f_errorsource
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    CString str_file_name(f_errorsource);

    CPQMLogMgr::GetInstance()->DisplayScanError(f_errorid, f_errstr, f_errorlevel, str_file_name);

    return S_OK;
}

//*******************************Method Header*********************************

//Method Name    :SendSequencesToMemory()

//Author         :PATNI/

//Purpose        :

//*****************************************************************************
STDMETHODIMP CPqmIpcCom::SendSequencesToMemory(
    VARIANT* m_snd_seq_list
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        m_ptrManager->SendSequencesToMemory(m_snd_seq_list);
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::SendSequencesToMemory"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    return S_OK;
}

//*******************************Method Header*********************************

//Method Name    :SetPQMMode()

//Author         :PATNI/

//Purpose        :

//*****************************************************************************
STDMETHODIMP CPqmIpcCom::SetPQMMode(int mode)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        m_ptrManager->SetPQMMode(mode);
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::SetPQMMode"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    return S_OK;
}

//*******************************Method Header*********************************

//Method Name    :SetExeAutoVoice()

//Author         :PATNI/

//Purpose        :

//*****************************************************************************
STDMETHODIMP CPqmIpcCom::SetExeAutoVoice(
    int l_exe_auto_voice
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        m_ptrManager->SetExeAutoVoice(l_exe_auto_voice);
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::SetExeAutoVoice"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    return S_OK;
}

/****************************Method Header************************************

//Method Name   :UpdateGUIControlsInVarfiledDB()

//Author        :PATNI/JS

//Purpose       :For updating controls in PQM GUI to Varfield DB

//*****************************************************************************/
STDMETHODIMP CPqmIpcCom::UpdateGUIControlsInVarfieldDB(
    VARIANT* pData,
    int f_scan_index
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        if (!m_ptrManager->UpdateGUIControlsInVarfieldDB(pData, f_scan_index)) {
            return S_FALSE;

        } else {
            return S_OK;
        }

        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::UpdateGUIControlsInVarfieldDB"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
}


//Patni-HAR/2010Sep20/Modified/Rename function from SendIndex() to DeleteProtocolFromVarField()
STDMETHODIMP CPqmIpcCom::DeleteProtocolFromVarField(
    int f_index
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        //Patni-HAR/2010Sep20/Modified/Rename function from SendIndex() to DeleteProtocolFromVarField()
        m_ptrManager->DeleteProtocolFromVarField(f_index);
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::SendIndex"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    return S_OK;
}

//+Patni-NP/2009Sep15/Added/MVC004766
STDMETHODIMP CPqmIpcCom::GetDBDTCurrentMode(
    int f_index , int* f_mode
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        m_ptrManager->GetDBDTCurrentMode(f_index, f_mode);

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::GetDBDTCurrentMode"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    return S_OK;
}
//-Patni-NP/2009Sep15/Added/MVC004766

STDMETHODIMP CPqmIpcCom::DuplicateProtocol(
    int f_from_pos,
    int f_to_pos,
    int f_group_id
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    int l_acq_order = -1;

    //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        l_acq_order = m_ptrManager->DuplicateProtocol(f_from_pos, f_to_pos, f_group_id);

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::DuplicateProtocol"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    return l_acq_order;
}

STDMETHODIMP CPqmIpcCom::DuplicateSequenceInVarfield(int f_pos_list)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        m_ptrManager->DuplicateSequenceInVarfield(f_pos_list);
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::DuplicateSequenceInVarfield"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    return S_OK;
}

STDMETHODIMP CPqmIpcCom::ReArrange(int f_drag_index, int f_drop_index)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        m_ptrManager->ReArrange(f_drag_index, f_drop_index);
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::ReArrange"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    return S_OK;

}
/****************************Method Header************************************
//Method Name   :SetShimmWBCoil()
//Author        :PATNI/LK
//Purpose       :Sets the ShimSCan UseWB menu item status
//****************************************************************************/
STDMETHODIMP CPqmIpcCom::SetShimmWBCoil(
    BOOL f_shim_wb_coil
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        m_ptrManager->SetShimmWBCoil(f_shim_wb_coil);
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::SetShimmWBCoil"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    return S_OK;
}

/****************************Method Header************************************
//Method Name   :GetSeqDataofDisplaySequences()
//Author        :PATNI/LK - JS
//Purpose       :Retrieves the first 10 display sequences and retursn it to GUI
//****************************************************************************/
STDMETHODIMP CPqmIpcCom::GetSeqDataofDisplaySequences(
    VARIANT* pdata,
    int f_mode,
    int f_firstseq
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        pdata->byref = (void*)m_ptrManager->GetSeqDataofDisplaySequences(f_mode, f_firstseq);
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::GetSeqDataofDisplaySequences"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    return S_OK;
}

/****************************Method Header************************************
//Method Name   :SetShimFSYFlag()
//Author        :PATNI/LK
//Purpose       :Sets the SHimFsy flag of PQM's Shared Memory
//****************************************************************************/
STDMETHODIMP CPqmIpcCom::SetShimFSYFlag(
    int f_shimfsy_flag
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        m_ptrManager->SetShimFsyFlag(f_shimfsy_flag);
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::SetShimFSYFlag"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    return S_OK;
}


/****************************Method Header************************************
//Method Name   :SetPrescanHook()
//Author        :PATNI/LK
//Purpose       :Sets the SHimFsy flag of PQM's Shared Memory
//****************************************************************************/
STDMETHODIMP CPqmIpcCom::SetPrescanHook(
    BOOL f_prescan_hook
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        m_ptrManager->SetPrescanHook(f_prescan_hook);
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::SetPrescanHook"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    return S_OK;
}




/****************************Method Header************************************
//Method Name   :PQMManualPrescanClose()
//Author        :PATNI/LK
//Purpose       :Sets the SHimFsy flag of PQM's Shared Memory
//****************************************************************************/
STDMETHODIMP CPqmIpcCom::PQMManualPrescanClose()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        m_ptrManager->PQMManualPrescanClose();
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::PQMManualPrescanClose"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    return S_OK;
}


/****************************Method Header************************************
//Method Name   :PQMManualPrescanApply()
//Author        :PATNI/LK
//Purpose       :Sets the SHimFsy flag of PQM's Shared Memory
//****************************************************************************/
STDMETHODIMP CPqmIpcCom::PQMManualPrescanApply(VARIANT* f_pdata)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        m_ptrManager->PQMManualPrescanApply(f_pdata);
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::PQMManualPrescanApply"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    return S_OK;
}

/****************************Method Header************************************
//Method Name   :GetManualPreScanDefaultData()
//Author        :PATNI/LK
//Purpose       :Get the default values from study file for ManualPrescan.
//****************************************************************************/
STDMETHODIMP CPqmIpcCom::GetManualPreScanDefaultData(VARIANT** f_pdata)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    //Patni-AMT/2009Jul06/Modified/ Return corrected.
    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        if (m_ptrManager->GetManualPreScanDefaultData(f_pdata) == 0) {
            return S_OK;

        } else {
            return S_FALSE;
        }

        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::GetManualPreScanDefaultData"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
}

/****************************Method Header************************************
//Method Name   :GetMRSDefaultValue()
//Author        :PATNI/LK
//Purpose       :Sets the SHimFsy flag of PQM's Shared Memory
//****************************************************************************/
STDMETHODIMP CPqmIpcCom::GetMRSDefaultValue(VARIANT** f_data)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    int l_result = 0;

    if (m_ptrManager) {
        l_result = m_ptrManager->GetMrsDefaultData(f_data);
    }

    return l_result;
}




/****************************Method Header************************************
//Method Name   :MrsScanStartUiCB()
//Author        :PATNI/LK
//Purpose       :Sets the SHimFsy flag of PQM's Shared Memory
//****************************************************************************/
STDMETHODIMP CPqmIpcCom::MrsScanStartUiCB(VARIANT* f_data)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    int l_result = 0;
    SMRSScan_s* l_data = (SMRSScan_s*)f_data->byref;

    if (m_ptrManager && l_data) {
        l_result = m_ptrManager->MrsScanStartUiCB(l_data);
    }

    return l_result;
}




/****************************Method Header************************************
//Method Name   :ProconReqUiCB()
//Author        :PATNI/SS
//Purpose       :
//****************************************************************************/
//Patni-Ajay/2010July23/Added/TMSC-REDMINE-443
STDMETHODIMP CPqmIpcCom::ProconReqUiCB(int f_tag, int f_acq_order , int f_maskValue)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    int l_result = 0;
    // TODO: Add your implementation code here

    if (m_ptrManager) {
        //Patni-Ajay/2010July23/Added/TMSC-REDMINE-443
        l_result =	m_ptrManager->ProconReqUiCB(f_tag, f_acq_order, f_maskValue);
    }

    return l_result;
}




/****************************Method Header************************************
//Method Name   :PqmSeverConfigToProCon
//Author        :PATNI/SS
//Purpose       :
//****************************************************************************/
STDMETHODIMP CPqmIpcCom::PqmSeverConfigToProCon(int f_tag, int f_index, int* f_index_list, int f_size)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    int l_result = 0;

    // TODO: Add your implementation code here
    if (m_ptrManager) {
        l_result =	m_ptrManager->PqmSeverConfigToProCon(f_tag, f_index, f_index_list, f_size);
    }

    return l_result;
}



/****************************Method Header************************************
//Method Name   :SetAutoPlan
//Author        :PATNI/Ashish SM3
//Purpose       :
//****************************************************************************/
STDMETHODIMP CPqmIpcCom::SetAutoPlan(int f_auto_plan)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        m_ptrManager->SetAutoPlan(f_auto_plan);
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::SetAutoPlan"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    return S_OK;
}



/****************************Method Header************************************
//Method Name   :SetPlanAndScan
//Author        :PATNI/Ashish Sm3
//Purpose       :
//****************************************************************************/
STDMETHODIMP CPqmIpcCom::SetPlanAndScan(int f_plan_scan)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        m_ptrManager->SetPlanAndScan(f_plan_scan);
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::SetPlanAndScan"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    return S_OK;
}

/****************************Method Header************************************
//Method Name   :SetSkipRgn
//Author        :PATNI/Ashish Sm3
//Purpose       :
//****************************************************************************/
STDMETHODIMP CPqmIpcCom::SetSkipRgn(int f_skip_rgn)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        m_ptrManager->SetSkipRgn(f_skip_rgn);
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::SetSkipRgn"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    return S_OK;
}

/****************************Method Header************************************
//Method Name   :SetAllCoverageRgn
//Author        :PATNI/Ashish Sm3
//Purpose       :
//****************************************************************************/
STDMETHODIMP CPqmIpcCom::SetAllCoverageRgn(int f_all_cov_rgn)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        m_ptrManager->SetAllCoverageRgn(f_all_cov_rgn);
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::SetAllCoverageRgn"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    return S_OK;
}

/****************************Method Header************************************
//Method Name   :SetPrepWbCoil
//Author        :PATNI/Ashish Sm3
//Purpose       :
//****************************************************************************/
STDMETHODIMP CPqmIpcCom::SetPrepWbCoil(int f_Prep_Wb_Coil)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        m_ptrManager->SetPrepWbCoil(f_Prep_Wb_Coil);
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::SetPrepWbCoil"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    return S_OK;
}


/****************************Method Header************************************
//Method Name   :SetAutoView()
//Author        :PATNI/SS
//Purpose       :
//****************************************************************************/
STDMETHODIMP CPqmIpcCom::SetAutoView(int)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here

    return S_OK;
}


//************************************Method Header************************************
// Method Name  : SendElectrodeType
// Author       : PATNI/ ASHISH SM3
// Purpose      :
//***********************************************************************************

STDMETHODIMP CPqmIpcCom::SendElectrodeType(int f_electrode_type)
{
    int l_result = 0;

    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        l_result = m_ptrManager->SendElectrodeType(f_electrode_type);
    }

    return l_result;
}

/****************************Method Header************************************
//Method Name   :SetActiveDialog()
//Author        :PATNI/Ashish SM3
//Purpose       :
//****************************************************************************/
HRESULT CPqmIpcCom::SetActiveDialog(int f_nDialogType, BOOL f_bSet)
{
    //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        m_ptrManager->SetActiveDialog(f_nDialogType, f_bSet);

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::SetActiveDialog"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    return S_OK;
}



/****************************Method Header************************************
//Method Name   :SetScanOffsetValue()
//Author        :PATNI/MSN
//Purpose       :
//****************************************************************************/
STDMETHODIMP CPqmIpcCom::SetScanOffsetValue(int index, float f_scanoffset_x, float f_scanoffset_y, float f_scanoffset_z, const BOOL f_set_mdoe)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        m_ptrManager->SetScanOffsetValue(index, f_scanoffset_x, f_scanoffset_y, f_scanoffset_z, f_set_mdoe);
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::SetScanOffsetValue"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    return S_OK;
}
//SM3_Mangesh_Start
//*******************************Method Header*********************************

//Method Name    :GetContrastAgent()

//Author         :PATNI/Mangesh

//Purpose        :

//*****************************************************************************
//+Patni-ARD/2010Apr08/Commented/JaFT# IR-100 code review
/*
STDMETHODIMP CPqmIpcCom::GetContrastAgent(
    VARIANT** pData
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        m_ptrManager->GetContrastAgent(pData);
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::GetContrastAgent"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    return S_OK;

}
*/
//-Patni-ARD/2010Apr08/Commented/JaFT# IR-100 code review

//*******************************Method Header*********************************

//Method Name    :GetScanAnatomy()

//Author         :PATNI/Mangesh

//Purpose        :

//*****************************************************************************
STDMETHODIMP CPqmIpcCom::GetScanAnatomy(
    VARIANT** pData
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        m_ptrManager->GetScanAnatomy(pData);
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::GetScanAnatomy"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    return S_OK;

}

//SM3_Mangesh_End


//+Patni-Ajay/2010July23/Added/TMSC-REDMINE-443
//*******************************Method Header*********************************
//Method Name    :GetPresetItem()
//Author         :
//Purpose        :
//*****************************************************************************
STDMETHODIMP CPqmIpcCom::GetPresetItem(
    VARIANT** pData
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())


    if (m_ptrManager) {
        m_ptrManager->GetPresetItem(pData);

    } else {
        return S_FALSE;
    }

    return S_OK;

}
//-Patni-Ajay/2010July23/Added/TMSC-REDMINE-443
/****************************Method Header*************************************
//Method Name   :GetCoilSelectionData()
//Author        :PATNI/HAR
//Purpose       :
//****************************************************************************/
STDMETHODIMP CPqmIpcCom::GetCoilSelectionData(
    VARIANT* f_pdata,
    int f_mode)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        f_pdata->byref = (void*)m_ptrManager->GetCoilSelectionData(f_mode);
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::GetCoilSelectionData"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;

    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    return S_OK;
}

/****************************Method Header*************************************
//Method Name   :SendMessageToAcqManForCoil()
//Author        :PATNI/HAR
//Purpose       :
//****************************************************************************/
STDMETHODIMP CPqmIpcCom::SendMessageToAcqManForCoil(
    int f_mode,
    int f_acq_order
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        m_ptrManager->SendMessageToAcqManForCoil(f_mode, f_acq_order);
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::SendMessageToAcqManForCoil"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    return S_OK;
}

/****************************Method Header*************************************
//Method Name   :GetConnectCoilInfo()
//Author        :PATNI/HAR
//Purpose       :
//****************************************************************************/
STDMETHODIMP CPqmIpcCom::GetConnectCoilInfo(
    VARIANT* f_data
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        f_data->byref = m_ptrManager->GetConnectCoilInfo();
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::GetConnectCoilInfo"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    return S_OK;
}

/****************************Method Header*************************************
//Method Name   :SendCoilSelReadyToIPC()
//Author        :PATNI/HAR
//Purpose       :
//****************************************************************************/
STDMETHODIMP CPqmIpcCom::SendCoilSelReadyToIPC(
    int f_index
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        m_ptrManager->SendCoilSelReadyToIPC(f_index);
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        ////WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::SendCoilSelReadyToIPC"),
        //_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    return S_OK;
}

/****************************Method Header*************************************
//Method Name   :GetShowCoilList()
//Author        :PATNI/HAR
//Purpose       :
//****************************************************************************/
STDMETHODIMP CPqmIpcCom::GetShowCoilList(
    VARIANT* f_data
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    f_data->byref = m_ptrManager->GetShowCoilInfo();
    return S_OK;
}

STDMETHODIMP CPqmIpcCom::GetCoordinateList(
    VARIANT* f_data
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    f_data->byref = m_ptrManager->GetCoordinateList();
    return S_OK;
}


/****************************Method Header*************************************
//Method Name   :GetSelectCoilList()
//Author        :PATNI/HAR
//Purpose       :
//****************************************************************************/
STDMETHODIMP CPqmIpcCom::GetSelectCoilList(
    VARIANT* f_data
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        f_data->byref = m_ptrManager->GetSelectCoilInfo();
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::GetSelectCoilList"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    return S_OK;
}

/****************************Method Header*************************************
//Method Name   :GetSelectCoilList()
//Author        :PATNI/HAR
//Purpose       :
//****************************************************************************/
STDMETHODIMP CPqmIpcCom::SendSelectedCoilListToIPC(
    VARIANT* pData,
    int f_protocol
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        m_ptrManager->SendSelectedCoilListToIPC(pData, f_protocol);
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::SendSelectedCoilListToIPC"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    return S_OK;
}

STDMETHODIMP CPqmIpcCom::SendConnectedCoilListToIPC(
    VARIANT* pData,
    int f_protocol
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        m_ptrManager->SendConnectedCoilListToIPC(pData, f_protocol);
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::SendConnectedCoilListToIPC"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    return S_OK;
}

STDMETHODIMP CPqmIpcCom::GetCoilNameFromAcqOrder(int f_acq_order, VARIANT** coilnamefrom)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here


    if (m_ptrManager) {

        m_ptrManager->GetCoilNameFromAcqOrder(f_acq_order, coilnamefrom) ;
    }

    return S_OK;
}
//Nitin coil
STDMETHODIMP CPqmIpcCom::GetCoilNameFromTransmiter(int f_acq_order, VARIANT** coilnamefrom)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here


    if (m_ptrManager) {

        m_ptrManager->GetCoilNameFromTransmiter(f_acq_order, coilnamefrom) ;
    }

    return S_OK;
}
//Nitin coil

//Nitin coil
STDMETHODIMP CPqmIpcCom::GetCoilNameFromReceiver(int f_acq_order, VARIANT** coilnamefrom)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here


    if (m_ptrManager) {

        m_ptrManager->GetCoilNameFromReceiver(f_acq_order, coilnamefrom) ;
    }

    return S_OK;
}
//Nitin coil

STDMETHODIMP CPqmIpcCom::GetInsideLimit(int f_index, int* f_in_side_limit, int* f_out_side_limit, BOOL* f_enable)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        m_ptrManager->GetInsideLimit(f_index, *f_in_side_limit, *f_out_side_limit, *f_enable) ;
    }

    return S_OK;
}

STDMETHODIMP CPqmIpcCom::OnPQMExit()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {

        m_ptrManager->OnPQMExit();
    }

    return S_OK;
}


STDMETHODIMP CPqmIpcCom::SetDBDtCancelFlag(BOOL f_dbdt_flag)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())


    if (m_ptrManager) {

        m_ptrManager->SetDBDtCancelFlag(f_dbdt_flag);
    }

    return S_OK;
}

STDMETHODIMP CPqmIpcCom::SetMoveLocatorMode(BOOL f_flag)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {

        m_ptrManager->SetMoveLocatorMode(f_flag);
    }

    return S_OK;
}
STDMETHODIMP CPqmIpcCom::SetBatchForSARNCoil(BOOL f_flag)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        m_ptrManager->SetBatchForSARNCoil(f_flag);
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::SetBatchForSARNCoil"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    return S_OK;
}

/****************************Method Header*************************************
//Method Name   : GetConnectedCoilListForDisplayFromIPC
//Author        : PATNI/AMT
//Purpose       : Get connected coil list for display at GUI
//****************************************************************************/
STDMETHODIMP CPqmIpcCom::GetConnectedCoilListForDisplayFromIPC(VARIANT* f_data)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {

        f_data->byref = m_ptrManager->GetConnectedCoilListForDisplay();
    }

    return S_OK;
}
/****************************Method Header*************************************
//Method Name   : GetConnectedCoilInformationForGUIDisplay
//Author        : PATNI/MRP
//Purpose       : To display the Coil Info Dialog
//****************************************************************************/
STDMETHODIMP CPqmIpcCom::GetConnectedCoilInformationForGUIDisplay(BOOL f_flag)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    if (m_ptrManager) {

        m_ptrManager->GetConnectedCoilInformationForGUIDisplay();
    }


    return S_OK;
}

STDMETHODIMP CPqmIpcCom::GetControlValueFromDB(VARIANT* f_pdata, int f_scan_index)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        if (m_ptrManager->GetControlValueFromDB(f_pdata, f_scan_index)) {
            return S_OK;

        } else {
            return S_FALSE;
        }

        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::GetControlValueFromDB"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
}

STDMETHODIMP CPqmIpcCom::GetCurrentWSEEditProt(VARIANT* f_data)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        m_ptrManager->GetCurrentWSEEditProt(f_data);
    }

    return S_OK;
}

STDMETHODIMP CPqmIpcCom::GetGateInfoList(VARIANT* pData)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    if (m_ptrManager) {
        m_ptrManager->GetGateInfoList(pData);
    }

    return S_OK;
}

STDMETHODIMP CPqmIpcCom::SendAcqInfoForGate()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    if (m_ptrManager) {
        m_ptrManager->SendAcqInfoForGate();
    }

    return S_OK;
}

STDMETHODIMP CPqmIpcCom::ConnectToAcqMan()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        m_ptrManager->ConnectToAcqMan();
    }

    return S_OK;
}

/****************************Method Header*************************************
//Method Name   :GetSequenceInformation()
//Author        :PATNI/HAR
//Purpose       :
//****************************************************************************/
STDMETHODIMP CPqmIpcCom::GetSequenceInformation(
    VARIANT* f_data,
    int f_acq_order
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        f_data->byref = m_ptrManager->GetSequenceInformation(f_data, f_acq_order);
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::GetSequenceInformation"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    return S_OK;
}

//********************************Method Header********************************
//Method Name   : CheckNextProtocol()
//Author        : PATNI/MRP
//Purpose       : Checks the next protocol for Auto Scan
//*****************************************************************************
STDMETHODIMP CPqmIpcCom::CheckNextProtocol(
    int l_protocol_no,
    BOOL f_call_frm_gui
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    if (m_ptrManager) {

        m_ptrManager->CheckNextProtocol(l_protocol_no, f_call_frm_gui);
    }

    return S_OK;
}

//+Patni-HEMANT/ADDED On 3/31/2009 2:55:35 PM/ PSP/ IDS#202, IDS#192
//
STDMETHODIMP CPqmIpcCom::ConnectToProconFromGUI()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here

    if (m_ptrManager) {

        m_ptrManager->ConnectToProconFromGUI();
    }


    return S_OK;
}
STDMETHODIMP CPqmIpcCom::GetNewSeriesLoids(VARIANT* f_strpatientloid)
{
    if (m_ptrManager) {
        m_ptrManager->GetNewSeriesLoids(f_strpatientloid);
    }

    return S_OK;
}
//-Patni-ARD/2009June1/Added/JFT# PQM-SR GL_SR_SQSaved

//-Patni-HEMANT/ ADDED On3/31/2009 2:55:39 PM/ PSP/ IDS#202, IDS#192
//+ Patni-PJS/2009Jun05/Added/JFT# 251,251,251,IR(5)
//********************************Method Header********************************
//Method Name   : GetCurrentSarInfoToDisplay()
//Author        : PATNI/PJS
//Purpose       : GetCurrent sar info to Display
//*****************************************************************************
//Patni-HAR/2010Jun23/Modified/MVC008324_2
STDMETHODIMP CPqmIpcCom::GetCurrentSarInfoToDisplay(VARIANT** pdata)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    BOOL  l_result = false;

    if (m_ptrManager) {

        l_result = m_ptrManager->GetCurrentSarInfoToDisplay(pdata);
    }

    return l_result;
}

//+Patni-ARD/2009Jun17/Added/JFT# AcqOrder Checking between IPC and GUI
//********************************Method Header********************************
//Method Name   : GetIPCTopMostWaitAcqOrder()
//Author        : PATNI/ARD
//Purpose       : Get Current top most wait protocol from IPC
//*****************************************************************************
STDMETHODIMP CPqmIpcCom::GetIPCTopMostWaitAcqOrder(int* f_acqorder)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        m_ptrManager->GetTopMostWaitAcqOrder(f_acqorder);
        return S_OK;

    } else {
        return S_FALSE;
    }
}
//********************************Method Header********************************
//Method Name   : FileCopy()
//Author        : PATNI/ARD
//Purpose       : 20090309/Added/ACE-1# handling of stabilization
//*****************************************************************************
STDMETHODIMP CPqmIpcCom::FileCopy(BSTR f_existingfilename, BSTR f_newfilename)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    if (m_ptrManager) {

        if (!m_ptrManager->FileCopy(f_existingfilename, f_newfilename)) {
            return S_OK;

        } else {
            return S_FALSE;
        }
    }

    return S_OK;
}


//-Patni-ARD/2009Jun17/Added/JFT# AcqOrder Checking between IPC and GUI
// + Patni HAR / 2009 Jul 16 / Added / With Series defect
STDMETHODIMP CPqmIpcCom::GetStudyPath(VARIANT* f_study_path)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        m_ptrManager->GetStudyPath(f_study_path);
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::GetStudyPath"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    return S_OK;
}
// - Patni HAR / 2009 Jul 16 / Added / With Series defect
//********************************Method Header********************************
//Method Name   : GetHasParent()
//Author        : PATNI/ARD
//Purpose       : 20090722/Added/JFT# 177
//*****************************************************************************
STDMETHODIMP CPqmIpcCom::GetHasParent(int f_acq_order)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        return 	m_ptrManager->GetHasParent(f_acq_order);
    }

    return S_OK;
}


//+Patni-HEMANT/ ADDED On6/12/2009 1:14:13 PM/ ACE-2 / WFDA
STDMETHODIMP CPqmIpcCom::RecvCallBack(VARIANT* messageParam, VARIANT* messageRet, VARIANT* pCustom)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    // TODO: Add your implementation code here
    if (m_ptrManager) {

        void* messageParam_implus = (void*)messageParam;
        void* messageRet_implus = (void*)messageRet;
        void* pCustom_implus = (void*)pCustom;

        //+TMSC-Tanoue/2010Sep03/MVC008324
        try {
            if (!m_ptrManager->RecvCallBackCFW(messageParam_implus, messageRet_implus, pCustom_implus)) {
                return S_OK;

            } else {
                return S_FALSE;
            }

        } catch (...) {
            PQM_TRACE(SYSTEM_FUNC_MARKER, _T("CPqmIpcCom::RecvCallBack"),
                      _T("[EXCEPTION] m_ptrManager->RecvCallBack()"));
            throw;
        }

        //-TMSC-Tanoue/2010Sep03/MVC008324
    }

    return S_OK;
}



STDMETHODIMP CPqmIpcCom::OnPQMDuplicateProtocol(int f_index_to_duplicate, int f_duplicate_index)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    if (m_ptrManager) {

        if (!m_ptrManager->DuplicateProtocol_InScanServer(f_index_to_duplicate, f_duplicate_index)) {
            return S_OK;

        } else {
            return S_FALSE;
        }
    }



    return S_OK;
}

STDMETHODIMP CPqmIpcCom::OnPQMDeleteProtocol(int f_protocol_index)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    if (m_ptrManager) {

        if (!m_ptrManager->DeleteProtocol_InScanServer(f_protocol_index)) {
            return S_OK;

        } else {
            return S_FALSE;
        }
    }

    return S_OK;
}
//-Patni-HEMANT/ ADDED On6/12/2009 1:14:13 PM/ ACE-2 / WFDA


//+ Patni-PJS/2009Jul23/Modify/IDS#442,MVC002699
//********************************Method Header********************************
//Method Name   : GetSarInfoPtr()
//Author        : PATNI/PJS
//Purpose       : GetCurrent sar info pointer
//*****************************************************************************
STDMETHODIMP CPqmIpcCom::GetSarInfoPtr(VARIANT* pdata)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {

        return m_ptrManager->GetSarInfoPtr(pdata);
    }

    return S_FALSE;
}

//********************************Method Header********************************
//Method Name   : SetProtocolIndex()
//Author        : PATNI/PJS
//Purpose       : Sets the Protocol Index
//*****************************************************************************
STDMETHODIMP CPqmIpcCom::SetProtocolIndex(int f_index)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {

        m_ptrManager->SetProtocolIndex(f_index);
        return S_OK;
    }

    return S_FALSE;
}

//- Patni-PJS/2009Jul23/Modify/IDS#442,MVC002699


//+ Patni-PJS/2009Jul29/Modified/MVC0003961
//********************************Method Header********************************
//Method Name   : GetCouchPos()
//Author        : PATNI/PJS
//Purpose       : Get Couch Pos
//*****************************************************************************
STDMETHODIMP CPqmIpcCom::GetCouchPos(VARIANT* f_pData)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {

        m_ptrManager->GetCouchPos(f_pData);
        return S_OK;
    }

    return S_FALSE;
}
//- Patni-PJS/2009Jul29/Modified/MVC0003961


//+Patni-Manishkumar/2009Jul29/Modified/MVC003316
//********************************Method Header********************************
//Method Name   : GetTotalSequeunces()
//Author        : PATNI/Manishkumar
//Purpose       : Get total number of sequences to be transfered.
//*****************************************************************************
STDMETHODIMP CPqmIpcCom::GetTotalSequeunces(VARIANT* f_pVariant)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {

        m_ptrManager->GetTotalSequence(f_pVariant);
        return S_OK;
    }

    return S_FALSE;

}
//-Patni-Manishkumar/2009Jul29/Modified/MVC003316

//********************************Method Header********************************
//Method Name   : GetGUIListProtocolInfo()
//Author        : PATNI/ARD
//Purpose       :
//*****************************************************************************
STDMETHODIMP CPqmIpcCom::GetGUIListProtocolInfo(VARIANT* f_pdata, int f_count_gui_list)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    if (m_ptrManager) {

        //+Patni-HAR/2010Feb24/Modified/MSA0251-00187
        if (!m_ptrManager->GetGUIListProtocolInfo(f_pdata, f_count_gui_list)) {
            return S_FALSE;

        } else {
            return S_OK;
        }

        //-Patni-HAR/2010Feb24/Modified/MSA0251-00187
    }

    return S_FALSE;
}

//-Patni-ARD/2009September7/Added/Prototype 2


//+Patni-Sribanta/2011Jan06/Commented/TMSC-REDMINE-1026

//+Patni-PJS/2009Sep23/Added/MVC4941
//*******************************Method Header*********************************
//Method Name    :InitializeTicks()
//Author         :PATNI/PJS
//Purpose        :Initialize ticks
//*****************************************************************************
STDMETHODIMP CPqmIpcCom::InitializeTicks(
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())


    if (m_ptrManager) {
        m_ptrManager->InitializeTicks();

    } else {
        return S_FALSE;
    }

    return S_OK;
}

//-Patni-Sribanta/2011Jan06/Commented/TMSC-REDMINE-1026

//-Patni-PJS/2009Sep23/Added/MVC4941
//+Patni-Manishkumar/2009Oct12/ADDED/MVC004921/Loc.enable
//*******************************Method Header*********************************
//Method Name    :GetPqmPqIsGlocOk()
//Author         :PATNI/Manishkumar
//Purpose        :Check the status of protocol and enable locator button
//*****************************************************************************
STDMETHODIMP CPqmIpcCom::GetPqmPqIsGlocOk()
{

    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        if (m_ptrManager->GetPqmPqIsGlocOk()) {
            return S_OK;

        } else {
            return S_FALSE;
        }

    } else {
        return S_FALSE;
    }

    return S_OK;
}
//-Patni-Manishkumar/2009Oct12/ADDED/MVC004921/Loc.enable

//+Patni-AMT/2009Oct08/Added/MVC005124-Code Review-MAX channel
//*******************************Method Header*********************************
//Method Name    :GetCoilSysSetting()
//Author         :PATNI/AMT
//Purpose        :Get System setting for coil
//*****************************************************************************
STDMETHODIMP CPqmIpcCom::GetCoilSysSetting(VARIANT* f_data)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    if (m_ptrManager) {
        if (m_ptrManager->GetCoilSysSetting(f_data)) {
            return S_OK;

        } else {
            return S_FALSE;
        }
    }

    return S_FALSE;
}
//+Patni-AMT/2009Oct08/Added/MVC005124-Code Review-MAX channel

//+Patni-ARD/2009Oct28/Added/IR 85 Auto Transfer
//*******************************Method Header*********************************
//Method Name    :GetEnableDisableAutoTransfer()
//Author         :PATNI/ARD
//Purpose        :Returns S_OK if Auto Transfer functionality can be performed
//                else returns S_FALSE
//*****************************************************************************
STDMETHODIMP CPqmIpcCom::GetEnableDisableAutoTransfer()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        if (m_ptrManager->CanEnableAutoTransfer()) {
            return S_OK;

        } else {
            return S_FALSE;
        }
    }

    return S_FALSE;
}
//+Patni-ARD/2009Oct28/Added/IR 85 Auto Transfer

//+Patni-RSG/Added/2009Nov25/MSA0248-0033
STDMETHODIMP CPqmIpcCom::UpdateProtocolDataInSeries(VARIANT* pData, BSTR f_seriesid, int f_prot_index)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        if (!m_ptrManager->UpdateGUIControlsInDB(pData, f_seriesid, f_prot_index)) {
            return S_FALSE;

        } else {
            return S_OK;
        }

        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::UpdateGUIControlsInDB"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    return S_OK;
}
//-Patni-RSG/Added/2009Nov25/MSA0248-0033


//+Patni-DH+ARD/2009Dec03/Added/DeFT# MVC005787+MSA0248-00087
//*******************************Method Header*********************************
//Method Name    :GetPatientScanoffset()
//Author         :PATNI/DH+ARD
//Purpose        :Returns the converted scan offset values for display
//*****************************************************************************
STDMETHODIMP CPqmIpcCom::GetPatientScanoffset(int f_acq_order, VARIANT**  f_scanoffset)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        if (m_ptrManager->GetPatientScanoffset(f_acq_order, f_scanoffset)) {
            return S_OK;

        } else {
            return S_FALSE;
        }
    }

    return S_FALSE ;

}
//-Patni-DH+ARD/2009Dec03/Added/DeFT# MVC005787+MSA0248-00087
//+ MEITEC/2010Apr23/Added/IR86
//*******************************Method Header*********************************
//Method Name    :RaiseEvent()
//Author         :PATNI
//Purpose        :This method calls to the event sink
//*****************************************************************************
bool CPqmIpcCom::RaiseDisplayEvent(
    BSTR f_debugdata
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    bool bRet = true;

    Fire_DisplayDebugData(f_debugdata);

    return bRet;
}

//*******************************Method Header*********************************
//Method Name    :WrapperRaiseEvent()
//Author         :PATNI/
//Purpose        :This method get the message from the Lib
//*****************************************************************************
bool CPqmIpcCom::WrapperRaiseDisplayEvent(
    BSTR f_debugdata,
    void* ptrObj
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    bool bRet = true;
    CPqmIpcCom* mySelf = (CPqmIpcCom*) ptrObj;

    bRet = mySelf->RaiseDisplayEvent(f_debugdata);

    return bRet;
}

STDMETHODIMP CPqmIpcCom::StartPQMDebug(int f_debugOption, UINT f_buttonmask)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        if (m_ptrManager->StartPQMDebug(f_debugOption, f_buttonmask)) {
            return S_OK;

        } else {
            return S_FALSE;
        }
    }

    return S_FALSE ;
}

STDMETHODIMP CPqmIpcCom::PqmDebugStatusLogOpen()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        if (m_ptrManager->PqmDebugStatusLogOpen()) {
            return S_OK;

        } else {
            return S_FALSE;
        }
    }

    return S_FALSE ;
}
//- MEITEC/2010Apr23/Added/IR86

//+Patni-RAJ/2010Jan18/Added/PH#3#/IR86/DebugWindow
STDMETHODIMP CPqmIpcCom::SelectListandTotalCnt(int* f_selected_list, int f_size)
{
    // TODO: Add your implementation code here
    int  l_result = 0;

    if (m_ptrManager) {
        m_ptrManager->SelectListandTotalCnt(f_selected_list, f_size);
    }

    return l_result;
}
//-Patni-RAJ/2010Jan18/Added/PH#3#/IR86/DebugWindow


//+Patni-Manish/2010Jan11/Added/Ph#3#IR-122/RemoteConsole
STDMETHODIMP CPqmIpcCom::RemoteCheckActiveFlag()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //+ MEITEC/2010Apr08/Modified/Ph#3 IR-122/RemoteConsole
    if (!m_ptrManager) {
        return S_FALSE;
    }

    return m_ptrManager->RemoteCheckActiveFlag() ? S_OK : S_FALSE;
    //- MEITEC/2010Apr08/Modified/Ph#3 IR-122/RemoteConsole
}
//-Patni-Manish/2010Jan11/Added/Ph#3#IR-122/RemoteConsole
//+Patni-DKH/2010Jan14/Added/IR-87,Plan Duplicator
//*****************************************************************************
//Method Name   : InitPlanDuplicateSettings
//Author        : PATNI\DKH
//Purpose       : To initialize plan duplicate settings
//*****************************************************************************
STDMETHODIMP CPqmIpcCom::InitPlanDuplicateSettings()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        if (!(m_ptrManager->InitWithDefaultSettings())) {
            return S_FALSE;

        } else {
            return S_OK;
        }

    } else {
        return S_FALSE;
    }

    return S_OK;
}

//**************************************************************************
//Method Name   : ReadPlanDuplicateSettings
//Author        : PATNI\DKH
//Purpose       : To Read plan duplicate settings
//**************************************************************************
STDMETHODIMP CPqmIpcCom::ReadPlanDuplicateSettings(
    VARIANT** f_data
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        if (!m_ptrManager->ReadPlanDuplicateSettings(f_data)) {
            return S_FALSE;

        } else {
            return S_OK;
        }

    } else {
        return S_FALSE;
    }

    return S_OK;
}

//**************************************************************************
//Method Name   : WritePlanDuplicateMask
//Author        : PATNI\DKH
//Purpose       : To write plan duplicate mask
//**************************************************************************
STDMETHODIMP CPqmIpcCom::WritePlanDuplicateMask(
    long f_imask , BOOL f_bsaveflag
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        if (!m_ptrManager->WritePlanDuplicateMask(f_imask, f_bsaveflag)) {
            return S_FALSE;

        } else {
            return S_OK;
        }

    } else {
        return S_FALSE;
    }

    return S_OK;
}
//-Patni-DKH/2010Jan14/Added/IR-87,Plan Duplicator
//+Patni-DKH/2010Jan29/Added/PH#3#/FBI
//**************************************************************************
//Method Name   : SetPqmPmDelay
//Author        : PATNI\DKH
//Purpose       :
//**************************************************************************
STDMETHODIMP CPqmIpcCom::SetPqmPmDelay(int f_delay1, int f_delay2)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        if (!m_ptrManager->SetPqmPmDelay(f_delay1, f_delay2)) {
            return S_FALSE;

        } else {
            return S_OK;
        }

    } else {
        return S_FALSE;
    }

    return S_OK;
}
//-Patni-DKH/2010Jan29/Added/PH#3#/FBI

//+Patni-HAR/2010Jan29/Modified/DeFT#MVC006364
//****************************Method Header************************************
//Method Name   : SetChildThreadStatusToIPC()
//Author        : PATNI\HAR
//Purpose       :
//*****************************************************************************
STDMETHODIMP CPqmIpcCom::SetChildThreadStatusToIPC(
    int* f_pData,
    int f_count,
    int f_child_thread_status
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        if (!m_ptrManager->SetChildThreadStatusToIPC(f_pData, f_count, f_child_thread_status)) {
            return S_FALSE;

        } else {
            return S_OK;
        }

    } else {

        return S_FALSE;
    }
}
//-Patni-HAR/2010Jan29/Modified/DeFT#MVC006364


//********************************Method Header********************************
//Method Name   : SetFirstWaitProtForAutoScan()
//Author        :
//Purpose       :
//*****************************************************************************
STDMETHODIMP CPqmIpcCom::SetFirstWaitProtForAutoScan(const int f_acqorder)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        m_ptrManager->SetFirstWaitProtForAutoScan(f_acqorder);
        return S_OK;

    } else {
        return S_FALSE;
    }
}

//+Patni-PJS/2010Mar11/Added/DeFT# MCM0229-00178
//**************************************************************************
//Method Name   :  GetScanTimeForCurrProt()
//Author        :  PATNI/PJS
//Purpose       :  Get scan time for the current scan protocol.
//**************************************************************************
STDMETHODIMP CPqmIpcCom::GetScanTimeForCurrProt(VARIANT* f_pdata)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        if (m_ptrManager->GetScanTimeForCurrProt(f_pdata)) {
            return S_OK;

        } else {
            return S_FALSE;
        }

    } else {
        return S_FALSE;
    }
}
//-Patni-PJS/2010Mar11/Added/DeFT# MCM0229-00178

//+Patni-HAR/2010Feb24/Added/MSA0251-00187
//*****************************************************************************
//Method Name   : SetScanProtocolInfoFromGUI
//Author        : PATNI\HAR
//Purpose       :
//*****************************************************************************
STDMETHODIMP CPqmIpcCom::SetScanProtocolInfoFromGUI(VARIANT* pData)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    if (m_ptrManager) {
        if (FALSE == m_ptrManager->SetScanProtocolInfoFromGUI(pData)) {
            return S_FALSE;
        }
    }

    return S_OK;
}
//-Patni-HAR/2010Feb24/Added/MSA0251-00187//+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : GetMagneticCenterPos
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::GetMagneticCenterPos(VARIANT** f_data)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here

    if (m_ptrManager) {

        m_ptrManager->GetMagneticCenterPosForCoilSel(f_data) ;

        return S_OK;
    }

    return S_FALSE;
}
//-Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement


//+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : GetCDSDebugParam
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::GetCDSDebugParam(VARIANT** f_compass_debug_param)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here

    if (m_ptrManager) {

        m_ptrManager->GetCDSDebugParam(f_compass_debug_param) ;

        return S_OK;
    }

    return S_FALSE;
}
//-Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement


//+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : SetASCOMPASSParameter
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::SetASCOMPASSParameter(VARIANT** f_cdscompasssetting_param)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here

    if (m_ptrManager) {

        m_ptrManager->SetASCOMPASSParameter(f_cdscompasssetting_param) ;

        return S_OK;
    }

    return S_FALSE;
}
//-Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement


//+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : GetASCOMPASSParameter
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::GetASCOMPASSParameter(VARIANT** f_cdscompasssetting_param)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here

    if (m_ptrManager) {

        m_ptrManager->GetASCOMPASSParameter(f_cdscompasssetting_param) ;

        return S_OK;
    }

    return S_FALSE;
}
//-Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement


//+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : OnOffCDSDebug
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::OnOffCDSDebug(BOOL f_OnOff)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    if (m_ptrManager) {
        m_ptrManager->OnOffCDSDebug(f_OnOff) ;
        return S_OK;
    }

    return S_FALSE;
}
//-Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement

//+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : OnOffAsCompass
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::OnOffAsCompass(int f_protocol, BOOL f_OnOff)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    if (m_ptrManager) {
        if (m_ptrManager->OnOffAsCompass(f_protocol, (f_OnOff > 0))) {
            return S_OK;
        }
    }

    return S_FALSE;
}
//-Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement


//+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : CDSDebugClosed
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::CDSDebugClosed(VARIANT** f_compass_debug_param, BOOL f_set)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    // TODO: Add your implementation code here
    if (m_ptrManager) {
        if (m_ptrManager->CDSDebugClosed(f_compass_debug_param, (f_set > 0))) {
            return S_OK;
        }
    }

    return S_FALSE;
}
//-Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement


//+Patni-Hemant/2010Feb10/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : CoilSelectionClosed
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::CoilSelectionClosed(BOOL f_ok_pressed)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    if (m_ptrManager) {
        m_ptrManager->CoilSelectionClosed(f_ok_pressed);
        return S_OK;
    }

    return S_FALSE;
}
//-Patni-Hemant/2010Feb10/Added/ACE-Phase#3/CDS Requirement

//+Patni-Hemant/2010Feb22/Added/ACE-Phase#3/CDS Requirement
STDMETHODIMP CPqmIpcCom::SetCDSCheckCoil(int f_check)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    if (m_ptrManager) {
        m_ptrManager->SetCDSCheckCoilFlag(f_check);
        return S_OK;
    }

    return S_FALSE;
}
//-Patni-Hemant/2010Feb22/Added/ACE-Phase#3/CDS Requirement

//+Patni-Hemant/2010Feb23/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : GetASCompassFlag
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::GetASCompassFlag(int f_acqorder, BOOL* f_compass_flag)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    if (m_ptrManager) {
        if (m_ptrManager->GetASCompassFlag(f_acqorder, f_compass_flag)) {
            return S_OK;
        }
    }

    return S_FALSE;
}
//-Patni-Hemant/2010Feb23/Added/ACE-Phase#3/CDS Requirement
//+Patni-PJS/2010Apr22/Added/JaFT# MVC007277
//*****************************************************************************
//Method Name   : GetCouchMethodForProt
//Author        : PATNI\PJS
//Purpose       : This function is used to get the couch method for the given
//                protocol.
//*****************************************************************************
STDMETHODIMP CPqmIpcCom::GetCouchMethodForProt(int f_acq_order, VARIANT* f_pdata)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    if (m_ptrManager) {
        if (FALSE == m_ptrManager->GetCouchMethodForProt(f_acq_order, f_pdata)) {
            return S_FALSE;
        }
    }

    return S_OK;
}
//-Patni-PJS/2010Apr22/Added/JaFT# MVC007277

//+Patni-RUP/2010Apr21/Added/To Fix TMSC-REDMINE-68 defect
//******************************************************************************
//Function Name:- OnShowHidePQM()
//Author       :- PATNI\RUP
//Purpose      :- This function is called when PQM GUI is hidden or shown
//******************************************************************************
STDMETHODIMP CPqmIpcCom::OnShowHidePQM(BOOL f_show_flag)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        m_ptrManager->OnShowHidePQM(f_show_flag);
        return S_OK;
    }

    return S_FALSE;
}
//-Patni-RUP/2010Apr21/Added/To Fix TMSC-REDMINE-68 defect


//+Patni-Hemant/2010Jun7/Added/MaFT/MVC008166
//************************************Method Header************************************
// Method Name  : SetAvSpdrMap
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::SetAvSpdrMap(BOOL f_flag_val)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        m_ptrManager->SetAvSpdrMap(f_flag_val);
        return S_OK;
    }

    return S_FALSE;
}
//-Patni-Hemant/2010Jun7/Added/MaFT/MVC008166

//************************************Method Header************************************
// Method Name  : GetAutoInsertedSeqDataToDisplay
// Author       : PATNI/AM
// Purpose      :
//*************************************************************************************

STDMETHODIMP CPqmIpcCom::GetAutoInsertedSeqDataToDisplay(
    VARIANT* pdata, int* f_insertedmappos, int* f_target_pos,
    int* const f_is_shim_required, int* const f_is_map_required
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        if (FALSE == m_ptrManager->GetAutoInsertedSeqDataToDisplay(&pdata, f_insertedmappos, f_target_pos, f_is_shim_required, f_is_map_required)) {
            return S_FALSE;
        }
    }

    return S_OK;
}


STDMETHODIMP CPqmIpcCom::GetTargetProtInformation(
    int* f_target_pos
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        m_ptrManager->GetTargetProtInformation(f_target_pos);
    }

    return S_OK;
}



//+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
STDMETHODIMP CPqmIpcCom::GetToolMenuXMLData(VARIANT** pData)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here

    if (m_ptrManager) {
        if (m_ptrManager->GetToolMenuXMLData(pData)) {
            return S_OK;
        }
    }

    return S_FALSE;
}
//-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu


//+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
STDMETHODIMP CPqmIpcCom::GetSFTMenuXMLData(VARIANT** pData)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    if (m_ptrManager) {
        if (m_ptrManager->GetSFTMenuXMLData(pData)) {
            return S_OK;
        }
    }

    return S_FALSE;
}
//-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu


//+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
STDMETHODIMP CPqmIpcCom::GetSFTParamValue(VARIANT** f_param, int f_acq_order)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {

        m_ptrManager->GetSFTParamValue(f_param, f_acq_order) ;
    }

    return S_OK;
}
//-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu

//+Patni-PJS/2010Oct26/Added/WFDA/IR-97
//*****************************************************************************
//Method Name   : CopyWFDAProtocolTags
//Author        : PATNI\PJS
//Purpose       :
//*****************************************************************************
//IR-97
/*STDMETHODIMP CPqmIpcCom::CopyWFDAProtocolTags(int f_index_being_dup, int f_index_for_dup)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    if (m_ptrManager) {
        if (S_FALSE == m_ptrManager->CopyWFDAProtocolTags(f_index_being_dup, f_index_for_dup)) {
            return S_FALSE;
        }
        return S_FALSE;
    }

    return S_FALSE;
}*/
//IR-97
//-Patni-PJS/2010Oct26/Added/WFDA/IR-97




//+Patni-AJS/2010Sep3/Added/ScanCommentDialog/IR-163
//*******************************Method Header*********************************
//Method Name    :GetScanComment()
//Author         : PATNI\AJS
//Purpose        :
//*****************************************************************************
STDMETHODIMP CPqmIpcCom::GetScanCommentXMLData(
    VARIANT** pData
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())


    if (m_ptrManager) {
        m_ptrManager->GetScanCommentXMLData(pData);

    } else {
        return S_FALSE;
    }

    return S_OK;

}
//-Patni-AJS/2010Sep3/Added/ScanCommentDialog/IR-163

//+Patni-PJS/2011Feb21/Added/IR-97
/****************************Method Header************************************

//Method Name   :SetIsValidWFDAScan()

//Author        :PATNI/JS

//Purpose       :
//*****************************************************************************/
STDMETHODIMP CPqmIpcCom::SetIsValidWFDAScan(
    BOOL f_is_valid_wfda_scan
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {

        m_ptrManager->SetIsValidWFDAScan(f_is_valid_wfda_scan);
        return S_OK;

    } else {
        return S_FALSE;
    }
}

STDMETHODIMP CPqmIpcCom::SendIndex(
    int f_index
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    //Patni-Dhanesh/2009Aug19/Modified/cpp test corrections
    if (m_ptrManager) {
        m_ptrManager->SendIndex(f_index);
        //+Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    } else {
        //WriteTraceLog(DOMAIN_NAME,USER_FUNC_MARKER,_T("CPqmIpcCom::SendIndex"),
        //	_T("m_ptrManager is NULL"),false);
        return S_FALSE;
    }

    //-Patni-Dhanesh/2009Aug19/Modified/cpp test corrections

    return S_OK;
}
//-Patni-PJS/2011Feb21/Added/IR-97


//+Patni-PJS/2011Jan29/Modified/WFDA/IR-97
/****************************Method Header************************************

//Method Name   :CheckPlnStatus()

//Author        :PATNI/JS

//Purpose       :
//*****************************************************************************/
STDMETHODIMP CPqmIpcCom::CheckPlnStatus(
    int f_acq_order
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        if (false == m_ptrManager->CheckPlnStatus(f_acq_order)) {
            return S_FALSE;

        } else {
            return S_OK;
        }

    } else {
        return S_FALSE;
    }
}

/****************************Method Header************************************

//Method Name   :IsProtocolInScan

//Author        :PATNI/JS

//Purpose       :

//*****************************************************************************/
STDMETHODIMP CPqmIpcCom::IsProtocolInScan(
    int f_acq_order
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        if (!m_ptrManager->IsProtocolInScan(f_acq_order)) {
            return S_FALSE;

        } else {
            return S_OK;
        }

    } else {
        return S_FALSE;
    }
}



/****************************Method Header************************************

//Method Name   :GetProtocolInformation()

//Author        :PATNI/JS

//Purpose       :
//*****************************************************************************/
STDMETHODIMP CPqmIpcCom::GetProtocolInformation(
    VARIANT* pData,
    int f_acq_order
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        if (!m_ptrManager->GetProtocolInformation(pData, f_acq_order)) {
            return S_FALSE;

        } else {
            return S_OK;
        }

    } else {
        return S_FALSE;
    }
}

//-Patni-PJS/2011Jan29/Modified/WFDA/IR-97


//+Patni-Abhishek/2010Dec18/Added/Redmine-1074_part1
/***************************Method Header*************************************/
//Method Name    : GetScanAgent()
//Author         : Abhishek
//Purpose        : To get the Scan Agent from the study file
//*****************************************************************************/
STDMETHODIMP CPqmIpcCom::GetScanAgent(const int f_protocol_index, VARIANT* f_scan_agent)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        if (m_ptrManager->GetScanAgent(f_protocol_index, f_scan_agent)) {
            return S_OK;
        }
    }

    return S_FALSE;
}
//-Patni-Abhishek/2010Dec18/Added/Redmine-1074_part1

//+Patni-Sudhir/2011Jan13/Added/Redmine-434
/***************************Method Header*************************************/
//Method Name    : GetNRFilterStringForProtocol()
//Author         : Sudhir
//Purpose        : To get the Filter ID of selected protocol from the study file
//*****************************************************************************/
STDMETHODIMP CPqmIpcCom::GetNRFilterStringForProtocol(const int f_acq_order, const int f_NRFLG, VARIANT* f_NRVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    if (m_ptrManager) {
        if (m_ptrManager->GetNRFilterStringForProtocol(f_acq_order, f_NRFLG, f_NRVal)) {
            return S_OK;
        }
    }

    return S_FALSE;
}
//-Patni-Sudhir/2011Jan13/Added/Redmine-434
/***************************Method Header*************************************/
//Method Name    : GetNRFilterStringForProtocol()
//Author         :
//Purpose        :
//*****************************************************************************/
STDMETHODIMP CPqmIpcCom::GetEMTFilterStringForProtocol(const int f_acq_order, VARIANT* f_EMTVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    if (m_ptrManager) {
        if (m_ptrManager->GetEMTFilterStringForProtocol(f_acq_order, f_EMTVal)) {
            return S_OK;
        }
    }

    return S_FALSE;
}
/***************************Method Header*************************************/
//Method Name    : IsWireLessSystem()
//Author         :
//Purpose        :
//*****************************************************************************/
STDMETHODIMP CPqmIpcCom::IsWireLessSystem()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    if (m_ptrManager) {
        if (m_ptrManager->IsWireLessSystem()) {
            return S_OK;
        }
    }

    return S_FALSE;
}

//+Patni-PJS/2011Feb24/Added/TMSC_REDMINE-1480
/***************************Method Header*************************************/
//Method Name    : InvokeMRSProcessTool()
//Author         : PATNI\PJS
//Purpose        : This function is used to invoke MRS ProcessTool
//*****************************************************************************/
STDMETHODIMP CPqmIpcCom::InvokeMRSProcessTool(const int f_acq_order)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    if (m_ptrManager) {
        if (m_ptrManager->InvokeMRSProcessTool(f_acq_order)) {
            return S_OK;
        }
    }

    return S_FALSE;
}

/***************************Method Header*************************************/
//Method Name    : InvokeMRSProcessTool()
//Author         : PATNI\PJS
//Purpose        : This function is used to invoke MRS ProcessTool
//*****************************************************************************/
STDMETHODIMP CPqmIpcCom::GetMRSLicense()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    if (m_ptrManager) {
        if (m_ptrManager->GetMRSLicense()) {
            return S_OK;
        }
    }

    return S_FALSE;
}


STDMETHODIMP CPqmIpcCom::GetMRS_BRST_License()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    if (m_ptrManager) {
        if (m_ptrManager->GetMRS_BRST_License()) {
            return S_OK;
        }
    }

    return S_FALSE;
}


STDMETHODIMP CPqmIpcCom::GetPostProcessFlag()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    if (m_ptrManager) {
        if (m_ptrManager->GetPostProcessFlag()) {
            return S_OK;
        }
    }

    return S_FALSE;
}

/***************************Method Header*************************************/
//Method Name    : IsSelectedSeqMRS()
//Author         : PATNI\PJS
//Purpose        : This function is returns S_OK if the protocol
//                 with acq order is MRS else returns S_FALSE
//*****************************************************************************/
STDMETHODIMP CPqmIpcCom::IsSelectedSeqMRS(const int f_acq_order)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    if (m_ptrManager) {
        if (m_ptrManager->IsSelectedSeqMRS(f_acq_order)) {
            return S_OK;
        }
    }

    return S_FALSE;
}
//-Patni-PJS/2011Feb24/Added/TMSC_REDMINE-1480

/***************************Method Header*************************************/
//Method Name    : SetCalaApplnRunning()
//Author         : PATNI\PJS
//Purpose        :
//*****************************************************************************/
STDMETHODIMP CPqmIpcCom::SetAutoLocatorRunning(
    BOOL f_cala_appln_runng
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    if (m_ptrManager) {
        m_ptrManager->SetAutoLocatorRunning(f_cala_appln_runng);
        return S_OK;
    }

    return S_FALSE;
}

/****************************Method Header*************************************
//Method Name   :GetProtIndexAndWorkingMode()
//Author        :PATNI/PJS
//Purpose       :
//****************************************************************************/
STDMETHODIMP CPqmIpcCom::GetProtIndexAndWorkingMode(
    int f_acq_order,
    VARIANT* pData
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        m_ptrManager->GetProtIndexAndWorkingMode(f_acq_order, pData);

    } else {
        return S_FALSE;
    }

    return S_OK;
}

/***************************Method Header*************************************/
//Method Name    : IsValidSequenceToInvokeCALAAppln()
//Author         : PATNI\PJS
//Purpose        :
//*****************************************************************************/
STDMETHODIMP CPqmIpcCom::IsValidSequenceToInvokeAutoLocator(const int f_acq_order, const int f_autolocatortype)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here
    if (m_ptrManager) {
        if (m_ptrManager->IsValidSequenceToInvokeAutoLocator(f_acq_order, f_autolocatortype)) {
            return S_OK;
        }
    }

    return S_FALSE;
}

/****************************Method Header*************************************
//Method Name   :GetProtIndexAndWorkingMode()
//Author        :PATNI/PJS
//Purpose       :
//****************************************************************************/
STDMETHODIMP CPqmIpcCom::GetProtIndexAndWorkingModeForDoneProt(
    VARIANT* pData
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        m_ptrManager->GetProtIndexAndWorkingMode(pData);

    } else {
        return S_FALSE;
    }

    return S_OK;
}

/****************************Method Header*************************************
//Method Name   :GetCalaPlannedStatus()
//Author        :PATNI/Dhanesh
//Purpose       :
//****************************************************************************/
STDMETHODIMP CPqmIpcCom::GetAutoLocatorPlannedStatus(
    const int f_acq_order, int* f_cala_planned_status
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        m_ptrManager->GetAutoLocatorPlannedStatus(f_acq_order, f_cala_planned_status);

    } else {
        return S_FALSE;
    }

    return S_OK;
}

/***************************Method Header*************************************/
//Method Name    : GetAutoLocatorLicense()
//Author         : PATNI\Dhanesh
//Purpose        : This function is used to check CALA License
//*****************************************************************************/
STDMETHODIMP CPqmIpcCom::GetAutoLocatorLicense(const int f_autolocator_type)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
        
        if (m_ptrManager) {
            if (m_ptrManager->GetAutoLocatorLicense(f_autolocator_type)) {
                return S_OK;
            }
        }

    return S_FALSE;
}
//************************************Method Header************************************
// Method Name  : CalculateTimeDiff
// Author       : PATNI/
// Purpose      : This function is used to get the time difference between the Inject Time of contrast agent and current time
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::CalculateTimeDiff(VARIANT* f_pdata)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (!m_ptrManager) {
        return S_FALSE;
    }

    m_ptrManager->CalculateTimeDiff(f_pdata);
    return S_OK;
}
//************************************Method Header************************************
// Method Name  : GetProtectedStatus
// Author       : IGATE/SS
// Purpose      :Read protected tag value
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::GetProtectedStatus(BSTR bstr)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (!m_ptrManager) {
        return S_FALSE;
    }

    if (m_ptrManager->GetProtectedStatus(bstr)) {
        return S_OK;
    }

    return S_FALSE;
}

//************************************Method Header************************************
// Method Name  : SetDelayTimeToVf
// Author       : PATNI/AKR
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::SetDelayTimeToVf(VARIANT* f_pdata)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (!m_ptrManager) {
        return S_FALSE;
    }

    m_ptrManager->SetDelayTimeToVf(f_pdata);
    return S_OK;
}

//************************************Method Header************************************
// Method Name  : GetMulitScanDynamic_License
// Author       : iGATEPatni/Abhishek
// Purpose      : Redmine -2187 This will return Mulitscan dynamic lisence
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::GetMulitScanDynamic_License()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        return m_ptrManager->GetMulitScanDynamic_License();;
    }

    return S_FALSE;
}
//************************************Method Header************************************
// Method Name  : ExecuteAutoScanStart
// Author       : PATNI/AKR
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::ExecuteAutoScanStart()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (!m_ptrManager) {
        return S_FALSE;
    }

    m_ptrManager->ExecuteAutoScanStart();

    return S_OK;
}

//************************************Method Header************************************
// Method Name  : AllocateGroupIdForDuplicateProtocols
// Author       : PATNI/AKR
// Purpose      : Allocate group id for duplicated protocol
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::AllocateGroupIdForDuplicateProtocols(VARIANT* f_pdata)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (!m_ptrManager) {
        return S_FALSE;
    }

    m_ptrManager->AllocateGroupIdForDuplicateProtocols(f_pdata);

    return S_OK;
}
//************************************Method Header************************************
// Method Name  : OnDeleteUpdateMasterSlave
// Author       : PATNI/AKR
// Purpose      : Update group id on deleting a protocol
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::OnDeleteUpdateMasterSlave(VARIANT* f_pdata)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (!m_ptrManager) {
        return S_FALSE;
    }

    m_ptrManager->OnDeleteUpdateMasterSlave(f_pdata);

    return S_OK;
}
//************************************Method Header************************************
// Method Name  : UpdateGroupNum
// Author       : PATNI/AKR
// Purpose      : update group num
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::UpdateGroupNum()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (!m_ptrManager) {
        return S_FALSE;
    }

    m_ptrManager->UpdateGroupNum();

    return S_OK;
}
//************************************Method Header************************************
// Method Name  : GetProtocolInfoForAutoCopyMasterSlave
// Author       : PATNI/Varun
// Purpose      : This function is used to get the protocol count
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::GetProtocolInfoForAutoCopy(VARIANT* pdata)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (!m_ptrManager) {
        return S_FALSE;
    }

    m_ptrManager->GetProtocolInfoForAutoCopy(pdata);
    return S_OK;
}
STDMETHODIMP CPqmIpcCom::GetScanMutex(VARIANT* f_mutex)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager)
        return m_ptrManager->GetScanMutex(&f_mutex) ? S_OK : S_FALSE ;

    return S_FALSE;
}
//************************************Method Header************************************
// Method Name  : GetStudyLock
// Author       : iGATE/Sribanta
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::GetStudyLock(VARIANT* f_mutex)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager)
        return m_ptrManager->GetStudyLock(&f_mutex) ? S_OK : S_FALSE ;

    return S_FALSE;
}

/****************************Method Header*************************************
//Method Name   :GetProtIndexAndWorkingMode()
//Author        :PATNI/Pavan
//Purpose       : Set channel mode selection as per user selected from coil dialog
//****************************************************************************/
STDMETHODIMP CPqmIpcCom::SetChannelModeSelection(const int f_selected_item, BOOL f_selectedchannel_mode, const int f_system_channel)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        m_ptrManager->SetChannelModeSelection(f_selected_item, f_selectedchannel_mode, f_system_channel);

    } else {
        return S_FALSE;
    }

    return S_OK;
}

/****************************Method Header*************************************
//Method Name   :GetProtIndexAndWorkingMode()
//Author        :PATNI/Pavan
//Purpose       : Set default channel mode, if protocol is transfer from PQM to memory
//****************************************************************************/
STDMETHODIMP CPqmIpcCom::GetChannelModeSelection(const int f_selected_item, VARIANT* f_pdata)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        m_ptrManager->GetChannelModeSelection(f_selected_item, f_pdata);

    } else {
        return S_FALSE;
    }

    return S_OK;
}

/****************************Method Header*************************************
//Method Name   :GetNumSignalfromSelectedSection()
//Author        :PATNI/Pavan
//Purpose       : Set default channel mode, if protocol is transfer from PQM to memory
//****************************************************************************/
STDMETHODIMP CPqmIpcCom::GetNumSignalfromSelectedSection(const int f_selected_item, VARIANT* f_pdata)
{

    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        m_ptrManager->GetNumSignalfromSelectedSection(f_selected_item, f_pdata);

    } else {
        return S_FALSE;
    }

    return S_OK;
}

/****************************Method Header*************************************
//Method Name   :SetNumChannels()
//Author        :PATNI/Pavan
//Purpose       : Update numchannels as per selected mode and update coils accordingly
//****************************************************************************/
STDMETHODIMP CPqmIpcCom::SetNumChannels(const int f_prot, const int f_channel)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        m_ptrManager->SetNumChannels(f_prot, f_channel);

    } else {
        return S_FALSE;
    }

    return S_OK;
}

/****************************Method Header*************************************
//Method Name   :SetNumChannelsforChannelMode()
//Author        :PATNI/Pavan
//Purpose       :
//****************************************************************************/
STDMETHODIMP CPqmIpcCom::SetNumChannelsforChannelMode(const BOOL f_use_coil_channel, const int f_system_enable_channel, const int f_limited_channel) // 20-Oct-11
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        m_ptrManager->SetNumChannelsforChannelMode(f_use_coil_channel, f_system_enable_channel, f_limited_channel);

    } else {
        return S_FALSE;
    }

    return S_OK;
}


/****************************Method Header*************************************
//Method Name   :SendWirelessGateParam()
//Author        :
//Purpose       :
//****************************************************************************/

STDMETHODIMP CPqmIpcCom::SendWirelessGateParam(
    VARIANT* f_pdata
)
{

    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager && m_ptrManager->SendWirelessGateParam(f_pdata)) {
        return S_OK;
    }

    return S_FALSE;
}

/****************************Method Header*************************************
//Method Name   :GetWirelessGatingSelections()
//Author        :
//Purpose       :
//****************************************************************************/
STDMETHODIMP CPqmIpcCom::GetWirelessGatingSelections(
    VARIANT* f_data
)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager && m_ptrManager->GetWirelessGatingSelections(f_data)) {
        return S_OK;
    }

    return S_FALSE;
}


//************************************Method Header************************************
// Method Name  : SetWirelessParamtoProt
// Author       :
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::SetWirelessParamtoProt(VARIANT** f_wireless_param)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here

    if (m_ptrManager) {

        m_ptrManager->SetWirelessParamtoProt(f_wireless_param) ;

        return S_OK;
    }

    return S_FALSE;
}

//************************************Method Header************************************
// Method Name  : CheckAndGetWaitingForAutoScanStartAcqOrder
// Author       : Patni/Nilesh
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::CheckAndGetWaitingForAutoScanStartAcqOrder(int* f_waiting_for_auto_scan_acq_order)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        m_ptrManager->CheckAndGetWaitingForAutoScanStartAcqOrder(f_waiting_for_auto_scan_acq_order);
        return S_OK;
    }

    return S_FALSE;
}
//************************************Method Header************************************
// Method Name  : GetMasterPositionForSlave
// Author       : Patni\AKR
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::GetMasterPositionForSlave(const int f_slave_index, int* f_master_position)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        m_ptrManager->GetMasterPositionForSlave(f_slave_index, f_master_position) ;
        return S_OK;
    }

    return S_FALSE;
}

//************************************Method Header************************************
// Method Name  : ChangeSlaveToNormalProtocol
// Author       : Patni/AKR
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::ChangeSlaveToNormalProtocol(const int f_slave_index)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        m_ptrManager->ChangeSlaveToNormalProtocol(f_slave_index);
        return S_OK;
    }

    return S_FALSE;
}
//************************************Method Header************************************
// Method Name  : GetProtocolIndexToUpdate
// Author       : Patni/AKR
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::GetProtocolIndexToUpdate(int* const f_protocol_index)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        m_ptrManager->GetProtocolIndexToUpdate(f_protocol_index);
        return S_OK;
    }

    return S_FALSE;
}
//************************************Method Header************************************
// Method Name  : GetMasterListForPlanMark
// Author       : Patni/AKR
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::GetMasterListForPlanMark(VARIANT* f_pdata)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        m_ptrManager->GetMasterListForPlanMark(f_pdata);
        return S_OK;
    }

    return S_FALSE;
}
//************************************Method Header************************************
// Method Name  : PerformAutoCopyForMasterSlave
// Author       : Patni/AKR
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::PerformAutoCopyForMasterSlave(VARIANT* const f_pdata)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        m_ptrManager->PerformAutoCopyForMasterSlave(f_pdata);
        return S_OK;
    }

    return S_FALSE;
}
//************************************Method Header************************************
// Method Name  : ResetAutoScanTimer
// Author       : Patni/AKR
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::ResetAutoScanTimer()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        m_ptrManager->ResetAutoScanTimer();
        return S_OK;
    }

    return S_FALSE;
}

//************************************Method Header************************************
// Method Name  : UpdateSARInfo
// Author       : Patni
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::UpdateSARInfo()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        m_ptrManager->UpdateSARInfo();
        return S_OK;
    }

    return S_FALSE;
}
//************************************Method Header************************************
// Method Name  : GetRunningProcCount
// Author       : iGatePatni
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::GetRunningProcCount(int* f_proc_count)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        m_ptrManager->GetRunningProcCount(f_proc_count);
        return S_OK;
    }

    return S_FALSE;
}

//************************************Method Header************************************
// Method Name  : SendSAEInfo
// Author       : iGate
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::SendSAEInfo()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        m_ptrManager->SendSAEInfo();
        return S_OK;
    }

    return S_FALSE;
}

//************************************Method Header************************************
// Method Name  : GetShimState
// Author       : iGATE
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::GetShimState(int* f_shim_state)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        m_ptrManager->GetShimState(f_shim_state);
        return S_OK;
    }

    return S_FALSE;
}
//************************************Method Header************************************
// Method Name  : GetAMBStatusData
// Author       : iGATE
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::GetAMBStatusData(int* f_selected_items, int count, VARIANT* data)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here

    if (m_ptrManager) {
        if (m_ptrManager->GetAMBStatusData(f_selected_items, count, &data))
            return S_OK;
    }

    return S_FALSE;

}
//************************************Method Header************************************
// Method Name  : AMBScanAbort
// Author       : iGATE
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::AMBScanAbort()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        m_ptrManager->AMBScanAbort() ;
        return S_OK;
    }

    return S_FALSE;
}
//************************************Method Header************************************
// Method Name  : AMBScanCancel
// Author       : iGATE
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::AMBScanCancel(VARIANT* data)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    // TODO: Add your implementation code here

    if (m_ptrManager) {

        //m_ptrManager->AMBScanCancel(&data) ;

        return S_OK;
    }

    return S_FALSE;
}
//************************************Method Header************************************
// Method Name  : GetAmbPlan
// Author       : iGATE
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::GetAmbPlan(VARIANT* data)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {

        m_ptrManager->GetAMBPlan(&data);

        return S_OK;
    }

    return S_FALSE;

}
//************************************Method Header************************************
// Method Name  : AMBScanStart
// Author       : iGATE
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::AMBScanStart(VARIANT* data, BOOL* f_is_scan_start)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        *f_is_scan_start = m_ptrManager->AMBScanStart(&data);
        return S_OK;
    }

    return S_FALSE;
}
//************************************Method Header************************************
// Method Name  : GetAMBRearrangeIndexes
// Author       : iGATE
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::GetAMBRearrangeIndexes(int* f_rotocol_index_to_move, int* f_target_index)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    m_ptrManager->GetRearrangeIndex(f_rotocol_index_to_move, f_target_index);

    return S_OK;
}
//************************************Method Header************************************
// Method Name  : AMBReArrange
// Author       : iGATE
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::AMBReArrange(int f_drag_index, int f_drop_index)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        m_ptrManager->AMBReArrange(f_drag_index, f_drop_index);

    } else {
        return S_FALSE;
    }

    return S_OK;

}
//************************************Method Header************************************
// Method Name  : GetAMBAutoScanItems
// Author       : iGATE
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::GetAMBAutoScanItems(int** f_arr_items, int* f_count, BOOL* l_is_auto_scan_on)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        m_ptrManager->GetAMBAutoScanItems(f_arr_items, f_count, l_is_auto_scan_on);

    } else {
        return S_FALSE;
    }

    return S_OK;
}
//************************************Method Header************************************
// Method Name  : GetAMBMasterProtocols
// Author       : iGATE
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::GetAMBMasterProtocols(VARIANT* f_data)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        m_ptrManager->GetAMBMasterProtocols(&f_data);

    } else {
        return S_FALSE;
    }

    return S_OK;
}
//************************************Method Header************************************
// Method Name  : GetAMBLicense
// Author       : iGATE
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::GetAMBLicense()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {

        if (m_ptrManager->GetAMBLicense())
            return S_OK;
    }

    return S_FALSE;
}
//************************************Method Header************************************
// Method Name  : GetAMBShimMapArray
// Author       : iGATE
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::GetAMBShimMapArray(VARIANT* f_data)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        m_ptrManager->GetAMBShimMapArray(&f_data);
        return S_OK;
    }

    return S_OK;
}
//************************************Method Header************************************
// Method Name  : GetAMBIndexToUdpate
// Author       : iGATE
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::GetAMBIndexToUdpate()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        return m_ptrManager->GetAMBIndexToUdpate();

    } else {
        return S_FALSE;
    }

    return S_OK;
}
//************************************Method Header************************************
// Method Name  : CheckSlaveProtocolForDuplicate
// Author       : iGATE
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::CheckSlaveProtocolForDuplicate(int* f_selected_items, int f_count)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        if (m_ptrManager->CheckSlaveProtocolForDuplicate(f_selected_items, f_count)) {
            return S_OK;
        }
    }


    return S_FALSE;
}
//************************************Method Header************************************
// Method Name  : GetAMBSlaveCount
// Author       : iGATE
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::GetAMBSlaveCount(int* f_count)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        m_ptrManager->GetAMBSlaveCount(f_count);

    } else {
        return S_FALSE;
    }

    return S_OK;
}
//************************************Method Header************************************
// Method Name  : AMBSlaveMatchSequenceParam
// Author       : iGATE
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::AMBSlaveMatchSequenceParam(VARIANT* f_pData, const int f_start_index, const int f_end_index)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        if (m_ptrManager->AMBSlaveMatchSequenceParam(f_pData, f_start_index, f_end_index)) {
            return S_OK;
        }
    }

    return S_FALSE;
}
//************************************Method Header************************************
// Method Name  : PqmPqAMBSlaveLost
// Author       : iGATE
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::PqmPqAMBSlaveLost(int* f_selected_list, int* f_selected_count, BOOL* f_slave_exist)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        *f_slave_exist = m_ptrManager->PqmPqAMBSlaveLost(f_selected_list, f_selected_count);
        return S_OK;
    }

    return S_FALSE;

}

STDMETHODIMP CPqmIpcCom::SetIsAMBPlanning(const BOOL f_is_amb_planning)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        m_ptrManager->SetIsAMBPlanning(f_is_amb_planning > 0);
        return S_OK;
    }

    return S_FALSE;
}

STDMETHODIMP CPqmIpcCom::GetIndexForParentImageRemoved(int* f_acq_order)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        *f_acq_order = m_ptrManager->GetIndexForParentImageRemoved();
        return S_OK;
    }

    return S_FALSE;
}
//************************************Method Header************************************
// Method Name  : CheckAMBMenuClickConditions
// Author       : iGATE
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::CheckAMBMenuClickConditions(int* f_selected_items, int count, BOOL* f_return)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        *f_return  = m_ptrManager->CheckAMBMenuClickConditions(f_selected_items, count);
        return S_OK;
    }

    return S_FALSE;

}

STDMETHODIMP CPqmIpcCom::GetAPCRetry(BOOL* f_is_apc_retry)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        *f_is_apc_retry  = m_ptrManager->GetAPCRetry();
        return S_OK;
    }

    return S_FALSE;
}
/***************************Method Header*************************************/
//Method Name    : ReConnectSARManager()
//Author         : Patni-AKR
//Purpose        : To Reconnect SAR Manager
//*****************************************************************************/
STDMETHODIMP CPqmIpcCom::ReConnectSARManager()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        if (m_ptrManager->ReConnectSARManager()) {
            return S_OK;
        }
    }

    return S_FALSE;
}


//************************************MethodHeader************************************
// Method Name  : HandleAbortFailForProtocol()
// Author       : PATNI/AKR
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::HandleAbortFailForProtocol()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        if (m_ptrManager->HandleAbortFailForProtocol()) {
            return S_OK;
        }
    }

    return S_FALSE;
}
//************************************MethodHeader************************************
// Method Name  : CalcSARControlAndPutVFForAll()
// Author       : PATNI/AKR
// Purpose      :
//***********************************************************************************
STDMETHODIMP CPqmIpcCom::CalcSARControlAndPutVFForAll()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        if (m_ptrManager->CalcSARControlAndPutVFForAll()) {
            return S_OK;
        }
    }

    return S_FALSE;
}

/***************************Method Header*************************************/
//Method Name    : GetSARControlLicense()
//Author         : iGATE/Jayanthi
//Purpose        :
//*****************************************************************************/
STDMETHODIMP CPqmIpcCom::GetSARControlLicense()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        if (m_ptrManager->GetSARControlLicense()) {
            return S_OK;
        }
    }

    return S_FALSE;
}
/***************************Method Header*************************************/
//Method Name    : GetMaxTimeForPagerTimer()
//Author         : iGATE/AKR
//Purpose        :
//*****************************************************************************/
STDMETHODIMP CPqmIpcCom::GetMaxTimeForPagerTimer(int* f_fax_pager_timer_time)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {
        if (m_ptrManager->GetMaxTimeForPagerTimer(f_fax_pager_timer_time)) {
            return S_OK;
        }
    }

    return S_FALSE;
}

//********************************Method Header********************************
//Method Name   : GetSarInfoPtrForIndex()
//Author        : iGATE
//Purpose       : GetCurrent sar info pointer
//*****************************************************************************
STDMETHODIMP CPqmIpcCom::GetSarInfoPtrForIndex(VARIANT* f_pdata)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_ptrManager) {

        return m_ptrManager->GetSarInfoPtrForIndex(f_pdata);
    }

    return S_FALSE;
}

STDMETHODIMP CPqmIpcCom::GetGDCSeriesLOIDForProtocol(int *f_prot_idx, VARIANT *f_seriesloid)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
		
		if (m_ptrManager) {
			m_ptrManager->GetGDCSeriesLOIDForProtocol(*f_prot_idx, f_seriesloid);
			return S_OK;
		}
		
		return S_FALSE;
}

STDMETHODIMP CPqmIpcCom::GetTiPrepLicense(const int f_tiprep_type)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
    if (m_ptrManager) {
        if (m_ptrManager->GetTiPrepLicense(f_tiprep_type)) {
            return S_OK;
        }
    }        
    return S_FALSE;

}

STDMETHODIMP CPqmIpcCom::IsProtocolforTiPrep(int f_acq_order)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here

    if (m_ptrManager) {
        if (m_ptrManager->IsProtocolforTiPrep(f_acq_order) == true) {
            return S_OK;
        }
    }        
    return S_FALSE;
}

STDMETHODIMP CPqmIpcCom::CanTiPrepEnable()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here

    if (NULL != m_ptrManager) {
        if (m_ptrManager->CanTiPrepEnable()){
            return S_OK;
        }
    }       
	return S_FALSE;
}

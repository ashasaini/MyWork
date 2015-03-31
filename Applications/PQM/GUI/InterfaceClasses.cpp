//TMSC-REDMINE-782
//TMSC-REDMINE-780
//TMSC-REDMINE-778
// InterfaceClasses.cpp: implementation of the InterfaceClasses class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "InterfaceClasses.h"
#include "PQMView.h"


//************************************Method Header************************************
// Method Name  : CQueueDialogHandler
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CQueueDialogHandler::CQueueDialogHandler(
    CPQMView* pqm_view
): m_pqm_view(pqm_view)
{
}

//************************************Method Header************************************
// Method Name  : SendMessageToProcon
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CQueueDialogHandler::SendMessageToProcon(
    const ProconMessageEnum procon_message
)const
{
    if (m_pqm_view) {
        switch (procon_message) {
                //+Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1
            case PQM_PROCON_CANCEL:
            case PQM_PROCON_QUEUE_NEXT:
            case PQM_PROCON_NEXT_COPY:
            case PQM_PROCON_QUEUE_EXIT:
                //+Patni-HEMANT/MOSIDIED On 3/12/2009 6:02:08 PM/ PSP#232
                //woe we will process GUI PQM_PROCON_CANCEL message on receipt of
                //PQM_MSG_PROCON_CMD_EXIT from IPC
                m_pqm_view->SendMessageToProcon(procon_message, false, true);
                //-Patni-HEMANT/MOSIDIED On 3/12/2009 6:02:08 PM/ PSP#232

                break;


            default:
                m_pqm_view->SendMessageToProcon(procon_message, true, true);
                return;
                //-Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1
        }

    } else { 		// - Patni + HAR / 2009 Mar 06 / Added / MRS
        ASSERT(FALSE);
    } 		// - Patni - HAR / 2009 Mar 06 / Added / MRS
}


//************************************Method Header************************************
// Method Name  : CMRSPreScanDlgHandler
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CMRSPreScanDlgHandler::CMRSPreScanDlgHandler(
    CPQMView* pqm_view
):	m_pqm_view(pqm_view)
{

}


//************************************Method Header************************************
// Method Name  : MrsScanStartUiCB
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CMRSPreScanDlgHandler::MrsScanStartUiCB(
    SMRSScan_s* mrsscan_values
)const
{

    LPCTSTR FUNC_NAME = _T("CMRSPreScanDlgHandler::MrsScanStartUiCB");

    //Patni-DKH/2009Aug26/Modified/cpp test corrections
    if (NULL != m_pqm_view) {
        m_pqm_view->MrsScanStartUiCB(mrsscan_values) ;
        //+Patni-DKH/2009Aug26/Modified/cpp test corrections

    } else {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_pqm_view pointer is NULL"));
    }

    //-Patni-DKH/2009Aug26/Modified/cpp test corrections
}

//************************************Method Header************************************
// Method Name  : MrsScanAbort
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CMRSPreScanDlgHandler::MrsScanAbort(
)const
{

    LPCTSTR FUNC_NAME = _T("CMRSPreScanDlgHandler::MrsScanAbort");

    //Patni-DKH/2009Aug26/Modified/cpp test corrections
    if (NULL != m_pqm_view) {
        m_pqm_view->MrsScanAbort() ;
        //+Patni-DKH/2009Aug26/Modified/cpp test corrections

    } else {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_pqm_view pointer is NULL"));

    }

    //-Patni-DKH/2009Aug26/Modified/cpp test corrections
}

//************************************Method Header************************************
// Method Name  : MrsScanAbort
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CMRSPreScanDlgHandler::MrsScanClose(
)const
{

    LPCTSTR FUNC_NAME = _T("CMRSPreScanDlgHandler::MrsScanClose");

    if (NULL != m_pqm_view) {
        m_pqm_view->MrsScanClose() ;

    } else {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_pqm_view pointer is NULL"));

    }
}


//************************************Method Header************************************
// Method Name  : CAxisLocatorDialogInterface
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CAxisLocatorDialogInterface::CAxisLocatorDialogInterface(
    CPQMView* pqm_view
): m_pqm_view(pqm_view)
{
}


//************************************Method Header************************************
// Method Name  : OnAxisLocatorDialogClosed
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CAxisLocatorDialogInterface::OnAxisLocatorDialogClosed(
    bool ok_pressed /*= true*/
)const
{
    LPCTSTR FUNC_NAME = _T("CAxisLocatorDialogInterface::OnAxisLocatorDialogClosed");

    //Patni-DKH/2009Aug26/Modified/cpp test corrections
    if (NULL != m_pqm_view) {
        //+Patni-Hemant/2010Feb10/Modified/ACE-Phase#3/CDS Requirement
        //m_pqm_view->OnSelectionStatusChanged();
        m_pqm_view->OnAxisLocatorDialogClosed(ok_pressed);
        //-Patni-Hemant/2010Feb10/Modified/ACE-Phase#3/CDS Requirement
        //+Patni-DKH/2009Aug26/Modified/cpp test corrections

    } else {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_pqm_view pointer is NULL"));

    }

    //-Patni-DKH/2009Aug26/Modified/cpp test corrections
}

//************************************Method Header************************************
// Method Name  : ApplySelectedCoils
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CAxisLocatorDialogInterface::ApplySelectedCoils(
)const
{


    LPCTSTR FUNC_NAME = _T("CAxisLocatorDialogInterface::ApplySelectedCoils");

    //Patni-SS/2009Apr07/Modi/PSP1#12
    //Patni-DKH/2009Aug26/Modified/cpp test corrections
    if (NULL != m_pqm_view) {
        //REDMINE-783_Update
        //m_pqm_view->GetPqmChildThread().PostMessageToThread(ON_COILAPPLY, NULL, NULL);
        m_pqm_view->ApplyCoilToProtocols();
        return true ;
        //+Patni-DKH/2009Aug26/Modified/cpp test corrections

    } else {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_pqm_view pointer is NULL"));

        return false;
    }

    //-Patni-DKH/2009Aug26/Modified/cpp test corrections

}

//+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
float CAxisLocatorDialogInterface::GetMagneticCenterPos(
)const
{


    LPCTSTR FUNC_NAME = _T("CAxisLocatorDialogInterface::GetMagneticCenterPos");

    if (NULL != m_pqm_view) {

        return m_pqm_view->GetMagneticCenterPos();

    } else {


        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_pqm_view pointer is NULL"));

    }

    return 0.0;
}

//-Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement


int CAxisLocatorDialogInterface::GetChannelModeSelection()
{
    LPCTSTR FUNC_NAME = _T("CAxisLocatorDialogInterface::GetChannelModeSelection");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    int l_selected_item = -1;
    int* selected_items = NULL ;
    const int count = m_pqm_view->GetPQMListCtrl().GetselectedItems(&selected_items);

    if (selected_items == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Can not get selected item"));
        return -1;
    }

    l_selected_item = selected_items[0];
    DEL_PTR_ARY(selected_items);

    VARIANT* pdata = new VARIANT;
    int l_channel_mode = -1;
    int l_acq_order = m_pqm_view->GetAcqOderForGivenProtocolIndex(l_selected_item);

    if (S_FALSE != m_pqm_view->GetAcquisition()->GetChannelModeSelection(l_acq_order, pdata)) { // Review comment-11-Jan-11

        if (-1 == pdata->intVal) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Invalid channel mode value from VFC"));
            DEL_PTR(pdata);
            return -1;
        }

        l_channel_mode = pdata->intVal;
    }

    DEL_PTR(pdata);
    return l_channel_mode;
}

BOOL CAxisLocatorDialogInterface::GetIsChannelModeApplicable()
{
    return m_pqm_view->GetIsChannelModeApplicable();
}

//************************************Method Header************************************
// Method Name  : CInterfaceClassForScanCommentDlg
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CInterfaceClassForScanCommentDlg::CInterfaceClassForScanCommentDlg(
    CPQMView* pqm_view
): m_pqm_view(pqm_view)
{
}

//************************************Method Header************************************
// Method Name  : CanModifyWithCommentDialog
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CInterfaceClassForScanCommentDlg::CanModifyWithCommentDialog(
)const
{
    if (m_pqm_view) {

        return m_pqm_view->CanModifyWithCommentDialog();
    }

    return false ;
}

//************************************Method Header************************************
// Method Name  : GetCurrentScanCommentAndScanID
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CInterfaceClassForScanCommentDlg::GetCurrentScanCommentAndScanID(
    CString& f_scan_id,
    CString& f_scan_comment
)const
{
    if (m_pqm_view) {
        return m_pqm_view->GetCurrentScanCommentAndScanID(&f_scan_id, &f_scan_comment);
    }

    return false ;
}


//************************************Method Header************************************
// Method Name  : SetCurrentScanCommentAndScanID
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CInterfaceClassForScanCommentDlg::SetCurrentScanCommentAndScanID(
    CString* f_scan_id,
    CString* f_scan_comment
)const
{

    if (m_pqm_view) {

        m_pqm_view->SetCurrentScanCommentAndScanID(f_scan_id, f_scan_comment);
    }

    return ;
}



//************************************Method Header************************************
// Method Name  : GetCurrentScanComment
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CString CInterfaceClassForScanCommentDlg::GetCurrentScanComment()const
{

    if (m_pqm_view) {

        CString f_scan_comment;
        m_pqm_view->GetCurrentScanCommentAndScanID(NULL, &f_scan_comment);
        return f_scan_comment ;
    }

    return _T("") ;
}

//************************************Method Header************************************
// Method Name  : GetCurrentScanID
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CString CInterfaceClassForScanCommentDlg::GetCurrentScanID()const
{

    if (m_pqm_view) {

        CString f_scan_id;
        m_pqm_view->GetCurrentScanCommentAndScanID(&f_scan_id);

        return f_scan_id ;
    }

    return _T("") ;
}

//-------------------------------------------------------------------------

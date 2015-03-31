// TiPrepAppController.cpp: implementation of the CTiPrepAppController class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TiPrepAppController.h"
#include "DPSManager.h"
#include "PQMView.h"
#include <libStudyManager/CFW/CFWReceiveMsg.h>
#include <libStudyManager/CFW/CFWReplyMsg.h>
using MR::ACQ::SM::COMMON::CFW::CCFWReceiveMsg;
using MR::ACQ::SM::COMMON::CFW::CCFWReplyMsg;
//#include "PQMGlobal.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CPQMView* g_MainView;
using namespace MR::ACQ::SM::COMMON;

CTiPrepAppController::CTiPrepAppController()
{
	
}

CTiPrepAppController::~CTiPrepAppController()
{
	
}

int CTiPrepAppController::InvokeTiPrep(bool bymenu)
{
	m_tiprep_loadgl_wait = 1;
	SetTiPrepInvoked(true); // Need to keep 2 flags one for click/auto event and one for succesful load
	return GetTiPrepTemplate();
}

bool CTiPrepAppController::CanTiPrepInvoke()
{
	//Check if WSE / Locator / CALA /Auto scan is running
	return true;
}

bool CTiPrepAppController::CanTiPrepEnable()
{
    //Ti prep menu shall be disabled for following conditions:
    // page is not acquire page
    // ti prep license not present
    // invalid ti prep protocol
    // scan and reconstruction are not done for selected protocol
    // CALA is running
    // SE is running
    // Locator is running
    
    if (g_MainView && g_MainView->GetPQMGlobal().GetPQMMode() == K_SCHEDULING){    //if schedule page
        return(false);
    }
    
    if(g_MainView->IsScanning()){               //scaning is on
        return(false);
    }
    
    if(g_MainView->IsPausedScanning()){         //scanning is paused
        return(false);
    }
    
    if (g_MainView->GetWaitCursor()){             //wait cursor
        return(false);
    }
    
    if(true == g_MainView->GetIsLocatorOpen()){ //if locator is open
        return(false);
    }
    
    if (true == g_MainView->GetIsWSEOpen()){    //if WSE is open
        return(false);
    }
    
    if(S_OK != g_MainView->GetAcquisition()->CanTiPrepEnable()){ //checks for license,  cala running, locator running
        return(false);
    }

    //if protocol is invalid for ti prep
    if(S_OK != IsProtocolforTiPrep()){
        return(false);
    }
    
    return true;
}
#if 0
void CTiPrepAppController::SetTiPrepRunning(bool isrunning)
{
	m_tiprep_running = isrunning; // not required for display
}

bool CTiPrepAppController::GetTiPrepRunningState()
{
	return m_tiprep_running;
	
}
#endif
bool CTiPrepAppController::GetTiPrepLoadWait()
{
	return m_tiprep_loadgl_wait;
}

void CTiPrepAppController::SetTiPrepLoadWait(bool iswait)
{
	m_tiprep_loadgl_wait = iswait;
	
}

int CTiPrepAppController::GetTiPrepTemplate()
{
	CDPSManager *l_server = CDPSManager::GetInstance();
	//return 	 l_server->SendASyncMsg(_T("Pager"),_T("GL_Pager_GetPageInfo")); //AN test sync communcation
	CCFWReceiveMsg l_re_msg ;
	CDPSManager::GetInstance()->SendSyncMsg(_T("Pager"),_T("GL_Pager_GetPageInfo"),&l_re_msg ); //Is there any risk of using synchronous communication while some oher process is going on ?
	CString l_parameter = l_re_msg.GetParameter();
	ExtractPagerInfo(l_parameter);
	return 0;
}

int CTiPrepAppController::ExtractPagerInfo(const CString &csParam)
{
	CString csouttemplate;
				int index = csParam.Find(_T("Acquire"));
				CString tempstr = csParam.Right(csParam.GetLength() - index);
				index = tempstr.Find(_T(" "));
				tempstr = tempstr.Left(index);
				index = tempstr.Find(_T("="));
				CString acqstudyloid = tempstr.Right(tempstr.GetLength() - index - 1);
				m_study_loid = acqstudyloid;
				index = csParam.Find(CString(_T("Display1")));
				tempstr = csParam.Right(csParam.GetLength() - index);
				index = tempstr.Find(_T(" "));
				tempstr = tempstr.Left(index);
				index = tempstr.Find(_T("="));
				CString disp1studyloid = tempstr.Right(tempstr.GetLength() - index - 1);
				
				index = csParam.Find(CString(_T("Display2")));
				tempstr = csParam.Right(csParam.GetLength() - index);
				index = tempstr.Find(_T(" "));
				tempstr = tempstr.Left(index);
				index = tempstr.Find(_T("="));
				CString disp2studyloid = tempstr.Right(tempstr.GetLength() - index - 1);
				if (acqstudyloid.CompareNoCase(disp1studyloid) == 0  )
				{
					csouttemplate = _T("Display1");
					
				} else if (acqstudyloid.CompareNoCase(disp2studyloid) == 0) 
				{
					csouttemplate = _T("Display2");
					
				} else {
					csouttemplate = _T("");
					
				}
				m_cs_tiprep_template = csouttemplate;
				if (m_tiprep_loadgl_wait) {
					if (!csouttemplate.IsEmpty()) {
						g_MainView->SwitchPage(csouttemplate);
						SendLoadTiPrep();
					} else {
						CDPSManager::GetInstance()->SendASyncMsg(_T("Pager"),_T("GL_Pager_AssignPage"),m_study_loid);
						//LoadStudy in template
					}
				}
				return 0;
}
#if 0
int CTiPrepAppController::ChangeStudyInEmptyTemplate(const CString &cstemplate )
{
	CString csParam = cstemplate;
	csParam += _T("=");
	csParam += m_study_loid;
	m_cs_tiprep_template = cstemplate;
	return	CDPSManager::GetInstance()->SendASyncMsg(_T("Pager"),_T("FW_ChangePatient"),csParam);
}
#endif
int CTiPrepAppController::SendLoadTiPrep(const CString &csParam)
{
	if (!csParam.IsEmpty())
		m_cs_tiprep_template = csParam;
	int ret = 0;
	if ( !m_cs_tiprep_template.IsEmpty())
		CDPSManager::GetInstance()->SendASyncMsg(_T("PAGER"),_T("GL_Pager_SwitchPage"),m_cs_tiprep_template);
	SelectImagesForTiPrep();
	CString parentname;
	
	parentname =m_cs_tiprep_template;;
	parentname += _T(".Controller");
		CDPSManager::GetInstance()->SendASyncMsg(parentname,_T("FW_SwitchSceneBranch"),_T("Film/ToolBox")); //temporariliy commented due to somehang issue
	ret =  CDPSManager::GetInstance()->SendASyncMsg(m_cs_tiprep_template, _T("FW_LoadGL"),_T("TiPrep/TiPrep.Document"),_T("1/0"));
	m_tiprep_loadgl_wait = 0; // is it required to wait till tiprep responds ack? maybe not
	return ret;
}

#if 0
int CTiPrepAppController::TiPrepConnected(const CString &cstemplate)
{
	m_isconnected_tiprep = 1;
	m_cs_tiprep_template  = cstemplate;
	return 0;
}

int CTiPrepAppController::TiPrepConnectionClose()
{
	m_isconnected_tiprep  = 0;
	return 0;
}
#endif
int CTiPrepAppController::OnRecvTiPrepRegister(int pid, const CString &csstudyloid,const CString &cstemplate)
{
	GetPQMStudyLoid();
	CString target = cstemplate;//m_cs_tiprep_template;
//	target += _T(".TiPrep");
	CString param = _T("S_FAIL");
	CString custom = _T("FALSE");
	
	if ( csstudyloid.CompareNoCase(m_study_loid) == 0) //even better to check if study loid are same.safest option
	{
		param = _T("S_OK");
		if (m_bisTiPrepInvoked)
			custom = _T("TRUE");
		else
			custom = _T("FALSE");
	}	
	CDPSManager::GetInstance()->SendASyncMsg(target,_T("RE_REGISTER_TIPREP_TO_PQM"),param,custom);
	return 0;
	
}
//on change study, pqm exit, end exam, or tiprep close connection set it false
//set tru on menu click or auto invoke
void CTiPrepAppController::SetTiPrepInvoked(bool flag)
{
	m_bisTiPrepInvoked = flag;
	
}

int CTiPrepAppController::CanTiPrepUpdate(int protocol, const CString &studyloid, const CString &cstemplate)
{
	CString flag = _T("TRUE");
	GetPQMStudyLoid();
	CString target = cstemplate; //Need to take from CFW everytime 
	//	target += _T(".TiPrep");
	if (studyloid.CompareNoCase(m_study_loid) != 0) flag =  _T("FALSE");
	else if (g_MainView->GetIsWSEOpen()) flag =  _T("FALSE");
	CDPSManager::GetInstance()->SendASyncMsg(target,_T("RE_CAN_TIPREP_UPDATE"),flag);
		if (flag.CompareNoCase(_T("TRUE")) == 0)
			g_MainView->SetWaitCursor(true);
	
	//Once again check if study loid is same, 
	//check if auto scan or locator or CALA running, or scan started for given protocol
	return TRUE;
	
}

int CTiPrepAppController::OnRecvConnectionClose(const CString &studyloid,const CString &cstemplate)
{
	GetPQMStudyLoid();
	if (studyloid.CompareNoCase(m_study_loid) == 0)
		SetTiPrepInvoked(false);
	return 0;
	
}

int CTiPrepAppController::ProcessUpdateRequest(const CString &csParam, const CString & sender)
{
	int protocol = 0;
	GetPQMStudyLoid();
	CString csstudyloid = csParam;
				int index = csParam.Find(_T("Study"));
				CString tempstr = csParam.Right(csParam.GetLength() - index);
				index = tempstr.Find(_T(","));
				if (index != -1)tempstr = tempstr.Left(index);
				index = tempstr.Find(_T("="));
				csstudyloid = tempstr.Right(tempstr.GetLength() - index - 1);
				index = csParam.Find(_T("Protocol"));
				tempstr = csParam.Right(csParam.GetLength() - index);
				index = tempstr.Find(_T(","));
				if (index != -1)tempstr = tempstr.Left(index);
				index = tempstr.Find(_T("="));
				CString prot = tempstr.Right(tempstr.GetLength() - index - 1);
				char l_intermediate_char[10];
				sprintf(l_intermediate_char, "%S", prot);
				protocol = atoi(l_intermediate_char);
				if (csstudyloid.CompareNoCase(m_study_loid) != 0) return E_ERROR;
				
				CanTiPrepUpdate(protocol,csstudyloid,sender);
				return 0;
}

int CTiPrepAppController::SelectImagesForTiPrep()
{
	CString studypath;
	int* selected_seq = NULL;
    int l_seq_count = g_MainView->GetPQMListCtrl().GetselectedItems(&selected_seq);
	if (NULL == selected_seq) {
        //PQM_TRACE(COMMHANDLER_MARKER_PQM_DPS, FUNC_NAME, _T("selected_seq is NULL"));
        return FALSE;
    }
	
	CSequence* l_seq = (CSequence*)g_MainView->GetPQMListCtrl().GetItemData(selected_seq[0]);
	
	if (NULL == l_seq) {
		//  PQM_TRACE(COMMHANDLER_MARKER_PQM_DPS, FUNC_NAME, _T("l_seq is NULL"));
		return FALSE;
	}
	
	int l_acq_order = -1;
	l_acq_order = _ttoi(l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER")));
	
	int prot_idx  =	g_MainView->GetProtocolIndexFromAcqorder(l_acq_order);

    g_MainView->GetAcquisition()->IsProtocolforTiPrep(prot_idx);

	if (true){

	VARIANT* pData = new VARIANT;
	g_MainView->GetAcquisition()->GetGDCSeriesLOIDForProtocol(&prot_idx,pData);
	
	const CString l_study_path = pData->bstrVal;
	DEL_PTR(pData);

	CString target, parameter;
	target = m_cs_tiprep_template + _T(".iselector");
//	CDPSManager::GetInstance()->SendASyncMsg(target,_T("FW_Show"));
	parameter =l_study_path;// _T("12743")
    CDPSManager::GetInstance()->SendASyncMsg(target,_T("GL_IS_Get_Select_Series_IC_Name"),parameter);
}

	return 0;
}

void CTiPrepAppController::GetPQMStudyLoid()
{
	if (!m_study_loid.IsEmpty()) return ;
	//	m_study_loid = g_MainView->Getstudyloid();
	//	if (!m_study_loid.IsEmpty()) return ;
	CCFWReceiveMsg l_re_msg ;
	CDPSManager::GetInstance()->SendSyncMsg(_T("Pager"),_T("GL_Pager_GetPageInfo"),&l_re_msg ); //Is there any risk of using synchronous communication while some oher process is going on ?
	CString l_parameter = l_re_msg.GetParameter();
	if (!l_parameter.IsEmpty())
	ExtractPagerInfo(l_parameter);
	else 
		m_study_loid = g_MainView->Getstudyloid();
	return ;
}

void CTiPrepAppController::CloseConnectionToTiPrep()
{
	GetPQMStudyLoid();
	if (m_cs_tiprep_template.IsEmpty()) return;
	CString target = m_cs_tiprep_template; //Need to take from CFW everytime 
	target += _T(".TiPrep");
	CDPSManager::GetInstance()->SendASyncMsg(target,_T("PQM_CLOSE_CONN"),m_study_loid);
}

HRESULT CTiPrepAppController::IsProtocolforTiPrep()
{
    CString studypath;
    int* selected_seq = NULL;
    int l_seq_count = g_MainView->GetPQMListCtrl().GetselectedItems(&selected_seq);
    if (NULL == selected_seq) {
        //PQM_TRACE(COMMHANDLER_MARKER_PQM_DPS, FUNC_NAME, _T("selected_seq is NULL"));
        return S_FALSE;
    }
    
    CSequence* l_seq = (CSequence*)g_MainView->GetPQMListCtrl().GetItemData(selected_seq[0]);
    
    if (NULL == l_seq) {
        //  PQM_TRACE(COMMHANDLER_MARKER_PQM_DPS, FUNC_NAME, _T("l_seq is NULL"));
        return S_FALSE;
    }
    
    int l_acq_order = -1;
    l_acq_order = _ttoi(l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER")));
    
    return g_MainView->GetAcquisition()->IsProtocolforTiPrep(l_acq_order);
}

HRESULT CTiPrepAppController::GetTiPrepLicense( const int tipreptype )
{
    return g_MainView->GetAcquisition()->GetTiPrepLicense(TIPREP_SCANNER);
}

int CTiPrepAppController::SendRefreshMsgToTiPrep()
{
    GetPQMStudyLoid();
    if (m_cs_tiprep_template.IsEmpty()) return;
    CString target = m_cs_tiprep_template; //Need to take from CFW everytime 
    target += _T(".TiPrep");
	CDPSManager::GetInstance()->SendASyncMsg(target,_T("REFRESH_TIPREP"),m_study_loid);
    return(0);
}

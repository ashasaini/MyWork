// KtReconController.cpp: implementation of the CKtReconController class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KtReconController.h"
#include "KtReconDlg.h"
#include "KtReconErrors.h"
#include "CommonDefinitions.h"
#include "KtCommandProcessor.h"
#include "KtEngineDataCopy.h"
#include "KtVROMProc.h"
#include "KtReconViewer.h"
#include "LocalDBMgr.h"
#include "KtReconLogHelper.h"
#include "KtICMMgr.h"
#include "KtDPSCommMgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKtReconController::CKtReconController() : m_csstudyloid(_T(""))
{
    m_csStudyFileName = _T(""); //_T("");//TEST
    m_nSelectedProtIndex = 1;   //TEST
	m_is_wait_cursor = false; //+Mpower/20130812/PN/added
    
    m_ExecProc = NULL;
    m_ExecProc = new CKtReconExecProc();
    if(NULL == m_ExecProc){
		CString l_error_string(_T("CKtReconController():m_ExecProc is NULL"));
		CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
    }    
    m_KtReconDlg = NULL;    
}

CKtReconController::~CKtReconController()
{
    if(NULL != m_ExecProc){
        delete m_ExecProc;
        m_ExecProc = NULL;
    }
}

int CKtReconController::OnView()
{
    try{
        if(NULL == m_ExecProc){
			CString l_error_string(_T("CKtReconController():m_ExecProc is NULL"));
			CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
            return(E_ERR_EXCEPTION);
        }
        
        int l_nStatus = E_NO_ERROR;
        CString l_csCommand(_T(""));
        
        CKtCommandProcessor* l_CmdProc;
        CKtReconViewer l_ReconVwr;  
        l_CmdProc = &l_ReconVwr;
        
        l_nStatus = l_CmdProc->GetCommand(m_csStudyFileName,m_nSelectedProtIndex,l_csCommand);
        if(E_NO_ERROR != l_nStatus){
			CString l_error_string(_T("Error occured in GetCommand for ktReconViewer"));
			CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
            return(l_nStatus);
        }
        
        l_nStatus = m_ExecProc->InvokeExec(KTRECONVIEWER_INIT,l_csCommand,false);
        if(E_NO_ERROR != l_nStatus){
			CString l_error_string(_T("Error occured in InvokeExec"));
			CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
            return(l_nStatus);
        }              
    }catch(...)
    {
		CString l_error_string(_T("Exception occured in OnView"));
		CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
        return(E_ERR_EXCEPTION);
    }
    return(E_NO_ERROR);
}

int CKtReconController::OnKtRecon()
{
    try{
        if(NULL == m_ExecProc){
			CString l_error_string(_T("CKtReconController():m_ExecProc is NULL"));
			CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
            return(E_ERROR_NULL_FUNCPARAMS);
        }
        
        int l_nStatus = E_NO_ERROR;
        CString l_csCommand(_T(""));
        
        CKtCommandProcessor* l_CmdProc;
        CKtVROMProc l_VROMProc;        
        l_CmdProc = &l_VROMProc;
        
        l_nStatus = l_CmdProc->GetCommand(m_csStudyFileName,m_nSelectedProtIndex,l_csCommand);
        if(E_NO_ERROR != l_nStatus){
			CString l_error_string(_T("Error occurred in GetCommand for VROMProc"));
			CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
            return(l_nStatus);
        }
        
        l_nStatus = m_ExecProc->InvokeExec(VROMPROCESSOR_INIT,l_csCommand,false);
        if(E_NO_ERROR != l_nStatus){
			CString l_error_string(_T("Error occured in InvokeExec"));
			CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
            return(l_nStatus);
        }        
        
        CKtReconViewer l_ReconVwr;  
        l_CmdProc = &l_ReconVwr;
        l_csCommand.Empty();
        l_ReconVwr.SetIsCalledFromKtReconButton(true);        
        l_nStatus = l_CmdProc->GetCommand(m_csStudyFileName,m_nSelectedProtIndex,l_csCommand);
        l_ReconVwr.SetIsCalledFromKtReconButton(false); 
        if(E_NO_ERROR != l_nStatus){
			CString l_error_string(_T("Error occurred in GetCommand for ktReconViewer"));
			CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
            return(l_nStatus);
        }
        
        l_nStatus = m_ExecProc->InvokeExec(KTRECONVIEWER_INIT,l_csCommand,true);
        if(E_NO_ERROR != l_nStatus){
			CString l_error_string(_T("Error occured in InvokeExec"));
			CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
            return(l_nStatus);
        }       
    }catch(...)
    {
		CString l_error_string(_T("Exception occured in OnKtRecon"));
		CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
        return(E_ERR_EXCEPTION);
    }
    return(E_NO_ERROR);
}

int CKtReconController::OnGetData()
{
    try{
        if(NULL == m_ExecProc){
			CString l_error_string(_T("CKtReconController():m_ExecProc is NULL"));
			CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
            return(E_ERROR_NULL_FUNCPARAMS);
        }
        
        int l_nStatus = E_NO_ERROR;
        CString l_csCommand(_T(""));
        
        CKtCommandProcessor* l_CmdProc;
        CKtEngineDataCopy l_CEngnDataCopy;    
        l_CmdProc = &l_CEngnDataCopy;
        
        l_nStatus = l_CmdProc->GetCommand(m_csStudyFileName,m_nSelectedProtIndex,l_csCommand);
        if(E_NO_ERROR != l_nStatus){
			CString l_error_string(_T("Error occured in GetCommand"));
			CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
            return(l_nStatus);
        }
        
        l_nStatus = m_ExecProc->InvokeExec(RAWCOPY_INIT,l_csCommand,false);
        if(E_NO_ERROR != l_nStatus){
			CString l_error_string(_T("Error occurred in InvokeExec"));
			CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
            return(l_nStatus);
        }      
    }catch(...)
    {
		CString l_error_string(_T("Exception occured in OnGetData"));
		CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
        return(E_ERR_EXCEPTION);
    }
    return(E_NO_ERROR);
}
int CKtReconController::OnExit()
{
	//	CKtDPSCommMgr::exit = 1;
    if(NULL == m_ExecProc){
		CString l_error_string(_T("CKtReconController():CKtReconController():m_ExecProc is NULL"));
		CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
        return(E_ERROR);
    }
    
    m_ExecProc->ExitProc();
	CKtDPSCommMgr *l_server = NULL;
	l_server = CKtDPSCommMgr::GetInstance();
			 CString l_target = _T("");
			 l_target = l_server->GetParentName();
			 l_target += _T(".AdvCardiacGL");
			 // AfxMessageBox(l_target);
			 l_server->Connect(l_target);
			 IMplusMessage *l_msg = NEW_MSG();;
			 l_msg->SetCommand(_T("FW_RequestClose"));
			 l_msg->SetParameter("KtRecon");
			 l_msg->SetCustom(_T(""));
			 CString sender = l_server->GetParentName();
			 sender+= ".";
			 sender+= "KtRecon";
			 l_msg->SetSender(_bstr_t(sender));
			 l_server->SendMsg(l_msg);
			 l_server->Disconnect();
			 DEL_MSG(l_msg);
			 return(E_NO_ERROR);
}

void CKtReconController::SetDlg( CKtReconDlg* f_dlg )
{
	if (f_dlg!= NULL)
	{
		m_KtReconDlg = f_dlg;
	} else {
		//log error
	}
}

// CKtReconController * CKtReconController::GetKtController()
// {
//     try{
//         
//     }catch(...)
//     {
//         
//     }
// }

int CKtReconController::GetDataForDisplayingScannedProtocol(/*in*///vector <CString> & f_csImgLOID,
															// vector<CString> & f_csSeriesLoidVector,
															// vector<long> & f_nSeriesNumberVector,
                                                            vector<long> & f_nProtocolNumberVector,
                                                            //CString & f_source_series,
                                                            int & f_nTotalProtocolCount
                                                            //int & f_nTotalSeriesCount
															)
{
    try{
        //vector<CString> f_csImgLOID;
        CString l_csSeriesLOID = _T("");
		int  f_nTotalSeriesCount = 0;
        CString l_csStudyLOID = _T("");
        vector<CString> l_csAcqVec;
        int l_nStatus = E_NO_ERROR;
        LPCTSTR *l_SeriesLOIDArray = NULL;
        long l_nLastProtocolNumber = 0;
        bool protocolFound = true;
		
        CLocalDBMgr localdb;
#if 0
        localdb.GetParentLOID(f_csImgLOID.at(0), LOID_TYPE_IMAGE, l_csSeriesLOID);
        localdb.GetParentLOID(l_csSeriesLOID, LOID_TYPE_SERIES, l_csStudyLOID);
        long current_Series = 0;
        int status = 0;
        status = localdb.GetSeriesNumberFromDB(l_csSeriesLOID,&current_Series);
        if (status != E_NO_ERROR)
        {
			CString l_error_string(_T("Error occurred in GetSeriesNumberFromDB"));
			CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
            return(status);
        }
        int proc_num = 0;
        proc_num = current_Series%1000;
        current_Series -= proc_num;
        f_source_series.Format(_T("%d"),(current_Series /*- proc_num*/));
#endif
        l_nStatus = localdb.GetProtAcqOrderFromDB
            (m_csstudyloid, &f_nTotalSeriesCount, l_csAcqVec, &l_SeriesLOIDArray);
        
        if((E_NO_ERROR != l_nStatus) || (NULL == l_SeriesLOIDArray) ) {
            
            if(NULL != l_SeriesLOIDArray) {
                delete [] l_SeriesLOIDArray;
                l_SeriesLOIDArray = NULL;
            }
			CString l_error_string(_T("Error occured in GetProtAcqOrderFromDB"));
			CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
            return(l_nStatus);
        }
        
        for(int l_ni = 0; l_ni < f_nTotalSeriesCount; l_ni++) {
            CString l_csSeriesLD(l_SeriesLOIDArray[l_ni]);
			// f_csSeriesLoidVector.push_back(l_csSeriesLD);
            long l_nSeriesLOID = 0;
            l_nSeriesLOID = _ttol(l_csSeriesLD);
            long l_nProtocolNumber = 0;
            l_nStatus = localdb.GetSeriesNumberFromDB(l_csSeriesLD, &l_nProtocolNumber); 
            
            if(E_NO_ERROR != l_nStatus) {
                if(NULL != l_SeriesLOIDArray) {
                    delete [] l_SeriesLOIDArray;
                    l_SeriesLOIDArray = NULL;
                }
				CString l_error_string(_T("Error occured in GetSeriesNumberFromDB"));
				CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
                return(l_nStatus);
            }
            
			// f_nSeriesNumberVector.push_back(l_nProtocolNumber);
#if 1
            if((abs(l_nProtocolNumber - l_nLastProtocolNumber) < 1000) ){ 
                protocolFound = false;
            } else {
                protocolFound = true;
                f_nTotalProtocolCount++;
                f_nProtocolNumberVector.push_back(l_nProtocolNumber);
                l_nLastProtocolNumber = l_nProtocolNumber;
            }
#endif
        }
        
        if(NULL != l_SeriesLOIDArray) {
            delete [] l_SeriesLOIDArray;
            l_SeriesLOIDArray = NULL;
        }
        
		//        f_csImgLOID.clear();
        l_csAcqVec.clear();
    }catch(...) {
		CString l_error_string(_T("Exception occured in GetDataForDisplayingScannedProtocol"));
		CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
        return(E_ERR_EXCEPTION);
    }
    return E_NO_ERROR;
}
#if 0

int CKtReconController::OnRecvMsgFromIsel(const CString &f_csICID)
{
    try{
        //get the loid of selected images from iselector
        CKtICMMgr l_mgr;
        int l_nStatus = E_NO_ERROR;        
        CString csParam = CString(static_cast<LPCTSTR>(f_csICID));
        vector<CString> f_csImgLOID;
		
        l_nStatus = l_mgr.GetLOIDSelectedImagesFromIsel(csParam,f_csImgLOID);
        if(E_NO_ERROR != l_nStatus){
			CString l_error_display = _T("");
			l_error_display = _T("GetLOIDSelectedImagesFromIsel failed");
			CKtReconLogHelper::WriteToErrorLog(l_error_display, __LINE__);
            //log error
            if(m_KtReconDlg && m_KtReconDlg->GetSafeHwnd()){
                m_KtReconDlg->DisplayErr(l_nStatus);
            }
            return(E_ERROR);
        }
		
        //get the study file name
        CString l_csStudyFileName(_T(""));
        l_nStatus = l_mgr.GetStudyFileName(csParam,m_csStudyFileName,f_csImgLOID);
        if(E_NO_ERROR != l_nStatus){
			CString l_error_display = _T("");
			l_error_display = _T("GetStudyFileName failed");
			CKtReconLogHelper::WriteToErrorLog(l_error_display, __LINE__);
            //log error
            if(m_KtReconDlg){
                l_nStatus = KT_RECON_STUDYNAME_ERROR;
                m_KtReconDlg->DisplayErr(l_nStatus);
                return(E_ERROR);
            }
		}  
		
        //set the loid in controller
        l_nStatus = SetImgLoidInController(f_csImgLOID);
        if(E_NO_ERROR != l_nStatus){
			CString l_error_display = _T("");
			l_error_display = _T("SetImgLoidInController failed");
			CKtReconLogHelper::WriteToErrorLog(l_error_display, __LINE__);
            //log error
            if(m_KtReconDlg){
                m_KtReconDlg->DisplayErr(l_nStatus);
            }
            return(E_ERROR);
        }       
		
        //PN Start #########################################
        int l_nTotalProtocolCount = 0;
        int l_nTotalSeriesCount = 0;
        vector <CString> l_csSeriesLoidVector;
        vector <long> l_csProtocolNumberVector;
        vector <long> l_csSeriesNumberVector;
        CString csSourceProt (_T(""));
		
        l_nStatus = GetDataForDisplayingScannedProtocol(f_csImgLOID,
			l_csSeriesLoidVector, 
			l_csSeriesNumberVector, 
			l_csProtocolNumberVector, 
			csSourceProt,
			l_nTotalProtocolCount, 
			l_nTotalSeriesCount);
		
        if(E_NO_ERROR != l_nStatus){
            CString l_cserrmsg = (_T("GetDataForDisplayingScannedProtocol Failed."));
			CKtReconLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__);
            l_nTotalProtocolCount = 0;
            l_nTotalSeriesCount = 0;
            //log error
            if(m_KtReconDlg){
                m_KtReconDlg->DisplayErr(l_nStatus);
            }
            return(E_ERROR);            
        }
		
        if(l_nTotalProtocolCount > 0) {
            CString strItem = _T("");
            LVITEM lv;
            for(int l_nCnt_i = 0; l_nCnt_i < l_nTotalProtocolCount; l_nCnt_i++) {
                strItem.Format(_T("%d"), l_csProtocolNumberVector.at(l_nCnt_i));
                lv.iItem = l_nCnt_i;
                lv.iSubItem = 0;
                lv.pszText = strItem.GetBuffer(strItem.GetLength());
                lv.mask = LVIF_TEXT;
                m_KtReconDlg->FillProtocolListBox(lv);
            }
            m_csSeriesLoid = l_csSeriesLoidVector;
            m_nsSeriesNumberArray = l_csSeriesNumberVector;
        }else{
            //log error //recheck the message to be written to the log file
			
            if(m_KtReconDlg){
                l_nStatus = KTRECON_NO_SCANNED_PROTOCOL;
                m_KtReconDlg->DisplayErr(l_nStatus);
            }
			CString l_error_display = _T("");
			l_error_display = _T("There is no scaned protocol");
			CKtReconLogHelper::WriteToErrorLog(l_error_display, __LINE__);
            return(E_ERROR);
        }
		
        l_csSeriesLoidVector.clear();
        l_csSeriesNumberVector.clear();
        l_csProtocolNumberVector.clear();
        //PN End ###############################################        
    }catch(...)
    {
        //log error
		CString l_error_display = _T("");
		l_error_display = _T("Exception occured in OnRecvMsgFromIsel");
		CKtReconLogHelper::WriteToErrorLog(l_error_display, __LINE__);
        return(E_ERR_EXCEPTION);
    }
    return(E_NO_ERROR);
}
#endif

int CKtReconController::SetImgLoidInController( /*in*/vector <CString> & f_csImgLOID )
{
    try{
        vector<CString>::iterator l_csit1;
        m_csImageLoid.clear();
        l_csit1 = f_csImgLOID.begin();
        
        for(l_csit1 = f_csImgLOID.begin() ; l_csit1 < f_csImgLOID.end(); l_csit1++) {
            m_csImageLoid.push_back(*l_csit1);
        }
    }catch(...)
    {
        //log error
		CString l_error_display = _T("");
		l_error_display = _T("Exception occured in SetImgLoidInController");
		CKtReconLogHelper::WriteToErrorLog(l_error_display, __LINE__);
        return(E_ERR_EXCEPTION);
    }
    return(E_NO_ERROR);
}

//+Mpower/20130812/PN/added
/*************************************************************
* Method:       SetWaitCursor
* Description:  This function sets Wait cursor
* Parameter:    bool flag
*               indicating whether to display Wait cursor or not
* Returns:      void
*************************************************************/
void CKtReconController::SetWaitCursor(bool f_flag)
{
    m_is_wait_cursor = f_flag;
    
    if(NULL == m_KtReconDlg) {
        CKtReconLogHelper::WriteToErrorLog(_T("SetWaitCursor(): m_KtReconDlg is NULL"), __LINE__);
    }
    if (m_KtReconDlg && m_KtReconDlg->GetSafeHwnd()) 
		m_KtReconDlg->SendMessage(WM_SETCURSOR, 0, 0);
//	AfxGetApp()->DoWaitCursor(1);
}

/*************************************************************
* Method:       IsWaitCursor
* Description:  This function returns whether current cursor
*               is Wait Cursor
* Returns:      bool
*************************************************************/
bool CKtReconController::IsWaitCursor()
{
    return m_is_wait_cursor;
}

//-Mpower/20130812/PN/added
/////////////////////////////////END OF FILE /////////////////////////////////////

int CKtReconController::GetStudyLoidFromPager()
{
	CKtDPSCommMgr *l_server = NULL;
	l_server = CKtDPSCommMgr::GetInstance();
			 CString l_target = _T("Pager");
			 l_server->Connect(l_target);
			 IMplusMessage *l_msg = NEW_MSG();;
			 l_msg->SetCommand(_T("GL_Pager_GetPageInfo"));
			 l_msg->SetParameter("");
			 l_msg->SetCustom(_T(""));
			 CString sender = l_server->GetParentName();
			 sender+= ".";
			 sender+= "KtRecon";
			 l_msg->SetSender(_bstr_t(sender));
			 l_server->SendMsg(l_msg);
			 l_server->Disconnect();
			 DEL_MSG(l_msg);
			 SetWaitCursor(true);
			 return 0;
}

int CKtReconController::SetStudyFileLoid(CString &Studyloid)
{
	if (!Studyloid.IsEmpty()) {
		m_csstudyloid = Studyloid;
		if (m_KtReconDlg && m_KtReconDlg->GetSafeHwnd())	m_KtReconDlg->LoadScannedListBox();
		
	}
	SetWaitCursor(false);
	//AfxGetApp()->DoWaitCursor(-1);
	//	m_csStudyFileName = csStudyName;
	// CKtReconLogHelper::WriteToErrorLog(m_csStudyFileName, __LINE__,__FILE__,ERRORVIEW_DISPLAY);
	
	return 0;
}

void CKtReconController::SetProtocolNumber(int prot_no)
{
	m_nSelectedProtIndex  = prot_no;
}

int CKtReconController::StartGUI(CWnd *parent)
{
	return 0;
}

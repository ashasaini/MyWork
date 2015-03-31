// ACGAppController.cpp: implementation of the ACGAppController class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ACGAppController.h"
#include "DPSComManager.h"
#include "CommonDefinitions.h"
#include "ACGLConfigReader.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

ACGAppController* ACGAppController::m_ACGController = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CACGLConfigReader;

ACGAppController::ACGAppController()
{
    for(int index = 0; index < NUMBER_OF_TOOLS; index++){
        SetButtonPressedStatus(0,index);
    }
    
    m_KeyValuePairs.clear();
    m_nToolNum = 0;
    m_csToolName.Empty();
	isExitClcked = false;
}

ACGAppController::~ACGAppController()
{
    if(NULL != m_ACGLDlg){
        delete m_ACGLDlg;
        m_ACGLDlg = NULL;
    }
    
    if(NULL != m_ACGController){
        delete m_ACGController;
        m_ACGController = NULL;
    }
    
    m_KeyValuePairs.clear();
}

int ACGAppController::InvokeApplication()
{
    try{
        CDPSComManager *commgr = CDPSComManager::GetInstance();
        CString l_str;        
        
        CString custom = _T("1/0"); //1/startup
        CString param = _T("300/300/700/500");
        
        l_str.Format(_T("%s/%s.Document/%s"),m_csToolName,m_csToolName,(param));
        
        CString parentname = commgr->GetParentName();//PAGER;_T("Acquire");//_T("Display1"); //TiPrep
        commgr->Connect(parentname);
        IMplusMessage *l_msg = NULL;
        CString sender = parentname;//commgr->GetParentName();//PAGER;
        sender+= SEP;
        sender+= APP_NAME;//m_csToolName;//_T("Display1.TiPrep");
        
        l_msg = NEW_MSG();
        l_msg->SetSender(/*_bstr_t(sender)*/_T(""));
        l_msg->SetCommand(_bstr_t(_T("FW_LoadGL")));
        l_msg->SetCustom(_bstr_t(custom));
        l_msg->SetParameter(_bstr_t(l_str)); 
        
        //l_msg=commgr->CreateMessage(FW_LOAD_GL,0,_bstr_t(sender),_bstr_t(l_str),_bstr_t(custom));
        if (l_msg){
            commgr->SendMsg(l_msg);
            commgr->Disconnect();
            DEL_MSG(l_msg);
            l_msg = NULL;
        }        
    } catch(...) {
        CString l_csmsg(_T("Exception occurred in InvokeApplication"));
        CACGLLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
        return(E_ERROR);
    }
    return(E_NO_ERROR);
}

int ACGAppController::CloseTool()
{
    try{
        CDPSComManager *commgr = CDPSComManager::GetInstance();
        CString parentname =commgr->GetParentName();;//PAGER;//_T("Display1");
        CString app_name = APP_NAME;//_T("AdvCardiacGL");
		parentname += _T(".MPlusClinAppToolBox"); //Check if issues only in toolbox
        commgr->Connect(parentname);
        IMplusMessage *l_msg = NEW_MSG();;
        l_msg->SetCommand(_T("FW_RequestClose"));
        l_msg->SetParameter(_bstr_t(app_name));
        l_msg->SetCustom(_T(""));
        CString sender = commgr->GetParentName();;
        l_msg->SetSender(/*_T("Display1.TiPrep")*/_bstr_t(sender));
        //MessageBox(NULL,sender,NULL,NULL);
        commgr->SendMsg(l_msg);
        commgr->Disconnect();
        DEL_MSG(l_msg);
    }catch(...) {
        CString l_csmsg(_T("Exception occurred in CloseTool"));
        CACGLLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
        return(E_ERROR);
    }    
	return(E_NO_ERROR);
}

int ACGAppController::SwitchPageScene(CString& f_PageScene)
{   
    try{        
        CDPSComManager *commgr = CDPSComManager::GetInstance();
        CString command, parameter, parentname,custom,sender;
        
        parentname =commgr->GetParentName();;//PAGER;//_T("Display1");
        parentname += _T(".Controller");
        commgr->Connect(parentname);
        IMplusMessage *l_msg = NEW_MSG();
                
        l_msg->SetCommand(_T("FW_SwitchSceneBranch"));
        l_msg->SetParameter(_bstr_t(f_PageScene));
        l_msg->SetCustom(_T(""));
        /*CString*/ sender = commgr->GetParentName();;//PAGER;
        sender+= SEP;
        sender+= APP_NAME;
        l_msg->SetSender(/*_T("Display1.TiPrep")*/_bstr_t(sender));
        commgr->SendMsg(l_msg);
        //  commgr->Disconnect();
        DEL_MSG(l_msg);
    }catch(...) {
        CString l_csmsg(_T("Exception occurred in SwitchPageScene"));
        CACGLLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
        return(E_ERROR);
    }
    return(E_NO_ERROR);
}
#if 0
int ACGAppController::RegisterMsg(int branch)
{
    try{
        UNREFERENCED_PARAMETER(branch);
        CDPSComManager *commgr = CDPSComManager::GetInstance();
        CString parentname =commgr->GetParentName();;//PAGER;//_T("Display1");
        //parentname += _T(".MPlusClinAppToolbox");
        CString app_name = _T("AdvCardiacGL");
        parentname += _T(".Controller");
        commgr->Connect(parentname);
        IMplusMessage *l_msg = NEW_MSG();;        
        l_msg->SetCommand(_T("FW_SubscribeMsg"));        
        l_msg->SetCustom(_T("FW_Hide"));
        CString sender = commgr->GetParentName();;//PAGER;
        sender+= SEP;
        sender+= app_name;
        l_msg->SetSender(/*_T("Display1.TiPrep")*/_bstr_t(sender));
        l_msg->SetParameter(/*_T("TiPrep")*/_bstr_t(app_name));
        commgr->SendMsg(l_msg);
        commgr->Disconnect();
        DEL_MSG(l_msg);
    }catch(...) {
        CString l_csmsg(_T("Exception occurred in RegisterMsg"));
        CACGLLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
        return(E_ERROR);
    }
    return(E_NO_ERROR);
}
#endif
int ACGAppController::QueryActivePage()
{
    try{
        CDPSComManager *commgr = CDPSComManager::GetInstance();
        
        CString parentname = commgr->GetParentName();//PAGER;//_T("Display1");
      //  parentname += _T(".Controller");
        CString target = parentname + _T(".Controller");
        commgr->Connect(target);
        IMplusMessage *l_msg = NEW_MSG();;
        l_msg->SetCommand(_T("FW_QueryCurrentSceneBranch"));
        l_msg->SetParameter(_T(""));
        l_msg->SetCustom(_T(""));
        CString sender = parentname;
        sender+= SEP;
        sender+= APP_NAME;
        l_msg->SetSender(/*_T("Display1.TiPrep")*/_bstr_t(sender));
        commgr->SendMsg(l_msg);
        commgr->Disconnect();
        DEL_MSG(l_msg);
    }catch(...) {
        CString l_csmsg(_T("Exception occurred in QueryActivePage"));
        CACGLLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
        return(E_ERROR);
    }
    return(E_NO_ERROR);
}

ACGAppController* ACGAppController::GetInstance()
{
    try{
        if(NULL == m_ACGController) {
            m_ACGController = new ACGAppController();
            
            if(NULL == m_ACGController) {
                CString l_cserr = _T("Memory allocation failed for ACGAppController.");
                CACGLLogHelper::WriteToErrorLog(l_cserr, __LINE__, __FILE__);
            }
        }
    }catch(...) {
        CString l_csmsg(_T("Exception occurred in GetInstance"));
        CACGLLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
    }
    return m_ACGController;
}

void ACGAppController::DeleteInstance()
{
    try{
        if(m_ACGController) {
            delete m_ACGController;
            m_ACGController = NULL;
        }
    }catch(...) {
            CString l_csmsg(_T("Exception occurred in DeleteInstance"));
            CACGLLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
    }	
}



ACGAppController& ACGAppController::operator=( ACGAppController& f_ACGController )
{
    try {
        if(this == &f_ACGController) {
            return(*this);
        }
        //pending
    }catch(...) {
        CString l_csmsg(_T("Exception occurred in ACGAppController assignment operator"));
        CACGLLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
    }    
    
    return (*this);
}

int ACGAppController::ReadToolConfigSettingsFromFile()
{
    try{
        CACGLConfigReader l_ConfigReader;    
        vector<KeyValuePair> l_KeyValuePairs;
        
        int l_nStatus = l_ConfigReader.ReadToolConfigSettings(l_KeyValuePairs);
        if(E_NO_ERROR != l_nStatus){
            CString l_csStr(_T("ReadToolConfigSettings returned error"));
            CACGLLogHelper::WriteToErrorLog(l_csStr, __LINE__, __FILE__);
            return(E_ERROR);
        }        
        
		l_nStatus = SetConfigSettings(l_KeyValuePairs);
		if(E_NO_ERROR != l_nStatus){
			CString l_csStr(_T("SetConfigSettings returned error"));
			CACGLLogHelper::WriteToErrorLog(l_csStr, __LINE__, __FILE__);
			return(E_ERROR);
		}        
    }catch(...) {
        CString l_csmsg(_T("Exception occurred in ReadToolConfigSettingsFromFile"));
        CACGLLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
        return(E_ERROR);
    }
    return(E_NO_ERROR);    
}

int ACGAppController::GetPageScene( CString& f_csPageScene )
{
    try{
        //pending validate f_nPos
        
        const size_type count = m_KeyValuePairs.size();
        
        if((unsigned int)m_nToolNum > count){
            CString l_csmsg(_T("Incorrect function parameter received in GetPageScene"));
            CACGLLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
            return(E_ERROR);
        }        
        
        f_csPageScene.Empty();    
        KeyValuePair l_OneKeyValuePair;
        string l_csPageSceneName;
        
        l_OneKeyValuePair = m_KeyValuePairs[m_nToolNum];
        l_csPageSceneName = l_OneKeyValuePair.second;     
        
        f_csPageScene = l_csPageSceneName.c_str();    
    }catch(...) {
        CString l_csmsg(_T("Exception occurred in GetPageScene"));
        CACGLLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
        return(E_ERROR);
    }
    return(E_NO_ERROR);
}

int ACGAppController::GetConfigToolSettingsForDisplay(vector<KeyValuePair>& f_KeyValuePairs)
{
    try{
        vector<KeyValuePair>::iterator l_csit1;
        l_csit1 = m_KeyValuePairs.begin();
        f_KeyValuePairs.clear();
        
		if(0 != m_KeyValuePairs.size()){
			for(l_csit1 = m_KeyValuePairs.begin() ; l_csit1 < m_KeyValuePairs.end(); l_csit1++) {
				f_KeyValuePairs.push_back(*l_csit1);
			}
		}
    }catch(...) {
        CString l_csmsg(_T("Exception occurred in GetConfigToolSettingsForDisplay"));
        CACGLLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
        return(E_ERROR);
    }
    return(E_NO_ERROR);
}

int ACGAppController::SetConfigSettings( vector<KeyValuePair>& f_KeyValuePairs )
{
    try{
        vector<KeyValuePair>::iterator l_csit1;
        l_csit1 = f_KeyValuePairs.begin();
        m_KeyValuePairs.clear();
        
        for(l_csit1 = f_KeyValuePairs.begin() ; l_csit1 < f_KeyValuePairs.end(); l_csit1++) {
            m_KeyValuePairs.push_back(*l_csit1);
        }
    }catch(...) {
        CString l_csmsg(_T("Exception occurred in SetConfigSettings"));
        CACGLLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
        return(E_ERROR);
    }
    return(E_NO_ERROR);
}

int ACGAppController::SetToolNumber( int f_nToolNum )
{
	if((f_nToolNum >= 0) && (f_nToolNum < NUMBER_OF_TOOLS)){
		m_nToolNum = f_nToolNum;
	}else{
		CString l_csmsg(_T("Invalid number"));
        CACGLLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
        return(E_ERROR);
	}    
    return(E_NO_ERROR);
}

int ACGAppController::SetToolName( CString &f_csToolName )
{
    m_csToolName = f_csToolName;
    return(E_NO_ERROR);
}

int ACGAppController::SendGLToolShowHide(CString &tool,bool flag)
{
    try {
        CDPSComManager *commgr = CDPSComManager::GetInstance();
        CString command = _T("");
        CString parentname = commgr->GetParentName();//PAGER;//_T("Display1");
        if (flag) {command = _T("GL_Tool_Show");} else {command = _T("GL_Tool_Hide");}
        CString target = parentname + _T(".") + tool;
        commgr->Connect(target);
        IMplusMessage *l_msg = NEW_MSG();;
        l_msg->SetCommand(_bstr_t(command));
        l_msg->SetParameter(_T(""));
        l_msg->SetCustom(_T(""));
        CString sender = parentname;
        sender+= SEP;
        sender+= APP_NAME;
        l_msg->SetSender(/*_T("Display1.TiPrep")*/_bstr_t(sender));
        commgr->SendMsg(l_msg);
        commgr->Disconnect();
        DEL_MSG(l_msg);
    }catch(...) {
        CString l_csmsg(_T("Exception occurred in QueryActivePage"));
        CACGLLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
        return(E_ERROR);
    }
    return(E_NO_ERROR);
}
int ACGAppController::ShowHideCardiacToolsByScene(CString &csscene)
{
    try{
            KeyValuePair l_OneKeyValuePair;
            CString l_csmsg(_T(""));
            //l_csmsg.Format(_T("ShowHideCardiacToolsByScene"));
            //CACGLLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);   
          
           for(int index = 0; index < NUMBER_OF_TOOLS ; index++) {            
               
               if(1 == m_ButtonPressedStatus[index]){
                   
                   l_OneKeyValuePair = m_KeyValuePairs.at(index);
                                     
                   CString l_csToolScene(l_OneKeyValuePair.second.c_str());
                   
                   if (-1 != csscene.Find(l_csToolScene)){
                       
                       CString l_csToolName(l_OneKeyValuePair.first.c_str());

                       CString l_csmsg(_T(""));
                       
                       SendGLToolShowHide(l_csToolName,true);
                   }else{

                       CString l_csToolName(l_OneKeyValuePair.first.c_str());

                       CString l_csmsg(_T(""));
                       SendGLToolShowHide(l_csToolName,false);
                   }
               }
           }
     }catch(...){
        CString l_csmsg(_T("Exception occurred in ShowHideCardiacToolsByScene"));
        CACGLLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
        return(E_ERROR);
        
    }
    return E_NO_ERROR;
}

int ACGAppController::SetButtonPressedStatus( int f_nStatus, int f_nIndex )
{
    if((f_nIndex < NUMBER_OF_TOOLS) && (f_nIndex >= 0)){
        m_ButtonPressedStatus[f_nIndex] = f_nStatus;
    }
    return(E_NO_ERROR);    
}

int ACGAppController::GetButtonPressedStatus( int &f_nStatus, int f_nIndex )
{
    if((f_nIndex < NUMBER_OF_TOOLS) && (f_nIndex >= 0)){
        f_nStatus = m_ButtonPressedStatus[f_nIndex];
    }
    return(E_NO_ERROR);
}

void ACGAppController::AddZOrderLayer()
{
    try {
        CDPSComManager *pqm = NULL;
        pqm = CDPSComManager::GetInstance();
        
        if(pqm != NULL) {
            IMplusMessage *l_msg = NULL;
            CString l_parameter = _T("");
            l_parameter += _T("Z_ORDER_PANEL_GL_LAYER");
            l_parameter += _T("/");
            l_parameter += _T("Z_ORDER_ISEL_LAYER");//Add the layer after Z_ORDER_ISEL_LAYER
            BSTR l_bstr_parameter = _bstr_t(l_parameter);
            pqm->Connect(CString(pqm->GetParentName()));
            l_msg = pqm->CreateMessage(FW_AddZOrderLayer, 1, pqm->GetParentName(), l_bstr_parameter);
            
            if(l_msg != NULL) {
                pqm->SendMsg(l_msg);
                DEL_MSG(l_msg);
            }
        } else {
            CString l_cserrmsg = (_T("CDPSComManager NULL pointer"));
            CACGLLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
        }
    } catch(...) {
        CACGLLogHelper::WriteToErrorLog(_T("AddZOrderLayer(): Exception occured"), __LINE__, __FILE__);
    }
}

BOOL ACGAppController::RegisterWindowZOrder(HWND hWnd_i)
{
    BOOL l_result = FALSE;
    
    CDPSComManager *l_server = CDPSComManager::GetInstance();
    
    if( l_server != NULL ){
        
        IMplusMessage *l_msg = NULL;
        
        CString l_sparent(_T(""));
        l_sparent.Format(_T("%s"),(LPCTSTR)(l_server->GetParentName()));
        l_server->Connect(l_sparent);
        
        //l_server->Connect(CString(l_server->GetParentName()));
        
        l_msg = l_server->CreateMessage(FW_RegisterWindowZOrder,1,l_server->GetParentName()); 
        
        CString l_parameter = _T("");
        l_parameter.Format(_T("%ld/%s"), hWnd_i,_T("Z_ORDER_FLOAT_GL_LAYER"));
        l_parameter += _T("/");
        l_parameter += _T("Top");
        
        l_msg->SetParameter( _bstr_t(l_parameter));
        if (!l_server->SendMsg(l_msg) ) {
            l_result = FALSE;
        }
        
        DEL_MSG(l_msg);
        l_msg = NULL;
        l_server->Disconnect();
    }		
    return l_result;	
}

BOOL ACGAppController::UnRegisterWindowZOrder(HWND hWnd_i)
{
    try{        
        BOOL l_result = FALSE;
        CDPSComManager *l_server = CDPSComManager::GetInstance();
        
        if(l_server != NULL) {
            IMplusMessage *l_msg = NULL;
            BSTR l_bsparent = NULL;
            l_bsparent = l_server->GetParentName();
            
            if(NULL != l_bsparent) { 
                l_server->Connect(CString(l_bsparent)); 
                l_msg = l_server->CreateMessage(FW_UnregisterWindowZOrder, 1, l_server->GetParentName());
                CString l_parameter = _T("");
                l_parameter.Format(_T("%ld/%s"), hWnd_i, _T("Z_ORDER_FLOAT_GL_LAYER"));
                
                l_msg->SetParameter(_bstr_t(l_parameter));
                if(l_msg != NULL) {
                    if(!l_server->SendMsg(l_msg)) {
                        l_result = FALSE;
                    }
                    DEL_MSG(l_msg);
                    l_msg = NULL;
                }
                
                l_server->Disconnect();
            } else {
                CString l_cserr(_T("DPSComm Parent is NULL "));
                CACGLLogHelper::WriteToErrorLog(l_cserr, __LINE__);
            }
        } else { 
            CString l_cserr(_T("CDPSComManager is NULL "));
            CACGLLogHelper::WriteToErrorLog(l_cserr, __LINE__);
            return FALSE;
        }
        
        return l_result;
    }catch(...) {
        CString l_csStr(_T("Exception occured in "));
        CACGLLogHelper::WriteToErrorLog(l_csStr, __LINE__, __FILE__);
        return FALSE;
    }
}

BOOL ACGAppController::ChangeWindowZOrder(HWND hWnd_i)
{    
    BOOL l_result = FALSE;    
    CDPSComManager *l_server = CDPSComManager::GetInstance();
    
    if( l_server != NULL ){
        
        IMplusMessage *l_msg = NULL;
        CString l_sparent(_T(""));
        l_sparent.Format(_T("%s"),(LPCTSTR)(l_server->GetParentName()));
        l_server->Connect(l_sparent);
        //l_server->Connect(CString(l_server->GetParentName()));
        
        l_msg = l_server->CreateMessage(FW_ChangeWindowZOrder,1,l_server->GetParentName()); 
        
        CString l_parameter = _T("");
        l_parameter.Format(_T("%ld/%s"), hWnd_i,_T("Z_ORDER_FLOAT_GL_LAYER"));
        l_parameter += _T("/");
        l_parameter += _T("Top");
        
        l_msg->SetParameter( _bstr_t(l_parameter));
        
        if (!l_server->SendMsg(l_msg) ) {
            l_result = FALSE;
        }
        
        DEL_MSG(l_msg);
        l_msg = NULL;
        l_server->Disconnect();              
    }		
    
    return l_result;	
}
///////////////////////////////// END OF FILE /////////////////////////////////



int ACGAppController::UpdatedInvokedToolList(CString &csClosingtool)
{
 KeyValuePair l_OneKeyValuePair;
            
           for(int index = 0; index < NUMBER_OF_TOOLS ; index++) {            
               
               if(1 == m_ButtonPressedStatus[index]){
//                    CString l_csmsg(_T(""));
//                    l_csmsg.Format(_T("ShowHideCardiacToolsByScene: ******** m_ButtonPressedStatus = %d index = %d"),m_ButtonPressedStatus[index],index);
//                    CACGLLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
                   
                   l_OneKeyValuePair = m_KeyValuePairs.at(index);
                                     
                          CString l_csToolName(l_OneKeyValuePair.first.c_str());
                   
                   if (-1 != csClosingtool.Find(l_csToolName)){
					   m_ButtonPressedStatus[index] = 0;
				   }
			   }
                       
		   }
		   return 0;


}

bool ACGAppController::IsAnyToolRunning()
{
 for(int index = 0; index < NUMBER_OF_TOOLS ; index++) {     
		  if (1 == m_ButtonPressedStatus[index]) {
			  return true;
		  }
	  }
	
return false;
}

int ACGAppController::QueryActiveDisplayPage()
{
try{
        CDPSComManager *commgr = CDPSComManager::GetInstance();
        
        CString parentname = commgr->GetParentName();//PAGER;//_T("Display1");
      //  parentname += _T(".Controller");
        CString target = _T("Pager");
        commgr->Connect(target);
        IMplusMessage *l_msg = NEW_MSG();;
        l_msg->SetCommand(_T("FW_QueryActivePage"));
        l_msg->SetParameter(_T(""));
        l_msg->SetCustom(_T(""));
        CString sender = parentname;
        sender+= SEP;
        sender+= APP_NAME;
        l_msg->SetSender(/*_T("Display1.TiPrep")*/_bstr_t(sender));
        commgr->SendMsg(l_msg);
        commgr->Disconnect();
        DEL_MSG(l_msg);
    }catch(...) {
        CString l_csmsg(_T("Exception occurred in QueryActiveDisplayPage"));
        CACGLLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
        return(E_ERROR);
    }
    return(E_NO_ERROR);
}

int ACGAppController::SendCloseToTools()
{
try{
	CDPSComManager *commgr = CDPSComManager::GetInstance();
	
	CString parentname = commgr->GetParentName();//PAGER;//_T("Display1");
	//  parentname += _T(".Controller");
	CString target = _T("");
	 KeyValuePair l_OneKeyValuePair;
	for (int index = 0; index <NUMBER_OF_TOOLS; index++) {
		if(1 == m_ButtonPressedStatus[index]){
			l_OneKeyValuePair = m_KeyValuePairs.at(index);
			
			CString l_csToolName(l_OneKeyValuePair.first.c_str());
			target  = parentname + _T(".")+l_csToolName;
			commgr->Connect(target);
			IMplusMessage *l_msg = NEW_MSG();;
			l_msg->SetCommand(_T("Wakeup_CloseTool"));
			l_msg->SetParameter(_T(""));
			l_msg->SetCustom(_T(""));
			CString sender = parentname;
			sender+= SEP;
			sender+= APP_NAME;
			l_msg->SetSender(/*_T("Display1.TiPrep")*/_bstr_t(sender));
			commgr->SendMsg(l_msg);
			commgr->Disconnect();
			DEL_MSG(l_msg);
		}
	}
}catch(...) {
	CString l_csmsg(_T("Exception occurred in QueryActiveDisplayPage"));
	CACGLLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
	return(E_ERROR);
}
return(E_NO_ERROR);
}

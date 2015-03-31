// KtICMMgr.cpp: implementation of the CKtICMMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KtICMMgr.h"
#include "KtDPSCommMgr.h"
#include "LocalDBMgr.h"
#include "KtReconErrors.h"
#include <vector>

#include <iostream>
#include "ImageCollectionMgr/CImageCollectionManager.h"
#include "ImageCollectionMgr/CImageLeaf.h"
#include "GLCommon/ICManagerHelper.h"
#include "GLCommon/ScopedICMPtr.h"
#include "KtReconLogHelper.h"

using namespace MPlus::DPS::GLCommon;
using namespace std;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKtICMMgr::CKtICMMgr()
{
    
}

CKtICMMgr::~CKtICMMgr()
{
    
}

BOOL CKtICMMgr::GetSelectedImages()
{   
    try {
        CKtDPSCommMgr *l_server = NULL;
        l_server = CKtDPSCommMgr::GetInstance();
        
        if(NULL == l_server) {
            CKtReconLogHelper::WriteToErrorLog
                (_T("GetSelectedImages(): l_server is NULL"), __LINE__, __FILE__);
            return FALSE;
        }
        
        IMplusMessage *l_msg = NULL;
        CString l_target;
        l_target.Empty();
        l_target = l_server->GetParentName();
        l_target += ".";
        l_target += _T("iselector");
        l_server->Connect(l_target);
        l_msg = l_server->CreateMessage(GL_IS_Get_Selected_IC_Name, 1, _bstr_t(l_target));
        
        if(l_msg != NULL) {
            l_server->SendMsg(l_msg);
            DEL_MSG(l_msg);
            l_msg = NULL;
        } else {
            return FALSE;
        }
        
        l_server->Disconnect();
        m_is_wait_request_isel = true;
    } catch(...) {
        CString l_error_string(_T("Exception occured in GetSelectedImages"));
        CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
        return FALSE;
    }
    
    return TRUE;
}

int CKtICMMgr::GetStudyFileName(const CString &f_csICID,CString &f_csStudyFileName,/*in*/vector <CString> & f_csImgLOID)
{
    try {
        //####### NOTE 
        //code commented below is not required
        //it was implemented earlier but later it was changed

        // first get the current selected images
//         ICManagerHelper* pICMgr = ICManagerHelper::GetInstance();
//         
//         if(NULL == pICMgr) {
//             CString l_csErrMsg(_T("GetStudyFileName(): pICMgr NULL Pointer"));
//             CKtReconLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__);
//             return (E_ERROR);
//         }
//         
//         ScopedImageCollectionPtr pInputIC(pICMgr->QueryImageCollection(f_csICID));
//         
//         //vector <CString> l_csImageLoid;
//         CString csImageLoid = _T("");        
//         
         CString l_csProtocolName(_T(""));
         CString l_csReconProt_Org(_T(""));
         CString l_csStudyName(_T(""));
         
         
         CLocalDBMgr l_localdbmgr;
         CString l_csCompletepath;
//         
//         ScopedImageNodePtr pScopeImageNode(pInputIC->GetAt(0));
//         
//         if(!pScopeImageNode->IsLeaf()) {
//             CString l_csErrMsg(_T("Image node not found"));
//             CKtReconLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__);
//             pICMgr->Release();
//             return (E_ERROR);
//         }
//         
//         pScopeImageNode->GetID(csImageLoid);
//         f_csImgLOID.push_back(csImageLoid);
        int l_ndbstat = 0;
        
        l_ndbstat = l_localdbmgr.GetProtocolNameFromImageLevelDB(f_csImgLOID.at(0), 
            l_csProtocolName,l_csReconProt_Org,l_csCompletepath);
        
        if(E_NO_ERROR != l_ndbstat){
            CString l_csErrMsg(_T("DB Error:Unable to get Protocol name from DB for selected Image"));
            CKtReconLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__);
            //pICMgr->Release();
            return (E_ERROR);
        }
        CString l_csTempStr(_T(""));
        l_csTempStr.Format(_T("%s"), l_csProtocolName);
        int cnt = l_csTempStr.Find(_T(':'));
        l_csStudyName = l_csTempStr.Left(cnt);
        
        cnt = l_csStudyName.Find(_T("Run"),0);
        int totalCnt = l_csStudyName.GetLength();
        l_csStudyName = l_csStudyName.Right(totalCnt - (cnt));
        
        
        int folder_break = 0;
        folder_break = l_csStudyName.Find('/');
        if (folder_break != -1)
            l_csStudyName = l_csStudyName.Left(folder_break);
        
        totalCnt = l_csStudyName.GetLength();
        cnt = l_csStudyName.Find(_T('.'));
        if (cnt != -1)
            l_csStudyName = l_csStudyName.Left(cnt);
        
        f_csStudyFileName = l_csStudyName;
        return E_NO_ERROR;        
        
    } catch(...) {
        CString l_error_string(_T("Exception occured in GetStudyFileName"));
        CKtReconLogHelper::WriteToErrorLog(l_error_string, __LINE__);
        return (E_ERR_EXCEPTION);
    }
    return(E_NO_ERROR);
}

int CKtICMMgr::GetLOIDSelectedImagesFromIsel(const CString &f_csICID,/*out*/vector <CString> &f_csImgLOID)
{
    try {
        // first get the current selected images
        ICManagerHelper* pICMgr = ICManagerHelper::GetInstance();
        
        if(NULL == pICMgr) {
            CString l_csErrMsg(_T("NULL Pointer"));
            //CKtReconLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return(E_ERROR_NULL_FUNCPARAMS);
        }
        
        ScopedImageCollectionPtr pInputIC(pICMgr->QueryImageCollection(f_csICID));
        
        // get the image count
        long nImageNum = pInputIC->GetNodeCount(FALSE);
        CString tmp = _T("");
        tmp.Format(_T("Number of Selected Images: %d"), nImageNum);
        OutputDebugString(tmp);
        
        vector <CString> l_csImageLoid;
        CString csImageLoid = _T("");        
        CString l_csProtNode_org(_T(""));
        CString l_csProtocolName(_T(""));
        CString l_csProtNode(_T(""));
        CString l_csReconProt_Org(_T(""));
        CString l_csReconProt(_T(""));
        int l_ni = 0;
        
        CLocalDBMgr l_localdbmgr;
        CString l_csCompletepath;
        
        for(l_ni = 0; l_ni < nImageNum; l_ni++) {
            ScopedImageNodePtr pScopeImageNode(pInputIC->GetAt(l_ni));
            
            if(!pScopeImageNode->IsLeaf()) {
                CString l_csErrMsg(_T("Image node not found"));
                //CKtReconLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                pICMgr->Release();
                return (E_ERROR);
            }
            
            pScopeImageNode->GetID(csImageLoid);
            l_csImageLoid.push_back(csImageLoid);
            int l_ndbstat = 0;
            
            if(l_ni == 0) {
                l_ndbstat = l_localdbmgr.GetProtocolNameFromImageLevelDB(l_csImageLoid.at(0), 
                    l_csProtocolName,l_csReconProt_Org,l_csCompletepath);
                
                if(l_ndbstat == E_NO_ERROR) {
                    l_csProtNode_org = l_csProtocolName; 
                } else {
                    CString l_csErrMsg(_T("DB Error:Unable to get Protocol name from DB for selected Image"));
                    //CKtReconLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                    pICMgr->Release();
                    return (E_IMG_TAG_PROT_FAIL);
                }
            } else {
                l_ndbstat = l_localdbmgr.GetProtocolNameFromImageLevelDB(l_csImageLoid.at(l_ni), 
                    l_csProtocolName,l_csReconProt,l_csCompletepath);
                
                if(l_ndbstat != E_NO_ERROR) {
                    CString l_csErrMsg(_T("DB Error:Unable to get Protocol name from DB for selected Image"));
                    //CKtReconLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                    pICMgr->Release();
                    return (E_IMG_TAG_PROT_FAIL);
                }
                
                l_csProtNode = l_csProtocolName;
                //                 if ((CTiPrepView::GetView())->GetISelEvent()== EVENT_RECON_IMG || (CTiPrepView::GetView())->GetISelEvent()== EVENT_AUTOPHASE)
                //                 {
                //                     if(0 == l_csReconProt.Compare(l_csReconProt_Org)) { // i.e same series
                //                         continue;
                //                     } else {    //i.e image is of some other series
                //                         CString l_csErrMsg(_T("Selected Images are not of same series"));
                //                         //CKtReconLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                //                         pICMgr->Release();
                //                         return (E_SELECT_SINGLE_SERIES);
                //                     }
                //                     
                //                 }else{
                //                     if(0 == l_csProtNode.Compare(l_csProtNode_org)) { // i.e same series
                //                         continue;
                //                     } else {    //i.e image is of some other series
                //                         CString l_csErrMsg(_T("Selected Images are not of same series"));
                //                         //CKtReconLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                //                         pICMgr->Release();
                //                         return (E_SELECT_SINGLE_SERIES);
                //                     }
                //                 }
                
            }
        }      
        
        if(0 == nImageNum) {
            CString l_csErrMsg(_T("No image is selected from Image Matrix"));
            //CKtReconLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return(E_SELECT_IMG_ON_ISEL); 
        }        
        
        // fill the func parameter vector of loid
        vector<CString>::iterator l_csit1;
        l_csit1 = l_csImageLoid.begin();
        f_csImgLOID.clear();
        
        for(l_csit1 = l_csImageLoid.begin() ; l_csit1 < l_csImageLoid.end(); l_csit1++) {
            f_csImgLOID.push_back(*l_csit1);
        }
        
    } catch(...) {
        CString l_csErrMsg(_T("Exception in GetLOIDSelectedImagesFromIsel()"));
        //CKtReconLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
        return (E_ERR_EXCEPTION);
    }
    return(E_NO_ERROR);
}
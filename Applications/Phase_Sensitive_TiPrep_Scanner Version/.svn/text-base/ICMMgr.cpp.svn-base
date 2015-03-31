
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#include "stdafx.h"
#include "ICMMgr.h"
#include "TiPrepView.h"
#include "TiPrepTrace.h"
#include "TiPrepLogHelper.h"
#include "TiPrepController.h"
#include "LocalDBMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#include <iostream>
#include "ImageCollectionMgr/CImageCollectionManager.h"
#include "ImageCollectionMgr/CImageLeaf.h"
#include "GLCommon/ICManagerHelper.h"
#include "GLCommon/ScopedICMPtr.h"

using namespace MPlus::DPS::GLCommon;
using namespace std;


/**
* Constructor.
* This function is used for initialization of member variables.
*/
CICMMgr::CICMMgr()
{
    /*TIPREP_TRACE(CICMMgr::CICMMgr);*/
}

/**
* Destructor.
* This function is used for releasing the allocated memory.
*/
CICMMgr::~CICMMgr()
{
    /*TIPREP_TRACE(CICMMgr::~CICMMgr);*/
}

///////////////////////////////////////////////////////////////////////////////
// Member Functions
///////////////////


/*************************************************************
* Method:       GetLOIDSelectedImages
* Description:  This function is used to get the LOID of all
*               the images selected by the user on Iselector.
* Parameter:    const CString & f_csICID
*               Contains all the LOID information of all the
*               images.
* Returns:      int
*               Success status.
*************************************************************/
int CICMMgr::GetLOIDSelectedImages(const CString &f_csICID)
{
    TIPREP_TRACE(CICMMgr::GetLOIDSelectedImages,MRTraceMarker3);
    
    try {
        // first get the current selected images
        ICManagerHelper* pICMgr = ICManagerHelper::GetInstance();
        
        if(NULL == pICMgr) {
            CString l_csErrMsg(_T("NULL Pointer"));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return (E_ERROR);
        }
        
        ScopedImageCollectionPtr pInputIC(
            pICMgr->QueryImageCollection(f_csICID));
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
	if(0 == nImageNum) {
            CString l_csErrMsg(_T("No image is selected from Image Matrix"));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return (I_PROPER_IMG); 
        }
        if(1 == nImageNum  && (CTiPrepView::GetView())->GetISelEvent() <= EVENT_RESELECT) { 
            return (I_FEW_IMG);
        }
        for(l_ni = 0; l_ni < nImageNum; l_ni++) {
            ScopedImageNodePtr pScopeImageNode(pInputIC->GetAt(l_ni));
            
            if(!pScopeImageNode->IsLeaf()) {
                CString l_csErrMsg(_T("Image node not found"));
                CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                pICMgr->Release();
                return (E_ERROR);
            }
            
            pScopeImageNode->GetID(csImageLoid);
            l_csImageLoid.push_back(csImageLoid);
            int l_ndbstat = 0;
            
            if(l_ni == 0) {
//                 l_ndbstat = (CTiPrepView::GetView())->GetLocalDBProtocolNameAtImageLevel
//                     (l_csImageLoid.at(0), l_csProtocolName,l_csReconProt_Org);
                l_ndbstat = l_localdbmgr.GetProtocolNameFromImageLevelDB(l_csImageLoid.at(0), 
                    l_csReconProt_Org,l_csProtocolName,l_csCompletepath);
                
                if(l_ndbstat == E_NO_ERROR) {
                    l_csProtNode_org = l_csProtocolName; 
                } else {
                    CString l_csErrMsg(_T("DB Error:Unable to get Protocol name from DB for selected Image"));
                    CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                    pICMgr->Release();
                    return (E_ERROR);
                }
            } else {
//                l_ndbstat = (CTiPrepView::GetView())->GetLocalDBProtocolNameAtImageLevel(
//                    l_csImageLoid.at(l_ni), l_csProtocolName,l_csReconProt);
                l_ndbstat = l_localdbmgr.GetProtocolNameFromImageLevelDB(l_csImageLoid.at(l_ni), 
                    l_csReconProt,l_csProtocolName,l_csCompletepath);
                
                if(l_ndbstat != E_NO_ERROR) {
                    CString l_csErrMsg(_T("DB Error:Unable to get Protocol name from DB for selected Image"));
                    CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                    pICMgr->Release();
                    return (E_ERROR);
                }
                
                l_csProtNode = l_csProtocolName;
                if ((CTiPrepView::GetView())->GetISelEvent()== EVENT_RECON_IMG || (CTiPrepView::GetView())->GetISelEvent()== EVENT_AUTOPHASE)
                {
                    if(0 == l_csReconProt.Compare(l_csReconProt_Org)) { // i.e same series
                        continue;
                    } else {    //i.e image is of some other series
                        CString l_csErrMsg(_T("Selected Images are not of same series"));
                        CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                        pICMgr->Release();
                        return (E_SELECT_SINGLE_SERIES);
                    }
                    
                }else{
                    if(0 == l_csProtNode.Compare(l_csProtNode_org)) { // i.e same series
                        continue;
                    } else {    //i.e image is of some other series
                        CString l_csErrMsg(_T("Selected Images are not of same series"));
                        CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                        pICMgr->Release();
                        return (E_SELECT_SINGLE_SERIES);
                    }
                }
                
            }
        }
         pICMgr->Release();
        int set_status = 0;
        switch((CTiPrepView::GetView())->GetISelEvent()) {
        case EVENT_AUTOPHASE:
            (CTiPrepController::GetInstance())->SetPCLOIDs(l_csImageLoid, nImageNum); 
            break;
        case EVENT_MG_SELECT:
            (CTiPrepController::GetInstance())->SetPCLOIDs(l_csImageLoid, nImageNum); 
            break;
        case EVENT_RE_SELECT:
            (CTiPrepController::GetInstance())->SetPCLOIDs(l_csImageLoid, nImageNum, REAL); 
            break;
        case EVENT_IMG_SELECT:
            (CTiPrepController::GetInstance())->SetPCLOIDs(l_csImageLoid, nImageNum, IMGN); 
            break;
        case EVENT_POLARITY_CORRECTION:
            set_status = (CTiPrepController::GetInstance())->SetImageLOIDForPolarityCorrection(nImageNum, l_csImageLoid);
            break;
        case EVENT_ADD_OFFSET:
            set_status = (CTiPrepController::GetInstance())->SetImageLOIDForAddOffset(nImageNum, l_csImageLoid);//vilas
            break;
        case EVENT_DATA_TYPE:
            set_status = (CTiPrepView::GetView())->getDataForm(l_csImageLoid, nImageNum);
            break;
        case EVENT_RECON_IMG:
            set_status = (CTiPrepController::GetInstance())->getReconstuctionImgs(l_csImageLoid/*, nImageNum*/);
            break;
        case EVENT_RESELECT:
        case EVENT_INIT:
        default:
            (CTiPrepView::GetView())->SetImageLOID(nImageNum, l_csImageLoid);
                
            break;
        }
        /*if(0 == nImageNum) {
            CString l_csErrMsg(_T("No image is selected from Image Matrix"));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return (I_PROPER_IMG); 
        }
        if(1 == nImageNum  && (CTiPrepView::GetView())->GetISelEvent() <= EVENT_RESELECT) { 
            return (I_FEW_IMG);
        }*/
        
        int l_nEvent = EVENT_RECON_IMG;
        if(l_nEvent == (CTiPrepView::GetView())->GetISelEvent()){
            if (E_NO_ERROR != set_status){
                return set_status;
            }
        }
        
        return (set_status);
    } catch(...) {
        CString l_csErrMsg(_T("Exception in GetLOIDSelectedImages()"));
        CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
        return (E_ERR_EXCEPTION);
    }
    return(E_NO_ERROR);
}

///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////


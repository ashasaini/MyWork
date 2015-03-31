/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  Module: TiPrep
  Author: IGATE-Advanced Cardiac Team
*******************************************************************************/

#include "stdafx.h"
#include "ROIHandler.h"
#include "TiPrepTrace.h"
#include "TiPrepLogHelper.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace std;
#include "MamROIManager_i.c"
#include "MamROIManager.h"
#include "CosVis\MPlusCosVisRotHelper.h"
#include "CosVis\custom\Component\CosVis_i.c"

/*#include "shared/model/IcfVectorModel.h"
#include "shared/tools/IcfModelPtr.h"
#include "shared/study/icfimagemodel.h"
#include "IIcfImageModel.h"
#include "shared/study/IcfStudyComponent.h"
#include "shared/study/icfimageparameters.h"
#include "C:/SharedMPlus/MR/Sources/CApp/Overlay/src/ROIOverlay/model/MamRoiStrokeModel.h"*/
//#include "shared/persistence/IcfPersistentMemoryArchive.h"
#include "TiPrepView.h"

CROIHandler* CROIHandler::m_roi_ptr = NULL;
const CString EXCEPTION_ROIHANDLER = L"Exception occurred in ROI handler";

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//****************************Method Header***************************
//Method Name   : CROIHandler
//Purpose       : Constructor
//********************************************************************
/**
*   A constructor.
*   This function is used for initialization of class variables.
*/
CROIHandler::CROIHandler()
{
    /*TIPREP_TRACE(CROIHandler::CROIHandler);*/
    m_csOperationPath = _T("1_Screen/1_Frame/Main");
    m_Roi_count = 0;
	m_protocol_name = _T("");  
}
//****************************Method Header***************************
//Method Name   : CROIHandler
//Purpose       : Destructor
//********************************************************************
/**
*   A destructor.
*   This function is used for deleting the memory if used.
*/
CROIHandler::~CROIHandler()
{
   /* TIPREP_TRACE(CROIHandler::~CROIHandler);*/
    m_Roi_count = 0;
    
    if(m_uid_point_list.GetSize() > 0) {
        m_uid_point_list.RemoveAll();
    }
    
    m_vEllipseRoiparam.clear();
    m_vRoiparam.clear();
}

/*************************************************************
* Method:       GetInstance
* Description:  This function creates CROIHandler instance if
*               it is not created before.
* Returns:      CROIHandler*
*************************************************************/
CROIHandler* CROIHandler::GetInstance()
{
    /*TIPREP_TRACE(CROIHandler::GetInstance);*/
    
    if(NULL == m_roi_ptr) {
        m_roi_ptr = new CROIHandler();
    }
    
    return m_roi_ptr;
}

/*************************************************************
* Method:       DeleteInstance
* Description:  This function destructs the singleton pointer
*               of type CROIHandler.
* Returns:      void
*************************************************************/
void CROIHandler::DeleteInstance()
{
    /*TIPREP_TRACE(CROIHandler::DeleteInstance);*/
    
    if(NULL != m_roi_ptr) {
        delete m_roi_ptr;
        m_roi_ptr = NULL;
    }
}

/*************************************************************
* Method:       InitializeObserver
* Description:  This function initializes observer for communication.
* Parameter:    CString f_channel_name
*               f_channel_name  Contains GL name for communication.
* Returns:      HRESULT
*               S_OK on success.
*************************************************************/
HRESULT CROIHandler::InitializeObserver(CString f_channel_name)
{
    TIPREP_TRACE(CROIHandler::InitializeObserver,MRTraceMarker3);
    
    try {
        HRESULT hr;
        CStringArray aFrameLevelModelList;
        aFrameLevelModelList.Add(_T("Roi Stroke Model"));
        int nFrameLevelModelCount = aFrameLevelModelList.GetSize();
        BSTR* bsFrameLevelModelList = new BSTR[nFrameLevelModelCount];
        
        if(NULL == bsFrameLevelModelList) {
            CString l_csErrMsg(_T("NULL pointer"));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return E_FAIL;
        }
        
        for(int index = 0; index < nFrameLevelModelCount; index++) {
            *(bsFrameLevelModelList + index) =
                (aFrameLevelModelList.GetAt(index)).AllocSysString();
        }
        
        m_csGLname = f_channel_name;
        short nSendMessageType = 1;
        _bstr_t bstr_GL_full_name = _bstr_t(static_cast<LPCTSTR>(m_csGLname));
        short nTimeOut = 1000;
        BSTR* pScreenNameList = NULL;
        short nScreenCount = 0;
        _bstr_t operation_path = _T("");
        
        if(FAILED(m_ppCosVisObject->GetScreenPageNameList(
            &nScreenCount, &pScreenNameList))) {
            CString l_csErrMsg(_T("GetScreenPageNameList failed"));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            hr = E_FAIL;
        }
        
        if(NULL == pScreenNameList) {
            CString l_csErrMsg(_T("GetScreenPageNameList returned NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            hr = E_FAIL;
        }
        
        int nPathCount = 0;
        BSTR* pSpecifiedPathList = NULL;
        
        for(int nCount = 0; nCount < nScreenCount; nCount++) {
            CString csScreenName = _T("");
            csScreenName = pScreenNameList[nCount];
            
            if(FAILED(GetSpecifiedScreenAllPath(
                _bstr_t(csScreenName),
                &nPathCount, &pSpecifiedPathList))) {
                if(NULL == pSpecifiedPathList) {
                    CString l_csErrMsg(_T("GetSpecifiedScreenAllPath returned NULL"));
                    CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                    hr = E_FAIL;
                }
                
                for(int i = 0; i < nPathCount; i++) {
                    ::SysFreeString(*(pSpecifiedPathList + i));
                    *(pSpecifiedPathList + i) = NULL;
                }
                
                delete []pSpecifiedPathList;
                pSpecifiedPathList = NULL;
                
                for(i = 0; i < nScreenCount; i++) {
                    ::SysFreeString(*(pScreenNameList + i));
                    *(pScreenNameList + i) = NULL;
                }
                
                if(NULL != pScreenNameList) {
                    delete [] pScreenNameList;
                    pScreenNameList = NULL;
                }
                
                for(i = 0; i < nFrameLevelModelCount; i++) {
                    ::SysFreeString(*(bsFrameLevelModelList + i));
                    *(bsFrameLevelModelList + i) = NULL;
                }
                
                if(NULL  != bsFrameLevelModelList) {
                    delete []bsFrameLevelModelList;
                    bsFrameLevelModelList = NULL;
                }
                
                CString l_csErrMsg(_T("GetSpecifiedScreenAllPath failed"));
                CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                return E_FAIL;
            }
            
            for(int i = 0; i < nPathCount; i++) {
                m_opPathList.Add(*(pSpecifiedPathList + i));
                operation_path = *(pSpecifiedPathList + i);
                hr = m_ppCosVisObject->RegisterObserver(bstr_GL_full_name,
                    operation_path,
                    static_cast<short>(nFrameLevelModelCount),
                    bsFrameLevelModelList,
                    nSendMessageType,
                    static_cast<short>(nTimeOut));
            }
            
            for(i = 0; i < nPathCount; i++) {
                ::SysFreeString(*(pSpecifiedPathList + i));
                *(pSpecifiedPathList + i) = NULL;
            }
            
            delete []pSpecifiedPathList;
            pSpecifiedPathList = NULL;
        }
        
        if(FAILED(hr)) {
            CString l_csErrMsg(_T("Fail to Initialize observer."));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            
            for(int i = 0; i < nPathCount; i++) {
                ::SysFreeString(*(pSpecifiedPathList + i));
                *(pSpecifiedPathList + i) = NULL;
            }
            
            delete []pSpecifiedPathList;
            pSpecifiedPathList = NULL;
            
            for(i = 0; i < nScreenCount; i++) {
                ::SysFreeString(*(pScreenNameList + i));
                *(pScreenNameList + i) = NULL;
            }
            
            if(NULL != pScreenNameList) {
                delete [] pScreenNameList;
                pScreenNameList = NULL;
            }
            
            for(i = 0; i < nFrameLevelModelCount; i++) {
                ::SysFreeString(*(bsFrameLevelModelList + i));
                *(bsFrameLevelModelList + i) = NULL;
            }
            
            if(NULL  != bsFrameLevelModelList) {
                delete []bsFrameLevelModelList;
                bsFrameLevelModelList = NULL;
            }
            
            return E_FAIL;
        }
        
        for(int i = 0; i < nScreenCount; i++) {
            ::SysFreeString(*(pScreenNameList + i));
            *(pScreenNameList + i) = NULL;
        }
        
        if(NULL  != pScreenNameList) {
            delete [] pScreenNameList;
            pScreenNameList = NULL;
        }
        
        for(i = 0; i < nFrameLevelModelCount; i++) {
            ::SysFreeString(*(bsFrameLevelModelList + i));
            *(bsFrameLevelModelList + i) = NULL;
        }
        
        if(NULL  != bsFrameLevelModelList) {
            delete [] bsFrameLevelModelList;
            bsFrameLevelModelList = NULL;
        }
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_ROIHANDLER, __LINE__, __FILE__);
        return E_FAIL;
    }
    
    return S_OK;
}

/*************************************************************
* Method:       InitializeCosVisChannel
* Description:  This function is used to initialize CosVischannel
*               for communication.
* Parameter:    CString f_parentname
*               Contains parent name with communication
*               channel is establish.
* Returns:      BOOL
*************************************************************/
BOOL CROIHandler::InitializeCosVisChannel(CString f_parentname)
{
    TIPREP_TRACE(CROIHandler::InitializeCosVisChannel,MRTraceMarker3);
    
    try {
        m_ppCosVisObject = NULL;
        m_ppMamROIControlHandlerManager = NULL;
        //  initialize the CosVIs interface
        CComPtr<IUnknown> pUnk;
        CString csCosVisName;
        csCosVisName = f_parentname + _T(".CosVis");
        
        if(FAILED(CRotHelper::GetObject(&pUnk, LPCTSTR(csCosVisName)))) {
            CString csResult(_T("Fail to Initialize CosVis Interface."));
            CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
            return FALSE;
        }
        
        if(FAILED(pUnk->QueryInterface(IID_ICosVis, (void **)&m_ppCosVisObject))) {
            CString csResult(_T("Fail to Initialize CosVis Interface."));
            CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
            return FALSE;
        }
        
        //  initialize the CosVis interface
        CComPtr<IUnknown> pUk;
        CString csManagerFullname;
        csManagerFullname = f_parentname + _T(".CosVis.MamROIManager.MamROIControlHandlerManager");
        
        if(FAILED(CRotHelper::GetObject(&pUk, (LPCTSTR)csManagerFullname))) {
            CString csResult(_T("Fail to Initialize ROI Overlay Manager Interface."));
            CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
            return FALSE;
        }
        
        if(FAILED(pUk->QueryInterface(IID_IMamROIControlHandlerManager, (void**) &m_ppMamROIControlHandlerManager))) {
            CString csResult(_T("Fail to Initialize ROI Overlay Manager Interface."));
            CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
            return FALSE;
        }
    } catch(...) {
        CString csResult(_T("ROI Exception occurred in InitializeCosVisChannel()"));
        CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
        return FALSE;
    }
    
    return TRUE;
}

/*************************************************************
* Method:       GetSpecifiedScreenAllPath
* Description:  This function used to get valid path for communication.
* Parameter:    BSTR bsScreenName
*               Contains screen name on which ROI is drawn.
* Parameter:    int * pPathCount
*               Out parameter total no paths.
* Parameter:    BSTR * * ppSpecifiedPathList
*               Out parameter list of all valid Path.
* Returns:      HRESULT
*               S_OK on success.
*************************************************************/
HRESULT CROIHandler::GetSpecifiedScreenAllPath(BSTR bsScreenName,
                                               int* pPathCount,
                                               BSTR** ppSpecifiedPathList)
{
    TIPREP_TRACE(CROIHandler::GetSpecifiedScreenAllPath,MRTraceMarker3);
    
    try {
        // 1.check validity of interface pointers
        if(m_ppMamROIControlHandlerManager == NULL) {
            CString l_csErrMsg(_T("NULL pointer"));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return E_FAIL;
        }
        
        if(FAILED(m_ppMamROIControlHandlerManager->GetAllValidPath(
            bsScreenName, pPathCount, ppSpecifiedPathList))) {
            CString l_csErrMsg(_T("GetAllValidPath failed"));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return E_FAIL;
        }
        
        return S_OK;
    } catch(...) {
        CString csResult(_T("ROI Exception occurred in GetSpecifiedScreenAllPath()"));
        CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
        return E_FAIL;
    }
}

/*************************************************************
* Method:       DrawROIComplete
* Description:  This function is used to draw ROI on specified path.
* Parameter:    CString f_csSender
*               Name of request sender.
* Parameter:    CString f_csCommand
*               Used get command.
* Parameter:    CString f_csParam
*               Parameter to draw ROI.
* Parameter:    _bstr_t f_custom
*               Used to Get the Stream.
* Returns:      BOOL
*************************************************************/
BOOL CROIHandler::DrawROIComplete(CString f_csSender, CString f_csCommand,
                                  CString f_csParam, _bstr_t f_custom)
{
    TIPREP_TRACE(CROIHandler::DrawROIComplete,MRTraceMarker3);
#if 0
    try {
        CString csObservedModel = f_csParam;
        int nPos = csObservedModel.ReverseFind(_T(':'));
        
        if((nPos < 0) || (nPos >= csObservedModel.GetLength())) {
            CString l_csErrMsg(_T("ObservedModel not found"));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return FALSE;
        }
        
        CString csObservedModelPath = csObservedModel.Left(nPos);
        IcfModelPtr<IMamRoiStrokeModel> InputModelProxy;
        IcfImplPtr<MamRoiStrokeModel> InputModel;
        InputModelProxy.CreateInstance(InputModel.CreateInstance());
        
        if(InputModelProxy == NULL) {
            CString l_csErrMsg(_T("NULL pointer"));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return FALSE;
        }
        
        // Get the model's IIcfPersistentObject interface
        CComPtr<IMamRoiStrokeModel> i_model;
        i_model = InputModelProxy.Model();
        CComQIPtr<IIcfPersistentObject> i_persistent(i_model);
        _bstr_t *pstrProp = &f_custom;
        int len = pstrProp->length() * sizeof(WCHAR);
        WCHAR *buffer1 = static_cast<WCHAR*>(*pstrProp);
        std::string astr((const char*)buffer1, len);
        std::stringstream astream(astr, std::ios_base::in);
        IcfImplPtr<IcfPersistentMemoryArchive> archive;
        archive.CreateInstance();
        
        if(archive == NULL) {
            CString l_csErrMsg(_T("NULL Pointer"));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return FALSE;
        }
        
        if(FAILED(archive->Initialise())) {
            CString l_csErrMsg(_T("Initialise failed"));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return FALSE;
        }
        
        // Get the Stream
        archive->ReadFromStream(astream);
        CComQIPtr<IIcfPersistentInputStream> input(archive);
        archive->OpenObject(_T("temp object"), _T("temp object type"));
        i_persistent->Read(input);
        archive->CloseInputObject(NULL);
        BOOL isStroke = FALSE;
        
        if(FAILED(i_model->GetIsStroke(&isStroke))) {
            CString l_csErrMsg(_T("GetIsStroke failed"));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return FALSE;
        }
        
        // Get the Unique ID
        DWORD dwUID = 0;
        
        if(FAILED(i_model->GetUniqueID(&dwUID))) {
            CString l_csErrMsg(_T("GetUniqueID failed"));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return FALSE;
        }
        
        CString csTemp = _T("");
        csTemp.Format(_T("%u"), dwUID);
        _bstr_t bstrUID = csTemp;
        _bstr_t bstrPath = csObservedModelPath;
        
        // Draw complete
        if(FALSE == isStroke) {
            CString l_csErrMsg(_T("isStroke is false"));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            int l_nStatus = SaveDrawROIPosition(bstrUID, bstrPath);
            
            if(E_NO_ERROR != l_nStatus) {
                l_csErrMsg.Format(_T("Faild to save ROI positons."));
                CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            }
            
        }
    } catch(...) {
        CString csResult(_T("ROI Exception occurred in DrawROIComplete()"));
        CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
        return FALSE;
    }
#endif
    return TRUE;
}

/*************************************************************
* Method:       SaveDrawROIPosition
* Description:  This function is used to save ROI central point
*               position by created UID.
* Parameter:    BSTR roi_uid
*               ROI object unique ID
* Parameter:    BSTR path
*               operation path
* Returns:      void
*************************************************************/
//[7/9/2013 Shweta]start return type modified.
int CROIHandler::SaveDrawROIPosition(BSTR roi_uid, BSTR path)
{
    TIPREP_TRACE(CROIHandler::SaveDrawROIPosition,MRTraceMarker3);
    HRESULT hr;
    
    try {
        CComBSTR bsOperationPath;
        bsOperationPath = path;
        CurvePoint_t* pVectorList = NULL;
        int nVectorCount = 0;
        ROIObject_t roiObject;
        hr = m_ppMamROIControlHandlerManager->GetMeasurementObjectInImageCoordination(bsOperationPath,
            roi_uid,
            &roiObject,
            &nVectorCount,
            &pVectorList);
        
        if(FAILED(hr)) {
            CString l_csErrMsg(_T("GetMeasurementObjectInImageCoordination failed"));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return E_ERROR;
        }
        
        if((0 == nVectorCount) || (NULL == pVectorList)) {
            CString l_csErrMsg(_T("NULL Pointer or vector count is zero."));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return E_ERROR;
        }
        
        float x0 = static_cast<float>((pVectorList[0].x + pVectorList[1].x) / 2.0);
        float y0 = static_cast<float>((pVectorList[0].y + pVectorList[1].y) / 2.0);
        CString uid(roi_uid);
        Uid_Point upt;
        upt.uid = uid;
        upt.x = x0;
        upt.y = y0;
        m_uid_point_list.Add(upt);
        ReleaseVectorList(pVectorList);
    } catch(...) {
        CString csResult(_T("ROI Exception occurred in SaveDrawROIPosition()"));
        CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
        return E_ERR_EXCEPTION;
    }
    
    return E_NO_ERROR;
}

/*************************************************************
* Method:       ReleaseVectorList
* Description:  This function is used to free the memory
*               allocated CurvePoint_t pointer.
* Parameter:    CurvePoint_t * pVectorList
*               Contain vector list.
* Returns:      void
*************************************************************/
void CROIHandler::ReleaseVectorList(CurvePoint_t* pVectorList)
{
    TIPREP_TRACE(CROIHandler::ReleaseVectorList,MRTraceMarker3);
    
    try {
        ::CoTaskMemFree(pVectorList);
        pVectorList = NULL;
    } catch(...) {
        CString csResult(_T("ROI Exception occurred in ReleaseVectorList"));
        CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
        return;
    }
}

/*************************************************************
* Method:       AutoSaveDB
* Description:  This function is used
* Parameter:    BOOL f_flag
*               Used to check the status.
* Returns:      BOOL
*************************************************************/
BOOL CROIHandler::AutoSaveDB(BOOL f_flag)
{
    TIPREP_TRACE(CROIHandler::AutoSaveDB,MRTraceMarker9);
    
    try {
        int status = 0;
        CComBSTR option;
        
        if(NULL == m_ppCosVisObject) {
            CString l_csErrMsg(_T("NULL Pointer"));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return FALSE;
        }
        
        CComBSTR bsOperationPath;
        bsOperationPath = _T("1_Screen");
        OutputDebugString(m_csOperationPath);
        
        if(FAILED(m_ppCosVisObject->SetAutoSaveDB(bsOperationPath, f_flag))) {
            CString l_csErrMsg(_T("SetAutoSaveDB failed"));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            status = E_ERROR;
        }
        
        if(f_flag) {
            option = _T("*");
        } else {
            option = _T("LUT/Scantime/Demographic/TRANSFORMATION");
        }
        
        OutputDebugString(option);
        BSTR tmppath = NULL;
        BSTR tmpoption = NULL;
        tmppath = ::SysAllocString(_T("1_Screen")); 
        tmpoption = ::SysAllocString(option); 
        _bstr_t bsOpPath = tmppath;
        _bstr_t bsOption =  tmpoption;
        
        if(FAILED(m_ppCosVisObject->SetLoadInfoName(bsOpPath, bsOption))) {
            CString l_csErrMsg(_T("SetLoadInfoName failed"));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            status = E_ERROR;
        } else {
        }
        ::SysFreeString(tmppath);        
        ::SysFreeString(tmpoption);
        if(status) {
            return FALSE;
        } else {
            return TRUE;
        }
    } catch(...) {
        CString csResult(_T("ROI Exception occurred in AutoSaveDB()"));
        CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
        return FALSE;
    }
}

/*************************************************************
* Method:       DrawROI
* Description:  This function used to draw ROI on image.
* Parameter:    int f_nRoiNumber
*               Number of ROIs to be drawn.
* Parameter:    BOOL f_show
* Returns:      BOOL
*************************************************************/
BOOL CROIHandler::DrawROI(int f_nRoiNumber, BOOL f_show)
{
    TIPREP_TRACE(CROIHandler::DrawROI,MRTraceMarker3);
    
    try {
        if(NULL == m_ppMamROIControlHandlerManager || NULL == m_ppCosVisObject) {
            CString l_csErrMsg(_T("NULL Pointer"));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return FALSE;
        }
        
        if(0 == f_nRoiNumber) {
            m_uid_point_list.RemoveAll();
            m_Roi_count = 0;
        }
        
        CComBSTR bsOperationPath;
        bsOperationPath = CComBSTR(static_cast<LPCTSTR>(m_csOperationPath));

        //Trace log
#ifdef _UT_LOG_

        if(f_nRoiNumber == 0){
            CString l_trace_line (_T(""));
            l_trace_line.Format(_T("\n---------------------------ROI DRAWING PROC START-------------------------\n"));
            CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker5, l_trace_line,
                _T("CROIHandler::DrawROI"));
        }

        if(f_nRoiNumber == 0){
            CString l_trace_line (_T(""));
            l_trace_line.Format(_T("\nBSOperation path= %s\n"),m_csOperationPath);
            CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker5, l_trace_line,
                _T("CROIHandler::DrawROI"));
        }
#endif
        //Trace log

        int vectorListNum = 2;
        CComBSTR bsUid;
        ROIObject_t stROIObject;
		//Trace log
#ifdef _UT_LOG_
        if(f_nRoiNumber == 0){
            CString l_trace_line (_T(""));
            l_trace_line.Format(_T("\n-------------------CreateROIObject\n"));
            CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker5, l_trace_line,
                _T("CROIHandler::DrawROI"));
        }
#endif
		//Trace log
        CreateROIObject(&stROIObject, f_nRoiNumber);
#ifdef _UT_LOG_
        //Trace log
        if(f_nRoiNumber == 0){
            CString l_trace_line (_T(""));

           l_trace_line.Format(_T("ROIObject RoiNo:%d\n")
                                _T("No of point =%d\n")
                                _T("isselect = %d\n")
                                _T("angle = %lf\n")
                                _T("handleType =%u\n")
                                _T("ROI type = %d\n")
                                _T("max_point = %d\n")
                                _T("visible = %d\n")
                                _T("selectable = %d\n")
                                _T("color_name = %d\n")
                                _T("text_size[0] = %d\n")
                                _T("text_size[1] = %d\n")
                                _T("lineType = %d\n")
                                _T("lineWidth = %d\n")
                                _T("font_size = %d\n")
                                _T("displayId = %d\n")
                                _T("callout_fixed = %d\n")
                                _T("callout_position[0] = %f\n")
                                _T("callout_position[1] = %f\n")
                                _T("text = %s\n")
                                _T("arm1Length = %lf\n")
                                _T("arm2Length = %lf\n")
                                _T("markerType = %d\n")
                                _T("owner = %s\n")
                                _T("arrow type = %d\n")
                                _T("opStatus = %d\n")
                                _T("InformationVisible = %d\n")
                                _T("text_fontType = %s\n")
                                ,
                                f_nRoiNumber,
                                stROIObject.vectorListCount,
                                stROIObject.isselect ,
                                stROIObject.angle ,
                                stROIObject.handleType ,
                                stROIObject.type,
                                stROIObject.max_point ,
                                stROIObject.visible ,
                                stROIObject.selectable ,
                                stROIObject.color_name ,
                                stROIObject.text_size[0] ,
                                stROIObject.text_size[1] ,
                                stROIObject.lineType ,
                                stROIObject.lineWidth ,
                                stROIObject.font_size ,
                                stROIObject.displayId ,
                                stROIObject.callout_fixed ,
                                stROIObject.callout_position[0] ,
                                stROIObject.callout_position[1] ,
                                stROIObject.text ,
                                stROIObject.arm1Length ,
                                stROIObject.arm2Length ,
                                stROIObject.markerType ,
                                stROIObject.owner ,
                                stROIObject.arrow ,
                                stROIObject.opStatus ,
                                stROIObject.InformationVisible,
                                stROIObject.text_fontType
                                );

                                
            CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker5, l_trace_line,
                _T("CROIHandler::DrawROI"));
        }
        //Trace log
#endif


        memcpy(&(m_RoiObject[f_nRoiNumber]),&stROIObject,sizeof(ROIObject_t));

        CurvePoint_t* vectorList = NULL;
        float x1 = 0, y1 = 0, x2 = 0, y2 = 0, l_radius = 0;
        float l_fHeight = 0, l_fWidth = 0.0; 
        int l_nStatus = CalEllipseCordination(x1, y1, x2, y2, l_fHeight, l_fWidth, f_nRoiNumber); 
        
        if(E_NO_ERROR != l_nStatus) {
            CString l_csErrMsg(_T("Fail to calculate ellipse parameters"));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return FALSE;
        }
        
        vectorList = new CurvePoint_t[2];
        
        if(NULL == vectorList) {
            CString l_csErrMsg(_T("Vector list is NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return FALSE;
        }
        
        vectorList[0].x = x1;
        vectorList[0].y = y1;
        vectorList[0].z = 0.0f;
        vectorList[1].x = x2;
        vectorList[1].y = y2;
        vectorList[1].z = 0.0f;        
#ifdef _UT_LOG_

        if(f_nRoiNumber == 0){
            CString l_trace_line (_T(""));
            l_trace_line.Format(_T("\nCurve pts Input to function AddROIInImageCoordination-->\nROI[%d]x0 = %f,y0 = %f,z0 = %f,x1 = %f,y1 = %f,z1 = %f\n"),
                                    f_nRoiNumber,vectorList[0].x,vectorList[0].y,vectorList[0].z,vectorList[1].x,vectorList[1].y,vectorList[1].z);
            CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker5, l_trace_line,
                _T("CROIHandler::DrawROI"));
        }

        if(f_nRoiNumber == 0){
            CString l_trace_line (_T(""));
            l_trace_line.Format(_T("\n-------------------AddROIInImageCoordination\n"));
            CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker5, l_trace_line,
                _T("CROIHandler::DrawROI"));
        }
#endif
        if(FAILED(m_ppMamROIControlHandlerManager->AddROIInImageCoordination(bsOperationPath, stROIObject,
            &bsUid, vectorListNum, vectorList))) {
            CString csResult(_T("Fail to Add ROI."));
            CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
            
            if(NULL != vectorList) {
                delete []vectorList;
                vectorList = NULL;
            }
            
            return FALSE;
        }
		//TRACE COMMENTED
#ifdef _UT_LOG_
        if(f_nRoiNumber == 0){
            CString l_trace_line (_T(""));
            l_trace_line.Format(_T("\nRESET ---->pUID = %s\n"),BSTR(bsUid));
            CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker5, l_trace_line,
                _T("CROIHandler::DrawROI"));
        }
#endif
        //------------------------------------------------------To get The curve point drawn
        if(vectorList){
            memset(vectorList , 0x0 ,2*sizeof(CurvePoint_t));
         //TRACE COMMENTED
#ifdef _UT_LOG_
            if(f_nRoiNumber == 0){
                CString l_trace_line (_T(""));
                //l_trace_line.Format(_T("\n-------------------GetCurvePointInImage\n"));
				l_trace_line.Format(_T("\n-------------------GetCurvePoint\n"));
                CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker5, l_trace_line,
                    _T("CROIHandler::DrawROI"));
            }
#endif

            // m_ppMamROIControlHandlerManager->GetCurvePointInImage(bsOperationPath,bsUid,vectorListNum,vectorList);        Sachin 09-Aug-13

			m_ppMamROIControlHandlerManager->GetCurvePoint(bsOperationPath,bsUid,vectorListNum,vectorList);
            m_curveptinfo[f_nRoiNumber].CurvePts[0].x = vectorList[0].x;
            m_curveptinfo[f_nRoiNumber].CurvePts[0].y = vectorList[0].y;
            m_curveptinfo[f_nRoiNumber].CurvePts[0].z = vectorList[0].z;
            m_curveptinfo[f_nRoiNumber].CurvePts[1].x = vectorList[1].x;
            m_curveptinfo[f_nRoiNumber].CurvePts[1].y = vectorList[1].y;
            m_curveptinfo[f_nRoiNumber].CurvePts[1].z = vectorList[1].z;
			//TRACE COMMENTED
#ifdef _UT_LOG_
            if(f_nRoiNumber == 0){
                CString l_trace_line (_T(""));
                //l_trace_line.Format(_T("\nGetCurvePointInImage output-->\nROI[%d] :x0 = %f,y0 = %f,x1 = %f,y1 = %f\n"),f_nRoiNumber,
				  l_trace_line.Format(_T("\nGetCurvePoint output-->\nROI[%d] :x0 = %f,y0 = %f,z0 = %f,x1 = %f,y1 = %f,z1 = %f\n"),f_nRoiNumber,
                                    vectorList[0].x,vectorList[0].y,vectorList[0].z,vectorList[1].x,vectorList[1].y,vectorList[1].z);
                CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker5, l_trace_line,
                _T("CROIHandler::DrawROI"));
            }
#endif
        }
        //----------------------------------------------------------------------------------------------------------
        CString csResult;
        csResult.Format(_T("Succeed to Add ROI: %s"), bsUid);
        //------------HIDE THE TWO ROIS AT THE TIME OF CREATION-------------//
        HRESULT hr;
        
        if(/*(f_nRoiNumber > 0 &&*/ (TRUE != f_show)/*)  &&
            (!(CTiPrepView::GetView()->IsT1MapFlagSet() && f_nRoiNumber == 1))*/) { 
            hr =  m_ppMamROIControlHandlerManager->SetVisible(bsOperationPath,
                bsUid, FALSE);
            hr = m_ppMamROIControlHandlerManager->SetIsSelected(bsOperationPath,
                bsUid, FALSE);
        }
        
        if(FAILED(hr)) { 
            CString l_cserrmsg(_T("DrawROI() Failed to Set visible FALSE"));
            CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
        }
		//TRACE COMMENTED
#ifdef _UT_LOG_
        if(f_nRoiNumber == 0){
            CString l_trace_line (_T(""));
            l_trace_line.Format(_T("\n---------------------------ROI DRAWING PROC END-------------------------\n"));
            CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker5, l_trace_line,
                _T("CROIHandler::DrawROI"));
        }
#endif

        m_Roi_count++;
        float x0 = static_cast<float>((x1 + x2) * 0.5);
        float y0 = static_cast<float>((y1 + y2) * 0.5);
        CString uid(bsUid);
        Uid_Point upt;
        upt.uid = uid;
        upt.x = x0;
        upt.y = y0;
        upt.radius = /*l_radius*/l_fHeight; //radius is Height for ellipse
        upt.width = l_fWidth;
        
        if(f_nRoiNumber < m_uid_point_list.GetSize()) {
            m_uid_point_list.RemoveAt(f_nRoiNumber); 
        }
        
        m_uid_point_list.InsertAt(f_nRoiNumber, upt); 
        ::SysFreeString(stROIObject.text_fontType);
        stROIObject.text_fontType = NULL;
        ::SysFreeString(stROIObject.text);
        stROIObject.text = NULL;
        ::SysFreeString(stROIObject.owner);
        stROIObject.owner = NULL;
        
        if(NULL != vectorList) {
            delete []vectorList;
            vectorList = NULL;
        }
        
        if(FAILED(m_ppCosVisObject->ForceUpdate())) {
            CString csResult(_T("Fail to Update CosVis."));
            CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
            return FALSE;
        }
        
        return TRUE;
    } catch(...) {
        CString csResult(_T("ROI Exception occurred in DrawROI()"));
        CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
        return FALSE;
    }
}

/*************************************************************
* Method:       CreateROIObject
* Description:  This function is used to create ROI object.
* Parameter:    ROIObject_t * stROIObject
*               ROI object.
* Parameter:    int f_nRoiNumber
*               ROI number to be displayed when ROI is drawn.
* Returns:      HRESULT
*************************************************************/
HRESULT CROIHandler::CreateROIObject(ROIObject_t* stROIObject,
                                     int f_nRoiNumber)
{
    TIPREP_TRACE(CROIHandler::CreateROIObject,MRTraceMarker3);
    stROIObject->vectorList = NULL;
    stROIObject->vectorListCount = 2;
    stROIObject->isselect = TRUE;
    stROIObject->angle = 0.0f;
    stROIObject->handleType = 983040;
    stROIObject->type = e_mam_measurement_ellipse;
    
    for(int index = 0 ; index < 12 ; index++) {
        stROIObject->patient_matrix[index] = 0;
        stROIObject->image_matrix[index] = 0;
    }
    
    stROIObject->max_point = 16384;
    stROIObject->visible = TRUE;
    stROIObject->selectable = TRUE;
    stROIObject->color_name = 0;
    stROIObject->text_size[0] = 0;
    stROIObject->text_size[1] = 0;
    stROIObject->lineType = 0;
    stROIObject->lineWidth = 1;
    stROIObject->font_size = 15;
    stROIObject->displayId = (long)f_nRoiNumber;
    stROIObject->callout_fixed = FALSE;
    stROIObject->callout_position[0] = -1;
    stROIObject->callout_position[1] = -1;
    stROIObject->text = ::SysAllocString(_T(""));
    stROIObject->arm1Length = 1.00f;
    stROIObject->arm2Length = 1.00f;
    stROIObject->markerType = 0;
    stROIObject->owner = ::SysAllocString(_T("ROI"));
    stROIObject->arrow = ArrowType_None;
    stROIObject->opStatus = OpStatus_move;
    stROIObject->InformationVisible = FALSE;
    stROIObject->text_fontType = ::SysAllocString(_T("Times New Roman"));
	// +ROI Chnages for 2.20 V2_20_ROI_DEFIX
#ifdef V2_20_ROI_DEFIX
	CEllipticalROIParams l_crRoiparam;
if(m_vEllipseRoiparam.empty() == false) {
     l_crRoiparam = m_vEllipseRoiparam.at(f_nRoiNumber);
    stROIObject->roi_center[0] = (l_crRoiparam.GetCenter_X());
    stROIObject->roi_center[1] = (l_crRoiparam.GetCenter_Y());
} else {
	stROIObject->roi_center[0] = 10;
    stROIObject->roi_center[1] = 10;
}
    stROIObject->roi_center[2] = 0;
    stROIObject->sAnnotationFontType = ::SysAllocString( _T("Times New Roman") );;
    stROIObject->lAnnotationFontSize = 12;
    stROIObject->lAnnotationFontColor = 0;
    stROIObject->lAnnotationFontWeight = FW_NORMAL;
    stROIObject->lAnnotationFontWidth = 0;
    stROIObject->byAnnotationFontCharSet = DEFAULT_CHARSET;
    stROIObject->byAnnotationFontQuality = DEFAULT_QUALITY;
    stROIObject->byAnnotationFontPitch = DEFAULT_PITCH | FF_SWISS;
    stROIObject->byAnnotationFontItalic = FALSE;
    stROIObject->byAnnotationFontUnderline = FALSE;
    stROIObject->byAnnotationFontStrikeout = FALSE;
		//- ROI Chnages for 2.20
#endif
    return S_OK;
}

/*************************************************************
* Method:       CalCircleCordination
* Description:  This function calculates center and radius for ROI.
* Parameter:    float & x1
*               Out parameter used is to hold X co-ordinate for ROI drawing.
* Parameter:    float & y1
*               Out parameter used is to hold X co-ordinate for ROI drawing.
* Parameter:    float & x2
*               Out parameter used is to hold X co-ordinate for ROI drawing.
* Parameter:    float & y2
*               Out parameter used is to hold X co-ordinate for ROI drawing.
* Parameter:    float & f_fradius
*               Contains radius for ROI drawing.
* Parameter:    int f_nroi_no
* Returns:      void
*************************************************************/
int CROIHandler::CalCircleCordination(float &x1,
                                      float &y1,
                                      float &x2,
                                      float &y2,
                                      float &f_fradius,
                                      int f_nroi_no) 
{
    TIPREP_TRACE(CROIHandler::CalCircleCordination,MRTraceMarker3);
    
    try {
        float f_x0 = 15.0f;
        float f_y0 = 15.0f;
        float f_Radius = 3.0f;
        char* l_ptr = "";
        CCircleROIParams l_crRoiparam;
        
        switch(/*m_Roi_count*/f_nroi_no) { 
        case 0:
            l_crRoiparam = m_vRoiparam.at(0);
            f_x0 = l_crRoiparam.GetCenter_X();
            f_y0 = l_crRoiparam.GetCenter_Y();
            f_Radius = l_crRoiparam.GetRadius();
            break;
        case 1:
            l_crRoiparam = m_vRoiparam.at(1);
            f_x0 = l_crRoiparam.GetCenter_X();
            f_y0 = l_crRoiparam.GetCenter_Y();
            f_Radius = l_crRoiparam.GetRadius();
            break;
        case 2:
            l_crRoiparam = m_vRoiparam.at(2);
            f_x0 = l_crRoiparam.GetCenter_X();
            f_y0 = l_crRoiparam.GetCenter_Y();
            f_Radius = l_crRoiparam.GetRadius();
            break;
        case 3:
            l_crRoiparam = m_vRoiparam.at(3);
            f_x0 = l_crRoiparam.GetCenter_X();
            f_y0 = l_crRoiparam.GetCenter_Y();
            f_Radius = l_crRoiparam.GetRadius();
            break;
        case 4:
            l_crRoiparam = m_vRoiparam.at(4);
            f_x0 = l_crRoiparam.GetCenter_X();
            f_y0 = l_crRoiparam.GetCenter_Y();
            f_Radius = l_crRoiparam.GetRadius();
            break;
        default:
            return E_ERROR;
        }
        
        //  }
        f_Radius = fabs(f_Radius);
        x1 = (f_x0 - f_Radius);
        y1 = (f_y0 - f_Radius);
        x2 = (f_x0 + f_Radius);
        y2 = (f_y0 + f_Radius);
        f_fradius = (f_Radius);
    } catch(...) {
        CString csResult(_T("ROI Exception occurred in CalCircleCordination()"));
        CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
        return E_ERR_EXCEPTION;
    }
    
    return E_NO_ERROR;
}

/*************************************************************
* Method:       GetROICoordinatesFromFile
* Description:  This function is used
* Returns:      void
*************************************************************/
void CROIHandler::GetROICoordinatesFromFile()
{
    //read radius and center values from file.
}

/*************************************************************
* Method:       MouseMoveROI
* Description:  This function is used to handle Mouse move event.
* Parameter:    BOOL f_bCheckFlag
* Returns:      BOOL
*************************************************************/
BOOL CROIHandler::MouseMoveROI(BOOL f_bCheckFlag)
{
    TIPREP_TRACE(CROIHandler::MouseMoveROI,MRTraceMarker3);
    
    try {
        CComBSTR bsOperationPath;
        CString csScreenPath = m_csOperationPath.Left(8);
        bsOperationPath = CComBSTR(static_cast<LPCTSTR>(csScreenPath));
        
        if(m_ppCosVisObject == NULL) {
            CString csResult(_T("NULL pointer"));
            CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
            return FALSE;
        }
        
        // 2.set active input mode to select ROI objects
        
        if(FAILED(m_ppMamROIControlHandlerManager->SetCanEdit(bsOperationPath, f_bCheckFlag))) {
            CString csResult(_T("SetCanEdit is failed."));
            CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
            return FALSE;
        }
        
        CString path = bsOperationPath;
        path = path + _T("/*/*");
        CComBSTR bPath = path;
        
        if(FAILED(m_ppMamROIControlHandlerManager->SetDrawingROIMode(bPath, FALSE))) {
            CString csResult(_T("SetDrawingROIMode is failed."));
            CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
            return FALSE;
        }
        
        CComBSTR active;
        m_ppCosVisObject->GetActiveInputMode(&active);
        CString csActiveMode(active);
        
        if(0 == csActiveMode.Compare(_T("Default"))) {
            return FALSE;
        }
        
        _bstr_t Edit = _T("Default");
        
        if(FAILED(m_ppCosVisObject->SetActiveInputMode(Edit))) {
            CString csResult(_T("SetActiveInputMode is failed."));
            CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
            return FALSE;
        }
        
        return TRUE;
    } catch(...) {
        CString csResult(_T("ROI Exception in MouseMoveROI()"));
        CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
        return FALSE;
    }
}

/*************************************************************
* Method:       GetROIPosition
* Description:  This function is used to get ROI parameter.
* Parameter:    CString * f_csMoveCenterX
*               Contains amount by which center is moved in X direction.
* Parameter:    CString * f_csMoveCenterY
*               Contains amount by which center is moved in X direction.
* Parameter:    CString * f_csMoveRadius
*               Contains amount by which radius is changed.
* Returns:      BOOL
*************************************************************/
BOOL CROIHandler::GetROIPosition(CString *f_csMoveCenterX,
                                 CString *f_csMoveCenterY,
                                 CString *f_csMoveRadius,/*Height for elliplse*/ 
                                 CString *f_csMoveWidth)
{
    TIPREP_TRACE(CROIHandler::GetROIPosition,MRTraceMarker3);
    HRESULT hr;
    
    try {
        CComBSTR bsOperationPath;
        bsOperationPath = CComBSTR(static_cast<LPCTSTR>(m_csOperationPath));
        
        if(NULL == m_ppMamROIControlHandlerManager) {
            CString csResult(_T("NULL pointer."));
            CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
            return FALSE;
        }
        
        // Get ROI UID of selected ROIs
        int nROICount = 0;
        BSTR* pUIDList = NULL;
        hr = m_ppMamROIControlHandlerManager->GetMeasurementList(bsOperationPath,
            &nROICount,
            &pUIDList,
            AllROI);
        
        if(FAILED(hr) || (NULL == pUIDList)) {  
            CString csResult(_T("GetMeasurementList is failed."));
            CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
            return FALSE;
        }
        
        int l_uidList_size = m_uid_point_list.GetSize();
		//TRACE COMMENTED
#ifdef _UT_LOG_
        CString l_trace_line (_T(""));
        l_trace_line.Format(_T("Obtaining current ROI positions using GetMeasurementObjectInImageCoordination():"));
        CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker4, l_trace_line,
            _T("CROIHandler::GetROIPosition"));
#endif
        for(int l_ni = 0; l_ni < /*nROICount*/ NUMBEROFROI; l_ni++/*, pUIDList++*/) { //AN //Modified
            if(NULL == pUIDList) {
                CString l_cserr(_T("GetROIPosition () UID is NULL"));
                CTiPrepLogHelper::WriteToErrorLog(l_cserr, __LINE__, __FILE__);
                return FALSE;
            }
            
            CurvePoint_t* pVectorList = NULL;
            int nVectorCount = 2;
            ROIObject_t roiObject;

            hr = m_ppMamROIControlHandlerManager->GetMeasurementObjectInImageCoordination(bsOperationPath,
                *(pUIDList),
                &roiObject,
                &nVectorCount,
                &pVectorList);
            
            if(FAILED(hr)) {
                CString csResult(_T("GetMeasurementObjectInImageCoordination failed."));
                CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
                pUIDList -= l_ni; 
                ReleaseUIDList(pUIDList, nROICount);
                return FALSE;
            }
			//TRACE COMMENTED
#ifdef _UT_LOG_
            l_trace_line.Format(_T("vector [0].x : %0.6lf,vector[0].y : %0.6lf"),pVectorList[0].x,pVectorList[0].y);
            CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker4, l_trace_line,
                _T("CROIHandler::SaveMoveROI"));
            l_trace_line.Format(_T("vector [1].x : %0.6lf,vector[1].y : %0.6lf"),pVectorList[1].x,pVectorList[1].y);
            CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker4, l_trace_line,
                _T("CROIHandler::SaveMoveROI"));
#endif

            if((0 == nVectorCount) || (NULL == pVectorList)) {
                CString csResult(_T("GetMeasurementObjectInImageCoordination CurvePoint Vector NULL."));
                CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
                return FALSE;
            }
			//TRACE COMMENTED
#ifdef _UT_LOG_
            l_trace_line = _T("Calculating center positions and dimensions from above vector values :");
            CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker4, l_trace_line,
                _T("CROIHandler::GetROIPosition"));
#endif
            float l_x0 = static_cast<float>((pVectorList[0].x + pVectorList[1].x) * 0.5);
            float l_y0 = static_cast<float>((pVectorList[0].y + pVectorList[1].y) * 0.5);
            float l_r0 = static_cast<float>(fabs((pVectorList[1].y - l_y0) * 2.0));  //Height
            float l_r1 = static_cast<float>(fabs((pVectorList[1].x - l_x0) * 2.0)); //Widht
            //l_x0 = l_x0 + 0.05;
            f_csMoveCenterX[l_ni].Format(_T("%0.1f"), (10*l_x0 + 0.5)*0.1 );
            //l_y0 = l_y0 + 0.05;
            f_csMoveCenterY[l_ni].Format(_T("%0.1f"), (10*l_y0 + 0.5)*0.1 );
            //l_r0 = l_r0 + 0.05;
            f_csMoveRadius[l_ni].Format(_T("%0.1f"), (10*l_r0 + 0.5)*0.1 );//its height parameter for ellipse.
            //l_r1 = l_r1 + 0.05;
            f_csMoveWidth[l_ni].Format(_T("%0.1f"), (10*l_r1 + 0.5)*0.1);
            ReleaseVectorList(pVectorList);
			//TRACE COMMENTED
#ifdef _UT_LOG_
            l_trace_line.Format(_T(" ROI %d, Center-X : %0.6lf, Center-Y : %0.6lf, height :%0.6lf, width: %0.6lf"),
                l_ni,l_x0,l_y0,l_r0,l_r1);
            CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker4, l_trace_line,
                _T("CROIHandler::GetROIPosition"));
#endif
            if((NULL != pUIDList) && (NULL != (pUIDList + 1))) {
                pUIDList++;
            }
        }
        
        pUIDList -= NUMBEROFROI ; //AN
        ReleaseUIDList(pUIDList, nROICount);
    } catch(...) {
        CString csResult(_T("ROI Exception occurred in GetROIPosition"));
        CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
        return FALSE;
    }
    
    return TRUE;
}

/*************************************************************
* Method:       ReleaseUIDList
* Description:  This function is used to release ROI UID.
* Parameter:    BSTR * pUIDList
*               Contains UID list.
* Parameter:    int nCount
*               Number of total UID in list.
* Returns:      void
*************************************************************/
void CROIHandler::ReleaseUIDList(BSTR* pUIDList, int nCount)
{
    TIPREP_TRACE(CROIHandler::ReleaseUIDList,MRTraceMarker3);
    
    try {
        if(NULL == pUIDList) {
            return;
        }
        
        for(int i = 0; i < nCount; i++) {
            if(NULL != pUIDList[i]) {
                ::SysFreeString(*(pUIDList + i));
                *(pUIDList + i) = NULL;
            }
        }
        
        ::CoTaskMemFree(pUIDList);
        pUIDList = NULL;
    }  catch(CMemoryException &e) {
        TCHAR   szCause[255] = L"";
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Memory can not be allocated because of this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
    } catch(CFileException &e) {
        TCHAR   szCause[255] = L"";
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Memory can not be allocated because of this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
    } catch(CException &e) {
        TCHAR   szCause[255] = L"";
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Memory can not be allocated because of this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
    } catch(...) {
        CString l_cserrmsg = _T("Exception Occurred in ReleaseUIDList");
        CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
    }
}

/*************************************************************
* Method:       ResetROIPosition
* Description:  This function is used to reset ROI to original
*               position.
* Returns:      BOOL
*************************************************************/
BOOL CROIHandler::ResetROIPosition(CString cs,int f_nImageHt,int f_nImageWt)
{
    TIPREP_TRACE(CROIHandler::ResetROIPosition,MRTraceMarker3);
    
    try {
#if 0 
   /*
        //Reset to default positions
        float l_dx0 = -100.0f;
        float l_dy0 = -90.0f;
        //float l_dRadius = 3.0;
        //By default shape of ROI will be circular.
        float l_fHeight = 8.0f; 
        float l_fWidth = 8.0f;
        CEllipticalROIParams l_crParam;        
    
        if(m_vEllipseRoiparam.empty() == false) {
            m_vEllipseRoiparam.clear();                                                
        }
        
        if(m_vEllipseRoiparam.empty()) {
            
            CString l_csLogMsg = (_T("Values set in vector 'm_vEllipseRoiparam' at start of 'ResetROI' function"));
            CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker4, l_csLogMsg);
            l_csLogMsg.Empty();

            if(cs.CompareNoCase(_T("AVG")) == 0) {//AVG case
                //             if(f_nImageWt >0 &&f_nImageHt >0){
                //                 l_dx0 = -(3.0*f_nImageWt/8.0);
                //                 l_dy0 = -(3.0*f_nImageHt/8.0);
                //             }else{
                //             }
                for(int l_nRoi = 0; l_nRoi < MAX_LINE_ROI; l_nRoi++) {
                    if(l_fHeight < 0) {
                        l_fHeight = -l_fHeight;
                    }
                    
                    if(l_fWidth < 0) {
                        l_fWidth = -l_fWidth;
                    }
                    
                    l_crParam.SetHeight(l_fHeight);
                    l_crParam.SetWidth(l_fWidth);
                    l_crParam.SetCenter_X(l_dx0 +(l_nRoi)*15);
                    l_crParam.SetCenter_Y(l_dy0 *(0 + 1));
                    
                    l_csLogMsg.Format(_T("ROI No.=%d CenterX=%0.6f CenterY=%0.6f Height=%0.6f Width=%0.6f \n"), 
                        l_nRoi, l_dx0 +(l_nRoi)*15,l_dy0,l_fHeight, l_fWidth);
                    CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker4, l_csLogMsg);
                    
                    m_vEllipseRoiparam.push_back(l_crParam);
                }
            } else {//MAP case
                for(int l_nRoi = 0; l_nRoi < MAX_LINE_ROI; l_nRoi++) {
                    if(0 == l_nRoi){
                        //                     l_dx0 = -(3.0*f_nImageWt/8.0);
                        //                     l_dy0 = -(3.0*f_nImageHt/8.0);
                        l_fHeight = 35;
                        l_fWidth = 35;
                    }else if(1 == l_nRoi){
                        //                     l_dx0 = -(3.0*f_nImageWt/8.0);
                        //                     l_dy0 = -(3.0*f_nImageHt/8.0);
                        l_fHeight = 55;
                        l_fWidth = 55;
                    }else{
                        //                     if(f_nImageWt >0 &&f_nImageHt >0){
                        //                         l_dx0 = -(3.0*f_nImageWt/8.0);
                        //                         l_dy0 = -(3.0*f_nImageHt/8.0);
                        //                     }else{
                        //                     }
                        l_fHeight = 8.0f;
                        l_fWidth = 8.0f;
                    }
                    if(l_fHeight < 0) {
                        l_fHeight = -l_fHeight;
                    }
                    if(l_fWidth < 0) {
                        l_fWidth = -l_fWidth;
                    }
                    l_crParam.SetHeight(l_fHeight);
                    l_crParam.SetWidth(l_fWidth);
                    if(0 == l_nRoi || 1 == l_nRoi){
                        l_crParam.SetCenter_X(l_dx0);
                    }else{
                        l_crParam.SetCenter_X(l_dx0 +(l_nRoi)*15);
                    }
                    l_crParam.SetCenter_Y(l_dy0 *(0 + 1));
                    
                    
                    l_csLogMsg.Format(_T("ROI No.=%d CenterX=%0.6f CenterY=%0.6f Height=%0.6f Width=%0.6f \n"), 
                        l_nRoi,l_dx0 +(l_nRoi)*15,l_dy0,l_fHeight, l_fWidth);
                    CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker4, l_csLogMsg);
                    
                    m_vEllipseRoiparam.push_back(l_crParam);
                }
            }
        }
        
        BOOL l_bResult = SetRoiPosition();//Communicate to set the ROi
        
        if(TRUE != l_bResult) {
            CString csResult(_T("Unable to ResetROIPositions"));
            CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
            return FALSE;
        }
        */
#else
        if(NULL == m_ppMamROIControlHandlerManager) {
            CString csResult(_T("NULL pointer.RESET Failed"));
            CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
            return FALSE;
        }
        HRESULT hr;
        int nVectorCount = 2;
        CComBSTR bsOperationPath;
        bsOperationPath = CComBSTR(static_cast<LPCTSTR>(m_csOperationPath));
        
        int nROICount = 0;
#ifdef _UT_LOG_

       {
            CString l_trace_line (_T(""));
            l_trace_line.Format(_T("\n---------------------------RESET ROI PROC START-------------------------\n"));
            CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker5, l_trace_line,
                _T("CROIHandler::ResetROIPosition"));
        }
#endif
        //-----------------------To get UID list and ROIcount-------------------------
        BSTR* pUIDList = NULL;
#ifdef _UT_LOG_
        {
            CString l_trace_line (_T(""));
            l_trace_line.Format(_T("\n-------------------GetMeasurementList\n"));
            CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker5, l_trace_line,
                _T("CROIHandler::ResetROIPosition"));
        }
#endif
        hr = m_ppMamROIControlHandlerManager->GetMeasurementList(
            bsOperationPath,
            &nROICount,
            &pUIDList,
            AllROI);
#ifdef _UT_LOG_
        {
            CString l_trace_line (_T(""));
            l_trace_line.Format(_T("\nRESET ---->pUID = %s\n"),*pUIDList);
            CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker5, l_trace_line,
                _T("CROIHandler::ResetROIPosition"));
        }
#endif
        
        if(FAILED(hr) || (NULL == pUIDList)) {  
            CString csResult(_T("GetMeasurementList failed. fail to get ROI position from Image."));
            CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
            return FALSE;
        }

        CurvePoint_t * pVectorList = NULL;

        CurvePoint_t * l_pVectorList = NULL;
        l_pVectorList = new CurvePoint_t[2];
        if(NULL == l_pVectorList){
            CString csResult(_T("RESET ---->l_pVectorList is NULL."));
            CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
            return FALSE;
        }

        for(int l_nRoi = 0; l_nRoi < nROICount; l_nRoi++) {
#ifdef _UT_LOG_
            if(l_nRoi == 0){
                CString l_trace_line (_T(""));
                l_trace_line.Format(_T("\nRESET ---->BSOperation path= %s\n"),m_csOperationPath);
                CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker5, l_trace_line,
                    _T("CROIHandler::ResetROIPosition"));
            }
#endif
            memset(l_pVectorList,0x0,2*sizeof(CurvePoint_t));
            ROIObject_t roiObject;
            //-----------------------To get Vector info  of current ROI in loop-------------------------
#ifdef _UT_LOG_
            if(l_nRoi == 0){
                CString l_trace_line (_T(""));
                l_trace_line.Format(_T("\n-------------------GetMeasurementObjectInImageCoordination\n"));
                CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker5, l_trace_line,
                    _T("CROIHandler::ResetROIPosition"));
            }
#endif
            hr = m_ppMamROIControlHandlerManager->GetMeasurementObjectInImageCoordination(bsOperationPath,
                *(pUIDList),
                &roiObject,
                &nVectorCount,
                &pVectorList);
            
            if(FAILED(hr)) {
                CString csResult(_T("GetMeasurementObjectInImageCoordination failed."));
                CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
                pUIDList -= l_nRoi; 
                ReleaseUIDList(pUIDList, nROICount);
                ReleaseVectorList(l_pVectorList);
                return FALSE;
            }            
            if((0 >= nVectorCount) || (NULL == pVectorList)) {
                CString csResult(_T("pVectorList is NULL"));
                CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
                pUIDList -= l_nRoi; 
                ReleaseUIDList(pUIDList, nROICount);
                ReleaseVectorList(l_pVectorList);
                return FALSE;
            }
             
#ifdef _UT_LOG_
            
            if(l_nRoi == 0){
                CString l_trace_line (_T(""));
                l_trace_line.Format(_T("\nRESET ---------->ROIObject[RoiNo:%d]:\n")
                    _T("No of point =%d\n")
                    _T("isselect = %d\n")
                    _T("angle = %lf\n")
                    _T("handleType =%u\n")
                    _T("ROI type = %d\n")
                   _T("max_point = %d\n")
                    _T("visible = %d\n")
                    _T("selectable = %d\n")
                    _T("color_name = %d\n")
                    _T("text_size[0] = %d\n")
                    _T("text_size[1] = %d\n")
                    _T("lineType = %d\n")
                    _T("lineWidth = %d\n")
                    _T("font_size = %d\n")
                    _T("displayId = %d\n")
                    _T("callout_fixed = %d\n")
                    _T("callout_position[0] = %f\n")
                    _T("callout_position[1] = %f\n")
                    _T("text = %s\n")
                    _T("arm1Length = %lf\n")
                    _T("arm2Length = %lf\n")
                    _T("markerType = %d\n")
                    _T("owner = %s\n")
                    _T("arrow type = %d\n")
                    _T("opStatus = %d\n")
                    _T("InformationVisible = %d\n")
                    _T("text_fontType = %s\n")
                    ,
                    l_nRoi,
                    roiObject.vectorListCount,
                    roiObject.isselect ,
                    roiObject.angle ,
                    roiObject.handleType ,
                    roiObject.type,
                    roiObject.max_point ,
                    roiObject.visible ,
                    roiObject.selectable ,
                    roiObject.color_name ,
                    roiObject.text_size[0] ,
                    roiObject.text_size[1] ,
                    roiObject.lineType ,
                    roiObject.lineWidth ,
                    roiObject.font_size ,
                    roiObject.displayId ,
                    roiObject.callout_fixed ,
                    roiObject.callout_position[0] ,
                    roiObject.callout_position[1] ,
                    roiObject.text ,
                    roiObject.arm1Length ,
                    roiObject.arm2Length ,
                    roiObject.markerType ,
                    roiObject.owner ,
                    roiObject.arrow ,
                    roiObject.opStatus ,
                    roiObject.InformationVisible ,
                    roiObject.text_fontType);
                
                
                CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker5, l_trace_line,
                    _T("CROIHandler::ResetROIPosition"));
        }
#endif 

            //------------------ReSet the Original Vectorinfo to current roi in the loop-------------------------------------

            l_pVectorList[0].x = m_curveptinfo[l_nRoi].CurvePts[0].x;
            l_pVectorList[0].y = m_curveptinfo[l_nRoi].CurvePts[0].y;            
            l_pVectorList[0].z = m_curveptinfo[l_nRoi].CurvePts[0].z;  
            l_pVectorList[1].x = m_curveptinfo[l_nRoi].CurvePts[1].x;
            l_pVectorList[1].y = m_curveptinfo[l_nRoi].CurvePts[1].y;
            l_pVectorList[1].z = m_curveptinfo[l_nRoi].CurvePts[1].z;
        
            //-------------------------------------------trace LOG-------------------------------------------------------
#ifdef _UT_LOG_
            if(l_nRoi == 0){
                CString l_trace_line(_T(""));
                l_trace_line.Format(_T("\nRESET ---------->GetMeasurementObjectInImageCoordination() output:\n")
                                    _T("ROI[%d]x0 : %f,y0 : %f x1 : %f,y1 : %f\n"),                                    
                                    l_nRoi,pVectorList[0].x,pVectorList[0].y,pVectorList[1].x,pVectorList[1].y);
                CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker5, l_trace_line,
                    _T("CROIHandler::ResetROIPosition"));
            }

            if(l_nRoi == 0){
                CString l_trace_line(_T(""));
                l_trace_line.Format(_T("\nRESET ---------->Curve Points Stored Initially and input to SetMeasurementObject:\n")                
                    _T("ROI[%d]x0 : %f,y0 : %f x1 : %f,y1 : %f\n"),                                    
                    l_nRoi,l_pVectorList[0].x,l_pVectorList[0].y,l_pVectorList[1].x,l_pVectorList[1].y);
                 CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker5, l_trace_line,
                    _T("CROIHandler::ResetROIPosition"));
            }
            //-------------------------------------------trace LOG-------------------------------------------------------
            if(l_nRoi == 0){
                CString l_trace_line (_T(""));
                l_trace_line.Format(_T("\n-------------------SetMeasurementObject\n"));
                CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker5, l_trace_line,
                    _T("CROIHandler::ResetROIPosition"));
            }
#endif

            hr = m_ppMamROIControlHandlerManager->SetMeasurementObject(bsOperationPath,
                *(pUIDList),
                roiObject,
                nVectorCount,
                //pVectorList);
            l_pVectorList);
        
            if(FAILED(hr)) {
                CString csResult(_T("Reset SetMeasurementObject failed."));
                CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
                pUIDList -= l_nRoi; 
                ReleaseUIDList(pUIDList, nROICount);
                ReleaseVectorList(l_pVectorList);
                return FALSE;
            }                    
        
		//*************************************************************

	    int vectorListNum = 2;
		CurvePoint_t* vectorList = NULL;
        vectorList = new CurvePoint_t[2];
        if(vectorList){
            memset(vectorList , 0x0 ,2*sizeof(CurvePoint_t));
#ifdef _UT_LOG_

            if(l_nRoi == 0){
                CString l_trace_line (_T(""));
        		l_trace_line.Format(_T("\n-------------------GetCurvePoint after Set \n"));
                CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker5, l_trace_line,
                    _T("CROIHandler::ResetROIPosition"));
            }
            
			m_ppMamROIControlHandlerManager->GetCurvePoint(bsOperationPath,*(pUIDList),vectorListNum,vectorList);
            if(l_nRoi == 0){
                CString l_trace_line (_T(""));
                 l_trace_line.Format(_T("\nGetCurvePoint output after Set-->\nROI[%d] :x0 = %f,y0 = %f,x1 = %f,y1 = %f\n"),l_nRoi,
                                    vectorList[0].x,vectorList[0].y,vectorList[1].x,vectorList[1].y);
                CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker5, l_trace_line,
                _T("CROIHandler::ResetROIPosition"));
            }
#endif
        }
        //*************************************************************

            if((NULL != pUIDList) && (NULL != (pUIDList + 1))) {
                pUIDList++;
            }
            ReleaseVectorList(pVectorList); 
        }
        pUIDList -= (nROICount); 
        ReleaseUIDList(pUIDList, nROICount);
        ReleaseVectorList(l_pVectorList);
        //update CosVis
        if(NULL != m_ppCosVisObject){
            if(FAILED(m_ppCosVisObject->ForceUpdate())) {
                CString csResult(_T("Fail to Update CosVis."));
                CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
                return FALSE;
            }
        }
#ifdef _UT_LOG_
        {
            CString l_trace_line (_T(""));
            l_trace_line.Format(_T("\n---------------------------RESET ROI PROC END-------------------------\n"));
            CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker5, l_trace_line,
                _T("CROIHandler::ResetROIPosition"));
        }
#endif


#endif
    } catch(...) {
        CString csResult(_T("ROI Exception occurred in ResetROIPosition"));
        CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
        return FALSE;
    }
    
    return TRUE;
}

/*************************************************************
* Method:       SaveMoveROI
* Description:  This function is used move ROI from original
*               position and save new ROI position.
* Parameter:    CString f_csMoveCenterX[]
*               Contains amount by which center is moved in X direction.
* Parameter:    CString f_csMoveCenterY[]
*               Contains amount by which center is moved in X direction.
* Parameter:    CString f_csMoveRadius[]
*               Contains amount by which radius is changed.
* Returns:      BOOL
*************************************************************/
BOOL CROIHandler::SaveMoveROI(CString f_csMoveCenterX[],
                              CString f_csMoveCenterY[],
                              CString f_csMoveRadius[],
                              CString f_csMoveWidth[])
{
    TIPREP_TRACE(CROIHandler::SaveMoveROI,MRTraceMarker3);
    
    try {
        HRESULT hr;
        CComBSTR bsOperationPath;
        bsOperationPath = CComBSTR(static_cast<LPCTSTR>(m_csOperationPath));
        
        if(NULL == m_ppMamROIControlHandlerManager) {
            CString csResult(_T("NULL pointer. Fail to move ROI position."));
            CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
            return FALSE;
        }
#ifdef _UT_LOG_
        CString l_trace_line (_T(""));
#endif
            
        // Get ROI UID of selected ROIs
        int nROICount = 0;
        BSTR* pUIDList = NULL;
        hr = m_ppMamROIControlHandlerManager->GetMeasurementList(
            bsOperationPath,
            &nROICount,
            &pUIDList,
            AllROI);
        
        if(FAILED(hr) || (NULL == pUIDList)) {  
            CString csResult(_T("GetMeasurementList failed. fail to get ROI position from Image."));
            CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
            return FALSE;
        }
		//TRACE COMMENTED
#ifdef _UT_LOG_
        l_trace_line.Format(_T("SaveMoveROI() Input Parameters values retrieved from string values:"));
        CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker4, l_trace_line,
                    _T("CROIHandler::SaveMoveROI"));
#endif
        for(int l_ni = 0; l_ni < NUMBEROFROI; l_ni++/*, pUIDList++*/) {
            float l_x_destination = 0.0f;
            float l_y_destination = 0.0f;
            float l_x_displacement = 0.0f;
            float l_y_displacement = 0.0f;
            float l_rad_destination = 0.0f;
            float l_fHeight_destination = 0.0f;
            float l_fWidth_destination = 0.0f;
            double l_temp_cenX = 0.0;
            double l_temp_cenY = 0.0;
            double l_temp_radius = 0.0;
            double l_temp_width = 0.0;
            TCHAR* pTchar = NULL;
            
            l_temp_cenX = _tcstod(f_csMoveCenterX[l_ni] , &pTchar);
            pTchar = NULL;
            l_temp_cenY = _tcstod(f_csMoveCenterY[l_ni] , &pTchar);
            pTchar = NULL;
            l_temp_radius = _tcstod(f_csMoveRadius[l_ni] , &pTchar);//it contains height for ellipse
            pTchar = NULL;
            l_temp_width = _tcstod(f_csMoveWidth[l_ni], &pTchar); //it contains width for ellipse
            l_x_destination = static_cast<float>(l_temp_cenX);
            l_y_destination = static_cast<float>(l_temp_cenY);
            l_fHeight_destination = static_cast<float>(fabs(l_temp_radius));
            l_fWidth_destination = static_cast<float>(fabs(l_temp_width));
            // Get selected ROI current position
            CurvePoint_t* pVectorList = NULL;
            int nVectorCount = 2;
            ROIObject_t roiObject;
            
            if(NULL == pUIDList) {
                CString l_cserr(_T("GetROIPosition () UID is NULL"));
                CTiPrepLogHelper::WriteToErrorLog(l_cserr, __LINE__, __FILE__);
                return FALSE;
            }
			//TRACE COMMENTED
#ifdef _UT_LOG_
            l_trace_line.Format(_T("ROI %d, Center-X : %0.6lf, Center-Y : %0.6lf, height :%0.6lf, width: %0.6lf"),
                l_ni, l_x_destination,l_y_destination,l_fHeight_destination,l_fWidth_destination);
            CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker4, l_trace_line,
                    _T("CROIHandler::SaveMoveROI"));
#endif
            hr = m_ppMamROIControlHandlerManager->GetMeasurementObjectInImageCoordination(bsOperationPath,
                *(pUIDList),
                &roiObject,
                &nVectorCount,
                &pVectorList);
            
            if(FAILED(hr)) {
                CString csResult(_T("GetMeasurementObjectInImageCoordination failed."));
                CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
                pUIDList -= l_ni; 
                ReleaseUIDList(pUIDList, nROICount);
                return FALSE;
            }

            if((0 == nVectorCount) || (NULL == pVectorList)) {
                return FALSE;
            }
#ifdef _UT_LOG_
            l_trace_line.Format(_T("GetMeasurementObjectInImageCoordination() output parameter values:"));
            CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker4, l_trace_line,
                _T("CROIHandler::SaveMoveROI"));
    
            l_trace_line.Format(_T("vector [0].x : %0.6lf,vector[0].y : %0.6lf"),pVectorList[0].x,pVectorList[0].y);
            CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker4, l_trace_line,
                _T("CROIHandler::SaveMoveROI"));
            l_trace_line.Format(_T("vector [1].x : %0.6lf,vector[1].y : %0.6lf"),pVectorList[1].x,pVectorList[1].y);
            CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker4, l_trace_line,
                _T("CROIHandler::SaveMoveROI"));
#endif
            float l_x0 = static_cast<float>((pVectorList[0].x + pVectorList[1].x) * 0.5);
            float l_y0 = static_cast<float>((pVectorList[0].y + pVectorList[1].y) * 0.5);
            float l_r0 = static_cast<float>(fabs((pVectorList[1].y - l_y0) * 2.0));   
            float l_r1 = static_cast<float>(fabs((pVectorList[1].x - l_x0) * 2.0));
#ifdef _UT_LOG_
            l_trace_line.Format(_T("ROI center and dimensions values calculated from above vector positions: "));
            CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker4, l_trace_line,
                _T("CROIHandler::SaveMoveROI"));

            l_trace_line.Format(_T("ROI %d, Center-X : %0.6lf, Center-Y : %0.6lf, height :%0.6lf, width: %0.6lf"),
                l_ni,l_x0,l_y0,l_r0,l_r1);
            CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker4, l_trace_line,
                _T("CROIHandler::SaveMoveROI"));
#endif

            if(
                !(  
                    ( fabs(l_x0 - l_x_destination) < 0.000001f )    && 
                    ( fabs(l_y0 - l_y_destination) < 0.000001f )    && 
                    ( fabs(l_r0 - l_fHeight_destination) < 0.000001f )  && 
                    ( fabs(l_r1 - l_fWidth_destination) < 0.000001f )
                 )
               )
                {
                CString csResult;
                pVectorList[0].x = static_cast<float>(l_temp_cenX) - static_cast<float>(fabs((l_temp_width * 0.5))); 
                pVectorList[0].y = static_cast<float>(l_temp_cenY) - static_cast<float>(fabs((l_temp_radius * 0.5)));
                pVectorList[0].z = 0.0f;
                pVectorList[1].x = static_cast<float>(l_temp_cenX) + static_cast<float>(fabs(l_temp_width * 0.5));//   width for ellipse
                pVectorList[1].y = static_cast<float>(l_temp_cenY) + static_cast<float>(fabs(l_temp_radius * 0.5));//   height for ellipse
                pVectorList[1].z = 0.0f;
                //---- Save new ROI Position ----//
                BOOL l_bResult = SaveNewROIPosition(*pUIDList, static_cast<float>(l_temp_cenX),
                    static_cast<float>(l_temp_cenY),
                    static_cast<float>(l_temp_radius),
                    static_cast<float>(l_temp_width));
                
                if(TRUE != l_bResult) {
                    CString csResult(_T("Failed to Save new ROI positions."));
                    CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
                }
                
                //---- Reset the ROI in original position----//
#ifdef _UT_LOG_
                l_trace_line.Format(_T("input parameter values set for SetMeasurementObject() :"));
                CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker4, l_trace_line,
                    _T("CROIHandler::SaveMoveROI"));
                
                l_trace_line.Format(_T("vector [0].x : %0.6lf,vector[0].y : %0.6lf"),
                    pVectorList[0].x,pVectorList[0].y);
                CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker4, l_trace_line,
                    _T("CROIHandler::SaveMoveROI"));
                l_trace_line.Format(_T("vector [1].x : %0.6lf,vector[1].y : %0.6lf"),
                    pVectorList[1].x,pVectorList[1].y);
                CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker4, l_trace_line,
                    _T("CROIHandler::SaveMoveROI"));
#endif

                hr = m_ppMamROIControlHandlerManager->SetMeasurementObject(bsOperationPath,
                    *(pUIDList),
                    roiObject,
                    nVectorCount,
                    pVectorList);

                if(FAILED(hr)) {
                    CString csResult(_T("SetMeasurementObject failed."));
                    CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
                    pUIDList -= l_ni; 
                    ReleaseUIDList(pUIDList, nROICount);
                    return FALSE;
                }
            }
            
            ReleaseVectorList(pVectorList);
            
           
            if((NULL != pUIDList) && (NULL != (pUIDList + 1))) {
                pUIDList++;
            }
            
           
        }
        
        pUIDList -= (NUMBEROFROI); //-1); //AN
        ReleaseUIDList(pUIDList, nROICount);
        
        //update CosVis
        if(FAILED(m_ppCosVisObject->ForceUpdate())) {
            CString csResult(_T("Fail to Update CosVis."));
            CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
            return FALSE;
        }
    } catch(...) {
        CString csResult(_T("ROI Exception occurred in SaveMoveROI"));
        CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
        return FALSE;
    }
    
    return TRUE;
}

/*************************************************************
* Method:       DeleteROI
* Description:  This function is to delete ROI on exit of TIPrep.
* Returns:      BOOL
*************************************************************/
BOOL CROIHandler::DeleteROI()
{
    TIPREP_TRACE(CROIHandler::DeleteROI,MRTraceMarker3);
    
    try {
        HRESULT hr;
        CComBSTR bsOperationPath;
        bsOperationPath = CComBSTR(static_cast<LPCTSTR>(m_csOperationPath));
        int nROICount = 0;
        BSTR* pUIDList = NULL;
        hr = m_ppMamROIControlHandlerManager->GetMeasurementList(bsOperationPath,
            &nROICount,
            &pUIDList,
            AllROI);
        
        if(FAILED(hr) || (NULL == pUIDList)) {   
            CString csResult(_T("GetMeasurementList Failed."));
            CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
            return FALSE;
        }
        if (nROICount <= 0) return TRUE;
        for(int l_ni = 0; l_ni < nROICount; l_ni++/*, pUIDList++*/) {
            hr = m_ppMamROIControlHandlerManager->DeleteROI(bsOperationPath, *(pUIDList));
            
            if(FAILED(hr)) {
                CString csResult(_T("DeleteROI Failed."));
                CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
                return FALSE;
            }
            
           
            if((NULL != pUIDList) && (NULL != (pUIDList + 1))) {
                pUIDList++;
            }
            
            
        }
        
        if(FAILED(m_ppCosVisObject->ForceUpdate())) {
            CString csResult(_T("Fail to Update CosVis."));
            CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
            return FALSE;
        }
        
        _bstr_t operation_path = _T("");
        _bstr_t bstr_GL_full_name = _bstr_t(static_cast<LPCTSTR>(m_csGLname));
        
        for(l_ni = 0; l_ni < m_opPathList.GetSize(); l_ni++) {
            operation_path = _bstr_t(static_cast<LPCTSTR>(m_opPathList.GetAt(l_ni)));
            hr = m_ppCosVisObject->UnRegisterObserver(bstr_GL_full_name, operation_path);
            
            if(FAILED(hr)) {
                CString csResult(_T("UnRegisterObserver Failed."));
                CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
                return FALSE;
            }
        }
        
        m_Roi_count = 0;
        m_uid_point_list.RemoveAll(); 
    } catch(...) {
        CString csResult(_T("ROI Exception occurred in DeleteROI"));
        CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
        return FALSE;
    }
    
    return TRUE;
}

/*************************************************************
* Method:       HideROI
* Description:  This function is used to hide the ROI.
* Parameter:    int f_nRoiNumber
*               Number of selected ROI.
* Parameter:    BOOL flag
*               Status of selected ROI.
* Returns:      BOOL
*************************************************************/
BOOL CROIHandler::HideROI(int f_nRoiNumber, BOOL flag)
{
    TIPREP_TRACE(CROIHandler::HideROI,MRTraceMarker3);
    HRESULT hr;
    
    try {
        CComBSTR bsOperationPath;
        int nROICount = 0;
        BSTR* pUIDList = NULL;
        bsOperationPath = CComBSTR(static_cast<LPCTSTR>(m_csOperationPath));
        /*
        Get the UID number of the ROI from f_nROINumber.
        */
        CString l_uid = m_uid_point_list.GetAt(f_nRoiNumber).uid;
        CComBSTR pUID;
        pUID = CComBSTR(static_cast<LPCTSTR>(l_uid));
        hr = m_ppMamROIControlHandlerManager->SetVisible(bsOperationPath,
            pUID, flag);
        
        if(FAILED(hr)) {
            CString csResult(_T("Fail to Set visible ROI."));
            CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
            return FALSE;
        }
        
        hr = m_ppMamROIControlHandlerManager->SetIsSelected(bsOperationPath,
            pUID, FALSE);
        if(FAILED(hr)) {
            CString csResult(_T("Fail to Set visible ROI."));
            CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
            return FALSE;
        }
        
        //update CosVis
        if(FAILED(m_ppCosVisObject->ForceUpdate())) {
            CString csResult(_T("Fail to Update CosVis."));
            CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
            return FALSE;
        }
    } catch(...) {
        CString csResult(_T("ROI Exception occurred in HideROI"));
        CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
        return FALSE;
    }
    
    return TRUE;
}

/*************************************************************
* Method:       SaveNewROIPosition
* Description:  This function save new ROI position of selected
*               ROI.
* Parameter:    BSTR f_UID
*               UID of selected ROI.
* Parameter:    float f_csCenterX
*               Contains amount by which center is moved in X
*               direction.
* Parameter:    float f_csCenterY
*               Contains amount by which center is moved in Y
*               direction.
* Parameter:    float f_csRadius
*               Contains amount by which radius is changed.
* Returns:      BOOL
*************************************************************/
BOOL CROIHandler::SaveNewROIPosition(BSTR f_UID, float f_csCenterX,
                                     float f_csCenterY,
                                     float f_csRadius,
                                     float f_fWidth)
{
    TIPREP_TRACE(CROIHandler::SaveNewROIPosition,MRTraceMarker3);
    
    try {
        int l_uidList_size = m_uid_point_list.GetSize();
        CString l_csTemp_uid(f_UID);
        
        for(int l_ni = 0; l_ni < l_uidList_size; l_ni++) {
            if(l_csTemp_uid == m_uid_point_list.GetAt(l_ni).uid) {
                Uid_Point l_temp;
                l_temp.uid = l_csTemp_uid;
                l_temp.x = f_csCenterX;
                l_temp.y = f_csCenterY;
                l_temp.radius = f_csRadius;
                l_temp.width = f_fWidth;
                m_uid_point_list.SetAt(l_ni, l_temp);
            }
        }
    } catch(...) {
        CString csResult(_T("ROI Exception occurred in SaveNewROIPosition"));
        CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
        return FALSE;
    }
    
    return TRUE;
}

/*************************************************************
* Method:       GetROIBufferInfo
* Description:  This function is used to get image map for
*               selected images.
* Parameter:    byte * l_csImROI[]
*               used to hold map  information for each ROI.
* Parameter:    int * l_nROIH
*               out parameter used to get height of images.
* Parameter:    int * l_nROIW
*               out parameter used to get width of images.
* Returns:      BOOL
*************************************************************/
BOOL CROIHandler::GetROIBufferInfo(byte* l_csImROI[], int *l_nROIH, int *l_nROIW)
{
    HRESULT hr;
    TIPREP_TRACE(CROIHandler::GetROIBufferInfo,MRTraceMarker3);
    
    try {
        CComBSTR bsOperationPath;
        bsOperationPath = CComBSTR(static_cast<LPCTSTR>(m_csOperationPath));
        
        if(NULL == m_ppMamROIControlHandlerManager) {
            CString csResult(_T("NULL Pointer."));
            CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
            return FALSE;
        }
        
        // Get ROI UID of selected ROIs
        int nROICount = 0;
        BSTR* pUIDList = NULL;
        hr = m_ppMamROIControlHandlerManager->GetMeasurementList(bsOperationPath,
            &nROICount,
            &pUIDList,
            AllROI);
        
        if(FAILED(hr) || (NULL == pUIDList)) {   
            CString csResult(_T("GetMeasurementList Failed."));
            CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
            return FALSE;
        }
        
        // Get ROI Object
        if(nROICount == 0) {
            CString csResult(_T("ROI count is zero."));
            CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
            return FALSE;
        } else {
            int nWidth = 0;
            int nHeight = 0;
            byte* pROIBuffer = NULL;
            int nSize = 0;
            
            for(int l_nROI = 0; l_nROI < NUMBEROFROI; l_nROI++/*,pUIDList++*/) {
                l_csImROI[l_nROI] = NULL; 
                hr = m_ppMamROIControlHandlerManager->GetROIBuffer(bsOperationPath,  //path
                    *(pUIDList),      //UID
                    &nSize,
                    &l_csImROI[l_nROI],      //Buffer
                    l_nROIW,          //Buffer width
                    l_nROIH);        //Buffer height
                
                if(FAILED(hr) || (NULL == l_csImROI[l_nROI])) {
                    CString csResult(_T("GetROIBuffer Failed."));
                    CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
                    pUIDList -= l_nROI; 
                    ReleaseUIDList(pUIDList, nROICount);
                    return FALSE;
                }
                
                //CoTaskMemFree( l_csImROI[l_nROI] );
                if((NULL != pUIDList) && (NULL != (pUIDList + 1))) {
                    pUIDList++;
                }
            }
        }
        
        pUIDList -= NUMBEROFROI; //AN
        ReleaseUIDList(pUIDList, nROICount);
    } catch(...) {
        CString csResult(_T("ROI Exception occurred in GetROIBufferInfo"));
        CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
        return FALSE;
    }
    
    return TRUE;
}

/*************************************************************
* Method:       GetsaveROIParams
* Description:  This function is used to get ROI parameters
*               from file.
* Parameter:    CString & f_csProtocolName
*               path for text file.
* Parameter:    CString cs
*               Protocol name.
* Returns:      BOOL
*************************************************************/
BOOL CROIHandler::GetsaveROIParams(CString &f_csProtocolName, 
                                   CString cs ,
                                   int f_nImageHt,
                                   int f_nImageWt)
{
    TIPREP_TRACE(CROIHandler::GetsaveROIParams,MRTraceMarker3)
        
        try{
        float l_dx0 = -100.0f;
        float l_dy0 = -90.0f;
        // float l_dRadius = 3.0;
        //By default shape of ROI will be circular.
        float l_fHeight = 8.0f; 
        float l_fWidth = 8.0f;
        BOOL l_bResult = FALSE;
        CEllipticalROIParams l_crParam; 
        
        if(m_vEllipseRoiparam.empty() == false) {
            m_vEllipseRoiparam.clear();                                               
        }
        
        if(!f_csProtocolName.IsEmpty()) {
            if(cs.CompareNoCase(_T("AVG")) == 0) {
                CString l_csFliePath = _T("c:/MrMplus/Data/TiPrep/AVGROIInfo.txt");
                l_bResult = m_RoiParamabj.ReadROIInfo(l_csFliePath,
                    f_csProtocolName,
                    m_vEllipseRoiparam);

                //Dont check returned error 
            } else {
                CString l_csFliePath = _T("c:/MrMplus/Data/TiPrep/MAPROIInfo.txt");
                l_bResult = m_RoiParamabj.ReadROIInfo(l_csFliePath,
                    f_csProtocolName,
                    m_vEllipseRoiparam);
                //Dont check returned error .....
            }
        }
        

        if(/*m_vRoiparam.empty()*/m_vEllipseRoiparam.empty()) {//default position set
	       //TRACE COMMENTED
#ifdef _UT_LOG_
            CString l_csLogMsg = (_T("ROI parameters are not available in text files so default values are being set"));
            CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker4, l_csLogMsg);
            l_csLogMsg.Empty();
#endif

            if(cs.CompareNoCase(_T("AVG")) == 0) {//AVG case
                //             if(f_nImageWt >0 &&f_nImageHt >0){
                //                 l_dx0 = -(3.0*f_nImageWt/8.0);
                //                 l_dy0 = -(3.0*f_nImageHt/8.0);
                //             }else{
                //             }
                
                for(int l_nRoi = 0; l_nRoi < NUMBEROFROI; l_nRoi++) {
                    if(l_fHeight < 0) {
                        l_fHeight = -l_fHeight;
                    }
                    
                    if(l_fWidth < 0) {
                        l_fWidth = -l_fWidth;
                    }
                    l_crParam.SetHeight(l_fHeight);
                    l_crParam.SetWidth(l_fWidth);
                    l_crParam.SetCenter_X(l_dx0 +(l_nRoi)*15);
                    l_crParam.SetCenter_Y(l_dy0 *(0 + 1));
                    m_vEllipseRoiparam.push_back(l_crParam);
                }
            } else {//MAP case
                for(int l_nRoi = 0; l_nRoi < MAX_LINE_ROI; l_nRoi++) {
                    if(0 == l_nRoi){
                        //                     l_dx0 = -(3.0*f_nImageWt/8.0);
                        //                     l_dy0 = -(3.0*f_nImageHt/8.0);
                        l_fHeight = 35.0f;
                        l_fWidth = 35.0f;
                    }else if(1 == l_nRoi){
                        //                     l_dx0 = -(3.0*f_nImageWt/8.0);
                        //                     l_dy0 = -(3.0*f_nImageHt/8.0);
                        l_fHeight = 55.0f;
                        l_fWidth = 55.0f;
                    }else{
                        //                     if(f_nImageWt >0 &&f_nImageHt >0){
                        //                         l_dx0 = -(3.0*f_nImageWt/8.0);
                        //                         l_dy0 = -(3.0*f_nImageHt/8.0);
                        //                     }else{
                        //                    }
                        l_fHeight = 8.0f;
                        l_fWidth = 8.0f;
                    }
                    if(l_fHeight < 0) {
                        l_fHeight = -l_fHeight;
                    }
                    
                    if(l_fWidth < 0) {
                        l_fWidth = -l_fWidth;
                    }
                    l_crParam.SetHeight(l_fHeight);
                    l_crParam.SetWidth(l_fWidth);
                    if(0 == l_nRoi || 1 == l_nRoi){
                        l_crParam.SetCenter_X(l_dx0);
                    }else{
                        l_crParam.SetCenter_X(l_dx0 +(l_nRoi)*15);
                    }
                    l_crParam.SetCenter_Y(l_dy0 *(0 + 1));
                    m_vEllipseRoiparam.push_back(l_crParam);
                }
            }
        } else{
			//TRACE COMMENTED
#ifdef _UT_LOG_
            CString l_csLogMsg = (_T("ROI parameters are read from text files."));
            CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker4, l_csLogMsg);
            l_csLogMsg.Empty();
#endif
        }
        
        return TRUE;
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_ROIHANDLER, __LINE__, __FILE__);
        return FALSE;
    }
}

/*************************************************************
* Method:       SaveUpdateROIParamsInFile
* Description:  This function is used
* Parameter:    CString f_csProtocolName
* Parameter:    CString cs
* Returns:      BOOL
*************************************************************/
BOOL CROIHandler::SaveUpdateROIParamsInFile(CString &f_csProtocolName, CString &cs)
{
    TIPREP_TRACE(CROIHandler::SaveUpdateROIParamsInFile,MRTraceMarker3)
        try{
#if 1
        HRESULT hr;
        m_vEllipseRoiparam.clear(); 
        BOOL l_bResult = FALSE;
        CEllipticalROIParams l_crParam; 
        CComBSTR bsOperationPath;
        bsOperationPath = CComBSTR(static_cast<LPCTSTR>(m_csOperationPath));
        
        if(NULL == m_ppMamROIControlHandlerManager) {
            CString csResult(_T("NULL pointer."));
            CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
            return FALSE;
        }
        
        // Get ROI UID of selected ROIs
        int nROICount = 0;
        BSTR* pUIDList = NULL;
        hr = m_ppMamROIControlHandlerManager->GetMeasurementList(
            bsOperationPath,
            &nROICount,
            &pUIDList,
            AllROI);
        
        if(FAILED(hr) || (NULL == pUIDList)) {   
            CString csResult(_T("GetMeasurementList failed."));
            CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
            return FALSE;
        }
        
        for(int l_ni = 0; l_ni < nROICount; l_ni++/*, pUIDList++*/) { 
            // Get selected ROI current position
            CurvePoint_t* pVectorList = NULL;
            int nVectorCount = 2;
            ROIObject_t roiObject;
            
            if(NULL == pUIDList) {
                CString l_cserrmsg(_T(""));
                l_cserrmsg.Format(_T("ROI UID is NULL for ROI # %d"));
                CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
            }
            
            hr = m_ppMamROIControlHandlerManager->GetMeasurementObjectInImageCoordination(bsOperationPath,
                *(pUIDList),
                &roiObject,
                &nVectorCount,
                &pVectorList);
            
            if(FAILED(hr)) {
                CString csResult(_T("GetMeasurementObjectInImageCoordination failed."));
                CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
                pUIDList -= l_ni; 
                ReleaseUIDList(pUIDList, nROICount);
                return FALSE;
            }
            
            if((0 == nVectorCount) || (NULL == pVectorList)) {
                CString csResult(_T("GetMeasurementObjectInImageCoordination CurvePoint Vector NULL."));
                CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
                pUIDList -= l_ni; 
                ReleaseUIDList(pUIDList, nROICount);
                return FALSE;
            }
            
            float l_x0 = static_cast<float>((pVectorList[0].x + pVectorList[1].x) *0.5);
            float l_y0 = static_cast<float>((pVectorList[0].y + pVectorList[1].y) *0.5);
            float l_r0 = static_cast<float>(fabs((pVectorList[1].y - l_y0) * 2));
            float l_r1 = static_cast<float>(fabs((pVectorList[1].x - l_x0) * 2));
            l_crParam.SetHeight(l_r0);
            l_crParam.SetWidth(l_r1);
            l_crParam.SetCenter_X(l_x0);
            l_crParam.SetCenter_Y(l_y0);
            m_vEllipseRoiparam.push_back(l_crParam);
            ReleaseVectorList(pVectorList);
            
            if((NULL != pUIDList) && (NULL != (pUIDList + 1))) {
                pUIDList++;
            }
        }
        
        pUIDList -= (nROICount);
        ReleaseUIDList(pUIDList, nROICount);
#else
        //Code commented not in use.............
        /*********************************************************/
        CEllipticalROIParams l_ellipseparam;
        int l_uidList_size = m_uid_point_list.GetSize();
        
        if(l_uidList_size > 0) {
            m_vEllipseRoiparam.clear();
        }
        
        for(int l_ni = 0; l_ni < l_uidList_size; l_ni++) {
            l_ellipseparam.SetHeight(m_uid_point_list.GetAt(l_ni).radius);
            l_ellipseparam.SetWidth(m_uid_point_list.GetAt(l_ni).width); //priyanka modified
            l_ellipseparam.SetCenter_X(m_uid_point_list.GetAt(l_ni).x);
            l_ellipseparam.SetCenter_Y(m_uid_point_list.GetAt(l_ni).y);
            m_vEllipseRoiparam.push_back(l_ellipseparam);
        }
        
#endif
        char destFldr[100] = {'\0'};
        strcpy(destFldr, "c:/MrMplus/Data/TiPrep");    
        if(ERROR_PATH_NOT_FOUND != CreateDirectoryA((LPCSTR)destFldr, NULL)) {
            if(cs.CompareNoCase(_T("AVG")) == 0) {
                CString l_avgfile(AVGROIFILE);
                l_bResult = m_RoiParamabj.WriteROIInfo(l_avgfile, f_csProtocolName, m_vEllipseRoiparam);//  [6/20/2013 Shweta] added for ellipse ROI
            } else if(cs.CompareNoCase(_T("MAP")) == 0) {
                CString l_mapfile(MAPROIFILE);
                l_bResult = m_RoiParamabj.WriteROIInfo(l_mapfile, f_csProtocolName, m_vEllipseRoiparam);//  [6/20/2013 Shweta] added for ellipse ROI
                //Dont check returned error ..
            } else {
            }
        }else{
            CTiPrepLogHelper::WriteToErrorLog
                (_T("SaveUpdateROIParamsInFile():c:/MrMplus/Data/TiPrep  path not found"), __LINE__);
            pUIDList -= l_ni; 
            ReleaseUIDList(pUIDList, nROICount);
            return FALSE;
        }
        return TRUE;
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_ROIHANDLER, __LINE__, __FILE__);
        return FALSE;
    }
}
/*************************************************************
* Method:       SetRoiPosition
* Description:  This function is used
* Returns:      BOOL
*************************************************************/
BOOL CROIHandler::SetRoiPosition()
{
    TIPREP_TRACE(CROIHandler::SetRoiPosition,MRTraceMarker3)
        try{
        CString l_csMoveCenterX[MAX_LINE_ROI];  
        CString l_csMoveCenterY[MAX_LINE_ROI];  
        CString l_csMoveRadius[MAX_LINE_ROI];   
        CString l_csMoveHeight[MAX_LINE_ROI];
        CString l_csMoveWidth[MAX_LINE_ROI];
        /*************************************************/
		//TRACE COMMENTED
#ifdef _UT_LOG_
        CString l_csLogMsg = (_T("Values set in strings in function SetRoiPosition\n These values are sent as input parameters to SaveMoveROI function"));
        CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker4, l_csLogMsg);
        l_csLogMsg.Empty();
#endif 

        for(int l_nRoi = 0; l_nRoi < MAX_LINE_ROI; l_nRoi++) {
            l_csMoveHeight[l_nRoi].Format(_T("%f"), m_vEllipseRoiparam.at(l_nRoi).GetHeight());
            l_csMoveWidth[l_nRoi].Format(_T("%f"), m_vEllipseRoiparam.at(l_nRoi).GetWidth());
            l_csMoveCenterX[l_nRoi].Format(_T("%f"), m_vEllipseRoiparam.at(l_nRoi).GetCenter_X());
            l_csMoveCenterY[l_nRoi].Format(_T("%f"), m_vEllipseRoiparam.at(l_nRoi).GetCenter_Y());
            //TRACE COMMENTED
#ifdef _UT_LOG_
            l_csLogMsg.Format(_T("ROI No.=%d\n String for CenterX=%s\n String for CenterY=%s\n String for Height=%s\n String for Width=%s\n "), 
                l_nRoi, l_csMoveCenterX[l_nRoi],l_csMoveCenterY[l_nRoi],l_csMoveHeight[l_nRoi], l_csMoveWidth[l_nRoi]);
            CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker4, l_csLogMsg);
#endif

        }
        
        BOOL l_bResult = SaveMoveROI(l_csMoveCenterX, l_csMoveCenterY, l_csMoveHeight, l_csMoveWidth); //  [6/20/2013 Shweta] modified
        /*************************************************/
        return l_bResult;
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_ROIHANDLER, __LINE__, __FILE__);
        return FALSE;
    }
}

/*************************************************************
* Method:       CalEllipseCordination
* Description:  This function calculates center and radius for ROI.
* Parameter:    float & x1
*               Out parameter used is to hold X co-ordinate for ROI drawing.
* Parameter:    float & y1
*               Out parameter used is to hold X co-ordinate for ROI drawing.
* Parameter:    float & x2
*               Out parameter used is to hold X co-ordinate for ROI drawing.
* Parameter:    float & y2
*               Out parameter used is to hold X co-ordinate for ROI drawing.
* Parameter:    float & f_fradius
*               Contains radius for ROI drawing.
* Parameter:    int f_nroi_no
* Returns:      void
*************************************************************/
int CROIHandler::CalEllipseCordination(float &x1,
                                       float &y1,
                                       float &x2,
                                       float &y2,
                                       float &f_fHeight,
                                       float &f_fWidth,
                                       int f_nroi_no)
{
    TIPREP_TRACE(CROIHandler::CalEllipseCordination,MRTraceMarker3);
    
    try {
        double f_x0 = 15.0;
        double f_y0 = 0.0;
        double l_fHeight = 5.0;
        double l_fWidth = 3.0;
        char* l_ptr = "";
        CEllipticalROIParams l_crRoiparam;
        
        switch(f_nroi_no) {
        case 0:
            l_crRoiparam = m_vEllipseRoiparam.at(0);
            f_x0 = l_crRoiparam.GetCenter_X();
            f_y0 = l_crRoiparam.GetCenter_Y();
            l_fHeight = l_crRoiparam.GetHeight();
            l_fWidth  = l_crRoiparam.GetWidth();
            break;
        case 1:
            l_crRoiparam = m_vEllipseRoiparam.at(1);
            f_x0 = l_crRoiparam.GetCenter_X();
            f_y0 = l_crRoiparam.GetCenter_Y();
            l_fHeight = l_crRoiparam.GetHeight();
            l_fWidth  = l_crRoiparam.GetWidth();
            break;
        case 2:
            l_crRoiparam = m_vEllipseRoiparam.at(2);
            f_x0 = l_crRoiparam.GetCenter_X();
            f_y0 = l_crRoiparam.GetCenter_Y();
            l_fHeight = l_crRoiparam.GetHeight();
            l_fWidth  = l_crRoiparam.GetWidth();
            break;
        case 3:
            l_crRoiparam = m_vEllipseRoiparam.at(3);
            f_x0 = l_crRoiparam.GetCenter_X();
            f_y0 = l_crRoiparam.GetCenter_Y();
            l_fHeight = l_crRoiparam.GetHeight();
            l_fWidth  = l_crRoiparam.GetWidth();
            break;
        case 4:
            l_crRoiparam = m_vEllipseRoiparam.at(4);
            f_x0 = l_crRoiparam.GetCenter_X();
            f_y0 = l_crRoiparam.GetCenter_Y();
            l_fHeight = l_crRoiparam.GetHeight();
            l_fWidth  = l_crRoiparam.GetWidth();
            break;
        default:
            return E_ERROR;
        }
        
        l_fHeight = fabs(l_fHeight);
        x1 = static_cast<float>(f_x0 - (l_fWidth * 0.5));
        y1 = static_cast<float>(f_y0 - (l_fHeight * 0.5));
        x2 = static_cast<float>(f_x0 + (l_fWidth * 0.5));
        y2 = static_cast<float>(f_y0 + (l_fHeight * 0.5));
        l_fHeight = l_crRoiparam.GetHeight();
        l_fWidth  = l_crRoiparam.GetWidth();
        //TRACE COMMENTED
#ifdef _UT_LOG_
        CString l_csLogMsg = 
            (_T("Values calculated for top left and bottom right points in function CalEllipseCordination\n"));
        CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker4, l_csLogMsg);
        l_csLogMsg.Empty();

        l_csLogMsg.Format(_T("ROI No.=%d\n TopLeftX=%0.6f\n TopLeftY=%0.6f\n BottomRightX=%0.6f\n BottomRightY=%0.6f\n "), 
            f_nroi_no, x1,y1,x2, y2);
        CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker4, l_csLogMsg);
#endif

    } catch(...) {
        CString csResult(_T("ROI Exception occurred in CalEllipseCordination()"));
        CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
        return E_ERR_EXCEPTION;
    }
    
    return E_NO_ERROR;
}

BOOL CROIHandler::SetProtocolNameForSetROIParams(CString &f_protocol_name)
{
	m_protocol_name =f_protocol_name;
	return TRUE;

}
CString CROIHandler::GetProtocolNameForSetROIParams()
{
	return m_protocol_name;
	
}
BOOL CROIHandler::RedrawROI(bool isreset, CString &protocol, CString &csmode)
{
    TIPREP_TRACE(CROIHandler::RedrawROI,MRTraceMarker3);
    
    try{
        if(FALSE == DeleteROI()){
            CString csResult(_T("DeleteROI failed"));
            CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
            return FALSE;
        }
	    if (!isreset){
//          Read from file
//          GetsaveROIParams(protocol,csmode,0,0); // already called from outside		
	    } else {
            if(FALSE == ResetROIPos(csmode)){ //hard code values
                CString csResult(_T("ResetROIPos failed"));
                CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
                return FALSE;
            }
            //This routine fills the ellipse param vector information
	    }
	    for (int i = 0; i < MAX_LINE_ROI; i++){
		    BOOL flag = FALSE;
	        flag = CTiPrepView::GetView()->GetCTiPrepDlgInstance()->GetROIDisplayCheck(i);
            if(FALSE == DrawROI(i,flag)){ //instead of true use visible status of checkbox
                CString csResult(_T("DrawROI failed [%d]"),i);
                CTiPrepLogHelper::WriteToErrorLog(csResult, __LINE__, __FILE__);
                return FALSE;
            }
	    }
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_ROIHANDLER, __LINE__, __FILE__);
        return FALSE;
    }
    return TRUE;
}

/*************************************************************
* Method:    	ResetROIPos
* Description: 	This function is used 
* Parameter: 	CString cs :
* Returns:   	BOOL
*************************************************************/
BOOL CROIHandler::ResetROIPos(CString cs)
{
    TIPREP_TRACE(CROIHandler::ResetROIPos,MRTraceMarker3);        
    try{        
        m_vEllipseRoiparam.clear();                                            
        
        float l_dx0 = -100.0f;
        float l_dy0 = -90.0f;
        float l_fHeight = 8.0f; 
        float l_fWidth = 8.0f;
        CEllipticalROIParams l_crParam; 
        if(/*m_vRoiparam.empty()*/m_vEllipseRoiparam.empty()) {//default position set
#ifdef _UT_LOG_
            CString l_csLogMsg = (_T("ROI parameters are not available in text files so default values are being set"));
            CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker4, l_csLogMsg);
            l_csLogMsg.Empty();
#endif
            
            if(cs.CompareNoCase(_T("AVG")) == 0) {//AVG case
                //             if(f_nImageWt >0 &&f_nImageHt >0){
                //                 l_dx0 = -(3.0*f_nImageWt/8.0);
                //                 l_dy0 = -(3.0*f_nImageHt/8.0);
                //             }else{
                //             }                
                for(int l_nRoi = 0; l_nRoi < NUMBEROFROI; l_nRoi++) {
                    
                    l_crParam.SetHeight(l_fHeight);
                    l_crParam.SetWidth(l_fWidth);
                    l_crParam.SetCenter_X(l_dx0 +(l_nRoi)*15);
                    l_crParam.SetCenter_Y(l_dy0 *(0 + 1));
                    m_vEllipseRoiparam.push_back(l_crParam);
                }
            } else {//MAP case
                for(int l_nRoi = 0; l_nRoi < MAX_LINE_ROI; l_nRoi++) {
                    if(0 == l_nRoi){
                        //                     l_dx0 = -(3.0*f_nImageWt/8.0);
                        //                     l_dy0 = -(3.0*f_nImageHt/8.0);
                        l_fHeight = 35.0f;
                        l_fWidth = 35.0f;
                    }else if(1 == l_nRoi){
                        //                     l_dx0 = -(3.0*f_nImageWt/8.0);
                        //                     l_dy0 = -(3.0*f_nImageHt/8.0);
                        l_fHeight = 55.0f;
                        l_fWidth = 55.0f;
                    }else{
                        //                     if(f_nImageWt >0 &&f_nImageHt >0){
                        //                         l_dx0 = -(3.0*f_nImageWt/8.0);
                        //                         l_dy0 = -(3.0*f_nImageHt/8.0);
                        //                     }else{
                        //                    }
                        l_fHeight = 8.0f;
                        l_fWidth = 8.0f;
                    }
                    if(l_fHeight < 0) {
                        l_fHeight = -l_fHeight;
                    }
                    
                    if(l_fWidth < 0) {
                        l_fWidth = -l_fWidth;
                    }
                    l_crParam.SetHeight(l_fHeight);
                    l_crParam.SetWidth(l_fWidth);
                    if(0 == l_nRoi || 1 == l_nRoi){
                        l_crParam.SetCenter_X(l_dx0);
                    }else{
                        l_crParam.SetCenter_X(l_dx0 +(l_nRoi)*15);
                    }
                    l_crParam.SetCenter_Y(l_dy0 *(0 + 1));
                    m_vEllipseRoiparam.push_back(l_crParam);
                }
            }
        }
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_ROIHANDLER, __LINE__, __FILE__);
        return FALSE;
    }
    return TRUE;
}

BOOL CROIHandler::SetROIPos(CString f_Roi_cenX[], CString f_Roi_cenY[], CString f_Roi_height[], CString f_Roi_Width[])
{
    TIPREP_TRACE(CROIHandler::SetROIPos,MRTraceMarker3)        
    try{        
        m_vEllipseRoiparam.clear();    //CleanUP                                        
        
        CEllipticalROIParams l_crParam; 
        if(m_vEllipseRoiparam.empty()) {            
            for(int l_nRoi = 0; l_nRoi < NUMBEROFROI; l_nRoi++) {
                
                float l_x_destination = 0.0f;
                float l_y_destination = 0.0f;
                float l_x_displacement = 0.0f;
                float l_y_displacement = 0.0f;
                float l_rad_destination = 0.0f;
                float l_fHeight_destination = 0.0f;
                float l_fWidth_destination = 0.0f;
                double l_temp_cenX = 0.0;
                double l_temp_cenY = 0.0;
                double l_temp_radius = 0.0;
                double l_temp_width = 0.0;
                TCHAR* pTchar = NULL;
                
                l_temp_cenX = _tcstod(f_Roi_cenX[l_nRoi] , &pTchar);
                pTchar = NULL;
                l_temp_cenY = _tcstod(f_Roi_cenY[l_nRoi] , &pTchar);
                pTchar = NULL;
                l_temp_radius = _tcstod(f_Roi_height[l_nRoi] , &pTchar);//it contains height for ellipse
                pTchar = NULL;
                l_temp_width = _tcstod(f_Roi_Width[l_nRoi], &pTchar); //it contains width for ellipse
                pTchar = NULL;
                l_x_destination = static_cast<float>(l_temp_cenX);
                l_y_destination = static_cast<float>(l_temp_cenY);
                l_fHeight_destination = static_cast<float>(fabs(l_temp_radius));
                l_fWidth_destination = static_cast<float>(fabs(l_temp_width));

                l_crParam.SetHeight(l_fHeight_destination);
                l_crParam.SetWidth(l_fWidth_destination);
                l_crParam.SetCenter_X(l_x_destination);
                l_crParam.SetCenter_Y(l_y_destination);
                m_vEllipseRoiparam.push_back(l_crParam);
            }
        }
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_ROIHANDLER, __LINE__, __FILE__);
        return FALSE;
    }
    return TRUE;
}
///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////
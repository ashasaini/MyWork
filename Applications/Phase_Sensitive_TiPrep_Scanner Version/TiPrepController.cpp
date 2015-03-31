// TiPrepController.cpp: implementation of the CTiPrepController class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#include "stdafx.h"
#include <afxsock.h>
#include "ReconRawCheck.h"
#include "TiPrepController.h"
#include "TiPrepView.h"
#include "TiPrepLogHelper.h"
#include "TiPrepTrace.h"
#include "IniFileReader.h"
#include "CommonDefinitions.h"
#include "TiProlimUpdater.h"
#include "vfcDBMgr.h"
#include "tami/libWNP/WNP_types.h"
#include <tami/vfStudy/vf_appcodes.h>  
#include <tami/libmathServer/msGetConfig.h>
#include "NodeCopy.h"
#include "PDataParser/INIReader.h"


CTiPrepController* CTiPrepController::m_TiController = NULL;
//extern CTiPrepDlg * m_prep_dlg;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTiPrepController::CTiPrepController():
m_pAddOffset(NULL), m_pPolarityCorr(NULL),
m_cReconObj(NULL), m_phasecorr(NULL),m_nDataForm(2),
m_nSingleButtonPhaseCorr(0),m_nProtocolnoPC(0),
m_nProcessNoPC(0),m_csStudyFileName(_T("")),m_tiupdater(NULL),m_ApplyTiDlg(NULL)
{
    //
}

/*************************************************************
* Method:    	CTiPrepController
* Description: 	This is copy constructor 
* Parameter: 	CTiPrepController & f_controller :
* Returns:   	
*************************************************************/
CTiPrepController::CTiPrepController(CTiPrepController & f_controller)
{
   /* TIPREP_TRACE(CTiPrepController::CTiPrepController);*/

    
    if(NULL != f_controller.m_pAddOffset) {
        if(m_pAddOffset) {
            delete m_pAddOffset;
            m_pAddOffset = NULL;
        }
        
        m_pAddOffset = new CAddOffset(*(f_controller.m_pAddOffset));
    }
    
    if(NULL != f_controller.m_pPolarityCorr) {
        if(m_pPolarityCorr) {
            delete m_pPolarityCorr;
            m_pPolarityCorr = NULL;
        }
        
        m_pPolarityCorr = new CPolarityCorrection(*(f_controller.m_pPolarityCorr));
    }  

    if(NULL != f_controller.m_cReconObj) {
        if(m_cReconObj) {
            delete m_cReconObj;
            m_cReconObj = NULL;
        }
        
        m_cReconObj = new CReconstruction(*(f_controller.m_cReconObj));
    }  

    if(NULL != f_controller.m_phasecorr) {
        if(m_phasecorr) {
            delete m_phasecorr;
            m_phasecorr = NULL;
        }
        
        m_phasecorr = new CPhaseCorrection(*(f_controller.m_phasecorr));
    } 

     if(NULL != f_controller.m_ApplyTiDlg) {
        if(m_ApplyTiDlg) {
            delete m_ApplyTiDlg;
            m_ApplyTiDlg = NULL;
        }
        
        m_ApplyTiDlg = new CApplyTi(*(f_controller.m_ApplyTiDlg));
        m_ApplyTiDlg->Create(IDD_DIALOG_APPLY_TI, AfxGetMainWnd());
        m_ApplyTiDlg->ShowWindow(SW_HIDE);
    } 
    
    m_nDataForm = f_controller.m_nDataForm;
    m_nSingleButtonPhaseCorr = f_controller.m_nSingleButtonPhaseCorr;
    m_nProtocolnoPC = f_controller.m_nProtocolnoPC;
    m_nProcessNoPC = f_controller.m_nProcessNoPC;
    m_csStudyFileName = f_controller.m_csStudyFileName;

}

/*************************************************************
* Method:    	operator=
* Description: 	This function is used 
* Parameter: 	CTiPrepView & f_controller :
* Returns:   	CTiPrepView&
*************************************************************/
CTiPrepController& CTiPrepController::operator = (CTiPrepController& f_controller)
{
    TIPREP_TRACE(CTiPrepController::operatoroverload,MRTraceMarker9);
    
    if(this == &f_controller) {
        return(*this);
    }
    
    CopyObject(f_controller);

    return (*this);
}

/*************************************************************
* Method:    	CopyObject
* Description: 	This function is used to copy data
* Parameter: 	CTiPrepController & f_controller :
* Returns:   	int
*************************************************************/
int CTiPrepController::CopyObject (CTiPrepController& f_controller)
{
    TIPREP_TRACE(CTiPrepController::CopyObject operatoroverload,MRTraceMarker9);

    if(NULL != f_controller.m_pAddOffset) {
        if(m_pAddOffset) {
            delete m_pAddOffset;
            m_pAddOffset = NULL;
        }
        
        m_pAddOffset = new CAddOffset(*(f_controller.m_pAddOffset));
    }
    
    if(NULL != f_controller.m_pPolarityCorr) {
        if(m_pPolarityCorr) {
            delete m_pPolarityCorr;
            m_pPolarityCorr = NULL;
        }
        
        m_pPolarityCorr = new CPolarityCorrection(*(f_controller.m_pPolarityCorr));
    }  
    
    if(NULL != f_controller.m_cReconObj) {
        if(m_cReconObj) {
            delete m_cReconObj;
            m_cReconObj = NULL;
        }
        
        m_cReconObj = new CReconstruction(*(f_controller.m_cReconObj));
    }  
    
    if(NULL != f_controller.m_phasecorr) {
        if(m_phasecorr) {
            delete m_phasecorr;
            m_phasecorr = NULL;
        }
        
        m_phasecorr = new CPhaseCorrection(*(f_controller.m_phasecorr));
    }
    
    if(NULL != f_controller.m_ApplyTiDlg) {
        if(m_ApplyTiDlg) {
            delete m_ApplyTiDlg;
            m_ApplyTiDlg = NULL;
        }
        
        m_ApplyTiDlg = new CApplyTi(*(f_controller.m_ApplyTiDlg));
        m_ApplyTiDlg->Create(IDD_DIALOG_APPLY_TI, AfxGetMainWnd());
        m_ApplyTiDlg->ShowWindow(SW_HIDE);
    }
       
    m_nDataForm = f_controller.m_nDataForm;
    m_nSingleButtonPhaseCorr = f_controller.m_nSingleButtonPhaseCorr;
    m_nProtocolnoPC = f_controller.m_nProtocolnoPC;
    m_nProcessNoPC = f_controller.m_nProcessNoPC;
    m_csStudyFileName = f_controller.m_csStudyFileName;

    return TRUE;
}

/*************************************************************
* Method:    	~CTiPrepController
* Description: 	This function is used 
* Returns:   	
*************************************************************/
CTiPrepController::~CTiPrepController()
{
    if(NULL != m_TiController) {
        delete m_TiController;
        m_TiController = NULL;
    }
    
    if(m_pAddOffset != NULL) {
        delete m_pAddOffset;
        m_pAddOffset = NULL;
    }
    
    if(m_pPolarityCorr != NULL) {
        delete m_pPolarityCorr;
        m_pPolarityCorr = NULL;
    }

    if(NULL != m_ApplyTiDlg) {
        if(m_ApplyTiDlg->GetSafeHwnd()) {
            m_ApplyTiDlg->DestroyWindow();
        }
        
        delete m_ApplyTiDlg;
        m_ApplyTiDlg = NULL;
    }
}

/*************************************************************
* Method:       GetInstance
* Description:  This function is used to create instance of
*               CTiPrepController class.
* Returns:      CTiPrepController*
*               Instance of CTiPrepControllerclass.
*************************************************************/
CTiPrepController* CTiPrepController::GetInstance()
{
    /*TIPREP_TRACE(CTiPrepController::GetInstance);*/
    if(NULL == m_TiController) {
        m_TiController = new CTiPrepController();
        
        if(NULL == m_TiController) {
            CString l_cserr = _T("Memory allocation failed for CTiPrepController.");
            CTiPrepLogHelper::WriteToErrorLog(l_cserr, __LINE__,__FILE__);
        }
    }
    
    return m_TiController;
}

/*************************************************************
* Method:       DeleteInstance
* Description:  This function is used to release the allocated
*               memory of the instance of CTiPrepController class.
* Returns:      void
*************************************************************/
void CTiPrepController::DeleteInstance()
{
    /*TIPREP_TRACE(CTiPrepController::DeleteInstance);*/
    if(m_TiController) {
        delete m_TiController;
        m_TiController = NULL;
    }
}

/*************************************************************
* Method:       SetImageLOIDForPolarityCorrection
* Description:  This function is used
* Parameter:    long f_no_of_images_selected
* Parameter:    vector <CString> f_image_loid
* Returns:      int
*************************************************************/
int CTiPrepController::SetImageLOIDForPolarityCorrection(long f_no_of_images_selected,
                                                         /*In*/vector <CString> & f_image_loid)
{
    CString l_selected_protocol_name(_T(""));
    CString l_selected_series_no(_T(""));
    
    try{
        if(NULL == m_pPolarityCorr) {
            m_pPolarityCorr = new CPolarityCorrection();
            
            if(NULL == m_pPolarityCorr) {
                CString l_csErrMsg(_T("Unable to allocate memory."));
                CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__,__FILE__);
                return E_ERROR;
            }
        }
        
        CString l_image_data_type(_T(""));
        if (f_image_loid.size() <= 0)
        {
            return I_PROPER_IMG;
        }
        int l_tag_reading_status = CTiPrepView::GetView()->vfcDataGetDataForm(l_image_data_type, f_image_loid); 
        
        if(E_NO_ERROR != l_tag_reading_status) {
            CString l_error_msg(_T("Unable to read tag."));
            CTiPrepLogHelper::WriteToErrorLog(l_error_msg, __LINE__, __FILE__);
            return l_tag_reading_status;
        }
        
        if(l_image_data_type.CompareNoCase(_T("Magnitude")) == 0) {
            int l_nStatus = m_pPolarityCorr->SetImageLOIDForPolarityCorrection(
                f_no_of_images_selected, f_image_loid);
            
            if(E_NO_ERROR != l_nStatus){
                CString l_csmsg
                    (_T("SetImageLOIDForPolarityCorrection(): SetImageLOIDForPolarityCorrection returned error"));
                CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
                return(l_nStatus);
            }
        } else {
            CString l_error_msg(_T("Please select Magnitude Image"));
            //int l_nmsgCode = I_MAG_IMG; 
            return I_MAG_IMG;
        }
        
//        if(NULL != m_prep_dlg)     {//vilas_31Oct..check moved to view
        if(0 < f_image_loid.size()) {
            CTiPrepView::GetView()->DisplaySeriesNumber(f_image_loid.at(0), POLARITY_CORR_DLG);
        } else { 
            return I_PROPER_IMG;
        }
    }catch(...){
        CString l_csmsg(_T("SetImageLOIDForPolarityCorrection(): Exception Occurred."));
        CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
        return E_ERROR;
    }
    return E_NO_ERROR;
}
/*************************************************************
* Method:       PolarityCorrectionCalculation
* Description:  This function is used
* Returns:      int
*************************************************************/
int CTiPrepController::PolarityCorrectionCalculation()
{
    int l_roi_checked = 0;
    byte* l_image_roi_data[MAX_LINE_ROI] = {NULL, NULL, NULL, NULL, NULL};
    memset(l_image_roi_data, 0x0, (MAX_LINE_ROI * sizeof(byte*)));
    int l_roi_height = 0;
    int l_roi_width = 0;
    
    try{
        if(NULL != m_pPolarityCorr) {
            int l_fun_status = GetROIStatusForINROI(l_roi_checked);
            
            if(E_NO_ERROR != l_fun_status) { 
                return l_fun_status;
            }
            
            int l_status = CTiPrepView::GetView()->GetROIInstance()->GetROIBufferInfo
                (l_image_roi_data, &l_roi_height, &l_roi_width);
            
            if(!l_status) {
                CString l_error_msg(_T("Unable to get image map."));
                CTiPrepLogHelper::WriteToErrorLog(l_error_msg,__LINE__,__FILE__);
                return E_ROI_MAP;
            }
            
            CLocalDBMgr localDB; 
            int l_fun_execution_status = m_pPolarityCorr->PolarityCorrectionCalculation
                (l_image_roi_data, l_roi_checked, &localDB);
            
            if(E_NO_ERROR == l_fun_execution_status) {
                CTiPrepView::GetView()->RefreshIsel();
            } else { 
                CTiPrepLogHelper::WriteToErrorLog
                    (_T("PolarityCorrectionCalculation():l_fun_execution_status is ERROR"),__LINE__,__FILE__);
                return l_fun_execution_status;
            }
        } else { 
            CTiPrepLogHelper::WriteToErrorLog
                (_T("PolarityCorrectionCalculation():m_pPolarityCorr is NULL"),__LINE__,__FILE__);
            return E_ERROR;
        }
    }catch(...){
        CTiPrepLogHelper::WriteToErrorLog
            (_T("PolarityCorrectionCalculation():Exception Occurred"),__LINE__,__FILE__);
        return E_ERROR;
    }
    return E_NO_ERROR;
}

/*************************************************************
* Method:       GetROIStatusForINROI
* Description:  This function is used
* Returns:      int
*************************************************************/
int CTiPrepController::GetROIStatusForINROI(int &f_roi_status_for_inroi)
{
    CTiPrepDlg *l_prep_dlg;
    try{
        l_prep_dlg = CTiPrepView::GetView()->GetCTiPrepDlgInstance();
        
        if(NULL == l_prep_dlg) {
            CTiPrepLogHelper::WriteToErrorLog(_T("TiPrep Dlg object is null"), __LINE__, __FILE__);
            return E_ERROR;
        }
        
        //vilas_31Oct..check logic applied.
        if(BST_CHECKED == l_prep_dlg->m_checkDispROI1Data.GetCheck()) {
            f_roi_status_for_inroi = 0;
        } else if(BST_CHECKED == l_prep_dlg->m_checkDispROI2Data.GetCheck()) {
            f_roi_status_for_inroi = 1;
        } else if (BST_CHECKED == l_prep_dlg->m_checkDispROI3Data.GetCheck()) { 
            f_roi_status_for_inroi = 2;
        } else { 
            f_roi_status_for_inroi = 0;
            return E_NO_ROI_SEL_INROI;
        }
    }catch(...){
        CTiPrepLogHelper::WriteToErrorLog
            (_T("GetROIStatusForINROI():Exception Occurred"),__LINE__,__FILE__);
        return E_ERROR;
    }
    return E_NO_ERROR;
}

/*************************************************************
* Method:       AddOffsetCalculation
* Description:  This function is used
* Returns:      int
*************************************************************/
int CTiPrepController::AddOffsetCalculation(CString & f_offset_value, 
                                            int f_add_offset_check_box_status,
                                            int *f_actual_expected_offset_value)
{
    int l_roi_checked = 0;
    byte* l_image_roi_data[MAX_LINE_ROI] = {NULL, NULL, NULL, NULL, NULL};
    int l_roi_height = 0;
    int l_roi_width = 0;
    
    try{
        if(NULL != m_pAddOffset) {
            CIniFileReader l_ini_reader;
            int l_proc_roi_option = 0;
            int l_ini_reader_status = l_ini_reader.GetProcOption(l_proc_roi_option);
            if(E_NO_ERROR != l_ini_reader_status) {
                CTiPrepLogHelper::WriteToErrorLog(_T("ini file not read properly"), __LINE__, __FILE__);
                return l_ini_reader_status;
            }
            int l_fun_status = GetROIStatusForINROI(l_roi_checked);
            if(l_proc_roi_option!= INROI || f_add_offset_check_box_status==BST_UNCHECKED) {			
                if(E_NO_ERROR != l_fun_status) { 
                    CTiPrepLogHelper::WriteToErrorLog(_T("GetROIStatusForINROI failed"), __LINE__, __FILE__);
                    return l_fun_status; 
                }
            }
            
            int l_status = CTiPrepView::GetView()->GetROIInstance()->GetROIBufferInfo(
                l_image_roi_data, &l_roi_height, &l_roi_width);
            
            if(!l_status) {
                return E_ROI_MAP;
            }
            
            if(NULL == l_image_roi_data) {
                CString l_error_display(_T("Map is NULL"));
                CTiPrepLogHelper::WriteToErrorLog(l_error_display, __LINE__, __FILE__);
                return E_ERROR_NULL_FUNCPARAMS;
            }
            
            
            
            if(ALLIMG == l_proc_roi_option) {
                memset(l_image_roi_data[l_roi_checked], 1, sizeof(byte)*l_roi_height * l_roi_width);
            }
            
            int l_nstatus = m_pAddOffset->GetOffsetData(f_offset_value,
                f_add_offset_check_box_status,
                l_image_roi_data[l_roi_checked],
                l_roi_checked,f_actual_expected_offset_value);
            
            for(int l_no_of_roi = 0 ; l_no_of_roi < MAX_LINE_ROI; l_no_of_roi++) {
                CoTaskMemFree(l_image_roi_data[l_no_of_roi]);
                l_image_roi_data[l_no_of_roi] = NULL;
            }
            
            if(E_NO_ERROR == l_nstatus) {
                CTiPrepView::GetView()->RefreshIsel();
            } else {
                return l_nstatus; 
            }
        } else { 
            CTiPrepLogHelper::WriteToErrorLog
                (_T("AddOffsetCalculation() : m_pAddOffset is NULL"), __LINE__, __FILE__);
            return E_ERROR;
        }
    }catch(...){
        CTiPrepLogHelper::WriteToErrorLog
            (_T("AddOffsetCalculation() : Exception Occurred"), __LINE__, __FILE__);
        return E_ERROR;
    }
    
    return E_NO_ERROR;
}

/*************************************************************
* Method:       SetImageLOIDForAddOffset
* Description:  This function is used
* Parameter:    long f_no_of_images_selected
* Parameter:    vector <CString> f_image_loid
* Returns:      int
*************************************************************/
int CTiPrepController::SetImageLOIDForAddOffset(long f_no_of_images_selected,
                                                /*In*/vector <CString> & f_image_loid)
                                                
{
    try{
        if(NULL == m_pAddOffset) {
            m_pAddOffset = new CAddOffset();
        
            if(NULL == m_pAddOffset) { 
                CString l_csErrMsg(_T("SetImageLOIDForAddOffset():m_pAddOffset is NULL."));
                CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                return E_ERROR;
            }
        }
    
        m_pAddOffset->SetImageLOIDForAddOffset(f_no_of_images_selected, f_image_loid);
    
        if(0 < f_image_loid.size()) {
            CTiPrepView::GetView()->DisplaySeriesNumber(f_image_loid.at(0), ADD_OFFSET_DLG);
        } else {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("SetImageLOIDForAddOffset(): loid size <0 error"), __LINE__, __FILE__);
            return E_ERROR; 
        }
    }catch(...){
        CTiPrepLogHelper::WriteToErrorLog
            (_T("SetImageLOIDForAddOffset(): Exception Occurred"), __LINE__, __FILE__);
        return E_ERROR;
    }
    return E_NO_ERROR;
}

/*************************************************************
* Method:    	GetSelectedImageLOIDForAddOffset
* Description: 	This function is used 
* Parameter: 	vector <CString> & f_image_loid :
* Returns:   	int
*************************************************************/
int CTiPrepController:: GetSelectedImageLOIDForAddOffset(vector <CString> &f_image_loid)
{
    try{
        if(NULL == m_pAddOffset) { 
            CString l_csErrMsg(_T("GetSelectedImageLOIDForAddOffset():m_pAddOffset is NULL."));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return E_ERROR;
        }
        
        if(E_NO_ERROR != m_pAddOffset->GetSelectedImageLOIDForAddOffset(f_image_loid)){
            CString l_csErrMsg(_T("GetSelectedImageLOIDForAddOffset():GetSelectedImageLOIDForAddOffset Failed."));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return E_ERROR;
        }
    }catch(...){
        CString l_csErrMsg(_T("GetSelectedImageLOIDForAddOffset():Eception Occurred."));
        CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
        return E_ERROR;  
    }
    return E_NO_ERROR;
}

/*************************************************************
* Method:    	GetSelectedProtocolForAddOffset
* Description: 	This function is used 
* Parameter: 	CString & f_csProtName :
* Returns:   	int
*************************************************************/
int CTiPrepController::GetSelectedProtocolForAddOffset(CString & f_csProtName)
{
    vector<CString> l_csImgInfo;
    
    if(NULL == m_pAddOffset) { 
        CTiPrepLogHelper::WriteToErrorLog
            (_T("GetSelectedProtocolForAddOffset(): m_pAddOffset or m_pdb_mgr is NULL"), __LINE__, __FILE__);
        return E_ERROR;
    }
    
    m_pAddOffset->GetSelectedImageLOIDForAddOffset(l_csImgInfo);
    CLocalDBMgr localdb;
    int status = 0;
    if(0 < l_csImgInfo.size()) {
        CString l_csProtocolName = _T("");
        CString l_csProcessName = _T("");
		CString f_csStudyName;
        CString f_csCompletepath;
        status = localdb.GetProtocolNameFromImageLevelDB(l_csImgInfo.at(0), f_csProtName,f_csStudyName,f_csCompletepath);
        if (status != E_NO_ERROR)
        {
            CTiPrepLogHelper::WriteToErrorLog(_T("GetSelectedProtocolForAddOffset() : GetProtocolNameFromImageLevelDB() return fail status"),__LINE__,__FILE__);
            return status;
        }
    } else { 
        CTiPrepLogHelper::WriteToErrorLog
            (_T("GetSelectedProtocolForAddOffset(): l_csImgInfo.size()  is Zero"), __LINE__, __FILE__);
        return E_ERROR;
    }
    
    return E_NO_ERROR;
}

/*************************************************************
* Method:       Close_AddOffset
* Description:  This function is used
* Returns:      void
*************************************************************/
void CTiPrepController:: Close_AddOffset()
{
    if(NULL != m_pAddOffset) {
        delete m_pAddOffset;
        m_pAddOffset = NULL;
    }
}

/*************************************************************
* Method:       doRawChkAndRecon
* Description:  This function is used
* Parameter:    int f_nDataForm
* Returns:      int
*************************************************************/
int CTiPrepController::doRawChkAndRecon(int f_nDataForm)
{
    try{
    CString l_csStudyFileName;
    int l_nStatus = E_NO_ERROR;
    int l_nProtocolNum = 0;
    l_nStatus = m_cReconObj->getProtocolNumber(l_nProtocolNum, l_csStudyFileName);
    if (E_NO_ERROR != l_nStatus)
    {
        CTiPrepLogHelper::WriteToErrorLog(_T("Failed to get Study file name."), ERROR_LOGONLY);
        return E_RECON_FAIL;
    }
    l_nStatus = rawCheckRecon(l_csStudyFileName,l_nProtocolNum);
    if (E_NO_ERROR != l_nStatus)//AS modified status check
    {
        if(l_nStatus == E_RAW_CHECK_FAIL){
            CTiPrepLogHelper::WriteToErrorLog(_T("Raw Check failed"), ERROR_LOGONLY);
            return E_RAW_CHECK_FAIL;
        }else if(l_nStatus == E_NO_RAW){
            CTiPrepLogHelper::WriteToErrorLog(_T("Raw data is not present for selected protocol"), ERROR_LOGONLY);
            return E_NO_RAW;
        }
    }
    CIRSListner m_IrsComm;
    BOOL l_nstatus = m_IrsComm.InitializeIRS();   
    if (TRUE != l_nstatus){
        CString l_cserrmsg = (_T("Failed to Initialize IRS"));
        CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);  
        return E_ERROR;
    }
    Sleep(2000);
    l_nStatus = m_cReconObj->performReconstruction(l_csStudyFileName, l_nProtocolNum, f_nDataForm);
    
    if(E_NO_ERROR != l_nStatus) {
        m_IrsComm.ForceExitIRS();
        return E_RECON_FAIL;
    }else{
        m_IrsComm.WaitForIRSDone();
    }
    }catch(...){
        CString lcsmsg(_T("Exception occurred in doRawChkAndRecon"));
        CTiPrepLogHelper::WriteToErrorLog(lcsmsg, __LINE__, __FILE__);
        E_ERR_EXCEPTION;

    }
    return E_NO_ERROR;
}

int CTiPrepController::getReconstuctionImgs(vector<CString> &f_csloids/*, int numImgs*/)
{
    if (NULL == m_cReconObj)
    {
        m_cReconObj = new CReconstruction();
    }
    CString l_protocol_name, l_intermediate_string, l_study_file_name(_T("")), l_csProcessName;
    //int l_process_no = 0;
    int l_protocol_no = 0;
    //int l_nDataType = 0;
    //int l_tag_reading_status = 0;
    CLocalDBMgr l_localdb ;
    //vector<CString> l_img_loid;
    if (f_csloids.size() <= 0)
    {
        CTiPrepLogHelper::WriteToErrorLog(_T("getReconstuctionImgs sz is l 0"),__LINE__,__FILE__);
        return I_PROPER_IMG;
    }
    
    CString l_csSeriesLOID(_T(""));
    int l_nStatus = l_localdb.GetParentLOID(f_csloids.at(0), LOID_TYPE_IMAGE, l_csSeriesLOID);
    
    if(E_NO_ERROR != l_nStatus) {
        CTiPrepLogHelper::WriteToErrorLog(_T("Unable to get file path from LocalDB."), ERROR_LOGONLY);
        return(l_nStatus);
    }
    
    CLocalDBMgr localdb;
    l_nStatus = localdb.GetProtocolNameFromSeriesLevelDB(l_csSeriesLOID, l_protocol_name, l_csProcessName);
    
    if(E_NO_ERROR != l_nStatus) {
        CTiPrepLogHelper::WriteToErrorLog(_T("Unable to get protocol."), ERROR_LOGONLY);
        return(l_nStatus);
    }
    
    l_intermediate_string.Format(_T("%s"), l_csProcessName);
    int l_count_special_char = l_intermediate_string.Find(_T(':'));
    l_study_file_name = l_intermediate_string.Left(l_count_special_char);
    int l_total_length = l_csProcessName.GetLength();
    l_count_special_char = l_csProcessName.ReverseFind(_T(':'));
    l_intermediate_string = l_csProcessName.Right(l_total_length - (l_count_special_char + 2));
    l_count_special_char = l_intermediate_string.ReverseFind(_T('/'));
    l_intermediate_string = l_intermediate_string.Left(l_count_special_char);
    l_count_special_char = l_intermediate_string.Find(_T('.'));
    l_total_length = l_intermediate_string.GetLength();
    l_intermediate_string = l_intermediate_string.Right(l_total_length - (l_count_special_char + 1));
    char l_intermediate_char[10];
    sprintf(l_intermediate_char, "%S", l_intermediate_string);
    l_protocol_no = atoi(l_intermediate_char);
    m_cReconObj->setProtocolNumber(l_protocol_no, l_study_file_name);
    CString l_csseries_number(_T(""));
    long series_no = 0;
    l_localdb.GetSeriesNumberFromDB(l_csSeriesLOID, &series_no);
    series_no = series_no / 1000;
    series_no = series_no * 1000;
    l_csseries_number.Format(_T("%d"), series_no);
    //m_prep_dlg->DisplayReconSelectedSeries(l_csseries_number);//vilas_31Oct..
    (CTiPrepView::GetView()->GetCTiPrepDlgInstance())->DisplayReconSelectedSeries(l_csseries_number);
    return E_NO_ERROR;
}

/*************************************************************
* Method:       SetPCLOIDs
* Description:  This function is used
* Parameter:    vector<CString> & f_csloids
* Parameter:    int numImgs
* Returns:      void
*************************************************************/
void CTiPrepController::SetPCLOIDs(vector<CString> &f_csloids, int numImgs, int manual_kind)
{
    if(!m_phasecorr) {
        m_phasecorr = new CPhaseCorrection();
    } 
    
    if(!m_phasecorr) {
        CString l_csmsg(_T("SetPCLOIDs() Memory allocation failed"));
        CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
        return ;
    }
    
    if(manual_kind == MAGNITUDE) {
        m_phasecorr->setImgInfo(numImgs, f_csloids);
    } else if(manual_kind == REAL) {
        m_phasecorr->SetRealSeriesLOID(/*numImgs,*/ f_csloids);
    } else if(manual_kind == IMGN) {
        m_phasecorr->SetImgnSeriesLOID(/*numImgs,*/ f_csloids);
    }
}

#define MEMFREE(ptr) if (ptr){ \
    free(ptr); \
    ptr = NULL;\
}
#define DEL_ARR_PTR(ptr) if (ptr){ \
    delete [] (ptr); \
    ptr = NULL;\
}
/*************************************************************
* Method:    	SeriesPhaseCorrection
* Description: 	This function is used 
* Returns:   	int
*************************************************************/
int CTiPrepController::SeriesPhaseCorrection()
{
    TIPREP_TRACE(CTiPrepView::SeriesPhaseCorrection,MRTraceMarker3);
    void *reImgBuff = NULL;
    void *imgnImgBuff = NULL;
    void *reImgBuff2 = NULL;
    void *imgnImgBuff2 = NULL;
    void *ImageArea_mag = NULL;
    void *ImageArea_mag2 = NULL;
    int status = 0;
    void *outImg = NULL;
    unsigned short l_nbitsallocated = 16;  //get this from Local DB
    unsigned short l_nwidth = 0;
    unsigned short l_nheight = 0;
    byte *roibuff = NULL;
    int l_ndataform = m_nDataForm;
    bool l_binROI = false; //read from ini
    bool l_bdoROIavg = false; //read from ini
    double roi_avgval_re = 0; //calculate //not done yet
    double roi_avgval_im = 0; //calculate //not done yet
    CString l_csimgLOID(_T(""));
    int l_nselectedIdx = 0;
    int imgsz = 0; // from database
    int l_nNumImgs = 0; // from selected LOIDs
    CLocalDBMgr localdb ;
    vector<CString> l_csloid;
    byte* l_image_roi_data[MAX_LINE_ROI] = {NULL, NULL, NULL, NULL, NULL};
    double ** roi_avg_ori = NULL;
    double ** imgn_roi_avg_ori = NULL;
    float l_img_scale_factor =0;
    
    try {
        if(!m_phasecorr) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("Phase correction class object is null"), __LINE__, __FILE__);
            return E_PHASE_CORRECTION;
        }
        
        l_nNumImgs = m_phasecorr->GetNumSelectedImgs();
        m_phasecorr->getSelImgLOID(l_csloid);
        vector<CString> l_csReloid;
        vector<CString> l_csImloid;
        vector<CString> l_csMagloid;
        m_phasecorr->getRealImgLOID(l_csReloid);
        m_phasecorr->getImgnImgLOID(l_csImloid);
        m_phasecorr->getSelImgLOID(l_csMagloid);
        
        if(l_csReloid.size() < 0) {
            CTiPrepLogHelper::WriteToErrorLog(_T("Please Select a Real Series"), __LINE__, __FILE__);
            return E_VALIDATION_FAILED;
        }
        
        if(l_csImloid.size() < 0) {
            CTiPrepLogHelper::WriteToErrorLog(_T("Please Select an Imaginary Series"), __LINE__, __FILE__);
            return E_VALIDATION_FAILED;
        }
        if(l_csMagloid.size() < 0) {
            CTiPrepLogHelper::WriteToErrorLog(_T("Please Select an Magnitude Series"), __LINE__, __FILE__);
            return E_VALIDATION_FAILED;
        }
        
        CIniFileReader l_ini_reader;
        //int l_in_roi_option = 2;
        int l_proc_roi_option = 0;
        int l_nphase_option = 0;
        int l_ini_reader_status = l_ini_reader.GetProcOption(l_proc_roi_option);
        
        if(E_NO_ERROR != l_ini_reader_status) {
            CTiPrepLogHelper::WriteToErrorLog(_T("ini file not read properly"), __LINE__, __FILE__);
            return E_PHASE_CORRECTION;
        }
        
        l_ini_reader_status = l_ini_reader.GetPhaseOption(l_nphase_option);
        
        if(E_NO_ERROR != l_ini_reader_status) {
            CTiPrepLogHelper::WriteToErrorLog(_T("ini file not read properly"), __LINE__, __FILE__);
            return E_PHASE_CORRECTION;
        }
        
        l_bdoROIavg = false;
        
        if(l_nphase_option == ROIAVG) {
            l_bdoROIavg = true;
        }
        int l_roi_checked = 0;

        int l_fun_status = GetROIStatusForINROI(l_roi_checked);
        
        if(l_bdoROIavg || l_proc_roi_option == INROI) {
            if(E_NO_ERROR != l_fun_status || l_roi_checked >= MAX_NUM_ROI) { 
                CTiPrepLogHelper::WriteToErrorLog(_T("GetROIStatusForINROI failed"), __LINE__, __FILE__);
                return l_fun_status;
            }
        }
        CCommonCalculations calc;
        int k = 0;
        int l_min_val = 0;
        
        if(l_bdoROIavg /*&& l_proc_roi_option == INROI*/) {  
            roi_avg_ori = new double*[MAX_NUM_ROI]();
            
            if(NULL == roi_avg_ori) {
                CTiPrepLogHelper::WriteToErrorLog
                    (_T("SeriesPhaseCorrection(): memory allocation failed"), __LINE__, __FILE__);
                //MEMFREE(imgnImgBuff2);
                //MEMFREE(reImgBuff2);
                //MEMFREE(outImg);
                return E_PHASE_CORRECTION;
            }
            
            imgn_roi_avg_ori = new double*[MAX_NUM_ROI]();
            
            if(NULL == imgn_roi_avg_ori) {
                //MEMFREE(imgnImgBuff2);
                //MEMFREE(reImgBuff2);
                //MEMFREE(outImg);
                //MEMFREE(roi_avg_ori);
                CTiPrepLogHelper::WriteToErrorLog
                    (_T("SeriesPhaseCorrection(): memory allocation failed"), __LINE__, __FILE__);
                DEL_ARR_PTR(roi_avg_ori);
                return E_PHASE_CORRECTION;
            }
            
            for(int j = 0 ; j < MAX_NUM_ROI; j++) {
                roi_avg_ori[j] = new double [l_csReloid.size()] ;
                
                if(NULL == roi_avg_ori[j]) {
                    //MEMFREE(imgnImgBuff2);
                    //MEMFREE(reImgBuff2);
                    //MEMFREE(outImg);
                    CTiPrepLogHelper::WriteToErrorLog
                        (_T("SeriesPhaseCorrection(): memory allocation failed"), __LINE__, __FILE__);
                    
                    for(k = 0; k < j; k++) {
                        DEL_ARR_PTR(roi_avg_ori[k]);
                        DEL_ARR_PTR(imgn_roi_avg_ori[k])
                    }
                    
                    DEL_ARR_PTR(roi_avg_ori);
                    DEL_ARR_PTR(imgn_roi_avg_ori);
                    return E_PHASE_CORRECTION;
                }
                
                imgn_roi_avg_ori[j] = new double [l_csReloid.size()] ;
                
                if(NULL == imgn_roi_avg_ori[j]) {
                    //MEMFREE(imgnImgBuff2);
                    //MEMFREE(reImgBuff2);
                    //MEMFREE(outImg);
                    CTiPrepLogHelper::WriteToErrorLog
                        (_T("SeriesPhaseCorrection(): memory allocation failed"), __LINE__, __FILE__);
                    
                    for(k = 0; k < j; k++) {
                        DEL_ARR_PTR(roi_avg_ori[k]);
                        DEL_ARR_PTR(imgn_roi_avg_ori[k]);
                    }
                    
                    DEL_ARR_PTR(roi_avg_ori);
                    DEL_ARR_PTR(imgn_roi_avg_ori);
                    return E_PHASE_CORRECTION;
                }
            }
            
            status = calc.FillingROIBuffer(&l_min_val, l_csReloid, l_csReloid.size(), roi_avg_ori);
            
            if(status != E_NO_ERROR) {
                //MEMFREE(imgnImgBuff2);
                //MEMFREE(reImgBuff2);
                //MEMFREE(outImg);
                CTiPrepLogHelper::WriteToErrorLog
                    (_T("SeriesPhaseCorrection(): FillingROIBuffer() Real failed"), __LINE__, __FILE__);
                
                for(k = 0; k < MAX_NUM_ROI; k++) {
                    DEL_ARR_PTR(roi_avg_ori[k]);
                    DEL_ARR_PTR(imgn_roi_avg_ori[k]);
                }
                
                DEL_ARR_PTR(roi_avg_ori);
                DEL_ARR_PTR(imgn_roi_avg_ori);
                return E_PHASE_CORRECTION;
            }
            
            status = calc.FillingROIBuffer(&l_min_val, l_csImloid, l_csImloid.size(), imgn_roi_avg_ori);
            
            if(status != E_NO_ERROR) {
                //MEMFREE(imgnImgBuff2);
                //MEMFREE(reImgBuff2);
                //MEMFREE(outImg);
                CTiPrepLogHelper::WriteToErrorLog
                    (_T("SeriesPhaseCorrection(): FillingROIBuffer() Imaginay failed"), __LINE__, __FILE__);
                
                for(k = 0; k < MAX_NUM_ROI; k++) {
                    DEL_ARR_PTR(roi_avg_ori[k]);
                    DEL_ARR_PTR(imgn_roi_avg_ori[k]);
                }
                
                DEL_ARR_PTR(roi_avg_ori);
                DEL_ARR_PTR(imgn_roi_avg_ori);
                return E_PHASE_CORRECTION;
            }
        }
        
      
        //allocate outImg for imgsize
        status = localdb.TagReadingForImageData(l_csImloid.at(l_nNumImgs - 1), 
            &imgsz, &l_nwidth, 
            &l_nheight, &l_nbitsallocated, 
            &imgnImgBuff2, &l_img_scale_factor);
        
        if(status != E_NO_ERROR) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("SeriesPhaseCorrection(): TagReadingForImageData returned error"), __LINE__, __FILE__);
            MEMFREE(imgnImgBuff2);
            
            for(k = 0; k < MAX_NUM_ROI; k++) {
                if(roi_avg_ori) {
                    DEL_ARR_PTR(roi_avg_ori[k]);
                }
                
                if(imgn_roi_avg_ori) {
                    DEL_ARR_PTR(imgn_roi_avg_ori[k]);
                }
            }
            
            DEL_ARR_PTR(roi_avg_ori);
            DEL_ARR_PTR(imgn_roi_avg_ori);
            return E_PHASE_CORRECTION;
        }
        
        status = localdb.TagReadingForImageData(l_csReloid.at(l_nNumImgs - 1),
            &imgsz, &l_nwidth, 
            &l_nheight, &l_nbitsallocated, 
            &reImgBuff2,&l_img_scale_factor);
        
        if(status != E_NO_ERROR) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("SeriesPhaseCorrection(): TagReadingForImageData returned error"), __LINE__, __FILE__);
            MEMFREE(imgnImgBuff2);
            MEMFREE(reImgBuff2);
            
            for(k = 0; k < MAX_NUM_ROI; k++) {
                if(roi_avg_ori) {
                    DEL_ARR_PTR(roi_avg_ori[k]);
                }
                
                if(imgn_roi_avg_ori) {
                    DEL_ARR_PTR(imgn_roi_avg_ori[k]);
                }
            }
            
            DEL_ARR_PTR(roi_avg_ori);
            DEL_ARR_PTR(imgn_roi_avg_ori);
            return E_PHASE_CORRECTION;
        }
        
        outImg = malloc(imgsz);
        
        if(NULL == outImg) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("SeriesPhaseCorrection(): memory allocation failed"), __LINE__, __FILE__);
            MEMFREE(imgnImgBuff2);
            MEMFREE(reImgBuff2);
            
            for(k = 0; k < MAX_NUM_ROI; k++) {
                if(roi_avg_ori) {
                    DEL_ARR_PTR(roi_avg_ori[k]);
                }
                
                if(imgn_roi_avg_ori) {
                    DEL_ARR_PTR(imgn_roi_avg_ori[k]);
                }
            }
            
            DEL_ARR_PTR(roi_avg_ori);
            DEL_ARR_PTR(imgn_roi_avg_ori);
            return E_PHASE_CORRECTION;
        }
        status = localdb.TagReadingForImageData(l_csMagloid.at(l_nNumImgs - 1),
            &imgsz, &l_nwidth, 
            &l_nheight, &l_nbitsallocated, 
            &ImageArea_mag2,&l_img_scale_factor);
        if(status != E_NO_ERROR || l_nwidth <= 0 || l_nheight <= 0 ) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("SeriesPhaseCorrection(): TagReadingForImageData returned error"), __LINE__, __FILE__);
            MEMFREE(imgnImgBuff2);
            MEMFREE(reImgBuff2);
            MEMFREE(outImg);
            MEMFREE(ImageArea_mag2);
            
            for(k = 0; k < MAX_NUM_ROI; k++) {
                if(roi_avg_ori) {
                    DEL_ARR_PTR(roi_avg_ori[k]);
                }
                
                if(imgn_roi_avg_ori) {
                    DEL_ARR_PTR(imgn_roi_avg_ori[k]);
                }
            }
            
            DEL_ARR_PTR(roi_avg_ori);
            DEL_ARR_PTR(imgn_roi_avg_ori);
            return E_PHASE_CORRECTION;
        }
        //for ALLIMG
        //roibuff = NULL;
        unsigned int sz = 0;
        int l_roi_height = 0;
        int l_roi_width = 0;
        
//         if(CTiPrepController::GetInstance() == NULL) {
//             CString lcsmsg(_T("CTiPrepController::GetInstance() is NULL"));
//             CTiPrepLogHelper::WriteToErrorLog(lcsmsg, __LINE__, __FILE__);
//             MEMFREE(imgnImgBuff2);
//             MEMFREE(reImgBuff2);
//             MEMFREE(outImg);
//             MEMFREE(ImageArea_mag2);
//             
//             for(k = 0; k < MAX_NUM_ROI; k++) {
//                 if(roi_avg_ori) {
//                     DEL_ARR_PTR(roi_avg_ori[k]);
//                 }
//                 
//                 if(imgn_roi_avg_ori) {
//                     DEL_ARR_PTR(imgn_roi_avg_ori[k]);
//                 }
//             }
//             
//             DEL_ARR_PTR(roi_avg_ori);
//             DEL_ARR_PTR(imgn_roi_avg_ori);
//             return E_PHASE_CORRECTION;
//         }        
        if (l_proc_roi_option == INROI) {
        int l_status = CTiPrepView::GetView()->GetROIInstance()->GetROIBufferInfo(
            l_image_roi_data, 
            &l_roi_height, 
            &l_roi_width);
        
        if(!l_status) {
            CString l_error_msg(_T("Unable to get image map."));
            CTiPrepLogHelper::WriteToErrorLog
                (_T("SeriesPhaseCorrection(): Unable to get image map."), __LINE__, __FILE__);
            MEMFREE(imgnImgBuff2);
            MEMFREE(reImgBuff2);
            MEMFREE(outImg);
            MEMFREE(ImageArea_mag2);
            
            for(k = 0; k < MAX_NUM_ROI; k++) {
                if(roi_avg_ori) {
                    DEL_ARR_PTR(roi_avg_ori[k]);
                }
                
                if(imgn_roi_avg_ori) {
                    DEL_ARR_PTR(imgn_roi_avg_ori[k]);
                }
            }
            
            DEL_ARR_PTR(roi_avg_ori);
            DEL_ARR_PTR(imgn_roi_avg_ori);
            return E_PHASE_CORRECTION;
        }
        
        if(NULL == l_image_roi_data) {
            CString l_error_display(_T("Map is NULL"));
            CTiPrepLogHelper::WriteToErrorLog(l_error_display, __LINE__, __FILE__);
            MEMFREE(imgnImgBuff2);
            MEMFREE(reImgBuff2);
            MEMFREE(outImg);
            MEMFREE(ImageArea_mag2);
            
            for(k = 0; k < MAX_NUM_ROI; k++) {
                if(roi_avg_ori) {
                    DEL_ARR_PTR(roi_avg_ori[k]);
                }
                
                if(imgn_roi_avg_ori) {
                    DEL_ARR_PTR(imgn_roi_avg_ori[k]);
                }
            }
            
            DEL_ARR_PTR(roi_avg_ori);
            DEL_ARR_PTR(imgn_roi_avg_ori);
            return E_PHASE_CORRECTION;
        }
		}
        
        if(l_proc_roi_option == ALLIMG) { 
			roibuff = (byte*)malloc(sizeof(byte)*l_nwidth*l_nheight);
			if(NULL == l_image_roi_data || NULL == roibuff) {
				CString l_error_display(_T("Map is NULL"));
				CTiPrepLogHelper::WriteToErrorLog(l_error_display, __LINE__, __FILE__);
				MEMFREE(imgnImgBuff2);
				MEMFREE(reImgBuff2);
				MEMFREE(outImg);
				MEMFREE(ImageArea_mag2);
				
				for(k = 0; k < MAX_NUM_ROI; k++) {
					if(roi_avg_ori) {
						DEL_ARR_PTR(roi_avg_ori[k]);
					}
					
					if(imgn_roi_avg_ori) {
						DEL_ARR_PTR(imgn_roi_avg_ori[k]);
					}
				}
				
				DEL_ARR_PTR(roi_avg_ori);
				DEL_ARR_PTR(imgn_roi_avg_ori);
                MEMFREE(roibuff);
				return E_PHASE_CORRECTION;
        }
             memset(roibuff, 1, sizeof(byte)*l_nheight * l_nwidth); 
          //  memset(l_image_roi_data[l_roi_checked], 1, sizeof(byte)*l_roi_height * l_roi_width);
        } else {        
            roibuff = l_image_roi_data[l_roi_checked];
		}
        vector <CString> l_csNewLOIDs;
        int ret;
        CString l_csseries (_T(""));
        switch(l_ndataform) {
        case REAL:
            ret = CreateSeries(l_csReloid,l_csNewLOIDs);
            //            ret = localdb.CreateSeriesCopy(l_csReloid, l_csNewLOIDs,l_csseries);
            break;
        case IMGN:
            ret = CreateSeries(l_csImloid,l_csNewLOIDs);
            //   ret = localdb.CreateSeriesCopy(l_csImloid, l_csNewLOIDs,l_csseries);
            break;
        case MAGNITUDE:
            ret = CreateSeries(l_csMagloid,l_csNewLOIDs);
            //  ret = localdb.CreateSeriesCopy(l_csMagloid, l_csNewLOIDs,l_csseries);
            break;
        default:
            CString lcserrmsg(_T("PhaseCorrectionT() dataform invalid"));
            CTiPrepLogHelper::WriteToErrorLog(lcserrmsg, __LINE__, __FILE__);
            break;
        }
        if (ret != E_NO_ERROR)
        {
			MEMFREE(imgnImgBuff2);
            MEMFREE(reImgBuff2);
            MEMFREE(outImg);
            //MEMFREE(imgnImgBuff);
            //MEMFREE(reImgBuff);
            MEMFREE(ImageArea_mag2);
            for(k = 0; k < MAX_NUM_ROI; k++) {
                DEL_ARR_PTR(roi_avg_ori[k]);
                DEL_ARR_PTR(imgn_roi_avg_ori[k]);
            }
            DEL_ARR_PTR(roi_avg_ori);
            DEL_ARR_PTR(imgn_roi_avg_ori);
            for(int l_no_of_roi = 0 ; l_no_of_roi < MAX_LINE_ROI; l_no_of_roi++) {
                CoTaskMemFree(l_image_roi_data[l_no_of_roi]);
                l_image_roi_data[l_no_of_roi] = NULL;
            }
			if (l_proc_roi_option == ALLIMG) {
				if (roibuff)				
					free (roibuff);
				roibuff = NULL;
			}
            return ret;
        }
        if(l_csNewLOIDs.size() <= 0) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("SeriesPhaseCorrection(): loid size <0 error"), __LINE__, __FILE__);
            MEMFREE(imgnImgBuff2);
            MEMFREE(reImgBuff2);
            MEMFREE(outImg);
            //MEMFREE(imgnImgBuff);
            //MEMFREE(reImgBuff);
            MEMFREE(ImageArea_mag2);
            for(k = 0; k < MAX_NUM_ROI; k++) {
                DEL_ARR_PTR(roi_avg_ori[k]);
                DEL_ARR_PTR(imgn_roi_avg_ori[k]);
            }
            
            DEL_ARR_PTR(roi_avg_ori);
            DEL_ARR_PTR(imgn_roi_avg_ori);
            for(int l_no_of_roi = 0 ; l_no_of_roi < MAX_LINE_ROI; l_no_of_roi++) {
                CoTaskMemFree(l_image_roi_data[l_no_of_roi]);
                l_image_roi_data[l_no_of_roi] = NULL;
            }
			if (l_proc_roi_option == ALLIMG) {
				if (roibuff)				
					free (roibuff);
				roibuff = NULL;
            }
            return E_PHASE_CORRECTION;
        }
        
        for(int i = 0; i < l_nNumImgs; i++) {
            if(/*l_proc_roi_option == INROI && */l_bdoROIavg) {
                if(roi_avg_ori != NULL && roi_avg_ori[l_nselectedIdx] != NULL
                    && imgn_roi_avg_ori != NULL && imgn_roi_avg_ori[l_nselectedIdx] != NULL) {
                    roi_avgval_re = roi_avg_ori[l_nselectedIdx][i];
                    roi_avgval_im = imgn_roi_avg_ori[l_nselectedIdx][i];
                } else {
                    CString errmsg
                        (_T("roi_avg_ori or imgn_roi_avg_ori is NULL, cannot perform roi average phase correction"));
                    CTiPrepLogHelper::WriteToErrorLog(errmsg, __LINE__, __FILE__);
                    MEMFREE(imgnImgBuff2);
                    MEMFREE(reImgBuff2);
                    MEMFREE(outImg);
                    MEMFREE(ImageArea_mag2);
                    
                    for(k = 0; k < MAX_NUM_ROI; k++) {
                        if(roi_avg_ori) {
                            DEL_ARR_PTR(roi_avg_ori[k]);
                        }
                        
                        if(imgn_roi_avg_ori) {
                            DEL_ARR_PTR(imgn_roi_avg_ori[k]);
                        }
                    }
                    
                    DEL_ARR_PTR(roi_avg_ori);
                    DEL_ARR_PTR(imgn_roi_avg_ori);
                    
                    for(int l_no_of_roi = 0 ; l_no_of_roi < MAX_LINE_ROI; l_no_of_roi++) {
                        CoTaskMemFree(l_image_roi_data[l_no_of_roi]);
                        l_image_roi_data[l_no_of_roi] = NULL;
                    }
					if (l_proc_roi_option == ALLIMG) {
						if (roibuff)				
							free (roibuff);
						roibuff = NULL;
					}
                    return E_PHASE_CORRECTION;
                }
            }
            
            CLocalDBMgr localdb;
            status = localdb.GetImgBuffer(l_csImloid.at(i), &imgnImgBuff, &sz);
            
            if(status != E_NO_ERROR) {
                CTiPrepLogHelper::WriteToErrorLog
                    (_T("SeriesPhaseCorrection(): GetImgBuffer() Imaginay failed"), __LINE__, __FILE__);
                MEMFREE(imgnImgBuff2);
                MEMFREE(reImgBuff2);
                MEMFREE(outImg);
                MEMFREE(imgnImgBuff);
                MEMFREE(ImageArea_mag2);
                
                for(k = 0; k < MAX_NUM_ROI; k++) {
                    if(roi_avg_ori) {
                        DEL_ARR_PTR(roi_avg_ori[k]);
                    }
                    
                    if(imgn_roi_avg_ori) {
                        DEL_ARR_PTR(imgn_roi_avg_ori[k]);
                    }
                }
                
                DEL_ARR_PTR(roi_avg_ori);
                DEL_ARR_PTR(imgn_roi_avg_ori);
                
                for(int l_no_of_roi = 0 ; l_no_of_roi < MAX_LINE_ROI; l_no_of_roi++) {
                    CoTaskMemFree(l_image_roi_data[l_no_of_roi]);
                    l_image_roi_data[l_no_of_roi] = NULL;
                }
				if (l_proc_roi_option == ALLIMG) {
					if (roibuff)				
						free (roibuff);
					roibuff = NULL;
				}
                return E_PHASE_CORRECTION;
            }
            
            status = localdb.GetImgBuffer(l_csReloid.at(i), &reImgBuff, &sz);
            
            if(status != E_NO_ERROR) {
                CTiPrepLogHelper::WriteToErrorLog
                    (_T("SeriesPhaseCorrection(): GetImgBuffer() real failed"), __LINE__, __FILE__);
                MEMFREE(imgnImgBuff2);
                MEMFREE(reImgBuff2);
                MEMFREE(outImg);
                MEMFREE(imgnImgBuff);
                MEMFREE(reImgBuff);
                MEMFREE(ImageArea_mag2);
                
                for(k = 0; k < MAX_NUM_ROI; k++) {
                    if(roi_avg_ori) {
                        DEL_ARR_PTR(roi_avg_ori[k]);
                    }
                    
                    if(imgn_roi_avg_ori) {
                        DEL_ARR_PTR(imgn_roi_avg_ori[k]);
                    }
                }
                
                DEL_ARR_PTR(roi_avg_ori);
                DEL_ARR_PTR(imgn_roi_avg_ori);
                
                for(int l_no_of_roi = 0 ; l_no_of_roi < MAX_LINE_ROI; l_no_of_roi++) {
                    CoTaskMemFree(l_image_roi_data[l_no_of_roi]);
                    l_image_roi_data[l_no_of_roi] = NULL;
                }
				if (l_proc_roi_option == ALLIMG) {
					if (roibuff)				
						free (roibuff);
					roibuff = NULL;
				}
                return E_PHASE_CORRECTION;
            }
            
            status = localdb.GetImgBuffer(l_csMagloid.at(i), &ImageArea_mag, &sz);
            
            if(status != E_NO_ERROR) {
                CTiPrepLogHelper::WriteToErrorLog
                    (_T("SeriesPhaseCorrection(): GetImgBuffer() real failed"), __LINE__, __FILE__);
                MEMFREE(imgnImgBuff2);
                MEMFREE(reImgBuff2);
                MEMFREE(outImg);
                MEMFREE(imgnImgBuff);
                MEMFREE(reImgBuff);
                MEMFREE(ImageArea_mag2);
                MEMFREE(ImageArea_mag);
                
                for(k = 0; k < MAX_NUM_ROI; k++) {
                    if(roi_avg_ori) {
                        DEL_ARR_PTR(roi_avg_ori[k]);
                    }
                    
                    if(imgn_roi_avg_ori) {
                        DEL_ARR_PTR(imgn_roi_avg_ori[k]);
                    }
                }
                
                DEL_ARR_PTR(roi_avg_ori);
                DEL_ARR_PTR(imgn_roi_avg_ori);
                
                for(int l_no_of_roi = 0 ; l_no_of_roi < MAX_LINE_ROI; l_no_of_roi++) {
                    CoTaskMemFree(l_image_roi_data[l_no_of_roi]);
                    l_image_roi_data[l_no_of_roi] = NULL;
                }
				if (l_proc_roi_option == ALLIMG) {
					if (roibuff)				
						free (roibuff);
					roibuff = NULL;
				}
                return E_PHASE_CORRECTION;
            }
            
            //     //read images and tags..
            CString l_cserrmsg(_T(""));
            
            switch(l_nbitsallocated) {
            case 16:
                status = m_phasecorr->PhaseCorrectionT((short *)outImg, (short *)reImgBuff, 
                    (short *)imgnImgBuff,(short *)reImgBuff2, 
                    (short *)imgnImgBuff2,(short *)ImageArea_mag,
                    (short *)ImageArea_mag2, l_nheight, l_nwidth,
                    l_nbitsallocated,roibuff, l_ndataform,l_binROI, 
                    l_bdoROIavg, roi_avgval_re, roi_avgval_im);
                break;
            case 32:
                status =  m_phasecorr->PhaseCorrectionT((int *)outImg, 
                    (int *)reImgBuff, (int *)imgnImgBuff,
                    (int *)reImgBuff2, (int *)imgnImgBuff2,
                    (int *)ImageArea_mag,(int *)ImageArea_mag2, 
                    l_nheight, l_nwidth,l_nbitsallocated, roibuff, 
                    l_ndataform, l_binROI, l_bdoROIavg, 
                    roi_avgval_re, roi_avgval_im);
                break;
            default:
                l_cserrmsg.Format(_T("SeriesPhaseCorrection(): Invalid data size: %d"), l_nbitsallocated);
                CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
                MEMFREE(imgnImgBuff2);
                MEMFREE(reImgBuff2);
                MEMFREE(outImg);
                MEMFREE(imgnImgBuff);
                //MEMFREE(reImgBuff);
                MEMFREE(ImageArea_mag2);
                MEMFREE(ImageArea_mag);
                
                for(k = 0; k < MAX_NUM_ROI; k++) {
                    if(roi_avg_ori) {
                        DEL_ARR_PTR(roi_avg_ori[k]);
                    }
                    
                    if(imgn_roi_avg_ori) {
                        DEL_ARR_PTR(imgn_roi_avg_ori[k]);
                    }
                }
                
                DEL_ARR_PTR(roi_avg_ori);
                DEL_ARR_PTR(imgn_roi_avg_ori);
                
                for(int l_no_of_roi = 0 ; l_no_of_roi < MAX_LINE_ROI; l_no_of_roi++) {
                    CoTaskMemFree(l_image_roi_data[l_no_of_roi]);
                    l_image_roi_data[l_no_of_roi] = NULL;
                }
				if (l_proc_roi_option == ALLIMG) {
					if (roibuff)				
						free (roibuff);
					roibuff = NULL;
				}
                return E_PHASE_CORRECTION;
                break;
            }
            
            if(E_NO_ERROR != status) {
                l_cserrmsg.Format(_T("SeriesPhaseCorrection(): PhaseCorrectionT() failed"));
                CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
                MEMFREE(imgnImgBuff2);
                MEMFREE(reImgBuff2);
                MEMFREE(outImg);
                MEMFREE(imgnImgBuff);
                MEMFREE(reImgBuff);
                MEMFREE(ImageArea_mag2);
                MEMFREE(ImageArea_mag);
                
                for(k = 0; k < MAX_NUM_ROI; k++) {
                    if(roi_avg_ori) {
                        DEL_ARR_PTR(roi_avg_ori[k]);
                    }
                    
                    if(imgn_roi_avg_ori) {
                        DEL_ARR_PTR(imgn_roi_avg_ori[k]);
                    }
                }
                
                DEL_ARR_PTR(roi_avg_ori);
                DEL_ARR_PTR(imgn_roi_avg_ori);
                
                for(int l_no_of_roi = 0 ; l_no_of_roi < MAX_LINE_ROI; l_no_of_roi++) {
                    CoTaskMemFree(l_image_roi_data[l_no_of_roi]);
                    l_image_roi_data[l_no_of_roi] = NULL;
                }
				if (l_proc_roi_option == ALLIMG) {
					if (roibuff)				
						free (roibuff);
					roibuff = NULL;
				}
                return E_PHASE_CORRECTION;
            }
            
            l_csimgLOID = l_csNewLOIDs[i];
            
            if(E_NO_ERROR != localdb.UpdateImageBuffer(l_csimgLOID, outImg, imgsz)) { 
                CTiPrepLogHelper::WriteToErrorLog
                    (_T("SeriesPhaseCorrection(): UpdateImageBuffer() failed"), __LINE__, __FILE__);
            }
            
            MEMFREE(imgnImgBuff);
            MEMFREE(reImgBuff);
            MEMFREE(ImageArea_mag);
        }
        
        for(int l_no_of_roi = 0 ; l_no_of_roi < MAX_LINE_ROI; l_no_of_roi++) {
            CoTaskMemFree(l_image_roi_data[l_no_of_roi]);
            l_image_roi_data[l_no_of_roi] = NULL;
        }
        
        MEMFREE(imgnImgBuff2);
        MEMFREE(reImgBuff2);
        MEMFREE(outImg);
        MEMFREE(ImageArea_mag2);                
        for(k = 0; k < MAX_NUM_ROI; k++) {
            if(roi_avg_ori) {
                DEL_ARR_PTR(roi_avg_ori[k]);
            }
            
            if(imgn_roi_avg_ori) {
                DEL_ARR_PTR(imgn_roi_avg_ori[k]);
            }
        }
        
        DEL_ARR_PTR(roi_avg_ori);
        DEL_ARR_PTR(imgn_roi_avg_ori);
		if (l_proc_roi_option == ALLIMG) {
			if (roibuff)				
				free (roibuff);
			roibuff = NULL;
		}
        CTiPrepView::GetView()->RefreshIsel();
    } catch(...) {
        CString errmsg(_T("Exception occurred in SeriesPhaseCorrection"));
        CTiPrepLogHelper::WriteToErrorLog(errmsg, __LINE__, __FILE__);
        
        for(int l_no_of_roi = 0 ; l_no_of_roi < MAX_LINE_ROI; l_no_of_roi++) {
            CoTaskMemFree(l_image_roi_data[l_no_of_roi]);
            l_image_roi_data[l_no_of_roi] = NULL;
        }
        
        MEMFREE(imgnImgBuff2);
        MEMFREE(reImgBuff2);
        MEMFREE(outImg);
        MEMFREE(ImageArea_mag2);
        
        int k = 0;
        
        for(k = 0; k < MAX_NUM_ROI; k++) {
            if(roi_avg_ori) {
                DEL_ARR_PTR(roi_avg_ori[k]);
            }
            
            if(imgn_roi_avg_ori) {
                DEL_ARR_PTR(imgn_roi_avg_ori[k]);
            }
        }
        
        DEL_ARR_PTR(roi_avg_ori);
        DEL_ARR_PTR(imgn_roi_avg_ori);
        MEMFREE(imgnImgBuff);
        MEMFREE(reImgBuff);
        MEMFREE(ImageArea_mag);
        return E_ERR_EXCEPTION;
    }
    
    return E_NO_ERROR;
}

/*************************************************************
* Method:    	AutoPhaseValidations
* Description: 	This function is used 
* Returns:   	int
*************************************************************/
int CTiPrepController::AutoPhaseValidations()
{
    TIPREP_TRACE(CTiPrepView::AutoPhaseValidations,MRTraceMarker3);
    CString l_csimgLOID(_T(""));
    CString l_protocol_name, l_intermediate_string, l_study_file_name(_T("")), l_csProcessName;
    int l_nNumImgs = 0; // from selected LOIDs
    CString l_csStudyFileName;
    CLocalDBMgr l_localdb ;
    vector<CString> l_csloid;
    
    if(!m_phasecorr) {
        CTiPrepLogHelper::WriteToErrorLog
            (_T("Phase correction class object is null"), __LINE__, __FILE__);
        return E_PHASE_CORRECTION;
    }
    
    l_nNumImgs = m_phasecorr->GetNumSelectedImgs();
    m_phasecorr->getSelImgLOID(l_csloid);
    CString l_csSeriesLOID(_T(""));
    int l_nStatus = l_localdb.GetParentLOID(l_csloid.at(0), LOID_TYPE_IMAGE, l_csSeriesLOID);
    
    if(E_NO_ERROR != l_nStatus) {
        CTiPrepLogHelper::WriteToErrorLog(_T("Unable to get file path from LocalDB."), ERROR_LOGONLY);
        return(E_PHASE_CORRECTION);
    }
    
    CLocalDBMgr localdb;
    l_nStatus = localdb.GetProtocolNameFromSeriesLevelDB(l_csSeriesLOID, l_protocol_name, l_csProcessName);
    
    if(E_NO_ERROR != l_nStatus) {
        CTiPrepLogHelper::WriteToErrorLog(_T("Unable to get protocol."), ERROR_LOGONLY);
        return(E_PHASE_CORRECTION);
    }
    
    l_intermediate_string.Format(_T("%s"), l_csProcessName);
    int l_count_special_char = l_intermediate_string.Find(_T(':'));
    l_csStudyFileName = l_intermediate_string.Left(l_count_special_char);
    int l_total_length = l_csProcessName.GetLength();
    l_count_special_char = l_csProcessName.ReverseFind(_T(':'));
    l_intermediate_string = l_csProcessName.Right(l_total_length - (l_count_special_char + 2));
    l_count_special_char = l_intermediate_string.ReverseFind(_T('/'));
    l_intermediate_string = l_intermediate_string.Left(l_count_special_char);
    l_count_special_char = l_intermediate_string.Find(_T('.'));
    l_total_length = l_intermediate_string.GetLength();
    l_intermediate_string = l_intermediate_string.Right(l_total_length - (l_count_special_char + 1));
    char l_intermediate_char[10];
    sprintf(l_intermediate_char, "%S", l_intermediate_string);
    int l_protocol_no = atoi(l_intermediate_char);
    l_count_special_char = l_csProcessName.ReverseFind(_T(':'));
    l_total_length = l_csProcessName.GetLength();
    l_intermediate_string = l_csProcessName.Right(l_total_length - (l_count_special_char + 2));
    l_count_special_char = l_intermediate_string.ReverseFind(_T('/'));
    l_intermediate_string = l_intermediate_string.Right(l_count_special_char);
    l_count_special_char = l_intermediate_string.Find(_T('.'));
    l_total_length = l_intermediate_string.GetLength();
    l_intermediate_string = l_intermediate_string.Right(l_total_length - (l_count_special_char + 1));
    sprintf(l_intermediate_char, "%S", l_intermediate_string);
    int l_process_no = atoi(l_intermediate_char);
    imgProcessData_t l_imgData[3] = {{0, 0, 0, 0, NULL},
    {0, 0, 0, 0, NULL},
    {0, 0, 0, 0, NULL}
    };
    m_csStudyFileName = l_csStudyFileName;
    m_nProtocolnoPC = l_protocol_no;
    m_nProcessNoPC = l_process_no;
    CvfcDBMgr l_vfcDb;
    VfDBHandle_t l_vfd;

    l_nStatus = l_vfcDb.OpenDBFile(l_csStudyFileName,l_vfd);
    
    if(E_NO_ERROR != l_nStatus) {
         l_vfcDb.CloseDBFile(l_vfd);
        return E_PHASE_CORRECTION;
    }
    
    int l_nDataFlag[3] = {0};
    l_nStatus = l_vfcDb.protTraverse(l_protocol_no, 1, l_imgData, l_nDataFlag,l_vfd);
    
    if(E_NO_ERROR != l_nStatus) {
        for(int l_nCnt = 0; l_nCnt < 3; l_nCnt++) {
            if(NULL != l_imgData[l_nCnt].convPolImgs) {
                free(l_imgData[l_nCnt].convPolImgs);
            }
        }
         l_vfcDb.CloseDBFile(l_vfd);
        return E_PHASE_CORRECTION;
    }
     l_vfcDb.CloseDBFile(l_vfd);
    
    
     if(l_nDataFlag[0] != 1 || l_nDataFlag[2] != 1) {
         l_nStatus = rawCheckRecon(l_csStudyFileName,l_protocol_no);
         if (E_NO_ERROR != l_nStatus)
         {
             if(l_nStatus == E_RAW_CHECK_FAIL){
                 CTiPrepLogHelper::WriteToErrorLog(_T("Raw Check failed."), ERROR_LOGONLY);
                 return E_RAW_CHECK_FAIL;
             }else if(l_nStatus == E_NO_RAW){
                 CTiPrepLogHelper::WriteToErrorLog(_T("Raw data is not present for selected protocol"), ERROR_LOGONLY);
                 return E_NO_RAW;
             }
		}
     }
    if(l_nDataFlag[0] != 1) {
	
        AfxGetApp()->DoWaitCursor(1);
        CIRSListner m_IrsComm;
        BOOL l_nstatus = m_IrsComm.InitializeIRS();   
        if (TRUE != l_nstatus){
            CString l_cserrmsg = (_T("Failed to Initialize IRS"));
            CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);  
            return E_ERROR;
        }
        Sleep(2000);
        CReconstruction  l_cReconObj;
        l_nStatus = l_cReconObj.performReconstruction(l_csStudyFileName, l_protocol_no,
            VFC_FORM_REAL_ONLY);
        if(E_NO_ERROR != l_nStatus){
            CTiPrepLogHelper::WriteToErrorLog
                (_T("Reconstruction failed for data type VFC_FORM_REAL_ONLY."), ERROR_LOGONLY);
            AfxGetApp()->DoWaitCursor(-1);
            m_IrsComm.ForceExitIRS();
            return E_PHASE_CORRECTION;           
        }else{
            m_IrsComm.WaitForIRSDone();
        }
        l_nDataFlag[0] = 1;
    }    
    if(l_nDataFlag[2] != 1) {
        AfxGetApp()->DoWaitCursor(1);
        CIRSListner m_IrsComm;
        BOOL l_nstatus = m_IrsComm.InitializeIRS();   
        if (TRUE != l_nstatus){
            CString l_cserrmsg = (_T("Failed to Initialize IRS"));
            CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);  
            return E_ERROR;
        }
        Sleep(2000);
        CReconstruction  l_cReconObj;
        l_nStatus = l_cReconObj.performReconstruction(l_csStudyFileName, l_protocol_no,
            VFC_FORM_IMGN_ONLY);
        if(E_NO_ERROR != l_nStatus){
            CTiPrepLogHelper::WriteToErrorLog
                (_T("Reconstruction failed for data type VFC_FORM_IMGN_ONLY."), ERROR_LOGONLY);
            AfxGetApp()->DoWaitCursor(-1);
            m_IrsComm.ForceExitIRS();
            return E_PHASE_CORRECTION;            
        }else{
            m_IrsComm.WaitForIRSDone();
        }

        l_nDataFlag[2] = 1;
    }
   
        int status = 0;
    if (l_nDataFlag[2] != 0 && l_nDataFlag[0] != 0)
    {
        AfxGetApp()->DoWaitCursor(1);
        l_nStatus = validateSelectionSingleButton(m_nProtocolnoPC,m_csStudyFileName);
        if (E_NO_ERROR != l_nStatus)
        {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("validateSelectionSingleButton failed."), ERROR_LOGONLY);
            AfxGetApp()->DoWaitCursor(-1);
            return E_PHASE_CORRECTION;  
        }
        AfxGetApp()->DoWaitCursor(-1);
    }

    for(int l_nCnt = 0; l_nCnt < 3; l_nCnt++) {
        if(NULL != l_imgData[l_nCnt].convPolImgs) {
            free(l_imgData[l_nCnt].convPolImgs);
        }
    }
    return status;
}

/*************************************************************
* Method:    	GetSelImgInfoFromIsel
* Description: 	This function is used 
* Parameter: 	int & f_MaxCounter :
* Parameter: 	int f_nEvent :
* Returns:   	int
*************************************************************/
int CTiPrepController::GetSelImgInfoFromIsel(int & f_MaxCounter, int f_nEvent/*, CString f_csSeriesName*/)
{
    CLocalDBMgr localdb;
    CString l_csimgloid;
    int l_ntotalimgs = 0;
    vector<CString> l_loid;
    CString lcsdataForm(_T(""));
    CString l_csseries_number(_T(""));
    CString l_csSeriesName(_T(""));
    CTiPrepDlg* l_TiPrepDlg = NULL;
    l_TiPrepDlg = (CTiPrepView::GetView())->GetCTiPrepDlgInstance();//vilas_31Oct..to remove global variable

    int status = 0;
    switch(f_nEvent) {
    case EVENT_AUTOPHASE:
        if(!m_phasecorr) {
            //m_phasecorr = new CPhaseCorrection();
            CTiPrepLogHelper::WriteToErrorLog(_T("pointer is null"), __LINE__, __FILE__);
            status = E_ERROR_NULL_FUNCPARAMS;
            break;
            //vilas_31Oct..further code will never work so break here
        }
        
        if(m_phasecorr) {//vilas_31Oct..checkrepeated
//             if(m_phasecorr) {
            l_csimgloid = m_phasecorr->GetFirstLOID();
//             }
            
            if(l_csimgloid.IsEmpty()) { 
            //vilas_31Oct..this is already handled much earlier in CICMMgr::GetLOIDSelectedImages
                //so better to skip further processing in CDPSComManager::RecvCallBack
                //in that case flow will not reach here..comment added there as well

                //show message please select series in iselector, this is handled below, so do nothing
                break;
            }
            
            localdb.GetParentLOID(l_csimgloid, LOID_TYPE_IMAGE, l_csSeriesName);
            localdb.GetChildNodeLOIDs(l_csSeriesName, l_ntotalimgs,LOID_TYPE_SERIES);
            //vilas_31Oct.. <3 condition is not clear
            if(f_MaxCounter < 3 && m_phasecorr->GetNumSelectedImgs()/*l_loid.size()*/ != l_ntotalimgs) { //temporary
                CTiPrepView::GetView()->GetSelectedSeriesImages(l_csSeriesName);
                f_MaxCounter++;
            } else if(m_phasecorr->GetNumSelectedImgs()/*l_loid.size()*/ == l_ntotalimgs) {
                f_MaxCounter = 0;
                long series_no = 0;
                localdb.GetSeriesNumberFromDB(l_csSeriesName, &series_no);
                int prot_no = (series_no - (series_no % 1000));
                if (prot_no < 0 ) {
                    prot_no = - prot_no;
                }
                l_csseries_number.Format(_T("%d"), prot_no);
                
                if(NULL != l_TiPrepDlg) {
                    l_TiPrepDlg->DisplayPhCorrSeries(l_csseries_number);
                } else {
                    //
                }
                int l_nStatus = AutoPhaseValidations();
                if_no_error(l_nStatus) {
                }else{
                    CTiPrepLogHelper::WriteToErrorLog(_T("OnCmdGetSelImg() SeriesPhaseCorrection error"), __LINE__, __FILE__);
                    CTiPrepView::GetView()->DisplayErr(l_nStatus);
                }                
            } else if(f_MaxCounter > 3) {
                f_MaxCounter = 0;
            }
        }
        
        break;
    case EVENT_MG_SELECT:
        lcsdataForm = _T("Magnitude");
        
        if(m_phasecorr) {
            l_csimgloid = m_phasecorr->GetFirstLOID();
            
            if(l_csimgloid.IsEmpty()) {
                break;
            }
            
            localdb.GetParentLOID(l_csimgloid, LOID_TYPE_IMAGE, l_csSeriesName);
            localdb.GetChildNodeLOIDs(l_csSeriesName, l_ntotalimgs,LOID_TYPE_SERIES);
            
            if(f_MaxCounter < 3 && m_phasecorr->GetNumSelectedImgs() != l_ntotalimgs) { 
                CTiPrepView::GetView()->GetSelectedSeriesImages(l_csSeriesName);
                f_MaxCounter++;
            }  else {
                f_MaxCounter = 0;
                m_phasecorr->getSelImgLOID(l_loid);
                status = ValidateManualSeries(lcsdataForm, l_loid);
                if (status == E_NO_ERROR) {
                    l_csseries_number = (_T(""));
                    long series_no = 0;
                    localdb.GetSeriesNumberFromDB(l_csSeriesName, &series_no);
                    l_csseries_number.Format(_T("%d"), series_no);
                }
                else {
                    l_csseries_number = _T("");
                }
                //vilas_31Oct..to remove global variable
                if(l_TiPrepDlg) {
                    l_TiPrepDlg->DisplayPCSelectedSeries(l_csseries_number, MAGNITUDE);
                }
            }
        } else {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("OnCmdGetSelImg() m_phasecorr is NULL"), __LINE__, __FILE__);
        }
        
        break;
    case EVENT_RE_SELECT:
        lcsdataForm = _T("Real");
        
        if(m_phasecorr) {
            m_phasecorr->getRealImgLOID(l_loid);
            
            if(l_loid.size() <= 0) {
                break;
            } else {
                localdb.GetParentLOID(l_loid.at(0), LOID_TYPE_IMAGE, l_csSeriesName);
                localdb.GetChildNodeLOIDs(l_csSeriesName, l_ntotalimgs);
            }
            
            if(f_MaxCounter < 3 && (int)l_loid.size()/*l_loid.size()*/ != l_ntotalimgs) { 
                CTiPrepView::GetView()->GetSelectedSeriesImages(l_csSeriesName);
                f_MaxCounter++;
            }  else {
                f_MaxCounter = 0;
                status = ValidateManualSeries(lcsdataForm, l_loid);
                if_no_error(status) {
                    long series_no = 0;
                    l_csseries_number = (_T(""));
                    localdb.GetSeriesNumberFromDB(l_csSeriesName, &series_no);
                    l_csseries_number.Format(_T("%d"), series_no);
                }
                else {
                    l_csseries_number = _T("");
                }
                //vilas_31Oct..to remove global variable
                if(l_TiPrepDlg) {
                    l_TiPrepDlg->DisplayPCSelectedSeries(l_csseries_number, REAL);
                }
            }
        } else {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("OnCmdGetSelImg() m_pAddOffset is NULL"), __LINE__, __FILE__);
        }
        
        break;
    case EVENT_IMG_SELECT:
        lcsdataForm = _T("Imaginary");
        
        if(m_phasecorr) {
            m_phasecorr->getImgnImgLOID(l_loid);
            
            if(l_loid.size() <= 0) {
                break;
            } else {
                localdb.GetParentLOID(l_loid.at(0), LOID_TYPE_IMAGE, l_csSeriesName);
                localdb.GetChildNodeLOIDs(l_csSeriesName, l_ntotalimgs);
            }
            
            if(f_MaxCounter < 3 && (int)l_loid.size()/*l_loid.size()*/ != l_ntotalimgs) { //temporary
                CTiPrepView::GetView()->GetSelectedSeriesImages(l_csSeriesName);
                f_MaxCounter++;
            }  else {
                f_MaxCounter = 0;
                status = ValidateManualSeries(lcsdataForm, l_loid);
                if(status == E_NO_ERROR) {
                    l_csseries_number = (_T(""));
                    long series_no = 0;
                    localdb.GetSeriesNumberFromDB(l_csSeriesName, &series_no);
                    l_csseries_number.Format(_T("%d"), series_no);
                }
                else {
                    l_csseries_number = (_T(""));
                }
                //vilas_31Oct..to remove global variable
                if(l_TiPrepDlg) {
                    l_TiPrepDlg->DisplayPCSelectedSeries(l_csseries_number, IMGN);
                }
            }
        } else {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("OnCmdGetSelImg() m_pAddOffset is NULL"), __LINE__, __FILE__);
        }
        
    case EVENT_POLARITY_CORRECTION:
        
        // if(CTiPrepController::GetInstance()) {
            l_csimgloid = /*CTiPrepController::GetInstance()->*/GetFirstPolarityLOID();
            
            if(l_csimgloid.IsEmpty()) {
                break;
            }
            
            localdb.GetParentLOID(l_csimgloid, LOID_TYPE_IMAGE, l_csSeriesName);
            localdb.GetChildNodeLOIDs(l_csSeriesName, l_ntotalimgs);
            
            if(f_MaxCounter < 3 && /*CTiPrepController::GetInstance()->*/GetNumImagesForPolarityCorr() != l_ntotalimgs) { //temporary
                CTiPrepView::GetView()->GetSelectedSeriesImages(l_csSeriesName);
                f_MaxCounter++;
            } else {
                f_MaxCounter = 0;
            }
//         } else {
//             CTiPrepLogHelper::WriteToErrorLog(_T("OnCmdGetSelImg() CTiPrepController::GetInstance() returns NULL"), __LINE__, __FILE__);
//         }
        
        break;
    case EVENT_ADD_OFFSET:
        
 //        if(CTiPrepController::GetInstance()) {
            l_csimgloid = /*CTiPrepController::GetInstance()->*/GetFirstOffsetLOID(); 
            
            if(l_csimgloid.IsEmpty()) {
                break;
            }
            
            localdb.GetParentLOID(l_csimgloid, LOID_TYPE_IMAGE, l_csSeriesName);
            localdb.GetChildNodeLOIDs(l_csSeriesName, l_ntotalimgs);
            
            if(f_MaxCounter < 3 && /*CTiPrepController::GetInstance()->*/GetNumImagesForAddOffset()/*l_loid.size()*/ != l_ntotalimgs) { //temporary
                CTiPrepView::GetView()->GetSelectedSeriesImages(l_csSeriesName);
                f_MaxCounter++;
            } else {
                f_MaxCounter = 0;
            }
//         } else {
//             CTiPrepLogHelper::WriteToErrorLog
//                 (_T("OnCmdGetSelImg() CTiPrepController::GetInstance() returns NULL"), __LINE__, __FILE__);
//         }
        
        break;
    default:
        break;
    }
    
    return status;
}

/*************************************************************
* Method:    	ValidateManualSeries
* Description: 	This function is used 
* Parameter: 	CString & img_data_form :
* Parameter: 	vector<CString> & f_loids :
* Returns:   	int
*************************************************************/
int CTiPrepController::ValidateManualSeries(CString& img_data_form, vector<CString> &f_loids)
{
    TIPREP_TRACE(CTiPrepController::ValidateManualSeries,MRTraceMarker3);
    
    try {
        CString l_csimgLOID(_T(""));
        CString f_csDataType(_T("")); 
        int l_nstatus = CTiPrepView::GetView()->vfcDataGetDataForm(f_csDataType, f_loids); 
        
        if(E_NO_ERROR != l_nstatus) {
            CString lcsmsg(_T("Unable to get Data Form"));
            CTiPrepLogHelper::WriteToErrorLog(lcsmsg, __LINE__, __FILE__);
            f_loids.clear();
            return E_ERROR;
        }
        
        if(f_csDataType.CompareNoCase(img_data_form)) {
            CString lcserrmsg(_T(""));
            lcserrmsg.Format(_T("Please Select %s Image"), img_data_form);
            f_loids.clear();
            if (img_data_form.CompareNoCase(_T("Magnitude")) == 0)
                return I_MAG_IMG;
            else if (img_data_form.CompareNoCase(_T("Real")) == 0)
                return I_REAL_IMG;
            else if (img_data_form.CompareNoCase(_T("Imaginary")) == 0)
                return I_IMGRY_IMG;
            else 
                return E_ERROR;
        }
        
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(_T("Exception in validation"), __LINE__, __FILE__);
        f_loids.clear();
        return E_ERROR;
    }
    
    return E_NO_ERROR;
}

/*************************************************************
* Method:    	ValidateSelectedSeries
* Description: 	This function is used 
* Returns:   	int
*************************************************************/
int CTiPrepController::ValidateSelectedSeries()
{
    TIPREP_TRACE(CTiPrepController::ValidateSelectedSeries,MRTraceMarker3);
    int l_nNumImgs = 0; // from selected LOIDs
    CString l_csProtocolName, l_csStudyFileName;
    int l_nProtocolNum = 0;
    int l_nReProtocolNum = 0;
    CLocalDBMgr localdb ;
    int l_nProcNum_re = 0;
    int l_nProcNum_img = 0;
    vector<CString> l_csloid;
    
    try {
        if(!m_phasecorr) {
            CTiPrepLogHelper::WriteToErrorLog(_T("Phase correction class object is null"), __LINE__, __FILE__);
            return E_ERROR_NULL_FUNCPARAMS;
        }
        
        l_nNumImgs = m_phasecorr->GetNumSelectedImgs();
        m_phasecorr->getSelImgLOID(l_csloid);
        vector<CString> l_csReloid;
        vector<CString> l_csImloid;
        vector<CString> l_csMgloid;
        m_phasecorr->getRealImgLOID(l_csReloid);
        m_phasecorr->getImgnImgLOID(l_csImloid);
        m_phasecorr->getSelImgLOID(l_csMgloid);
        if(l_csReloid.size() <= 0) {
            CTiPrepLogHelper::WriteToErrorLog(_T("Please Select a Real Series"));
            return I_REAL_IMG;
        }
        
        if(l_csImloid.size() <= 0) {
            CTiPrepLogHelper::WriteToErrorLog(_T("Please Select an Imaginary Series"));
            return I_IMGRY_IMG;
        }
        if(l_csMgloid.size() <= 0) {
            CTiPrepLogHelper::WriteToErrorLog(_T("Please Select an Imaginary Series"));
            return I_MAG_IMG;
        }
        if((l_csReloid.size() != l_csImloid.size()) || (l_csReloid.size() != l_csMgloid.size())) {
            CTiPrepLogHelper::WriteToErrorLog(_T("Number of images in Real and Imaginary are different"));
            return E_NUM_DIFF;
        }
        
        l_nNumImgs = m_phasecorr->GetNumSelectedImgs();
        CString l_csseries(_T(""));
        int l_nstatus = localdb.GetParentLOID(l_csReloid[0], LOID_TYPE_IMAGE, l_csseries);
        
        if(l_csseries.IsEmpty()) {
            CString l_csmsg(_T("Unable to get Parent LOID"));
            CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
            return E_VALIDATION_FAILED;
        }
        
        CString l_csProcessName(_T(""));
        CString l_intermediate_string(_T(""));
        l_nstatus = localdb.GetProtocolNameFromSeriesLevelDB(l_csseries, l_csProtocolName, l_csProcessName);
        if_no_error(l_nstatus) {
            l_intermediate_string.Format(_T("%s"), l_csProcessName);
            int l_count_special_char = l_intermediate_string.Find(_T(':'));
            l_csStudyFileName = l_intermediate_string.Left(l_count_special_char);
            int l_total_length = l_csProcessName.GetLength();
            l_count_special_char = l_csProcessName.ReverseFind(_T(':'));
            l_intermediate_string = l_csProcessName.Right(l_total_length - (l_count_special_char + 2));
            l_count_special_char = l_intermediate_string.ReverseFind(_T('/'));
            l_intermediate_string = l_intermediate_string.Left(l_count_special_char);
            l_count_special_char = l_intermediate_string.Find(_T('.'));
            l_total_length = l_intermediate_string.GetLength();
            l_intermediate_string = l_intermediate_string.Right(l_total_length - (l_count_special_char + 1));
            char l_intermediate_char[10];
            sprintf(l_intermediate_char, "%S", l_intermediate_string);
            l_nReProtocolNum = atoi(l_intermediate_char);
            l_count_special_char = l_csProcessName.ReverseFind(_T(':'));
            l_total_length = l_csProcessName.GetLength();
            l_intermediate_string = l_csProcessName.Right(l_total_length - (l_count_special_char + 2));
            l_count_special_char = l_intermediate_string.ReverseFind(_T('/'));
            l_intermediate_string = l_intermediate_string.Right(l_count_special_char);
            l_count_special_char = l_intermediate_string.Find(_T('.'));
            l_total_length = l_intermediate_string.GetLength();
    l_intermediate_string = l_intermediate_string.Right(l_total_length - (l_count_special_char + 1));
            sprintf(l_intermediate_char, "%S", l_intermediate_string);
            l_nProcNum_re = atoi(l_intermediate_char);
        }
        else {
            CString lcsmsg(_T("Unable to get vfc Path name from DB"));
            CTiPrepLogHelper::WriteToErrorLog(lcsmsg, __LINE__, __FILE__);
            return E_VALIDATION_FAILED;
        }
        
        l_nstatus = localdb.GetParentLOID(l_csImloid[0], LOID_TYPE_IMAGE, l_csseries);
        
        if(l_csseries.IsEmpty()) {
            CString l_csmsg(_T("Unable to get Parent LOID"));
            CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
            return E_VALIDATION_FAILED;
        }
        l_nstatus = localdb.GetProtocolNameFromSeriesLevelDB(l_csseries, l_csProtocolName, l_csProcessName);
        if_no_error(l_nstatus) {
            l_intermediate_string.Format(_T("%s"), l_csProcessName);
            int l_count_special_char = l_intermediate_string.Find(_T(':'));
            l_csStudyFileName = l_intermediate_string.Left(l_count_special_char);
            int l_total_length = l_csProcessName.GetLength();
            l_count_special_char = l_csProcessName.ReverseFind(_T(':'));
            l_intermediate_string = l_csProcessName.Right(l_total_length - (l_count_special_char + 2));
            l_count_special_char = l_intermediate_string.ReverseFind(_T('/'));
            l_intermediate_string = l_intermediate_string.Left(l_count_special_char);
            l_count_special_char = l_intermediate_string.Find(_T('.'));
            l_total_length = l_intermediate_string.GetLength();
            l_intermediate_string = l_intermediate_string.Right(l_total_length - (l_count_special_char + 1));
            char l_intermediate_char[10];
            sprintf(l_intermediate_char, "%S", l_intermediate_string);
            l_nProtocolNum = atoi(l_intermediate_char);
            //Processed index
            l_count_special_char = l_csProcessName.ReverseFind(_T(':'));
            l_total_length = l_csProcessName.GetLength();
            l_intermediate_string = l_csProcessName.Right(l_total_length - (l_count_special_char + 2));
            l_count_special_char = l_intermediate_string.ReverseFind(_T('/'));
            l_intermediate_string = l_intermediate_string.Right(l_count_special_char);
            l_count_special_char = l_intermediate_string.Find(_T('.'));
            l_total_length = l_intermediate_string.GetLength();
    l_intermediate_string = l_intermediate_string.Right(l_total_length - (l_count_special_char + 1));
            sprintf(l_intermediate_char, "%S", l_intermediate_string);
            l_nProcNum_img = atoi(l_intermediate_char);
        }
        else {
            CString lcsmsg(_T("Unable to get vfc Path name from DB"));
            CTiPrepLogHelper::WriteToErrorLog(lcsmsg, __LINE__, __FILE__);
            return E_VALIDATION_FAILED;
        }
        
        if(l_nReProtocolNum != l_nProtocolNum) {
            return E_PROT_DIFF;
        }
        l_nstatus = localdb.GetParentLOID(l_csMgloid[0], LOID_TYPE_IMAGE, l_csseries);
        
        if(l_csseries.IsEmpty()) {
            CString l_csmsg(_T("Unable to get Parent LOID"));
            CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
            return E_VALIDATION_FAILED;
        }
        l_nstatus = localdb.GetProtocolNameFromSeriesLevelDB(l_csseries, l_csProtocolName, l_csProcessName);
        int prot_mag = 0, procc_mag = 0;
        if_no_error(l_nstatus) {
            l_intermediate_string.Format(_T("%s"), l_csProcessName);
            int l_count_special_char = l_intermediate_string.Find(_T(':'));
            l_csStudyFileName = l_intermediate_string.Left(l_count_special_char);
            int l_total_length = l_csProcessName.GetLength();
            l_count_special_char = l_csProcessName.ReverseFind(_T(':'));
            l_intermediate_string = l_csProcessName.Right(l_total_length - (l_count_special_char + 2));
            l_count_special_char = l_intermediate_string.ReverseFind(_T('/'));
            l_intermediate_string = l_intermediate_string.Left(l_count_special_char);
            l_count_special_char = l_intermediate_string.Find(_T('.'));
            l_total_length = l_intermediate_string.GetLength();
            l_intermediate_string = l_intermediate_string.Right(l_total_length - (l_count_special_char + 1));
            char l_intermediate_char[10];
            sprintf(l_intermediate_char, "%S", l_intermediate_string);
            prot_mag = atoi(l_intermediate_char);
            //Processed index
            l_count_special_char = l_csProcessName.ReverseFind(_T(':'));
            l_total_length = l_csProcessName.GetLength();
            l_intermediate_string = l_csProcessName.Right(l_total_length - (l_count_special_char + 2));
            l_count_special_char = l_intermediate_string.ReverseFind(_T('/'));
            l_intermediate_string = l_intermediate_string.Right(l_count_special_char);
            l_count_special_char = l_intermediate_string.Find(_T('.'));
            l_total_length = l_intermediate_string.GetLength();
            l_intermediate_string = l_intermediate_string.Right(l_total_length - (l_count_special_char + 1));
            sprintf(l_intermediate_char, "%S", l_intermediate_string);
            procc_mag = atoi(l_intermediate_char);
        }
        else {
            CString lcsmsg(_T("Unable to get vfc Path name from DB"));
            CTiPrepLogHelper::WriteToErrorLog(lcsmsg, __LINE__, __FILE__);
            return E_VALIDATION_FAILED;
        }
        
        if(l_nReProtocolNum != prot_mag) {
            return E_PROT_DIFF;
        }
        
        l_nstatus = 0;
        int l_nimgngdckind = 0;
        int l_nregdckind = 0;
        CvfcDBMgr l_vfcDb;
        VfDBHandle_t  l_vfd;
        int vf_status = l_vfcDb.OpenDBFile(l_csStudyFileName,l_vfd);
        
        if(vf_status != E_NO_ERROR) {
//             CString lcsmsg(_T("ValidateSelectedSeries() vfc open failed"));
//             CTiPrepLogHelper::WriteToErrorLog(lcsmsg, __LINE__, __FILE__);
            return E_VALIDATION_FAILED;
        }
        
        l_nstatus = l_vfcDb.getGDCImageKind(l_nProtocolNum, l_nProcNum_re, l_nregdckind,l_vfd);
        
        if(E_NO_ERROR != l_nstatus) {
            CTiPrepLogHelper::WriteToErrorLog(_T("Unable to get GDC kind"));
            l_vfcDb.CloseDBFile(l_vfd);
            return E_VALIDATION_FAILED;
        }
        
        l_nstatus = l_vfcDb.getGDCImageKind(l_nProtocolNum, l_nProcNum_img, l_nimgngdckind,l_vfd);
        
        if(l_nstatus != E_NO_ERROR) {
            CString lcsmsg(_T("Unable to get GDC kind name from vfc"));
            CTiPrepLogHelper::WriteToErrorLog(lcsmsg, __LINE__, __FILE__);
            l_vfcDb.CloseDBFile(l_vfd);
            return E_VALIDATION_FAILED;
        }
        
        int imggdc_mg = 0;
        l_nstatus = l_vfcDb.getGDCImageKind(l_nProtocolNum, procc_mag, imggdc_mg,l_vfd);
        
        if(l_nstatus != E_NO_ERROR) {
            CString lcsmsg(_T("Unable to get GDC kind name from vfc"));
            CTiPrepLogHelper::WriteToErrorLog(lcsmsg, __LINE__, __FILE__);
            l_vfcDb.CloseDBFile(l_vfd);
            return E_VALIDATION_FAILED;
        }
        
        l_vfcDb.CloseDBFile(l_vfd);
        
        if((l_nimgngdckind != l_nregdckind) ||
            (l_nimgngdckind != imggdc_mg)){
            return E_GDC_DIFF;
        }
        
    } catch(...) {
        CString lcsmsg(_T("Excpetion occurred in ValidateSelectedSeries()"));
        CTiPrepLogHelper::WriteToErrorLog(lcsmsg, __LINE__, __FILE__);
        return E_ERR_EXCEPTION;
    }
    
    return E_NO_ERROR;
}

/*************************************************************
* Method:    	setDataFormPhaseCorrect
* Description: 	This function is used 
* Parameter: 	CString & f_csDataForm :
* Returns:   	int
*************************************************************/
int CTiPrepController::setDataFormPhaseCorrect(CString &f_csDataForm)
{
    m_nDataForm = REAL;
    if (f_csDataForm == _T("Real"))
    {
        m_nDataForm = REAL;
    }else if(f_csDataForm == _T("Imaginary")){
        m_nDataForm = IMGN;
        
    }else if (f_csDataForm == _T("Magnitude"))
    {
        m_nDataForm = MAGNITUDE;
    }
    return E_NO_ERROR;
}

int CTiPrepController::rawCheckRecon(CString &f_csStudyFileName,int &f_nProtocolNo)
{
    TIPREP_TRACE(CTiPrepController::rawCheckRecon,MRTraceMarker3);
    msGetConfig_t  msConfig;
    status_t sts = E_NO_ERROR;
   // CTiPrepView::GetView()->SetWaitCursor(false);
    //AfxGetApp()->DoWaitCursor(-1); // -1->>remove the hourglass cursor
    AfxGetApp()->DoWaitCursor(1); // 1->>display the hourglass cursor
    msGetConfig(&msConfig);

    //RawCheck -start
    CReconRawCheck l_CrawCheck;
    CString l_csEngineName(msConfig.engineHostName);
    CString l_csStatusFilePath(RAWCHECK_STATUS_FILEPATH);
    if(FALSE == l_CrawCheck.RawCheck(
                l_csEngineName, 
                f_csStudyFileName, 
                f_nProtocolNo,
                l_csStatusFilePath)){
        AfxGetApp()->DoWaitCursor(-1); // -1->>remove the hourglass cursor
        return(E_RAW_CHECK_FAIL);

    }

    //RawCheck -end

    int l_nrawCheckStatus = E_NO_ERROR;
    sts = GetRawCheckStatusFromFile(l_nrawCheckStatus);
    if(E_NO_ERROR != sts){
        CString l_csRawCheckErr =_T("RawCheck status Reading Failed");
        CTiPrepLogHelper::WriteToErrorLog(l_csRawCheckErr, __LINE__,__FILE__);
        AfxGetApp()->DoWaitCursor(-1); // -1->>remove the hourglass cursor
        return(E_RAW_CHECK_FAIL);
    }else{
        //implies file reading was successful and l_nrawCheckStatus has rawCheck status read from file
        //which either E_NO_ERROR or E_ERROR
        AfxGetApp()->DoWaitCursor(-1); // -1->>remove the hourglass cursor
        return(l_nrawCheckStatus);
    }    

    AfxGetApp()->DoWaitCursor(-1); // -1->>remove the hourglass cursor
    return(E_NO_ERROR);

}

int CTiPrepController::validateSelectionSingleButton(int f_nprotocol_no,/*in*/CString & f_csStudyFileName)
{
    TIPREP_TRACE(CTiPrepController::validateSelectionSingleButton,MRTraceMarker3);
    try
    {
        //int l_nNumImgs = 0; // from selected LOIDs
        //CString l_csProtocolName, l_csTempStr;
        //int l_nProtocolNum = 0;
        //int l_nReProtocolNum = 0;
        //CLocalDBMgr localdb ;
        //int l_nProcNum_re = 0;
        //int l_nProcNum_img = 0;
        //vector<CString> l_csloid;
        vector<CString> l_csReloid;
        vector<CString> l_csImloid;

        int l_nStatus = E_NO_ERROR;
        imgProcessData_t l_imgData[3] = {{0, 0, 0, 0, NULL},
        {0, 0, 0, 0, NULL},
        {0, 0, 0, 0, NULL}
        };
        int l_nDataFlag[3] = {0};
        CvfcDBMgr l_vfcDb;
        VfDBHandle_t  l_vfd;
        l_nStatus = l_vfcDb.OpenDBFile(f_csStudyFileName,l_vfd);
        if(E_NO_ERROR != l_nStatus) {
            return E_PHASE_CORRECTION;
        }

        l_nStatus = l_vfcDb.protTraverse(f_nprotocol_no, 1, l_imgData, l_nDataFlag,l_vfd);
        
        if(E_NO_ERROR != l_nStatus) {
            for(int l_nCnt = 0; l_nCnt < 3; l_nCnt++) {
                if(NULL != l_imgData[l_nCnt].convPolImgs) {
                    free(l_imgData[l_nCnt].convPolImgs);
                }
            }
            
            return E_PHASE_CORRECTION;
        }
        
        l_nStatus = m_phasecorr->checkSelection(l_imgData);
        
        if(E_NO_ERROR != l_nStatus) {
            //error logging n
            return E_VALIDATION_FAILED;
        }
        
        vector <CString> l_vcsMagImgLoid;
        
        VfPathElem_t l_ImgPath[4] = {
            {SVN_ST_PROTOCOL,   0},
            {SVN_ST_PROCESSED,  0},
            {SVN_ST_ECHO,       0},
            {SVN_ST_POSITION,   0}
        };
        CString l_csImageLoid;
        
        for(int l_nCnt = 0; l_nCnt < l_imgData[0].nImages; l_nCnt++) {
            //real
            l_csImageLoid.Format(_T(""));
            l_ImgPath[0].index = l_imgData[0].convPolImgs[l_nCnt].protocol[1];
            l_ImgPath[1].index = l_imgData[0].convPolImgs[l_nCnt].processed[1];
            l_ImgPath[2].index = l_imgData[0].convPolImgs[l_nCnt].echo[1];
            l_ImgPath[3].index = l_imgData[0].convPolImgs[l_nCnt].image[1];
            l_nStatus = l_vfcDb.GetVfMapID(l_ImgPath, l_csImageLoid,l_vfd);
            if(E_NO_ERROR != l_nStatus){
                CTiPrepLogHelper::WriteToErrorLog(_T("Unable to get MapID."), ERROR_LOGONLY);
                for(int l_nCnt = 0; l_nCnt < 3; l_nCnt++) {
                    if(NULL != l_imgData[l_nCnt].convPolImgs) {
                        free(l_imgData[l_nCnt].convPolImgs);
                    }
            }
                return E_PHASE_CORRECTION;
            }
            if(l_csImageLoid == _T("")){
                CTiPrepLogHelper::WriteToErrorLog(_T("MapID is blank"), ERROR_LOGONLY);
                for(int l_nCnt = 0; l_nCnt < 3; l_nCnt++) {
                    if(NULL != l_imgData[l_nCnt].convPolImgs) {
                        free(l_imgData[l_nCnt].convPolImgs);
                    }
            }
                return E_PHASE_CORRECTION;
                
            }
            l_csReloid.push_back(l_csImageLoid);
            l_csImageLoid.Format(_T(""));
            //Magnitude
            l_ImgPath[0].index = l_imgData[1].convPolImgs[l_nCnt].protocol[1];
            l_ImgPath[1].index = l_imgData[1].convPolImgs[l_nCnt].processed[1];
            l_ImgPath[2].index = l_imgData[1].convPolImgs[l_nCnt].echo[1];
            l_ImgPath[3].index = l_imgData[1].convPolImgs[l_nCnt].image[1];
            l_nStatus = l_vfcDb.GetVfMapID(l_ImgPath, l_csImageLoid,l_vfd);
            if(E_NO_ERROR != l_nStatus){
                CTiPrepLogHelper::WriteToErrorLog(_T("Unable to get MapID."), ERROR_LOGONLY);
                for(int l_nCnt = 0; l_nCnt < 3; l_nCnt++) {
                    if(NULL != l_imgData[l_nCnt].convPolImgs) {
                        free(l_imgData[l_nCnt].convPolImgs);
                    }
            }
                return E_PHASE_CORRECTION;
            }
            if(l_csImageLoid == _T("")){
                CTiPrepLogHelper::WriteToErrorLog(_T("MapID is blank"), ERROR_LOGONLY);
                for(int l_nCnt = 0; l_nCnt < 3; l_nCnt++) {
                    if(NULL != l_imgData[l_nCnt].convPolImgs) {
                        free(l_imgData[l_nCnt].convPolImgs);
                    }
            }
                return E_PHASE_CORRECTION;            
            }
            l_vcsMagImgLoid.push_back(l_csImageLoid);
            l_csImageLoid.Format(_T(""));
            //Imaginary
            l_ImgPath[0].index = l_imgData[2].convPolImgs[l_nCnt].protocol[1];
            l_ImgPath[1].index = l_imgData[2].convPolImgs[l_nCnt].processed[1];
            l_ImgPath[2].index = l_imgData[2].convPolImgs[l_nCnt].echo[1];
            l_ImgPath[3].index = l_imgData[2].convPolImgs[l_nCnt].image[1];
            l_nStatus = l_vfcDb.GetVfMapID(l_ImgPath, l_csImageLoid,l_vfd);
            if(E_NO_ERROR != l_nStatus){
                CTiPrepLogHelper::WriteToErrorLog(_T("Unable to get MapID."), ERROR_LOGONLY);
                for(int l_nCnt = 0; l_nCnt < 3; l_nCnt++) {
                    if(NULL != l_imgData[l_nCnt].convPolImgs) {
                        free(l_imgData[l_nCnt].convPolImgs);
                    }
            }
                return E_PHASE_CORRECTION;
            }
            if(l_csImageLoid == _T("")){
                CTiPrepLogHelper::WriteToErrorLog(_T("MapID is blank"), ERROR_LOGONLY);
                for(int l_nCnt = 0; l_nCnt < 3; l_nCnt++) {
                    if(NULL != l_imgData[l_nCnt].convPolImgs) {
                        free(l_imgData[l_nCnt].convPolImgs);
                    }
            }
                return E_PHASE_CORRECTION;            
            }
            l_csImloid.push_back(l_csImageLoid);
        }
        
        m_phasecorr->SetImgnSeriesLOID(/*l_imgData[0].nImages,*/ l_csImloid);
        m_phasecorr->SetRealSeriesLOID(/*l_imgData[0].nImages,*/ l_csReloid);
        m_phasecorr->setImgInfo(l_imgData[0].nImages, l_vcsMagImgLoid);
        l_nStatus = l_vfcDb.CloseDBFile(l_vfd);
        if (E_NO_ERROR != l_nStatus)
        {
            CTiPrepLogHelper::WriteToErrorLog(_T("VFC close error"), ERROR_LOGONLY);
            for(int l_nCnt = 0; l_nCnt < 3; l_nCnt++) {
                if(NULL != l_imgData[l_nCnt].convPolImgs) {
                    free(l_imgData[l_nCnt].convPolImgs);
                }
            }
            return E_PHASE_CORRECTION;
        }
        l_nStatus = SeriesPhaseCorrection();
        if (E_NO_ERROR != l_nStatus)
        {
            CTiPrepLogHelper::WriteToErrorLog(_T(" SeriesPhaseCorrection error"), __LINE__, __FILE__);
            CTiPrepView::GetView()->DisplayErr(l_nStatus);
            BOOL l_bStattusFlag = TRUE;
            CTiPrepView::GetView()->enableOneButtonPC(l_bStattusFlag);
        } else {
            BOOL l_bStattusFlag = TRUE;
            CTiPrepView::GetView()->enableOneButtonPC(l_bStattusFlag);
            CErrorDlg l_err_dlg;
            CString l_csTemp(_T("Phase Correction successfully completed."));
			l_err_dlg.SetErrorMessage(l_csTemp);
            l_csTemp.Empty();
            l_csTemp = (_T("Phase Correction"));
			l_err_dlg.SetDlgTitle(l_csTemp);
			l_err_dlg.DoModal();
        }
        for(l_nCnt = 0; l_nCnt < 3; l_nCnt++) {
            if(NULL != l_imgData[l_nCnt].convPolImgs) {
                free(l_imgData[l_nCnt].convPolImgs);
            }
        }
    }
    catch (CMemoryException & e)
    {
        TCHAR   szCause[255] = L"";
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Memory can not be allocated because of this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        return E_ERR_EXCEPTION;
    }
    catch (CFileException & e)
    {
        TCHAR   szCause[255] = L"";
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Memory can not be allocated because of this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        return E_ERR_EXCEPTION;
    }
    catch (CException & e)
    {
        TCHAR   szCause[255] = L"";
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Memory can not be allocated because of this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        return E_ERR_EXCEPTION;
    }catch(...){
        CTiPrepLogHelper::WriteToErrorLog(_T("Exception occurred in validateSelectionSingleButton"), __LINE__, __FILE__);
        return E_ERR_EXCEPTION;
    }
    return E_NO_ERROR;
}

/*************************************************************
* Method:    	CreateSeries
* Description: 	This function is used 
* Parameter: 	vector<CString> & imgLOIDs :
* Parameter: 	vector<CString > & outLOIDs :
* Returns:   	int
*************************************************************/
int CTiPrepController::CreateSeries(vector<CString> &imgLOIDs, vector<CString > &outLOIDs)
{
    TIPREP_TRACE(CTiPrepView::CreateSeries,MRTraceMarker3);
    CNodeCopy ndcpy;
    CString l_csProtocolName (_T("")),l_csTempStr (_T("")),l_csStudyFileName (_T(""));
    int l_nProtocolNum = 0;
    
    CLocalDBMgr localdb ;
    int l_nProcNum = 0;
    //vector<CString> l_csloid;
    if (imgLOIDs.size() < 0)
    {
        return I_PROPER_IMG;
    }
    //int nImgs = imgLOIDs.size();
    
    //char  **img_loids = {NULL};
    
    CString l_csproc (_T(""));
    int l_nstatus = E_NO_ERROR;
    
    try {
        CString f_csCompletepath;
        l_nstatus = localdb.GetProtocolNameFromImageLevelDB(imgLOIDs[0],l_csProtocolName,l_csproc,f_csCompletepath);//sb
        if (l_nstatus ==  E_NO_ERROR){
            l_csTempStr.Format(_T("%s"), l_csproc);
            int cnt = l_csTempStr.Find(_T(':'));
            l_csStudyFileName = l_csTempStr.Left(cnt);
            int l_nTotalCnt = l_csproc.GetLength();
            l_csTempStr = l_csTempStr.Right(l_nTotalCnt - cnt -2);
            CString vfpath = l_csTempStr;
            cnt = l_csTempStr.Find(_T('/'));
            vfpath = l_csTempStr.Right(cnt);
            l_csTempStr = l_csTempStr.Left(cnt);
            cnt = l_csTempStr.Find(_T('.'));
            l_csTempStr = l_csTempStr.Right(cnt);
            char l_cTempChar[10];
            sprintf(l_cTempChar, "%S", l_csTempStr);
            l_nProtocolNum = atoi(l_cTempChar);
            
            //AN/ Processed index
            cnt = vfpath.Find(_T('/'));
            cnt = vfpath.Find(_T('.'));
            l_csTempStr = vfpath.Right(cnt);
            sprintf(l_cTempChar, "%S", l_csTempStr);
            l_nProcNum = atoi(l_cTempChar);
        }    else {
            CTiPrepLogHelper::WriteToErrorLog(_T("GetProtocolNameFromImageLevelDB() returned Error."),__LINE__,__FILE__);
            return l_nstatus;
            
        } 
        int l_nstatus = 0;
        //int l_nimgngdckind = 0;
        //int l_nregdckind = 0;
        char l_cTempChar[256]= {0};
        sprintf(l_cTempChar, "%S", l_csStudyFileName);
        int newprocnode = 0;
        CString csseries_loid (_T(""));
        l_nstatus = localdb.CreateSeriesCopy(imgLOIDs,outLOIDs,csseries_loid);
        if (outLOIDs.size() <= 0)
        {
            CTiPrepLogHelper::WriteToErrorLog(_T("out loids from CreateSeriesCopy() is 0."),__LINE__,__FILE__);
            return l_nstatus;
        }
        l_nstatus = ndcpy.pdNodeCopy(l_cTempChar,l_nProtocolNum,l_nProtocolNum,l_nProcNum,&newprocnode);
        if (l_nstatus != E_NO_ERROR)
        {
            CTiPrepLogHelper::WriteToErrorLog(_T("pdNodeCopy() returned Error."),__LINE__,__FILE__);
            return l_nstatus;
        }
        //AN Below code sets reference of newly created vfc and localdb series in eacxh other, Note, without following code also, Synchornization error is solved
        //Set tag 00291051, 00291052 in sereies and image level of newly created series
        /*
        CString vfpath (_T(""));
        CString tmpvfpath = _T("");
        tmpvfpath.Format(_T("%s:"),l_csStudyFileName);
        vfpath.Format(_T("%s:/1.%d/7.%d"),l_csStudyFileName,l_nProtocolNum,newprocnode);
        l_nstatus= localdb.UpdateVFPathInDB(csseries_loid,(LPCTSTR)vfpath,LOID_TYPE_SERIES);
        if (l_nstatus != E_NO_ERROR)
        {
        CTiPrepLogHelper::WriteToErrorLog(_T("UpdateVFPathInDB() failed."),__LINE__,__FILE__);
        return l_nstatus;
        }
        CStringArray csarr;
        CString tmp (_T(""));
        CvfcDBMgr l_vfcDb;
        VfPathElem_t l_ImgPath[4] = {
        {SVN_ST_PROTOCOL,   l_nProtocolNum},
        {SVN_ST_PROCESSED,  newprocnode},
        {SVN_ST_ECHO,       0},
        {SVN_ST_POSITION,   0}
        };
        l_nstatus = l_vfcDb.OpenDBFile(l_csStudyFileName,true); //Read Write True
        if (l_nstatus != E_NO_ERROR)
        {
        CTiPrepLogHelper::WriteToErrorLog(_T("pdNodeCopy() returned Error."),__LINE__,__FILE__);
        return l_nstatus;
        }
        
          l_nstatus =  l_vfcDb.GetProcSubVFPaths(l_ImgPath,csarr);
          if (l_nstatus != E_NO_ERROR)
          {
          CTiPrepLogHelper::WriteToErrorLog(_T("GetProcSubVFPaths() failed."),__LINE__,__FILE__);
          return l_nstatus;
          }
          for (int i = 0; i < nImgs; i++)
          {
          tmp = tmpvfpath + csarr.GetAt(i);
          
            l_nstatus =  localdb.UpdateVFPathInDB(outLOIDs[i],tmp,LOID_TYPE_IMAGE);
            if (l_nstatus != E_NO_ERROR)
            {
            CTiPrepLogHelper::WriteToErrorLog(_T("UpdateVFPathInDB() failed."),__LINE__,__FILE__);
            return l_nstatus;
            }
            }
            
              //Update newly created LOIDs in vfc new series
              
                
                  char series_loid[16] = {0};
                  img_loids = new char*[imgLOIDs.size()];
                  if (img_loids == NULL)
                  {
                  CString l_cserr (_T("Memory allocation failed in CreateSeries()"));
                  CTiPrepLogHelper::WriteToErrorLog(l_cserr,__LINE__,__FILE__);
                  l_vfcDb.CloseDBFile();
                  return E_MEM_ALLOC;
                  }
                  char * stmp = "";
                  for (int j = 0; j <nImgs; j++ )
                  {
                  img_loids[j] = new char[16]();
                  if (img_loids[j] == NULL)
                  {
                  for (int m = 0; m < j; m++)
                  {
                  DEL_ARR_PTR(img_loids[j]);
                  }
                  DEL_ARR_PTR(img_loids);
                  CString l_cserr (_T("Memory allocation failed in CreateSeries()"));
                  CTiPrepLogHelper::WriteToErrorLog(l_cserr,__LINE__,__FILE__);
                  return E_MEM_ALLOC;
                  }
                  }
                  wcstombs(series_loid, csseries_loid, csseries_loid.GetLength());
                  for ( unsigned int k = 0; k < outLOIDs.size(); k++)
                  {
                  wcstombs(img_loids[k], outLOIDs.at(0), outLOIDs.at(0).GetLength() + 1);
                  }
                  
                    l_nstatus = l_vfcDb.UpdateLOIDForProcessNode(l_ImgPath,series_loid,img_loids,nImgs);
                    if (l_nstatus != E_NO_ERROR)
                    {
                    CTiPrepLogHelper::WriteToErrorLog(_T("UpdateLOIDForProcessNode() return fail"),__LINE__,__FILE__);
                    }
                    if (NULL != img_loids)
                    {
                    
                      for (int i = 0; i < nImgs ; i++)
                      {   
                      DEL_ARR_PTR(img_loids[i]);
                      }
                      DEL_ARR_PTR(img_loids[i]);
                      }
                      
        l_vfcDb.CloseDBFile();*/
    } catch(...)
    {
        CTiPrepLogHelper::WriteToErrorLog(_T("Exception occurred in CreateSeries()"),__LINE__,__FILE__);
        //     if (NULL != img_loids)
        //     {
        //         
        //         for (int i = 0; i < nImgs ; i++)
        //         {   
        //             DEL_ARR_PTR(img_loids[i]);
        //         }
        //         DEL_ARR_PTR(img_loids[i]);
        //     }
        return E_ERR_EXCEPTION;
    }
    return l_nstatus;
} 

/*************************************************************
* Method:    	InitializeIRS
* Description: 	This function is used 
* Returns:   	int
*************************************************************/
int CTiPrepController::InitializeIRS(){

    int l_nstatus = FALSE;
    //l_nstatus = m_IrsComm.InitializeIRS();

    return l_nstatus;
}

//AS commented //rawCheck status is now read from a file /start
int CTiPrepController::GetRawCheckStatusFromFile( int &f_nStatus )
{
    // reads the file written on redirection of rawCheck's output 
    TIPREP_TRACE(CTiPrepController::GetRawCheckStatusFromFile,MRTraceMarker3)
    FILE * fp = fopen(RAWCHECK_STATUS_FILEPATH,"r");
    
    try{            
        if (fp != NULL)
        {
            char buf[126];
            fgets(buf, sizeof(buf),fp);
            
            if(buf[0] == '0')
            {
				//TRACE COMMENTED
#ifdef _UT_LOG_
				CString l_csTempstr = _T("");
				l_csTempstr.Format(_T("Raw data is present"));
				CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker3, l_csTempstr, _T("CTiPrepController::GetRawCheckStatusFromFile"));
#endif

                //implies rawCheck exit status was success
                //i.e raw data is present
                f_nStatus = E_NO_ERROR;
                
            } else if(buf[0] == '1'){
				//TRACE COMMENTED
#ifdef _UT_LOG_
				CString l_csTempstr = _T("");
				l_csTempstr.Format(_T("Raw data is not present"));
				CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker3, l_csTempstr, _T("CTiPrepController::GetRawCheckStatusFromFile"));
#endif
                //implies rawCheck exit status was failure
                //raw data is not present
                f_nStatus = E_NO_RAW;
            } else {
                CString l_csTempstr = _T("");
                l_csTempstr.Format(_T("Raw Check return invalid status. Study file may not be present at engine"));
                CTiPrepLogHelper::WriteToErrorLog(l_csTempstr,__LINE__,__FILE__);
                l_csTempstr = _T("error message from rawchk process: \n");
                do {
                l_csTempstr += buf;
                l_csTempstr += _T("\n");
                }
                while ( NULL != fgets(buf, sizeof(buf),fp));
                CTiPrepLogHelper::WriteToErrorLog(l_csTempstr,__LINE__,__FILE__);
                f_nStatus = E_RAW_CHECK_FAIL;
            }
        }else{            
            CString l_csTempstr = _T("");
            l_csTempstr.Format(_T("Error Opening rawCheckStatus file"));
                CTiPrepLogHelper::WriteToErrorLog(l_csTempstr,__LINE__,__FILE__);
            f_nStatus = E_RAW_CHECK_FAIL;
            return(E_ERROR);
        }   
        //TRACE COMMENTED
#ifdef _UT_LOG_
        CString l_csTempstr = _T("");
        l_csTempstr.Format(_T("rawCheck Status read from file is : %d"),f_nStatus);
        CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker3, l_csTempstr, _T("CTiPrepController::GetRawCheckStatusFromFile"));
#endif
        fclose(fp);
        DeleteRawCheckStatusFile();
        return(E_NO_ERROR);

    }catch(...)
    {
        CTiPrepLogHelper::WriteToErrorLog(_T("Exception occurred in GetRawCheckStatusFromFile()"),__LINE__,__FILE__);
        f_nStatus = E_ERROR;        
        if(fp){
            fclose(fp);
        };        
        return(E_ERROR);
    }
}

int CTiPrepController::DeleteRawCheckStatusFile()
{
    // deletes the file written on redirection of rawCheck's output 
    TIPREP_TRACE(CTiPrepController::DeleteRawCheckStatusFile,MRTraceMarker3)

        CString l_csFilePath(_T(""));
        l_csFilePath.Format(_T("%S"),RAWCHECK_STATUS_FILEPATH);

        if(FALSE == SetFileAttributes(l_csFilePath, FILE_ATTRIBUTE_NORMAL)) {
            CTiPrepLogHelper::WriteToErrorLog(_T("DeleteRawCheckStatusFile(): RAWCHECK_STATUS_FILEPATH SetFileAttributes Failed"), __LINE__);
        }
        
        if(!(DeleteFile(l_csFilePath))) { // failed to delete
            CString l_csErrMsg(_T(""));
            l_csErrMsg.Format(_T("Failed to unlink file : %S.File may not be updated"), RAWCHECK_STATUS_FILEPATH);
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__);
        } else { // succeeded
			//TRACE COMMENTED
#ifdef _UT_LOG_
            CString l_csErrMsg(_T(""));
            l_csErrMsg.Format(_T("File %S deleted successfully"), RAWCHECK_STATUS_FILEPATH);
            CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker3, l_csErrMsg);
#endif
    }
        return(E_NO_ERROR);        
}

//AS commented //rawCheck status is now read from a file /end
///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////
//AN scanner
int CTiPrepController::ExtractPagerInfo(CString &csParam)
{
	
				int index = csParam.Find(_T("Acquire"));
				CString tempstr = csParam.Right(csParam.GetLength() - index);
				index = tempstr.Find(_T(" "));
				tempstr = tempstr.Left(index);
				index = tempstr.Find(_T("="));
				CString acqstudyloid = tempstr.Right(tempstr.GetLength() - index - 1);
				
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
				CString csparent = CString(CDPSComManager::GetInstance()->GetParentName());
				if (acqstudyloid.CompareNoCase(disp1studyloid) == 0 && csparent.CompareNoCase(_T("Display1")) == 0 )
				{
					m_csStudyLoid = disp1studyloid;
					
					//MessageBox(CString(m_dps_com_manager->m_parent_name));
				} else if (acqstudyloid.CompareNoCase(disp2studyloid) == 0 && csparent.CompareNoCase(_T("Display2")) == 0) 
				{
					m_csStudyLoid = disp2studyloid;
					//MessageBox(CString(m_dps_com_manager->m_parent_name));
					
				} else {
					//	msg = _T("Study is differnt. No Connection to PQM");
					//no connections
					//for pqm it should return error
				}
				if (!m_csStudyLoid.IsEmpty())
				{
					isConnectedToPQM = E_START_CONNECTION;
					CWnd * wnd = CTiPrepView::GetView()->GetCTiPrepDlgInstance();
					wnd->GetDlgItem(IDC_BUTTON_APPLYTI)->EnableWindow(TRUE);
					CDPSComManager *l_server = CDPSComManager::GetInstance();
					CString target = _T("Acquire.PQM");
					l_server->Connect(target);
					IMplusMessage *l_msg = NEW_MSG();;
					l_msg->SetCommand(_T("REGISTER_TIPREP_TO_PQM"));
					l_msg->SetParameter(_bstr_t(m_csStudyLoid));
					l_msg->SetCustom(_T(""));
					CString sender = l_server->GetParentName();
					sender+= ".";
					sender+= "TiPrep";
					l_msg->SetSender(_bstr_t(sender));
					l_server->SendMsg(l_msg);
					DEL_MSG(l_msg);
					l_server->Disconnect();
					
					//send RegisterGL to PQM
				} 
				return 0;
}

int CTiPrepController::GetPagerInfo()
{
	CDPSComManager *l_server = CDPSComManager::GetInstance();
			 //l_target = l_server->GetParentName();
	// AfxMessageBox(l_target);
	CString target = _T("Pager");
			 l_server->Connect(target);
			 //  l_server->Connect(/*l_branch_boss*/l_target);
			 IMplusMessage *l_msg = NEW_MSG();;
			 l_msg->SetCommand(_T("GL_Pager_GetPageInfo"));
			 l_msg->SetParameter("");
			 l_msg->SetCustom(_T(""));
			 CString sender = l_server->GetParentName();
			 sender+= ".";
			 sender+= "TiPrep";
			 l_msg->SetSender(_bstr_t(sender));
			 l_server->SendMsg(l_msg);
			 DEL_MSG(l_msg);
			 l_server->Disconnect();
			 return 0;
}

int CTiPrepController::OnApplyTI(int prot,float ti)
{
	if (isConnectedToPQM != E_NO_CONNECTION){
		if (!m_tiupdater)
			m_tiupdater = new CTiProlimUpdater();
		CLocalDBMgr localdb;
		CString studyname;
		localdb.GetStudyNameFromStudyLoid(m_csStudyLoid,studyname);
		if (m_tiupdater)m_tiupdater->SetData(prot,ti,studyname);
		CDPSComManager *l_server = CDPSComManager::GetInstance();
		//l_target = l_server->GetParentName();
		// AfxMessageBox(l_target);
		CString target = _T("Acquire.PQM");
		l_server->Connect(target);
		//  l_server->Connect(/*l_branch_boss*/l_target);
		IMplusMessage *l_msg = NEW_MSG();;
		l_msg->SetCommand(_T("CAN_TIPREP_UPDATE"));
		CString param ;
		param.Format(_T("Study=%s,Protocol=%d"),m_csStudyLoid,prot);
		l_msg->SetParameter(_bstr_t(param));
		l_msg->SetCustom(_T(""));
		CString sender = l_server->GetParentName();
		sender+= ".";
		sender+= "TiPrep";
		l_msg->SetSender(_bstr_t(sender));
		l_server->SendMsg(l_msg);
		DEL_MSG(l_msg);
		l_server->Disconnect();

	}
	return 0;
}

void CTiPrepController::SendTiPrepCloseToPQM()
{
	if (isConnectedToPQM != E_NO_CONNECTION)
	{
		CDPSComManager *l_server = CDPSComManager::GetInstance();
		//l_target = l_server->GetParentName();
		// AfxMessageBox(l_target);
		CString target = _T("Acquire.PQM");
		l_server->Connect(target);
		//  l_server->Connect(/*l_branch_boss*/l_target);
		IMplusMessage *l_msg = NEW_MSG();;
		l_msg->SetCommand(_T("TIPREP_CONNECTION_CLOSE"));
		l_msg->SetParameter(_bstr_t(m_csStudyLoid));
		l_msg->SetCustom(_T(""));
		CString sender = l_server->GetParentName();
		sender+= ".";
		sender+= "TiPrep";
		l_msg->SetSender(_bstr_t(sender));
		l_server->SendMsg(l_msg);
		DEL_MSG(l_msg);
		l_server->Disconnect();
	}
}

int CTiPrepController::UpdateTi(bool flag)
{
	if (!flag)
	{
		//show error message that exit locator etc 
		return 1;
	}
int	status = E_ERROR;
	if (m_tiupdater) {
	status=	m_tiupdater->ProlimTi();
	}
	if (isConnectedToPQM != E_NO_CONNECTION)
	{
		CDPSComManager *l_server = CDPSComManager::GetInstance();
		//l_target = l_server->GetParentName();
		// AfxMessageBox(l_target);
		CString target = _T("Acquire.PQM");
		l_server->Connect(target);
		//  l_server->Connect(/*l_branch_boss*/l_target);
		IMplusMessage *l_msg = NEW_MSG();;
		l_msg->SetCommand(_T("TIPREP_UPDATE_COMPLETED"));
		l_msg->SetParameter(_bstr_t(m_csStudyLoid));
		l_msg->SetCustom(_T(""));
		CString sender = l_server->GetParentName();
		sender+= ".";
		sender+= "TiPrep";
		l_msg->SetSender(_bstr_t(sender));
		l_server->SendMsg(l_msg);
		DEL_MSG(l_msg);
		l_server->Disconnect();
	}
		 MessageBox(NULL,NULL,_T("updated ti"),NULL);


return status;
}

void CTiPrepController::RefreshApplyTiGUI()
{
    if(m_ApplyTiDlg){
        m_ApplyTiDlg->RefreshGUI();
    }
}

void CTiPrepController::DisplayApplyTiDlg()
{
    if(m_ApplyTiDlg){
       m_ApplyTiDlg->ShowWindow(SW_SHOW);
    }

    CTiPrepView::GetView()->AddZOrderLayer();
    CTiPrepView::GetView()->RegisterWindowZOrder(m_ApplyTiDlg->GetSafeHwnd());   
}

int CTiPrepController::GetApplyTiLicense( BOOL &f_nLicense )
{

        CString l_ini_file_name(_T(""));
        l_ini_file_name = getenv("MRMP_Config");
        l_ini_file_name += _T("\\PQM");
        l_ini_file_name += _T("\\PQM_TiPrep_License.ini");
        
        if(PathFileExists(l_ini_file_name)) {
            CIniReader l_ini_file_name(l_ini_file_name);
            CString l_cspwdRead = l_ini_file_name.GetKeyValue(_T("TiPrep_License"), _T("TiPrep"));
            
            if(l_cspwdRead.CompareNoCase(_T("ON")) == 0) {
                f_nLicense = TRUE;
            } else {
                f_nLicense = FALSE;
            }
        }else{
            f_nLicense = FALSE;
        }

        CString l_csDbgStr(_T(""));
        l_csDbgStr.Format(_T("in GetApplyTiLicense lic = %d"),f_nLicense);
        CTiPrepLogHelper::WriteToErrorLog(l_csDbgStr);
        return 0;
}

//AN scanner

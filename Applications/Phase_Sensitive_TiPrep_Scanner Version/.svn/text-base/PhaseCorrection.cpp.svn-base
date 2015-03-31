// PhaseCorrection.cpp: implementation of the CPhaseCorrection class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#include "stdafx.h"
#include "tiprep.h"
#include "PhaseCorrection.h"
#include "TiPrepLogHelper.h"
#include "TiPrepTrace.h"

#include <tami/vfStudy/vf_appcodes.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define SHORT 16
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//****************************Method Header***************************
//Method Name   : CPhaseCorrection
//Purpose       : Constructor
//********************************************************************
CPhaseCorrection::CPhaseCorrection():
m_nNum_Images_selected(0),
m_selectd_image_dataform(0)
{
    m_csImageLoid.clear();
    m_csRealLoid.clear();  
    m_csImgnLoid.clear();  
}
//****************************Method Header***************************
//Method Name   : CPhaseCorrection::~CPhaseCorrection()
//Purpose       : Destructor
//********************************************************************
CPhaseCorrection::~CPhaseCorrection()
{
    m_csImageLoid.clear(); 
    m_csRealLoid.clear();  
    m_csImgnLoid.clear();  
}
CPhaseCorrection::CPhaseCorrection(CPhaseCorrection & f_ptr)
{
    /*TIPREP_TRACE(CPhaseCorrection::CPhaseCorrection);*/

    if(f_ptr.m_csImageLoid.size() > 0) {
        setImgInfo(f_ptr.m_csImageLoid.size(), f_ptr.m_csImageLoid);
    }
    m_selectd_image_dataform = f_ptr.m_selectd_image_dataform;
    m_nNum_Images_selected = f_ptr.m_nNum_Images_selected;
}
//****************************Method Header***************************
//Method Name   : CPhaseCorrection::~CPhaseCorrection()
//Purpose       : Overrider
//********************************************************************
CPhaseCorrection& CPhaseCorrection::operator = (CPhaseCorrection& f_ptr)
{
   /* TIPREP_TRACE(CTiPrepView::operator);*/

    if(this == &f_ptr) {
        return(*this);
    }

    if(f_ptr.m_csImageLoid.size() > 0) {
        setImgInfo(f_ptr.m_csImageLoid.size(), f_ptr.m_csImageLoid);
    }
    m_selectd_image_dataform = f_ptr.m_selectd_image_dataform;
    m_nNum_Images_selected = f_ptr.m_nNum_Images_selected;
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
// Member Functions
///////////////////

/*************************************************************
* Method:       setImgInfo
* Description:  This function set number of images selected by
*               user on ISelector and stores image LOID
*               for each image.
* Parameter:    long f_lnNum_img
*               Contains number of images select on ISelector.
* Parameter:    vector <CString> & f_csImgLOID
*               Contains LOID for each image.
* Returns:      void
*************************************************************/
void CPhaseCorrection::setImgInfo(long f_lnNum_img, /*in*/vector <CString> &f_csImgLOID)
{
    TIPREP_TRACE(CPhaseCorrection::setImgInfo,MRTraceMarker3);
    
    try{
        m_nNum_Images_selected = f_lnNum_img;
        vector<CString>::iterator l_csit1;
        m_csImageLoid.clear();
        l_csit1 = f_csImgLOID.begin();
        
        for(l_csit1 = f_csImgLOID.begin() ; l_csit1 < f_csImgLOID.end(); l_csit1++) {
            m_csImageLoid.push_back(*l_csit1);
        }
    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog
            (_T("Exception occurred in Phase correction Class"), __LINE__, __FILE__);
    }
}

/*************************************************************
* Method:       getSelImgLOID
* Description:  This function gives LOID of images selected on
*               ISelector.
* Parameter:    vector <CString> & f_csImgLOID
*               Out parameter to get LOID of selected images on
*               ISelector.
* Returns:      void
*************************************************************/
void CPhaseCorrection:: getSelImgLOID(vector <CString> &f_csImgLOID)
{
    TIPREP_TRACE(CPhaseCorrection::getSelImgLOID,MRTraceMarker3);
    
    try{
        vector<CString>::iterator l_csit1;
        l_csit1 = m_csImageLoid.begin();
        f_csImgLOID.clear();
        
        for(l_csit1 = m_csImageLoid.begin() ; l_csit1 < m_csImageLoid.end(); l_csit1++) {
            f_csImgLOID.push_back(*l_csit1);
        }
    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog
            (_T("Exception occurred in Phase correction Class"), __LINE__, __FILE__);
    }
}

/*************************************************************
* Method:       SetRealSeriesLOID
* Description:  This function is used
* Parameter:    long f_lnNum_img
* Parameter:    vector <CString> & f_csImgLOID
* Returns:      void
*************************************************************/
void CPhaseCorrection::SetRealSeriesLOID(/*long f_lnNum_img,*/ vector <CString> &f_csImgLOID)
{
    TIPREP_TRACE(CPhaseCorrection::SetRealSeriesLOID,MRTraceMarker3);
    
    try{
        vector<CString>::iterator l_csit1;
        m_csRealLoid.clear();
        l_csit1 = f_csImgLOID.begin();
        
        for(l_csit1 = f_csImgLOID.begin() ; l_csit1 < f_csImgLOID.end(); l_csit1++) {
            m_csRealLoid.push_back(*l_csit1);
        }
    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog
            (_T("Exception occurred in Phase correction Class"), __LINE__, __FILE__);
    }
}
/*************************************************************
* Method:       SetImgnSeriesLOID
* Description:  This function is used
* Parameter:    long f_lnNum_img
* Parameter:    vector <CString> & f_csImgLOID
* Returns:      void
*************************************************************/
void CPhaseCorrection::SetImgnSeriesLOID(/*long f_lnNum_img,*/ vector <CString> &f_csImgLOID)
{
    TIPREP_TRACE(CPhaseCorrection::SetImgnSeriesLOID,MRTraceMarker3);
    
    try{
        vector<CString>::iterator l_csit1;
        m_csImgnLoid.clear();
        l_csit1 = f_csImgLOID.begin();
        
        for(l_csit1 = f_csImgLOID.begin() ; l_csit1 < f_csImgLOID.end(); l_csit1++) {
            m_csImgnLoid.push_back(*l_csit1);
        }
    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog
            (_T("Exception occurred in Phase correction Class"), __LINE__, __FILE__);
    }
}
/*************************************************************
* Method:       getRealImgLOID
* Description:  This function is used
* Parameter:    vector <CString> & f_csImgLOID
* Returns:      void
*************************************************************/
void CPhaseCorrection:: getRealImgLOID(vector <CString> &f_csImgLOID)
{
    TIPREP_TRACE(CPhaseCorrection::getSelImgLOID,MRTraceMarker3);
    
    try{
        vector<CString>::iterator l_csit1;
        l_csit1 = m_csRealLoid.begin();
        f_csImgLOID.clear();
        
        for(l_csit1 = m_csRealLoid.begin() ; l_csit1 < m_csRealLoid.end(); l_csit1++) {
            f_csImgLOID.push_back(*l_csit1);
        }
        
    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog
            (_T("Exception occurred in Phase correction Class"), __LINE__, __FILE__);
    }
}
/*************************************************************
* Method:       getImgnImgLOID
* Description:  This function is used
* Parameter:    vector <CString> & f_csImgLOID
* Returns:      void
*************************************************************/
void CPhaseCorrection:: getImgnImgLOID(vector <CString> &f_csImgLOID)
{
    TIPREP_TRACE(CPhaseCorrection::getSelImgLOID,MRTraceMarker3);
    
    try{
        vector<CString>::iterator l_csit1;
        l_csit1 = m_csImgnLoid.begin();
        f_csImgLOID.clear();
        
        for(l_csit1 = m_csImgnLoid.begin() ; l_csit1 < m_csImgnLoid.end(); l_csit1++) {
            f_csImgLOID.push_back(*l_csit1);
        }
        
    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog
            (_T("Exception occurred in Phase correction Class"), __LINE__, __FILE__);
    }
}

/*************************************************************
* Method:    	checkSelection
* Description: 	This function is used 
* Parameter: 	imgProcessData_t * f_imgData :
* Returns:   	status_t
*************************************************************/
status_t CPhaseCorrection:: checkSelection(imgProcessData_t *f_imgData)
{
    status_t l_nstatus = E_NO_ERROR;
    //int l_nImgs = 0;
    TIPREP_TRACE(CPhaseCorrection::checkSelection,MRTraceMarker3);
    
    try{
        if(NULL == f_imgData){
            CString lcserrmsg(_T("f_imgData is NULL"));
            CTiPrepLogHelper::WriteToErrorLog(lcserrmsg, __LINE__, __FILE__);
            l_nstatus = E_ERROR;
            return(l_nstatus);
        }
        if(f_imgData[0].dataform  != VFC_FORM_REAL_ONLY
            && f_imgData[1].dataform  != VFC_FORM_MAGNITUDE
            && f_imgData[2].dataform  != VFC_FORM_IMGN_ONLY) {
            CString lcserrmsg(_T("checkSelection() Invalid Data form selected"));
            CTiPrepLogHelper::WriteToErrorLog(lcserrmsg, __LINE__, __FILE__);
            l_nstatus = E_ERROR;
            return(l_nstatus);
        }
        
        if(f_imgData[0].nImages == 0 ||
            f_imgData[1].nImages == 0 || f_imgData[2].nImages == 0) {
            CString lcserrmsg(_T("checkSelection() No images selected error"));
            CTiPrepLogHelper::WriteToErrorLog(lcserrmsg, __LINE__, __FILE__);
            l_nstatus = E_ERROR;
            return(l_nstatus);
        } else {
            l_nstatus = E_NO_ERROR;
        }
        
        if(f_imgData[0].nImages == f_imgData[1].nImages &&
            f_imgData[1].nImages == f_imgData[2].nImages) {
            l_nstatus = E_NO_ERROR;
        } else {
            CString lcserrmsg(_T("checkSelection() Number of images selected in real and imaginary are different"));
            CTiPrepLogHelper::WriteToErrorLog(lcserrmsg, __LINE__, __FILE__);
            l_nstatus = E_ERROR;
            return(l_nstatus);
        }
        
        if(f_imgData[0].imgGDCtype == f_imgData[1].imgGDCtype &&
            f_imgData[1].imgGDCtype == f_imgData[2].imgGDCtype) {
            l_nstatus = E_NO_ERROR;
        } else {
            printf("E_GDC_DIFF error\n");
            CString lcserrmsg(_T("checkSelection() GDC kind in real and imaginary are different"));
            CTiPrepLogHelper::WriteToErrorLog(lcserrmsg, __LINE__, __FILE__);
            l_nstatus = E_ERROR;
            return(l_nstatus);
        }
        
        for(int l_nImgs = 0; l_nImgs < f_imgData[1].nImages; l_nImgs++) {
            if(f_imgData[0].convPolImgs[l_nImgs].protocol[1] != f_imgData[1].convPolImgs[l_nImgs].protocol[1] &&
                f_imgData[1].convPolImgs[l_nImgs].protocol[1] != f_imgData[2].convPolImgs[l_nImgs].protocol[1]) {
                printf("E_PROT_DIFF error.\n");
                CString lcserrmsg(_T("checkSelection() real and imaginary series are of different protocol"));
                CTiPrepLogHelper::WriteToErrorLog(lcserrmsg, __LINE__, __FILE__);
                l_nstatus = E_ERROR;
                break;
            }
        }
        
        return(l_nstatus);
    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog
            (_T("Exception occurred in Phase correction Class"), __LINE__, __FILE__);
        return E_ERROR;
    }
}

///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////
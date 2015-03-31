// DcmController.cpp: implementation of the CDcmController class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "planduplicator.h"
#include "DcmController.h"
#include "PDTrace.h"
#include "PDLogHelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] =__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDcmController::CDcmController()
{
    PD_TRACE(CDcmController::CDcmController);
}

CDcmController::~CDcmController()
{
    PD_TRACE(CDcmController::~CDcmController);
}

/*************************************************************
* Method:    	startDICOMConversion
* Description: 	This function is used 
* Parameter: 	const char * f_cstudyName :
* Parameter: 	const int & f_nportIndex :
* Parameter: 	const CString & f_Dicom_filepath :
* Returns:   	int
*************************************************************/
int CDcmController::startDICOMConversion(const char *f_cstudyName,
                                         const int& f_nportIndex,
                                         const CString& f_Dicom_filepath
                                         )
{
    PD_TRACE(CDcmController::startDICOMConversion);
    if (NULL == f_cstudyName)
    {
        CString l_csErrMsg = ErrorList[PD_PARAMETER_INVALID];
        CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
        return(E_ERROR);
    }
    int l_nTotalNumImg = 0;
    CDBWrapper l_dbWrapper;
    vector<CString> l_csFileName;
    char l_csfinalPath[BUFFER_SIZE_160] = {0};
    int l_nstatus = E_NO_ERROR;
    
    l_nstatus = l_dbWrapper.getTotalNumImages(f_cstudyName,
        f_nportIndex,l_csFileName,&l_nTotalNumImg);
    if(E_ERROR == l_nstatus){
        CString l_csErrMsg = _T("Failed to get image information for conversion"); //AN
        CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__,ERRORVIEW_DISPLAY);
        return(l_nstatus);
    }
    if (0 >= l_nTotalNumImg)
    {
        CString l_csErrMsg = _T("Function getTotalNumImages() total images is 0");
        CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
        return(E_ERROR);
    }
    CString l_csRefStr = "";
    CPDDicomConverter l_dcmconverter;
    int l_ncnt = 0;
    for(l_ncnt = 0;l_ncnt < l_nTotalNumImg;l_ncnt++){
        
        l_csRefStr = l_csFileName.at(l_ncnt);
        
        fragmentationStr(l_csRefStr);
        CString l_csfilePath(f_Dicom_filepath);
        CImageInfo l_imginfo = m_vImageInfo.at(l_ncnt);
        
        l_csfilePath += l_imginfo.getDcmFileName();
        
        wcstombs(l_csfinalPath,l_csfilePath,BUFFER_SIZE_160);
        l_nstatus = l_dcmconverter.WriteInDICOMFile(l_csfinalPath,&l_imginfo);
        if (E_NO_ERROR != l_nstatus)
        {
            return (E_ERROR);
           // l_nstatus = l_dcmconverter.GetDcmErrorcode();
        }
        //codereview..21jun12..use return value for error logging
//         if ( (0 < l_nstatus) && (l_nstatus < PD_PROC_ERROR) )
//         {
//             CString l_csErrMsg (_T(""));
//             l_csErrMsg = ErrorList[l_nstatus];
//             CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__,ERRORVIEW_DISPLAY); //AN
//             return(E_ERROR);
//         }
    }
    //AN/ Error Handling
    
    return(E_NO_ERROR);
}

/*************************************************************
* Method:    	getImageInfo
* Description: 	This function is used 
* Parameter: 	const int & f_nImgNum :
* Returns:   	const CImageInfo
*************************************************************/
const CImageInfo CDcmController:: getImageInfo(const int& f_nImgNum)
{
    PD_TRACE(CDcmController::getImageInfo);
    return(m_vImageInfo.at(f_nImgNum));
}

/*************************************************************
* Method:    	setImageInfo
* Description: 	This function is used 
* Parameter: 	CImageInfo & val :
* Returns:   	void
*************************************************************/
void CDcmController::setImageInfo(CImageInfo& val)
{ 
    PD_TRACE(CDcmController::setImageInfo);
    m_vImageInfo.push_back(val);
}

/*************************************************************
* Method:    	fragmentationStr
* Description: 	This function is used 
* Parameter: 	CString & f_csRefStr :
* Returns:   	int
*************************************************************/
int CDcmController::fragmentationStr(CString& f_csRefStr)
{
    PD_TRACE(CDcmController::fragmentationStr);
    CImageInfo l_sImageInfo;
    CString l_csTempStr;
    int l_nIndex = f_csRefStr.Find(' ');
    CString l_csFileName = f_csRefStr.Left(l_nIndex);
    
    l_csTempStr = f_csRefStr.Right((f_csRefStr.GetLength())-(l_nIndex+1));
    l_nIndex = l_csTempStr.Find(' ');
    CString l_csStudyLOID = l_csTempStr.Left(l_nIndex);
    
    l_csTempStr = l_csTempStr.Right((l_csTempStr.GetLength())-(l_nIndex+1));
    l_nIndex = l_csTempStr.Find(' ');
    CString l_csSeriesLOID = l_csTempStr.Left(l_nIndex);
    
    l_csTempStr = l_csTempStr.Right((l_csTempStr.GetLength())-(l_nIndex+1));
    CString l_csPositionLOID = l_csTempStr;
    
    l_sImageInfo.setDcmFileName(l_csFileName);
    l_sImageInfo.setStudyLOID(l_csStudyLOID);
    l_sImageInfo.setSeriesLOID(l_csSeriesLOID);
    l_sImageInfo.setImageLOID(l_csPositionLOID);
    setImageInfo(l_sImageInfo);
    
    return(E_NO_ERROR);
}

////////////////////////    End Of File     /////////////////////////////////
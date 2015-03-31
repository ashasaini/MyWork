// ImageInfo.cpp: implementation of the CImageInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "planduplicator.h"
#include "ImageInfo.h"
#include "PDTrace.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*************************************************************
* Method:    	CImageInfo
* Description: 	Constructor and destructor
* Returns:   	
*************************************************************/
CImageInfo::CImageInfo()
{
    PD_TRACE(CImageInfo::CImageInfo);
}

CImageInfo::~CImageInfo()
{
    PD_TRACE(CImageInfo::~CImageInfo);
}
const CString CImageInfo:: getImageLOID(void)
{ 
    PD_TRACE(CImageInfo::getImageLOID);
    
    return m_csImageLOID; 
}

/*************************************************************
* Method:    	setImageLOID
* Description: 	This function is used 
* Parameter: 	CString & val :
* Returns:   	void
*************************************************************/
void CImageInfo::setImageLOID(CString& f_val) 			//Mpower 040/2012-08-24/AS/Variable name modified
{  
    PD_TRACE(CImageInfo::setImageLOID);
    m_csImageLOID = f_val; 
}

/*************************************************************
* Method:    	getSeriesLOID
* Description: 	This function is used 
* Parameter: 	void  :
* Returns:   	const CString
*************************************************************/
const CString CImageInfo::getSeriesLOID(void) 
{ 
    PD_TRACE(CImageInfo::getSeriesLOID);
    return(m_csSeriesLOID);
}
/*************************************************************
* Method:    	setSeriesLOID
* Description: 	This function is used 
* Parameter: 	CString & val :
* Returns:   	void
*************************************************************/
void CImageInfo::setSeriesLOID(CString& val) 
{ 
    PD_TRACE(CImageInfo::setSeriesLOID);
    m_csSeriesLOID = val; 
}

/*************************************************************
* Method:    	getStudyLOID
* Description: 	This function is used 
* Parameter: 	void  :
* Returns:   	const CString
*************************************************************/
const CString CImageInfo::getStudyLOID(void)
{ 
    PD_TRACE(CImageInfo::getStudyLOID);
    return(m_csStudyLOID); 
}
/*************************************************************
* Method:    	setStudyLOID
* Description: 	This function is used 
* Parameter: 	CString & val :
* Returns:   	void
*************************************************************/
void CImageInfo::setStudyLOID(CString& val) 
{ 
    PD_TRACE(CImageInfo::setStudyLOID);
    m_csStudyLOID = val; 
}
/*************************************************************
* Method:    	getDcmFileName
* Description: 	This function is used 
* Parameter: 	void  :
* Returns:   	const CString
*************************************************************/
const CString CImageInfo::getDcmFileName(void) 
{ 
    PD_TRACE(CImageInfo::getDcmFileName);
    return(m_csDcmFileName); 
}
/*************************************************************
* Method:    	setDcmFileName
* Description: 	This function is used 
* Parameter: 	CString & val :
* Returns:   	void
*************************************************************/
void CImageInfo::setDcmFileName(CString& val)
{
    PD_TRACE(CImageInfo::setDcmFileName);
    m_csDcmFileName = val; 
}

////////////////////////    End Of File     /////////////////////////////////
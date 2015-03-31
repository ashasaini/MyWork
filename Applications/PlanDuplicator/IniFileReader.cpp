// IniFileReader.cpp: implementation of the CIniReader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "common.h"
#include "IniFileReader.h"
#include "PDTrace.h"
#include "PDLogHelper.h"

#include "PDataParser/INIReader.h"
#include "Shlwapi.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const CString MPR_FILEPATH_SECTION = L"MPR_FILEPATH_SECTION";
const CString READING_MODE_SECTION = L"READING_MODE";
const CString VOI_FILEPATH_SECTION = L"VOI_FILEPATH_SECTION";

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//****************************Method Header************************************

//Method Name   : CIniReader

//Author        :PATNI

//Purpose       : Constructor
//*****************************************************************************
CIniFileReader::CIniFileReader() :
m_csinifile_name (_T("")),
m_nini_error_code (0),
m_nmpr_protocol_index (0),
m_csmpr_dicom_file_path (_T("")),
m_csmpr_exe_path (_T("")),
m_csmpr_file_path (_T("")),
m_csmpr_reading_mode (_T("")),
m_csmpr_study_file_path (_T("")),
m_csvoi_dicom_file_path (_T("")),
m_csvoi_exe_path (_T("")),
m_csvoi_file_path (_T(""))
{
    PD_TRACE(CIniFileReader::CIniFileReader);
    m_csinifile_name = getenv(MRMP_CONFIG);
    m_csinifile_name += PD_TOOL;
    m_csinifile_name += PD_INI_FILE;
    m_nini_error_code = E_NO_ERROR;
}

CIniFileReader::~CIniFileReader()
{
    PD_TRACE(CIniFileReader::~CIniFileReader);
}

//****************************Method Header************************************
/**This function used to retrieve all the values in the config ini file.
@param &p_config_params :: object of structure holding values 
@return FALSE: Values dont exist
@return TRUE: Values exist and copied successfully
@see
@since 1.00
**/
//*****************************************************************************
int CIniFileReader::ReadIniParams()
{
    PD_TRACE(CIniFileReader::ReadIniParams); 
    DWORD dwAttrs; 
    dwAttrs = GetFileAttributes(m_csinifile_name);
    if ( INVALID_FILE_ATTRIBUTES == dwAttrs ){
        CString l_csErrMsg = ErrorList[PD_NO_CONFIG_FILE];
        CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__,CUSTOM_ERRORDIALOG_DISPLAY);
        return(E_ERROR);
    }
    
    CIniReader l_ini_reader(m_csinifile_name);
    m_csmpr_file_path = l_ini_reader.GetKeyValue(L"FILE_PATH",MPR_FILEPATH_SECTION);
    m_csmpr_dicom_file_path = l_ini_reader.GetKeyValue(L"DICOM_FILE_PATH",MPR_FILEPATH_SECTION);
    m_csmpr_exe_path = l_ini_reader.GetKeyValue(L"EXE_PATH" ,MPR_FILEPATH_SECTION);
    m_csmpr_reading_mode = l_ini_reader.GetKeyValue(L"READING_MODE",READING_MODE_SECTION);
    m_csmpr_study_file_path = l_ini_reader.GetKeyValue(L"STUDY_FILE_PATH",READING_MODE_SECTION);
    
    m_csvoi_file_path = l_ini_reader.GetKeyValue(L"FILE_PATH",VOI_FILEPATH_SECTION);
    m_csvoi_dicom_file_path = l_ini_reader.GetKeyValue(L"DICOM_FILE_PATH",VOI_FILEPATH_SECTION);
    m_csvoi_exe_path = l_ini_reader.GetKeyValue(L"EXE_PATH" ,VOI_FILEPATH_SECTION);
    
    CString l_csmpr_protocol_index (_T(""));
    l_csmpr_protocol_index = l_ini_reader.GetKeyValue(L"PROTOCOL_INDEX",READING_MODE_SECTION);
    char *l_mpr_protidx_str = new char[BUFFER_SIZE_8];
    wcstombs(l_mpr_protidx_str,(LPCTSTR) l_csmpr_protocol_index, BUFFER_SIZE_8);
    m_nmpr_protocol_index =(int)atoi(l_mpr_protidx_str);
    
    delete []l_mpr_protidx_str;
    l_mpr_protidx_str = NULL;
    
    return(E_NO_ERROR);
}

//////////////////////////////////////////////////////////////////////
// getter functions
//////////////////////////////////////////////////////////////////////

/*************************************************************
* Method:    	GetMPRFilePath
* Description: 	This function is used 
* Parameter: 	void  :
* Returns:   	CString
*************************************************************/
CString CIniFileReader::GetMPRFilePath(void)
{
    PD_TRACE(CIniFileReader::GetMPRFilePath);
    return m_csmpr_file_path;
}

/*************************************************************
* Method:    	GetVOIFilePath
* Description: 	This function is used 
* Parameter: 	void  :
* Returns:   	CString
*************************************************************/
CString CIniFileReader::GetVOIFilePath(void)
{
    PD_TRACE(CIniFileReader::GetVOIFilePath);
    return m_csvoi_file_path;
}

/*************************************************************
* Method:    	GetMPRDicomFilePath
* Description: 	This function is used 
* Parameter: 	void  :
* Returns:   	CString
*************************************************************/
CString CIniFileReader::GetMPRDicomFilePath(void)
{
    PD_TRACE(CIniFileReader::GetMPRDicomFilePath);
    return m_csmpr_dicom_file_path;
}

/*************************************************************
* Method:    	GetVOIDicomFilePath
* Description: 	This function is used 
* Parameter: 	void  :
* Returns:   	CString
*************************************************************/
CString CIniFileReader::GetVOIDicomFilePath(void)
{
    PD_TRACE(CIniFileReader::GetVOIDicomFilePath);
    return m_csvoi_dicom_file_path;
}

/*************************************************************
* Method:    	GetMPRExePath
* Description: 	This function is used 
* Parameter: 	void  :
* Returns:   	CString
*************************************************************/
CString CIniFileReader::GetMPRExePath(void)
{
    PD_TRACE(CIniFileReader::GetMPRExePath);
    return m_csmpr_exe_path;
}

/*************************************************************
* Method:    	GetVOIExePath
* Description: 	This function is used 
* Parameter: 	void  :
* Returns:   	CString
*************************************************************/
CString CIniFileReader::GetVOIExePath(void)
{
    PD_TRACE(CIniFileReader::GetVOIExePath);
    return m_csvoi_exe_path;
}

/*************************************************************
* Method:    	GetMPRStudyFilePath
* Description: 	This function is used 
* Parameter: 	void  :
* Returns:   	CString
*************************************************************/
CString CIniFileReader::GetMPRStudyFilePath(void)
{
    PD_TRACE(CIniFileReader::GetMPRStudyFilePath);
    return m_csmpr_study_file_path;
}


/*************************************************************
* Method:    	GetMPRProtocolIndex
* Description: 	This function is used 
* Parameter: 	void  :
* Returns:   	int
*************************************************************/
int CIniFileReader::GetMPRProtocolIndex(void)
{
    PD_TRACE(CIniFileReader::GetMPRProtocolIndex);
    return m_nmpr_protocol_index;
}

/*************************************************************
* Method:    	GetMPRReadingMode
* Description: 	This function is used 
* Parameter: 	void  :
* Returns:   	CString
*************************************************************/
CString CIniFileReader::GetMPRReadingMode(void)
{
    PD_TRACE(CIniFileReader::GetMPRReadingMode);
    return m_csmpr_reading_mode;
}

////////////////////////    End Of File     /////////////////////////////////
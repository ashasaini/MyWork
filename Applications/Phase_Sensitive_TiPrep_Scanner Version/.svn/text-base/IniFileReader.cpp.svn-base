// IniFileReader.cpp: implementation of the CIniFileReader class.
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#include "stdafx.h"
#include "resource.h"
#include "IniFileReader.h"
#include "PDataParser/INIReader.h"

#include "TiPrepTrace.h"
#include "TiPrepLogHelper.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//+Mpower_65/Jun-2013/VP..T1MAP_pwd
const CString TI_PREP_APP = L"TiPrep";
#define T1MAP_OPTION _T("APP_OPTION")
#define TIPREP_OPTIONS_INI_FILE _T("\\T1Map_Option.ini")
#define TIPREP_PROC_OPTIONS _T("PROC_OPTION")
#define TIPREP_PHASE_OPTIONS _T("PHASE_OPTION")
#define TIPREP_THREADS _T("NUMBER_THREADS")
#define TIPREP_PROC_OPTIONS_INI_FILE _T("\\TiPrep.ini")

//-Mpower_65/Jun-2013/VP..T1MAP_pwd

// Construction/Destruction

//****************************Method Header***************************
//Method Name   : CIniReader
//Purpose       : Constructor
//********************************************************************
CIniFileReader::CIniFileReader() : m_ini_file_name(_T(""))
{
    /*TIPREP_TRACE(CIniFileReader::CIniFileReader);*/
    m_ini_file_name = getenv(MRMP_CONFIG);
    m_ini_file_name += _T("\\TiPrep");
    m_ini_file_name += TIPREP_OPTIONS_INI_FILE;
    m_ini_file_name_proc_option = getenv(MRMP_CONFIG);
    m_ini_file_name_proc_option += _T("\\TiPrep");
    m_ini_file_name_proc_option += TIPREP_PROC_OPTIONS_INI_FILE;
}
//****************************Method Header***************************
//Method Name   : CIniReader
//Purpose       : Destructor
//********************************************************************
CIniFileReader::~CIniFileReader()
{
    /*TIPREP_TRACE(CIniFileReader::~CIniFileReader);*/
}

///////////////////////////////////////////////////////////////////////////////
// Member Functions
///////////////////


/*************************************************************
* Method:       GetT1MapOption
* Description:  This function is used
* Parameter:    void
* Returns:      int
*************************************************************/
int CIniFileReader::GetT1MapOption(int &f_nAppOption)
{
    TIPREP_TRACE(CIniFileReader::GetT1MapOption,MRTraceMarker3);
    
    if(PathFileExists(m_ini_file_name)) {
        CIniReader l_ini_file_name(m_ini_file_name);
        CString l_cspwdRead = l_ini_file_name.GetKeyValue(T1MAP_OPTION, TI_PREP_APP);
        
        if(l_cspwdRead.CompareNoCase(_T("_TMSC_ALL_APP_")) == 0) {
            f_nAppOption = ALL_APP;
        } else if(l_cspwdRead.CompareNoCase(_T("_TMSC_TIPREP_ONLY_")) == 0) {
            f_nAppOption = TIPREP_ONLY;
        } else {
            f_nAppOption = TIPREP_ONLY;
        }
    }else{
        f_nAppOption = TIPREP_ONLY;
    }
    
    //by default T1Map application will be hidden
    return E_NO_ERROR;
}

/*************************************************************
* Method:       GetProcOption
* Description:  This function is used
* Parameter:    void
* Returns:      int
*************************************************************/
int CIniFileReader::GetProcOption(int &f_proc_roi_option)
{
    TIPREP_TRACE(CIniFileReader::GetProcOption,MRTraceMarker3);
    
    if(PathFileExists(m_ini_file_name_proc_option)) {
        CIniReader l_ini_file_name(m_ini_file_name_proc_option);
        CString l_get_key_value = l_ini_file_name.GetKeyValue(TIPREP_PROC_OPTIONS, TI_PREP_APP);
        
        if(l_get_key_value.CompareNoCase(_T("INROI")) == 0) {
            f_proc_roi_option = INROI;
        } else if(l_get_key_value.CompareNoCase(_T("ALLIMG")) == 0) {
            f_proc_roi_option = ALLIMG;
        } else {
            f_proc_roi_option = ALLIMG; 
        }
    }
    
    return E_NO_ERROR;
}

/*************************************************************
* Method:    	GetPhaseOption
* Description: 	This function is used 
* Parameter: 	int & f_proc_roi_option :
* Returns:   	int
*************************************************************/
int CIniFileReader::GetPhaseOption(int &f_proc_roi_option)
{
    TIPREP_TRACE(CIniFileReader::GetPhaseOption,MRTraceMarker3);
    
    if(PathFileExists(m_ini_file_name_proc_option)) {
        CIniReader l_ini_file_name(m_ini_file_name_proc_option);
        CString l_get_key_value = l_ini_file_name.GetKeyValue(TIPREP_PHASE_OPTIONS, TI_PREP_APP);
        
        if(l_get_key_value.CompareNoCase(_T("ROIAVG")) == 0) {
            f_proc_roi_option = ROIAVG;
        } else if(l_get_key_value.CompareNoCase(_T("ALLIMG")) == 0) {
            f_proc_roi_option = ALLIMG;
        } else {
            f_proc_roi_option = ALLIMG;
        }
    }
    
    return E_NO_ERROR;
}

///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////

int CIniFileReader::GetNumThreads(int &numthread)
{
	 TIPREP_TRACE(CIniFileReader::GetNumThreads,MRTraceMarker3);
    
    if(PathFileExists(m_ini_file_name_proc_option)) {
        CIniReader l_ini_file_name(m_ini_file_name_proc_option);
        CString l_get_key_value = l_ini_file_name.GetKeyValue(TIPREP_THREADS, TI_PREP_APP);
        
        if(l_get_key_value.CompareNoCase(_T("")) == 0) {
            numthread = 4;
        }  else {
			char tmp[16] = {0};
  wcstombs(tmp, l_get_key_value, 16);
  numthread = atoi(tmp);
  if (numthread <= 0 || numthread > 8)
			numthread = 4;
           
        }
    }
    
    return E_NO_ERROR;

}

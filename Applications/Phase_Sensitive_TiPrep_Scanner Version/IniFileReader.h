// IniFileReader.h: interface for the CIniFileReader class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#if !defined(AFX_INIFILEREADER_H__4F790096_42DA_4757_976E_C68B669786DC__INCLUDED_)
#define AFX_INIFILEREADER_H__4F790096_42DA_4757_976E_C68B669786DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include<string>


class CIniFileReader
{
//construction,copy & destruction
public:
    CIniFileReader();
    virtual ~CIniFileReader();

//implementation
public:
	int GetNumThreads(int &numthread);
    int GetT1MapOption(int &f_nAppOption);
    int GetProcOption(int &f_proc_roi_option);
    int GetPhaseOption(int &f_proc_roi_option); 

//variables
private:
    CString m_ini_file_name;
    CString m_ini_file_name_proc_option; 
};


#endif // !defined(AFX_INIFILEREADER_H__4F790096_42DA_4757_976E_C68B669786DC__INCLUDED_)


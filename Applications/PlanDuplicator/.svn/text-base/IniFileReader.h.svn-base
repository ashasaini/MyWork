// IniFileReader.h: interface for the CIniFileReader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INIREADER_H__75A1F2E7_857C_4A70_84CC_F032186C63BD__INCLUDED_)
#define AFX_INIREADER_H__75A1F2E7_857C_4A70_84CC_F032186C63BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include<string>
#include "FileReader.h"

class CIniFileReader 
{
public:
    CIniFileReader();
    virtual ~CIniFileReader();
    int ReadIniParams();
    
// getter functions
    CString GetVOIFilePath(void);
    CString GetMPRFilePath(void);
    CString GetMPRDicomFilePath(void);
    CString GetVOIDicomFilePath(void);
    CString GetMPRExePath(void);
    CString GetVOIExePath(void);
    CString GetMPRStudyFilePath(void);
    int GetMPRProtocolIndex(void);
    CString GetMPRReadingMode(void);

private:
    CString m_csinifile_name;
    int m_nini_error_code;
    CString m_csmpr_file_path;
    CString m_csmpr_dicom_file_path;
    CString m_csmpr_exe_path;
    CString m_csmpr_reading_mode;
    CString m_csmpr_study_file_path;
    int m_nmpr_protocol_index;
    
    CString m_csvoi_file_path;
    CString m_csvoi_dicom_file_path;
    CString m_csvoi_exe_path;
};

#endif // !defined(AFX_INIREADER_H__75A1F2E7_857C_4A70_84CC_F032186C63BD__INCLUDED_)

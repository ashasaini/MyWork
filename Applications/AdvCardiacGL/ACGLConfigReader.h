// ACGLConfigReader.h: interface for the CACGLConfigReader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACGLCONFIGREADER_H__C941636F_68BE_42D8_875E_606A3019B029__INCLUDED_)
#define AFX_ACGLCONFIGREADER_H__C941636F_68BE_42D8_875E_606A3019B029__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommonDefinitions.h"

class CACGLConfigReader  
{
public:
	CACGLConfigReader();
	virtual ~CACGLConfigReader();

    int ReadToolConfigSettings(std::vector<KeyValuePair>& f_KeyValuePairs);
	int ReadIniFile( const std::string& iniFilePath, const std::string& sectionName, std::vector<KeyValuePair>& f_KeyValuePairs );

private:
    CString m_ini_file_path;
};
#endif // !defined(AFX_ACGLCONFIGREADER_H__C941636F_68BE_42D8_875E_606A3019B029__INCLUDED_)
///////////////////////////////// END OF FILE /////////////////////////////////

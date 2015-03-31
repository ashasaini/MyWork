// KtIniFileReader.h: interface for the KtIniFileReader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KTINIFILEREADER_H__F8448CD5_40A8_4D34_BD9E_42AB4B1F2160__INCLUDED_)
#define AFX_KTINIFILEREADER_H__F8448CD5_40A8_4D34_BD9E_42AB4B1F2160__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KtIniFileReader  
{
public:
	KtIniFileReader();
	virtual ~KtIniFileReader();
    int GetNumberOfThreadsFromConfigFile(int &f_nNumOfThreads);

private:
    CString m_cfg_file_name;
};

#endif // !defined(AFX_KTINIFILEREADER_H__F8448CD5_40A8_4D34_BD9E_42AB4B1F2160__INCLUDED_)
/////////////////////////////////END OF FILE /////////////////////////////////////
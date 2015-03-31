// CoilChannelMode.h: interface for the CCoilChannelMode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COILCHANNELMODE_H__FC362077_8833_4E5B_8F2C_4D9831CF8BE0__INCLUDED_)
#define AFX_COILCHANNELMODE_H__FC362077_8833_4E5B_8F2C_4D9831CF8BE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPQMView;

class CCoilChannelMode
{
public:
    CCoilChannelMode(CPQMView* f_pqm_view);
    virtual ~CCoilChannelMode();

    static BOOL ValidateMaxCoilChannelFile(const int f_cf_system_enable_channel, int& f_system_enable_channel, int& f_limited_channel);

    BOOL CheckCoilForcibleMaxMode();
    void SetChannelMode();

private:
    CPQMView* m_pqm_view;
    //CCoilSelectionUtility* m_selection_utiliy;
    int m_limited_channel;

    static BOOL ReadMaxCoilChannelFile(const int f_cf_system_enable_channel, int& f_system_enable_channel, int& f_limited_channel);
    static BOOL ValidateMaxCoilChannelValues(const int f_cf_system_enable_channel, const int f_system_enable_channel, const int f_limited_channel);
    static BOOL GetConfigFilePath(CString& f_config_file_path, CString f_file_name);
};

#endif // !defined(AFX_COILCHANNELMODE_H__FC362077_8833_4E5B_8F2C_4D9831CF8BE0__INCLUDED_)

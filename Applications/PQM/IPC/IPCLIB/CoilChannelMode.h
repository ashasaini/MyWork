// CoilChannelMode.h: interface for the CCoilChannelMode class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_COILCHANNELMODE_H__FA83E2D5_A0BE_4F61_A103_8A7473365D57__INCLUDED_)
//#define AFX_COILCHANNELMODE_H__FA83E2D5_A0BE_4F61_A103_8A7473365D57__INCLUDED_

#ifndef __SM_IPCLIB_COILCHANNELMODE_H_
#define __SM_IPCLIB_COILCHANNELMODE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPqmProtocol;
class CVarFieldHandle;
class CPQMStudy;
class CScanProcedure;


class CCoilChannelMode
{

public:
    CCoilChannelMode(CPQMStudy* f_pqm_studyptr);
    virtual ~CCoilChannelMode();

    void GetNumSignalfromSelectedSection(const int f_selected_item, VARIANT* f_pdata);
    int UpdateCoil(const int f_prot, const int f_numchannels);
    void GetChannelModeSelection(const int f_selected_item, VARIANT* f_pdata);
    void SetChannelModeSelection(const int f_selected_item, BOOL f_coilchannelmode, CPqmProtocol* f_prot_obj = NULL);

    void SetNumChannelsforChannelMode(const BOOL f_use_coil_channel_mode, const int f_system_enable_channel, const int f_limited_channel);

    int GetSystemEnableChannel() const;
    int GetLimitedChannel() const;
    BOOL IsCoilChannelModeApplicable() const; // 20-Oct-11

private:
    CPQMStudy* m_pqm_studyptr;
    int m_system_enable_channel;
    int m_limited_channel;
    BOOL m_use_coil_channel_mode; // 20-Oct-11
};

#endif // !defined__SM_IPCLIB_COILCHANNELMODE_H_

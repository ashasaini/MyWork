// WirelessGatingController.h: interface for the CWirelessGatingController class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_WIRELESSGATINGCONTROLLER_H__
#define __SM_WIRELESSGATINGCONTROLLER_H__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <libStudyManager/NonCopyable.h>

class CPQMView ;
class CPQMWirelessGating;

class CWirelessGatingController  :
    private MR::ACQ::SM::COMMON::NonCopyable<CWirelessGatingController>
{
public:
    CWirelessGatingController();
    virtual ~CWirelessGatingController();

    void OnClickMenuitemWirelessGating();
    void ShowHideOpenedWindows(const bool f_show = true) ;

    void UpdateOKBtnStatus();
    void InitializeWirelessParametersWithConfigDefault();
    void ReadWirelessParamfromConfig(CIniReader* f_ini_reader);
    CPQMWirelessGating*	GetWirelessGatingDialog()const;
    void SetPQMView(CPQMView* f_pqmview);

protected :
    virtual bool CanEnableWirelessDlgOKBtn() ;

    bool GetWirelessGatingSelections(s_wireless_gating* const f_wireless_gating)const;
    void ValidateWireLessGatingConfigParam(Wireless_gating_t* f_sIPCConfigInfo);

    void SetWirelessDefaultParamtoIPC(const Wireless_gating_t& f_wireless_param);

private:
    CPQMView* m_pqmview;
    CPQMWirelessGating* m_pqmwirelessgating_dlg;
    Wireless_gating_t m_wireless_default_config_param;

    bool m_is_pqmwirelessgating_window_opened_before_swithcing_page;

};

#endif // #ifndef __SM_WIRELESSGATINGCONTROLLER_H__

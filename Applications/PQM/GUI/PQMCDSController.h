//TMSC-REDMINE-783
//TMSC-REDMINE-782
//TMSC-REDMINE-780
//Redmine-775
// PQMCDSController.h: interface for the CPQMCDSController class.
//
//////////////////////////////////////////////////////////////////////

//Redmine-777
#ifndef __SM_PQM_CDS_CONTROLLER_H__
#define __SM_PQM_CDS_CONTROLLER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>
#include <PQM/CDSCommonStructures.h>


class CPQMView ;
class CCOMPASS_SettingDialog;
class CCOMPASS_Location_Debug;
class CIniReader;
class CPQMListCtrl ;

class CPQMCDSController  : private MR::ACQ::SM::COMMON::NonCopyable<CPQMCDSController> /*: public CPQMCDSControllerInterface*/
{
public:
    explicit CPQMCDSController(CPQMView* f_pqm_view);
    virtual ~CPQMCDSController();

    void InitCDSController();
    void ShowHideOpenedWindows(const bool f_show = true) ;
    void InitializeCDSParametersWithDefault(SIPCConfigInfo* f_sIPCConfigInfo) const;
    void InitializeCDSParametersWithDefault(sCDSConfigInfo* f_cdsconfiginfo) const;
    bool ReadCDSParams(CIniReader* f_ini_reader, SIPCConfigInfo* f_sIPCConfigInfo) ;
    void OnASCOMPASSSettings() ;
    void OnOffAsCompass()const;
    void OnOffCDSDebug();
    void CDSApplyCoiltoProtocols()const;
    void OpenCompassDebug();
    bool IsCDSDebugON() const;
    void UpdateCompassCoilToBelowProtocols()const;
    void UpdateCompassCoilToCurrentProtocol()const;
    void DisplayCoilSelforCOMPASSConfirmation()const;
    void InitializeCDSParameters(const SIPCConfigInfo* f_sIPCConfigInfo);
    void InitializeCDSParametersWithConfigDefault();
    void InitializeCDSParameters(const sCDSConfigInfo* f_cdsconfiginfo);
    void UpdateAsCompassFlagForCurrentProtocol() const;
    CCOMPASS_Location_Debug* GetCompasLocationDebugDialog()const;
    CCOMPASS_SettingDialog* GetCOMPASS_SettingDlg()const;

private:
    CPQMView* m_pqm_view ;
    CCOMPASS_SettingDialog* m_ascompass_settingdialog;
    CCOMPASS_Location_Debug* m_compass_location_debugdialog;
    bool m_is_ascompassse_setting_opened_before_swithcing_page;
    bool m_is_compass_debug_opened_before_swithcing_page;
    bool m_cdsdebug_flag ;
    sCDSConfigInfo m_cdsconfiginfo_default ;

    bool ValidateCDSParams(const SIPCConfigInfo* f_sIPCConfigInfo) ;

    bool GetParameterValue(const CString& l_param_value_str, int& l_param_value_int);
    CPQMListCtrl* GetPQMListCtrl() const;
};

#endif // #ifndef __SM_PQM_CDS_CONTROLLER_H__

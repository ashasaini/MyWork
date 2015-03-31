// PQMAMBController.h: interface for the CPQMAMBController class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_PQM_AMB_CONTROLLER_H__
#define __SM_PQM_AMB_CONTROLLER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>
#include "PQMAMBDlg.h"
class CPQMView ;
//class CPQMAMBDlg;


class CPQMAMBController :
    public CAMBDialogInterface,
    private MR::ACQ::SM::COMMON::NonCopyable<CPQMAMBController>
{
public:
    explicit CPQMAMBController();
    virtual ~CPQMAMBController();

    void InitializeAMBController(CPQMView* const f_pqm_view, const CRect& f_parent_rect);
    void ShowHideOpenedWindows(const bool f_show = true);
    void OnAMBScanMenu();
    bool IsAMBInvoked()const;
    void CloseAMBOnError()const;
    bool AMBScanStartThread();
    void UpdateAMBAbortButtonState(const BOOL f_amb_abort_btn_state);
    void RecAmbEditExit()const;
    void RecAMBPqmScanMsg()const;
    void RecAmbScanWait()const;
    void RecAmbScanStartError()const;
    void RecAmbScanStartMsg(const bool f_is_irs_msg = false)const;
    void EnabledisableAMBScanRelatedButtons(const bool f_is_enable);
    void AmbScanStateSetting();
    void UpdateAMBScanButtonState(const BOOL f_amb_scan_btn_state);

    //Implement CAMBDialogInterface methods.
    void GetAmbPlan(VARIANT* f_pData);
    bool RemoteCheckActiveFlag();
    void OnClickButtonScanAbort();
    bool IsWaitCursor() const;
    bool AMBScanStart() ;
    void OnClickScanStart();
    void AMBScanCancel(VARIANT* f_pData);
    void AMBScanAbort() ;
    void AMBDlgCancel();
    bool IsAcqumanReadyForNextRequest() const;


    CPQMAMBDlg* GetAMBDlgPtr()const;
    bool IsAMBScanStarted() const ;
    void UpdateStatesForCtrls(const pqm_msg_tag f_message_to_process);
    void UpdateDelayTimeState();

    void SetIsCanResetAMBprotocolsOnWseExit(const bool f_canreset_ambprotocols_on_wse_exit);
    bool GetIsCanResetAMBprotocolsOnWseExit() const;
    void SetIswaitforAMBDlgOpen(const bool f_is_wait_for_amb_dlg_open);
    bool GetIswaitforAMBDlgOpen() const;

protected:

private:
    CPQMView* m_pqm_view;
    CPQMAMBDlg*	m_amb_dlg;
    bool m_is_amb_opened_before_swithcing_page;
    bool m_batch_setting_for_coil_sar_backup;
    bool m_canreset_ambprotocols_on_wse_exit;
    bool m_is_wait_for_amb_dlg_open;
};

#endif // #ifndef __SM_PQM_AMB_CONTROLLER_H__

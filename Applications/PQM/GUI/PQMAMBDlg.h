// PQMAMBDlg.h: interface for the CPQMAMBDlg class.
//
//////////////////////////////////////////////////////////////////////


#ifndef __SM_AMB_DLG_H__
#define __SM_AMB_DLG_H__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <MPlusUICommon/ComboBoxEx.h>
#include <MPlusUICommon/ButtonEx.h>
#include <MPlusUICommon/RadioButtonEx.h>
#include <MPlusUICommon/DialogEx.h>
#include <MPlusUICommon/FrameEx.h>
#include <MPlusUICommon/CheckBoxEx.h>
#include <MPlusUICommon/PopupMenuEx.h>
#include <libStudyManager/NonCopyable.h>
#include <libStudyManager/UI/ZOrderWindow.h>
#include "CustomStatic.h"
#include "AMBDialogUtility.h"

using namespace MPlus::UICommon;


class CAMBDialogInterface
{

public:
    virtual void GetAmbPlan(VARIANT* f_pData) = 0;
    virtual bool RemoteCheckActiveFlag() = 0;
    virtual void OnClickButtonScanAbort() = 0;
    virtual bool IsWaitCursor() const = 0;
    virtual bool AMBScanStart() = 0;
    virtual void OnClickScanStart() = 0;
    virtual void AMBScanCancel(VARIANT* f_pData) = 0;
    virtual void AMBScanAbort() = 0;
    virtual void AMBDlgCancel() = 0;
    virtual bool IsAcqumanReadyForNextRequest() const = 0;
};

/****************************************************************************
 * Scan State (for Buttons)
 ****************************************************************************/
typedef enum _AmbEditScanState_e {
    AmbEditScanStateSetting,
    AmbEditScanStateScanning,
    AmbEditScanStatePrep,
    AMBEditScanStateAcqman,
    AMBEditScanStateAbort,
    AMBEditScanStateDelayTime
} AmbEditScanState_e;


class CPQMAMBDlg : public MR::ACQ::SM::COMMON::UI::CZOrderDialog,
    private MR::ACQ::SM::COMMON::NonCopyable<CPQMAMBDlg>
{

public:
    explicit CPQMAMBDlg(CAMBDialogInterface* f_ambinterface, const CRect& parent_rect = CRect(0, 0, 0, 0), CWnd* pParent = NULL); // standard constructor
    ~CPQMAMBDlg();

    void RecAMBPqmScanMsg();

    void RecAmbScanWait();

    void RecAmbScanStartError();

    void AmbEditCommScanWait();
    void AmbEditCommPqmScan(void);

    void AmbEditCommScanStartError();

    void CloseAMBOnError();
    void UpdateAMBAbortButtonState(const BOOL f_amb_abort_btn_state);
    void OnClickAMBScanStart(const bool f_is_irs_msg = false);
    void EnabledisableAMBScanRelatedButtons(const bool f_is_enable);
    void UpdateAMBScanButtonState(const BOOL f_amb_scan_btn_state);
    void AmbScanStateSetting();

    amb_t& GetAMBStatus();

    bool IsAMBScanStarted() const ;
    void UpdateStatesForCtrls(const pqm_msg_tag f_message_to_process);
    void UpdateDelayTimeState();

protected:
    //{{AFX_VIRTUAL(CPQMAMBDlg)
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    //}}AFX_VIRTUAL

    // Generated message map functions
    //{{AFX_MSG(CPQMAMBDlg)
    virtual BOOL OnInitDialogImpl();
    afx_msg void OnClickButtonCancel();
    afx_msg void OnClickButtonAMBScanStart();
    afx_msg void OnClickButtonAMBScanAbort();
    afx_msg void OnClickRadioButtonSeg2();
    afx_msg void OnClickRadioButtonSeg3();
    afx_msg void OnClickRadioButtonSeg4();

    afx_msg void OnSelchangeAmbMasterDirection();
    afx_msg void OnSelchangeAmbSub2Direction();
    afx_msg void OnSelchangeAmbSub3Direction();
    afx_msg void OnSelchangeAmbSub4Direction();
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

    virtual void OnCancel();
    virtual void OnClose();

    //}}AFX_MSG

private:

    // Dialog Data
    //{{AFX_DATA(CPQMCFADlg)
    //enum { IDD = IDD_AMB_DIALOG };
    //}}AFX_DATA
    CAMBDialogInterface* m_ambinterface;
    AmbEditScanState_e m_amb_scan_state;

    CAMBDialogUtility m_ambdialog_utility;
    amb_t m_amb_status;

    CRect m_parent_rect ;
    CRect m_dialog_rect;
    BUTTON_COLORS_t m_btnDefColor;

    ButtonEx m_start_button;
    ButtonEx m_cancel_button;
    ButtonEx m_abort_button;
    ButtonEx m_prep_button;

    ComboBoxEx	m_master_direction_combo;
    ComboBoxEx	m_sub1_direction_combo;
    ComboBoxEx	m_sub2_direction_combo;
    ComboBoxEx	m_sub3_direction_combo;
    ComboBoxEx	m_map_autovoice_combo;

    CCustomStatic	m_master_setting_label;
    CCustomStatic	m_station_lable;
    CCustomStatic	m_master;
    CCustomStatic	m_map_autovoice;

    RadioButtonEx m_sub1;

    CheckBoxEx m_shim;
    CheckBoxEx m_map;

    FrameEx m_station_frame;
    FrameEx m_fram_1;
    FrameEx m_fram_2;

    CCustomStatic	m_amb_staion_label[AMB_STATION_MAX];

    EditCtrlEx	m_amb_segment_editbox[AMB_SEGMENT_MAX][AMB_STATION_MAX];

    int m_iteration_menu_seg;
    int m_iteration_menu_station;

    bool m_amb_abort_btn_click;
    HCURSOR m_wait_cursor, m_org_cursor ;

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPQMAMBDlg)
    virtual BOOL PreTranslateMessageImpl(MSG* pMsg);
    //}}AFX_VIRTUAL

    void CreateControlsForStation();
    void CreateEditControlsForSegments();
    void AmbEditRefresh();
    BOOL IgnoreLeftClickOnSegmentsAndStations(MSG* pMsg);
    void OnMenuIteration1();
    void OnMenuIteration2();
    void OnMenuIteration3();
    void OnMenuIteration4();
    bool LoadSegmentIterationMenu(MSG* pMsg);
    void InitSettingforSegment2();
    void OnCheckShim();
    void OnCheckMap();
    void OnAMBPrep();
    void UdpateEditStyle(EditCtrlEx& f_edit) ;
    void UpdateSegmentEditForSeg(const int f_count);
    void UpdateComboEditForSeg(const int f_count);
    void UpdateRepeat(const int f_rep);
    void UpdateIterationMenu(PopupMenuEx* f_iterationmenu, const int f_seg, const int f_station);

    void SetButtonStyle();
    void SetDialogRibbonHeight();
    void AmbEditUiStationSet(const int f_station);
    void AmbEditUiSegmentSet(int f_segment);
    void AmbEditUiShimmingSet(const int f_status);
    void AmbEditUiMapSet(const int f_status);
    void AmbEditUiPrepSet(const int f_status, const  int f_prepEnableStatus);
    void AmbEditUiScanCellSet(const int f_segment, const int f_station, const int f_isTop, const int f_prep, const int f_repeat, const int f_sensitive);
    void AmbEditUiScanButtonSet();
    void AmbEditUiScanStateSet(const AmbEditScanState_e f_state);
    void AmbEditUiDirectionSet(int f_segment, int f_direction);

    EditCtrlEx& GetSegEditForStation(const int f_segment, const int f_station);

    DECLARE_MESSAGE_MAP()
};

#endif // #ifndef __SM_AMB_DLG_H__


// PlanDuplicatorDlg.h : header file
//
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: PlanDuplicator
    Author: PATNI-PlanDuplicator Team
*******************************************************************************/
#if !defined(AFX_PLANDUPLICATORDLG_H__B90C36A8_77FD_4491_B3E2_1DD133A09E01__INCLUDED_)
#define AFX_PLANDUPLICATORDLG_H__B90C36A8_77FD_4491_B3E2_1DD133A09E01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

#include <tchar.h>
#include <string.h>
extern "C" {
#include <tami/libvfToUi/vfToUiDef.h>
#include <tami/libvfToUi/vfToUiproto.h>
}

#include <MPlusUICommon/DialogEx.h>
#include <MPlusUICommon/ButtonEx.h>
#include <MPlusUICommon/EditCtrlEx.h>
#include <MPlusUICommon/CheckBoxEx.h>
#include <MPlusUICommon/FrameEx.h>
#include <MPlusUICommon/ListCtrlEx.h>
#include <MPlusUICommon/ListBoxEx.h>
#include <MPlusUICommon/RadioButtonEx.h>
#include "dataTypes.h"
#include <atlbase.h>
#include<vector>
using namespace std;
using namespace MPlus::UICommon;
typedef vector<p_data_t> PROTVECTOR; 
/////////////////////////////////////////////////////////////////////////////
// CPlanDuplicatorDlg dialog
class CPDController;
class CPlanDuplicatorDlg : public  DialogEx
{
    // Construction
public:
	~CPlanDuplicatorDlg();
    CPlanDuplicatorDlg(CWnd* pParent = NULL);   // standard constructor
    // Dialog Data
    //{{AFX_DATA(CPlanDuplicatorDlg)
    enum { IDD = IDD_PLANDUPLICATOR_DIALOG };
    //}}AFX_DATA
    
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPlanDuplicatorDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
    
    // Implementation
    
protected:
	BOOL PreTranslateMessage(MSG* pMsg);
    
    // Generated message map functions
    //{{AFX_MSG(CPlanDuplicatorDlg)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);//
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnButtonBrowse();
    afx_msg void OnButtonGetdata();
    afx_msg void OnRadioSmartmpr();
    afx_msg void OnRadioSmartvoi();
    afx_msg void OnRadioAutoCouch ();
    afx_msg void OnRadioAutoPlan ();
    afx_msg void OnRadioManual ();
    afx_msg void OnButtonUpdateseq();
    afx_msg void OnCheckDicomaxis();
    afx_msg void OnRowClkListctrlData(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnRowClkListUnscannedprot(NMHDR* pNMHDR, LRESULT* pResult);
   // afx_msg void OnCheckAutoupdate();
    afx_msg void OnButtonSmartmpr();
    afx_msg void OnExit();
    virtual void OnCancel();
    afx_msg void OnButtonConvert();
    afx_msg void OnClickListctrlScannedprot(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnButtonCalculate();
    afx_msg void OnRadioSliceNumber();
    afx_msg void OnRadioSliceGap();
    afx_msg void OnRadioSliceThickness();
    afx_msg void OnRadioAxial();
    afx_msg void OnRadioCoronal();
    afx_msg void OnRadioSagittal();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
    HICON m_hIcon;

private:
    void ControlsMove(int &f_nshiftControl);
    void InitializeControls(void);
    void DisableEnableRadioBtns(bool f_bcheck);
    void UncheckAll(void);
    void CheckMain(void);
    void CalculateSelect(void);
    int ShowData(bool f_ncalculate_flag);
    int RunAutoUpdate(int f_unScannedProt[]);
    int showStatus(int &f_protno,const CString& f_slicePlane,int &f_flag);
    
    //Mpower 030/2012-06-26/AS/Modified/cpp change
    int getProtocols(const CString & f_sfileName);
    void CreateProtocolIndexString(const int &f_index);
    
    
    void InitListColumnForMPR(void);
    void InitListColumnForVOI(void);
    void ShowMPRData(void);
    void ShowVOIData(bool f_ncalculate_flag);
    template <class T>
        void MoveControlsUpDown(int offset,T &f_ctrl,CRect &ControlRect){
        f_ctrl.GetWindowRect(&ControlRect);
        ControlRect.top += offset;
        ControlRect.bottom += offset;
        ScreenToClient(&ControlRect);
        f_ctrl.MoveWindow(&ControlRect,TRUE);
    }
    ButtonEx m_BrowseButton;
    ButtonEx m_GetDataButton;
    ButtonEx m_UpdateSeqButton;
    ButtonEx m_ConvertButton;
    ButtonEx m_ExitButton;
    ButtonEx m_SmartMPRButton;
    ButtonEx m_btn_calculate;
    
    ListCtrlEx  m_listctrl_data;
    ListCtrlEx  m_listctrl_scannedProtocol;
    ListCtrlEx  m_listctrl_UnscannedProtocol;
    ListCtrlEx  m_listctrl_dataDicomAxis;
    ListCtrlEx  m_listctrl_status;
    
    //Smart VOI/MPR
    RadioButtonEx m_radio_smart_mpr;
    RadioButtonEx m_radio_smart_voi;

    //Mode of operation 
    RadioButtonEx m_radio_auto_couch;
    //RadioButtonEx m_radio_auto_plan;
   // RadioButtonEx m_radio_manual;

    //Axis 
    RadioButtonEx m_radio_axial;
    //RadioButtonEx m_radio_coronal;
  //  RadioButtonEx m_radio_sagittal;
    RadioButtonEx m_radio_slice_number;
    //RadioButtonEx m_radio_slice_gap;
  //  RadioButtonEx m_radio_slice_thickness;

    FrameEx m_Mode;
    FrameEx m_Axis;
    FrameEx m_Parameter;

    CStatic m_UpdatedSequenceText;

    CBrush m_list_brush;
    CFont m_font;
    CStatic m_Labelvantageaxis;
    CStatic m_LabelUpdateSequence;
    CheckBoxEx  m_check_DicomAxis;
    EditCtrlEx  m_FileName;
    CString m_windowText;
    
    int m_ncurrrowno;
    int m_ncurrunscannedrowno;
    int m_ncurrscannedrowno;
    funcParams_t m_fcParams;
    
    PROTVECTOR m_p_data;
    PROTVECTOR m_p_data_scanned;
    int m_ntotal_protocol;
    int m_nScannedtotal_protocol;
    CPDController *m_controller;
    CString m_prot_index_string;
    int     m_nmode;
    int     m_naxis;
    int     m_nparameter;
    BOOL m_bisCalculateDone; //Mpower 047/Sep-2012/AN/Added
};

#endif // !defined(AFX_PLANDUPLICATORDLG_H__B90C36A8_77FD_4491_B3E2_1DD133A09E01__INCLUDED_)

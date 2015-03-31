/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#if !defined(AFX_TIPREPDLG_H__967F0E55_7010_4101_AE09_4EAF7A11A0D3__INCLUDED_)
#define AFX_TIPREPDLG_H__967F0E55_7010_4101_AE09_4EAF7A11A0D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TiPrepDlg.h : header file
//
#include "PopupDlg.h"
#include "ROICopyDlg.h"
#include "ReconstructionDlg.h"
#include "PolarityCorrectionDlg.h" 
#include "ErrorDlg.h"
#include "SetROIParamsDlg.h"
#include "AddOffsetDlg.h"
#include "TiGraphCtrl.h"
#include "DisplayData.h"
#include "ManualPhaseCorrDlg.h"

#include <MPlusUICommon/ButtonEx.h>
#include <MPlusUICommon/EditCtrlEx.h>
#include <MPlusUICommon/CheckBoxEx.h>
#include "MPlusUICommon/RadioButtonEx.h"
#include <MPlusUICommon/FrameEx.h>
#include <MPlusUICommon/ListCtrlEx.h>
#include <MPlusUICommon/ListBoxEx.h>
#include "MPlusUICommon\PopupMenuEx.h"
#include "MPlusUICommon\ComboBoxEx.h"
#include "TiPrepProgressDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CTiPrepDlg dialog
class CAboutDlg;
class CTiPrepDlg : public CPopupDlg
{
// Construction
public:
    void DisableApplyTiButton();
    CTiPrepDlg(CWnd* pParent = NULL);   // standard constructor
    ~CTiPrepDlg();
    HICON m_hIcon;
    CBrush m_list_brush;
    bool IsReselectWait();
    void ClearGraphCtl();
    void DisplaySeriesName(CString &f_csProtocolName, int f_nSelDlg = 0);
    int OnRecvReselectDone();
    BOOL GetROIDisplayCheck(int f_nRoiNo);
    void SetDlgPos();
    BOOL SetAppOption(int f_nAppOption);
// Dialog Data
    //{{AFX_DATA(CTiPrepDlg)
	enum { IDD = IDD_TIPREP_DIALOG };
	ButtonEx	m_ApplyTI;
    CStatic m_Label_Auto_Phase_Correction;
    CStatic m_lbl_filter;
    CString m_image_data_type_value;
	//}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTiPrepDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL PreTranslateMessage(MSG* msg);
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CTiPrepDlg)
    afx_msg void OnButtonExit();
    afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
    virtual BOOL OnInitDialog();
    afx_msg void OnClose();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnButtonCopyROI();
    afx_msg void OnButtonSetROI();
    afx_msg void OnButtonRecon();
    afx_msg void OnMenuPolarityCorrection(); 
    afx_msg void OnButtonApply();
    afx_msg void OnButtonReselect();
    afx_msg void OnCheckOrgdata();
    afx_msg void OnCheckInvdata();
    afx_msg void OnCheckFitdata();
    afx_msg void OnCheckRoi1Dislpay();
    afx_msg void OnCheckRoi2Display();
    afx_msg void OnCheckRoi3Display();
    afx_msg void OnCheckNoiseRoi1Display();
    afx_msg void OnCheckNoiseRoi2Display();
    afx_msg void OnCheckOffsetCorrection();
    afx_msg void OnCheckCreateSeriesinAverageFilter(); 
    afx_msg void OnCheckRoi1Reversemin();
    afx_msg void OnCheckRoi2Reversemin();
    afx_msg void OnCheckRoi3Reversemin();
    afx_msg void OnRadio1stequ();
    afx_msg void OnRadio2ndequ();
    afx_msg void OnRadio3rdequ();
    afx_msg void OnRadio4rthequ();
    afx_msg void OnRadio5thequ();
    afx_msg void OnButtonResetroi();
    afx_msg void OnCheckEditROI();
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnPhaseCorrectionClick(); 
    afx_msg void OnDataTypeDisplay();
    afx_msg void OnButtonSaveRoiData();
    afx_msg void OnHideBtnClick();
    afx_msg void OnMenuAddoffset();
    afx_msg void OnCheckT1map();
    afx_msg void OnCheckManualGuess();
    afx_msg void OnButtonManualGuess();
    afx_msg void OnOptionsManualPhaseCorr();
    afx_msg void OnButtonGetDataType();
	afx_msg void OnAboutMenu();
	afx_msg void OnButtonApply_Ti();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    void InitializeControls();
    void ShowControls(BOOL f_bShowCtrl = TRUE);
    BOOL CreateGraphCtrl();
    int getMeanT1(BOOL *f_roi, BOOL *f_roiRev,
                   double * f_pfT1Vals, double * f_pfErrVals,
                   double * f_pfT1RevVals, double * f_pfErrRevVals);
    int getMeanT1MapMode(BOOL f_roi, BOOL f_roiRev,
                          double * f_pfT1Vals, double * f_pfErrVals,
                          double * f_pfT1RevVals, double * f_pfErrRevVals);   
    int getMeanTi(BOOL *f_roi, BOOL *f_roiRev,
                      double * f_pfTiVals, double * f_pfErrVals,
                      double * f_pfTiRevVals, double * f_pfErrRevVals,
                      double * f_pfT1RevVals,double * f_pfT1Vals,
                      CString & f_csMean_Ti);
    int getMeanTiMapMode(BOOL f_roi, BOOL f_roiRev,
                             double * f_pfTiVals, double * f_pfErrVals,
                             double * f_pfTiRevVals, double * f_pfErrRevVals,
                             double * f_pfT1RevVals,double * f_pfT1Vals,CString & f_csMean_Ti);//sb   
    BOOL ShowHideROI(int f_nROINo, int f_nChecked);
    bool EnableMapMode();
    bool EnableAvgMode();
private:
    int m_dlgHeightOrg;
    int m_nAppOption;
	CTiPrepProgressDlg * m_progressdlg;
public:
    BOOL isEditROI() {return m_checkEditROIData.GetCheck();} //AN Added
    BOOL ValidateOffset(CString f_csOffsetNo);
    void displayData();
    void displayMapModeData();      
    void GetSeriesNumber(CString &f_csSeriesNumber, CString &f_csProtocolName);
    void displayNoiseData();    
    void DisplayReconSelectedSeries(CString & series_name);
	CTiPrepDlg(CTiPrepDlg& f_dlg);
	LRESULT OnUpdateProgress(WPARAM wParam, LPARAM lParam);
public:
	void SetProgressDlg(CTiPrepProgressDlg * dlg);
	void EnableDisableROI(bool flag);
    void GUIOnApply();
    void GUIOnIdle();
    void DisplayErr(int &f_nmsg_code, int dlgcd = 0); 
    void EnableFilterCtrls(bool flag);
    int GetFilterSize(int &); 
    bool IsCreateFilteredSeries(); 
    bool IsAvgFilterOn(); 
    void DisplayPCSelectedSeries(CString & series_name, int select_btn); 
    void DisplayPhCorrSeries(CString &name); 
    void DisplayPixPos(int x, int y); 
    void DisplayControlsMapMode(bool f_bIsCalledFromShowHide);/* This flag shall be passed from true only if
                                                                 this function is called from Show/Hide button's callback
                                                                 Other wise it shall be true always */
    void SetRepeatationTime(float &f_tr_value); 
    void SetDataForm(const CString &f_csDataType);
    void EnablePhaseCorrectionButton(BOOL &f_bFlag);
    ButtonEx m_ApplyButton;
    ButtonEx m_SaveRoiDataToFileButton;
    ButtonEx m_ReselectButton;
    ButtonEx m_ResetROIButton;
    ButtonEx m_ExitButton;
//    ButtonEx m_SelectMgImgButton;
 //   ButtonEx m_SelectReImgButton;
  //  ButtonEx m_SelectImImgButton;
//    ButtonEx m_CorrectPhaseImgButton;
  //  ButtonEx m_SelectReconButton;
  //  ButtonEx m_ReconstructionButton;
  //  ButtonEx m_CopyButton;
    ButtonEx m_one_button_phase_correction; 
    ButtonEx m_hide_btn; 
    CStatic m_inversionTime;
    CStatic m_recoveryTime;
 //   CStatic m_invTI;
    CStatic m_LabelDispOpt;
    CStatic m_Display;
    CStatic m_fittingErr;
    CStatic m_reverseMin;
    //CStatic m_recoveryT1;
    CStatic m_lableROI1;
    CStatic m_lableROI2;
    CStatic m_lableROI3;
    CStatic m_lableMean;
    CStatic m_lableRepetitionTime;
    CStatic m_static_series_name; 
    CStatic m_lbl_pix_pos;       
    CStatic m_data_form;
    CStatic m_LabelNoiseCorrection;
    CStatic m_LabelNoiseCorrectionVal;
    ComboBoxEx m_image_data_type_control;
    CFont m_font;
    CheckBoxEx  m_checkOrgData;
    CheckBoxEx  m_checkInvData;
    CheckBoxEx  m_checkFitData;
    CheckBoxEx  m_checkEditROIData;
    CheckBoxEx  m_checkDispROI1Data;
    CheckBoxEx  m_checkDispROI2Data;
    CheckBoxEx  m_checkDispROI3Data;
    CheckBoxEx  m_checkRevDispROI1Data;
    CheckBoxEx  m_checkRevDispROI2Data;
    CheckBoxEx  m_checkRevDispROI3Data;
    CheckBoxEx  m_checkNoiseROI1Data;
    CheckBoxEx  m_checkNoiseROI2Data;
    CheckBoxEx  m_checkOffsetCorrection;
    CheckBoxEx  m_checkT1Map;
    RadioButtonEx m_radioEqu1;
    RadioButtonEx m_radioEqu2;
    RadioButtonEx m_radioEqu3;
    RadioButtonEx m_radioEqu4;
    RadioButtonEx m_radioEqu5;
    EditCtrlEx  m_ROI1FitErr;
    EditCtrlEx  m_ROI2FitErr;
    EditCtrlEx  m_ROI3FitErr;
    EditCtrlEx  m_ROI1TiVal;
    EditCtrlEx  m_ROI2TiVal;
    EditCtrlEx  m_ROI3TiVal;
    EditCtrlEx  m_ROIMeanTiVal;
    EditCtrlEx  m_ROI1T1Val;
    EditCtrlEx  m_ROI2T1Val;
    EditCtrlEx  m_ROI3T1Val;
    EditCtrlEx  m_ROIMeanT1Val;
    EditCtrlEx  m_Recon;
    EditCtrlEx  m_RepTime;
    EditCtrlEx  m_GraphView;
    EditCtrlEx  m_selected_protocol_display; 
    EditCtrlEx m_NoiseCorrectionVal;    
    FrameEx m_displayOption;
    FrameEx m_dataCalc;
    FrameEx m_equetionFrame;
    FrameEx m_phase_correction_frame; 
    FrameEx m_select_filter_frame;
    CStatic m_select_filter_static_box;
    ComboBoxEx m_select_filter_combo_box;
    CStatic m_filter_size_static_box;
    EditCtrlEx m_filter_size_edit_box;
    CheckBoxEx m_create_series_check_box;
    CStatic m_create_series_static_box;
    FrameEx m_NoiseCorrectionFrame;     
//    ListCtrlEx  m_listctrl_scannedProtocol;
//    ListCtrlEx  m_listctrl_Reocn;
    PopupMenuEx m_menu;
    PopupMenuEx m_menufile;
    PopupMenuEx m_menufilepopup;
    PopupMenuEx m_menuAboutpopup;
    PopupMenuEx m_menueditpopup;
    CheckBoxEx m_initialGuessChk;
    ButtonEx   m_initialGuessApply;
    ButtonEx m_bxDataType;
    EditCtrlEx m_exDataType;
    /**
    * Instance of CTiGraphCtrl class.
    */
    CTiGraphCtrl m_tigraphctrl;

    /**
    * Flag to check if 'Apply' button is checked.
    */
    BOOL m_flag;
    //CDisplayData *m_pDisplayData;
	CManualPhaseCorrDlg * m_pManualPhaseCorr;
    CReconstructionDlg *m_pReconDlg;
    CROICopyDlg *m_pCopyROIDlg;
	CSetROIParamsDlg *m_setroi_dlg;

    CAboutDlg *m_AbtDlg;
    CPolarityCorrectionDlg *m_polarity_correction_dlg;
    CAddOffsetDlg *m_AddOffsetDlg;
    bool m_is_reselect_wait; //flag is set when reselect button is clicked
    int m_nEqnSelectFlag;
    bool m_ApplyProcessFlag;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIPREPDLG_H__967F0E55_7010_4101_AE09_4EAF7A11A0D3__INCLUDED_)

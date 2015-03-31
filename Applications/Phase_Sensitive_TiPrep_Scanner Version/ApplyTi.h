#if !defined(AFX_APPLYTI_H__B054F464_8EFA_445B_BFBE_44EE79FB88FA__INCLUDED_)
#define AFX_APPLYTI_H__B054F464_8EFA_445B_BFBE_44EE79FB88FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ApplyTi.h : header file
//
#include "PopupDlg.h"
#include "resource.h"
#include <MPlusUICommon/ButtonEx.h>
#include <MPlusUICommon/EditCtrlEx.h>
#include <MPlusUICommon/CheckBoxEx.h>
#include <MPlusUICommon/RadioButtonEx.h>
#include <MPlusUICommon/FrameEx.h>
#include <MPlusUICommon/ListCtrlEx.h>
#include "SpinCtrl.h"


/////////////////////////////////////////////////////////////////////////////
// CApplyTi dialog

class CApplyTi : public CPopupDlg
{
// Construction
public:
	CApplyTi(CWnd* pParent = NULL);   // standard constructor
    CApplyTi(CApplyTi& f_ApplyTiDlg);
    void RefreshGUI();
// Dialog Data
	//{{AFX_DATA(CApplyTi)
	enum { IDD = IDD_DIALOG_APPLY_TI };
    SpinCtrl m_SpinInputTi;
	CStatic	m_SelUnscanProtLabel;
	CStatic	m_OriginalTiLabel;
	CStatic	m_NewTiLabel;
	CStatic	m_SetTiModeLabel;
	CStatic	m_AvgLabel;
	ListCtrlEx	m_UnScanProtList;
	EditCtrlEx	m_VFTiEdit;
	EditCtrlEx	m_ROI3Edit;
	EditCtrlEx	m_ROI2Edit;
	EditCtrlEx	m_ROI1Edit;
	EditCtrlEx	m_OriginalTiEdit;
	EditCtrlEx	m_OffsetTiEdit;
	EditCtrlEx	m_NewTiEdit;
	EditCtrlEx	m_InputTiEdit;
	EditCtrlEx	m_AverageTiEdit;
	CheckBoxEx	m_ROI3Check;
	CheckBoxEx	m_ROI2Check;
	CheckBoxEx	m_ROI1Check;
	CheckBoxEx	m_OffsetCheck;
	FrameEx	m_TiValuesFrame;
	FrameEx	m_SetTiValueFrame;
	FrameEx	m_FrameUnscannedProtocolsFrame;
    FrameEx	m_SETTIModeFrame;
    FrameEx	m_ROIAVGFrame;
	ButtonEx	m_Cancel;
	ButtonEx	m_Apply;
    RadioButtonEx m_CalculateModeRadio;
    RadioButtonEx m_VFTiModeRadio;
    RadioButtonEx m_InputTiRadio;
    CFont m_font;
    CBrush m_list_brush;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CApplyTi)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void InitializeControls();
    void FillUnScannedProtocolList();
    void UpdateControls();
    void ClearUnscannedProtocolList();
    void ClearData();
    

	// Generated message map functions
	//{{AFX_MSG(CApplyTi)
	virtual void OnCancel();
	afx_msg void OnApply();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClickListUnscanProtocol(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRadioVfTival();
	afx_msg void OnRadioInputti();
	afx_msg void OnRadioCalcmode();
	afx_msg void OnCheckRoi1();
	afx_msg void OnCheckRoi2();
	afx_msg void OnCheckroi3();
	afx_msg void OnCheckOffset();
    afx_msg LRESULT UpdateControlsfromSpin(WPARAM, LPARAM);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
        BOOL ValidateOffset(CString f_csOffsetNo);
private:
    std::vector <long> m_csProtocolNumberVector;
    std::vector <int> m_nProtIndexVector;
    std::vector <int> m_nTiValueVector;
    std::vector<CString> m_csProtSequenceVector;
    int m_nTotalUnScannedProt ;
    int m_nTotalUnScannedSeries ;
    CString m_csStudyPath;
    CString m_csOriginalTiValue;
    int m_ModeUsed;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APPLYTI_H__B054F464_8EFA_445B_BFBE_44EE79FB88FA__INCLUDED_)

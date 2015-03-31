#if !defined(AFX_PAGEONE_H__1952E8FF_282F_4863_851A_C9A10E69C4E5__INCLUDED_)
#define AFX_PAGEONE_H__1952E8FF_282F_4863_851A_C9A10E69C4E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageEPI.h : header file
//
#include "PageBase.h"
/////////////////////////////////////////////////////////////////////////////
// CPageEPI dialog
#include "EPIController.h"

class CPageEPI : public CPageBase//CPropertyPage
{
    DECLARE_DYNCREATE(CPageEPI)

// Construction
private:
    CPageEPI();
    static CPageEPI * m_pPageOne;

public:
    static CPageEPI * GetPageOneInstance();
    static void DeletePageOneInstance();

    ~CPageEPI();

// Dialog Data
    //{{AFX_DATA(CPageEPI)
	enum { IDD = IDD_PROPPAGE_EPI };
	CEdit	m_EditMerge;
	CButton	m_CtrlReconButton;
	CComboBox	m_ctrlCombStepSelection;
	CStatic	m_CtrlLabStepSelection;
	CButton	m_ctrlGrprecon;
    CListCtrl   m_RawDataList;
    CButton m_frmCtrlStudyName;
    CComboBox   m_comboChannelSelection;
    CComboBox   m_comboDataForm;
    CStatic m_ctrlDisplayOptions;
    CButton m_buttonMregeData;
    CButton m_buttonCorrectPhase;
    CButton m_chkCtrlPCMethod;
    CButton m_chkCtrlShiftPeak;
    CButton m_radioPC;
    CButton m_radioNonSpeeder;
    CButton m_radioShiftPeak;
    CButton m_frmShiftPeak;
    CButton m_frmPCOptions;
    CButton m_frmPhaseOptions;
    CButton m_frmProcessSelection;
    CButton m_kSpaceSeparateEO;
    CButton m_rawDataSeparate;
    CButton m_radioCtrlT2;
    CButton m_radioCtrlT1;
    CButton m_frmKspaceSeparate;
    CButton m_chkProcess;
    BOOL    m_chkProcessFlag;
    int     m_radioT2Flag;
    BOOL    m_chkShiftPeak;
    BOOL    m_chkPCMethod;
    CButton m_radioPCDiff;
    CButton m_radioShiftPeakLineFit;
    CButton m_radioPCAll;
    CButton m_radioShiftPeakFreq;
    CButton m_radioPCRelative;
    CComboBox m_Combo;
    CString m_csDisplayString;
	int		m_radioShiftPeakFlag;
	//}}AFX_DATA


// Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CPageEPI)
public:
    //virtual void OnInitialUpdate();
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL


// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CPageEPI)
    afx_msg void OnCheckProcess();
    afx_msg void OnButtonDisplayKspace();
    afx_msg void OnCheckShiftpeak();
    afx_msg void OnCheckPcMethod();
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnDropdownCombo1DataForm();
    afx_msg void OnItemchangedListProtocol(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnButtonSeparateRawData();
	afx_msg void OnButtonMergeData();
	afx_msg void OnButtonRecon();
	afx_msg void OnButtonSepaarteKspace();
	afx_msg void OnButtonCorrectPhase();
	afx_msg void OnRadioTsLf();
	afx_msg void OnRadioTsFreq();
	afx_msg void OnRadioTimeShift();
	afx_msg void OnRadioDft();
	afx_msg void OnRadioDifference();
	afx_msg void OnRadioAllpc();
	afx_msg void OnRadioRelativePc();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
protected:
#ifdef _DEBUG
    //virtual void AssertValid() const;
    //virtual void Dump(CDumpContext& dc) const;
#endif
    CString m_csPathSelection;
public:
    void setListCtrlData(protInfo_t *&f_protData, int f_nTotalRawNodes,int f_nmergeDataFlag);
    void setStudyName(const CString & f_csStudyFileName);
    int ClearGUI();
    // Operations
private:
    BOOL IsProcessed(CString &cs);

//  int DisplayRawData();
    //void setDisplayTextForImageDisplay(CString &f_csDisplayStr);
private:
    int m_nCurHeight;
    int m_nScrollPos;
    CRect m_rect;
    BOOL m_spinFlag;
    protInfo_t *m_protData;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEONE_H__1952E8FF_282F_4863_851A_C9A10E69C4E5__INCLUDED_)

/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/

#if !defined(AFX_MANUALINITIALGUESSDLG_H__CFBDD513_AF13_4B04_A42F_AF27F96F3F77__INCLUDED_)
#define AFX_MANUALINITIALGUESSDLG_H__CFBDD513_AF13_4B04_A42F_AF27F96F3F77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ManualInitialGuessDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CManualInitialGuessDlg dialog
#include "PopupDlg.h"
#include <MPlusUICommon/ButtonEx.h>
#include <MPlusUICommon/EditCtrlEx.h>

class CManualInitialGuessDlg : public CPopupDlg
{
    // Construction
public:
    CManualInitialGuessDlg(CWnd* pParent = NULL);   // standard constructor
	~CManualInitialGuessDlg();
    
    // Dialog Data
    //{{AFX_DATA(CManualInitialGuessDlg)
    enum { IDD = IDD_DIALOG_MANUAL_INITIAL_GUESS };
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA
    
    
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CManualInitialGuessDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
    
    // Implementation
protected:
    HICON m_hIcon;
    
    // Generated message map functions
    //{{AFX_MSG(CManualInitialGuessDlg)
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnPaint();
    afx_msg void OnButtonInitialGuessApply();
    virtual void OnCancel();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
public:
    int getEqnFlag() const;
    void setEqnFlag(int val);
	CManualInitialGuessDlg& operator = (CManualInitialGuessDlg& f_manualinitialguess);
    
private:
    void InitializeControls();
    BOOL ValidateValues(CString &f_csValue, int f_nCondition, double &f_dValue);
    void SetDlgPos();
    int ValidateInitialValues();
private:
    ButtonEx m_buttonManualApply;
    ButtonEx m_buttonExit;
    EditCtrlEx m_editAFactor;
    EditCtrlEx m_editBFactor;
    EditCtrlEx m_editMFactor;
    EditCtrlEx m_editNFactor;
    
    CStatic m_lblAFactor;
    CStatic m_lblBFactor;
    CStatic m_lblMFactor;
    CStatic m_lblNFactor;
    
    CFont m_fontPhaseCorrection;
    CBrush m_list_brush;
    
    int m_nEqnFlag;

    double m_dAFactor;
    double m_dBFactor;
    double m_dMFactor;
    double m_dNFactor;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MANUALINITIALGUESSDLG_H__CFBDD513_AF13_4B04_A42F_AF27F96F3F77__INCLUDED_)

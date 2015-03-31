/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/

#if !defined(AFX_MANUALPHASECORRDLG_H__232C6DB7_5F36_4D9A_A96C_FA10F0227393__INCLUDED_)
#define AFX_MANUALPHASECORRDLG_H__232C6DB7_5F36_4D9A_A96C_FA10F0227393__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ManualPhaseCorrDlg.h : header file
//
#include "PopupDlg.h"
#include <MPlusUICommon/ButtonEx.h>
#include <MPlusUICommon/EditCtrlEx.h>
#include <MPlusUICommon/CheckBoxEx.h>
/////////////////////////////////////////////////////////////////////////////
// CManualPhaseCorrDlg dialog

class CManualPhaseCorrDlg : public CPopupDlg
{
    // Construction
public:
    void DisplaySeriesSelectedForManualPhaseCorr(const CString & f_csSeriesName, const int f_nSelectedDataType);
    CManualPhaseCorrDlg(CWnd* pParent = NULL);   // standard constructor
    CManualPhaseCorrDlg& operator = (CManualPhaseCorrDlg& f_manualphasecorr);
    CManualPhaseCorrDlg(CManualPhaseCorrDlg& f_dlg);
	~CManualPhaseCorrDlg();
    // Dialog Data
    //{{AFX_DATA(CManualPhaseCorrDlg)
    enum { IDD = IDD_DIALOG_MANUAL_PHASE_CORR };
    ButtonEx    m_Button_Select_Real;
    ButtonEx    m_Button_Select_Magnitude;
    ButtonEx    m_Button_Select_Imaginary;
    ButtonEx    m_Button_Ok;
    EditCtrlEx  m_Edit_Real;
    EditCtrlEx  m_Edit_Magnitude;
    EditCtrlEx  m_Edit_Imaginary;
    CStatic m_Label_Real;
    CStatic m_Label_Magnitude;
    CStatic m_Label_Imaginary;
    ButtonEx    m_Button_Phase_Corr;
    //}}AFX_DATA
    
    
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CManualPhaseCorrDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
    
    // Implementation
protected:
    
    // Generated message map functions
    //{{AFX_MSG(CManualPhaseCorrDlg)
    afx_msg void OnButtonCorrectPhaseManual();
    afx_msg void OnButtonSelectImaginary();
    afx_msg void OnButtonSelectMagnitude();
    afx_msg void OnButtonSelectReal();
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual void OnCancel();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    HICON m_hIcon;
    void InitializeManualPhaseCorrDlgControls();
    CBrush m_list_brush;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MANUALPHASECORRDLG_H__232C6DB7_5F36_4D9A_A96C_FA10F0227393__INCLUDED_)

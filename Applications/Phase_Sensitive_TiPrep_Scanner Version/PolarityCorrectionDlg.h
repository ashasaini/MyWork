/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#if !defined(AFX_POLARITYCORRECTIONDLG_H__7DCA6B6E_2830_48D8_8D24_580BA7A19FCE__INCLUDED_)
#define AFX_POLARITYCORRECTIONDLG_H__7DCA6B6E_2830_48D8_8D24_580BA7A19FCE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PopupDlg.h"
#include "PolarityCorrection.h"
#include <MPlusUICommon/ButtonEx.h>
#include <MPlusUICommon/EditCtrlEx.h>
class CTiPrepDlg;

// PolarityCorrectionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPolarityCorrectionDlg dialog

class CPolarityCorrectionDlg : public CPopupDlg
{
// Construction
public:
    CPolarityCorrectionDlg(CWnd* pParent = NULL);   // standard constructor
    CPolarityCorrectionDlg& operator = (CPolarityCorrectionDlg& f_polarity_correctionDlg_ptr);
	CPolarityCorrectionDlg(CPolarityCorrectionDlg& f_polaritycorrectiondlg);
	~CPolarityCorrectionDlg();
// Dialog Data
    //{{AFX_DATA(CPolarityCorrectionDlg)
    enum { IDD = IDD_POLARITY_CORRECTION };
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPolarityCorrectionDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation

public:
    void RefreshSelectedSeries(CString &f_csSeriesNumber);

protected:
    HICON m_hIcon;
    // Generated message map functions
    //{{AFX_MSG(CPolarityCorrectionDlg)
    // NOTE: the ClassWizard will add member functions here
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnMagnitudeSeriesSelectBtn();
    afx_msg void OnPolarityCorrectionBtnClick();
    virtual void OnCancel();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    void InitializeControls();
    void Display();

    ButtonEx m_magnitude_select_btn;
    ButtonEx m_exit_polarity_correction;
    ButtonEx m_polarity_correction_btn ;
    CString m_selected_series_no;
    CBrush m_list_brush;
    EditCtrlEx m_static_magnitude;
    EditCtrlEx m_edit_magnitude;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POLARITYCORRECTIONDLG_H__7DCA6B6E_2830_48D8_8D24_580BA7A19FCE__INCLUDED_)

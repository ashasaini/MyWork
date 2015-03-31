/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#if !defined(AFX_RECONSTRUCTIONDLG_H__74A21F2C_9548_48F0_B7A8_E07B12A786C4__INCLUDED_)
#define AFX_RECONSTRUCTIONDLG_H__74A21F2C_9548_48F0_B7A8_E07B12A786C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "PopupDlg.h"
#include <MPlusUICommon/ButtonEx.h>
#include <MPlusUICommon/ListCtrlEx.h>
#include <MPlusUICommon/EditCtrlEx.h>
#include "MPlusUICommon\ComboBoxEx.h"

/**
* This class is used for Reconstruction.
*/
class CReconstructionDlg : public CPopupDlg
{
// Construction
public:
    CReconstructionDlg(CWnd* pParent = NULL);   // standard constructor
    CReconstructionDlg& operator = (CReconstructionDlg& f_Rcondlg_ptr);
	CReconstructionDlg(CReconstructionDlg& f_recondlg);

// Dialog Data
    //{{AFX_DATA(CReconstructionDlg)
	enum { IDD = IDD_DIALOG_RECON };
	//}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CReconstructionDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;
    // Generated message map functions
    //{{AFX_MSG(CReconstructionDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnButtonReconstruction();
    afx_msg void OnButtonSelectRecon();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual void OnCancel();

    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    void InitializeControls();
private:
    ButtonEx m_button_select;
    ButtonEx m_button_exit;
    ButtonEx m_button_reconstruction;//+Shweta 30-May-13
    CStatic m_static_recon_prot;
    ComboBoxEx m_combo_data_form;
    CFont m_font;
    CBrush m_list_brush;
public:
    void setSeriseName(const CString &f_csSeriseName);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECONSTRUCTIONDLG_H__74A21F2C_9548_48F0_B7A8_E07B12A786C4__INCLUDED_)

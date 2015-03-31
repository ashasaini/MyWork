#if !defined(AFX_IMAGELISTDLG_H__CECEAFA6_3DF3_43A3_A541_C392AAB4406D__INCLUDED_)
#define AFX_IMAGELISTDLG_H__CECEAFA6_3DF3_43A3_A541_C392AAB4406D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageListDlg.h : header file
//
#include "ComboListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CImageListDlg dialog

class CImageListDlg : public CDialog
{
// Construction
public:
    CImageListDlg(CWnd* pParent = NULL);   // standard constructor
    ~CImageListDlg();
    void SetListPaths(CStringArray &f_CsList);
    void RefreshList();
    void SetDialogTitle(CString &f_cs);
// Dialog Data
    //{{AFX_DATA(CImageListDlg)
    enum { IDD = IDD_DIALOG_IMAGELIST };
    CComboListCtrl  m_ImageList;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CImageListDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    CStringArray m_CsArray;
    CString m_DialogTitle;
    CString m_csCurrentSelectedPath;

    void UpdateImage_selected_row();
    // Generated message map functions
    //{{AFX_MSG(CImageListDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnClose();
    afx_msg void OnClickListImage(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg LRESULT PopulateComboList(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT UpdateImager(WPARAM wParam, LPARAM lParam);

    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGELISTDLG_H__CECEAFA6_3DF3_43A3_A541_C392AAB4406D__INCLUDED_)

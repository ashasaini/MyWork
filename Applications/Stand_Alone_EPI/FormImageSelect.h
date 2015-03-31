#if !defined(AFX_EPIFORMVIEW_H__46C92489_4079_4AC2_BA22_35CDAE034AD6__INCLUDED_)
#define AFX_EPIFORMVIEW_H__46C92489_4079_4AC2_BA22_35CDAE034AD6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EPIFormView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormImageSelect form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CFormImageSelect : public CFormView
{
protected:
    CFormImageSelect();           // protected constructor used by dynamic creation
    DECLARE_DYNCREATE(CFormImageSelect)

// Form Data
public:
    //{{AFX_DATA(CFormImageSelect)
    enum { IDD = IDD_FORM_IMAGE_SELECT };
	CEdit	m_editSpin;
    CListCtrl   m_ctrlImageDataList;
    CComboBox   m_ctrlComboStepSelection;
    CComboBox   m_ctrlDataForm;
    CSpinButtonCtrl m_ctrlSpin;
    CListCtrl   m_ctrlRawDataList;
    //}}AFX_DATA

// Attributes
public:
//    void setListCtrlData(char **f_cListData,int f_nTotalRawNodes);
//  void setStudyName(const CString & f_csStudyFileName);
// Operations
private:
//    BOOL IsProcessed( CString &cs );
    //void setDisplayTextForImageDisplay(CString &f_csDisplayStr);

public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CFormImageSelect)
public:
    virtual void OnInitialUpdate();
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    virtual ~CFormImageSelect();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    // Generated message map functions
    //{{AFX_MSG(CFormImageSelect)
    afx_msg void OnSelchangeComboEpiDataForm();
    afx_msg void OnDeltaposSpinChannel(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnSelchangeComboSelectStep();
	afx_msg void OnClickListRawDataProtocolList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListImageDataProtocolList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedListRawDataProtocolList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedListImageDataProtocolList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
public:
//  void GetFilesInDirectory(std::vector<string> &out, const string &directory);
    int setListCtrlData(char **f_cListData, int f_nTotalRawNodes,
                        int f_nProtIdx, int f_nChannelCnt);
    BOOL IsProcessed(CString &cs);
    int DisplayRawData();
    int getFileList(CString &f_csDirPath);
    void addStep(CString &f_csStep);
    void ClearSelectStepComboBox();
    int DisplayKT1T2Data();
    int ClearGUIForm();
    int DisplayImageData();
    int DisplayKEvenOddData();
    int DisplayPhaseCorrectedData();
private:
    CString m_csPathSelection;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EPIFORMVIEW_H__46C92489_4079_4AC2_BA22_35CDAE034AD6__INCLUDED_)

//TMSC-REDMINE-783
//TMSC-REDMINE-781
//TMSC-REDMINE-780
//Redmine-775
//Redmine-777
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMAnatomyDlg.h
 *  Overview: Structure of CPQMAnatomyDlg class.
 *  Last Modified: 2007/8/10 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2007/6/15 12:00:00     PQM5 Baselined code
 *   2.0          Patni            2007/8/10 12:00:00     Implemented PQM6 requirements & Formatted
 *
 *****************************************************************************/
#ifndef __SM_PQM_ANATOMY_DLG_H__
#define __SM_PQM_ANATOMY_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <MPlusUICommon/DialogEx.h>
#include <libStudyManager/NonCopyable.h>
#include <libStudyManager/UI/ZOrderWindow.h>

using namespace MPlus::UICommon ;
#define TOTAL_ANATOMY_BITMAPS	2*SAR_REGIONS


class CPQMView;

// ! This class is used for displaying anatomy

class CPQMAnatomyDlg : public MR::ACQ::SM::COMMON::UI::CZOrderDialog,
    private MR::ACQ::SM::COMMON::NonCopyable<CPQMAnatomyDlg>
{
public:
    CPQMAnatomyDlg(const CRect& parent_rect = CRect(0, 0, 0, 0), const int currently_selected_sar_region = 0, CWnd* pParent = NULL);  // standard constructor
    ~CPQMAnatomyDlg();		//Added by Hemant On 6/25/2008 6:24:18 PM

    void SetVariablesForParameterEditing(const int f_nItem_index, const bool f_bprev_auto_scan_flag_status);

protected:

    // Generated message map functions
    //{{AFX_MSG(CPQMAnatomyDlg)
    virtual BOOL OnInitDialogImpl();
    virtual void OnCancel();
    afx_msg void OnChestButton() ;
    afx_msg void OnAbdomenButton() ;
    afx_msg void OnPelvisButton() ;
    afx_msg void OnShoulderButton() ;
    afx_msg void OnExtremityButton() ;
    afx_msg void OnHandButton() ;
    afx_msg void OnAnkleButton() ;
    afx_msg void OnHeadButton() ;
    afx_msg void OnTSpineButton() ;
    afx_msg void OnCSpineButton() ;
    //}}AFX_MSG

    //{{AFX_VIRTUAL(CPQMAnatomyDlg)
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

private:
    // Dialog Data
    //{{AFX_DATA(CPQMAnatomyDlg)
    enum { IDD = IDD_ANATOMY };
    //}}AFX_DATA

    SAR_REGION m_currently_selected_sar_region;
    CPQMView*	m_parent;
    CRect		m_parent_rect;
    int m_nItem_index;
    bool m_bPrev_auto_scan_flag_status;
    CStatic m_anatomy_chest ;
    CStatic m_anatomy_abdomen;
    CStatic m_anatomy_pelvis;
    CStatic m_anatomy_shoulder;
    CStatic m_anatomy_extremity ;
    CStatic m_anatomy_hand ;
    CStatic m_anatomy_ankle ;
    CStatic m_anatomy_head ;
    CStatic m_anatomy_tlspine ;
    CStatic m_anatomy_cspine ;
    CBitmap m_anatomy_bitmap[TOTAL_ANATOMY_BITMAPS] ;
    int n_extremityCounter;

    //{{AFX_VIRTUAL(CPQMAnatomyDlg)
    //}}AFX_VIRTUAL

    void StoreSelectedSarRegionInSelectedProtocol(const int sar_region);
    void UpdateAnatomy(const SAR_REGION currently_selected_sar_region);
    BOOL IsValidSarRegion(const SAR_REGION sar_region)const;
    void LoadImageForAnatomyButton(const SAR_REGION sar_region, const bool normal_image = true, const int sar_region_type = 1) ;
    void OnChangeAnatomySelection(const SAR_REGION currently_selected_sar_region);
    UINT GetAnatomyButtonInfo(const SAR_REGION sar_region, int& index_normal, int& index_selected, int& bitmap_index_offset, const int sar_region_type = 1)const ;
    void SetBitmapsForStatics();

    SAR_REGION GetCurrentlySelectedSarRegion()const;
    void SetCurrentlySelectedSarRegion(const SAR_REGION currently_selected_sar_region);

    void SetMultilingual();

    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #ifndef __SM_PQM_ANATOMY_DLG_H__

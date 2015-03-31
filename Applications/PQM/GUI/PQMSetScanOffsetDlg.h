//TMSC-REDMINE-783
//TMSC-REDMINE-780
//Redmine-775
//Redmine-777
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMSetScanOffsetDlg.h
 *  Overview: Structure of CPQMSetScanOffsetDlg class.
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
#ifndef __SM_PQM_SET_SCAN_OFFSET_DLG_H__
#define __SM_PQM_SET_SCAN_OFFSET_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <MPlusUICommon/ButtonEx.h>
#include <libStudyManager/NonCopyable.h>
#include <libStudyManager/UI/ZOrderWindow.h>

#include "CustomStatic.h"
#include "PQMEdit.h"
#include "ScanOffset/ScanOffsetVal.h"
#include "ScanOffset/MoveCouchWnd.h"

using namespace MPlus::UICommon ;

class CSequence;


// The class is used to display set scan offset dialog and to set scan offset.

class CPQMSetScanOffsetDlg : public MR::ACQ::SM::COMMON::UI::CZOrderDialog,
    private MR::ACQ::SM::COMMON::NonCopyable<CPQMSetScanOffsetDlg>
{
public:
    CPQMSetScanOffsetDlg(
        const COUCH_POSITION f_couch_pos,
        Vector_t f_current_scan_offset,
        Vector_t f_min_value,
        Vector_t f_max_value,
        PatientInsertDirection f_patient_in_or_out,
        PatientRotationDirection f_patient_side,
        CWnd* pParent = NULL
    ); // standard constructor
    ~CPQMSetScanOffsetDlg();

    const Vector_t GetScanOffsetVector();

protected:
    //{{AFX_VIRTUAL(CPQMSetScanOffsetDlg)
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    afx_msg void OnPaint();
    //}}AFX_VIRTUAL

    // Generated message map functions
    //{{AFX_MSG(CPQMSetScanOffsetDlg)
    virtual BOOL OnInitDialogImpl();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnMoveCouchLeft();
    afx_msg void OnMoveCouchRightButton();
    LRESULT OnUpdateCouchOffset(WPARAM wParam, LPARAM lParam);
    afx_msg void OnKillfocusEditScanOffsetX();
    afx_msg void OnKillfocusEditScanOffsetY();
    afx_msg void OnKillfocusEditScanOffsetZ();

    //}}AFX_MSG

private:

    ButtonEx m_ok;
    ButtonEx m_cancel;

    CCustomStatic m_image_offset_static;
    CCustomStatic m_move_couch_static;
    CCustomStatic m_move_couch_range_static;

    ButtonEx m_move_couch_left_button;
    ButtonEx m_move_couch_right_button;

    CPQMEdit m_lr_edit;
    CPQMEdit m_ap_edit;
    CPQMEdit m_move_couch_edit;

    CSequence* m_seq ;

    COUCH_POSITION m_couch_pos;

    CMoveCouchWnd m_move_couchwnd;
    CScanOffsetVal m_scan_offset;

    virtual BOOL PreTranslateMessageImpl(MSG* pMsg);

    void SetMultilingual();

    int GetPatientOrientationOptionFor(const OrientationType f_orientation_type, const int orientation_value)const;
    BOOL SetBitmapForButton(ButtonEx& button_object, const UINT bitmap_resource_id, const bool center_bitmap = true);

    void InitializeDlg();
    void InitCtrls();
    void InitData();
    void InitStaticCtrls();
    void UpdateMinMaxCouchRange();
    void InitButtons();
    void InitEditBoxes();
    void SetDialogRibbonHeight();

    bool ProcessEditBoxMsg(MSG* pMsg);
    bool ProcessMouseWheenMsg(MSG* pMsg);
    bool ProcessKeyDownMsg(MSG* pMsg);

    const ScanOffsetEnum GetScanOffsetEnumForWnd(const HWND& f_hwnd) const;

    void IncrementDecrementValue(const ScanOffsetEnum f_scanoffsetenum, const float f_increment = 1.f);
    void UpdateEditBoxAll();
    void UpdateEditBox(const ScanOffsetEnum f_scanoffsetenum);

    CWnd* GetEditBox(const ScanOffsetEnum f_scanoffsetenum);
    CString GetEditText(const ScanOffsetEnum f_scanoffsetenum);

    void PaintStaticImages(CDC* f_dc);

    void UpdateCouchOffset();

    void UpdateControlPos();

    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #ifndef __SM_PQM_SET_SCAN_OFFSET_DLG_H__

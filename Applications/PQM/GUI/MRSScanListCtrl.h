//TMSC-Redmine-783
//TMSC-Redmine-780
//Redmine-775
//Redmine-777
#ifndef __SM_MRS_SCAN_LISTCTRL_H__
#define __SM_MRS_SCAN_LISTCTRL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MRSScanListCtrl.h : header file
//

#include <MPlusUICommon/ListCtrlEx.h>
#include <MPlusUICommon/CheckBoxEx.h>
#include <libStudyManager/NonCopyable.h>

using namespace MPlus::UICommon ;


enum MRSSCAN_ROWITEM_TYPE {
    LOCAL_SHIMMING = 0,						//Patni-Ravindra Acharya/22Nov2010/Added/IR-168
    ADJ_CENTER_FREQ ,						//Patni-Ravindra Acharya/22Nov2010/Modified/IR-168
    SPECTRUM_GRAPH ,						//Patni-Ravindra Acharya/22Nov2010/Added/IR-168
    WATER_SIGNAL,
    WATER_SATURATION,
    TOTAL_MRSSCAN_ROWITEM
};

enum STATUS_TYPE {
    STATUS_WAIT = 0,
    STATUS_DONE,
    STATUS_FAIL,
    STATUS_CURRENT,
    STATUS_DISABLED,
    STATUS_TOTAL
};

struct MRSScanItem {

public:
    STATUS_TYPE m_item_status;
    bool m_checked_status;
    CString m_row_string;
    bool m_enable_ctrl ;

    void SetMRSScanItemData(const STATUS_TYPE item_status, const bool checked_status, const CString& row_string, const bool enable_ctrl) {

        m_item_status = item_status;
        m_checked_status = checked_status;
        m_row_string = row_string;
        m_enable_ctrl = enable_ctrl ;
    }
};


/////////////////////////////////////////////////////////////////////////////
// CMRSScanListCtrl window

class CMRSScanListCtrl : public ListCtrlEx,
    private MR::ACQ::SM::COMMON::NonCopyable<CMRSScanListCtrl>
{
public:
    CMRSScanListCtrl();
    virtual ~CMRSScanListCtrl();

    void InvalidateStatus(const MRSSCAN_ROWITEM_TYPE row_index);
    void SetSelection(const MRSSCAN_ROWITEM_TYPE row_index);

    void UpdateData()const;
    void DisableMRSScanListCtrl();
    BOOL InsertColumns();
    void InsertControls();

    bool IsMrsLicensePresent()const;
    void SetMrsLicensePresent(const bool f_mrsBRSTLicence_present);


    MRSScanItem* GetMRSScanItem(const MRSSCAN_ROWITEM_TYPE mrsscan_rowitem_type)const;
    void SetMRSScanData(MRSScanItem* mrsitem_array);

protected:
    //{{AFX_MSG(CMRSScanListCtrl)
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnShowSpectrumBtn();
    afx_msg void OnAdjFreqBtn() ;
    //}}AFX_MSG

private:
    bool scan_strt;
    bool m_mrsBRSTLicence_present ;
    MRSScanItem* m_mrsitem_array;
    CheckBoxEx m_mrsscan_checks[TOTAL_MRSSCAN_ROWITEM];

    virtual void MeasureItem(LPMEASUREITEMSTRUCT measure_item_struct);
    virtual void DrawItem(LPDRAWITEMSTRUCT draw_item_struct);
    virtual BOOL PreTranslateMessage(MSG* pMsg);

    void CenterRectForRowIndex(const int row_index, const int col_index, CRect& rect_tocenter, const bool forwidth = true, const bool forheight = true);
    void ToggleCheckForItem(const int row_index);
    void UpdateSelStatus(const int& row_index, const bool&  check_status, const bool&  enable_status) ;


    MRSScanItem* GetMRSScanItem(const int f_list_index)const;
    const MRSSCAN_ROWITEM_TYPE GetMRSIndexForListIndex(const int f_row_index) const;
    const int GetListIndexFor(const MRSSCAN_ROWITEM_TYPE f_row_index) const;

    CRect GetCellRect(const int row_no, const int col_no);
    CRect GetCheckRectForItem(const int index);
    UINT GetIconIDForStatus(const STATUS_TYPE status)const;

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #ifndef __SM_MRS_SCAN_LISTCTRL_H__

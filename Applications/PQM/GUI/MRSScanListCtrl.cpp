//REDMINE_782_CPP_Test_Tool
//TMSC-REDMINE-782
//TMSC-REDMINE-781
//TMSC-Redmine-780
// MRSScanListCtrl.cpp : implementation file
//TMSC-REDMINE-777

#include "stdafx.h"
#include "MRSScanListCtrl.h"
#include <PQM/resource.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define ITEM_HEIGHT 50

#define STATUS_COLUMN 0
#define CHECK_COLUMN 1

#define CHECK_TEXT_OFFSET 5

const int status_column_width = 50;

#define ICON_HEIGHT_WIDTH 32

#define SPEC_GRPAH_OFFSET 30

BEGIN_MESSAGE_MAP(CMRSScanListCtrl, ListCtrlEx)
    //{{AFX_MSG_MAP(CMRSScanListCtrl)
    ON_WM_MEASUREITEM_REFLECT()
    ON_WM_KEYDOWN()
    ON_BN_CLICKED(IDC_MRSDIALOG_SPECTRUM_GRAPH_CHECK, OnShowSpectrumBtn)		//Patni-Ravindra Acharya/14Dec2010/Added/IR-168
    ON_BN_CLICKED(IDC_MRSDIALOG_ADJ_CENTER_FREQ_CHECK, OnAdjFreqBtn)		//Patni-Ravindra Acharya/14Dec2010/Added/IR-168
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMRSScanListCtrl

CMRSScanListCtrl::CMRSScanListCtrl(
): scan_strt(false),
    m_mrsBRSTLicence_present(true),
    m_mrsitem_array(NULL)
{
}

CMRSScanListCtrl::~CMRSScanListCtrl()
{

}


/////////////////////////////////////////////////////////////////////////////
// CMRSScanListCtrl message handlers

//************************************Method Header************************************
// Method Name  : InValidateRow
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CMRSScanListCtrl::InvalidateStatus(
    const MRSSCAN_ROWITEM_TYPE row_index
)
{

    CRect cell_rect ;
    cell_rect = GetCellRect(GetListIndexFor(row_index), STATUS_COLUMN);
    InvalidateRect(&cell_rect);
}

void CMRSScanListCtrl::SetSelection(const MRSSCAN_ROWITEM_TYPE row_index)
{

    const int l_list_index = GetListIndexFor(row_index);

    SetSelectionMark(l_list_index);
    SetItemState(l_list_index, LVIS_SELECTED, LVIS_SELECTED);
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CMRSScanListCtrl::UpdateData(
)const
{
    m_mrsitem_array[ADJ_CENTER_FREQ].m_checked_status = m_mrsscan_checks[ADJ_CENTER_FREQ].GetCheck() ? true : false;
    m_mrsitem_array[WATER_SIGNAL].m_checked_status =  m_mrsscan_checks[WATER_SIGNAL].GetCheck() ? true : false;
    m_mrsitem_array[WATER_SATURATION].m_checked_status = m_mrsscan_checks[WATER_SATURATION].GetCheck() ? true : false;

    if (IsMrsLicensePresent()) {
        m_mrsitem_array[LOCAL_SHIMMING].m_checked_status = m_mrsscan_checks[LOCAL_SHIMMING].GetCheck() ? true : false;

        m_mrsitem_array[SPECTRUM_GRAPH].m_checked_status = m_mrsscan_checks[SPECTRUM_GRAPH].GetCheck() ? true : false ;
    }
}


//+Patni-RSG/ADDED/2009-May-15/JFT#268
//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ RSG
// Purpose      :
//***********************************************************************************
void CMRSScanListCtrl::DisableMRSScanListCtrl(
)
{
    scan_strt = true;
    m_mrsscan_checks[ADJ_CENTER_FREQ].EnableWindow(FALSE);
    m_mrsscan_checks[WATER_SIGNAL].EnableWindow(FALSE);
    m_mrsscan_checks[WATER_SATURATION].EnableWindow(FALSE);


    if (IsMrsLicensePresent()) {
        m_mrsscan_checks[LOCAL_SHIMMING].EnableWindow(FALSE);
        m_mrsscan_checks[SPECTRUM_GRAPH].EnableWindow(FALSE) ;
    }
}
//-Patni-RSG/ADDED/2009-May-15/JFT#268

BOOL CMRSScanListCtrl::InsertColumns(
)
{
    //LoadStatusIcons();

    CRect client_area;
    GetClientRect(&client_area);

    InsertColumn(0, _T("1"), LVCFMT_CENTER, status_column_width, status_column_width);
    InsertColumn(1, _T("2 "), LVCFMT_CENTER, client_area.Width() - status_column_width, client_area.Width() - status_column_width);

    return TRUE ;
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CMRSScanListCtrl::InsertControls(
)
{

    const UINT l_checkbox_id [] = {
        IDC_MRSDIALOG_LOCAL_SHIMMING_CHECK,
        IDC_MRSDIALOG_ADJ_CENTER_FREQ_CHECK,
        IDC_MRSDIALOG_SPECTRUM_GRAPH_CHECK,
        IDC_MRSDIALOG_WATER_SIGNAL_REF_CHECK,
        IDC_MRSDIALOG_WATER_SATURATION_REF_CHECK
    };


    int l_row_counter = 0;
    CRect check_rect ;

    for (int i = 0 ; i < TOTAL_MRSSCAN_ROWITEM ; i++) {

        if (!m_mrsitem_array[i].m_row_string.IsEmpty()) {

            InsertItem(l_row_counter, _T(""));
            check_rect = GetCheckRectForItem(l_row_counter);

            if (l_checkbox_id[i] == IDC_MRSDIALOG_SPECTRUM_GRAPH_CHECK)
                check_rect.OffsetRect(SPEC_GRPAH_OFFSET, 0);

            m_mrsscan_checks[i].Create(_T("") , WS_CHILD | WS_VISIBLE  | BS_CHECKBOX, check_rect, this, l_checkbox_id[i]);
            m_mrsscan_checks[i].SetCheck(m_mrsitem_array[i].m_checked_status);
            m_mrsscan_checks[i].EnableWindow(m_mrsitem_array[i].m_enable_ctrl);

            l_row_counter ++;
        }
    }

}


bool CMRSScanListCtrl::IsMrsLicensePresent()const
{
    return m_mrsBRSTLicence_present;
}

void CMRSScanListCtrl::SetMrsLicensePresent(
    const bool f_mrsBRSTLicence_present
)
{
    m_mrsBRSTLicence_present = f_mrsBRSTLicence_present ;
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
MRSScanItem* CMRSScanListCtrl::GetMRSScanItem(
    const MRSSCAN_ROWITEM_TYPE mrsscan_rowitem_type
)const
{
    if ((mrsscan_rowitem_type >= LOCAL_SHIMMING) && (mrsscan_rowitem_type < TOTAL_MRSSCAN_ROWITEM)) {

        return &(m_mrsitem_array[mrsscan_rowitem_type]);
    }

    return NULL;
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CMRSScanListCtrl::SetMRSScanData(
    MRSScanItem* mrsitem_array
)
{
    m_mrsitem_array = mrsitem_array;
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CMRSScanListCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // TODO: Add your message handler code here and/or call default

    ListCtrlEx::OnKeyDown(nChar, nRepCnt, nFlags);

    switch (nChar) {

        case VK_SPACE:

            if (!scan_strt) {
                int index = GetSelectionMark();

                //+Patni-Ravindra Acharya/14Dec2010/Modified/IR-168
                if (FALSE == m_mrsscan_checks[index].IsWindowEnabled()) {
                    break ;
                }

                ToggleCheckForItem(index);

                switch (index) {

                    case SPECTRUM_GRAPH:
                        OnShowSpectrumBtn();
                        break;

                    case ADJ_CENTER_FREQ :
                        OnAdjFreqBtn();
                        break;
                }

                //-Patni-Ravindra Acharya/14Dec2010/Modified/IR-168

            }

            break;

    }

    ListCtrlEx::OnKeyDown(nChar, nRepCnt, nFlags);
}



//+Patni-Ravindra Acharya/14Dec2010/Added/IR-168
//************************************Method Header************************************
// Method Name  : OnShowSpectrumBtn
// Author       : Ravindra Acharya
// Purpose      : Event Handler of Show Spectrum Check Box
//***********************************************************************************
void CMRSScanListCtrl::OnShowSpectrumBtn()
{
    // TODO: Add your control notification handler code here
    if (m_mrsscan_checks[SPECTRUM_GRAPH].GetCheck())
        UpdateSelStatus(LOCAL_SHIMMING, true, false) ;

    else
        m_mrsscan_checks[LOCAL_SHIMMING].EnableWindow(TRUE);
}
//-Patni-Ravindra Acharya/14Dec2010/Added/IR-168


//+Patni-Ravindra Acharya/14Dec2010/Added/IR-168
//************************************Method Header************************************
// Method Name  : OnAdjFreqBtn
// Author       : Ravindra Acharya
// Purpose      : Event Handler of Adjust Center Frequency Check Box
//***********************************************************************************
void CMRSScanListCtrl::OnAdjFreqBtn()
{
    bool b_adj_cent_fre_check = (m_mrsscan_checks[ADJ_CENTER_FREQ].GetCheck()) > 0;
    UpdateSelStatus(SPECTRUM_GRAPH, b_adj_cent_fre_check , b_adj_cent_fre_check) ;
    OnShowSpectrumBtn();
}
//-Patni-Ravindra Acharya/14Dec2010/Added/IR-168


void CMRSScanListCtrl::MeasureItem(
    LPMEASUREITEMSTRUCT measure_item_struct
)
{

    measure_item_struct->itemHeight = ITEM_HEIGHT;
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CMRSScanListCtrl::DrawItem(
    LPDRAWITEMSTRUCT draw_item_struct
)
{

    ListCtrlEx::DrawItem(draw_item_struct);

    CDC dc;
    dc.Attach(draw_item_struct->hDC);
    const int prev_mode = dc.SetBkMode(OPAQUE);

    CRect item_rect = draw_item_struct->rcItem ;

    //+Patni-HAR/2010May11/Added/MVC007466
    COLORREF clrText;
    COLORREF clrBk;
    const bool bSelected = (ODS_SELECTED == (draw_item_struct->itemState & ODS_SELECTED));
    GetCellColors(draw_item_struct->itemID, 0, bSelected, clrText, clrBk);
    dc.SetBkColor(clrBk);
    dc.SetTextColor(clrText);
    CBrush bgBrush(clrBk);
    //-Patni-HAR/2010May11/Added/MVC007466


    CRect cellrect = GetCellRect(draw_item_struct->itemID, STATUS_COLUMN);
    cellrect.right = cellrect.left + ICON_HEIGHT_WIDTH;
    cellrect.bottom = cellrect.top + ICON_HEIGHT_WIDTH;
    CenterRectForRowIndex(draw_item_struct->itemID, STATUS_COLUMN, cellrect);

    MRSScanItem* mrsscanitem = GetMRSScanItem(draw_item_struct->itemID);
    UINT icon_id = GetIconIDForStatus(mrsscanitem->m_item_status);

    if (icon_id > 0) {

        HICON icon = AfxGetApp()->LoadIcon(icon_id);
        dc.DrawIcon(cellrect.left, cellrect.top, icon);
    }

    CRect check_rect = GetCheckRectForItem(draw_item_struct->itemID);
    cellrect = GetCellRect(draw_item_struct->itemID, CHECK_COLUMN);
    cellrect.left = check_rect.right + CHECK_TEXT_OFFSET;


    //+Patni-Ravindra Acharya/02Dec2010/Added/IR-168
    if (IsMrsLicensePresent()) {
        if (SPECTRUM_GRAPH == draw_item_struct->itemID) {
            cellrect.OffsetRect(SPEC_GRPAH_OFFSET, 0);
        }
    }

    //-Patni-Ravindra Acharya/02Dec2010/Added/IR-168
    cellrect.top += 1;
    dc.FillRect(&cellrect, &bgBrush);
    dc.DrawText(mrsscanitem->m_row_string , cellrect, DT_PATH_ELLIPSIS | DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    dc.SetBkMode(prev_mode);
    dc.Detach() ;
}

//+Patni-SS/ADDED/2009-May-15/JFT#115
BOOL CMRSScanListCtrl::PreTranslateMessage(
    MSG* pMsg)
{

    if (pMsg->message == WM_SYSKEYDOWN) {
        if (pMsg->wParam == VK_F4 && (pMsg->wParam & VK_MENU)) {
            pMsg->wParam = NULL;
            return false;
        }
    }


    return ListCtrlEx::PreTranslateMessage(pMsg);
}


//************************************Method Header************************************
// Method Name  : CenterRectForRowIndex
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CMRSScanListCtrl::CenterRectForRowIndex(
    const int row_index,
    const int col_index,
    CRect& rect_tocenter,
    const bool forwidth /*= true*/,
    const bool forheight /*= true*/
)
{

    CRect rect(0, 0, 0, 0) ;

    if ((row_index >= 0) && (row_index < GetItemCount())) {

        rect = GetCellRect(row_index , col_index) ;

        const int height = rect_tocenter.Height() ;
        const int width = rect_tocenter.Width() ;

        const int height_cell = rect.Height() ;
        const int width_cell = rect.Width() ;

        if (forwidth) {

            rect_tocenter.left = rect.left + (long)((width_cell - width) * 0.5f) ;
            rect_tocenter.right = rect_tocenter.left + width;
        }

        if (forheight) {

            rect_tocenter.top = rect.top + (long)((height_cell - height) * 0.5f) ;
            rect_tocenter.bottom = rect_tocenter.top + height;
        }
    }
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CMRSScanListCtrl::ToggleCheckForItem(const int row_index)
{

    bool new_state = !m_mrsscan_checks[row_index].GetCheck();
    m_mrsscan_checks[row_index].SetCheck(new_state);
    m_mrsitem_array[row_index].m_checked_status = new_state;
}



//+Patni-Ravindra Acharya/14Dec2010/Added/IR-168
//************************************Method Header************************************
// Method Name  : UpdateSelStatus
// Author       : Ravindra Acharya
// Purpose      : To Update the Check Mark Status and Display Status of the Specified Check Box
//***********************************************************************************
void CMRSScanListCtrl::UpdateSelStatus(const int& row_index, const bool&  check_status, const bool&  enable_status)
{
    m_mrsscan_checks[row_index].SetCheck(check_status) ;
    m_mrsscan_checks[row_index].EnableWindow(enable_status) ;

}
//-Patni-Ravindra Acharya/14Dec2010/Added/IR-168


MRSScanItem* CMRSScanListCtrl::GetMRSScanItem(
    const int f_list_index
)const
{


    return GetMRSScanItem(GetMRSIndexForListIndex(f_list_index));

}

const MRSSCAN_ROWITEM_TYPE CMRSScanListCtrl::GetMRSIndexForListIndex(
    const int f_row_index
) const
{

    const int l_list_index = (int) f_row_index;

    int l_data_index = -1;

    for (int i = 0 ; i < TOTAL_MRSSCAN_ROWITEM ; i++) {

        if (!m_mrsitem_array[i].m_row_string.IsEmpty()) {

            l_data_index ++;
        }

        if (l_list_index == l_data_index) {

            return MRSSCAN_ROWITEM_TYPE(i);
        }
    }

    return TOTAL_MRSSCAN_ROWITEM;

}

const int CMRSScanListCtrl::GetListIndexFor(
    const MRSSCAN_ROWITEM_TYPE f_row_index
) const
{

    int l_list_index = -1;

    for (int i = 0 ; i <= f_row_index ; i++) {

        if (!m_mrsitem_array[i].m_row_string.IsEmpty()) {
            l_list_index ++;
        }
    }

    return l_list_index;
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CRect CMRSScanListCtrl::GetCellRect(
    const int row_no,
    const int col_no
)
{
    if (row_no >= GetItemCount()) {
        ASSERT(FALSE) ;
        return CRect(0, 0, 0, 0);
    }

    CRect rect;
    GetSubItemRect(row_no, col_no, LVIR_BOUNDS | LVIR_LABEL, rect);

    rect.DeflateRect(1, 1) ;

    return rect;
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CRect CMRSScanListCtrl::GetCheckRectForItem(
    const int index
)
{

    CRect cell_rect = GetCellRect(index, CHECK_COLUMN);
    CRect check_rect = cell_rect;
    check_rect.left += 5;
    check_rect.right = check_rect.left + 20;
    check_rect.bottom = check_rect.top + 20;

    CenterRectForRowIndex(index, CHECK_COLUMN, check_rect, false, true);

    return check_rect;
}



//************************************Method Header************************************
// Method Name  : GetIconIDForStatus
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
UINT CMRSScanListCtrl::GetIconIDForStatus(
    const STATUS_TYPE status
)const
{

    switch (status) {

        case STATUS_WAIT :
            return IDI_WAIT_ICON;

        case STATUS_DONE:
            return IDI_DONE_ICON;

        case STATUS_FAIL:
            return IDI_FAIL_ICON;

        case STATUS_CURRENT:
            return IDI_CURRENT_ICON;

        case STATUS_DISABLED :
            return IDI_DISABLED_ICON;
    }

    return -1;
}
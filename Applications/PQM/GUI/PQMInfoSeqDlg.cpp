//TMSC-REDMINE-782
//TMSC-REDMINE-781
//TMSC-REDMINE-780
//TMSC-REDMINE-778
//TMSC-REDMINE-777
//TMSC-REDMINE-774
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMInfoSeqDlg.cpp
 *  Overview: Implementation of CPQMInfoSeqDlg class.
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
#include "stdafx.h"
#include "PQMInfoSeqDlg.h"
#include <MPlusI18n/MPlusI18nCatalog.h> // Added By KT/ACE-2/15-06-2009/Multilingualisation
#include <libStudyManager/UI/ZOrderLayerFactory.h>
#include <PQM/resource.h>
#include "PQMView.h"
#include "PQMUtility.h"
#include "DPSManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using MR::ACQ::SM::COMMON::UI::CZOrderDialog;
using MR::ACQ::SM::COMMON::UI::CZOrderLayerFactory;

BEGIN_MESSAGE_MAP(CPQMInfoSeqDlg, CZOrderDialog)
    //{{AFX_MSG_MAP(CPQMInfoSeqDlg)
    ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
    //}}AFX_MSG_MAP

    //+Patni-Raghavendra/2012Jan31/Added/REDMINE-2895
    ON_WM_SIZE()
    ON_WM_GETMINMAXINFO()
    ON_WM_SETCURSOR()
    //-Patni-Raghavendra/2012Jan31/Added/REDMINE-2895

END_MESSAGE_MAP()

CRect CPQMInfoSeqDlg::m_orgwindowrect = CRect(0, 0, 0, 0);

//************************************Method Header****************************
//Method Name:  CPQMInfoSeqDlg()
//Author:  PATNI/GP
//Purpose: Constructor
//****************************************************************************/
CPQMInfoSeqDlg::CPQMInfoSeqDlg(CWnd* pParent /*=NULL*/)

    : CZOrderDialog(CZOrderLayerFactory::GetIselLayer(),
                    CDPSManager::GetInstance(), IDD_INFO_SEQ, pParent),
    m_parent(pParent),

    m_org_client_rect(CRect(0, 0, 0, 0)),
    m_show_wait_for_infoseqdlg(false)
{

}
//********************************Method Header********************************
//Method Name   :OnSize()
//Author        :PATNI/Raghavendra
//Purpose       :
//*****************************************************************************
void CPQMInfoSeqDlg::OnSize(UINT nType, int cx, int cy)
{
    CZOrderDialog::OnSize(nType, cx, cy);
    Resize(cx, cy);
}
//************************************Method Header************************************
// Method Name  : OnSetCursor
// Author       : iGATE
// Purpose      :
//***********************************************************************************
BOOL CPQMInfoSeqDlg::OnSetCursor(CWnd* pWnd,
                                 UINT nHitTest,
                                 UINT message)
{
    if (m_show_wait_for_infoseqdlg) {
        ::SetCursor(::LoadCursor(NULL, IDC_WAIT));
        return TRUE ;
    }

    return CDialog::OnSetCursor(pWnd, nHitTest, message);

}
//-Patni-Raghavendra/2012Jan31/Added/REDMINE-2895
//********************************Method Header********************************
//Method Name   :OnGetMinMaxInfo()
//Author        :PATNI/Raghavendra
//Purpose       :
//*****************************************************************************
void CPQMInfoSeqDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
    // TODO: Add your message handler code here and/or call default


    if (!m_org_client_rect.IsRectEmpty()) {
        lpMMI->ptMinTrackSize.x = m_org_client_rect.Width() ;
        lpMMI->ptMinTrackSize.y = m_org_client_rect.Height() ;
    }



    CZOrderDialog::OnGetMinMaxInfo(lpMMI);
}


//************************************Method Header****************************
//Method Name:  DisplaySeqInformation
//Author:  PATNI/GP
//Purpose: DisplaySeqInformation
//****************************************************************************/
void CPQMInfoSeqDlg::DisplaySeqInformation(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMInfoSeqDlg::DisplaySeqInformation");
    PQM_TRACE_SCOPED(FUNC_NAME);


    LVITEM l_lvitem;
    l_lvitem.mask         = LVCF_WIDTH  | LVIF_TEXT;
    l_lvitem.state        = 0;
    l_lvitem.stateMask    = 0;

    CPQMListCtrl& list_ctrl = ((CPQMView*)m_parent)->GetPQMListCtrl();

    m_seq_info_list.DeleteAllItems();

    //+Patni/Nitin/ACE2/ACE2-Skip Precan Requirement
    //reset the seq info edit
    if (m_seqinfo_edit.IsWindowVisible()) {
        m_seqinfo_edit.SetWindowText(_T(""));
    }

    //-Patni/Nitin/ACE2/ACE2-Skip Precan Requirement

    //+Patni-MP & NP/2010Jul10/Modified/MVC008324-2
    const int sel_index = ((CPQMView*)m_parent)->GetSelectedSeqIndex();

    try {

        ((CPQMView*)m_parent)->UpdateSarInfo();

        if (sel_index != -1)	{

            std::vector<SeqInfo*> l_vSeqInfo;
            std::vector<SeveralCols*> l_vSeveralCols;

            //+Patni-PP/2009Jun01/Modified/SU14 CPP test initialization corrections
            std::vector<SeqInfo*>::iterator l_itSeqInfo = NULL;
            std::vector<SeveralCols*>::iterator l_itSeveralCols = NULL;
            //+Patni-PP/2009Jun01/Modified/SU14 CPP test initialization corrections

            SeqInfo* l_pqm_seq_info = NULL;
            SeveralCols* l_pqm_seq_columns = NULL;
            list_ctrl.GetAppDictionaryConvertedValueList(sel_index);

            l_vSeqInfo = list_ctrl.GetPairedVector();

            int l_param_count = 0;

            for (l_itSeqInfo = l_vSeqInfo.begin();
                 l_itSeqInfo != l_vSeqInfo.end(); l_itSeqInfo++) {

                l_lvitem.iItem        = l_param_count;
                l_lvitem.iSubItem     = 0;

                l_pqm_seq_info = (SeqInfo*) * l_itSeqInfo;

                if (l_pqm_seq_info == NULL)
                    continue;

                l_lvitem.pszText = l_pqm_seq_info->strParameterLabel.GetBuffer(l_pqm_seq_info->strParameterLabel.GetLength() + 1);

                m_seq_info_list.InsertItem(&l_lvitem);

                l_vSeveralCols = l_pqm_seq_info->v_SeveralColumns;

                for (l_itSeveralCols = l_vSeveralCols.begin();
                     l_itSeveralCols != l_vSeveralCols.end(); l_itSeveralCols++
                    ) {

                    l_pqm_seq_columns =  *l_itSeveralCols;

                    if (l_pqm_seq_columns == NULL)
                        continue;

                    m_seq_info_list.SetItemText(l_pqm_seq_columns->param_count, l_pqm_seq_columns->column_number,
                                                l_pqm_seq_columns->strConvertedValue.GetBuffer(MAX_PATH));
                }

                l_param_count++;
            }

            l_vSeqInfo.clear();
            list_ctrl.Clear();
            //SM3 Defect fix 619 End Ashish


            //+Patni/Nitin/ACE2/ACE2-Skip Precan Requirement
            UpdateSeqinfoEdit(sel_index);
            //-Patni/Nitin/ACE2/ACE2-Skip Precan Requirement

        }

    } catch (...) {

        PQM_TRACE(COMMHANDLER_MARKER_PQM_DPS, FUNC_NAME,
                  _T("Unknown Exception occured."));
    }

    //+Patni-MP & NP/2010Jul10/Modified/MVC008324-2


    m_seqinfo_edit.EnableWindow((sel_index != -1));
}

//************************************Method Header************************************
// Method Name  : SetWaitCursor
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMInfoSeqDlg::SetWaitCursor(
    const bool f_show_wait_cursor
)
{
    m_show_wait_for_infoseqdlg = f_show_wait_cursor;
}


void CPQMInfoSeqDlg::DoDataExchange(
    CDataExchange* pDX
)
{
    CZOrderDialog::DoDataExchange(pDX);

    //{{AFX_DATA_MAP(CPQMInfoSeqDlg)
    DDX_Control(pDX, IDC_SEQ_INFO_LIST, m_seq_info_list);
    DDX_Control(pDX, IDC_BUTTON_CLOSE, m_c_close);
    DDX_Control(pDX, IDC_SEQINFO_EDIT, m_seqinfo_edit);
    //}}AFX_DATA_MAP
}


/////////////////////////////////////////////////////////////////////////////
// CPQMInfoSeqDlg message handlers
//************************************Method Header****************************
//Method Name:  OnInitDialogImpl
//Author:  PATNI/gP
//Purpose: Initialize dialog and its controls.
//****************************************************************************/
BOOL CPQMInfoSeqDlg::OnInitDialogImpl(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMInfoSeqDlg::OnInitDialogImpl");
    PQM_TRACE_SCOPED(FUNC_NAME);



    CString title = CPQMUtility::GetMultiLingualString(_T("IDS_RES_MEM_GUI_INFO_SEQ_TITLE"));

    if (!title.IsEmpty()) {
        CWnd ::SetWindowText(title);
    }

    CRect list_rect ;
    m_seq_info_list.GetWindowRect(&list_rect);

    // Inserting Columns
    //
    const int single_coulmn_width = (list_rect.Width() - 30) / TOTAL_SEQ_INFO_COLUMNS;

    for (int j = 0; j < TOTAL_SEQ_INFO_COLUMNS; j++)    {
        m_seq_info_list.InsertColumn(j, SEQ_INFO_COLUMN_STRINGS[j], LVCFMT_LEFT, single_coulmn_width);
    }

    //+Patni/Nitin/ACE2/ACE2-Skip Precan Requirement
    m_seqinfo_edit.ShowWindow((GetKeyState(VK_CONTROL) < 0) ? SW_SHOW : SW_HIDE);
    m_seqinfo_edit.SetLimitText(128);
    //-Patni/Nitin/ACE2/ACE2-Skip Precan Requirement

    //Added by Hemant On 1/21/2009 11:07:34 AM
    m_seq_info_list.SetBkColor(BACKGROUNDCOLOR);
    m_seq_info_list.SetTextBkColor(BACKGROUNDCOLOR);
    m_seq_info_list.SetTextColor(TEXTCOLOR);
    m_seq_info_list.SetFont(_T("Arial"), 12);

    if (m_orgwindowrect.IsRectEmpty()) {
        GetWindowRect(&m_orgwindowrect) ;
    }

    if (m_org_client_rect.IsRectEmpty()) {
        GetWindowRect(m_org_client_rect);
    }

    SetWindowPos(NULL, m_orgwindowrect.left, m_orgwindowrect.top, m_orgwindowrect.Width(), m_orgwindowrect.Height(), SWP_NOZORDER);

    CRect l_client_rect;
    GetClientRect(&l_client_rect) ;
    Resize(l_client_rect.Width() , l_client_rect.Height()) ;
    SetButtonAppearence();
    SetDialogRibbonHeight();

    SetMultilingual();//Language usha
    DisplaySeqInformation();

    return TRUE;
}
//********************************Method Header********************************
//Method Name   :Resize()
//Author        :PATNI/Raghavendra
//Purpose       :
//*****************************************************************************
void CPQMInfoSeqDlg::Resize(
    const int f_cx,
    const int f_cy
)
{
    //must do this , as when minimizing and then agina restoring .. gives some incorrect sizing for some controls..
    if (f_cx <= 0 &&  f_cy <= 0)
        return ;

    const int niGapX = 10 ;

    CRect clWindowRect ;
    GetWindowRect(&clWindowRect) ;
    ScreenToClient(&clWindowRect) ;

    CWnd* clpWnd =  NULL;
    CRect clChildWndRect(0, 0, 0, 0);

    int niIncrementX = 0, niIncrementY = 0;

    if (clpWnd = GetDlgItem(IDC_BUTTON_CLOSE)) {
        clpWnd->GetWindowRect(clChildWndRect) ;
        ScreenToClient(clChildWndRect) ;

        niIncrementY = clWindowRect.bottom - clChildWndRect.bottom - niGapX ;

        niIncrementX = clWindowRect.right - clChildWndRect.right - niGapX;

    }


    OffsetChild(IDC_BUTTON_CLOSE, CSize(niIncrementX, niIncrementY)) ;
    OffsetChild(IDC_SEQINFO_EDIT, CSize(0, niIncrementY)) ;

    IncrementSizeChild(IDC_SEQ_INFO_LIST, CSize(niIncrementX , niIncrementY)) ;

    CRect list_rect ;
    m_seq_info_list.GetWindowRect(&list_rect);

    const int column_increment = niIncrementX / TOTAL_SEQ_INFO_COLUMNS;

    for (int j = 0; j < TOTAL_SEQ_INFO_COLUMNS; j++)    {

        m_seq_info_list.SetColumnWidth(j, m_seq_info_list.GetColumnWidth(j) + column_increment);
    }


}
//********************************Method Header********************************
//Method Name   :OffsetChild()
//Author        :PATNI/Raghavendra
//Purpose       :
//*****************************************************************************
void CPQMInfoSeqDlg::OffsetChild(
    const UINT f_ctrlid,
    const CSize& f_offset
)
{
    CWnd* clpWnd = NULL ;

    if (clpWnd = GetDlgItem(f_ctrlid)) {
        CRect clChildWndRect ;

        clpWnd->GetWindowRect(clChildWndRect) ;
        ScreenToClient(clChildWndRect) ;

        clChildWndRect.OffsetRect(f_offset) ;
        clpWnd->SetWindowPos(NULL , clChildWndRect.left , clChildWndRect.top , clChildWndRect.Width() , clChildWndRect.Height() , SWP_NOZORDER | SWP_NOSIZE) ;
    }
}
//********************************Method Header********************************
//Method Name   :IncrementSizeChild()
//Author        :PATNI/Raghavendra
//Purpose       :
//*****************************************************************************
void CPQMInfoSeqDlg::IncrementSizeChild(
    const UINT f_ctrlid,
    const CSize& f_inc_size
)
{
    CWnd* clpWnd = NULL ;

    if (clpWnd = GetDlgItem(f_ctrlid)) {
        CRect clChildWndRect ;
        clpWnd->GetWindowRect(clChildWndRect) ;
        ScreenToClient(clChildWndRect) ;

        clChildWndRect.right += f_inc_size.cx ;
        clChildWndRect.bottom += f_inc_size.cy ;

        clpWnd->SetWindowPos(NULL , clChildWndRect.left , clChildWndRect.top , clChildWndRect.Width() , clChildWndRect.Height() , SWP_NOZORDER | SWP_NOMOVE) ;
    }
}


//************************************Method Header****************************
//Method Name:  OnButtonClose
//Author:  PATNI/GP
//Purpose: Close sequence information dialog
//****************************************************************************/
void CPQMInfoSeqDlg::OnButtonClose(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMInfoSeqDlg::OnButtonClose");
    PQM_TRACE_SCOPED(FUNC_NAME);

    CPQMLogMgr::GetInstance()->WriteOpeLog(OP_IDB_SEQ_INFO_CLOSE);
    GetWindowRect(&m_orgwindowrect) ;//

    DestroyWindow();
}

BOOL CPQMInfoSeqDlg::PreTranslateMessageImpl(
    MSG* pMsg
)
{
    if (m_show_wait_for_infoseqdlg) {
        if ((pMsg->message == WM_KEYUP) ||
            (pMsg->message == WM_KEYDOWN) ||
            (pMsg->message == WM_LBUTTONDOWN) ||
            (pMsg->message == WM_RBUTTONDOWN) ||
            (pMsg->message == WM_NCLBUTTONDOWN) ||
            (pMsg->message == WM_LBUTTONDBLCLK) ||
            (pMsg->message == WM_RBUTTONDBLCLK) ||
            (pMsg->message == WM_MOUSEWHEEL))

            return TRUE ;
    }

    //+Patni- Hemant/Manishkumar/Added/To correct the appearance of dialog after hitting "ENTER"
    //+Patni/Nitin/ACE2/ACE2-Skip Precan Requirement
    if ((WM_KEYDOWN == pMsg->message) && (VK_RETURN == pMsg->wParam)) {

        if (pMsg->hwnd == m_seqinfo_edit.m_hWnd) {

            SaveSeqinfo();

            return TRUE;

        } else {
            pMsg->wParam = NULL;
        }
    }

    //-Patni/Nitin/ACE2/ACE2-Skip Precan Requirement

    //+Patni-Abhishek/2010Dec18/Added/Redmine-1074_part1
    if (pMsg->message == WM_RBUTTONDOWN && pMsg->hwnd == m_seqinfo_edit.m_hWnd) {
        return TRUE;
    }

    //-Patni-Abhishek/2010Dec18/Added/Redmine-1074_part1

    return CZOrderDialog::PreTranslateMessageImpl(pMsg);
}



//+Patni/Nitin/ACE2/ACE2-Skip Precan Requirement
void CPQMInfoSeqDlg::UpdateSeqinfoEdit(
    const int f_seq_index
)
{

    if (f_seq_index != -1) {

        CPQMView* l_parent_view = (CPQMView*)m_parent ;

        if (l_parent_view) {
            m_seqinfo_edit.SetWindowText(l_parent_view->GetSequenceInfo());
        }
    }
}

void CPQMInfoSeqDlg::SaveSeqinfo(
)const
{
    if (m_seqinfo_edit.IsWindowVisible()) {

        CString l_skip_prescan ;
        m_seqinfo_edit.GetWindowText(l_skip_prescan);

        l_skip_prescan.TrimLeft();
        l_skip_prescan.TrimRight();

        if (m_parent) {
            ((CPQMView*)m_parent)->SetSequenceInfo(l_skip_prescan);
        }
    }

}
//-Patni/Nitin/ACE2/ACE2-Skip Precan Requirement

//Language usha
void CPQMInfoSeqDlg::SetMultilingual()
{
    //dialog title
    CString l_chTitle =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_INFO_SEQ_DLG_TITLE"));

    if (_tcscmp(l_chTitle , _T("")) != 0) {
        CWnd ::SetWindowText(l_chTitle);
    }

    CString l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_SAR_BUTTON_CLOSE"));
    HWND l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDC_BUTTON_CLOSE);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMInfoSeqDlg::SetButtonAppearence(
)
{
    SetButtonFont();
    m_c_close.SetCurveType(CURVE_LARGE_5);
}


//************************************Method Header************************************
// Method Name  : SetButtonFont
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
BOOL CPQMInfoSeqDlg::SetButtonFont(
)
{
    LPCTSTR button_font_name = _T("Verdana BOLD") ;
    const int size = 9;

    m_c_close.SetPrimaryTextFont(button_font_name, size);
    m_c_close.SetPrimaryTextAllign(TUSBUTTON_ALIGN_HCENTER_VCENTER);

    m_seqinfo_edit.SetFont(button_font_name, size);

    return TRUE;
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMInfoSeqDlg::SetDialogRibbonHeight(
)
{
    CRect client_rect;
    GetClientRect(&client_rect);

    CRect ok_button_rect ;
    m_c_close.GetWindowRect(&ok_button_rect);
    ScreenToClient(&ok_button_rect);


    SetRibbonHeight(client_rect.bottom - ok_button_rect.top + 5);
}




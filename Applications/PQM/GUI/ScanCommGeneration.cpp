//TMSC-REDMINE-782
//TMSC-REDMINE-781
//TMSC-REDMINE-777
//TMSC-REDMINE-779
//TMSC-REDMINE-780
// ScanCommGeneration.cpp: implementation of the CScanCommGeneration class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "ScanCommGeneration.h"
#include <libStudyManager/UI/ZOrderLayerFactory.h>
#include "DPSManager.h"
#include "PQMCommentDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using MR::ACQ::SM::COMMON::UI::CZOrderDialog;
using MR::ACQ::SM::COMMON::UI::CZOrderLayerFactory;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//Patni-AJS/2010Oct01/Added/IR-163_Part2
CRect CScanCommGeneration::m_orgwindowrect = CRect(0, 0, 0, 0);


BEGIN_MESSAGE_MAP(CScanCommGeneration, CZOrderDialog)
    //{{AFX_MSG_MAP(CScanCommGeneration)
    ON_LBN_SELCHANGE(IDC_LIST_SEQ, OnSelchangeListSeq)
    ON_LBN_SELCHANGE(IDC_LIST_CONT, OnSelchangeListCont)
    ON_LBN_SELCHANGE(IDC_LIST_OPT, OnSelchangeListOpt)
    ON_WM_DESTROY()

    ON_WM_SIZE()
    ON_WM_GETMINMAXINFO()

    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CScanCommGeneration dialog

//********************************Method Header********************************
//Method Name   :CScanCommentXMLReader()
//Author        :PATNI/AJS
//Purpose       :
//*****************************************************************************
CScanCommGeneration::CScanCommGeneration(CWnd* pParent, ScanCommentXMLData* f_scan_dialog_data
                                        )
    : CZOrderDialog(CZOrderLayerFactory::GetIselLayer(),
                    CDPSManager::GetInstance(), IDD_SCAN_COMMENT_XML_DIALOG, pParent),
    m_parent_wnd((CPQMCommentDlg*) pParent),
    m_scan_dialog_data(f_scan_dialog_data),
    m_org_client_rect(CRect(0, 0, 0, 0))
{
}

//********************************Method Header********************************
//Method Name   :CScanCommentXMLReader()
//Author        :PATNI/AJS
//Purpose       :
//*****************************************************************************
CScanCommGeneration::~CScanCommGeneration()
{
}


//********************************Method Header********************************
//Method Name   :RefreshDialogCtrls()
//Author        :PATNI/AJS
//Purpose       :
//*****************************************************************************
void CScanCommGeneration::RefreshDialogCtrls(
    const CString& f_comm_str
)
{

    UnCheckListItems();

    m_comm_val.SetWindowText(f_comm_str);

    int l_start = 0, l_value = 0;

    //TA Review Comment_9 - l_str come out of the loop
    CString l_str = _T("");

    while (-1 != l_value) {

        l_value = f_comm_str.Find(_T(','), l_start);

        if (-1 == l_value) {
            l_str = f_comm_str.Mid(l_start, f_comm_str.GetLength());

        } else {
            l_str = f_comm_str.Mid(l_start, l_value - l_start);
        }

        CheckListItem(l_str);
        l_start = l_value + 1;
    }

}

void CScanCommGeneration::EnableDisableApply(
    const bool f_apply_state
)
{

    if (::IsWindow(m_hWnd)) {
        GetDlgItem(IDOK)->EnableWindow(f_apply_state);
    }

}


void CScanCommGeneration::DoDataExchange(CDataExchange* pDX)
{
    CZOrderDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CScanCommGeneration)
    DDX_Control(pDX, IDC_COMM_VALUE, m_comm_val);
    DDX_Control(pDX, IDC_LIST_OPT, m_opt_list);
    DDX_Control(pDX, IDC_LIST_CONT, m_cont_list);
    DDX_Control(pDX, IDC_LIST_SEQ, m_seq_list);
    DDX_Control(pDX, IDOK, m_ok_button);
    DDX_Control(pDX, IDCANCEL, m_cancel_button);
    DDX_Control(pDX, IDC_LIST_OPT_STATIC, m_opt_static);
    DDX_Control(pDX, IDC_LIST_CONT_STATIC, m_cont_static);
    DDX_Control(pDX, IDC_LIST_SEQ_STATIC, m_seq_static);

    //}}AFX_DATA_MAP
}


/////////////////////////////////////////////////////////////////////////////
// CScanCommGeneration message handlers


//********************************Method Header********************************
//Method Name   :PreTranslateMessage()
//Author        :PATNI/AJS
//Purpose       :
//*****************************************************************************
BOOL CScanCommGeneration::PreTranslateMessageImpl(
    MSG* pMsg
)
{

    if (pMsg && pMsg->message == WM_RBUTTONDOWN) {
        return TRUE;
    }

    return CZOrderDialog::PreTranslateMessageImpl(pMsg);
}


//********************************Method Header********************************
//Method Name   :OnOK()
//Author        :PATNI/AJS
//Purpose       :
//*****************************************************************************
void CScanCommGeneration::OnOK()
{
    // TODO: Add extra validation here
    /*CDialog*/ //DialogEx::OnOK();

    LPCTSTR FUNC_NAME = _T("CScanCommGeneration::OnOK");
    PQM_TRACE_SCOPED(FUNC_NAME);

    CString l_comm_data;
    GetDlgItemText(IDC_COMM_VALUE, l_comm_data);

    if (m_parent_wnd) {
        m_parent_wnd->UpdateAndScanComment(l_comm_data);
    }
}


//********************************Method Header********************************
//Method Name   :OnInitDialogImpl()
//Author        :PATNI/AJS
//Purpose       :
//*****************************************************************************
BOOL CScanCommGeneration::OnInitDialogImpl()
{
    LPCTSTR FUNC_NAME = _T("CScanCommGeneration::OnInitDialogImpl");
    PQM_TRACE_SCOPED(FUNC_NAME);



    //Moved above SetDialogRibbonHeight function...
    //Changed Window to Client Rect...
    //+For Resizing

    //+Patni-AJS/2010Oct01/Added/IR-163_Part2
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
    //+For Resizing
    //
    //-Patni-AJS/2010Oct01/Added/IR-163_Part2

    // TODO: Add extra initialization here
    SetDialogRibbonHeight();

    SetButtonAppearence();

    SetMultilingual();
    LoadScanCommentData();

    //For Horz Scrollbar
    InitHorzBar();



    return CZOrderDialog::OnInitDialogImpl(false);  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


//********************************Method Header********************************
//Method Name   :OnSelchangeListSeq()
//Author        :PATNI/AJS
//Purpose       :
//*****************************************************************************
void CScanCommGeneration::OnSelchangeListSeq()
{
    // TODO: Add your control notification handler code here
    UpdateScanCommData();
}


//********************************Method Header********************************
//Method Name   :OnSelchangeListCont()
//Author        :PATNI/AJS
//Purpose       :
//*****************************************************************************
void CScanCommGeneration::OnSelchangeListCont()
{
    // TODO: Add your control notification handler code here
    UpdateScanCommData();
}


//********************************Method Header********************************
//Method Name   :OnSelchangeListOpt()
//Author        :PATNI/AJS
//Purpose       :
//*****************************************************************************
void CScanCommGeneration::OnSelchangeListOpt()
{
    // TODO: Add your control notification handler code here
    UpdateScanCommData();
}

//********************************Method Header********************************
//Method Name   :OnDestroyImpl()
//Author        :PATNI/AJS
//Purpose       :
//*****************************************************************************
void CScanCommGeneration::OnDestroyImpl()
{
    LPCTSTR FUNC_NAME = _T("CScanCommGeneration::OnDestroyImpl");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //Patni-AJS/2010Oct01/Added/IR-163_Part2
    GetWindowRect(&m_orgwindowrect) ;

    CZOrderDialog::OnDestroyImpl();

}


//********************************Method Header********************************
//Method Name   :OnGetMinMaxInfo()
//Author        :PATNI/AJS
//Purpose       :
//*****************************************************************************
void CScanCommGeneration::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
    // TODO: Add your message handler code here and/or call default

    //+Patni-AJS/2010Oct01/Added/IR-163_Part2
    if (!m_org_client_rect.IsRectEmpty()) {
        lpMMI->ptMinTrackSize.x = m_org_client_rect.Width() ;
        lpMMI->ptMinTrackSize.y = m_org_client_rect.Height() ;
    }

    //-Patni-AJS/2010Oct01/Added/IR-163_Part2

    CZOrderDialog::OnGetMinMaxInfo(lpMMI);
}


//********************************Method Header********************************
//Method Name   :OnSize()
//Author        :PATNI/AJS
//Purpose       :
//*****************************************************************************
void CScanCommGeneration::OnSize(UINT nType, int cx, int cy)
{
    CZOrderDialog::OnSize(nType, cx, cy);
    Resize(cx, cy);
}

//********************************Method Header********************************
//Method Name   :OnCancel()
//Author        :PATNI/AJS
//Purpose       :
//*****************************************************************************
void CScanCommGeneration::OnCancel()
{
    LPCTSTR FUNC_NAME = _T("CScanCommGeneration::OnCancel");
    PQM_TRACE_SCOPED(FUNC_NAME);

    // TODO: Add extra cleanup here
    if (this->GetSafeHwnd()) {

        DestroyWindow();
    }

    /*CDialog*/ //DialogEx::OnCancel();
}
//********************************Method Header********************************
//Method Name   :InitializeListBox()
//Author        :PATNI/AJS
//Purpose       :
//*****************************************************************************
void CScanCommGeneration::InitializeListBox(ListBoxEx* f_listbox, CList<CString, CString>* f_seq_data)
{

    //TA Review Comment_7 - NULL check for f_seq_data included
    if (!f_listbox || !f_seq_data) {
        return ;
    }

    f_listbox->ResetContent();

    POSITION pos = f_seq_data->GetHeadPosition();

    while (pos != NULL) {
        f_listbox->AddString(f_seq_data->GetNext(pos));
    }
}


//********************************Method Header********************************
//Method Name   :LoadScanCommentData()
//Author        :PATNI/AJS
//Purpose       :
//*****************************************************************************
void CScanCommGeneration::LoadScanCommentData()
{
    m_seq_list.SetExStyle(LIST_RADIO);
    InitializeListBox(&m_seq_list, &(m_scan_dialog_data->m_seq_comment_list));

    m_cont_list.SetExStyle(LIST_RADIO);
    InitializeListBox(&m_cont_list, &(m_scan_dialog_data->m_cont_comment_list));

    m_opt_list.SetExStyle(LIST_CHECK);
    InitializeListBox(&m_opt_list, &(m_scan_dialog_data->m_opt_comment_list));

}


//********************************Method Header********************************
//Method Name   :CheckListItem()
//Author        :PATNI/AJS
//Purpose       :
//*****************************************************************************
void CScanCommGeneration::CheckListItem(const CString& f_str)
{
    if (CheckListItem(&m_seq_list, f_str)) {
        return ;
    }

    if (CheckListItem(&m_cont_list, f_str)) {
        return ;
    }

    if (CheckListItem(&m_opt_list, f_str)) {
        return ;
    }
}


//********************************Method Header********************************
//Method Name   :CheckListItem()
//Author        :PATNI/AJS
//Purpose       :
//*****************************************************************************
bool CScanCommGeneration::CheckListItem(ListBoxEx* f_list , const CString& f_str)const
{
    //TA Review Comment_8 - l_str come out of the loop
    CString l_str;

    for (int i = 0 ; i < f_list->GetCount() ; i++) {
        f_list->GetText(i, l_str);

        if (f_str.Compare(l_str) == 0) {
            f_list->SetCheck(i, true);
            return true ;
        }
    }

    return false;
}


//********************************Method Header********************************
//Method Name   :UnCheckListItems()
//Author        :PATNI/AJS
//Purpose       :
//*****************************************************************************
void CScanCommGeneration::UnCheckListItems()
{

    UnCheckListItems(&m_seq_list);
    UnCheckListItems(&m_cont_list);
    UnCheckListItems(&m_opt_list);
}


//********************************Method Header********************************
//Method Name   :UnCheckListItems()
//Author        :PATNI/AJS
//Purpose       :
//*****************************************************************************
void CScanCommGeneration::UnCheckListItems(
    ListBoxEx* f_list
)
{
    for (int i = 0 ; i < f_list->GetCount() ; i++) {
        f_list->SetCheck(i, false);
    }
}


//********************************Method Header********************************
//Method Name   :UpdateScanCommData()
//Author        :PATNI/AJS
//Purpose       :
//*****************************************************************************
void CScanCommGeneration::UpdateScanCommData(
)
{

    int l_max_list_items = m_seq_list.GetCount();

    if (l_max_list_items < m_cont_list.GetCount()) {
        l_max_list_items = m_cont_list.GetCount();
    }

    if (l_max_list_items < m_opt_list.GetCount()) {
        l_max_list_items = m_opt_list.GetCount();
    }

    const int l_max_list = 3;

    ListBoxEx* f_list[l_max_list] = {&m_seq_list, &m_cont_list, &m_opt_list};

    int i = 0;

    CString l_scan_comment = _T("");

    UINT* l_checked_items = new UINT [l_max_list_items] ;

    for (int l_list_count = 0; l_list_count < l_max_list ; l_list_count++) {

        for (i = 0; i < l_max_list_items; i++) {
            l_checked_items[i] = 99999;
        }

        f_list[l_list_count]->GetExSelItems(l_checked_items, l_max_list_items);
        UpdateScanCommData(f_list[l_list_count], l_checked_items, l_max_list_items, &l_scan_comment);
    }

    DEL_PTR_ARY(l_checked_items)

    m_comm_val.SetWindowText(l_scan_comment);

}


//********************************Method Header********************************
//Method Name   :UpdateScanCommData()
//Author        :PATNI/AJS
//Purpose       :
//*****************************************************************************
void CScanCommGeneration::UpdateScanCommData(
    const ListBoxEx* f_list,
    const UINT* f_checked_items,
    const int f_max_items,
    CString* f_str
)
{

    if (!f_list || !f_str || !f_checked_items) {
        return ;
    }

    CString l_str_to_return = _T("");
    CString l_list_item = _T("");

    int j = 0;

    for (int l_index = 0 ; (l_index < f_list->GetCount()) && (l_index < f_max_items) ; l_index++) {

        for (j = 0; (j < f_list->GetCount()) && (j < f_max_items) ; j++) {

            if (l_index == (int)f_checked_items[j]) {

                if (!l_str_to_return.IsEmpty()) {
                    l_str_to_return += _T(",");
                }


                f_list->GetText(f_checked_items[j], l_list_item);

                l_str_to_return += l_list_item;
                break ;
            }

        }
    }

    if (l_str_to_return.IsEmpty()) {
        return ;
    }

    if (!(*f_str).IsEmpty() && !l_str_to_return.IsEmpty()) {
        (*f_str) += _T(",");
    }

    (*f_str) += l_str_to_return;
}


//********************************Method Header********************************
//Method Name   :Resize()
//Author        :PATNI/AJS
//Purpose       :
//*****************************************************************************
void CScanCommGeneration::Resize(
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
    CRect clChildWndRect ;

    int niIncrementX = 0, niIncrementY = 0;

    if (clpWnd = GetDlgItem(IDCANCEL)) {
        clpWnd->GetWindowRect(clChildWndRect) ;
        ScreenToClient(clChildWndRect) ;

        niIncrementY = clWindowRect.bottom - clChildWndRect.bottom - niGapX ;
        niIncrementX = clWindowRect.right - clChildWndRect.right - niGapX;
    }

    OffsetChild(IDOK, CSize(0, niIncrementY)) ;
    OffsetChild(IDCANCEL, CSize(niIncrementX, niIncrementY)) ;

    IncrementSizeChild(IDC_COMM_VALUE, CSize(niIncrementX , 0)) ;
    OffsetChild(IDC_COMM_VALUE, CSize(0, niIncrementY)) ;

    const int l_total_listboxes = 3;
    const int l_num_of_gaps = 2;

    //+Patni-AJS/2010Oct01/Added/IR-163_Part2

    const int l_factor = niIncrementX % l_total_listboxes ;

    niIncrementX = niIncrementX / l_total_listboxes;

    //For Sequence...
    //
    IncrementSizeChild(IDC_LIST_SEQ, CSize(niIncrementX , niIncrementY)) ;
    IncrementSizeChild(IDC_LIST_SEQ_STATIC, CSize(niIncrementX , 0)) ;

    //For Contrast...
    //
    IncrementSizeChild(IDC_LIST_CONT, CSize(niIncrementX , niIncrementY)) ;
    OffsetChild(IDC_LIST_CONT, CSize(niIncrementX, 0)) ;

    IncrementSizeChild(IDC_LIST_CONT_STATIC, CSize(niIncrementX , 0)) ;
    OffsetChild(IDC_LIST_CONT_STATIC, CSize(niIncrementX, 0)) ;


    //For Option...
    //
    IncrementSizeChild(IDC_LIST_OPT, CSize((niIncrementX + l_factor) , niIncrementY)) ;
    OffsetChild(IDC_LIST_OPT, CSize(niIncrementX * l_num_of_gaps, 0)) ;

    IncrementSizeChild(IDC_LIST_OPT_STATIC, CSize((niIncrementX + l_factor), 0)) ;
    OffsetChild(IDC_LIST_OPT_STATIC, CSize(niIncrementX * l_num_of_gaps, 0)) ;

    //-Patni-AJS/2010Oct01/Added/IR-163_Part2

    Invalidate(FALSE) ;
}


//********************************Method Header********************************
//Method Name   :OffsetChild()
//Author        :PATNI/AJS
//Purpose       :
//*****************************************************************************
void CScanCommGeneration::OffsetChild(
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
//Author        :PATNI/AJS
//Purpose       :
//*****************************************************************************
void CScanCommGeneration::IncrementSizeChild(
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


//********************************Method Header********************************
//Method Name   :InitHorzBar()
//Author        :PATNI/AJS
//Purpose       :
//*****************************************************************************
void CScanCommGeneration::InitHorzBar()
{
    SetHorzBar(&m_seq_list, &m_scan_dialog_data->m_seq_comment_list);
    SetHorzBar(&m_cont_list, &m_scan_dialog_data->m_cont_comment_list);
    SetHorzBar(&m_opt_list, &m_scan_dialog_data->m_opt_comment_list);
}


//********************************Method Header********************************
//Method Name   :SetHorzBar()
//Author        :PATNI/AJS
//Purpose       :
//*****************************************************************************
void CScanCommGeneration::SetHorzBar(
    ListBoxEx* f_listbox,
    CList<CString, CString>* f_list
)
{

    if (!f_listbox || !f_list) {
        return ;
    }

    CDC* l_listdc = f_listbox->GetDC();

    if (!l_listdc) {
        return;
    }

    CFont* l_list_font = f_listbox->GetFont();

    if (!l_list_font) {

        f_listbox->ReleaseDC(l_listdc);
        return;
    }


    CString l_max_str = _T(""), l_str = _T("");
    POSITION pos = f_list->GetHeadPosition();

    while (pos != NULL) {
        l_str = f_list->GetNext(pos);

        if (l_max_str.GetLength() < l_str.GetLength()) {
            l_max_str = l_str;
        }
    }


    CFont* l_dc_old_font = l_listdc->SelectObject(l_list_font);

    TEXTMETRIC tm;
    l_listdc->GetTextMetrics(&tm);
    CSize l_text_size = l_listdc->GetTextExtent(l_max_str, l_max_str.GetLength());

    const int l_corner_offset = 7 ;
    l_text_size.cx += (tm.tmAveCharWidth + f_listbox->GetRadio()->GetWidth() + l_corner_offset) ;

    f_listbox->SetHorizontalExtent(l_text_size.cx);

    l_listdc->SelectObject(l_dc_old_font);
    f_listbox->ReleaseDC(l_listdc);
}


//********************************Method Header********************************
//Method Name   :SetDialogRibbonHeight()
//Author        :PATNI/AJS
//Purpose       :
//*****************************************************************************
void CScanCommGeneration::SetDialogRibbonHeight(
)
{
    const int RIBBON_GAP = 5;

    CRect client_rect;
    GetClientRect(&client_rect);

    CRect ok_button_rect ;
    m_cancel_button.GetWindowRect(&ok_button_rect);
    ScreenToClient(&ok_button_rect);


    SetRibbonHeight(client_rect.bottom - ok_button_rect.top + RIBBON_GAP);
}


//********************************Method Header********************************
//Method Name   :SetMultilingual()
//Author        :PATNI/AJS
//Purpose       :
//*****************************************************************************
void CScanCommGeneration::SetMultilingual()
{
    CString l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_BUTTON_APPLY"));
    HWND l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDOK);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;

    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_COMMENTDLG_CANCEL"));
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDCANCEL);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;

    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDC_LIST_SEQ_STATIC"));
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDC_LIST_SEQ_STATIC);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;

    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDC_LIST_CONT_STATIC"));
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDC_LIST_CONT_STATIC);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;

    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDC_LIST_OPT_STATIC"));
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDC_LIST_OPT_STATIC);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }


    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDD_SCAN_COMMENT_XML_DIALOG"));

    if (_tcscmp(l_chOutput , _T("")) != 0) {

        SetWindowText(l_chOutput);
    }
}


//********************************Method Header********************************
//Method Name   :SetButtonAppearence()
//Author        :PATNI/AJS
//Purpose       :
//*****************************************************************************
void CScanCommGeneration::SetButtonAppearence(
)
{
    LPCTSTR button_font_name = _T("Verdana BOLD") ;
    const int size = 9;

    m_comm_val.SetFont(button_font_name, size);

    m_ok_button.SetPrimaryTextFont(button_font_name, size);
    m_cancel_button.SetPrimaryTextFont(button_font_name, size);

    m_ok_button.SetCurveType(CURVE_LARGE_5);
    m_cancel_button.SetCurveType(CURVE_LARGE_5);


    WINDOW_COLORS_t stColors_o;
    GetWindowColors(stColors_o);
    COLORREF bk_color = stColors_o.crClientColor;

    m_opt_static.LoadDeafultStaticCtrlColorsWithBKColor(bk_color);
    m_cont_static.LoadDeafultStaticCtrlColorsWithBKColor(bk_color);
    m_seq_static.LoadDeafultStaticCtrlColorsWithBKColor(bk_color);

    LPCTSTR l_list_font = _T("Arial");
    const int l_listtext_size = 12;
    m_opt_list.SetTextColor(TEXTCOLOR);
    m_opt_list.SetFont(l_list_font, l_listtext_size);

    m_cont_list.SetTextColor(TEXTCOLOR);
    m_cont_list.SetFont(l_list_font, l_listtext_size);

    m_seq_list.SetTextColor(TEXTCOLOR);
    m_seq_list.SetFont(l_list_font, l_listtext_size);

}

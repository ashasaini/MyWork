// ImageListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "epi.h"
#include "ImageListDlg.h"
#include "EPIController.h"
#include "CommonDefinations.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageListDlg dialog


CImageListDlg::CImageListDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CImageListDlg::IDD, pParent), m_DialogTitle(_T("List")), m_csCurrentSelectedPath(_T(""))
{
    //{{AFX_DATA_INIT(CImageListDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CImageListDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CImageListDlg)
    DDX_Control(pDX, IDC_LIST_IMAGE, m_ImageList);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImageListDlg, CDialog)
    //{{AFX_MSG_MAP(CImageListDlg)
    ON_NOTIFY(NM_CLICK, IDC_LIST_IMAGE, OnClickListImage)
    ON_WM_CLOSE()
    ON_MESSAGE(WM_SET_ITEMS, PopulateComboList)
    ON_MESSAGE(WM_UPDATE_IMAGE,UpdateImager)
    //}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageListDlg message handlers

BOOL CImageListDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    SetWindowText(m_DialogTitle);
    m_ImageList.InsertColumn(0, "Name", LVCFMT_LEFT, 200);
    m_ImageList.InsertColumn(1, "Form", LVCFMT_LEFT, 150);
    m_ImageList.InsertColumn(2, "Tag", LVCFMT_LEFT, 50);
    m_ImageList.SetReadOnlyColumns(0);
    m_ImageList.SetComboColumns(1);
    RECT lp;
    m_ImageList.GetClientRect(&lp);
    int width = 300;
    width = abs(lp.right - lp.left - 20);
    m_ImageList.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    RefreshList();
    // TODO: Add extra initialization here
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CImageListDlg::SetListPaths(CStringArray &f_CsArray)
{
    m_CsArray.RemoveAll();
    m_CsArray.Append(f_CsArray);
}

void CImageListDlg::RefreshList()
{
    m_ImageList.DeleteAllItems();
    LVITEM lv00;

    for(int i = 0; i < m_CsArray.GetSize(); i++) {
        CString l_cstmp = m_CsArray.GetAt(i);
        int cnt = l_cstmp.ReverseFind('\\');
        cnt = l_cstmp.GetLength() - (cnt + 1);
        CString csright = l_cstmp.Right(cnt);
        lv00.iItem = i;
        lv00.iSubItem = 0;
        lv00.pszText = csright.GetBuffer(csright.GetLength());
        lv00.mask = LVIF_TEXT;
        m_ImageList.InsertItem(&lv00);
        csright.ReleaseBuffer();
        CString l_form(sMAGNITUDE);
        lv00.iSubItem = 1;
        lv00.pszText = l_form.GetBuffer(l_form.GetLength());
        m_ImageList.SetItem(&lv00);
        l_form.ReleaseBuffer();
    }
}

void CImageListDlg::SetDialogTitle(CString &f_cs)
{
    m_DialogTitle = f_cs;
    SetWindowText(m_DialogTitle);
}

void CImageListDlg::OnClickListImage(NMHDR* pNMHDR, LRESULT* pResult)
{
    try {
        // TODO: Add your control notification handler code here
        UNREFERENCED_PARAMETER(pNMHDR);
        UpdateImage_selected_row();
        *pResult = 0;
    } catch(...) {
        CEPILogHelper::WriteExceptionToLog(__LINE__, __FILE__);
    }
}

CImageListDlg::~CImageListDlg()
{
    m_CsArray.RemoveAll();
}

LRESULT CImageListDlg::PopulateComboList(WPARAM wParam, LPARAM lParam)
{
    // Get the Combobox window pointer
    CComboBox* pInPlaceCombo = static_cast<CComboBox*>(GetFocus());
    // Get the inplace combbox top left
    CRect obWindowRect;
    pInPlaceCombo->GetWindowRect(&obWindowRect);
    CPoint obInPlaceComboTopLeft(obWindowRect.TopLeft());
    // Get the active list
    // Get the control window rect
    // If the inplace combobox top left is in the rect then
    // The control is the active control
    m_ImageList.GetWindowRect(&obWindowRect);
    int iColIndex = wParam;
    CStringList* pComboList = reinterpret_cast<CStringList*>(lParam);
    pComboList->RemoveAll();

    if(obWindowRect.PtInRect(obInPlaceComboTopLeft)) {
        if(1 == iColIndex) {
            pComboList->AddTail(sMAGNITUDE);
            pComboList->AddTail(sPHASE);
            pComboList->AddTail(sREAL);
            pComboList->AddTail(sIMAGINARY);
        }
    }

    return 1;
}

void CImageListDlg::OnClose()
{
    // TODO: Add your message handler code here and/or call default
    ShowWindow(SW_HIDE);
    //CDialog::OnClose();
}

LRESULT CImageListDlg::UpdateImager( WPARAM wParam, LPARAM lParam )
{
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);
    UpdateImage_selected_row();
	return 1;
}

void CImageListDlg::UpdateImage_selected_row()
{
    int row = m_ImageList.GetSelectionMark();
    CString csfilename = m_ImageList.GetItemText(row, 0);
    m_csCurrentSelectedPath = m_CsArray.GetAt(row);
    int cnt = m_csCurrentSelectedPath.ReverseFind('\\');
    CString csleft = m_csCurrentSelectedPath.Left(cnt);
    SetDialogTitle(csleft);
    CString csForm = m_ImageList.GetItemText(row, 1);
    
    if(csForm.IsEmpty()) {
        return;
    }
    
    int DataForm = 0;//Magnitude
    
    if(csForm.CollateNoCase(sMAGNITUDE) == 0) {
        DataForm = MAGNITUDE;
    }
    
    if(csForm.CollateNoCase(sPHASE) == 0) {
        DataForm = PHASE;
    }
    
    if(csForm.CollateNoCase(sREAL) == 0) {
        DataForm = REAL;
    }
    
    if(csForm.CollateNoCase(sIMAGINARY) == 0) {
        DataForm = IMAGINARY;
    }
    
     CEPIController::GetInstance()->readFileDisplayImage(m_csCurrentSelectedPath,
                                                         DataForm,
                                                         FOLDER_PATH);
}

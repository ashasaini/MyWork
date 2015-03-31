//REDMINE_782_CPP_Test_Tool
//TMSC-REDMINE-783
//TMSC-REDMINE-782
//TMSC-REDMINE-781
//TMSC-REDMINE-780
//TMSC-REDMINE-778
//TMSC-REDMINE-777
//TMSC-REDMINE-779
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMSetScanOffsetDlg.cpp
 *  Overview: Implementation of CPQMSetScanOffsetDlg class.
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
#include "PQMSetScanOffsetDlg.h"
#include <libStudyManager/UI/ZOrderLayerFactory.h>
#include <PQM/resource.h>
#include "PQMUtility.h"
#include "DPSManager.h"
#include "PQMImageUtility.h"
#include "UIButtonUtility.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using MR::ACQ::SM::COMMON::UI::CZOrderDialog;
using MR::ACQ::SM::COMMON::UI::CZOrderLayerFactory;


#define RIBBON_GAP 10
const COLORREF g_image_bk_color = RGB(19, 25, 35);


BEGIN_MESSAGE_MAP(CPQMSetScanOffsetDlg, CZOrderDialog)
    //{{AFX_MSG_MAP(CPQMSetScanOffsetDlg)
    ON_BN_CLICKED(IDC_BUTTON_DECR_Z, OnMoveCouchLeft)
    ON_BN_CLICKED(IDC_BUTTON_INCR_Z, OnMoveCouchRightButton)
    ON_WM_PAINT()
    ON_MESSAGE(WM_UPDATE_COUCH_OFFSET, OnUpdateCouchOffset)
    ON_EN_KILLFOCUS(IDC_EDIT_SCAN_OFFSET_X, OnKillfocusEditScanOffsetX)
    ON_EN_KILLFOCUS(IDC_EDIT_SCAN_OFFSET_Y, OnKillfocusEditScanOffsetY)
    ON_EN_KILLFOCUS(IDC_EDIT_SCAN_OFFSET_Z, OnKillfocusEditScanOffsetZ)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


//************************************Method Header**************************************
//Method Name:  CPQMSetScanOffsetDlg
//Author     :  PATNI/HEMANT
//Purpose    :
//**************************************************************************************/
CPQMSetScanOffsetDlg::CPQMSetScanOffsetDlg(
    const COUCH_POSITION f_couch_pos,
    Vector_t f_current_scan_offset,
    Vector_t f_min_value,
    Vector_t f_max_value,
    PatientInsertDirection f_patient_in_or_out,
    PatientRotationDirection f_patient_side,
    CWnd* p_parent /*=NULL*/
)   : CZOrderDialog(CZOrderLayerFactory::GetIselLayer(), CDPSManager::GetInstance(),
                        IDD_SCANOFFSET_DIALOG, p_parent),
    m_couch_pos(f_couch_pos),
    m_move_couchwnd(f_couch_pos, f_patient_in_or_out, f_patient_side, f_min_value.z, f_max_value.z)
{
    m_scan_offset.SetScanOffsetData(
        ScanOffset(f_current_scan_offset.x, f_current_scan_offset.y, f_current_scan_offset.z),
        ScanOffset(f_min_value.x, f_min_value.y, f_min_value.z),
        ScanOffset(f_max_value.x, f_max_value.y, f_max_value.z)
    );
}



//************************************Method Header************************************
// Method Name  : ~CPQMSetScanOffsetDlg
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CPQMSetScanOffsetDlg::~CPQMSetScanOffsetDlg(
)
{

}

const Vector_t CPQMSetScanOffsetDlg::GetScanOffsetVector(
)
{
    const ScanOffset l_scanoffset =  m_scan_offset.GetOffset();
    Vector_t temp;
    temp.x = l_scanoffset[ScanOffsetX] ;
    temp.y = l_scanoffset[ScanOffsetY] ;
    temp.z = l_scanoffset[ScanOffsetZ] ;
    return temp;
}

//************************************Method Header**************************************
//Method Name:  DoDataExchange
//Author     :  PATNI/HEMANT
//Purpose    : To exchange and validate dialog data for CPQMGateInfoDlg
//**************************************************************************************/
void CPQMSetScanOffsetDlg::DoDataExchange(
    CDataExchange* pDX
)
{

    CZOrderDialog::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_MOVE_COUCH_STATIC, m_move_couch_static);
    DDX_Control(pDX, IDC_MOVE_COUCH_RANGE_STATIC, m_move_couch_range_static);
    DDX_Control(pDX, IDC_IMAGE_OFFSET_STATIC, m_image_offset_static);

    DDX_Control(pDX, IDOK, m_ok);
    DDX_Control(pDX, IDCANCEL, m_cancel);
    DDX_Control(pDX, IDC_EDIT_SCAN_OFFSET_X, m_lr_edit);
    DDX_Control(pDX, IDC_EDIT_SCAN_OFFSET_Y, m_ap_edit);
    DDX_Control(pDX, IDC_EDIT_SCAN_OFFSET_Z, m_move_couch_edit);
    DDX_Control(pDX, IDC_BUTTON_DECR_Z, m_move_couch_left_button);
    DDX_Control(pDX, IDC_BUTTON_INCR_Z, m_move_couch_right_button);

}



//************************************Method Header************************************
// Method Name  : OnPaint
// Author       : PATNI/ HEMANT
// Purpose      : Dialog paint function
//***********************************************************************************
void CPQMSetScanOffsetDlg::OnPaint(
)
{
    CPaintDC dc(this); // device context for painting
    PaintStaticImages(&dc);
    // Do not call /*CDialog*/DialogEx::OnPaint() for painting messages
}



/*************************************************************************************
//Module:  OnInitDialogImpl
//Author:  PATNI/Hemant
//Purpose: Initialize controls in dialog box.
//**************************************************************************************/
BOOL CPQMSetScanOffsetDlg::OnInitDialogImpl(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMSetScanOffsetDlg::OnInitDialogImpl");
    PQM_TRACE_SCOPED(FUNC_NAME);


    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Min Offset Range->") + m_scan_offset.GetMinOffset().GetLogString());
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Max Offset Range->") + m_scan_offset.GetMaxOffset().GetLogString());
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Scan Offset ->") + m_scan_offset.GetOffset().GetLogString());

    SetMultilingual();
    InitializeDlg();

    return CZOrderDialog::OnInitDialogImpl();
}

/*************************************************************************************
//Module:  OnOK
//Author:  PATNI/HEMANT
//Purpose: Set scan offset value in protocol
//**************************************************************************************/
void CPQMSetScanOffsetDlg::OnOK(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMSetScanOffsetDlg::OnOK");
    PQM_TRACE_SCOPED(FUNC_NAME);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Scan Offset ->") + m_scan_offset.GetOffset().GetLogString());

    if (m_nFlags & (WF_MODALLOOP | WF_CONTINUEMODAL)) {
        EndModalLoop(IDOK);
    }
}

/*************************************************************************************
//Module:  OnCancel
//Author:  PATNI/HEMANT
//Purpose: Closes dialog box.
//**************************************************************************************/
void CPQMSetScanOffsetDlg::OnCancel(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMSetScanOffsetDlg::OnCancel");
    PQM_TRACE_SCOPED(FUNC_NAME);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Scan Offset -> Unchanged"));

    CPQMLogMgr::GetInstance()->WriteOpeLog(OP_IDB_SCAN_OFFSET_CANCEL);

    if (m_nFlags & (WF_MODALLOOP | WF_CONTINUEMODAL)) {
        EndModalLoop(IDCANCEL);
    }
}



/*************************************************************************************
//Module:  PreTranslateMessageImpl
//Author:  PATNI/HEMANT
//Purpose:
//**************************************************************************************/
BOOL CPQMSetScanOffsetDlg::PreTranslateMessageImpl(MSG* pMsg)
{
    if (ProcessEditBoxMsg(pMsg)) {
        return TRUE;
    }

    return CZOrderDialog::PreTranslateMessageImpl(pMsg);
}


/*************************************************************************************
//Module:  SetMultilingual
//Author:  PATNI/HEMANT
//Purpose:
//**************************************************************************************/
void CPQMSetScanOffsetDlg::SetMultilingual()
{
    //dialog title
    CString l_chTitle =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_SET_SCAN_OFFSET_DLG_TITLE"));

    if (_tcscmp(l_chTitle , _T("")) != 0) {
        CWnd ::SetWindowText(l_chTitle);
    }

    CString l_chOutput;
    HWND l_buttonUserHwnd;

#define MULTI_LINGUAL(STR_ID,BTN_ID)	l_chOutput =  CPQMUtility::GetMultiLingualString(STR_ID);	\
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, BTN_ID);		\
    if (_tcscmp(l_chOutput , _T("")) != 0) {				\
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);		\
    }

    MULTI_LINGUAL(_T("IDS_PQM_BUTTON_SET"), IDOK)
    MULTI_LINGUAL(_T("IDS_PQM_CANCEL_BUTTON"), IDCANCEL)
    MULTI_LINGUAL(_T("IDS_PQM_CANCEL_BUTTON"), IDCANCEL)
    MULTI_LINGUAL(_T("IDS_IMAGE_OFFSET_STATIC"), IDC_IMAGE_OFFSET_STATIC)
    MULTI_LINGUAL(_T("IDS_MOVE_COUCH_STATIC"), IDC_MOVE_COUCH_STATIC)

#undef MULTI_LINGUAL
}


/*************************************************************************************
//Module:  ProcessEditBoxMsg
//Author:  PATNI/HEMANT
//Purpose:
//**************************************************************************************/
bool CPQMSetScanOffsetDlg::ProcessEditBoxMsg(MSG* pMsg)
{

    if (NULL == pMsg) {
        return false;
    }

    switch (pMsg->message) {

        case WM_MOUSEWHEEL: {
            return ProcessMouseWheenMsg(pMsg);
        }
        break;

        case WM_KEYDOWN : {
            return ProcessKeyDownMsg(pMsg);
        }
        break;
    }

    return false;

}


/*************************************************************************************
//Module:  ProcessMouseWheenMsg
//Author:  PATNI/HEMANT
//Purpose:
//**************************************************************************************/
bool CPQMSetScanOffsetDlg::ProcessMouseWheenMsg(
    MSG* pMsg
)
{
    if (!pMsg) {
        return false;
    }

    const ScanOffsetEnum l_scanoffset_edit = GetScanOffsetEnumForWnd(pMsg->hwnd);

    if (ScanOffset_Invalid == l_scanoffset_edit) {
        return false;
    }

    const float l_increment = ((short) HIWORD(pMsg->wParam) < 0) ? -1.f : 1.f;

    IncrementDecrementValue(l_scanoffset_edit, l_increment);

    if (ScanOffsetZ == l_scanoffset_edit) {
        UpdateCouchOffset();

    }

    return false;
}


/*************************************************************************************
//Module:  ProcessKeyDownMsg
//Author:  PATNI/HEMANT
//Purpose:
//**************************************************************************************/
bool CPQMSetScanOffsetDlg::ProcessKeyDownMsg(
    MSG* pMsg
)
{

    switch (pMsg->wParam) {

        case VK_RETURN :
        case VK_UP :
        case VK_DOWN :
            break;

        default :
            return false;
    }

    const ScanOffsetEnum l_scanoffset_edit = GetScanOffsetEnumForWnd(pMsg->hwnd);

    if (ScanOffset_Invalid == l_scanoffset_edit) {
        return false;
    }

    switch (pMsg->wParam) {

        case VK_RETURN : {
            m_scan_offset.UpdateValue(l_scanoffset_edit, GetEditText(l_scanoffset_edit));
            UpdateEditBox(l_scanoffset_edit);
        }
        break;

        case VK_UP : {
            IncrementDecrementValue(l_scanoffset_edit, 0.1f);
        }
        break;

        case VK_DOWN : {
            IncrementDecrementValue(l_scanoffset_edit, -0.1f);
        }
        break;
    }

    if (ScanOffsetZ == l_scanoffset_edit) {
        UpdateCouchOffset();

    }

    return true;
}


/*************************************************************************************
//Module:  GetScanOffsetEnumForWnd
//Author:  PATNI/HEMANT
//Purpose:
//**************************************************************************************/
const ScanOffsetEnum CPQMSetScanOffsetDlg::GetScanOffsetEnumForWnd(
    const HWND& f_hwnd
) const
{

    if (m_lr_edit.m_hWnd == f_hwnd) {
        return ScanOffsetX;

    } else if (m_ap_edit.m_hWnd == f_hwnd) {
        return ScanOffsetY;

    } else if (m_move_couch_edit.m_hWnd == f_hwnd) {
        return ScanOffsetZ;
    }

    return ScanOffset_Invalid;
}


/*************************************************************************************
//Module:  IncrementDecrementValue
//Author:  PATNI/HEMANT
//Purpose:
//**************************************************************************************/
void CPQMSetScanOffsetDlg::IncrementDecrementValue(
    const ScanOffsetEnum f_scanoffsetenum,
    const float f_increment /*= 1.f*/
)
{
    m_scan_offset.Increment(f_scanoffsetenum, f_increment);
    UpdateEditBox(f_scanoffsetenum);
}


/*************************************************************************************
//Module:  UpdateEditBoxAll
//Author:  PATNI/HEMANT
//Purpose:
//**************************************************************************************/
void CPQMSetScanOffsetDlg::UpdateEditBoxAll(
)
{

    UpdateEditBox(ScanOffsetX);
    UpdateEditBox(ScanOffsetY);
    UpdateEditBox(ScanOffsetZ);
}


/*************************************************************************************
//Module:  UpdateEditBox
//Author:  PATNI/HEMANT
//Purpose:
//**************************************************************************************/
void CPQMSetScanOffsetDlg::UpdateEditBox(
    const ScanOffsetEnum f_scanoffsetenum
)
{
    CWnd* l_editbox = GetEditBox(f_scanoffsetenum);

    if (l_editbox) {

        l_editbox->SetWindowText(m_scan_offset.GetString(f_scanoffsetenum));
    }
}

/*************************************************************************************
//Module:  GetEditBox
//Author:  PATNI/HEMANT
//Purpose:
//**************************************************************************************/
CWnd* CPQMSetScanOffsetDlg::GetEditBox(
    const ScanOffsetEnum f_scanoffsetenum
)
{

    switch (f_scanoffsetenum) {

        case ScanOffsetX:
            return (&m_lr_edit);

        case ScanOffsetY:
            return (&m_ap_edit);

        case ScanOffsetZ:
            return (&m_move_couch_edit);
    }

    return NULL;
}


/*************************************************************************************
//Module:  GetEditText
//Author:  PATNI/HEMANT
//Purpose:
//**************************************************************************************/
CString CPQMSetScanOffsetDlg::GetEditText(
    const ScanOffsetEnum f_scanoffsetenum
)
{

    CString l_str = _T("");
    CWnd* l_wnd = NULL ;

    if (l_wnd = GetEditBox(f_scanoffsetenum)) {
        l_wnd->GetWindowText(l_str);
    }


    return l_str;
}


/*************************************************************************************
//Module:  OnMoveCouchLeft
//Author:  PATNI/HEMANT
//Purpose:
//**************************************************************************************/
void CPQMSetScanOffsetDlg::OnMoveCouchLeft()
{
    // TODO: Add your control notification handler code here

    IncrementDecrementValue(ScanOffsetZ, (m_couch_pos == COUCH_POSITION_LEFT) ? -1.f : 1.f);
    UpdateCouchOffset();

}


/*************************************************************************************
//Module:  OnMoveCouchRightButton
//Author:  PATNI/HEMANT
//Purpose:
//**************************************************************************************/
void CPQMSetScanOffsetDlg::OnMoveCouchRightButton()
{
    // TODO: Add your control notification handler code here
    IncrementDecrementValue(ScanOffsetZ, (m_couch_pos == COUCH_POSITION_LEFT) ? 1.f : -1.f);
    UpdateCouchOffset();
}



/*************************************************************************************
//Module:  PaintStaticImages
//Author:  PATNI/HEMANT
//Purpose:
//**************************************************************************************/
void CPQMSetScanOffsetDlg::PaintStaticImages(
    CDC* f_dc
)
{

    CRect l_rect;

    CPen pen(PS_SOLID, 1, RGB(28, 60, 85));
    CPen* Old_pen = f_dc->SelectObject(&pen);
    CBrush* Old_brush = (CBrush*)f_dc->SelectStockObject(NULL_BRUSH);

    GetDlgItem(IDC_IMAGE_OFFSET)->GetWindowRect(&l_rect);
    ScreenToClient(&l_rect);

    CPQMImageUtility::DrawBitmap(IDB_IMAGE_OFFSET_HUMAN, l_rect, f_dc);

    GetDlgItem(IDC_IMAGE_OFFSET)->GetWindowRect(&l_rect);
    ScreenToClient(&l_rect);

    l_rect.InflateRect(1, 1);
    f_dc->Rectangle(&l_rect);

    GetDlgItem(IDC_MOVE_COUCH_LEFT)->GetWindowRect(&l_rect);
    ScreenToClient(&l_rect);
    CPQMImageUtility::DrawBitmap((m_couch_pos == COUCH_POSITION_LEFT) ? IDB_MOVE_COUCH_LEFT : IDB_MOVE_COUCH_RIGHT, l_rect, f_dc);
    l_rect.InflateRect(1, 1);
    f_dc->Rectangle(&l_rect);

    f_dc->SelectObject(Old_pen);
    f_dc->SelectObject(Old_brush);
}

/*************************************************************************************
//Module:  OnUpdateCouchOffset
//Author:  PATNI/HEMANT
//Purpose:
//**************************************************************************************/
LRESULT CPQMSetScanOffsetDlg::OnUpdateCouchOffset(
    WPARAM wParam,
    LPARAM lParam
)
{

    const ScanOffset& l_max_offset = m_scan_offset.GetMaxOffset();
    const ScanOffset& l_min_offset = m_scan_offset.GetMinOffset();
    const ScanOffset& l_offset = m_scan_offset.GetOffset();

    const float l_increment_factor = m_move_couchwnd.GetDragFactorX() * ((m_couch_pos == COUCH_POSITION_LEFT) ? 1 : -1);
    const float l_increment = ((l_increment_factor < 0.f) ? (-1) * l_min_offset[ScanOffsetZ] : l_max_offset[ScanOffsetZ]) ;

    const float l_newoffset = l_increment * l_increment_factor ;
    IncrementDecrementValue(ScanOffsetZ, (l_newoffset - l_offset[ScanOffsetZ]));

    return 0;
}


/*************************************************************************************
//Module:  OnKillfocusEditScanOffsetX
//Author:  PATNI/HEMANT
//Purpose:
//**************************************************************************************/
void CPQMSetScanOffsetDlg::OnKillfocusEditScanOffsetX(
)
{
    UpdateEditBox(ScanOffsetX);
}

/*************************************************************************************
//Module:  OnKillfocusEditScanOffsetY
//Author:  PATNI/HEMANT
//Purpose:
//**************************************************************************************/
void CPQMSetScanOffsetDlg::OnKillfocusEditScanOffsetY(
)
{
    UpdateEditBox(ScanOffsetY);
}

/*************************************************************************************
//Module:  OnKillfocusEditScanOffsetZ
//Author:  PATNI/HEMANT
//Purpose:
//**************************************************************************************/
void CPQMSetScanOffsetDlg::OnKillfocusEditScanOffsetZ(
)
{
    UpdateEditBox(ScanOffsetZ);
    UpdateCouchOffset();
}

/*************************************************************************************
//Module:  UpdateCouchOffset
//Author:  PATNI/HEMANT
//Purpose:
//**************************************************************************************/
void CPQMSetScanOffsetDlg::UpdateCouchOffset(
)
{
    const ScanOffset& l_max_offset = m_scan_offset.GetMaxOffset();
    const ScanOffset& l_min_offset = m_scan_offset.GetMinOffset();
    const ScanOffset& l_offset = m_scan_offset.GetOffset();

    const float l_factor = (l_offset[ScanOffsetZ] < 0.f) ? (-1) * l_min_offset[ScanOffsetZ] : l_max_offset[ScanOffsetZ] ;

    const float l_facorZ = (l_offset[ScanOffsetZ] / l_factor) * ((m_couch_pos == COUCH_POSITION_LEFT) ? 1 : -1);

    m_move_couchwnd.SetImagePos(l_facorZ);
    m_move_couchwnd.SetImagePos(l_facorZ);
}


/*************************************************************************************
//Module:  UpdateControlPos
//Author:  PATNI/HEMANT
//Purpose:
//**************************************************************************************/
void CPQMSetScanOffsetDlg::UpdateControlPos()
{

    if (m_couch_pos == COUCH_POSITION_LEFT) return;

    CRect l_client_rect;
    GetClientRect(&l_client_rect);

    CRect l_ref_wnd_rect;
    GetDlgItem(IDC_MOVE_COUCH_LEFT)->GetWindowRect(&l_ref_wnd_rect);
    ScreenToClient(&l_ref_wnd_rect);

    int l_right_gap = 0;
    CRect l_control_rect;

#define UPDATEBUTTONPOS(wnd)	wnd.GetWindowRect(&l_control_rect);	\
    ScreenToClient(&l_control_rect);	\
    l_right_gap = l_ref_wnd_rect.left + (l_ref_wnd_rect.right - l_control_rect.right) ;		\
    l_control_rect.right = l_right_gap + l_control_rect.Width();							\
    l_control_rect.left = l_right_gap ;

#define SETWINDOWPOS(wnd, rect)	wnd.SetWindowPos(NULL,			\
        rect.left,						\
        rect.top,						\
        rect.Width(),					\
        rect.Height(),					\
        SWP_NOZORDER | SWP_NOSIZE);

    UPDATEBUTTONPOS(m_move_couch_range_static)
    SETWINDOWPOS(m_move_couch_range_static, l_control_rect)

    UPDATEBUTTONPOS(m_move_couch_edit)
    SETWINDOWPOS(m_move_couch_edit, l_control_rect)

    UPDATEBUTTONPOS(m_move_couch_left_button)

    CRect l_left_button_rect, l_right_button_rect;
    l_left_button_rect = l_control_rect;

    UPDATEBUTTONPOS(m_move_couch_right_button)
    l_right_button_rect = l_control_rect;


    SETWINDOWPOS(m_move_couch_left_button, l_right_button_rect)
    SETWINDOWPOS(m_move_couch_right_button, l_left_button_rect)

#undef SETWINDOWPOS
#undef UPDATEBUTTONPOS
}


/*************************************************************************************
//Module:  GetPatientOrientationOptionFor
//Author:  PATNI/HEMANT
//Purpose:
//**************************************************************************************/
int CPQMSetScanOffsetDlg::GetPatientOrientationOptionFor(
    const OrientationType f_orientation_type,
    const int orientation_value
)const
{

    switch (f_orientation_type) {

        case OriType_PatientInsertDir :

            switch (orientation_value) {
                case  VFC_PATORI_HEAD_FIRST :
                    return (int) Patient_FeetFirst;

                case VFC_PATORI_FEET_FIRST :
                    return (int) Patient_HeadFirst;
            }

            break ;

        case OriType_PatientRotationDir :

            switch (orientation_value) {

                case VFC_PATORI_SUPINE:
                    return (int) PatientRotation_SUPINE ;

                case VFC_PATORI_PRONE:
                    return (int) PatientRotation_PRONE ;

                case VFC_PATORI_RIGHT_UP:
                    return (int) PatientRotation_RIGHT_UP ;

                case VFC_PATORI_LEFT_UP:
                    return (int) PatientRotation_LEFT_UP ;
            }

            break ;
            //-Patni-Hemant/2009Nov17/Modified/IR#90/Patient Orientation

        case OriType_PatientViewDir :

            switch (orientation_value) {

                case VFC_VIEW_FROM_HEAD:
                    return (int)PatientView_FROM_HEAD ;
                    break;

                case VFC_VIEW_FROM_FEET:
                    return (int) PatientView_FROM_FEET ;
                    break;
            }

            break ;
    }

    return -1 ;
}


/*************************************************************************************
//Module:  InitializeDlg
//Author:  PATNI/HEMANT
//Purpose:
//**************************************************************************************/
void CPQMSetScanOffsetDlg::InitializeDlg()
{

    WINDOW_COLORS_t l_dialog_color;
    GetWindowColors(l_dialog_color);
    l_dialog_color.crClientColor = RGB(63, 82, 118);
    SetWindowColors(l_dialog_color);

    InitCtrls();
    InitData();

    InitStaticCtrls();
    InitEditBoxes();
    InitButtons();

    UpdateEditBoxAll();


    SetDialogRibbonHeight();
}


/*************************************************************************************
//Module:  InitCtrls
//Author:  PATNI/HEMANT
//Purpose:
//**************************************************************************************/
void CPQMSetScanOffsetDlg::InitCtrls(
)
{

    CWnd* wnd = GetDlgItem(IDC_MOVE_COUCH_WND);

    if (wnd) {
        CRect graph_rect;
        wnd->GetWindowRect(&graph_rect);
        ScreenToClient(&graph_rect);
        wnd->DestroyWindow();
        m_move_couchwnd.Create(
            NULL, _T("Move Couch"),
            WS_CHILD | WS_VISIBLE,
            graph_rect, this, IDC_MOVE_COUCH_WND
        );
    }


    m_move_couchwnd.InitMoveCouchWnd();

    UpdateControlPos();
}



/*************************************************************************************
//Module:  InitData
//Author:  PATNI/HEMANT
//Purpose:
//**************************************************************************************/
void CPQMSetScanOffsetDlg::InitData(
)
{
    UpdateCouchOffset();
}



/*************************************************************************************
//Module:  InitStaticCtrls
//Author:  PATNI/HEMANT
//Purpose:
//**************************************************************************************/
void CPQMSetScanOffsetDlg::InitStaticCtrls()
{

    WINDOW_COLORS_t stColors_o;
    GetWindowColors(stColors_o);
    COLORREF bk_color = stColors_o.crClientColor;

    const COLORREF  g_pqmdialog_static_ctrl_text_color   = RGB(191, 209, 255);
    const CString g_pqmdialog_static_font_name		     =	_T("Arial");
    const int g_pqmdialog_static_font_size			     =	130 ;

    m_image_offset_static.SetColorData(bk_color, g_pqmdialog_static_ctrl_text_color);
    m_image_offset_static.SetFontAttributes(g_pqmdialog_static_font_name, g_pqmdialog_static_font_size);
    m_image_offset_static.UpdateFont();

    m_move_couch_static.SetColorData(bk_color, g_pqmdialog_static_ctrl_text_color);
    m_move_couch_static.SetFontAttributes(g_pqmdialog_static_font_name, g_pqmdialog_static_font_size);
    m_move_couch_static.UpdateFont();

    m_move_couch_range_static.SetColorData(g_image_bk_color, g_pqmdialog_static_ctrl_text_color);
    m_move_couch_range_static.SetFontAttributes(g_pqmdialog_static_font_name, g_pqmdialog_static_font_size);
    m_move_couch_range_static.UpdateFont();

    if (m_couch_pos == COUCH_POSITION_RIGHT) {
        m_move_couch_range_static.ModifyStyle(SS_RIGHT, SS_LEFT);

    } else {
        m_move_couch_range_static.ModifyStyle(SS_LEFT, SS_RIGHT);
    }

    UpdateMinMaxCouchRange();
}



/*************************************************************************************
//Module:  UpdateMinMaxCouchRange
//Author:  PATNI/HEMANT
//Purpose:
//**************************************************************************************/
void CPQMSetScanOffsetDlg::UpdateMinMaxCouchRange(
)
{
    const ScanOffset& l_min_offset = m_scan_offset.GetMinOffset();
    const ScanOffset& l_max_offset = m_scan_offset.GetMaxOffset();

    CString str;
    str.Format(_T("(%4.1f - %4.1f cm)"), l_min_offset[ScanOffsetZ], l_max_offset[ScanOffsetZ]);

    m_move_couch_range_static.SetWindowText(str);

}



/*************************************************************************************
//Module:  InitButtons
//Author:  PATNI/HEMANT
//Purpose:
//**************************************************************************************/
void CPQMSetScanOffsetDlg::InitButtons()
{

    SetBitmapForButton(m_move_couch_left_button, IDB_MOVE_COUCH_LEFT_BUTTON_BITMAP);
    SetBitmapForButton(m_move_couch_right_button, IDB_MOVE_COUCH_RIGHT_BUTTON_BITMAP);
}


/*************************************************************************************
//Module:  SetBitmapForButton
//Author:  PATNI/HEMANT
//Purpose:
//**************************************************************************************/
BOOL CPQMSetScanOffsetDlg::SetBitmapForButton(
    ButtonEx& button_object,
    const UINT bitmap_resource_id,
    const bool center_bitmap /*= true*/
)
{
    return CUIButtonUtility::GetInstance()->SetBitmapForButton(button_object, bitmap_resource_id, RGB(0, 255, 0), center_bitmap);
}


/*************************************************************************************
//Module:  InitEditBoxes
//Author:  PATNI/HEMANT
//Purpose:
//**************************************************************************************/
void CPQMSetScanOffsetDlg::InitEditBoxes()
{

    const CString l_button_font_name = _T("Arial") ;
    const int l_edit_font = 11;
    const COLORREF l_edit_text_color = RGB(191, 209, 255);

    m_lr_edit.SetBkColor(g_image_bk_color);
    m_ap_edit.SetBkColor(g_image_bk_color);
    m_move_couch_edit.SetBkColor(g_image_bk_color);

    m_lr_edit.SetFont(l_button_font_name, l_edit_font);
    m_ap_edit.SetFont(l_button_font_name, l_edit_font);
    m_move_couch_edit.SetFont(l_button_font_name, l_edit_font);

    m_lr_edit.SetTextColor(l_edit_text_color);
    m_ap_edit.SetTextColor(l_edit_text_color);
    m_move_couch_edit.SetTextColor(l_edit_text_color);

    m_lr_edit.SetLimitText(TEXT_LIMIT);
    m_ap_edit.SetLimitText(TEXT_LIMIT);
    m_move_couch_edit.SetLimitText(TEXT_LIMIT);

    m_lr_edit.SetTextAlignment(VCENTER);
    m_ap_edit.SetTextAlignment(VCENTER);
    m_move_couch_edit.SetTextAlignment(VCENTER);
}

/*************************************************************************************
//Module:  SetDialogRibbonHeight
//Author:  PATNI/HEMANT
//Purpose:
//**************************************************************************************/
void CPQMSetScanOffsetDlg::SetDialogRibbonHeight(
)
{

    CRect client_rect;
    GetClientRect(&client_rect);

    CRect ok_button_rect ;
    m_ok.GetWindowRect(&ok_button_rect);
    ScreenToClient(&ok_button_rect);

    const int height_diff = client_rect.bottom - ok_button_rect.top + RIBBON_GAP;

    SetRibbonHeight(height_diff);
}


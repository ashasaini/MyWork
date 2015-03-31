//TMSC-REDMINE-783
//TMSC-REDMINE-782
//TMSC-REDMINE-781
//TMSC-Redmine-780
//TMSC-REDMINE-778
//TMSC-REDMINE-777
//TMSC-REDMINE-774
// PatientOrientationDialog.cpp : implementation file
//

#include "stdafx.h"
#include "PatientOrientationDialog.h"
#include <libStudyManager/UI/ZOrderLayerFactory.h>
#include <PQM/resource.h>
#include "DPSManager.h"
#include "UIButtonUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using MR::ACQ::SM::COMMON::UI::CZOrderLayerFactory;

BEGIN_MESSAGE_MAP(CPatientOrientationDialog, CZOrderPatientOriDialog)
    //{{AFX_MSG_MAP(CPatientOrientationDialog)
    ON_WM_ERASEBKGND()
    ON_BN_CLICKED(IDC_ORIENTATION_OPTION_1, OnOrientationOption1)
    ON_BN_CLICKED(IDC_ORIENTATION_OPTION_2, OnOrientationOption2)
    ON_BN_CLICKED(IDC_ORIENTATION_OPTION_3, OnOrientationOption3)
    ON_COMMAND_RANGE(ID_ORIENTATION1_1, ID_ORIENTATION1_2, OnOrientationCombo1)
    ON_COMMAND_RANGE(ID_ORIENTATION2_1, ID_ORIENTATION2_4, OnOrientationCombo2)
    ON_COMMAND_RANGE(ID_ORIENTATION3_1, ID_ORIENTATION3_2, OnOrientationCombo3)
    ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
    ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
    ON_WM_SETCURSOR()

    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPatientOrientationDialog dialog


CPatientOrientationDialog::CPatientOrientationDialog(CPatientOrientationInterface* patientorientation_interface, CWnd* pParent /*=NULL*/, const bool allow_orientation_change /*= true*/, const COUCH_POSITION couch_position/*= COUCH_POSITION_RIGHT*/)
    : CZOrderPatientOriDialog(CZOrderLayerFactory::GetPanelLayer(),
                              CDPSManager::GetInstance(), IDD_PATIENT_ORIENTATION_DIALOG, pParent),
    //Patni-PJS/2010Apr20/Added/JaFT# MVC007170_CDR_01
    m_orientationoption1(PatientInsertDirection_Invalid),
    m_orientationoption2(PatientRotationDirection_Invalid),
    m_orientationoption3(PatientViewDirection_Invalid),
    m_couch_position(couch_position),
    m_allow_orientation_change(allow_orientation_change),
    m_patientorientation_interface(patientorientation_interface),
    m_show_wait_cursor_for_patientorientationdlg(false)
{
    //{{AFX_DATA_INIT(CPatientOrientationDialog)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT


    for (int index = 0; index < OrientationType_Total ; index++) {
        m_allow_orienationFor[index] = true ;
    }


}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CPatientOrientationDialog::~CPatientOrientationDialog(
)
{
    //DeleteImageListForOrientations();
    UnLoadMainImageArray();

    if (m_bkbrush.m_hObject)
        m_bkbrush.DeleteObject();

    m_tooltip_font.DeleteObject();

}


/////////////////////////////////////////////////////////////////////////////
// CPatientOrientationDialog message handlers

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPatientOrientationDialog::UpdateOrientationAccordingto(
    const PatientInsertDirection orientationoption1,
    const PatientRotationDirection orientationoption2,
    const PatientViewDirection orientationoption3
)
{
    if ((m_orientationoption1 == orientationoption1)
        && (m_orientationoption2 == orientationoption2)
        && (m_orientationoption3 == orientationoption3)
       )
        return ;

    m_orientationoption1 = orientationoption1 ;
    m_orientationoption2 = orientationoption2 ;
    m_orientationoption3 = orientationoption3 ;

    UpdateComboImage(OriType_PatientInsertDir);
    UpdateComboImage(OriType_PatientRotationDir);
    UpdateComboImage(OriType_PatientViewDir);
    UpdateMainImage();
}

//************************************Method Header************************************
// Method Name  : OnOffOrientationChangeFlag
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPatientOrientationDialog::OnOffOrientationChangeFlag(
    const bool allow_orientation_change
)
{
    m_allow_orientation_change = allow_orientation_change ;
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
PatientInsertDirection CPatientOrientationDialog::GetPatientOrientationOption1(
)const
{
    return m_orientationoption1;
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
PatientRotationDirection CPatientOrientationDialog::GetPatientOrientationOption2(
)const
{

    return m_orientationoption2;
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
PatientViewDirection CPatientOrientationDialog::GetPatientOrientationOption3(
)const
{
    return m_orientationoption3;
}
//************************************Method Header************************************
// Method Name  : SetWaitCursor
// Author       : iGate
// Purpose      :
//***********************************************************************************
void CPatientOrientationDialog::SetWaitCursor(
    const bool f_show_wait_cursor
)
{
    m_show_wait_cursor_for_patientorientationdlg = f_show_wait_cursor;

}

void CPatientOrientationDialog::DoDataExchange(CDataExchange* pDX)
{
    CZOrderPatientOriDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPatientOrientationDialog)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    DDX_Control(pDX, IDC_ORIENTATION_OPTION_1	, m_orientation_option_1);
    DDX_Control(pDX, IDC_ORIENTATION_OPTION_2	, m_orientation_option_2);
    DDX_Control(pDX, IDC_ORIENTATION_OPTION_3	, m_orientation_option_3);
    DDX_Control(pDX, IDC_PATIENT_ORIENTATION_MAIN_IMAGE_STATIC	, m_patient_orientation_main_image_static);
    //}}AFX_DATA_MAP
}



//+Patni-HAR/2009JAN13/MODIFIED/MVC006380

void CPatientOrientationDialog::OnOK()
{
    // TODO: Add extra validation here

    //we don't call following for modeless dialog..
    //CDialog::OnOK();
}

void CPatientOrientationDialog::OnCancel()
{
    // TODO: Add extra cleanup here

    //we don't call following for modeless dialog..
    //CDialog::OnCancel();
}


BOOL CPatientOrientationDialog::OnInitDialogImpl()
{
    if (m_couch_position == COUCH_POSITION_INVALID) {

        ASSERT(FALSE);
        return false ;
    }


    LoadMainImageArray();

    const DWORD style = m_patient_orientation_main_image_static.GetStyle();
    const DWORD new_style = style | (SS_BITMAP | SS_NOTIFY | SS_CENTERIMAGE);
    m_patient_orientation_main_image_static.ModifyStyle(style, new_style);

    SetButtonAppearence();
    UpdateMainImage();

    m_bkbrush.CreateSolidBrush(/*BK_COLOR*/RGB(27, 36, 54));

    InitToolTip();

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
BOOL CPatientOrientationDialog::OnEraseBkgnd(CDC* pDC)
{
    CBrush* old_brush = pDC->SelectObject(&m_bkbrush);

    CPen pen(PS_SOLID, 1, g_pqm_static_bkcolor);
    CPen* old_pen = pDC->SelectObject(&pen);

    CRect client_rect;
    GetClientRect(&client_rect);
    pDC->Rectangle(&client_rect);

    pDC->SelectObject(old_brush);
    pDC->SelectObject(old_pen);

    return TRUE;
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPatientOrientationDialog::OnOrientationOption1()
{
    LPCTSTR FUNC_NAME = _T("CPatientOrientationDialog::OnOrientationOption1");

    if (!CanChangeOrieantationFor(OriType_PatientInsertDir))
        return;

    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-Hemant/2009Nov17/Modified/IR#90/Patient Orientation
    //
    //ShowPopUpMenuFor(OriType_PatientInsertDir);
    m_orientationoption1 = GetNextOrientationOptionForType1();
    PatientOrientationOptionChangedFor(OriType_PatientInsertDir);
    //-Patni-Hemant/2009Nov17/Modified/IR#90/Patient Orientation
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPatientOrientationDialog::OnOrientationOption2()
{
    LPCTSTR FUNC_NAME = _T("CPatientOrientationDialog::OnOrientationOption2");

    if (!CanChangeOrieantationFor(OriType_PatientRotationDir))
        return;

    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-Hemant/2009Nov17/Modified/IR#90/Patient Orientation
    //
    //ShowPopUpMenuFor(OriType_PatientRotationDir);
    m_orientationoption2 = GetNextOrientationOptionForType2();
    PatientOrientationOptionChangedFor(OriType_PatientRotationDir);
    //-Patni-Hemant/2009Nov17/Modified/IR#90/Patient Orientation
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPatientOrientationDialog::OnOrientationOption3()
{
    LPCTSTR FUNC_NAME = _T("CPatientOrientationDialog::OnOrientationOption3");

    if (!CanChangeOrieantationFor(OriType_PatientViewDir))
        return;

    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-Hemant/2009Nov17/Modified/IR#90/Patient Orientation
    //
    //ShowPopUpMenuFor(OriType_PatientViewDir);
    m_orientationoption3 = GetNextOrientationOptionForType3();
    PatientOrientationOptionChangedFor(OriType_PatientViewDir);
    //-Patni-Hemant/2009Nov17/Modified/IR#90/Patient Orientation
}



//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPatientOrientationDialog::OnOrientationCombo1(UINT nID)
{

    LPCTSTR FUNC_NAME = _T("CPatientOrientationDialog::OnOrientationCombo1");

    switch (nID) {

        case ID_ORIENTATION1_1 :
            m_orientationoption1 = Patient_FeetFirst;
            break ;

        case ID_ORIENTATION1_2 :
            m_orientationoption1 = Patient_HeadFirst;
            break ;
    }

    UpdateComboImage(OriType_PatientInsertDir);
    UpdateMainImage();

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_patientorientation_interface) {

        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("m_patientorientation_interface pointer is NULL"));

        return;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections
    m_patientorientation_interface->OnOrienationChangedFor(OriType_PatientInsertDir);
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPatientOrientationDialog::OnOrientationCombo2(UINT nID)
{

    LPCTSTR FUNC_NAME = _T("CPatientOrientationDialog::OnOrientationCombo2");

    switch (nID) {

            //+Patni-Hemant/2009Nov16/Added/IR#90/ Patient Orientation
            //
            //        case ID_ORIENTATION2_1 :
            //            m_orientationoption2 = OrientationOption2_1;
            //            break ;
            //        case ID_ORIENTATION2_2 :
            //            m_orientationoption2 = OrientationOption2_2;
            //            break ;
            //        case ID_ORIENTATION2_3 :
            //            m_orientationoption2 = OrientationOption2_3;
            //            break ;
            //        case ID_ORIENTATION2_4 :
            //            m_orientationoption2 = OrientationOption2_4;
            //            break ;

        case ID_ORIENTATION2_1 :
            m_orientationoption2 = PatientRotation_SUPINE;
            break ;

        case ID_ORIENTATION2_2 :
            m_orientationoption2 = PatientRotation_LEFT_UP;
            break ;

        case ID_ORIENTATION2_3 :
            m_orientationoption2 = PatientRotation_PRONE;
            break ;

        case ID_ORIENTATION2_4 :
            m_orientationoption2 = PatientRotation_RIGHT_UP;
            break ;
            //
            //-Patni-Hemant/2009Nov16/Added/IR#90/ Patient Orientation

    }

    UpdateComboImage(OriType_PatientRotationDir);
    UpdateMainImage();

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_patientorientation_interface) {

        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("m_patientorientation_interface pointer is NULL"));

        return;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections
    m_patientorientation_interface->OnOrienationChangedFor(OriType_PatientRotationDir);
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPatientOrientationDialog::OnOrientationCombo3(UINT nID)
{

    LPCTSTR FUNC_NAME = _T("CPatientOrientationDialog::OnOrientationCombo3");

    switch (nID) {

        case ID_ORIENTATION3_1 :
            m_orientationoption3 = PatientView_FROM_HEAD;
            break ;

        case ID_ORIENTATION3_2 :
            m_orientationoption3 = PatientView_FROM_FEET;
            break ;
    }

    UpdateComboImage(OriType_PatientViewDir);
    UpdateMainImage();

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_patientorientation_interface) {

        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("m_patientorientation_interface pointer is NULL"));

        return;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections
    m_patientorientation_interface->OnOrienationChangedFor(OriType_PatientViewDir);
}


BOOL CPatientOrientationDialog::OnToolTipText(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
{
    TOOLTIPTEXT* l_pttt = (TOOLTIPTEXT*) pNMHDR;

    if (l_pttt) {

        static CToolTipCtrl* l_pToolTip = NULL;
        CToolTipCtrl* l_ptt = AfxGetThreadState()->m_pToolTip;
        l_ptt->ModifyStyle(0, TTS_NOPREFIX);

        UINT nID = ((*l_pttt).hdr).idFrom;

        CString l_str_tooltip = GetToolTipTextForPatienOrientation(nID);

        if (l_str_tooltip.IsEmpty()) {
            return FALSE;
        }

        if (l_ptt != l_pToolTip) { // new tooltip

            l_ptt->SetTipBkColor(RGB(40, 106, 175));
            l_ptt->SetTipTextColor(RGB(255, 255, 255));
            l_ptt->SetFont(&m_tooltip_font, FALSE);
            l_pToolTip = l_ptt;

        } else if (l_ptt->GetFont() != &m_tooltip_font) {

            l_ptt->SetTipBkColor(RGB(40, 106, 175));
            l_ptt->SetTipTextColor(RGB(255, 255, 255));
            l_ptt->SetFont(&m_tooltip_font, TRUE);
        }

        l_ptt->SetMaxTipWidth(-1);


        CDC* l_tooltip_dc = l_ptt->GetDC();

        CRect calc_rect(0, 0, 0, 0);
        CFont* old_font = l_tooltip_dc->SelectObject(&m_tooltip_font);

        l_tooltip_dc->DrawText(l_str_tooltip, &calc_rect, DT_CALCRECT);

        l_tooltip_dc->SelectObject(old_font);

        l_tooltip_dc->DeleteDC();


        l_ptt->SetMaxTipWidth(calc_rect.Width() + 2);

        ::lstrcpy(l_pttt->szText, (LPCTSTR) l_str_tooltip);

    }

    *pResult = 0;
    return TRUE;
}
//************************************Method Header************************************
// Method Name  : OnSetCursor
// Author       : iGate
// Purpose      :
//***********************************************************************************

BOOL CPatientOrientationDialog::OnSetCursor(CWnd* pWnd,
        UINT nHitTest,
        UINT message)
{
    if (m_show_wait_cursor_for_patientorientationdlg) {
        ::SetCursor(::LoadCursor(NULL, IDC_WAIT));
        return TRUE ;
    }

    return CDialog::OnSetCursor(pWnd, nHitTest, message);

}

//************************************Method Header************************************
// Method Name  :PreTranslateMessageImpl
// Author       : iGATE
// Purpose      :
//***********************************************************************************
BOOL CPatientOrientationDialog::PreTranslateMessageImpl(
    MSG* pMsg
)
{
    if (m_show_wait_cursor_for_patientorientationdlg) {
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

    return CZOrderPatientOriDialog::PreTranslateMessageImpl(pMsg);
}

void CPatientOrientationDialog::InitToolTip()
{

    EnableToolTips(TRUE);

    m_tooltip_font.CreateFont(18,
                              0,
                              0,
                              0,
                              FW_NORMAL,
                              FALSE,
                              FALSE,
                              0,
                              ANSI_CHARSET,
                              OUT_DEFAULT_PRECIS,
                              CLIP_DEFAULT_PRECIS,
                              DEFAULT_QUALITY,
                              DEFAULT_PITCH | FF_SWISS,
                              _T("Arial")
                             );

}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPatientOrientationDialog::LoadImageForOrienationButton(
    ButtonEx& button,
    const UINT bitmap_resource_id
)
{

    SetBitmapForButton(button, bitmap_resource_id);

}



//************************************Method Header************************************
// Method Name  : UpdateMainImage
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPatientOrientationDialog::UpdateMainImage(
)
{
    SetMainImageFor(m_orientationoption1, m_orientationoption2, m_orientationoption3);
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPatientOrientationDialog::UpdateComboImage(
    const OrientationType orientation_type
)
{
    switch (orientation_type) {

        case OriType_PatientInsertDir : {
            LoadImageForOrienationButton(m_orientation_option_1, GetBitmapResourceIdFor(m_orientationoption1));
        }
        break ;

        case OriType_PatientRotationDir : {
            LoadImageForOrienationButton(m_orientation_option_2, GetBitmapResourceIdFor(m_orientationoption2));
        }
        break ;

        case OriType_PatientViewDir : {
            LoadImageForOrienationButton(m_orientation_option_3, GetBitmapResourceIdFor(m_orientationoption3));
        }
        break ;
    }
}


//************************************Method Header************************************
// Method Name  : LoadMainImageArray
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPatientOrientationDialog::LoadMainImageArray(
)
{
    //+Patni-PP/2009May28/Modified/SU14 CPP test initialization corrections
    //UINT bitmap_resource_startid = ( m_couch_position == COUCH_POSITION_LEFT) ? IDB_GRP11_BITMAP : IDB_GRP21_BITMAP;
    UINT bitmap_resource_startid = (m_couch_position == COUCH_POSITION_LEFT) ? IDB_GRP11_BITMAP : IDB_GRP21_BITMAP;

    //-Patni-PP/2009May28/Modified/SU14 CPP test initialization corrections
    for (int bitmap = 0; bitmap < TOTAL_MAIN_IMAGES ; bitmap++) {
        m_main_image_array[bitmap].LoadBitmap(bitmap_resource_startid ++);
    }
}



//************************************Method Header************************************
// Method Name  : UnLoadMainImageArray
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPatientOrientationDialog::UnLoadMainImageArray(
)
{
    for (int bitmap = 0; bitmap < TOTAL_MAIN_IMAGES ; bitmap++) {
        m_main_image_array[bitmap].DeleteObject();
    }
}


//************************************Method Header************************************
// Method Name  : CanChangeOrieantationFor
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPatientOrientationDialog::CanChangeOrieantationFor(
    const OrientationType orientation_type
)const
{
    //check if global flag for all the orientation is off
    //if its off , don't allow for any combo
    if (!m_allow_orientation_change)
        return false;

    return m_allow_orienationFor[orientation_type];
}


//+Patni-Hemant/2009Nov17/Added/IR#90/Patient Orientation
//************************************Method Header************************************
// Method Name  : PatientOrientationOptionChangedFor
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPatientOrientationDialog::PatientOrientationOptionChangedFor(
    const OrientationType f_orientation_type
)
{
    LPCTSTR FUNC_NAME = _T("CPatientOrientationDialog::PatientOrientationOptionChangedFor");
    PQM_TRACE_SCOPED(FUNC_NAME);

    UpdateComboImage(f_orientation_type);
    UpdateMainImage();

    if (m_patientorientation_interface) {
        m_patientorientation_interface->OnOrienationChangedFor(f_orientation_type);
    }
}
//-Patni-Hemant/2009Nov17/Modified/IR#90/Patient Orientation


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
const MainOrientationImage CPatientOrientationDialog::GetPatientOrientationMainImageFor(
    const PatientInsertDirection option1,
    const PatientRotationDirection option2,
    const PatientViewDirection option3
)const
{
    return (MainOrientationImage)((((int)option1 + 1) * 100) +
                                  (((int)option2 + 1) * 10) +
                                  ((int)option3 + 1)) ;
}



//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
int CPatientOrientationDialog::GetMainImageArrayIndexFor(
    const MainOrientationImage main_orientation_value
)const
{
    if ((main_orientation_value >= MainOrientationImage_111) &&
        (main_orientation_value < MainOrientationImage_total)
       ) {

        switch (main_orientation_value) {

            case MainOrientationImage_111 :
            case MainOrientationImage_112 :

                return 0;

            case MainOrientationImage_121 :
            case MainOrientationImage_122 :

                return 1;

            case MainOrientationImage_131 :
            case MainOrientationImage_132 :

                return 2;

            case MainOrientationImage_141 :
            case MainOrientationImage_142 :

                return 3;

            case MainOrientationImage_211 :
            case MainOrientationImage_212 :

                return 4;

            case MainOrientationImage_221 :
            case MainOrientationImage_222 :

                return 5;

            case MainOrientationImage_231 :
            case MainOrientationImage_232 :

                return 6;

            case MainOrientationImage_241 :
            case MainOrientationImage_242 :

                return 7;
        }
    }

    return -1;

}



//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPatientOrientationDialog::SetMainImageFor(
    const PatientInsertDirection option1,
    const PatientRotationDirection option2,
    const PatientViewDirection option3
)
{
    const int bitmap_resource_index = GetMainImageArrayIndexFor(GetPatientOrientationMainImageFor(option1, option2, option3)) ;

    if (bitmap_resource_index != -1) {
        m_patient_orientation_main_image_static.SetBitmap((HBITMAP) m_main_image_array[bitmap_resource_index]);

    } else {
        m_patient_orientation_main_image_static.SetBitmap(NULL);

        Invalidate();
        UpdateWindow();
    }
}


//+Patni-Hemant/2009Nov17/Added/IR#90/Patient Orientation
//************************************Method Header************************************
// Method Name  : GetNextOrientationOptionForType1
// Author       : PATNI/ HEMANT
// Purpose      : returns next orientation option for combo type OriType_PatientInsertDir
//***********************************************************************************
PatientInsertDirection CPatientOrientationDialog::GetNextOrientationOptionForType1(
)const
{

    PatientInsertDirection l_orientationoption1 = (PatientInsertDirection)(m_orientationoption1 + 1);

    if (l_orientationoption1 >= PatientInsertDirection_Invalid) {
        l_orientationoption1 = Patient_FeetFirst;
    }

    return l_orientationoption1;
}
//-Patni-Hemant/2009Nov17/Added/IR#90/Patient Orientation


//+Patni-Hemant/2009Nov17/Added/IR#90/Patient Orientation
//************************************Method Header************************************
// Method Name  : GetNextOrientationOptionForType2
// Author       : PATNI/ HEMANT
// Purpose      : returns next orientation option for combo type OriType_PatientRotationDir
//***********************************************************************************
PatientRotationDirection CPatientOrientationDialog::GetNextOrientationOptionForType2(
)const
{
    switch (m_orientationoption2) {

        case PatientRotation_SUPINE :
            return PatientRotation_LEFT_UP;
            break ;

        case PatientRotation_PRONE :
            return PatientRotation_RIGHT_UP;
            break ;

        case PatientRotation_RIGHT_UP :
            return PatientRotation_SUPINE;
            break ;

        case PatientRotation_LEFT_UP :
            return PatientRotation_PRONE;
            break ;

        case PatientRotationDirection_Invalid :
            return PatientRotationDirection_Invalid;
            break ;

        default :
            return PatientRotationDirection_Invalid;
    }

    return PatientRotationDirection_Invalid;
}


//+Patni-Hemant/2009Nov17/Added/IR#90/Patient Orientation
//************************************Method Header************************************
// Method Name  : GetNextOrientationOptionForType3
// Author       : PATNI/ HEMANT
// Purpose      : returns next orientation option for combo type OriType_PatientViewDir
//***********************************************************************************
PatientViewDirection CPatientOrientationDialog::GetNextOrientationOptionForType3(
)const
{
    PatientViewDirection l_orientationoption3 = (PatientViewDirection)(m_orientationoption3 + 1);

    if (l_orientationoption3 >= PatientViewDirection_Invalid) {
        l_orientationoption3 = PatientView_FROM_HEAD;
    }

    return l_orientationoption3;
}
//-Patni-Hemant/2009Nov17/Added/IR#90/Patient Orientation

//************************************Method Header************************************
// Method Name  : SetBitmapForButton
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
BOOL CPatientOrientationDialog::SetBitmapForButton(
    ButtonEx& f_button_object,
    const UINT bitmap_resource_id,
    const bool f_center_bitmap /*= true*/
)const
{
    return CUIButtonUtility::GetInstance()->SetIconForButton(f_button_object, bitmap_resource_id, f_center_bitmap);
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
int CPatientOrientationDialog::GetBitmapResourceIdFor(
    const PatientInsertDirection orientationoption
)const
{

    switch (orientationoption) {

        case Patient_FeetFirst :
            return (m_couch_position == COUCH_POSITION_LEFT) ? IDB_GRP1_COMBO11_BITMAP : IDB_GRP2_COMBO11_BITMAP ;

        case Patient_HeadFirst :
            return (m_couch_position == COUCH_POSITION_LEFT) ? IDB_GRP1_COMBO12_BITMAP : IDB_GRP2_COMBO12_BITMAP ;
    }

    return -1;
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
int CPatientOrientationDialog::GetBitmapResourceIdFor(
    const PatientRotationDirection orientationoption
)const
{

    switch (orientationoption) {

            //+Patni-Hemant/2009Nov16/Added/IR#90/ Patient Orientation
            //
            //        case OrientationOption2_1 :
            //            return (m_couch_position == COUCH_POSITION_LEFT) ? IDB_GRP1_COMBO21_BITMAP : IDB_GRP2_COMBO21_BITMAP ;
            //        case OrientationOption2_2 :
            //            return (m_couch_position == COUCH_POSITION_LEFT) ?  IDB_GRP1_COMBO22_BITMAP : IDB_GRP2_COMBO22_BITMAP ;
            //        case OrientationOption2_3 :
            //            return (m_couch_position == COUCH_POSITION_LEFT) ?  IDB_GRP1_COMBO23_BITMAP : IDB_GRP2_COMBO23_BITMAP ;
            //        case OrientationOption2_4 :
            //            return (m_couch_position == COUCH_POSITION_LEFT) ? IDB_GRP1_COMBO24_BITMAP : IDB_GRP2_COMBO24_BITMAP;

        case PatientRotation_SUPINE :
        case PatientRotation_PRONE :
        case PatientRotation_RIGHT_UP :
        case PatientRotation_LEFT_UP :
            return (m_couch_position == COUCH_POSITION_LEFT) ?  IDB_GRP1_COMBO23_BITMAP : IDB_GRP2_COMBO23_BITMAP ;
            //
            //-Patni-Hemant/2009Nov16/Added/IR#90/ Patient Orientation
    }

    return -1;
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
int CPatientOrientationDialog::GetBitmapResourceIdFor(
    const PatientViewDirection orientationoption
)const
{

    switch (orientationoption) {

        case PatientView_FROM_HEAD :
            return (m_couch_position == COUCH_POSITION_LEFT) ? IDB_GRP1_COMBO31_BITMAP : IDB_GRP2_COMBO31_BITMAP ;

        case PatientView_FROM_FEET :
            return (m_couch_position == COUCH_POSITION_LEFT) ? IDB_GRP1_COMBO32_BITMAP : IDB_GRP2_COMBO32_BITMAP ;
    }

    return -1;
}



//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPatientOrientationDialog::SetButtonAppearence(
)
{
    m_orientation_option_1.SetCurveType(CURVE_SMALL_3);
    m_orientation_option_2.SetCurveType(CURVE_SMALL_3);
    m_orientation_option_3.SetCurveType(CURVE_SMALL_3);

    SetBitmapsForButtons();
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPatientOrientationDialog::SetBitmapsForButtons(
)
{

    UpdateComboImage(OriType_PatientInsertDir);
    UpdateComboImage(OriType_PatientRotationDir);
    UpdateComboImage(OriType_PatientViewDir);
}


const CString CPatientOrientationDialog::GetToolTipTextForPatienOrientation(
    const UINT f_nID
) const
{

    if (f_nID == (UINT)m_orientation_option_1.m_hWnd) {

        PatientInsertDirection l_patient_insertdir = GetPatientOrientationOption1();

        switch (l_patient_insertdir) {
            case Patient_FeetFirst:
                return CPQMUtility::GetMultiLingualString(_T("IDS_PQM_HEAD_FIRST"));
                break;

            case Patient_HeadFirst:
                return CPQMUtility::GetMultiLingualString(_T("IDS_PQM_FEET_FIRST"));
                break;
        }

    } else if (f_nID == (UINT)m_orientation_option_2.m_hWnd) {

        PatientRotationDirection l_patient_rotationdir = GetPatientOrientationOption2();

        switch (l_patient_rotationdir) {

            case PatientRotation_SUPINE:
                return CPQMUtility::GetMultiLingualString(_T("IDS_PQM_ORIENTATION_SUPINE"));
                break;

            case PatientRotation_PRONE:
                return CPQMUtility::GetMultiLingualString(_T("IDS_PQM_ORIENTATION_PRONE"));
                break;

            case PatientRotation_RIGHT_UP:
                return CPQMUtility::GetMultiLingualString(_T("IDS_PQM_ORIENTATION_RIGHT_UP"));
                break;

            case PatientRotation_LEFT_UP:
                return CPQMUtility::GetMultiLingualString(_T("IDS_PQM_ORIENTATION_LEFT_UP"));
                break;
        }

    } else if (f_nID == (UINT)m_orientation_option_3.m_hWnd) {

        PatientViewDirection l_patient_viewdir = GetPatientOrientationOption3();

        if (l_patient_viewdir == PatientViewDirection_Invalid) {

            return _T("");
        }

        return CPQMUtility::GetMultiLingualString(_T("IDS_PQM_ORIENTATION_VIEW"));
    }

    return _T("");

}


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
 *  File Name: PQMAnatomyDlg.cpp
 *  Overview: Implementation of CPQMAnatomyDlg class.
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
#include "PQMAnatomyDlg.h"
#include <libStudyManager/UI/ZOrderLayerFactory.h>
#include <PQM/resource.h>
#include "PQMView.h"
#include "DPSManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using MR::ACQ::SM::COMMON::UI::CZOrderDialog;
using MR::ACQ::SM::COMMON::UI::CZOrderLayerFactory;


BEGIN_MESSAGE_MAP(CPQMAnatomyDlg, CZOrderDialog)

    //{{AFX_MSG_MAP(CPQMAnatomyDlg)

    ON_STN_CLICKED(IDC_STATIC_CHEST_BUTTON      , OnChestButton)
    ON_STN_CLICKED(IDC_STATIC_ABDOMEN_BUTTON    , OnAbdomenButton)
    ON_STN_CLICKED(IDC_STATIC_PELVIS_BUTTON     , OnPelvisButton)
    ON_STN_CLICKED(IDC_STATIC_SHOULDER_BUTTON   , OnShoulderButton)
    ON_STN_CLICKED(IDC_STATIC_EXTREMITY_BUTTON  , OnExtremityButton)
    //Patni-PJS/2010Apr09/Added/JaFT# IR-137
    //ON_STN_CLICKED(IDC_STATIC_EXTREMITY2_BUTTON , OnExtremity2Button)
    ON_STN_CLICKED(IDC_STATIC_HAND_BUTTON       , OnHandButton)
    ON_STN_CLICKED(IDC_STATIC_ANKLE_BUTTON      , OnAnkleButton)
    ON_STN_CLICKED(IDC_STATIC_HEAD_BUTTON       , OnHeadButton)
    ON_STN_CLICKED(IDC_STATIC_TLSPINE_BUTTON    , OnTSpineButton)
    ON_STN_CLICKED(IDC_STATIC_CSPINE_BUTTON     , OnCSpineButton)

    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

//************************************Method Header**************************************

//Method Name:  CPQMAnatomyDlg

//Author:  PATNI/SVP

//Purpose: Constructor

//*************************************************************************************

CPQMAnatomyDlg::CPQMAnatomyDlg(const CRect& parent_rect /*= CRect(0,0,0,0)*/, const int currently_selected_sar_region /*= 0*/, CWnd* pParent /*=NULL*/)

    : CZOrderDialog(CZOrderLayerFactory::GetIselLayer(),
                    CDPSManager::GetInstance(), IDD_ANATOMY, pParent),
    //Patni-PJS/2010Apr20/Added/JaFT# MVC007170_CDR_01
    m_currently_selected_sar_region((SAR_REGION) currently_selected_sar_region),
    m_parent((CPQMView*)pParent),
    m_parent_rect(parent_rect),
    m_nItem_index(1),
    m_bPrev_auto_scan_flag_status(true),
    n_extremityCounter(1)
{

    LPCTSTR FUNC_NAME = _T("CPQMAnatomyDlg::CPQMAnatomyDlg");
    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
              _T("Trace of Constructor"));



}


//************************************Method Header************************************
// Method Name  :~CPQMAnatomyDlg
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CPQMAnatomyDlg::~CPQMAnatomyDlg()
{

    for (int bitmap_count = 0; bitmap_count < TOTAL_ANATOMY_BITMAPS ; bitmap_count++) {

        if (m_anatomy_bitmap[bitmap_count].m_hObject)
            m_anatomy_bitmap[bitmap_count].DeleteObject();
    }
}


//+Patni-MN 2009Mar16/Added/PSP1#72
//************************************Method Header************************************
// Method Name  : SetVariablesForParameterEditing
// Author       : PATNI/ Manishkumar
// Purpose      : Set the values for parameter editiing, called from PQMView.
//***********************************************************************************
void CPQMAnatomyDlg::SetVariablesForParameterEditing(const int f_nItem_index, const bool f_bprev_auto_scan_flag_status)
{
    m_nItem_index = f_nItem_index;
    m_bPrev_auto_scan_flag_status = f_bprev_auto_scan_flag_status;
}
//-Patni-MN 2009Mar16/Added/PSP1#72


//************************************Method Header**************************************

//Method Name:  OnInitDialogImpl

//Author:  PATNI/GP

//Purpose: Initialize anatomy structure & load bitmaps on static controls

//**************************************************************************************/
BOOL CPQMAnatomyDlg::OnInitDialogImpl(
)
{

    LPCTSTR FUNC_NAME = _T("CPQMAnatomyDlg::OnInitDialogImpl");
    PQM_TRACE_SCOPED(FUNC_NAME);

    SetMultilingual();  // Added By KT/ACE-2/11-06-2009/Multilingualisation

    ASSERT(m_parent != NULL);

    if (m_parent == NULL) {

        CPQMLogMgr* l_pqm_mgr =  CPQMLogMgr::GetInstance();
        l_pqm_mgr->WriteEventLog(IDS_ERR_VIEW_PTR, ERROR_LOG_WARNING1  ,
                                 _T("PQMAnatomyDlg.cpp"), __LINE__); // Patni-KSS/ADDED/2010May19/IR-82


    } else {

        SetBitmapsForStatics();
        //now select currently selected sar..
        LoadImageForAnatomyButton(GetCurrentlySelectedSarRegion(), false);

        if (!m_parent_rect.IsRectEmpty()) {

            CRect sar_window_rect;
            GetWindowRect(&sar_window_rect);

            //Added by Hemant On 12/11/2008 1:48:53 PM
            const int border_height = GetSystemMetrics(SM_CYDLGFRAME) + GetSystemMetrics(SM_CYBORDER);

            const int window_left = m_parent_rect.right;
            const int window_top = m_parent_rect.bottom - sar_window_rect.Height() -
                                   (GetTitlBarHeight() - (2 * GetSystemMetrics(SM_CYCAPTION) - 2 * border_height));

            SetWindowPos(&wndTop , window_left , window_top , sar_window_rect.Width(), sar_window_rect.Height(), SWP_NOSIZE);
        }
    }

    SetRibbonHeight(0);


    return CZOrderDialog::OnInitDialogImpl(false);
}


//************************************Method Header**************************************

//Method Name:  OnCancel

//Author:  PATNI/SVP

//Purpose: Close Anatomy dialog

//**************************************************************************************
void CPQMAnatomyDlg::OnCancel(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMAnatomyDlg::OnCancel");

    CPQMLogMgr::GetInstance()->WriteOpeLog(OP_IDB_ANATOMY_CLOSE);

    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-HEMANT/ADDED On 4/3/2009 5:40:23 PM/ PSP / Internal Defect
    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_parent) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_parent pointer is NULL"));

    } else {
        //-Patni-DKH/2009Aug24/Modified/cpp test corrections
        m_parent->OnDialogClosedForParameterEditing(m_nItem_index, m_bPrev_auto_scan_flag_status);
    }//Patni-DKH/2009Aug24/Modified/cpp test corrections

    //-Patni-HEMANT/ADDED On 4/3/2009 5:40:23 PM/ PSP / Internal Defect
    //+Patni-PJS/2010Aug17/Modified/REDMINE-562
    if (m_nFlags & (WF_MODALLOOP | WF_CONTINUEMODAL)) {
        EndModalLoop(IDCANCEL);
    }

    //-Patni-PJS/2010Aug17/Modified/REDMINE-562
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMAnatomyDlg::OnChestButton(
)
{

    UpdateAnatomy(CHEST);
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMAnatomyDlg::OnAbdomenButton(
)
{
    UpdateAnatomy(ABDOMEN);
}
//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMAnatomyDlg::OnPelvisButton(
)
{

    UpdateAnatomy(PELVIS);
}
//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMAnatomyDlg::OnShoulderButton(
)
{
    UpdateAnatomy(SHOULDER);
}
//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMAnatomyDlg::OnExtremityButton(
)
{
    UpdateAnatomy(EXTREMITY);
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMAnatomyDlg::OnHandButton(
)
{
    UpdateAnatomy(HAND);
}
//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMAnatomyDlg::OnAnkleButton(
)
{
    UpdateAnatomy(ANKLE);
}
//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMAnatomyDlg::OnHeadButton(
)
{
    UpdateAnatomy(HEAD);
}
//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMAnatomyDlg::OnTSpineButton(
)
{
    UpdateAnatomy(TL_SPINE);
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMAnatomyDlg::OnCSpineButton(
)
{
    UpdateAnatomy(C_SPINE);
}


//************************************Method Header**************************************

//Method Name:  DoDataExchange

//Author:  PATNI/SVP

//Purpose: Data exchange

//*************************************************************************************

void CPQMAnatomyDlg::DoDataExchange(
    CDataExchange* pDX
)
{

    CZOrderDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPQMAnatomyDlg)

    DDX_Control(pDX, IDC_STATIC_CHEST_BUTTON     , m_anatomy_chest);
    DDX_Control(pDX, IDC_STATIC_ABDOMEN_BUTTON	 , m_anatomy_abdomen);
    DDX_Control(pDX, IDC_STATIC_PELVIS_BUTTON    , m_anatomy_pelvis);
    DDX_Control(pDX, IDC_STATIC_SHOULDER_BUTTON  , m_anatomy_shoulder);
    DDX_Control(pDX, IDC_STATIC_EXTREMITY_BUTTON , m_anatomy_extremity);
    //Patni-PJS/2010Apr09/Added/JaFT# IR-137
    //DDX_Control(pDX, IDC_STATIC_EXTREMITY2_BUTTON, m_anatomy_extremity2);
    DDX_Control(pDX, IDC_STATIC_HAND_BUTTON      , m_anatomy_hand);
    DDX_Control(pDX, IDC_STATIC_ANKLE_BUTTON     , m_anatomy_ankle);
    DDX_Control(pDX, IDC_STATIC_HEAD_BUTTON      , m_anatomy_head);
    DDX_Control(pDX, IDC_STATIC_TLSPINE_BUTTON   , m_anatomy_tlspine);
    DDX_Control(pDX, IDC_STATIC_CSPINE_BUTTON    , m_anatomy_cspine);

    //}}AFX_DATA_MAP
}
//************************************Method Header**************************************

//Method Name: StoreSelectedSarRegionInSelectedProtocol

//Author:  PATNI/SVP

//Purpose: Store Selected Sar Region In Selected Protocol

//**************************************************************************************/
void CPQMAnatomyDlg::StoreSelectedSarRegionInSelectedProtocol(
    const int sar_region
)
{
    LPCTSTR FUNC_NAME = _T("CPQMAnatomyDlg::StoreSelectedSarRegionInSelectedProtocol");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    ASSERT(m_parent != NULL);

    if (m_parent == NULL) {

        CPQMLogMgr* l_pqm_mgr =  CPQMLogMgr::GetInstance();
        l_pqm_mgr->WriteEventLog(IDS_ERR_VIEW_PTR, ERROR_LOG_WARNING1  ,
                                 _T("CPQMAnatomyDlg.cpp"), __LINE__); // Patni-KSS/ADDED/2010May19/IR-82


    } else {

        SetCurrentlySelectedSarRegion((SAR_REGION)sar_region);
    }
}


//+REDMINE-781
void CPQMAnatomyDlg::UpdateAnatomy(
    const SAR_REGION currently_selected_sar_region
)
{

    SetCurrentlySelectedSarRegion(currently_selected_sar_region);
    OnChangeAnatomySelection(currently_selected_sar_region);
}
//+REDMINE-781


//************************************Method Header************************************
// Method Name  : IsValidSarRegion
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
inline BOOL CPQMAnatomyDlg::IsValidSarRegion(
    const SAR_REGION sar_region
)const
{
    return ((sar_region >= SAR_REGION(ABDOMEN)) && sar_region < SAR_REGIONS) ;
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMAnatomyDlg::LoadImageForAnatomyButton(
    const SAR_REGION sar_region,
    const bool normal_image,
    const int sar_region_type/*= true*/
)
{
    if (!IsValidSarRegion(sar_region))
        return ;


    int normal_bitmap_index = 0, selected_bitmap_index = 0, bitmap_index_offset = 0;
    const UINT button_id = GetAnatomyButtonInfo(sar_region, normal_bitmap_index, selected_bitmap_index, bitmap_index_offset, sar_region_type);

    if (button_id == -1) {

        ASSERT(FALSE) ;
        return ;
    }

    const int bitmap_index_in_array = normal_image ? (sar_region - SAR_REGION(1) + bitmap_index_offset) : (SAR_REGIONS - SAR_REGION(1) + sar_region - SAR_REGION(1) + 2 * bitmap_index_offset);

    if (!(HBITMAP)m_anatomy_bitmap[bitmap_index_in_array].m_hObject)
        m_anatomy_bitmap[bitmap_index_in_array].LoadBitmap(normal_image ? normal_bitmap_index : selected_bitmap_index);

    CStatic* button = (CStatic*) GetDlgItem(button_id);
    button->SetBitmap((HBITMAP) m_anatomy_bitmap[bitmap_index_in_array]);
    //+Patni-PJS/2010Apr09/Added/JaFT# IR-137
    /*if (n_extremityCounter < 2) {
        if (sar_region == EXTREMITY) {

            n_extremityCounter++;
            LoadImageForAnatomyButton(sar_region, normal_image, 2);

            n_extremityCounter--;
        }
    }*/
    //-Patni-PJS/2010Apr09/Added/JaFT# IR-137
    n_extremityCounter = 1;
}



//************************************Method Header************************************
// Method Name  : OnChangeAnatomySelection
// Author       : PATNI/ HEMANT
// Purpose      : the sar region has changed.. do the processing.. needed..
//***********************************************************************************
void CPQMAnatomyDlg::OnChangeAnatomySelection(
    const SAR_REGION currently_selected_sar_region
)
{
    LPCTSTR FUNC_NAME = _T("CPQMAnatomyDlg::OnChangeAnatomySelection");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //the local function called.. which was called. in PQMStatic..
    StoreSelectedSarRegionInSelectedProtocol((int)currently_selected_sar_region);

    //+Patni-MN 2009Mar16/Added/PSP1#72
    BeginWaitCursor();

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_parent) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_parent pointer is NULL"));

    } else {
        //-Patni-DKH/2009Aug24/Modified/cpp test corrections

        m_parent->SetAnatomy(m_nItem_index , GetCurrentlySelectedSarRegion());
        m_parent->OnDialogClosedForParameterEditing(m_nItem_index, m_bPrev_auto_scan_flag_status);

    }//Patni-DKH/2009Aug24/Modified/cpp test corrections

    EndWaitCursor();
    //-Patni-MN 2009Mar16/Added/PSP1#72

    //now close the dialog..
    //+Patni-PJS/2010Aug17/Modified/REDMINE-562
    if (m_nFlags & (WF_MODALLOOP | WF_CONTINUEMODAL)) {
        EndModalLoop(IDOK);
    }

    //OnOK();	//it was also being done in pre code..
    //-Patni-PJS/2010Aug17/Modified/REDMINE-562
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
UINT CPQMAnatomyDlg::GetAnatomyButtonInfo(
    const SAR_REGION sar_region,
    int& index_normal,
    int& index_selected,
    int& bitmap_index_offset,
    const int sar_region_type /*= 1*/
)const
{
    if (!IsValidSarRegion(sar_region)) {
        return -1 ;
    }

    index_normal = -1;
    index_selected = -1;
    bitmap_index_offset = 0;

    UINT button_id = 0u;

    switch (sar_region) {

        case HEAD:
            button_id = IDC_STATIC_HEAD_BUTTON ;
            index_normal = IDB_BITMAP_S_HEAD;
            index_selected = IDB_BITMAP_B_HEAD;
            break;

        case C_SPINE:
            button_id = IDC_STATIC_CSPINE_BUTTON ;
            index_normal = IDB_BITMAP_S_CSPINE;
            index_selected = IDB_BITMAP_B_CSPINE;
            break;

        case TL_SPINE:
            button_id = IDC_STATIC_TLSPINE_BUTTON ;
            index_normal = IDB_BITMAP_S_TLSPINE;
            index_selected = IDB_BITMAP_B_TLSPINE;
            break;

        case CHEST:
            button_id = IDC_STATIC_CHEST_BUTTON ;
            index_normal = IDB_BITMAP_S_CHEST;
            index_selected = IDB_BITMAP_B_CHEST;
            break;

        case ABDOMEN:
            button_id = IDC_STATIC_ABDOMEN_BUTTON ;
            index_normal = IDB_BITMAP_S_ABDOMEN;
            index_selected = IDB_BITMAP_B_ABDOMEN;
            break;

        case PELVIS:
            button_id = IDC_STATIC_PELVIS_BUTTON ;
            index_normal = IDB_BITMAP_S_PELVIS;
            index_selected = IDB_BITMAP_B_PELVIS;
            break;

        case SHOULDER:
            button_id = IDC_STATIC_SHOULDER_BUTTON ;
            index_normal = IDB_BITMAP_S_SHOULDER;
            index_selected = IDB_BITMAP_B_SHOULDER;
            break;

        case EXTREMITY:

            if (sar_region_type == 1) {
                button_id = IDC_STATIC_EXTREMITY_BUTTON ;
                index_normal = IDB_BITMAP_S_EXTREMITY;
                index_selected = IDB_BITMAP_B_EXTREMITY;
                //+Patni-PJS/2010Apr09/Added/JaFT# IR-137
            } /*else if (sar_region_type == 2) {

                //case EXTREMITY2:

                bitmap_index_offset = 1;

                button_id = IDC_STATIC_EXTREMITY2_BUTTON ;
                index_normal = IDB_BITMAP_S_EXTREMITY2;
                index_selected = IDB_BITMAP_B_EXTREMITY2;
            }*/
            //-Patni-PJS/2010Apr09/Added/JaFT# IR-137
            break;

        case HAND:

            // we have encounter single extra type for ext
            bitmap_index_offset = 1;

            button_id = IDC_STATIC_HAND_BUTTON ;
            index_normal = IDB_BITMAP_S_HAND;
            index_selected = IDB_BITMAP_B_HAND;
            break;

        case ANKLE:

            // we have encounter single extra type for ext
            bitmap_index_offset = 1;

            button_id = IDC_STATIC_ANKLE_BUTTON ;
            index_normal = IDB_BITMAP_S_ANKLE;
            index_selected = IDB_BITMAP_B_ANKLE;
            break;

        default:
            return -1 ;

            break;
    }

    return button_id;
}


//************************************Method Header************************************
// Method Name  : SetBitmapsForStatics
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMAnatomyDlg::SetBitmapsForStatics(
)
{

    DWORD style = m_anatomy_chest.GetStyle();
    DWORD new_style = style | (SS_BITMAP | SS_NOTIFY | SS_CENTERIMAGE);

    m_anatomy_chest.ModifyStyle(style, new_style);
    m_anatomy_abdomen.ModifyStyle(style, new_style);
    m_anatomy_pelvis.ModifyStyle(style, new_style);
    m_anatomy_shoulder.ModifyStyle(style, new_style);
    m_anatomy_extremity.ModifyStyle(style, new_style);
    //Patni-PJS/2010Apr09/Added/JaFT# IR-137
    //m_anatomy_extremity2.ModifyStyle(style, new_style);
    m_anatomy_hand.ModifyStyle(style, new_style);
    m_anatomy_ankle.ModifyStyle(style, new_style);
    m_anatomy_head.ModifyStyle(style, new_style);
    m_anatomy_tlspine.ModifyStyle(style, new_style);
    m_anatomy_cspine.ModifyStyle(style, new_style);


    LoadImageForAnatomyButton(HEAD);
    LoadImageForAnatomyButton(C_SPINE);
    LoadImageForAnatomyButton(TL_SPINE);
    LoadImageForAnatomyButton(CHEST);
    LoadImageForAnatomyButton(ABDOMEN);
    LoadImageForAnatomyButton(PELVIS);
    LoadImageForAnatomyButton(SHOULDER);
    LoadImageForAnatomyButton(EXTREMITY);
    //LoadImageForAnatomyButton(EXTREMITY2);
    LoadImageForAnatomyButton(HAND);
    LoadImageForAnatomyButton(ANKLE);
}

//************************************Method Header************************************
// Method Name  : GetCurrentlySelectedSarRegion
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
inline SAR_REGION CPQMAnatomyDlg::GetCurrentlySelectedSarRegion(
)const
{

    return m_currently_selected_sar_region;
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMAnatomyDlg::SetCurrentlySelectedSarRegion(
    const SAR_REGION currently_selected_sar_region
)
{

    if (m_currently_selected_sar_region != currently_selected_sar_region) {

        LoadImageForAnatomyButton(m_currently_selected_sar_region);
        LoadImageForAnatomyButton(currently_selected_sar_region, false);

        m_currently_selected_sar_region = currently_selected_sar_region ;	//set it as current
    }
}

// Added By KT/ACE-2/11-06-2009/Multilingualisation
void CPQMAnatomyDlg::SetMultilingual()
{
    //dialog title
    const CString l_chTitle =  CPQMUtility::GetMultiLingualString(_T("IDS_ANATOMY_DLG_TITLE"));

    if (!l_chTitle.IsEmpty()) {
        CWnd ::SetWindowText(l_chTitle);
    }
}
// Added By KT/ACE-2/11-06-2009/Multilingualisation

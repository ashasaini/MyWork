//TMSC-REDMINE-783
//TMSC-REDMINE-782
//TMSC-REDMINE-781
//TMSC-REDMINE-780
//TMSC-REDMINE-778
//TMSC-REDMINE-777
//TMSC-REDMINE-774

// CoilSelWin.cpp : implementation file
//

#include "stdafx.h"
#include "CoilSelWin.h"
#include <tami/libAcq/SaturnCoilCombinChk.h> //Patni-MRP/2010APR26/Modified/COMMON_CDR_06
#include <libStudyManager/UI/ZOrderLayerFactory.h>
#include <PQM/CoilDefinitions.h>
#include "PQM.h"
#include "DPSManager.h"
#include "UIButtonUtility.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using MR::ACQ::SM::COMMON::UI::CZOrderDialog;
using MR::ACQ::SM::COMMON::UI::CZOrderLayerFactory;

#define RIBBON_GAP 5
#define ABSF(a) ((a) < 0.0 ? (-(a)) : (a))

//+Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement
const COLORREF g_normalchannel = RGB(255, 255, 255);
const COLORREF g_highlightchannel = RGB(255, 255, 0);

const COLORREF g_msgwnd_default_textcolor = RGB(255, 255, 255) ;
const COLORREF g_msgwnd_warning_textcolor = RGB(255, 120, 100);
const COLORREF g_msgwnd_default_bkcolor = RGB(13, 16, 26) ;

//-Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement
/////////////////////////////////////////////////////////////////////////////
// CCoilSelWin dialog
const int g_gui_section_gap = 1;

BEGIN_MESSAGE_MAP(CCoilSelWin, CZOrderDialog)
    //{{AFX_MSG_MAP(CCoilSelWin)
    ON_BN_CLICKED(IDC_BUTTON_INVOKE_COIL, OnButtonInvokeCoil)
    ON_BN_CLICKED(IDC_BUTTON_SCOIL_RIGHT, OnSpineCoilMoveRight)
    ON_BN_CLICKED(IDC_BUTTON_SCOIL_LEFT, OnSpineCoilMoveLeft)
    //+Patni-Hemant/2010Mar9/Commented/ACE-Phase#3/CDS Requirement
    ////Patni-AMT/2009Aug27/Added/ACE2 Spine Coil Req
    //ON_BN_CLICKED(IDC_BUTTON_SCOIL_MIDDLE, OnSpineCoilMoveMiddle)
    //-Patni-Hemant/2010Mar9/Commented/ACE-Phase#3/CDS Requirement

    ON_BN_CLICKED(IDC_BUTTON_FCOIL_RIGHT, OnFreeCoilMoveRight)
    ON_BN_CLICKED(IDC_BUTTON_FCOIL_LEFT, OnFreeCoilMoveLeft)
    ON_WM_CLOSE()
    //Button handlers
    ON_BN_CLICKED(IDC_BUTTON_GANTRY, OnGantry)
    ON_BN_CLICKED(IDC_BUTTON_COIL_CHANNEL_MODE, OnButtonClickCoilChannelMode)
    ON_COMMAND(ID_MENU_NORMAL, OnClickMenuItemNormal)
    ON_COMMAND(ID_MENU_MAX, OnClickMenuItemMax)

    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_CONTROL_RANGE(BN_CLICKED, IDC_BUTTON_L1,
                     IDC_BUTTON_L2, OnLinear)

    //+Patni-Hemant/2010Mar1/Modified/ACE-Phase#3/CDS Requirement
    //Patni-ARD/2009Sep25/Modified/Defect Fix MVC004910
    //    ON_CONTROL_RANGE(BN_CLICKED, IDC_BUTTON_SPINE_1,
    //                    IDC_BUTTON_SPINE_32, OnCouch)
    ON_CONTROL_RANGE(BN_CLICKED, IDC_BUTTON_SPINE_1,
                     IDC_BUTTON_SPINE_37, OnCouch)
    //-Patni-Hemant/2010Mar1/Modified/ACE-Phase#3/CDS Requirement
    //Patni-HAR/2009Nov24/DeFT/IR-110
    ON_CONTROL_RANGE(BN_CLICKED, IDC_BUTTON_SB1,
                     IDC_BUTTON_SB4, OnCouchSB)
    //+Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement
    ON_WM_PAINT()
    //-Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement
    ON_WM_SETCURSOR()

    //}}AFX_MSG_MAP

END_MESSAGE_MAP()


CCoilSelWin::CCoilSelWin(CCoilSelectionUtility* selection_utiliy,
                         CBaseAxisLocatorDialogInterface* cbaseaxislocatordialog_interface,
                         const UINT dialog_id,
                         CWnd* pParent /*= NULL*/,
                         const COUCH_POSITION couch_position /*= COUCH_POSITION_LEFT*/
                        )
    : CZOrderDialog(CZOrderLayerFactory::GetIselLayer(),
                    CDPSManager::GetInstance(), dialog_id, pParent),
    //Patni-PJS/2010Apr20/Added/JaFT# MVC007170_CDR_01
    m_couch_position(couch_position),
    m_store_visible_state_for_coil_list(false),
    m_cbaseaxislocatordialog_interface(cbaseaxislocatordialog_interface),
    m_p_dialog_coil_list(NULL),
    m_selection_utiliy(selection_utiliy),
    m_selected_channel_mode(-1),
    m_show_wait_cursor_for_coilselwin(false)
{
    //{{AFX_DATA_INIT(CCoilSelWin)
    //}}AFX_DATA_INIT

    //m_pstCoil = NULL;

    //m_pstSection = NULL;		/*/Section Information*/

    LPCTSTR FUNC_NAME = _T("CCoilSelWin::CCoilSelWin");
    PQM_TRACE_SCOPED(FUNC_NAME);


    //m_selection_utiliy = new CCoilSelectionUtility(this);


    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_selection_utiliy) {

        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));

    } else {
        //-Patni-DKH/2009Aug24/Modified/cpp test corrections
        m_selection_utiliy->SetUIClassInterfaceForCoilSelDialog(static_cast<CUIClassInterfaceForCoilSelDialog*>(this)) ;
    }//Patni-DKH/2009Aug24/Modified/cpp test corrections

    //  + Himanshu 24 Aug 2008
    //memset(&m_to_pqm_list_coil, 0, sizeof(VftCoilListInfo_t));
    //  - Himanshu 24 Aug 2008



    m_btnLabel[0].GetButtonColors(m_btnDefColor);

    //m_pstCoil = new CoilSelectionCoil_t[10];

    //Added by Hemant On 1/8/2009 5:45:41 PM
    //+Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement
    ResetHighlight();
    //m_highlight_1 = 3;
    //-Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CCoilSelWin::~CCoilSelWin(
)
{
    //+Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement
    HBITMAP l_old_handle = (HBITMAP)m_coil_main_image;

    if (l_old_handle)
        m_coil_main_image.DeleteObject();

    //-Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement
    Cleanup();
}


//+Patni-MSN 2009May-26/Un-Commented/PSP1#371
//+Patni-Manishkumar 2009April-22/Added/PSP1#371
//***************************Method Header*************************************
//Method Name    : ChangeViewOfCoilListDlg()
//Author         : PATNI / Manishkumar
//Purpose        : To disable/Enable the Coil list Dlg
//*****************************************************************************
void CCoilSelWin::ChangeViewOfCoilListDlg(const bool f_bCheck)const
{
    if (m_p_dialog_coil_list && ::IsWindow(m_p_dialog_coil_list->m_hWnd)) {
        m_p_dialog_coil_list->EnableWindow(f_bCheck);
    }
}
//-Patni-Manishkumar 2009April-22/Added/PSP1#371

//-Patni-MSN 2009May-26/Un-Commented/PSP1#371


//***************************Method Header*************************************
//Method Name    : DisplayCoil()
//Author         : PATNI /
//Purpose        :
//*****************************************************************************
void CCoilSelWin::DisplayCoil(
)
{
    if (NULL == GetSafeHwnd()) {
        return;
    }

    CreateCoilListDialog(false);
    DisplayButtonsOnAxisLocator();

    if (m_p_dialog_coil_list) {

        m_p_dialog_coil_list->DisplayCoilWithConnectCoilCheck();
    }

    EnableDisableDialogState(TRUE);

    return ;
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCoilSelWin::ShowHideOpenedWindowsOnPageChanged(
    const bool f_show /*= true*/
)
{
    if (m_p_dialog_coil_list) {

        if (::IsWindow(m_p_dialog_coil_list->m_hWnd)) {
            if (!f_show) {

                //Modified by Hemant On 3/5/2009 2:50:59 PM
                //m_store_visible_state_for_coil_list = m_p_dialog_coil_list->IsWindowVisible();
                m_store_visible_state_for_coil_list = m_p_dialog_coil_list->IsWindowVisible() ? true : false;
            }

            m_p_dialog_coil_list->ShowWindow((f_show && m_store_visible_state_for_coil_list) ? SW_SHOW : SW_HIDE);
        }
    }
}

//+Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : CoilSelSetCenterPos
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCoilSelWin::CoilSelSetCenterPos(
)
{

    //Note : InitUtilityFlags function called in OnInitDialog reset this flag
    //so no need to take care any where else.
    if (m_selection_utiliy) {

        m_selection_utiliy->SetCoilSelMode(COIL_SEL_CDS);

        ResetHighlight();

        if (m_cbaseaxislocatordialog_interface) {

            float l_highlited_channels = m_cbaseaxislocatordialog_interface->GetMagneticCenterPos() ;
            CoilSelectionUiMapCenterPos(l_highlited_channels) ;

            return ;
        }
    }

    ASSERT(FALSE);
}
//-Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : GetSelectedChannelMode
// Author       : PATNI/ Pavan
// Purpose      :
//***********************************************************************************
int CCoilSelWin::GetSelectedChannelMode() const
{
    return m_selected_channel_mode;
}

//************************************Method Header************************************
// Method Name  : SetSelectedChannelMode
// Author       : PATNI/ Pavan
// Purpose      :
//***********************************************************************************
void CCoilSelWin::SetSelectedChannelMode(const int f_mode)
{
    m_selected_channel_mode = f_mode;
}

void CCoilSelWin::EnableDisableDialogState(
    const BOOL f_state
)
{
    if (::IsWindow(m_hWnd)) {

        m_btnCancel.EnableWindow(f_state);
        m_btnOK.EnableWindow(f_state);
        ShowCloseButton(f_state == TRUE);

        if (m_p_dialog_coil_list) {
            m_p_dialog_coil_list->EnableDisableDialogState(f_state);
        }
    }
}

CCoilSelectionUtility* CCoilSelWin::GetSelectionUtility(
)const
{
    return m_selection_utiliy;
}

void CCoilSelWin::SetWindowCaption(
    const CString& f_window_caption,
    const bool f_applychange /*= false*/		//Added by Hemant On 12/5/2008 6:59:37 PM
)
{
    m_window_caption  = f_window_caption ;

    //Added by Hemant On 12/5/2008 6:59:22 PM
    if (f_applychange)
        SetWindowCaption() ;

}

void CCoilSelWin::DoDataExchange(CDataExchange* pDX)
{
    CZOrderDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CCoilSelWin)
    DDX_Control(pDX, IDC_EDIT_MSG, m_edtMsg);
    DDX_Control(pDX, IDC_BUTTON_L2, m_btnL[1]);
    DDX_Control(pDX, IDC_BUTTON_L1, m_btnL[0]);
    DDX_Control(pDX, IDC_BUTTON_GANTRY, m_btnGantry);
    DDX_Control(pDX, IDC_BUTTON_COIL_CHANNEL_MODE, m_btnCoilChannelMode);
    DDX_Control(pDX, IDOK, m_btnOK);
    DDX_Control(pDX, IDCANCEL, m_btnCancel);
    DDX_Control(pDX, IDC_BUTTON_SCOIL_RIGHT, m_btnSCRight);
    DDX_Control(pDX, IDC_BUTTON_SCOIL_LEFT, m_btnSCLeft);
    //+Patni-Hemant/2010Mar9/Commented/ACE-Phase#3/CDS Requirement
    ////Patni-HEMANT/ADDED On 8/27/2009 12:28:15 PM/ Spine Coil Req
    //DDX_Control(pDX, IDC_BUTTON_SCOIL_MIDDLE, m_btnSCMiddle);
    //-Patni-Hemant/2010Mar9/Commented/ACE-Phase#3/CDS Requirement

    DDX_Control(pDX, IDC_BUTTON_INVOKE_COIL, m_btnInvokeCoil);
    DDX_Control(pDX, IDC_BUTTON_FCOIL_RIGHT, m_btnFCRight);
    DDX_Control(pDX, IDC_BUTTON_FCOIL_LEFT, m_btnFCLeft);
    DDX_Control(pDX, IDC_BUTTON_LBL_A7, m_btnLabel[6]);
    DDX_Control(pDX, IDC_BUTTON_LBL_A6, m_btnLabel[5]);
    DDX_Control(pDX, IDC_BUTTON_LBL_A5, m_btnLabel[4]);
    DDX_Control(pDX, IDC_BUTTON_LBL_A4, m_btnLabel[3]);
    DDX_Control(pDX, IDC_BUTTON_LBL_A3, m_btnLabel[2]);
    DDX_Control(pDX, IDC_BUTTON_LBL_A2, m_btnLabel[1]);
    DDX_Control(pDX, IDC_BUTTON_LBL_A1, m_btnLabel[0]);
    DDX_Control(pDX, IDC_BUTTON_LBL_L3, m_btnLabel[8]);
    DDX_Control(pDX, IDC_BUTTON_LBL_L2, m_btnLabel[7]);
    DDX_Control(pDX, IDC_BUTTON_LBL_G,  m_btnLabel[9]);

    //+Patni-HAR/2009Nov24/DeFT/IR-110
    DDX_Control(pDX, IDC_BUTTON_SB4, m_btnCouch_sb[3]);
    DDX_Control(pDX, IDC_BUTTON_SB3, m_btnCouch_sb[2]);
    DDX_Control(pDX, IDC_BUTTON_SB2, m_btnCouch_sb[1]);
    DDX_Control(pDX, IDC_BUTTON_SB1, m_btnCouch_sb[0]);
    //-Patni-HAR/2009Nov24/DeFT/IR-110
    //+Patni-Hemant/2010Mar1/Added/ACE-Phase#3/CDS Requirement
    DDX_Control(pDX, IDC_BUTTON_SPINE_33, m_btnCouch[32]);
    DDX_Control(pDX, IDC_BUTTON_SPINE_34, m_btnCouch[33]);
    DDX_Control(pDX, IDC_BUTTON_SPINE_35, m_btnCouch[34]);
    DDX_Control(pDX, IDC_BUTTON_SPINE_36, m_btnCouch[35]);
    DDX_Control(pDX, IDC_BUTTON_SPINE_37, m_btnCouch[36]);
    //-Patni-Hemant/2010Mar1/Added/ACE-Phase#3/CDS Requirement
    //Patni-ARD/2009Sep25/Added/Defect Fix MVC004910
    DDX_Control(pDX, IDC_BUTTON_SPINE_32, m_btnCouch[31]);
    DDX_Control(pDX, IDC_BUTTON_SPINE_31, m_btnCouch[30]);
    DDX_Control(pDX, IDC_BUTTON_SPINE_30, m_btnCouch[29]);

    DDX_Control(pDX, IDC_BUTTON_SPINE_29, m_btnCouch[28]);
    DDX_Control(pDX, IDC_BUTTON_SPINE_28, m_btnCouch[27]);
    DDX_Control(pDX, IDC_BUTTON_SPINE_27, m_btnCouch[26]);
    DDX_Control(pDX, IDC_BUTTON_SPINE_26, m_btnCouch[25]);
    DDX_Control(pDX, IDC_BUTTON_SPINE_25, m_btnCouch[24]);
    DDX_Control(pDX, IDC_BUTTON_SPINE_24, m_btnCouch[23]);
    DDX_Control(pDX, IDC_BUTTON_SPINE_23, m_btnCouch[22]);
    DDX_Control(pDX, IDC_BUTTON_SPINE_22, m_btnCouch[21]);
    DDX_Control(pDX, IDC_BUTTON_SPINE_21, m_btnCouch[20]);
    DDX_Control(pDX, IDC_BUTTON_SPINE_20, m_btnCouch[19]);

    DDX_Control(pDX, IDC_BUTTON_SPINE_19, m_btnCouch[18]);
    DDX_Control(pDX, IDC_BUTTON_SPINE_18, m_btnCouch[17]);
    DDX_Control(pDX, IDC_BUTTON_SPINE_17, m_btnCouch[16]);
    DDX_Control(pDX, IDC_BUTTON_SPINE_16, m_btnCouch[15]);
    DDX_Control(pDX, IDC_BUTTON_SPINE_15, m_btnCouch[14]);
    DDX_Control(pDX, IDC_BUTTON_SPINE_14, m_btnCouch[13]);
    DDX_Control(pDX, IDC_BUTTON_SPINE_13, m_btnCouch[12]);
    DDX_Control(pDX, IDC_BUTTON_SPINE_12, m_btnCouch[11]);
    DDX_Control(pDX, IDC_BUTTON_SPINE_11, m_btnCouch[10]);
    DDX_Control(pDX, IDC_BUTTON_SPINE_10, m_btnCouch[9]);

    DDX_Control(pDX, IDC_BUTTON_SPINE_9, m_btnCouch[8]);
    DDX_Control(pDX, IDC_BUTTON_SPINE_8, m_btnCouch[7]);
    DDX_Control(pDX, IDC_BUTTON_SPINE_7, m_btnCouch[6]);
    DDX_Control(pDX, IDC_BUTTON_SPINE_6, m_btnCouch[5]);
    DDX_Control(pDX, IDC_BUTTON_SPINE_5, m_btnCouch[4]);
    DDX_Control(pDX, IDC_BUTTON_SPINE_4, m_btnCouch[3]);
    DDX_Control(pDX, IDC_BUTTON_SPINE_3, m_btnCouch[2]);
    DDX_Control(pDX, IDC_BUTTON_SPINE_2, m_btnCouch[1]);
    DDX_Control(pDX, IDC_BUTTON_SPINE_1, m_btnCouch[0]);

    //+Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement
    DDX_Control(pDX, IDC_AXISLOCATOR, m_main_image);
    DDX_Control(pDX, IDC_STATIC_CHANNEL_MODE, m_channel_mode);
    //-Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement
    //}}AFX_DATA_MAP
}

//***************************Method Header*************************************
//Method Name    : OnOK()
//Author         : PATNI /
//Purpose        :
//*****************************************************************************
void CCoilSelWin::OnOK(
)
{
    // TODO: Add extra validation here
    LPCTSTR FUNC_NAME = _T("CCoilSelWin::OnOK");
    PQM_TRACE_SCOPED(FUNC_NAME);


    //CheckSelectCoil
    if (!UiCheckSelectCoil()) {
        return;
    }

    //Pqm coil choose to send information to set
    m_selection_utiliy->UiSetToPqmSelectCoil();

    //Added by Hemant On 11/13/2008 4:56:04 PM
    m_cbaseaxislocatordialog_interface->ApplySelectedCoils();

    DestroyWindow();

    m_cbaseaxislocatordialog_interface->OnAxisLocatorDialogClosed();
}

//***************************Method Header*************************************
//Method Name    : OnCancel()
//Author         : PATNI /
//Purpose        :
//*****************************************************************************
void CCoilSelWin::OnCancel(
)
{
    // TODO: Add extra cleanup here

    LPCTSTR FUNC_NAME = _T("CCoilSelWin::OnCancel");
    PQM_TRACE_SCOPED(FUNC_NAME);


    DestroyWindow();

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_cbaseaxislocatordialog_interface) {

        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("m_cbaseaxislocatordialog_interface pointer is NULL"));

    } else {
        //-Patni-DKH/2009Aug24/Modified/cpp test corrections
        m_cbaseaxislocatordialog_interface->OnAxisLocatorDialogClosed(false);
    }//Patni-DKH/2009Aug24/Modified/cpp test corrections
}


//***************************Method Header*************************************
//Method Name    : OnClose()
//Author         : PATNI /
//Purpose        :
//*****************************************************************************
void CCoilSelWin::OnClose(
)
{

    // TODO: Add your message handler code here
    //Cleanup();

    OnCancel();

    //DialogEx::OnClose();
}


//***************************Method Header*************************************
//Method Name    : OnButtonInvokeCoil()
//Author         : PATNI /
//Purpose        :
//*****************************************************************************
void CCoilSelWin::OnButtonInvokeCoil(
)
{
    // TODO: Add your control notification handler code here
    CreateCoilListDialog(true);
    m_p_dialog_coil_list->EnableDisableDialogState(m_btnOK.IsWindowEnabled());

    if (m_p_dialog_coil_list) {
        m_p_dialog_coil_list->SetWindowCaption(m_window_caption) ;
    }
}

//***************************Method Header*************************************
//Method Name    : OnSpineCoilMoveLeft()
//Author         : PATNI / AMT
//Purpose        : Moves Spine coils to position 3
//*****************************************************************************
void CCoilSelWin::OnSpineCoilMoveLeft(
)
{
    //+Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement
    MoveSpineCoils(CoilMove_Left);
    //-Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement

    return ;
}


//***************************Method Header*************************************
//Method Name    : OnSpineCoilMoveRight()
//Author         : PATNI / AMT
//Purpose        : Moves Spine coils to position 6
//*****************************************************************************
void CCoilSelWin::OnSpineCoilMoveRight(
)
{
    //+Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement
    MoveSpineCoils(CoilMove_Right);
    //-Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement

    return ;
}

//***************************Method Header*************************************
//Method Name    : OnFreeCoilMoveLeft()
//Author         : PATNI / AMT
//Purpose        : Moves Free coil to one position left
//*****************************************************************************
void CCoilSelWin::OnFreeCoilMoveLeft(
)
{
    MoveFreeCoils(CoilMove_Left);

    return;
}

//***************************Method Header*************************************
//Method Name    : OnFreeCoilMoveRight()
//Author         : PATNI / AMT
//Purpose        : Moves Free coils one position right
//*****************************************************************************
void CCoilSelWin::OnFreeCoilMoveRight(
)
{
    MoveFreeCoils(CoilMove_Right);

    return ;
}

//+Patni-PJS/2009May27/ADDED/IDS#472
//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ PANKAJ SHARMA
// Purpose      :
//***********************************************************************************
void CCoilSelWin::OnDestroyImpl()
{
    LPCTSTR FUNC_NAME = _T("CCoilSelWin::OnDestroyImpl");
    PQM_TRACE_SCOPED(FUNC_NAME);

    if (m_p_dialog_coil_list) {
        m_p_dialog_coil_list->DestroyWindow();
    }

    CZOrderDialog::OnDestroyImpl();
}
//-Patni-PJS/2009May27/ADDED/IDS#472
//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ Pavan
// Purpose      : To display channel Mode menu
//***********************************************************************************
void CCoilSelWin::OnButtonClickCoilChannelMode()
{
    LPCTSTR FUNC_NAME = _T("CCoilSelWin::OnButtonClickCoilChannelMode");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //CreateChannelModeMenu();

    CRect rect_for_utility_button ;
    m_btnCoilChannelMode.GetWindowRect(&rect_for_utility_button);

    PopupMenuEx context_menu;
    context_menu.LoadMenu(IDR_COIL_CHANNEL_MODE);

    LPCTSTR font_name = _T("Verdana") ;
    const int font_size = 13;

    context_menu.SetFont(font_name, font_size);

    context_menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, rect_for_utility_button.left,
                                rect_for_utility_button.bottom, this, 0, 0);

}
//************************************Method Header************************************
// Method Name  : OnClickMenuItemNormal()
// Author       : PATNI/ Pavan
// Purpose      : To display channel Mode menu
//***********************************************************************************
void CCoilSelWin::OnClickMenuItemNormal()
{
    LPCTSTR FUNC_NAME = _T("CCoilSelWin::OnClickMenuItemNormal");
    PQM_TRACE_SCOPED(FUNC_NAME);
    TCHAR l_data[MAX_PATH] = {0};

    CString l_mode = CPQMUtility::GetMultiLingualString(_T("IDS_CHANNEL_MODE_NORMAL"));
    SetDlgItemText(IDC_BUTTON_COIL_CHANNEL_MODE, l_mode);
    SetSelectedChannelMode(CHANNEL_MODE_NORMAL);

}
//************************************Method Header************************************
// Method Name  : OnClickMenuItemMax()
// Author       : PATNI/ Pavan
// Purpose      : To display channel Mode menu
//***********************************************************************************
void CCoilSelWin::OnClickMenuItemMax()
{
    LPCTSTR FUNC_NAME = _T("CCoilSelWin::OnClickMenuItemMax");
    PQM_TRACE_SCOPED(FUNC_NAME);

    CString l_mode = CPQMUtility::GetMultiLingualString(_T("IDS_CHANNEL_MODE_MAX"));

    SetDlgItemText(IDC_BUTTON_COIL_CHANNEL_MODE, l_mode);
    SetSelectedChannelMode(CHANNEL_MODE_MAX);
    //m_selected_channel_mode = 1;
    // TODO:: Coil channels will be system enable channel
}
//***************************Method Header*************************************
//Method Name    : OnGantry()
//Author         : PATNI /
//Purpose        :
//*****************************************************************************
void CCoilSelWin::OnGantry(
)
{
    LPCTSTR FUNC_NAME = _T("CCoilSelWin::OnGantry");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_selection_utiliy) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));

        return;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    CoilSelectionSectionButton_s* l_coil_button = m_selection_utiliy->GetCoilButton(GetBaseIndexForCoilButtonFor(CoilType_G));

    if (l_coil_button->position.sw == FALSE) {
        /* OFF->ON */
        l_coil_button->position.sw = TRUE;

    } else {
        /* ON->OFF */
        l_coil_button->position.sw = FALSE;
        UpdateMsgWindowWith(IDS_CS_MES_SELECT_SECTION);
    }

    SetEnableDisableButtonColor(m_btnGantry, l_coil_button->position.sw ? true : false);
    SetEnableDisableButtonColor(m_btnLabel[9], l_coil_button->position.sw ? true : false);

    UiCheckCombine();       // always call at last/..
}


//***************************Method Header*************************************
//Method Name    : OnInitDialog()
//Author         : PATNI /
//Purpose        :
//*****************************************************************************
BOOL CCoilSelWin::OnInitDialogImpl()
{
    LPCTSTR FUNC_NAME = _T("CCoilSelWin::OnInitDialog");
    PQM_TRACE_SCOPED(FUNC_NAME);

    SetMultilingual(); // Added By KT/ACE-2/11-06-2009/Multilingualisation

    SetButtonStyle();

    //+Patni-Hemant/2010Mar26/Added/ACE-Phase#3/CDS Requirement
    ResetHighlight();
    //-Patni-Hemant/2010Mar26/Added/ACE-Phase#3/CDS Requirement

    //+Patni-Hemant/2010Feb23/Modified/ACE-Phase#3/CDS Requirement
    //    COLORREF l_normal_color = RGB(13, 16, 26) ;
    //    COLORREF l_text_color = RGB(255, 255, 255) ;
    const COLORREF l_normal_color = g_msgwnd_default_bkcolor ;
    const COLORREF l_text_color = g_msgwnd_default_textcolor ;
    //-Patni-Hemant/2010Feb23/Modified/ACE-Phase#3/CDS Requirement

    m_edtMsg.SetColorData(l_normal_color , l_text_color);
    m_edtMsg.SetFontAttributes(_T("Arial"), 100);
    m_edtMsg.UpdateFont();

    SetDialogRibbonHeight();

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_selection_utiliy) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));

    } else {
        //-Patni-DKH/2009Aug24/Modified/cpp test corrections
        m_selection_utiliy->InitUtilityFlags();
    }//Patni-DKH/2009Aug24/Modified/cpp test corrections


    CRect l_window_rect;
    GetWindowRect(&l_window_rect) ;
    CPoint l_actual_topleft(1070, 556) ;

    l_window_rect.OffsetRect(l_actual_topleft - l_window_rect.TopLeft()) ;
    SetWindowPos(NULL, l_window_rect.left, l_window_rect.top, l_window_rect.Width() , l_window_rect.Height() , SWP_NOSIZE | SWP_NOZORDER) ;

    UisetCoilButtonPosition();

    SetTitleBarHeight(24) ;
    SetFont(_T("Arial"), 10) ;

    SetWindowCaption();
    //+Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement
    const int l_main_bitmap = (m_couch_position == COUCH_POSITION_LEFT) ? IDB_BITMAP_AXISLOCATOR_LEFT : IDB_BITMAP_AXISLOCATOR_RIGHT;

    if (NULL == m_p_dialog_coil_list) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_p_dialog_coil_list is NULL"));
    }

    if (!(m_coil_main_image.m_hObject)) {

        //+Patni-Ravindra Acharya/2010May26/Added/IR-147
        CString l_path = GetBitmapPath(l_main_bitmap) ;

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_path);

        if (l_path.IsEmpty()) {
            return TRUE ;
        }

        HBITMAP hBitmap = (HBITMAP)::LoadImage(NULL, l_path, IMAGE_BITMAP,
                                               0, 0, LR_LOADFROMFILE);

        if (NULL == hBitmap) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Coil Image is Not Loaded Properly."));
            return TRUE ;
        }

        m_coil_main_image.m_hObject = hBitmap ;

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Coil Image is Already Loaded."));
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Coil Image is Available Now."));

    m_main_image.ShowWindow(SW_HIDE);
    GetDlgItem(IDC_NUMBER_1)->ShowWindow(SW_HIDE);
    //-Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement

    EnableDisableDialogState(FALSE);
    m_channel_mode.SetColorData(l_normal_color , l_text_color);

    m_channel_mode.SetFontAttributes(_T("Verdana"), 130);
    m_channel_mode.SetTextColor(RGB(102, 139, 200));
    m_channel_mode.UpdateFont();

    InitializeChannelMode();
    CRect wnd_rect;
    GetWindowRect(&wnd_rect);
    CRect target_rect(0, 0, 0, 0);
    target_rect.right = target_rect.left + wnd_rect.Width();
    target_rect.bottom = target_rect.top + wnd_rect.Height();
    InvalidateRect(&target_rect, TRUE);
    UpdateWindow();

    //return CZOrderDialog::OnInitDialogImpl();
    return TRUE;
    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


//***************************Method Header*************************************
//Method Name    : OnCouch()
//Author         : PATNI /
//Purpose        :
//*****************************************************************************
void CCoilSelWin::OnCouch(
    UINT uID
)
{
    LPCTSTR FUNC_NAME = _T("CCoilSelWin::OnCouch");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_selection_utiliy) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));

        return;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    const int button_index = uID - IDC_BUTTON_SPINE_1 ;
    CoilSelectionSectionButton_t* l_section_button = m_selection_utiliy->GetSectionButton(button_index);

    SectionCoilButtonUiOnCoilPB(button_index, true);

    /* Check Combine */
    UiCheckCombine();


    /* linkflgを無効に */

    //CoilSelectionSectionButton_t *l_section_button = NULL;
    for (int i = 0; i < MAX_SECTION_BUTTON; i++) {
        l_section_button = m_selection_utiliy->GetSectionButton(i);

        if (l_section_button)
            l_section_button->position.linkflg = FALSE;
    }
}

//+Patni-HAR/2009Nov24/DeFT/IR-110
//***************************Method Header*************************************
//Method Name    : OnCouchSB()
//Author         : PATNI / HAR
//Purpose        : Handles events for Sentinal coils/QD Head coil
//*****************************************************************************
void CCoilSelWin::OnCouchSB(
    UINT uID
)
{
    LPCTSTR FUNC_NAME = _T("CCoilSelWin::OnCouchSB");
    PQM_TRACE_SCOPED(FUNC_NAME);

    if (NULL == m_selection_utiliy) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));
        return;
    }

    const int l_lbutton_index = (uID - IDC_BUTTON_SB1) ;

    const int button_index = l_lbutton_index + GetBaseIndexForCoilButtonFor(CoilType_A) ;

    CoilSelectionSectionButton_s* l_coil_button = m_selection_utiliy->GetCoilButton(button_index);

    if (l_coil_button->position.sw == FALSE) {
        /* OFF->ON */
        l_coil_button->position.sw = TRUE;

    } else {
        /* ON->OFF */
        l_coil_button->position.sw = FALSE;
        UpdateMsgWindowWith(IDS_CS_MES_SELECT_SECTION);
    }

    SetButtonAppearenceForACoilButton((ACoilItems)l_lbutton_index) ;
    UiCheckCombine();       // always call at last/..
}
//-Patni-HAR/2009Nov24/DeFT/IR-110

//***************************Method Header*************************************
//Method Name    : OnLinear()
//Author         : PATNI /
//Purpose        :
//*****************************************************************************
void CCoilSelWin::OnLinear(
    UINT uID
)
{
    LPCTSTR FUNC_NAME = _T("CCoilSelWin::OnLinear");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_selection_utiliy) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));

        return;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    const int l_lbutton_index = (uID - IDC_BUTTON_L1) ;
    CoilSelectionSectionButton_s* l_coil_button = m_selection_utiliy->GetCoilButton(
                l_lbutton_index + GetBaseIndexForCoilButtonFor(CoilType_L)
            );

    if (l_coil_button->position.sw == FALSE) {
        /* OFF->ON */
        l_coil_button->position.sw = TRUE;

    } else {
        /* ON->OFF */
        l_coil_button->position.sw = FALSE;
        UpdateMsgWindowWith(IDS_CS_MES_SELECT_SECTION);
    }

    //l_coil_button->position.sw = !l_coil_button->position.sw;

    SetButtonAppearenceForLCoilButton((LCoilItems)l_lbutton_index) ;

    UiCheckCombine();       // always call at last/..
}



void CCoilSelWin::OnLButtonDown(
    UINT flags,
    CPoint point
)
{
    LPCTSTR FUNC_NAME = _T("CCoilSelWin::OnLButtonDown");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_selection_utiliy) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));

        return;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    CZOrderDialog::OnLButtonDown(flags, point) ;

    m_selection_utiliy->SetPressPoint(point) ;
    SetCapture();

    //state = event->xbutton.state;


}

void CCoilSelWin::OnLButtonUp(
    UINT flags,
    CPoint point
)
{

    LPCTSTR FUNC_NAME = _T("CCoilSelWin::OnLButtonUp");
    PQM_TRACE_SCOPED(FUNC_NAME);

    ReleaseCapture();
    CZOrderDialog::OnLButtonUp(flags, point) ;

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_selection_utiliy) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));

        return;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    CPoint press_point = m_selection_utiliy->GetPressPoint();

    if (press_point.x == 0 || press_point.y == 0) {

        m_selection_utiliy->SetPressPoint(CPoint(0, 0)) ;
        m_selection_utiliy->SetReleasePoint(CPoint(0, 0)) ;

        return ;
    }

    m_selection_utiliy->SetReleasePoint(point) ;

    /* Judg */
    JudgCoilPB(true);

    /* Check Combine */
    UiCheckCombine();

    /* linkflgを無効に */
    CoilSelectionSectionButton_t* section_buton_n = NULL;

    for (int i = 0; i < MAX_SECTION_BUTTON; i++) {

        //CoilSelectionSectionButton[i].position.linkflg = False;

        section_buton_n = m_selection_utiliy->GetSectionButton(i);
        section_buton_n->position.linkflg = FALSE;
    }

    m_selection_utiliy->SetPressPoint(CPoint(0, 0)) ;
    m_selection_utiliy->SetReleasePoint(CPoint(0, 0)) ;
    m_selection_utiliy->SetBeforePoint(CPoint(0, 0)) ;

}

void CCoilSelWin::OnMouseMove(
    UINT nFlags,
    CPoint point
)
{
    CZOrderDialog::OnMouseMove(nFlags, point) ;


    LPCTSTR FUNC_NAME = _T("CCoilSelWin::OnMouseMove");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_selection_utiliy) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));

        return;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    CPoint press_point = m_selection_utiliy->GetPressPoint();

    if (!((nFlags & MK_LBUTTON) == MK_LBUTTON)) {

        return ;
    }

    if (press_point.x == 0 || press_point.y == 0) {

        m_selection_utiliy->SetReleasePoint(CPoint(0, 0)) ;
        return ;
    }

    m_selection_utiliy->SetReleasePoint(point) ;

    /* Judg */
    JudgCoilPB(false);
}

//+Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCoilSelWin::OnPaint()
{
    LPCTSTR FUNC_NAME = _T("CCoilSelWin::OnPaint");

    CPaintDC l_paintdc(this); // device context for painting

    const int bk_mode = l_paintdc.SetBkMode(TRANSPARENT);
    const COLORREF old_color = l_paintdc.SetTextColor(g_normalchannel);

    CDC l_memdc;
    l_memdc.CreateCompatibleDC(&l_paintdc);
    CBitmap* l_oldbmp = l_memdc.SelectObject(&m_coil_main_image);

    if (NULL == l_oldbmp) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_oldbmp is NULL"));
    }

    CRect l_rect = GetMainImageRect();

    if (TRUE != l_paintdc.BitBlt(l_rect.left, l_rect.top, l_rect.Width(), l_rect.Height(), &l_memdc, 0, 0, SRCCOPY)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("BitBlt is false"));
    }

    l_memdc.SelectObject(l_oldbmp);
    l_memdc.DeleteDC();

    const int l_multiplier = (m_couch_position == COUCH_POSITION_LEFT) ? 1 : -1 ;
    GetDlgItem(IDC_NUMBER_1)->GetWindowRect(&l_rect);
    ScreenToClient(&l_rect);

    l_rect.OffsetRect(1 * l_multiplier, 0);

    CString str ;

    for (int i = 0 ; i < 15 ; i++) {

        str.Format(_T("%d"), (i + 1));

        if (IsHighlighted(i + 1)) {

            l_paintdc.SetTextColor(g_highlightchannel);
        }

        l_paintdc.DrawText(str, &l_rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);

        l_rect.OffsetRect(((l_rect.Width()) * l_multiplier) + (1 * l_multiplier) , 0);

        l_paintdc.SetTextColor(g_normalchannel);
    }

    l_paintdc.SetTextColor(old_color);
    l_paintdc.SetBkMode(bk_mode);
}
//-Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement

//************************************Method Header************************************
// Method Name  : SetWaitCursor
// Author       : iGate
// Purpose      :
//***********************************************************************************

BOOL CCoilSelWin::OnSetCursor(CWnd* pWnd,
                              UINT nHitTest,
                              UINT message)
{
    if (m_show_wait_cursor_for_coilselwin) {
        ::SetCursor(::LoadCursor(NULL, IDC_WAIT));
        return TRUE ;
    }

    return CDialog::OnSetCursor(pWnd, nHitTest, message);

}

BOOL CCoilSelWin::PreTranslateMessageImpl(
    MSG* pMsg
)
{
    if (m_show_wait_cursor_for_coilselwin) {
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

    if (pMsg->message == WM_LBUTTONDOWN) {

        if (pMsg->hwnd != m_hWnd) {

            for (int i = 0; i < MAX_SECTION_BUTTON; i++) {

                if (m_btnCouch[i].m_hWnd == pMsg->hwnd) {

                    CWnd* window =  CWnd::FromHandle(pMsg->hwnd);

                    CPoint point = (CPoint) pMsg->lParam ;
                    window->ClientToScreen(&point);
                    ScreenToClient(&point);

                    //SendMessage(WM_LBUTTONDOWN, pMsg->wParam, (LPARAM) (long)(point));
                    OnLButtonDown(pMsg->wParam, point);
                    return TRUE ;
                }
            }
        }

    }

    if (pMsg->message == WM_LBUTTONUP) {

        if (pMsg->hwnd != m_hWnd) {

            for (int i = 0; i < MAX_SECTION_BUTTON; i++) {

                if (m_btnCouch[i].m_hWnd == pMsg->hwnd) {

                    CWnd* window =  CWnd::FromHandle(pMsg->hwnd);

                    CPoint point = (CPoint) pMsg->lParam ;
                    window->ClientToScreen(&point);
                    ScreenToClient(&point);

                    //SendMessage(WM_LBUTTONDOWN, pMsg->wParam, (LPARAM) (long)(point));
                    OnLButtonUp(pMsg->wParam, point);
                    return TRUE ;
                }
            }
        }
    }

    return CZOrderDialog::PreTranslateMessageImpl(pMsg);
}



//+Patni-Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : UpdateWarningMsgWindowWith
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCoilSelWin::UpdateWarningMsgWindowWith(
    const int f_msgstring_id
)
{
    const CString l_msgstring = GetStringForID(f_msgstring_id);

    m_selection_utiliy->GetCoilSelectionGlobal()->message = f_msgstring_id;
    //Logic for changing the color..

    m_edtMsg.SetTextColor(g_msgwnd_warning_textcolor);
    m_edtMsg.UpdateFont();

    m_edtMsg.SetWindowText(l_msgstring) ;
}

//-Patni-Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement


//+Patni-Hemant/2010Mar4/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : UpdateMsgWindowWithMsgOK
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCoilSelWin::UpdateMsgWindowWithMsgOK(
)
{
    if (!m_selection_utiliy) {
        return ;
    }

    if (m_selection_utiliy->GetCoilSelMode() == COIL_SEL_CDS) {
        UpdateMsgWindowWith(IDS_CDS_MES_AFTER_CDS);

    } else {
        UpdateMsgWindowWith(IDS_CS_MES_OK);
    }
}
//-Patni-Hemant/2010Mar4/Added/ACE-Phase#3/CDS Requirement


//***************************Method Header*************************************
//Method Name    : UiCheckCombine()
//Author         : PATNI / HAR + AMT
//Purpose        :
//*****************************************************************************
void CCoilSelWin::UiCheckCombine(
)
{
    LPCTSTR FUNC_NAME = _T("CCoilSelWin::UiCheckCombine");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_selection_utiliy) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));

        return;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    CoilSelectionSectionButton_t* l_section_button  = NULL;

    for (int i = 0; i < MAX_SECTION_BUTTON; i++) {

        l_section_button = m_selection_utiliy->GetSectionButton(i);

        if (l_section_button->section) {

            if (l_section_button->section->enable[l_section_button->port] == TRUE &&
                (l_section_button->coil->combine & COMBINE_WB_MASK) ==
                COMBINE_SWITCH_SELECT) {

                if (UiCheckSectionCombine(i)) {
                    //PENDING
                    //CoilSelectionUiSetWarningColor(l_section_button->w);
                    //Added by Hemant On 11/8/2008 3:42:49 PM , to overcome the pending..
                    ButtonEx* l_button_to_process = GetCoilButtonForIndex(i) ;

                    if (l_button_to_process)
                        SetWarningButtonColor(*l_button_to_process);

                    //CoilSelectionUiMessage("MES_COMBINE");
                    //+Patni-Hemant/2010Feb12/Modified/ACE-Phase#3/CDS Requirement
                    //UpdateMsgWindowWith(IDS_CS_MES_COMBINE);
                    UpdateWarningMsgWindowWith(IDS_CS_MES_COMBINE);
                    //-Patni-Hemant/2010Feb12/Modified/ACE-Phase#3/CDS Requirement

                } else {
                    //PENDING

                    //CoilSelectionUiSetSelectColor(l_section_button->w);
                    ButtonEx* l_button_to_process = GetCoilButtonForIndex(i) ;

                    if (l_button_to_process)
                        SetEnableDisableButtonColor(*l_button_to_process);

                    //CoilSelectionUiMessage("MES_OK");
                    //+Patni-Hemant/2010Feb12/Modified/ACE-Phase#3/CDS Requirement
                    //UpdateMsgWindowWith(IDS_CS_MES_OK);
                    UpdateMsgWindowWithMsgOK();
                    //-Patni-Hemant/2010Feb12/Modified/ACE-Phase#3/CDS Requirement
                }
            }
        }
    }

    CoilSelectionSectionButton_t* l_coil_button     = NULL;

    for (i = 0; i < MAX_COIL_BUTTON; i++) {

        l_coil_button = m_selection_utiliy->GetCoilButton(i);

        if (l_coil_button->position.sw == TRUE &&
            l_coil_button->coil != NULL &&
            (l_coil_button->coil->combine & COMBINE_WB_MASK) ==
            COMBINE_SWITCH_SELECT) {

            if (UiCheckCoilCombine(i)) {


                //CoilSelectionUiSetWarningColor(l_coil_button->w);
                CoilSelectionUiSetWarningColor(i);

                //CoilSelectionUiMessage("MES_COMBINE");
                //+Patni-Hemant/2010Feb12/Modified/ACE-Phase#3/CDS Requirement
                //UpdateMsgWindowWith(IDS_CS_MES_COMBINE);
                UpdateWarningMsgWindowWith(IDS_CS_MES_COMBINE);
                //-Patni-Hemant/2010Feb12/Modified/ACE-Phase#3/CDS Requirement

            } else {


                //CoilSelectionUiSetSelectColor(CoilSelectionCoilButton[i].w);
                ButtonEx* l_button_to_process = GetCoilButtonForIndex(i) ;

                if (l_button_to_process)
                    SetEnableDisableButtonColor(*l_button_to_process);

                //CoilSelectionUiMessage("MES_OK");
                //+Patni-Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement
                //UpdateMsgWindowWith(IDS_CS_MES_OK);
                UpdateMsgWindowWithMsgOK();
                //-Patni-Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement
            }
        }
    }

    UiChangeMessage();
}


void CCoilSelWin::CoilSelectionUiMessage(
    const int f_message_id
)
{

    UpdateMsgWindowWith(f_message_id);
}


void CCoilSelWin::SelectCoilUiSetSelectColorForIndex(
    const int f_coil_buttonindex
)
{

    ButtonEx* l_button_to_process = GetCoilButtonForIndex(f_coil_buttonindex) ;

    if (l_button_to_process)
        SetEnableDisableButtonColor(*l_button_to_process);

}

void CCoilSelWin::SelectCoilUiSetDefaultColorForIndex(
    const int f_coil_buttonindex
)
{

    ButtonEx* l_button_to_process = GetCoilButtonForIndex(f_coil_buttonindex) ;

    if (l_button_to_process)
        SetEnableDisableButtonColor(*l_button_to_process, false);
}

void CCoilSelWin::SelectCoilSetWarningButtonColorForIndex(
    const int f_coil_buttonindex
)
{

    CoilSelectionUiSetWarningColor(f_coil_buttonindex) ;
}


void CCoilSelWin::SelectCoilUiSetLableForSelectionCoilIndex(
    const int f_coil_buttonindex,
    const CString& f_lable
)
{

    ButtonEx* l_button_to_process = GetCoilButtonForIndex(f_coil_buttonindex) ;

    if (l_button_to_process)
        l_button_to_process->SetWindowText(f_lable);
}

void CCoilSelWin::SelectCoilUiSetSensitiveButton(
    const int f_coil_buttonindex,
    const bool f_enable
)
{

    ButtonEx* l_button_to_process = GetCoilButtonForIndex(f_coil_buttonindex) ;

    if (l_button_to_process)
        l_button_to_process->ShowWindow(f_enable ? SW_SHOW : SW_HIDE);

}


void CCoilSelWin::SelectCoilEnableDisableButton(
    const int f_select_buttonindex,
    const bool f_enable
)
{

    ButtonEx* l_button_to_process = GetCoilButtonForIndex(f_select_buttonindex) ;

    if (l_button_to_process)
        l_button_to_process->EnableWindow(f_enable ? SW_SHOW : SW_HIDE);
}

//+Patni-Hemant/2009Dec25/Modified/MVC006387
void CCoilSelWin::SectionCoilSetSelectColorForButton(
    const int f_coil_buttonindex,
    const bool f_redraw_button /*= false*/
)
{

    ButtonEx* l_button_to_process = GetSectionButtonForIndex(f_coil_buttonindex) ;

    if (l_button_to_process) {
        SetEnableDisableButtonColor(*l_button_to_process, true, f_redraw_button);
    }

}

void CCoilSelWin::SectionCoilSetDefaultColorForButton(
    const int f_section_buttonindex,
    const bool f_redraw_button /*= false*/
)
{

    ButtonEx* l_button_to_process = GetSectionButtonForIndex(f_section_buttonindex) ;

    if (l_button_to_process) {
        SetEnableDisableButtonColor(*l_button_to_process, false, f_redraw_button);
    }
}
//-Patni-Hemant/2009Dec25/Modified/MVC006387

//***************************Method Header*************************************
//Method Name    : DisplayButtonsOnAxisLocator()
//Author         : PATNI /
//Purpose        :
//*****************************************************************************
void CCoilSelWin::DisplayButtonsOnAxisLocator(
)
{
    return ;
}


void CCoilSelWin::SectionCoilSetWarningButtonColor(
    const int f_section_buttonindex
)
{
    ButtonEx* l_button_to_process = GetSectionButtonForIndex(f_section_buttonindex) ;

    if (l_button_to_process) {

        SetWarningButtonColor(*l_button_to_process);
        return ;
    }
}

void CCoilSelWin::SectionCoilSetLableForButton(
    const int f_section_buttonindex,
    const CString& f_lable
)
{

    ButtonEx* l_button_to_process = GetSectionButtonForIndex(f_section_buttonindex) ;

    if (l_button_to_process)
        l_button_to_process->SetWindowText(f_lable);
}

void CCoilSelWin::SectionCoilUpdateSectionSize(
    const int f_section_buttonindex,
    CoilSelectionSectionButton_t* f_section_btn
)
{
    if (NULL == f_section_btn) {
        return;
    }

    ButtonEx* l_button_to_process = GetSectionButtonForIndex(f_section_buttonindex) ;

    if (LOCATION_HEAD == f_section_btn->coil->location && f_section_btn->section->elemXYZ[2] >= 240) {
        sCSConfigParams* l_coil_config_params = m_selection_utiliy->GetCSConfigParams();

        const int l_coil_lable_size = l_coil_config_params->m_couchLabelSize;
        double l_gui_sec_size = l_coil_lable_size;

        if (COUCH_POSITION_LEFT == m_couch_position) {
            f_section_btn->position.rightX = (int)(1 + f_section_btn->position.leftX + (l_gui_sec_size / l_coil_lable_size) * GetGUISectionWidthDefault());

        } else {
            f_section_btn->position.leftX = (int)(f_section_btn->position.rightX - (l_gui_sec_size / l_coil_lable_size) * GetGUISectionWidthDefault());
        }

        const int l_sec_width = f_section_btn->position.rightX - f_section_btn->position.leftX;

        if (COUCH_POSITION_LEFT == m_couch_position)
            f_section_btn->position.rightX += l_sec_width ;

        else
            f_section_btn->position.leftX = f_section_btn->position.rightX - l_sec_width * 2;

    } else if (f_section_btn->coil->position[0] == -1) {

        if (l_button_to_process) {
            //update the size now..

            sCSConfigParams* l_coil_config_params = m_selection_utiliy->GetCSConfigParams();

            const int l_coil_lable_size = l_coil_config_params->m_couchLabelSize;
            double l_gui_sec_size = l_coil_lable_size;

            const double l_step = l_coil_config_params->gui_sec_step;

            for (double i = l_coil_config_params->gui_sec_size_start; i < l_coil_config_params->gui_sec_size_end; i += l_step) {

                if (((i - l_step / 2) *l_coil_lable_size <= f_section_btn->section->elemXYZ[2]) && (f_section_btn->section->elemXYZ[2] < (i + l_step / 2) * l_coil_lable_size)) {
                    l_gui_sec_size = i * l_coil_lable_size;
                    break;
                }
            }

            if (m_couch_position == COUCH_POSITION_LEFT) {
                f_section_btn->position.rightX = (int)(1 + f_section_btn->position.leftX + (l_gui_sec_size / l_coil_lable_size) * GetGUISectionWidthDefault());

            } else {
                f_section_btn->position.leftX = (int)(f_section_btn->position.rightX - (l_gui_sec_size / l_coil_lable_size) * GetGUISectionWidthDefault());
            }

            const int l_previous_btn_id = (IDC_BUTTON_SPINE_1 + f_section_buttonindex) - 1;

            ButtonEx* l_prev_btn = GetSectionButtonForIndex(f_section_buttonindex - 1) ;

            if (l_prev_btn && l_prev_btn->IsWindowVisible()) {

                switch (l_previous_btn_id) {

                    case -1:
                    case IDC_BUTTON_SPINE_15:
                    case IDC_BUTTON_SPINE_22:
                        break;

                    default:

                        CoilSelectionSectionButton_t* l_prev_btn = m_selection_utiliy->GetSectionButton(f_section_buttonindex - 1);

                        const int l_sec_width = f_section_btn->position.rightX - f_section_btn->position.leftX;

                        if (m_couch_position == COUCH_POSITION_LEFT) {
                            f_section_btn->position.leftX = l_prev_btn->position.rightX;
                            f_section_btn->position.rightX = f_section_btn->position.leftX + l_sec_width;

                        } else {
                            f_section_btn->position.rightX = l_prev_btn->position.leftX - g_gui_section_gap;
                            f_section_btn->position.leftX = f_section_btn->position.rightX - l_sec_width;
                        }
                }
            }
        }
    }

    l_button_to_process->SetWindowPos(NULL, f_section_btn->position.leftX, f_section_btn->position.topY, f_section_btn->position.rightX - f_section_btn->position.leftX , f_section_btn->position.bottomY - f_section_btn->position.topY , SWP_NOZORDER) ;
}


void CCoilSelWin::SectionCoilSetSensitiveButton(
    const int f_section_buttonindex,
    const bool f_enable
)
{

    ButtonEx* l_button_to_process = GetSectionButtonForIndex(f_section_buttonindex) ;

    if (l_button_to_process)
        l_button_to_process->ShowWindow(f_enable ? SW_SHOW : SW_HIDE);
}

void CCoilSelWin::SectionCoilEnableDisableButton(
    const int f_section_buttonindex,
    const bool f_enable
)
{

    ButtonEx* l_button_to_process = GetSectionButtonForIndex(f_section_buttonindex) ;

    if (l_button_to_process)
        l_button_to_process->EnableWindow(f_enable ? SW_SHOW : SW_HIDE);
}

void CCoilSelWin::UpdateMsgWindowWith(
    const int f_msgstring_id
)
{
    //Patni-Hemant/2010May6/Commented/JaFT/SM_PH3_CDR-CDS_CDR_05
    //CString l_msgstring ;

    LPCTSTR FUNC_NAME = _T("CCoilSelWin::UpdateMsgWindowWith");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_selection_utiliy) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));

    } else {
        //-Patni-DKH/2009Aug24/Modified/cpp test corrections
        m_selection_utiliy->GetCoilSelectionGlobal()->message = f_msgstring_id;
    }//Patni-DKH/2009Aug24/Modified/cpp test corrections

    //   l_msgstring.LoadString(f_msgstring_id);

    //+Patni-Hemant/2010Feb12/Modified/ACE-Phase#3/CDS Requirement

    m_edtMsg.SetTextColor(g_msgwnd_default_textcolor);
    m_edtMsg.UpdateFont();

    //-Patni-Hemant/2010Feb12/Modified/ACE-Phase#3/CDS Requirement

    // Added By KT/ACE-2/11-06-2009/Multilingualisation
    m_edtMsg.SetWindowText(GetStringForID(f_msgstring_id)) ;
}



void CCoilSelWin::CoilSelectionUiSetWarningColor(
    const int f_coil_button_index
)
{

    ButtonEx* l_button_to_process = GetCoilButtonForIndex(f_coil_button_index) ;

    if (l_button_to_process) {

        SetWarningButtonColor(*l_button_to_process);
        return ;
    }


    ASSERT(FALSE) ;
    return;
}


void CCoilSelWin::SectionCoilButtonUiOnCoilPB(const int n, const bool flg)
{
    LPCTSTR FUNC_NAME = _T("CCoilSelWin::SectionCoilButtonUiOnCoilPB");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_selection_utiliy) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));

        return;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections
    CoilSelectionSectionButton_t* l_section_button = m_selection_utiliy->GetSectionButton(n) ;

    //Patni-Hemant/2009Nov25/Modified/IR110
    bool sensitive = m_btnCouch[n].IsWindowVisible() && m_btnCouch[n].IsWindowEnabled();

    if (sensitive && !l_section_button->position.linkflg

        //&&(state != ShiftMask || (beforeLeftX  == 0 && beforeTopY == 0))){
       ) {


        //+Patni-Hemant/2009Dec25/Modified/MVC006387
        /* sw==False：選択 */
        if (!l_section_button->section->enable[l_section_button->port]) {

            //CoilSelectionUiSetSelectColor(l_section_button->w);
            SectionCoilSetSelectColorForButton(n, true);

            if (flg) {
                /* On */

                l_section_button->section->enable[l_section_button->port] = true;			 /* Release */

                SectionCoilButtonUiLink(n);
            }

            /* sw==True：選択解除 */

        } else if (l_section_button->section->enable[l_section_button->port]) {

            //CoilSelectionUiSetDefaultColor(l_section_button->w);
            //CoilSelectionUiSetDefaultColor(l_section_button->w);
            SectionCoilSetDefaultColorForButton(n, true);

            if (flg) {
                /* Off */
                l_section_button->section->enable[l_section_button->port] = false;			 /* Release */

                SectionCoilButtonUiLink(n);
            }
        }

        //-Patni-Hemant/2009Dec25/Modified/MVC006387
    }
}


/*********************************************************************
 * Description:
 * Assumptions:
 * Side Effects:
 * Return Value:
 *********************************************************************/
void CCoilSelWin::SectionCoilButtonUiOffCoilPB(const int n, const bool flg)
{
    LPCTSTR FUNC_NAME = _T("CCoilSelWin::SectionCoilButtonUiOnCoilPB");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_selection_utiliy) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));
        return;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections
    CoilSelectionSectionButton_t* l_section_button = m_selection_utiliy->GetSectionButton(n) ;

    //Patni-Hemant/2009Nov25/Modified/IR110
    BOOL sensitive = m_btnCouch[n].IsWindowVisible() && m_btnCouch[n].IsWindowEnabled();

    //	/* Sensitive取得 */
    //	XtVaGetValues(l_section_button->w,
    //					XmNsensitive,	 &sensitive,
    //					NULL);

    if (sensitive && !l_section_button->position.linkflg
        //&&(state != ShiftMask || (beforeLeftX  == 0 && beforeTopY == 0))){
       ) {

        //+Patni-Hemant/2009Dec25/Modified/MVC006387
        if (! l_section_button->section->enable[l_section_button->port]) {

            SectionCoilSetDefaultColorForButton(n, true);

            //l_section_button->section->enable[l_section_button->port] = TRUE;

        } else {

            SectionCoilSetSelectColorForButton(n, true);

            //l_section_button->section->enable[l_section_button->port] = FALSE;
        }

        //-Patni-Hemant/2009Dec25/Modified/MVC006387
    }

}

void CCoilSelWin::SectionCoilButtonUiLink(const int n)
{
    LPCTSTR FUNC_NAME = _T("CCoilSelWin::SectionCoilButtonUiLink");
    PQM_TRACE_SCOPED(FUNC_NAME);


    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_selection_utiliy) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));

        return;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    CoilSelectionSectionButton_t* l_section_button = m_selection_utiliy->GetSectionButton(n) ;

    CoilSelectionSectionButton_t* l_section_button_i = NULL;

    if (l_section_button->section->link != NULL) {
        for (int i = 0; i < MAX_SECTION_BUTTON; i++) {

            l_section_button_i = m_selection_utiliy->GetSectionButton(i) ;

            if (l_section_button_i->section != NULL) {

                if (l_section_button->HasLinkedSectionOfSameCoil(l_section_button_i) ||
                    m_selection_utiliy->IsLinkedSectionOfGroupedCoil(l_section_button, l_section_button_i)) {

                    if (l_section_button->section->enable[l_section_button->port]  ==  TRUE) {

                        l_section_button_i->section->enable[l_section_button_i->port]  =   TRUE;

                        SectionCoilSetSelectColorForButton(i);

                    } else {

                        l_section_button_i->section->enable[l_section_button_i->port]  =   false;

                        SectionCoilSetDefaultColorForButton(i);
                    }

                    l_section_button_i->position.linkflg = true;
                }
            }
        }
    }
}



void CCoilSelWin::CreateCoilListDialog(
    const bool f_show /*= false*/
)
{
    LPCTSTR FUNC_NAME = _T("CCoilSelWin::CreateCoilListDialog");
    PQM_TRACE_SCOPED(FUNC_NAME);

    if (m_p_dialog_coil_list && ::IsWindow(m_p_dialog_coil_list->m_hWnd) && m_p_dialog_coil_list->IsWindowVisible())
        return ;

    if (!m_p_dialog_coil_list) {
        m_p_dialog_coil_list = new CCoilLstDlg(m_selection_utiliy, static_cast<CCoilSelWinInterface*>(this), this);
    }

    if (m_p_dialog_coil_list) {
        m_p_dialog_coil_list->SetWindowCaption(m_window_caption) ;

        if (!m_p_dialog_coil_list->GetSafeHwnd()) {

            m_p_dialog_coil_list->Create(IDD_DIALOG_COIL_LST, this); //+Patni-SS/MODI/2009-May-27/this pointer should be passed to handel pretanslate message

        } else {
            m_p_dialog_coil_list->SetFocus();
        }
    }


    if (f_show) {
        //+Patni-Dhanesh/2009Aug20/Modified/cpp test corrections
        if (m_p_dialog_coil_list) {
            m_p_dialog_coil_list->ShowWindow(SW_SHOW);

            bool check_combine = true ;	//we only want to check the combine..
            m_p_dialog_coil_list->DisplayCoilInformation(false, &check_combine);

        } else {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("m_p_dialog_coil_list pointer is NULL"));
        }

        //-Patni-Dhanesh/2009Aug20/Modified/cpp test corrections
    }
}


void CCoilSelWin::ReflectDataToListDialog()const
{

    if (m_p_dialog_coil_list && ::IsWindow(m_p_dialog_coil_list->m_hWnd) && m_p_dialog_coil_list->IsWindowVisible())
        m_p_dialog_coil_list->DisplayCoilInformation();
}


//+Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : IsHighlighted
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
inline bool CCoilSelWin::IsHighlighted(
    int f_index
) const  	//Patni-Hemant/2010May6/Added/JaFT/SM_PH3_CDR-CDS_CDR_08
{

    return ((f_index == m_highlight_1) || (f_index == m_highlight_2));
}
//-Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement



//+Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : CoilSelectionUiMapCenterPos
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCoilSelWin::CoilSelectionUiMapCenterPos(
    const float f_center_pos
)
{
    LPCTSTR FUNC_NAME = _T("CCoilSelWin::CoilSelectionUiMapCenterPos");
    PQM_TRACE_SCOPED(FUNC_NAME);


    if (f_center_pos == 0) {
        return;	/* Do not display field center */
    }

    if (f_center_pos >  15.0) {
        return;	/* Do not display field center*/
    }

    int	l_integer = (int)f_center_pos;

    float l_decimal = f_center_pos - (float)l_integer;

    /* set the first highlighting channel */
    //XtMapWidget(CoilSelectionUiGetWidget(tag));
    m_highlight_1 = l_integer ;

    /* set the second highlighting channel, if fraction is available */
    if (l_decimal) {
        //XtMapWidget(CoilSelectionUiGetWidget(tag+1));
        m_highlight_2 = l_integer + 1;
    }

    CString l_log_str ;
    l_log_str.Format(_T("Center Pos = %f, \
                         Hightlighted Channel 1 = %d, \
                         Hightlighted Channel 2 = %d" \
                       ),                                \
                     f_center_pos, m_highlight_1, m_highlight_2  \
                    );
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_str);

    InvalidateRect(GetMainImageRect());
    UpdateWindow();
}
//-Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement

//************************************Method Header************************************
// Method Name  : MoveFreeCoils
// Author       : iGATE / RJ
// Purpose      : Moves free coils as per click i.e. left or right
//***********************************************************************************
void CCoilSelWin::MoveFreeCoils(
    const CCoilSelWin::CoilMove f_move_free_coil
)
{
    LPCTSTR FUNC_NAME = _T("CCoilSelWin::MoveFreeCoils");
    PQM_TRACE_SCOPED(FUNC_NAME);

    if (NULL == m_selection_utiliy) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));

        return;
    }

    if (!m_selection_utiliy->IsCoilApplied())
        return;

    int l_move = 1;

    switch (f_move_free_coil) {

        case CoilMove_Left:
            l_move = -1 ;
            break ;

        case CoilMove_Right:
            l_move = 1 ;
            break ;

        default :
            return ;
    }

    m_selection_utiliy->CoilSelectionCoilSetFreePosition(l_move, 1, true);

    UiCheckCombine();

    if (!m_selection_utiliy->HasDisallowableCombineCoil())
        ReflectDataToListDialog();
}

//************************************Method Header************************************
// Method Name  : MoveSpineCoils
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCoilSelWin::MoveSpineCoils(
    const CCoilSelWin::CoilMove f_movespine
)
{
    LPCTSTR FUNC_NAME = _T("CCoilSelWin::MoveSpineCoils");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_selection_utiliy) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));

        return;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections


    if (!m_selection_utiliy->IsCoilApplied())
        return;

    int l_move = 1;

    switch (f_movespine) {

        case CoilMove_Left:
            l_move = -1 ;
            break ;

        case CoilMove_Right:
            l_move = 1 ;
            break ;

        default :
            return ;
    }


    m_selection_utiliy->CoilSelectionCoilSetFreePosition(l_move, 2, true);

    UiCheckCombine();

    if (!m_selection_utiliy->HasDisallowableCombineCoil()) {
        ReflectDataToListDialog();
    }
}


bool CCoilSelWin::UisetCoilButtonPosition()
{

    LPCTSTR FUNC_NAME = _T("CCoilSelWin::UisetCoilButtonPosition");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_selection_utiliy) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));

        return false;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    CoilSelectionSectionButton_t* l_section_btn = NULL , * l_pre_section_btn = NULL;

    RECT l_btn_rect  = {0};
    CWnd* l_btn       = NULL;


    UINT l_button_id = IDC_BUTTON_SPINE_1 ;

    for (int i = 0; i < MAX_SECTION_BUTTON; i++, l_button_id++) {

        memset(&l_btn_rect, 0, sizeof(RECT));

        l_section_btn = m_selection_utiliy->GetSectionButton(i);
        l_btn = GetDlgItem(l_button_id);
        l_btn->GetWindowRect(&l_btn_rect);

        ScreenToClient(&l_btn_rect) ;

        l_section_btn->position.topY    = l_btn_rect.top;
        l_section_btn->position.bottomY = l_btn_rect.bottom;

        switch (l_button_id) {

            case IDC_BUTTON_SPINE_1:
            case IDC_BUTTON_SPINE_16:
            case IDC_BUTTON_SPINE_17:
            case IDC_BUTTON_SPINE_18:
            case IDC_BUTTON_SPINE_19:
            case IDC_BUTTON_SPINE_20:
            case IDC_BUTTON_SPINE_21:
            case IDC_BUTTON_SPINE_22:
            case IDC_BUTTON_SPINE_23:

                l_section_btn->position.leftX = l_btn_rect.left;
                l_section_btn->position.rightX = l_btn_rect.right;
                break;

            default: {

                l_pre_section_btn = m_selection_utiliy->GetSectionButton(i - 1);
                const int l_prev_btn_width = (l_pre_section_btn->position.rightX - l_pre_section_btn->position.leftX);

                if (m_couch_position == COUCH_POSITION_LEFT) {

                    l_section_btn->position.leftX = l_pre_section_btn->position.rightX + g_gui_section_gap;
                    l_section_btn->position.rightX = l_section_btn->position.leftX + l_prev_btn_width;

                } else {

                    l_section_btn->position.rightX = l_pre_section_btn->position.leftX - g_gui_section_gap;
                    l_section_btn->position.leftX = l_section_btn->position.rightX - l_prev_btn_width;
                }
            }
        }


        l_section_btn->m_org_pos = l_section_btn->position;
    }

    return true;
}


//***************************Method Header*************************************
//Method Name    : Cleanup()
//Author         : PATNI /
//Purpose        :
//*****************************************************************************
inline void CCoilSelWin::Cleanup(
)
{

    DEL_PTR(m_p_dialog_coil_list)
}


//***************************Method Header*************************************
//Method Name    : UiChangeMessage()
//Author         : PATNI / HAR + AMT
//Purpose        :
//*****************************************************************************
void CCoilSelWin::UiChangeMessage(
)
{
    LPCTSTR FUNC_NAME = _T("CCoilSelWin::UiChangeMessage");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    CoilSelectionGlobal_t* l_CoilSelectionGLobal = GetCoilSelectionGlobal() ;

    if (NULL == l_CoilSelectionGLobal) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("l_CoilSelectionGLobal pointer is NULL"));
        return;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    if (l_CoilSelectionGLobal->message == IDS_CS_MES_COMBINE) {
        return ;
    }

    CoilSelectionSectionButton_t* l_section_button = NULL;
    CoilSelectionSectionButton_t* l_coil_button = NULL;

    int l_max = 0, l_min = 0, l_position = -1;
    BOOL    l_enable = FALSE;
    CString l_err_msg = _T("");

    int l_leftmost_sect_left = 0;
    int l_rightmost_sect_left = 0;

    //Select Check
    for (int i = 0; i < MAX_SECTION_BUTTON; i++) {

        l_section_button = m_selection_utiliy->GetSectionButton(i);

        if (l_section_button->section) {

            if (l_section_button->section->enable[l_section_button->port] == TRUE) {

                l_enable = TRUE;
                l_position = m_selection_utiliy->ComputePosition(i);

                if (m_couch_position == COUCH_POSITION_LEFT) {

                    if ((l_rightmost_sect_left == 0) || (l_section_button->position.leftX > l_rightmost_sect_left)) {
                        l_rightmost_sect_left = l_section_button->position.leftX;
                        l_max = l_position ;
                    }

                    if ((0 == l_leftmost_sect_left) || (l_section_button->position.leftX < l_leftmost_sect_left)) {
                        l_leftmost_sect_left = l_section_button->position.leftX;
                        l_min = l_position;
                    }

                } else {

                    if ((l_leftmost_sect_left == 0) || (l_section_button->position.rightX > l_leftmost_sect_left)) {
                        l_leftmost_sect_left = l_section_button->position.rightX;
                        l_min = l_position;
                    }

                    if ((0 == l_rightmost_sect_left) || (l_section_button->position.rightX < l_rightmost_sect_left)) {
                        l_rightmost_sect_left = l_section_button->position.rightX;
                        l_max = l_position ;
                    }

                }
            }
        }
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Selection Check is OK"));

    if (!l_enable) {

        if (!m_selection_utiliy->IsCoilApplied())
            return ;

        for (i = 0; i < MAX_COIL_BUTTON; i++) {

            l_coil_button = m_selection_utiliy->GetCoilButton(i);

            if (l_coil_button->position.sw == TRUE) {

                break;
            }
        }

        if (i == MAX_COIL_BUTTON) {

            //CoilSelectionUiMessage("MES_SELECT_SECTION");
            UpdateMsgWindowWith(IDS_CS_MES_SELECT_SECTION);

            return;
        }
    }

    //Channel Check
    int channels = 0;
    CString l_trace_msg = _T("");

    for (i = 0; i < MAX_SECTION_BUTTON; i++) {

        l_section_button = m_selection_utiliy->GetSectionButton(i);

        if (l_section_button && l_section_button->section) {

            if (l_section_button->section->enable[l_section_button->port]) {

                channels += l_section_button->section->minChannel;
                l_trace_msg.Format(_T("Coil-Section[channels][SystemChannels] = [%d][%d]"),
                                   channels, GetCoilSelectionGlobal()->coil.numChannels);
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_msg);

                if (channels > GetCoilSelectionGlobal()->coil.numChannels) {

                    l_err_msg.Empty();
                    l_err_msg.Format(_T("Coil-Section[channels > SystemChannels] = [%d>%d]"),
                                     channels, GetCoilSelectionGlobal()->coil.numChannels);

                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_err_msg);
                    UpdateWarningMsgWindowWith(IDS_CS_MES_CHANNEL_OVER);

                    return;
                }
            }
        }
    }

    for (i = 0; i < MAX_COIL_BUTTON; i++) {

        l_coil_button = m_selection_utiliy->GetCoilButton(i);

        if (l_coil_button &&
            l_coil_button->position.sw &&
            l_coil_button->coil) {

            l_trace_msg.Format(_T("l_coil_button[%d]->coil->numofsec = [%d]"),
                               i, l_coil_button->coil->numofsec);

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_msg);

            channels += (l_coil_button->coil->numofsec && l_coil_button->section) ? l_coil_button->section->minChannel : l_coil_button->coil->maxChannel;

            l_trace_msg.Format(_T("Coil[channels][SystemChannels] = [%d][%d]"),
                               channels, GetCoilSelectionGlobal()->coil.numChannels);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_msg);

            if (channels > GetCoilSelectionGlobal()->coil.numChannels) {


                l_err_msg.Empty();
                l_err_msg.Format(_T("Coil[channels > SystemChannels] = [%d>%d]"),
                                 channels, GetCoilSelectionGlobal()->coil.numChannels);

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_err_msg);
                UpdateWarningMsgWindowWith(IDS_CS_MES_CHANNEL_OVER);

                return;
            }
        }
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Channel Check is OK"));

    // Preference field Check
    if ((ABSF(l_rightmost_sect_left - l_leftmost_sect_left) - g_gui_section_gap) > ((GetGUISectionWidthDefault() + g_gui_section_gap) * m_selection_utiliy->GetCSConfigParams()->couch_select_range)) {

        //+Patni-Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement
        //UpdateMsgWindowWith(IDS_CS_MES_BAD_FIELD);
        UpdateWarningMsgWindowWith(IDS_CS_MES_BAD_FIELD);
        //-Patni-Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement
        return;
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Preference field Check is OK"));

    //+Patni-Hemant/2010May6/Added/JaFT/SM_PH3_CDR-CDS_CDR_06
    const int l_head_coil_btn = 16 ;
    const int l_neck_coil_btn = 20 ;
    const int l_btn_below_neck_coil = 24 ;
    //+Patni-Hemant/2010May6/Added/JaFT/SM_PH3_CDR-CDS_CDR_06

    // Continuousness Check
    for (i = 0; i < MAX_SECTION_BUTTON; i++) {

        l_section_button = m_selection_utiliy->GetSectionButton(i);

        if (l_section_button && l_section_button->enable) {

            if (l_section_button->section != NULL &&
                l_section_button->section->section == 1) {

                if (!UiCheckContinuousness(i)) {

                    //CoilSelectionUiMessage("MES_CONTINUOUSNESS");
                    //+Patni-AMT/2009Oct05/Modified/MVC005124-Code Review
                    l_err_msg.Empty();
                    l_err_msg.Format(_T("UiCheckContinuousness for %d"), i);

                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_err_msg);

                    //-Patni-AMT/2009Oct05/Modified/MVC005124-Code Review
                    //+Patni-Hemant/2010Feb12/Modified/ACE-Phase#3/CDS Requirement
                    //UpdateMsgWindowWith(IDS_CS_MES_CONTINUOUSNESS);
                    UpdateWarningMsgWindowWith(IDS_CS_MES_CONTINUOUSNESS);
                    //-Patni-Hemant/2010Feb12/Modified/ACE-Phase#3/CDS Requirement

                    return;
                }
            }

            //+Patni-Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement

            //Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_06
            if (l_head_coil_btn == i) {

                CoilSelectionSectionButton_t* l_coil_button_1 = m_selection_utiliy->GetSectionButton(15);
                CoilSelectionSectionButton_t* l_coil_button_2 = m_selection_utiliy->GetSectionButton(14);

                if (!l_coil_button_1->enable && !l_coil_button_2->enable) {

                    if (!UiCheckContinuousness(i)) {

                        /* ????? */
                        //CoilSelectionUiWarningMessage("MES_CONTINUOUSNESS");
                        UpdateWarningMsgWindowWith(IDS_CS_MES_CONTINUOUSNESS);

                        return;
                    }
                }
            }

            //+Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_06
            //			if(20 == i){
            //
            //				if(m_selection_utiliy->GetSectionButton(23) && m_selection_utiliy->GetSectionButton(23)->enable)
            //				{	/* Head6?Spine????? */
            //					if( (m_selection_utiliy->GetSectionButton(20)->section->enable[m_selection_utiliy->GetSectionButton(20)->port] == TRUE)
            //					&& ((m_selection_utiliy->GetSectionButton(23)->section->enable[m_selection_utiliy->GetSectionButton(23)->port] == TRUE)
            //					||  (m_selection_utiliy->GetSectionButton(24)->section->enable[m_selection_utiliy->GetSectionButton(24)->port] == TRUE)
            //					||  (m_selection_utiliy->GetSectionButton(25)->section->enable[m_selection_utiliy->GetSectionButton(25)->port] == TRUE)
            //					||  (m_selection_utiliy->GetSectionButton(26)->section->enable[m_selection_utiliy->GetSectionButton(26)->port] == TRUE) ) )
            //					{
            //						/* ????? */
            //						//CoilSelectionUiWarningMessage("MES_SN_DOWN");
            //						UpdateWarningMsgWindowWith(IDS_CS_MES_SN_DOWN);
            //						return;
            //					}
            //				}
            //			}

            if (l_neck_coil_btn == i) {

                if (m_selection_utiliy->GetSectionButton(l_btn_below_neck_coil) && m_selection_utiliy->GetSectionButton(l_btn_below_neck_coil)->enable) {
                    /* Head6?Spine????? */
                    if ((m_selection_utiliy->GetSectionButton(l_neck_coil_btn)->section->enable[m_selection_utiliy->GetSectionButton(l_neck_coil_btn)->port] == TRUE)
                        && ((m_selection_utiliy->GetSectionButton(l_btn_below_neck_coil)->section->enable[m_selection_utiliy->GetSectionButton(l_btn_below_neck_coil)->port] == TRUE)
                            || (m_selection_utiliy->GetSectionButton(l_btn_below_neck_coil + 1)->section->enable[m_selection_utiliy->GetSectionButton(l_btn_below_neck_coil + 1)->port] == TRUE)
                            || (m_selection_utiliy->GetSectionButton(l_btn_below_neck_coil + 2)->section->enable[m_selection_utiliy->GetSectionButton(l_btn_below_neck_coil + 2)->port] == TRUE)
                            || (m_selection_utiliy->GetSectionButton(l_btn_below_neck_coil + 3)->section->enable[m_selection_utiliy->GetSectionButton(l_btn_below_neck_coil + 3)->port] == TRUE))) {
                        /* ????? */
                        //CoilSelectionUiWarningMessage("MES_SN_DOWN");
                        UpdateWarningMsgWindowWith(IDS_CS_MES_SN_DOWN);
                        return;
                    }
                }
            }

            //-Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_06

            //-Patni-Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement
        }
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Continuousness Check is OK"));

    if (GetCoilSelectionGlobal()->message != IDS_CS_MES_COMBINE) {
        //+Patni-Hemant/2010Feb12/Modified/ACE-Phase#3/CDS Requirement
        //UpdateMsgWindowWith(IDS_CS_MES_OK);

        UpdateMsgWindowWithMsgOK();
        //-Patni-Hemant/2010Feb12/Modified/ACE-Phase#3/CDS Requirement
    }
}

//***************************Method Header*************************************
//Method Name    : UiCheckContinuousness()
//Author         : PATNI / HAR + AMT
//Purpose        :
//*****************************************************************************
BOOL CCoilSelWin::UiCheckContinuousness(
    const int f_count
)const
{
    LPCTSTR FUNC_NAME = _T("CCoilSelWin::UiCheckContinuousness");
    PQM_TRACE_SCOPED(FUNC_NAME);

    int 	i = -1;
    BOOL    l_on_section[11] = {0};

    //DB_FUNC_ENTER("[coilSelectionUiCheckContinuousness]:");
    CoilSelectionSectionButton_t* l_section_button = NULL;
    //Head coil (exceptional)

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_selection_utiliy) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));

        return FALSE;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections
    //+Patni-Hemant/2010Feb15/Modified/ACE-Phase#3/CDS Requirement
    //    if (f_count == 15) {
    //
    //        l_section_button = m_selection_utiliy->GetSectionButton(15);
    //
    //        if (l_section_button->section->enable[l_section_button->port] == TRUE) {
    //
    //            l_section_button = m_selection_utiliy->GetSectionButton(17);
    //
    //            if (l_section_button->section->enable[l_section_button->port] == FALSE) {
    //
    //                l_section_button = m_selection_utiliy->GetSectionButton(19);
    //
    //                if (l_section_button->section != NULL &&
    //
    //                    l_section_button->section->enable[l_section_button->port] ==
    //
    //                    TRUE) {
    //
    //                    return FALSE;
    //                }
    //            }
    //        }
    //
    //        return TRUE;
    //    }

    /* Head coil (Exceptions) */
    /* 15 17 19
          20
       16 18	*/
    if ((f_count == 15) || (f_count == 16)) {
        if (((m_selection_utiliy->GetSectionButton(15)->section != NULL)
             && (m_selection_utiliy->GetSectionButton(15)->section->enable[m_selection_utiliy->GetSectionButton(15)->port] == TRUE))
            || ((m_selection_utiliy->GetSectionButton(16)->section != NULL)
                && (m_selection_utiliy->GetSectionButton(16)->section->enable[m_selection_utiliy->GetSectionButton(16)->port] == TRUE))
           ) {
            if (((m_selection_utiliy->GetSectionButton(17)->section != NULL)
                 && (m_selection_utiliy->GetSectionButton(17)->section->enable[m_selection_utiliy->GetSectionButton(17)->port] == FALSE))
                || ((m_selection_utiliy->GetSectionButton(18)->section != NULL)
                    && (m_selection_utiliy->GetSectionButton(18)->section->enable[m_selection_utiliy->GetSectionButton(18)->port] == FALSE))
               ) {
                if (((m_selection_utiliy->GetSectionButton(19)->section != NULL)
                     && (m_selection_utiliy->GetSectionButton(19)->section->enable[m_selection_utiliy->GetSectionButton(19)->port] == TRUE))
                    || ((m_selection_utiliy->GetSectionButton(20)->section != NULL)
                        && (m_selection_utiliy->GetSectionButton(20)->section->enable[m_selection_utiliy->GetSectionButton(20)->port] == TRUE))
                    || ((m_selection_utiliy->GetSectionButton(21)->section != NULL)
                        && (m_selection_utiliy->GetSectionButton(21)->section->enable[m_selection_utiliy->GetSectionButton(21)->port] == TRUE))
                   ) {
                    return FALSE;
                }
            }
        }

        return TRUE;
    }

    //-Patni-Hemant/2010Feb15/Modified/ACE-Phase#3/CDS Requirement

    l_section_button = m_selection_utiliy->GetSectionButton(f_count);

    for (i = 0; i < l_section_button->coil->numofsec; i++) {

        l_section_button = m_selection_utiliy->GetSectionButton(f_count + i);

        if (l_section_button->section->enable[l_section_button->port] ==
            TRUE) {

            l_on_section[i] = TRUE;
        }
    }

    // Check
    l_section_button = m_selection_utiliy->GetSectionButton(f_count);

    for (i = 0; i < l_section_button->coil->numofsec; i++) {

        if (l_on_section[i]) {

            for (i = i; i < l_section_button->coil->numofsec; i++) {

                if (!l_on_section[i]) {

                    for (i = i; i < l_section_button->coil->numofsec; i++) {

                        if (l_on_section[i]) {

                            return FALSE;
                        }
                    }
                }
            }
        }
    }

    return TRUE;
}


//***************************Method Header*************************************
//Method Name    : UiCheckSectionCombine()
//Author         : PATNI / HAR + AMT
//Purpose        :
//*****************************************************************************
BOOL CCoilSelWin::UiCheckSectionCombine(
    const int f_count
)const
{
    LPCTSTR FUNC_NAME = _T("CCoilSelWin::UiCheckSectionCombine");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_selection_utiliy) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));

        return FALSE;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    //search for a coil
    int j = 0;
    int  position[8] = { -1, -1, -1, -1, -1, -1, -1, -1};
    CoilSelectionSectionButton_t* l_section_button = NULL;

    for (int i = 0; i < 8; i++) {

        l_section_button = m_selection_utiliy->GetSectionButton(f_count);

        if ((int)l_section_button->section->section - j < 1) {

            break;
        }

        position[i] = f_count - j;
        j++;
    }

    j = 1;

    for (i = i; i < 8; i++) {

        l_section_button = m_selection_utiliy->GetSectionButton(f_count);

        if ((int)l_section_button->section->section + j >
            l_section_button->coil->numofsec) {

            break;
        }

        position[i] = f_count + j;
        j++;
    }

    //Check Combine
    for (i = 0; i < MAX_SECTION_BUTTON; i++) {

        l_section_button = m_selection_utiliy->GetSectionButton(i);

        if (l_section_button->section) {


            if (l_section_button->section->enable[l_section_button->port] == TRUE &&
                position[0] != i && position[1] != i &&
                position[2] != i && position[3] != i &&
                position[4] != i && position[5] != i &&
                position[6] != i &&  position[7] != i) {

                return TRUE;
            }
        }
    }

    CoilSelectionSectionButton_t* l_coil_button = NULL;

    for (i = 0; i < MAX_COIL_BUTTON; i++) {

        l_coil_button = m_selection_utiliy->GetCoilButton(i);

        if (l_coil_button->position.sw == TRUE) {
            return TRUE;
        }
    }

    return FALSE;
}

//***************************Method Header*************************************
//Method Name    : UiCheckCoilCombine()
//Author         : PATNI / HAR + AMT
//Purpose        :
//*****************************************************************************
BOOL CCoilSelWin::UiCheckCoilCombine(
    const int f_count
)const
{
    LPCTSTR FUNC_NAME = _T("CCoilSelWin::UiCheckCoilCombine");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_selection_utiliy) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));

        return FALSE;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections


    CoilSelectionSectionButton_t* l_section_button = NULL;

    for (int i = 0; i < MAX_SECTION_BUTTON; i++) {

        l_section_button = m_selection_utiliy->GetSectionButton(i);

        if (l_section_button->section) {

            if (l_section_button->section->enable[l_section_button->port] ==
                TRUE) {

                return TRUE;
            }
        }
    }

    CoilSelectionSectionButton_t* l_coil_button = NULL;

    for (i = 0; i < MAX_COIL_BUTTON; i++) {

        l_coil_button = m_selection_utiliy->GetCoilButton(i);

        if (l_coil_button->position.sw == TRUE && (i != f_count)) {
            return TRUE;
        }
    }

    return FALSE;
}

//***************************Method Header*************************************
//Method Name    : UiCheckSelectCoil()
//Author         : PATNI / HAR + AMT
//Purpose        :
//*****************************************************************************
BOOL CCoilSelWin::UiCheckSelectCoil(
)
{
    LPCTSTR FUNC_NAME = _T("CCoilSelWin::UiCheckSelectCoil");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_selection_utiliy) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));

        return FALSE;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    // Select Check
    int l_max = 0, l_min = 0, l_position = -1;		/* Position */
    bool_t  l_enable = FALSE;
    CoilSelectionSectionButton_t* l_section_button = NULL;

    int l_leftmost_sect_left = 0;
    int l_rightmost_sect_left = 0;

    for (int i = 0; i < MAX_SECTION_BUTTON; i++) {

        l_section_button = m_selection_utiliy->GetSectionButton(i);

        if (l_section_button->section) {

            if (l_section_button->section->enable[l_section_button->port] == TRUE) {

                l_enable = TRUE;
                l_position = m_selection_utiliy->ComputePosition(i);

                if (m_couch_position == COUCH_POSITION_LEFT) {

                    if ((l_rightmost_sect_left == 0) || (l_section_button->position.leftX > l_rightmost_sect_left)) {
                        l_rightmost_sect_left = l_section_button->position.leftX;
                        l_max = l_position ;
                    }

                    if ((0 == l_leftmost_sect_left) || (l_section_button->position.leftX < l_leftmost_sect_left)) {
                        l_leftmost_sect_left = l_section_button->position.leftX;
                        l_min = l_position;
                    }

                } else {

                    if ((l_leftmost_sect_left == 0) || (l_section_button->position.rightX > l_leftmost_sect_left)) {
                        l_leftmost_sect_left = l_section_button->position.rightX;
                        l_min = l_position;
                    }

                    if ((0 == l_rightmost_sect_left) || (l_section_button->position.rightX < l_rightmost_sect_left)) {
                        l_rightmost_sect_left = l_section_button->position.rightX;
                        l_max = l_position ;
                    }

                }

            }
        }
    }

    CoilSelectionSectionButton_t* l_coil_button = NULL;

    if (!l_enable) {

        for (i = 0; i < MAX_COIL_BUTTON; i++) {

            l_coil_button = m_selection_utiliy->GetCoilButton(i);

            if (l_coil_button->position.sw == TRUE) {
                break;
            }
        }

        if (i == MAX_COIL_BUTTON) {

            CCoilSelectionUtility::DisplayMessageinWarningDialog(
                CPQMUtility::GetMultiLingualString(_T("IDS_CS_WAR_NO_SELECT")),
                SCAN_WARN_OK_CANCEL_BUTTONS);

            return FALSE;
        }
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Selection Check is OK"));

    //Combine Check
    for (i = 0; i < MAX_SECTION_BUTTON; i++) {

        l_section_button = m_selection_utiliy->GetSectionButton(i);

        if (l_section_button->section) {

            if (l_section_button->section->enable[l_section_button->port] ==

                TRUE &&
                (l_section_button->coil->combine & COMBINE_WB_MASK) ==
                COMBINE_SWITCH_SELECT) {

                if (UiCheckSectionCombine(i)) {

                    CCoilSelectionUtility::DisplayMessageinWarningDialog(
                        CPQMUtility::GetMultiLingualString(_T("IDS_CS_WAR_CHECK_COMBINE_3")),
                        SCAN_WARN_OK_CANCEL_BUTTONS);

                    return FALSE;
                }
            }
        }
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Combine Check is OK"));

    // Channel Check */
    int channels = 0;
    CString l_trace_msg = _T("");

    for (i = 0; i < MAX_SECTION_BUTTON; i++) {

        l_section_button = m_selection_utiliy->GetSectionButton(i);

        if (l_section_button->section) {

            if (l_section_button->section->enable[l_section_button->port]) {

                channels += l_section_button->section->minChannel;

                l_trace_msg.Format(_T("Coil-Section[channels][SystemChannels] = [%d][%d]"),
                                   channels, GetCoilSelectionGlobal()->coil.numChannels);
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_msg);

                if (channels > GetCoilSelectionGlobal()->coil.numChannels) {


                    CCoilSelectionUtility::DisplayMessageinWarningDialog(
                        CPQMUtility::GetMultiLingualString(_T("IDS_CS_WAR_CHECK_CHANNEL")),
                        SCAN_WARN_OK_CANCEL_BUTTONS);

                    return FALSE;
                }
            }
        }
    }

    for (i = 0; i < MAX_COIL_BUTTON; i++) {

        l_coil_button = m_selection_utiliy->GetCoilButton(i);

        if (l_coil_button->position.sw) {

            if (NULL == l_coil_button->coil) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Error Occured. Coil=NULL Found in Channel Check"));

            } else {

                l_trace_msg.Format(_T("l_coil_button[%d]->coil->numofsec = [%d]"),
                                   i, l_coil_button->coil->numofsec);
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_msg);

                channels += (l_coil_button->coil->numofsec && l_coil_button->section) ? l_coil_button->section->minChannel : l_coil_button->coil->maxChannel;

                l_trace_msg.Format(_T("Coil[channels][SystemChannels] = [%d][%d]"),
                                   channels, GetCoilSelectionGlobal()->coil.numChannels);
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace_msg);

                if (channels > GetCoilSelectionGlobal()->coil.numChannels) {

                    CCoilSelectionUtility::DisplayMessageinWarningDialog(
                        CPQMUtility::GetMultiLingualString(_T("IDS_CS_WAR_CHECK_CHANNEL")),
                        SCAN_WARN_OKBUTTON);

                    return FALSE;
                }
            }
        }
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Channel is OK"));

    // Preference field Check
    if ((ABSF(l_rightmost_sect_left - l_leftmost_sect_left) - g_gui_section_gap) > ((GetGUISectionWidthDefault() + g_gui_section_gap) * m_selection_utiliy->GetCSConfigParams()->couch_select_range)) {

        CCoilSelectionUtility::DisplayMessageinWarningDialog(
            CPQMUtility::GetMultiLingualString(_T("IDS_CS_WAR_BAD_FIELD")),
            SCAN_WARN_OKBUTTON);

        return FALSE;
    }

    for (i = 0; i < MAX_COIL_BUTTON; i++) {

        l_coil_button = m_selection_utiliy->GetCoilButton(i);

        if ((l_coil_button->coil) && (l_coil_button->position.sw == TRUE) &&
            ((l_coil_button->coil->combine & COMBINE_WB_MASK) == COMBINE_SWITCH_SELECT)
           ) {

            if (UiCheckCoilCombine(i)) {

                CCoilSelectionUtility::DisplayMessageinWarningDialog(CPQMUtility::GetMultiLingualString(_T("IDS_CS_WAR_CHECK_COMBINE_3")), SCAN_WARN_OKBUTTON);

                return FALSE;
            }
        }
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Preference field Check is OK"));

    //Continuousness Check
    for (i = 0; i < MAX_SECTION_BUTTON; i++) {

        l_section_button = m_selection_utiliy->GetSectionButton(i);

        if (l_section_button->enable) {

            if (l_section_button->section && l_section_button->section->section == 1) {

                if (!UiCheckContinuousness(i)) {

                    CCoilSelectionUtility::DisplayMessageinWarningDialog(
                        CPQMUtility::GetMultiLingualString(_T("IDS_CS_WAR_CHECK_CONTINUOUSNESS")),
                        SCAN_WARN_OKBUTTON);

                    return FALSE;
                }
            }

            //+Patni-Hemant/2010Feb15/Added/ACE-Phase#3/CDS Requirement
            if (i == 16) {

                if (!m_selection_utiliy->GetSectionButton(15)->enable && !m_selection_utiliy->GetSectionButton(14)->enable) {

                    if (!UiCheckContinuousness(i)) {

                        CCoilSelectionUtility::DisplayMessageinWarningDialog(
                            CPQMUtility::GetMultiLingualString(_T("IDS_CS_WAR_CHECK_CONTINUOUSNESS")),
                            SCAN_WARN_OKBUTTON);

                        return FALSE;
                    }
                }
            }

            //-Patni-Hemant/2010Feb15/Added/ACE-Phase#3/CDS Requirement
        }
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Continuousness Check is OK"));

    /* List Check */
    if (!m_selection_utiliy->CoilCompareList()) {

        MPLUSERRORUSERRESPONSE_e l_response = CCoilSelectionUtility::DisplayMessageinWarningDialog(
                CPQMUtility::GetMultiLingualString(_T("IDS_CS_WAR_CHECK_LIST")),
                SCAN_WARN_OK_CANCEL_BUTTONS);

        return (l_response == MPlusErrResponse_OK);
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("List Check is OK"));

    return TRUE;
}


void CCoilSelWin::JudgCoilPB(
    const bool f_flg
)
{

    LPCTSTR FUNC_NAME = _T("CCoilSelWin::JudgCoilPB");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_selection_utiliy) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));

        return;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections
    const int pressX = m_selection_utiliy->GetPressPoint().x ;
    const int pressY = m_selection_utiliy->GetPressPoint().y ;

    const int releaseX = m_selection_utiliy->GetReleasePoint().x ;
    const int releaseY = m_selection_utiliy->GetReleasePoint().y ;

    CRect l_drag_rect;
    l_drag_rect.left = pressX < releaseX ? pressX : releaseX ;
    l_drag_rect.right = pressX > releaseX ? pressX : releaseX ;
    l_drag_rect.top = pressY < releaseY ? pressY : releaseY ;
    l_drag_rect.bottom = pressY > releaseY ? pressY : releaseY ;

    int leftX = 0, rightX = 0, topY = 0, bottomY = 0;

    /* setX */
    if (pressX  <= releaseX) {
        leftX = pressX;
        rightX = releaseX;

    } else {
        leftX = releaseX;
        rightX = pressX;
    }

    /* setY */
    if (pressY  <= releaseY) {
        topY = pressY;
        bottomY = releaseY;

    } else {
        topY = releaseY;
        bottomY = pressY;
    }

    m_selection_utiliy->SetBeforePoint(CPoint(0, 0)) ;

    CoilSelectionSectionButton_t* section_buton_n  = NULL;

    for (int n = 0; n < MAX_SECTION_BUTTON; n++) {

        section_buton_n = m_selection_utiliy->GetSectionButton(n);

        /* X Condition */
        if (section_buton_n->position.rightX >= leftX && section_buton_n->position.leftX <= rightX) {
            /* Y Condition */
            if (section_buton_n->position.bottomY >= topY && section_buton_n->position.topY <= bottomY) {
                SectionCoilButtonUiOnCoilPB(n, f_flg);			/* CoilPB ON */

            } else {
                SectionCoilButtonUiOffCoilPB(n, f_flg);			/* CoilPB OFF */
            }

        } else {
            SectionCoilButtonUiOffCoilPB(n, f_flg);				/* CoilPB OFF */
        }
    }

    m_selection_utiliy->SetReleasePoint(CPoint(0, 0)) ;
}


//+Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : ResetHighlight
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
inline void CCoilSelWin::ResetHighlight(
)
{
    m_highlight_1 = -1;
    m_highlight_2 = -1;
}

//************************************Method Header************************************
// Method Name  : InitializeChannelMode
// Author       : PATNI/ Pavan
// Purpose      :
//***********************************************************************************
void CCoilSelWin::InitializeChannelMode()
{
    LPCTSTR FUNC_NAME = _T("CCoilSelWin::InitializeChannelMode");
    PQM_TRACE_SCOPED(FUNC_NAME);

    if (!m_cbaseaxislocatordialog_interface->GetIsChannelModeApplicable()) {
        m_btnCoilChannelMode.ShowWindow(SW_HIDE);
        m_channel_mode.ShowWindow(SW_HIDE);

    } else {
        m_channel_mode.ShowWindow(SW_SHOW);
        CString l_mode = CPQMUtility::GetMultiLingualString(_T("IDS_STATIC_CHANNEL_MODE"));
        m_channel_mode.SetWindowText(l_mode);

        int l_channel_mode = m_cbaseaxislocatordialog_interface->GetChannelModeSelection();

        switch (l_channel_mode) {
            case 0:
                // Or We can call normal menu click event
                l_mode = CPQMUtility::GetMultiLingualString(_T("IDS_CHANNEL_MODE_NORMAL"));
                m_btnCoilChannelMode.SetWindowText(l_mode); // TODO::Replace hard coded values by mentioning string in cat file

                SetSelectedChannelMode(CHANNEL_MODE_NORMAL);
                break;

            case 1:
                // Or We can call Max menu click event
                l_mode = CPQMUtility::GetMultiLingualString(_T("IDS_CHANNEL_MODE_MAX"));
                m_btnCoilChannelMode.SetWindowText(l_mode); // TODO::Replace hard coded values by mentioning string in cat file

                SetSelectedChannelMode(CHANNEL_MODE_MAX);
                break;

            default:
                l_mode = CPQMUtility::GetMultiLingualString(_T("IDS_CHANNEL_MODE_NORMAL"));
                m_btnCoilChannelMode.SetWindowText(l_mode); // If channel mode tag is not present in PQM protocol then on coil window, default value is normal.

                SetSelectedChannelMode(CHANNEL_MODE_NORMAL);
                break;
        }
    }
}
//-Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement

//+Patni-Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : GetStringForID
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CString CCoilSelWin::GetStringForID(
    const int f_msgstring_id
)const
{

    //Patni-Hemant/2010May6/Commented/JaFT/SM_PH3_CDR-CDS_CDR_07
    //CString l_msgstring = _T("") ;

    switch (f_msgstring_id) {

        case IDS_CS_MES_SELECT_SECTION :
            //Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_07
            return CPQMUtility::GetMultiLingualString(_T("IDS_CS_MES_SELECT_SECTION"));
            break ;

        case IDS_CS_MES_CHANNEL_OVER :
            //Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_07
            return CPQMUtility::GetMultiLingualString(_T("IDS_CS_MES_CHANNEL_OVER"));
            break ;

        case IDS_CS_MES_BAD_FIELD :
            //Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_07
            return CPQMUtility::GetMultiLingualString(_T("IDS_CS_MES_BAD_FIELD"));
            break ;

        case IDS_CS_MES_CONTINUOUSNESS :
            //Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_07
            return CPQMUtility::GetMultiLingualString(_T("IDS_CS_MES_CONTINUOUSNESS"));
            break ;

        case IDS_CS_MES_OK :
            //Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_07
            return CPQMUtility::GetMultiLingualString(_T("IDS_CS_MES_OK"));
            break ;

        case IDS_CS_MES_COMBINE :
            //Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_07
            return CPQMUtility::GetMultiLingualString(_T("IDS_CS_MES_COMBINE"));
            break ;

        case IDS_CS_MES_SELECT_COILLIST :
            //Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_07
            return CPQMUtility::GetMultiLingualString(_T("IDS_CS_MES_SELECT_COILLIST"));
            break ;

        case IDS_CS_MES_UPPER_1 :
            //Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_07
            return CPQMUtility::GetMultiLingualString(_T("IDS_CS_MES_UPPER_1"));
            break ;

        case IDS_CS_MES_UPPER_2 :
            //Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_07
            return CPQMUtility::GetMultiLingualString(_T("IDS_CS_MES_UPPER_2"));
            break ;

            //+Patni-Hemant/2010Mar26/Added/ACE-Phase#3/CDS Requirement
        case IDS_CS_MES_NO_POSITION :
            //Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_07
            return CPQMUtility::GetMultiLingualString(_T("IDS_CS_MES_NO_POSITION"));
            break ;

        case IDS_CS_MES_SN_DOWN :
            //Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_07
            return CPQMUtility::GetMultiLingualString(_T("IDS_CS_MES_SN_DOWN"));
            break ;

        case IDS_CDS_MES_AFTER_CDS :
            //Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_07
            return CPQMUtility::GetMultiLingualString(_T("IDS_CDS_MES_AFTER_CDS"));
            break ;

            //-Patni-Hemant/2010Mar26/Added/ACE-Phase#3/CDS Requirement

        default :
            //Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_07
            return _T("");
    }


    //+Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_05
    //return l_msgstring ;
    return _T("");
    //-Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_05
}
//-Patni-Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement

void CCoilSelWin::SetMultilingual()
{

    // other controls
    CString l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_BUTTON_OK"));
    HWND l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDOK);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;

    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_CANCEL_BUTTON"));
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDCANCEL);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;


    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_BUTTON_INVOKE_COIL"));
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDC_BUTTON_INVOKE_COIL);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_STATIC_CHANNEL_MODE"));
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDC_STATIC_CHANNEL_MODE);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_STATIC_CHANNEL_MODE"));
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDC_BUTTON_COIL_CHANNEL_MODE);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;
}
// Added By KT/ACE-2/11-06-2009/Multilingualisation

int CCoilSelWin::GetGUISectionWidthDefault() const
{

    CRect l_rect;
    GetDlgItem(IDC_NUMBER_1)->GetWindowRect(&l_rect);
    ScreenToClient(&l_rect);

    return l_rect.Width();
}

COUCH_POSITION CCoilSelWin::GetCouchAxis() const
{
    return m_couch_position;
}

//************************************Method Header************************************
// Method Name  : SetWaitCursor
// Author       : iGate
// Purpose      :
//***********************************************************************************
void CCoilSelWin::SetWaitCursor(
    const bool f_show_wait_cursor
)
{
    m_show_wait_cursor_for_coilselwin = f_show_wait_cursor;

    if (m_p_dialog_coil_list && ::IsWindow(m_p_dialog_coil_list->m_hWnd)) {
        m_p_dialog_coil_list->SetWaitCursor(f_show_wait_cursor);
    }

}
CoilSelectionGlobal_t* CCoilSelWin::GetCoilSelectionGlobal(
)const
{
    LPCTSTR FUNC_NAME = _T("CCoilSelWin::GetCoilSelectionGlobal");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_selection_utiliy) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));

        return NULL;

    } else {
        //-Patni-DKH/2009Aug24/Modified/cpp test corrections
        return m_selection_utiliy->GetCoilSelectionGlobal() ;
    }//Patni-DKH/2009Aug24/Modified/cpp test corrections
}


//***************************Method Header*************************************
//Method Name    : SetButtonStyle()
//Author         : PATNI /
//Purpose        :
//*****************************************************************************
BOOL CCoilSelWin::SetButtonStyle(
)
{
    CString sFontName;
    sFontName.LoadString(IDS_FONT_NAME);

    int iFontSize = AFX_IDS_FONTSIZE;

    m_btnSCRight.SetButtonType(TUSBUTTON_ROUND_NONE);
    m_btnSCRight.SetPrimaryTextFont(sFontName, iFontSize);
    //+Patni-Hemant/2010Mar10/Modified/ACE-Phase#3/CDS Requirement
    //SetBitmapForButton(m_btnSCRight, (m_couch_position == COUCH_POSITION_LEFT) ? IDB_BITMAP_SPINE_RIGHT : IDB_BITMAP_SPINE_LEFT_RIGHTDIALOG);
    SetBitmapForButton(m_btnSCRight, (m_couch_position == COUCH_POSITION_LEFT) ? IDB_BITMAP_MOVE_FREE_RIGHT : IDB_BITMAP_MOVE_FREE_LEFT);
    //-Patni-Hemant/2010Mar10/Modified/ACE-Phase#3/CDS Requirement

    //+Patni-HEMANT/ADDED On 8/27/2009 12:28:15 PM/ Spine Coil Req
    m_btnSCMiddle.SetButtonType(TUSBUTTON_ROUND_NONE);
    m_btnSCMiddle.SetPrimaryTextFont(sFontName, iFontSize);
    SetBitmapForButton(m_btnSCMiddle, IDB_BITMAP_SPINE_MIDDLE_DIALOG);
    //-Patni-HEMANT/ADDED On 8/27/2009 12:28:15 PM/ Spine Coil Req

    m_btnSCLeft.SetButtonType(TUSBUTTON_ROUND_NONE);
    m_btnSCLeft.SetPrimaryTextFont(sFontName, iFontSize);
    //+Patni-Hemant/2010Mar10/Modified/ACE-Phase#3/CDS Requirement
    //SetBitmapForButton(m_btnSCLeft, (m_couch_position == COUCH_POSITION_LEFT) ? IDB_BITMAP_SPINE_LEFT : IDB_BITMAP_SPINE_RIGHT_RIGHTDIALOG);
    SetBitmapForButton(m_btnSCLeft, (m_couch_position == COUCH_POSITION_LEFT) ? IDB_BITMAP_MOVE_FREE_LEFT : IDB_BITMAP_MOVE_FREE_RIGHT);
    //-Patni-Hemant/2010Mar10/Modified/ACE-Phase#3/CDS Requirement


    m_btnFCRight.SetButtonType(TUSBUTTON_ROUND_NONE);
    m_btnFCRight.SetPrimaryTextFont(sFontName, iFontSize);
    SetBitmapForButton(m_btnFCRight, (m_couch_position == COUCH_POSITION_LEFT) ? IDB_BITMAP_MOVE_FREE_RIGHT : IDB_BITMAP_MOVE_FREE_LEFT);

    m_btnFCLeft.SetButtonType(TUSBUTTON_ROUND_NONE);
    m_btnFCLeft.SetPrimaryTextFont(sFontName, iFontSize);
    SetBitmapForButton(m_btnFCLeft, (m_couch_position == COUCH_POSITION_LEFT) ? IDB_BITMAP_MOVE_FREE_LEFT : IDB_BITMAP_MOVE_FREE_RIGHT);


    iFontSize -= 1;
    m_btnGantry.SetButtonStyle(BS_PUSHBUTTON);
    m_btnGantry.SetButtonType(TUSBUTTON_ROUND_NONE);
    m_btnGantry.SetPrimaryTextFont(sFontName, iFontSize, true);

    //+Patni-Hemant/2009Dec25/Modified/MVC006387
    //SetColorButton(&m_btnGantry, RGB(0, 0, 0));
    SetEnableDisableButtonColor(m_btnGantry, false);
    //-Patni-Hemant/2009Dec25/Modified/MVC006387

    for (int i = 0; i < AFX_ID_LINEAR_MAX; i++) {
        m_btnL[i].SetButtonStyle(BS_PUSHBUTTON);
        m_btnL[i].SetButtonType(TUSBUTTON_ROUND_NONE);
        m_btnL[i].SetPrimaryTextFont(sFontName, iFontSize, true);
    }

    //Font size changed
    //iFontSize -= 3;
    sFontName = _T("Arial") ;
    iFontSize = AFX_IDS_FONTSIZE - 2  ;

    for (i = 0; i < MAX_SECTION_BUTTON; i++) {
        m_btnCouch[i].SetButtonStyle(BS_PUSHBUTTON);
        m_btnCouch[i].SetButtonType(TUSBUTTON_ROUND_NONE);
        m_btnCouch[i].SetPrimaryTextFont(sFontName, iFontSize , true);
    }

    //+Patni-HAR/2009Nov24/DeFT/IR-110
    for (i = 0; i < 4; i++) {
        m_btnCouch_sb[i].SetButtonStyle(BS_PUSHBUTTON);
        m_btnCouch_sb[i].SetButtonType(TUSBUTTON_ROUND_NONE);
        m_btnCouch_sb[i].SetPrimaryTextFont(sFontName, iFontSize , true);
    }

    //Font size changed
    //iFontSize -= 2;
    for (i = 0; i < 10; i++) {
        m_btnLabel[i].SetButtonType(TUSBUTTON_ROUND_CIRCLE);
        m_btnLabel[i].SetPrimaryTextFont(sFontName, iFontSize, true);
    }

    //-------------------------------------------------------------------------
    //Added by Hemant On 12/11/2008 11:17:28 AM
    sFontName = _T("Verdana") ;
    iFontSize = 13;
    m_btnCancel.SetPrimaryTextFont(sFontName, iFontSize);
    m_btnOK.SetPrimaryTextFont(sFontName, iFontSize);
    m_btnInvokeCoil.SetPrimaryTextFont(sFontName, iFontSize);
    //-------------------------------------------------------------------------

    return TRUE;
}

//************************************Method Header************************************
// Method Name  : SetBitmapForButton
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
BOOL CCoilSelWin::SetBitmapForButton(
    ButtonEx& button_object,
    const UINT bitmap_resource_id,
    const bool center_bitmap /*= true*/
)
{
    return CUIButtonUtility::GetInstance()->SetBitmapForButton(button_object, bitmap_resource_id, RGB(0, 255, 0), center_bitmap);
}


//+Patni-Hemant/2009Dec25/Modified/MVC006387
void CCoilSelWin::SetEnableDisableButtonColor(
    ButtonEx& f_button ,
    const bool f_enable /*= true*/,
    const bool f_redraw_button /*= false*/
)
{
    CUIButtonUtility::GetInstance()->SetEnableDisableButtonColor(f_button, f_enable, f_redraw_button);
}
//-Patni-Hemant/2009Dec25/Modified/MVC006387

//+Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CRect CCoilSelWin::GetMainImageRect(
)
{
    LPCTSTR FUNC_NAME = _T("CCoilSelWin::GetMainImageRect");

    CRect l_rect;
    m_main_image.GetWindowRect(&l_rect);

    BITMAP bitmap_object ;
    m_coil_main_image.GetBitmap(&bitmap_object);

    ScreenToClient(&l_rect);
    l_rect.left = l_rect.left;
    l_rect.top = l_rect.top;

    l_rect.right = l_rect.left + bitmap_object.bmWidth;
    l_rect.bottom = l_rect.top + bitmap_object.bmHeight;

    const int rect_width = 832;
    const int rect_height = 506;

    if ((l_rect.Width() < rect_width) || (l_rect.Height() < rect_height)) {

        l_rect.left = 8;
        l_rect.top =  5;
        l_rect.right = l_rect.left + rect_width;
        l_rect.bottom = l_rect.top + rect_height;

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Invalid Rect"));
    }

    return l_rect;
}
//-Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement


int CCoilSelWin::GetBaseIndexFor(
    const CoilTypes coiltypes
)const
{

    return CCoilSelectionUtility::GetBaseIndexFor(coiltypes) ;
}


int CCoilSelWin::GetBaseIndexForCoilButtonFor(
    const CoilTypes coiltypes
)const
{

    return CCoilSelectionUtility::GetBaseIndexForCoilButtonFor(coiltypes) ;
}


//************************************Method Header************************************
// Method Name  : GetBitmapPath
// Author       : PATNI/ Ravindra Acharya
// Purpose      : Retruns Path of Bitmap
//***********************************************************************************

//+Patni-Ravindra Acharya/2010May25/Added/IR-147
CString CCoilSelWin::GetBitmapPath(const int f_bitmap_id)const
{
    //variable Declaration & Initialization
    char* path = NULL ;

    // set path = MRMP_DATA
    if ((path = getenv("MRMP_DATA"))  == NULL) {
        path = "." ;
    }

    // Set path = (MRMP_DATA + PQM)
    CString l_path(path) ;
    l_path += _T("\\PQM\\") ;

    switch (f_bitmap_id) {
        case IDB_BITMAP_AXISLOCATOR_LEFT :
            return l_path + Left_AxisLocator ;

        case IDB_BITMAP_AXISLOCATOR_RIGHT:
            return l_path + Right_AxisLocator ;

        default:
            return l_path + Left_AxisLocator ;
    }

    return _T("") ;
}
//-Patni-Ravindra Acharya/2010May25/Added/IR-147




ButtonEx* CCoilSelWin::GetCoilButtonForIndex(
    const int f_coil_button_index
)
{

    int l_button_index = -1 ;

    switch (f_coil_button_index) {

        case 0 :

            return &m_btnGantry ;

        case 1 :
        case 2 : {


            l_button_index = f_coil_button_index - GetBaseIndexForCoilButtonFor(CoilType_L) ;
            return & (m_btnL[l_button_index]) ;

        }

        //+Patni-HAR/2009Nov20/Uncommented/IR-110
        case 3 :
        case 4 :
        case 5 :

        case 6 :
        case 7 :
        case 8 :
        case 9 : {
            l_button_index = f_coil_button_index - GetBaseIndexForCoilButtonFor(CoilType_A) ;
            return & (m_btnCouch_sb[l_button_index]) ;
        }
    }

    //-Patni-HAR/2009Nov20/Uncommented/IR-110
    return NULL ;
}


ButtonEx* CCoilSelWin::GetSectionButtonForIndex(
    const int f_coil_button_index
)
{

    if (f_coil_button_index >= 0 && f_coil_button_index < MAX_SECTION_BUTTON)
        return & m_btnCouch[f_coil_button_index] ;

    return NULL ;
}


void CCoilSelWin::SetWindowCaption(
)
{
    // Replace & with && in window's caption to display ampersand(&).
    m_window_caption.Replace(_T("&"), _T("&&"));

    SetWindowText(m_window_caption) ;

    if (m_p_dialog_coil_list) {
        m_p_dialog_coil_list->SetWindowCaption(m_window_caption) ;
    }
}

void CCoilSelWin::SetButtonAppearenceForLCoilButton(
    const LCoilItems lcoilitems
)
{

    LPCTSTR FUNC_NAME = _T("CCoilSelWin::SetButtonAppearenceForLCoilButton");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_selection_utiliy) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));

        return;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections
    CoilSelectionSectionButton_s* l_coil_button = m_selection_utiliy->GetCoilButton(
                lcoilitems + GetBaseIndexForCoilButtonFor(CoilType_L)
            );

    SetEnableDisableButtonColor(m_btnL[lcoilitems], l_coil_button->position.sw ? true : false);
    SetEnableDisableButtonColor(m_btnLabel[7 + lcoilitems], l_coil_button->position.sw ? true : false);

    m_btnL[lcoilitems].ShowWindow(l_coil_button->enable);

}

void CCoilSelWin::SetButtonAppearenceForGCoilButton(
    const GCoilItems gcoilitems
)
{
    LPCTSTR FUNC_NAME = _T("CCoilSelWin::SetButtonAppearenceForGCoilButton");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_selection_utiliy) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));

        return;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    CoilSelectionSectionButton_s* l_coil_button = m_selection_utiliy->GetCoilButton(GetBaseIndexForCoilButtonFor(CoilType_G));

    SetEnableDisableButtonColor(m_btnGantry, l_coil_button->position.sw ? true : false);
    SetEnableDisableButtonColor(m_btnLabel[8 + gcoilitems], l_coil_button->position.sw ? true : false);

    m_btnGantry.ShowWindow(l_coil_button->enable);
}

//+Patni-HAR/2009Nov24/DeFT/IR-110
void CCoilSelWin::SetButtonAppearenceForACoilButton(
    const ACoilItems f_a_coil_items
)
{

    CoilSelectionSectionButton_s* l_coil_button = m_selection_utiliy->GetCoilButton(f_a_coil_items + GetBaseIndexForCoilButtonFor(CoilType_A));

    if (l_coil_button->coil && l_coil_button->enable) {


        //m_btnL[(int)lcoilitems].SetWindowText(l_label);

    } else {
        m_btnCouch_sb[(int)f_a_coil_items].SetWindowText(_T(""));
    }

    SetEnableDisableButtonColor(m_btnCouch_sb[f_a_coil_items], l_coil_button->position.sw ? true : false);
    SetEnableDisableButtonColor(m_btnCouch_sb[7 + f_a_coil_items], l_coil_button->position.sw ? true : false);

    m_btnCouch_sb[f_a_coil_items].ShowWindow(l_coil_button->enable);
}
//-Patni-HAR/2009Nov24/DeFT/IR-110



void CCoilSelWin::SetWarningButtonColor(
    ButtonEx& f_button
)
{

    BUTTON_COLORS_t l_button_color = m_btnDefColor;

    l_button_color.crBrushFaceStart = RGB(255, 80, 132) ;
    l_button_color.crBrushFaceEnd = RGB(255, 150, 180) ;

    f_button.SetButtonColors(l_button_color);
}

//***************************Method Header*************************************
//Method Name    : SetColorButton()
//Author         : PATNI /
//Purpose        :
//*****************************************************************************
BOOL CCoilSelWin::SetColorButton(
    ButtonEx* pbtn,
    const COLORREF cColor
)
{
    BUTTON_COLORS_t btnColor;
    pbtn->GetButtonColors(btnColor);
    btnColor.crBrushFaceStart   = cColor;
    btnColor.crBrushFaceEnd	    = cColor;

    BUTTON_COLORS_t l_button_color;
    pbtn->GetButtonColors(l_button_color);

    if ((btnColor.crBrushFaceStart == l_button_color.crBrushFaceStart)
        && (btnColor.crBrushFaceEnd == l_button_color.crBrushFaceEnd)
       )
        return TRUE;

    pbtn->SetButtonColors(btnColor);
    return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CCoilSelWin message handlers



//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCoilSelWin::SetDialogRibbonHeight(
)
{

    CRect client_rect;
    GetClientRect(&client_rect);

    CRect ok_button_rect ;
    m_btnCancel.GetWindowRect(&ok_button_rect);
    ScreenToClient(&ok_button_rect);


    SetRibbonHeight(client_rect.bottom - ok_button_rect.top + RIBBON_GAP);
}



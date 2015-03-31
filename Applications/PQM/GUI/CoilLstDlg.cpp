//REDMINE_782_CPP_Test_Tool
//TMSC-REDMINE-782
//TMSC-REDMINE-781
//TMSC-Redmine-780
//TMSC-REDMINE-778
//TMSC-REDMINE-777
//redmine-779
//Redmine-774
// CoilLstDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CoilLstDlg.h"
#include <math.h>
#include <libStudyManager/UI/ZOrderLayerFactory.h>
#include <PQM/resource.h>
#include "DPSManager.h"
#include "UIButtonUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define RIBBON_GAP 5

using namespace MR::ACQ::SM::COMMON;
using MR::ACQ::SM::COMMON::UI::CZOrderDialog;
using MR::ACQ::SM::COMMON::UI::CZOrderLayerFactory;

/////////////////////////////////////////////////////////////////////////////
// CCoilLstDlg dialog

BEGIN_MESSAGE_MAP(CCoilLstDlg, CZOrderDialog)
    //{{AFX_MSG_MAP(CCoilLstDlg)
    ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
    ON_WM_CLOSE()
    ON_WM_SETCURSOR()
    ON_COMMAND_RANGE(ID_G_MENU_DYNAMIC, ID_G_MENU_DYNAMIC_MAX, OnGCoilMenu)
    ON_COMMAND_RANGE(ID_L_MENU_DYNAMIC, ID_L_MENU_DYNAMIC_MAX, OnLCoilMenu)
    ON_COMMAND_RANGE(ID_A_MENU_DYNAMIC, ID_A_MENU_DYNAMIC_MAX, OnACoilMenu)

    ON_COMMAND_RANGE(ID_PORT_MENU_DYNAMIC, ID_PORT_MENU_DYNAMIC_MAX,
                     OnPortMenu)
    ON_COMMAND_RANGE(ID_POS_MENU_DYNAMIC, ID_POS_MENU_DYNAMIC_MAX,
                     OnPosMenu)

    ON_CONTROL_RANGE(BN_CLICKED, IDC_BUTTON_PORT_1, IDC_BUTTON_PORT_7,
                     OnComboPort)
    ON_CONTROL_RANGE(BN_CLICKED, IDC_BUTTON_COIL_1, IDC_BUTTON_COIL_10,
                     OnComboCoil)
    ON_CONTROL_RANGE(BN_CLICKED, IDC_BUTTON_POS_1, IDC_BUTTON_POS_9,
                     OnComboPos)
    //}}AFX_MSG_MAP


END_MESSAGE_MAP()


CCoilLstDlg::CCoilLstDlg(CCoilSelectionUtility* selection_utiliy, CCoilSelWinInterface* ccoilselwininterface, CWnd* pParent /*=NULL*/)
    : CZOrderDialog(CZOrderLayerFactory::GetIselLayer(),
                    CDPSManager::GetInstance(), IDD_DIALOG_COIL_LST, pParent),
    m_ccoilselwininterface(ccoilselwininterface),
    m_selection_utiliy(selection_utiliy),
    m_window_caption(_T("")),
    pGPortMenu(NULL),
    pAPortMenu(NULL),
    pPosMenu(NULL),
    pPortMenu(NULL),
    ubtnID(0),
    iLIndex(0),
    iAIndex(0),
    m_show_wait_cursor_coillistdlg(false)
{
    //{{AFX_DATA_INIT(CCoilLstDlg)
    //}}AFX_DATA_INIT

    pLPortMenu[0]	= NULL;
    pLPortMenu[1]   = NULL;
}

CCoilLstDlg::~CCoilLstDlg()
{

    CleanUp();
}

void CCoilLstDlg::DisplayCoilWithConnectCoilCheck(
)
{
    LPCTSTR FUNC_NAME = _T("CCoilLstDlg::DisplayCoilWithConnectCoilCheck");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-Hemant/2010Mar5/Modified/ACE-Phase#3/CDS Requirement
    if (!m_selection_utiliy || !m_selection_utiliy->GetCoilSelectionGlobal()) {
        //-Patni-Hemant/2010Mar5/Modified/ACE-Phase#3/CDS Requirement

        ASSERT(FALSE) ;
        return ;
    }

    const int l_pqmmode = m_selection_utiliy->GetCoilSelectionGlobal()->config.pqmMode ;

    //moved to start as this function was not called due to some reasons./but
    //though the statement should have been executed...before
    //m_selection_utiliy->SetUIClassInterfaceForCoilListDialog(static_cast<CUIClassInterfaceForCoilListDialog*> (this) );

    //Added by Hemant On 10/15/2008 6:10:57 PM
    bool check_combine = true ;

    //+Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement
    status_t l_sts = COIL_S_NO_ERROR ;

    //-Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement
    if (l_pqmmode != K_SCHEDULING) {

        CString l_str = _T("");
        bool process_compare_flag = m_selection_utiliy->ProcessConnectCoilMsg(m_selection_utiliy->GetConnectCoil(), l_str)  ;

        //Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement
        UpdateHighlightColorsForPositionCombos();

        if (!process_compare_flag && !l_str.IsEmpty()) {

            ShowWindow(SW_SHOW);

            //Modified by Hemant . the checkcombine flag is to be taken
            //after OK or Cancel pressed..
            //DisplayCoilInformation(false , &check_combine);
            DisplayCoilInformation(false);

            UpdateMarkerWindows();

            //display the message box for user query...and do accordingly..

            //MessageBox(l_str, NULL, MB_OKCANCEL|MB_SYSTEMMODAL)


            //+Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement
            //CoilMismatchOKCancel(l_response == MPlusErrResponse_OK) ;
            l_sts = CoilMismatchOKCancel(
                        MPlusErrResponse_OK == CCoilSelectionUtility::DisplayMessageinWarningDialog(l_str, SCAN_WARN_OK_CANCEL_BUTTONS)
                    ) ;
            //-Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement

            //+Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement
            /*
            if(l_sts != COIL_S_OTHER) {
            //-Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement
            //added by Hemant - when multiple atlas coils are connected,
            //check combine was returning true.. before.. then I checked and found that
            //the check combine value is to be taken after OK or Cancel button
            //so I am getting the check combine value now..
            MessageBoxTypeEnum messageboxtypeenum = MessageBoxType_OK_CANCEL;
            //check_combine = m_selection_utiliy->CoilCheckList(TRUE, l_str, messageboxtypeenum) ;
            	//+Patni-Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement
            	//check_combine = m_selection_utiliy->CoilCheckList(TRUE, l_str, messageboxtypeenum) ? true : false;
            	check_combine = (m_selection_utiliy->CoilCheckList(TRUE, l_str, messageboxtypeenum) == COIL_S_NO_ERROR)? true : false;
            	//-Patni-Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement
            //+Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement

            } else{
            	check_combine = false ;
            }
            */
            check_combine = (l_sts == COIL_S_NO_ERROR);

            UpdateHighlightColorsForPositionCombos();

            //-Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement
            //
            //end of addition by hemant
            //+Patni-AD/2009Jun10/Added/SU14 IR#72

        } else {

            ShowWindow(SW_SHOW);
        }

        //-Patni-AD/2009Jun10/Added/SU14 IR#72
        //    } else{
        //
        //        ShowWindow(SW_SHOW);
        //
        //        DisplayCoilInformation();
        //        UpdateMarkerWindows();
    }

    //If any coil is avialbel in selected... close the dialog..
    //
    CoilSelectionSelectCoil_t* l_local = NULL;	//Patni-PP/2009May28/Modified/SU14 CPP test initialization corrections
    bool is_single_coil_available = false ;

    for (int coil_button = 0 ; coil_button < MAX_COIL_BUTTON ; coil_button ++) {

        l_local = m_selection_utiliy->GetSelectedCoil(coil_button);

        if (l_local) {

            if (l_local->coil && l_local->coil->label) {

                is_single_coil_available = true ;
            }
        }

        if (is_single_coil_available)
            break ;
    }

    if (l_pqmmode == K_SCHEDULING) {

        if (!is_single_coil_available) {

            ShowWindow(SW_SHOW);
            DisplayCoilInformation();
            UpdateMarkerWindows();
        }
    }

    //+Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement
    //CoilUiListDlgOkCB() ;
    if ((l_sts == COIL_S_OTHER) && (l_sts == COIL_S_NO_ERROR)) {
        m_selection_utiliy->CoilSetCoilButton();
        m_selection_utiliy->CoilMapCoilButton();
    }

    //-Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement

    //Modified by Hemant - when check combine is failed then don't
    //close the window..
    //if(is_single_coil_available) {
    if (is_single_coil_available && check_combine) {

        CleanUp();
        DestroyWindow();
    }

    m_selection_utiliy->ResetAllMarkerSigns();
    UpdateMarkerWindows();
}


void CCoilLstDlg::DisplayCoilInformation(
    const bool selected_only /*= false*/ ,
    bool* check_combine /*= NULL*/,
    const bool f_reset_float_pos /*= false*/
)
{
    LPCTSTR FUNC_NAME = _T("CCoilLstDlg::DisplayCoilInformation");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-Dhanesh/2009Aug20/Modified/cpp test corrections
    if (NULL == m_selection_utiliy) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));
        return;

    }

    //-Patni-Dhanesh/2009Aug20/Modified/cpp test corrections

    CoilSelectionSelectCoil_t* l_selected_coil = NULL, *l_applied_coil = NULL;

    for (int coil_button = 0 ; coil_button < MAX_COIL_BUTTON ; coil_button ++) {

        l_selected_coil = m_selection_utiliy->GetSelectedCoil(coil_button);

        l_applied_coil = m_selection_utiliy->GetAppliedCoil(coil_button);

        if (NULL == l_selected_coil || NULL == l_applied_coil) {
            continue;
        }

        if (l_selected_coil->coil &&
            l_applied_coil->coil &&
            (strcmp(l_selected_coil->coil->label, l_applied_coil->coil->label) == 0) &&
            l_selected_coil->port == l_applied_coil->port) {

            l_selected_coil->position = l_applied_coil->position;
            l_selected_coil->float_position = l_applied_coil->float_position;
        }

        DisplayCoilFor(l_selected_coil, coil_button);

        if (f_reset_float_pos) {
            l_selected_coil->float_position = (float)l_selected_coil->position;
        }
    }

    //+Patni-Dhanesh/2009Aug20/Modified/cpp test corrections
    //Moved the code snippet inside the if condition
    //Added by Hemant On 10/15/2008 5:47:31 PM
    if (check_combine) {

        //+Patni-Hemant/2010Mar4/Commented/ACE-Phase#3/CDS Requirement
        //CString l_str(_T(""));
        //MessageBoxTypeEnum messageboxtypeenum = MessageBoxType_OK_CANCEL;
        //-Patni-Hemant/2010Mar4/Commented/ACE-Phase#3/CDS Requirement

        //*check_combine = m_selection_utiliy->CoilCheckList(TRUE, l_str, messageboxtypeenum) ;

        //+Patni-Hemant/2010Feb15/Modified/ACE-Phase#3/CDS Requirement
        //*check_combine = m_selection_utiliy->CoilCheckList(TRUE, l_str, messageboxtypeenum) ? true : false;
        *check_combine = (m_selection_utiliy->CoilCheckList(FALSE) != COIL_S_NO_ERROR) ? true : false;
        //-Patni-Hemant/2010Feb15/Modified/ACE-Phase#3/CDS Requirement
    }

    m_selection_utiliy->CheckPosition(m_selection_utiliy->GetSelectedCoil());

}

void CCoilLstDlg::EnableDisableDialogState(const BOOL f_state)
{

    if (::IsWindow(m_hWnd)) {
        m_btnCancel.EnableWindow(f_state);
        m_btnOK.EnableWindow(f_state);
        m_btnApply.EnableWindow(f_state);
        ShowCloseButton(f_state == TRUE);
    }
}

void CCoilLstDlg::SetWindowCaption(
    const CString& f_window_caption
)
{
    m_window_caption  = f_window_caption ;
    SetWindowCaption();//Added by Hemant On 2/27/2009 3:02:41 PM

}

void CCoilLstDlg::SetWindowCaption(
)
{
    if (IsWindow(m_hWnd)) {
        SetWindowText(m_window_caption) ;
    }
}

//************************************Method Header************************************
// Method Name  : SetWaitCursor
// Author       : iGate
// Purpose      :
//***********************************************************************************
void CCoilLstDlg::SetWaitCursor(const bool f_show_wait_cursor)
{
    m_show_wait_cursor_coillistdlg = f_show_wait_cursor;

}
void CCoilLstDlg::DoDataExchange(CDataExchange* pDX)
{
    CZOrderDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CCoilLstDlg)
    DDX_Control(pDX, IDOK, m_btnOK);
    DDX_Control(pDX, IDCANCEL, m_btnCancel);
    DDX_Control(pDX, IDC_LIST_PORT, m_lstPort);
    DDX_Control(pDX, IDC_BUTTON_PORT_1, m_btnPort[0]);
    DDX_Control(pDX, IDC_BUTTON_PORT_2, m_btnPort[1]);
    DDX_Control(pDX, IDC_BUTTON_PORT_3, m_btnPort[2]);
    DDX_Control(pDX, IDC_BUTTON_PORT_4, m_btnPort[3]);
    DDX_Control(pDX, IDC_BUTTON_PORT_5, m_btnPort[4]);
    DDX_Control(pDX, IDC_BUTTON_PORT_6, m_btnPort[5]);
    DDX_Control(pDX, IDC_BUTTON_PORT_7, m_btnPort[6]);
    DDX_Control(pDX, IDC_BUTTON_COIL_1, m_btnCoil[0]);
    DDX_Control(pDX, IDC_BUTTON_COIL_2, m_btnCoil[1]);
    DDX_Control(pDX, IDC_BUTTON_COIL_3, m_btnCoil[2]);
    DDX_Control(pDX, IDC_BUTTON_COIL_4, m_btnCoil[3]);
    DDX_Control(pDX, IDC_BUTTON_COIL_5, m_btnCoil[4]);
    DDX_Control(pDX, IDC_BUTTON_COIL_6, m_btnCoil[5]);
    DDX_Control(pDX, IDC_BUTTON_COIL_7, m_btnCoil[6]);
    DDX_Control(pDX, IDC_BUTTON_COIL_8, m_btnCoil[7]);
    DDX_Control(pDX, IDC_BUTTON_COIL_9, m_btnCoil[8]);
    DDX_Control(pDX, IDC_BUTTON_COIL_10, m_btnCoil[9]);
    DDX_Control(pDX, IDC_BUTTON_POS_1, m_btnPos[0]);
    DDX_Control(pDX, IDC_BUTTON_POS_2, m_btnPos[1]);
    DDX_Control(pDX, IDC_BUTTON_POS_3, m_btnPos[2]);
    DDX_Control(pDX, IDC_BUTTON_POS_4, m_btnPos[3]);
    DDX_Control(pDX, IDC_BUTTON_POS_5, m_btnPos[4]);
    DDX_Control(pDX, IDC_BUTTON_POS_6, m_btnPos[5]);
    DDX_Control(pDX, IDC_BUTTON_POS_7, m_btnPos[6]);
    DDX_Control(pDX, IDC_BUTTON_POS_8, m_btnPos[7]);
    DDX_Control(pDX, IDC_BUTTON_POS_9, m_btnPos[8]);

    DDX_Control(pDX, IDC_BUTTON_APPLY, m_btnApply);

    DDX_Control(pDX,  IDC_STATIC1  ,  m_marker_sign_statics[0]);
    DDX_Control(pDX,  IDC_STATIC2  ,  m_marker_sign_statics[1]);
    DDX_Control(pDX,  IDC_STATIC3  ,  m_marker_sign_statics[2]);
    DDX_Control(pDX,  IDC_STATIC4  ,  m_marker_sign_statics[3]);
    DDX_Control(pDX,  IDC_STATIC5  ,  m_marker_sign_statics[4]);
    DDX_Control(pDX,  IDC_STATIC6  ,  m_marker_sign_statics[5]);
    DDX_Control(pDX,  IDC_STATIC7  ,  m_marker_sign_statics[6]);
    DDX_Control(pDX,  IDC_STATIC8  ,  m_marker_sign_statics[7]);
    DDX_Control(pDX,  IDC_STATIC9  ,  m_marker_sign_statics[8]);
    DDX_Control(pDX,  IDC_STATIC10 ,  m_marker_sign_statics[9]);


    //}}AFX_DATA_MAP
}


BOOL CCoilLstDlg::OnInitDialogImpl()
{
    LPCTSTR FUNC_NAME = _T("CCoilLstDlg::OnInitDialogImpl");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-Dhanesh/2009Aug24/Modified/cpp test corrections
    if (NULL == m_selection_utiliy) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));

        return FALSE;
    }

    //-Patni-Dhanesh/2009Aug24/Modified/cpp test corrections



    // TODO: Add extra initialization here
    SetMultilingual(); // Added By KT/ACE-2/11-06-2009/Multilingualisation
    m_selection_utiliy->SetUIClassInterfaceForCoilListDialog(static_cast<CUIClassInterfaceForCoilListDialog*>(this));

    m_btnPos[0].GetButtonColors(m_btnDefColor);

    for (int static_index = 0; static_index < MAX_COIL_BUTTON ; static_index++) {

        SetColorForMarkerStatic(static_index) ;
    }

    SetButtonFont();

    CreateCoilMenu();

    SetDialogRibbonHeight();

    CRect l_window_rect;
    GetWindowRect(&l_window_rect) ;
    const CPoint l_actual_topleft(611, 556) ;
    l_window_rect.OffsetRect(l_actual_topleft - l_window_rect.TopLeft()) ;

    SetWindowPos(NULL, l_window_rect.left, l_window_rect.top, l_window_rect.Width() , l_window_rect.Height() , SWP_NOSIZE | SWP_NOZORDER) ;
    SetTitleBarHeight(24) ;
    SetFont(_T("Arial"), 10) ;

    EnableDisableDialogState(FALSE);

    InvalidateRect(l_window_rect) ;
    UpdateWindow();

    SetWindowCaption();

    return CZOrderDialog::OnInitDialogImpl(false); // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


//****************************Method Header************************************
//Method Name   :OnOK()
//Author        :PATNI / NJ
//Purpose       :
//*****************************************************************************
void CCoilLstDlg::OnOK()
{
    LPCTSTR FUNC_NAME = _T("CCoilLstDlg::OnOK");
    PQM_TRACE_SCOPED(FUNC_NAME);

    if (!ApplyCoilInformation())
        return ;


    CleanUp();
    DestroyWindow();
}

//****************************Method Header************************************
//Method Name   :OnCancel()
//Author        :PATNI / NJ
//Purpose       :
//*****************************************************************************
void CCoilLstDlg::OnCancel()
{
    LPCTSTR FUNC_NAME = _T("CCoilLstDlg::OnCancel");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-Dhanesh/2009Aug24/Modified/cpp test corrections
    if (NULL == m_selection_utiliy) {
        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));
        return;
    }

    //-Patni-Dhanesh/2009Aug24/Modified/cpp test corrections

    ClearSelectedCoilInformation();
    m_selection_utiliy->SetPqmListCoil(m_selection_utiliy->GetterToPqmListCoil(), false);
    m_selection_utiliy->SetAppliedCoil(true);

    CleanUp();

    DestroyWindow();
}

//****************************Method Header************************************
//Method Name   :OnButtonApply()
//Author        :PATNI / NJ
//Purpose       :
//*****************************************************************************
void CCoilLstDlg::OnButtonApply(
)
{
    LPCTSTR FUNC_NAME = _T("CCoilLstDlg::OnButtonApply");
    PQM_TRACE_SCOPED(FUNC_NAME);

    ApplyCoilInformation();
}
//************************************Method Header************************************
// Method Name  : OnSetCursor
// Author       : iGate
// Purpose      :
//***********************************************************************************
BOOL CCoilLstDlg::OnSetCursor(CWnd* pWnd,
                              UINT nHitTest,
                              UINT message)
{
    if (m_show_wait_cursor_coillistdlg) {
        ::SetCursor(::LoadCursor(NULL, IDC_WAIT));
        return TRUE ;
    }

    return CDialog::OnSetCursor(pWnd, nHitTest, message);

}
//************************************Method Header************************************
// Method Name  :PreTranslateMessageImpl
// Author       :iGATE
// Purpose      :
//***********************************************************************************
BOOL CCoilLstDlg::PreTranslateMessageImpl(
    MSG* pMsg
)
{
    if (m_show_wait_cursor_coillistdlg) {
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

    return CZOrderDialog::PreTranslateMessageImpl(pMsg);;
}

//****************************Method Header************************************
//Method Name   :OnComboPos()
//Author        :PATNI / NJ
//Purpose       :
//*****************************************************************************
void CCoilLstDlg::OnComboPos(UINT uPosId)
{
    const int iIndex = uPosId - IDC_BUTTON_POS_1;

    if (iIndex == 7 || iIndex == 8) {
        return;
    }

    ubtnID = uPosId;
    CString sbtnText = _T("");
    m_btnCoil[iIndex].GetWindowText(sbtnText);

    CoilSelectionSelectCoil_s* l_coil_struct = m_selection_utiliy->GetSelectedCoil(iIndex);

    if (l_coil_struct && l_coil_struct->coil) {

        //+Patni-Hemant/2010Feb26/Added/ACE-Phase#3/CDS Requirement
        //CreatePosMenu(l_coil_struct->coil->position);
        CreatePosMenu(l_coil_struct->coil->position, l_coil_struct);
        //-Patni-Hemant/2010Feb26/Added/ACE-Phase#3/CDS Requirement
        DisplayMenu(pPosMenu, uPosId);

        //preparing click ids..
        PrepareButtonClickIDAndCoilTypeClickedID(uPosId) ;
    }
}

//****************************Method Header************************************
//Method Name   :OnComboCoil()
//Author        :PATNI / NJ
//Purpose       :
//*****************************************************************************
void CCoilLstDlg::OnComboCoil(UINT uCoilId)
{
    LPCTSTR FUNC_NAME = _T("CCoilLstDlg::OnComboCoil");
    PQM_TRACE_SCOPED(FUNC_NAME);
    PopupMenuEx* pPQMMenu = NULL;

    iAIndex = -1;

    switch (uCoilId) {
        case IDC_BUTTON_COIL_10:

            //Patni-DKH/2009Aug24/Modified/cpp test corrections
            if (NULL != pGPortMenu) {
                pPQMMenu = pGPortMenu;
                //+Patni-DKH/2009Aug24/Modified/cpp test corrections

            } else {

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("pGPortMenu pointer is NULL"));

            }

            //Patni-DKH/2009Aug24/Modified/cpp test corrections
            break;

        case IDC_BUTTON_COIL_8:

            //Patni-DKH/2009Aug24/Modified/cpp test corrections
            if (NULL != pLPortMenu[0]) {
                pPQMMenu = pLPortMenu[0];
                iLIndex = 0;
                //+Patni-DKH/2009Aug24/Modified/cpp test corrections

            } else {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("pLPortMenu[0] pointer is NULL"));
            }

            //-Patni-DKH/2009Aug24/Modified/cpp test corrections

            break;

        case IDC_BUTTON_COIL_9:

            //Patni-DKH/2009Aug24/Modified/cpp test corrections
            if (NULL != pLPortMenu[1]) {
                pPQMMenu = pLPortMenu[1];
                iLIndex = 1;
                //+Patni-DKH/2009Aug24/Modified/cpp test corrections

            } else {

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("pLPortMenu[1] pointer is NULL"));
            }

            //-Patni-DKH/2009Aug24/Modified/cpp test corrections
            break;

        case IDC_BUTTON_COIL_1:
        case IDC_BUTTON_COIL_2:
        case IDC_BUTTON_COIL_3:
        case IDC_BUTTON_COIL_4:
        case IDC_BUTTON_COIL_5:
        case IDC_BUTTON_COIL_6:
        case IDC_BUTTON_COIL_7:

            pPQMMenu = pAPortMenu;//[0];
            iAIndex = (uCoilId - IDC_BUTTON_COIL_1);
    }

    //Choose the btn ID
    ubtnID = uCoilId;

    if (iAIndex != -1) {
        UpdateACoilMenu();
        pPQMMenu = pAPortMenu ;
    }

    if (pPQMMenu)
        DisplayMenu(pPQMMenu, ubtnID);
}


//****************************Method Header************************************
//Method Name   :OnComboPort()
//Author        :PATNI / NJ
//Purpose       :
//*****************************************************************************
void CCoilLstDlg::OnComboPort(UINT uPortId)
{
    ubtnID = uPortId;

    PreparePortMenu();

    DisplayMenu(pPortMenu, uPortId);

    //preparing click ids..
    PrepareButtonClickIDAndCoilTypeClickedID(uPortId, true) ;
}


void CCoilLstDlg::OnACoilMenu(
    UINT uCoilMenuID
)
{
    LPCTSTR FUNC_NAME = _T("CCoilLstDlg::OnACoilMenu");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //-Patni-Dhanesh/2009Aug24/Modified/cpp test corrections
    if (NULL == m_selection_utiliy) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));
        return;
    }

    //-Patni-Dhanesh/2009Aug24/Modified/cpp test corrections

    //if the new item selected is empty string then, reset that A coil index item..

    const int act_coil_index_to_update = iAIndex ;
    CoilSelectionSelectCoil_t* l_current_coil = m_selection_utiliy->GetSelectedCoil(act_coil_index_to_update);
    CoilSelectionSelectCoil_t l_prev_coil = *l_current_coil;

    CString prev_string_value = _T("") ;

    if (l_current_coil && l_current_coil->coil)
        prev_string_value = l_current_coil->coil->label;

    //we have reset the coil ..
    ResetCoilDataForDisplayInformation(l_current_coil);

    CString sMenuText;
    pAPortMenu->GetMenuString(uCoilMenuID, sMenuText, MF_BYCOMMAND);
    sMenuText.TrimLeft();
    sMenuText.TrimRight();

    if (!sMenuText.IsEmpty()) {

        //we have selected the coil..

        CoilSelectionCoil_t* l_selected_coil = GetSelectionCoilFromCouchDataFor(sMenuText);

        if (l_selected_coil) {

            //we have set the coil.. other infor is reseted preveously
            l_current_coil->coil = l_selected_coil;

            if (l_selected_coil->position[0] != -1) {

                //if its not free coil, then apply the first default position..
                l_current_coil->position = l_selected_coil->position[0] ;
                l_current_coil->float_position = (float)l_current_coil->position;
            }

            //+Patni-Hemant/2010Mar31/Added/ACE-Phase#3/CDS Requirement
            if ((l_selected_coil->position[0] == -1)
                && (l_selected_coil->position[1] != 0)
               ) {
                l_current_coil->position = l_selected_coil->position[1] ;
                l_current_coil->float_position = (float)l_current_coil->position;
            }

            //-Patni-Hemant/2010Mar31/Added/ACE-Phase#3/CDS Requirement
        }
    }

    //we have coil data in l_current_coil . not display caccordingly..
    DisplayACoilFrom(l_current_coil, (ACoilItems) act_coil_index_to_update);

    if (!prev_string_value.IsEmpty())
        l_prev_coil.coil = GetSelectionCoilFromCouchDataFor(prev_string_value) ;

    //+Patni-Hemant/2009Mar03/Modify/ACE-1/Internal defect
    //ShowConflictsFor(l_current_coil, &l_prev_coil , act_coil_index_to_update);
    ShowConflictsFor(NULL, &l_prev_coil , act_coil_index_to_update);
    ShowConflictsFor(l_current_coil, NULL, act_coil_index_to_update);
    //-Patni-Hemant/2009Mar03/Modify/ACE-1/Internal defect
    //

    //now following will show the conflcits...with red marking and etc..
    //or will reset the combos of others

    //now this may happen that the selected coils may have
    //+Patni-Hemant/2010Mar8/Added/ACE-Phase#3/CDS Requirement
    m_selection_utiliy->CheckCombine() ;
    //-Patni-Hemant/2010Mar8/Added/ACE-Phase#3/CDS Requirement

    UpdateHighlightColorsForPositionCombos();
}


//****************************Method Header************************************
//Method Name   :OnLCoilMenu()
//Author        :PATNI / NJ
//Purpose       :
//*****************************************************************************
void CCoilLstDlg::OnLCoilMenu(
    UINT uID
)
{
    LPCTSTR FUNC_NAME = _T("CCoilLstDlg::OnLCoilMenu");

    CString sMenuText;

    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_selection_utiliy) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));

        return;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    //Get the menu text
    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == pLPortMenu) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("pLPortMenu pointer is NULL"));

        return;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    pLPortMenu[iLIndex]->GetMenuString(uID, sMenuText, MF_BYCOMMAND);

    CoilSelectionCoil_t* l_coil_struct = NULL;
    CoilSelectionSelectCoil_t l_local;
    memset(&l_local, 0, sizeof(CoilSelectionSelectCoil_t));

    CString l_label = _T("");

    DllT* l_linear_head = m_selection_utiliy->GetHead();//GetLinearHead(iLIndex);

    for (DllT* l_trav = l_linear_head ; l_trav != NULL; l_trav = l_trav->next) {

        l_coil_struct = (CoilSelectionCoil_t*)l_trav->item;
        l_label = l_coil_struct->label ;

        if (!l_label.CompareNoCase(sMenuText)) {
            l_local.coil = l_coil_struct;
            break;

        } else {

            continue;
        }
    }

    const int act_coil_index_to_update = (iLIndex + GetBaseIndexFor(CoilType_L));

    m_selection_utiliy->SetSelectedCoil(&l_local, act_coil_index_to_update);

    // + 10F 10F problem AMT
    m_selection_utiliy->SetPortForLinerCoil();

    // - 10F 10F problem AMT

    UpdateLCoilMenu(sMenuText, uID);

    //+Patni-Hemant/2010Mar8/Added/ACE-Phase#3/CDS Requirement
    m_selection_utiliy->CheckCombine() ;

    //-Patni-Hemant/2010Mar8/Added/ACE-Phase#3/CDS Requirement
}


//****************************Method Header************************************
//Method Name   :OnGCoilMenu()
//Author        :PATNI / NJ
//Purpose       :
//*****************************************************************************
void CCoilLstDlg::OnGCoilMenu(
    UINT uID
)
{
    LPCTSTR FUNC_NAME = _T("CCoilLstDlg::OnGCoilMenu");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_selection_utiliy) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));
        return;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    //Get the menu text
    CString sMenuText;
    pGPortMenu->GetMenuString(uID, sMenuText, MF_BYCOMMAND);

    CoilSelectionCoil_t* l_coil_struct = NULL;
    CoilSelectionSelectCoil_t l_local;
    memset(&l_local, 0, sizeof(CoilSelectionSelectCoil_t));
    CString l_label = _T("");

    DllT* l_trestle_head = m_selection_utiliy->GetHead();

    for (DllT* l_trav = l_trestle_head ; l_trav != NULL; l_trav = l_trav->next) {

        l_coil_struct = (CoilSelectionCoil_t*)l_trav->item;
        l_label = l_coil_struct->label;

        if (!l_label.CompareNoCase(sMenuText)) {
            l_local.coil = l_coil_struct;
            break;

        } else {
            continue;
        }
    }

    m_selection_utiliy->SetSelectedCoil(&l_local, 9);

    UpdateGCoilMenu(sMenuText, uID);

    //+Patni-Hemant/2010Feb15/Modified/ACE-Phase#3/CDS Requirement
    ////Added by Hemant On 10/15/2008 5:39:12 PM
    //CString l_str(_T(""));
    //MessageBoxTypeEnum messageboxtypeenum = MessageBoxType_OK_CANCEL;
    //
    //
    //    if (!m_selection_utiliy->CoilCheckList(TRUE, l_str, messageboxtypeenum)) {
    //
    //    }
    m_selection_utiliy->CheckCombine() ;
    //-Patni-Hemant/2010Feb15/Modified/ACE-Phase#3/CDS Requirement
}


//****************************Method Header************************************
//Method Name   :OnPortMenu()
//Author        :PATNI / NJ
//Purpose       :
//*****************************************************************************
void CCoilLstDlg::OnPortMenu(
    UINT uPortID
)
{
    LPCTSTR FUNC_NAME = _T("CCoilLstDlg::OnPortMenu");
    PQM_TRACE_SCOPED(FUNC_NAME);

    CString sMenuText;
    pPortMenu->GetMenuString(uPortID, sMenuText, MF_BYCOMMAND);

    sMenuText.TrimLeft();
    sMenuText.TrimRight();

    if (sMenuText.IsEmpty()) {
        SetColorButton((ButtonEx*)GetDlgItem(ubtnID), RGB(255, 0, 0));

    } else {
        SetDefaultColor((ButtonEx*)GetDlgItem(ubtnID)) ;
    }

    //Set the focus to coil combo
    m_btnCoil[(ubtnID - IDC_BUTTON_PORT_1)].SetFocus();

    UpdatePortMenu(sMenuText, uPortID);

    CoilSelectionSelectCoil_t* l_local = m_selection_utiliy->GetSelectedCoil(iAIndex) ;

    if (sMenuText.IsEmpty()) {
        l_local->port = 0 ;

    } else {

        sMenuText = sMenuText.Right(sMenuText.GetLength() - 1);
        l_local->port = (_ttoi(sMenuText));
    }

}

//****************************Method Header************************************
//Method Name   :OnPosMenu()
//Author        :PATNI / NJ
//Purpose       :
//*****************************************************************************
void CCoilLstDlg::OnPosMenu(
    UINT uPosID
)
{
    LPCTSTR FUNC_NAME = _T("CCoilLstDlg::OnPosMenu");
    PQM_TRACE_SCOPED(FUNC_NAME);

    CString sMenuText;
    pPosMenu->GetMenuString(uPosID, sMenuText, MF_BYCOMMAND);

    sMenuText.TrimLeft();

    if (sMenuText.IsEmpty()) {
        SetColorButton((ButtonEx*)GetDlgItem(ubtnID), RGB(255, 0, 0));

    } else {
        SetDefaultColor((ButtonEx*)GetDlgItem(ubtnID)) ;
    }

    //Set the focus to coil combo
    m_btnCoil[(ubtnID - IDC_BUTTON_POS_1)].SetFocus();

    UpdatePosMenu(sMenuText, uPosID);

    CoilSelectionSelectCoil_t* l_local = m_selection_utiliy->GetSelectedCoil(iAIndex) ;
    l_local->position = _ttoi(sMenuText) ;
    l_local->float_position = (float) l_local->position;

    UpdateHighlightColorsForPositionCombos();

}


void CCoilLstDlg::CheckCombineFailedFor(
    const int select_coil_index
)const
{

    bool l_flag = (select_coil_index == -1) ;

    for (int coil_button = 0 ; coil_button < MAX_COIL_BUTTON ; coil_button ++) {

        EnableDisableCoilButtonsFor(coil_button, l_flag ? true : (coil_button == select_coil_index));
    }
}

//+Patni-Hemant/2010Feb25/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : UiSetMenuDefaultColorForCouch
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCoilLstDlg::UiSetMenuDefaultColorForCouch(
    const ACoilButtonType f_acoilbuttontype,
    const ACoilItems f_acoilitems
)
{
    ButtonEx* l_buton = GetCouchButton(f_acoilbuttontype, f_acoilitems);

    if (l_buton) {
        SetDefaultColor(l_buton);
    }
}
//-Patni-Hemant/2010Feb25/Added/ACE-Phase#3/CDS Requirement


void CCoilLstDlg::UiSetMenuWarningColorForCouch(
    const ACoilButtonType f_acoilbuttontype,
    const ACoilItems f_acoilitems
)
{
    //+Patni-Hemant/2010Feb25/Modified/ACE-Phase#3/CDS Requirement
    //    switch (f_acoilbuttontype) {
    //
    //        case ACoilPortButtons   :
    //            l_buton = & m_btnPort[f_acoilitems] ;
    //
    //            break ;
    //
    //        case ACoilCoilButtons:
    //
    //            l_buton = & m_btnCoil[f_acoilitems] ;
    //
    //            break ;
    //
    //        case ACoilPositionButtons:
    //            l_buton = & m_btnPos[f_acoilitems] ;
    //
    //            break ;
    //
    //    }

    ButtonEx* l_buton = GetCouchButton(f_acoilbuttontype, f_acoilitems);
    //-Patni-Hemant/2010Feb25/Modified/ACE-Phase#3/CDS Requirement

    if (l_buton)
        SetColorButton(l_buton, RGB(255, 0, 0));

}


//****************************Method Header************************************
//Method Name   :CreateCoilMenu()
//Author        :PATNI / NJ
//Purpose       :
//*****************************************************************************
void CCoilLstDlg::CreateCoilMenu(
)
{
    LPCTSTR FUNC_NAME = _T("CCoilLstDlg::CreateCoilMenu");
    PQM_TRACE_SCOPED(FUNC_NAME);

    CreateGCoilMenu();
    CreateLCoilMenu();
    CreateACoilMenu();
}


//****************************Method Header************************************
//Method Name   :CreatePosMenu()
//Author        :PATNI / NJ
//Purpose       :
//*****************************************************************************
//+Patni-Hemant/2010Feb26/Modified/ACE-Phase#3/CDS Requirement
void CCoilLstDlg::CreatePosMenu(
    const int* const pPos,
    CoilSelectionSelectCoil_s* f_coil_struct
)
//-Patni-Hemant/2010Feb26/Modified/ACE-Phase#3/CDS Requirement
{
    LPCTSTR FUNC_NAME = _T("CCoilLstDlg::CreatePosMenu");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //Patni-Dhanesh/2009Aug24/Modified/cpp test corrections
    if (NULL == pPos) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("pPos pointer is NULL"));

        return;
    }

    //-Patni-Dhanesh/2009Aug24/Modified/cpp test corrections

    //+Patni-Hemant/2010Feb26/Added/ACE-Phase#3/CDS Requirement
    if (!f_coil_struct) {

        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("f_coil_struct pointer is NULL"));
        return ;
    }

    //-Patni-Hemant/2010Feb26/Added/ACE-Phase#3/CDS Requirement

    CreatePositionMenu();
    //+Patni-Hemant/2010Feb26/Added/ACE-Phase#3/CDS Requirement
    const int l_prev_pos = f_coil_struct->position ;
    f_coil_struct->position = 0;
    //-Patni-Hemant/2010Feb26/Added/ACE-Phase#3/CDS Requirement

    searchSelPosition(LOCATION_UPPER);
    searchSelPosition(LOCATION_LOWER);

    //+Patni-Hemant/2010Feb26/Added/ACE-Phase#3/CDS Requirement
    if (f_coil_struct->coil == NULL) {
        m_selection_utiliy->CheckPosition(m_selection_utiliy->GetSelectedCoil());
        return ;
    }

    f_coil_struct->position = l_prev_pos;
    searchSelPosition(f_coil_struct->coil->location & 0x000F);
    //-Patni-Hemant/2010Feb26/Added/ACE-Phase#3/CDS Requirement

    CString sMenuItem ;

    //now remove unnecessary menu items..accoridng to given arguments...
    if (pPos[0] == -1) {

        //all menu items are needed..

        //+Patni-Hemant/2010Feb26/Modified/ACE-Phase#3/CDS Requirement
        //        for (int l_index = pPos[1] + 1 ; l_index <= MAX_SECTION; l_index ++) {
        //
        //            sMenuItem.Format(_T("%d"), l_index) ;
        //            //RemoveMenuItemFromMenu(pPosMenu, sMenuItem) ;
        //        }

        const int l_coil_size_in_lables = (int) m_selection_utiliy->GetTotalLablesOccupied(f_coil_struct) ;

        for (int i = 1; i <= MAX_SECTION; i++) {

            if (i + l_coil_size_in_lables - 1 <= MAX_SECTION) {

                //XtManageChild(CoilSelectionPositionButton[no][i]);
            } else {
                sMenuItem.Format(_T("%d"), i) ;
                RemoveMenuItemFromMenu(pPosMenu, sMenuItem) ;
            }
        }

        //f_coil_struct->position = 0;
        //-Patni-Hemant/2010Feb26/Modified/ACE-Phase#3/CDS Requirement

    } else {

        //+Patni-Hemant/2010Feb26/Modified/ACE-Phase#3/CDS Requirement
        /*
        int l_prev_menu_item = 0;

        for (int i = 0; pPos[i] != 0; i++) {

            int l_current_item = pPos[i];

            if (l_prev_menu_item != -1 && l_prev_menu_item < l_current_item) {

                for (int l_index = l_prev_menu_item + 1 ; l_index < l_current_item; l_index ++) {

                    sMenuItem.Format(_T("%d"), l_index) ;
                    RemoveMenuItemFromMenu(pPosMenu, sMenuItem) ;
                }
            }

            l_prev_menu_item = l_current_item;
        }

        for (int l_index = l_prev_menu_item + 1 ; l_index <= MAX_SECTION; l_index ++) {

            sMenuItem.Format(_T("%d"), l_index) ;
            RemoveMenuItemFromMenu(pPosMenu, sMenuItem) ;
        }
        */

        bool l_insert_menu[MAX_SECTION + 1] = {0} ;

        //Patni-Hemant/2010May6/Added/JaFT/SM_PH3_CDR-CDS_CDR_04
        const int l_total_pos_flags = 4 ;

        for (int i = 1; i <= MAX_SECTION; i++) {

            //+Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_04
            //for(int j = 0; j < 4; j++){
            for (int j = 0; j < l_total_pos_flags; j++) {
                //-Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_04

                if (f_coil_struct->coil->position[j] == i) {

                    //XtManageChild(CoilSelectionPositionButton[no][i]);
                    l_insert_menu[i] = true;

                    if (j == 0) {

                        //XtVaSetValues(parent,
                        //	XmNmenuHistory,	  CoilSelectionPositionButton[no][i],
                        //	NULL);

                        f_coil_struct->position = i;

                        searchSelPosition(f_coil_struct->coil->location & 0x000F);
                    }
                }
            }
        }

        for (i = 1; i <= MAX_SECTION; i++) {
            if (l_insert_menu[i] == false) {
                sMenuItem.Format(_T("%d"), i) ;
                RemoveMenuItemFromMenu(pPosMenu, sMenuItem) ;
            }
        }

        //-Patni-Hemant/2010Feb26/Modified/ACE-Phase#3/CDS Requirement
    }

    //+Patni-Hemant/2010Feb26/Added/ACE-Phase#3/CDS Requirement
    m_selection_utiliy->CheckPosition(m_selection_utiliy->GetSelectedCoil());
    //-Patni-Hemant/2010Feb26/Added/ACE-Phase#3/CDS Requirement
}


//****************************Method Header************************************
//Method Name   :DisplayMenu()
//Author        :PATNI / NJ
//Purpose       :
//*****************************************************************************
void CCoilLstDlg::DisplayMenu(PopupMenuEx* pPQMMenu, const UINT uId)
{
    CRect rect;
    GetDlgItem(uId)->GetWindowRect(&rect);

    CString sFontName(_T(""));
    sFontName.LoadString(IDS_FONT_NAME);
    pPQMMenu->SetFont(sFontName, AFX_IDS_FONTSIZE, false);

    pPQMMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON,
                             rect.left, rect.top + pPQMMenu->GetMenuItemHeight() , this);

}



//****************************Method Header************************************
//Method Name   :CleanUp()
//Author        :PATNI / NJ
//Purpose       :
//*****************************************************************************
void CCoilLstDlg::CleanUp(
)
{
    LPCTSTR FUNC_NAME = _T("CCoilLstDlg::CleanUp");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-Hemant/2010Apr1/Added/ACE-Phase#3/CDS Requirement
    m_selection_utiliy->SetUIClassInterfaceForCoilListDialog(NULL);
    //-Patni-Hemant/2010Apr1/Added/ACE-Phase#3/CDS Requirement

    ClearGCoilMenu();

    //Patni-DKH/2009Aug26/Modified/cpp test corrections
    if (NULL != pLPortMenu) {
        for (int i = 0; i < 2; i++) {

            DEL_PTR(pLPortMenu[i])
        }

        //+Patni-DKH/2009Aug26/Modified/cpp test corrections

    } else {

        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("pLPortMenu pointer is NULL"));

    }

    //-Patni-DKH/2009Aug26/Modified/cpp test corrections

    DEL_PTR(pAPortMenu)

    DEL_PTR(pPortMenu)

    DEL_PTR(pPosMenu)

}


//****************************Method Header************************************
//Method Name   :UpdatePortMenu()
//Author        :PATNI / NJ
//Purpose       :
//*****************************************************************************
void CCoilLstDlg::UpdatePortMenu(CString sMenuText, const UINT iMenuItemID)
{
    LPCTSTR FUNC_NAME = _T("CCoilLstDlg::UpdatePortMenu");

    CString strEdit;

    //Patni-Dhanesh/2009Aug24/Modified/cpp test corrections
    if (NULL != GetDlgItem(ubtnID)) {
        GetDlgItem(ubtnID)->GetWindowText(strEdit);
        //+Patni-Dhanesh/2009Aug24/Modified/cpp test corrections

    } else {

        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("GetDlgItem(ubtnID) pointer is NULL"));
        return;
    }

    //-Patni-Dhanesh/2009Aug24/Modified/cpp test corrections

    //Set edit control with the menu item currently sel.
    GetDlgItem(ubtnID)->SetWindowText(sMenuText);

    strEdit.TrimLeft();
    sMenuText.TrimLeft();

    //Delete this selected menu item from menu
    if (!sMenuText.IsEmpty()) {

        m_lstPort.DeleteString(m_lstPort.FindStringExact(-1, sMenuText));
    }

    //Add the edit text in the menu
    if (!strEdit.IsEmpty()) {

        m_lstPort.AddString(strEdit);
    }

    MaintainPortMenu();
}


//****************************Method Header************************************
//Method Name   :UpdatePosMenu()
//Author        :PATNI / NJ
//Purpose       :
//*****************************************************************************
void CCoilLstDlg::UpdatePosMenu(
    const CString& sMenuText,
    const UINT iMenuItemID
)const
{

    LPCTSTR FUNC_NAME = _T("CCoilLstDlg::UpdatePosMenu");

    CWnd* l_wnd = GetDlgItem(ubtnID);

    if (NULL != l_wnd) {

        //Set edit control with the menu item currently selected
        l_wnd->SetWindowText(sMenuText);

    } else {

        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("ubtnID is Invalid"));
        return;
    }

    //-Patni-Dhanesh/2009Aug24/Modified/cpp test corrections
}


//****************************Method Header************************************
//Method Name   :UpdateLCoilMenu()
//Author        :PATNI / NJ
//Purpose       :
//*****************************************************************************
void CCoilLstDlg::UpdateLCoilMenu(
    const CString& sMenuText,
    const UINT iMenuItemID
)const
{
    LPCTSTR FUNC_NAME = _T("CCoilLstDlg::UpdateLCoilMenu");

    CString strEdit;

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    CWnd* l_dlgitem = GetDlgItem(ubtnID);

    if (NULL != l_dlgitem) {
        l_dlgitem->GetWindowText(strEdit);

        //Set edit control with the menu item currently sel.
        l_dlgitem->SetWindowText(sMenuText);

    } else {

        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("l_dlgitem pointer is NULL"));

        return;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

}


//****************************Method Header************************************
//Method Name   :UpdateGCoilMenu()
//Author        :PATNI / NJ
//Purpose       :
//*****************************************************************************
void CCoilLstDlg::UpdateGCoilMenu(
    const CString& sMenuText,
    const UINT iMenuItemID
)const
{
    LPCTSTR FUNC_NAME = _T("CCoilLstDlg::UpdateGCoilMenu");

    CString strEdit;

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    CWnd* l_dlgitem = GetDlgItem(ubtnID);

    if (NULL != l_dlgitem) {
        l_dlgitem->GetWindowText(strEdit);

        //Set edit control with the menu item currently sel.
        l_dlgitem->SetWindowText(sMenuText);

    } else {

        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("l_dlgitem pointer is NULL"));

        return;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

}


void CCoilLstDlg::MaintainPortMenu()
{
    LPCTSTR FUNC_NAME = _T("CCoilLstDlg::MaintainPortMenu");
    PQM_TRACE_SCOPED(FUNC_NAME);

    if (pPortMenu) {
        pPortMenu = NULL;
    }

    DEL_PTR(pPortMenu)

    pPortMenu = new PopupMenuEx();

    pPortMenu->CreatePopupMenu();

    CString sMenuItem = "  ";
    pPortMenu->InsertMenu(0, MF_BYPOSITION | MF_STRING,
                          ID_PORT_MENU_DYNAMIC , sMenuItem);

    PQM_TRACE(USER_FUNC_MARKER, _T("Menu Item ->"), sMenuItem);

    for (int i = 1; i < m_lstPort.GetCount(); i++) {

        m_lstPort.GetText(i, sMenuItem);
        pPortMenu->InsertMenu(-1, MF_BYPOSITION | MF_STRING,
                              ID_PORT_MENU_DYNAMIC + i, sMenuItem);

        PQM_TRACE(USER_FUNC_MARKER, _T("Sub Menu Item ->"), sMenuItem);
    }
}


void CCoilLstDlg::CreateLCoilMenu(
)
{
    LPCTSTR FUNC_NAME = _T("CCoilLstDlg::CreateLCoilMenu");

    PQM_TRACE_SCOPED(FUNC_NAME);

    //Indicate the coil selection window is closed

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_selection_utiliy) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));

    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    CString sFontName;
    sFontName.LoadString(IDS_FONT_NAME);

    int menu_item_counter = ID_L_MENU_DYNAMIC ;

    CString l_log_str = _T("");
    CString l_label = _T("");

    for (int i = 0; i < 2; i++) {

        DllT* l_linear_head = m_selection_utiliy->GetLinearHead(i);

        //Added by Hemant On 10/6/2008 7:06:57 PM
        DEL_PTR(pLPortMenu[i])
        //End Of addition By Hemant On10/6/2008 7:07:02 PM

        pLPortMenu[i] = new PopupMenuEx();

        //+Patni-DKH/2009Aug24/Modified/cpp test corrections
        if (NULL == pLPortMenu[i]) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("pLPortMenu[i] pointer is NULL"));
            continue;
        }

        pLPortMenu[i]->CreatePopupMenu();
        pLPortMenu[i]->InsertMenu(0, MF_BYPOSITION | MF_STRING, menu_item_counter++,
                                  _T("                       "));

        l_log_str = _T("");
        l_log_str.Format(_T("Creating L%d Menu"), i + 1);
        PQM_TRACE(USER_FUNC_MARKER, _T(""), l_log_str);

        if (l_linear_head) {

            for (DllT* l_trav = l_linear_head; l_trav != NULL; l_trav = l_trav->next) {

                CoilSelectionCoilButton_t* l_button_struct = static_cast<CoilSelectionCoilButton_t*>(l_trav->item) ;

                if (l_button_struct && l_button_struct->coil) {

                    l_label = l_button_struct->coil->label ;
                    pLPortMenu[i]->InsertMenu(-1, MF_BYPOSITION | MF_STRING, menu_item_counter++ , l_label);

                    PQM_TRACE(USER_FUNC_MARKER, _T("Menu Item ->"), l_label);
                }
            }
        }
    }
}

void CCoilLstDlg::CreateACoilMenu(
)
{
    LPCTSTR FUNC_NAME = _T("CCoilLstDlg::CreateACoilMenu");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //Indicate the coil selection window is closed

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_selection_utiliy) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));

        return;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections
    DllT* l_couch_head = m_selection_utiliy->GetCouchHead(0);
    //Patni-AMT/2009Nov10/Commented/DeFT/MCM0229-00154
    ClearACoilMenu();
    pAPortMenu = new PopupMenuEx();
    pAPortMenu->CreatePopupMenu();

    int menu_item_counter = ID_A_MENU_DYNAMIC ;
    pAPortMenu->InsertMenu(0, MF_BYPOSITION | MF_STRING, menu_item_counter++,
                           _T("                       "));

    CString l_label = _T("");
    CString l_sub_label = _T("");
    CString sStr1 = _T("");
    CString sStr2 = _T("");
    int min_string_length = 0;

    if (l_couch_head) {

        for (DllT* l_trav = l_couch_head; l_trav != NULL; l_trav = l_trav->next) {

            CoilSelectionCoilButton_t* l_button_struct = (
                        (CoilSelectionCoilButton_t*)l_trav->item);
            CoilSelectionCoil_t* l_coil_struct = l_button_struct->coil;
            l_label = l_coil_struct->label;

            if (l_button_struct->subMenu != NULL) {
                DllT* l_sub_trav = l_button_struct->subMenu ;

                CoilSelectionCoilButton_t* l_sub_button_struct =
                    ((CoilSelectionCoilButton_t*)l_sub_trav->item);
                CoilSelectionCoil_t* l_sub_coil_struct = l_sub_button_struct->coil;
                l_sub_label = l_sub_coil_struct->label;
                PopupMenuEx* pAPortSubMenu = new PopupMenuEx();

                if (pAPortSubMenu == NULL) {
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                              _T("pAPortSubMenu pointer is NULL"));
                    return;
                }

                pAPortSubMenu->CreatePopupMenu();
                m_paportsubmenu_list.AddTail(pAPortSubMenu);
                pAPortSubMenu->InsertMenu(-1, MF_BYPOSITION | MF_STRING,
                                          menu_item_counter++ , l_label);

                PQM_TRACE(USER_FUNC_MARKER, _T("Menu Item ->"), l_label);

                for (; l_sub_trav != NULL; l_sub_trav = l_sub_trav->next) {

                    if (l_coil_struct->coil_id != l_sub_coil_struct->coil_id) {
                        break;

                    } else {
                        l_sub_button_struct =
                            ((CoilSelectionCoilButton_t*)l_sub_trav->item);
                        l_sub_coil_struct = l_sub_button_struct->coil;

                        l_sub_label = l_sub_coil_struct->label;

                        pAPortSubMenu->InsertMenu(-1, MF_BYPOSITION | MF_STRING,
                                                  menu_item_counter++, l_sub_label);

                        PQM_TRACE(USER_FUNC_MARKER, _T("Sub Menu Item ->"), l_sub_label);
                    }
                }

                //Get the Group Name for the sub menu
                TCHAR sGroupLabel[32] = {0};
                sStr1 = l_sub_coil_struct->label;
                sStr2 = l_coil_struct->label;

                min_string_length = sStr1.GetLength() > sStr2.GetLength() ? sStr2.GetLength() : sStr1.GetLength() ;

                for (int i = 0; i < min_string_length; i++) {

                    if (sStr1.GetAt(i) == sStr2.GetAt(i)) {

                        sGroupLabel[i] = sStr1.GetAt(i);

                    } else {
                        break;
                    }
                }

                sGroupLabel[i] = '\0';
                HMENU hmenu = pAPortSubMenu->GetSafeHmenu();
                pAPortMenu->AppendMenu(MF_STRING | MF_POPUP, (UINT) hmenu, sGroupLabel);

                PQM_TRACE(USER_FUNC_MARKER, _T("Group Menu Item ->"), sGroupLabel);

            } else {
                pAPortMenu->InsertMenu(-1, MF_BYPOSITION | MF_STRING,
                                       menu_item_counter++ , l_label);

                PQM_TRACE(USER_FUNC_MARKER, _T("Menu Item ->"), l_label);
            }
        }
    }
}


void CCoilLstDlg::CreateGCoilMenu()
{

    LPCTSTR FUNC_NAME = _T("CCoilLstDlg::CreateGCoilMenu");

    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_selection_utiliy) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));

        return;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections


    ClearGCoilMenu();
    pGPortMenu = new PopupMenuEx();
    pGPortMenu->CreatePopupMenu();

    //Added by Hemant On 10/15/2008 1:16:28 PM .
    //menu items were wrongly inserted before..
    int menu_item_counter = ID_G_MENU_DYNAMIC ;

    pGPortMenu->InsertMenu(0, MF_BYPOSITION | MF_STRING, menu_item_counter++,
                           _T("                       "));

    DllT* l_head = m_selection_utiliy->GetTrestleHead();

    if (!l_head) {
        return ;
    }

    CString l_label = _T("");
    CString l_sub_label = _T("");
    CString sStr1 = _T("");
    CString sStr2 = _T("");
    int min_string_length = 0;

    for (DllT* l_trav = l_head; l_trav != NULL; l_trav = l_trav->next) {

        CoilSelectionCoilButton_t* l_button_struct = (
                    (CoilSelectionCoilButton_t*)l_trav->item);

        CoilSelectionCoil_t* l_coil_struct = l_button_struct->coil;

        l_label = l_coil_struct->label;

        if (l_button_struct->subMenu != NULL) {

            DllT* l_sub_trav = l_button_struct->subMenu ;

            CoilSelectionCoilButton_t* l_sub_button_struct =
                ((CoilSelectionCoilButton_t*)l_sub_trav->item);

            CoilSelectionCoil_t* l_sub_coil_struct = l_sub_button_struct->coil;

            l_sub_label = l_sub_coil_struct->label;

            PopupMenuEx* pGPortSubMenu = new PopupMenuEx();

            //+Patni-Dhanesh/2009Aug20/Modified/cpp test corrections
            if (pGPortSubMenu == NULL) {

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("pGPortSubMenu pointer is NULL"));

                return;
            }

            //-Patni-Dhanesh/2009Aug20/Modified/cpp test corrections

            pGPortSubMenu->CreatePopupMenu();

            //added to menu list..
            m_pgportsubmenu_list.AddTail(pGPortSubMenu);


            //Modified by Hemant .. incorrect menu item insertion..
            //			pGPortSubMenu->InsertMenu(-1,
            //						MF_BYPOSITION|MF_STRING,ID_G_MENU_DYNAMIC +
            //						l_button_struct->menu_id, l_label);

            pGPortSubMenu->InsertMenu(-1,
                                      MF_BYPOSITION | MF_STRING, menu_item_counter++ , l_label);
            //End Of addition By Hemant On10/15/2008 1:15:50 PM

            PQM_TRACE(USER_FUNC_MARKER, _T("Menu Item ->"), l_label);


            for (; l_sub_trav != NULL; l_sub_trav = l_sub_trav->next) {
                if (l_coil_struct->coil_id != l_sub_coil_struct->coil_id) {
                    break;

                } else {
                    l_sub_button_struct =
                        ((CoilSelectionCoilButton_t*)l_sub_trav->item);
                    l_sub_coil_struct = l_sub_button_struct->coil;

                    l_sub_label = l_sub_coil_struct->label;

                    //Added by Hemant On 10/15/2008 1:18:14 PM
                    //					pGPortSubMenu->InsertMenu(-1,
                    //						MF_BYPOSITION|MF_STRING,ID_G_MENU_DYNAMIC +
                    //						l_sub_button_struct->menu_id, l_sub_label);

                    pGPortSubMenu->InsertMenu(-1,
                                              MF_BYPOSITION | MF_STRING, menu_item_counter++, l_sub_label);
                    //
                    //End Of addition By Hemant On10/15/2008 1:18:09 PM

                    PQM_TRACE(USER_FUNC_MARKER, _T("Sub Menu Item ->"), l_sub_label);

                }
            }

            //Get the Group Name for the sub menu
            sStr1 = l_sub_coil_struct->label;
            sStr2 = l_coil_struct->label ;

            min_string_length = sStr1.GetLength() > sStr2.GetLength() ? sStr2.GetLength() : sStr1.GetLength() ;

            TCHAR sGroupLabel[32] = {0};

            for (int i = 0; i < min_string_length; i++) {

                if (sStr1.GetAt(i) == sStr2.GetAt(i)) {
                    sGroupLabel[i] = sStr1.GetAt(i);

                } else {
                    break;
                }
            }

            sGroupLabel[i] = '\0';
            HMENU hmenu = pGPortSubMenu->GetSafeHmenu();
            pGPortMenu->AppendMenu(MF_STRING | MF_POPUP, (UINT) hmenu, sGroupLabel);

        } else {

            //Modified by Hemant
            //			pGPortMenu->InsertMenu(-1, MF_BYPOSITION|MF_STRING,ID_G_MENU_DYNAMIC
            //				+ l_button_struct->menu_id, l_label);
            pGPortMenu->InsertMenu(-1, MF_BYPOSITION | MF_STRING, menu_item_counter++, l_label);
            //
            //End Of addition By Hemant On10/15/2008 1:20:04 PM

            PQM_TRACE(USER_FUNC_MARKER, _T("Menu Item ->"), l_label);
        }
    }
}


void CCoilLstDlg::DisplayCoilFor(
    const CoilSelectionSelectCoil_t* f_local,
    const int f_coil_button_index
)const
{

    switch (f_coil_button_index) {

        case 9 :  {

            DisplayGCoilFrom(f_local, GCoilItems_1);
        }
        break ;

        case 7 :
        case 8 : {

            DisplayLCoilFrom(f_local, (LCoilItems)(f_coil_button_index - GetBaseIndexFor(CoilType_L)));
        }
        break ;

        case 0 :
        case 1 :
        case 2 :
        case 3 :
        case 4 :
        case 5 :
        case 6 :

        {
            DisplayACoilFrom(f_local, (ACoilItems) f_coil_button_index);

        }
        break ;
    }
}


void CCoilLstDlg::EnableDisableCoilButtonsFor(
    const int f_coil_button_index,
    const bool f_enable
)const
{

    switch (f_coil_button_index) {

        case 9 :  {

            EnableDisableGCoilButtonsFor(GCoilItems_1 , f_enable);
        }
        break ;

        case 7 :
        case 8 : {
            EnableDisableLCoilButtonsFor((LCoilItems)(f_coil_button_index - GetBaseIndexFor(CoilType_L)),
                                         f_enable);
        }
        break ;

        case 0 :
        case 1 :
        case 2 :
        case 3 :
        case 4 :
        case 5 :
        case 6 : {
            EnableDisableACoilButtonsFor((ACoilItems) f_coil_button_index, f_enable);

        }
        break ;
    }
}

void CCoilLstDlg::EnableDisableGCoilButtonsFor(
    const GCoilItems f_gcoilitem,
    const bool f_enable
)const
{

    const int button_id = GetButtonIDFor(f_gcoilitem) ;
    CWnd* wnd = GetDlgItem(button_id);

    if (wnd && wnd->IsWindowVisible())
        wnd->EnableWindow(f_enable);

    return ;

}

void CCoilLstDlg::EnableDisableLCoilButtonsFor(
    const LCoilItems f_lcoilitem,
    const bool f_enable
)const
{

    int button_id = GetButtonIDFor(f_lcoilitem, LCoilCoilButtons);
    CWnd* wnd = GetDlgItem(button_id);

    if (wnd && wnd->IsWindowVisible()) {

        wnd->EnableWindow(f_enable);

        //As we have coil button processed here..
        //we should also do this for port and position button

        button_id = GetButtonIDFor(f_lcoilitem, LCoilPositionButtons);
        wnd = GetDlgItem(button_id);

        if (wnd && wnd->IsWindowVisible())
            wnd->EnableWindow(f_enable);
    }

}

void CCoilLstDlg::EnableDisableACoilButtonsFor(
    const ACoilItems f_acoilitem,
    const bool f_enable
)const
{
    int button_id = GetButtonIDFor(f_acoilitem, ACoilCoilButtons);
    CWnd* wnd = GetDlgItem(button_id);

    if (wnd && wnd->IsWindowVisible()) {
        wnd->EnableWindow(f_enable);

        //As we have coil button processed here..
        //we should also do this for port and position button

        button_id = GetButtonIDFor(f_acoilitem, ACoilPortButtons);
        wnd = GetDlgItem(button_id);

        if (wnd && wnd->IsWindowVisible())
            wnd->EnableWindow(f_enable);

        button_id = GetButtonIDFor(f_acoilitem, ACoilPositionButtons);
        wnd = GetDlgItem(button_id);

        if (wnd && wnd->IsWindowVisible())
            wnd->EnableWindow(f_enable);
    }
}



void CCoilLstDlg::ClearCoilInformation(
    const CoilTypes coiltypes
)
{

    switch (coiltypes) {

        case CoilType_G: {

            for (int button_index = 0 ; button_index < GCoilItems_TOTAL; button_index++) {
                DisplayGCoilFrom(NULL, (GCoilItems) button_index) ;
            }
        }
        break ;

        case CoilType_L: {

            for (int button_index = 0 ; button_index < LCoilItems_TOTAL; button_index++) {
                DisplayLCoilFrom(NULL, (LCoilItems) button_index) ;
            }
        }
        break ;

        case CoilType_A: {

            for (int button_index = 0; button_index < ACoilItems_TOTAL; button_index++) {
                DisplayACoilFrom(NULL, (ACoilItems) button_index) ;
            }

        }
        break ;
    }

}


//+Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement
//void CCoilLstDlg::CoilMismatchOKCancel(
status_t CCoilLstDlg::CoilMismatchOKCancel(
    //-Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement
    const bool f_ok /*= true*/
)
{
    LPCTSTR FUNC_NAME = _T("CCoilLstDlg::CoilMismatchOKCancel");
    PQM_TRACE_SCOPED(FUNC_NAME);

    status_t l_sts = COIL_S_ERROR;

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_selection_utiliy) {

        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));
        //+Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement
        //return;
        return l_sts;
        //-Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement
    }

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_ccoilselwininterface) {

        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("m_ccoilselwininterface pointer is NULL"));
        //+Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement
        //return;
        return l_sts;
        //-Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections
    ClearSelectedCoilInformation();

    if (f_ok) {

        l_sts = m_selection_utiliy->SetPqmListCoil(
                    m_selection_utiliy->GetConnectCoil(), FALSE);

    } else {

        // Read PQM coil list
        l_sts = m_selection_utiliy->SetPqmListCoil(
                    m_selection_utiliy->GetShowCoilList(), FALSE);
    }

    //+Patni-Hemant/2010Mar2/Modified/ACE-Phase#3/CDS Requirement
    //CoilUiListDlgOkCB();
    if ((f_ok && (l_sts != COIL_S_OTHER)) || !f_ok) {

        //+Patni-Sribanta/2010Aug10/Modified/TMSC-REDMINE-581
        //CoilUiListDlgOkCB();
        l_sts = CoilUiListDlgOkCB(!f_ok) ? l_sts : COIL_S_OTHER ;
        //-Patni-Sribanta/2010Aug10/Modified/TMSC-REDMINE-581
    }

    //-Patni-Hemant/2010Mar2/Modified/ACE-Phase#3/CDS Requirement

    DisplayCoilInformation(true, NULL, true) ;

    //Patni-Hemant/2010Mar2/Modified/ACE-Phase#3/CDS Requirement
    //m_ccoilselwininterface->DisplayButtonsOnAxisLocator();


    if (l_sts == COIL_S_ERROR) {
        //PENDING
        //DB_PRINTF("",("%s:%d ERR_ILLEGAL_PQM_DATA sts : COIL_S_ERROR \n",__FILE__,__LINE__, sts));
        //CoilSelectionUiErrorNAVI(CoilSelectionUiFetchLiteral("ERR_ILLEGAL_PQM_DATA"),ERR_ERROR, NULL);

        //+Patni-Hemant/2009Mar17/Modify/ACE-1/Internal defect, removing the MFC messagebox
        //MessageBox(_T("The data transmitted from the Sequence Queue contains an error."));

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("The data transmitted from the Sequence Queue contains an error."));

        //-Patni-Hemant/2009Mar17/Modify/ACE-1/Internal defect, removing the MFC messagebox
        //+Patni-Hemant/2010Apr12/Added/ACE-Phase#3/CDS Requirement

        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
            CPQMUtility::GetMultiLingualString(_T("IDS_CS_ERR_ILLEGAL_PQM_DATA")),
            MPlus::EH::Information, _T("PQM"), _T("PQM"));

        //-Patni-Hemant/2010Apr12/Added/ACE-Phase#3/CDS Requirement

    } else if (l_sts != COIL_S_OTHER) {

        // PENDING
        // Get the CoilListDlg pointer and call its Ok method.
        //CoilUiListDlgOkCB(NULL,NULL,NULL);
        //        m_p_dialog_coil_list->OnOK();
    }

    //+Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement
    //return;
    return l_sts;
    //-Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement
}


bool CCoilLstDlg::IsPortOccupied(
    const int f_port_number_onebase
)const
{
    CString sbtnPortText = _T("");

    for (int couchport_buttonindex = 0; couchport_buttonindex < MAX_COUCH_PORT ; couchport_buttonindex++) {

        m_btnPort[couchport_buttonindex].GetWindowText(sbtnPortText);
        sbtnPortText.TrimLeft();
        sbtnPortText.TrimRight();
        sbtnPortText = sbtnPortText.Right(sbtnPortText.GetLength() - 1) ;

        if ((_ttoi(sbtnPortText)) == f_port_number_onebase)
            return true ;
    }

    return false ;
}


void CCoilLstDlg::PreparePortMenu(
)
{
    DEL_PTR(pPortMenu)

    pPortMenu = new PopupMenuEx();
    pPortMenu->CreatePopupMenu();

    CString sMenuItem = "";
    pPortMenu->InsertMenu(0, MF_BYPOSITION | MF_STRING, ID_PORT_MENU_DYNAMIC , sMenuItem);

    int menu_item_count = 1;

    for (int port_index = 1; port_index <= MAX_COUCH_PORT; port_index++) {

        if (IsPortOccupied(port_index))
            continue ;

        sMenuItem.Format(_T("A%d"), port_index) ;
        pPortMenu->InsertMenu(-1, MF_BYPOSITION | MF_STRING,
                              ID_PORT_MENU_DYNAMIC + (menu_item_count++) , sMenuItem);
    }
}



void CCoilLstDlg::UpdateHighlightColorsForPositionCombos(
)
{
    LPCTSTR FUNC_NAME = _T("CCoilLstDlg::UpdateHighlightColorsForPositionCombos");

    for (int i = 0; i < MAX_COUCH_PORT; i++) {

        SetDefaultColor(&m_btnPos[i]) ;
    }


    //-Patni-Dhanesh/2009Aug24/Modified/cpp test corrections
    if (NULL != m_selection_utiliy) {
        m_selection_utiliy->CheckPosition(m_selection_utiliy->GetSelectedCoil());
        //+Patni-Dhanesh/2009Aug24/Modified/cpp test corrections

    } else {

        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));
    }

    //-Patni-Dhanesh/2009Aug24/Modified/cpp test corrections
    return ;
}


void CCoilLstDlg::CreatePositionMenu(
)
{
    DEL_PTR(pPosMenu)

    pPosMenu = new PopupMenuEx();
    pPosMenu->CreatePopupMenu();

    CString sMenuItem = "";
    pPosMenu->InsertMenu(0, MF_BYPOSITION | MF_STRING, ID_POS_MENU_DYNAMIC , sMenuItem);

    int menu_item_count = 1;
    int menu_item_id = -1;

    for (int l_pos_index = 1; l_pos_index <= MAX_SECTION; l_pos_index++) {

        menu_item_id = ID_POS_MENU_DYNAMIC + (menu_item_count) ;
        sMenuItem.Format(_T("%d"), l_pos_index) ;
        pPosMenu->InsertMenu(-1, MF_BYPOSITION | MF_STRING,
                             menu_item_id , sMenuItem);

        //        if(l_filled_sec_array[l_pos_index-1] >= 1) {
        //
        //            pPosMenu->SetImage(0, menu_item_id ) ;
        //        }

        menu_item_count ++ ;
    }
}


//+Patni-Hemant/2010Mar5/Added/ACE-Phase#3/CDS Requirement
//void CCoilLstDlg::searchSelPosition(int location)
void CCoilLstDlg::searchSelPosition(const long location)const
//-Patni-Hemant/2010Mar5/Added/ACE-Phase#3/CDS Requirement
{
    LPCTSTR FUNC_NAME = _T("CCoilLstDlg::searchSelPosition");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-PP/2009May28/Modified/SU14 CPP test initialization corrections
    CoilSelectionSection_t*		sec_t = NULL;
    DllT*				dll = NULL;
    //-Patni-PP/2009May28/Modified/SU14 CPP test initialization corrections

    bool_t	selpos[16] = {0};
    bzero(selpos, sizeof(selpos));
    int sec = 0;

    for (int i = 0; i < MAX_COUCH_PORT + MAX_LINEAR_PORT; i++) {

        //-Patni-Dhanesh/2009Aug24/Modified/cpp test corrections
        if (NULL == m_selection_utiliy) {

            PQM_TRACE(USER_FUNC_MARKER,
                      FUNC_NAME,
                      _T("m_selection_utiliy pointer is NULL"));
            continue;
        }

        //-Patni-Dhanesh/2009Aug24/Modified/cpp test corrections

        CoilSelectionSelectCoil_t* selectCoil = m_selection_utiliy->GetSelectedCoil(i) ;

        if (selectCoil->coil != NULL) {
            //+Patni-Hemant/2010Feb15/Modified/ACE-Phase#3/CDS Requirement
            //if ((selectCoil->coil->location) == (location)
            if ((selectCoil->coil->location & location)
                //-Patni-Hemant/2010Feb15/Modified/ACE-Phase#3/CDS Requirement
                && selectCoil->position != 0) {


                int l_counter = 0;
                int l_max_section_num = 0;
                int l_tempsection = 0;

                for (dll = selectCoil->coil->section; dll != NULL; dll = dll->next) {
                    sec_t = (CoilSelectionSection_t*)dll->item;
                    sec = sec_t->section;
                    l_tempsection = sec_t->section;

                    if (sec_t->elemXYZ[2] >= 240)
                        l_tempsection = sec_t->section * 2;

                    if (selectCoil->coil->location == LOCATION_HEAD) {
                        sec = (l_tempsection / 2);

                        if (sec > 2 && l_tempsection < 7) {
                            sec--;
                        }
                    }

                    selpos[selectCoil->position + sec - 1] = true;
                    l_counter ++;

                    if ((selectCoil->position + sec - 1) > l_max_section_num) {
                        l_max_section_num = selectCoil->position + sec - 1;
                    }
                }

                if (selectCoil->coil->location != LOCATION_HEAD) {
                    const int l_coil_size_in_lables = (int) m_selection_utiliy->GetTotalLablesOccupied(selectCoil) ;

                    for (int k = l_counter; k <= l_coil_size_in_lables; k++) {
                        selpos[l_max_section_num++] = true;
                    }
                }
            }
        }
    }

    int n = -1;

    for (int l = 0; l < MAX_COUCH_PORT + MAX_LINEAR_PORT; l++) {

        CoilSelectionSelectCoil_t* selectCoil = m_selection_utiliy->GetSelectedCoil(l);

        if (selectCoil->coil != NULL) {
            //+Patni-Hemant/2010Feb15/Modified/ACE-Phase#3/CDS Requirement
            //if ((selectCoil->coil->location) == (location)) {
            if (selectCoil->coil->location & location) {
                //-Patni-Hemant/2010Feb15/Modified/ACE-Phase#3/CDS Requirement

                for (n = 1; n <= MAX_SECTION; n++) {

                    if (selpos[n] == TRUE) {

                        //pPosMenu->SetImage(0, ID_POS_MENU_DYNAMIC + (n) ) ;
                        pPosMenu->CheckMenuItem(ID_POS_MENU_DYNAMIC + (n) , MF_BYCOMMAND | MF_CHECKED) ;
                    }

                    //+Patni-Hemant/2010Feb26/Added/ACE-Phase#3/CDS Requirement
                    else {
                        pPosMenu->CheckMenuItem(ID_POS_MENU_DYNAMIC + (n) , MF_BYCOMMAND | MF_UNCHECKED) ;
                    }

                    //-Patni-Hemant/2010Feb26/Added/ACE-Phase#3/CDS Requirement
                }
            }
        }
    }
}


void CCoilLstDlg::ShowConflictsFor(
    const CoilSelectionSelectCoil_t* f_newcoil_selected,
    const CoilSelectionSelectCoil_t* f_oldcoil_selected,
    const int f_aport_button_index
)
{

    LPCTSTR FUNC_NAME = _T("CCoilLstDlg::ShowConflictsFor");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //note that f_newcoil_selected  is already inserted..


    int* l_same_coil_index = NULL ;
    int l_same_count = 0;

    if (f_oldcoil_selected && f_oldcoil_selected->coil)
        l_same_count = GetOccurencesForCoil(f_oldcoil_selected, &l_same_coil_index) ;

    else {
        l_same_count = GetOccurencesForCoil(f_newcoil_selected, &l_same_coil_index) ;
    }

    //+Patni-Hemant/2009Mar03/Modify/ACE-1/Internal defect
    //if(f_newcoil_selected->coil) {
    if (f_newcoil_selected && f_newcoil_selected->coil) {

        //we have not reseted the information.. so find conflicts//
        if (l_same_count > 1) {

            //we have more than one same coils..
            //so first.., show the port combo for this index..
            //and mark the red colors to ports button for
            //all index

            CoilSelectionSelectCoil_t* f_curcoil = NULL ;
            int l_a_button_index = 0;

            for (int index = 0; index < l_same_count ; index++) {

                //int l_a_button_index = CCoilSelectionUtility::GetConvertedCoilButtonIndexForSelectedCoilIndex(l_same_coil_index[index]) ;
                l_a_button_index = l_same_coil_index[index];
                m_btnPort[l_a_button_index].ShowWindow(SW_SHOW) ;

                //dont't color to all , instead whose port's are clashing.. color those only..
                //SetColorButton(&m_btnPort[l_a_button_index], RGB(255,0,0)) ;

                f_curcoil = m_selection_utiliy->GetSelectedCoil(l_a_button_index);

                if (f_curcoil) {

                    if (f_newcoil_selected->port == f_curcoil->port) {

                        //+Patni-Hemant/2009Mar03/Modify/ACE-1/Internal defects
                        //SetColorButton(&m_btnPort[l_a_button_index], RGB(255,0,0)) ;
                        if (f_curcoil->port == 0) {
                            SetColorButton(&m_btnPort[l_a_button_index], RGB(255, 0, 0)) ;

                        } else {
                            SetDefaultColor(&m_btnPort[l_a_button_index]);
                        }

                        //-Patni-Hemant/2009Mar03/Modify/ACE-1/Internal defects
                    }
                }
            }
        }

    } else {

        //f_newcoil_selected do not have coil/..

        //we have reseted the coil for this index. = act_coil_index_to_update..
        if (l_same_count == 1 && l_same_coil_index) {

            //we have found 1 instance of the f_oldcoil_selected->coil ..so resetthe
            //port from it..
            const int same_coil_button_index = l_same_coil_index[0] ;
            CoilSelectionSelectCoil_t* l_same_coil = m_selection_utiliy->GetSelectedCoil(same_coil_button_index);
            l_same_coil->port = 0 ;
            //l_same_coil->position = 0 ;

            SetDefaultColor(&m_btnPort[same_coil_button_index]);
            DisplayCoilFor(l_same_coil, same_coil_button_index);

        } else {

            //we have still more than 1 same coils...

            //lest say among n same coils, m coils's port is same..
            //if m ==1 . then change the color of that port buttons to default

            if (f_oldcoil_selected) {

                CoilSelectionSelectCoil_t* f_curcoil = NULL ;
                int l_same_port_count = 0;
                int l_same_port_last_button_index = 0;
                int l_a_button_index = 0;

                for (int index = 0; index < l_same_count ; index++) {

                    l_a_button_index = l_same_coil_index[index];
                    f_curcoil = m_selection_utiliy->GetSelectedCoil(l_a_button_index);

                    if (f_curcoil) {

                        if (f_oldcoil_selected->port == f_curcoil->port) {

                            l_same_port_count ++ ;
                            l_same_port_last_button_index = l_a_button_index;
                        }
                    }
                }

                if ((l_same_port_count == 1) && (l_same_port_last_button_index != 0)) {

                    SetDefaultColor(&m_btnPort[l_same_port_last_button_index]);
                }
            }
        }
    }

    DEL_PTR_ARY(l_same_coil_index)

}


//+Patni-AMT/2009Nov10/Commented/DeFT/MCM0229-00154
void CCoilLstDlg::RemoveMenuItemFromMenu(
    CMenu* f_menu ,
    const CString& f_menu_item_to_remove
)const
{
    LPCTSTR FUNC_NAME = _T("CCoilLstDlg::RemoveMenuItemFromMenu");
    PQM_TRACE_SCOPED(FUNC_NAME);

    if (f_menu == NULL) {
        return;
    }

    CString l_sub_menu_string = _T("");
    CString l_menu_item_string = _T("");
    CMenu* l_sub_menu = NULL ;
    int l_smenu_cnt = -1;

    const int l_pmenu_cnt = f_menu->GetMenuItemCount();

    for (int l_index = 0; l_index < l_pmenu_cnt; l_index++) {

        l_sub_menu = f_menu->GetSubMenu(l_index);

        if (l_sub_menu != NULL) {

            l_smenu_cnt = l_sub_menu->GetMenuItemCount();

            for (int l_sub_index = 0; l_sub_index < l_smenu_cnt; l_sub_index++) {

                l_sub_menu->GetMenuString(l_sub_index, l_sub_menu_string,
                                          MF_BYPOSITION);

                if (l_sub_menu_string.CompareNoCase(f_menu_item_to_remove) == 0) {

                    PQM_TRACE(GUI_CONTROLS_MARKER, FUNC_NAME,
                              _T("Removing Menu Item :") + l_sub_menu_string);

                    //+Patni-Hemant/2010May14/Modified/JaFT/TMSC-REDMINE-273
                    //
                    //delete the sub menu for which match has found
                    l_sub_menu->DeleteMenu(l_sub_index, MF_BYPOSITION);

                    //delete the main menu if all sub menus are deleted
                    if (1 == l_smenu_cnt) {
                        f_menu->DeleteMenu(l_index, MF_BYPOSITION);

                        PQM_TRACE(GUI_CONTROLS_MARKER, FUNC_NAME,
                                  _T("Removing Sub Menu :") + l_sub_menu_string);
                    }

                    //-Patni-Hemant/2010May14/Modified/JaFT/TMSC-REDMINE-273

                    return ;
                }
            }

        } else {

            f_menu->GetMenuString(l_index, l_menu_item_string, MF_BYPOSITION);

            if (l_menu_item_string.CompareNoCase(f_menu_item_to_remove) == 0) {

                f_menu->DeleteMenu(l_index, MF_BYPOSITION);

                PQM_TRACE(GUI_CONTROLS_MARKER, FUNC_NAME,
                          _T("Removing Menu :") + l_menu_item_string);
                return ;
            }
        }
    }
}
//-Patni-AMT/2009Nov10/Commented/DeFT/MCM0229-00154



void CCoilLstDlg::UpdateACoilMenu(
)
{
    LPCTSTR FUNC_NAME = _T("CCoilLstDlg::UpdateACoilMenu");

    if (NULL == m_selection_utiliy) {
        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));

        return;
    }

    CreateACoilMenu();

    //update the menu for showing next time..

    //if currently selected item is not null then get the total occurences of it
    //if the total exceds the alloablw then remove that item from menu..
    //
    //for old coil item..if not null, restore it in menu..

    CoilSelectionSelectCoil_t* l_coil_struct = NULL ;
    CString l_coil_lable = _T("") ;

    for (int l_index = 0; l_index < MAX_COUCH_PORT ; l_index++) {
        l_coil_struct = m_selection_utiliy->GetSelectedCoil(l_index);

        if (l_coil_struct && l_coil_struct->coil) {

            l_coil_lable = l_coil_struct->coil->label ;

            if (GetSameCoilCount(l_coil_lable) >= l_coil_struct->coil->numSCoil)
                RemoveMenuItemFromMenu(pAPortMenu, l_coil_lable) ;
        }
    }
}



void CCoilLstDlg::ResetCoilDataForDisplayInformation(
    CoilSelectionSelectCoil_t* f_local
)
{
    if (f_local) {

        f_local->coil = NULL ;
        f_local->port = 0 ;
        f_local->position = 0 ;
    }
}


void CCoilLstDlg::DisplayGCoilFrom(
    const CoilSelectionSelectCoil_t* f_local,
    const GCoilItems f_gcoilitem
)const
{
    //+REDMINE_781
    /*

    LPCTSTR FUNC_NAME = _T("CCoilLstDlg::DisplayGCoilFrom");
    int button_id = GetButtonIDFor(f_gcoilitem) ;

    //Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL != f_local) {
        if (f_local->coil && f_local->coil->label) {

            if (button_id != -1)
                SetButtonTextForID(button_id, f_local->coil->label);

            return ;
        }

        //+Patni-DKH/2009Aug24/Modified/cpp test corrections

    } else {

        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("f_local pointer is NULL"));

        return;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (button_id != -1)
        SetButtonTextForID(button_id, _T(""));

    */
    DisplayGLCoil(GetButtonIDFor(f_gcoilitem), f_local);
    //-REDMINE_781
}


void CCoilLstDlg::DisplayLCoilFrom(
    const CoilSelectionSelectCoil_t* f_local,
    const LCoilItems f_lcoilitem
)const
{

    //+REDMINE_781
    /*
    LPCTSTR FUNC_NAME = _T("CCoilLstDlg::DisplayLCoilFrom");
    int button_id = -1 ;
    button_id = GetButtonIDFor(f_lcoilitem);

    //Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL != f_local) {
        if (f_local->coil && f_local->coil->label) {

            CString l_label(f_local->coil->label);

            if (button_id != -1) {

                SetButtonTextForID(button_id, l_label);
            }

            return ;
        }

        //+Patni-DKH/2009Aug24/Modified/cpp test corrections

    } else {

        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("f_local pointer is NULL"));

        return;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    if (button_id != -1)
        SetButtonTextForID(button_id, _T(""));

    */
    //-REDMINE_781
    DisplayGLCoil(GetButtonIDFor(f_lcoilitem), f_local);

    //ASSERT(FALSE);
    return ;
}


void CCoilLstDlg::DisplayACoilFrom(
    const CoilSelectionSelectCoil_t* f_local,
    const ACoilItems f_acoilitem
)const
{
    int button_id = -1 ;

    if (f_local && f_local->coil) {

        CString l_label(f_local->coil->label);

        button_id = GetButtonIDFor(f_acoilitem) ;

        if (button_id != -1) {

            SetButtonTextForID(button_id, l_label);
            GetDlgItem(button_id)->EnableWindow();
        }

        //set text for port combo..and enable combo
        button_id = GetButtonIDFor(f_acoilitem, ACoilPortButtons) ;

        if (button_id != -1) {

            if (f_local->port != 0)
                l_label.Format(_T("A%d"), f_local->port);

            else {
                l_label = _T("") ;
            }

            SetButtonTextForID(button_id, l_label);
            GetDlgItem(button_id)->ShowWindow(l_label.IsEmpty() ? SW_HIDE : SW_SHOW);
            //GetDlgItem(button_id)->EnableWindow();
        }

        //set text for position combo..and enable combo
        button_id = GetButtonIDFor(f_acoilitem, ACoilPositionButtons) ;

        if (button_id != -1) {

            if (f_local->position != 0) {
                l_label.Format(_T("%d"), f_local->position);

            } else {
                l_label = _T("") ;
            }

            CString coildstring = (f_local->coil->label) ;
            GetDlgItem(button_id)->EnableWindow(!coildstring.IsEmpty()) ;
            SetButtonTextForID(button_id, l_label);
        }

        return ;

    } else {

        if (!f_local || !f_local->coil) {

            CString l_label(_T(""));

            button_id = GetButtonIDFor(f_acoilitem) ;

            if (button_id != -1) {

                SetButtonTextForID(button_id, l_label);
            }

            //set text for port combo..and enable combo
            button_id = GetButtonIDFor(f_acoilitem, ACoilPortButtons) ;

            if (button_id != -1) {

                SetButtonTextForID(button_id, l_label);
                GetDlgItem(button_id)->ShowWindow(l_label.IsEmpty() ? SW_HIDE : SW_SHOW);
            }

            //set text for position combo..and enable combo
            button_id = GetButtonIDFor(f_acoilitem, ACoilPositionButtons) ;

            if (button_id != -1) {

                //+Patni-Hemant/2009Mar17/COMMENTED/ACE-1/, not needed
                //GetDlgItem(button_id)->EnableWindow(FALSE);

                SetButtonTextForID(button_id, l_label);
            }

            return ;

        }
    }

    if (button_id != -1) {
        SetButtonTextForID(button_id, _T(""));
        GetDlgItem(button_id)->EnableWindow(FALSE);
    }

    //ASSERT(FALSE);
    return ;
}


//+REDMINE_781
void CCoilLstDlg::DisplayGLCoil(
    const int f_button_id,
    const CoilSelectionSelectCoil_t* f_local
) const
{

    LPCTSTR FUNC_NAME = _T("CCoilLstDlg::DisplayGLCoil");

    if (NULL == f_local) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("f_local pointer is NULL"));
        return;
    }

    if (f_button_id != -1) {

        if (f_local->coil && f_local->coil->label) {
            SetButtonTextForID(f_button_id, f_local->coil->label);

        } else {
            SetButtonTextForID(f_button_id, _T(""));
        }
    }
}
//-REDMINE_781


void CCoilLstDlg::ClearSelectedCoilInformation(
)
{
    LPCTSTR FUNC_NAME = _T("CCoilLstDlg::ClearSelectedCoilInformation");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_selection_utiliy) {

        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));

        return;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections
    m_selection_utiliy->CoilSelectionClearPqmListCoilMenu();

    ClearCoilInformation(CoilType_G);
    ClearCoilInformation(CoilType_L);
    ClearCoilInformation(CoilType_A);

}


//Patni-Sribanta/2010Aug10/Modified/TMSC-REDMINE-581
bool CCoilLstDlg::CoilUiListDlgOkCB(
    const bool f_ignore_pos_change /*= false*/
)const
{
    LPCTSTR FUNC_NAME = _T("CCoilLstDlg::CoilUiListDlgOkCB");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //CoilSelectionDialogBtn(1);

    //+Patni-Hemant/2010Mar4/Commented/ACE-Phase#3/CDS Requirement

    //    CString l_str ;
    //    MessageBoxTypeEnum messageboxtypeenum = MessageBoxType_OK_CANCEL;
    //-Patni-Hemant/2010Mar4/Commented/ACE-Phase#3/CDS Requirement

    //Modified by Hemant - we should display the changes here...
    //so if is removed..
    //	if(!m_selection_utiliy->CoilCheckList(TRUE, l_str, messageboxtypeenum)){
    //			return;
    //	}

    //+Patni-Hemant/2009Mar17/Modify/ACE-1/Internal defect, removing the MFC messagebox
    //m_selection_utiliy->CoilCheckList(TRUE, l_str, messageboxtypeenum);

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_selection_utiliy) {

        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));

        //+Patni-Sribanta/2010Aug10/Added/TMSC-REDMINE-581
        return false;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections
    //+Patni-Hemant/2010Feb15/Modified/ACE-Phase#3/CDS Requirement
    //if (m_selection_utiliy->CoilCheckList(TRUE, l_str, messageboxtypeenum)) {
    if (m_selection_utiliy->CoilCheckList(FALSE) == COIL_S_NO_ERROR) {
        //-Patni-Hemant/2010Feb15/Modified/ACE-Phase#3/CDS Requirement
        //-Patni-Hemant/2009Mar17/Modify/ACE-1/Internal defect, removing the MFC messagebox

        m_selection_utiliy->SetAppliedCoil();
        m_selection_utiliy->CoilSetCoilButton(false, -1, f_ignore_pos_change);
        m_selection_utiliy->CoilMapCoilButton();
        m_selection_utiliy->SetToPqmListCoil();

        //+Patni-Sribanta/2010Aug10/Added/TMSC-REDMINE-581
        return true ;
    }

    //+Patni-Sribanta/2010Aug10/Added/TMSC-REDMINE-581
    return false ;
}


void CCoilLstDlg::UpdateMarkerWindows(
)
{
    LPCTSTR FUNC_NAME = _T("CCoilLstDlg::UpdateMarkerWindows");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_selection_utiliy) {

        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections


    MarkerSignEnum l_markersignenum = MarkerSign_Invalid;
    CWnd* l_static_wnd = NULL ;

    for (int l_button_index = 0 ; l_button_index < MAX_COIL_BUTTON; l_button_index++) {

        l_markersignenum = m_selection_utiliy->GetMarkerSignForIndex(l_button_index) ;

        //+Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement
        //        //+Patni-HAR/2009April09/Added/Code Review
        //        if (l_markersignenum == MarkerSign_Invalid) {
        //            continue;
        //
        //        } else {
        //            //-Patni-HAR/2009April09/Added/Code Review
        //-Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement

        l_static_wnd = GetDlgItem(IDC_STATIC1 + l_button_index) ;

        if (l_static_wnd && l_static_wnd->GetSafeHwnd() != NULL) {
            l_static_wnd->SetWindowText(GetStringForMarkerSign(l_markersignenum)) ;
        }

        //+Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement
        //        }
        //-Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement
    }

}



//****************************Method Header************************************
//Method Name   :OnButtonApply()
//Author        :PATNI / NJ
//Purpose       :
//*****************************************************************************
bool CCoilLstDlg::ApplyCoilInformation(
)const
{
    LPCTSTR FUNC_NAME = _T("CCoilLstDlg::ApplyCoilInformation");
    PQM_TRACE_SCOPED(FUNC_NAME);

    CCoilSelectionUtility* l_utility = m_selection_utiliy;

    if (!l_utility) {
        return false;
    }

    //+Patni-Hemant/2010Feb12/Modified/ACE-Phase#3/CDS Requirement
    //int l_index = 0;
    //CString l_str(_T(""));
    //MessageBoxTypeEnum messageboxtypeenum = MessageBoxType_OK_CANCEL;

    //if (!l_utility->CoilCheckList(TRUE, l_str, messageboxtypeenum)) {
    //
    //		return false;
    //
    //
    //       //display the message box for user query...and do accordingly..
    //
    //        //+Patni-PP/2009May28/Modified/SU14 CPP test initialization corrections
    //        MPLUSERRORUSERRESPONSE_e l_response = MPlusErrResponse_Error;
    //
    //        //-Patni-PP/2009May28/Modified/SU14 CPP test initialization corrections
    //        switch (messageboxtypeenum) {
    //
    //            case  MessageBoxType_OK_CANCEL  :
    //                l_response = CCoilSelectionUtility::DisplayMessageinWarningDialog(l_str, SCAN_WARN_OK_CANCEL_BUTTONS);
    //                break ;
    //
    //            case  MessageBoxType_OK  :
    //                l_response = CCoilSelectionUtility::DisplayMessageinWarningDialog(l_str, SCAN_WARN_OKBUTTON);
    //                break ;
    //
    //            case  MessageBoxType_CANCEL :
    //                l_response = CCoilSelectionUtility::DisplayMessageinWarningDialog(l_str, SCAN_WARN_OKBUTTON);
    //                break ;
    //
    //            default :
    //                l_response = CCoilSelectionUtility::DisplayMessageinWarningDialog(l_str, SCAN_WARN_OK_CANCEL_BUTTONS);
    //
    //        }
    //
    //        switch (messageboxtypeenum) {
    //
    //            case  MessageBoxType_OK_CANCEL  :
    //            case  MessageBoxType_OK  :
    //
    //                CoilSelectionOKCancel(l_response == MPlusErrResponse_OK) ;
    //
    //
    //                break ;
    //
    //            case  MessageBoxType_CANCEL  :
    //
    //                break ;
    //
    //        }
    //
    //        //if we have displayed Ok and cancel button..and user has pressed Ok
    //        //then if
    //        switch (messageboxtypeenum) {
    //
    //            case  MessageBoxType_OK_CANCEL  :
    //            case  MessageBoxType_OK :
    //
    //                if (l_response != MPlusErrResponse_OK)
    //                    return false ;
    //
    //                break ;
    //
    //            default :
    //                return false ;
    //        }
    //    }
    if (l_utility->CoilCheckList(TRUE) != COIL_S_NO_ERROR) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("CoilCheckList returned Error"));

        return false;
    }

    //-Patni-Hemant/2010Feb12/Modified/ACE-Phase#3/CDS Requirement
    l_utility->SetAppliedCoil();

    l_utility->CoilSetCoilButton();
    l_utility->CoilMapCoilButton();

    m_selection_utiliy->SetToPqmListCoil();

    //+Patni-Hemant/2010Mar3/Added/ACE-Phase#3/CDS Requirement
    //Set the appropriate button on Coil Selection Window
    //m_ccoilselwininterface->DisplayButtonsOnAxisLocator();
    //-Patni-Hemant/2010Mar3/Added/ACE-Phase#3/CDS Requirement

    return true ;

}


void CCoilLstDlg::PrepareButtonClickIDAndCoilTypeClickedID(
    const int f_button_id,
    const bool f_for_port /*= false*/
)
{

    switch (f_button_id) {
        case IDC_BUTTON_POS_1 :
        case IDC_BUTTON_POS_2 :
        case IDC_BUTTON_POS_3 :
        case IDC_BUTTON_POS_4 :
        case IDC_BUTTON_POS_5 :
        case IDC_BUTTON_POS_6 :
        case IDC_BUTTON_POS_7 :

        case IDC_BUTTON_PORT_1 :
        case IDC_BUTTON_PORT_2 :
        case IDC_BUTTON_PORT_3 :
        case IDC_BUTTON_PORT_4 :
        case IDC_BUTTON_PORT_5 :
        case IDC_BUTTON_PORT_6 :
        case IDC_BUTTON_PORT_7 :    {

            if (f_for_port)
                iAIndex = (f_button_id - IDC_BUTTON_PORT_1) ;

            else
                iAIndex = (f_button_id - IDC_BUTTON_POS_1) ;
        }
        break ;

        case IDC_BUTTON_POS_8 :
        case IDC_BUTTON_POS_9 : {

            iLIndex = (f_button_id - IDC_BUTTON_POS_8) ;
        }
    }

    ubtnID = f_button_id;
}


void CCoilLstDlg::ClearGCoilMenu(
)
{

    UTIL::DeleteAndClearMFCList<PopupMenuEx>(&m_pgportsubmenu_list);
    DEL_PTR(pGPortMenu)

}


//+Patni-AMT/2009Nov10/Commented/DeFT/MCM0229-00154
void CCoilLstDlg::ClearACoilMenu(
)
{
    UTIL::DeleteAndClearMFCList<PopupMenuEx>(&m_paportsubmenu_list);
    DEL_PTR(pAPortMenu)

}
//-Patni-AMT/2009Nov10/Commented/DeFT/MCM0229-00154

CoilSelectionCoil_t* CCoilLstDlg::GetSelectionCoilFromCouchDataFor(
    const CString& f_coil_name
)const
{
    LPCTSTR FUNC_NAME = _T("CCoilLstDlg::GetSelectionCoilFromCouchDataFor");

    //-Patni-Dhanesh/2009Aug24/Modified/cpp test corrections
    if (NULL == m_selection_utiliy) {

        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("m_selection_utiliy pointer is NULL"));

        return NULL ;
    }

    //-Patni-Dhanesh/2009Aug24/Modified/cpp test corrections

    DllT* l_couch_head = m_selection_utiliy->GetHead();//GetCouchHead(0);

    DllT* l_trav = NULL;

    CoilSelectionCoil_t* l_coil_struct = NULL ;

    CString l_label(_T(""));

    for (l_trav = l_couch_head ; l_trav != NULL; l_trav = l_trav->next) {

        l_coil_struct = (CoilSelectionCoil_t*)l_trav->item;
        l_label = l_coil_struct->label;

        if (!l_label.CompareNoCase(f_coil_name))
            return l_coil_struct;
    }

    return NULL ;
}


int CCoilLstDlg::GetOccurencesForCoil(
    const CoilSelectionSelectCoil_t* f_coil,
    int** f_same_coil_index
)
{
    if (!f_coil) {

        ASSERT(FALSE);
        return -1;
    }

    int l_same_count = 0;
    CoilSelectionSelectCoil_t* l_current_coil =  NULL;

    for (int l_coil_button = 0 ; l_coil_button < MAX_COIL_BUTTON ; l_coil_button ++) {

        l_current_coil = m_selection_utiliy->GetSelectedCoil(l_coil_button);

        if (l_current_coil->coil == f_coil->coil)
            l_same_count ++ ;
    }

    if (l_same_count) {

        (*f_same_coil_index) = new int[l_same_count] ;

        l_same_count = 0;

        for (int l_coil_button = 0 ; l_coil_button < MAX_COIL_BUTTON ; l_coil_button ++) {

            l_current_coil = m_selection_utiliy->GetSelectedCoil(l_coil_button);

            if (l_current_coil->coil == f_coil->coil)  {
                (*f_same_coil_index)[l_same_count] = l_coil_button ;
                l_same_count ++ ;
            }
        }


    }

    return l_same_count ;
}


// Added By KT/ACE-2/11-06-2009/Multilingualisation
void CCoilLstDlg::SetMultilingual()
{
    // other controls
    CString l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_BUTTON_APPLY"));
    HWND l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDC_BUTTON_APPLY);

    if (_tcscmp(l_chOutput , _T("")) != 0) {
        ::SetWindowText(l_buttonUserHwnd, l_chOutput);
    }

    l_chOutput = _T("");
    l_buttonUserHwnd = NULL;

    l_chOutput =  CPQMUtility::GetMultiLingualString(_T("IDS_PQM_BUTTON_OK"));
    l_buttonUserHwnd = ::GetDlgItem(m_hWnd, IDOK);

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
}
// Added By KT/ACE-2/11-06-2009/Multilingualisation

//****************************Method Header************************************
//Method Name   :SetButtonFont()
//Author        :PATNI / NJ
//Purpose       :
//*****************************************************************************
BOOL CCoilLstDlg::SetButtonFont()
{
    CString sFontName;
    sFontName.LoadString(IDS_FONT_NAME);

    int iFontSize = AFX_IDS_FONTSIZE;

    for (int i = 0; i < AFX_ID_COIL_MAX; i++) {
        m_btnCoil[i].SetButtonStyle(TUSBUTTON_ROUND_ALL);
        m_btnCoil[i].SetPrimaryTextFont(sFontName, iFontSize);

        m_btnCoil[i].SetCurveType(CURVE_SMALL_3);

        m_btnCoil[i].SetPrimaryTextAllign(TUSBUTTON_ALIGN_LEFT_VCENTER);
        SetBitmapForButton(m_btnCoil[i], IDB_BITMAP_COIL_LISTSEL, false, true);
    }

    for (i = 0; i < AFX_ID_POSITION_MAX; i++) {
        m_btnPos[i].EnableHoverEffect(false);
        m_btnPos[i].SetButtonStyle(TUSBUTTON_ROUND_ALL);
        m_btnPos[i].SetPrimaryTextFont(sFontName, iFontSize);

        m_btnPos[i].SetCurveType(CURVE_SMALL_3);
        m_btnPos[i].SetPrimaryTextAllign(TUSBUTTON_ALIGN_LEFT_VCENTER);
        SetBitmapForButton(m_btnPos[i], /*IDB_BITMAP_POS*/IDB_BITMAP_COIL_LISTSEL, false, true);
    }

    for (i = 0; i < AFX_ID_PORT_MAX; i++) {
        m_btnPort[i].EnableHoverEffect(false);
        m_btnPort[i].SetButtonStyle(TUSBUTTON_ROUND_ALL);

        m_btnPort[i].SetPrimaryTextFont(sFontName, iFontSize);
        m_btnPort[i].SetCurveType(CURVE_SMALL_3);
        m_btnPort[i].SetPrimaryTextAllign(TUSBUTTON_ALIGN_LEFT_VCENTER);
        SetBitmapForButton(m_btnPort[i], /*IDB_BITMAP_PORT*/IDB_BITMAP_COIL_LISTSEL, false, true);
        m_btnPort[i].ShowWindow(SW_HIDE);
    }

    //Added by Hemant On 12/11/2008 12:59:24 PM
    sFontName = _T("Verdana") ;
    iFontSize = 13;
    m_btnCancel.SetPrimaryTextFont(sFontName, iFontSize);
    m_btnOK.SetPrimaryTextFont(sFontName, iFontSize);
    m_btnApply.SetPrimaryTextFont(sFontName, iFontSize);
    //

    return TRUE;
}

//************************************Method Header************************************
// Method Name  : SetBitmapForButton
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
BOOL CCoilLstDlg::SetBitmapForButton(
    ButtonEx& button_object,
    const UINT bitmap_resource_id,
    const bool center_bitmap /*= true*/,
    const bool right_align /*= false */
)const
{
    return CUIButtonUtility::GetInstance()->SetBitmapForButton(button_object, bitmap_resource_id, RGB(0, 255, 0), center_bitmap, right_align);
}


int CCoilLstDlg::GetSameCoilCount(const CString& sCoil)const
{
    int iCount = 0;
    CString sbtnText = _T("");

    for (int i = 0 ; i < AFX_ID_COUCH_MAX; i++) {
        m_btnCoil[i].GetWindowText(sbtnText);

        if (!sCoil.CompareNoCase(sbtnText)) {
            ++iCount;

        } else {
            continue;
        }
    }

    return iCount;
}


CString CCoilLstDlg::GetStringForMarkerSign(
    const MarkerSignEnum markersignenum
)const
{

    switch (markersignenum) {


        case MarkerSign_Plus :
            return _T("+") ;

        case MarkerSign_Minus :
            return _T("-") ;

        default :
            //case MarkerSign_None :
            //case MarkerSign_Invalid

            return _T("") ;
    }
}



int CCoilLstDlg::GetButtonIDFor(
    const GCoilItems f_gcoilitem
)const
{

    switch (f_gcoilitem) {

        case GCoilItems_1 :

            return IDC_BUTTON_COIL_10 ;
            break ;

        default :

            ASSERT(FALSE);
            return -1;

    }

    return -1 ;
}


int CCoilLstDlg::GetButtonIDFor(
    const LCoilItems f_lcoilitem,
    const LCoilButtonType lcoilbuttontype /*= LCoilCoilButtons*/
)const
{
    int button_id = -1;

    switch (lcoilbuttontype)  {

        case LCoilCoilButtons:
            button_id = IDC_BUTTON_COIL_8 ;
            break ;

        case LCoilPositionButtons :
            button_id = IDC_BUTTON_POS_8 ;
            break ;
    }

    if (button_id != -1) {
        button_id += (int)f_lcoilitem ;
    }

    return button_id ;
}


int CCoilLstDlg::GetButtonIDFor(
    const ACoilItems f_acoilitem,
    const ACoilButtonType acoilbuttontype /*= ACoilCoilButtons*/
)const
{

    int button_id = -1;

    switch (acoilbuttontype)  {

        case ACoilPortButtons:

            button_id = IDC_BUTTON_PORT_1 ;
            break ;

        case ACoilCoilButtons:
            button_id = IDC_BUTTON_COIL_1 ;
            break ;

        case ACoilPositionButtons :
            button_id = IDC_BUTTON_POS_1 ;
            break ;
    }

    if (button_id != -1) {
        button_id += (int)f_acoilitem ;
    }

    return button_id ;
}


//+Patni-Hemant/2010Feb25/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : GetCouchButton
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
ButtonEx* CCoilLstDlg::GetCouchButton(
    const ACoilButtonType f_acoilbuttontype,
    const ACoilItems f_acoilitems
)
{
    ButtonEx* l_buton = NULL ;

    if ((f_acoilitems >= ACoilItems_1) && (f_acoilitems < ACoilItems_TOTAL)) {

        switch (f_acoilbuttontype) {

            case ACoilPortButtons   :
                l_buton = & m_btnPort[f_acoilitems] ;

                break ;

            case ACoilCoilButtons:
                l_buton = & m_btnCoil[f_acoilitems] ;

                break ;

            case ACoilPositionButtons:
                l_buton = & m_btnPos[f_acoilitems] ;

                break ;
        }
    }

    return l_buton ;
}
//-Patni-Hemant/2010Feb25/Added/ACE-Phase#3/CDS Requirement


int CCoilLstDlg::GetBaseIndexFor(
    const CoilTypes coiltypes
)const
{

    return CCoilSelectionUtility::GetBaseIndexFor(coiltypes) ;
}

void CCoilLstDlg::SetDefaultColor(ButtonEx* pbtn)
{
    pbtn->SetButtonColors(m_btnDefColor);
}


BOOL CCoilLstDlg::SetColorButton(ButtonEx* pbtn, const COLORREF cColor)const
{
    BUTTON_COLORS_t btnColor;
    pbtn->GetButtonColors(btnColor);
    btnColor.crBrushEdgeStart = cColor;	//Red Color
    btnColor.crBrushEdgeEnd	  = cColor;
    btnColor.crBottomEdgeShadow = cColor;
    btnColor.crbrushCircleStart = cColor;
    btnColor.crbrushCircleEnd = cColor;
    btnColor.crBottomEdgePushed = cColor;
    btnColor.crUpperEdgeShadow = cColor;
    btnColor.crFocusRect = cColor;
    pbtn->SetButtonColors(btnColor);
    return TRUE;
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCoilLstDlg::SetDialogRibbonHeight(
)
{
    CRect client_rect;
    GetClientRect(&client_rect);

    CRect ok_button_rect ;
    m_btnCancel.GetWindowRect(&ok_button_rect);
    ScreenToClient(&ok_button_rect);

    //const int height_diff = client_rect.bottom - ok_button_rect.top + RIBBON_GAP;
    SetRibbonHeight(/*height_diff*/ client_rect.bottom - ok_button_rect.top + RIBBON_GAP);
}


void CCoilLstDlg::SetColorForMarkerStatic(
    const int f_marker_index
)
{
    COLORREF l_normal_color = RGB(13, 16, 26) ;

    switch (f_marker_index) {

        case 1 :
        case 2 :
            l_normal_color = RGB(17, 26, 42);
            break ;
    }

    const COLORREF l_text_color = RGB(255, 255, 255) ;

    m_marker_sign_statics[f_marker_index].SetColorData(l_normal_color , l_text_color);

    m_marker_sign_statics[f_marker_index].SetFontAttributes(_T("Arial"), 150);

    m_marker_sign_statics[f_marker_index].UpdateFont();
}




void CCoilLstDlg::SetButtonTextForID(
    const UINT f_button_id,
    const CString& f_button_string
)const
{
    if (f_button_id > 0) {
        //+Patni-DKH/2009Aug26/Modified/cpp test corrections
        CWnd* l_dlgitem = GetDlgItem(f_button_id);

        if (NULL != l_dlgitem) {
            l_dlgitem->SetWindowText(f_button_string);
        }

        //-Patni-DKH/2009Aug26/Modified/cpp test corrections
    }
}
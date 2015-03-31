//REDMINE_782_CPP_Test_Tool
//TMSC-REDMINE-782
//TMSC-REDMINE-781
//TMSC-Redmine-780
//TMSC-REDMINE-778
//TMSC-REDMINE-777
//redmine-779


/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMListCtrl.cpp
 *  Overview: Implementation of CPQMListCtrl class.
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
#include "PQMListCtrl.h"
#include <tami/vfStudy/vf_appcodes.h>
#include "MainFrm.h"
#include "PQMView.h" //Patni-NP/2009Sept15/Added/MVC#004766
#include "PQMImageUtility.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern Data_decl_list  g_sequence_param_name_list;
//Patni-NP/2009Sept22/Added/MVC#004914
extern CPQMView* g_MainView;

const CString PQM_DISP_DICT = _T("PQM_Display_Dictionary.xml");

//-------------------------------------------------------------------------
//Added by Hemant On 7/1/2008 3:26:03 PM
static const int SCROLL_TIMER_ID		= 1;
#define DRAW_TRANSPARENT	TRUE
#define MODE_ALIGN_CENTER	TRUE

//+Patni-Manish/2009May26/Added/JFT#IR18/GUI Renewal
//+Patni-RUP/2010Jul19/Modified/TMSC-REDMINE-466
//#define TOTAL_MODE_FALGS	2
#define TOTAL_MODE_FALGS	3
//-Patni-RUP/2010Jul19/Modified/TMSC-REDMINE-466

//Patni-Hemant/2009Nov15/Modified/IR#90/Queue List
#define MODE_OPTION_BITMAP_WIDTH   28

//-Patni-Manish/2009May26/Added/JFT#IR18/GUI Renewal

//Patni-Hemant/2009Nov15/Modified/IR#90/Queue List
#define MODE_OPTION_BITMAP_HEIGHT  28

#define MODE_OPTIONS_GAP	3
#define AUTO_SCAN_BITMAP_GAP 3
#define X_ADJUSTMENT_FOR_AUTO_SCAN_BITMAP 2

//Patni-Hemant/2009Nov15/Modified/IR#90/Queue List
#define ITEM_HEIGHT 38
//-------------------------------------------------------------------------
const int DELAY_TIME_WIDTH = 70;//Redmine-2187

BEGIN_MESSAGE_MAP(CPQMListCtrl, ListCtrlEx)
    //{{AFX_MSG_MAP(CPQMListCtrl)

    ON_WM_MEASUREITEM_REFLECT()	//added by Hemant
    ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged)		//added by Hemant

    ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)		//added by Hemant
    ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)		//added by Hemant

    ON_MESSAGE(WM_HIDE_CONTEXT, HideContexMenu)
    ON_WM_CAPTURECHANGED()

    //-------------------------------------------------------------------------
    //added by Hemant - for drag drop..
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnBeginDrag)
    ON_WM_TIMER()
    ON_WM_LBUTTONDOWN()
    ON_WM_RBUTTONDOWN()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_RBUTTONDBLCLK()
    ON_WM_KEYDOWN()
    //-------------------------------------------------------------------------

    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


//************************************Method Header**************************************
//Method Name:  CPQMListCtrl
//Author:  PATNI/SVP
//Purpose: Constructor.
//**************************************************************************************/
CPQMListCtrl::CPQMListCtrl(
):	m_ScrollDirection(scrollNull),
    m_pDragImage(NULL),
    m_number_column_alignment(LVCFMT_RIGHT),
    m_scan_id_column_alignment(LVCFMT_LEFT),
    m_rightclick_cell(CPoint(-1, -1)),
    m_last_planned_protocol(-1),
    m_nDropIndex(-1),
    m_nPrevDropIndex(-1),
    m_uPrevDropState(NULL),
    m_dwStyle(NULL),
    m_isdraging(false),
    m_uScrollTimer(0),
    m_bias_value_for_protocol_number(DEFAULT_SERIES_BIAS),
    m_trsc_index(-1),
    m_mode_column_index(-1),
    m_protocolno_column_index(-1),
    m_status_column_index(-1),
    m_time_column_index(-1),
    m_sar_column_index(-1),
    m_scan_id_column_index(-1),
    m_plan_column_index(-1),
    m_delay_column_index(-1),
    m_dic_manager(NULL),
    m_list(NULL),
    m_pwchtip(NULL)
{
    ResetWSEEditingProtocolIndex();
}


//************************************Method Header**************************************
//Method Name:  ~CPQMListCtrl
//Author:  PATNI/SVP
//Purpose: Destructor.
//**************************************************************************************/
CPQMListCtrl::~CPQMListCtrl(
)
{
    //+Patni-ARD/2010Jan27/Added/DeFT# MVC3817
    DEL_PTR(m_pDragImage)

    //-Patni-ARD/2010Jan27/Added/DeFT# MVC3817

    DEL_PTR_ARY(m_pwchtip)

    //+Patni-Manish/2009May29/Added/MVC002100-ToolTip Modification
    m_tooltip_font.DeleteObject();
    //-Patni-Manish/2009May29/Added/MVC002100-ToolTip Modification

    m_plan_icon_font.DeleteObject();

    //Added by Hemant On 6/20/2008 7:02:45 PM - for drag drop.
    KillScrollTimer();
    //Added by Hemant On 12/11/2008 3:33:15 PM
    m_column_default_width_array.RemoveAll();
}



//************************************Method Header****************************
//Author      :  PATNI/GP
//Purpose     :  This method will display TRSC value.
//*****************************************************************************
void CPQMListCtrl::DisplayTRSC(
    const int series_index,
    const CString& str_trsc)
{

    if (m_trsc_index >= 0)
        SetItemText(series_index, m_trsc_index, str_trsc);

}



//************************************Method Header**************************************
//Method Name :  UpdateProtocolStatus
//Author      :  PATNI/DJ
//Purpose     :  Update the Acquisition status of selected protocol.
//**************************************************************************************/
BOOL CPQMListCtrl::UpdateProtocolStatus(
    const int selected_index,
    const int status
)
{

    LPCTSTR FUNC_NAME = _T("CPQMListCtrl::UpdateProtocolStatus");
    PQM_TRACE_SCOPED(FUNC_NAME);

    if (!IsValidSequenceIndex(selected_index)) {

        ASSERT(FALSE);
        return FALSE;
    }

    DicParamConvertedValueList  l_converted_value_list;
    DicParamNameList            l_param_name_list;

    SEQUENCE_PARAM l_param[100] = {0};	//Patni-PP/2009Jun01/Modified/SU14 CPP test initialization corrections

    CSequence* l_seq = (CSequence*) GetItemData(selected_index);

    l_seq->SetAcqStatus(status);

    Data_decl_list l_sequence_param_name_list;
    l_seq->GetSequenceParamStruct(&l_param[0], l_sequence_param_name_list);

    ApplyParameterDictionary(l_param[0], l_converted_value_list, l_param_name_list, l_sequence_param_name_list);

    //+Patni-Hemant/2009Nov15/Modified/IR#90/Queue List
    //InvalidateItemsForStatus(selected_index);
    Invalidate(FALSE);
    //-Patni-Hemant/2009Nov15/Modified/IR#90/Queue List

    return TRUE;
}




//************************************Method Header************************************
// Method Name  : CreateListCtrl
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPQMListCtrl::CreateListCtrl(
    const UINT id,
    const CRect& rect,
    CWnd* parentwnd
)
{
    DWORD style = WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_OWNERDRAWFIXED | LVS_REPORT | LVS_SHOWSELALWAYS ;

    Create(style, rect, parentwnd, id);
    SetExtendedStyle(GetExtendedStyle() /*| LVS_EX_GRIDLINES*/ | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);
    ModifyStyle(GetStyle(), GetStyle() | LVS_REPORT | LVS_OWNERDRAWFIXED | LVS_SHOWSELALWAYS | LVS_EX_TRACKSELECT);
    //+Patni-Manish/2009May29/Added/MVC002100-ToolTip Modification
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
    //-Patni-Manish/2009May29/Added/MVC002100-ToolTip Modification

    m_plan_icon_font.CreateFont(17,
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
                                _T("VERDANA BOLD")
                               );
    EnableToolTips();

    return true ;
}

//************************************Method Header************************************
// Method Name  : DeleteSelectedProtocols
// Author       : PATNI/ HEMANT
// Purpose      : This function will delete all the selected protocols.. and also manage the m_list variable for the same..
//***********************************************************************************
bool CPQMListCtrl::DeleteSelectedProtocols(
)
{
    //+Patni-DKH/2010Nov19/Added/TMSC-REDMINE-923
    LPCTSTR FUNC_NAME = _T("CPQMListCtrl::DeleteSelectedProtocols");

    //Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Actual Protocol deletion Start"));

    try {
        //-Patni-DKH/2010Nov19/Added/TMSC-REDMINE-923
        // CPQMView* p_parent = (CPQMView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

        CPQMView* p_parent  = g_MainView;

        //+Patni-Dhanesh/2009Aug20/Modified/cpp test corrections
        if (NULL == p_parent) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("p_parent pointer is NULL"));
            return false;

        } else {
            //-Patni-Dhanesh/2009Aug20/Modified/cpp test corrections

            int* selected_items = NULL ;
            int count = GetselectedItems(&selected_items);

            if (!count || !selected_items) {

                //Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("Actual Protocol Deletion Ends with NO Item to delete..."));

                DEL_PTR_ARY(selected_items)

                return false;
            }

            //Patni-HAR/2010Jan29/Added/DeFT#MVC006364

            if (p_parent->CheckForScanIndexAndMarkForDelete(selected_items, count)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Scanning Index selected for deletion"));

                DEL_PTR_ARY(selected_items);
                return false;
            }

            int l_can_update_group_num = p_parent->OnDeleteUpdateMasterSlave(selected_items, count);

            //+Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1
            CString l_str = _T("");
            l_str.Format(_T("Total items for  Deletion are : %d"), count);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
            //-Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1

            //+ Patni-RJ/2009Mar29/Added/PSP- Performance code
#ifdef __SM_PERFORMANCE_CHECK__
            __int64 l_freq, l_stop, l_start;

            QueryPerformanceCounter((LARGE_INTEGER*)&l_start);
#endif

            //- Patni-RJ/2009Mar29/Added/PSP- Performance code
            //Patni-HAR/2010Jan18/Added/DeFT#MVC006364

            // + Patni- Sribanta/2010Jun10/Added/MVC7933
            p_parent->SendDeleteReserveDEGL();
            // - Patni- Sribanta/2010Jun10/Added/MVC7933

            int item_to_remove = -1;
            CSequence* l_seq = NULL;
            POSITION pos = NULL;
            CString l_msg = _T("");
            int	l_protected_count = 0;
            bool l_protected_status = false;

            for (int index = 0 ; index < count ; index ++) {

                item_to_remove = selected_items[index] - index + l_protected_count;

                //+Patni-HAR/2010Jan29/Added/DeFT#MVC006364
                if (p_parent->GetScanIndex() == item_to_remove) {
                    break;
                }

                //-Patni-HAR/2010Jan29/Added/DeFT#MVC006364

                //+Patni-Hemant-MP/2011Mar25/Added/TMSC-REDMINE-1486-Part1
                if (GetWSEEditingProtocolIndex() == item_to_remove) {

                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("WSE Edited Protocol Found while Deletion"));
                    break;
                }

                //-Patni-Hemant-MP/2011Mar25/Added/TMSC-REDMINE-1486-Part1

                l_seq = (CSequence*) GetItemData(item_to_remove);
                //Patni-PJS/2009Dec24/Added/MVC006364
                //Patni-PJS/2010Mar02/Commented/MSA00251-00241
                //m_current_prot_in_delete_process = item_to_remove;

                CString l_seqID;

                l_seq->GetCPASCOMSequence()->GetSequenceID(l_seqID);

                BSTR  l_bstr_seqID = l_seqID.AllocSysString();

                if	(TRUE == p_parent->GetProtectedStatus(l_bstr_seqID)) {
                    l_protected_status = true;

                    if (0 == l_protected_count) {
                        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(CPQMUtility::GetMultiLingualString(_T("IDS_DELETE_ERROR")),
                                MPlus::EH::Information, _T("PQM"), _T("PQM"));
                    }

                }

                ::SysFreeString(l_bstr_seqID);

                if (!l_protected_status) {
                    if (!p_parent->DeleteProtocolWithIndex(l_seq, item_to_remove)) {

                        ASSERT(FALSE);
                        break ;
                    }
                }

                if (true == l_protected_status) {
                    l_protected_count++;
                    l_protected_status = false ;
                    continue;
                }

                //first delete the item form the listctrl...
                DeleteItem(item_to_remove);

                //+Patni-DKH/2010Nov19/Added/TMSC-REDMINE-923
                l_msg = _T("");
                l_msg.Format(_T("Deleted protocol with index : %d"), selected_items[index]);
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          l_msg);
                //-Patni-DKH/2010Nov19/Added/TMSC-REDMINE-923



                pos = m_list->FindIndex(item_to_remove);
                m_list->RemoveAt(pos);

                //+Patni-DKH/2010Nov19/Added/TMSC-REDMINE-923
                l_msg = _T("");
                l_msg.Format(_T("Removed protocol with index from list: %d from sequence list"), selected_items[index]);
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          l_msg);
                //-Patni-DKH/2010Nov19/Added/TMSC-REDMINE-923

                //Patni-PJS/2009Dec24/Added/MVC006364
                //Patni-PJS/2010Mar02/Commented/MSA00251-00241
                //m_current_prot_in_delete_process = -1;

                DEL_PTR(l_seq);
            }

            if (g_MainView->GetAcquisition() != NULL) {
                g_MainView->GetAcquisition()->CalcSARControlAndPutVFForAll();
            }

            // + Patni- Sribanta/2010Jun10/Added/MVC7933
            p_parent->SendDeleteCompleteDEGL();
            // - Patni- Sribanta/2010Jun10/Added/MVC7933


            if (l_can_update_group_num) {
                p_parent->UpdateGroupNum();
            }

            p_parent->OnDeleteUpdateMasterSlaveOnGUI();


            //+Patni-HAR/2010Jan29/Added/DeFT#MVC006364
            //Patni-PJS/2010Mar02/Commented/MSA00251-00241
            //m_current_prot_in_delete_process = -1;
            //-Patni-HAR/2010Jan29/Added/DeFT#MVC006364

            //+ Patni-RJ/2009Mar29/Added/PSP- Performance code
#ifdef __SM_PERFORMANCE_CHECK__
            QueryPerformanceFrequency((LARGE_INTEGER*)&l_freq);
            QueryPerformanceCounter((LARGE_INTEGER*)&l_stop);
            CString l_temp = _T("");
            l_temp.Format(_T("{CPQMListCtrl::DeleteSelectedProtocols} Time take to delete %d protocols"), count);
            CPQMLogMgr::GetInstance()->TracePerformance(l_start, l_stop, l_freq, l_temp);
#endif
            //- Patni-RJ/2009Mar29/Added/PSP- Performance code

            Invalidate();
            UpdateWindow();

            //Modified by Hemant - we are required to select
            //int seelct_next_item = selected_items[count-1] ;
            int seelct_next_item = selected_items[0] ;

            if (seelct_next_item >= GetItemCount())
                seelct_next_item = GetItemCount() - 1 ;

            if (seelct_next_item < 0)
                seelct_next_item = 0;

            //Added by Hemant On 10/6/2008 11:53:07 AM
            if (seelct_next_item < GetItemCount()) {
                //
                //End Of addition By Hemant On10/6/2008 11:53:23 AM


                SelectOnlyItem(seelct_next_item);
                EnsureVisible(seelct_next_item, FALSE);
            }

            //+Patni-ARD/2010Apr29/Modified/JaFT# MCM0229-00178_CDR_06 + code review
            DEL_PTR_ARY(selected_items);
            //-Patni-ARD/2010Apr29/Modified/JaFT# MCM0229-00178_CDR_06 + code review

            //+ Patni-RJ/2009Mar29/Added/PSP- Performance code
#ifdef __SM_PERFORMANCE_CHECK__
            __int64 l_freq2, l_stop2, l_start2;

            QueryPerformanceCounter((LARGE_INTEGER*)&l_start2);
#endif
            //- Patni-RJ/2009Mar29/Added/PSP- Performance code

            p_parent->RemoevAutoScanFromFirstWaitItem();
            //+ Patni-RJ/2009Mar29/Added/PSP- Performance code
#ifdef __SM_PERFORMANCE_CHECK__
            QueryPerformanceFrequency((LARGE_INTEGER*)&l_freq2);
            QueryPerformanceCounter((LARGE_INTEGER*)&l_stop2);

            CPQMLogMgr::GetInstance()->TracePerformance(l_start2, l_stop2, l_freq2, _T("RemoevAutoScanFromFirstWaitItem in DeleteSelectedProtocols"));
#endif
            //- Patni-RJ/2009Mar29/Added/PSP- Performance code
        }

        //+ Patni-RJ/2009Mar29/Added/PSP- Performance code
#ifdef __SM_PERFORMANCE_CHECK__
        __int64 l_freq, l_stop, l_start;

        QueryPerformanceCounter((LARGE_INTEGER*)&l_start);
#endif

        //- Patni-RJ/2009Mar29/Added/PSP- Performance code

        //update the total remaining time..
        p_parent->DisplayTotalRemainingTimeOnPQM();

        //+ Patni-RJ/2009Mar29/Added/PSP- Performance code
#ifdef __SM_PERFORMANCE_CHECK__
        QueryPerformanceFrequency((LARGE_INTEGER*)&l_freq);
        QueryPerformanceCounter((LARGE_INTEGER*)&l_stop);
        CPQMLogMgr::GetInstance()->TracePerformance(l_start, l_stop, l_freq, _T("DisplayTotalRemainingTimeOnPQM in DeleteSelectedProtocols"));

#endif
        //- Patni-RJ/2009Mar29/Added/PSP- Performance code

        //Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Actual Protocol Deletion Ends Normally"));

        return true;
    }

    //+Patni-DKH/2010Nov19/Added/TMSC-REDMINE-923
    catch (CException* e) {

        //Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Actual Protocol Deletion Ends with exception E..."));

        TCHAR l_excp[1024] = {0};
        e->GetErrorMessage(l_excp, 1024);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  l_excp);
        throw;

    } catch (...) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Unhandled Exception occured."));
        throw;
    }

    //-Patni-DKH/2010Nov19/Added/TMSC-REDMINE-923

    //Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("Actual Protocol Deletion Ends abonormal"));

}

//************************************Method Header************************************
// Method Name  : InsertProtocolAt
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
int CPQMListCtrl::InsertProtocolAt(
    CSequence* sequence_to_insert,
    int index_at_which_to_insert /*= -1*/,
    const bool select_item /*= true*/
)
{
    LPCTSTR FUNC_NAME = _T("CPQMListCtrl::InsertProtocolAt");

    PQM_TRACE_SCOPED(FUNC_NAME);

    try {

        if (NULL == sequence_to_insert) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("sequence_to_insert is NULL"));
            return -1;
        }

        if ((index_at_which_to_insert < 0) || (index_at_which_to_insert >= GetItemCount()))
            index_at_which_to_insert = GetItemCount();

        CAppDictionaryDetails       l_dic_detail;
        DicParamConvertedValueList  l_converted_value_list;
        DicParamNameList            l_param_name_list;

        CString l_temp = _T("");

        SEQUENCE_PARAM l_param[100] = {0};	//Patni-PP/2009Jun01/Modified/SU14 CPP test initialization corrections
        Data_decl_list l_sequence_param_name_list;
        sequence_to_insert->GetSequenceParamStruct(&l_param[0], l_sequence_param_name_list);

        ApplyParameterDictionary(l_param[0], l_converted_value_list, l_param_name_list, l_sequence_param_name_list);
        POSITION l_sub_pos = m_app_dic_converted_value_list.GetHeadPosition();

        while (1) {

            l_dic_detail = m_app_dic_converted_value_list.GetAt(l_sub_pos);

            if (l_dic_detail.SeqListCtrlDisplay() == true) {
                l_temp = m_app_dic_converted_value_list.GetAt(l_sub_pos).GetConvertedValue();
                m_app_dic_converted_value_list.GetNext(l_sub_pos);

                break;
            }

            m_app_dic_converted_value_list.GetNext(l_sub_pos);
        }

        const int HoldValue = sequence_to_insert->GetCPASCOMSequence()->GetSequenceParam()->GetInt(_T("HOLD"));

        //HoldValue |= (VFC_HOLD_PRE_SCAN);
        CString str_label = l_temp;

        CString str_mode1 = _T(""), str_mode2 = _T(""), str_mode3 = _T("");

        CPQMView* p_parent = NULL;

        //p_parent = (CPQMView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();
        p_parent = (CPQMView*)GetParent();

        ASSERT(p_parent != NULL);

        g_MainView->GetSarInfoPtrForIndex(sequence_to_insert, index_at_which_to_insert);

        const int acq_status = sequence_to_insert->GetAcqStatus();

        if ((!(HoldValue & VFC_HOLD_PRE_SCAN)) && (acq_status == PR_ACQ_STATUS_WAIT)) {
            str_mode1.LoadString(IDS_PQM_LABEL_STATUS_AUTO);
        }

        if ((HoldValue & VFC_HOLD_COVERAGE) && (acq_status == PR_ACQ_STATUS_WAIT)) {
            str_mode2.LoadString(IDS_PQM_LABEL_STATUS_BREATH);
        }

        if ((HoldValue & VFC_HOLD_COUCH) && (acq_status == PR_ACQ_STATUS_WAIT)) {
            str_mode3.LoadString(IDS_PQM_LABEL_STATUS_COUCH);

        }

        l_temp.Format(_T("%s"), str_label);

        LVITEM l_lvitem ;
        l_lvitem.mask         = LVIF_PARAM | LVIF_TEXT;
        l_lvitem.state        = 0;
        l_lvitem.stateMask    = 0;

        l_lvitem.iItem        = index_at_which_to_insert;
        l_lvitem.iSubItem     = 0;
        l_lvitem.lParam       = (LPARAM)sequence_to_insert;

        l_lvitem.pszText      = l_temp.GetBuffer(l_temp.GetLength());

        InsertItem(&l_lvitem);

        int l_subitem = 0;
        l_sub_pos = m_app_dic_converted_value_list.GetHeadPosition();

        CString parameter_string = _T("");

        while (l_sub_pos) {
            l_dic_detail = m_app_dic_converted_value_list.GetNext(l_sub_pos);

            //+Patni-HEMANT/ADDED On 4/29/2009 2:37:40 PM/ ACE /Internal Error
            //+Patni-NP/2010Apr12/Modified/MVC005120
            //if (!(l_strblock.CompareNoCase(_T("1000"))))//5120
            {
                //+Patni-NP/2010Apr12/Modified/MVC005120
                //-Patni-HEMANT/ADDED On 4/29/2009 2:37:40 PM/ ACE /Internal Error

                l_temp =  l_dic_detail.GetConvertedValue();

                if (l_dic_detail.SeqListCtrlDisplay() == true) {

                    parameter_string = l_dic_detail.GetParameterValue() ;

                    if (!CanSetTextItemForPameterString(parameter_string)) {

                        if (parameter_string.Compare(_T("SCANTIME")) == 0) {

                            sequence_to_insert->SetScanTimeString(l_temp);
                        }

                        if (parameter_string.Compare(_T("DELAYTIME")) == 0) {

                            if ((l_temp.Compare(ZERO_TIME) == 0 || l_temp.Compare(INVALID_STRING) == 0)) {
                                l_temp = _T("") ;
                            }

                            sequence_to_insert->SetDelayTimeString(l_temp);
                        }


                        l_temp = _T("") ;
                    }

                    //+Patni-Hemant/2009Dec21/Added/MVC006231
                    if (parameter_string.Compare(_T("COMM")) == 0) {

                        l_temp = IncludeSpaceUsingAlignment(l_temp, GetApplicationDictionaryAlignment(l_dic_detail.GetAlignment()));
                        //-Patni-Hemant/2009Dec21/Added/MVC006231

                        //+Patni-Sudhir/2011Jan27/Added/Redmine-434

                    } else if (parameter_string.Compare(_T("NRFLG")) == 0) {
                        const int l_acqOrder =  _wtoi(sequence_to_insert->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER")));
                        const int l_NRFLG = _wtoi(l_temp);
                        l_temp = GetNRFilterStringForProtocol(l_acqOrder, l_NRFLG);
                    }

                    //-Patni-Sudhir/2011Jan27/Added/Redmine-434

                    else if (parameter_string.Compare(_T("EMTFLG")) == 0) {
                        const int l_acqOrder =  _wtoi(sequence_to_insert->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER")));
                        l_temp = GetEMTFilterStringForProtocol(l_acqOrder);
                    }

                    SetItemText(index_at_which_to_insert, l_subitem, l_temp);
                    l_subitem++;
                }
            }
        }

        l_converted_value_list.RemoveAll();
        l_param_name_list.RemoveAll();

        if (select_item) {
            SelectOnlyItem(index_at_which_to_insert);
            EnsureVisible(index_at_which_to_insert, FALSE);
        }

    } catch (...) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception Occured..."));
        throw;
    }

    return index_at_which_to_insert;
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::DuplicateSelectedItems(

    //Patni-KSS/2010Jul16/ADDED/IR-156_2
    const bool f_append /*= false*/
)
{
    LPCTSTR FUNC_NAME = _T("CPQMListCtrl::DuplicateSelectedItems");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, f_append ? _T("f_append = True") : _T("f_append = False"));

    int* selected_items = NULL ;
    const int count = GetselectedItems(&selected_items);

    if (!count || !selected_items) {

        //+Patni-ARD/2010Apr29/Modified/JaFT# MCM0229-00178_CDR_06 + code review
        DEL_PTR_ARY(selected_items);
        //-Patni-ARD/2010Apr29/Modified/JaFT# MCM0229-00178_CDR_06 + code review

        return;
    }


    CPQMView* p_parent = (CPQMView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

    if (NULL == p_parent) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("p_parent pointer is NULL"));
        return;
    }

    bool l_can_update_group_num = false;
    int* l_new_groupids = g_MainView->AllocateGroupIdForDuplicateProtocols(selected_items, count);

    if (NULL == l_new_groupids) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_new_groupids is NULL"));
    }


    //+Patni-RSG/2009June5/Modified/JFT-534
    int new_item_index = selected_items[0];
    //-Patni-RSG/2009June5/Modified/JFT-534

    int offset_for_duplication = -1;
    CSequence* seq_to_duplicate = NULL ;
    int last_index = -1;

    int l_curr_group_id = 0;

    int l_offset_to_add = 0;

    for (int index = 0 ; index < count ; index ++) {
        //for(int index = (count-1) ;index >= 0 ; index --) {

        offset_for_duplication = selected_items[index] ;

        seq_to_duplicate = (CSequence*) GetItemData(offset_for_duplication);

        if (seq_to_duplicate != NULL) { //+Patni-SR/2009Sept30/Modified/IDS-1144


            //+Patni-KSS/2010June25/MODIFIED/IR-156_2
            //+Patni-RSG/2009June5/Modified/JFT-534
            //int new_item_index = DuplicateSequenceforIndex( selected_items[index] );


            if (l_new_groupids) {
                l_curr_group_id = l_new_groupids[index];
            }

            if (f_append) {
                new_item_index = DuplicateSequenceforIndex(offset_for_duplication, GetItemCount(), -1, l_curr_group_id);

            } else {
                new_item_index = DuplicateSequenceforIndex(offset_for_duplication, -1,
                                 (seq_to_duplicate->GetAcqStatus() != PR_ACQ_STATUS_WAIT) ? l_offset_to_add : -1, l_curr_group_id);
            }


            //-Patni-RSG/2009June5/Modified/JFT-534
            //-Patni-KSS/2010June25/MODIFIED/IR-156_2


            if (new_item_index < 0) {
                //                ASSERT(FALSE);
                //+ patni-Sribanta/2010August09/Modified/TMSC-REDMINE-556
                //Patni-PJS/2011Jan29/Modified/IR-97
                l_can_update_group_num = true;
                continue;		//we will continue doing duplication for other items..
                //+ patni-Sribanta/2010August09/Modified/TMSC-REDMINE-556
            }

            l_offset_to_add++;
            //Added by Hemant On 11/13/2008 4:44:37 PM
            //remove the selection from
            EnableHighlighting(selected_items[index] , false);

            //select the copied index..
            EnableHighlighting(new_item_index, true);

            Invalidate(FALSE);
            UpdateWindow();
            //End Of addition By Hemant On11/13/2008 4:45:11 PM

            //change the selected indexex according to new item..
            for (last_index = (count - 1) ; last_index >= index ; last_index --) {

                if (selected_items[last_index] >= new_item_index) {
                    selected_items[last_index] ++ ;

                } else {

                    //we don't have any index for adjustment..now..
                    break ;
                }
            }
        } //-Patni-SR/2009Sept30/Modified/IDS-1144
    }


    if (l_can_update_group_num) {
        p_parent->UpdateGroupNum();
    }

    if (NULL != g_MainView->GetAcquisition()) {
        g_MainView->GetAcquisition()->CalcSARControlAndPutVFForAll();
    }

    DEL_PTR_ARY(l_new_groupids);


    Invalidate();
    UpdateWindow();

    //Patni-RSG/2009June5/Modified/JFT-534
    EnsureVisible(new_item_index, FALSE);

    //Patni-ARD/2010Apr29/Modified/JaFT# MCM0229-00178_CDR_06 + code review
    DEL_PTR_ARY(selected_items);

}



//************************************Method Header************************************
// Method Name  : IsValidSequenceIndex
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
BOOL CPQMListCtrl::IsValidSequenceIndex(
    const int index
)const
{
    const int total_list_items = GetItemCount();

    return (total_list_items ? ((index >= 0) && (index < total_list_items)) : FALSE);
}



//************************************Method Header************************************
// Method Name  : InvalidateSelectedItemsForModeOption
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::InvalidateItemsForModeOption(
    const int* const items_to_invalidate,
    const int item_count,
    const int mode_option_index /*= -1*/
)
{

    LPCTSTR FUNC_NAME = _T("CPQMListCtrl::InvalidateItemsForModeOption");

    //+Patni-DKH/2009Aug26/Modified/cpp test corrections
    if (NULL == items_to_invalidate) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("items_to_invalidate pointer is NULL"));
        return;
    }

    //-Patni-DKH/2009Aug26/Modified/cpp test corrections

    if ((m_mode_column_index != -1) &&  item_count) {

        for (int index = 0 ; index < item_count; index ++) {
            InvalidateItemsForModeOption(items_to_invalidate[index], mode_option_index);
        }
    }
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::InvalidateItemsForModeOption(
    const int items_to_invalidate,
    const int mode_option_index /*= -1*/
)
{

    if (m_mode_column_index != -1) {

        if (mode_option_index == -1) {
            InvalidateItemForColumn(items_to_invalidate, m_mode_column_index);

        } else {
            InvalidateRect(GetCellRect(items_to_invalidate, m_mode_column_index));
        }
    }
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::InvalidateItemsForSarRegion(
    const int* const items_to_invalidate,
    const int item_count
)
{
    LPCTSTR FUNC_NAME = _T("CPQMListCtrl::InvalidateItemsForSarRegion");

    //+Patni-DKH/2009Aug26/Modified/cpp test corrections
    if (NULL == items_to_invalidate) {
        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("items_to_invalidate pointer is NULL"));
        return;
    }

    //+Patni-DKH/2009Aug26/Modified/cpp test corrections

    if ((m_sar_column_index != -1)  && item_count) {

        for (int index = 0 ; index < item_count; index ++) {
            InvalidateItemsForSarRegion(items_to_invalidate[index]);
        }
    }
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::InvalidateItemsForSarRegion(
    const int items_to_invalidate
)
{
    if (m_sar_column_index != -1)
        InvalidateItemForColumn(items_to_invalidate, m_sar_column_index);
}



//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::InvalidateItemsForScanTime(
    const int* const items_to_invalidate,
    const int item_count
)
{

    LPCTSTR FUNC_NAME = _T("CPQMListCtrl::InvalidateItemsForScanTime");

    //+Patni-DKH/2009Aug26/Modified/cpp test corrections
    if (NULL == items_to_invalidate) {
        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("items_to_invalidate pointer is NULL"));
        return;
    }

    //+Patni-DKH/2009Aug26/Modified/cpp test corrections
    if ((m_time_column_index != -1)  && item_count) {

        for (int index = 0 ; index < item_count; index ++) {

            InvalidateItemsForScanTime(items_to_invalidate[index]);
        }
    }
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::InvalidateItemsForScanTime(
    const int items_to_invalidate
)
{
    //+Patni-Hemant/2009Nov15/Modified/IR#90/Queue List
    //
    //	if(m_time_column_index !=-1){
    //
    //		InvalidateItemForColumn(items_to_invalidate, m_time_column_index);
    //	}

    InvalidateItemsForStatus(items_to_invalidate);
    //-Patni-Hemant/2009Nov15/Modified/IR#90/Queue List
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::InvalidateItemsForStatus(
    const int* const items_to_invalidate,
    const int item_count
)
{

    LPCTSTR FUNC_NAME = _T("CPQMListCtrl::InvalidateItemsForStatus");

    //+Patni-DKH/2009Aug26/Modified/cpp test corrections
    if (NULL == items_to_invalidate) {
        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("items_to_invalidate pointer is NULL"));
        return;
    }

    //+Patni-DKH/2009Aug26/Modified/cpp test corrections

    if ((m_status_column_index != -1) && item_count) {

        for (int index = 0 ; index < item_count; index ++) {

            InvalidateItemsForStatus(items_to_invalidate[index]);
        }
    }
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::InvalidateItemsForStatus(
    const int items_to_invalidate
)
{

    if (m_status_column_index != -1) {

        InvalidateItemForColumn(items_to_invalidate, m_status_column_index);
    }
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::InvalidateItemsForWSEEditingColumn(
    const int items_to_invalidate
)
{
    //+Patni-Hemant/v/Modified/ IR#90/Queue List
    //if(m_wseediting_column_index!=-1){
    //	InvalidateItemForColumn(items_to_invalidate, m_wseediting_column_index);
    //}

    InvalidateItemsForPLANColumn(items_to_invalidate);
    //-Patni-Hemant/2009Nov15/Modified/ IR#90/Queue List
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::InvalidateItemsForPLANColumn(
    const int items_to_invalidate
)
{
    if (m_plan_column_index != -1) {

        InvalidateItemForColumn(items_to_invalidate, m_plan_column_index);
    }
}
//************************************Method Header************************************
// Method Name  : InvalidateItemsForDelayTime
// Author       : PATNI/AKR
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::InvalidateItemsForDelayTime(
    const int* const f_items_to_invalidate,
    const int f_item_count
)
{

    LPCTSTR FUNC_NAME = _T("CPQMListCtrl::InvalidateItemsForDelayTime");

    if (NULL == f_items_to_invalidate) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_items_to_invalidate pointer is NULL"));
        return;
    }

    if (-1 != m_delay_column_index  && f_item_count) {
        for (int l_index = 0 ; l_index < f_item_count; l_index ++) {
            InvalidateItemForColumn(f_items_to_invalidate[l_index], m_delay_column_index);
        }
    }
}
//************************************Method Header**************************************
//Method Name:  Init
//Author:  PATNI/RD
//Purpose: Initialize list control and scroll bar
//**************************************************************************************/
void CPQMListCtrl::Init(
)
{
    //Patni/2010Dec06/Commented/V2.0/IR-173-Part2



    SetFont(_T("Arial"), 14);
    SetTextColor(RGB(225, 234, 254));
}


//************************************Method Header**************************************
//Method Name:  IsRowSelected
//Author:  PATNI/RD
//Purpose: Check row is selected
//**************************************************************************************/
bool CPQMListCtrl::IsRowSelected(
    HWND hWnd,
    const int row
)const
{
    return ListView_GetItemState(hWnd, row, LVIS_SELECTED) != 0;
}


void CPQMListCtrl::Clear()
{
    m_vSeqInfo.clear();
}


//************************************Method Header**************************************
//Method Name:  UpdateSequences
//Author:  PATNI/MSN
//Purpose: Updates the selected sequence list on list control.
void CPQMListCtrl::UpdateSequences(
    const int* const sellist,
    const int count
)
{
    if ((NULL == (CPQMView*)GetParent()) || (count <= 0)) {
        return ;
    }


    CString  l_temp = _T("");	//Patni-PP/2009Jun01/Modified/SU14 CPP test initialization corrections

    DicParamConvertedValueList  l_converted_value_list;
    DicParamNameList            l_param_name_list;

    CSequence* l_seq = NULL;
    CAppDictionaryDetails  l_dic_detail;

    POSITION l_sub_pos = NULL;	//Patni-PP/2009Jun01/Modified/SU14 CPP test initialization corrections
    SEQUENCE_PARAM l_param[100] = {0}; //Patni-PP/2009Jun01/Modified/SU14 CPP test initialization corrections

    LVITEM  l_lvitem = {0};	//Patni-PP/2009Jun01/Modified/SU14 CPP test initialization corrections
    l_lvitem.mask         = LVIF_PARAM | LVIF_TEXT;
    l_lvitem.state        = 0;
    l_lvitem.stateMask    = 0;

    int l_subitem = -1;
    int HoldValue = -1;
    int acq_status = -1;
    CSequence* sequence_struct = NULL;

    CString str_label = _T("");
    CString str_mode1 = _T(""), str_mode2 = _T(""), str_mode3 = _T("");

    for (int index = 0; index < count; index++) {

        l_subitem = 1;

        l_seq = (CSequence*) GetItemData(sellist[index]);

        Data_decl_list l_sequence_param_name_list;
        l_seq->GetSequenceParamStruct(&l_param[0], l_sequence_param_name_list);

        ApplyParameterDictionary(l_param[0], l_converted_value_list, l_param_name_list, l_sequence_param_name_list);
        l_sub_pos = m_app_dic_converted_value_list.GetHeadPosition();

        while (1) {
            l_dic_detail = m_app_dic_converted_value_list.GetAt(l_sub_pos);

            if (l_dic_detail.SeqListCtrlDisplay() == true) {
                l_temp = m_app_dic_converted_value_list.GetAt(l_sub_pos).GetConvertedValue();
                m_app_dic_converted_value_list.GetNext(l_sub_pos);
                break;
            }

            m_app_dic_converted_value_list.GetNext(l_sub_pos);
        }

        l_lvitem.iItem        = sellist[index];
        l_lvitem.iSubItem     = 0;
        l_lvitem.lParam       = (LPARAM)l_seq;

        HoldValue = l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetInt(_T("HOLD"));
        str_label = l_temp;

        sequence_struct = GetSequenceStructForIndex(sellist[index]);
        acq_status = sequence_struct->GetAcqStatus();
        //

        if ((!(HoldValue & VFC_HOLD_PRE_SCAN)) && (acq_status == PR_ACQ_STATUS_WAIT)) {
            str_mode1.LoadString(IDS_PQM_LABEL_STATUS_AUTO);
        }

        if ((HoldValue & VFC_HOLD_COVERAGE) && (acq_status == PR_ACQ_STATUS_WAIT)) {
            str_mode2.LoadString(IDS_PQM_LABEL_STATUS_BREATH);
        }

        if ((HoldValue & VFC_HOLD_COUCH) && (acq_status == PR_ACQ_STATUS_WAIT)) {
            str_mode3.LoadString(IDS_PQM_LABEL_STATUS_COUCH);
        }

        l_temp.Empty();
        //CString str_tmp = str_mode1 + str_mode2 + str_mode3;

        //l_temp.Format(_T("%s"), str_label);
        l_temp = str_label ;

        CString parameter_string = l_dic_detail.GetParameterValue();

        if (!CanSetTextItemForPameterString(parameter_string)) {

            if (parameter_string.Compare(_T("SCANTIME")) == 0) {

                l_seq->SetScanTimeString(l_temp);
            }

            if (parameter_string.Compare(_T("DELAYTIME")) == 0) {

                if ((l_temp.Compare(ZERO_TIME) == 0 || l_temp.Compare(INVALID_STRING) == 0)) {

                    l_temp = _T("") ;
                }

                l_seq->SetDelayTimeString(l_temp);
            }


            l_temp = _T("") ;
        }

        l_lvitem.pszText      = l_temp.GetBuffer(l_temp.GetLength());

        SetItem(&l_lvitem);

        while (l_sub_pos) {
            l_dic_detail = m_app_dic_converted_value_list.GetNext(l_sub_pos);

            //+Patni-HEMANT/ADDED On 4/29/2009 2:37:40 PM/ ACE /Internal Error
            //l_strblock	= l_dic_detail.GetBlock();
            //+Patni-NP/2010Apr12/Modified/MVC005120
            //                    if (!(l_strblock.CompareNoCase(_T("1000"))))
            {
                //-Patni-NP/2010Apr12/Modified/MVC005120
                //-Patni-HEMANT/ADDED On 4/29/2009 2:37:40 PM/ ACE /Internal Error

                l_temp =  l_dic_detail.GetConvertedValue();

                if (l_dic_detail.SeqListCtrlDisplay() == true) {

                    //-------------------------------------------------------------------------
                    //Added by Hemant On 7/1/2008 12:27:21 PM
                    parameter_string = l_dic_detail.GetParameterValue() ;

                    if (!CanSetTextItemForPameterString(parameter_string)) {

                        if (parameter_string.Compare(_T("SCANTIME")) == 0) {

                            l_seq->SetScanTimeString(l_temp);
                        }

                        if (parameter_string.Compare(_T("DELAYTIME")) == 0) {

                            if ((l_temp.Compare(ZERO_TIME) == 0 || l_temp.Compare(INVALID_STRING) == 0)) {

                                l_temp = _T("") ;
                            }

                            l_seq->SetDelayTimeString(l_temp);
                        }


                        l_temp = _T("") ;
                    }

                    //-------------------------------------------------------------------------

                    //+Patni-Hemant/2009Dec21/Added/MVC006231
                    if (parameter_string.Compare(_T("COMM")) == 0) {

                        l_temp = IncludeSpaceUsingAlignment(l_temp, GetApplicationDictionaryAlignment(l_dic_detail.GetAlignment()));
                        //-Patni-Hemant/2009Dec21/Added/MVC006231

                        //+Patni-Sudhir/2011Jan27/Added/Redmine-434

                    } else if (parameter_string.Compare(_T("NRFLG")) == 0) {

                        const int l_acqOrder = _wtoi(l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER")));
                        const int l_NRFLG = _wtoi(l_temp);
                        l_temp = GetNRFilterStringForProtocol(l_acqOrder, l_NRFLG);

                    }

                    //-Patni-Sudhir/2011Jan27/Added/Redmine-434

                    else if (parameter_string.Compare(_T("EMTFLG")) == 0) {
                        const int l_acqOrder = _wtoi(l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER")));
                        l_temp = GetEMTFilterStringForProtocol(l_acqOrder);
                    }

                    SetItemText(sellist[index], l_subitem, l_temp);
                    l_subitem++;
                }
            }
        }

        l_converted_value_list.RemoveAll();
        l_param_name_list.RemoveAll();
    }
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::SelectOnlyItem(
    const int index
)
{
    if (IsValidSequenceIndex(index)) {

        ModifyStyle(0, LVS_SINGLESEL);
        SetSelectionMark(index);
        SetItemState(index, (LVIS_FOCUSED | LVIS_SELECTED), (LVIS_FOCUSED | LVIS_SELECTED));
        ModifyStyle(LVS_SINGLESEL, 0);

        //Added by Hemant On 11/19/2008 10:25:56 AM
        SetSelectionMark(index);
        SendMessage(WM_HIDE_CONTEXT, 0, 0);
    }
}


//+Patni/2010Dec06/Added/V2.0/IR-173-Part2
//************************************Method Header**************************************
//Method Name:  Dictinstance
//Author:  PATNI
//Purpose:
//**************************************************************************************/
void CPQMListCtrl::Dictinstance()
{
    CString l_str(_T("")) ;

    switch (g_MainView->GetPQMGlobal().GetPQMMode()) {

        case K_SCHEDULING :
            l_str = _T("SchedulePQM");
            break ;

        case K_IMAGING :
            l_str = _T("AcquirePQM");
            break ;

        default :
            l_str = _T("AcquirePQM");
    }

    m_dic_manager = CDictionaryManager::GetInstance(PQM_DISP_DICT , l_str);
    ApplyDisplayDictionary();

}
//-Patni/2010Dec06/Added/V2.0/IR-173-Part2




//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::MakeCurrentWSEEditingProtocolToPlanned(
)
{
    const int prev_wse_editingindex = GetWSEEditingProtocolIndex();

    if (prev_wse_editingindex != -1) {
        CSequence* l_seq = (CSequence*) GetItemData(prev_wse_editingindex);

        if (l_seq) {

            l_seq->SetWSEEditingEnum(WSE_Planned);

            m_last_planned_protocol = prev_wse_editingindex ;
        }
    }

    InvalidateItemsForPLANColumn(prev_wse_editingindex);
}

void CPQMListCtrl::ResetLastPlannedProtocol()
{

    m_last_planned_protocol = -1;
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::ResetWSEEditingProtocolIndex(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMListCtrl::ResetWSEEditingProtocolIndex");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    const int prev_wse_editingindex = GetWSEEditingProtocolIndex();

    if (prev_wse_editingindex != -1) {
        CSequence* l_seq = (CSequence*) GetItemData(prev_wse_editingindex);

        if (l_seq) {

            WSE_EditingEnum current_wseenum = l_seq->GetWSEEditingEnum() ;

            //added by Hemant on 23/12/08
            //before making it off, if its current and planned or planned, then store it as planned
            //
            if ((current_wseenum == WSE_Planned) || (WSE_Planned_AND_Current == current_wseenum))
                l_seq->SetWSEEditingEnum(WSE_Planned);

            else {
                //end addition.
                l_seq->SetWSEEditingEnum(WSE_None);
            }
        }
    }
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
int CPQMListCtrl::GetselectedItems(
    int** selected_items,
    bool* iswait_selected /*= NULL*/,
    bool* iscurrent_selected /*= NULL*/,
    bool* isdone_selected /*= NULL*/,
    bool* isfail_selected /*= NULL*/,
    bool* isfirstwait_selected /*= NULL*/,
    bool* iswse_eidted_protocol_selected /*= NULL*/,
    bool* iscoil_selected_for_all /*= NULL*/		//+Patni-HEMANT/ADDED On 8/12/2009 1:44:12 PM/ MVC#4232
)
{

    int index = 0;

    if (GetItemCount() <= 0) {
        *selected_items = NULL;
        return index;
    }

    UINT selected_count = GetSelectedCount();
    POSITION pos = GetFirstSelectedItemPosition();

    if ((selected_count > 0) && pos) {
        (*selected_items) = new int [selected_count] ;

    } else {
        *selected_items = NULL;
        return index;
    }

    bool already_checked_for_wait = false ;

    //+Patni-HEMANT/ADDED On 8/12/2009 1:44:12 PM/ MVC#4232
    if (iscoil_selected_for_all) {
        (*iscoil_selected_for_all) = true;
    }

    //-Patni-HEMANT/ADDED On 8/12/2009 1:44:12 PM/ MVC#4232


    int currently_selected_seq = -1;
    CSequence* seq = NULL ;

    while (pos) {

        currently_selected_seq = GetNextSelectedItem(pos);
        seq = (CSequence*) GetItemData(currently_selected_seq);

        //+Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1
        if (NULL == seq) {
            continue ;
        }

        //-Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1

        if (iswse_eidted_protocol_selected && !(*iswse_eidted_protocol_selected)) {

            if (GetWSEEditingProtocolIndex() == currently_selected_seq)
                (*iswse_eidted_protocol_selected) = true ;
        }

        const int acq_status = seq->GetAcqStatus();

        switch (acq_status) {

            case PR_ACQ_STATUS_SUCCESS :

                if (isdone_selected && !(*isdone_selected))
                    *isdone_selected = true ;

                break;

            case PR_ACQ_STATUS_FAIL :

                if (isfail_selected && !(*isfail_selected))
                    *isfail_selected = true ;

                break;

            case PR_ACQ_STATUS_CURRENT :

                if (iscurrent_selected && !(*iscurrent_selected))
                    *iscurrent_selected = true ;

                break;

            case PR_ACQ_STATUS_WAIT :

                if (g_MainView->GetScanIndex() == currently_selected_seq) {
                    if (iscurrent_selected && !(*iscurrent_selected))
                        *iscurrent_selected = true ;

                } else if (!already_checked_for_wait) {

                    already_checked_for_wait = true ;

                    if (iswait_selected && !(*iswait_selected)) {

                        *iswait_selected = true ;
                    }

                    //now check for first wait also..
                    //If this selected index = 0 , then its the first wait..
                    //else
                    //if this selected index is >1 and if the prev index is wait then this index is not first wait..
                    if (isfirstwait_selected && !(*isfirstwait_selected)) {

                        if (currently_selected_seq == 0) {

                            (*isfirstwait_selected) = true ;

                        } else {

                            CSequence* prev_seq = (CSequence*) GetItemData(currently_selected_seq - 1);

                            //Patni-Hemant-MP/2011Mar08/Modified/TMSC-REDMINE-1486-Part1
                            if (prev_seq && (prev_seq->GetAcqStatus() != PR_ACQ_STATUS_WAIT)  && (prev_seq->GetAcqStatus() != PR_ACQ_STATUS_CURRENT))

                                (*isfirstwait_selected) = true ;		//current item is first wait..

                            else
                                (*isfirstwait_selected) = false ;		//current item is not first wait..
                        }
                    }
                }

                break ;
        }

        //+Patni-HEMANT/ADDED On 8/12/2009 1:44:12 PM/ MVC#4232
        if (iscoil_selected_for_all && (*iscoil_selected_for_all)) {

            CPqmPrMode pr_mode = seq->GetPrMode();
            (*iscoil_selected_for_all) = ((*iscoil_selected_for_all) && (!pr_mode.m_recv_coil_label.IsEmpty()));
        }

        //-Patni-HEMANT/ADDED On 8/12/2009 1:44:12 PM/ MVC#4232
        //+ Patni-PJS/2009Dec24/Added+Modified/MVC006364
        //-Patni-PJS/2010Mar02/Commented/MSA00251-00241
        //if (currently_selected_seq != m_current_prot_in_delete_process) {
        (*selected_items)[index] = currently_selected_seq;
        //+Patni-PJS/2010Mar02/Commented/MSA00251-00241
        /*} else {
            int l_new_index = currently_selected_seq + 1;

            if (!IsValidSequenceIndex(l_new_index)) {
                (*selected_items) = NULL;
                PQM_TRACE(USER_FUNC_MARKER, _T("CPQMListCtrl::GetSelectedItems"),
                          _T("Invalid selected item"));
            }

            (*selected_items)[index] = l_new_index;
            CString l_cs_selectedItem = _T("");
            l_cs_selectedItem.Format(_T("%d"), (currently_selected_seq + 1));
            PQM_TRACE(USER_FUNC_MARKER, _T("CPQMListCtrl::GetSelectedItems"),
                      l_cs_selectedItem);
        }*/
        //-Patni-PJS/2010Mar02/Commented/MSA00251-00241
        //- Patni-PJS/2009Dec24/Added+Modified/MVC006364

        index ++;
    }

    //Patni-Hemant-MP/2011Mar08/Modified/TMSC-REDMINE-1486-Part1
    return /*selected_count*/ index;
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
int CPQMListCtrl::GetFirstWaitProtocolIndex(
    const bool f_consider_scan_index /*= false*/
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMListCtrl::GetFirstWaitProtocolIndex");

    const int l_total_count = GetItemCount();

    CSequence* pseq = NULL ;

    for (int index = 0; index < l_total_count; index++) {

        pseq = (CSequence*) GetItemData(index);

        if (pseq == NULL) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("pseq is NULL"));
            return -1;
        }

        //+Patni-Hemant/2009Sept01/Added/Move_Couch and duplicate Error\IDS#826
        //if (pseq->GetAcqStatus() == PR_ACQ_STATUS_WAIT)
        //	return index;
        if (pseq->GetAcqStatus() == PR_ACQ_STATUS_WAIT) {

            if (f_consider_scan_index) {
                extern CPQMView* g_MainView;

                if (g_MainView->GetScanIndex() == index) {

                    index++;
                }
            }

            //index is zero based so >= ...
            if (index >= GetItemCount()) {

                CString str;
                str.Format(_T("l_total_count = %d, New Total Count = %d"), GetItemCount());
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str);

                return -1;
            }

            return index;
        }

        //-Patni-Hemant/2009Sept01/Added/Move_Couch and duplicate Error\IDS#826
    }

    return -1 ;
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
int CPQMListCtrl::GetFirstWaitProtocolForAutoPlan(
)const
{

    LPCTSTR FUNC_NAME = _T("CPQMListCtrl::GetFirstWaitProtocolForAutoPlan");

    //+Patni-Dhanesh/2009Aug20/Modified/cpp test corrections
    if (NULL == m_list) {
        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("m_list pointer is NULL"));
        return -1 ;
    }

    //-Patni-Dhanesh/2009Aug20/Modified/cpp test corrections

    POSITION pos = m_list->GetHeadPosition();
    int index = 0;
    int holdvalue = -1;
    CSequence* pseq  = NULL ;

    while (pos) {

        pseq = m_list->GetNext(pos);

        //+Patni-Hemant/2009Dec1/Modified/MVC005852
        if (g_MainView->GetScanIndex() != index) {

            holdvalue = pseq->GetCPASCOMSequence()->GetSequenceParam()->GetInt(_T("HOLD"));

            if (pseq->GetAcqStatus() == PR_ACQ_STATUS_WAIT
                && ((holdvalue & VFC_HOLD_PRE_SCAN) == VFC_HOLD_PRE_SCAN)
                && (WSE_None == pseq->GetWSEEditingEnum())
               ) {
                return index;
            }
        } //-Patni-Hemant/2009Dec1/Modified/MVC005852

        index ++;
    }

    return -1 ;
}

//SM3 Defect fix 619  Start Ashish
void CPQMListCtrl::GetAppDictionaryConvertedValueList(const int selected_Index
                                                     )
{

    CSequence* l_seq = (CSequence*) GetItemData(selected_Index);
    //+Patni-NP/2009Sept15/Added/MVC#004766
    const int l_acq_order = l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetInt(_T("ACQORDER"));
    //Patni-NP/2009Sep21/Commented/MVC004865

    //+Patni-NP/2009Sept22/Added/MVC#004914
    //	CPQMView *p_parent = NULL;
    //	p_parent = (CPQMView*)GetParent();
    const int l_dbdtmode = g_MainView->GetDBDtMode(l_acq_order);
    //Nitin Coil
    const CString l_coil_name = g_MainView->GetCoilStringForTransmiter(l_acq_order);
    const CString l_coil_recv = g_MainView->GetCoilStringForReceiver(l_acq_order);
    //Nitin Coil
    g_MainView->GetSequenceInformation(selected_Index);
    //-Patni-NP/2009Sept22/Added/MVC#004914
    //+Patni-NP/2009Sep21/Commented/MVC004865
    //Nitin Coil
    //CString l_coil_name = l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("TRANSMIT"));
    //CString l_coil_recv = l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("RECEIVER"));
    //Nitin Coil
    //-Patni-NP/2009Sep21/Commented/MVC004865
    SEQUENCE_PARAM l_param[100] = {0};
    Data_decl_list l_sequence_param_name_list;
    l_seq->GetSequenceParamStruct(&l_param[0], l_sequence_param_name_list);

    //Patni-Hemant/2010Mar10/Modified/MSA0251-00277
    DicParamConvertedValueList  l_converted_value_list;
    DicParamNameList            l_param_name_list;
    ApplyParameterDictionary(l_param[0], l_converted_value_list, l_param_name_list, l_sequence_param_name_list, true);

    //Patni-Hemant/2010Mar10/Modified/MSA0251-00277
    //l_pos_app_dic = m_app_dic_converted_value_list.GetHeadPosition();
    POSITION l_pos_app_dic = m_app_dic_converted_value_list_seq_info.GetHeadPosition();


    CString l_laterality = _T("");
    int l_param_count  = 0;
    CAppDictionaryDetails l_dic_detail;
    CString l_temp(_T(""));
    SeveralCols* l_SeveralCols = NULL;

    while (l_pos_app_dic) {

        //Patni-Hemant/2010Mar10/Modified/MSA0251-00277
        //l_temp = m_app_dic_converted_value_list.GetAt(l_pos_app_dic).GetConvertedValue();
        l_temp = m_app_dic_converted_value_list_seq_info.GetAt(l_pos_app_dic).GetConvertedValue();

        //Patni-Hemant/2010Mar10/Modified/MSA0251-00277
        //l_dic_detail = m_app_dic_converted_value_list.GetNext(l_pos_app_dic);
        l_dic_detail = m_app_dic_converted_value_list_seq_info.GetNext(l_pos_app_dic);

        if (l_dic_detail.PQMListCtrlDisplay() == true) { //+Patni-NP/2010Apr12/Modified/MVC005120

            SeqInfo* l_SeqInfo = new SeqInfo();
            l_SeqInfo->strParameterLabel = l_dic_detail.GetLongLabel();

            //+Patni-MP/2010Jun30/Added/MVC008265
            if (l_SeqInfo->strParameterLabel.CompareNoCase(_T("Scan Region")) == 0) {
                l_temp = l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("BODYPART"));
                //+Patni-PJS/2010Jul13/Added/MVC008265_2
                l_laterality = l_seq->GetLateralityShortStr();

                if (!l_laterality.IsEmpty()) {
                    l_temp = l_temp + _T(":") + l_laterality;
                }

                //-Patni-PJS/2010Jul13/Added/MVC008265_2
                //-Patni-MP/2010Jun30/Added/MVC008265

            } else if (l_SeqInfo->strParameterLabel == _T("dB/dt")) {
                CString l_dbdt(_T(""));

                if (l_dbdtmode == 0) {
                    //l_SeqInfo->strParameterLabel+= _T("<80 %(---)");
                    //TMSC/2009Nov15/Modified/Code Changes
                    l_dbdt = _T("<=80 %(---)");
                    l_temp = l_dbdt + l_temp;

                } else if (l_dbdtmode == 1) {
                    //l_SeqInfo->strParameterLabel+= _T("<100 %(1st)");
                    l_dbdt = _T("<=100 %(1st)");
                    l_temp = l_dbdt + l_temp;

                } else  if (l_dbdtmode == 2) {
                    //l_SeqInfo->strParameterLabel+= _T("> %(2nd)");
                    l_dbdt = _T(">100 %(2nd)");
                    l_temp = l_dbdt + l_temp;
                }

                //-Patni-NP/2009Sept15/Added/MVC#004766
                //+Patni-NP/2009Sep21/Commented/MVC004865
                //+Patni-ARD/2009Sep28/Modified/Defect Fix MVC004865

            } else if (l_SeqInfo->strParameterLabel == _T("Transmitter Coil")) {
                l_temp = l_coil_name;

            } else if (l_SeqInfo->strParameterLabel == _T("Receiver Coil")) {
                l_temp = l_coil_recv;
                //-Patni-ARD/2009Sep28/Modified/Defect Fix MVC004865
                //-Patni-NP/2009Sep21/Commented/MVC004865

                //+Patni-Sudhir/2011Jan13/Added/Redmine-434

            } else if (l_SeqInfo->strParameterLabel.CompareNoCase(_T("Filter ID")) == 0) {

                const int l_NRFLG = _wtoi(l_temp);
                l_temp = GetNRFilterStringForProtocol(l_acq_order, l_NRFLG);
            }

            //-Patni-Sudhir/2011Jan13/Added/Redmine-434

            else if (l_SeqInfo->strParameterLabel.CompareNoCase(_T("Clarify Filter ID")) == 0) {
                l_temp = GetEMTFilterStringForProtocol(l_acq_order);
            }



            for (int j = 1; j < TOTAL_SEQ_INFO_COLUMNS; j++) {
                l_SeveralCols = new SeveralCols();
                l_SeveralCols->param_count = l_param_count;
                l_SeveralCols->column_number = j;
                l_temp.TrimLeft();
                l_temp.TrimRight();
                l_SeveralCols->strConvertedValue = l_temp;

                l_SeqInfo->v_SeveralColumns.push_back(l_SeveralCols);
            }

            m_vSeqInfo.push_back(l_SeqInfo);

            l_param_count++;
        }//+Patni-NP/2010Apr12/Modified/MVC005120
    }
}


//************************************Method Header************************************
// Method Name  : GetItemsForAutoVoiceAndMoveCouch
// Author       : PATNI/ HEMANT
// Purpose      : This function will return false when we can not apply autovoice or move couch
//                If this function returns true then f_items will be array of protocol index
//                for which we can apply move couch or auto voice
//                Because there are some different conditions for move couch and autovoice,
//                f_option should be passed properly in this function
//                Caller of this function should take care of deleting f_items
//***********************************************************************************
bool CPQMListCtrl::GetItemsForAutoVoiceAndMoveCouch(
    const int f_focus_item,
    int** f_items,
    int& f_count,
    const int f_option
)
{

    if (!IsValidSequenceIndex(f_focus_item)) {
        return false;
    }

    CSequence* l_focus_seq = (CSequence*)GetItemData(f_focus_item);

    if (l_focus_seq->GetAcqStatus() != PR_ACQ_STATUS_WAIT) {
        return false;
    }

    int* selected_items = NULL ;
    int count = 0;

    if (IsRowSelected(m_hWnd, f_focus_item)) {

        bool iswait_selected = false ;
        bool iscurrent_selected = false ;
        bool isdone_selected = false ;
        bool isfail_selected = false ;
        bool iswse_eidted_protocol_selected = false;
        bool iscoil_selected_for_all = false;

        count = GetselectedItems(&selected_items, &iswait_selected, &iscurrent_selected, &isdone_selected, &isfail_selected, NULL, &iswse_eidted_protocol_selected, &iscoil_selected_for_all);

        if (((count <= 0) || iscurrent_selected || isdone_selected || isfail_selected)
            || ((MOVE_COUCH_OPTION == f_option) && iswse_eidted_protocol_selected)
            || ((MOVE_COUCH_OPTION == f_option) && !iscoil_selected_for_all)
           ) {

            DEL_PTR_ARY(selected_items);
            return false;
        }

    } else {

        const bool iscoil_selected_for_all = (!l_focus_seq->GetPrMode().m_recv_coil_label.IsEmpty());

        if (MOVE_COUCH_OPTION == f_option) {
            if ((f_focus_item == GetWSEEditingProtocolIndex()) || !iscoil_selected_for_all) {
                return false;
            }
        }

        count = 1 ;
        selected_items = new int [count];
        selected_items[0] = f_focus_item;
    }

    f_count = count ;
    *f_items = selected_items;

    return true;
}



//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CString CPQMListCtrl::GetCellString(
    const CPoint& cell_index
)
{
    LPCTSTR FUNC_NAME = _T("CPQMListCtrl::GetCellString");

    CString str_toreturn = _T("") ;

    //+Patni-Dhanesh/2009Aug20/Modified/cpp test corrections
    if (NULL == m_list) {
        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("m_list pointer is NULL"));
        return str_toreturn;
    }

    //-Patni-Dhanesh/2009Aug20/Modified/cpp test corrections

    if (m_list && (cell_index.y >= 0) && (cell_index.y < m_list->GetCount())) {

        if ((unsigned int)cell_index.x == m_protocolno_column_index) {

            const int protocol_co_index_with_bias = GetBiasValueForProtocolNumber() * (cell_index.y + 1) ; // Defect#577

            str_toreturn.Format(_T("%d"), protocol_co_index_with_bias);
        }
    }

    return str_toreturn;
}

int CPQMListCtrl::GetLastPlannedProtocol(
)const
{

    return m_last_planned_protocol ;

}


//+Patni-Hemant/2010May24/Added/MaFT/TMSC-REDMINE-MVC007927
void CPQMListCtrl::SetLastPlannedProtocol(
    const int f_last_edited_protocol
)
{
    m_last_planned_protocol = f_last_edited_protocol;
}
//-Patni-Hemant/2010May24/Added/MaFT/TMSC-REDMINE-MVC007927

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
int CPQMListCtrl::GetWSEEditingProtocolIndex(
)const
{
    int return_index = -1;

    if (m_list) {

        POSITION l_pos = m_list->GetHeadPosition();
        CSequence* l_seq = NULL ;

        if (l_pos) {
            return_index = 0;

            while (l_pos) {

                l_seq = (CSequence*) m_list->GetNext(l_pos);

                if (l_seq) {

                    if ((WSE_Current == l_seq->GetWSEEditingEnum()) || (WSE_Planned_AND_Current == l_seq->GetWSEEditingEnum())) {
                        return return_index;
                    }
                }

                return_index ++;
            }
        }
    }


    return -1 ;
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::SetWSEEditingProtocolIndex(
    const int wse_editing_protocol_index
)
{
    LPCTSTR FUNC_NAME = _T("CPQMListCtrl::SetWSEEditingProtocolIndex");

    //+Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part2
    CString l_str = _T("");
    l_str.Format(_T("New WSE Editing Index is : %d"), wse_editing_protocol_index);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
    //-Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part2

    if (IsValidSequenceIndex(wse_editing_protocol_index)) {
        CSequence* l_seq = (CSequence*) GetItemData(wse_editing_protocol_index);

        if (l_seq) {

            if (l_seq->GetWSEEditingEnum() == WSE_Planned)
                l_seq->SetWSEEditingEnum(WSE_Planned_AND_Current);

            else if (l_seq->GetWSEEditingEnum() != WSE_Planned_AND_Current) { //Patni-MP/2010Sep22/Added/MVC008802
                //end of addition
                l_seq->SetWSEEditingEnum(WSE_Current);
            }
        }

        InvalidateItemsForPLANColumn(wse_editing_protocol_index);
        //Patni-Hemant/2009Dec1/Added/MVC005852
        UpdateWindow();
    }
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
int CPQMListCtrl::GetBitmapResourceIDForSARRegion(
    const int row_index
)
{

    CSequence* sequence_struct = GetSequenceStructForIndex(row_index);

    if (sequence_struct) {

        switch (sequence_struct->GetPrMode().m_anatomy) {

            case HEAD:
                return IDB_BITMAP_SAR_HEAD;

            case C_SPINE:
                return IDB_BITMAP_SAR_CSPINE;

            case TL_SPINE:
                return IDB_BITMAP_SAR_TLSPINE;

            case CHEST:
                return IDB_BITMAP_SAR_CHEST;

            case ABDOMEN:
                return IDB_BITMAP_SAR_ABDOMEN;

            case PELVIS:
                return IDB_BITMAP_SAR_PELVIS;

            case SHOULDER:
                return IDB_BITMAP_SAR_SHOULDER;

            case EXTREMITY:
                return IDB_BITMAP_SAR_EXTREMITY;

                //  case EXTREMITY2:
                //	return IDB_BITMAP_SAR_EXTREMITY2;

            case HAND:
                return IDB_BITMAP_SAR_HAND;

            case ANKLE:
                return IDB_BITMAP_SAR_ANKLE;
        }
    }

    //ASSERT(FALSE) ;
    return -1 ;
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
int CPQMListCtrl::GetFirstSelectedItem(
    bool* mulitple_selection /*= NULL*/
)const
{

    POSITION pos = GetFirstSelectedItemPosition();

    if (!pos)
        return -1;

    if (mulitple_selection)
        (*mulitple_selection) = (GetSelectedCount() > 1) ;

    return GetNextSelectedItem(pos);
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CPoint CPQMListCtrl::GetRightClickCell()
{

    return m_rightclick_cell;
}


std::vector<SeqInfo*> CPQMListCtrl::GetPairedVector()
{
    return m_vSeqInfo;
}


//************************************Method Header************************************
// Method Name  : GetProtocolNumberCol
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
int CPQMListCtrl::GetProtocolNumberCol()const
{
    return m_protocolno_column_index ;
}



//************************************Method Header************************************
// Method Name  : SetSequenceListPtr
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::SetSequenceListPtr(
    CSeqList* list
)
{
    m_list = list ;
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::SetBiasValueForProtocolNumber(
    const int bias_value_for_protocol_number
)
{

    LPCTSTR FUNC_NAME = _T("CPQMListCtrl::SetBiasValueForProtocolNumber");

    CString l_log_data = _T("");
    l_log_data.Format(_T("Series BIAS=%d"), bias_value_for_protocol_number);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_data);

    m_bias_value_for_protocol_number = bias_value_for_protocol_number ;
}
//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
int CPQMListCtrl::GetBiasValueForProtocolNumber(
)const
{

    return m_bias_value_for_protocol_number ;
}
//+Patni-HAR/2010Jan29/Added/DeFT#MVC006364
void CPQMListCtrl::SetChildStatusInSequence(
    const int f_start_index,
    const int* const selected_items,
    const int f_count,
    const CHILD_THREAD_STATUS f_child_thread_status
)const
{

    if (f_start_index == f_count) {
        return;
    }

    //+Patni-HAR/2010Jan29/Added/DeFT#MVC006364
    int* l_acq_order_arr = new int[f_count];
    memset(l_acq_order_arr, 0, sizeof(*l_acq_order_arr) * f_count);
    //vector <int> l_acq_order_vec;

    //CPQMView* p_parent = (CPQMView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

    CPQMView* p_parent  = g_MainView;

    CString l_string_scan_index(_T(""));
    CSequence* l_seq = NULL ;
    CString l_seqID;
    int l_unprotected_count = f_count;
    int l_unprotected_index = 0;

    for (int index = f_start_index; index < f_count ; index ++) {
        l_seq = (CSequence*) GetItemData(selected_items[index]);
        l_seq->GetCPASCOMSequence()->GetSequenceID(l_seqID);
        BSTR  l_bstr_seqID = l_seqID.AllocSysString();

        if	(TRUE == p_parent->GetProtectedStatus(l_bstr_seqID)) {
            l_unprotected_count--;
            ::SysFreeString(l_bstr_seqID);
            l_bstr_seqID = NULL;
            continue;
        }

        ::SysFreeString(l_bstr_seqID);
        l_bstr_seqID = NULL;

        l_seq->SetChildThreadStatus(f_child_thread_status);
        l_string_scan_index = l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER"));
        l_acq_order_arr[l_unprotected_index++] = _ttoi(l_string_scan_index);
    }


    if (l_acq_order_arr == NULL) {
        return;
    }

    p_parent->SetChildThreadStatusToIPC(l_acq_order_arr, l_unprotected_count , f_child_thread_status);

    //+Patni-ARD/2010Apr29/Modified/JaFT# MCM0229-00178_CDR_06 + code review
    DEL_PTR_ARY(l_acq_order_arr);
    //-Patni-ARD/2010Apr29/Modified/JaFT# MCM0229-00178_CDR_06 + code review
    //-Patni-HAR/2010Jan18/Added/DeFT#MVC006364
}
//-Patni-HAR/2010Jan29/Added/DeFT#MVC006364

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
BOOL CPQMListCtrl::OnNotify(
    WPARAM wparam,
    LPARAM lparam,
    LRESULT* result
)
{
    NMHDR* pNHMDR = (LPNMHDR) lparam;

    switch (pNHMDR->code) {
        case HDN_DIVIDERDBLCLICK : {

            LPNMHEADER pNHMDR = (LPNMHEADER) lparam;

            //SetColumnWidth(pNHMDR->iItem, 100);
            const unsigned int column_no = pNHMDR->iItem ;

            if ((column_no == m_sar_column_index) ||
                (column_no == m_status_column_index) ||
                (column_no == m_time_column_index) ||
                (column_no == m_protocolno_column_index) ||
                (column_no == m_mode_column_index) ||
                //+Patni-Hemant/v/Commented/IR#90/Queue List
                //(column_no == m_wseediting_column_index) ||

                (column_no == m_plan_column_index) //Added by Hemant On 12/22/2008 10:54:45 AM

               ) {

                SetColumnWidth(pNHMDR->iItem, m_column_default_width_array.GetAt(column_no));
                return TRUE;
            }
        }
    }

    return ListCtrlEx::OnNotify(wparam, lparam, result);
}



//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::OnItemchanged(
    NMHDR* pNMHDR,
    LRESULT* pResult
)
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    CPQMView* p_parent = (CPQMView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

    if (p_parent && !m_isdraging) {

        if (pNMListView->uNewState) {

            p_parent->OnSelectionStatusChanged();

        } else {

            const int first_selected = GetFirstSelectedItem() ;

            if (IsValidSequenceIndex(first_selected) && (first_selected != pNMListView->iItem)) {

                p_parent->OnSelectionStatusChanged();
            }
        }
    }
    AfxMessageBox(_T("in OnItemchanged"));
    *pResult = 0;
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
BOOL CPQMListCtrl::OnToolTipText(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
{


    const int MAX_TOOLTIP_WIDTH = 500;
    TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
    UINT nID = pNMHDR->idFrom;

    if (nID == 0)	  	// Notification in NT from automatically
        return FALSE;   	// created tooltip

    int row = ((nID - 1) >> 10) & 0x3fffff ;
    int col = (nID - 1) & 0x3ff;

    const CPoint cell_index(col, row);

    //+Patni-Manish/2009May29/Added/MVC002100-ToolTip Modification
    static CToolTipCtrl* pToolTip = NULL;
    CToolTipCtrl* ptt = AfxGetThreadState()->m_pToolTip;
    ptt->ModifyStyle(0, TTS_NOPREFIX);
    ptt->SetMaxTipWidth(MAX_TOOLTIP_WIDTH);

    if (ptt != pToolTip) { // new tooltip
        ptt->SetTipBkColor(RGB(40, 106, 175));
        ptt->SetTipTextColor(RGB(255, 255, 255));
        ptt->SetFont(&m_tooltip_font, FALSE);
        ptt->SetMaxTipWidth(-1);
        pToolTip = ptt;

    } else if (ptt->GetFont() != &m_tooltip_font) {

        ptt->SetTipBkColor(RGB(40, 106, 175));
        ptt->SetTipTextColor(RGB(255, 255, 255));

        ptt->SetFont(&m_tooltip_font);
        ptt->SetMaxTipWidth(-1);
    }

    //-Patni-Manish/2009May29/Added/MVC002100-ToolTip Modification

    CDC* l_tooltip_dc = ptt->GetDC();

    CRect calc_rect(0, 0, 0, 0);

    CFont* old_font = l_tooltip_dc->SelectObject(&m_tooltip_font);

    // Use Item's name as the tool tip. Change this for something different.
    // Like use its file size, etc.

    CString strTipText = GetToolTipTextForCell(cell_index, l_tooltip_dc , calc_rect);

    if (strTipText.IsEmpty()) {

        l_tooltip_dc->SelectObject(old_font);
        l_tooltip_dc->DeleteDC();

        *pResult = 0;
        return FALSE;
    }

    l_tooltip_dc->SelectObject(old_font);
    l_tooltip_dc->DeleteDC();

    DEL_PTR_ARY(m_pwchtip)

    m_pwchtip = new WCHAR[strTipText.GetLength() + 1];
    lstrcpy(m_pwchtip, strTipText);
    pTTTW->lpszText = (WCHAR*)m_pwchtip ;

    *pResult = 0;

    return TRUE;    // message was handled

}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
    ListCtrlEx::OnMouseMove(nFlags, point);

    //+Patni-PJS/2010Aug31/Added/TMSC-REDMINE-536
    if (g_MainView->IsWaitCursor()) {
        return;
    }

    //-Patni-PJS/2010Aug31/Commented/TMSC-REDMINE-536
    CRect rectClient;
    GetClientRect(rectClient);

    //+Patni-ARD/2010Jan27/Added/DeFT# MVC3817
    if (IsDragging() && m_pDragImage) {
        // Must be dragging, as there is a drag image.

        //+Patni-Hemant/2010May14/Added/JaFT/TMSC-REDMINE-276
        if (((nFlags & MK_LBUTTON) != MK_LBUTTON)) {
            CompleteDragDrop();
            return ;
        }

        //-Patni-Hemant/2010May14/Added/JaFT/TMSC-REDMINE-276


        // Move the drag image.
        CPoint ptDragImage(point);
        ClientToScreen(&ptDragImage);

        if (rectClient.bottom > point.y && rectClient.top < point.y) {

            ptDragImage.x = 390;
            m_pDragImage->DragMove(ptDragImage);
            // Leave dragging so we can update potential drop target selection.
            m_pDragImage->DragLeave(CWnd::GetDesktopWindow());
        }

        // Force x coordinate to always be in list control - only interested in y coordinate.
        // In effect the list control has captured all horizontal mouse movement.
        static const int nXOffset = 8;
        CRect rect;
        GetWindowRect(rect);
        CWnd* pDropWnd = CWnd::WindowFromPoint(CPoint(rect.left + nXOffset, ptDragImage.y));

        int* l_selected_items = NULL ;
        const int l_count = GetselectedItems(&l_selected_items);

        CSequence* l_seq = NULL;

        if ((l_count == 1) && l_selected_items) {
            l_seq = (CSequence*)GetItemData(l_selected_items[0]);
        }

        // Get the window under the drop point.
        if (pDropWnd == this) {
            // Still in list control so select item under mouse as potential drop target.
            point.x = nXOffset;	// Ensures x coordinate is always valid.

            //+Patni-Hemant/2009Sept10/Modified/MVC2884/Drop of sequences
            int l_hit_rowindex = HitTest(point);

            if (l_hit_rowindex < 0) {
                l_hit_rowindex = (point.y > rectClient.top) ? GetItemCount() : -1;
            }

            //-Patni-ARD/2010Jan27/Added/DeFT# MVC3817
            UpdateSelection(l_hit_rowindex);
            //-Patni-Hemant/2009Sept10/Modified/MVC2884/Drop of sequences
        }


        CPoint ptClientDragImage(ptDragImage);
        ScreenToClient(&ptClientDragImage);

        // Client rect includes header height, so ignore it, i.e.,
        // moving the mouse over the header (and higher) will result in a scroll up.
        CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);

        if (pHeader) {
            CRect rectHeader;
            pHeader->GetClientRect(rectHeader);
            rectClient.top += rectHeader.Height();
        }

        if (ptClientDragImage.y < rectClient.top) {
            // Mouse is above the list control - scroll up.
            SetScrollTimer(scrollUp);

        } else if (ptClientDragImage.y > rectClient.bottom) {
            // Mouse is below the list control - scroll down.
            SetScrollTimer(scrollDown);

        } else {
            KillScrollTimer();
        }

        if (rectClient.bottom > point.y && rectClient.top < point.y) {
            m_pDragImage->DragEnter(CWnd::GetDesktopWindow(), ptDragImage);
        }

        //-Patni-ARD/2010Jan27/Added/DeFT# MVC3817

    } else {
        KillScrollTimer();
    }
}



//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
    //+Patni-Hemant/2010May14/Modified/JaFT/TMSC-REDMINE-276
    //    //+Patni-ARD/2010Jan27/Added/DeFT# MVC3817
    //    if (IsDragging() && m_pDragImage ) {
    //        KillScrollTimer();
    //
    //        // Release the mouse capture and end the dragging.
    //        ::ReleaseCapture();
    //
    //		m_pDragImage->DragLeave(CWnd::GetDesktopWindow());
    //
    //        m_pDragImage->EndDrag();
    //
    //        if ( m_pDragImage ){
    //            delete m_pDragImage;
    //            m_pDragImage = NULL;
    //        }
    //
    //        // Drop the item on the list.
    //        DropItem();
    //
    //        m_isdraging = false;
    //    }
    //    //-Patni-ARD/2010Jan27/Added/DeFT# MVC3817
    //+Patni-PJS/2010Aug31/Added/TMSC-REDMINE-536
    if (g_MainView->IsWaitCursor()) {
        return;
    }

    //-Patni-PJS/2012010Aug310Aug06/Commented/TMSC-REDMINE-536
    CompleteDragDrop(false);
    //-Patni-Hemant/2010May14/Modified/JaFT/TMSC-REDMINE-276

    ListCtrlEx::OnLButtonUp(nFlags, point);
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPCTSTR FUNC_NAME = _T("CPQMListCtrl::OnBeginDrag");

    //+Patni-PJS/2010Aug31/Added/TMSC-REDMINE-536
    //+Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1
    if (g_MainView->IsWaitCursor() || !(g_MainView->CanPerformGUIOperation()) || g_MainView->IsAMB()) {

        PQM_TRACE(USER_FUNC_MARKER,  _T("CPQMListCtrl::OnBeginDrag"),
                  _T("Could not start rearrange"));
        return;
    }

    //-Patni-Hemant-MP/2011Mar08/Added/TMSC-REDMINE-1486-Part1
    //-Patni-PJS/2010Aug31/Commented/TMSC-REDMINE-536

    //only one item can be dragged..
    if (GetSelectedCount() != 1) {

        //+Patni-DKH/2009Aug24/Modified/cpp test corrections
        if (NULL == pResult) {
            PQM_TRACE(USER_FUNC_MARKER,
                      FUNC_NAME,
                      _T("pResult pointer is NULL"));
            return;

        } else {
            //-Patni-DKH/2009Aug24/Modified/cpp test corrections
            *pResult = 0;
            return ;
        }//Patni-DKH/2009Aug24/Modified/cpp test corrections
    }

    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    if (pNMListView) {
        m_nPrevDropIndex	= -1;
        m_uPrevDropState	= NULL;

        // Items being dragged - can be one or more.
        m_anDragIndexes.RemoveAll();
        POSITION pos = GetFirstSelectedItemPosition();

        while (pos) {
            m_anDragIndexes.Add(GetNextSelectedItem(pos));
        }

        DWORD dwStyle = GetStyle();

        if ((dwStyle & LVS_SINGLESEL) == LVS_SINGLESEL) {
            // List control is single select; we need it to be multi-select so
            // we can show both the drag item and potential drag target as selected.
            m_dwStyle = dwStyle;
            ModifyStyle(LVS_SINGLESEL, NULL);
        }

        //-Patni-ARD/2010Jan27/Added/DeFT# MVC3817
        if (m_anDragIndexes.GetSize() > 0) {
            DEL_PTR(m_pDragImage)

            CPoint ptDragItem;

            int* l_selected_items = NULL ;
            const int  l_count = GetselectedItems(&l_selected_items);

            CSequence* l_seq = NULL;

            if ((l_count == 1) && l_selected_items) {
                l_seq = (CSequence*)GetItemData(l_selected_items[0]);
            }

            CString l_str_scanID(_T(""));
            int l_acq_status = -1;

            if (l_seq) {
                l_str_scanID = l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("COMM"));
                l_acq_status = l_seq->GetAcqStatus();
            }

            CRect rectClient;
            CPoint point;
            GetCursorPos(&point);
            GetClientRect(rectClient);
            m_pDragImage = CreateDragImageEx(&ptDragItem, l_str_scanID, l_acq_status);

            if (m_pDragImage) {
                m_pDragImage->BeginDrag(0, CPoint(122, 0));
                m_pDragImage->DragEnter(CWnd::GetDesktopWindow(), ptDragItem);
                SetCapture();
            }

            m_isdraging = true;
        }

        //-Patni-ARD/2010Jan27/Added/DeFT# MVC3817
    }
    g_MainView->GetTiPrepController()->SendRefreshMsgToTiPrep();
    AfxMessageBox(_T("in OnBeginDrag"));
    *pResult = 0;
}


//************************************Method Header************************************
// Method Name  : OnTimer
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::OnTimer(UINT nIDEvent)
{
    //+Patni-ARD/2010Jan27/Added/DeFT# MVC3817
    if (nIDEvent == SCROLL_TIMER_ID && IsDragging() && m_pDragImage) {
        WPARAM wParam	= 0u;	//Patni-PP/2009Jun01/Modified/SU14 CPP test initialization corrections
        int nDropIndex	= -1;

        if (m_ScrollDirection == scrollUp) {
            wParam		= MAKEWPARAM(SB_LINEUP, 0);
            nDropIndex	= m_nDropIndex - 1;

        } else if (m_ScrollDirection == scrollDown) {
            wParam		= MAKEWPARAM(SB_LINEDOWN, 0);
            nDropIndex	= m_nDropIndex + 1;
        }

        m_pDragImage->DragShowNolock(FALSE);
        SendMessage(WM_VSCROLL, wParam, NULL);
        UpdateSelection(nDropIndex);
        m_pDragImage->DragShowNolock(TRUE);

    } else {
        ListCtrlEx::OnTimer(nIDEvent);
    }

    //-Patni-ARD/2010Jan27/Added/DeFT# MVC3817
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::OnLButtonDown(
    UINT flags,
    CPoint point
)
{
    //lbuttondown is performed first so that it changes the selection ..
    //+Patni-PJS/2010Aug06/Added/TMSC-REDMINE-536
    if (g_MainView->IsWaitCursor()) {
        return;
    }

    //-Patni-PJS/2010Aug06/Commented/TMSC-REDMINE-536.
    const int option_count = PerformActionOnMouseClickAt(point);
    CPoint cell_number = GetCellNoFromPoint(point);

    m_isdraging = false;

    if (IsValidSequenceIndex(cell_number.y)) {

        if (((unsigned int)cell_number.x == m_mode_column_index)) {

            //if(!IsRowSelected(m_hWnd, cell_number.y))
            //	ListCtrlEx::OnLButtonDown(flags, point);

            if (option_count != -1)
                OnModeOptionClicked(cell_number.y, option_count);

            return ;
        }

        if (IsRowSelected(m_hWnd, cell_number.y)) {

            if ((flags & ~MK_LBUTTON) == 0) {
                if ((unsigned int)cell_number.x == m_sar_column_index) {

                    OnSarCellClicked(cell_number.y);
                    return ;
                }
            }
        }
    }

    //+Patni-Hemant/2009Nov16/Commented/IR#90/Bottom Buttons
    //
    //    //+Patni-Manish/2009May26/Added/JFT#IR18/GUI Renewal
    //    CPQMView *p_parent = NULL;
    //    p_parent = (CPQMView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();
    //    int l_nBitmapResourceID = GetBitmapResourceIDForSARRegion(cell_number.y);
    //    p_parent->SetBitmapForAntomyButton(l_nBitmapResourceID);
    //    //-Patni-Manish/2009May26/Added/JFT#IR18/GUI Renewal
    //
    //-Patni-Hemant/2009Nov16/Commented/IR#90/Bottom Buttons


    //Added by Hemant On 8/20/2008 4:40:18 PM - keep track of first selected item..
    //before click..
    const int first_selected_item = GetFirstSelectedItem();

    //no mode option click.. so default action..
    ListCtrlEx::OnLButtonDown(flags, point);

    //+Patni-ARD/2010Jan27/Added/DeFT# MVC3817
    if (m_pDragImage) {

        IMAGEINFO l_image_info;
        m_pDragImage->GetImageInfo(0, &l_image_info);

    }

    //-Patni-ARD/2010Jan27/Added/DeFT# MVC3817
    //-------------------------------------------------------------------------
    //Added by Hemant On 8/20/2008 4:40:18 PM
    if (GetFirstSelectedItem() == -1) {

        //if clikc has removed the selection from all item, restore selection to only first item..
        //
        if (IsValidSequenceIndex(first_selected_item))
            EnableHighlighting(first_selected_item , true);
    }

    //-------------------------------------------------------------------------
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::OnRButtonDown(
    UINT flags,
    CPoint point
)
{
    //+Patni-PJS/2010Aug06/Added/TMSC-REDMINE-536
    if (g_MainView->IsWaitCursor()) {
        return;
    }

    //-Patni-PJS/2010Aug06/Commented/TMSC-REDMINE-536
    //Patni-Hemant/2010May14/Added/JaFT/TMSC-REDMINE-276
    CompleteDragDrop();

    CPoint cell_number = GetCellNoFromPoint(point);

    //Added by Hemant On 8/20/2008 4:40:18 PM - keep track of first selected item..
    //before click..
    if (!IsValidSequenceIndex(cell_number.y)) {

        //do not perform R click operation on invalid row index..
        return ;
    }

    if (!IsRowSelected(m_hWnd, cell_number.y)) {

        //beware .. order of calling does matter..

        if ((unsigned int)cell_number.x != m_mode_column_index)
            ListCtrlEx::OnRButtonDown(flags, point);

        PerformActionOnRButtonClickAt(point);

    } else
        PerformActionOnRButtonClickAt(point);

}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::OnLButtonDblClk(
    UINT flags,
    CPoint point
)
{
    //+Patni-PJS/2010Aug31/Added/TMSC-REDMINE-536
    if (g_MainView->IsWaitCursor()) {
        return;
    }

    //-Patni-PJS/2010Aug31/Commented/TMSC-REDMINE-536

    //Added by Hemant On 8/20/2008 4:40:18 PM - keep track of first selected item..
    //before click..
    CPoint cell_number = GetCellNoFromPoint(point);

    if (!IsValidSequenceIndex(cell_number.y)) {

        //do not perform dbl click operation on invalid row index..
        return ;
    }

    //no mode option click.. so default action..
    ListCtrlEx::OnLButtonDblClk(flags, point);

    PerformActionOnDblClickAt(point);

}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::OnRButtonDblClk(
    UINT flags,
    CPoint point
)
{
    //+Patni-PJS/2010Aug31/Added/TMSC-REDMINE-536
    if (g_MainView->IsWaitCursor()) {
        return;
    }

    //-Patni-PJS/2010Aug31/Commented/TMSC-REDMINE-536

    //Added by Hemant On 8/20/2008 4:40:18 PM - keep track of first selected item..
    //before click..

    if (!IsValidSequenceIndex(GetCellNoFromPoint(point).y)) {

        //do not perform dbl click operation on invalid row index..
        return ;
    }

    //no mode option click.. so default action..
    ListCtrlEx::OnRButtonDblClk(flags, point);
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::OnKeyDown(
    UINT nChar,
    UINT nRepCnt,
    UINT nFlags
)
{
    //+Patni-PJS/2010Aug31/Added/TMSC-REDMINE-536
    if (g_MainView->IsWaitCursor()) {
        return;
    }

    //-Patni-PJS/2010Aug31/Commented/TMSC-REDMINE-536
    switch (nChar) {

        case VK_SPACE :
            return ;
    }

    ListCtrlEx::OnKeyDown(nChar, nRepCnt, nFlags);
}


//************************************Method Header************************************
// Method Name  : HideContexMenu
// Author       : PATNI/
// Purpose      :
//***********************************************************************************
LRESULT CPQMListCtrl::HideContexMenu(
    WPARAM wParam,
    LPARAM lParam
)
{
    SetCapture();
    ReleaseCapture();
    return 0;
}

//************************************Method Header************************************
// Method Name  : OnCaptureChanged
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::OnCaptureChanged(CWnd* pWnd)
{
    if (IsDragging() && m_pDragImage) {
        SetCapture();
    }

}
//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
int CPQMListCtrl::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
    CPQMListCtrl* listctrl = const_cast<CPQMListCtrl*>(this);
    const CPoint cell_number = listctrl->GetCellNoFromPoint(point) ;

    CRect clCellRect = listctrl->GetCellRect(cell_number.y , cell_number.x) ;

    if (cell_number.y == -1) {
        return -1;
    }

    int mode_option_index = 0;

    CRect option_rect(0, 0, 0, 0);

    for (int option_count = 0; option_count < TOTAL_MODE_FALGS ; option_count ++) {
        option_rect = listctrl->GetOptionRectForModeOptionIndex(cell_number.y, option_count);

        if (!option_rect.IsRectEmpty()) {

            if (option_rect.PtInRect(point)) {
                mode_option_index = option_count + 1;
                break ;
            }
        }
    }

    const int coulmn_index = (((cell_number.x) << 5) + ((mode_option_index) & 0xf) + 1);

    pTI->hwnd = m_hWnd;

    pTI->uId = (UINT)(((cell_number.y) << 10) + ((coulmn_index) & 0x3ff) + 1);

    pTI->lpszText = LPSTR_TEXTCALLBACK;


    pTI->rect = clCellRect;

    return pTI->uId;
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::MeasureItem(
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
void CPQMListCtrl::DrawItem(
    LPDRAWITEMSTRUCT draw_item_struct
)
{
    //+Patni-Hemant/2009Nov15/Added/IR#90/Queue List
    const int item_no = draw_item_struct->itemID;

    CSequence* l_sequence_struct = GetSequenceStructForIndex(item_no);

    if (l_sequence_struct) {

        switch (l_sequence_struct->GetAcqStatus()) {
            case PR_ACQ_STATUS_SUCCESS :
            case PR_ACQ_STATUS_FAIL:
                SetTextColor(RGB(60, 65 , 75), false);
                SetTextSelColor(RGB(60, 65 , 75), false);
                break ;

            case PR_ACQ_STATUS_CURRENT:
            case PR_ACQ_STATUS_WAIT:
            default :
                SetTextColor(RGB(225, 234, 254), false);
                SetTextSelColor(RGB(225, 234, 254), false);
        }
    }

    //-Patni-Hemant/2009Nov15/Added/IR#90/Queue List


    ListCtrlEx::DrawItem(draw_item_struct);

    CDC dc;
    dc.Attach(draw_item_struct->hDC);

    const int prev_mode = dc.SetBkMode(OPAQUE);

    //+Patni-HAR/2010May11/Added/MVC007466
    COLORREF clrText;
    COLORREF clrBk;
    const bool bSelected = (ODS_SELECTED == (draw_item_struct->itemState & ODS_SELECTED));
    GetCellColors(draw_item_struct->itemID, 0, bSelected, clrText, clrBk);
    dc.SetBkColor(clrBk);
    dc.SetTextColor(clrText);
    CBrush bgBrush(clrBk);
    //-Patni-HAR/2010May11/Added/MVC007466
    //Patni-Hemant/2009Nov15/Commented/IR#90/Queue List
    // const int item_no = draw_item_struct->itemID;

    CRect cellrect(0, 0, 0, 0);
    int current_col = 0;
    CString cell_string = _T("");

    //Commented by Hemant On 1/21/2009 11:57:07 AM
    //for defect 769, we wil now apply alignment for protocol no
    //according to display dictionary
    //previously it was customized according to requirement.
    //I am not deleting following code for future use.

    const COLORREF l_sar_over_color = RGB(245, 195, 75);

    sarInfo_p l_sar_info = l_sequence_struct->GetSARInfoPtrForSeq();

    if (l_sar_info && ((IEC_SAR_OVER == l_sar_info->ope_mode) || l_sar_info->coilProtect)) {
        if (PR_ACQ_STATUS_CURRENT == l_sequence_struct->GetAcqStatus() || PR_ACQ_STATUS_WAIT == l_sequence_struct->GetAcqStatus()) {
            dc.SetTextColor(l_sar_over_color);
        }
    }

    if (m_protocolno_column_index != -1) {

        cell_string = GetCellString(CPoint(m_protocolno_column_index, item_no));

        DrawTextWithUpdatedStringAndFormate(item_no, m_protocolno_column_index, m_number_column_alignment, dc, bgBrush, cell_string);
    }

    if (m_scan_id_column_index != -1) {

        CPASParamStore* l_seq_para =  l_sequence_struct->GetCPASCOMSequence()->GetSequenceParam();

        if (l_seq_para) {
            cell_string = l_seq_para->GetString(_T("COMM"));
        }

        DrawTextWithUpdatedStringAndFormate(item_no, m_scan_id_column_index, m_scan_id_column_alignment, dc, bgBrush, cell_string);


    }

    if (m_time_column_index != -1) {

        current_col = m_time_column_index;
        cellrect = GetCellRect(item_no, current_col);
        CSequence* l_seq = (CSequence*) GetItemData(item_no);

        cell_string = l_seq->GetScanTimeString();

        CRect calc_rect ;
        dc.DrawText(cell_string, &calc_rect, DT_CALCRECT);


        UINT flags = DT_END_ELLIPSIS | DT_VCENTER | DT_SINGLELINE;
        flags |= (cellrect.Width() > calc_rect.Width()) ? DT_CENTER : DT_LEFT;

        //Patni-HAR/2010May11/Added/MVC007466
        cellrect.top += 1;//Patni-Hemant/2011Apr06/Added/TMSC-REDMINE-1600
        dc.FillRect(&cellrect, &bgBrush);
        dc.DrawText(cell_string, cellrect, flags);

        //Patni-Hemant/2009Nov15/Commented/ IR#90/Queue List
        //DrawAutoScanIcon(&dc, item_no, cellrect);
    }

    dc.SetTextColor(clrText);

    if (m_delay_column_index != -1) {

        current_col = m_delay_column_index;
        cellrect = GetCellRect(item_no, current_col);
        CSequence* l_seq = (CSequence*) GetItemData(item_no);

        CString l_delay_time = _T("");

        if (l_seq) {
            l_delay_time = l_seq->GetDelayTimeString();

            if (!l_delay_time.Compare(ZERO_TIME) || !l_delay_time.Compare(INVALID_STRING)) {
                l_delay_time = _T("");
            }
        }

        CRect l_calc_rect ;
        dc.DrawText(l_delay_time, &l_calc_rect, DT_CALCRECT);

        UINT l_flags = DT_END_ELLIPSIS | DT_VCENTER | DT_SINGLELINE;

        l_flags |= (cellrect.Width() > l_calc_rect.Width()) ? DT_CENTER : DT_LEFT;

        cellrect.top += 1;
        dc.FillRect(&cellrect, &bgBrush);
        dc.DrawText(l_delay_time, cellrect, l_flags);
    }


    if (m_sar_column_index != -1)
        DrawSARCellData(&dc, item_no);

    //Added by Hemant On 12/22/2008 10:54:11 AM
    if (m_plan_column_index != -1)
        DrawPLANCellData(&dc, item_no, m_plan_column_index);

    if (m_mode_column_index != -1)
        DrawModeCellData(&dc, item_no);

    if (m_status_column_index != -1)
        DrawStatusCellData(&dc, item_no);

    dc.SetBkMode(prev_mode);
    dc.Detach() ;
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::OnModeOptionClicked(
    const int row_index,
    const int mode_option_index
)const
{

    CPQMView* p_parent = (CPQMView*)GetParent();
    ASSERT(p_parent != NULL);

    if (p_parent->IsAMBInvoked())
        return;

    CSequence* sequence_struct = GetSequenceStructForIndex(row_index);

    if (!sequence_struct) {
        ASSERT(FALSE);
        return ;
    }

    bool l_is_wse_open = IsValidSequenceIndex(GetWSEEditingProtocolIndex()) > 0;

    switch (mode_option_index) {

        case AUTO_TRANSFER_OPTION : {

            //p_parent->OnClickButtonAutoTransfer(row_index);
        }
        break ;

        case BREATH_HOLD_OPTION : {

            //+Patni-Abhishek/2010Dec14/Modified/IR-180
            if (l_is_wse_open)
                return;

            //-Patni-Abhishek/2010Dec14/Modified/IR-180

            p_parent->OnClickButtonBreathHold(row_index);
        }
        break ;

        case AUTO_VOICE_OPTION : {

            p_parent->OnToggleAutoVoice(row_index);
        }
        break ;

        case CONTRAST_AGENT_OPTION : {

            //+Patni-Abhishek/2010Dec14/Modified/IR-180
            if (l_is_wse_open)
                return;

            //-Patni-Abhishek/2010Dec14/Modified/IR-180

            p_parent->ApplyContrastAgentSettings(row_index, sequence_struct->GetPrMode().m_contrast ? -1 : 0) ;
        }
        break ;

        case MOVE_COUCH_OPTION : {

            //+Patni-RUP/2010Jul23/Modified/TMSC-REDMINE-466
            //p_parent->OnClickButtonMoveCouch(row_index);
            p_parent->OnToggleMoveCouchOption(row_index);
            //-Patni-RUP/2010Jul23/Modified/TMSC-REDMINE-466
        }
        break ;
    }
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
int CPQMListCtrl::DuplicateSequenceforIndex(
    const int index_being_duplicated,
    int new_index_for_duplicated_protocol /*= -1*/,
    const int offset_index, /*= -1*/
    const int f_group_id	/*= 0*/
)
{

    LPCTSTR FUNC_NAME = _T("CPQMListCtrl::DuplicateSequenceforIndex");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-KSS/2010June25/ADDED/IR-156_2
    CPQMView* p_parent = (CPQMView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();
    //-Patni-KSS/2010June25/ADDED/IR-156_2

    if (!IsSequenceIndexValid(index_being_duplicated)) {
        ASSERT(FALSE);
        return -1;
    }

    CSequence* seq_to_duplicate = (CSequence*) GetItemData(index_being_duplicated);

    if (!seq_to_duplicate) {
        ASSERT(FALSE);
        return -1;
    }

    const int first_wait_protocol_index = GetFirstWaitProtocolIndex(true);

    const int status = seq_to_duplicate->GetAcqStatus();

    //const int index_being_duplicated = index;	//zero based..

    //abreviations : C - current , F - Fail , D - Done/ success...
    //

    if (new_index_for_duplicated_protocol == -1) {

        switch (status) {

            case PR_ACQ_STATUS_SUCCESS :
            case PR_ACQ_STATUS_FAIL :
            case PR_ACQ_STATUS_CURRENT :  {

                //if the item being duplicated is C , F , D then duplicate it just above the first wait protocol..
                //it there isn't any first wait protocol.. put it at last..

                if (first_wait_protocol_index < 0) {

                    //we don't have any wait protocol..

                    //add at last..
                    new_index_for_duplicated_protocol = GetItemCount();

                } else {

                    new_index_for_duplicated_protocol = first_wait_protocol_index;
                }
            }
            break ;

            case PR_ACQ_STATUS_WAIT : {

                //when wait state protocol is being duplicated.. duplicate it to just below it..
                new_index_for_duplicated_protocol = index_being_duplicated + 1 ;
            }
            break ;

            default : {

                ASSERT(FALSE);
                return -1;
            }
        }
    }

    if (offset_index != -1) {

        new_index_for_duplicated_protocol += offset_index;
        //if(IsSequenceIndexValid(new_index_for_duplicated_protocol))
    }

    //    CPQMView* p_parent = NULL;
    //    p_parent = (CPQMView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

    //+Patni-PJS/2010Mar12/Added/DeFT# MCM0229-00178
    CSequence*  l_curr_seq = seq_to_duplicate;

    if (PR_ACQ_STATUS_CURRENT == status) {
        l_curr_seq = p_parent->GetCurrentSequence();

        if (NULL == l_curr_seq) {
            return -1;
        }
    }

    //+Patni-Abhishek/2010Dec14/Modified/IR-180
    //+Patni-Abhishek/2011Feb09/Modified/IR-180
    AUTOVOICE_OPTIONS l_auto_voice_option = l_curr_seq->GetAutoVoiceOption();

    if (NONE_NONE != l_auto_voice_option) {
        l_curr_seq->SetPrevAutoVoiceOption(l_auto_voice_option);
    }

    MOVECOUCH_METHOD l_movecouch_method = l_curr_seq->GetMoveCouchMethodOption();

    if (NOMOVE != l_movecouch_method) {
        l_curr_seq->SetPrevMoveCouchMethodOption(l_movecouch_method);
    }

    CPqmPrMode l_prmode_prev = l_curr_seq->GetPrMode();

    if (l_prmode_prev.m_contrast != 0) {
        l_prmode_prev.m_Prev_Contrast = l_prmode_prev.m_contrast;
        l_prmode_prev.m_Prev_Str_Contrast = l_prmode_prev.m_str_contrast;
        l_curr_seq->SetPrMode(l_prmode_prev);
    }

    //-Patni-Abhishek/2011Feb09/Modified/IR-180
    //-Patni-Abhishek/2010Dec14/Modified/IR-180

    return p_parent->DuplicateSequenceforIndex(l_curr_seq, index_being_duplicated,
            new_index_for_duplicated_protocol, m_isdraging, f_group_id);
    //-Patni-PJS/2010Mar12/Added/DeFT# MCM0229-00178
}


//************************************Method Header************************************
// Method Name  : DropItem
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::DropItem(
)
{

    LPCTSTR FUNC_NAME = _T("CPQMListCtrl::DropItem");
    PQM_TRACE_SCOPED(FUNC_NAME);

    RestorePrevDropItemState();
    //---------------

    if (m_anDragIndexes.GetSize() == 1) {

        CPQMView* p_parent = (CPQMView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

        if (p_parent) {

            p_parent->SetWaitCursor(true) ;

            DragDropListItems((int) m_anDragIndexes[0], m_nDropIndex);

            //Patni-ARD/2009Sep8/Modified/Prototype 2
            //+Patni-HAR/2010Feb24/Modified/MSA0251-00187
            //+Patni-PJS/2010Jun17/Modified/V1.20#MVC008324_1

            //+Patni-Hemant/Optimization
            /*
            if (TYPE_FALSE == p_parent->GetGUIListProtocolInfo()) {
                g_MainView->SetMismatchErrorFlag(TRUE);
                //-Patni-PJS/2010Jun17/Modified/V1.20#MVC008324_1

            } else {
                g_MainView->SetMismatchErrorFlag(FALSE);
            }
            */

            g_MainView->SetMismatchErrorFlag((TYPE_FALSE == p_parent->GetGUIListProtocolInfo()) ? TRUE : FALSE);
            //-Patni-Hemant/Optimization

            //-Patni-HAR/2010Feb24/Modified/MSA0251-00187
            p_parent->SetWaitCursor(false) ;
        }
    }

    if (m_dwStyle != NULL) {
        // Style was modified, so return it back to original style.
        ModifyStyle(NULL, m_dwStyle);
        m_dwStyle = NULL;
    }
}


void CPQMListCtrl::RestorePrevDropItemState()
{
    if (m_nPrevDropIndex > -1 && m_nPrevDropIndex < GetItemCount()) {
        // Restore state of previous potential drop target.
        SetItemState(m_nPrevDropIndex, m_uPrevDropState, LVIS_SELECTED);
    }
}


void CPQMListCtrl::UpdateSelection(const int nDropIndex)
{
    if (nDropIndex > -1 && nDropIndex < GetItemCount()) {
        // Drop index is valid and has changed since last mouse movement.

        if (m_nDropIndex == nDropIndex)
            return ;

        RestorePrevDropItemState();

        // Save information about current potential drop target for restoring next time round.
        m_nPrevDropIndex = nDropIndex;
        m_uPrevDropState = GetItemState(nDropIndex, LVIS_SELECTED);

        // Select current potential drop target.
        //Patni-ARD/2010Jan27/Commented/DeFT# MVC3817
        //SetItemState(nDropIndex, LVIS_SELECTED, LVIS_SELECTED);
        m_nDropIndex = nDropIndex;		// Used by DropItem().

        UpdateWindow();

    } else {
        RestorePrevDropItemState();
        EnsureVisible(m_nDropIndex, TRUE);

        //+Patni-Hemant/2009Sept10/Modified/MVC2884/Drop of sequences
        if (nDropIndex >= 0) {
            m_nDropIndex = nDropIndex;

        } else {
            m_nDropIndex = -1;
        }

        //-Patni-Hemant/2009Sept10/Modified/MVC2884/Drop of sequences
    }
}


void CPQMListCtrl::KillScrollTimer()
{
    if (m_uScrollTimer != 0) {
        KillTimer(SCROLL_TIMER_ID);
        m_uScrollTimer		= 0;
        m_ScrollDirection	= scrollNull;
    }
}


//************************************Method Header************************************
// Method Name  : IsSequenceIndexValid
// Author       : PATNI/ HEMANT
// Purpose      : for checking 0 based sequnece index only
//***********************************************************************************
bool CPQMListCtrl::IsSequenceIndexValid(
    const int index
)const
{
    return ((index < GetItemCount()) && index >= 0);
}


//************************************Method Header************************************
// Method Name  : DragDropListItems
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPQMListCtrl::DragDropListItems(
    const int drag_index,
    int drop_index
)
{
    LPCTSTR FUNC_NAME = _T("CPQMListCtrl::DragDropListItems");

    //+Patni-HEMANT-HAR/2010Mar04/Added/MSA00251-00246
    CString l_str(_T(""));
    l_str.Format(_T("[Drag Index = %d][Drop Index = %d]"), drag_index, drop_index);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
    //-Patni-HEMANT-HAR/2010Mar04/Added/MSA00251-00246

    //check for validity of the indexs..
    if (!IsValidSequenceIndex(drag_index)) {

        ASSERT(FALSE);
    }

    //for dragging C, F , D items : only can be dropped on W state ...
    //when dropping C, F , D items on W state, duplicate dragged item just over dropped item..

    //for ddragging W state.. can only be dropped W...
    //it dropped W is below dragged W : move the dragged item over it..
    //it dropped W is above dragged W : move the dragged item below it..

    CSequence* seq_to_dragged = (CSequence*) GetItemData(drag_index);

    if (!seq_to_dragged) {
        ASSERT(FALSE);
        return false;
    }

    //+Patni-HEMANT-HAR/2010Mar04/Added-Modified/MSA00251-00246
    int status = seq_to_dragged->GetAcqStatus() ;

    if ((PR_ACQ_STATUS_WAIT == status) && (drag_index == g_MainView->GetScanIndex())) {
        status = PR_ACQ_STATUS_CURRENT;
    }

    //-Patni-HEMANT-HAR/2010Mar04/Added-Modified/MSA00251-00246


    switch (status) {

        case PR_ACQ_STATUS_SUCCESS :
        case PR_ACQ_STATUS_FAIL :
        case PR_ACQ_STATUS_CURRENT :  {

            //dragged item is C , F or D here..
            //now decide where to put the duplicated item..

            //if drop index is valid .. then use dropped index.. else put it at last..

            if (IsValidSequenceIndex(drop_index)) {

                switch (((CSequence*)(GetItemData(drop_index)))->GetAcqStatus()) {

                    case PR_ACQ_STATUS_SUCCESS :
                    case PR_ACQ_STATUS_FAIL :
                    case PR_ACQ_STATUS_CURRENT :  {

                        return true;
                    }

                    break ;

                    //+Patni-HEMANT-HAR/2010Mar04/Added/MSA00251-00246
                    case PR_ACQ_STATUS_WAIT : {

                        const int l_scan_index = g_MainView->GetScanIndex() ;

                        if (drop_index == l_scan_index) {


                            l_str.Format(_T("Duplicating on Scan Index =>[scan index = %d][Drop Index = %d]"),
                                         l_scan_index, drop_index);


                            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);

                            return true ;
                        }
                    }

                    break ;

                    default :
                        return true ;
                        //-Patni-HEMANT-HAR/2010Mar04/Added/MSA00251-00246

                }

            } else {

                //+Patni-Manish/2009Aug07/Added/MVC2884/Drop of sequences
                if (-1 == drop_index) {
                    return true;

                } else {
                    drop_index = GetItemCount();		//adding new item at last now..
                }

                //-Patni-Manish/2009Aug07/Added/MVC2884/Drop of sequences
            }

            int* l_new_groupids = NULL;

            int l_selected_item[] = {drag_index};

            if (g_MainView) {
                l_new_groupids = g_MainView->AllocateGroupIdForDuplicateProtocols(l_selected_item, 1);
            }

            int l_grp_id = 0;

            if (l_new_groupids) {
                l_grp_id = l_new_groupids[0];
            }

            const int index_for_new_item_inserted = DuplicateSequenceforIndex(drag_index, drop_index, -1, l_grp_id);

            DEL_PTR_ARY(l_new_groupids);


            if (IsValidSequenceIndex(index_for_new_item_inserted))  {
                SelectOnlyItem(index_for_new_item_inserted);
                EnsureVisible(index_for_new_item_inserted, FALSE);
            }

#ifdef _DEBUG

            if (index_for_new_item_inserted < 0)
                ASSERT(FALSE);

#endif //_DEBUG

        }
        break ;

        case PR_ACQ_STATUS_WAIT : {

            //now move this dragged item.. according to dropped item..

            //+Patni-HEMANT/ADDED On 8/21/2009 3:04:42 PM/ ACE+JFT / MVC2884
            if (!IsValidSequenceIndex(drop_index)) {
                drop_index = (drop_index >= GetItemCount()) ? (GetItemCount() - 1) : GetFirstWaitProtocolIndex(true);

            } else {

                //Here..dropped index is valid...

                if (drop_index == drag_index) {
                    return true;	//don't do anything..just return..
                }

                //now if the dropped index is NON WAIT, the dragged will become first wait..
                CSequence* dropped_seq = (CSequence*) GetItemData(drop_index);

                //+Patni-HEMANT-HAR/2010Mar04/Added/MSA00251-00246
                if ((drop_index == g_MainView->GetScanIndex()) || (PR_ACQ_STATUS_WAIT != dropped_seq->GetAcqStatus())) {

                    l_str.Format(_T("[In loop of scan index or protocol status][Drop Index = %d]"), drop_index);

                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);

                    drop_index = GetFirstWaitProtocolIndex(true) ;
                }

                //-Patni-HEMANT-HAR/2010Mar04/Added/MSA00251-00246
            }

            //-Patni-HEMANT/ADDED On 8/21/2009 3:04:42 PM/ ACE+JFT / MVC2884


            //for efficiency.. don't arrange it indexes are same..
            if (drop_index == drag_index)
                return true;

            CPQMView* p_parent = (CPQMView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();
            p_parent->MoveProtocolToIndex(drag_index, drop_index);

        }
        break ;

        default : {

            ASSERT(FALSE);
            return false;
        }
    }

    g_MainView->GetAcquisition()->CalcSARControlAndPutVFForAll();

    return true ;
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::InvalidateItemForColumn(
    const int row_index, const int column_index
)
{

    CRect cellrect = GetCellRect(row_index, column_index);
    cellrect.InflateRect(1, 1);
    InvalidateRect(cellrect);
}


//************************************Method Header**************************************
//Method Name:  EnableHighlighting
//Author:  PATNI/RD
//Purpose: Highlight list control
//**************************************************************************************/
void CPQMListCtrl::EnableHighlighting(
    const int row,
    const bool bHighlight
)
{
    ListView_SetItemState(m_hWnd, row, bHighlight ? LVIS_SELECTED : 0, LVIS_SELECTED);

    //Added by Hemant On 11/19/2008 10:26:24 AM
    if (bHighlight) {
        SetSelectionMark(row);
    }
}



//+Patni-ARD/2010Jan27/Added/DeFT# MVC3817
//************************************Method Header************************************
// Method Name  : CreateDragImageEx
// Author       : PATNI/ ARD
// Purpose      : Create the Image with text on begining of dragging the sequence
//***********************************************************************************
CImageList* CPQMListCtrl::CreateDragImageEx(LPPOINT lpPoint, const CString& f_str_scanID, const int f_acq_status)
{
    CRect rectSingle;
    CRect rectComplete(0, 0, 0, 0);
    int	nIndex	= -1;
    BOOL bFirst	= TRUE;

    // Determine the size of the drag image.
    POSITION pos = GetFirstSelectedItemPosition();

    while (pos) {
        nIndex = GetNextSelectedItem(pos);
        GetItemRect(nIndex, rectSingle, LVIR_BOUNDS);

        if (bFirst) {
            // Initialize the CompleteRect
            GetItemRect(nIndex, rectComplete, LVIR_BOUNDS);
            bFirst = FALSE;
        }

        rectComplete.UnionRect(rectComplete, rectSingle);
    }

    // Create bitmap in memory DC
    CClientDC dcClient(this);
    CDC dcMem;
    CBitmap Bitmap;

    if (!dcMem.CreateCompatibleDC(&dcClient)) {
        return NULL;
    }

    if (!Bitmap.CreateCompatibleBitmap(&dcClient, rectComplete.Width(), rectComplete.Height())) {
        return NULL;
    }

    CBitmap* pOldMemDCBitmap = dcMem.SelectObject(&Bitmap);

    // Paint each DragImage in the DC.
    CImageList* pSingleImageList = NULL;
    CPoint pt;

    pos = GetFirstSelectedItemPosition();

    CFont l_font;

    CWnd* wnd = g_MainView->GetDlgItem(IDC_PQM_SEQ_LISTCTRL);

    if (wnd == NULL) {
        CPQMLogMgr* l_pqm_mgr =  CPQMLogMgr::GetInstance();
        l_pqm_mgr->WriteEventLog(IDS_ERR_VIEW_PTR,
                                 ERROR_LOG_WARNING1, _T("PQMListCtrl.cpp"), __LINE__); // Patni-KSS/ADDED/2010May19/IR-82

        LOGFONT lf;
        memset(&lf, 0, sizeof(LOGFONT));
        lf.lfHeight = 22;
        l_font.CreateFontIndirect(&lf);

    } else {
        CFont* l_p_log_font = wnd->GetFont();

        if (l_p_log_font) {

            LOGFONT l_log_font;
            l_p_log_font->GetLogFont(&l_log_font);
            l_font.CreateFontIndirect(&l_log_font);
        }
    }

    CFont* l_pFont = dcMem.SelectObject(&l_font);

    while (pos) {
        nIndex = GetNextSelectedItem(pos);
        GetItemRect(nIndex, rectSingle, LVIR_BOUNDS);

        pSingleImageList = CreateDragImage(nIndex, &pt);

        if (pSingleImageList) {
            // Make sure width takes into account not using LVS_EX_FULLROWSELECT style.
            IMAGEINFO ImageInfo;
            pSingleImageList->GetImageInfo(0, &ImageInfo);
            rectSingle.right = rectSingle.left + (ImageInfo.rcImage.right - ImageInfo.rcImage.left);

            pSingleImageList->DrawIndirect(
                &dcMem,
                0,
                CPoint(rectSingle.left - rectComplete.left,
                       rectSingle.top - rectComplete.top),
                rectSingle.Size(),
                CPoint(0, 0));

            DEL_PTR(pSingleImageList)
        }
    }

    dcMem.FillSolidRect(0, 0, rectComplete.Width(), rectComplete.Height(), RGB(37, 99, 165));

    CRect tmp_rect(0, 0, rectComplete.Width(), rectComplete.Height());
    const COLORREF old_color = dcMem.SetTextColor(RGB(255, 255, 255));
    const int old_mode = dcMem.SetBkMode(TRANSPARENT);
    dcMem.DrawText(f_str_scanID, tmp_rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);

    if (f_acq_status != PR_ACQ_STATUS_WAIT) {
        CRect tmp_rect2(0, 0, rectComplete.Width() / 2, rectComplete.Height());
        dcMem.DrawText(_T("  +"), tmp_rect2, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    }

    dcMem.SetBkMode(old_mode);
    dcMem.SetTextColor(old_color);

    dcMem.SelectObject(l_pFont);
    l_pFont->DeleteObject();

    dcMem.SelectObject(pOldMemDCBitmap);

    // Create the imagelist	with the merged drag images.
    CImageList* pCompleteImageList = new CImageList;

    // Here green is used as mask color.
    pCompleteImageList->Create(rectComplete.Width(), rectComplete.Height(), ILC_COLOR24 | ILC_MASK, 0, 1);

    pCompleteImageList->Add(&Bitmap, RGB(0, 0, 0));

    Bitmap.DeleteObject();

    // As an optional service:
    // Find the offset of the current mouse cursor to the imagelist
    // this we can use in BeginDrag().
    if (lpPoint) {
        CPoint ptCursor;
        GetCursorPos(&ptCursor);
        ScreenToClient(&ptCursor);

        lpPoint->x = 122;//ptCursor.x - rectComplete.left;
        lpPoint->y = ptCursor.y - rectComplete.top ;
    }

    ScreenToClient(lpPoint);
    return pCompleteImageList;
}
//-Patni-ARD/2010Jan27/Added/DeFT# MVC3817


//************************************Method Header************************************
// Method Name  : DrawModeCellData
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::DrawModeCellData(
    CDC* dc, const int row_index
)
{

    CRect option_rect ;
    int bitmap_id = 0;	//Patni-PP/2009Jun01/Modified/SU14 CPP test initialization corrections

    for (int option_count = 0; option_count < TOTAL_MODE_FALGS ; option_count ++) {

        option_rect = GetOptionRectForModeOptionIndex(row_index, option_count);

        if (!option_rect.IsRectEmpty()) {
            bitmap_id = GetBitmapResourceIDForModeOption(row_index, option_count);

            if (bitmap_id <= 0)
                continue ;

            //+Patni-Hemant/2009Nov15/Modified/IR#90/Queue List
            //DrawOptionWith(dc, option_rect, bitmap_id);
            DrawIconForRect(dc, bitmap_id, option_rect);
            //-Patni-Hemant/2009Nov15/Modified/IR#90/Queue List
        }
    }
}


//************************************Method Header************************************
// Method Name  : DrawStatusCellData
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::DrawStatusCellData(
    CDC* dc,
    const int row_index
)
{
    const int total_statuscol = 1;
    int bitmap_resourceid = -1;
    CRect l_icon_rect(0, 0, 0, 0);

    for (int column_index = 0; column_index < total_statuscol ; column_index++) {

        bitmap_resourceid = GetBitmapResourceIDForStatus(row_index);

        if (bitmap_resourceid <= 0) {
            return ;
        }

        //+Patni-Hemant/2009Nov15/Modified/IR#90/Queue List
        //DrawImageWith(dc, bitmap_resourceid, CPoint(m_status_column_index, row_index), CRect(0,0,0,0), false, true);
        l_icon_rect = GetImageRectForResourceID(bitmap_resourceid);

        if (!l_icon_rect.IsRectEmpty()) {
            DrawIcon(dc, row_index, m_status_column_index, bitmap_resourceid, l_icon_rect);
        }

        //-Patni-Hemant/2009Nov15/Modified/IR#90/Queue List
    }
}



//************************************Method Header************************************
// Method Name  : DrawStatusCellData
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::DrawSARCellData(
    CDC* dc,
    const int row_index
)
{
    const int bitmap_resourceid = GetBitmapResourceIDForSARRegion(row_index);

    if (bitmap_resourceid <= 0) {
        return ;
    }

    DrawImageWith(dc, bitmap_resourceid, CPoint(m_sar_column_index, row_index));
}


//+Patni-Hemant/2009Nov15/Added/IR#90/Queue List
//************************************Method Header************************************
// Method Name  : DrawStatusCellData
// Author       : PATNI/ HEMANT
// Purpose      : This function draws the icon on f_dc device context using given parameters
//***********************************************************************************
void CPQMListCtrl::DrawIcon(
    CDC* f_dc,
    const int f_row_index,
    const int f_col_index,
    const UINT f_icon_id,
    CRect f_icon_rect
)
{

    if (IsValidSequenceIndex(f_row_index)) {

        CenterRectForRowIndex(f_row_index, f_col_index, f_icon_rect);
        DrawIconForRect(f_dc, f_icon_id, f_icon_rect);
    }
}
//-Patni-Hemant/2009Nov15/Added/IR#90/Queue List


//+Patni-Hemant/2009Nov15/Added/IR#90/Queue List
//************************************Method Header************************************
// Method Name  : DrawIconForRect
// Author       : PATNI/ HEMANT
// Purpose      : This function draws the icon using f_icon_id resource icon id and
//                other given parameters with f_dc device context.
//***********************************************************************************
void CPQMListCtrl::DrawIconForRect(
    CDC* f_dc,
    const UINT f_icon_id,
    const CRect& f_icon_rect
)const
{
    HICON hIcon = AfxGetApp()->LoadIcon(f_icon_id);
    DrawIconEx(f_dc->m_hDC, f_icon_rect.left, f_icon_rect.top, hIcon, f_icon_rect.Width(), f_icon_rect.Height(), 0,
               NULL, DI_NORMAL);
}
//-Patni-Hemant/2009Nov15/Added/IR#90/Queue List


//************************************Method Header************************************
// Method Name  : DrawStatusCellData
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::DrawPLANCellData(
    CDC* dc,
    const int row_index,
    const int f_col_index
)
{
    CString l_plantxtstr = _T("");
    const int bitmap_resourceid = GetBitmapResourceIDForPLANNEDProtocol(row_index, l_plantxtstr);

    if (bitmap_resourceid <= 0 || (!l_plantxtstr.CompareNoCase(_T("")) &&
                                   (IDI_PLAN_CONE_NON_WAIT_PROTOCOL_ICON == bitmap_resourceid || IDI_PLAN_CONE_WAIT_PROTOCOL_ICON == bitmap_resourceid))) {

        return ;
    }

    //+Patni-Hemant/2009Nov15/Modified/IR#90/Queue List
    //DrawImageWith(dc, bitmap_resourceid, CPoint(m_plan_column_index, row_index));
    CRect l_icon_rect = GetImageRectForResourceID(bitmap_resourceid);

    if (!l_icon_rect.IsRectEmpty()) {
        DrawIcon(dc, row_index, m_plan_column_index, bitmap_resourceid, l_icon_rect);

        CRect cellrect = GetCellRect(row_index, f_col_index);

        if (IDI_PLAN_CONE_NON_WAIT_PROTOCOL_ICON == bitmap_resourceid) {
            CFont* old_font = dc->SelectObject(&m_plan_icon_font);
            dc->SetTextColor(RGB(58, 63, 73));
            int old_mode = dc->SetBkMode(TRANSPARENT);
            dc->DrawText(l_plantxtstr, cellrect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            dc->SelectObject(old_font);
            dc->SetBkMode(old_mode);

        } else if (IDI_PLAN_CONE_WAIT_PROTOCOL_ICON == bitmap_resourceid) {
            CFont* old_font = dc->SelectObject(&m_plan_icon_font);
            dc->SetTextColor(RGB(204, 204, 204));
            int old_mode = dc->SetBkMode(TRANSPARENT);
            dc->DrawText(l_plantxtstr, cellrect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            dc->SelectObject(old_font);
            dc->SetBkMode(old_mode);
        }
    }

    //-Patni-Hemant/2009Nov15/Modified/IR#90/Queue List
}



//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::DrawImageWith(
    CDC* dc,
    const UINT bitmap_resourceid,
    const CPoint& cell_index,
    CRect rect_tocenter /*= CRect(0,0,0,0)*/,
    const bool horizontal_center /*= true*/,
    const bool vertical_center /*= true*/
)
{

    CRect cellrect = GetCellRect(cell_index.y, cell_index.x);

    CBitmap bitmap ;
    BITMAP bitmap_object;
    bitmap.LoadBitmap(bitmap_resourceid);
    bitmap.GetBitmap(&bitmap_object);

    if (rect_tocenter.IsRectEmpty()) {

        rect_tocenter = cellrect;

        rect_tocenter.right = rect_tocenter.left + bitmap_object.bmWidth;
        rect_tocenter.bottom = rect_tocenter.top + bitmap_object.bmHeight;
        CenterRectForRowIndex(cell_index.y, cell_index.x, rect_tocenter, horizontal_center, vertical_center);
    }

    if (rect_tocenter.Width() > cellrect.Width()) {

        bitmap.DeleteObject();
        return ;
    }

    if (DRAW_TRANSPARENT) {

        bitmap.DeleteObject();
        CPQMImageUtility::DrawTrans(dc->m_hDC, rect_tocenter.left, rect_tocenter.top, bitmap_resourceid, rect_tocenter, RGB(255, 255, 255));

    } else {

        CDC memdc ;
        memdc.CreateCompatibleDC(dc);
        HBITMAP old_bitmap = (HBITMAP)::SelectObject(memdc, (HBITMAP)bitmap);

        dc->StretchBlt(cellrect.left, cellrect.top, bitmap_object.bmWidth , bitmap_object.bmHeight , &memdc , 0, 0, bitmap_object.bmWidth , bitmap_object.bmHeight , SRCCOPY);

        ::SelectObject(memdc, old_bitmap);
        ::DeleteDC(memdc.m_hDC);

        bitmap.DeleteObject();

        //Patni-Hemant/2009Jan15/Added/GDI Leak Fix
        ::DeleteDC(memdc.m_hDC);
    }
}


//************************************Method Header************************************
// Method Name  : CenterRectForRowIndex
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::CenterRectForRowIndex(
    const int row_index,
    const int col_index,
    CRect& rect_tocenter,
    const bool forwidth /*= true*/,
    const bool forheight /*= true*/
)
{

    if ((row_index >= 0) && (row_index < GetItemCount())) {

        CRect rect = GetCellRect(row_index , col_index) ;

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
bool CPQMListCtrl::CanSetTextItemForPameterString(
    const CString& parameter_string
)const
{

    if ((!parameter_string.Compare(_T("MODE"))) ||
        (!parameter_string.Compare(_T("SAR"))) ||
        (!parameter_string.Compare(_T("PROTOCOLNO"))) ||
        (!parameter_string.Compare(_T("ACQSTATUS2"))) ||
        (!parameter_string.Compare(_T("SCANTIME"))) ||
        (!parameter_string.Compare(_T("PLAN"))) ||
        (!parameter_string.Compare(_T("WSEEDITING"))) ||
        (!parameter_string.Compare(_T("DELAYTIME")))
       ) {

        return false;
    }

    return true ;
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::OnSarCellClicked(
    const int row_index
)const
{
    if (!IsValidSequenceIndex(row_index)) {

        ASSERT(FALSE);
        return ;
    }

    CPQMView* p_parent = (CPQMView*)GetParent();
    ASSERT(p_parent != NULL);
    //+Patni-SS/2009Apr07/Modi/PSP1#12
    //p_parent->m_pqmchildthread.PostMessageToThread(ON_ANATOMYAPPLY,(WPARAM)row_index,NULL);
    p_parent->OnClickButtonSarRegion(row_index);
    //-Patni-SS/2009Apr07/Modi/PSP1#12
}



//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
int CPQMListCtrl::PerformActionOnMouseClickAt(
    const CPoint& point
)
{
    CPoint cell_number = GetCellNoFromPoint(point);

    if ((unsigned int)cell_number.x == m_mode_column_index) {

        return (PerformActionOnModeCellClick(point , cell_number)) ;
    }

    return -1 ;
}


//************************************Method Header************************************
// Method Name  : PerformActionOnRButtonClickAt
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::PerformActionOnRButtonClickAt(
    const CPoint& point
)
{
    PerformActionOnRButtonClickForModeOptions(point, GetCellNoFromPoint(point));
}



//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::PerformActionOnRButtonClickForModeOptions(
    const CPoint& point_in_client,
    const CPoint& cell_number
)
{

    if (g_MainView->IsWaitCursor()) {
        return;
    }

    if ((cell_number.y < 0) || (cell_number.y >= GetItemCount()))
        return ;

    CSequence* sequence_struct = GetSequenceStructForIndex(cell_number.y);

    if (!sequence_struct) {
        ASSERT(FALSE);
        return ;
    }

    CPoint point_at_which_to_show_the_menu = point_in_client;


    int* selected_items = NULL ;
    bool iswait_selected = false ;
    bool iscurrent_selected = false ;
    bool isdone_selected = false ;
    bool isfail_selected = false ;
    bool isfirstwait_selected = false ;

    const int count = GetselectedItems(&selected_items, &iswait_selected, &iscurrent_selected, &isdone_selected, &isfail_selected, &isfirstwait_selected);

    //+Patni-ARD/2010Apr29/Modified/JaFT# MCM0229-00178_CDR_06 + code review
    DEL_PTR_ARY(selected_items);
    //-Patni-ARD/2010Apr29/Modified/JaFT# MCM0229-00178_CDR_06 + code review

    UINT menu_to_show = 0u;//Patni-DKH/2009Aug24/Modified/cpp test corrections
    CRect option_rect;

    if (menu_to_show == 0) { //Patni-AD/2009Aug27/Modified/SU15 //cpp test corrections

        option_rect = GetOptionRectForModeOptionIndex(cell_number.y, AUTO_VOICE_OPTION);

        if (option_rect.PtInRect(point_in_client)) {
            menu_to_show = IDR_MENU_AUTO_VOICE;
            point_at_which_to_show_the_menu = CPoint(option_rect.left , option_rect.bottom);
        }
    }

    if (menu_to_show == 0) { //Patni-AD/2009Aug27/Modified/SU15 //cpp test corrections
        option_rect = GetOptionRectForModeOptionIndex(cell_number.y, CONTRAST_AGENT_OPTION);

        if (option_rect.PtInRect(point_in_client)) {

            //+Patni-Abhishek/2010Dec14/Modified/IR-180
            ///now check weather , this optionis on..if not return..
            //if (!pr_mode.m_contrast)
            //    return ;
            //-Patni-Abhishek/2010Dec14/Modified/IR-180

            //you can not change the contrast agent for followings..
            //+Patni-PJS/2010Sep1/Added/TMSC-REDMINE-599
            CPQMView* p_parent = (CPQMView*)GetParent();

            //+Patni-Abhishek/2011Feb09/Modified/IR-180
            if ((-1 != p_parent->GetScanIndex()) && (PR_ACQ_STATUS_WAIT != sequence_struct->GetAcqStatus()) || (!p_parent->CanPerformModeOptionOperations())) {
                return ;
            }

            //-Patni-Abhishek/2011Feb09/Modified/IR-180
            //-Patni-PJS/2010Sep1/Added/TMSC-REDMINE-599

            menu_to_show = IDR_CONTRAST_AGENT_MENU;
        }
    }

    //+Patni-RUP/2010Jul19/Added/TMSC-REDMINE-466
    if (0 == menu_to_show) {
        option_rect = GetOptionRectForModeOptionIndex(cell_number.y, MOVE_COUCH_OPTION);

        if (option_rect.PtInRect(point_in_client)) {

            CPQMView* p_parent = (CPQMView*)GetParent();

            if ((K_IMAGING != p_parent->GetPQMGlobal().GetPQMMode())) {
                return;
            }

            menu_to_show = IDR_MENU_COUCH;
        }
    }

    switch (menu_to_show) {

        case IDR_MENU_AUTO_VOICE:
        case IDR_MENU_COUCH:

            if (PR_ACQ_STATUS_WAIT != sequence_struct->GetAcqStatus()) {
                return;
            }

            if (IsRowSelected(m_hWnd, cell_number.y)) {
                if ((iscurrent_selected || isdone_selected || isfail_selected)) {
                    return;
                }
            }
    }


    if (0 == menu_to_show && ((unsigned int)cell_number.x == m_plan_column_index)) {
        if (CanShowMasterSlavePlanMenu(cell_number.y)) {
            menu_to_show = IDR_MENU_PLAN_MASTER_SLAVE;

        } else {
            return;
        }
    }


    if ((menu_to_show == 0)  && ((unsigned int)cell_number.x != m_mode_column_index)
        && ((unsigned int)cell_number.x != m_plan_column_index))
        menu_to_show = IDR_PROTOCOL_MENU ;

    CPQMView* p_parent = (CPQMView*)GetParent();

    if (p_parent->IsAMBInvoked() && (menu_to_show != IDR_PROTOCOL_MENU)) {
        return ;
    }

    if (menu_to_show != 0) { //Patni-AD/2009Aug27/Modified/SU15 //cpp test corrections

        ClientToScreen(&point_at_which_to_show_the_menu);

        PopupMenuEx context_menu;
        context_menu.LoadMenu(menu_to_show);

        PopupMenuEx* context_sub_menu = NULL;	//Patni-PP/2009Jun01/Modified/SU14 CPP test initialization corrections
        context_sub_menu = (PopupMenuEx*)context_menu.GetSubMenu(0);


        LPCTSTR font_name = _T("Trebuchet MS") ;
        const int font_size = 10;

        context_menu.SetFont(font_name, font_size);

        switch (menu_to_show) {

            case IDR_PROTOCOL_MENU:
                UpdateProtocolContextMenu(context_sub_menu, cell_number.y);

                break ;

            case IDR_MENU_AUTO_VOICE :
                UpdateAutoVoiceContextMenu(context_sub_menu, cell_number.y);

                break;

                //SM3_mangesh_Start
            case IDR_CONTRAST_AGENT_MENU :
                UpdateContrastAgentContextMenu(context_sub_menu, cell_number.y);
                ClientToScreen(&option_rect);

                point_at_which_to_show_the_menu.x = option_rect.left;
                point_at_which_to_show_the_menu.y = option_rect.bottom;
                break;

                //SM3_mangesh_End
                //+Patni-RUP/2010Jul22/Added/TMSC-REDMINE-466
            case IDR_MENU_COUCH:
                UpdateMoveCouchContextMenu(context_sub_menu, cell_number.y);
                ClientToScreen(&option_rect);
                point_at_which_to_show_the_menu.x = option_rect.left;
                point_at_which_to_show_the_menu.y = option_rect.bottom;
                break;

                //-Patni-RUP/2010Jul22/Added/TMSC-REDMINE-466
            case IDR_MENU_PLAN_MASTER_SLAVE:
                UpdatePlanMasterSlaveContextMenu(context_sub_menu, cell_number.y);
                break;

        }//end switch

        m_rightclick_cell = cell_number;

        if (context_sub_menu) {

            CPQMView* p_parent = (CPQMView*)GetParent();
            ASSERT(p_parent != NULL);

            context_menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point_at_which_to_show_the_menu.x ,
                                        point_at_which_to_show_the_menu.y, /*this*/p_parent, 0, 0);
            //+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
            //Patni-Hemant/2011Apr20/Modified/TMSC-REDMINE-1671
            p_parent->DetachSFTToolmenu();
            //-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
        }
    }
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::PerformActionOnDblClickAt(
    const CPoint& point
)
{
    CPoint cell_number = GetCellNoFromPoint(point);

    if (IsValidSequenceIndex(cell_number.y)) {

        if (((unsigned int)cell_number.x >= m_status_column_index) && ((unsigned int)cell_number.x <= m_plan_column_index)) {

            CPQMView* p_parent = (CPQMView*)GetParent();

            if (p_parent) {
                p_parent->OnDblclkListParameter(cell_number.y);
            }
        }
    }
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
int CPQMListCtrl::PerformActionOnModeCellClick(
    const CPoint& point_in_client,
    const CPoint& cell_number	//to get the data and set/ change it..
)
{
    CRect option_rect;

    for (int option_count = 0; option_count < TOTAL_MODE_FALGS ; option_count ++) {
        option_rect = GetOptionRectForModeOptionIndex(cell_number.y, option_count);

        if (!option_rect.IsRectEmpty()) {

            if (option_rect.PtInRect(point_in_client)) {

                return option_count;
            }
        }
    }

    return -1 ;
}


//************************************Method Header**********************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::UpdateProtocolContextMenu(
    PopupMenuEx* menu,
    const int row_index
)
{
    LPCTSTR FUNC_NAME = _T("CPQMListCtrl::UpdateProtocolContextMenu");

    CPQMView* p_parent = (CPQMView*)GetParent();

    //+Patni-HEMANT/ADDED On 8/24/2009 12:18:09 PM/ Code Review
    if (!(menu && p_parent)) {
        return ;
    }

    //+Patni-PJS/2011Feb24/Added/TMSC_REDMINE-1480
    /*if (S_OK != p_parent->GetAcquisition()->GetMRSLicense()) {
        menu->RemoveMenu(ID_PROTOCOL_MRS_RETRY, MF_BYCOMMAND);

    } else {
        int* selected_seq = NULL;
        bool is_valid_mrs_seq = false;
        int count = GetselectedItems(&selected_seq);

        if (selected_seq) {
            CSequence* l_seq = (CSequence*)GetItemData(selected_seq[0]);

            if (l_seq) {
                int l_acq_order = _ttoi(l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER")));

                if ((PR_ACQ_STATUS_SUCCESS == l_seq->GetAcqStatus()) && (count == 1) && (S_OK == p_parent->GetAcquisition()->IsSelectedSeqMRS(l_acq_order))
                    && (!p_parent->IsMRSProcessToolStarted())) {
                    is_valid_mrs_seq = true;
                }
            }
        }

        if (!is_valid_mrs_seq  || (K_IMAGING != p_parent->GetPQMGlobal().GetPQMMode())) {
            menu->EnableMenuItem(ID_PROTOCOL_MRS_RETRY, MF_BYCOMMAND | MF_GRAYED);
        }

        int l_scan_index = p_parent->GetScanIndex();

        if (l_scan_index >= 0) {
            CSequence* l_seq = (CSequence*) GetItemData(l_scan_index);

            if (l_seq) {
                int l_acq_order = _ttoi(l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER")));

                if (S_OK == p_parent->GetAcquisition()->IsSelectedSeqMRS(l_acq_order)) {
                    menu->EnableMenuItem(ID_PROTOCOL_MRS_RETRY, MF_BYCOMMAND | MF_GRAYED);
                }
            }
        }
    }*/

    //-Patni-PJS/2011Feb24/Added/TMSC_REDMINE-1480
    //To remove the ID_CALA_CARDIAC_LOCATOR menu when Cardiac license is not present
    int* selected_seq = NULL;
    int l_seq_count = GetselectedItems(&selected_seq);

    if (selected_seq) {
        if (l_seq_count > 1) {
            menu->EnableMenuItem(ID_CARDIAC_LOCATOR, MF_BYCOMMAND | MF_GRAYED);
            menu->EnableMenuItem(ID_NEURO_LOCATOR, MF_BYCOMMAND | MF_GRAYED);


        } else {
            CSequence* l_seq = (CSequence*)GetItemData(selected_seq[0]);

            if (l_seq) {
                int l_acq_order = _ttoi(l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetString(_T("ACQORDER")));

                if ((K_IMAGING != p_parent->GetPQMGlobal().GetPQMMode()) ||
                    (!(S_OK == p_parent->GetAcquisition()->IsValidSequenceToInvokeAutoLocator(l_acq_order, AUTOLOCATOR_CARDIAC)) && (l_seq_count == 1))) {

                    menu->EnableMenuItem(ID_CARDIAC_LOCATOR, MF_BYCOMMAND | MF_GRAYED);
                }

                if ((K_IMAGING != p_parent->GetPQMGlobal().GetPQMMode()) ||
                    (!(S_OK == p_parent->GetAcquisition()->IsValidSequenceToInvokeAutoLocator(l_acq_order, AUTOLOCATOR_NEURO)) && (l_seq_count == 1))) {

                    menu->EnableMenuItem(ID_NEURO_LOCATOR, MF_BYCOMMAND | MF_GRAYED);
                }
            }
        }
    }

    HRESULT l_cala_license = p_parent->GetAcquisition()->GetAutoLocatorLicense(AUTOLOCATOR_CARDIAC);
    HRESULT l_hala_license = p_parent->GetAcquisition()->GetAutoLocatorLicense(AUTOLOCATOR_NEURO);

    //Remove the Easy tech menu if license not present
    if ((S_OK != l_cala_license) && (S_OK != l_hala_license)) {

        CString l_chOutput = CPQMUtility::GetMultiLingualString(_T("ID_LOCATOR_CONCIERGE"));
        CString l_tar_str = _T("");
        CString l_lang_str = _T("");

        for (int index = 0; index < (int)menu->GetMenuItemCount() ; index ++) {

            menu->GetMenuString(index, l_tar_str, MF_BYPOSITION);
            l_lang_str = CPQMUtility::GetMultiLingualString(l_tar_str);

            if (l_lang_str.CompareNoCase(l_chOutput) == 0) {
                menu->RemoveMenu(index, MF_BYPOSITION);
            }
        }

    } else if (S_OK != l_cala_license) { //Remove individual menu items.
        menu->RemoveMenu(ID_CARDIAC_LOCATOR, MF_BYCOMMAND);

    } else if (S_OK != l_hala_license) {
        menu->RemoveMenu(ID_NEURO_LOCATOR, MF_BYCOMMAND);
    }

    //+Patni-ARD/2010Jan29/Added/DeFT# MVC4446
    if ((p_parent->GetIsLocatorOpen()) && (p_parent->GetWaitCursor() == false))
        menu->EnableMenuItem(ID_PROTOCOL_REFERROI, MF_BYCOMMAND);

    else
        menu->EnableMenuItem(ID_PROTOCOL_REFERROI, MF_BYCOMMAND | MF_GRAYED);

    //-Patni-ARD/2010Jan29/Added/DeFT# MVC4446

    //+Patni-PJS/2010Jan7/Added/MVC006659
    if (p_parent->GetPQMGlobal().GetPQMMode() != K_IMAGING) {
        //+Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image
        //menu->EnableMenuItem(ID_PROTOCOLMENU_MOVECOUCH, MF_BYCOMMAND | MF_GRAYED);
        //+Patni-RUP/2010Jul23/Commented/TMSC-REDMINE-466
        //menu->EnableMenuItem(ID_PROTOCOLMENU_CENTER_OF_THE_PLAN, MF_BYCOMMAND | MF_GRAYED);
        //menu->EnableMenuItem(ID_PROTOCOLMENU_NO_MOVE, MF_BYCOMMAND | MF_GRAYED);
        //menu->EnableMenuItem(ID_PROTOCOLMENU_POSITION_OF_REFERENCE_IMAGE, MF_BYCOMMAND | MF_GRAYED);
        //-Patni-RUP/2010Jul23/Commented/TMSC-REDMINE-466
        //-Patni-Manish/2010Feb11/Added/MCM0229-00122/Parent Image
    }

    //-Patni-PJS/2010Jan7/Added/MVC006659
    //-Patni-HEMANT/ADDED On 8/24/2009 12:18:09 PM/ Code Review

    if (GetSelectedCount() != 1) {

        menu->EnableMenuItem(ID_MENUITEM_INFO_SEQ, MF_BYCOMMAND | MF_GRAYED);
        menu->EnableMenuItem(ID_COMMENT, MF_BYCOMMAND | MF_GRAYED);
    }

    const bool l_vf_file_locked = !p_parent->CanPerformEditingForVFFile();

    int* selected_items = NULL ;

    bool iswait_selected = false ;

    bool iscurrent_selected = false ;

    bool isdone_selected = false ;

    bool isfail_selected = false ;

    bool isfirstwait_selected = false ;

    bool iswse_eidted_protocol_selected = false;

    //+Patni-HEMANT/ADDED On 8/12/2009 1:44:12 PM/ MVC#4232
    bool iscoil_selected_for_all = true;

    //+Patni-HEMANT/ADDED On 8/12/2009 1:44:12 PM/ MVC#4232
    int count = GetselectedItems(&selected_items, &iswait_selected, &iscurrent_selected, &isdone_selected, &isfail_selected, &isfirstwait_selected , &iswse_eidted_protocol_selected, &iscoil_selected_for_all);

    //+Patni-Dhanesh/2009Aug20/Modified/cpp test corrections
    if (NULL == selected_items) {
        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("selected_items pointer is NULL"));
        return;
    }

    //-Patni-Dhanesh/2009Aug20/Modified/cpp test corrections

    //+Patni-ARD/2009Jun15/Modified/JFT# Move couch difference issue
    const int first_selected_item = selected_items [0] ;		//we know atleast one item is selcted and menu will be there..
    CSequence* l_seq = (CSequence*) GetItemData(first_selected_item);
    //-Patni-ARD/2009Jun15/Modified/JFT# Move couch difference issue

    //+Patni-HEMANT/ADDED On 8/12/2009 1:44:12 PM/ MVC#4232
    const int holdvalue = l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetInt(_T("HOLD"));


    if ((holdvalue & VFC_HOLD_COVERAGE) == VFC_HOLD_COVERAGE) { //Breadth
        menu->CheckMenuItem(ID_PROTOCOLMENU_BREATHHOLD, MF_BYCOMMAND | MF_CHECKED);
    }

    if (l_vf_file_locked || (iscurrent_selected || isdone_selected || isfail_selected)) {

        //breath hold and move couch will remain disable when wse editing is happening,
        //or wait cursor is enable or any non wait protocol is selected...
        menu->EnableMenuItem(ID_PROTOCOLMENU_BREATHHOLD, MF_BYCOMMAND | MF_GRAYED);
    }

    //-Patni-HEMANT/ADDED On 8/12/2009 1:44:12 PM/ MVC#4232

    if (p_parent->GetMulitScanDynamic_License()) { //Remine-2187
        if (!iswait_selected || 1 != count) {
            menu->EnableMenuItem(ID_PROTOCOLMENU_EDITDELAY, MF_BYCOMMAND | MF_GRAYED);

        } else {
            menu->EnableMenuItem(ID_PROTOCOLMENU_EDITDELAY, MF_BYCOMMAND);
        }

    } else {
        menu->RemoveMenu(ID_PROTOCOLMENU_EDITDELAY, MF_BYCOMMAND);
    }


    //+Patni-HEMANT/ADDED On 9/23/2009 5:31:29 PM/ IDS Internal
    //seq info disable for current protocol
    if (iscurrent_selected) {
        menu->EnableMenuItem(ID_MENUITEM_INFO_SEQ, MF_BYCOMMAND | MF_GRAYED);
    }

    //-Patni-HEMANT/ADDED On 9/23/2009 5:31:29 PM/ IDS Internal

    if (!CheckAMBContextMenuStatus() || g_MainView->IsAMBInvoked()) {
        menu->EnableMenuItem(ID_PROTOCOLMENU_AMB, MF_BYCOMMAND | MF_GRAYED);
    }


    if (l_vf_file_locked || (first_selected_item == 0) || (iscurrent_selected || isdone_selected || isfail_selected || isfirstwait_selected || iswse_eidted_protocol_selected)) {

        //+Patni-PJS/2009Jun25/Modified/JFT#351
        //+Patni-ARD/2009Oct13/Modified/Defect Fix# MSA0248-00034


        if ((holdvalue & VFC_HOLD_PRE_SCAN) != VFC_HOLD_PRE_SCAN) {
            menu->CheckMenuItem(ID_PROTOCOL_AUTO_SCAN, MF_BYCOMMAND | MF_CHECKED);
        }

        //-Patni-ARD/2009Oct13/Modified/Defect Fix# MSA0248-00034

        //-Patni-PJS/2009Jun25/Modified/JFT#351
        menu->EnableMenuItem(ID_PROTOCOL_AUTO_SCAN, MF_BYCOMMAND | MF_GRAYED);


        //+Patni-Sribanta/2010July02/Modified/IR-156_2
        //Patni-Manish/2009Jul17/Added/MVC002331/To disable the comment on WSE/Locator invoke
        //if (iscurrent_selected || iswse_eidted_protocol_selected) {
        //    menu->EnableMenuItem(ID_COMMENT, MF_BYCOMMAND | MF_GRAYED);
        //}
        //-Patni-Sribanta/2010July02/Modified/IR-156_2

        if (iscurrent_selected || isdone_selected || isfail_selected || l_vf_file_locked)		//gray the set all item as not wait is selected..
            menu->EnableMenuItem(ID_SET_ALL, MF_BYCOMMAND | MF_GRAYED);

    } else {

        //+Patni-ARD/2009Oct13/Modified/Defect Fix# MSA0248-00034
#ifndef PHASE1
        //check the auto scan according to first selected row..
        const int holdvalue = l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetInt(_T("HOLD"));

        if ((holdvalue & VFC_HOLD_PRE_SCAN) != VFC_HOLD_PRE_SCAN) {
            menu->CheckMenuItem(ID_PROTOCOL_AUTO_SCAN, MF_BYCOMMAND | MF_CHECKED);
        }

#else
        menu->EnableMenuItem(ID_PROTOCOL_AUTO_SCAN, MF_BYCOMMAND | MF_GRAYED);
#endif
        //-Patni-ARD/2009Oct13/Modified/Defect Fix# MSA0248-00034
    }

    //Patni-Hemant/2010May6/Added/JaFT/SM_PH3_CDR-CDS_CDR_12
    const bool l_can_update_protocol =
        iscurrent_selected || isdone_selected || isfail_selected || iswse_eidted_protocol_selected;

    //+Patni-Hemant/2009Nov13/Added/IR 85 Auto Transfer
    //
    //+Patni-Hemant+ARD/2009Nov04/Added/IR 85 Auto Transfer
    if (S_OK != p_parent->GetAcquisition()->GetEnableDisableAutoTransfer() || g_MainView->IsAMBInvoked()) {

        menu->EnableMenuItem(ID_PROTOCOLMENU_AUTOTRANSFER, MF_BYCOMMAND | MF_GRAYED);

    } else {

        //for current, done, fail and for protcol edited by WSE - auto transfer is to be disabled.
        //+Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_12
        //if (l_vf_file_locked || iscurrent_selected || isdone_selected || isfail_selected || iswse_eidted_protocol_selected) {
        if (l_vf_file_locked || l_can_update_protocol) {
            //-Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_12

            menu->EnableMenuItem(ID_PROTOCOLMENU_AUTOTRANSFER, MF_BYCOMMAND | MF_GRAYED);
        }
    }

    //Patni-Hemant/2009Nov18/Modified/IR#85/Defect Fix - Scenario given by Tanoue San
    const int l_auto_transfer_flag = ((l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetInt(_T("APOST"))) > 0) ? MF_CHECKED : MF_UNCHECKED;

    menu->CheckMenuItem(ID_PROTOCOLMENU_AUTOTRANSFER, MF_BYCOMMAND | l_auto_transfer_flag);
    //-Patni-Hemant+ARD/2009Nov04/Added/IR 85 Auto Transfer
    //
    //-Patni-Hemant/2009Nov13/Added/IR 85 Auto Transfer


    //Modified by Hemant On 11/13/2008 4:53:40 PM ,
    //we have have to check extra so ask parent..
    //act we should also disable this when using schedule mode aslo
    //if(p_parent->IsScanning()) {
    if (p_parent->ShouldDisableCoilGateInfo()) { //Patni-HAR/2010May06/Added/TMSC-REDMINE-198

        //disable the coil and gate info if scan is being performed..
        menu->EnableMenuItem(ID_MENUITEM_INFO_COIL, MF_BYCOMMAND | MF_GRAYED);
        menu->EnableMenuItem(ID_MENUITEM_INFO_GATE, MF_BYCOMMAND | MF_GRAYED);
    }

    //For Plan and duplicate
    //according to our understanding Plan and Duplicate should work when
    //WSE is open for index1 and plan and duplicate is clicked for index2.
    //(note : both index1 and index2 are valid)
    //That is, wse should be open and plan and duplicate should only be allowed
    //for item which is not being edited currently.
    //+Patni-Ajay/2010July28/TMSC-REDMINE-443
    //if (!p_parent->CanPlanAndDuplicateforIndex(row_index)) {

    //   menu->EnableMenuItem(ID_PROTOCOLMENU_PLANDUPLICATE, MF_BYCOMMAND | MF_GRAYED);
    //}
    //-Patni-Ajay/2010July28/TMSC-REDMINE-443

    //
    //+Patni-HAR/2010Feb16/Added/MSA0251-00166
    if (!l_seq->CanChangeModeOptionBreathHold() || g_MainView->IsAMBInvoked()) {
        menu->EnableMenuItem(ID_PROTOCOLMENU_BREATHHOLD, MF_BYCOMMAND | MF_GRAYED);
    }

    //-Patni-HAR/2010Feb16/Added/MSA0251-00166
    //+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement

    //+Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_12
    //if(iscurrent_selected || isdone_selected || isfail_selected || iswse_eidted_protocol_selected) {
    if (l_can_update_protocol || g_MainView->IsAMBInvoked()) {
        //-Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_12

        menu->EnableMenuItem(ID_ENABLE_AS_COMPASS, MF_BYCOMMAND | MF_GRAYED);
    }

    //if(l_seq->GetASCompassFlag()>0) {
    if (l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetInt(_T("AS-COMPASS")) > 0) {
        menu->CheckMenuItem(ID_ENABLE_AS_COMPASS, MF_BYCOMMAND | MF_CHECKED);
    }

    //-Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement

    //+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
    //Patni-Hemant/2010July21/Commented/IR-152_Part 3
    p_parent->AttachSFTMenu(menu);
    //Patni-Hemant/2010July21/Commented/IR-152_Part 3
    //-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu

    //Patni-Ajay/2010July28/TMSC-REDMINE-443
    p_parent->AttachPlanDuplicateMenu(row_index, menu);


    //+Patni/KSS/2010June25/ADDED/IR-156_2
    if (p_parent->EnableDisableDeleteButton() == FALSE || g_MainView->IsAMBInvoked()) {
        menu->EnableMenuItem(ID_PROTOCOLMENU_DELETE, MF_BYCOMMAND | MF_GRAYED);
    }

    if (g_MainView->IsAMBInvoked()) {

        menu->EnableMenuItem(ID_PROTOCOL_AUTO_SCAN, MF_BYCOMMAND | MF_GRAYED);
        menu->EnableMenuItem(ID_COMMENT, MF_BYCOMMAND | MF_GRAYED);
        menu->EnableMenuItem(ID_SET_ALL, MF_BYCOMMAND | MF_GRAYED);
        menu->EnableMenuItem(ID_MENUITEM_PLNDUP_CONFIG, MF_BYCOMMAND | MF_GRAYED);


    }

    if (g_MainView->IsAMB()) {
        menu->EnableMenuItem(ID_PROTOCOLMENU_APPEND, MF_BYCOMMAND | MF_GRAYED);
        menu->EnableMenuItem(ID_PROTOCOLMENU_DELETE, MF_BYCOMMAND | MF_GRAYED);
        menu->EnableMenuItem(ID_ENABLE_AS_COMPASS, MF_BYCOMMAND | MF_GRAYED);
        menu->EnableMenuItem(ID_LOCATOR_CONCIERGE, MF_BYCOMMAND | MF_GRAYED);
        menu->EnableMenuItem(ID_PROTOCOLMENU_APPEND, MF_BYCOMMAND | MF_GRAYED);
        menu->EnableMenuItem(ID_PROTOCOLMENU_EDITDELAY, MF_BYCOMMAND | MF_GRAYED);
        menu->EnableMenuItem(ID_CARDIAC_LOCATOR, MF_BYCOMMAND | MF_GRAYED);
        menu->EnableMenuItem(ID_NEURO_LOCATOR, MF_BYCOMMAND | MF_GRAYED);
    }

    
    //KJ/2014
    if(   
       (true != g_MainView->GetTiPrepController().CanTiPrepEnable())        
      ){
        menu->EnableMenuItem(ID_MENU_TIPREP, MF_BYCOMMAND | MF_GRAYED);
    }else{
        menu->EnableMenuItem(ID_MENU_TIPREP, MF_BYCOMMAND | MF_ENABLED);
    }

    if(
        (S_OK != ((g_MainView->GetTiPrepController()).GetTiPrepLicense(TIPREP_SCANNER)))
      ){
        menu->RemoveMenu(ID_MENU_TIPREP, MF_BYCOMMAND);
    }

    //-Patni/KSS/2010June25/ADDED/IR-156_2
    //+Patni-ARD/2010Apr29/Modified/JaFT# MCM0229-00178_CDR_06 + code review
    DEL_PTR_ARY(selected_items);
    //DEL_PTR(l_pdata);    //Patni-RUP/2010Jul23/Commented/TMSC-REDMINE-466
    //-Patni-PJS/2010Apr22/Added/JaFT# MVC007277
    //-Patni-ARD/2010Apr29/Modified/JaFT# MCM0229-00178_CDR_06 + code review
}




//************************************Method Header**********************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::UpdateAutoVoiceContextMenu(
    PopupMenuEx* menu,
    const int row_index
)
{
    LPCTSTR FUNC_NAME = _T("CPQMListCtrl::UpdateAutoVoiceContextMenu");

    CSequence* sequence_struct = GetSequenceStructForIndex(row_index);

    //+Patni-Dhanesh/2009Aug20/Modified/cpp test corrections
    if (NULL == sequence_struct) {
        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("sequence_struct pointer is NULL"));
        return;
    }

    //-Patni-Dhanesh/2009Aug20/Modified/cpp test corrections

    const int selected_auto_voice_id = GetAutoVoiceOptionForID(sequence_struct->GetAutoVoiceOption());
    //Patni-PJS/2010Jan19/Added/Phase 3 MVC004363
    const int l_prescan_auto_voice_id = sequence_struct->GetPrescanAutoVoiceOptn();

    if (selected_auto_voice_id != -1) {

        menu->CheckMenuItem(selected_auto_voice_id, MF_BYCOMMAND | MF_CHECKED);
        //Patni-PJS/2010Jan19/Added/Phase 3 MVC004363
        menu->CheckMenuItem(l_prescan_auto_voice_id, MF_BYPOSITION | MF_CHECKED);
    }
}

//************************************Method Header**********************************
// Method Name  : UpdateContrastAgentContextMenu
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPQMListCtrl::UpdateContrastAgentContextMenu(
    PopupMenuEx* menu,
    const int row_index
)
{
    return ((CPQMView*)GetParent())->CreateContrastAgentPopupMenu(menu, row_index) ;
}


//************************************Method Header**************************************
//Method Name:  ApplyParameterDictionary
//Author:  PATNI/DJ
//Purpose: Apply the parameter dictionary rules on row sequence data.
//**************************************************************************************/
void CPQMListCtrl::ApplyParameterDictionary(
    const SEQUENCE_PARAM& f_seq_param,
    DicParamConvertedValueList&  f_converted_value_list,
    DicParamNameList&           f_param_name_list,
    Data_decl_list& f_sequence_param_name_list,
    //Patni-Hemant/2010Mar10/Added/MSA0251-00277
    const bool f_seq_info /*= false*/
)
{
    LPCTSTR FUNC_NAME = _T("CPQMListCtrl::ApplyParameterDictionary");
    PQM_TRACE_SCOPED(FUNC_NAME);

    try {

        DicParamValueList           l_param_value_list;
        DATA_DECL l_decl = {0};
        _variant_t      l_param_value;

        POSITION l_pos = f_sequence_param_name_list.GetHeadPosition();

        while (l_pos) {

            l_decl = f_sequence_param_name_list.GetNext(l_pos);
            f_param_name_list.AddTail((CString)l_decl.m_name);
            FormSeqParamType(l_decl, l_decl.m_value, l_param_value);
            l_param_value_list.AddTail(l_param_value);
        }

        //+Patni-DKH/2009Aug24/Modified/cpp test corrections
        if (NULL == m_dic_manager) {
            PQM_TRACE(USER_FUNC_MARKER,
                      FUNC_NAME,
                      _T("m_dic_manager pointer is NULL"));
            return;
        }

        //-Patni-DKH/2009Aug24/Modified/cpp test corrections
        // Call to Parameter dictionary conversion
        m_dic_manager->ApplyDictionaryConversion(
            f_converted_value_list,
            l_param_value_list,
            f_param_name_list);

        //+Patni-Hemant/2010Mar10/Added/MSA0251-00277
        if (!f_seq_info) {

            // Call to Application dictionary for the parameters to be shown on the GUI
            m_dic_manager->ApplyAppDictionaryFormatting(
                m_app_dic_converted_value_list,
                f_param_name_list,
                f_converted_value_list);

        } else {

            m_dic_manager->ApplyAppDictionaryFormatting(
                m_app_dic_converted_value_list_seq_info,
                f_param_name_list,
                f_converted_value_list);
        }

    } catch (...) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exception Occured..."));
        throw;
    }


    //-Patni-Hemant/2010Mar10/Added/MSA0251-00277
}



//************************************Method Header**************************************
//Method Name:  FormSeqParamType
//Author:  PATNI/DJ
//Purpose: Form the seuqence parameter type.
//**************************************************************************************/
void CPQMListCtrl::FormSeqParamType(
    const DATA_DECL& f_decl,
    VALUE f_val,
    _variant_t& f_param_value
) const
{
    CString l_temp = _T("");

    switch (f_decl.m_type) {
        case DT_LONG:
            f_param_value.vt = VT_I4;
            f_param_value.lVal = f_val.lng;
            break;

        case DT_INT:
            f_param_value.vt = VT_I4;
            f_param_value.intVal = f_val.integer;
            break;

        case DT_STR:
            f_param_value.vt = VT_BSTR;
            l_temp = (CString)f_val.str;
            f_param_value.bstrVal = l_temp.AllocSysString();
            break;

        case DT_BOOL:
            f_param_value.vt = VT_I4;
            f_param_value.lVal = f_val.lng;

            break;

        case DT_DBL:
            f_param_value.vt = VT_R8;
            f_param_value.dblVal = f_val.dlb;
            break;

        case DT_FLT:
            f_param_value.vt = VT_R4;
            f_param_value.fltVal = (float)f_val.dlb;
            break;

        case DT_VECTOR:
            f_param_value.Attach(f_val.var_data);
    }
}


//************************************Method Header**************************************
//Method Name:  ApplyDisplayDictionary
//Author:  PATNI/DJ
//Purpose: Prepare the Header information of the list control.
//**************************************************************************************/
void CPQMListCtrl::ApplyDisplayDictionary(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMListCtrl::ApplyDisplayDictionary");

    // Call dictionary library function..
    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_dic_manager) {
        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("m_dic_manager pointer is NULL"));

        return;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections
    m_dic_manager->GetApplicationDisplayDetails(m_app_dic_converted_value_list);

    //Patni-Hemant/2010Mar10/Added/MSA0251-00277
    m_dic_manager->GetApplicationDisplayDetails(m_app_dic_converted_value_list_seq_info);

    // Set the column header for the sequences.
    // 1. Get the Label to be set as the header.
    // 2. Calculate the header width to be set.
    // 3. Get the alignment and set the alignment.

    CPQMView* p_parent = (CPQMView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

    //Added by Hemant On 12/11/2008 3:33:15 PM
    m_column_default_width_array.RemoveAll();

    //Patni-HEMANT/MODIFIED On 4/29/2009 2:06:27 PM/ ACE /ACE Internal Defect
    int l_column_no = 0;
    int l_width     = DEFAULT_COL_WIDTH; // default width
    int l_alignment = DICT_CENTER;            // default alignment

    CAppDictionaryDetails l_dic_detail;
    POSITION l_pos = m_app_dic_converted_value_list.GetHeadPosition();

    CString l_strColumnName = _T("");
    //+Redmine-2187
    BOOL l_mulitscandynamic_license = p_parent->GetMulitScanDynamic_License();

    while (l_pos) {

        l_dic_detail = m_app_dic_converted_value_list.GetNext(l_pos);

        if (!l_dic_detail.GetLabel().IsEmpty()) {

            //+Patni-PJS/2010May03/Added/JaFT# MVC007111
            //+Patni-Hemant/2009Jan15/Modified/GDI Leak Fix

            /*CDC* l_dc = GetDC();

              l_font_width = l_dc->GetTextExtent(l_temp, (l_temp.GetLength()));
              //l_width = (l_font_width.cx / l_temp.GetLength()) * (l_dic_detail.GetWidth());

                if (l_width <= 0) {
                l_width = DEFAULT_COL_WIDTH;
                }

                  ReleaseDC(l_dc);
            */
            //-Patni-Hemant/2009Jan15/Modified/GDI Leak Fix
            if (l_dic_detail.SeqListCtrlDisplay() == true) {
                l_width = l_dic_detail.GetWidth();

                if (l_width <= 0) {
                    l_width = DEFAULT_COL_WIDTH;
                }

                l_alignment = GetApplicationDictionaryAlignment(l_dic_detail.GetAlignment());

                //-Patni-PJS/2010May03/Added/JaFT# MVC007111
                //+Patni-NP/2010Apr12/Modified/MVC005120
                // if (!(l_strblock.CompareNoCase(_T("1000")))) //5120
                {
                    l_strColumnName = (l_dic_detail.GetLabel().Compare(_T("Sts")) == 0) ? _T("") :
                                      CPQMUtility::GetMultiLingualString(l_dic_detail.GetLabel()) ;

                    CString parameter_string = l_dic_detail.GetParameterValue() ;

                    //+Redmine-2187
                    if ((parameter_string.Compare(_T("DELAYTIME")) == 0) && !l_mulitscandynamic_license) {
                        continue;

                    }

                    if ((parameter_string.Compare(_T("COMM")) == 0) && !l_mulitscandynamic_license) {
                        l_width += DELAY_TIME_WIDTH;
                    }

                    //-Redmine-2187
                    //InsertColumn( l_column_no, l_dic_detail.GetLabel(),l_alignment , l_width);
                    InsertColumn(l_column_no, l_strColumnName, l_alignment , l_width);
                    //-Patni-Manish/2009July16/Added/JFT#GUIRenewal-Removed the WSE Column label

                    //+Patni-HEMANT/ADDED On 4/29/2009 2:01:31 PM/ ACE /Internal Defect

                    StoreColumnIndex(l_column_no, parameter_string);
                    l_column_no++;	//This is moved from outside ..
                    //
                    //-Patni-HEMANT/ADDED On 4/29/2009 2:01:31 PM/ ACE /Internal Defect


                    //Added by Hemant On 12/11/2008 3:33:08 PM
                    m_column_default_width_array.Add(l_width);

                    //Added by Hemant On 1/22/2009 5:04:41 PM
                    CString number_column_string = _T("PROTOCOLNO");

                    if (number_column_string.CompareNoCase(l_dic_detail.GetParameterValue()) == 0) {

                        m_number_column_alignment = l_alignment;
                    }

                    //End Of addition By Hemant On1/22/2009 5:05:28 PM

                }

                p_parent->GetHeaderArray().Add(l_dic_detail.GetLabel()); //Add label in global string array
                p_parent->GetTagArray().Add(l_dic_detail.GetParameterValue()); //This array will contain tag values.
                p_parent->GetInfoLableArray().Add(l_dic_detail.GetLongLabel());
            }

            if (g_scanID_tag.Compare(l_dic_detail.GetParameterValue()) == 0) {
                // m_c_para_listctrl = l_column_no-1;
            }
        }

        //+Patni-HEMANT/COMMENTED On 4/29/2009 2:03:49 PM/ ACE /Internal Error
        //l_column_no++;

        //SM3_Mangesh_End
    }//end while
}


//+Patni-HEMANT/ADDED On 4/29/2009 1:59:33 PM/ ACE /Internal Error
void CPQMListCtrl::StoreColumnIndex(
    const int coulm_index,
    const CString& parameter_string
)
{
    if (parameter_string.Compare(_T("MODE")) == 0) {

        m_mode_column_index = coulm_index ;

    } else if (parameter_string.Compare(_T("ACQSTATUS2")) == 0) {

        m_status_column_index = coulm_index ;

    } else if (parameter_string.Compare(_T("PROTOCOLNO")) == 0) {

        m_protocolno_column_index = coulm_index ;

    } else if (parameter_string.Compare(_T("SCANTIME")) == 0) {

        m_time_column_index = coulm_index ;

    } else if (parameter_string.Compare(_T("SAR")) == 0) {

        m_sar_column_index = coulm_index ;

        //+Patni-Hemant/2009Nov15/Commented/IR#90/Queue List
        //  } else if(parameter_string.Compare(_T("WSEEDITING")) == 0) {
        //
        //		m_wseediting_column_index = coulm_index ;
        //
        //-Patni-Hemant/2009Nov15/Commented/IR#90/Queue List

    } else if (parameter_string.Compare(_T("COMM")) == 0) {

        m_scan_id_column_index = coulm_index ;

    } else if (parameter_string.Compare(_T("PLAN")) == 0) {

        m_plan_column_index = coulm_index ;

    } else if (parameter_string.Compare(_T("TRSC")) == 0) {

        m_trsc_index = coulm_index ;
    }

    else if (parameter_string.Compare(_T("DELAYTIME")) == 0) {

        m_delay_column_index = coulm_index ;
    }



}
//-Patni-HEMANT/ADDED On 4/29/2009 1:59:33 PM/ ACE /Internal Error


//+Patni-Hemant/2009Dec21/Added/MVC006231
//************************************Method Header************************************
// Method Name  : IncludeSpaceUsingAlignment
// Author       : PATNI/ HEMANT
// Purpose      : according to alignment value, string should be concatenated with space..
//***********************************************************************************
const CString CPQMListCtrl::IncludeSpaceUsingAlignment(
    const CString& l_input_string,
    const int l_alignment
)const
{

    switch (l_alignment) {

        case LVCFMT_LEFT:
            return CString(_T(" ") + l_input_string);
            break ;

        case LVCFMT_RIGHT :
            return CString(l_input_string + _T(" ")) ;
            break ;
    }

    return l_input_string;
}
//-Patni-Hemant/2009Dec21/Added/MVC006231



//+Patni-Hemant/2010May14/Added/JaFT/TMSC-REDMINE-276
//************************************Method Header************************************
// Method Name  : CompleteDragDrop
// Author       : PATNI/ HEMANT
// Purpose      : This function performs or cancels the drop operation
//***********************************************************************************
void CPQMListCtrl::CompleteDragDrop(
    const bool f_cancel /*= true*/
)
{
    if (!IsDragging() || !m_pDragImage) {
        return ;
    }

    KillScrollTimer();

    // Release the mouse capture and end the dragging.
    ::ReleaseCapture();

    m_pDragImage->DragLeave(CWnd::GetDesktopWindow());

    m_pDragImage->EndDrag();

    DEL_PTR(m_pDragImage)

    // Drop the item on the list.
    if (!f_cancel) {
        DropItem();
    }

    m_isdraging = false;
}
//-Patni-Hemant/2010May14/Added/JaFT/TMSC-REDMINE-276


//************************************Method Header************************************
// Method Name  : UpdateMoveCouchContextMenu
// Author       : PATNI/ HEMANT
// Purpose      : Updates Move Couch Right click f_menu as per PQM conditions
//                f_row_index is the protocol index to consider for right click event
//***********************************************************************************
void CPQMListCtrl::UpdateMoveCouchContextMenu(
    PopupMenuEx* f_menu,
    const int f_row_index
)
{
    LPCTSTR FUNC_NAME = _T("CPQMListCtrl::UpdateMoveCouchContextMenu");

    CPQMView* l_parent = (CPQMView*)GetParent();

    if (NULL == l_parent) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("l_parent pointer is NULL"));
        return;
    }

    CSequence* l_seq = (CSequence*) GetItemData(f_row_index);

    if (NULL == l_seq) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_seq pointer is NULL"));
        return;
    }


    int l_id_selected = -1;

    switch (l_seq->GetMoveCouchMethodOption()) {

        case CENTREPLAN:
            l_id_selected = ID_MOVECOUCHMENU_CENTER_OF_THE_PLAN ;
            break ;

        case PARENTREFPOS:
            l_id_selected = ID_MOVECOUCHMENU_POSITION_OF_REFERENCE_IMAGE ;
            break ;
    }

    if (-1 != l_id_selected) {
        f_menu->CheckMenuItem(l_id_selected, MF_BYCOMMAND | MF_CHECKED);
    }

    if (K_IMAGING != l_parent->GetPQMGlobal().GetPQMMode()) {
        f_menu->EnableMenuItem(ID_MOVECOUCHMENU_CENTER_OF_THE_PLAN, MF_BYCOMMAND | MF_GRAYED);
        f_menu->EnableMenuItem(ID_MOVECOUCHMENU_POSITION_OF_REFERENCE_IMAGE, MF_BYCOMMAND | MF_GRAYED);
        return ;
    }

    int* l_selected_items = NULL ;
    int l_size = 0;

    if (!GetItemsForAutoVoiceAndMoveCouch(f_row_index, &l_selected_items, l_size, MOVE_COUCH_OPTION)) {

        f_menu->EnableMenuItem(ID_MOVECOUCHMENU_CENTER_OF_THE_PLAN, MF_BYCOMMAND | MF_GRAYED);
        f_menu->EnableMenuItem(ID_MOVECOUCHMENU_POSITION_OF_REFERENCE_IMAGE, MF_BYCOMMAND | MF_GRAYED);

        DEL_PTR_ARY(l_selected_items);
        return ;
    }

    if (IsAMBPlnProtocolSelected(l_selected_items, l_size)) {
        f_menu->EnableMenuItem(ID_MOVECOUCHMENU_CENTER_OF_THE_PLAN, MF_BYCOMMAND | MF_GRAYED);
        f_menu->EnableMenuItem(ID_MOVECOUCHMENU_POSITION_OF_REFERENCE_IMAGE, MF_BYCOMMAND | MF_GRAYED);

        DEL_PTR_ARY(l_selected_items);
        return ;
    }

    DEL_PTR_ARY(l_selected_items);

    BOOL l_enable = FALSE;
    int l_in_side_limit = 0, l_out_side_limit = 0;

    g_MainView->GetAcquisition()->GetInsideLimit(g_MainView->GetAcqOderForGivenProtocolIndex(f_row_index), &l_in_side_limit,
            &l_out_side_limit, &l_enable);

    if (!l_enable || g_MainView->IsWaitCursor()) {
        f_menu->EnableMenuItem(ID_MOVECOUCHMENU_CENTER_OF_THE_PLAN, MF_BYCOMMAND | MF_GRAYED);
        f_menu->EnableMenuItem(ID_MOVECOUCHMENU_POSITION_OF_REFERENCE_IMAGE, MF_BYCOMMAND | MF_GRAYED);
        return;
    }
}


//************************************Method Header************************************
// Method Name  : CanShowMasterSlavePlanMenu
// Author       : PATNI/AKR
// Purpose      :
//***********************************************************************************
bool CPQMListCtrl::CanShowMasterSlavePlanMenu(const int f_row_index)
{
    bool l_is_show_menu = false;
    int* l_selected_items = NULL;
    int l_selected_count = GetselectedItems(&l_selected_items);
    DEL_PTR_ARY(l_selected_items);

    if (1 != l_selected_count) {
        return l_is_show_menu;
    }

    CPQMView* p_parent = (CPQMView*)GetParent();

    if (p_parent && p_parent->GetMulitScanDynamic_License()) {
        CSequence* l_seq = (CSequence*) GetItemData(f_row_index);

        if (l_seq && TRUE ==  l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetInt(_T("MSDENABLEFLAG")) &&
            FALSE == l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetInt(_T("MASTERFLAG")) &&
            PR_ACQ_STATUS_WAIT == l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetInt(_T("ACQSTATUS2")) &&
            !p_parent->GetIsAMBPlanning() && !p_parent->IsAMBInvoked()) {

            l_is_show_menu = true;
        }
    }

    return l_is_show_menu;
}
//************************************Method Header************************************
// Method Name  : UpdatePlanMasterSlaveContextMenu
// Author       : PATNI/AKR
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::UpdatePlanMasterSlaveContextMenu(
    PopupMenuEx* f_menu,
    const int f_protocol_index
)
{
    CSequence* l_seq = (CSequence*) GetItemData(f_protocol_index);

    if (l_seq && l_seq->GetCPASCOMSequence()->GetSequenceParam()->GetInt(_T("GROUPID")) > 0) {
        f_menu->EnableMenuItem(ID_PLAN_RESET, MF_BYCOMMAND);

    } else {
        f_menu->EnableMenuItem(ID_PLAN_RESET, MF_BYCOMMAND | MF_GRAYED);
    }

    CPQMView* p_parent = (CPQMView*)GetParent();

    if (NULL == p_parent) {
        return;
    }

    p_parent->CreatePlanMasterSlavePopupMenu(f_menu, f_protocol_index);
}

bool CPQMListCtrl::IsAMBPlnProtocolSelected(
    int* f_selected_index,
    const int f_count
)
{
    if (!f_selected_index || f_count < 0)
        return false;

    for (int i = 0; i < f_count; i++) {

        if (g_MainView->IsProtocolSelectedForAMBPlan(f_selected_index[i])) {
            return true;
        }
    }

    return false;
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
int CPQMListCtrl::GetBitmapResourceIDForStatus(
    const int row_index
)
{
    CSequence* sequence_struct = GetSequenceStructForIndex(row_index);

    //+Patni-Hemant/2009Nov15/Modified/IR#90/Queue List
    if (sequence_struct) {
        switch (sequence_struct->GetAcqStatus()) {

            case PR_ACQ_STATUS_CURRENT: {
                CPQMView* p_parent = (CPQMView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

                if (p_parent->GetWaitTimeSARControlFlag()) {
                    return IDI_WAIT_FOR_AUTO_SCAN;
                }

                return (p_parent->IsPausedScanning() ? IDI_PAUSE_PROTOCOL_ICON : IDI_CURRENT_PROTOCOL_ICON);

            }
            break ;

            case PR_ACQ_STATUS_FAIL: {
                return IDI_FAIL_PROTOCOL_ICON;
            }
            break ;

            case PR_ACQ_STATUS_SUCCESS: {
                return IDI_DONE_PROTOCOL_ICON;
            }
            break ;

            case PR_ACQ_STATUS_WAIT: {

                const int l_holdvalue = sequence_struct->GetCPASCOMSequence()->GetSequenceParam()->GetInt(_T("HOLD"));

                CPQMView* p_parent = (CPQMView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

                bool l_is_wait_for_auto_scan_icon = false;

                const bool l_is_auto_scan_on = !((l_holdvalue & VFC_HOLD_PRE_SCAN) == VFC_HOLD_PRE_SCAN);

                if (p_parent && l_is_auto_scan_on) {
                    int l_first_wait_prot = -1;
                    p_parent->GetIndexOfFirstWaitStateProtocol(l_first_wait_prot);

                    if (IsValidSequenceIndex(l_first_wait_prot) &&
                        -1 != p_parent->CheckAndGetWaitingForAutoScanStartAcqOrder()) {

                        if (l_first_wait_prot == row_index) {
                            l_is_wait_for_auto_scan_icon  = true;

                            return IDI_WAIT_FOR_AUTO_SCAN;
                        }
                    }
                }

                return GetIconIDforStatus(sequence_struct, l_is_auto_scan_on);
            }
            break ;

            default :
                return -1;
        }
    }

    //-Patni-Hemant/2009Nov15/Modified/IR#90/Queue List

    ASSERT(FALSE) ;
    return -1 ;
}
//************************************Method Header************************************
// Method Name  : GetIconIDforStatus
// Author       : iGate
// Purpose      :
//***********************************************************************************
int CPQMListCtrl::GetIconIDforStatus(CSequence* f_seq_struct,
                                     const bool f_flag_auto_scan)
{
    CPQMView* l_parent = (CPQMView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();
    int l_first_wait_prot = -1;


    if (NULL == l_parent) {
        return -1;
    }


    if (l_parent->GetSARControlLicense()) {

        if (NULL == f_seq_struct) {
            return -1;
        }

        sarInfo_t* l_sar_info = f_seq_struct->GetSARInfoPtrForSeq();

        if (NULL == l_sar_info) {
            return -1;
        }


        if (VFC_SAE_LEVEL_OVER == l_sar_info->sae.sae_level) {

            return f_flag_auto_scan ? IDI_ICON_AUTOSCAN_SAE : IDI_ICON_SAE;

        } else if ((l_sar_info->sarControl.coilProtect & IEC_OVER_10SEC) || (l_sar_info->sarControl.sarProtect & IEC_OVER_10SEC)) {

            return f_flag_auto_scan ? IDI_ICON_AUTOSCAN_10SEC : IDI_ICON_10SEC;

        } else if ((l_sar_info->sarControl.coilProtect & IEC_OVER_6MIN) || (l_sar_info->sarControl.sarProtect & IEC_OVER_6MIN)) {

            if (-1 == l_sar_info->sarControl.waitTime) {
                return  f_flag_auto_scan ? IDI_ICON_AUTOSCAN_6MIN_NOWAIT : IDI_ICON_6MIN_NOWAIT;

            } else {
                return  f_flag_auto_scan ? IDI_ICON_AUTOSCAN_6MIN_WAIT : IDI_ICON_6MIN_WAIT;
            }
        }
    }

    return f_flag_auto_scan ? IDI_AUTO_SCAN_ICON : -1;
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
int CPQMListCtrl::GetBitmapResourceIDForPLANNEDProtocol(
    const int row_index,
    CString& f_plantxtstr
)
{
    CSequence* sequence_struct = GetSequenceStructForIndex(row_index);

    int l_plan_icon_id = -1;

    if (sequence_struct) {

        bool l_is_msd_flag = false;
        bool l_cala_hala_flag = false;
        const int l_acq_order = sequence_struct->GetCPASCOMSequence()->GetSequenceParam()->GetInt(_T("ACQORDER"));

        const int l_acq_status = sequence_struct->GetAcqStatus();
        const int l_master_flag = sequence_struct->GetCPASCOMSequence()->GetSequenceParam()->GetInt(_T("MASTERFLAG"));
        const int l_group_id = sequence_struct->GetCPASCOMSequence()->GetSequenceParam()->GetInt(_T("GROUPID"));

        const int l_amb_flag = sequence_struct->GetCPASCOMSequence()->GetSequenceParam()->GetInt(_T("AMB-FLAG"));
        const int l_amb_mode_flag = sequence_struct->GetCPASCOMSequence()->GetSequenceParam()->GetInt(_T("AMB-MODE-FLAG"));
        int l_is_master_slave = PQM_NORMAL_PROTOCOL;

        if (l_group_id) {
            l_is_master_slave = l_master_flag ? PQM_MASTER_PROTOCOL : PQM_SLAVE_PROTOCOL;
        }

        CString l_series_id = _T("");

        if (PQM_SLAVE_PROTOCOL == l_is_master_slave) {
            l_is_msd_flag =  true;
            int l_master_series_num = -1;
            g_MainView->GetAcquisition()->GetMasterPositionForSlave(row_index, &l_master_series_num);

            if (-1 != l_master_series_num) {
                l_series_id.Format(_T("%d"), l_master_series_num);
            }
        }

        int l_calaplanstatus = 0;
        g_MainView->GetAcquisition()->GetAutoLocatorPlannedStatus(l_acq_order, &l_calaplanstatus);

        if (PLANNED_STATUS_NONE != l_calaplanstatus) {
            if (PLANNED_STATUS_NOVECTOR == l_calaplanstatus || PLANNED_STATUS_STARTED == l_calaplanstatus) {
                f_plantxtstr = _T("E");

            } else {
                f_plantxtstr = _T("P");
            }

            l_cala_hala_flag =  true;
        }

        switch (sequence_struct->GetWSEEditingEnum()) {

                //+Patni-Hemant/2009Nov15/Modified/IR#90/Queue List
            case WSE_Planned:

                if ((sequence_struct->GetAMBEditingEnum() == AMB_PLANNED)
                    || (l_amb_flag && (VFC_AMB_MODE_MASTER == l_amb_mode_flag || VFC_AMB_MODE_SLAVE == l_amb_mode_flag))) {
                    f_plantxtstr = _T("AP");

                } else if (!l_cala_hala_flag) {
                    f_plantxtstr = _T("P");
                }

                l_plan_icon_id = GetIconIDFromStatus(l_acq_status);
                break ;

            case WSE_Current:
            case WSE_Planned_AND_Current:
                l_plan_icon_id = IDI_PLAN_ACTIVE_ICON;
                break ;

            case WSE_None:

                if (sequence_struct->GetAMBEditingEnum() == AMB_PLAN) {
                    f_plantxtstr = _T("A");
                    l_plan_icon_id = GetIconIDFromStatus(l_acq_status);
                    break;

                }

            default : {
                if (l_is_msd_flag || l_cala_hala_flag) {
                    l_plan_icon_id = GetIconIDFromStatus(l_acq_status);
                }
            }
        }

        if (-1 != l_plan_icon_id && l_series_id.CompareNoCase(_T(""))) {
            f_plantxtstr += l_series_id;
        }

    }

    //-Patni-Hemant/2009Nov15/Modified/IR#90/Queue List

    return l_plan_icon_id ;
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CRect CPQMListCtrl::GetOptionRectForModeOptionIndex(
    const int row_index,
    const int mode_option_index
)
{
    const int current_col = m_mode_column_index;
    const CRect cellrect = GetCellRect(row_index, current_col);
    CRect mode_option_rect = cellrect;

    const int x_offset = MODE_OPTION_BITMAP_WIDTH;
    const int y_offset = 0 ;


    int start_x = cellrect.left;

    CRect total_option_rect = cellrect;
    total_option_rect.right = total_option_rect.left + (x_offset * TOTAL_MODE_FALGS) + (MODE_OPTIONS_GAP * TOTAL_MODE_FALGS) /*- MODE_OPTIONS_GAP*/;

    if (total_option_rect.Width() < cellrect.Width()) {

        CenterRectForRowIndex(row_index, current_col, total_option_rect, MODE_ALIGN_CENTER);

        //Patni-Hemant/2009Nov15/Modified/IR#90/Queue List
        start_x = total_option_rect.left + MODE_OPTIONS_GAP;
    }

    mode_option_rect.left = (start_x) + (x_offset * mode_option_index) + (mode_option_index * MODE_OPTIONS_GAP) ;
    mode_option_rect.right = mode_option_rect.left + MODE_OPTION_BITMAP_WIDTH ;
    mode_option_rect.top += y_offset ;
    mode_option_rect.bottom = mode_option_rect.top + MODE_OPTION_BITMAP_HEIGHT;

    CenterRectForRowIndex(row_index, current_col, mode_option_rect, false, true);

    return (cellrect.right < mode_option_rect.right) ? CRect(0, 0, 0, 0) : mode_option_rect ;
}



//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
int CPQMListCtrl::GetBitmapResourceIDForModeOption(
    const int row_index,
    const int mode_option_index
)const
{

    CSequence* sequence_struct = GetSequenceStructForIndex(row_index);

    if (sequence_struct) {

        switch (mode_option_index) {

                //+Patni-Hemant/2009Nov15/Modified/IR#90/Queue List
            case AUTO_VOICE_OPTION : {

                return ((sequence_struct->GetAutoVoiceOption() != NONE_NONE) ? IDI_VOICE_ACTIVE :
                        ((sequence_struct->GetAcqStatus() == PR_ACQ_STATUS_SUCCESS)  || (sequence_struct->GetAcqStatus() == PR_ACQ_STATUS_FAIL)) ?
                        IDI_VOICE_DISABLE_NO_FUNCTION : IDI_VOICE_DISABLE) ;
            }
            break ;

            case CONTRAST_AGENT_OPTION : {

                CPqmPrMode& pr_mode = sequence_struct->GetPrMode();
                return ((pr_mode.m_contrast) ? IDI_INJECTOR_ACTIVE :
                        ((sequence_struct->GetAcqStatus() == PR_ACQ_STATUS_SUCCESS)  || (sequence_struct->GetAcqStatus() == PR_ACQ_STATUS_FAIL)) ?
                        IDI_INJECTOR_DISABLE_NO_FUNCTION : IDI_INJECTOR_DISABLE);
            }
            break ;

            //+Patni-RUP/2010Jul19/Added/TMSC-REDMINE-466
            case MOVE_COUCH_OPTION : {
                MOVECOUCH_METHOD l_movecouch_method = sequence_struct->GetMoveCouchMethodOption();
                return ((l_movecouch_method != NOMOVE) ? IDI_MOVEBED_ACTIVE :
                        ((sequence_struct->GetAcqStatus() == PR_ACQ_STATUS_SUCCESS)  || (sequence_struct->GetAcqStatus() == PR_ACQ_STATUS_FAIL)) ?
                        IDI_MOVEBED_DISABLED : IDI_MOVEBED_NORMAL);
            }

            //-Patni-RUP/2010Jul19/Added/TMSC-REDMINE-466
        }
    }

    //-Patni-Hemant/2009Nov15/Modified/IR#90/Queue List

    return -1;
}



//+Patni-Hemant/2009Nov15/Added/IR#90/Queue List
//************************************Method Header************************************
// Method Name  : GetHeightWidthForIcon
// Author       : PATNI/ HEMANT
// Purpose      : This function fills height and width parameter for given icon
//                resourse id. If resource id is found then function returns true
//                else function returns false
//***********************************************************************************
bool CPQMListCtrl::GetHeightWidthForIcon(
    const int f_IconId,
    int& f_width,
    int& f_height
)
{
    f_width = 0;
    f_height = 0;

    switch (f_IconId) {

            //+Patni-Hemant/2009Dec7/Modified/IR#90/Queue List
        case IDI_FAIL_PROTOCOL_ICON:
        case IDI_DONE_PROTOCOL_ICON:

            f_width = 18;
            f_height = 18;

            break ;

        case IDI_CURRENT_PROTOCOL_ICON:
        case IDI_PAUSE_PROTOCOL_ICON:
        case IDI_AUTO_SCAN_ICON:
        case IDI_WAIT_FOR_AUTO_SCAN:
        case IDI_ICON_SAE:
        case IDI_ICON_AUTOSCAN_SAE:
        case IDI_ICON_10SEC:
        case IDI_ICON_AUTOSCAN_10SEC:
        case IDI_ICON_6MIN_NOWAIT:
        case IDI_ICON_6MIN_WAIT:
        case IDI_ICON_AUTOSCAN_6MIN_WAIT:
        case IDI_ICON_AUTOSCAN_6MIN_NOWAIT:

            //-Patni-Hemant/2009Dec7/Modified/IR#90/Queue List

            f_width = 29;
            f_height = 24;

            break ;

        case IDI_PLAN_ACTIVE_ICON:

            f_width = 29;
            f_height = 28;

            break ;

        case IDI_PLAN_CONE_WAIT_PROTOCOL_ICON:
        case IDI_PLAN_CONE_NON_WAIT_PROTOCOL_ICON:
            f_width = 34;
            f_height = 28;
            break;

        default :
            return false;
    }

    return true;
}
//-Patni-Hemant/2009Nov15/Added/IR#90/Queue List

//************************************Method Header************************************
// Method Name  : GetAutoVoiceOptionForID
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
int CPQMListCtrl::GetAutoVoiceOptionForID(
    const AUTOVOICE_OPTIONS auto_voice_option
)const
{
    switch (auto_voice_option) {

        case NONE_NONE :
            return ID_MENUITEM_N_N ;

        case NONE_B1 :
            return ID_MENUITEM_N_B1 ;

        case NONE_B2 :
            return ID_MENUITEM_N_B2 ;

        case NONE_B3 :
            return ID_MENUITEM_N_B3 ;

        case NONE_MOTION :
            return ID_MENUITEM_N_M1 ;

        case B1_NONE :
            return ID_MENUITEM_B1_N ;

        case B1_B1 :
            return ID_MENUITEM_B1_B1;

        case B1_B2 :
            return ID_MENUITEM_B1_B2;

        case B1_B3 :
            return ID_MENUITEM_B1_B3;

        case B1_MOTION :
            return ID_MENUITEM_B1_M1;

        case B2_NONE :
            return ID_MENUITEM_B2_N;

        case B2_B1 :
            return ID_MENUITEM_B2_B1 ;

        case B2_B2 :
            return ID_MENUITEM_B2_B2;

        case B2_B3 :
            return ID_MENUITEM_B2_B3;

        case B2_MOTION :
            return ID_MENUITEM_B2_M1;

        case B3_NONE :
            return ID_MENUITEM_B3_N;

        case B3_B1 :
            return ID_MENUITEM_B3_B1;

        case B3_B2 :
            return ID_MENUITEM_B3_B2;

        case B3_B3 :
            return ID_MENUITEM_B3_B3;

        case B3_MOTION :
            return ID_MENUITEM_B3_M1;

        case MOTION_NONE :
            return ID_MENUITEM_M1_N ;

        case MOTION_B1 :
            return ID_MENUITEM_M1_B1;

        case MOTION_B2 :
            return ID_MENUITEM_M1_B2;

        case MOTION_B3 :
            return ID_MENUITEM_M1_B3;

        case MOTION_MOTION :
            return ID_MENUITEM_M1_M1;
    }

    //ASSERT(FALSE);	//we should not reach here..
    return -1;
}

//************************************Method Header************************************
// Method Name  : GetAutoVoiceOptionForID
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
const CString CPQMListCtrl::GetAutoVoiceStringForID(
    const AUTOVOICE_OPTIONS auto_voice_option
)const
{
    switch (auto_voice_option) {

        case NONE_NONE :
            return CPQMUtility::GetMultiLingualString(_T("IDS_NONE_NONE"));

        case NONE_B1 :
            return CPQMUtility::GetMultiLingualString(_T("IDS_NONE_B1"));

        case NONE_B2 :
            return CPQMUtility::GetMultiLingualString(_T("IDS_NONE_B2"));

        case NONE_B3 :
            return CPQMUtility::GetMultiLingualString(_T("IDS_NONE_B3"));

        case NONE_MOTION :
            return CPQMUtility::GetMultiLingualString(_T("IDS_NONE_MOTION"));

        case B1_NONE :
            return CPQMUtility::GetMultiLingualString(_T("IDS_B1_NONE"));

        case B1_B1 :
            return CPQMUtility::GetMultiLingualString(_T("IDS_B1_B1"));

        case B1_B2 :
            return CPQMUtility::GetMultiLingualString(_T("IDS_B1_B2"));

        case B1_B3 :
            return CPQMUtility::GetMultiLingualString(_T("IDS_B1_B3"));

        case B1_MOTION :
            return CPQMUtility::GetMultiLingualString(_T("IDS_B1_MOTION"));

        case B2_NONE :
            return CPQMUtility::GetMultiLingualString(_T("IDS_B2_NONE"));

        case B2_B1 :
            return CPQMUtility::GetMultiLingualString(_T("IDS_B2_B1"));

        case B2_B2 :
            return CPQMUtility::GetMultiLingualString(_T("IDS_B2_B2"));

        case B2_B3 :
            return CPQMUtility::GetMultiLingualString(_T("IDS_B2_B3"));

        case B2_MOTION :
            return CPQMUtility::GetMultiLingualString(_T("IDS_B2_MOTION"));

        case B3_NONE :
            return CPQMUtility::GetMultiLingualString(_T("IDS_B3_NONE"));

        case B3_B1 :
            return CPQMUtility::GetMultiLingualString(_T("IDS_B3_B1"));

        case B3_B2 :
            return CPQMUtility::GetMultiLingualString(_T("IDS_B3_B2"));

        case B3_B3 :
            return CPQMUtility::GetMultiLingualString(_T("IDS_B3_B3"));

        case B3_MOTION :
            return CPQMUtility::GetMultiLingualString(_T("IDS_B3_MOTION"));

        case MOTION_NONE :
            return CPQMUtility::GetMultiLingualString(_T("IDS_MOTION_NONE"));

        case MOTION_B1 :
            return CPQMUtility::GetMultiLingualString(_T("IDS_MOTION_B1"));

        case MOTION_B2 :
            return CPQMUtility::GetMultiLingualString(_T("IDS_MOTION_B2"));

        case MOTION_B3 :
            return CPQMUtility::GetMultiLingualString(_T("IDS_MOTION_B3"));

        case MOTION_MOTION :
            return CPQMUtility::GetMultiLingualString(_T("IDS_MOTION_MOTION"));
    }

    //ASSERT(FALSE);	//we should not reach here..
    return _T("");
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
const CString CPQMListCtrl::GetToolTipTextForCell(
    const CPoint& cell_index,
    CDC* f_dc,
    CRect& f_tooltip_text_rect
)
{
    CString tool_tip_text = _T("");
    unsigned int act_column_index = (((cell_index.x - 1) >> 5) & 0x3fff);
    const int option_index = (cell_index.x - 1) & 0xf;

    if (act_column_index == m_mode_column_index) {
        CString mode_option_index = _T("");

        switch (option_index - 1) {

            case AUTO_TRANSFER_OPTION: {
            }
            break;

            case  BREATH_HOLD_OPTION: {
            }
            break;

            case  AUTO_VOICE_OPTION	: {
                mode_option_index = GetAutoVoiceStringForIndex(cell_index.y, f_dc, f_tooltip_text_rect);
            }
            break;

            case  CONTRAST_AGENT_OPTION	: {
                mode_option_index = GetContrastAgentToolTipStr(cell_index.y, f_dc, f_tooltip_text_rect);
            }
            break;

            case  MOVE_COUCH_OPTION	: {
                mode_option_index = GetMoveCouchStringForIndex(cell_index.y, f_dc, f_tooltip_text_rect);
            }
            break;
        }

        tool_tip_text = mode_option_index ;
        tool_tip_text.TrimLeft();
        tool_tip_text.TrimRight();
    }

    // + Patni-Sribanta/2010June26/Added/IR-156_2
    else if ((act_column_index == m_scan_id_column_index)
             || (act_column_index == m_time_column_index)
             || (act_column_index == m_status_column_index)
             || (act_column_index == m_protocolno_column_index)
            ) {


        CPQMView* p_parent = (CPQMView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

        CString f_scan_id(_T(""));
        p_parent->GetHoveredProtocolIdent(&f_scan_id, cell_index.y);
        tool_tip_text = f_scan_id;
        tool_tip_text.TrimLeft();
        tool_tip_text.TrimRight();

        const CString l_str_sar_reason = OnToolSARSAROverinformation(cell_index.y);

        if (!l_str_sar_reason.IsEmpty()) {
            tool_tip_text += _T("\n") + l_str_sar_reason;
        }
    }

    // - Patni-Sribanta/2010June25/Added/IR-156_2

    return tool_tip_text;
}

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CSequence* CPQMListCtrl::GetSequenceStructForIndex(
    const int index
)const
{

    LPCTSTR FUNC_NAME = _T("CPQMListCtrl::GetSequenceStructForIndex");

    if (index < 0) {

        ASSERT(FALSE) ;
        return FALSE ;
    }

    //+Patni-Dhanesh/2009Aug20/Modified/cpp test corrections
    if (NULL == m_list) {
        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("m_list pointer is NULL"));
        return NULL ;
    }

    //-Patni-Dhanesh/2009Aug20/Modified/cpp test corrections

    if (index >= m_list->GetCount()) {
        ASSERT(FALSE) ;
        return FALSE ;
    }

    POSITION pos = m_list->GetHeadPosition();

    if (pos) {

        for (int item = 0; item < index; item++) {
            m_list->GetNext(pos);
        }

        return (m_list->GetNext(pos));
    }


    return NULL ;
}


//+Patni-Sudhir/2011Jan27/Added/Redmine-434
//************************************Method Header*****************************
// Method Name  : GetNRFilterStringForProtocol
// Author       : PATNI/Sudhir
// Purpose      : The function value of Filter ID
//******************************************************************************

CString CPQMListCtrl::GetNRFilterStringForProtocol(const int l_acqOrder, const int l_NRFLG)
{
    VARIANT* str_NRVal = new VARIANT;
    CString l_NRVAL = _T("");

    if (g_MainView->GetAcquisition()->GetNRFilterStringForProtocol(l_acqOrder, l_NRFLG, str_NRVal) == S_OK) {
        l_NRVAL = str_NRVal->pcVal;
    }

    DEL_PTR(str_NRVal);
    return l_NRVAL;
}
//-Patni-Sudhir/2011Jan27/Added/Redmine-434



//************************************Method Header*****************************
// Method Name  : GetEMTONEFilterStringForProtocol
// Author       :
// Purpose      :
//******************************************************************************
const CString CPQMListCtrl::GetEMTFilterStringForProtocol(const int f_acqOrder)const
{
    VARIANT* str_EMTVal = new VARIANT;
    LPCTSTR l_default_name = _T("----------");
    CString l_EMTVAL = l_default_name;

    str_EMTVal->byref = &l_EMTVAL;

    if (g_MainView->GetAcquisition()->GetEMTFilterStringForProtocol(f_acqOrder, str_EMTVal) == S_OK) {

        if (1 != str_EMTVal->intVal) {
            l_EMTVAL = l_default_name;
        }
    }

    str_EMTVal->byref = NULL;
    DEL_PTR(str_EMTVal);
    return l_EMTVAL;
}



//************************************Method Header*****************************
// Method Name  : GetContrastAgentToolTipStr
// Author       :
// Purpose      :
//******************************************************************************
const CString CPQMListCtrl::GetContrastAgentToolTipStr(
    const int f_row_index,
    CDC* f_dc,
    CRect& f_tooltip_text_rect
)const
{

    CString l_str_tooltip = CPQMUtility::GetMultiLingualString(_T("IDS_PQM_CONTRAST_AGENT"));
    CSequence* l_seq = (CSequence*) GetItemData(f_row_index);

    if (NULL == l_seq) {
        f_dc->DrawText(l_str_tooltip, &f_tooltip_text_rect, DT_CALCRECT);
        return l_str_tooltip;
    }

    return CPQMImageUtility::GetToolTipValueFor(l_str_tooltip, l_seq->GetPrMode().m_str_contrast, f_dc, f_tooltip_text_rect);

}


//+Patni-RUP/2010Jul22/Added/TMSC-REDMINE-466
//************************************Method Header*****************************
// Method Name  : GetMoveCouchStringForIndex
// Author       : PATNI/RUP
// Purpose      : The function gets multi text for the couch position
//******************************************************************************
const CString CPQMListCtrl::GetMoveCouchStringForIndex(
    const int f_row_index,
    CDC* f_dc,
    CRect& f_tooltip_text_rect
)const
{
    CString str = CPQMUtility::GetMultiLingualString(_T("IDS_MOVE_COUCH"));
    CSequence* l_sequence_struct = GetSequenceStructForIndex(f_row_index);

    if (NULL == l_sequence_struct) {
        f_dc->DrawText(str, &f_tooltip_text_rect, DT_CALCRECT);
        return str;
    }

    CString l_value = _T("");

    switch (l_sequence_struct->GetMoveCouchMethodOption()) {

        case PARENTREFPOS:
            l_value = CPQMUtility::GetMultiLingualString(_T("IDS_PQM_POSITION_OF_REF_IMAGE"));
            break;

        case CENTREPLAN:
            l_value = CPQMUtility::GetMultiLingualString(_T("IDS_PQM_CENTER_OF_THE_PLAN"));
            break;

        default:
            l_value = CPQMUtility::GetMultiLingualString(_T("IDS_PQM_NO_MOVE"));
            break;
    }

    return CPQMImageUtility::GetToolTipValueFor(str, l_value, f_dc, f_tooltip_text_rect);
}



//************************************Method Header**************************************
//Method Name:  GetApplicationDictionaryAlignment
//Author:  PATNI/DJ
//Purpose: Return the Column alignment information.
//**************************************************************************************/
int CPQMListCtrl::GetApplicationDictionaryAlignment(
    const int f_align
)const
{
    switch (f_align) {

        case DICT_CENTER:
            return LVCFMT_CENTER;

        case DICT_LEFT:
            return LVCFMT_LEFT;

        case DICT_RIGHT:
            return LVCFMT_RIGHT;

        default:
            return DEFAULT_APP_DIC_ALIGNMENT;

    }

    return DEFAULT_APP_DIC_ALIGNMENT ;
}



//+Patni-Hemant/2009Nov24/Commented/IR#90/Queue List



//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
const CPoint CPQMListCtrl::GetCellNoFromPoint(
    const CPoint& point_in_client
)
{
    LVHITTESTINFO info ;

    info.pt = point_in_client ;
    SubItemHitTest(&info) ;

    return CPoint(info.iSubItem, info.iItem);
}


//+Patni-Hemant/v/Added/IR#90/Queue List
//************************************Method Header************************************
// Method Name  : GetImageRectForResourceID
// Author       : PATNI/ HEMANT
// Purpose      : This function returns the icon rect for given icon resource id
//***********************************************************************************
CRect CPQMListCtrl::GetImageRectForResourceID(
    const int f_resource_id
)
{
    CRect l_icon_rect(0, 0, 0, 0);

    int l_width = 0;
    int l_height = 0;
    GetHeightWidthForIcon(f_resource_id, l_width, l_height);
    l_icon_rect.right = l_width;
    l_icon_rect.bottom = l_height;

    return l_icon_rect;

}
//-Patni-Hemant/2009Nov15/Added/IR#90/Queue List

//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
const CString CPQMListCtrl::GetAutoVoiceStringForIndex(
    const int row_index,
    CDC* f_dc,
    CRect& f_tooltip_text_rect
)const
{
    CString str = CPQMUtility::GetMultiLingualString(_T("IDS_PQM_AUTO_VOICE"));

    if (!IsValidSequenceIndex(row_index)) {

        f_dc->DrawText(str, &f_tooltip_text_rect, DT_CALCRECT);
        return str;
    }

    CString l_value = GetAutoVoiceStringForID(((CSequence*) GetItemData(row_index))->GetAutoVoiceOption()) ;
    return CPQMImageUtility::GetToolTipValueFor(str, l_value, f_dc, f_tooltip_text_rect);

}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
const CRect CPQMListCtrl::GetCellRect(
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



void CPQMListCtrl::SetScrollTimer(const EScrollDirection ScrollDirection)
{
    if (m_uScrollTimer == 0) {
        m_uScrollTimer = SetTimer(SCROLL_TIMER_ID, 100, NULL);
    }

    m_ScrollDirection = ScrollDirection;
}

int CPQMListCtrl::GetIconIDFromStatus(const int f_acqatatus)const
{
    switch (f_acqatatus) {
        case PR_ACQ_STATUS_SUCCESS :
        case PR_ACQ_STATUS_FAIL :
            return IDI_PLAN_CONE_NON_WAIT_PROTOCOL_ICON;

        case PR_ACQ_STATUS_CURRENT :
        case PR_ACQ_STATUS_WAIT:
            return IDI_PLAN_CONE_WAIT_PROTOCOL_ICON;

    }

    return -1;
}


//************************************Method Header************************************
// Method Name  : IsSelectedItemsInSequence
// Author       : PATNI/ SANJAY
// Purpose      :
//***********************************************************************************
bool CPQMListCtrl::IsSelectedItemsInSequence()
{

    UINT l_selected_count = GetSelectedCount();

    if (l_selected_count <= 1) {
        return false;
    }

    POSITION l_pos = GetFirstSelectedItemPosition();

    int l_currently_selected_seq = -1;
    int l_prev_selected_seq  = -1;
    int l_count = 0;

    while (l_pos) {

        l_currently_selected_seq = GetNextSelectedItem(l_pos);

        if (l_count != 0) {

            if (!(l_currently_selected_seq  == l_prev_selected_seq + 1)) {
                return false;
            }

        }

        l_prev_selected_seq = l_currently_selected_seq;
        l_count++;
    }

    return true;
}

//************************************Method Header************************************
// Method Name  : CheckAMBContextMenuStatus
// Author       : iGATE
// Purpose      :
//***********************************************************************************
bool CPQMListCtrl::CheckAMBContextMenuStatus()
{
    int* l_selected_items = NULL ;
    bool l_iswait_selected = false ;
    bool l_iscurrent_selected = false ;
    bool l_isdone_selected = false ;
    bool l_isfail_selected = false ;
    bool l_isfirstwait_selected = false ;
    bool l_iswse_eidted_protocol_selected = false;

    int l_count = GetselectedItems(&l_selected_items, &l_iswait_selected, &l_iscurrent_selected, &l_isdone_selected, &l_isfail_selected, &l_isfirstwait_selected , &l_iswse_eidted_protocol_selected, NULL);

    if ((NULL == l_selected_items) && (l_count < 0))
        return false;

    bool l_amb_license = (g_MainView->GetAcquisition()->GetAMBLicense() == S_OK);
    l_isfirstwait_selected = (l_selected_items[0] == GetFirstWaitProtocolIndex(true));

    if (!(K_IMAGING == g_MainView->GetPQMGlobal().GetPQMMode()) ||
        !(l_count >= 2 && l_count <= 6) ||
        !l_isfirstwait_selected  ||
        (TRUE == l_iscurrent_selected) ||
        (TRUE == l_isdone_selected)  ||
        (TRUE == l_isfail_selected) ||
        (TRUE == l_iswse_eidted_protocol_selected) ||
        !l_amb_license ||
        (!g_MainView->IsAcqumanReadyForNextRequest()) ||
        g_MainView->GetIsWSEOpen() ||
        g_MainView->GetMemoryAliveFlag() || !IsSelectedItemsInSequence() ||
        !g_MainView->IsLocatorEnabled() ||
        g_MainView->AMBCheckIsAnyDialogOpen() ||
        g_MainView->GetAPCRetry()
       )
        return false;

    return true;
}

//************************************Method Header************************************
// Method Name  : IsAMBFlagEnable
// Author       : iGATE
// Purpose      :
//***********************************************************************************
bool CPQMListCtrl::IsAMBFlagEnable()
{
    int* l_selected_seq = NULL;
    int l_seq_count = GetselectedItems(&l_selected_seq);

    if (l_selected_seq) {
        for (int i = 0; i < l_seq_count; i++) {
            CSequence* l_seq = (CSequence*)GetItemData(l_selected_seq[i]);

            if (!l_seq->GetAMBFlag())
                return false;
        }
    }

    return true;
}
//************************************Method Header************************************
// Method Name  : InvalidateItemsForSARControl
// Author       : PATNI/ AKR
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::InvalidateItemsForSARControl()
{
    const int l_total_item_count = GetItemCount();
    const int l_first_wait_index = GetFirstWaitProtocolIndex(true);

    for (int l_index = l_first_wait_index ; l_index < l_total_item_count; ++l_index) {
        if (-1 != m_protocolno_column_index) {
            InvalidateItemForColumn(l_index, m_protocolno_column_index);
        }

        if (-1 != m_time_column_index) {
            InvalidateItemForColumn(l_index, m_time_column_index);
        }

        if (-1 != m_scan_id_column_index) {
            InvalidateItemForColumn(l_index, m_scan_id_column_index);
        }

        if (-1 != m_status_column_index) {
            InvalidateItemForColumn(l_index, m_status_column_index);
        }
    }
}

//************************************Method Header************************************
// Method Name  : CheckAndEndDragging
// Author       : iGATE/RJ
// Purpose      : Check dragging is in progress & if then cancels the drop operation
//***********************************************************************************
void CPQMListCtrl::CheckAndEndDragging()
{

    if (IsDragging() && m_pDragImage) {
        CompleteDragDrop();
        Invalidate();
    }
}

//************************************Method Header*********************************
// Method Name  : DrawTextWithUpdatedStringAndFormate
// Author       : iGate/AKR
// Purpose      :
//***********************************************************************************
void CPQMListCtrl::DrawTextWithUpdatedStringAndFormate(const int f_item_number, const unsigned int f_column_id,
        const int f_col_alignment, CDC& f_cdc, CBrush& f_bg_brush, CString f_cell_string)
{

    CRect l_cellrect = GetCellRect(f_item_number, f_column_id);
    l_cellrect.top += 1;


    CRect l_calc_rect ;
    f_cdc.DrawText(f_cell_string, &l_calc_rect, DT_CALCRECT);

    UINT flags = DT_END_ELLIPSIS | DT_VCENTER | DT_SINGLELINE;
    const int l_adjust_col_alignment = 3;

    if (l_cellrect.Width() <= l_calc_rect.Width()) {
        flags |= DT_LEFT ;

        if (f_column_id == m_scan_id_column_index) {
            l_cellrect.left += l_adjust_col_alignment;
        }

    } else {
        switch (f_col_alignment) {

            case LVCFMT_LEFT:
                flags |= DT_LEFT ;

                if (f_column_id == m_scan_id_column_index) {
                    l_cellrect.left += l_adjust_col_alignment;
                }

                break ;

            case LVCFMT_RIGHT :
                flags |= DT_RIGHT;

                if (f_column_id == m_scan_id_column_index) {
                    l_cellrect.right -= l_adjust_col_alignment;
                }

                break ;

            case LVCFMT_CENTER:
                flags |= DT_CENTER ;
                break ;
        }
    }

    f_cell_string = IncludeSpaceUsingAlignment(f_cell_string, f_col_alignment);
    f_cdc.FillRect(&l_cellrect, &f_bg_brush);
    f_cdc.DrawText(f_cell_string, l_cellrect,  DT_NOPREFIX | flags);
}

//****************************Method Header************************************
//Method Name	: OnToolSARSAROverinformation()
//Author		: iGate/
//Purpose		:
//*****************************************************************************
CString CPQMListCtrl::OnToolSARSAROverinformation(const int f_seq_index) const
{
    CString l_str_sar_info_text(_T(""));
    CSequence* l_sequence_struct = GetSequenceStructForIndex(f_seq_index);

    if (NULL == l_sequence_struct) {
        return l_str_sar_info_text;
    }

    sarInfo_p l_sar_info = l_sequence_struct->GetSARInfoPtrForSeq();

    if (NULL == l_sar_info) {
        return l_str_sar_info_text;
    }

    if (l_sar_info->sarControl.sarProtect & IEC_OVER_6MIN) {

        l_str_sar_info_text += _T("\nStandard SAR Over");
    }

    if (l_sar_info->sarControl.sarProtect & IEC_OVER_10SEC) {

        l_str_sar_info_text += _T("\nShort SAR Over");
    }

    if (l_sar_info->sarControl.coilProtect & IEC_OVER_6MIN) {

        l_str_sar_info_text += _T("\nStandard Coil Protection Over");
    }

    if (l_sar_info->sarControl.coilProtect & IEC_OVER_10SEC) {

        l_str_sar_info_text += _T("\nShort Coil Protection Over");
    }

    if (l_sar_info->sarControl.waitTime > 0) {

        CString l_str_wait_time(_T(""));
        l_str_wait_time.Format(_T("\nWait Time : %d sec"), l_sar_info->sarControl.waitTime);
        l_str_sar_info_text += l_str_wait_time ;
    }

    return l_str_sar_info_text;
}


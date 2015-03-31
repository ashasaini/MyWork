//TMSC-Redmine-783
//TMSC-Redmine-780
//Redmine-775
//Redmine-777
//Redmine-774
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMListCtrl.h
 *  Overview: Structure of CPQMListCtrl class.
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
#ifndef __SM_PQM_LISTCTRL_H__
#define __SM_PQM_LISTCTRL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <atlbase.h>
#include <MPlusUICommon/ListCtrlEx.h>
#include <MPlusUICommon/PopupMenuEx.h>
#include <libStudyManager/NonCopyable.h>
#include <Dictionary/DicDictionaryManager.h>
#include <PQM/IPCCom.h>

using namespace MPlus::UICommon ;

//-------------------------------------------------------------------------
//+Patni-Manish/2009May26/Added/JFT#IR18/GUI Renewal
//+Patni-RUP/2010Jul19/Modified/TMSC-REDMINE-466
/*
#define AUTO_VOICE_OPTION		0
#define CONTRAST_AGENT_OPTION	1
#define AUTO_TRANSFER_OPTION	2
#define BREATH_HOLD_OPTION		3
#define MOVE_COUCH_OPTION		4
*/
#define AUTO_VOICE_OPTION		0
#define MOVE_COUCH_OPTION		1
#define CONTRAST_AGENT_OPTION	2
#define AUTO_TRANSFER_OPTION	3
#define BREATH_HOLD_OPTION	    4
//-Patni-RUP/2010Jul19/Modified/TMSC-REDMINE-466
//-Patni-Manish/2009May26/Added/JFT#IR18/GUI Renewal


//SM3 Defect fix 619 Start Ashish
typedef struct {
    int param_count;
    int column_number;
    CString strConvertedValue;
} SeveralCols;

typedef struct {
    CString strParameterLabel;

    std::vector<SeveralCols*> v_SeveralColumns;
} SeqInfo;
//SM3 Defect fix 619 End Ashish


//! The class is used to create list control and display protocol data

class CPQMListCtrl : public ListCtrlEx,
    private MR::ACQ::SM::COMMON::NonCopyable<CPQMListCtrl>
{
public:
    CPQMListCtrl();
    virtual ~CPQMListCtrl();

    void DisplayTRSC(const int series_index, const CString& str_trsc);
    BOOL UpdateProtocolStatus(const int selected_index, const int status);
    bool CreateListCtrl(const UINT id, const CRect& rect, CWnd* parentwnd);
    bool DeleteSelectedProtocols();
    int InsertProtocolAt(CSequence* sequence_to_insert, int index_at_which_to_insert = -1, const bool select_item = true);

    void DuplicateSelectedItems(const bool f_append = false);
    int DuplicateSequenceforIndex(const int index_being_duplicated, int new_index_for_duplicated_protocol = -1,
                                  const int offset_index = -1, const int f_group_id = 0);

    BOOL IsValidSequenceIndex(const int index)const;
    void InvalidateItemsForModeOption(const int* const items_to_invalidate, const int item_count, const int mode_option_index = -1) ;
    void InvalidateItemsForModeOption(const int items_to_invalidate, const int mode_option_index = -1) ;
    void InvalidateItemsForSarRegion(const int* const items_to_invalidate, const int item_count) ;
    void InvalidateItemsForSarRegion(const int items_to_invalidate);
    void InvalidateItemsForScanTime(const int* const items_to_invalidate, const int item_count) ;
    void InvalidateItemsForScanTime(const int items_to_invalidate) ;
    void InvalidateItemsForStatus(const int* const items_to_invalidate, const int item_count) ;
    void InvalidateItemsForStatus(const int items_to_invalidate) ;
    void InvalidateItemsForWSEEditingColumn(const int items_to_invalidate) ;
    void InvalidateItemsForPLANColumn(const int items_to_invalidate) ;
    void InvalidateItemsForDelayTime(const int* const f_items_to_invalidate, const int f_item_count);
    void Init();
    bool IsRowSelected(HWND hWnd, const int row)const;
    void Clear();
    void UpdateSequences(const int* const sellist, const int count);
    void SelectOnlyItem(const int index);
    void Dictinstance();
    void MakeCurrentWSEEditingProtocolToPlanned();
    void ResetLastPlannedProtocol();
    void ResetWSEEditingProtocolIndex()const;
    int GetselectedItems(int** selected_items, bool* iswait_selected = NULL, bool* iscurrent_selected = NULL,
                         bool* isdone_selected = NULL, bool* isfail_selected = NULL,
                         bool* isfirstwait_selected = NULL,
                         //+Patni-HEMANT/ADDED On 8/12/2009 1:44:12 PM/ MVC#4232
                         bool* iswse_eidted_protocol_selected = NULL,
                         bool* iscoil_selected_for_all = NULL
                                 //-Patni-HEMANT/ADDED On 8/12/2009 1:44:12 PM/ MVC#4232
                        );
    int GetFirstWaitProtocolIndex(const bool f_consider_scan_index = false)const;
    int GetFirstWaitProtocolForAutoPlan()const;
    void GetAppDictionaryConvertedValueList(const int selected_Index);

    bool GetItemsForAutoVoiceAndMoveCouch(const int f_focus_item, int** f_items, int& f_count, const int f_option = -1);

    //it will be used by other class so making it public
    CString GetCellString(const CPoint& cell_index);

    int GetLastPlannedProtocol()const;
    void SetLastPlannedProtocol(const int f_last_edited_protocol);

    int GetWSEEditingProtocolIndex()const;
    void SetWSEEditingProtocolIndex(const int wse_editing_protocol_index);

    int GetBitmapResourceIDForSARRegion(const int row_index);

    int GetFirstSelectedItem(bool* mulitple_selection = NULL)const;
    CPoint GetRightClickCell();
    std::vector<SeqInfo*> GetPairedVector();

    int GetProtocolNumberCol()const;

    void SetSequenceListPtr(CSeqList* list);
    int  GetBiasValueForProtocolNumber()const;
    void SetBiasValueForProtocolNumber(const int bias_value_for_protocol_number);
    void SetChildStatusInSequence(const int f_start_index, const int* const selected_items, const int f_count, const CHILD_THREAD_STATUS f_child_thread_status)const;
    bool IsSelectedItemsInSequence();
    bool CheckAMBContextMenuStatus();
    bool IsAMBFlagEnable();
    void InvalidateItemsForSARControl();
    void CheckAndEndDragging();

protected:
    //{{AFX_VIRTUAL(CPQMListCtrl)
    virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
    //}}AFX_VIRTUAL

    // Generated message map functions

    //{{AFX_MSG(CPQMListCtrl)
    afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);//added by Hemant
    afx_msg BOOL OnToolTipText(UINT id, NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnCaptureChanged(CWnd* pWnd);
    LRESULT HideContexMenu(WPARAM wParam, LPARAM lParam);
    //}}AFX_MSG

private:

    enum EScrollDirection {
        scrollNull,
        scrollUp,
        scrollDown
    };
    EScrollDirection	m_ScrollDirection;
    DicAppDictionaryDetailList   m_app_dic_converted_value_list;
    DicAppDictionaryDetailList  m_app_dic_converted_value_list_seq_info;

    CImageList*			m_pDragImage;
    int m_number_column_alignment ;
    int	m_scan_id_column_alignment;
    CPoint m_rightclick_cell;
    int m_last_planned_protocol ;

    CDWordArray			m_anDragIndexes;
    int					m_nDropIndex;
    int					m_nPrevDropIndex;
    UINT				m_uPrevDropState;
    DWORD				m_dwStyle;
    bool				m_isdraging;
    UINT				m_uScrollTimer;

    CUIntArray m_column_default_width_array;
    int m_bias_value_for_protocol_number;
    CFont m_tooltip_font;
    CFont m_plan_icon_font;
    std::vector<SeqInfo*> m_vSeqInfo;
    int m_trsc_index;
    unsigned int m_mode_column_index;
    unsigned int m_protocolno_column_index;
    unsigned int m_status_column_index;
    unsigned int m_time_column_index;
    unsigned int m_sar_column_index;
    unsigned int m_scan_id_column_index ;
    unsigned int m_plan_column_index;
    unsigned int m_delay_column_index;

    CDictionaryManager* m_dic_manager;
    CSeqList* m_list;
    WCHAR*	  m_pwchtip;

    virtual int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
    virtual void MeasureItem(LPMEASUREITEMSTRUCT measure_item_struct);
    virtual void DrawItem(LPDRAWITEMSTRUCT draw_item_struct);
    void OnModeOptionClicked(const int row_index, const int mode_option_index)const;

    bool IsDragging() const {
        return m_isdraging ;
    }
    void DropItem();
    void RestorePrevDropItemState();
    void UpdateSelection(const int nDropIndex);
    void KillScrollTimer();
    bool IsSequenceIndexValid(const int index)const;
    bool DragDropListItems(const int drag_index, int drop_index);
    void InvalidateItemForColumn(const int row_index, const int column_index);
    void EnableHighlighting(const int row, const bool bHighlight);
    CImageList* CreateDragImageEx(LPPOINT lpPoint, const CString& f_str_scanID, const int f_acq_status) ;
    void DrawModeCellData(CDC* dc, const int row_index);
    void DrawStatusCellData(CDC* dc, const int row_index);
    void DrawSARCellData(CDC* dc, const int row_index);
    void DrawIcon(CDC* f_dc, const int f_row_index, const int f_col_index, const UINT f_icon_id, CRect f_icon_rect);
    void DrawIconForRect(CDC* f_dc, const UINT f_icon_id, const CRect& f_icon_rect)const;
    void DrawPLANCellData(CDC* dc, const int row_index, const int f_col_index);
    void DrawImageWith(CDC* dc, const UINT bitmap_resourceid, const CPoint& cell_index, CRect rect_tocenter = CRect(0, 0, 0, 0), const bool horizontal_center = true, const  bool vertical_center = true) ;
    void CenterRectForRowIndex(const int row_index, const int col_index, CRect& rect_tocenter, const bool forwidth = true, const bool forheight = true) ;
    void DrawTrans(HDC hDC, const int x, const int y, const UINT image_resourceid, const CRect& display_rect);
    void CreateMask(HDC hDC, HBITMAP& m_hbmMask, CBitmap& m_thumb_image, const CRect& display_rect);
    bool CanSetTextItemForPameterString(const CString& parameter_string)const;
    void OnSarCellClicked(const int row_index)const;
    int PerformActionOnMouseClickAt(const CPoint& point);
    void PerformActionOnRButtonClickAt(const CPoint& point);
    void PerformActionOnRButtonClickForModeOptions(const CPoint& point_in_client, const CPoint& cell_number)	;
    void PerformActionOnDblClickAt(const CPoint& point);
    int PerformActionOnModeCellClick(const CPoint& point_in_client, const CPoint& cell_number);
    void UpdateProtocolContextMenu(PopupMenuEx* menu, const int row_index);
    void UpdateAutoVoiceContextMenu(PopupMenuEx* menu, const int row_index);
    bool UpdateContrastAgentContextMenu(PopupMenuEx* menu, const int row_index);
    void ApplyParameterDictionary(const SEQUENCE_PARAM& f_seq_param,
                                  DicParamConvertedValueList&  f_converted_value_list,
                                  DicParamNameList&           f_param_name_list,
                                  Data_decl_list& f_sequence_param_name_list,
                                  const bool f_seq_info = false
                                 );

    void FormSeqParamType(const DATA_DECL& f_decl, VALUE f_val, _variant_t& f_param_value) const;
    void ApplyDisplayDictionary();
    void StoreColumnIndex(const int coulm_index, const CString& parameter_string);
    const CString IncludeSpaceUsingAlignment(const CString& l_input_string, const int l_alignment)const;
    void CompleteDragDrop(const bool f_cancel = true);
    void UpdateMoveCouchContextMenu(PopupMenuEx* f_menu, const int f_row_index);


    bool CanShowMasterSlavePlanMenu(const int f_row_index);
    void UpdatePlanMasterSlaveContextMenu(PopupMenuEx* f_menu, const int f_protocol_index);
    bool IsAMBPlnProtocolSelected(int* f_selected_index, const int f_count);

    int GetBitmapResourceIDForStatus(const int row_index);
    int GetIconIDforStatus(CSequence* f_seq_struct, const bool f_flag_auto_scan);
    int GetBitmapResourceIDForPLANNEDProtocol(const int row_index, CString& f_plantxtstr);
    CRect GetOptionRectForModeOptionIndex(const int row_index, const int mode_option_index);
    int GetBitmapResourceIDForModeOption(const int row_index, const int mode_option_index)const;
    bool GetHeightWidthForIcon(const int f_IconId, int& f_width, int& f_height);
    int GetAutoVoiceOptionForID(const AUTOVOICE_OPTIONS auto_voice_option)const;
    const CString GetAutoVoiceStringForID(const AUTOVOICE_OPTIONS auto_voice_option)const;
    const CString GetToolTipTextForCell(const CPoint& cell_index, CDC* f_dc, CRect& f_tooltip_text_rect);
    CSequence* GetSequenceStructForIndex(const int index)const;

    //Patni-Sudhir/2011Jan27/Added/Redmine-434
    CString GetNRFilterStringForProtocol(const int l_acqOrder, const int l_NRFLG);
    const CString GetEMTFilterStringForProtocol(const int f_acqOrder)const;

    const CString GetToolTipValueFor(const CString f_param, const CString f_value, CDC* f_dc, CRect& f_tooltip_text_rect) const ;
    const CString GetContrastAgentToolTipStr(const int f_row_index, CDC* f_dc, CRect& f_tooltip_text_rect)const;
    const CString GetMoveCouchStringForIndex(const int f_row_index, CDC* f_dc, CRect& f_tooltip_text_rect)const;
    int GetApplicationDictionaryAlignment(const int f_align)const;
    const CPoint GetCellNoFromPoint(const CPoint& point_in_client);
    CRect GetImageRectForResourceID(const int f_resource_id);
    const CString GetAutoVoiceStringForIndex(const int row_index, CDC* f_dc, CRect& f_tooltip_text_rect)const;
    const CRect GetCellRect(const int row_no, const int col_no);
    void SetScrollTimer(const EScrollDirection ScrollDirection);
    int GetIconIDFromStatus(const int f_acqatatus)const;
    void DrawTextWithUpdatedStringAndFormate(const int f_item_number, const unsigned int f_column_id,
            const int f_col_alignment, CDC& f_cdc, CBrush& f_bg_brush, CString f_cell_string);
    CString OnToolSARSAROverinformation(const int f_seq_index) const;


    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #ifndef __SM_PQM_LISTCTRL_H__

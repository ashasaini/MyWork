//TMSC-REDMINE-783
//Redmine-775
//TMSC-REDMINE-777
//TMSC-REDMINE-780
// ScanCommGeneration.h: interface for the CScanCommGeneration class.
//
//////////////////////////////////////////////////////////////////////

//Redmine-777
#ifndef __SM_SCAN_COMM_GENERATION_H__
#define __SM_SCAN_COMM_GENERATION_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <MPlusUICommon/ButtonEx.h>
#include <MPlusUICommon/DialogEx.h>
#include <MPlusUICommon/ListBoxEx.h>
#include <MPlusUICommon/EditCtrlEx.h>
#include <libStudyManager/NonCopyable.h>
#include <libStudyManager/UI/ZOrderWindow.h>

#include <PQM/PQMCommonStructures.h>
#include "CustomStatic.h"

using namespace MPlus::UICommon ;


class CPQMCommentDlg ;

class CScanCommGeneration : public MR::ACQ::SM::COMMON::UI::CZOrderDialog,
    private MR::ACQ::SM::COMMON::NonCopyable<CScanCommGeneration>
{
public:
    CScanCommGeneration(CWnd* pParent, ScanCommentXMLData* f_scan_dialog_data);
    virtual ~CScanCommGeneration();

    void RefreshDialogCtrls(const CString& f_comm_str);

    void EnableDisableApply(const bool f_apply_state);

protected:
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CScanCommGeneration)
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL PreTranslateMessageImpl(MSG* msg);

    //}}AFX_VIRTUAL

    // Generated message map functions
    //{{AFX_MSG(CScanCommGeneration)
    virtual void OnOK();
    virtual BOOL OnInitDialogImpl();
    afx_msg void OnSelchangeListSeq();
    afx_msg void OnSelchangeListCont();
    afx_msg void OnSelchangeListOpt();
    virtual void OnDestroyImpl();

    afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) ;
    afx_msg void OnSize(UINT nType, int cx, int cy) ;

    //}}AFX_MSG

private:
    static CRect m_orgwindowrect;
    CPQMCommentDlg* m_parent_wnd;
    ScanCommentXMLData* m_scan_dialog_data;
    CRect m_org_client_rect;

    //TA Review Comment_6 - member variables private
    EditCtrlEx	m_comm_val;
    ListBoxEx	m_opt_list;
    ListBoxEx	m_cont_list;
    ListBoxEx	m_seq_list;
    ButtonEx	m_ok_button;
    ButtonEx	m_cancel_button;
    CCustomStatic	m_opt_static;
    CCustomStatic	m_cont_static;
    CCustomStatic	m_seq_static;

    virtual void OnCancel();
    void InitializeListBox(ListBoxEx* f_listbox, CList<CString, CString>* f_seq_data);
    void LoadScanCommentData();
    void CheckListItem(const CString& f_str);
    bool CheckListItem(ListBoxEx* f_list, const CString& f_str)const;
    void UnCheckListItems();
    void UnCheckListItems(ListBoxEx* f_list);
    void UpdateScanCommData();
    void UpdateScanCommData(const ListBoxEx* f_list, const UINT* f_checked_items, const int f_max_items, CString* f_str);

    //+For Resizing.
    void Resize(const int f_cx , const int f_cy);
    void OffsetChild(const UINT f_ctrlid , const CSize& f_offset);
    void IncrementSizeChild(const UINT f_ctrlid , const CSize& f_inc_size);
    //-For Resizing.

    //+For Horz Scrollbar
    void InitHorzBar();

    void SetHorzBar(ListBoxEx* f_listbox, CList<CString, CString>* f_list);
    //-For Horz Scrollbar
    void SetDialogRibbonHeight();
    void SetMultilingual();
    void SetButtonAppearence();

    DECLARE_MESSAGE_MAP()
};

#endif // #ifndef __SM_SCAN_COMM_GENERATION_H__

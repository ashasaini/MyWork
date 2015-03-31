//TMSC-REDMINE-783
//TMSC-REDMINE-781
//TMSC-Redmine-780
//Redmine-775
//Redmine-777
//Redmine-774
#ifndef __SM_COIL_LST_DLG_H__
#define __SM_COIL_LST_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CoilLstDlg.h : header file

#include <MPlusUICommon/PopupMenuEx.h>
#include <MPlusUICommon/DialogEx.h>
#include <MPlusUICommon/ButtonEx.h>
#include <libStudyManager/NonCopyable.h>
#include <libStudyManager/UI/ZOrderWindow.h>

#include "CoilSelectionUtility.h"
#include "CustomStatic.h"

using namespace MPlus::UICommon;

#define COMBINE_WB_MASK				0x0F


//Necessary code ends here
/////////////////////////////////////////////////////////////////////////////
// CCoilLstDlg dialog


class CCoilSelWinInterface
{
private:
    virtual void DisplayButtonsOnAxisLocator() = 0;
};



class CCoilLstDlg : public MR::ACQ::SM::COMMON::UI::CZOrderDialog,
    public CUIClassInterfaceForCoilListDialog,
    private MR::ACQ::SM::COMMON::NonCopyable<CCoilLstDlg>
{


    // Construction
public:
    CCoilLstDlg(CCoilSelectionUtility* selection_utiliy, CCoilSelWinInterface* ccoilselwininterface, CWnd* pParent = NULL);   // standard constructor
    virtual ~CCoilLstDlg();

    void DisplayCoilWithConnectCoilCheck();
    void DisplayCoilInformation(const bool selected_only = false, bool* check_combine = NULL, const bool f_reset_float_pos = false);
    void EnableDisableDialogState(const BOOL f_state);
    void SetWindowCaption(const CString& f_window_caption);
    void SetWindowCaption();
    void SetWaitCursor(const bool f_show_wait_cursor);

protected:
    //{{AFX_VIRTUAL(CCoilLstDlg)
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Generated message map functions
    //{{AFX_MSG(CCoilLstDlg)
    virtual BOOL OnInitDialogImpl();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnButtonApply();
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    //}}AFX_MSG

private:
    BUTTON_COLORS_t m_btnDefColor;
    CCoilSelWinInterface* m_ccoilselwininterface;
    CCoilSelectionUtility* m_selection_utiliy;
    CString m_window_caption;
    bool m_show_wait_cursor_coillistdlg;

    // Dialog Data
    //{{AFX_DATA(CCoilLstDlg)
    //enum { IDD = IDD_DIALOG_COIL_LST };
    /*CButton*/
    ButtonEx	m_btnCancel;
    /*CButton*/
    ButtonEx	m_btnOK;
    /*CButton*/
    ButtonEx	m_btnApply;
    /*CButton*/
    ButtonEx	m_btnPort[MAX_COUCH_PORT];
    /*CButton*/
    ButtonEx	m_btnCoil[10];
    /*CButton*/
    ButtonEx	m_btnPos[9];
    CListBox    m_lstPort;

    CCustomStatic m_marker_sign_statics[MAX_COIL_BUTTON];

    //}}AFX_DATA

    PopupMenuEx* pGPortMenu;

    CList<PopupMenuEx*, PopupMenuEx*> m_pgportsubmenu_list;
    CList<PopupMenuEx*, PopupMenuEx*> m_paportsubmenu_list;
    PopupMenuEx* pLPortMenu[2];
    PopupMenuEx* pAPortMenu;
    PopupMenuEx* pPosMenu;
    PopupMenuEx* pPortMenu;

    UINT ubtnID;
    int iLIndex;
    int iAIndex;

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CCoilLstDlg)
    virtual BOOL PreTranslateMessageImpl(MSG* pMsg);
    //}}AFX_VIRTUAL

    void OnComboPos(UINT uPosId);
    void OnComboCoil(UINT uCoilId);
    void OnComboPort(UINT uPortId);
    void OnACoilMenu(UINT uCoilID);
    void OnLCoilMenu(UINT uID);
    void OnGCoilMenu(UINT uID);
    void OnPortMenu(UINT uPortID);
    void OnPosMenu(UINT uPosID);
    virtual void CheckCombineFailedFor(const int select_coil_index)const;
    virtual void UiSetMenuDefaultColorForCouch(const ACoilButtonType f_acoilbuttontype, const ACoilItems f_acoilitems);
    virtual void UiSetMenuWarningColorForCouch(const ACoilButtonType f_acoilbuttontype, const ACoilItems f_acoilitems) ;

    //for Menus :
    void CreateCoilMenu();
    void CreatePosMenu(const int* const pPos, CoilSelectionSelectCoil_s* f_coil_struct);
    void DisplayMenu(PopupMenuEx* pPQMMenu, const UINT uId);
    void CleanUp();
    void UpdatePortMenu(CString sMenuText, const UINT iMenuItemID);
    void UpdatePosMenu(const CString& sMenuText, const UINT iMenuItemID)const;
    void UpdateLCoilMenu(const CString& sMenuText, const UINT iMenuItemID)const;
    void UpdateGCoilMenu(const CString& sMenuText, const UINT iMenuItemID)const;
    void MaintainPortMenu();
    void CreateLCoilMenu();
    void CreateACoilMenu();
    void CreateGCoilMenu();
    void DisplayCoilFor(const CoilSelectionSelectCoil_t* f_local, const int f_coil_button_index)const;
    void EnableDisableCoilButtonsFor(const int f_coil_button_index, const bool f_enable)const;
    void EnableDisableGCoilButtonsFor(const GCoilItems f_gcoilitem, const bool f_enable)const;
    void EnableDisableLCoilButtonsFor(const LCoilItems f_lcoilitem, const bool f_enable)const;
    void EnableDisableACoilButtonsFor(const ACoilItems f_acoilitem, const bool f_enable)const;
    void ClearCoilInformation(const CoilTypes coiltypes);
    status_t CoilMismatchOKCancel(const bool f_ok = true);
    bool IsPortOccupied(const int f_port_number_onebase)const;
    void PreparePortMenu() ;
    void UpdateHighlightColorsForPositionCombos();
    void CreatePositionMenu();
    void searchSelPosition(const long location)const;
    void ShowConflictsFor(const CoilSelectionSelectCoil_t* f_newcoil_selected, const CoilSelectionSelectCoil_t* f_oldcoil_selected, const int f_aport_button_index);
    void RemoveMenuItemFromMenu(CMenu* f_menu , const CString& f_menu_item_to_remove)const;
    void UpdateACoilMenu();
    void ResetCoilDataForDisplayInformation(CoilSelectionSelectCoil_t* f_local);
    void DisplayGCoilFrom(const CoilSelectionSelectCoil_t* f_local, const GCoilItems f_gcoilitem)const;
    void DisplayLCoilFrom(const CoilSelectionSelectCoil_t* f_local, const LCoilItems f_lcoilitem)const;
    void DisplayACoilFrom(const CoilSelectionSelectCoil_t* f_local, const ACoilItems f_acoilitem)const;
    void DisplayGLCoil(const int f_button_id, const CoilSelectionSelectCoil_t* f_local) const ;
    void ClearSelectedCoilInformation();
    bool CoilUiListDlgOkCB(const bool f_ignore_pos_change = false)const;
    void UpdateMarkerWindows();
    bool ApplyCoilInformation()const;
    void PrepareButtonClickIDAndCoilTypeClickedID(const int f_button_id, const bool f_for_port = false);
    void ClearGCoilMenu();
    void ClearACoilMenu(); //Patni-AMT/2009Nov10/Added/DeFT/MCM0229-00154
    CoilSelectionCoil_t* GetSelectionCoilFromCouchDataFor(const CString& f_coil_name)const;
    int GetOccurencesForCoil(const CoilSelectionSelectCoil_t* f_coil, int** f_same_coil_index);
    void SetMultilingual(); // Added By KT/ACE-2/11-06-2009/Multilingualisation
    BOOL SetButtonFont();
    BOOL SetBitmapForButton(ButtonEx& button_object, const UINT bitmap_resource_id, const bool center_bitmap = true, const bool right_align = false)const;

    int GetSameCoilCount(const CString& sCoil)const;
    CString GetStringForMarkerSign(const MarkerSignEnum markersignenum)const;
    int GetButtonIDFor(const GCoilItems f_gcoilitem)const;
    int GetButtonIDFor(const LCoilItems f_lcoilitem, const LCoilButtonType lcoilbuttontype = LCoilCoilButtons)const ;
    int GetButtonIDFor(const ACoilItems f_acoilitem, const ACoilButtonType acoilbuttontype = ACoilCoilButtons)const ;
    ButtonEx* GetCouchButton(const ACoilButtonType f_acoilbuttontype, const ACoilItems f_acoilitems);
    int GetBaseIndexFor(const CoilTypes coiltypes)const ;

    void SetDefaultColor(ButtonEx* pbtn);
    BOOL SetColorButton(ButtonEx* pbtn, const COLORREF cColor)const;
    void SetDialogRibbonHeight();
    void SetColorForMarkerStatic(const int f_marker_index);
    void SetButtonTextForID(const UINT f_button_id, const CString& f_button_string)const;

    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #ifndef __SM_COIL_LST_DLG_H__

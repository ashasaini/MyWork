//TMSC-REDMINE-783
//TMSC-REDMINE-780
//Redmine-775
//Redmine-777
//TMSC-REDMINE-774
#ifndef __SM_COIL_SEL_WIN_H__
#define __SM_COIL_SEL_WIN_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*#ifdef _AFX_COIL_RIGHT
	#define IDD_COIL_SELECTION_WINDOW IDD_COIL_SELECTION_WINDOW_RIGHT
#endif

#ifndef _AFX_COIL_RIGHT
#define IDD_COIL_SELECTION_WINDOW IDD_COIL_SELECTION_WINDOW_LEFT
#endif
*/

// CoilSelWin.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCoilSelWin dialog

//Necessary code for using MPlusUICommon Library
#include <MPlusUICommon/DialogEx.h>
#include <MPlusUICommon/ButtonEx.h>

#include <libStudyManager/NonCopyable.h>
#include <libStudyManager/UI/ZOrderWindow.h>

#include "CoilLstDlg.h"
#include "CustomStatic.h"
//Patni-HEMANT/ADDED On 8/12/2009 10:24:25 AM/ Restructure
#include "InterfaceClasses.h"
#include "CoilSelectionUtility.h"

#define CHANNEL_MODE_NORMAL     0
#define CHANNEL_MODE_MAX        1
using namespace MPlus::UICommon;

//Necessary code ends here
//class CCoilSelectionUtility;
class CPQMView;

class CCoilSelWin : public MR::ACQ::SM::COMMON::UI::CZOrderDialog,
    public CCoilSelWinInterface ,
    public CUIClassInterfaceForCoilSelDialog,
    private MR::ACQ::SM::COMMON::NonCopyable<CCoilSelWin>
{
public :
    CCoilSelWin(CCoilSelectionUtility* selection_utiliy, CBaseAxisLocatorDialogInterface* cbaseaxislocatordialog_interface, const UINT dialog_id , CWnd* pParent = NULL, const COUCH_POSITION couch_position = COUCH_POSITION_LEFT);  // standard constructor
    ~CCoilSelWin();

    void ChangeViewOfCoilListDlg(const bool f_bCheck)const;
    void DisplayCoil() ;

    void ShowHideOpenedWindowsOnPageChanged(const bool f_show /*= true*/);
    void CoilSelSetCenterPos();
    int GetSelectedChannelMode() const;
    void SetSelectedChannelMode(const int f_mode);
    void EnableDisableDialogState(const BOOL f_state);

    CCoilSelectionUtility* GetSelectionUtility()const;
    void SetWindowCaption(const CString& f_window_caption, const bool f_applychange = false);

    int GetGUISectionWidthDefault() const;
    COUCH_POSITION GetCouchAxis() const;
    void SetWaitCursor(const bool f_show_wait_cursor);

protected:
    //{{AFX_VIRTUAL(CCoilSelWin)
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Generated message map functions
    //{{AFX_MSG(CCoilSelWin)
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnClose();
    afx_msg void OnButtonInvokeCoil();
    afx_msg void OnSpineCoilMoveLeft();
    afx_msg void OnSpineCoilMoveRight();
    afx_msg void OnFreeCoilMoveLeft();
    afx_msg void OnFreeCoilMoveRight();
    virtual void OnDestroyImpl();
    afx_msg void OnButtonClickCoilChannelMode();
    afx_msg void OnClickMenuItemNormal();
    afx_msg void OnClickMenuItemMax();
    //Button handlers
    afx_msg void OnGantry();
    virtual BOOL OnInitDialogImpl();
    afx_msg void OnCouch(UINT uID);
    afx_msg void OnCouchSB(UINT uID);
    afx_msg void OnLinear(UINT uID);
    afx_msg void OnLButtonDown(UINT flags, CPoint point);
    afx_msg void OnLButtonUp(UINT flags, CPoint point);
    afx_msg void OnMouseMove(UINT flags, CPoint point);
    afx_msg void OnPaint();
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    //}}AFX_MSG

private :
    enum CoilMove { CoilMove_Left, CoilMove_Right, CoilMove_None};
    COUCH_POSITION m_couch_position;
    BUTTON_COLORS_t m_btnDefColor;

    bool m_store_visible_state_for_coil_list ;		//Added by Hemant On 1/8/2009 5:45:33 PM
    CBaseAxisLocatorDialogInterface* m_cbaseaxislocatordialog_interface ;
    CString m_window_caption;
    CStatic m_main_image;
    CBitmap m_coil_main_image;
    int m_highlight_1 ;
    int m_highlight_2 ;
    int m_selected_channel_mode;
    bool m_show_wait_cursor_for_coilselwin;

    //{{AFX_DATA(CCoilSelWin)
    ButtonEx	m_btnCancel;
    ButtonEx	m_btnOK;
    ButtonEx	m_btnSCRight;
    ButtonEx	m_btnSCLeft;
    ButtonEx	m_btnInvokeCoil;
    ButtonEx	m_btnFCRight;
    ButtonEx	m_btnFCLeft;
    //Patni-HEMANT/ADDED On 8/27/2009 12:28:15 PM/ Spine Coil Req
    ButtonEx	m_btnSCMiddle;
    ButtonEx	m_btnCouch[MAX_SECTION_BUTTON];
    ButtonEx    m_btnCouch_sb[4];    //Patni-HAR/2009Nov24/DeFT/IR-110
    ButtonEx	m_btnLabel[MAX_COIL_BUTTON];
    ButtonEx	m_btnL[2];
    ButtonEx	m_btnGantry;
    ButtonEx    m_btnCoilChannelMode;
    CCustomStatic m_channel_mode;
    CCustomStatic	m_edtMsg;
    //}}AFX_DATA

    CCoilLstDlg* m_p_dialog_coil_list;
    CCoilSelectionUtility*			m_selection_utiliy;

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CCoilSelWin)
    virtual BOOL PreTranslateMessageImpl(MSG* pMsg);
    //}}AFX_VIRTUAL

    virtual void UpdateWarningMsgWindowWith(const int f_msgstring_id);
    virtual void UpdateMsgWindowWithMsgOK();
    virtual void UiCheckCombine();
    virtual void CoilSelectionUiMessage(const int f_message_id);
    virtual void SelectCoilUiSetSelectColorForIndex(const int f_coil_buttonindex) ;
    virtual void SelectCoilUiSetDefaultColorForIndex(const int f_coil_buttonindex);
    virtual void SelectCoilSetWarningButtonColorForIndex(const int f_coil_buttonindex) ;
    virtual void SelectCoilUiSetLableForSelectionCoilIndex(const int f_coil_buttonindex, const CString& f_lable);
    virtual void SelectCoilUiSetSensitiveButton(const int f_coil_buttonindex, const bool f_enable) ;
    virtual void SelectCoilEnableDisableButton(const int f_select_buttonindex, const bool f_enable) ;
    virtual void SectionCoilSetSelectColorForButton(const int f_section_buttonindex, const bool f_redraw_button = false) ;
    virtual void SectionCoilSetDefaultColorForButton(const int f_section_buttonindex, const bool f_redraw_button = false) ;
    void DisplayButtonsOnAxisLocator();

    virtual void SectionCoilSetWarningButtonColor(const int f_section_buttonindex) ;
    virtual void SectionCoilUpdateSectionSize(const int f_section_buttonindex, CoilSelectionSectionButton_t* f_section_btn);
    virtual void SectionCoilSetLableForButton(const int f_section_buttonindex, const CString& f_lable);
    virtual void SectionCoilSetSensitiveButton(const int f_section_buttonindex, const bool f_enable) ;
    virtual void SectionCoilEnableDisableButton(const int f_section_buttonindex, const bool f_enable) ;

    void UpdateMsgWindowWith(const int f_msgstring_id) ;
    void CoilSelectionUiSetWarningColor(const int f_coil_button_index);
    void SectionCoilButtonUiOnCoilPB(const int n, const bool flg);
    void SectionCoilButtonUiOffCoilPB(const int n, const bool flg);
    void SectionCoilButtonUiLink(const int n);
    void CreateCoilListDialog(const bool f_show = false);
    void ReflectDataToListDialog()const;
    bool IsHighlighted(int f_index) const;
    void CoilSelectionUiMapCenterPos(const float f_center_pos);
    void MoveFreeCoils(const CCoilSelWin::CoilMove f_move_free_coil);
    void MoveSpineCoils(const CCoilSelWin::CoilMove f_movespine);
    bool UisetCoilButtonPosition();
    void Cleanup();
    void UiChangeMessage();
    BOOL UiCheckContinuousness(const int f_count)const;
    BOOL UiCheckSectionCombine(const int f_count)const;
    BOOL UiCheckCoilCombine(const int f_count)const;
    BOOL UiCheckSelectCoil();
    void JudgCoilPB(const bool f_flg);
    void ResetHighlight();
    void InitializeChannelMode();

    CString GetStringForID(const int f_msgstring_id)const ;
    BOOL SetButtonStyle();
    BOOL SetBitmapForButton(
        ButtonEx& button_object,
        const UINT bitmap_resource_id,
        const bool center_bitmap = true
    );
    void SetEnableDisableButtonColor(ButtonEx& f_button , const bool f_enable = true, const bool f_redraw_button = false);
    void SetMultilingual(); // Added By KT/ACE-2/11-06-2009/Multilingualisation

    CoilSelectionGlobal_t* GetCoilSelectionGlobal()const;
    CRect GetMainImageRect();
    int GetBaseIndexFor(const CoilTypes coiltypes)const;
    int GetBaseIndexForCoilButtonFor(const CoilTypes coiltypes)const ;
    CString GetBitmapPath(const int f_bitmap_id)const ;
    ButtonEx* GetCoilButtonForIndex(const int f_coil_button_index) ;
    ButtonEx* GetSectionButtonForIndex(const int f_coil_button_index);

    void SetWindowCaption();
    void SetButtonAppearenceForLCoilButton(const LCoilItems lcoilitems);
    void SetButtonAppearenceForGCoilButton(const GCoilItems gcoilitems);
    void SetButtonAppearenceForACoilButton(const ACoilItems gcoilitems);
    void SetWarningButtonColor(ButtonEx& f_button);
    BOOL SetColorButton(ButtonEx* pbtn, const COLORREF cColor);
    void SetDialogRibbonHeight();

    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #ifndef __SM_COIL_SEL_WIN_H__

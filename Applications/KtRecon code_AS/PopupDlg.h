// PopupDlg.h: interface for the CPopupDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POPUPDLG_H__154E9B01_69DA_4B7E_99A0_59B825E82D4A__INCLUDED_)
#define AFX_POPUPDLG_H__154E9B01_69DA_4B7E_99A0_59B825E82D4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <MPlusUICommon/DialogEx.h>
using namespace MPlus::UICommon;
/**
* This micro is used for background color of dialogbox.
*/
const COLORREF BKCOLOR = RGB(27, 36, 54);
/**
* This micro is used for background color of frame.
*/
const COLORREF FRAME_BKCOLOR = RGB(70, 80, 100);
/**
* This micro is used for text color.
*/
const COLORREF TEXT_BKCOLOR = RGB(191, 209, 255);
/**
* This micro is used for heading text size.
*/
const int FONT_SIZE = 12;
/**
* This micro is used for text size.
*/
const int TEXT_SIZE = 10;
/**
* This micro is used for text format.
*/
const TCHAR FONT[]  = _T("Arial");
/**
* This micro is used for Menu Creation.
*/
const TCHAR MENU_OPTIONS[]             = _T("Options");
const TCHAR MENU_HELP[]             = _T("Help");
/**
* This micro is used for Menu Creation.
*/
const TCHAR MENU_SETROI[]              = _T("Set ROI");

const TCHAR MENU_ABOUT[]              = _T("About");
/**
* This micro is used for Menu Creation.
*/
const TCHAR MENU_RECON[]             = _T("Reconstruction");
/**
* This micro is used for Menu Creation.
*/
const TCHAR MENU_COPYROI[]             = _T("Copy ROI");

const TCHAR MENU_POLARITYCORRECTION[]  = _T("Polarity Correction"); //priyanka - polarity correction

const TCHAR MENU_ADD_OFFSET[]  = _T("Add Offset");
const TCHAR MENU_MANUAL_PHASE_CORR[]  = _T("Manual Phase Correction");

/**
* Base class for Dialog class.
* This class act as base class for all dialog class which derived form
* DialogEx class of MPlus.
*/
class CPopupDlg : public DialogEx
{
    // Construction
public:
    CPopupDlg(UINT IDD, CWnd* pParent = NULL);  // standard constructor
    
    // Dialog Data
    //{{AFX_DATA(CPopupDlg)
    //enum { IDD = IDD_DIALOG_COPYROI };
    //}}AFX_DATA
    virtual ~CPopupDlg();
    
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPopupDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
    
    // Implementation
protected:
    
    // Generated message map functions
    //{{AFX_MSG(CPopupDlg)
    virtual void OnCancel();
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* msg);
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
    
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
protected:
    CFont m_pLabel_Font;
    CBrush m_list_brush;
    
};

#endif // !defined(AFX_POPUPDLG_H__154E9B01_69DA_4B7E_99A0_59B825E82D4A__INCLUDED_)

/*****************************************************************************
*
*  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
*
*  File Name: DelayTimeDlg.h
*  Overview: Structure of DelayTimeDlg class.
*  Purpose:  NFD002_Demo_CR-3 Delay Edit Dialog
*****************************************************************************
*                       Revision History
*****************************************************************************
*  Revision      Author           Date & Time            Changes
*  ==========================================================================
*
*****************************************************************************/

#if !defined(AFX_DELAYTIMEDLG_H__1BD3B3D6_A077_4C4E_8A33_19B7D1071EA6__INCLUDED_)
#define AFX_DELAYTIMEDLG_H__1BD3B3D6_A077_4C4E_8A33_19B7D1071EA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DelayTimeDlg.h : header file
//
#include <MPlusUICommon/EditCtrlEx.h>
#include <MPlusUICommon/ButtonEx.h>
#include <MPlusUICommon/DialogEx.h>
#include <libStudyManager/NonCopyable.h>
#include <libStudyManager/UI/ZOrderWindow.h>
using namespace MPlus::UICommon ;

#include "CustomStatic.h"
const unsigned int BACKSLASHH = 92;
/////////////////////////////////////////////////////////////////////////////
// CDelayTimeDlg dialog
//+Redmine-2185
struct DelayTimeParameter {
    int m_min_value;
    int m_max_value;
    int m_curr_value;
    int m_increment_value;

    DelayTimeParameter(const int min, const int max)
        : m_min_value(min), m_max_value(max), m_curr_value(0), m_increment_value(INCREASE_BY_NUMBER)
    {}
};

//-Redmine-2185
class CDelayTimeDlg : public MR::ACQ::SM::COMMON::UI::CZOrderDialog,
    private MR::ACQ::SM::COMMON::NonCopyable<CDelayTimeDlg>
{
    // Construction
public:
    explicit CDelayTimeDlg(CWnd* pParent = NULL);   // standard constructor
    virtual ~CDelayTimeDlg();

    int ConvertTimeStrToInt(const CString& f_str);
    CString ConvertIntStrToTimeStr(const CString& f_str);

protected:

    // Generated message map functions
    //{{AFX_MSG(CDelayTimeDlg)
    virtual BOOL PreTranslateMessageImpl(MSG* pMsg);
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialogImpl();
    afx_msg void OnDelayClose();
    afx_msg void OnDelayOk();
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);//Redmine-2185
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    // Dialog Data
    //{{AFX_DATA(CDelayTimeDlg)
    enum { IDD = IDD_DELAY };
    EditCtrlEx m_edit_delay_sec;//Redmine-2185
    EditCtrlEx m_edit_delay_min;//Redmine-2185
    CCustomStatic m_lable_delay_time;
    CCustomStatic m_lable_delay_colon;
    ButtonEx	m_close;
    ButtonEx	m_ok;
    //}}AFX_DATA
    //Redmine-2185
    DelayTimeParameter	m_delay_time_minute;
    DelayTimeParameter	m_delay_time_sec;

    void SetButtonAppearence();
    void SetButtonFont();
    void SetDialogRibbonHeight();
    bool SaveDelayTime();
    void DisplayCurrentDelayTime();
    CString GetCurrentDelayTime();
    void SetMultilingual();
    //+Redmine-2185
    void ProcessKeyDownMsg(MSG* pMsg);
    void IncreaseMinuteByOneUnit();
    void DecreaseMinuteByOneUnit();
    void IncreaseSecondByOneUnit();
    void DecreaseSecondByOneUnit();
    void DisplayCurrentMinuteValue();
    void DisplayCurrentSecondValue();
    //-Redmine-2185
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DELAYTIMEDLG_H__1BD3B3D6_A077_4C4E_8A33_19B7D1071EA6__INCLUDED_)

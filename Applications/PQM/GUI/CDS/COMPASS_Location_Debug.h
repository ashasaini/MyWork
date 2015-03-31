//TMSC-REDMINE-783
//TMSC-REDMINE-782
//TMSC-REDMINE-780
//Redmine-775
//Redmine-777
#ifndef __SM_COMPASS_LOCATION_DEBUG_H__
#define __SM_COMPASS_LOCATION_DEBUG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// COMPASS_Location_Debug.h : header file
//

#include <MPlusUICommon/ButtonEx.h>
#include <MPlusUICommon/DialogEx.h>
#include <libStudyManager/NonCopyable.h>
#include <libStudyManager/UI/ZOrderWindow.h>

#include <PQM/CDSCommonStructures.h>
#include "../CustomStatic.h"


#include "CustomLocDbgEditBox.h"
using namespace MPlus::UICommon ;

/////////////////////////////////////////////////////////////////////////////
// CCOMPASS_Location_Debug dialog

class CCOMPASS_Location_Debug : public MR::ACQ::SM::COMMON::UI::CZOrderDialog,
    private MR::ACQ::SM::COMMON::NonCopyable<CCOMPASS_Location_Debug>
{

public :
    explicit CCOMPASS_Location_Debug(CWnd* pParent = NULL);   // standard constructor

    void GetCompassDebugParam(COMPASS_Debugg_Param  f_debug_param[MAX_PARAM]);
    void SetCompassDebugParam(COMPASS_Debugg_Param  f_debug_param[MAX_PARAM], const bool f_update = false);

    // Overrides
    // ClassWizard generated virtual function overrides
protected:
    //{{AFX_VIRTUAL(CCOMPASS_Location_Debug)
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
    // Generated message map functions
    //{{AFX_MSG(CCOMPASS_Location_Debug)
    virtual BOOL OnInitDialogImpl();
    virtual void OnOK();
    virtual void OnCancel();

    //}}AFX_MSG

private:
    COMPASS_Debugg_Param m_debug_param[MAX_PARAM] ;
    CCustomStatic	m_coil_id[MAX_PARAM] ;
    CCustomStatic	m_port_name[MAX_PARAM] ;
    CCustomLocDbgEditBox 	m_coil_location[MAX_PARAM] ;
    CCustomLocDbgEditBox 	m_detect_flag[MAX_PARAM] ;
    ButtonEx	m_set_button ;
    ButtonEx	m_cancel_button ;

    void InitCtrlForInternationalization();
    void UpdateDialogParams();
    void UpdateDataForIndex(const int f_index);
    void UpdateDataFromDialog();
    void UpdateDataItemFromDialog(const int f_index);
    void SetButtonStyle();

    void SetDialogRibbonHeight();

    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #ifndef __SM_COMPASS_LOCATION_DEBUG_H__

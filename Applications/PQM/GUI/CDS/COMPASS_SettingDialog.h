//Redmine-783
//Redmine-782
//Redmine-775
//Redmine-777
//Redmine-780
#ifndef __SM_COMPASS_SETTING_DIALOG_H__
#define __SM_COMPASS_SETTING_DIALOG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// COMPASS_SettingDialog.h : header file
//

#include <MPlusUICommon/ButtonEx.h>
#include <MPlusUICommon/SliderCtrlEx.h>
#include <MPlusUICommon/FrameEx.h>
#include <MPlusUICommon/DialogEx.h>
#include <libStudyManager/NonCopyable.h>
#include <libStudyManager/UI/ZOrderWindow.h>

#include <PQM/CDSCommonStructures.h>
#include "../CustomStatic.h"

#include "CDSEdit.h"

using namespace MPlus::UICommon ;

/////////////////////////////////////////////////////////////////////////////
// CCOMPASS_SettingDialog dialog

enum AS_COMPASS_PARAM_ENUM { AS_COMPASS_PARAM_MAGNETIC_CENTER_LENGTH , AS_COMPASS_PARAM_COIL_COVERAGE , AS_COMPASS_PARAM_INVALID } ;

class CCOMPASS_SettingDialog : public MR::ACQ::SM::COMMON::UI::CZOrderDialog,
    private MR::ACQ::SM::COMMON::NonCopyable<CCOMPASS_SettingDialog>
{
public :
    explicit CCOMPASS_SettingDialog(CWnd* pParent = NULL);   // standard constructor

    ASCompassSettingParam* GetASCOMPASSParameter(const AS_COMPASS_PARAM_ENUM f_parameter_type);
    void SetASCOMPASSParameter(const AS_COMPASS_PARAM_ENUM f_parameter_type, const ASCompassSettingParam& f_parameter_values);

protected:
    //{{AFX_VIRTUAL(CCOMPASS_SettingDialog)
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL


    // Generated message map functions
    //{{AFX_MSG(CCOMPASS_SettingDialog)
    virtual BOOL OnInitDialogImpl();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnParam1Up();
    afx_msg void OnParam2Down();
    afx_msg void OnParam2Up();
    afx_msg void OnParam1Down();
    LRESULT OnKillFocusEditCtrl(WPARAM wParam, LPARAM lParam);
    //}}AFX_MSG

private:
    ASCompassSettingParam m_dist_from_magnetic_center ;
    ASCompassSettingParam m_min_coverage_for_section ;
    ButtonEx m_ok_button;
    ButtonEx m_cancel_button;
    ButtonEx m_param1_up ;
    ButtonEx m_param2_up ;
    ButtonEx m_param1_down ;
    ButtonEx m_param2_down ;
    CCDSEdit m_edit_param1;
    CCDSEdit m_edit_param2;
    SliderCtrlEx m_slider_param1;
    SliderCtrlEx m_slider_param2;
    FrameEx m_fram_1;
    FrameEx m_fram_2;
    CCustomStatic m_min_param1;
    CCustomStatic m_max_param1;
    CCustomStatic m_min_param2;
    CCustomStatic m_max_param2;

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CCOMPASS_SettingDialog)
    virtual BOOL PreTranslateMessageImpl(MSG* pMsg);
    //}}AFX_VIRTUAL

    void InitCtrlForInternationalization();
    void UpdateEditCtrl(const AS_COMPASS_PARAM_ENUM  f_parameter_type);
    void OnSliderChange(const AS_COMPASS_PARAM_ENUM  f_parameter_type, const UINT nPos);
    void UpdateSliderCtrl(const AS_COMPASS_PARAM_ENUM  f_parameter_type);
    void IncreamentDecrementParameter(const AS_COMPASS_PARAM_ENUM  f_parameter_type, const bool f_increment = true);
    void InitializeCtrlsWithData();
    void UpdateMinMaxStaticText();
    void InitializeSliderCtrls();
    void InitializeSliderCtrl(const AS_COMPASS_PARAM_ENUM  f_parameter_type);
    bool ProcessEditCtrlForEnterKey(MSG* pMsg);
    void OnEditCtrlUpdated(const AS_COMPASS_PARAM_ENUM f_parameter_type);
    void SetButtonStyle();
    void SetBitmapForButton(ButtonEx& f_button_object, const UINT f_bitmap_resource_id, const bool f_center_bitmap = true)const;

    int GetValidatedValueFor(const AS_COMPASS_PARAM_ENUM  f_parameter_type , const int f_value);
    int GetEditCtrlTextValue(const AS_COMPASS_PARAM_ENUM  f_parameter_type);
    CString GetStringValueFor(const AS_COMPASS_PARAM_ENUM  f_parameter_type);
    SliderCtrlEx* GetSlider(const AS_COMPASS_PARAM_ENUM  f_parameter_type);
    EditCtrlEx* GetEditCtrl(const AS_COMPASS_PARAM_ENUM  f_parameter_type);
    int GetSliderPos(const AS_COMPASS_PARAM_ENUM  f_parameter_type);

    void SetDialogRibbonHeight();

    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #ifndef __SM_COMPASS_SETTING_DIALOG_H__

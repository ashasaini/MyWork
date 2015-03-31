//REDMINE_782_CPP_Test_Tool
//TMSC-REDMINE-783
//TMSC-REDMINE-781
//TMSC-Redmine-780
//Redmine-777
//Redmine-774
#ifndef __SM_PATIENT_ORIENTATION_DIALOG_H__
#define __SM_PATIENT_ORIENTATION_DIALOG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PatientOrientationDialog.h : header file
//

#include <MPlusUICommon/ButtonEx.h>
#include <libStudyManager/UI/ZOrderWindow.h>
#include "DailogInterfaces.h"
#include "PatientOrientationConstants.h"

using namespace MPlus::UICommon ;
typedef MR::ACQ::SM::COMMON::UI::TZOrderWindow<CDialog> CZOrderPatientOriDialog;


enum MainOrientationImage {

    MainOrientationImage_default = 0,	//Patni-DKH/2009Aug26/Modified/cpp test correction
    //------------------------
    MainOrientationImage_111 = 111,
    MainOrientationImage_112 = 112,

    MainOrientationImage_121 = 121,
    MainOrientationImage_122 = 122,


    MainOrientationImage_131 = 131,
    MainOrientationImage_132 = 132,

    MainOrientationImage_141 = 141,
    MainOrientationImage_142 = 142,
    //------------------------

    //------------------------
    MainOrientationImage_211 = 211,
    MainOrientationImage_212 = 212,

    MainOrientationImage_221 = 221,
    MainOrientationImage_222 = 222,

    MainOrientationImage_231 = 231,
    MainOrientationImage_232 = 232,

    MainOrientationImage_241 = 241,
    MainOrientationImage_242 = 242,
    //------------------------

    MainOrientationImage_total
};



#define TOTAL_MAIN_IMAGES	8

/////////////////////////////////////////////////////////////////////////////
// CPatientOrientationDialog dialog

class CPatientOrientationDialog : public CZOrderPatientOriDialog
{
public:
    explicit CPatientOrientationDialog(CPatientOrientationInterface* patientorientation_interface, CWnd* pParent = NULL, const bool allow_orientation_change = true, const COUCH_POSITION couch_position = COUCH_POSITION_RIGHT);  // standard constructor
    ~CPatientOrientationDialog();


    void UpdateOrientationAccordingto(const PatientInsertDirection orientationoption1, const PatientRotationDirection orientationoption2, const PatientViewDirection orientationoption3);
    void OnOffOrientationChangeFlag(const bool allow_orientation_change);

    PatientInsertDirection GetPatientOrientationOption1()const;
    PatientRotationDirection GetPatientOrientationOption2()const;
    PatientViewDirection GetPatientOrientationOption3()const;
    void SetWaitCursor(const bool f_show_wait_cursor);

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPatientOrientationDialog)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation

    // Generated message map functions
    //{{AFX_MSG(CPatientOrientationDialog)
    virtual void OnOK();
    virtual void OnCancel();
    virtual BOOL OnInitDialogImpl();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnOrientationOption1();
    afx_msg void OnOrientationOption2();
    afx_msg void OnOrientationOption3();
    afx_msg void OnOrientationCombo1(UINT nID);
    afx_msg void OnOrientationCombo2(UINT nID);
    afx_msg void OnOrientationCombo3(UINT nID);
    afx_msg BOOL OnToolTipText(UINT id, NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    //}}AFX_MSG
private:

    CFont m_tooltip_font;
    PatientInsertDirection m_orientationoption1 ;
    PatientRotationDirection m_orientationoption2 ;
    PatientViewDirection m_orientationoption3 ;
    COUCH_POSITION m_couch_position;


    CBitmap m_main_image_array[ TOTAL_MAIN_IMAGES ];
    CBrush m_bkbrush ;
    bool m_allow_orientation_change;
    bool m_allow_orienationFor[OrientationType_Total];
    CPatientOrientationInterface* m_patientorientation_interface;
    bool m_show_wait_cursor_for_patientorientationdlg;

    // Dialog Data
    //{{AFX_DATA(CPatientOrientationDialog)
    ButtonEx m_orientation_option_1;
    ButtonEx m_orientation_option_2;
    ButtonEx m_orientation_option_3;
    CStatic m_patient_orientation_main_image_static;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPatientOrientationDialog)
    virtual BOOL PreTranslateMessageImpl(MSG* pMsg);
    //}}AFX_VIRTUAL

    void InitToolTip();
    void LoadImageForOrienationButton(ButtonEx& button, const UINT bitmap_resource_id);
    void UpdateMainImage();
    void UpdateComboImage(const OrientationType orientation_type);
    void LoadMainImageArray();
    void UnLoadMainImageArray();
    bool CanChangeOrieantationFor(const OrientationType orientation_type)const;
    void PatientOrientationOptionChangedFor(const OrientationType f_orientation_type);

    const MainOrientationImage GetPatientOrientationMainImageFor(const PatientInsertDirection option1, const PatientRotationDirection option2, const PatientViewDirection option3)const;
    int GetMainImageArrayIndexFor(const MainOrientationImage main_orientation_value)const;
    void SetMainImageFor(const PatientInsertDirection option1, const PatientRotationDirection option2, const PatientViewDirection option3);

    PatientInsertDirection GetNextOrientationOptionForType1() const;
    PatientRotationDirection GetNextOrientationOptionForType2() const;
    PatientViewDirection GetNextOrientationOptionForType3() const;
    BOOL SetBitmapForButton(ButtonEx& button_object, const UINT bitmap_resource_id, const bool center_bitmap = true)const ;

    int GetBitmapResourceIdFor(const PatientInsertDirection orientationoption)const;
    int GetBitmapResourceIdFor(const PatientRotationDirection orientationoption)const;
    int GetBitmapResourceIdFor(const PatientViewDirection orientationoption)const;

    void SetButtonAppearence();
    void SetBitmapsForButtons();

    const CString GetToolTipTextForPatienOrientation(const UINT f_nID) const;

    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #ifndef __SM_PATIENT_ORIENTATION_DIALOG_H__

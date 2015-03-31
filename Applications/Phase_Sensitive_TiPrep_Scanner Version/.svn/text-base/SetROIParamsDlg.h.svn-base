/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#if !defined(AFX_SETROIPARAMSDLG_H__60778494_0BC4_457E_84E6_6923B6217DCF__INCLUDED_)
#define AFX_SETROIPARAMSDLG_H__60778494_0BC4_457E_84E6_6923B6217DCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PopupDlg.h"
#include <MPlusUICommon/ButtonEx.h>
#include <MPlusUICommon/EditCtrlEx.h>
#include <MPlusUICommon/CheckBoxEx.h>
  //+priyanka//set-roi
#include <afxtempl.h>
#include "ROIReader.h"
  //-priyanka//set-roi
typedef enum {
   CENTER_X = 0,
   CENTER_Y,
   HEIGHT,
   WIDTH
} TextFiledInSetROIDlg;
/**
* This class is used to edit ROI.
*/
class CSetROIParamsDlg : public CPopupDlg
{
    // Construction
public:
    CSetROIParamsDlg(CWnd* pParent = NULL);   // standard constructor
    CSetROIParamsDlg& operator = (CSetROIParamsDlg& f_SetRoiParamDlg_ptr);
	CSetROIParamsDlg(CSetROIParamsDlg& f_setroiparamsdlg);

	~CSetROIParamsDlg();   //+priyanka//set-roi
    void SetT1MapMode(bool f_bIsMapMode);//AS/added

    // Dialog Data
    //{{AFX_DATA(CSetROIParamsDlg)
    enum { IDD = IDD_DIALOG_SETROI };
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA
    
    
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSetROIParamsDlg)
     virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
   
    // Implementation
protected:
    HICON m_hIcon;
    // Generated message map functions
    //{{AFX_MSG(CSetROIParamsDlg)
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnPaint();
    afx_msg void OnButtonSave();
    virtual void OnCancel();
	afx_msg void OnSaveBtnSetRoiParams();
	afx_msg void OnCheckDefaultParamSetRoi();
	afx_msg void OnChangeEditText();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    void IntializeControls();
    void DisplayRoiParams();
    BOOL ValidateROIParams(CString f_Roi_cenX[], CString f_Roi_cenY[], 
    CString f_Roi_radius[], CString f_Roi_Width[]); 
	//+priyanka//set-roi
    void CalculateDefaultROIPosition();
	void SetDataInStructure(int &f_roi_no);
    void SetHyphenInTextField(int f_nRoiNum,int f_nTextField);
    void OnEnterKey();
	//-priyanka//set-roi
private:
    bool m_bIsMapMode;
    CFont m_font;
    CBrush m_list_brush;
    
    ButtonEx m_button_Save;
    ButtonEx m_button_Exit;
    //+priyanka//set-roi
    ButtonEx m_save_btn_set_roi_param; 
    CheckBoxEx m_check_box_set_roi_param;
    //-priyanka//set-roi
    
    EditCtrlEx m_edit_ROI1_CenX;
    EditCtrlEx m_edit_ROI2_CenX;
    EditCtrlEx m_edit_ROI3_CenX;
    EditCtrlEx m_edit_ROI4_CenX;
    EditCtrlEx m_edit_ROI5_CenX;
    EditCtrlEx m_edit_ROI1_CenY;
    EditCtrlEx m_edit_ROI2_CenY;
    EditCtrlEx m_edit_ROI3_CenY;
    EditCtrlEx m_edit_ROI4_CenY;
    EditCtrlEx m_edit_ROI5_CenY;
    EditCtrlEx m_edit_ROI1_Radius;
    EditCtrlEx m_edit_ROI2_Radius;
    EditCtrlEx m_edit_ROI3_Radius;
    EditCtrlEx m_edit_ROI4_Radius;
    EditCtrlEx m_edit_ROI5_Radius;
    
    EditCtrlEx m_edit_ROI1_Width;
    EditCtrlEx m_edit_ROI2_Width;
    EditCtrlEx m_edit_ROI3_Width;
    EditCtrlEx m_edit_ROI4_Width;
    EditCtrlEx m_edit_ROI5_Width;
    
    CString m_Roi_Width[MAX_LINE_ROI]; //priyanka//set-roi
    
    CString m_Roi_cenX[MAX_LINE_ROI];   
    CString m_Roi_cenY[MAX_LINE_ROI];   
    CString m_Roi_radius[MAX_LINE_ROI]; 


	//+priyanka//set-roi
	typedef struct Roi_Saving_Point {
        float x;
        float y;
        float radius; //for elliptical ROI its height
        float width;
    } Roi_Saving_Point;

	CArray<Roi_Saving_Point, Roi_Saving_Point> m_roi_saving_point_list;
    unsigned short m_nImage_height;//sb
    unsigned short m_nImage_width;//sb
		//-priyanka//set-roi
    
private:
    void InitializeControls();
    int validateRange( double f_dRoi_cenX[], double f_dRoi_cenY[]);//sb
    
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETROIPARAMSDLG_H__60778494_0BC4_457E_84E6_6923B6217DCF__INCLUDED_)

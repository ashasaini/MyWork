/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#include "stdafx.h"
#include "tiprep.h"
#include "SetROIParamsDlg.h"
#include "TiPrepView.h"
#include "LocalDBMgr.h"

#include "TiPrepTrace.h"
#include "TiPrepLogHelper.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//****************************Method Header***************************
//Method Name   : CSetROIParamsDlg
//Purpose       : Constructor
//********************************************************************
/**
*   A constructor.
*   This function is used for initialization of class variables.
*/
CSetROIParamsDlg::CSetROIParamsDlg(CWnd* pParent /*=NULL*/)
: CPopupDlg(CSetROIParamsDlg::IDD, pParent),
m_nImage_height(0),
m_nImage_width(0),
m_bIsMapMode(false)
{
    //{{AFX_DATA_INIT(CSetROIParamsDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_font.CreatePointFont(110, _T("Arial"));
    m_hIcon = AfxGetApp()->LoadIcon(IDI_TIPREP_ICON2); //Inside the dialog's constructor.
}
  //+priyanka//set-roi

CSetROIParamsDlg& CSetROIParamsDlg::operator = (CSetROIParamsDlg& f_SetRoiParamDlg_ptr)
{
    /*TIPREP_TRACE(CSetROIParamsDlg::operator);*/
    
    if(this == &f_SetRoiParamDlg_ptr) {
        return(*this);
    }
    m_hIcon = AfxGetApp()->LoadIcon(IDI_TIPREP_ICON2); //Inside the dialog's constructor.
    return (*this);
}

CSetROIParamsDlg::CSetROIParamsDlg(CSetROIParamsDlg & f_setroiparamsdlg) : CPopupDlg(CSetROIParamsDlg::IDD)
{
   m_hIcon = f_setroiparamsdlg.m_hIcon;
   m_bIsMapMode = f_setroiparamsdlg.m_bIsMapMode;
   m_nImage_height =f_setroiparamsdlg.m_nImage_height;
   m_nImage_width =f_setroiparamsdlg.m_nImage_width;
}
//****************************Method Header***************************
//Method Name   : ~CSetROIParamsDlg()
//Purpose       : Destructor
//********************************************************************
//Add destructor

CSetROIParamsDlg::~CSetROIParamsDlg()
{

	if(m_roi_saving_point_list.GetSize() > 0) {
		m_roi_saving_point_list.RemoveAll();
	}

	try{
		m_list_brush.DeleteObject();
	}catch(...)
	{
		CString l_csmsg(_T("Exception occurred in Set ROI dialog"));
        CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
		
	}


}
//-priyanka//set-roi   

/*************************************************************
* Method:       DoDataExchange
* Description:  This function is used
* Parameter:    CDataExchange * pDX
* Returns:      void
*************************************************************/
void CSetROIParamsDlg::DoDataExchange(CDataExchange* pDX)
{
    CPopupDlg::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSetROIParamsDlg)
    DDX_Control(pDX, IDC_EDIT_ROI1_CENX, m_edit_ROI1_CenX);
    DDX_Control(pDX, IDC_EDIT_ROI1_CENY, m_edit_ROI1_CenY);
    DDX_Control(pDX, IDC_EDIT_ROI1_RADIUS, m_edit_ROI1_Radius);
    DDX_Control(pDX, IDC_EDIT_ROI2_CENX, m_edit_ROI2_CenX);
    DDX_Control(pDX, IDC_EDIT_ROI2_CENY, m_edit_ROI2_CenY);
    DDX_Control(pDX, IDC_EDIT_ROI2_RADIUS, m_edit_ROI2_Radius);
    DDX_Control(pDX, IDC_EDIT_ROI3_CENX, m_edit_ROI3_CenX);
    DDX_Control(pDX, IDC_EDIT_ROI3_CENY, m_edit_ROI3_CenY);
    DDX_Control(pDX, IDC_EDIT_ROI3_RADIUS, m_edit_ROI3_Radius);
    DDX_Control(pDX, IDC_EDIT_ROI4_CENX, m_edit_ROI4_CenX);
    DDX_Control(pDX, IDC_EDIT_ROI4_CENY, m_edit_ROI4_CenY);
    DDX_Control(pDX, IDC_EDIT_ROI4_RADIUS, m_edit_ROI4_Radius);
    DDX_Control(pDX, IDC_EDIT_ROI5_CENX, m_edit_ROI5_CenX);
    DDX_Control(pDX, IDC_EDIT_ROI5_CENY, m_edit_ROI5_CenY);
    DDX_Control(pDX, IDC_EDIT_ROI5_RADIUS, m_edit_ROI5_Radius);
    DDX_Control(pDX, IDC_EDIT_WIDTH_ROI1, m_edit_ROI1_Width);
    DDX_Control(pDX, IDC_EDIT_WIDTH_ROI2, m_edit_ROI2_Width);
    DDX_Control(pDX, IDC_EDIT_WIDTH_ROI3, m_edit_ROI3_Width);
    DDX_Control(pDX, IDC_EDIT_WIDTH_ROI4, m_edit_ROI4_Width);
    DDX_Control(pDX, IDC_EDIT_WIDTH_ROI5, m_edit_ROI5_Width);
    DDX_Control(pDX, IDC_BUTTON_SAVE, m_button_Save);
    DDX_Control(pDX, IDC_SAVE_SET_ROI_PARAMS, m_save_btn_set_roi_param);
    DDX_Control(pDX, IDC_CHECK_DEFAULT_PARAM_SET_ROI, m_check_box_set_roi_param);
    DDX_Control(pDX, IDCANCEL, m_button_Exit);
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetROIParamsDlg, CPopupDlg)
//{{AFX_MSG_MAP(CSetROIParamsDlg)
ON_WM_CTLCOLOR()
ON_WM_PAINT()
ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_SAVE_SET_ROI_PARAMS, OnSaveBtnSetRoiParams)
	ON_BN_CLICKED(IDC_CHECK_DEFAULT_PARAM_SET_ROI, OnCheckDefaultParamSetRoi)
	ON_EN_CHANGE(IDC_EDIT_ROI1_CENX, OnChangeEditText)
	ON_EN_CHANGE(IDC_EDIT_ROI1_CENY, OnChangeEditText)
	ON_EN_CHANGE(IDC_EDIT_ROI1_RADIUS, OnChangeEditText)
	ON_EN_CHANGE(IDC_EDIT_ROI2_CENX, OnChangeEditText)
	ON_EN_CHANGE(IDC_EDIT_ROI2_CENY, OnChangeEditText)
	ON_EN_CHANGE(IDC_EDIT_ROI2_RADIUS, OnChangeEditText)
	ON_EN_CHANGE(IDC_EDIT_ROI3_CENX, OnChangeEditText)
	ON_EN_CHANGE(IDC_EDIT_ROI3_CENY, OnChangeEditText)
	ON_EN_CHANGE(IDC_EDIT_ROI3_RADIUS, OnChangeEditText)
	ON_EN_CHANGE(IDC_EDIT_ROI4_CENX, OnChangeEditText)
	ON_EN_CHANGE(IDC_EDIT_ROI4_CENY, OnChangeEditText)
	ON_EN_CHANGE(IDC_EDIT_ROI4_RADIUS, OnChangeEditText)
	ON_EN_CHANGE(IDC_EDIT_ROI5_CENX, OnChangeEditText)
	ON_EN_CHANGE(IDC_EDIT_ROI5_CENY, OnChangeEditText)
	ON_EN_CHANGE(IDC_EDIT_ROI5_RADIUS, OnChangeEditText)
	ON_EN_CHANGE(IDC_EDIT_WIDTH_ROI1, OnChangeEditText)
	ON_EN_CHANGE(IDC_EDIT_WIDTH_ROI2, OnChangeEditText)
	ON_EN_CHANGE(IDC_EDIT_WIDTH_ROI3, OnChangeEditText)
	ON_EN_CHANGE(IDC_EDIT_WIDTH_ROI4, OnChangeEditText)
	ON_EN_CHANGE(IDC_EDIT_WIDTH_ROI5, OnChangeEditText)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetROIParamsDlg message handlers

/*************************************************************
* Method:       OnInitDialog
* Description:  This function is used
* Returns:      BOOL
*************************************************************/
BOOL CSetROIParamsDlg::OnInitDialog()
{
    try{
        CPopupDlg::OnInitDialog();
        
        SetIcon(m_hIcon, TRUE);			// Set big icon
        SetIcon(m_hIcon, FALSE);		// Set small icon

        WINDOW_COLORS_t l_window_color;
        GetWindowColors(l_window_color);
        m_list_brush.CreateSolidBrush(l_window_color.crClientColor);
        SetRibbonHeight(0);
        InitializeControls();
        CTiPrepView::GetView()->GetROIInstance()->MouseMoveROI(FALSE); 
        BOOL l_falg = (CTiPrepView::GetView())->GetROIInstance()->GetROIPosition
            (m_Roi_cenX, m_Roi_cenY, m_Roi_radius, m_Roi_Width);
        
        if(!l_falg) {
            CString l_csErrMsg(_T("Unable to get ROI position."));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return FALSE;
        }
        
        DisplayRoiParams();
        CTiPrepView::GetView()->AddZOrderLayer();
        if(FALSE == CTiPrepView::GetView()->RegisterWindowZOrder(m_hWnd)){
            CString l_csErrMsg(_T("OnInitDialog :RegisterWindowZOrder failed."));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
        }
		m_save_btn_set_roi_param.EnableWindow(false); //disabled
    }
    catch(...){
        CString l_csErrMsg(_T("OnInitDialog :Exception Occurred"));
        CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
        return FALSE;
    }
    return TRUE;  
}

/*************************************************************
* Method:       OnCtlColor
* Description:  This function is used
* Parameter:    CDC * pDC
* Parameter:    CWnd * pWnd
* Parameter:    UINT nCtlColor
* Returns:      HBRUSH
*************************************************************/
HBRUSH CSetROIParamsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CPopupDlg::OnCtlColor(pDC, pWnd, nCtlColor);
    
    if(nCtlColor == CTLCOLOR_STATIC) {
        pDC->SetBkColor(BKCOLOR);
        pDC->SetTextColor(TEXT_BKCOLOR);
        pDC->SetBkMode(OPAQUE);
        return (HBRUSH)m_list_brush;
    } else {
        //
    }
    
    return hbr;
}

/*************************************************************
* Method:       OnPaint
* Description:  This function is used
* Returns:      void
*************************************************************/
void CSetROIParamsDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    if(IsIconic()) {
        CPaintDC dc(this); // device context for painting
        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    } else {
        //CPopupDlg::OnPaint();
    }
}

/*************************************************************
* Method:       InitializeControls
* Description:  This function is used to initialize dialog controls.
* Returns:      void
*************************************************************/
void CSetROIParamsDlg::InitializeControls()
{
    m_edit_ROI1_CenX.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_edit_ROI1_CenX.SetBkColor(BKCOLOR);
    m_edit_ROI1_CenY.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_edit_ROI1_CenY.SetBkColor(BKCOLOR);
    m_edit_ROI1_Radius.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_edit_ROI1_Radius.SetBkColor(BKCOLOR);
    m_edit_ROI2_CenX.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_edit_ROI2_CenX.SetBkColor(BKCOLOR);
    m_edit_ROI2_CenY.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_edit_ROI2_CenY.SetBkColor(BKCOLOR);
    m_edit_ROI2_Radius.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_edit_ROI2_Radius.SetBkColor(BKCOLOR);
    m_edit_ROI3_CenX.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_edit_ROI3_CenX.SetBkColor(BKCOLOR);
    m_edit_ROI3_CenY.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_edit_ROI3_CenY.SetBkColor(BKCOLOR);
    m_edit_ROI3_Radius.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_edit_ROI3_Radius.SetBkColor(BKCOLOR);
    m_edit_ROI4_CenX.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_edit_ROI4_CenX.SetBkColor(BKCOLOR);
    m_edit_ROI4_CenY.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_edit_ROI4_CenY.SetBkColor(BKCOLOR);
    m_edit_ROI4_Radius.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_edit_ROI4_Radius.SetBkColor(BKCOLOR);
    m_edit_ROI5_CenX.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_edit_ROI5_CenX.SetBkColor(BKCOLOR);
    m_edit_ROI5_CenY.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_edit_ROI5_CenY.SetBkColor(BKCOLOR);
    m_edit_ROI5_Radius.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_edit_ROI5_Radius.SetBkColor(BKCOLOR);
    m_edit_ROI1_Width.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_edit_ROI1_Width.SetBkColor(BKCOLOR);
    m_edit_ROI2_Width.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_edit_ROI2_Width.SetBkColor(BKCOLOR);
    m_edit_ROI3_Width.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_edit_ROI3_Width.SetBkColor(BKCOLOR);
    m_edit_ROI4_Width.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_edit_ROI4_Width.SetBkColor(BKCOLOR);
    m_edit_ROI5_Width.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_edit_ROI5_Width.SetBkColor(BKCOLOR);
    m_button_Save.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
    //+priyanka//set-roi
    m_save_btn_set_roi_param.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true); 
    m_check_box_set_roi_param.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_check_box_set_roi_param.SetBkColor(BKCOLOR);
    //-priyanka//set-roi

    m_button_Exit.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
    GetDlgItem(IDC_STATIC_CENTER_X)->SetFont(&m_font, TRUE);
    GetDlgItem(IDC_STATIC_CENTER_Y)->SetFont(&m_font, TRUE);
    GetDlgItem(IDC_STATIC_RADIUS)->SetFont(&m_font, TRUE);
    GetDlgItem(IDC_STATIC_WIDTH)->SetFont(&m_font, TRUE);
    GetDlgItem(IDC_STATIC_ROI1_PARAMS)->SetFont(&m_font, TRUE);
    GetDlgItem(IDC_STATIC_ROI2_PARAMS)->SetFont(&m_font, TRUE);
    GetDlgItem(IDC_STATIC_ROI3_PARAMS)->SetFont(&m_font, TRUE);
    GetDlgItem(IDC_STATIC_ROI4_PARAMS)->SetFont(&m_font, TRUE);
    GetDlgItem(IDC_STATIC_ROI5_PARAMS)->SetFont(&m_font, TRUE);
    if(true == m_bIsMapMode){
        
        //Hide ROI 3 controls from "Set ROI Parameters dialog"
        GetDlgItem(IDC_STATIC_ROI3_PARAMS)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_ROI3_CENX)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_ROI3_CENY)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_ROI3_RADIUS)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_WIDTH_ROI3)->ShowWindow(SW_HIDE);
    }else{

        //show ROI 3 controls from "Set ROI Parameters dialog"
        GetDlgItem(IDC_STATIC_ROI3_PARAMS)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_EDIT_ROI3_CENX)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_EDIT_ROI3_CENY)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_EDIT_ROI3_RADIUS)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_EDIT_WIDTH_ROI3)->ShowWindow(SW_SHOW);
    }
	if(E_NO_ERROR != CTiPrepView::GetView()->GetHeightWidthImageFromDB(m_nImage_height,m_nImage_width)){
        CString l_csErrMsg(_T("GetHeightWidthImageFromDB failed."));
        CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
    }

}

/*************************************************************
* Method:       DisplayRoiParams
* Description:  This function is used to display ROI parameter.
* Returns:      void
*************************************************************/
void CSetROIParamsDlg::DisplayRoiParams()
{
    
    for(int l_ni = 0; l_ni < MAX_LINE_ROI; l_ni++) {
        switch(l_ni) {
        case 0:
            m_edit_ROI1_CenX.SetWindowText(m_Roi_cenX[l_ni]);
            m_edit_ROI1_CenY.SetWindowText(m_Roi_cenY[l_ni]);
            m_edit_ROI1_Radius.SetWindowText(m_Roi_radius[l_ni]);
            m_edit_ROI1_Width.SetWindowText(m_Roi_Width[l_ni]);
            break;
        case 1:
            m_edit_ROI2_CenX.SetWindowText(m_Roi_cenX[l_ni]);
            m_edit_ROI2_CenY.SetWindowText(m_Roi_cenY[l_ni]);
            m_edit_ROI2_Radius.SetWindowText(m_Roi_radius[l_ni]);
            m_edit_ROI2_Width.SetWindowText(m_Roi_Width[l_ni]);
            break;
        case 2:
            m_edit_ROI3_CenX.SetWindowText(m_Roi_cenX[l_ni]);
            m_edit_ROI3_CenY.SetWindowText(m_Roi_cenY[l_ni]);
            m_edit_ROI3_Radius.SetWindowText(m_Roi_radius[l_ni]);
            m_edit_ROI3_Width.SetWindowText(m_Roi_Width[l_ni]);
            break;
        case 3:
            m_edit_ROI4_CenX.SetWindowText(m_Roi_cenX[l_ni]);
            m_edit_ROI4_CenY.SetWindowText(m_Roi_cenY[l_ni]);
            m_edit_ROI4_Radius.SetWindowText(m_Roi_radius[l_ni]);
            m_edit_ROI4_Width.SetWindowText(m_Roi_Width[l_ni]);
            break;
        case 4:
            m_edit_ROI5_CenX.SetWindowText(m_Roi_cenX[l_ni]);
            m_edit_ROI5_CenY.SetWindowText(m_Roi_cenY[l_ni]);
            m_edit_ROI5_Radius.SetWindowText(m_Roi_radius[l_ni]);
            m_edit_ROI5_Width.SetWindowText(m_Roi_Width[l_ni]);
            break;
        } 
        
        SetDataInStructure(l_ni); 
    }
}

/*************************************************************
* Method:       OnButtonSave
* Description:  This function used to save ROI parameter.
* Returns:      void
*************************************************************/
void CSetROIParamsDlg::OnButtonSave()
{

    
    try{
        for(int l_ni = 0; l_ni < MAX_LINE_ROI; l_ni++) {
            switch(l_ni) {
            case 0:
                m_edit_ROI1_CenX.GetWindowText(m_Roi_cenX[l_ni]);
                m_edit_ROI1_CenY.GetWindowText(m_Roi_cenY[l_ni]);
                m_edit_ROI1_Radius.GetWindowText(m_Roi_radius[l_ni]);
                m_edit_ROI1_Width.GetWindowText(m_Roi_Width[l_ni]);
                break;
            case 1:
                m_edit_ROI2_CenX.GetWindowText(m_Roi_cenX[l_ni]);
                m_edit_ROI2_CenY.GetWindowText(m_Roi_cenY[l_ni]);
                m_edit_ROI2_Radius.GetWindowText(m_Roi_radius[l_ni]);
                m_edit_ROI2_Width.GetWindowText(m_Roi_Width[l_ni]);
                break;
            case 2:
                m_edit_ROI3_CenX.GetWindowText(m_Roi_cenX[l_ni]);
                m_edit_ROI3_CenY.GetWindowText(m_Roi_cenY[l_ni]);
                m_edit_ROI3_Radius.GetWindowText(m_Roi_radius[l_ni]);
                m_edit_ROI3_Width.GetWindowText(m_Roi_Width[l_ni]);
                break;
            case 3:
                m_edit_ROI4_CenX.GetWindowText(m_Roi_cenX[l_ni]);
                m_edit_ROI4_CenY.GetWindowText(m_Roi_cenY[l_ni]);
                m_edit_ROI4_Radius.GetWindowText(m_Roi_radius[l_ni]);
                m_edit_ROI4_Width.GetWindowText(m_Roi_Width[l_ni]);
                break;
            case 4:
                m_edit_ROI5_CenX.GetWindowText(m_Roi_cenX[l_ni]);
                m_edit_ROI5_CenY.GetWindowText(m_Roi_cenY[l_ni]);
                m_edit_ROI5_Radius.GetWindowText(m_Roi_radius[l_ni]);
                m_edit_ROI5_Width.GetWindowText(m_Roi_Width[l_ni]);
                break;
            } 
            SetDataInStructure(l_ni);
        }
        BOOL l_status = ValidateROIParams(m_Roi_cenX, m_Roi_cenY, m_Roi_radius, m_Roi_Width); 
        
        if(!l_status) {
            int l_nErrorMsgCode = I_FLOAT_VAL;
            (CTiPrepView::GetView())->DisplayErr(l_nErrorMsgCode,SET_ROI_DLG);
        } else {
            //Old method Commented
            /*BOOL l_falg = (CTiPrepView::GetView())->GetROIInstance()->SaveMoveROI(m_Roi_cenX,
                m_Roi_cenY, m_Roi_radius, m_Roi_Width);*/
            //New Method Start
            BOOL flag =(CTiPrepView::GetView())->GetROIInstance()->SetROIPos(m_Roi_cenX,m_Roi_cenY, m_Roi_radius, m_Roi_Width);
            if(flag == FALSE) {
                CString l_csErrMsg(_T("Unable to SetROIPos."));
                CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                return;
            }
            CString l_csProtocolName(_T(""));
            CString mode(_T(""));
            flag =(CTiPrepView::GetView())->GetROIInstance()->RedrawROI(false,l_csProtocolName,mode);
            if(flag == FALSE) {
                CString l_csErrMsg(_T("Unable to save ROI position."));
                CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
                return;
            }
            //New Method End 
            DisplayRoiParams();
		    m_save_btn_set_roi_param.EnableWindow(true); //enabled
        }
    }catch(...){
        CString l_csErrMsg(_T("OnButtonSave : Exception Occured."));
        CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
        return;
    }
}

/*************************************************************
* Method:       ValidateROIParams
* Description:  This function used to validate ROI parameter
*               entered by user.
* Parameter:    CString f_Roi_cenX[]
*               X co-ordinate of center.
* Parameter:    CString f_Roi_cenY[]
*               Y co-ordinate of center.
* Parameter:    CString f_Roi_radius[]
*               Radius of ROI.
* Returns:      BOOL
*************************************************************/
BOOL CSetROIParamsDlg::ValidateROIParams(CString f_Roi_cenX[],
                                         CString f_Roi_cenY[],
                                         CString f_Roi_radius[],
                                         CString f_Roi_width[])
{
    BOOL l_result = TRUE;
    char val[100];
    int l_nDotCnt = 0;
    int l_nj = 0;
    double f_dRoi_Width[MAX_LINE_ROI]={0.0};
    double f_dRoi_cenX[MAX_LINE_ROI]={0.0};  
    double f_dRoi_cenY[MAX_LINE_ROI]={0.0};   
    double f_dRoi_radius[MAX_LINE_ROI]={0.0};
    
    try{
        for(int l_ni = 0; l_ni < MAX_LINE_ROI; l_ni++) {
            if(ROI_3 == l_ni){  //validation not required for ROI3 in map mode
                if(true == m_bIsMapMode){
                    continue;
                }
            }
            //--------------Center X--------------
            l_nDotCnt = 0;
            int l_nstrlength = f_Roi_cenX[l_ni].GetLength();
            wcstombs(val, f_Roi_cenX[l_ni], 100);
            for(l_nj = 0; l_nj < l_nstrlength; l_nj++) {
                if('-' == val[l_nj] && l_nj == 0) {
                    l_nj++;
                } 
                if('.' == val[l_nj]) {
                    l_nDotCnt++;
                    l_nj++;
                }
                if(FALSE == isdigit(val[l_nj])) {
                    SetHyphenInTextField(l_ni,CENTER_X);
                    return FALSE;
                }
            }
            if(1 < l_nDotCnt) {
                SetHyphenInTextField(l_ni,CENTER_X);
                return FALSE;
            }
            f_dRoi_cenX[l_ni] = atof(val);
            f_dRoi_cenX[l_ni] = f_dRoi_cenX[l_ni]*100.0;
            f_dRoi_cenX[l_ni] = (f_dRoi_cenX[l_ni] + 0.5);
            f_dRoi_cenX[l_ni] = (f_dRoi_cenX[l_ni] / 100.0);
            
            f_Roi_cenX[l_ni].Format(_T("%0.1lf"),f_dRoi_cenX[l_ni]);
            //------------
            //-----------------Center Y------------
            l_nDotCnt = 0;
            l_nstrlength = f_Roi_cenY[l_ni].GetLength();
            wcstombs(val, f_Roi_cenY[l_ni], 100);
            for(l_nj = 0; l_nj < l_nstrlength; l_nj++) {
                if('-' == val[l_nj] && l_nj == 0) {
                    l_nj++;
                } 
                if('.' == val[l_nj]) {
                    l_nDotCnt++;
                    l_nj++;
                }
                if(FALSE == isdigit(val[l_nj])) {
                    SetHyphenInTextField(l_ni,CENTER_Y);
                    return FALSE;
                }
            }
            if(1 < l_nDotCnt) {
                SetHyphenInTextField(l_ni,CENTER_Y);
                return FALSE;
            }
            //         f_dRoi_cenY[l_ni] = atof(val);
            //         f_dRoi_cenY[l_ni] = f_dRoi_cenY[l_ni] + 0.05;
            //AS
            f_dRoi_cenY[l_ni] = atof(val);
            f_dRoi_cenY[l_ni] = f_dRoi_cenY[l_ni]*100.0;
            f_dRoi_cenY[l_ni] = (f_dRoi_cenY[l_ni] + 0.5);
            f_dRoi_cenY[l_ni] = (f_dRoi_cenY[l_ni] / 100.0);
            //AS
            f_Roi_cenY[l_ni].Format(_T("%0.1lf"),f_dRoi_cenY[l_ni]); 
            
            //---------------
            //--------------Height---------------
            l_nDotCnt = 0;
            l_nstrlength = f_Roi_radius[l_ni].GetLength();
            wcstombs(val, f_Roi_radius[l_ni], 100);
            for(l_nj = 0; l_nj < l_nstrlength; l_nj++) {
                if('.' == val[l_nj]) {
                    l_nDotCnt++;
                    l_nj++;
                }
                if(FALSE == isdigit(val[l_nj])) {
                    SetHyphenInTextField(l_ni,HEIGHT);
                    return FALSE;
                }
            }
            if(1 < l_nDotCnt) {
                SetHyphenInTextField(l_ni,HEIGHT);
                return FALSE;
            }
            if(atof(val) > 0) {
                l_result = TRUE;
            } else {
                SetHyphenInTextField(l_ni,HEIGHT);
                return FALSE;
            }
            //         f_dRoi_radius[l_ni] = atof(val);
            //         f_dRoi_radius[l_ni] = f_dRoi_radius[l_ni] + 0.05; //hight for ellipse
            //AS
            f_dRoi_radius[l_ni] = atof(val);
            f_dRoi_radius[l_ni] = f_dRoi_radius[l_ni]*100.0;
            f_dRoi_radius[l_ni] = (f_dRoi_radius[l_ni] + 0.5);
            f_dRoi_radius[l_ni] = (f_dRoi_radius[l_ni] / 100.0);
            //AS
            
            f_Roi_radius[l_ni].Format(_T("%0.1lf"),f_dRoi_radius[l_ni]);
            //----------------
            //------------Width---------------
            l_nDotCnt = 0;
            l_nstrlength = f_Roi_width[l_ni].GetLength();
            wcstombs(val,f_Roi_width[l_ni], 100);
            for(l_nj = 0; l_nj < l_nstrlength; l_nj++) {
                if('.' == val[l_nj]) {
                    l_nDotCnt++;
                    l_nj++;
                }
                if(FALSE == isdigit(val[l_nj])) {
                    SetHyphenInTextField(l_ni,WIDTH);
                    return FALSE;
                }
            }
            if(1 < l_nDotCnt) {
                SetHyphenInTextField(l_ni,WIDTH);
                return FALSE;
            }
            if(atof(val) > 0.0f) {
                l_result = TRUE;
            } else {
                SetHyphenInTextField(l_ni,WIDTH);
                return FALSE;
            }
            //         f_dRoi_Width[l_ni] = atof(val);
            //         f_dRoi_Width[l_ni] = f_dRoi_Width[l_ni] + 0.05;
            //AS
            f_dRoi_Width[l_ni] = atof(val);
            f_dRoi_Width[l_ni] = f_dRoi_Width[l_ni]*100.0;
            f_dRoi_Width[l_ni] = (f_dRoi_Width[l_ni] + 0.5);
            f_dRoi_Width[l_ni] = (f_dRoi_Width[l_ni] / 100.0);
            //AS
            f_Roi_width[l_ni].Format(_T("%0.1lf"),f_dRoi_Width[l_ni]);
            //-------------------        
        }
        l_result = validateRange(f_dRoi_cenX,f_dRoi_cenY);
        if (E_NO_ERROR != l_result)
        {
            return FALSE;
        }else{
            l_result = TRUE;
        }
    }catch(...){
        CString l_csErrMsg(_T("ValidateROIParams : Exception Occured."));
        CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
        return FALSE;
    }
    
    return l_result;
}
/*************************************************************
* Method:    	OnCancel
* Description: 	This function is used 
* Returns:   	void
*************************************************************/
void CSetROIParamsDlg::OnCancel()
{
    if (CTiPrepView::GetView()->GetCTiPrepDlgInstance()->isEditROI()) {
        CTiPrepView::GetView()->GetROIInstance()->MouseMoveROI(TRUE); 
    }

    CTiPrepView::GetView()->UnRegisterWindowZOrder(m_hWnd);
    CPopupDlg::OnCancel();
}


/*************************************************************
* Method:    	OnSaveBtnSetRoiParams
* Description: 	This function is used 
* Returns:   	void
*************************************************************/
void CSetROIParamsDlg::OnSaveBtnSetRoiParams() 
{
    CTiPrepDlg *l_tiprep_dlg = NULL;

    try{
        l_tiprep_dlg = CTiPrepView::GetView()->GetCTiPrepDlgInstance();
        if(NULL == l_tiprep_dlg) {
            CTiPrepLogHelper::WriteToErrorLog(_T("TiPrep Dlg object is null"), __LINE__, __FILE__);
            return;
        }
        CEllipticalROIParams l_ellipseparam;
        CROIReader l_roi_reader;
        int l_roi_saving_list_size = m_roi_saving_point_list.GetSize();
        vector<CEllipticalROIParams> l_ellipseparam_vector;
        if(l_ellipseparam_vector.empty() == false) {
            l_ellipseparam_vector.clear();                                                
        }
        CString l_protocol_name = (_T(""));
        l_protocol_name = (CTiPrepView::GetView())->GetROIInstance()->GetProtocolNameForSetROIParams();
        for(int l_roi_no = 0; l_roi_no < l_roi_saving_list_size; l_roi_no++) {
            l_ellipseparam.SetHeight(m_roi_saving_point_list.GetAt(l_roi_no).radius);
            l_ellipseparam.SetWidth(m_roi_saving_point_list.GetAt(l_roi_no).width);
            l_ellipseparam.SetCenter_X(m_roi_saving_point_list.GetAt(l_roi_no).x);
            l_ellipseparam.SetCenter_Y(m_roi_saving_point_list.GetAt(l_roi_no).y);
            l_ellipseparam_vector.push_back(l_ellipseparam);
        }
        int l_status = E_NO_ERROR;
        char l_dest_folder[100] = {'\0'};
        strcpy(l_dest_folder, "c:/MrMplus/Data/TiPrep");    
        if(ERROR_PATH_NOT_FOUND != CreateDirectoryA((LPCSTR)l_dest_folder, NULL)) {
            if(l_tiprep_dlg->m_checkT1Map.GetCheck() ==BST_UNCHECKED) {
                CString l_avgfile(AVGROIFILE);
                l_status = l_roi_reader.WriteROIInfo(l_avgfile, l_protocol_name, l_ellipseparam_vector);
            } else /*if(m_check_box_set_roi_param.GetCheck()==BST_CHECKED)*/ {
                CString l_mapfile(MAPROIFILE);
                l_status = l_roi_reader.WriteROIInfo(l_mapfile, l_protocol_name, l_ellipseparam_vector);
            }
        } 
		m_save_btn_set_roi_param.EnableWindow(false); //disabled
    }catch(...){
        CString l_csErrMsg(_T("OnSaveBtnSetRoiParams : Exception Occurred."));
        CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
        return ;
    }
	
}

/*************************************************************
* Method:    	OnCheckDefaultParamSetRoi
* Description: 	This function is used to set default parameters
* Returns:   	void
*************************************************************/
void CSetROIParamsDlg::OnCheckDefaultParamSetRoi() 
{
    if(m_check_box_set_roi_param.GetCheck()==BST_UNCHECKED) {
        
        BOOL l_falg = (CTiPrepView::GetView())->GetROIInstance()->GetROIPosition
            (m_Roi_cenX, m_Roi_cenY, m_Roi_radius, m_Roi_Width);

        if(FALSE == l_falg){
            return;
        }
    }else{
        CalculateDefaultROIPosition();
    }

    DisplayRoiParams();
}

/*************************************************************
* Method:    	CalculateDefaultROIPosition
* Description: 	This function is used 
* Returns:   	void
*************************************************************/
void CSetROIParamsDlg::CalculateDefaultROIPosition() 
{
    float l_fheight = 0.0;
    CString l_default_height_str = (_T(""));
    
    float l_fwidtht = 0.0;
    CString l_default_width_str = (_T(""));
    
    float l_default_y_pos = 0.0f;
    CString l_default_y_pos_str = (_T(""));
    
    CString l_default_x_pos_str = (_T(""));
    float l_default_x_pos = -100.0f;
    
    CTiPrepDlg *l_tiprep_dlg = NULL;
    int l_roi_no = 0;
    l_tiprep_dlg = CTiPrepView::GetView()->GetCTiPrepDlgInstance();
    
    
    l_fheight = 8.0;
    l_default_height_str.Format(_T("%0.1f"), l_fheight); 
    
    l_fwidtht = 8.0;
    l_default_width_str.Format(_T("%0.1f"), l_fwidtht);
    
    l_default_y_pos = -90.0f;
    l_default_y_pos_str.Format(_T("%0.1f"), l_default_y_pos);
    
   
    for(l_roi_no = 0; l_roi_no < MAX_LINE_ROI; l_roi_no++) {
        if(l_tiprep_dlg->m_checkT1Map.GetCheck() ==BST_UNCHECKED) {
            l_default_x_pos_str.Empty();
            l_default_x_pos_str.Format(_T("%0.1f"), (l_default_x_pos +(l_roi_no)*15.0f));
            m_Roi_cenX[l_roi_no] = l_default_x_pos_str;
            
            m_Roi_cenY[l_roi_no] = l_default_y_pos_str;
            m_Roi_radius[l_roi_no] = l_default_height_str;
            m_Roi_Width[l_roi_no] = l_default_width_str;
        } else {                
            if(0 == l_roi_no){   //ROI 1                
                l_fheight = 35.0f;
                l_fwidtht = 35.0f;
            } else if(1 == l_roi_no) {//ROI 2
                l_fheight = 55.0f;
                l_fwidtht = 55.0f;
            } else {//noise ROIs
                l_fheight = 8.0f;
                l_fwidtht = 8.0f;
            }

            l_default_height_str.Format(_T("%0.1f"), l_fheight); 
            m_Roi_radius[l_roi_no] = l_default_height_str;
            
            l_default_width_str.Format(_T("%0.1f"), l_fwidtht);
            m_Roi_Width[l_roi_no] = l_default_width_str;
            
            if(0 == l_roi_no || 1 == l_roi_no){
                l_default_x_pos_str.Format(_T("%0.1f"), l_default_x_pos);
            }else{
                l_default_x_pos_str.Format(_T("%0.1f"), (l_default_x_pos +(l_roi_no)*15.0f));
            }
            m_Roi_cenX[l_roi_no] = l_default_x_pos_str;
            
            m_Roi_cenY[l_roi_no] = l_default_y_pos_str;
        }
    }
}

/*************************************************************
* Method:    	SetDataInStructure
* Description: 	This function is used 
* Parameter: 	int & f_roi_no :
* Returns:   	void
*************************************************************/
void CSetROIParamsDlg::SetDataInStructure(int &f_roi_no) 
{
    try{
	Roi_Saving_Point l_roi_data;
	char l_string_for_x[128] = {0};
	char l_string_for_y[128] = {0};
	char l_string_for_radius[128] = {0};
	char l_string_for_width[128] = {0};

	wcstombs(l_string_for_x, m_Roi_cenX[f_roi_no], 128);
	float l_x_value =atof(l_string_for_x);

	wcstombs(l_string_for_y, m_Roi_cenY[f_roi_no], 128);
	float l_y_value =atof(l_string_for_y);

	wcstombs(l_string_for_radius, m_Roi_radius[f_roi_no], 128);
	float l_radius_value =atof(l_string_for_radius);

	wcstombs(l_string_for_width, m_Roi_Width[f_roi_no], 128);
	float l_width_value =atof(l_string_for_width);

	l_roi_data.x= l_x_value;
	l_roi_data.y= l_y_value;
	l_roi_data.radius= l_radius_value;
	l_roi_data.width= l_width_value;

    if(f_roi_no < m_roi_saving_point_list.GetSize()) {
        m_roi_saving_point_list.RemoveAt(f_roi_no); 
    }
    m_roi_saving_point_list.InsertAt(f_roi_no, l_roi_data); 

    }catch(...){
        CString l_csErrMsg(_T("CalculateDefaultROIPosition : Exception Occurred."));
        CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
        return ;
    }
}

/*************************************************************
* Method:    	SetT1MapMode
* Description: 	This function is used 
* Parameter: 	bool f_bIsMapMode :
* Returns:   	void
*************************************************************/
void CSetROIParamsDlg::SetT1MapMode(bool f_bIsMapMode )
{
    m_bIsMapMode = f_bIsMapMode;
}

/*************************************************************
* Method:    	validateRange
* Description: 	This function is used 
* Parameter: 	double f_dRoi_cenX :
* Parameter: 	double f_dRoi_cenY :
* Returns:   	int
*************************************************************/
int CSetROIParamsDlg::validateRange( double f_dRoi_cenX[], double f_dRoi_cenY[] )
{
    int l_nLRX = -(m_nImage_width/2);
    int l_nURX = (m_nImage_width/2);
    int l_nLRY = -(m_nImage_height/2);
    int l_nURY = (m_nImage_height/2);
    for (int l_nRoi = 0;l_nRoi < MAX_LINE_ROI;l_nRoi++){
        
        if(l_nRoi == ROI_3){
            // as position of roi 3 is not to be considered for validation in case of map mode
            if(m_bIsMapMode == true){   
                continue;
            }
        }
        
        if ((l_nLRX >= f_dRoi_cenX[l_nRoi]) || (l_nURX < f_dRoi_cenX[l_nRoi])){
            SetHyphenInTextField(l_nRoi,CENTER_X);
            return E_ERROR;
        }
        
        if ((l_nLRY >= f_dRoi_cenY[l_nRoi]) || (l_nURY < f_dRoi_cenY[l_nRoi])){
            SetHyphenInTextField(l_nRoi,CENTER_Y);
            return E_ERROR;
        }
        
    }
    return E_NO_ERROR;
}

void CSetROIParamsDlg::SetHyphenInTextField( int f_nRoiNum,int f_nTextField)
{
    CString l_csTempStr(_T("---.---"));
    switch(f_nRoiNum)
    {
    case ROI_1:
        switch(f_nTextField)
        {
            case CENTER_X: //ROI1 x field
                m_edit_ROI1_CenX.SetWindowText(l_csTempStr);           
                break;
            case CENTER_Y: //ROI1 Y field
                m_edit_ROI1_CenY.SetWindowText(l_csTempStr);            
                break;
            case HEIGHT: //ROI1 Height field
                m_edit_ROI1_Radius.SetWindowText(l_csTempStr);            
                break;
            case WIDTH: //ROI1 Width field
                m_edit_ROI1_Width.SetWindowText(l_csTempStr);
                break;
            default:
                break;
            }
        break;

    case ROI_2:
        switch(f_nTextField)
        {
            case CENTER_X: //ROI2 x field
                m_edit_ROI2_CenX.SetWindowText(l_csTempStr);           
                break;
            case CENTER_Y: //ROI2 Y field
                m_edit_ROI2_CenY.SetWindowText(l_csTempStr);           
                break;
            case HEIGHT: //ROI2 Height field
                m_edit_ROI2_Radius.SetWindowText(l_csTempStr);            
                break;
            case WIDTH: //ROI2 Width field
                m_edit_ROI2_Width.SetWindowText(l_csTempStr);
                break;
            default:
                break;
        }
        break;

    case ROI_3:
        switch(f_nTextField)
        {
            case CENTER_X: //ROI3 x field
                m_edit_ROI3_CenX.SetWindowText(l_csTempStr);           
                break;
            case CENTER_Y: //ROI3 Y field
                m_edit_ROI3_CenY.SetWindowText(l_csTempStr);          
                break;
            case HEIGHT: //ROI3 Height field
                m_edit_ROI3_Radius.SetWindowText(l_csTempStr);           
                break;
            case WIDTH: //ROI3 Width field
                m_edit_ROI3_Width.SetWindowText(l_csTempStr);
                break;
            default:
                break;
            }
        break;

    case NOISE_ROI_1:
        switch(f_nTextField)
        {
            case CENTER_X: //ROI4 x field
                m_edit_ROI4_CenX.SetWindowText(l_csTempStr);
                break;
            case CENTER_Y: //ROI4 Y field
                m_edit_ROI4_CenY.SetWindowText(l_csTempStr);
                break;
            case HEIGHT: //ROI4 Height field
                m_edit_ROI4_Radius.SetWindowText(l_csTempStr);
                break;
            case WIDTH: //ROI4 Width field
                m_edit_ROI4_Width.SetWindowText(l_csTempStr);
                break;
            default:
                break;
            }
        break;

    case NOISE_ROI_2:
        switch(f_nTextField)
        {
            case CENTER_X: //ROI5 x field
                m_edit_ROI5_CenX.SetWindowText(l_csTempStr);
                break;
            case CENTER_Y: //ROI5 Y field
                m_edit_ROI5_CenY.SetWindowText(l_csTempStr);
                break;
            case HEIGHT: //ROI5 Height field
                m_edit_ROI5_Radius.SetWindowText(l_csTempStr);
                break;
            case WIDTH: //ROI5 Width field
                m_edit_ROI5_Width.SetWindowText(l_csTempStr);
                break;
            default:
                break;
        }
        break;

    default:
        break;        
    }
}

BOOL CSetROIParamsDlg::PreTranslateMessage( MSG* pMsg )
{
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
    {
        OnEnterKey();
        return TRUE;
    }
    else if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE){
        return TRUE;
    }

   
	return DialogEx::PreTranslateMessage(pMsg);
}

///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////

void CSetROIParamsDlg::OnChangeEditText() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPopupDlg::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
    //if (pMsg->message == WM_COMMAND && (pMsg->wParam == EN_CHANGE))
    //{
        m_save_btn_set_roi_param.EnableWindow(FALSE);
      //  return TRUE;
    //}
	
}

void CSetROIParamsDlg::OnEnterKey()
{
    try{
        for(int l_ni = 0; l_ni < MAX_LINE_ROI; l_ni++) {
            switch(l_ni) {
            case 0:
                m_edit_ROI1_CenX.GetWindowText(m_Roi_cenX[l_ni]);
                m_edit_ROI1_CenY.GetWindowText(m_Roi_cenY[l_ni]);
                m_edit_ROI1_Radius.GetWindowText(m_Roi_radius[l_ni]);
                m_edit_ROI1_Width.GetWindowText(m_Roi_Width[l_ni]);
                break;
            case 1:
                m_edit_ROI2_CenX.GetWindowText(m_Roi_cenX[l_ni]);
                m_edit_ROI2_CenY.GetWindowText(m_Roi_cenY[l_ni]);
                m_edit_ROI2_Radius.GetWindowText(m_Roi_radius[l_ni]);
                m_edit_ROI2_Width.GetWindowText(m_Roi_Width[l_ni]);
                break;
            case 2:
                m_edit_ROI3_CenX.GetWindowText(m_Roi_cenX[l_ni]);
                m_edit_ROI3_CenY.GetWindowText(m_Roi_cenY[l_ni]);
                m_edit_ROI3_Radius.GetWindowText(m_Roi_radius[l_ni]);
                m_edit_ROI3_Width.GetWindowText(m_Roi_Width[l_ni]);
                break;
               
            case 3:
                m_edit_ROI4_CenX.GetWindowText(m_Roi_cenX[l_ni]);
                m_edit_ROI4_CenY.GetWindowText(m_Roi_cenY[l_ni]);
                m_edit_ROI4_Radius.GetWindowText(m_Roi_radius[l_ni]);
                m_edit_ROI4_Width.GetWindowText(m_Roi_Width[l_ni]);
                break;
            case 4:
                m_edit_ROI5_CenX.GetWindowText(m_Roi_cenX[l_ni]);
                m_edit_ROI5_CenY.GetWindowText(m_Roi_cenY[l_ni]);
                m_edit_ROI5_Radius.GetWindowText(m_Roi_radius[l_ni]);
                m_edit_ROI5_Width.GetWindowText(m_Roi_Width[l_ni]);
                break;
            } 
            
        }
        BOOL l_status = ValidateROIParams(m_Roi_cenX, m_Roi_cenY, m_Roi_radius, m_Roi_Width); 
        
        if(!l_status) {
            int l_nErrorMsgCode = I_FLOAT_VAL;
            (CTiPrepView::GetView())->DisplayErr(l_nErrorMsgCode,SET_ROI_DLG);
        }
    }catch(...){
        CString l_csErrMsg(_T("OnEnterKey : Exception Occured."));
        CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
        return;
    }
}

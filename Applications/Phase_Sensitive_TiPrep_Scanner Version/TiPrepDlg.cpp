/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#include "stdafx.h"
#include "tiprep.h"
#include "TiPrepDlg.h"
#include "DPSComManager.h"
#include "TiPrepView.h"
//#include "LocalDBMgr.h"
#include "TiPrepTrace.h"
#include "TiPrepLogHelper.h"
#include "ManualInitialGuessDlg.h"
#include "AboutDlg.h"
#include "TiPrepController.h"
#include <float.h>

#include <limits>
   using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

/**
* Represents.
*/
int time_data[20]       = {1000, 1299, 1599, 1899, 2199,
2500, 2800, 3100, 3400, 3699,
3999, 4299, 4599, 4899, 5199,
5500, 5799, 6100, 6399, 6700
};

/**
* Represents.
*/
double Org_intensity_data_ROI1[20]  = {105.492089, 80.962688, 61.285644, 35.250039,
20.367203, 19.945823, 35.296261, 47.281057, 64.933166, 83.077523, 101.103934, 115.561882,
128.992289, 141.597328, 151.65286, 162.620834, 179.746383, 189.403147, 197.7709, 202.76245
};

/**
* Represents.
*/
double Org_intensity_data_ROI2[20]  = {111.850064, 85.43238, 66.370931, 42.142115,
22.918909, 18.254628, 32.516359, 51.642172, 66.889703, 85.903354, 102.741728, 117.001173,
135.622303, 152.498856, 162.962551, 174.896696, 185.631469, 200.052092, 206.018083, 211.523397
};

/**
* Represents.
*/
double Org_intensity_data_ROI3[20]  = {108.964048, 80.614954, 56.4451, 35.545049, 22.583737,
22.482848, 31.96472, 54.689712, 69.109043, 82.792548, 100.174614, 115.669635, 130.338762,
142.493325, 154.726249, 172.691112, 183.008301, 190.644475, 199.044688, 207.101835
};


/**
* Represents color for background.
*/
#define BACKGROUNDCOLOR     RGB(27,36,54)

const CString EXCEPTION_TIPREPDLG = L"Exception occurred in TiPrep dialog";

/////////////////////////////////////////////////////////////////////////////
// CTiPrepDlg dialog

/**
* Constructor.
* This function is used for initialization of member variables.
*/
CTiPrepDlg::CTiPrepDlg(CWnd* pParent /*=NULL*/)
: CPopupDlg(CTiPrepDlg::IDD, pParent),
m_image_data_type_value(_T("")),
m_nAppOption(1),
m_pManualPhaseCorr(NULL),
m_polarity_correction_dlg(NULL),
m_AddOffsetDlg (NULL),
m_setroi_dlg(NULL),
m_AbtDlg(NULL),
m_ApplyProcessFlag(false),
m_flag(FALSE),
m_is_reselect_wait(false),
m_nEqnSelectFlag(0),
m_pReconDlg(NULL),
m_pCopyROIDlg(NULL),
m_dlgHeightOrg(0),m_progressdlg(NULL)
{
    /*TIPREP_TRACE(CTiPrepDlg::CTiPrepDlg);*/
    //{{AFX_DATA_INIT(CTiPrepDlg)
    m_image_data_type_value = _T(""); 
    //}}AFX_DATA_INIT
    m_hIcon = AfxGetApp()->LoadIcon(IDI_TIPREP_ICON2); //Inside the dialog's constructor.
}

CTiPrepDlg::~CTiPrepDlg()
{
    if(NULL != m_pManualPhaseCorr) {
        delete m_pManualPhaseCorr;
        m_pManualPhaseCorr = NULL;
    }
    if(NULL != m_polarity_correction_dlg) {
        delete m_polarity_correction_dlg;
        m_polarity_correction_dlg = NULL;
    }

    if(NULL != m_AddOffsetDlg) {
        delete m_AddOffsetDlg;
        m_AddOffsetDlg = NULL;
    }
    if(NULL != m_pReconDlg) {
        delete m_pReconDlg;
        m_pReconDlg = NULL;
    }
    if(NULL != m_setroi_dlg) {
        delete m_setroi_dlg;
        m_setroi_dlg = NULL;
    }
    if(NULL != m_AbtDlg) {
        delete m_AbtDlg;
        m_AbtDlg = NULL;
    }
    if(NULL != m_pCopyROIDlg) {
        delete m_pCopyROIDlg;
        m_pCopyROIDlg = NULL;
    }

	try{
		m_list_brush.DeleteObject();
	}catch(...)
	{
		CString l_csmsg(_T("Exception occurred in CTiPrepDlg"));
        CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
		
	}

}
CTiPrepDlg::CTiPrepDlg(CTiPrepDlg & f_dlg) : CPopupDlg(CTiPrepDlg::IDD)
{
    if(NULL != f_dlg.m_pManualPhaseCorr) {
        if(m_pManualPhaseCorr) {
            delete m_pManualPhaseCorr;
            m_pManualPhaseCorr = NULL;
        }
        m_pManualPhaseCorr = new CManualPhaseCorrDlg(*(f_dlg.m_pManualPhaseCorr));
    }
	if(NULL != f_dlg.m_pReconDlg) {
        if(m_pReconDlg) {
            delete m_pReconDlg;
            m_pReconDlg = NULL;
        }
        m_pReconDlg = new CReconstructionDlg(*(f_dlg.m_pReconDlg));
    }
	if(NULL != f_dlg.m_pCopyROIDlg) {
        if(m_pCopyROIDlg) {
            delete m_pCopyROIDlg;
            m_pCopyROIDlg = NULL;
        }
        m_pCopyROIDlg = new CROICopyDlg(*(f_dlg.m_pCopyROIDlg));
    }
	if(NULL != f_dlg.m_setroi_dlg) {
        if(m_setroi_dlg) {
            delete m_setroi_dlg;
            m_setroi_dlg = NULL;
        }
        m_setroi_dlg = new CSetROIParamsDlg(*(f_dlg.m_setroi_dlg));
    }
	if(NULL != f_dlg.m_AbtDlg) {
        if(m_AbtDlg) {
            delete m_AbtDlg;
            m_AbtDlg = NULL;
        }
        m_AbtDlg = new CAboutDlg(*(f_dlg.m_AbtDlg));
    }
	if(NULL != f_dlg.m_polarity_correction_dlg) {
        if(m_polarity_correction_dlg) {
            delete m_polarity_correction_dlg;
            m_polarity_correction_dlg = NULL;
        }
        m_polarity_correction_dlg = new CPolarityCorrectionDlg(*(f_dlg.m_polarity_correction_dlg));
    }
	if(NULL != f_dlg.m_AddOffsetDlg) {
        if(m_AddOffsetDlg) {
            delete m_AddOffsetDlg;
            m_AddOffsetDlg = NULL;
        }
        m_AddOffsetDlg = new CAddOffsetDlg(*(f_dlg.m_AddOffsetDlg));
    }
	m_is_reselect_wait = f_dlg.m_is_reselect_wait;
	m_nEqnSelectFlag = f_dlg.m_nEqnSelectFlag;
	m_ApplyProcessFlag = f_dlg.m_ApplyProcessFlag;
}
/*************************************************************
* Method:       DoDataExchange
* Description:  This function is used
* Parameter:    CDataExchange * pDX
* Returns:      void
*************************************************************/
void CTiPrepDlg::DoDataExchange(CDataExchange* pDX)
{
    TIPREP_TRACE(CTiPrepDlg::DoDataExchange,MRTraceMarker9);
    CPopupDlg::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CTiPrepDlg)
	DDX_Control(pDX, IDC_BUTTON_APPLYTI, m_ApplyTI);
    DDX_Control(pDX, IDC_STATIC_PHASE_CORRECTION_AUTO, m_Label_Auto_Phase_Correction);
    DDX_Control(pDX, IDC_STATIC_FILTERS, m_lbl_filter);
    DDX_CBString(pDX, IDC_IMAGE_DATA_TYPE, m_image_data_type_value);
    DDX_Control(pDX, IDC_STATIC_PIX_POS, m_lbl_pix_pos);
    DDX_Control(pDX, IDC_HIDE_BTN, m_hide_btn);
    DDX_Control(pDX, IDC_SELECTED_PROTOCOL_DISPLAY, m_selected_protocol_display);
    DDX_Control(pDX, IDC_IMAGE_DATA_TYPE, m_image_data_type_control);
    DDX_Control(pDX, IDC_STATIC_ONE_BUTTON_PHASE_CORRECTION, m_phase_correction_frame);
    DDX_Control(pDX, IDC_STATIC_AVERAGE_FILTER, m_select_filter_frame);
    DDX_Control(pDX, IDC_STATIC_SELECT_FILTER, m_select_filter_static_box);
    DDX_Control(pDX, IDC_COMBO_SELECT_FILTER, m_select_filter_combo_box);
    DDX_Control(pDX, IDC_STATIC_FILTER_SIZE, m_filter_size_static_box);
    DDX_Control(pDX, IDC_EDIT_FILTER_SIZE, m_filter_size_edit_box);
    DDX_Control(pDX, IDC_CHECK_CREATE_SERIES, m_create_series_check_box);
    DDX_Control(pDX, IDC_STATIC_CREATE_SERIES, m_create_series_static_box);
    DDX_Control(pDX, IDC_PHASE_CORRECTION, m_one_button_phase_correction);
    DDX_Control(pDX, IDC_STATIC_DATA_FORM, m_data_form);
    DDX_Control(pDX, IDC_STATIC_SERIES_NAME, m_static_series_name);
    DDX_Control(pDX, IDC_BUTTON_SAVE_ROI_DATA, m_SaveRoiDataToFileButton);
    DDX_Control(pDX, IDC_BUTTON_APPLY, m_ApplyButton);
    DDX_Control(pDX, IDC_BUTTON_RESELECT, m_ReselectButton);
    DDX_Control(pDX, IDC_BUTTON_RESETROI, m_ResetROIButton);
    DDX_Control(pDX, IDC_BUTTON_EXIT, m_ExitButton);
    DDX_Control(pDX, IDC_STATIC_DISPLAYOPTION, m_LabelDispOpt);
    DDX_Control(pDX, IDC_CHECK_ORGDATA, m_checkOrgData);
    DDX_Control(pDX, IDC_CHECK_INVDATA, m_checkInvData);
    DDX_Control(pDX, IDC_CHECK_FITDATA, m_checkFitData);
    DDX_Control(pDX, IDC_CHECK_EDITROI, m_checkEditROIData);
    DDX_Control(pDX, IDC_CHECK_ROI1_DISLPAY, m_checkDispROI1Data);
    DDX_Control(pDX, IDC_CHECK_ROI2_DISPLAY, m_checkDispROI2Data);
    DDX_Control(pDX, IDC_CHECK_ROI3_DISPLAY_NEW, m_checkDispROI3Data);
    DDX_Control(pDX, IDC_CHECK_ROI1_REVERSEMIN, m_checkRevDispROI1Data);
    DDX_Control(pDX, IDC_CHECK_ROI2_REVERSEMIN, m_checkRevDispROI2Data);
    DDX_Control(pDX, IDC_CHECK_ROI3_DISPLAY, m_checkRevDispROI3Data);
    DDX_Control(pDX, IDC_CHECK_T1MAP, m_checkT1Map);//KJ
    DDX_Control(pDX, IDC_RADIO_1STEQU, m_radioEqu1);
    DDX_Control(pDX, IDC_EDIT_ROI1FITTINGERROR, m_ROI1FitErr);
    DDX_Control(pDX, IDC_EDIT_ROI2FITTINGERROR, m_ROI2FitErr);
    DDX_Control(pDX, IDC_EDIT_ROI3FITTINGERROR, m_ROI3FitErr);
    DDX_Control(pDX, IDC_EDIT_ROI1TI, m_ROI1TiVal);
    DDX_Control(pDX, IDC_EDIT_ROI2TI, m_ROI2TiVal);
    DDX_Control(pDX, IDC_EDIT_ROI3TI, m_ROI3TiVal);
    DDX_Control(pDX, IDC_EDIT_MEANTI, m_ROIMeanTiVal);
    DDX_Control(pDX, IDC_EDIT_ROI1T1, m_ROI1T1Val);
    DDX_Control(pDX, IDC_EDIT_ROI2T1, m_ROI2T1Val);
    DDX_Control(pDX, IDC_EDIT_ROI3T1, m_ROI3T1Val);
    DDX_Control(pDX, IDC_EDIT_MEANT1, m_ROIMeanT1Val);
    DDX_Control(pDX, IDC_EDIT_REPETITIONTIME, m_RepTime);
    DDX_Control(pDX, IDC_STATIC_FRAME_DISPLAY_OPTION, m_displayOption);
    DDX_Control(pDX, IDC_STATIC_DISPLAY, m_Display);
    DDX_Control(pDX, IDC_STATIC_REVERSEMIN, m_reverseMin);
    DDX_Control(pDX, IDC_STATIC_FITTINGERROR, m_fittingErr);
    DDX_Control(pDX, IDC_STATIC_INVERSIONTIME, m_inversionTime);
    DDX_Control(pDX, IDC_STATIC_RECOVERYTIME, m_recoveryTime);
    DDX_Control(pDX, IDC_STATIC_ROI1, m_lableROI1);
    DDX_Control(pDX, IDC_STATIC_ROI2, m_lableROI2);
    DDX_Control(pDX, IDC_STATIC_ROI3, m_lableROI3);
    DDX_Control(pDX, IDC_STATIC_MEAN, m_lableMean);
    DDX_Control(pDX, IDC_STATIC_REPETITIONTIME, m_lableRepetitionTime);
    DDX_Control(pDX, IDC_STATIC_FRAME_CALCULATED_DATA, m_dataCalc);
    DDX_Control(pDX, IDC_STATIC_FRAME_EQUETN_WINDOW, m_equetionFrame);
    DDX_Control(pDX, IDC_CHECK_NOISE_ROI1_DISLPAY, m_checkNoiseROI1Data);
    DDX_Control(pDX, IDC_CHECK_NOISE_ROI2_DISLPAY, m_checkNoiseROI2Data);
    DDX_Control(pDX, IDC_CHECK_OFFSET_CORRECTION_DISLPAY, m_checkOffsetCorrection);
    DDX_Control(pDX, IDC_EDIT_NOISE_CORRECTION_VALUE, m_NoiseCorrectionVal);
    DDX_Control(pDX, IDC_STATIC_FRAME_NOISE_CORRECTION, m_NoiseCorrectionFrame);
    DDX_Control(pDX, IDC_STATIC_NOISE_CORRECTION, m_LabelNoiseCorrection);
    DDX_Control(pDX, IDC_STATIC_NOISE_CORRECTION_VALUE, m_LabelNoiseCorrectionVal);
    DDX_Control(pDX, IDC_CHECK_MANUAL_GUESS, m_initialGuessChk);
    DDX_Control(pDX, IDC_BUTTON_MANUAL_GUESS, m_initialGuessApply);
    DDX_Control(pDX, IDC_BUTTON_DATA_TYPE, m_bxDataType);
    DDX_Control(pDX, IDC_EDIT_DATA_TYPE, m_exDataType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTiPrepDlg, CPopupDlg)
//{{AFX_MSG_MAP(CTiPrepDlg)
ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
ON_WM_WINDOWPOSCHANGING()
ON_WM_ACTIVATE()
ON_WM_CLOSE()
ON_WM_CTLCOLOR()
ON_COMMAND(ID_OPTIONS_COPYROI, OnButtonCopyROI)
ON_COMMAND(ID_OPTIONS_SETROI, OnButtonSetROI)
ON_COMMAND(ID_OPTIONS_RECONSTRUCTION, OnButtonRecon)
ON_COMMAND(ID_OPTIONS_POLARITYCORRECTION, OnMenuPolarityCorrection) //priyanka - polarity correction
ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
ON_BN_CLICKED(IDC_BUTTON_RESELECT, OnButtonReselect)
ON_BN_CLICKED(IDC_CHECK_ORGDATA, OnCheckOrgdata)
ON_BN_CLICKED(IDC_CHECK_INVDATA, OnCheckInvdata)
ON_BN_CLICKED(IDC_CHECK_FITDATA, OnCheckFitdata)
ON_BN_CLICKED(IDC_CHECK_ROI1_DISLPAY, OnCheckRoi1Dislpay)
ON_BN_CLICKED(IDC_CHECK_ROI2_DISPLAY, OnCheckRoi2Display)
ON_BN_CLICKED(IDC_CHECK_ROI3_DISPLAY_NEW, OnCheckRoi3Display)
ON_BN_CLICKED(IDC_CHECK_NOISE_ROI1_DISLPAY, OnCheckNoiseRoi1Display)
ON_BN_CLICKED(IDC_CHECK_NOISE_ROI2_DISLPAY, OnCheckNoiseRoi2Display)
ON_BN_CLICKED(IDC_CHECK_OFFSET_CORRECTION_DISLPAY, OnCheckOffsetCorrection)
ON_BN_CLICKED(IDC_CHECK_CREATE_SERIES, OnCheckCreateSeriesinAverageFilter) //+priyanka-Average-Filter
ON_BN_CLICKED(IDC_CHECK_ROI1_REVERSEMIN, OnCheckRoi1Reversemin)
ON_BN_CLICKED(IDC_CHECK_ROI2_REVERSEMIN, OnCheckRoi2Reversemin)
ON_BN_CLICKED(IDC_CHECK_ROI3_DISPLAY, OnCheckRoi3Reversemin)
ON_BN_CLICKED(IDC_RADIO_1STEQU, OnRadio1stequ)
ON_BN_CLICKED(IDC_RADIO_2NDEQU, OnRadio2ndequ)
ON_BN_CLICKED(IDC_RADIO_3RDEQU, OnRadio3rdequ)
ON_BN_CLICKED(IDC_RADIO_4THEQU, OnRadio4rthequ)
ON_BN_CLICKED(IDC_RADIO_5THEQU, OnRadio5thequ)
ON_BN_CLICKED(IDC_BUTTON_RESETROI, OnButtonResetroi)
ON_BN_CLICKED(IDC_CHECK_EDITROI, OnCheckEditROI)
ON_WM_SETCURSOR()
ON_BN_CLICKED(IDC_PHASE_CORRECTION, OnPhaseCorrectionClick)
ON_CBN_EDITCHANGE(IDC_IMAGE_DATA_TYPE, OnDataTypeDisplay)
ON_BN_CLICKED(IDC_BUTTON_SAVE_ROI_DATA, OnButtonSaveRoiData)
ON_BN_CLICKED(IDC_HIDE_BTN, OnHideBtnClick)
ON_COMMAND(ID_OPTIONS_ADDOFFSET, OnMenuAddoffset)
ON_BN_CLICKED(IDC_CHECK_T1MAP, OnCheckT1map)
ON_BN_CLICKED(IDC_CHECK_MANUAL_GUESS, OnCheckManualGuess)
ON_BN_CLICKED(IDC_BUTTON_MANUAL_GUESS, OnButtonManualGuess)
ON_COMMAND(ID_OPTIONS_MANUAL_PHASE_CORR, OnOptionsManualPhaseCorr)
ON_BN_CLICKED(IDC_BUTTON_DATA_TYPE, OnButtonGetDataType)
	ON_COMMAND(ID_HELP_ABOUT, OnAboutMenu)
ON_WM_SYSCOMMAND()
	ON_MESSAGE(WM_UPDATE_PROGRESS, OnUpdateProgress)
	ON_BN_CLICKED(IDC_BUTTON_APPLYTI, OnButtonApply_Ti)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
LRESULT CTiPrepDlg::OnUpdateProgress(WPARAM wParam, LPARAM lParam)
{
	BOOL threadstatus  = (BOOL)wParam;
	if (m_progressdlg && m_progressdlg->GetSafeHwnd())
		m_progressdlg->UpdateProgress(threadstatus);

return 0;
}
/////////////////////////////////////////////////////////////////////////////
// CTiPrepDlg message handlers

/*************************************************************
* Method:       OnInitDialog
* Description:  This function is used
* Returns:      BOOL
*************************************************************/
BOOL CTiPrepDlg::OnInitDialog()
{
    TIPREP_TRACE(CTiPrepDlg::OnInitDialog,MRTraceMarker3);
    DialogEx::OnInitDialog();
    
    try {
        SetIcon(m_hIcon, TRUE);			// Set big icon
        SetIcon(m_hIcon, FALSE);		// Set small icon

        WINDOW_COLORS_t l_window_color;
        GetWindowColors(l_window_color);
        m_list_brush.CreateSolidBrush(l_window_color.crClientColor);
        InitializeControls();
        BOOL l_bCreateGraph = CreateGraphCtrl();
        
        if(!l_bCreateGraph) {
            CString l_csErrMsg(_T("Unable to create graph area."));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return FALSE;
        }
        
        if((CTiPrepView::GetView())->IsWaitIsel()) {
            (CTiPrepView::GetView())->SetWaitCursor(true);
        }
        
        long l_nNumImgSel = 0;
        int l_nStatus = (CTiPrepView::GetView())->getNumImgSelectedISel(l_nNumImgSel);
        
        if(l_nStatus == E_NO_ERROR && l_nNumImgSel != 0) {
            //
        }
        
        (CTiPrepView::GetView())->DisplaySeriesNumber(); 
        m_nEqnSelectFlag = 0;//1st eqn selected on init.
        SetDlgPos();
        EnableFilterCtrls(false);
        
        if(ALL_APP == m_nAppOption) {
            //do nothing
        }
        
        if(TIPREP_ONLY == m_nAppOption) {
            (CTiPrepView::GetView())->SetT1MapFlag(false);
            m_checkT1Map.ShowWindow(FALSE);
            m_checkT1Map.EnableWindow(FALSE);
            m_select_filter_combo_box.ShowWindow(false);
            m_filter_size_edit_box.ShowWindow(false);
            m_create_series_check_box.ShowWindow(false);
            GetDlgItem(IDC_STATIC_AVERAGE_FILTER)->ShowWindow(false);
            GetDlgItem(IDC_STATIC_FILTERS)->ShowWindow(false);
            GetDlgItem(IDC_STATIC_SELECT_FILTER)->ShowWindow(false);
            GetDlgItem(IDC_STATIC_FILTER_SIZE)->ShowWindow(false);
            GetDlgItem(IDC_STATIC_CREATE_SERIES)->ShowWindow(false);
            EnableAvgMode();
        }
        
        m_filter_size_edit_box.SetWindowText(_T("3"));
    } catch(...) { 
        CTiPrepLogHelper::WriteToErrorLog(_T("Excpetion oninitdlg"), __LINE__, __FILE__);
    }
    
    return TRUE;  
}

/*************************************************************
* Method:       SetAppOption
* Description:  This function is used
* Parameter:    int f_nAppOption :
* Returns:      BOOL
*************************************************************/
BOOL CTiPrepDlg::SetAppOption(int f_nAppOption)
{
    m_nAppOption = f_nAppOption;
    return TRUE;
}

/*************************************************************
* Method:       PreTranslateMessage
* Description:  This function processes the Pre translate message
*               of dialog box for navigation purpose.
* Parameter:    MSG * msg
*               Message information.
* Returns:      BOOL
*************************************************************/
BOOL CTiPrepDlg::PreTranslateMessage(MSG* msg)
{
    //TIPREP_TRACE(CTiPrepDlg::PreTranslateMessage,MRTraceMarker9);
    
    if(msg->message == WM_KEYDOWN) {
        if(msg->wParam == VK_RETURN || msg->wParam == VK_ESCAPE) {
            
            if (GetFocus() == GetDlgItem(IDC_EDIT_FILTER_SIZE)) 
            {
                if (CTiPrepView::GetView()->GetT1MapFlag() && IsAvgFilterOn())
                {
                    int filtersz = 0;
                    int status = GetFilterSize(filtersz);
                    if (status != E_NO_ERROR) 
                        DisplayErr(status);
                }
            }
            OutputDebugString(_T("OK/ESCAPE"));
        }
    }else if (msg->message == WM_LBUTTONDOWN){
        CRect rect;
        CWnd * wnd = NULL;
        wnd = GetDlgItem(IDC_BUTTON_APPLY);
        if (wnd) {
            wnd->GetWindowRect(&rect);
            
            ScreenToClient(&rect);
            ScreenToClient(&msg->pt);
           
            if (msg->pt.x >= rect.left && 
                msg->pt.x < (rect.right ) &&
                msg->pt.y >= rect.top &&
                msg->pt.y < (rect.bottom)
                ){
                 //SetDlgItemText(IDC_BUTTON_APPLY,_T("CLICK"));
                if(true == m_ApplyProcessFlag){
                   // SetDlgItemText(IDC_BUTTON_APPLY,_T("YES"));
                    return(TRUE);
                }
            }
        }
    }
    return CPopupDlg::PreTranslateMessage(msg);
}

/*************************************************************
* Method:       OnButtonExit
* Description:  This function is message handler.
*               Called when 'Exit' button is clicked.
* Returns:      void
*************************************************************/
void CTiPrepDlg::OnButtonExit()
{
    TIPREP_TRACE(CTiPrepDlg::OnButtonExit,MRTraceMarker3);
    (CTiPrepView::GetView())->OnExit();
}

/*************************************************************
* Method:       OnWindowPosChanging
* Description:  This function is used
* Parameter:    WINDOWPOS * lpwndpos
* Returns:      void
*************************************************************/
void CTiPrepDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
    TIPREP_TRACE(CTiPrepDlg::OnWindowPosChanging,MRTraceMarker9);
    lpwndpos->flags = lpwndpos->flags | SWP_NOREPOSITION | SWP_NOZORDER | SWP_NOSENDCHANGING;
}

/*************************************************************
* Method:       OnActivate
* Description:  This function is used
* Parameter:    UINT nState
* Parameter:    CWnd * pWndOther
* Parameter:    BOOL bMinimized
* Returns:      void
*************************************************************/
void CTiPrepDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
    /*TIPREP_TRACE(CTiPrepDlg::OnActivate,MRTraceMarker9);*/
    DialogEx::OnActivate(nState, pWndOther, bMinimized);
    
    if(WA_INACTIVE != nState) {
        CWnd* pChildWnd = this ? this->GetNextWindow(GW_HWNDPREV) : NULL;
        
        while(pChildWnd) {
            {
                if(pChildWnd->IsWindowVisible()) {
                    pChildWnd->SetFocus();
                    break;
                }
            }
            pChildWnd = pChildWnd->GetNextWindow(GW_HWNDPREV);
            if (pChildWnd == this) break; 
        }
        
        (CTiPrepView::GetView())->ChangeWindowZOrder(this->GetSafeHwnd());
        
        if(pChildWnd && pChildWnd != this) {
            (CTiPrepView::GetView())->ChangeWindowZOrder(pChildWnd->GetSafeHwnd());
        }
        
        if((CTiPrepView::GetView())->GetChildHWND()) {
            (CTiPrepView::GetView())->ChangeWindowZOrder((CTiPrepView::GetView())->GetChildHWND());
        }
    }
}

/*************************************************************
* Method:       OnClose
* Description:  This function is message handler.
*               Called when 'Close' button is clicked.
* Returns:      void
*************************************************************/
void CTiPrepDlg::OnClose()
{
    TIPREP_TRACE(CTiPrepDlg::OnClose,MRTraceMarker3);
    (CTiPrepView::GetView())->OnExit();
}

/*************************************************************
* Method:       InitializeControls
* Description:  This function initializes all controls to give
*               better look & feel.
* Returns:      void
*************************************************************/
void CTiPrepDlg::InitializeControls()
{
    TIPREP_TRACE(CTiPrepDlg::InitializeControls,MRTraceMarker9);
    SetRibbonHeight(50);//KJ
    
    m_ApplyTI.SetPrimaryTextFont(_T("Arial"), FONT_SIZE, true);
    m_ApplyTI.SetButtonType(TUSBUTTON_ROUND_ALL);

    m_SaveRoiDataToFileButton.SetPrimaryTextFont(_T("Arial"), FONT_SIZE, true);
    m_SaveRoiDataToFileButton.SetButtonType(TUSBUTTON_ROUND_ALL);
    m_ApplyButton.SetPrimaryTextFont(_T("Arial"), FONT_SIZE, true);
    m_ApplyButton.SetButtonType(TUSBUTTON_ROUND_ALL);
    m_ReselectButton.SetPrimaryTextFont(_T("Arial"), FONT_SIZE, true);
    m_ReselectButton.SetButtonType(TUSBUTTON_ROUND_ALL);
    m_ResetROIButton.SetPrimaryTextFont(_T("Arial"), FONT_SIZE, true);
    m_ResetROIButton.SetButtonType(TUSBUTTON_ROUND_ALL);
    m_ExitButton.SetPrimaryTextFont(_T("Arial"), FONT_SIZE, true);
    m_ExitButton.SetButtonType(TUSBUTTON_ROUND_ALL);
   // m_SelectMgImgButton.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
   // m_SelectMgImgButton.SetButtonType(TUSBUTTON_ROUND_ALL);
   // m_SelectReImgButton.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
   // m_SelectReImgButton.SetButtonType(TUSBUTTON_ROUND_ALL);
  //  m_SelectImImgButton.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
  //  m_SelectImImgButton.SetButtonType(TUSBUTTON_ROUND_ALL);
  //  m_CorrectPhaseImgButton.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
  //  m_CorrectPhaseImgButton.SetButtonType(TUSBUTTON_ROUND_ALL);
  //  m_SelectReconButton.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
  //  m_SelectReconButton.SetButtonType(TUSBUTTON_ROUND_ALL);
  //  m_ReconstructionButton.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
  //  m_ReconstructionButton.SetButtonType(TUSBUTTON_ROUND_ALL);
  //  m_CopyButton.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
  //  m_CopyButton.SetButtonType(TUSBUTTON_ROUND_ALL);
    m_hide_btn.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
    m_hide_btn.SetButtonType(TUSBUTTON_ROUND_ALL);
    m_LabelDispOpt.SetFont(&m_font, true);
    m_inversionTime.SetFont(&m_font, true);
   // m_invTI.SetFont(&m_font, true);
    m_recoveryTime.SetFont(&m_font, true);
   // m_recoveryT1.SetFont(&m_font, true);
    m_fittingErr.SetFont(&m_font, true); 
    m_reverseMin.SetFont(&m_font, true);
    m_Display.SetFont(&m_font, true);
    m_lableRepetitionTime.SetFont(&m_pLabel_Font, true);
    m_lableMean.SetFont(&m_pLabel_Font, true);
    m_lableROI1.SetFont(&m_font, true);
    m_lableROI2.SetFont(&m_font, true);
    m_lableROI3.SetFont(&m_font, true);
    m_static_series_name.SetFont(&m_pLabel_Font, true);
    m_lbl_filter.SetFont(&m_font, true); 
    m_lbl_pix_pos.SetFont(&m_pLabel_Font, true); 
    m_checkOrgData.SetBkColor(BKCOLOR);
    m_checkOrgData.SetFont(FONT, TEXT_SIZE);
    m_checkOrgData.SetCheck(TRUE);
    m_checkInvData.SetBkColor(BKCOLOR);
    m_checkInvData.SetFont(FONT, TEXT_SIZE);
    m_checkInvData.SetCheck(TRUE);
    m_checkEditROIData.SetBkColor(BKCOLOR);
    m_checkEditROIData.SetFont(FONT, TEXT_SIZE);
    m_checkEditROIData.SetCheck(TRUE);
    m_checkFitData.SetBkColor(BKCOLOR);
    m_checkFitData.SetFont(FONT, TEXT_SIZE);
    m_checkFitData.SetCheck(TRUE);
    m_checkDispROI1Data.SetBkColor(BKCOLOR);
    m_checkDispROI1Data.SetFont(FONT, TEXT_SIZE);
    m_checkDispROI1Data.SetCheck(TRUE);
    m_checkDispROI2Data.SetBkColor(BKCOLOR);
    m_checkDispROI2Data.SetFont(FONT, FONT_SIZE);
    m_checkDispROI2Data.SetCheck(FALSE);
    m_checkDispROI3Data.SetBkColor(BKCOLOR);
    m_checkDispROI3Data.SetFont(FONT, FONT_SIZE);
    m_checkDispROI3Data.SetCheck(FALSE);
    m_checkRevDispROI1Data.SetBkColor(BKCOLOR);
    m_checkRevDispROI1Data.SetFont(FONT, FONT_SIZE);
    m_checkRevDispROI1Data.SetCheck(FALSE);
    m_checkRevDispROI2Data.SetBkColor(BKCOLOR);
    m_checkRevDispROI2Data.SetFont(FONT, FONT_SIZE);
    m_checkRevDispROI2Data.SetCheck(FALSE);
    m_checkRevDispROI3Data.SetBkColor(BKCOLOR);
    m_checkRevDispROI3Data.SetFont(FONT, FONT_SIZE);
    m_checkRevDispROI3Data.SetCheck(FALSE);
    m_checkNoiseROI1Data.SetBkColor(BKCOLOR);
    m_checkNoiseROI1Data.SetFont(FONT, TEXT_SIZE);
    m_checkNoiseROI1Data.SetCheck(FALSE);
    m_checkNoiseROI2Data.SetBkColor(BKCOLOR);
    m_checkNoiseROI2Data.SetFont(FONT, TEXT_SIZE);
    m_checkNoiseROI2Data.SetCheck(FALSE);
    m_checkOffsetCorrection.SetBkColor(BKCOLOR);
    m_checkOffsetCorrection.SetFont(FONT, TEXT_SIZE);
    m_checkOffsetCorrection.SetCheck(FALSE);
    m_checkT1Map.SetBkColor(BKCOLOR);
    m_checkT1Map.SetFont(FONT, TEXT_SIZE);
    m_checkT1Map.SetCheck(FALSE);
    m_NoiseCorrectionVal.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_NoiseCorrectionVal.SetReadOnly(TRUE);
    m_LabelNoiseCorrection.SetFont(&m_font, true);
    m_LabelNoiseCorrectionVal.SetFont(&m_pLabel_Font, true);
    m_radioEqu1.SetBkColor(BKCOLOR);
    m_radioEqu1.SetFont(FONT, TEXT_SIZE);
    m_radioEqu1.SetCheck(TRUE);
    m_radioEqu2.SetBkColor(BKCOLOR);
    m_radioEqu2.SetFont(FONT, TEXT_SIZE);
    m_radioEqu2.SetCheck(FALSE);
    m_radioEqu3.SetBkColor(BKCOLOR);
    m_radioEqu3.SetFont(FONT, TEXT_SIZE);
    m_radioEqu3.SetCheck(FALSE);
    m_radioEqu4.SetBkColor(BKCOLOR);
    m_radioEqu4.SetFont(FONT, TEXT_SIZE);
    m_radioEqu4.SetCheck(FALSE);
    m_radioEqu5.SetBkColor(BKCOLOR);
    m_radioEqu5.SetFont(FONT, TEXT_SIZE);
    m_radioEqu5.SetCheck(FALSE);
    m_ROI1FitErr.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_ROI1FitErr.SetReadOnly(TRUE);
    m_ROI2FitErr.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_ROI2FitErr.SetReadOnly(TRUE);
    m_ROI3FitErr.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_ROI3FitErr.SetReadOnly(TRUE);
    m_ROI1TiVal.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_ROI1TiVal.SetReadOnly(TRUE);
    m_ROI2TiVal.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_ROI2TiVal.SetReadOnly(TRUE);
    m_ROI3TiVal.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_ROI3TiVal.SetReadOnly(TRUE);
    m_ROIMeanTiVal.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_ROIMeanTiVal.SetReadOnly(TRUE);
    m_ROI1T1Val.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_ROI1T1Val.SetReadOnly(TRUE);
    m_ROI2T1Val.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_ROI2T1Val.SetReadOnly(TRUE);
    m_ROI3T1Val.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_ROI3T1Val.SetReadOnly(TRUE);
    m_ROIMeanT1Val.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_ROIMeanT1Val.SetReadOnly(TRUE);
   // m_Recon.SetFont(_T("Arial"), TEXT_SIZE, true);
    //m_Recon.SetReadOnly(TRUE);
    m_RepTime.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_RepTime.SetBkColor(BKCOLOR);
//    m_listctrl_scannedProtocol.SetFont(_T("Arial"), TEXT_SIZE, true);
    //insert columns in list control
//    m_listctrl_scannedProtocol.InsertColumn(0, _T("Scanned Protocol"), LVCFMT_LEFT, 120);
//    m_listctrl_scannedProtocol.SetColumnRange(0, 120, 120);
//    m_listctrl_Reocn.SetFont(_T("Arial"), TEXT_SIZE, true);
    //insert columns in list control
//    m_listctrl_Reocn.InsertColumn(0, _T("P Data Form"), LVCFMT_LEFT, 120);
//    m_listctrl_Reocn.SetColumnRange(0, 120, 120);
    m_menufilepopup.CreatePopupMenu();
    m_menufilepopup.AppendMenu(MF_STRING, ID_OPTIONS_SETROI, MENU_SETROI);
    m_menufilepopup.AppendMenu(MF_STRING, ID_OPTIONS_COPYROI, MENU_COPYROI);
    m_menufilepopup.AppendMenu(MF_STRING, ID_OPTIONS_RECONSTRUCTION, MENU_RECON);
    m_menufilepopup.AppendMenu(MF_STRING, ID_OPTIONS_POLARITYCORRECTION, MENU_POLARITYCORRECTION); 
    m_menufilepopup.AppendMenu(MF_STRING, ID_OPTIONS_ADDOFFSET, MENU_ADD_OFFSET);
    m_menufilepopup.AppendMenu(MF_STRING, ID_OPTIONS_MANUAL_PHASE_CORR, MENU_MANUAL_PHASE_CORR);
    m_menufilepopup.SetFont(FONT, FONT_SIZE);

    m_menuAboutpopup.CreatePopupMenu();
    m_menuAboutpopup.AppendMenu(MF_STRING, ID_HELP_ABOUT, MENU_ABOUT);
    m_menuAboutpopup.SetFont(FONT, FONT_SIZE);

    m_menufile.CreateMenu();
    m_menufile.AppendMenu(MF_BYPOSITION | MF_POPUP, (UINT)m_menufilepopup.GetSafeHmenu(), MENU_OPTIONS);

    m_menufile.AppendMenu(MF_BYPOSITION | MF_POPUP, (UINT)m_menuAboutpopup.GetSafeHmenu(), MENU_HELP);

    m_menufile.SetFont(FONT, FONT_SIZE);
    m_menufile.SetMenuBar(this);
    m_displayOption.SetFrameColor(FRAME_BKCOLOR);
    m_dataCalc.SetFrameColor(FRAME_BKCOLOR);
    m_equetionFrame.SetFrameColor(FRAME_BKCOLOR);
    m_NoiseCorrectionFrame.SetFrameColor(FRAME_BKCOLOR);    
    //for phase correction outer frame
    m_phase_correction_frame.SetFrameColor(FRAME_BKCOLOR);
    m_select_filter_frame.SetFrameColor(FRAME_BKCOLOR);
    m_select_filter_static_box.SetFont(&m_pLabel_Font, true);
    m_select_filter_combo_box.AddString(_T("None"));
    m_select_filter_combo_box.AddString(_T("Average"));
    m_select_filter_combo_box.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_select_filter_combo_box.SetCurSel(0);
    m_filter_size_static_box.SetFont(&m_pLabel_Font, true);
    m_filter_size_edit_box.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_create_series_check_box.SetBkColor(BKCOLOR);
    m_create_series_check_box.SetFont(FONT, TEXT_SIZE);
    m_create_series_check_box.SetCheck(FALSE);
    m_create_series_static_box.SetFont(&m_pLabel_Font, true);
    //one button  phase correction button settings
    m_one_button_phase_correction.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
    m_one_button_phase_correction.SetButtonType(TUSBUTTON_ROUND_ALL);
    //for combo box
    m_image_data_type_control.AddString(_T("Real"));
    m_image_data_type_control.AddString(_T("Imaginary"));
    m_image_data_type_control.AddString(_T("Magnitude"));
    m_image_data_type_control.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_image_data_type_control.SetCurSel(0);
    //for data form static box
    m_data_form.SetFont(&m_pLabel_Font, true);
    // for  protocol display
    m_selected_protocol_display.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_selected_protocol_display.SetReadOnly(TRUE);
    m_initialGuessApply.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
    m_initialGuessApply.SetButtonType(TUSBUTTON_ROUND_ALL);
    m_initialGuessChk.SetBkColor(BKCOLOR);
    m_initialGuessChk.SetFont(FONT, TEXT_SIZE);
    m_initialGuessChk.SetCheck(FALSE);
    m_bxDataType.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
    m_bxDataType.SetButtonType(TUSBUTTON_ROUND_ALL);
    m_exDataType.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_exDataType.SetReadOnly(TRUE);
    m_Label_Auto_Phase_Correction.SetFont(&m_font, true);
    GetDlgItem(IDC_STATIC_NOISE_ROI1)->SetFont(&m_pLabel_Font, true);
    GetDlgItem(IDC_STATIC_NOISE_ROI2)->SetFont(&m_pLabel_Font, true);
    GetDlgItem(IDC_STATIC_OFFSET_CORRECTION)->SetFont(&m_pLabel_Font, true);
}



/*************************************************************
* Method:       OnCtlColor
* Description:  This function is used
* Parameter:    CDC * pDC
* Parameter:    CWnd * pWnd
* Parameter:    UINT nCtlColor
* Returns:      HBRUSH
*************************************************************/
HBRUSH CTiPrepDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    //TIPREP_TRACE(CTiPrepDlg::OnCtlColor,MRTraceMarker9);
    HBRUSH hbr = DialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
    
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
* Method:       OnButtonCopyROI
* Description:  This function is message handler.
*               Called when 'Copy ROI' is clicked under
*               'Options' menu.
* Returns:      void
*************************************************************/
void CTiPrepDlg::OnButtonCopyROI()
{
    TIPREP_TRACE(CTiPrepDlg::OnButtonCopyROI,MRTraceMarker9);

    try{
    if (m_pCopyROIDlg && m_pCopyROIDlg->GetSafeHwnd()) return;
    long l_nImgSel = 0;
    int status = 0;
    status = (CTiPrepView::GetView())->getNumImgSelectedISel(l_nImgSel);
    if (status != E_NO_ERROR || l_nImgSel <= 0)
    {
        int err = E_APPLY_IMAGE_NOT_PROPER;
        DisplayErr(err);
        return;
    }
	if (m_pCopyROIDlg && m_pCopyROIDlg->GetSafeHwnd()) return;
    m_pCopyROIDlg = new CROICopyDlg(this);
    if (NULL != m_pCopyROIDlg)
    {
        m_pCopyROIDlg->DoModal();
    }
    if (m_pCopyROIDlg)
        delete m_pCopyROIDlg;
    m_pCopyROIDlg  =NULL;

    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_TIPREPDLG, __LINE__, __FILE__);
    }
}

/*************************************************************
* Method:       OnButtonSetROI
* Description:  This function is message handler.
*               Called when 'Set ROI' is clicked under
*               'Options' menu.
* Returns:      void
*************************************************************/
void CTiPrepDlg::OnButtonSetROI()
{
    TIPREP_TRACE(CTiPrepDlg::OnButtonSetROI,MRTraceMarker9);
    
    /*m_ptr_dlg = new CSetROIParamsDlg();
    if (NULL != m_ptr_dlg)
    {
    m_ptr_dlg->DoModal();
}*/
    try {
	if (m_setroi_dlg && m_setroi_dlg->GetSafeHwnd()) return; 
	long l_nImgSel = 0;
    int status = 0;
    status = (CTiPrepView::GetView())->getNumImgSelectedISel(l_nImgSel);
    if (status != E_NO_ERROR || l_nImgSel <= 0)
    {
        int err = E_APPLY_IMAGE_NOT_PROPER;
        DisplayErr(err);
        return;
    }	

    m_setroi_dlg = new CSetROIParamsDlg(this);
    if (NULL != m_setroi_dlg)//AS/added
        {
            if(BST_CHECKED == m_checkT1Map.GetCheck()){ // if map mode is selected
                m_setroi_dlg->SetT1MapMode(true);
            }else{  //indicates avg mode
                m_setroi_dlg->SetT1MapMode(false);
            }
            m_setroi_dlg->DoModal();
        }
    if (m_setroi_dlg)
    {
        delete m_setroi_dlg;
        m_setroi_dlg = NULL;
    }
    } catch (...)
    {
        CTiPrepLogHelper::WriteToErrorLog(_T("Exception occurred in OnButtonSetROI"),__LINE__,__FILE__);
    }
}

/*************************************************************
* Method:       OnButtonRecon
* Description:  This function is message handler.
*               Called when 'Reconstruction' is clicked
*               under 'Options' menu.
* Returns:      void
*************************************************************/
void CTiPrepDlg::OnButtonRecon()
{
    TIPREP_TRACE(CTiPrepDlg::OnButtonRecon,MRTraceMarker9);
    
    try{
        if(m_pReconDlg && m_pReconDlg->GetSafeHwnd()) {
            return;
        }
        
        m_pReconDlg = new CReconstructionDlg(this); 
        
        if(NULL != m_pReconDlg) {
            m_pReconDlg->DoModal();
        }
        
        if(m_pReconDlg) {
            delete m_pReconDlg;
            m_pReconDlg = NULL;
        }
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_TIPREPDLG, __LINE__, __FILE__);
    }
}

/*************************************************************
* Method:       OnMenuPolarityCorrection
* Description:  This function is used
* Returns:      void
*************************************************************/
void CTiPrepDlg::OnMenuPolarityCorrection()
{
    TIPREP_TRACE(CTiPrepDlg::OnMenuPolarityCorrection,MRTraceMarker9);
	if(m_polarity_correction_dlg && m_polarity_correction_dlg->GetSafeHwnd()) return;
    m_polarity_correction_dlg = new CPolarityCorrectionDlg(this);
    if (NULL != m_polarity_correction_dlg)
    {
        m_polarity_correction_dlg->DoModal();
    }
    if (m_polarity_correction_dlg)
    {
        delete m_polarity_correction_dlg;
        m_polarity_correction_dlg = NULL;
	}
}

/*************************************************************
* Method:       OnButtonApply
* Description:  This function is message handler.
*               Called when 'Apply' button is clicked.
* Returns:      void
*************************************************************/
void CTiPrepDlg::OnButtonApply()
{
    TIPREP_TRACE(CTiPrepDlg::OnButtonApply,MRTraceMarker3);
    
    
    try{
        m_tigraphctrl.DisplayGraph(FALSE);
        int status = (CTiPrepView::GetView())->OnApply(m_nEqnSelectFlag);
        if (status != E_NO_ERROR){
            DisplayErr(status); 
        }
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_TIPREPDLG, __LINE__, __FILE__);
    }
}

/*************************************************************
* Method:       CreateGraphCtrl
* Description:  This function is used to create Graph area on GUI.
* Returns:      BOOL
*************************************************************/
BOOL CTiPrepDlg::CreateGraphCtrl()
{
    TIPREP_TRACE(CTiPrepDlg::CreateGraphCtrl,MRTraceMarker9);
    
    try{
        CWnd* l_wnd = GetDlgItem(IDC_EDIT_GRAPH);
        CString l_temp_str = _T("");
        
        if(l_wnd) {
            CRect graph_rect;
            l_wnd->GetWindowRect(&graph_rect);
            ScreenToClient(&graph_rect);
            l_wnd->DestroyWindow();
            BOOL bret = m_tigraphctrl.Create(l_temp_str,
                WS_CHILD | WS_VISIBLE | WS_BORDER ,
                graph_rect,
                this,
                IDC_EDIT_GRAPH);
            return bret;
        }
        
        return FALSE;
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_TIPREPDLG, __LINE__, __FILE__);
        return FALSE;
    }
}

/*************************************************************
* Method:       getMeanT1
* Description:  This function is used to calculate average
*               value of T1(Recovery time) of selected ROI's.
* Parameter:    BOOL * f_roi
*               Selected ROI status.
* Parameter:    BOOL * f_roiRev
*               ROI reverse status.
* Returns:      void
*************************************************************/
int CTiPrepDlg::getMeanT1(BOOL *f_roi, BOOL *f_roiRev,
                           double * f_pfT1Vals, double * f_pfErrVals,
                           double * f_pfT1RevVals, double * f_pfErrRevVals)
{
    TIPREP_TRACE(CTiPrepDlg::getMeanT1,MRTraceMarker3);
    
    try{
        CString l_temp_str = _T("");
        int l_roiDisp = 0;
        double l_dVal_T1[3];
        double l_dVal_Err[3] ;
        double l_dmeanVal_T1 = 0.0;
        
        for(int l_nroi = 0 ; l_nroi < MAX_NUM_ROI ; l_nroi++) {
            if(!f_roiRev[l_nroi]) {
                l_dVal_T1[l_nroi] = f_pfT1Vals[l_nroi];
                l_dVal_Err[l_nroi] = f_pfErrVals[l_nroi];
            } else {
                l_dVal_T1[l_nroi] = f_pfT1RevVals[l_nroi];
                l_dVal_Err[l_nroi] = f_pfErrRevVals[l_nroi];
            }
            
            if(f_roi[l_nroi]) {
                if(l_dVal_T1[l_nroi] != -1){
                    l_roiDisp++;
                    l_dmeanVal_T1 += l_dVal_T1[l_nroi] ;
                }            
            }
        }
        
        if(0 != l_roiDisp) {
            if (0> l_dmeanVal_T1)//Zero is allowed but Negative not allowed
            {
                m_ROIMeanT1Val.SetWindowText(_T("---.---"));
            }else{
                l_dmeanVal_T1 = l_dmeanVal_T1 / l_roiDisp ;
                //l_dmeanVal_T1 += 0.05;
                l_temp_str.Format(_T("%0.1lf"), (10*l_dmeanVal_T1 + 0.5)*0.1);
                m_ROIMeanT1Val.SetWindowText(l_temp_str);
            }
        } else {
            l_dmeanVal_T1 = -1 ;
            m_ROIMeanT1Val.SetWindowText(_T("---.---"));
        }
        return E_NO_ERROR;
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_TIPREPDLG, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:       getMeanTi
* Description:  This function is used to calculate average value
*               of Ti(Inversion time) of selected ROI's.
* Parameter:    BOOL * f_roi
*               Selected ROI status.
* Parameter:    BOOL * f_roiRev
*               ROI reverse status.
* Parameter:    CString & f_csMean_Ti_Value
* Returns:      void
*************************************************************/
int CTiPrepDlg::getMeanTi(BOOL *f_roi, BOOL *f_roiRev,
                              double * f_pfTiVals, double * f_pfErrVals,
                              double * f_pfTiRevVals, double * f_pfErrRevVals,
                              double * f_pfT1RevVals,double * f_pfT1Vals,
                              CString & f_csMean_Ti)
{
    TIPREP_TRACE(CTiPrepDlg::getMeanTi,MRTraceMarker3);
    
    int l_roiDisp = 0;
    double l_dVal_TI[3];
    double l_dVal_T1[3];
    double l_dVal_Err[3] ;
    double l_dmeanVal_TI = 0.0;
    
    try{
        for(int l_nroi = 0 ; l_nroi < MAX_NUM_ROI ; l_nroi++) {
            if(!f_roiRev[l_nroi]) {
                l_dVal_TI[l_nroi] = f_pfTiVals[l_nroi];
                l_dVal_Err[l_nroi] = f_pfErrVals[l_nroi];
                l_dVal_T1[l_nroi] = f_pfT1Vals[l_nroi];
            } else {
                l_dVal_TI[l_nroi] = f_pfTiRevVals[l_nroi];
                l_dVal_Err[l_nroi] = f_pfErrRevVals[l_nroi];
                l_dVal_T1[l_nroi] = f_pfT1RevVals[l_nroi];
            }

            if(f_roi[l_nroi]) {
//                 if (/*l_dVal_T1[l_nroi] != -1 &&*/ l_dVal_TI[l_nroi] <= DBL_MAX && l_dVal_TI[l_nroi] >= DBL_MIN)
//                 {
//                     l_roiDisp++;
//                     l_dmeanVal_TI += l_dVal_TI[l_nroi] ;
//                 }            

                if(l_dVal_TI[l_nroi] == numeric_limits<double>::infinity( )){
                    //implies infinity
                }else{
					if(-1 != (int)l_dVal_T1[l_nroi]){
						l_roiDisp++;
						l_dmeanVal_TI += l_dVal_TI[l_nroi] ;
					}                    
                }

            }
        }
        
        if(0 != l_roiDisp) {
            l_dmeanVal_TI = l_dmeanVal_TI / l_roiDisp ;
           /* if (0 > l_dmeanVal_TI) //Zero is allowed  Negative also  allowed
            {
                f_csMean_Ti.Format(_T("---.---"));
                m_ROIMeanTiVal.SetWindowText(f_csMean_Ti);
            }else*/
            {
                //l_dmeanVal_TI += 0.05;
                if(l_dmeanVal_TI == numeric_limits<double>::infinity( )){
                    //implies infinity
                    f_csMean_Ti.Format(_T("Inf"));
                    m_ROIMeanTiVal.SetWindowText(f_csMean_Ti);
                }else{
                    f_csMean_Ti.Format(_T("%0.1lf"), /*(10**/l_dmeanVal_TI /*+ 0.5)*0.1*/);
                    m_ROIMeanTiVal.SetWindowText(f_csMean_Ti);
                }
            }
        } else {
            l_dmeanVal_TI = -1 ;
            m_ROIMeanTiVal.SetWindowText(_T("---.---"));
        }
        
        return E_NO_ERROR;
        
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_TIPREPDLG, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:    	getMeanT1MapMode
* Description: 	This function is used 
* Parameter: 	BOOL f_roi :
* Parameter: 	BOOL f_roiRev :
* Parameter: 	double * f_pfT1Vals :
* Parameter: 	double * f_pfErrVals :
* Parameter: 	double * f_pfT1RevVals :
* Parameter: 	double * f_pfErrRevVals :
* Returns:   	void
*************************************************************/
int CTiPrepDlg::getMeanT1MapMode(BOOL f_roi, BOOL f_roiRev, double * f_pfT1Vals, 
                                 double * f_pfErrVals, double * f_pfT1RevVals, 
                                 double * f_pfErrRevVals)
{
    TIPREP_TRACE(CTiPrepDlg::getMeanT1MapMode,MRTraceMarker3);
    CString l_temp_str = _T("");
    int l_roiDisp = 0;
    double l_dVal_T1;
    double l_dVal_Err ;
    double l_dmeanVal_T1 = 0.0;
    long l_nNumOfPoints = 0;
    
    try{
        if(NULL == f_pfT1Vals || NULL == f_pfT1RevVals || NULL == f_pfErrVals || NULL == f_pfErrRevVals) {
            CTiPrepLogHelper::WriteToErrorLog
                (_T("getMeanT1MapMode(): Function parameter found NULL"), __LINE__, __FILE__);
            return E_ERROR;
        }
        
        l_nNumOfPoints = CTiPrepView::GetView()->m_DisplayData->GetTotalNumberOfPointsInAnnularRegion();
        
        for(int l_nroi = 0 ; l_nroi < l_nNumOfPoints ; l_nroi++) {
            if(!f_roiRev) {
                l_dVal_T1 = f_pfT1Vals[l_nroi];
                l_dVal_Err = f_pfErrVals[l_nroi];
            } else {
                l_dVal_T1 = f_pfT1RevVals[l_nroi];
                l_dVal_Err = f_pfErrRevVals[l_nroi];
            }
            
            if(f_roi) {
                if(l_dVal_T1 != -1){
                    l_roiDisp++;
                    l_dmeanVal_T1 += l_dVal_T1;
                }          
            }
        }
        
        if(0 != l_roiDisp) {
            if (0 > l_dmeanVal_T1)
            {
                m_ROIMeanT1Val.SetWindowText(_T("---.---"));
            }else{
                l_dmeanVal_T1 = l_dmeanVal_T1 / l_roiDisp ;
                l_temp_str.Format(_T("%0.1lf"), (10*l_dmeanVal_T1 + 0.5)*0.1 );
                m_ROIMeanT1Val.SetWindowText(l_temp_str);
            }
        } else {
            l_dmeanVal_T1 = -1 ;
            m_ROIMeanT1Val.SetWindowText(_T("---.---")); 
        }
        
        return E_NO_ERROR;
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_TIPREPDLG, __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:       displayData
* Description:  This function is used to display calculated
*               values of Ti, T1 and Fitting error of selected
*               ROI's on GUI.
* Returns:      void
*************************************************************/
void CTiPrepDlg::displayData()
{
    TIPREP_TRACE(CTiPrepDlg::displayData,MRTraceMarker3);
    
    try {
        CString l_temp_str, l_temp_errStr, l_temp_t1val;
        BOOL l_roi[MAX_NUM_ROI];
        BOOL l_roiRev[3];
        BOOL l_graphOpt[3];
        //Get check box status
        l_roi[ROI_1] = m_checkDispROI1Data.GetCheck();
        l_roi[ROI_2] = m_checkDispROI2Data.GetCheck();
        l_roi[ROI_3] = m_checkDispROI3Data.GetCheck();
        l_roiRev[0] = m_checkRevDispROI1Data.GetCheck();
        l_roiRev[1] = m_checkRevDispROI2Data.GetCheck();
        l_roiRev[2] = m_checkRevDispROI3Data.GetCheck();
        l_graphOpt[0] = m_checkOrgData.GetCheck();
        l_graphOpt[1] = m_checkInvData.GetCheck();
        l_graphOpt[2] = m_checkFitData.GetCheck();
        double * l_tivalues = NULL;
        double * l_Errvalues = NULL;
        double * l_t1Values = NULL;
        double * l_tiRevValues = NULL;
        double * l_errRevValuels = NULL;
        double * l_t1RevValues = NULL;
        CTiPrepView::GetView()->GetDisplayVals(&l_tivalues, &l_Errvalues,
            &l_t1Values, &l_tiRevValues,
            &l_errRevValuels, &l_t1RevValues);

        CString l_csMean_Ti = _T("");
        getMeanTi(l_roi, l_roiRev,l_tivalues, l_Errvalues,
            l_tiRevValues, l_errRevValuels,l_t1RevValues,l_t1Values,
            l_csMean_Ti);

        getMeanT1(l_roi, l_roiRev,
            l_t1Values, l_Errvalues,
            l_t1RevValues, l_errRevValuels);

        m_tigraphctrl.SetMeanTiValue(l_csMean_Ti);
        m_tigraphctrl.ResetGraphWindow(TRUE, l_roi, l_roiRev, l_graphOpt);
        
        for(int l_nroi = 0; l_nroi < MAX_NUM_ROI; l_nroi++) {
            if(l_roi[l_nroi]) {
                if(l_roiRev[l_nroi]) {
                    if(-1 == l_t1RevValues[l_nroi]){
                        l_temp_str.Format(_T("Err"));
                        l_temp_errStr.Format(_T("Err"));
                        l_temp_t1val.Format(_T("Err"));
                    }else{
                        //l_tiRevValues[l_nroi] = l_tiRevValues[l_nroi] + 0.05;
//                         if((l_tiRevValues[l_nroi] > DBL_MAX) || (l_tiRevValues[l_nroi] < DBL_MIN)){
//                             l_temp_str.Format(_T("Inf"));
//                             
//                         }else{
//                             l_temp_str.Format(_T("%0.1lf"), /*(10**/l_tiRevValues[l_nroi] /*+ 0.5)*0.1*/ );                            
//                         }                        
//                         if((l_tiRevValues[l_nroi] <= DBL_MAX) && (l_tiRevValues[l_nroi] >= DBL_MIN)){
//                             l_temp_str.Format(_T("%0.1lf"), /*(10**/l_tiRevValues[l_nroi] /*+ 0.5)*0.1*/ );                            
//                         }else{
//                             l_temp_str.Format(_T("Inf"));                            
//                         }

                        if(l_tiRevValues[l_nroi] == numeric_limits<double>::infinity( )){
                            //implies infinity
                            l_temp_str.Format(_T("Inf"));
                        }else{
                            l_temp_str.Format(_T("%0.1lf"), /*(10**/l_tiRevValues[l_nroi] /*+ 0.5)*0.1*/ );
                        }
                        //l_errRevValuels[l_nroi] = l_errRevValuels[l_nroi] + 0.05;
                        l_temp_errStr.Format(_T("%0.1lf"),(10*l_errRevValuels[l_nroi] + 0.5)*0.1 );
                        //l_t1RevValues[l_nroi] = l_t1RevValues[l_nroi] + 0.05;
                        l_temp_t1val.Format(_T("%0.1lf"), (10*l_t1RevValues[l_nroi] + 0.5)*0.1 );
                    }
                } else {
                    if(-1 == l_t1Values[l_nroi]){
                        l_temp_str.Format(_T("Err"));
                        l_temp_errStr.Format(_T("Err"));
                        l_temp_t1val.Format(_T("Err"));
                    }else{
                        //l_tivalues[l_nroi] = l_tivalues[l_nroi] + 0.05;
//                         if((l_tivalues[l_nroi] > DBL_MAX) || (l_tivalues[l_nroi] < DBL_MIN)){
//                             l_temp_str.Format(_T("Inf"));
// 
//                         }else{
//                             l_temp_str.Format(_T("%0.1lf"), /*(10**/l_tivalues[l_nroi] /*+ 0.5)*0.1*/ );
//                         }
//                         if((l_tivalues[l_nroi] <= DBL_MAX) && (l_tivalues[l_nroi] >= DBL_MIN)){
//                             l_temp_str.Format(_T("%0.1lf"), /*(10**/l_tivalues[l_nroi] /*+ 0.5)*0.1*/ );
//                             
//                         }else{
//                             l_temp_str.Format(_T("Inf"));
//                             
//                         }

                        if(l_tivalues[l_nroi] == numeric_limits<double>::infinity( )){
                            //implies infinity
                            l_temp_str.Format(_T("Inf"));
                        }else{
                            l_temp_str.Format(_T("%0.1lf"), /*(10**/l_tivalues[l_nroi] /*+ 0.5)*0.1*/ );
                        }
                        //l_Errvalues[l_nroi] = l_Errvalues[l_nroi] + 0.05;
                        l_temp_errStr.Format(_T("%0.1lf"), (10*l_Errvalues[l_nroi] + 0.5)*0.1);
                        //l_t1Values[l_nroi] = l_t1Values[l_nroi] + 0.05;
                        l_temp_t1val.Format(_T("%0.1lf"),(10*l_t1Values[l_nroi] + 0.5)*0.1 );
                    }
                }
                
                switch(l_nroi) {
                case 0:
                    m_ROI1TiVal.SetWindowText(l_temp_str);
                    m_ROI1FitErr.SetWindowText(l_temp_errStr);
                    m_ROI1T1Val.SetWindowText(l_temp_t1val);
                    break;
                case 1:
                    m_ROI2TiVal.SetWindowText(l_temp_str);
                    m_ROI2FitErr.SetWindowText(l_temp_errStr);
                    m_ROI2T1Val.SetWindowText(l_temp_t1val);
                    break;
                case 2:
                    m_ROI3TiVal.SetWindowText(l_temp_str);
                    m_ROI3FitErr.SetWindowText(l_temp_errStr);
                    m_ROI3T1Val.SetWindowText(l_temp_t1val);
                    break;
                default:
                    ;
                }
            } else {
                l_temp_str.Format(_T("---.---"));
                
                switch(l_nroi) {
                case 0:
                    m_ROI1TiVal.SetWindowText(l_temp_str);
                    m_ROI1FitErr.SetWindowText(l_temp_str);
                    m_ROI1T1Val.SetWindowText(l_temp_str);
                    break;
                case 1:
                    m_ROI2TiVal.SetWindowText(l_temp_str);
                    m_ROI2FitErr.SetWindowText(l_temp_str);
                    m_ROI2T1Val.SetWindowText(l_temp_str);
                    break;
                case 2:
                    m_ROI3TiVal.SetWindowText(l_temp_str);
                    m_ROI3FitErr.SetWindowText(l_temp_str);
                    m_ROI3T1Val.SetWindowText(l_temp_str);
                    break;
                default:
                    ;
                }
            }
        }
    } catch(CMemoryException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted;
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Memory error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
    } catch(CException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted;
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Exception occurred with this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(_T("Exception Occurred during DisplayData"), __LINE__, __FILE__);
    }
}

/*************************************************************
* Method:       displayNoiseData
* Description:  This function is used
* Returns:      void
*************************************************************/
void CTiPrepDlg::displayNoiseData()
{
    TIPREP_TRACE(CTiPrepDlg::displayNoiseData,MRTraceMarker3);
    
    try {
        BOOL l_roi[MAX_NOISE_ROI];
        l_roi[0] = m_checkNoiseROI1Data.GetCheck();
        l_roi[1] = m_checkNoiseROI2Data.GetCheck();
        BOOL l_boffsetCorrection;
        CString l_csNoise_Correction_Val(_T(""));
        l_boffsetCorrection = m_checkOffsetCorrection.GetCheck();
        
        if(TRUE == l_boffsetCorrection) {
            double l_dnoiseOffset = 0.0;
            CTiPrepView::GetView()->GetNoiseOffsetVal(l_dnoiseOffset);
            l_csNoise_Correction_Val.Format(_T("%0.1lf"), l_dnoiseOffset);
            m_NoiseCorrectionVal.SetWindowText(l_csNoise_Correction_Val);
        } else {
            l_csNoise_Correction_Val.Format(_T("---.---"));
            m_NoiseCorrectionVal.SetWindowText(l_csNoise_Correction_Val);
        }
        
        m_tigraphctrl.ResetGraphWindowNoiseRoi(TRUE, l_roi);
    } catch(CException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted;
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Exception occurred with this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog
            (_T("Exception Occurred during displayNoiseData"), __LINE__, __FILE__);
    }
}

/*************************************************************
* Method:       OnButtonReselect
* Description:  This function is message handler.
*               Called when 'Reselect' button is clicked.
* Returns:      void
*************************************************************/
void CTiPrepDlg::OnButtonReselect()
{
    TIPREP_TRACE(CTiPrepDlg::OnButtonReselect,MRTraceMarker9);
    (CTiPrepView::GetView())->OnReselect();
    BOOL l_roi[3];
    BOOL l_noiseRoi[2];
    BOOL l_roiRev[3];
    BOOL l_graphOpt[3];
    l_roi[0] = m_checkDispROI1Data.GetCheck();
    l_roi[1] = m_checkDispROI2Data.GetCheck();
    l_roi[2] = m_checkDispROI3Data.GetCheck();
    l_noiseRoi[0] = m_checkNoiseROI1Data.GetCheck(); 
    l_noiseRoi[1] = m_checkNoiseROI2Data.GetCheck(); 
    l_roiRev[0] = m_checkRevDispROI1Data.GetCheck();
    l_roiRev[1] = m_checkRevDispROI2Data.GetCheck();
    l_roiRev[2] = m_checkRevDispROI3Data.GetCheck();
    l_graphOpt[0] = m_checkOrgData.GetCheck();
    l_graphOpt[1] = m_checkInvData.GetCheck();
    l_graphOpt[2] = m_checkFitData.GetCheck();
    CString l_temp_str = "";
    m_tigraphctrl.SetMeanTiValue(l_temp_str);
    m_tigraphctrl.ResetGraphWindow(FALSE, l_roi, l_roiRev, l_graphOpt);
    m_tigraphctrl.ResetGraphWindowNoiseRoi(FALSE, l_noiseRoi);  
    m_ROI1TiVal.SetWindowText(l_temp_str);
    m_ROI2TiVal.SetWindowText(l_temp_str);
    m_ROI3TiVal.SetWindowText(l_temp_str);
    m_ROI1FitErr.SetWindowText(l_temp_str);
    m_ROI2FitErr.SetWindowText(l_temp_str);
    m_ROI3FitErr.SetWindowText(l_temp_str);
    m_ROIMeanTiVal.SetWindowText(l_temp_str);
    m_ROI1T1Val.SetWindowText(l_temp_str);
    m_ROI2T1Val.SetWindowText(l_temp_str);
    m_ROI3T1Val.SetWindowText(l_temp_str);
    m_ROIMeanT1Val.SetWindowText(l_temp_str);
   // m_Recon.SetWindowText(l_temp_str);
    m_static_series_name.SetWindowText(l_temp_str);
    m_NoiseCorrectionVal.SetWindowText(l_temp_str); 
    m_exDataType.SetWindowText(l_temp_str);
    m_lbl_pix_pos.SetWindowText(l_temp_str);
}

/*************************************************************
* Method:       OnCheckOrgdata
* Description:  This function is message handler.
*               Called when 'Org Data' check box is checked/unchecked.
* Returns:      void
*************************************************************/
void CTiPrepDlg::OnCheckOrgdata()
{
    TIPREP_TRACE(CTiPrepDlg::OnCheckOrgdata,MRTraceMarker9);
    BOOL l_roi[3];
    BOOL l_roiRev[3];
    BOOL l_graphOpt[3];
    l_roi[0] = m_checkDispROI1Data.GetCheck();
    l_roi[1] = m_checkDispROI2Data.GetCheck();
    l_roi[2] = m_checkDispROI3Data.GetCheck();
    l_roiRev[0] = m_checkRevDispROI1Data.GetCheck();
    l_roiRev[1] = m_checkRevDispROI2Data.GetCheck();
    l_roiRev[2] = m_checkRevDispROI3Data.GetCheck();
    l_graphOpt[0] = m_checkOrgData.GetCheck();
    l_graphOpt[1] = m_checkInvData.GetCheck();
    l_graphOpt[2] = m_checkFitData.GetCheck();
    
    if(m_flag) {
        m_tigraphctrl.ResetGraphWindow(TRUE, l_roi, l_roiRev, l_graphOpt);
        CTiPrepView::GetView()->RefreshGraphAndDataOnGui();
    } else {
        //
    }
}

/*************************************************************
* Method:       OnCheckInvdata
* Description:  This function is message handler.
*               Called when 'Inv Data' check box is checked/unchecked.
* Returns:      void
*************************************************************/
void CTiPrepDlg::OnCheckInvdata()
{
    TIPREP_TRACE(CTiPrepDlg::OnCheckInvdata,MRTraceMarker9);
    BOOL l_roi[3];
    BOOL l_roiRev[3];
    BOOL l_graphOpt[3];
    l_roi[0] = m_checkDispROI1Data.GetCheck();
    l_roi[1] = m_checkDispROI2Data.GetCheck();
    l_roi[2] = m_checkDispROI3Data.GetCheck();
    l_roiRev[0] = m_checkRevDispROI1Data.GetCheck();
    l_roiRev[1] = m_checkRevDispROI2Data.GetCheck();
    l_roiRev[2] = m_checkRevDispROI3Data.GetCheck();
    l_graphOpt[0] = m_checkOrgData.GetCheck();
    l_graphOpt[1] = m_checkInvData.GetCheck();
    l_graphOpt[2] = m_checkFitData.GetCheck();
    
    if(m_flag) {
        m_tigraphctrl.ResetGraphWindow(TRUE, l_roi, l_roiRev, l_graphOpt);
        CTiPrepView::GetView()->RefreshGraphAndDataOnGui();
    } else {
        //
    }
}

/*************************************************************
* Method:       OnCheckFitdata
* Description:  This function is message handler.
*               Called when 'Fit Data' check box is checked/unchecked.
* Returns:      void
*************************************************************/
void CTiPrepDlg::OnCheckFitdata()
{
    TIPREP_TRACE(CTiPrepDlg::OnCheckFitdata,MRTraceMarker9);
    BOOL l_roi[3];
    BOOL l_roiRev[3];
    BOOL l_graphOpt[3];
    l_roi[0] = m_checkDispROI1Data.GetCheck();
    l_roi[1] = m_checkDispROI2Data.GetCheck();
    l_roi[2] = m_checkDispROI3Data.GetCheck();
    l_roiRev[0] = m_checkRevDispROI1Data.GetCheck();
    l_roiRev[1] = m_checkRevDispROI2Data.GetCheck();
    l_roiRev[2] = m_checkRevDispROI3Data.GetCheck();
    l_graphOpt[0] = m_checkOrgData.GetCheck();
    l_graphOpt[1] = m_checkInvData.GetCheck();
    l_graphOpt[2] = m_checkFitData.GetCheck();
    
    if(m_flag) {
        m_tigraphctrl.ResetGraphWindow(TRUE, l_roi, l_roiRev, l_graphOpt);
        CTiPrepView::GetView()->RefreshGraphAndDataOnGui();
    } else {
        //
    }
}

/*************************************************************
* Method:       OnCheckRoi1Dislpay
* Description:  This function is message handler.
*               Called when 'ROI1' check box is checked/unchecked.
* Returns:      void
*************************************************************/
void CTiPrepDlg::OnCheckRoi1Dislpay()
{
    TIPREP_TRACE(CTiPrepDlg::OnCheckRoi1Dislpay,MRTraceMarker9);
    
    if(m_flag) {
        //    displayData();
        CTiPrepView::GetView()->RefreshGraphAndDataOnGui();
    }
    
    int l_nchecked = m_checkDispROI1Data.GetCheck();
    
    if(BST_CHECKED == m_checkT1Map.GetCheck()) {
        ShowHideROI(ROI_1, l_nchecked);
        ShowHideROI(ROI_2, l_nchecked); //l_nchecked will be same for roi2
        //weather checked or unchecked both should be shown or hidden together
    } else {
        ShowHideROI(ROI_1, l_nchecked);
    }
}

/*************************************************************
* Method:       OnCheckRoi2Display
* Description:  This function is message handler.
*               Called when 'ROI2' check box is checked/unchecked.
* Returns:      void
*************************************************************/
void CTiPrepDlg::OnCheckRoi2Display()
{
    TIPREP_TRACE(CTiPrepDlg::OnCheckRoi2Display,MRTraceMarker9);
    
    if(m_flag) {
        CTiPrepView::GetView()->RefreshGraphAndDataOnGui();
    }
    
    int l_nchecked = m_checkDispROI2Data.GetCheck();
    ShowHideROI(ROI_2, l_nchecked);
}

/*************************************************************
* Method:       OnCheckRoi3Display
* Description:  This function is message handler.
*               Called when 'ROI3' check box is checked/unchecked.
* Returns:      void
*************************************************************/
void CTiPrepDlg::OnCheckRoi3Display()
{
    TIPREP_TRACE(CTiPrepDlg::OnCheckRoi3Display,MRTraceMarker9);
    
    if(m_flag) {
        //    displayData();
        CTiPrepView::GetView()->RefreshGraphAndDataOnGui();
    }
    
    int l_nchecked = m_checkDispROI3Data.GetCheck();
    ShowHideROI(ROI_3, l_nchecked);
}

/*************************************************************
* Method:       OnCheckNoiseRoi1Display
* Description:  This function is message handler.
*               Called when 'Noise ROI 1' check box is checked/unchecked.
* Returns:      void
*************************************************************/
void CTiPrepDlg::OnCheckNoiseRoi1Display()
{
    TIPREP_TRACE(CTiPrepDlg::OnCheckNoiseRoi1Display,MRTraceMarker9);
    
    if(m_flag) {
        if(m_checkOffsetCorrection.GetCheck() == BST_CHECKED) {
            ClearGraphCtl();
            int status=  CTiPrepView::GetView()->OnApply(m_nEqnSelectFlag);
            if (status != E_NO_ERROR)DisplayErr(status);
            
        } else {
            CTiPrepView::GetView()->RefreshGraphAndDataOnGui();
        }
    }
    
    int l_nchecked = m_checkNoiseROI1Data.GetCheck();
    ShowHideROI(NOISE_ROI_1, l_nchecked);
}

/*************************************************************
* Method:       OnCheckNoiseRoi2Display
* Description:  This function is message handler.
*               Called when 'Noise ROI 2' check box is checked/unchecked.
* Returns:      void
*************************************************************/
void CTiPrepDlg::OnCheckNoiseRoi2Display()
{
    TIPREP_TRACE(CTiPrepDlg::OnCheckNoiseRoi2Display,MRTraceMarker9);
    
    if(m_flag) {
        if(m_checkOffsetCorrection.GetCheck() == BST_CHECKED) {
            ClearGraphCtl(); 
            int status = CTiPrepView::GetView()->OnApply(m_nEqnSelectFlag);
            if (status != E_NO_ERROR)DisplayErr(status);
            
        } else {
            CTiPrepView::GetView()->RefreshGraphAndDataOnGui();
        }
    }
    
    int l_nchecked = m_checkNoiseROI2Data.GetCheck();
    ShowHideROI(NOISE_ROI_2, l_nchecked);
}

BOOL CTiPrepDlg::ShowHideROI(int f_nROINo, int f_nChecked)
{
    TIPREP_TRACE(CTiPrepDlg::ShowHideROI,MRTraceMarker3);
    BOOL l_bHideflag = FALSE;
    l_bHideflag = (CTiPrepView::GetView())->HideROI(f_nROINo, f_nChecked);
    
    if(FALSE == l_bHideflag) {
        CString l_csErrMsg(_T("Unable to hide/show ROI."));
        CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
        return l_bHideflag;
    }
    
    UpdateWindow();
    return l_bHideflag;
}

/*************************************************************
* Method:       OnCheckOffsetCorrection
* Description:  This function is message handler.
*               Called when 'Offset Correction' check box is
*               checked/unchecked.
* Returns:      void
*************************************************************/
void CTiPrepDlg::OnCheckOffsetCorrection()
{
    TIPREP_TRACE(CTiPrepDlg::OnCheckOffsetCorrection,MRTraceMarker9);
    
    if(!m_flag) {
        CString l_csErrMsg(_T("Apply button not clicked yet"));
        CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
        return;
    }
    
    if(BST_UNCHECKED == m_checkNoiseROI1Data.GetCheck() && BST_UNCHECKED == m_checkNoiseROI2Data.GetCheck()) {
        CString l_csErrMsg(_T("No noise ROI selected"));
        CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
        m_NoiseCorrectionVal.SetWindowText(_T("---.---"));
        return;
    }
    
    
    ClearGraphCtl(); 
    int status = CTiPrepView::GetView()->OnApply(m_nEqnSelectFlag);
    if (status != E_NO_ERROR)DisplayErr(status); 
    
    UpdateWindow();
}

/*************************************************************
* Method:       OnCheckRoi1Reversemin
* Description:  This function is message handler.
*               Called when 'Reverse Min for ROI1' check box is
*               checked/unchecked.
* Returns:      void
*************************************************************/
void CTiPrepDlg::OnCheckRoi1Reversemin()
{
    TIPREP_TRACE(CTiPrepDlg::OnCheckRoi1Reversemin,MRTraceMarker9);
    
    if(m_flag) {
        if(CTiPrepView::GetView()->GetT1MapFlag()) {
            if(m_checkRevDispROI1Data.GetCheck()) {
                CTiPrepView::GetView()->SendMsg("MINUPDATE");
            } else {
                CTiPrepView::GetView()->SendMsg("UPDATE");
            }
            CTiPrepView::GetView()->InvokeT1Map();//AS/added
        }
        
        CTiPrepView::GetView()->RefreshGraphAndDataOnGui();
    } else {
    }
}

/*************************************************************
* Method:       OnCheckRoi2Reversemin
* Description:  This function is message handler.
*               Called when 'Reverse Min for ROI2' check box
*               is checked/unchecked.
* Returns:      void
*************************************************************/
void CTiPrepDlg::OnCheckRoi2Reversemin()
{
    TIPREP_TRACE(CTiPrepDlg::OnCheckRoi2Reversemin,MRTraceMarker9);
    
    if(m_flag) {
        CTiPrepView::GetView()->RefreshGraphAndDataOnGui();
    } else {
    }
}

/*************************************************************
* Method:       OnCheckRoi3Reversemin
* Description:  This function is message handler.
*               Called when 'Reverse Min for ROI3' check box is
*               checked/unchecked.
* Returns:      void
*************************************************************/
void CTiPrepDlg::OnCheckRoi3Reversemin()
{
    TIPREP_TRACE(CTiPrepDlg::OnCheckRoi3Reversemin,MRTraceMarker9);
    
    if(m_flag) {
        CTiPrepView::GetView()->RefreshGraphAndDataOnGui();
    } else {
        //
    }
}

/*************************************************************
* Method:       OnRadio1stequ
* Description:  This function is message handler.
*               Called when 'Eqn 1' radio button is checked/unchecked.
* Returns:      void
*************************************************************/
void CTiPrepDlg::OnRadio1stequ()
{
    TIPREP_TRACE(CTiPrepDlg::OnRadio1stequ,MRTraceMarker9);
    m_nEqnSelectFlag = 0;
    int status =(CTiPrepView::GetView())->OnEquationOne(m_nEqnSelectFlag);
    if (status != E_NO_ERROR)
        DisplayErr(status);
}

/*************************************************************
* Method:       OnRadio2ndequ
* Description:  This function is message handler.
*               Called when 'Eqn 2' radio button is checked/unchecked.
* Returns:      void
*************************************************************/
void CTiPrepDlg::OnRadio2ndequ()
{
    TIPREP_TRACE(CTiPrepDlg::OnRadio2ndequ,MRTraceMarker9);
    m_nEqnSelectFlag = 1;
    int status = (CTiPrepView::GetView())->OnEquationTwo(m_nEqnSelectFlag);
    if (status != E_NO_ERROR)
        DisplayErr(status);
}

/*************************************************************
* Method:       OnButtonResetroi
* Description:  This function is message handler.
* Called when 'Reset ROI' button is clicked.
* Returns:      void
*************************************************************/
void CTiPrepDlg::OnButtonResetroi()
{
    TIPREP_TRACE(CTiPrepDlg::OnButtonResetroi,MRTraceMarker9);
    BOOL l_falg =TRUE;
    unsigned short l_ht = 0;
    unsigned short l_wt = 0;
    if(E_NO_ERROR != CTiPrepView::GetView()->GetHeightWidthImageFromDB(l_ht,l_wt)){
        CString l_csErrMsg(_T("GetHeightWidthImageFromDB failed."));
        CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
	//TRACE COMMENTED
	}
#ifdef _UT_LOG_
	else{
        CString l_csLogMsg(_T(""));
        l_csLogMsg.Format(_T("Image height=%d width=%d"), l_ht, l_wt);
        CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker4, l_csLogMsg);
    }
#endif
	CString mode = _T("");
    if(true == ((CTiPrepView::GetView())->GetT1MapFlag()) ){
		mode = _T("MAP");
        // l_falg = (CTiPrepView::GetView())->GetROIInstance()->ResetROIPosition(_T("MAP"),l_ht,l_wt);
    }else{
		mode= _T("AVG");
        // l_falg = (CTiPrepView::GetView())->GetROIInstance()->ResetROIPosition(_T("AVG"),l_ht,l_wt);
    }
	CString l_csProtocolName(_T(""));
    l_falg = (CTiPrepView::GetView())->GetROIInstance()->RedrawROI(true,l_csProtocolName,mode);
    if(!l_falg) {
        CString l_csErrMsg(_T("ROI position not reseted."));
        CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
        return;
    }
}



/*************************************************************
* Method:       OnCheckEditROI
* Description:  This function is used
* Returns:      void
*************************************************************/
void CTiPrepDlg::OnCheckEditROI()
{
    TIPREP_TRACE(CTiPrepDlg::OnCheckEditROI,MRTraceMarker9);
    BOOL l_bCheckFlag = m_checkEditROIData.GetCheck();
    BOOL l_falg = (CTiPrepView::GetView())->GetROIInstance()->MouseMoveROI(l_bCheckFlag);
    
    if(!l_falg) {
        CString l_csErrMsg(_T("Unable to change state of ROI editable to noneditable / noneditable to editable."));
        CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
        return;
    }
}

/*************************************************************
* Method:       GetSeriesNumber
* Description:  This function is used
* Parameter:    CString & f_csSeriesNumber
* Parameter:    CString & f_csProtocolName
* Returns:      void
*************************************************************/
void CTiPrepDlg::GetSeriesNumber(CString &f_csSeriesNumber, CString &f_csProtocolName)
{
    TIPREP_TRACE(CTiPrepDlg::GetSeriesNumber,MRTraceMarker3);
    
    try{
        CString l_csProcessNode = _T(""), l_csProtNode = _T(""), l_csTempStr = _T("");
        char l_cpC[32];
        int cnt = f_csProtocolName.ReverseFind(_T('.'));
        l_csProcessNode = f_csProtocolName.Right((f_csProtocolName.GetLength() - (cnt + 1)));
        cnt = f_csProtocolName.ReverseFind(_T('/'));
        l_csTempStr = f_csProtocolName.Left(cnt);
        cnt = l_csTempStr.ReverseFind(_T('.'));
        l_csProtNode = l_csTempStr.Right((l_csTempStr.GetLength() - (cnt + 1)));
        wcstombs(l_cpC, l_csProtNode, 32);
        int l_nTempObj = atoi(l_cpC);
        int l_nProtIndex = 1000 * (l_nTempObj + 1);
        wcstombs(l_cpC, l_csProcessNode, 32);
        l_nTempObj = atoi(l_cpC);
        int l_nProcessIndex = l_nProtIndex + l_nTempObj;
        f_csSeriesNumber.Format(_T("%d"), l_nProcessIndex);
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_TIPREPDLG, __LINE__, __FILE__);
    }
}

/*************************************************************
* Method:       ValidateOffset
* Description:  This function validates TR value entered
* Parameter:    CString f_csOffsetNo
* Returns:      BOOL
*************************************************************/
BOOL CTiPrepDlg::ValidateOffset(CString f_csOffsetNo)
{
    BOOL l_result = FALSE;
    char val[100];
    int l_nDotCnt = 0;
    
    if(f_csOffsetNo.IsEmpty()) {
        return FALSE;
    }
    
    try{
        wcstombs(val, f_csOffsetNo, 100);
        int l_nstrlength = f_csOffsetNo.GetLength();
        
        for(int l_ni = 0; l_ni < l_nstrlength; l_ni++) { 
            if('.' == val[l_ni]) { 
                l_nDotCnt++;
                l_ni++;
            }
            
            if(FALSE == isdigit(val[l_ni])) {
                return FALSE;
            }
        }
        
        if(1 < l_nDotCnt) {
            return FALSE;
        }
        
        double rad = atof(val);
        
        if(rad <= 0) {
            return FALSE;
        }
        
        l_result = TRUE;
        return l_result;
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_TIPREPDLG, __LINE__, __FILE__);
        return FALSE;
    }
}

/*************************************************************
* Method:       GetROIDisplayCheck
* Description:  This function is used to check if ROI1, 2, 3 CheckBoxes are selected or not
* Parameter:    int f_nRoiNo : 0,1 or 2 for three ROIs
* Returns:      BOOL
*************************************************************/
BOOL CTiPrepDlg::GetROIDisplayCheck(int f_nRoiNo)
{
    BOOL l_ischeck = FALSE;
    
    switch(f_nRoiNo) {
    case 0:
        l_ischeck = m_checkDispROI1Data.GetCheck();
        break;
    case 1:
        if (CTiPrepView::GetView()->GetT1MapFlag())
        {
            l_ischeck = m_checkDispROI1Data.GetCheck();
        } else {
            l_ischeck = m_checkDispROI2Data.GetCheck();
        }
        break;
    case 2:
        l_ischeck = m_checkDispROI3Data.GetCheck();
        break;
    case 3:
        l_ischeck = m_checkNoiseROI1Data.GetCheck();
        break;
    case 4:
        l_ischeck = m_checkNoiseROI2Data.GetCheck();
        break;
    default:
        break;
    }
    
    return l_ischeck;
}

/*************************************************************
* Method:       OnRecvReselectDone
* Description:  This function is called when image LOID is retrieved from Image Matrix on click of Reselect
* Returns:      void
*************************************************************/
int CTiPrepDlg::OnRecvReselectDone()
{
    TIPREP_TRACE(CTiPrepDlg::OnRecvReselectDone,MRTraceMarker3);
    //BOOL l_resultROI = FALSE;
    CString l_csSeriesName = _T("");
    int l_t1_map_check_box_status = 0; 
    try {
        m_is_reselect_wait = false;
        int l_nCheckStatus = m_checkT1Map.GetCheck();
        
        if(l_nCheckStatus == BST_CHECKED) {
            l_t1_map_check_box_status = 1;
            m_tigraphctrl.GetT1MapCheckBoxStatus(l_t1_map_check_box_status);
        } else if(l_nCheckStatus == BST_UNCHECKED) {
            l_t1_map_check_box_status = 0;
            m_tigraphctrl.GetT1MapCheckBoxStatus(l_t1_map_check_box_status);
        }
        
      int status =   (CTiPrepView::GetView())->GetSelSeriesName(l_nCheckStatus/*,l_csSeriesName*/);
      return status;
    } catch(CMemoryException &e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Memory can not be allocated because of this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        (CTiPrepView::GetView())->GetROIInstance()->DeleteROI();
        return E_ERR_EXCEPTION;
    } catch(CFileException &e) {
        TCHAR   szCause[255] = L"";
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Memory can not be allocated because of this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        (CTiPrepView::GetView())->GetROIInstance()->DeleteROI();
        return E_ERR_EXCEPTION;
    } catch(CException &e) {
        TCHAR   szCause[255] = L"";
        CString l_csStrFormatted = _T("");
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Memory can not be allocated because of this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        (CTiPrepView::GetView())->GetROIInstance()->DeleteROI();
        return E_ERR_EXCEPTION;
    } catch(...) {
        CString l_csStrFormatted = _T("");
        l_csStrFormatted = _T("Exception occured on Reselct ");
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
        (CTiPrepView::GetView())->GetROIInstance()->DeleteROI();
        return E_ERR_EXCEPTION;
    }
}

/*************************************************************
* Method:       OnSetCursor handles WM_SETCURSOR message for setting Wait cursor when necessary
* Description:  This function is used
* Parameter:    CWnd * pWnd :
* Parameter:    UINT nHitTest :
* Parameter:    UINT message :
* Returns:      BOOL
*************************************************************/
BOOL CTiPrepDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    //TIPREP_TRACE(CTiPrepDlg::OnSetCursor);
    
    if((CTiPrepView::GetView())->IsWaitCursor()) {
        ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
        return TRUE;
    } else {
        // ReleaseCapture();
    }
    
    return CPopupDlg::OnSetCursor(pWnd, nHitTest, message);
}
/*************************************************************
* Method:       SetDlgPos
* Description:  This function is used
* Returns:      void
*************************************************************/
void CTiPrepDlg::SetDlgPos()
{
    RECT desktop_rect;
    CRect tidlg_rect;
    GetDesktopWindow()->GetWindowRect(&desktop_rect);
    GetWindowRect(&tidlg_rect);
    long x_pos = (desktop_rect.right - tidlg_rect.Width() - 10);
    long y_pos = desktop_rect.top + 15;
    
    MoveWindow(x_pos, y_pos, tidlg_rect.Width(), tidlg_rect.Height());
}


/*************************************************************
* Method:       DisplaySeriesName displays current Series name
*               below graph window
* Description:  This function is used
* Returns:      void
*************************************************************/
void CTiPrepDlg::DisplaySeriesName(CString &f_csProtocolName, int f_nSelDlg)
{
    //vector<CString> l_csImgInfo;
    CString l_csProtocolName = _T("");
    CString l_csTempStr(_T(""));
    
    if(POLARITY_CORR_DLG == f_nSelDlg) {
       if (m_polarity_correction_dlg) m_polarity_correction_dlg->RefreshSelectedSeries(f_csProtocolName); 
    } else if(ADD_OFFSET_DLG == f_nSelDlg) {
     if (m_AddOffsetDlg)   m_AddOffsetDlg->RefreshSelectedSeries(f_csProtocolName);
    } else {
        CString l_csdisplay(_T(""));
        l_csdisplay.Format(_T("Current Series : %s"), f_csProtocolName);
        m_static_series_name.SetWindowText(l_csdisplay);
    }
}

/*************************************************************
* Method:       ClearGraphCtl
* Description:  This function is used
* Returns:      void
*************************************************************/
void CTiPrepDlg::ClearGraphCtl()
{
    TIPREP_TRACE(CTiPrepDlg::ClearGraphCtl,MRTraceMarker9);
    BOOL l_roi[MAX_NUM_ROI];
    BOOL l_noiseRoi[MAX_NOISE_ROI];
    BOOL l_roiRev[3];
    BOOL l_graphOpt[3];
    l_roi[0] = m_checkDispROI1Data.GetCheck();
    l_roi[1] = m_checkDispROI2Data.GetCheck();
    l_roi[2] = m_checkDispROI3Data.GetCheck();
    l_noiseRoi[0] = m_checkNoiseROI1Data.GetCheck(); 
    l_noiseRoi[1] = m_checkNoiseROI2Data.GetCheck(); 
    
    l_roiRev[0] = m_checkRevDispROI1Data.GetCheck();
    l_roiRev[1] = m_checkRevDispROI2Data.GetCheck();
    l_roiRev[2] = m_checkRevDispROI3Data.GetCheck();
    l_graphOpt[0] = m_checkOrgData.GetCheck();
    l_graphOpt[1] = m_checkInvData.GetCheck();
    l_graphOpt[2] = m_checkFitData.GetCheck();
    CString l_temp_str = "";
    m_tigraphctrl.SetMeanTiValue(l_temp_str);
    m_tigraphctrl.ResetGraphWindow(FALSE, l_roi, l_roiRev, l_graphOpt);
    m_tigraphctrl.ResetGraphWindowNoiseRoi(FALSE, l_noiseRoi);
    m_ROI1TiVal.SetWindowText(l_temp_str);
    m_ROI2TiVal.SetWindowText(l_temp_str);
    m_ROI3TiVal.SetWindowText(l_temp_str);
    m_ROI1FitErr.SetWindowText(l_temp_str);
    m_ROI2FitErr.SetWindowText(l_temp_str);
    m_ROI3FitErr.SetWindowText(l_temp_str);
    m_ROIMeanTiVal.SetWindowText(l_temp_str);
    m_ROI1T1Val.SetWindowText(l_temp_str);
    m_ROI2T1Val.SetWindowText(l_temp_str);
    m_ROI3T1Val.SetWindowText(l_temp_str);
    m_ROIMeanT1Val.SetWindowText(l_temp_str);
    m_NoiseCorrectionVal.SetWindowText(l_temp_str); 
    m_flag = FALSE;// AN/Added
}


bool CTiPrepDlg::IsReselectWait()
{
    return m_is_reselect_wait;
}

/*************************************************************
* Method:       OnRadio3rdequ
* Description:  This function is used
* Returns:      void
*************************************************************/
void CTiPrepDlg::OnRadio3rdequ()
{
    TIPREP_TRACE(CTiPrepDlg::OnRadio3rdequ,MRTraceMarker9);
    m_nEqnSelectFlag = 2;
    int status = (CTiPrepView::GetView())->OnEquationThree(m_nEqnSelectFlag);
    if (status != E_NO_ERROR)
        DisplayErr(status);
}
/*************************************************************
* Method:       OnRadio4rthequ
* Description:  This function is used
* Returns:      void
*************************************************************/
void CTiPrepDlg::OnRadio4rthequ()
{
    TIPREP_TRACE(CTiPrepDlg::OnRadio4rthequ,MRTraceMarker9);
    m_nEqnSelectFlag = 3;
    int status = (CTiPrepView::GetView())->OnEquationFour(m_nEqnSelectFlag);
    if (status != E_NO_ERROR)
        DisplayErr(status);
}
/*************************************************************
* Method:       OnRadio5thequ
* Description:  This function is used
* Returns:      void
*************************************************************/
void CTiPrepDlg::OnRadio5thequ()
{
    TIPREP_TRACE(CTiPrepDlg::OnRadio5thequ,MRTraceMarker9);
    m_nEqnSelectFlag = 4;
    int status = 0;
    status = (CTiPrepView::GetView())->OnEquationFive(m_nEqnSelectFlag);
    if (status != E_NO_ERROR)
        DisplayErr(status);
}

/*************************************************************
* Method:       OnPhaseCorrectionClick
* Description:  This function is used
* Returns:      void
*************************************************************/
void CTiPrepDlg::OnPhaseCorrectionClick()
{
    //vector<CString> l_csImgInfo;
    //LPTSTR out_loid = NULL;
    CString loid = _T("parent Loid : ");
    CString l_csnewseries_loid(_T(""));
    //vector <CString> l_csNewLOIDs;
    //unsigned int i = 0;
    m_one_button_phase_correction.EnableWindow(FALSE);
    CTiPrepView::GetView()->SetIselEvent(EVENT_AUTOPHASE);
    //CTiPrepView::GetView()->SetWaitCursor(true);
    AfxGetApp()->DoWaitCursor(1);
    BOOL l_bselImg = CTiPrepView::GetView()->GetSelectedImages();

    if(FALSE == l_bselImg){
        AfxGetApp()->DoWaitCursor(-1);
    }
}

/*************************************************************
* Method:       OnDataTypeDisplay
* Description:  This function is used
* Returns:      void
*************************************************************/
void CTiPrepDlg::OnDataTypeDisplay()
{
/* m_image_data_type_control.AddString(L"Real");
m_image_data_type_control.AddString(L"Imaginary");
    m_image_data_type_control.AddString(L"Magnitude");*/
}

/*************************************************************
* Method:       OnButtonSaveRoiData
* Description:  This function is used
* Returns:      void
*************************************************************/
void CTiPrepDlg::OnButtonSaveRoiData() //AS/defect fix
{
    BOOL l_roi[3];  //AS/defect fix
    l_roi[0] = m_checkDispROI1Data.GetCheck();
    l_roi[1] = m_checkDispROI2Data.GetCheck();
    l_roi[2] = m_checkDispROI3Data.GetCheck();
    bool l_bStatus = CTiPrepView::GetView()->SaveRoiDataToFile(l_roi);
}

/*************************************************************
* Method:       OnHideBtnClick
* Description:  This function is used
* Returns:      void
*************************************************************/
void CTiPrepDlg::OnHideBtnClick()
{
    CString l_csCtrlText;
    m_hide_btn.GetWindowText(l_csCtrlText);
    
    if(l_csCtrlText == _T("Hide")) {
        m_hide_btn.SetWindowText(_T("Show"));
        CRect l_rcHideBtn;
        CRect l_rcMovedCtrl;
        m_hide_btn.GetWindowRect(&l_rcHideBtn);
        ScreenToClient(&l_rcHideBtn);
        int l_nRectTop = l_rcHideBtn.bottom + 22;
        m_SaveRoiDataToFileButton.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        int l_nRectLeft = l_rcMovedCtrl.left;
        m_SaveRoiDataToFileButton.SetWindowPos
            (&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);

        m_ApplyTI.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_ApplyTI.SetWindowPos
            (&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);

        m_ApplyButton.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_ApplyButton.SetWindowPos
            (&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        m_ReselectButton.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_ReselectButton.SetWindowPos
            (&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        m_ResetROIButton.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_ResetROIButton.SetWindowPos
            (&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        m_ExitButton.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_ExitButton.SetWindowPos
            (&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        GetWindowRect(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_dlgHeightOrg = l_rcMovedCtrl.Height();
        MoveWindow(l_rcMovedCtrl.left, l_rcMovedCtrl.top, 
            l_rcMovedCtrl.Width(), 672/*l_nRectTop + l_rcMovedCtrl.top+81*/); 
        ShowControls(FALSE);
    } else {
        m_hide_btn.SetWindowText(_T("Hide"));
        //CRect l_rcHideBtn;
        CRect l_rcMovedCtrl;
        GetWindowRect(&l_rcMovedCtrl);
        m_SaveRoiDataToFileButton.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        int l_nRectLeft = l_rcMovedCtrl.left;
        m_SaveRoiDataToFileButton.SetWindowPos
            (&wndTop, 9, 982, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        
        m_ApplyTI.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_ApplyTI.SetWindowPos
            (&wndTop, 152, 982, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER); 


        m_ApplyButton.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_ApplyButton.SetWindowPos
            (&wndTop, 291, 982, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER); 
        m_ReselectButton.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_ReselectButton.SetWindowPos
            (&wndTop, 444, 982, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER); 
        m_ResetROIButton.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_ResetROIButton.SetWindowPos
            (&wndTop, 597, 982, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER); 
        m_ExitButton.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_ExitButton.SetWindowPos
            (&wndTop, 750, 982, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER); 
        GetWindowRect(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        MoveWindow(l_rcMovedCtrl.left, l_rcMovedCtrl.top, l_rcMovedCtrl.Width(), m_dlgHeightOrg); 
        ShowControls(TRUE);
    }
    
    if(m_checkT1Map.GetCheck() == BST_CHECKED) {
        DisplayControlsMapMode(true);
    }
}


/*************************************************************
* Method:       ShowControls
* Description:  This function is used
* Parameter:    BOOL f_bShowCtrl
* Returns:      void
*************************************************************/
void CTiPrepDlg::ShowControls(BOOL f_bShowCtrl)
{
    //m_SelectMgImgButton.ShowWindow(f_bShowCtrl);
  //  m_SelectReImgButton.ShowWindow(f_bShowCtrl);
  //  m_SelectImImgButton.ShowWindow(f_bShowCtrl);
   // m_CorrectPhaseImgButton.ShowWindow(f_bShowCtrl);
  //  m_SelectReconButton.ShowWindow(f_bShowCtrl);
  //  m_ReconstructionButton.ShowWindow(f_bShowCtrl);
  //  m_CopyButton.ShowWindow(f_bShowCtrl);
    //m_invTI.ShowWindow(f_bShowCtrl);
    //m_recoveryT1.ShowWindow(f_bShowCtrl);

    m_LabelDispOpt.ShowWindow(f_bShowCtrl);
    m_inversionTime.ShowWindow(f_bShowCtrl);
    m_recoveryTime.ShowWindow(f_bShowCtrl);
    m_fittingErr.ShowWindow(f_bShowCtrl);
    m_reverseMin.ShowWindow(f_bShowCtrl);
    m_Display.ShowWindow(f_bShowCtrl);
    m_lableRepetitionTime.ShowWindow(f_bShowCtrl);
    m_lableMean.ShowWindow(f_bShowCtrl);
    m_lableROI1.ShowWindow(f_bShowCtrl);
    m_lableROI2.ShowWindow(f_bShowCtrl);
    m_lableROI3.ShowWindow(f_bShowCtrl);
    m_checkOrgData.ShowWindow(f_bShowCtrl);
    m_checkInvData.ShowWindow(f_bShowCtrl);
    m_checkEditROIData.ShowWindow(f_bShowCtrl);
    m_checkFitData.ShowWindow(f_bShowCtrl);
    m_checkDispROI1Data.ShowWindow(f_bShowCtrl);
    m_checkDispROI2Data.ShowWindow(f_bShowCtrl);
    m_checkDispROI3Data.ShowWindow(f_bShowCtrl);
    m_checkRevDispROI1Data.ShowWindow(f_bShowCtrl);
    m_checkRevDispROI2Data.ShowWindow(f_bShowCtrl);
    m_checkRevDispROI3Data.ShowWindow(f_bShowCtrl);
    m_checkNoiseROI1Data.ShowWindow(f_bShowCtrl);
    m_checkNoiseROI2Data.ShowWindow(f_bShowCtrl);
    m_checkOffsetCorrection.ShowWindow(f_bShowCtrl);
    m_NoiseCorrectionVal.ShowWindow(f_bShowCtrl);
    m_radioEqu1.ShowWindow(f_bShowCtrl);
    m_radioEqu2.ShowWindow(f_bShowCtrl);
    m_radioEqu3.ShowWindow(f_bShowCtrl);
    m_radioEqu4.ShowWindow(f_bShowCtrl);
    m_radioEqu5.ShowWindow(f_bShowCtrl);
    m_ROI1FitErr.ShowWindow(f_bShowCtrl);
    m_ROI2FitErr.ShowWindow(f_bShowCtrl);
    m_ROI3FitErr.ShowWindow(f_bShowCtrl);
    m_ROI1TiVal.ShowWindow(f_bShowCtrl);
    m_ROI2TiVal.ShowWindow(f_bShowCtrl);
    m_ROI3TiVal.ShowWindow(f_bShowCtrl);
    m_ROIMeanTiVal.ShowWindow(f_bShowCtrl);
    m_ROI1T1Val.ShowWindow(f_bShowCtrl);
    m_ROI2T1Val.ShowWindow(f_bShowCtrl);
    m_ROI3T1Val.ShowWindow(f_bShowCtrl);
    m_ROIMeanT1Val.ShowWindow(f_bShowCtrl);
  //  m_Recon.ShowWindow(f_bShowCtrl);
    m_RepTime.ShowWindow(f_bShowCtrl);
//    m_listctrl_scannedProtocol.ShowWindow(f_bShowCtrl);
//    m_listctrl_Reocn.ShowWindow(f_bShowCtrl);
//    m_listctrl_Reocn.ShowWindow(f_bShowCtrl);
    m_displayOption.ShowWindow(f_bShowCtrl);
    m_dataCalc.ShowWindow(f_bShowCtrl);
    m_equetionFrame.ShowWindow(f_bShowCtrl);
    m_NoiseCorrectionFrame.ShowWindow(f_bShowCtrl);
    m_phase_correction_frame.ShowWindow(f_bShowCtrl);
    m_select_filter_frame.ShowWindow(f_bShowCtrl);
    m_select_filter_static_box.ShowWindow(f_bShowCtrl);
    m_select_filter_combo_box.ShowWindow(f_bShowCtrl);
    m_filter_size_static_box.ShowWindow(f_bShowCtrl);
    m_filter_size_edit_box.ShowWindow(f_bShowCtrl);
    m_create_series_check_box.ShowWindow(f_bShowCtrl);
    m_create_series_static_box.ShowWindow(f_bShowCtrl);
    m_one_button_phase_correction.ShowWindow(f_bShowCtrl);
    m_image_data_type_control.ShowWindow(f_bShowCtrl);
    m_data_form.ShowWindow(f_bShowCtrl);
    m_selected_protocol_display.ShowWindow(f_bShowCtrl);
    m_LabelNoiseCorrection.ShowWindow(f_bShowCtrl);
    m_LabelNoiseCorrectionVal.ShowWindow(f_bShowCtrl);
    GetDlgItem(IDC_STATIC_NOISE_ROI1)->ShowWindow(f_bShowCtrl);
    GetDlgItem(IDC_STATIC_NOISE_ROI2)->ShowWindow(f_bShowCtrl);
    GetDlgItem(IDC_STATIC_OFFSET_CORRECTION)->ShowWindow(f_bShowCtrl);
    if(m_checkT1Map.GetCheck()){
        EnableFilterCtrls(true); 
    }else{
        EnableFilterCtrls(false); 
    }
}

/*************************************************************
* Method:       OnMenuAddoffset
* Description:  This function is used
* Returns:      void
*************************************************************/
void CTiPrepDlg::OnMenuAddoffset()
{
    TIPREP_TRACE(CTiPrepDlg::OnMenuAddoffset,MRTraceMarker9);
    
	if(m_AddOffsetDlg && m_AddOffsetDlg->GetSafeHwnd()) return;
    m_AddOffsetDlg = new CAddOffsetDlg(this);
    if (NULL != m_AddOffsetDlg)
    {
        m_AddOffsetDlg->DoModal();
    }
    if (m_AddOffsetDlg)
    {
        delete m_AddOffsetDlg;
        m_AddOffsetDlg = NULL;
    }
}
/*************************************************************
* Method:       OnCheckT1map
* Description:  This function is used
* Returns:      void
*************************************************************/
void CTiPrepDlg::OnCheckT1map()
{
    TIPREP_TRACE(CTiPrepDlg::OnCheckT1map,MRTraceMarker9);
    //BOOL l_bT1mapflag = FALSE;
    int l_t1_map_check_box_status = 0; 
    ClearGraphCtl();
   // (CTiPrepView::GetView())->OnReselect(); //AS
    //CTiPrepView::GetView()->GetRoiDataAndDisplayROI(); //AS
    m_flag = FALSE;
    if(CTiPrepView::GetView()->SavePreviousROIstate() == FALSE) { 
        CTiPrepLogHelper::WriteToErrorLog
            (_T("OnCheckT1map(): SavePreviousROIstate Failed"), __LINE__, __FILE__);
    }
    CTiPrepView::GetView()->DeAllocateMemoryOnCheckT1Map();      
    if(m_checkT1Map.GetCheck() == BST_CHECKED) {
        EnableFilterCtrls(true);
        l_t1_map_check_box_status = 1;
        m_tigraphctrl.GetT1MapCheckBoxStatus(l_t1_map_check_box_status);
        (CTiPrepView::GetView())->SetT1MapFlag(true);
    } else if(m_checkT1Map.GetCheck() == BST_UNCHECKED) {
        EnableFilterCtrls(false);
        l_t1_map_check_box_status = 1;
        m_tigraphctrl.GetT1MapCheckBoxStatus(l_t1_map_check_box_status);
        (CTiPrepView::GetView())->SetT1MapFlag(false);
        (CTiPrepView::GetView())->SendMsg("EXIT");  
    }
    
    DisplayControlsMapMode(false);
    UpdateWindow();
}

/*************************************************************
* Method:       DisplayPixPos
* Description:  This function is used
* Parameter:    int x
* Parameter:    int y
* Returns:      void
*************************************************************/
void CTiPrepDlg::DisplayPixPos(int x, int y)
{
    CString str(_T(""));
    str.Format(_T("Mouse Pos: (%d,%d)"), x, y);
    m_lbl_pix_pos.SetWindowText(str);
}

/*************************************************************
* Method:       DisplayPhCorrSeries
* Description:  This function is used
* Parameter:    CString & name
* Returns:      void
*************************************************************/
void CTiPrepDlg::DisplayPhCorrSeries(CString &name)
{
    m_selected_protocol_display.SetWindowText(name);
}

void CTiPrepDlg::DisplayPCSelectedSeries(CString & series_name, int select_btn)
{
    m_pManualPhaseCorr->DisplaySeriesSelectedForManualPhaseCorr(series_name, select_btn);
    
}

void CTiPrepDlg::displayMapModeData()
{
    TIPREP_TRACE(CTiPrepDlg::displayMapModeData,MRTraceMarker3);
    
    try {
        CString l_temp_str, l_temp_errStr, l_temp_t1val;
        //double meanVal_TI = 0.0;
        BOOL l_roi_status = FALSE;
        BOOL l_roiRev_status = FALSE;
        BOOL l_graphOpt[3];
        //Get check box status
        l_roi_status = m_checkDispROI1Data.GetCheck();
        l_roiRev_status = m_checkRevDispROI1Data.GetCheck();
        l_graphOpt[ORG_DATA] = m_checkOrgData.GetCheck();
        l_graphOpt[INV_DATA] = m_checkInvData.GetCheck();
        l_graphOpt[FIT_DATA] = m_checkFitData.GetCheck();
        double *l_tivalues = NULL;
        double *l_Errvalues = NULL;
        double *l_t1Values = NULL;
        double *l_tiRevValues = NULL;
        double *l_errRevValuels = NULL;
        double *l_t1RevValues = NULL;
        CTiPrepView::GetView()->GetDisplayVals(&l_tivalues, &l_Errvalues,
            &l_t1Values, &l_tiRevValues,
            &l_errRevValuels, &l_t1RevValues);
        CString l_csMean_Ti = _T("");
        int l_nStatus = E_NO_ERROR;
        l_nStatus = getMeanTiMapMode(l_roi_status, l_roiRev_status,
            l_tivalues, l_Errvalues,
            l_tiRevValues, l_errRevValuels,l_t1RevValues,l_t1Values,l_csMean_Ti);
        getMeanT1MapMode(l_roi_status, l_roiRev_status,
            l_t1Values, l_Errvalues,
            l_t1RevValues, l_errRevValuels);
        m_tigraphctrl.SetMeanTiValue(l_csMean_Ti);
        m_tigraphctrl.ResetGraphWindowMapMode(TRUE, l_roi_status, l_roiRev_status, l_graphOpt);  
        int l_nSelectedIndex = CTiPrepView::GetView()->m_DisplayData->GetSelectedIndex();
        long l_nNumOfPoints = CTiPrepView::GetView()->m_DisplayData->GetTotalNumberOfPointsInAnnularRegion();
        
        if((l_nSelectedIndex < 0) || (l_nSelectedIndex >= l_nNumOfPoints)) { 
            CString l_csval(_T(""));
            l_csval.Format(_T("Selected Index: %d"),l_nSelectedIndex);
            CTiPrepLogHelper::WriteToTraceLog(_T("MRI_TIPREP"), MRTraceMarker14, l_csval, _T("displayMapModeData(): Selected index out of range error"));
            l_temp_str.Format(_T("---.---"));
            m_ROI1TiVal.SetWindowText(l_temp_str);
            m_ROI1FitErr.SetWindowText(l_temp_str);
            m_ROI1T1Val.SetWindowText(l_temp_str);
         //   m_ROIMeanTiVal.SetWindowText(l_temp_str);
        //    m_ROIMeanT1Val.SetWindowText(l_temp_str);
            return;
        }
        //double l_dTempObj = 0.0;
        if(l_roi_status) {
            if(l_roiRev_status) {
                if(-1 == l_t1RevValues[l_nSelectedIndex]){
                    l_temp_str.Format(_T("Err"));
                    l_temp_errStr.Format(_T("Err"));
                    l_temp_t1val.Format(_T("Err"));
                }else{
                    //l_tiRevValues[l_nSelectedIndex] = l_tiRevValues[l_nSelectedIndex] + 0.05;
//                     if((l_tiRevValues[l_nSelectedIndex] > DBL_MAX) || (l_tiRevValues[l_nSelectedIndex] < DBL_MIN)){
//                         l_temp_str.Format(_T("Inf"));
//                         
//                     }else{
//                         l_temp_str.Format(_T("%0.1lf"), /*(10**/l_tiRevValues[l_nSelectedIndex] /*+ 0.5)*0.1*/ );                       
//                     }
//                     if((l_tiRevValues[l_nSelectedIndex] <= DBL_MAX) && (l_tiRevValues[l_nSelectedIndex] >= DBL_MIN)){
//                         
//                         l_temp_str.Format(_T("%0.1lf"), /*(10**/l_tiRevValues[l_nSelectedIndex] /*+ 0.5)*0.1*/ );                       
//                     }else{
//                         l_temp_str.Format(_T("Inf"));
//                         
//                     }
                    if(l_tiRevValues[l_nSelectedIndex] == numeric_limits<double>::infinity( )){
                        //implies infinity
                        l_temp_str.Format(_T("Inf"));
                    }else{
                        l_temp_str.Format(_T("%0.1lf"), /*(10**/l_tiRevValues[l_nSelectedIndex] /*+ 0.5)*0.1*/ );
                    }
                    //l_errRevValuels[l_nSelectedIndex] = l_errRevValuels[l_nSelectedIndex] + 0.05;
                    l_temp_errStr.Format(_T("%0.1lf"), (10*l_errRevValuels[l_nSelectedIndex] + 0.5)*0.1);
                    //l_t1RevValues[l_nSelectedIndex] = l_t1RevValues[l_nSelectedIndex] + 0.05;
                    l_temp_t1val.Format(_T("%0.1lf"), (10*l_t1RevValues[l_nSelectedIndex] + 0.5)*0.1);
                }

            } else {
                if(-1 == l_t1Values[l_nSelectedIndex]){
                    l_temp_str.Format(_T("Err"));
                    l_temp_errStr.Format(_T("Err"));
                    l_temp_t1val.Format(_T("Err"));
                }else{
                    //l_tivalues[l_nSelectedIndex] = l_tivalues[l_nSelectedIndex] + 0.05;
//                     if((l_tivalues[l_nSelectedIndex] > DBL_MAX) || (l_tivalues[l_nSelectedIndex] < DBL_MIN)){
//                         l_temp_str.Format(_T("Inf"));
//                         
//                     }else{
//                         l_temp_str.Format(_T("%0.1lf"), /*(10**/l_tivalues[l_nSelectedIndex] /*+ 0.5)*0.1*/ );                       
//                     }
//                     if((l_tivalues[l_nSelectedIndex] <= DBL_MAX) && (l_tivalues[l_nSelectedIndex] >= DBL_MIN)){
//                         
//                         l_temp_str.Format(_T("%0.1lf"), /*(10**/l_tivalues[l_nSelectedIndex] /*+ 0.5)*0.1*/ );                       
//                     }else{
//                         l_temp_str.Format(_T("Inf"));
//                         
//                     }

                    if(l_tivalues[l_nSelectedIndex] == numeric_limits<double>::infinity( )){
                        //implies infinity
                        l_temp_str.Format(_T("Inf"));
                    }else{
                        l_temp_str.Format(_T("%0.1lf"), /*(10**/l_tivalues[l_nSelectedIndex] /*+ 0.5)*0.1*/ );
                    }

                    //l_Errvalues[l_nSelectedIndex] = l_Errvalues[l_nSelectedIndex] + 0.05;
                    l_temp_errStr.Format(_T("%0.1lf"), (10*l_Errvalues[l_nSelectedIndex] + 0.5)*0.1);
                    //l_t1Values[l_nSelectedIndex] = l_t1Values[l_nSelectedIndex] + 0.05;
                    l_temp_t1val.Format(_T("%0.1lf"), (10*l_t1Values[l_nSelectedIndex] + 0.5)*0.1);
                }
            }
            
            m_ROI1TiVal.SetWindowText(l_temp_str);
            m_ROI1FitErr.SetWindowText(l_temp_errStr);
            m_ROI1T1Val.SetWindowText(l_temp_t1val);
        } else {
            l_temp_str.Format(_T("---.---"));
            m_ROI1TiVal.SetWindowText(l_temp_str);
            m_ROI1FitErr.SetWindowText(l_temp_str);
            m_ROI1T1Val.SetWindowText(l_temp_str);
        }
    } catch(CMemoryException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted;
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Memory error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog(l_csStrFormatted, __LINE__, __FILE__);
    } catch(CException& e) {
        TCHAR   szCause[255];
        CString l_csStrFormatted;
        e.GetErrorMessage(szCause, 255);
        l_csStrFormatted = _T("Exception occurred with this error: ");
        l_csStrFormatted += szCause;
        CTiPrepLogHelper::WriteToErrorLog
            (l_csStrFormatted, __LINE__, __FILE__);
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog
            (_T("Exception Occurred during displayMapModeData"), __LINE__, __FILE__);
    }
}

/*************************************************************
* Method:    	getMeanTiMapMode
* Description: 	This function is used 
* Parameter: 	BOOL f_roi :
* Parameter: 	BOOL f_roiRev :
* Parameter: 	double * f_pfTiVals :
* Parameter: 	double * f_pfErrVals :
* Parameter: 	double * f_pfTiRevVals :
* Parameter: 	double * f_pfErrRevVals :
* Returns:   	CString
*************************************************************/
int CTiPrepDlg::getMeanTiMapMode(BOOL f_roi, 
                                     BOOL f_roiRev, double * f_pfTiVals, 
                                     double * f_pfErrVals, double * f_pfTiRevVals, 
                                     double * f_pfErrRevVals,double * f_pfT1RevVals,double * f_pfT1Vals,CString & f_csMean_Ti)
{
    TIPREP_TRACE(CTiPrepDlg::getMeanTiMapMode,MRTraceMarker3);
    try{
        
        int l_roiDisp = 0;
        double l_dVal_TI;
        double l_dVal_T1 = 0.0;
        double l_dVal_Err ;
        double l_dmeanVal_TI = 0.0;
        long l_nNumOfPoints = 0;
        l_nNumOfPoints = CTiPrepView::GetView()->m_DisplayData->GetTotalNumberOfPointsInAnnularRegion();
        
        if(NULL == f_pfTiVals || NULL == f_pfTiRevVals || NULL == f_pfErrVals || NULL == f_pfErrRevVals) {
            CTiPrepLogHelper::WriteToErrorLog(_T("getMeanTiMapMode(): Function parameter found NULL"), __LINE__, __FILE__);
            f_csMean_Ti.Format(_T("---.---"));
            return E_ERROR;
        }
        
        for(int l_nroi = 0 ; l_nroi < l_nNumOfPoints ; l_nroi++) {
            if(!f_roiRev) {
                l_dVal_TI = f_pfTiVals[l_nroi];
                l_dVal_Err = f_pfErrVals[l_nroi];
                l_dVal_T1 = f_pfT1Vals[l_nroi];
            } else {
                l_dVal_TI = f_pfTiRevVals[l_nroi];
                l_dVal_Err = f_pfErrRevVals[l_nroi];
                l_dVal_T1 = f_pfT1Vals[l_nroi];
            }
            
            if(f_roi) { 
                //             if ((l_dVal_TI <= DBL_MAX) && (l_dVal_TI >= DBL_MIN))
                //             {
                //                 l_roiDisp++;
                //                 l_dmeanVal_TI += l_dVal_TI ;
                // 
                //             }
                if(l_dVal_TI == numeric_limits<double>::infinity( )){
                    //implies infinity
                }else{
					if(-1 != (int)l_dVal_T1){
						l_roiDisp++;
						l_dmeanVal_TI += l_dVal_TI ; 
					}
                    
                }
            }
        }
        
        if(0 != l_roiDisp) {
            //             if (0 >= l_dmeanVal_TI)
            //             {
            //                 f_csMean_Ti.Format(_T("---.---"));
            //                 m_ROIMeanTiVal.SetWindowText(f_csMean_Ti);
            //             }else
            {
                l_dmeanVal_TI = l_dmeanVal_TI / l_roiDisp ;
                
                if(l_dmeanVal_TI == numeric_limits<double>::infinity( )){
                    //implies infinity
                    f_csMean_Ti.Format(_T("Inf"));
                    m_ROIMeanTiVal.SetWindowText(f_csMean_Ti);
                }else{
                    f_csMean_Ti.Format(_T("%0.1lf"), /*(10**/l_dmeanVal_TI /*+ 0.5)*0.1*/);
                    m_ROIMeanTiVal.SetWindowText(f_csMean_Ti);
                }
                
            }
        } else {
            l_dmeanVal_TI = -1 ;
            m_ROIMeanTiVal.SetWindowText(_T("---.---"));
        }
        
        // m_ROIMeanTiVal.SetWindowText(_T("\x221E")); checking
        return E_NO_ERROR;
    }catch(...){
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_TIPREPDLG, __LINE__, __FILE__);
        return E_ERROR;        
    }
}

/*************************************************************
* Method:    	DisplayControlsMapMode
* Description: 	This function is used 
* Returns:   	void
*************************************************************/
void CTiPrepDlg::DisplayControlsMapMode(bool f_bIsCalledFromShowHide)
{
    bool l_bT1MapMode = (CTiPrepView::GetView())->GetT1MapFlag();
    
    if(true == l_bT1MapMode) {
        if(true != f_bIsCalledFromShowHide){   //NOTE: when f_bIsCalledFromShowHide is true it indicates
            //this function is called from Show/Hide callback
            //hence the following should not get executed in case of Show/Hide button
                                               //when not called from Show/Hide button the following should get executed
        m_checkDispROI1Data.SetCheck(BST_CHECKED);
        m_checkDispROI2Data.SetCheck(BST_UNCHECKED);
        m_checkDispROI3Data.SetCheck(BST_UNCHECKED);
        
            m_checkRevDispROI1Data.SetCheck(BST_UNCHECKED);            
            m_checkNoiseROI1Data.SetCheck(BST_UNCHECKED);
            m_checkNoiseROI2Data.SetCheck(BST_UNCHECKED);
            m_checkOffsetCorrection.SetCheck(BST_UNCHECKED);
        }
        m_checkRevDispROI2Data.SetCheck(BST_UNCHECKED);
        m_checkRevDispROI3Data.SetCheck(BST_UNCHECKED);
        GetDlgItem(IDC_CHECK_ROI2_DISPLAY)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_CHECK_ROI3_DISPLAY_NEW)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_CHECK_ROI2_REVERSEMIN)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_CHECK_ROI3_DISPLAY)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_ROI2FITTINGERROR)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_ROI3FITTINGERROR)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_ROI2TI)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_ROI3TI)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_ROI2T1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_ROI3T1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_ROI2)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_ROI3)->ShowWindow(SW_HIDE);
        BOOL l_bHideflag = FALSE;
        if(true != f_bIsCalledFromShowHide){     //refer NOTE above for this condition
        l_bHideflag = (CTiPrepView::GetView())->GetROIInstance()->HideROI(0, TRUE);
        
        if(!l_bHideflag) {
            CString l_csErrMsg(_T("Unable to hide/show ROI."));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
        }
        
        l_bHideflag = (CTiPrepView::GetView())->GetROIInstance()->HideROI(2, FALSE);
        
        if(!l_bHideflag) {
            CString l_csErrMsg(_T("Unable to hide/show ROI."));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return;
        }
        l_bHideflag = (CTiPrepView::GetView())->GetROIInstance()->HideROI(NOISE_ROI_1, FALSE);
        
        if(!l_bHideflag) {
            CString l_csErrMsg(_T("Unable to hide/show ROI."));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return;
        }
        l_bHideflag = (CTiPrepView::GetView())->GetROIInstance()->HideROI(NOISE_ROI_2, FALSE);
        
        if(!l_bHideflag) {
            CString l_csErrMsg(_T("Unable to hide/show ROI."));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return;
            }
        }
        m_initialGuessApply.ShowWindow(SW_HIDE);
        m_initialGuessApply.ShowWindow(SW_HIDE);
    } else if(false == l_bT1MapMode) {
         if(true != f_bIsCalledFromShowHide){    //refer NOTE above for this condition
        m_checkDispROI1Data.SetCheck(BST_CHECKED);
        m_checkDispROI2Data.SetCheck(BST_UNCHECKED);
        m_checkDispROI3Data.SetCheck(BST_UNCHECKED);
       
            m_checkRevDispROI1Data.SetCheck(BST_UNCHECKED);            
            m_checkNoiseROI1Data.SetCheck(BST_UNCHECKED);
            m_checkNoiseROI2Data.SetCheck(BST_UNCHECKED);
            m_checkOffsetCorrection.SetCheck(BST_UNCHECKED);
        }

        m_checkRevDispROI2Data.SetCheck(BST_UNCHECKED);
        m_checkRevDispROI3Data.SetCheck(BST_UNCHECKED);
        GetDlgItem(IDC_CHECK_ROI2_DISPLAY)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_CHECK_ROI3_DISPLAY)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_CHECK_ROI2_REVERSEMIN)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_CHECK_ROI3_DISPLAY_NEW)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_EDIT_ROI2FITTINGERROR)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_EDIT_ROI3FITTINGERROR)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_EDIT_ROI2TI)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_EDIT_ROI3TI)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_EDIT_ROI2T1)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_EDIT_ROI3T1)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATIC_ROI2)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATIC_ROI3)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATIC_PIX_POS)->ShowWindow(SW_HIDE);
        BOOL l_bHideflag = FALSE;
        if(true != f_bIsCalledFromShowHide){     //refer NOTE above for this condition
        l_bHideflag = (CTiPrepView::GetView())->GetROIInstance()->HideROI(0, TRUE);
        
        if(!l_bHideflag) {
            CString l_csErrMsg(_T("Unable to hide/show ROI."));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
        }
        
        l_bHideflag = (CTiPrepView::GetView())->GetROIInstance()->HideROI(1, FALSE);
        
        if(!l_bHideflag) {
            CString l_csErrMsg(_T("Unable to hide/show ROI."));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
        }
        
        l_bHideflag = (CTiPrepView::GetView())->GetROIInstance()->HideROI(2, FALSE);
        
        if(!l_bHideflag) {
            CString l_csErrMsg(_T("Unable to hide/show ROI."));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return;
        }
        l_bHideflag = (CTiPrepView::GetView())->GetROIInstance()->HideROI(NOISE_ROI_1, FALSE);
        
        if(!l_bHideflag) {
            CString l_csErrMsg(_T("Unable to hide/show ROI."));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return;
        }
        l_bHideflag = (CTiPrepView::GetView())->GetROIInstance()->HideROI(NOISE_ROI_2, FALSE);
        
        if(!l_bHideflag) {
            CString l_csErrMsg(_T("Unable to hide/show ROI."));
            CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            return;
            }
        }
        m_lbl_pix_pos.ShowWindow(SW_HIDE);
        m_initialGuessApply.ShowWindow(SW_SHOW);
        m_initialGuessApply.ShowWindow(SW_SHOW);

    }
    if(true != f_bIsCalledFromShowHide){     //refer NOTE above for this condition
    CTiPrepView::GetView()->GetRoiDataAndDisplayROI(); 
    if (true == l_bT1MapMode)
    {
        BOOL l_bHideflag = (CTiPrepView::GetView())->GetROIInstance()->HideROI(1, TRUE); 
    
             if(!l_bHideflag) {
                CString l_csErrMsg(_T("Unable to hide/show ROI."));
                CTiPrepLogHelper::WriteToErrorLog(l_csErrMsg, __LINE__, __FILE__);
            }        
        }
    }
    Invalidate();
}

/*************************************************************
* Method:    	SetRepeatationTime
* Description: 	This function is used 
* Parameter: 	float & f_tr_value :
* Returns:   	void
*************************************************************/
void CTiPrepDlg::SetRepeatationTime(float &f_tr_value)
{
    CString l_rep_time(_T(""));
    //f_tr_value = f_tr_value + 0.05;
    l_rep_time.Format(_T("%0.1f"), /*(10**/f_tr_value /*+ 0.5)*0.1*/);
    m_RepTime.SetWindowText(l_rep_time);
}

/*************************************************************
* Method:    	OnCheckManualGuess
* Description: 	This function is used 
* Returns:   	void
*************************************************************/
void CTiPrepDlg::OnCheckManualGuess()
{
    if(BST_CHECKED == m_initialGuessChk.GetCheck()) {
        m_initialGuessApply.EnableWindow(TRUE);
        (CTiPrepView::GetView())->setInitialGuessFlag(1);
    } else if(BST_UNCHECKED == m_initialGuessChk.GetCheck()) {
        m_initialGuessApply.EnableWindow(FALSE);
        (CTiPrepView::GetView())->setInitialGuessFlag(0);
    }
}

/*************************************************************
* Method:    	OnButtonManualGuess
* Description: 	This function is used 
* Returns:   	void
*************************************************************/
void CTiPrepDlg::OnButtonManualGuess()
{
    CManualInitialGuessDlg l_manualGuessDlg;
    l_manualGuessDlg.setEqnFlag(m_nEqnSelectFlag);
    
    if(IDCANCEL == l_manualGuessDlg.DoModal()) {
        //
    }
}

/*************************************************************
* Method:    	EnableMapMode
* Description: 	This function is used 
* Returns:   	bool
*************************************************************/
bool CTiPrepDlg::EnableMapMode()
{
    GetDlgItem(IDC_CHECK_T1MAP)->ShowWindow(SW_SHOW);
    m_checkT1Map.SetCheck(TRUE);//+Mpower_65/Jun-2013/VP..T1MAP_pwd
    return(true);
}

/*************************************************************
* Method:    	EnableAvgMode
* Description: 	This function is used 
* Returns:   	bool
*************************************************************/
bool CTiPrepDlg::EnableAvgMode()
{
    GetDlgItem(IDC_CHECK_T1MAP)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_RADIO_4THEQU)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_RADIO_5THEQU)->ShowWindow(SW_HIDE);
    return(true);
}

/*************************************************************
* Method:       OnCheckCreateSeriesinAverageFilter
* Description:  This function is message handler.
*               Called when '' check box is checked/unchecked.
* Returns:      void
*************************************************************/
void CTiPrepDlg::OnCheckCreateSeriesinAverageFilter()
{
    TIPREP_TRACE(CTiPrepDlg::OnCheckCreateSeriesinAverageFilter,MRTraceMarker9);
}

/*************************************************************
* Method:    	IsAvgFilterOn
* Description: 	This function is used 
* Returns:   	bool
*************************************************************/
bool CTiPrepDlg::IsAvgFilterOn()
{
    CString l_type_of_filter(_T(""));
    m_select_filter_combo_box.GetWindowText(l_type_of_filter);
    
    if("None" == l_type_of_filter) {
        return false;
    } else {
        return true; 
    }
}

/*************************************************************
* Method:    	IsCreateFilteredSeries
* Description: 	This function is used 
* Returns:   	bool
*************************************************************/
bool CTiPrepDlg::IsCreateFilteredSeries()
{
    //check make series checkbox state
    int l_create_series_check_status = m_create_series_check_box.GetCheck();
    
    if(1 == l_create_series_check_status) {
        return true;
    } else {
        return false;
    }
}

/*************************************************************
* Method:    	GetFilterSize
* Description: 	This function is used 
* Parameter: 	int & filtersz :
* Returns:   	int
*************************************************************/
int CTiPrepDlg::GetFilterSize(int &filtersz)
{
    //validate text field and return text value
    CString l_filter_size_string(_T(""));
    char * l_filter_size_char = new char[10];
	if(NULL == l_filter_size_char){
		return E_ERROR;
	}
    m_filter_size_edit_box.GetWindowText(l_filter_size_string);
    sprintf(l_filter_size_char, "%S", l_filter_size_string);
    int l_filter_size = atoi(l_filter_size_char);   
    int l_csStrLen = l_filter_size_string.GetLength();
    for (int l_nCnt = 0;l_nCnt < l_csStrLen;l_nCnt++)
    {
        if(FALSE == isdigit(l_filter_size_char[l_nCnt])) {
			if(l_filter_size_char){
				delete [] l_filter_size_char;
				l_filter_size_char = NULL;
			}
            return E_ERROR_FILTER_INVALID;
        }
    }
    if(l_filter_size % 2 != 0 && l_filter_size > 1 && l_filter_size <= 21){
        filtersz = l_filter_size;
		if(l_filter_size_char){
			delete [] l_filter_size_char;
			l_filter_size_char = NULL;
		}
        return E_NO_ERROR;
    } else {
		if(l_filter_size_char){
			delete [] l_filter_size_char;
			l_filter_size_char = NULL;
		}
        return E_ERROR_FILTER_INVALID;
    }
}

/*************************************************************
* Method:    	OnOptionsManualPhaseCorr
* Description: 	This function is used 
* Returns:   	void
*************************************************************/
void CTiPrepDlg::OnOptionsManualPhaseCorr()
{
    if(NULL == m_pManualPhaseCorr) {
        m_pManualPhaseCorr = new CManualPhaseCorrDlg(this);
    }
    
    if(NULL != m_pManualPhaseCorr) {
        m_pManualPhaseCorr->DoModal();
        delete m_pManualPhaseCorr;
        m_pManualPhaseCorr = NULL;
    }
}

/*************************************************************
* Method:    	OnButtonGetDataType
* Description: 	This function is used 
* Returns:   	void
*************************************************************/
void CTiPrepDlg::OnButtonGetDataType()
{
    //long l_nNumImgSel = 0;
    CString l_csDataType(_T(""));
    CTiPrepView::GetView()->SetIselEvent(EVENT_DATA_TYPE);
    CTiPrepView::GetView()->SetWaitCursor(true);
    BOOL l_bselImg = CTiPrepView::GetView()->GetSelectedImages();

    if(FALSE == l_bselImg){
        CTiPrepView::GetView()->SetWaitCursor(false);
    }
}

/*************************************************************
* Method:    	SetDataForm
* Description: 	This function is used 
* Parameter: 	const CString & f_csDataType :
* Returns:   	void
*************************************************************/
void CTiPrepDlg::SetDataForm(const CString &f_csDataType)
{
    m_exDataType.SetWindowText(f_csDataType);
}

/*************************************************************
* Method:    	EnableFilterCtrls
* Description: 	This function is used 
* Parameter: 	bool flag :
* Returns:   	void
*************************************************************/
void CTiPrepDlg::EnableFilterCtrls(bool flag)
{
    GetDlgItem(IDC_STATIC_AVERAGE_FILTER)->ShowWindow(flag);
    GetDlgItem(IDC_STATIC_FILTERS)->ShowWindow(flag);
    GetDlgItem(IDC_STATIC_SELECT_FILTER)->ShowWindow(flag);
    GetDlgItem(IDC_STATIC_FILTER_SIZE)->ShowWindow(flag);
    GetDlgItem(IDC_STATIC_CREATE_SERIES)->ShowWindow(flag);
    m_select_filter_combo_box.ShowWindow(flag);
    m_filter_size_edit_box.ShowWindow(flag);
    m_create_series_check_box.ShowWindow(flag);
    m_select_filter_combo_box.EnableWindow(flag);
    m_filter_size_edit_box.EnableWindow(flag);
    m_create_series_check_box.EnableWindow(flag);
}

/*************************************************************
* Method:    	DisplayReconSelectedSeries
* Description: 	This function is used 
* Parameter: 	CString & f_csseries_name :
* Returns:   	void
*************************************************************/
void CTiPrepDlg::DisplayReconSelectedSeries(CString & f_csseries_name)
{
    m_pReconDlg->setSeriseName(f_csseries_name);
}


/*************************************************************
* Method:    	DisplayErr
* Description: 	This function is used 
* Parameter: 	int & f_nmsg_code :
* Parameter: 	int dlg_code :
* Returns:   	void
*************************************************************/
void CTiPrepDlg::DisplayErr(int &f_nmsg_code, int dlg_code)
{
    CErrorDlg *dlg = NULL;
    CWnd * parent = NULL;
    HWND pHwnd = NULL;
	//dlg = new CErrorDlg(this);

    
    switch(dlg_code) {
    case POLARITY_CORR_DLG:
        if (m_polarity_correction_dlg && m_polarity_correction_dlg->GetSafeHwnd())     
        {
            parent = m_polarity_correction_dlg;
            pHwnd = m_polarity_correction_dlg->GetSafeHwnd();
        }
        break;
    case ADD_OFFSET_DLG:
        if(m_AddOffsetDlg && m_AddOffsetDlg->GetSafeHwnd())    {
            parent = m_AddOffsetDlg;
            pHwnd = m_AddOffsetDlg->GetSafeHwnd();
        }
        break;
    case RECON_DLG:
        if (m_pReconDlg && this->m_pReconDlg->GetSafeHwnd()) {
            parent = m_pReconDlg;
            pHwnd = m_pReconDlg->GetSafeHwnd();
        }
        break;
    case PHASE_CORRECTION_DLG:
        if (m_pManualPhaseCorr && this->m_pManualPhaseCorr->GetSafeHwnd()) {
            parent = m_pManualPhaseCorr;
            pHwnd = m_pManualPhaseCorr->GetSafeHwnd();
        }
        break;
    case COPY_ROI_DLG:
        if (m_pCopyROIDlg && this->m_pCopyROIDlg->GetSafeHwnd()) {
            parent = m_pCopyROIDlg;
            pHwnd = m_pCopyROIDlg->GetSafeHwnd();
        }
        break;
    case SET_ROI_DLG:
        if (m_setroi_dlg && this->m_setroi_dlg->GetSafeHwnd()) {
            parent = m_setroi_dlg;
            pHwnd = m_setroi_dlg->GetSafeHwnd();
        }
        break; 
    default:
//         if (m_polarity_correction_dlg && m_polarity_correction_dlg->GetSafeHwnd())     
//         {
//             parent = m_polarity_correction_dlg;
//             pHwnd = m_polarity_correction_dlg->GetSafeHwnd();
//         } else {
        parent = this;
            pHwnd = this->GetSafeHwnd();
      //  }
        break;
    }

    if (EVENT_AUTOPHASE == CTiPrepView::GetView()->GetISelEvent())
    {
        m_one_button_phase_correction.EnableWindow(TRUE);
    }
    
    dlg = new CErrorDlg(parent);
    if (!dlg) 
    {
        CTiPrepLogHelper::WriteToErrorLog(_T("DisplayErr Mem allocation failed"),__LINE__,__FILE__);
        return;
	}
    dlg->SetHWnd(pHwnd);
    dlg->SetErrorValue(f_nmsg_code);
    dlg->ShowErrorMessage();

	if(NULL != dlg){
		delete dlg;
		dlg = NULL;
	}
}

void CTiPrepDlg::OnAboutMenu() 
{
    
    try{
        if(m_AbtDlg && m_AbtDlg->GetSafeHwnd()) {
            return;
        }
        
        m_AbtDlg = new CAboutDlg(this); 
        
        if(NULL != m_AbtDlg) {
            m_AbtDlg->DoModal();
        }
        
        if(m_AbtDlg) {
            delete m_AbtDlg;
            m_AbtDlg = NULL;
        }
    } catch(...) {
        CTiPrepLogHelper::WriteToErrorLog(EXCEPTION_TIPREPDLG, __LINE__, __FILE__);
    }
}

void CTiPrepDlg::GUIOnApply()
{

    AfxGetApp()->DoWaitCursor(1); // 1->>display the hourglass cursor
    m_ApplyButton.EnableWindow(FALSE);
    m_ReselectButton.EnableWindow(FALSE);
    m_ResetROIButton.EnableWindow(FALSE);
    m_checkT1Map.EnableWindow(FALSE);
    m_checkOffsetCorrection.EnableWindow(FALSE);

    GetDlgItem(IDC_RADIO_1STEQU)->EnableWindow(FALSE);
    GetDlgItem(IDC_RADIO_2NDEQU)->EnableWindow(FALSE);
    GetDlgItem(IDC_RADIO_3RDEQU)->EnableWindow(FALSE);
    GetDlgItem(IDC_RADIO_4THEQU)->EnableWindow(FALSE);
    GetDlgItem(IDC_RADIO_5THEQU)->EnableWindow(FALSE);
    
}

void CTiPrepDlg::GUIOnIdle()
{    
    AfxGetApp()->DoWaitCursor(-1); // -1->>remove the hourglass cursor
    m_ApplyButton.EnableWindow(TRUE);
    m_ReselectButton.EnableWindow(TRUE);
    m_ResetROIButton.EnableWindow(TRUE);
    m_checkT1Map.EnableWindow(TRUE);
    m_checkOffsetCorrection.EnableWindow(TRUE);
    GetDlgItem(IDC_RADIO_1STEQU)->EnableWindow(TRUE);
    GetDlgItem(IDC_RADIO_2NDEQU)->EnableWindow(TRUE);
    GetDlgItem(IDC_RADIO_3RDEQU)->EnableWindow(TRUE);
    GetDlgItem(IDC_RADIO_4THEQU)->EnableWindow(TRUE);
    GetDlgItem(IDC_RADIO_5THEQU)->EnableWindow(TRUE);
}

void CTiPrepDlg::EnablePhaseCorrectionButton(BOOL &f_bFlag)
{
    m_one_button_phase_correction.EnableWindow(TRUE);
}

///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////
void CTiPrepDlg::EnableDisableROI(bool flag)
{
m_checkT1Map.EnableWindow(flag);
m_SaveRoiDataToFileButton.EnableWindow(flag);
m_ApplyButton.EnableWindow(flag);
m_ResetROIButton.EnableWindow(flag);
}


void CTiPrepDlg::SetProgressDlg(CTiPrepProgressDlg *dlg)
{
    //if(dlg == NULL){ //AN/Revw/Delete progressdlg before calling this function with NULL param and only set pointer inside this function.
     //   delete m_progressdlg;
       // m_progressdlg = NULL;
   // }else{
        m_progressdlg = dlg;
    //}
}

void CTiPrepDlg::OnButtonApply_Ti() 
{
	CTiPrepView::GetView()->OnApply_Ti();
}

void CTiPrepDlg::DisableApplyTiButton()
{
    m_ApplyTI.ShowWindow(SW_SHOW);
    m_ApplyTI.EnableWindow(TRUE);
}

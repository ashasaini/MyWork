// PageOne.cpp : implementation file
//

#include "stdafx.h"
#include "epi.h"
#include "PageEPI.h"
//#include "EPIFormView.h"
#include "CommonDefinations.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageEPI property page
CPageEPI * CPageEPI::m_pPageOne = NULL;

IMPLEMENT_DYNCREATE(CPageEPI, CPropertyPage)


/*************************************************************
* Method:       CPageEPI
* Description:  This function is used
* Returns:
*************************************************************/
CPageEPI::CPageEPI() : CPageBase/*CPropertyPage*/(CPageEPI::IDD)
{
    //{{AFX_DATA_INIT(CPageEPI)
    m_chkProcessFlag = FALSE;
    m_radioT2Flag = -1;
    m_chkShiftPeak = FALSE;
    m_chkPCMethod = FALSE;
    m_spinFlag = FALSE;
    m_protData = NULL;
	m_radioShiftPeakFlag = -1;
	//}}AFX_DATA_INIT
}

/*************************************************************
* Method:       ~CPageEPI
* Description:  This function is used
* Returns:
*************************************************************/
CPageEPI::~CPageEPI()
{
    if(NULL != m_protData) {
        delete []m_protData;
        m_protData = NULL;
    }
}

/*************************************************************
* Method:       GetPageOneInstance
* Description:  This function is used
* Returns:      CPageEPI *
*************************************************************/
CPageEPI * CPageEPI::GetPageOneInstance()
{
    if(NULL == m_pPageOne) {
        m_pPageOne = new CPageEPI;
    } else {
        //
    }

    return m_pPageOne;
}
/*************************************************************
* Method:       DeletePageOneInstance
* Description:  This function is used
* Returns:      void
*************************************************************/
void CPageEPI::DeletePageOneInstance()
{
    if(NULL != m_pPageOne) {
        delete m_pPageOne;
        m_pPageOne = NULL;
    }
}

/*************************************************************
* Method:       DoDataExchange
* Description:  This function is used
* Parameter:    CDataExchange * pDX :
* Returns:      void
*************************************************************/
void CPageEPI::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPageEPI)
	DDX_Control(pDX, IDC_EDITMERGE, m_EditMerge);
	DDX_Control(pDX, IDC_BUTTON_RECON, m_CtrlReconButton);
	DDX_Control(pDX, IDC_COMBO_SELECT_STEP, m_ctrlCombStepSelection);
	DDX_Control(pDX, IDC_STATIC_STEP_SELECT, m_CtrlLabStepSelection);
	DDX_Control(pDX, IDC_STATIC_RECON, m_ctrlGrprecon);
    DDX_Control(pDX, IDC_LIST_PROTOCOL, m_RawDataList);
    DDX_Control(pDX, IDC_STATIC_STUDY_FILE_NAME, m_frmCtrlStudyName);
    DDX_Control(pDX, IDC_BUTTON_MERGE_DATA, m_buttonMregeData);
    DDX_Control(pDX, IDC_BUTTON_CORRECT_PHASE, m_buttonCorrectPhase);
    DDX_Control(pDX, IDC_CHECK_PC_METHOD, m_chkCtrlPCMethod);
    DDX_Control(pDX, IDC_CHECK_SHIFTPEAK, m_chkCtrlShiftPeak);
    DDX_Control(pDX, IDC_RADIO_DFT, m_radioPC);
    DDX_Control(pDX, IDC_RADIO_NON_SPEEDER, m_radioNonSpeeder);
    DDX_Control(pDX, IDC_RADIO_TIME_SHIFT, m_radioShiftPeak);
    DDX_Control(pDX, IDC_STATIC_SHIFT_PEAK_OPTIONS, m_frmShiftPeak);
    DDX_Control(pDX, IDC_STATIC_PC_OPTIONS, m_frmPCOptions);
    DDX_Control(pDX, IDC_STATIC_PHASE_OPTIONS, m_frmPhaseOptions);
    DDX_Control(pDX, IDC_STATIC_PROCESS_SELECTION, m_frmProcessSelection);
    DDX_Control(pDX, IDC_BUTTON_SEPAARTE_KSPACE, m_kSpaceSeparateEO);
    DDX_Control(pDX, IDC_BUTTON_SEPARATE_RAW_DATA, m_rawDataSeparate);
    DDX_Control(pDX, IDC_RADIO_T2, m_radioCtrlT2);
    DDX_Control(pDX, IDC_RADIO_T1, m_radioCtrlT1);
    DDX_Control(pDX, IDC_STATIC_KSPACE_SEPARATE, m_frmKspaceSeparate);
    DDX_Control(pDX, IDC_CHECK_PROCESS, m_chkProcess);
    DDX_Check(pDX, IDC_CHECK_PROCESS, m_chkProcessFlag);
    DDX_Radio(pDX, IDC_RADIO_T2, m_radioT2Flag);
    DDX_Check(pDX, IDC_CHECK_SHIFTPEAK, m_chkShiftPeak);
    DDX_Check(pDX, IDC_CHECK_PC_METHOD, m_chkPCMethod);
    DDX_Control(pDX, IDC_RADIO_DIFFERENCE, m_radioPCDiff);
    DDX_Control(pDX, IDC_RADIO_TS_LF, m_radioShiftPeakLineFit);
    DDX_Control(pDX, IDC_RADIO_ALLPC, m_radioPCAll);
    DDX_Control(pDX, IDC_RADIO_TS_FREQ, m_radioShiftPeakFreq);
    DDX_Control(pDX, IDC_RADIO_RELATIVE_PC, m_radioPCRelative);
	DDX_Radio(pDX, IDC_RADIO_TIME_SHIFT, m_radioShiftPeakFlag);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageEPI, CPropertyPage)
//{{AFX_MSG_MAP(CPageEPI)
    ON_BN_CLICKED(IDC_CHECK_PROCESS, OnCheckProcess)
    ON_BN_CLICKED(IDC_CHECK_SHIFTPEAK, OnCheckShiftpeak)
    ON_BN_CLICKED(IDC_CHECK_PC_METHOD, OnCheckPcMethod)
    ON_WM_CTLCOLOR()
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PROTOCOL, OnItemchangedListProtocol)
    ON_WM_SIZE()
    ON_WM_VSCROLL()
    ON_BN_CLICKED(IDC_BUTTON_SEPARATE_RAW_DATA, OnButtonSeparateRawData)
	ON_BN_CLICKED(IDC_BUTTON_MERGE_DATA, OnButtonMergeData)
	ON_BN_CLICKED(IDC_BUTTON_RECON, OnButtonRecon)
	ON_BN_CLICKED(IDC_BUTTON_SEPAARTE_KSPACE, OnButtonSepaarteKspace)
	ON_BN_CLICKED(IDC_BUTTON_CORRECT_PHASE, OnButtonCorrectPhase)
	ON_BN_CLICKED(IDC_RADIO_TS_LF, OnRadioTsLf)
	ON_BN_CLICKED(IDC_RADIO_TS_FREQ, OnRadioTsFreq)
	ON_BN_CLICKED(IDC_RADIO_TIME_SHIFT, OnRadioTimeShift)
	ON_BN_CLICKED(IDC_RADIO_DFT, OnRadioDft)
	ON_BN_CLICKED(IDC_RADIO_DIFFERENCE, OnRadioDifference)
	ON_BN_CLICKED(IDC_RADIO_ALLPC, OnRadioAllpc)
	ON_BN_CLICKED(IDC_RADIO_RELATIVE_PC, OnRadioRelativePc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageEPI message handlers

/*************************************************************
* Method:       OnInitDialog
* Description:  This function is used
* Returns:      BOOL
*************************************************************/
BOOL CPageEPI::OnInitDialog()
{
    CPropertyPage::OnInitDialog();
    CRect l_rcHideBtn;
    CRect l_rcMovedCtrl;
    //insert columns in list control
    m_RawDataList.DeleteAllItems();
    int width = 60;
    m_RawDataList.InsertColumn(0, _T("Protocol"), LVCFMT_LEFT, width);
    m_RawDataList.InsertColumn(1, _T("Sequence Name"), LVCFMT_LEFT, width + 60);
    m_RawDataList.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_SUBITEMIMAGES);
    GetWindowRect(m_rect);
    m_nScrollPos = 0;
    m_radioNonSpeeder.SetCheck(1);
    m_rawDataSeparate.ShowWindow(FALSE);
    m_frmKspaceSeparate.ShowWindow(FALSE);
    m_radioCtrlT2.ShowWindow(FALSE);
    m_radioCtrlT1.ShowWindow(FALSE);
    m_kSpaceSeparateEO.ShowWindow(FALSE);
    m_chkProcess.SetCheck(1);
    OnCheckProcess();
    m_radioCtrlT1.SetCheck(TRUE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

/*************************************************************
* Method:       OnCheckProcess
* Description:  This function is called when manual processing
is enabled
* Returns:      void
*************************************************************/
void CPageEPI::OnCheckProcess()
{
    CRect l_rcHideBtn;
    CRect l_rcMovedCtrl;

    if(BST_UNCHECKED == m_chkProcess.GetCheck()) {
        m_rawDataSeparate.ShowWindow(FALSE);
        m_frmKspaceSeparate.ShowWindow(FALSE);
        m_radioCtrlT2.ShowWindow(FALSE);
        m_radioCtrlT1.ShowWindow(FALSE);
        m_kSpaceSeparateEO.ShowWindow(FALSE);
        //        m_kSpaceDisplayEO.ShowWindow(FALSE);
        m_frmProcessSelection.GetWindowRect(&l_rcHideBtn);
        ScreenToClient(&l_rcHideBtn);
        int l_nRectTop = l_rcHideBtn.bottom + 10;
        m_frmPhaseOptions.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        int l_nRectLeft = l_rcMovedCtrl.left;
        m_frmPhaseOptions.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        m_frmPhaseOptions.RedrawWindow();
        l_nRectTop = l_nRectTop + 20;
        m_frmShiftPeak.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_frmShiftPeak.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        m_frmPCOptions.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_frmPCOptions.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        m_frmPCOptions.RedrawWindow();
        l_nRectTop = l_nRectTop + 20;
        m_chkCtrlShiftPeak.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_chkCtrlShiftPeak.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        m_chkCtrlPCMethod.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_chkCtrlPCMethod.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        l_nRectTop = l_nRectTop + 25;
        m_radioShiftPeak.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_radioShiftPeak.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        m_radioPC.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_radioPC.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        l_nRectTop = l_nRectTop + 30;
        m_radioShiftPeakLineFit.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_radioShiftPeakLineFit.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        m_radioPCDiff.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_radioPCDiff.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        l_nRectTop = l_nRectTop + 30;
        m_radioShiftPeakFreq.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_radioShiftPeakFreq.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        m_radioPCAll.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_radioPCAll.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        l_nRectTop = l_nRectTop + 30;
        m_radioPCRelative.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_radioPCRelative.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        l_nRectTop = l_nRectTop + 40;
        m_buttonCorrectPhase.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_buttonCorrectPhase.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);


        l_nRectTop = l_nRectTop + 20;
        l_nRectTop = l_nRectTop + 20;
        m_ctrlGrprecon.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_ctrlGrprecon.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        
        l_nRectTop = l_nRectTop + 20;
        m_CtrlLabStepSelection.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_CtrlLabStepSelection.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        
        m_ctrlCombStepSelection.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_ctrlCombStepSelection.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);

        l_nRectTop = l_nRectTop + 40;
        m_buttonMregeData.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_buttonMregeData.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);

        m_EditMerge.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_EditMerge.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);


        l_nRectTop = l_nRectTop + 20;
        l_nRectTop = l_nRectTop + 40;
        m_CtrlReconButton.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_CtrlReconButton.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);

        UpdateWindow();
    } else {
        m_rawDataSeparate.ShowWindow(TRUE);
        m_frmKspaceSeparate.ShowWindow(TRUE);
        m_radioCtrlT2.ShowWindow(TRUE);
        m_radioCtrlT1.ShowWindow(TRUE);
        m_kSpaceSeparateEO.ShowWindow(TRUE);
        CRect l_rcMovedCtrlPos;
        m_rawDataSeparate.GetWindowRect(&l_rcMovedCtrlPos);
        ScreenToClient(&l_rcMovedCtrlPos);
        int l_nRectTopPos = l_rcMovedCtrlPos.bottom + 15;
        m_frmKspaceSeparate.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        int l_nRectLeftPos = l_rcMovedCtrl.left;
        m_frmKspaceSeparate.SetWindowPos(&wndTop, l_nRectLeftPos, l_nRectTopPos, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        l_nRectTopPos = l_nRectTopPos + 20;
        m_radioCtrlT1.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeftPos = l_rcMovedCtrl.left;
        m_radioCtrlT1.SetWindowPos(&wndTop, l_nRectLeftPos, l_nRectTopPos, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        m_radioCtrlT2.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeftPos = l_rcMovedCtrl.left;
        m_radioCtrlT2.SetWindowPos(&wndTop, l_nRectLeftPos, l_nRectTopPos, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        l_nRectTopPos = l_nRectTopPos + 35;
        m_kSpaceSeparateEO.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeftPos = l_rcMovedCtrl.left;
        m_kSpaceSeparateEO.SetWindowPos(&wndTop, l_nRectLeftPos, l_nRectTopPos, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        m_frmKspaceSeparate.GetWindowRect(&l_rcHideBtn);
        ScreenToClient(&l_rcHideBtn);
        int l_nRectTop = l_rcHideBtn.bottom + 30;
        m_frmPhaseOptions.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        int l_nRectLeft = l_rcMovedCtrl.left;
        m_frmPhaseOptions.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        m_frmPhaseOptions.RedrawWindow();
        l_nRectTop = l_nRectTop + 20;
        m_frmShiftPeak.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_frmShiftPeak.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        m_frmShiftPeak.RedrawWindow();
        m_frmPCOptions.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_frmPCOptions.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        m_frmPCOptions.RedrawWindow();
        l_nRectTop = l_nRectTop + 20;
        m_chkCtrlShiftPeak.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_chkCtrlShiftPeak.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        m_chkCtrlPCMethod.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_chkCtrlPCMethod.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        l_nRectTop = l_nRectTop + 25;
        m_radioShiftPeak.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_radioShiftPeak.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        m_radioPC.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_radioPC.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        l_nRectTop = l_nRectTop + 30;
        m_radioShiftPeakLineFit.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_radioShiftPeakLineFit.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        m_radioPCDiff.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_radioPCDiff.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        l_nRectTop = l_nRectTop + 30;
        m_radioShiftPeakFreq.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_radioShiftPeakFreq.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        m_radioPCAll.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_radioPCAll.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        l_nRectTop = l_nRectTop + 30;
        m_radioPCRelative.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_radioPCRelative.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        l_nRectTop = l_nRectTop + 40;
        l_nRectTop = l_nRectTop + 20;
        m_buttonCorrectPhase.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_buttonCorrectPhase.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);

        l_nRectTop = l_nRectTop + 20;
        l_nRectTop = l_nRectTop + 20;
        m_ctrlGrprecon.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_ctrlGrprecon.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        
        l_nRectTop = l_nRectTop + 20;
        m_CtrlLabStepSelection.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_CtrlLabStepSelection.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        
        m_ctrlCombStepSelection.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_ctrlCombStepSelection.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);

        l_nRectTop = l_nRectTop + 40;
        m_buttonMregeData.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_buttonMregeData.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
        
        m_EditMerge.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_EditMerge.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);

        l_nRectTop = l_nRectTop + 20;
        l_nRectTop = l_nRectTop + 40;
        m_CtrlReconButton.GetWindowRect(&l_rcMovedCtrl);
        ScreenToClient(&l_rcMovedCtrl);
        l_nRectLeft = l_rcMovedCtrl.left;
        m_CtrlReconButton.SetWindowPos(&wndTop, l_nRectLeft, l_nRectTop, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);

        UpdateWindow();
    }
}

/*************************************************************
* Method:       OnButtonDisplayKspace
* Description:  This function is used
* Returns:      void
*************************************************************/
void CPageEPI::OnButtonDisplayKspace()
{
    // TODO: Add your control notification handler code here
    CString l_csDisplayStr("Display Even Odd:");
    //setDisplayTextForImageDisplay(l_csDisplayStr);
}

/*************************************************************
* Method:       OnCheckShiftpeak
* Description:  This function is used
* Returns:      void
*************************************************************/
void CPageEPI::OnCheckShiftpeak()
{
    // TODO: Add your control notification handler code here
    if(m_chkCtrlShiftPeak.GetCheck() == BST_CHECKED) {
        m_radioShiftPeak.EnableWindow(TRUE);
        m_radioShiftPeakFreq.EnableWindow(TRUE);
        m_radioShiftPeakLineFit.EnableWindow(TRUE);
    } else if(m_chkCtrlShiftPeak.GetCheck() == BST_UNCHECKED) {
        m_radioShiftPeak.EnableWindow(FALSE);
        m_radioShiftPeakFreq.EnableWindow(FALSE);
        m_radioShiftPeakLineFit.EnableWindow(FALSE);
    }
}

/*************************************************************
* Method:       OnCheckPcMethod
* Description:  This function is used
* Returns:      void
*************************************************************/
void CPageEPI::OnCheckPcMethod()
{
    // TODO: Add your control notification handler code here
    if(m_chkCtrlPCMethod.GetCheck() == BST_CHECKED) {
        m_radioPCDiff.EnableWindow(TRUE);
        m_radioPCAll.EnableWindow(TRUE);
        m_radioPC.EnableWindow(TRUE);
        m_radioPCRelative.EnableWindow(TRUE);
    }

    if(m_chkCtrlPCMethod.GetCheck() == BST_UNCHECKED) {
        m_radioPCDiff.EnableWindow(FALSE);
        m_radioPCAll.EnableWindow(FALSE);
        m_radioPC.EnableWindow(FALSE);
        m_radioPCRelative.EnableWindow(FALSE);
    }
}

/*************************************************************
* Method:       setListCtrlData
* Description:  This function is used
* Parameter:    char * * f_cListData :
* Parameter:    int f_nTotalRawNodes :
* Returns:      void
*************************************************************/
void CPageEPI::setListCtrlData(protInfo_t *&f_protData, int f_nTotalRawNodes,int f_nmergeDataFlag)
{
    //Fill the listcontrol here
    if(f_protData == NULL) {
        return;
    }

    if(NULL != m_protData) {
        delete []m_protData;
        m_protData = NULL;
    }

    m_protData = new protInfo_t[f_nTotalRawNodes];

    if(f_protData && f_nTotalRawNodes > 0) {
        m_RawDataList.DeleteAllItems();

        //m_ProcessedList.DeleteAllItems();
        for(int l_nCnt_i = 0; l_nCnt_i < f_nTotalRawNodes; l_nCnt_i++) {
            m_protData[l_nCnt_i].csProtocolName = f_protData[l_nCnt_i].csProtocolName;
            m_protData[l_nCnt_i].protocolNum = f_protData[l_nCnt_i].protocolNum;
            m_protData[l_nCnt_i].sequencename = f_protData[l_nCnt_i].sequencename;
            CString strItem(_T(""));
            CString strItem1(_T(""));
            strItem.Format(_T("%s"), f_protData[l_nCnt_i].csProtocolName);
            LVITEM lv;
            lv.iItem = l_nCnt_i;
            lv.iSubItem = 0;
            lv.pszText = strItem.GetBuffer(strItem.GetLength());
            lv.mask = LVIF_TEXT;

            if(!strItem.IsEmpty() && FALSE == IsProcessed(strItem)) {
                m_RawDataList.InsertItem(&lv);
                m_RawDataList.SetItemText(l_nCnt_i, 0, strItem);
                strItem1.Format(_T("%s"), f_protData[l_nCnt_i].sequencename);
                m_RawDataList.SetItemText(l_nCnt_i, 1, strItem1);
            } else {
                //m_ProcessedList.InsertItem(&lv);
            }

            strItem.ReleaseBuffer();
        }
    }
    if (1 != f_nmergeDataFlag)
    {
        m_ctrlCombStepSelection.ResetContent();
        ASSERT(m_ctrlCombStepSelection.GetCount() == 0);

    }
}
/*************************************************************
* Method:       IsProcessed
* Description:  This function is used
* Parameter:    CString & cs :
* Returns:      BOOL
*************************************************************/
BOOL CPageEPI::IsProcessed(CString &cs)
{
    int cnt = 0;
    cnt = cs.Find("9.");

    if(cnt < 0) {
        return FALSE;
    } else {
        return TRUE;
    }
}
/*************************************************************
* Method:       setStudyName
* Description:  This function is used
* Parameter:    const CString & f_csStudyFileName :
* Returns:      void
*************************************************************/
void CPageEPI::setStudyName(const CString & f_csStudyFileName)
{
    int l_nTotalLen = f_csStudyFileName.GetLength();
    int l_nCount = f_csStudyFileName.ReverseFind(_T('\\'));
    CString l_csStudyFileName = f_csStudyFileName.Right(l_nTotalLen - (l_nCount + 1));
    m_frmCtrlStudyName.SetWindowText(l_csStudyFileName);
}


#define     DEF_CLR_DIALOG_BACK_COLOR  RGB( 9,86,123 )

/*************************************************************
* Method:       OnCtlColor
* Description:  This function is used
* Parameter:    CDC * pDC :
* Parameter:    CWnd * pWnd :
* Parameter:    UINT nCtlColor :
* Returns:      HBRUSH
*************************************************************/
HBRUSH CPageEPI::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
    //HBRUSH hbr = CPageBase::OnCtlColor(pDC, pWnd, nCtlColor);
    return hbr;
}

/*************************************************************
* Method:       OnItemchangedListProtocol
* Description:  This function is used
* Parameter:    NMHDR * pNMHDR :
* Parameter:    LRESULT * pResult :
* Returns:      void
*************************************************************/
void CPageEPI::OnItemchangedListProtocol(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    UNREFERENCED_PARAMETER(pNMListView);
    // TODO: Add your control notification handler code here
    int l_nProtocolNo = 0;

    if((pNMListView->uChanged & LVIF_STATE)
            && (pNMListView->uNewState & LVNI_SELECTED)) {
        int rowRaw = pNMListView->iItem;
        CString s1(_T(""));

        if(rowRaw < 0) {
            return;
        }

        if(rowRaw > -1) {
            s1 = m_RawDataList.GetItemText(rowRaw, 0);
        }

        if(s1.IsEmpty() == TRUE) {
            m_RawDataList.EndWaitCursor();
            return;
        }

        m_csPathSelection = s1;
        int l_nCount = m_RawDataList.GetItemCount();

        for(int l_ncnt = 0; l_ncnt < l_nCount; l_ncnt++) {
            if(s1.Compare(m_protData[l_ncnt].csProtocolName) == 0) {
                l_nProtocolNo = m_protData[l_ncnt].protocolNum;
                break;
            }
        }
        m_ctrlCombStepSelection.ResetContent();
        ASSERT(m_ctrlCombStepSelection.GetCount() == 0);
        m_EditMerge.SetWindowText("");
        int status = (CEPIController::GetInstance())->displayImagePath(l_nProtocolNo,1);
        if(status != E_NO_ERROR) {
            CEPILogHelper::WriteToErrorLog(_T("Error in displaying the image path."), __LINE__, __FILE__);
            //error
        }
    }


    *pResult = 0;
}

/*************************************************************
* Method:       OnSize
* Description:  This function is used
* Parameter:    UINT nType :
* Parameter:    int cx :
* Parameter:    int cy :
* Returns:      void
*************************************************************/
void CPageEPI::OnSize(UINT nType, int cx, int cy)
{
    CPropertyPage::OnSize(nType, cx, cy);
    // TODO: Add your message handler code here.
    m_nCurHeight = cy;
    int nScrollMax;

    if(cy < m_rect.Height()) {
        nScrollMax = m_rect.Height() - cy;
    } else {
        nScrollMax = 0;
    }

    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_ALL; // SIF_ALL = SIF_PAGE | SIF_RANGE | SIF_POS;
    si.nMin = 0;
    si.nMax = nScrollMax;
    si.nPage = si.nMax / 10;
    si.nPos = 0;
    SetScrollInfo(SB_VERT, &si, TRUE);
}

/*************************************************************
* Method:       OnVScroll
* Description:  This function is used
* Parameter:    UINT nSBCode :
* Parameter:    UINT nPos :
* Parameter:    CScrollBar * pScrollBar :
* Returns:      void
*************************************************************/
void CPageEPI::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: Add your message handler code here and/or call default
    // TODO: Add your message handler code here and/or call default.
    int nDelta;
    int nMaxPos = m_rect.Height() - m_nCurHeight;

    if(TRUE == m_spinFlag) {
        m_spinFlag = FALSE;
        //      DisplayRawData();
        //CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
        return;
    }

    switch(nSBCode) {
        case SB_LINEDOWN:

            if(m_nScrollPos >= nMaxPos) {
                return;
            }

            nDelta = min(nMaxPos / 100, nMaxPos - m_nScrollPos);
            break;
        case SB_LINEUP:

            if(m_nScrollPos <= 0) {
                return;
            }

            nDelta = -min(nMaxPos / 100, m_nScrollPos);
            break;
        case SB_PAGEDOWN:

            if(m_nScrollPos >= nMaxPos) {
                return;
            }

            nDelta = min(nMaxPos / 10, nMaxPos - m_nScrollPos);
            break;
        case SB_THUMBPOSITION:
            nDelta = (int)nPos - m_nScrollPos;
            break;
        case SB_PAGEUP:

            if(m_nScrollPos <= 0) {
                return;
            }

            nDelta = -min(nMaxPos / 10, m_nScrollPos);
            break;
        default:
            return;
    }

    m_nScrollPos += nDelta;
    SetScrollPos(SB_VERT, m_nScrollPos, TRUE);
    ScrollWindow(0, -nDelta);
    CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}
void CPageEPI::OnButtonSeparateRawData()
{
    // TODO: Add your control notification handler code here
    if(m_csPathSelection.Compare("") == 0) {
        (CEPIController::GetInstance())->displayError(EPI_SELECT_PROTOCOL);
        //AfxMessageBox("Please select Protocol.");
        return;
    }

    CString s1;
    int l_nProtocolNo;
    s1 = m_csPathSelection;
    int l_nCount = m_RawDataList.GetItemCount();

    for(int l_ncnt = 0; l_ncnt < l_nCount; l_ncnt++) {
        if(s1.Compare(m_protData[l_ncnt].csProtocolName) == 0) {
            l_nProtocolNo = m_protData[l_ncnt].protocolNum;
            break;
        }
    }

    int l_nStatus = (CEPIController::GetInstance())->seperateTemplateAndKspace(l_nProtocolNo);

    if(E_NO_ERROR != l_nStatus) {
        return;
    }
    int nDex = 0;
    CString f_csStep("Separate Data");
    nDex = m_ctrlCombStepSelection.FindStringExact(nDex, f_csStep);
    
    if(CB_ERR == nDex) {
        m_ctrlCombStepSelection.AddString(f_csStep);
    }    
    m_ctrlCombStepSelection.SetCurSel(0);

}
int CPageEPI::ClearGUI()
{
    m_RawDataList.DeleteAllItems();
    return E_NO_ERROR;
}
void CPageEPI::OnButtonMergeData() 
{
    int l_nSelPos = m_ctrlCombStepSelection.GetCurSel();
    if (-1 == l_nSelPos)
    {
        (CEPIController::GetInstance())->displayError(EPI_PERFORM_SEPARATE);
        //AfxMessageBox("Please select Protocol.");
        return;
    }

    CString l_csSelStep;
    int l_nBuffCnt = m_ctrlCombStepSelection.GetLBTextLen(l_nSelPos);
    m_ctrlCombStepSelection.GetLBText(l_nSelPos, l_csSelStep.GetBuffer(l_nBuffCnt));
    l_csSelStep.ReleaseBuffer();
    if (l_csSelStep.Compare("") == 0)
    {
        (CEPIController::GetInstance())->displayError(EPI_SELECT_MERGE_STEP);
        //AfxMessageBox("Please select Protocol.");
        return;
    }

	// TODO: Add your control notification handler code here
	int l_dstProt = -1;
    int l_nStatus = (CEPIController::GetInstance())->mergeData(l_csSelStep,l_dstProt);

    if (E_NO_ERROR != l_nStatus)
    {
        return;
    }
	int l_nCount = m_RawDataList.GetItemCount();
	CString l_nProtocolName;
	for(int l_ncnt = 0; l_ncnt < l_nCount; l_ncnt++) {
		if(l_dstProt == m_protData[l_ncnt].protocolNum) {
			l_nProtocolName = m_protData[l_ncnt].csProtocolName;
			break;
		}
	}
	m_EditMerge.SetWindowText(l_nProtocolName);

}

void CPageEPI::OnButtonRecon() 
{
	// TODO: Add your control notification handler code here
    if(m_csPathSelection.Compare("") == 0) {
        (CEPIController::GetInstance())->displayError(EPI_SELECT_PROTOCOL);
        //AfxMessageBox("Please select Protocol.");
        return;
    }    
    CString s1;
    int l_nProtocolNo = -1;
    s1 = m_csPathSelection;
    int l_nCount = m_RawDataList.GetItemCount();
    
    for(int l_ncnt = 0; l_ncnt < l_nCount; l_ncnt++) {
        if(s1.Compare(m_protData[l_ncnt].csProtocolName) == 0) {
            l_nProtocolNo = m_protData[l_ncnt].protocolNum;
            break;
        }
    }
    int l_nStatus = (CEPIController::GetInstance())->ReRecon(l_nProtocolNo);
    if (E_NO_ERROR != l_nStatus)
    {
        return;
    }

}

void CPageEPI::OnButtonSepaarteKspace() 
{
	// TODO: Add your control notification handler code here
    int l_nTempSelFlag = -1;
    int l_nT1ChkFlag = m_radioCtrlT1.GetCheck();
    if (BST_CHECKED == l_nT1ChkFlag){
        l_nTempSelFlag = T1_SELECTION;
    }else{
        l_nTempSelFlag = T2_SELECTION;
    }
    if (-1 == l_nTempSelFlag)
    {
        (CEPIController::GetInstance())->displayError(EPI_SELECT_TEMPLATE);
        //AfxMessageBox("Please select Protocol.");
        return;
        
    }
    int l_nStatus = (CEPIController::GetInstance())->separateEvenOdd();
    if (E_NO_ERROR != l_nStatus)
    {
        return;
    }

    int nDex = 0;
    CString f_csStep("Separate Even Odd");
    nDex = m_ctrlCombStepSelection.FindStringExact(nDex, f_csStep);
    
    if(CB_ERR == nDex) {
        m_ctrlCombStepSelection.AddString(f_csStep);
    }    
    m_ctrlCombStepSelection.SelectString(0,f_csStep);
}

void CPageEPI::OnButtonCorrectPhase() 
{
	// TODO: Add your control notification handler code here
    int l_nPCShiftPeakMethodFlag = -1;
    int l_nTempSelFlag = -1;
    int l_nShiftPeakChkFlag = -1;
    int l_nPCSelectionFlag = -1;
    if(BST_CHECKED == m_chkCtrlShiftPeak.GetCheck()){
        l_nShiftPeakChkFlag = m_radioShiftPeak.GetCheck();
        if (BST_CHECKED == l_nShiftPeakChkFlag)
        {
            l_nPCShiftPeakMethodFlag = TIME_SHIFT;
            l_nPCSelectionFlag = 1;
        }
        l_nShiftPeakChkFlag = m_radioShiftPeakLineFit.GetCheck();
        if (BST_CHECKED == l_nShiftPeakChkFlag)
        {
            l_nPCShiftPeakMethodFlag = TIME_SHIFT_LINE_FIT;
            l_nPCSelectionFlag = 1;
        }

    }
    if (1 != l_nPCSelectionFlag)
    {
        (CEPIController::GetInstance())->displayError(EPI_SELECT_PC_METHOD);
        return;
    }
    int l_nT1ChkFlag = m_radioCtrlT1.GetCheck();
    if (BST_CHECKED == l_nT1ChkFlag){
        l_nTempSelFlag = T1_SELECTION;
    }else{
        l_nTempSelFlag = T2_SELECTION;
    }

    int l_nStatus = (CEPIController::GetInstance())->correctPhase(l_nPCShiftPeakMethodFlag,
                                                                  l_nTempSelFlag);
    if (E_NO_ERROR != l_nStatus)
    {
        return;
    }
    
    int nDex = 0;
    CString f_csStep("Phase Correction");
    nDex = m_ctrlCombStepSelection.FindStringExact(nDex, f_csStep);
    
    if(CB_ERR == nDex) {
        m_ctrlCombStepSelection.AddString(f_csStep);
    }    
    m_ctrlCombStepSelection.SelectString(0,f_csStep);

	
}

void CPageEPI::OnRadioTsLf() 
{
	// TODO: Add your control notification handler code here
    if (BST_CHECKED == m_radioShiftPeak.GetCheck())
    {
        m_radioShiftPeak.SetCheck(FALSE);
    }
	
}

void CPageEPI::OnRadioTsFreq() 
{
	// TODO: Add your control notification handler code here
    if (BST_CHECKED == m_radioShiftPeak.GetCheck())
    {
        m_radioShiftPeak.SetCheck(FALSE);
    }

}

void CPageEPI::OnRadioTimeShift() 
{
	// TODO: Add your control notification handler code here
    if (BST_CHECKED == m_radioShiftPeakLineFit.GetCheck())
    {
        m_radioShiftPeakLineFit.SetCheck(FALSE);
    }
    if (BST_CHECKED == m_radioShiftPeakFreq.GetCheck())
    {
        m_radioShiftPeakFreq.SetCheck(FALSE);
    }

}

void CPageEPI::OnRadioDft() 
{
	// TODO: Add your control notification handler code here
    if (BST_CHECKED == m_radioPCDiff.GetCheck())
    {
        m_radioPCDiff.SetCheck(FALSE);
    }
    if (BST_CHECKED == m_radioPCAll.GetCheck())
    {
        m_radioPCAll.SetCheck(FALSE);
    }
    if (BST_CHECKED == m_radioPCRelative.GetCheck())
    {
        m_radioPCRelative.SetCheck(FALSE);
    }


}

void CPageEPI::OnRadioDifference() 
{
	// TODO: Add your control notification handler code here    
    if (BST_CHECKED == m_radioPC.GetCheck())
    {
        m_radioPC.SetCheck(FALSE);
    }
}

void CPageEPI::OnRadioAllpc() 
{
	// TODO: Add your control notification handler code here
    if (BST_CHECKED == m_radioPC.GetCheck())
    {
        m_radioPC.SetCheck(FALSE);
    }
}

void CPageEPI::OnRadioRelativePc() 
{
	// TODO: Add your control notification handler code here
    if (BST_CHECKED == m_radioPC.GetCheck())
    {
        m_radioPC.SetCheck(FALSE);
    }
}

// ApplyTi.cpp : implementation file
//

#include "stdafx.h"
#include "tiprep.h"
#include "ApplyTi.h"
#include "TiPrepView.h"
#include "TiPrepLogHelper.h"
#include "TiPrepController.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define  ID_SPIN_TIVAL 123456
/////////////////////////////////////////////////////////////////////////////
// CApplyTi dialog


CApplyTi::CApplyTi(CWnd* pParent /*=NULL*/)
	: CPopupDlg(CApplyTi::IDD, pParent),m_csStudyPath(_T("")),m_csOriginalTiValue(_T("")),m_ModeUsed(1)
{
	//{{AFX_DATA_INIT(CApplyTi)
	//}}AFX_DATA_INIT
}

CApplyTi::CApplyTi(CApplyTi& f_ApplyTiDlg): CPopupDlg(CApplyTi::IDD)
{
    
}

void CApplyTi::DoDataExchange(CDataExchange* pDX)
{
	CPopupDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CApplyTi)
	DDX_Control(pDX, IDC_STATIC_SEL_UNSCAN_PROT, m_SelUnscanProtLabel);
	DDX_Control(pDX, IDC_STATIC_ORIGINAL_TI, m_OriginalTiLabel);
	DDX_Control(pDX, IDC_STATIC_NEW_TI, m_NewTiLabel);
	DDX_Control(pDX, IDC_STATIC_MODE_SETTI, m_SetTiModeLabel);
	DDX_Control(pDX, IDC_STATIC_AVGROI, m_AvgLabel);
	DDX_Control(pDX, IDC_LIST_UNSCAN_PROTOCOL, m_UnScanProtList);
	DDX_Control(pDX, IDC_EDIT_VFTIVALUE, m_VFTiEdit);
	DDX_Control(pDX, IDC_EDIT_ROI3_VAL, m_ROI3Edit);
	DDX_Control(pDX, IDC_EDIT_ROI2_VAL, m_ROI2Edit);
	DDX_Control(pDX, IDC_EDIT_ROI1VAL, m_ROI1Edit);
	DDX_Control(pDX, IDC_EDIT_ORIGINAL_TI, m_OriginalTiEdit);
	DDX_Control(pDX, IDC_EDIT_OFFSET, m_OffsetTiEdit);
	DDX_Control(pDX, IDC_EDIT_NEW_TI, m_NewTiEdit);
	DDX_Control(pDX, IDC_EDIT_INPUTTI, m_InputTiEdit);
	DDX_Control(pDX, IDC_EDIT_AVG_VAL, m_AverageTiEdit);
	DDX_Control(pDX, IDC_CHECKROI3, m_ROI3Check);
	DDX_Control(pDX, IDC_CHECK_ROI2, m_ROI2Check);
	DDX_Control(pDX, IDC_CHECK_ROI1, m_ROI1Check);
	DDX_Control(pDX, IDC_CHECK_OFFSET, m_OffsetCheck);
	DDX_Control(pDX, IDC_STATIC_TI_VALUES, m_TiValuesFrame);
	DDX_Control(pDX, IDC_STATIC_SET_TI_VALUE, m_SetTiValueFrame);
	DDX_Control(pDX, IDC_STATIC_PROTOCOL, m_FrameUnscannedProtocolsFrame);
	DDX_Control(pDX, IDC_STATIC_MODES, m_SETTIModeFrame);
	DDX_Control(pDX, IDC_STATIC_ROIAVG, m_ROIAVGFrame);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, ID_APPLY, m_Apply);
    DDX_Control(pDX, IDC_RADIO_VF_TIVAL, m_VFTiModeRadio);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CApplyTi, CPopupDlg)
	//{{AFX_MSG_MAP(CApplyTi)
	ON_BN_CLICKED(ID_APPLY, OnApply)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_CLICK, IDC_LIST_UNSCAN_PROTOCOL, OnClickListUnscanProtocol)
	ON_BN_CLICKED(IDC_RADIO_VF_TIVAL, OnRadioVfTival)
	ON_BN_CLICKED(IDC_RADIO_INPUTTI, OnRadioInputti)
	ON_BN_CLICKED(IDC_RADIO_CALCMODE, OnRadioCalcmode)
	ON_BN_CLICKED(IDC_CHECK_ROI1, OnCheckRoi1)
	ON_BN_CLICKED(IDC_CHECK_ROI2, OnCheckRoi2)
	ON_BN_CLICKED(IDC_CHECKROI3, OnCheckroi3)
	ON_BN_CLICKED(IDC_CHECK_OFFSET, OnCheckOffset)
    ON_MESSAGE(WM_CMD_SPIN, UpdateControlsfromSpin)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CApplyTi message handlers

void CApplyTi::OnCancel() 
{
	// TODO: Add extra cleanup here
	CTiPrepView::GetView()->UnRegisterWindowZOrder(m_hWnd);
	CPopupDlg::OnCancel();
}

void CApplyTi::OnApply() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString values;
	int l_nRow = -1;
    l_nRow = m_UnScanProtList.GetSelectionMark();
	if (l_nRow < 0)  {
		//Display error message
		return;
	}
	float ti = 0;
	int protocol_index = 0;
	CString csvalue;
	m_NewTiEdit.GetWindowText(csvalue);
	 char val[100];

        wcstombs(val, csvalue, 100);
	ti = atoi(val);//m_nTiValueVector.at(l_nRow);
	ti /= 1000; //while updating convert to seconds
	protocol_index = m_nProtIndexVector.at(l_nRow);

	values.Format(_T("%s=%d,%s=%0.2f"),_T("Protocol index"),protocol_index,_T("Ti value"),ti);
	 MessageBox(NULL,values,NULL);
   	CTiPrepController::GetInstance()->OnApplyTI(protocol_index,ti);
}

BOOL CApplyTi::OnInitDialog() 
{
	CPopupDlg::OnInitDialog();
	
    InitializeControls();
    ClearUnscannedProtocolList();
    FillUnScannedProtocolList();
    UpdateControls();

    CTiPrepView::GetView()->AddZOrderLayer();
    CTiPrepView::GetView()->RegisterWindowZOrder(m_hWnd);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CApplyTi::InitializeControls()
{
    WINDOW_COLORS_t l_window_color;
    GetWindowColors(l_window_color);
    m_list_brush.CreateSolidBrush(l_window_color.crClientColor);

    m_Apply.SetPrimaryTextFont(_T("Arial"), FONT_SIZE, true);
    m_Apply.SetButtonType(TUSBUTTON_ROUND_ALL);    
    m_Cancel.SetPrimaryTextFont(_T("Arial"), FONT_SIZE, true);
    m_Cancel.SetButtonType(TUSBUTTON_ROUND_ALL);

    m_SelUnscanProtLabel.SetFont(&m_font, true);
    m_OriginalTiLabel.SetFont(&m_font, true);
    m_NewTiLabel.SetFont(&m_font, true);
    m_SetTiModeLabel.SetFont(&m_font, true);
    m_AvgLabel.SetFont(&m_font, true);

    m_VFTiEdit.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_VFTiEdit.SetReadOnly(FALSE);
    m_InputTiEdit.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_InputTiEdit.SetReadOnly(FALSE);
    m_OffsetTiEdit.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_OffsetTiEdit.SetReadOnly(FALSE);
    m_AverageTiEdit.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_AverageTiEdit.SetReadOnly(FALSE);    


    m_ROI3Edit.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_ROI3Edit.SetReadOnly(TRUE);
    m_ROI2Edit.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_ROI2Edit.SetReadOnly(TRUE);
    m_ROI1Edit.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_ROI1Edit.SetReadOnly(TRUE);
    m_OriginalTiEdit.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_OriginalTiEdit.SetReadOnly(TRUE);
    m_NewTiEdit.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_NewTiEdit.SetReadOnly(TRUE);

    m_ROI3Check.SetBkColor(BKCOLOR);
    m_ROI3Check.SetFont(FONT, TEXT_SIZE);
    m_ROI3Check.SetCheck(FALSE);
    m_ROI2Check.SetBkColor(BKCOLOR);
    m_ROI2Check.SetFont(FONT, TEXT_SIZE);
    m_ROI2Check.SetCheck(FALSE);
    m_ROI1Check.SetBkColor(BKCOLOR);
    m_ROI1Check.SetFont(FONT, TEXT_SIZE);
    m_ROI1Check.SetCheck(TRUE);
    m_OffsetCheck.SetBkColor(BKCOLOR);
    m_OffsetCheck.SetFont(FONT, TEXT_SIZE);
    m_OffsetCheck.SetCheck(FALSE);

    m_VFTiModeRadio.SetBkColor(BKCOLOR);
    m_VFTiModeRadio.SetFont(FONT, TEXT_SIZE);
    m_VFTiModeRadio.SetCheck(TRUE);

    m_UnScanProtList.SetFont(_T("Arial"), TEXT_SIZE, true);
    //insert columns in list control
    m_UnScanProtList.InsertColumn(0, _T("No"), LVCFMT_LEFT, 55);
    m_UnScanProtList.InsertColumn(1, _T("TI Value"), LVCFMT_LEFT, 60);
    m_UnScanProtList.InsertColumn(2, _T("Scan ID"), LVCFMT_LEFT, 100);
    m_UnScanProtList.InsertColumn(3, _T("Protocol"), LVCFMT_LEFT, 90);

   // m_UnScanProtList.SetColumnRange(0, 150, 150);
    m_UnScanProtList.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT|LVS_EX_INFOTIP);

    CRect rect;
    if(m_InputTiEdit.GetSafeHwnd()){
        m_InputTiEdit.GetWindowRect(&rect);
        ScreenToClient(&rect);
        if(!m_SpinInputTi.Create(WS_CHILD | WS_VISIBLE | UDS_ALIGNRIGHT | // dwStyle
              UDS_SETBUDDYINT|UDS_AUTOBUDDY,
            rect,                   // rect
            this,       // CWnd* pParentWnd
            IDC_SPIN_INPUT_TI)) {      // UINT  nID
            return ;
        }
    }
    m_SpinInputTi.SetRange(0,1000);
    m_SpinInputTi.SetPos (100);

    SetRibbonHeight(60);
}

HBRUSH CApplyTi::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CApplyTi::FillUnScannedProtocolList()
{
    long l_nImgSel = 0;
    int l_nMsgCode = 0;
    int l_nTotalProtocolCount = 0;
    int l_nTotalSeriesCount = 0;
    
       int  l_nstatus = (CTiPrepView::GetView())->TiCopyHandler(
                                                            m_nTiValueVector,
                                                            m_csProtSequenceVector,
                                                            m_nProtIndexVector, 
                                                            m_csProtocolNumberVector,                                                            
                                                            l_nTotalProtocolCount, 
                                                            l_nTotalSeriesCount,
                                                            m_csStudyPath);
        if(E_NO_ERROR != l_nstatus){
            CString l_cserrmsg = (_T("TiCopyHandler Failed."));
            CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);     
            m_nTotalUnScannedProt = 0;
            m_nTotalUnScannedSeries = 0;
        }
        else{
            m_nTotalUnScannedProt = l_nTotalProtocolCount;
            m_nTotalUnScannedSeries = l_nTotalSeriesCount;
        }
        
        if(m_nTotalUnScannedProt > 0) {
            CString strItem(_T(""));
            LVITEM lv;
            
            for(int l_nCnt_i = 0; l_nCnt_i < m_nTotalUnScannedProt; l_nCnt_i++) {
                strItem.Format(_T("%d"), m_csProtocolNumberVector.at(l_nCnt_i));
                lv.iItem = l_nCnt_i;
                lv.iSubItem = 0;//as only one column present
                lv.pszText = strItem.GetBuffer(strItem.GetLength());
                lv.mask = LVIF_TEXT;
                m_UnScanProtList.InsertItem(&lv);
                strItem.ReleaseBuffer();

                CString l_csTiVal(_T(""));
                if(m_nTiValueVector.at(l_nCnt_i) <= 0){
                    l_csTiVal = _T(" --- ");
                }else{
                    l_csTiVal.Format(_T("%d"),m_nTiValueVector.at(l_nCnt_i));
                }
                lv.iSubItem = 1;
                lv.pszText = l_csTiVal.GetBuffer(l_csTiVal.GetLength());
                m_UnScanProtList.SetItem(&lv);
                l_csTiVal.ReleaseBuffer();

                CString l_csSequence(_T(""));
                l_csSequence.Format(_T("%s"),m_csProtSequenceVector.at(l_nCnt_i));
                lv.iSubItem = 2;
                lv.pszText = l_csSequence.GetBuffer(l_csSequence.GetLength());
                m_UnScanProtList.SetItem(&lv);
                l_csSequence.ReleaseBuffer();

                CString l_csTreeIndex(_T(""));
                l_csTreeIndex.Format(_T("1.%d"),m_nProtIndexVector.at(l_nCnt_i));
                lv.iSubItem = 3;
                lv.pszText = l_csTreeIndex.GetBuffer(l_csTreeIndex.GetLength());
                m_UnScanProtList.SetItem(&lv);
                l_csTreeIndex.ReleaseBuffer();

            }
        }
}

void CApplyTi::OnClickListUnscanProtocol(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
    int l_nRow = -1;
    l_nRow = m_UnScanProtList.GetSelectionMark();
    if(l_nRow >= 0){
        CString l_csProtocolNum = m_UnScanProtList.GetItemText(l_nRow, 0);
        CString comparestring(_T(""));
        comparestring.Format(_T("%d"),m_csProtocolNumberVector.at(l_nRow));//1000,2000
        if(l_csProtocolNum.CompareNoCase(comparestring) == 0){
            int l_nTI = 0;
            (CTiPrepView::GetView())->getVfcTI(m_csStudyPath,m_nProtIndexVector.at(l_nRow),&l_nTI);
                m_csOriginalTiValue.Format(_T("%d"),l_nTI);
                UpdateControls();
        }
    }
	*pResult = 0;
}

void CApplyTi::UpdateControls()
{
    double l_dNewTiValue = 0.0;

    m_OriginalTiEdit.SetWindowText(m_csOriginalTiValue);
    m_VFTiEdit.SetWindowText(m_csOriginalTiValue);

    //=====================================================================================================
    double *l_dTIValues = NULL;
    l_dTIValues = (CTiPrepView::GetView())->GetCurrentROIsTIValues();
    BOOL l_roiCheckStatus[MAX_NUM_ROI];
    l_roiCheckStatus[ROI_1] = m_ROI1Check.GetCheck();
    l_roiCheckStatus[ROI_2] = m_ROI2Check.GetCheck();
    l_roiCheckStatus[ROI_3] = m_ROI3Check.GetCheck();
    double l_dmeanVal_TI = 0.0;
    int l_roiDisp = 0;
    for(int l_nroi = 0 ; l_nroi < MAX_NUM_ROI ; l_nroi++) {        
        if(l_roiCheckStatus[l_nroi] && l_dTIValues) {            
                    l_roiDisp++;
                    l_dmeanVal_TI += l_dTIValues[l_nroi] ;
        }
    }    
    if(0 != l_roiDisp) {
            l_dmeanVal_TI = l_dmeanVal_TI / l_roiDisp ;
    }
    CString l_csTI(_T(" ----- "));
    if(l_roiCheckStatus[ROI_1] && l_dTIValues){
        l_csTI.Format(_T("%0.1lf"),l_dTIValues[0]);
        m_ROI1Edit.SetWindowText(l_csTI);
    }else{
        l_csTI = _T(" ----- ");
        m_ROI1Edit.SetWindowText(l_csTI);
    }
    if(l_roiCheckStatus[ROI_2] && l_dTIValues){
        
        l_csTI.Format(_T("%0.1lf"),l_dTIValues[1]);
        m_ROI2Edit.SetWindowText(l_csTI);
    }else{
        l_csTI = _T(" ----- ");
        m_ROI2Edit.SetWindowText(l_csTI);
    }
    if(l_roiCheckStatus[ROI_3] && l_dTIValues){
        
        l_csTI.Format(_T("%0.1lf"),l_dTIValues[2]);
        m_ROI3Edit.SetWindowText(l_csTI);
    }else{
        l_csTI = _T(" ----- ");
        m_ROI3Edit.SetWindowText(l_csTI);
    }
    if(l_roiCheckStatus[ROI_1] || l_roiCheckStatus[ROI_2] || l_roiCheckStatus[ROI_3]){
        l_csTI.Format(_T("%0.1lf"),l_dmeanVal_TI);
        m_AverageTiEdit.SetWindowText(l_csTI);
    }else{
        l_csTI = _T(" ----- ");
        m_AverageTiEdit.SetWindowText(l_csTI);
    }
   
    if(3 == m_ModeUsed){
        l_dNewTiValue = l_dNewTiValue + l_dmeanVal_TI;
    }
    //=====================================================================================================

    double l_dOffsetValue = 0.0;    
    if(m_OffsetCheck.GetCheck()){
        CString l_csOffset(_T(""));
        m_OffsetTiEdit.GetWindowText(l_csOffset);
        if(FALSE == ValidateOffset(l_csOffset)){
            l_dOffsetValue = 0.0;
        }else{
            char l_cpC[150];
            wcstombs(l_cpC, l_csOffset, 100);
            l_dOffsetValue = atof(l_cpC);
        }

        l_dNewTiValue = l_dNewTiValue + l_dOffsetValue;
    }
    //=====================================================================================================   

    double l_dVfValue = 0.0;
    if(1 == m_ModeUsed){
        CString l_csVf(_T(""));
        m_VFTiEdit.GetWindowText(l_csVf);
        if(FALSE == ValidateOffset(l_csVf)){            
            l_dVfValue = 0.0;
        }else{
            char l_cpC[150];
            wcstombs(l_cpC, l_csVf, 100);
            l_dVfValue = atof(l_cpC);
        }
        l_dNewTiValue = l_dNewTiValue + l_dVfValue;
    }
    //=====================================================================================================

    double l_dInputValue = 0.0;
    if(2 == m_ModeUsed){        
        CString l_csInput(_T(""));
        m_InputTiEdit.GetWindowText(l_csInput);        
        char l_cpC[150];
        wcstombs(l_cpC, l_csInput, 100);
        l_dInputValue = atof(l_cpC);

        l_dNewTiValue = l_dNewTiValue + l_dInputValue;
    }
    //=====================================================================================================
    CString l_csNewTiValue(_T(""));
    l_csNewTiValue.Format(_T("%0.1lf"),l_dNewTiValue);
    m_NewTiEdit.SetWindowText(l_csNewTiValue);
}

BOOL CApplyTi::ValidateOffset(CString f_csOffsetNo)
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
        return FALSE;
    }
}

void CApplyTi::OnRadioVfTival() 
{
	// TODO: Add your control notification handler code here
    m_ModeUsed = 1;
	UpdateControls();
}

void CApplyTi::OnRadioInputti() 
{
	// TODO: Add your control notification handler code here
    m_ModeUsed = 2;
    UpdateControls();
}

void CApplyTi::OnRadioCalcmode() 
{
	// TODO: Add your control notification handler code here
    m_ModeUsed = 3;
    UpdateControls();
}

void CApplyTi::OnCheckRoi1() 
{
	// TODO: Add your control notification handler code here

	UpdateControls();
}

void CApplyTi::OnCheckRoi2() 
{
	// TODO: Add your control notification handler code here
	UpdateControls();
}

void CApplyTi::OnCheckroi3() 
{
	// TODO: Add your control notification handler code here
    UpdateControls();
}

void CApplyTi::OnCheckOffset() 
{
	// TODO: Add your control notification handler code here
    UpdateControls();

}

BOOL CApplyTi::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
    if (
        pMsg->message == WM_KEYDOWN 
        &&
        pMsg->wParam == VK_RETURN 
       )
    {
        UpdateControls();
    }
	return CPopupDlg::PreTranslateMessage(pMsg);
}

LRESULT CApplyTi::UpdateControlsfromSpin( WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(wparam);
    UNREFERENCED_PARAMETER(lparam);
    UpdateControls();
    return 1;
}

void CApplyTi::ClearUnscannedProtocolList()
{
    m_UnScanProtList.DeleteAllItems();
}

void CApplyTi::ClearData()
{
    m_csProtocolNumberVector.clear();
    m_nProtIndexVector.clear();
    m_nTiValueVector.clear();
    m_csProtSequenceVector.clear();
    m_nTotalUnScannedProt = 0;
    m_nTotalUnScannedSeries = 0;
    m_csStudyPath.Empty();
    m_csOriginalTiValue.Empty();
    m_ModeUsed = 3;
}

void CApplyTi::RefreshGUI()
{
    ClearUnscannedProtocolList();
    ClearData();
    FillUnScannedProtocolList();
}

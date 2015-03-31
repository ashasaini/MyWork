// ROICopyDlg.cpp : implementation file
//
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#include "stdafx.h"
#include "tiprep.h"
#include "ROICopyDlg.h"
#include "TiPrepView.h"
#include "LocalDBMgr.h"

#include "TiPrepTrace.h"
#include "TiPrepLogHelper.h"
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//****************************Method Header***************************
//Method Name   : CROICopyDlg
//Purpose       : Constructor
//********************************************************************
/**
*   A constructor.
*   This function is used for initialization of class variables.
*/
CROICopyDlg::CROICopyDlg(CWnd* pParent /*=NULL*/)
: CPopupDlg(CROICopyDlg::IDD, pParent), m_nTotalScannedProt(0), m_nTotalScannedSeries(0)
{
    //{{AFX_DATA_INIT(CROICopyDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_hIcon = AfxGetApp()->LoadIcon(IDI_TIPREP_ICON2); //Inside the dialog's constructor.
}
CROICopyDlg& CROICopyDlg::operator = (CROICopyDlg& f_CopyRoiDlg_ptr)
{
    /*TIPREP_TRACE(CROICopyDlg::operator);*/
    
    if(this == &f_CopyRoiDlg_ptr) {
        return(*this);
    }
    m_hIcon = AfxGetApp()->LoadIcon(IDI_TIPREP_ICON2);
    return (*this);
}

CROICopyDlg::CROICopyDlg(CROICopyDlg & f_roicopydlg) : CPopupDlg(CROICopyDlg::IDD)
{
	m_hIcon = f_roicopydlg.m_hIcon;
	m_nTotalScannedProt = f_roicopydlg.m_nTotalScannedProt;
	m_nTotalScannedSeries = f_roicopydlg.m_nTotalScannedSeries;
}

/*************************************************************
* Method:       DoDataExchange
* Description:  This function is used
* Parameter:    CDataExchange * pDX
* Returns:      void
*************************************************************/
void CROICopyDlg::DoDataExchange(CDataExchange* pDX)
{
    CPopupDlg::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CROICopyDlg)
	DDX_Control(pDX, IDC_STATIC_SOURCE_PROT, m_label_source_prot);
	DDX_Control(pDX, IDC_EDIT_SOURCE_PROT, m_edit_source_prot);
    DDX_Control(pDX, IDC_LIST_SCANNED_PROT_LIST, m_listctrl_scannedProtocol);
    DDX_Control(pDX, IDC_BUTTON_ROICOPY, m_button_copy);
    DDX_Control(pDX, IDCANCEL, m_button_exit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CROICopyDlg, CPopupDlg)
//{{AFX_MSG_MAP(CROICopyDlg)
ON_BN_CLICKED(IDC_BUTTON_ROICOPY, OnButtonRoicopy)
ON_BN_CLICKED(IDCANCEL, OnCancel)
ON_WM_SHOWWINDOW()
ON_WM_CTLCOLOR()

//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CROICopyDlg message handlers

/*************************************************************
* Method:       OnInitDialog
* Description:  This function is used
* Returns:      BOOL
*************************************************************/
BOOL CROICopyDlg::OnInitDialog()
{
    TIPREP_TRACE(CROICopyDlg::OnInitDialog,MRTraceMarker3);

    CPopupDlg::OnInitDialog();
    
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    int l_nTotalProtocolCount = 0;
    int l_nTotalSeriesCount = 0;
    int l_nstatus = E_NO_ERROR;
    vector <CString> l_csSeriesLoidVector;
    vector <long> l_csProtocolNumberVector;
    vector <long> l_csSeriesNumberVector;
    WINDOW_COLORS_t l_window_color;
    GetWindowColors(l_window_color);

    m_list_brush.CreateSolidBrush(l_window_color.crClientColor);

    AfxGetApp()->DoWaitCursor(1);


    SetRibbonHeight(0);
    m_listctrl_scannedProtocol.SetFont(_T("Arial"), TEXT_SIZE, true);
    //insert columns in list control
    m_listctrl_scannedProtocol.InsertColumn(0, _T("Target Protocol"), LVCFMT_LEFT, 150);
    m_listctrl_scannedProtocol.SetColumnRange(0, 150, 150);
    m_button_copy.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
    m_button_exit.SetPrimaryTextFont(_T("Arial"), TEXT_SIZE, true);
    m_edit_source_prot.SetFont(_T("Arial"), TEXT_SIZE, true);
    m_edit_source_prot.SetReadOnly(TRUE);
    m_listctrl_scannedProtocol.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);
    CFont  l_font;
    m_label_source_prot.SetFont(&l_font, true);

    long l_nImgSel = 0;
    int l_nMsgCode = 0;
    
    l_nstatus = (CTiPrepView::GetView())->getNumImgSelectedISel(l_nImgSel);
    if (E_NO_ERROR != l_nstatus || 0 >= l_nImgSel)
    {
        l_nMsgCode = E_APPLY_IMAGE_NOT_PROPER;
    }else{
        CString csSourceProt (_T(""));
        l_nstatus = (CTiPrepView::GetView())->RoiCopyHandler(l_csSeriesLoidVector, l_csSeriesNumberVector, l_csProtocolNumberVector, csSourceProt,l_nTotalProtocolCount, l_nTotalSeriesCount);
        if(E_NO_ERROR != l_nstatus){
            CString l_cserrmsg = (_T("OnInitDialog() : RoiCopyHandler Failed."));
            CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);     
            m_nTotalScannedProt = 0;
            m_nTotalScannedSeries = 0;
        }
        else{
            m_nTotalScannedProt = l_nTotalProtocolCount;
            m_nTotalScannedSeries = l_nTotalSeriesCount;
        }
        
        if(m_nTotalScannedProt > 0) {
            CString strItem = EMPTY_STRING;
            LVITEM lv;
            
            for(int l_nCnt_i = 0; l_nCnt_i < m_nTotalScannedProt; l_nCnt_i++) {
                strItem.Format(_T("%d"), l_csProtocolNumberVector.at(l_nCnt_i));
                lv.iItem = l_nCnt_i;
                lv.iSubItem = 0;//as only one column present
                lv.pszText = strItem.GetBuffer(strItem.GetLength());
                lv.mask = LVIF_TEXT;
                m_listctrl_scannedProtocol.InsertItem(&lv);
            }
            m_csSeriesLoid = l_csSeriesLoidVector;
            m_nsSeriesNumberArray = l_csSeriesNumberVector;
        }
        m_edit_source_prot.SetWindowText(csSourceProt);
        l_csSeriesLoidVector.clear();
        l_csSeriesNumberVector.clear();
        l_csProtocolNumberVector.clear();
    }
    CTiPrepView::GetView()->AddZOrderLayer();
    
    if(FALSE == (CTiPrepView::GetView())->RegisterWindowZOrder(m_hWnd)){
        CString l_cserrmsg = (_T("OnInitDialog() : RegisterWindowZOrder Failed."));
        CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
        //Dont Return from here
    }
    AfxGetApp()->DoWaitCursor(-1);
    return TRUE;
}

/*************************************************************
* Method:       OnCancel
* Description:  This function is used
* Returns:      void
*************************************************************/
void CROICopyDlg::OnCancel()
{
    TIPREP_TRACE(CROICopyDlg::OnCancel,MRTraceMarker3);    
    
    if(FALSE == (CTiPrepView::GetView())->UnRegisterWindowZOrder(m_hWnd)){
        CString l_cserrmsg = (_T("OnCancel() : UnRegisterWindowZOrder Failed."));
        CTiPrepLogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
    }
    
    CPopupDlg::OnCancel();
}

/*************************************************************
* Method:       OnButtonRoicopy
* Description:  This function is used
* Returns:      void
*************************************************************/
void CROICopyDlg::OnButtonRoicopy()
{
    TIPREP_TRACE(CROICopyDlg::OnButtonRoicopy,MRTraceMarker3);
    
    try{
        int l_nRow = -1;
        int l_nstatus = E_NO_ERROR;
        long l_SeriesNumber = -1;
        int l_ni = 0;
        
        if(m_nTotalScannedProt>0 && m_nTotalScannedSeries>0){
            if(0 == m_csSeriesLoid.size()) {
                int msgCode = I_PROPER_IMG;
                (CTiPrepView::GetView())->DisplayErr(msgCode, COPY_ROI_DLG);
                return;
            }
            
            l_nRow = m_listctrl_scannedProtocol.GetSelectionMark();
            
            if(l_nRow < 0) {
                int msgCode = E_SELECT_TARGET_PROTOCOL;
                (CTiPrepView::GetView())->DisplayErr(msgCode, COPY_ROI_DLG);
                return;
            }
            
            m_listctrl_scannedProtocol.BeginWaitCursor();
            CString s1 = m_listctrl_scannedProtocol.GetItemText(l_nRow, 0);
            l_SeriesNumber = _ttol(s1);
            
            for(l_ni = 0; l_ni < m_nTotalScannedSeries; l_ni++) {
                if(m_nsSeriesNumberArray.at(l_ni) == l_SeriesNumber) {
                    break;
                }
            }
            if(l_ni == m_nTotalScannedSeries){
                int msgCode = E_SERIES_NOT_FOUND_IN_LIST;
                (CTiPrepView::GetView())->DisplayErr(msgCode, COPY_ROI_DLG);
                m_listctrl_scannedProtocol.EndWaitCursor();
                return;
            }
            
            l_nstatus = (CTiPrepView::GetView())->OnROICopy(m_csSeriesLoid.at(l_ni));
            if(E_NO_ERROR != l_nstatus){
                int msgCode = E_COPY_ROI_FUNCTIONALITY_FAILED;//l_nstatus
                (CTiPrepView::GetView())->DisplayErr(msgCode, COPY_ROI_DLG);
                m_listctrl_scannedProtocol.EndWaitCursor();
                return;
            }
            m_listctrl_scannedProtocol.EndWaitCursor();
        }
    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog
            (_T("Exception occurred in ROICopy Dialog"), __LINE__, __FILE__);
    }
}

//****************************Method Header***************************
//Method Name   : CROICopyDlg
//Purpose       : Destructor
//********************************************************************
CROICopyDlg::~CROICopyDlg()
{
    m_nsSeriesNumberArray.clear();
    m_csSeriesLoid.clear();

	try{
		m_list_brush.DeleteObject();
	}catch(...)
	{
		CString l_csmsg(_T("Exception occurred in Copy ROI dialog"));
        CTiPrepLogHelper::WriteToErrorLog(l_csmsg, __LINE__, __FILE__);
		
	}
}

/*************************************************************
* Method:    	OnShowWindow
* Description: 	This function is used 
* Parameter: 	BOOL bShow :
* Parameter: 	UINT nStatus :
* Returns:   	void
*************************************************************/
void CROICopyDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CPopupDlg::OnShowWindow(bShow, nStatus);
    if(m_nTotalScannedProt <= 0 || m_nTotalScannedSeries <=0){
        //int msgCode = E_GETTING_SCANNED_PROTOCOL_LIST_FAILED;//l_nstatus
        m_listctrl_scannedProtocol.EndWaitCursor();
        return;
    }
}

/*************************************************************
* Method:    	OnCtlColor
* Description: 	This function is used 
* Parameter: 	CDC * pDC :
* Parameter: 	CWnd * pWnd :
* Parameter: 	UINT nCtlColor :
* Returns:   	HBRUSH
*************************************************************/
HBRUSH CROICopyDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
//    TIPREP_TRACE(CTiPrepDlg::OnCtlColor,MRTraceMarker9);
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

///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////

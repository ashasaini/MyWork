// PlanDuplicatorDlg.cpp : implementation file
//
/*****************************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: PlanDuplicator
    Author: PATNI-PlanDuplicator Team
*****************************************************************************************/
#include "stdafx.h"
#include "PlanDuplicator.h"
#include "PlanDuplicatorDlg.h"
#include "PDLogHelper.h"
#include "IniFileReader.h"
#include "PDErrorMsgDlg.h"
#include "Winsock2.h"
#include <conio.h>
#include "PDController.h"
#include <tami/libWNP/WinNativePorting.h>
#include <tami/libplTree/plsftree.h>
#include <tami/libpl/plParDefs.h>
#include "vfcDBMgr.h"
#include "PDLogHelper.h"
#include "PDTrace.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const COLORREF FRAME_BKCOLOR = RGB(70, 80, 100);
#define APPLICATION_NAME L"PDTool"
#include <LogLibrary\MRErrorLog.h>
using namespace MR::ERRORLOG;
#include "ErrorDisp\ErrorDispConsts.h"
#include "ErrorDisp\ErrorDisplay.h"
using namespace MPlus::EH;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public  DialogEx // CDialog
{
public:
    CAboutDlg();
    
    // Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    //}}AFX_DATA
    
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
    
    // Implementation
protected:
    //{{AFX_MSG(CAboutDlg)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() :  DialogEx (CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    DialogEx ::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg,  DialogEx)
//{{AFX_MSG_MAP(CAboutDlg)
// No message handlers
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlanDuplicatorDlg dialog

//****************************Method Header************************************

//Method Name   :CPlanDuplicatorDlg

//Author        :PATNI

//Purpose       :Constructor
//*****************************************************************************

CPlanDuplicatorDlg::CPlanDuplicatorDlg(CWnd* pParent /*=NULL*/)
:  DialogEx (CPlanDuplicatorDlg::IDD, pParent),
m_ntotal_protocol(0),
m_nScannedtotal_protocol(0),
m_ncurrrowno(-1),
m_ncurrunscannedrowno(0),
m_controller(NULL),
m_nmode (-1),
m_naxis(-1),
m_nparameter(-1),
m_ncurrscannedrowno(-1),
m_bisCalculateDone(FALSE) //Mpower 047/Sep-2012/AN/Added
{
    PD_TRACE(CPlanDuplicatorDlg::CPlanDuplicatorDlg);
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);    
}

/*************************************************************
* Method:       ~CPlanDuplicatorDlg
* Description:  Destructor
* Returns:      
*************************************************************/
CPlanDuplicatorDlg::~CPlanDuplicatorDlg()
{
    if (NULL != m_controller)
    {
        delete m_controller;
        m_controller = NULL;
        
    }
}
//****************************Method Header************************************

//Method Name   :DoDataExchange

//Author        :PATNI

//Purpose       :This function is called by the framework to exchange and
//               validate dialog data
//*****************************************************************************
void CPlanDuplicatorDlg::DoDataExchange(CDataExchange* pDX)
{
    PD_TRACE(CPlanDuplicatorDlg::DoDataExchange);
    DialogEx ::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPlanDuplicatorDlg)
    DDX_Control(pDX, IDC_STATIC_MODE, m_Mode);
    DDX_Control(pDX, IDC_STATIC_AXIS, m_Axis);
    DDX_Control(pDX, IDC_STATIC_PARAMETER, m_Parameter);
    DDX_Control(pDX, IDC_BUTTON_CALCULATE2, m_btn_calculate);
    DDX_Control(pDX, IDC_BUTTON_BROWSE, m_BrowseButton);
    DDX_Control(pDX, IDC_BUTTON_GETDATA, m_GetDataButton);
    DDX_Control(pDX, IDC_BUTTON_UPDATESEQ, m_UpdateSeqButton);
    DDX_Control(pDX, IDC_BUTTON_COVERT, m_ConvertButton);
    DDX_Control(pDX, IDC_BUTTON_SMARTMPR, m_SmartMPRButton);
    DDX_Control(pDX, IDCANCEL, m_ExitButton);
    DDX_Control(pDX, IDC_EDIT_TEXTFILEPATH, m_FileName);
    DDX_Control(pDX, IDC_STATIC_UPDATED_SEQUENCE, m_UpdatedSequenceText);
    DDX_Control(pDX, IDC_STATIC_VANTAGEAXIS, m_Labelvantageaxis);
    DDX_Control(pDX, IDC_LISTCTRL_DATA, m_listctrl_data);
    DDX_Control(pDX, IDC_LIST_DICOMAXIS, m_listctrl_dataDicomAxis);
    DDX_Control(pDX, IDC_LISTCTRL_STATUS, m_listctrl_status);
    DDX_Control(pDX, IDC_LISTCTRL_SCANNEDPROT, m_listctrl_scannedProtocol);
    DDX_Control(pDX, IDC_LIST_UNSCANNEDPROT, m_listctrl_UnscannedProtocol);
    //DDX_Control(pDX, IDC_CHECK_AUTOUPDATE, m_check);
    DDX_Control(pDX, IDC_CHECK_DICOMAXIS, m_check_DicomAxis);
    DDX_Control(pDX, IDC_RADIO_AUTO_COUCH, m_radio_auto_couch);
    DDX_Control(pDX, IDC_RADIO_AXIAL, m_radio_axial);
    DDX_Control(pDX, IDC_RADIO_SLICE_NUMBER, m_radio_slice_number);
    DDX_Control(pDX, IDC_RADIO_SMART_MPR, m_radio_smart_mpr);
    DDX_Radio(pDX, IDC_RADIO_AUTO_COUCH, m_nmode);
    DDX_Radio(pDX, IDC_RADIO_AXIAL, m_naxis);
    DDX_Radio(pDX, IDC_RADIO_SLICE_NUMBER, m_nparameter);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPlanDuplicatorDlg,  DialogEx)
//{{AFX_MSG_MAP(CPlanDuplicatorDlg)
ON_WM_CTLCOLOR()
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
ON_BN_CLICKED(IDC_BUTTON_GETDATA, OnButtonGetdata)
ON_BN_CLICKED(IDC_RADIO_SMART_MPR, OnRadioSmartmpr)
ON_BN_CLICKED(IDC_RADIO_SMART_VOI, OnRadioSmartvoi)
ON_BN_CLICKED(IDC_RADIO_AUTO_COUCH, OnRadioAutoCouch)
ON_BN_CLICKED(IDC_RADIO_AUTO_PLAN, OnRadioAutoPlan)
ON_BN_CLICKED(IDC_RADIO_MANUAL, OnRadioManual)
ON_BN_CLICKED(IDC_BUTTON_UPDATESEQ, OnButtonUpdateseq)
ON_BN_CLICKED(IDC_CHECK_DICOMAXIS, OnCheckDicomaxis)
ON_NOTIFY(NM_CLICK, IDC_LISTCTRL_DATA, OnRowClkListctrlData)
ON_NOTIFY(NM_CLICK, IDC_LIST_UNSCANNEDPROT, OnRowClkListUnscannedprot)
ON_BN_CLICKED(IDC_BUTTON_SMARTMPR, OnButtonSmartmpr)
ON_BN_CLICKED(IDC_BUTTON_COVERT, OnButtonConvert)
ON_NOTIFY(NM_CLICK, IDC_LISTCTRL_SCANNEDPROT, OnClickListctrlScannedprot)
ON_BN_CLICKED(IDC_BUTTON_CALCULATE2, OnButtonCalculate)
ON_BN_CLICKED(IDC_RADIO_SLICE_NUMBER, OnRadioSliceNumber)
ON_BN_CLICKED(IDC_RADIO_SLICE_GAP, OnRadioSliceGap)
ON_BN_CLICKED(IDC_RADIO_SLICE_THICKNESS, OnRadioSliceThickness)
ON_BN_CLICKED(IDC_RADIO_AXIAL, OnRadioAxial)
ON_BN_CLICKED(IDC_RADIO_CORONAL, OnRadioCoronal)
ON_BN_CLICKED(IDC_RADIO_SAGITTAL, OnRadioSagittal)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlanDuplicatorDlg message handlers

//****************************Method Header************************************

//Method Name   :OnInitDialog

//Author        :PATNI

//Purpose       :This function is called in response to the WM_INITDIALOG msg
//*****************************************************************************
BOOL CPlanDuplicatorDlg::OnInitDialog()
{
    PD_TRACE(CPlanDuplicatorDlg::OnInitDialog);
    DialogEx ::OnInitDialog();
    
    WINDOW_COLORS_t l_window_color;
    GetWindowColors(l_window_color);
    m_list_brush.CreateSolidBrush(l_window_color.crClientColor);
    CString l_strFileName (_T(""));
   
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);
    
    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (NULL != pSysMenu){
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        
        if (TRUE != strAboutMenu.IsEmpty()){
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }
    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon   
    //To initialise all controls.
    InitializeControls();
    
    //To get studyFileName from commandLine
    GetDlgItem(IDC_LIST_DICOMAXIS)->ShowWindow(FALSE);
    CString m_Strfile = GetCommandLine();   
    
    int l_nIndex = m_Strfile.ReverseFind(' ');
    int l_nTempIndexStr = ((m_Strfile.GetLength())-(l_nIndex+1));
    CString l_TempStr = m_Strfile.Right(l_nTempIndexStr);   
     if ( -1 == l_TempStr.Find(_T("study"))){
         l_TempStr = _T("/gp/sites/site-5000/study/Run84.-5000"); //Test Anu
       }
    wcstombs(m_fcParams.studyfilename,(LPCTSTR) l_TempStr, BUFFER_SIZE_128);
    
    CString temp(m_fcParams.studyfilename);
    
    //to set default location of varfieldData file  
    CEdit* l_pName = (CEdit*)GetDlgItem(IDC_EDIT_TEXTFILEPATH); 
    //Vilas..July2012..Revw..use local variable for inifile_reader
	//Mpower 039//Jul-12/Added/AN
    m_controller = new CPDController();
    if (NULL == m_controller)
    {
        CString l_cserrmsg = _T("Controller NULL, memory allocation error");
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        return(TRUE);
        
    }
    m_controller->Initialize();
    m_controller->SetAppMode(SMART_VOI);
    CIniFileReader * l_ini_reader = NULL;
    l_ini_reader =  m_controller->GetIniFileReader();
    if (NULL == l_ini_reader)
    {
        CString l_cserr = _T("Ini reader NULL");
        CPDLogHelper::WriteToErrorLog(l_cserr,__LINE__);
        return(TRUE);
    }
    
    if(TRUE == m_radio_smart_mpr.GetCheck()){
        l_strFileName = l_ini_reader->GetMPRFilePath();
        if (TRUE == l_strFileName.IsEmpty()){
            l_strFileName = SMARTMPR_FILE_PATH;     
        }
    } else {
        l_strFileName = l_ini_reader->GetVOIFilePath();
        if(TRUE == l_strFileName.IsEmpty()){
            l_strFileName = SMARTVOI_FILE_PATH;         
        }
    }
    
    l_pName->SetWindowText(l_strFileName);  
    m_UpdateSeqButton.EnableWindow(FALSE);
    
    if(E_ERROR == getProtocols(l_TempStr) ){
//         CString l_csErrMsg = ErrorList[PD_STUDY_OPEN]; 
//         CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
    }
    
    return(TRUE);  // return TRUE  unless you set the focus to a control
}

/*************************************************************
* Method:    	OnSysCommand
* Description: 	The framework calls this member function when
                the user selects a command from the Control
*               menu, or when the user selects the Maximize
*               or the Minimize button.
* Parameter: 	UINT nID :
* Parameter: 	LPARAM lParam :
* Returns:   	void
*************************************************************/
void CPlanDuplicatorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    PD_TRACE(CPlanDuplicatorDlg::OnSysCommand);
    
    if ((nID & 0xFFF0) == IDM_ABOUTBOX){
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }else{
        DialogEx::OnSysCommand(nID, lParam);
    }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
/*************************************************************
* Method:    	OnPaint
* Description: 	The framework calls this member function when
*               Windows or an application makes a request to
*               repaint a portion of an application's window.   
* Returns:   	void
*************************************************************/
void CPlanDuplicatorDlg::OnPaint() 
{
    PD_TRACE(CPlanDuplicatorDlg::OnPaint);
	m_listctrl_UnscannedProtocol.ShowScrollBar(SB_VERT,TRUE);
	m_listctrl_status.ShowScrollBar(SB_VERT,TRUE);
    if (TRUE == IsIconic()){
        CPaintDC dc(this); // device context for painting
        // Center icon in client rectangle
        int l_cxIcon = GetSystemMetrics(SM_CXICON);
        int l_cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int l_x = (rect.Width() - l_cxIcon + 1) / 2;
        int l_y = (rect.Height() - l_cyIcon + 1) / 2;       
        // Draw the icon
        dc.DrawIcon(l_x, l_y, m_hIcon);
    }else{
        DialogEx::OnPaint();
    }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
/*************************************************************
* Method:    	OnQueryDragIcon
* Description: 	The framework calls this member function by a
*               minimized (iconic) window that does not have 
*               an icon defined for its class.
* Returns:   	HCURSOR
*************************************************************/
HCURSOR CPlanDuplicatorDlg::OnQueryDragIcon()
{
    PD_TRACE(CPlanDuplicatorDlg::OnQueryDragIcon);
    return(HCURSOR) m_hIcon;
}

/*************************************************************
* Method:    	OnButtonBrowse
* Description: 	This event is called when Browse button is clicked.
*               It opens a fileDialog which enables user to
*               select file from the folder or directory and 
*               display path in file editbox.User
*               can even edit editbox manually.
* Returns:   	void
*************************************************************/
void CPlanDuplicatorDlg::OnButtonBrowse() 
{
    PD_TRACE(CPlanDuplicatorDlg::OnButtonBrowse);
    
    CFileDialog l_fileDlg(TRUE/*Open=TRUE Save=False*/,NULL/*Filename Extension*/,_T("SVOI_PLNdata.txt")/*Initial Filename*/,OFN_ENABLESIZING|OFN_EXPLORER|OFN_FILEMUSTEXIST/*Flags*/,_T("SmartMPR Files(*.*)|*.*||")/*Filetype Filter*/,this/*parent Window*/);
    
    if(l_fileDlg.DoModal() == IDOK){        
        CString l_strFileName = l_fileDlg.GetPathName();        
        m_FileName.SetWindowText(l_strFileName);    
    }   
    
}


/*************************************************************
* Method:    	InitializeControls
* Description: 	Initializes all controls on the GUI
* Parameter: 	void  :
* Returns:   	void
*************************************************************/
void CPlanDuplicatorDlg::InitializeControls(void)
{
    PD_TRACE(CPlanDuplicatorDlg::InitializeControls);
    m_BrowseButton.SetPrimaryTextFont(_T("Arial"), FONT_SIZE_12, TRUE);
    m_BrowseButton.SetButtonType(TUSBUTTON_ROUND_ALL);
    m_BrowseButton.EnableWindow(FALSE); //AN/Added
    
    m_GetDataButton.SetPrimaryTextFont(_T("Arial"), FONT_SIZE_12, TRUE);
    m_GetDataButton.SetButtonType(TUSBUTTON_ROUND_ALL);
    
    m_ExitButton.SetPrimaryTextFont(_T("Arial"), FONT_SIZE_12, TRUE);
    m_ExitButton.SetButtonType(TUSBUTTON_ROUND_ALL);
    
    m_UpdateSeqButton.SetPrimaryTextFont(_T("Arial"), FONT_SIZE_12, TRUE);
    m_UpdateSeqButton.SetButtonType(TUSBUTTON_ROUND_ALL);
    
    m_ConvertButton.SetPrimaryTextFont(_T("Arial"), FONT_SIZE_12, TRUE);
    m_ConvertButton.SetButtonType(TUSBUTTON_ROUND_ALL);
    
    m_SmartMPRButton.SetPrimaryTextFont(_T("Arial"), FONT_SIZE_12, TRUE);
    m_SmartMPRButton.SetButtonType(TUSBUTTON_ROUND_ALL);
    m_SmartMPRButton.SetWindowText(_T("Smart VOI"));
    
    m_btn_calculate.SetPrimaryTextFont(_T("Arial"), FONT_SIZE_12, TRUE);
    m_btn_calculate.SetButtonType(TUSBUTTON_ROUND_ALL);
    
    m_FileName.SetFont( _T("Arial"), FONT_SIZE_12, TRUE );    
    
    m_Labelvantageaxis.SetFont(&m_font,TRUE);
    
    m_check_DicomAxis.SetFont(_T("Arial"),FONT_SIZE);
    m_check_DicomAxis.SetBkColor(BKCOLOR);  
    m_check_DicomAxis.EnableWindow(FALSE);
    
    m_radio_auto_couch.SetFont(_T("Arial"), FONT_SIZE_10, FALSE );
    m_radio_auto_couch.SetBkColor(BKCOLOR);
    m_radio_auto_couch.SetCheck(1);
    m_nmode = AUTO_COUCH;
    
    m_radio_axial.SetFont(_T("Arial"), FONT_SIZE_10, FALSE );
    m_radio_axial.SetBkColor(BKCOLOR);
    m_radio_axial.SetCheck(1);
    m_naxis = 0;
    
    m_radio_slice_number.SetFont(_T("Arial"), FONT_SIZE_10, FALSE );
    m_radio_slice_number.SetBkColor(BKCOLOR);
    m_radio_slice_number.SetCheck(1);
    m_nparameter = 0;
    
    m_radio_smart_mpr.SetFont(_T("Arial"), FONT_SIZE_10, FALSE );
    m_radio_smart_mpr.SetBkColor(BKCOLOR);
    m_radio_smart_mpr.SetCheck(0);
    
    RadioButtonEx*l_radio_smart_voi = NULL;
    
    l_radio_smart_voi=(RadioButtonEx*)GetDlgItem(IDC_RADIO_SMART_VOI);
    if (NULL != l_radio_smart_voi){
        l_radio_smart_voi->SetCheck(1);
    }
    
    DisableEnableRadioBtns(FALSE);
    UncheckAll();
    
    m_Mode.SetFrameColor(FRAME_BKCOLOR);
    m_Mode.SetFont(_T("Arial"), FONT_SIZE_10, TRUE );
    m_Axis.SetFrameColor(FRAME_BKCOLOR);
    m_Axis.SetFont(_T("Arial"), FONT_SIZE_10, TRUE );
    m_Parameter.SetFrameColor(FRAME_BKCOLOR);
    m_Parameter.SetFont(_T("Arial"), FONT_SIZE_10, TRUE );
    
    m_UpdatedSequenceText.SetFont(&m_font,TRUE);
    
    m_listctrl_data.SetFont(_T("Arial"), FONT_SIZE_10, TRUE );
    //insert columns in list control
    //AS added   
    RECT l_listctrl_status;
    m_listctrl_status.GetWindowRect(&l_listctrl_status);
    
    m_listctrl_status.SetWindowPos(NULL,l_listctrl_status.left,l_listctrl_status.top,640,l_listctrl_status.bottom-l_listctrl_status.top,SWP_NOZORDER); 
    InitListColumnForVOI();
    
    m_listctrl_dataDicomAxis.SetFont(_T("Arial"), FONT_SIZE_10, TRUE );
    m_listctrl_status.SetFont(_T("Arial"), FONT_SIZE_10, TRUE );
    
    //insert columns in list control
       
    m_listctrl_scannedProtocol.SetFont(_T("Arial"), FONT_SIZE_10, TRUE );
    //insert columns in list control
    m_listctrl_scannedProtocol.InsertColumn(0,_T("Scanned Protocol"),LVCFMT_LEFT,150);
    m_listctrl_scannedProtocol.SetColumnRange(0,150,BUFFER_SIZE_160);
    
    m_listctrl_UnscannedProtocol.SetFont(_T("Arial"), FONT_SIZE_10, TRUE );
    //insert columns in list control
    m_listctrl_UnscannedProtocol.InsertColumn(0,_T("Unscanned Protocol"),LVCFMT_LEFT,150);
    m_listctrl_UnscannedProtocol.SetColumnRange(0,160,160); 

    SetRibbonHeight(45);
//    m_listctrl_data.ShowScrollBar(SB_HORZ); //AN
    
}

/*************************************************************
* Method:    	OnButtonGetdata
* Description: 	This event is called when user clicks on 
*               GetData button.
*               It reads path specified in edit box. Pass it to
*                the function SaveSlicePlaneData. Checks the
*                return type if it is non zero value shows 
*                error else display data in list control box
* Returns:   	void
*************************************************************/
void CPlanDuplicatorDlg::OnButtonGetdata() 
{
    PD_TRACE(CPlanDuplicatorDlg::OnButtonGetdata);
    
    bool l_ncalculate_flag = FALSE;
    m_listctrl_data.EnableWindow(TRUE);
    m_listctrl_dataDicomAxis.EnableWindow(TRUE);
    int l_nerror_code = 0;  
    m_bisCalculateDone = FALSE; //Mpower 047/Sep-2012/AN/Added
    m_listctrl_data.DeleteAllItems();   
    m_listctrl_dataDicomAxis.DeleteAllItems();  
    //    m_listctrl_data.EnableWindow(FALSE);
    
  //  CString l_check = "";
    CString l_strFileName (_T(""));
    m_FileName.GetWindowText(l_strFileName);
    m_ncurrrowno = 0;
    if(TRUE == l_strFileName.IsEmpty())
    {       
        CIniFileReader * l_ini_reader = NULL;
        l_ini_reader =  m_controller->GetIniFileReader(); 
      
        if(TRUE == m_radio_smart_mpr.GetCheck())
        {
            if (NULL != l_ini_reader)
            {           
                l_strFileName = l_ini_reader->GetMPRFilePath();
            }
            if (TRUE == l_strFileName.IsEmpty())
            {
                l_strFileName = SMARTMPR_FILE_PATH;     
            }
        } else {
            if (NULL != l_ini_reader)
            {
                l_strFileName = l_ini_reader->GetVOIFilePath();
            }
            if(TRUE == l_strFileName.IsEmpty()){
                l_strFileName = SMARTVOI_FILE_PATH;         
            }
        }
    } else {      

//         CString l_csErrMsg = _T("Select file first");
//         CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__,CUSTOM_ERRORDIALOG_DISPLAY);
//         m_UpdateSeqButton.EnableWindow(FALSE);
//         m_listctrl_data.EnableWindow(TRUE);
    }
    l_nerror_code = m_controller->SaveSlicePlaneData(l_strFileName);
    
    if(E_NO_ERROR == l_nerror_code ){
        ShowData(l_ncalculate_flag);
    } else {
        //no need to log, error is displayed  inside function
        }
    if (TRUE == m_radio_smart_mpr.GetCheck())
    {
        DisableEnableRadioBtns(FALSE);
        UncheckAll();
        RadioButtonEx *l_auto_move = NULL;
        l_auto_move= (RadioButtonEx*)GetDlgItem(IDC_RADIO_AUTO_PLAN);
        //         if (NULL != l_auto_move) 
        //         {
        l_auto_move->EnableWindow(TRUE);
        l_auto_move->SetCheck(1);
        m_nmode = AUTO_PLAN;
        l_auto_move= (RadioButtonEx*)GetDlgItem(IDC_RADIO_MANUAL);
        l_auto_move->EnableWindow(TRUE);
        m_UpdateSeqButton.EnableWindow(TRUE);
        m_btn_calculate.EnableWindow(FALSE);
        //}
        
    } else{
        DisableEnableRadioBtns(TRUE);
        UncheckAll();
        CheckMain();
        CWnd *l_auto_move = NULL;
        m_radio_slice_number.EnableWindow(FALSE);
        l_auto_move= GetDlgItem(IDC_RADIO_SLICE_GAP);
        l_auto_move->EnableWindow(FALSE);
        l_auto_move= GetDlgItem(IDC_RADIO_SLICE_THICKNESS);
        l_auto_move->EnableWindow(FALSE);
        m_btn_calculate.EnableWindow(TRUE);
        m_radio_slice_number.SetCheck(0);
        m_UpdateSeqButton.EnableWindow(TRUE);
        m_btn_calculate.EnableWindow(FALSE);
    }
}


/*************************************************************************
* Method:    	getProtocols
* Description: 	This function reads study file. Gets order of protocol from 
*               study file by calling getProtAcqOrder function. After getting 
*               the order it call CheckIsProtocolScanned to check protocol 
*               is scanned or not and according display the protocol in 
*               particular listctrl box.
* Parameter: 	const CString & f_sfileName :
* Returns:   	int
*************************************************************************/
int CPlanDuplicatorDlg::getProtocols(const CString & f_sfileName)
{   
    PD_TRACE(CPlanDuplicatorDlg::getProtocols);
    
    int l_nstatus = 0;
    int l_ni = 0;
    int l_nj = 0;
    int l_nk = 0;
    int l_ntotal_prot = -1;
    int l_nerror_code = 0;
    CString l_protStr(_T(""));
    p_data_t * prot_list = NULL;
    p_data_t l_prot_info = {0};
    l_nstatus = m_controller->GetProtocolsList(f_sfileName,&prot_list,&l_ntotal_prot);
    
    if (E_ERROR == l_nstatus){
        CString l_csErrMsg = _T("Function GetProtocolsList() returned an error"); 
        CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
        return(E_ERROR);
    }
//     if ( 0 >= l_ntotal_prot ) {
//       //  l_nerror_code = PD_NO_UNSCANNED_PROTOCOL;
//         CString l_csErrMsg = "No Protocols available in Study File";
//         CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__,ERRORVIEW_DISPLAY);
//         return(E_ERROR);
//     }
    for(l_ni = 0;l_ni < l_ntotal_prot;l_ni++) {
        if ( UNSCANNED == prot_list[l_ni].isScanned ) {
            l_prot_info.protocolNum = prot_list[l_ni].protocolNum;
            m_ntotal_protocol++;           
            l_prot_info.protocol = (l_ni+1)*1000;    
            l_protStr.Format(_T("%d"),l_prot_info.protocol);        
            m_p_data.push_back(l_prot_info);
            m_listctrl_UnscannedProtocol.InsertItem(l_nj,NULL);
            m_listctrl_UnscannedProtocol.SetItemText(l_nj,0,l_protStr);
            l_nj++;
        }else if (SCANNED == prot_list[l_ni].isScanned) {
            l_prot_info.protocolNum = prot_list[l_ni].protocolNum;
            l_prot_info.protocol = (l_ni+1)*1000; 
            l_protStr.Format(_T("%d"),l_prot_info.protocol); 
            m_p_data_scanned.push_back(l_prot_info);
            m_listctrl_scannedProtocol.InsertItem(l_nk,NULL);
            m_listctrl_scannedProtocol.SetItemText(l_nk,0,l_protStr);           
            l_nk++;
            m_nScannedtotal_protocol++;
        }
    }
    if (0 == m_nScannedtotal_protocol)
    {
        l_nerror_code = PD_NO_SCANNED_PROTOCOL;
        CString l_csErrMsg = ErrorList[l_nerror_code];
        CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__);
    } else {
        m_controller->SetScannedProtIdx(m_p_data_scanned[m_nScannedtotal_protocol].protocolNum);
    }
    if(0 == m_ntotal_protocol){
        l_nerror_code = PD_NO_UNSCANNED_PROTOCOL;
        CString l_csErrMsg = ErrorList[l_nerror_code];
        CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__,CUSTOM_ERRORDIALOG_DISPLAY);
        return(E_ERROR);
    }
    
    return(E_NO_ERROR);
}


//****************************Method Header************************************

//Method Name   :OnCtlColor

//Author        :PATNI

//Purpose       :The framework calls this function when a child
//               control is about to be drawn
//****************************************************************************
HBRUSH CPlanDuplicatorDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
    PD_TRACE(CPlanDuplicatorDlg::OnCtlColor);
    HBRUSH hbr = DialogEx::OnCtlColor(pDC, pWnd, nCtlColor); 
    if (m_nmode != MANUAL)
    {
        // change text colors of list ctrl
        m_listctrl_data.SetTextColor(DISABLED_LIST_COLOR);
    }
    else
    {
        m_listctrl_data.SetTextColor(ENABLED_LIST_COLOR);
    }
    if(nCtlColor == CTLCOLOR_STATIC){
        pDC->SetBkColor(BKCOLOR);
        pDC->SetTextColor(TEXT_BKCOLOR);
        pDC->SetBkMode(OPAQUE);
        return(m_list_brush);
    }else{
        //
    }
    
    return(hbr);
}

/*************************************************************
* Method:    	OnCheckDicomaxis
* Description: 	On check of Dicom Axis checkbox Dicom data List
*               is made visible and on uncheck it hides the 
*               Dicom data List and adjust the height of dialog 
*               box and placement of all other controls in both
*               the cases.
* Returns:   	void
*************************************************************/
void CPlanDuplicatorDlg::OnCheckDicomaxis() 
{
    PD_TRACE(CPlanDuplicatorDlg::OnCheckDicomaxis);
    
    if(0 != m_listctrl_dataDicomAxis.GetItemCount()){
        m_listctrl_dataDicomAxis.EnableWindow(TRUE);
        
        if(TRUE == m_check_DicomAxis.GetCheck()){
            WINDOWPLACEMENT l_wndpl;
            GetWindowPlacement( &l_wndpl );
            l_wndpl.rcNormalPosition.bottom += 150;
            SetWindowPlacement( &l_wndpl );
            int l_noffset = 150;
            GetDlgItem(IDC_LIST_DICOMAXIS)->ShowWindow(TRUE);
            ControlsMove(l_noffset);        
            CRect l_ContorlRect;
            MoveControlsUpDown(1,m_listctrl_scannedProtocol,l_ContorlRect);         
        }else{
            WINDOWPLACEMENT l_wndpl;
            GetWindowPlacement( &l_wndpl );
            l_wndpl.rcNormalPosition.bottom -= 150;
            SetWindowPlacement( &l_wndpl );
            int l_noffset = -150;
            GetDlgItem(IDC_LIST_DICOMAXIS)->ShowWindow(FALSE);
            ControlsMove(l_noffset);        
            CRect l_ContorlRect;
            MoveControlsUpDown(-1,m_listctrl_scannedProtocol,l_ContorlRect);    
        }
    } else {
        if(m_check_DicomAxis.GetCheck() == TRUE){
            WINDOWPLACEMENT l_wndpl;
            GetWindowPlacement( &l_wndpl );
            l_wndpl.rcNormalPosition.bottom += 150;
            SetWindowPlacement( &l_wndpl );
            int l_noffset = 150;
            GetDlgItem(IDC_LIST_DICOMAXIS)->ShowWindow(TRUE);
            ControlsMove(l_noffset);        
            CRect l_ContorlRect;
            MoveControlsUpDown(1,m_listctrl_scannedProtocol,l_ContorlRect);         
        }else{
            WINDOWPLACEMENT l_wndpl;
            GetWindowPlacement( &l_wndpl );
            l_wndpl.rcNormalPosition.bottom -= 150;
            SetWindowPlacement( &l_wndpl );
            int l_noffset = -150;
            GetDlgItem(IDC_LIST_DICOMAXIS)->ShowWindow(FALSE);
            ControlsMove(l_noffset);        
            CRect l_ContorlRect;
            MoveControlsUpDown(-1,m_listctrl_scannedProtocol,l_ContorlRect);    
        }       
    }
}

/*************************************************************
* Method:    	ControlsMove
* Description: 	It adjust the placement of controls up and
                down according to specification.
* Parameter: 	int & f_nshiftControl :
* Returns:   	void
*************************************************************/
void CPlanDuplicatorDlg::ControlsMove(int &f_nshiftControl)
{
    PD_TRACE(CPlanDuplicatorDlg::ControlsMove);
    CRect ControlRect;
    CWnd *l_auto_move = NULL;
    
    MoveControlsUpDown(f_nshiftControl,m_radio_auto_couch,ControlRect);
    l_auto_move= GetDlgItem(IDC_RADIO_AUTO_PLAN);
    MoveControlsUpDown(f_nshiftControl,*l_auto_move,ControlRect);
    l_auto_move= GetDlgItem(IDC_RADIO_MANUAL);
    MoveControlsUpDown(f_nshiftControl,*l_auto_move,ControlRect);
    
    MoveControlsUpDown(f_nshiftControl,m_radio_slice_number,ControlRect);
    l_auto_move= GetDlgItem(IDC_RADIO_SLICE_GAP);
    MoveControlsUpDown(f_nshiftControl,*l_auto_move,ControlRect);
    l_auto_move= GetDlgItem(IDC_RADIO_SLICE_THICKNESS);
    MoveControlsUpDown(f_nshiftControl,*l_auto_move,ControlRect);
    
    MoveControlsUpDown(f_nshiftControl,m_radio_axial,ControlRect);
    l_auto_move= GetDlgItem(IDC_RADIO_CORONAL);
    MoveControlsUpDown(f_nshiftControl,*l_auto_move,ControlRect);
    l_auto_move= GetDlgItem(IDC_RADIO_SAGITTAL);
    MoveControlsUpDown(f_nshiftControl,*l_auto_move,ControlRect);
    
    MoveControlsUpDown(f_nshiftControl,m_Mode,ControlRect);
    MoveControlsUpDown(f_nshiftControl,m_Axis,ControlRect);
    MoveControlsUpDown(f_nshiftControl,m_Parameter,ControlRect);
    MoveControlsUpDown(f_nshiftControl,m_btn_calculate,ControlRect);
    
    MoveControlsUpDown(f_nshiftControl,m_listctrl_UnscannedProtocol,ControlRect);
    MoveControlsUpDown(f_nshiftControl,m_UpdateSeqButton,ControlRect);
    MoveControlsUpDown(f_nshiftControl,m_UpdatedSequenceText,ControlRect);
    MoveControlsUpDown(f_nshiftControl,m_ExitButton,ControlRect);
    MoveControlsUpDown(f_nshiftControl,m_listctrl_status,ControlRect);
    MoveControlsUpDown(f_nshiftControl,m_listctrl_data,ControlRect);
    MoveControlsUpDown(f_nshiftControl,m_Labelvantageaxis,ControlRect);
}

/*************************************************************
* Method:    	ShowData
* Description: 	Display VarfieldData & DicomData in respective
*               ListCtrl box
* Parameter: 	bool f_ncalculate_flag :
* Returns:   	int
*************************************************************/
int CPlanDuplicatorDlg::ShowData(bool f_ncalculate_flag)
{
    PD_TRACE(CPlanDuplicatorDlg::ShowData);
    m_listctrl_data.DeleteAllItems();
    m_listctrl_dataDicomAxis.DeleteAllItems();
    
    if (m_radio_smart_mpr.GetCheck()){
        ShowMPRData();
    }else{
        ShowVOIData(f_ncalculate_flag);
    }
    if (m_ncurrrowno >= 0){
        m_listctrl_data.SetItemState(m_ncurrrowno,LVIS_SELECTED,LVIS_SELECTED | LVIS_FOCUSED);
        m_listctrl_data.EnsureVisible(m_ncurrrowno,FALSE);
    } else {
        m_listctrl_data.SetItemState(-1,0,LVIS_SELECTED);
    }
    return(E_NO_ERROR);
}

/*************************************************************
* Method:    	OnButtonUpdateseq
* Description: 	This event is called when user clicks on update
*               Sequence button It checks whether AutoUpadate
*               check box is checked or not and perform the
*               action accordingly. 
* Returns:   	void
*************************************************************/
void CPlanDuplicatorDlg::OnButtonUpdateseq() 
{
    PD_TRACE(CPlanDuplicatorDlg::OnButtonUpdateseq);
    CString l_csUnScannedData (_T(""));    
    int l_nprotIndex = 0;
    int l_status = E_NO_ERROR;
    AfxGetApp()->DoWaitCursor(1);
    try
    {
        l_csUnScannedData = m_listctrl_UnscannedProtocol.GetItemText(m_ncurrunscannedrowno,0);
        char *l_cpC = new char[BUFFER_SIZE_160];
        
        if (NULL == l_cpC){
            AfxGetApp()->DoWaitCursor(-1);
            CString l_cserrmsg = ErrorList[PD_MEMORY_ALLOCATION];
            CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__,ERRORVIEW_DISPLAY); //AS
            return;
        }
        
        wcstombs(l_cpC,(LPCTSTR) l_csUnScannedData, BUFFER_SIZE_160);
        int l_nIndex=(int)atoi(l_cpC);
        
        if (NULL != l_cpC){
            delete [] l_cpC;    
            l_cpC = NULL;
        }
        
        if (0 == m_listctrl_UnscannedProtocol.GetSelectedCount()){
            AfxGetApp()->DoWaitCursor(-1);
            CString l_csErrMsg = ErrorList[PD_SELECT_UNSCANNED_PROTOCOL];
            CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__,CUSTOM_ERRORDIALOG_DISPLAY);
            return;
        }
        int i = 0;
        for(i = 0;i < m_ntotal_protocol;i++){
            if(l_nIndex == m_p_data[i].protocol){
                l_nprotIndex = m_p_data[i].protocolNum;
                break;
            }
        }
        
        int l_countdata = m_listctrl_data.GetItemCount();
        
        if (0 < l_countdata)
        {     
            
            //if ( AUTO_COUCH == /*m_radio_auto_plan.GetCheck()*/m_nmode ){   
            if (MANUAL == m_nmode){  //Manual Case
                if (0 >= m_listctrl_data.GetSelectedCount())
                {
                    AfxGetApp()->DoWaitCursor(-1);
                    CString l_csErrMsg = ErrorList[PD_SELECT_SLICEPLAN];
                    CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__,CUSTOM_ERRORDIALOG_DISPLAY);
                    return;
                    
                }else if (/*0 == m_listctrl_data.GetSelectedCount() ||*/ 0 >= m_listctrl_UnscannedProtocol.GetSelectedCount()){
                    AfxGetApp()->DoWaitCursor(-1);
                    CString l_csErrMsg = ErrorList[PD_SELECT_UNSCANNED_PROTOCOL];
                    CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__,CUSTOM_ERRORDIALOG_DISPLAY);
                    return;
                }else{              
                    CString l_Vfstr = m_controller->GetPDDataHandle()->GetVantageSlicePlane(m_ncurrrowno);
                    m_fcParams.sel_protocol_index_value[0] = l_nprotIndex;  
                    CString l_plane_name = m_listctrl_data.GetItemText(m_ncurrrowno,0);
                    int l_nscout_select = !(l_plane_name.Compare(_T(SLICE_PLANE_SCOUT2)));
                    m_fcParams.isScoutSelected = l_nscout_select; //Mpower 039//Jul-12/Added/AN
                    l_status = m_controller->Updatefields(/*l_mode,*/m_ncurrrowno,m_fcParams/*l_Vfstr*/,l_nscout_select);
                    if (E_ERROR != l_status) //If Updation is completed create a string of selected protocol index
                    {
                        CreateProtocolIndexString(m_fcParams.sel_protocol_index_value[0]);
                    }           
                    showStatus(m_fcParams.sel_protocol_index_value[0],l_Vfstr,l_status);//Mpower 039//Jul-12/Added/AN
                    
                }           
            } else if(AUTO_PLAN == m_nmode) {// Auto Plan mode
                 m_bisCalculateDone = FALSE; //Mpower 047/Sep-2012/AN/Added
                int l_protCount = m_listctrl_UnscannedProtocol.GetItemCount();
                int l_nsliceplanes = m_listctrl_data.GetItemCount() - 1;
                if(l_nsliceplanes > (l_protCount - m_ncurrunscannedrowno)) {             
                    CString l_csErrMsg (_T(""));
                    l_csErrMsg = ErrorList[PD_REQ_UPDATE_VARFIELD];
                    CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__,CUSTOM_ERRORDIALOG_DISPLAY);
                    
                } else {
                    int l_minCount = m_listctrl_UnscannedProtocol.GetItemCount()-m_ncurrunscannedrowno;
                    // int l_minCount = m_listctrl_data.GetItemCount() - 1; // Except "scout2"
                    int *l_unScannedProt= new int[l_minCount];
                    
                    if(NULL == l_unScannedProt){
                        AfxGetApp()->DoWaitCursor(-1);
                        return;
                    }
                    
                    int j=0,k=0;
                    for(j = i,k = 0;j < m_ntotal_protocol;j++,k++){
                        l_unScannedProt[k] = m_p_data[j].protocolNum;           
                    }
                    
                    if_error(RunAutoUpdate(l_unScannedProt))
                    {
                        CString l_cserrmsg = _T("ERROR in RunAutoUpdate");
                        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__,ERROR_LOGONLY);
                       
                        if (NULL != l_unScannedProt)
                        {       //Mpower 040/2012-08-16/AS/Added
                        delete [] l_unScannedProt;
                        l_unScannedProt=NULL;
                    }
                        AfxGetApp()->DoWaitCursor(-1);
                        return;
                    }
                    
                    if (NULL != l_unScannedProt){
                        delete [] l_unScannedProt;
                        l_unScannedProt=NULL;
                    }
                }
            }
			//Mpower 039//Jul-12/Added/AN
            else // Auto Couch mode 
            {
                int l_nrowno = 0;
                m_ncurrrowno = 0;
                CalculateSelect();
                CString l_Vfstr = m_controller->GetPDDataHandle()->GetVantageSlicePlane(m_ncurrrowno);
                m_fcParams.sel_protocol_index_value[0] = l_nprotIndex;  
                //set mode as Smart MPR or Smart VOI
                CString l_plane_name = m_listctrl_data.GetItemText(0,0); //ALways scout should be 1st line
                
                status_t l_nstatus = E_NO_ERROR;
                m_controller->ClearCalcData();
                l_nstatus = m_controller->CalculateParams(l_nrowno,m_fcParams);        
                if (l_nstatus != E_NO_ERROR)
                {
                    l_nstatus = E_ERROR;
                    CString l_cserrmsg = _T("Calculation error. Cannot update sequence");        //ErrorList[PD_CALC_ERROR];
                    CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
                    // return;
                } else 
                {   
                    l_nstatus = m_controller->Updatefields(m_ncurrrowno,m_fcParams,1);
                }
                if (E_NO_ERROR == l_nstatus) //If Updation is completed create a string of selected protocol index
                {
                    CreateProtocolIndexString(m_fcParams.sel_protocol_index_value[0]);
                }    
                showStatus(m_fcParams.sel_protocol_index_value[0],l_Vfstr,l_nstatus);                
            }
            
            //If String is not empty send message to PQM for updation.
            if (TRUE != m_prot_index_string.IsEmpty())
            {
                BOOL l_nstatus = E_ERROR;
                l_nstatus = m_controller->SendMessagePQM(m_prot_index_string);
                if (E_NO_ERROR != l_nstatus)
                {
                    CString l_csErrMsg = ErrorList[PD_SEND_MSG_PQM];
                    CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__,ERRORVIEW_DISPLAY); //AN
                }
                
                m_prot_index_string.Empty();
            } else {
                CString l_cserr (_T("No protocols to be updated in PQM"));
                CPDLogHelper::WriteToErrorLog(l_cserr,__LINE__);
            }
        }
        else
        {
            AfxGetApp()->DoWaitCursor(-1);
            CString l_csErrMsg = ErrorList[PD_SELECT_UNSCANNED_PROTOCOL];
            CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__,CUSTOM_ERRORDIALOG_DISPLAY);
            return;
            
        }
         AfxGetApp()->DoWaitCursor(-1);
        ShowData(TRUE);
    }
    catch(...){
        CString l_csErrMsg = _T("Exception occured during Update ");
        CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__,ERRORVIEW_DISPLAY); //AN
    }
}

/*************************************************************
* Method:    	OnRowClkListctrlData
* Description: 	This function is used 
* Parameter: 	NMHDR * pNMHDR :
* Parameter: 	LRESULT * pResult :
* Returns:   	void
*************************************************************/
void CPlanDuplicatorDlg::OnRowClkListctrlData(NMHDR* pNMHDR, LRESULT* pResult)
{
    PD_TRACE(CPlanDuplicatorDlg::OnRowClkListctrlData);
    
    if ((NULL == pNMHDR) || (NULL == pResult)){
        //return;
    }else{
        NMLISTVIEW* l_plist = (NMLISTVIEW*)pNMHDR;
        m_ncurrrowno = l_plist->iItem;  
        *pResult = 0;
        CString l_plane_name;
        
        if (TRUE == m_radio_smart_mpr.GetCheck()){// Smart MPR
            //
        } else{//Smart VOI
            l_plane_name = m_listctrl_data.GetItemText(m_ncurrrowno,0);
            
            if(m_nmode == MANUAL){// Mode Manual
                if(0 == l_plane_name.Compare(_T(SLICE_PLANE_SCOUT2))){
//                    m_controller->SetSlicePlaneSelection(0);
                    CWnd *l_auto_move_state = NULL;
                    m_radio_slice_number.EnableWindow(FALSE);
                    l_auto_move_state= GetDlgItem(IDC_RADIO_SLICE_GAP);
                    l_auto_move_state->EnableWindow(FALSE);
                    l_auto_move_state= GetDlgItem(IDC_RADIO_SLICE_THICKNESS);
                    l_auto_move_state->EnableWindow(FALSE);
                    RadioButtonEx *l_auto_move = NULL;
                    m_radio_slice_number.SetCheck(0);
                    l_auto_move= (RadioButtonEx*)GetDlgItem(IDC_RADIO_SLICE_GAP);
                    l_auto_move->SetCheck(0);
                    l_auto_move= (RadioButtonEx*)GetDlgItem(IDC_RADIO_SLICE_THICKNESS);
                    l_auto_move->SetCheck(0);
                    m_nparameter = -1;
                    m_fcParams.isScoutSelected= TRUE;
                }
                
                else if(0 == l_plane_name.Compare(_T(SLICE_PLANE_MAP)) || 0 == l_plane_name.Compare(_T(SLICE_PLANE_SHIMMING)))
                {
                    CWnd *l_auto_move_state = NULL;
                    m_radio_slice_number.EnableWindow(FALSE);
                    l_auto_move_state= GetDlgItem(IDC_RADIO_SLICE_GAP);
                    l_auto_move_state->EnableWindow(FALSE);
                    l_auto_move_state= GetDlgItem(IDC_RADIO_SLICE_THICKNESS);
                    l_auto_move_state->EnableWindow(FALSE);
                    RadioButtonEx *l_auto_move = NULL;
                    m_radio_slice_number.SetCheck(0);
                    l_auto_move= (RadioButtonEx*)GetDlgItem(IDC_RADIO_SLICE_GAP);
                    l_auto_move->SetCheck(1);
                    m_nparameter = 1; //AN Added : Slice gap default
                    l_auto_move= (RadioButtonEx*)GetDlgItem(IDC_RADIO_SLICE_THICKNESS);
                    l_auto_move->SetCheck(0);
                    m_fcParams.isScoutSelected= FALSE;
                }
                
                else if(0 == l_plane_name.Compare(_T(SLICE_PLANE_MULTISLICE)))
                {
                    RadioButtonEx *l_auto_move = NULL;
//                    m_controller->SetSlicePlaneSelection(1);
                    m_radio_slice_number.SetCheck(1);
                    m_nparameter = 0; //AN Added : Slice number Default
                    l_auto_move= (RadioButtonEx*)GetDlgItem(IDC_RADIO_SLICE_GAP);
                    l_auto_move->SetCheck(0);
                    l_auto_move= (RadioButtonEx*)GetDlgItem(IDC_RADIO_SLICE_THICKNESS);
                    l_auto_move->SetCheck(0);
                    CWnd *l_auto_move_state = NULL;
                    m_radio_slice_number.EnableWindow(TRUE);
                    l_auto_move_state= GetDlgItem(IDC_RADIO_SLICE_GAP);
                    l_auto_move_state->EnableWindow(TRUE);
                    l_auto_move_state= GetDlgItem(IDC_RADIO_SLICE_THICKNESS);
                    l_auto_move_state->EnableWindow(TRUE);
                    m_fcParams.isScoutSelected= FALSE;
                }
                else
                {
                    CString l_cserr (_T("Sliceplane displayed does not match"));
                    CPDLogHelper::WriteToErrorLog(l_cserr,__LINE__);
                }
            }
            //  }
            //}
            // if(SMART_VOI == m_controller->GetMPRorVOIMode())
            // {
            if ((TRUE == m_bisCalculateDone) && (0 == l_plane_name.Compare(_T(SLICE_PLANE_SCOUT2)))) //Mpower 047/Sep-2012/AN/Added
            {
                m_UpdateSeqButton.EnableWindow(TRUE);
                
            } else
            {
                m_UpdateSeqButton.EnableWindow(FALSE);
            }
            // }
        }
    }
}

/*************************************************************
* Method:    	OnRowClkListUnscannedprot
* Description: 	When user selects any row in LIST_UNSCANNEDPROT
*               box index of that row is
*               stored in variable.
* Parameter: 	NMHDR * pNMHDR :
* Parameter: 	LRESULT * pResult :
* Returns:   	void
*************************************************************/
void CPlanDuplicatorDlg::OnRowClkListUnscannedprot(NMHDR* pNMHDR, LRESULT* pResult) 
{
    PD_TRACE(CPlanDuplicatorDlg::OnRowClkListUnscannedprot);
    
    if ((NULL == pNMHDR) || (NULL == pResult))
    {
        CString l_cserr (_T("OnRowClkListUnscannedprot NULL parameter"));
        CPDLogHelper::WriteToErrorLog(l_cserr,__LINE__);
        return;
    }else{
        NMLISTVIEW* l_plist = (NMLISTVIEW*)pNMHDR;
        m_ncurrunscannedrowno = l_plist->iItem;
        *pResult = 0;
    }
    
    CString l_plane_name(_T(""));
    l_plane_name =  m_listctrl_data.GetItemText(m_ncurrrowno,0);
    
    if (SMART_VOI == m_controller->GetMPRorVOIMode())
    {
        if (MANUAL == m_nmode)
        {//Mpower 047/Sep-2012/AN/Added
            if ((0 == l_plane_name.Compare(_T(SLICE_PLANE_SCOUT2))) && (m_bisCalculateDone))  //if Scout2 slice plane is selected and Calculate already done,Update should not be disabled
            {
                m_UpdateSeqButton.EnableWindow(TRUE); 
            } else {
			//Mpower 047/Sep-2012/AN/Added :End
                
                m_UpdateSeqButton.EnableWindow(FALSE);
            }
        }
    }
}

/*************************************************************
* Method:    	RunAutoUpdate
* Description: 	This function is called when user checks
*               autoUpdate and clicks on updatesequence.
* Parameter: 	int f_unScannedProt :
* Returns:   	int
*************************************************************/
int CPlanDuplicatorDlg::RunAutoUpdate(int f_unScannedProt[])
{
    PD_TRACE(CPlanDuplicatorDlg::RunAutoUpdate);
    int l_status = E_NO_ERROR;
	//Mpower 039//Jul-12/Modified/AN/Start
    int l_ntotal_slice_plane = m_listctrl_data.GetItemCount();
    
    if (SMART_VOI == m_controller->GetMPRorVOIMode()){
        m_controller->ClearCalcData();
    }
    int l_ncurrrowno = 0;
    for(l_ncurrrowno = 1; l_ncurrrowno < l_ntotal_slice_plane; l_ncurrrowno++) { //Start from "map", instead of "scout2"
        m_fcParams.sel_protocol_index_value[0] = f_unScannedProt[l_ncurrrowno - 1];
        if (SMART_VOI == m_controller->GetMPRorVOIMode())
        {   
            m_ncurrrowno = l_ncurrrowno;
            CalculateSelect(/*l_ncurrrowno*/);
            l_status = m_controller->CalculateParams(l_ncurrrowno,m_fcParams);
        }
        m_listctrl_data.SetItemState(m_ncurrrowno,LVIS_SELECTED,LVIS_SELECTED | LVIS_FOCUSED);
        m_listctrl_data.EnsureVisible(m_ncurrrowno,FALSE);
        if (E_NO_ERROR == l_status)
        {
            l_status = m_controller->Updatefields(/*l_mode,*/l_ncurrrowno,m_fcParams,0);
        } else {
            l_status = E_ERROR;
            CString l_cserrmsg = _T("Calculation error. Cannot update sequence");        //ErrorList[PD_CALC_ERROR];
            CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
        }
        CString l_Vfstr = m_controller->GetPDDataHandle()->GetVantageSlicePlane(l_ncurrrowno);
        showStatus(m_fcParams.sel_protocol_index_value[0],l_Vfstr,l_status);
        //  if (!l_status){
        if (E_NO_ERROR == l_status) {
            CreateProtocolIndexString(m_fcParams.sel_protocol_index_value[0]);
        }
    }
    m_ncurrrowno = 1;
		//Mpower 039//Jul-12/Modified/AN/End
    return(l_status);
}

/*************************************************************
* Method:    	showStatus
* Description: 	This function display the status of updated protocol.
* Parameter: 	int & f_protno :
* Parameter: 	char * f_slicePlane :
* Parameter: 	int & f_flag :
* Returns:   	int
*************************************************************/
int CPlanDuplicatorDlg::showStatus(int &f_protno,const CString  &f_slicePlane,int &f_flag)
{
    PD_TRACE(CPlanDuplicatorDlg::showStatus);
    CString l_cstrEdit(_T(""));
    CString l_strProt(_T(""));
    bool l_flag= FALSE;
    int l_protocol=0;
    int i = 0;
    for(i = 0;i < m_ntotal_protocol;i++){
        
        if(f_protno == m_p_data[i].protocolNum){
            l_protocol = m_p_data[i].protocol;
            l_strProt.Format(_T("%d"),l_protocol);
            break;
        }
    }
    
    int l_ni = m_listctrl_status.GetItemCount();
    int j = 0;
    for(j = 0;j < l_ni;j++){
        l_cstrEdit = m_listctrl_status.GetItemText(j,0);
        
        if(0 == l_cstrEdit.Compare(l_strProt)){
            l_flag = TRUE;
            break;
        }
    }
    
    if (TRUE == l_flag){
        CString l_str2(f_slicePlane);
        //l_str2.Delete(0,11);    
        l_cstrEdit.Format(_T("%d"),l_protocol);
        m_listctrl_status.SetItemText(j,0,l_cstrEdit);
        //   m_listctrl_status.SetItemText(j,0,l_cstrEdit);      
        m_listctrl_status.SetItemText(j,1,l_str2);
        if(TRUE == m_radio_smart_mpr.GetCheck()){           //i.e if mode is SmartMPR mode
            
            if(E_NO_ERROR == f_flag){
                l_cstrEdit.Format(_T("Done"));
                m_listctrl_status.SetItemText(j,2,l_cstrEdit);
            }else{
                l_cstrEdit.Format(_T("Fail"));
                m_listctrl_status.SetItemText(j,2,l_cstrEdit);
            }
        }
			//Mpower 039//Jul-12/Added/AS
        else{                                               //i.e if mode is SmartVOI mode
            switch(m_fcParams.selectedAxis)//Axis
            {
            case AXIAL://Axial
                l_cstrEdit.Format(_T("Axial"));
                m_listctrl_status.SetItemText(j,2,l_cstrEdit);
                break;
            case CORONAL://Coronal
                l_cstrEdit.Format(_T("Coronal"));
                m_listctrl_status.SetItemText(j,2,l_cstrEdit); 
                break;
            case SAGITTAL://Sagittal
                l_cstrEdit.Format(_T("Sagittal"));
                m_listctrl_status.SetItemText(j,2,l_cstrEdit);
                break;
            default:
                l_cstrEdit.Format(_T("None"));
                m_listctrl_status.SetItemText(j,2,l_cstrEdit);
                break;
            }
            
            switch(m_fcParams.selectedParam)//Parameter
            {
            case SEL_SLICE_NUM: //Slice Number
                l_cstrEdit.Format(_T("Slice Number"));
                m_listctrl_status.SetItemText(j,3,l_cstrEdit);
                break;
            case SEL_SLICE_GAP: //Slice Gap
                l_cstrEdit.Format(_T("Slice Gap"));
                m_listctrl_status.SetItemText(j,3,l_cstrEdit);
                break;
            case SEL_SLICE_THICKNESS://Slice Thickness
                l_cstrEdit.Format(_T("Slice Thickness"));
                m_listctrl_status.SetItemText(j,3,l_cstrEdit);
                break;
            default:
                l_cstrEdit.Format(_T("None"));
                m_listctrl_status.SetItemText(j,3,l_cstrEdit);
                break;
            }
            
            switch(m_nmode)
            {
            case 0://Auto Couch
                l_cstrEdit.Format(_T("Auto Couch"));
                m_listctrl_status.SetItemText(j,4,l_cstrEdit);
                break;
            case 1: //Auto Plan
                l_cstrEdit.Format(_T("Auto Plan"));
                m_listctrl_status.SetItemText(j,4,l_cstrEdit);
                break;
            case 2://Manual
                l_cstrEdit.Format(_T("Manual"));
                m_listctrl_status.SetItemText(j,4,l_cstrEdit);
                break;
            default:
                l_cstrEdit.Format(_T("None"));
                m_listctrl_status.SetItemText(j,4,l_cstrEdit);
                break;
            }
            
            
            if(E_NO_ERROR == f_flag){
                l_cstrEdit.Format(_T("Done"));
                m_listctrl_status.SetItemText(j,5,l_cstrEdit);
            }else{
                l_cstrEdit.Format(_T("Fail"));
                m_listctrl_status.SetItemText(j,5,l_cstrEdit);
            }
            
        }
        //AS ends
        l_flag=FALSE;
    }else{
        l_cstrEdit.Format(_T("%d"),l_protocol);
        CString l_str2(f_slicePlane);       
        //l_str2.Delete(0,11);
        m_listctrl_status.InsertItem(l_ni,NULL);
        m_listctrl_status.SetItemText(l_ni,0,l_cstrEdit);       
        m_listctrl_status.SetItemText(l_ni,1,l_str2);
        
        //AS added
        if(TRUE == m_radio_smart_mpr.GetCheck()){           //i.e if mode is SmartMPR mode
            
            if(E_NO_ERROR == f_flag){
                l_cstrEdit.Format(_T("Done"));
                m_listctrl_status.SetItemText(j,2,l_cstrEdit);
            }else{
                l_cstrEdit.Format(_T("Fail"));
                m_listctrl_status.SetItemText(j,2,l_cstrEdit);
            }
        }
			//Mpower 039//Jul-12/Added/AS
        else{                                               //i.e if mode is SmartVOI mode
            switch(m_fcParams.selectedAxis)//Axis
            {
            case AXIAL://Axial
                l_cstrEdit.Format(_T("Axial"));
                m_listctrl_status.SetItemText(j,2,l_cstrEdit);
                break;
            case CORONAL://Coronal
                l_cstrEdit.Format(_T("Coronal"));
                m_listctrl_status.SetItemText(j,2,l_cstrEdit); 
                break;
            case SAGITTAL://Sagittal
                l_cstrEdit.Format(_T("Sagittal"));
                m_listctrl_status.SetItemText(j,2,l_cstrEdit);
                break;
            default:
                l_cstrEdit.Format(_T("None"));
                m_listctrl_status.SetItemText(j,2,l_cstrEdit);
                break;
            }
            
            switch(m_fcParams.selectedParam)//Parameter
            {
            case SEL_SLICE_NUM: //Slice Number
                l_cstrEdit.Format(_T("Slice Number"));
                m_listctrl_status.SetItemText(j,3,l_cstrEdit);
                break;
            case SEL_SLICE_GAP: //Slice Gap
                l_cstrEdit.Format(_T("Slice Gap"));
                m_listctrl_status.SetItemText(j,3,l_cstrEdit);
                break;
            case SEL_SLICE_THICKNESS://Slice Thickness
                l_cstrEdit.Format(_T("Slice Thickness"));
                m_listctrl_status.SetItemText(j,3,l_cstrEdit);
                break;
            default:
                l_cstrEdit.Format(_T("None"));
                m_listctrl_status.SetItemText(j,3,l_cstrEdit);
                break;
            }
            
            switch(m_nmode)
            {
            case 0://Auto Couch
                l_cstrEdit.Format(_T("Auto Couch"));
                m_listctrl_status.SetItemText(j,4,l_cstrEdit);
                break;
            case 1: //Auto Plan
                l_cstrEdit.Format(_T("Auto Plan"));
                m_listctrl_status.SetItemText(j,4,l_cstrEdit);
                break;
            case 2://Manual
                l_cstrEdit.Format(_T("Manual"));
                m_listctrl_status.SetItemText(j,4,l_cstrEdit);
                break;
            default:
                l_cstrEdit.Format(_T("None"));
                m_listctrl_status.SetItemText(j,4,l_cstrEdit);
                break;
            }
            
            if(E_NO_ERROR == f_flag){
                l_cstrEdit.Format(_T("Done"));
                m_listctrl_status.SetItemText(j,5,l_cstrEdit);
            }else{
                l_cstrEdit.Format(_T("Fail"));
                m_listctrl_status.SetItemText(j,5,l_cstrEdit);
            }
        }
        //AS ends   
    }
    return(E_NO_ERROR);   
}


/*************************************************************
* Method:    	OnButtonSmartmpr
* Description:  Invokes the SmartMPR
* Returns:   	void
*************************************************************/
void CPlanDuplicatorDlg::OnButtonSmartmpr() 
{
    PD_TRACE(CPlanDuplicatorDlg::OnButtonSmartmpr);
    if (NULL != m_controller)
    { int l_nstatus = m_controller->InvokeSmartMPRVoi(/*SMART_MPR*/);//Mpower 007/AN/Modified
        if(E_NO_ERROR != l_nstatus)
        {
            CString l_cserr (_T("Error invoking SmartMPR.exe/SmartVOI.exe"));
            CPDLogHelper::WriteToErrorLog(l_cserr,__LINE__);
        }
    }  
    else
    {
        CString l_cserr (_T("OnButtonSmartmpr controller NULL"));
        CPDLogHelper::WriteToErrorLog(l_cserr,__LINE__);
    }
}

/*************************************************************
* Method:    	OnCancel
* Description: 	On check of Auto Update check box it disables 
*               the selection of listctrl and on uncheck of
*               it enables selection.
* Returns:   	void
*************************************************************/
void CPlanDuplicatorDlg::OnCancel() 
{
    PD_TRACE(CPlanDuplicatorDlg::OnCancel);
    if (NULL != m_controller)
    {
        m_controller->Close();
    } 
    else
    {
        CString l_cserr (_T("OnCancel controller NULL"));
        CPDLogHelper::WriteToErrorLog(l_cserr,__LINE__);
    }
    
    DialogEx::OnCancel();
}

/*************************************************************
* Method:    	CreateProtocolIndexString
* Description: 	Creating a CString containing selected
*               unscanned protocols
* Parameter: 	const int & f_index :
* Returns:   	void
*************************************************************/
void CPlanDuplicatorDlg::CreateProtocolIndexString(const int &f_index)
{
    PD_TRACE(CPlanDuplicatorDlg::CreateProtocolIndexString);
    CString l_protocol_index;
    l_protocol_index.Format(_T("%d"),(int)(f_index));
    
    if (TRUE == m_prot_index_string.IsEmpty())
        m_prot_index_string += l_protocol_index;
    else
        m_prot_index_string += ',' + l_protocol_index;
}

/*************************************************************
* Method:    	OnButtonConvert
* Description: 	Convert the study file data to DICOM files
* Returns:   	void
*************************************************************/
void CPlanDuplicatorDlg::OnButtonConvert() 
{
    PD_TRACE(CPlanDuplicatorDlg::OnButtonConvert);
    char *l_studyname = NULL;
    status_t l_status = E_NO_ERROR;
  //  BOOL l_bIniFile = TRUE;
  //  CString l_csErrorStr (_T(""));
    CString l_Dicom_filepath (_T(""));
    CString l_Reading_Mode(_T(""));
  //  int l_nIniErrorCode = 0;
  //  int prot_index = 0;
 //   CString l_error (_T(""));
    try
    {
        l_studyname = new char[BUFFER_SIZE_128]; 
        
        if(NULL == l_studyname){
            CString l_csErrMsg = ErrorList[PD_MEMORY_ALLOCATION];
            CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__); //Mpower 040/2012-08-09/AS/Modified
            return;
        }
        
        int l_protidx = -1;
        CString l_Study_filepath;
        AfxGetApp()->DoWaitCursor(1); // 1->>display the hourglass cursor  
        
        //vilas.rev..code below uses controller extensively so code can be shifted to controller
        //last else block using list control can be kept here and called from controller
        if(m_radio_smart_mpr.GetCheck()) {
            l_Dicom_filepath= m_controller->GetIniFileReader()->GetMPRDicomFilePath();
            if (l_Dicom_filepath.IsEmpty())
            {
                l_Dicom_filepath = SMARTMPR_DICOM_PATH;
            }
        } else {
            l_Dicom_filepath = m_controller->GetIniFileReader()->GetVOIDicomFilePath(); //VOI
            if (l_Dicom_filepath.IsEmpty())
            {
                l_Dicom_filepath = SMARTVOI_DICOM_PATH;
            }
        }  
        
        m_controller->RemoveDirectoryRecursive(l_Dicom_filepath);
        l_Reading_Mode = m_controller->GetIniFileReader()->GetMPRReadingMode();
        
        if (l_Reading_Mode.CompareNoCase(_T("MANUAL")) == 0){
            l_Study_filepath = m_controller->GetIniFileReader()->GetMPRStudyFilePath();
            l_protidx = m_controller->GetIniFileReader()->GetMPRProtocolIndex();
            wcstombs(l_studyname,(LPCTSTR) l_Study_filepath, BUFFER_SIZE_128);
//             if (0 == m_nScannedtotal_protocol)
//             {
//                 CString l_csErrMsg = ErrorList[PD_NO_SCANNED_PROTOCOL];
//                 CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__,CUSTOM_ERRORDIALOG_DISPLAY);
//                 return;
//             }
//             if (0 == m_listctrl_scannedProtocol.GetSelectedCount()){
//                 CString l_csErrMsg = ErrorList[PD_SELECT_SCANNED_PROTOCOL];
//                 CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__,CUSTOM_ERRORDIALOG_DISPLAY);
//                 return;
//             }

        } else {
            if (0 == m_nScannedtotal_protocol)
            {
                int l_nerror_code = PD_NO_SCANNED_PROTOCOL;
                CString l_csErrMsg = ErrorList[l_nerror_code];
                CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__,CUSTOM_ERRORDIALOG_DISPLAY);
              
                if(NULL != l_studyname){    //Mpower 040/2012-08-09/AS/Added
                    delete []l_studyname; 
                    l_studyname = NULL;
                }
                return;
            }
            
            
            if (0 == m_listctrl_scannedProtocol.GetSelectedCount()){
                CString l_csErrMsg = ErrorList[PD_SELECT_SCANNED_PROTOCOL];
                CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__,CUSTOM_ERRORDIALOG_DISPLAY);
                return;
            }else{// do some lengthy processing 
                CString l_scanned_proto;            
                l_scanned_proto = m_listctrl_scannedProtocol.GetItemText(m_ncurrscannedrowno,0);    
                char *l_cpC = new char[BUFFER_SIZE_20]; //AN // buffer size changed from 150 to 20, for reading scanned protocol index
                
                if(NULL == l_cpC){
                    if(NULL != l_studyname){    //Mpower 040/2012-08-09/AS/Added
                        delete []l_studyname; 
                        l_studyname = NULL;
                }
                    return;
                }
                
                wcstombs(l_cpC,(LPCTSTR) l_scanned_proto, 20); //AN // size changed from 100 to 20
                int l_nIndex=(int)atoi(l_cpC);
                int l_nprotIndex = -1;
                int i = 0;
                for(i = 0;i < m_nScannedtotal_protocol;i++){
                    if(l_nIndex == m_p_data_scanned[i].protocol){
                        l_nprotIndex = m_p_data_scanned[i].protocolNum;
                        break;
                    }               
                }
                
                l_protidx = l_nprotIndex ;
                // l_studyname = m_fcParams.studyfilename;
                memcpy(l_studyname,m_fcParams.studyfilename,BUFFER_SIZE_128);
                
                if(NULL != l_cpC){
                    delete []l_cpC; //Mpower 030/2012-06-26/AS/Modified/cpp change
                    l_cpC = NULL;
                }
            }   
        }
        
        l_status = m_controller->ConvertDicom(l_protidx,l_Dicom_filepath,l_studyname);
        if(E_ERROR == l_status)
        {
            CString l_cserr (_T("Dicom Conversion failed"));
            CPDLogHelper::WriteToErrorLog(l_cserr,__LINE__,CUSTOM_ERRORDIALOG_DISPLAY);
            //  int l_error_code = l_DcmVarfieldInfo.GetErrorCode();
            //  CString l_Str(ErrorList[l_error_code]);     
            //  AfxMessageBox(l_Str);
        }
        
        AfxGetApp()->DoWaitCursor(-1); // -1->>remove the hourglass cursor
    }
    catch (...)
    {
        CString l_cserrmsg (_T("Exception occured during Dicom Convert"));
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
    }
    
    if(NULL != l_studyname){
        delete []l_studyname; //Mpower 030/2012-06-26/AS/Modified/cpp change
        l_studyname = NULL;
    }
}

/*************************************************************
* Method:    	OnClickListctrlScannedprot
* Description: 	This function is used 
* Parameter: 	NMHDR * pNMHDR :
* Parameter: 	LRESULT * pResult :
* Returns:   	void
*************************************************************/
void CPlanDuplicatorDlg::OnClickListctrlScannedprot(NMHDR* pNMHDR, LRESULT* pResult) 
{
    PD_TRACE(CPlanDuplicatorDlg::OnClickListctrlScannedprot);
    
    if ((NULL == pNMHDR) || (NULL == pResult)){
        CString l_cserrmsg = ErrorList[PD_PARAMETER_INVALID];
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__); //AN
    }else{
        NMLISTVIEW* l_plist = (NMLISTVIEW*)pNMHDR;
        m_ncurrscannedrowno = l_plist->iItem;
        *pResult = 0;
    }
}

/*************************************************************
* Method:    	OnButtonCalculate
* Description: 	This function is used 
* Returns:   	void
*************************************************************/
void CPlanDuplicatorDlg::OnButtonCalculate() 
{
    PD_TRACE(CPlanDuplicatorDlg::OnButtonCalculate);
    CString l_listUnScannedData(_T(""));    
    int l_nprotIndex = 0;
    bool l_calculate_flag = FALSE;
    CString l_plane_name(_T(""));
    
    if (-1 == m_ncurrrowno){
        CString l_csErrMsg = ErrorList[PD_SELECT_SLICEPLAN];
        CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__,CUSTOM_ERRORDIALOG_DISPLAY);
        return;
    }
    
    l_plane_name = m_listctrl_data.GetItemText(m_ncurrrowno,0);
    int l_slice_plan_sel=0;
    l_slice_plan_sel = m_listctrl_UnscannedProtocol.GetSelectedCount();
    if (0 == l_plane_name.Compare(_T(SLICE_PLANE_MAP)) || 0 == l_plane_name.Compare(_T(SLICE_PLANE_SHIMMING)) || 0 == l_plane_name.Compare(_T(SLICE_PLANE_MULTISLICE))){  
       
        if (0 == l_slice_plan_sel){
            CString l_csErrMsg = ErrorList[PD_SELECT_UNSCANNED_PROTOCOL];
            CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__,CUSTOM_ERRORDIALOG_DISPLAY);
            return;
        }
    }
    //AN/Commenetd
//     if (0 > m_ncurrunscannedrowno) {
//         CString l_csErrMsg = ErrorList[PD_SELECT_UNSCANNED_PROTOCOL];
//         CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__,CUSTOM_ERRORDIALOG_DISPLAY);
//         return; 
//     }
    
    l_listUnScannedData = m_listctrl_UnscannedProtocol.GetItemText(m_ncurrunscannedrowno,0);
    char *l_cpC = NULL;
    l_cpC = new char[BUFFER_SIZE_160];
    
    if(NULL == l_cpC){
        CString l_cserrmsg = ErrorList[PD_MEMORY_ALLOCATION];
        CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__,ERRORVIEW_DISPLAY); //AN
        return;
    }
    
    wcstombs(l_cpC,(LPCTSTR) l_listUnScannedData, BUFFER_SIZE_160);
    int l_nIndex=(int)atoi(l_cpC);
    
    if(NULL != l_cpC){       //Mpower 040/2012-08-09/AS/Added
        delete []l_cpC;
        l_cpC = NULL;
    }

    int i = 0;
    for(i = 0;i < m_ntotal_protocol;i++){
        if(l_nIndex == m_p_data[i].protocol){
            l_nprotIndex = m_p_data[i].protocolNum;
            break;
        }
    }
    int l_ntotal_slice_plan = m_listctrl_data.GetItemCount();
    
    CalculateSelect(/*m_ncurrrowno*/);
    m_fcParams.sel_protocol_index_value[0] = l_nprotIndex;
    m_controller->ClearCalcData();
    int l_nstatus = m_controller->CalculateParams(m_ncurrrowno,m_fcParams);
        
    if (E_NO_ERROR == l_nstatus){              //AS modified
        l_calculate_flag = TRUE;
        m_bisCalculateDone = TRUE; //Mpower 047/Sep-2012/AN/Added
            m_UpdateSeqButton.EnableWindow(TRUE);
			//Mpower 047/Sep-2012/AN/Added :Start
            if (0 == l_plane_name.Compare(_T(SLICE_PLANE_SCOUT2)))
            {
                 m_bisCalculateDone = TRUE;

            } else
            {
                m_bisCalculateDone = FALSE;

            }
			//Mpower 047/Sep-2012/AN/Added :End
       
    }else{
        m_bisCalculateDone = FALSE;
        m_UpdateSeqButton.EnableWindow(FALSE);
        CString l_csErrMsg = ErrorList[PD_CALC_ERROR];
        CPDLogHelper::WriteToErrorLog(l_csErrMsg,__LINE__,ERROR_LOGONLY);
    }
    ShowData(l_calculate_flag);
}

/*************************************************************
* Method:       DisableEnableRadioBtns
* Description:  Disables/Enables the radio buttons(under Axis, Mode, Parameter frame) as data 
//              is yet not displayed on the Vantage/DICOM axis 
* Parameter:    bool f_true_or_false : TRUE / FALSE
* Returns:      void
*************************************************************/
void CPlanDuplicatorDlg::DisableEnableRadioBtns(bool f_true_or_false)
{
    PD_TRACE(CPlanDuplicatorDlg::DisableEnableRadioBtns);
    CWnd *l_auto_move = NULL;
    
    m_radio_axial.EnableWindow(f_true_or_false);
    l_auto_move= GetDlgItem(IDC_RADIO_CORONAL);
    l_auto_move->EnableWindow(f_true_or_false);
    l_auto_move= GetDlgItem(IDC_RADIO_SAGITTAL);
    l_auto_move->EnableWindow(f_true_or_false);
    
    m_radio_slice_number.EnableWindow(f_true_or_false);
    l_auto_move= GetDlgItem(IDC_RADIO_SLICE_GAP);
    l_auto_move->EnableWindow(f_true_or_false);
    l_auto_move= GetDlgItem(IDC_RADIO_SLICE_THICKNESS);
    l_auto_move->EnableWindow(f_true_or_false);
    
    m_radio_auto_couch.EnableWindow(f_true_or_false);
    l_auto_move= GetDlgItem(IDC_RADIO_AUTO_PLAN);
    l_auto_move->EnableWindow(f_true_or_false);
    l_auto_move= GetDlgItem(IDC_RADIO_MANUAL);
    l_auto_move->EnableWindow(f_true_or_false);
}

/*************************************************************
* Method:       UncheckAll
* Description:  This function is used remove all selections
*               and displays default status
* Returns:      void
*************************************************************/
void CPlanDuplicatorDlg::UncheckAll(void) 
{
    PD_TRACE(CPlanDuplicatorDlg::UncheckAll);
    RadioButtonEx *l_auto_move = NULL;
    
    l_auto_move= (RadioButtonEx*)GetDlgItem(IDC_RADIO_AUTO_PLAN);
    l_auto_move->SetCheck(0);
    l_auto_move= (RadioButtonEx*)GetDlgItem(IDC_RADIO_MANUAL);
    l_auto_move->SetCheck(0);
    
    l_auto_move= (RadioButtonEx*)GetDlgItem(IDC_RADIO_SLICE_GAP);
    l_auto_move->SetCheck(0);
    l_auto_move= (RadioButtonEx*)GetDlgItem(IDC_RADIO_SLICE_THICKNESS);
    l_auto_move->SetCheck(0);
    
    l_auto_move= (RadioButtonEx*)GetDlgItem(IDC_RADIO_CORONAL);
    l_auto_move->SetCheck(0);
    l_auto_move= (RadioButtonEx*)GetDlgItem(IDC_RADIO_SAGITTAL);
    l_auto_move->SetCheck(0);
    
    m_radio_auto_couch.SetCheck(0);
    m_radio_axial.SetCheck(0);
    m_radio_slice_number.SetCheck(0);
    
    m_nmode = -1;
    m_naxis = -1;
    m_nparameter = -1;
    
    m_btn_calculate.EnableWindow(FALSE);
    m_UpdateSeqButton.EnableWindow(FALSE);
}

/*************************************************************
* Method:       CheckMain
* Description:  This function is used to default selection for VOI mode
* Returns:      void
*************************************************************/
void CPlanDuplicatorDlg::CheckMain(void) 
{
    PD_TRACE(CPlanDuplicatorDlg::CheckMain);
    m_radio_auto_couch.SetCheck(1);
    m_nmode = AUTO_COUCH;
    m_radio_axial.SetCheck(1);
    m_naxis = AXIAL;
    m_radio_slice_number.SetCheck(1);
    m_nparameter = 0;
}

/*************************************************************
* Method:       OnRadioSmartmpr
* Description:  Clears the listbox Vantage Axis and disables the radio buttons
*              (under Axis, Mode, Parameter frame). 
* Returns:      void
*************************************************************/
void CPlanDuplicatorDlg::OnRadioSmartmpr() 
{
    PD_TRACE(CPlanDuplicatorDlg::OnRadioSmartmpr);
    // m_controller->SetVOI_or_MPR(1);
    CEdit* l_pName = (CEdit*)GetDlgItem(IDC_EDIT_TEXTFILEPATH); 
    CString l_strFileName (_T(""));
    m_bisCalculateDone = FALSE;//AN/Added
    m_check_DicomAxis.EnableWindow(TRUE);
    m_listctrl_dataDicomAxis.EnableWindow(TRUE);
    m_listctrl_dataDicomAxis.DeleteAllItems();
    m_listctrl_data.EnableWindow(TRUE);
    m_listctrl_data.DeleteAllItems();   
    
    InitListColumnForMPR();
    DisableEnableRadioBtns(FALSE);
    UncheckAll();
    m_SmartMPRButton.SetWindowText(_T("Smart MPR"));
    CIniFileReader * l_ini_reader = NULL;
    l_ini_reader =  m_controller->GetIniFileReader(); 
    
    if (NULL != l_ini_reader)
    {           
        l_strFileName = l_ini_reader->GetMPRFilePath();
    }
    if (TRUE == l_strFileName.IsEmpty())
    {
        l_strFileName = SMARTMPR_FILE_PATH;     
    }
    
    l_pName->SetWindowText(l_strFileName);
    m_controller->SetAppMode(SMART_MPR);
    m_btn_calculate.EnableWindow(FALSE);
    m_UpdateSeqButton.SetWindowText(_T("Update sequence"));
}

/*************************************************************
* Method:    	OnRadioSmartvoi
* Description: Clears the listbox Vantage Axis and disables
*              the radio buttons
*              (under Axis, Mode, Parameter frame).
* Returns:   	void
*************************************************************/
void CPlanDuplicatorDlg::OnRadioSmartvoi() 
{
    PD_TRACE(CPlanDuplicatorDlg::OnRadioSmartvoi);
    //    m_controller->SetVOI_or_MPR(0);
    CEdit* l_pName = (CEdit*)GetDlgItem(IDC_EDIT_TEXTFILEPATH); 
    CString l_strFileName;
    m_bisCalculateDone = FALSE; //AN/Added
    m_check_DicomAxis.EnableWindow(FALSE);
    m_listctrl_data.EnableWindow(TRUE);
    m_listctrl_dataDicomAxis.EnableWindow(FALSE);
    m_listctrl_data.DeleteAllItems();
    m_listctrl_dataDicomAxis.DeleteAllItems();  
    InitListColumnForVOI();
    DisableEnableRadioBtns(FALSE);
    UncheckAll();
    m_SmartMPRButton.SetWindowText(_T("Smart VOI"));
    CIniFileReader * l_ini_reader = NULL;
    l_ini_reader =  m_controller->GetIniFileReader(); 
    
    if (NULL != l_ini_reader)
    {           
        l_strFileName = l_ini_reader->GetVOIFilePath();
    }
    if (TRUE == l_strFileName.IsEmpty())
    {
        l_strFileName = SMARTVOI_FILE_PATH;     
    }  
    l_pName->SetWindowText(l_strFileName);  
    m_controller->SetAppMode(SMART_VOI);
    
    m_UpdateSeqButton.EnableWindow(FALSE);
    m_UpdateSeqButton.SetWindowText(_T("Calculate && Setup"));
    //AS added to hide the dicom axis list box
    if(TRUE == m_check_DicomAxis.GetCheck())
    {   
        WINDOWPLACEMENT l_wndpl;
        GetWindowPlacement( &l_wndpl );
        l_wndpl.rcNormalPosition.bottom -= 150;
        SetWindowPlacement( &l_wndpl );
        int l_noffset = -150;
        GetDlgItem(IDC_LIST_DICOMAXIS)->ShowWindow(FALSE);
        ControlsMove(l_noffset);        
        CRect l_ContorlRect;
        MoveControlsUpDown(-1,m_listctrl_scannedProtocol,l_ContorlRect); 
        m_check_DicomAxis.SetCheck(FALSE);
    }
    //AS ends
     
}

/*************************************************************
* Method:       OnRadioAutoCouch
* Description:  Callback function for radio button, it enables / disables parameter selection
* Returns:      void
*************************************************************/
void CPlanDuplicatorDlg::OnRadioAutoCouch() 
{
    PD_TRACE(CPlanDuplicatorDlg::OnRadioAutoCouch);
    CWnd *l_auto_move_state = NULL;
    RadioButtonEx *l_auto_move = NULL;
    m_nmode = AUTO_COUCH;
    m_fcParams.isScoutSelected = 1;
    
    l_auto_move= (RadioButtonEx*)GetDlgItem(IDC_RADIO_SLICE_GAP);
    l_auto_move->SetCheck(0);
    l_auto_move= (RadioButtonEx*)GetDlgItem(IDC_RADIO_SLICE_THICKNESS);
    l_auto_move->SetCheck(0);
    m_UpdateSeqButton.SetWindowText(_T("Calculate && Setup"));
    m_btn_calculate.EnableWindow(FALSE);
    m_UpdateSeqButton.EnableWindow(TRUE);
    
    m_radio_slice_number.EnableWindow(FALSE);
    l_auto_move_state= GetDlgItem(IDC_RADIO_SLICE_GAP);
    l_auto_move_state->EnableWindow(FALSE);
    l_auto_move_state= GetDlgItem(IDC_RADIO_SLICE_THICKNESS);
    l_auto_move_state->EnableWindow(FALSE);
    
    m_radio_slice_number.SetCheck(0);
    //    m_nparameter = -1;
    //    m_listctrl_data.EnableWindow(FALSE);
    m_listctrl_data.SetItemState(0, LVIS_SELECTED , LVIS_SELECTED | LVIS_FOCUSED);
    m_listctrl_data.EnsureVisible(0,FALSE);
    
}


/*************************************************************
* Method:    	OnRadioAutoPlan
* Description: 	This function is used 
* Returns:   	void
*************************************************************/
void CPlanDuplicatorDlg::OnRadioAutoPlan() 
{
    PD_TRACE(CPlanDuplicatorDlg::OnRadioAutoPlan);
    CWnd *l_auto_move_state = NULL;
    RadioButtonEx *l_auto_move = NULL;
    m_nmode = AUTO_PLAN;
    
    l_auto_move= (RadioButtonEx*)GetDlgItem(IDC_RADIO_SLICE_GAP);
    l_auto_move->SetCheck(0);
    l_auto_move= (RadioButtonEx*)GetDlgItem(IDC_RADIO_SLICE_THICKNESS);
    l_auto_move->SetCheck(0);
    
    m_radio_slice_number.EnableWindow(FALSE);
    l_auto_move_state= GetDlgItem(IDC_RADIO_SLICE_GAP);
    l_auto_move_state->EnableWindow(FALSE);
    l_auto_move_state= GetDlgItem(IDC_RADIO_SLICE_THICKNESS);
    l_auto_move_state->EnableWindow(FALSE);
    
    if(FALSE == m_radio_smart_mpr.GetCheck()){
        m_UpdateSeqButton.SetWindowText(_T("Calculate && Setup"));
    }
    
    m_UpdateSeqButton.EnableWindow(TRUE);
    m_btn_calculate.EnableWindow(FALSE);
    
    m_radio_slice_number.SetCheck(0);
    //m_nparameter = 1;
    // m_listctrl_data.EnableWindow(FALSE);
    if(FALSE == m_radio_smart_mpr.GetCheck()){
        m_listctrl_data.SetItemState(1, LVIS_SELECTED , LVIS_SELECTED | LVIS_FOCUSED);
        m_listctrl_data.EnsureVisible(1,FALSE);
    } else 
    {
        m_listctrl_data.SetItemState(0, LVIS_SELECTED , LVIS_SELECTED | LVIS_FOCUSED);
        m_listctrl_data.EnsureVisible(0,FALSE);
    }
    
}

/*************************************************************
* Method:    	OnRadioManual
* Description: 	This function is used 
* Returns:   	void
*************************************************************/
void CPlanDuplicatorDlg::OnRadioManual() 
{
    PD_TRACE(CPlanDuplicatorDlg::OnRadioManual);
    m_listctrl_data.EnableWindow(TRUE);
    m_nmode = MANUAL;
    m_ncurrrowno = -1;
    if(SMART_VOI == m_controller->GetMPRorVOIMode())
    {
        m_UpdateSeqButton.EnableWindow(FALSE);
        m_UpdateSeqButton.SetWindowText(_T("Update Sequence"));
        m_btn_calculate.EnableWindow(TRUE);
    }
    else
    {
        m_btn_calculate.EnableWindow(FALSE);
        m_UpdateSeqButton.EnableWindow(TRUE);
    }
    m_listctrl_data.SetItemState(-1, 0 , LVIS_SELECTED | LVIS_FOCUSED);
    
}

/*************************************************************
* Method:       ShowVOIData
* Description:  This function is used to display VarfieldData
*               & DicomData for MPR mode
* Returns:      void
*************************************************************/
void CPlanDuplicatorDlg::ShowVOIData(bool f_ncalculate_flag)
{
    PD_TRACE(CPlanDuplicatorDlg::ShowVOIData);
    f32vec3_t l_Vfdata1 = {0};
    f32vec3_t l_Vfdata2 = {0};
    f32vec2_t l_fov = {0};
    float l_fslice_thickness = 0.0f;
    float l_fslice_gap = 0.0f;
    float l_fcouch_offset = 0.0f;
    int l_nslice_num;
    
    f32vec3_t l_Dcdata1 = {0};
    f32vec3_t l_Dcdata2 = {0};
    int status = 0;
    int size = 0;
    CString l_plane_name(_T(""));
    size = m_controller->GetPDDataHandle()->GetDataSize();
    int l_ni = 0;
    CPDCoordinates * l_pd_coordinates = NULL;
    for(l_ni = 0;l_ni < size; l_ni++){               
        l_Vfdata1 = m_controller->GetPDDataHandle()->GetVantageData(l_ni,COORD1);
        l_Vfdata2 = m_controller->GetPDDataHandle()->GetVantageData(l_ni,COORD2);
        if(TRUE == f_ncalculate_flag){
            l_pd_coordinates = m_controller->GetPDDataHandle()->GetVantageData(l_ni);
            if (NULL != l_pd_coordinates)
            {
                l_pd_coordinates->GetSliceAttributes(SLICE_THICKNESS,(char *)&l_fslice_thickness);
                l_pd_coordinates->GetSliceAttributes(SLICE_GAP,(char *)&l_fslice_gap);
                l_pd_coordinates->GetSliceAttributes(FOV,(char *)&l_fov);
                l_pd_coordinates->GetSliceAttributes(SLICE_NUM,(char *)&l_nslice_num);
                l_pd_coordinates->GetSliceAttributes(COUCH_OFFSET,(char *)&l_fcouch_offset);
            }
        }
        CString l_Vfstr = m_controller->GetPDDataHandle()->GetVantageSlicePlane(l_ni);
        CString l_str2(l_Vfstr);
        m_controller->GetPDDataHandle()->ConvertDICOMAxis(l_Vfdata1,l_Dcdata1,1);
        m_controller->GetPDDataHandle()->ConvertDICOMAxis(l_Vfdata2,l_Dcdata2,1);
        CString l_cstrEdit(_T(""));
        m_listctrl_data.InsertItem(l_ni,NULL);
        m_listctrl_data.SetItemText(l_ni,0,l_str2);     
        l_cstrEdit.Format(_T("%0.4f,%0.4f,%0.4f"),l_Vfdata1.x,l_Vfdata1.y,l_Vfdata1.z);
        m_listctrl_data.SetItemText(l_ni,1,l_cstrEdit);
        l_cstrEdit.Format(_T("%0.4f,%0.4f,%0.4f"),l_Vfdata2.x,l_Vfdata2.y,l_Vfdata2.z);
        m_listctrl_data.SetItemText(l_ni,2,l_cstrEdit);
        
        if (1 == f_ncalculate_flag)
        {   
            if (AUTO_COUCH == m_nmode)
            {           // mode is auto couch
                l_plane_name = m_listctrl_data.GetItemText(l_ni,0);
                
                if (0 == l_plane_name.Compare(_T(SLICE_PLANE_SCOUT2)))
                {
                    l_cstrEdit.Format(_T("%0.4f x %0.4f"),l_fov.x,l_fov.y);
                    m_listctrl_data.SetItemText(l_ni,3,l_cstrEdit);
                    l_cstrEdit.Format(_T("%0.4f"),l_fslice_thickness);
                    m_listctrl_data.SetItemText(l_ni,4,l_cstrEdit);
                    l_cstrEdit.Format(_T("%0.4f"),l_fslice_gap);
                    m_listctrl_data.SetItemText(l_ni,5,l_cstrEdit);
                    l_cstrEdit.Format(_T("%0.4d"),l_nslice_num);
                    m_listctrl_data.SetItemText(l_ni,6,l_cstrEdit);      
                    l_cstrEdit.Format(_T("%0.4f"),l_fcouch_offset);
                    m_listctrl_data.SetItemText(l_ni,7,l_cstrEdit);                                    
                    
                }
                else
                {
                    l_cstrEdit.Format(_T("---.--- x ---.---"));
                    m_listctrl_data.SetItemText(l_ni,3,l_cstrEdit);
                    l_cstrEdit.Format(_T("---.---"));
                    m_listctrl_data.SetItemText(l_ni,4,l_cstrEdit);
                    l_cstrEdit.Format(_T("---.---"));
                    m_listctrl_data.SetItemText(l_ni,5,l_cstrEdit);
                    l_cstrEdit.Format(_T("---.---"));
                    m_listctrl_data.SetItemText(l_ni,6,l_cstrEdit);
                    l_cstrEdit.Format(_T("---.---"));
                    m_listctrl_data.SetItemText(l_ni,7,l_cstrEdit);
                }
            }   
				//Mpower 039//Jul-12/Added/AS
            else if (AUTO_PLAN == m_nmode)
            {           // mode is auto plan
                l_plane_name = m_listctrl_data.GetItemText(l_ni,0);
                
                if(0 == l_plane_name.Compare(_T(SLICE_PLANE_SCOUT2))){
                    l_cstrEdit.Format(_T("---.--- x ---.---"));
                    m_listctrl_data.SetItemText(l_ni,3,l_cstrEdit);
                    l_cstrEdit.Format(_T("---.---"));
                    m_listctrl_data.SetItemText(l_ni,4,l_cstrEdit);
                    l_cstrEdit.Format(_T("---.---"));
                    m_listctrl_data.SetItemText(l_ni,5,l_cstrEdit);
                    l_cstrEdit.Format(_T("---.---"));
                    m_listctrl_data.SetItemText(l_ni,6,l_cstrEdit);
                    l_cstrEdit.Format(_T("---.---"));
                    m_listctrl_data.SetItemText(l_ni,7,l_cstrEdit);
                    
                }
                else
                {
                    l_cstrEdit.Format(_T("%0.4f x %0.4f"),l_fov.x,l_fov.y);
                    m_listctrl_data.SetItemText(l_ni,3,l_cstrEdit);
                    l_cstrEdit.Format(_T("%0.4f"),l_fslice_thickness);
                    m_listctrl_data.SetItemText(l_ni,4,l_cstrEdit);
                    l_cstrEdit.Format(_T("%0.4f"),l_fslice_gap);
                    m_listctrl_data.SetItemText(l_ni,5,l_cstrEdit);
                    l_cstrEdit.Format(_T("%d"),l_nslice_num);
                    m_listctrl_data.SetItemText(l_ni,6,l_cstrEdit);      
                    l_cstrEdit.Format(_T("---.---"));
                    m_listctrl_data.SetItemText(l_ni,7,l_cstrEdit);
                }
            }
            
            else if (MANUAL ==  m_nmode)
            {           //mode is manual
                
                if (l_ni == m_ncurrrowno){
                    
                    l_plane_name = m_listctrl_data.GetItemText(m_ncurrrowno,0);

                    if (0 == l_plane_name.Compare(_T(SLICE_PLANE_SCOUT2))){
                        l_cstrEdit.Format(_T("---.--- x ---.---"));
                        m_listctrl_data.SetItemText(l_ni,3,l_cstrEdit);
                        l_cstrEdit.Format(_T("---.---"));
                        m_listctrl_data.SetItemText(l_ni,4,l_cstrEdit);
                        l_cstrEdit.Format(_T("---.---"));
                        m_listctrl_data.SetItemText(l_ni,5,l_cstrEdit);
                        l_cstrEdit.Format(_T("---.---"));
                        m_listctrl_data.SetItemText(l_ni,6,l_cstrEdit);
                        l_cstrEdit.Format(_T("---.---"));

                        l_cstrEdit.Format(_T("%0.4f"),l_fcouch_offset);
                        m_listctrl_data.SetItemText(l_ni,7,l_cstrEdit); 
                    }
                    else{
                        l_cstrEdit.Format(_T("%0.4f x %0.4f"),l_fov.x,l_fov.y);
                        m_listctrl_data.SetItemText(l_ni,3,l_cstrEdit);
                        l_cstrEdit.Format(_T("%0.4f"),l_fslice_thickness);
                        m_listctrl_data.SetItemText(l_ni,4,l_cstrEdit);
                        l_cstrEdit.Format(_T("%0.4f"),l_fslice_gap);
                        m_listctrl_data.SetItemText(l_ni,5,l_cstrEdit);
                        l_cstrEdit.Format(_T("%d"),l_nslice_num);
                        m_listctrl_data.SetItemText(l_ni,6,l_cstrEdit);
                        l_cstrEdit.Format(_T("---.---"));
                        m_listctrl_data.SetItemText(l_ni,7,l_cstrEdit);

                    }
                    
                }else{
                    l_cstrEdit.Format(_T("---.--- x ---.---"));
                    m_listctrl_data.SetItemText(l_ni,3,l_cstrEdit);
                    l_cstrEdit.Format(_T("---.---"));
                    m_listctrl_data.SetItemText(l_ni,4,l_cstrEdit);
                    l_cstrEdit.Format(_T("---.---"));
                    m_listctrl_data.SetItemText(l_ni,5,l_cstrEdit);
                    l_cstrEdit.Format(_T("---.---"));
                    m_listctrl_data.SetItemText(l_ni,6,l_cstrEdit);
                    l_cstrEdit.Format(_T("---.---"));
                    m_listctrl_data.SetItemText(l_ni,7,l_cstrEdit);
                }
            } 
            else
            {
                CString l_cserr (_T("Invalid mode in showdata()"));
                CPDLogHelper::WriteToErrorLog(l_cserr,__LINE__);
            }
        } else{
            l_cstrEdit.Format(_T("---.--- x ---.---"));
            m_listctrl_data.SetItemText(l_ni,3,l_cstrEdit);
            l_cstrEdit.Format(_T("---.---"));
            m_listctrl_data.SetItemText(l_ni,4,l_cstrEdit);
            l_cstrEdit.Format(_T("---.---"));
            m_listctrl_data.SetItemText(l_ni,5,l_cstrEdit);
            l_cstrEdit.Format(_T("---.---"));
            m_listctrl_data.SetItemText(l_ni,6,l_cstrEdit);
            l_cstrEdit.Format(_T("---.---"));
            m_listctrl_data.SetItemText(l_ni,7,l_cstrEdit);
        }
    }   
}

/*************************************************************
* Method:       ShowMPRData
* Description:  This function is used to display VarfieldData
*               & DicomData for MPR mode
* Returns:      void
*************************************************************/
void CPlanDuplicatorDlg::ShowMPRData(void)
{
    PD_TRACE(CPlanDuplicatorDlg::ShowMPRData);
    
    f32vec3_t l_Vfdata1 = {0};
    f32vec3_t l_Vfdata2 = {0};
    f32vec3_t l_Vfdata3 = {0};
    f32vec3_t l_Dcdata1 = {0};
    f32vec3_t l_Dcdata2 = {0};
    f32vec3_t l_Dcdata3= {0};
    f32vec2_t l_fov = {0};
    float l_fslice_thickness = 0.0f;
    float l_fslice_gap = 0.0f;
    int l_nslice_num = 0;
    int status = 0;
    int size = 0;
    size = m_controller->GetPDDataHandle()->GetDataSize();
    int l_ni = 0;
    CPDCoordinates * l_pd_coordinates = NULL;
    for(l_ni = 0;l_ni < size;l_ni++){               
        l_Vfdata1 = m_controller->GetPDDataHandle()->GetVantageData(l_ni,SLICE_ORI_VECTOR);
        l_Vfdata2 = m_controller->GetPDDataHandle()->GetVantageData(l_ni,PHASE_ORI_VECTOR);
        l_Vfdata3 = m_controller->GetPDDataHandle()->GetVantageData(l_ni,OFFSET_VECTOR);
        l_pd_coordinates = m_controller->GetPDDataHandle()->GetVantageData(l_ni);
        if (NULL != l_pd_coordinates)
        {
            l_pd_coordinates->GetSliceAttributes(SLICE_THICKNESS,(char *)&l_fslice_thickness);
            l_pd_coordinates->GetSliceAttributes(SLICE_GAP,(char *)&l_fslice_gap);
            l_pd_coordinates->GetSliceAttributes(FOV,(char *)&l_fov);
            l_pd_coordinates->GetSliceAttributes(SLICE_NUM,(char *)&l_nslice_num);
        } 
        else
        {
            CString l_cserrmsg (_T("PD Data NULL ShowData()"));
            CPDLogHelper::WriteToErrorLog(l_cserrmsg,__LINE__);
            return;
        }
        
        CString l_Vfstr = m_controller->GetPDDataHandle()->GetVantageSlicePlane(l_ni);
        CString l_str2(l_Vfstr);
        m_controller->GetPDDataHandle()->ConvertDICOMAxis(l_Vfdata1,l_Dcdata1,1);
        m_controller->GetPDDataHandle()->ConvertDICOMAxis(l_Vfdata2,l_Dcdata2,1);
        m_controller->GetPDDataHandle()->ConvertDICOMAxis(l_Vfdata3,l_Dcdata3,1);
        CString l_cstrEdit(_T(""));
        m_listctrl_data.InsertItem(l_ni,NULL);
        m_listctrl_data.SetItemText(l_ni,0,l_str2);     
        l_cstrEdit.Format(_T("%0.4f,%0.4f,%0.4f"),l_Vfdata1.x,l_Vfdata1.y,l_Vfdata1.z);
        m_listctrl_data.SetItemText(l_ni,1,l_cstrEdit);
        l_cstrEdit.Format(_T("%0.4f,%0.4f,%0.4f"),l_Vfdata2.x,l_Vfdata2.y,l_Vfdata2.z);
        m_listctrl_data.SetItemText(l_ni,2,l_cstrEdit);
        l_cstrEdit.Format(_T("%0.4f,%0.4f,%0.4f"),l_Vfdata3.x,l_Vfdata3.y,l_Vfdata3.z);
        m_listctrl_data.SetItemText(l_ni,3,l_cstrEdit);
        //AN/Commented
//         m_listctrl_dataDicomAxis.InsertItem(l_ni,NULL);
//         m_listctrl_dataDicomAxis.SetItemText(l_ni,0,l_str2);
//         l_cstrEdit.Format(_T("%0.4f,%0.4f,%0.4f"),l_Dcdata1.x,l_Dcdata1.y,l_Dcdata1.z);
//         m_listctrl_dataDicomAxis.SetItemText(l_ni,1,l_cstrEdit);
//         l_cstrEdit.Format(_T("%0.4f,%0.4f,%0.4f"),l_Dcdata2.x,l_Dcdata2.y,l_Dcdata2.z);
//         m_listctrl_dataDicomAxis.SetItemText(l_ni,2,l_cstrEdit);
//         l_cstrEdit.Format(_T("%0.4f,%0.4f,%0.4f"),l_Dcdata3.x,l_Dcdata3.y,l_Dcdata3.z);
//         m_listctrl_dataDicomAxis.SetItemText(l_ni,3,l_cstrEdit);  
        
        l_cstrEdit.Format(_T("%0.4f x %0.4f"),l_fov.x,l_fov.y);
        m_listctrl_data.SetItemText(l_ni,4,l_cstrEdit);
        l_cstrEdit.Format(_T("%0.4f"),l_fslice_thickness);
        m_listctrl_data.SetItemText(l_ni,5,l_cstrEdit);
        l_cstrEdit.Format(_T("%0.4f"),l_fslice_gap);
        m_listctrl_data.SetItemText(l_ni,6,l_cstrEdit);
        l_cstrEdit.Format(_T("%d"),l_nslice_num);
        m_listctrl_data.SetItemText(l_ni,7,l_cstrEdit);
        
        m_listctrl_dataDicomAxis.InsertItem(l_ni,NULL);
        m_listctrl_dataDicomAxis.SetItemText(l_ni,0,l_str2);
        l_cstrEdit.Format(_T("%0.4f,%0.4f,%0.4f"),l_Dcdata1.x,l_Dcdata1.y,l_Dcdata1.z);
        m_listctrl_dataDicomAxis.SetItemText(l_ni,1,l_cstrEdit);
        l_cstrEdit.Format(_T("%0.4f,%0.4f,%0.4f"),l_Dcdata2.x,l_Dcdata2.y,l_Dcdata2.z);
        m_listctrl_dataDicomAxis.SetItemText(l_ni,2,l_cstrEdit);
        l_cstrEdit.Format(_T("%0.4f,%0.4f,%0.4f"),l_Dcdata3.x,l_Dcdata3.y,l_Dcdata3.z);
        m_listctrl_dataDicomAxis.SetItemText(l_ni,3,l_cstrEdit);  
        l_cstrEdit.Format(_T("%0.4f x %0.4f"),l_fov.x,l_fov.y);
        m_listctrl_dataDicomAxis.SetItemText(l_ni,4,l_cstrEdit);
        l_cstrEdit.Format(_T("%0.4f"),l_fslice_thickness);
        m_listctrl_dataDicomAxis.SetItemText(l_ni,5,l_cstrEdit);
        l_cstrEdit.Format(_T("%0.4f"),l_fslice_gap);
        m_listctrl_dataDicomAxis.SetItemText(l_ni,6,l_cstrEdit);
        l_cstrEdit.Format(_T("%d"),l_nslice_num);
        m_listctrl_dataDicomAxis.SetItemText(l_ni,7,l_cstrEdit);
    }   
}

/*************************************************************
* Method:       InitListColumnForVOI
* Description:  This function is used to set column headers
*               for VOI data in sliceplane listctrl 
* Returns:      void
*************************************************************/
void CPlanDuplicatorDlg::InitListColumnForVOI(void)
{   
    int i=0;
    for(i = 7; i >= 0; i--) {
        m_listctrl_data.DeleteColumn(i);
        m_listctrl_dataDicomAxis.DeleteColumn(i);
    }
    
    m_listctrl_data.InsertColumn(0,_T("SlicePlane"),LVCFMT_CENTER,75);
    m_listctrl_data.SetColumnRange(0,80,80);
    m_listctrl_data.InsertColumn(1,_T("Vector 1"),LVCFMT_CENTER,195);
    m_listctrl_data.SetColumnRange(1,195,195);
    m_listctrl_data.InsertColumn(2,_T("Vector 2"),LVCFMT_CENTER,195);
    m_listctrl_data.SetColumnRange(2,195,195);
    m_listctrl_data.InsertColumn(3,_T("FOV"),LVCFMT_CENTER,150);
    m_listctrl_data.SetColumnRange(3,150,150);
    m_listctrl_data.InsertColumn(4,_T("Slice Thickness"),LVCFMT_CENTER,120);
    m_listctrl_data.SetColumnRange(4,120,120);
    m_listctrl_data.InsertColumn(5,_T("Slice Gap"),LVCFMT_CENTER,120);
    m_listctrl_data.SetColumnRange(5,120,120);
    m_listctrl_data.InsertColumn(6,_T("Slice Number"),LVCFMT_CENTER,120);
    m_listctrl_data.SetColumnRange(6,120,120);
    m_listctrl_data.InsertColumn(7,_T("Couch Offset"),LVCFMT_CENTER,120);
    m_listctrl_data.SetColumnRange(7,120,120);
    m_listctrl_dataDicomAxis.SetFont(_T("Arial"), FONT_SIZE_10, TRUE );
    m_listctrl_status.DeleteAllItems();
    for (i = 7; i >= 0; i--) {
        m_listctrl_status.DeleteColumn(i);       
    }
    
    //insert columns in list control
    m_listctrl_status.InsertColumn(0,_T("Protocol"),LVCFMT_CENTER,80);
    m_listctrl_status.SetColumnRange(0,100,100);
    m_listctrl_status.InsertColumn(1,_T("Slice Plane"),LVCFMT_CENTER,100);
    m_listctrl_status.SetColumnRange(1,100,100);
    //AS added
    m_listctrl_status.InsertColumn(2,_T("Axis"),LVCFMT_CENTER,100);   
    m_listctrl_status.SetColumnRange(2,100,100);
    m_listctrl_status.InsertColumn(3,_T("Parameter"),LVCFMT_CENTER,100);
    m_listctrl_status.SetColumnRange(3,110,110);
    m_listctrl_status.InsertColumn(4,_T("Mode"),LVCFMT_CENTER,100);
    m_listctrl_status.SetColumnRange(4,100,100);
    m_listctrl_status.InsertColumn(5,_T("Status"),LVCFMT_CENTER,130);
    m_listctrl_status.SetColumnRange(5,100,100); //130
    
}

/*************************************************************
* Method:       InitListColumnForMPR
* Description:  This function is used to set column headers in
*               sliceplane listctrl for MPR data
* Returns:      void
*************************************************************/
void CPlanDuplicatorDlg::InitListColumnForMPR(void)
{
    PD_TRACE(CPlanDuplicatorDlg::InitListColumnForMPR);
    int i=0;
    for (i = 7; i >= 0; i--) {
        m_listctrl_data.DeleteColumn(i);
        m_listctrl_dataDicomAxis.DeleteColumn(i);
    }
    
    m_listctrl_data.InsertColumn(0,_T("SlicePlane"),LVCFMT_CENTER,110);
    m_listctrl_data.SetColumnRange(0,80,80);
    m_listctrl_data.InsertColumn(1,_T("Slice Vector"),LVCFMT_CENTER,195);
    m_listctrl_data.SetColumnRange(1,195,195);
    m_listctrl_data.InsertColumn(2,_T("Phase Vector"),LVCFMT_CENTER,195);
    m_listctrl_data.SetColumnRange(2,195,195);
    m_listctrl_data.InsertColumn(3,_T("Offset Vector"),LVCFMT_CENTER,200);
    m_listctrl_data.SetColumnRange(3,200,200);
    m_listctrl_data.InsertColumn(4,_T("FOV"),LVCFMT_CENTER,150);
    m_listctrl_data.SetColumnRange(4,150,150);
    m_listctrl_data.InsertColumn(5,_T("Slice Thickness"),LVCFMT_CENTER,120);
    m_listctrl_data.SetColumnRange(5,120,120);
    m_listctrl_data.InsertColumn(6,_T("Slice Gap"),LVCFMT_CENTER,120);
    m_listctrl_data.SetColumnRange(6,120,120);
    m_listctrl_data.InsertColumn(7,_T("Slice Number"),LVCFMT_CENTER,120);
    m_listctrl_data.SetColumnRange(7,120,120);
    
    m_listctrl_dataDicomAxis.SetFont(_T("Arial"), FONT_SIZE_10, TRUE );
    m_listctrl_status.SetFont(_T("Arial"), FONT_SIZE_10, TRUE );
    
    //insert columns in list control
    m_listctrl_dataDicomAxis.InsertColumn(0,_T("SlicePlane"),LVCFMT_CENTER,80);
    m_listctrl_dataDicomAxis.SetColumnRange(0,80,80);
    m_listctrl_dataDicomAxis.InsertColumn(1,_T("Slice Vector"),LVCFMT_CENTER,195);
    m_listctrl_dataDicomAxis.SetColumnRange(1,195,195);
    m_listctrl_dataDicomAxis.InsertColumn(2,_T("Phase Vector"),LVCFMT_CENTER,195);
    m_listctrl_dataDicomAxis.SetColumnRange(2,195,195);
    m_listctrl_dataDicomAxis.InsertColumn(3,_T("Offset Vector"),LVCFMT_CENTER,200);
    m_listctrl_dataDicomAxis.SetColumnRange(3,200,200);
    m_listctrl_dataDicomAxis.InsertColumn(4,_T("FOV"),LVCFMT_CENTER,150);
    m_listctrl_dataDicomAxis.SetColumnRange(4,150,150);
    m_listctrl_dataDicomAxis.InsertColumn(5,_T("Slice Thickness"),LVCFMT_CENTER,120);
    m_listctrl_dataDicomAxis.SetColumnRange(5,120,12060);
    m_listctrl_dataDicomAxis.InsertColumn(6,_T("Slice Gap"),LVCFMT_CENTER,120);
    m_listctrl_dataDicomAxis.SetColumnRange(6,120,120);
    m_listctrl_dataDicomAxis.InsertColumn(7,_T("Slice Number"),LVCFMT_CENTER,120);
    m_listctrl_dataDicomAxis.SetColumnRange(7,120,120);
    
    m_listctrl_status.DeleteAllItems();
    for (i = 7; i >= 0; i--) {
        m_listctrl_status.DeleteColumn(i);       
    }
    
    //AS added   
    //insert columns in list control
    m_listctrl_status.InsertColumn(0,_T("Protocol"),LVCFMT_LEFT,220);
    m_listctrl_status.SetColumnRange(0,220,220);
    m_listctrl_status.InsertColumn(1,_T("Slice Plane"),LVCFMT_LEFT,200);
    m_listctrl_status.SetColumnRange(1,200,200);
    m_listctrl_status.InsertColumn(2,_T("Status"),LVCFMT_LEFT,220);
    m_listctrl_status.SetColumnRange(2,190,190); 
}

/*************************************************************
* Method:       OnRadioSliceNumber
* Description:  Callback function 
* Returns:      void
*************************************************************/
void CPlanDuplicatorDlg::OnRadioSliceNumber() 
{
    PD_TRACE(CPlanDuplicatorDlg::OnRadioSliceNumber);
    m_nparameter = 0;
   // m_controller->SetAttributeSelection(m_nparameter);
    m_UpdateSeqButton.EnableWindow(FALSE);
    
}

/*************************************************************
* Method:       OnRadioSliceGap
* Description:  Callback fucntion 
* Returns:      void
*************************************************************/
void CPlanDuplicatorDlg::OnRadioSliceGap() 
{
    PD_TRACE(CPlanDuplicatorDlg::OnRadioSliceGap);
    m_nparameter = 1;
//    m_controller->SetAttributeSelection(m_nparameter);
    m_UpdateSeqButton.EnableWindow(FALSE);
    
}

/*************************************************************
* Method:       OnRadioSliceThickness
* Description:  Callback function
* Returns:      void
*************************************************************/
void CPlanDuplicatorDlg::OnRadioSliceThickness() 
{
    PD_TRACE(CPlanDuplicatorDlg::OnRadioSliceThickness);
    m_nparameter = 2;
//    m_controller->SetAttributeSelection(m_nparameter);
    m_UpdateSeqButton.EnableWindow(FALSE);
    
}

/*************************************************************
* Method:       OnRadioAxial
* Description:  Callback fucntion 
* Returns:      void
*************************************************************/
void CPlanDuplicatorDlg::OnRadioAxial() 
{
    PD_TRACE(CPlanDuplicatorDlg::OnRadioAxial);
    m_naxis = 0;
    if (m_nmode == MANUAL) //Manual
    {
        m_UpdateSeqButton.EnableWindow(FALSE);
    }
    
}

/*************************************************************
* Method:       OnRadioCoronal
* Description:  Callback function
* Returns:      void
*************************************************************/
void CPlanDuplicatorDlg::OnRadioCoronal() 
{
    PD_TRACE(CPlanDuplicatorDlg::OnRadioCoronal);
    m_naxis = 1;
    if (m_nmode == MANUAL) //Manual
    {
        m_UpdateSeqButton.EnableWindow(FALSE);
    }
    
}

/*************************************************************
* Method:       OnRadioSagittal
* Description:  Callback function for sagittal radio button
* Returns:      void
*************************************************************/
void CPlanDuplicatorDlg::OnRadioSagittal() 
{
    PD_TRACE(CPlanDuplicatorDlg::OnRadioSagittal);
    m_naxis = 2;
    if (m_nmode == MANUAL) //Manual
    {
        m_UpdateSeqButton.EnableWindow(FALSE);
    }
    
}
/*************************************************************
* Method:       CalculateSelect
* Description:  This function is used to get selected parameters from Radio buttons
* Returns:      void
*************************************************************/
void CPlanDuplicatorDlg::CalculateSelect(/* int f_ncurr_slice_plane_idx*/)
{
    PD_TRACE(CPlanDuplicatorDlg::CalculateSelect);
    
    CString l_plane_name;
    l_plane_name = m_listctrl_data.GetItemText(m_ncurrrowno,0);
    
    if (0 == l_plane_name.Compare(_T(SLICE_PLANE_MULTISLICE)))
    {   
        if (m_nmode == AUTO_PLAN) //Auto Plan
        {
            m_fcParams.selectedParam = SEL_SLICE_NUM;
        } 
        else                                         //Manual
        {
            switch(m_nparameter)//Parameter
            {
            case 0://Slice Number
                m_fcParams.selectedParam = SEL_SLICE_NUM;
                break;
            case 1://Slice Gap
                m_fcParams.selectedParam = SEL_SLICE_GAP;
                break;
            case 2://Slice Thickness
                m_fcParams.selectedParam = SEL_SLICE_THICKNESS;
                break;
            default:
                m_fcParams.selectedParam = SEL_SLICE_NUM; //AN/Added Default Multi slice
            }
        }
    } 
    else if (0 == l_plane_name.Compare(_T(SLICE_PLANE_SCOUT2)))
    {
        m_fcParams.selectedParam = NONE;
    } 
    else                                             //Map and Shimming
    {
        m_fcParams.selectedParam = SEL_SLICE_GAP;
    }
    switch(m_naxis)//Axis
    {
    case 0://Axial
        m_fcParams.selectedAxis = AXIAL;
        break;
    case 1://Coronal
        m_fcParams.selectedAxis = CORONAL;
        break;
    case 2://Sagittal
        m_fcParams.selectedAxis = SAGITTAL;
        break;
    default:
        break;
    }
    
    switch(m_nmode)
    {
    case AUTO_COUCH://Auto Couch
        m_fcParams.isScoutSelected = 1;
        break;
    case AUTO_PLAN: //Auto Plan
        m_fcParams.isScoutSelected = 0;
        break;
    case MANUAL://Manual
        if(0 == l_plane_name.Compare(_T(SLICE_PLANE_SCOUT2)))
        {
            m_fcParams.isScoutSelected = 1;
        } else
        {
            m_fcParams.isScoutSelected = 0;
        }
        break;
    default:
        m_fcParams.isScoutSelected= FALSE;
        break;
    }
}

	//Mpower 039//Jul-12/Added/AN/
/*************************************************************
* Method:       PreTranslateMessage
* Description:  This function is used for processing window messages
* Parameter:    MSG * pMsg :
* Returns:      BOOL
*************************************************************/
BOOL CPlanDuplicatorDlg::PreTranslateMessage(MSG *pMsg)
{
    if (pMsg->message == WM_LBUTTONDOWN)
    {
        if (m_nmode != MANUAL) //No list selection for Auto Plan and Auto Couch
        {     
            CRect l_listctrl_data_rect;
            CRect l_scroll_data_rect;
            m_listctrl_data.GetWindowRect(&l_listctrl_data_rect);
            CDC * screen = GetDC();
            bool bHScroll = (m_listctrl_data.GetStyle()&WS_HSCROLL)!=0;
            bool bVScroll = (m_listctrl_data.GetStyle()&WS_VSCROLL)!=0;
            int l_nscroll_height = 0;
            int l_nscroll_width = 0;
            if (bHScroll)
            {
                l_nscroll_height = (int)(GetSystemMetrics(SM_CYHSCROLL) * 1.8);
            }
            if (bVScroll)
            {
                l_nscroll_width = (int)(GetSystemMetrics(SM_CXVSCROLL) * 1.8);
            }
            ScreenToClient(&l_listctrl_data_rect);
            ScreenToClient(&pMsg->pt);
            if (pMsg->pt.x >= l_listctrl_data_rect.left && 
                pMsg->pt.x < (l_listctrl_data_rect.right - l_nscroll_width) &&
                pMsg->pt.y >= l_listctrl_data_rect.top &&
                pMsg->pt.y < (l_listctrl_data_rect.bottom - l_nscroll_height)
                )
            {
                return(TRUE);
            }
        }
    }
    return DialogEx::PreTranslateMessage(pMsg);
}


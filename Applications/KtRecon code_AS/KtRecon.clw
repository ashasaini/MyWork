; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CKtReconDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ktrecon.h"
LastPage=0

ClassCount=9
Class1=CErrorDlg
Class2=CInPlaceFrame
Class3=CKtReconApp
Class4=CAboutDlg
Class5=CKtReconDlg
Class6=CKtReconDoc
Class7=CKtReconView
Class8=CMainFrame
Class9=CPopupDlg

ResourceCount=7
Resource1=IDR_SRVR_EMBEDDED
Resource2=IDR_MAINFRAME
Resource3=IDR_SRVR_INPLACE
Resource4=IDD_KTRECON_FORM
Resource5=IDD_DIALOG_KTRECON
Resource6=IDD_ABOUTBOX
Resource7=IDD_DIALOG_ERROR

[CLS:CErrorDlg]
Type=0
BaseClass=CPopupDlg
HeaderFile=ErrorDlg.h
ImplementationFile=ErrorDlg.cpp

[CLS:CInPlaceFrame]
Type=0
BaseClass=COleDocIPFrameWnd
HeaderFile=IpFrame.h
ImplementationFile=IpFrame.cpp

[CLS:CKtReconApp]
Type=0
BaseClass=CWinApp
HeaderFile=KtRecon.h
ImplementationFile=KtRecon.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=KtRecon.cpp
ImplementationFile=KtRecon.cpp

[CLS:CKtReconDlg]
Type=0
BaseClass=DialogEx
HeaderFile=KtReconDlg.h
ImplementationFile=KtReconDlg.cpp
LastObject=IDC_LIST_SCANNED_PROTOCOL

[CLS:CKtReconDoc]
Type=0
BaseClass=COleServerDoc
HeaderFile=KtReconDoc.h
ImplementationFile=KtReconDoc.cpp

[CLS:CKtReconView]
Type=0
BaseClass=CFormView
HeaderFile=KtReconView.h
ImplementationFile=KtReconView.cpp

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp

[CLS:CPopupDlg]
Type=0
BaseClass=DialogEx
HeaderFile=PopupDlg.h
ImplementationFile=PopupDlg.cpp

[DLG:IDD_DIALOG_ERROR]
Type=1
Class=CErrorDlg
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDC_STATIC_ERROR,static,1342308352

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_DIALOG_KTRECON]
Type=1
Class=CKtReconDlg
ControlCount=8
Control1=IDC_BUTTON_EXIT,button,1342246656
Control2=IDC_STATIC_PROTOCOL_DISPLAY_FRAME,button,1342177287
Control3=IDC_STATIC_SCANNED_PROT,static,1342308352
Control4=IDC_STATIC_DATA_OPERATION_FRAME,button,1342177287
Control5=IDC_BUTTON_GET_DATA,button,1342246656
Control6=IDC_BUTTON_KTRECON,button,1342246656
Control7=IDC_BUTTON_VIEW,button,1342246656
Control8=IDC_LIST_SCANNED_PROTOCOL,SysListView32,1350631424

[DLG:IDD_KTRECON_FORM]
Type=1
Class=CKtReconView
ControlCount=0

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_MRU_FILE1
Command6=ID_APP_EXIT
Command7=ID_EDIT_UNDO
Command8=ID_EDIT_CUT
Command9=ID_EDIT_COPY
Command10=ID_EDIT_PASTE
Command11=ID_APP_ABOUT
CommandCount=11

[MNU:IDR_SRVR_INPLACE]
Type=1
Class=?
Command1=ID_EDIT_UNDO
Command2=ID_EDIT_CUT
Command3=ID_EDIT_COPY
Command4=ID_EDIT_PASTE
Command5=ID_APP_ABOUT
CommandCount=5

[MNU:IDR_SRVR_EMBEDDED]
Type=1
Class=?
Command1=ID_FILE_UPDATE
Command2=ID_FILE_SAVE_COPY_AS
Command3=ID_APP_EXIT
Command4=ID_EDIT_UNDO
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_APP_ABOUT
CommandCount=8

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_UNDO
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_NEXT_PANE
Command13=ID_PREV_PANE
CommandCount=13

[ACL:IDR_SRVR_INPLACE]
Type=1
Class=?
Command1=ID_EDIT_UNDO
Command2=ID_EDIT_CUT
Command3=ID_EDIT_COPY
Command4=ID_EDIT_PASTE
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_CANCEL_EDIT_SRVR
CommandCount=9

[ACL:IDR_SRVR_EMBEDDED]
Type=1
Class=?
Command1=ID_FILE_UPDATE
Command2=ID_EDIT_UNDO
Command3=ID_EDIT_CUT
Command4=ID_EDIT_COPY
Command5=ID_EDIT_PASTE
Command6=ID_EDIT_UNDO
Command7=ID_EDIT_CUT
Command8=ID_EDIT_COPY
Command9=ID_EDIT_PASTE
Command10=ID_NEXT_PANE
Command11=ID_PREV_PANE
CommandCount=11


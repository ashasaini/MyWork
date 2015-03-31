# Microsoft Developer Studio Project File - Name="PQM" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=PQM - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PQM.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PQM.mak" CFG="PQM - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PQM - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "PQM - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/MRI/13690/Study Manager/Work In Progress/Hemant/PQM/GUI", JBIOAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PQM - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /Od /I "$(MRMP_HOME)\Sources\Acquire" /I "$(MPLUSHOME)\Include" /I "$(MRMP_HOME)\Sources\Acquire\PQM\IPC\IPCCOM" /I "$(MRMP_HOME)\Include" /I "$(MPLUSHOME)\Include\MMM" /I "$(MRMP_HOME)\Sources\Acquire\PQM\IPC" /D "__INTTYPES_H__" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /D "__NUTC__" /D "__WIN_NATIVE_PORTING__" /D "__WNP_SIGNAL__" /D "MRI" /D "IRIX65" /D "__WNP_PATNI__" /D "PQM_MODE" /D "MPLUS_V3" /D "__WNP_MPLUS__" /Fr /FD /EHa /c
# SUBTRACT CPP /WX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 libSMCFW.lib libSMUI.lib libSMLog.lib MPlusI18nCatalog.lib MPlusI18nResourceLoader.lib MPlusConfig.lib MPlusErrorHandler.lib MplusEventLog.lib MPlusErrorDisplay.lib MQRT.lib PASDispDictionary.lib Mpluscomm.lib MRErrorLogLibrary.lib MRTraceLogLibrary.lib MROpeLogLibrary.lib Probe.lib MPlusUICommon.lib libdll.lib MPlusProcessManagerIF.lib Psapi.Lib PASCom.lib PDataTypesU.lib PDataParserU.lib Pcommlib.lib libWNP.lib wsock32.lib Ws2_32.lib MPlusConfig2.lib MPlusDBdtManager.lib libSMMutex.lib PErrorHandler.lib dbghelp.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /incremental:yes /map /debug /machine:I386 /libpath:"$(MPLUSHOME)\lib\Release" /libpath:"$(MRMP_HOME)\Lib\Release" /MAPINFO:LINES /MAPINFO:EXPORTS
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=move Release\PQM.exe $(MRMP_HOME)\Bin\Release	move Release\PQM.map $(MRMP_HOME)\Lib\Release
# End Special Build Tool

!ELSEIF  "$(CFG)" == "PQM - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(MRMP_HOME)\Sources\Acquire" /I "$(MPLUSHOME)\Include" /I "$(MRMP_HOME)\Sources\Acquire\PQM\IPC\IPCCOM" /I "$(MRMP_HOME)\Include" /I "$(MPLUSHOME)\Include\MMM" /I "$(MRMP_HOME)\Sources\Acquire\PQM\IPC" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "WIN32" /D "_UNICODE" /D "UNICODE" /D "WSETRACEPRINT" /D "DETAIL_LOG" /D "__NUTC__" /D "__WIN_NATIVE_PORTING__" /D "MRI" /D "IRIX65" /D "__WNP_PATNI__" /D "PQM_MODE" /D "MPLUS_V3" /D "__WNP_MPLUS__" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 libSMCFWd.lib libSMUId.lib libSMLogd.lib PDataParserUd.lib PDataTypesUd.lib Pcommlibd.lib MPlusI18nCatalogd.lib MPlusI18nResourceLoaderd.lib MPlusErrorHandlerd.lib MplusEventLogd.lib MQRT.lib PASDispDictionaryd.lib Mpluscomm_d.lib MRErrorLogLibraryd.lib MROpeLogLibraryd.lib MRTraceLogLibraryd.lib Probe.lib MPlusUICommond.lib libdll.lib MPlusProcessManagerIFd.lib Psapi.Lib PASCOMd.lib MPlusErrorDisplayd.lib libWNP.lib wsock32.lib Ws2_32.lib MPlusConfig2d.lib MPlusDBdtManagerd.lib libSMMutexd.lib PErrorHandler.lib dbghelp.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /nodefaultlib:"uafxcwd.lib" /out:"Debug/PQMd.exe" /pdbtype:sept /libpath:"$(MPLUSHOME)\lib\debug" /libpath:"$(MRMP_HOME)\Lib\Debug"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=move Debug\PQMd.exe $(MRMP_HOME)\Bin\Debug	copy resource.h $(MRMP_HOME)\Include\PQM
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "PQM - Win32 Release"
# Name "PQM - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Controls"

# PROP Default_Filter ""
# Begin Group "Other ctrls"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CustomStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomStatic.h
# End Source File
# Begin Source File

SOURCE=.\PQMEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMEdit.h
# End Source File
# Begin Source File

SOURCE=.\PQMGraphCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMGraphCtrl.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\PQMListCtrl.cpp
# End Source File
# End Group
# Begin Group "Dialogs"

# PROP Default_Filter ""
# Begin Group "MRSPrescan classes"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MRSPreScanDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MRSPreScanDlg.h
# End Source File
# Begin Source File

SOURCE=.\MRSScanListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\MRSScanListCtrl.h
# End Source File
# End Group
# Begin Group "ManualPreScan"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\InterfaceForPQMManualPreScanDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\InterfaceForPQMManualPreScanDlg.h
# End Source File
# Begin Source File

SOURCE=.\ManualPrescanEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\ManualPrescanEdit.h
# End Source File
# Begin Source File

SOURCE=.\PQMManualPreScanDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMManualPreScanDlg.h
# End Source File
# End Group
# Begin Group "Patient Orientation dialog"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PatientOrientationDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\PatientOrientationDialog.h
# End Source File
# End Group
# Begin Group "Queue & MRS related dialog"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\QueueDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\QueueDialog.h
# End Source File
# End Group
# Begin Group "Axis Locator Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CoilLstDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CoilLstDlg.h
# End Source File
# Begin Source File

SOURCE=.\CoilSelectionUtility.cpp
# End Source File
# Begin Source File

SOURCE=.\CoilSelectionUtility.h
# End Source File
# Begin Source File

SOURCE=.\CoilSelWin.cpp
# End Source File
# Begin Source File

SOURCE=.\CoilSelWin.h
# End Source File
# End Group
# Begin Group "Other Dialogs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DelayTimeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PasswordDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMAnatomyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMCFADlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMCoilInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMCommentDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PqmdBdtCautionDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMDebugWinDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMGateInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMInfoSeqDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMPlanDupDlg.cpp
# End Source File
# End Group
# Begin Group "CDS"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CDS\CDSEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\CDS\CDSEdit.h
# End Source File
# Begin Source File

SOURCE=.\CDS\COMPASS_Location_Debug.cpp
# End Source File
# Begin Source File

SOURCE=.\CDS\COMPASS_Location_Debug.h
# End Source File
# Begin Source File

SOURCE=.\CDS\COMPASS_SettingDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\CDS\COMPASS_SettingDialog.h
# End Source File
# Begin Source File

SOURCE=.\CDS\CustomLocDbgEditBox.cpp
# End Source File
# Begin Source File

SOURCE=.\CDS\CustomLocDbgEditBox.h
# End Source File
# Begin Source File

SOURCE=.\PQMCDSController.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMCDSController.h
# End Source File
# End Group
# Begin Group "ScanCommGeneration"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ScanCommGeneration.cpp
# End Source File
# Begin Source File

SOURCE=.\ScanCommGeneration.h
# End Source File
# End Group
# Begin Group "Scan Offset"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ScanOffset\MoveCouchWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ScanOffset\MoveCouchWnd.h
# End Source File
# Begin Source File

SOURCE=.\PQMSetScanOffsetDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMSetScanOffsetDlg.h
# End Source File
# Begin Source File

SOURCE=.\ScanOffset\ScanOffsetVal.cpp
# End Source File
# Begin Source File

SOURCE=.\ScanOffset\ScanOffsetVal.h
# End Source File
# End Group
# Begin Group "WirelessGatingDialog"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PQMWirelessGating.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMWirelessGating.h
# End Source File
# Begin Source File

SOURCE=.\WirelessGatingController.cpp
# End Source File
# Begin Source File

SOURCE=.\WirelessGatingController.h
# End Source File
# End Group
# Begin Group "AMB"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AMBDialogUtility.cpp
# End Source File
# Begin Source File

SOURCE=.\AMBDialogUtility.h
# End Source File
# Begin Source File

SOURCE=.\PQMAMBController.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMAMBController.h
# End Source File
# Begin Source File

SOURCE=.\PQMAMBDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMAMBDlg.h
# End Source File
# End Group
# End Group
# Begin Group "Data"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PQMSequence.cpp
# End Source File
# End Group
# Begin Group "Communication"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DPSManager.cpp
# End Source File
# Begin Source File

SOURCE=.\IPCComManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Sink.cpp
# End Source File
# End Group
# Begin Group "Utilities"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PQMLogMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMUtility.cpp
# End Source File
# End Group
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PQMGlobal.cpp
# End Source File
# End Group
# Begin Group "MessageHandlers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AcqmanMessageHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\IPCMessageHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMMessageHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\SRMessageHandler.cpp
# End Source File
# End Group
# Begin Group "Main"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DailogInterfaces.cpp
# End Source File
# Begin Source File

SOURCE=.\InterfaceClasses.cpp
# End Source File
# Begin Source File

SOURCE=.\IpFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\PQM.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMView.cpp
# End Source File
# Begin Source File

SOURCE=.\SrvrItem.cpp
# End Source File
# End Group
# Begin Group "Others"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ControlStateMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\ControlStateMgr.h
# End Source File
# Begin Source File

SOURCE=.\ExceptionAttacher.cpp
# End Source File
# Begin Source File

SOURCE=.\PqmChildThread.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMImageUtility.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMImageUtility.h
# End Source File
# Begin Source File

SOURCE=.\UIButtonUtility.cpp
# End Source File
# Begin Source File

SOURCE=.\UIButtonUtility.h
# End Source File
# End Group
# Begin Group "WFDA"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\WFDAHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\WFDAHandlerInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\WFDAHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\WFDAImplementer.cpp
# End Source File
# Begin Source File

SOURCE=.\WFDAMsgReceiver.cpp
# End Source File
# Begin Source File

SOURCE=.\WFDAMsgSender.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\AutoLocatorAppController.cpp
# End Source File
# Begin Source File

SOURCE=.\CoilChannelMode.cpp
# End Source File
# Begin Source File

SOURCE=.\DEGLCommunicator.cpp
# End Source File
# Begin Source File

SOURCE=.\PQM.rc
# End Source File
# Begin Source File

SOURCE=.\SFTMenuHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TiPrepAppController.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Controls Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PQMListCtrl.h
# End Source File
# End Group
# Begin Group "Main Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DailogInterfaces.h
# End Source File
# Begin Source File

SOURCE=.\InterfaceClasses.h
# End Source File
# Begin Source File

SOURCE=.\IpFrame.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\PQM.h
# End Source File
# Begin Source File

SOURCE=.\PQMDoc.h
# End Source File
# Begin Source File

SOURCE=.\PQMView.h
# End Source File
# Begin Source File

SOURCE=.\SrvrItem.h
# End Source File
# End Group
# Begin Group "Dialogs Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DelayTimeDlg.h
# End Source File
# Begin Source File

SOURCE=.\PasswordDlg.h
# End Source File
# Begin Source File

SOURCE=.\PQMAnatomyDlg.h
# End Source File
# Begin Source File

SOURCE=.\PQMCFADlg.h
# End Source File
# Begin Source File

SOURCE=.\PQMCoilInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\PQMCommentDlg.h
# End Source File
# Begin Source File

SOURCE=.\PqmdBdtCautionDialog.h
# End Source File
# Begin Source File

SOURCE=.\PQMDebugWinDlg.h
# End Source File
# Begin Source File

SOURCE=.\PQMGateInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\PQMInfoSeqDlg.h
# End Source File
# Begin Source File

SOURCE=.\PQMPlanDupDlg.h
# End Source File
# End Group
# Begin Group "Data Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PQMSequence.h
# End Source File
# End Group
# Begin Group "Communication Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DPSManager.h
# End Source File
# Begin Source File

SOURCE=.\IPCComManager.h
# End Source File
# Begin Source File

SOURCE=.\Sink.h
# End Source File
# End Group
# Begin Group "Utilities Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PQMLogMgr.h
# End Source File
# Begin Source File

SOURCE=.\PQMUtility.h
# End Source File
# End Group
# Begin Group "Common Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PatientOrientationConstants.h
# End Source File
# Begin Source File

SOURCE=.\PqmChildThread.h
# End Source File
# Begin Source File

SOURCE=.\PQMGlobal.h
# End Source File
# End Group
# Begin Group "MessageHandlers Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AcqmanMessageHandler.h
# End Source File
# Begin Source File

SOURCE=.\IPCMessageHandler.h
# End Source File
# Begin Source File

SOURCE=.\PQMMessageHandler.h
# End Source File
# Begin Source File

SOURCE=.\SRMessageHandler.h
# End Source File
# End Group
# Begin Group "WFDA Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\WFDAConstants.h
# End Source File
# Begin Source File

SOURCE=.\WFDAHandler.h
# End Source File
# Begin Source File

SOURCE=.\WFDAHandlerInterface.h
# End Source File
# Begin Source File

SOURCE=.\WFDAHelper.h
# End Source File
# Begin Source File

SOURCE=.\WFDAImplementer.h
# End Source File
# Begin Source File

SOURCE=.\WFDAMsgReceiver.h
# End Source File
# Begin Source File

SOURCE=.\WFDAMsgSender.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\AutoLocatorAppController.h
# End Source File
# Begin Source File

SOURCE=.\CoilChannelMode.h
# End Source File
# Begin Source File

SOURCE=.\DEGLCommunicator.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Include\PQM\resource.h
# End Source File
# Begin Source File

SOURCE=.\SFTMenuHandler.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TiPrepAppController.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\res\Abdomen.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\PQM_IMAGES\add.ico
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\Add_button.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Ankle.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\ScanOffset\Button_Icons\arrow_B_active.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ScanOffset\Button_Icons\arrow_B_normal.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ScanOffset\Button_Icons\arrow_LB_active.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ScanOffset\Button_Icons\arrow_LB_normal.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ScanOffset\Button_Icons\arrow_LT_active.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ScanOffset\Button_Icons\arrow_LT_normal.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ScanOffset\Button_Icons\arrow_RB_active.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ScanOffset\Button_Icons\arrow_RB_normal.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ScanOffset\Button_Icons\arrow_RT_active.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ScanOffset\Button_Icons\arrow_RT_normal.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ScanOffset\Button_Icons\arrow_T_active.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ScanOffset\Button_Icons\arrow_T_normal.ico
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\AutoScan_button.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\AutoScan_Non_Selected.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\PQM_LIST_IMAGES\AutoScan_Protocol.ico
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\AutoScan_Selcted_Image.bmp
# End Source File
# Begin Source File

SOURCE=".\RES\Coil Selection Images\Axislocator_Left.bmp"
# End Source File
# Begin Source File

SOURCE=".\RES\Coil Selection Images\Axislocator_Right.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\b_Abdomen.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\b_Abdomen.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b_Ankle.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\b_Ankle.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b_Chest.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\b_Chest.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b_Cspine.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\b_Cspine.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b_Extremity.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\b_Extremity.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b_Extremity2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\b_Extremity2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b_Hand.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\b_Hand.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b_Head.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\b_Head.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b_Pelvis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\b_Pelvis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b_Shoulder.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\b_Shoulder.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=".\RES\Coil Selection Images\Bitmap_Coil_Listsel.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_u.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\bmp00000.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\bmp00000_Disabled.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\bmp00001_Disabled.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\bmp00002_Disabled.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\bmp00003_Disabled.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\bmp00004_Disabled.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\PQM_IMAGES\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cfa_left.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cfa_right.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CheckedRadioButton.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Chest.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\Close_button.bmp
# End Source File
# Begin Source File

SOURCE=".\RES\Coil Selection Images\CoilListWindow.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\Cspine.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\PQM_LIST_IMAGES\Current_Protocol.ico
# End Source File
# Begin Source File

SOURCE=.\RES\PQM_IMAGES\customize.ico
# End Source File
# Begin Source File

SOURCE=.\RES\PQM_IMAGES\delete.ico
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\Delete_Button.bmp
# End Source File
# Begin Source File

SOURCE=".\res\Patient Orientation Images\Group1\Combo1\direction_l_turningup_foot.bmp"
# End Source File
# Begin Source File

SOURCE=".\RES\Patient Orientation Images\Combo1\direction_l_turningup_foot.ico"
# End Source File
# Begin Source File

SOURCE=".\res\Patient Orientation Images\Group1\Combo1\direction_l_turningup_head.bmp"
# End Source File
# Begin Source File

SOURCE=".\RES\Patient Orientation Images\Combo1\direction_l_turningup_head.ico"
# End Source File
# Begin Source File

SOURCE=".\res\Patient Orientation Images\Group2\Combo1\direction_r_turningup_foot.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Patient Orientation Images\Group2\Combo1\direction_r_turningup_head.bmp"
# End Source File
# Begin Source File

SOURCE=.\RES\PQM_LIST_IMAGES\Done_Protocol.ico
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\Dummy_button1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\Dummy_button2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Extremity.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Extremity2.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\PQM_LIST_IMAGES\Fail_protocol.ico
# End Source File
# Begin Source File

SOURCE=.\res\FocusCheckedRadioButton.bmp
# End Source File
# Begin Source File

SOURCE=.\res\FocusUncheckedRadioButton.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\grp2_com.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Hand.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Head.bmp
# End Source File
# Begin Source File

SOURCE=".\res\Patient Orientation Images\Group1\human_a_1.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Patient Orientation Images\Group1\human_a_2.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Patient Orientation Images\Group1\human_a_3.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Patient Orientation Images\Group1\human_a_4.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Patient Orientation Images\Group1\human_a_5.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Patient Orientation Images\Group1\human_a_6.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Patient Orientation Images\Group1\human_a_7.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Patient Orientation Images\Group1\human_a_8.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Patient Orientation Images\Group2\human_b_1.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Patient Orientation Images\Group2\human_b_2.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Patient Orientation Images\Group2\human_b_3.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Patient Orientation Images\Group2\human_b_4.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Patient Orientation Images\Group2\human_b_5.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Patient Orientation Images\Group2\human_b_6.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Patient Orientation Images\Group2\human_b_7.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Patient Orientation Images\Group2\human_b_8.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00004.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\Inject_button.bmp
# End Source File
# Begin Source File

SOURCE=.\res\jintai.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\Locator_button.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\PQM_IMAGES\memory.ico
# End Source File
# Begin Source File

SOURCE=.\RES\PQM_LIST_IMAGES\mode_injector_active.ico
# End Source File
# Begin Source File

SOURCE=.\RES\PQM_LIST_IMAGES\mode_injector_dis.ico
# End Source File
# Begin Source File

SOURCE=.\RES\PQM_LIST_IMAGES\mode_injector_normal.ico
# End Source File
# Begin Source File

SOURCE=.\RES\PQM_LIST_IMAGES\mode_moveBed_active.ico
# End Source File
# Begin Source File

SOURCE=.\RES\PQM_LIST_IMAGES\mode_moveBed_dis.ico
# End Source File
# Begin Source File

SOURCE=.\RES\PQM_LIST_IMAGES\mode_moveBed_normal.ico
# End Source File
# Begin Source File

SOURCE=.\RES\PQM_LIST_IMAGES\mode_Voice_active.ico
# End Source File
# Begin Source File

SOURCE=.\RES\PQM_LIST_IMAGES\mode_Voice_dis.ico
# End Source File
# Begin Source File

SOURCE=.\RES\PQM_LIST_IMAGES\mode_Voice_normal.ico
# End Source File
# Begin Source File

SOURCE=".\RES\Coil Selection Images\MoveFreeLeft.bmp"
# End Source File
# Begin Source File

SOURCE=".\RES\Coil Selection Images\MoveFreeRight.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\Pass_button.bmp
# End Source File
# Begin Source File

SOURCE=".\RES\Patient Orientation Images\Patient_orientation_dialog.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\Pause_button.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\PQM_LIST_IMAGES\Pause_Protocol.ico
# End Source File
# Begin Source File

SOURCE=.\res\Pelvis.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\PQM_LIST_IMAGES\Plan_01.ico
# End Source File
# Begin Source File

SOURCE=.\RES\PQM_LIST_IMAGES\Plan_active.ico
# End Source File
# Begin Source File

SOURCE=.\RES\PQM_LIST_IMAGES\Plan_dis.ico
# End Source File
# Begin Source File

SOURCE=.\RES\PQM_LIST_IMAGES\PlannedProtocol_Image.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM.ico
# End Source File
# Begin Source File

SOURCE=.\res\PQM.rc2
# End Source File
# Begin Source File

SOURCE=.\PQM.reg
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\PQM_LATEST.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQMDoc.ico
# End Source File
# Begin Source File

SOURCE=.\RES\PQMListCtrl_Area_Image.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\PQMSeq_Status_Current.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\PQMSeq_Status_Done.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\PQMSeq_Status_Error.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\PQMSeq_Status_Wait.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\Preview_button.bmp
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\Resume_button.bmp
# End Source File
# Begin Source File

SOURCE=".\res\Patient Orientation Images\Group1\Combo2\rotate_left.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Patient Orientation Images\Group2\Combo2\rotate_left.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Patient Orientation Images\Group1\Combo2\rotate_liedown.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Patient Orientation Images\Group2\Combo2\rotate_liedown.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Patient Orientation Images\Group1\Combo2\rotate_right.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Patient Orientation Images\Group2\Combo2\rotate_right.bmp"
# End Source File
# Begin Source File

SOURCE=".\RES\Patient Orientation Images\Combo2\rotate_right.ico"
# End Source File
# Begin Source File

SOURCE=".\res\Patient Orientation Images\Group1\Combo2\rotate_turningup.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\Patient Orientation Images\Group2\Combo2\rotate_turningup.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\s_Abdomen.bmp
# End Source File
# Begin Source File

SOURCE=.\res\s_Abdomen.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\s_Ankle.bmp
# End Source File
# Begin Source File

SOURCE=.\res\s_Ankle.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\s_Chest.bmp
# End Source File
# Begin Source File

SOURCE=.\res\s_Chest.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\s_Cspine.bmp
# End Source File
# Begin Source File

SOURCE=.\res\s_Cspine.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\s_Extremity.bmp
# End Source File
# Begin Source File

SOURCE=.\res\s_Extremity.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\s_Extremity2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\s_Extremity2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\s_Hand.bmp
# End Source File
# Begin Source File

SOURCE=.\res\s_Hand.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\s_Head.bmp
# End Source File
# Begin Source File

SOURCE=.\res\s_Head.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\s_Pelvis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\s_Pelvis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\s_Shoulder.bmp
# End Source File
# Begin Source File

SOURCE=.\res\s_Shoulder.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\s_TLspine.bmp
# End Source File
# Begin Source File

SOURCE=.\res\s_TLspine.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_LIST_IMAGES\SAR_Abdomen.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_LIST_IMAGES\SAR_Ankle.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_LIST_IMAGES\SAR_Chest.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_LIST_IMAGES\SAR_Cspine.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_LIST_IMAGES\SAR_Extremity.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_LIST_IMAGES\SAR_Extremity2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_LIST_IMAGES\SAR_Hand.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_LIST_IMAGES\SAR_Head.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_LIST_IMAGES\SAR_Pelvis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_LIST_IMAGES\SAR_Shoulder.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_LIST_IMAGES\SAR_TLspine.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\ScanAnatomy_button_Image.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\ScanEdit_button.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\PQM_IMAGES\ScanOffset.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ScanOffset\ScanOffset_R_Line.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\ScanOffset\ScanOffset_x.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\ScanOffset\ScanOffset_y.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\ScanOffset\ScanOffset_z.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Scrollbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SelectedChekbox.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Shoulder.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Slider_Thumb.BMP
# End Source File
# Begin Source File

SOURCE=".\RES\Coil Selection Images\SpineLeft.bmp"
# End Source File
# Begin Source File

SOURCE=".\RES\Coil Selection Images\SpineLeft_Right.bmp"
# End Source File
# Begin Source File

SOURCE=".\RES\Coil Selection Images\SpineRight.bmp"
# End Source File
# Begin Source File

SOURCE=".\RES\Coil Selection Images\SpineRight_Right.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\Status_Current_Disable.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\Status_Current_Enable.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\Status_Done_Disable.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\Status_Done_Enable.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\Status_Fail_Disable.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\Status_Fail_Enable.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\PQM_IMAGES\Status_Paused_Enable.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\temp_cfa.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TLspine.bmp
# End Source File
# Begin Source File

SOURCE=.\res\UncheckedRadioButton.bmp
# End Source File
# Begin Source File

SOURCE=.\res\UnSelectedChekbox.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\PQM_IMAGES\untitled1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PQM_IMAGES\Utility_button.bmp
# End Source File
# Begin Source File

SOURCE=".\res\Patient Orientation Images\Group1\Combo3\view_r_side_foot.bmp"
# End Source File
# Begin Source File

SOURCE=".\RES\Patient Orientation Images\Combo3\view_r_side_foot.ico"
# End Source File
# Begin Source File

SOURCE=".\res\Patient Orientation Images\Group1\Combo3\view_r_side_head.bmp"
# End Source File
# Begin Source File

SOURCE=".\RES\Patient Orientation Images\Combo3\view_r_side_head.ico"
# End Source File
# Begin Source File

SOURCE=.\RES\PQM_IMAGES\WSE.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\RES\PQM_LIST_IMAGES\10sec.ico
# End Source File
# Begin Source File

SOURCE=.\RES\PQM_LIST_IMAGES\6min_cant_wait.ico
# End Source File
# Begin Source File

SOURCE=.\RES\PQM_LIST_IMAGES\6min_wait.ico
# End Source File
# Begin Source File

SOURCE=.\RES\PQM_LIST_IMAGES\AutoScan_Protocol_10sec.ico
# End Source File
# Begin Source File

SOURCE=.\RES\PQM_LIST_IMAGES\AutoScan_Protocol_6min_cant_wait.ico
# End Source File
# Begin Source File

SOURCE=.\RES\PQM_LIST_IMAGES\AutoScan_Protocol_6min_wait.ico
# End Source File
# Begin Source File

SOURCE=.\RES\PQM_LIST_IMAGES\AutoScan_Protocol_SAE.ico
# End Source File
# Begin Source File

SOURCE=.\RES\PQM_LIST_IMAGES\Delay_protocol.ico
# End Source File
# Begin Source File

SOURCE=".\RES\ScanOffset Images\image_offset_human.bmp"
# End Source File
# Begin Source File

SOURCE=".\RES\ScanOffset Images\move_couch_1.bmp"
# End Source File
# Begin Source File

SOURCE=".\RES\ScanOffset Images\move_couch_2.bmp"
# End Source File
# Begin Source File

SOURCE=".\RES\ScanOffset Images\move_couch_center.bmp"
# End Source File
# Begin Source File

SOURCE=".\RES\ScanOffset Images\move_couch_human_left_back.bmp"
# End Source File
# Begin Source File

SOURCE=".\RES\ScanOffset Images\move_couch_human_left_down.bmp"
# End Source File
# Begin Source File

SOURCE=".\RES\ScanOffset Images\move_couch_human_left_front.bmp"
# End Source File
# Begin Source File

SOURCE=".\RES\ScanOffset Images\move_couch_human_left_up.bmp"
# End Source File
# Begin Source File

SOURCE=".\RES\ScanOffset Images\move_couch_human_right_back.bmp"
# End Source File
# Begin Source File

SOURCE=".\RES\ScanOffset Images\move_couch_human_right_down.bmp"
# End Source File
# Begin Source File

SOURCE=".\RES\ScanOffset Images\move_couch_human_right_front.bmp"
# End Source File
# Begin Source File

SOURCE=".\RES\ScanOffset Images\move_couch_human_right_up.bmp"
# End Source File
# Begin Source File

SOURCE=".\RES\ScanOffset Images\Move_Couch_Left_Button.bmp"
# End Source File
# Begin Source File

SOURCE=".\RES\ScanOffset Images\Move_Couch_Right_Button.bmp"
# End Source File
# Begin Source File

SOURCE=.\RES\PQM_LIST_IMAGES\Region_Unselect.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\PQM_LIST_IMAGES\SAE.ico
# End Source File
# Begin Source File

SOURCE=.\RES\PQM_IMAGES\ScanOffset_Active.ico
# End Source File
# Begin Source File

SOURCE=.\RES\ScanOffset\ScanOffset_L_Line.bmp
# End Source File
# Begin Source File

SOURCE=".\RES\Coil Selection Images\SpineMiddle.bmp"
# End Source File
# End Target
# End Project

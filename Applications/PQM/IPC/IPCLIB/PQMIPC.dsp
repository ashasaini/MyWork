# Microsoft Developer Studio Project File - Name="PQMIPC" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=PQMIPC - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PQMIPC.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PQMIPC.mak" CFG="PQMIPC - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PQMIPC - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "PQMIPC - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/MRI/13690/MRMPLUS/Sources/Acquire/PQM/IPC/IPCLIB", HDOEAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PQMIPC - Win32 Release"

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
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 MPlusConfig.lib MPlusErrorHandler.lib MplusEventLog.lib MPlusErrorDisplayLibrary.lib MQRT.lib Vfstudy.lib Win32Dep.lib OSIndepdnt.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /nodefaultlib:"msvcrtd.lib" /libpath:"$(MPLUSHOME)\lib\Release" /libpath:"..\..\..\..\libraries\OSIndepdnt\Release" /libpath:"..\..\..\..\libraries\Win32Dep\Release" /libpath:"..\..\..\..\SharedMPlus\lib\Release"
# SUBTRACT LINK32 /pdb:none
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /Zi /Od /I "$(MRMP_HOME)\Include" /I "$(MRMP_Home)\Sources\Acquire" /I "$(MPLUSHOME)\Include" /D "__INTTYPES_H__" /D "_MSXMLPARSER" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "WIN32" /D "__NUTC__" /D "__WIN_NATIVE_PORTING__" /D "__WNP_SIGNAL__" /D "MRI" /D "IRIX65" /D "__WNP_PATNI__" /D "PQM_MODE" /D "_SDE_WIN32_" /D "__WNP_MPLUS__" /D "STL_PORT" /D "ACQINFO_RM" /Fr /FD /EHa /c
# SUBTRACT CPP /WX
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "$(MRMP_Home)\Sources\Acquire\PQM\GUI" /d "NDEBUG" /d "_UNICODE" /d "_AFXDLL" /d "WIN32"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=move Release\PQMIPC.lib $(MRMP_HOME)\Lib\Release
# End Special Build Tool

!ELSEIF  "$(CFG)" == "PQMIPC - Win32 Debug"

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
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 MPlusConfigd.lib MPlusErrorHandlerd.lib MplusEventLogd.lib MPlusErrorDisplayLibraryd.lib MQRT.lib Vfstudy.lib Win32Dep.lib OSIndepdnt.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"$(MPLUSHOME)\lib\debug" /libpath:"..\..\..\..\libraries\OSIndepdnt\Debug" /libpath:"..\..\..\..\libraries\Win32Dep\Debug" /libpath:"..\..\..\..\SharedMPlus\lib\Debug"
# SUBTRACT LINK32 /incremental:no
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "$(MRMP_HOME)\Include" /I "$(MRMP_Home)\Sources\Acquire" /I "$(MPLUSHOME)\Include" /D "__INTTYPES_H__" /D "_MSXMLPARSER" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "WIN32" /D "__NUTC__" /D "__WIN_NATIVE_PORTING__" /D "__WNP_SIGNAL__" /D "MRI" /D "IRIX65" /D "__WNP_PATNI__" /D "PQM_MODE" /D "__WNP_MPLUS__" /D "_SDE_WIN32_" /D "STL_PORT" /D "ACQINFO_RM" /FR /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "$(MRMP_Home)\Sources\Acquire\PQM\GUI" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD LIB32 /out:"Debug\PQMIPCd.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=move Debug\PQMIPCd.lib $(MRMP_HOME)\Lib\Debug
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "PQMIPC - Win32 Release"
# Name "PQMIPC - Win32 Debug"
# Begin Group "Application"

# PROP Default_Filter ""
# Begin Group "App SRC"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "AutoMap and AutoShim"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AutoInsertMapManager.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoInsertMapOperation.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoInsertProcedure.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoInsertProtConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoInsertProtocol.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoInsertProtocolManager.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoInsertProtocolOperation.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoInsertProtocolStudy.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoInsertProtocolStudyManager.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoInsertShimManager.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoInsertShimOperation.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoInsertXmlData.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoInsertXmlDataManager.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoInsertXMLReader.cpp
# End Source File
# Begin Source File

SOURCE=.\LibPLTreeWrapper.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\AcqManNotifier.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoLocatorParameterSetter.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoLocatorPostProc.cpp
# End Source File
# Begin Source File

SOURCE=.\CFShiftCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\CheckSequence.cpp
# End Source File
# Begin Source File

SOURCE=.\CoilChannelMode.cpp
# End Source File
# Begin Source File

SOURCE=.\CoilSelectionTool.cpp
# End Source File
# Begin Source File

SOURCE=.\Color.cpp
# End Source File
# Begin Source File

SOURCE=.\DBServerWrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\DefaultPGUIDXmlDataManager.cpp
# End Source File
# Begin Source File

SOURCE=.\DefaultPGUIDXMLReader.cpp
# End Source File
# Begin Source File

SOURCE=.\Electrode.cpp
# End Source File
# Begin Source File

SOURCE=.\GateMode.cpp
# End Source File
# Begin Source File

SOURCE=.\GateParamSetter.cpp
# End Source File
# Begin Source File

SOURCE=.\GPLibAdapter.cpp
# End Source File
# Begin Source File

SOURCE=.\IInterlockData.cpp
# End Source File
# Begin Source File

SOURCE=.\ILicenseInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\InterlockDB.cpp
# End Source File
# Begin Source File

SOURCE=.\IObjectManager.cpp
# End Source File
# Begin Source File

SOURCE=.\IRSListner.cpp
# End Source File
# Begin Source File

SOURCE=.\LibSARMgrClientAppInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\MasterSlave.cpp
# End Source File
# Begin Source File

SOURCE=.\MoveCouch.cpp
# End Source File
# Begin Source File

SOURCE=.\OlpTemp.cpp
# End Source File
# Begin Source File

SOURCE=.\PASCOMInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\PeerSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\PGUIData.cpp
# End Source File
# Begin Source File

SOURCE=.\PlanDuplicateSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\PlanDuplicateXMLReader.cpp
# End Source File
# Begin Source File

SOURCE=.\PosSlabInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMAdapter.cpp
# End Source File
# Begin Source File

SOURCE=.\PqmAuditLog.cpp
# End Source File
# Begin Source File

SOURCE=.\PqmAutoInsert.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMdBdt.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMDBSALib.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMDebugInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMIPC.cpp
# End Source File
# Begin Source File

SOURCE=.\PqmIpcManager.cpp
# End Source File
# Begin Source File

SOURCE=.\PqmLocalShimming.cpp
# End Source File
# Begin Source File

SOURCE=.\PqmMRSScan.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMNotifier.cpp
# End Source File
# Begin Source File

SOURCE=.\PqmPathData.cpp
# End Source File
# Begin Source File

SOURCE=.\PqmPrMode.cpp
# End Source File
# Begin Source File

SOURCE=.\PqmProbe.cpp
# End Source File
# Begin Source File

SOURCE=.\PqmProbeServer.cpp
# End Source File
# Begin Source File

SOURCE=.\PqmProcon.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMProConAdapter.cpp
# End Source File
# Begin Source File

SOURCE=.\PqmProconNotifier.cpp
# End Source File
# Begin Source File

SOURCE=.\PqmProtocol.cpp
# End Source File
# Begin Source File

SOURCE=.\PqmPrTRSC.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMSAR.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMShimSharedData.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\PqmSpeeder.cpp
# End Source File
# Begin Source File

SOURCE=.\PqmUtilities.cpp
# End Source File
# Begin Source File

SOURCE=.\SAEInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\ScanAnatomyReader.cpp
# End Source File
# Begin Source File

SOURCE=.\ScanCommentXMLReader.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectionCoilData.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectionSectionData.cpp
# End Source File
# Begin Source File

SOURCE=.\SFTMenuXMLReader.cpp
# End Source File
# Begin Source File

SOURCE=.\ShimmingRequest.cpp
# End Source File
# Begin Source File

SOURCE=.\SocketMsgListThreadSafe.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TiPrepUpdater.cpp
# End Source File
# Begin Source File

SOURCE=.\VarFieldHandle.cpp
# End Source File
# Begin Source File

SOURCE=.\Vector2D.cpp
# End Source File
# Begin Source File

SOURCE=.\Vector3D.cpp
# End Source File
# End Group
# Begin Group "App HDR"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "AutoMap and AutoShim HDRs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AutoInsertConstant.h
# End Source File
# Begin Source File

SOURCE=.\AutoInsertMapManager.h
# End Source File
# Begin Source File

SOURCE=.\AutoInsertMapOperation.h
# End Source File
# Begin Source File

SOURCE=.\AutoInsertProcedure.h
# End Source File
# Begin Source File

SOURCE=.\AutoInsertProtConfig.h
# End Source File
# Begin Source File

SOURCE=.\AutoInsertProtocol.h
# End Source File
# Begin Source File

SOURCE=.\AutoInsertProtocolManager.h
# End Source File
# Begin Source File

SOURCE=.\AutoInsertProtocolOperation.h
# End Source File
# Begin Source File

SOURCE=.\AutoInsertProtocolStudy.h
# End Source File
# Begin Source File

SOURCE=.\AutoInsertProtocolStudyManager.h
# End Source File
# Begin Source File

SOURCE=.\AutoInsertShimManager.h
# End Source File
# Begin Source File

SOURCE=.\AutoInsertShimOperation.h
# End Source File
# Begin Source File

SOURCE=.\AutoInsertXmlData.h
# End Source File
# Begin Source File

SOURCE=.\AutoInsertXmlDataManager.h
# End Source File
# Begin Source File

SOURCE=.\AutoInsertXMLReader.h
# End Source File
# Begin Source File

SOURCE=.\LibPLTreeWrapper.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\AcqManNotifier.h
# End Source File
# Begin Source File

SOURCE=.\AutoLocatorParameterSetter.h
# End Source File
# Begin Source File

SOURCE=.\AutoLocatorPostProc.h
# End Source File
# Begin Source File

SOURCE=.\CFShiftCheck.h
# End Source File
# Begin Source File

SOURCE=.\CheckSequence.h
# End Source File
# Begin Source File

SOURCE=.\CoilChannelMode.h
# End Source File
# Begin Source File

SOURCE=.\CoilSelectionTool.h
# End Source File
# Begin Source File

SOURCE=.\Color.h
# End Source File
# Begin Source File

SOURCE=.\DBServerWrapper.h
# End Source File
# Begin Source File

SOURCE=.\DefaultPGUIDXmlDataManager.h
# End Source File
# Begin Source File

SOURCE=.\DefaultPGUIDXMLReader.h
# End Source File
# Begin Source File

SOURCE=.\Electrode.h
# End Source File
# Begin Source File

SOURCE=.\GateMode.h
# End Source File
# Begin Source File

SOURCE=.\GateParamSetter.h
# End Source File
# Begin Source File

SOURCE=.\GPLibAdapter.h
# End Source File
# Begin Source File

SOURCE=.\IInterlockData.h
# End Source File
# Begin Source File

SOURCE=.\ILicenseInfo.h
# End Source File
# Begin Source File

SOURCE=.\InterlockDB.h
# End Source File
# Begin Source File

SOURCE=.\IObjectManager.h
# End Source File
# Begin Source File

SOURCE=.\IRSListner.h
# End Source File
# Begin Source File

SOURCE=.\LibSARMgrClientAppInterface.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\Include\tami\common\machdefs.h
# End Source File
# Begin Source File

SOURCE=.\MasterSlave.h
# End Source File
# Begin Source File

SOURCE=.\MoveCouch.h
# End Source File
# Begin Source File

SOURCE=.\OlpTemp.h
# End Source File
# Begin Source File

SOURCE=.\PASCOMInterface.h
# End Source File
# Begin Source File

SOURCE=.\PeerSocket.h
# End Source File
# Begin Source File

SOURCE=.\PGUIData.h
# End Source File
# Begin Source File

SOURCE=.\PlanDuplicateSettings.h
# End Source File
# Begin Source File

SOURCE=.\PlanDuplicateXMLReader.h
# End Source File
# Begin Source File

SOURCE=.\PosSlabInfo.h
# End Source File
# Begin Source File

SOURCE=.\PQMAdapter.h
# End Source File
# Begin Source File

SOURCE=.\PqmAuditLog.h
# End Source File
# Begin Source File

SOURCE=.\PqmAutoInsert.h
# End Source File
# Begin Source File

SOURCE=.\PQMdBdt.h
# End Source File
# Begin Source File

SOURCE=.\PQMDBSALib.h
# End Source File
# Begin Source File

SOURCE=.\PQMDebugInfo.h
# End Source File
# Begin Source File

SOURCE=.\PQMIPC.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\Include\PQM\PQMIPCManager.h
# End Source File
# Begin Source File

SOURCE=.\PqmLocalShimming.h
# End Source File
# Begin Source File

SOURCE=.\PqmMRSScan.h
# End Source File
# Begin Source File

SOURCE=.\PQMNotifier.h
# End Source File
# Begin Source File

SOURCE=.\PqmPrMode.h
# End Source File
# Begin Source File

SOURCE=.\PqmProbe.h
# End Source File
# Begin Source File

SOURCE=.\PqmProbeServer.h
# End Source File
# Begin Source File

SOURCE=.\PQMProConAdapter.h
# End Source File
# Begin Source File

SOURCE=.\PqmProconNotifier.h
# End Source File
# Begin Source File

SOURCE=.\PqmProtocol.h
# End Source File
# Begin Source File

SOURCE=.\PqmPrTRSC.h
# End Source File
# Begin Source File

SOURCE=.\PQMSAR.h
# End Source File
# Begin Source File

SOURCE=.\PqmShimSharedData.h
# End Source File
# Begin Source File

SOURCE=.\PqmShimStruct.h
# End Source File
# Begin Source File

SOURCE=.\PQMSocket.h
# End Source File
# Begin Source File

SOURCE=.\PqmSpeeder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\include\PQM\PqmUtilities.h
# End Source File
# Begin Source File

SOURCE=.\ProtocolUtil.h
# End Source File
# Begin Source File

SOURCE=.\RMCMsg.h
# End Source File
# Begin Source File

SOURCE=.\SAEInfo.h
# End Source File
# Begin Source File

SOURCE=.\ScanAnatomy.h
# End Source File
# Begin Source File

SOURCE=.\ScanAnatomyData.h
# End Source File
# Begin Source File

SOURCE=.\ScanAnatomyReader.h
# End Source File
# Begin Source File

SOURCE=.\ScanCommentXMLReader.h
# End Source File
# Begin Source File

SOURCE=.\SelectionCoilData.h
# End Source File
# Begin Source File

SOURCE=.\SelectionSectionData.h
# End Source File
# Begin Source File

SOURCE=.\SFTMenuXMLReader.h
# End Source File
# Begin Source File

SOURCE=.\ShimCommon.h
# End Source File
# Begin Source File

SOURCE=.\ShimmingRequest.h
# End Source File
# Begin Source File

SOURCE=.\ShimmingRequestStructures.h
# End Source File
# Begin Source File

SOURCE=.\SocketMsgListThreadSafe.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TiPrepUpdater.h
# End Source File
# Begin Source File

SOURCE=.\VarFieldHandle.h
# End Source File
# Begin Source File

SOURCE=.\Vector2D.h
# End Source File
# Begin Source File

SOURCE=.\Vector3D.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\PQMIPC.ico
# End Source File
# Begin Source File

SOURCE=.\PQMIPC.rc
# End Source File
# Begin Source File

SOURCE=.\res\PQMIPC.rc2
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# End Group
# End Group
# Begin Group "PQM"

# PROP Default_Filter ""
# Begin Group "PQM SRC"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CheckFOVInMap.cpp
# End Source File
# Begin Source File

SOURCE=.\Coil.cpp
# End Source File
# Begin Source File

SOURCE=.\CoilDBV.cpp
# End Source File
# Begin Source File

SOURCE=.\CoilSet.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorData.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\Couch.cpp
# End Source File
# Begin Source File

SOURCE=.\CouchConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\DataManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ICoilData.cpp
# End Source File
# Begin Source File

SOURCE=.\IData.cpp
# End Source File
# Begin Source File

SOURCE=.\IPathData.cpp
# End Source File
# Begin Source File

SOURCE=.\ISectionData.cpp
# End Source File
# Begin Source File

SOURCE=.\PathDB.cpp
# End Source File
# Begin Source File

SOURCE=.\Pqm.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMCoilData.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMLogMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMSectionData.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMStudy.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMStudyMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMXMLParser.cpp
# End Source File
# Begin Source File

SOURCE=.\SectionDB.cpp
# End Source File
# Begin Source File

SOURCE=.\SharedMemory.cpp
# End Source File
# Begin Source File

SOURCE=.\VoiceManager.cpp
# End Source File
# End Group
# Begin Group "PQM HDR"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CheckFOVInMap.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\Include\PQM\coil.h
# End Source File
# Begin Source File

SOURCE=.\CoilDBV.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\Include\PQM\CoilDefinitions.h
# End Source File
# Begin Source File

SOURCE=.\CoilSet.h
# End Source File
# Begin Source File

SOURCE=.\ColorData.h
# End Source File
# Begin Source File

SOURCE=.\ColorSetting.h
# End Source File
# Begin Source File

SOURCE=.\Couch.h
# End Source File
# Begin Source File

SOURCE=.\CouchConfig.h
# End Source File
# Begin Source File

SOURCE=.\DataManager.h
# End Source File
# Begin Source File

SOURCE=.\ICoilData.h
# End Source File
# Begin Source File

SOURCE=.\IData.h
# End Source File
# Begin Source File

SOURCE=.\ISectionData.h
# End Source File
# Begin Source File

SOURCE=.\PathDB.h
# End Source File
# Begin Source File

SOURCE=.\Pqm.h
# End Source File
# Begin Source File

SOURCE=.\PQMCoilData.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\Include\PQM\pqmcommon.h
# End Source File
# Begin Source File

SOURCE=.\PQMConfig.h
# End Source File
# Begin Source File

SOURCE=.\PqmPathData.h
# End Source File
# Begin Source File

SOURCE=.\PQMSectionData.h
# End Source File
# Begin Source File

SOURCE=.\PQMStudy.h
# End Source File
# Begin Source File

SOURCE=.\PQMStudyMgr.h
# End Source File
# Begin Source File

SOURCE=.\PQMXMLParser.h
# End Source File
# Begin Source File

SOURCE=.\SectionDB.h
# End Source File
# Begin Source File

SOURCE=.\SharedMemory.h
# End Source File
# Begin Source File

SOURCE=.\StringDefines.h
# End Source File
# Begin Source File

SOURCE=.\VoiceManager.h
# End Source File
# End Group
# End Group
# Begin Group "ACQMAN"

# PROP Default_Filter ""
# Begin Group "Main"

# PROP Default_Filter ""
# Begin Group "Main SRC"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\InterlockData.cpp
# End Source File
# Begin Source File

SOURCE=.\PqmPmAcqman.cpp
# End Source File
# Begin Source File

SOURCE=.\ScanProcedure.cpp
# End Source File
# End Group
# Begin Group "Main HDR"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\InterlockData.h
# End Source File
# Begin Source File

SOURCE=.\PqmPmAcqman.h
# End Source File
# Begin Source File

SOURCE=.\ScanProcedure.h
# End Source File
# End Group
# End Group
# Begin Group "Scan Transactions"

# PROP Default_Filter ""
# Begin Group "ST SRC"

# PROP Default_Filter ""
# Begin Group "PreScan src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AcqSetFlag.cpp
# End Source File
# Begin Source File

SOURCE=.\PreScan.cpp
# End Source File
# Begin Source File

SOURCE=.\PreScanAMB.cpp
# End Source File
# Begin Source File

SOURCE=.\PreScanAPC.cpp
# End Source File
# Begin Source File

SOURCE=.\PreScanCFA.cpp
# End Source File
# Begin Source File

SOURCE=.\PreScanCoil.cpp
# End Source File
# Begin Source File

SOURCE=.\PreScanFirst.cpp
# End Source File
# Begin Source File

SOURCE=.\PreScanModeAll.cpp
# End Source File
# Begin Source File

SOURCE=.\PreScanPHCOR.cpp
# End Source File
# Begin Source File

SOURCE=.\PreScanRFL.cpp
# End Source File
# Begin Source File

SOURCE=.\PreScanTx.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\AcqInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\InterLock.cpp
# End Source File
# Begin Source File

SOURCE=.\Scan.cpp
# End Source File
# Begin Source File

SOURCE=.\ScanTransaction.cpp
# End Source File
# Begin Source File

SOURCE=.\ShimCtrl.cpp
# End Source File
# End Group
# Begin Group "ST HDR"

# PROP Default_Filter ""
# Begin Group "PreScan HDR"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AcqSetFlag.h
# End Source File
# Begin Source File

SOURCE=.\PreScan.h
# End Source File
# Begin Source File

SOURCE=.\PreScanAMB.h
# End Source File
# Begin Source File

SOURCE=.\PreScanAPC.h
# End Source File
# Begin Source File

SOURCE=.\PreScanCFA.h
# End Source File
# Begin Source File

SOURCE=.\PreScanCoil.h
# End Source File
# Begin Source File

SOURCE=.\PreScanFirst.h
# End Source File
# Begin Source File

SOURCE=.\PreScanModeAll.h
# End Source File
# Begin Source File

SOURCE=.\PreScanPHCOR.h
# End Source File
# Begin Source File

SOURCE=.\PreScanRFL.h
# End Source File
# Begin Source File

SOURCE=.\PreScanTx.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\AcqInfo.h
# End Source File
# Begin Source File

SOURCE=.\InterLock.h
# End Source File
# Begin Source File

SOURCE=.\Scan.h
# End Source File
# Begin Source File

SOURCE=.\ScanTransaction.h
# End Source File
# Begin Source File

SOURCE=.\ShimCtrl.h
# End Source File
# End Group
# End Group
# Begin Group "Messages"

# PROP Default_Filter ""
# Begin Group "MSG SRC"

# PROP Default_Filter ""
# End Group
# Begin Group "MSG HDR"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PQMMessageStructures.h
# End Source File
# End Group
# End Group
# Begin Group "CDS"

# PROP Default_Filter ""
# Begin Group "CDS SRC"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CDSPrescan.cpp
# End Source File
# Begin Source File

SOURCE=.\CDSUtilities.cpp
# End Source File
# Begin Source File

SOURCE=.\pqmCDS.cpp
# End Source File
# Begin Source File

SOURCE=.\pqmCDSDebug.cpp
# End Source File
# Begin Source File

SOURCE=.\pqmCDSInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\PrescanCFARetry.cpp
# End Source File
# End Group
# Begin Group "CDS HDR"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CDSCommonStructures.h
# End Source File
# Begin Source File

SOURCE=.\CDSConstants.h
# End Source File
# Begin Source File

SOURCE=.\CDSPrescan.h
# End Source File
# Begin Source File

SOURCE=.\CDSUtilities.h
# End Source File
# Begin Source File

SOURCE=.\pqmCDS.h
# End Source File
# Begin Source File

SOURCE=.\pqmCDSDebug.h
# End Source File
# Begin Source File

SOURCE=.\pqmCDSInfo.h
# End Source File
# Begin Source File

SOURCE=.\PrescanCFARetry.h
# End Source File
# End Group
# End Group
# Begin Group "AMB"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\pqmAMB.cpp
# End Source File
# Begin Source File

SOURCE=.\pqmAMB.h
# End Source File
# Begin Source File

SOURCE=.\pqmAMBConstants.cpp
# End Source File
# Begin Source File

SOURCE=.\pqmAMBConstants.h
# End Source File
# Begin Source File

SOURCE=.\pqmAMBStructs.cpp
# End Source File
# Begin Source File

SOURCE=.\pqmAMBStructs.h
# End Source File
# Begin Source File

SOURCE=.\pqmAMBUtilities.cpp
# End Source File
# Begin Source File

SOURCE=.\pqmAMBUtilities.h
# End Source File
# Begin Source File

SOURCE=.\PQMPmSARManager.cpp
# End Source File
# Begin Source File

SOURCE=.\PQMPmSARManager.h
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

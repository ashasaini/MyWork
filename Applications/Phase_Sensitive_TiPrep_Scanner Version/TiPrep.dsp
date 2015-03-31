# Microsoft Developer Studio Project File - Name="TiPrep" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=TiPrep - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TiPrep.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TiPrep.mak" CFG="TiPrep - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TiPrep - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "TiPrep - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TiPrep - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GR /GX /ZI /I "$(MPLUSHOME)/include" /I "$(MRMP_HOME)/Include" /I "$(MPLUSSHARE)/include" /I "$(MPLUSHOME)/etc/ICF/include/common/src" /I "$(MPLUSHOME)/etc/ICF/include/common/idl/Release" /I "$(MPLUSHOME)/etc/ICF/include/icf_base/src" /I "$(MPLUSHOME)/include/CosVis" /I "$(MPLUSHOME)/include/cosvis/develop/link" /I "$(MPLUSHOME)/etc/ICF/include/icf_render/src" /I "$(MPLUSSHARE)/include/FEX" /I "$(MPLUSSHARE)/include/MPlusDicomTag" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "UNICODE" /D "_UNICODE" /D _WIN32_WINNT=0x0500 /D "__WIN_NATIVE_PORTING__" /D "V2_20_ROI_DEFIX" /FR /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /WX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL" /d "V2_20_ROI_DEFIX"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Ws2_32.lib MPlusUICommon.lib MPlusComm.lib comsupp.lib MamROIOverlayControlhandlerDll.lib MPlusMamROIManager.lib ICMLib.lib MPlusOpLog.lib MPlusErrorDisplay.lib MplusErrorHandler.lib MplusEventLog.lib MPlusTrace.lib Psapi.lib MPlusGLCommon.lib vf.lib DBSAVR.lib DBSALib.lib MRErrorLogLibrary.lib MRTraceLogLibrary.lib PDataParserU.lib PDataTypesU.lib shlwapi.lib CreateSeriesNumber.lib MPlusDBServerIFClasses.lib libWNP.lib wsock32.lib libdll.lib libwellks.lib libIRS.lib libmathServer.lib libinit.lib LibVfToUI.lib libproMsg.lib libpl.lib vf.lib libWNP.lib libdll.lib libTl.lib libgpdb.lib gdbm.lib libmathServer.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /incremental:yes /debug /machine:I386 /libpath:"$(MRMP_HOME)\Lib\release" /libpath:"$(MPLUSHOME)/lib/release" /libpath:"$(MPLUSSHARE)/lib/release" /libpath:"$(MPLUSHOME)/etc/icf/lib/release" /libpath:"$(MPLUSSHARE)\Lib\Release"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=move Release\TiPrep.exe $(MPLUSSHARE)\Bin\Release
# End Special Build Tool

!ELSEIF  "$(CFG)" == "TiPrep - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "$(MPLUSHOME)/include" /I "$(MRMP_HOME)/Include" /I "$(MPLUSSHARE)/include" /I "$(MPLUSHOME)/etc/ICF/include/common/src" /I "$(MPLUSHOME)/etc/ICF/include/common/idl/Release" /I "$(MPLUSHOME)/etc/ICF/include/icf_base/src" /I "$(MPLUSHOME)/include/CosVis" /I "$(MPLUSHOME)/include/cosvis/develop/link" /I "$(MPLUSHOME)/etc/ICF/include/icf_render/src" /I "$(MPLUSSHARE)/include/FEX" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /D _WIN32_WINNT=0x0500 /D "__WIN_NATIVE_PORTING__" /D "V2_20_ROI_DEFIX" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Ws2_32.lib MPlusUICommond.lib MPlusComm_d.lib comsupp.lib MamROIOverlayControlhandlerDlld.lib MPlusMamROIManagerd.lib icf_alloc_ud.lib icf_base_ud.lib icf_render_tools_ud.lib icf_render_ud.lib icf_server_ud.lib icf_shared_ud.lib ICMLibd.lib MPlusOpLogd.lib MPlusErrorDisplayd.lib MplusErrorHandlerd.lib MplusEventLogd.lib MPlusTraced.lib Psapi.lib MPlusGLCommond.lib vf.lib DBSAVRd.lib DBSALibd.lib MRErrorLogLibraryd.lib MRTraceLogLibraryd.lib PDataParserUd.lib PDataTypesUd.lib shlwapi.lib CreateSeriesNumber.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"$(MRMP_HOME)\Lib\Debug" /libpath:"$(MPLUSHOME)\lib\debug" /libpath:"$(MPLUSSHARE)\lib\Debug" /libpath:"$(MPLUSHOME)\etc\icf\lib\Debug" /libpath:"$(MPLUSSHARE)\Lib\Debug"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=move Debug\TiPrep.exe $(MPLUSSHARE)\Bin\Debug
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "TiPrep - Win32 Release"
# Name "TiPrep - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AddOffset.cpp
# End Source File
# Begin Source File

SOURCE=.\AddOffsetDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ApplyTi.cpp
# End Source File
# Begin Source File

SOURCE=.\AvgFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\Calc_equn1.cpp
# End Source File
# Begin Source File

SOURCE=.\Calc_equn2.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcStrategy.cpp
# End Source File
# Begin Source File

SOURCE=.\CircleROIParams.cpp
# End Source File
# Begin Source File

SOURCE=.\CommonCalculations.cpp
# End Source File
# Begin Source File

SOURCE=.\CPercentBar.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\DisplayData.cpp
# End Source File
# Begin Source File

SOURCE=.\DPSComManager.cpp
# End Source File
# Begin Source File

SOURCE=.\EllipticalROIParams.cpp
# End Source File
# Begin Source File

SOURCE=.\ErrorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ErrorExt.cpp
# End Source File
# Begin Source File

SOURCE=.\ExtraPointsGen.cpp
# End Source File
# Begin Source File

SOURCE=.\ICMMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\IniFileReader.cpp
# End Source File
# Begin Source File

SOURCE=.\InitialGuess.cpp
# End Source File
# Begin Source File

SOURCE=.\IpFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\IRSListner.cpp
# End Source File
# Begin Source File

SOURCE=.\LineFitStrategy.cpp
# End Source File
# Begin Source File

SOURCE=.\LocalDBMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ManualInitialGuessDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ManualPhaseCorrDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ManualPhaseCorrection.cpp
# End Source File
# Begin Source File

SOURCE=.\MapWriter.cpp
# End Source File
# Begin Source File

SOURCE=.\MatrixSolver.cpp
# End Source File
# Begin Source File

SOURCE=.\NodeCopy.cpp
# End Source File
# Begin Source File

SOURCE=.\NodeCreate.cpp
# End Source File
# Begin Source File

SOURCE=.\PhaseCorrection.cpp
# End Source File
# Begin Source File

SOURCE=.\PolarityCorrection.cpp
# End Source File
# Begin Source File

SOURCE=.\PolarityCorrectionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PopupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProcessExec.cpp
# End Source File
# Begin Source File

SOURCE=.\ReconRawCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\Reconstruction.cpp
# End Source File
# Begin Source File

SOURCE=.\ReconstructionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ROICopyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ROIHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\ROIReader.cpp
# End Source File
# Begin Source File

SOURCE=.\SetROIParamsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SocketMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\SpinCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SrvrItem.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\T1Map.cpp
# End Source File
# Begin Source File

SOURCE=.\TiGraphCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\TiPrep.cpp
# End Source File
# Begin Source File

SOURCE=.\TiPrep.rc
# End Source File
# Begin Source File

SOURCE=.\TiPrepChildThread.cpp
# End Source File
# Begin Source File

SOURCE=.\TiPrepController.cpp
# End Source File
# Begin Source File

SOURCE=.\TiPrepDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TiPrepDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\TiPrepLogHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\TiPrepProgressDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TiPrepTrace.cpp
# End Source File
# Begin Source File

SOURCE=.\TiPrepView.cpp
# End Source File
# Begin Source File

SOURCE=.\TiProlimUpdater.cpp
# End Source File
# Begin Source File

SOURCE=.\vfcDBMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\VfcNodeCopy.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AboutDlg.h
# End Source File
# Begin Source File

SOURCE=.\AddOffset.h
# End Source File
# Begin Source File

SOURCE=.\AddOffsetDlg.h
# End Source File
# Begin Source File

SOURCE=.\ApplyTi.h
# End Source File
# Begin Source File

SOURCE=.\AvgFilter.h
# End Source File
# Begin Source File

SOURCE=.\Calc_equn1.h
# End Source File
# Begin Source File

SOURCE=.\Calc_equn2.h
# End Source File
# Begin Source File

SOURCE=.\CalcStrategy.h
# End Source File
# Begin Source File

SOURCE=.\CircleROIParams.h
# End Source File
# Begin Source File

SOURCE=.\CommonCalculations.h
# End Source File
# Begin Source File

SOURCE=.\CommonDefinitions.h
# End Source File
# Begin Source File

SOURCE=.\CPercentBar.h
# End Source File
# Begin Source File

SOURCE=.\CustomSocket.h
# End Source File
# Begin Source File

SOURCE=.\DisplayData.h
# End Source File
# Begin Source File

SOURCE=.\DPSComManager.h
# End Source File
# Begin Source File

SOURCE=.\EllipticalROIParams.h
# End Source File
# Begin Source File

SOURCE=.\ErrorDlg.h
# End Source File
# Begin Source File

SOURCE=.\ExtraPointsGen.h
# End Source File
# Begin Source File

SOURCE=.\ICMMgr.h
# End Source File
# Begin Source File

SOURCE=.\IniFileReader.h
# End Source File
# Begin Source File

SOURCE=.\InitialGuess.h
# End Source File
# Begin Source File

SOURCE=.\IpFrame.h
# End Source File
# Begin Source File

SOURCE=.\IRSListner.h
# End Source File
# Begin Source File

SOURCE=.\LineFitStrategy.h
# End Source File
# Begin Source File

SOURCE=.\LocalDBMgr.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\ManualInitialGuessDlg.h
# End Source File
# Begin Source File

SOURCE=.\ManualPhaseCorrDlg.h
# End Source File
# Begin Source File

SOURCE=.\ManualPhaseCorrection.h
# End Source File
# Begin Source File

SOURCE=.\MapWriter.h
# End Source File
# Begin Source File

SOURCE=.\MatrixSolver.h
# End Source File
# Begin Source File

SOURCE=.\NodeCopy.h
# End Source File
# Begin Source File

SOURCE=.\NodeCreate.h
# End Source File
# Begin Source File

SOURCE=.\PhaseCorrection.h
# End Source File
# Begin Source File

SOURCE=.\PolarityCorrection.h
# End Source File
# Begin Source File

SOURCE=.\PolarityCorrectionDlg.h
# End Source File
# Begin Source File

SOURCE=.\PopupDlg.h
# End Source File
# Begin Source File

SOURCE=.\ProcessExec.h
# End Source File
# Begin Source File

SOURCE=.\ReconRawCheck.h
# End Source File
# Begin Source File

SOURCE=.\Reconstruction.h
# End Source File
# Begin Source File

SOURCE=.\ReconstructionDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ROICopyDlg.h
# End Source File
# Begin Source File

SOURCE=.\ROIHandler.h
# End Source File
# Begin Source File

SOURCE=.\ROIReader.h
# End Source File
# Begin Source File

SOURCE=.\SetROIParamsDlg.h
# End Source File
# Begin Source File

SOURCE=.\SocketMgr.h
# End Source File
# Begin Source File

SOURCE=.\SpinCtrl.h
# End Source File
# Begin Source File

SOURCE=.\SrvrItem.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\T1Map.h
# End Source File
# Begin Source File

SOURCE=.\TiGraphCtrl.h
# End Source File
# Begin Source File

SOURCE=.\TiPrep.h
# End Source File
# Begin Source File

SOURCE=.\TiPrepChildThread.h
# End Source File
# Begin Source File

SOURCE=.\TiPrepController.h
# End Source File
# Begin Source File

SOURCE=.\TiPrepDlg.h
# End Source File
# Begin Source File

SOURCE=.\TiPrepDoc.h
# End Source File
# Begin Source File

SOURCE=.\TiPrepErrors.h
# End Source File
# Begin Source File

SOURCE=.\TiPrepLogHelper.h
# End Source File
# Begin Source File

SOURCE=.\TiPrepProgressDlg.h
# End Source File
# Begin Source File

SOURCE=.\TiPrepTrace.h
# End Source File
# Begin Source File

SOURCE=.\TiPrepView.h
# End Source File
# Begin Source File

SOURCE=.\TiProlimUpdater.h
# End Source File
# Begin Source File

SOURCE=.\vfcDBMgr.h
# End Source File
# Begin Source File

SOURCE=.\VfcNodeCopy.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\l_tiprep.ico
# End Source File
# Begin Source File

SOURCE=.\res\TiPrep.ico
# End Source File
# Begin Source File

SOURCE=.\res\TiPrep.rc2
# End Source File
# Begin Source File

SOURCE=.\res\tiprep_i.ico
# End Source File
# Begin Source File

SOURCE=.\res\TiPrepDoc.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\TiPrep.reg
# End Source File
# End Target
# End Project

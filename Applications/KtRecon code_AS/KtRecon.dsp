# Microsoft Developer Studio Project File - Name="KtRecon" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=KtRecon - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "KtRecon.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "KtRecon.mak" CFG="KtRecon - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "KtRecon - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "KtRecon - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "KtRecon - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GR /GX /ZI /I "$(MPLUSHOME)/include" /I "$(MRMP_HOME)/Include" /I "$(MPLUSSHARE)/include" /I "$(MPLUSHOME)/etc/ICF/include/common/src" /I "$(MPLUSHOME)/etc/ICF/include/common/idl/Release" /I "$(MPLUSHOME)/etc/ICF/include/icf_base/src" /I "$(MPLUSHOME)/include/CosVis" /I "$(MPLUSHOME)/include/cosvis/develop/link" /I "$(MPLUSHOME)/etc/ICF/include/icf_render/src" /I "$(MPLUSSHARE)/include/FEX" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "UNICODE" /D "_UNICODE" /D _WIN32_WINNT=0x0500 /D "__WIN_NATIVE_PORTING__" /FR /Yu"stdafx.h" /FD /c
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
# ADD LINK32 Ws2_32.lib MPlusUICommon.lib MPlusComm.lib comsupp.lib ICMLib.lib MPlusErrorDisplay.lib MplusErrorHandler.lib MPlusTrace.lib Psapi.lib MPlusOpLog.lib MPlusGLCommon.lib vf.lib DBSAVR.lib DBSALib.lib MRErrorLogLibrary.lib MRTraceLogLibrary.lib PDataParserU.lib PDataTypesU.lib shlwapi.lib MPlusDBServerIFClasses.lib libWNP.lib wsock32.lib libdll.lib libwellks.lib libIRS.lib libmathServer.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /incremental:yes /debug /machine:I386 /libpath:"$(MRMP_HOME)\Lib\release" /libpath:"$(MPLUSHOME)/lib/release" /libpath:"$(MPLUSSHARE)/lib/release" /libpath:"$(MPLUSHOME)/etc/icf/lib/release" /libpath:"$(MPLUSSHARE)\Lib\Release"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=move Release\KtRecon.exe $(MPLUSSHARE)\Bin\Release\KtRecon.exe
# End Special Build Tool

!ELSEIF  "$(CFG)" == "KtRecon - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "$(MPLUSHOME)/include" /I "$(MRMP_HOME)/Include" /I "$(MPLUSSHARE)/include" /I "$(MPLUSHOME)/etc/ICF/include/common/src" /I "$(MPLUSHOME)/etc/ICF/include/common/idl/Release" /I "$(MPLUSHOME)/etc/ICF/include/icf_base/src" /I "$(MPLUSHOME)/include/CosVis" /I "$(MPLUSHOME)/include/cosvis/develop/link" /I "$(MPLUSHOME)/etc/ICF/include/icf_render/src" /I "$(MPLUSSHARE)/include/FEX" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /D _WIN32_WINNT=0x0500 /D "__WIN_NATIVE_PORTING__" /FR /Yu"stdafx.h" /FD /GZ /c
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
PostBuild_Cmds=move Debug\KtRecon.exe $(MPLUSSHARE)\Bin\Debug\KtRecon.exe
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "KtRecon - Win32 Release"
# Name "KtRecon - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ErrorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ErrorExt.cpp
# End Source File
# Begin Source File

SOURCE=.\IpFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\KtCommandProcessor.cpp
# End Source File
# Begin Source File

SOURCE=.\KtDPSCommMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\KtEngineDataCopy.cpp
# End Source File
# Begin Source File

SOURCE=.\KtICMMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\KtIniFileReader.cpp
# End Source File
# Begin Source File

SOURCE=.\KtRecon.cpp
# End Source File
# Begin Source File

SOURCE=.\KtRecon.rc
# End Source File
# Begin Source File

SOURCE=.\KtReconController.cpp
# End Source File
# Begin Source File

SOURCE=.\KtReconDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\KtReconDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\KtReconExecProc.cpp
# End Source File
# Begin Source File

SOURCE=.\KtReconLogHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\KtReconTrace.cpp
# End Source File
# Begin Source File

SOURCE=.\KtReconView.cpp
# End Source File
# Begin Source File

SOURCE=.\KtReconViewer.cpp
# End Source File
# Begin Source File

SOURCE=.\KtVROMProc.cpp
# End Source File
# Begin Source File

SOURCE=.\LocalDBMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\PopupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SrvrItem.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CommonDefinitions.h
# End Source File
# Begin Source File

SOURCE=.\ErrorDlg.h
# End Source File
# Begin Source File

SOURCE=.\IpFrame.h
# End Source File
# Begin Source File

SOURCE=.\KtCommandProcessor.h
# End Source File
# Begin Source File

SOURCE=.\KtDPSCommMgr.h
# End Source File
# Begin Source File

SOURCE=.\KtEngineDataCopy.h
# End Source File
# Begin Source File

SOURCE=.\KtICMMgr.h
# End Source File
# Begin Source File

SOURCE=.\KtIniFileReader.h
# End Source File
# Begin Source File

SOURCE=.\KtRecon.h
# End Source File
# Begin Source File

SOURCE=.\KtReconController.h
# End Source File
# Begin Source File

SOURCE=.\KtReconDlg.h
# End Source File
# Begin Source File

SOURCE=.\KtReconDoc.h
# End Source File
# Begin Source File

SOURCE=.\KtReconErrors.h
# End Source File
# Begin Source File

SOURCE=.\KtReconExecProc.h
# End Source File
# Begin Source File

SOURCE=.\KtReconLogHelper.h
# End Source File
# Begin Source File

SOURCE=.\KtReconTrace.h
# End Source File
# Begin Source File

SOURCE=.\KtReconView.h
# End Source File
# Begin Source File

SOURCE=.\KtReconViewer.h
# End Source File
# Begin Source File

SOURCE=.\KtVROMProc.h
# End Source File
# Begin Source File

SOURCE=.\LocalDBMgr.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\PopupDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SrvrItem.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\KtRecon.ico
# End Source File
# Begin Source File

SOURCE=.\res\KtRecon.rc2
# End Source File
# Begin Source File

SOURCE=.\res\KtReconDoc.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\KtRecon.reg
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

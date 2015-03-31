# Microsoft Developer Studio Project File - Name="AdvCardiacGL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=AdvCardiacGL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AdvCardiacGL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AdvCardiacGL.mak" CFG="AdvCardiacGL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AdvCardiacGL - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "AdvCardiacGL - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AdvCardiacGL - Win32 Release"

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
# ADD CPP /nologo /MD /w /W0 /GX /ZI /Od /I "$(MPLUSHOME)/include" /I "$(MRMP_HOME)/Include" /I "$(MPLUSSHARE)/include" /I "$(MPLUSHOME)/etc/ICF/include/common/src" /I "$(MPLUSHOME)/etc/ICF/include/common/idl/Release" /I "$(MPLUSHOME)/include/CosVis" /I "$(MPLUSHOME)/include/cosvis/develop/link" /I "GNORE:4786" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D _WIN32_WINNT=0x0500 /D "__WIN_NATIVE_PORTING__" /D "_UNICODE" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /fo"Release/ACGL.res" /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 MPlusUICommon.lib MPlusComm.lib comsupp.lib MPlusUICommon.lib MPlusComm.lib comsupp.lib MPlusOpLog.lib MPlusErrorDisplay.lib MplusErrorHandler.lib MplusEventLog.lib MPlusTrace.lib Psapi.lib MRErrorLogLibrary.lib MRTraceLogLibrary.lib PDataParserU.lib PDataTypesU.lib shlwapi.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /libpath:"$(MRMP_HOME)\Lib\release" /libpath:"$(MPLUSHOME)/lib/release" /libpath:"$(MPLUSSHARE)/lib/release" /libpath:"$(MPLUSSHARE)\Lib\Release" /IGNORE:4786
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy Release\AdvCardiacGL.exe $(MPLUSSHARE)\Bin\Release\AdvCardiacGL.exe
# End Special Build Tool

!ELSEIF  "$(CFG)" == "AdvCardiacGL - Win32 Debug"

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
# ADD CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /I "$(MPLUSHOME)/include" /I "$(MRMP_HOME)/Include" /I "$(MPLUSSHARE)/include" /I "$(MPLUSHOME)/etc/ICF/include/common/src" /I "$(MPLUSHOME)/etc/ICF/include/common/idl/Release" /I "$(MPLUSHOME)/etc/ICF/include/icf_base/src" /I "$(MPLUSHOME)/include/CosVis" /I "$(MPLUSHOME)/include/cosvis/develop/link" /I "$(MPLUSHOME)/etc/ICF/include/icf_render/src" /I "$(MPLUSSHARE)/include/FEX" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_UNICODE" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 MPlusUICommon.lib MPlusComm.lib comsupp.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"$(MRMP_HOME)\Lib\release" /libpath:"$(MPLUSHOME)/lib/release" /libpath:"$(MPLUSSHARE)/lib/release"

!ENDIF 

# Begin Target

# Name "AdvCardiacGL - Win32 Release"
# Name "AdvCardiacGL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ACGAppController.cpp
# End Source File
# Begin Source File

SOURCE=.\ACGLConfigReader.cpp
# End Source File
# Begin Source File

SOURCE=.\ACGLDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ACGLLogHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\ACGLTrace.cpp
# End Source File
# Begin Source File

SOURCE=.\AdvCardiacGL.cpp
# End Source File
# Begin Source File

SOURCE=.\AdvCardiacGL.rc
# End Source File
# Begin Source File

SOURCE=.\AdvCardiacGLDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\AdvCardiacGLView.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfirmationDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DPSComManager.cpp
# End Source File
# Begin Source File

SOURCE=.\IpFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
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

SOURCE=.\ACGAppController.h
# End Source File
# Begin Source File

SOURCE=.\ACGLConfigReader.h
# End Source File
# Begin Source File

SOURCE=.\ACGLDlg.h
# End Source File
# Begin Source File

SOURCE=.\ACGLLogHelper.h
# End Source File
# Begin Source File

SOURCE=.\ACGLTrace.h
# End Source File
# Begin Source File

SOURCE=.\AdvCardiacGL.h
# End Source File
# Begin Source File

SOURCE=.\AdvCardiacGLDoc.h
# End Source File
# Begin Source File

SOURCE=.\AdvCardiacGLView.h
# End Source File
# Begin Source File

SOURCE=.\CommonDefinitions.h
# End Source File
# Begin Source File

SOURCE=.\ConfirmationDlg.h
# End Source File
# Begin Source File

SOURCE=.\DPSComManager.h
# End Source File
# Begin Source File

SOURCE=.\IpFrame.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
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

SOURCE=.\res\AdvCardiacGL.ico
# End Source File
# Begin Source File

SOURCE=.\res\AdvCardiacGL.rc2
# End Source File
# Begin Source File

SOURCE=.\res\AdvCardiacGLDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\l_advcardiacgl.ico
# End Source File
# Begin Source File

SOURCE=.\res\l_tiprep.ico
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\AdvCardiacGL.reg
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

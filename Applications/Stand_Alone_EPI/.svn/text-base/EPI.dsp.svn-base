# Microsoft Developer Studio Project File - Name="EPI" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=EPI - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "EPI.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "EPI.mak" CFG="EPI - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "EPI - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "EPI - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "EPI - Win32 Release"

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
# ADD CPP /nologo /MD /W4 /WX /GX /O2 /I "./include" /I "./include/tami" /I "./include/tami/vfstudy" /I "./include/tami/common" /I "./bin" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "V35M" /D "__WIN_NATIVE_PORTING__" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 vf.lib OPENGL32.lib GLU32.lib shlwapi.lib libvfToUi.lib libTl.lib libgpdb.lib libmathServer.lib libgdbm.lib Ws2_32.lib Psapi.Lib gdbm.lib libWNP.lib wsock32.lib libdll.lib libwellks.lib libIRS.lib libimgsrv.lib /nologo /subsystem:windows /machine:I386 /libpath:".\lib" /IGNORE:4089
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy      Release\EPI.exe      c:\gp\Bin\ 
# End Special Build Tool

!ELSEIF  "$(CFG)" == "EPI - Win32 Debug"

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
# ADD CPP /nologo /MDd /W4 /WX /Gm /GX /ZI /Od /I "./include" /I "./include/tami" /I "./include/tami/vfstudy" /I "./include/tami/common" /I "./bin" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "V35M" /D "__WIN_NATIVE_PORTING__" /FR /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT CPP /X
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 vf.lib OPENGL32.lib GLU32.lib shlwapi.lib libvfToUi.lib libTl.lib libgpdb.lib libmathServer.lib libgdbm.lib Ws2_32.lib Psapi.Lib gdbm.lib libWNP.lib wsock32.lib libdll.lib libwellks.lib libIRS.lib libimgsrv.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"MSVCRT.lib" /pdbtype:sept /libpath:".\lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy      Debug\EPI.exe      c:\gp\Bin\ 
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "EPI - Win32 Release"
# Name "EPI - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Application_cpp"

# PROP Default_Filter ".cpp"
# Begin Source File

SOURCE=.\ComboListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\EPI.cpp
# End Source File
# Begin Source File

SOURCE=.\EPIController.cpp
# End Source File
# Begin Source File

SOURCE=.\EPIDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\EPISpinCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\EPIView.cpp
# End Source File
# Begin Source File

SOURCE=.\FormImageSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\FormImageViewer.cpp
# End Source File
# Begin Source File

SOURCE=.\FormStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\InPlaceCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\InPlaceEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\OpenRawDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PageBase.cpp
# End Source File
# Begin Source File

SOURCE=.\PageEPI.cpp
# End Source File
# Begin Source File

SOURCE=.\PageFSE.cpp
# End Source File
# Begin Source File

SOURCE=.\SheetOptions.cpp
# End Source File
# Begin Source File

SOURCE=.\SlideToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "VFC_cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ScrPadHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\VFChandler.cpp
# End Source File
# Begin Source File

SOURCE=.\VFCReader.cpp
# End Source File
# Begin Source File

SOURCE=.\VFCWriter.cpp
# End Source File
# End Group
# Begin Group "OPENGL_cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\OpenGLControl.cpp
# End Source File
# Begin Source File

SOURCE=.\OpenGLDevice.cpp
# End Source File
# End Group
# Begin Group "Process_cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DataSeparator.cpp
# End Source File
# Begin Source File

SOURCE=.\EvenOddSeparator.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageDataProcessor.cpp
# End Source File
# Begin Source File

SOURCE=.\kspaceSeparator.cpp
# End Source File
# Begin Source File

SOURCE=.\ReconProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\ReconRawCheck.cpp
# End Source File
# End Group
# Begin Group "IRS_cpp"

# PROP Default_Filter ".cpp"
# Begin Source File

SOURCE=.\IRSListner.cpp
# End Source File
# End Group
# Begin Group "FileHandler_cpp"

# PROP Default_Filter ".cpp"
# Begin Source File

SOURCE=.\FileReader.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageReader.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageWriter.cpp
# End Source File
# Begin Source File

SOURCE=.\ImgFileHandler.cpp
# End Source File
# End Group
# Begin Group "LOG_cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\EPILogHelper.cpp
# End Source File
# End Group
# Begin Group "RC_cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\EPI.rc
# End Source File
# End Group
# Begin Group "PhaseCorrection_cpp"

# PROP Default_Filter ".cpp"
# Begin Source File

SOURCE=.\PhaseCorrectionStrategy.cpp
# End Source File
# Begin Source File

SOURCE=.\ShiftPickStrategy.cpp
# End Source File
# Begin Source File

SOURCE=.\TimeShift.cpp
# End Source File
# Begin Source File

SOURCE=.\TimeShiftLineFit.cpp
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Application_h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ComboListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\EPI.h
# End Source File
# Begin Source File

SOURCE=.\EPIController.h
# End Source File
# Begin Source File

SOURCE=.\EPIDoc.h
# End Source File
# Begin Source File

SOURCE=.\EPISpinCtrl.h
# End Source File
# Begin Source File

SOURCE=.\EPIView.h
# End Source File
# Begin Source File

SOURCE=.\FormImageSelect.h
# End Source File
# Begin Source File

SOURCE=.\FormImageViewer.h
# End Source File
# Begin Source File

SOURCE=.\FormStatus.h
# End Source File
# Begin Source File

SOURCE=.\ImageListDlg.h
# End Source File
# Begin Source File

SOURCE=.\InPlaceCombo.h
# End Source File
# Begin Source File

SOURCE=.\InPlaceEdit.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\PageBase.h
# End Source File
# Begin Source File

SOURCE=.\PageEPI.h
# End Source File
# Begin Source File

SOURCE=.\PageFSE.h
# End Source File
# Begin Source File

SOURCE=.\SheetOptions.h
# End Source File
# Begin Source File

SOURCE=.\SlideToolBar.h
# End Source File
# Begin Source File

SOURCE=.\OpenRawDlg.h
# End Source File
# Begin Source File


SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "VFC_h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ScrPadHandler.h
# End Source File
# Begin Source File

SOURCE=.\VFChandler.h
# End Source File
# Begin Source File

SOURCE=.\VFCReader.h
# End Source File
# Begin Source File

SOURCE=.\VFCWriter.h
# End Source File
# End Group
# Begin Group "OPENGL_h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\OpenGLControl.h
# End Source File
# Begin Source File

SOURCE=.\OpenGLDevice.h
# End Source File
# End Group
# Begin Group "Process_h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DataSeparator.h
# End Source File
# Begin Source File

SOURCE=.\EvenOddSeparator.h
# End Source File
# Begin Source File

SOURCE=.\ImageDataProcessor.h
# End Source File
# Begin Source File

SOURCE=.\kspaceSeparator.h
# End Source File
# Begin Source File

SOURCE=.\ReconProcess.h
# End Source File
# Begin Source File

SOURCE=.\ReconRawCheck.h
# End Source File
# End Group
# Begin Group "IRS_h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\IRSListner.h
# End Source File
# End Group
# Begin Group "FileHandler_h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FileReader.h
# End Source File
# Begin Source File

SOURCE=.\ImageReader.h
# End Source File
# Begin Source File

SOURCE=.\ImageWriter.h
# End Source File
# Begin Source File

SOURCE=.\ImgFileHandler.h
# End Source File
# End Group
# Begin Group "LOG_h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\EPIErrors.h
# End Source File
# Begin Source File

SOURCE=.\EPILogHelper.h
# End Source File
# End Group
# Begin Group "Common_h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CommonDefinations.h
# End Source File
# End Group
# Begin Group "RC_h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# End Group
# Begin Group "PhaseCorrection_h"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=.\PhaseCorrectionStrategy.h
# End Source File
# Begin Source File

SOURCE=.\ShiftPickStrategy.h
# End Source File
# Begin Source File

SOURCE=.\TimeShift.h
# End Source File
# Begin Source File

SOURCE=.\TimeShiftLineFit.h
# End Source File
# End Group
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\EPI.ico
# End Source File
# Begin Source File

SOURCE=.\res\EPI.rc2
# End Source File
# Begin Source File

SOURCE=.\res\EPIDoc.ico
# End Source File
# Begin Source File

SOURCE=.\sdi.ico
# End Source File
# Begin Source File

SOURCE=.\res\slider_b.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

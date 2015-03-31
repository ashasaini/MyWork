# Microsoft Developer Studio Project File - Name="PlanDuplicator" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=PlanDuplicator - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PlanDuplicator.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PlanDuplicator.mak" CFG="PlanDuplicator - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PlanDuplicator - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "PlanDuplicator - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PlanDuplicator - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /WX /GR /GX /ZI /I "$(MPLUSHOME)\Include" /I "$(MRMP_HOME)\Include" /I "$(MPLUSSHARE)/Include/" /I "../RelimitMgr/" /I "../ScrPadMgr/" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "__WIN_NATIVE_PORTING__" /D "__NUTC__" /D "__WNP_MPLUS__" /D "__WNP_SOCKET__" /D "TCPCONN" /FR /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 VF.lib libdll.lib libTl.lib LibVfToUI.lib Ws2_32.lib libproMsg.lib libpl.lib libWNP.lib psapi.lib libSar.lib libgpdb.lib liblicense2.lib libinit.lib libseq.lib gdbm.lib libmathServer.lib libUIError.lib libAcq.lib libdbg.lib MPlusUICommon.lib libSpeeder.lib MPlusComm.lib comsupp.lib libpltree.lib DBSAVR.lib DBSALib.lib MPlusDBServerIFClasses.lib shlwapi.lib MRErrorLogLibrary.lib MRTraceLogLibrary.lib PDataParserU.lib PDataTypesU.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /incremental:yes /debug /machine:I386 /libpath:"$(MPLUSHOME)\lib\Release" /libpath:"$(MRMP_HOME)\Lib\Release"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy release\PlanDuplicator.exe $(MRMP_Home)\Bin\release\PlanDuplicator.exe
# End Special Build Tool

!ELSEIF  "$(CFG)" == "PlanDuplicator - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GR /GX /ZI /I "$(MPLUSHOME)\Include" /I "$(MRMP_HOME)\Include" /I "$(MPLUSSHARE)/Include/" /I "../RelimitMgr/" /I "../ScrPadMgr/" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "__WIN_NATIVE_PORTING__" /D "__NUTC__" /D "__WNP_MPLUS__" /D "__WNP_SOCKET__" /D "TCPCONN" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 VF.lib libdll.lib libTl.lib LibVfToUI.lib Ws2_32.lib libproMsg.lib libpl.lib libWNP.lib psapi.lib libSar.lib libgpdb.lib liblicense2.lib libinit.lib libseq.lib gdbm.lib libmathServer.lib libUIError.lib libAcq.lib libdbg.lib libpltreepvt.lib MPlusUICommond.lib libSpeeder.lib MPlusComm_d.lib comsupp.lib DBSAVRd.lib DBSALibd.lib MPlusDBServerIFClassesd.lib shlwapi.lib MRErrorLogLibraryd.lib MRTraceLogLibraryd.lib PDataParserUd.lib PDataTypesUd.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"$(MPLUSHOME)\lib\debug" /libpath:"$(MRMP_HOME)\Lib\debug"
# SUBTRACT LINK32 /nodefaultlib
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy debug\PlanDuplicator.exe $(MRMP_Home)\Bin\debug\PlanDuplicator.exe
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "PlanDuplicator - Win32 Release"
# Name "PlanDuplicator - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DBMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\DBWrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\DcmController.cpp
# End Source File
# Begin Source File

SOURCE=.\DPSComManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\IniFileReader.cpp
# End Source File
# Begin Source File

SOURCE=.\LocalDBMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\PDCalculation.cpp
# End Source File
# Begin Source File

SOURCE=.\PDController.cpp
# End Source File
# Begin Source File

SOURCE=.\PDCoordinates.cpp
# End Source File
# Begin Source File

SOURCE=.\PDDataFileReader.cpp
# End Source File
# Begin Source File

SOURCE=.\PDDataHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\PDDicomConverter.cpp
# End Source File
# Begin Source File

SOURCE=.\PDDicomFileWriter.cpp
# End Source File
# Begin Source File

SOURCE=.\PDErrorMsgDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PDLogHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\PDProcessExec.cpp
# End Source File
# Begin Source File

SOURCE=.\PDTrace.cpp
# End Source File
# Begin Source File

SOURCE=.\PlanDuplicator.cpp
# End Source File
# Begin Source File

SOURCE=.\PlanDuplicatorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RelimitLogHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\SmartMPRData.cpp
# End Source File
# Begin Source File

SOURCE=.\SmartVOIData.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\vfcDBMgr.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Common.h
# End Source File
# Begin Source File

SOURCE=.\DBMgr.h
# End Source File
# Begin Source File

SOURCE=.\DBWrapper.h
# End Source File
# Begin Source File

SOURCE=.\DcmController.h
# End Source File
# Begin Source File

SOURCE=.\DPSComManager.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\tami\common\errors.h
# End Source File
# Begin Source File

SOURCE=.\groupdefs.h
# End Source File
# Begin Source File

SOURCE=.\ImageInfo.h
# End Source File
# Begin Source File

SOURCE=.\IniFileReader.h
# End Source File
# Begin Source File

SOURCE=.\LocalDBMgr.h
# End Source File
# Begin Source File

SOURCE=.\PDCalculation.h
# End Source File
# Begin Source File

SOURCE=.\PDController.h
# End Source File
# Begin Source File

SOURCE=.\PDCoordinates.h
# End Source File
# Begin Source File

SOURCE=.\PDDataFileReader.h
# End Source File
# Begin Source File

SOURCE=.\PDDataHandler.h
# End Source File
# Begin Source File

SOURCE=.\PDDicomConverter.h
# End Source File
# Begin Source File

SOURCE=.\PDDicomFileWriter.h
# End Source File
# Begin Source File

SOURCE=.\PDErrorMsgDlg.h
# End Source File
# Begin Source File

SOURCE=.\PDLogHelper.h
# End Source File
# Begin Source File

SOURCE=.\PDProcessExec.h
# End Source File
# Begin Source File

SOURCE=.\PDTrace.h
# End Source File
# Begin Source File

SOURCE=.\PlanDuplicator.h
# End Source File
# Begin Source File

SOURCE=.\PlanDuplicatorDlg.h
# End Source File
# Begin Source File

SOURCE=.\plNameTableP.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\..\MRMPlus\include\tami\libplTree\plsftree.h
# End Source File
# Begin Source File

SOURCE=.\RelimitLogHelper.h
# End Source File
# Begin Source File

SOURCE=.\SmartMPRData.h
# End Source File
# Begin Source File

SOURCE=.\SmartVOIData.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\vfcDBMgr.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\file_typ.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PlanDuplicator.ico
# End Source File
# Begin Source File

SOURCE=.\PlanDuplicator.rc
# End Source File
# Begin Source File

SOURCE=.\res\PlanDuplicator.rc2
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# End Group
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\dataTypes.h
# End Source File
# End Group
# Begin Group "Relimit"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\relimFuncs.c
# End Source File
# Begin Source File

SOURCE=.\relimit.c
# End Source File
# Begin Source File

SOURCE=.\relimitMsg.h
# End Source File
# Begin Source File

SOURCE=.\relimitParams.h
# End Source File
# Begin Source File

SOURCE=.\relimitProcess.c
# End Source File
# Begin Source File

SOURCE=.\relimitSpawn.c
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

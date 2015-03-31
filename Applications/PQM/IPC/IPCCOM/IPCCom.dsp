# Microsoft Developer Studio Project File - Name="IPCCom" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=IPCCom - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "IPCCom.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "IPCCom.mak" CFG="IPCCom - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IPCCom - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "IPCCom - Win32 Unicode Release MinDependency" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/MRI/13690/MRMPLUS/Sources/Acquire/PQM/IPC/IPCCOM", JCOEAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "IPCCom - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\IPCLIB" /I "$(MPLUSHOME)\include" /I "$(MRMP_HOME)\Include" /I "$(MRMP_HOME)\Sources\Acquire" /D "__INTTYPES_H__" /D "_MSXMLPARSER" /D "_DEBUG" /D "_UNICODE" /D "_ATL_STATIC_REGISTRY" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "__NUTC__" /D "__WIN_NATIVE_PORTING__" /D "__WNP_SIGNAL__" /D "MRI" /D "IRIX65" /D "__WNP_PATNI__" /D "PQM_MODE" /D "_SDE_WIN32_" /D "_AFXEXT" /D "__WNP_MPLUS__" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# SUBTRACT RSC /x
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 libSMMutexd.lib libSMLogd.lib PDataTypesUd.lib PDataParserUd.lib Pcommlibd.lib PASCOMd.lib DBSAReadTempd.lib DBSAAccessLogd.lib DBSAVRd.lib DBSALibd.lib MRTraceLogLibraryd.lib MRErrorLogLibraryd.lib MROpeLogLibraryd.lib PQMIPCd.lib libSAR.lib DBSAVRMakeDBd.lib MPlusErrorDisplayd.lib MPlusI18nCatalogd.lib Mpluscomm_d.lib libRemoted.lib MPlusDBServerIFClassesd.lib MPlusDBServerVRLibd.lib MplusAuditClientd.lib MplusUserManageClientd.lib wsock32.lib advapi32.lib winmm.lib Ws2_32.lib VF.lib libIrs.lib libdll.lib libwellks.lib libTl.lib LibVfToUI.lib libproMsg.lib libpl.lib shlwapi.lib pdet.lib libmsintf.lib libmsipc.lib libWNP.lib psapi.lib libgpdb.lib liblicense2.lib libinit.lib libseq.lib gdbm.lib libms.lib libmsVf.lib libmsVf1.lib libmsVf2.lib libmsVf3.lib libmathServer.lib libUIError.lib probe.lib libAcq.lib libdbdt.lib libsg.lib libgt.lib libSpeeder.lib librbvap.lib libVAPcom.lib libgv.lib libdbg.lib libplTree.lib librb.lib VoiceMgrd.lib PErrorHandler.lib dbghelp.lib libMRSUtil.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"msvcrt.lib" /nodefaultlib:"LIBCMTd.lib" /nodefaultlib:"uafxcwd.lib" /out:"Debug/IPCComd.dll" /pdbtype:sept /libpath:"$(MPLUSHOME)\lib\Debug" /libpath:"$(MRMP_HOME)\Lib\Debug"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=move Debug\IPCComd.dll $(MRMP_HOME)\Bin\Debug	move Debug\IPCComd.lib $(MRMP_HOME)\Lib\Debug	move IPCCom.h $(MRMP_HOME)\Include\PQM	move IPCCom.tlb $(MRMP_HOME)\Include\PQM
# End Special Build Tool

!ELSEIF  "$(CFG)" == "IPCCom - Win32 Unicode Release MinDependency"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinDependency"
# PROP BASE Intermediate_Dir "ReleaseUMinDependency"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseUMinDependency"
# PROP Intermediate_Dir "ReleaseUMinDependency"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_UNICODE" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /Od /I "..\IPCLIB" /I "$(MRMP_HOME)\Include" /I "$(MPLUSHOME)\Include" /I "$(MRMP_HOME)\Sources\Acquire" /D "__INTTYPES_H__" /D "_MSXMLPARSER" /D "NDEBUG" /D "_UNICODE" /D "_ATL_STATIC_REGISTRY" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "__NUTC__" /D "__WIN_NATIVE_PORTING__" /D "__WNP_SIGNAL__" /D "MRI" /D "IRIX65" /D "__WNP_PATNI__" /D "PQM_MODE" /D "_SDE_WIN32_" /D "_AFXEXT" /D "__WNP_MPLUS__" /Fr /FD /EHa /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 libSMMutex.lib libSMLog.lib PDataTypesU.lib PDataParserU.lib Pcommlib.lib PASCOM.lib MPlusConfig.lib MPlusErrorHandler.lib MplusEventLog.lib MPlusErrorDisplay.lib MPlusDBServerIFClasses.lib MPlusDBServerVRLib.lib DBSAReadTemp.lib DBSAVR.lib DBSALib.lib DBSAAccessLog.lib MRTraceLogLibrary.lib MRErrorLogLibrary.lib MROpeLogLibrary.lib PQMIPC.lib libSar.lib DBSAVRMakeDB.lib MPlusI18nCatalog.lib Mpluscomm.lib libRemote.lib MplusAuditClient.lib MplusUserManageClient.lib wsock32.lib advapi32.lib winmm.lib Ws2_32.lib VF.lib libIrs.lib libdll.lib libwellks.lib libTl.lib LibVfToUI.lib libproMsg.lib libpl.lib shlwapi.lib pdet.lib libmsintf.lib libmsipc.lib libWNP.lib psapi.lib libgpdb.lib liblicense2.lib libinit.lib libseq.lib gdbm.lib libms.lib libmsVf.lib libmsVf1.lib libmsVf2.lib libmsVf3.lib libmathServer.lib libUIError.lib probe.lib libAcq.lib libdbdt.lib libsg.lib libgt.lib libSpeeder.lib librbvap.lib libVAPcom.lib libgv.lib libdbg.lib libplTree.lib librb.lib VoiceMgr.lib PErrorHandler.lib dbghelp.lib libMRSUtil.lib /nologo /base:"0x66000000" /subsystem:windows /dll /incremental:yes /map /debug /machine:I386 /nodefaultlib:"LIBCMT.lib" /libpath:"$(MPLUSHOME)\lib\Release" /libpath:"$(MRMP_HOME)\Lib\Release" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /profile /nodefaultlib
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=move ReleaseUMinDependency\IPCCom.dll $(MRMP_HOME)\Bin\Release	move ReleaseUMinDependency\IPCCom.lib $(MRMP_HOME)\Lib\Release	move IPCCom.h $(MRMP_HOME)\Include\PQM	move IPCCom.tlb $(MRMP_HOME)\Include\PQM	move ReleaseUMinDependency\IPCCom.map $(MRMP_HOME)\Lib\Release
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "IPCCom - Win32 Debug"
# Name "IPCCom - Win32 Unicode Release MinDependency"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\IPCCom.cpp
# End Source File
# Begin Source File

SOURCE=.\IPCCom.def
# End Source File
# Begin Source File

SOURCE=.\IPCCom.idl
# ADD MTL /tlb ".\IPCCom.tlb" /h "IPCCom.h" /iid "IPCCom_i.c" /Oicf
# End Source File
# Begin Source File

SOURCE=.\IPCCom.rc
# End Source File
# Begin Source File

SOURCE=.\PqmIpcCom.cpp
# End Source File
# Begin Source File

SOURCE=..\IPCLIB\pqmstudy.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\IPCCom.h
# End Source File
# Begin Source File

SOURCE=.\IPCComCP.h
# End Source File
# Begin Source File

SOURCE=.\PqmIpcCom.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\PqmIpcCom.rgs
# End Source File
# End Group
# End Target
# End Project

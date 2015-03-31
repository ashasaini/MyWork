
@echo off
goto START

rem * Copyright(c) 2008 TOSHIBA Medical Systems Corporation,
rem * All Rights Reserved.
rem *
rem * @author :	PATNI
rem * @version:	0.0	Date: 2008-12-01

:START

setlocal

:BUILD

rem ----------------------------------------------------------------------------
rem +Patni-PH/2011Mar15/Modified/REDMINE-1423

set mplus.build.name="IPCCom"

echo [Building %mplus.build.name%]


:BUILD_ALL
set mplus.build.target=
goto BUILD_MSDEV


:BUILD_MSDEV

if "%1"=="release" GOTO RELEASE
if "%1"=="debug" GOTO DEBUG
if "%1"=="all" GOTO ALL
if "%1"=="clean" GOTO BUILD_CLEAN
if "%1"=="" GOTO RELEASE
if not "%1"=="" GOTO RELEASE

:RELEASE
msdev %mplus.build.name%.dsp /MAKE "%mplus.build.name% - Win32 Unicode Release MinDependency" /BUILD
GOTO NEXT

:DEBUG
msdev %mplus.build.name%.dsp /MAKE "%mplus.build.name% - Win32 debug" /BUILD
GOTO NEXT

:BUILD_CLEAN
set mplus.build.target=/CLEAN

:ALL
msdev %mplus.build.name%.dsp /MAKE ALL %mplus.build.target%
GOTO NEXT

:Next
if ERRORLEVEL 1 (
echo %mplus.build.name% %mplus.build.target% BUILD_ERROR
) else (
echo %mplus.build.name% %mplus.build.target% BUILD_SUCCESS
)

rem %mplus.build.failonerror% if ERRORLEVEL 1 goto BUILD_ERROR

rem -Patni-PH/2011Mar15/Modified/REDMINE-1423
rem ----------------------------------------------------------------------------
goto BUILD_END

:BUILD_ERROR
exit /B 1

:BUILD_END

ver > nul
endlocal

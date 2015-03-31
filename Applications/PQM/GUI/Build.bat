
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

set mplus.build.name="PQM"

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
msdev %mplus.build.name%.dsp /MAKE "%mplus.build.name% - Win32 release" /BUILD

rem //Redmine-1423
if ERRORLEVEL 1 (
echo %mplus.build.name% - Win32 release BUILD_ERROR
) else (
echo %mplus.build.name% - Win32 release BUILD_SUCCESS
) 

GOTO BUILD_RES_RELEASE

:DEBUG
msdev %mplus.build.name%.dsp /MAKE "%mplus.build.name% - Win32 debug" /BUILD

rem //Redmine-1423
if ERRORLEVEL 1 (
echo %mplus.build.name% - Win32 debug BUILD_ERROR
) else (
echo %mplus.build.name% - Win32 debug BUILD_SUCCESS
) 

GOTO BUILD_RES_DEBUG

:BUILD_CLEAN
set mplus.build.target=/CLEAN

:ALL
msdev %mplus.build.name%.dsp /MAKE ALL %mplus.build.target%

if ERRORLEVEL 1 (
echo %mplus.build.name% %mplus.build.target% BUILD_ERROR
) else (
echo %mplus.build.name% %mplus.build.target% BUILD_SUCCESS
)

if "%1"=="clean" goto BUILD_RES_CLEAN
goto BUILD_RES

:BUILD_RES
:BUILD_RES_DEBUG
rem ----------------------------------------------------------------------------
rem Build Resource DLLs (Debug)

pushd %MPLUSHOME%\devutils\debug\MPlusI18n

rem +Patni-MSN/2009Mar20/Modified/PSP1 Modified the I18N file paths as per TMSC request

CALL MPlusI18nCreateRcs.bat  %MRMP_HOME%\Sources\Acquire\PQM\GUI PQM 

@echo off
rem //Redmine-1423
if ERRORLEVEL 1 (
echo [BUILD_ERROR] ***MPlusI18nCreateRcs.bat failed***
) else (
echo MPlusI18nCreateRcs.bat BUILD_SUCCESS
) 

CALL MPlusI18nXls2Xml.bat %MRMP_HOME%\Doc\i18nData PQMGUI_cat

@echo off
rem //Redmine-1423
if ERRORLEVEL 1 (
echo [BUILD_ERROR] ***MPlusI18nXls2Xml.bat failed***
) else (
echo MPlusI18nXls2Xml.bat BUILD_SUCCESS
move /Y %MRMP_HOME%\Doc\i18nData\PQMGUI_cat.xml %MRMP_HOME%\Doc\i18nData\xml
) 

CALL MPlusI18nTranslateRcs.bat %MRMP_HOME%\Sources\Acquire\PQM\GUI PQM %MRMP_HOME%\Doc\i18nData\xml\PQMGUI_cat.xml

@echo off
rem //Redmine-1423
if ERRORLEVEL 1 (
echo [BUILD_ERROR] ***MPlusI18nTranslateRcs.bat failed***
) else (
echo MPlusI18nTranslateRcs.bat BUILD_SUCCESS
) 

CALL MPlusI18nCreateAllResDlls.bat PQM %MRMP_HOME%\Sources\Acquire\PQM\GUI %MRMP_HOME%\bin\Debug d

@echo off
rem //Redmine-1423
if ERRORLEVEL 1 (
echo [BUILD_ERROR] ***MPlusI18nCreateAllResDlls.bat failed***
) else (
echo MPlusI18nCreateAllResDlls.bat BUILD_SUCCESS
) 

CALL MPlusI18nCreateCatsd.bat PQMGUI_cat %MRMP_HOME%\Doc\i18nData\xml %MRMP_HOME%\bin\Debug

@echo off
rem //Redmine-1423
if ERRORLEVEL 1 (
echo [BUILD_ERROR] ***MPlusI18nCreateCatsd.bat failed***
) else (
echo MPlusI18nCreateCatsd.bat BUILD_SUCCESS
) 

popd

if not "%1" == "all" GOTO NEXT
:BUILD_RES_RELEASE
rem ----------------------------------------------------------------------------
rem Build Resource DLLs (Release)

pushd %MPLUSHOME%\devutils\Release\MPlusI18n

CALL MPlusI18nCreateRcs.bat  %MRMP_HOME%\Sources\Acquire\PQM\GUI PQM 

@echo off
rem //Redmine-1423
if ERRORLEVEL 1 (
echo [BUILD_ERROR] ***MPlusI18nCreateRcs.bat failed***
) else (
echo MPlusI18nCreateRcs.bat BUILD_SUCCESS
) 

CALL MPlusI18nXls2Xml.bat %MRMP_HOME%\Doc\i18nData PQMGUI_cat

@echo off
rem //Redmine-1423
if ERRORLEVEL 1 (
echo [BUILD_ERROR] ***MPlusI18nXls2Xml.bat failed***
) else (
echo MPlusI18nXls2Xml.bat BUILD_SUCCESS
move /Y %MRMP_HOME%\Doc\i18nData\PQMGUI_cat.xml %MRMP_HOME%\Doc\i18nData\xml
) 

CALL MPlusI18nTranslateRcs.bat %MRMP_HOME%\Sources\Acquire\PQM\GUI PQM %MRMP_HOME%\Doc\i18nData\xml\PQMGUI_cat.xml

@echo off
rem //Redmine-1423
if ERRORLEVEL 1 (
echo [BUILD_ERROR] ***MPlusI18nTranslateRcs.bat failed***
) else (
echo MPlusI18nTranslateRcs.bat BUILD_SUCCESS
) 

CALL MPlusI18nCreateAllResDlls.bat PQM %MRMP_HOME%\Sources\Acquire\PQM\GUI %MRMP_HOME%\bin\Release

@echo off
rem //Redmine-1423
if ERRORLEVEL 1 (
echo [BUILD_ERROR] ***MPlusI18nCreateAllResDlls.bat failed***
) else (
echo MPlusI18nCreateAllResDlls.bat BUILD_SUCCESS
) 

CALL MPlusI18nCreateCats.bat PQMGUI_cat %MRMP_HOME%\Doc\i18nData\xml %MRMP_HOME%\bin\Release

@echo off
rem //Redmine-1423
if ERRORLEVEL 1 (
echo [BUILD_ERROR] ***MPlusI18nCreateCats.bat failed***
) else (
echo MPlusI18nCreateCats.bat BUILD_SUCCESS
) 

rem -Patni-MSN/2009Mar20/Modified/PSP1 Modified the I18N file paths as per TMSC request

popd

:Next

rem %mplus.build.failonerror% if ERRORLEVEL 1 goto BUILD_ERROR

rem -Patni-PH/2011Mar15/Modified/REDMINE-1423
rem ----------------------------------------------------------------------------
goto BUILD_END

:BUILD_ERROR
exit /B 1

:BUILD_RES_CLEAN

:BUILD_END

ver > nul
endlocal

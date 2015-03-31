//Redmine-777
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: StdAfx.h
 *  Overview: source file that includes just the standard includes.
 *  Last Modified: 2007/8/10 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2007/6/15 12:00:00     PQM5 Baselined code
 *   2.0          Patni            2007/8/10 12:00:00     Implemented PQM6 requirements & Formatted
 *
 *****************************************************************************/
// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
#ifndef __SM_STD_AFX_H__
#define __SM_STD_AFX_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//Commented by Hemant On 3/5/2009 2:52:35 PM
//#pragma warning (disable:4005)
//#pragma warning (disable:4786)


#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxole.h>         // MFC OLE classes
#include <afxdisp.h>        // MFC Automation classes
#include <afxdocob.h>
#include <afxdtctl.h>       // MFC support for Internet Explorer 4 Common Controls

#include <comdef.h>
#include <iostream>
#include <winsock2.h>

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>         // MFC support for Windows Common Controls
#include <vector>
#endif // _AFX_NO_AFXCMN_SUPPORT
//////////////////////////////

#include <MPlusEh/ErrorItem.h>
#include <MPlusEh/ErrorLevel.h>
#include <MPlusEh/ErrorStatus.h>
#include <ErrorDisp/ErrorDisplay.h>
#include <ErrorDisp/ErrorDispConsts.h>
#include <MPlusEvLog/EventLogEntry.h>
#include <MPlusEvLog/Logger.h>
#include <MPlusEvLog/LogType.h>
#include <cfw/cfw.h>
#include <tami/libWNP/WinNativePorting.h>
#include <tami/common/errors.h>
#include <tami/libSAR/sar.h> // Patni-PJS/2009Jul23/Modify/IDS#442,MVC002699

//TMSC-REDMINE-774
#include "PqmGuiConstants.h"
//#include "PQMTraceInfo.h"
/////////////////////////////


typedef struct  tagINFO {
    TCHAR csPageName[1024];
} GLInfo;


/////////////////////////////

#define SCAN_BUTTON_EVENT               WM_USER+1
#define DQA_BUTTON_EVENT                WM_USER+2

//+Patni-SS/2009Apr07/Modi/PSP1#12
#define ON_DUPLICATE		WM_USER +10
#define ON_DELETE			WM_USER +11
#define ON_DROPITEM			WM_USER +12
#define ON_COILAPPLY		WM_USER +13
#define ON_SETALL			WM_USER +14
#define ON_ANATOMYAPPLY		WM_USER +15
//+Patni-SS/2009Nov16/Added/IR#90/SFT Menu
#define ON_SFT_MENU			WM_USER +16
#define ON_VFC_MENU			WM_USER +17
#define ON_SFT_MENU_COMMAND	WM_USER +18   //Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu

//Patni-KSS/2010Jul16/ADDED/IR-156_2
#define ON_DUPLICATE_APPEND WM_USER +19

//+Patni-Sribanta/2011Jan11/Added/TMSC-REDMINE-1026
#define ON_VOICE_MENUITEM_CLICK WM_USER + 20
#define ON_SCAN_START			WM_USER + 21
#define ON_SCAN_RESUME			WM_USER + 22

//-Patni-Sribanta/2011Jan11/Added/TMSC-REDMINE-1026

//-Patni-SS/2009Nov16/Added/IR#90/SFT Menu
//-Patni-SS/2009Apr07/Modi/PSP1#12

//+Patni-Hemant/2010Mar26/Added/ACE-Phase#3/CDS Requirement
#define ON_CDS_DEBUG_CLOSE		WM_USER +23
//-Patni-Hemant/2010Mar26/Added/ACE-Phase#3/CDS Requirement

#define ON_SCAN_EDIT_OR_PLAN	WM_USER + 24
#define ON_SCAN_PAUSE			WM_USER + 25
#define ON_UPDATE_SARINFO       WM_USER + 26
#define ON_CFA_OPERATION        WM_USER + 27
#define ON_IRS_SCAN_MSG			WM_USER + 28
#define ON_INVOKE_PROBE_DLG     WM_USER + 29
#define ON_END_EXAM             WM_USER + 30
#define ON_AMB_DUPLCIATE        WM_USER + 31




#define WM_CMD_OPEN                     (WM_USER + 308)
#define WM_CMD_CLOSE                    (WM_USER + 309)
#define WM_CMD_SHOW                     (WM_USER + 310)
#define WM_CMD_HIDE                     (WM_USER + 311)
#define WM_CMD_SCAN_START				(WM_USER + 312)
#define WM_CMD_CFA_SHOW					(WM_USER + 313)
#define WM_CMD_SHOW_CFA_DIALOG			(WM_USER + 314)  //Patni-MN/2009Mar13/Added/PSP1#203 
#define WM_RECONNECT_TO_PROCON			(WM_USER + 315)  //+Patni-HEMANT/ADDED On 3/31/2009 3:08:59 PM/ PSP / IDS#202, IDS#192
#define WM_CMD_ABORT_FAIL				(WM_USER + 316)  //Patni-SS/2009Mar31/Added/PSP1#XX
#define WM_CMD_ABORT_WAIT				(WM_USER + 317)  //Patni-SS/2009Mar31/Added/PSP1#XX
#define WM_CMD_DBS_NOT_IN_SYNC          (WM_USER + 318)  //Patni-AMT/2009Mar21/Added/PSP1#001 
#define WM_HANDLE_IRS_MESSAGE			(WM_USER + 319)  //Patni-HEMANT/2009Sept05/Added/ MVC#002493
#define WM_HANDLE_WAITCURSOR_MESSAGE	(WM_USER + 320)  //Patni-Manishkumar/2009Sept15/Added/MVC004760/Msg to set wait cursor
#define WM_EDIT_KILL_FOCUS				(WM_USER + 321)  //Patni-Hemant/2010Feb10/Added/ACE-Phase#3/CDS Requirement
#define WM_CDS_COIL_CONFIRMATION		(WM_USER + 322)  //Patni-Hemant/2010Mar23/Added/ACE-Phase#3/CDS Requirement
#define WM_CDS_LOCATION_DEBUG			(WM_USER + 323)  //Patni-Hemant/2010Mar23/Added/ACE-Phase#3/CDS Requirement
#define WM_CDS_APPLY_CMPS_COIL_CUR_PROT	(WM_USER + 324)  //Patni-Hemant/2010Mar23/Added/ACE-Phase#3/CDS Requirement
#define WM_CMD_FILE_NOT_EXIST			(WM_USER + 325)	 //Patni-HAR/2010Apr21/Added/MVC007534
#define WM_CMD_START_MRS_PRESCAN        (WM_USER + 326)  //Patni-HAR/2010Jun30/Added/V1.30#TMSC-REDMINE-316
#define WM_CMD_ABORT_MRS_PRESCAN        (WM_USER + 327)  //Patni-HAR/2010Jun30/Added/V1.30#TMSC-REDMINE-316
#define WM_OPEN_MANUAL_PRESCAN			(WM_USER + 328)	 //Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
#define WM_CFSHIFT_CFA_READY            (WM_USER + 329)		//Patni-Ravindra Acharya/2010July23/Added/IR-153
//+Patni-Sribanta/2011Jan11/Added/TMSC-REDMINE-1026
#define WM_UPDATE_DEBUG_WINDOW          (WM_USER + 330)
#define WM_PQM2COILSEL_CONNECT_COIL		(WM_USER + 331)
#define WM_MSG_CORRECT_COIL				(WM_USER + 332)
#define WM_PROCESS_MRS_MSG				(WM_USER + 333)
#define WM_REFRESH_GUI					(WM_USER + 334)
#define WM_INVOKE_COIL_INFO				(WM_USER + 335)
#define WM_INVOKE_GATE_DIALOG			(WM_USER + 336)
//-Patni-Sribanta/2011Jan11/Added/TMSC-REDMINE-1026

#define WM_UPDATE_IMAGE_OFFSET (WM_USER + 337)
#define WM_UPDATE_COUCH_OFFSET (WM_USER + 338)
#define WM_CMD_GL_SR_SHOW				(WM_USER + 339)
#define WM_CMD_INJECT_TIMER					(WM_USER + 340)
#define WM_HIDE_CONTEXT					(WM_USER + 341)
#define WM_INVOKE_PROBE_DIALOG          (WM_USER + 342)
#define WM_HANDLE_WAITCURSOR_MESSAGE_PQM_BUTTONS (WM_USER + 343)
#define WM_INVOKE_AMB			                 (WM_USER + 344)
#define WM_CMD_SARMANAGER_START			(WM_USER + 345)
#define WM_CMD_PAGER_TIME_UPDATE		(WM_USER + 346)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #ifndef __SM_STD_AFX_H__

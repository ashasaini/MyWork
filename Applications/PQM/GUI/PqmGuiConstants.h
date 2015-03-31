//TMSC-REDMINE-777
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PqmConstants.h
 *  Overview: Structure of Constants used in PQM.
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
#ifndef PQMCONSTANTS_H
#define PQMCONSTANTS_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Common/CommonDefine.h>
#include <PQM/PQMCommanMessages.h>
#include "PQMSequence.h"
#include "PQMLogMgr.h"
#include "PQMUtility.h"	// Added By KT/ACE-2/11-06-2009/Multilingualisation	
//DICITONARY related constant values
#define PAS_COL_WD          150
#define DEFAULT_COL_WIDTH   40

#define DICT_CENTER               2
#define DICT_RIGHT                1
#define DICT_LEFT                 0

//Patni-PJS/2010July6/Added/TMSC-REDMINE-392_2
#define BK_COLOR RGB(63,82,118)

#define DEFAULT_APP_DIC_ALIGNMENT DICT_CENTER

//Patni-ARD/2010Jan6/Added/DeFT# IR 100
#define DEFAULT_CONTRASTAGENT       _T("Gadolinium")
//DICITONARY related constant values

const CString g_scanID_tag = _T("SEQ");

const int HEIGHT_EDIT_CTRL_COMBO = 39;
const int HEIGHT_ITEMS_IN_COMBO = 40;

//const static int START_VOICEMENU_ITEM = 3000;
//const static int START_LANGUAGEMENU_ITEM = 3500;

//Position & dimension of CPQMSequenceInfoDlg

const int LEFT = 550;
const int TOP = 50;
const int WIDTH = 550;
const int HEIGHT = 400;

const NUM_VOICE = 5;
static TCHAR* auto_voice_code[NUM_VOICE] = {_T("--"), _T("B1"), _T("B2"),
        _T("B3"), _T("M1")
                                           };

typedef CList<CSequence*, CSequence*> CSeqList;

//+Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
enum {
    SCAN_ERR_NOTIFY = 3,    //Blue
    SCAN_ERR_WARNING = 4,   //Yellow 4 = 5
    SCAN_ERR_CRUCIAL = 6    //Red
};
//-Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView

enum {
    ELECTRODE_I = 1,
    ELECTRODE_II,
    ELECTRODE_III,
    ELECTRODE_AVR,
    ELECTRODE_AVL,
    ELECTRODE_AVF
};


#define PST_INT32       0
#define PST_FLT32       1
#define PST_FLT64       2
#define PSK_OVER_SAMPLE_RATE    "OverSampleRate"
#define PSM_OVER_SAMPLE_RATE    overSampleRate
#define PST_OVER_SAMPLE_RATE    PST_INT32
#define PSD_OVER_SAMPLE_RATE    "1"

#define PSK_START_POINT     "StartPoint"
#define PSM_START_POINT     startPoint
#define PST_START_POINT     PST_INT32
#define PSD_START_POINT     "0"

#define PSK_END_POINT       "EndPoint"
#define PSM_END_POINT       endPoint
#define PST_END_POINT       PST_INT32
#define PSD_END_POINT       "1023"

#define PSK_CENTER_POINT    "CenterPoint"
#define PSM_CENTER_POINT    centerPoint
#define PST_CENTER_POINT    PST_INT32
#define PSD_CENTER_POINT    "512"

#define PSK_SHIFT_DIRECTION "ShiftDirection"
#define PSM_SHIFT_DIRECTION shiftDirection
#define PST_SHIFT_DIRECTION PST_INT32
#define PSD_SHIFT_DIRECTION "0"

#define PSK_NUM_CF_AVG      "NumCFAvg"
#define PSM_NUM_CF_AVG      numCFAvg
#define PST_NUM_CF_AVG      PST_INT32
#define PSD_NUM_CF_AVG      "2"

#define PSK_ADC_PITCH       "ADCPitch"
#define PSM_ADC_PITCH       adcPitch
#define PST_ADC_PITCH       PST_INT32
#define PSD_ADC_PITCH       "200"
#define PSK_READOUT_SAMPLE  "ReadoutSample"
#define PSM_READOUT_SAMPLE  readoutSample
#define PST_READOUT_SAMPLE  PST_INT32
#define PSD_READOUT_SAMPLE  "1024"
#define OFFSET(m)   (int)((char *)&(((PreScanCFMethod_p)NULL)->m))
#define KEY_TABLE(n)    {PSK_##n,PST_##n,OFFSET(PSM_##n),PSD_##n}
#define PRESCAN_CF_DAILY        0
#define PRESCAN_CF_RAMPUP       1

#define PRESCAN_CF_METHOD_DAILY     _T("CF.method.Daily")
#define PRESCAN_CF_METHOD_RAMPUP    _T("CF.method.RampUp")
#define PRESCAN_CF_CURVE_DATA       "CF.curveData"

#define PRESCAN_CF_KEY_VALUE        "CenterFrequency:"
#define PRESCAN_CF_ADC_PITCH        "ADCPitch:"
#define PRESCAN_CF_FFT_POINTS       "FFTPoints:"

#define PRESCAN_CF_ADC_SAMPLE       1024
#define PRESCAN_CF_CURVE_DATA_LEN   1024
#define PRESCAN_CF_CURVE_DATA_CENTER    512

#define PQM_IPC_INI_SEC             _T("PQMIPC")
#define PQM_IPC_LOG_FILE            _T("LOG_GUI_FILE")

const int TOTAL_SEQ_INFO_COLUMNS    = 2;
//const CString SEQ_INFO_COLUMN_STRINGS[TOTAL_SEQ_INFO_COLUMNS] = { _T("Parameter Name"), _T("Parameter Value") };  // Added By KT/ACE-2/11-06-2009/Multilingualisation
const CString SEQ_INFO_COLUMN_STRINGS[2] = {CPQMUtility::GetMultiLingualString(_T("IDS_PARAMETER_NAME")), CPQMUtility::GetMultiLingualString(_T("IDS_PARAMETER_VALUE"))}; // Added By KT/ACE-2/11-06-2009/Multilingualisation

const int K_SCHEDULING = 0;
const int K_IMAGING = 1;
const int K_EZSCAN = 2;
const int K_INIT = 99;

// For Trace Log
//Patni-PJS/2011May2/Commented/IR-97
//const _TCHAR DOMAIN_NAME[]                  =   _T("MRI_PQM");

///Markers for Trace Log

#define COMMHANDLER_MARKER_SR_PQM            MR::TRACELOG::MRTraceMarker1
#define COMMHANDLER_MARKER_PQM_VOICE         MR::TRACELOG::MRTraceMarker2
#define COMMHANDLER_MARKER_PQM_ACQMAN        MR::TRACELOG::MRTraceMarker3
#define COMMHANDLER_MARKER_PQM_DPS           MR::TRACELOG::MRTraceMarker4
#define GUI_CONTROLS_MARKER                  MR::TRACELOG::MRTraceMarker5
#define SCAN_OPERATION_MARKER                MR::TRACELOG::MRTraceMarker6
#define USER_FUNC_MARKER                     MR::TRACELOG::MRTraceMarker7
#define SYSTEM_FUNC_MARKER                   MR::TRACELOG::MRTraceMarker8
#define GUI_FUNCTION_MARKER_IN               MR::TRACELOG::MRTraceMarkerIn
#define GUI_FUNCTION_MARKER_OUT              MR::TRACELOG::MRTraceMarkerOut

#define	CF_SHOW_PPM    6.0	//Patni-ARD/20090315/Added/ACE-1 # Prescan newly added
// For Event Log

const _TCHAR USER_NAME[]                    =   _T("TMSC");

const _TCHAR APP_NAME[]                     =   _T("PQM");

// For Operation Log

const BYTE SUBSYSTEM_ID                     =    0x41;
const BYTE COMPONENT_ID                     =    0x02;

//Operation Id for Operation Log
const WORD OP_ID_APP                        =    0x0000;
const WORD OP_ID_ANATOMY                    =    0x0010;
const WORD OP_ID_COMMENT                    =    0x0020;
const WORD OP_ID_SEQ_OP                     =    0x0030;
const WORD OP_ID_SCAN_OP                    =    0x0040;
const WORD OP_ID_DLG_CLOSE                  =    0x004A;
const WORD OP_ID_SCAN_OFFSET                =    0x0050;
const WORD OP_ID_CENTER_FREQ                =    0x006A;
const WORD OP_ID_MENU_PAS                   =    0x0070;
const WORD OP_ID_MENU_INFO                  =    0x007A;
const WORD OP_ID_MENU_UTILITY               =    0x0080;
const WORD OP_ID_MENU_VOICE                 =    0x00A0;
const WORD OP_ID_SCAN_STG                   =    0x00AA;

// Application start,close
const WORD OP_ID_APP_START = OP_ID_APP + 0x0000;
const WORD OP_ID_APP_CLOSE = OP_ID_APP + 0x0001;

// Anatomy dialog buttons
const WORD OP_IDB_ANATOMY_SELECTION = OP_ID_ANATOMY + 0x0001;
const WORD OP_IDB_ANATOMY_CLOSE     = OP_ID_ANATOMY + 0x0002;

// Comment dialog buttons
const WORD OP_IDB_SCAN_COMMENT  = OP_ID_COMMENT + 0x0001;
const WORD OP_IDB_COMMENT_CLOSE = OP_ID_COMMENT + 0x0002;

// Sequence operations buttons
const WORD OP_IDB_SET_ALL       = OP_ID_SEQ_OP + 0x0001;
const WORD OP_IDB_SEQ_EDITOR    = OP_ID_SEQ_OP + 0x0002;
const WORD OP_IDB_SEQ_DELETE    = OP_ID_SEQ_OP + 0x0003;
const WORD OP_IDB_SEQ_DUPLICATE = OP_ID_SEQ_OP + 0x0004;

// Scanning buttons
const WORD OP_IDB_SCAN_PLAN   = OP_ID_SCAN_OP + 0x0001;
const WORD OP_IDB_SCAN_START  = OP_ID_SCAN_OP + 0x0002;
const WORD OP_IDB_SCAN_PAUSE  = OP_ID_SCAN_OP + 0x0003;
const WORD OP_IDB_SCAN_RESUME = OP_ID_SCAN_OP + 0x0004;
const WORD OP_IDB_SCAN_ABORT  = OP_ID_SCAN_OP + 0x0005;

// Close buttons of Information dialogs
const WORD OP_IDB_SEQ_INFO_CLOSE  = OP_ID_DLG_CLOSE + 0x0001;
const WORD OP_IDB_COIL_INFO_CLOSE = OP_ID_DLG_CLOSE + 0x0002;
const WORD OP_IDB_GATE_INFO_CLOSE = OP_ID_DLG_CLOSE + 0x0003;

// Scan Offset dialog buttons
const WORD OP_IDB_SCAN_OFFSET_X_INCRE = OP_ID_SCAN_OFFSET + 0x0001;
const WORD OP_IDB_SCAN_OFFSET_X_DECRE = OP_ID_SCAN_OFFSET + 0x0002;
const WORD OP_IDB_SCAN_OFFSET_Y_INCRE = OP_ID_SCAN_OFFSET + 0x0003;
const WORD OP_IDB_SCAN_OFFSET_Y_DECRE = OP_ID_SCAN_OFFSET + 0x0004;
const WORD OP_IDB_SCAN_OFFSET_Z_INCRE = OP_ID_SCAN_OFFSET + 0x0005;
const WORD OP_IDB_SCAN_OFFSET_Z_DECRE = OP_ID_SCAN_OFFSET + 0x0006;
const WORD OP_IDB_SCAN_OFFSET_SET     = OP_ID_SCAN_OFFSET + 0x0007;
const WORD OP_IDB_SCAN_OFFSET_CANCEL  = OP_ID_SCAN_OFFSET + 0x0008;

// Center Frequency Control dialog buttons
const WORD OP_IDB_CENTER_FREQ_CFA_RETRY = OP_ID_CENTER_FREQ + 0x0001;
const WORD OP_IDB_CENTER_FREQ_LEFT      = OP_ID_CENTER_FREQ + 0x0002;
const WORD OP_IDB_CENTER_FREQ_RIGHT     = OP_ID_CENTER_FREQ + 0x0003;
const WORD OP_IDB_CENTER_FREQ_OK        = OP_ID_CENTER_FREQ + 0x0004;
const WORD OP_IDB_CENTER_FREQ_APPLY     = OP_ID_CENTER_FREQ + 0x0005;
const WORD OP_IDB_CENTER_FREQ_CANCEL    = OP_ID_CENTER_FREQ + 0x0006;

// Menus
// PAS Menu
const WORD OP_IDM_PAS_POPUP_GET_MEMORIZED = OP_ID_MENU_PAS + 0x0001;
const WORD OP_IDM_PAS_POPUP_STORE         = OP_ID_MENU_PAS + 0x0002;
const WORD OP_IDM_PAS_POPUP_STORE_EZPAS   = OP_ID_MENU_PAS + 0x0003;

// Information Menu
const WORD OP_IDM_INFORMATION_POPUP_SEQ_INFO  = OP_ID_MENU_INFO + 0x0001;
const WORD OP_IDM_INFORMATION_POPUP_COIL_INFO = OP_ID_MENU_INFO + 0x0002;
const WORD OP_IDM_INFORMATION_POPUP_GATE_INFO = OP_ID_MENU_INFO + 0x0003;

// Utility Menu
const WORD OP_IDM_UTILITY_POPUP_SCAN_OFFSET      = OP_ID_MENU_UTILITY + 0x0001;
const WORD OP_IDM_UTILITY_POPUP_AUTO_PLAN        = OP_ID_MENU_UTILITY + 0x0002;
const WORD OP_IDM_UTILITY_POPUP_PLAN_SCAN        = OP_ID_MENU_UTILITY + 0x0003;
const WORD OP_IDM_UTILITY_POPUP_SKIP_RGN         = OP_ID_MENU_UTILITY + 0x0004;
const WORD OP_IDM_UTILITY_POPUP_ALL_COVERAGE_RGN = OP_ID_MENU_UTILITY + 0x0005;
const WORD OP_IDM_UTILITY_POPUP_SPEEDERMAP       = OP_ID_MENU_UTILITY + 0x0006;
const WORD OP_IDM_UTILITY_POPUP_MULTI_COVERAGE   = OP_ID_MENU_UTILITY + 0x0007;
const WORD OP_IDM_UTILITY_POPUP_BATCH_SETTING    = OP_ID_MENU_UTILITY + 0x0008;
const WORD OP_IDM_UTILITY_POPUP_ECG              = OP_ID_MENU_UTILITY + 0x0009;
const WORD OP_IDM_UTILITY_POPUP_PERIPHERAL       = OP_ID_MENU_UTILITY + 0x000A;
const WORD OP_IDM_UTILITY_POPUP_RESPIRATORY      = OP_ID_MENU_UTILITY + 0x000B;
const WORD OP_IDM_UTILITY_POPUP_ELECTRODE_I      = OP_ID_MENU_UTILITY + 0x000C;
const WORD OP_IDM_UTILITY_POPUP_ELECTRODE_II     = OP_ID_MENU_UTILITY + 0x000D;
const WORD OP_IDM_UTILITY_POPUP_ELECTRODE_III    = OP_ID_MENU_UTILITY + 0x000E;
const WORD OP_IDM_UTILITY_POPUP_ELECTRODE_AVR    = OP_ID_MENU_UTILITY + 0x000F;
const WORD OP_IDM_UTILITY_POPUP_ELECTRODE_AVL    = OP_ID_MENU_UTILITY + 0x0010;
const WORD OP_IDM_UTILITY_POPUP_ELECTRODE_AVF    = OP_ID_MENU_UTILITY + 0x0011;
const WORD OP_IDM_UTILITY_POPUP_PREP_WB_COIL     = OP_ID_MENU_UTILITY + 0x0012;
const WORD OP_IDM_UTILITY_POPUP_SHIMMING_WB_COIL = OP_ID_MENU_UTILITY + 0x0013;
const WORD OP_IDM_UTILITY_CENTER_FREQ            = OP_ID_MENU_UTILITY + 0x0014;
const WORD OP_IDM_UTILITY_ADJUST_CENTER_FREQ     = OP_ID_MENU_UTILITY + 0x0015;
const WORD OP_IDM_UTILITY_APC_RETRY              = OP_ID_MENU_UTILITY + 0x0016;
const WORD OP_IDM_UTILITY_APC_PAUSE              = OP_ID_MENU_UTILITY + 0x0017;

// Voice Menu
const WORD OP_IDM_VOICE_POPUP_TEST_BREATH1   = OP_ID_MENU_VOICE + 0x0001;
const WORD OP_IDM_VOICE_POPUP_TEST_BREATH2   = OP_ID_MENU_VOICE + 0x0002;
const WORD OP_IDM_VOICE_POPUP_TEST_BREATH3   = OP_ID_MENU_VOICE + 0x0003;
const WORD OP_IDM_VOICE_POPUP_TEST_MOTION    = OP_ID_MENU_VOICE + 0x0004;
const WORD OP_IDM_VOICE_POPUP_TEST_RELAX     = OP_ID_MENU_VOICE + 0x0005;
const WORD OP_IDM_VOICE_POPUP_TEST_USER      = OP_ID_MENU_VOICE + 0x0006;
const WORD OP_IDM_VOICE_POPUP_LANG_SELECTION = OP_ID_MENU_VOICE + 0x0007;

const int NUM_1     = 1;
const int NUM_4     = 4;
const int NUM_8     = 8;
const int NUM_10    = 10;
const int NUM_40    = 40;
const int NUM_50    = 50;
const int NUM_80    = 80;
const int NUM_100   = 100;
const int NUM_128   = 128;
const int NUM_110   = 110;
const int NUM_150   = 150;
const int NUM_200   = 200;
const int NUM_240   = 240;
const int NUM_250   = 250;
const int NUM_255   = 255;
const int NUM_262   = 262;
const int NUM_280   = 280;
const int NUM_1024  = 1024;
const int MAX_LINE  = 152;
const int NUM_295   = 295;
const int NUM_260   = 260;
const int NUM_25    = 25;
const int NUM_12    = 12;
const int NUM_14    = 14;
const int NUM_26    = 26;
const int MAX_SEQUENCE = 10;

const DWORD ERR_ID = 111;

// Voice
const CString PQM_AV_BREATH1            =   L"Breath1";
const CString PQM_AV_BREATH2            =   L"Breath2";
const CString PQM_AV_BREATH3            =   L"Breath3";
const CString PQM_AV_MOTION             =   L"Motion";
const CString PQM_AV_RELAX              =   L"Relax";

const int PQM_AV_VOICE_1            =   1;
const int PQM_AV_VOICE_2            =   2;
const int PQM_AV_VOICE_3            =   3;
const int PQM_AV_VOICE_4            =   4;
const int PQM_AV_VOICE_5            =   5;

const int PQM_AV_NUM_VOICES         =   5;

const int PQM_AV_CMD_PLAY           =   1;
const int PQM_AV_CMD_PLAY_W         =   2;
const int PQM_AV_CMD_DELAY          =   3;
const int PQM_AV_CMD_DELAY_W        =   4;
const int PQM_AV_CMD_VOLUME         =   5;
const int PQM_AV_CMD_VOLUME_W       =   6;
const int PQM_AV_CMD_PROG           =   7;
const int PQM_AV_CMD_PROG_W         =   8;
const int PQM_AV_CMD_PDELAY         =   9;
const int PQM_AV_CMD_PDELAY_W       =   10;
const int PQM_AV_CMD_INTERVAL       =   11;
const int PQM_AV_CMD_INTERVAL_W     =   12;
const int PQM_AV_CMD_TRAP           =   13;
const int PQM_AV_CMD_TRAP_W         =   14;
const int font_height = 14;
const int height = 12;
const int width = 2;
const int buf_size = 256;
const int max_text = 255;

// constants for CFA Dialog
const int VFC_SHIM_FSY_WATER = 0;
const int VFC_SHIM_FSY_FAT =   1;

const double FAT_MOVE_GRAPH_CORD = -90;
const double WATER_MOVE_GRAPH_CORD = 0;

const int DECIMAL_ONE_DIVIDE_FACTOR  = 10;
const double FLOAT_ONE_DIVIDE_FACTOR  = 10.0;
const double FLOAT_THOUSAND_DIVIDE_FACTOR  = 10000000.0;

const int MIN_PPM_OFFSET_VAL = -60;
const int MAX_PPM_OFFSET_VAL = 60;

const int PPM_TEXT_LIMIT = 4;

const int CFA_VALID_DOT_POS = 2;

//commented by Hemant - done in different way..
//const int GRAPH_CTRL_ID = 60000;

const int ZERO_OFFSET_VAL   = 0;

const double DOUBLE_NUM_2   = 2.0;
const double DOUBLE_NUM_5   = 5.0;
const double DOUBLE_NUM_10  = 10.0;
const double DOUBLE_NUM_40  = 40.0;
const double DOUBLE_NUM_90  = 90.0;
const double DOUBLE_NUM_95  = 95.0;
const double DOUBLE_NUM_100 = 100.0;
const double DOUBLE_NUM_640 = 640.0;

const int VFC_GAT_METHOD_BIT_ECG = 1;   /* ECG */
const int VFC_GAT_METHOD_BIT_PERIPHERAL = (1 << 1); /* Peripheral */
const int VFC_GAT_METHOD_BIT_RESPIRATORY = (1 << 2);  /* Respiratory */
const int  PQM_DLG_SHIM = 1;
//+Patni-PJS/2010Apr20/Added/JaFT# MVC007170_CDR_01
const LPCTSTR Z_ORDER_PANEL_GL_LAYER  = _T("Z_ORDER_PANEL_GL_LAYER");
const LPCTSTR Z_ORDER_ISEL_LAYER      = _T("Z_ORDER_ISEL_LAYER");
const LPCTSTR Z_ORDER_SYSTEM_LAYER    = _T("Z_ORDER_SYSTEM_LAYER");
//-Patni-PJS/2010Apr20/Added/JaFT# MVC007170_CDR_01
#define VFC_SHIM_NONE           0
#define VFC_SHIM_STANDARD       1


#define E_NO_ERROR      0
#define E_ERROR         1
#define E_ERROR_STAT  -1


#define BACKGROUNDCOLOR					RGB(35,48,70)
#define TEXTCOLOR						RGB(255,255,255)
#define MENUACTIVETEXTCOLOR             RGB(255, 255, 255)
#define MENUINACTIVETEXTCOLOR           RGB(150, 150, 150)
#define TEXT_INACTIVECOLOR				RGB(150, 150, 150)

#define RESET_BTN_BACKGROUNDCOLOR       RGB(50,112,121);
#define RESET_BTN_UPPEREDGESHADOWCOLOR  RGB(158, 158, 158);
#define RESET_BTN_BOTTOMEDGESHADOWCOLOR RGB(23, 64, 69);
#define RESET_BTN_FOCUSRECTCOLOR        RGB(0, 255, 255);

enum CFADlg_Status {
    CFA_DLG_NORMAL = 0,
    CFA_DLG_SHIMMING_DONE,
    CFA_DLG_SHIMMING_SEQUENCE
};


enum {
    SCAN_WARN_INFORMATION = 3,
    SCAN_WARN_WARNING = 4,
    SCAN_WARN_CRUCIAL = 5

};

enum {
    SCAN_WARN_SYSMODAL = 1,
    SCAN_WARN_APPMODAL = 2,
    SCAN_WARN_MODELESS = 3,
};

enum {
    SCAN_WARN_OKBUTTON = 1,
    SCAN_WARN_CANCELBUTTON = 2,
    SCAN_WARN_RETRYBUTTON = 4,
    SCAN_WARN_YESBUTTON = 8,
    SCAN_WARN_NOBUTTON = 10,
    SCAN_WARN_OKONLYBUTTON = SCAN_WARN_OKBUTTON,
    SCAN_WARN_OK_CANCEL_BUTTONS = SCAN_WARN_OKBUTTON | SCAN_WARN_CANCELBUTTON,
    SCAN_WARN_OK_CANCEL_RETRY_BUTTONS = SCAN_WARN_OKBUTTON | SCAN_WARN_CANCELBUTTON | SCAN_WARN_RETRYBUTTON,
    SCAN_WARN_YES_NO_BUTTONS = SCAN_WARN_YESBUTTON | SCAN_WARN_NOBUTTON
};



//Added by Hemant On 3/5/2009 1:00:29 PM , For coil dialogs
#define AFX_IDS_FONTSIZE	10


#define FILE_PATH_SIZE 500
//Patni-PJS/2010Nov23/Modified/TMSC-REDMINE-774
//const int MAXIMUM_COIL_IN_PORT = 16;

const CString DPS_MSGID_FW_LOADGL  = _T("FW_LoadGL");
const CString DPS_CALA_PARAM = _T("CALA/CALA.Document");
const CString DPS_HALA_PARAM = _T("HALA/HALA.Document");
const CString DPS_CALA_CUSTOM = _T("0/0");
const TCHAR DEFAULT_AL_INVOKE_MODE = _T('1');
const int CALA_REPLY_TIME_OUT = 60000;
const int WFDA_REPLY_TIME_OUT = 10000;
//const COLORREF  g_pqmdialog_static_ctrl_bk_color	 = RGB(63,82,118);
const COLORREF  g_pqmdialog_static_ctrl_text_color   = RGB(191, 209, 255);
const CString g_pqmdialog_static_font_name		     =	_T("Arial");
const int g_pqmdialog_static_font_size			     =	130 ;

const COLORREF g_pqm_static_bkcolor				    = RGB(15, 18, 29);
const COLORREF g_pqm_static_textcolor				= RGB(191, 209, 255) ;

const CString pqm_static_font_name = _T("Arial");
const UINT pqm_static_font_size	 = 120;

//Patni-Hemant/2009Nov17/Modified/IR#90/Bottom Buttons
const COLORREF g_no_data_warning_color              = RGB(254, 204, 0);

//+Patni-RUP/2010Jul22/Commented/TMSC-REDMINE-466
//+Patni-DKH/2010Apr12/Added/Phase3#MCM0229-00187
//+Patni-PJS/2010Apr22/Added/JaFT# MVC007277
//typedef enum {
//    NOMOVE          = -1,
//    CENTREPLAN      = 0,
//    PARENTREFPOS    = 1
//} MOVECOUCH_METHOD;
//-Patni-PJS/2010Apr22/Added/JaFT# MVC007277
//-Patni-DKH/2010Apr12/Added/Phase3#MCM0229-00187
//-Patni-RUP/2010Jul22/Commented/TMSC-REDMINE-466

//+Patni-Hemant/2010Feb26/Added/ACE-Phase#3/CDS Requirement
const int __recv_coil_id = 1094909952 ; //COILKEY;
#define RECVR_COIL _T("RC")
//-Patni-Hemant/2010Feb26/Added/ACE-Phase#3/CDS Requirement
//+Patni-PJS/2010July1/Added/V1.30/TMSC-REDMINE-392
const CString DLG_TITLE_FONT_NAME	= _T("Arial");


const int DLG_TITLE_FONT_SIZE		= 15;
//-Patni-PJS/2010July1/Added/V1.30/TMSC-REDMINE-392

//+Patni-Hemant/2010Apr28/Added/JaFT/IR-137
const CString g_L_None_MenuName = _T("");
const CString g_L_Left_MenuName = _T("Left");
const CString g_L_Right_MenuName = _T("Right");

const CString g_L_None_ShortName = _T("");
const CString g_L_Left_ShortName = _T("L");
const CString g_L_Right_ShortName = _T("R");
const CString g_L_Invalid_ShortName = _T("");

//+Patni-Ravindra Acharya/2010June25/Added/IR-147
const LPCTSTR Left_AxisLocator = _T("res\\CoilSectionScreen\\Axislocator_Left.bmp") ;
const LPCTSTR Right_AxisLocator = _T("res\\CoilSectionScreen\\Axislocator_Right.bmp") ;
//+Patni-Ravindra Acharya/2010June25/Added/IR-147
//-Patni-Hemant/2010Apr28/Added/JaFT/IR-137
//+Patni-HAR/2010Apr26/Added/IR-134
#define SCRIPT_EXE      _T("Perl")
#define CALC_EDDY_B0    _T("calcMRSEddy")
#define CALC_EDDY_B1    _T("calcMRSEddy_Dif")
//Patni-HAR/2010May07/Modified/IR-134
//-Patni-HAR/2010Apr26/Modified/IR-134--Reading of Process Path from env var
#define EDDY_SCRIPT     _T(" /gp/seq/sequences/Utilities/proRec/MRSeddy/MRSeddy_proc.pl")
#define LAUNCHER_BATCH  _T("MTLauncher.bat");
//-Patni-HAR/2010Apr26/Added/IR-134

//+Patni-DKH/2010Dec03/Modified/TMSC-REDMINE-979
const DWORD g_dps_syncmsg_timeout = 30000;


const int NUM_5		= 5;
const int NUM_60	= 60;
const int SECOND_MIN_DELAY = 00;
const int SECOND_MAX_DELAY = 59;
const int MINUTE_MIN_DELAY = 00;
const int MINUTE_MAX_DELAY = 99;
const int INCREASE_BY_NUMBER = 01;
const int DELAY_TIME_LENGTH = 02;


const int SHIM_STATE_WAIT           =   0;
const int SHIM_STATE_READY          =   1;
const int SHIM_STATE_ACTIVE         =   2;


#endif //PQMCONSTANTS_H
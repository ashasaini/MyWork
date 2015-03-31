/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMGUICommon.h
 *  Overview: Structure GUI Commonly used constants and structures.
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
#ifndef PQM_GUICOMMON_H
#define PQM_GUICOMMON_H

#include <PQM/resource.h>

const int  PQM_MAX_PROTOCOLS =	64;	       //Maximum number of protocols allowed in PQM window

const int  VFC_PATORI_HEAD_FIRST = 0;		// Patient entered magnet head first
const int  VFC_PATORI_FEET_FIRST = 1;		// Patient entered magnet feet first

const int  VFC_PATORI_PRONE	 = 0;			// Patient has back side up in magnet
const int  VFC_PATORI_SUPINE = 1;			// Patient has front side up in magnet
const int  VFC_PATORI_LEFT_UP =	2;			// Patient has left side up in magnet
const int  VFC_PATORI_RIGHT_UP = 3;			// Patient has right side up in magnet


const int  VFC_SAR_ANATOMY_HEAD = 1;
const int  VFC_SAR_ANATOMY_ABDOMEN = 2;
const int  VFC_SAR_ANATOMY_CHEST = 3;
const int  VFC_SAR_ANATOMY_CSPINE = 4;
const int  VFC_SAR_ANATOMY_TLSPINE = 5;
const int  VFC_SAR_ANATOMY_PELVIS = 6;
const int  VFC_SAR_ANATOMY_LOWER_EXT = 7;
const int  VFC_SAR_ANATOMY_UPPER_EXT = 8;
const int  VFC_SAR_ANATOMY_MISC = 9;

const int  VFC_VIEW_FROM_HEAD =	0;			// View images from head as default
const int  VFC_VIEW_FROM_FEET =	1;			// from feet as default

const int  PR_ACQ_STATUS_WAIT = 0;
const int  PR_ACQ_STATUS_SUCCESS = 1;
const int  PR_ACQ_STATUS_FAIL = 2;
const int  PR_ACQ_STATUS_CURRENT = 3;

#define VFC_HOLD_PRE_SCAN	(1L<<0)			// 制御＆通知    on press auto scan   
#define VFC_HOLD_MAIN_SCAN	(1L<<1)			// 制御＆通知
#define VFC_HOLD_COVERAGE	(1L<<2)			// 制御＆通知    on press breath hold 
#define VFC_HOLD_PREP		(1L<<3)			// 通知 */
#define VFC_HOLD_MOVE_BED	(1L<<4)			// 制御＆通知 
#define VFC_HOLD_COUCH		(1L<<10)		// PQMのみで使用 on press move couch  
#define VFC_HOLD_APC		(1L<<16)		// 制御＆通知 
#define VFC_HOLD_CFA		(1L<<17)		// 制御＆通知 
#define VFC_HOLD_RGN		(1L<<18)		// 制御＆通知 


const int  VFC_AUTO_VOICE_NONE = 0;			// オートボイスOFF
const int  VFC_AUTO_VOICE_BREATH1 =	1;		// 息止めオートボイス
const int  VFC_AUTO_VOICE_BREATH2 =	2;		// 息止めオートボイス
const int  VFC_AUTO_VOICE_BREATH3 =	3;		// 息止めオートボイス
const int  VFC_AUTO_VOICE_MOTION = 4;		// 体動防止オートボイス

const int PROT_LIMIT1 = 25; // for protocol limit (after 'Z' change to 'AA')
const int PROT_LIMIT2 = 51; // for protocol limit (after 'ZZ' change to 'AAA')

//Menu label offset
const int  MENU_LEFT_OFFSET = 50;
const int  MENU_RIGHT_OFFSET = 50;

const int MIN_SLIDER_RANGE = 0;
const int MAX_SLIDER_RANGE_X = 400;
const int MAX_SLIDER_RANGE_Y = 200;
const int MAX_SLIDER_RANGE_Z = 200;

const int TEXT_LIMIT = 6;
//const float MIN_SCAN_OFFSET_X = -20.0;
//const float MAX_SCAN_OFFSET_X =  20.0;
//const float MIN_SCAN_OFFSET_Y = -10.0;
//const float MAX_SCAN_OFFSET_Y =  10.0;
//const float MIN_SCAN_OFFSET_Z = -10.0;
//const float MAX_SCAN_OFFSET_Z =  10.0;
const int ADJUST_SLIDER_POS_X = 20;
const int ADJUST_SLIDER_POS_Y = 10;
const int ADJUST_SLIDER_POS_Z = 10;
const int SLIDER_POS_MULTIPLY_FACTOR = 10;
const int SLIDER_POS_DIVIDE_FACTOR = 2;
// const double TEMP_VAL = 0.1;
const float TEMP_VAL = (float)0.1;

const int VALID_DOT_POS = 3;

//Bitmap length & width for combo box
const BITAMP_LENGTH = 76;
const BITAMP_WIDTH = 38;

//Constants for scroll dialog
const int  PAGEUPDOWN = 10;
const int  LINEUPDOWN = 5;
const int  MAX_POS_DEV = 20;


// This enum is used to define sar status constants.
typedef	enum	_sar_status_e {
    PQM_SAR_OK = 0,		    // SAR is good for Scan.
    PQM_SAR_OK_BUT_SAR,		// SAR is NG of SAR, but Scan ok.
    PQM_SAR_OK_BUT_COIL,	// SAR is NG of COIL, but Scan ok.
    PQM_SAR_ERROR,		    // SAR is NG of COIL, but Scan ok.
    PQM_SAR_LIMIT_OVER,		// SAR is NG
    PQM_SAR_COIL_PROTECT,	// SAR is NG for coil protect.
    PQM_SAR_ALL_STS
} sar_status_e;

// This enum is used to define sar regions.
typedef enum SAR_REGION {
    ABDOMEN = 1,
    CHEST  ,
    PELVIS ,
    TL_SPINE  ,
    SHOULDER  ,
    HEAD ,
    C_SPINE ,
    EXTREMITY ,
    HAND ,
    ANKLE ,
    EXTREMITY2,
    SAR_REGIONS

    //    HEAD = 0,
    //    C_SPINE,
    //    TL_SPINE,
    //	CHEST,
    //    ABDOMEN,
    //    PELVIS,
    //    SHOULDER,
    //    EXTREMITY,
    //    EXTREMITY2,
    //    HAND,
    //    ANKLE,
    //	SAR_REGIONS
};

// This structure is used to group sar regions
struct ANATOMY_GROUP {

    int static_anatomy_id;  // ID of static anatomy
    int bitmap_b;           // Bitmap ID after selecting anatomy
    int bitmap_s;           // Bitmap ID before selecting anatomy
    int left;               // left x-coordinate of anatomy
    int top;                // top y-coordinate of anatomy
};

// Initialise Anatomy structure
static ANATOMY_GROUP sar[SAR_REGIONS] = {

    {IDC_STATIC_ABDOMEN_BUTTON, IDB_BITMAP_B_ABDOMEN, IDB_BITMAP_S_ABDOMEN, 76, 244},
    {IDC_STATIC_CHEST_BUTTON, IDB_BITMAP_B_CHEST, IDB_BITMAP_S_CHEST, 76, 175},
    {IDC_STATIC_PELVIS_BUTTON, IDB_BITMAP_B_PELVIS, IDB_BITMAP_S_PELVIS, 66, 298},
    {IDC_STATIC_TLSPINE_BUTTON, IDB_BITMAP_B_TLSPINE, IDB_BITMAP_S_TLSPINE, 243, 179},
    {IDC_STATIC_SHOULDER_BUTTON, IDB_BITMAP_B_SHOULDER, IDB_BITMAP_S_SHOULDER, 36, 175},
    {IDC_STATIC_HEAD_BUTTON, IDB_BITMAP_B_HEAD, IDB_BITMAP_S_HEAD, 83, 90},
    {IDC_STATIC_CSPINE_BUTTON, IDB_BITMAP_B_CSPINE, IDB_BITMAP_S_CSPINE, 243, 141},
    {IDC_STATIC_EXTREMITY_BUTTON, IDB_BITMAP_B_EXTREMITY, IDB_BITMAP_S_EXTREMITY, 66, 372},
    {IDC_STATIC_EXTREMITY2_BUTTON, IDB_BITMAP_B_EXTREMITY2, IDB_BITMAP_S_EXTREMITY2, 149, 90},
    {IDC_STATIC_HAND_BUTTON, IDB_BITMAP_B_HAND, IDB_BITMAP_S_HAND, 136, 10},
    {IDC_STATIC_ANKLE_BUTTON, IDB_BITMAP_B_ANKLE, IDB_BITMAP_S_ANKLE, 201, 484}

    //    	{IDC_STATIC_HEAD,IDB_BITMAP_B_HEAD,IDB_BITMAP_S_HEAD,83,90},
    //	{IDC_STATIC_CSPINE,IDB_BITMAP_B_CSPINE,IDB_BITMAP_S_CSPINE,243,141},
    //	{IDC_STATIC_TLSPINE,IDB_BITMAP_B_TLSPINE,IDB_BITMAP_S_TLSPINE,243,179},
    //	{IDC_STATIC_CHEST,IDB_BITMAP_B_CHEST,IDB_BITMAP_S_CHEST,76,175},
    //	{IDC_STATIC_ABDOMEN,IDB_BITMAP_B_ABDOMEN,IDB_BITMAP_S_ABDOMEN,76,244},
    //	{IDC_STATIC_PELVIS,IDB_BITMAP_B_PELVIS,IDB_BITMAP_S_PELVIS,66,298},
    //	{IDC_STATIC_SHOULDER,IDB_BITMAP_B_SHOULDER,IDB_BITMAP_S_SHOULDER,36,175},
    //	{IDC_STATIC_EXTREMITY,IDB_BITMAP_B_EXTREMITY,IDB_BITMAP_S_EXTREMITY,66,372},
    //	{IDC_STATIC_EXTREMITY2,IDB_BITMAP_B_EXTREMITY2,IDB_BITMAP_S_EXTREMITY2,149,90},
    //	{IDC_STATIC_HAND,IDB_BITMAP_B_HAND,IDB_BITMAP_S_HAND,136,10},
    //	{IDC_STATIC_ANKLE,IDB_BITMAP_B_ANKLE,IDB_BITMAP_S_ANKLE,201,484}

};

//Patni-Hemant/2009Nov24/Commented/IR#90

// This structure is used to store scan offset
typedef struct {
    float x;
    float y;
    float z;
} Vector_t;


// This structure is used to store orientation of patient in magnet
typedef struct {
    // End of patient that entered the magnet
    int	end_in;

    // Patient pos on couch
    int	side_up;
} Patori_t;

typedef enum {
    STATE_READY = 3,
    STATE_ACTIVE = 4,
    STATE_INACTIVE = 5
} MessageState;

#define TRUE_STATE					_T("true")
#define FALSE_STATE					_T("false")
#define DPS_PAGER					_T("Pager")

const int MAX_TIMER_COUNT			= 5999;
const int INJECT_TIMER_INTERVAL		= 500;
#define ZERO_TIME					_T("00:00")
#define	INVALID_STRING				_T("----------")
#define DELAYTIME					_T("DELAYTIME")

#define PQM_INVALID_PROTOCOL	-1
#define PQM_NORMAL_PROTOCOL		0
#define PQM_SLAVE_PROTOCOL		1
#define PQM_MASTER_PROTOCOL		2

#endif

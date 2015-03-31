// dataTypes.h: Header file for common variables
//
// test
/*****************************************************************************************

Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
Module: PlanDuplicator
Author: PATNI-PlanDuplicator Team
*****************************************************************************************/

#if !defined(AFX_PD_DATATYPES_H__C94B9E67_ECDF_4742_B72E_4094BD31D931__INCLUDED_)
#define AFX_PD_DATATYPES_H__C94B9E67_ECDF_4742_B72E_4094BD31D931__INCLUDED_

//#include "StdAfx.h"

#include "C:\MRMPlus\include\tami\common\basicdefs.h"

typedef enum{
    AXIAL,
    SAGITTAL,
    CORONAL
} PlaneAxis;

#define SLICE_PLANE_SCOUT2 "scout2"
#define SLICE_PLANE_MAP "map"
#define SLICE_PLANE_SHIMMING "shimming"
#define SLICE_PLANE_MULTISLICE "multislice"

#define FONT_SIZE_10 10
#define FONT_SIZE_12 12

#define DICOM_VER_BUFF_SZ 2

#define BUFFER_SIZE_8 8
#define BUFFER_SIZE_16 16
#define BUFFER_SIZE_20 20
#define BUFFER_SIZE_64 64
#define BUFFER_SIZE_100 100
#define BUFFER_SIZE_128 128
#define BUFFER_SIZE_160 160
#define BUFFER_SIZE_256 256
#define BUFFER_SIZE_1024 1024

#define PROT_PATH_LEVEL 1
#define SERIES_PATH_LEVEL 2
#define VG_PATH_LEVEL 3
#define IMG_PATH_LEVEL 4

typedef enum ErrorCode_Enum_e{
PD_NO_ERROR = 0,
PD_NO_CONFIG_FILE,// 1
PD_READ_CONFIG_FILE,// 2
PD_FTP_CONNECTION, // 3
PD_FTP_TRANSFER, // 4
PD_DATA_FILE_OPEN,// 5
PD_STUDY_OPEN,// 6
PD_VARFIELD_UPDATE, // 7
PD_SEQUENCE_UPDATED, // 8
PD_DATA_FILE_INVALID, // 9
PD_SELECT_UNSCANNED_PROTOCOL, // 10
PD_NO_UNSCANNED_PROTOCOL, // 11
PD_IMAGE_SIZE, // 12
PD_SERVER_INIT_DONE, // 13
PD_SRV_CONNECTION_ERROR, // 14
PD_SOC_WRITE_FAIL, // 15
PD_REQ_UPDATE_VARFIELD,// 16
PD_DATA_FILE_MISSING, // 17
PD_SLICEPLANE_INVALID, //18
PD_PARAMETER_INVALID, //19
PD_SEND_MSG_PQM, //20
PD_SELECT_SCANNED_PROTOCOL, //21
PD_DICOM_FILE_CREATION, //22
PD_MEMORY_ALLOCATION, //23
PD_NODE_DICOM_CREATION, //24
PD_EXCEPTION, //25
PD_MPLUSDB,//26
PD_CONFIG_OPEN,//27
PD_CONFIG_SECTION,//28
PD_CONFIG_KEY,//29
PD_NO_SCANNED_PROTOCOL, //30
PD_CALC_ERROR,           //31
PD_DATA_FILE_ABSENT,       //32
PD_PROC_ERROR,              //33
PD_SELECT_SLICEPLAN    //34
} ErrorCode_Enum_t;
typedef enum {
        ALL,
        SLICE_ORI_VECTOR = 1,
        PHASE_ORI_VECTOR,
        OFFSET_VECTOR,
        COUCH_OFFSET,
        SLICE_THICKNESS,
        SLICE_GAP,
        SLICE_NUM,
        FOV,
        COORD1,
        COORD2
} PDData_t;

typedef enum {
        SEL_SLICE_NUM,
        SEL_SLICE_GAP,
        SEL_SLICE_THICKNESS,
        NONE
}PDSliceParam_t;

typedef enum {
    SMART_VOI,
    SMART_MPR
} PDAppMode_t;
typedef struct funcParams_s {
   // int error_code;
    //char destFilePath[BUFFER_SIZE_64];
    //int sel_protocol_count;
    //int sel_scanned_protocol_count;
   // int status_protocol[10];
    int isScoutSelected;
    PDSliceParam_t selectedParam;
    PlaneAxis selectedAxis;
//     f32vec3_t v_slice;
//     f32vec3_t v_phase;
//     f32vec3_t v_offset;// 0 - varfield updated  -1 - Failed
    char studyfilename[BUFFER_SIZE_128];
    int sel_protocol_index_value[10];
  //  int sel_scanned_protocol_index_value[10];


} funcParams_t;

static char *ErrorList[] = {
         "FTP Transfer Complete",                      // 0
        "Config file not present.",                   // 1
        "Reading Config file Error.",                 // 2
        "FTP - Cannot Connect to remote machine",     // 3
        "FTP File Transfer Error",                    // 4
        "Data File not present",                        // 5
        "Could not open the study file",              //6
        "Error in Updating Varfield",                 //7
        "Sequence Updated",                           //8
        "Selected Data File is Invalid",                   //9
        "Please select an Unscanned Protocol",              //10
        "No unscanned protocols available",              //11
        "Could not open the study file",              //12
        "Comm srv init done",//13
        "Could not connect to server",//14
        "Socket writing failed",//15
        "Too few protocols to auto update",//16
        "No such a file present",//17
        "No such a slice plane present",//18
        "One of the parameter is null",//19
        "Failed to send message to PQM", //20
        "Please select Scanned Protocol",             //21
        "DICOM file creation failed",//22
        "Can not allocate memory.",//23
        "No Process/Echo/Image node found for DICOM file creation",//24
        "Exception thrown in function",//25
        "Unable to load Patient/Study/Series/Image",//26
        "Could not open config file", //27
        "Specified Section is not present in config file.",//28
        "Specified key or value for given key is not present in config file",//29
        "No scanned protocols available", //30
        "Calculation error",                //31
        "Data is not present.",             //32
        "Process is already running",        //33
        "Please select a SlicePlane"              //34

};
typedef enum {
  UNSCANNED, SCANNED, SCAN_FAIL
} ScanStatus;
typedef struct p_data_s
{
    int protocolNum;
    int protocol;
    ScanStatus isScanned;
} p_data_t;

//Added..vgp..2012/6/20
typedef enum MSG_OPTIONS{
    ERROR_LOGONLY,
    ERRORVIEW_DISPLAY,
    ERRORDIALOG_DISPLAY,
    CUSTOM_ERRORDIALOG_DISPLAY
} Msg_Option;

#ifndef RELIMITUPDATEPARAMS_T 
#define RELIMITUPDATEPARAMS_T 
typedef struct relimitUpdateParams_s
{
    f32vec3_t   v_slice, v_phase, v_offset;
    f32vec3_t   m_vec1, m_vec2;
    int m_nSliceNum;
    flt32_t m_fSliceGap;
    flt32_t m_fscliceThck;
    f32vec2_t m_fov;
    int m_nScannedProtIdx;
    int m_nCenterSliceNum;
    int m_nProcIdx;
    int m_nMpr_Voi_flag;
    int m_nflag_scout_selection;
    float m_fcouchOffset;    
} relimitUpdateParams_t;
#endif
#ifdef __cplusplus
const COLORREF BKCOLOR = RGB(27,36,54);
const COLORREF TEXT_BKCOLOR = RGB(191,209,255);
const COLORREF DISABLED_LIST_COLOR = RGB(70,74,81);
const COLORREF ENABLED_LIST_COLOR = RGB(102,139,200);
const int FONT_SIZE = 12;
const int E_SUCCESS = 0;
#endif
#define TEMPFILEPATH  "/usr/tmp/PDTOOL.tmp_file"
#define SMARTMPR_FILE_PATH "C:/gp/data/PlanDuplicator/SMPR_PLNdata.txt"
#define SMARTVOI_FILE_PATH "C:/gp/data/PlanDuplicator/SVOI_PLNdata.txt"
#define SMARTMPR_EXE_PATH "C:/gp/bin/SmartMPR2.exe"
#define DEFAULT_CONFIG_FILE L"C:/MRMPlus/config/MRConfig/PDTool/PD_Config.ini"
#define SMARTVOI_EXE_PATH "C:/gp/bin/SmartVOI.exe"
#define SMARTMPR_DICOM_PATH "C:/gp/data/PlanDuplicator/Dicom/"
#define SMARTVOI_DICOM_PATH "C:/gp/data/PlanDuplicator/DICOM_SCOUT/"
#define SMART_MPR_EXE "SmartMPR2.exe"
#define SMART_VOI_EXE "SmartVoi.exe"
#define GP_DATA "GP_DATA"
#define PD_FOLDER "/PlanDuplicator/"
#define SELECTION_0 0
#define SELECTION_1 1
#define SELECTION_2 2
#endif // !defined(AFX_PD_DATATYPES_H__C94B9E67_ECDF_4742_B72E_4094BD31D931__INCLUDED_)

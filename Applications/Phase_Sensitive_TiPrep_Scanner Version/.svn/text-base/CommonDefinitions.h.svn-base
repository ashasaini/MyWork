// CommonDefinitions.h: common data is stored in this file
/**
* Used for receiving the Parent GL Name from the page.
*/
#if !defined(AFX_COMMONDEFINITIONS_H__9CAC680D_A9AA_49CE_9342_6354621BD4EA__INCLUDED_)
#define AFX_COMMONDEFINITIONS_H__9CAC680D_A9AA_49CE_9342_6354621BD4EA__INCLUDED_

// This structure is used for receiving the Parent GL Name
// from the page.
typedef struct  tagINFO {
    TCHAR csPageName[1024];
} GLInfo;

/**
* Used to define message used for CFW communication.
*/
typedef enum {
    //FW_Error                        = -1,
    FW_CheckState                 = 1,
    FW_Close                        = 2,
    RE_FW_ReportState             = 3,
    FW_Show                     = 4,
    FW_Hide                     = 5,
    RE_FW_CheckState                = 6,
    RE_FW_Close                 = 7,
    FW_ReportState              = 8,
    RE_FW_Show                  = 9,
    RE_FW_Hide                  = 10,
   // FW_ERR_UnknownCommandReceived = 11,
   // RE_FW_ERR_UnknownCommandReceived = 12,
    FW_RequestClose             = 13,
    FW_RequestHideGL                = 14,
    FW_RequestShowGL                = 15,
    RE_FW_RequestHideGL         = 16,
  //  RE_FW_RequestShowGL         = 17,
    FW_RegisterWindowZOrder     = 18,
    FW_ChangeWindowZOrder           = 19,
    FW_UnregisterWindowZOrder       = 20,
    FW_AddZOrderLayer               = 21,
   // FW_GetAllZOrderLayer            = 22,
   // RE_FW_GetAllZOrderLayer     = 23,
   // FW_GetAllWindowInLayer      = 24,
  //  RE_FW_GetAllWindowInLayer       = 25,
  //  GL_BlockDesign_RequestValue = 26,
   // RE_GL_BlockDesign_RequestValue = 27,
   // GL_BlockDesign_Update           = 28,
    //RE_GL_BlockDesign_Update        = 29,
   // GL_BlockDesign_SetDynamicPara = 30,
   // RE_GL_BlockDesign_SetDynamicPara = 31,
  //  FW_ShowInScanPreferenceDialog  = 32,
  //  RE_FW_ShowInScanPreferenceDialog = 33,
  //  GL_TensorParamSet_RequestValue = 34,
 //   RE_GL_TensorParamSet_RequestValue = 35,
 //   GL_TensorParamSet_Update = 36,
  //  RE_GL_TensorParamSet_Update = 37,
    GL_Tool_Show                = 38,
    RE_GL_Tool_Show             = 39,
    GL_Tool_Hide                = 40,
    RE_GL_Tool_Hide             = 41,
    FW_RegisterMsg              = 42,
    RE_FW_RegisterMsg           = 43,
    GL_IS_Send_CONTROLLER_IC_Name = 44,
    GL_IS_Get_Selected_IC_Name  = 45,
   // RE_GL_IS_Get_Selected_IC_Name = 46,
    FW_UnRegisterMsg              = 47,
    RE_FW_UnRegisterMsg            = 48,
    FW_Disable_Process_Btn           = 49,
    RE_FW_EnableButton              = 50,
    GL_IS_Get_Select_Series_IC_Name = 51,   
    GL_IS_Refresh_Image_Matrix      = 52  
} tiprep_message_t;

/**
* Used to define status of TiPrep Application.
*/
typedef enum {
    STATE_READY = 3,
    STATE_ACTIVE = 4,
    STATE_INACTIVE = 5
} MessageState;

/**
* Used to store ROI related CFW message information.
*/
struct ROI_CMD {
    CString csSender;
    CString csReceiver;
    CString csCommand;
    CString csParameter;
    _bstr_t Custom;
};
/**
* Used to specify data form : real, imaginary or magnitude.
*/
typedef enum  {
    MAGNITUDE = 0,
    PHASE,  //1
    REAL, //2
    IMGN //3

} eDataForm;
/**
* Used to specify options INROI or ALLIMG in the ini file.
*/
typedef enum {
    INROI = 1,
    ALLIMG
} proc_option;
typedef enum {
    ROIAVG = 1,
    ALLIMG_p
} phase_option;

/**
* Used to identify which select event caused iselector response.
*/

typedef  enum {
    EVENT_INIT = 0,
    EVENT_RESELECT,
    EVENT_MGSEL,
    EVENT_AUTOPHASE,
    EVENT_POLARITY_CORRECTION, 
    EVENT_MG_SELECT,
    EVENT_RE_SELECT,
    EVENT_IMG_SELECT,
    EVENT_ADD_OFFSET,
    EVENT_DATA_TYPE,
    EVENT_RECON_IMG
};

typedef enum {
    I_NO_ERROR = 0,
    E_COMON_ERROR,  // 1
    E_MIN_INTENSITY,    // 2
    I_VAL_OFFSET,       // 3
    I_PROPER_IMG,       // 4
    I_REAL_IMG,     // 5
    I_MAG_IMG,      // 6
    I_IMGRY_IMG,        // 7
    E_IMG_SERIES,       // 8
    I_FEW_IMG,      // 9
    I_ROI_OUT_RGN,  //10
    I_ROI_INFO,     // 11
    E_NO_RAW,           // 12
    I_VALID_TR,     // 13
    E_VF_OPEN,      // 14
    E_Vf_GET_ERR,       // 15
    E_GDC_DIFF,     // 17
    E_NUM_DIFF,     // 18
    E_PROT_DIFF,        // 19
    I_FLOAT_VAL,        // 20
    I_INT_VAL,          // 21
    E_NO_REAL,      // 22
    E_NO_IMG,           // 23
    E_NO_MAG,           //24
    E_FILE_TRANSFER,    // 25
    E_RECON_FAIL,       // 26
    E_FILE_WRITE,       // 27
    E_NO_ROI_SELECTED, //28
    E_MEM_ALLOC,      //29
    E_SELECT_SINGLE_SERIES, //30
    E_NO_DATA_FOUND,            //31
    E_SELECT_ALL_SERIES,        //32
    E_FITTING_ERROR,         //33
    E_DATAPOINTS_ERROR,         //34
    E_NO_ROI_SEL_INROI,        //35
    E_ERROR_NULL_FUNCPARAMS ,      //36
    E_VARFIELD_DATA_TYPE_ERROR, // 37
    E_OFFSET_RANGE_ERROR,   //38
    E_APPLY_IMAGE_NOT_PROPER, //39
    E_SAME_PROTOCOL_SERIES,    //40
    E_AVG_INTENSITY ,    //41
    E_ROI_DISPLAY_ERROR, //42
    E_ZERO_SIZE_ERROR, //43
    E_SELECT_TARGET_PROTOCOL, //44
    E_NO_MAPFILE ,//45
    E_MAPFILE_DIM_ERROR, //46 /KJ
    E_MAP_ROI_NO_DATA ,//47 /AN
    E_SELECT_VALID_MAP_POINT, //48
    E_NO_DATA_FOUND_MAP,//49
    E_ERROR_FILTER_INVALID, //50
    E_SMALL_FILTER_VAL,//51
    E_ROI_RADIUS_TOO_LARGE,//52
    E_WRITE_T1MAP_TO_STUDY, //53
    E_CHECK_T1MAP_IF_PRESENT,   //54
    E_VALID_INTIAL_GUESS_VALUE, //55
    E_ENTER_INITIAL_GUESS,//56
    E_ROI_MAP,   //57
    E_IMG_DATA_FAIL, //58
    E_IMG_SCALE_FAIL,//59
    E_IMG_TIME_STAMP_FAIL,//60
	E_IMG_TAG_FAIL,//61
	E_IMG_TAG_PROT_FAIL, //62
	E_SERIES_NO_FAIL,// 63
    E_DB_ERROR, //64
	E_DB_UPDATE_ERROR,//65
	E_ERR_IMG_LOAD_FAIL, //66
    E_SOCKET_ERR, //67
    E_ERR_EXCEPTION , //68
    E_SERIES_NOT_FOUND_IN_LIST,//69
    E_GETTING_PROTOCOL_NUM_AT_SERIES_LEVEL,//70
    E_COPY_ROI_FUNCTIONALITY_FAILED,//71
    E_GETTING_SCANNED_PROTOCOL_LIST_FAILED,//72
	E_VALIDATION_FAILED,//73
    E_PHASE_CORRECTION, //74
    E_VFC_FILE_NOT_FOUND,//75
    E_VF_UPDATE_FAIL, //76
    E_RAW_CHECK_FAIL, //77
    E_LAST_VAL  
} ;

typedef enum {
	E_NO_CONNECTION,
	E_START_CONNECTION,
	E_CONNECTED,
	E_CLOSE_CONNECTION
} ePQM_CONNECT_STATE;

/**
* Represents the number of ROI to be drawn in TiPrep Application.
*/
#define NUMBEROFROI 5
#define DOMAIN_TIPREP     _T("MRI_TIPREP")
#define TIPREP_TRACE_CSSDK _T("TIPREP_TRACE_CSSDK")
#define TIPREP_ERROR_CSSDK _T("TIPREP_ERROR_CSSDK")
#define TIPREP_TRACE_FILE_PATH _T("TIPREP_TRACE_FILE_PATH")
#define TIPREP_ERROR_FILE_PATH _T("TIPREP_ERROR_FILE_PATH")
#define TIPREP_TRACE_LEVEL _T("TraceLevelTiPrep")
#define TIPREP_MARKER _T("MARKER")
#define MRMP_CONFIG     "MRMP_Config"
#define MRMP_LOGDIR     "MRMP_LOGDIR"
#define NUMBER_BASE 16
#define TIPREP_INI_FILE _T("\\TiPrep.ini")
#define TIPREP_TRACECONFIG_INI_FILE _T("\\TiPrepTraceConfig.ini")
#define TIPREP_ERROR_LOG_FILE _T("TiPrepErrorLog.txt")
#define TIPREP_TRACE_LOG_FILE _T("TiPrepTraceLog.txt")
#define T1MAP_EXE_PATH "C:/SharedMPlus/MR/bin/release/T1MapViewer.exe"
#define T1MAP_EXE_PATH_PRODUCT "C:/SharedMPlus/MR/bin/T1MapViewer.exe"
#define T1MAP_EXE_NAME "T1MapViewer.exe"
#define RAWCHECK_STATUS_FILEPATH "C:\\tmp\\rawCheckStatus.txt"

/**
* Represents the offset between the 3 ROI's.
*/
#define ROIOFFSET 10
enum ERROR_CODE {
    
};
enum DIALOG_ID {
    TIPREP_DLG = 0,
    POLARITY_CORR_DLG = 1,
    ADD_OFFSET_DLG = 2,
    COPY_ROI_DLG = 3,
    SET_ROI_DLG = 4,
    RECON_DLG = 5,
    PHASE_CORRECTION_DLG = 6
};
typedef struct {
    int     protocol[2];
    int     processed[2];
    int     echo[2];
    int     image[2];
} ImagNode_t;
typedef struct imgProcessData_s {
    int dataform;
    int nImages;
    int imgGDCtype;
    int numNode;
    ImagNode_t  *convPolImgs; 
} imgProcessData_t;

/**
* Used to define status of TiPrep Application.
*/
typedef enum {
    ROI_1 = 0,
    ROI_2,
    ROI_3,
    NOISE_ROI_1,
    NOISE_ROI_2
} ROI_Index;

typedef enum {
    ALL_APP = 1,
    TIPREP_ONLY
} App_Options;

const int ROICNT = 3;
//CONST CString COLUMN_HEADER1(_T(",,ROI1,,,ROI2,,,ROI3\n"));
//CONST CString COLUMN_HEADER2(_T("Img#,Original,Inverted,CurveFit,Original,Inverted,CurveFit,Original,Inverted,CurveFit\n"));
CONST CString COLUMN_HEADER1_MAPMODE(_T("At Point [X,Y]:"));
CONST CString COLUMN_HEADER2_MAPMODE(_T("Img#,Original,Inverted,CurveFit\n"));
#define MAX_NUM_ROI 3   //Number of regular ROIs on image
#define MAX_NOISE_ROI 2 //Number of Noise ROIs on image 
#define MAX_LINE_ROI (MAX_NUM_ROI+MAX_NOISE_ROI)    //Total number of ROIs i.e Regular ROI and Noise ROI

#define T1BACK_FILEPATH _T("c:/MrMplus/Data/TiPrep/T1Back")
#define T1MAP_FILEPATH _T("c:/MrMplus/Data/TiPrep/T1Map")
#define T1MINMAP_FILEPATH _T("c:/MrMplus/Data/TiPrep/T1MinMap")
//vilas_31Oct..to avoid macros, use template function 
template <typename T>
void ReleaseTiMemory(T * & inptr){
    if(NULL != inptr){
        delete inptr;
        inptr = NULL;
    }
}

#define E_IMPROPER_INDEX -1
#endif // !defined(AFX_COMMONDEFINITIONS_H__9CAC680D_A9AA_49CE_9342_6354621BD4EA__INCLUDED_)
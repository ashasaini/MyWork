/******************************************************************************
 *  
 *    File Overview:
 *      This file defines routines for...
 *
 *    Objects with external linkage defined in this file:
 *      
 *
 *****************************************************************************/
/* Revision History:
 * yyyy-mm-dd     modified-by     modifications-done
 * 2001-09-24     Y.Watanabe      Created.
 * End of History */



#ifndef msGetConfig_H__
#define msGetConfig_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <tami/imgsrv/ImgsrvLib.h>

#define CF_FILE       "mathServer.cf"
#define ETC_PATH_ENV  "GP_ETC_PATH"
#define TMP_PATH_ENV  "GP_TMP_DIR"
#define MPLUS_ENV     "MPLUSHOME"

#define MISC          "%MISC%"
#define RAWSRV        "%RAWSRV%"
#define RECON         "%RECON%"
#define IMGSRV        "%IMGSRV%"
#define IMGREG        "%IMGREG%"
#define IMGREGDB      "%IMGREGDB%"

#define MAXMEMSIZE        "MaxMemSize"
#define MAXJOB            "MaxJob" 
#define CURRENT_MODE      "CurrentMode"
#define CONSOLE           "Console"
#define ENGINE            "Engine"
#define AP		          "AP"
#define MS_ENGINE_NONE    "NONE"
#define MS_ENGINE_CONNECT "CONNECT"
#define MS_AP_NONE        "NONE"
#define MS_AP_CONNECT     "CONNECT"

#define CONSOLE_SYSTEM    "ConsoleSystem"
#define MS_SYSTEM_MPLUS   "MPlus"
#define MS_SYSTEM_GP      "gp"

#define RECON_AP          "ReconAP"
#define PHYSICAL_CPU      "PhysicalCPU"
#define FFT_PROC          "FFTProc"
#define MAX_CH            "MaxCh"
#define CH_PER_FFT_PROC   "ChPerFFTProc"
#define RESLICE_PROC      "ResliceProc"

#define CONSOLE_HOST_NAME        "ConsoleHostName"
#define CONSOLE_HOST_ADDRESS     "ConsoleHostAddress"
#define CONSOLE_NETWORK_NAME     "ConsoleNetworkName"
#define CONSOLE_NETWORK_ADDRESS  "ConsoleNetworkAddress"
#define CONSOLE_MOUNT_TOP_PATH   "ConsoleMountTopPath"
#define CONSOLE_MOUNT_STUDY_PATH "ConsoleMountStudyPath"

#define ENGINE_HOST_NAME        "EngineHostName"
#define ENGINE_HOST_ADDRESS     "EngineHostAddress"
#define ENGINE_NETWORK_NAME     "EngineNetworkName"
#define ENGINE_NETWORK_ADDRESS  "EngineNetworkAddress"

#define ENGINE_DATA_NAME1       "EngineDataName1"
#define ENGINE_DATA_ADDRESS1    "EngineDataAddress1"
#define ENGINE_DATA_NAME2       "EngineDataName2"
#define ENGINE_DATA_ADDRESS2    "EngineDataAddress2"

#define AP_HOST_NAME1           "APHostName1"
#define AP_HOST_ADDRESS1        "APHostAddress1"
#define AP_DATA_NAME1           "APDataName1"
#define AP_DATA_ADDRESS1        "APDataAddress1"
#define AP_HOST_NAME2           "APHostName2"
#define AP_HOST_ADDRESS2        "APHostAddress2"
#define AP_DATA_NAME2           "APDataName2"
#define AP_DATA_ADDRESS2        "APDataAddress2"

/* V9.01*R245 Add */
#define AP_HOST1				"APHost1"
#define AP_DATA1				"APData1"
#define AP_HOST2				"APHost2"
#define AP_DATA2				"APData2"

#define ENGINE_MOUNT_TOP_PATH   "EngineMountTopPath"
#define ENGINE_MOUNT_STUDY_PATH "EngineMountStudyPath"

#define PNR_SAVE_STUDY "PnRSaveStudy"

#define MS_QUEUE_2D     "Queue2D"
#define MS_QUEUE_3D     "Queue3D"
#define MS_QUEUE_REREC  "QueueRerec"
#define MS_QUEUE_DIRECT "QueueDirect"

#define MS_RAWDATA_MODE     "RawDataMode"
#define MS_RAWDATA_STUDY    "Study"
#define MS_RAWDATA_MEMORY   "Memory"
#define MS_RAWDATA_SEQUENCE "Sequence"

#define MS_NUM_CPU "NumCPU"

#define SHM_SIZE "ShmSize"
#define LOC_MEM_SIZE "LocMemSize"
#define PNR_MEM_SIZE "PnRMemSize"
#define INIT_SHM_SIZE "InitShmSize"

#define INTER_DISK		"IntermediateDisk"
#define INTER_MEM_SIZE	"IntermediateMemSize"
#define RESL_DISK		"ResliceDisk"
#define RESL_MEM_SIZE	"ResliceMemSize"

/* 2010.12.03 for PARALLEL */
#define MS_PRIORITY_2D     "Priority2D"
#define MS_PRIORITY_3D     "Priority3D"
#define MS_PRIORITY_REREC  "PriorityRerec"
#define MS_PRIORITY_DIRECT "PriorityDirect"

#define IMG_DATA_TYPE "ImgDataType"
#define IMG_DATA_FORM "ImgDataForm"
#define IMG_SLICE     "ImgSlice"
#define LOC_DATA_TYPE "LocDataType"
#define LOC_DATA_FORM "LocDataForm"
#define LOC_MATRIX_X  "LocMatrixX"
#define LOC_MATRIX_Y  "LocMatrixY"
#define LOC_SLICE     "LocSlice"
#define STABLE_MAX_SLICE "StableMaxSlice"

#define ESCAPE  '\\'
#define MS_CHAR_SIZE 128

#ifdef __WNP_MPLUS__
enum {
	CONFIG_TYPE_CONSOLE = 0,
	CONFIG_TYPE_ENGINE,
	CONFIG_TYPE_AP,
} ;
#endif

typedef struct msGetConfig_s{
  char engine[MS_CHAR_SIZE];
  int  engineMode;			/* 0 -- NONE,    1 -- CONNECT */
  char ap[MS_CHAR_SIZE];	/* V9.01 Add */
  int  apMode;				/* 0 -- NONE,    1 -- CONNECT */
  char current[MS_CHAR_SIZE];
  int  currentMode;			/* 0 -- console, 1 -- engine, 2 -- ap */
  char console[MS_CHAR_SIZE];
  int  consoleSystem;		/* 0 -- MPlus, 1 -- gp */

  int reconAP;				/* V9.01 Add */
  int physicalCPU;			/* V9.01 Add */
  int fftProc;				/* V9.01 Add */
  int maxCh;				/* V9.01 Add */
  int chPerFftProc;			/* V9.01 Add */
  int resliceProc;			/* V9.01 Add */

  char consoleHostName[MS_CHAR_SIZE];
  char consoleHostAddress[MS_CHAR_SIZE];
  char consoleNetworkName[MS_CHAR_SIZE];
  char consoleNetworkAddress[MS_CHAR_SIZE];
  char consoleMountTopPath[MS_CHAR_SIZE];
  char consoleMountStudyPath[MS_CHAR_SIZE];

  char engineHostName[MS_CHAR_SIZE];
  char engineHostAddress[MS_CHAR_SIZE];
  char engineNetworkName[MS_CHAR_SIZE];
  char engineNetworkAddress[MS_CHAR_SIZE];
  char engineDataName1[MS_CHAR_SIZE];		/* V9.01 Add */
  char engineDataAddress1[MS_CHAR_SIZE];
  char engineDataName2[MS_CHAR_SIZE];
  char engineDataAddress2[MS_CHAR_SIZE];

  char engineMountTopPath[MS_CHAR_SIZE];
  char engineMountStudyPath[MS_CHAR_SIZE];

  char APHostName1[MS_CHAR_SIZE];			/* V9.01 Add */
  char APHostAddress1[MS_CHAR_SIZE];
  char APDataName1[MS_CHAR_SIZE];
  char APDataAddress1[MS_CHAR_SIZE];
  char APHostName2[MS_CHAR_SIZE];
  char APHostAddress2[MS_CHAR_SIZE];
  char APDataName2[MS_CHAR_SIZE];
  char APDataAddress2[MS_CHAR_SIZE];

/* V9.01*R245 Add */
  char APHost1[MS_CHAR_SIZE];
  char APData1[MS_CHAR_SIZE];
  char APHost2[MS_CHAR_SIZE];
  char APData2[MS_CHAR_SIZE];

  char PnRSaveStudy[MS_CHAR_SIZE];

  int queue2D;
  int queue3D;
  int queueRerec;
  int queueDirect;

  char rawData[MS_CHAR_SIZE];
  int  rawDataMode; /* 0 -- study, 1 -- memory, 2 -- sequence */

  int numCpu;

  struct rawsrv_s{
    int maxMemSize;
    int shmSize;					/* V9.01 Add */
    int locatorMemSize;
    int PnRmemSize;
	int initShmSize;				/* MPlus Add */
  }rawsrv;
  struct recon_s{
    int maxMemSize;
    char intermediateDisk[MS_CHAR_SIZE];	/* V9.01 Add */
    int intermediateMemSize;			/* V9.01 Add */
    char resliceDisk[MS_CHAR_SIZE];		/* V9.01 Add */
    int resliceMemSize;			/* V9.01 Add */
    /* 2010.12.03 for PARALLEL */
    int priority2D;
    int priority3D;
    int priorityRerec;
    int priorityDirect;
  }recon;
  struct imgsrv_s{
    int maxMemSize;
  }imgsrv;
  struct imgreg_s{
    int maxMemSize;

    imgsrv_const_type_t imgDataType;
    imgsrv_const_form_t imgDataForm;
    int                 imgSlice;

    imgsrv_const_type_t locDataType;
    imgsrv_const_form_t locDataForm;
    int                 locMatrixX;
    int                 locMatrixY;
    int                 locSlice;
    int                 stableMaxSlice;
  }imgreg;
  struct imgregDB_s{
    int maxJob;
  }imgregDB;
}msGetConfig_t, *msGetConfig_p;

void msGetConfig(msGetConfig_p  config);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* msGetConfig_H__ */


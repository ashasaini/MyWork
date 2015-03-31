// This structure is used for receiving the Parent GL Name 
// from the page.
#if !defined(AFX_KTCOMMONDEF__INCLUDED_)
#define AFX_KTCOMMONDEF__INCLUDED_

typedef struct  tagINFO
{
    TCHAR csPageName[1024];
}GLInfo;

typedef enum {
    FW_Error                        = -1,
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
    FW_ERR_UnknownCommandReceived = 11,
    RE_FW_ERR_UnknownCommandReceived = 12,
    FW_RequestClose             = 13,
    FW_RequestHideGL                = 14,
    FW_RequestShowGL                = 15,
    RE_FW_RequestHideGL         = 16,
    RE_FW_RequestShowGL         = 17,
    FW_RegisterWindowZOrder     = 18,
    FW_ChangeWindowZOrder           = 19,
    FW_UnregisterWindowZOrder       = 20,
    FW_AddZOrderLayer               = 21,
    FW_GetAllZOrderLayer            = 22,
    RE_FW_GetAllZOrderLayer     = 23,
    FW_GetAllWindowInLayer      = 24,
    RE_FW_GetAllWindowInLayer       = 25,
    GL_BlockDesign_RequestValue = 26,
    RE_GL_BlockDesign_RequestValue= 27,
    GL_BlockDesign_Update           = 28,
    RE_GL_BlockDesign_Update        = 29,
    GL_BlockDesign_SetDynamicPara = 30,
    RE_GL_BlockDesign_SetDynamicPara=31,
    FW_ShowInScanPreferenceDialog  = 32,
    RE_FW_ShowInScanPreferenceDialog = 33,
    GL_TensorParamSet_RequestValue = 34,
    RE_GL_TensorParamSet_RequestValue = 35,
    GL_TensorParamSet_Update = 36,
    RE_GL_TensorParamSet_Update = 37,
    GL_IS_Get_Selected_IC_Name = 38,
} tiprep_message_t;


typedef enum {
    STATE_READY = 3,
    STATE_ACTIVE = 4,
    STATE_INACTIVE = 5
} MessageState;
#define true 1
#define false 0

#define RAWCOPY_EXEC_PATH "c:/gp/bin/sh rawCopy"
#define VROMPROC_EXEC_PATH "c:/usr/local/bin/VROMProcessor_kt.exe"
#define RECONVIEWER_EXEC_PATH "c:/usr/local/bin/ktReconViewer.exe"

#define RAWCOPY_EXEC_NAME "rawCopy"
#define VROMPROC_EXEC_NAME "VROMProcessor_kt.exe"
#define RECONVIEWER_EXEC_NAME "ktReconViewer.exe"

#define NUMBER_OF_PROCESSORS_DEFAULT 1

#define SITE    "SITE"
#define STUDY   "/study/"
#define DEFAULT_SITE "/gp/sites/site5000"
#define NO_OF_THREADS "NUMBER_OF_PROCESSORS"
#define MAX_NO_OF_THREADS 2

#define RAWCOPY_INIT 0
#define VROMPROCESSOR_INIT 1
#define KTRECONVIEWER_INIT 2

#define RAWCOPY_EXIT "KT_RAWCOPY_EXIT"
#define VROMPROCESSOR_EXIT "KT_VROMPROCESSOR_EXIT"
#define KTRECONVIEWER_EXIT "KT_RECONVIEWER_EXIT"
#define DEFAULT_EXIT "KT_DEFAULT_EXIT"
#define SETNORMAL_CURSOR "KT_SETNORMAL_CURSOR"

#define MRMP_CONFIG     "MRMP_Config"
#define KTRECON_INI_FILE _T("\\KtRecon.ini")
#define KTRECON_TRACECONFIG_INI_FILE _T("\\KtReconTraceConfig.ini")
#define KTRECON_ERROR_LOG_FILE _T("KtReconErrorLog.txt")
#define KTRECON_TRACE_LOG_FILE _T("KtReconTraceLog.txt")

#define KTRECON_TRACE_CSSDK _T("KTRECON_TRACE_CSSDK")
#define KTRECON_ERROR_CSSDK _T("KTRECON_ERROR_CSSDK")
#define KTRECON_TRACE_FILE_PATH _T("KTRECON_TRACE_FILE_PATH")
#define KTRECON_ERROR_FILE_PATH _T("KTRECON_ERROR_FILE_PATH")
#define KTRECON_TRACE_LEVEL _T("TraceLevelKtRecon")
#define KTRECON_MARKER _T("MARKER")
#define NUMBER_BASE 16

#define MEMFREE(ptr) if (ptr){ \
    free(ptr); \
    ptr = NULL;\
}

#define BUFFER_SIZE_128 128
#define BUFFER_SIZE_256 256
#endif
/////////////////////////////////END OF FILE /////////////////////////////////////
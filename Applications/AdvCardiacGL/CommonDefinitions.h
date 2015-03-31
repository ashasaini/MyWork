
#ifndef _COMMONDEFINITIONS_H
#define _COMMONDEFINITIONS_H
#include <afxcoll.h>
#include "Shlwapi.h"
#include <atlbase.h>
#include <atlconv.h>
#include <string>
#include <vector>
#include "ACGLTrace.h"
#include "ACGLLogHelper.h"
using namespace std;
//#pragma warning(disable:4786)
typedef struct  tagINFO
{
    TCHAR csPageName[1024];
}GLInfo;

#define WM_INIT_TOOL         (WM_USER + 100)
#define WM_CMD_CLOSE            (WM_USER + 101)
#define WM_CMD_SHOW             (WM_USER + 102)
#define WM_CMD_HIDE             (WM_USER + 103)

typedef enum {
    STATE_READY = 3,
        STATE_ACTIVE = 4,
        STATE_INACTIVE = 5
} MessageState;

#define TOOL_NAME _T("KtRecon")
#define PAGER _T("Display1")
#define SEP _T(".") 

typedef enum{
	Toolbox = 0,
		Process =1
}  SCENE;

#define SCENE_TOOLBOX	(_T("Film/ToolBox"))
#define SCENE_PROCESS	(_T("Process/Master"))

#define E_NO_ERROR 0
#define E_ERROR 1

#define MRMP_CONFIG     "MRMP_Config"

typedef std::pair<std::string, std::string> KeyValuePair;
    typedef std::vector<KeyValuePair>::size_type size_type;

// char *Vantage_Tools_Icon_FileName[] = {
//     "l_tiprep.ico",     //0
//     "l_ktrecon.ico",    //1
//     "l_reslice.ico",    //2
// };
#define APP_NAME _T("AdvCardiacGL")

#define NUMBER_OF_TOOLS 15

#define ACGL_INI_FILE _T("\\AdvCardiacGL.ini")
#define ACGL_TRACE_CSSDK _T("ACGL_TRACE_CSSDK")
#define ACGL_ERROR_CSSDK _T("ACGL_ERROR_CSSDK")
#define ACGL_TRACE_FILE_PATH _T("ACGL_TRACE_FILE_PATH")
#define ACGL_ERROR_FILE_PATH _T("ACGL_ERROR_FILE_PATH")
#define ACGL_TRACE_LEVEL _T("TraceLevelACGL")
#define ACGL_MARKER _T("MARKER")
#define MRMP_CONFIG     "MRMP_Config"
#define MRMP_LOGDIR     "MRMP_LOGDIR"
#define NUMBER_BASE 16
#define ACGL_TRACECONFIG_INI_FILE _T("\\ACGLTraceConfig.ini")
#define ACGL_ERROR_LOG_FILE _T("ACGLErrorLog.txt")
#define ACGL_TRACE_LOG_FILE _T("ACGLTraceLog.txt")

#endif
///////////////////////////////// END OF FILE /////////////////////////////////
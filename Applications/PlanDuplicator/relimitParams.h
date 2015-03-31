#ifndef _RELIMITPARAMS_H
#define _RELIMITPARAMS_H

#include <windows.h>
#undef LANG_ENGLISH
#undef LANG_JAPANESE
#undef NO_ERROR

#include <tami/libdll/dll.h>
#include <tami/libWNP/WinNativePorting.h>
#include "dataTypes.h"

#include "C:\MRMPlus\include\tami\common\basicdefs.h"

#define TRACE_FUNC_ENTER( func_name )          \
TraceFuncEnter( func_name );

#define TRACE_FUNC_EXIT( func_name )          \
TraceFuncExit( func_name );


typedef void *PVOID;
typedef PVOID HANDLE;

#define BUFF_LONG_LEN	1024


typedef struct relimitParams_s
{
//    char *studyFileName;
//    int  sourceProtocolIndex;
    /* Socket parameters */
    int32_t		socketPort;
    int32_t		socketId;
    int32_t		socketFd;
    HANDLE		pHandle;
    /**/
    int32_t		dirSegShmid;
    pid_t		pid;
    int32_t		wrfd;
    int32_t		rdfd;
    int32_t		recreate_hot_line;
    char		scratchPad[BUFFER_SIZE_128];
    char		seqgenName[BUFFER_SIZE_128];
    char        linkCommand[BUFFER_SIZE_128];
    char        fepCommand[BUFFER_SIZE_128];
    char        studyFile[BUFFER_SIZE_256];
    int         protIndex;
    f32vec3_t   v_slice, v_phase, v_offset, v_vec1, v_vec2;
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

} relimitParams_t;


#define PROCON_SERVICE_NAME_BASE	"proClient"
#define PROCON_SERVICE_NAME_SUFFIX	"2"
#define PROCON_SERVICE_NAME 		PROCON_SERVICE_NAME_BASE  PROCON_SERVICE_NAME_SUFFIX
#define SERVICE_NAME_MAX	30

/* mutex ロックまでの待ち時間(ms) */
#define MUTEX_WAIT_TIME		0

/* serviceに使用ポートのリストが記述されている場合は Imakefileで
   USE_SERVICES_LIST を有効にする。
     proClient2 1803,proClient3 1804,...
   使用ポートの先頭だけが記述されている場合は無効にする。
     proClient2 1803
   これらの場合の使用最大ポート数は SERVICE_PORT_MAX で定義されている。
#define USE_SERVICES_LIST
#define SERVICE_PORT_MAX   2
*/
#define SERVICE_PORT_MAX	5
#define RETRY_COUNT		5
/* prolim開始待ち時間(s) */
#define PL_STARTUP_WAIT_TIME	10
/* prolim終了待ち時間(ms) */
#define PL_TERMINATE_WAIT_TIME	5000
/* ProMgrExecリトライ回数 */
#define PL_RETRY_COUNT	3
/* ProMgrExec起動リトライ待ち時間 */
#define PL_SLEEP_TIME	3


typedef struct lockList_s
{
    char     serviceName[SERVICE_NAME_MAX];
    int	     portNum;
    HANDLE   hMutex;
} lockList_t;

#endif

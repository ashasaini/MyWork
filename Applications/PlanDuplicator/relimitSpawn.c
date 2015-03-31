#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>

#include <tami/common/basicdefs.h>
#include <tami/common/globalSwitches.h>
#include <tami/libvf/varfields.h>
#include <tami/vfStudy/vfs_structure.h>
#include <tami/vfStudy/vfs_plan.h>
#include <tami/libTl/Tl.h>
#include <tami/libseq/seq.h>
#include <tami/libproMsg/proMsg.h>
#include "RelimitLogHelper.h"
#include "relimitParams.h"
//#include "dataTypes.h"

extern status_t relimitTerminateProcess(void);
extern status_t relimitDone(void);
extern relimitParams_t	relimitG;
extern status_t StartupSync(void);
extern status_t WaitForMsgfromProlim(void);
char *relimitExec(char *scratchPad, char *sequenceName, pid_t *pid_out);


/*************************************************************
* Method:    	relimitExec
* Description: 	This function is used 
* Parameter: 	char * scratchPad :
* Parameter: 	char * sequenceName :
* Parameter: 	pid_t * pid_out :
* Returns:   	char *
*************************************************************/
char *relimitExec(char *scratchPad,
                  char *sequenceName,
                  pid_t *pid_out)
{
    char                command[BUFF_LONG_LEN]={0};
    char                invocation[BUFF_LONG_LEN]={0};
    PROCESS_INFORMATION pi;
    STARTUPINFO         si;
    int                 retry = PL_RETRY_COUNT;
    
    TRACE_FUNC_ENTER("relimitExec() : ");
    
    //printf("\n seqnam %s \t scratpad  %s\n", scratchPad, sequenceName);
    fflush(stdout);
    seqioSeqDir(invocation, sequenceName);  /* in libseq.a */
    strcat(invocation, "/plSrc/prolim.exe");

    //strcpy(invocation,"C:\\gp\\seq\\sequences\\FE_crt\\plSrc\\prolim");

    /*CString tmp;
    tmp.Format(_T("%s %s %d %s %s %s %s %s %d %s"),
            CString(invocation),
            "-sock", relimitG.socketPort,
            GSW_PROCON_SCRATCHPAD, scratchPad,
            GSW_STUDY, relimitG.studyFile,
            GSW_PROTOCOL, relimitG.protIndex,
            _T("-nl"));
    _tcscpy(command, tmp);*/
    sprintf(command, "%s %s %s %s %s %s %d %s %d %s",
        invocation,
        GSW_PROCON_SCRATCHPAD, scratchPad,
        GSW_STUDY, relimitG.studyFile,
        GSW_PROTOCOL, relimitG.protIndex,
        "-sock", relimitG.socketPort,"-nl");

    printf("\n ****invocation is %s", invocation);

    printf("\n command is %s",command);
    fflush(stdout);

    GetStartupInfo(&si);

    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    if (!CreateProcess(NULL, command, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
    {
        printf("E: CreateProcess() error count=%d (%s) error no=%d\n", retry, sequenceName, GetLastError());
        fflush(stdout);
        return(NULL);
    }//Vilas..July2012..Revw..code below can go into else block
    else
    {
        *pid_out  = pi.dwProcessId;
        relimitG.pHandle = pi.hProcess;
        if(E_NO_ERROR == StartupSync())
        {   
            if(E_NO_ERROR != WaitForMsgfromProlim())
            {
                return NULL;
            }
            return sequenceName;
        }
        //printf("E: ProMgrStartupSync() error count=%d (%s)\n", retry, sequenceName);
        fflush(stdout);
        relimitTerminateProcess();
     }
    TRACE_FUNC_EXIT("relimitExec() : ");
    return(NULL);
}


/*********************************************************************
 *
 *  (c) Copyright 1994 TOSHIBA Corporation All Rights Reserved
 *
 *    File Overview:
 *      This file defines routines for...
 *
 *    Objects with external linkage defined in this file:
 *
 *********************************************************************/
/* Revision History:
 * Oct.20.94	Naoyuki FURUDATE	Created
 * Nov.21.94	Naoyuki FURUDATE	Modified
 * Aug. 1.95	Naoyuki FURUDATE	Modified
 * End of History */


#include <winsock2.h>
#include <tami/libWNP/WinNativePorting.h>
#include <tami/common/basicdefs.h>
#include <tami/vfStudy/vfs_structure.h>
#include <tami/vfStudy/vfs_plan.h>
#include <tami/libvf/varfields.h>

#include <tami/libproMsg/proMsg.h>
#include <tami/proCon/proMsgCodes.h>
#include <tami/libplTree/plsftree.h>
#include <tami/libpl/plPar.h>
//#include <tami/libpl/plParDefs.h>

#if 0 /* already included in libpl/plTable.c */
#include <tami/libplTree/plstdtbl.h>
#else
#include <tami/libpl/pltbldec.h>
#endif

#include <tami/libpl/plTable.h>

#include "relimitMsg.h"
#include "relimitParams.h"
#include "plNameTableP.h"
#ifdef __NUTC__
#include <sys/types.h>
#include <string.h>
#include "RelimitLogHelper.h"

#define MAX_NO_RETRY 5
//extern status_t TerminateProlim(void);
status_t relimitTerminateProcess(void);
char *relimitExec(char *scratchPad, char *sequenceName, pid_t *pid_out);
status_t SetupProlim(void);
status_t RelimitProlim(void);
status_t SaveProlim(void);
status_t TerminateProlim(void);
int32_t relimMakeSocket(void);
int relimGetPortNumber(void);
void relimitCloseMutex(void);
status_t FinishProlim(void);
status_t WaitForMsgfromProlim(void);
#endif
     
//#define LOG_FILE "C:/MRMPlus/log/Error/PDErrorLog.txt"
// void DebugInit()
// {
//     if ( freopen (LOG_FILE, "w+", stdout) == NULL ) {
//         fopen ("/tmp/PDErrorLog.txt", "w+");
//     }
// 
// }

/*********************************************************************
 * Global variables
 *********************************************************************/
extern relimitParams_t	relimitG;

/*********************************************************************
 * Static variables
 *********************************************************************/
static VfPathElem_t path[PROT_PATH_LEVEL] = { {SVN_ST_PROTOCOL, 0} };
static int	retryCt;

HANDLE g_hMutex = NULL;
lockList_t portLock[SERVICE_PORT_MAX] = {{"_UD_", -1, NULL},};
int lockCount=0;

/*********************************************************************
 * Static function prototypes
 *********************************************************************/
static status_t SendTreeID        (void);
static status_t WaitMsgCode       (int);
static status_t CreateTables      (void);
static status_t InitTables        (void);


/*************************************************************
* Method:    	SendTreeID
* Description: 	This function is used 
* Parameter: 	void  :
* Returns:   	status_t
*************************************************************/
static status_t SendTreeID(void)
{
    status_t	status = E_ERROR;
    proMsg_t	proMsg = {0,0L,0,NULL};

    TRACE_FUNC_ENTER("SendTreeID() : ");
    if (MAX_NO_RETRY < retryCt){
        WriteToErrorLog("E: *** Too  many retry!!!\n",__LINE__,ERROR_LOGONLY);
        relimitG.recreate_hot_line = 1;
        return(E_ERROR);
    }
    status = ProMsgSend(relimitG.wrfd, PROMSG_CON_TREE_ID,
                        sizeof(relimitG.dirSegShmid), &relimitG.dirSegShmid);
    if(E_NO_ERROR != status)
    {
      WriteToErrorLog("E ProMsgSend failed\n",__LINE__,ERROR_LOGONLY);
      return(E_ERROR);
    }
    status = ProMsgReceive(relimitG.rdfd, &proMsg);
    if(E_NO_ERROR != status)   
/*PROMSG_E_IO if read failed
PROMSG_E_EOF if end-of-file
PROMSG_E_MALLOC if malloc failed*/
      
    {//Vilas..July2012..Revw..if possible use status == E_ERROR study 'ProMsgReceive' before deciding
        WriteToErrorLog("E: *** Socket Disconnected.\n",__LINE__,ERROR_LOGONLY);
        relimitG.recreate_hot_line = 1;
        return(E_ERROR);
    }
    if(PROMSG_LIM_RELIMIT_DONE == proMsg.code)
    {
        return(E_NO_ERROR);
    }
    else if(PROMSG_REQ_TREE_ID == proMsg.code)
    {
        retryCt++;
        status = SendTreeID();
        if(E_NO_ERROR != status)
        {  
            WriteToErrorLog("E : *** Function SeTreeID() returned an error.\n",__LINE__,ERROR_LOGONLY);
            return(E_ERROR);
        }
    }
    else
    {
        WriteToErrorLog("E: *** Unexpected message is received.\n",__LINE__,ERROR_LOGONLY);
        relimitG.recreate_hot_line = 1;
        return(E_ERROR);
    }
    TRACE_FUNC_EXIT("SendTreeID() : ");
    return(E_NO_ERROR);
}

/*************************************************************
* Method:    	WaitMsgCode
* Description: 	This function is used 
* Parameter: 	int code :
* Returns:   	status_t
*************************************************************/
static status_t WaitMsgCode(int	code)
{
    proMsg_t	proMsg;
    status_t    l_nstatus = E_NO_ERROR;
    
    TRACE_FUNC_ENTER("WaitMsgCode() : ");
    if(E_NO_ERROR != ProMsgReceive(relimitG.rdfd, &proMsg))
    {
       WriteToErrorLog("E: *** Cannot receive message from prolim.\n",__LINE__,ERROR_LOGONLY);
        relimitG.pid = 0;
        
        l_nstatus = TerminateProlim();
        if_error(l_nstatus)
        {
            printf(" Error TerminateProlim %s    %d\n", __FILE__, __LINE__);
            fflush(stdout);
            return(E_ERROR);
        }
        relimitG.recreate_hot_line = 1;
        return(E_ERROR);
    }//Vilas..July2012..Revw..if we use else if here code can have only one return statement using 'status'
    
    if(proMsg.code == code)
    {
        return(E_NO_ERROR);
    }
    else if(PROMSG_REQ_TREE_ID == proMsg.code)
    {
        retryCt = 0;
        l_nstatus = SendTreeID();
        if (E_NO_ERROR != l_nstatus)
        {
            WriteToErrorLog("E: *** Function SendTreeID() returned an error.\n",__LINE__,ERROR_LOGONLY);
            return(E_ERROR);
        }
    }
    else if(PROMSG_LIM_RESPAWN == proMsg.code)
    {
        l_nstatus = E_NO_ERROR;
        l_nstatus = ProMsgSend(relimitG.wrfd, PROMSG_CMD_EXIT, 0, NULL);
        if_error(l_nstatus)
        {
            WriteToErrorLog("E: *** Function ProMsgSend() returned an error",__LINE__,ERROR_LOGONLY);
            return(E_ERROR);
        }
        
        l_nstatus = ProMsgReceive(relimitG.rdfd, &proMsg);
        if_error(l_nstatus)
        {
           WriteToErrorLog("E: *** Function ProMsgReceive() returned an error.\n",__LINE__,ERROR_LOGONLY);
            return(E_ERROR);
        }
        
        l_nstatus = relimitTerminateProcess();
        if_error(l_nstatus)
        {
            WriteToErrorLog("E: *** Function relinitTerminateProcess() returned an error.\n",__LINE__,ERROR_LOGONLY);
            return(E_ERROR);
        }
        
        if(NULL != relimitExec(relimitG.scratchPad,relimitG.seqgenName,&relimitG.pid))
        {
            l_nstatus = ProMsgReceive(relimitG.rdfd, &proMsg);
            if(E_NO_ERROR == l_nstatus)
            {
                if(proMsg.code == PROMSG_REQ_TREE_ID)
                {
                    retryCt = 0;
                    l_nstatus = SendTreeID();
                    if (E_NO_ERROR != l_nstatus)
                    {
                        WriteToErrorLog("E: *** Function SendTreeID() returned an error",__LINE__,ERROR_LOGONLY);
                        return(E_ERROR);
                    }
                }
            }else{
                WriteToErrorLog("E: *** Function ProMsgReceive() returned an error.\n",__LINE__,ERROR_LOGONLY);
                return(E_ERROR);
            }
        
    }
    else
    {
        l_nstatus = E_ERROR;
    }
    
    if(E_NO_ERROR != l_nstatus)
    {
        l_nstatus =  relimitTerminateProcess();
        if_error(l_nstatus)
        {
           WriteToErrorLog("E: *** Function relimitTerminateProcess() returned an error.\n",__LINE__,ERROR_LOGONLY);
            return(E_ERROR);
        }
    }
    return(l_nstatus);
}

else
{
    register unsigned int	l_nmsgIndex = 0;
    char		*msg1 = NULL;
    char		*msg2 = NULL;
    
    unsigned int l_nsize = (sizeof(msgDefs) / sizeof(*msgDefs));    
    for(l_nmsgIndex = 0; l_nmsgIndex < l_nsize ; l_nmsgIndex++)
    {
        if ((unsigned)proMsg.code == msgDefs[l_nmsgIndex].code){
            msg1 = msgDefs[l_nmsgIndex].message;
        }
        else if ((unsigned)code == msgDefs[l_nmsgIndex].code){
            msg2 = msgDefs[l_nmsgIndex].message;
        }
        printf("E: *** Receive unexpected message from prolim.(%s -> %s)\n", msg1, msg2);
       // WriteToErrorLog("E: *** Receive unexpected message from prolim.(%s -> %s)\n",__LINE__,ERROR_LOGONLY);
    }
    
    l_nstatus = TerminateProlim();
    if_error(l_nstatus)
    {
        printf(" Error TerminateProlim %s    %d\n", __FILE__, __LINE__);
        fflush(stdout);
        return(E_ERROR);
    }
    relimitG.recreate_hot_line = 1;
    return(E_ERROR);
}
    TRACE_FUNC_EXIT("WaitMsgCode() : ");
    return(E_NO_ERROR);
}


/*************************************************************
* Method:    	CreateTables
* Description: 	This function is used 
* Parameter: 	void  :
* Returns:   	status_t
*************************************************************/
static status_t CreateTables(void)
{
    int			entryCt=0;
    int l_nstatus = E_ERROR;
    plParCt = sizeof(name_table) / sizeof(*name_table);
    l_nstatus = plGetTables(plParCt);

    TRACE_FUNC_ENTER("CreateTables() : ");
    if(E_NO_ERROR != l_nstatus){
        WriteToErrorLog("E: *** Function plGetTables() returned an error.\n",__LINE__,ERROR_LOGONLY);
        return(E_ERROR);
    }

    for(entryCt = 0; entryCt < plParCt; entryCt++)
    {
        plParNameTbl[entryCt]	= name_table[entryCt].name;
        plSVN_ST[entryCt]	= name_table[entryCt].pl.name;
        plVfSVN_ST[entryCt]	= name_table[entryCt].vf.st_name;
        plVfSVD_ST[entryCt]	= name_table[entryCt].vf.st_depth;
        plVfSVN[entryCt]	= name_table[entryCt].vf.vf_name;
        plVfSVT[entryCt]	= name_table[entryCt].vf.vf_type;
        plValTypeTbl[entryCt]	= name_table[entryCt].pl.type;
        plUnitsTbl[entryCt]	= name_table[entryCt].pl.unit;
        plSfParCt[entryCt]	= 0;
        plDefLimItemCtTbl[entryCt] = 0;
        plDefLimTypeTbl[entryCt] = plLimTNone;
    }
    TRACE_FUNC_EXIT("CreateTables() : ");
    return(E_NO_ERROR);
    
}

/*************************************************************
* Method:    	InitTables
* Description: 	This function is used 
* Parameter: 	void  :
* Returns:   	status_t
*************************************************************/
static status_t InitTables(void)
{
    int		entryCt=0;
    plValT	value;
    plStatusT	plStat;
    status_t l_return_val = E_NO_ERROR;
    
    TRACE_FUNC_ENTER("InitTables() : ")
    
    value.valueType	 = plValTInt;
    value.units		 = plUnitsTunitless;
    value.itemCt	 = 1;
    PL_VAL_INT(value) = FALSE;
    for(entryCt = 0; entryCt < plParCt; entryCt++)
    {
        plStat = plPutVal(path, SVD_ST_STUDY,
        name_table[entryCt].name, &value);
        if(plStat != plStatSuccess)
        {
            printf("E: *** in plPutVal: %s\n", plStrStat(plStat));
            l_return_val = E_ERROR;
            break;
        }
    }
    TRACE_FUNC_EXIT("InitTables() : ");
    return(l_return_val);
}

/*************************************************************
* Method:    	SetupProlim
* Description: 	This function is used 
* Parameter: 	void  :
* Returns:   	status_t
*************************************************************/
status_t SetupProlim(void)
{
    plStatusT	plStat = plStatSuccess;
    int status = E_ERROR;
    int32_t dirSegShmid = 0;
    //plStatusT l_stat = plStatSuccess;

     TRACE_FUNC_ENTER("SetupProlim() : ");
    if(relimitG.dirSegShmid > 0){ 
        plExit(relimitG.dirSegShmid);
    }
    plStat = plInitTree(1000, &dirSegShmid);
    if(plStat != plStatSuccess)
    {
        fprintf(stdout, "E: *** in plInitTree: %s\n", plStrStat(plStat));
        relimitG.recreate_hot_line = 1;
        return(E_ERROR);
    }
    relimitG.dirSegShmid = dirSegShmid;
     plStat = plCreateStudyNode();
    if (plStat != plStatSuccess )
    {
        printf( "E: *** in plCreateStudyNode: %s\n", plStrStat(plStat));
        relimitG.recreate_hot_line = 1;
        return(E_ERROR);
    }

    if (E_NO_ERROR != CreateTables())
    {
        WriteToErrorLog("E: *** CreateTables() returned an error.\n",__LINE__,ERROR_LOGONLY);
        return(E_ERROR);
	}


    plStat = plAttachTree();
    if (plStatSuccess != plStat){
       printf( "E: *** in plAttachTree: %s\n", plStrStat(plStat));
        return(E_ERROR);
    }
    status = InitTables() ;
    if(E_NO_ERROR != status){
        printf("E: *** in InitTables: %s\n", plStrStat(plStat));
        return(E_ERROR);
    }
    TRACE_FUNC_EXIT("SetupProlim() : ");
    return(E_NO_ERROR);
}

/*************************************************************
* Method:    	FinishProlim
* Description: 	This function is used 
* Parameter: 	void  :
* Returns:   	status_t
*************************************************************/
status_t FinishProlim(void)
{
    plStatusT	plStat = plStatSuccess;
     TRACE_FUNC_ENTER("FinishProlim() : ");
    if(0 < relimitG.dirSegShmid){ 
        plStat = plExit(relimitG.dirSegShmid);
        if(plStatSuccess != plStat){
            return(E_ERROR);
        }
        relimitG.dirSegShmid = 0;
    } 
    TRACE_FUNC_EXIT("FinishProlim() : ");
    return(E_NO_ERROR);
}

/*************************************************************
* Method:    	RelimitProlim
* Description: 	This function is used 
* Parameter: 	void  :
* Returns:   	status_t
*************************************************************/
status_t RelimitProlim(void)
{
     TRACE_FUNC_ENTER("RelimitProlim() : ");
    if_error(ProMsgSend(relimitG.wrfd, PROMSG_CON_RELIMIT, 0, NULL))
    { 
       WriteToErrorLog("E: *** Function ProMsgSend() returned an error.\n",__LINE__,ERROR_LOGONLY);
    	return(E_ERROR);
    }
    if_error(WaitMsgCode(PROMSG_LIM_RELIMIT_DONE))
	{
         WriteToErrorLog("E: *** Function WaitMsgCode() returned an error.\n",__LINE__,ERROR_LOGONLY);
		return(E_ERROR);
    }
    TRACE_FUNC_EXIT("RelimitProlim() : ");
    return(E_NO_ERROR);
}

/*************************************************************
* Method:    	relimitTerminateProcess
* Description: 	This function is used 
* Parameter: 	void  :
* Returns:   	status_t
*************************************************************/
status_t relimitTerminateProcess(void)
{   status_t status = E_NO_ERROR;
TRACE_FUNC_ENTER("relimitTerminateProcess() : ");
    if(NULL != relimitG.pHandle){   
            if(WAIT_TIMEOUT == WaitForSingleObject(relimitG.pHandle, PL_TERMINATE_WAIT_TIME)){
                status = TerminateProcess(relimitG.pHandle, 0);
                if(status != E_NO_ERROR){
                    WriteToErrorLog("E: *** Function TerminateProcess() returned an error.\n",__LINE__,ERROR_LOGONLY);
                    return(status);
                }
            }
            CloseHandle(relimitG.pHandle);
            relimitG.pHandle = NULL;
    }else{
        WriteToErrorLog("E: *** relimitG.pHandle found NULL.\n",__LINE__,ERROR_LOGONLY);
        return(status);
    }
    if(0 != relimitG.wrfd){     //AS doubt
        close(relimitG.wrfd);
    }else{

    }
    relimitG.pid = 0;
    relimitG.socketFd = -1;
    relimitG.wrfd = relimitG.rdfd = 0;
    TRACE_FUNC_EXIT("relimitTerminateProcess() : ");
	return(E_NO_ERROR);
}

/*************************************************************
* Method:    	TerminateProlim
* Description: 	This function is used 
* Parameter: 	void  :
* Returns:   	status_t
*************************************************************/
status_t TerminateProlim(void)
{
     TRACE_FUNC_ENTER("TerminateProlim() : ");
    if(relimitG.pid){
        if_error(ProMsgSend(relimitG.wrfd, PROMSG_CMD_EXIT, 0, NULL)){
           WriteToErrorLog("E: *** Function ProMsgSend() returned an error.\n",__LINE__,ERROR_LOGONLY);
        	return(E_ERROR);
        }        
        if_error(WaitMsgCode(PROMSG_CMD_EXIT)){
           WriteToErrorLog("E: *** Function WaitMsgCode() returned an error.\n",__LINE__,ERROR_LOGONLY);
            return(E_ERROR);
       	}
        relimitTerminateProcess();//Vilas..July2012..Revw..already handled in 'waitmsgcode'
    }
    if(relimitG.dirSegShmid){
        if_error( FinishProlim()){
	       	WriteToErrorLog("Problem while clearing shared memory\n",__LINE__,ERROR_LOGONLY);
		   	return(E_ERROR);
       	}
    }
    TRACE_FUNC_EXIT("TerminateProlim() : ");
    return(E_NO_ERROR);
}

/*************************************************************
* Method:    	SaveProlim
* Description: 	This function is used 
* Parameter: 	void  :
* Returns:   	status_t
*************************************************************/
status_t SaveProlim(void)
{   
    int l_nstatus = E_ERROR;
    plValT	val;
    plStatusT	plStat = plStatSuccess;
     TRACE_FUNC_ENTER("SaveProlim() : ");
    
    plStat = plGetProtocolVal(PL_SEQUENCE_TEMPLATE, &val);
    if(plStatSuccess == plStat)
    {
        if(strcmp(PL_VAL_STRING(val), relimitG.seqgenName))
        {
            printf("I: Sequence is changed from %s to %s.\n",
            relimitG.seqgenName,
            PL_VAL_STRING(val));
        }
    }
    else
    {
        WriteToErrorLog("E: *** Cannot get new sequence name.\n",__LINE__,ERROR_LOGONLY);
        return(E_ERROR);
    }

    l_nstatus = ProMsgSend(relimitG.wrfd, PROMSG_CON_SAVE_TREE, 0, NULL);
    if_error(l_nstatus)
    {       
        WriteToErrorLog("E: *** Function ProMsgSend() returned an error.\n",__LINE__,ERROR_LOGONLY);
    	return(E_ERROR);
    }
    if_error(WaitMsgCode(PROMSG_LIM_SAVE_DONE))
    {
        return E_ERROR;
    }
    TRACE_FUNC_EXIT("SaveProlim() : ");
    return(E_NO_ERROR);
}

/*********************************************************************
 * Description:
 * Assumptions:
 * Side Effects:
 * Return Value:
 *********************************************************************/
status_t ProMgrUpdateProlim(void)
{   status_t l_nstatus = E_ERROR;
     TRACE_FUNC_ENTER("ProMgrUpdateProlim() : ");
    l_nstatus = ProMsgSend(relimitG.wrfd, PROMSG_CON_RELIMIT, 0, NULL);
    if_error(l_nstatus){
       WriteToErrorLog("E: *** Function ProMsgSend() returned an error.\n",__LINE__,ERROR_LOGONLY);
        return(E_ERROR);
    }

    l_nstatus = WaitMsgCode(PROMSG_LIM_RELIMIT_DONE);
    if_error(l_nstatus){ 
        WriteToErrorLog("E: *** Function WaitMsgCode() returned an error.\n",__LINE__,ERROR_LOGONLY);
        return(E_ERROR);
    }
    TRACE_FUNC_EXIT("ProMgrUpdateProlim() : ");
    return(E_NO_ERROR);
}

/*********************************************************************
 * Description:
 * Assumptions:
 * Side Effects:
 * Return Value:
 *********************************************************************/
status_t ProMgrReloadProlim(void)
{
    int l_nstatus = ProMsgSend(relimitG.wrfd, PROMSG_CON_RELOAD_TREE, 0, NULL);
     TRACE_FUNC_ENTER("ProMgrReloadProlim() : ");
    if_error(l_nstatus){
        WriteToErrorLog("E: *** Function ProMsgSend() returned an error.\n",__LINE__,ERROR_LOGONLY);
        return(E_ERROR);
    }

    l_nstatus = WaitMsgCode(PROMSG_LIM_RELIMIT_DONE);
    if_error(l_nstatus){
        WriteToErrorLog("E: *** Function WaitMsgCode() returned an error.\n",__LINE__,ERROR_LOGONLY);
        return(E_ERROR);
    }
    TRACE_FUNC_EXIT("ProMgrReloadProlim() : ");
    return(E_NO_ERROR);
}

// /*********************************************************************
//  * Description:    ポート番号取得時、mutexを用いて二重取得を防止する
//  * Assumptions:
//  * Side Effects:
//  * Return Value:
// *********************************************************************/
// int relimitGetPortNumber(void)//Vilas..July2012..Revw..function use commented..not used anywhere
// {
//    struct servent  *proConService;
//     int             i, cnt, start;
// 
// //    int             port;
// 
//     
//     WSADATA wsaData;     
//     if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
//         //PROCON_DB_//printf("[ProConMakeSocket]","WSAStartup failed\n",TRACE_MISC);
//         //printf("\nerror");
//         return(E_ERROR);
// 	}
// 
// 
//     /*start = atoi(PROCON_SERVICE_NAME_SUFFIX);
//     if(strcmp(portLock[0].serviceName , "_UD_") == 0)
//     {
//         char    srvName[SERVICE_NAME_MAX];
// 
//         // initial setup 
//         for(i = start, lockCount = 0; i < start + SERVICE_PORT_MAX; i++)
//         {
//             sprintf(srvName, "%s%d", PROCON_SERVICE_NAME_BASE, start);
//             if ((proConService = getservbyname(srvName, "tcp")) != NULL)
//             {
//                 strcpy(portLock[lockCount].serviceName, srvName);
//                 portLock[lockCount].portNum = ntohs(proConService->s_port);
//                 //portLock[lockCount].portNum = htonl(18002);
//                 portLock[lockCount].hMutex = NULL;
//                 lockCount++;
//             }
//         }
//     }*/
//     if(0 == lockCount)
//     {
//         /* not found port */
//         g_hMutex = NULL;
//         return(E_ERROR);
//     }
//     for(cnt = 0; cnt <= lockCount; cnt++)
//     {
//         if(NULL == portLock[cnt].portNum != -1 && portLock[cnt].hMutex)
//         {
//             g_hMutex = CreateMutex(NULL, FALSE,(LPTSTR)(LPCTSTR) portLock[cnt].serviceName);
//             if(NULL != g_hMutex){
//                 if(WAIT_TIMEOUT != WaitForSingleObject(g_hMutex, MUTEX_WAIT_TIME)){
//                     portLock[cnt].hMutex = g_hMutex;
//                     return portLock[cnt].portNum;
//                 }
//                 else
//                     CloseHandle(g_hMutex);
//             }
//         }
//     }
//     /* cannot get mutex */
//     g_hMutex = NULL;
//     return -2;
// }

/*********************************************************************
 * Description:   mutex終了処理
 * Assumptions:
 * Side Effects:
 * Return Value:
 *********************************************************************/
void relimitCloseMutex(void)
{
     TRACE_FUNC_ENTER("relimitCloseMutex() : ");
    if(NULL != g_hMutex){
        int l_nLcounter = 0;
        for(l_nLcounter = 0; l_nLcounter < lockCount; l_nLcounter++)
        {
            if(portLock[l_nLcounter].hMutex == g_hMutex){//Vilas..July2012..Revw..pointer comparison pl check if this is ok
            
                break;
            }
        }
        if(l_nLcounter< lockCount){
            portLock[l_nLcounter].hMutex = NULL;
        }
        CloseHandle(g_hMutex);
        g_hMutex = NULL;
    }
    TRACE_FUNC_EXIT("relimitCloseMutex() : ");
}
/*********************************************************************
 * Description: ソケットを生成する
 * Assumptions:
 * Side Effects:
 * Return Value:
 *********************************************************************/
int32_t relimMakeSocket(void)
{
    struct sockaddr_in  sockAddr;
    int32_t sockId = 0;
    struct servent *proMgrService = NULL;
    
    WSADATA wsaData;     
     TRACE_FUNC_ENTER("relimMakeSocket() : ");
    if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
        WriteToErrorLog("E: *** WSAStartup failed.\n",__LINE__,ERROR_LOGONLY);
    }
    
    
    proMgrService = getservbyname("proClient2","tcp");
    if(NULL == proMgrService)
    {
        //ProMgrPrintLog("E: *** Socket not created (%s:%d)\n",__FILE__,__LINE__);
        //Vilas..July2012..Revw..uncomment error log
        return(E_ERROR);
    }

    memset((char *)&sockAddr,0,sizeof(sockAddr));			
    sockAddr.sin_family      = AF_INET;
    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    relimitG.socketPort = (int)ntohs(proMgrService->s_port);
    
    sockAddr.sin_port        = proMgrService->s_port;	
    //-Patni-AT/2009Mar20/Modified/PSP-1#internal "libWNP_I patch2"
    /*
    * Make socket
    */
    
    
   if (0 > (sockId = socket(AF_INET, SOCK_STREAM, 0)))
    {
        WriteToErrorLog("E: *** ProMgrMakeSocket: socket\n",__LINE__,ERROR_LOGONLY);
        return(E_ERROR);
    }
    relimitG.socketId = sockId;
    
    if(bind(sockId, (struct sockaddr *)&sockAddr, sizeof(sockAddr)) == -1)
    {
        WriteToErrorLog("E: *** ProMgrMakeSocket: bind\n",__LINE__,ERROR_LOGONLY);
        return(E_ERROR);
    }

    if(0 > listen(sockId, 5))
    {
       WriteToErrorLog("E: *** ProMgrMakeSocket: listen\n",__LINE__,ERROR_LOGONLY);
        return(E_ERROR);
    }
    TRACE_FUNC_EXIT("relimMakeSocket() : ");
    return(sockId);
}


/*********************************************************************
 * Description: ソケットへの接続を accept する
 * Assumptions:
 * Side Effects:
 * Return Value:
 *********************************************************************/
int AcceptSocket(int sockId)
{
    int                 fd = 0;
    struct sockaddr_in  from;
    int                 sockLen = 0;
 //   int l_nstatus =E_ERROR;
    /* Accept the INET socket for Windows */
     TRACE_FUNC_ENTER("AcceptSocket() : ");
    sockLen = sizeof(struct sockaddr_in);
    
    fd = accept(sockId, (struct sockaddr *)&from, &sockLen);
    if (0 > fd){
        WriteToErrorLog("E: *** ProMgrMakeSocket: accept error\n",__LINE__,ERROR_LOGONLY);
        return(E_ERROR);
    }
    TRACE_FUNC_EXIT("AcceptSocket() : ");
    return(fd);
}
/*********************************************************************
 * Description:	タイムアウト時間指定で着信を監視する。
 * Assumptions:
 * Side Effects:
 * Return Value:
 *********************************************************************/
int CheckInput(int fd, struct timeval *timeout)
{
    int    rc = 0;
    fd_set readfds;
    int    repeat = RETRY_COUNT;
    struct timeval timeoutval;
    int error_no = E_ERROR;
     TRACE_FUNC_ENTER("CheckInput() : ");
    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);
    for(; repeat > 0; repeat--)
    {
        timeoutval = *timeout;
        rc = select(fd + 1, &readfds, NULL, NULL, &timeoutval);
        error_no = WSAGetLastError();
        if(0 > rc)
        {
            if(EINTR == errno)
                continue; /* signal interrupt */
            WriteToErrorLog("E: *** ProMgrCheckInput error\n",__LINE__,ERROR_LOGONLY);
            return 2;
        }
        if((0 == rc) || (FD_ISSET(fd, &readfds)))
            break;
    }
    if((0 == rc) || (0 == repeat))
    {
        WriteToErrorLog("E: *** ProMgrCheckInput timeout\n",__LINE__,ERROR_LOGONLY);
        return E_ERROR;
    }
    TRACE_FUNC_EXIT("CheckInput() : ");
    return(E_NO_ERROR);
}
/*********************************************************************
 * Description: prolimの情報を取得する
 * Assumptions:
 * Side Effects:
 * Return Value:
 *********************************************************************/
static status_t Negotiation(int socketFd)
{
    status_t    status = E_ERROR;
    proMsg_t    msg;
     TRACE_FUNC_ENTER("Negotiation() : ");
    status = ProMsgReceive(socketFd, &msg);

    if(E_NO_ERROR == status){
            if(PROMSG_CMD_REGIST == msg.code){
                pid_t   pid = 0;
                char *  ident = (char *)msg.data + sizeof(pid_t);

                memcpy(&pid, msg.data, sizeof(pid_t));
                if(0 == strcmp(ident, "prolim")){
                    relimitG.rdfd = relimitG.wrfd = relimitG.socketFd = socketFd;
                    relimitG.pid = pid;
                }else{
                    status = E_ERROR;
                }
            }else{
            status = E_ERROR;
            }
            if(E_NO_ERROR != status){
                WriteToErrorLog("E: *** proMgrNegotiation error\n",__LINE__,ERROR_LOGONLY);
            }
        ProMsgFree(&msg);
    }else{
           WriteToErrorLog("E: *** Function ProMsgReceive() returned an error\n",__LINE__,ERROR_LOGONLY);
    }
    TRACE_FUNC_EXIT("Negotiation() : ");
    return(status);
}
/*********************************************************************
 * Description:	prolimとの通信を開始する
 * Assumptions:
 * Side Effects:
 * Return Value:
 *********************************************************************/
status_t StartupSync(void)
{
    status_t       status;
    int            socketFd;
    struct timeval timeout;
     TRACE_FUNC_ENTER("StartupSync() : ");
    if(0 <= relimitG.socketId)
    {
        timeout.tv_sec  = PL_STARTUP_WAIT_TIME;
        timeout.tv_usec = 0;
        if (!CheckInput(relimitG.socketId, &timeout))
        {
            socketFd = AcceptSocket(relimitG.socketId); 

            if (-1 == socketFd)
                return(E_ERROR);
            //if (E_NO_ERROR != (status = Negotiation(socketFd)))
			status = Negotiation(socketFd);
			if (E_NO_ERROR != status)
            {
                return(status);
            }
        }
        else
        {
            return(E_ERROR);
        }
    }
    else
        return(E_ERROR);

    TRACE_FUNC_EXIT("StartupSync() : ");
    return(E_NO_ERROR);
}


/*************************************************************
* Method:    	WaitForMsgfromProlim
* Description: 	This function is used 
* Parameter: 	void  :
* Returns:   	status_t
*************************************************************/
status_t WaitForMsgfromProlim(void)
{
       int l_nstatus = E_ERROR;
        TRACE_FUNC_ENTER("WaitForMsgfromProlim() : ");
    l_nstatus = WaitMsgCode(PROMSG_REQ_TREE_ID);
    if_error(l_nstatus){
        WriteToErrorLog("E: *** Function WaitMsgCode() returned an error.\n",__LINE__,ERROR_LOGONLY);
        return(E_ERROR);
    }

    l_nstatus = ProMsgSend(relimitG.wrfd,PROMSG_CON_TREE_ID,sizeof(relimitG.dirSegShmid),&relimitG.dirSegShmid);
    if_error(l_nstatus){
        WriteToErrorLog("E: *** Function ProMsgSend() returned an error.\n",__LINE__,ERROR_LOGONLY);
        return(E_ERROR);
    }

    l_nstatus = WaitMsgCode(PROMSG_LIM_RELIMIT_DONE);

    if_error(l_nstatus){
       WriteToErrorLog("E: *** Function WaitMsgCode() returned an error.\n",__LINE__,ERROR_LOGONLY);
        return(E_ERROR);
    }

    TRACE_FUNC_EXIT("WaitForMsgfromProlim() : ");
    return(E_NO_ERROR);
}

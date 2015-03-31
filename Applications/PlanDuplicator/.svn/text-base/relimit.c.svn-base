#include <stdio.h>
#include <stdlib.h>
#include "RelimitLogHelper.h"
#include "relimitParams.h"
//#include "dataTypes.h"


/*
“seqgen –m executable –i “studyfile” –l <linkCommand> -f <fepCommand> -p <protocol> -s <sub-protocol> -e <outputfile> -v <version>



e.g. c:/gp/seq/sequences/FE_Crt/seqgen.exe –m executable –i Runxxx.-5000 –l linkE1 –f fepE1 –p 0 –s 0 –e img –v 01

*/

relimitParams_t relimitG;

/*************************************************************
* Method:    	Relimit
* Description: 	This function is used 
* Parameter: 	char * studyFileName :
* Parameter: 	int sourceProtocolIndex :
* Parameter: 	relimitUpdateParams_t * f_relimitUpdateParams :
* Returns:   	int
*************************************************************/
int Relimit(const char *studyFileName,
            const int  sourceProtocolIndex,
            const relimitUpdateParams_t *f_relimitUpdateParams);
status_t relimitDone(void);

extern int32_t relimMakeSocket(void);
//extern int relimitGetPortNumber(void);
extern void relimitCloseMutex(void);
extern int relimitProcess(void);
extern status_t FinishProlim(void);
extern status_t TerminateProlim(void);

/*************************************************************
* Method:    	Relimit
* Description: 	This function is used 
* Parameter: 	char * studyFileName :
* Parameter: 	int sourceProtocolIndex :
* Parameter: 	relimitUpdateParams_t * f_relimitUpdateParams :
* Returns:   	int
*************************************************************/
int Relimit(const char *studyFileName,
            const int  sourceProtocolIndex,
            const relimitUpdateParams_t *f_relimitUpdateParams)
{
//Vilas..July2012..Revw..remove commented code from delivery..if reqd keep a separate copy of .cpp code
    //char invocation[BUFF_LONG_LEN];
    int l_status = E_NO_ERROR;
    
    TRACE_FUNC_ENTER("Relimit() : ");
    fflush(stdout);
    
    // socket initialization
    if(NULL == studyFileName)
    {
        WriteToErrorLog("\n Improper studyfile ",__LINE__,ERROR_LOGONLY);
        return(E_ERROR);
    }
    //fprintf(stdout, "Study file name is %s\n", studyFileName);
    //relimitG.dirSegShmid = 0;
    /*relimitG.socketPort = relimitGetPortNumber();
    if(0 > relimitG.socketPort)
    {
        if(-1 == relimitG.socketPort)
        {
            //printf("E: *** socket port error\n");
        }
        else
        {
            //printf("E: *** socket mutex error\n");
        }
        relimitDone();
        return(E_ERROR);
    }*/
    relimitG.socketId = relimMakeSocket();
    if(0 > relimitG.socketId)
    {
        //relimitDone();//Vilas..July2012..Revw..commented code..pl check
        //Vilas..July2012..Revw..set and use return status if possible
        return(E_ERROR);
    }

    strcpy(relimitG.studyFile, studyFileName);
    relimitG.protIndex = sourceProtocolIndex;
    relimitG.v_offset = f_relimitUpdateParams->v_offset;
    relimitG.v_phase = f_relimitUpdateParams->v_phase;
    relimitG.v_slice = f_relimitUpdateParams->v_slice;
    relimitG.m_nSliceNum = f_relimitUpdateParams->m_nSliceNum;
    relimitG.m_fSliceGap = f_relimitUpdateParams->m_fSliceGap;
    relimitG.m_fscliceThck = f_relimitUpdateParams->m_fscliceThck;
    relimitG.m_fov = f_relimitUpdateParams->m_fov;
    relimitG.m_nScannedProtIdx = f_relimitUpdateParams->m_nScannedProtIdx;
    relimitG.m_nProcIdx = f_relimitUpdateParams->m_nProcIdx;
    relimitG.m_nCenterSliceNum = f_relimitUpdateParams->m_nCenterSliceNum;
    relimitG.m_nMpr_Voi_flag = f_relimitUpdateParams->m_nMpr_Voi_flag;
    relimitG.v_vec1 = f_relimitUpdateParams->m_vec1;
    relimitG.v_vec2 = f_relimitUpdateParams->m_vec2;
    relimitG.m_nflag_scout_selection = f_relimitUpdateParams->m_nflag_scout_selection;
    relimitG.m_fcouchOffset = f_relimitUpdateParams->m_fcouchOffset;


    fprintf(stdout, "In relimit process-- study name is %s and index is %d\n", relimitG.studyFile, relimitG.protIndex);
    fflush(stdout);

    if (E_NO_ERROR != relimitProcess()) {
        l_status = E_ERROR;
    }
    if_error(relimitDone())//Vilas..July2012..Revw..set and use return status if possible
    {
         WriteToErrorLog("E: *** Function relimitDone() returned an error.\n",__LINE__,ERROR_LOGONLY);
        return(E_ERROR);
    }
    TRACE_FUNC_EXIT("Relimit() : ");
    return(l_status);
}

/*************************************************************
* Method:    	relimitDone
* Description: 	This function is used 
* Returns:   	status_t
*************************************************************/
status_t relimitDone(void)
{
    TRACE_FUNC_ENTER("relimitDone() : ");
    if (relimitG.socketId > 0)
    {
      close(relimitG.socketId);//Vilas..July2012..Revw..check if this function has return value
    }  
    relimitG.socketId = -1;

    WSACleanup();//Vilas..July2012..Revw..check if this function has return value
    FinishProlim();//Vilas..July2012..Revw..set and use return status
    relimitCloseMutex();//Vilas..July2012..Revw..check if this function has return value

    TRACE_FUNC_EXIT("relimitDone() : ");
    return(E_NO_ERROR);
}

/*************************************************************
* Method:    	ForceExit
* Description: 	This function is used 
* Parameter: 	char * file :
* Parameter: 	int line :
* Returns:   	int
*************************************************************/
int ForceExit(char *file, int line)
{   int l_nstatus = E_ERROR;
    TRACE_FUNC_ENTER("ForceExit() : ");
    if(0 < relimitG.pid){ 
        l_nstatus = TerminateProlim(); 
        if_error(l_nstatus){
            printf(" Error TerminateProlim %s    %d\n", __FILE__, __LINE__);
            fflush(stdout);
            return(E_ERROR);
        }
    } 
    //printf("E: *** Abnormal exit (%s:%d)\n", file, line);
    l_nstatus = relimitDone();
    if_error(l_nstatus){
       WriteToErrorLog("E: *** Function relimitDone() returned an error.\n",__LINE__,ERROR_LOGONLY);
        return(E_ERROR);
    }
    TRACE_FUNC_EXIT("ForceExit() : ");
    return(E_NO_ERROR);
}

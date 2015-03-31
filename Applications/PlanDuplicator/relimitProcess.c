#include <stdio.h>
#include <stdlib.h>
#include "RelimitLogHelper.h"
#include "relimitParams.h"
//#include "dataTypes.h"

#include <tami/common/basicdefs.h>
#include <tami/libvf/varfields.h>
#include <tami/vfStudy/vfstudy.h>
#include <tami/vfStudy/vfs_structure.h>
#include <tami/vfStudy/vf_appcodes.h>
#include <tami/libpl/plSeqSelDB.h>
#include <tami/common/globalSwitches.h>
#include <tami/libseq/seq.h>
#include <tami/libpltree/Plsftree.h>
#include <tami/libpl/plParDefs.h>
#include <tami/libvfToUi/vfToUiproto.h>


status_t CopyScratchPadToStudyFile(void);
int createScratchPad(void);
int getSeqName(char *pasFileName, char *seqName);
int TraverseProt(void);
status_t UpdateVector(VfPathElem_t path[]);
status_t UpdateSliceThickness(float thickness);
status_t UpdateSliceNumber(int number);
static status_t FlUpdateLocatorImages(VfPathElem_t path[],int slice_number,int f_nScannedProtIdx,int f_nProcIdx);
status_t UpdateSliceGap(float gap);
status_t UpdatePlanStatus(void);
status_t UpdateFov(f32vec2_t f_fFov);
status_t UpdateOffsetForVOI(int f_nflag_scout_selection,VfPathElem_t path[],float f_fcouchOffset);

extern relimitParams_t    relimitG;
extern char *relimitExec(char *scratchPad, char *sequenceName, pid_t *pid_out);
extern status_t SetupProlim(void);
extern status_t RelimitProlim(void);
extern status_t SaveProlim(void);
extern status_t TerminateProlim(void);
extern int ForceExit(char *file, int line);
extern int GetSubNodes(VfDBHandle_t srcVf, VfPathElem_t pathP[], int curLevel,INT *f_ntotalsubnodes);
extern status_t relimitDone();

/*************************************************************
* Method:    	relimitProcess
* Description: 	This function is used 
* Parameter: 	void  :
* Returns:   	int
*************************************************************/
int relimitProcess(void)
{

    int status = 0;
    status_t l_status = 0;
   TRACE_FUNC_ENTER("relimitProcess() : ");
    fflush(stdout);
    
    status = createScratchPad();
    if(E_NO_ERROR != status)
    {
        return(E_ERROR);
    }
    
    status = getSeqName(relimitG.scratchPad, relimitG.seqgenName);  //status check
        
    printf("scratchpad is %s,seqname is %s\n", relimitG.scratchPad, relimitG.seqgenName);
    fflush(stdout);
    
    if_error(SetupProlim()) {
        WriteToErrorLog("Error SetupProlim\n",__LINE__,ERROR_LOGONLY);
         fflush(stdout);
         return(E_ERROR);
    }

    if(relimitExec(relimitG.scratchPad,
        relimitG.seqgenName,
        &relimitG.pid) == NULL)
    {
        printf("E: *** Cannot Execute prolim (%s).\n",relimitG.seqgenName);
        fflush(stdout);
        return(E_ERROR);
    }

    
    if_error(TraverseProt()) {
        printf("Error TraverseProt %s    %d\n", __FILE__, __LINE__);
        fflush(stdout);
        return(E_ERROR);
    }

    if_error(RelimitProlim()) {
        printf("Error RelimitProlim %s    %d\n", __FILE__, __LINE__);
        fflush(stdout);
        return(E_ERROR);
    }

    if_error(SaveProlim()) {
        printf("Error SaveProlim %s    %d\n", __FILE__, __LINE__);
        fflush(stdout);
        return(E_ERROR);
    }
    if_error(TerminateProlim())
    {
        printf(" Error TerminateProlim %s    %d\n", __FILE__, __LINE__);
        fflush(stdout);
        return(E_ERROR);
    }

    l_status = relimitDone();
    if (E_NO_ERROR != l_status){
      return(l_status);
    }

    if(E_NO_ERROR != CopyScratchPadToStudyFile())
    {
        printf("Error saving scratch pad\n %s    %d\n", __FILE__, __LINE__);
        fflush(stdout);
        return(E_ERROR);
    }
    fprintf(stdout, "[relimitProcess:] Exit %s    %d\n", __FILE__, __LINE__);
        fflush(stdout);

    TRACE_FUNC_EXIT("relimitProcess() : ");
    return(E_NO_ERROR);
}


/*************************************************************
* Method:    	CopyScratchPadToStudyFile
* Description: 	This function is used 
* Parameter: 	void  :
* Returns:   	status_t
*************************************************************/
status_t CopyScratchPadToStudyFile(void)
{
    VfDBName_t		vf_name;
    VfDBHandle_t	vfd_dst;
    VfPathElem_t	vf_path_dst[MAX_DEPTH], vf_path_src[MAX_DEPTH];
    status_t status = E_NO_ERROR;
    
    TRACE_FUNC_ENTER("CopyScratchPadToStudyFile() : ");
    memset((char *)vf_path_dst, 0x00, sizeof(vf_path_dst));
    memset((char *)vf_path_src, 0x00, sizeof(vf_path_src));

    vf_path_dst[0].subtree_name = SVN_ST_PROTOCOL;
    vf_path_dst[0].index = relimitG.protIndex;
    vf_path_src[0].subtree_name = SVN_ST_PROTOCOL;
    vf_path_src[0].index = 0;

    vf_name.filename = relimitG.studyFile;
    if_error(VfOpen(VFFT_FILE, vf_name, O_RDWR, &vfd_dst))
    {
        printf("E: *** in VfOpen(%s) (%d,%s).\n",
               vf_name.filename, __LINE__, __FILE__);
        ForceExit(__FILE__, __LINE__);
    }

    status = VuiCopyProtocolFromFile(vfd_dst,
                            relimitG.protIndex,
                            relimitG.scratchPad,
                            0, 0, NULL);
    if (E_NO_ERROR != status)
    {
      return(E_ERROR);
    }
    
    VfClose(vfd_dst);
     TRACE_FUNC_EXIT("CopyScratchPadToStudyFile() : ");
    return(E_NO_ERROR);
}

/*************************************************************
* Method:    	createScratchPad
* Description: 	This function is used 
* Parameter: 	void  :
* Returns:   	int
*************************************************************/
int createScratchPad(void)
{
    VfDBHandle_t    vfd = NULL;
    VfDBName_t      vfName;
    status_t        status = E_ERROR;

    TRACE_FUNC_ENTER("createScratchPad() : ");
    // Open the Study file to which protocol has to be copied
    strcpy(relimitG.scratchPad, "/usr/tmp/PDTOOL.tmp_file");
    unlink(relimitG.scratchPad);
    vfName.filename = relimitG.scratchPad;
    status = VfOpen(VFFT_FILE, vfName, O_CREAT | O_RDWR, &vfd);
    if(E_NO_ERROR != status)
    {
        printf("Could not open the study file %s \n", relimitG.studyFile);
        return(E_ERROR);
    }

    if(E_NO_ERROR != VuiCopyProtocolFromFile(vfd, 0,
                               relimitG.studyFile,
                               relimitG.protIndex,
                               0, NULL))
    {
        VfClose(vfd);
        WriteToErrorLog("\n,error ",__LINE__,ERROR_LOGONLY);
        return(E_ERROR);
    }
    VfClose(vfd);
    TRACE_FUNC_EXIT("createScratchPad() : ");
    return(E_NO_ERROR);
}

/*************************************************************
* Method:    	getSeqName
* Description: 	This function is used 
* Parameter: 	char * pasFileName :
* Parameter: 	char * seqName :
* Returns:   	int
*************************************************************/
int getSeqName(char *pasFileName, char *seqName)

{
    status_t      status = E_ERROR;
    int           n = 0;                      /* number of varfield request block */
    VfDBHandle_t    vfd = NULL ;
    VfDBName_t      vfName;
    VfFieldSpec_t varfield[5];
    VfPathElem_t      path[PROT_PATH_LEVEL];
    int           num_found = 0;            /* number of varfields read */
    char          seqTemplate[200];       /* path to seqgen */

     TRACE_FUNC_ENTER("getSeqName() : ");

    vfName.filename = pasFileName;
    status = VfOpen(VFFT_FILE, vfName , O_RDONLY, &vfd);
    if(E_NO_ERROR != status)
    {
        return(E_ERROR);
    }

    /* read the name of the sequence */
    n = 0;
    VFF_SET_ARGS(varfield[n], SVN_PLN_TEMPLATE_SEQ, SVT_PLN_TEMPLATE_SEQ,
                 sizeof(seqTemplate) - 1, seqTemplate, VFO_VARIABLE_LEN);
    n++;

    path[0].subtree_name = SVD_ST_PROTOCOL;
    path[0].index = 0;
    status = VfGetFields (vfd, path,
                          SVD_ST_PROTOCOL,
                          varfield, n, &num_found);

    if ((E_NO_ERROR != status) || (1 != num_found))
    {
        return(E_ERROR);
    }

    n = 0;
    seqTemplate[varfield[n].size] = '\0';    // terminate
    n++;

    strcpy(seqName, seqTemplate);

    if (NULL != vfd)
    {
        VfClose(vfd);
    }
    TRACE_FUNC_EXIT("getSeqName() : ");
    return(E_NO_ERROR);
}


/*************************************************************
* Method:    	TraverseProt
* Description: 	This function is used 
* Parameter: 	void  :
* Returns:   	int
*************************************************************/
int TraverseProt(void)
{

    VfDBHandle_t            vfId = NULL;
    VfDBName_t              vfName;
    status_t                status = E_ERROR;
    VfPathElem_t            pathP[SERIES_PATH_LEVEL];
    int                     i = 0;
    int                     retVal = 0;
    int                     total = 0,active = 0;
//    int l_nSliceNumber,l_nScannedProtIdx;//fill value
    BITFLD_DECL(mask,   MAX_BITFLD);
    TRACE_FUNC_ENTER("TraverseProt() : ");

    fflush(stdout);
    
    total = MAX_BITFLD;
    BITFLD_INIT(mask,MAX_BITFLD,0);
    
    pathP[0].subtree_name = SVN_ST_PROTOCOL;
    pathP[0].index    = 0;
    
    pathP[1].subtree_name = SVN_ST_VISUALGROUP;
    pathP[1].index    = 0;
    
    vfName.filename =  relimitG.scratchPad;
    WriteToTraceLog("Opening ScratchPad\n","[TraverseProt]");
    fflush(stdout);
    status = VfOpen(VFFT_FILE, vfName,O_RDONLY, &vfId);
    if(E_NO_ERROR != status) {
        WriteToErrorLog("Could not open the study file\n",__LINE__,ERROR_LOGONLY);
        fflush(stdout);
        return(status);
    }

    status = VfScanSubtree(vfId,pathP,SVD_ST_PROTOCOL,
                          SVN_ST_VISUALGROUP,mask,&total,&active);
    if(E_NO_ERROR != status) {
        WriteToErrorLog("Error in getting VG nodes\n",__LINE__,ERROR_LOGONLY);
        fflush(stdout);
        return(status);
    }
    
    fprintf(stdout,"Total VG = %d\n",total);
    fflush(stdout);

    for( i = 0; i < total; i++) {
        pathP[1].index = i;
        fprintf(stdout,"PT complete path is (%d,%d),(%d,%d)\n",
                         pathP[0].subtree_name,pathP[0].index,
                         pathP[1].subtree_name,pathP[1].index);
        fflush(stdout);
        if(1 == relimitG.m_nMpr_Voi_flag){
        status= UpdateVector(pathP);
        if (E_NO_ERROR != status){
            break;
        }
        }else{
            status= UpdateOffsetForVOI(relimitG.m_nflag_scout_selection,pathP,relimitG.m_fcouchOffset);
            if (E_NO_ERROR != status){
                break;
        }
        }
        status = UpdateSliceThickness(relimitG.m_fscliceThck);
        if (E_NO_ERROR != status){
            break;
        }
        status = UpdateSliceGap(relimitG.m_fSliceGap);
        if (E_NO_ERROR != status){
            break;
        }
        status = UpdateSliceNumber(relimitG.m_nSliceNum);
        if (E_NO_ERROR != status){
            break;
        }
        status = FlUpdateLocatorImages(pathP,relimitG.m_nCenterSliceNum,relimitG.m_nScannedProtIdx,relimitG.m_nProcIdx);
        if (E_NO_ERROR != status ){
            break;
        }
        status = UpdatePlanStatus();
        if (E_NO_ERROR != status){
            break;
        }
        status = UpdateFov(relimitG.m_fov);
        if (E_NO_ERROR != status){
            break;
        }
    }

    VfClose(vfId);
    WriteToTraceLog("ScratchPad closed\n","[TraverseProt]");
    fflush(stdout);
    TRACE_FUNC_EXIT("TraverseProt() : ");
    return(status);
}

/*************************************************************
* Method:    	UpdateVector
* Description: 	GlocUpdateLocation updates the GL location in the tree by
*               updating the polar angle the azimuth angle and the magnitude.
*Assumptions:   Patient coordinate system negates X and Z from GL
*               modeling coordinate system. 
* Parameter: 	VfPathElem_t path :
* Returns:   	status_t
*************************************************************/
status_t UpdateVector(VfPathElem_t path[])
{
    float               offset[3] = {0.0};
    float               orientation[3] = {0.0};
    float               phase[3] = {0.0};
    float               *vec = NULL;
    plValT              value;
    status_t            plStatus =  E_NO_ERROR;
    char                *name = NULL;
    char l_csval[200] = {0};
    TRACE_FUNC_ENTER("UpdateVector() : ");
    fflush(stdout);

    offset[0] = (float)relimitG.v_offset.x;
    offset[1] = (float)relimitG.v_offset.y;
    offset[2] = (float)relimitG.v_offset.z;
    
    orientation[0] = (float)relimitG.v_slice.x;
    orientation[1] = (float)relimitG.v_slice.y;
    orientation[2] = (float)relimitG.v_slice.z;
    
    phase[0] = (float)relimitG.v_phase.x;
    phase[1] = (float)relimitG.v_phase.y;
    phase[2] = (float)relimitG.v_phase.z;
    
    value.itemCt = 1;

    fprintf(stdout,"PT complete path is (%d,%d),(%d,%d)\n",
                     path[0].subtree_name,path[0].index,
                     path[1].subtree_name,path[1].index);
    fflush(stdout);

    /* vgSliceOffsetVector */
    name = PL_VG_SLICE_OFFSET_VECTOR;
    vec = offset;
    plStatus = plGetVal(path,SVD_ST_VISUALGROUP,name,&value);
    if_error(plStatus){
        WriteToErrorLog("Error in plGetVal\n",__LINE__,ERROR_LOGONLY);
        fflush(stdout);
        return(plStatus);
    }
    PL_VAL_FLTVEC3_X(value) = vec[0];
    PL_VAL_FLTVEC3_Y(value) = vec[1];
    PL_VAL_FLTVEC3_Z(value) = vec[2];
  
    sprintf(l_csval,"Updated_Offset_Vector(x,y,z)-->(%f , %f , %f)\n",
        PL_VAL_FLTVEC3_X(value),
        PL_VAL_FLTVEC3_Y(value),
        PL_VAL_FLTVEC3_Z(value));
    WriteToTraceLog(l_csval,"UpdateVector()");

    plStatus = plPutVal(path,SVD_ST_VISUALGROUP,name,&value);
    if_error(plStatus){
        WriteToErrorLog("Error in plPutVal PL_VG_SLICE_OFFSET_VECTOR\n",__LINE__,ERROR_LOGONLY);
        fflush(stdout);
        return(plStatus);
    }
    if_error(RelimitProlim())
    {
        printf("Error RelimitProlim %s    %d\n", __FILE__, __LINE__);
        fflush(stdout);
        return(E_ERROR);
    }

    /* vgSliceOrientationVector */
    name = PL_VG_SLICE_ORI_VECTOR;
    vec = orientation;
    plStatus = plGetVal(path,SVD_ST_VISUALGROUP,name,&value);
    if_error(plStatus){
        fprintf(stdout, "Error in plGetVal %s    %d\n", __FILE__, __LINE__);
        fflush(stdout);
        return(plStatus);
    }
    PL_VAL_FLTVEC3_X(value) = vec[0];
    PL_VAL_FLTVEC3_Y(value) = vec[1];
    PL_VAL_FLTVEC3_Z(value) = vec[2];

    sprintf(l_csval,"Updated_Slice_Vector(x,y,z)-->(%f , %f , %f)\n",
        PL_VAL_FLTVEC3_X(value),
        PL_VAL_FLTVEC3_Y(value),
        PL_VAL_FLTVEC3_Z(value));
    WriteToTraceLog(l_csval,"UpdateVector()");

    plStatus = plPutVal(path,SVD_ST_VISUALGROUP,name,&value);
    if_error(plStatus){
        WriteToErrorLog("Error in plPutVal\n",__LINE__,ERROR_LOGONLY);
        fflush(stdout);
        return(plStatus);
    }
    if_error(RelimitProlim())
    {
       WriteToErrorLog("Error RelimitProlim PL_VG_SLICE_ORI_VECTOR\n",__LINE__,ERROR_LOGONLY);
        fflush(stdout);
        return(E_ERROR);
    }
    /* vgSlicePhaseVector */
    name = PL_VG_SLICE_PHASE_VECTOR;
    vec = phase;
    plStatus = plGetVal(path,SVD_ST_VISUALGROUP,name,&value);
    if_error(plStatus){
        fprintf(stdout, "Error in plGetVal %s    %d\n", __FILE__, __LINE__);
        fflush(stdout);
        return(plStatus);
    }
    PL_VAL_FLTVEC3_X(value) = vec[0];
    PL_VAL_FLTVEC3_Y(value) = vec[1];
    PL_VAL_FLTVEC3_Z(value) = vec[2];

    sprintf(l_csval,"Updated_Phase_Vector(x,y,z)-->(%f , %f , %f)\n",
        PL_VAL_FLTVEC3_X(value),
        PL_VAL_FLTVEC3_Y(value),
        PL_VAL_FLTVEC3_Z(value));
    WriteToTraceLog(l_csval,"UpdateVector()");
    plStatus = plPutVal(path,SVD_ST_VISUALGROUP,name,&value);
    if_error(plStatus){
        return(plStatus);
    }
    if_error(RelimitProlim())
    {
        printf("Error RelimitProlim PL_VG_SLICE_PHASE_VECTOR %s    %d\n", __FILE__, __LINE__);
        fflush(stdout);
        return(E_ERROR);
    }
    fflush(stdout);

     TRACE_FUNC_EXIT("UpdateVector() : ");
    return(E_NO_ERROR);
    
}

/*************************************************************
* Method:    	UpdateSliceThickness
* Description: 	This function is used 
* Parameter: 	float thickness :
* Returns:   	status_t
*************************************************************/
status_t UpdateSliceThickness(float thickness)
{
    plValT              value;
    status_t            plStatus = E_ERROR;
    VfPathElem_t      pathPT[PROT_PATH_LEVEL];
     char l_csval[200] = {0};
    TRACE_FUNC_ENTER("UpdateSliceThickness() : ");
    pathPT[0].subtree_name = SVN_ST_PROTOCOL;
    pathPT[0].index    = 0;
    
    value.itemCt = 1;
    value.u.value[0].real =thickness;// svoi_t[pos_count].thickness
    value.units = plUnitsTm;
    value.valueType = plValTFloat;
    
    sprintf(l_csval,"Slice thickness value -->%f \n",
        PL_VAL_REAL(value));
      WriteToTraceLog(l_csval,"UpdateSliceThickness()");
    plStatus = plPutVal(pathPT,SVD_ST_PROTOCOL,PL_SELECTED_SLICE_THK ,&value);
    if_error(plStatus){
        WriteToErrorLog("Error in plPutVal PL_SELECTED_SLICE_THK\n",__LINE__,ERROR_LOGONLY);
        return(plStatus);
    }
    TRACE_FUNC_EXIT("UpdateSliceThickness() : ");
    return(E_NO_ERROR);
}

/*************************************************************
* Method:    	UpdateSliceNumber
* Description: 	This function is used 
* Parameter: 	int number :
* Returns:   	status_t
*************************************************************/
status_t UpdateSliceNumber(int number)
{
    plValT              value;
    status_t            plStatus = E_ERROR;
    VfPathElem_t      pathPT[PROT_PATH_LEVEL];
    char l_csval[200] = {0};
     TRACE_FUNC_ENTER("UpdateSliceNumber() : ");
    
    pathPT[0].subtree_name = SVN_ST_PROTOCOL;
    pathPT[0].index    = 0;
    value.itemCt = 1;
    value.u.value[0].integer = (int)number;// svoi_t[pos_count].sliceNo
    value.units = plUnitsTunitless;
    value.valueType = plValTInt;
    sprintf(l_csval,"Slice number value -->%d \n",
        PL_VAL_INT(value));
     WriteToTraceLog(l_csval,"UpdateSliceNumber()");
    plStatus =plPutVal(pathPT,SVD_ST_PROTOCOL,PL_TOTAL_NUM_SLICES ,&value);
    if(E_NO_ERROR != plStatus){
        WriteToErrorLog("Error in plPutVal PL_VG_NUM_SLICES\n",__LINE__,ERROR_LOGONLY);
        return(plStatus);
    }    
    return(E_NO_ERROR);
}

/*************************************************************
* Method:    	FlUpdateLocatorImages
* Description: 	This function is used 
* Parameter: 	VfPathElem_t path :
* Parameter: 	int slice_number :
* Parameter: 	int f_nScannedProtIdx :
* Returns:   	status_t
*************************************************************/
static status_t FlUpdateLocatorImages(VfPathElem_t path[],int slice_number,int f_nScannedProtIdx,int f_nProcIdx)
{
    plValT       value;
    status_t     plStatus = 0;
    char l_csval[200] = {0};
     TRACE_FUNC_ENTER("FlUpdateLocatorImages() : ");
    value.u.value[0].pathelem.subtree_name =  SVN_ST_PROTOCOL;
    value.u.value[0].pathelem.index = f_nScannedProtIdx;//set value
    value.u.value[1].pathelem.subtree_name =  SVN_ST_PROCESSED;
    value.u.value[1].pathelem.index =  f_nProcIdx;
    value.u.value[2].pathelem.subtree_name =  SVN_ST_ECHO;
    value.u.value[2].pathelem.index =  0;
    value.u.value[3].pathelem.subtree_name = SVN_ST_POSITION;
    value.u.value[3].pathelem.index = (int)slice_number;
    value.itemCt = 4;
    value.units     = plUnitsTunitless;
    value.valueType = plValTPathElem;
    DB_PRINTF("D",("Locator image = \n",value.u.value[3].pathelem.index));
    sprintf(l_csval,"Locator image  -->(%d.%d/%d.%d/%d.%d/%d.%d)\n",
      SVN_ST_PROTOCOL,f_nScannedProtIdx,
      SVN_ST_PROCESSED,f_nProcIdx,
      SVN_ST_ECHO,0,
      SVN_ST_POSITION,slice_number);
     WriteToTraceLog(l_csval,"FlUpdateLocatorImages()");
    plStatus = plPutVal(path,SVD_ST_PROTOCOL,PL_LOCATOR_IMAGES ,&value);
    if_error(plStatus){
        WriteToErrorLog("Error in PL_STATUS\n",__LINE__,ERROR_LOGONLY);
        return(plStatus);
    }
    TRACE_FUNC_EXIT("FlUpdateLocatorImages() : ");
    return(E_NO_ERROR);
}
/*************************************************************
* Method:    	UpdateSliceGap
* Description: 	This function is used 
* Parameter: 	float gap :
* Returns:   	status_t
*************************************************************/
status_t UpdateSliceGap(float gap)
{
    plValT              value;
    status_t            plStatus = E_ERROR;    
    VfPathElem_t      pathPT[PROT_PATH_LEVEL];    
     char l_csval[200] = {0};
    TRACE_FUNC_ENTER("UpdateSliceGap() : ");
    pathPT[0].subtree_name = SVN_ST_PROTOCOL;
    pathPT[0].index    = 0;
    
    value.itemCt = 1;
    value.u.value[0].real = /*slice_gap_sl;*/gap;//svoi_t[pos_count].slicegap
    value.units = plUnitsTm;
    value.valueType = plValTFloat;
    sprintf(l_csval,"Slice gap value -->%f \n",
        PL_VAL_REAL(value));
     WriteToTraceLog(l_csval,"UpdateSliceGap()");
    plStatus = plPutVal(pathPT,SVD_ST_PROTOCOL,PL_SELECTED_GAP ,&value);
    if_error(plStatus){
        WriteToErrorLog("Error in plPutVal PL_SELECTED_GAP\n",__LINE__,ERROR_LOGONLY);
        return(plStatus);
    }    
    TRACE_FUNC_EXIT("UpdateSliceGap() : ");
    return(E_NO_ERROR);
}
/*************************************************************
* Method:    	UpdatePlanStatus
* Description: 	This function is used 
* Parameter: 	void  :
* Returns:   	status_t
*************************************************************/
status_t UpdatePlanStatus(void)
{
    plValT             value;
    status_t         plStatus=E_NO_ERROR;
    VfPathElem_t  pathPT[PROT_PATH_LEVEL];
     char l_csval[200] = {0};
    TRACE_FUNC_ENTER("UpdatePlanStatus() : ");
    pathPT[0].subtree_name = SVN_ST_PROTOCOL;
    pathPT[0].index    = 0;    
    
    /*PLN_STATUS update.*/
    plStatus = plGetVal(pathPT,SVD_ST_PROTOCOL,PL_STATUS,&value);
    if_error(plStatus){
        PL_VAL_BITFIELD(value) = 0;
    }
    PL_VAL_BITFIELD(value) |= VFC_PLN_STATUS_GLOC;
    
    plStatus = plPutVal(pathPT,SVD_ST_PROTOCOL,PL_STATUS,&value);
    if_error(plStatus){
        WriteToErrorLog("Error in PL_STATUS\n",__LINE__,ERROR_LOGONLY);
        return(plStatus);
    }
    TRACE_FUNC_EXIT("UpdatePlanStatus() : ");
    return(plStatus);
}
/*************************************************************
* Method:    	UpdateFov
* Description: 	This function is used 
* Parameter: 	f32vec2_t f_fFov :
* Returns:   	status_t
*************************************************************/
status_t UpdateFov(f32vec2_t f_fFov)
{
    plValT              value;
    status_t            plStatus;
    char                *name;
    int                     i=0;
    VfPathElem_t  pathPT[PROT_PATH_LEVEL];
     char l_csval[200] = {0};
     TRACE_FUNC_ENTER("UpdateFov() : ");
    /*FOV update*/
    pathPT[0].subtree_name = SVN_ST_PROTOCOL;
    pathPT[0].index    = 0;
    name = PL_PE_FOV;
    plStatus = plGetVal(pathPT,SVD_ST_PROTOCOL,name,&value);
    if_error(plStatus){
        WriteToErrorLog("Error in  plGetVal PL_PE_FOV\n",__LINE__,ERROR_LOGONLY);
        return(plStatus);
    }
    PL_VAL_REAL(value) = f_fFov.y;
    plStatus = plPutVal(pathPT,SVD_ST_PROTOCOL,name,&value);
    if_error(plStatus){
       WriteToErrorLog("Error in  plPutVal PL_PE_FOV\n",__LINE__,ERROR_LOGONLY);
        return(plStatus);
    }
    name = PL_RO_FOV;
    plStatus = plGetVal(pathPT,SVD_ST_PROTOCOL,name,&value);
    if_error(plStatus){
        WriteToErrorLog("Error in  plGetVal PL_PE_FOV\n",__LINE__,ERROR_LOGONLY);
        return(plStatus);
    }
    PL_VAL_REAL(value) = f_fFov.x;
    sprintf(l_csval,"FOV value(x,y)-->(%f , %f)\n",
        f_fFov.x,f_fFov.y);
    WriteToTraceLog(l_csval,"UpdateFov()");
    plStatus = plPutVal(pathPT,SVD_ST_PROTOCOL,name,&value);
    if_error(plStatus){
        WriteToErrorLog("Error in plPutVal PL_RO_FOV\n",__LINE__,ERROR_LOGONLY);
        return(plStatus);
    }
     TRACE_FUNC_EXIT("UpdateFov() : ");
    return(plStatus);
}
/*************************************************************
* Method:    	UpdateOffsetForVOI
* Description: 	This function is used 
* Parameter: 	int f_nflag_scout_selection :
* Parameter: 	VfPathElem_t path :
* Parameter: 	float f_fcouchOffset :
* Returns:   	status_t
*************************************************************/
status_t UpdateOffsetForVOI(int f_nflag_scout_selection,VfPathElem_t path[],float f_fcouchOffset)
{
    f32vec3_t               l_fVec1 = {0.0,0.0,0.0};
    f32vec3_t               l_fVec2 = {0.0,0.0,0.0};
    plValT              value;
    status_t            plStatus=E_NO_ERROR;
    char               *name = NULL;
    char l_csval[200] = {0};
  TRACE_FUNC_ENTER("UpdateOffsetForVOI() : ");
    
    l_fVec1 = relimitG.v_vec1;
    l_fVec2 = relimitG.v_vec2;
    name = PL_VG_SLICE_OFFSET_VECTOR;
    plStatus = plGetVal(path,SVD_ST_VISUALGROUP,name,&value);
    if_error(plStatus){
        WriteToErrorLog("Error in plGetVal PL_VG_SLICE_OFFSET_VECTOR\n",__LINE__,ERROR_LOGONLY);
        return(plStatus);
    }
    //Need to set only Z value of offset vector for scout
    if(0 == f_nflag_scout_selection){ 
        PL_VAL_FLTVEC3_X(value) = (l_fVec1.x + l_fVec2.x)/2;
        PL_VAL_FLTVEC3_Y(value) = (l_fVec1.y + l_fVec2.y)/2;
        PL_VAL_FLTVEC3_Z(value) = (l_fVec1.z + l_fVec2.z)/2;
    }else{
		PL_VAL_FLTVEC3_Z(value) = f_fcouchOffset;
    }
    
    
    sprintf(l_csval,"Offset_Vector(x,y,z)-->(%f , %f , %f)\n",
        PL_VAL_FLTVEC3_X(value),
        PL_VAL_FLTVEC3_Y(value),
        PL_VAL_FLTVEC3_Z(value));
    WriteToTraceLog(l_csval,"UpdateOffsetForVOI()");
    
    plStatus = plPutVal(path,SVD_ST_VISUALGROUP,name,&value);
    if_error(plStatus)
    {
        WriteToErrorLog("Error in plPutVal PL_VG_SLICE_OFFSET_VECTOR\n",__LINE__,ERROR_LOGONLY);
        return(plStatus);
    }
    TRACE_FUNC_EXIT("UpdateOffsetForVOI() : ");
    return(plStatus);
}
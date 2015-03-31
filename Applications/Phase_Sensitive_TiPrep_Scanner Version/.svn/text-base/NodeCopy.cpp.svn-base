// NodeCopy.cpp: implementation of the CNodeCopy class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NodeCopy.h"
#include "NodeCreate.h"
#include "TiPrepTrace.h"
#include "TiPrepLogHelper.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNodeCopy::CNodeCopy()
{
    /*TIPREP_TRACE(CNodeCopy::CNodeCopy);*/

}

CNodeCopy::~CNodeCopy()
{
    /*TIPREP_TRACE(CNodeCopy::~CNodeCopy);*/

}
/*************************************************************
* Method:       pdNodeCopy
* Description:  Copies a protocol from the given source file to the
*               destination file mentioned.
* Parameter:    char * srcfile : source file
* Parameter:    int srcProtIndex : source protocol index
* Parameter:    int dstProtIndex : destination protocol index
* Parameter:    int src_process_node : source process index
* Parameter:    int * dest_process_node : destination process index
* Returns:      status_t : E_NO_ERROR/E_ERROR
*************************************************************/
status_t CNodeCopy::pdNodeCopy(char *srcfile, int srcProtIndex, int dstProtIndex,
                    int src_process_node, int *dest_process_node)
{
    TIPREP_TRACE(CNodeCopy::pdNodeCopy,MRTraceMarker3);
    
    VfDBHandle_t    vfd;
    VfDBName_t      vfName;
    status_t        status = E_NO_ERROR;
    //int             protocol = -1;
    
    try{
        // Check whether the command line arguments are proper.
        if(srcfile == NULL || srcProtIndex < 0
            || dstProtIndex < 0) {
            DB_PRINTF("e", ("ERROR:from_file = %s ,srcProtIndex = %s,dstProtIndex = %d \n",
                srcfile, srcProtIndex, dstProtIndex));
            return(E_ERROR);
        }
        
        // Open the Study file to which protocol has to be copied
        vfName.filename = srcfile;
        status = VfOpen(VFFT_FILE, vfName, O_RDWR, &vfd);
        
        if(status != E_NO_ERROR) {
            DB_PRINTF("e", ("Could not open the study file %s \n", srcfile));
            return(E_ERROR);
        }
        
        status = CopyProcessedNode(vfd,
            dstProtIndex, // dst index
            vfd,
            srcProtIndex, // source index
            1, // Image group , this should be one always in order to copy processed node.
            /*&protocol,*/
            src_process_node,
            dest_process_node);
        //vilas_review_29-11-2012..status not used for error check
        VfClose(vfd);
        return(status);
    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog
            (_T("Exception occurred in Node Copy Class"), __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:       CopyProcessedNode
* Description:  Copies a process node from the given source file to the
*               destination file mentioned.
* Parameter:    VfDBHandle_t vfd_dst : destination study file handle
* Parameter:    int vf_dst_index : destination protocol index
* Parameter:    VfDBHandle_t vfd_src : Source study file handle
* Parameter:    int vf_src_index : source protocol index
* Parameter:    int image_group : image group
* Parameter:    int * protocol : protocol number
* Parameter:    int src_process_node : source process node
* Parameter:    int * dest_process_node : destination process node
* Returns:      int
*************************************************************/
int  CNodeCopy::CopyProcessedNode(VfDBHandle_t vfd_dst,
                       int      vf_dst_index,
                       VfDBHandle_t vfd_src,
                       int      vf_src_index,
                       int      image_group,
                       /*int      *protocol,*/
                       int      src_process_node,
                       int      *dest_process_node)
{
    TIPREP_TRACE(CNodeCopy::CopyProcessedNode,MRTraceMarker3);

    try{
    VfPathElem_t    vf_dst_path[MAX_DEPTH], vf_src_path[MAX_DEPTH];
    int         vf_dst_depth = 1;
    int         vf_src_depth = 1;
    status_t        status = E_NO_ERROR;    
    memset(vf_dst_path , 0 , sizeof(vf_dst_path)) ;
    memset(vf_src_path , 0 , sizeof(vf_src_path)) ;
    vf_src_path[0].subtree_name = SVN_ST_PROTOCOL;
    vf_src_path[0].index    = vf_src_index;
    vf_dst_path[0].subtree_name = SVN_ST_PROTOCOL;
    vf_dst_path[0].index    = vf_dst_index;
    vf_dst_depth = SVD_ST_PROTOCOL;     
    vf_src_depth = SVD_ST_PROTOCOL;     
    status = CopySubtree(vfd_dst,
                         vf_dst_path,
                         vf_dst_depth,
                         vfd_src,
                         vf_src_path,
                         vf_src_depth,
                         image_group,
                         7,
                         src_process_node,
                         dest_process_node); // 7 --- Processed node
    //vilas_review_29-11-2012..status not used for error check
    return(status);
    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog
            (_T("Exception occurred in Node Copy Class"), __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:       CopySubtree
* Description:  Copies a Subtree from the given source file to the
*               destination file mentioned.
* Parameter:    VfDBHandle_t vfd_dst : destination study file handle
* Parameter:    VfPathElem_t vf_dst_path :  destination path
* Parameter:    int vf_dst_depth : destination depth
* Parameter:    VfDBHandle_t vfd_src : Source study file handle
* Parameter:    VfPathElem_t vf_src_path : source path
* Parameter:    int vf_src_depth : source depth
* Parameter:    int image_group : image group
* Parameter:    VfName_t subtree_name : Depth to be copied
* Parameter:    int src_process_node : source process node
* Parameter:    int * dest_process_node : destination process node
* Returns:      int
*************************************************************/
int CNodeCopy::CopySubtree(VfDBHandle_t    vfd_dst,
                VfPathElem_t    vf_dst_path[],
                int     vf_dst_depth,
                VfDBHandle_t    vfd_src,
                VfPathElem_t    vf_src_path[],
                int     vf_src_depth,
                int     image_group,
                VfName_t        subtree_name,
                int         src_process_node,
                int         *dest_process_node)
{
    TIPREP_TRACE(CNodeCopy::CopySubtree,MRTraceMarker3);

    try{
    VfPathElem_t    vf_dst_path_save[MAX_DEPTH];
    VfPathElem_t    vf_src_path_save[MAX_DEPTH];
    int         total = 0;
    int         active = 0;
    int         total_dst = 0;
    int active_dst = 0;
    status_t        status = E_NO_ERROR;
    status_t        vfStat = E_NO_ERROR;
    VfFieldSpec_t   fieldList;
    int                 group[2];
    int                num = 0;
    BITFLD_DECL(mask,   MAX_BITFLD);
    BITFLD_DECL(mask_dst,   MAX_BITFLD);
    DB_FUNC_ENTER("[CopySubtree]:\t");
    total = MAX_BITFLD;
    total_dst = MAX_BITFLD;
    BITFLD_INIT(mask, MAX_BITFLD, 0);
    BITFLD_INIT(mask_dst, MAX_BITFLD, 0);
    DB_PRINTF("D", ("\n destination path %d%d %d%d %d%d and depth is %d",
                    vf_src_path[0].subtree_name,
                    vf_src_path[0].index,
                    vf_src_path[1].subtree_name,
                    vf_src_path[1].index,
                    vf_src_path[2].subtree_name,
                    vf_src_path[2].index, vf_src_depth));
    vfStat = VfScanSubtree(vfd_src, vf_src_path, vf_src_depth,
                           subtree_name, mask, &total, &active);
    if_error(vfStat) {
        DB_PRINTF("E", ("\n error in CopySubtree , line no %d", __LINE__));
        return(vfStat);
    }
    DB_PRINTF("I", ("\n In CopySubtree , line no %d and vfStat %d", __LINE__, vfStat));
    DB_PRINTF("I", ("\n In CopySubtree , total =  %d and active = %d", total, active));

    if(active == 0) {
        return(E_NO_ERROR);
    }

    DB_PRINTF("D", ("\n destination path %d%d %d%d %d%d and depth is %d",
                    vf_dst_path[0].subtree_name,
                    vf_dst_path[0].index,
                    vf_dst_path[1].subtree_name,
                    vf_dst_path[1].index,
                    vf_dst_path[2].subtree_name,
                    vf_dst_path[2].index, vf_dst_depth));
    vfStat = VfScanSubtree(vfd_src, vf_dst_path, vf_dst_depth,
                           subtree_name, mask_dst, &total_dst, &active_dst);

    if(vfStat == E_EXISTS) {
        total_dst = 0;
        active_dst = 0;
    } else {
        if_error(vfStat) {
            DB_PRINTF("E", ("\n error in CopySubtree , line no %d and vfStat %d", __LINE__, vfStat));
            return(vfStat);
        }
    }

    if(active == 0) {
        return(E_NO_ERROR);
    }

    VFF_SET_ARGS(fieldList,
                 SVN_GEN_VF_GROUP,
                 SVT_GEN_VF_GROUP,
                 0, 0,
                 VFO_ALLOCATE | VFO_VARIABLE_LEN);
    status = VfGetFields(vfd_dst,
                         vf_dst_path,
                         SVD_ST_PROTOCOL,
                         &fieldList,
                         1, &num);

    if(status == E_NO_ERROR && 1 == num) {
        memcpy(group, fieldList.buffer, fieldList.size);
        operator delete(fieldList.buffer);
    }

    DB_PRINTF("I", ("\n %d %d ", __LINE__, vfStat));
    DB_PRINTF("D", ("\n destination path %d%d %d%d %d%d and depth is %d",
                    vf_dst_path[0].subtree_name,
                    vf_dst_path[0].index,
                    vf_dst_path[1].subtree_name,
                    vf_dst_path[1].index,
                    vf_dst_path[2].subtree_name,
                    vf_dst_path[2].index, vf_dst_depth));
    vfStat = VfCreateSubtree(vfd_dst, vf_dst_path, vf_dst_depth,
                             subtree_name, -1);

    if(vfStat != E_EXISTS) {
        if_error(vfStat) {
            DB_PRINTF("E", ("\n error in CopySubtree , line no %d", __LINE__));
            return(vfStat);
        }
    }

    DB_PRINTF("E", ("\n %d %d ", __LINE__, vfStat));
    vf_dst_path[vf_dst_depth].subtree_name = subtree_name;
    vf_src_path[vf_src_depth].subtree_name = subtree_name;
    vf_dst_depth++;
    vf_src_depth++;
    memcpy(vf_dst_path_save, vf_dst_path, sizeof(VfPathElem_t) * vf_dst_depth);
    memcpy(vf_src_path_save, vf_src_path, sizeof(VfPathElem_t) * vf_src_depth);
    {
        if(BITFLD_ON(mask, src_process_node)) { /* add. V5.00 */
            memcpy(vf_dst_path, vf_dst_path_save,
                   sizeof(VfPathElem_t) * vf_dst_depth);
            memcpy(vf_src_path, vf_src_path_save,
                   sizeof(VfPathElem_t) * vf_src_depth);
            vf_dst_path[vf_dst_depth - 1].index = total_dst;
            vf_src_path[vf_src_depth - 1].index = src_process_node;
            status = VfAddBranch(vfd_dst, vf_dst_path, vf_dst_depth);
            //vilas_review_29-11-2012..multiple status are returned use them all for error logging
            if_error(status) {
                DB_PRINTF("E", ("\n error in CopySubtree , line no %d", __LINE__));
                return(status);
            }
#if 0
            status = VuiCopyNode(vfd_dst,
                                 vf_dst_path,
                                 vf_dst_depth,
                                 vfd_src,
                                 vf_src_path,
                                 vf_src_depth,
                                 image_group,
                                 group[1],
                                 vf_dst_path[SVD_ST_PROCESSED - 1].index + 1);
            if_error(status) {
                return(status);
            }
#else

            status = NodeCreate::pCopyNode(vfd_dst,
                               vf_dst_path,
                               vf_dst_depth,
                               vfd_src,
                               vf_src_path,
                               vf_src_depth,
                               image_group,
                               group[1],
                               vf_dst_path[SVD_ST_PROCESSED - 1].index + 1);
            if_error(status) {
                return(status);
            }
#endif
            *dest_process_node = vf_dst_path[1].index;
        }
    }
    
    return(E_NO_ERROR);
    }
    catch(...){
        CTiPrepLogHelper::WriteToErrorLog
            (_T("Exception occurred in Node Copy Class"), __LINE__, __FILE__);
        return E_ERROR;
    }
}

///////////////////////////////////////////////////////////////////////////////
// End of file
//////////////

// VfcNodeCopy.cpp: implementation of the CVfcNodeCopy class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VfcNodeCopy.h"
#include "TiPrepLogHelper.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVfcNodeCopy::CVfcNodeCopy()
{
    
}

CVfcNodeCopy::~CVfcNodeCopy()
{
    
}
/*************************************************************
* Method:    	CopyProcessNode
* Description: 	This function is used 
* Parameter: 	VfDBHandle_t vfd_dst :
* Parameter: 	VfPathElem_t vf_dst_path :
* Parameter: 	int vf_dst_depth :
* Parameter: 	VfDBHandle_t vfd_src :
* Parameter: 	VfPathElem_t vf_src_path :
* Parameter: 	int vf_src_depth :
* Returns:   	status_t
*************************************************************/
status_t CVfcNodeCopy::CopyProcessNode(
                                       VfDBHandle_t vfd_dst,
                                       VfPathElem_t vf_dst_path[],
                                       int vf_dst_depth,
                                       VfDBHandle_t vfd_src,
                                       VfPathElem_t vf_src_path[],
                                       int vf_src_depth
                                       
                                       
                                       )
{
    vuiCopyNode(vfd_dst,vf_dst_path,vf_dst_depth,vfd_src,vf_src_path,vf_src_depth,1,0);
    return 0;
}

/*************************************************************
* Method:    	vuiCopyNode
* Description: 	This function is used 
* Parameter: 	VfDBHandle_t vfd_dst :
* Parameter: 	VfPathElem_t vf_dst_path :
* Parameter: 	int vf_dst_depth :
* Parameter: 	VfDBHandle_t vfd_src :
* Parameter: 	VfPathElem_t vf_src_path :
* Parameter: 	int vf_src_depth :
* Parameter: 	int image_group :
* Parameter: 	int is_copy_from_scratch_pad :
* Returns:   	status_t
*************************************************************/
status_t CVfcNodeCopy::vuiCopyNode(
                                   VfDBHandle_t vfd_dst,
                                   VfPathElem_t vf_dst_path[],
                                   int vf_dst_depth,
                                   VfDBHandle_t vfd_src,
                                   VfPathElem_t vf_src_path[],
                                   int vf_src_depth,
                                   int image_group,
                                   int is_copy_from_scratch_pad
                                   )
{
    try{
        VfPathElem_t    vf_dst_path_save[MAX_DEPTH];
        VfPathElem_t    vf_src_path_save[MAX_DEPTH];
        VfFieldSpec_t*   vf_dst_req = NULL;
        VfFieldSpec_t*   vf_src_req = NULL;
        int				vf_dst_req_len = 0;
        int				vf_src_req_len = 0;
        VfName_t        subtrees[MAX_DEPTH] = {0};
        int				num_subtree = 0;
        register int    i = 0;
        status_t        vfStat = E_ERROR;
        status_t        status = E_ERROR;
        const int       MAX_COPY_LEN = vuiGetMaxCopyLength();
        const int       dst_size = sizeof(VfFieldSpec_t) * MAX_COPY_LEN;
        
        
        memset(vf_dst_path_save, 0, sizeof(VfPathElem_t) * MAX_DEPTH);
        memset(vf_src_path_save, 0, sizeof(VfPathElem_t) * MAX_DEPTH);
        
        memcpy(vf_dst_path_save, vf_dst_path, sizeof(VfPathElem_t) * MAX_DEPTH);
        memcpy(vf_src_path_save, vf_src_path, sizeof(VfPathElem_t) * MAX_DEPTH);
        
        vfStat = VfGetAllFields(vfd_src, vf_src_path, vf_src_depth, NULL,
            &vf_src_req, &vf_src_req_len);
        
        if_error(vfStat) {
            return vfStat;
        }
        
        if (vf_src_req_len == 0) {
            return E_NO_ERROR;
        }
        
        vf_dst_req = (VfFieldSpec_t*)malloc(dst_size);
        if (vf_dst_req == NULL) {
            return E_ERROR;
        }
        memset(vf_dst_req, 0, dst_size);
        
        
        num_subtree = 0;
        vf_dst_req_len = 0;
        
        for (i = 0; i < vf_src_req_len; i++) {
            
            if (vf_src_req[i].type == VFT_SUBTREE) {
                
                if (image_group == 0 && vf_src_req[i].name == SVN_ST_PROCESSED) {
                    
                } else {
                    subtrees[num_subtree] = vf_src_req[i].name;
                    num_subtree++;
                }
                
            } else if ((vf_src_depth == SVD_ST_IMAGE &&
                (vf_src_req[i].name == SVN_GEN_DATA
                || (vf_src_req[i].name >= SVN_GEN_DATA1 &&
                vf_src_req[i].name <= SVN_GEN_DATA127)))
                ||  /* MRS データはコピーしない */
                ((vf_src_req[i].name >= SVN_MRS_REF_RAW_DATA &&
                vf_src_req[i].name <= SVN_MRS_REF_RAW_DATA7) ||
                (vf_src_req[i].name >= SVN_MRS_REF_RAW_DATA8 &&
                vf_src_req[i].name <= SVN_MRS_REF_RAW_DATA127) ||
                (vf_src_req[i].name >= SVN_MRS_REF_WSIGNAL  &&
                vf_src_req[i].name <= SVN_MRS_REF_WSIGNAL7) ||
                (vf_src_req[i].name >= SVN_MRS_REF_WSIGNAL8 &&
                vf_src_req[i].name <= SVN_MRS_REF_WSIGNAL127) ||
                vf_src_req[i].name == SVN_MRS_WATER_DATA  ||
                vf_src_req[i].name == SVN_MRS_FS_DATA)
                || /*      位相補正生データはコピーしない V9.01 */
                (vf_src_req[i].name == SVN_GEN_PHASE_CORRECTION_DATA)
                || /* non copy some fields on save scratch pad to VF */
                (is_copy_from_scratch_pad == TRUE &&
                vuiIsNonCopyFieldOnSaveScratchPadToVF(vf_src_req[i].name) == TRUE) ||
                (vuiIsNonCopyField(vf_src_req[i].name) == TRUE)) {
            } else {
                VFF_SET_ARGSG(vf_dst_req[vf_dst_req_len],
                    vf_src_req[i].name,
                    vf_src_req[i].type,
                    vf_src_req[i].size,
                    NULL,
                    VFO_ALLOCATE | VFO_VARIABLE_LEN,
                    vf_src_req[i].group);
                vf_dst_req_len++;
                
                if (MAX_COPY_LEN <= vf_dst_req_len) {
                    vfStat = vuiCopyFields(vfd_src, vf_src_path, vf_src_depth,
                        vfd_dst, vf_dst_path, vf_dst_depth,
                        vf_dst_req, vf_dst_req_len, image_group);
                    
                    if_error(vfStat) {
                        free(vf_src_req);
                        free(vf_dst_req);
                        vf_src_req = vf_dst_req = NULL;
                        return vfStat;
                    }
                    
                    vf_dst_req_len = 0;
                    memset(vf_dst_req, 0, dst_size);
                }
            }
        }
        
        if (vf_dst_req_len > 0) {
            vfStat = vuiCopyFields(vfd_src, vf_src_path, vf_src_depth,
                vfd_dst, vf_dst_path, vf_dst_depth,
                vf_dst_req, vf_dst_req_len, image_group);
            
            if_error(vfStat) {
                free(vf_src_req);
                free(vf_dst_req);
                vf_src_req = vf_dst_req = NULL;
                return vfStat;
            }
        }
        
        free(vf_src_req);
        free(vf_dst_req);
        vf_src_req = vf_dst_req = NULL;
        
        for (i = 0; i < num_subtree; i++) {
            
            memcpy(vf_src_path, vf_src_path_save, sizeof(VfPathElem_t) * vf_src_depth);
            memcpy(vf_dst_path, vf_dst_path_save, sizeof(VfPathElem_t) * vf_dst_depth);
            
            status = vuiCopySubtree(vfd_dst,
                vf_dst_path,
                vf_dst_depth,
                vfd_src,
                vf_src_path,
                vf_src_depth,
                image_group,
                subtrees[i],
                is_copy_from_scratch_pad);
            
            if_error(status) {
                return status;
            }
        }
        
        return E_NO_ERROR;
}catch(...) {
    CTiPrepLogHelper::WriteToErrorLog
        (_T("Exception occurred in VF Node Copy Class"), __LINE__, __FILE__);
    return E_ERROR;
}
}

/*************************************************************
* Method:    	vuiCopySubtree
* Description: 	This function is used 
* Parameter: 	VfDBHandle_t vfd_dst :
* Parameter: 	VfPathElem_t vf_dst_path :
* Parameter: 	int vf_dst_depth :
* Parameter: 	VfDBHandle_t vfd_src :
* Parameter: 	VfPathElem_t vf_src_path :
* Parameter: 	int vf_src_depth :
* Parameter: 	int image_group :
* Parameter: 	VfName_t subtree_name :
* Parameter: 	int is_copy_from_scratch_pad :
* Returns:   	status_t
*************************************************************/
status_t CVfcNodeCopy::vuiCopySubtree(
                                      VfDBHandle_t vfd_dst,
                                      VfPathElem_t vf_dst_path[],
                                      int	vf_dst_depth,
                                      VfDBHandle_t vfd_src,
                                      VfPathElem_t vf_src_path[],
                                      int	vf_src_depth,
                                      int	image_group,
                                      VfName_t subtree_name,
                                      int is_copy_from_scratch_pad
                                      )
{
    try{
        VfPathElem_t	vf_dst_path_save[MAX_DEPTH];
        VfPathElem_t	vf_src_path_save[MAX_DEPTH];
        register int	i = 0;
        int			total = 0, active = 0;
        status_t		status = 0;
        status_t		vfStat = 0;
        BITFLD_DECL(mask,	MAX_BITFLD);
        DB_FUNC_ENTER("[vuiCopySubtree]:");
        
        total = MAX_BITFLD;
        BITFLD_INIT(mask, MAX_BITFLD, 0);
        vfStat = VfScanSubtree(vfd_src, vf_src_path, vf_src_depth,
            subtree_name, mask, &total, &active);
        
        if_error(vfStat) {
            return vfStat;
        }
        
        if (active == 0) {
            return E_NO_ERROR;
        }
        
        vfStat = VfCreateSubtree(vfd_dst, vf_dst_path, vf_dst_depth,
            subtree_name, -1);
        
        if (vfStat != E_EXISTS) {
            
            if_error(vfStat) {
                return vfStat;
            }
        }
        
        vf_dst_path[vf_dst_depth].subtree_name = subtree_name;
        vf_src_path[vf_src_depth].subtree_name = subtree_name;
        vf_dst_depth++;
        vf_src_depth++;
        memcpy(vf_dst_path_save, vf_dst_path, sizeof(VfPathElem_t) * vf_dst_depth);
        memcpy(vf_src_path_save, vf_src_path, sizeof(VfPathElem_t) * vf_src_depth);
        
        for (i = 0; i < total; i++) {
            if (BITFLD_ON(mask, i)) {
                memcpy(vf_dst_path, vf_dst_path_save, sizeof(VfPathElem_t) * vf_dst_depth);
                memcpy(vf_src_path, vf_src_path_save, sizeof(VfPathElem_t) * vf_src_depth);
                vf_dst_path[vf_dst_depth - 1].index = i;
                vf_src_path[vf_src_depth - 1].index = i;
                
                status = VfAddBranch(vfd_dst, vf_dst_path, vf_dst_depth);
                
                if_error(status) {
                    return status;
                }
                
                status = vuiCopyNode(vfd_dst,
                    vf_dst_path,
                    vf_dst_depth,
                    vfd_src,
                    vf_src_path,
                    vf_src_depth,
                    image_group,
                    is_copy_from_scratch_pad);
                if_error(status) {
                    return status;
                }
            }
        }
        
        return E_NO_ERROR;
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog
            (_T("Exception occurred in VF Node Copy Class"), __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:    	vuiGetMaxCopyLength
* Description: 	This function is used 
* Returns:   	int
*************************************************************/
int CVfcNodeCopy::vuiGetMaxCopyLength()
{
    static int max = 200;
    static int is_checked = FALSE;
    
    if (is_checked == FALSE) {
        FILE* fp = fopen("/MRMPlus/config/MRConfig/libvfToUi/VuiCopyMaxSize.dat", "rt");
        
        if (fp) {
            int n = 0;
            
            if (-1 != fscanf(fp, "%d", &n) && n > 0) {
                max = n;
            }
            
            fclose(fp);
        }
        
        is_checked = TRUE;
    }
    
    return max;
}

/*************************************************************
* Method:    	vuiCopyFields
* Description: 	This function is used 
* Parameter: 	VfDBHandle_t vfd_src :
* Parameter: 	VfPathElem_t * vf_src_path :
* Parameter: 	int vf_src_depth :
* Parameter: 	VfDBHandle_t vfd_dst :
* Parameter: 	VfPathElem_t * vf_dst_path :
* Parameter: 	int vf_dst_depth :
* Parameter: 	VfFieldSpec_t * vf_dst_req :
* Parameter: 	int vf_dst_req_len :
* Parameter: 	int image_group :
* Returns:   	status_t
*************************************************************/
status_t CVfcNodeCopy::vuiCopyFields(VfDBHandle_t vfd_src, VfPathElem_t* vf_src_path, int vf_src_depth,
                                     VfDBHandle_t vfd_dst, VfPathElem_t* vf_dst_path, int vf_dst_depth,
                                     VfFieldSpec_t* vf_dst_req, int vf_dst_req_len, int image_group)
{
    try{
        int i = 0;
        status_t vfStat = 0;
        time_t			vf_tm = 0;
        int				vf_num = 0;
        
        vfStat = VfGetFields(vfd_src, vf_src_path, vf_src_depth,
            vf_dst_req, vf_dst_req_len, &vf_num);
        
        if_error(vfStat) {
            clearBuffer(vf_dst_req, vf_dst_req_len);
            return vfStat;
        }
        
        for (i = 0; i < vf_dst_req_len; i++) {
            if (vf_dst_req[i].group > 0) {
                vf_dst_req[i].group = image_group;
            }
            
            VFF_SET_ARGSG(vf_dst_req[i],
                vf_dst_req[i].name,
                vf_dst_req[i].type,
                vf_dst_req[i].size,
                vf_dst_req[i].buffer,
                VFO_REPLACE,
                vf_dst_req[i].group);
        }
        
        vf_dst_depth++;
        vfStat = VfPutFields(vfd_dst, vf_dst_path, vf_dst_depth,
            vf_dst_req, vf_dst_req_len, &vf_num, &vf_tm);
        
        if_error(vfStat) {
        }
        
        clearBuffer(vf_dst_req, vf_dst_req_len);
        
        return vfStat;
    }catch(...) {
        CTiPrepLogHelper::WriteToErrorLog
            (_T("Exception occurred in VF Node Copy Class"), __LINE__, __FILE__);
        return E_ERROR;
    }
}

/*************************************************************
* Method:    	clearBuffer
* Description: 	This function is used 
* Parameter: 	VfFieldSpec_t * req :
* Parameter: 	int length :
* Returns:   	void
*************************************************************/
void CVfcNodeCopy::clearBuffer(VfFieldSpec_t* req, int length)
{ 
    int i = 0;
    
    for (i = 0; i < length; i++) {
        if (req[i].buffer != NULL) {
            free(req[i].buffer);
            req[i].buffer = NULL;
        }
    }
}
//****************************Method Header************************************

//Method Name   :vuiIsNonCopyFieldOnSaveScratchPadToVF

//Author        :TMSC/Tanoue

//Purpose       :

//*****************************************************************************
int CVfcNodeCopy::vuiIsNonCopyFieldOnSaveScratchPadToVF(const VfName_t name)
{
    const VfName_t name_list[] = {
        SVN_PRS_FILM_LAYOUT,
            SVN_PRC_AUTO_PERFUSION_ACMIN,
            SVN_PRC_AUTO_PERFUSION_ADT,
            SVN_PRC_AUTO_PERFUSION_AAT,
            SVN_PRC_AUTO_PERFUSION_ZOOM,
            SVN_PRC_AUTO_PERFUSION_THRESHOLD,
            SVN_PRC_AUTO_PERFUSION_MAPPING,
            SVN_PRC_AUTO_PERFUSION_BASE,
            SVN_PRC_AUTO_PERFUSION,
            SVN_EXE_ACQ_CONDITION,
            SVN_EXE_PRESCAN_RGN_ACTION,
            SVN_EXE_PRESCAN_RFL_ACTION,
            SVN_EXE_PRESCAN_FSY_ACTION,
            SVN_EXE_PRESCAN_STGC_RFOUT_RATIO1,
            SVN_EXE_PRESCAN_STGC_RFOUT_RATIO2,
            SVN_EXE_PRESCAN_STGC_REFLECT1,
            SVN_EXE_PRESCAN_STGC_REFLECT2,
            SVN_EXE_PAC_MATRIX,
            SVN_PRC_PE_SRTTBL,
            SVN_PRC_RO_INTERPOL_DATA,
            SVN_PRC_PE_SRTTBL2,
            SVN_EXE_RTSAR_AVERAGE_STANDARD_IN_PROTOCOL,
            SVN_EXE_PRESCAN_MTGC_RFOUT_RATIO1,
            SVN_EXE_PRESCAN_MTGC_RFOUT_RATIO2,
            SVN_EXE_PRESCAN_MTGC_REFLECT1,
            SVN_EXE_PRESCAN_MTGC_REFLECT2
    };
    
    const int name_list_len = sizeof(name_list) / sizeof(*name_list);
    int i = 0;
    
    for (i = 0; i < name_list_len; i++) {
        if (name_list[i] == name) {
            return TRUE;
        }
    }
    
    return FALSE;
}


/*************************************************************
* Method:    	vuiIsNonCopyField
* Description: 	This function is used 
* Parameter: 	const VfName_t name :
* Returns:   	int
*************************************************************/
int CVfcNodeCopy::vuiIsNonCopyField(const VfName_t name)
{
    switch (name) {
        
    case SVN_EXE_SEQUENCE_RGN_OFFSET:
    case SVN_EXE_RTSAR_EXPOSURE_HISTORY_WB:
    case SVN_EXE_RTSAR_EXPOSURE_HISTORY_HEAD:
    case SVN_EXE_RTSAR_EXPOSURE_HISTORY_PARTIAL:
        //	case SVN_EXE_RTSAR_EXPOSURE_HISTORY_PREAL:
        
        return TRUE;
        
    default:
        
        return FALSE;
    }
    
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// End of File
//////////////


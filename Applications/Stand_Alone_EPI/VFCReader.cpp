// VFCReader.cpp: implementation of the CVFCReader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EPI.h"
#include "VFCReader.h"
// #include <Shlwapi.h>
#include <IO.H>
#include <math.h>
#include "CommonDefinations.h"
#include "EPILogHelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

# define REALINDX 0
# define IMAGINARYINDX 1
# define M_PI       3.14159265358979323846  /* pi */

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
EXTERN_C {
    VfPathElem_t        g_sarch_node_path[] = {
        {SVN_ST_PROTOCOL,   0},
        {SVN_ST_SUBPROTOCOL,    0},
        {SVN_ST_SLICEGROUP, 0},
        {SVN_ST_SLICE,      0},
        {SVN_ST_IMAGE,      0}
    };

    VfPathElem_t        g_sarch_node_path2[] = {
        {SVN_ST_PROTOCOL,   0},
        {SVN_ST_PROCESSED,  0},
        {SVN_ST_ECHO,       0},
        {SVN_ST_POSITION,   0},
        {0,         0}
    };

};

CVFCReader::CVFCReader(): m_Path_index_top(NULL),
    m_Path_index_now(NULL),
    m_data(NULL), m_TotalRawNodes(0),
    m_selected_Image_Posotion(0),
    m_SelectedChannel(0)
{
    m_vfd = NULL;
}

CVFCReader::~CVFCReader()
{
    CleanUp();
    CloseDBFile();
}

void CVFCReader::checkFunction()
{
//    AfxMessageBox("Hi");
}

int CVFCReader::OpenDBFile()
{
    try {
        VfDBName_t l_name;
        int l_nstatus = 0;
        char* l_cpStudyfilename = NULL;
        CString l_csErrMsg ;
        l_cpStudyfilename = new char[128];

        if(NULL == l_cpStudyfilename) {
            CString l_csErrMsg = _T("Memory can not be allocated because of error");
            return(EPI_MEM_ALLOC);
        }

        sprintf(l_cpStudyfilename, "%s", m_csStudyFileName);
        l_name.filename = l_cpStudyfilename;
		m_vfd = NULL;
        l_nstatus = ::VfOpen(VFFT_FILE, l_name, /*O_RDONLY*/ O_RDWR , &m_vfd);

        if(E_NO_ERROR != l_nstatus) {
            l_csErrMsg.Format(_T("vfopen error"));

            if(NULL != l_cpStudyfilename) {
                delete []l_cpStudyfilename;
                l_cpStudyfilename = NULL;
            }

            if(E_FILE_NOT_FOUND == l_nstatus) {
                return EPI_VF_FILE_NOT_FOUND;
            }

            return(EPI_VF_OPEN);
        }

        if(NULL != l_cpStudyfilename) {
            delete []l_cpStudyfilename;
            l_cpStudyfilename = NULL;
        }
    } catch(...) {
        return E_ERROR;
    }

    return E_NO_ERROR;
}

int CVFCReader::CloseDBFile()
{
    try {
        if(m_vfd) {
            ::VfClose(m_vfd);
            m_vfd = NULL;
        }
    } catch(...) {
        CString l_csErrMsg = _T("Exception occured while closing vfDB");
        return EPI_ERR_EXCEPTION;
    }

    return E_NO_ERROR;
}

void CVFCReader::CleanUp()
{
    try {
        if(m_data) {
            for(int i = 0; i < m_TotalRawNodes; i++) {
                if(m_data[i]) {
                    free(m_data[i]);
                }
            }

            free(m_data);
        }

        m_data = NULL;

        if(m_Path_index_top) {
            Disp_node_count_unset(m_Path_index_top);
            m_Path_index_top = NULL;
        }
    } catch(...) {
        CEPILogHelper::WriteToErrorLog(_T("Exception occurred in CleanUp."), __LINE__, __FILE__);
    }
}

void CVFCReader::Disp_node_count_unset(Data_path_t *path_now)
{
#ifdef DEBUG
    printf("DDisp_node_count_unset IN\n");
#endif

    try {
        if(NULL != path_now) {
            if(NULL != path_now->next) {
                Disp_node_count_unset(path_now->next);
            }

            free((void *)path_now);
        }
    } catch(...) {
        CEPILogHelper::WriteToErrorLog(_T("Exception occurred in Disp_node_count_unset()."), __LINE__, __FILE__);
    }

    return;
}

status_t CVFCReader::Disp_node_count()
{
    try {
        CleanUp();

        if(NULL == m_vfd) {
            return EPI_VFC_FAILED;
        }

        m_Path_index_top = (Data_path_t *)malloc(sizeof(Data_path_t));
        memset(m_Path_index_top, 0x00, sizeof(Data_path_t));
        m_Path_index_num = 0;
        m_Path_index_now = m_Path_index_top;
        int     flg = 0;

        /*Count Slab Slice */
        if(Disp_node_count_set(m_vfd,
                               &m_Path_index_now,
                               &m_Path_index_num,
                               SVD_ST_PROTOCOL - 1,
                               (VfPathElem_t *)g_sarch_node_path) != E_NO_ERROR) {
            flg++;
        }

        if(Disp_node_count_set2(m_vfd,
                                &m_Path_index_now,
                                &m_Path_index_num,
                                SVD_ST_PROTOCOL - 1,
                                (VfPathElem_t *)g_sarch_node_path2) != E_NO_ERROR) {
            flg++;
        }

        if(flg == 2) {
            //clear the list control
        }

        m_Path_index_now = m_Path_index_top;
        int l_nStatus = Disp_Make_list(m_Path_index_num, m_Path_index_top);

        if(l_nStatus != E_NO_ERROR) {
            CEPILogHelper::WriteToErrorLog(_T("failed to create display list."), __LINE__, __FILE__);
            return EPI_VFC_FAILED;
        }

        l_nStatus = rawDataCheck();

        if(E_NO_ERROR != l_nStatus) {
            return l_nStatus;
        }
    } catch(...) {
        return E_ERROR;
    }

    return E_NO_ERROR;
}

int CVFCReader::Disp_node_count_set(VfDBHandle_t studyHandle, /*VfHndle*/ Data_path_t **path_now, /*path array*/ int *index_num, /*total index*/ int depth, /*start depth*/ VfPathElem_t *sarch_path /*use path element*/)
{
    int         i = 0;
    int         j = 0;
    int         k = 0;
    int         h = 0;
    int         n = 0;
    int         count = 0;
    int         se = 0;
    int         pe = 0;
    int         ro = 0;
    int         total = 0;
    int         total_sav = 0;
    int         active = 0;
    int         status = E_ERROR;
    int         num_done = 0;
    int         img_type = 0;
    VftTimeCtrlParam_t  time_type;
    int         dynamicFlag = 0;
    int         gateFlag = 0;
    int         multiphaseFlag = 0;
    int         scankind = 0;
    int         nStamps = 0;
    int         appcode = 0;
    int         hyepi_slice = 0;
    VftTimeCtrlParam_t  nRepete;
    int32_t         dim[3];
    int32_t         data_desc[3];
    Data_path_t     *path_save;
    VfFieldSpec_t       vf_req[5];
    unsigned int        *bitBranch;
    /*  BITFLD_DECL( bitBranch, 512 );*/

    try {
        /* Serch Total Branch And bitBranch*/
        for(total = 128, total_sav = 128;;) {
            bitBranch = (unsigned int *)malloc((total / (sizeof(unsigned int))));
            status = VfScanSubtree(studyHandle,
                                   sarch_path,
                                   depth,
                                   sarch_path[depth].subtree_name,
                                   bitBranch,
                                   &total,
                                   &active);

            if(status == E_NO_ERROR) {
            } else {
                free(bitBranch);
                CEPILogHelper::WriteToErrorLog(_T("VfScanSubtree failed."), __LINE__, __FILE__);
                return(EPI_VFC_FAILED);
            }

            if(total_sav >= total) {
                break;
            } else {
                total_sav   *= 2;
                total       = total_sav;
                free(bitBranch);
            }
        }

        /* requercev call for image node*/
        if(depth != (SVD_ST_IMAGE - 1)) {
            for(i = 0; i < total; i++) {
                if(BITFLD_ON(bitBranch, i)) {
                    sarch_path[ depth ].index = i;

                    if(Disp_node_count_set(studyHandle,
                                           path_now,
                                           index_num,
                                           depth + 1,
                                           sarch_path) == ERROR) {
                        continue;
                        /*return( ERROR );*/
                    }
                }
            }

            /* In image node*/
        } else {
            /* Loop For Total Numberof Branch */
            for(i = 0; i < total; i++) {
                /* if active branch */
                if(BITFLD_ON(bitBranch, i)) {
                    /* set image branch index */
                    sarch_path[SVD_ST_IMAGE-1].index = i;
                    n = 0;
                    VFF_SET_ARGS(vf_req[n],
                                 SVN_PLN_IMAGING_MODE,
                                 SVT_PLN_IMAGING_MODE,
                                 sizeof(img_type),
                                 &img_type, 0);
                    n++;
                    VFF_SET_ARGS(vf_req[n],
                                 SVN_PLN_APP_CODE,
                                 SVT_PLN_APP_CODE,
                                 sizeof(appcode),
                                 &appcode, 0);
                    n++;
                    VFF_SET_ARGS(vf_req[n],
                                 SVN_PLN_TIME_CTRL_PARAM,
                                 SVT_PLN_TIME_CTRL_PARAM,
                                 sizeof(time_type),
                                 &time_type, 0);
                    n++;
                    if_error(status = VfGetFields(studyHandle,
                                                  g_sarch_node_path,
                                                  SVD_ST_PROTOCOL,
                                                  vf_req,
                                                  n,
                                                  &num_done)) {
                        continue;
                    }
                    dynamicFlag = FALSE;
                    gateFlag    = 0;
                    multiphaseFlag  = FALSE;
                    n = 0;
                    VFF_SET_ARGS(vf_req[n],
                                 SVN_PLN_DYNAMIC_SCAN,
                                 SVT_PLN_DYNAMIC_SCAN,
                                 sizeof(int32_t),
                                 &dynamicFlag, 0);
                    n++;
                    VFF_SET_ARGS(vf_req[n],
                                 SVN_PLN_GATING_METHOD,
                                 SVT_PLN_GATING_METHOD,
                                 sizeof(int32_t),
                                 &gateFlag, 0);
                    n++;
                    VFF_SET_ARGS(vf_req[n],
                                 SVN_PLN_TIME_DIM_SCAN,
                                 SVT_PLN_TIME_DIM_SCAN,
                                 sizeof(int32_t),
                                 &multiphaseFlag, 0);
                    n++;
                    if_error(status = VfGetFields(studyHandle,
                                                  g_sarch_node_path,
                                                  SVD_ST_PROTOCOL,
                                                  vf_req,
                                                  n,
                                                  &num_done)) {
                        dynamicFlag = FALSE;
                        gateFlag    = 0;
                        multiphaseFlag  = FALSE;
                    }

                    if(gateFlag && multiphaseFlag == FALSE) {
                        scankind = SCAN_KIND_GATE;
                    } else if(gateFlag && multiphaseFlag == TRUE) {
                        scankind = SCAN_KIND_GATE_MPHASE;
                    } else if(dynamicFlag == TRUE) {
                        scankind = SCAN_KIND_DYNAMIC;
                    } else {
                        scankind = SCAN_KIND_NORMAL;
                    }

                    /* 3D image raw data */
                    if(img_type ==  VFC_IMAGING_MODE_3D) {
                        VFF_SET_ARGS(vf_req[0],
                                     SVN_GEN_DATA_DIM,
                                     SVT_GEN_DATA_DIM,
                                     sizeof(int32_t) * 3,
                                     dim, 0);
                        VFF_SET_ARGS(vf_req[1],
                                     SVN_GEN_DATA_DESC,
                                     VFT_INT32,
                                     sizeof(int32_t) * 3,
                                     data_desc, 0);
                        if_error(status = VfGetFields(studyHandle,
                                                      g_sarch_node_path,
                                                      SVD_ST_IMAGE,
                                                      vf_req,
                                                      2,
                                                      &num_done)) {
                            continue;
                        }

                        if(num_done != 2) {
                            continue;
                        }

                        if(appcode == VFC_APP_CODE_HYEPI) {
                            VFF_SET_ARGS(vf_req[0],
                                         SVN_PLN_SLICES_IN_SG,
                                         VFT_INT32,
                                         sizeof(hyepi_slice),
                                         &hyepi_slice, 0);
                            if_error(status = VfGetFields(studyHandle,
                                                          g_sarch_node_path,
                                                          SVD_ST_SLICEGROUP,
                                                          vf_req,
                                                          1,
                                                          &num_done)) {
                                continue;
                            }

                            if(num_done != 1) {
                                continue;
                            }
                        }

                        for(j = 0; j < 3; j++) {
                            switch(data_desc[j]) {
                                case VFC_DATA_RO:
                                    ro  = dim[j];
                                    break;
                                case VFC_DATA_PE:
                                    pe  = dim[j];
                                    break;
                                case VFC_DATA_SE:
                                    se  = dim[j];
                                    break;
                            }
                        }

                        if(appcode == VFC_APP_CODE_HYEPI && se == 1) {
                            se = hyepi_slice;
                            pe = pe / hyepi_slice;
                        }

                        /* 2D Time dimension format image raw data */
                    } else if(scankind == SCAN_KIND_DYNAMIC ||
                              scankind == SCAN_KIND_GATE_MPHASE) {
                        if(scankind == SCAN_KIND_DYNAMIC) {
                            VFF_SET_ARGS(vf_req[0],
                                         SVN_EXE_NUM_TIME_STAMP,
                                         SVT_EXE_NUM_TIME_STAMP,
                                         sizeof(int32_t),
                                         &nStamps, 0);
                            if_error(status = VfGetFields(studyHandle,
                                                          g_sarch_node_path,
                                                          SVD_ST_SUBPROTOCOL,
                                                          vf_req,
                                                          1,
                                                          &num_done)) {
                                continue;
                            }

                            if(num_done != 1) {
                                continue;
                            }

                            se = nStamps;
                        } else {
                            VFF_SET_ARGS(vf_req[0],
                                         SVN_PLN_TIME_CTRL_PARAM,
                                         SVT_PLN_TIME_CTRL_PARAM,
                                         sizeof(nRepete),
                                         &nRepete, 0);
                            if_error(status = VfGetFields(studyHandle,
                                                          g_sarch_node_path,
                                                          SVD_ST_PROTOCOL,
                                                          vf_req,
                                                          1,
                                                          &num_done)) {
                                continue;
                            }

                            if(num_done != 1) {
                                continue;
                            }

                            for(h = 0, count = 0; h < nRepete.numSegment; h++) {
                                count += nRepete.segment[h].repeat;
                            }

                            se = count;
                        }

                        VFF_SET_ARGS(vf_req[0],
                                     SVN_GEN_DATA_DIM,
                                     SVT_GEN_DATA_DIM,
                                     sizeof(int32_t) * 2,
                                     dim, 0);
                        VFF_SET_ARGS(vf_req[1],
                                     SVN_GEN_DATA_DESC,
                                     VFT_INT32,
                                     sizeof(int32_t) * 2,
                                     data_desc, 0);
                        if_error(status = VfGetFields(studyHandle,
                                                      g_sarch_node_path,
                                                      SVD_ST_IMAGE,
                                                      vf_req,
                                                      2,
                                                      &num_done)) {
                            continue;
                        }

                        if(num_done != 2) {
                            continue;
                        }

                        for(j = 0; j < 2; j++) {
                            switch(data_desc[j]) {
                                case VFC_DATA_RO:
                                    ro  = dim[j];
                                    break;
                                case VFC_DATA_PE:
                                    pe  = dim[j];
                                    break;
                            }
                        }/*END OF SWITCH*/

                        /* 2D Normal image raw data */
                    } else {
                        VFF_SET_ARGS(vf_req[0],
                                     SVN_GEN_DATA_DIM,
                                     SVT_GEN_DATA_DIM,
                                     sizeof(int32_t) * 2,
                                     dim, 0);
                        VFF_SET_ARGS(vf_req[1],
                                     SVN_GEN_DATA_DESC,
                                     VFT_INT32,
                                     sizeof(int32_t) * 2,
                                     data_desc, 0);
                        if_error(status = VfGetFields(studyHandle,
                                                      g_sarch_node_path,
                                                      SVD_ST_IMAGE,
                                                      vf_req,
                                                      2,
                                                      &num_done)) {
                            continue;
                        }

                        if(num_done != 2) {
                            continue;
                        }

                        for(j = 0; j < 2; j++) {
                            switch(data_desc[j]) {
                                case VFC_DATA_RO:
                                    ro  = dim[j];
                                    break;
                                case VFC_DATA_PE:
                                    pe  = dim[j];
                                    break;
                            }
                        }/*END OF SWITCH*/

                        se = 1;
                    }/*END OF  IF ELSE*/

                    /* Loop num raw data in 1 image data array */

                    for(k = 0; k < se; k++) {
                        path_save       = *path_now;
                        path_save->slice_of_3d  = k;
                        path_save->read_size    = ro;
                        path_save->pe_size  = pe;
                        path_save->tree_type    = RAW_DATA_TREE;
                        path_save->type_of_data = img_type;
                        path_save->type_of_time = scankind;
                        path_save->protocol = sarch_path[0].index;
                        path_save->subprotocol  = sarch_path[1].index;
                        path_save->slicegroupe  = sarch_path[2].index;
                        path_save->slice    = sarch_path[3].index;
                        path_save->image    = sarch_path[4].index;
                        path_save->next     = (Data_path_t *)malloc(sizeof(Data_path_t));

                        if(!path_save->next) {
                            free(bitBranch);
                            return(EPI_VFC_FAILED);
                        }

                        memset((char *)path_save->next, NULL, sizeof(Data_path_t));
                        *path_now       = path_save->next;

                        if(!(*index_num)) {
                            path_save->prev = NULL;
                        } else {
                            path_save->prev = path_save;
                        }

                        (*index_num)++;
                    }/*END OF FOR*/
                }/*END OF IF*/
            }/*END OF FOR*/
        }

        free(bitBranch);
    } catch(...) {
        return E_ERROR;
    }

    return(E_NO_ERROR);
}

int CVFCReader::Disp_node_count_set2(VfDBHandle_t studyHandle, /*VfHndle*/ Data_path_t **path_now, /*path array*/ int *index_num, /*total index*/ int depth, /*start depth*/ VfPathElem_t *sarch_path /*use path element*/)
{
    int     i = 0;
    int     j = 0;
    int     k = 0;
    int     pe = 0;
    int     ro = 0;
    int     total       = 128;
    int     total_sav   = 128;
    int     active = 0;
    int     status = 0;
    int     num_done = 0;
    int     img_type = 0;
    int     data_form = 0;
    int     scankind = SCAN_KIND_NORMAL;
    int32_t     dim[2];
    int32_t     data_desc[2];
    Data_path_t *path_save;
    VfFieldSpec_t   vf_req[5];
    unsigned int    *bitBranch;
    /*  BITFLD_DECL( bitBranch, 512 );*/

    try {
        /* Serch Total Branch And bitBranch*/
        for(;;) {
            bitBranch = (unsigned int *)malloc((total / (sizeof(unsigned int))));

            if((status = VfScanSubtree(studyHandle,
                                       sarch_path,
                                       depth,
                                       sarch_path[depth].subtree_name,
                                       bitBranch,
                                       &total,
                                       &active)) == E_NO_ERROR) {
#ifdef DEBUG
                printf("set2_scan_slice active = %d\n", active);
                printf("set2_scan_slice total = %d\n", total);
#endif
            } else {
#ifdef DEBUG
                printf("DDisp_node_count_set2 VfScanSubtree failure!\n");
                printf("DDisp_node_count_set2 depth = %d\n", depth);
                printf("DDisp_node_count_set2 sarch_path[0].index = %d\n", sarch_path[0].index);
                printf("DDisp_node_count_set2 sarch_path[1].index = %d\n", sarch_path[1].index);
                printf("DDisp_node_count_set2 sarch_path[2].index = %d\n", sarch_path[2].index);
                printf("DDisp_node_count_set2 sarch_path[3].index = %d\n", sarch_path[3].index);
#endif
                free(bitBranch);
                CEPILogHelper::WriteToErrorLog(_T("[Disp_node_count_set2]:VfScanSubtree failed."), __LINE__, __FILE__);
                return(EPI_VFC_FAILED);
            }

            if(total_sav >= total) {
                break;
            } else {
                total_sav   *= 2;
                total       = total_sav;
                free(bitBranch);
            }
        }

        /* requercev call for Position Node*/
        if(depth != (SVD_ST_POSITION - 1)) {
            for(i = 0; i < total; i++) {
                if(BITFLD_ON(bitBranch, i)) {
                    sarch_path[ depth ].index = i;

                    if(Disp_node_count_set2(studyHandle,
                                            path_now,
                                            index_num,
                                            depth + 1,
                                            sarch_path) == ERROR) {
                        continue;
                        /*return( ERROR );*/
                    }
                }
            }
        } else {
            for(i = 0; i < total; i++) {
                if(BITFLD_ON(bitBranch, i)) {
                    sarch_path[SVD_ST_POSITION-1].index = i;
#ifdef DEBUG
                    printf("DDisp_node_count_set2 sarch_path[0].index = %d\n", sarch_path[0].index);
                    printf("DDisp_node_count_set2 sarch_path[1].index = %d\n", sarch_path[1].index);
                    printf("DDisp_node_count_set2 sarch_path[2].index = %d\n", sarch_path[2].index);
                    printf("DDisp_node_count_set2 sarch_path[3].index = %d\n", sarch_path[3].index);
#endif
                    VFF_SET_ARGS(vf_req[0],
                                 SVN_PLN_IMAGING_MODE,
                                 SVT_PLN_IMAGING_MODE,
                                 sizeof(img_type),
                                 &img_type, 0);
                    if_error(status = VfGetFields(studyHandle,
                                                  sarch_path,
                                                  SVD_ST_PROTOCOL,
                                                  vf_req,
                                                  1,
                                                  &num_done)) {
#ifdef DEBUG
                        printf("VfGetFields error !\n");
#endif
                        CEPILogHelper::WriteToErrorLog(_T("VfGetFields error !\n"), __LINE__, __FILE__);
                        continue;
                    }
                    VFF_SET_ARGS(vf_req[0],
                                 SVN_GEN_DATA_DIM,
                                 SVT_GEN_DATA_DIM,
                                 sizeof(int32_t) * 2,
                                 dim, 0);
                    VFF_SET_ARGS(vf_req[1],
                                 SVN_GEN_DATA_DESC,
                                 VFT_INT32,
                                 sizeof(int32_t) * 2,
                                 data_desc, 0);
                    if_error(status = VfGetFields(studyHandle,
                                                  sarch_path,
                                                  SVD_ST_ECHO,
                                                  vf_req,
                                                  2,
                                                  &num_done)) {
#ifdef DEBUG
                        printf(" VfGetFields ERROR\n");
#endif
                        CEPILogHelper::WriteToErrorLog(_T("VfGetFields error !\n"), __LINE__, __FILE__);
                        continue;
                    }

                    if(num_done != 2) {
                        continue;
                    }

                    VFF_SET_ARGS(vf_req[0],
                                 SVN_GEN_DATA_FORM,
                                 SVT_GEN_DATA_FORM,
                                 sizeof(data_form),
                                 &data_form, 0);
                    if_error(status = VfGetFields(studyHandle,
                                                  sarch_path,
                                                  SVD_ST_PROCESSED,
                                                  vf_req,
                                                  1,
                                                  &num_done)) {
#ifdef DEBUG
                        printf(" VfGetFields ERROR\n");
#endif
                        CEPILogHelper::WriteToErrorLog(_T("VfGetFields error !\n"), __LINE__, __FILE__);
                        continue;
                    }

                    if(num_done != 1) {
                        continue;
                    }

                    ro  = dim[0];
                    pe  = dim[1];
#if 1

                    for(j = 0; j < 2; j++) {
                        switch(data_desc[j]) {
                            case VFC_DATA_RO:
                                ro  = dim[j];
#ifdef DEBUG
                                printf("Data Read Size = %d\n", dim[j]);
#endif
                                break;
                            case VFC_DATA_PE:
                                pe  = dim[j];
#ifdef DEBUG
                                printf("Data Phase Size = %d\n", dim[j]);
#endif
                                break;
                        }/*END OF SWITCH*/
                    }

#endif
                    path_save       = *path_now;
                    path_save->slice_of_3d  = k;
                    path_save->read_size    = ro;
                    path_save->pe_size  = pe;
                    /*
                    if( data_form != VFC_FORM_COMPLEX ){
                        path_save->read_size    = pe;
                        path_save->pe_size  = ro;
                    }
                    */
                    path_save->tree_type    = PROCESSED_TREE;
                    path_save->type_of_data = img_type;
                    path_save->type_of_time = scankind;
                    path_save->protocol = sarch_path[0].index;
                    path_save->processed    = sarch_path[1].index;
                    path_save->echo     = sarch_path[2].index;
                    path_save->position = sarch_path[3].index;
                    path_save->next     = (Data_path_t *)malloc(sizeof(Data_path_t));

                    if(!path_save->next) {
                        free(bitBranch);
                        return(EPI_VFC_FAILED);
                    }

                    memset((char *)path_save->next, NULL, sizeof(Data_path_t));
                    *path_now       = path_save->next;

                    if(!(*index_num)) {
                        path_save->prev = NULL;
                    } else {
                        path_save->prev = path_save;
                    }

                    (*index_num)++;
                }/*END OF IF*/
            }/*END OF FOR*/
        }

        free(bitBranch);
    } catch(...) {
        return E_ERROR;
    }

    return(E_NO_ERROR);
}

int CVFCReader::Disp_Make_list(int num , Data_path_t *Path_index_list)
{
    int     i;
    Data_path_t *list;
    char        txt1[16];
    char        txt2[16];
    char        txt3[16];
    char        txt4[16];
    char        txt5[16];
    char        txt6[16];

    try {
        if(num <= 0) {
            return E_ERROR;
        }

        if(m_data != NULL) {
            free((void*)m_data);
            m_data = NULL;
        }

        m_data = (char**)malloc(num * sizeof(char*));

        if(NULL == m_data) {
            return E_ERROR;
        }

        for(i = 0; i < num; i++) {
            m_data[i] = (char*)malloc(MAXIMAGE_LIST_LEN * sizeof(char));

            if(NULL == m_data[i]) {
                for(int k = 0; k < i; k++) {
                    free(m_data[i]);
                }

                free(m_data);
                m_data = NULL;
            } else {
                memset(m_data[i], 0x00, MAXIMAGE_LIST_LEN * sizeof(char));
            }
        }

        m_TotalRawNodes = num;
        list = Path_index_list;

        for(i  = 0;  i < num; i++) {
            if(list->tree_type == RAW_DATA_TREE) {
                sprintf(txt1, "1.%d", list->protocol);
                sprintf(txt2, "2.%d", list->subprotocol);
                sprintf(txt3, "3.%d", list->slicegroupe);
                sprintf(txt4, "5.%d", list->slice);
                sprintf(txt5, "6.%d", list->image);

                if(list->type_of_data == VFC_IMAGING_MODE_3D) {
                    sprintf(txt6,
                            "SN= %d",
                            list->slice_of_3d);
                } else if(list->type_of_time == SCAN_KIND_GATE) {
                    sprintf(txt6,
                            "SN= %d",
                            list->slice_of_3d);
                } else if(list->type_of_time == SCAN_KIND_GATE_MPHASE) {
                    sprintf(txt6,
                            "SN= %d",
                            list->slice_of_3d);
                } else if(list->type_of_time == SCAN_KIND_DYNAMIC) {
                    sprintf(txt6,
                            "SN= %d",
                            list->slice_of_3d);
                } else if(list->type_of_data == VFC_IMAGING_MODE_2D) {
                    sprintf(txt6,
                            "SN= %d",
                            list->slice_of_3d);
                } else {
                    sprintf(txt6, "SN= 0");
                }

                strcat(m_data[i], txt1);
                strcat(m_data[i], "/");
                strcat(m_data[i], txt2);
                strcat(m_data[i], "/");
                strcat(m_data[i], txt3);
                strcat(m_data[i], "/");
                strcat(m_data[i], txt4);
                strcat(m_data[i], "/");
                strcat(m_data[i], txt5);
                strcat(m_data[i], "/");
                strcat(m_data[i], txt6);
            } else {
                sprintf(txt1, "1.%d", list->protocol);
                sprintf(txt2, "7.%d", list->processed);
                sprintf(txt3, "8.%d", list->echo);
                sprintf(txt4, "9.%d", list->position);
                sprintf(txt5, "1.0");
                strcat(m_data[i], txt1);
                strcat(m_data[i], "/");
                strcat(m_data[i], txt2);
                strcat(m_data[i], "/");
                strcat(m_data[i], txt3);
                strcat(m_data[i], "/");
                strcat(m_data[i], txt4);
                // strcat( m_data[i], "/" );
                // strcat( m_data[i], txt5 );
            }

            memset(m_data[i] + strlen(m_data[i]), 0x20, 1);
            list = list->next;
        }
    } catch(...) {
        return E_ERROR;
    }

    return E_NO_ERROR;
}

char** CVFCReader::getData() const
{
    return m_data;
}

int CVFCReader::getTotalRawNodes() const
{
    return m_TotalRawNodes;
}

int CVFCReader::getNumberofChannel(int &f_nProtocolNo, int &f_nTotalChannel)
{
    try {
#if 0
        int     status;
        int     num_done;
        VfFieldSpec_t   vf_req[2];
        VfPathElem_t             l_imagePath[] = {
            {SVN_ST_PROTOCOL,   f_nProtocolNo},
            {SVN_ST_SUBPROTOCOL,    0},
            {SVN_ST_SLICEGROUP, 0},
            {SVN_ST_SLICE,      0},
            {SVN_ST_IMAGE,      0}
        };;
        VFF_SET_ARGS(vf_req[0],
                     SVN_EXE_NUM_PAC_SCAN,
                     SVT_EXE_NUM_PAC_SCAN,
                     sizeof(f_nTotalChannel),
                     &f_nTotalChannel, 0);
        status = VfGetFields(m_vfd,
                             l_imagePath,
                             SVD_ST_PROTOCOL,
                             vf_req,
                             1, &num_done);

        if((status != E_NO_ERROR) || (num_done != 1)) {
            CEPILogHelper::WriteToErrorLog(_T("Unable to get No of channel."), __LINE__, __FILE__);
            return (EPI_VFC_FAILED);
        }

#endif
        int     status;
        int     num_done;
        int     n = 0;
        VfFieldSpec_t   vf_req[2];
        VfPathElem_t l_imagePath[] = {
            {SVN_ST_PROTOCOL,   f_nProtocolNo},
            {SVN_ST_SUBPROTOCOL,    0},
            {SVN_ST_SLICEGROUP, 0},
            {SVN_ST_SLICE,      0},
            {SVN_ST_IMAGE,      0}
        };
        f_nTotalChannel = 0;

        for(int l_nChannelCnt = 0; l_nChannelCnt < 128; l_nChannelCnt++) {
            n = 0;
            num_done = 0;

            if(l_nChannelCnt == 0) {
                VFF_SET_ARGSG(vf_req[n], SVN_GEN_DATA, 0,
                              0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN, 0);
                n++;
            } else {
                VFF_SET_ARGSG(vf_req[n], (SVN_GEN_DATA + (20 + l_nChannelCnt - 1)), 0,
                              0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN, 0);
                n++;
            }

            l_imagePath[0].index = f_nProtocolNo;
            status = VfGetFields(m_vfd, l_imagePath, SVD_ST_IMAGE, vf_req,
                                 n, (int*)&num_done);

            if(status != 0 || num_done != n) {
                if(E_FILE_NOT_FOUND == status) {
                    CString l_csTemp;
                    l_csTemp.Format("getNumberofChannel :: Failed to get data for channel %d.", f_nTotalChannel);
                    CEPILogHelper::WriteToErrorLog(l_csTemp, __LINE__, __FILE__);
                    break;
                }

                break;
            }

            f_nTotalChannel++;
        }

        m_nTotalChannel = f_nTotalChannel;
    } catch(...) {
        return E_ERROR;
    }

    return E_NO_ERROR;
}
double* CVFCReader::ReadRawData(int &all_data , int &read_dim, double &img_scale, int &f_nDataType, BOOL f_bIntDataReq, int f_nChannelNum)
{
    int     j = 0;
    int     size = 0;
    int     status = 0;
    double      *buffer = NULL;
    int     num_done = 0;
    int     num_data = 0;
    int     data_type = 0;
    int     data_type_ORG = 0;
    int     data_form = 0;
    int     gr_num = 0;
    int     *gr_point  = NULL;
    int     gr_no = 0;
    VfFieldSpec_t   vf_req[2];
    int     gen_data = 0;
//     UpdatePathIndexNow();
//     UpdateRawPath();
    all_data    = 0;
    read_dim = 0;
    img_scale = 0.0;

    try {
        VFF_SET_ARGS(vf_req[0],
                     SVN_GEN_DATA_TYPE,
                     SVT_GEN_DATA_TYPE,
                     sizeof(data_type),
                     &data_type, 0);
        VFF_SET_ARGS(vf_req[1],
                     SVN_GEN_DATA_FORM,
                     SVT_GEN_DATA_FORM,
                     sizeof(data_form),
                     &data_form, 0);
        if_error(status = VfGetFields(m_vfd,
                                      g_sarch_node_path,
                                      SVD_ST_SUBPROTOCOL,
                                      vf_req,
                                      2,
                                      &num_done)) {
            CEPILogHelper::WriteToErrorLog(_T("VfGetFields failed to get SVN_GEN_DATA_TYPE or SVN_GEN_DATA_FORM"), __LINE__, __FILE__);
            return(NULL);
        }
        data_type_ORG   = data_type;
        data_type   = data_type & 0x00ffffff;
        f_nDataType = data_type;
        VFF_SET_ARGS(vf_req[0],
                     SVN_GEN_VF_GROUP,
                     SVT_GEN_VF_GROUP,
                     0,
                     0, VFO_ALLOCATE | VFO_VARIABLE_LEN);
        if_error(status = VfGetFields(m_vfd,
                                      g_sarch_node_path,
                                      SVD_ST_PROTOCOL,
                                      vf_req,
                                      1,
                                      &num_done)) {
            CEPILogHelper::WriteToErrorLog(_T("VfGetFields failed to get SVN_GEN_VF_GROUP."), __LINE__, __FILE__);
            return(NULL);
        }

        if(num_done == 1) {
            gr_num = vf_req[0].size / sizeof(int);

            if(gr_num >= 2) {
                gr_point = (int *)vf_req[0].buffer;
                gr_no = gr_point[1];
            }

//      free( ( void *)vf_req[0].buffer );
        }

        /*** if type of data don't match int32_t or int16_t or flt32_t, return above routine***/
        if(data_type != VFT_INT32 &&
                data_type != VFT_INT16 &&
                data_type != VFT_FLT32) {
            CEPILogHelper::WriteToErrorLog(_T("Data type don't match int32_t or int16_t or flt32_t."), __LINE__, __FILE__);
            return(NULL);
        }

        m_Path_index_now->form_of_data = data_form;
        vf_req[0].offset = 0;
//  switch( m_SelectedChannel){
//  case 1: gen_data = SVN_GEN_DATA; break;
//  case 2: gen_data = SVN_GEN_DATA1; break;
//  case 3: gen_data = SVN_GEN_DATA2; break;
//  case 4: gen_data = SVN_GEN_DATA3; break;
//  case 5: gen_data = SVN_GEN_DATA4; break;
//  case 6: gen_data = SVN_GEN_DATA5; break;
//  case 7: gen_data = SVN_GEN_DATA6; break;
//  case 8: gen_data = SVN_GEN_DATA7; break;
//  case 9: gen_data = SVN_GEN_DATA8; break;
//  case 10: gen_data = SVN_GEN_DATA9; break;
//  case 11: gen_data = SVN_GEN_DATA10; break;
//  case 12: gen_data = SVN_GEN_DATA11; break;
//  case 13: gen_data = SVN_GEN_DATA12; break;
//  case 14: gen_data = SVN_GEN_DATA13; break;
//  case 15: gen_data = SVN_GEN_DATA14; break;
//  case 16: gen_data = SVN_GEN_DATA15; break;
//  case 17: gen_data = SVN_GEN_DATA16; break;
//  case 18: gen_data = SVN_GEN_DATA17; break;
//  case 19: gen_data = SVN_GEN_DATA18; break;
//  case 20: gen_data = SVN_GEN_DATA19; break;
//  case 21: gen_data = SVN_GEN_DATA20; break;
//  case 22: gen_data = SVN_GEN_DATA21; break;
//  case 23: gen_data = SVN_GEN_DATA22; break;
//  case 24: gen_data = SVN_GEN_DATA23; break;
//  case 25: gen_data = SVN_GEN_DATA24; break;
//  case 26: gen_data = SVN_GEN_DATA25; break;
//  case 27: gen_data = SVN_GEN_DATA26; break;
//  case 28: gen_data = SVN_GEN_DATA27; break;
//  case 29: gen_data = SVN_GEN_DATA28; break;
//  case 30: gen_data = SVN_GEN_DATA29; break;
//  case 31: gen_data = SVN_GEN_DATA30; break;
//  case 32: gen_data = SVN_GEN_DATA31; break;
//  default: gen_data = SVN_GEN_DATA; break;
//  }

        if(TRUE == f_bIntDataReq) {
            if(f_nChannelNum == 0) {
                gen_data = SVN_GEN_DATA;
            } else {
                gen_data = (SVN_GEN_DATA + (20 + f_nChannelNum - 1));
            }
        } else {
            if(m_SelectedChannel == 0) {
                gen_data = SVN_GEN_DATA;
            } else {
                gen_data = (SVN_GEN_DATA + (20 + m_SelectedChannel - 1));
            }
        }

        /* 3D Slise */
        if(m_Path_index_now->type_of_data ==  VFC_IMAGING_MODE_3D ||
                m_Path_index_now->type_of_time == SCAN_KIND_GATE_MPHASE ||
                m_Path_index_now->type_of_time == SCAN_KIND_DYNAMIC) {
            if(data_type == VFT_INT16) {
                size    =   m_Path_index_now->read_size *
                            m_Path_index_now->pe_size   *
                            sizeof(int16_t) * 2;
            } else if(data_type == VFT_INT32) {
                size    =   m_Path_index_now->read_size *
                            m_Path_index_now->pe_size   *
                            sizeof(int32_t) * 2;
            } else if(data_type == VFT_FLT32) {
                size    =   m_Path_index_now->read_size *
                            m_Path_index_now->pe_size   *
                            sizeof(float) * 2;
            } else {
                size    =   m_Path_index_now->read_size *
                            m_Path_index_now->pe_size   *
                            sizeof(int32_t) * 2;
            }

            VFF_SET_ARGSG(vf_req[0],
                          gen_data,
                          data_type,
                          size,
                          0,
                          VFO_DATA_OFFSET | VFO_ALLOCATE | VFO_VARIABLE_LEN,
                          gr_no);
            VFF_SET_ARGSG(vf_req[1],
                          gen_data,
                          data_type_ORG,
                          size,
                          0,
                          VFO_DATA_OFFSET | VFO_ALLOCATE | VFO_VARIABLE_LEN,
                          gr_no);
            vf_req[0].offset = (size * m_Path_index_now->slice_of_3d);
#ifdef DEBUG
            printf("3D Slise size = %d\n", size);
#endif
        } else {
            VFF_SET_ARGSG(vf_req[0],
                          gen_data,
                          data_type,
                          0,
                          0,
                          VFO_ALLOCATE | VFO_VARIABLE_LEN,
                          gr_no);
            VFF_SET_ARGSG(vf_req[1],
                          gen_data,
                          data_type_ORG,
                          0,
                          0,
                          VFO_ALLOCATE | VFO_VARIABLE_LEN,
                          gr_no);
        }

        if_error(status = VfGetFields(m_vfd,
                                      g_sarch_node_path,
                                      SVD_ST_IMAGE,
                                      &vf_req[0],
                                      1,
                                      &num_done)) {
#ifdef DEBUG
            printf("VfGetFields error!(image data)\n");
#endif
            CEPILogHelper::WriteToErrorLog(_T("VfGetFields error!(image data)\n"), __LINE__, __FILE__);
            return(NULL);
        }

        if(num_done != 1 && vf_req[0].size <= 0) {
            if_error(status = VfGetFields(m_vfd,
                                          g_sarch_node_path,
                                          SVD_ST_IMAGE,
                                          &vf_req[1],
                                          1,
                                          &num_done)) {
                return(NULL);
            }

            if(num_done != 1 && vf_req[0].size <= 0) {
                CEPILogHelper::WriteToErrorLog(_T("VfGetFields error!(image data)\n"), __LINE__, __FILE__);
                return(NULL);
            } else {
                ;
            }
        }

        read_dim = m_Path_index_now->read_size;

        /* Change Double Data */
        if(data_type == VFT_INT16) {
            size    = vf_req[0].size * (sizeof(double) / sizeof(int16_t));
            buffer  = (double *)malloc(size);

            if(!buffer) {
                CEPILogHelper::WriteToErrorLog(_T("Memory allocation for buffer failed.(VFT_INT32)\n"), __LINE__, __FILE__);
                return(NULL);
            }

            num_data = vf_req[0].size / sizeof(int16_t);
#ifdef DEBUG
            printf("data_type == VFT_INT32"
                   "size = %d num_data = %d\n",
                   size,
                   num_data);
#endif

            for(j = 0; j < num_data; j++) {
                int16_t * tmp = NULL;
                tmp = (int16_t *)(vf_req[0].buffer);

                if(tmp) {
                    *(buffer + j) =
                        ((double)(*(tmp + j))) / SHRT_MAX;
                }
            }

            vf_req[0].size  *= (sizeof(int32_t) / sizeof(int16_t));
            img_scale = SHRT_MAX;
        } else if(data_type == VFT_INT32) {
            size    = vf_req[0].size * (sizeof(double) / sizeof(int32_t));
            buffer  = (double *)malloc(size);

            if(!buffer) {
                CEPILogHelper::WriteToErrorLog(_T("Memory allocation for buffer failed.(VFT_INT32)\n"), __LINE__, __FILE__);
                return(NULL);
            }

            num_data = vf_req[0].size / sizeof(int32_t);
#ifdef DEBUG
            printf("data_type == VFT_INT32 size = %d num_data = %d vf_req[0].size = %d\n",
                   size, num_data, vf_req[0].size);
#endif

            for(j = 0; j < num_data; j++) {
                int32_t * tmp = NULL;
                tmp = (int32_t *)(vf_req[0].buffer);

                if(tmp) {
                    *(buffer + j) =
                        ((double)(*(tmp + j))) / LONG_MAX;
                }
            }

            img_scale = LONG_MAX;
        } else if(data_type == VFT_FLT32) {
            size    = vf_req[0].size * (sizeof(double) / sizeof(float));
            buffer  = (double *)malloc(size);

            if(!buffer) {
                CEPILogHelper::WriteToErrorLog(_T("Memory allocation for buffer failed.(VFT_FLT32)\n"), __LINE__, __FILE__);
                return(NULL);
            }

            num_data = vf_req[0].size / sizeof(float);

            for(j = 0; j < num_data; j++) {
                float * tmp = NULL;
                tmp = (float *)(vf_req[0].buffer);

                if(tmp) {
                    *(buffer + j) =
                        ((double)(*(tmp + j))) / FLT_MAX;
                }
            }

            img_scale = FLT_MAX;
        } else {
            CEPILogHelper::WriteToErrorLog(_T("Data type don't match int32_t or int16_t or flt32_t."), __LINE__, __FILE__);
            return(NULL);
        }

#ifdef DEBUG
        //printf( "Raw Image Data Size = %d\n", vf_req[0].size );
        //printf( "Raw Image Data read_dim = %d\n", *read_dim );
        //printf( "Raw Image Data num_data = %d\n", num_data );
#endif
        all_data = num_data;
    } catch(...) {
        CEPILogHelper::WriteToErrorLog(_T("Unable to ReadRawData."), __LINE__, __FILE__);
    }

    return(buffer);
}
void CVFCReader::UpdatePathIndexNow()
{
    Data_path_t *list = NULL;

    try {
        list =  m_Path_index_top;

        for(int i = 0;  i < m_selected_Image_Posotion; i++) {
            list = list->next;
        }
    } catch(...) {
        CEPILogHelper::WriteToErrorLog(_T("Unable to UpdatePathIndexNow."), __LINE__, __FILE__);
    }

    m_Path_index_now = list;
}

void CVFCReader::UpdateRawPath()
{
    g_sarch_node_path[SVD_ST_PROTOCOL-1].index  = m_Path_index_now->protocol;
    g_sarch_node_path[SVD_ST_SUBPROTOCOL-1].index   = m_Path_index_now->subprotocol;
    g_sarch_node_path[SVD_ST_SLICEGROUP-1].index    = m_Path_index_now->slicegroupe;
    g_sarch_node_path[SVD_ST_SLICE-1].index     = m_Path_index_now->slice;
    g_sarch_node_path[SVD_ST_IMAGE-1].index     = m_Path_index_now->image;
}

void CVFCReader::SetSelectedPositionData(CString &cs)
{
    try {
        int idx = 0;

        if(m_data && m_TotalRawNodes > 0) {
            for(idx = 0; idx < m_TotalRawNodes; idx++) {
                CString tmp(_T(""));
                tmp.Format(_T("%s"), m_data[idx]);

                if(0 == tmp.CompareNoCase(cs)) {
                    break;
                }
            }
        }

        m_selected_Image_Posotion = idx;
        UpdatePathIndexNow();
        UpdateRawPath();
    } catch(...) {
        CEPILogHelper::WriteToErrorLog(_T("Unable to SetSelectedPositionData."), __LINE__, __FILE__);
    }
}
void CVFCReader::GetDataType(int &f_datatype)
{
    f_datatype = (m_Path_index_now->tree_type);
}
void CVFCReader::GetDataForm(int &f_dataForm)
{
    f_dataForm = (m_Path_index_now->form_of_data);
}


int CVFCReader::dumpRawdata(CString &f_csPath, const int &f_nDataForm,
                            double *&f_dData, CString &f_csFileName,
                            int &f_nFileSize, int &f_nChannelSelection)
{
    try {
        int type = 0;
        int form = 0;
        int all_data = 0;
        int read_dim ;
        double *data = NULL;
        double Image_scale = 0.0;
        int         data_size = 0;
        int         read_size = 0;
        int         phase_size = 0;
        int l_nStatus = E_NO_ERROR;
        CString fname(_T("EPIIMG"));
        m_SelectedChannel = f_nChannelSelection;
        SetSelectedPositionData(f_csPath);
        GetDataType(type);
        data = NULL;
        UpdatePathIndexNow();
        UpdateRawPath();
        int l_nDataType = VFT_INT32;
        data = ReadRawData(all_data, read_dim, Image_scale, l_nDataType);

        if(NULL == data) {
            CEPILogHelper::WriteToErrorLog(_T("Raw data reading failed."), __LINE__, __FILE__);
            return(EPI_VFC_FAILED);
        }

        if(read_dim <= 0) {
            if(data) {
                free(data);
                data = NULL;
            }

            if(f_dData) {
                free(f_dData);
                f_dData = NULL;
            }

            CEPILogHelper::WriteToErrorLog(_T("Raw data buffer is NULL or read dim <= 0."), __LINE__, __FILE__);
            return EPI_VFC_FAILED;
        }

        GetDataForm(form);
        CString l_csChannelName;
        l_csChannelName.Format("_Channel_%d", m_SelectedChannel);

        if(type == RAW_DATA_TREE && form == VFC_FORM_COMPLEX) {
            switch(f_nDataForm) {
                case VFC_FORM_IMGN_ONLY:
                    fname  = fname + _T("_Imaginary");
                    fname = fname + l_csChannelName;
                    data_size   = 2;
                    read_size   = read_dim;
                    phase_size  = all_data / (read_dim * 2);
                    l_nStatus = MakeImaginaryData(data, all_data, f_dData);
                    break;
                case VFC_FORM_REAL_ONLY:
                    fname  = fname + _T("_Real");
                    fname = fname + l_csChannelName;
                    data_size   = 2;
                    read_size   = read_dim;
                    phase_size  = all_data / (read_dim * 2);
                    l_nStatus = MakeRealData(data, all_data, f_dData);
                    break;
                case VFC_FORM_PHASE:
                    fname  = fname + _T("_Phase");
                    fname = fname + l_csChannelName;
                    data_size   = 1;
                    read_size   = read_dim;
                    phase_size  = all_data / (read_dim * 2);
                    l_nStatus = MakePhaseData(data, all_data, f_dData);
                    break;
                case VFC_FORM_MAGNITUDE:
                    fname  = fname + _T("_Magnitude");
                    fname = fname + l_csChannelName;
                    data_size   = 1;
                    read_size   = read_dim;
                    phase_size  = all_data / (read_dim * 2);
                    l_nStatus = MakeMagnitudeData(data, all_data, f_dData);
                    break;
                default:
                    break;
            }
        }

        if(E_NO_ERROR != l_nStatus || NULL == f_dData) {
            if(data) {
                free(data);
                data = NULL;
            }

            if(f_dData) {
                free(f_dData);
                f_dData = NULL;
            }

            CString l_csTemp;
            l_csTemp.Format("Complex data conversion failed for %d data form.", f_nDataForm);
            CEPILogHelper::WriteToErrorLog(l_csTemp, __LINE__, __FILE__);
            return E_ERROR;
        }

        f_nFileSize = read_size * phase_size * sizeof(double);
        CString csdim(_T(""));
        csdim.Format(_T("_%dx%d"), read_size, phase_size);
        fname = fname + csdim;
        f_csFileName = fname;

        if(data) {
            free(data);
            data = NULL;
        }
    } catch(...) {
        return 1;
    }

    return 0;
}

int CVFCReader::MakeMagnitudeData(double * inBuf , int numdata, double *&outBuf)
{
    try {
        int     i;
        int     j;
        double      tmp;
//    double        *buf = NULL;
        outBuf = (double *)malloc((numdata / 2) * sizeof(double));

        if(!(outBuf)) {
            CEPILogHelper::WriteToErrorLog("Memory allocation for MakeMagnitudeData() failed for outBuf  )", __LINE__, __FILE__);
            return NULL;
        }

        for(i = 0, j = 0; i < numdata; i += 2, j++) {
            if((inBuf + i) && (inBuf + i + 1)) {
                tmp  = sqrt(
                           (*(inBuf + i))      * (*(inBuf + i))
                           + (*(inBuf + i + 1))  * (*(inBuf + i + 1))
                       );

                if((outBuf + j)) {
                    *(outBuf + j) = tmp;
                }
            }
        }
    } catch(...) {
        return E_ERROR;
    }

    return E_NO_ERROR;
}

int CVFCReader::MakePhaseData(double * inBuf , int numdata, double *&outBuf)

{
    try {
        int     i;
        int     j;
        double      tmp;
//     double       *buf = NULL;
        double      calc_value1;
        double      calc_value2;
        double      calc_value3;
        double      calc_value4;
        outBuf = (double *)malloc((numdata / 2) * sizeof(double));

        if(!(outBuf)) {
            CEPILogHelper::WriteToErrorLog("Memory allocation in MakePhaseData() failed for outBuf", __LINE__, __FILE__);
            return NULL;
        }

        for(i = 0, j = 0; i < numdata; i += 2, j++) {
            if((inBuf + i) && (inBuf + i + 1)) {
                calc_value1 = cos((double)((*(inBuf + i)) * M_PI) / 180.0);
                calc_value2 = sin((double)((*(inBuf + i)) * M_PI) / 180.0);
                calc_value3 = cos((double)((*(inBuf + i + 1)) * M_PI) / 180.0);
                calc_value4 = sin((double)((*(inBuf + i + 1)) * M_PI) / 180.0);
                tmp = atan2(((calc_value2 * calc_value3) - (calc_value1 * calc_value4)),
                            ((calc_value1 * calc_value3) + (calc_value2 * calc_value4)));
                tmp = (tmp / M_PI) * 180.0;
                /*      tmp /= ( double )SHRT_MAX;*/

                if((outBuf + j)) {
                    *(outBuf + j) = tmp;
                }
            }
        }
    } catch(...) {
        return E_ERROR;
    }

    return E_NO_ERROR;
}


int CVFCReader::MakeRealData(double * inBuf , int numdata, double *&outBuf)
{
    try {
        int     i;
        int     j;
        //double      tmp;
//     double       *buf = NULL;
        outBuf = (double *)malloc((numdata / 2) * sizeof(double));

        if(!(outBuf)) {
            CEPILogHelper::WriteToErrorLog("Memory allocation in MakeRealData() failed for outBuf", __LINE__, __FILE__);
            return NULL;
        }

        for(i = REALINDX, j = 0; i < numdata; i += 2, j++) { //start with zero
//             if((inBuf + i)) {
//                 tmp  = *(inBuf + i);
// 
//                 if((outBuf + j)) {
//                     *(outBuf + j) = tmp;
//                 }
//             }
			outBuf[j] = inBuf[i];
        }
    } catch(...) {
        return E_ERROR;
    }

    return E_NO_ERROR;
}

int CVFCReader::MakeImaginaryData(double * inBuf , int numdata, double *&outBuf)
{
    try {
        int     i;
        int     j;
//        double      tmp;
//     double       *buf = NULL;
        outBuf = (double *)malloc((numdata / 2) * sizeof(double));

        if(NULL == outBuf) {
            CEPILogHelper::WriteToErrorLog("Memory allocation in MakeImaginaryData() failed for outBuf ", __LINE__, __FILE__);
            return EPI_MEM_ALLOC;
        }

        if(!(outBuf)) {
            return NULL;
        }

        for(i = IMAGINARYINDX, j = 0; i < numdata; i += 2, j++) { //starts with "1"
//             if((inBuf + i + IMAGINARYINDX)) {
//                 tmp  = *(inBuf + i + IMAGINARYINDX);
// 
//                 if((outBuf + j)) {
//                     *(outBuf + j) = tmp;
//                 }
//             }
			outBuf[j] = inBuf[i];
        }
    } catch(...) {
        return E_ERROR;
    }

    return E_NO_ERROR;
}

int CVFCReader::MakeProcessImageData(double * inBuf , int numdata , double *&outBuf)
{
    try {
        int     i;
        double      tmp;
//     double       *buf = NULL;
        outBuf = (double *)malloc((numdata) * sizeof(double));

        if(!(outBuf)) {
            return NULL;
        }

        for(i = REALINDX; i < numdata; i += 1) {  //start with zero
            if((inBuf + i)) {
                tmp  = *(inBuf + i);

                if((outBuf + i)) {
                    *(outBuf + i) = tmp;
                }
            }
        }
    } catch(...) {
        return E_ERROR;
    }

    return E_NO_ERROR;
}
int CVFCReader::protTraverse(const int &f_ProtIdx, int &f_nSubProtNo, int &f_nSubGrpNo,
                             int &f_nSliceNo, int &f_nSliceImgNo)
{
    try {
        status_t                l_nstatus;
        VfPathElem_t pathP[5] = { SVN_ST_PROTOCOL,   0, /* This path order should be maintained */
                                  SVN_ST_SUBPROTOCOL, 0,
                                  SVN_ST_SLICEGROUP, 0,
                                  SVN_ST_SLICE,      0,
                                  SVN_ST_IMAGE,      0
                                };
        int                     i, j, k/*, m*/;
//      int                     retVal = 0;
        pathP[0].index = f_ProtIdx;
        //    Get No of subprotocols.
        l_nstatus = GetSubNodes(pathP, SVD_ST_PROTOCOL, f_nSubProtNo);

        if(l_nstatus != E_NO_ERROR) {
//          CString l_cserrmsg("No SunProtocol data");
//          CEPILogHelper::WriteToErrorLog(l_cserrmsg,__LINE__,__FILE__);
            return(EPI_VFC_FAILED);
        }

        if(0 == f_nSubProtNo) {
            printf("\n No ImageData");
            CString l_cserrmsg("No SunProtocol data");
            CEPILogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
            return(EPI_VFC_FAILED);
        }

        //    Get no of slice groups in each subprotocol.
        for(i = 0; i < f_nSubProtNo; i++) {
            pathP[SVD_ST_SUBPROTOCOL -1].index = i;
            l_nstatus = GetSubNodes(pathP, SVD_ST_SUBPROTOCOL, f_nSubGrpNo);

            if(l_nstatus != E_NO_ERROR) {
//              CString l_cserrmsg("No SunProtocol data");
//              CEPILogHelper::WriteToErrorLog(l_cserrmsg,__LINE__,__FILE__);
                return(EPI_VFC_FAILED);
            }

            if(0 == f_nSubGrpNo) {
                CString l_cserrmsg("No Slice group present.");
                CEPILogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
                return(EPI_VFC_FAILED);
            }

            for(j = 0; j < f_nSubGrpNo; j++) {
                pathP[SVD_ST_SLICEGROUP -1].index = j;
                l_nstatus = GetSubNodes(pathP, SVD_ST_SLICEGROUP, f_nSliceNo);

                if(l_nstatus != E_NO_ERROR) {
//                  CString l_cserrmsg("No SunProtocol data");
//                  CEPILogHelper::WriteToErrorLog(l_cserrmsg,__LINE__,__FILE__);
                    return(EPI_VFC_FAILED);
                }

                if(0 == f_nSliceNo) {
                    CString l_cserrmsg("No Slice present.SVN_ST_SLICE");
                    CEPILogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
                    return(EPI_VFC_FAILED);
                }

                for(k = 0; k < f_nSliceNo; k++) {
                    pathP[SVD_ST_SLICE -1].index = k;
                    l_nstatus = GetSubNodes(pathP, SVD_ST_SLICE, f_nSliceImgNo);

                    if(l_nstatus != E_NO_ERROR) {
//                      CString l_cserrmsg("No SunProtocol data");
//                      CEPILogHelper::WriteToErrorLog(l_cserrmsg,__LINE__,__FILE__);
                        return(EPI_VFC_FAILED);
                    }

                    if(0 == f_nSliceImgNo) {
                        CString l_cserrmsg("No Slice image present.SVN_ST_IMAGE");
                        CEPILogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
                        return(EPI_VFC_FAILED);
                    }

//                  for(m = 0; m < f_nSliceImgNo; m++)
//                  {
//                      pathP[SVD_ST_SLICE ].index = m;
//                  }
                }
            }
        }

        return(E_NO_ERROR);
    } catch(...) {
        return E_ERROR;
    }
}
int CVFCReader::GetSubNodes(VfPathElem_t f_pathP[],
                            int f_curLevel,
                            int &f_ntotalsubnodes)
{
    int l_ntotalSubNodes = 256;
    BITFLD_DECL(vfBranches, 256);
    BITFLD_INIT(vfBranches, 256, 0);

    try {
        int l_nactiveSubNodes = 0;
        status_t l_ret = E_NO_ERROR;
        l_ret = ::VfScanSubtree(m_vfd, f_pathP, f_curLevel,
                                f_pathP[f_curLevel].subtree_name, vfBranches, &l_ntotalSubNodes,
                                &l_nactiveSubNodes);

        if(E_NO_ERROR != l_ret) {
            CString l_cserrmsg;
            l_cserrmsg.Format("VfScanSubtree Error. Error returned by VfScanSubtree %d", l_ret);
            CEPILogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
            f_ntotalsubnodes = -1;
            return(EPI_VFC_FAILED) ;
        } else {
            //dummy else
        }
    } catch(...) {
        CString l_cserrmsg("EXCEPTION occurred in VfScanSubtree.");
        CEPILogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
        f_ntotalsubnodes = -1;
        return(EPI_ERR_EXCEPTION);
    }

    f_ntotalsubnodes = l_ntotalSubNodes;
    return(E_NO_ERROR);
}

int CVFCReader::getProtocolData(VfPathElem_t f_pathP[],
                                int &f_nNumRepetition,
                                int &f_nDataForm,
                                int &f_nDataType,
                                int &f_nDataHeight,
                                int &f_nDataWidth)
{
    try {
        status_t                 status;
        int                      n;      /* number of varfield request block */
        VfFieldSpec_t            varfield[5];    /* varfield */
        int                      num_found;  /* number of varfields read */
        int32_t                  dataDim[2];  /* Data dimensions */
        int32_t			         data_desc[3];/*Data description*/
        n = 0;
        VFF_SET_ARGS(varfield[n], SVN_PLN_REPETITIONS, SVT_PLN_REPETITIONS,
                     sizeof(f_nNumRepetition), &f_nNumRepetition, 0);
        n++;
        status = VfGetFields(m_vfd,
                             f_pathP,
                             SVD_ST_PROTOCOL,
                             varfield, n, &num_found);

        if((status != E_NO_ERROR) || (num_found != n)) {
            CEPILogHelper::WriteToErrorLog(_T("Failed to get SVN_PLN_REPETITIONS."), __LINE__, __FILE__);
            return (EPI_VFC_FAILED);
        }

        // Get Data Form and Data Type.
        n = 0;
        num_found = 0;
        VFF_SET_ARGS(varfield[n], SVN_GEN_DATA_FORM, SVT_GEN_DATA_FORM,
                     sizeof(f_nDataForm), &f_nDataForm, 0);
        n++;
        VFF_SET_ARGS(varfield[n], SVN_GEN_DATA_TYPE, SVT_GEN_DATA_TYPE,
                     sizeof(f_nDataType), &f_nDataType, 0);
        n++;
        status = VfGetFields(m_vfd,
                             f_pathP,
                             SVD_ST_SUBPROTOCOL,
                             varfield, n, &num_found);

        if((status != E_NO_ERROR) || (num_found != n)) {
            CEPILogHelper::WriteToErrorLog(_T("Failed to get SVN_GEN_DATA_TYPE or SVN_GEN_DATA_FORM."), __LINE__, __FILE__);
            return (EPI_VFC_FAILED);
        }

        n = 0;
        num_found = 0;
        VFF_SET_ARGS(varfield[n], SVN_GEN_DATA_DIM, SVT_GEN_DATA_DIM,
                     sizeof(int32_t) * 2, &dataDim, 0);
        n++;
        status = VfGetFields(m_vfd,
                             f_pathP,
                             SVD_ST_IMAGE,
                             varfield, n, &num_found);

        if(status != E_NO_ERROR || num_found != n) {
            CEPILogHelper::WriteToErrorLog(_T("Failed to get SVN_GEN_DATA_DIM."), __LINE__, __FILE__);
            return (EPI_VFC_FAILED);
        }

        n = 0;
        num_found = 0;
        VFF_SET_ARGS(varfield[n], SVN_GEN_DATA_DESC, SVT_GEN_DATA_DESC,
            sizeof(int32_t) * 3, &data_desc, 0);
        n++;
        status = VfGetFields(m_vfd,
            f_pathP,
            SVD_ST_IMAGE,
            varfield, n, &num_found);
        
        if(status != E_NO_ERROR || num_found != n) {
            CEPILogHelper::WriteToErrorLog(_T("Failed to get SVN_GEN_DATA_DIM."), __LINE__, __FILE__);
            return (EPI_VFC_FAILED);
        }
        for( int j = 0; j < 3; j++ ){
            switch( data_desc[j] ){                
            case VFC_DATA_RO:
                f_nDataWidth	= dataDim[j];
                break;
            case VFC_DATA_PE:
                f_nDataHeight	= dataDim[j];
                break;
            }
        }
        //sizeof( int32_t )*2
    } catch(...) {
        return E_ERROR;
    }

    return E_NO_ERROR;
}

int CVFCReader::getRawData(VfPathElem_t vfPath[],
                           int &f_nDataHeight,
                           int &f_nDataWidth,
                           int &f_nMultiplier,
                           const int &f_nChannelNo,
                           int &f_nDataTypeSize,
                           int &f_nNumRepetition,
                           void *&f_cRawData, long &f_nImgRawDataSize, int &f_nDataType)
{
    try {
        status_t                 status;
        int l_nDataType = 0;
        int l_nDataForm = 0;
        int n = 0;
        int num_found = 0;
        VfFieldSpec_t varfield[5];
        status = getProtocolData(vfPath, f_nNumRepetition, l_nDataForm, l_nDataType, f_nDataHeight, f_nDataWidth);

        if(E_NO_ERROR != status) {
            //Error
            CEPILogHelper::WriteToErrorLog(_T("Failed to get protocol info."), __LINE__, __FILE__);
            return status;
        }

        status = getImageSize(l_nDataType, l_nDataForm,                             
                              f_nDataTypeSize, f_nMultiplier);

        if(E_NO_ERROR != status) {
            CEPILogHelper::WriteToErrorLog(_T("Failed to get image size."), __LINE__, __FILE__);
            return EPI_VFC_FAILED;
        }

        m_SelectedChannel = f_nChannelNo;

        if(f_nChannelNo == 0) {
            VFF_SET_ARGSG(varfield[n], SVN_GEN_DATA, 0,
                          0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN, 0);
            n++;
        } else {
            VFF_SET_ARGSG(varfield[n], (SVN_GEN_DATA + (20 + f_nChannelNo - 1)), 0,
                          0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN, 0);
            n++;
        }

        status = VfGetFields(m_vfd, vfPath, SVD_ST_IMAGE, varfield,
                             n, (int*)&num_found);

        if(E_NO_ERROR != status) {
            CString l_csTemp;
            l_csTemp.Format("Failed to get SVN_GEN_DATA for channel %d.", f_nChannelNo);
            CEPILogHelper::WriteToErrorLog(l_csTemp, __LINE__, __FILE__);
            return EPI_VFC_FAILED;
        }

        f_nImgRawDataSize = f_nDataHeight * f_nDataWidth * f_nMultiplier * f_nDataTypeSize;
        f_nDataType = l_nDataType;
        f_cRawData = (void*)malloc(f_nImgRawDataSize);


        if(NULL == f_cRawData) {
            CEPILogHelper::WriteToErrorLog(_T("Memory allocation failed for raw data."), __LINE__, __FILE__);
            return EPI_MEM_ALLOC;
        }
        memset(f_cRawData,0x00,f_nImgRawDataSize);
        memcpy(f_cRawData, varfield[0].buffer, f_nImgRawDataSize);
    } catch(...) {
        return E_ERROR;
    }

    return E_NO_ERROR;
}

int CVFCReader::getImageSize(const int &f_nDataType,
                             const int &f_nDataForm,
                             int &f_nDataTypeSize, 
                             int &f_nMultiplier)
{
    try {

        switch(f_nDataType) {
            case VFT_INT16:
                f_nDataTypeSize = sizeof(int16_t);
                break;
            case VFT_INT32:
                f_nDataTypeSize = sizeof(int32_t);
                break;
            case VFT_FLT32:
                f_nDataTypeSize = sizeof(flt32_t);
                break;
            default:
                return (E_ERROR);
        }
        switch(f_nDataForm) {
            case VFC_FORM_MAGNITUDE:
            case VFC_FORM_PHASE:
            case VFC_FORM_REAL_ONLY:
            case VFC_FORM_IMGN_ONLY:
            case VFC_FORM_POLAR:
                f_nMultiplier = 1;
                break;
            case VFC_FORM_COMPLEX:
                f_nMultiplier = 2;
                break;
            default:
                return (E_ERROR);
        }
    } catch(...) {
        return E_ERROR;
    }

    return E_NO_ERROR;
}
int CVFCReader::getProtAcqOrder(int *f_narray, int &f_nacqOrderSize)
{
    try {
        status_t      status = E_NO_ERROR;
        VfFieldSpec_t fieldSpec[1] = {0};
        int           numDone = 0;
        int32_t       arq_order[256] = {0} ;
        VFF_SET_ARGS(fieldSpec[0], SVN_PLN_PROT_ACQ_ORDER, VFT_INT32,
                     sizeof(arq_order), arq_order, VFO_VARIABLE_LEN);
        status = VfGetFields(m_vfd, NULL, SVD_ST_STUDY, fieldSpec, 1, &numDone);

        if((status != E_NO_ERROR) || (numDone != 1)) {
            //Error
            return(E_ERROR);
        }

        f_nacqOrderSize = 0;
        f_nacqOrderSize = fieldSpec[0].size / sizeof(int);
        memcpy(f_narray, arq_order, fieldSpec[0].size);
    } catch(...) {
        return E_ERROR;
    }

    return(E_NO_ERROR);
}
int CVFCReader::CheckIsProtocolScanned(int &f_nprotIndx)
{
    VfPathElem_t    NodePath[1] = {0};
    VfFieldSpec_t   VfReq[1] = {0};
    int32_t         acq_status = 0;
    int             num_get = 0;
    int  n = 0;
    int sts = E_NO_ERROR;
    NodePath[0].subtree_name = SVN_ST_PROTOCOL;
    NodePath[0].index = f_nprotIndx;
    n = 0 ;

    try {
        VFF_SET_ARGS(VfReq[n],
                     SVN_EXE_PROTOCOL_ACQ_STATUS,
                     SVT_EXE_PROTOCOL_ACQ_STATUS,
                     sizeof(acq_status), &acq_status, 0);
        n++;
        sts = VfGetFields(m_vfd, NodePath,
                          SVD_ST_PROTOCOL, VfReq, n, &num_get);

        if(E_NO_ERROR != sts) {
            return(-1);
        }
    } catch(...) {
        return 1;
    }

    return(acq_status);
}
#define ALPHA_NUMERIC_LARGE 64
#define ALPHA_NUMERIC_MEDIUM 52
#define ALPHA_NUMERIC_SMALL 26

int CVFCReader::getLabelAlpha(int protIndx, CString &l_csStr)
{
    try {
        char *alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        int  status = E_NO_ERROR ;
        int     numDone = 0;
        int  i = 0;
        int j = 0;
        VfFieldSpec_t   reqFlds[1] = {0};
        int  n = 0;
        char str1[32];
        // DB_FUNC_ENTER("[getLabelAlpha]\t");
        n = 0;
        VFF_SET_ARGS(reqFlds[n], SVN_PLN_PROT_ACQ_ORDER, SVT_PLN_PROT_ACQ_ORDER,
                     0, NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);
        n++ ;
        status = VfGetFields(m_vfd, NULL, 0, reqFlds, n, (int *)&numDone) ;

        if(E_NO_ERROR == status && n == numDone) {
            int32_t *protAcqNums = (int32_t *) reqFlds[0].buffer;
            int32_t size = reqFlds[0].size / sizeof(int32_t);

            for(i = 0; i < size; i++) {
                if(protAcqNums[i] == protIndx) {
                    break;
                }
            }

            if(i == size) {
                status = -1;
            }

            //free(reqFlds[0].buffer);
        } else {
            status = -1;
        }

        if(E_NO_ERROR == status) {
            j = i + 1;

            if(i < ALPHA_NUMERIC_SMALL) {
                sprintf(str1, "%c", alpha[i]);
            } else if(i >= ALPHA_NUMERIC_SMALL && i < ALPHA_NUMERIC_MEDIUM) {
                i = i - ALPHA_NUMERIC_SMALL;
                sprintf(str1, "%c%c", alpha[i], alpha[i]);
            } else if(i >= ALPHA_NUMERIC_MEDIUM && i < ALPHA_NUMERIC_LARGE) {
                i = i - ALPHA_NUMERIC_MEDIUM;
                sprintf(str1, "%c%c%c", alpha[i], alpha[i], alpha[i]);
            }
        } else {
            return(status);
        }

        l_csStr.Format("%s", str1);
    } catch(...) {
        return 1;
    }

    return(0);
}

int CVFCReader::getProtocolInfo(protInfo_t *&f_protData, int &f_nTotalProtocol)
{
    try {
        int l_nProtArray[128] = {0};
        int l_nAcqSize = 0;
        int l_nStatus = getProtAcqOrder(l_nProtArray, l_nAcqSize);

        if(E_NO_ERROR != l_nStatus) {
            return E_ERROR;
        }

        f_protData = new protInfo_t[l_nAcqSize];
		int totalscannedprots = 0;
        for(int l_nCnt = 0; l_nCnt < l_nAcqSize; l_nCnt++) {
            if(CheckIsProtocolScanned(l_nProtArray[l_nCnt]) == VFC_ACQ_STATUS_SUCCESS) {
                CString l_csTemp("");
                CString l_csSeq("");
                getLabelAlpha(l_nProtArray[l_nCnt], l_csTemp);
                f_protData[l_nCnt].protocolNum = l_nProtArray[l_nCnt];
                f_protData[l_nCnt].csProtocolName = l_csTemp;
                GetSeqName(l_nProtArray[l_nCnt], l_csSeq);

                if(l_csTemp.IsEmpty() == TRUE) {
                    CString l_cserrmsg("Failed to get sequence name for protocol %d ", l_nProtArray[l_nCnt]);
                    CEPILogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
                    return(EPI_VFC_FAILED);
                }

                f_protData[l_nCnt].sequencename = l_csSeq;
				totalscannedprots++;
            }
        }

       // f_nTotalProtocol = l_nAcqSize;
		 f_nTotalProtocol = totalscannedprots;
    } catch(...) {
        return E_ERROR;
    }

    return E_NO_ERROR;
}
int CVFCReader::vftoDUMPDATATYPE(void *dataBuffer, void *dftBuf, int data_type, int numPoints)
{
    if(NULL == dataBuffer || NULL == dftBuf){
        return E_ERROR;
    }
    switch(data_type) {
        case VFT_INT16: {
            int16_t *inP = (int16_t *)dataBuffer;
            DUMPDATATYPE *outP = (DUMPDATATYPE *)dftBuf;
            int i;

            for(i = 0; i < numPoints; i++) {
                outP[i] = (DUMPDATATYPE)inP[i];
            }
        }
        break;
        case VFT_INT32: {
            int32_t *inP = (int32_t *)dataBuffer;
            DUMPDATATYPE *outP = (DUMPDATATYPE *)dftBuf;
            int i;

            for(i = 0; i < numPoints; i++) {
                outP[i] = (DUMPDATATYPE)inP[i];
            }
        }
        break;
        case VFT_FLT32:
            memcpy(dftBuf, dataBuffer, numPoints * sizeof(DUMPDATATYPE));
            break;
        default:
            return (E_ERROR);
    }

    return 0;
}

/*
if (TRUE != l_bRawFlag)
{
CEPILogHelper::WriteToErrorLog(_T("Selected study file does not contains Raw data.")))
return EPI_NO_RAW_DATA;
}

*/

BOOL CVFCReader::IsProcessed(CString &cs)
{
    int cnt = 0;
    cnt = cs.Find("9.");

    if(cnt < 0) {
        return FALSE;
    } else {
        return TRUE;
    }
}

int CVFCReader::rawDataCheck()
{
    try {
        int l_bRawChk = FALSE;

        for(int l_nCnt_i = 0; l_nCnt_i < m_TotalRawNodes; l_nCnt_i++) {
            CString strItem(_T(""));

            if(NULL != m_data[l_nCnt_i]) {
                strItem.Format(_T("%s"), m_data[l_nCnt_i]);
            }

            if(FALSE == IsProcessed(strItem)) {
                l_bRawChk = TRUE;
            }
        }

        if(TRUE != l_bRawChk) {
            CEPILogHelper::WriteToErrorLog(_T("Selected study file does not have Raw data."), __LINE__, __FILE__);
            return EPI_NO_RAW_DATA;
        }
    } catch(...) {
        return E_ERROR;
    }

    return E_NO_ERROR;
}

int CVFCReader::protocolCheckEPI(int f_nProtIdx)
{
    status_t            status;
    int             numDone;
    VfFieldSpec_t       fieldSpec[3];
    VfPathElem_t        path[2];
    int                 i;
    status = E_ERROR;
    path[0].subtree_name = SVN_ST_PROTOCOL;
    path[0].index = f_nProtIdx;
    // Get sequence TYPE...
    i = 0;

    try {
        VFF_SET_ARGS(fieldSpec[i], SVN_PLN_SEQ_IDENT, VFT_STRING, 0,
                     NULL, VFO_ALLOCATE | VFO_VARIABLE_LEN);
        i++;
        status = VfGetFields(m_vfd, path, SVD_ST_PROTOCOL, fieldSpec, i, &numDone);

        if(status != E_NO_ERROR || numDone != i || fieldSpec[0].buffer == NULL) {
            CEPILogHelper::WriteToErrorLog("\n Could not get sequence name", __LINE__, __FILE__);
            return -1;
        }

        status = 0;

        if(!strstr((char*)fieldSpec[0].buffer, "EPI")) {
            status = -1;
        }
    }
//     free(fieldSpec[0].buffer);
//     fieldSpec[0].buffer = NULL;
    catch(...) {
        CEPILogHelper::WriteToErrorLog("Unable to load protocolCheckEPI ", __LINE__, __FILE__);
        return(EPI_ERR_EXCEPTION);
    }

    return status;
}
status_t CVFCReader::GetSeqName(int protocolNum, CString &sequencename)
{
    VfPathElem_t path[] = {
        {SVN_ST_PROTOCOL, protocolNum},  //-à Notice protocol number used to read the tag for that protocol
        {SVN_ST_VISUALGROUP, 0},
    };
    status_t status = E_ERROR;
    VfFieldSpec_t vfReq[2] = {0};
    int n = 0, num = 0;
    char l_ident[60] = {0};

    try {
        //following code is used to set tag data type, tag name, variable which will have result of tag etc
        VFF_SET_ARGS(vfReq[0], SVN_PLN_TEMPLATE_SEQ, SVT_PLN_TEMPLATE_SEQ, 60, l_ident, VFO_ALLOCATE | VFO_VARIABLE_LEN);
        n++;
        //following is used to read the tags for given path and study file.
        status = VfGetFields(m_vfd, (path), SVD_ST_PROTOCOL, vfReq, n , &num);

        if(E_ERROR != status && (1 == num)) {
            sequencename = l_ident;
        }
    } catch(...) {
        CString l_cserrmsg("EXCEPTION occurred in GetSeqName()");
        CEPILogHelper::WriteToErrorLog(l_cserrmsg, __LINE__, __FILE__);
        return(EPI_ERR_EXCEPTION);
    }

    return status;
}
int CVFCReader::DumpProcessedData(CString &f_csPath, int &f_height,
                                  double *&f_dData, CString &f_csFileName, int &f_nFileSize, int &f_width)
{
    try {
        int type = 0;
        int all_data ;
        int read_dim ;
        double Image_scale = 0.0;
        int         read_size = 0;
        int         phase_size = 0;
        CString fname(_T("EPIIMG"));
        SetSelectedPositionData(f_csPath);
        GetDataType(type);
        UpdatePathIndexNow();
        UpdateImagePath();
        f_dData = ReadImageData(all_data, read_dim, Image_scale);
        f_width = read_dim;
        f_height = all_data / f_width;

        if(NULL == f_dData) {
            CEPILogHelper::WriteToErrorLog(_T("Processed data reading failed."), __LINE__, __FILE__);
            return EPI_VFC_FAILED;
        }

        if(NULL == f_dData || read_dim <= 0) {
            if(f_dData) {
                free(f_dData);
            }

            CEPILogHelper::WriteToErrorLog(_T("Processed data buffer is NULL or read dim <= 0."), __LINE__, __FILE__);
            return EPI_VFC_FAILED;
        }

        f_nFileSize = read_size * phase_size * sizeof(double);
        getProcessImgDataDimentions(f_height,f_width);
        CString csdim(_T(""));
        csdim.Format(_T("_%dx%d"), read_size, phase_size);
        fname = fname + csdim;
        f_csFileName = fname;
    } catch(...) {
        return 1;
    }

    return 0;
}
double* CVFCReader::ReadImageData(int &all_data , int &read_dim, double &img_scale)
{
    int     j;
    int     size;
    int     status;
    double  *buffer = NULL;
    int     num_done;
    int     num_data;
    int     data_type;
    int     data_type_ORG;
    int     data_form;
    int     gr_num;
    int     *gr_point;
    int     gr_no = 0;
    VfFieldSpec_t   vf_req[2];
#ifdef DEBUG
    printf("DDisp_Read_Image2 IN\n");
#endif

    try {
        VFF_SET_ARGS(vf_req[0],
                     SVN_GEN_DATA_TYPE,
                     SVT_GEN_DATA_TYPE,
                     sizeof(data_type),
                     &data_type, 0);
        VFF_SET_ARGS(vf_req[1],
                     SVN_GEN_DATA_FORM,
                     SVT_GEN_DATA_FORM,
                     sizeof(data_form),
                     &data_form, 0);
        if_error(status = VfGetFields(m_vfd,
                                      g_sarch_node_path2,
                                      SVD_ST_PROCESSED,
                                      vf_req,
                                      2,
                                      &num_done)) {
#ifdef DEBUG
            printf("VfGetFields error !\n");
#endif
            return(NULL);
        }
        data_type_ORG   = data_type;
        data_type   = data_type & 0x00ffffff;

        if(data_type != VFT_INT32 &&
                data_type != VFT_INT16 &&
                data_type != VFT_FLT32) {
#ifdef DEBUG
            printf("Processed data type don't match VFT_INT32 or VFT_INT16 or VFT_FLT32!\n");
#endif
            return(NULL);
        }

        m_Path_index_now->form_of_data = data_form;
#ifdef DEBUG

        if(data_type == VFT_INT32) {
            printf("raw data type VFT_INT32!\n");
        } else if(data_type == VFT_INT16) {
            printf("raw data type VFT_INT16!\n");
        } else if(data_type == VFT_FLT32) {
            printf("raw data type VFT_FLT32!\n");
        }

#endif
        VFF_SET_ARGS(vf_req[0],
                     SVN_GEN_VF_GROUP,
                     SVT_GEN_VF_GROUP,
                     0,
                     0, VFO_ALLOCATE | VFO_VARIABLE_LEN);
        if_error(status = VfGetFields(m_vfd,
                                      g_sarch_node_path,
                                      SVD_ST_PROTOCOL,
                                      vf_req,
                                      1,
                                      &num_done)) {
#ifdef DEBUG
            printf("VfGetFields error !\n");
#endif
            return(NULL);
        }

        if(num_done == 1) {
            gr_num = vf_req[0].size / sizeof(int);

            if(gr_num >= 2) {
                gr_point = (int *)vf_req[0].buffer;
                gr_no = gr_point[1];
            }
        }

        VFF_SET_ARGSG(vf_req[0],
                      SVN_GEN_DATA,
                      data_type,
                      0,
                      0,
                      VFO_ALLOCATE | VFO_VARIABLE_LEN,
                      gr_no);
        VFF_SET_ARGSG(vf_req[1],
                      SVN_GEN_DATA,
                      data_type_ORG,
                      0,
                      0,
                      VFO_ALLOCATE | VFO_VARIABLE_LEN,
                      gr_no);
        if_error(status = VfGetFields(m_vfd,
                                      g_sarch_node_path2,
                                      SVD_ST_POSITION,
                                      &vf_req[0],
                                      1,
                                      &num_done)) {
#ifdef DEBUG
            printf("VfGetFields error!(image data)\n");
#endif
            return(NULL);
        }

        if(num_done != 1 && vf_req[0].size <= 0) {
            if_error(status = VfGetFields(m_vfd,
                                          g_sarch_node_path2,
                                          SVD_ST_POSITION,
                                          &vf_req[1],
                                          1,
                                          &num_done)) {
                return(NULL);
            }

            if(num_done != 1 && vf_req[0].size <= 0) {
                return(NULL);
            } else {
                ;
            }
        }

        read_dim = m_Path_index_now->pe_size;//Read to Pe

        /* Change Double Data */
        if(data_type == VFT_INT16) {
            size    = vf_req[0].size * (sizeof(double) / sizeof(int16_t));
            buffer  = (double *)malloc(size);

            if(!buffer) {
                return(NULL);
            }

            num_data = vf_req[0].size / sizeof(int16_t);

            for(j = 0; j < num_data; j++) {
                double tmp = 0.0;
                tmp = (double)(*(((int16_t *)(vf_req[0].buffer)) + num_data - (j + 1))) / SHRT_MAX;
                *(buffer + j) =
                    ((tmp));
            }

            img_scale = SHRT_MAX;
        } else if(data_type == VFT_FLT32) {
            size        = vf_req[0].size * (sizeof(double) / sizeof(float));;
            buffer      = (double *)malloc(size);

            if(!buffer) {
                return(NULL);
            }

            num_data    = vf_req[0].size / sizeof(float);

            for(j = 0; j < num_data; j++) {
                *(buffer + j) =
                    ((double)(*(((float *)(vf_req[0].buffer)) + num_data - (j + 1)))) / FLT_MAX;
            }

            img_scale = FLT_MAX;
        } else if(data_type == VFT_INT32) {
            size        = vf_req[0].size * (sizeof(double) / sizeof(int32_t));
            buffer      = (double *)malloc(size);

            if(!buffer) {
                return(NULL);
            }

            num_data    = vf_req[0].size / sizeof(int32_t);

            for(j = 0; j < num_data; j++) {
                *(buffer + j) =
                    ((double)(*(((int32_t *)(vf_req[0].buffer)) + num_data - (j + 1)))) / LONG_MAX;
            }

            img_scale = LONG_MAX;
        } else {
            free((void *)vf_req[0].buffer);
            return(NULL);
        }

        all_data = num_data;
    } catch(...) {
        CEPILogHelper::WriteToErrorLog(_T("Unable to read ReadImageDatas"), __LINE__, __FILE__);
    }

    return(buffer);
}
void CVFCReader::UpdateImagePath()
{
    g_sarch_node_path2[SVD_ST_PROTOCOL-1].index = m_Path_index_now->protocol;
    g_sarch_node_path2[SVD_ST_PROCESSED-1].index    = m_Path_index_now->processed;
    g_sarch_node_path2[SVD_ST_ECHO-1].index     = m_Path_index_now->echo;
    g_sarch_node_path2[SVD_ST_POSITION-1].index = m_Path_index_now->position;
}

int CVFCReader::vftointgr(void *DataBuffer, void *dftBuf, int data_type, int numPoints)
{
    switch(data_type) {
    case VFT_INT16: {
        flt32_t *inP = (flt32_t *)DataBuffer;
        int16_t *outP = (int16_t *)dftBuf;
        int i;
        
        for(i = 0; i < numPoints; i++) {
            if(inP[i] < INT16_MIN) {
                outP[i] = INT16_MIN;
            } else if(inP[i] > INT16_MAX) {
                outP[i] = INT16_MAX;
            } else {
                outP[i] = (int16_t)inP[i];
            }
        }
                    }
        break;
    case VFT_INT32: {
        flt32_t *inP = (flt32_t *)DataBuffer;
        int32_t *outP = (int32_t *)dftBuf;
        int i;
        
        for(i = 0; i < numPoints; i++) {
            if(inP[i] < INT32_MIN) {
                outP[i] = INT32_MIN;
            } else if(inP[i] > INT32_MAX) {
                outP[i] = INT32_MAX;
            } else {
                outP[i] = (int)inP[i];
            }
        }
                    }
        break;
    case VFT_FLT32:
        memcpy(dftBuf, DataBuffer, numPoints * sizeof(float));
        break;
    default:
        return (EPI_ERROR);
    }
    
    return 0;
}

int CVFCReader::getProcessImgDataDimentions(int &f_nDataHeight,int &f_nDataWidth )
{
    status_t                 status;
    int                      n;      /* number of varfield request block */
    VfFieldSpec_t            varfield[5];    /* varfield */
    int                      num_found;  /* number of varfields read */
    int32_t                  dataDim[2];  /* Data dimensions */
    int32_t			         data_desc[3];/*Data description*/

    try{
    n = 0;
    num_found = 0;
    VFF_SET_ARGS(varfield[n], SVN_GEN_DATA_DIM, SVT_GEN_DATA_DIM,
        sizeof(int32_t) * 2, &dataDim, 0);
    n++;
    status = VfGetFields(m_vfd,
        g_sarch_node_path2,
        SVD_ST_ECHO,
        varfield, n, &num_found);
    
    if(status != E_NO_ERROR || num_found != n) {
        CEPILogHelper::WriteToErrorLog(_T("Failed to get SVN_GEN_DATA_DIM."), __LINE__, __FILE__);
        return (EPI_VFC_FAILED);
    }
    
    n = 0;
    num_found = 0;
    VFF_SET_ARGS(varfield[n], SVN_GEN_DATA_DESC, SVT_GEN_DATA_DESC,
        sizeof(int32_t) * 2, &data_desc, 0);
    n++;
    status = VfGetFields(m_vfd,
        g_sarch_node_path2,
        SVD_ST_ECHO,
        varfield, n, &num_found);
    
    if(status != E_NO_ERROR || num_found != n) {
        CEPILogHelper::WriteToErrorLog(_T("Failed to get SVN_GEN_DATA_DESC."), __LINE__, __FILE__);
        return (EPI_VFC_FAILED);
    }
//     f_nDataHeight = dataDim[0] < dataDim[1]?dataDim[0]:dataDim[1];
//     f_nDataWidth = dataDim[0] > dataDim[1]?dataDim[0]:dataDim[1];
    f_nDataHeight = dataDim[0]/* < dataDim[1]?dataDim[0]:dataDim[1]*/;
    f_nDataWidth = dataDim[1]/* > dataDim[1]?dataDim[0]:dataDim[1]*/;

    
//     for( int j = 0; j < 2; j++ ){
//         switch( data_desc[j] ){                
//         case VFC_DATA_RO:
//             f_nDataWidth	= f_nDataWidth;
//             l_csTemp.Format("RO = %d",f_nDataWidth);
// //            AfxMessageBox(l_csTemp);
//             break;
//         case VFC_DATA_PE:
//             f_nDataHeight	= f_nDataHeight;
//             l_csTemp.Format("PE = %d",f_nDataHeight);
// //            AfxMessageBox(l_csTemp);
// 
//             break;
//         }
//     }
    }catch(...){
        CEPILogHelper::WriteToErrorLog(_T("Unable to read ReadImageDatas"), __LINE__, __FILE__);
        return EPI_ERR_EXCEPTION;
    }
    return E_NO_ERROR;
}

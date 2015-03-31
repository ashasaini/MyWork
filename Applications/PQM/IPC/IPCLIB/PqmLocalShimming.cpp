//REDMINE-1213_Update_03_MAY
//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-774
//Redmine-778
//Redmine-780
//Redmine-781
/***********************************************************************************
*
*    Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
*
*    Module: PQM-IPC
*    Author: PATNI
*************************************************************************************

  *                       Revision History
*******************************************************************************
*  Revision      Author           Date & Time            Changes
*  ==========================================================================
*   1.0           PATNI      2008/6/30 13:00:00     Created
*
******************************************************************************/

#include "stdafx.h"
#include "PqmLocalShimming.h"
#include <math.h>
#include <libStudyManager/destroy.h>
#include <PQM/PQMLogMgr.h>
#include <PQM/PqmUtilities.h>
#include <PQM/resource.h>
#include "Pqm.h"
#include "PqmShimSharedData.h"
#include "VarFieldHandle.h" //TMSC-Tanoue/2010Aug25/MVC008345
#include "ShimCommon.h"

extern  shimGlobal_t* globalP;
//****************************Method Header************************************
//Method Name   : CPqmLocalShimming
//Author        : PATNI
//Purpose       :
//*****************************************************************************
//+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
//CPqmLocalShimming::CPqmLocalShimming()
CPqmLocalShimming::CPqmLocalShimming(): m_pqm(0)
{

}

//****************************Method Header************************************
//Method Name   : ~CPqmLocalShimming
//Author        : PATNI
//Purpose       :
//*****************************************************************************
CPqmLocalShimming::~CPqmLocalShimming()
{

}

//+Patni-HAR-Ravindra/2010Dec03/Added/V2.00#IR-168
int CPqmLocalShimming::CalcShimandHRShimArea(
    shimGlobal_t* f_glbalp
)
{
    if (NULL == f_glbalp) {
        return E_ERROR;
    }

    int l_status = E_NO_ERROR;

    globalP = f_glbalp;

    //Here, MRS voxel of information, the rectangular solid for the displaced,
    //startPosi endPosi and calculates the set
    l_status = MrsCalcShimArea();

    if (l_status != TRUE) {
        return l_status ;
    }

    //Area is appropriate to determine whether
    if ((l_status = MrsCheckHrShimArea()) != E_NO_ERROR) {
        //Outside of the area because of an error
        return l_status;
    }

    return E_NO_ERROR;
}
//-Patni-HAR-Ravindra/2010Dec03/Added/V2.00#IR-168

//****************************Method Header************************************
//Method Name   : ShimQuit
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int CPqmLocalShimming::ShimQuit(
    const int flag
)const
{
    if (NULL == globalP) return FALSE;

    //shimStudyClose();
    DEL_PTR(globalP->shimStudyP->vfHandle);

    switch (flag) {
        case SHIM_FATAL_ERROR:
            ShimmPopErrorDialog(2, IDS_OP_SHIM_FATAL_ERROR, _T("IDS_OP_SHIM_FATAL_ERROR"));
            break;

        case SHIM_MS_ERROR:
            ShimmPopErrorDialog(2, IDS_OP_ERROR_MS, _T("IDS_OP_ERROR_MS"));
            break;

        case MRS_MS_LIMIT_ERROR:
            ShimmPopErrorDialog(2, IDS_OP_MRS_MS_LIMIT_ERROR, _T("IDS_OP_MRS_MS_LIMIT_ERROR"));
            break;
    }

    if (flag == SHIM_NORMAL_END) {
        CPqmShimSharedData::GetInstance()->SetHRShimPath(NULL);

    }

    //+Patni-HAR/2009Nov27/Commented/DeFT# MSA0248-00139
    //if(globalP)
    //{
    //	delete globalP;
    //	globalP = NULL;
    //
    //}
    //-Patni-HAR/2009Nov27/Commented/DeFT# MSA0248-00139

    return TRUE;
}

//****************************Method Header************************************
//Method Name   : ShimLocalRun
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int CPqmLocalShimming::ShimLocalRun(shimGlobal_t* f_glbalp)
{
    int l_status = E_NO_ERROR;

    if (NULL == f_glbalp) {
        return E_ERROR;
    }

    //+Patni-/2009June03/JFT#210,IR 47,48
    globalP = f_glbalp;
    //+Patni-/2009June03/JFT#210,IR 47,48

    /* Here, MRS voxel of information, the rectangular solid for the displaced,
    startPosi endPosi and calculates the set */
    MrsCalcShimArea();

    /* Area is appropriate to determine whether */
    if ((l_status = MrsCheckHrShimArea()) != E_NO_ERROR) {
        /* Outside of the area because of an error */
        return l_status;
    }

    if ((l_status = ShimMsShimRun()) != E_NO_ERROR) {
        return l_status;
    }

    return E_NO_ERROR;
}
//****************************Method Header************************************
//Method Name   : SetPqmToLocalShim
//Author        : PATNI
//Purpose       :
//*****************************************************************************
CPqmLocalShimming::SetPqmToLocalShim(CPqm* f_pqm)
{
    m_pqm = f_pqm;
}


//****************************Method Header************************************
//Method Name   : Shimf0Write
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int CPqmLocalShimming::Shimf0Write()const
{
    LPCTSTR FUNC_NAME = _T("CPqmLocalShimming::Shimf0Write");
    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    /*
    VfFieldSpec_t	l_req_flds [5];
    int			l_n;
    int			typeFormatDepth;
    int			l_num_done ;
    VfPathElem_t	*l_process_path_p ;
    int32_t		l_water_fat;
    */
    VfFieldSpec_t	l_req_flds [5] = {0};
    int			l_num_done = 0;
    VfPathElem_t*	l_process_path_p = 0;

    //- Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    if (globalP->shimInfoP->hr_shim_processed == -1) {
        //	shimPrintf("*E","*E   %s:%d,Error\l_n",__FILE__,__LINE__);
        return E_ERROR;
    }

    int32_t l_water_fat = 0;

    l_process_path_p  = globalP->shimStudyP->hrShimNodes;
    l_process_path_p [SVD_ST_PROCESSED - 1].index = globalP->shimInfoP->hr_shim_processed;

    int l_n = 0;
    int typeFormatDepth = SVD_ST_PROCESSED;
    VFF_SET_ARGS(l_req_flds [l_n], SVN_SEQ_SHIM_FSY_FLAG, SVT_SEQ_SHIM_FSY_FLAG,
                 sizeof(int32_t), &l_water_fat, VFO_REPLACE);
    l_n++;


    //+Patni-SS/2009Apr01/PSP1#VFLOCK
    CString str(_T(""));
    str.Format(_T("Protocol = %d, l_water_fat = %d,"), globalP->shimInfoP->hr_shim_processed, l_water_fat);

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
              _T("Shimf0Write Put SHIM_FSY_FLAG in study file (VfPutField): ") + str);
    //-Patni-SS/2009Apr01/PSP1#VFLOCK

    status_t l_nStatus = globalP->shimStudyP->vfHandle->PutFields(l_process_path_p ,
                         typeFormatDepth, l_req_flds , l_n, &l_num_done , NULL);

    if (l_nStatus != E_NO_ERROR || l_num_done  != l_n) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
        return E_ERROR;
    }

    return E_NO_ERROR;

}

//****************************Method Header************************************
//Method Name   : MrsCrossProduct
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int CPqmLocalShimming::MrsCrossProduct(
    double  l_p[],
    double  l_q[],
    double  l_cross_product[]
)const
{
    l_cross_product[0] = l_p[1] * l_q[2] - l_p[2] * l_q[1];
    l_cross_product[1] = l_p[2] * l_q[0] - l_p[0] * l_q[2];
    l_cross_product[2] = l_p[0] * l_q[1] - l_p[1] * l_q[0];

    MrsNormalize(l_cross_product);
    return TRUE;
}

//****************************Method Header************************************
//Method Name   : ShimMsShimRun
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int CPqmLocalShimming::ShimMsShimRun(
)
{

    static msInteractiveShim_t	shimData;

    memset(shimData.study_name, NULL, sizeof(shimData.study_name));

    if (sizeof(shimData.study_name) < strlen(globalP->shimStudyP->study)) {
        /*shimPrintf("*E","*E   %s : %d. strlen Error %d,%d\sn",__FILE__,__LINE__,
        sizeof(shimData.study_name) ,strlen(globalP->shimStudyP->study));*/
        return E_ERROR;
    }


    CString tmp_str(_T("/Study/"));

    CString str(globalP->shimStudyP->study);
    int l_pos = str.ReverseFind('/');
    l_pos++;
    str.Delete(0, l_pos);
    tmp_str += str;


    // Get Scratchpad file path
    //strcpy(shimData.study_name,globalP->shimStudyP->study,strlen(globalP->shimStudyP->study));
    wcstombs(shimData.study_name, tmp_str, tmp_str.GetLength() + 1);//+Patni-HAR/2009Aug31/Modified/Corrected call for wcstombs()
    shimData.protocolNode = globalP->shimStudyP->hrShimNodes[SVD_ST_PROTOCOL - 1].index;
    /* 磁場分布計算のための画像は7.0にあると考えていい */
    shimData.processedNode = 0;
    shimData.startPosi = globalP->shimStudyP->startPosi;
    shimData.endPosi = globalP->shimStudyP->endPosi;
    /* 高分解能シミングの中心を渡す */
    shimData.centerPosi = globalP->hr_shimInfoP->offsetVec;
    /* ３チャンネルとして計算させる */
    shimData.shimming_Channels = 3;

    m_shimming_request.SendLocalShimReqTomsShim(&shimData);
    //Patni-HAR/2009Nov05/Modifed/MSA0248-00139-MRS-Crash Return type corrected
    return ShimMsEndCB(&shimData);
}

//****************************Method Header************************************
//Method Name   : ShimCheckOffset3
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int CPqmLocalShimming::ShimCheckOffset3(
    const int f_typeformatdepth
)const
{
    VfFieldSpec_t	l_req_flds[5] = {0}; //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    int			l_num_done = 0; //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections

    int32_t		l_shim_coil[SHIM_COIL] = {0};



    if (globalP->shimInfoP->hr_shim_processed == -1) {
        return E_ERROR;
    }

    VfPathElem_t*	l_process_path_p  = globalP->shimStudyP->imageNodes;
    l_process_path_p [SVD_ST_PROCESSED - 1].index = 0;
    l_process_path_p [SVD_ST_ECHO - 1].index = 0;
    l_process_path_p [SVD_ST_POSITION - 1].index = 0;
    VfPathElem_t*        l_shim_path_p  = globalP->shimStudyP->hrShimNodes;
    l_shim_path_p [SVD_ST_PROCESSED - 1].index = globalP->shimInfoP->hr_shim_processed;

    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(l_req_flds, 0, sizeof(VfFieldSpec_t) * 5);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    int l_n = 0;
    VFF_SET_ARGS(l_req_flds [l_n], SVN_SEQ_SHIM_OFFSET, SVT_SEQ_SHIM_OFFSET,
                 sizeof(int32_t)*SHIM_COIL, l_shim_coil, 0);
    l_n++;

    status_t l_status = globalP->shimStudyP->vfHandle->GetFields(l_shim_path_p , f_typeformatdepth, l_req_flds , l_n, &l_num_done);

    if (l_status != E_NO_ERROR
        || l_num_done  != l_n) {
        return E_ERROR;
    }

    for (int l_i = 0; l_i < 3; l_i++) {
        if ((l_shim_coil[l_i] > globalP->shimInfoP->localShimLimit) ||
            (l_shim_coil[l_i] < -(globalP->shimInfoP->localShimLimit))) {
            return MRS_MS_LIMIT_ERROR;
        }
    }

    return E_NO_ERROR;

}

//****************************Method Header************************************
//Method Name   : HrCheckArea
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int CPqmLocalShimming::HrCheckArea(
) const
{
    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    /*
    shimStudy_t  *l_mrs_p ;
    hr_shimInfo_t *l_hr_p ;
    */
    //- Patni-PJS/2009Apr25/Modify/CPP Test tool execution


    shimStudy_t* l_mrs_p  = globalP->shimStudyP;
    hr_shimInfo_t* l_hr_p  = globalP->hr_shimInfoP;

    if ((l_mrs_p ->startPosi.x < l_hr_p ->startPosi.x) ||
        (l_mrs_p ->startPosi.y < l_hr_p ->startPosi.y) ||
        (l_mrs_p ->startPosi.z < l_hr_p ->startPosi.z) ||
        (l_mrs_p ->endPosi.x > l_hr_p ->endPosi.x) ||
        (l_mrs_p ->endPosi.y > l_hr_p ->endPosi.y) ||
        (l_mrs_p ->endPosi.z > l_hr_p ->endPosi.z)) {
        return SHIM_AREA_ERROR;
    }

    return E_NO_ERROR;
}

//****************************Method Header************************************
//Method Name   : HrShimCalcPosi
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int CPqmLocalShimming::HrShimCalcPosi(
) const
{
    //+Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    flt32_t l_maxim[3] = {0.0}, l_minim[3] = {0.0};
    flt32_t l_vec1[3] = {0.0}, l_vec2[3] = {0.0}, l_vec3[3] = {0.0}, l_vec4[3] = {0.0}, l_vec5[3] = {0.0},
                                           l_vec6[3] = {0.0}, l_vec7[3] = {0.0}, l_vec8[3] = {0.0};
    flt32_t l_shift_so[3] = {0.0}, l_shift_pe [3] = {0.0}, l_shift_ro[3] = {0.0};
    //-Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections

    l_shift_so [0] = globalP->hr_shimInfoP->sliceVec.x * globalP->hr_shimInfoP->wholeThickness * 0.5f;
    l_shift_so [1] = globalP->hr_shimInfoP->sliceVec.y * globalP->hr_shimInfoP->wholeThickness * 0.5f;
    l_shift_so [2] = globalP->hr_shimInfoP->sliceVec.z * globalP->hr_shimInfoP->wholeThickness * 0.5f;
    l_shift_pe [0] = globalP->hr_shimInfoP->peVec.x * globalP->hr_shimInfoP->fovVec.x * 0.5f;
    l_shift_pe [1] = globalP->hr_shimInfoP->peVec.y * globalP->hr_shimInfoP->fovVec.x * 0.5f;
    l_shift_pe [2] = globalP->hr_shimInfoP->peVec.z * globalP->hr_shimInfoP->fovVec.x * 0.5f;
    l_shift_ro [0] = globalP->hr_shimInfoP->roVec.x * globalP->hr_shimInfoP->fovVec.y * 0.5f;
    l_shift_ro [1] = globalP->hr_shimInfoP->roVec.y * globalP->hr_shimInfoP->fovVec.y * 0.5f;
    l_shift_ro [2] = globalP->hr_shimInfoP->roVec.z * globalP->hr_shimInfoP->fovVec.y * 0.5f;


    /* 8-point demand to coordinate */

    /* X, y, z maximum, minimum seek */
    for (int l_i = 0; l_i < 3; l_i++) {
        l_vec1[l_i] = l_shift_so [l_i] + l_shift_pe [l_i] + l_shift_ro [l_i];
        l_maxim[l_i] = l_minim[l_i] = l_vec1[l_i];

        l_vec2[l_i] = l_shift_so [l_i] + l_shift_pe [l_i] - l_shift_ro [l_i];

        if (l_vec2[l_i] > l_maxim[l_i]) {
            l_maxim[l_i] = l_vec2[l_i];
        }

        if (l_vec2[l_i] < l_minim[l_i]) {
            l_minim[l_i] = l_vec2[l_i];
        }

        l_vec3[l_i] = l_shift_so [l_i] - l_shift_pe [l_i] + l_shift_ro [l_i];

        if (l_vec3[l_i] > l_maxim[l_i]) {
            l_maxim[l_i] = l_vec3[l_i];
        }

        if (l_vec3[l_i] < l_minim[l_i]) {
            l_minim[l_i] = l_vec3[l_i];
        }

        l_vec4[l_i] = l_shift_so [l_i] - l_shift_pe [l_i] - l_shift_ro [l_i];

        if (l_vec4[l_i] > l_maxim[l_i]) {
            l_maxim[l_i] = l_vec4[l_i];
        }

        if (l_vec4[l_i] < l_minim[l_i]) {
            l_minim[l_i] = l_vec4[l_i];
        }

        l_vec5[l_i] = -l_shift_so [l_i] + l_shift_pe [l_i] + l_shift_ro [l_i];

        if (l_vec5[l_i] > l_maxim[l_i]) {
            l_maxim[l_i] = l_vec5[l_i];
        }

        if (l_vec5[l_i] < l_minim[l_i]) {
            l_minim[l_i] = l_vec5[l_i];
        }

        l_vec6[l_i] = -l_shift_so [l_i] + l_shift_pe [l_i] - l_shift_ro [l_i];

        if (l_vec6[l_i] > l_maxim[l_i]) {
            l_maxim[l_i] = l_vec6[l_i];
        }

        if (l_vec6[l_i] < l_minim[l_i]) {
            l_minim[l_i] = l_vec6[l_i];
        }

        l_vec7[l_i] = -l_shift_so [l_i] - l_shift_pe [l_i] + l_shift_ro [l_i];

        if (l_vec7[l_i] > l_maxim[l_i]) {
            l_maxim[l_i] = l_vec7[l_i];
        }

        if (l_vec7[l_i] < l_minim[l_i]) {
            l_minim[l_i] = l_vec7[l_i];
        }

        l_vec8[l_i] = -l_shift_so [l_i] - l_shift_pe [l_i] - l_shift_ro [l_i];

        if (l_vec8[l_i] > l_maxim[l_i]) {
            l_maxim[l_i] = l_vec8[l_i];
        }

        if (l_vec8[l_i] < l_minim[l_i]) {
            l_minim[l_i] = l_vec8[l_i];
        }

    }

    /*To further offset StartPosi, endPosi set */
    globalP->hr_shimInfoP->startPosi.x = globalP->hr_shimInfoP->offsetVec.x + l_minim[0];
    globalP->hr_shimInfoP->startPosi.y = globalP->hr_shimInfoP->offsetVec.y + l_minim[1];
    globalP->hr_shimInfoP->startPosi.z = globalP->hr_shimInfoP->offsetVec.z + l_minim[2];
    globalP->hr_shimInfoP->endPosi.x = globalP->hr_shimInfoP->offsetVec.x + l_maxim[0];
    globalP->hr_shimInfoP->endPosi.y = globalP->hr_shimInfoP->offsetVec.y + l_maxim[1];
    globalP->hr_shimInfoP->endPosi.z = globalP->hr_shimInfoP->offsetVec.z + l_maxim[2];
    return TRUE;

}

//****************************Method Header************************************
//Method Name   : HrShimSetWholeThickness
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int CPqmLocalShimming::HrShimSetWholeThickness(
) const
{
    flt32_t l_thick = globalP->hr_shimInfoP->thickness;
    flt32_t l_gap = globalP->hr_shimInfoP->sliceGap;
    flt32_t l_num = (flt32_t)globalP->hr_shimInfoP->sliceNum;

    globalP->hr_shimInfoP->wholeThickness = (l_thick + l_gap) * l_num - l_gap; //l_whole;
    return TRUE;
}

//****************************Method Header************************************
//Method Name   : HrShimSetRO
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int CPqmLocalShimming::HrShimSetRO(
)const
{
    double  l_so[3] = {0}, l_pe[3] = {0}, l_ro[3] = {0}; //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections

    l_so[0] = globalP->hr_shimInfoP->sliceVec.x;
    l_so[1] = globalP->hr_shimInfoP->sliceVec.y;
    l_so[2] = globalP->hr_shimInfoP->sliceVec.z;
    l_pe[0] = globalP->hr_shimInfoP->peVec.x;
    l_pe[1] = globalP->hr_shimInfoP->peVec.y;
    l_pe[2] = globalP->hr_shimInfoP->peVec.z;

    MrsCrossProduct(l_pe, l_so, l_ro);

    globalP->hr_shimInfoP->roVec.x = (float)l_ro[0];
    globalP->hr_shimInfoP->roVec.y = (float)l_ro[1];
    globalP->hr_shimInfoP->roVec.z = (float)l_ro[2];
    return TRUE;
}

//****************************Method Header************************************
//Method Name   : HrCheckVolume
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int CPqmLocalShimming::HrCheckVolume(
) const
{
    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    /*
    shimStudy_t  *l_mrs_p;
    hr_shimInfo_t *l_hr_p;
    flt32_t  l_min_x, l_min_y, l_min_z;
    flt32_t  l_vol_x, l_vol_y, l_vol_z;
    flt32_t  l_adjust, l_adjust_s, l_adjust_e;
    */

    //- Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    //shimPrintf("@","@    hrCheckVolume()\l_n");

    shimStudy_t*  l_mrs_p = globalP->shimStudyP;
    hr_shimInfo_t* l_hr_p = globalP->hr_shimInfoP;

    /* CD 5 minutes, 5 minutes pixel size */
    flt32_t l_min_x = (l_hr_p ->fovVec.x / l_hr_p ->acq_matrix.x) * 5.0f;
    flt32_t l_min_y = (l_hr_p ->fovVec.y / l_hr_p ->acq_matrix.y) * 5.0f;
    flt32_t l_min_z  = (l_hr_p ->thickness + l_hr_p ->sliceGap) * 5.0f;

    /*The size of local SHIMINGUERIA */
    flt32_t l_vol_x = l_mrs_p ->endPosi.x - l_mrs_p ->startPosi.x;
    flt32_t l_vol_y  = l_mrs_p ->endPosi.y - l_mrs_p ->startPosi.y;
    flt32_t l_vol_z  = l_mrs_p ->endPosi.z - l_mrs_p ->startPosi.z;

    flt32_t  l_adjust = 0.0f, l_adjust_s = 0.0f, l_adjust_e = 0.0f;

    if (l_min_x > l_vol_x) {
        /*SHIMINGUERIA local (x direction) from the small increase */
        l_adjust = l_min_x - l_vol_x;
        l_adjust_s  = l_mrs_p ->startPosi.x - (l_adjust / 2.0f);
        l_adjust_e  = l_mrs_p ->endPosi.x + (l_adjust / 2.0f);

        if (l_adjust_s  < l_hr_p ->startPosi.x) {
            /*Direction and start to spread out to be high-resolution SHIMINGUERIA
            	End direction only widen */
            l_mrs_p ->endPosi.x += l_adjust;

        } else if (l_adjust_e  > l_hr_p ->endPosi.x) {
            /* End direction and spread out to be high-resolution SHIMINGUERIA
            	Start direction only widen */
            l_mrs_p ->startPosi.x -= l_adjust;

        } else {
            /* Expand both halves */
            l_mrs_p ->endPosi.x += (l_adjust / 2.0f);
            l_mrs_p ->startPosi.x -= (l_adjust / 2.0f);
        }
    }

    if (l_min_y > l_vol_y) {
        /*LOCAL SHIMINGUERIA (y direction) from the small increase */
        l_adjust = l_min_y - l_vol_y ;
        l_adjust_s  = l_mrs_p ->startPosi.y - (l_adjust / 2.0f);
        l_adjust_e  = l_mrs_p ->endPosi.y + (l_adjust / 2.0f);

        if (l_adjust_s  < l_hr_p ->startPosi.y) {
            l_mrs_p ->endPosi.y += l_adjust;

        } else if (l_adjust_e  > l_hr_p ->endPosi.y) {
            l_mrs_p ->startPosi.y -= l_adjust;

        } else {
            l_mrs_p ->endPosi.y += (l_adjust / 2.0f);
            l_mrs_p ->startPosi.y -= (l_adjust / 2.0f);
        }
    }

    if (l_min_z  > l_vol_z) {
        /* SHIMINGUERIA local (z direction) from the small increase */
        l_adjust = l_min_z  - l_vol_z ;
        l_adjust_s  = l_mrs_p ->startPosi.z - (l_adjust / 2.0f);
        l_adjust_e  = l_mrs_p ->endPosi.z + (l_adjust / 2.0f);

        if (l_adjust_s  < l_hr_p ->startPosi.z) {
            l_mrs_p ->endPosi.z += l_adjust;

        } else if (l_adjust_e  > l_hr_p ->endPosi.z) {
            l_mrs_p ->startPosi.z -= l_adjust;

        } else {
            l_mrs_p ->endPosi.z += (l_adjust / 2.0f);
            l_mrs_p ->startPosi.z -= (l_adjust / 2.0f);
        }
    }

    return TRUE;
}

//****************************Method Header************************************
//Method Name   : MrsCheckHrShimArea
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int CPqmLocalShimming::MrsCheckHrShimArea(
)const
{
    /* SURAISUBEKUTA FEZUBEKUTA and to seek RIDOBEKUTA */
    HrShimSetRO();

    /* Slice the size seek direction */
    HrShimSetWholeThickness();

    /*startPosi, endPosi to calculate the value Cart*/
    HrShimCalcPosi();

    /* SHIMINGUERIA SHIMINGU local high-resolution space is not check out * /
    / * Outside the area if it returns l_status SHIM_AREA_ERROR */
    status_t l_status = HrCheckArea();

    if (l_status) {
        return l_status;
    }

    /* SHIMINGUERIA local, high-resolution information SHIMINGU,
    CD 5 minutes or less than 5 minutes pixels, the larger region.
    In this case, high-resolution SHIMINGU outside of the area to avoid */
    HrCheckVolume();

    return E_NO_ERROR;
}

//****************************Method Header************************************
//Method Name   : MrsSetRO
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int CPqmLocalShimming::MrsSetRO(
)const
{
    double  l_so[3] = {0}, l_pe[3] = {0}, l_ro[3] = {0};	//Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections

    l_so[0] = globalP->mrsInfoP->sliceVec.x;
    l_so[1] = globalP->mrsInfoP->sliceVec.y;
    l_so[2] = globalP->mrsInfoP->sliceVec.z;
    l_pe[0] = globalP->mrsInfoP->peVec.x;
    l_pe[1] = globalP->mrsInfoP->peVec.y;
    l_pe[2] = globalP->mrsInfoP->peVec.z;

    MrsCrossProduct(l_pe, l_so, l_ro);

    globalP->mrsInfoP->roVec.x = (float)l_ro[0];
    globalP->mrsInfoP->roVec.y = (float)l_ro[1];
    globalP->mrsInfoP->roVec.z = (float)l_ro[2];
    return TRUE;
}

//****************************Method Header************************************
//Method Name   : MrsCalcShimArea
//Author        : PATNI
//Purpose       :
//*****************************************************************************
int CPqmLocalShimming::MrsCalcShimArea(
)const
{
    /* SURAISUBEKUTA FEZUBEKUTA and to seek RIDOBEKUTA */
    MrsSetRO();

    /* Slice the size seek direction*/
    MrsSetWholeThickness();

    /* startPosi,endPosiを計算し値を入れる */
    MrsCalcPosi();
    return TRUE;
}

//****************************Method Header************************************
//Method Name   : MrsCalcPosi
//Author        : PATNI
//Purpose       :
//*****************************************************************************
void CPqmLocalShimming::MrsCalcPosi(
)const
{
    //+Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    flt32_t l_maxim[3] = {0.0}, l_minim[3] = {0.0};
    flt32_t l_vec1[3] = {0.0}, l_vec2[3] = {0.0}, l_vec3[3] = {0.0}, l_vec4[3] = {0.0}, l_vec5[3] = {0.0},
                                           l_vec6[3] = {0.0}, l_vec7[3] = {0.0}, l_vec8[3] = {0.0};
    flt32_t l_shift_so [3] = {0.0}, l_shift_pe[3] = {0.0}, l_shift_ro [3] = {0.0};
    //-Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections

    mrsInfo_t*   l_pos_p  = globalP->mrsInfoP;

    l_shift_so [0] = l_pos_p ->sliceVec.x * l_pos_p ->wholeThickness * 0.5f;
    l_shift_so [1] = l_pos_p ->sliceVec.y * l_pos_p ->wholeThickness * 0.5f;
    l_shift_so [2] = l_pos_p ->sliceVec.z * l_pos_p ->wholeThickness * 0.5f;
    l_shift_pe [0] = l_pos_p ->peVec.x * l_pos_p ->fovVec.x * 0.5f;
    l_shift_pe [1] = l_pos_p ->peVec.y * l_pos_p ->fovVec.x * 0.5f;
    l_shift_pe [2] = l_pos_p ->peVec.z * l_pos_p ->fovVec.x * 0.5f;
    l_shift_ro [0] = l_pos_p ->roVec.x * l_pos_p ->fovVec.y * 0.5f;
    l_shift_ro [1] = l_pos_p ->roVec.y * l_pos_p ->fovVec.y * 0.5f;
    l_shift_ro [2] = l_pos_p ->roVec.z * l_pos_p ->fovVec.y * 0.5f;

    /* 8-point demand to coordinate /
    / * X, y, z maximum, minimum seek */
    for (int l_i = 0; l_i < 3; l_i++) {
        l_vec1[l_i] = l_shift_so [l_i] + l_shift_pe [l_i] + l_shift_ro [l_i];
        l_maxim[l_i] = l_minim[l_i] = l_vec1[l_i];

        l_vec2[l_i] = l_shift_so [l_i] + l_shift_pe [l_i] - l_shift_ro [l_i];

        if (l_vec2[l_i] > l_maxim[l_i]) {
            l_maxim[l_i] = l_vec2[l_i];
        }

        if (l_vec2[l_i] < l_minim[l_i]) {
            l_minim[l_i] = l_vec2[l_i];
        }

        l_vec3[l_i] = l_shift_so [l_i] - l_shift_pe [l_i] + l_shift_ro [l_i];

        if (l_vec3[l_i] > l_maxim[l_i]) {
            l_maxim[l_i] = l_vec3[l_i];
        }

        if (l_vec3[l_i] < l_minim[l_i]) {
            l_minim[l_i] = l_vec3[l_i];
        }

        l_vec4[l_i] = l_shift_so [l_i] - l_shift_pe [l_i] - l_shift_ro [l_i];

        if (l_vec4[l_i] > l_maxim[l_i]) {
            l_maxim[l_i] = l_vec4[l_i];
        }

        if (l_vec4[l_i] < l_minim[l_i]) {
            l_minim[l_i] = l_vec4[l_i];
        }

        l_vec5[l_i] = -l_shift_so [l_i] + l_shift_pe [l_i] + l_shift_ro [l_i];

        if (l_vec5[l_i] > l_maxim[l_i]) {
            l_maxim[l_i] = l_vec5[l_i];
        }

        if (l_vec5[l_i] < l_minim[l_i]) {
            l_minim[l_i] = l_vec5[l_i];
        }

        l_vec6[l_i] = -l_shift_so [l_i] + l_shift_pe [l_i] - l_shift_ro [l_i];

        if (l_vec6[l_i] > l_maxim[l_i]) {
            l_maxim[l_i] = l_vec6[l_i];
        }

        if (l_vec6[l_i] < l_minim[l_i]) {
            l_minim[l_i] = l_vec6[l_i];
        }

        l_vec7[l_i] = -l_shift_so [l_i] - l_shift_pe [l_i] + l_shift_ro [l_i];

        if (l_vec7[l_i] > l_maxim[l_i]) {
            l_maxim[l_i] = l_vec7[l_i];
        }

        if (l_vec7[l_i] < l_minim[l_i]) {
            l_minim[l_i] = l_vec7[l_i];
        }

        l_vec8[l_i] = -l_shift_so [l_i] - l_shift_pe [l_i] - l_shift_ro [l_i];

        if (l_vec8[l_i] > l_maxim[l_i]) {
            l_maxim[l_i] = l_vec8[l_i];
        }

        if (l_vec8[l_i] < l_minim[l_i]) {
            l_minim[l_i] = l_vec8[l_i];
        }

    }

    /* ssstartPosi, endPosi set */
    globalP->shimStudyP->startPosi.x = l_pos_p ->offsetVec.x + l_minim[0];
    globalP->shimStudyP->startPosi.y = l_pos_p ->offsetVec.y + l_minim[1];
    globalP->shimStudyP->startPosi.z = l_pos_p ->offsetVec.z + l_minim[2];
    globalP->shimStudyP->endPosi.x = l_pos_p ->offsetVec.x + l_maxim[0];
    globalP->shimStudyP->endPosi.y = l_pos_p ->offsetVec.y + l_maxim[1];
    globalP->shimStudyP->endPosi.z = l_pos_p ->offsetVec.z + l_maxim[2];
}

//****************************Method Header************************************
//Method Name   : MrsNormalize
//Author        : PATNI
//Purpose       :
//*****************************************************************************
void CPqmLocalShimming::MrsNormalize(
    double v[]
)const
{
    double l_length = sqrt(v[0] * v[0]  +  v[1] * v[1]  +  v[2] * v[2]);

    if (l_length != 0.) {
        for (int l_i = 0; l_i < 3; l_i++) {
            v[l_i] = v[l_i] / l_length;
        }

    } else {
        for (int l_i = 0; l_i < 3; l_i++) {
            v[l_i] = 0.;
        }
    }
}
//****************************Method Header************************************
//Method Name   : MrsSetWholeThickness
//Author        : PATNI
//Purpose       :
//*****************************************************************************
void CPqmLocalShimming::MrsSetWholeThickness(
)const
{
    flt32_t L_thick = globalP->mrsInfoP->thickness;
    flt32_t l_gap = globalP->mrsInfoP->sliceGap;
    flt32_t l_num = (flt32_t)globalP->mrsInfoP->sliceNum;

    globalP->mrsInfoP->wholeThickness = (L_thick + l_gap) * l_num - l_gap; //l_whole;
}

void CPqmLocalShimming::ShimmPopErrorDialog(
    const int type,
    const int f_error_id,
    const CString& f_errmsg
)const
{
    //+Patni-KSS/2011Feb17/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(type);
    UNREFERENCED_PARAMETER(f_error_id);
    //-Patni-KSS/2011Feb17/Added/CPP Test tool Execution

    //+Patni-HAR/2009April09/Added/MRSCSI Scan Fix
    MRERROR_LOG_DATA    error_log_data;

    error_log_data.error_message = CPqmUtilities::GetMultiLingualString(f_errmsg);
    //Patni-AMT/2009Dec4/Modified/DeFT# MSA0248-00139-Code Review
    error_log_data.error_source  = _T("PQM");




    CPQMLogMgr::GetInstance()->DisplayMessageinErrorDialog(error_log_data, MPlus::EH::Information);
    //-Patni-HAR/2009April09/Added/MRSCSI Scan Fix
}

//****************************Method Header************************************
//Method Name   : ShimMsEndCB
//Author        : PATNI
//Purpose       :
//*****************************************************************************
//+Patni-HAR/2009Nov05/Modifed/MSA0248-00139-MRS-Crash Return type corrected
status_t CPqmLocalShimming::ShimMsEndCB(
    msInteractiveShim_t* f_shim_data
)const
{
    LPCTSTR FUNC_NAME = _T("CPqmLocalShimming::ShimMsEndCB");
    int l_errflag = 0;

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (NULL == f_shim_data) {
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    globalP->shimInfoP->hr_shim_processed = f_shim_data->patient[1];

    if (f_shim_data->status != E_NO_ERROR) {
        if (globalP->shimStudyP->shimInfo_mode == SHIM_AUTO_MODE) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("shimm status is E_ERROR"));
            ShimQuit(SHIM_MS_ERROR);
            return E_ERROR;

        } else {
            ShimmPopErrorDialog(2, IDS_OP_ERROR_MS, "IDS_OP_ERROR_MS");
            //MrsPopErrorDialog(3,OP_ERROR_MS)
            return E_ERROR;
        }
    }

    /* Beyond the limits not to check whether */
    status_t l_status = ShimCheckOffset3(SVD_ST_PROCESSED);

    if (l_status == MRS_MS_LIMIT_ERROR) {
        if (globalP->shimStudyP->shimInfo_mode == SHIM_AUTO_MODE) {
            ShimQuit(MRS_MS_LIMIT_ERROR);
            return E_ERROR;

        } else {
            ShimmPopErrorDialog(2, IDS_OP_ERROR_MS, _T("IDS_OP_ERROR_MS"));
            //MrsPopErrorDialog(3,MrsPopErrorDialog(3,OP_ERROR_MS);
            return E_ERROR;
        }

    } else if (l_status != E_NO_ERROR) {
        l_errflag = 1;
    }

    /* Water / fat flag */
    if (Shimf0Write() != E_NO_ERROR) {
        l_errflag = 1;
    }

    if (globalP->shimStudyP->debug_mode)

        //	ShimMsResult();
        if (globalP->shimStudyP->shimInfo_mode == SHIM_AUTO_MODE && l_errflag) {
            ShimQuit(SHIM_MS_ERROR);
            return E_ERROR;

        } else if (l_errflag) {
            ShimmPopErrorDialog(2, IDS_OP_ERROR_RESULT , _T("IDS_OP_ERROR_RESULT"));
            return E_ERROR;
        }

    //MrsPopErrorDialog(3,MrsPopErrorDialog(3,OP_ERROR_RESULT);

    /*The following process. */
    //   mrsProcHandler(MRS_PROC_LOCAL);

    return E_NO_ERROR;
}
//-Patni-HAR/2009Nov05/Modifed/MSA0248-00139-MRS-Crash Return type corrected


//Redmine-783
//Redmine-780
//Redmine-781
// CheckFOVInMap.cpp: implementation of the CCheckFOVInMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CheckFOVInMap.h"
#include <math.h>
#include <PQM/PQMLogMgr.h>
#include <PQM/PqmUtilities.h>		//Patni/2011Mar31/Added/V2.0/IR-181_NFD1_Prod_Req_Part1
#include "VarFieldHandle.h" //TMSC-Tanoue/2010Aug25/MVC008345
#include "GPLibAdapter.h" //TMSC-Tanoue/2010Aug25/MVC008345
#include "PosSlabInfo.h"		//Patni/2011Mar31/Added/V2.0/IR-181_NFD1_Prod_Req_Part1


#define MS_CHECK_ALL_POINT_MODE		0
#define MS_CHECK_SLAB_CENTER_MODE	1
#define MS_CHECK_SLICE_CENTER_MODE	2

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCheckFOVInMap::CCheckFOVInMap()
{

}

CCheckFOVInMap::~CCheckFOVInMap()
{

}
//***************************Method Header*************************************
//Method Name    : PqmCheckFOVinMap()
//Author         : PATNI/MRP
//Purpose        :
//*****************************************************************************
status_t CCheckFOVInMap::PqmCheckFOVinMap(CVarFieldHandle* const vf_handle,
        const int32_t       spProtocol,
        const int32_t       mapProtocol,
        const int32_t       mode)

{
    LPCTSTR FUNC_NAME = _T("CCheckFOVInMap::PqmCheckFOVinMap");

    if (!vf_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("vf_handle is NULL"));
        return E_ERROR;
    }

    /*
    VfPathElem_t spPath[SVD_ST_SLICEGROUP];
    VfPathElem_t spVPath[SVD_ST_SLICEGROUP];
    VgSgMap_t    *spVgSgMapP=NULL;
    */
    VfPathElem_t spPath[SVD_ST_SLICEGROUP] = {0};
    VfPathElem_t spVPath[SVD_ST_SLICEGROUP] = {0};

    /*
    VfPathElem_t mapPath[SVD_ST_SLICEGROUP];
    VfPathElem_t mapVPath[SVD_ST_SLICEGROUP];
    VgSgMap_t    *mapVgSgMapP=NULL;
    */
    VfPathElem_t mapPath[SVD_ST_SLICEGROUP] = {0};
    VfPathElem_t mapVPath[SVD_ST_SLICEGROUP] = {0};
    //- Patni-PJS/2009Apr25/Modify/CPP Test tool execution

    /*  printf("spProtocol : %d, mapProtocol : %d\n",spProtocol,mapProtocol);*/

    spVPath[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    spVPath[SVD_ST_PROTOCOL - 1].index        = spProtocol;
    spVPath[SVD_ST_VISUALGROUP - 1].subtree_name = SVN_ST_VISUALGROUP;
    spVPath[SVD_ST_VISUALGROUP - 1].index        = 0;

    mapVPath[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    mapVPath[SVD_ST_PROTOCOL - 1].index        = mapProtocol;
    mapVPath[SVD_ST_VISUALGROUP - 1].subtree_name = SVN_ST_VISUALGROUP;
    mapVPath[SVD_ST_VISUALGROUP - 1].index        = 0;

    spPath[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    spPath[SVD_ST_PROTOCOL - 1].index        = spProtocol;
    spPath[SVD_ST_SUBPROTOCOL - 1].subtree_name = SVN_ST_SUBPROTOCOL;
    spPath[SVD_ST_SUBPROTOCOL - 1].index        = 0;
    spPath[SVD_ST_SLICEGROUP - 1].subtree_name  = SVN_ST_SLICEGROUP;
    spPath[SVD_ST_SLICEGROUP - 1].index         = 0;

    mapPath[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    mapPath[SVD_ST_PROTOCOL - 1].index        = mapProtocol;
    mapPath[SVD_ST_SUBPROTOCOL - 1].subtree_name = SVN_ST_SUBPROTOCOL;
    mapPath[SVD_ST_SUBPROTOCOL - 1].index        = 0;
    mapPath[SVD_ST_SLICEGROUP - 1].subtree_name  = SVN_ST_SLICEGROUP;
    mapPath[SVD_ST_SLICEGROUP - 1].index         = 0;

    int32_t    checkMode  = 0;
    status_t   status = E_NO_ERROR;

    if (mode == 0) {
        checkMode = MS_CHECK_ALL_POINT_MODE;

    } else if (mode == 1) {
        checkMode = MS_CHECK_SLAB_CENTER_MODE;

    } else if (mode == 2) {
        checkMode = MS_CHECK_SLICE_CENTER_MODE;

    } else {
        status = E_ERROR;
        //goto cleanUp;
        //FreeMem((void*)spPosSlabInfo,(void*)mapPosSlabInfo);
        return status;
    }

    ClibmsVfAdapter libmsvfAdapter(vf_handle);
    int32_t	  spnVisuals = 0;

    status = libmsvfAdapter.msVfCountSlabs(spProtocol, &spnVisuals);

    if_error(status) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("msVfCountSlabs() failed."));
        //goto cleanUp;
        //FreeMem((void*)spPosSlabInfo,(void*)mapPosSlabInfo);
        return status;
    }

    int32_t	  mapnVisuals = 0;

    status = libmsvfAdapter.msVfCountSlabs(mapProtocol, &mapnVisuals);

    if_error(status) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("msVfCountSlabs() failed."));
        //goto cleanUp;
        //FreeMem((void*)spPosSlabInfo,(void*)mapPosSlabInfo);
        return status;
    }

    printf("spnVisuals : %d, mapnVisuals : %d\n", spnVisuals, mapnVisuals);

    msPosSlabInfo_t* spPosSlabInfo = (msPosSlabInfo_t*)malloc(sizeof(msPosSlabInfo_t) * spnVisuals);

    //+Patni/Rajendra/2011Mar10/commented/TMSC-Review-Comments

    //  if (spPosSlabInfo == (msPosSlabInfo_t*)NULL) {		//Patni-RUP/2009Aug21/Modified/cpp test corrections Need to check

    if (spPosSlabInfo == NULL) { //Patni-Rajendra/2011Mar10/Added/TMSC-Review-Comments
        status = E_ERROR;
        //goto cleanUp;
        //FreeMem((void*)spPosSlabInfo,(void*)mapPosSlabInfo);
        return status;

    }

    //-Patni/Rajendra/2011Mar10/commented/TMSC-Review-Comments
    msPosSlabInfo_t* mapPosSlabInfo = (msPosSlabInfo_t*)malloc(sizeof(msPosSlabInfo_t) * mapnVisuals);

    if (mapPosSlabInfo == NULL) {		//-Patni-RUP/2009Aug21/Modified/cpp test corrections Need to check
        status = E_ERROR;

        FREE_PTR(spPosSlabInfo);

        return status;
    }


    /***********************************************************
     * SPEEDER MAPのVGは撮像済なのでホームポジション座標系
     * しかし本撮像のプロトコルはプランしている時には同じだが、
     * プラン情報が切れた場合は磁場中心座標系になる。
     * ここではそのOffset量を求める。　V7.00
     ***********************************************************/
    flt32_t	offset = 0.0f;
    status = calcOffsetValue(vf_handle, spProtocol, &offset);

    if_error(status) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("calcOffsetValue() failed."));
        //goto cleanUp;
        //FreeMem((void*)spPosSlabInfo,(void*)mapPosSlabInfo);

        FREE_PTR(spPosSlabInfo);

        FREE_PTR(mapPosSlabInfo);

        return status;
    }

    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    VfFieldSpec_t 	fieldSpec[10] = {0};	/* varfield specification */
    memset(fieldSpec, 0, sizeof(VfFieldSpec_t) * 10);

    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    int        numDone    = 0;
    int32_t    i = 0, j = 0, cntV = 0;

    for (cntV = 0; cntV < spnVisuals; cntV++) {
        /* Protocol Node */
        i = 0;

        //+Patni/2011Mar31/Modified/V2.0/IR-181_NFD1_Prod_Req_Part1
        f32vec2_t l_fov;
        VFF_SET_ARGS(fieldSpec[i], SVN_PLN_FOV, VFT_FLTVEC2,
                     sizeof(l_fov), &l_fov, 0);
        i++;

        status = vf_handle->GetFields(spPath, SVD_ST_PROTOCOL, fieldSpec, i, &numDone);
        spPosSlabInfo[cntV].plnFov.SetXY(l_fov.x, l_fov.y) ;
        //-Patni/2011Mar31/Modified/V2.0/IR-181_NFD1_Prod_Req_Part1

        if_error(status || numDone != i) {
            status = E_ERROR;

            //goto cleanUp;
            //FreeMem((void*)spPosSlabInfo,(void*)mapPosSlabInfo);
            FREE_PTR(spPosSlabInfo);

            FREE_PTR(mapPosSlabInfo);

            return status;
        }

        /* Slicegroup Node */
        // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
        memset(fieldSpec, 0, sizeof(VfFieldSpec_t) * 10);
        // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
        i = 0;
        VFF_SET_ARGS(fieldSpec[i], SVN_PLN_THICKNESS, VFT_FLT32,
                     sizeof spPosSlabInfo[cntV].sliceThickness, &spPosSlabInfo[cntV].sliceThickness, 0);
        i++;
        VFF_SET_ARGS(fieldSpec[i], SVN_PLN_SLICE_GAP, VFT_FLT32,
                     sizeof spPosSlabInfo[cntV].sliceGap, &spPosSlabInfo[cntV].sliceGap, 0);
        i++;

        status = vf_handle->GetFields(spPath, SVD_ST_SLICEGROUP, fieldSpec, i, &numDone);

        if_error(status || numDone != i) {
            status = E_ERROR;

            //goto cleanUp;
            //FreeMem((void*)spPosSlabInfo,(void*)mapPosSlabInfo);
            FREE_PTR(spPosSlabInfo);

            FREE_PTR(mapPosSlabInfo);

            return status;
        }

        /* Visualgroup Node */
        // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
        memset(fieldSpec, 0, sizeof(VfFieldSpec_t) * 10);
        // - SM4 HIMANSHU_INITIALISE 27 DEC 2008


        //+Patni/2011Mar31/Modified/V2.0/IR-181_NFD1_Prod_Req_Part1
        f32vec3_t l_offsetvec, l_slicevec, l_pevec;
        i = 0;
        VFF_SET_ARGS(fieldSpec[i], SVN_PLN_VG_OFFSET_VECTOR, VFT_FLTVEC3,
                     sizeof(l_offsetvec), &l_offsetvec, 0);
        i++;
        VFF_SET_ARGS(fieldSpec[i], SVN_PLN_VG_SLICE_ORI_VECTOR, VFT_FLTVEC3,
                     sizeof(l_slicevec), &l_slicevec, 0);
        i++;
        VFF_SET_ARGS(fieldSpec[i], SVN_PLN_VG_PHASE_ORI_VECTOR, VFT_FLTVEC3,
                     sizeof(l_pevec), &l_pevec, 0);
        i++;
        VFF_SET_ARGS(fieldSpec[i], SVN_PLN_SLICES_IN_VG, VFT_INT32,
                     sizeof spPosSlabInfo[cntV].sliceNum, &spPosSlabInfo[cntV].sliceNum, 0);
        i++;
        //+Patni-MP/2009Mar07/Commented/ACE-1#MCM0213-00092
        spVPath[SVD_ST_VISUALGROUP - 1].index = cntV;

        status = vf_handle->GetFields(spVPath, SVD_ST_VISUALGROUP, fieldSpec, i, &numDone);
        spPosSlabInfo[cntV].offsetVec = l_offsetvec;
        spPosSlabInfo[cntV].sliceVec = l_slicevec;
        spPosSlabInfo[cntV].peVec = l_pevec;
        //-Patni/2011Mar31/Modified/V2.0/IR-181_NFD1_Prod_Req_Part1

        if_error(status || numDone != i) {
            status = E_ERROR;

            //goto cleanUp;
            //FreeMem((void*)spPosSlabInfo,(void*)mapPosSlabInfo);
            FREE_PTR(spPosSlabInfo);

            FREE_PTR(mapPosSlabInfo);

            return status;
        }
        //+Patni/2011Mar31/Modified/V2.0/IR-181_NFD1_Prod_Req_Part1
        spPosSlabInfo[cntV].offsetVec.SetZ(spPosSlabInfo[cntV].offsetVec.GetZ() - offset)  ;
        /* リードアウト方向ベクトルの算出 */
        CPqmUtilities::calcReadOriVector(&spPosSlabInfo[cntV].peVec,
                                         &spPosSlabInfo[cntV].sliceVec,
                                         &spPosSlabInfo[cntV].roVec);

        /* make check info */
        msPosSlabInfo_t::makeCheckPointsInfo(&spPosSlabInfo[cntV]);
        //-Patni/2011Mar31/Modified/V2.0/IR-181_NFD1_Prod_Req_Part1

        /*    print_msPosSlabInfo(&spPosSlabInfo[cntV]);*/
    }

    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(fieldSpec, 0, sizeof(VfFieldSpec_t) * 10);

    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    for (cntV = 0; cntV < mapnVisuals; cntV++) {
        /* Protocol Node */
        i = 0;
        //+Patni/2011Mar31/Added+Modified/V2.0/IR-181_NFD1_Prod_Req_Part1
        f32vec2_t l_fov;
        VFF_SET_ARGS(fieldSpec[i], SVN_PLN_FOV, VFT_FLTVEC2,
                     sizeof(l_fov), &l_fov, 0);
        i++;

        status = vf_handle->GetFields(mapPath, SVD_ST_PROTOCOL, fieldSpec, i, &numDone);
        mapPosSlabInfo[cntV].plnFov.SetXY(l_fov.x, l_fov.y);
        //-Patni/2011Mar31/Added+Modified/V2.0/IR-181_NFD1_Prod_Req_Part1

        if_error(status || numDone != i) {
            status = E_ERROR;

            //goto cleanUp;
            //FreeMem((void*)spPosSlabInfo,(void*)mapPosSlabInfo);
            FREE_PTR(spPosSlabInfo);

            FREE_PTR(mapPosSlabInfo);

            return status;
        }

        /* Slicegroup Node */
        // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
        memset(fieldSpec, 0, sizeof(VfFieldSpec_t) * 10);
        // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
        i = 0;
        VFF_SET_ARGS(fieldSpec[i], SVN_PLN_THICKNESS, VFT_FLT32,
                     sizeof mapPosSlabInfo[cntV].sliceThickness, &mapPosSlabInfo[cntV].sliceThickness, 0);
        i++;
        VFF_SET_ARGS(fieldSpec[i], SVN_PLN_SLICE_GAP, VFT_FLT32,
                     sizeof mapPosSlabInfo[cntV].sliceGap, &mapPosSlabInfo[cntV].sliceGap, 0);
        i++;

        status = vf_handle->GetFields(mapPath, SVD_ST_SLICEGROUP, fieldSpec, i, &numDone);

        if_error(status || numDone != i) {
            status = E_ERROR;

            //goto cleanUp;
            //FreeMem((void*)spPosSlabInfo,(void*)mapPosSlabInfo);
            FREE_PTR(spPosSlabInfo);

            FREE_PTR(mapPosSlabInfo);

            return status;
        }

        /* Visualgroup Node */
        // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
        memset(fieldSpec, 0, sizeof(VfFieldSpec_t) * 10);
        // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
        i = 0;

        //+Patni/2011Mar31/Modified/V2.0/IR-181_NFD1_Prod_Req_Part1
        f32vec3_t l_offsetvec, l_slicevec, l_pevec;
        VFF_SET_ARGS(fieldSpec[i], SVN_PLN_VG_OFFSET_VECTOR, VFT_FLTVEC3,
                     sizeof(l_offsetvec), &l_offsetvec, 0);
        i++;
        VFF_SET_ARGS(fieldSpec[i], SVN_PLN_VG_SLICE_ORI_VECTOR, VFT_FLTVEC3,
                     sizeof(l_slicevec), &l_slicevec, 0);
        i++;
        VFF_SET_ARGS(fieldSpec[i], SVN_PLN_VG_PHASE_ORI_VECTOR, VFT_FLTVEC3,
                     sizeof(l_pevec), &l_pevec, 0);
        i++;
        VFF_SET_ARGS(fieldSpec[i], SVN_PLN_SLICES_IN_VG, VFT_INT32,
                     sizeof mapPosSlabInfo[cntV].sliceNum, &mapPosSlabInfo[cntV].sliceNum, 0);
        i++;
        //-Patni/2011Mar31/Modified/V2.0/IR-181_NFD1_Prod_Req_Part1

        status = vf_handle->GetFields(mapVPath, SVD_ST_VISUALGROUP, fieldSpec, i, &numDone);

        if_error(status || numDone != i) {
            status = E_ERROR;

            //goto cleanUp;
            //FreeMem((void*)spPosSlabInfo,(void*)mapPosSlabInfo);
            FREE_PTR(spPosSlabInfo);

            FREE_PTR(mapPosSlabInfo);

            return status;
        }

        //+Patni/2011Mar31/Modified/V2.0/IR-181_NFD1_Prod_Req_Part1
        mapPosSlabInfo[cntV].offsetVec = l_offsetvec;
        mapPosSlabInfo[cntV].sliceVec = l_slicevec;
        mapPosSlabInfo[cntV].peVec = l_pevec;


        /* リードアウト方向ベクトルの算出 */
        CPqmUtilities::calcReadOriVector(&mapPosSlabInfo[cntV].peVec,
                                         &mapPosSlabInfo[cntV].sliceVec,
                                         &mapPosSlabInfo[cntV].roVec);

        /* make check info */
        msPosSlabInfo_t::makeCheckPointsInfo(&mapPosSlabInfo[cntV]);
        //-Patni/2011Mar31/Modified/V2.0/IR-181_NFD1_Prod_Req_Part1

        /*print_msPosSlabInfo(&mapPosSlabInfo[cntV]);*/
    }

    /* compare spPosSlabInfo with mapPosSlabInfo */

    for (i = 0; i < mapnVisuals; i++) { /* map info (1 slab info) */
        for (j = 0; j < spnVisuals; j++) {/* speeder info (plural slab infos) */
            status = cmpMsPosSlabInfo(&spPosSlabInfo[j], &mapPosSlabInfo[i], checkMode);

            if (status == E_ERROR) {
                PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("calcOffsetValue() failed."));
                //FreeMem((void*)spPosSlabInfo,(void*)mapPosSlabInfo);
                FREE_PTR(spPosSlabInfo);

                FREE_PTR(mapPosSlabInfo);

                return status;
                //goto cleanUp;
            }
        }
    }

    // cleanUp:
    //  if(spPosSlabInfo != NULL) free(spPosSlabInfo);
    //  if(mapPosSlabInfo != NULL) free(mapPosSlabInfo);

    FREE_PTR(spPosSlabInfo);

    FREE_PTR(mapPosSlabInfo);

    return (status);

}

//***************************Method Header*************************************
//Method Name    : calcOffsetValue()
//Author         : PATNI/MRP
//Purpose        :
//*****************************************************************************
status_t CCheckFOVInMap::calcOffsetValue(CVarFieldHandle* const vf_handle,
        const int32_t      protocol,
        flt32_t*	offset)
{
    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    /*
    VfFieldSpec_t       vfReq[8];
    VfPathElem_t        path[SVD_ST_PROTOCOL];
    flt32_t		homePos, couchPos;
    VfPathElem_t	locImages[4];
    int32_t		patient_orient[2];
    int32_t		plan_status ;
    int			n,num;
    bool_t		plan_protocol;
    status_t		sts ;
    */
    VfFieldSpec_t       vfReq[8] = {0};
    VfPathElem_t        path[SVD_ST_PROTOCOL] = {0};
    flt32_t		homePos = 0.0f, couchPos = 0.0f;
    VfPathElem_t	locImages[4] = {0};
    int32_t		patient_orient[2] = {0};
    int32_t		plan_status = 0;
    int			n = 0;
    bool_t		plan_protocol = FALSE;
    //- Patni-PJS/2009Apr25/Modify/CPP Test tool execution

    /*  SVN_PLN_PATIENT_ORIENT[0] 患者挿入方向
        SVN_EXE_COUCH             現在の寝台位置
        SNV_EXE_HOME_POSITION     ホームポジション

        SVN_PLN_STATUS            pln_status
        SVN_PLN_LOCATOR_IMAGES    親画像有無
    */

    path[0].subtree_name = SVN_ST_PROTOCOL;
    path[0].index = protocol;

    memset(locImages , 0, sizeof(locImages)) ;
    memset(patient_orient , 0, sizeof(patient_orient)) ;
    memset(vfReq , 0, sizeof(vfReq)) ;
    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(vfReq, 0, sizeof(VfFieldSpec_t) * 8);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    n = 0 ;
    VFF_SET_ARGS(vfReq[0], SVN_EXE_HOME_POSITION,
                 SVT_EXE_HOME_POSITION, sizeof(homePos),
                 &homePos, 0);
    VFF_SET_ARGS(vfReq[1], SVN_EXE_COUCH,
                 SVT_EXE_COUCH, sizeof(couchPos),
                 &couchPos, 0);
    VFF_SET_ARGS(vfReq[2], SVN_PLN_PATIENT_ORIENT,
                 SVT_PLN_PATIENT_ORIENT, sizeof(patient_orient),
                 patient_orient, 0);
    VFF_SET_ARGS(vfReq[3], SVN_PLN_STATUS,
                 SVT_PLN_STATUS, sizeof(plan_status),
                 &plan_status, 0);
    VFF_SET_ARGS(vfReq[4], SVN_PLN_LOCATOR_IMAGES, SVT_PLN_LOCATOR_IMAGES,
                 sizeof(locImages), locImages, 0L);

    int num = 0;
    status_t sts = vf_handle->GetFields(path, SVD_ST_PROTOCOL, vfReq, 5, &num);

    if (sts == E_NO_ERROR) {
        if (vfReq[4].status == E_NO_ERROR && locImages[0].subtree_name > 0)
            plan_protocol = TRUE;

        else if (vfReq[3].status == E_NO_ERROR && (plan_status & VFC_PLN_STATUS_GLOC))
            plan_protocol = TRUE;

        if (plan_protocol == FALSE) {
            /* ここで磁場中心座標からホームポジション座標への
             Offset計算をおこなう */
            if (vfReq[0].status != E_NO_ERROR ||
                vfReq[1].status != E_NO_ERROR ||
                vfReq[2].status != E_NO_ERROR)	return(E_ERROR) ;

            if (patient_orient[0] == VFC_PATORI_HEAD_FIRST)
                *offset = (flt32_t)((homePos - couchPos) / 1000.0) ;

            else
                *offset = (flt32_t)((couchPos - homePos) / 1000.0) ;

        } else {
            *offset = 0 ;
        }
    }

    return sts;

}

//***************************Method Header*************************************
//Method Name    : cmpMsPosSlabInfo()
//Author         : PATNI/MRP
//Purpose        :
//*****************************************************************************
status_t CCheckFOVInMap::cmpMsPosSlabInfo(msPosSlabInfo_t* sppos,
        msPosSlabInfo_t* mappos,
        const int32_t         checkMode)
{
    status_t status = E_NO_ERROR;
    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    //int32_t i;
    int32_t i = 0;

    //+Patni/2011Mar31/Modified/V2.0/IR-181_NFD1_Prod_Req_Part1
    CVector3D spMaxPos(-99999, -99999, -99999);/* x, y, zの最大値だけをセット */
    CVector3D mapMaxPos(-99999, -99999, -99999);
    CVector3D spMinPos(99999, 99999, 99999);/* x, y, zの最小値だけをセット */
    CVector3D mapMinPos(99999, 99999, 99999);

    switch (checkMode) {
        case MS_CHECK_ALL_POINT_MODE : {
            for (i = 0; i < 8; i++) {
                Vector3DUtility::setMaxMinPosition(&sppos->eachPos[i], &spMaxPos, &spMinPos);
                Vector3DUtility::setMaxMinPosition(&mappos->eachPos[i], &mapMaxPos, &mapMinPos);
            }

        }
        break ;

        case MS_CHECK_SLAB_CENTER_MODE : {
            Vector3DUtility::setMaxMinPosition(&sppos->slabCenter, &spMaxPos, &spMinPos);

            for (i = 0; i < 8; i++) {
                Vector3DUtility::setMaxMinPosition(&mappos->eachPos[i], &mapMaxPos, &mapMinPos);
            }

        }
        break ;

        case MS_CHECK_SLICE_CENTER_MODE : {
            for (i = 0; i < 2; i++) {
                Vector3DUtility::setMaxMinPosition(&sppos->sliceCenter[i], &spMaxPos, &spMinPos);
            }

            for (i = 0; i < 8; i++) {
                Vector3DUtility::setMaxMinPosition(&mappos->eachPos[i], &mapMaxPos, &mapMinPos);
            }
        }
        break;

        default :
            return status ;
    }

#if 1
    printf("MAP : min - (%f, %f, %f), max - (%f, %f, %f)\n",
           mapMinPos.GetX(), mapMinPos.GetY(), mapMinPos.GetZ(),
           mapMaxPos.GetX(), mapMaxPos.GetY(), mapMaxPos.GetZ());
    printf("SP  : min - (%f, %f, %f), max - (%f, %f, %f)\n",
           spMinPos.GetX(), spMinPos.GetY(), spMinPos.GetZ(),
           spMaxPos.GetX(), spMaxPos.GetY(), spMaxPos.GetZ());
#endif

    /*if ((mapMinPos.x <= spMinPos.x) && (spMaxPos.x <= mapMaxPos.x) &&
    (mapMinPos.y <= spMinPos.y) && (spMaxPos.y <= mapMaxPos.y) &&
    (mapMinPos.z <= spMinPos.z) && (spMaxPos.z <= mapMaxPos.z)) {
    status = E_NO_ERROR;

      } else
      status = E_ERROR;
    */

    status = ((mapMinPos <= spMinPos) && (spMaxPos <= mapMaxPos)) ? E_NO_ERROR : E_ERROR ;

    //-Patni/2011Mar31/Modified/V2.0/IR-181_NFD1_Prod_Req_Part1

    return status;
}
void CCheckFOVInMap::FreeMem(void* ptr1, void* ptr2)
{
    if (ptr1 != NULL) {
        msPosSlabInfo_t* ptr3 = (msPosSlabInfo_t*)ptr1;
        FREE_PTR(ptr3);
    }

    FREE_PTR(ptr2);

}


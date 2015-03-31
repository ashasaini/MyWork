//REDMINE-1213_Update_03_MAY
//Redmine-781_CPP_TestTool
//Redmine-780
//Redmine-781
// CDSUtilities.cpp: implementation of the CCDSUtilities class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CDSUtilities.h"
#include <PQM/PQMLogMgr.h>
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"
#include "VarFieldHandle.h" //TMSC-Tanoue/2010Aug25/MVC008345


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//************************************Method Header************************************
// Method Name  : GetCdsCoordinate (V9.51)
// Author       : PATNI/ HEMANT
// Purpose      : CDS read the information (Pqm location of the coil was calculated)
//***********************************************************************************
bool CCDSUtilities::GetCdsCoordinate(
    CVarFieldHandle* const f_vf_handle,
    const int f_protocol,
    Vft_CdsCoilLocation_t* f_coordinatetbl
)
{
    LPCTSTR FUNC_NAME = _T("CCDSUtilities::GetCdsCoordinate");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, FUNC_NAME);

    if (!f_vf_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_vf_handle is NULL"));
        return false;
    }

    VfFieldSpec_t	vfReq;
    memset(&vfReq, 0, sizeof(VfFieldSpec_t));

    int l_size = sizeof(Vft_CdsCoilLocation_t) * PQM_NUM_COIL_PORT;
    VFF_SET_ARGS(vfReq,
                 SVN_PLN_CDS_COIL_COORDINATE,
                 SVT_PLN_CDS_COIL_COORDINATE,
                 l_size,
                 f_coordinatetbl,
                 0);

    int l_num = 0;

    VfPathElem_t	pelm[2] = {0};
    pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    pelm[SVD_ST_PROTOCOL - 1].index = f_protocol;
    status_t l_sts = f_vf_handle->GetFields(pelm, SVN_ST_PROTOCOL, &vfReq, 1, &l_num);

    if (l_sts != E_NO_ERROR || l_num != 1) {
        l_sts = E_ERROR ;	/* エラー時のチェックは？ */
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exiting CCDSUtilities::GetCdsCoordinate"));

    return l_sts != E_NO_ERROR ;
}


//************************************Method Header************************************
// Method Name  : GetCdsLocation (V9.51)
// Author       : PATNI/ HEMANT
// Purpose      : CDS read the information (acqman location of the coil is measured)
//***********************************************************************************
bool CCDSUtilities::GetCdsLocation(
    CVarFieldHandle* const f_vf_handle,
    const int f_protocol,
    Vft_CdsCoilLocation_t* f_locationtbl
)
{
    LPCTSTR FUNC_NAME = _T("CCDSUtilities::GetCdsLocation");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, FUNC_NAME);

    if (!f_vf_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_vf_handle is NULL"));
        return false;
    }

    VfFieldSpec_t	vfReq;
    memset(&vfReq, 0, sizeof(VfFieldSpec_t));

    int l_size = sizeof(Vft_CdsCoilLocation_t) * PQM_NUM_COIL_PORT;
    VFF_SET_ARGS(vfReq,
                 SVN_EXE_CDS_COIL_LOCATION,
                 SVT_EXE_CDS_COIL_LOCATION,
                 l_size,
                 f_locationtbl,
                 0);

    int l_num = 0;

    VfPathElem_t	pelm[2] = {0};
    pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    pelm[SVD_ST_PROTOCOL - 1].index = f_protocol ;
    status_t l_sts = f_vf_handle->GetFields(pelm, SVN_ST_PROTOCOL, &vfReq, 1, &l_num);

    if (l_sts != E_NO_ERROR || l_num != 1) {
        l_sts = E_ERROR ;	/* エラー時のチェックは？ */
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exiting CCDSUtilities::GetCdsLocation"));

    return l_sts != E_NO_ERROR ;
}


//************************************Method Header************************************
// Method Name  : GetCdsStatus (V9.51)
// Author       : PATNI/ HEMANT
// Purpose      : CDS load status information
//***********************************************************************************
bool CCDSUtilities::GetCdsStatus(
    CVarFieldHandle* const f_vf_handle,
    const int f_protocol,
    int* f_cds_status
)
{
    LPCTSTR FUNC_NAME = _T("CCDSUtilities::GetCdsStatus");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, FUNC_NAME);

    if (!f_vf_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_vf_handle is NULL"));
        return false;
    }

    VfFieldSpec_t	vfReq;
    memset(&vfReq, 0, sizeof(VfFieldSpec_t));
    int l_size = sizeof(int32_t);
    VFF_SET_ARGS(vfReq,
                 SVN_PLN_CDS_STATUS,
                 SVT_PLN_CDS_STATUS,
                 l_size,
                 f_cds_status,
                 0);

    int l_num = 0;

    VfPathElem_t	pelm[2] = {0};
    pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    pelm[SVD_ST_PROTOCOL - 1].index = f_protocol ;
    status_t l_sts = f_vf_handle->GetFields(pelm, SVN_ST_PROTOCOL, &vfReq, 1, &l_num);

    if (l_sts != E_NO_ERROR || l_num != 1) {
        l_sts = E_ERROR ;	/* エラー */
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exiting CCDSUtilities::GetCdsStatus"));

    return l_sts != E_NO_ERROR ;
}



//************************************Method Header************************************
// Method Name  : PutCdsCoordinate (V9.51)
// Author       : PATNI/ HEMANT
// Purpose      : CDS wrote the information (Pqm location of the coil was calculated)
//***********************************************************************************
bool CCDSUtilities::PutCdsCoordinate(
    CVarFieldHandle* const f_vf_handle,
    const int f_protocol,
    Vft_CdsCoilLocation_t* f_coordinatetbl
)
{
    LPCTSTR FUNC_NAME = _T("CCDSUtilities::PutCdsCoordinate");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, FUNC_NAME);

    if (!f_vf_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_vf_handle is NULL"));
        return false;
    }

    VfFieldSpec_t	vfReq;
    memset(&vfReq, 0, sizeof(VfFieldSpec_t));
    int l_size = sizeof(Vft_CdsCoilLocation_t) * PQM_NUM_COIL_PORT;

    VFF_SET_ARGS(vfReq,
                 SVN_PLN_CDS_COIL_COORDINATE,
                 SVT_PLN_CDS_COIL_COORDINATE,
                 l_size,
                 f_coordinatetbl,
                 VFO_REPLACE);

    int l_num = 0;

    time_t		tm = 0l;
    VfPathElem_t	pelm[2] = {0};
    pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    pelm[SVD_ST_PROTOCOL - 1].index = f_protocol ;

    status_t l_sts = f_vf_handle->PutFields(pelm, SVD_ST_PROTOCOL, &vfReq, 1, &l_num, &tm);

    if (l_sts != E_NO_ERROR || l_num != 1) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
        l_sts = E_ERROR ;	/* エラー */
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exiting CCDSUtilities::PutCdsCoordinate"));

    return l_sts != E_NO_ERROR ;
}


//************************************Method Header************************************
// Method Name  : PutCdsLocation (V9.51)
// Author       : PATNI/ HEMANT
// Purpose      : CDS wrote the information (debug for)
//***********************************************************************************
bool CCDSUtilities::PutCdsLocation(
    CVarFieldHandle* const f_vf_handle,
    const int f_protocol,
    Vft_CdsCoilLocation_t* f_locationtbl
)
{
    LPCTSTR FUNC_NAME = _T("CCDSUtilities::PutCdsLocation");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, FUNC_NAME);

    if (!f_vf_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_vf_handle is NULL"));
        return false;
    }

    VfFieldSpec_t	vfReq;
    memset(&vfReq, 0, sizeof(VfFieldSpec_t));

    int l_size = sizeof(Vft_CdsCoilLocation_t) * PQM_NUM_COIL_PORT;
    VFF_SET_ARGS(vfReq,
                 SVN_EXE_CDS_COIL_LOCATION,
                 SVT_EXE_CDS_COIL_LOCATION,
                 l_size,
                 f_locationtbl,
                 VFO_REPLACE);

    int l_num = 0;

    time_t		tm = 0l;
    VfPathElem_t	pelm[2] = {0};
    pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    pelm[SVD_ST_PROTOCOL - 1].index = f_protocol ;

    status_t l_sts = f_vf_handle->PutFields(pelm, SVD_ST_PROTOCOL, &vfReq, 1, &l_num, &tm);

    if (l_sts != E_NO_ERROR || l_num != 1) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() failed"));
        l_sts = E_ERROR ;	/* エラー */
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exiting CCDSUtilities::PutCdsLocation"));

    return (l_sts != E_NO_ERROR) ;
}


//************************************Method Header************************************
// Method Name  : PutCdsStatus (V9.51)
// Author       : PATNI/ HEMANT
// Purpose      : CDS wrote the information status
//***********************************************************************************
bool CCDSUtilities::PutCdsStatus(
    CVarFieldHandle* const f_vf_handle,
    const int f_protocol,
    int* f_cds_status
)
{
    LPCTSTR FUNC_NAME = _T("CCDSUtilities::PutCdsStatus");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, FUNC_NAME);

    if (!f_vf_handle) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_vf_handle is NULL"));
        return false;
    }

    VfFieldSpec_t	vfReq;
    memset(&vfReq, 0, sizeof(VfFieldSpec_t));

    int l_size = sizeof(int32_t);
    VFF_SET_ARGS(vfReq,
                 SVN_PLN_CDS_STATUS,
                 SVT_PLN_CDS_STATUS,
                 l_size,
                 f_cds_status,
                 VFO_REPLACE);

    int	l_num = 0;

    time_t		tm = 0l;
    VfPathElem_t	pelm[2] = {0};
    pelm[SVD_ST_PROTOCOL - 1].subtree_name = SVN_ST_PROTOCOL;
    pelm[SVD_ST_PROTOCOL - 1].index = f_protocol ;

    status_t l_sts = f_vf_handle->PutFields(pelm, SVD_ST_PROTOCOL, &vfReq, 1, &l_num, &tm);

    if (l_sts != E_NO_ERROR || l_num != 1) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() failed"));
        l_sts = E_ERROR ;	/* エラー */
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Exiting CCDSUtilities::PutCdsStatus"));

    return (l_sts != E_NO_ERROR) ;
}


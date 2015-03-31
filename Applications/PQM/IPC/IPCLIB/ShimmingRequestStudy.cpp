//REDMINE-1213_Update_03_MAY
//Redmine-778
//Redmine-781
// ShimmingRequestStudy.cpp: implementation of the CShimmingRequestStudy class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ShimmingRequestStudy.h"
#include <math.h>
#include <libvf/vfStudy/vfstudy.h>
#include <libvf/varfields.h>
#include <libvf/Common/bitfields.h>
#include <libvf/vfStudy/vfs_plan.h>
#include <libvf/vfStudy/vf_appcodes.h>
#include <libStudyManager/destroy.h>
#include <PQM/PQMLogMgr.h>
#include "ShimmingRequestStructures.h"
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"
#include "VarFieldHandle.h"

extern SShimGlobal_t*	 g_shimglobal;
extern SShimInfo_t   	s_shiminfo;

//****************************Method Header************************************
//Method Name   :CShimmingRequestStudy
//Author        :PATNI/
//Purpose       :
//*****************************************************************************
CShimmingRequestStudy::CShimmingRequestStudy(
)
{
    LPCTSTR FUNC_NAME = _T("CShimmingRequestStudy::CShimmingRequestStudy");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CShimmingRequestStudy::CShimmingRequestStudy"));
}

//****************************Method Header************************************
//Method Name   :CShimmingRequestStudy
//Author        :PATNI/
//Purpose       :
//*****************************************************************************
CShimmingRequestStudy::~CShimmingRequestStudy(
)
{
    LPCTSTR FUNC_NAME = _T("CShimmingRequestStudy::~CShimmingRequestStudy");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CShimmingRequestStudy::~CShimmingRequestStudy"));
}

///****************************Method Header************************************
//Method Name   :ShimStudyOpen
//Author        :PATNI/
//Purpose       :
//*****************************************************************************
BOOL CShimmingRequestStudy::ShimStudyOpen(
)
{
    LPCTSTR FUNC_NAME = _T("CShimmingRequestStudy::ShimStudyOpen");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CShimmingRequest::ShimStudyOpen"));

    CString l_tracemsg(_T(""));
    l_tracemsg.Format(_T("Shim Study file --> %s"), g_shimglobal->shimStudyP->study);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_tracemsg);

    DEL_PTR(g_shimglobal->shimStudyP->vfHandle);
    g_shimglobal->shimStudyP->vfHandle = new CVarFieldHandle(g_shimglobal->shimStudyP->study);

    if (g_shimglobal->shimStudyP->vfHandle->Open() != E_NO_ERROR) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T(" Unable to open shim Study file"));
        return FALSE;
    }

    return TRUE;
}

//****************************Method Header************************************
//Method Name   :ShimPutOffset
//Author        :PATNI/
//Purpose       :
//*****************************************************************************
BOOL CShimmingRequestStudy::ShimPutOffset(
    int f_typeformat_depth
)
{
    LPCTSTR FUNC_NAME = _T("CShimmingRequestStudy::ShimPutOffset");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CShimmingRequest::ShimPutOffset"));

    VfFieldSpec_t	l_reqflds[5];
    int			    n = 0;
    int			    l_numdone = 0;
    int 		    l_shim_coil[SHIM_COIL] = {0};
    int	    	    l_grad_coil[GRAD_CHANNEL] = {0};

    int     l_status = -1;

    if (g_shimglobal->shimInfoP->adjust_processed == -1) {
        //shimPrintf("%s : %d shimPutOffset() - adjust_processed Error \n", __FILE__, __LINE__);
        return FALSE;
    }

    VfPathElem_t*	l_processpathp = g_shimglobal->shimStudyP->imageNodes;
    l_processpathp[SVD_ST_PROCESSED - 1].index = g_shimglobal->shimInfoP->adjust_processed;

    f_typeformat_depth = SVD_ST_PROCESSED;

    l_processpathp[SVD_ST_ECHO - 1].index = 0;
    l_processpathp[SVD_ST_POSITION - 1].index = 0;

    //    shimPrintf("%s : %d shimPutOffset() - GetOffsetPath = /1.%d/7.%d \n", __FILE__, __LINE__,
    //                processPathP[SVD_ST_PROTOCOL-1].index,
    //                processPathP[SVD_ST_PROCESSED-1].index);

    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(l_reqflds, 0, sizeof(VfFieldSpec_t) * 5);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008

    VFF_SET_ARGS(l_reqflds[n], SVN_SEQ_SHIM_OFFSET, SVT_SEQ_SHIM_OFFSET,
                 sizeof(int)*SHIM_COIL, l_shim_coil, 0);
    n++;
    VFF_SET_ARGS(l_reqflds[n], SVN_SEQ_SHIM_GRAD_OFFSET, SVT_SEQ_SHIM_GRAD_OFFSET,
                 sizeof(int)*GRAD_CHANNEL, l_grad_coil, 0);
    n++;

    try {
        l_status = g_shimglobal->shimStudyP->vfHandle->GetFields(l_processpathp,
                   f_typeformat_depth, l_reqflds, n, &l_numdone);

    } catch (...) {
        CString	l_print_string = _T("");
        l_print_string.Format(_T("VfGetFields Crashed with handle=%x/Path=/1.%d/RequestCnt=%d/ReturnCnt=%d"),
                              g_shimglobal->shimStudyP->vfHandle, f_typeformat_depth[0].index, n, l_numdone);

        PQM_LOG(DOMAIN_NAME, USER_FUNC_MARKER,
                _T("CShimmingRequestStudy::ShimPutOffset"), l_print_string);
        throw;
    }

    if (l_status != E_NO_ERROR || l_numdone != n) {
        //shimPrintf("%s : %d shimPutOffset() - VfGetFields Error \n", __FILE__, __LINE__);
        return FALSE;
    }

    //    {
    //	for(int i = 0; i< SHIM_COIL;i++)
    //            shimPrintf("\t shimCoil[%d] = %d \n", i, shim_coil[i]);
    //	for(i = 0; i< GRAD_CHANNEL;i++)
    //            shimPrintf("\t gradCoil[%d] = %d \n", i, grad_coil[i]);
    //    }

    n = 0;
    f_typeformat_depth = SVD_ST_STUDY;
    VFF_SET_ARGS(l_reqflds[n], SVN_SEQ_SHIM_OFFSET, SVT_SEQ_SHIM_OFFSET,
                 sizeof(int)*SHIM_COIL, l_shim_coil, VFO_REPLACE);
    n++;
    VFF_SET_ARGS(l_reqflds[n], SVN_SEQ_SHIM_GRAD_OFFSET, SVT_SEQ_SHIM_GRAD_OFFSET,
                 sizeof(int)*GRAD_CHANNEL, l_grad_coil, VFO_REPLACE);
    n++;

    //+Patni-SS/2009Apr01/PSP1#VFLOCK
    CString l_log_msg = _T("");
    l_log_msg.Format(_T("SVN_SEQ_SHIM_OFFSET=%d, SVN_SEQ_SHIM_GRAD_OFFSET = %d"), l_shim_coil, l_grad_coil);

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_log_msg);
    //-Patni-SS/2009Apr01/PSP1#VFLOCK

    int l_status = -1;

    //+Patni-Manish/2009Sept02/Added/VFLOG
    try {
        l_status = g_shimglobal->shimStudyP->vfHandle->PutFields(NULL,
                   f_typeformat_depth, l_reqflds, n, &l_numdone, NULL);

    } catch (...) {
        CString	l_print_string = _T("");
        l_print_string.Format(_T("VfPutFields Crashed with handle=%x/Path=NULL/RequestCnt=%d/ReturnCnt=%d"),
                              g_shimglobal->shimStudyP->vfHandle, n, l_numdone);

        PQM_LOG(DOMAIN_NAME, USER_FUNC_MARKER,
                _T("CShimmingRequestStudy::ShimPutOffset"), l_print_string);
        throw;
    }

    //-Patni-Manish/2009Sept02/Added/VFLOG

    if (l_status != E_NO_ERROR || l_numdone != n) {
        //shimPrintf("%s : %d shimPutOffset() - VfPutFields Error \n", __FILE__, __LINE__);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
        return FALSE;
    }

    return TRUE;
}
//****************************Method Header************************************
//Method Name   :ShimGetZ2
//Author        :PATNI/
//Purpose       :
//*****************************************************************************
BOOL CShimmingRequestStudy::ShimGetZ2(
    int* f_z2
)
{
    LPCTSTR FUNC_NAME = _T("CShimmingRequestStudy::ShimGetZ2");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of ShimGetZ2"));

    VfFieldSpec_t	l_reqflds[5];
    int			    n = 0;
    int			    l_numdone = 0;
    int             l_typeformatdepth = SVD_ST_STUDY;
    int 		    l_shim_coil[SHIM_COIL] = {0};
    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(l_reqflds, 0, sizeof(VfFieldSpec_t) * 5);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    VFF_SET_ARGS(l_reqflds[n], SVN_SEQ_SHIM_OFFSET, SVT_SEQ_SHIM_OFFSET,
                 sizeof(int)*SHIM_COIL, l_shim_coil, 0);
    n++;

    int l_status = -1;

    try {
        l_status = g_shimglobal->shimStudyP->vfHandle->GetFields(NULL,
                   l_typeformatdepth, l_reqflds, n, &l_numdone);

    } catch (...) {
        CString	l_print_string = _T("");
        l_print_string.Format(_T("VfGetFields Crashed with handle=%x/Path=/1.%d/RequestCnt=%d/ReturnCnt=%d"),
                              g_shimglobal->shimStudyP->vfHandle, l_typeformatdepth, n, l_numdone);

        PQM_LOG(DOMAIN_NAME, USER_FUNC_MARKER,
                _T("CShimmingRequestStudy::ShimGetZ2"), l_print_string);
        throw;
    }

    if (l_status != E_NO_ERROR || l_numdone != n) {
        //shimPrintf("%s : %d shimGetZ2() - Result Read Error \n", __FILE__, __LINE__);
        return FALSE;
    }

    *f_z2 = l_shim_coil[CHANNEL_Z2];

    return TRUE;
}

//****************************Method Header************************************
//Method Name   :ShimPutFsyVal
//Author        :PATNI/
//Purpose       :
//*****************************************************************************
BOOL CShimmingRequestStudy::ShimPutFsyVal(
    int f_typeformatdepth
)
{
    LPCTSTR FUNC_NAME = _T("CShimmingRequestStudy::ShimPutFsyVal");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of ShimPutFsyVal"));

    VfFieldSpec_t	l_reqflds[5];
    int			    n = 0;
    int			    l_numdone = -1;
    VfPathElem_t*	l_processpathp;
    double		    l_fsy_value = 0.0;

    int	 l_status = -1;

    if (g_shimglobal->shimInfoP->adjust_processed == -1) {
        //shimPrintf("%s : %d shimPutFsyVal() - adjust_processed Error \n", __FILE__, __LINE__);
        return FALSE;
    }

    l_processpathp = g_shimglobal->shimStudyP->imageNodes;

    l_processpathp[SVD_ST_PROCESSED - 1].index = g_shimglobal->shimInfoP->adjust_processed;

    f_typeformatdepth = SVD_ST_PROCESSED;

    l_processpathp[SVD_ST_ECHO - 1].index = 0;
    l_processpathp[SVD_ST_POSITION - 1].index = 0;

    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(l_reqflds, 0, sizeof(VfFieldSpec_t) * 5);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008

    VFF_SET_ARGS(l_reqflds[n], SVN_EXE_SHIM_FSY_VALUE, SVT_EXE_SHIM_FSY_VALUE,
                 sizeof(double), &l_fsy_value, 0);
    n++;

    //    shimPrintf("%s : %d shimPutFsyVal() - GetFsytPath = /1.%d/7.%d \n", __FILE__, __LINE__,
    //		processPathP[SVD_ST_PROTOCOL-1].index,
    //		processPathP[SVD_ST_PROCESSED-1].index);

    try {
        l_status = g_shimglobal->shimStudyP->vfHandle->GetFields(l_processpathp,
                   f_typeformatdepth, l_reqflds, n, &l_numdone);

    } catch (...) {
        CString	l_print_string = _T("");
        l_print_string.Format(_T("VfGetFields Crashed with handle=%x/Path=/1.%d/RequestCnt=%d/ReturnCnt=%d"),
                              g_shimglobal->shimStudyP->vfHandle, f_typeformatdepth, n, l_numdone);

        PQM_LOG(DOMAIN_NAME, USER_FUNC_MARKER,
                _T("CShimmingRequestStudy::ShimPutFsyVal"), l_print_string);
        throw;
    }

    if (l_status != E_NO_ERROR || l_numdone != n) {
        //shimPrintf("%s : %d shimPutFsyVal() - fsy_value VfGetFields Error \n", __FILE__, __LINE__);
        return FALSE;
    }

    //shimPrintf("%s : %d shimPutFsyVal() - B0 Default: %f \n", __FILE__, __LINE__, fsy_value);
    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(l_reqflds, 0, sizeof(VfFieldSpec_t) * 5);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    n = 0;
    f_typeformatdepth = SVD_ST_STUDY;

    VFF_SET_ARGS(l_reqflds[n], SVN_EXE_SHIM_FSY_VALUE, SVT_EXE_SHIM_FSY_VALUE,
                 sizeof(double), &l_fsy_value, VFO_REPLACE);
    n++;
    //+Patni-SS/2009Apr01/PSP1#VFLOCK
    CString l_log_msg = _T("");
    l_log_msg.Format(_T("SVN_SEQ_SHIM_OFFSET=%d, SVN_SEQ_SHIM_GRAD_OFFSET = %d"), l_shim_coil, l_grad_coil);

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, l_log_msg);
    //-Patni-SS/2009Apr01/PSP1#VFLOCK

    int l_status = -1;

    //+Patni-Manish/2009Sept02/Added/VFLOG
    try {
        l_status = g_shimglobal->shimStudyP->vfHandle->PutFields(NULL,
                   f_typeformatdepth, l_reqflds, n, &l_numdone, NULL) ;

    } catch (...) {
        CString	l_print_string = _T("");
        l_print_string.Format(_T("VfPutFields Crashed with handle=%x/Path=NULL/RequestCnt=%d/ReturnCnt=%d"),
                              g_shimglobal->shimStudyP->vfHandle, n, l_numdone);

        PQM_LOG(DOMAIN_NAME, USER_FUNC_MARKER,
                _T("CShimmingRequestStudy::ShimPutFsyVal"), l_print_string);
        throw;
    }

    //-Patni-Manish/2009Sept02/Added/VFLOG

    if (l_status != E_NO_ERROR || l_numdone != n) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PutFields() Failed."));
        //shimPrintf("%s : %d shimPutFsyVal() - fsy_value VfPutFields Error \n", __FILE__, __LINE__);
        return FALSE;
    }

    return TRUE;
}

//****************************Method Header************************************
//Method Name   :ShimReadTotalSlices
//Author        :PATNI/LK
//Purpose       :
//*****************************************************************************
void CShimmingRequestStudy::ShimReadTotalSlices(
)
{
    LPCTSTR FUNC_NAME = _T("CShimmingRequestStudy::ShimReadTotalSlices");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of ShimReadTotalSlices"));


    if (g_shimglobal->shimInfoP->totalSlice == -1) {
        int                 n = 0;
        VfFieldSpec_t       l_reqflds[5];
        BOOL                l_status = FALSE;
        VfPathElem_t*        l_processpathp = g_shimglobal->shimStudyP->imageNodes;
        int l_typeformatdepth = SVD_ST_PROTOCOL;

        // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
        memset(l_reqflds, 0, sizeof(VfFieldSpec_t) * 5);
        // - SM4 HIMANSHU_INITIALISE 27 DEC 2008

        VFF_SET_ARGS(l_reqflds[n], SVN_PLN_TOTAL_SLICES, SVT_PLN_TOTAL_SLICES,
                     sizeof(int), &g_shimglobal->shimInfoP->totalSlice, 0);
        n++;
        int                 l_numdone = -1;

        try {
            l_status = g_shimglobal->shimStudyP->vfHandle->GetFields(l_processpathp,
                       l_typeformatdepth, l_reqflds, n, &l_numdone);

        } catch (...) {
            CString	l_print_string = _T("");
            l_print_string.Format(_T("VfGetFields Crashed with handle=%x/Path=/1.%d/RequestCnt=%d/ReturnCnt=%d"),
                                  g_shimglobal->shimStudyP->vfHandle, l_processpathp[0].index, n, l_numdone);

            PQM_LOG(DOMAIN_NAME, USER_FUNC_MARKER,
                    _T("CShimmingRequestStudy::ShimReadTotalSlices"), l_print_string);
            throw;
        }


        if (l_status != E_NO_ERROR || l_numdone != n) {
            g_shimglobal->shimInfoP->totalSlice = FIELDPROFILEIMAGES;
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("Failed to read SVN_PLN_TOTAL_SLICES from shim study"));
        }
    }

    CString l_tracemsg(_T(""));
    l_tracemsg.Format(_T("Total Slices Read from Study file --> %d"), g_shimglobal->shimInfoP->totalSlice);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_tracemsg);
}

//****************************Method Header************************************
//Method Name   :ShimInfoRead
//Author        :PATNI/
//Purpose       :
//*****************************************************************************
BOOL CShimmingRequestStudy::ShimInfoRead(
)
{

    LPCTSTR FUNC_NAME = _T("CShimmingRequestStudy::ShimInfoRead");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of ShimInfoRead"));

    ShimReadTotalSlices();

    if (g_shimglobal->shimStudyP->shimInfo_mode != SHIM_MANUAL_MODE &&
        g_shimglobal->shimStudyP->shimInfo_mode != SHIM_AUTO_MODE) {

        CString l_tracemsg(_T(""));
        l_tracemsg.Format(_T("Failed to set Mode --> %d"), g_shimglobal->shimStudyP->shimInfo_mode);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_tracemsg);
        return FALSE;
    }

    return TRUE;
}

//****************************Method Header************************************
//Method Name   :ShimFlagRead
//Author        :PATNI/
//Purpose       :
//*****************************************************************************
BOOL CShimmingRequestStudy::ShimFlagRead(
)
{

    LPCTSTR FUNC_NAME = _T("CShimmingRequestStudy::ShimFlagRead");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of ShimFlagRead"));


    if (g_shimglobal->shimInfoP->shim_flag == -1) {
        int             n = 0;
        VfFieldSpec_t	l_reqflds[5];
        int			    l_numdone = -1;
        BOOL    		l_status = FALSE;

        VfPathElem_t*	l_processpathp = g_shimglobal->shimStudyP->imageNodes;
        int l_typeformatdepth = SVD_ST_PROTOCOL;

        // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
        memset(l_reqflds, 0, sizeof(VfFieldSpec_t) * 5);
        // - SM4 HIMANSHU_INITIALISE 27 DEC 2008

        VFF_SET_ARGS(l_reqflds[n], SVN_SEQ_SHIM_FLAG, SVT_SEQ_SHIM_FLAG,
                     sizeof(int), &g_shimglobal->shimInfoP->shim_flag, 0);
        n++;

        try {
            l_status = g_shimglobal->shimStudyP->vfHandle->GetFields(l_processpathp,
                       l_typeformatdepth, l_reqflds, n, &l_numdone);

        } catch (...) {
            CString	l_print_string = _T("");
            l_print_string.Format(_T("VfGetFields Crashed with handle=%x/Path=/1.%d/RequestCnt=%d/ReturnCnt=%d"),
                                  g_shimglobal->shimStudyP->vfHandle, l_processpathp[0].index, n, l_numdone);

            PQM_LOG(DOMAIN_NAME, USER_FUNC_MARKER,
                    _T("CShimmingRequestStudy::ShimFlagRead"), l_print_string);
            throw;
        }

        if (l_status != E_NO_ERROR || l_numdone != n) {
            g_shimglobal->shimInfoP->shim_flag = VFC_SHIM_STANDARD;

            CString l_tracemsg(_T(""));
            l_tracemsg.Format(_T("Failed to read Shim Flag SVN_SEQ_SHIM_FLAG and default is set to VFC_SHIM_STANDARD"));
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_tracemsg);
        }
    }

    int l_flg = g_shimglobal->shimInfoP->shim_flag;
    CString l_shimflg(_T(""));

    if (l_flg == VFC_SHIM_NONE) {
        l_shimflg.Format(_T("Shim Flag --> VFC_SHIM_NONE"));

    } else if (l_flg == VFC_SHIM_STANDARD) {
        l_shimflg.Format(_T("Shim Flag --> VFC_SHIM_STANDARD"));

    } else if (l_flg == VFC_SHIM_HIGH_RESOLUTION) {
        l_shimflg.Format(_T("Shim Flag --> VFC_SHIM_HIGH_RESOLUTION"));
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_shimflg);
    return TRUE;
}

//****************************Method Header************************************
//Method Name   :ShimDataKindCheck
//Author        :PATNI/
//Purpose       :
//*****************************************************************************
BOOL CShimmingRequestStudy::ShimDataKindCheck(
)
{
    LPCTSTR FUNC_NAME = _T("CShimmingRequestStudy::ShimDataKindCheck");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of ShimDataKindCheck"));


    if (g_shimglobal->shimInfoP->calc_processed == -1) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("calc_processed Error"));
        return FALSE;
    }

    VfPathElem_t*        l_processpathp = g_shimglobal->shimStudyP->imageNodes;
    l_processpathp[SVD_ST_PROCESSED - 1].index = g_shimglobal->shimInfoP->calc_processed;
    l_processpathp[SVD_ST_ECHO - 1].index = 0;
    l_processpathp[SVD_ST_POSITION - 1].index = 0;

    int l_typeformatdepth = SVD_ST_PROCESSED;

    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    VfFieldSpec_t       l_reqflds[5];
    memset(l_reqflds, 0, sizeof(VfFieldSpec_t) * 5);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008

    int                 n = 0;
    int                  l_datakind = -1;
    VFF_SET_ARGS(l_reqflds[n], SVN_PRC_DATA_KIND, SVT_PRC_DATA_KIND,
                 sizeof(int), &l_datakind, 0);
    n++;

    int l_status = -1;
    int                 l_numdone = -1;

    try {
        l_status = g_shimglobal->shimStudyP->vfHandle->GetFields(l_processpathp,
                   l_typeformatdepth, l_reqflds, n, &l_numdone);

    } catch (...) {
        CString	l_print_string = _T("");
        l_print_string.Format(_T("VfGetFields Crashed with handle=%x/Path=/1.%d/RequestCnt=%d/ReturnCnt=%d"),
                              g_shimglobal->shimStudyP->vfHandle, l_processpathp[0].index, n, l_numdone);

        PQM_LOG(DOMAIN_NAME, USER_FUNC_MARKER,
                _T("CShimmingRequestStudy::ShimDataKindCheck"), l_print_string);
        throw;
    }

    if (l_status != E_NO_ERROR || l_numdone != n) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Data Kind Read Error"));
        return FALSE;
    }

    if (l_datakind != VFC_PROC_SHIM_CALC) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("data Not CALC"));
        return FALSE;
    }

    return TRUE;
}

//****************************Method Header************************************
//Method Name   :ShimPositionRead
//Author        :PATNI/
//Purpose       :
//*****************************************************************************
BOOL CShimmingRequestStudy::ShimPositionRead(
)
{
    LPCTSTR FUNC_NAME = _T("CShimmingRequestStudy::ShimPositionRead");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of ShimPositionRead"));


    SShimStudy_t*	l_studyp = g_shimglobal->shimStudyP;
    VfPathElem_t*	l_imagepathp = l_studyp->nodes;
    VfPathElem_t*	l_processpathp = l_studyp->imageNodes;
    position_t*		l_posp = g_shimglobal->posiDataP;

    l_imagepathp[SVD_ST_SUBPROTOCOL - 1].index = 0;
    l_imagepathp[SVD_ST_SLICEGROUP - 1].index = 0;
    l_imagepathp[SVD_ST_SLICE - 1].index = 0;
    l_imagepathp[SVD_ST_IMAGE - 1].index = 0;

    l_processpathp[SVD_ST_PROCESSED - 1].index = 0;
    l_processpathp[SVD_ST_ECHO - 1].index = 0;
    l_processpathp[SVD_ST_POSITION - 1].index = 0;

    //    shimPrintf("%s : %d shimPositionRead() - image /1.%d,process:/1.%d/7.%d/8.%d \n",
    //		__FILE__, __LINE__,
    //	        imagePathP[SVD_ST_PROTOCOL-1].index,
    //	        processPathP[SVD_ST_PROTOCOL-1].index,
    //	        processPathP[SVD_ST_PROCESSED-1].index,
    //	        processPathP[SVD_ST_ECHO-1].index);
    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    VfFieldSpec_t	l_reqflds[5];
    memset(l_reqflds, 0, sizeof(VfFieldSpec_t) * 5);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    int			    n = 0;
    VFF_SET_ARGS(l_reqflds[n], SVN_PLN_FOV, SVT_PLN_FOV, sizeof(f32vec2_t), &l_posp->fovVec, 0);
    n++;

    VFF_SET_ARGS(l_reqflds[n], SVN_PLN_TOTAL_SLICES, SVT_PLN_TOTAL_SLICES, sizeof(int),
                 &l_posp->sliceNum, 0);
    n++;

    VFF_SET_ARGS(l_reqflds[n], SVN_PLN_SELECTED_SLICE_GAP, SVT_PLN_SELECTED_SLICE_GAP, sizeof(float),
                 &l_posp->sliceGap, 0);
    n++;

    int l_status = -1;
    int	l_typeformatdepth =  SVD_ST_PROTOCOL; // Protocol Node Data Fov & Total Slice Read ;
    int	l_numdone = -1;

    try {
        l_status = g_shimglobal->shimStudyP->vfHandle->GetFields(l_imagepathp,
                   l_typeformatdepth, l_reqflds, n, &l_numdone);

    } catch (...) {
        CString	l_print_string = _T("");
        l_print_string.Format(_T("VfGetFields Crashed with handle=%x/Path=/1.%d/RequestCnt=%d/ReturnCnt=%d"),
                              g_shimglobal->shimStudyP->vfHandle, l_imagepathp[0].index, n, l_numdone);

        PQM_LOG(DOMAIN_NAME, USER_FUNC_MARKER,
                FUNC_NAME, l_print_string);
        throw;
    }

    if (l_status != E_NO_ERROR || l_numdone != n) {
        //shimPrintf("%s : %d shimPositionRead() - FOV Read Error \n", __FILE__, __LINE__);
        return FALSE;
    }

    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(l_reqflds, 0, sizeof(VfFieldSpec_t) * 5);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    n = 0;
    l_typeformatdepth = SVD_ST_SLICEGROUP;

    VFF_SET_ARGS(l_reqflds[n], SVN_PLN_OFFSET_VECTOR, SVT_PLN_OFFSET_VECTOR, sizeof(f32vec3_t),
                 &l_posp->offsetVec, 0);
    n++;

    VFF_SET_ARGS(l_reqflds[n], SVN_PLN_SLICE_ORI_VECTOR, SVT_PLN_SLICE_ORI_VECTOR, sizeof(f32vec3_t),
                 &l_posp->sliceVec, 0);
    n++;

    VFF_SET_ARGS(l_reqflds[n], SVN_PLN_THICKNESS, SVT_PLN_THICKNESS, sizeof(float),
                 &l_posp->thickness, 0);
    n++;

    VFF_SET_ARGS(l_reqflds[n], SVN_PLN_PHASE_ORI_VECTOR, SVT_PLN_PHASE_ORI_VECTOR, sizeof(f32vec3_t),
                 &l_posp->peVec, 0);
    n++;

    try {
        l_status = g_shimglobal->shimStudyP->vfHandle->GetFields(l_imagepathp,
                   l_typeformatdepth, l_reqflds, n, &l_numdone);

    } catch (...) {
        CString	l_print_string = _T("");
        l_print_string.Format(_T("VfGetFields Crashed with handle=%x/Path=/1.%d/RequestCnt=%d/ReturnCnt=%d"),
                              g_shimglobal->shimStudyP->vfHandle, l_imagepathp[0].index, n, l_numdone);

        PQM_LOG(DOMAIN_NAME, USER_FUNC_MARKER, FUNC_NAME, l_print_string);
        throw;
    }

    if (l_status != E_NO_ERROR || l_numdone != n) {
        //shimPrintf("%s : %d shimPositionRead() - OFFSET & SLICE_ORI Read Error \n", __FILE__, __LINE__);
        return FALSE;
    }

    // + SM4 HIMANSHU_INITIALISE 27 DEC 2008
    memset(l_reqflds, 0, sizeof(VfFieldSpec_t) * 5);
    // - SM4 HIMANSHU_INITIALISE 27 DEC 2008
    n = 0;
    l_typeformatdepth = SVD_ST_ECHO;
    VFF_SET_ARGS(l_reqflds[n], SVN_PRC_CEN_TO_CEN_DIST, SVT_PRC_CEN_TO_CEN_DIST, sizeof(float),
                 &l_posp->cen_to_cen_dist, 0);
    n++;

    try {
        l_status = g_shimglobal->shimStudyP->vfHandle->GetFields(l_processpathp,
                   l_typeformatdepth, l_reqflds, n, &l_numdone);

    } catch (...) {
        CString	l_print_string = _T("");
        l_print_string.Format(_T("VfGetFields Crashed with handle=%x/Path=/1.%d/RequestCnt=%d/ReturnCnt=%d"),
                              g_shimglobal->shimStudyP->vfHandle, l_processpathp[0].index, n, l_numdone);

        PQM_LOG(DOMAIN_NAME, USER_FUNC_MARKER, FUNC_NAME, l_print_string);
        throw;
    }


    if (l_status != E_NO_ERROR || l_numdone != n) {
        //shimPrintf("%s : %d shimPositionRead() - CEN_TO_CEN_DIST Read Error \n", __FILE__, __LINE__);
        return FALSE;
    }

    float l_domain = (l_posp->sliceNum - 1) / 2.0 * l_posp->cen_to_cen_dist + (l_posp->thickness / 2.0);

    switch (Direction(l_posp->sliceVec.x, l_posp->sliceVec.y, l_posp->sliceVec.z)) {

        case SHIMAXICAL:
            l_studyp->startPosi.z 	= l_posp->offsetVec.z - l_domain;
            l_studyp->endPosi.z 	= l_posp->offsetVec.z + l_domain;
            l_studyp->startPosi.x 	= l_posp->offsetVec.x - l_posp->fovVec.x / 2.0;
            l_studyp->endPosi.x 	= l_posp->offsetVec.x + l_posp->fovVec.x / 2.0;
            l_studyp->startPosi.y 	= l_posp->offsetVec.y - l_posp->fovVec.y / 2.0;
            l_studyp->endPosi.y 	= l_posp->offsetVec.y + l_posp->fovVec.y / 2.0;
            break;

        case SHIMSAGITAL:
            l_studyp->startPosi.x 	= l_posp->offsetVec.x - l_domain;
            l_studyp->endPosi.x 	= l_posp->offsetVec.x + l_domain;
            l_studyp->startPosi.z 	= l_posp->offsetVec.z - l_posp->fovVec.y / 2.0;
            l_studyp->endPosi.z 	= l_posp->offsetVec.z + l_posp->fovVec.y / 2.0;
            l_studyp->startPosi.y 	= l_posp->offsetVec.y - l_posp->fovVec.x / 2.0;
            l_studyp->endPosi.y 	= l_posp->offsetVec.y + l_posp->fovVec.x / 2.0;
            break;

        case SHIMCORONAL:
            l_studyp->startPosi.y 	= l_posp->offsetVec.y - l_domain;
            l_studyp->endPosi.y 	= l_posp->offsetVec.y + l_domain;
            l_studyp->startPosi.z 	= l_posp->offsetVec.z - l_posp->fovVec.y / 2.0;
            l_studyp->endPosi.z 	= l_posp->offsetVec.z + l_posp->fovVec.y / 2.0;
            l_studyp->startPosi.x 	= l_posp->offsetVec.x - l_posp->fovVec.x / 2.0;
            l_studyp->endPosi.x 	= l_posp->offsetVec.x + l_posp->fovVec.x / 2.0;
            break;

        default:
            return E_SHIM_SLICE_ORI_ERROR;
    }

    //    shimPrintf("\t offsetVec = %f %f %f \n", posP->offsetVec.x, posP->offsetVec.y, posP->offsetVec.z);
    //    shimPrintf("\t sliceVec  = %f %f %f \n", posP->sliceVec.x,  posP->sliceVec.y,  posP->sliceVec.z);
    //    shimPrintf("\t fovVec    = %f %f \n",    posP->fovVec.x,    posP->fovVec.y);
    //    shimPrintf("\t sliceNum  = %d \n",       posP->sliceNum);
    //    shimPrintf("\t thicknes  = %f \n",       posP->thickness);
    //    shimPrintf("\t cen_to_cen_dist = %f \n", posP->cen_to_cen_dist);
    //    shimPrintf("\t start - %f %f %f \n",     studyP->startPosi.x, studyP->startPosi.y, studyP->startPosi.z);
    //    shimPrintf("\t end   - %f %f %f \n",     studyP->endPosi.x,   studyP->endPosi.y,   studyP->endPosi.z);

    return TRUE;
}
//****************************Method Header************************************
//Method Name   :Direction
//Author        :PATNI/
//Purpose       :
//*****************************************************************************
int CShimmingRequestStudy::Direction(
    float x,
    float y,
    float z
)
{

    LPCTSTR FUNC_NAME = _T("CShimmingRequestStudy::Direction");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of Direction"));

    double fx, fy, fz;
    fx = fy = fz = 0.0;

    fx = fabs(x);
    fy = fabs(y);
    fz = fabs(z);

    if ((0.0 < fx && 1.0 > fx) || (0.0 < fy && 1.0 > fy) ||
        (0.0 < fz && 1.0 > fz))
        return SHIM_SURFACE_ERROR;

    if (fx >= fy && fx >= fz)
        return SHIMSAGITAL;

    if (fy >= fx && fy >= fz)
        return SHIMCORONAL;

    if (fz >= fx && fz >= fy)
        return SHIMAXICAL;

    return FALSE;
}


//****************************Method Header************************************
//Method Name   :ShimStudyClose
//Author        :PATNI/
//Purpose       :
//*****************************************************************************
void CShimmingRequestStudy::ShimStudyClose(
)
{
    LPCTSTR FUNC_NAME = _T("CShimmingRequestStudy::ShimStudyClose");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of ShimStudyClose"));

    DEL_PTR(g_shimglobal->shimStudyP->vfHandle);
}
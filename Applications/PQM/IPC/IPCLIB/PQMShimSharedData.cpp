//Redmine-783
//Redmine-774
//Redmine-778
//Redmine-779
//Redmine-780
//Redmine-782
// PQMShim.cpp: implementation of the CPQMShim class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PqmShimSharedData.h"

#include <errno.h>
#include <sys/types.h>
#include <tami/vfStudy/vf_appcodes.h>
#include <PQM/PQMLogMgr.h>
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"
#ifdef __NUTC__
#define __NC_INTERNAL__
#endif

CPqmShimSharedData* CPqmShimSharedData::m_pqmshimshareddata = NULL;

CPqmShimSharedData* CPqmShimSharedData::GetInstance(
)
{
    if (m_pqmshimshareddata == NULL) {
        m_pqmshimshareddata = new CPqmShimSharedData;
    }

    return m_pqmshimshareddata;
}

//*******************************Method Header*********************************
//Method Name    :~CPqmShim
//Author         :PATNI/LK
//Purpose        :destructor
//*****************************************************************************
CPqmShimSharedData::~CPqmShimSharedData(
)
{
    LPCTSTR FUNC_NAME = _T("CPqmShimSharedData::~CPqmShimSharedData");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("destructor of CPqmShimSharedData"));

    //Patni-HAR/2009Aug11/Added/Memory Leaks
    DEL_PTR(m_pqmshimshareddata);
}

//*******************************Method Header*********************************
//Method Name    :InitShim
//Author         :PATNI/LK
//Purpose        :
//*****************************************************************************
BOOL CPqmShimSharedData::InitShim(
)
{
    LPCTSTR FUNC_NAME = _T("CPqmShimSharedData::InitShim");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of InitShim"));

    m_shimstate	       = SHIM_STATE_WAIT;
    m_shimfsyflag      = VFC_SHIM_FSY_WATER;

    memset(m_activeshimpath, 0, sizeof(VfPathElem_t) * SVD_ST_PROCESSED);
    memset(m_activehrshimpath, 0, sizeof(VfPathElem_t) * SVD_ST_PROCESSED);

    return TRUE;
}

//*******************************Method Header*********************************
//Method Name    :CopyCouchInfo
//Author         :PATNI/LK
//Purpose        :
//*****************************************************************************
BOOL CPqmShimSharedData::CopyCouchInfo(
    int*     f_scannumber,
    float*   f_couchpos,
    float*   f_homepos
)
{
    LPCTSTR FUNC_NAME = _T("CPqmShimSharedData::CopyCouchInfo");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CopyCouchInfo"));

    bool set_scan_number = false, set_couch_pos =  false, set_home_pos = false;	//Patni-MJC/2009Aug17/Modified/cpp test corrections

    if (f_scannumber) {				//Patni-MJC/2009Aug17/Modified/cpp test corrections
        *f_scannumber = m_scannumber;
        set_scan_number = true;		//Patni-MJC/2009Aug17/Modified/cpp test corrections
    }

    if (f_couchpos) {					//Patni-MJC/2009Aug17/Modified/cpp test corrections
        *f_couchpos   = m_couchpos;
        set_couch_pos = true;		//Patni-MJC/2009Aug17/Modified/cpp test corrections
    }

    if (f_homepos) {					//Patni-MJC/2009Aug17/Modified/cpp test corrections
        *f_homepos    = m_homepos;
        set_home_pos = true;		//Patni-MJC/2009Aug17/Modified/cpp test corrections
    }

    if (set_scan_number && set_couch_pos && set_home_pos) {	//Patni-MJC/2009Aug17/Modified/cpp test corrections
        return TRUE;

    } else {							//Patni-MJC/2009Aug17/Modified/cpp test corrections
        return FALSE;					//Patni-MJC/2009Aug17/Modified/cpp test corrections
    }

    return TRUE;
}
//*******************************Method Header*********************************
//Method Name    :CopyBackCouchInfo
//Author         :PATNI/LK
//Purpose        :
//*****************************************************************************
BOOL CPqmShimSharedData::CopyBackCouchInfo(
    const int     f_scannumber,
    const float   f_couchpos,
    const float   f_homepos
)
{
    LPCTSTR FUNC_NAME = _T("CPqmShimSharedData::CopyBackCouchInfo");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CopyBackCouchInfo"));

    m_scannumber = f_scannumber;
    m_couchpos   = f_couchpos;
    m_homepos    = f_homepos;

    return TRUE;
}

//*******************************Method Header*********************************
//Method Name    :CopyTuneInfo
//Author         :PATNI/LK
//Purpose        :
//*****************************************************************************
BOOL CPqmShimSharedData::CopyTuneInfo(
    float*   f_txtunepos,
    float*   f_rflpos,
    float*	f_cfapos    //Patni-DKH/2010Feb18/Added/Phase3#MCM0229-00187
)
{
    LPCTSTR FUNC_NAME = _T("CPqmShimSharedData::CopyTuneInfo");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CopyTuneInfo"));

    bool set_tune_pos = false, set_rfl_pos =  false , set_cfa_pos = false;//Patni-MJC/2009Aug17/Modified/cpp test corrections

    if (f_txtunepos) {			//Patni-MJC/2009Aug17/Modified/cpp test corrections
        *f_txtunepos = m_txtunepos;
        set_tune_pos = true;	//Patni-MJC/2009Aug17/Modified/cpp test corrections
    }

    if (f_rflpos) {				//Patni-MJC/2009Aug17/Modified/cpp test corrections
        *f_rflpos    = m_rflpos;
        set_rfl_pos = true;		//Patni-MJC/2009Aug17/Modified/cpp test corrections
    }

    //Patni-DKH/2010Feb18/Added/Phase3#MCM0229-00187
    if (f_cfapos) {
        *f_cfapos = m_cfaPos;
        set_cfa_pos = true;
    }

    if (set_tune_pos && set_rfl_pos && set_cfa_pos) {	//Patni-DKH/2010Feb18/Added/Phase3#MCM0229-00187
        return TRUE;

    } else {							//Patni-MJC/2009Aug17/Modified/cpp test corrections
        return FALSE;					//Patni-MJC/2009Aug17/Modified/cpp test corrections
    }
}

//*******************************Method Header*********************************
//Method Name    :ShimRegistCouchInfo
//Author         :PATNI/LK
//Purpose        :
//*****************************************************************************
BOOL CPqmShimSharedData::ShimRegistCouchInfo(
    int*    	f_scannumber,
    float*	f_couchpos,
    float*	f_homepos
)
{
    LPCTSTR FUNC_NAME = _T("CPqmShimSharedData::ShimRegistCouchInfo");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of ShimRegistCouchInfo"));

    if (f_scannumber == NULL) {
        m_scannumber = -1;
        m_couchpos   = PQM_COUCH_INIT_POSITION;
        m_homepos    = PQM_COUCH_INIT_POSITION;

    } else {
        if (m_scannumber != *f_scannumber) {
            m_scannumber = *f_scannumber;

            if (f_couchpos != NULL) {
                m_couchpos   = *f_couchpos;
            }

            if (f_couchpos != NULL) {
                m_homepos    = *f_couchpos;
            }

        } else {
            if (f_couchpos != NULL) {
                m_couchpos   = *f_couchpos;
            }
        }

        if (f_homepos != NULL) {
            *f_homepos = m_homepos;
        }
    }

    return TRUE;
}

//*******************************Method Header*********************************
//Method Name    :ShimRegistTuneInfo
//Author         :PATNI/LK
//Purpose        :
//*****************************************************************************
BOOL CPqmShimSharedData::ShimRegistTuneInfo(
    float*   f_txtunepos,
    float*   f_rflpos,
    //Patni-DKH/2010Feb18/Added/Phase3#MCM0229-00187
    float*   f_cfapos
)
{

    LPCTSTR FUNC_NAME = _T("CPqmShimSharedData::ShimRegistTuneInfo");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of ShimRegistTuneInfo"));

    if (f_txtunepos != NULL) {
        m_txtunepos = *f_txtunepos;
    }

    if (f_rflpos != NULL) {
        m_rflpos = *f_rflpos;
    }

    //Patni-DKH/2010Feb18/Added/Phase3#MCM0229-00187
    if (f_cfapos != NULL) {
        m_cfaPos = *f_cfapos;
    }

    //Patni-DKH/2010Feb18/Added/Phase3#MCM0229-00187
    return TRUE;
}

//*******************************Method Header*********************************
//Method Name    :ShimRegistStudy
//Author         :PATNI/LK
//Purpose        :
//*****************************************************************************
BOOL CPqmShimSharedData::ShimRegistStudy(
    const BOOL            f_isimaging,
    study_id_t*      f_study_id,
    study_id_t*      f_another_study,
    bool*            f_conflict,
    bool*            f_samestudy
)
{
    LPCTSTR FUNC_NAME = _T("CPqmShimSharedData::ShimRegistStudy");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of ShimRegistStudy"));

    if (f_conflict) {
        *f_conflict = TRUE;
    }

    if (f_samestudy) {
        *f_samestudy = FALSE;
    }

    if (f_isimaging) {
        if (!m_scheduleactive || memcmp(&(m_schedule_study), f_study_id, sizeof(study_id_t))
           ) {
            if (f_conflict) {
                *f_conflict = FALSE;
            }

            if (!memcmp(&m_imaging_study, f_study_id, sizeof(study_id_t))) {
                if (f_samestudy) {
                    *f_samestudy = TRUE;
                }

            } else {
                memcpy(&m_imaging_study, f_study_id, sizeof(study_id_t));
            }

            m_currentprotocol = -1;
        }

        memcpy(f_another_study, &m_schedule_study, sizeof(study_id_t));
        m_imagingactive = 1;

    } else {
        if (!m_imagingactive || memcmp(&m_imaging_study, f_study_id, sizeof(study_id_t))
           ) {
            if (f_conflict) {
                *f_conflict = FALSE;
            }

            if (!memcmp(&m_schedule_study, f_study_id, sizeof(study_id_t))) {
                if (f_samestudy) {
                    *f_samestudy = TRUE;
                }

            } else {
                memcpy(&m_schedule_study, f_study_id, sizeof(study_id_t));
            }
        }

        memcpy(f_another_study, &m_imaging_study, sizeof(study_id_t));
        m_scheduleactive = 1;
    }

    return TRUE;
}

//*******************************Method Header*********************************
//Method Name    :ShimRegistProtocol
//Author         :PATNI/LK
//Purpose        :
//*****************************************************************************
BOOL CPqmShimSharedData::ShimRegistProtocol(
    const int f_protocol
)
{

    LPCTSTR FUNC_NAME = _T("CPqmShimSharedData::ShimRegistProtocol");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of ShimRegistProtocol"));

    m_currentprotocol = f_protocol;

    return TRUE;
}

BOOL CPqmShimSharedData::CopyShimPos(float*	f_shimpos)
{
    LPCTSTR FUNC_NAME = _T("CPqmShimSharedData::CopyShimPos");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CopyShimPos"));

    if (f_shimpos) {
        *f_shimpos = m_shimPos;
        return TRUE;

    } else {
        return FALSE;
    }
}

//*******************************Method Header*********************************
//Method Name    :GetScheduleActive
//Author         :PATNI/RAJ
//Purpose        :
//*****************************************************************************
study_id_t  CPqmShimSharedData::GetScheduleActive() const
{
    return m_schedule_study;
}

//*******************************Method Header*********************************
//Method Name    :GetImagingActive
//Author         :PATNI/RAJ
//Purpose        :
//*****************************************************************************
study_id_t  CPqmShimSharedData::GetImagingActive() const
{
    return  m_imaging_study;
}

int CPqmShimSharedData::GetCurrentProtocolNumber() const
{
    return m_currentprotocol;
}
// Patni - HAR / 2009Mar18 / Added / PSP Defect Sheet : Defect#212
//+Patni-RAJ/2010Jan18/Added/PH#3#/IR -86/Debug Window
//Patni-RAJ/2010Apr16/Commented/PH#3# IR86_CDR_11
//*******************************Method Header*********************************
//Method Name    :GetScanNumber
//Author         :PATNI/RAJ
//Purpose        :
//*****************************************************************************
int CPqmShimSharedData::GetScanNumber() const
{
    return m_scannumber;
}

//*******************************Method Header*********************************
//Method Name    :GetCouchPos
//Author         :PATNI/RAJ
//Purpose        :
//*****************************************************************************
float CPqmShimSharedData::GetCouchPos() const
{
    return m_couchpos;
}

//*******************************Method Header*********************************
//Method Name    :GetHomePos
//Author         :PATNI/RAJ
//Purpose        :
//*****************************************************************************
float CPqmShimSharedData::GetHomePos() const
{
    return m_homepos;
}

//*******************************Method Header*********************************
//Method Name    :Gettxtunepos
//Author         :PATNI/RAJ
//Purpose        :
//*****************************************************************************
float CPqmShimSharedData::Gettxtunepos() const
{
    return m_txtunepos;
}

//*******************************Method Header*********************************
//Method Name    :Getrflpos
//Author         :PATNI/RAJ
//Purpose        :
//*****************************************************************************
float CPqmShimSharedData::Getrflpos() const
{
    return m_rflpos;
}

//*******************************Method Header*********************************
//Method Name    :GetShimState
//Author         :PATNI/LK
//Purpose        :
//*****************************************************************************
BOOL CPqmShimSharedData::GetShimState(
    int* f_shimstate
)
{
    LPCTSTR FUNC_NAME = _T("CPqmShimSharedData::GetShimState");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetShimState"));

    if (f_shimstate) {			//Patni-MJC/2009Aug17/Modified/cpp test corrections
        *f_shimstate = m_shimstate;
        return TRUE;

    } else {
        return FALSE;
    }
}

//*******************************Method Header*********************************
//Method Name    :SetShimState
//Author         :PATNI/LK
//Purpose        :
//*****************************************************************************
BOOL CPqmShimSharedData::SetShimState(
    const int f_shimstate
)
{
    LPCTSTR FUNC_NAME = _T("CPqmShimSharedData::SetShimState");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetShimState"));
    m_shimstate = f_shimstate;
    return TRUE;
}

//*******************************Method Header*********************************
//Method Name    :GetShimFsyFlag
//Author         :PATNI/LK
//Purpose        :
//*****************************************************************************
BOOL CPqmShimSharedData::GetShimFsyFlag(
    int*	f_shimfsyflag
)
{
    LPCTSTR FUNC_NAME = _T("CPqmShimSharedData::GetShimFsyFlag");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetShimFsyFlag"));

    if (f_shimfsyflag) {			//Patni-MJC/2009Aug17/Modified/cpp test corrections
        *f_shimfsyflag = m_shimfsyflag;
        return TRUE;

    } else {
        return FALSE;
    }
}

//*******************************Method Header*********************************
//Method Name    :SetShimFsyFlag
//Author         :PATNI/LK
//Purpose        :
//*****************************************************************************
BOOL CPqmShimSharedData::SetShimFsyFlag(
    const int f_shimfsyflag
)
{
    LPCTSTR FUNC_NAME = _T("CPqmShimSharedData::SetShimFsyFlag");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetShimFsyFlag"));

    m_shimfsyflag = f_shimfsyflag;
    return TRUE;
}

//*******************************Method Header*********************************
//Method Name    :GetShimPath
//Author         :PATNI/LK
//Purpose        :
//*****************************************************************************
BOOL CPqmShimSharedData::GetShimPath(
    VfPathElem_t* const	f_shimpath
)
{
    LPCTSTR FUNC_NAME = _T("CPqmShimSharedData::GetShimPath");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetShimPath"));

    memcpy(f_shimpath, m_activeshimpath, sizeof(VfPathElem_t) * SVD_ST_PROCESSED);
    return TRUE;
}
//*******************************Method Header*********************************
//Method Name    :SetShimPath
//Author         :PATNI/LK
//Purpose        :
//*****************************************************************************
BOOL CPqmShimSharedData::SetShimPath(
    VfPathElem_t* const   f_shimpath
)
{
    LPCTSTR FUNC_NAME = _T("CPqmShimSharedData::SetShimPath");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetShimPath"));

    if (f_shimpath == NULL) {
        memset(m_activeshimpath, 0, sizeof(VfPathElem_t) * SVD_ST_PROCESSED);

    } else {
        memcpy(m_activeshimpath, f_shimpath, sizeof(VfPathElem_t) * SVD_ST_PROCESSED);
    }

    return TRUE;
}
//*******************************Method Header*********************************
//Method Name    :GetHRShimPath
//Author         :PATNI/LK
//Purpose        :
//*****************************************************************************
BOOL CPqmShimSharedData::GetHRShimPath(
    VfPathElem_t* const	f_shimpath
)
{
    LPCTSTR FUNC_NAME = _T("CPqmShimSharedData::GetHRShimPath");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetHRShimPath"));

    memcpy(f_shimpath, m_activehrshimpath, sizeof(VfPathElem_t) * SVD_ST_PROCESSED);
    return TRUE;
}
//*******************************Method Header*********************************
//Method Name    :SetHRShimPath
//Author         :PATNI/LK
//Purpose        :
//*****************************************************************************
BOOL CPqmShimSharedData::SetHRShimPath(
    VfPathElem_t* const    f_shimpath
)
{
    LPCTSTR FUNC_NAME = _T("CPqmShimSharedData::SetHRShimPath");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetHRShimPath"));

    if (f_shimpath == NULL) {
        memset(m_activehrshimpath, 0, sizeof(VfPathElem_t) * SVD_ST_PROCESSED);

    } else {
        memcpy(m_activehrshimpath, f_shimpath, sizeof(VfPathElem_t) * SVD_ST_PROCESSED);
    }

    return TRUE;
}

//*******************************Method Header*********************************
//Method Name    :GetCfaPos
//Author         :PATNI/LK
//Purpose        :
//*****************************************************************************
BOOL CPqmShimSharedData::GetCfaPos(
    float* f_cfapos
)
{
    LPCTSTR FUNC_NAME = _T("CPqmShimSharedData::GetCfaPos");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetCfaPos"));

    if (f_cfapos) {			//Patni-MJC/2009Aug17/Modified/cpp test corrections
        *f_cfapos = m_cfaPos;
        return TRUE;

    } else {
        return FALSE;
    }
}

BOOL CPqmShimSharedData::SetCfaPos(
    const float f_cfapos
)
{

    LPCTSTR FUNC_NAME = _T("CPqmShimSharedData::SetCfaPos");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetCfaPos"));
    m_cfaPos = f_cfapos;
    return TRUE;
}

//*******************************Method Header*********************************
//Method Name    :GetShimState
//Author         :PATNI/LK
//Purpose        :
//*****************************************************************************
BOOL CPqmShimSharedData::GetShimPos(
    float* f_shimpos
)
{
    LPCTSTR FUNC_NAME = _T("CPqmShimSharedData::GetShimPos");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetShimPos"));

    if (f_shimpos) {			//Patni-MJC/2009Aug17/Modified/cpp test corrections
        *f_shimpos = m_shimPos;
        return TRUE;

    } else {
        return FALSE;
    }
}

BOOL CPqmShimSharedData::SetShimPos(
    const float f_shimpos
)
{
    LPCTSTR FUNC_NAME = _T("CPqmShimSharedData::SetShimPos");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetShimPos"));
    m_shimPos = f_shimpos;
    return TRUE;
}

//-Patni-DKH/2010Feb18/Added/Phase3#MCM0229-00187

//*******************************Method Header*********************************
//Method Name    :CPqmShim
//Author         :PATNI/LK
//Purpose        :constructor
//*****************************************************************************
CPqmShimSharedData::CPqmShimSharedData():
    m_scheduleactive(0),
    m_imagingactive(0),
    m_currentprotocol(-1),
    m_scannumber(-1),
    m_couchpos(PQM_COUCH_INIT_POSITION),
    m_homepos(PQM_COUCH_INIT_POSITION),
    m_txtunepos(PQM_COUCH_INIT_POSITION),
    m_rflpos(PQM_COUCH_INIT_POSITION),
    m_shimstate(SHIM_STATE_WAIT),
    m_shimfsyflag(VFC_SHIM_FSY_WATER),
    m_cfaPos(0.0f),
    m_shimPos(0.0f)
{
    LPCTSTR FUNC_NAME = _T("CPqmShimSharedData::CPqmShimSharedData");
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, _T("constructor of CPqmShimSharedData"));

    m_schedule_study.run_num    = -1;
    m_schedule_study.site_id    = -1;

    m_imaging_study.run_num     = -1;
    m_imaging_study.site_id     = -1;

    memset(m_activeshimpath, 0, sizeof(VfPathElem_t) * SVD_ST_PROCESSED);
    memset(m_activehrshimpath, 0, sizeof(VfPathElem_t) * SVD_ST_PROCESSED);
}

//*******************************Method Header*********************************
//Method Name    :ShimClear
//Author         :PATNI/LK
//Purpose        :
//*****************************************************************************
BOOL CPqmShimSharedData::ShimClear(
    const bool	f_isimaging
)
{
    LPCTSTR FUNC_NAME = _T("CPqmShimSharedData::ShimClear");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of ShimClear"));

#if 0

    if (f_isimaging) {
        m_imaging_study.run_num = -1;
        m_imaging_study.site_id = -1;
        protocol	  = -1;

    } else {
        m_schedule_study.run_num = -1;
        m_schedule_study.site_id = -1;
    }

#else

    if (f_isimaging) {
        m_imagingactive = 0;

    } else {
        m_scheduleactive = 0;
    }

#endif
    return TRUE;
}

//BOOL PqmShmDebug(void	(*debug_func)(char *,...))
//{
//    if(ShimStart() != TRUE)
//	return FALSE;
//
//    debug_func("---SCHEDULE---");
//    debug_func("shm.study       = Run%d.%d",
//	       pqmShm_p->schedule.run_num,pqmShm_p->schedule.site_id);
//    debug_func("active          = %d",pqmShm_p->scheduleActive);
//    debug_func("---IMAGING----");
//    debug_func("shm.study       = Run%d.%d",
//	       pqmShm_p->imaging.run_num,pqmShm_p->imaging.site_id);
//    debug_func("active          = %d",pqmShm_p->imagingActive);
//    debug_func("protocol        = %d",pqmShm_p->protocol);
//    debug_func("shm.scanNumner  = %d",pqmShm_p->scanNumber);
//    debug_func("shm.couchPos    = %f",pqmShm_p->couchPos);
//    debug_func("shm.homePos     = %f",pqmShm_p->homePos);
//    debug_func("shm.txtunePos   = %f",pqmShm_p->txtunePos);
//    debug_func("shm.rflPos      = %f",pqmShm_p->rflPos);
//
//    return ShimEnd();
//}
//
///***********************************************************************
// * PqmShmShimDebug
// ***********************************************************************/
//status_t PqmShmShimDebug(void	(*debug_func)(char *,...))
//{
//    if(pqmShmStart() != TRUE)
//	return FALSE;
//
//    debug_func("shimState        = %d",pqmShm_p->shimState);
//    debug_func("shimFsyFlag      = %d",pqmShm_p->shimFsyFlag);
//    debug_func("activeShimPath   = /%d.%d/%d.%d",
//	       pqmShm_p->activeShimPath[0].subtree_name,
//	       pqmShm_p->activeShimPath[0].index,
//	       pqmShm_p->activeShimPath[1].subtree_name,
//	       pqmShm_p->activeShimPath[1].index);
//    debug_func("activeHRShimPath = /%d.%d/%d.%d",
//	       pqmShm_p->activeHRShimPath[0].subtree_name,
//	       pqmShm_p->activeHRShimPath[0].index,
//	       pqmShm_p->activeHRShimPath[1].subtree_name,
//	       pqmShm_p->activeHRShimPath[1].index);
//
//    return pqmShmEnd();
//}


/*********************************************************************/
/* (c) 1989 Toshiba Corporation All Rights Reserved
   NAME
   seminit - semaphore initialize
   SYNOPSIS
   LONG	seminit(semno,value)
   INT	semno		(i) : id in the semaphore group
   INT	value		(i) : semaphore initial value
   LONG	seminit()	(r) : status
   FUNCTION
   This module makes the semaphore and semaphore group.
   If it succeed, it will return the 'NORMAL' status, If not
   ,return the 'ERROR' status.
   REVISION
   1.00	Kouji Ohta	23.Mar.1989	original
   */
/* 2.00	K.Kokubun	09.May.1995	changed */
/* 2.01 N.Furudate	21.Sep.1995	Modified for memory */
/* 2.02 N.Furudate	19.Mar.1997	Modified for pqm */

//static status_t	seminit(int32_t	semno,
//			int32_t	value)
//{
//    status_t	status;	/* operaiton status */
//    int32_t	semid;	/* semaphore identifier */
//    union	semnum {
//	int32_t		value;
//	struct semid_ds sem;
//	uint16_t	array[1];
//    } arg;
//    DB_FUNC_ENTER("[seminit]:");
//
//    status = E_NO_ERROR ;
//    if((semid = semget(PQM_SEM_KEY,PQM_SEM_NUM,
//		       IPC_CREAT|IPC_EXCL|00666)) != OS_ERROR)
//    {
//	arg.value = value;
//	if (semctl(semid,semno,SETVAL,arg) == OS_ERROR)
//	{
//	    DB_PRINTF("",("semctl : %s\n",strerror(errno)));
//	    status = E_ERROR;
//	}
//    }
//    else
//    {
//	DB_FUNC_EXIT();
//	if(errno == EEXIST) return E_NO_ERROR;
//	DB_PRINTF("",("semget : %s\n",strerror(errno)));
//	status = E_ERROR;
//    }
//    DB_FUNC_EXIT();
//    return(status) ;
//}
//
///* (c) 1989 Toshiba Corporation All Rights Reserved
//   NAME
//   semaphore - semaphore operation
//   SYNOPSIS
//   LONG	semaphore(semno,cmd)
//   INT	semno		(i) : semaphore no
//   INT	cmd		(i) : command ( P or V )
//   LONG	semaphore()	(r) : return status
//   FUNCTION
//   This module controles the sempahore.
//   REVISION
//   1.00	Kouji Ohta	23.Mar.1989	original
//   */
///* 2.00	K.Kokubun	09.May.1995 changed */
///* 2.01 N.Furudate	21.Sep.1995 Modified for memory */
///* 2.02 N.Furudate	19.Mar.1997	Modified for pqm */
//
//static status_t	semaphore(int32_t	semno,
//			  int32_t	cmd)
//{
//    status_t	status;
//    int32_t	semid;
//    int32_t	nsops;
//    struct	sembuf	sops[CNTLUNIT];
//    DB_FUNC_ENTER("[semaphore]:");
//
//    status = E_NO_ERROR;
//    if ((semid = semget(PQM_SEM_KEY,PQM_SEM_NUM,
//		      IPC_ALLOC|00666)) != OS_ERROR)
//    {
//	nsops = CNTLUNIT ;
//
//#ifdef __NUTC__
//	sops[0].sem_num = (unsigned short)(semno);	/* semaphore no */
//	sops[0].sem_op =(unsigned short)( cmd); /* P or V operation */
//#else
//	sops[0].sem_num = semno;	/* semaphore no */
//	sops[0].sem_op = cmd;	        /* P or V operation */
//#endif
//
//	sops[0].sem_flg = SEM_UNDO;	/* semaphore flag */
//	do {
//	    status = semop(semid,sops,nsops) ;
//	} while((status == (-1)) && (errno == EINTR)) ;
//    }
//    else
//    {
//	status = E_ERROR;
//    }
//    DB_FUNC_EXIT();
//    return(status);
//}
//
///* (c) 1989 Toshiba Corporation All Rights Reserved
//   NAME
//   semaphore - semaphore operation
//   SYNOPSIS
//   LONG	semaphore(semno,cmd)
//   INT	semno		(i) : semaphore no
//   INT	cmd		(i) : command ( P or V )
//   LONG	semaphore()	(r) : return status
//   FUNCTION
//   This module controles the sempahore.
//   REVISION
//   1.00	Kouji Ohta	23.Mar.1989	original
//   */
///* 2.00	K.Kokubun	09.May.1995 changed */
///* 2.01 N.Furudate	21.Sep.1995 Modified for memory
//   recreate appending NO_WAIT to original semaphore function */
///* 2.02 N.Furudate	19.Mar.1997	Modified for pqm */
//
//static status_t	semaphore_nowait(int32_t	semno,
//				 int32_t	cmd)
//{
//    status_t	status;
//    int32_t	semid;
//    int32_t	nsops;
//    struct	sembuf	sops[CNTLUNIT];
//
//    status = E_NO_ERROR;
//    if ((semid = semget(PQM_SEM_KEY,PQM_SEM_NUM,
//		      IPC_ALLOC|00666)) != OS_ERROR)
//    {
//	nsops = CNTLUNIT ;
//
//
//#ifdef __NUTC__
//	sops[0].sem_num = (unsigned short)(semno);		/* semaphore no */
//	sops[0].sem_op =(short)(cmd);	/* P or V operation */
//#else
//	sops[0].sem_num = semno;		/* semaphore no */
//	sops[0].sem_op =cmd;	/* P or V operation */
//#endif
//
//
//	sops[0].sem_flg = SEM_UNDO|IPC_NOWAIT;	/* semaphore flag */
//
//	status = semop(semid,sops,nsops) ;
//	if(status == (-1))
//	{
//	    DB_PRINTF("",("semop : %s\n",strerror(errno)));
//	    if(errno == EAGAIN) return E_BUSY;
//	    else	        return E_ERROR;
//	}
//    }
//    else
//    {
//	status = E_ERROR;
//    }
//    return(status) ;
//}
//
//static status_t	semtest(int32_t	semno,
//			int32_t	*counts)
//{
//    status_t	status;
//    int32_t	semid;
//    union semun {
//	int val;
//	struct semid_ds *buf;
//	ushort *array;
//    } arg;
//
//    status = E_NO_ERROR;
//    if ((semid = semget(PQM_SEM_KEY,PQM_SEM_NUM,
//			IPC_ALLOC|00666)) != OS_ERROR)
//    {
//	do {
//	    status=semctl(semid,semno,GETVAL,&arg);
//	} while((status == (-1)) && (errno == EINTR));
//	if (status == (-1))
//	{
//	    status=E_ERROR;
//	}
//	else
//	{
//	    *counts = status;
//	    status = E_NO_ERROR;
//	}
//    }
//    else
//    {
//	status = E_ERROR;
//    }
//    return(status);
//}

// Patni - HAR / 2009Mar18 / Added / PSP Defect Sheet : Defect#212
//-Patni-RAJ/2010Jan18/Added/PH#3#/IR -86/Debug Window
//+Patni-DKH/2010Feb18/Added/Phase3#MCM0229-00187
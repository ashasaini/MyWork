//Redmine-781_CPP_TestTool
//Redmine-778
//Redmine-780
//Redmine-781

// AutoMapShimStudyManager.cpp: implementation of the CAutoInsertProtocolStudyManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AutoInsertProtocolStudyManager.h"
#include <libStudyManager/Log/ScopedFuncInOutLog.h>
#include <PQM/PQMLogMgr.h>
#include "pqmstudymgr.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//**************************************************************************
//Method Name   : CAutoInsertProtocolStudyManager
//Author        : PATNI/DKH
//Purpose       : default constructor
//**************************************************************************
CAutoInsertProtocolStudyManager::CAutoInsertProtocolStudyManager(
    CPQMStudyMgr* f_studymgr
): m_study_manager(f_studymgr)
{
}
//**************************************************************************
//Method Name   : ~CAutoInsertProtocolStudyManager
//Author        : PATNI/DKH
//Purpose       : This is the Destructor
//**************************************************************************
CAutoInsertProtocolStudyManager::~CAutoInsertProtocolStudyManager()
{
}
//**************************************************************************
//Method Name   : MakeAutoSeries
//Author        : PATNI/DKH
//Purpose       :
//**************************************************************************
void CAutoInsertProtocolStudyManager::MakeAutoSeries(
    SequenceList* f_sequence_list,
    const int f_target_position
)
{
    LPCTSTR FUNC_NAME = _T("CAutoInsertProtocolStudyManager::MakeAutoSeries");
    PQM_SCOPED_TRACE();

    if (NULL == m_study_manager) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("m_study_manager pointer is NULL"));
        return;
    }

    if (NULL == f_sequence_list) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_map_sequence_list points to NULL"));
        return;
    }

    LPCTSTR l_loid = _T("");
    m_study_manager->MakeSeries(f_sequence_list, l_loid, MAP_AUTO_MODE, f_target_position);
    CString seqorder(_T(""));
    seqorder.Format(_T("%d"), m_study_manager->GetSeriesCount());
    BSTR l_bstrseqorder = seqorder.AllocSysString();
    ::SysFreeString(l_bstrseqorder);
}

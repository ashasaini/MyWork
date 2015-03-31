//Redmine-781_CPP_TestTool
//Redmine-781
//Redmine-782

// PathDB.cpp: implementation of the CPathDB class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PathDB.h"
#include <ErrorDisp/ErrorDispConsts.h>
#include <PQM/PQMLogMgr.h>
#include <PQM/PqmUtilities.h>
//Patni-Rajendra/2011Apr1/Added/TMSC-REDMINE-773
#include <libStudyManager/Mutex/ScopedLock.h>
#include "IPathData.h"
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//Patni-Rajendra/2011Mar29/Added/TMSC-REDMINE-1559
using namespace MR::ACQ::SM::COMMON::MUTEX;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//Patni-Rajendra/2011Apr1/Modified/TMSC-REDMINE-773
MR::ACQ::SM::COMMON::MUTEX::CThreadMutex CPathDB::m_critical_sec;
IPathData* CPathDB::m_data = NULL;

CPathDB::CPathDB()
{

}

CPathDB::~CPathDB()
{

}

//*********************************************************************
//Method Name   : Search
//Author        : Patni/AMT
//Purpose       : Searches path data into path db and creates a linklist
//                  if not found returns false
//*********************************************************************
bool CPathDB::Search(IPathData* f_data)
{
    LPCTSTR FUNC_NAME = _T("CPathDB::Search");

    if (NULL == f_data) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("f_data is NULL, Can not add path data to linklist"));
        return false;
    }

    // This method is accessing static member(m_data) of a class so it required
    //  thread safe execution.
    // enter critical section for making thread safe
    //Patni-Rajendra/2011Apr1/Modified/TMSC-REDMINE-1559
    CScopedLock l_scoped_lock(&m_critical_sec);

    m_data = f_data;

    char*			l_li_fname = NULL;
    char*			l_ld_fname = NULL;
    GpdbGetFilePath(PATH_DB_NAME, &l_li_fname, &l_ld_fname);

    if (l_li_fname == NULL || l_ld_fname == NULL) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Can't get file name for PATH_DB_NAME DB file"));
        //Patni-AMT/2009Nov16/Modified/DeFT#MVC004996
        FreeResources(l_li_fname, l_ld_fname, _T("IDS_CS_ERR_NO_PATH_DB"));
        // exit critical section for making thread safe
        return false;
    }

    gpdb_handle_p	l_pdb		= GPDBOpen(l_li_fname, l_ld_fname, O_RDONLY);

    if (l_pdb == NULL) {

        CString l_err_msg = _T("");
        l_err_msg.Format(_T("Can't open PATH_DB_NAME DB files %s or %s"),
                         l_li_fname, l_ld_fname);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_err_msg);
        //Patni-AMT/2009Nov16/Modified/DeFT#MVC004996
        FreeResources(l_li_fname, l_ld_fname, _T("IDS_CS_ERR_ILLEGAL_PATH_DB"));
        // exit critical section for making thread safe
        return false;
    }

    PathDBSearch(l_pdb, CPathDB::GPDBCallback,
                 PATH_DB_KEY_INSTALLED, GPDB_ON, NULL);

    GPDBClose(l_pdb);
    m_data->DllFindHead();
    m_data = NULL;

    FreeResources(l_li_fname, l_ld_fname);
    // exit critical section for making thread safe

    return true;
}

void CPathDB::GPDBCallback(void* f_key, path_db_rec_p f_db_rec_p)
{
    if (m_data != NULL) {
        m_data->AddData(f_key, f_db_rec_p);
    }
}

//+Patni-AMT/2009Nov16/Added/DeFT#MVC004996
//*****************************************************************************
//Method Name   : FreeResources
//Author        : Patni/AMT
//Purpose       : Free memory allocated to file name and if msg id is
//                  specified then display error message
//*****************************************************************************
void CPathDB::FreeResources(
    char* f_li_fname,
    char* f_ld_fname,
    CString f_msg
)
{

    FREE_PTR(f_li_fname);

    FREE_PTR(f_ld_fname);

    if (!f_msg.IsEmpty()) {
        //Illegal coil database connected message dispaly
        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
            CPqmUtilities::GetMultiLingualString(f_msg),
            MPlus::EH::Crucial, APP_NAME, APP_NAME);
    }
}

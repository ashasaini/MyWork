//Redmine-783
//Redmine-774
// SectionDB.cpp: implementation of the CSectionDB class.
//
//////////////////////////////////////////////////////////////////////
//Redmine-781
#include "stdafx.h"
#include "SectionDB.h"
#include <ErrorDisp/ErrorDispConsts.h>
#include <tami/libgpdb/section.h>
#include <PQM/PQMLogMgr.h>
#include <PQM/PqmUtilities.h> // Added By KT/ACE-2/15-06-2009/Multilingualisation
#include <PQM/CoilDefinitions.h>
//Patni-Rajendra/2011Apr1/Added/TMSC-REDMINE-773
#include <libStudyManager/Mutex/ScopedLock.h>
#include "ISectionData.h"
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//Patni-Rajendra/2011Apr1/Added/TMSC-REDMINE-1559
using namespace MR::ACQ::SM::COMMON::MUTEX;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
ISectionData* CSectionDB::m_data = NULL;
//Patni-Rajendra/2011Apr1/Modified/TMSC-REDMINE-1559
MR::ACQ::SM::COMMON::MUTEX::CThreadMutex CSectionDB::m_critical_sec;

CSectionDB::CSectionDB()
{

}

CSectionDB::~CSectionDB()
{
    // No delete of data is required as we are not assigning memory
}

bool CSectionDB::Search(
    ISectionData* f_data,
    ...
)
{
    LPCTSTR FUNC_NAME = _T("CSectionDB::Search");

    // This method is accessing static member(m_data) of a class so it required
    //  thread safe execution.
    // enter critical section for making thread safe
    //Patni-Rajendra/2011Apr1/Modified/TMSC-REDMINE-1559
    CScopedLock l_scoped_lock(&m_critical_sec);

    //+Patni-AMT/2009Nov16/Added/DeFT#MVC004996
    if (NULL == f_data) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("f_data is NULL, Can not add coil Section to linklist"));
        return false;
    }

    m_data = f_data;

    char*			l_li_fname = NULL;
    char*			l_ld_fname = NULL;
    GpdbGetFilePath(SECTION_DB_NAME, &l_li_fname, &l_ld_fname);

    if (l_li_fname == NULL || l_ld_fname == NULL) {

        DB_PRINTF("C", ("Set GPDB_PATH for SECTION \n"));

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Can't get file name for SECTION_DB_NAME DB file"));
        //Patni-AMT/2009Nov16/Modified/DeFT#MVC004996
        FreeResources(l_li_fname, l_ld_fname, _T("IDS_CS_ERR_NO_SECTION_DB"));
        return false;
    }

    // Open section database
    gpdb_handle_p	l_sdb		= NULL;

    if (!(l_sdb = SeDBOpen(l_li_fname, l_ld_fname, O_RDONLY))) {

        CString l_err_msg = _T("");
        l_err_msg.Format(_T("Can't opne SECTION_DB_NAME DB files %s or %s"),
                         l_li_fname, l_ld_fname)

        DB_PRINTF("", ("Cna't Open %s or %s.\n", l_li_fname, l_ld_fname));
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_err_msg);
        //Patni-AMT/2009Nov16/Modified/DeFT#MVC004996
        FreeResources(l_li_fname, l_ld_fname, _T("IDS_CS_ERR_ILLEGAL_SECTION_DB"));
        return false;
    }

    FreeResources(l_li_fname, l_ld_fname);
    //-Patni-AMT/2009Nov16/Added/DeFT#MVC004996

    // get a list of section information from the section DB
    va_list args = NULL;
    va_start(args, f_data);

    // Get first argument to check variable argument is passed or not.
    int l_first = va_arg(args, int);
    va_end(args);

    if (l_first < 0) {
        // No search criteria specified by calling function hence need to call
        // GetAll function
        SeDBGetAll(l_sdb, CSectionDB::GPDBCallback);

    } else {
        // Search criteria is specified by calling function hence need to call
        // Search with search criteria.

        va_start(args, f_data);
        SeDBSearch(l_sdb, CSectionDB::GPDBCallback, va_pass(args));
        va_end(args);
    }

    SeDBClose(l_sdb);

    m_data->DllFindHead();

    m_data = NULL;

    return true;
}

//+Patni-AMT/2009Nov16/Added/DeFT#MVC004996
//*****************************************************************************
//Method Name   : FreeResources
//Author        : Patni/AMT
//Purpose       : Free memory allocated to file name and if msg id is
//                  specified then display error message
//*****************************************************************************
void CSectionDB::FreeResources(
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
//-Patni-AMT/2009Nov16/Added/DeFT#MVC004996

void CSectionDB::GPDBCallback(
    void* f_key,
    sedb_rec_p f_db_rec_p
)
{
    if (NULL != m_data) {
        m_data->AddData(f_key, f_db_rec_p);
    }
}

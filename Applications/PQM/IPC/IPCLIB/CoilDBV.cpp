//Redmine-783
// CoilDB1.cpp: implementation of the CCoilDBV class.
//
//////////////////////////////////////////////////////////////////////
//Redmine-781
#include "stdafx.h"
#include "CoilDBV.h"

//Added by Hemant On 3/4/2009 7:02:54 PM
//+Patni-Hemant/2009Mar03/Added/ACE-1-Venus Coil
#include <ErrorDisp/ErrorDispConsts.h>
#include <PQM/PQMLogMgr.h>
//-Patni-Hemant/2009Mar03/Added/ACE-1-Venus Coil
#include <PQM/CoilDefinitions.h>
#include <PQM/PqmUtilities.h> // Added By KT/ACE-2/15-06-2009/Multilingualisation
//Patni-Rajendra/2011Apr1/Added/TMSC-REDMINE-773
#include <libStudyManager/Mutex/ScopedLock.h>
#include "ICoilData.h"
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
ICoilData* CCoilDBV::m_data = NULL;
//Patni-Rajendra/2011Apr1/Modified/TMSC-REDMINE-1559
MR::ACQ::SM::COMMON::MUTEX::CThreadMutex CCoilDBV::m_critical_sec;

CCoilDBV::CCoilDBV()
{

}

CCoilDBV::~CCoilDBV()
{

}
//*****************************************************************************
//Method Name   : CCoilDBV::Search
//Author        : Patni/AMIT
//Purpose       : Search coil data in coil db and create a linklist for
//                  found records. If can not ipen file, return false;
//*****************************************************************************
bool CCoilDBV::Search(ICoilData* f_data, ...)
{
    LPCTSTR FUNC_NAME = _T("CCoilDBV::Search");

    if (NULL == f_data) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("f_data is NULL, Can not add coil to linklist"));
        return false;
    }

    // This method is accessing static member(m_data) of a class so it required
    //  thread safe execution.
    // enter critical section for making thread safe
    //Patni-Rajendra/2011Apr1/Modified/TMSC-REDMINE-1559
    CScopedLock l_scoped_lock(&m_critical_sec);

    m_data = f_data;

    // Get coil database name.
    char*			l_li_fname = NULL;
    char*			l_ld_fname = NULL;

    GpdbGetFilePath(CDB_DB_NAME, &l_li_fname, &l_ld_fname);

    //+Patni-AMT/2009Nov16/Modified/DeFT#MVC004996
    if (l_li_fname == NULL || l_ld_fname == NULL) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Can't get file name for CDB_DB_NAME DB file"));
        //Patni-AMT/2009Nov16/Modified/DeFT#MVC004996
        FreeResources(l_li_fname, l_ld_fname, _T("IDS_CS_ERR_NO_COIL_DB"));

        return false;
    }

    // open the databases
    gpdb_handle_p	l_cdb		= CdbOpen(l_li_fname, l_ld_fname, O_RDONLY);

    if (NULL == l_cdb) {

        CString l_err_msg = _T("");
        DB_PRINTF("", ("Cna't Open %s or %s.\n", l_li_fname, l_ld_fname));

        l_err_msg.Format(_T("Can't opne CDB_DB_NAME DB files %s or %s"),
                         l_li_fname, l_ld_fname);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_err_msg);
        //Patni-AMT/2009Nov16/Modified/DeFT#MVC004996
        FreeResources(l_li_fname, l_ld_fname, _T("IDS_CS_ERR_ILLEGAL_COIL_DB"));

        return false;
    }

    FreeResources(l_li_fname, l_ld_fname);
    //-Patni-AMT/2009Nov16/Modified/DeFT#MVC004996

    // get a list of installed-coils from the Coil DB
    va_list args = 0;
    va_start(args, f_data);

    // Get first argument to check variable argument is passed or not.
    int l_first = va_arg(args, int);
    va_end(args);

    if (l_first < 0) {
        // No search criteria specified by calling function hence need to call
        // GetAll function
        CdbGetAll(l_cdb, CCoilDBV::GPDBCallback);

    } else {
        // Search criteria is specified by calling function hence need to call
        // Search with search criteria.

        va_start(args, f_data);
        CdbSearch(l_cdb, CCoilDBV::GPDBCallback, va_pass(args));
        va_end(args);
    }

    CdbClose(l_cdb);


    m_data = NULL;

    // exit critical section for making thread safe

    return true;
}

//+Patni-AMT/2009Nov16/Added/DeFT#MVC004996
//*****************************************************************************
//Method Name   : FreeResources
//Author        : Patni/AMT
//Purpose       : Free memory allocated to file name and if msg id is
//                  specified then display error message
//*****************************************************************************
void CCoilDBV::FreeResources(
    char* f_li_fname,
    char* f_ld_fname,
    CString  f_msg
)
{

    FREE_PTR(f_li_fname);

    FREE_PTR(f_ld_fname);

    if (!f_msg.IsEmpty()) {

        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
            CPqmUtilities::GetMultiLingualString(f_msg),
            MPlus::EH::Crucial, APP_NAME, APP_NAME);
    }
}
//-Patni-AMT/2009Nov16/Added/DeFT#MVC004996

void CCoilDBV::GPDBCallback(
    void* f_key,
    cdb_rec_p f_db_rec_p
)
{
    if (NULL != m_data) {
        m_data->AddData(f_key, f_db_rec_p);
    }
}

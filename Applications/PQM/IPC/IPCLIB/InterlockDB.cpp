// InterlockDB.cpp: implementation of the CInterlockDB class.
//
//////////////////////////////////////////////////////////////////////
//Redmine-781
#include "stdafx.h"
#include "InterlockDB.h"
#include <ErrorDisp/ErrorDispConsts.h>
#include <tami/libWNP/WinNativePorting.h>
#include <tami/libproMsg/proMsg.h>
#include <tami/libgpdb/Interlock.h>
#include <PQM/Resource.h>
#include <PQM/PQMLogMgr.h>
#include <PQM/PqmUtilities.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IInterLockData* CInterlockDB::m_data = NULL;

//CCriticalSection CInterlockDB::m_critical_sec;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInterlockDB::CInterlockDB(
)
{

}

CInterlockDB::~CInterlockDB(
)
{

}

bool CInterlockDB::Search(
    IInterLockData* f_data
)
{
    LPCTSTR FUNC_NAME = _T("CInterlockDB::Search");
    char*		dbmFile = NULL, *mcfFile = NULL;

    GpdbGetFilePath("interlockDB", &dbmFile, &mcfFile);

    if (dbmFile == NULL || mcfFile == NULL || f_data == NULL) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("f_data is NULL, Can not add InterLock data to linklist"));
        return false;
    }

    m_data = f_data;
    m_data->m_type = 0;

    // get a list of installed-coils from the Coil DB

    idb_handle_p	idbHandle = IdbOpen(dbmFile, mcfFile, O_RDONLY);

    m_data->m_type = IDB_INTERLOCK;

    IdbSearch(idbHandle, CInterlockDB::GPDBCallback, IDB_KEY_INSTALLED, IDB_ON,
              IDB_KEY_TYPE, IDB_INTERLOCK,
              NULL);


    m_data->m_type = IDB_WEIGHT_RATIO;

    IdbSearch(idbHandle, CInterlockDB::GPDBCallback, IDB_KEY_INSTALLED, IDB_ON,
              IDB_KEY_TYPE, IDB_WEIGHT_RATIO,
              NULL);

    IdbClose(idbHandle);

    FREE_PTR(dbmFile);

    FREE_PTR(mcfFile);

    //if(idbCoilCount == 0 || idbWeightCount == 0)
    //	PqmUiErrorDialog(PqmUiFetchLiteral("ERR_NO_INTERLOCK_DB"),ERR_ERROR);

    return true;
}

void CInterlockDB::GPDBCallback(
    void* f_key,
    idb_rec_p f_db_rec_p
)
{
    if (NULL != m_data) {
        m_data->AddData(f_key, f_db_rec_p);

    }
}
//Redmine-783
//Redmine-780
//Redmine-782
// InterlockData.cpp: implementation of the CInterlockData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InterlockData.h"
#include <tami/libWNP/WinNativePorting.h>
#include <tami/libproMsg/proMsg.h>
#include <tami/libgpdb/Interlock.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInterlockData::CInterlockData():
    idbCoilList(NULL),
    idbWeightList(NULL),
    idbCoilCount(0),
    idbWeightCount(0),
    SzIlockCoilList(0),
    SzIlockWeightList(0)
{
}

CInterlockData::~CInterlockData(
)
{
    //+Patni-HAR/2009Mar22/Memory Leak
    for (int i = 0; i <= idbCoilCount; i++) {
        free(idbCoilList[i].value);
        free(&idbCoilList[i]) ;
    }

    idbCoilList = NULL;

    //+Patni-HAR/2009Mar22/Memory Leak
    for (i = 0; i <= idbWeightCount; i++) {
        free(idbWeightList[i].value);
        free(&idbWeightList[i]);
    }

    idbWeightList = NULL;
}


int CInterlockData::GetCoilCount(
) const
{
    return idbCoilCount;
}

int CInterlockData::GetWeightCount(
) const
{
    return idbWeightCount;
}

InterlockDB_t* CInterlockData::GetCoilList(
) const
{
    return idbCoilList;
}

InterlockDB_t* CInterlockData::GetWeightList(
) const
{
    return idbWeightList;
}


void CInterlockData::AddData(
    void* f_key,
    idb_rec_p f_db_rec_p
)
{
    //DB_FUNC_ENTER("[pqmInterlockAddCoilList]:");
    if (m_type == IDB_INTERLOCK) {

        if (SzIlockCoilList == idbCoilCount) {

            idbCoilList = (InterlockDB_t*)realloc(idbCoilList,
                                                  sizeof(InterlockDB_t) * (ALLOC_UNIT + SzIlockCoilList));

            if (idbCoilList == NULL) {

                //DB_PRINTF("",("%s:%d malloc error\n",__FILE__,__LINE__));
                //PqmUiSafeExit(PQM_FATAL);
                return;
            }

            SzIlockCoilList += ALLOC_UNIT;
        }

        idbCoilList[idbCoilCount].key = (int)f_key;
        memcpy(idbCoilList[idbCoilCount].value,
               f_db_rec_p->value,
               sizeof(idbCoilList[idbCoilCount].value));

        idbCoilCount++;

    } else if (m_type == IDB_WEIGHT_RATIO) {

        //DB_FUNC_ENTER("[pqmInterlockAddWeightList]:");

        if (SzIlockWeightList == idbWeightCount) {

            idbWeightList = (InterlockDB_t*)realloc(idbWeightList,
                                                    sizeof(InterlockDB_t) * (ALLOC_UNIT + SzIlockWeightList));

            if (idbWeightList == NULL) {

                //DB_PRINTF("",("%s:%d malloc error\n",__FILE__,__LINE__));
                //PqmUiSafeExit(PQM_FATAL);
                return;
            }

            SzIlockWeightList += ALLOC_UNIT;
        }

        idbWeightList[idbWeightCount].key = (int)f_key;

        memcpy(idbWeightList[idbWeightCount].value, f_db_rec_p->value,
               sizeof(idbWeightList[idbWeightCount].value));

        //DB_PRINTF("i",("WeightRatio [%d] : anatomy = %d\n",
        //	idbWeightCount,(int)key));

        idbWeightCount++;

    } else {
        AfxMessageBox(_T("Improper Database"), MB_SYSTEMMODAL);
    }

}

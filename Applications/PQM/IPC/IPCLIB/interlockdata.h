//Redmine-783
//Redmine-780
// InterlockData.h: interface for the CInterlockData class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_INTERLOCKDATA_H__
#define __SM_IPCLIB_INTERLOCKDATA_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tami/libgpdb/interlock.h>
#include "IInterLockData.h"

#define IDB_BUF_SIZE	(sizeof(((idb_rec_p)NULL)->value)/sizeof(long))
#define	ALLOC_UNIT	10

typedef	struct	_pqmDataDB_t {
    int			key;
    unsigned long	value[IDB_BUF_SIZE];
} InterlockDB_t;

/*********************************************************************
 * Static function prototypes
 *********************************************************************/
//static	void pqmInterlockAddCoilList(void *, idb_rec_p);
//static	void pqmInterlockAddWeightList(void *, idb_rec_p);

/*********************************************************************
 * Static variables
 *********************************************************************/


class CInterlockData : public IInterLockData
{
public:

    CInterlockData();
    virtual ~CInterlockData();

    int GetCoilCount() const;
    int GetWeightCount() const;

    InterlockDB_t* GetCoilList() const;
    InterlockDB_t* GetWeightList() const;

private:

    InterlockDB_t*	idbCoilList;
    InterlockDB_t*	idbWeightList;
    int				idbCoilCount;
    int				idbWeightCount;
    int				SzIlockCoilList;
    int				SzIlockWeightList;

    virtual void AddData(void* f_key, idb_rec_p f_db_rec_p);

};
#endif // #ifndef __SM_IPCLIB_INTERLOCKDATA_H__ 

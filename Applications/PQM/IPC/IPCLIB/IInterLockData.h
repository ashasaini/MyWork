// IInterLockData.h: interface for the IInterLockData class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_IINTERLOCKDATA_H__
#define __SM_IPCLIB_IINTERLOCKDATA_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tami/libgpdb/interlock.h>
#include "IData.h"

class IInterLockData : public IData
{
public:
    IInterLockData(
    );

    virtual ~IInterLockData(
    );

    virtual void AddData(
        void* f_key,
        idb_rec_p f_db_rec_p
    ) = 0;

    int m_type;

};

#endif // #ifndef __SM_IPCLIB_IINTERLOCKDATA_H__ 

// ICoilData.h: interface for the ICoilData class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_ICOILDATA_H__
#define __SM_IPCLIB_ICOILDATA_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tami/libgpdb/coil.h>
#include <libStudyManager/NonCopyable.h>
#include "IData.h"

class ICoilData : public IData
    , private MR::ACQ::SM::COMMON::NonCopyable<ICoilData>
{
public:
    ICoilData();

    virtual ~ICoilData();

    virtual void AddData(void* f_key, cdb_rec_p f_db_rec_p) = 0;
};

#endif // #ifndef __SM_IPCLIB_ICOILDATA_H__ 

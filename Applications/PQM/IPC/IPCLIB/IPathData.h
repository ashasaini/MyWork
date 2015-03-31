// IPathData.h: interface for the IPathData class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_IPATHDATA_H__
#define __SM_IPCLIB_IPATHDATA_H__
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tami/libgpdb/path.h>
#include <libStudyManager/NonCopyable.h>
#include "IData.h"

class IPathData : public IData
    , private MR::ACQ::SM::COMMON::NonCopyable<IPathData>
{
public:
    IPathData(
    );

    virtual ~IPathData(
    );

    virtual void AddData(
        void* f_key,
        path_db_rec_p f_db_rec_p
    ) = 0;
};

#endif // #ifndef __SM_IPCLIB_IPATHDATA_H__ 
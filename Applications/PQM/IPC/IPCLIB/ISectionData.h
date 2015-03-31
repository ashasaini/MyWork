// ISectionData.h: interface for the ISectionData class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_ISECTIONDATA_H__
#define __SM_IPCLIB_ISECTIONDATA_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tami/libgpdb/section.h>
#include <libStudyManager/NonCopyable.h>
#include "IData.h"

class ISectionData : public IData
    , private MR::ACQ::SM::COMMON::NonCopyable<ISectionData>
{
public:
    ISectionData();
    virtual ~ISectionData();

    virtual void AddData(
        void* f_key,
        sedb_rec_p f_sedb_rec
    ) = 0;
};
#endif // #ifndef __SM_IPCLIB_ISECTIONDATA_H__ 

// PqmSectionData.h: interface for the CPqmSectionData class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_PQMSECTIONDATA_H__
#define __SM_IPCLIB_PQMSECTIONDATA_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ISectionData.h"

class CPqmSectionData : public ISectionData
{
public:

    CPqmSectionData();

    virtual ~CPqmSectionData();

    void AddData(void* f_key, sedb_rec_p f_db_rec_p);
};
#endif // #ifndef __SM_IPCLIB_PQMSECTIONDATA_H__ 

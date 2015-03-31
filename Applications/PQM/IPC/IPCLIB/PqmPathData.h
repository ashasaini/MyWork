//Redmine-783
//Redmine-780
// PqmPathData.h: interface for the CPqmPathData class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_PQMPATHDATA_H__
#define __SM_IPCLIB_PQMPATHDATA_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IPathData.h"

class CPqmPathData : public IPathData
{
public:

    //Constructor / Destrcutor
    CPqmPathData();
    virtual ~CPqmPathData();

    // + 578 AMIT PATEL Shimming WB Onsite
    bool GetPathDataFromKey(const int f_key, path_db_rec_p f_rec);
    // - 578 AMIT PATEL Shimming WB Onsite

private:
    void AddData(void* f_key, path_db_rec_p f_db_rec_p);

};
#endif // #ifndef __SM_IPCLIB_PQMPATHDATA_H__ 

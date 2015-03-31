// InterlockDB.h: interface for the CInterlockDB class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_INTERLOCKDB_H__
#define __SM_IPCLIB_INTERLOCKDB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>
#include "IInterlockData.h"

//*****************************************************************************
// To pass variable argument list to library.
/*template<byte count>
struct SVaPassNext{
    SVaPassNext<count - 1> big;
    DWORD dw;
};
template<> struct SVaPassNext<0>{};
class CVaPassNext{
public:
    SVaPassNext<50> svapassnext;
    CVaPassNext(va_list & args){
		try{
			memcpy(&svapassnext, args, sizeof(svapassnext));
		} catch (...) {}
    }
};
#define va_pass(valist) CVaPassNext(valist).svapassnext*/
//*****************************************************************************

class CInterlockDB : private MR::ACQ::SM::COMMON::NonCopyable<CInterlockDB>
{

public:
    CInterlockDB();
    virtual ~CInterlockDB();

    bool Search(IInterLockData* f_data);

    static void GPDBCallback(
        void* f_key,
        idb_rec_p f_db_rec_p
    );

private:
    static IInterLockData* m_data;
};

#endif // #ifndef __SM_IPCLIB_INTERLOCKDB_H__ 

//Redmine-774
// PQMProConAdapter.h: interface for the CPQMProConAdapter class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_PQMPROCON_ADAPTER_H__
#define __SM_IPCLIB_PQMPROCON_ADAPTER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PQMAdapter.h"

class CPQMProConAdapter  : public CPQMAdapter
{
public:

    //Constrcutor / Destrcutor
    CPQMProConAdapter();
    virtual ~CPQMProConAdapter();

private:

    //+Patni-NP/2010Apr26/Added/MaFT MSA251-204
    virtual int GetHeaderSize() ;
    virtual int GetBodySize(PLBYTE buffer) ;
    virtual PLBYTE GetBodyPtr(int sz) ;
    //-Patni-NP/2010Apr26/Added/MaFT MSA251-204
};

#endif // #ifndef __SM_IPCLIB_PQMPROCON_ADAPTER_H__

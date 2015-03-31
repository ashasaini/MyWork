//Redmine-783
//Redmine-780
// IData.h: interface for the IData class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_IDATA_H__
#define __SM_IPCLIB_IDATA_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tami/libdll/dll.h>

class IData
{

public:

    IData();

    virtual ~IData();

    void DllFindHead();

    DllT* GetHead() const;
    void  SetHead(DllT* f_head);

    int GetCount() const;

protected:

    DllT* m_head;
};

#endif // #ifndef __SM_IPCLIB_IDATA_H__ 

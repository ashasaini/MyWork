//Redmine-781_CPP_TestTool
//Redmine-777
//Redmine-780
// AutoMapOP.h: interface for the CAutoInsertMapOperation class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_AUTOINSERT_MAPOPERATION_H__
#define __SM_IPCLIB_AUTOINSERT_MAPOPERATION_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AutoInsertProtocolOperation.h"

class CAutoInsertMapOperation: public CAutoInsertProtocolOperation
{
    mutable int m_error_code;
public:
    //Constructor/Destructor
    explicit CAutoInsertMapOperation(CPqm* const f_pqm_ptr, CAutoInsertProtocolStudy* const f_ptr_auto_insert_study);
    virtual ~CAutoInsertMapOperation();

    void SetErrorCode(const int f_error_code);
    int GetErrorCode() const;

private:

    //Serach for reuse MAP
    AutoInsert_status CheckForReuse(const int f_index)const;
    AutoInsert_status SearchMapDataInDoneProtocol(const int f_protocol, int& f_error_code)const;
};

#endif // #ifndef __SM_IPCLIB_AUTOINSERT_MAPOPERATION_H__

//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-774
//Redmine-777
//Redmine-780
// AutoShimOperation.h: interface for the CAutoInsertShimOperation class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_AUTOSHIMOPERATION_H__
#define __SM_IPCLIB_AUTOSHIMOPERATION_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AutoInsertProtocolOperation.h"



class CAutoInsertShimOperation :
    public CAutoInsertProtocolOperation
{
public:

    //Constructor/Destructor
    explicit CAutoInsertShimOperation(CPqm* f_pqm_ptr, CAutoInsertProtocolStudy* const f_ptr_auto_insert_study);

    virtual ~CAutoInsertShimOperation();

    //Update water/fat selection for CFA graph in varfield DB and internal memory
    void UpdateWaterFatSection(const int f_shimState, const int f_protocol)const;

    //Search for reuse shimming
    AutoInsert_status CheckForReuse(const int f_protocol)const;
    //Get Shim FSY flag vlaue
    int GetShimFSYFlagValue() const;
    //Return the path of the FSY flag
    CString GetAutoShimFSYPath() const;

private:

    //Search reusable shim data in Done status protocols.
    bool SearchShimDataInDoneProtocol(const int f_protocol)const;


};

#endif // #ifndef __SM_IPCLIB_AUTOSHIMOPERATION_H__ 

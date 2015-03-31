//Redmine-783
//Redmine-780
// CheckFOVInMap.h: interface for the CCheckFOVInMap class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_CHECKFOVINMAP_H_
#define __SM_IPCLIB_CHECKFOVINMAP_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>
#include "pqmstudy.h"

//Patni/2011Mar31/Added/V2.0/IR-181_NFD1_Prod_Req_Part1
typedef struct msPosSlabInfo_s msPosSlabInfo_t;

// pre-declaration
class CVarFieldHandle; //TMSC-Tanoue/2010Aug25/MVC008345

class CCheckFOVInMap : private MR::ACQ::SM::COMMON::NonCopyable<CCheckFOVInMap>
{

public:
    //Constrcutor / Destrcutor
    CCheckFOVInMap();
    virtual ~CCheckFOVInMap();

    status_t PqmCheckFOVinMap(CVarFieldHandle* const vf_handle, const int32_t  spProtocol,
                              const int32_t  mapProtocol, const int32_t mode);

private:

    status_t calcOffsetValue(CVarFieldHandle* const vf_handle, const int32_t protocol, flt32_t*	offset);
    status_t cmpMsPosSlabInfo(msPosSlabInfo_t* sppos, msPosSlabInfo_t* mappos, const int32_t  checkMode);
    void FreeMem(void* ptr1, void* ptr2);


};

#endif // #ifndef __SM_IPCLIB_CHECKFOVINMAP_H__ 

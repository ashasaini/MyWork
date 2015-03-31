//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-780
// PqmLocalShimming.h: interface for the CPqmLocalShimming class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_PQMLOCALSHIMMING_H__
#define __SM_IPCLIB_PQMLOCALSHIMMING_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

extern "C" {
#include <tami/libmsintf/msintf.h>
}
#include <libStudyManager/NonCopyable.h>
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"
#include "PqmShimStruct.h"
#include "ShimmingRequest.h"

class CPqm;

class CPqmLocalShimming : private MR::ACQ::SM::COMMON::NonCopyable<CPqmLocalShimming>
{
public:
    //Constructor / Destructor
    CPqmLocalShimming();
    virtual ~CPqmLocalShimming();

    //Patni-HAR-Ravindra/2010Dec03/Added/V2.00#IR-168
    int CalcShimandHRShimArea(shimGlobal_t* f_glbalp);
    int ShimQuit(const int)const;
    int ShimLocalRun(shimGlobal_t*);

    SetPqmToLocalShim(CPqm*);

private:

    CPqm* m_pqm;
    CShimmingRequest m_shimming_request;
    //Patni-HAR/2009Nov05/Modifed/MSA0248-00139-MRS-Crash Return type corrected

    int  Shimf0Write()const;
    int  MrsCrossProduct(double*, double*, double*)const;
    int  ShimMsShimRun();
    int  ShimCheckOffset3(const int f_typeformatdepth)const;
    int  HrCheckArea() const;
    int  HrShimCalcPosi() const;
    int  HrShimSetWholeThickness() const;
    int  HrShimSetRO()const;
    int  HrCheckVolume() const;
    int  MrsCheckHrShimArea()const;
    int  MrsSetRO()const;
    int  MrsCalcShimArea()const;
    void MrsCalcPosi()const;
    void MrsNormalize(double*)const;
    void MrsSetWholeThickness()const;
    void ShimmPopErrorDialog(const int type, const int f_error_id, const CString& f_errmsg)const;

    status_t ShimMsEndCB(msInteractiveShim_t*)const;

};

#endif // #ifndef __SM_IPCLIB_PQMLOCALSHIMMING_H__ 

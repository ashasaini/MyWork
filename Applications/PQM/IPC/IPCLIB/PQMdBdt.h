//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-780
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMAdapter.h
 *  Overview: Structure of CPQMAdapter class.
 *  Last Modified: 2008/5/30 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2007/6/15 12:00:00     PQM5 Baselined code
 *   2.0          Patni            2007/8/10 12:00:00     Implemented PQM6 requirements & Formatted
 *   3.0          Patni/LK         2008/5/30 12:00:00     Restructured after
                                                            TA review
 *****************************************************************************/
#ifndef __SM_IPCLIB_PQMDBDT_H__
#define __SM_IPCLIB_PQMDBDT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tami/libgt/gtStruct.h>

extern "C" {
#include <tami/seqgen/seqgen.h>
#include <tami/libdbdt/dBdt.h>
#include <tami/seqgen/sgDesigner.h>
}
#include <libStudyManager/NonCopyable.h>

class CPqmProtocol;
class IObjectManager;
class ILicenseInfo;
class CVarFieldHandle; //TMSC-Tanoue/2010Aug25/MVC008345

class CPQMdBdt : private MR::ACQ::SM::COMMON::NonCopyable<CPQMdBdt>
{
public:
    //Constructor / Destructor
    CPQMdBdt(const int f_mode, ILicenseInfo* f_licenseinfo, IObjectManager* f_objectmanager);
    virtual ~CPQMdBdt();

    status_t IECCheck(CVarFieldHandle* const  f_vf_handle ,  CPqmProtocol*  obj, const bool f_is_prescan_done = false);
    //Patni-HAR-SS/2010Feb09/Modified/MSA0251-00125
    status_t dBdtCalc(CVarFieldHandle* const f_vf_handle,  CPqmProtocol*  f_obj);

private:

    ILicenseInfo* m_license_info;
    IObjectManager* m_obj_manager;

    //+ Patni-MP/2009Aug29/Added/MVC004423
    void	PqmIECScanCancel();
    //+ Patni-PJS/2009Jun13/Modify/JFT# 252/IR(5)
    //void	ErrorInSaturn(int	dBdtcode, char *dBdtdata, int	ope_mode, char *sardata);
    //Patni-HAR/2010Feb08/Modified/MSA0251-00125

    bool	ErrorInSaturn(int	f_dbdtcode, char* f_dbdtdata, int	f_ope_mode, char* f_sardata, CPqmProtocol* f_protocol_object);
    status_t IECCheckSAR(CVarFieldHandle* const f_vf_handle, CPqmProtocol* obj, char* sarMsg)const;
    int Check(CVarFieldHandle* const f_vf_handle,  CPqmProtocol*  obj, char* buf)const;
    int CheckMode2RO(CVarFieldHandle* const f_vf_handle, const int f_prot , const int f_mode)const;
    int CheckMode2(CVarFieldHandle* const f_vf_handle,  CPqmProtocol*  f_obj)const;
    int CheckMode1FOV(CVarFieldHandle* const f_vf_handle, const int f_prot , flt32_t* f_lim , const int f_mode)const;
    int CheckMode1Plan(CVarFieldHandle* const f_vf_handle, const int  f_prot)const;

    int VecCheck(f32vec3_t* vec1, f32vec3_t* vec2)const;
    int CheckMode1(CVarFieldHandle* const f_vf_handle,  CPqmProtocol*  obj, char* buf)const;
    int CheckHighMode(CVarFieldHandle* const f_vf_handle,  CPqmProtocol*  obj)const;
    int HighMode(CVarFieldHandle* const f_vf_handle,  CPqmProtocol*  f_obj)const;
    int Check2ndMode(CVarFieldHandle* const f_vf_handle,  CPqmProtocol*  f_obj)const;
    //Patni-HAR/2010Feb08/Modified/MSA0251-00125
    bool Error(int dBdtcode, char* dBdtdata, int	sarcode, char* sardata, CPqmProtocol* f_protocol_object);

    int CalcdBdt(CVarFieldHandle* const f_vf_handle, CPqmProtocol* f_protocol, flt32_t* f_val, int* f_mode)const;
    status_t PqmCalcdBdt(flt32_t* f_slew, flt32_t* f_val)const;
    void InsetCrossProduct(double  p[], double  q[], double  crossProduct[])const;
    void InsetNormalize(double  v[])const;

    //+ Patni-MP/2009Aug29/Added/MVC004423
    CString GetErrorStringFromInt(const int f_literal)const;
    int GetIECEdition(long*	f_edition)const;
};
#endif // #ifndef __SM_IPCLIB_PQMDBDT_H__ 

/////////////////////////////////////////////////////////////////////////////

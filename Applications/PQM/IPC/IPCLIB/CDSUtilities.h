//Redmine-780
// CDSUtilities.h: interface for the CCDSUtilities class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_CDSUTILITIES_H__
#define __SM_IPCLIB_CDSUTILITIES_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tami/libvf/varfields.h>
#include <libStudyManager/NonCopyable.h>

// pre-declaretion
class CVarFieldHandle; //TMSC-Tanoue/2010Aug25/MVC008345

class CCDSUtilities : private MR::ACQ::SM::COMMON::NonCopyable<CCDSUtilities>
{
public:
    static bool GetCdsCoordinate(CVarFieldHandle* const f_vf_handle, const int f_protocol,
                                 Vft_CdsCoilLocation_t* f_coordinatetbl);
    static bool GetCdsLocation(CVarFieldHandle* const f_vf_handle, const int f_protocol,
                               Vft_CdsCoilLocation_t* f_locationtbl) ;
    static bool GetCdsStatus(CVarFieldHandle* const f_vf_handle, const int f_protocol,
                             int* f_cds_status) ;
    static bool PutCdsCoordinate(CVarFieldHandle* const f_vf_handle, const int f_protocol,
                                 Vft_CdsCoilLocation_t* f_coordinatetbl);
    static bool PutCdsLocation(CVarFieldHandle* const f_vf_handle, const int f_protocol,
                               Vft_CdsCoilLocation_t* f_locationtbl) ;
    static bool PutCdsStatus(CVarFieldHandle* const f_vf_handle, const int f_protocol,
                             int* f_cds_status) ;
};

#endif // #ifndef __SM_IPCLIB_CDSUTILITIES_H__ 

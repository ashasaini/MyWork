// pqmAMBUtilities.h: interface for the CpqmAMBUtilities class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_PQMAMBUTILITIES_H__
#define __SM_IPCLIB_PQMAMBUTILITIES_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tami/vfStudy/vf_apptypes.h>
#include <tami/libvf/varfields.h>
#include <tami/pqm/pqmAmbMsg.h>
#include <libStudyManager/NonCopyable.h>
#include "pqmAMBStructs.h"

class CPqmProtocol;
class CPqm;
class CVarFieldHandle;

class CpqmAMBUtilities
{
public:
    CpqmAMBUtilities();
    virtual ~CpqmAMBUtilities();
    static status_t pqmAmbParamPutStudy(CVarFieldHandle* const	f_vf_handle, amb_t*	const f_amb_plan, Entrytbl_t* const f_prot_tbl);
    static status_t pqmAmbParamGetStudy(CVarFieldHandle* const	f_vf_handle, amb_t*	const f_amb_plan, const int prot);
    static status_t	pqmAmbMatrixCheck(CVarFieldHandle* const handle, const int32_t protocol_index);
    static void pqmAmbCheckGDC_LocatorImage(CVarFieldHandle* const f_vf_handle, Entrytbl_t* const f_prot_tbl);
    static void pqmAmbAutoScanSet(CVarFieldHandle* const f_vf_handle, const int f_prot, CPqmProtocol* const f_protocol , int f_hold_scan);
    static void pqmAmbStartError(const int f_code);
    static status_t pqmAmbGetProtCheckData(CVarFieldHandle* const f_vfd, const int f_prot, VfFieldSpec_t* const fvfReq, PqmAmbProtCheckDataT* const f_tbl);
    static status_t pqmAmbSetOffset(CVarFieldHandle* const f_vfd , CPqmProtocol* f_protocol, const flt32_t	f_couchPos);
    static status_t pqmAmbSetAcqmanTbl(CVarFieldHandle* const f_vfd , const int	f_prot, int* const f_prot_tbl, int* const f_couch_tbl);
    static status_t pqmAmbPreScanCopy(CVarFieldHandle* const f_vfd , const int f_from_prot, const  int f_to_prot);
    static void pqmAmbPrepSet(CVarFieldHandle* const f_vfd, const int f_prot, int f_data);
    static status_t pqmAmbCheckMovingCouch(CPqmProtocol*  const f_obj , CPqmPrMode* const f_mode,
                                           float* const f_newposition, float* const f_moveval, int* const f_msg);
    static void pqmAmbSetScanOff(CPqmProtocol* const f_master_obj, CPqmProtocol* const f_obj , const float f_offset);
    static status_t pqmAmbConvProtToPos(int* const f_prot_list, const int f_n, int* f_pos_list);
    static status_t pqmAmbModeSet(CVarFieldHandle*	const f_vf_handle, const int	f_prot, int f_mode);
    static status_t pqmAmbScanIDSetScan(int f_numSegment, CVarFieldHandle* const f_vfd, int	f_prot, int f_type);
    static status_t pqmAmbGetOtherProtocols(void* f_data, const int f_pos, int f_count, char* const	f_filename);
    static status_t  pqmAmbAfterOtherProtocols(VfDBHandle_t f_db_handle, int f_protocol, void* f_data);

    static void SetPQM(CPqm* f_pqm) ;
    static int GetGDCValue(CVarFieldHandle* const f_vf_handle, const int f_prot_index);
    static int GetAutoMipPreview(CVarFieldHandle* const f_vf_handle, const int f_prot_index);
    static void DeleteTagsFromAMBSlaveProt(CVarFieldHandle* const f_vf_handle, const int f_protocol);
    static void SetValForSlaveProt(CPqmProtocol* f_protocol);

    static status_t pqmAmbSetChannelModeForMapShim(CVarFieldHandle* f_vfd, int f_master_prot, int f_map_shim_prot);

private:
    static CPqm* m_pqm;

};

#endif // #ifndef __SM_IPCLIB_PQMAMBUTILITIES_H__

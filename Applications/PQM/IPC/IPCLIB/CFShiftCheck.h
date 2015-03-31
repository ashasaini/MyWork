// CFShiftCheck.h: interface for the CCFShiftCheck class.
//
//////////////////////////////////////////////////////////////////////
#ifndef __SM_IPCLIB_CFSHIFTCHECK_H_
#define __SM_IPCLIB_CFSHIFTCHECK_H_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tami/acqman/AM_ClientMessages.h>
#include "acqsetflag.h"

class CPqmProtocol;
class CVarFieldHandle;
class CPQMStudy;
class CScanProcedure;

typedef struct PQM_CFShiftTemp_s {
    float m_currtemp[MAX_TEMP_DATA];
    float m_basetemp[MAX_TEMP_DATA];

    PQM_CFShiftTemp_s() ;

} PQM_CFShiftTemp_t;

typedef struct PQM_CFShiftData_s {
    float m_temp_coeff[MAX_TEMP_DATA];
    int m_gif[MAX_TEMP_DATA];
    float m_threshold_val;
    float m_threshold_cfa_val;
    PQM_CFShiftData_s() ;

} PQM_CFShiftData_t;



class CCFShiftCheck
{
public:
    explicit CCFShiftCheck(CPQMStudy* f_pqm);
    virtual ~CCFShiftCheck();

    int CheckCFShiftValue(CPSAcqSetFlag::DATA f_data, CPqmProtocol* f_curr_protocol, CScanProcedure* f_scan_proc) const;
    BOOL GetIsCFARetryExecuted() const ;
    void SetIsCFARetryExecuted(const BOOL f_retrycfa);

    void SetcfShiftTemp(AM_AcqManToClient_t* const f_data);

private:
    BOOL m_resetretryexecuted;
    PQM_CFShiftTemp_t m_cfshifttemp;
    CPQMStudy* m_pqm_studyptr;

    BOOL ReadcfShiftInfoFile(const TCHAR* f_file_path, PQM_CFShiftData_t* f_cf_shiftdata, int& f_temp_count) const;
    int CopyBaseTemperature(CVarFieldHandle* const f_vf_handle, const int f_from_prot, const int f_to_prot) const ;
    int CalculateTemperature(const int f_base_protocol_index, float& f_threshold_val, float& f_temperature) const ;
    BOOL IsRetryCFARequired(const float f_threshold_val, const float f_temperature) const;
    void ReadcfShiftData(PQM_CFShiftData_t* f_cf_shiftdata, CString f_data, int& f_count, bool f_threshold_read = false) const;

};

#endif // #ifndef __SM_IPCLIB_CFSHIFTCHECK_H_
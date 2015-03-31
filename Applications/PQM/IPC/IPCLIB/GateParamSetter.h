// GateParamSetter.h: interface for the CGateParamSetter class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_GATEPARAMSETTER_H__
#define __SM_IPCLIB_GATEPARAMSETTER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tami/acqman/AM_ClientMessages.h>
#include <tami/acqman/AM_ClientMessages_unicode.h>

#include "LibPLTreeWrapper.h"

class CPqm;
class CVarFieldHandle;
class CGateIntervalData;

#define MAX_GATE_VALS 3

class CGateParamSetter
{
public:
    CGateParamSetter();
    virtual ~CGateParamSetter();

    void UpdateWirelessGateParam(AM_ClientToAcqMan_unicode_t& f_data) const;

    void UpdateCurrentandUsableGateValforScan(AM_AcqManToClient_t* f_data);
    status_t WriteValidatedGateParametertoStudy(CVarFieldHandle* const f_vf_handle, const int f_protocol, const int f_gate_method);

    void GateModeScanCallBack(AM_AcqManToClient_t* f_data);
    void GateModeWSECallBack(AM_AcqManToClient_t* f_data);

    bool HasDefaultUsableValue(const int f_method) const;
    void UpdateUsableValue(const int f_method, const bool f_update_default_val_only);

    void ResetGateInterval();
    void SetPQM(CPqm* const f_pqm);

    Wireless_gating_t* const GetWirelessGateParam();
    void SetWirelessGateParam(Wireless_gating_t* const f_wirelessgateinfo);

    void GetUsableGateVal(const int f_method, int32_t* gate_val) const;

private:


    CPqm* m_pqm_ptr;
    Wireless_gating_t m_wireless_param;
    char* m_auto_interval_scratch_pad;

    CGateIntervalData* m_gate_data;

    status_t CreateAutoIntervalScratchPad(const int f_protocol)const;
    void DeleteAutoIntervalScratchPad(char* const f_file_name)const;
    status_t WriteParametersToScratchPad(const int f_gate_method, const int f_protocol, char* m_auto_interval_scratch_pad) const;

    status_t WriteWirelessGateParamtoStudy(CVarFieldHandle* const f_vf_handle, const int f_protocol) const;

    void UpdateGateInterval(VftGatingParam_s& f_gating_param, const int f_gate_method) const;

    void SetCurrentGateval(const AM_AcqManGateInfo_t& f_gate_data);

    bool HasEcgBit(const int f_method) const;
    bool HasPeripheralBit(const int f_method) const;
    bool HasRespiratoryBit(const int f_method) const;
};



#endif // #ifndef __SM_IPCLIB_GATEPARAMSETTER_H__

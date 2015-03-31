//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-777
//Redmine-780
// AutoShimManager.h: interface for the CAutoInsertShimManager class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_AUTOSHIM_MANAGER_H__
#define __SM_IPCLIB_AUTOSHIM_MANAGER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AutoInsertProtocolManager.h"

class CPqm;
class CAutoShim;
class CScanTranscation;
class CAutoShimOperation;
class CAutoMapShimStudy;

class CAutoInsertShimManager :
    public CAutoInsertProtocolManager
{
public:
    //Constructor / Destructor
    explicit CAutoInsertShimManager(CPqm* f_pqm, const bool f_licence_available = true);
    virtual ~CAutoInsertShimManager();

    //Update Water/Fat selection on Varfield and internal memory
    void UpdateWaterFatSection(const int f_shimState, const int f_protocol)const;

    // Is Auto shimming required
    virtual AutoInsert_status AutoInsertProtocol(CPqmProtocol* const f_protocol, const AutoInsertData& f_autoinsert_data)const;
    virtual AutoInsert_status CheckOnceInsertionPerScanStartCase(CPqmProtocol* f_target_prot) const;
    virtual CPqmProtocol* GetPrevAutoInsertProtocolInfo(CPqmProtocol* const f_target_prot, int& f_prev_counter) const;

private:

    AutoInsert_status AutoShimming(CPqmProtocol* const f_protocol, const AutoInsertData& f_autoinsert_data)const;

    bool IsAutoInsertProtocolRequired(CPqmProtocol* const f_protocol)const;

};

#endif // #ifndef __SM_IPCLIB_AUTOSHIM_MANAGER_H__ 

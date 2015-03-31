// AutoInsertProcedure.h: interface for the AutoInsertProcedure class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_AUTOINSERTPROCEDURE_H__
#define __SM_IPCLIB_AUTOINSERTPROCEDURE_H__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>
#include "AutoInsertConstant.h"

class CAutoInsertProtocolManager;
class CPqm;
class CPqmProtocol;


class CAutoInsertProcedure :
    private MR::ACQ::SM::COMMON::NonCopyable<CAutoInsertProcedure>
{

public:
    CAutoInsertProcedure();
    virtual ~CAutoInsertProcedure();

    void InitAutoInsertProc(CPqm* f_pqm, const bool f_auto_insert_lic = true);
    int AutoMapAutoShimming(CPqmProtocol* const f_protocol,	const AutoInsertData& f_autoinsert_data);
    BOOL GetAutoInsertedSeqDataToDisplay(VARIANT** f_autinsert_data,
                                         int* f_insertedmappos,
                                         int* f_target_pos,
                                         int* const f_is_shim_required,
                                         int* const f_is_map_required) const;

    void UpdateWaterFatSection(const int f_shimState, const int f_protocol)const;
    void ProtocolEditingCompleted(CPqmProtocol* const f_protocol) const;

    bool CanDisplayCFAGraph(CPqmProtocol* const f_protocol) const;
    void GetTargetProtInformation(int* f_target_pos) const;

private:

    CPqm* m_pqm;
    CAutoInsertProtocolManager* m_ptr_automap_mgr;
    CAutoInsertProtocolManager* m_ptr_autoshim_mgr;
    CPqmProtocol* m_target_prot;
};

#endif // #ifndef __SM_IPCLIB_AUTOINSERTPROCEDURE_H__ 

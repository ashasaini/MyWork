//Redmine-783
//Redmine-777
//Redmine-780
// AutoMapMgr.h: interface for the CAutoInsertMapManager class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_AUTOINSERT_MAPMANAGER_H__
#define __SM_IPCLIB_AUTOINSERT_MAPMANAGER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AutoInsertProtocolManager.h"

class CAutoInsertMapManager: public CAutoInsertProtocolManager
{
public:

    //Constructor / Destructor
    explicit CAutoInsertMapManager(CPqm* const f_pqm, const bool f_licence_available = true);
    virtual ~CAutoInsertMapManager();
    virtual AutoInsert_status AutoInsertProtocol(CPqmProtocol* const f_protocol, const AutoInsertData& f_autoinsert_data)const;
    virtual AutoInsert_status CheckOnceInsertionPerScanStartCase(CPqmProtocol* f_target_prot) const;
    virtual CPqmProtocol* GetPrevAutoInsertProtocolInfo(CPqmProtocol* const f_target_prot, int& f_prev_counter) const;

protected :
    virtual bool IsCoilChangedForTargetProt(CPqmProtocol* const f_target_protocol)const;

private:
    AutoInsert_status AutoMap(CPqmProtocol* const f_protocol, const AutoInsertData& f_autoinsert_data)const;
    AutoInsert_status DisplayErrorMessageForMap(CPqmProtocol* f_protocol)const;
    bool IsAutoInsertProtocolRequired(CPqmProtocol* const f_auto_protocol)const;
};



#endif // #ifndef __SM_IPCLIB_AUTOINSERT_MAPMANAGER_H__

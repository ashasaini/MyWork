//Redmine-1213
//Redmine-783
//Redmine-780
//Redmine-782
// AutoManager.h: interface for the CAutoInsertProtocolManager class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_AUTOINSERT_PROTOCOLMANAGER_H__
#define __SM_IPCLIB_AUTOINSERT_PROTOCOLMANAGER_H__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <PASCOMLib/SequenceParameter.h>
#include "AutoInsertProtocol.h"
#include "AutoInsertConstant.h"


class CPqm;
class CAutoInsertProtocolStudy;
class CAutoInsertProtocolOperation ;



class CAutoInsertProtocolManager :
    private MR::ACQ::SM::COMMON::NonCopyable<CAutoInsertProtocolManager>
{
public:

    //Destructor
    virtual ~CAutoInsertProtocolManager();

    virtual AutoInsert_status AutoInsertProtocol(CPqmProtocol* const f_target_prot, const AutoInsertData& f_autoinsert_data,
            const bool f_can_display_relimit_message) const;

    virtual void UpdateWaterFatSection(const int f_shimState, const int f_target_prot)const ;
    bool StartAutoInsertProtocol(const int f_target_protocol_number, const scan_mode f_mode)const;
    BOOL GetAutoInsertDataForGUI(SequenceList* const l_autinsert_data, int* const f_insertedmappos, int* const f_target_pos, int* const f_is_shim_required, int* const f_is_map_required);

    bool GetHasDisplayedRelimitMessage() const;

protected:
    //CPqm class pointer
    CPqm* m_pqm_ptr;

    mutable bool m_can_display_relimit_message;
    mutable bool m_has_displayed_relimit_message;


    //CAutoOperation class pointer
    CAutoInsertProtocolOperation* m_ptr_auto_operation;

    //CAtutoInsertStudy class pointer
    CAutoInsertProtocolStudy* m_ptr_auto_insert_study;

    //CAutoFunctions class object
    mutable CAutoInsertProtocol m_auto_func;

    //Constructor
    explicit CAutoInsertProtocolManager(CPqm* f_ptr_pqm, const bool f_licence_available = true);
    virtual bool IsAutoInsertProtocolRequired(CPqmProtocol* const f_auto_protocol)const = 0;
    virtual AutoInsert_status CheckOnceInsertionPerScanStartCase(CPqmProtocol* f_target_prot) const = 0;
    virtual AutoInsert_status AutoInsertProtocol(CPqmProtocol* const f_target_prot, const AutoInsertData& f_autoinsert_data) const = 0;
    virtual CPqmProtocol* GetPrevAutoInsertProtocolInfo(CPqmProtocol* const f_target_prot, int& f_prev_counter) const;

    AutoInsert_status CheckForReuse(const int f_target_prot)const;
    virtual AutoInsert_status InsertAutoInsertProt(CPqmProtocol* const f_target_prot,
            const AutoInsertData& f_autoinsert_data,
            const scan_mode f_mode)const;

    void AbortTargetScantoWait(CPqmProtocol* f_target_prot) const;

    virtual bool IsAutoInsertionRequired(CPqmProtocol* const f_target_protocol)const;
    bool IsTargetProtEdited(CPqmProtocol* const f_target_protocol)const;
    virtual bool IsCoilChangedForTargetProt(CPqmProtocol* const f_target_protocol)const;

    //Get & Set the value of Auto flag
    bool GetAutoFlag() const;
    CPqmProtocol* GetAutoProtocol() const;
    void SetAutoProt(CPqmProtocol* const f_auto_protocol);

private:
    //SequenceList class object
    SequenceList m_sequence_list;
};

#endif // #ifndef __SM_IPCLIB_AUTOINSERT_PROTOCOLMANAGER_H__

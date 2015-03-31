//Redmine-1213
//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-780
// AutoOperation.h: interface for the CAutoInsertProtocolOperation class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_AUTOINSERT_PROTOCOLOPERATION_H__
#define __SM_IPCLIB_AUTOINSERT_PROTOCOLOPERATION_H__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <PASCOMLib/SequenceParameter.h>
#include <libStudyManager/NonCopyable.h>
#include "AutoInsertProtocolStudyManager.h"
#include "AutoInsertConstant.h"


class CScanProcedure;
class CAutoInsertProtocolStudy;
class CPqmProtocol;
class CPqm;

class CAutoInsertProtocolOperation :
    private MR::ACQ::SM::COMMON::NonCopyable<CAutoInsertProtocolOperation>
{
public:

    virtual ~CAutoInsertProtocolOperation();
    virtual AutoInsert_status CheckForReuse(const int f_index) const = 0;

    void InsertAutProtocolOnUI(const scan_mode f_mode)const;
    void ResetCurrentScan(const int f_target_position);
    CPqmProtocol* AddAutoProtocol(int f_target_pos, SequenceList& f_sequence_list, const scan_mode f_mode, CString& f_error_id);
    virtual void UpdateWaterFatSection(const int f_State, const int f_index)const ;

protected:


    CAutoInsertProtocolStudyManager m_ptr_auto_insert_study_manager;
    CScanProcedure* const m_scan_procedure;
    CPqm* m_pqm_ptr;
    CAutoInsertProtocolStudy* m_ptr_auto_insert_study;

    //Constructor
    explicit CAutoInsertProtocolOperation(CPqm* f_ptr_pqm, CAutoInsertProtocolStudy* const f_ptr_auto_insert_study);
};

#endif // #ifndef __SM_IPCLIB_AUTOINSERT_PROTOCOLOPERATION_H__

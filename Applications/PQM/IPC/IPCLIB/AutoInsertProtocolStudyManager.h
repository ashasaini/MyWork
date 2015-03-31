//Redmine-777
//Redmine-780

// AutoMapShimStudyManager.h: interface for the CAutoInsertProtocolStudyManager class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_AUTOINSERT_PROTOCOLSTUDY_MANAGER_H__
#define __SM_IPCLIB_AUTOINSERT_PROTOCOLSTUDY_MANAGER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>
#include <PASCOMLib/SequenceParameter.h>


class CPQMStudyMgr ;
class CAutoInsertProtocolStudyManager :
    private MR::ACQ::SM::COMMON::NonCopyable<CAutoInsertProtocolStudyManager>
{
public:
    explicit CAutoInsertProtocolStudyManager(CPQMStudyMgr* f_studymgr);
    virtual ~CAutoInsertProtocolStudyManager();

    void MakeAutoSeries(SequenceList* f_sequence_list, const int f_target_position);

private:
    CPQMStudyMgr* m_study_manager;

};

#endif // #ifndef __SM_IPCLIB_AUTOINSERT_PROTOCOLSTUDY_MANAGER_H__

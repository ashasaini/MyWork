//TMSC-REDMINE-783
//TMSC-REDMINE-780
//Redmine-775
//Redmine-777
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: IPCComManager.h
 *  Overview: Structure of CIPCComManager class.
 *  Last Modified: 2007/8/10 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2007/6/15 12:00:00     PQM5 Baselined code
 *   2.0          Patni            2007/8/10 12:00:00     Implemented PQM6 requirements & Formatted
 *
 *****************************************************************************/
#ifndef __SM_IPC_COM_MANAGER_H__
#define __SM_IPC_COM_MANAGER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>


class CSequence;

typedef CList<CSequence*, CSequence*> CSeqList;

// CIPCComManager is responsible for all communication of sequences between PQM and IPC

class CIPCComManager : private MR::ACQ::SM::COMMON::NonCopyable<CIPCComManager>

{
public:

    CIPCComManager();
    virtual ~CIPCComManager();

    bool GetList(CSeqList* objlist, const long maxlimit);

    SequenceList& GetSendSeqList();
    SequenceList& GetRecSeqList();

    void SetSndSequence(CSequence* pseq);

private:

    SequenceList m_snd_seq_list;
    SequenceList m_rec_seq_list;

};

#endif // #ifndef __SM_IPC_COM_MANAGER_H__

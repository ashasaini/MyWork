//Redmine-783
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: acqsetflag.h
 *  Overview: Structure of CPSAcqSetFlag class.
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
#ifndef __SM_IPCLIB_ACQSETFLAG_H__
#define __SM_IPCLIB_ACQSETFLAG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>

class CPSAcqSetFlag : private MR::ACQ::SM::COMMON::NonCopyable<CPSAcqSetFlag>
{
public:
    typedef enum TAG {
        K_PR_PRESCAN_NONE = -1,
        K_PR_PRESCAN_FSY = 31, //Patni-HAR/Modified/order of K_PR_PRESCAN_FSY is changed in _pr_tag_e enum
        K_PR_PRESCAN_RFL,
        K_PR_PRESCAN_RGN,
        TAG_TOTAL = 3
    };

    typedef enum DATA {
        PQM_VFC_PRESCAN_NONE = -1,
        PQM_VFC_PRESCAN_CLINICAL,
        PQM_VFC_PRESCAN_USE_PREVIOUS,
        PQM_VFC_PRESCAN_USE_VARFIELD,
        //Patni-DKH/2010Apr12/Added/Phase3#MCM0229-00187
        PQM_VFC_PRESCAN_LOAD_VARFIELD = 4,
        PQM_DATA_TOTAL = 5//Patni-DKH/2010Feb18/Added/Phase3#MCM0229-00187
    };

    CPSAcqSetFlag();
    CPSAcqSetFlag(const TAG tag, const DATA data);
    ~CPSAcqSetFlag();

    TAG  GetTag() const;
    void SetTag(const TAG tag);

    DATA GetData() const;
    void SetData(const DATA data);

protected:

private:

    TAG     m_tag;
    DATA    m_data;
};

typedef std::vector<CPSAcqSetFlag*> CPSAcqSetFlagVec;

#endif // #ifndef __SM_IPCLIB_ACQSETFLAG_H__

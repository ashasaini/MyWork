//Redmine-783
//////////////////////////////////////////////////////////////////////
// AutoFunctions.h: interface for the CAutoInsertProtocol class.
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_AUTOINSERT_PROTOCOL_H__
#define __SM_IPCLIB_AUTOINSERT_PROTOCOL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>

class CPqmProtocol;

class CAutoInsertProtocol : private MR::ACQ::SM::COMMON::NonCopyable<CAutoInsertProtocol>
{
public:

    //Constructor / Destructor
    CAutoInsertProtocol(const bool f_auto_flag = true);
    virtual ~CAutoInsertProtocol();

    bool GetAutoFlag() const;

    CPqmProtocol* GetAutoProtocol() const;
    void SetAutoProtocol(CPqmProtocol* const f_auto_protocol) ;

private:

    bool m_auto_flag;
    CPqmProtocol* m_auto_protocol;

};


#endif // #ifndef __SM_IPCLIB_AUTOINSERT_PROTOCOL_H__

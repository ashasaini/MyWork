//Redmine-780
// IObjectManager.h: interface for the IObjectManager class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_IOBJECTMANAGER_H__
#define __SM_IPCLIB_IOBJECTMANAGER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>
#include <PQM/PQMCommanMessages.h>

class CPqmPmAcqman;
class CPQMStudy;

class IObjectManager : private MR::ACQ::SM::COMMON::NonCopyable<IObjectManager>
{
public:
    IObjectManager();
    virtual ~IObjectManager();
    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    /*
    virtual CPqmPmAcqman* GetAcqManager() = 0;
    virtual CPQMStudy*  GetStudy() = 0;
    */
    virtual CPqmPmAcqman* const GetAcqManager() const = 0;
    virtual CPQMStudy*  const GetStudy() const = 0;
    //- Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    virtual void WriteToUI(const pqm_msg_tag Msg) const = 0;
    virtual BOOL GetDBDtCancelFlag() const = 0;

};

#endif // #ifndef __SM_IPCLIB_IOBJECTMANAGER_H__ 

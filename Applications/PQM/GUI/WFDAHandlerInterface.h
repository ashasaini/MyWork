// WFDAHandlerInterface.h: interface for the CWFDAHandlerInterface class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_WFDAHANDLER_INTERFACE_H__
#define __SM_WFDAHANDLER_INTERFACE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>
#include "WFDAConstants.h"

class CWFDAHandlerInterface : private MR::ACQ::SM::COMMON::NonCopyable<CWFDAHandlerInterface>
{

public:
    CWFDAHandlerInterface();
    virtual ~CWFDAHandlerInterface();

    virtual bool ProcessReceivedMsg(const WFDAMessageData& f_data) = 0;

};

#endif // #ifndef __SM_WFDAHANDLER_INTERFACE_H__

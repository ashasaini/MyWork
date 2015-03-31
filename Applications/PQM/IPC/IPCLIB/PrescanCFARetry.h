//Redmine-782
// PrescanCFARetry.h: interface for the CPrescanCFARetry class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_PRESCAN_CFARETRY_H__
#define __SM_IPCLIB_PRESCAN_CFARETRY_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "PreScan.h"

class CPrescanCFARetry : public CPreScan
{
public:
    explicit CPrescanCFARetry(CScanProcedure* scan_proc);
    virtual ~CPrescanCFARetry();

};

#endif // #ifndef __SM_IPCLIB_PRESCAN_CFARETRY_H__

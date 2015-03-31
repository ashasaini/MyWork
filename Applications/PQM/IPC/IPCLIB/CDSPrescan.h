// CDSPrescan.h: interface for the CCDSPrescan class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_CDSPRESCAN_H__
#define __SM_IPCLIB_CDSPRESCAN_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "PreScan.h"

class CScanProcedure;

class CCDSPrescan : public CPreScan
{
public:
    explicit CCDSPrescan(CScanProcedure* f_scan_proc);
    virtual ~CCDSPrescan();

};

#endif // #ifndef __SM_IPCLIB_CDSPRESCAN_H__ 

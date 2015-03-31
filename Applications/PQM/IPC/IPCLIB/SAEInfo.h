// SAEInfo.h: interface for the CSAEInfo class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_SAEINFO_H__
#define __SM_IPCLIB_SAEINFO_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ScanTransaction.h"
class CScanProcedure;


class CSAEInfo  : public CScanTransaction
{
public:
    explicit CSAEInfo(CScanProcedure* f_scan_proc);
    virtual ~CSAEInfo();

    virtual bool PrepareRequest();
    void SetTotalSAE(const int32_t f_total_sae);

private:
    int32_t m_total_sae;

    virtual void ProcessResponse(AM_ClientMessageHeader_t* f_header, AM_AcqManToClient_t* f_body);


};

#endif // #ifndef __SM_IPCLIB_SAEINFO_H__ 

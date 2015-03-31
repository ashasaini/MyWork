// AutoLocatorPostProc1.h: interface for the CAutoLocatorPostProc class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_AUTOLOCATORPOSTPROC_H__
#define __SM_IPCLIB_AUTOLOCATORPOSTPROC_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>
#include <libStudyManager/Mutex/ThreadMutex.h>
#include <PQM/PQMCommonStructures.h>

class CPqmProtocol;
class CPqm;

//using namespace MR::ACQ::SM::COMMON;


class CAutoLocatorPostProc: private MR::ACQ::SM::COMMON::NonCopyable<CAutoLocatorPostProc>
{

    CPqm* m_pqm_ptr;
    unsigned int m_recon_timeout;
    BOOL m_auto_locator_running;

    mutable MR::ACQ::SM::COMMON::MUTEX::CThreadMutex m_cs;

    //functions
    BOOL IsValidHALAProtocolForScan(CPqmProtocol* f_prot) const;
    BOOL IsValidCALAProtocolForScan(CPqmProtocol* f_prot) const;
    BOOL IsValidSequenceToInvokeCALAAppln(CPqmProtocol* f_protocol) const;
    BOOL IsValidSequenceToInvokeHALAAppln(CPqmProtocol* f_protocol) const;
    bool CheckAutoLocatorConditions(CPqmProtocol* f_protocol) const;
    void WaitForReconstruction(CPqmProtocol* f_pqmprotocol) const;
    BOOL IsValidAutoLocatorProtocolForScan(CPqmProtocol* f_prot, const AUTOLOCATOR_TYPE f_autolocator_type) const;

public:
    CAutoLocatorPostProc(CPqm* f_pqm);
    virtual ~CAutoLocatorPostProc();

    bool HandleAutoLocatorInvocation(CPqmProtocol* f_protocol, const bool f_Autolocator_after_acquisition = true);

    BOOL IsAutoLocatorRunning() const;
    void SetAutoLocatorRunning(const BOOL f_autolocator_running);

    BOOL IsValidSequenceToInvokeAutoLocator(const int f_acq_order, const AUTOLOCATOR_TYPE f_autolocator_type) const;
    BOOL CheckALProtocolScanConditions(CPqmProtocol* f_protocol) const;

};

#endif // #ifndef __SM_IPCLIB_AUTOLOCATORPOSTPROC_H__

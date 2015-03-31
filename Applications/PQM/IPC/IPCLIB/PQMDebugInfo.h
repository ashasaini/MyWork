//Redmine-783
// PQMDebugInfo.h: interface for the CPQMDebugInfo class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_PQMDEBUGINFO_H__
#define __SM_IPCLIB_PQMDEBUGINFO_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tami/libvf/varfields.h>
#include <libStudyManager/NonCopyable.h>
#include "acqsetflag.h"
#include "pqmmessagestructures.h"

//pre-declaration
class CPqm;
class CPqmProtocol;
class CVarFieldHandle; //TMSC-Tanoue/2010Aug25/MVC008345

class CPQMDebugInfo : private MR::ACQ::SM::COMMON::NonCopyable<CPQMDebugInfo>
{
public:
    //Constructor / Destructor
    CPQMDebugInfo();
    virtual ~CPQMDebugInfo();

    static CPQMDebugInfo* GetInstance(CPqm* f_pqmptr);

    //+Patni-HAR/2010Apr09/Added/Phase3#MCM0229-00187
    void PrescanDebug(const AM_ClientToAcqMan_unicode_t& f_data);
    //-Patni-HAR/2010Apr09/Added/Phase3#MCM0229-00187
    void StartPQMDebug(const int f_idebugoption, const UINT f_buttonmask);
    bool PqmDebugStatusLogOpen();
    void PqmDebugAcqLogOpen();

    void PqmDebugAcqMonitor(const CString f_csDebugData, ...);
    void PqmDebugIrsMonitor(const CString f_csDebugData, ...);
    void PqmDebugProMonitor(const CString f_csDebugData, ...);

private:

    //Patni-DKH/2010May13/Modified/Phase3#Redmine-210
    static CPqm* m_pqm_ptr;
    static CPQMDebugInfo* m_pqm_debuginfo;

    int m_iCUR_DEBUG;
    CStdioFile m_statuslogfile;
    CStdioFile m_logFile;
    CStdioFile m_acqlogfile;

    void pqmDebugDisplayCount(void);
    void pqmDebugDisplayList(void);
    void pqmDebugDisplayCouch(void);
    void pqmDebugDisplayGlobal(void);
    void pqmDebugDisplayStudy(void);
    void pqmDebugDisplayProtData(void);
    void pqmDebugAcq(void);
    void pqmDebugSize(void);
    void pqmDebugReset(void);
    void pqmDebugPq(void);
    void pqmDebugSar(void);
    void pqmDebugSarInfo(CPqmProtocol* f_pqmprotocol);
    void pqmDebugSarMon(void);
    void pqmDebugShimming(void);
    void pqmDebugAddList(const CString f_formatstr, ...);
    void pqmDebugProtData(CPqmProtocol* f_pqmprotocol);
    //void PqmCdsDebug(); - Definition Commented
    void PqmPrDebug();
    void PqmCoilDebug();
    void PqmPqDebug();
    void PqmSarDebugRfl();
    //Patni-AMT/2010Jan11/Added/DeFT# IR-86 Debug Window
    int PqmShmShimDebug();
    int PqmShmDebug();
    void PqmDebugStatusLogClose();

    //Patni-DKH/2010May13/Modified/Phase3#Redmine-210
    static void pqmDebugSarAddList(char* f_formatstr, ...);

    int PqmPrGetShimmingOffset(CVarFieldHandle* const vf_handle,
                               VfPathElem_t*	path,
                               int32_t	  depth,
                               PqmShim_t*	shim);

    CString GetStringForPrescanKind(const CPSAcqSetFlag::TAG kind);
    CString GetStringForPrescanAction(const int action);
    const CString GetDateTimeStamp(const bool f_bhourminformat = false);

};

#endif // #ifndef __SM_IPCLIB_PQMDEBUGINFO_H__ 

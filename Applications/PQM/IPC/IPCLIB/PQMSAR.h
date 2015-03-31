//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-780
//Redmine-782
// PQMSAR.h: interface for the CPQMSAR class.
//
//////////////////////////////////////////////////////////////////////
#ifndef __SM_IPCLIB_PQMSAR_H__
#define __SM_IPCLIB_PQMSAR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <tami/common/machdefs.h>
#include <tami/common/basicdefs.h>
#include <tami/vfStudy/vf_appcodes.h>
#include <libStudyManager/NonCopyable.h>

#include "sarp.h"

#define	MIN_SAR		0.000001

// pre-declaretion
class CPqm;
class CPqmProtocol;
class CVarFieldHandle; //TMSC-Tanoue/2010Aug25/MVC008345

class CPQMSAR : private MR::ACQ::SM::COMMON::NonCopyable<CPQMSAR>
{

public:
    //Constrcutor / Destrcutor
    explicit CPQMSAR(CPqm* l_pqm);
    virtual ~CPQMSAR();


    void ResetRfl();
    void ResetSARInfo(CPqmProtocol* const f_protocol);
    void RecalculateSAR(CVarFieldHandle* const f_vf_handle, CPqmProtocol* const	f_protocol);
    status_t PutRflParams(CVarFieldHandle* const vf_handle, const int f_prot);

    static CString FindSarMode(const int32_t f_mode);

    void GetValueFromVF(CVarFieldHandle* const f_vf_handle, CPqmProtocol*	const f_protocol);
    void GetSARValue(CPqmProtocol* const);
    sarVfRflParams_t GetRflParams()const;

    void GetNewRfl(CVarFieldHandle* const f_vf_handle, const int f_prot);
    void SetNewRfl(CVarFieldHandle* const f_vf_handle, CPqmProtocol*  const f_protocol);

private:

    sarVfRflParams_t	rflParams;
    sarVfTGCParams_t	tgcParams;		/* V9.25 */
    sarStat_t			pqmSarStat;
    CPqm*               m_pqm;

    void CalculateSAR(CVarFieldHandle* const f_vf_handle, CPqmProtocol*	const f_protocol)const;
    void DownReCalcSAR(CVarFieldHandle* const f_vf_handle, CPqmProtocol* const	f_protocol);

    static void pqmSarSetNewCouch(CPqmProtocol*	f_protocol);
    static void pqmSarSetVfRfl(CVarFieldHandle* const f_vf_handle, CPqmProtocol*	f_protocol);

    sarStat_t GetSarStat(void) const;

    void SetProtocolSARStats(CPqmProtocol* const l_startobj, const sarStat_t l_sarStat);

};

#endif // #ifndef __SM_IPCLIB_PQMSAR_H__ 

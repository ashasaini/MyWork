//Redmine-783
//Redmine-780
// pqmCDSDebug.h: interface for the CpqmCDSDebug class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_PQMCDSDEBUG_H__
#define __SM_IPCLIB_PQMCDSDEBUG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>
#include <PQM/CDSCommonStructures.h>


class CPqmProtocol ;
class CPqm;

class CpqmCDSDebug : private MR::ACQ::SM::COMMON::NonCopyable<CpqmCDSDebug>
{
public:

    CpqmCDSDebug();
    virtual ~CpqmCDSDebug();

    void PqmUiCdsDebugWindowCB(CPqmProtocol* const f_protocol_obj) ;
    void pqmUiDebugSetCdsCB(COMPASS_Debugg_Param* const f_compass_debug_param) ;


    //Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_03
    void GetCDSDebugParam(COMPASS_Debugg_Param* const f_compass_debug_param) const;
    //Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_03
    CPqmProtocol* GetDebugProtocol() const;


    //Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_03
    int  GetCurrentCDSDebugProtocol() const;
    void SetCurrentCDSDebugProtocol(const int f_debug_protocol) ;

    //Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_03
    bool GetCDSDebugFlag() const ;
    void SetCDSDebugFlag(const bool f_cdsdebug_flag) ;

    void SetPQM(CPqm* f_pqm);

private:

    bool  m_cdsdebug_flag;
    int   m_currentCDSdebugprotocol ;
    CPqm* m_pqm;
    void pqmUiDebugCancelCdsCB(COMPASS_Debugg_Param* const f_compass_debug_param) ;

};

#endif // #ifndef __SM_IPCLIB_PQMCDSDEBUG_H__ 

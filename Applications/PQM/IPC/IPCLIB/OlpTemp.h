//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-780
// OlpTemp.h: interface for the COlpTemp class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_OLPTEMP_H__
#define __SM_IPCLIB_OLPTEMP_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <ErrorDisp/ErrorDispConsts.h>
#include "ScanTransaction.h"

class CPqmProtocol;
class CVarFieldHandle; //TMSC-Tanoue/2010Aug25/MVC008345

class COlpTemp : public CScanTransaction
{
public:
    //Constructor / Destructor
    explicit COlpTemp(CScanProcedure* f_scan_proc);  //Patni-KSS/2011Feb17/Modified/CPP Test tool Execution
    virtual ~COlpTemp();

    bool  PqmAcqGetOLPTemp(CPqmProtocol* f_prObj, const int f_mode) ;
    bool  pqmAcqSendOLPTemp(const int f_mode);

    static void DisplayMessage(const CString& f_errmsg,
                               const MPlus::EH::MPLUSERRORLEVEL_e l_error_level = MPlus::EH::Information) ; //Patni-KSS/2011Feb17/Modified/CPP Test tool Execution

private:


    void CFAReady() const;

    bool PrepareRequest();

    void ProcessResponse(AM_ClientMessageHeader_t* f_hdr, //Patni-KSS/2011Feb17/Modified/CPP Test tool Execution
                         AM_AcqManToClient_t* f_body) ;


    status_t pqmAcqOLPTempCallback(CVarFieldHandle* const vf_handle,
                                   const int f_type, AM_AcqManToClient_t* f_ptrData) ;

    status_t pqmAcqErrOLPTemp(const int f_type, const int f_error_id, //Patni-KSS/2011Feb17/Modified/CPP Test tool Execution
                              const CString& f_errmsg,
                              AM_AcqManToClient_t* f_response) const;

};
#endif // #ifndef __SM_IPCLIB_OLPTEMP_H__ 



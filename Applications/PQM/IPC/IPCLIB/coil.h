//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-780
//Redmine-782
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: coil.h
 *  Overview: Structure of CCoil class.
 *  Last Modified: 2007/8/10 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2007/6/15 12:00:00     PQM5 Baselined code
 *   2.0          Patni            2007/8/10 12:00:00     Implemented PQM6 requirements & Formatted
 *
 *****************************************************************************/
#ifndef __SM_IPCLIB_COIL_H__
#define __SM_IPCLIB_COIL_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include <tami/common/basicdefs.h>
#include <tami/libgpdb/coil.h>
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include <PQM/CoilDefinitions.h>

// This class is having the coil structure for holding coil Information.
// This structure will be used by the functions which is specified in CoilDB
class CCoil : public CObject
{

public:
    DECLARE_SERIAL(CCoil);

    // Constructor / Destructor
    CCoil();
    virtual ~CCoil();

    // Copy contructor
    explicit CCoil(const CCoil& obj);

    // Operator Overlaoding
    CCoil& operator = (const CCoil& opr);

    int IsSectionDB() const;

    long  GetOutsideLimit() const;
    long  GetInsideLimit() const;
    unsigned long GetCoilKey() const;
    long GetRFLLimit() const;
    int  GetTuneLimit() const;
    BOOL GetEnable() const;
    CString GetLabel() const;
    int  GetCoilId() const;
    BOOL GetRecieveWBEnable() const;
    long GetPacPattern() const;
    BOOL IsWBCoil() const;
    int  GetMaxChannel() const;
    int GetPort() const;

    PqmCoil_t* GetCoil() const;
    bool SetCoil(PqmCoil_t* f_coil) ;

private:

    // + V9.25 Coil Support@AMT
    // Coil information is stored into PqmCoil_t so data part is commented
    //  out and interface is used as it is.
    //    BOOL m_wbcoil;                  // WB Coil
    //    BOOL m_speederenable;           // Speeder enable
    //    BOOL m_recievewbenable;         // Receive WB enable
    //    BOOL m_enable;                  // Enable
    //
    //    long m_pacpattern;               // PAC Pattern
    //    long m_outsidelimit;             // Outside limit
    //    long m_insidelimit;              // Inside limit
    //    long m_tunelimit;                // Tune limit
    //    long m_rfl_limit;                // RFL limit
    //    int m_coiltype;                 // Coil type
    //
    //    CString m_label;                // Label name
    //    CString m_coil_description;     // Coil description
    //
    //
    //    CString m_port;
    //    CString m_channel;
    ////  UID Deleted
    //    int m_coilid;                   // Coil ID
    unsigned long m_coil_key;

    PqmCoil_t*    m_coil_v;
    // - V9.25 Coil Support@AMT
    void CCoilCopy(const CCoil& f_opr);   // Copy the coil object

    void SetCoilId(const unsigned long id);
    void SetPacPattern(const long pacpattern);
    void SetIsWBCoil(const BOOL wbcoil);
    void SetEnable(const BOOL enable);
    void SetLabel(CString& label);
    void SetRecieveWBEnable(const BOOL recievewbenable);
    void SetOutsideLimit(const long outsidelimit);
    void SetInsideLimit(const long insidelimit);
    void SetTuneLimit(const long tunelimit);
    void SetRFLLimit(const long rfl_limit);
    void SetCoilKey(const unsigned long coil_key);

    BOOL GetSpeederEnable() const;
    void SetSpeederEnable(const BOOL speederenable);

    cdb_coil_type_t  GetCoilType() const;
    void SetCoilType(const cdb_coil_type_t& coiltype);

    CString GetCoilDescription() const;
    void SetCoilDescription(CString& coil_desp);



    //void SetPort(int& f_port); - Definition Commented

    cdb_channels_t GetChannel()const;
    void SetChannel(const cdb_channels_t channel);

};


#endif // #ifndef __SM_IPCLIB_COIL_H__ 

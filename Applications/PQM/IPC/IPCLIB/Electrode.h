//Redmine-783
//Redmine-780
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: Electrode.h
 *  Overview: Structure of CElectrode class.
 *  Last Modified: 2008/8/11 15:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2008/8/11 15:00:00     Implemented SM6 requirements & Formatted
 *
 *****************************************************************************/


//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_ELECTRODE_H__
#define __SM_IPCLIB_ELECTRODE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ScanTransaction.h"

class CPqm;
class CPqmPmAcqman;
class CScanProcedure;

class CElectrode : public CScanTransaction
{
public:
    //Constructor / Destructor
    explicit CElectrode(CScanProcedure* f_scan_proc);
    virtual ~CElectrode();

    bool PrepareRequest();
    void SetElectrodeType(const int f_electrode_type);

private:

    CPqm* m_pqm_ptr;
    CPqmPmAcqman* m_acqman_ptr;
    int m_nelectrode_type;

    void ErrElectrode(const int f_type, const int f_error_id, const CString& f_errrmsg, AM_AcqManToClient_t* f_response, AM_ClientMessageHeader_t* f_header);		// Added By KT/ACE-2/15-06-2009/Multilingualisation
    void ElectrodeCallback(const int f_type, AM_AcqManToClient_t* f_data, AM_ClientMessageHeader_t* f_header);
    void ProcessResponse(AM_ClientMessageHeader_t* f_header, AM_AcqManToClient_t* f_data);


    CString GetString(const int f_id)const;
};

#endif // #ifndef __SM_IPCLIB_ELECTRODE_H__

//Redmine-783
//Redmine-780
//Redmine-782
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: scan.h
 *  Overview: Structure of CScan class.
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

#ifndef __SM_IPCLIB_SCAN_H__
#define __SM_IPCLIB_SCAN_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include "ScanTransaction.h"

class CScanProcedure;

class CScan : public CScanTransaction
{
public:
    explicit CScan(CScanProcedure* scan_proc);
    virtual ~CScan();
    status_t AcqProbingThresHold(const float f_max, const float f_min);

private:

    //+Patni-PJS/2011Mar2/Modified/TMSC_REDMINE-1480-Review Comments
    unsigned int  m_mrs_thread_id;
    HANDLE m_mrs_thread_handle;
    //-Patni-PJS/2011Mar2/Modified/TMSC_REDMINE-1480-Review Comments

    int  MaintainPrepStudy(const CString& f_file_name, CPqmProtocol* f_current_protocol)const;
    int  MaintainMotionCorrectStudy(const CString& f_file_name, CPqmProtocol* f_current_protocol)const;
    int  AcquisitionDone();
    void ProcessResponse(AM_ClientMessageHeader_t* hdr, AM_AcqManToClient_t* body);
    int  AcquisitionCallback(const int type, AM_AcqManToClient_t* data);
    //-Patni-MP/2009Dec17/Modified/MVC006301
    int  ErrAcquisition(const int type, const CString& error_str,
                        AM_AcqManToClient_t* data);

    virtual bool PrepareRequest();

};
#endif // #ifndef __SM_IPCLIB_SCAN_H__ 


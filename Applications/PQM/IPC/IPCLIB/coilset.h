//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-780
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: coilset.h
 *  Overview: Structure of CCoilSet class.
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
#ifndef __SM_IPCLIB_COILSET_H__
#define __SM_IPCLIB_COILSET_H__
#if _MSC_VER > 1000
#pragma once
#endif

#include <libStudyManager/NonCopyable.h>
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "Coil.h"

class CCoil;

class CCoilSet : private MR::ACQ::SM::COMMON::NonCopyable<CCoilSet>
{
public:
    // Constructor and destructor of CCoilSet class
    CCoilSet();
    virtual ~CCoilSet();

    CCoil* const GetTransmitCoil()const;
    void   SetTransmitCoil(CCoil* f_tx_coil);

    CCoil*  const GetReceiveCoil()const;
    void   SetReceiveCoil(CCoil* f_rcv_coil);

private:

    // V9.25 Coil Support@AMT
    CCoil* m_tx_coil;       // TX Coil data
    CCoil* m_rcv_coil;       // Rcv Coil data
};

#endif // #ifndef __SM_IPCLIB_COILSET_H__ 

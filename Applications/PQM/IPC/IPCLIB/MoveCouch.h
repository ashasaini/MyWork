//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-780
//Redmine-782
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: MoveCouch.h
 *  Overview: Structure of CMoveCouch class.
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

#ifndef __SM_IPCLIB_MOVECOUCH_H__
#define __SM_IPCLIB_MOVECOUCH_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ScanTransaction.h"

//class CPqm;
//class CPqmPmAcqman;
class CScanProcedure;

class CMoveCouch : public CScanTransaction
{
public:
    //Constructor / Destructor
    explicit CMoveCouch(CScanProcedure* f_scan_proc);
    virtual ~CMoveCouch();

    bool PrepareRequest();
    void SetFuntionAndPosition(const int	f_function, const float f_position);

private:

    int m_function;
    float m_position;

    void ProcessResponse(AM_ClientMessageHeader_t* f_header, AM_AcqManToClient_t* f_data);
};
#endif // #ifndef __SM_IPCLIB_MOVECOUCH_H__ 

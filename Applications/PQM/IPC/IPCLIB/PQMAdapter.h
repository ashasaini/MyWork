//Redmine-783
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMAdapter.h
 *  Overview: Structure of CPQMAdapter class.
 *  Last Modified: 2008/5/30 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2007/6/15 12:00:00     PQM5 Baselined code
 *   2.0          Patni            2007/8/10 12:00:00     Implemented PQM6 requirements & Formatted
 *   3.0          Patni/LK         2008/5/30 12:00:00     Restructured after
                                                            TA review
 *****************************************************************************/
#ifndef __SM_IPCLIB_PQMADAPTER_H__
#define __SM_IPCLIB_PQMADAPTER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>
//+Patni-NP/2010Apr26/Added/MaFT MSA251-204
#include <Pcommlib/DataAdapter.h>
//-Patni-NP/2010Apr26/Added/MaFT MSA251-204

class CPQMAdapter : public CDataAdapter
    , private MR::ACQ::SM::COMMON::NonCopyable<CPQMAdapter>
{
public:

    //Constructor / Destructor
    CPQMAdapter();
    virtual ~CPQMAdapter();

private:

    virtual int GetHeaderSize(); //Patni-NP/2010Apr26/Added/MaFT MSA251-204
    virtual int GetBodySize(PLBYTE buffer);//Patni-NP/2010Apr26/Added/MaFT MSA251-204
    virtual PLBYTE GetBodyPtr(int sz) ;//Patni-NP/2010Apr26/Added/MaFT MSA251-204

    //+Patni-PH/2010May04/Commented/MaFT MSA251-204
    // Read the header content from the socket
    //virtual bool ReadHeader(PLBYTE* buffer, int buffer_size, long* header_size, long* data_size);
    //-Patni-PH/2010May04/Commented/MaFT MSA251-204
};

#endif // #ifndef __SM_IPCLIB_PQMADAPTER_H__ 

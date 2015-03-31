/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: DBServerWrapper.h
 *  Overview: Defination of CDBServerWrapper class.
 *  Last Modified: 2010/01/29 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2010/01/29 12:00:00
 *
 *
 *****************************************************************************/


#ifndef __SM_IPCLIB_DBSERVERWRAPPER_H__
#define __SM_IPCLIB_DBSERVERWRAPPER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>

//+Patni-MP/2009Jan28/Added/Phase-3 MVC002888
class CDBServerWrapper : private MR::ACQ::SM::COMMON::NonCopyable<CDBServerWrapper>
{
public:
    CDBServerWrapper();
    virtual ~CDBServerWrapper();
    static bool CheckLocalDBSpace(long& f_diskspace);
};
//-Patni-MP/2009Jan28/Added/Phase-3 MVC002888
#endif // #ifndef __SM_IPCLIB_DBSERVERWRAPPER_H__ 

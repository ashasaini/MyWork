/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: DBServerWrapper.cpp
 *  Overview: Implementation of CDBServerWrapper class.
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

#include "stdafx.h"
#include "DBServerWrapper.h"
#include <DBServer/MpDbsBaseObjectInfo.h>
#include <DBServer/MpDbsDBServer_Defs.h>
#include <PQM/PQMLogMgr.h>
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
using namespace MPlus::DBServer;
//+Patni-MP/2009Jan28/Added/Phase-3 MVC002888
CDBServerWrapper::CDBServerWrapper()
{

}

CDBServerWrapper::~CDBServerWrapper()
{

}

//*******************************Method Header*********************************
//Method Name    :CheckLocalDBSpace()
//Author         :PATNI/MP
//Purpose        :To Check the space available on the local database.
//                This function returns true if sufficient space is there on
//                local database else it returns false and also populates the
//                function argument with available disk space if the function
//                fails.
//History        :New specification is received on 16-Feb-10. Checked for
//                free space with following condition
//                    l_DB_status.lFreeSpace != 0
//                    l_DB_status.lFreePixelSpace != 0
//*****************************************************************************
bool CDBServerWrapper::CheckLocalDBSpace(
    long& f_diskspace
)
{
    //+Patni-AMT/2010Feb16/Modified/Ph-3# MVC002888-Change in specification
    LPCTSTR FUNC_NAME = _T("CDBServerWrapper::CheckLocalDBSpace");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    bool                l_ret_val = false;
    MpDbsBaseObjectInfo l_obj_base_info;
    DBS_DB_STATUS_t     l_DB_status;

    f_diskspace = -1;

    if (MP_DBS_SUCCESS == l_obj_base_info.DBStatusCheck(l_DB_status)) {

        f_diskspace = l_DB_status.lFreeSpace;
        l_ret_val = (l_DB_status.lFreeSpace != 0 && l_DB_status.lFreePixelSpace != 0);

    } else {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("DBServer is not running"));
        l_ret_val = false;
    }

    return l_ret_val;
    //-Patni-AMT/2010Feb16/Modified/Ph-3# MVC002888-Change in specification
}
//-Patni-MP/2009Jan28/Added/Phase-3 MVC002888
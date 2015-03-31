// pqmCDSInfo.h: interface for the CpqmCDSInfo class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_PQMCDSINFO_H__
#define __SM_IPCLIB_PQMCDSINFO_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <PQM/CoilDefinitions.h>
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"

struct CpqmCDSInfo {

    int m_src_protocol;
    int m_cds_flag ;
    int m_cds_step ;
    bool m_cds_rescan ;
    PqmCoil_p m_coil_tbl[PQM_NUM_COIL_PORT] ;
    int32_t m_org_coordinatetbl [PQM_NUM_COIL_PORT] ;
    int32_t m_used_locationtbl[PQM_NUM_COIL_PORT] ;
    Vft_CdsCoilLocation_t m_locationtbl [PQM_NUM_COIL_PORT] ;
    Vft_CdsCoilLocation_t m_coordinatetbl[PQM_NUM_COIL_PORT] ;
    int m_cds_status ;
    /* 0000 0000 0000 0000
    				   |||
    				   ||+----AUTO
    				   |+-----Imaging Positioning
    				   +------Using the previous value  */

    //Member variables ...
    CpqmCDSInfo();
    virtual ~CpqmCDSInfo();
};
#endif // #ifndef __SM_IPCLIB_PQMCDSINFO_H__ 

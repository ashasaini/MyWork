//Redmine-782
// pqmCDSInfo.cpp: implementation of the CpqmCDSInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pqmCDSInfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CpqmCDSInfo::CpqmCDSInfo():
    m_src_protocol(-1),
    m_cds_flag(0),
    m_cds_step(-1),
    m_cds_rescan(false),
    m_cds_status(0)
{


    for (int l_index = 0; l_index < PQM_NUM_COIL_PORT ; l_index++) {
        m_org_coordinatetbl [l_index] = 0 ;
        m_used_locationtbl[l_index] = 0 ;
        m_coil_tbl [l_index] = 0 ;
    }

    memset(m_coordinatetbl, 0 , sizeof(Vft_CdsCoilLocation_t) * PQM_NUM_COIL_PORT);
    memset(m_locationtbl, 0 , sizeof(Vft_CdsCoilLocation_t) * PQM_NUM_COIL_PORT);


}


CpqmCDSInfo::~CpqmCDSInfo()
{

}

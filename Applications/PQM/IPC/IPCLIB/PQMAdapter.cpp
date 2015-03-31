//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-780
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMAdapter.cpp
 *  Overview: Implementation of CPQMAdapter class.
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
#include "stdafx.h"
#include "PQMAdapter.h"
#include <PQM/PQMLogMgr.h>
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"

//****************************Method Header************************************
//Method Name   : CPQMAdapter()
//Author        : PATNI\LK
//Purpose       :
//*****************************************************************************
CPQMAdapter::CPQMAdapter(
)
{
}

//****************************Method Header************************************
//Method Name   : ~CPQMAdapter()
//Author        : PATNI\LK
//Purpose       :
//*****************************************************************************
CPQMAdapter::~CPQMAdapter(
)
{
}

//+Patni-NP/2010Apr26/Added/MaFT MSA251-204
//****************************Method Header********************
//Method Name   :	CPQMAdapter::GetHeaderSize
//Author        :	NP
//Purpose       :
//*************************************************************
int CPQMAdapter::GetHeaderSize()
{
    int l_header_size = 0;

    AM_ClientMessageHeader_t client_msg;
    UNREFERENCED_PARAMETER(client_msg);
    l_header_size = sizeof(client_msg);

    return l_header_size;
}

//****************************Method Header********************
//Method Name   :	CPQMAdapter::GetBodySize
//Author        :	NP
//Purpose       :
//*************************************************************
int CPQMAdapter::GetBodySize(const PLBYTE buffer)
{
    if (! buffer) return -1;

    int body_sz = ((AM_ClientMessageHeader_t*)(buffer))->byte_count;

    if (body_sz < 0) return -1;

    return body_sz;
}

//****************************Method Header********************
//Method Name   :	CPQMAdapter::GetBodyPtr
//Author        :	NP
//Purpose       :
//*************************************************************
PLBYTE CPQMAdapter::GetBodyPtr(int sz)
{
    //Patni-KSS/2011Feb21/Modified/CPP Test tool Execution
    UNREFERENCED_PARAMETER(sz);
    return NULL;
}
//-Patni-NP/2010Apr26/Added/MaFT MSA251-204

//+Patni-PH/2010May04/Commented/MaFT MSA251-204
//****************************Method Header************************************
//Method Name   : ReadHeader()
//Author        : PATNI\LK
//Purpose       :
//*****************************************************************************
/*
bool CPQMAdapter::ReadHeader(
    PLBYTE* buffer,
    int buffer_size,
    long* header_size,
    long* data_size
)
{
    CPQMLogMgr::GetInstance()->WriteTraceLog(DOMAIN_NAME,
            USER_FUNC_MARKER,
            _T("CPQMAdapter::ReadHeader"),
            _T("CPQMAdapter::ReadHeader"), true);

    bool return_flag = false;

    int head_size = 0;
    AM_ClientMessageHeader_t client_msg;
    head_size = sizeof(client_msg);

    if (buffer) {
        if (buffer_size < head_size) {

            return return_flag;
        }

        CSocketMsg msg(*buffer, buffer_size);
        memcpy(&client_msg, msg.GetBuffer(), head_size);

        if (client_msg.byte_count > 0) {
            *data_size = client_msg.byte_count;

        } else {
            *data_size = 0;
        }

        *header_size = head_size;
        return_flag = true;
    }

    return return_flag;
}*/
//-Patni-PH/2010May04/Commented/MaFT MSA251-204



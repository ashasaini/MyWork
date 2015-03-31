//Redmine-781_CPP_TestTool
//Redmine-774
// PQMProConAdapter.cpp: implementation of the CPQMProConAdapter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PQMProConAdapter.h"
#include <tami/libpromsg/promsg.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPQMProConAdapter::CPQMProConAdapter()
{

}

CPQMProConAdapter::~CPQMProConAdapter()
{

}

//+Patni-NP/2010Apr26/Added/MaFT MSA251-204
//****************************Method Header********************
//Method Name   :	CPQMProConAdapter::GetHeaderSize
//Author        :	NP
//Purpose       :
//*************************************************************
int CPQMProConAdapter::GetHeaderSize()
{
    //Patni-AMT/2010May07/Modified/JaFT# Warning Removal.
    return PROMSG_HEAD_LEN;//sizeof(client_msg);
}

//****************************Method Header********************
//Method Name   :	CPQMProConAdapter::GetBodySize
//Author        :	NP
//Purpose       :
//*************************************************************
int CPQMProConAdapter::GetBodySize(PLBYTE buffer)
{
    if (! buffer) return -1;

    int body_sz = ((proMsg_t*)(buffer))->length;

    if (body_sz < 0) return -1;

    return body_sz;
}

//****************************Method Header********************
//Method Name   :	CPQMProConAdapter::GetBodyPtr
//Author        :	NP
//Purpose       :
//*************************************************************
PLBYTE CPQMProConAdapter :: GetBodyPtr(int sz)
{
    //Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(sz);

    return NULL;
}
//-Patni-NP/2010Apr26/Added/MaFT MSA251-204

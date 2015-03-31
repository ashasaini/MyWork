//Redmine-774
//Redmine-778
//Redmine-781
// AutoFunctions.cpp: implementation of the CAutoInsertProtocol class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AutoInsertProtocol.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CAutoInsertProtocol::CAutoInsertProtocol(
    const bool f_auto_flag
) : m_auto_flag(f_auto_flag),
    m_auto_protocol(NULL)
{
}

CAutoInsertProtocol::~CAutoInsertProtocol()
{
    m_auto_protocol = NULL;

}


//**************************************************************************
//Method Name   : GetAutoMapFlag
//Author        : PATNI/DKH
//Purpose       : This function retrieves the current setting of auto flag
//**************************************************************************
bool CAutoInsertProtocol::GetAutoFlag() const
{
    return m_auto_flag;
}

//**************************************************************************
//Method Name   : GetAutoMapProtocol
//Author        : PATNI/DKH
//Purpose       : This function is used to get the value of Map Protocol
//**************************************************************************
CPqmProtocol* CAutoInsertProtocol::GetAutoProtocol() const
{
    return m_auto_protocol;
}
//**************************************************************************
//Method Name   : SetAutoMapProtocol
//Author        : PATNI/DKH
//Purpose       : This function is used to set the value of Map Protocol
//**************************************************************************
void CAutoInsertProtocol::SetAutoProtocol(CPqmProtocol* const f_auto_protocol)
{
    m_auto_protocol = f_auto_protocol ;
}


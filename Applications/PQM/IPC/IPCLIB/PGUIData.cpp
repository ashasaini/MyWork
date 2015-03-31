// PGUIData.cpp: implementation of the CPGUIData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PGUIData.h"

using namespace MR::ACQ::SM::COMMON;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PGUIData::PGUIData()
{

}

PGUIData::~PGUIData()
{
    UTIL::DeleteAndClearMFCList <DefaultPGUIDXMLData> (&m_pguidata);
}
//**************************************************************************
//Method Name   : TagDataInformation
//Author        : PATNI
//Purpose       :
//**************************************************************************
TagDataInformation::TagDataInformation()
{
    m_inscan_type = _T("") ;
    m_pguid_value = _T("");

}
//**************************************************************************
//Method Name   : ~TagDataInformation
//Author        : PATNI
//Purpose       :
//**************************************************************************
TagDataInformation::~TagDataInformation()
{
}

//**************************************************************************
//Method Name   : InscanCondition
//Author        : PATNI
//Purpose       :
//**************************************************************************
InscanCondition::InscanCondition()
{
    m_inscan_tag_name = _T("");
    m_inscan_value = _T("");
}
//**************************************************************************
//Method Name   : ~InscanCondition
//Author        : PATNI
//Purpose       :
//**************************************************************************
InscanCondition::~InscanCondition()
{

}
//**************************************************************************
//Method Name   : DefaultPGUIDXMLData
//Author        : PATNI
//Purpose       :
//**************************************************************************
DefaultPGUIDXMLData::DefaultPGUIDXMLData()
{

}
//**************************************************************************
//Method Name   : ~DefaultPGUIDXMLData
//Author        : PATNI
//Purpose       :
//**************************************************************************
DefaultPGUIDXMLData::~DefaultPGUIDXMLData()
{
    UTIL::DeleteAndClearMFCList <InscanCondition> (&m_inscan_cond_data_list);
}




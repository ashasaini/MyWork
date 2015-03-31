// AutoInsertXmlData.cpp: implementation of the AutoInsertXmlData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AutoInsertXmlData.h"

using namespace MR::ACQ::SM::COMMON;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//**************************************************************************
//Method Name   : AutoInsertXMLData
//Author        : PATNI
//Purpose       :
//**************************************************************************

AutoInsertXMLData::AutoInsertXMLData()
{
}

//**************************************************************************
//Method Name   : ~AutoInsertXMLData
//Author        : PATNI
//Purpose       :
//**************************************************************************
AutoInsertXMLData::~AutoInsertXMLData()
{
    UTIL::DeleteAndClearMFCList <ShimMapStudy> (&m_shim_study) ;
    UTIL::DeleteAndClearMFCList <ShimMapStudy> (&m_map_study) ;
}

//**************************************************************************
//Method Name   : TagInformation
//Author        : PATNI
//Purpose       :
//**************************************************************************
TagInformation::TagInformation()
{
    m_tag_name = _T("") ;
    m_value = _T("");

}

//**************************************************************************
//Method Name   : TagInformation
//Author        : PATNI
//Purpose       :
//**************************************************************************
TagInformation::TagInformation(const CString& f_tag_name, const CString& f_value)
{
    m_tag_name = f_tag_name ;
    m_value = f_value ;
}

//**************************************************************************
//Method Name   : ~TagInformation
//Author        : PATNI
//Purpose       :
//**************************************************************************
TagInformation::~TagInformation()
{
}

//**************************************************************************
//Method Name   : Condition
//Author        : PATNI
//Purpose       :
//**************************************************************************
Condition::Condition()
{

}

//**************************************************************************
//Method Name   : ~Condition
//Author        : PATNI
//Purpose       :
//**************************************************************************
Condition::~Condition()
{
    UTIL::DeleteAndClearMFCList<TagInformation> (&m_tag_info) ;
}

//**************************************************************************
//Method Name   : ScanRegionEnlargement
//Author        : PATNI
//Purpose       :
//**************************************************************************
ScanRegionEnlargement::ScanRegionEnlargement()
{
    m_tag = _T("");
    m_percentage_range = 0;
}

//**************************************************************************
//Method Name   : ScanRegionEnlargement
//Author        : PATNI
//Purpose       :
//**************************************************************************
ScanRegionEnlargement::ScanRegionEnlargement(const CString& f_tag, const long f_percentage_range)
{
    m_tag = f_tag ;
    m_percentage_range = f_percentage_range ;
}

//**************************************************************************
//Method Name   : ~ScanRegionEnlargement
//Author        : PATNI
//Purpose       :
//**************************************************************************
ScanRegionEnlargement::~ScanRegionEnlargement()
{
}

//**************************************************************************
//Method Name   : ScanRegionLimit
//Author        : PATNI
//Purpose       :
//**************************************************************************
ScanRegionLimit::ScanRegionLimit()
{
    m_tag = _T("");
    m_limit_range = -1;
}


//**************************************************************************
//Method Name   : ~ScanRegionLimit
//Author        : PATNI
//Purpose       :
//**************************************************************************
ScanRegionLimit::~ScanRegionLimit()
{
}

//**************************************************************************
//Method Name   : ShimMapStudy
//Author        : PATNI
//Purpose       :
//**************************************************************************
ShimMapStudy::ShimMapStudy()
{

}

//**************************************************************************
//Method Name   : ~ShimMapStudy
//Author        : PATNI
//Purpose       :
//**************************************************************************
ShimMapStudy::~ShimMapStudy()
{
    UTIL::DeleteAndClearMFCList <Condition> (&m_condition_list) ;
    UTIL::DeleteAndClearMFCList <ScanRegionEnlargement>(&m_scanenlargement_list) ;
}
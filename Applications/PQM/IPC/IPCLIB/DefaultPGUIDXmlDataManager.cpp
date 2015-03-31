// DefaultPGUIDXmlDataManager.cpp: implementation of the CDefaultPGUIDXmlDataManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DefaultPGUIDXmlDataManager.h"
#include <PQM/PQMLogMgr.h>
#include "PqmIpcLibConstants.h"
#include "DefaultPGUIDXMLReader.h"
#include "PGUIData.h"
#include "PqmProtocol.h"
#include "PqmPrMode.h"

const CString ANATOMY_STRING_NAME = _T("SVN_PLN_SCAN_ANATOMY");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//**************************************************************************
//Method Name   : CDefaultPGUIDXmlDataManager
//Author        : PATNI
//Purpose       : Constructor
//**************************************************************************
CDefaultPGUIDXmlDataManager::CDefaultPGUIDXmlDataManager()
{

}
//**************************************************************************
//Method Name   : ~CDefaultPGUIDXmlDataManager
//Author        : PATNI
//Purpose       : Destructor
//**************************************************************************
CDefaultPGUIDXmlDataManager::~CDefaultPGUIDXmlDataManager()
{

}
//**************************************************************************
//Method Name   : ReadDefaultPGUIDXML
//Author        : PATNI
//Purpose       : Wrapper function for ReadDefaultPGUIDXML of CDefaultPGUIDXMLReader
//**************************************************************************
bool CDefaultPGUIDXmlDataManager::ReadDefaultPGUIDXML()
{
    LPCTSTR FUNC_NAME = _T("CDefaultPGUIDXmlDataManager::ReadDefaultPGUIDXML");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    CDefaultPGUIDXMLReader l_default_pguid_data;

    return l_default_pguid_data.ReadDefaultPGUIDXML(m_pguid_insert_data);
}
//**************************************************************************
//Method Name   : GetPGUIDFromXML
//Author        : PATNI
//Purpose       : Return the Default PGUID
//**************************************************************************
CString CDefaultPGUIDXmlDataManager::GetPGUIDFromXML(
    const int f_inscan_type, const int f_acq_order, CPQMStudy* f_pqm_study)const
{
    LPCTSTR FUNC_NAME = _T("CDefaultPGUIDXmlDataManager::GetPGUIDFromXML");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    CString l_return_string = _T("");

    const CList<DefaultPGUIDXMLData*, DefaultPGUIDXMLData*> * l_default_xml_data = NULL;

    l_default_xml_data = &m_pguid_insert_data.m_pguidata;

    POSITION l_condition_pos = NULL , l_tag_info_pos = NULL;

    DefaultPGUIDXMLData* l_xml_data = NULL ;
    TagDataInformation* l_tag_info = NULL ;
    InscanCondition* l_condition = NULL;
    CString l_str_anatomy = _T("");
    POSITION l_pguid_data_pos = m_pguid_insert_data.m_pguidata.GetHeadPosition();

    while (l_pguid_data_pos) {

        l_xml_data = l_default_xml_data->GetNext(l_pguid_data_pos);
        l_tag_info = l_xml_data->m_tagdata_info;

        //Check for Inscan Type
        if (NUM_0 == l_tag_info->m_inscan_type.CompareNoCase(GetInScanString(f_inscan_type))) {

            l_condition_pos = l_xml_data->m_inscan_cond_data_list.GetHeadPosition();

            if (l_condition_pos != NULL) {

                CPqmProtocol* l_pqm_protocol = f_pqm_study->GetProtocol(f_acq_order);

                int l_anatomy = l_pqm_protocol->GetPqmPrMode()->GetAnatomy();

                while (l_condition_pos) {

                    l_condition = l_xml_data->m_inscan_cond_data_list.GetNext(l_condition_pos);

                    //Check for anatomy name
                    if (NUM_0 == l_condition->m_inscan_tag_name.CompareNoCase(ANATOMY_STRING_NAME)) {
                        //Check for anatomy value
                        if (NUM_0 == l_condition->m_inscan_value.CompareNoCase(GetAnatomyString(l_anatomy))) {
                            return l_tag_info->m_pguid_value;
                        }
                    }
                }

            } else {
                return l_tag_info->m_pguid_value;
            }
        }
    }

    return l_return_string;
}

//**************************************************************************
//Method Name   : GetInScanString
//Author        : PATNI
//Purpose       :
//**************************************************************************
CString CDefaultPGUIDXmlDataManager::GetInScanString(int f_inscantype)const
{
    switch (f_inscantype) {

        case VFC_INSCAN_FMRI:
            return _T("VFC_INSCAN_FMRI");

        case VFC_INSCAN_DWI:
            return _T("VFC_INSCAN_DWI");

        case VFC_INSCAN_PWI:
            return _T("VFC_INSCAN_PWI");

        case VFC_INSCAN_MRS:
            return _T("VFC_INSCAN_MRS");

        case VFC_INSCAN_CARDIAC:
            return _T("VFC_INSCAN_CARDIAC");

        case VFC_INSCAN_MR3D:
            return _T("VFC_INSCAN_MR3D");

        default:
            return _T("VFC_INSCAN_NONE");
    }
}
//**************************************************************************
//Method Name   : GetInScanString
//Author        : PATNI
//Purpose       :
//**************************************************************************
CString CDefaultPGUIDXmlDataManager::GetAnatomyString(int f_anatomy)const
{
    switch (f_anatomy) {

        case 1:
            return _T("VFC_ANATOMY_ABDOMEN");

        case 2:
            return _T("VFC_ANATOMY_CHEST");

        case 3:
            return _T("VFC_ANATOMY_PELVIS");

        case 4:
            return _T("VFC_ANATOMY_TL_SPINE");

        case 5:
            return _T("VFC_ANATOMY_SHOULDER");

        case 6:
            return _T("VFC_ANATOMY_HEAD");

        case 7:
            return _T("VFC_ANATOMY_C_SPINE");

        case 8:
            return _T("VFC_ANATOMY_EXTRA");

        case 9:
            return _T("VFC_ANATOMY_HAND");

        case 10:
            return _T("VFC_ANATOMY_ANKLE");

        default:
            return _T("VFC_INSCAN_NONE");
    }
}

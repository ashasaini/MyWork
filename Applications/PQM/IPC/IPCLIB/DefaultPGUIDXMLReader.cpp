// DefaultPGUIDXMLReader.cpp: implementation of the CDefaultPGUIDXMLReader class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "DefaultPGUIDXMLReader.h"
#include <PDataParser/XMLTag.h>
#include <PDataParser/MSXML4DomParser.h>
#include <PQM/Resource.h>
#include <PQM/PqmUtilities.h>
#include <PQM/PQMLogMgr.h>
#include "PqmIpcLibConstants.h"

const static CPTString  ROOT				= _T("ROOT");
const static CPTString  DEFAULT_PGUID       = _T("DEFAULTPGUID");
const static CPTString  D_PGUID             = _T("D.PGUID");
const static CPTString  INSCAN_TYPE         = _T("InscanType");
const static CPTString  INSCAN_CONDITION    = _T("Condition");
const static CPTString  VARFIELD            = _T("VarField");

//****************************Method Header************************************
//Method Name   :CDefaultPGUIDXMLReader()
//Author        :PATNI
//Purpose       :Constructor
//*****************************************************************************
CDefaultPGUIDXMLReader::CDefaultPGUIDXMLReader()
{

}
//****************************Method Header************************************
//Method Name   :~CDefaultPGUIDXMLReader()
//Author        :PATNI
//Purpose       :Destructor
//*****************************************************************************
CDefaultPGUIDXMLReader::~CDefaultPGUIDXMLReader()
{

}
//**************************************************************************
//Method Name   : ReadAutoInsertProtocolXML
//Author        : PATNI
//Purpose       : Reading data from PQM_Default_PGUID.xml file
//**************************************************************************
bool CDefaultPGUIDXMLReader::ReadDefaultPGUIDXML(
    PGUIData& f_pguid_insert_data
)
{
    LPCTSTR FUNC_NAME = _T("CDefaultPGUIDXMLReader::ReadAutoInsertProtocolXML");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    CPTString filename = CPqmUtilities::GetPqmDefaultPGUIDFileName();

    if (filename.IsEmpty()) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PQM_Default_PGUID XMLFile Empty"));

        return false;
    }

    struct _stat statusstruct;

    const int filestatus = _wstat(filename, &statusstruct);

    if (filestatus != 0) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PQM_Default_PGUID XML File Not Found"));

        CString l_error = CPqmUtilities::GetMultiLingualString(_T("IDS_DEFAULT_PGUID_CONFIG_FILE_MISSING"));
        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
            l_error, MPlus::EH::Crucial, APP_NAME, APP_NAME);
        return false;
    }

    CMSXML4DomParser domparser ;

    try {
        domparser.LoadFromFile(filename);

    }  catch (...) {

        CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
            CPqmUtilities::GetMultiLingualString(_T("IDS_INVALID_VALUES_IN_XML_FILE")),
            MPlus::EH::Crucial, APP_NAME, APP_NAME);

        return false;
    }

    CXMLTag* root = (CXMLTag*) domparser.GetRoot();

    if (ROOT != root->GetName())
        return false;

    int l_count = -1;
    int l_child_count = root->GetChildPSCount();

    for (int i = 0; i < l_child_count; i++) {
        CXMLTag* l_child = (CXMLTag*)root->GetChildPS(i);

        DefaultPGUIDXMLData* l_defaultpguidXMLdata = new  DefaultPGUIDXMLData();
        l_count = l_child->GetChildPSCount();

        if (l_child->GetName() == DEFAULT_PGUID) {

            TagDataInformation* l_tagdata_info = new TagDataInformation();

            for (int i = 0; i < l_count; i++) {

                CXMLTag* l_guidata_child = (CXMLTag*)l_child->GetChildPS(i);

                InsertData(l_guidata_child, l_defaultpguidXMLdata, l_tagdata_info);
            }

            l_defaultpguidXMLdata->m_tagdata_info = l_tagdata_info;
        }

        f_pguid_insert_data.m_pguidata.AddTail(l_defaultpguidXMLdata);
    }

    return true;
}

//**************************************************************************
//Method Name   : InsertData
//Author        : PATNI
//Purpose       : Inserting data in to list
//**************************************************************************
void CDefaultPGUIDXMLReader::InsertData(
    CXMLTag* f_child,
    DefaultPGUIDXMLData* f_defaultpguidxmldata,
    TagDataInformation* l_tagdata_info
)
{
    LPCTSTR FUNC_NAME = _T("CDefaultPGUIDXMLReader::InsertData");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    CXMLTag::NV_POS l_validate = NULL;

    int l_count = f_child->GetChildPSCount();

    CPTString l_attribname = _T(""), l_attribute_val = _T("") , l_attribname1 = _T(""), l_attribute_val1 = _T("") ;

    if (D_PGUID == f_child->GetName()) {
        CXMLTag::NV_POS l_pos;
        l_attribname    = _T("");
        l_attribute_val = _T("");

        l_pos = f_child->GetFirstAttribute(l_attribname, l_attribute_val);
        l_tagdata_info->m_pguid_value = l_attribute_val;

    } else if (INSCAN_TYPE == f_child->GetName()) {
        CXMLTag::NV_POS l_pos;
        l_attribname    = _T("");
        l_attribute_val = _T("");

        l_pos = f_child->GetFirstAttribute(l_attribname, l_attribute_val);
        l_tagdata_info->m_inscan_type = l_attribute_val;

    } else if (INSCAN_CONDITION == f_child->GetName()) {
        const int l_count = f_child->GetChildPSCount();

        for (int i = 0; i < l_count ; i++) {
            InscanCondition* l_inscan_cdn = new InscanCondition;

            CXMLTag::NV_POS l_pos;
            CXMLTag* l_child_varfield = (CXMLTag*)f_child->GetChildPS(i);

            l_attribname    = _T("");
            l_attribute_val = _T("");
            l_pos = l_child_varfield->GetFirstAttribute(l_attribname, l_attribute_val);
            l_pos = l_child_varfield->GetNextAttribute(l_pos, l_attribname1, l_attribute_val1);
            l_inscan_cdn->m_inscan_tag_name = l_attribute_val;
            l_inscan_cdn->m_inscan_value = l_attribute_val1;

            f_defaultpguidxmldata->m_inscan_cond_data_list.AddTail(l_inscan_cdn);

        }
    }
}



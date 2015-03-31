// AutoInsertXMLReader.cpp: implementation of the CAutoInsertXMLReader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AutoInsertXMLReader.h"
#include <PDataParser/XMLTag.h>
#include <PDataParser/MSXML4DomParser.h>
#include <PQM/Resource.h>
#include <PQM/PqmUtilities.h>
#include <PQM/PQMLogMgr.h>
#include "PqmIpcLibConstants.h"

const static CPTString  STUDY_FILE_SELECTION = _T("StudyFileSelection");
const static CPTString  AUTO_SHIMMING        = _T("AutoShimming");
const static CPTString  AUTO_MAPP             = _T("AutoMap");
const static CPTString  SHIM_STUDY           = _T("ShimStudy");
const static CPTString  MAP_STUDY            = _T("MapStudy");
const static CPTString  FILE_PATH            = _T("FilePath");
const static CPTString  SCAN_REGION_ENLARGEMENT = _T("ScanRegionEnlargementRange");
const static CPTString  SCAN_REGION_LIMIT = _T("ScanRegionLimitRange");
const static CPTString CONDITION             = _T("Condition");
const static CPTString VARFIELD              = _T("VarField");

//**************************************************************************
//Method Name   : ReadAutoInsertProtocolXML
//Author        : PATNI
//Purpose       :
//**************************************************************************
bool CAutoInsertXMLReader::ReadAutoInsertProtocolXML(
    AutoInsertXMLData& f_auto_insert_data
)
{

    LPCTSTR FUNC_NAME = _T("CAutoInsertXMLReader::ReadAutoInsertProtocolXML");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    CPTString filename = CPqmUtilities::GetCoilSarFileName();

    if (filename.IsEmpty()) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("AutoInsert XMLFile Empty"));

        return false;
    }

    struct _stat statusstruct;

    const int filestatus = _wstat(filename, &statusstruct);

    if (filestatus != 0) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("AutoInsert XML File Not Found "));

        CString l_error = CPqmUtilities::GetMultiLingualString(_T("IDS_AUTOINSERT_CONFIG_FILE_MISSING"));
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

    if (STUDY_FILE_SELECTION != root->GetName())
        return false;

    int l_count = -1;
    int l_child_count = root->GetChildPSCount();

    for (int i = 0; i < l_child_count; i++) {
        CXMLTag* l_child = (CXMLTag*)root->GetChildPS(i);

        l_count = l_child->GetChildPSCount();

        bool l_auto_shim_map;

        if (AUTO_SHIMMING == l_child->GetName()) {
            l_auto_shim_map = false;

        } else if (AUTO_MAPP == l_child->GetName()) {
            l_auto_shim_map = true;
        }

        CXMLTag* l_mapshim_child = (CXMLTag*)l_child->GetChildPS(0);

        if (l_mapshim_child->GetName() == SHIM_STUDY || l_mapshim_child->GetName() == MAP_STUDY) {
            for (int i = 0; i < l_count; i++) {
                CXMLTag* l_mashim_child = (CXMLTag*)l_child->GetChildPS(i);

                if (true == l_auto_shim_map) {
                    InsertData(l_mashim_child, f_auto_insert_data.m_map_study);

                } else {
                    InsertData(l_mashim_child, f_auto_insert_data.m_shim_study);
                }
            }
        }
    }

    return true;
}

//**************************************************************************
//Method Name   : InsertData
//Author        : PATNI
//Purpose       :
//**************************************************************************
void CAutoInsertXMLReader::InsertData(
    CXMLTag* f_child,
    CList < ShimMapStudy*,
    ShimMapStudy* > & f_shim_study
)
{

    LPCTSTR FUNC_NAME = _T("CAutoInsertXMLReader::InsertData");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    ShimMapStudy* l_study = new ShimMapStudy;

    CXMLTag* l_filepath_child = (CXMLTag*)f_child->GetChildPS(0);

    CPTString l_attribname1 = _T(""), l_attribute_val1 = _T(""), l_attribute = _T("");
    CXMLTag::NV_POS l_pos = l_filepath_child->GetFirstAttribute(l_attribname1, l_attribute);

    CXMLTag::NV_POS l_validate = NULL;

    if (l_pos == l_validate) {
        return ;
    }

    if (FILE_PATH != l_attribname1) {
        return ;
    }

    l_study->m_filename = l_attribute;

    int l_count = f_child->GetChildPSCount();

    CPTString l_attribname = _T(""), l_attribute_val = _T("");

    //shimstudy/mapstudy loop

    for (int i = 1; i < l_count; i++) {
        CXMLTag* l_child_condition = (CXMLTag*)f_child->GetChildPS(i);

        if (CONDITION == l_child_condition->GetName()) {

            const int l_count = l_child_condition->GetChildPSCount();

            Condition* l_cdn = new Condition;

            for (int j = 0; j < l_count ; j++) {
                CXMLTag::NV_POS l_pos;

                CXMLTag* l_child_varfield = (CXMLTag*)l_child_condition->GetChildPS(j);

                if (VARFIELD != l_child_varfield->GetName())
                    break;

                l_attribname    = _T("");
                l_attribute_val = _T("");
                l_pos = l_child_varfield->GetFirstAttribute(l_attribname, l_attribute_val);
                l_pos = l_child_varfield->GetNextAttribute(l_pos, l_attribname1, l_attribute_val1);

                TagInformation* l_tag_info = new TagInformation((CString)l_attribute_val, (CString)l_attribute_val1);
                l_cdn->m_tag_info.AddTail(l_tag_info);
            }

            l_study->m_condition_list.AddTail(l_cdn);

        } else if (f_child->GetChildPS(i)->GetName() == SCAN_REGION_ENLARGEMENT) {
            CXMLTag::NV_POS l_pos;

            CXMLTag* l_scan_region = (CXMLTag*)f_child->GetChildPS(i);

            const int l_count = l_scan_region->GetChildPSCount();

            for (int j = 0 ; j < l_count ; j++) {

                CXMLTag* l_child_tag = (CXMLTag*)l_scan_region->GetChildPS(j);

                l_pos = l_child_tag->GetFirstAttribute(l_attribname1, l_attribute);
                l_attribname1 = l_child_tag->GetName();

                ScanRegionEnlargement* l_scanrgn = new ScanRegionEnlargement((CString)l_attribname1, ReadLongValue(l_attribute)) ;
                l_study->m_scanenlargement_list.AddTail(l_scanrgn);
            }

        } else if (f_child->GetChildPS(i)->GetName() == SCAN_REGION_LIMIT) {
            CXMLTag::NV_POS l_pos;

            CXMLTag* l_scan_region_limit = (CXMLTag*)f_child->GetChildPS(i);

            const int l_count = l_scan_region_limit->GetChildPSCount();

            for (int j = 0 ; j < l_count ; j++) {

                CXMLTag* l_child_tag = (CXMLTag*)l_scan_region_limit->GetChildPS(j);

                l_pos = l_child_tag->GetFirstAttribute(l_attribname1, l_attribute);
                l_attribname1 = l_child_tag->GetName();
                l_attribute.TrimLeft();
                l_attribute.TrimRight();

                if (l_attribute.IsEmpty()) {
                    l_attribute = _T("-1");
                }

                CString l_tag_name = (CString)l_attribname1 ;
                ScanRegionLimitEnum ScanRegionLimitTag = ScanRegionLimit_Total;

                if (l_tag_name.CompareNoCase(_T("SL")) == 0) {
                    ScanRegionLimitTag = ScanRegionLimit_SL;

                } else if (l_tag_name.CompareNoCase(_T("PE")) == 0) {
                    ScanRegionLimitTag = ScanRegionLimit_PE;

                } else if (l_tag_name.CompareNoCase(_T("RO")) == 0) {
                    ScanRegionLimitTag = ScanRegionLimit_RO;

                } else {
                    continue;
                }

                l_study->m_scanregionlimit_list[ScanRegionLimitTag].m_tag = l_tag_name;
                l_study->m_scanregionlimit_list[ScanRegionLimitTag].m_limit_range = ReadFloatValue(l_attribute);
            }
        }
    }

    f_shim_study.AddTail(l_study);

}

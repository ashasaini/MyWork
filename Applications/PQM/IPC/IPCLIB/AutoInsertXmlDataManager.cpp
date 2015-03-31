// AutoInsertXmlDataManager.cpp: implementation of the CAutoInsertData class.
//
//////////////////////////////////////////////////////////////////////

// AutoInsertXmlDataManager.cpp: implementation of the CAutoInsertData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AutoInsertXmlDataManager.h"
#include <tami/dicom/anatomy.h>
#include <PQM/PQMLogMgr.h>
#include "AutoInsertXMLReader.h"
#include "PqmProtocol.h"
#include "PqmPrMode.h"

#define ANATOMY_TYPE_MAX		10
const CString MAP_COIL_STRING = _T("SVN_PLN_SECTION_RECVR_COIL");
const CString SHIM_COIL_STRING = _T("SVN_PLN_RECVR_COIL_SHOW_LIST");
const CString ANATOMY_STRING = _T("SVN_PLN_SCAN_ANATOMY");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//**************************************************************************
//Method Name   : CAutoInsertXmlDataManager
//Author        : PATNI
//Purpose       : Constructor
//**************************************************************************
CAutoInsertXmlDataManager::CAutoInsertXmlDataManager(
) :
    m_shimmapstudy_ptr(NULL),
    m_study_file_selection()
{

}
//**************************************************************************
//Method Name   : ~CAutoInsertXmlDataManager
//Author        : PATNI
//Purpose       : Destructor
//**************************************************************************
CAutoInsertXmlDataManager::~CAutoInsertXmlDataManager()
{

}

//**************************************************************************
//Method Name   : ReadAutoInsertProtocolXML
//Author        : PATNI
//Purpose       :
//**************************************************************************

bool CAutoInsertXmlDataManager::ReadAutoInsertProtocolXML()
{
    CAutoInsertXMLReader l_auto_insert_data;

    return l_auto_insert_data.ReadAutoInsertProtocolXML(m_study_file_selection);
}

//**************************************************************************
//Method Name   : GenerateMapShimData
//Author        : PATNI
//Purpose       :
//**************************************************************************
ShimMapStudy* CAutoInsertXmlDataManager::GenerateMapShimData(
    CPqmProtocol* const f_targ_protocol,
    const scan_mode f_mode
)
{

    m_shimmapstudy_ptr = GetAutoShimMapData(f_targ_protocol, f_mode);
    return m_shimmapstudy_ptr;
}


//**************************************************************************
//Method Name   : GetAutoShimMapData
//Author        : PATNI
//Purpose       :
//**************************************************************************
ShimMapStudy* CAutoInsertXmlDataManager::GetAutoShimMapData(
) const
{
    return m_shimmapstudy_ptr;
}


//**************************************************************************
//Method Name   : GetAutoShimMapData
//Author        : PATNI
//Purpose       :
//**************************************************************************
ShimMapStudy* CAutoInsertXmlDataManager::GetAutoShimMapData(
    CPqmProtocol* const f_targ_protocol,
    const scan_mode f_mode
) const
{
    if (NULL == f_targ_protocol)
        return NULL ;

    const CList<ShimMapStudy*, ShimMapStudy* > * l_shim_map_study = NULL ;

    switch (f_mode) {
        case SHIM_MODE:
            l_shim_map_study = &(m_study_file_selection.m_shim_study) ;
            break;

        case MAP_MODE:
            l_shim_map_study = &(m_study_file_selection.m_map_study) ;
            break;
    }

    if (NULL == l_shim_map_study) {
        return NULL ;
    }

    POSITION l_shimmap_study_pos = l_shim_map_study->GetHeadPosition();
    POSITION l_condition_pos = NULL , l_tag_info_pos = NULL;

    ShimMapStudy* l_shim_study = NULL ;
    Condition* l_condition = NULL ;
    TagInformation* l_tag_info = NULL ;

    bool l_condition_satisfied = true;

    while (l_shimmap_study_pos) {

        l_condition_satisfied = true ;

        l_shim_study = l_shim_map_study->GetNext(l_shimmap_study_pos);
        l_condition_pos = l_shim_study->m_condition_list.GetHeadPosition();

        while (l_condition_pos) {

            l_condition = l_shim_study->m_condition_list.GetNext(l_condition_pos);
            l_tag_info_pos = l_condition->m_tag_info.GetHeadPosition();

            while (l_tag_info_pos) {

                l_tag_info = l_condition->m_tag_info.GetNext(l_tag_info_pos);
                l_condition_satisfied = IsConditionSatisfied(*l_tag_info , f_targ_protocol, f_mode);

                if (false == l_condition_satisfied) {
                    break;
                }
            }

            if (l_condition_satisfied) {
                return l_shim_study;

            }

        }

        if (l_condition_satisfied) {
            return l_shim_study;
        }

    }

    return NULL ;
}

//**************************************************************************
//Method Name   : IsConditionSatisfied
//Author        : PATNI
//Purpose       :
//**************************************************************************

bool CAutoInsertXmlDataManager::IsConditionSatisfied(
    const TagInformation& f_tag_info,
    CPqmProtocol* const f_targ_protocol,
    const scan_mode f_mode
)const
{

    bool l_ischecking_this = false;

    if (IsConditionSatisfiedForCoil(f_tag_info, f_targ_protocol, l_ischecking_this, f_mode)) {
        return true ;

    } else if (!l_ischecking_this && IsConditionSatisfiedForSar(f_tag_info, f_targ_protocol, l_ischecking_this)) {
        return true ;
    }

    return false ;
}

//**************************************************************************
//Method Name   : IsConditionSatisfiedForCoil
//Author        : PATNI
//Purpose       :
//**************************************************************************

bool CAutoInsertXmlDataManager::IsConditionSatisfiedForCoil(
    const TagInformation& f_tag_info,
    CPqmProtocol* const f_targ_protocol,
    bool& f_is_checking_this,
    const scan_mode f_mode
)const
{

    LPCTSTR FUNC_NAME = _T("CAutoInsertXmlDataManager::IsConditionSatisfiedForCoil");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));
    f_is_checking_this = false;

    switch (f_mode) {
        case MAP_MODE:

            if (IsCoilConditionSatisfiedForAutoMap(f_tag_info, f_targ_protocol, f_is_checking_this))
                return true;

            break;

        case SHIM_MODE:

            if (IsCoilConditionSatisfiedForAutoShimming(f_tag_info, f_targ_protocol, f_is_checking_this))
                return true;

            break;
    }


    return false;
}

//**************************************************************************
//Method Name   : IsConditionSatisfiedForSar
//Author        : PATNI
//Purpose       :
//**************************************************************************

bool CAutoInsertXmlDataManager::IsConditionSatisfiedForSar(
    const TagInformation& f_tag_info,
    CPqmProtocol* const f_targ_protocol,
    bool& f_is_checking_this
)const
{

    bool l_return_val = false;

    f_is_checking_this = false;

    if (NUM_0 == f_tag_info.m_tag_name.CompareNoCase(ANATOMY_STRING)) {

        f_is_checking_this = true;

        CPqmPrMode* l_mode = f_targ_protocol->GetPqmPrMode();

        if (l_mode) {
            l_return_val = (GetAnatomyValue(f_tag_info.m_value) == l_mode->GetAnatomy());
        }
    }

    return l_return_val;
}

//**************************************************************************
//Method Name   : GetAnatomyValue
//Author        : PATNI
//Purpose       :
//**************************************************************************

int CAutoInsertXmlDataManager::GetAnatomyValue(
    const CString& f_str_anatomy
)const
{

    char* l_anatomy_str = NULL;

    for (int i = 0; i < ANATOMY_TYPE_MAX; i++) {

        WCHAR l_seq_path[MAX_PATH] = {0};
        mbstowcs(l_seq_path, anatomyType[i].anatomyStr, MAX_PATH);

        if (NUM_0 == f_str_anatomy.CompareNoCase(l_seq_path)) {

            return anatomyType[i].anatomyNum;
        }
    }

    return -1;
}

//**************************************************************************
//Method Name   : IsCoilConditionSatisfiedForAutoMap
//Author        : PATNI
//Purpose       :
//**************************************************************************
int CAutoInsertXmlDataManager::IsCoilConditionSatisfiedForAutoMap(
    const TagInformation& f_tag_info,
    CPqmProtocol* const f_targ_protocol,
    bool f_is_checking_this
)const
{

    LPCTSTR FUNC_NAME = _T("CAutoInsertXmlDataManager::IsCoilConditionSatisfiedForAutoMap");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NUM_0 == f_tag_info.m_tag_name.CompareNoCase(MAP_COIL_STRING)) {

        f_is_checking_this = true;

        PqmSaturnCoil_t* l_saturn_coil = f_targ_protocol->GetSaturnCoil();

        CString l_str;

        for (int i = 0; i < l_saturn_coil->numSection; i++) {

            l_str.Format(_T("%X"), l_saturn_coil->RcvCoil[i].coilID);

            if (NUM_0 == f_tag_info.m_value.Compare(l_str)) {

                return true ;
            }
        }
    }

    return false;
}
//**************************************************************************
//Method Name   : IsCoilConditionSatisfiedForAutoShimming
//Author        : PATNI
//Purpose       :
//**************************************************************************
int CAutoInsertXmlDataManager::IsCoilConditionSatisfiedForAutoShimming(
    const TagInformation& f_tag_info,
    CPqmProtocol* const f_targ_protocol,
    bool f_is_checking_this
)const
{

    LPCTSTR FUNC_NAME = _T("CAutoInsertXmlDataManager::IsCoilConditionSatisfiedForAutoShimming");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (NUM_0 == f_tag_info.m_tag_name.CompareNoCase(SHIM_COIL_STRING)) {

        f_is_checking_this = true;

        PqmSaturnCoil_t* l_saturn_coil = f_targ_protocol->GetSaturnCoil();

        CString l_str = _T("");

        for (int l_i = 0; l_i < l_saturn_coil->CoilList.num; l_i++) {
            l_str.Format(_T("%X"), l_saturn_coil->CoilList.coillist[l_i].coilID);

            if (NUM_0 == f_tag_info.m_value.Compare(l_str)) {
                return true;
            }
        }

    }

    return false;
}

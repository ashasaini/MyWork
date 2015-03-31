// PlanDuplicateXMLReader.cpp: implementation of the PlanDuplicateXMLReader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PlanDuplicateXMLReader.h"
#include <PQM/PlanDuplicateConstants.h>
#include <PQM/PQMCommonStructures.h>

//#include <libStudyManager/strcnv.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlanDuplicateXMLReader::CPlanDuplicateXMLReader()
{

}

CPlanDuplicateXMLReader::~CPlanDuplicateXMLReader()
{

}

//************************************Method Header************************************
// Method Name  : GetPresetItem
// Author       : PATNI/ Ajay
// Purpose      :
//***********************************************************************************
int CPlanDuplicateXMLReader::GetPresetItem(VARIANT** f_preset_item_data)
{
    CPTString l_preset_item_id = L"";
    CPTString l_preset_mask_value = L"";
    PresetItem l_presetItem;
    CPresetItemData* l_preset_item_data = (CPresetItemData*)(*f_preset_item_data)->byref;

    const int l_nodecnt = GetNodeCnt();

    for (int l_index = 0; l_index < l_nodecnt; l_index++) {

        l_preset_item_id = GetAttributeValue(l_index,
                                             g_presetitem_str,
                                             g_preset_id_str,
                                             g_attribute_name_1);

        l_preset_mask_value = GetAttributeValue(l_index,
                                                g_presetitem_str,
                                                g_mask_str,
                                                g_attribute_name_2);

        l_presetItem.preset_id =  l_preset_item_id ;
        l_presetItem.mask_value =  l_preset_mask_value ;

        l_preset_item_data->AddTail(l_presetItem);
    }

    return (l_nodecnt > 0) ? E_NO_ERROR : E_ERROR;
}
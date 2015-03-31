//Redmine-778
//Redmine-781
// SelectionSectionData.cpp: implementation of the CSelectionSectionData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SelectionSectionData.h"
#include <PQM/PQMLogMgr.h>
#include "SelectionCoilData.h"
#include "DataManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//****************************Method Header************************************
//Method Name   : CSelectionSectionData()
//Author        : PATNI\HAR
//Purpose       : Constructor
//*****************************************************************************
CSelectionSectionData::CSelectionSectionData(
)
{

}

//****************************Method Header************************************
//Method Name   : ~CSelectionSectionData()
//Author        : PATNI\HAR
//Purpose       : Destructor
//*****************************************************************************
CSelectionSectionData::~CSelectionSectionData(
)
{

}

//****************************Method Header************************************
//Method Name   : AddData()
//Author        : PATNI\HAR
//Purpose       :
//*****************************************************************************
void CSelectionSectionData::AddData(
    void* f_key,
    sedb_rec_p f_sedb_rec
)
{
    LPCTSTR FUNC_NAME = _T("CSelectionSectionData::AddData");

    DB_FUNC_ENTER("[coilSelectionSectionAddList]:");

    if (f_sedb_rec->insectionPri != 1) {
        /* insectionPriが１以外は無視 */
        return;
    }

    //+Patni-RUP/2009Aug20/Modified/cpp test corrections
    //CSelectionCoilData* l_selection_coil_data = CDataManager::GetInstance()->
    //	GetSelectionCoilData();

    CDataManager* l_data_manager = CDataManager::GetInstance();


    if (l_data_manager == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("l_data_manager pointer is NULL"));
        return;
    }

    CSelectionCoilData* l_selection_coil_data = l_data_manager->GetSelectionCoilData();

    if (l_selection_coil_data == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("l_selection_coil_data pointer is NULL"));
        return;
    }

    //-Patni-RUP/2009Aug20/Modified/cpp test corrections

    CoilSelectionCoil_t* l_selection_coil = l_selection_coil_data->GetSelectionCoilData();

    //+Patni-RUP/2009Aug20/Modified/cpp test corrections
    if (l_selection_coil == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("l_selection_coil pointer is NULL"));
        return;
    }

    //-Patni-RUP/2009Aug20/Modified/cpp test corrections

    int i = 0, seccnt = 0;

    if (l_selection_coil->groupId[0] != COILID_NONE) {

        for (i = 0; i < 4; i++) {

            if (l_selection_coil->groupId[i] == f_sedb_rec->coil_id) {

                for (int j = 0; j < i; j++) {

                    seccnt = seccnt + l_selection_coil->Groupsec[j];
                }

                break;
            }
        }

    } else {
        i		= 0;
        seccnt	= 0;
    }


    //+Patni-PJS/2009Sep10/Modified/TMSC CPP test review comment resolved
    /*if(4 == i) {
        i = i-1;
    }*/
    //-Patni-PJS/2009Sep10/Modified/TMSC CPP test review comment resolved

    CoilSelectionSection_t*	l_selection_section = (CoilSelectionSection_t*)malloc(sizeof(CoilSelectionSection_t));

    if (l_selection_section == NULL) {
        // + Himanshu SM 2
        //DB_PRINTF("",("%s:%d malloc error\n",__FILE__,__LINE__));
        //CoilSelectionCommSendControlMsg(PROMSG_COILSEL2PQM_ERROR);
        // - Himanshu SM 2
        return;
    }

    memset(l_selection_section, 0, sizeof(CoilSelectionSection_t));
    l_selection_section->coil_id		=	f_sedb_rec->coil_id;
    l_selection_section->section_id		=	f_sedb_rec->section_id;

    l_selection_section->section		=	f_sedb_rec->section_id + seccnt;
    l_selection_section->minChannel		=	f_sedb_rec->minChannel;
    l_selection_section->link			=	f_sedb_rec->link;

    for (int z = 0; z < 3; z++) {
        l_selection_section->elemXYZ[z]    =   f_sedb_rec->elemXYZ[z];
    }

    /*--------------------------debug-----------------------------*/
    //DB_PRINTF("C",(  "----------------SECTION------------------\n"));
    //DB_PRINTF("C",(  "coil_id        =%x\n", l_selection_section->coil_id));
    //DB_PRINTF("C",(  "section_id     =%x\n", l_selection_section->section_id));
    //DB_PRINTF("C",(  "section        =%d\n", l_selection_section->section));
    //DB_PRINTF("C",(  "minChannel     =%d\n", l_selection_section->minChannel));
    //DB_PRINTF("C",(  "link           =%x\n", l_selection_section->link));
    //DB_PRINTF("C",(  "-----------------------------------------\n"));
    /*------------------------------------------------------------*/

    /* Section数カウント */

    if (i < 4) { //Patni-PJS/2009Sep10/Modified/TMSC CPP test review comment resolved

        l_selection_coil->numofsec++;
        l_selection_coil->Groupsec[i]++;

        //+Patni-PJS/2009Sep10/Modified/TMSC CPP test review comment resolved

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Array out of Bound"));
        return;
    }

    //-Patni-PJS/2009Sep10/Modified/TMSC CPP test review comment resolved

    //DB_PRINTF("C",("COIL: %x SECTION: %d\n",l_selection_section->coil_id,
    //	l_selection_section->section_id));

    //Additional Information Section
    l_selection_coil->section = dllAppendNode(l_selection_coil->section,
                                l_selection_section, 0, NULL);

    DLL_FIND_HEAD(l_selection_coil->section);
}
//Redmine-783
//Redmine-774
//Redmine-778
//Redmine-780
//Redmine-781
//Redmine-782
// SectionCoilData.cpp: implementation of the CSectionCoilData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SelectionCoilData.h"
#include <PQM/PQMLogMgr.h>
#include "Pqm.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//****************************Method Header************************************
//Method Name   : CSectionCoilData()
//Author        : PATNI\HAR
//Purpose       : Constructor
//*****************************************************************************
CSelectionCoilData ::CSelectionCoilData():
    m_temp_selection_coil(NULL) //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
{
}

//****************************Method Header************************************
//Method Name   : ~CSectionCoilData()
//Author        : PATNI\HAR
//Purpose       : Destructor
//*****************************************************************************
CSelectionCoilData::~CSelectionCoilData(
)
{
    DEL_PTR(m_temp_selection_coil);

    // + Patni - HAR / 2009Mar13 / Added / Memory Leak
    while (m_head->next) {
        m_head = m_head->next;
        //+Patni-HAR/2010June07/Added/Memory Leak
        CoilSelectionCoil_t* l_coil = (CoilSelectionCoil_t*)m_head->prev->item;

        if (l_coil) {
            while (l_coil->section->next) {
                l_coil->section = l_coil->section->next;
                FREE_PTR(l_coil->section->prev->item);
                FREE_PTR(l_coil->section->prev);
            }

            FREE_PTR(l_coil->section->item);
            FREE_PTR(l_coil->section);
        }

        FREE_PTR(m_head->prev->item);
        FREE_PTR(m_head->prev);
    }

    FREE_PTR(m_head->item);
    FREE_PTR(m_head);
    // - Patni - HAR / 2009Mar13 / Added / Memory Leak
}

//****************************Method Header************************************
//Method Name   : CoilMaintain()
//Author        : PATNI\HAR
//Purpose       :
//*****************************************************************************
void CSelectionCoilData::CoilMaintain(
)const
{
    LPCTSTR FUNC_NAME = _T("CSelectionCoilData::CoilMaintain");
    //DB_FUNC_ENTER("[coilSelectionCoilMaintain]:");

    //+Patni-HEMANT/ADDED On 8/11/2009 5:34:50 PM/ MVC2710
    PQM_TRACE1(MR::TRACELOG::MRTraceMarker8, FUNC_NAME, _T("Trace of CSelectionCoilData::CoilMaintain"));

    CDataManager* l_data_manager = CDataManager::GetInstance();

    if (l_data_manager == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                  _T("l_data_manager is NULLL"));
        return;
    }

    CPqm* l_pqm = l_data_manager->GetPQM();

    if (l_pqm == NULL) {
        PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
                  _T("l_pqm is NULLL"));
        return;
    }

    //Patni-AMT/2009Nov06/Deleted/MSA0248-00100
    //-Patni-HEMANT/ADDED On 8/11/2009 5:34:50 PM/ MVC2710

    register	DllT*	l_dll	= NULL;
    CoilSelectionCoil_t*	l_selection_coil	= NULL;

    for (l_dll = m_head; l_dll != NULL; l_dll = l_dll->next) {

        l_selection_coil = (CoilSelectionCoil_t*)l_dll->item;
        l_selection_coil->enable = FALSE;

        // SaturnCoil
        if (l_selection_coil->port  == CDB_PORT_G ||
            l_selection_coil->port  == CDB_PORT_D) {

            if (l_selection_coil->groupId[0] == COILID_NONE) {

                l_selection_coil->enable = TRUE;
            }

            if (l_selection_coil->groupId[0] == l_selection_coil->coil_id) {

                if (CoilInstallCheckGroupCoil(l_selection_coil->groupId)) {

                    l_selection_coil->enable = TRUE;
                }
            }

            /* OldCoil */

        } else {

            l_selection_coil->enable = TRUE;
            // SFT.mode 		 -> use by QD Whole Body 4ch/8ch
            // license MRAPV2002 -> use by Peripheral Vascular coil

            //Patni-HEMANT/ADDED On 8/11/2009 5:34:50 PM/ MVC2710
            if (l_pqm->GetSFTMode() == SFT_CLINICAL) {

                if (l_selection_coil->port == CDB_PORT_A &&
                    l_selection_coil->channel != CDB_CHANNEL_C) {

                    l_selection_coil->enable = FALSE;
                }
            }
        }

        /* システムチャネル数よりMaxChannelが大きいものはリストから除外 */

        //Patni-AMT/2009Nov06/Deleted/MSA0248-00100
        if (l_pqm->GetNumChannels() < l_selection_coil->maxChannel
            && l_selection_coil->numofsec == 0) {

            l_selection_coil->enable = FALSE;
        }
    }
}

//****************************Method Header************************************
//Method Name   : GetSelectionCoilData()
//Author        : PATNI\HAR
//Purpose       :
//*****************************************************************************
CoilSelectionCoil_t* CSelectionCoilData::GetSelectionCoilData(
)
{
    if (m_temp_selection_coil) {
        return m_temp_selection_coil;

    } else {
        return NULL;
    }
}

//****************************Method Header************************************
//Method Name   : AddData()
//Author        : PATNI\HAR
//Purpose       :
//*****************************************************************************
void CSelectionCoilData::AddData(
    void* f_key,
    cdb_rec_p f_db_rec_p
)
{
    CoilSelectionCoil_t* l_selection_coil = (CoilSelectionCoil_t*)malloc(sizeof(CoilSelectionCoil_t));

    if (l_selection_coil == NULL) {
        // + Himanshu SM2
        //DB_PRINTF("",("%s:%d malloc error\n",__FILE__,__LINE__));
        //CoilSelectionCommSendControlMsg(PROMSG_COILSEL2PQM_ERROR);
        // - Himanshu SM2
        return;
    }

    memset(l_selection_coil, 0, sizeof(CoilSelectionCoil_t));
    l_selection_coil->coil_id		=	f_db_rec_p->coil_id;
    l_selection_coil->port				=	f_db_rec_p->port;
    l_selection_coil->channel			=	f_db_rec_p->channel;
    l_selection_coil->maxChannel		=	f_db_rec_p->maxChannel;
    l_selection_coil->sort_order		=	f_db_rec_p->sort_order;
    l_selection_coil->label				=	strdup(f_db_rec_p->description);
    //l_selection_coil->description		=	XmStringCreateSimple(l_selection_coil->label);
    l_selection_coil->short_name		=	strdup(f_db_rec_p->shortName);
    l_selection_coil->short_name[3]		=	NULL;
    l_selection_coil->numSCoil			=	f_db_rec_p->numSCoil;
    l_selection_coil->combine			=	f_db_rec_p->combine;


    //+Patni-Hemant/2009Mar03/Added/ACE-1/Venus Coil
    l_selection_coil->portDef			=	f_db_rec_p->portDef;

    //-Patni-Hemant/2009Mar03/Added/ACE-1/Venus Coil
    for (int i = 0; i < 4; i++) {
        l_selection_coil->groupId[i]	=	f_db_rec_p->groupId[i];
        l_selection_coil->position[i]	=	f_db_rec_p->pos4[i];
    }

    l_selection_coil->location			=	f_db_rec_p->coilLocation;

    m_temp_selection_coil = l_selection_coil;
    //	/*--------------------------debug-----------------------------*/
    //	DB_PRINTF("C",(  "----------------COIL------------------\n"));
    //	DB_PRINTF("C",(  "coil_id        =%x\n", l_selection_coil->coil_id));
    //	DB_PRINTF("C",(  "port           =%c\n", l_selection_coil->port));
    //	DB_PRINTF("C",(  "channel        =%c\n", l_selection_coil->channel));
    //	DB_PRINTF("C",(  "maxChannel     =%d\n", l_selection_coil->maxChannel));
    //	DB_PRINTF("C",(  "sort_order     =%d\n", l_selection_coil->sort_order));
    //	DB_PRINTF("C",(  "label          =%s\n", l_selection_coil->label));
    //	DB_PRINTF("C",(  "short_name     =%s\n", l_selection_coil->short_name));
    //	DB_PRINTF("C",(  "numSCoil       =%x\n", l_selection_coil->numSCoil));
    //	DB_PRINTF("C",(  "combine        =%x\n", l_selection_coil->combine));
    //	for(i = 0; i < 4; i++){
    //		DB_PRINTF("C",(  "groupId[%d]    =%x\n", i, l_selection_coil->groupId[i]));
    //		DB_PRINTF("C",(  "position[%d] =%x\n", i, l_selection_coil->position[i]));
    //	}
    //	DB_PRINTF("C",(  "location       =%x\n", l_selection_coil->location));
    //	DB_PRINTF("C",(  "sectionDB      =%x\n", db_rec_p->sectionDB));
    //	DB_PRINTF("C",(  "--------------------------------------\n"));
    //	/*------------------------------------------------------------*/
    //

    if (f_db_rec_p->sectionDB == 1 &&
        f_db_rec_p->groupId[0] == COILID_NONE ||
        f_db_rec_p->groupId[0] == f_db_rec_p->coil_id) {

        //In SectionDB
        CDataManager::GetInstance()->ReadSelectionSectionDB(0,
                SECTION_DB_KEY_INSTALLED, 	SEDB_ON,
                SECTION_DB_KEY_ID, 			l_selection_coil->coil_id,
                NULL);

        //Information Group
        if (l_selection_coil->groupId[0] != COILID_NONE) {
            for (i = 1; i < 4; i++) {

                if (l_selection_coil->groupId[i] == COILID_NONE) {
                    break;
                }

                CDataManager::GetInstance()->ReadSelectionSectionDB(0,
                        SECTION_DB_KEY_INSTALLED,   SEDB_ON,
                        // Need to send group id from the group and not coil id
                        // SECTION_DB_KEY_ID,      l_selection_coil->coil_id,
                        SECTION_DB_KEY_ID,      l_selection_coil->groupId[i],
                        NULL
                                                                   );
            }
        }

    } else {
        // None SectionDB
        l_selection_coil->numofsec = 0;
    }

    //	DB_PRINTF("C",("COIL: %x PORT: (%c)%d\n",
    //		l_selection_coil->coil_id,
    //		l_selection_coil->port,l_selection_coil->port));

    if (m_head == NULL) {
        dllAppendNode(m_head, l_selection_coil, l_selection_coil->sort_order,
                      &m_head);

    } else {
        DllT* l_next = NULL;
        register DllT*		l_dll				= NULL;

        for (l_dll = m_head; l_dll != NULL; l_dll = l_dll->next) {
            if (l_dll->type > l_selection_coil->sort_order) {

                l_next = l_dll;
                break;
            }
        }

        if (l_next == NULL) {
            dllAppendNode(m_head, l_selection_coil, l_selection_coil->sort_order,
                          &m_head);

        } else {
            dllInsertBeforeNode(l_next, l_selection_coil, l_selection_coil->sort_order,
                                &m_head);
        }
    }

    DllFindHead();
}

//****************************Method Header************************************
//Method Name   : CoilInstallCheckGroupCoil()
//Author        : PATNI\HAR
//Purpose       :
//*****************************************************************************
BOOL CSelectionCoilData::CoilInstallCheckGroupCoil(
    unsigned short* f_group_id
)const
{
    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (f_group_id == NULL) {
        return FALSE;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    register	DllT*	dll = NULL; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
    CoilSelectionCoil_t*	coil_p = NULL; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
    bool_t				enable = 0; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections

    for (int i = 1; i < 4; i++) {

        if (f_group_id[i] == COILID_NONE) {
            return TRUE;
        }

        enable = FALSE;

        for (dll = m_head; dll != NULL; dll = dll->next) {

            coil_p = (CoilSelectionCoil_t*)dll->item;

            if (f_group_id[i] == coil_p->coil_id) {
                enable = TRUE;
            }
        }

        if (!enable) {
            return FALSE;
        }
    }

    return TRUE;
}

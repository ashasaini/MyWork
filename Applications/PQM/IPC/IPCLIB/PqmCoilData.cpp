//REDMINE-1213
//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-779
//Redmine-780
//Redmine-781
//Redmine-782
// PqmCoilData.cpp: implementation of the CPqmCoilData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PqmCoilData.h"
#include <tami/libAcq/AcqCoilSignal.h>
#include <PQM/PQMLogMgr.h>
#include <PQM/pqmcommon.h>
#include "Pqm.h"
#include "pqmstudy.h"
#include "PqmSectionData.h"
#include "PqmPathData.h"
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"
#include "Coil.h"       // V9.25 coil Support@AMT

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPqmCoilData::CPqmCoilData():
    m_wb_coil(NULL)
{
}

CPqmCoilData::~CPqmCoilData()
{
    DEL_PTR(m_wb_coil);

    // + Patni - HAR / 2009Mar13 / Added / Memory Leak
    while (m_head->next) {
        m_head = m_head->next;
        FREE_PTR(m_head->prev->item);
        FREE_PTR(m_head->prev);
    }

    FREE_PTR(m_head->item);
    FREE_PTR(m_head);
    // - Patni - HAR / 2009Mar13 / Added / Memory Leak

}

void CPqmCoilData::AddData(
    void* f_key,
    cdb_rec_p f_db_rec_p
)
{
    PqmCoil_t*		l_coil_p = (PqmCoil_t*)malloc(sizeof(PqmCoil_t));

    if (l_coil_p == NULL) {
#ifdef NEED_MODI
        DB_PRINTF("", ("%s:%d malloc error\n", __FILE__, __LINE__));
        PqmUiSafeExit(PQM_FATAL);
#endif
        return;
    }

    memset(l_coil_p, 0, sizeof(PqmCoil_t));
    l_coil_p->coil_id		= (int)f_key;
    l_coil_p->type			= f_db_rec_p->type;
    l_coil_p->port			= f_db_rec_p->port;
    l_coil_p->channel		= f_db_rec_p->channel;
    l_coil_p->sort_order	= f_db_rec_p->sort_order;
    l_coil_p->label			= strdup(f_db_rec_p->description);

    //l_coil_p->description	= XmStringCreateSimple(coil_p->label);

    // LibAcq AMT not required as its XmString in libAcq\AcqCoilSignal.h
    //l_coil_p->description   = strdup(f_db_rec_p->description);
    l_coil_p->outSideLimit	= f_db_rec_p->outSideLimit;
    l_coil_p->inSideLimit	= f_db_rec_p->inSideLimit;
    l_coil_p->tuneLimit		= f_db_rec_p->tuneLimit;
    l_coil_p->rflLimit		= f_db_rec_p->rflLimit;
    l_coil_p->mview			= f_db_rec_p->mview;
    l_coil_p->receiveWB_enable	 = f_db_rec_p->receiveWB_enable;
    l_coil_p->speeder_enable	 = f_db_rec_p->speeder_enable;

    // V9.00
    l_coil_p->combine		= f_db_rec_p->combine;
    l_coil_p->numSCoil		= f_db_rec_p->numSCoil;
    l_coil_p->portDef		= f_db_rec_p->portDef;

    l_coil_p->warPosZ		= f_db_rec_p->warPosZ;
    l_coil_p->coilDetect	= f_db_rec_p->coilDetect;
    l_coil_p->sectionDB		= f_db_rec_p->sectionDB;
    l_coil_p->movingBed		= f_db_rec_p->movingBed;
    l_coil_p->maxChannel	= f_db_rec_p->maxChannel;

    strcpy(l_coil_p->shortName, f_db_rec_p->shortName);

    for (int i = 0; i < 4; i++) {
        l_coil_p->groupId[i] =	f_db_rec_p->groupId[i];
    }

    l_coil_p->patient_orient = f_db_rec_p->patient_orient;

    //Patni-ARD/20090226/Added/ACE-1# Enhance the withstand load of couch Start
    if (f_db_rec_p->weight <= PQM_MAX_COIL_WEIGHT) {
        l_coil_p->weight			= f_db_rec_p->weight;

    } else {
        l_coil_p->weight			= 0;
    }

    //Patni-ARD/20090226/Added/ACE-1# Enhance the withstand load of couch End
    /** For 50 & 200 **/
    //+Patni-HAR/2010Feb10/Modified/CDS Requirement

    l_coil_p->size = f_db_rec_p->size;
    //l_coil_p->size = 505;

    l_coil_p->coilLocation = f_db_rec_p->coilLocation;
    //-Patni-HAR/2010Feb10/Modified/CDS Requirement
    cdb_coil_port_t	l_port_type = CDB_PORT_A;	//Patni-MJC/2009Aug17/Modified/cpp test corrections

    l_port_type = GetWholeBodyPort();

    if (l_coil_p->port == l_port_type && l_coil_p->channel == CDB_CHANNEL_C) {
        // PqmGlobal.wbCoil = l_coil_p;
        m_wb_coil = l_coil_p;
        l_coil_p->wholeBody = TRUE;
    }

#ifdef MODI
    DB_PRINTF("c", ("COIL: %x PORT: (%c)%d - (%c)%d\n",
                    coil_p->coil_id,
                    coil_p->port, coil_p->port,
                    port_type, port_type));
#endif

    if (m_head == NULL)
        dllAppendNode(m_head, l_coil_p, l_coil_p->sort_order, &m_head);

    else {
        DllT*	next = NULL;	//Patni-MJC/2009Aug17/Modified/cpp test corrections
        register DllT*	l_dll = NULL;

        for (l_dll = m_head; l_dll != NULL; l_dll = l_dll->next) {

            if (l_dll->type > l_coil_p->sort_order) {

                next = l_dll;
                break;
            }
        }

        if (next == NULL)
            dllAppendNode(m_head, l_coil_p, l_coil_p->sort_order, &m_head);

        else
            dllInsertBeforeNode(next, l_coil_p, l_coil_p->sort_order, &m_head);
    }

    DllFindHead();
}

//****************************Method Header****************************
// Method Name  :CoilMaintain
// Author       :PATNI/
// Purpose      :
//*********************************************************************
void CPqmCoilData::CoilMaintain(
)
{
    LPCTSTR FUNC_NAME = _T("CPqmCoilData::CoilMaintain");
    // Get Value of Is Saturn System from CPQM Class
    CPqm* l_pqm = CDataManager::GetInstance()->GetPQM();

    //+Patni-RUP/2009Aug21/Modified/cpp test corrections
    if (l_pqm == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqm pointer is NULL"));
        return;
    }

    //-Patni-RUP/2009Aug21/Modified/cpp test corrections

    DllT*        	l_dll = NULL ;
    PqmCoil_t*		l_coil_p = NULL;

    for (l_dll = m_head; l_dll != NULL; l_dll = l_dll->next) {
        l_coil_p = (PqmCoil_t*)l_dll->item;

        l_coil_p->enable = FALSE;

        if (l_pqm->IsSaturnCoilSystem()  && l_coil_p->mview & 0xffffffff) {  /* New RFL Coil */

            l_coil_p->elm_pattern[0] = 0 ;
            l_coil_p->elm_pattern[1] = 0 ;
            l_coil_p->channel_mask = 0xffffffff;
            l_coil_p->num_channels = l_coil_p->maxChannel;
            l_coil_p->pac_pattern = 0 ;
            l_coil_p->enable = TRUE;

        } else if (l_coil_p->mview & 0x80000000) {
            /*
            DllT *l_path_head = m_db[PATH_DB]->GetHead();
            l_node = dllNodeType(l_path_head, (long)l_coil_p->mview & COIL_PHYSICALID_MASK);

            if(NULL == l_node )
            {
                continue;
            }

            l_coil_p->channel_mask = l_coil_p->mview & 0x7FFF0000;
            l_coil_p->channel_mask >>= 16;
            if(l_coil_p->channel_mask == 0)
            {
                l_coil_p->channel_mask = 0x7FFF;
            }


            l_coil_p->elm_pattern[1] = ((path_db_rec_p)l_node->item)->path[0];
            l_coil_p->elm_pattern[0] = ((path_db_rec_p)l_node->item)->path[1];
            shift = 0;
            bits = 0;
            for(i = 0,j = 0;i < PQM_NUM_COIL_ELEMENT;i++,shift += 4)
            {
                if ( shift == 32 )
                {
                    shift = 0 ;
                    j = 1 ;
                }
                id = (l_coil_p->elm_pattern[j] & (unsigned int)(0x0F << shift)) >> shift;
                if(id > 0 && id < (sizeof(bits) * 8) ) bits |= (1<<(id-1));
            }

            l_coil_p->num_channels = 0;
            shift = 1;
            l_coil_p->pac_pattern = 0;

            for(i = 0;i < PQM_NUM_COIL_ELEMENT;i++)
            {
                if(bits & shift)
                {
                    l_coil_p->num_channels++;
                    l_coil_p->pac_pattern |= (i+1) << (i*4);
                }
                shift <<= 1;
            }


            /*
            if(!(l_coil_p->channel_mask & PqmGlobal.channel_mask))
                l_coil_p->enable = FALSE;
            else
                l_coil_p->enable = TRUE;
            */
            //l_coil_p->enable = TRUE;    // temp definition

        }

        /*
                DB_PRINTF("C",("%s [0x%x]: %x <> %x  pac_pattern=0x%x\n",
                    coil_p->label,coil_p->coil_id,coil_p->channel_mask,
                    PqmGlobal.channel_mask,coil_p->pac_pattern));
        */
        /* V5.30
        SFT.mode          -> use by QD Whole Body 4ch/8ch
        license MRAPV2002 -> use by Peripheral Vascular coil
        */

        //+Patni-HEMANT/ADDED On 8/11/2009 2:11:50 PM/ MVC2710
        //As per Amit san,Following lines are uncommented for sft mdoe functionality
        if (l_pqm && l_pqm->GetSFTMode() == SFT_CLINICAL) {
            cdb_coil_port_t	port_type = CDB_PORT_A;		//Patni-MJC/2009Aug17/Modified/cpp test corrections

            // For 50 & 200
            if (GP_MAGNET(l_pqm->GetSysType()) == GP_MAGNET_50) {
                port_type = CDB_PORT_B;

            } else {
                port_type = CDB_PORT_A;
            }

            if (l_coil_p->port == port_type && l_coil_p->channel != CDB_CHANNEL_C) {
                l_coil_p->enable = FALSE;
            }
        }

        //-Patni-HEMANT/ADDED On 8/11/2009 2:11:50 PM/ MVC2710

        /*
                if(!l_is_saturn)
                {

                    for ( j = 0 ; j < numPVcoil ; j++ )
                    {
                        if( (IS_COIL_SAME( l_coil_p->coil_id , PVcoilTbl[j]) )&&
                            ( PqmGlobal.MRAPV2002_License == FALSE ) )
                        {
                            l_coil_p->enable = FALSE;
                            break ;
                        }
                    }

                    if(l_coil_p->num_channels > 1 &&
                        (l_node = dllNodeType(gainHead,l_coil_p->coil_id)) != NULL)

                    {
                        int	cnt[PQM_NUM_VFEP_CHANNEL];

                        l_coil_p->gain_enable = TRUE;
                        shift = 0;
                        for(i = 0;i < PQM_NUM_VFEP_CHANNEL;i++)
                        {
                            l_coil_p->gain_data[i] = 0.0;
                            cnt[i] = 0;
                        }
                        for(i = 0, j = 0 ;i < PQM_NUM_COIL_ELEMENT;i++,shift += 4)
                        {
                            if ( shift == 32 )
                            {
                                shift = 0 ; j = 1;
                            }
                            id = (unsigned)(l_coil_p->elm_pattern[j] & (0x0F << shift)) >> shift;

                            if(id > 0 && id <= PQM_NUM_VFEP_CHANNEL )
                            {
                                l_coil_p->gain_data[id-1] +=
                                    ((PqmCoilGain_p)l_node->item)->data[i];
                                cnt[id-1]++;
                            }
                        }
                        for(i = 0;i < PQM_NUM_VFEP_CHANNEL;i++)
                        {
                            if(l_coil_p->gain_data[i] == 0.0) l_coil_p->gain_data[i] = 1.0;
                            else if(cnt[i] > 1) l_coil_p->gain_data[i] /= (flt32_t)cnt[i];
                        }
                    }
                    else
                    {
                        l_coil_p->gain_enable = FALSE;
                        for(i = 0;i < PQM_NUM_VFEP_CHANNEL;i++)
                        {
                            l_coil_p->gain_data[i] = 1.0;
                        }
                    }
                } //PqmIsSaturnCoilSystem
        */
    }
}

//***************************Method Header*************************************
//Method Name    : PqmIsSameCoilInSaturn()
//Author         : PATNI/AMT
//Purpose        : Checks if Saturn coil a and Saturn Coil B are same or not.
//*****************************************************************************
status_t CPqmCoilData::PqmIsSameCoilInSaturn(
    PqmSaturnCoil_t* f_coil_a,
    PqmSaturnCoil_t* f_coil_b,
    PqmSaturnRcvCoil_t* const  f_tbl_a,
    PqmSaturnRcvCoil_t* const f_tbl_b
)
{
    LPCTSTR FUNC_NAME = _T("CPqmCoilData::PqmIsSameCoilInSaturn");

    if ((f_coil_b == NULL) || (f_coil_a == NULL) || (f_tbl_a == NULL) || (f_tbl_b == NULL))
        return FALSE;

    //	DebugBreak();
    if (f_tbl_a->numCoil == f_tbl_b->numCoil &&  f_tbl_a->numCoil == 0) {
        return TRUE;
    }

    int			l_num_a = 0, l_num_b = 0;	//Patni-MJC/2009Aug17/Modified/cpp test corrections

    l_num_a = f_coil_a->CoilList.num;
    l_num_b = f_coil_b->CoilList.num;

    for (int i = 0 ; i < f_coil_a->CoilList.num ; i++) {
        if (f_coil_a->CoilList.coillist[i].coilID == WBCOIL_PHYSICAL_ID) {
            l_num_a--;
        }
    }

    for (i = 0 ; i < f_coil_b->CoilList.num ; i++) {
        if (f_coil_b->CoilList.coillist[i].coilID == WBCOIL_PHYSICAL_ID) {
            l_num_b--;
        }
    }

    if (l_num_a != l_num_b) {
        return FALSE;
    }

    int j = 0, k = 0;

    for (i = 0 ; i < f_coil_a->CoilList.num ; i++) {
        for (j = 0 ; j < f_coil_b->CoilList.num ; j++) {
            /* WBは特別なので、CoilListの中に存在してもチェックしない */
            if (f_coil_a->CoilList.coillist[i].coilID ==
                WBCOIL_PHYSICAL_ID && f_coil_a->CoilList.num > 1) {
                break;
            }

            if (f_coil_a->CoilList.coillist[i].coilID == f_coil_b->CoilList.coillist[j].coilID
                && f_coil_a->CoilList.coillist[i].portID[0] == f_coil_b->CoilList.coillist[j].portID[0]) {

                CCoil       l_tmp_coil;
                PqmCoil_t*	l_coil_p = NULL;
                PqmCoilidGetSaturnCoil(
                    f_coil_a->CoilList.coillist[i].coilID,
                    f_coil_a->CoilList.coillist[i].portID[0], NULL, l_tmp_coil);

                if (l_tmp_coil.GetCoil() == NULL) {

                    break;
                }

                l_coil_p = l_tmp_coil.GetCoil();

                if (l_coil_p->numSCoil == 1) {
                    break;

                } else {
                    Boolean	gFlg = FALSE;

                    for (k = 0 ; k < f_coil_a->CoilList.num ; k++) {
                        if (i != k && f_coil_a->CoilList.coillist[i].coilID ==
                            f_coil_a->CoilList.coillist[k].coilID) {

                            gFlg = TRUE;
                        }
                    }

                    for (k = 0 ; k < f_coil_b->CoilList.num ; k++) {
                        if (j != k && f_coil_b->CoilList.coillist[j].coilID ==
                            f_coil_b->CoilList.coillist[k].coilID) {

                            gFlg = TRUE;
                        }
                    }

                    if (gFlg) {
                        if (f_coil_a->CoilList.coillist[i].portID[1] ==
                            f_coil_b->CoilList.coillist[j].portID[1]) {

                            break;
                        }

                        if (f_coil_a->CoilList.coillist[i].portID[1] ==
                            f_coil_b->CoilList.coillist[j].portID[1]) {

                            break;
                        }

                    } else {
                        break;
                    }
                }
            }
        }

        if (j >= f_coil_b->CoilList.num) {
            return FALSE;
        }
    }

    CPqm* l_pqm = CDataManager::GetInstance()->GetPQM();

    //+Patni-RUP/2009Aug21/Modified/cpp test corrections
    if (l_pqm == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqm pointer is NULL"));
        return FALSE;
    }

    //-Patni-RUP/2009Aug21/Modified/cpp test corrections

    PqmCoil_t*	l_tx_coil_a = NULL, *l_tx_coil_b = NULL;

    if (!l_pqm ->GetStudy()->GetTxCoilInSaturn(f_tbl_a, &l_tx_coil_a)) {
        return FALSE;
    }

    if (!l_pqm ->GetStudy()->GetTxCoilInSaturn(f_tbl_b, &l_tx_coil_b)) {
        return FALSE;
    }

    if (l_tx_coil_a != l_tx_coil_b) {
        return FALSE;
    }

    return TRUE ;
}

//+Patni-Hemant/2010Feb5/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : PqmIsSameCoilInCDS
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CPqmCoilData::PqmIsSameCoilInCDS(
    PqmSaturnCoil_t* f_coil_cur,
    PqmSaturnCoil_t* f_coil_last,
    PqmSaturnRcvCoil_t* const f_tbl_cur,
    PqmSaturnRcvCoil_t* const f_tbl_last
) const  	//Patni-Hemant/2010May6/Added/JaFT/SM_PH3_CDR-CDS_CDR_19
{

    //+Patni-Hemant/2010May6/Commented/JaFT/SM_PH3_CDR-CDS_CDR_19
    //int i = 0, j = 0;
    //int num_a = 0, num_b =0;
    //-Patni-Hemant/2010May6/Commented/JaFT/SM_PH3_CDR-CDS_CDR_19

    if (f_tbl_cur->numCoil == f_tbl_last->numCoil &&  f_tbl_cur->numCoil == 0) {
        return true;
    }

    //+Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_19
    int num_a = f_coil_cur->CoilList.num;
    int num_b = f_coil_last->CoilList.num;
    //-Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_19

    //Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_19
    for (int i = 0 ; i < f_coil_cur->CoilList.num ; i++) {
        if (f_coil_cur->CoilList.coillist[i].coilID == WBCOIL_PHYSICAL_ID) {
            num_a--;
        }
    }

    for (i = 0 ; i < f_coil_last->CoilList.num ; i++) {
        if (f_coil_last->CoilList.coillist[i].coilID == WBCOIL_PHYSICAL_ID) {
            num_b--;
        }
    }

    /* WB, except when a different number of coils connected */
    if (num_a != num_b)	{
        return false;
    }

    for (i = 0 ; i < f_coil_cur->CoilList.num ; i++) {
        //Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_19
        for (int j = 0 ; j < f_coil_last->CoilList.num ; j++) {
            /* WB is so special, CoilList not even exist in the check */
            if (f_coil_cur->CoilList.coillist[i].coilID == WBCOIL_PHYSICAL_ID && f_coil_cur->CoilList.num > 1) {
                break;
            }

            if (f_coil_cur->CoilList.coillist[i].coilID == f_coil_last->CoilList.coillist[j].coilID
                && f_coil_cur->CoilList.coillist[i].portID[0] == f_coil_last->CoilList.coillist[j].portID[0]
                && f_coil_cur->CoilList.coillist[i].portID[1] == f_coil_last->CoilList.coillist[j].portID[1]) {
                break;
            }
        }

        if (j >= f_coil_last->CoilList.num) {
            return false;
        }
    }

    CPqm* l_pqm = CDataManager::GetInstance()->GetPQM();

    /* Error sending coil to be equal */
    PqmCoil_t* txcoil_a = NULL, *txcoil_b = NULL;

    if (!l_pqm ->GetStudy()->GetTxCoilInSaturn(f_tbl_cur, &txcoil_a)) {
        return false;
    }

    if (!l_pqm ->GetStudy()->GetTxCoilInSaturn(f_tbl_last, &txcoil_b)) {
        return false;
    }

    if (txcoil_a != txcoil_b) {
        return false;
    }

    return true ;
}
//-Patni-Hemant/2010Feb5/Added/ACE-Phase#3/CDS Requirement

//***************************Method Header*************************************
//Method Name    : PqmInstallCheckSaturn()
//Author         : PATNI/AMT
//Purpose        : This function checks coil list is installed or not.
//*****************************************************************************
status_t CPqmCoilData::PqmInstallCheckSaturn(
    VftCoilListInfo_t* const  coillist
)const
{
    LPCTSTR FUNC_NAME = _T("CPqmCoilData::PqmInstallCheckSaturn");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (coillist == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("coillist pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    CCoil l_coil ;

    for (int i = 0; i < coillist->num; i++) {

        PqmCoilidGetSaturnCoil(coillist->coillist[i].coilID,
                               coillist->coillist[i].portID[0], NULL, l_coil);

        if (l_coil.GetCoil() == NULL) {
            return E_ERROR ;
        }
    }

    return  E_NO_ERROR ;
}

//***************************Method Header*************************************
//Method Name    : PqmCoilIsDegeneracy()
//Author         : PATNI/AMT
//Purpose        :
//*****************************************************************************
bool_t CPqmCoilData::PqmCoilIsDegeneracy(
    const int               f_num_Section,	 // Selected element number
    VftSectionRecvrCoil_t	f_recvr_Coil[]	 // Selected coil
)const
{
    LPCTSTR FUNC_NAME = _T("CPqmCoilData::PqmCoilIsDegeneracy");
    sedb_rec_t*  l_sec_p[PQM_NUM_COIL_ELEMENT] = {0}; // Acquire section information
    int			l_sec_cnt = 0;						  // Section information count//Patni-MJC/2009Aug17/Modified/cpp test corrections
    int			i = 0;	//Patni-MJC/2009Aug17/Modified/cpp test corrections

    //DB_FUNC_ENTER("[PqmCoilIsDegeneracy]:");

    for (i = 0; i < f_num_Section; i++) {
        if (E_NO_ERROR != PqmCoilGetSaturnSection(&f_recvr_Coil[i], &l_sec_cnt, l_sec_p)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PqmCoilGetSaturnSection failed"));
        }

        if (l_sec_cnt >  0) {
            if (l_sec_p[0]->minChannel != l_sec_p[0]->maxChannel) {
                return TRUE;
            }
        }
    }

    return FALSE;
}

status_t CPqmCoilData::PqmIsSameCoilInSaturnforSPEEDER(PqmSaturnCoil_t* coil_a,
        PqmSaturnCoil_t* coil_b)const
{
    if (coil_a->numSection != coil_b->numSection)
        return FALSE ;

    int n = sizeof(VftSectionRecvrCoil_t) ;

    for (int i = 0 ; i < coil_a->numSection ; i++) {
        //+IR-193/Signal num check added
        if (coil_a->Signal[i].num != coil_b->Signal[i].num)
            return FALSE;


        if (memcmp(&coil_a->RcvCoil[i], &coil_b->RcvCoil[i], n))
            return FALSE ;
    }

    VftSectionSignal_t*	signal_a = NULL, *signal_b = NULL;

    /* 縮退情報チェック */
    if (coil_a->DegeneracyFlg == TRUE && coil_b->DegeneracyFlg == TRUE) {/* MAP：ON 本撮像：ON */
        signal_a = coil_a->SignalDegeneracy;
        signal_b = coil_b->SignalDegeneracy;

    } else if (coil_a->DegeneracyFlg == FALSE && coil_b->DegeneracyFlg == TRUE) {/* MAP：OFF 本撮像：ON */
        signal_a = coil_a->SignalDegeneracy;
        signal_b = coil_b->Signal;

    } else if (coil_a->DegeneracyFlg == TRUE && coil_b->DegeneracyFlg == FALSE) {/* MAP：ON 本撮像：OFF */
        return TRUE;

    } else {/* MAP：OFF 本撮像：OFF */
        return TRUE;
    }

    n = sizeof(VftSectionSignal_t) ;

    for (i = 0 ; i < coil_a->numSection ; i++) {
        if (memcmp(&signal_a[i], &signal_b[i], n))
            return FALSE ;
    }


    return TRUE ;
}

//+Patni-Hemant/2010Feb10/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : PqmCoilGetSaturnSectionMax
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
status_t CPqmCoilData::PqmCoilGetSaturnSectionMax(
    VftSectionRecvrCoil_t*	 f_recvrCoil,		/* Coil Information */
    int* max_sec)		/* SectionID up */
{
    LPCTSTR FUNC_NAME = _T("CPqmCoilData::PqmCoilGetSaturnSectionMax");

    //DB_FUNC_ENTER("[PqmCoilGetSaturnSection]:");
    if (f_recvrCoil == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_recvrCoil pointer is NULL"));
        return E_ERROR;
    }

    CDataManager* l_data_manager = CDataManager::GetInstance();

    if (l_data_manager == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_data_manager pointer is NULL"));
        return E_ERROR;
    }

    *max_sec = 0;

    if (f_recvrCoil == NULL || max_sec == NULL) {
        return E_ERROR;
    }

    //	if((coil_p = PqmCoilGetSaturnCoil(f_recvrCoil)) == NULL)
    //	{
    //		return E_ERROR;
    //	}
    CCoil l_coil ;

    if (!PqmCoilGetSaturnCoil(f_recvrCoil, l_coil)) {

        return E_ERROR;
    }

    PqmCoil_t*		coil_p = l_coil.GetCoil();

    if ((coil_p == NULL) || coil_p->sectionDB == 0) {
        return E_NO_ERROR;
    }

    DllT* sectionHead = l_data_manager->GetPQMSectionData()->GetHead();

    register DllT*	dll = NULL;
    sedb_rec_t*		sec_rec_p = NULL;
    int				max = 0;

    for (dll = sectionHead; dll != NULL; dll = dll->next) {
        sec_rec_p = (sedb_rec_t*)dll->item;

        if (sec_rec_p->coil_id    == f_recvrCoil->coilID
            &&  sec_rec_p->port       == f_recvrCoil->portID[0]) {
            if (max < sec_rec_p->section_id) {
                max = sec_rec_p->section_id;
            }
        }
    }

    *max_sec = max;

    if (*max_sec == 0) {
        return E_ERROR;
    }

    //DB_FUNC_EXIT();

    return E_NO_ERROR;
}
//-Patni-Hemant/2010Feb10/Added/ACE-Phase#3/CDS Requirement

//+Patni-Hemant/2010Feb10/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : PqmGetCDSRWBInSaturn
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
PqmCoil_t* CPqmCoilData::PqmGetCDSRWBInSaturn(
    VftCoilListInfo_t* const  f_coillist,
    int f_mask
)
{
    LPCTSTR FUNC_NAME = _T("CPqmCoilData::PqmGetCDSRWBInSaturn");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    if (!f_coillist) {
        return NULL ;
    }

    int	rwb_enable = -1, wb_type = 0;
    PqmCoil_t* p = NULL;

    CCoil l_satrun_coil;

    for (int i = 0; i < f_coillist->num; i++) {
        PqmCoilidGetSaturnCoil(f_coillist->coillist[i].coilID, f_coillist->coillist[i].portID[0], NULL, l_satrun_coil);
        p = l_satrun_coil.GetCoil();

        //Patni-Hemant/2010Mar3/Added/ACE-Phase#3/CDS Requirement
        //Review comment - Sarjeet san
        if (!p) {
            continue ;
        }

        if ((p != m_wb_coil) && (rwb_enable == -1)) {
            rwb_enable = p->receiveWB_enable ;
            wb_type = rwb_enable & SATURN_RWB_COIL ;
            rwb_enable &= f_mask ;

        } else if (p != m_wb_coil) {
            if (rwb_enable != (p->receiveWB_enable & f_mask) &&
                wb_type != (p->receiveWB_enable & SATURN_RWB_COIL)) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("Returning NULL on Exiting CPqmCoilData::PqmGetCDSRWBInSaturn"));

                return NULL ;
            }
        }
    }

    if (wb_type == SATURN_RWB_COIL && rwb_enable == f_mask) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Sending WB coil on Exiting CPqmCoilData::PqmGetCDSRWBInSaturn"));

        return m_wb_coil;
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("Returning NULL on Exiting CPqmCoilData::PqmGetCDSRWBInSaturn"));

    return NULL;
}
//-Patni-Hemant/2010Feb10/Added/ACE-Phase#3/CDS Requirement

//***************************Method Header*************************************
//Method Name    : GetWBCoil()
//Author         : PATNI/AMT
//Purpose        : Return whole body coil
//*****************************************************************************
bool CPqmCoilData::GetWBCoil(
    CCoil& f_wb_coil
)
{
    f_wb_coil.SetCoil(m_wb_coil) ;
    return true;
}

/***************************Method Header*************************************/
//Method Name    : PqmCoilGetCoil()
//Author         : PATNI/AMT
//Purpose        : This function returns coil based on id specified by its
//                  argument
//*****************************************************************************/
bool CPqmCoilData::PqmCoilGetCoil(
    const int f_coil_id,
    CCoil& f_coil
)const
{
    register DllT*	l_dll    = NULL;
    PqmCoil_t*		l_coil_p = NULL;

    for (l_dll = m_head; l_dll != NULL; l_dll = l_dll->next) {

        l_coil_p = (PqmCoil_t*)l_dll->item;

        if (l_coil_p->coil_id == f_coil_id) {
            f_coil.SetCoil(l_coil_p);
            return true;
        }
    }

    f_coil.SetCoil(NULL);
    return false;
}

/***************************Method Header*************************************/
//Method Name    : PqmCoilGetCoilGr()
//Author         : PATNI/AMT
//Purpose        : This function returns coilgroup based on id specified by its
//                  argument
//*****************************************************************************/
bool CPqmCoilData::PqmCoilGetCoilGr(
    const int f_coil_id,
    CCoil& f_coil
)const
{
    register DllT*   l_dll      = NULL;
    PqmCoil_t*		l_coil_p   = NULL;

    for (l_dll = m_head; l_dll != NULL; l_dll = l_dll->next) {

        l_coil_p = (PqmCoil_t*) l_dll->item;

        if (IS_COIL_SAME(l_coil_p->coil_id , f_coil_id)) {
            f_coil.SetCoil(l_coil_p);
            return true;
        }
    }

    f_coil.SetCoil(NULL);
    return false;
}

/***************************Method Header*************************************/
//Method Name    : PqmCoilGetCoilRWB()
//Author         : PATNI/AMT
//Purpose        : This function returns receiver whole body coil based on
//                  specified by its argument
//*****************************************************************************/
bool CPqmCoilData::PqmCoilGetCoilRWB(
    const int f_coil_id,
    CCoil& f_coil
)const
{
    register DllT*   l_dll      = NULL;
    PqmCoil_t*		l_coil_p   = NULL;

    for (l_dll = m_head; l_dll != NULL; l_dll = l_dll->next) {

        l_coil_p = (PqmCoil_t*) l_dll->item;

        if (IS_COIL_SAME(l_coil_p->coil_id , f_coil_id)) {

            if (l_coil_p->receiveWB_enable) {
                f_coil.SetCoil(l_coil_p) ;
                return true;
            }
        }
    }

    f_coil.SetCoil(NULL);
    return false;
}

/***************************Method Header*************************************/
//Method Name    : PqmCoilGetEnableCoil()
//Author         : PATNI/AMT
//Purpose        : This function returns enable coil based on specified by
//                  its argument
//*****************************************************************************/
bool CPqmCoilData::PqmCoilGetEnableCoil(
    const int f_coil_id,
    CCoil& f_coil
)const
{
    register DllT*   l_dll      = NULL;
    PqmCoil_t*		l_coil_p   = NULL;

    for (l_dll = m_head; l_dll != NULL; l_dll = l_dll->next) {

        l_coil_p = (PqmCoil_t*) l_dll->item;

        if (IS_COIL_SAME(l_coil_p->coil_id, f_coil_id) && l_coil_p->enable) {
            f_coil.SetCoil(l_coil_p);
            return true;;
        }
    }

    f_coil.SetCoil(NULL);
    return false;
}

/***************************Method Header*************************************/
//Method Name    : PqmCoilGetTxCoil()
//Author         : PATNI/AMT
//Purpose        : This function returns transmiter coil based on coil id
//                  specified by its argument
//*****************************************************************************/
bool CPqmCoilData::PqmCoilGetTxCoil(
    const int f_rcvCoil_id,
    CCoil& f_coil
)const
{
    register DllT*   l_dll      = NULL;
    PqmCoil_t*		l_coil_p   = NULL;

    for (l_dll = m_head; l_dll != NULL; l_dll = l_dll->next) {

        l_coil_p = (PqmCoil_t*) l_dll->item;

        if (l_coil_p->coil_id == f_rcvCoil_id) {

            if (l_coil_p->type == CDB_TRANSMIT_N_RCV) {
                f_coil.SetCoil(l_coil_p) ;
                return true;

            } else {
                // return PqmGlobal.wbCoil;
                f_coil.SetCoil(m_wb_coil) ;
                return true;
            }
        }
    }

    f_coil.SetCoil(NULL);
    return false;
}

//*****************************************************************************
//                      Saturn Coil Related Functions.
//*****************************************************************************

/***************************Method Header*************************************/
//Method Name    : PqmCoilGetSaturnCoil()
//Author         : PATNI/AMT
//Purpose        : This function returns Saturn coil based on coil id
//                  specified by its argument
//*****************************************************************************/
bool CPqmCoilData::PqmCoilGetSaturnCoil(
    VftSectionRecvrCoil_t*	const f_coil,
    CCoil& f_satrun_coil
)const
{
    register DllT*   l_dll      = NULL;
    PqmCoil_t*		l_coil_p   = NULL;

    for (l_dll = m_head; l_dll != NULL; l_dll = l_dll->next) {

        l_coil_p = (PqmCoil_t*) l_dll->item;

        if (IS_COIL_PHYSICAL_SAME(l_coil_p->coil_id, f_coil->coilID) &&
            (l_coil_p->port == f_coil->portID[0])) {

            if (l_coil_p->channel == NULL) {
                f_satrun_coil.SetCoil(l_coil_p);
                return true;

            } else if (l_coil_p->channel == f_coil->channel) {
                f_satrun_coil.SetCoil(l_coil_p);
                return true;
            }
        }
    }

    f_satrun_coil.SetCoil(NULL);
    return false;
}

/***************************Method Header*************************************/
//Method Name    : PqmCoilGetSaturnCoilGr()
//Author         : PATNI/AMT
//Purpose        : This function returns Saturn coil group, based on coil id
//                  specified by its argument
//*****************************************************************************/
bool CPqmCoilData::PqmCoilGetSaturnCoilGr(
    VftSectionRecvrCoil_t*	const f_coil,
    CCoil& f_satrun_coil
)const
{
    register DllT*   l_dll      = NULL;
    PqmCoil_t*		l_coil_p   = NULL;

    for (l_dll = m_head; l_dll != NULL; l_dll = l_dll->next) {

        l_coil_p = (PqmCoil_t*) l_dll->item;

        if (IS_COIL_PHYSICAL_SAME(l_coil_p->coil_id, f_coil->coilID) &&
            (l_coil_p->port == f_coil->portID[0])) {
            f_satrun_coil.SetCoil(l_coil_p);
            return true;
        }
    }

    f_satrun_coil.SetCoil(NULL);
    return false;
}

/***************************Method Header*************************************/
//Method Name    : PqmCoilGetSaturnCoilRWB()
//Author         : PATNI/AMT
//Purpose        : This function returns whole body Saturn coil based on coil id
//                  specified by its argument
//*****************************************************************************/
bool CPqmCoilData::PqmCoilGetSaturnCoilRWB(
    VftSectionRecvrCoil_t*	const f_coil,
    CCoil& f_satrun_coil
)const
{
    register DllT*   l_dll      = NULL;
    PqmCoil_t*		l_coil_p   = NULL;

    for (l_dll = m_head; l_dll != NULL; l_dll = l_dll->next) {

        l_coil_p = (PqmCoil_t*) l_dll->item;

        if (IS_COIL_PHYSICAL_SAME(l_coil_p->coil_id, f_coil->coilID) &&
            (l_coil_p->port == f_coil->portID[0])) {

            if (l_coil_p->receiveWB_enable) {
                f_satrun_coil.SetCoil(l_coil_p);
                return true;
            }
        }
    }

    f_satrun_coil.SetCoil(NULL);
    return false;
}

/***************************Method Header*************************************/
//Method Name    : PqmCoilGetSaturnEnableCoil()
//Author         : PATNI/AMT
//Purpose        : This function returns Enable Saturn coil based on coil id
//                  specified by its argument
//*****************************************************************************/
bool CPqmCoilData::PqmCoilGetSaturnEnableCoil(
    VftSectionRecvrCoil_t* const f_coil,
    CCoil& f_satrun_coil
)const
{
    register DllT*   l_dll      = NULL;
    PqmCoil_t*		l_coil_p   = NULL;

    for (l_dll = m_head; l_dll != NULL; l_dll = l_dll->next) {

        l_coil_p = (PqmCoil_t*) l_dll->item;

        if (IS_COIL_PHYSICAL_SAME(l_coil_p->coil_id, f_coil->coilID) &&
            (l_coil_p->port == f_coil->portID[0]) && l_coil_p->enable) {
            f_satrun_coil.SetCoil(l_coil_p);
            return true;
        }
    }

    f_satrun_coil.SetCoil(NULL);
    return NULL;
}

/***************************Method Header*************************************/
//Method Name    : PqmCoilGetSaturnTxCoil()
//Author         : PATNI/AMT
//Purpose        : This function returns Transmiter Saturn coil based on coil id
//                  specified by its argument
//*****************************************************************************/
bool CPqmCoilData::PqmCoilGetSaturnTxCoil(
    VftSectionRecvrCoil_t* const f_coil,
    CCoil& f_satrun_coil
)const
{
    register DllT*   l_dll      = NULL;
    PqmCoil_t*		l_coil_p   = NULL;

    for (l_dll = m_head; l_dll != NULL; l_dll = l_dll->next) {

        l_coil_p = (PqmCoil_t*) l_dll->item;

        if (IS_COIL_PHYSICAL_SAME(l_coil_p->coil_id, f_coil->coilID) &&
            (l_coil_p->port == f_coil->portID[0])) {

            if (l_coil_p->type == CDB_TRANSMIT_N_RCV) {
                f_satrun_coil.SetCoil(l_coil_p);
                return true;

            } else {
                // return PqmGlobal.wbCoil;
                f_satrun_coil.SetCoil(m_wb_coil);
                return true;
            }
        }
    }

    f_satrun_coil.SetCoil(NULL);
    return NULL;
}

/***************************Method Header*************************************/
//Method Name    : PqmCoilGetSaturnCombine()
//Author         : PATNI/AMT
//Purpose        : This function returns Combine table for list of Saturn coil
//                  specified by its argument
//*****************************************************************************/
status_t CPqmCoilData::PqmCoilGetSaturnCombine(
    VftCoilListInfo_t*	 const f_coilList,    // Coil information
    int*				 f_combine		// Combine information Table
)const
{

    if (f_coilList == NULL || f_combine == NULL) {
        return E_ERROR;
    }

    register DllT*	l_dll      = NULL;
    PqmCoil_t*		l_coil_p   = NULL;

    for (int i = 0; i < f_coilList->num; i++) {

        for (l_dll = m_head; l_dll != NULL; l_dll = l_dll->next) {

            l_coil_p = (PqmCoil_t*) l_dll->item;

            if (IS_COIL_PHYSICAL_SAME(l_coil_p->coil_id, f_coilList->coillist[i].coilID)
                &&  l_coil_p->port == f_coilList->coillist[i].portID[0]) {

                f_combine[i] = l_coil_p->combine;
                break;
            }
        }

        if (l_dll == NULL) {
            return E_ERROR;
        }
    }

    return E_NO_ERROR;
}

//***************************Method Header*************************************
//Method Name    : PqmCoilidGetSaturnCoil()
//Author         : PATNI/AMT
//Purpose        : This function returns coil based on group id specifed by its
//                  agrument
//*****************************************************************************
bool CPqmCoilData::PqmCoilidGetSaturnCoil(
    const int  f_coilid,
    const char f_port,
    const char f_channel,
    CCoil& f_satrun_coil
)const
{
    register DllT*	l_dll = NULL;
    PqmCoil_t*		l_coil_p = NULL;

    for (l_dll = m_head; l_dll != NULL; l_dll = l_dll->next) {

        l_coil_p = (PqmCoil_t*) l_dll->item;

        if (IS_COIL_PHYSICAL_SAME(l_coil_p->coil_id, f_coilid)
            &&  l_coil_p->port  ==  f_port) {
            if (f_channel == NULL || f_channel == ' '
                || l_coil_p->channel == f_channel) {
                f_satrun_coil.SetCoil(l_coil_p) ;
                return true;
            }
        }
    }

    f_satrun_coil.SetCoil(NULL) ;
    return false;
}

//***************************Method Header*************************************
//Method Name    : PqmCoilGetSaturnSection()
//Author         : PATNI/AMT
//Purpose        : This function acquires section database information based on
//                  specified by its argument.
//*****************************************************************************
status_t CPqmCoilData::PqmCoilGetSaturnSection(
    VftSectionRecvrCoil_t*	 f_recvrCoil,	// Coil Information
    int*		 f_sec_cnt,		            // Section count
    sedb_rec_t**	 f_sec_p)const		            // Section Information
{
    LPCTSTR FUNC_NAME = _T("CPqmCoilData::PqmCoilGetSaturnSection");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (f_recvrCoil == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_recvrCoil pointer is NULL"));
        return E_ERROR;
    }

    if (f_sec_cnt == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_sec_cnt pointer is NULL"));
        return E_ERROR;
    }

    if (f_sec_p == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_sec_p pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    *f_sec_cnt = 0;
    CCoil           l_coil;
    PqmCoilGetSaturnCoil(f_recvrCoil, l_coil);

    //+Patni-Hemant/2010Feb16/Modified/ACE-Phase#3/CDS Requirement
    //	if (l_coil.IsSectionDB() == 0) {
    //        return E_NO_ERROR;
    //    }

    if ((l_coil.GetCoil() == NULL) || (l_coil.IsSectionDB() == 0)) {
        return E_NO_ERROR;
    }

    //-Patni-Hemant/2010Feb16/Modified/ACE-Phase#3/CDS Requirement

    //+Patni-RUP/2009Aug24/Modified/cpp test corrections
    //l_sec_head = CDataManager::GetInstance()->GetPQMSectionData()->GetHead();
    CDataManager* l_data_manager = CDataManager::GetInstance();

    if (l_data_manager == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_data_manager pointer is NULL"));
        return E_ERROR;
    }

    CPqmSectionData* l_pqm_section_data = l_data_manager->GetPQMSectionData();

    if (l_pqm_section_data == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqm_section_data pointer is NULL"));
        return E_ERROR;
    }

    DllT*            l_sec_head  = l_pqm_section_data->GetHead();
    //-Patni-RUP/2009Aug24/Modified/cpp test corrections

    register DllT*	l_dll       = NULL;
    sedb_rec_t*		l_sec_rec_p = NULL;
    int				l_counter    = 0;

    for (l_dll = l_sec_head; l_dll != NULL; l_dll = l_dll->next) {
        l_sec_rec_p = (sedb_rec_t*) l_dll->item;

        if (l_sec_rec_p->coil_id    == f_recvrCoil->coilID    &&
            l_sec_rec_p->port       == f_recvrCoil->portID[0] &&
            l_sec_rec_p->section_id == f_recvrCoil->SectionID) {

            f_sec_p[ l_counter ]    =  l_sec_rec_p;
            l_counter++;
        }
    }

    *f_sec_cnt = l_counter;

    if (*f_sec_cnt == 0) {
        return E_ERROR;
    }

    return E_NO_ERROR;
}

//***************************Method Header*************************************
//Method Name    : PqmAllCoilGetSaturn()
//Author         : PATNI/AMT
//Purpose        : This function acquires section database information based on
//                  specifed by its agrument.
//*****************************************************************************
status_t CPqmCoilData::PqmAllCoilGetSaturn(
    PqmSaturnCoil_t*     f_coil,
    PqmSaturnRcvCoil_t*  const f_coil_tbl
)const
{
    int     i = 0, j = 0;	//Patni-MJC/2009Aug17/Modified/cpp test corrections
    CCoil	l_coil;

    f_coil_tbl->numCoil = 0 ;

    for (j = 0, i = 0 ; i < f_coil->numSection ; i++) {

        PqmCoilGetSaturnCoil(&f_coil->RcvCoil[i], l_coil) ;

        if (l_coil.GetCoil() != NULL) {

            //+TMSC-Tanoue/2010Sep03/MVC008324/Modified for i is zero
            if (i == 0
                ||  f_coil->RcvCoil[i - 1].coilID != f_coil->RcvCoil[i].coilID
                ||  f_coil->RcvCoil[i - 1].portID[0] != f_coil->RcvCoil[i].portID[0]
                ||  f_coil->RcvCoil[i - 1].portID[1] != f_coil->RcvCoil[i].portID[1]) {
                f_coil_tbl->coiltbl_p[j++] = l_coil.GetCoil() ;
                f_coil_tbl->numCoil++ ;
            }

            //-TMSC-Tanoue/2010Sep03/MVC008324

        } else {

            bzero(&f_coil_tbl->coiltbl_p, sizeof(f_coil_tbl->coiltbl_p)) ;
            f_coil_tbl->numCoil = 0 ;
            return E_ERROR ;
        }
    }

    return  E_NO_ERROR ;
}



//***************************Method Header*************************************
//Method Name    : PqmCoilGetMinChannel()
//Author         : PATNI/AMT
//Purpose        :
//*****************************************************************************
int CPqmCoilData::PqmCoilGetMinChannel(
    const int               f_num_Section,
    VftSectionRecvrCoil_t   f_recvr_Coil[]
)const
{
    LPCTSTR FUNC_NAME = _T("CPqmCoilData::PqmCoilGetMinChannel");
    sedb_rec_t*  l_sec_p[PQM_NUM_COIL_ELEMENT] = {0};
    int	        l_sec_cnt = -1;
    int         i = -1;
    int			l_channel = 0;
    CCoil       l_coil;

    // DB_FUNC_ENTER("[PqmCoilGetMinChannel]:");

    //---------------------------------------------
    //     Minimum Section count
    //---------------------------------------------
    for (i = 0; i < f_num_Section; i++) {

        if (E_NO_ERROR != PqmCoilGetSaturnSection(&f_recvr_Coil[i], &l_sec_cnt, l_sec_p)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("PqmCoilGetSaturnSection failed"));
        }

        if (l_sec_cnt ==  0) {
            PqmCoilGetSaturnCoil(&f_recvr_Coil[i], l_coil) ;
            l_channel += l_coil.GetMaxChannel();
        }

        //---------------------------------------------
        //   Minimum channel count
        //---------------------------------------------
        else if (l_sec_cnt >  0) {

            l_channel += l_sec_p[0]->minChannel;
        }
    }

    return l_channel;
}

//***************************Method Header*************************************
//Method Name    : PqmGetRflTuneInSaturn()
//Author         : PATNI/AMT
//Purpose        : Retruns a RFL and Tune limit based on coil.
//*****************************************************************************
status_t CPqmCoilData::PqmGetRflTuneInSaturn(
    PqmSaturnRcvCoil_t* const f_coil_t,
    int* f_rfl_limit,
    int* f_tune_limit
)const
{

    if (f_coil_t->numCoil == 0) {
        return E_ERROR ;
    }

    PqmCoil_t*	l_coil = NULL;

    for (int i = 0 ; i < f_coil_t->numCoil ; i++) {

        l_coil = f_coil_t->coiltbl_p[i] ;

        if (i == 0) {

            *f_tune_limit = l_coil->tuneLimit ;
            *f_rfl_limit = l_coil->rflLimit ;

        } else {

            *f_tune_limit = MIN(*f_tune_limit, l_coil->tuneLimit) ;
            *f_rfl_limit = MIN(*f_rfl_limit, l_coil->rflLimit) ;
        }
    }

    return  E_NO_ERROR ;
}


//***************************Method Header*************************************
//Method Name    : GetRWBInSaturn()
//Author         : PATNI/HAR
//Purpose        :
//*****************************************************************************
bool CPqmCoilData::GetRWBInSaturn(
    PqmSaturnRcvCoil_t* const f_saturn_recv_coil,
    const int f_mask,
    CCoil& f_ccoil
)const
{
    //Coil has been selected not without * View
    if (f_saturn_recv_coil->numCoil == 0)	{
        //return NULL;//MRP here
        return false;
    }

    //Top of the coil to store
    PqmCoil_t*	l_coil = f_saturn_recv_coil->coiltbl_p[0];

    //Back coil RWB
    int l_rwb_enable = l_coil->receiveWB_enable;
    int l_wb_type = l_rwb_enable & SATURN_RWB_COIL ;

    l_rwb_enable &= f_mask;

    for (int i = 0; i < f_saturn_recv_coil->numCoil; i++) {

        l_coil = f_saturn_recv_coil->coiltbl_p[i];

        if (l_rwb_enable != (l_coil->receiveWB_enable & f_mask) &&
            l_wb_type != (l_coil->receiveWB_enable & SATURN_RWB_COIL)) {

            f_ccoil.SetCoil(NULL);
            return false;
        }
    }

    if (l_wb_type == SATURN_RWB_COIL && l_rwb_enable == f_mask) {
        f_ccoil.SetCoil(m_wb_coil);
        return true;

    } else if (f_saturn_recv_coil->numCoil == 1) {
        CCoil l_local_coil;
        //If the traditional coil WB - No need to find a coil
        PqmCoilGetCoilRWB(f_saturn_recv_coil->coiltbl_p[0]->coil_id, l_local_coil);

        if ((l_local_coil.GetCoil() != NULL) &&
            (l_local_coil.GetCoilId() != f_saturn_recv_coil->coiltbl_p[0]->coil_id) &&
            ((l_local_coil.GetRecieveWBEnable() & SATURN_RWB_COIL)
             != SATURN_RWB_COIL)) {

            /* ****--WB--のRWB */
            l_rwb_enable = l_local_coil.GetRecieveWBEnable();
            l_rwb_enable &= f_mask;

            if (l_rwb_enable) {
                f_ccoil.SetCoil(l_local_coil.GetCoil());
                return true;
            }
        }
    }

    //return NULL ;//MRP here
    return false;
}

//***************************Method Header*************************************
//Method Name    : GetInOutSideLimit()
//Author         : PATNI/HAR
//Purpose        :
//*****************************************************************************
int CPqmCoilData::GetInOutSideLimit(
    PqmSaturnRcvCoil_t* const f_saturn_recv_coil,
    int* f_in_limit,
    int* f_out_limit
)const
{
    LPCTSTR FUNC_NAME = _T("CPqmCoilData::GetInOutSideLimit");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (f_saturn_recv_coil == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_saturn_recv_coil pointer is NULL"));
        return E_ERROR;
    }

    if (f_in_limit == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_in_limit pointer is NULL"));
        return E_ERROR;
    }

    if (f_out_limit == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_out_limit pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    if (f_saturn_recv_coil->numCoil == 0) {
        return E_ERROR;
    }

    PqmCoil_t* l_coil = f_saturn_recv_coil->coiltbl_p[0];

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (l_coil == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_coil pointer is NULL"));
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    *f_in_limit = l_coil->inSideLimit;
    *f_out_limit = l_coil->outSideLimit;

    for (int i = 0; i < f_saturn_recv_coil->numCoil; i++) {

        l_coil = f_saturn_recv_coil->coiltbl_p[i];

        if (*f_in_limit > l_coil->inSideLimit) {
            *f_in_limit = l_coil->inSideLimit;
        }

        if (*f_out_limit > l_coil->outSideLimit) {
            *f_out_limit = l_coil->outSideLimit ;
        }
    }

    return E_NO_ERROR ;
}
//***************************Method Header*************************************
//Method Name    : GetSpeederInSaturn()
//Author         : PATNI/MRP
//Purpose        :
//*****************************************************************************
status_t CPqmCoilData::GetSpeederInSaturn(PqmSaturnRcvCoil_t* coil_t, const int    mask)const
{
    if (coil_t->numCoil == 0)	return E_ERROR ;

    PqmCoil_t*	p  = NULL;

    for (int i = 0 ; i < coil_t->numCoil ; i++) {
        p = coil_t->coiltbl_p[i] ;

        if (mask) {
            if ((p->speeder_enable & mask) != mask) {
                return FALSE ;
            }

        } else if (!p->speeder_enable) {
            return FALSE ;
        }
    }

    return  TRUE ;
}
//***************************Method Header*************************************
//Method Name    : PqmCoilGetPathDB()
//Author         : PATNI/MRP
//Purpose        :
//*****************************************************************************
status_t CPqmCoilData::PqmCoilGetPathDB(const int key , path_db_rec_p p)const
{
    LPCTSTR FUNC_NAME = _T("CPqmCoilData::PqmCoilGetPathDB");
    //Uncomment after Path database is implemented

    //+Patni-RUP/2009Aug24/Modified/cpp test corrections
    //DllT *l_path_head = CDataManager::GetInstance()->GetPqmPathData()->GetHead();

    CDataManager* l_data_manager = CDataManager::GetInstance();

    if (l_data_manager == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_data_manager pointer is NULL"));
        return FALSE;
    }

    CPqmPathData* l_pqm_path_data = l_data_manager->GetPqmPathData();

    if (l_pqm_path_data == NULL) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_pqm_path_data pointer is NULL"));
        return FALSE;
    }

    DllT* l_path_head = l_pqm_path_data->GetHead();
    //-Patni-RUP/2009Aug24/Modified/cpp test corrections

    DllT* node = dllNodeType(l_path_head, key);

    if (node == NULL) {
        /* こういう状態があってはならないが
         * enable = FALSEであるため、このコイルが
         * 表示されることはない
         */
        return FALSE;
    }

    memcpy(p , ((path_db_rec_p)node->item) , sizeof(path_db_rec_t)) ;

    return TRUE ;

}


//****************************Method Header****************************
// Method Name  :GetWholeBodyPort
// Author       :PATNI/
// Purpose      :
//*********************************************************************
cdb_coil_port_t CPqmCoilData::GetWholeBodyPort()
{

#ifdef MODI

    return (GP_MAGNET(PqmGlobal.sys_type) == GP_MAGNET_50) ? CDB_PORT_B : CDB_PORT_A;

#else

    return CDB_PORT_A;

#endif

}





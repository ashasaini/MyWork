//REDMINE_782_CPP_Test_Tool
//TMSC-REDMINE-782
//TMSC-REDMINE-781
//TMSC-REDMINE-780
//TMSC-REDMINE-778
//TMSC-REDMINE-777
//redmine-779

// CoilSelectionUtility.cpp: implementation of the CCoilSelectionUtility class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CoilSelectionUtility.h"
#include <math.h>
#include <tami/libAcq/SaturnCoilCombinChk.h>
#include <libStudyManager/strcnv.h>
#include <PQM/resource.h>
#include "CoilLstDlg.h"

#define ABSF(a) ((a) < 0.0 ? (-(a)) : (a))

using namespace MR::ACQ::SM::COMMON;

CoilSelectionGlobal_t::CoilSelectionGlobal_t(
)
{
    coil.channelPerVFEP = 8;
    coil.numChannels = 16;
    coil.sft_mode = -2;
    coil.VAPChannels = 2;
    message = IDS_CS_MES_SELECT_SECTION;
    config.pqmMode = 1;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//***************************Method Header*************************************
//Method Name    : CCoilSelectionUtility()
//Author         : PATNI / HAR
//Purpose        :
//*****************************************************************************
CCoilSelectionUtility::CCoilSelectionUtility(
):	m_has_disallowable_combine_coil(false),
    m_coil_selection_mode(COIL_SEL_NORMAL),
    m_connect_coil(NULL),
    m_show_coil_list(NULL),
    m_select_coil_list(NULL),
    m_uiclassinterfaceforcoilseldialog(NULL),
    m_coillistdialog_interface(NULL),
    m_select_count(0),
    m_head(NULL),
    m_coil_trestle_head(NULL),
    m_compare_flag(FALSE),
    m_list_flag(FALSE)
{
    //    m_coil_selection_global.coil.sft_mode = -1;
    //	m_coil_selection_global.coil.numChannels = 16;
    //	m_coil_selection_global.coil.VAPChannels= 8;
    //	m_coil_selection_global.coil.channelPerVFEP = 2;
    //
    //+Patni-PP/2009May28/Added/SU14 CPP test initialization corrections
    *m_coil_infomation	= NULL;


    for (int i = 0; i < 2 ; i++) {

        m_coil_liner_head[i] = NULL ;
    }

    for (i = 0; i < 7 ; i++) {

        m_coil_couch_head[i] = NULL ;
    }


    m_coil_selection_global.CoilSelectionGlobal_t();
    InitUtilityFlags();

    //+Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement
    SetCoilSelMode(COIL_SEL_NORMAL) ;
    //-Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement

    //REDMINE_782_CPP_Test_Tool
    ResetAllMarkerSigns();
}


//***************************Method Header*************************************
//Method Name    : ~CCoilSelectionUtility()
//Author         : PATNI / HAR
//Purpose        :
//*****************************************************************************
CCoilSelectionUtility::~CCoilSelectionUtility(
)
{
    LPCTSTR FUNC_NAME = _T("CCoilSelectionUtility::~CCoilSelectionUtility");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //  + Himanshu 24 Aug 2008
    DEL_PTR(m_connect_coil)

    DEL_PTR(m_show_coil_list)

    DEL_PTR(m_select_coil_list)

    //  - Himanshu 24 Aug 2008
}

//***************************Method Header*************************************
//Method Name    : CheckCombine()
//Author         : PATNI / HAR
//Purpose        :
//*****************************************************************************
//Patni-AMT/2009Oct27/Modified/MCM0229-00006
status_t CCoilSelectionUtility::CheckCombine(
)
{
    LPCTSTR FUNC_NAME = _T("CCoilSelectionUtility::CheckCombine");
    PQM_TRACE_SCOPED(FUNC_NAME);
    //+Patni-KSS/2011Apr15/Commented/Defect No-84
    //if (NULL == m_coillistdialog_interface) {
    //    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
    //              _T("m_coillistdialog_interface pointer is NULL"));
    //    return COIL_S_ERROR;
    //}
    //-Patni-KSS/2011Apr15/Commented/Defect No-84

    int	buf[MAX_PORT] = {0};	//Patni-PP/2009May28/Added/SU14 CPP test initialization corrections
    memset(buf, 0, sizeof(buf));

    int	cnt = 0;

    SetDisallowableCombineCoil(false);

    for (int i = 0; i < MAX_PORT; i++) {

        if (m_selected_coil[i].coil != NULL) {

            if (m_selected_coil[i].coil->combine == COMBINE_NOT_TOGETHER) {

                //Added by Hemant On 2/2/2009 5:03:39 PM
                //don't call when ptr is not there
                //That is, the second window is not opened for once.
                //The following only enables or disables the options/buttons on second dialog..

                //Added by Hemant On 10/15/2008 5:12:44 PM

                //+Patni-KSS/2011Apr15/Added/Defect No-84
                if (m_coillistdialog_interface) {
                    m_coillistdialog_interface->CheckCombineFailedFor(i);
                }

                //-Patni-KSS/2011Apr15/Added/Defect No-84

                SetDisallowableCombineCoil(true);
                //Patni-AMT/2009Oct27/Modified/MCM0229-00006
                return COIL_S_ERROR;

            } else if (m_selected_coil[i].coil->combine
                       == COMBINE_PERMISSION_CONNECT) {

                buf[cnt] =  i;
                cnt	++;
            }
        }
    }

    //Added by Hemant On 2/2/2009 5:03:39 PM
    //don't call when ptr is not there
    //That is, the second window is not opened for once.
    //The following only enables or disables the options/buttons on second dialog..

    //+Patni-KSS/2011Apr15/Added/Defect No-84
    if (m_coillistdialog_interface) {
        m_coillistdialog_interface->CheckCombineFailedFor(-1);
    }

    //-Patni-KSS/2011Apr15/Added/Defect No-84

    if (cnt >= 2) {
        // Display warning in Red Color
        for (i = 0; i < cnt; i++) {
            //CoilSelectionUiSetMenuWarningColor(
            //CoilSelectionUiGetWidget(W_DLG_COIL1_OM + buf[i]));
        }

        //Patni-AMT/2009Oct27/Modified/MCM0229-00006
        return COIL_S_OTHER;
    }

    //Patni-AMT/2009Oct27/Modified/MCM0229-00006
    return COIL_S_NO_ERROR;
}



//***************************Method Header*************************************
//Method Name    : CheckPosition()
//Author         : PATNI / HAR
//Purpose        :
//*****************************************************************************
//Patni-AMT/2009Oct27/Modified/MCM0229-00006
status_t CCoilSelectionUtility::CheckPosition(
    const CoilSelectionSelectCoil_t* f_coil
)const
{
    LPCTSTR FUNC_NAME = _T("CCoilSelectionUtility::CheckPosition");
    PQM_TRACE_SCOPED(FUNC_NAME);
    //+Patni-KSS/2011Apr15/Commented/Defect No-84
    //if (NULL == m_coillistdialog_interface) {
    //    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
    //              _T("m_coillistdialog_interface pointer is NULL"));
    //    return COIL_S_ERROR;
    //}
    //-Patni-KSS/2011Apr15/Commented/Defect No-84

    //+Patni-Hemant/2010Feb15/Added/ACE-Phase#3/CDS Requirement
    //    int     i = -1;
    //    int     position = -1;
    //    int     buf[MAX_SECTION_BUTTON] = {0};
    //    int     buf2[MAX_SECTION_BUTTON] = {0};
    //
    //    BOOL    l_flag = TRUE;
    //    CoilSelectionSection_t  *l_section = NULL;
    //    DllT                    *l_trav = NULL;
    //
    //   CoilSelectionSelectCoil_t *l_select_coil = NULL;

    int	j = 0, position = 0, location = 0, wide = 0;

    //+Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_09
    //int			buf[3][MAX_SECTION] = {0}, buf2[3][MAX_SECTION] = {0};
    const int l_total_coil_layers = 3;
    int	buf[l_total_coil_layers][MAX_SECTION] = {0};
    int	buf2[l_total_coil_layers][MAX_SECTION] = {0};
    //-Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_09

    status_t rtn = COIL_S_NO_ERROR;
    //-Patni-Hemant/2010Feb15/Added/ACE-Phase#3/CDS Requirement

    //+Patni-Hemant/2010Mar1/Added/ACE-Phase#3/CDS Requirement
    //+Patni-KSS/2011Apr15/Added/Defect No-84
    if (m_coillistdialog_interface) {
        for (int i = 0; i < MAX_COUCH_PORT + MAX_LINEAR_PORT ; i++) {
            m_coillistdialog_interface->UiSetMenuDefaultColorForCouch(ACoilPositionButtons, (ACoilItems) i);
        }
    }

    //-Patni-KSS/2011Apr15/Added/Defect No-84
    //-Patni-Hemant/2010Mar1/Added/ACE-Phase#3/CDS Requirement

    //Patni-KSS/2011Apr15/Added/Defect No-84
    for (int i = 0; i < MAX_COUCH_PORT + MAX_LINEAR_PORT; i++) {

        if (f_coil[i].coil != NULL) {
            if (f_coil[i].coil->numofsec > 0
                && f_coil[i].coil->location != 0x0000
                && f_coil[i].position <= 0) {

                //PENDING
                //CoilSelectionUiSetMenuWarningColor( CoilSelectionUiGetWidget(W_DLG_COUCH1_OM + i));

                //Added by Hemant On 2/2/2009 5:03:39 PM
                //don't call when ptr is not there
                //That is, the second window is not opened for once.
                //The following only enables or disables the options/buttons on second dialog..
                //+Patni-KSS/2011Apr15/Added/Defect No-84
                if (m_coillistdialog_interface) {
                    m_coillistdialog_interface->UiSetMenuWarningColorForCouch(ACoilPositionButtons, (ACoilItems) i) ;
                }

                //-Patni-KSS/2011Apr15/Added/Defect No-84
                //+Patni-Hemant/2010Feb15/Added/ACE-Phase#3/CDS Requirement
                //l_flag = FALSE;
                rtn = COIL_S_NOPROCESS;
                //-Patni-Hemant/2010Feb15/Added/ACE-Phase#3/CDS Requirement

            } else if (f_coil[i].coil->location != 0x0000) { //REDMINE-485/TMSC/Tanoue/4ch Flex Coil
                //+Patni-Hemant/2010Feb15/Modified/ACE-Phase#3/CDS Requirement
                /*
                position = ComputePosition(f_coil[i].coil->location,
                                           f_coil[i].position);

                if (position != 99) {

                    for (l_trav = f_coil[i].coil->section;
                         l_trav != NULL; l_trav = l_trav->next) {

                        l_section = (CoilSelectionSection_t*)l_trav->item;

                        buf[position + l_section->section - 1]++;

                        if (buf[position + l_section->section - 1] >= 2) {

                            //PENDING
                            //CoilSelectionUiSetMenuWarningColor(
                            //CoilSelectionUiGetWidget(W_DLG_COUCH1_OM + i));
                            //CoilSelectionUiSetMenuWarningColor(
                            //CoilSelectionUiGetWidget(W_DLG_COUCH1_OM +
                            //buf2[position + sec_t->section-1]));

                            //Added by Hemant On 2/2/2009 5:03:39 PM
                            //don't call when ptr is not there
                            //That is, the second window is not opened for once.
                            //The following only enables or disables the options/buttons on second dialog..
                            if (m_coillistdialog_interface) {

                                m_coillistdialog_interface->UiSetMenuWarningColorForCouch(ACoilPositionButtons, (ACoilItems) i) ;
                                m_coillistdialog_interface->UiSetMenuWarningColorForCouch(ACoilPositionButtons, (ACoilItems) buf2[position + l_section->section-1]) ;
                            }
                        }

                        // To retain the last number of menu
                        buf2[position + l_section->section-1] = i;
                    }
                }
                */
                if (f_coil[i].coil->location & LOCATION_UPPER) {
                    /* UPPER */
                    location = 0;

                } else if (f_coil[i].coil->location & LOCATION_LOWER) {
                    /* LOWER */
                    location = 1;

                } else if (f_coil[i].coil->location & LOCATION_CENTER) {
                    /* CENTER */
                    location = 2;
                }

                if (f_coil[i].coil->location == LOCATION_HEAD) {
                    /* HEAD is fixed at 2 and (HEAD is so special coil) */

                    wide = 2;

                    if (f_coil[i].coil->numofsec > 2) {

                        CoilSelectionSection_t* l_section = NULL;
                        DllT* l_trav = NULL;

                        for (l_trav = f_coil[i].coil->section;
                             l_trav != NULL; l_trav = l_trav->next) {

                            l_section = (CoilSelectionSection_t*)l_trav->item;

                            if (l_section && l_section->section >= 7) {
                                wide = 3;
                            }
                        }
                    }


                } else {
                    wide = (int) GetTotalLablesOccupied(&f_coil[i]);
                }

                for (j = 0; j < wide; j++) {
                    position = f_coil[i].position + j - 1;
                    buf[location][position]++;

                    if (buf[location][position] >= 2) {
                        /* More than two overlapping */

                        //CoilSelectionUiSetMenuWarningColor( CoilSelectionUiGetWidget(W_DLG_COUCH1_OM + i));
                        //CoilSelectionUiSetMenuWarningColor( CoilSelectionUiGetWidget(W_DLG_COUCH1_OM
                        //	+ buf2[location][position]));
                        //+Patni-KSS/2011Apr15/Added/Defect No-84
                        if (m_coillistdialog_interface) {
                            m_coillistdialog_interface->UiSetMenuWarningColorForCouch(ACoilPositionButtons, (ACoilItems) i) ;
                            m_coillistdialog_interface->UiSetMenuWarningColorForCouch(ACoilPositionButtons, (ACoilItems)(buf2[location][position])) ;
                        }

                        //-Patni-KSS/2011Apr15/Added/Defect No-84

                        if (rtn != COIL_S_NOPROCESS)	rtn = COIL_S_ERROR;
                    }

                    buf2[location][position] = i;		/* Hold a number of the previous menu */
                }

                //-Patni-Hemant/2010Feb15/Modified/ACE-Phase#3/CDS Requirement
            }
        }
    }

    //+Patni-Hemant/2010Feb15/Modified/ACE-Phase#3/CDS Requirement
    //    if (!l_flag) {
    //        //Patni-AMT/2009Oct27/Modified/MCM0229-00006
    //        return COIL_S_NOPROCESS;
    //    }
    //
    //    for (i = 0; i < MAX_SECTION_BUTTON; i++) {
    //
    //        if (buf[i] >= 2) {
    //            //Patni-AMT/2009Oct27/Modified/MCM0229-00006
    //            return COIL_S_ERROR;
    //        }
    //    }
    //
    //
    //    //Patni-AMT/2009Oct27/Modified/MCM0229-00006
    //    return COIL_S_NO_ERROR;

    return rtn;
    //-Patni-Hemant/2010Feb15/Modified/ACE-Phase#3/CDS Requirement
}

//***************************Method Header*************************************
//Method Name    : ComputePosition()
//Author         : PATNI / HAR
//Purpose        :
//*****************************************************************************
int CCoilSelectionUtility::ComputePosition(
    const int location,
    const int position
)const
{
    int coordinate = 0;
    //DB_FUNC_ENTER("[coilSelectionComputePosition]:");

    switch (location) {
        case LOCATION_UPPER:	 /* Upper */
            coordinate = position - 1;
            break;

        case LOCATION_LOWER:	 /* Lower */
            //+Patni-Hemant/2010Feb15/Added/ACE-Phase#3/CDS Requirement
            ////Patni-AMT/2009Oct05/Modified/MVC005124 - Code Review
            //coordinate = position - 1 + MAX_SECTION + 4;
            coordinate = position - 1 + MAX_SECTION + 7;
            //-Patni-Hemant/2010Feb15/Added/ACE-Phase#3/CDS Requirement
            break;

        case LOCATION_HEAD:		 /* Head */

            //+Patni-Hemant/2010Feb15/Added/ACE-Phase#3/CDS Requirement
        case LOCATION_HEAD_CENTER:		 /* Head Center */
            //-Patni-Hemant/2010Feb15/Added/ACE-Phase#3/CDS Requirement
            coordinate = MAX_SECTION;
            break;

        case LOCATION_SPINE:	 /* Spine */
            //+Patni-Hemant/2010Feb15/Added/ACE-Phase#3/CDS Requirement
            ////Patni-AMT/2009Oct05/Modified/Defect Fix MVC004910/MVC005124 - Code Review
            //coordinate = position - 1 + MAX_SECTION + 4;
            coordinate = position - 1 + MAX_SECTION + 7;
            //-Patni-Hemant/2010Feb15/Added/ACE-Phase#3/CDS Requirement
            break;

        default:
            //Patni-AMT/2009Oct27/Modified/MCM0229-00006
            coordinate = -1;
            break;
    }

    return coordinate;
}

float CCoilSelectionUtility::GetTotalLablesOccupied(
    const CoilSelectionSelectCoil_t* const f_coil_struct
)const
{
    float l_ret_val = 0.f ;

    if (f_coil_struct) {
        CoilSelectionSection_t* l_section1 = (CoilSelectionSection_t*)f_coil_struct->coil->section->item;
        l_ret_val = (float) ceil((f_coil_struct->coil->numofsec * l_section1->elemXYZ[2]) /  127.f) ;
    }

    return l_ret_val;
}


//***************************Method Header*************************************
//Method Name    : ComputePosition()
//Author         : PATNI / HAR + AMT
//Purpose        :
//*****************************************************************************
int CCoilSelectionUtility::ComputePosition(
    const int position
)const
{
    int coordinate = 0;

    //Patni-Hemant/2010May6/Added/JaFT/SM_PH3_CDR-CDS_CDR_10
    const int l_head_neck_btn_cnt = 7;

    if (position < MAX_SECTION) {               /* Upper */
        /* Set up the coil position = position + 1 button (so starting from scratch) */
        coordinate = position + 1;

        //Patni-AMT/2009Oct05/Modified/MVC005124

        //+Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_10
        //} else if (MAX_SECTION + 6 <= position) {   /* Lower */

    } else if (MAX_SECTION + l_head_neck_btn_cnt <= position) {   /* Lower */
        //-Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_10

        //+Patni-Hemant/2010Feb15/Modified/ACE-Phase#3/CDS Requirement
        //Set up the coil position = position + 1 button (so starting from scratch)
        //coordinate = position + 1 - MAX_SECTION - 4;
        coordinate = position + 1

                     //+Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_10
                     //- MAX_SECTION - 6; //  The upper number - the number of lower
                     - MAX_SECTION - l_head_neck_btn_cnt; //  The upper number - the number of lower
        //-Patni-Hemant/2010May6/Modified/JaFT/SM_PH3_CDR-CDS_CDR_10

        //-Patni-Hemant/2010Feb15/Modified/ACE-Phase#3/CDS Requirement

    } else {                                    /* Head */
        switch (position) {
            case 15 :
            case 16 :
                coordinate = 1;
                break;

            case 17 :
            case 18 :
                coordinate = 2;
                break;

            case 19 :
            case 20 :   //Patni-AMT/2009Oct05/Modified/MVC005124
                coordinate = 3;
                break;
        }
    }

    return coordinate;
}

//***************************Method Header*************************************
//Method Name    : CoilCheckList()
//Author         : PATNI / HAR
//Purpose        :
//*****************************************************************************
//+Patni-Hemant/2010Feb15/Modified/ACE-Phase#3/CDS Requirement
//BOOL CCoilSelectionUtility::CoilCheckList(
//    BOOL f_flag,
//    CString &f_message,
//    MessageBoxTypeEnum& messageboxtypeenum /*= MessageBoxType_OK_CANCEL*/
//)
status_t CCoilSelectionUtility::CoilCheckList(
    const BOOL f_flag
)
//-Patni-Hemant/2010Feb15/Modified/ACE-Phase#3/CDS Requirement
{
    LPCTSTR FUNC_NAME = _T("CCoilSelectionUtility::CoilCheckList");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-AMT/2009Oct27/Modified/MCM0229-00006
    // 0, -1 changed to COIL_S_ERROR, COIL_S_NO_ERROR, COIL_S_NOPROCESS
    // Combine Check
    status_t l_rtn_sts = CheckCombine();

    //+Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement
    MPLUSERRORUSERRESPONSE_e l_response = MPlusErrResponse_Error;

    //-Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement
    if (l_rtn_sts == COIL_S_ERROR) {

        if (f_flag) {



            //CoilSelectionUiWarningDialog(
            //CoilSelectionUiFetchLiteral("WAR_CHECK_COMBINE"),
            //coilSelectionCheckOkCB,
            //coilSelectionCheckCancelCB,
            //NULL);

            //+Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement
            //messageboxtypeenum = MessageBoxType_OK_CANCEL ;
            l_response = CCoilSelectionUtility::DisplayMessageinWarningDialog(
                             CPQMUtility::GetMultiLingualString(_T("IDS_CS_WAR_CHECK_COMBINE")),
                             SCAN_WARN_OK_CANCEL_BUTTONS);

            CoilSelectionOKCancel(l_response == MPlusErrResponse_OK) ;
            //-Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement
        }

        //+Patni-Hemant/2010Feb12/Modified/ACE-Phase#3/CDS Requirement
        //return FALSE;
        return ((l_response == MPlusErrResponse_OK) ? COIL_S_NO_ERROR : COIL_S_ERROR);
        //-Patni-Hemant/2010Feb12/Modified/ACE-Phase#3/CDS Requirement

    } else if (l_rtn_sts == COIL_S_OTHER) {

        if (f_flag) {


            //CoilSelectionUiWarningDialog(
            //CoilSelectionUiFetchLiteral("WAR_CHECK_COMBINE_2"),
            //coilSelectionCheckOkCB,
            //coilSelectionCheckCancelCB,
            //NULL);

            //+Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement
            //messageboxtypeenum = MessageBoxType_OK_CANCEL ;
            l_response = CCoilSelectionUtility::DisplayMessageinWarningDialog(
                             CPQMUtility::GetMultiLingualString(_T("IDS_CS_WAR_CHECK_COMBINE_2")),
                             SCAN_WARN_OKBUTTON);
            CoilSelectionOKCancel(l_response != MPlusErrResponse_OK) ;
            //-Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement
        }

        //+Patni-Hemant/2010Feb12/Modified/ACE-Phase#3/CDS Requirement
        //return FALSE;
        return COIL_S_ERROR;
        //-Patni-Hemant/2010Feb12/Modified/ACE-Phase#3/CDS Requirement
    }

    if (CheckPort() != COIL_S_NO_ERROR) {

        if (f_flag) {

            //CoilSelectionUiWarningDialog(
            //CoilSelectionUiFetchLiteral("WAR_CHECK_PORT"),
            //coilSelectionCheckCancelCB,
            //NULL,
            //NULL);
            //+Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement
            //messageboxtypeenum = MessageBoxType_CANCEL ;
            l_response = CCoilSelectionUtility::DisplayMessageinWarningDialog(
                             CPQMUtility::GetMultiLingualString(_T("IDS_CS_WAR_CHECK_PORT")),
                             SCAN_WARN_OKBUTTON);
            CoilSelectionOKCancel(l_response != MPlusErrResponse_OK) ;
            //-Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement
        }

        //+Patni-Hemant/2010Feb12/Modified/ACE-Phase#3/CDS Requirement
        //return FALSE;
        return COIL_S_ERROR;
        //-Patni-Hemant/2010Feb12/Modified/ACE-Phase#3/CDS Requirement
    }

    /*
     * Positionチェック：設定されていないまたは、重なっているコイルが
     * あれば警告メッセージを表示しコイルリストへ戻る。
     */
    l_rtn_sts = CheckPosition(m_selected_coil);

    if (l_rtn_sts == COIL_S_NOPROCESS) {

        if (f_flag) {

            //CoilSelectionUiWarningDialog(
            //CoilSelectionUiFetchLiteral("WAR_CHECK_NO_POSITION"),
            //coilSelectionCheckCancelCB,
            //NULL,
            //NULL);

            //+Patni-Hemant/2010Mar2/Commented/ACE-Phase#3/CDS Requirement
            //			//+Patni-ARD/2009-Nov-25/Added/DeFT# MVC003149 + MVC005179
            //            CPQMLogMgr::GetInstance()->DisplayScanError( IDS_CS_WAR_CHECK_NO_POSITION,
            //				_T("IDS_CS_WAR_CHECK_NO_POSITION"), SCAN_ERR_NOTIFY, L"CoilSelectionUtility.cpp");
            //			//-Patni-ARD/2009-Nov-25/Added/DeFT# MVC003149 + MVC005179
            //-Patni-Hemant/2010Mar2/Commented/ACE-Phase#3/CDS Requirement

            //+Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement
            //messageboxtypeenum = MessageBoxType_CANCEL ;
            l_response = CCoilSelectionUtility::DisplayMessageinWarningDialog(
                             CPQMUtility::GetMultiLingualString(_T("IDS_CS_WAR_CHECK_NO_POSITION")),
                             SCAN_WARN_OKBUTTON);
            CoilSelectionOKCancel(l_response != MPlusErrResponse_OK) ;
            //-Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement
        }

        //+Patni-Hemant/2010Feb15/Modified/ACE-Phase#3/CDS Requirement
        //return FALSE;
        return COIL_S_NOPROCESS;
        //-Patni-Hemant/2010Feb15/Modified/ACE-Phase#3/CDS Requirement

    } else if (l_rtn_sts == COIL_S_ERROR) {

        if (f_flag) {

            //CoilSelectionUiWarningDialog(
            //CoilSelectionUiFetchLiteral("WAR_CHECK_POSITION"),
            //coilSelectionCheckCancelCB,
            //NULL,
            //NULL);

            //+Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement
            //messageboxtypeenum = MessageBoxType_CANCEL ;
            l_response = CCoilSelectionUtility::DisplayMessageinWarningDialog(
                             CPQMUtility::GetMultiLingualString(_T("IDS_CS_WAR_CHECK_POSITION")),
                             SCAN_WARN_OKBUTTON);
            CoilSelectionOKCancel(l_response != MPlusErrResponse_OK) ;
            //-Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement
        }

        //+Patni-Hemant/2010Feb15/Modified/ACE-Phase#3/CDS Requirement
        //return FALSE;
        return COIL_S_ERROR;
        //-Patni-Hemant/2010Feb15/Modified/ACE-Phase#3/CDS Requirement
    }

    //+Patni-Hemant/2010Feb15/Modified/ACE-Phase#3/CDS Requirement
    ////-Patni-AMT/2009Oct27/Modified/MCM0229-00006
    //return TRUE;
    return COIL_S_NO_ERROR ;
    //-Patni-Hemant/2010Feb15/Modified/ACE-Phase#3/CDS Requirement
}



//***************************Method Header*************************************
//Method Name    : CoilSetCoilButton()
//Author         : PATNI / HAR
//Purpose        :
//*****************************************************************************
void CCoilSelectionUtility::CoilSetCoilButton(
    const bool f_offset /*= false*/,
    const int f_step /*= -1*/,
    const bool f_ignore_pos_change /*= false*/
)
{
    LPCTSTR FUNC_NAME = _T("CCoilSelectionUtility::CoilSetCoilButton");
    PQM_TRACE_SCOPED(FUNC_NAME);

    int j = -1, l = -1, position = -1;

    //CCoilLstDlg					*l_list_dialog = NULL;
    //l_list_dialog = GetListDialog();
    //DB_FUNC_ENTER("[CoilSelectionCoilSetCoilButton]:");

    for (int i = 0; i < MAX_SECTION_BUTTON; i++) {

        m_section_button[i].enable  = FALSE;
        m_section_button[i].coil	= NULL;
        m_section_button[i].section = NULL;
        m_section_button[i].label   = '\0';
        m_section_button[i].port    = 0;
        m_section_button[i].w       = 0;
    }

    for (i = 0; i < MAX_PORT; i++) {
        m_coil_button[i].enable     = FALSE;
        m_coil_button[i].coil	    = NULL;
        m_coil_button[i].section    = NULL;
        m_coil_button[i].label      = '\0';
        m_coil_button[i].port       = 0;
        m_coil_button[i].w          = 0;
    }

    // PENDING
    //for(i = W_COIL_TRESTLE_LBL; i <= W_COIL5_LBL; i++){
    //	XtUnmapWidget(CoilSelectionUiGetWidget(i));
    //}

    m_select_count = 0;

    for (i = 0; i < MAX_PORT; i++) {

        //CString l_str(l_select_coil->coil->description);
        //AfxMessageBox(l_str, MB_SYSTEMMODAL);

        if (m_applied_coil[i].coil != NULL) {
            m_select_count++;
        }
    }

    if (m_select_count == 0) {
        //Display Message

        CoilSelectionUiMessage(IDS_CS_MES_SELECT_COILLIST);

    } else {

        // PENDING Display UIMessage

        if (m_coil_selection_global.message != IDS_CS_MES_UPPER_1 &&
            m_coil_selection_global.message != IDS_CS_MES_UPPER_2) {

            CoilSelectionUiMessage(IDS_CS_MES_SELECT_SECTION);

        }
    }


    CoilSelectionSection_t* l_section = NULL;
    DllT* l_trav = NULL;
    CString  l_label = _T("");
    CString l_sub_label = _T("");

    for (i = MAX_PORT - 1; i >= 0; i--) {

        //+Patni-Hemant/2010Feb15/Added/ACE-Phase#3/CDS Requirement
        //          if (m_applied_coil[i].coil != NULL) {
        //            if (m_applied_coil[i].coil->combine == COMBINE_NOT_TOGETHER) {
        if ((m_applied_coil[i].coil != NULL) && (m_applied_coil[i].coil->combine == COMBINE_NOT_TOGETHER)) {
            //-Patni-Hemant/2010Feb15/Added/ACE-Phase#3/CDS Requirement

            if (m_applied_coil[i].coil->location == 0) {

                if (m_applied_coil[i].coil->port != CDB_PORT_G
                    && m_applied_coil[i].coil->port != CDB_PORT_D) {

                    //XtMapWidget(CoilSelectionUiGetWidget(W_COIL_TRESTLE_LBL));
                    j = 0;

                } else if (m_applied_coil[i].coil->port  == CDB_PORT_G) {

                    j = 3;

                    if (m_applied_coil[i].port != 0) {

                        //XtVaSetValues(	CoilSelectionUiGetWidget(W_COIL3_LBL),
                        //				XmNlabelString,				label,
                        //				NULL);
                        //XtMapWidget(CoilSelectionUiGetWidget(W_COIL3_LBL));
                        //XmStringFree(label);
                    }

                } else {

                    if (m_applied_coil[i].coil->channel == CDB_CHANNEL_A) {

                        //XtMapWidget(CoilSelectionUiGetWidget(W_COIL1_LBL));
                        j = 1;
                    }

                    if (m_applied_coil[i].coil->channel == CDB_CHANNEL_B) {

                        //XtMapWidget(CoilSelectionUiGetWidget(W_COIL2_LBL));
                        j = 2;
                    }
                }

                m_coil_button[j].coil = m_applied_coil[i].coil;
                m_applied_coil[j].port = m_applied_coil[i].port;


                if (m_applied_coil[i].coil->port  != 'G' &&
                    m_applied_coil[i].coil->port  != 'D') {

                    m_coil_button[j].label = m_applied_coil[i].coil->label;

                } else {
                    m_coil_button[j].label = m_applied_coil[i].coil->short_name;
                }

                if (m_applied_coil[i].coil->numofsec) {

                    l_section = (CoilSelectionSection_t*)m_applied_coil[i].coil->section->item;
                    m_coil_button[j].section = l_section;
                }

                m_coil_button[j].enable = TRUE;

                return;

            } else {

                if (m_applied_coil[i].position != 0) {

                    position = ComputePosition(
                                   m_applied_coil[i].coil->location,
                                   m_applied_coil[i].position);

                    for (l_trav = m_applied_coil[i].coil->section;
                         l_trav != NULL; l_trav = l_trav->next) {

                        l_label = _T("");

                        l_section = (CoilSelectionSection_t*)l_trav->item;

                        CoilSelectionSectionButton_t* l_section_btn = &m_section_button[position + l_section->section - 1];

                        l_section_btn->section = l_section;
                        l_section_btn->coil = m_applied_coil[i].coil;
                        l_section_btn->port = m_applied_coil[i].port;

                        bool l_can_offset = f_offset;

                        if ((-1 != f_step)) {

                            l_can_offset = (m_applied_coil[i].coil->location & f_step) > 0;

                            if (l_can_offset) {
                                OffsetButtonAsPerSize(&m_applied_coil[i], l_section_btn, !f_offset);
                            }

                        } else if (!f_ignore_pos_change) {

                            OffsetButtonAsPerSize(&m_applied_coil[i], l_section_btn, f_offset);
                        }

                        l_label.Format(_T("%s%d"),
                                       m_applied_coil[i].coil->short_name,
                                       l_section->section);

                        //label = XmStringCreateSimple(selectCoil[i].coil->short_name);
                        //num   = XmStringCreateSimple(c);
                        //{ Nilesh J.
                        l_sub_label = (m_section_button[position +
                                                        l_section->section - 1 ].label);

                        //} Nilesh J.
                        l_sub_label.Format(_T("%s%d"),
                                           m_applied_coil[i].coil->short_name,
                                           l_section->section);

                        m_section_button[position + l_section->section - 1 ].enable = TRUE;
                    }

                    return;
                }
            }

            //Patni-Hemant/2010Feb15/Commented/ACE-Phase#3/CDS Requirement
            //}
        }
    }

    l = 3;

    for (i = 0; i < MAX_PORT; i++) {

        if (m_applied_coil[i].coil != NULL) {

            if (m_applied_coil[i].coil->location == 0) {

                if (m_applied_coil[i].coil->port  != CDB_PORT_G
                    && m_applied_coil[i].coil->port  != CDB_PORT_D) {

                    //XtMapWidget(CoilSelectionUiGetWidget(W_COIL_TRESTLE_LBL));

                    j = 0;

                } else if (m_applied_coil[i].coil->port == CDB_PORT_G) {

                    j = l;

                    if (m_applied_coil[i].port != 0) {


                        //XtVaSetValues(	CoilSelectionUiGetWidget(W_COIL3_LBL+j-3),
                        //	XmNlabelString,				label,
                        //	NULL);
                        //XtMapWidget(CoilSelectionUiGetWidget(W_COIL3_LBL+j-3));
                        //XmStringFree(label);
                        ASSERT(FALSE);
                    }

                    l++;

                } else {

                    if (m_applied_coil[i].coil->channel == CDB_CHANNEL_A) {

                        //XtMapWidget(CoilSelectionUiGetWidget(W_COIL1_LBL));
                        j = 1;
                    }

                    if (m_applied_coil[i].coil->channel == CDB_CHANNEL_B) {

                        //XtMapWidget(CoilSelectionUiGetWidget(W_COIL2_LBL));
                        j = 2;
                    }
                }

                m_coil_button[j].coil = m_applied_coil[i].coil;
                m_coil_button[j].port = m_applied_coil[i].port;

                if (m_applied_coil[i].coil->numofsec) {

                    l_section = (CoilSelectionSection_t*)m_applied_coil[i].coil->section->item;
                    m_coil_button[j].section = l_section;
                }

                if (m_applied_coil[i].coil->port != CDB_PORT_G &&
                    m_applied_coil[i].coil->port  != CDB_PORT_D) {

                    m_coil_button[j].label =
                        m_applied_coil[i].coil->label;

                } else {
                    m_coil_button[j].label = m_applied_coil[i].coil->short_name;
                }

                m_coil_button[j].enable = TRUE;

            } else {

                if (m_applied_coil[i].position != 0) {

                    position = ComputePosition(m_applied_coil[i].coil->location,
                                               m_applied_coil[i].position);

                    for (l_trav = m_applied_coil[i].coil->section;
                         l_trav != NULL; l_trav = l_trav->next) {

                        l_section = (CoilSelectionSection_t*)l_trav->item;

                        CoilSelectionSectionButton_t* l_section_btn = &m_section_button[position + l_section->section - 1];

                        l_section_btn->section = l_section;
                        l_section_btn->coil = m_applied_coil[i].coil;
                        l_section_btn->port = m_applied_coil[i].port;

                        bool l_can_offset = f_offset;

                        if ((-1 != f_step)) {

                            l_can_offset = (m_applied_coil[i].coil->location & f_step) > 0;

                            if (l_can_offset) {
                                OffsetButtonAsPerSize(&m_applied_coil[i], l_section_btn, !f_offset);
                            }

                        } else if (!f_ignore_pos_change) {

                            OffsetButtonAsPerSize(&m_applied_coil[i], l_section_btn, f_offset);
                        }

                        char c[2] = {0};
                        sprintf(c, "%d", l_section->section);

                        //CoilSelectionSectionButton[position + l_section->section - 1].label = XmStringConcat(label, num);
                        m_section_button[position + l_section->section - 1].label =  m_applied_coil[i].coil->short_name + CString(c);
                        m_section_button[position + l_section->section - 1].enable = TRUE;
                    }
                }
            }
        }
    }
}


//***************************Method Header*************************************
//Method Name    : CoilMapCoilButton()
//Author         : PATNI / HAR
//Purpose        :
//*****************************************************************************
void CCoilSelectionUtility::CoilMapCoilButton(
    const bool f_is_move_coil_btn	/*false*/)
{
    LPCTSTR FUNC_NAME = _T("CCoilSelectionUtility::CoilMapCoilButton");
    PQM_TRACE_SCOPED(FUNC_NAME);

    int l_count = 0;	//Patni-PP/2009May28/Added/SU14 CPP test initialization corrections

    for (int i = 0; i < MAX_SECTION_BUTTON; i++) {
        //Patni-Hemant/2009Sep17/Removed/MVC004912 - Coil Crash
        if (m_section_button[i].enable == TRUE) {

            SectionCoilSetLableForButton(i, m_section_button[i].label);
            SectionCoilUpdateSectionSize(i, &m_section_button[i]);
            SectionCoilSetSensitiveButton(i, true);
            SectionCoilEnableDisableButton(i, true);

            if (m_section_button[i].section->enable[m_section_button[i].port]) {

                SectionCoilSetSelectColorForButton(i);

                //+Patni-Hemant/2010Feb15/Added/ACE-Phase#3/CDS Requirement
                //if (m_coil_selection_global.message != IDS_CS_MES_UPPER_1 &&
                if (m_coil_selection_global.message != IDS_CS_MES_NO_POSITION &&
                    //-Patni-Hemant/2010Feb15/Added/ACE-Phase#3/CDS Requirement

                    m_coil_selection_global.message != IDS_CS_MES_UPPER_2) {

                    //+Patni-Hemant/2010Feb15/Modified/ACE-Phase#3/CDS Requirement
                    //CoilSelectionUiMessage(IDS_CS_MES_OK);
                    if (m_coil_selection_global.message != IDS_CS_MES_NO_POSITION) {
                        UpdateMsgWindowWithMsgOK();
                    }

                    //-Patni-Hemant/2010Feb15/Modified/ACE-Phase#3/CDS Requirement
                }

            } else {
                SectionCoilSetDefaultColorForButton(i);
            }

            if (m_select_count == 1 && m_section_button[i].coil->numofsec == 1) {

                SectionCoilSetSelectColorForButton(i);
                m_section_button[i].section->enable[m_section_button[i].port] = TRUE;

                //+Patni-Hemant/2010Feb15/Added/ACE-Phase#3/CDS Requirement
                //if (m_coil_selection_global.message != IDS_CS_MES_UPPER_1 &&
                if (m_coil_selection_global.message != IDS_CS_MES_NO_POSITION &&
                    //-Patni-Hemant/2010Feb15/Added/ACE-Phase#3/CDS Requirement

                    m_coil_selection_global.message != IDS_CS_MES_UPPER_2) {

                    //+Patni-Hemant/2010Feb15/Modified/ACE-Phase#3/CDS Requirement
                    //CoilSelectionUiMessage(IDS_CS_MES_OK);
                    if (m_coil_selection_global.message != IDS_CS_MES_NO_POSITION) {
                        UpdateMsgWindowWithMsgOK();
                    }

                    //-Patni-Hemant/2010Feb15/Modified/ACE-Phase#3/CDS Requirement
                }
            }

            if (m_section_button[i].coil->combine == COMBINE_PERMISSION_CONNECT) {
                l_count ++;
            }

        } else {
            m_section_button[i].position.linkflg = FALSE;
            SectionCoilSetDefaultColorForButton(i);
            SectionCoilSetSensitiveButton(i, false);
        }
    }


    for (i = 0; i < MAX_COIL_BUTTON; i++) {

        if (m_coil_button[i].enable == TRUE) {

            SelectCoilUiSetLableForSelectionCoilIndex(i,
                    CString(m_coil_button[i].label));
            SelectCoilUiSetSensitiveButton(i, true);

            //Added by Hemant On 10/15/2008 7:21:42 PM
            SelectCoilEnableDisableButton(i, true);

            if (m_coil_button[i].coil->combine == COMBINE_PERMISSION_CONNECT) {
                l_count ++;
            }

            if (m_coil_button[i].coil->combine == COMBINE_PERMISSION_CONNECT
                || m_coil_button[i].coil->combine == COMBINE_NOT_TOGETHER
                || m_select_count == 1) {


                //CoilSelectionUiSetSelectColor(CoilSelectionCoilButton[i].w);

                if (true != f_is_move_coil_btn) {
                    SelectCoilUiSetSelectColorForIndex(i);
                    m_coil_button[i].position.sw = TRUE;
                }

                //+Patni-Hemant/2010Feb15/Added/ACE-Phase#3/CDS Requirement
                //if (m_coil_selection_global.message != IDS_CS_MES_UPPER_1 &&
                if (m_coil_selection_global.message != IDS_CS_MES_NO_POSITION &&
                    //-Patni-Hemant/2010Feb15/Added/ACE-Phase#3/CDS Requirement

                    m_coil_selection_global.message != IDS_CS_MES_UPPER_2) {

                    //+Patni-Hemant/2010Feb15/Modified/ACE-Phase#3/CDS Requirement
                    //CoilSelectionUiMessage(IDS_CS_MES_OK);
                    if (m_coil_selection_global.message != IDS_CS_MES_NO_POSITION) {
                        UpdateMsgWindowWithMsgOK();
                    }

                    //-Patni-Hemant/2010Feb15/Modified/ACE-Phase#3/CDS Requirement
                }
            }

        } else {

            SelectCoilUiSetSensitiveButton(i, false);
            SelectCoilUiSetDefaultColorForIndex(i);
            m_coil_button[i].position.sw = FALSE;
        }
    }

    if (l_count > 0) {

        for (i = 0; i < MAX_SECTION_BUTTON; i++) {

            if (m_section_button[i].enable == TRUE &&
                m_section_button[i].coil->combine != COMBINE_PERMISSION_CONNECT) {

                m_section_button[i].section->enable[m_section_button[i].port] = FALSE;
                SectionCoilSetDefaultColorForButton(i) ;
                SectionCoilEnableDisableButton(i, false);
            }
        }

        for (i = 0; i < MAX_COIL_BUTTON; i++) {
            if (m_coil_button[i].enable  == TRUE &&
                m_coil_button[i].coil->combine != COMBINE_PERMISSION_CONNECT) {

                //+Patni-Hemant/2010Feb15/Added/ACE-Phase#3/CDS Requirement

                //					SelectCoilEnableDisableButton(i, false);
                //					SelectCoilUiSetDefaultColorForIndex(i);
                //					m_coil_button[i].position.sw = FALSE;

                //Other than the coil OFF Combine0x01 insensitive to the
                m_coil_button[i].position.sw = FALSE;
                SelectCoilUiSetDefaultColorForIndex(i);
                SelectCoilEnableDisableButton(i, false);
                //-Patni-Hemant/2010Feb15/Added/ACE-Phase#3/CDS Requirement
            }
        }
    }

    //+Patni-Hemant/2010Feb15/Added/ACE-Phase#3/CDS Requirement
    //if( (m_coil_selection_global.message == IDS_CS_MES_OK)	&&
    //	(m_coil_selection_global.message == IDS_CDS_MES_AFTER_CDS) )
    //{
    //Patni-DKH/2010Jun30/Added/TMSC-REDMINE-390
    if (m_select_count > 0) {
        m_uiclassinterfaceforcoilseldialog->UiCheckCombine();
    }

    //}
    //-Patni-Hemant/2010Feb15/Added/ACE-Phase#3/CDS Requirement
}

//***************************Method Header*************************************
//Method Name    : ProcessShowCoilMsg()
//Author         : PATNI / HAR
//Purpose        :
//*****************************************************************************
void CCoilSelectionUtility::ProcessShowCoilMsg(
    VftCoilListInfo_t* f_show_coil
)
{

    LPCTSTR FUNC_NAME = _T("CCoilSelWin::ProcessShowCoilMsg");
    PQM_TRACE_SCOPED(FUNC_NAME);

    if (f_show_coil->num > 0) {
        int l_status = SetPqmListCoil(f_show_coil, TRUE);

        if (l_status == COIL_S_NO_ERROR) {

            CoilSetCoilButton(true);
            CoilMapCoilButton();

        } else if (l_status == COIL_S_OTHER) {

            CoilSetCoilButton(true);
            CoilMapCoilButton();

            //+Patni-Hemant/2010Mar29/Added/ACE-Phase#3/CDS Requirement
            if (m_uiclassinterfaceforcoilseldialog && (GetCoilSelMode() == COIL_SEL_CDS)) {
                m_uiclassinterfaceforcoilseldialog->UpdateWarningMsgWindowWith(IDS_CS_MES_NO_POSITION);
            }

            //-Patni-Hemant/2010Mar29/Added/ACE-Phase#3/CDS Requirement

        } else {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("The data transmitted from the Sequence Queue contains an error."));

            //+Patni-Hemant/2010Apr12/Added/ACE-Phase#3/CDS Requirement
            CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
                CPQMUtility::GetMultiLingualString(_T("IDS_CS_ERR_ILLEGAL_PQM_DATA")),
                MPlus::EH::Information, _T("PQM"), //MEITEC/2010May13/Modified/TMSC-REDMINE-162
                _T("PQM"));
            //-Patni-Hemant/2010Apr12/Added/ACE-Phase#3/CDS Requirement
        }

    } else {
        // Write Log
    }

    m_list_flag = TRUE;

    if (m_connect_coil->num > 0 && m_show_coil_list->num == 0 &&
        m_compare_flag
        && m_coil_selection_global.config.pqmMode != K_SCHEDULING
       ) {
        m_compare_flag = FALSE;

        if (!CompareListCoil()) {

            //   CString l_send_str(_T("The coil selection in the study file does not match ,\
            //      \nthe coil actually connected.,\
            //       \nDo you want to overwrite the coil selection with the coil actually connected?"));


            //MessageBox(NULL,l_str, NULL, MB_OKCANCEL|MB_SYSTEMMODAL);
            CCoilSelectionUtility::DisplayMessageinWarningDialog(
                CommunicationCoilMessage(CPQMUtility::GetMultiLingualString(_T("IDS_COIL_SEL_STUDY_FILE_NOT_MATCHING"))),
                SCAN_WARN_OK_CANCEL_BUTTONS
            );
        }
    }
}


//************************************Method Header****************************
// Method Name  : ProcessSelectCoilMsg()
// Author       : PATNI / HAR
// Purpose      :
//*****************************************************************************
void CCoilSelectionUtility::ProcessSelectCoilMsg(
    const coilSelect_t* f_select_coil
)
{
    LPCTSTR FUNC_NAME = _T("CCoilSelectionUtility::ProcessSelectCoilMsg");
    PQM_TRACE_SCOPED(FUNC_NAME);

    BOOL l_status = FALSE;

    //    __asm int 3;
    if (f_select_coil->num > 0) {

        l_status = UiSetPqmSelectCoil();

        if (!l_status) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("The data transmitted from the Sequence Queue contains an error."));
            //+Patni-Hemant/2010Apr12/Added/ACE-Phase#3/CDS Requirement

            CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
                CPQMUtility::GetMultiLingualString(_T("IDS_CS_ERR_ILLEGAL_PQM_DATA")),
                MPlus::EH::Information, _T("PQM"),
                _T("PQM"));
            //-Patni-Hemant/2010Apr12/Added/ACE-Phase#3/CDS Requirement
        }
    }

    if (m_connect_coil->num > 0 && m_compare_flag
        // PENDING
        //&& m_coil_selection_global.config.pqmMode != K_SCHEDULING
       ) {
        m_compare_flag = FALSE;

        if (!CompareListCoil()) {

            CCoilSelectionUtility::DisplayMessageinWarningDialog(
                CommunicationCoilMessage(CPQMUtility::GetMultiLingualString(_T("IDS_CS_WAR_DISAGREEMENT_CONNECT_DATA"))),
                SCAN_WARN_OK_CANCEL_BUTTONS);
        }
    }
}


//***************************Method Header*************************************
//Method Name    : ProcessConnectCoilMsg()
//Author         : PATNI / HAR + AMT
//Purpose        :
//*****************************************************************************
bool CCoilSelectionUtility::ProcessConnectCoilMsg(
    VftCoilListInfo_t* f_connect_coil,
    CString& error_message
)
{
    LPCTSTR FUNC_NAME = _T("CCoilSelectionUtility::ProcessConnectCoilMsg");
    PQM_TRACE_SCOPED(FUNC_NAME);

    if (f_connect_coil->num > 0 && m_compare_flag  && m_list_flag &&
        m_coil_selection_global.config.pqmMode != K_SCHEDULING) {

        m_compare_flag = FALSE;

        if (!CompareListCoil()) {

            error_message = CommunicationCoilMessage(CPQMUtility::GetMultiLingualString(_T("IDS_CS_WAR_DISAGREEMENT_CONNECT_DATA")));
            return false ;

            //AfxMessageBox(l_str);
            /* コイルリスト表示 */
            // PENDING
            //            XtManageChild(CoilSelectionUiGetWidget(W_DLG_LIST_FD));
            //            /* 警告 */
            //            CoilSelectionUiWarningDialog( str,
            //                coilSelectionCommOkCB,
            //                coilSelectionCommCancelCB,
            //                NULL);
        }
    }

    // PENDING
    //CoilSelectionUiUnfreeze();
    return true;
}


//***************************Method Header*************************************
//Method Name    : CoilCompareList()
//Author         : PATNI / HAR + AMT
//Purpose        :
//*****************************************************************************
BOOL CCoilSelectionUtility::CoilCompareList(
)
{
    LPCTSTR FUNC_NAME = _T("CCoilSelectionUtility::CoilCompareList");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //DB_FUNC_ENTER("[CoilSelectionCoilCheckList]:");

    // Coiled coil to coil selection screen displayed on the screen the list to see if
    //VftCoilListInfo_t l_to_pqm_list_coil = GetterToPqmListCoil();

    CoilSelectionCoil_t* l_coil = NULL;
    int j = -1;

    for (int i = 0; i < m_to_pqm_list_coil.num; i++) {

        //Group coiled coil search for and retrieve information

        l_coil = SearchCoil(m_to_pqm_list_coil.coillist[i].coilID,
                            m_to_pqm_list_coil.coillist[i].portID[0],
                            m_to_pqm_list_coil.coillist[i].channel, FALSE);

        //Coiled coil from the list to find matching coil
        //+Patni-PP/2009Sep14/Modified/cpp test corrections
        if (NULL == l_coil) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("l_coil pointer is NULL"));
            continue;
        }

        //-Patni-PP/2009Sep14/Modified/cpp test corrections

        for (j = 0; j < MAX_PORT; j++) {
            //Skip Back groups other than the coil
            if (l_coil->groupId[0] != COILID_NONE &&
                l_coil->groupId[0] != m_to_pqm_list_coil.coillist[i].coilID) {

                break;
            }

            if ((m_applied_coil[j].coil != NULL) &&
                (m_to_pqm_list_coil.coillist[i].coilID ==
                 m_applied_coil[j].coil->coil_id) &&
                (m_to_pqm_list_coil.coillist[i].portID[0] ==
                 m_applied_coil[j].coil->port) &&
                ((m_to_pqm_list_coil.coillist[i].portID[1]	==
                  m_applied_coil[j].port + '0') ||
                 (m_to_pqm_list_coil.coillist[i].portID[1]	== ' ') ||
                 (m_applied_coil[j].port == 0))) {

                break;
            }
        }

        //If there is no match coil
        if (j == MAX_PORT) {

            return FALSE;
        }
    }

    //Coiled coil to coil select list of screen displayed on the screen to see if
    for (j = 0; j < MAX_PORT; j++) {
        //Coil selected from among the coil to find a match.

        for (i = 0; i < m_to_pqm_list_coil.num; i++) {
            //Is not selected in the navigation
            if (m_applied_coil[j].coil == NULL) {
                break;
            }

            if ((m_to_pqm_list_coil.coillist[i].coilID ==
                 m_applied_coil[j].coil->coil_id) &&
                (m_to_pqm_list_coil.coillist[i].portID[0] ==
                 m_applied_coil[j].coil->port) &&
                ((m_to_pqm_list_coil.coillist[i].portID[1]	==
                  m_applied_coil[j].port + '0') ||
                 (m_to_pqm_list_coil.coillist[i].portID[1]	== ' ') ||
                 (m_applied_coil[j].port == 0))) {

                break;
            }
        }

        //If there is no match coil
        if (i == m_to_pqm_list_coil.num) {

            return FALSE;
        }
    }

    //All coil match
    return TRUE;
}

void CCoilSelectionUtility::InitUtilityFlags(
)
{
    LPCTSTR FUNC_NAME = _T("CCoilSelectionUtility::InitUtilityFlags");
    PQM_TRACE_SCOPED(FUNC_NAME);

    m_compare_flag          = TRUE;
    m_list_flag             = FALSE;

    m_release_point.x = -1;
    m_release_point.y = -1;
    m_before_point.x = -1;

    m_press_point.x = -1;
    m_press_point.y = -1;
    m_before_point.y = -1;

    //  + Himanshu 24 Aug 2008
    m_connect_coil          = NULL;
    m_select_coil_list      = NULL;
    m_show_coil_list        = NULL;

    for (int i = 0; i < MAX_SECTION_BUTTON; i++) {
        memset(&m_section_button[i].coil, 0, sizeof(CoilSelectionCoil_t*));
        m_section_button[i].enable = FALSE;
        m_section_button[i].label = '\0';
        m_section_button[i].port = -1;
        m_section_button[i].position.sw = FALSE;
        memset(&m_section_button[i].section, 0, sizeof(CoilSelectionSection_t*));
        m_section_button[i].w = -1;
    }

    for (i = 0; i < MAX_COIL_BUTTON; i++) {
        memset(&m_coil_button[i].coil, 0, sizeof(CoilSelectionCoil_t*));
        m_coil_button[i].enable = FALSE;
        m_coil_button[i].label = '\0';
        m_coil_button[i].port = -1;
        m_coil_button[i].position.sw = FALSE;
        // memset(&m_coil_button[i].section, 0, sizeof(CoilSelectionSection_t*));
        m_coil_button[i].section = NULL;
        m_coil_button[i].w = -1;
    }

    memset(m_applied_coil, 0,
           sizeof(CoilSelectionSelectCoil_t)*MAX_PORT);
    memset(m_selected_coil, 0,
           sizeof(CoilSelectionSelectCoil_t)*MAX_PORT);

    memset(&m_to_pqm_list_coil, 0, sizeof(VftCoilListInfo_t));
    memset(&m_list_coil_coordinate, 0, sizeof(VftCoilCoordinateListInfo_t));
    m_select_count = 0;

    if (!m_connect_coil) {
        m_connect_coil = new VftCoilListInfo_t;
    }

    memset(m_connect_coil, 0, sizeof(VftCoilListInfo_t));

    if (!m_show_coil_list) {
        m_show_coil_list = new VftCoilListInfo_t;
    }

    memset(m_show_coil_list, 0, sizeof(VftCoilListInfo_t));

    if (!m_select_coil_list) {
        m_select_coil_list = new coilSelect_t;
    }

    memset(m_select_coil_list, 0, sizeof(coilSelect_t));
    //  - Himanshu 24 Aug 2008

    // + Reset Selection AMT
    // Reset enable[] of section record as to reset previous selection

    if (m_head != NULL) {

        DllT* l_sec_trav = NULL;

        for (DllT* l_trav = m_head; l_trav != NULL; l_trav = l_trav->next) {

            CoilSelectionCoil_t* l_coil = (CoilSelectionCoil_t*)l_trav->item;

            if (l_coil->section != NULL) {

                for (l_sec_trav = l_coil->section; l_sec_trav != NULL;
                     l_sec_trav = l_sec_trav->next) {

                    CoilSelectionSection_t* l_section =
                        (CoilSelectionSection_t*)l_sec_trav->item;
                    memset(l_section->enable, 0, sizeof(Boolean) * 8);
                }
            }
        }
    }

    // - Reset Selection AMT
}


void CCoilSelectionUtility::CoilSelectionClearPqmListCoilMenu()
{
    int i = 0;	//Patni-PP/2009May28/Added/SU14 CPP test initialization corrections

    for (i = 0; i < MAX_PORT; i++) {

        m_applied_coil[i].coil = NULL;
        m_selected_coil[i].coil = NULL;
    }
}


//+Patni-Hemant/2010Mar4/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : CoilSelectionCoilSetFreePosition
// Author       : PATNI/ HEMANT
// Purpose      : Repositioning free coil
//***********************************************************************************
void CCoilSelectionUtility::CoilSelectionCoilSetFreePosition(
    const int direction,		/* Movement direction */
    const int	step,			/* Stage move */
    const bool f_is_move_coil_btn				/*=false*/
)
{
    LPCTSTR FUNC_NAME = _T("CCoilSelectionUtility::CoilSelectionCoilSetFreePosition");
    PQM_TRACE_SCOPED(FUNC_NAME);

    CoilSelectionSelectCoil_t*	table[MAX_PORT];
    int							index[MAX_PORT];
    int							num_t = 0;

    int l_coilwide = 0, l_coil_overlap = 0, l_location = 0, l_position = 0;
    int	k = 0;
    const int l_total_coil_layers = 3;
    int	buf[l_total_coil_layers][MAX_SECTION] = {0};

    //DB_FUNC_ENTER("[CoilSelectionCoilSetFreePosition]:");

    /* direction  -1:left
     * 			   1:right */
    /* step        1:Upper
     * 			   2:Lower */


    /* Only information table can be moved onto coil */
    for (int i = 0; i < MAX_PORT; i++) {
        if ((m_applied_coil[i].coil != NULL)			&&
            (m_applied_coil[i].coil->location & step)	&&	/*Moving coil stage */
            (m_applied_coil[i].coil->position[0] == -1)	&&	/* Free coil */
            (m_applied_coil[i].position >= 1)) {			/* Coil location has been set */

            const float l_percentage_offset = m_applied_coil[i].float_position - m_applied_coil[i].position;

            if (l_percentage_offset > 0.f) {

                if (direction < 0) {

                    int position = ComputePosition(m_applied_coil[i].coil->location, m_applied_coil[i].position);
                    CoilSelectionSectionButton_t* l_section_btn = &m_section_button[position - 1];
                    l_section_btn->position = l_section_btn->m_org_pos;
                    m_applied_coil[i].float_position = (float)m_applied_coil[i].position;
                    continue;
                }
            }

            int position = ComputePosition(m_applied_coil[i].coil->location, m_applied_coil[i].position);

            DllT* l_trav = m_applied_coil[i].coil->section;
            CoilSelectionSection_t* section = (CoilSelectionSection_t*)(l_trav->item);
            CoilSelectionSectionButton_t* l_section_btn = &m_section_button[position];
            const float l_tot_region = (m_applied_coil[i].coil->numofsec * ABSF(l_section_btn->position.rightX - l_section_btn->position.leftX)) / (float)(m_uiclassinterfaceforcoilseldialog->GetGUISectionWidthDefault() + 1);

            /* They can not move is set to end the exclusion */
            if (((direction > 0)	&&
                 (m_applied_coil[i].position + 1 + l_tot_region - 1 <= MAX_SECTION)) ||
                ((direction < 0)	&&
                 (m_applied_coil[i].position - 1 >= 1))) {


                table[num_t] = &m_applied_coil[i];
                index[num_t] = i;
                num_t++;
            }

        }
    }


    /* Table to move away from the coil to sort in the */
    /* Right: Position a large order */
    /* Left: Position by small */

    int j = 0;
    CoilSelectionSelectCoil_t*	buf_t;
    int buf_i = 0;

    if (num_t > 1) {
        for (i = 0; i < num_t; i++) {
            for (j = num_t - 1; j > i; j--) {
                if (((table[j - 1]->position < table[j]->position) && (direction > 0)) ||	/* right */
                    ((table[j - 1]->position > table[j]->position) && (direction < 0))) {	/* left */
                    buf_t = table[j];
                    table[j]   = table[j - 1];
                    table[j - 1] = buf_t;
                    buf_i = index[j];
                    index[j]   = index[j - 1];
                    index[j - 1] = buf_i;
                }
            }
        }
    }

    /* table to move the coil is stored in*/
    float l_temp_position = 0.f;

    for (i = 0; i < num_t; i++) {
        table[i]->position = table[i]->position + direction;
        l_temp_position = table[i]->float_position;

        for (int t = 0; t < MAX_PORT && i == 0; t++) {
            if (m_applied_coil[t].coil != NULL) {

                if (m_applied_coil[t].coil->location != 0x0000 && m_applied_coil[t].position > 0) {
                    if (m_applied_coil[t].coil->location & LOCATION_UPPER) {
                        /* UPPER */
                        l_location = 0;

                    } else if (m_applied_coil[t].coil->location & LOCATION_LOWER) {
                        /* LOWER */
                        l_location = 1;

                    } else if (m_applied_coil[t].coil->location & LOCATION_CENTER) {
                        /* CENTER */
                        l_location = 2;
                    }

                    if (m_applied_coil[t].coil->location == LOCATION_HEAD) {
                        /* HEAD is fixed at 2 and (HEAD is so special coil) */

                        l_coilwide = 2;

                        if (m_applied_coil[t].coil->numofsec > 2) {

                            CoilSelectionSection_t* l_section = NULL;
                            DllT* l_trav = NULL;

                            for (l_trav = m_applied_coil[t].coil->section;
                                 l_trav != NULL; l_trav = l_trav->next) {

                                l_section = (CoilSelectionSection_t*)l_trav->item;

                                if (l_section && l_section->section >= 7) {
                                    l_coilwide = 3;
                                }
                            }
                        }

                    } else {
                        l_coilwide = (int) GetTotalLablesOccupied(&m_applied_coil[t]);
                    }

                    for (k = 0; k < l_coilwide; k++) {

                        l_position = m_applied_coil[t].position + k  - 1;

                        if (l_position >= 0) {
                            buf[l_location][l_position]++;

                            if (buf[l_location][l_position] >= 2) {
                                /* More than two overlapping */
                                l_coil_overlap = 1;
                            }
                        }
                    }
                }
            }
        }

        if (CheckPosition(m_applied_coil) == COIL_S_ERROR || l_coil_overlap) {
            /* If overlapping is undone */
            table[i]->position = table[i]->position - direction;
            table[i]->float_position = l_temp_position;

        } else {
            /*Study destination location information write to */
            //tag = W_DLG_COUCH1_OM + index[i];
            //XtVaSetValues( CoilSelectionUiGetWidget(tag),
            //			  XmNmenuHistory,	   CoilSelectionPositionButton[index[i]][table[i]->position],
            //			  NULL);

            /* Change the color of the menu position */
            searchSelPosition(table[i]->coil->location & 0x000F);
        }
    }


    CheckPosition(m_selected_coil);				/* Control Aposematism*/
    CoilSetCoilButton(true, step);	/* Select button configuration information section*/
    CoilMapCoilButton(f_is_move_coil_btn);	/*Select button display section */
    SetToPqmListCoil();	/* Pass information set Pqm */

    CheckCombine();		/* Check & Change Color Combine */
}
//-Patni-Hemant/2010Mar4/Added/ACE-Phase#3/CDS Requirement

void CCoilSelectionUtility::ResetAllMarkerSigns()
{

    for (int index = 0; index < MAX_COIL_BUTTON ; index ++) {

        ResetMarkerSign(index) ;
    }
}

//***************************Method Header*************************************
//Method Name    : UiSetToPqmSelectCoil()
//Author         : PATNI / HAR + AMT
//Purpose        :
//*****************************************************************************
void CCoilSelectionUtility::UiSetToPqmSelectCoil(
)
{
    LPCTSTR FUNC_NAME = _T("CCoilSelectionUtility::UiSetToPqmSelectCoil");
    PQM_TRACE_SCOPED(FUNC_NAME);



    //DB_FUNC_ENTER("[CoilSelectionUiSetToPqmSelectCoil]:");

    //Patni-ARD/2009October16/Added/Additional Log#MSA0248-00051, MSA0248-00014
    memset(&m_to_pqm_select_coil, NULL, sizeof(coilSelect_t));

    int j = 0;

    for (int i = 0; i < MAX_SECTION_BUTTON; i++) {

        if (m_section_button[i].section) {

            if (m_section_button[i].section->enable[m_section_button[i].port]
                == TRUE) {

                m_to_pqm_select_coil.num = j + 1;

                m_to_pqm_select_coil.select[j].coilID =
                    m_section_button[i].section->coil_id;

                m_to_pqm_select_coil.select[j].portID[0] =
                    m_section_button[i].coil->port;

                if (m_section_button[i].port > 0) {

                    m_to_pqm_select_coil.select[j].portID[1] =
                        m_section_button[i].port + '0';

                } else {

                    m_to_pqm_select_coil.select[j].portID[1] = ' ';
                }

                m_to_pqm_select_coil.select[j].channel =
                    m_section_button[i].coil->channel;

                m_to_pqm_select_coil.select[j].SectionID =
                    m_section_button[i].section->section_id;

                j ++;
            }
        }
    }

    for (i = 0; i < MAX_COIL_BUTTON; i++) {

        if (m_coil_button[i].position.sw == TRUE) {

            m_to_pqm_select_coil.num = j + 1;

            m_to_pqm_select_coil.select[j].coilID =
                m_coil_button[i].coil->coil_id;

            m_to_pqm_select_coil.select[j].portID[0] =
                m_coil_button[i].coil->port;

            if (m_coil_button[i].port > 0) {

                m_to_pqm_select_coil.select[j].portID[1] =
                    m_coil_button[i].port + '0';

            } else {
                m_to_pqm_select_coil.select[j].portID[1] = ' ';
            }

            m_to_pqm_select_coil.select[j].channel =
                m_coil_button[i].coil->channel;

            if (m_coil_button[i].section != NULL) {
                m_to_pqm_select_coil.select[j].SectionID =
                    m_coil_button[i].section->section_id;
            }

            j ++;
        }
    }
}


//************************************Method Header****************************
// Method Name  : SetToPqmListCoil()
// Author       : PATNI / HAR
// Purpose      :
//*****************************************************************************
void CCoilSelectionUtility::SetToPqmListCoil(
)
{
    LPCTSTR FUNC_NAME = _T("CCoilSelectionUtility::SetToPqmListCoil");
    PQM_TRACE_SCOPED(FUNC_NAME);

    int j = -1, k = -1;

    memset(&m_to_pqm_list_coil, 0, sizeof(VftCoilListInfo_t));
    memset(&m_list_coil_coordinate, 0, sizeof(VftCoilCoordinateListInfo_t));

    //Port is under priority
    for (int i = MAX_PORT - 1; i >= 0; i--) {

        if (m_applied_coil[i].coil != NULL) {

            if (m_applied_coil[i].coil->combine == COMBINE_NOT_TOGETHER) {

                m_to_pqm_list_coil.num	= 1;

                m_to_pqm_list_coil.coillist[0].coilID =
                    m_applied_coil[i].coil->coil_id;

                m_to_pqm_list_coil.coillist[0].portID[0] =
                    m_applied_coil[i].coil->port;

                if (m_applied_coil[i].port > 0) {

                    m_to_pqm_list_coil.coillist[0].portID[1] =
                        m_applied_coil[i].port + '0';

                } else {
                    m_to_pqm_list_coil.coillist[0].portID[1] = ' ';
                }

                m_to_pqm_list_coil.coillist[0].channel =
                    m_applied_coil[i].coil->channel;

                m_to_pqm_list_coil.coillist[0].coordinate =
                    m_applied_coil[i].position;

                if (m_applied_coil[i].coil->groupId[0] != COILID_NONE) {

                    for (j = 1; j < 4; j++) {

                        if (m_applied_coil[i].coil->groupId[j] == COILID_NONE) {
                            break;
                        }

                        m_to_pqm_list_coil.num = 1 + j;

                        m_to_pqm_list_coil.coillist[0 + j].coilID =
                            m_applied_coil[i].coil->groupId[j];

                        m_to_pqm_list_coil.coillist[0 + j].portID[0] =
                            m_to_pqm_list_coil.coillist[0].portID[0];

                        m_to_pqm_list_coil.coillist[0 + j].portID[1] =
                            m_to_pqm_list_coil.coillist[0].portID[1];

                        m_to_pqm_list_coil.coillist[0 + j].channel =
                            m_to_pqm_list_coil.coillist[0].channel;

                        m_to_pqm_list_coil.coillist[0 + j].coordinate =
                            m_to_pqm_list_coil.coillist[0].coordinate +
                            m_applied_coil[i].coil->Groupsec[j];
                    }

                    i = i + j - 1;
                }

                m_list_coil_coordinate.num = 1;
                m_list_coil_coordinate.coordinate_list[0].coordinate = m_applied_coil[i].float_position;
                m_list_coil_coordinate.coordinate_list[0].portID[0] = m_to_pqm_list_coil.coillist[0].portID[0];
                m_list_coil_coordinate.coordinate_list[0].portID[1] = m_to_pqm_list_coil.coillist[0].portID[1];

                return;
            }
        }
    }

    j = 0;

    for (i = 0; i < MAX_PORT; i++) {

        if (m_applied_coil[i].coil != NULL) {

            m_to_pqm_list_coil.num = j + 1;

            m_to_pqm_list_coil.coillist[j].coilID =
                m_applied_coil[i].coil->coil_id;

            m_to_pqm_list_coil.coillist[j].portID[0] =
                m_applied_coil[i].coil->port;

            if (m_applied_coil[i].port > 0) {

                m_to_pqm_list_coil.coillist[j].portID[1] =
                    m_applied_coil[i].port + '0';

            } else {
                m_to_pqm_list_coil.coillist[j].portID[1] = ' ';
            }

            m_to_pqm_list_coil.coillist[j].channel =
                m_applied_coil[i].coil->channel;

            m_to_pqm_list_coil.coillist[j].coordinate =
                m_applied_coil[i].position;

            m_list_coil_coordinate.coordinate_list[j].coordinate =
                m_applied_coil[i].float_position;

            if (m_applied_coil[i].coil->groupId[0] != COILID_NONE) {

                for (k = 1; k < 4; k++) {

                    if (m_applied_coil[i].coil->groupId[k] == COILID_NONE) {

                        break;
                    }

                    m_to_pqm_list_coil.num = 1 + j + k;
                    m_to_pqm_list_coil.coillist[j + k].coilID =
                        m_applied_coil[i].coil->groupId[k];

                    m_to_pqm_list_coil.coillist[j + k].portID[0] =
                        m_to_pqm_list_coil.coillist[j].portID[0];

                    m_to_pqm_list_coil.coillist[j + k].portID[1] =
                        m_to_pqm_list_coil.coillist[j].portID[1];

                    m_to_pqm_list_coil.coillist[j + k].channel =
                        m_to_pqm_list_coil.coillist[j].channel;

                    m_to_pqm_list_coil.coillist[j + k].coordinate =
                        m_to_pqm_list_coil.coillist[j].coordinate +
                        m_applied_coil[i].coil->Groupsec[k];

                    m_list_coil_coordinate.coordinate_list[j + k].coordinate =
                        m_list_coil_coordinate.coordinate_list[j].coordinate +
                        m_applied_coil[i].coil->Groupsec[k];

                }

                j = j + k - 1;
            }

            j ++;
        }
    }

    m_list_coil_coordinate.num = m_to_pqm_list_coil.num;

    for (i = 0; i < m_to_pqm_list_coil.num; i++) {
        m_list_coil_coordinate.coordinate_list[i].portID[0] = m_to_pqm_list_coil.coillist[i].portID[0];
        m_list_coil_coordinate.coordinate_list[i].portID[1] = m_to_pqm_list_coil.coillist[i].portID[1];
    }

}

bool CCoilSelectionUtility::IsLinkedSectionOfGroupedCoil(
    const CoilSelectionSectionButton_t* const lhs,
    const CoilSelectionSectionButton_t* const rhs) const
{
    if (!lhs->IsGroupedCoil(rhs)) {
        return false;
    }

    if (lhs->HasLinkWithSectionIdOf(rhs)) {
        if (lhs->HasSameSectionAndLinkId() || !HasCoilSameSectionIdWithLinkedId(lhs)) {
            return true;
        }
    }

    return false;
}

//************************************Method Header****************************
// Method Name  : IsCoilApplied()
// Author       : iGATE / RJ
// Purpose      : To check is coil applied or not
//*****************************************************************************
bool CCoilSelectionUtility::IsCoilApplied(
)const
{
    for (int iIndex = 0; iIndex < MAX_PORT; iIndex++) {

        if (m_applied_coil[iIndex].coil != NULL) {
            return true;
        }
    }

    return false;
}

//***************************Method Header*************************************
//Method Name    : HasDisallowableCombineCoil()
//Author         : iGATE /	RJ
//Purpose        : Is the selected coil is Disallowable Combine Coil or not
//*****************************************************************************
bool CCoilSelectionUtility::HasDisallowableCombineCoil(
)const
{
    return m_has_disallowable_combine_coil;
}

MPLUSERRORUSERRESPONSE_e CCoilSelectionUtility::DisplayMessageinWarningDialog(
    const CString& f_warn_message,
    const int f_warn_buttonstype,
    const int f_warn_level /*= SCAN_WARN_INFORMATION*/ ,
    const int f_warn_dialogtype /*= SCAN_WARN_SYSMODAL*/
)
{
    const CString f_warn_source = _T("PQM") ;

    HWND l_focussed_handle = GetFocus();

    MPLUSERRORUSERRESPONSE_e l_ret_val = (MPLUSERRORUSERRESPONSE_e) CPQMLogMgr::GetInstance()->DisplayMessageinWarningDialog(  // SM4 Himanshu COIL_DEFECT_PQM_HANG 20090126
            f_warn_message,
            f_warn_source,
            f_warn_level,//MPlus::EH::Crucial,
            f_warn_dialogtype, //MPlus::EH::SysModal,
            f_warn_buttonstype//MPlus::EH::OkCancel
                                         ) ;
    SetFocus(l_focussed_handle);

    return l_ret_val;
}




int CCoilSelectionUtility::GetBaseIndexFor(
    const CoilTypes coiltypes
)
{

    switch (coiltypes) {


        case CoilType_G :
            return 9  ;

        case CoilType_L :
            return (GetBaseIndexFor(CoilType_A) + ACoilItems_TOTAL) ;

        case CoilType_A :
            return 0  ;
    }

    return -1;
}

int CCoilSelectionUtility::GetBaseIndexForCoilButtonFor(
    const CoilTypes coiltypes
)
{

    switch (coiltypes) {


        case CoilType_G :

            return 0  ;

        case CoilType_L :

            return (GetBaseIndexForCoilButtonFor(CoilType_G) + GCoilItems_TOTAL)  ;
            //return (GetBaseIndexFor(CoilType_A) + ACoilItems_TOTAL) ;

        case CoilType_A :

            return (GetBaseIndexForCoilButtonFor(CoilType_L) + LCoilItems_TOTAL) ;
            //return 0 ;
    }

    return -1;
}

// + Himanshu SM2
//************************************Method Header**********************************
// Method Name  : GetHed()
// Author       : PATNI / HAR
// Purpose      :
//***********************************************************************************
DllT* CCoilSelectionUtility::GetHead(
)const
{
    return m_head;
}


//***************************Method Header*************************************
//Method Name    : SetHead()
//Author         : PATNI / HAR
//Purpose        :
//*****************************************************************************
void CCoilSelectionUtility::SetHead(
    DllT* f_head
)
{
    m_head = f_head;
}



//************************************Method Header**********************************
// Method Name  : GetTrestleHead()
// Author       : PATNI / HAR
// Purpose      :
//***********************************************************************************
DllT* CCoilSelectionUtility::GetTrestleHead(
)const
{
    return m_coil_trestle_head;
}


//***************************Method Header*************************************
//Method Name    : SetTrestleHead()
//Author         : PATNI / HAR
//Purpose        :
//*****************************************************************************
void CCoilSelectionUtility::SetTrestleHead(
    DllT* f_trestle_head
)
{
    m_coil_trestle_head = f_trestle_head;
}


//************************************Method Header**********************************
// Method Name  : GetCouchHead()
// Author       : PATNI / HAR
// Purpose      :
//***********************************************************************************
DllT* CCoilSelectionUtility::GetCouchHead(
    const int f_index
)const
{
    LPCTSTR FUNC_NAME = _T("CCoilSelectionUtility::GetCouchHead");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_coil_couch_head) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_coil_couch_head pointer is NULL"));

        return NULL;

    } else {
        //-Patni-DKH/2009Aug24/Modified/cpp test corrections
        return m_coil_couch_head[f_index];
    }//Patni-DKH/2009Aug24/Modified/cpp test corrections
}


//***************************Method Header*************************************
//Method Name    : SetCouchHead()
//Author         : PATNI / HAR
//Purpose        :
//*****************************************************************************
void CCoilSelectionUtility::SetCouchHead(
    DllT* f_couch_head , const int index
)
{
    m_coil_couch_head[index] = f_couch_head;
}

//************************************Method Header**********************************
// Method Name  : GetLinearHead()
// Author       : PATNI / HAR
// Purpose      :
//***********************************************************************************
DllT* CCoilSelectionUtility::GetLinearHead(
    const int f_index
)const
{

    LPCTSTR FUNC_NAME = _T("CCoilSelectionUtility::GetLinearHead");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_coil_liner_head) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_coil_liner_head pointer is NULL"));

        return NULL;

    } else {
        //-Patni-DKH/2009Aug24/Modified/cpp test corrections
        return m_coil_liner_head[f_index];
    }//Patni-DKH/2009Aug24/Modified/cpp test corrections

}


//***************************Method Header*************************************
//Method Name    : SetLinearHead()
//Author         : PATNI / HAR
//Purpose        :
//*****************************************************************************
void CCoilSelectionUtility::SetLinearHead(
    DllT* f_linear_head, const int index
)
{
    m_coil_liner_head[index] = f_linear_head;
}


//  + Himanshu 24 Aug 2008
//***************************Method Header*************************************
//Method Name    : GetConnectCoil()
//Author         : PATNI / HAR
//Purpose        :
//*****************************************************************************
VftCoilListInfo_t* CCoilSelectionUtility::GetConnectCoil(
)const
{
    return m_connect_coil;
}


//***************************Method Header*************************************
//Method Name    : SetConnectCoil()
//Author         : PATNI / HAR
//Purpose        :
//*****************************************************************************
void CCoilSelectionUtility::SetConnectCoil(
    const VftCoilListInfo_t* f_connect_coil
)const
{
    // shift to ctor
    //    if (!m_connect_coil) {
    //        m_connect_coil = new VftCoilListInfo_t;
    //    }
    //
    memcpy(m_connect_coil, f_connect_coil, sizeof(VftCoilListInfo_t));
}

//***************************Method Header*************************************
//Method Name    : GetShowCoilList()
//Author         : PATNI / HAR
//Purpose        :
//*****************************************************************************
VftCoilListInfo_t* CCoilSelectionUtility::GetShowCoilList(
)const
{
    return m_show_coil_list;
}


//***************************Method Header*************************************
//Method Name    : SetShowCoilList()
//Author         : PATNI / HAR
//Purpose        :
//*****************************************************************************
void CCoilSelectionUtility::SetShowCoilList(
    const VftCoilListInfo_t* f_show_coil_list
)const
{
    // shift to ctor
    //    if (!m_show_coil_list) {
    //        m_show_coil_list = new VftCoilListInfo_t;
    //    }

    memcpy(m_show_coil_list, f_show_coil_list, sizeof(VftCoilListInfo_t));
}

void CCoilSelectionUtility::SetCoordinateList(
    const VftCoilCoordinateListInfo_t* f_coordinate_list
)
{

    memcpy(&m_list_coil_coordinate, f_coordinate_list, sizeof(VftCoilCoordinateListInfo_t));
}


//************************************Method Header**********************************
// Method Name  : GetToPqmSelectCoil()
// Author       : PATNI / HAR
// Purpose      :
//***********************************************************************************
coilSelect_t CCoilSelectionUtility::GetToPqmSelectCoil(
)const
{
    return m_to_pqm_select_coil;
}


//+Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : GetCoilSelMode
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
COIL_SEL_MODE CCoilSelectionUtility::GetCoilSelMode(
) const  	//Patni-Hemant/2010May6/Added/JaFT/SM_PH3_CDR-CDS_CDR_11
{
    return m_coil_selection_mode ;
}
//-Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement


//+Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : SetCoilSelMode
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCoilSelectionUtility::SetCoilSelMode(
    const COIL_SEL_MODE f_coil_selection_mode
)
{

    m_coil_selection_mode = f_coil_selection_mode ;
}
//-Patni-Hemant/2010Feb8/Added/ACE-Phase#3/CDS Requirement


CPoint CCoilSelectionUtility::GetPressPoint(

)
{
    return m_press_point ;
}


//***************************Method Header*************************************
//Method Name    : SetPressPoint()
//Author         : PATNI / AMT
//Purpose        : Sets mouse press point for section buttons
//*****************************************************************************
void CCoilSelectionUtility::SetPressPoint(
    const CPoint& f_press_point
)
{

    m_press_point.x = f_press_point.x;
    m_press_point.y = f_press_point.y;
}

CPoint CCoilSelectionUtility::GetReleasePoint(

)
{
    return m_release_point ;
}


//***************************Method Header*************************************
//Method Name    : SetPressPoint()
//Author         : PATNI / AMT
//Purpose        : Sets mouse press point for section buttons
//*****************************************************************************
void CCoilSelectionUtility::SetReleasePoint(
    const CPoint& f_release_point
)
{
    m_release_point.x = f_release_point.x;
    m_release_point.y = f_release_point.y;
}

//***************************Method Header*************************************
//Method Name    : GetSelectedCoil()
//Author         : PATNI /
//Purpose        :
//*****************************************************************************

CoilSelectionSelectCoil_t* CCoilSelectionUtility::GetSelectedCoil(
    const int f_index
)
{
    return &m_selected_coil[f_index];
}

//***************************Method Header*************************************
//Method Name    : SetSelectedCoil()
//Author         : PATNI /
//Purpose        :
//*****************************************************************************
void CCoilSelectionUtility::SetSelectedCoil(
    const CoilSelectionSelectCoil_t* f_coil_selection,
    const int f_index
)
{
    if (!IsValidCoilIndex(f_index)) {

        ASSERT(FALSE);
        return ;
    }

    // + Temp code
    if (f_coil_selection == NULL) {
        m_selected_coil[f_index].coil =  NULL;
        m_selected_coil[f_index].port =  0;
        m_selected_coil[f_index].position =  0;
        return ;
    }

    // - Temp code

    m_selected_coil[f_index].coil =  f_coil_selection->coil;
    m_selected_coil[f_index].port =  f_coil_selection->port;
    m_selected_coil[f_index].position =  f_coil_selection->position;
}

//***************************Method Header*************************************
//Method Name    : GetAppliedCoil()
//Author         : iGATE / RJ
//Purpose        : Gets the applied coil
//*****************************************************************************
CoilSelectionSelectCoil_t* CCoilSelectionUtility::GetAppliedCoil(
    const int f_index)
{
    return &m_applied_coil[f_index];
}

//***************************Method Header*************************************
//Method Name    : SetAppliedCoil()
//Author         : iGATE / RJ
//Purpose        : Sets the selected coils as applied
//*****************************************************************************
void CCoilSelectionUtility::SetAppliedCoil(
    const bool f_cancel /*= false*/
)
{
    for (int i = 0; i < MAX_PORT; i++) {

        m_applied_coil[i].coil =  m_selected_coil[i].coil;
        m_applied_coil[i].port =  m_selected_coil[i].port;
        m_applied_coil[i].position =  m_selected_coil[i].position;

        if (f_cancel)
            m_applied_coil[i].float_position =  m_selected_coil[i].float_position;
    }
}

VftCoilListInfo_t* CCoilSelectionUtility::GetterToPqmListCoil(
)
{
    return &m_to_pqm_list_coil;
}

VftCoilCoordinateListInfo_t* CCoilSelectionUtility::GetPqmCordinateListInfo()
{

    return &m_list_coil_coordinate;
}

//***************************Method Header*************************************
//Method Name    : GetSectionButton()
//Author         : PATNI /
//Purpose        :
//*****************************************************************************
CoilSelectionSectionButton_t* CCoilSelectionUtility::GetSectionButton(
    const int f_index
)
{
    return &m_section_button[f_index];
}


//***************************Method Header*************************************
//Method Name    : GetCoilButton()
//Author         : PATNI /
//Purpose        :
//*****************************************************************************
CoilSelectionSectionButton_t* CCoilSelectionUtility::GetCoilButton(
    const int f_index
)
{
    return &m_coil_button[f_index];
}


MarkerSignEnum CCoilSelectionUtility::GetMarkerSignForIndex(
    const int f_coil_button_index
)const
{
    if (f_coil_button_index >= 0  && f_coil_button_index < MAX_COIL_BUTTON) {

        return m_marker_sign[f_coil_button_index] ;
    }

    return MarkerSign_Invalid;

}


CoilSelectionGlobal_t* CCoilSelectionUtility::GetCoilSelectionGlobal(
)
{

    return &m_coil_selection_global;
}

//***************************Method Header*************************************
//Method Name    : GetSelectedCoil()
//Author         : iGate / RJ
//Purpose        : Returns coils selected in the coil list dialog
//*****************************************************************************
CoilSelectionSelectCoil_t* CCoilSelectionUtility::GetSelectedCoil(
)
{
    return m_selected_coil;
}

//***************************Method Header*************************************
//Method Name    : SetPqmListCoil()
//Author         : PATNI / HAR
//Purpose        :
//*****************************************************************************
int CCoilSelectionUtility::SetPqmListCoil(
    VftCoilListInfo_t* f_list_coil,
    const BOOL f_flag
)
{
    LPCTSTR FUNC_NAME = _T("CCoilSelectionUtility::SetPqmListCoil");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //DB_FUNC_ENTER("[CoilSelectionSetPqmListCoil]:");

    //+Patni-Hemant/2010Feb15/Commented/ACE-Phase#3/CDS Requirement
    /*
    //PositionFree coil
    for (i = 0; i < f_list_coil->num; i++) {

        //+Patni-DKH/2009Aug24/Modified/cpp test corrections
        if (NULL == f_list_coil) {
            CPQMLogMgr::GetInstance()->WriteTraceLog(DOMAIN_NAME,
                    USER_FUNC_MARKER,
                    _T("CCoilSelectionUtility::SetPqmListCoil"),
                    _T("f_list_coil pointer is NULL"));
            continue;
        }

        //-Patni-DKH/2009Aug24/Modified/cpp test corrections
        l_select_coil = SearchCoil(f_list_coil->coillist[i].coilID,
                                   f_list_coil->coillist[i].portID[0],
                                   f_list_coil->coillist[i].channel,
                                   TRUE);

        if (l_select_coil != NULL && l_select_coil->position[0] == -1) {
            n   = i;
            flg++;
        }
    }

    if (flg == 1) {

        l_select_coil = SearchCoil(f_list_coil->coillist[n].coilID,
                                   f_list_coil->coillist[n].portID[0],
                                   f_list_coil->coillist[n].channel,
                                   TRUE);

        if (l_select_coil != NULL && f_list_coil->coillist[n].coordinate <= 0) {

            f_list_coil->coillist[n].coordinate = l_select_coil->position[1];

            CoilSelectionUiMessage(IDS_CS_MES_UPPER_1);
        }
    }
    */
    //-Patni-Hemant/2010Feb15/Commented/ACE-Phase#3/CDS Requirement

    if (f_flag) {
        //  + Himanshu 24 Aug 2008

        m_to_pqm_list_coil = *f_list_coil;
        //memcpy(&m_to_pqm_list_coil,f_list_coil, sizeof(VftCoilListInfo_t)*12);

        //  - Himanshu 24 Aug 2008
    }

    int n = 0;

    status_t l_return = COIL_S_NO_ERROR;

    for (int i = 0; i < m_show_coil_list->num; i++) {

        SetPqmListCoilMenu(m_show_coil_list, m_applied_coil, i, &n, f_flag);
    }

    for (i = 0; i < f_list_coil->num; i++) {

        switch (SetPqmListCoilMenu(f_list_coil, m_selected_coil, i, &n, f_flag)) {

            case COIL_S_ERROR :
                return COIL_S_ERROR;

            case COIL_S_OTHER :
                l_return = COIL_S_OTHER;

        }
    }

    return l_return;
}

void CCoilSelectionUtility::SetPQMMode(
    const int f_mode
)
{

    m_coil_selection_global.config.pqmMode = f_mode ;
}


void CCoilSelectionUtility::SetUIClassInterfaceForCoilSelDialog(
    CUIClassInterfaceForCoilSelDialog* uiclassinterfaceforcoilseldialog
)
{

    m_uiclassinterfaceforcoilseldialog = uiclassinterfaceforcoilseldialog ;
}


void CCoilSelectionUtility::SetUIClassInterfaceForCoilListDialog(
    CUIClassInterfaceForCoilListDialog* coillistdialog_interface
)
{

    m_coillistdialog_interface = coillistdialog_interface;
}


//***************************Method Header*************************************
//Method Name    : SetCoilChannels()
//Author         : PATNI / AMT
//Purpose        : Sets channel informations.
//*****************************************************************************
void CCoilSelectionUtility::SetCoilChannels(
    const int f_VAP_channels,
    const int f_VFEP_channels
)
{

    m_coil_selection_global.coil.VAPChannels = f_VAP_channels ;
    m_coil_selection_global.coil.channelPerVFEP = f_VFEP_channels;
    m_coil_selection_global.coil.numChannels = f_VAP_channels * f_VFEP_channels;
}

//***************************Method Header*************************************
//Method Name    : SetPortForLinerCoil

//Author         : PATNI / AMT
//Purpose        : Sets Port
//*****************************************************************************
bool CCoilSelectionUtility::SetPortForLinerCoil(
    CoilSelectionSelectCoil_t* f_coil /*= NULL*/
)
{
    if (NULL == f_coil) {

        if (m_selected_coil[7].coil != NULL) m_selected_coil[7].port = 1;

        if (m_selected_coil[8].coil != NULL) m_selected_coil[8].port = 2;

    } else {

        if (f_coil[7].coil != NULL) f_coil[7].port = 1;

        if (f_coil[8].coil != NULL) f_coil[8].port = 2;
    }

    return true;
}

void CCoilSelectionUtility::SetBeforePoint(
    const CPoint& f_before_point
)
{
    m_before_point.x = f_before_point.x;
    m_before_point.y = f_before_point.y;
}

//***************************Method Header*************************************
//Method Name    : SetSelectCoilList()
//Author         : PATNI / HAR
//Purpose        :
//*****************************************************************************
void CCoilSelectionUtility::SetSelectCoilList(
    const coilSelect_t* f_select_coil_list
)const
{
    // shift to ctor
    //    if (!m_select_coil_list) {
    //        m_select_coil_list = new coilSelect_t;
    //    }
    //
    memcpy(m_select_coil_list, f_select_coil_list, sizeof(coilSelect_t));
}
//  - Himanshu 24 Aug 2008

sCSConfigParams* CCoilSelectionUtility::GetCSConfigParams(
)
{
    return &m_coilsel_configparams;

}

void CCoilSelectionUtility::SetCSConfigParams(const sCSConfigParams& f_scsconfigparams)
{
    m_coilsel_configparams = f_scsconfigparams;
}


//************************************Method Header****************************
//Method Name   : searchSelPosition
//Author        : PATNI/AMT
//Purpose       :
//****************************************************************************/
void CCoilSelectionUtility::searchSelPosition(const int f_location)const
{
    int     i = -1, l = -1;
    BOOL	l_sel_pos[16] = {0};	//Patni-PP/2009May28/Added/SU14 CPP test initialization corrections

    CoilSelectionSection_t*  l_sec_t = NULL;
    register DllT*			l_dll = NULL;

    memset(l_sel_pos, 0, sizeof(l_sel_pos));

    int l_sec = 0;

    for (i = 0; i < MAX_COUCH_PORT + MAX_LINEAR_PORT; i++) {

        if (m_applied_coil[i].coil != NULL) {

            if ((m_applied_coil[i].coil->location) == (f_location)
                && m_applied_coil[i].position != 0) {

                for (l_dll = m_applied_coil[i].coil->section;
                     l_dll != NULL; l_dll = l_dll->next) {

                    l_sec_t = (CoilSelectionSection_t*)l_dll->item;
                    l_sec = l_sec_t->section;

                    if (m_applied_coil[i].coil->location == LOCATION_HEAD) {

                        l_sec = (l_sec_t->section / 2) + (l_sec_t->section % 2);
                    }

                    l_sel_pos[m_applied_coil[i].position + l_sec - 1] = TRUE;

                }
            }
        }
    }

    //+REDMINE-774  //Redundent
    /*
    for (l = 0; l < MAX_COUCH_PORT + MAX_LINEAR_PORT; l++) {

        if (m_applied_coil[l].coil != NULL) {

            if ((m_applied_coil[l].coil->location) == (f_location)) {

                int n = -1;

                for (n = 1; n <= MAX_SECTION; n++) {

                    if (l_sel_pos[n] == TRUE) {
                        //PENDING
                        //CoilSelectionUiSetMenubtnSelectColor(CoilSelectionPositionButton[l][n]);

                    } else {
                        //PENDING
                        //CoilSelectionUiSetMenubtnDefaultColor(CoilSelectionPositionButton[l][n]);
                    }
                }
            }
        }
    }
    */
    //-REDMINE-774
}

//***************************Method Header*************************************
//Method Name    : CheckPort()
//Author         : PATNI / HAR
//Purpose        :
//*****************************************************************************
//Patni-AMT/2009Oct27/Modified/MCM0229-00006
status_t CCoilSelectionUtility::CheckPort(
)const
{
    LPCTSTR FUNC_NAME = _T("CCoilSelectionUtility::CheckPort");
    PQM_TRACE_SCOPED(FUNC_NAME);

    int     j = -1;
    int     buf[MAX_COUCH_PORT] = {0};

    for (int i = 0; i < MAX_COUCH_PORT; i++) {

        for (j = 0; j < MAX_COUCH_PORT; j++) {

            if (m_selected_coil[i].coil != NULL && m_selected_coil[j].coil != NULL
                && i != j) {

                if (m_selected_coil[i].coil->coil_id ==
                    m_selected_coil[j].coil->coil_id) {

                    buf[i] = 1;
                }
            }
        }
    }

    status_t    l_rtn = COIL_S_NO_ERROR;

    for (i = 0; i < MAX_COUCH_PORT; i++) {

        if (buf[i] == 1) {
            if (m_selected_coil[i].port == 0) {
                //PENDING
                //				//CoilSelectionUiSetMenuWarningColor(
                //				//CoilSelectionUiGetWidget(W_DLG_PORT1_OM + i));

                //Patni-AMT/2009Oct27/Modified/MCM0229-00006
                l_rtn = COIL_S_NOPROCESS;
            }
        }
    }

    //Patni-AMT/2009Oct27/Modified/MCM0229-00006
    return l_rtn;
}



//***************************Method Header*************************************
//Method Name    : SearchCoil()
//Author         : PATNI / HAR
//Purpose        :
//*****************************************************************************
CoilSelectionCoil_t* CCoilSelectionUtility::SearchCoil(
    const int f_id,
    const char f_port,
    const char f_channel,
    const BOOL f_flag
)
{
    CoilSelectionCoil_t* l_select_coil = NULL;

    DllT* l_trav = m_head;

    for (; l_trav != NULL; l_trav = l_trav->next) {

        l_select_coil = (CoilSelectionCoil_t*)l_trav->item;

        if (f_flag && !l_select_coil->enable) {
            continue;
        }

        if (f_channel == NULL || f_channel == ' ') {

            if (l_select_coil->coil_id == f_id && l_select_coil->port == f_port) {

                return l_select_coil;
            }

        } else {
            if (l_select_coil->coil_id == f_id
                && l_select_coil->port == f_port
                && l_select_coil->channel == f_channel) {

                return l_select_coil;
            }
        }
    }

    return NULL;
}


void CCoilSelectionUtility::OffsetButtonAsPerSize(
    CoilSelectionSelectCoil_t* f_selected_coil,
    CoilSelectionSectionButton_t* f_section_btn,
    const bool f_offset
)
{
    if (f_offset) {

        const float l_percentage_offset = f_selected_coil->float_position - f_selected_coil->position;

        if (l_percentage_offset > 0.f) {

            const int l_width = (int)((f_section_btn->position.rightX - f_section_btn->position.leftX) * l_percentage_offset) ;

            if (COUCH_POSITION_LEFT == m_uiclassinterfaceforcoilseldialog->GetCouchAxis()) {
                f_section_btn->position.leftX = f_section_btn->position.leftX + l_width;
                f_section_btn->position.rightX = f_section_btn->position.rightX + l_width;

            } else {
                f_section_btn->position.leftX = f_section_btn->position.leftX - l_width;
                f_section_btn->position.rightX = f_section_btn->position.rightX - l_width;
            }
        }

    } else {
        f_section_btn->position = f_section_btn->m_org_pos;
        f_selected_coil->float_position = (float)f_selected_coil->position ;
    }
}

//***************************Method Header*************************************
//Method Name    : CompareListCoil()
//Author         : PATNI / HAR + AMT
//Purpose        :
//*****************************************************************************
BOOL CCoilSelectionUtility::CompareListCoil(
)
{
    LPCTSTR FUNC_NAME = _T("CCoilSelectionUtility::CompareListCoil");
    PQM_TRACE_SCOPED(FUNC_NAME);

    int	j = -1, k = -1, l = -1;

    CoilSelectionCoil_t* l_coil_select = NULL;
    CoilSelectionCoil_t* l_error_display_coil = NULL; // SM4 Himanshu COIL_DEFECT_PQM_HANG 20090126

    //DB_FUNC_ENTER("[CoilSelectionCoilCompareListCoil]:");

    ASSERT(m_connect_coil);

    ResetAllMarkerSigns();

    //+Patni-Hemant/2009Mar05/ADDED/ACE-1/, ACE1, For Venus coil
    int l_portbit = -1 ;

    CString l_error_load = _T("");

    CString l_coil_label(_T(""));
    CString l_str(_T(""));

    for (int i = 0; i < m_connect_coil->num; i++) {

        l_coil_select = SearchCoil(m_connect_coil->coillist[i].coilID,
                                   m_connect_coil->coillist[i].portID[0],
                                   m_connect_coil->coillist[i].channel,
                                   FALSE);

        if (l_coil_select != NULL) {

            //-------------------------------------------------------------------------
            //+Patni-Hemant/Added/2009_MAR_03/ACE1
            //Project ACE1 , For Venus coil
            /* ポートチェック V9.30 */
            if (l_coil_select->port == CDB_PORT_G) {	/* Aポート */

                //Modifying vantage code
                //l_portbit = 1 << (atoi(&m_connect_coil->coillist[i].portID[1]) - 1);
                char value = m_connect_coil->coillist[i].portID[1];

                l_portbit = 1 << (UTIL::ConvertCharToInt(value) - 1);

            } else if (l_coil_select->port == CDB_PORT_F) {	/* Gポート */
                l_portbit = 1 << 7;
            }

            if ((l_coil_select->port == CDB_PORT_G)
                || (l_coil_select->port == CDB_PORT_F)) {
                if ((l_coil_select->portDef & l_portbit) == NULL) {

                    l_error_load = CPQMUtility::GetMultiLingualString(_T("IDS_ERR_DIFFERENT_PORT"));      // Added By KT/ACE-2/11-06-2009/Multilingualisation

                    char f_error_string[512] = { '\0' };
                    UTIL::Wcs2Mbs(f_error_string, l_error_load, 511);

                    char f_concate_string[512] = { '\0' };
                    CoilSelectionUiStringCat(f_error_string, l_coil_select->label, f_concate_string);

                    char f_port_string[128] = { '\0' };
                    GetCoilPortString(l_coil_select, f_port_string);
                    CoilSelectionUiErrorNAVI(f_concate_string, f_port_string, MPlus::EH::Information);

                    break;
                }
            }

            //-Patni-Hemant/Added/2009_MAR_06/ACE1
            //-------------------------------------------------------------------------


            ///Groups coil connection check

            for (j = 0; j < 4; j++) {

                if (l_coil_select->groupId[j] == COILID_NONE) {
                    break;
                }

                for (k = 0; k < m_connect_coil->num; k++) {

                    if (l_coil_select->groupId[j] ==
                        m_connect_coil->coillist[k].coilID) {

                        break;
                    }
                }

                if (k == m_connect_coil->num) {

                    for (l = 0; l < m_connect_coil->num - i; l++) {

                        m_connect_coil->coillist[i + l] = m_connect_coil->coillist[i + l + 1];
                    }

                    // GroupError
                    // + SM4 Himanshu COIL_DEFECT_PQM_HANG 20090126
                    l_error_display_coil = SearchCoil(l_coil_select->groupId[0], (char) l_coil_select->port, NULL, FALSE);
                    //	l_str.LoadString(IDS_ERR_OFF_GROUP_COIL); // Added By KT/ACE-2/15-06-2009/Multilingualisation
                    l_str =  CPQMUtility::GetMultiLingualString(_T("IDS_ERR_OFF_GROUP_COIL"));    // Added By KT/ACE-2/15-06-2009/Multilingualisation
                    int l_size = strlen(l_error_display_coil->label) + 1;
                    wchar_t* l_label = new wchar_t[l_size];
                    mbstowcs(l_label, l_error_display_coil->label, l_size);
                    l_coil_label.Format(l_str, l_label);

                    //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
                    CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(
                        l_coil_label, MPlus::EH::Information, _T("PQM"),
                        _T("PQM"));

                    DEL_PTR_ARY(l_label)

                    m_connect_coil->num--;
                    break;
                }
            }
        }
    }

    // Connect multiple search coil
    k = 0;
    int l_coil_id[10] = {0};
    memset(l_coil_id, 0, sizeof(l_coil_id));

    char l_port[10] = {0};
    memset(l_port, 0, sizeof(l_port));

    for (i = 0; i < m_connect_coil->num; i++) {

        for (j = 0; j < m_connect_coil->num; j++) {

            if (m_connect_coil->coillist[i].coilID ==
                m_connect_coil->coillist[j].coilID &&
                m_connect_coil->coillist[i].portID[0] ==
                m_connect_coil->coillist[j].portID[0] &&
                i != j) {

                l_coil_id[k] = m_connect_coil->coillist[i].coilID;
                l_port[k]   = m_connect_coil->coillist[i].portID[0];
                k++;
                break;
            }
        }
    }

    int l_connect_flag[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int l_select_flag[10]  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    BOOL l_multi_flag = FALSE;	// Coil for multiple connection
    BOOL l_whole_body_flag = FALSE; // WholeBodyFlg

    for (i = 0; i < m_connect_coil->num; i++) {

        for (j = 0; j < MAX_PORT; j++) {

            if (m_selected_coil[j].coil != NULL &&
                m_connect_coil->coillist[i].coilID ==
                m_selected_coil[j].coil->coil_id &&
                m_connect_coil->coillist[i].portID[0] ==
                m_selected_coil[j].coil->port) {

                for (l = 0; l < k; l++) {

                    if (m_connect_coil->coillist[i].coilID ==
                        l_coil_id[l] &&
                        m_connect_coil->coillist[i].portID[0] == l_port[l]) {
                        l_multi_flag = TRUE;
                    }
                }

                if (l_multi_flag || (m_selected_coil[j].port != 0)) {

                    l_multi_flag = FALSE;

                    if (ChartoInt(m_connect_coil->coillist[i].portID[1])
                        == m_selected_coil[j].port) {

                        l_select_flag[j] = 1;

                        m_connect_coil->coillist[i].coordinate =
                            m_selected_coil[j].position;

                        if (m_selected_coil[j].coil->coil_id == COILID_WHOLE_BODY) {

                            l_whole_body_flag = TRUE;
                        }

                        break;
                    }

                } else {

                    l_select_flag[j] = 1;

                    m_connect_coil->coillist[i].coordinate =
                        m_selected_coil[j].position;

                    if (m_selected_coil[j].coil->coil_id == COILID_WHOLE_BODY) {

                        l_whole_body_flag = TRUE;
                    }

                    break;
                }
            }
        }

        if (j == MAX_PORT) {
            l_connect_flag[i] = 1;
        }
    }

    BOOL l_return = TRUE;    // Return Value

    for (i = 0; i < MAX_PORT; i++) {

        if (l_select_flag[i] == 0 && m_selected_coil[i].coil != NULL) {

            SetMarkerSign(GetConvertedCoilButtonIndexForSelectedCoilIndex(i), MarkerSign_Minus) ;
            l_return = FALSE;
        }
    }

    int l_status = COIL_S_NO_ERROR;

    for (i = 0; i < m_connect_coil->num; i++) {

        if (l_connect_flag[i] == 1 &&
            m_connect_coil->coillist[i].coilID != COILID_WHOLE_BODY) {


            l_status = SetPqmListCoilMenu(m_connect_coil, m_selected_coil, i, &j, TRUE);

            if (l_status != COIL_S_ERROR && l_status != COIL_S_NOPROCESS) {
                //				XmString label = XmStringCreateSimple("+");
                //				XtVaSetValues(CoilSelectionUiGetWidget(W_DLG_CNCT1_LBL + j),
                //								XmNlabelString,				label,
                //								NULL);
                //				XmStringFree(label);

                SetMarkerSign(GetConvertedCoilButtonIndexForSelectedCoilIndex(j), MarkerSign_Plus) ;

                l_return = FALSE;
            }

        } else if (m_connect_coil->num == 1 && l_connect_flag[i] == 1) {

            l_status = SetPqmListCoilMenu(m_connect_coil, m_selected_coil, i, &j, TRUE);

            if (l_status  != COIL_S_ERROR && l_status  != COIL_S_NOPROCESS) {
                //				XmString label = XmStringCreateSimple("+");
                //				XtVaSetValues(CoilSelectionUiGetWidget(W_DLG_CNCT1_LBL + j),
                //								XmNlabelString,				label,
                //								NULL);
                //				XmStringFree(label);

                SetMarkerSign(GetConvertedCoilButtonIndexForSelectedCoilIndex(j) , MarkerSign_Plus) ;

                l_return = FALSE;
            }

        } else if (m_connect_coil->coillist[i].coilID == COILID_WHOLE_BODY) {

            if (!l_whole_body_flag) {
                //WB has been selected not connected to the coil removed from
                //the WB coil SU
                l_whole_body_flag = TRUE;

            } else {

                //WB has been selected to be removed from the WB coil is not
                //connected coil
                l_whole_body_flag = FALSE;
            }
        }
    }

    //Information removed from the WB to connect
    if (l_whole_body_flag) {

        m_connect_coil->num--;

        for (i = 0; i < m_connect_coil->num; i++) {

            m_connect_coil->coillist[i] = m_connect_coil->coillist[i + 1];
        }
    }

    return l_return;

}


//***************************Method Header*************************************
//Method Name    : ChartoInt()
//Author         : PATNI / HAR + AMT
//Purpose        :
//*****************************************************************************
int CCoilSelectionUtility::ChartoInt(
    const char c
)
{
    if ('1'	<= c  &&  c  <=  '9') {
        return c - '0';

    } else {
        return 0;
    }
}


//***************************Method Header*************************************
//Method Name    : CommunicationCoilMessage()
//Author         : PATNI / HAR + AMT
//Purpose        :
//*****************************************************************************
CString CCoilSelectionUtility::CommunicationCoilMessage(
    const CString& f_war_msg
)
{
    LPCTSTR FUNC_NAME = _T("CCoilSelectionUtility::CommunicationCoilMessage");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_show_coil_list) {

        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("m_show_coil_list pointer is NULL"));

        return _T("");
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_connect_coil) {

        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("m_connect_coil pointer is NULL"));

        return _T("");
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    CoilSelectionCoil_t* l_coil_p = NULL;

    CString l_show = _T("");

    for (int i = 0; i < m_show_coil_list->num; i++) {

        l_coil_p = SearchCoil(m_show_coil_list->coillist[i].coilID,
                              m_show_coil_list->coillist[i].portID[0],
                              m_show_coil_list->coillist[i].channel, TRUE);

        if (l_coil_p != NULL) {

            if (l_show.GetLength() <= 0) {
                l_show = l_coil_p->label;

            } else {
                l_show += _T(", ");
                l_show += l_coil_p->label;
            }

        }
    }

    CString  l_connect   = _T("");

    for (i = 0; i < m_connect_coil->num; i++) {
        if (m_connect_coil->coillist[i].portID[0] == CDB_PORT_D  &&
            m_connect_coil->coillist[i].portID[1] == '2') {


            l_coil_p = SearchCoil(m_connect_coil->coillist[i].coilID,
                                  m_connect_coil->coillist[i].portID[0],
                                  CDB_CHANNEL_B, TRUE);

        } else {

            l_coil_p = SearchCoil(m_connect_coil->coillist[i].coilID,
                                  m_connect_coil->coillist[i].portID[0],
                                  m_connect_coil->coillist[i].channel, TRUE);
        }

        if (l_coil_p != NULL) {

            if (l_connect.IsEmpty()) {
                l_connect = l_coil_p->label;

            } else {
                l_connect += _T(", ");
                l_connect += l_coil_p->label;
            }

        }
    }

    return (f_war_msg + l_show + _T(" -> ") + l_connect);
}

//+Patni-Hemant/Added/2009_MAR_06/ACE1
//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCoilSelectionUtility::CoilSelectionUiStringCat(
    char* f_main_string,
    char* f_add,
    char* f_out_string
)
{
    //Commented the vantage code.. by Hemant
    //char		*template = NULL ;
    char l_front[256] = { '\0' };
    char l_back[256] = { '\0' };

    //	bzero(l_front,    sizeof(l_front));
    //	bzero(l_back,     sizeof(l_back));
    //	bzero(f_out_string, sizeof(f_out_string));
    memset(f_out_string, 0 , sizeof(f_out_string));

    if (f_main_string != NULL) {
        if (f_add != NULL) {
            int i = 0, j = 0;//Patni-DKH/2009Aug24/Modified/cpp test corrections

            for (i = 0; i < (int)strlen(f_main_string); i++) {
                if (f_main_string[i] == '%' && f_main_string[i + 1] == 's') {
                    break;

                } else {
                    l_front[i] = f_main_string[i];
                }
            }

            for (i = i + 2; i < (int)strlen(f_main_string); i++) {
                l_back[j] = f_main_string[i];
                j++;
            }

            sprintf(f_out_string, "%s%s%s", l_front, f_add, l_back);
        }

    } else {
        CString temp_str = CPQMUtility::GetMultiLingualString(_T("IDS_COILSTR_CONCAT_ERRROR"));		// Added By KT/ACE-2/15-06-2009/Multilingualisation
        int l_size = temp_str.GetLength() + 1;		// Added By KT/ACE-2/15-06-2009/Multilingualisation
        char* l_temp = new char[l_size];			// Added By KT/ACE-2/15-06-2009/Multilingualisation
        memset(l_temp, 0, sizeof(char) * l_size);   // Added By KT/ACE-2/15-06-2009/Multilingualisation
        wcstombs(l_temp, temp_str, l_size);         // Added By KT/ACE-2/15-06-2009/Multilingualisation
        //	sprintf(f_out_string, "CoilSelectionUiStringCat Error"); // Added By KT/ACE-2/15-06-2009/Multilingualisation
        sprintf(f_out_string, l_temp); // Added By KT/ACE-2/15-06-2009/Multilingualisation

        DEL_PTR_ARY(l_temp)
    }

    return;
}
//-Patni-Hemant/Added/2009_MAR_06/ACE1


//+Patni-Hemant/Added/2009_MAR_06/ACE1
//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCoilSelectionUtility::CoilSelectionUiErrorNAVI(
    char* f_main_string,
    char* f_add_string,
    DWORD f_erro_type /*= MPlus::EH::Crucial*/
)
{
    //Commented the vantage code.. by Hemant
    //char	*template = NULL ;

    CString l_error_to_display;
    char f_out_string[256] = { '\0' };

    //Commented the vantage code.. by Hemant
    //DB_FUNC_ENTER("[CoilSelectionUiErrorNAVI]:");

    //bzero(f_out_string, sizeof(char)*128);

    if (f_main_string != NULL) {
        if (f_add_string != NULL) {
            /* 追加メッセージあり */
            CoilSelectionUiStringCat(f_main_string, f_add_string, f_out_string);

            //Modified the vantage code.. by Hemant
            //template = getenv("GP_TEMPLATE") ;
            //TeeNotify(err_level[level], "CoilSelection",template, pString2);
            UTIL::Mbs2Wcs(&l_error_to_display, f_out_string);
            //

        } else {/* 追加メッセージなし */

            //Modified the vantage code.. by Hemant
            //template = getenv("GP_TEMPLATE") ;
            //TeeNotify(err_level[level], "CoilSelection",template, pString);
            UTIL::Mbs2Wcs(&l_error_to_display, f_main_string);
            //
        }

    } else {/* エラー */

        //Modified the vantage code.. by Hemant
        //sprintf(pString, "CoilSelectionWarningDialog");
        //template = getenv("GP_TEMPLATE") ;
        //TeeNotify(err_level[level], "CoilSelection",template, pString);
        //l_error_to_display = _T("CoilSelectionWarningDialog");  // Added By KT/ACE-2/15-06-2009/Multilingualisation
        l_error_to_display =  CPQMUtility::GetMultiLingualString(_T("IDS_COIL_SELECTION_WARNING"));    // Added By KT/ACE-2/15-06-2009/Multilingualisation
    }

    CPQMLogMgr::GetInstance()->DisplayMessageInErrorView(l_error_to_display, f_erro_type, _T("PQM"), _T("PQM"));

    return ;
}
//-Patni-Hemant/Added/2009_MAR_06/ACE1

//+Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : CoilSelectionOKCancel
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CCoilSelectionUtility::CoilSelectionOKCancel(
    const bool f_ok /*= true*/
)
{

    if (f_ok) {
        coilSelectionCheckOkCB();

    } else {
        coilSelectionCheckCancelCB();
    }

    return f_ok;
}

//************************************Method Header************************************
// Method Name  : coilSelectionCheckOkCB
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCoilSelectionUtility::coilSelectionCheckOkCB(
)
{
    CoilSetCoilButton();
    CoilMapCoilButton();
    SetToPqmListCoil();
    return;
}

//************************************Method Header************************************
// Method Name  : coilSelectionCheckCancelCB
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
inline void CCoilSelectionUtility::coilSelectionCheckCancelCB(
)const
{
    // For future use. if we need to take action on cancel callback.
    return;
}

//************************************Method Header************************************
// Method Name  : UpdateMsgWindowWithMsgOK
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCoilSelectionUtility::UpdateMsgWindowWithMsgOK(
)const
{
    if (m_uiclassinterfaceforcoilseldialog) {
        m_uiclassinterfaceforcoilseldialog->UpdateMsgWindowWithMsgOK();
    }
}

//-Patni-Hemant/2010Mar2/Added/ACE-Phase#3/CDS Requirement


//***************************Method Header*************************************
//Method Name    : IsValidCoilIndex()
//Author         : PATNI /
//Purpose        :
//*****************************************************************************
inline bool CCoilSelectionUtility::IsValidCoilIndex(
    const int f_index
)const
{

    return (f_index >= 0 && f_index <= 9) ;
}



void CCoilSelectionUtility::CoilSelectionUiMessage(
    const int f_message_id
)const
{
    LPCTSTR FUNC_NAME = _T("CCoilSelectionUtility::CoilSelectionUiMessage");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_uiclassinterfaceforcoilseldialog) {

        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("m_uiclassinterfaceforcoilseldialog pointer is NULL"));

        return ;

    } else {
        //-Patni-DKH/2009Aug24/Modified/cpp test corrections
        m_uiclassinterfaceforcoilseldialog->CoilSelectionUiMessage(f_message_id);
        //Patni-DKH/2009Aug24/Modified/cpp test corrections
    }

}

void CCoilSelectionUtility::SelectCoilUiSetSelectColorForIndex(const int f_coil_buttonindex)const
{
    LPCTSTR FUNC_NAME = _T("CCoilSelectionUtility::SelectCoilUiSetSelectColorForIndex");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_uiclassinterfaceforcoilseldialog) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_uiclassinterfaceforcoilseldialog pointer is NULL"));

        return;

    } else {
        //-Patni-DKH/2009Aug24/Modified/cpp test corrections
        m_uiclassinterfaceforcoilseldialog->SelectCoilUiSetSelectColorForIndex(f_coil_buttonindex);
        //Patni-DKH/2009Aug24/Modified/cpp test corrections
    }
}


void CCoilSelectionUtility::SelectCoilSetWarningButtonColorForIndex(const int f_coil_buttonindex)const
{
    LPCTSTR FUNC_NAME = _T("CCoilSelectionUtility::SelectCoilSetWarningButtonColorForIndex");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_uiclassinterfaceforcoilseldialog) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_uiclassinterfaceforcoilseldialog pointer is NULL"));

        return;

    } else {
        //-Patni-DKH/2009Aug24/Modified/cpp test corrections
        m_uiclassinterfaceforcoilseldialog->SelectCoilSetWarningButtonColorForIndex(f_coil_buttonindex);
        //Patni-DKH/2009Aug24/Modified/cpp test corrections
    }

}


void CCoilSelectionUtility::SelectCoilUiSetLableForSelectionCoilIndex(
    const int f_coil_buttonindex,
    const CString& f_lable
)const
{
    LPCTSTR FUNC_NAME = _T("CCoilSelectionUtility::SelectCoilUiSetLableForSelectionCoilIndex");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_uiclassinterfaceforcoilseldialog) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_uiclassinterfaceforcoilseldialog pointer is NULL"));

        return;

    } else {
        //-Patni-DKH/2009Aug24/Modified/cpp test corrections
        m_uiclassinterfaceforcoilseldialog->SelectCoilUiSetLableForSelectionCoilIndex(f_coil_buttonindex, f_lable);
        //Patni-DKH/2009Aug24/Modified/cpp test corrections
    }

}

void CCoilSelectionUtility::SelectCoilUiSetSensitiveButton(
    const int f_coil_buttonindex,
    const bool f_enable
)const
{
    LPCTSTR FUNC_NAME = _T("CCoilSelectionUtility::SelectCoilUiSetSensitiveButton");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_uiclassinterfaceforcoilseldialog) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_uiclassinterfaceforcoilseldialog pointer is NULL"));

        return;

    } else {
        //-Patni-DKH/2009Aug24/Modified/cpp test corrections
        m_uiclassinterfaceforcoilseldialog->SelectCoilUiSetSensitiveButton(f_coil_buttonindex, f_enable);
        //Patni-DKH/2009Aug24/Modified/cpp test corrections
    }
}



void CCoilSelectionUtility::SelectCoilUiSetDefaultColorForIndex(
    const int f_coil_buttonindex
)const
{
    LPCTSTR FUNC_NAME = _T("CCoilSelectionUtility::SelectCoilUiSetDefaultColorForIndex");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_uiclassinterfaceforcoilseldialog) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_uiclassinterfaceforcoilseldialog pointer is NULL"));

        return;

    } else {
        //-Patni-DKH/2009Aug24/Modified/cpp test corrections
        m_uiclassinterfaceforcoilseldialog->SelectCoilUiSetDefaultColorForIndex(f_coil_buttonindex);
        //Patni-DKH/2009Aug24/Modified/cpp test corrections
    }

}
//-----------------------
//For section buttons..
void CCoilSelectionUtility::SectionCoilSetSelectColorForButton(
    const int f_section_buttonindex
)const
{
    LPCTSTR FUNC_NAME = _T("CCoilSelectionUtility::SectionCoilSetSelectColorForButton");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_uiclassinterfaceforcoilseldialog) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_uiclassinterfaceforcoilseldialog pointer is NULL"));

        return;

    } else {
        //-Patni-DKH/2009Aug24/Modified/cpp test corrections
        m_uiclassinterfaceforcoilseldialog->SectionCoilSetSelectColorForButton(f_section_buttonindex);
        //Patni-DKH/2009Aug24/Modified/cpp test corrections
    }
}


void CCoilSelectionUtility::SectionCoilSetDefaultColorForButton(
    const int f_section_buttonindex
)const
{
    LPCTSTR FUNC_NAME = _T("CCoilSelectionUtility::SectionCoilSetDefaultColorForButton");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_uiclassinterfaceforcoilseldialog) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_uiclassinterfaceforcoilseldialog pointer is NULL"));

        return;

    } else {
        //-Patni-DKH/2009Aug24/Modified/cpp test corrections
        m_uiclassinterfaceforcoilseldialog->SectionCoilSetDefaultColorForButton(f_section_buttonindex);
        //Patni-DKH/2009Aug24/Modified/cpp test corrections
    }

}


void CCoilSelectionUtility::SectionCoilSetWarningButtonColor(
    const int f_section_buttonindex
)const
{
    LPCTSTR FUNC_NAME = _T("CCoilSelectionUtility::SectionCoilSetWarningButtonColor");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_uiclassinterfaceforcoilseldialog) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_uiclassinterfaceforcoilseldialog pointer is NULL"));

        return;

    } else {
        //-Patni-DKH/2009Aug24/Modified/cpp test corrections
        m_uiclassinterfaceforcoilseldialog->SectionCoilSetWarningButtonColor(f_section_buttonindex);
        //Patni-DKH/2009Aug24/Modified/cpp test corrections
    }
}

void CCoilSelectionUtility::SectionCoilSetLableForButton(
    const int f_section_buttonindex,
    const CString& f_lable
)const
{
    LPCTSTR FUNC_NAME = _T("CCoilSelectionUtility::SectionCoilSetLableForButton");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_uiclassinterfaceforcoilseldialog) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_uiclassinterfaceforcoilseldialog pointer is NULL"));

        return;

    } else {
        //-Patni-DKH/2009Aug24/Modified/cpp test corrections
        m_uiclassinterfaceforcoilseldialog->SectionCoilSetLableForButton(f_section_buttonindex, f_lable);
        //Patni-DKH/2009Aug24/Modified/cpp test corrections
    }
}

void CCoilSelectionUtility::SectionCoilUpdateSectionSize(
    const int f_section_buttonindex,
    CoilSelectionSectionButton_t* f_section_btn
)const
{



    if (m_uiclassinterfaceforcoilseldialog) {
        m_uiclassinterfaceforcoilseldialog->SectionCoilUpdateSectionSize(f_section_buttonindex, f_section_btn);
    }
}


void CCoilSelectionUtility::SectionCoilSetSensitiveButton(
    const int f_section_buttonindex,
    const bool f_enable
)const
{
    LPCTSTR FUNC_NAME = _T("CCoilSelectionUtility::SectionCoilSetSensitiveButton");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_uiclassinterfaceforcoilseldialog) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_uiclassinterfaceforcoilseldialog pointer is NULL"));

        return;

    } else {
        //-Patni-DKH/2009Aug24/Modified/cpp test corrections
        m_uiclassinterfaceforcoilseldialog->SectionCoilSetSensitiveButton(f_section_buttonindex, f_enable);
        //Patni-DKH/2009Aug24/Modified/cpp test corrections
    }

}


void CCoilSelectionUtility::SectionCoilEnableDisableButton(
    const int f_section_buttonindex,
    const bool f_enable
)const
{
    LPCTSTR FUNC_NAME = _T("CCoilSelectionUtility::SectionCoilEnableDisableButton");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_uiclassinterfaceforcoilseldialog) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_uiclassinterfaceforcoilseldialog pointer is NULL"));

        return;

    } else {
        //-Patni-DKH/2009Aug24/Modified/cpp test corrections
        m_uiclassinterfaceforcoilseldialog->SectionCoilEnableDisableButton(f_section_buttonindex, f_enable);
        //Patni-DKH/2009Aug24/Modified/cpp test corrections
    }

}

void CCoilSelectionUtility::SelectCoilEnableDisableButton(
    const int f_select_buttonindex,
    const bool f_enable
)const
{

    LPCTSTR FUNC_NAME = _T("CCoilSelectionUtility::SelectCoilEnableDisableButton");

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_uiclassinterfaceforcoilseldialog) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_uiclassinterfaceforcoilseldialog pointer is NULL"));

        return;

    } else {
        //-Patni-DKH/2009Aug24/Modified/cpp test corrections
        m_uiclassinterfaceforcoilseldialog->SelectCoilEnableDisableButton(f_select_buttonindex, f_enable);
        //Patni-DKH/2009Aug24/Modified/cpp test corrections
    }

}

void CCoilSelectionUtility::ResetMarkerSign(
    const int f_index
)
{

    if (f_index >= 0  && f_index < MAX_COIL_BUTTON) {

        m_marker_sign[f_index] = MarkerSign_Invalid;
    }
}


//***************************Method Header*************************************
//Method Name    : UiSetPqmSelectCoil()
//Author         : PATNI / HAR + AMT
//Purpose        :
//*****************************************************************************
BOOL CCoilSelectionUtility::UiSetPqmSelectCoil(
)
{
    LPCTSTR FUNC_NAME = _T("CCoilSelectionUtility::UiSetPqmSelectCoil");
    PQM_TRACE_SCOPED(FUNC_NAME);


    //DB_FUNC_ENTER("[CoilSelectionUiSetPqmSelectCoil]:");
    //  + Himanshu 24 Aug 2008
    //coilSelect_t * l_select_coil_list = GetSelectCoilList();
    //  - Himanshu 24 Aug 2008

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == m_select_coil_list) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_select_coil_list pointer is NULL"));

        return FALSE;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections

    int	j = -1;
    BOOL l_flag = FALSE, l_free_flag = FALSE;

    for (int i = 0; i < m_select_coil_list->num; i++) {

        l_flag = FALSE;

        for (j = 0; j < MAX_SECTION_BUTTON; j++) {

            //l_section_button = GetSectionButton(j);

            if (m_section_button[j].coil != NULL &&
                m_section_button[j].coil->position[0] == -1) {

                l_free_flag = TRUE;
            }

            if (m_section_button[j].coil != NULL &&
                m_section_button[j].section != NULL) {

                if (m_select_coil_list->select[i].coilID ==
                    m_section_button[j].section->coil_id &&
                    m_select_coil_list->select[i].channel ==
                    m_section_button[j].coil->channel &&
                    m_select_coil_list->select[i].portID[0] ==
                    m_section_button[j].coil->port &&
                    m_select_coil_list->select[i].SectionID ==
                    m_section_button[j].section->section_id) {

                    if (m_section_button[j].coil->numSCoil != 0 &&
                        m_section_button[j].port != 0) {

                        if (ChartoInt(m_select_coil_list->select[i].portID[1]) ==
                            m_section_button[j].port) {


                            //PENING
                            //CoilSelectionUiSetSelectColor(CoilSelectionSectionButton[j].w);

                            //Modified by Hemant On 11/8/2008 4:10:57 PM, select G coil and atlas head from coillist dlg.
                            //Now select atlas head's any one button and close the coil sel end dialog.
                            //now re open coil dialog.
                            //Doing so and due to this commented code, the GCoil was getting selected
                            //and a coil buttons were not selected for index 0,1,2...
                            //corrected the function call will fix the bug..
                            //
                            //SelectCoilUiSetSelectColorForIndex(j);
                            SectionCoilSetSelectColorForButton(j);

                            m_section_button[j].section->enable[m_section_button[j].port] = TRUE;

                            l_flag = TRUE;

                            //PENDING
                            //+Patni-Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement
                            //if (m_coil_selection_global.message != IDS_CS_MES_UPPER_1) {
                            //CoilSelectionUiMessage(IDS_CS_MES_OK);
                            //}
                            if (m_coil_selection_global.message != IDS_CS_MES_NO_POSITION) {

                                UpdateMsgWindowWithMsgOK();
                            }

                            //-Patni-Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement

                        }

                    } else {

                        // PENDING
                        //CoilSelectionUiSetSelectColor(CoilSelectionSectionButton[j].w);


                        //Modified by Hemant On 11/8/2008 4:10:57 PM, select G coil and atlas head from coillist dlg.
                        //Now select atlas head's any one button and close the coil sel end dialog.
                        //now re open coil dialog.
                        //Doing so and due to this commented code, the GCoil was getting selected
                        //and a coil buttons were not selected for index 0,1,2...
                        //corrected the function call will fix the bug..
                        //
                        //SelectCoilUiSetSelectColorForIndex(j);
                        SectionCoilSetSelectColorForButton(j);


                        m_section_button[j].section->enable[m_section_button[j].port] = TRUE;

                        l_flag = TRUE;

                        //PENDING
                        //+Patni-Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement
                        //                        if (m_coil_selection_global.message != IDS_CS_MES_UPPER_1) {
                        //                            CoilSelectionUiMessage(IDS_CS_MES_OK);
                        //                        }
                        if (m_coil_selection_global.message != IDS_CS_MES_NO_POSITION) {

                            UpdateMsgWindowWithMsgOK();
                        }

                        //-Patni-Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement
                    }

                } else if (m_select_coil_list->select[i].coilID	==
                           m_section_button[j].coil->coil_id) {

                }

            }
        }

        for (j = 0; j < MAX_COIL_BUTTON; j++) {

            //l_coil_button = GetCoilButton(j);

            if (m_coil_button[j].coil != NULL) {

                if (m_select_coil_list->select[i].coilID ==
                    m_coil_button[j].coil->coil_id &&
                    m_select_coil_list->select[i].channel ==
                    m_coil_button[j].coil->channel &&
                    m_select_coil_list->select[i].portID[0]  ==
                    m_coil_button[j].coil->port) {

                    if (m_coil_button[j].coil->numSCoil != 0 &&
                        m_coil_button[j].port != 0) {

                        if (ChartoInt(m_select_coil_list->select[i].portID[1]) ==
                            m_coil_button[j].port) {


                            //CoilSelectionUiSetSelectColor(CoilSelectionCoilButton[j].w);
                            SelectCoilUiSetSelectColorForIndex(j);

                            m_coil_button[j].position.sw = TRUE;

                            //+Patni-Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement
                            //                            if (m_coil_selection_global.message != IDS_CS_MES_UPPER_1) {
                            //                                CoilSelectionUiMessage(IDS_CS_MES_OK);
                            //                            }
                            if (m_coil_selection_global.message != IDS_CS_MES_NO_POSITION) {
                                UpdateMsgWindowWithMsgOK();
                            }

                            //-Patni-Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement

                            l_flag = TRUE;
                        }

                    } else {


                        //CoilSelectionUiSetSelectColor(CoilSelectionCoilButton[j].w);
                        SelectCoilUiSetSelectColorForIndex(j);

                        m_coil_button[j].position.sw = TRUE;

                        //+Patni-Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement
                        //if (m_coil_selection_global.message != IDS_CS_MES_UPPER_1) {
                        //    CoilSelectionUiMessage(IDS_CS_MES_OK);
                        //}
                        if (m_coil_selection_global.message != IDS_CS_MES_NO_POSITION) {
                            UpdateMsgWindowWithMsgOK();
                        }

                        //-Patni-Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement

                        l_flag = TRUE;
                    }
                }
            }
        }

        if (!l_flag) {

            return FALSE;
        }
    }

    if (l_free_flag) {

        //+Patni-Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement
        //if (m_coil_selection_global.message != IDS_CS_MES_UPPER_1) {
        if (m_coil_selection_global.message != IDS_CS_MES_NO_POSITION) {
            //-Patni-Hemant/2010Feb12/Added/ACE-Phase#3/CDS Requirement
            CoilSelectionUiMessage(IDS_CS_MES_UPPER_2);
        }
    }

    return TRUE;
}

bool CCoilSelectionUtility::HasCoilSameSectionIdWithLinkedId(
    const CoilSelectionSectionButton_t* const f_section_btn) const
{
    for (int i = 0; i < MAX_SECTION_BUTTON; i++) {
        if (f_section_btn->HasLinkedSectionOfSameCoil(&m_section_button[i])) {
            return true;
        }
    }

    return false;
}

//+Patni-Hemant/Added/2009_MAR_06/ACE1
//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CCoilSelectionUtility::GetCoilPortString(
    const CoilSelectionCoil_t* f_coil_p,
    char* f_port_string
)
{
    std::string l_port_string("");

    if (f_coil_p->port == CDB_PORT_F) {	/* Gポート */
        l_port_string = "G";

    } else if (f_coil_p->port == CDB_PORT_G) {	/* Aポート */
        if (f_coil_p->portDef & 0x0001) {	/* A1 */
            if (l_port_string.empty()) {
                l_port_string = "A1";

            } else {
                l_port_string += ",A1";
            }
        }

        if (f_coil_p->portDef & 0x0002) {	/* A2 */
            if (l_port_string.empty()) {
                l_port_string = "A2";

            } else {
                l_port_string += ",A2";
            }
        }

        if (f_coil_p->portDef & 0x0004) {	/* A3 */
            if (l_port_string.empty()) {
                l_port_string = "A3";

            } else {
                l_port_string += ",A3";
            }
        }

        if (f_coil_p->portDef & 0x0008) {	/* A4 */
            if (l_port_string.empty()) {
                l_port_string = "A4";

            } else {
                l_port_string += ",A4";
            }
        }

        if (f_coil_p->portDef & 0x0010) {	/* A5 */
            if (l_port_string.empty()) {
                l_port_string = "A5";

            } else {
                l_port_string += ",A5";
            }
        }

        if (f_coil_p->portDef & 0x0020) {	/* A6 */
            if (l_port_string.empty()) {
                l_port_string = "A6";

            } else {
                l_port_string += ",A6";
            }
        }

        if (f_coil_p->portDef & 0x0040) {	/* A7 */
            if (l_port_string.empty()) {
                l_port_string = "A7";

            } else {
                l_port_string += ",A7";
            }
        }
    }

    strcpy(f_port_string , l_port_string.c_str());
}
//-Patni-Hemant/Added/2009_MAR_06/ACE1


//***************************Method Header*************************************
//Method Name    : SetPqmListCoilMenu()
//Author         : PATNI / HAR
//Purpose        :
//*****************************************************************************
//Patni-AMT/2009Oct27/Modified/MCM0229-00006
status_t CCoilSelectionUtility::SetPqmListCoilMenu(
    VftCoilListInfo_t* f_list_coil,
    CoilSelectionSelectCoil_t* f_coil,
    const int f_cnt,
    int* f_no,
    const BOOL f_flag
)
{
    LPCTSTR FUNC_NAME = _T("CCoilSelectionUtility::SetPqmListCoilMenu");
    PQM_TRACE_SCOPED(FUNC_NAME);

    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
    if (NULL == f_list_coil) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("f_list_coil pointer is NULL"));
        return COIL_S_NOPROCESS;
    }

    //-Patni-DKH/2009Aug24/Modified/cpp test corrections


    DllT* Head = NULL;
    int n = 0;
    int befomenu = 0;
    int j = -1;

    switch (f_list_coil->coillist[f_cnt].portID[0]) {

        case CDB_PORT_G:

            for (j = 0; j < MAX_COUCH_PORT; j++) {

                if (f_coil[j].coil == NULL) {
                    n         = j;

                    //+Patni-DKH/2009Aug24/Modified/cpp test corrections
                    if (NULL == f_no) {
                        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                                  _T("f_no pointer is NULL"));
                        break;
                    }

                    //-Patni-DKH/2009Aug24/Modified/cpp test corrections
                    *f_no       = j;
                    Head      = m_coil_couch_head[n];
                    befomenu  = 0;
                    break;
                }
            }

            break;

        case CDB_PORT_D:

            //+Patni-DKH/2009Aug24/Modified/cpp test corrections
            if (NULL == f_no) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("f_no pointer is NULL"));
                break;
            }

            //-Patni-DKH/2009Aug24/Modified/cpp test corrections
            if (f_list_coil->coillist[f_cnt].portID[1] == '1') { //+Patni-HAR/2009April01/Uncomment/Defect#342
                n         = 0;
                *f_no       = MAX_COUCH_PORT;
                Head      = m_coil_liner_head[0];
                befomenu  = MAX_COUCH_PORT;

            } else {
                n         = 1;
                *f_no       = MAX_COUCH_PORT + 1;
                Head      = m_coil_liner_head[1];
                befomenu  = MAX_COUCH_PORT;
            }

            break;

        default:
            n         = 0;

            //+Patni-DKH/2009Aug24/Modified/cpp test corrections
            if (NULL == f_no) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("f_no pointer is NULL"));
                break;
            }

            //-Patni-DKH/2009Aug24/Modified/cpp test corrections
            *f_no       = 9;
            Head      = m_coil_trestle_head;
            befomenu  = MAX_COUCH_PORT + MAX_LINEAR_PORT;
            break;
    }

    if (Head == NULL) {
        *f_no = 0;
        //DB_PRINTF("",("%s:%d CoilSelectionCoilButton_t error\n",__FILE__,__LINE__));
        return COIL_S_ERROR;
    }

    int  port = -1 ;
    CoilSelectionCoilButton_t*   btn_p = NULL , *btn2_p = NULL;
    int l_status = COIL_S_NO_ERROR;

    for (DllT* dll = Head; dll != NULL; dll = dll->next) {

        btn_p = (CoilSelectionCoilButton_t*)dll->item;

        if (btn_p->coil->coil_id  ==  f_list_coil->coillist[f_cnt].coilID
            && btn_p->coil->channel  ==  f_list_coil->coillist[f_cnt].channel
            && btn_p->coil->port	  ==  f_list_coil->coillist[f_cnt].portID[0]
            || (btn_p->coil->coil_id  ==  f_list_coil->coillist[f_cnt].coilID
                && btn_p->coil->port	  ==  f_list_coil->coillist[f_cnt].portID[0]
                && f_list_coil->coillist[f_cnt].channel == ' ')) {

            // PENDING
            //tag = W_DLG_COIL1_OM + befomenu + n;
            //XtVaSetValues( CoilSelectionUiGetWidget(tag),
            //				XmNmenuHistory, 	btn_p->button,
            //				NULL);
            f_coil[btn_p->no].coil = btn_p->coil;

            if (0 <= btn_p->no && btn_p->no < MAX_COUCH_PORT) {

                // Coil Menu
                //coilSelectionSetCoilMenu(btn_p->no);
                //PositionMenu
                //coilSelectionSetPositionMenu(btn_p->no);
                //PortMenu
                //coilSelectionSetPortMenu(btn_p->no, b);
                // Connect message position selection problem@AMT 20080927

                //+Patni-Hemant/2010Apr7/Added/ACE-Phase#3/CDS Requirement
                //m_applied_coil[btn_p->no].position =
                //    (btn_p->coil->position[0] != -1 ? btn_p->coil->position[0] : btn_p->coil->position[1]);
                f_coil[btn_p->no].position =
                    (btn_p->coil->position[0] != -1 ? btn_p->coil->position[0] : 0);
                //-Patni-Hemant/2010Apr7/Added/ACE-Phase#3/CDS Requirement

                //commented by Hemant - it gives error while displyaing connected coil
                //it act sets the port number where we are not required to add to it..
                //				// Connect message position selection problem@AMT 20080927
                //                m_applied_coil[btn_p->no].port = ChartoInt(f_list_coil->coillist[f_cnt].portID[1]);

            } else if (MAX_COUCH_PORT <= btn_p->no &&
                       btn_p->no < MAX_COUCH_PORT + MAX_LINEAR_PORT) { // Linear Port
                //PositionMenu
                //coilSelectionSetPositionMenu(btn_p->no);
                //PortMenu
                //coilSelectionSetPort(btn_p->no);
                //SetPortForLinerCoil(btn_p->no);
                SetPortForLinerCoil(f_coil);
            }


            //port
            if (btn_p->coil->port == CDB_PORT_G && btn_p->coil->numSCoil >= 2) {


                int     flg = -1; //Patni-AMT/2009Oct27/Modified/MCM0229-00006

                for (int x = 0; x < f_cnt; x++) {

                    if (x != f_cnt) {
                        if (f_list_coil->coillist[f_cnt].coilID ==
                            f_list_coil->coillist[x].coilID) {

                            flg = x;
                        }
                    }
                }

                //+Patni-AMT/2009Oct27/Modified/MCM0229-00006
                if (flg != -1) {
                    int  befo = -1;
                    //-Patni-AMT/2009Oct27/Modified/MCM0229-00006

                    port = ChartoInt(f_list_coil->coillist[f_cnt].portID[1]);

                    if (befomenu == 0 && port != 0) {

                        // PENDING
                        //tag  = W_DLG_PORT1_OM + n;
                        //XtVaSetValues( CoilSelectionUiGetWidget(tag),
                        //			  XmNmenuHistory,	  CoilSelectionPortButton[n][port],
                        //			  NULL);
                        //CoilSelectionCoilPortMenuCB( CoilSelectionPortButton[n][port],
                        //							 (XtPointer)n, NULL);

                        f_coil[n].port = port;

                    }

                    for (j = 0; j < n; j++) {

                        if (f_coil[j].coil->coil_id ==
                            f_list_coil->coillist[f_cnt].coilID) {

                            befo = j;
                        }
                    }

                    //Patni-AMT/2009Oct27/Modified/MCM0229-00006
                    if (befo != -1) {

                        port = ChartoInt(f_list_coil->coillist[flg].portID[1]);

                        if (befomenu == 0 && port != 0) {
                            // PENDING
                            //tag  = W_DLG_PORT1_OM + befo;
                            //XtVaSetValues( CoilSelectionUiGetWidget(tag),
                            //XmNmenuHistory,	  CoilSelectionPortButton[befo][port],
                            //NULL);
                            //CoilSelectionCoilPortMenuCB( CoilSelectionPortButton[befo][port],
                            //														 (XtPointer)befo, NULL);

                            f_coil[befo].port = port ;
                        }
                    }
                }
            }

            //position
            if (befomenu <= MAX_COUCH_PORT &&
                f_list_coil->coillist[f_cnt].coordinate != 0) {

                // PENDING
                //tag = W_DLG_COUCH1_OM + befomenu + n;
                //XtVaSetValues( CoilSelectionUiGetWidget(tag),
                //			  XmNmenuHistory,	  CoilSelectionPositionButton[n][listCoil->coillist[i].coordinate],
                //			  NULL);

                //m_applied_coil[btn_p->no].position =
                //f_list_coil->coillist[f_cnt].coordinate;
                f_coil[btn_p->no].position =
                    f_list_coil->coillist[f_cnt].coordinate;

                //m_applied_coil[btn_p->no].float_position =
                //m_list_coil_coordinate.coordinate_list[f_cnt].coordinate;
                f_coil[btn_p->no].float_position =
                    m_list_coil_coordinate.coordinate_list[f_cnt].coordinate;

                const int l_float_pos_diff = (int)((f_coil[btn_p->no].float_position - f_coil[btn_p->no].position) * 10.f);

                if (abs(l_float_pos_diff) > 9.9f) {
                    f_coil[btn_p->no].float_position = (float) f_coil[btn_p->no].position;
                }

                //Patni-AMT/2009Oct27/Modified/MCM0229-00006 - Removed code.
                CheckPosition(f_coil);

            } else if (btn_p->coil->position[0] == -1) {

                //Position Free
                l_status = COIL_S_OTHER;
            }

            //Combine
            if (f_coil[btn_p->no].coil != NULL) {

                if (f_coil[btn_p->no].coil->combine ==
                    COMBINE_NOT_TOGETHER) {

                    // PENDING
                    //coilSelectionMenuSensitive(btn_p->no, False);
                }
            }

            // PENDING write into trace
            //DB_PRINTF("",("%s:%d No Position Data \n",__FILE__,__LINE__));
            return l_status;
        }

        //subMenu
        if (btn_p->subMenu != NULL) {

            for (DllT* dll2 = btn_p->subMenu; dll2 != NULL; dll2 = dll2->next) {

                btn2_p = (CoilSelectionCoilButton_t*)dll2->item;

                if ((btn2_p->coil->coil_id  ==  f_list_coil->coillist[f_cnt].coilID
                     && btn2_p->coil->channel  ==  f_list_coil->coillist[f_cnt].channel
                     && btn2_p->coil->port	   ==  f_list_coil->coillist[f_cnt].portID[0])
                    || (btn2_p->coil->coil_id  ==  f_list_coil->coillist[f_cnt].coilID
                        && btn2_p->coil->port	   ==  f_list_coil->coillist[f_cnt].portID[0]
                        && f_list_coil->coillist[f_cnt].channel == ' ')) {

                    //tag = W_DLG_COIL1_OM + befomenu + n;
                    //coil

                    // PENDING
                    //XtVaSetValues( CoilSelectionUiGetWidget(tag),
                    //				XmNmenuHistory, 	btn2_p->button,
                    //				NULL);
                    f_coil[btn2_p->no].coil = btn2_p->coil;

                    //Combine
                    if (f_coil[btn2_p->no].coil != NULL) {

                        if (f_coil[btn2_p->no].coil->combine ==
                            COMBINE_NOT_TOGETHER) {

                            //coilSelectionMenuSensitive(btn2_p->no, False);
                        }
                    }

                    return COIL_S_NO_ERROR;
                }
            }
        }
    }

    //GroupsCoil
    // PENDING Write into trace
    //DB_PRINTF("",("%s:%d NoProces Error\n",__FILE__,__LINE__));
    return COIL_S_NOPROCESS;
}

int CCoilSelectionUtility::GetConvertedCoilButtonIndexForSelectedCoilIndex(
    const int f_selected_coil_index
)
{

    int coil_button_index = -1 ;

    switch (f_selected_coil_index) {

        case 9 :  {

            coil_button_index = GetBaseIndexForCoilButtonFor(CoilType_G);
        }
        break ;

        case 7 :
        case 8 : {


            coil_button_index = (LCoilItems)(f_selected_coil_index - GetBaseIndexFor(CoilType_L) + GetBaseIndexForCoilButtonFor(CoilType_L));

        }
        break ;

        case 0 :
        case 1 :
        case 2 :
        case 3 :
        case 4 :
        case 5 :
        case 6 :

        {
            coil_button_index = (ACoilItems)(f_selected_coil_index - GetBaseIndexFor(CoilType_A) + GetBaseIndexForCoilButtonFor(CoilType_A));
        }
    }

    return coil_button_index ;
}

//***************************Method Header*************************************
//Method Name    : GetSelectCoilList()
//Author         : PATNI / HAR
//Purpose        :
//*****************************************************************************
inline coilSelect_t* CCoilSelectionUtility::GetSelectCoilList(
)const
{
    return m_select_coil_list;
}

//***************************Method Header*************************************
//Method Name    : SetDisallowableCombineCoil()
//Author         : iGATE /	RJ
//Purpose        : Sets the selected coil is Disallowable Combine Coil or not
//*****************************************************************************
void CCoilSelectionUtility::SetDisallowableCombineCoil(
    const bool f_has_disallowable_combine_coil
)
{
    m_has_disallowable_combine_coil = f_has_disallowable_combine_coil;
}

void CCoilSelectionUtility::SetMarkerSign(
    const int f_index, const MarkerSignEnum f_markersignenum
)
{
    if (f_index >= 0  && f_index < MAX_COIL_BUTTON) {

        m_marker_sign[f_index] = (f_markersignenum) ;
    }
}

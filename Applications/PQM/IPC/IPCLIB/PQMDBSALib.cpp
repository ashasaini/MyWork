//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-778
//Redmine-779
//Redmine-780
//Redmine-781
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMDBSALib.cpp
 *  Overview: Implementation of CPQMDBSALib class.
 *  Last Modified: 2007/8/10 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2007/6/15 12:00:00     PQM5 Baselined code
 *   2.0          Patni            2007/8/10 12:00:00     Implemented PQM6 requirements & Formatted
 *
 *****************************************************************************/
#include "StdAfx.h"
#include "PQMDBSALib.h"
#include <tami/vfStudy/vfstudy.h>
#include <libStudyManager/Log/ScopedFuncInOutLog.h>
#include <PASCOMLib/PASCOMSequence.h>
#include <PQM/PQMLogMgr.h>
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"



// Replace DCM_TAG from Unicode string to ulong value
#ifdef DCM_TAG
#undef DCM_TAG
#endif
#define DCM_TAG( a, b ) ( 0x##a##b )

extern Data_decl_list  g_sequence_param_name_list;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//****************************Method Header************************************
//Method Name   :MakeTagXML
//Author        :PATNI/RD
//Purpose       :Makes hardcoded Tags as per XML
//*****************************************************************************
CDBSAVRNode* CPQMDBSALib::MakeTagXML(
    const int f_leafcount_series,
    const int f_nodecount,
    const int f_dataset
)
{
    LPCTSTR FUNC_NAME = _T("CPQMDBSALib::MakeTagXML");
    PQM_SCOPED_TRACE();

    //+Patni-KSS/2011Feb17/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(f_leafcount_series);
    UNREFERENCED_PARAMETER(f_nodecount);
    UNREFERENCED_PARAMETER(f_dataset);
    //-Patni-KSS/2011Feb17/Added/CPP Test tool Execution


    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of MakeTagXML"));

    CDBSAVRNode* l_input_tag = new CDBSAVRNode();

    CDBSAVR* l_vrparam = new CDBSAVRDT(DCM_ST_SERIES_DATE);
    l_input_tag->addLeaf(l_vrparam);

    //+Patni-HAR/2010Apr26/Commented/MVC006285
    //l_vrparam = new CDBSAVRTM(DCM_ST_SERIES_TIME);
    //l_input_tag->addLeaf(l_vrparam);
    //-Patni-HAR/2010Apr26/Commented/MVC006285

    l_vrparam = new CDBSAVRLO(DCM_ST_SERIES_DESCRIPTION);
    l_input_tag->addLeaf(l_vrparam);

    l_vrparam = new CDBSAVRLO(DCM_ST_CONTRAST_BOLUS_AGENT);
    l_input_tag->addLeaf(l_vrparam);

    l_vrparam = new CDBSAVRCS(DCM_ST_MR_ACQUISITION_TYPE);
    l_input_tag->addLeaf(l_vrparam);

    l_vrparam = new CDBSAVRDS(DCM_ST_REPETITION_TIME);
    l_input_tag->addLeaf(l_vrparam);

    l_vrparam = new CDBSAVRDS(DCM_ST_NUMBER_OF_AVERAGES);
    l_input_tag->addLeaf(l_vrparam);

    l_vrparam = new CDBSAVRLO(PQM_SVN_STD_PROTOCOL_NAME);
    l_input_tag->addLeaf(l_vrparam);


    l_vrparam = new CDBSAVRFL2(DCM_ST_ACQUISITION_MATRIX);  //not working
    l_input_tag->addLeaf(l_vrparam);

    l_vrparam = new CDBSAVRCS(DCM_ST_PATIENT_POSITION);
    l_input_tag->addLeaf(l_vrparam);

    l_vrparam = new CDBSAVRSL(PQM_SVN_PRC_REFINE_FILTER);
    l_input_tag->addLeaf(l_vrparam);

    l_vrparam = new CDBSAVRSL(PQM_SVN_PRC_K_SPACE_FILTER);
    l_input_tag->addLeaf(l_vrparam);

    l_vrparam = new CDBSAVRSL(PQM_SVN_PLN_COVERAGES);
    l_input_tag->addLeaf(l_vrparam);

    //Patni-Hemant/2009Oct30/Modified/IR 85 Auto Transfer
    l_vrparam = new CDBSAVRUS(PQM_SVN_PLN_AUTO_POSTPROC_FLG);

    l_input_tag->addLeaf(l_vrparam);

    l_vrparam = new CDBSAVRFL(PQM_SVN_PLN_SELECTED_SLICE_THK);
    l_input_tag->addLeaf(l_vrparam);

    l_vrparam = new CDBSAVRFL(PQM_SVN_PLN_SELECTED_SLICE_GAP);
    l_input_tag->addLeaf(l_vrparam);

    l_vrparam = new CDBSAVRFL2(PQM_SVN_PLN_FOV);
    l_input_tag->addLeaf(l_vrparam);

    l_vrparam = new CDBSAVRSL(PQM_SVN_PLN_TRANSMIT_COIL);
    l_input_tag->addLeaf(l_vrparam);

    l_vrparam = new CDBSAVRSL(PQM_SVN_PLN_TOTAL_TIME);
    l_input_tag->addLeaf(l_vrparam);

    l_vrparam = new CDBSAVRSL(PQM_SVN_PLN_RECVR_COIL);
    l_input_tag->addLeaf(l_vrparam);

    l_vrparam = new CDBSAVRSL(PQM_SVN_EXE_AUTO_VOICE);
    l_input_tag->addLeaf(l_vrparam);

    l_vrparam = new CDBSAVRFL2(PQM_SVN_PLN_SELECTED_CONTRAST_TE);
    l_input_tag->addLeaf(l_vrparam);

    l_vrparam = new CDBSAVRLO(PQM_SVN_PLN_TEMPLATE_SEQ);
    l_input_tag->addLeaf(l_vrparam);

    l_vrparam = new CDBSAVRUS(PQM_SVN_PLN_STATUS);
    l_input_tag->addLeaf(l_vrparam);

    l_vrparam = new CDBSAVRSL(PQM_SVN_PLN_HOLD);
    l_input_tag->addLeaf(l_vrparam);

    l_vrparam = new CDBSAVRSL(PQM_SVN_PLN_AUTO_VOICE);
    l_input_tag->addLeaf(l_vrparam);

    l_vrparam = new CDBSAVRSL(PQM_SVN_EXE_PROTOCOL_ACQ_STATUS);
    l_input_tag->addLeaf(l_vrparam);

    l_vrparam = new CDBSAVRUS(PQM_SVN_PLN_DYNAMIC_SCAN);
    l_input_tag->addLeaf(l_vrparam);

    l_vrparam = new CDBSAVRSL(PQM_SVN_PLN_INTERMIT_FLAG);
    l_input_tag->addLeaf(l_vrparam);

    l_vrparam = new CDBSAVRSL(PQM_SVN_PLN_APP_CODE);
    l_input_tag->addLeaf(l_vrparam);

    l_vrparam = new CDBSAVRFL(PQM_SVN_EXE_COUCH);
    l_input_tag->addLeaf(l_vrparam);
    //+Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
    //+ Study Manager 1
    // + SM4 Himanshu 5 Dec 2008 TEMP Remove Later
    //l_vrparam = new CDBSAVRDS(/*PQM_SVN_EXE_SERIES_HOME_POSITION*/PQM_DCM_MR_SERIES_HOME_POSITION);
    //__asm int 3;
    // + SM4 Himanshu 5 Dec 2008 TEMP Remove Later
    //l_input_tag->addLeaf(l_vrparam);

    //l_vrparam = new CDBSAVRUL(PQM_DCM_MR_SERIES_ACQ_CONDITION);
    //l_input_tag->addLeaf(l_vrparam);
    //- Study Manager 1
    //-Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
    l_vrparam = new CDBSAVRFL(PQM_SVN_SEQ_DBDT_VALUE);
    l_input_tag->addLeaf(l_vrparam);

    l_vrparam = new CDBSAVRSL(PQM_SVN_PLN_SCAN_ANATOMY);
    l_input_tag->addLeaf(l_vrparam);

    l_vrparam = new CDBSAVRFL(PQM_SVN_SEQ_SAR_TOTAL_TIME);
    l_input_tag->addLeaf(l_vrparam);

    l_vrparam = new CDBSAVRSL(PQM_SVN_PLN_VIEWING_DIR);
    l_input_tag->addLeaf(l_vrparam);

    l_vrparam = new CDBSAVRFL(PQM_SVN_PLN_SELECTED_TAG_THK);
    l_input_tag->addLeaf(l_vrparam);

    l_vrparam = new CDBSAVRUS(PQM_SVN_PLN_MOTION_SUPPRESSION);
    l_input_tag->addLeaf(l_vrparam);

    l_vrparam = new CDBSAVRSL(PQM_SVN_PLN_APP_OPTION);
    l_input_tag->addLeaf(l_vrparam);

    l_vrparam = new CDBSAVRUS(PQM_SVN_PLN_GRID_TAG);
    l_input_tag->addLeaf(l_vrparam);

    l_vrparam = new CDBSAVRSL(PQM_SVN_PLN_SLICE_ORIENTATION);
    l_input_tag->addLeaf(l_vrparam);

    l_vrparam = new CDBSAVRSL(PQM_SVN_PLN_PHASE_DIRECTION);
    l_input_tag->addLeaf(l_vrparam);

    l_vrparam = new CDBSAVRUS(PQM_SVN_PLN_WRAPAROUND_REDUCT);
    l_input_tag->addLeaf(l_vrparam);

    l_vrparam = new CDBSAVRLO(PQM_DCM_MR_SERIES_NODE_PATH);
    l_input_tag->addLeaf(l_vrparam);

    //Meghana for defect fixing
    l_vrparam = new CDBSAVRLO(PQM_DCM_MR_PROTOCOL_NODE_PATH);
    l_input_tag->addLeaf(l_vrparam);

    l_vrparam = new CDBSAVROB(DCM_MR_WFDA_CAPP_INITIAL_PARAMETER);
    l_input_tag->addLeaf(l_vrparam);
    l_vrparam = new CDBSAVROB(DCM_MR_WFDA_CAPP_ANALYSIS_PARAMETERS);
    l_input_tag->addLeaf(l_vrparam);
    l_vrparam = new CDBSAVRLO(DCM_MR_WFDA_CAPP_PREFERENCE_GUID);
    l_input_tag->addLeaf(l_vrparam);
    l_vrparam = new CDBSAVRUS(DCM_MR_WFDA_CAPP_INSCAN_FLAG);
    l_input_tag->addLeaf(l_vrparam);
    l_vrparam = new CDBSAVRLO(DCM_MR_WFDA_CAPP_INSCAN_CALC_MODE);
    l_input_tag->addLeaf(l_vrparam);
    std::vector<CDBSAVRLeaf*> *l_pleaf = NULL;  //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections
    std::vector<CDBSAVRLeaf*>::const_iterator q = NULL;  //Patni-AD/2009Jun01/Modified/SU14 //cpp test corrections

    if (l_input_tag) {
        l_pleaf = ((CDBSAVR_Utility*)l_input_tag)->GetLeafAddr();

        for (q = l_pleaf->begin(); q != l_pleaf->end(); q++) {

            this->MakeVR((static_cast<CDBSAVR*>(*q)), MRC_TRUE);
        }
    }

    return l_input_tag;
}

//****************************Method Header************************************
//Method Name   :SetPascomSequence
//Author        :PATNI/RD
//Purpose       :Sets the PascomSequence
//*****************************************************************************
void CPQMDBSALib::SetPascomSequence(
    CPASCOMSequence* f_seq
)
{
    LPCTSTR FUNC_NAME = _T("CPQMDBSALib::SetPascomSequence");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetPascomSequence"));

    m_pascom_sequence = f_seq;
}

//****************************Method Header************************************
//Method Name   :MakeVR
//Author        :PATNI/RD
//Purpose       :Fills the data in VR Node
//*****************************************************************************
bool CPQMDBSALib::MakeVR(
    CDBSAVR* f_vrleaf,
    const int f_dataset
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMDBSALib::MakeVR");
    CString l_str_msg(_T(""));
    l_str_msg.Format(_T("%d"), f_dataset);
    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, l_str_msg);

    static int l_tagno = 0;
    l_tagno++;

    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    /*
    time_t l_wktime;
    struct tm *l_my_time;
    */
    time_t l_wktime = 0l;
    struct tm* l_my_time = 0;
    //- Patni-PJS/2009Apr25/Modify/CPP Test tool execution

    if (f_dataset == MRC_TRUE) {

        if ((f_vrleaf->GetVRType() == DBSA_DCM_VR_DT)
            || (f_vrleaf->GetVRType() == DBSA_DCM_VR_TM)
            || (f_vrleaf->GetVRType() == DBSA_DCM_VR_DA)) {

            time(&l_wktime);
            l_my_time = localtime(&l_wktime);
        }
    }

    //Patni-RUP/2009Aug20/Modified/cpp test corrections
    //CDBSAVR *l_rtc=NULL;

    CPASParamStore* l_seq_param = NULL;

    if (m_pascom_sequence != NULL) {
        l_seq_param = m_pascom_sequence->GetSequenceParam();
    }

    DateTime_t l_tmdata;

    time(&l_wktime);
    l_my_time = localtime(&l_wktime);
    l_tmdata.year = l_my_time->tm_year + YEAR_OFFSET;
    l_tmdata.month = l_my_time->tm_mon + 1;
    l_tmdata.day = l_my_time->tm_mday;
    l_tmdata.hour = l_my_time->tm_hour;
    l_tmdata.minute = l_my_time->tm_min;
    l_tmdata.second = l_my_time->tm_sec;
    l_tmdata.offsetExist = true;
    l_tmdata.offsetvalue = TIME_OFFSET;
    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    //unsigned int l_vrtype;
    unsigned int l_vrtype = f_vrleaf->GetDBSAVRType();

    int l_result = 0;
    CString l_str = _T("");
    CString l_csdata = _T("");
    CString l_strmode = _T("");

    switch (l_vrtype) {
        case MRC_CDBSAVR_TYPE_OB:
            //+Patni-RUP/2009Aug20/Modified/cpp test corrections
            //l_rtc = new CDBSAVROB(f_vrleaf->GetTag());
            //if (f_dataset == MRC_TRUE){
            //    if (l_seq_param!=NULL){
            //        BYTE l_obdata[]={0x11, 0x22, 0x33};
            //        l_rtc->SetValue(l_obdata, VALUE_OBDATA, 1);
            //    }
            //}
            //-Patni-RUP/2009Aug20/Modified/cpp test corrections
            break;

        case MRC_CDBSAVR_TYPE_CS:

            if (f_dataset == MRC_TRUE) {

                switch (f_vrleaf->GetTag()) {
                    case DCM_ST_PATIENT_POSITION: {
                        if (f_dataset == MRC_TRUE) {
                            if (l_seq_param != NULL) {

                                l_seq_param->GetVector(_T("PORI"), l_csdata);
                                LPCTSTR l_patientposition = l_csdata;
                                l_result = f_vrleaf->SetValue(&l_patientposition, 1);

                                if (l_result != MRC_DBSA_SUCCESS) {
                                    l_str = DBSA_GetErrorString(l_result);
                                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                                }

                            }
                        }

                        break;
                    }

                    case DCM_ST_MR_ACQUISITION_TYPE: {
                        if (f_dataset == MRC_TRUE) {
                            if (l_seq_param != NULL) {
                                int l_mode = l_seq_param->GetInt(_T("IMGMODE"));

                                l_strmode.Format(_T("%d"), l_mode);
                                LPCTSTR l_acqtype = l_strmode;
                                l_result = f_vrleaf->SetValue(&l_acqtype, 1);

                                if (l_result != MRC_DBSA_SUCCESS) {
                                    l_str = DBSA_GetErrorString(l_result);
                                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                                }
                            }
                        }

                        break;
                    }

                    default:
                        break;
                }

                break;
            }

        case MRC_CDBSAVR_TYPE_DS:

            if (f_dataset == MRC_TRUE) {

                switch (f_vrleaf->GetTag()) {
                    case DCM_ST_REPETITION_TIME: {
                        if (f_dataset == MRC_TRUE) {
                            if (l_seq_param != NULL) {
                                float l_tr = l_seq_param->GetFloat(_T("TR"));
                                l_strmode.Format(_T("%f"), l_tr);
                                LPCTSTR l_trtype = l_strmode;
                                l_result = f_vrleaf->SetValue(&l_trtype, 1);

                                if (l_result != MRC_DBSA_SUCCESS) {
                                    l_str = DBSA_GetErrorString(l_result);
                                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                                }

                            }
                        }

                        break;
                    }

                    //+Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
                    // + SM4 Himanshu 5 Dec 2008 TEMP Remove Later
                    //case /*PQM_SVN_EXE_SERIES_HOME_POSITION*/PQM_DCM_MR_SERIES_HOME_POSITION: {

                    //__asm int 3;
                    //if (f_dataset == MRC_TRUE) {
                    //if (l_seq_param != NULL) {


                    //float l_flt_series_home_pos = -2000.00;
                    //CString l_series_home_pos = /*CPQMConfig::GetInstance()->GetHomePosition();//*/(_T("-1234.00"));
                    //l_series_home_pos.Format(_T("%f"), l_flt_series_home_pos);
                    //LPCTSTR l_naqtype = l_series_home_pos;
                    // - SM4 Himanshu 5 Dec 2008 TEMP Remove Later
                    //l_result = f_vrleaf->SetValue(((LPCTSTR*)(&l_series_home_pos)), 1);

                    //if (l_result != MRC_DBSA_SUCCESS) {
                    //  CString l_str;
                    //l_str = DBSA_GetErrorString(l_result);
                    //CPQMLogMgr::GetInstance()->WriteTraceLog(
                    //  DOMAIN_NAME, USER_FUNC_MARKER,
                    //_T("CDBSAVRNode::MakeVR"),
                    //l_str);
                    //}
                    //}
                    //}

                    //break;
                    //}
                    //-Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774

                    case DCM_ST_NUMBER_OF_AVERAGES: {
                        if (f_dataset == MRC_TRUE) {
                            if (l_seq_param != NULL) {
                                const int l_naq = l_seq_param->GetInt(_T("NAQ"));
                                CString l_strnaq = _T("");
                                l_strnaq.Format(_T("%d"), l_naq);
                                LPCTSTR l_naqtype = l_strnaq;
                                l_result = f_vrleaf->SetValue(&l_naqtype, 1);

                                if (l_result != MRC_DBSA_SUCCESS) {
                                    l_str = DBSA_GetErrorString(l_result);
                                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                                }
                            }
                        }

                        break;
                    }

                    default:
                        break;
                }

                break;
            }

            //DCM_MR_PRC_PROC_START_TIME (0019108D)
            //+Patni-HAR/2010Apr26/Commented/MVC006285
            //case MRC_CDBSAVR_TYPE_TM:
            //
            //	if (f_dataset == MRC_TRUE) {
            //		switch (f_vrleaf->GetTag()) {
            //		case DCM_ST_SERIES_TIME: {
            //			if (f_dataset == MRC_TRUE) {
            //				if (l_seq_param != NULL) {
            //					f_vrleaf->SetValue(&l_tmdata, 1);
            //				}
            //
            //				break;
            //			}
            //
            //		default:
            //			break;
            //								 }
            //
            //		}
            //
            //		break;
            //	}
            //-Patni-HAR/2010Apr26/Commented/MVC006285

        case MRC_CDBSAVR_TYPE_DT:

            if (f_dataset == MRC_TRUE) {
                switch (f_vrleaf->GetTag()) {
                    case DCM_ST_SERIES_DATE: {
                        if (f_dataset == MRC_TRUE) {
                            if (l_seq_param != NULL) {
                                f_vrleaf->SetValue(&l_tmdata, 1);

                            }

                            break;
                        }

                        default:
                            break;
                        }

                }

                break;
            }

        case MRC_CDBSAVR_TYPE_LO: {

            switch (f_vrleaf->GetTag()) {

                    //DCM_MP_SHORT_SERIES_INFO (0018 1030)
                case PQM_SVN_STD_PROTOCOL_NAME: {
                    if (f_dataset == MRC_TRUE) {
                        if (l_seq_param != NULL) {

                            LPCTSTR l_comment = l_seq_param->GetString(_T("COMM"));
                            l_result = f_vrleaf->SetValue(&l_comment, 1);

                            if (l_result != MRC_DBSA_SUCCESS) {
                                l_str = DBSA_GetErrorString(l_result);
                                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                            }
                        }
                    }

                    break;
                }

                //DCM_ST_SERIES_DESCRIPTION (0008 103E)
                //SVN_PLN_SCAN_COMMENTS
                case DCM_ST_SERIES_DESCRIPTION: {
                    if (f_dataset == MRC_TRUE) {
                        if (l_seq_param != NULL) {

                            LPCTSTR l_memo = l_seq_param->GetString(_T("MEMO"));
                            l_result = f_vrleaf->SetValue(&l_memo, 1);

                            if (l_result != MRC_DBSA_SUCCESS) {
                                l_str = DBSA_GetErrorString(l_result);
                                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                            }
                        }
                    }

                    break;
                }


                //DCM_MR_PLN_TEMPLATE_SEQ (001910CA)
                case PQM_SVN_PLN_TEMPLATE_SEQ : {
                    if (f_dataset == MRC_TRUE) {
                        if (l_seq_param != NULL) {

                            LPCTSTR l_seq = l_seq_param->GetString(_T("SEQ"));
                            l_result = f_vrleaf->SetValue(&l_seq, 1);

                            if (l_result != MRC_DBSA_SUCCESS) {
                                l_str = DBSA_GetErrorString(l_result);
                                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                            }
                        }

                    }

                    break;
                }

                case PQM_DCM_MR_SERIES_NODE_PATH : {
                    if (f_dataset == MRC_TRUE) {
                        if (l_seq_param != NULL) {

                            LPCTSTR l_node_path =  _T("NODE_PATH");
                            l_result = f_vrleaf->SetValue(&l_node_path, 1);

                            if (l_result != MRC_DBSA_SUCCESS) {
                                l_str = DBSA_GetErrorString(l_result);
                                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                            }
                        }

                    }

                    break;
                }

                //Meghana for defect fixing
                case PQM_DCM_MR_PROTOCOL_NODE_PATH : {
                    if (f_dataset == MRC_TRUE) {
                        if (l_seq_param != NULL) {

                            LPCTSTR l_node_path = _T("NODE_PATH");
                            l_result = f_vrleaf->SetValue(&l_node_path, 1);

                            if (l_result != MRC_DBSA_SUCCESS) {
                                l_str = DBSA_GetErrorString(l_result);
                                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                            }
                        }

                    }

                    break;
                }

                // (00180010)
                case DCM_ST_CONTRAST_BOLUS_AGENT : {
                    if (f_dataset == MRC_TRUE) {
                        if (l_seq_param != NULL) {

                            LPCTSTR l_contrastname = l_seq_param->GetString(_T("CONTRASTNAME"));
                            l_result = f_vrleaf->SetValue(&l_contrastname, 1);

                            if (l_result != MRC_DBSA_SUCCESS) {
                                l_str = DBSA_GetErrorString(l_result);
                                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                            }
                        }

                    }

                    break;
                }
                break;

                default:
                    break;
            }

            break;
        }


        case MRC_CDBSAVR_TYPE_VEC2: {

            CDBSAVRFL2* l_float_val = dynamic_cast<CDBSAVRFL2*>(f_vrleaf);

            if (l_float_val) { // FL with VM 2
                switch (f_vrleaf->GetTag()) {
                        //DCM_MR_PLN_FOV (001910BC)
                    case PQM_SVN_PLN_FOV:

                        if (f_dataset == MRC_TRUE) {
                            if (l_seq_param != NULL) {
                                CString l_sdata = _T("");
                                l_seq_param->GetVector(_T("FOV"), l_sdata);
                                CString l_cs1 = l_sdata.Left(l_sdata.Find(_T(","), 0));
                                CString l_cs2 = l_sdata.Right(l_sdata.Find(_T(",")));
                                char* l_ch1 = new char[l_cs1.GetLength() + 1];

                                // MEM_INI AMIT 20081230
                                if (l_ch1) {
                                    memset(l_ch1, 0, sizeof(char) * (l_cs1.GetLength() + 1));
                                    _wcstombsz(l_ch1, l_cs1, l_cs1.GetLength() + 1);
                                    char* l_ch2 = new char[l_cs2.GetLength() + 1];

                                    // MEM_INI AMIT 20081230
                                    if (l_ch2) {
                                        memset(l_ch2, 0 , sizeof(char) * (l_cs2.GetLength() + 1));
                                        _wcstombsz(l_ch2, l_cs2, l_cs2.GetLength() + 1);
                                        float l_fldata1 = (float)atof(l_ch1);
                                        float l_fldata2 = (float)atof(l_ch2);
                                        float l_fldata[] = {l_fldata1, l_fldata2};
                                        f_vrleaf->SetValue(l_fldata, VALUE_MULTIPLIER);

                                        //+Patni-HAR/2009Aug11/Added/Memory Leaks
                                        DEL_PTR_ARY(l_ch1);

                                        DEL_PTR_ARY(l_ch2);
                                        //-Patni-HAR/2009Aug11/Added/Memory Leaks

                                    } else {
                                        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_ch2 pointer is NULL"));
                                    }

                                } else {
                                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_ch1 pointer is NULL"));
                                }
                            }
                        }

                        break;

                        //DCM_MR_PLN_SELECTED_CONTRAST_TE (001910C6)
                    case PQM_SVN_PLN_SELECTED_CONTRAST_TE:

                        if (f_dataset == MRC_TRUE) {
                            if (l_seq_param != NULL) {
                                float l_fdata1 = l_seq_param->GetFloat(_T("TE"));
                                float l_fdata2 = 0.0f;
                                float l_fdata[] = {l_fdata1, l_fdata2};
                                f_vrleaf->SetValue(l_fdata, VALUE_MULTIPLIER);

                            } else {
                                long l_sldata = DEFAULT_DATA_VALUE;
                                f_vrleaf->SetValue(&l_sldata, VALUE_MULTIPLIER);
                            }
                        }

                        break;


                    case DCM_ST_ACQUISITION_MATRIX:

                        if (f_dataset == MRC_TRUE) {
                            if (l_seq_param != NULL) {
                                CString mtx_str = _T("");
                                l_seq_param->GetVector(_T("MTX"), mtx_str);
                                CString l_cs1 = mtx_str.Left(mtx_str.Find(_T(","), 0));
                                CString l_cs2 = mtx_str.Right(mtx_str.Find(_T(",")));
                                char* l_ch1 = new char[l_cs1.GetLength() + 1];

                                // MEM_INI AMIT 20081230
                                if (l_ch1) {
                                    memset(l_ch1, 0, sizeof(char) * (l_cs1.GetLength() + 1));
                                    _wcstombsz(l_ch1, l_cs1, l_cs1.GetLength() + 1);
                                    char* l_ch2 = new char[l_cs2.GetLength() + 1];

                                    if (l_ch2) {
                                        // MEM_INI AMIT 20081230
                                        memset(l_ch2, NULL, sizeof(char) * (l_cs2.GetLength() + 1));
                                        _wcstombsz(l_ch2, l_cs2, l_cs2.GetLength() + 1);
                                        float mtx_data1 = (float)atof(l_ch1);
                                        float mtx_data2 = (float)atof(l_ch2);
                                        float us_data[] = {mtx_data1, mtx_data2 };
                                        f_vrleaf->SetValue(us_data, VALUE_MULTIPLIER);

                                        //+Patni-HAR/2009Aug11/Added/Memory Leaks
                                        DEL_PTR_ARY(l_ch1);

                                        DEL_PTR_ARY(l_ch2);

                                    } else {
                                        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_ch2 pointer is NULL"));

                                    }

                                } else {
                                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_ch1 pointer is NULL"));
                                }

                                //-Patni-HAR/2009Aug11/Added/Memory Leaks
                            }
                        }

                        break;
                }
            }

            break;
        }

        case MRC_CDBSAVR_TYPE_VALUE: {

            CDBSAVRFD* l_double_val = dynamic_cast<CDBSAVRFD*>(f_vrleaf);
            CDBSAVRFL* l_float_val = dynamic_cast<CDBSAVRFL*>(f_vrleaf);
            CDBSAVRSL* l_long_val = dynamic_cast<CDBSAVRSL*>(f_vrleaf);
            CDBSAVRSS* l_short_val = dynamic_cast<CDBSAVRSS*>(f_vrleaf);
            CDBSAVRUS* l_unsigned_short_val = dynamic_cast<CDBSAVRUS*>(f_vrleaf);

            if (l_double_val) { //FD
                double* l_data_array = new double[f_vrleaf->GetSize()];

                DEL_PTR_ARY(l_data_array);

            } else if (l_float_val) { //FL

                switch (f_vrleaf->GetTag()) {
                        //DCM_MR_PLN_SELECTED_SLICE_THK (001910BA)
                    case PQM_SVN_PLN_SELECTED_SLICE_THK:

                        if (f_dataset == MRC_TRUE) {
                            if (l_seq_param != NULL) {
                                float l_fldata = l_seq_param->GetFloat(_T("TS"));
                                l_result = f_vrleaf->SetValue(&l_fldata, 1);

                                if (l_result != MRC_DBSA_SUCCESS) {
                                    l_str = DBSA_GetErrorString(l_result);
                                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                                }

                            } else {
                                long l_fldata = DEFAULT_DATA_VALUE;
                                f_vrleaf->SetValue(&l_fldata, 1);
                            }
                        }

                        break;

                        //DCM_MR_PLN_SELECTED_SLICE_GAP (001910BB)
                    case PQM_SVN_PLN_SELECTED_SLICE_GAP:

                        if (f_dataset == MRC_TRUE) {
                            if (l_seq_param != NULL) {
                                float l_fldata = l_seq_param->GetFloat(_T("GAP"));
                                l_result = f_vrleaf->SetValue(&l_fldata, 1);

                                if (l_result != MRC_DBSA_SUCCESS) {
                                    l_str = DBSA_GetErrorString(l_result);
                                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                                }

                            } else {
                                long l_fldata = DEFAULT_DATA_VALUE;
                                f_vrleaf->SetValue(&l_fldata, 1);
                            }
                        }

                        break;

                        //DCM_MR_EXE_COUCH (001910FD)
                    case PQM_SVN_EXE_COUCH:

                        if (f_dataset == MRC_TRUE) {
                            if (l_seq_param != NULL) {
                                float l_fldata = l_seq_param->GetFloat(_T("COUCH"));

                                l_result = f_vrleaf->SetValue(&l_fldata, 1);

                                if (l_result != MRC_DBSA_SUCCESS) {
                                    l_str = DBSA_GetErrorString(l_result);
                                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                                }

                            } else {
                                long l_fldata = DEFAULT_DATA_VALUE;
                                f_vrleaf->SetValue(&l_fldata, 1);
                            }
                        }

                        break;

                    case PQM_SVN_PLN_SELECTED_TAG_THK:

                        if (f_dataset == MRC_TRUE) {
                            if (l_seq_param != NULL) {
                                float l_fl_thkdata = l_seq_param->GetFloat(_T("THK"));

                                l_result = f_vrleaf->SetValue(&l_fl_thkdata, 1);

                                if (l_result != MRC_DBSA_SUCCESS) {
                                    l_str = DBSA_GetErrorString(l_result);
                                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                                }

                            } else {
                                long l_fldata = DEFAULT_DATA_VALUE;
                                f_vrleaf->SetValue(&l_fldata, 1);
                            }
                        }

                        break;

                        //DCM_MR_SEQ_DBDT_VALUE (0019112F)
                    case PQM_SVN_SEQ_DBDT_VALUE:

                        if (f_dataset == MRC_TRUE) {
                            if (l_seq_param != NULL) {
                                float l_fldata = l_seq_param->GetFloat(_T("DBDT"));
                                l_result = f_vrleaf->SetValue(&l_fldata, 1);

                                if (l_result != MRC_DBSA_SUCCESS) {
                                    l_str = DBSA_GetErrorString(l_result);
                                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                                }

                            } else {
                                long l_fldata = DEFAULT_DATA_VALUE;
                                f_vrleaf->SetValue(&l_fldata, 1);
                            }
                        }

                        break;

                        //DCM_MR_PRC_PROC_START_TIME (0019113A)
                    case PQM_SVN_SEQ_SAR_TOTAL_TIME :

                        if (f_dataset == MRC_TRUE) {
                            if (l_seq_param != NULL) {
                                float l_fldata = l_seq_param->GetFloat(_T("SAR"));

                                l_result = f_vrleaf->SetValue(&l_fldata, 1);

                                if (l_result != MRC_DBSA_SUCCESS) {
                                    l_str = DBSA_GetErrorString(l_result);
                                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                                }

                            } else {
                                long l_fldata = DEFAULT_DATA_VALUE;
                                f_vrleaf->SetValue(&l_fldata, 1);
                            }

                        }

                        break;


                        //DCM_MR_PLN_SEQ_BAND_WIDTH (00191146)
                        //+Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
                        /*case SVN_SEQ_BAND_WIDTH:

                            if (f_dataset == MRC_TRUE) {
                                if (l_seq_param != NULL) {
                                    float l_fldata = l_seq_param->GetFloat(_T("BW"));
                                    l_result = f_vrleaf->SetValue(&l_fldata, 1);

                                    if (l_result != MRC_DBSA_SUCCESS) {
                                        CString l_str;
                                        l_str = DBSA_GetErrorString(l_result);
                                        CPQMLogMgr::GetInstance()->WriteTraceLog(
                                            DOMAIN_NAME, USER_FUNC_MARKER,
                                            _T("CDBSAVRNode::MakeVR"),
                                            l_str);
                                    }

                                } else {
                                    long l_fldata = DEFAULT_DATA_VALUE;
                                    f_vrleaf->SetValue(&l_fldata, 1);
                                }
                            }

                            break;
                        */
                        //-Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
                    default:
                        break;
                }


            } else if (l_long_val) { //SL
                switch (f_vrleaf->GetTag()) {
                        //DCM_MR_PRC_REFINE_FILTER (00191050)
                    case PQM_SVN_PRC_REFINE_FILTER:

                        if (f_dataset == MRC_TRUE) {
                            if (l_seq_param != NULL) {
                                long l_fdata = (long)l_seq_param->GetFloat(_T("RFLT"));
                                l_result = f_vrleaf->SetValue(&l_fdata, 1);

                                if (l_result != MRC_DBSA_SUCCESS) {
                                    l_str = DBSA_GetErrorString(l_result);
                                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                                }

                            } else {
                                long l_sldata = DEFAULT_DATA_VALUE;
                                f_vrleaf->SetValue(&l_sldata, 1);
                            }
                        }

                        break;

                        //DCM_MR_PRC_K_SPACE_FILTER (00191051)
                    case PQM_SVN_PRC_K_SPACE_FILTER:

                        if (f_dataset == MRC_TRUE) {
                            if (l_seq_param != NULL) {
                                long l_fdata = (long)l_seq_param->GetFloat(_T("KFLT"));

                                l_result = f_vrleaf->SetValue(&l_fdata, 1);

                                if (l_result != MRC_DBSA_SUCCESS) {
                                    l_str = DBSA_GetErrorString(l_result);
                                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                                }

                            } else {
                                long l_sldata = DEFAULT_DATA_VALUE;
                                f_vrleaf->SetValue(&l_sldata, 1);
                            }
                        }

                        break;

                        //DCM_MR_PLN_SCAN_ANATOMY (00191132)
                    case PQM_SVN_PLN_SCAN_ANATOMY:

                        if (f_dataset == MRC_TRUE) {
                            if (l_seq_param != NULL) {
                                long sa_data = (long)l_seq_param->GetFloat(_T("SA"));

                                l_result = f_vrleaf->SetValue(&sa_data, 1);

                                if (l_result != MRC_DBSA_SUCCESS) {
                                    l_str = DBSA_GetErrorString(l_result);
                                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                                }

                            } else {
                                long l_sldata = DEFAULT_DATA_VALUE;
                                f_vrleaf->SetValue(&l_sldata, 1);
                            }
                        }

                        break;

                        //DCM_MR_PLN_COVERAGES (001910B6)
                    case PQM_SVN_PLN_COVERAGES:

                        if (f_dataset == MRC_TRUE) {
                            if (l_seq_param != NULL) {
                                long l_fdata = (long)l_seq_param->GetFloat(_T("COV"));

                                l_result = f_vrleaf->SetValue(&l_fdata, 1);

                                if (l_result != MRC_DBSA_SUCCESS) {
                                    l_str = DBSA_GetErrorString(l_result);
                                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                                }

                            } else {
                                long l_sldata = DEFAULT_DATA_VALUE;
                                f_vrleaf->SetValue(&l_sldata, 1);
                            }
                        }

                        break;

                        //DCM_MR_PLN_TOTAL_TIME (001910BE)
                    case PQM_SVN_PLN_TOTAL_TIME:

                        if (f_dataset == MRC_TRUE) {
                            if (l_seq_param != NULL) {
                                CString l_sdata = l_seq_param->GetString(_T("SCANTIME"));
                                char* l_scantime = new char[l_sdata.GetLength() + 1];

                                if (l_scantime) {			//Patni-MJC/2009Aug17/Modified/cpp test corrections
                                    memset(l_scantime, 0, sizeof(char) * (l_sdata.GetLength() + 1));
                                    _wcstombsz(l_scantime, l_sdata, l_sdata.GetLength());
                                    long l_sldata = atol(l_scantime);

                                    l_result = f_vrleaf->SetValue(&l_sldata, 1);

                                    //+Patni-HAR/2009Aug11/Added/Memory Leaks
                                    DEL_PTR_ARY(l_scantime);

                                    //-Patni-HAR/2009Aug11/Added/Memory Leaks
                                    if (l_result != MRC_DBSA_SUCCESS) {
                                        l_str = DBSA_GetErrorString(l_result);
                                        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                                    }

                                    //+Patni-MJC/2009Aug17/Modified/cpp test corrections

                                } else {
                                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_scantime pointer is NULL"));
                                }

                                //-Patni-MJC/2009Aug17/Modified/cpp test corrections

                            } else {
                                long l_sldata = DEFAULT_DATA_VALUE;
                                f_vrleaf->SetValue(&l_sldata, 1);
                            }
                        }

                        break;

                        //DCM_MR_PLN_HOLD (001910CD)
                    case PQM_SVN_PLN_HOLD:

                        if (f_dataset == MRC_TRUE) {
                            if (l_seq_param != NULL) {
                                long l_fdata = (long)l_seq_param->GetFloat(_T("HOLD"));

                                l_result = f_vrleaf->SetValue(&l_fdata, 1);

                                if (l_result != MRC_DBSA_SUCCESS) {
                                    l_str = DBSA_GetErrorString(l_result);
                                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                                }

                            } else {
                                long l_sldata = DEFAULT_DATA_VALUE;
                                f_vrleaf->SetValue(&l_sldata, 1);
                            }
                        }

                        break;

                        //DCM_MR_EXE_PROTOCOL_ACQ_STATUS (001910CF)
                    case PQM_SVN_EXE_PROTOCOL_ACQ_STATUS:

                        if (f_dataset == MRC_TRUE) {
                            if (l_seq_param != NULL) {
                                long l_fdata = 0;
                                l_result = f_vrleaf->SetValue(&l_fdata, 1);

                                if (l_result != MRC_DBSA_SUCCESS) {
                                    l_str = DBSA_GetErrorString(l_result);
                                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                                }

                            } else {
                                long l_sldata = DEFAULT_DATA_VALUE;
                                f_vrleaf->SetValue(&l_sldata, 1);
                            }
                        }

                        break;

                        //DCM_MR_PLN_APP_CODE (001910E9)
                    case PQM_SVN_PLN_APP_CODE:

                        if (f_dataset == MRC_TRUE) {
                            if (l_seq_param != NULL) {
                                long l_fdata = (long)l_seq_param->GetFloat(_T("IMGTECH"));

                                l_result = f_vrleaf->SetValue(&l_fdata, 1);

                                if (l_result != MRC_DBSA_SUCCESS) {
                                    l_str = DBSA_GetErrorString(l_result);
                                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                                }

                            } else {
                                long l_sldata = DEFAULT_DATA_VALUE;
                                f_vrleaf->SetValue(&l_sldata, 1);
                            }
                        }

                        break;

                        //DCM_MR_PLN_VIEWING_DIR (0019115D)
                    case PQM_SVN_PLN_VIEWING_DIR:

                        if (f_dataset == MRC_TRUE) {
                            if (l_seq_param != NULL) {
                                long l_fdata = (long)l_seq_param->GetFloat(_T("VDIR"));
                                l_result = f_vrleaf->SetValue(&l_fdata, 1);

                                if (l_result != MRC_DBSA_SUCCESS) {
                                    l_str = DBSA_GetErrorString(l_result);
                                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                                }

                            } else {
                                long l_sldata = DEFAULT_DATA_VALUE;
                                f_vrleaf->SetValue(&l_sldata, 1);
                            }
                        }

                        break;

                        //DCM_MR_PLN_TOTAL_SLICES (00191160)
                        //+Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
                        /*case SVN_PLN_TOTAL_SLICES:

                            if (f_dataset == MRC_TRUE) {
                                if (l_seq_param != NULL) {
                                    //+ Patni-PJS/2009Jul23/Modify/IDS#491,MVC002508
                                    //long l_fdata = (long)l_seq_param->GetFloat(_T("PLN"));
                                    long l_fdata = (long)l_seq_param->GetFloat(_T("NS"));
                                    //- Patni-PJS/2009Jul23/Modify/IDS#491,MVC002508
                                    l_result = f_vrleaf->SetValue(&l_fdata, 1);

                                    if (l_result != MRC_DBSA_SUCCESS) {
                                        CString l_str;
                                        l_str = DBSA_GetErrorString(l_result);
                        				PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                                    }

                                } else {
                                    long l_sldata = DEFAULT_DATA_VALUE;
                                    f_vrleaf->SetValue(&l_sldata, 1);
                                }
                            }

                            break;*/
                        //-Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
                    case PQM_SVN_PLN_TRANSMIT_COIL : {
                        if (f_dataset == MRC_TRUE) {
                            if (l_seq_param != NULL) {
                                long l_ltdata = (long)l_seq_param->GetFloat(_T("TC"));
                                l_result = f_vrleaf->SetValue(&l_ltdata, 1);

                                if (l_result != MRC_DBSA_SUCCESS) {
                                    l_str = DBSA_GetErrorString(l_result);
                                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                                }
                            }

                        }

                        break;
                    }

                    //DCM_MR_PLN_RECVR_COIL (00191149)
                    case PQM_SVN_PLN_RECVR_COIL : {
                        if (f_dataset == MRC_TRUE) {
                            if (l_seq_param != NULL) {
                                long l_ltdata = (long)l_seq_param->GetFloat(_T("RC"));
                                l_result = f_vrleaf->SetValue(&l_ltdata, 1);

                                if (l_result != MRC_DBSA_SUCCESS) {
                                    l_str = DBSA_GetErrorString(l_result);
                                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                                }
                            }

                        }

                        break;
                    }

                    case PQM_SVN_EXE_AUTO_VOICE : {
                        if (f_dataset == MRC_TRUE) {
                            if (l_seq_param != NULL) {
                                long l_ltdata = 0;
                                l_result = f_vrleaf->SetValue(&l_ltdata, 1);

                                if (l_result != MRC_DBSA_SUCCESS) {
                                    l_str = DBSA_GetErrorString(l_result);
                                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                                }
                            }

                        }

                        break;
                    }

                    case PQM_SVN_PLN_APP_OPTION : {
                        if (f_dataset == MRC_TRUE) {
                            if (l_seq_param != NULL) {
                                long l_ltdata = l_seq_param->GetInt(_T("APPOPT"));
                                l_result = f_vrleaf->SetValue(&l_ltdata, 1);

                                if (l_result != MRC_DBSA_SUCCESS) {
                                    l_str = DBSA_GetErrorString(l_result);
                                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                                }
                            }

                        }

                        break;
                    }

                    case PQM_SVN_PLN_SLICE_ORIENTATION : {
                        if (f_dataset == MRC_TRUE) {
                            if (l_seq_param != NULL) {
                                long l_plndata = l_seq_param->GetInt(_T("PLN"));
                                l_result = f_vrleaf->SetValue(&l_plndata, 1);

                                if (l_result != MRC_DBSA_SUCCESS) {
                                    l_str = DBSA_GetErrorString(l_result);
                                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                                }
                            }

                        }

                        break;
                    }

                    //DCM_MR_PLN_PHASE_DIRECTION (00211003)
                    case PQM_SVN_PLN_PHASE_DIRECTION:

                        if (f_dataset == MRC_TRUE) {
                            if (l_seq_param != NULL) {
                                long l_pddata = (long)l_seq_param->GetFloat(_T("PD"));
                                l_result = f_vrleaf->SetValue(&l_pddata, 1);

                                if (l_result != MRC_DBSA_SUCCESS) {
                                    l_str = DBSA_GetErrorString(l_result);
                                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                                }

                            } else {
                                long l_sldata = DEFAULT_DATA_VALUE;
                                f_vrleaf->SetValue(&l_sldata, 1);
                            }
                        }

                        break;


                        //+Patni-Hemant+ARD/2009Nov04/Code deleted/IR 85 Auto Transfer
                        //case PQM_SVN_PLN_AUTO_POSTPROC_FLG :{
                        //                                  } break ;
                        //-Patni-Hemant+ARD/2009Nov04/Code deleted/IR 85 Auto Transfer

                    default:
                        break;
                }


            } else if (l_short_val) { // SS
                short* l_data_array = new short[f_vrleaf->GetSize()];

                DEL_PTR_ARY(l_data_array);

            }  else if (l_unsigned_short_val) { // US
                switch (f_vrleaf->GetTag()) {
                        //+Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
                        //DCM_MR_PLN_VARI_FLIP_FLAG (0019115E)
                        /*case SVN_PLN_VARI_FLIP_FLAG:

                            if (f_dataset == MRC_TRUE) {
                                if (l_seq_param != NULL) {
                                    unsigned short usdata = l_seq_param->GetInt(_T("FLIP"));
                                    l_result = f_vrleaf->SetValue(&usdata, 1);

                                    if (l_result != MRC_DBSA_SUCCESS) {
                                        CString l_str;
                                        l_str = DBSA_GetErrorString(l_result);
                        				PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                                    }
                                }

                            }

                            break;
                            */
                        //-Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
                        //DCM_MR_PLN_MOTION_SUPPRESSION (0019119B)
                    case PQM_SVN_PLN_MOTION_SUPPRESSION:

                        if (f_dataset == MRC_TRUE) {
                            if (l_seq_param != NULL) {
                                unsigned short usdata = l_seq_param->GetInt(_T("MOTION")) ;
                                l_result = f_vrleaf->SetValue(&usdata, 1);

                                if (l_result != MRC_DBSA_SUCCESS) {
                                    l_str = DBSA_GetErrorString(l_result);
                                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                                }
                            }

                        }

                        break;

                        //DCM_MR_PLN_GRID_TAG (001911A5)
                    case PQM_SVN_PLN_GRID_TAG:

                        //case SVN_PLN_GATING_METHOD:
                        if (f_dataset == MRC_TRUE) {
                            if (l_seq_param != NULL) {
                                //+Patni-Ajay/Added/09Aug2010/TMSC-REDMINE-560
                                unsigned short usdata = l_seq_param->GetInt(_T("GATE"));
                                l_result = f_vrleaf->SetValue(&usdata, 1);

                                if (l_result != MRC_DBSA_SUCCESS) {
                                    l_str = DBSA_GetErrorString(l_result);
                                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                                }
                            }

                        }

                        break;


                        //DCM_MR_PLN_WRAPAROUND_REDUCT (00211004)
                    case PQM_SVN_PLN_WRAPAROUND_REDUCT:

                        if (f_dataset == MRC_TRUE) {
                            if (l_seq_param != NULL) {
                                unsigned short usdata = l_seq_param->GetInt(_T("WRAP")) ;
                                l_result = f_vrleaf->SetValue(&usdata, 1);

                                if (l_result != MRC_DBSA_SUCCESS) {
                                    l_str = DBSA_GetErrorString(l_result);
                                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                                }
                            }

                        }

                        break;

                    case PQM_SVN_PLN_STATUS:

                        if (f_dataset == MRC_TRUE) {
                            if (l_seq_param != NULL) {
                                unsigned short pln_data = l_seq_param->GetInt(_T("PLANSTATUS"));
                                l_result = f_vrleaf->SetValue(&pln_data, 1);

                                if (l_result != MRC_DBSA_SUCCESS) {
                                    l_str = DBSA_GetErrorString(l_result);
                                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                                }

                            }

                        }

                        break;

                        //DCM_MR_PLN_DYNAMIC_SCAN (001910DA)
                    case PQM_SVN_PLN_DYNAMIC_SCAN : {
                        if (f_dataset == MRC_TRUE) {
                            if (l_seq_param != NULL) {
                                unsigned short dyn_data = l_seq_param->GetInt(_T("DYN"));
                                l_result = f_vrleaf->SetValue(&dyn_data, 1);

                                if (l_result != MRC_DBSA_SUCCESS) {
                                    l_str = DBSA_GetErrorString(l_result);
                                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                                }

                            }

                        }

                        break;
                    }

                    //+Patni-Hemant+ARD/2009Oct30/Added/IR 85 Auto Transfer
                    case PQM_SVN_PLN_AUTO_POSTPROC_FLG : {
                        if (f_dataset == MRC_TRUE) {
                            if (l_seq_param != NULL) {
                                unsigned short l_sdata = l_seq_param->GetInt(_T("APOST"));
                                l_result = f_vrleaf->SetValue(&l_sdata, 1);

                                if (l_result != MRC_DBSA_SUCCESS) {
                                    l_str = DBSA_GetErrorString(l_result);
                                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                                }
                            }
                        }
                    }
                    break;
                    //-Patni-Hemant+ARD/2009Oct30/Added/IR 85 Auto Transfer

                    default:
                        break;

                }
            }

            //+Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
            /*else if (l_unsigned_long_val) { // UL
                switch (f_vrleaf->GetTag()) {
                    case PQM_DCM_MR_SERIES_ACQ_CONDITION : {
                        if (f_dataset == MRC_TRUE) {
                            if (l_seq_param != NULL) {
                                CString s_acq_con(_T("2000"));//CPQMConfig::GetInstance()->GetAcqCondition();
                                int l_size = s_acq_con.GetLength() + 1;
                                char* l_temp = new char[l_size];
                                // MEM_INI AMIT 20081230
                                memset(l_temp, 0, sizeof(char) * l_size);
                                wcstombs(l_temp, s_acq_con, l_size);

                                unsigned long l_acq_con = atoi(l_temp);
                                //unsigned long l_series_acq_condition = 1690;
                                l_result = f_vrleaf->SetValue(&l_acq_con, 1);

            					DEL_PTR_ARY(l_temp);

                                if (l_result != MRC_DBSA_SUCCESS) {
                                    CString l_str;
                                    l_str = DBSA_GetErrorString(l_result);
            						PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
                                }
                            }

                        }

                        break;
                    }
                    default:
                        break;

                }
            }

            break;*/
            //-Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
        }
        break;

        default:
            break;
    }

    return true;
}





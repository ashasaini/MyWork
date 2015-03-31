//REDMINE-1213_Update_03_MAY
//Redmine-783
//Redmine-781_CPP_TestTool
//Redmine-774
//Redmine-778
//Redmine-779
//Redmine-780
//Redmine-782
//Redmine-781
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMStudyMgr.cpp
 *  Overview: Implementation of CPQMStudyMgr class.
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
//This class will contain all the study related information like the patient name etc.

#include "stdafx.h"
#include "PQMStudyMgr.h"
//Patni-HAR/2010Apr22/Added/MVC007503, MVC007564
#include <MPlusDicomTag/MPlusDicomSeriesTag.h>
#include <DBSALib/DBSALib.h>
//+Patni-RSG/ADDED-2009May14/JFT- Series Bias Requirement # 1
#include <DBSALib/DBSAReadTemp.h>
//-Patni-RSG/ADDED-2009May14/JFT- Series Bias Requirement # 1
#include <libStudyManager/strcnv.h> //Tanoue/MVC5849
#include <LibDS/VFToMPlusAnatomyConverter.h>
#include <PQM/PQMLogMgr.h>
#include <PQM/resource.h>
#include <PQM/PqmUtilities.h> // Added By KT/ACE-2/15-06-2009/Multilingualisation
#include "pqm.h"
#include "PQMDBSALib.h"
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"
#include "pqmconfig.h"
#include "pqmstudy.h"
#include "PqmPrMode.h"


// Replace DCM_TAG from Unicode string to ulong value
#ifdef DCM_TAG
#undef DCM_TAG
#endif
#define DCM_TAG( a, b ) ( 0x##a##b )

using namespace MRMPlus::DBSALib;
using namespace MRMPlus::DBSAVR;
using namespace MR::ACQ::SM::COMMON; //Tanoue/MVC5849

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/****************************Method Header************************************
//Method Name   :CPQMStudyMgr()
//Author        :PATNI/JS
//Purpose       :Constructor
//*****************************************************************************/
CPQMStudyMgr::CPQMStudyMgr(CPqm* pqm):
    m_wacqorder(new WORD[MAX_BUFFER]),
    m_series_bias(0),
    m_new_series_loids(new CStringArray()),        //Patni-ARD/2009June1/Added/JFT# PQM-SR GL_SR_SQSaved
    m_leafcount_series(0),
    m_series_count(0),
    m_create_series_cnt(0),
    m_stloid(NULL),
    m_dbsa_study(NULL),
    m_pqm_ptr_dbsa(pqm),
    m_totalcount(0),
    m_leafcount(0),
    m_node_count(0)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudyMgr::CPQMStudyMgr");
    PQM_TRACE1(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMStudyMgr::CPQMStudyMgr"));
}

/****************************Method Header************************************
//Method Name   :~CPQMStudyMgr()
//Author        :PATNI/JS
//Purpose       :Destructor
//*****************************************************************************/
CPQMStudyMgr::~CPQMStudyMgr(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudyMgr::~CPQMStudyMgr");
    PQM_TRACE1(SYSTEM_FUNC_MARKER, FUNC_NAME, _T("Trace of CPQMStudyMgr::~CPQMStudyMgr"));

    DEL_PTR_ARY(m_wacqorder);

    //Patni-ARD/2009June1/Added/JFT# PQM-SR GL_SR_SQSaved

    DEL_PTR(m_new_series_loids);
}

//+Patni-Sribanta/2010July13/Added/IR-149
//*****************************************************************************
//Method Name   : UpdateRealtimeSAR()
//Author        : Patni / Sribanta
//Purpose       : Write the tag values to Local DB.
//****************************************************************************
BOOL CPQMStudyMgr::UpdateRealtimeSAR(
    const int f_acq_order
) const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudyMgr::UpdateRealtimeSAR");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CPQMStudyMgr::UpdateRealtimeSAR"));

    std::vector<f32vec3_t> rtsar_params_value;
    int l_realtimesar_tag_count = m_pqm_ptr_dbsa->GetStudy()->ReadRealtimeSAR(f_acq_order, &rtsar_params_value);

    if (0 == l_realtimesar_tag_count) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_realtimesar_tag_count is 0"));
    }

    std::vector<f32vec3_t> rtsar_average_standard;
    int l_rtsar_average_standard_tag_count = m_pqm_ptr_dbsa->GetStudy()->ReadSubProtocolLevelTagValue(f_acq_order, SVN_EXE_RTSAR_AVERAGE_STANDARD_IN_SUBPROTOCOL, &rtsar_average_standard);

    if (0 == l_rtsar_average_standard_tag_count) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_rtsar_average_standard_tag_count is 0"));
    }

    f32vec3_t l_curr_value_rtsar_max_standard_in_subprotocol = {0};
    m_pqm_ptr_dbsa->GetStudy()->ReadMaxValueFromSubProtocol(f_acq_order, SVN_EXE_RTSAR_MAX_STANDARD_IN_SUBPROTOCOL, l_curr_value_rtsar_max_standard_in_subprotocol);

    f32vec3_t l_curr_value_rtsar_max_short_in_subprotocol = {0};
    m_pqm_ptr_dbsa->GetStudy()->ReadMaxValueFromSubProtocol(f_acq_order, SVN_EXE_RTSAR_MAX_SHORT_IN_SUBPROTOCOL, l_curr_value_rtsar_max_short_in_subprotocol);

    f32vec3_t l_curr_value_rtsar_peak_power_rfl = {0};
    m_pqm_ptr_dbsa->GetStudy()->ReadProtocolLevelTagValue(f_acq_order, SVN_EXE_RTSAR_PEAK_POWER_RFL, l_curr_value_rtsar_peak_power_rfl);

    f32vec3_t l_pln_rtsar_max_standard_in_subprotocol = {0};
    m_pqm_ptr_dbsa->GetStudy()->ReadMaxValueFromSubProtocol(f_acq_order, SVN_PLN_RTSAR_MAX_STANDARD_IN_SUBPROTOCOL, l_pln_rtsar_max_standard_in_subprotocol);

    f32vec3_t l_pln_rtsar_max_short_in_subprotocol = {0};
    m_pqm_ptr_dbsa->GetStudy()->ReadMaxValueFromSubProtocol(f_acq_order, SVN_PLN_RTSAR_MAX_SHORT_IN_SUBPROTOCOL, l_pln_rtsar_max_short_in_subprotocol);

    CPqmProtocol* lpqm_protocol = m_pqm_ptr_dbsa->GetStudy()->GetProtocol(f_acq_order);

    if (NULL == lpqm_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("lpqm_protocol is null"));
        return FALSE;
    }

    CDBSAVRNode* l_input_tag = NULL;

    try {
        l_input_tag = new CDBSAVRNode();

    } catch (CMemoryException* e) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("EXCEPTION in new CDBSAVRNode"));
        // "e" contains information about the exception.
        e->Delete();
        DEL_PTR(l_input_tag);
        throw;
    }

#define SET_TAG_TABLE(n,v) {n, {v.x, v.y, v.z}}

    struct tag_table_s {
        DBSATag_t name;
        f32vec3_t value;
    } const tag_table[] = {SET_TAG_TABLE(PQM_DCM_EXE_RTSAR_MAX_STANDARD_IN_SUBPROTOCOL, l_curr_value_rtsar_max_standard_in_subprotocol),

                           SET_TAG_TABLE(PQM_DCM_EXE_RTSAR_MAX_SHORT_IN_SUBPROTOCOL, l_curr_value_rtsar_max_short_in_subprotocol),

                           SET_TAG_TABLE(PQM_DCM_EXE_RTSAR_PARAMS_WB, rtsar_params_value[0]),

                           SET_TAG_TABLE(PQM_DCM_EXE_RTSAR_PARAMS_HEAD, rtsar_params_value[1]),

                           SET_TAG_TABLE(PQM_DCM_EXE_RTSAR_PARAMS_PARTIAL, rtsar_params_value[2]),

                           SET_TAG_TABLE(PQM_DCM_EXE_RTSAR_PEAK_POWER_RFL, l_curr_value_rtsar_peak_power_rfl),

                           SET_TAG_TABLE(DCM_MR_PLN_RTSAR_MAX_STANDARD_IN_SUBPROTOCOL, l_pln_rtsar_max_standard_in_subprotocol),

                           SET_TAG_TABLE(DCM_MR_PLN_RTSAR_MAX_SHORT_IN_SUBPROTOCOL, l_pln_rtsar_max_short_in_subprotocol)
                          };

#undef SET_TAG_TABLE

    const int size_tagtable = sizeof(tag_table) / sizeof(*tag_table);

    for (int index = 0; index < size_tagtable ; index++) {
        if (!SetSeriesForDBSAVRFL(l_input_tag, lpqm_protocol, tag_table[index].name, tag_table[index].value)) {
            DEL_PTR(l_input_tag);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Problem writing to database"));
            return FALSE;
        }
    }

    SYSTEMTIME	l_curr_rtsar_end_point_time = {0};
    m_pqm_ptr_dbsa->GetStudy()->GetRTSARTime(f_acq_order, l_curr_rtsar_end_point_time);

    DateTime_t l_tagvalue_rtsar_ent_point_time = {NULL, NULL, NULL, NULL, NULL, NULL, TRUE, NULL};;

    l_tagvalue_rtsar_ent_point_time.year = l_curr_rtsar_end_point_time.wYear;
    l_tagvalue_rtsar_ent_point_time.month = l_curr_rtsar_end_point_time.wMonth;
    l_tagvalue_rtsar_ent_point_time.day = l_curr_rtsar_end_point_time.wDay;
    l_tagvalue_rtsar_ent_point_time.hour = l_curr_rtsar_end_point_time.wHour;
    l_tagvalue_rtsar_ent_point_time.minute = l_curr_rtsar_end_point_time.wMinute;
    l_tagvalue_rtsar_ent_point_time.second = (l_curr_rtsar_end_point_time.wSecond +
            ((double)l_curr_rtsar_end_point_time.wMilliseconds / 1000.0));

    if (!SetSeriesForDBSAVRTM(l_input_tag, lpqm_protocol, DCM_MR_EXE_N_RTSAR_END_POINT_TIME, l_tagvalue_rtsar_ent_point_time)) {
        DEL_PTR(l_input_tag);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Problem writing to database"));
        return FALSE;
    }


    DBSAFactory    l_factory;
    CDBSATagErrorList* l_tagerrlist = NULL;
    const CString l_series_loid = lpqm_protocol->GetSeriesLOID();

    const int l_error_code = l_factory.DBSA_WriteSeries(&l_tagerrlist, l_series_loid, l_input_tag);

    if (l_error_code != MRC_DBSA_SUCCESS) {
        if (l_tagerrlist) {
            TagErrorProcess(l_tagerrlist);
            DBSA_FreeErrTagList(&l_tagerrlist);
        }

        DEL_PTR(l_input_tag);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Problem writing to database"));
        return FALSE;
    }

    DEL_PTR(l_input_tag);
    WriteSeriesForDBSAVRSQFORRTSAR(lpqm_protocol, &rtsar_average_standard);
    return TRUE;
}

//-Patni-Sribanta/2010July30/Added/IR-149
//*****************************************************************************
//Method Name   : UpdateTotalSAEValue()
//Author        :
//Purpose       : Write the tag values to Local DB.
//****************************************************************************
BOOL CPQMStudyMgr::UpdateTotalSAEValue(
    const int& f_acq_order
)
{

    CPqmProtocol* lpqm_protocol = m_pqm_ptr_dbsa->GetStudy()->GetProtocol(f_acq_order);

    int32_t l_total_sae[NUM_2] = {0};
    m_pqm_ptr_dbsa->GetStudy()->GetTotalSAEValueFromVarfield(lpqm_protocol, l_total_sae);

    if (lpqm_protocol) {

        WriteSeriesForDBSAVRUSFORSAE(lpqm_protocol, l_total_sae);

    }

    return TRUE;
}
//*****************************************************************************
//Method Name   : WriteSeriesForDBSAVRUSFORSAE()
//Author        :
//Purpose       : Write the tag values to Local DB.
//****************************************************************************
BOOL CPQMStudyMgr::WriteSeriesForDBSAVRUSFORSAE(
    CPqmProtocol* f_pqm_protocol,
    int32_t f_values[]
)
{

    if (f_pqm_protocol) {
        CString 		l_series_loid = f_pqm_protocol->GetSeriesLOID();
        unsigned short l_tagvalue[NUM_2];
        l_tagvalue[0] = (unsigned short)f_values[0];
        l_tagvalue[1] = (unsigned short)f_values[1];
        int l_error_code = 0;

        CDBSAVRNode*	l_input_tag = new CDBSAVRNode();
        CDBSAVR*		l_vrparam = new CDBSAVRUS(DCM_MR_PLN_SAE_TOTAL, sizeof(unsigned short) * 2);

        if (l_input_tag && l_vrparam) {
            l_input_tag->addLeaf(l_vrparam);
            l_error_code = l_vrparam->SetValue(l_tagvalue, 2);

            CDBSATagErrorList* l_tagerrlist = NULL;

            if (l_error_code != MRC_DBSA_SUCCESS) {
                if (l_tagerrlist) {
                    TagErrorProcess(l_tagerrlist);
                    DBSA_FreeErrTagList(&l_tagerrlist);
                }

                return FALSE;
            }

            DBSAFactory 	l_factory;
            l_error_code = l_factory.DBSA_WriteSeries(&l_tagerrlist, l_series_loid, l_input_tag);

            if (l_error_code != MRC_DBSA_SUCCESS) {
                if (l_tagerrlist) {
                    TagErrorProcess(l_tagerrlist);
                    DBSA_FreeErrTagList(&l_tagerrlist);
                }

                return FALSE;
            }

            DEL_PTR(l_input_tag);

        }
    }

    return TRUE;
}

//*****************************************************************************
//Method Name   : WriteSeriesForDBSAVRSQFORRTSAR()
//Author        :
//Purpose       : Write the tag values to Local DB.
//****************************************************************************
BOOL CPQMStudyMgr::WriteSeriesForDBSAVRSQFORRTSAR(
    CPqmProtocol* f_pqm_protocol,
    std::vector<f32vec3_t>* f_tag_values
) const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::WriteSeriesForDBSAVRSQFORRTSAR");

    if (NULL == f_pqm_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_protocol is NULL"));
        return FALSE;
    }

    CDBSAVRNode* l_input_tag_SQ = NULL;

    try {
        l_input_tag_SQ = new CDBSAVRNode();

    } catch (CMemoryException* e) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("EXCEPTION in new CDBSAVRNode"));
        e->Delete();
        DEL_PTR(l_input_tag_SQ);
        throw;
    }

    int l_error_code = 0;
    int l_index = 0;

    for (std::vector<f32vec3_t>::const_iterator itr = f_tag_values->begin(), itrEnd = f_tag_values->end(); itr != itrEnd ; ++itr, l_index++) {
        CDBSAVRNode* l_input_tag = NULL;
        CDBSAVR* l_vrparam = NULL;

        try {
            l_input_tag = new CDBSAVRNode(DCM_MR_SEQ_ACQ_COVERAGE, l_index);
            l_vrparam = new CDBSAVRFL(DCM_MR_EXE_RTSAR_AVERAGE_STANDARD_IN_SUBPROTOCOL, sizeof(float) * 3);

        } catch (CMemoryException* e) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("EXCEPTION in new CDBSAVRNode"));
            e->Delete();
            DEL_PTR(l_input_tag);
            throw;
        }

        float l_tagvalue[] = {(*itr).x, (*itr).y, (*itr).z};

        l_error_code = l_vrparam->SetValue(l_tagvalue, 3);

        if (l_error_code != MRC_DBSA_SUCCESS) {
            DEL_PTR(l_input_tag_SQ);
            return FALSE;
        }

        l_input_tag->addLeaf(l_vrparam);
        l_input_tag_SQ->addNode(l_input_tag);
    }

    DBSAFactory    l_factory;
    CDBSATagErrorList* l_tagerrlist = NULL;
    CString l_series_loid = f_pqm_protocol->GetSeriesLOID();
    l_error_code = l_factory.DBSA_WriteSeries(&l_tagerrlist, l_series_loid, l_input_tag_SQ);

    if (l_error_code != MRC_DBSA_SUCCESS) {
        if (l_tagerrlist) {
            TagErrorProcess(l_tagerrlist);
            DBSA_FreeErrTagList(&l_tagerrlist);
        }

        DEL_PTR(l_input_tag_SQ);
        return FALSE;
    }

    DEL_PTR(l_input_tag_SQ);
    return TRUE;
}

/****************************Method Header************************************
Method Name   : WriteStudyIDInlocalDB()
Author        : PATNI/Sribanta
Purpose       : This function is used to Write the Run ID in local DB when the
			study file is created.
//*****************************************************************************/
BOOL CPQMStudyMgr::WriteStudyIDInlocalDB(
    const CString& f_run_num
)const
{
    CDBSAVR* l_vrparam = m_dbsa_study->GetValue(PQM_DCM_MR_STUDY_LEAF_5);

    if (l_vrparam == NULL) {
        l_vrparam = new CDBSAVRLO(PQM_DCM_MR_STUDY_LEAF_5);
        m_dbsa_study->addLeaf(l_vrparam);
    }

    BOOL l_result = l_vrparam->SetValue(((LPCTSTR*)(&f_run_num)), 1);

    CDBSATagErrorList* l_tagerrlist = NULL;

    if (l_result != MRC_DBSA_SUCCESS) {
        if (l_tagerrlist) {
            TagErrorProcess(l_tagerrlist);
            DBSA_FreeErrTagList(&l_tagerrlist);
        }

        return FALSE;
    }


    l_result =  m_dbsa_study->DBSA_Put(&l_tagerrlist);

    if (l_result != MRC_DBSA_SUCCESS) {

        LPTSTR l_study_name_buff = NULL;
        l_vrparam->GetString(&l_study_name_buff, 0);

        return FALSE;
    }


    return TRUE;
}

//+Patni-HAR/2010May06/Added/MVC006285
//*****************************************************************************
//Method Name   : SetScanDateTimeInLocalDB()
//Author        : Patni / HAR
//Purpose       :
//*****************************************************************************
BOOL CPQMStudyMgr::SetScanDateTimeInLocalDB(
    char* f_scan_date_time
)
{
    CDBSATagErrorList* l_tagerrlist = NULL;
    BOOL l_result = TRUE;
    DBSAFactory l_factory;

    if (m_dbsa_study == NULL) {
        l_result = l_factory.DBSA_LoadStudy(&m_dbsa_study, &l_tagerrlist, PQM, m_stloid, NULL);

        if (l_result != MRC_DBSA_SUCCESS) {

            if (l_tagerrlist) {
                TagErrorProcess(l_tagerrlist);
                DBSA_FreeErrTagList(&l_tagerrlist);
            }
        }
    }

    CDBSAVR* l_vrparam = m_dbsa_study->GetValue(DCM_ST_STUDY_TIME);

    if (l_vrparam == NULL) {
        l_vrparam = new CDBSAVRTM(DCM_ST_STUDY_TIME);
        m_dbsa_study->addLeaf(l_vrparam);
    }

    CString l_scan_date_time(f_scan_date_time);

    //+ Patni-Sribanta/2010May19/Added/MVC006285

    int l_tmp_date_pos = l_scan_date_time.Find(_T("/"), 1);
    CString l_tmp_date = l_scan_date_time.Left(l_tmp_date_pos);
    l_scan_date_time.Delete(0, l_tmp_date_pos + 1);

    l_tmp_date_pos = l_scan_date_time.Find(_T("/"), 1);
    CString l_tmp_month = l_scan_date_time.Left(l_tmp_date_pos);
    l_scan_date_time.Delete(0, l_tmp_date_pos + 1);

    l_tmp_date_pos = l_scan_date_time.Find(_T(","), 1);
    CString l_tmp_year = l_scan_date_time.Left(l_tmp_date_pos);
    l_scan_date_time.Delete(0, l_tmp_date_pos + 1);

    l_tmp_date_pos = l_scan_date_time.Find(_T(":"), 1);
    CString l_tmp_hour = l_scan_date_time.Left(l_tmp_date_pos);
    l_scan_date_time.Delete(0, l_tmp_date_pos + 1);

    l_tmp_date_pos = l_scan_date_time.Find(_T(":"), 1);
    CString l_tmp_min = l_scan_date_time.Left(l_tmp_date_pos);
    l_scan_date_time.Delete(0, l_tmp_date_pos + 1);

    CString l_tmp_sec = l_scan_date_time;


    DateTime_t l_scan_date_time_1 = {NULL, NULL, NULL, NULL, NULL, NULL, TRUE, NULL};

    l_scan_date_time_1.day = _wtoi(l_tmp_date);
    l_scan_date_time_1.hour = _wtoi(l_tmp_hour);
    l_scan_date_time_1.minute = _wtoi(l_tmp_min);
    l_scan_date_time_1.month = _wtoi(l_tmp_month);
    l_scan_date_time_1.offsetExist = TRUE;
    l_scan_date_time_1.offsetvalue = 0;
    l_scan_date_time_1.second = _wtoi(l_tmp_sec);
    l_scan_date_time_1.year = _wtoi(l_tmp_year);
    l_result = l_vrparam->SetValue(l_scan_date_time_1, 0);

    //- Patni-Sribanta/2010May19/Added/MVC006285

    if (l_result != MRC_DBSA_SUCCESS) {
        if (l_tagerrlist) {
            TagErrorProcess(l_tagerrlist);
            DBSA_FreeErrTagList(&l_tagerrlist);
        }

        return l_result;
    }

    l_result =  m_dbsa_study->DBSA_Put(&l_tagerrlist);

    if (l_result != MRC_DBSA_SUCCESS) {

        LPTSTR l_study_name_buff = NULL;
        l_vrparam->GetString(&l_study_name_buff, 0);

        return l_result;
    }

    return l_result;
}
//-Patni-HAR/2010May06/Added/MVC006285

//+Patni-HAR/2010Mar26/Added/MVC007422
//****************************Method Header************************************
//Method Name   : ReadSeriesNo()
//Author        : PATNI / HAR
//Purpose       : Reads Series number from Local DB
//*****************************************************************************
BOOL CPQMStudyMgr::ReadSeriesNo(
    BSTR f_series_loid,
    long& f_series_no
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudyMgr::ReadSeriesNo");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("[Series LOID : ]") + CString(f_series_loid));


    CDBSAVRNode*		l_vr_node		= new CDBSAVRNode();
    CDBSAVRLeaf*        l_leaf			= new CDBSAVRIS(PQM_DCM_ST_SERIES_NUMBER);
    CDBSASeries*		l_dbsa_series	= NULL;
    CDBSATagErrorList*	l_errlist		= NULL;
    BOOL				l_status		= FALSE;

    l_vr_node->addLeaf(l_leaf);
    DBSAFactory			l_factory;
    int l_result = l_factory.DBSA_LoadSeries(&l_dbsa_series, &l_errlist, PQM, f_series_loid, l_vr_node);

    if (l_result == MRC_DBSA_SUCCESS && l_dbsa_series != NULL) {
        CDBSAVR* l_vrparamBias  = NULL ;
        l_result = l_dbsa_series->DBSA_GetValue(&l_vrparamBias, PQM_DCM_ST_SERIES_NUMBER, 1, &l_errlist);

        if (l_result == MRC_DBSA_SUCCESS && l_vrparamBias != NULL) {
            l_result = l_vrparamBias->GetValue(&f_series_no, 1);

            if (l_result == MRC_DBSA_SUCCESS) {
                l_status = TRUE;

            } else {
                l_status = FALSE;

                if (l_errlist) {
                    TagErrorProcess(l_errlist);
                    DBSA_FreeErrTagList(&l_errlist);
                }

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("[GetValue() Failed]"));
            }

        } else {
            l_status = FALSE;

            if (l_errlist) {
                TagErrorProcess(l_errlist);
                DBSA_FreeErrTagList(&l_errlist);
            }

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("[DBSA_GetValue() Failed]"));
        }

    } else {
        l_status = FALSE;

        if (l_errlist) {
            TagErrorProcess(l_errlist);
            DBSA_FreeErrTagList(&l_errlist);
        }

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("[DBSA_LoadSeries() Failed]"));
    }

    DEL_PTR(l_leaf);

    return l_status;
}

//****************************Method Header************************************
//Method Name   : ReadProtectedStatus()
//Author        : IGATE/SS
//Purpose       : Read protected tag value from local database
///*****************************************************************************
BOOL CPQMStudyMgr::ReadProtectedStatus(
    BSTR f_series_loid
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudyMgr::ReadProtectedStatus");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("[Series LOID : ]") + CString(f_series_loid));

    CDBSASeries*		l_dbsa_series	= NULL;
    CDBSATagErrorList*	l_errlist		= NULL;
    BOOL				l_status		= FALSE;

    DBSAFactory			l_factory;

    CDBSAVRNode*		l_vr_node		= new CDBSAVRNode();
    CDBSAVRLeaf*        l_leaf			= new CDBSAVRUS(DCM_MP_PROTECTED_STATUS);
    l_vr_node->addLeaf(l_leaf);
    int l_result = l_factory.DBSA_LoadSeries(&l_dbsa_series, &l_errlist, PQM, f_series_loid, l_vr_node);

    if (l_result == MRC_DBSA_SUCCESS && l_dbsa_series != NULL) {
        CDBSAVR* l_vparam_prot_status  = NULL ;
        l_result = l_dbsa_series->DBSA_GetValue(&l_vparam_prot_status, DCM_MP_PROTECTED_STATUS , 1 , &l_errlist);

        if (l_result == MRC_DBSA_SUCCESS && l_vparam_prot_status != NULL) {
            unsigned short l_protectedStatus = 0;
            l_result = l_vparam_prot_status->GetValue(&l_protectedStatus, 1);

            if (l_result == MRC_DBSA_SUCCESS) {
                if (1 == l_protectedStatus) {

                    l_status = TRUE;
                }

            } else {


                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("[GetValue() Failed]"));

            }

        } else {


            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("[DBSA_GetValue() Failed]"));

        }

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("[DBSA_LoadSeries() Failed]"));
    }

    if (l_errlist) {
        TagErrorProcess(l_errlist);
        DBSA_FreeErrTagList(&l_errlist);
    }




    DBSA_FreeSeries(l_dbsa_series);
    l_dbsa_series = NULL;

    DEL_PTR(l_vr_node);

    return l_status;
}

//****************************Method Header************************************
//Method Name   : UpdateIECInfo()
//Author        : PATNI / HAR
//Purpose       : Update SAR, dBdt and Series Number information in Local DB for
//scanned protocol
//*****************************************************************************
int CPQMStudyMgr::UpdateIECInfo(
    const long f_series_number,
    const int f_sar_op_mode,
    const int f_dbdt_op_mode
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudyMgr::UpdateIECInfo");

    CString l_print_str(_T(""));
    l_print_str.Format(_T("[db/dt Operating Mode : %d]\
				[SAR Operating Mode : %d][Series Number : %ld]"),
                       f_dbdt_op_mode, f_sar_op_mode, f_series_number);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              l_print_str);

    int					l_result		= -1;


    if (m_dbsa_study) {
        CDBSATagErrorList*	l_errlist		= NULL;
        CDBSAVR*			l_vrparamBias	= NULL;
        l_result = m_dbsa_study->DBSA_GetValue(&l_vrparamBias, PQM_DCM_MR_SEQ_IEC_INFO, MRC_TRUE, &l_errlist);

        if (l_result == MRC_DBSA_SUCCESS) {
            unsigned short*		l_data_array	= NULL;
            unsigned short*		l_seriesBias	= NULL;
            int					l_size			= 0;

            if (l_vrparamBias != NULL) {

                l_size = l_vrparamBias->GetSize();

                l_seriesBias = new unsigned short[l_size];
                memset(l_seriesBias, 0, sizeof(unsigned short) * l_size);

                l_vrparamBias->GetValue(l_seriesBias, l_size);
                int l_total_size = l_size + 3;

                l_data_array = new unsigned short[l_total_size];
                memset(l_data_array, 0, sizeof(unsigned short) * (l_total_size));

                for (int i = 0; i < l_size; i++) {
                    l_data_array[i] = l_seriesBias[i];
                }

            } else {
                if (!l_vrparamBias) {

                    l_vrparamBias = new CDBSAVRUS(PQM_DCM_MR_SEQ_IEC_INFO, DCM_VR_LEN_US);
                    m_dbsa_study->addLeaf(l_vrparamBias);
                }

                l_data_array = new unsigned short[3];
                memset(l_data_array, 0, sizeof(unsigned short) * 3);
                l_size = 0;
            }

            unsigned short		l_series_number = (unsigned short)f_series_number;
            l_data_array[l_size++]	= l_series_number;
            l_data_array[l_size++]	= (unsigned short)f_sar_op_mode;
            l_data_array[l_size++]	= (unsigned short)f_dbdt_op_mode;

            l_result = l_vrparamBias->SetValue(l_data_array, l_size);

            if (l_result != MRC_DBSA_SUCCESS) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("[SetValue() Failed]"));

            } else {
                l_result = m_dbsa_study->DBSA_Put(&l_errlist);

                if (l_result != MRC_DBSA_SUCCESS) {

                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                              _T("[DBSA_Put() Failed]"));

                    if (l_errlist) {
                        TagErrorProcess(l_errlist);
                        DBSA_FreeErrTagList(&l_errlist);
                    }
                }
            }

            DEL_PTR_ARY(l_seriesBias);
            DEL_PTR_ARY(l_data_array);

        } else {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("[DBSA_GetValue() Failed]"));
        }
    }

    return l_result;
}

/****************************Method Header************************************
//Method Name   : WriteSeriesInformationAtSeriesLevel()
//Author        : PATNI / HAR
//Purpose       :
//*****************************************************************************/
BOOL CPQMStudyMgr::WriteSeriesInformationAtSeriesLevel(
    const CString& f_series_loid
)
{
    CDBSAVRNode* l_input_tag = new CDBSAVRNode();

    CDBSAVR* l_vr_station_name = new CDBSAVRSH(DCM_ST_STATION_NAME);
    l_input_tag->addLeaf(l_vr_station_name);


    char*   l_site_path_env = getenv("GP_SITE") ;
    CString l_site_path(l_site_path_env);
    LPCTSTR l_path = l_site_path.GetBuffer(l_site_path.GetLength() + 1);
    BOOL l_result = l_vr_station_name->SetValue(&l_path, 1);

    CString l_system_type(m_pqm_ptr_dbsa->GetSysName());

    CDBSAVR* l_manufac_name     = new CDBSAVRLO(DCM_ST_MANUFACTURERS_MODEL_NAME);
    l_input_tag->addLeaf(l_manufac_name);

    LPCTSTR l_model_name = l_system_type.GetBuffer(l_system_type.GetLength() + 1);
    l_result = l_manufac_name->SetValue(&l_model_name, 1);

    CDBSATagErrorList* l_tagerrlist = NULL;
    DBSAFactory l_factory;
    l_result = l_factory.DBSA_WriteSeries(&l_tagerrlist, f_series_loid, l_input_tag);

    if (l_result != MRC_DBSA_SUCCESS) {
        if (l_tagerrlist) {
            TagErrorProcess(l_tagerrlist);
            DBSA_FreeErrTagList(&l_tagerrlist);
        }

        l_result =  FALSE;
    }

    l_site_path.ReleaseBuffer();
    l_system_type.ReleaseBuffer();
    DEL_PTR(l_vr_station_name);
    DEL_PTR(l_manufac_name);
    return l_result;
}

//+Patni-AMT/2009Apr20/Added/PSP1#001
//****************************Method Header*************************************
//Method Name   : CheckSeriesInMPlusDB()
//Author        : PATNI/AMT
//Purpose       : This function will load the series specified by f_series_loid
//                  to check that serie is preent in MPlus DB.
//*****************************************************************************/
BOOL CPQMStudyMgr::CheckSeriesInMPlusDB(
    //Patni-DKH/2010Sep24/Modified/TMSC-REDMINE-756
    LPCTSTR f_series_loid , DBSAIndex_t& f_series_index
)
{

    CDBSASeries*         l_series = NULL;
    DBSAFactory         l_factory;
    CDBSATagErrorList*   l_tagerrlist   = NULL;

    CDBSAVRNode*		l_vr_node		= new CDBSAVRNode();
    CDBSAVRLeaf*        l_leaf			= new CDBSAVRUI(DCM_ST_SERIES_INSTANCE_UID);

    l_vr_node->addLeaf(l_leaf);
    BOOL l_ret_value = (l_factory.DBSA_LoadSeries(&l_series, &l_tagerrlist, PQM, f_series_loid, l_vr_node) != MRC_DBSA_SUCCESS) ? FALSE : TRUE ;

    if (TRUE == l_ret_value) {
        f_series_index = l_series->DBSA_GetIndex();
        l_series->DBSA_FreeLowObject();
    }


    DEL_PTR(l_vr_node);

    if (l_tagerrlist) {
        TagErrorProcess(l_tagerrlist);
        DBSA_FreeErrTagList(&l_tagerrlist);
    }

    DBSA_FreeSeries(l_series);
    l_series = NULL;

    return l_ret_value;
}   //-Patni-AMT/2009Apr20/Added/PSP1#001

//+Patni-DKH/2010Sep24/Modified/TMSC-REDMINE-756
//****************************Method Header************************************
//Method Name   :UpdateAcqOrderInLocalDB()
//Author        :PATNI/DKH
//Purpose       :To Update acqorder in Local DB
//*****************************************************************************
void CPQMStudyMgr::UpdateAcqOrderInLocalDB(
    WORD* f_acqorder, const int f_acqordersize
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudyMgr::UpdateAcqOrderInLocalDB");

    CString l_strlog(_T("")), l_stracqorderlog(_T(""));


    if (NULL == f_acqorder) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("f_acqorder is NULL"));
        return;
    }

    l_strlog.Format(_T("AcqSize : %d"), f_acqordersize);
    l_stracqorderlog = ConvertAcqOrderToString(f_acqorder, f_acqordersize);
    l_stracqorderlog += l_strlog ;

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              l_stracqorderlog);

    m_dbsa_study->DBSA_AcqOrderUpdate(f_acqorder, f_acqordersize);
}
//-Patni-DKH/2010Sep24/Modified/TMSC-REDMINE-756
//+Patni-DKH/2010Sep30/Added/TMSC-REDMINE-756 TMSC Review comments
//****************************Method Header************************************
//Method Name   :ConvertAcqOrderToString()
//Author        :PATNI/DKH
//Purpose       :To return the acqorder in string format
//*****************************************************************************
CString CPQMStudyMgr::ConvertAcqOrderToString(
    const WORD* f_acqorder,
    const int f_acqordersize) const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudyMgr::ConvertAcqOrderToString");

    CString l_buf(_T(""));

    if (NULL == f_acqorder) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("f_acqorder is NULL"));
        return l_buf;
    }

    TCHAR l_tmpbuff[MAX_BUFFER] = {0};
    int l_loc = 0;

    // for writing l_acqorder in log
    while (l_loc < f_acqordersize) {

        _ltow(f_acqorder[l_loc], l_tmpbuff, NUM_10);
        l_buf += l_tmpbuff;
        l_buf += _T(", ");
        l_loc++;
    }

    return l_buf ;
}
//-Patni-DKH/2010Sep30/Added/TMSC-REDMINE-756 TMSC Review comments

/****************************Method Header************************************
//Method Name   :LoadStudyLOID()
//Author        :PATNI/RD
//Purpose       :This method Loads the instance of CPQMDBSALib and reads the information
//               related to the series in the respective data members of the Series
//*****************************************************************************/
bool CPQMStudyMgr::LoadStudyLOID(
    LPCTSTR f_loid,
    const bool f_flag
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudyMgr::LoadStudyLOID");

    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    UNREFERENCED_PARAMETER(f_flag);
    CString l_str_msg(_T(""));
    l_str_msg.Format(_T("%s"), f_loid);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str_msg);

    //CPASParamStore *l_objParamStore = new CPASParamStore();         //Patni-RUP/2009Aug20/Modified/cpp test corrections
    if (m_dbsa_study) {

        //	if(wcscmp(f_loid, m_dbsa_study->DBSA_GetLOID()) != 0){
        DBSA_FreeStudy(m_dbsa_study);
        m_dbsa_study = NULL;
        //	}
    }

    if (m_dbsa_study == NULL) {
        CDBSATagErrorList* l_tagerrlist = NULL;
        DBSAFactory l_factory;
        int l_result = l_factory.DBSA_LoadStudy(&m_dbsa_study, &l_tagerrlist, PQM, f_loid, NULL);

        if (l_result != MRC_DBSA_SUCCESS) {

            if (l_tagerrlist) {
                TagErrorProcess(l_tagerrlist);
                DBSA_FreeErrTagList(&l_tagerrlist);
            }
        }
    }

    if (m_dbsa_study == NULL) {

        CPQMLogMgr::GetInstance()->WriteEventLog(
            _T("Study LOID pointer is null, unable to load study LOID."),
            ERROR_LOG_INFORMATION1, _T("PQMStudyMgr.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82


        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_STUDY_LOID_POINTER")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return FALSE;
    }

    //+Patni-SS/MODI-ADDED/2009-May-22/AcqOrder problem
    //+Patni-AMT/2009Mar27/Added/PSP1#307 Code Review.
    //m_series_count =  m_dbsa_study->DBSA_ObjectCount();
    WORD* l_acqorder = NULL;
    int l_acqorder_size = m_dbsa_study->DBSA_GetAcqOrder(&l_acqorder);
    m_series_count = l_acqorder_size;

    //-Patni-SS/MODI-ADDED/2009-May-22/AcqOrder problem
    //+Patni-DKH/Added/20010-Oct-05/TMSC-REDMINE-756-Part1
    if (NULL != l_acqorder) {
        DBSA_FreeLocalAcqOrder(l_acqorder);
    }

    //-Patni-DKH/Added/20010-Oct-05/TMSC-REDMINE-756-Part1

    //Now get the BIAS from the Study
    //+Patni-RSG/Modified-2009May14/JFT- Series Bias Requirement # 1
    const bool res = SetSeriesBiasVal();

    WriteCharSetAtStudyLevel();

    return TRUE;
    //-Patni-AMT/2009Mar27/Added/PSP1#307 Code Review.
}

void CPQMStudyMgr::FreeStudyLoid(
)
{

    if (m_dbsa_study) {

        DBSA_FreeStudy(m_dbsa_study);
        m_dbsa_study = NULL;
    }
}

/****************************Method Header************************************
//Method Name   :DuplicateProtocol()
//Author        :PATNI/RD
//Purpose       :Duplicate Protocol
//*****************************************************************************/
BOOL CPQMStudyMgr::DuplicateProtocol(
    CPASCOMSequence* f_dup_seq_List,
    const int f_selindex,
    const int f_index_duplicating_prot
)
{

    LPCTSTR FUNC_NAME = _T("CPQMStudyMgr::DuplicateProtocol");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, m_stloid);

    BOOL bResult = true;

    try {

        CString l_str;
        l_str.Format(_T("Input Params : f_selindex = %d, f_index_duplicating_prot = %d"), f_selindex, f_index_duplicating_prot);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);

        //+Patni-MJC/2009Aug17/Modified/cpp test corrections
        if (m_new_series_loids == NULL || m_dbsa_study == NULL) {
            return FALSE;
        }

        //Patni-ARD/2009June1/Added/JFT# PQM-SR GL_SR_SQSaved
        WORD* l_acqorder = NULL;
        int l_acqorder_size = m_dbsa_study->DBSA_GetAcqOrder(&l_acqorder);
        int l_seriescnt = l_acqorder_size;


        m_new_series_loids->RemoveAll();
        long l_maxprotocol_limit = CPQMConfig::GetInstance()->ReadMaxProtocolLimitfromFile();

        CString l_buf = ConvertAcqOrderToString(l_acqorder, l_acqorder_size);
        CString l_stracqsize(_T(""));
        l_stracqsize.Format(_T("AcqSize : %d"), l_acqorder_size);
        l_buf += l_stracqsize;
        //+Patni-DKH/2010Sep30/Modified/TMSC-REDMINE-756 TMSC Review comments

        //+ MEITEC/2010Jun23/Modified/MVC008324_1
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Current ACQ order in Duplicate : ") + l_buf);

        //- MEITEC/2010Jun23/Modified/MVC008324_1

        //+Patni-SS/COMMENTED/2009-May-22/AcqOrder problem

        if (/*m_dbsa_study->DBSA_ObjectCount()*/l_acqorder_size == l_maxprotocol_limit)    {
            bResult = FALSE;
            CPQMLogMgr::GetInstance()->DisplayScanError(IDS_MAX_PROTOCOL, _T("IDS_MAX_PROTOCOL"), SCAN_ERR_NOTIFY, _T("PQMStudyMgr.cpp"));
            return bResult;
        }

        //+Patni-SS/COMMENTED/2009-May-22/AcqOrder problem
        //l_seriescnt = m_dbsa_study->DBSA_ObjectCount();

        CPQMDBSALib* l_pqm_dbsalib_se = new CPQMDBSALib(MRC_DBSA_SERIES);
        const int l_leafcount_series = g_sequence_param_name_list.GetCount();
        SetLeafCountSeries(l_leafcount_series);
        l_pqm_dbsalib_se->SetPascomSequence(f_dup_seq_List);
        CDBSAVRNode* l_seriesnode = l_pqm_dbsalib_se->MakeTagXML(l_leafcount_series, 1, MRC_TRUE);

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("MakeTagXML Completed"));

        DEL_PTR(l_pqm_dbsalib_se);

        //Patni-RSG/2009Aug19/Modified/ACE# MVC3381
        LPCTSTR l_loid = NULL;
        int l_delete_count = 1;

        //+Patni-ARD/2009Jun19/Modified/JFT# DBServer access issue
        CString l_log_msg = _T("");

        CDBSATagErrorList* l_tagerrlist = NULL;
        CDBSASeries* l_series = NULL;

        int l_return_status = 0;
        int l_delete_status = 0;

        for (int i = 0; i < 3; i++) {

            l_return_status = m_dbsa_study->DBSA_CreateSeries(&l_series, &l_tagerrlist, PQM,
                              l_seriesnode);


            if (l_return_status == MRC_DBSA_SUCCESS) {
                break;
            }

            //+Patni-RSG/2009Aug19/Modified/ACE# MVC3381
            if (l_series) {
                l_loid = l_series->DBSA_GetLOID();

                if (l_loid) {
                    l_delete_status = 0;
                    l_delete_status = m_dbsa_study->DBSA_DeleteLowObject(&l_delete_count, &l_loid);

                    if (l_delete_status != MRC_DBSA_SUCCESS) {
                        l_log_msg.Empty();
                        l_log_msg.Format(_T("Unable To delete Series LOID= "));
                        l_log_msg += l_loid;
                        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_msg);
                    }
                }

                l_series->DBSA_FreeLowObject();
                l_series = NULL;
            }

            //+Patni-RSG/2009Aug19/Modified/ACE# MVC3381

            if (l_tagerrlist) {
                TagErrorProcess(l_tagerrlist);
                DBSA_FreeErrTagList(&l_tagerrlist);
            }

            l_log_msg.Empty();
            l_log_msg.Format(_T("DBSA_CreateSeries() - Retry Count = %d"), i + 1);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_msg);
        }

        CString temp_string = _T("");		// Added By KT/ACE-2/15-06-2009/Multilingualisation

        if (l_return_status != MRC_DBSA_SUCCESS || NULL == l_series) {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("DBSA_CreateSeries != MRC_DBSA_SUCCESS"));

            CPQMLogMgr::GetInstance()->DisplayError(
                CPqmUtilities::GetMultiLingualString(_T("IDS_UNABLE_TO_DUPLICATE_SERIES")),
                _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
                ERR_ID, APP_NAME);

            l_buf.Empty();
            l_buf = f_dup_seq_List->GetSequenceParam()->GetString(_T("ACQORDER"));
            int l_del_index = _ttoi(l_buf);

            /*
            CPqmProtocol* l_del_prot =
                m_pqm_ptr_dbsa->GetStudy()->GetItemFromPosition(m_series_count+1);
            int l_del_index = l_del_prot->GetProtocol();
            */
            l_log_msg.Empty();
            l_log_msg.Format(_T("Deleting protocol from Varfield = %d "),
                             l_del_index);

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_msg);

            m_pqm_ptr_dbsa->GetStudy()->DeleteProt(l_del_index);

            DEL_PTR(l_series);
            DEL_PTR(l_seriesnode);

            return FALSE;

        } else {

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("DBSA_CreateSeries = MRC_DBSA_SUCCESS"));

            //+Patni-RUP/2009Aug21/Modified/cpp test corrections
            if (l_series == NULL) {

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("l_series pointer is NULL"));
                return FALSE;
            }

            //-Patni-RUP/2009Aug21/Modified/cpp test corrections

            CString loid(l_series->DBSA_GetLOID());
            f_dup_seq_List->SetSequenceID(loid);
            //+Patni-SS/MODI/2009-May-22/AcqOrder problem
            m_series_count++;
            //m_series_count = m_dbsa_study->DBSA_ObjectCount();
            //+Patni-SS/MODI/2009-May-22/AcqOrder problem
            //Patni-ARD/2009June1/Added/JFT# PQM-SR GL_SR_SQSaved
            //Patni-ARD/2009Jun09/Modified/JFT# PQM SR Communication GL_SR_SQSaved
            m_new_series_loids->Add(loid);
            //-Patni-ARD/2009June1/Added/JFT# PQM-SR GL_SR_SQSaved
        }

        //-Patni-ARD/2009Jun19/Modified/JFT# DBServer access issue

        if (l_series == NULL) {
            DEL_PTR(l_seriesnode);

        } else {

            if (l_series->DBSA_GetLOID() == NULL) {
                DEL_PTR(l_seriesnode);
            }

            int idxtar = 0;
            int l_protocol_pos = 0;
            int l_exact_series_count = 0;

            if (f_selindex == 0)
                l_exact_series_count = l_seriescnt;

            else
                l_exact_series_count = m_series_count;

            for (int idx = 0; idx < l_exact_series_count; idx++, idxtar++) {
                if (idx == f_selindex && f_selindex != 0) {
                    m_wacqorder[idx] = l_series->DBSA_GetIndex();
                    l_protocol_pos = idx;
                    idxtar = idx + 1;
                }

                if ((idxtar  < l_exact_series_count) && (idx  < l_acqorder_size)) {
                    m_wacqorder[idxtar] = l_acqorder[idx];
                }
            }

            // if f_selindex is 0 then new created series will be added at end
            if (f_selindex == 0) {
                m_wacqorder[idxtar] = l_series->DBSA_GetIndex();
                l_protocol_pos = idxtar;
            }

            //+Patni-DKH/Added/20010-Oct-05/TMSC-REDMINE-756-Part1
            if (NULL != l_acqorder) {
                DBSA_FreeLocalAcqOrder(l_acqorder);
            }

            //-Patni-DKH/Added/20010-Oct-05/TMSC-REDMINE-756-Part1

            ///////////////////////Added by Sarjeet For seting the HOLD Value//////////
            CPqmProtocol* l_var_series = m_pqm_ptr_dbsa->GetStudy()->GetItemFromPosition(l_protocol_pos + 1);// l_protocol_pos is zero base index

            l_log_msg.Format(_T("%d"), l_var_series->GetProtocol());												 //and GetItemFromPosition take 1 base index
            f_dup_seq_List->GetSequenceParam()->SetString(_T("ACQORDER"), l_log_msg);

            //+Patni/SS/2010July06/Added/NFD001-AutoMap
            temp_string.Format(_T("l_protocol_pos = %d , Protocol Position = %d , Protocol Index = %d"), l_protocol_pos, l_var_series->GetPosition(), l_var_series->GetProtocol());

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, temp_string);

            //-Patni/SS/2010July06/Added/NFD001-AutoMap
            f_dup_seq_List->GetSequenceParam()->SetInt(_T("HOLD"), l_var_series->GetHoldScan());
            //+Tanoue/MVC5849
            UTIL::Mbs2Wcs(&l_log_msg, l_var_series->GetContrastName());
            f_dup_seq_List->GetSequenceParam()->SetString(_T("CONTRASTNAME"), l_log_msg);
            //-Tanoue/MVC5849
            f_dup_seq_List->GetSequenceParam()->SetInt(_T("AUTOVOICE"), l_var_series->GetExeAutoVoice());

            //Ashish changes Defect Fix # 564 Start
            CString l_str_scanoffset(_T(""));
            f32vec3_t l_offset = l_var_series->GetOffset();

            l_str_scanoffset.Format(_T("%f,%f,%f"), l_offset.x * 100, l_offset.y * 100, l_offset.z * 100);
            f_dup_seq_List->GetSequenceParam()->SetString(_T("SCANOFFSET"), l_str_scanoffset);
            //Ashish changes Defect Fix # 564 End

            //+Patni-PANKAJ(PJS)/2010May28/Added/MaFT/MVC008019
            f_dup_seq_List->GetSequenceParam()->SetString(_T("MEMO"), l_var_series->GetScanComment());
            f_dup_seq_List->GetSequenceParam()->SetString(_T("COMM"), l_var_series->GetScanID());
            //-Patni-PANKAJ(PJS)/2010May28/Added/MaFT/MVC008019

            //Patni-RUP/2010Jul27/Added/TMSC-REDMINE-466
            f_dup_seq_List->GetSequenceParam()->SetInt(_T("COUCHMETHOD"), l_var_series->GetcouchMethodFlg());

            //////////////////////Sarjeet/////////////////////////////////////////////
            // add series reference in to study file and protocol reference into series
            if (AddSeriesReference(l_var_series,
                                   (LPCTSTR)l_series->DBSA_GetLOID()) != E_NO_ERROR) {

                l_series->DBSA_FreeLowObject();
                l_series = NULL;
                return false;
            }

            CPqmProtocol* l_prev_prot = m_pqm_ptr_dbsa->GetStudy()->GetItemFromPosition(f_index_duplicating_prot + 1);

            if (l_prev_prot && m_pqm_ptr_dbsa->GetStudy()->IsProtocolInScan(l_prev_prot->GetProtocol())) {
                AddWFDACAPPValue(l_prev_prot->GetSeriesLOID(), l_var_series->GetSeriesLOID());
            }

            f_dup_seq_List->GetSequenceParam()->SetInt(_T("GROUPID"), l_var_series->GetGroupID());
            f_dup_seq_List->GetSequenceParam()->SetInt(_T("MASTERFLAG"), l_var_series->GetMasterFlag());
            f_dup_seq_List->GetSequenceParam()->SetString(_T("DELAYTIME"), l_var_series->GetDelayTime());
            f_dup_seq_List->GetSequenceParam()->SetInt(_T("MSDENABLEFLAG"), l_var_series->GetMultiSeriesDynamicFlag());

            //+Patni-Hemant/2009Nov13/Added/IR 85 Auto Transfer
            //by default for duplicate series data is copied but
            //if the autojob is not present then we need to reset
            //flag in db as well as in passcom param
            if (!CPQMConfig::GetInstance()->CanEnableAutoTransfer()) {
                int l_data = 0l;
                f_dup_seq_List->GetSequenceParam()->SetInt(_T("APOST"), l_data);
                l_var_series->SetAutoTransfer(l_data);

                //Note :UpdateAutoTransferValueInMPlusDB() should be called after AddSeriesReference()
                //mplus db updation should be made after that only.
                if (!UpdateAutoTransferValueInMPlusDB(l_var_series)) {
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                              _T("UpdateAutoTransferValueInMPlusDB() : Failed to update auto transfer tag in MPLUSDB"));
                }
            }

            //-Patni-Hemant/2009Nov13/Added/IR 85 Auto Transfer
            // + SM4 Himanshu 10 Dec 2008
            CPqmShimSharedData* l_pqmshimshareddat = CPqmShimSharedData::GetInstance();
            int f_scan_number = 0;
            float f_couch_pos = PQM_COUCH_INIT_POSITION;
            float f_home_pos = PQM_COUCH_INIT_POSITION;
            l_pqmshimshareddat->CopyCouchInfo(&f_scan_number, &f_couch_pos, &f_home_pos);

            //Patni-ARD/2009Dec7/Modified/DeFT# MVC006086 + self review
            AddAcqConAndHomePosAtSeriesLevel(l_var_series->GetSeriesLOID(), f_home_pos, f_scan_number);

            // - SM4 Himanshu 10 Dec 2008
            l_buf.Empty();

            //Patni-DKH/2010Sep30/Modified/TMSC-REDMINE-756 TMSC Review comments
            l_buf = ConvertAcqOrderToString(m_wacqorder, m_series_count);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_buf);

            temp_string.Format(_T("%d"), m_series_count);

            if (l_series) {
                l_series->DBSA_FreeLowObject();
                l_series = NULL;
            }

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Final Series Count in Duplicate: ") + temp_string);

            //+Patni-PJS/2010Feb05/Added/DeFT# PQM crash resolved
            try {
                m_dbsa_study->DBSA_AcqOrderUpdate(m_wacqorder, m_series_count);
                l_return_status =  m_dbsa_study->DBSA_Put(&l_tagerrlist);

                if (l_return_status != MRC_DBSA_SUCCESS) {
                    if (l_tagerrlist) {
                        TagErrorProcess(l_tagerrlist);
                        DBSA_FreeErrTagList(&l_tagerrlist);
                    }

                    CPQMLogMgr::GetInstance()->DisplayError(
                        CPqmUtilities::GetMultiLingualString(_T("IDS_UNABLE_TO_DUPLICATE_SERIES")),
                        _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
                        ERR_ID, APP_NAME);
                    bResult = false;
                }

            } catch (...) {


                CPQMLogMgr::GetInstance()->DisplayError(
                    CPqmUtilities::GetMultiLingualString(_T("IDS_UNABLE_TO_DUPLICATE_SERIES")),
                    _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
                    ERR_ID, APP_NAME);

                bResult = false;
            }

            //-Patni-PJS/2010Feb05/Added/DeFT# PQM crash resolved
            //+Patni-SS/COMMENTED/2009-May-22/AcqOrder problem
            //m_series_count = m_dbsa_study->DBSA_ObjectCount(); // this to get final series count in study file after duplicate operation has done.
            TCHAR l_srmsg[NUM_100] = {0};
            swprintf(l_srmsg, L"%s+%d", L"GL_SR_SQSaved", m_series_count);
            m_pqm_ptr_dbsa->WriteToUI(PQM_MSG_GL_SR_SQSAVED);

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_srmsg);

            DEL_PTR(l_seriesnode);
        }

    } catch (...) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("EXCEPTION Occured..."));
        throw;
    }

    return bResult;
}

/****************************Method Header************************************
//Method Name   :MakeSeries()
//Author        :PATNI/RD
//Purpose       :Creates Series under specified study
//*****************************************************************************/
void CPQMStudyMgr::MakeSeries(
    SequenceList* f_snd_seq_List,
    LPCTSTR f_studyloid,
    const mode_automap f_mode,
    const int f_targetpos
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudyMgr::MakeSeries");

    CString l_errstr(_T(""));
    CString temp_string = _T("");																   // Added By KT/ACE-2/15-06-2009/Multilingualisation


    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, f_studyloid);

    //+ Patni-RJ/2009Apr02/Added/PSP- Performance code
#ifdef __SM_PERFORMANCE_CHECK__
    __int64 l_freq = 0, l_stop = 0, l_start = 0;
    int l_total_series_count = f_snd_seq_List->GetCount();
    QueryPerformanceCounter((LARGE_INTEGER*)&l_start);
#endif

    //- Patni-RJ/2009Apr02/Added/PSP- Performance code
    // + PSP1 AMIT + HIMANSHU Code Optimization 20090218
    if (f_snd_seq_List == NULL || f_snd_seq_List->GetCount() == 0) {
        l_errstr.Format(_T("f_snd_seq_List is  NULL or its empty"));
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_errstr);

        return ;
    } // - PSP1 AMIT + HIMANSHU Code Optimization 20090218

    CDBSATagErrorList* l_tagerrlist = NULL;
    CDBSAVRNode* l_seriesnode = NULL;
    int l_return_status = 0;

    if (m_dbsa_study == NULL) {

        DBSAFactory l_factory;
        l_return_status = l_factory.DBSA_LoadStudy(&m_dbsa_study, &l_tagerrlist,
                          PQM, f_studyloid, l_seriesnode);

        if (l_return_status != MRC_DBSA_SUCCESS) {
            // + Intgr 6 Bug Fix Lokesh
            if (l_tagerrlist) {
                TagErrorProcess(l_tagerrlist);
                DBSA_FreeErrTagList(&l_tagerrlist);
            }

            // - Intgr 6 Bug Fix Lokesh

            CString l_error_str = _T("");
            l_error_str.Format(_T("%s[%s]"),
                               CPqmUtilities::GetMultiLingualString(_T("IDS_FAILED_TO_LOAD_STUDY")),
                               f_studyloid);

            CPQMLogMgr::GetInstance()->DisplayError(
                l_error_str, _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
                ERR_ID, APP_NAME);

            return ;
        }
    }


    //+Patni-SS/ADDED-COMMENTED/2009-May-22/ACqorder Problem
    //m_series_count = m_dbsa_study->DBSA_ObjectCount();
    WORD* l_acqorder = NULL;
    int l_acqsize  = m_dbsa_study->DBSA_GetAcqOrder(&l_acqorder);
    m_series_count = l_acqsize;
    //-Patni-SS/ADDED-COMMENTED/2009-May-22/ACqorder Problem

    // put l_acqorder in member variable
    memcpy(m_wacqorder, l_acqorder, sizeof(WORD) * m_series_count);
    //while( l_indx < l_acqsize ){
    //    m_wacqorder[l_indx] = l_acqorder[l_indx];
    //    l_indx++;
    //}
    //Patni-PJS/Added/TMSC-REDMINE-718
    //+Patni-DKH/Modified/20010-Oct-05/TMSC-REDMINE-756-Part1
    m_create_series_cnt = l_acqsize ;
    //m_create_series_cnt = m_dbsa_study->DBSA_GetAcqOrder(&l_acqorder);
    //-Patni-DKH/Modified/20010-Oct-05/TMSC-REDMINE-756-Part1

    //+Patni-DKH/Added/20010-Oct-05/TMSC-REDMINE-756-Part1
    if (NULL != l_acqorder) {
        DBSA_FreeLocalAcqOrder(l_acqorder);
    }

    //-Patni-DKH/Added/20010-Oct-05/TMSC-REDMINE-756-Part1

    //Patni-PJS/2011Jan25/Modified/IR-97
    bool l_gl_sr_sqsaved = false;

    //+Patni-ARD/2009Sept10/Added/MVC defect Fix #3909
    // + PSP1 Code Optimization AMIT + HIMANSHU 20090218
    if (f_snd_seq_List->GetCount() >= MAX_SEQUENCE) {
        /* When total count of protocols is more than 10
         * series information will be vanished once
         */
        m_new_series_loids->RemoveAll();
        CreateSeries(f_snd_seq_List, MAX_SEQUENCE);
        //Patni-PJS/2011Jan25/Modified/IR-97
        m_wacqorder[m_create_series_cnt] = NULL;

        //+Patni-PJS/2011Jan25/Modified/IR-97
        l_gl_sr_sqsaved = true;
        m_pqm_ptr_dbsa->WriteToUI(PQM_MSG_GL_SR_SQSAVED);
        //-Patni-PJS/2011Jan25/Modified/IR-97
        m_pqm_ptr_dbsa->WriteToUI(PQM_MSG_SEQ_RDY_DISPLAY);

    } else {
        /* When total count of protocols is less than 10
         * series information will be vanished to avoid the previously added info
         */
        m_new_series_loids->RemoveAll();
    }

    //-Patni-ARD/2009Sept10/Added/MVC defect Fix #3909
    //Patni/SS/2010July06/Modified/NFD001-AutoMap
    if (CreateSeries(f_snd_seq_List, -1, f_mode, f_targetpos)) {
        if (MAP_AUTO_MODE != f_mode) {
            m_pqm_ptr_dbsa->WriteToUI(PQM_MSG_SEQ_RDY);
        }
    }

    m_pqm_ptr_dbsa->WriteToUI(PQM_MSG_SEQUENCE_TRANSFER_COMPLETED);

    // - PSP1 Code Optimization AMIT + HIMANSHU 20090218

    //+ Patni-PJS/2009Jun05/Added/JFT# 251,251,251,IR(5)

    //PAtni-SS/SIFTED/2009June07/PQM hang problem
    //m_pqm_ptr_dbsa->WriteToUI(PQM_MSG_GL_SR_SQSAVED);

    // To print the Acqorder
    CString l_buf(_T(""));
    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    //TCHAR l_tmpbuff[MAX_BUFFER];
    TCHAR l_tmpbuff[MAX_BUFFER] = {0};
    int l_loc = 0;

    //+Patni-PJS/Added/TMSC-REDMINE-718
    //while (l_loc < m_series_count) {
    while (l_loc < m_create_series_cnt) {
        //-Patni-PJS/Added/TMSC-REDMINE-718

        _ltow(m_wacqorder[l_loc], l_tmpbuff, NUM_10);
        l_buf += l_tmpbuff;
        l_buf += L", ";
        l_loc++;
    }

    l_buf += L" AcqSize :";
    //+Patni-PJS/Added/TMSC-REDMINE-718
    //_ltow(m_series_count, l_tmpbuff, NUM_10);
    _ltow(m_create_series_cnt, l_tmpbuff, NUM_10);
    //-Patni-PJS/Added/TMSC-REDMINE-718
    l_buf += l_tmpbuff;

    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, l_buf);

    //+Patni-PJS/Added/TMSC-REDMINE-718
    //m_wacqorder[m_series_count] = NULL;
    //m_dbsa_study->DBSA_AcqOrderUpdate(m_wacqorder, m_series_count);

    //+Patni-PJS/2011Jan25/Modified/IR-97
    m_wacqorder[m_create_series_cnt] = NULL;
    m_dbsa_study->DBSA_AcqOrderUpdate(m_wacqorder, m_create_series_cnt);
    //-Patni-PJS/Added/TMSC-REDMINE-718
    l_return_status = m_dbsa_study->DBSA_Put(&l_tagerrlist);
    //-Patni-PJS/2011Jan25Modified/IR-97

    if (l_return_status != MRC_DBSA_SUCCESS) {

        if (l_tagerrlist) {
            TagErrorProcess(l_tagerrlist);
            DBSA_FreeErrTagList(&l_tagerrlist);
        }

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_UNABLE_TO_WRITE_DATA_IN_DB")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
    }

    //PAtni-SS/SIFTED/2009June07/PQM hang problem
    //+Patni-PJS/2011Jan25/Modified/IR-97
    if (!l_gl_sr_sqsaved) {
        m_pqm_ptr_dbsa->WriteToUI(PQM_MSG_GL_SR_SQSAVED);
    }

    //-Patni-PJS/2011Jan25Modified/IR-97

    //+ Patni-RJ/2009Apr02/Added/PSP- Performance code
#ifdef __SM_PERFORMANCE_CHECK__
    QueryPerformanceFrequency((LARGE_INTEGER*)&l_freq);
    QueryPerformanceCounter((LARGE_INTEGER*)&l_stop);

    temp_string.Format(_T("{MakeSeries} For writing %d seqs in MPlus database"),
                       l_total_series_count);
    CPQMLogMgr::GetInstance()->TracePerformance(l_start, l_stop, l_freq, temp_string);
    CPQMLogMgr::GetInstance()->PrintTime(_T("{PQM_Addition_End}"));
#endif
    //- Patni-RJ/2009Apr02/Added/PSP- Performance code

    DEL_PTR(l_seriesnode);

    if (!l_errstr.IsEmpty()) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_errstr);

        //Patni-ARD/2009Jun25/Modified/ACE# policy of ErrorView
        CPQMLogMgr::GetInstance()->DisplayScanError(IDS_MAX_PROTOCOL,
                _T("IDS_MAX_PROTOCOL"),
                SCAN_ERR_NOTIFY, L"PQMStudyMgr.cpp");
    }
}

/****************************Method Header************************************
//Method Name   :DeleteAll()
//Author        :PATNI/PJP
//Purpose       :This method deletes all the information of series
//*****************************************************************************/
BOOL CPQMStudyMgr::DeleteAll(
    BSTR f_bstr
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudyMgr::DeleteAll");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, f_bstr);

    if (m_dbsa_study == NULL) {
        return FALSE;
    }

    WORD* l_acqorder = NULL;
    int l_acqordersize = m_dbsa_study->DBSA_GetAcqOrder(&l_acqorder);

    CString l_seqid = f_bstr;
    LPCTSTR* l_seloidlist = new LPCTSTR[1];
    l_seloidlist[0] = l_seqid.AllocSysString();
    CDBSASeries* l_delSeries = NULL;
    CDBSATagErrorList* l_tagerrlist = NULL;
    int l_result = m_dbsa_study->DBSA_findSeries(&l_delSeries, &l_tagerrlist, PQM, l_seloidlist[0]);

    if (l_result != MRC_DBSA_SUCCESS || NULL == l_delSeries) {
        if (l_tagerrlist) {
            TagErrorProcess(l_tagerrlist);
            DBSA_FreeErrTagList(&l_tagerrlist);
        }

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("[DBSA_findSeries] failed.."));

        if (l_delSeries) {
            l_delSeries->DBSA_FreeLowObject();
            l_delSeries = NULL;
        }

        return FALSE;
    }

    WORD l_deleteindex = l_delSeries->DBSA_GetIndex();

    // + SM4 Himanshu PQM_CRASH Fix 20090127
    // - SM4 Himanshu PQM_CRASH Fix 20090127
    for (int i = 0, j = i;  i < l_acqordersize; j++, i++) {
        if (l_acqorder[i] == l_deleteindex) {
            j = i + 1;
        }

        //l_acqorder[i] = l_acqorder[j];
        if (i < (l_acqordersize - 1))
            m_wacqorder[i] = l_acqorder[j];
    }

    l_acqordersize--;

    //+Patni-DKH/Added/20010-Oct-05/TMSC-REDMINE-756-Part1
    if (NULL != l_acqorder) {
        DBSA_FreeLocalAcqOrder(l_acqorder);
    }

    //-Patni-DKH/Added/20010-Oct-05/TMSC-REDMINE-756-Part1
    int l_delete_count = 1;
    l_result = m_dbsa_study->DBSA_DeleteLowObject(&l_delete_count, l_seloidlist);

    if (l_result == MRC_DBSA_SUCCESS) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("DeleteAll Success"));

    } else {
        if (l_tagerrlist) {
            TagErrorProcess(l_tagerrlist);
            DBSA_FreeErrTagList(&l_tagerrlist);
        }

        //+Patni-HAR/2010Mar10/Modified/MSA0251-00278

        CPQMLogMgr::GetInstance()->WriteEventLog(
            CPqmUtilities::GetMultiLingualString(_T("IDS_DELETE_SERIES_ERROR")),
            ERROR_LOG_CRUCIAL1, _T("PQMStudyMgr.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82

        //-Patni-HAR/2010Mar10/Modified/MSA0251-00278

        if (l_delSeries) {
            l_delSeries->DBSA_FreeLowObject();
            l_delSeries = NULL;
        }

        return FALSE;
    }


    //+Patni-SS/MODI-ADDED/2009-May-22/AcqOrder problem
    m_series_count =  l_acqordersize;
    //-Patni-SS/MODI-ADDED/2009-May-22/AcqOrder problem

    //+ Patni-RJ/2009Apr02/Added/PSP- Performance code
#ifdef __SM_PERFORMANCE_CHECK__
    __int64 l_freq, l_stop, l_start;

    QueryPerformanceCounter((LARGE_INTEGER*)&l_start);
#endif
    //- Patni-RJ/2009Apr02/Added/PSP- Performance code

    m_dbsa_study->DBSA_AcqOrderUpdate(m_wacqorder, l_acqordersize);
    l_result = m_dbsa_study->DBSA_Put(&l_tagerrlist);

    if (l_result != MRC_DBSA_SUCCESS) {
        if (l_tagerrlist) {
            TagErrorProcess(l_tagerrlist);
            DBSA_FreeErrTagList(&l_tagerrlist);
        }

        //+Patni-HAR/2010Mar10/Modified/MSA0251-00278
        CPQMLogMgr::GetInstance()->WriteEventLog(
            CPqmUtilities::GetMultiLingualString(_T("IDS_DELETE_SERIES_ERROR")),
            ERROR_LOG_CRUCIAL1, _T("PQMStudyMgr.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82
        //-Patni-HAR/2010Mar10/Modified/MSA0251-00278
    }

    //+ Patni-RJ/2009Apr02/Added/PSP- Performance code
    CString l_temp(_T(""));
#ifdef __SM_PERFORMANCE_CHECK__
    QueryPerformanceFrequency((LARGE_INTEGER*)&l_freq);
    QueryPerformanceCounter((LARGE_INTEGER*)&l_stop);
    l_temp.Format(_T("{CPQMStudyMgr::DeleteAll} Time take by MPlus to delete protocol %d."), l_deleteindex);
    CPQMLogMgr::GetInstance()->TracePerformance(l_start, l_stop, l_freq, l_temp);
#endif
    //- Patni-RJ/2009Apr02/Added/PSP- Performance code

    l_temp.Format(_T("%d"), m_series_count);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_temp);

    return TRUE;
}

/****************************Method Header************************************
//Method Name   :WriteStudyNameatStudyLevel()
//Author        :PATNI/JS
//Purpose       :
//*****************************************************************************/
BOOL CPQMStudyMgr::WriteStudyNameatStudyLevel(
    const CString& f_study_id
)
{


    CDBSAVR* l_vrparam = m_dbsa_study->GetValue(PQM_DCM_MR_STUDY_NODE_PATH);

    if (l_vrparam == NULL) {
        l_vrparam = new CDBSAVRLO(PQM_DCM_MR_STUDY_NODE_PATH);
        m_dbsa_study->addLeaf(l_vrparam);
    }

    BOOL l_result = l_vrparam->SetValue(((LPCTSTR*)(&f_study_id)), 1);

    CDBSATagErrorList* l_tagerrlist = NULL;

    if (l_result != MRC_DBSA_SUCCESS) {
        if (l_tagerrlist) {
            TagErrorProcess(l_tagerrlist);
            DBSA_FreeErrTagList(&l_tagerrlist);
        }

        return l_result;
    }


    l_result =  m_dbsa_study->DBSA_Put(&l_tagerrlist);

    if (l_result != MRC_DBSA_SUCCESS) {

        LPTSTR l_study_name_buff = NULL;
        l_vrparam->GetString(&l_study_name_buff, 0);

        return l_result;
    } //Log error

    return l_result;
}

/****************************Method Header************************************
//Method Name   :WriteStudyNameatSeriesLevel()
//Author        :PATNI/JS
//Purpose       :
//*****************************************************************************/
BOOL CPQMStudyMgr::WriteStudyNameatSeriesLevel(
    const CString& f_study_id
)
{
    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_dbsa_study == NULL) {
        return FALSE;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections
    CDBSATagErrorList* l_tagerrlist = NULL;
    DBSAFactory l_factory;
    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    /*
    LPCTSTR l_series_id ;
    DBSAIndex_t l_sindex;
    */
    LPCTSTR l_series_id = 0;
    DBSAIndex_t l_sindex = 0u;
    //- Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    BOOL l_result = TRUE;
    int l_error_code = 0;
    CDBSASeries* l_series = NULL;
    //+Patni-SS/ADDED-COMMENTED/2009-May-22/ACqorder Problem
    WORD* l_acqorder = NULL;
    int l_seriescnt = m_dbsa_study->DBSA_GetAcqOrder(&l_acqorder);
    //int l_seriescnt = m_dbsa_study->DBSA_ObjectCount();
    //-Patni-SS/ADDED-COMMENTED/2009-May-22/ACqorder Problem

    //+Patni-DKH/Added/20010-Oct-05/TMSC-REDMINE-756-Part1
    if (NULL != l_acqorder) {
        DBSA_FreeLocalAcqOrder(l_acqorder);
    }

    //-Patni-DKH/Added/20010-Oct-05/TMSC-REDMINE-756-Part1

    for (l_result = m_dbsa_study->DBSA_GetFirstSeries(&l_series, &l_tagerrlist, PQM);
         l_result != MRC_DBSA_OBJECT_FIND_END && l_seriescnt > 0;
         l_result = m_dbsa_study->DBSA_GetNextSeries(&l_series,
                    &l_tagerrlist, PQM, l_sindex)
        ) {
        CDBSAVRNode* l_input_tag = new CDBSAVRNode();
        CDBSAVR* l_vrparam = new CDBSAVRUI();
        l_seriescnt--;
        l_sindex = l_series->DBSA_GetIndex();
        l_series_id = l_series->DBSA_GetLOID();
        l_input_tag->addLeaf(l_vrparam);
        l_error_code = l_vrparam->SetValue(((LPCTSTR*)(&f_study_id)), 1);

        if (l_error_code != MRC_DBSA_SUCCESS) {
            if (l_tagerrlist) {
                TagErrorProcess(l_tagerrlist);
                DBSA_FreeErrTagList(&l_tagerrlist);
            }

            return l_result;
        }

        l_error_code = l_factory.DBSA_WriteSeries(&l_tagerrlist, l_series_id,
                       l_input_tag);

        if (l_error_code != MRC_DBSA_SUCCESS) {
            if (l_tagerrlist) {
                TagErrorProcess(l_tagerrlist);
                DBSA_FreeErrTagList(&l_tagerrlist);
            }

            return l_result;
        }

        //			l_result = l_series->DBSA_Put(&l_tagerrlist);
        if (l_result != MRC_DBSA_SUCCESS) {
            if (l_tagerrlist) {
                TagErrorProcess(l_tagerrlist);
                DBSA_FreeErrTagList(&l_tagerrlist);
            }

            CPQMLogMgr::GetInstance()->DisplayError(
                CPqmUtilities::GetMultiLingualString(_T("IDS_FAILED_TO_WRITE_DATA_IN_DB")),
                _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
                ERR_ID, APP_NAME);
            return l_result;
        }
    }

    //    if (l_input_tag) {
    //        delete l_input_tag;
    //        l_input_tag = NULL;
    //    }

    if (l_series_id) {
        l_series_id = NULL;
    }

    return l_result;
}

/****************************Method Header************************************
//Method Name   :DBSA_Init()
//Author        :PATNI/
//Purpose       :To Init DBSALib
//*****************************************************************************/
void CPQMStudyMgr::DBSA_Init(
)
{
    DBSA_DBSInit(); // Init DBSALib
    DBSA_LogStart(MRC_DBSA_LOGOUT_DBSALIB); // To temp DBSALog
}

/****************************Method Header************************************
//Method Name   :AddAcqConAndHomePosAtStudyLevel()
//Author        :PATNI / HAR
//Purpose       :
//*****************************************************************************/
//Patni-ARD/2009Dec7/Modified/DeFT# MVC006086 + self review
BOOL CPQMStudyMgr::AddAcqConAndHomePosAtStudyLevel(
    float f_home_position,
    int f_acq_condition
)
{
    CDBSATagErrorList* l_tagerrlist = NULL;
    BOOL l_result = TRUE;

    if (m_dbsa_study == NULL) {
        DBSAFactory l_factory;
        l_result = l_factory.DBSA_LoadStudy(&m_dbsa_study, &l_tagerrlist, PQM, m_stloid, NULL);

        if (l_result != MRC_DBSA_SUCCESS) {

            if (l_tagerrlist) {
                TagErrorProcess(l_tagerrlist);
                DBSA_FreeErrTagList(&l_tagerrlist);
            }
        }
    }


    CDBSAVR* l_vrparam = m_dbsa_study->GetValue(PQM_DCM_MR_STUDY_HOME_POSITION);

    if (l_vrparam == NULL) {
        l_vrparam = new CDBSAVRDS(PQM_DCM_MR_STUDY_HOME_POSITION);
        m_dbsa_study->addLeaf(l_vrparam);
    }

    //+Patni-ARD/2009Dec7/Modified/DeFT# MVC006086 + self review
    CString l_str_home_position(_T(""));
    l_str_home_position.Format(_T("%.8f"), f_home_position);

    l_result = l_vrparam->SetValue(((LPCTSTR*)(&l_str_home_position)), 1);
    //-Patni-ARD/2009Dec7/Modified/DeFT# MVC006086 + self review

    if (l_result != MRC_DBSA_SUCCESS) {
        if (l_tagerrlist) {
            TagErrorProcess(l_tagerrlist);
            DBSA_FreeErrTagList(&l_tagerrlist);
        }

        return l_result;
    }


    l_result =  m_dbsa_study->DBSA_Put(&l_tagerrlist);

    if (l_result != MRC_DBSA_SUCCESS) {

        LPTSTR l_study_name_buff = NULL;
        l_vrparam->GetString(&l_study_name_buff, 0);

        return l_result;
    } //Log error

    l_vrparam = m_dbsa_study->GetValue(PQM_DCM_MR_STUDY_ACQ_CONDITION);

    if (l_vrparam == NULL) {
        l_vrparam = new CDBSAVRUL(PQM_DCM_MR_STUDY_ACQ_CONDITION);
        m_dbsa_study->addLeaf(l_vrparam);
    }

    //+Patni-ARD/2009Dec7/Modified/DeFT# MVC006086 + self review
    //unsigned long l_acq_con = -1;
    /*
    int l_size = f_acq_condition.GetLength() + 1;
    char * l_temp = new char[l_size];
    memset(l_temp, 0, sizeof(char) * l_size);	// MEM_INI AMIT 20081230
    wcstombs(l_temp, f_acq_condition, l_size);

    unsigned long l_acq_con = atoi(l_temp);
    */
    unsigned long l_acq_con = f_acq_condition;
    l_result = l_vrparam->SetValue(&l_acq_con, 1);
    //-Patni-ARD/2009Dec7/Modified/DeFT# MVC006086 + self review

    if (l_result != MRC_DBSA_SUCCESS) {
        if (l_tagerrlist) {
            TagErrorProcess(l_tagerrlist);
            DBSA_FreeErrTagList(&l_tagerrlist);
        }

        return l_result;
    }

    l_result =  m_dbsa_study->DBSA_Put(&l_tagerrlist);

    if (l_result != MRC_DBSA_SUCCESS) {

        LPTSTR l_study_name_buff = NULL;
        l_vrparam->GetString(&l_study_name_buff, 0);

        return l_result;
    } //Log error

    //delete [] l_temp;
    //l_temp = NULL;

    return l_result;
}

/****************************Method Header************************************
//Method Name   : AddAcqConAndHomePosAtSeriesLevel()
//Author        : PATNI / HAR
//Purpose       :
//*****************************************************************************/
//+Patni-ARD/2009Dec7/Modified/DeFT# MVC006086 + self review
BOOL CPQMStudyMgr::AddAcqConAndHomePosAtSeriesLevel(
    const CString& f_series_loid,
    const float f_home_position,
    const int f_acq_condition
)
{

    CDBSAVRNode* l_input_tag = new CDBSAVRNode();


    CDBSAVR* l_vrparam = new CDBSAVRDS(PQM_DCM_MR_SERIES_HOME_POSITION);
    l_input_tag->addLeaf(l_vrparam);

    CString l_str_home_position(_T(""));
    l_str_home_position.Format(_T("%.8f"), f_home_position);


    BOOL l_result = l_vrparam->SetValue(((LPCTSTR*)(&l_str_home_position)), 1);

    l_vrparam = new CDBSAVRUL(PQM_DCM_MR_SERIES_ACQ_CONDITION);
    l_input_tag->addLeaf(l_vrparam);

    unsigned long l_acq_con = f_acq_condition;
    l_result = l_vrparam->SetValue(&l_acq_con, 1);
    //-Patni-ARD/2009Dec7/Modified/DeFT# MVC006086 + self review

    CDBSATagErrorList* l_tagerrlist = NULL;
    DBSAFactory l_factory;
    l_result = l_factory.DBSA_WriteSeries(&l_tagerrlist, ((LPCTSTR)f_series_loid), l_input_tag);

    if (l_result != MRC_DBSA_SUCCESS) {
        if (l_tagerrlist) {
            TagErrorProcess(l_tagerrlist);
            DBSA_FreeErrTagList(&l_tagerrlist);
        }

        return l_result;
    }

    DEL_PTR(l_vrparam);

    return TRUE;
}

//+Patni-Hemant+ARD/2009Oct30/Commented/IR 85 Auto Transfer
/****************************Method Header************************************
//Method Name   :UpdateAutoTransferValueInMPlusDB()
//Author        :PATNI/Hemant+ARD
//Purpose       :Updates the auto transfer flag in Mplus DB for given protocol
//               Returns true on success or returns false.
//*****************************************************************************/
BOOL CPQMStudyMgr::UpdateAutoTransferValueInMPlusDB(
    CPqmProtocol* f_pqm_protocol
)
{

    if (f_pqm_protocol) {
        CString 		l_series_loid = f_pqm_protocol->GetSeriesLOID();
        unsigned short l_auto_transfervalue = f_pqm_protocol->GetAutoTransfer();

        int l_error_code = 0;

        CDBSAVRNode*	 l_input_tag = new CDBSAVRNode();
        CDBSAVR*		 l_vrparam = new CDBSAVRUS(PQM_SVN_PLN_AUTO_POSTPROC_FLG);

        if (l_input_tag && l_vrparam) {
            l_input_tag->addLeaf(l_vrparam);
            l_error_code = l_vrparam->SetValue(&l_auto_transfervalue, 1);

            CDBSATagErrorList* l_tagerrlist = NULL;

            if (l_error_code != MRC_DBSA_SUCCESS) {
                if (l_tagerrlist) {
                    TagErrorProcess(l_tagerrlist);
                    DBSA_FreeErrTagList(&l_tagerrlist);
                }

                ASSERT(FALSE);
                return FALSE;
            }

            DBSAFactory 	l_factory;
            l_error_code = l_factory.DBSA_WriteSeries(&l_tagerrlist, l_series_loid, l_input_tag);

            if (l_error_code != MRC_DBSA_SUCCESS) {
                if (l_tagerrlist) {
                    TagErrorProcess(l_tagerrlist);
                    DBSA_FreeErrTagList(&l_tagerrlist);
                }

                ASSERT(FALSE);
                return FALSE;
            }


            DEL_PTR(l_input_tag);

            return TRUE;
        }
    }

    return FALSE;
}
//-Patni-Hemant+ARD/2009Oct30/Commented/IR 85 Auto Transfer


/****************************Method Header************************************
//Method Name   :UpdateAnatomyValueInMPlusDB()
//Author        :PATNI/Sribanta
//Purpose       :Updates anatomy in Mplus DB.Returns true on success or returns false.
//*****************************************************************************/
BOOL CPQMStudyMgr::UpdateAnatomyValueInMPlusDB(
    CPqmProtocol* f_pqm_protocol
) const
{

    CString l_str = _T("");
    GetDicomStringforAnatomy(f_pqm_protocol, l_str);

    if (l_str.IsEmpty()) {
        return FALSE;
    }

    LPCTSTR* l_body_part = (LPCTSTR*) &l_str;

    CString 		l_series_loid = f_pqm_protocol->GetSeriesLOID();

    CDBSAVRNode* l_input_tag = new CDBSAVRNode();
    CDBSAVR* l_vr_station_name = new CDBSAVRCS(DCM_ST_BODY_PART_EXAMINED);
    l_input_tag->addLeaf(l_vr_station_name);


    int l_error_code = l_vr_station_name->SetValue(l_body_part, 1);

    CDBSATagErrorList* l_tagerrlist = NULL;
    DBSAFactory l_factory;

    int l_result = l_factory.DBSA_WriteSeries(&l_tagerrlist, l_series_loid, l_input_tag);

    if (l_result != MRC_DBSA_SUCCESS) {
        if (l_tagerrlist) {
            TagErrorProcess(l_tagerrlist);
            DBSA_FreeErrTagList(&l_tagerrlist);
        }

        l_result =  FALSE;
    }

    DEL_PTR(l_input_tag);
    return l_result;

}


/****************************Method Header************************************
//Method Name   :AddSeriesReference()
//Author        :PATNI/SS
//Purpose       :To set the series reference to each other in Varfield and MPlus
// Modified		:
//*****************************************************************************/
bool CPQMStudyMgr::AddSeriesReference(
    CPqmProtocol* f_protocol,
    LPCTSTR f_series_loid
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudyMgr::AddSeriesReference");

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_pqm_ptr_dbsa == NULL) {
        return E_ERROR;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    if (f_protocol == NULL /*|| !CheckSeriesCntInDB()*/)
        return E_ERROR;

    CPQMStudy* l_varfield_study = m_pqm_ptr_dbsa->GetStudy();
    CString l_fullpath = l_varfield_study->GetStudyPath();
    f_protocol->SetSeriesLOID(f_series_loid);

    int l_acq_order = f_protocol->GetProtocol();
    //Meghana for B comments no 2
    //l_fullpath += _T(":1.");
    l_fullpath += _T(":/1.");

    CString acq_no(_T(""));
    acq_no.Format(_T("%d"), l_acq_order);
    l_fullpath += acq_no;

    CDBSAVRNode* l_input_tag = new CDBSAVRNode();
    CDBSAVR* l_vrparam = new CDBSAVRLO(PQM_DCM_MR_SERIES_NODE_PATH);
    //Meghana for defect fixing..
    //+Patni-RSG/ADDED-2009May14/JFT- Mapped Path Requirement # 17
    CDBSAVR* l_vfpathleaf = new CDBSAVRLO(PQM_DCM_MR_PROTOCOL_NODE_PATH);
    l_input_tag->addLeaf(l_vrparam);
    l_input_tag->addLeaf(l_vfpathleaf);
    //-Patni-RSG/ADDED-2009May14/JFT- Mapped Path Requirement # 17

    LPCTSTR my_str = l_fullpath;
    int l_result = l_vrparam->SetValue(&my_str, 1);

    CDBSATagErrorList* l_tagerrlist = NULL;

    if (l_result != MRC_DBSA_SUCCESS) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, DBSA_GetErrorString(l_result));

        if (l_tagerrlist) {
            TagErrorProcess(l_tagerrlist);
            DBSA_FreeErrTagList(&l_tagerrlist);
        }

        DEL_PTR(l_input_tag);

        return E_ERROR;
    }

    //+Patni-RSG/ADDED-2009May14/JFT- Mapped Path Requirement # 17
    l_result = l_vfpathleaf->SetValue(&my_str, 1);

    if (l_result != MRC_DBSA_SUCCESS) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, DBSA_GetErrorString(l_result));

        if (l_tagerrlist) {
            TagErrorProcess(l_tagerrlist);
            DBSA_FreeErrTagList(&l_tagerrlist);
        }

        DEL_PTR(l_input_tag);

        return E_ERROR;
    }

    //+Patni-RSG/ADDED-2009May14/JFT- Mapped Path Requirement # 17

    DBSAFactory l_factory;
    l_result = l_factory.DBSA_WriteSeries(&l_tagerrlist, f_series_loid, l_input_tag);

    if (l_result != MRC_DBSA_SUCCESS) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, DBSA_GetErrorString(l_result));

        if (l_tagerrlist) {
            TagErrorProcess(l_tagerrlist);
            DBSA_FreeErrTagList(&l_tagerrlist);
        }

        DEL_PTR(l_input_tag);

        return E_ERROR;

    }

    DEL_PTR(l_input_tag);

    l_varfield_study->SetSeriesLOIDReference(l_acq_order, f_series_loid);

    return E_NO_ERROR;
}

/****************************Method Header************************************
//Method Name   :CreateSeries()
//Author        :PATNI/JS - LK
//Purpose       :
//*****************************************************************************/
bool CPQMStudyMgr::CreateSeries(
    SequenceList* f_snd_seq_List,
    const int f_firstuisequences,
    const mode_automap f_mode,
    const int f_targetpos
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudyMgr::CreateSeries");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of CreateSeries"));

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_dbsa_study == NULL || m_pqm_ptr_dbsa == NULL || m_new_series_loids == NULL) {
        return false;
    }

    POSITION l_pos = f_snd_seq_List->GetHeadPosition();

    if (NULL == l_pos) {
        return false;
    }

    CPQMDBSALib* l_pqm_dbsalib_se = NULL; //VRMake
    CDBSAVRNode* l_seriesnode = NULL;
    CDBSATagErrorList* l_tagerrlist = NULL;
    int l_leafcount_series = 0;
    int l_return_status = 0;
    CDBSASeries* l_series = NULL;
    int	l_seq_index = 0;
    int l_delete_count = 1;
    CString l_log_msg(_T(""));
    LPCTSTR l_loid = NULL;
    int l_delete_status = 0;
    CString temp_string(_T(""));
    int l_del_index = 0;
    float ret_val = 0.0f;
    CString l_str_whole_sar(_T(""));
    CString l_str_partial_sar(_T(""));
    CString l_str_head_sar(_T(""));
    CString l_str_pori(_T(""));
    CString l_str_scanoffset(_T(""));
    CString l_identstr(_T(""));
    CString l_str_b1_rms(_T(""));
    int f_scan_number = 0;
    float f_couch_pos = PQM_COUCH_INIT_POSITION;
    float f_home_pos = PQM_COUCH_INIT_POSITION;
    int l_data = 0;
    CPASCOMSequence* l_pseq = NULL;

    while (l_pos) {
        l_pseq = f_snd_seq_List->GetNext(l_pos);
        m_pqm_ptr_dbsa->SetWaitStatus(l_pseq);
        //-------------------------------------------------------------------------
        //commented by Hemant..
        //m_pqm_ptr_dbsa->ReSetScanAnatomy(l_pseq);
        //m_pqm_ptr_dbsa->ReSetReceiverCoil(l_pseq);
        //-------------------------------------------------------------------------
        l_pqm_dbsalib_se = new CPQMDBSALib(MRC_DBSA_SERIES);
        l_leafcount_series = g_sequence_param_name_list.GetCount();
        SetLeafCountSeries(l_leafcount_series);
        l_pqm_dbsalib_se->SetPascomSequence(l_pseq);
        l_seriesnode = l_pqm_dbsalib_se->MakeTagXML(l_leafcount_series, 1,
                       MRC_TRUE);
        l_delete_count = 1;

        //+Patni-ARD/2009Jun19/Modified/JFT# DBserver access issue
        l_log_msg = _T("");

        //Patni-RSG/2009Aug19/Modified/ACE# MVC3381
        l_loid = NULL;

        for (int i = 0; i < 3; i++) {
            l_return_status = m_dbsa_study->DBSA_CreateSeries(&l_series,
                              &l_tagerrlist, PQM, l_seriesnode);

            if (l_return_status == MRC_DBSA_SUCCESS) {
                break;
            }


            //+Patni-RSG/2009Aug19/Modified/ACE# MVC3381
            if (l_series) {
                l_loid = l_series->DBSA_GetLOID();

                if (l_loid) {
                    //PATNI-SS/MODDIFIED/20090901/MVC003381/review comments
                    l_delete_status = 0;
                    l_delete_status = m_dbsa_study->DBSA_DeleteLowObject(&l_delete_count, &l_loid);

                    if (l_delete_status != MRC_DBSA_SUCCESS) {
                        l_log_msg.Empty();
                        l_log_msg.Format(_T("Unable To delete Series LOID= "));
                        l_log_msg += l_loid;

                        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_msg);
                    }
                }

                l_series->DBSA_FreeLowObject();
                l_series = NULL;
            }

            //+Patni-RSG/2009Aug19/Modified/ACE# MVC3381

            if (l_tagerrlist) {
                TagErrorProcess(l_tagerrlist);
                DBSA_FreeErrTagList(&l_tagerrlist);
            }

            l_log_msg.Empty();
            l_log_msg.Format(_T("DBSA_CreateSeries() - Retry Count = %d"), i + 1);

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_msg);
        }

        if (l_return_status != MRC_DBSA_SUCCESS || NULL == l_series) {


            CPQMLogMgr::GetInstance()->DisplayError(
                CPqmUtilities::GetMultiLingualString(_T("IDS_FAILED_TO_CREATE_SERIES")),
                _T("PQMStudyMgr.cpp"),
                __LINE__, ERROR_LOG_CRUCIAL1,
                ERR_ID, APP_NAME);

            //+Patni-AMT/2009Jun15/Added/JFT# PQM Hang issue
            //+Patni-PJS/Added/TMSC-REDMINE-718
            /*CPqmProtocol* l_del_prot =
                m_pqm_ptr_dbsa->GetStudy()->GetItemFromPosition(m_series_count + 1);*/
            CPqmProtocol* l_del_prot =
                m_pqm_ptr_dbsa->GetStudy()->GetItemFromPosition((f_mode ==
                        MAP_NORMAL_MODE) ? m_create_series_cnt + 1 : f_targetpos);
            //-Patni-PJS/Added/TMSC-REDMINE-718
            l_del_index = l_del_prot->GetProtocol();

            l_log_msg.Empty();
            l_log_msg.Format(_T("Deleting protocol from Varfield = %d "),
                             l_del_index);

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_log_msg);

            m_pqm_ptr_dbsa->GetStudy()->DeleteProt(l_del_index);

            f_snd_seq_List->RemoveAt(f_snd_seq_List->Find(l_pseq));
            //-Patni-AMT/2009Jun15/Added/JFT# PQM Hang issue

            DEL_PTR(l_series);
            DEL_PTR(l_seriesnode);

            //+Patni-AMT/2009Jun15/Added/JFT# PQM Hang issue
            DEL_PTR(l_pqm_dbsalib_se);

            continue;
            //-Patni-AMT/2009Jun15/Added/JFT# PQM Hang issue
            //return;  //Patni-AMT/2009Jun15/commented/JFT# PQM Hang issue
        }

        //+Patni-ARD/2009Jun19/Modified/JFT# DBserver access issue

        //+Patni-ARD/2009June1/Modified/JFT# PQM-SR GL_SR_SQSaved message.
        temp_string = l_series->DBSA_GetLOID();
        l_pseq->SetSequenceID(temp_string);
        //Patni-ARD/2009Jun09/Modified/JFT# PQM SR Communication GL_SR_SQSaved
        m_new_series_loids->Add(temp_string);
        //-Patni-ARD/2009June1/Modified/JFT# PQM-SR GL_SR_SQSaved message.
        //+Patni-PJS/Added/TMSC-REDMINE-718
        //m_wacqorder[m_series_count++] = l_series->DBSA_GetIndex();
        m_wacqorder[m_create_series_cnt++] = l_series->DBSA_GetIndex();
        m_series_count++;
        //++ To update Acqorder in sequence... Sarjeet
        //+Patni/SS/2010July06/Modified/NFD001-AutoMap
        //CPqmProtocol* l_var_series = m_pqm_ptr_dbsa->GetStudy()->GetItemFromPosition(m_series_count);
        CPqmProtocol* l_var_series = m_pqm_ptr_dbsa->GetStudy()->GetItemFromPosition((f_mode == MAP_NORMAL_MODE) ? m_create_series_cnt : f_targetpos);

        if (f_mode == MAP_AUTO_MODE) {

            m_wacqorder[m_create_series_cnt] = NULL;
            m_dbsa_study->DBSA_AcqOrderUpdate(m_wacqorder, m_create_series_cnt);
            l_return_status = m_dbsa_study->DBSA_Put(&l_tagerrlist);
            l_log_msg.Format(_T("%d"), (m_create_series_cnt - 1));
            BSTR bstr = l_log_msg.AllocSysString();
            SetSeriesOrder(bstr, (f_targetpos - 1));
            SysFreeString(bstr);
        }

        //-Patni/SS/2010July06/Modified/NFD001-AutoMap
        l_log_msg.Format(_T("%d"), l_var_series->GetProtocol());

        //+Patni-HAR/2010Feb16/Added/MSA0251-00166
        l_pseq->GetSequenceParam()->SetInt(_T("DYN"), l_var_series->GetDynamic());
        //-Patni-HAR/2010Feb16/Added/MSA0251-00166

        //+ Patni-PJS/2009Jul29/Added/MVC0003961
        ret_val = l_var_series->GetPqmPrMode()->GetCouchPos();
        l_pseq->GetSequenceParam()->SetFloat(_T("COUCH"), ret_val);
        //- Patni-PJS/2009Jul29/Added/MVC0003961
        l_pseq->GetSequenceParam()->SetString(_T("ACQORDER"), l_log_msg);

        //+Patni-PJS/2011Jan25/Modified/IR-97
        float l_anat_val = (float)l_var_series->GetPqmPrMode()->GetAnatomy();
        l_pseq->GetSequenceParam()->SetFloat(_T("SA"), l_anat_val);
        //-Patni-PJS/2011Jan25Modified/IR-97

        //Note::Group id should be set before setting hold value to SequenceParam

        l_pseq->GetSequenceParam()->SetInt(_T("GROUPID"), l_var_series->GetGroupID());
        l_pseq->GetSequenceParam()->SetInt(_T("MASTERFLAG"), l_var_series->GetMasterFlag());
        l_pseq->GetSequenceParam()->SetInt(_T("MSDENABLEFLAG"), l_var_series->GetMultiSeriesDynamicFlag());
        l_pseq->GetSequenceParam()->SetFloat(_T("SCANTIME"), (float)l_var_series->GetScanTime());

        ///////////////////////Added by Sarjeet For seting the HOLD Value//////////
        l_pseq->GetSequenceParam()->SetInt(_T("HOLD"), l_var_series->GetHoldScan());

        //+Tanoue/MVC5849
        UTIL::Mbs2Wcs(&l_log_msg, l_var_series->GetContrastName());
        l_pseq->GetSequenceParam()->SetString(_T("CONTRASTNAME"), l_log_msg);
        //-Tanoue/MVC5849

        l_pseq->GetSequenceParam()->SetInt(_T("AUTOVOICE"), l_var_series->GetExeAutoVoice());
        //Patni-MRP/2009Aug16/Added/MVC002872 Reset Plan info
        l_pseq->GetSequenceParam()->SetInt(_T("PLANSTATUS"), l_var_series->GetPlnStatus());

        //+ Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display
        //+Patni-NP/2009Sep15/Added/MVC004766
        l_pseq->GetSequenceParam()->SetString(_T("DBDT"), m_pqm_ptr_dbsa->GetStudy()->GetDBDTValueFromStudy(l_var_series->GetProtocol()));

        //+ Patni-PJS/2009Jul23/Modify/IDS#442,MVC002699
        //+20091006 Tanoue-san
        if (SARCTRL_LICENCE_ON != m_pqm_ptr_dbsa->GetSARControlLicense()) {
            l_str_whole_sar.Format(_T("%.3f W/kg"), l_var_series->GetSarInfo()->tsar[sarTypeWholeBody].sar);
            l_pseq->GetSequenceParam()->SetString(_T("SAR"), l_str_whole_sar);
            l_str_partial_sar.Format(_T("%.3f W/kg"), l_var_series->GetSarInfo()->tsar[sarTypePartial].sar);
            l_pseq->GetSequenceParam()->SetString(_T("PARTIALBODYSAR"), l_str_partial_sar);
            l_str_head_sar.Format(_T("%.3f W/kg"), l_var_series->GetSarInfo()->tsar[sarTypeHead].sar);

        } else {

            l_str_whole_sar.Format(_T("%.3f W/kg"), l_var_series->GetSarInfo()->sarControl.tsarControl[sarTypeWholeBody].sar_6min);
            l_pseq->GetSequenceParam()->SetString(_T("SAR"), l_str_whole_sar);
            l_str_partial_sar.Format(_T("%.3f W/kg"), l_var_series->GetSarInfo()->sarControl.tsarControl[sarTypePartial].sar_6min);
            l_pseq->GetSequenceParam()->SetString(_T("PARTIALBODYSAR"), l_str_partial_sar);
            l_str_head_sar.Format(_T("%.3f W/kg"), l_var_series->GetSarInfo()->sarControl.tsarControl[sarTypeHead].sar_6min);

        }

        //-20091006 Tanoue-san
        l_pseq->GetSequenceParam()->SetString(_T("HEADSAR"), l_str_head_sar);
        l_str_b1_rms = _T("----------");

        if (l_var_series->GetSarInfo()->B1rms >= PQM_MIN_B1RMS) {
            l_str_b1_rms.Format(_T("%.2f uT"), l_var_series->GetSarInfo()->B1rms);
        }

        l_pseq->GetSequenceParam()->SetString(_T("B1RMS"), l_str_b1_rms);

        //- Patni-PJS/2009Jul23/Modify/IDS#442,MVC002699
        //- Patni-MSN/2009May15/Added/JFT - IR 5 - Scan ID, SAR, dBdt Display
        //+Patni-PJS/2010May20/Added/JaFT/TMSC-REDMINE-285
        l_pseq->GetSequenceParam()->SetString(_T("REUSESHIM"), _T(""));
        l_pseq->GetSequenceParam()->SetString(_T("REUSEMAP"), _T(""));
        //-Patni-PJS/2010May20/Added/JaFT/TMSC-REDMINE-285
        //+Patni-Manish/2009Jul15/Added/MVC003108/Patient Ori.Updated
        l_str_pori.Format(_T("%d,%d"), l_var_series->GetPqmPrMode()->GetPatoriEndIn(), l_var_series->GetPqmPrMode()->GetPatoriSideUp());
        l_pseq->GetSequenceParam()->SetString(_T("PORI"), l_str_pori);
        //-Patni-Manish/2009Jul15/Added/MVC003108/Patient Ori.Updated

        //Ashish changes Defect Fix # 564 Start
        f32vec3_t l_offset = l_var_series->GetOffset();

        l_str_scanoffset.Format(_T("%f,%f,%f"), l_offset.x * 100, l_offset.y * 100, l_offset.z * 100);
        l_pseq->GetSequenceParam()->SetString(_T("SCANOFFSET"), l_str_scanoffset);
        //Ashish changes Defect Fix # 564 End
        //////////////////////Sarjeet/////////////////////////////////////////////

        //Patni-HEMANT/ADDED On 9/11/2009 12:19:48 PM/ defect found during MVC003979
        l_pseq->GetSequenceParam()->SetString(_T("BODYPART"), l_var_series->GetPqmPrMode()->GetBodyPart());

        //+Patni-Hemant/2010Apr27/Added/JaFT/IR-137
        l_pseq->GetSequenceParam()->SetInt(_T("LATERALITY"), (int)l_var_series->GetPqmPrMode()->GetLaterality());
        //-Patni-Hemant/2010Apr27/Added/JaFT/IR-137

        //+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
        l_pseq->GetSequenceParam()->SetInt(_T("AS-COMPASS"), l_var_series->GetASCOMPASSFlag());
        l_pseq->GetSequenceParam()->SetInt(_T("AMB-FLAG"), l_var_series->GetAMBEnableFlag());
        l_pseq->GetSequenceParam()->SetInt(_T("AMB-MODE-FLAG"), l_var_series->GetAMBModeFlag());
        //-Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
        //Patni-HAR/2010Apr26/Added/IR-134
        l_pseq->GetSequenceParam()->SetInt(_T("EDDY"), l_var_series->GetMRSEddyAxis());

        if (f_mode == MAP_AUTO_MODE) {
            l_pseq->GetSequenceParam()->SetInt(_T("SA"), l_var_series->GetPqmPrMode()->GetAnatomy());

        }

        //Patni-RUP/2010Jul27/Added/TMSC-REDMINE-466
        l_pseq->GetSequenceParam()->SetInt(_T("COUCHMETHOD"), l_var_series->GetcouchMethodFlg());

        l_pseq->GetSequenceParam()->SetString(_T("DELAYTIME"), l_var_series->GetDelayTime());


        l_pseq->GetSequenceParam()->SetInt(_T("INSERTMAP"), l_var_series->GetMapRequired());
        l_pseq->GetSequenceParam()->SetInt(_T("INSERTSHIM"), l_var_series->GetRequireShimming());

        if (AddSeriesReference(l_var_series,
                               (LPCTSTR)l_series->DBSA_GetLOID()) != E_NO_ERROR) {

            l_series->DBSA_FreeLowObject();
            l_series = NULL;
            break;
        }

        //+Patni-Sribanta/2010June25/Added/IR-156_2
        CPQMStudy* l_varfield_study = m_pqm_ptr_dbsa->GetStudy();
        l_identstr = l_varfield_study->GetIDENTValueFor(l_var_series->GetProtocol());
        l_pseq->GetSequenceParam()->SetString(_T("IDENT"), l_identstr);
        //-Patni-Sribanta/2010June25/Added/IR-156_2


        // + SM4 Himanshu 10 Dec 2008
        CPqmShimSharedData* l_pqmshimshareddat = CPqmShimSharedData::GetInstance();
        f_scan_number = 0;
        f_couch_pos = PQM_COUCH_INIT_POSITION;
        f_home_pos = PQM_COUCH_INIT_POSITION;
        l_pqmshimshareddat->CopyCouchInfo(&f_scan_number, &f_couch_pos, &f_home_pos);

        //Patni-ARD/2009Dec7/Modified/DeFT# MVC006086 + self review
        AddAcqConAndHomePosAtSeriesLevel(l_var_series->GetSeriesLOID(), f_home_pos, f_scan_number);

        //+Patni-Hemant+ARD/2009Oct30/Commented/IR 85 Auto Transfer
        l_data = 0;
        l_data = l_var_series->GetAutoTransfer();
        l_pseq->GetSequenceParam()->SetInt(_T("APOST"), l_data);

        //Note :UpdateAutoTransferValueInMPlusDB() should be called after AddSeriesReference()
        //mplus db updation should be made after that only.
        if (!UpdateAutoTransferValueInMPlusDB(l_var_series)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("UpdateAutoTransferValueInMPlusDB() : Failed to update auto transfer tag in MPLUSDB"));
        }

        //-Patni-Hemant+ARD/2009Oct30/Commented/IR 85 Auto Transfer

        // - SM4 Himanshu 10 Dec 2008
        // -- Sarjeet

        l_series->DBSA_FreeLowObject();
        l_series = NULL;

        DEL_PTR(l_pqm_dbsalib_se);

        l_seq_index++;

        if ((f_firstuisequences != -1) && (l_seq_index >= f_firstuisequences)) {
            break;
        }
    }

    return true;
}

/****************************Method Header************************************
//Method Name   :UpdateComment()
//Author        :PATNI/JS
//Purpose       :For updating comment value in Local DB
//*****************************************************************************/
void CPQMStudyMgr::UpdateComment(
    BSTR f_seqlid,
    BSTR f_scancomment
)
{
    CString l_strcomment = f_scancomment;

    CString l_scancomment = l_strcomment.Left(l_strcomment.Find(_T(","), 0));
    CString l_scanid = l_strcomment.Right(l_strcomment.GetLength() - (l_strcomment.Find(_T(",")) + 1));

    LPCTSTR l_scan_comment = l_scancomment;
    LPCTSTR l_scan_id = l_scanid;

    CDBSAVRNode* l_input_tag = new CDBSAVRNode();

    CDBSAVR* l_vrparam = new CDBSAVRLO(DCM_ST_SERIES_DESCRIPTION);  //SVN_STD_PROTOCOL_NAME
    l_input_tag->addLeaf(l_vrparam);

    CDBSAVR* l_vrparam1 = new CDBSAVRLO(PQM_SVN_STD_PROTOCOL_NAME);  //SVN_STD_PROTOCOL_NAME
    l_input_tag->addLeaf(l_vrparam1);

    DBSAFactory l_factory;
    CDBSASeries* l_series = NULL;
    CDBSATagErrorList* l_tagerrlist = NULL;
    int l_result = l_factory.DBSA_LoadSeries(&l_series, &l_tagerrlist, PQM, f_seqlid, l_input_tag);

    if (l_result != MRC_DBSA_SUCCESS) {
        if (l_tagerrlist) {
            TagErrorProcess(l_tagerrlist);
            DBSA_FreeErrTagList(&l_tagerrlist);
        }

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_FAIL_TO_LOAD_SERIES")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return ;
    }

    l_result = l_vrparam->SetValue(&l_scan_comment, 1);

    if (l_result != MRC_DBSA_SUCCESS) {
        if (l_tagerrlist) {
            TagErrorProcess(l_tagerrlist);
            DBSA_FreeErrTagList(&l_tagerrlist);
        }
    }

    l_result = l_vrparam1->SetValue(&l_scan_id, 1);

    if (l_result != MRC_DBSA_SUCCESS) {
        if (l_tagerrlist) {
            TagErrorProcess(l_tagerrlist);
            DBSA_FreeErrTagList(&l_tagerrlist);
        }
    }

    if (l_series != NULL) {
        l_result = l_series->DBSA_WriteData(&l_tagerrlist, l_input_tag);

        if (l_result != MRC_DBSA_SUCCESS) {
            if (l_tagerrlist) {
                TagErrorProcess(l_tagerrlist);
                DBSA_FreeErrTagList(&l_tagerrlist);
            }


            CPQMLogMgr::GetInstance()->DisplayError(
                CPqmUtilities::GetMultiLingualString(_T("IDS_FAILED_TO_WRITE_DATA_IN_DB")),
                _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
                ERR_ID, APP_NAME);
            return ;
        }

        if (l_result != MRC_DBSA_SUCCESS) {
            if (l_tagerrlist) {
                TagErrorProcess(l_tagerrlist);
                DBSA_FreeErrTagList(&l_tagerrlist);
            }

            CPQMLogMgr::GetInstance()->DisplayError(
                CPqmUtilities::GetMultiLingualString(_T("IDS_FAILED_TO_WRITE_DATA_IN_DB")),
                _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
                ERR_ID, APP_NAME);
            return ;
        }

        //        l_result = l_series->DBSA_Put(&l_tagerrlist);
        if (l_result != MRC_DBSA_SUCCESS) {
            if (l_tagerrlist) {
                TagErrorProcess(l_tagerrlist);
                DBSA_FreeErrTagList(&l_tagerrlist);
            }

            CPQMLogMgr::GetInstance()->DisplayError(
                CPqmUtilities::GetMultiLingualString(_T("IDS_FAILED_TO_WRITE_DATA_IN_DB")),
                _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
                ERR_ID, APP_NAME);
            return ;
        }

    }

    DEL_PTR(l_input_tag);


    if (l_scan_comment) {
        l_scan_comment = NULL;
    }

    if (l_scan_id) {
        l_scan_id = NULL;
    }
}

/****************************Method Header************************************
//Method Name   :UpdateGUIControlsInDB()
//Author        :PATNI/JS
//Purpose       :For updating controls values in Local DB
//*****************************************************************************/
//+Patni-RSG/Modified/2009Nov25/MSA0248-0033
BOOL CPQMStudyMgr::UpdateGUIControlsInDB(
    VARIANT* f_pdata,
    BSTR f_seriesid,
    int f_scan_index
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudyMgr::UpdateGUIControlsInDB");
    //REDMINE-1779
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("Inside CPQMStudyMgr::UpdateGUIControlsInDB function"));
    CDBSAVRNode* l_input_tag = new CDBSAVRNode();

    CString l_series_id = BSTR(f_seriesid);
    LPCTSTR l_protocol_id = l_series_id;

    int l_error_code = 0;
    long l_hold_value = 0;

    //Patni-ARD/2009Oct30/Modified/IR 85 Auto Transfer
    unsigned short l_autotransfer_value = 0;

    long l_viewdir_value = 0;
    long l_sar_region_value = 0;
    long l_autovoice_value = 0;
    //Patni-PJS/Added/2009Nov11/MSA0248-0033
    LPCTSTR l_scan_id = _T(""), l_scan_cmnt = _T("");
    //+Patni-RSG/Added/2009Nov25/MSA0248-0033
    std::vector<char*> f_vect_series_loid;
    bool l_series_updated = false;
    //+Patni-PJS/2010Aug24/Added/TMSC-REDMINE-599
    LPCTSTR l_cont_str = _T("");
    //-Patni-PJS/2010Aug24/Added/TMSC-REDMINE-599

    //-Patni-RSG/Added/2009Nov25/MSA0248-0033
    CDBSAVR* l_vrparam = NULL;
    CDBSATagErrorList* l_tagerrlist = NULL;

    switch ((e_control_id)f_pdata->wReserved1) { // Control ID

        case AUTO_VOICE:
            l_autovoice_value = long(f_pdata->intVal);
            l_vrparam = new CDBSAVRSL(PQM_SVN_EXE_AUTO_VOICE);
            l_input_tag->addLeaf(l_vrparam);
            l_error_code = l_vrparam->SetValue(&l_autovoice_value, 1);
            break;


        case HOLD_VALUE:
            l_hold_value = long(f_pdata->intVal);
            l_vrparam = new CDBSAVRSL(PQM_SVN_PLN_HOLD);
            l_input_tag->addLeaf(l_vrparam);
            l_error_code = l_vrparam->SetValue(&l_hold_value, 1);

            break;

        case AUTO_TRANSFER:
            //+Patni-Hemant/2009Oct30/Modified/IR 85 Auto Transfer
            l_autotransfer_value = (unsigned short)(f_pdata->intVal);
            l_vrparam = new CDBSAVRUS(PQM_SVN_PLN_AUTO_POSTPROC_FLG);
            //-Patni-Hemant/2009Oct30/Modified/IR 85 Auto Transfer

            l_input_tag->addLeaf(l_vrparam);
            l_error_code = l_vrparam->SetValue(&l_autotransfer_value, 1);
            break;

        case CONTRAST_AGENT:
            l_vrparam = new CDBSAVRLO(DCM_ST_CONTRAST_BOLUS_AGENT);
            l_input_tag->addLeaf(l_vrparam);
            //+Patni-PJS/2010Aug24/Added+Modified/TMSC-REDMINE-599
            l_cont_str = (LPCTSTR)f_pdata->byref;
            //l_error_code = l_vrparam->SetValue((LPCTSTR*)f_pdata->byref, 1);
            l_error_code = l_vrparam->SetValue((&l_cont_str), 1);

            if (MRC_DBSA_SUCCESS != l_error_code) {
                DEL_PTR(l_input_tag);

                return FALSE;
            }

            if (false == UpdateAllImageLoidUnderProtocol(f_scan_index, f_seriesid, l_input_tag)) {
                DEL_PTR(l_input_tag);

                return FALSE;
            }

            l_series_updated = true;
            //-Patni-PJS/2010Aug24/Added+Modified/TMSC-REDMINE-599
            break;

        case SAR_REG:
            l_sar_region_value = long(f_pdata->intVal);
            l_vrparam = new CDBSAVRSL(PQM_SVN_PLN_SCAN_ANATOMY);
            l_input_tag->addLeaf(l_vrparam);
            l_error_code = l_vrparam->SetValue(&l_sar_region_value, 1);
            break;

        case PATIENT_ORIENTATION:
            l_vrparam = new CDBSAVRCS(DCM_ST_PATIENT_POSITION);
            l_input_tag->addLeaf(l_vrparam);
            l_error_code = l_vrparam->SetValue((LPCTSTR*)(CString*)f_pdata->byref, 1);
            break;


        case VIEW_DIRECTION:
            l_viewdir_value = long(f_pdata->intVal);
            l_vrparam = new CDBSAVRSL(PQM_SVN_PLN_VIEWING_DIR);
            l_input_tag->addLeaf(l_vrparam);
            l_error_code = l_vrparam->SetValue(&l_viewdir_value, 1);
            break;

            //+Patni-PJS/Added/2009Nov11/MSA0248-0033
        case SCAN_ID:
            l_scan_id = (LPCTSTR)f_pdata->byref;
            l_vrparam = new CDBSAVRLO(PQM_SVN_STD_PROTOCOL_NAME);  //SVN_STD_PROTOCOL_NAME
            l_input_tag->addLeaf(l_vrparam);
            l_error_code = l_vrparam->SetValue(&l_scan_id, 1);

            //+Patni-RSG/Added/2009Dec02/MSA0248-0033/MVC005786
            if (f_scan_index >= 0) {
                if (l_error_code != MRC_DBSA_SUCCESS) {
                    if (l_tagerrlist) {
                        TagErrorProcess(l_tagerrlist);
                        DBSA_FreeErrTagList(&l_tagerrlist);
                    }

                    DEL_PTR(l_input_tag);
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                              _T("failed to set PQM_SVN_STD_PROTOCOL_NAME value"));
                    return FALSE;
                }

                CDBSAVR* l_vrparam1 = new CDBSAVRSH(PQM_SVN_STD_SHORT_SERIES_INFO);
                int temp_tag_len = 16;
                TCHAR* temp_tag_data = new TCHAR[temp_tag_len + 1];
                memset(temp_tag_data, NULL, ((sizeof(TCHAR) * (temp_tag_len)) + 1));
                memcpy(temp_tag_data, l_scan_id, (sizeof(TCHAR) * (temp_tag_len)));
                LPCTSTR l_series_info = (LPCTSTR)temp_tag_data;
                l_error_code = l_vrparam1->SetValue(&l_series_info, 1);
                l_input_tag->addLeaf(l_vrparam1);

                if (l_error_code != MRC_DBSA_SUCCESS) {
                    if (l_tagerrlist) {
                        TagErrorProcess(l_tagerrlist);
                        DBSA_FreeErrTagList(&l_tagerrlist);
                    }

                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                              _T("failed to set PQM_SVN_STD_SHORT_SERIES_INFO value"));
                    DEL_PTR(l_input_tag);
                    return FALSE;
                }

                //+Patni-RSG/Added/2009Dec02/MSA0248-0033/MVC005786

                UpdateAllSeriesUnderProtocol(&f_vect_series_loid, f_scan_index, l_input_tag);
                l_series_updated = true;
            }

            //-Patni-RSG/Added/2009Nov25/MSA0248-0033
            break;

        case SCAN_COMMENT:
            l_scan_cmnt = (LPCTSTR)f_pdata->byref;
            l_vrparam = new CDBSAVRLO(DCM_ST_SERIES_DESCRIPTION);
            l_input_tag->addLeaf(l_vrparam);
            l_error_code = l_vrparam->SetValue(&l_scan_cmnt, 1);

            //+Review comment REDMINE-1779
            if (l_error_code != MRC_DBSA_SUCCESS) {
                if (l_tagerrlist) {
                    TagErrorProcess(l_tagerrlist);
                    DBSA_FreeErrTagList(&l_tagerrlist);
                }

                //REDMINE-1779
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("failed to set PQM_SVN_STD_SHORT_SERIES_INFO value"));
                DEL_PTR(l_input_tag);

                return FALSE;
            }


            //+Patni-RSG/Added/2009Nov25/MSA0248-0033
            if (f_scan_index >= 0) {
                UpdateAllSeriesUnderProtocol(&f_vect_series_loid, f_scan_index, l_input_tag);
                l_series_updated = true;
            }

            //-Patni-RSG/Added/2009Nov25/MSA0248-0033
            break;

            //-Patni-PJS/Added/2009Nov11/MSA0248-0033
        default:
            break;

    }


    if (l_error_code != MRC_DBSA_SUCCESS) {
        if (l_tagerrlist) {
            TagErrorProcess(l_tagerrlist);
            DBSA_FreeErrTagList(&l_tagerrlist);
        }

        DEL_PTR(l_input_tag);

        return FALSE;

    }

    //+Patni-RSG/Modified/2009Nov25/MSA0248-0033
    if (l_series_updated == false) {
        DBSAFactory l_factory;
        l_error_code = l_factory.DBSA_WriteSeries(&l_tagerrlist, l_protocol_id, l_input_tag);

        if (l_error_code != MRC_DBSA_SUCCESS) {
            if (l_tagerrlist) {
                TagErrorProcess(l_tagerrlist);
                DBSA_FreeErrTagList(&l_tagerrlist);
            }

            DEL_PTR(l_input_tag);
            return FALSE;
        }
    }

    //-Patni-RSG/Modified/2009Nov25/MSA0248-0033


    DEL_PTR(l_input_tag);

    if (l_protocol_id) {
        l_protocol_id = NULL;
    }

    return TRUE;

}
//+Patni-PJS/2010Oct26/Added/WFDA/IR-97
//****************************Method Header************************************
//Method Name   :CopyWFDAProtocolTags()
//Author        :PATNI/PJS
//Purpose       :Copy WFDA protocol tags.
//*****************************************************************************
/*void CPQMStudyMgr::CopyWFDAProtocolTags(
    CPqmProtocol* f_from_obj, CPqmProtocol* f_to_obj
)
{
    CDBSAVRNode*		l_input_tag = NULL;
    CDBSAVR*			l_vparam = NULL;
    CString             l_dbsa_tag_val = _T("");
    LPCTSTR				FUNC_NAME = _T("CPQMStudyMgr::CopyWFDAProtocolTags");
    DBSAFactory			l_factory;
    CDBSASeries*		l_dbsa_series  = NULL;
    CDBSATagErrorList*	l_errlist = NULL;
    int					l_result = 0;
    LPCTSTR*			l_series_loid = NULL;
    int					ret = 0;
    CDBSAVR 			*l_vparam_init_param = NULL, *l_vparam_analysis_param = NULL, *l_vparam_inscan_flag = NULL;

    CPQMStudy*          l_study = m_pqm_ptr_dbsa->GetStudy();
    if(NULL == f_from_obj || NULL == f_to_obj) {
        //Trace log
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
            _T("CPqmProtocol obj is NULL"));
        return;
    }

    //To fetch tag from Local DB

    l_input_tag	= new CDBSAVRNode();
    l_vparam_init_param = new CDBSAVROB(PQM_DCM_MR_WFDA_CAPP_INITIAL_PARAMETER);
    l_vparam_analysis_param = new CDBSAVROB(PQM_DCM_MR_WFDA_CAPP_ANALYSIS_PARAMETERS);
    l_vparam_inscan_flag = new CDBSAVROB(PQM_DCM_MR_WFDA_CAPP_INSCAN_FLAG);
    l_input_tag->addLeaf(l_vparam_init_param);
    l_input_tag->addLeaf(l_vparam_analysis_param);
    l_input_tag->addLeaf(l_vparam_inscan_flag);

    l_result = l_factory.DBSA_LoadSeries(&l_dbsa_series, &l_errlist, All, f_from_obj->GetSeriesLOID(), l_input_tag);

    if (MRC_DBSA_SUCCESS != l_result || (NULL == l_dbsa_series)) {
        if (l_errlist) {
            TagErrorProcess(l_errlist);
            DBSA_FreeErrTagList(&l_errlist);
        }

        CString temp_string = CPqmUtilities::GetMultiLingualString(_T("IDS_FAIL_TO_LOAD_SERIES"));
        CPQMLogMgr::GetInstance()->DisplayError(
            temp_string,
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
    }

    l_result = l_factory.DBSA_WriteSeries(&l_errlist, f_to_obj->GetSeriesLOID(), l_input_tag);

    if (MRC_DBSA_SUCCESS != l_result) {
        if (l_errlist) {
            TagErrorProcess(l_errlist);
            DBSA_FreeErrTagList(&l_errlist);
        }
        CString l_result_string = DBSA_GetErrorString(l_result);
        CString err_msg = _T("");
        err_msg.Format(_T("Unable To Update Series %s DBSA error string %s"), f_to_obj->GetSeriesLOID(), l_result_string);
        PQM_TRACE(USER_FUNC_MARKER, _T("CPQMStudyMgr::CopyWFDAProtocolTags"),
            err_msg);
    }
    DBSA_FreeSeries(l_dbsa_series);
    l_dbsa_series = NULL;

    if (l_input_tag) {
        delete l_input_tag;
        l_input_tag = NULL;
    }
}*/
//-Patni-PJS/2010Oct26/Added/WFDA/IR-97


/****************************Method Header************************************
//Method Name   :GetStudy()
//Author        :PATNI/RD
//Purpose       :Get Study Pointer
//*****************************************************************************/
CDBSAStudy* CPQMStudyMgr::GetStudy()
{

    LPCTSTR FUNC_NAME = _T("CPQMStudyMgr::GetStudy");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetStudy"));

    return m_dbsa_study;
}


//+Patni-Sribanta-HAR/2010Sep21/Added/TMSC-REDMINE-756
//****************************Method Header************************************
//Method Name   :GetSeriesCountInLocalDB()
//Author        :PATNI/HAR-Sribanta
//Purpose       :Function checks number of protocols present in Local DB and
//               return it.
//*****************************************************************************
int CPQMStudyMgr::GetSeriesCountInLocalDB()
{
    if (m_dbsa_study) {
        DBSA_FreeStudy(m_dbsa_study);
        m_dbsa_study = NULL;
    }

    if (m_dbsa_study == NULL) {
        DBSAFactory l_factory;
        CDBSATagErrorList* l_tagerrlist = NULL;
        int l_result = l_factory.DBSA_LoadStudy(&m_dbsa_study, &l_tagerrlist, PQM, m_stloid, NULL);

        if (l_result != MRC_DBSA_SUCCESS) {
            if (l_tagerrlist) {
                TagErrorProcess(l_tagerrlist);
                DBSA_FreeErrTagList(&l_tagerrlist);
            }
        }
    }

    WORD* l_acq_order = NULL;
    int l_acqsize = m_dbsa_study->DBSA_GetAcqOrder(&l_acq_order);

    if (NULL != l_acq_order) {
        DBSA_FreeLocalAcqOrder(l_acq_order);
    }

    return (l_acqsize);
}
//-Patni-Sribanta-HAR/2010Sep21/Added/TMSC-REDMINE-756

//Patni-PJS/2010May09/Added/JaFT# IR-141
//*****************************************************************************
//Method Name   : GetPatientUID
//Author        : Patni/PJS
//Purpose       : This function is used to get the Patient Id from Local DBGetPatientUID
//*****************************************************************************
CString CPQMStudyMgr::GetPatientUID(
    LPCTSTR f_patient_loid
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudyMgr::GetPatientUID");

    CString l_str_pnt_uid = _T("");

    if (!f_patient_loid) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Invalid patient loid"));
    }

    CDBSATagErrorList* error_list = NULL;
    CDBSAVR* leaf = new CDBSAVRIS(PQM_DCM_MR_PATIENT_ID);
    CDBSAVRNode* node =   new CDBSAVRNode();
    node->addLeaf(leaf);
    CDBSAPatient* out_node = NULL;

    DBSAFactory factory;
    int ret = factory.DBSA_LoadPatient(&out_node, &error_list, All, f_patient_loid, node);

    if (ret == MRC_DBSA_SUCCESS && out_node) {
        CDBSAVRLeaf* series_num_leaf = out_node->FindLeaf(PQM_DCM_MR_PATIENT_ID);

        if (series_num_leaf) {
            int l_size = ((CDBSAVR*)series_num_leaf)->GetSize();

            if (l_size > 0) {
                LPTSTR*  l_ptn_uid = new LPTSTR[l_size];
                ((CDBSAVR*)series_num_leaf)->GetValue(l_ptn_uid, l_size);
                l_str_pnt_uid = *l_ptn_uid;

            } else {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("failed to get the value for Patient ID"));
            }
        }

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("DBSA_LoadPatient failed"));
    }

    if (error_list) {
        DBSA_FreeErrTagList(&error_list);
    }

    if (out_node) {
        DBSA_FreePatient(out_node);
    }

    DEL_PTR_ARY(node);

    return  l_str_pnt_uid;
}

//*****************************************************************************
//Method Name   : GetStudyUID
//Author        : Patni/PJS
//Purpose       : This function is used to get the Study UID Id from Local DB
//*****************************************************************************
CString CPQMStudyMgr::GetStudyUID(
    LPCTSTR f_study_loid
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudyMgr::GetStudyUID");
    CString l_str_pnt_uid = _T("");

    if (!f_study_loid) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Invalid study loid"));
        return l_str_pnt_uid;
    }

    DBSAFactory factory;
    CDBSATagErrorList* error_list = NULL;
    CDBSAVR* leaf = new CDBSAVRIS(PQM_DCM_MR_STUDY_UID);
    CDBSAVRNode* node =   new CDBSAVRNode();
    node->addLeaf(leaf);
    CDBSAStudy* out_node = NULL;

    int ret = factory.DBSA_LoadStudy(&out_node, &error_list, All, f_study_loid, node);

    if (ret == MRC_DBSA_SUCCESS && out_node) {
        CDBSAVRLeaf* series_num_leaf = out_node->FindLeaf(PQM_DCM_MR_STUDY_UID);

        if (series_num_leaf) {
            int l_size = ((CDBSAVR*)series_num_leaf)->GetSize();

            if (l_size > 0) {
                LPTSTR* l_std_uid = new LPTSTR[l_size];
                ((CDBSAVR*)series_num_leaf)->GetValue(l_std_uid, l_size);
                l_str_pnt_uid = *l_std_uid;

            } else {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("failed to get the value for Study UID"));
            }
        }

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("DBSA_LoadStudy failed"));
    }

    if (error_list) {
        DBSA_FreeErrTagList(&error_list);
    }

    if (out_node) {
        DBSA_FreeStudy(out_node);
    }

    DEL_PTR_ARY(node);


    return  l_str_pnt_uid;
}

//+Patni-ARD/2009June1/Added/JFT# PQM-SR GL_SR_SQSaved message
//****************************Method Header*************************************
//Method Name   : GetNewSeriesLoids()
//Author        : PATNI/AMT
//Purpose       : This function will returns the array of newly creates series
//                  LOID due to  Addition and Duplication operation.
//*****************************************************************************/
CStringArray* CPQMStudyMgr::GetNewSeriesLoids(
)const
{
    return m_new_series_loids;
}
//-Patni-ARD/2009June1/Added/JFT# PQM-SR GL_SR_SQSaved message

/****************************Method Header************************************
//Method Name   :GetSeriesCount()
//Author        :PATNI
//Purpose       :Returns the series count
//*****************************************************************************/
int CPQMStudyMgr::GetSeriesCount()const
{
    //+Patni-PJS/Added/TMSC-REDMINE-718
    //return m_series_count;
    WORD* l_acqorder = NULL;
    //+Patni-DKH/Added/20010-Oct-05/TMSC-REDMINE-756-Part1
    int l_acqsize = m_dbsa_study->DBSA_GetAcqOrder(&l_acqorder);
    //-Patni-PJS/Added/TMSC-REDMINE-718

    if (NULL != l_acqorder) {
        DBSA_FreeLocalAcqOrder(l_acqorder);
    }

    return l_acqsize ;
    //-Patni-DKH/Added/20010-Oct-05/TMSC-REDMINE-756-Part1
}


/****************************Method Header************************************
//Method Name   :GetStudyNameatStudyLevel()
//Author        :PATNI/JS
//Purpose       :
//*****************************************************************************/
CString CPQMStudyMgr::GetStudyNameatStudyLevel(
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudyMgr::GetStudyNameatStudyLevel");

    if (m_dbsa_study) {
        //	if(wcscmp(l_studyloid, m_dbsa_study->DBSA_GetLOID()) != 0){
        DBSA_FreeStudy(m_dbsa_study);
        m_dbsa_study = NULL;
        //	}
    }

    CDBSATagErrorList* l_tagerrlist = NULL;

    if (m_dbsa_study == NULL) {
        LPCTSTR l_studyloid = GetLOID();
        DBSAFactory  l_dbsa_factory;
        l_dbsa_factory.DBSA_LoadStudy(&m_dbsa_study, &l_tagerrlist,
                                      PQM, l_studyloid, NULL);
    }

    CDBSAVR* l_vrparam = NULL;
    BOOL l_result = m_dbsa_study->DBSA_GetValue(&l_vrparam, PQM_DCM_MR_STUDY_NODE_PATH,
                    MRC_TRUE, &l_tagerrlist);
    CString l_study_name(_T(""));

    if (l_result == MRC_DBSA_SUCCESS) {
        //+Patni-AMT/2009Mar27/Added/PSP1#307
        if (l_vrparam != NULL) {
            LPTSTR l_study_name_buff = NULL;
            l_vrparam->GetString(&l_study_name_buff, 0);
            l_study_name = l_study_name_buff;
        }

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("CPQMStudyMgr::GetStudyNameatStudyLevel Study Name = ") + l_study_name);

        //-Patni-AMT/2009Mar27/Added/PSP1#307

    } else {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, DBSA_GetErrorString(l_result));
    }

    return l_study_name;
}

/****************************Method Header************************************
//Method Name   :GetSeriesLoid()
//Author        :PATNI/HAR
//Purpose       :
// Modified		: this method has modified for f_index
				: + sarjeet 24/04/08
//*****************************************************************************/
CString CPQMStudyMgr::GetSeriesLoid(
    const CString& f_study_loid,
    int* f_index
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudyMgr::GetSeriesLoid");

    if (f_index) {
        CString l_str_msg(_T(""));
        l_str_msg.Format(_T("Study Loid %s : Index = %d"), f_study_loid, *f_index);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str_msg);
    }

    int l_result = 0;
    //  if(m_dbsa_study){
    //		DBSA_FreeStudy(m_dbsa_study);
    //		//m_dbsa_study->DBSA_FreeLowObject();
    //		m_dbsa_study = NULL;
    // 	}

    CDBSATagErrorList* l_tagerrlist = NULL;

    if (m_dbsa_study == NULL) {
        DBSAFactory l_factory;
        l_result = l_factory.DBSA_LoadStudy(&m_dbsa_study, &l_tagerrlist, PQM, f_study_loid, NULL);

        if (l_result != MRC_DBSA_SUCCESS) {
            if (l_tagerrlist) {
                TagErrorProcess(l_tagerrlist);
                DBSA_FreeErrTagList(&l_tagerrlist);
            }

            return _T("");
        }
    }

    if (m_dbsa_study == NULL) {

        CPQMLogMgr::GetInstance()->WriteEventLog(
            _T("Study LOID pointer is null, unable to load study LOID."),
            ERROR_LOG_INFORMATION1, _T("PQMStudyMgr.cpp"), __LINE__); //Patni-KSS/2010May19/ADDED/IR-82

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_STUDY_LOID_POINTER")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return _T("");
    }

    CDBSASeries* l_series = NULL;

    try {

        //if (*f_index == 0 ) {
        if (*f_index == -1) {  //Meghana for defect fix @INT10 no 139
            l_result = m_dbsa_study->DBSA_GetFirstSeries(&l_series, &l_tagerrlist, PQM);

        } else {
            l_result = m_dbsa_study->DBSA_GetNextSeries(&l_series, &l_tagerrlist,
                       PQM, *f_index);
        }

    } catch (...) {
        //TMSC/2009Nov12/Modified/Code Changes
        throw;
        return _T("");
    }

    if (l_result != MRC_DBSA_SUCCESS) {
        if (l_tagerrlist) {
            TagErrorProcess(l_tagerrlist);
            DBSA_FreeErrTagList(&l_tagerrlist);
        }

        return _T("");
    }


    CString l_series_loid = l_series->DBSA_GetLOID();

    *f_index = 	l_series->DBSA_GetIndex();

    PQM_TRACE1(USER_FUNC_MARKER, FUNC_NAME, l_series_loid);
    return l_series_loid;
}

/****************************Method Header************************************
//Method Name   :GetSeriesBias()
//Author        :PATNI/RSG
//Purpose       :To Get Series Bias Value. First Read From MPlusDB if not found read
                 from config file if not found set 1000 as Bias value and return the same.
//*****************************************************************************/
bool CPQMStudyMgr::GetSeriesBias(
    unsigned short* f_series_bias
)
{
    //Now get the BIAS from the Study

    LPCTSTR FUNC_NAME = _T("CPQMStudyMgr::GetSeriesBias");
    CDBSAVR* l_vrparamBias      = NULL ;
    CDBSATagErrorList* l_errlist = NULL;
    bool l_present_in_db        = false;


    int l_result = m_dbsa_study->DBSA_GetValue(&l_vrparamBias, PQM_DCM_MR_SERIES_NUMBER_BIAS, MRC_TRUE, &l_errlist);

    if (l_result == MRC_DBSA_SUCCESS) {
        //Patni-MP/2009Mar29/Modified/Internal defect fix
        if (l_vrparamBias != NULL) {
            l_vrparamBias->GetValue(f_series_bias, 1);
            l_present_in_db = true;
        }

    } else {

        DEL_PTR(l_vrparamBias);


        if (l_errlist) {
            TagErrorProcess(l_errlist);
            DBSA_FreeErrTagList(&l_errlist);
        }

        PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN, FUNC_NAME,
                  _T("Error PQM_DCM_MR_SERIES_NUMBER_BIAS"));
    }

    //If no value for Bias is found than treat the value got from .ini file as the Bias value
    if (*f_series_bias == 0) {
        //Patni-ARD/2010Mar3/Modified/DeFT# IR-100 remove IPCConfig.ini
        //*f_series_bias = CPQMConfig::GetInstance()->GetSeriesBiasValue();

        *f_series_bias = m_series_bias;

        if (*f_series_bias <= 0) {
            *f_series_bias = 1000;
        }
    }

    //+Patni+RSG/2009Dec18/MVC6256
    /* if (l_vrparamBias) {
         delete l_vrparamBias;
         l_vrparamBias = NULL;
     }
     */
    //-Patni+RSG/2009Dec18/MVC6256
    return l_present_in_db;
}
//-Patni-RSG/2009Dec16/MVC6256

//+Patni-ARD/2010Mar3/Added/DeFT# IR-100 remove IPCConfig.ini
void CPQMStudyMgr::SetSeriesBias(const int f_series_bias)
{
    m_series_bias = f_series_bias;
}

/****************************Method Header************************************
//Method Name   :GetTotalCount()
//Author        :PATNI/RD
//Purpose       :Get Total Count information
//*****************************************************************************/
int CPQMStudyMgr::GetTotalCount(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudyMgr::GetTotalCount");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetTotalCount"));

    return m_totalcount;
}

/****************************Method Header************************************
//Method Name   :SetTotalCount()
//Author        :PATNI/RD
//Purpose       :Set Total Count information
//*****************************************************************************/
void CPQMStudyMgr::SetTotalCount(
    const int f_totalcount
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudyMgr::SetTotalCount");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetTotalCount"));

    m_totalcount = f_totalcount;
}

/****************************Method Header************************************
//Method Name   :GetLeafCount()
//Author        :PATNI/RD
//Purpose       :Get Leaf Count information
//*****************************************************************************/
int CPQMStudyMgr::GetLeafCount(
)const
{

    LPCTSTR FUNC_NAME = _T("CPQMStudyMgr::GetLeafCount");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetLeafCount"));

    return m_leafcount;
}

/****************************Method Header************************************
//Method Name   :SetLeafCount()
//Author        :PATNI/RD
//Purpose       :Set Leaf Count information
//*****************************************************************************/
void CPQMStudyMgr::SetLeafCount(
    const int f_leafcount
)
{

    LPCTSTR FUNC_NAME = _T("CPQMStudyMgr::SetLeafCount");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetLeafCount"));

    m_leafcount = f_leafcount;
}

/****************************Method Header************************************
//Method Name   :GetNodeCount()
//Author        :PATNI/RD
//Purpose       :Get Node Count information
/*****************************************************************************/
int CPQMStudyMgr::GetNodeCount(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudyMgr::GetNodeCount");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetNodeCount"));
    return m_node_count;
}

/****************************Method Header************************************
//Method Name   :SetNodeCount()
//Author        :PATNI/RD
//Purpose       :Set Node Count information
//*****************************************************************************/
void CPQMStudyMgr::SetNodeCount(
    const int f_nodecount
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudyMgr::SetNodeCount");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetNodeCount"));
    m_node_count = f_nodecount;
}

/****************************Method Header************************************
//Method Name   :GetLOID()
//Author        :PATNI/RD
//Purpose       :Get study LOID information
//*****************************************************************************/
LPCTSTR CPQMStudyMgr::GetLOID(
)const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudyMgr::GetLOID");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of GetLoid"));
    return m_stloid;
}

/****************************Method Header************************************
//Method Name   :SetLOID()
//Author        :PATNI/RD
//Purpose       :Setting LOID information
//*****************************************************************************/
void CPQMStudyMgr::SetLoid(
    LPCTSTR f_loid
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudyMgr::SetLoid");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetLoid"));
    m_stloid = f_loid;
}

/****************************Method Header************************************
//Method Name   :SetSeriesNoInDB()
//Author        :PATNI/MRP
//Purpose       :To set all the 4 series related tags in the MPlus DB
				 This function should be called after changing the status of the
				 protocol to "DONE" or "FAILED"
// Modified		:
//*****************************************************************************/
//+Patni-RSG/Modified/2009Dec16/MVC6256
int CPQMStudyMgr::SetSeriesNoInDB(
    CString f_series_loid,
    const int f_acqStatus,
    const int f_prot_num
)
//+Patni-RSG/Modified/2009Dec16/MVC6256
{

    LPCTSTR FUNC_NAME = _T("CPQMStudyMgr::SetSeriesNoInDB");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("Trace of CPQMStudyMgr::SetSeriesNoInDB"));

    LPCTSTR l_studyLOID = GetLOID();
    int l_result = -1;
    CDBSATagErrorList* l_errlist = NULL;
    DBSAFactory l_factory;
    //Patni-MP/2009Mar28/Modified/PSP-BIAS FIX



    if (m_dbsa_study == NULL) {

        l_result = l_factory.DBSA_LoadStudy(&m_dbsa_study, &l_errlist, PQM, l_studyLOID, NULL);

        if (l_result != MRC_DBSA_SUCCESS) {
            if (l_errlist) {
                TagErrorProcess(l_errlist);
                DBSA_FreeErrTagList(&l_errlist);
            }

            //+Patni-RSG/2009Dec30/Modified/MVC006555
            PQM_TRACE(USER_FUNC_MARKER,
                      FUNC_NAME, DBSA_GetErrorString(l_result));
            //-Patni-RSG/2009Dec30/Modified/MVC006555

            return E_ERROR;
        }

    }

    l_studyLOID = m_dbsa_study->DBSA_GetLOID();
    PQM_TRACE(COMMHANDLER_MARKER_PQM_ACQMAN,
              FUNC_NAME, l_studyLOID);

    //Add the Series No only if the Status of the protocol is Done or Wait
    //Patni-PJS/2009Dec15/Modified/MVC006256
    if ((f_acqStatus == C_PR_STATUS_DONE) || (f_acqStatus == C_PR_STATUS_FAIL) || (f_acqStatus == C_PR_STATUS_CURRENT)) {
        //Now get the BIAS from the Study
        //+Patni-RSG/2009Dec16/MVC6256
        unsigned short l_seriesBias = 0;
        bool status = GetSeriesBias(&l_seriesBias);

        if (f_prot_num >= 0) {
            bool l_series_num_st = ValidateSeriesNo(f_prot_num, f_series_loid, l_seriesBias);

            if (l_series_num_st) {
                return E_NO_ERROR;
            }
        }

        //Add Series Bias tag 00111006 to study
        CDBSAVRNode* l_input_tag_SQ = new CDBSAVRNode();

        if (!status) {
            CDBSAVR* l_vrparamBias = new CDBSAVRUS(PQM_DCM_MR_SERIES_NUMBER_BIAS, DCM_VR_LEN_US);
            unsigned short temp = l_seriesBias;
            int res = l_vrparamBias->SetValue(&temp, 1);

            //Patni-RSG/2009Dec30/Modified/MVC006555
            if (res != MRC_DBSA_SUCCESS) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, DBSA_GetErrorString(res));

            } else {
                l_input_tag_SQ->addLeaf(l_vrparamBias);
            }

            //Patni-RSG/2009Dec30/Modified/MVC006555
        }

        //-Patni-RSG/2009Dec16/MVC6256


        //Get the number of protocols from the Database
        //l_result = m_dbsa_study->DBSA_GetValue(&l_vrparamNumProt,PQM_DCM_MR_SCH_NUM_PROTOCOLS,MRC_TRUE,&l_errlist);
        CDBSAVR* l_vrparamNumProt = m_dbsa_study->GetValue(PQM_DCM_MR_SCH_NUM_PROTOCOLS);

        unsigned short l_seriescnt = 0;

        if (l_vrparamNumProt != NULL) {
            l_vrparamNumProt->GetValue(&l_seriescnt, 1);

        } else {
            DEL_PTR(l_vrparamNumProt);
        }

        l_seriescnt = l_seriescnt + 1;

        //Increment the count of protocols and write back to Study
        CDBSAVRNode* l_input_tag = new CDBSAVRNode();

        CDBSAVR* l_vrparam = new CDBSAVRUS(PQM_DCM_MR_SCH_NUM_PROTOCOLS);

        l_result = l_vrparam->SetValue(&l_seriescnt, 1);
        l_input_tag->addLeaf(l_vrparam);


        if (l_result != MRC_DBSA_SUCCESS) {
            //+Patni-RSG/2009Dec30/Modified/MVC006555
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, DBSA_GetErrorString(l_result));

            //-Patni-RSG/2009Dec30/Modified/MVC006555
            DEL_PTR(l_input_tag);

            return E_ERROR;
        }

        l_result = m_dbsa_study->DBSA_WriteData(&l_errlist, l_input_tag);

        if (l_result != MRC_DBSA_SUCCESS) {
            //+Patni-RSG/2009Dec30/Modified/MVC006555
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, DBSA_GetErrorString(l_result));
            //-Patni-RSG/2009Dec30/Modified/MVC006555

            if (l_errlist) {

                DEL_PTR(l_input_tag);

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, DBSA_GetErrorString(l_result));

                TagErrorProcess(l_errlist);
                DBSA_FreeErrTagList(&l_errlist);
            }


            return E_ERROR;
        }

        l_result = m_dbsa_study->DBSA_Put(&l_errlist);

        if (l_result != MRC_DBSA_SUCCESS) {
            if (l_errlist) {

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, DBSA_GetErrorString(l_result));

                TagErrorProcess(l_errlist);
                DBSA_FreeErrTagList(&l_errlist);
            }

            //+Patni-RSG/2009Dec30/Modified/MVC006555
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, DBSA_GetErrorString(l_result));
            //-Patni-RSG/2009Dec30/Modified/MVC006555

            return E_ERROR;
        }

        //write the SQ tag in the Study
        CDBSAVRNode* sq_node = new CDBSAVRNode(PQM_DCM_MR_SCH_PROTOCOL_GROUP, l_seriescnt - 1);

        l_vrparam = new CDBSAVRUS(PQM_DCM_MR_SCH_NUM_SERIES_IN_PROTOCOL);
        unsigned short l_dummy = 1;
        l_result = l_vrparam->SetValue(&l_dummy, 1);
        sq_node->addLeaf(l_vrparam);
        l_input_tag_SQ->addNode(sq_node);

        if (l_result != MRC_DBSA_SUCCESS) {

            //+Patni-RSG/2009Dec30/Modified/MVC006555
            const CString log_str = _T("Error SetValue PQM_DCM_MR_SCH_NUM_SERIES_IN_PROTOCOL: ");
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, log_str + DBSA_GetErrorString(l_result));
            //+Patni-RSG/2009Dec30/Modified/MVC006555

            DEL_PTR(l_input_tag_SQ);

            return E_ERROR;
        }


        l_result = l_factory.DBSA_WriteStudy(&l_errlist, l_studyLOID, l_input_tag_SQ);

        if (l_result != MRC_DBSA_SUCCESS) {

            //+Patni-RSG/2009Dec30/Modified/MVC006555
            const CString log_str = _T("Error DBSA_WriteStudy");
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, log_str + DBSA_GetErrorString(l_result));
            //+Patni-RSG/2009Dec30/Modified/MVC006555

            if (l_errlist) {
                DEL_PTR(l_input_tag_SQ);

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, DBSA_GetErrorString(l_result));

                TagErrorProcess(l_errlist);
                DBSA_FreeErrTagList(&l_errlist);
            }

            return E_ERROR;
        }

        //-Patni-RSG/Modified-2009May14/JFT- Series Bias Requirement # 1

        DEL_PTR(l_input_tag);

        DEL_PTR(l_input_tag_SQ);

        //Update the Series number at the Series level

        long l_series_number = 0;
        l_series_number = l_seriescnt;
        //Multiply the series no with bias
        l_series_number *= l_seriesBias;

        CDBSAVRNode* l_input_tag_SR_NO = new CDBSAVRNode();
        l_vrparam = new CDBSAVRIS(PQM_DCM_ST_SERIES_NUMBER);

        l_input_tag_SR_NO->addLeaf(l_vrparam);
        l_result = l_vrparam->SetValue(&l_series_number, 1);

        if (l_result != MRC_DBSA_SUCCESS) {
            //+Patni-RSG/2009Dec30/Modified/MVC006555
            const CString log_str = _T("CPQMStudyMgr::SetSeriesNoInDB Error SetValue PQM_DCM_ST_SERIES_NUMBER : ");
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, log_str + DBSA_GetErrorString(l_result));
            //+Patni-RSG/2009Dec30/Modified/MVC006555

            DEL_PTR(l_input_tag_SR_NO);

            return E_ERROR;
        }



        l_result = l_factory.DBSA_WriteSeries(&l_errlist, f_series_loid, l_input_tag_SR_NO);


        DEL_PTR(l_input_tag_SR_NO);

        if (l_result != MRC_DBSA_SUCCESS) {

            const CString log_str = _T("Error In DBSA_WriteSeries : ");
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, log_str + DBSA_GetErrorString(l_result));

            if (l_errlist) {
                TagErrorProcess(l_errlist);
                DBSA_FreeErrTagList(&l_errlist);
            }

            return E_ERROR;
        }
    }

    return E_NO_ERROR;
}

// + Patni - HAR / 2009Mar11 / Write Protocol Status In Local DB
//****************************Method Header*************************************
//Method Name   : SetAcquisitionStatusInLocalDB()
//Author        : PATNI / HAR
//Purpose       :
//*****************************************************************************/
BOOL CPQMStudyMgr::SetAcquisitionStatusInLocalDB(
    BSTR f_series_loid,
    const int f_acq_status
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudyMgr::SetAcquisitionStatusInLocalDB");

    CString l_print_str(_T(""));
    l_print_str.Format(_T("AcqStatus --> %d"), f_acq_status);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Series LOID -> ") + CString(f_series_loid) + l_print_str);

    CDBSAVRNode*  l_input_tag = new CDBSAVRNode();
    CDBSAVR* l_vrparam   = new CDBSAVRSL(PQM_SVN_EXE_PROTOCOL_ACQ_STATUS);

    ASSERT(l_vrparam);
    l_input_tag->addLeaf(l_vrparam);
    long l_acq_status = f_acq_status;
    l_vrparam->SetValue(&l_acq_status, 1);

    CDBSATagErrorList*   l_tagerrlist   = NULL;
    DBSAFactory l_factory;
    int l_error_code = l_factory.DBSA_WriteSeries(
                           &l_tagerrlist, f_series_loid, l_input_tag);

    BOOL l_result = TRUE;

    if (l_error_code != MRC_DBSA_SUCCESS) {
        if (l_tagerrlist) {
            TagErrorProcess(l_tagerrlist);
            DBSA_FreeErrTagList(&l_tagerrlist);
        }

        l_result = FALSE;
    }

    DEL_PTR(l_vrparam);

    return l_result;
}
// - Patni - HAR / 2009Mar11 / Write Protocol Status In Local DB

/****************************Method Header************************************
//Method Name   :SetLeafCountSeries()
//Author        :PATNI/RD
//Purpose       :Set LeafCount for Series under specified study
//*****************************************************************************/
void CPQMStudyMgr::SetLeafCountSeries(
    const int f_seriescount
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudyMgr::SetLeafCountSeries");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace of SetLeafCountSeries"));

    m_leafcount_series = f_seriescount;
}

/****************************Method Header************************************
//Method Name   :SetSeriesOrder()
//Author        :PATNI/PJP
//Purpose       :This method Rearranging the information of series
//*****************************************************************************/
void CPQMStudyMgr::SetSeriesOrder(
    BSTR f_bstr,
    const int f_dropindex
)
{

    LPCTSTR FUNC_NAME = _T("CPQMStudyMgr::SetSeriesOrder");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, f_bstr);

    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    //int l_orderarr[NUM_64];

    //+Patni-SS/MODI-ADDED/2009-May-22/AcqOrder problem
    //int l_seriescnt = m_dbsa_study->DBSA_ObjectCount();

    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (m_dbsa_study == NULL) {
        return;
    }

    //Patni-ARD/2009June1/Added/JFT# PQM-SR GL_SR_SQSaved
    WORD* l_acqorder = NULL;
    int l_acqorder_size = m_dbsa_study->DBSA_GetAcqOrder(&l_acqorder);

    // Read bString and form Array of parameters
    CString l_tmpid(_T(""));
    CString l_seqid = f_bstr;

    CStringArray l_str_arr;

    while (l_seqid.GetLength()) {
        l_tmpid = l_seqid.Left(l_seqid.Find(_T(" | "), 0));

        if (l_tmpid.GetLength() == 0) {
            l_str_arr.Add(l_seqid);
            l_seqid.TrimLeft(_T(" | "));
            break;

        } else {
            l_str_arr.Add(l_tmpid);
        }

        l_seqid.TrimLeft(l_tmpid);
        l_seqid.TrimLeft(_T(" | "));

    }

    int l_orderarr[NUM_64] = {0};
    memset(l_orderarr, NULL, NUM_64);

    for (int i = 0; i < l_str_arr.GetSize(); i++) {
        l_orderarr[i] = _wtoi(l_str_arr[i]);
    }

    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    //int l_count;
    int l_count = 0;
    bool fmatch = false;

    for (i = 0, l_count = 0 ; i < l_acqorder_size; i++) {
        //bool fmatch = false;
        fmatch = false;

        //- Patni-PJS/2009Apr25/Modify/CPP Test tool execution
        if (f_dropindex == i) {
            if (f_dropindex > l_orderarr[0]) {
                m_wacqorder[l_count++] = l_acqorder[i];
            }

            for (int j = 0 ; j < l_str_arr.GetSize(); j++) {
                m_wacqorder[l_count++] = l_acqorder[l_orderarr[j]];
            }

            if (f_dropindex < l_orderarr[0]) {
                m_wacqorder[l_count++] = l_acqorder[i];
            }

        } else {
            for (int j = 0 ; j < l_str_arr.GetSize(); j++) {
                if (i == l_orderarr[j]) {
                    fmatch = true;
                    break;
                }
            }

            if (!fmatch) {
                m_wacqorder[l_count++] = l_acqorder[i];
            }
        }
    }

    m_dbsa_study->DBSA_AcqOrderUpdate(m_wacqorder, m_series_count);

    //+Patni-DKH/Added/20010-Oct-05/TMSC-REDMINE-756-Part1
    if (NULL != l_acqorder) {
        DBSA_FreeLocalAcqOrder(l_acqorder);
    }

    //-Patni-DKH/Added/20010-Oct-05/TMSC-REDMINE-756-Part1
    CDBSATagErrorList* l_tagerrlist = NULL;
    int l_result = m_dbsa_study->DBSA_Put(&l_tagerrlist);

    if (l_result != MRC_DBSA_SUCCESS) {
        if (l_tagerrlist) {
            TagErrorProcess(l_tagerrlist);
            DBSA_FreeErrTagList(&l_tagerrlist);
        }


        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_REARRANGE_SERIES_IN_DATABASE")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
    }
}

/****************************Method Header************************************
//Method Name   :SetchangeOrder()
//Author        :PATNI/PJP
//Purpose       :Before changing order of series,the order of series in Local DB
//*****************************************************************************/
void CPQMStudyMgr::SetchangeOrder(
    LPCTSTR f_loid
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudyMgr::SetchangeOrder");

    CString l_str_msg(_T(""));
    l_str_msg.Format(_T("%d"), f_loid);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str_msg);

    CString l_str(f_loid);
    m_searray.Add(l_str);
}

/****************************Method Header************************************
//Method Name   :ReadSeriesData()
//Author        :PATNI/JS
//Purpose       :For reading the data from DB
//*****************************************************************************/
BOOL CPQMStudyMgr::ReadSeriesData(
    CDBSASeries* f_series,
    CPASParamStore* f_objParamStore
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudyMgr::ReadSeriesData");
    CDBSATagErrorList* l_tagerrlist = NULL;
    CString l_str(_T(""));
    WCHAR* tmp = NULL;
    CDBSAVR* l_vrparam = NULL;
    int l_result = f_series->DBSA_GetValue(&l_vrparam, DCM_ST_SERIES_DATE, MRC_FALSE, &l_tagerrlist);

    if (l_result == MRC_DBSA_SUCCESS) {

        tmp = new WCHAR[NUM_11];
        wcscpy(tmp, _T("01/15/2004"));

    } else {

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_SERIES_DATE_TAG_ERROR")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return false;
    }

    //+Patni-HAR/2010Apr26/Commented/MVC006285
    //l_result = f_series->DBSA_GetValue(&l_vrparam, DCM_ST_SERIES_TIME, MRC_FALSE, &l_tagerrlist);
    //
    //if (l_result == MRC_DBSA_SUCCESS) {
    //    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    //    /*
    //    unsigned long tmdata[NUM_2];
    //    double sec=0;
    //    */
    //    unsigned long tmdata[NUM_2] = {0l};
    //    double sec = 0.0f;
    //    //- Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    //    l_vrparam->GetValue(tmdata, (unsigned int)sec);
    //    l_scanstart.Format(_T("%s%s"), tmp, _T("[16:50]"));
    //
    //    if (tmp) {
    //        delete []tmp;
    //        tmp = NULL;
    //    }
    //
    //} else {
    //    temp_string = CPqmUtilities::GetMultiLingualString(_T("IDS_SERIES_TIME_TAG_ERROR"));  // Added By KT/ACE-2/15-06-2009/Multilingualisation
    //    CPQMLogMgr::GetInstance()->DisplayError(
    //        //_T("DCM_ST_SERIES_TIME Tag  Unavailable"),                                         // Added By KT/ACE-2/15-06-2009/Multilingualisation
    //        temp_string,                                                                        // Added By KT/ACE-2/15-06-2009/Multilingualisation
    //        _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
    //        ERR_ID, APP_NAME);
    //    return false;
    //}
    //-Patni-HAR/2010Apr26/Commented/MVC006285

    CString l_scanstart(_T(""));
    f_objParamStore->SetString(_T("SCANSTART"), l_scanstart);
    //+Patni-PJS/2010May20/Added/JaFT/TMSC-REDMINE-285
    f_objParamStore->SetString(_T("REUSESHIM"), _T(""));
    f_objParamStore->SetString(_T("REUSEMAP"), _T(""));
    //-Patni-PJS/2010May20/Added/JaFT/TMSC-REDMINE-285
    // + Jitendra PQM6 17 July 2007
    l_result = f_series->DBSA_GetValue(&l_vrparam, DCM_ST_SERIES_DESCRIPTION, MRC_FALSE, &l_tagerrlist);

    if (l_result == MRC_DBSA_SUCCESS) {
        LPTSTR dt = _T(""); //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
        int istr = l_vrparam->GetString(&dt, 0);
        l_str = dt;
        f_objParamStore->SetString(_T("MEMO"), l_str);

    } else {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, DBSA_GetErrorString(l_result));
        f_objParamStore->SetString(_T("MEMO"), _T(""));

    }

    //          + Jitendra PQM6 17 July 2007
    l_result = f_series->DBSA_GetValue(&l_vrparam, DCM_ST_CONTRAST_BOLUS_AGENT, MRC_FALSE, &l_tagerrlist);

    if (l_result == MRC_DBSA_SUCCESS) {
        LPTSTR dt = _T(""); //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
        int istr = l_vrparam->GetString(&dt, 0);
        l_str = dt;
        f_objParamStore->SetString(_T("CONTRASTNAME"), l_str);

    } else {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_str);
        f_objParamStore->SetString(_T("CONTRASTNAME"), _T(""));

    }


    // + Jitendra PQM6 17 July 2007
    l_result = f_series->DBSA_GetValue(&l_vrparam, DCM_ST_MR_ACQUISITION_TYPE, MRC_FALSE, &l_tagerrlist);

    if (l_result == MRC_DBSA_SUCCESS) {
        LPTSTR dt = _T(""); //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
        int istr = l_vrparam->GetString(&dt, 0);
        int fdata = _wtoi(dt);
        f_objParamStore->SetInt(_T("IMGMODE"), fdata);

    } else {

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_ACQUISITION_TYPE_TAG_ERROR")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return false;
    }

    // + Jitendra PQM6 17 July 2007
    l_result = f_series->DBSA_GetValue(&l_vrparam, DCM_ST_REPETITION_TIME, MRC_FALSE, &l_tagerrlist);

    if (l_result == MRC_DBSA_SUCCESS) {
        LPTSTR dt = _T(""); //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
        int istr = l_vrparam->GetString(&dt, 0);
        float fdata = (float)_wtol(dt);
        f_objParamStore->SetFloat(_T("TR"), fdata);

    } else {

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_REPETITION_TIME_TAG_ERROR")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return false;
    }

    // + Jitendra PQM6 17 July 2007
    l_result = f_series->DBSA_GetValue(&l_vrparam, DCM_ST_NUMBER_OF_AVERAGES, MRC_FALSE, &l_tagerrlist);

    if (l_result == MRC_DBSA_SUCCESS) {
        LPTSTR dt = _T(""); //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
        int istr = l_vrparam->GetString(&dt, 0);
        l_str = dt;
        f_objParamStore->SetVector(_T("NAQ"), l_str);

    } else {

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_NO_OF_AVERAGES_TAG_ERROR")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return false;
    }

    l_result = f_series->DBSA_GetValue(&l_vrparam, PQM_SVN_STD_PROTOCOL_NAME, MRC_FALSE, &l_tagerrlist);

    if (l_result == MRC_DBSA_SUCCESS) {
        LPTSTR dt = _T(""); //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
        int istr = l_vrparam->GetString(&dt, 0);
        l_str = dt;
        f_objParamStore->SetVector(_T("NAQ"), l_str);
        f_objParamStore->SetString(_T("COMM"), l_str);

    } else {

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_PROTOCOL_NAME_TAG_ERROR")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return false;
    }


    //+ Jitendra PQM6 17 July 2007
    l_result = f_series->DBSA_GetValue(&l_vrparam, DCM_ST_ACQUISITION_MATRIX, MRC_FALSE, &l_tagerrlist);

    if (l_result == MRC_DBSA_SUCCESS) {
        //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
        //unsigned short usdata[NUM_2];
        unsigned short usdata[NUM_2] = {0};
        l_vrparam->GetValue(usdata, 1);
        l_str.Format(_T("%d,%d"), usdata[0], usdata[1]);
        f_objParamStore->SetVector(_T("MTX"), l_str);

    } else {

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_ACQUISITION_MATRIX_TAG_ERROR")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return false;
    }

    l_result = f_series->DBSA_GetValue(&l_vrparam, PQM_SVN_PRC_REFINE_FILTER, MRC_FALSE, &l_tagerrlist);

    if (l_result == MRC_DBSA_SUCCESS) {
        //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
        //long rflt_data;
        long rflt_data = 0l;
        l_vrparam->GetValue(&rflt_data, 1);

        f_objParamStore->SetFloat(_T("RFLT"), (float)rflt_data);

    } else {

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_PRC_REFINE_FILTER_TAG_ERROR")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return false;
    }

    l_result = f_series->DBSA_GetValue(&l_vrparam, PQM_SVN_PRC_K_SPACE_FILTER, MRC_FALSE, &l_tagerrlist);

    if (l_result == MRC_DBSA_SUCCESS) {
        //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
        //long kflt_data;
        long kflt_data = 0L; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
        l_vrparam->GetValue(&kflt_data, 1);
        f_objParamStore->SetFloat(_T("KFLT"), (float)kflt_data);

    }   else {

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_PRC_SPACE_FILTER_TAG_ERROR")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return false;
    }

    l_result = f_series->DBSA_GetValue(&l_vrparam, PQM_SVN_PLN_COVERAGES, MRC_FALSE, &l_tagerrlist);

    if (l_result == MRC_DBSA_SUCCESS) {
        //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
        //long cov_data;
        long cov_data = 0l;
        l_vrparam->GetValue(&cov_data, 1);
        f_objParamStore->SetFloat(_T("COV"), (float)cov_data);

    } else {

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_PLN_COVERAGES_TAG_ERROR")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return false;
    }

    l_result = f_series->DBSA_GetValue(&l_vrparam, PQM_SVN_PLN_SELECTED_SLICE_THK, MRC_FALSE, &l_tagerrlist);

    if (l_result == MRC_DBSA_SUCCESS) {
        //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
        //float fdata;
        float fdata = 0.0f;
        l_vrparam->GetValue(&fdata, 1);
        //+ Patni-PJS/2009Jul23/Modify/IDS#491,MVC002508
        //f_objParamStore->SetFloat(_T("TS"), fdata);
        f_objParamStore->SetFloat(_T("NS"), fdata);
        //- Patni-PJS/2009Jul23/Modify/IDS#491,MVC002508

    } else {

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_SELECTED_SLICE_THK_TAG_ERROR")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return false;
    }

    l_result = f_series->DBSA_GetValue(&l_vrparam, PQM_SVN_PLN_SELECTED_SLICE_GAP, MRC_FALSE, &l_tagerrlist);

    if (l_result == MRC_DBSA_SUCCESS) {
        //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
        //float fdata;
        float fdata = 0.0f;
        l_vrparam->GetValue(&fdata, 1);
        f_objParamStore->SetFloat(_T("GAP"), fdata);

    } else {

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_SELECTED_SLICE_GAP_TAG_ERROR")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return false;
    }

    l_result = f_series->DBSA_GetValue(&l_vrparam, PQM_SVN_PLN_FOV, MRC_FALSE, &l_tagerrlist);

    if (l_result == MRC_DBSA_SUCCESS) {
        //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
        //float fldata[NUM_2];
        float fldata[NUM_2] = {0.0f};
        l_vrparam->GetValue(fldata, 1);
        l_str.Format(_T("%f,%f"), fldata[0], fldata[1]);
        f_objParamStore->SetVector(_T("FOV"), l_str);

    } else {

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_FOV_TAG_ERROR")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return false;
    }

    l_result = f_series->DBSA_GetValue(&l_vrparam, PQM_SVN_PLN_TRANSMIT_COIL, MRC_FALSE, &l_tagerrlist);

    if (l_result == MRC_DBSA_SUCCESS) {
        //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
        //long tcdata;
        long tcdata = 0l;
        l_vrparam->GetValue(&tcdata, 1);
        f_objParamStore->SetFloat(_T("TC"), (float)tcdata);

    } else {

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_TRANSMIT_COIL_TAG_ERROR")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return false;
    }

    l_result = f_series->DBSA_GetValue(&l_vrparam, PQM_SVN_PLN_TOTAL_TIME, MRC_FALSE, &l_tagerrlist);

    if (l_result == MRC_DBSA_SUCCESS) {
        //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
        //long stime;
        long stime = 0l;
        l_vrparam->GetValue(&stime, 1);
        CString ttime(_T(""));
        ttime.Format(_T("%d"), stime);
        f_objParamStore->SetString(_T("SCANTIME"), ttime);

    } else {

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_TOTAL_TIME_TAG_ERROR")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return false;
    }

    l_result = f_series->DBSA_GetValue(&l_vrparam, PQM_SVN_PLN_RECVR_COIL, MRC_FALSE, &l_tagerrlist);

    if (l_result == MRC_DBSA_SUCCESS) {
        //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
        //long rcdata;
        long rcdata = 0l;
        l_vrparam->GetValue(&rcdata, 1);
        f_objParamStore->SetFloat(_T("RC"), (float)rcdata);

    } else {

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_RECVR_COIL_TAG_ERROR")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return false;
    }

    l_result = f_series->DBSA_GetValue(&l_vrparam, PQM_SVN_PLN_SELECTED_CONTRAST_TE, MRC_FALSE, &l_tagerrlist);

    if (l_result == MRC_DBSA_SUCCESS) {
        double te_data = 0.005;
        l_vrparam->GetValue(&te_data, 1);
        f_objParamStore->SetFloat(_T("TE"), (float)te_data);

    } else {

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_CONTRAST_TE_TAG_ERROR")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return false;
    }

    l_result = f_series->DBSA_GetValue(&l_vrparam, PQM_SVN_PLN_TEMPLATE_SEQ, MRC_FALSE, &l_tagerrlist);

    if (l_result == MRC_DBSA_SUCCESS) {
        LPTSTR dt = _T(""); //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
        int istr = l_vrparam->GetString(&dt, 0);
        l_str = dt;
        f_objParamStore->SetString(_T("SEQ"), l_str);

        DEL_PTR_ARY(tmp);

    } else {

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_TEMPLATE_SEQ_TAG_ERROR")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return false;
    }

    l_result = f_series->DBSA_GetValue(&l_vrparam, PQM_SVN_PLN_STATUS, MRC_FALSE, &l_tagerrlist);

    if (l_result == MRC_DBSA_SUCCESS) {
        //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
        //unsigned short us_planstatus;
        unsigned short us_planstatus = 0;
        l_vrparam->GetValue(us_planstatus, 1);
        f_objParamStore->SetInt(_T("PLANSTATUS"), us_planstatus);

    } else {

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_STATUS_TAG_ERROR")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return false;
    }

    l_result = f_series->DBSA_GetValue(&l_vrparam, PQM_SVN_PLN_HOLD, MRC_FALSE, &l_tagerrlist);

    if (l_result == MRC_DBSA_SUCCESS) {
        //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
        //long fdata;
        long fdata = 0l;
        l_vrparam->GetValue(&fdata, 1);
        f_objParamStore->SetFloat(_T("HOLD"), (float)fdata);

    } else {

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_HOLD_TAG_ERROR")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return false;
    }

    l_result = f_series->DBSA_GetValue(&l_vrparam, PQM_SVN_EXE_PROTOCOL_ACQ_STATUS, MRC_FALSE, &l_tagerrlist);

    if (l_result == MRC_DBSA_SUCCESS) {
        //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
        //long acqdata;
        long acqdata = 0l;
        l_vrparam->GetValue(&acqdata, 1);
        f_objParamStore->SetFloat(_T("ACQSTATUS"), (float)acqdata);
        f_objParamStore->SetFloat(_T("ACQSTATUS2"), (float)acqdata);

    } else {

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_PROTOCOL_ACQ_STATUS_TAG_ERROR")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return false;
    }

    l_result = f_series->DBSA_GetValue(&l_vrparam, PQM_SVN_PLN_DYNAMIC_SCAN, MRC_FALSE, &l_tagerrlist);

    if (l_result == MRC_DBSA_SUCCESS) {
        //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
        //unsigned short dyndata;
        unsigned short dyndata = 0;
        l_vrparam->GetValue(&dyndata, 1);
        f_objParamStore->SetInt(_T("DYN"), dyndata);

    } else {

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_DYNAMIC_SCAN_TAG_ERROR")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return false;
    }

    l_result = f_series->DBSA_GetValue(&l_vrparam, PQM_SVN_PLN_APP_CODE, MRC_FALSE, &l_tagerrlist);

    if (l_result == MRC_DBSA_SUCCESS) {
        //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
        //long appopt_data;
        long appopt_data = 0l;
        l_vrparam->GetValue(&appopt_data, 1);
        f_objParamStore->SetFloat(_T("IMGTECH"), (float)appopt_data);

    } else {

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_APP_CODE_TAG_ERROR")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return false;
    }

    l_result = f_series->DBSA_GetValue(&l_vrparam, PQM_SVN_EXE_COUCH, MRC_FALSE, &l_tagerrlist);

    if (l_result == MRC_DBSA_SUCCESS) {
        //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
        //float couch_data;
        float couch_data = 0.0f;
        l_vrparam->GetValue(&couch_data, 1);
        f_objParamStore->SetFloat(_T("COUCH"), couch_data);
        f_objParamStore->SetFloat(_T("TRSC"), couch_data);

    } else {

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_COUCH_TAG_ERROR")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return false;
    }

    l_result = f_series->DBSA_GetValue(&l_vrparam, PQM_SVN_SEQ_DBDT_VALUE, MRC_FALSE, &l_tagerrlist);

    if (l_result == MRC_DBSA_SUCCESS) {
        //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
        //float ldata;
        float ldata = 0.0f; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
        l_vrparam->GetValue(&ldata, 1);
        f_objParamStore->SetFloat(_T("DBDT"), ldata);

    } else {

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_DBDT_VALUE_TAG_ERROR")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return false;
    }

    l_result = f_series->DBSA_GetValue(&l_vrparam, PQM_SVN_PLN_SCAN_ANATOMY, MRC_FALSE, &l_tagerrlist);

    if (l_result == MRC_DBSA_SUCCESS) {
        //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
        //long sa_data;
        long sa_data = 0l;
        l_vrparam->GetValue(&sa_data, 1);
        f_objParamStore->SetFloat(_T("SA"), (float)sa_data);

    } else {

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_SCAN_ANATOMY_TAG_ERROR")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return false;
    }

    l_result = f_series->DBSA_GetValue(&l_vrparam, PQM_SVN_SEQ_SAR_TOTAL_TIME, MRC_FALSE, &l_tagerrlist);

    if (l_result == MRC_DBSA_SUCCESS) {
        //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
        //float sar_data;
        float sar_data = 0.0f;
        l_vrparam->GetValue(&sar_data, 1);
        //+ Patni-PJS/2009Jul23/Modify/IDS#442,MVC002699
        //f_objParamStore->SetFloat(_T("SAR"), sar_data);
        l_str.Format(_T("%0.3f"), sar_data);
        f_objParamStore->SetString(_T("SAR"), l_str);

    } else {

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_SAR_TOTAL_TIME_TAG_ERROR")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return false;
    }

    l_result = f_series->DBSA_GetValue(&l_vrparam, PQM_SVN_PLN_VIEWING_DIR, MRC_FALSE, &l_tagerrlist);

    if (l_result == MRC_DBSA_SUCCESS) {
        //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
        //long vdir_data;
        long vdir_data = 0l;
        l_vrparam->GetValue(&vdir_data, 1);
        f_objParamStore->SetFloat(_T("VDIR"), (float)vdir_data);

    } else {

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_VIEWING_DIR_TAG_ERROR")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return false;
    }

    l_result = f_series->DBSA_GetValue(&l_vrparam, PQM_SVN_PLN_SELECTED_TAG_THK, MRC_FALSE, &l_tagerrlist);

    if (l_result == MRC_DBSA_SUCCESS) {
        //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
        //float thk_data;
        float thk_data = 0.0f;
        l_vrparam->GetValue(&thk_data, 1);
        f_objParamStore->SetFloat(_T("THK"), thk_data);

    } else {

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_SELECTED_TAG_THK_TAG_ERROR")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return false;
    }

    l_result = f_series->DBSA_GetValue(&l_vrparam, PQM_SVN_PLN_MOTION_SUPPRESSION, MRC_FALSE, &l_tagerrlist);

    if (l_result == MRC_DBSA_SUCCESS) {
        //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
        //unsigned short motion_data;
        unsigned short motion_data = 0;
        l_vrparam->GetValue(&motion_data, 1);
        f_objParamStore->SetInt(_T("MOTION"), motion_data);

    } else {

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_MOTION_SUPRESSION_TAG_ERROR")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return false;
    }

    l_result = f_series->DBSA_GetValue(&l_vrparam, PQM_SVN_PLN_APP_OPTION, MRC_FALSE, &l_tagerrlist);

    if (l_result == MRC_DBSA_SUCCESS) {
        //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
        //long app_data;
        long app_data = 0l;
        l_vrparam->GetValue(&app_data, 1);
        f_objParamStore->SetFloat(_T("APPOPT"), (float)app_data);

    } else {

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_APP_OPTION_TAG_ERROR")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return false;
    }

    l_result = f_series->DBSA_GetValue(&l_vrparam, PQM_SVN_PLN_SLICE_ORIENTATION, MRC_FALSE, &l_tagerrlist);

    if (l_result == MRC_DBSA_SUCCESS) {
        //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
        //long slice_data;
        long slice_data = 0l;
        l_vrparam->GetValue(&slice_data, 1);
        f_objParamStore->SetFloat(_T("PLN"), (float)slice_data);

    } else {

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_SLICE_ORIENTATION_TAG_ERROR")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return false;
    }

    l_result = f_series->DBSA_GetValue(&l_vrparam, PQM_SVN_PLN_PHASE_DIRECTION, MRC_FALSE, &l_tagerrlist);

    if (l_result == MRC_DBSA_SUCCESS) {
        //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
        //long pd_data;
        long pd_data = 0l;
        l_vrparam->GetValue(&pd_data, 1);
        f_objParamStore->SetFloat(_T("PD"), (float)pd_data);

    } else {

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_PHASE_DIRECTION_TAG_ERROR")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return false;
    }

    l_result = f_series->DBSA_GetValue(&l_vrparam, PQM_SVN_PLN_WRAPAROUND_REDUCT, MRC_FALSE, &l_tagerrlist);

    if (l_result == MRC_DBSA_SUCCESS) {
        //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
        //unsigned short wrap_data;
        unsigned short wrap_data = 0;
        l_vrparam->GetValue(&wrap_data, 1);
        f_objParamStore->SetInt(_T("WRAP"), wrap_data);

    } else {

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_WRAPAROUND_REDUCT_TAG_ERROR")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return false;
    }

    l_result = f_series->DBSA_GetValue(&l_vrparam, PQM_SVN_PLN_GRID_TAG, MRC_FALSE, &l_tagerrlist);

    if (l_result == MRC_DBSA_SUCCESS) {
        //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
        //unsigned short gm_data;
        unsigned short gm_data = 0;
        l_vrparam->GetValue(&gm_data, 1);
        //+Patni-Ajay/Added/09Aug2010/TMSC-REDMINE-560
        f_objParamStore->SetInt(_T("GATE"), gm_data);

    } else {

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_GRID_TAG_ERROR")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return false;
    }

    f_objParamStore->SetFloat(_T("FLIP"), (float)VALUE_FLIP);


    f_objParamStore->SetFloat(_T("FLOP"), (float)VALUE_FLOP);


    f_objParamStore->SetFloat(_T("BW"), (float)VALUE_BW);


    f_objParamStore->SetFloat(_T("NRFLG"), 0);


    f_objParamStore->SetFloat(_T("SORS"), 0);


    f_objParamStore->SetFloat(_T("FAT"), 0);


    f_objParamStore->SetFloat(_T("MTC"), 0);


    l_result = f_series->DBSA_GetValue(&l_vrparam, PQM_SVN_PLN_AUTO_POSTPROC_FLG, MRC_FALSE, &l_tagerrlist);

    if (l_result == MRC_DBSA_SUCCESS) {
        //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
        //long fdata;
        long fdata = 0l;
        l_vrparam->GetValue(&fdata, 1);
        f_objParamStore->SetInt(_T("APOST"), fdata);

    } else {

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_AUTO_POSTPROC_FLG_TAG_ERROR")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return false;
    }

    l_result = f_series->DBSA_GetValue(&l_vrparam, PQM_SVN_EXE_AUTO_VOICE, MRC_FALSE, &l_tagerrlist);

    if (l_result == MRC_DBSA_SUCCESS) {
        //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
        //long fdata;
        long fdata = 0l;
        l_vrparam->GetValue(&fdata, 1);
        f_objParamStore->SetInt(_T("AUTOVOICE"), fdata);

    } else {

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_AUTO_VOICETAG_ERROR")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        return false;
    }

    CString offset(_T("4,5,6"));
    f_objParamStore->SetString(_T("SCANOFFSET"), offset);

    return TRUE;
}

/****************************Method Header************************************
//Method Name   :TagErrorProcess()
//Author        :PATNI/JS
//Purpose       :For processing the error return by DBSALib
//*****************************************************************************/
void CPQMStudyMgr::TagErrorProcess(
    CDBSATagErrorList* f_taglist
)const
{

    LPCTSTR FUNC_NAME = _T("CPQMStudyMgr::TagErrorProcess");
    //+Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
    unsigned int l_pathcnt = 0;
    LPCTSTR l_objlevel = NULL , l_loid = NULL;

    CDBSAErrTag* l_errtag = NULL;
    int l_errcode = 0;
    DBSAErrTag_t* l_tagpath = 0;

    for (unsigned int l_errcnt = 0u; l_errcnt < (unsigned int)f_taglist->DBSA_GetErrCount(); l_errcnt ++) {
        //-Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
        l_errtag = NULL;
        l_errtag = f_taglist->DBSA_GetErrTag(l_errcnt);

        l_objlevel = NULL;
        l_objlevel = GetObjectLevelString(l_errtag->DBSA_GetObjLevel());            //ObjLevel
        //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
        //DBSAErrTag_t *l_tagpath;
        l_tagpath = 0;
        l_pathcnt = l_errtag->DBSA_GetTag(&l_tagpath);
        TCHAR l_tagstr[NUM_100] = {0};
        //- Patni-PJS/2009Apr25/Modify/CPP Test tool execution
        GetTagString(l_tagstr, l_tagpath, l_pathcnt);                       //TagPath

        l_loid = NULL;
        l_loid = l_errtag->DBSA_GetLOID();                          //LOID

        if (l_loid == NULL) {
            l_loid = _T("(null)");
        }

        l_errcode = l_errtag->DBSA_GetStatus();
        //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
        //TCHAR l_prval[BUFFER_ERR_PROCESS];
        TCHAR l_prval[BUFFER_ERR_PROCESS] = {0};

        switch (l_errcode) {
            case MRC_DBSA_TAG_NOT_EXIST:
            case MRC_DBSA_DBS_NO_SERIES_INDEX:
                wsprintf(l_prval, _T("[TagErr]Tag->%s, Err->%s, Object->%s, LOID->%s"),
                         l_tagstr, DBSA_GetErrorString(l_errcode), l_objlevel, l_loid);

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_prval);

                break;

            case MRC_DBSA_INVALID_FUNCTION:
            case MRC_DBSA_VR_MAXSIZE_UNKNOWN:
            case MRC_DBSA_DATACOUNT_OVERFLOW:
            case MRC_DBSA_DATACOUNT_UNDERFLOW:
            case MRC_DBSA_DATACOUNT_INVALID:
            case MRC_DBSA_STRINGSIZE_OVERFLOW:
            case MRC_DBSA_OFFSET_OVERFLOW:
            case MRC_DBSA_SETDATA_INVALID:
            case MRC_DBSA_NAME_GROUP_COUNTOVER:
            case MRC_DBSA_TAG_USED_MISTAKE:
            case MRC_DBSA_NO_DATA:
            case MRC_DBSA_DBS_TAGVALUE_READ_FAILED:
            case MRC_DBSA_DBS_TAGVALUE_SET_FAILED:
            case MRC_DBSA_DBS_VRVALUE_READ_FAILED:
            case MRC_DBSA_VRVALUE_SET_MISTAKE:
            case MRC_DBSA_DBS_SQITEM_READ_FAILED:
            case MRC_DBSA_DBS_SQITEM_SET_FAILED:
            case MRC_DBSA_DBS_SQATTR_READ_FAILED:
            case MRC_DBSA_DBS_SQATTR_SET_FAILED:
            case MRC_DBSA_DBS_DATA_READ_FAILED:
            case MRC_DBSA_DBS_DATA_SET_FAILED:
            case MRC_DBSA_DBS_TAG_STRING_ENTRY_FAILED:
            case MRC_DBSA_DBS_TAG_ATTR_ENTRY_FAILED:
            case MRC_DBSA_SQITEM_INDEX_UNKNOWN: {	//Patni-AD/2009Jun02/Added/SU14 //cpp test corrections
                LPCTSTR l_dcmtype = _T(""); //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
                l_dcmtype = GetDCMTypeString(l_errtag->DBSA_GetVR());               //VR

                TCHAR l_vmstr[NUM_100] = {0}; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
                GetVMSizeString(l_vmstr, l_errtag->DBSA_GetVMMin(), l_errtag->DBSA_GetVMMax());     //VM

                DBSALib_Type_e* l_used = NULL; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
                unsigned int l_usedcnt = 0u; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
                l_usedcnt = l_errtag->DBSA_GetUsed(&l_used);
                TCHAR l_usedstr[NUM_100] = {0}; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
                GetUsedString(l_usedstr, l_used, l_usedcnt);                        //Used

                wsprintf(l_prval, _T("[TagErr]Tag->%s, Err->%s, Object->%s, LOID->%s, VR->%s, VM->%s, Used->%s\n"),
                         l_tagstr, DBSA_GetErrorString(l_errcode), l_objlevel, l_loid, l_dcmtype, l_vmstr, l_usedstr);

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_prval);
            } //Patni-AD/2009Jun02/Added/SU14 //cpp test corrections
            break;

            default:
                wsprintf(l_prval, _T("[TagErr]Tag->%s, Err->%s, Object->%s, LOID->%s\n"),
                         l_tagstr, DBSA_GetErrorString(l_errcode), l_objlevel, l_loid);

                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_prval);

                break;
        }
    }

    return;
}

/****************************Method Header************************************
//Method Name   :UpdateAllSeriesUnderProtocol()
//Author        :PATNI/RSG
//Purpose       :Read LOID from all processed node of given protocol
                 and update respective series from MPlus
//*****************************************************************************/
//+Patni-RSG/Added/2009Nov25/MSA0248-0033
void CPQMStudyMgr::UpdateAllSeriesUnderProtocol(
    std::vector<char*> *f_vect_series_loid,
    const int f_scan_index,
    CDBSAVRNode* f_dbsa_node
)
{
    LPCTSTR FUNC_NAME = _T("CPQMStudyMgr::UpdateAllSeriesUnderProtocol");

    if (!f_dbsa_node || !f_vect_series_loid) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Invalid Parameter"));
        return;
    }

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("Trace of CPQMStudyMgr::UpdateAllSeriesUnderProtocol"));

    m_pqm_ptr_dbsa->GetStudy()->GetSeriesLOIDUnderProtocol(f_scan_index, f_vect_series_loid);

    if (f_vect_series_loid->size()) {
        CDBSATagErrorList* l_tagerrlist = NULL;
        DBSAFactory l_factory;
        int l_error_code = -1;
        std::vector<char*>::iterator l_iter_loid = NULL;
        int l_len = 0;

        for (l_iter_loid = f_vect_series_loid->begin(); l_iter_loid != f_vect_series_loid->end(); l_iter_loid++) {
            l_len = strlen((char*) * l_iter_loid) + 1;
            TCHAR* loid = new TCHAR[l_len];
            mbstowcs(loid, (char*)*l_iter_loid, sizeof(TCHAR)*l_len);
            l_error_code = l_factory.DBSA_WriteSeries(&l_tagerrlist, loid, f_dbsa_node);

            if (l_error_code != MRC_DBSA_SUCCESS) {
                if (l_tagerrlist) {
                    TagErrorProcess(l_tagerrlist);
                    DBSA_FreeErrTagList(&l_tagerrlist);
                }

                CString err_msg = _T("");
                err_msg.Format(_T("Unable To Update Series %s DBSA error Code %d"), loid, l_error_code);
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, err_msg);
            }

            DEL_PTR_ARY(loid);
        }
    }
}

/****************************Method Header**************************************************
Method Name   : UpdateAllImageLoidUnderProtocol()
Author        : PATNI/PJS
Purpose       : This function is used to update CDBSAVRNode of all Image loid under protocol.
//******************************************************************************************/
bool CPQMStudyMgr::UpdateAllImageLoidUnderProtocol(
    const int f_scan_index,
    LPCTSTR f_seriesid,
    CDBSAVRNode* f_dbsa_node
)
{
    LPCTSTR				 FUNC_NAME = _T("CPQMStudyMgr::UpdateAllImageLoidUnderProtocol");

    //Patni-KSS/2011Feb18/Added/CPP Test tool Execution
    UNREFERENCED_PARAMETER(f_seriesid);

    DBSAFactory			 l_factory;
    CDBSASeries*		 l_dbsa_series  = NULL;
    CDBSATagErrorList*	 l_errlist = NULL;
    bool				 l_status = false;
    LPCTSTR* 			 l_img_loid = NULL;
    int					 l_img_loid_cnt = 0;
    std::vector<CString> l_vect_str_series_loid;

    std::vector<CString>::iterator l_iter_loid = NULL;
    //Get all the series loid respective to the acq_order/scan index.
    GetMatchedSeriesLOIDForProtocol(&l_vect_str_series_loid, f_scan_index);

    CString l_loid(_T(""));
    int l_result = 0;

    //Iterate each series loid
    for (l_iter_loid = l_vect_str_series_loid.begin(); l_iter_loid != l_vect_str_series_loid.end(); l_iter_loid++) {
        l_loid = (CString) * l_iter_loid;
        //Load series loid object.
        l_result = l_factory.DBSA_LoadSeries(&l_dbsa_series, &l_errlist, All, l_loid, f_dbsa_node);

        if ((MRC_DBSA_SUCCESS != l_result) || (NULL == l_dbsa_series)) {
            if (l_errlist) {
                TagErrorProcess(l_errlist);
                DBSA_FreeErrTagList(&l_errlist);
            }

            CPQMLogMgr::GetInstance()->DisplayError(
                CPqmUtilities::GetMultiLingualString(_T("IDS_FAIL_TO_LOAD_SERIES")),
                _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
                ERR_ID, APP_NAME);
            l_vect_str_series_loid.clear();
            return l_status;
        }

        //Get all the image loid for the respective series loid.
        l_img_loid_cnt = l_dbsa_series->DBSA_GetLowLOIDList(&l_img_loid, MRC_IMAGETYPE_IMAGE);

        for (int l_index = 0; l_index < l_img_loid_cnt; l_index++) {
            //Write the contrast agent tag in Image loid object.
            l_dbsa_series->DBSA_WriteImage(l_img_loid[l_index], f_dbsa_node);
        }

        //Put contrast agent, from the Image loid object, in Local DB
        l_result = l_dbsa_series->DBSA_PutAll(&l_errlist);

        if (MRC_DBSA_SUCCESS != l_result) {
            if (l_errlist) {
                TagErrorProcess(l_errlist);
                DBSA_FreeErrTagList(&l_errlist);
            }

            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("DBSA_LoadStudy failed"));
            l_vect_str_series_loid.clear();
            return l_status;
        }

        DBSA_FreeSeries(l_dbsa_series);
        l_dbsa_series = NULL;
    }

    l_status = true;
    l_vect_str_series_loid.clear();
    return l_status;
}

//+Patni-RSG/2009Dec16/MVC6256
/****************************Method Header************************************
//Method Name   :ValidateSeriesNo()
//Author        :PATNI/RSG
//Purpose       :Read Series Number From Given Series and Validate it with passed
                 series number if match found will return true else false.
// Modified		:
//*****************************************************************************/
bool CPQMStudyMgr::ValidateSeriesNo(
    const int f_series_num,
    LPCTSTR f_series_loid,
    const unsigned short f_bias
)
{
    DBSAFactory l_factory;
    CDBSASeries* l_dbsa_series  = NULL;
    CDBSATagErrorList* l_errlist = NULL;

    int l_result = l_factory.DBSA_LoadSeries(&l_dbsa_series, &l_errlist, PQM, f_series_loid);

    if (l_result == MRC_DBSA_SUCCESS && l_dbsa_series != NULL) {
        CDBSAVR* l_vrparamBias      = NULL ;
        l_result = l_dbsa_series->DBSA_GetValue(&l_vrparamBias, PQM_DCM_ST_SERIES_NUMBER, 1, &l_errlist);

        if (l_result == MRC_DBSA_SUCCESS && l_vrparamBias != NULL) {
            long l_series_num = -1;
            l_result = l_vrparamBias->GetValue(&l_series_num, 1);

            if (l_result == MRC_DBSA_SUCCESS) {
                if (((f_series_num + 1)*f_bias) == l_series_num) {
                    return true;
                }
            }
        }
    }

    return false;
}
//-Patni-RSG/2009Dec16/MVC6256


LPTSTR* CPQMStudyMgr::GetDBSAVRCSStrings(const CString& f_dbsavrcsstr, int& f_vm) const
{

    CString main_str = f_dbsavrcsstr;
    CPtrArray l_ptrarray;
    const CString l_seperator = _T("\\");
    CString l_str = CPqmUtilities::GetNextString(main_str, l_seperator);

    while (!l_str.IsEmpty()) {

        TCHAR* mystr = new TCHAR[l_str.GetLength() + 1];
        memset((void*)mystr, 0, l_str.GetLength() * sizeof(TCHAR));
        memcpy((void*)mystr, (LPCTSTR)l_str, l_str.GetLength() * sizeof(TCHAR));
        mystr[l_str.GetLength()] = _T('\0');

        LPCTSTR str = mystr;
        l_ptrarray.Add((void*)str);

        l_str = CPqmUtilities::GetNextString(main_str, l_seperator);
    }

    const int size = l_ptrarray.GetSize();

    LPTSTR* final_strs = new LPTSTR[size];

    for (int i = 0 ; i < size; i++) {
        final_strs[i] = (LPTSTR)l_ptrarray.GetAt(i);
    }

    f_vm = size ;

    return final_strs;
}


/****************************Method Header************************************
//Method Name   :GetObjectLevelString()
//Author        :PATNI/JS
//Purpose       :For processing the error return by DBSALib
//*****************************************************************************/
LPTSTR CPQMStudyMgr::GetObjectLevelString(
    const DBSALib_ObjType_e f_objtype
)const
{
    switch (f_objtype) {
        case MRC_DBSA_OBJ_NULL:
            return _T("OBJ_NULL");
            break;

        case MRC_DBSA_PATIENT:
            return _T("Patient");
            break;

        case MRC_DBSA_STUDY:
            return _T("Study");
            break;

        case MRC_DBSA_SERIES:
            return _T("Series");
            break;

        case MRC_DBSA_IMAGE:
            return _T("Image");
            break;

        case MRC_DBSA_NONIMAGE:
            return _T("NonImage");
            break;

        case MRC_DBSA_SQTAG:
            return _T("SQ");
            break;

        default:
            return NULL;
            break;
    }
}

/****************************Method Header************************************
//Method Name   :GetDCMTypeString()
//Author        :PATNI/JS
//Purpose       :For processing the error return by DBSALib
//*****************************************************************************/
LPTSTR CPQMStudyMgr::GetDCMTypeString(
    const unsigned int f_dcmtype
)const
{
    switch (f_dcmtype) {
        case DBSA_DCM_VR_NULL:
            return _T("DCM_VR_NULL");
            break;

        case DBSA_DCM_VR_AE:
            return _T("DCM_VR_AE");
            break;

        case DBSA_DCM_VR_AS:
            return _T("DCM_VR_AS");
            break;

        case DBSA_DCM_VR_AT:
            return _T("DCM_VR_AT");
            break;

        case DBSA_DCM_VR_CS:
            return _T("DCM_VR_CS");
            break;

        case DBSA_DCM_VR_DA:
            return _T("DCM_VR_DA");
            break;

        case DBSA_DCM_VR_DS:
            return _T("DCM_VR_DS");
            break;

        case DBSA_DCM_VR_DT:
            return _T("DCM_VR_DT");
            break;

        case DBSA_DCM_VR_FL:
            return _T("DCM_VR_FL");
            break;

        case DBSA_DCM_VR_FD:
            return _T("DCM_VR_FD");
            break;

        case DBSA_DCM_VR_IS:
            return _T("DCM_VR_IS");
            break;

        case DBSA_DCM_VR_LO:
            return _T("DCM_VR_LO");
            break;

        case DBSA_DCM_VR_LT:
            return _T("DCM_VR_LT");
            break;

        case DBSA_DCM_VR_OB:
            return _T("DCM_VR_OB");
            break;

        case DBSA_DCM_VR_OW:
            return _T("DCM_VR_OW");
            break;

        case DBSA_DCM_VR_OF:
            return _T("DCM_VR_OF");
            break;

        case DBSA_DCM_VR_PN:
            return _T("DCM_VR_PN");
            break;

        case DBSA_DCM_VR_SH:
            return _T("DCM_VR_SH");
            break;

        case DBSA_DCM_VR_SL:
            return _T("DCM_VR_SL");
            break;

        case DBSA_DCM_VR_SS:
            return _T("DCM_VR_SS");
            break;

        case DBSA_DCM_VR_ST:
            return _T("DCM_VR_ST");
            break;

        case DBSA_DCM_VR_TM:
            return _T("DCM_VR_TM");
            break;

        case DBSA_DCM_VR_UI:
            return _T("DCM_VR_UI");
            break;

        case DBSA_DCM_VR_UL:
            return _T("DCM_VR_UL");
            break;

        case DBSA_DCM_VR_UN:
            return _T("DCM_VR_UN");
            break;

        case DBSA_DCM_VR_US:
            return _T("DCM_VR_US");
            break;

        case DBSA_DCM_VR_UT:
            return _T("DCM_VR_UT");
            break;

        case DBSA_DCM_VR_STRING:
            return _T("DCM_VR_STRNG");
            break;

        default:
            return NULL;
            break;
    }
}

/****************************Method Header************************************
//Method Name   :GetTagString()
//Author        :PATNI/JS
//Purpose       :
//*****************************************************************************/
void CPQMStudyMgr::GetTagString(
    LPTSTR f_tagstring,
    DBSAErrTag_t* f_tagpath,
    const unsigned int f_pathcnt
)const
{
    if (f_pathcnt > 0) {
        if (f_tagpath[0].ErrTagIndex == NULL) {
            wsprintf(f_tagstring, _T("%08X"), f_tagpath[0].ErrTagValue);

        } else {
            wsprintf(f_tagstring, _T("%08X:%d"), f_tagpath[0].ErrTagValue, *f_tagpath[0].ErrTagIndex);
        }

        for (unsigned int i = 1u; i < f_pathcnt; i++) { //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
            TCHAR wkTagStr[TAG_STRING] = {0}; //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections

            if (f_tagpath[i].ErrTagIndex == NULL) {
                wsprintf(wkTagStr, _T("/%08X"), f_tagpath[i].ErrTagValue);

            } else {
                wsprintf(wkTagStr, _T("/%08X:%d"), f_tagpath[i].ErrTagValue, *f_tagpath[i].ErrTagIndex);
            }

            lstrcat(f_tagstring, wkTagStr);
        }

    } else {
        lstrcpy(f_tagstring, _T(""));
    }

    return;
}

/****************************Method Header************************************
//Method Name   :GetVMSizeString()
//Author        :PATNI/JS
//Purpose       :
//*****************************************************************************/
void CPQMStudyMgr::GetVMSizeString(
    LPTSTR f_vmstring,
    const int f_vmmin,
    const int f_vmmax
)const
{
    //+ Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    /*
    TCHAR l_wkstr1[TAG_STRING];
    TCHAR l_wkstr2[TAG_STRING];
    */
    TCHAR l_wkstr1[TAG_STRING] = {0};
    TCHAR l_wkstr2[TAG_STRING] = {0};

    //- Patni-PJS/2009Apr25/Modify/CPP Test tool execution
    if (f_vmmin == 0) {
        lstrcpy(l_wkstr1, _T("n"));

    } else {
        wsprintf(l_wkstr1, _T("%d"), f_vmmin);
    }

    if (f_vmmax == 0) {
        lstrcpy(l_wkstr2, _T("n"));

    } else {
        wsprintf(l_wkstr2, _T("%d"), f_vmmax);
    }

    if (lstrcmp(l_wkstr1, l_wkstr2) == 0) {
        lstrcpy(f_vmstring, l_wkstr1);

    } else {
        wsprintf(f_vmstring, _T("%s-%s"), l_wkstr1, l_wkstr2);
    }

    return;
}

/****************************Method Header************************************
//Method Name   :GetUsedString()
//Author        :PATNI/JS
//Purpose       :
//*****************************************************************************/
void CPQMStudyMgr::GetUsedString(
    LPTSTR f_usedstring,
    DBSALib_Type_e* f_type,
    const unsigned int f_usedcnt
)const
{
    //+Patni-MJC/2009Aug17/Modified/cpp test corrections
    if (f_type == NULL) {
        return;
    }

    //-Patni-MJC/2009Aug17/Modified/cpp test corrections

    if (f_usedcnt > 0) {
        if (f_type[0] == PQM) {
            lstrcpy(f_usedstring, _T("PQM"));

        } else if (f_type[0] == NSDE) {
            lstrcpy(f_usedstring, _T("NSDE"));

        } else if (f_type[0] == Recon) {
            lstrcpy(f_usedstring, _T("Recon"));

        } else if (f_type[0] == AcqMan) {
            lstrcpy(f_usedstring, _T("AcqMan"));

        } else {
            lstrcpy(f_usedstring, _T("UNKNOWN"));
        }

        for (unsigned int i = 1u; i < f_usedcnt; i++) { //Patni-AD/2009Jun02/Modified/SU14 //cpp test corrections
            if (f_type[i] == PQM) {
                lstrcat(f_usedstring, _T("/PQM"));

            } else if (f_type[i] == NSDE) {
                lstrcat(f_usedstring, _T("/NSDE"));

            } else if (f_type[i] == Recon) {
                lstrcat(f_usedstring, _T("/Recon"));

            } else if (f_type[i] == AcqMan) {
                lstrcat(f_usedstring, _T("/AcqMan"));

            } else {
                lstrcpy(f_usedstring, _T("/UNKNOWN"));
            }
        }

    } else {
        lstrcpy(f_usedstring, _T(""));
    }

    return;
}

//+Patni-PJS/2010Aug24/Added/TMSC-REDMINE-599
/****************************Method Header**********************************************************
Method Name   : GetMatchedSeriesLOIDForProtocol()
Author        : PATNI/PJS
Purpose       : This function is used to get the matched series loid for the protocol from local DB.
//**************************************************************************************************/
void CPQMStudyMgr::GetMatchedSeriesLOIDForProtocol(
    std::vector<CString> *f_vect_str_series_loid, const int f_scan_index
)
{
    LPCTSTR				FUNC_NAME = _T("CPQMStudyMgr::GetMatchedSeriesLOIDForProtocol");
    //Get the current study file path
    CString l_study_loid = m_pqm_ptr_dbsa->GetStudy()->GetStudyLoid();

    if (l_study_loid.IsEmpty()) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("l_study_loid value is empty"));
        return;
    }

    //Format study loid corresponding to the acq order.
    CString l_study_path = m_pqm_ptr_dbsa->GetStudy()->GetStudyPath();
    CString l_acq_order(_T(""));
    l_acq_order.Format(_T("%d"), f_scan_index);
    l_study_path += _T(":/1.") + l_acq_order;
    //To fetch 0x00291054 tag from Local DB
    CDBSAVRNode*		l_input_tag = new CDBSAVRNode();
    CDBSAVR*			l_vrparam = new CDBSAVRLO(PQM_DCM_MR_PROTOCOL_NODE_PATH);
    l_input_tag->addLeaf(l_vrparam);

    DBSAFactory			l_factory;
    CDBSAStudy*			l_dbsa_study = NULL;
    CDBSATagErrorList*	l_errlist = NULL;
    int l_result = l_factory.DBSA_LoadStudy(&l_dbsa_study, &l_errlist, All, l_study_loid);

    if (MRC_DBSA_SUCCESS != l_result || (NULL == l_dbsa_study)) {
        if (l_errlist) {
            TagErrorProcess(l_errlist);
            DBSA_FreeErrTagList(&l_errlist);
        }

        CPQMLogMgr::GetInstance()->DisplayError(
            CPqmUtilities::GetMultiLingualString(_T("IDS_FAILED_TO_LOAD_STUDY")),
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);

        DEL_PTR(l_input_tag);

        return;
    }

    //Get all series loid for respective study loid.
    int					sz = 0;
    LPCTSTR*			l_series_loid = NULL;
    LPTSTR*				l_node_path = NULL;
    const int l_study_loid_cnt = l_dbsa_study->DBSA_GetLowLOIDList(&l_series_loid);
    CString temp_string(_T(""));

    CDBSASeries*		l_dbsa_series  = NULL;

    for (int l_index = 0; l_index < l_study_loid_cnt; l_index++) {
        l_result = l_factory.DBSA_LoadSeries(&l_dbsa_series, &l_errlist, All, l_series_loid[l_index], l_input_tag);

        if (MRC_DBSA_SUCCESS != l_result || (NULL == l_dbsa_series)) {
            if (l_errlist) {
                TagErrorProcess(l_errlist);
                DBSA_FreeErrTagList(&l_errlist);
            }

            CPQMLogMgr::GetInstance()->DisplayError(
                CPqmUtilities::GetMultiLingualString(_T("IDS_FAIL_TO_LOAD_SERIES")),
                _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
                ERR_ID, APP_NAME);
            break;
        }

        CDBSAVRLeaf* series_num_leaf = l_dbsa_series->FindLeaf(PQM_DCM_MR_PROTOCOL_NODE_PATH);

        if (NULL == series_num_leaf) {
            //Trace log
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("series_num_leaf is NULL"));
            break;
        }

        sz = ((CDBSAVR*)series_num_leaf)->GetSize();
        l_node_path = new LPTSTR[sz];
        ((CDBSAVR*)series_num_leaf)->GetValue(l_node_path, sz);
        temp_string.Empty();
        temp_string = l_node_path[0];

        //Store matched series loid in vector.
        if (!temp_string.CompareNoCase(l_study_path)) {
            temp_string = l_series_loid[l_index];
            f_vect_str_series_loid->push_back(temp_string);
        }

        DBSA_FreeSeries(l_dbsa_series);
        l_dbsa_series = NULL;

        DEL_PTR_ARY(l_node_path);
    }

    DBSA_FreeStudy(l_dbsa_study);
    l_dbsa_study = NULL;

    DEL_PTR(l_input_tag);

}
//-Patni-PJS/2010Aug24/Added/TMSC-REDMINE-599


/****************************Method Header************************************
//Method Name   :GetDicomStringforAnatomy()
//Author        :PATNI/Sribanta
//Purpose       :To gt dicom string value for selected anatomy
//*****************************************************************************/
void  CPQMStudyMgr::GetDicomStringforAnatomy(
    CPqmProtocol* f_pqm_protocol,
    CString& f_dicom_string
) const
{

    CString l_body_part = f_pqm_protocol->GetPqmPrMode()->GetBodyPart();
    f_dicom_string = _T("");

    if (!l_body_part.IsEmpty()) {
        f_dicom_string = l_body_part;

    } else {

        const int l_int = f_pqm_protocol->GetPqmPrMode()->GetAnatomy();

        if (l_int > 0) {
            char value[NUM_16] = {'\0'};

            switch (l_int) {
                case VFC_ANATOMY_HEAD	 :
                    strcpy(value, DCM_ANATOMY_HEAD)	   ;
                    break;

                case VFC_ANATOMY_ABDOMEN :
                    strcpy(value, DCM_ANATOMY_ABDOMEN) ;
                    break;

                case VFC_ANATOMY_CHEST	 :
                    strcpy(value, DCM_ANATOMY_CHEST)   ;
                    break;

                case VFC_ANATOMY_PELVIS  :
                    strcpy(value, DCM_ANATOMY_PELVIS)  ;
                    break;

                case VFC_ANATOMY_TL_SPINE:
                    strcpy(value, DCM_ANATOMY_TL_SPINE);
                    break;

                case VFC_ANATOMY_SHOULDER:
                    strcpy(value, DCM_ANATOMY_SHOULDER);
                    break;

                case VFC_ANATOMY_C_SPINE :
                    strcpy(value, DCM_ANATOMY_C_SPINE) ;
                    break;

                case VFC_ANATOMY_EXTREMITY:
                    strcpy(value, DCM_ANATOMY_EXTREMITY);
                    break;

                case VFC_ANATOMY_ANKLE	 :
                    strcpy(value, DCM_ANATOMY_ANKLE)   ;
                    break;

                case VFC_ANATOMY_HAND	 :
                    strcpy(value, DCM_ANATOMY_HAND)    ;
                    break;

                default:
                    break;
            }

            f_dicom_string = CString(value);
        }

    }

}



/****************************Method Header************************************
//Method Name   :GetSeriesBiasVal
//Author        :PATNI/RSG
//Purpose       :This method Get Value from study LOID from MPLusDB if not found
				 Then search in config and set in MPLUSDB if not found in Config
				 it will set default value as 1000
//*****************************************************************************/
bool CPQMStudyMgr::SetSeriesBiasVal(
)
{
    bool res = false;

    if (m_dbsa_study) {
        unsigned short l_seriesBias = 0;
        CDBSATagErrorList* l_errlist = NULL;
        CDBSAVR* l_vrparamBias = NULL;
        int l_result = m_dbsa_study->DBSA_GetValue(&l_vrparamBias, PQM_DCM_MR_SERIES_NUMBER_BIAS, MRC_TRUE, &l_errlist);

        if (l_result == MRC_DBSA_SUCCESS) {
            if (l_vrparamBias != NULL) {
                l_vrparamBias->GetValue(&l_seriesBias, 1);
            }
        }

        if (l_seriesBias == 0) {
            //Get the value from ini file
            //Patni-ARD/2010Mar3/Modified/DeFT# IR-100 remove IPCConfig.ini
            //l_seriesBias = CPQMConfig::GetInstance()->GetSeriesBiasValue();
            l_seriesBias = m_series_bias;

            if (l_seriesBias <= 0) {
                l_seriesBias = 1000;
            }

            //Add Series Bias tag 00111006 to study
            if (!l_vrparamBias) {
                l_vrparamBias = new CDBSAVRUS(PQM_DCM_MR_SERIES_NUMBER_BIAS, DCM_VR_LEN_US);
                m_dbsa_study->addLeaf(l_vrparamBias);
            }

            unsigned short temp = l_seriesBias;
            int status = l_vrparamBias->SetValue(&temp, 1);
            l_result = m_dbsa_study->DBSA_Put(&l_errlist);

            if (!l_result) {
                res = true;
            }
        }
    }

    return res;
}

/****************************Method Header************************************
//Method Name   :AddWFDACAPPValue
//Author        :PATNI/PJS
//Purpose       :This method is used to add WFDA Parameters during duplicating
				 duplicating WFDA protocol.
//*****************************************************************************/
BOOL CPQMStudyMgr::AddWFDACAPPValue(
    const CString l_series_loid_prev_prot,
    const CString l_series_loid_curr_prot
)
{
    LPCTSTR				FUNC_NAME = _T("CPQMStudyMgr::AddWFDACAPPValue");

    if (l_series_loid_prev_prot.IsEmpty() && l_series_loid_curr_prot.IsEmpty())	{
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Invalid l_series_loid_prev_prot or l_series_loid_curr_prot parameter"));
        return FALSE;
    }

    BOOL				l_status = TRUE;
    CDBSAVRNode*		l_input_tag = NULL;
    DBSAFactory			l_factory;
    CDBSASeries*		l_dbsa_series  = NULL;
    CDBSATagErrorList*	l_errlist = NULL;
    int					l_result = 0;
    CDBSAVR*			 l_vparam_init_param = NULL, *l_vparam_analysis_param = NULL, *l_vparam_inscan_flag = NULL, *l_vparam_guid_val = NULL, *l_vparam_calc_mode = NULL;

    //To fetch tag from Local DB

    l_input_tag	= new CDBSAVRNode();
    l_vparam_init_param = new CDBSAVROB(DCM_MR_WFDA_CAPP_INITIAL_PARAMETER);
    l_vparam_analysis_param = new CDBSAVROB(DCM_MR_WFDA_CAPP_ANALYSIS_PARAMETERS);
    l_vparam_guid_val = new CDBSAVRLO(DCM_MR_WFDA_CAPP_PREFERENCE_GUID);
    l_vparam_inscan_flag = new CDBSAVRUS(DCM_MR_WFDA_CAPP_INSCAN_FLAG);
    l_vparam_calc_mode = new CDBSAVRLO(DCM_MR_WFDA_CAPP_INSCAN_CALC_MODE);

    l_input_tag->addLeaf(l_vparam_init_param);
    l_input_tag->addLeaf(l_vparam_analysis_param);
    l_input_tag->addLeaf(l_vparam_guid_val);
    l_input_tag->addLeaf(l_vparam_inscan_flag);
    l_input_tag->addLeaf(l_vparam_calc_mode);

    l_result = l_factory.DBSA_LoadSeries(&l_dbsa_series, &l_errlist, All, l_series_loid_prev_prot, l_input_tag);

    if (MRC_DBSA_SUCCESS != l_result || (NULL == l_dbsa_series)) {
        if (l_errlist) {
            TagErrorProcess(l_errlist);
            DBSA_FreeErrTagList(&l_errlist);
        }

        CString temp_string = CPqmUtilities::GetMultiLingualString(_T("IDS_FAIL_TO_LOAD_SERIES"));
        CPQMLogMgr::GetInstance()->DisplayError(
            temp_string,
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        DEL_PTR(l_input_tag);
        return FALSE;
    }

    CDBSAVR* l_vrparamBias  = NULL ;
    CDBSAVRNode* l_input_tag_write = new CDBSAVRNode();
    CDBSAVR* l_vparam_init_param_write = new CDBSAVROB(DCM_MR_WFDA_CAPP_INITIAL_PARAMETER);
    l_result = l_dbsa_series->DBSA_GetValue(&l_vrparamBias, DCM_MR_WFDA_CAPP_INITIAL_PARAMETER, 1, &l_errlist);

    if (l_result == MRC_DBSA_SUCCESS && l_vrparamBias != NULL) {
        BYTE* ob_data = new BYTE[l_vrparamBias->GetSize() + 1];
        memset(ob_data, 0, l_vrparamBias->GetSize() + 1);
        l_vrparamBias->GetValue(ob_data, l_vrparamBias->GetSize());
        l_vparam_init_param_write->SetValue(ob_data, l_vrparamBias->GetSize());
        DEL_PTR_ARY(ob_data);

    } else {
        if (l_errlist) {
            TagErrorProcess(l_errlist);
            DBSA_FreeErrTagList(&l_errlist);
        }

        CString l_result_string = DBSA_GetErrorString(l_result);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  l_result_string);
        l_status = FALSE;
    }

    CDBSAVR* l_vparam_analysis_param_write = new CDBSAVROB(DCM_MR_WFDA_CAPP_ANALYSIS_PARAMETERS);
    l_result = l_dbsa_series->DBSA_GetValue(&l_vrparamBias, DCM_MR_WFDA_CAPP_ANALYSIS_PARAMETERS, 1, &l_errlist);

    if (l_result == MRC_DBSA_SUCCESS && l_vrparamBias != NULL) {
        BYTE* ob_data_analy_param = new BYTE[l_vrparamBias->GetSize() + 1];
        memset(ob_data_analy_param, 0, l_vrparamBias->GetSize() + 1);
        l_vrparamBias->GetValue(ob_data_analy_param, l_vrparamBias->GetSize());
        l_vparam_analysis_param_write->SetValue(ob_data_analy_param, l_vrparamBias->GetSize());
        DEL_PTR_ARY(ob_data_analy_param);

    } else {
        if (l_errlist) {
            TagErrorProcess(l_errlist);
            DBSA_FreeErrTagList(&l_errlist);
        }

        CString l_result_string = DBSA_GetErrorString(l_result);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  l_result_string);
        l_status = FALSE;
    }

    CDBSAVR* l_vparam_inscan_flag_write = new CDBSAVRUS(DCM_MR_WFDA_CAPP_INSCAN_FLAG);
    l_result = l_dbsa_series->DBSA_GetValue(&l_vrparamBias, DCM_MR_WFDA_CAPP_INSCAN_FLAG, 1, &l_errlist);

    if (l_result == MRC_DBSA_SUCCESS && l_vrparamBias != NULL) {
        BYTE* ob_data_inscan_flag = new BYTE[l_vrparamBias->GetSize() + 1];
        memset(ob_data_inscan_flag, 0, l_vrparamBias->GetSize() + 1);
        l_vrparamBias->GetValue(ob_data_inscan_flag, l_vrparamBias->GetSize());
        l_vparam_inscan_flag_write->SetValue(ob_data_inscan_flag, l_vrparamBias->GetSize());
        DEL_PTR_ARY(ob_data_inscan_flag);

    } else {
        if (l_errlist) {
            TagErrorProcess(l_errlist);
            DBSA_FreeErrTagList(&l_errlist);
        }

        CString l_result_string = DBSA_GetErrorString(l_result);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  l_result_string);
        l_status = FALSE;
    }

    CDBSAVRLeaf* l_vparam_inscan_calc_mode = ((CDBSAVRNode*)l_dbsa_series)->FindLeaf(DCM_MR_WFDA_CAPP_INSCAN_CALC_MODE);
    CDBSAVR* l_vparam_inscan_calc_mode_write = new CDBSAVRLO(DCM_MR_WFDA_CAPP_INSCAN_CALC_MODE);

    if (l_vparam_inscan_calc_mode_write) {
        LPTSTR l_inscan_calc_mode = _T("");
        l_result = ((CDBSAVR*)l_vparam_inscan_calc_mode)->GetValue(&l_inscan_calc_mode, ((CDBSAVR*)l_vparam_inscan_calc_mode)->GetSize());

        if (l_result != MRC_DBSA_SUCCESS) {
            CString l_result_string = DBSA_GetErrorString(l_result);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      l_result_string);
            l_status = FALSE;

        } else {
            l_vparam_inscan_calc_mode_write->SetValue(((LPCTSTR*)(&l_inscan_calc_mode)), 1);
        }
    }

    CDBSAVRLeaf* l_vparam_inscan_guid = ((CDBSAVRNode*)l_dbsa_series)->FindLeaf(DCM_MR_WFDA_CAPP_PREFERENCE_GUID);
    CDBSAVR* l_vparam_guid_write = new CDBSAVRLO(DCM_MR_WFDA_CAPP_PREFERENCE_GUID);

    if (l_vparam_inscan_guid) {
        LPTSTR l_inscan_guid = _T("");
        l_result = ((CDBSAVR*)l_vparam_inscan_guid)->GetValue(&l_inscan_guid, ((CDBSAVR*)l_vparam_inscan_guid)->GetSize());

        if (l_result != MRC_DBSA_SUCCESS) {
            CString l_result_string = DBSA_GetErrorString(l_result);
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      l_result_string);
            l_status = FALSE;

        } else {
            l_vparam_guid_write->SetValue(((LPCTSTR*)(&l_inscan_guid)), 1);
        }
    }

    l_input_tag_write->addLeaf(l_vparam_init_param_write);
    l_input_tag_write->addLeaf(l_vparam_analysis_param_write);
    l_input_tag_write->addLeaf(l_vparam_inscan_flag_write);
    l_input_tag_write->addLeaf(l_vparam_guid_write);
    l_input_tag_write->addLeaf(l_vparam_inscan_calc_mode_write);

    l_result = l_factory.DBSA_WriteSeries(&l_errlist, l_series_loid_curr_prot, l_input_tag_write);

    if (MRC_DBSA_SUCCESS != l_result) {
        if (l_errlist) {
            TagErrorProcess(l_errlist);
            DBSA_FreeErrTagList(&l_errlist);
        }

        CString temp_string = CPqmUtilities::GetMultiLingualString(_T("IDS_FAILED_TO_WRITE_DATA_IN_DB"));
        CPQMLogMgr::GetInstance()->DisplayError(
            temp_string,
            _T("PQMStudyMgr.cpp"), __LINE__, ERROR_LOG_CRUCIAL1,
            ERR_ID, APP_NAME);
        l_status = FALSE;
    }

    DEL_PTR(l_input_tag);
    DEL_PTR(l_input_tag_write);
    return l_status;
}

BOOL CPQMStudyMgr::WriteCharSetAtStudyLevel(
) const
{

    LPCTSTR	FUNC_NAME = _T("CPQMStudyMgr::WriteCharSetAtStudyLevel");
    CString l_charset = CPqmUtilities::GetCharset();

    int l_vm = -1;
    LPCTSTR* l_basestr = (LPCTSTR*) GetDBSAVRCSStrings(l_charset, l_vm);

    if ((NULL == l_basestr) || (l_vm < 0)) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("GetDBSAVRCSStrings Failed"));
    }

    CDBSAVR* l_vrparam = m_dbsa_study->GetValue(PQM_DCM_MR_STUDY_CHARSET);

    if (NULL == l_vrparam) {
        l_vrparam = new CDBSAVRCS(PQM_DCM_MR_STUDY_CHARSET);
        m_dbsa_study->addLeaf(l_vrparam);
    }

    BOOL l_result = l_vrparam->SetValue(l_basestr, l_vm);

    for (int i = 0; i < l_vm; i++) {
        LPTSTR l_str = (LPTSTR) l_basestr[i] ;
        DEL_PTR_ARY(l_str);
    }

    LPTSTR l_str_arry = (LPTSTR)l_basestr;
    DEL_PTR_ARY(l_str_arry);

    CDBSATagErrorList* l_tagerrlist = NULL;

    if (l_result != MRC_DBSA_SUCCESS) {
        if (l_tagerrlist) {
            TagErrorProcess(l_tagerrlist);
            DBSA_FreeErrTagList(&l_tagerrlist);
        }

        return FALSE;
    }

    l_result =  m_dbsa_study->DBSA_Put(&l_tagerrlist);

    if (l_result != MRC_DBSA_SUCCESS) {
        return FALSE;
    }

    return TRUE;
}

//*****************************************************************************
//Method Name   : SetSeriesForDBSAVRFL()
//Author        : iGATE
//Purpose       : Writes the RFL tag values to Local DB.
//****************************************************************************
BOOL CPQMStudyMgr::SetSeriesForDBSAVRFL(CDBSAVRNode* f_vr_node,
                                        CPqmProtocol* f_protocol,
                                        const DBSATag_t f_tag,
                                        const f32vec3_t& f_value
                                       )const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetSeriesForDBSAVRFL");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CPQMStudyMgr::SetSeriesForDBSAVRFL()"));

    if (NULL == f_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_protocol is NULL"));
        return FALSE;
    }

    CDBSAVR* l_vrparam = NULL;

    try {
        l_vrparam = new CDBSAVRFL3(f_tag);

    } catch (CMemoryException* e) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("EXCEPTION in new CDBSAVRNode"));
        e->Delete();
        throw;
    }

    f_vr_node->addLeaf(l_vrparam);

    float l_tagvalue[] = {f_value.x, f_value.y, f_value.z};

    if (MRC_DBSA_SUCCESS != l_vrparam->SetValue(l_tagvalue, 3)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed to SetValue"));
        return FALSE;
    }

    return TRUE;
}

//*****************************************************************************
//Method Name   : SetSeriesForDBSAVRFL()
//Author        : iGATE\Jayanthi
//Purpose       : Writes the RFL tag values to Local DB.
//****************************************************************************
BOOL CPQMStudyMgr::SetSeriesForDBSAVRTM(CDBSAVRNode* f_vr_node,
                                        CPqmProtocol* f_protocol,
                                        const DBSATag_t f_tag,
                                        /*const*/ DateTime_t& f_value
                                       )const
{
    LPCTSTR FUNC_NAME = _T("CPQMStudy::SetSeriesForDBSAVRFL");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("CPQMStudyMgr::SetSeriesForDBSAVRFL()"));

    if (NULL == f_protocol) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("f_protocol is NULL"));
        return FALSE;
    }

    CDBSAVR* l_vrparam = NULL;

    try {
        l_vrparam = new CDBSAVRTM(f_tag);

    } catch (CMemoryException* e) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("EXCEPTION in new CDBSAVRNode"));
        e->Delete();
        throw;
    }

    f_vr_node->addLeaf(l_vrparam);

    if (MRC_DBSA_SUCCESS != l_vrparam->SetValue(f_value, 0)) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Failed to SetValue"));
        return FALSE;
    }

    return TRUE;
}

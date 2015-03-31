//REDMINE_782_CPP_Test_Tool
//TMSC-REDMINE-783
//TMSC-REDMINE-782
//TMSC-REDMINE-781
//TMSC-REDMINE-780
//TMSC-REDMINE-778
//TMSC-REDMINE-777
//redmine-779
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMSequence.cpp
 *  Overview: Implementation of CPQMSequence class.
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
#include "stdafx.h"
#include "PQMSequence.h"
#include <tami/vfStudy/vf_appcodes.h>
#include "PQMView.h"

extern Data_decl_list  g_sequence_param_name_list;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


extern CPQMView* g_MainView;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//****************************Method Header********************
//Method Name   : CSequence::CSequence()
//Author        : PATNI/DJ
//Purpose       : Default Constructor.
//*************************************************************

CSequence::CSequence():
    m_movecouch_method(NOMOVE),
    m_PrevMovecouch_Method(NOMOVE),				//Patni-Abhishek/2010Dec14/Added/IR-180
    m_auto_voice_option(NONE_NONE),
    m_Prev_Auto_Voice_Option(NONE_NONE),		//Patni-Abhishek/2010Dec14/Added/IR-180

    m_prescan_aut_voi_optn(NONE),
    m_child_thread_status(CT_NONE),
    m_cpascom_sequence(NULL),
    m_auto_transfer(0),
    m_exe_auto_voice(0),
    m_scan_time_value(_T("")),
    m_canchange_modeoptionbreathhold(true),
    m_wse_editingenum(WSE_None),
    m_cds_flag(0),
    m_amb_flag(0),
    m_mrs_eddy_axis(0),
    m_delay_time(_T("")),
    m_amb_mode_flag(0),
    m_sar_info(NULL)
{

    m_pr_mode.m_anatomy = 0;
    m_pr_mode.m_rcv_coil_id = 0;
    m_pr_mode.m_recv_coil_label = _T("");
    m_pr_mode.m_couch_pos = 0.0f;
    m_pr_mode.m_home_pos = 0.0f;
    m_pr_mode.m_couch_offset = 0.0f;
    m_pr_mode.m_contrast = 0;
    m_pr_mode.m_str_contrast = _T("");
    m_pr_mode.m_scan_number = 0;
    m_pr_mode.m_view_dir = 0;
    //+Patni-Abhishek/2010Dec14/Added/IR-180
    m_pr_mode.m_Prev_Contrast = 0;
    m_pr_mode.m_Prev_Str_Contrast = _T("");
    //-Patni-Abhishek/2010Dec14/Added/IR-180

    // Girija TRSC 10-May-07
    m_pr_mode.m_str_trsc = _T("----");
    // -- Girija TRSC 10-May-07


    //+Patni-HAR/2010Feb16/Commented/MSA0251-00166
    // Now this flag is controlled by Initialize param
    //m_canchange_modeoptionbreathhold = true ;



    //Patni-HAR/2010Jan18/Added/DeFT#MVC006364

    //+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
    SetASCompassFlag(false);
    SetAMBFlag(false);
    SetAMBModeFlag(VFC_AMB_MODE_NONE);
    //-Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement

}


//****************************Method Header********************
//Method Name   : CSequence::CSequence(CPASCOMSequence *f_cpascom_sequence)
//Author        : PATNI/DJ
//Purpose       : Parameterized constructor.
//*************************************************************

CSequence::CSequence(
    CPASCOMSequence* f_cpascom_sequence
):	m_movecouch_method(NOMOVE),
    m_PrevMovecouch_Method(NOMOVE),				//Patni-Abhishek/2010Dec14/Added/IR-180
    m_auto_voice_option(NONE_NONE),
    m_Prev_Auto_Voice_Option(NONE_NONE),		//Patni-Abhishek/2010Dec14/Added/IR-180

    m_prescan_aut_voi_optn(NONE),
    m_child_thread_status(CT_NONE),
    m_cpascom_sequence(new CPASCOMSequence(*f_cpascom_sequence)),
    m_auto_transfer(0),
    m_exe_auto_voice(0),
    m_scan_time_value(_T("")),				//Patni-Sudhir/2011Mar03/Review Comment
    m_canchange_modeoptionbreathhold(true),
    m_wse_editingenum(WSE_None),
    m_cds_flag(0),
    m_amb_flag(0),
    m_mrs_eddy_axis(0),

    m_amb_mode_flag(0),
    m_sar_info(NULL)

{

    m_pr_mode.m_anatomy = 0;
    m_pr_mode.m_rcv_coil_id = 0;
    m_pr_mode.m_recv_coil_label = _T("");
    m_pr_mode.m_couch_pos = 0.0f;
    m_pr_mode.m_home_pos = 0.0f;
    m_pr_mode.m_couch_offset = 0.0f;
    m_pr_mode.m_contrast = 0;
    m_pr_mode.m_str_contrast = _T("");
    m_pr_mode.m_scan_number = 0;
    m_pr_mode.m_view_dir = 0;

    //+Patni-Abhishek/2010Dec14/Added/IR-180
    m_pr_mode.m_Prev_Contrast = 0;
    m_pr_mode.m_Prev_Str_Contrast = _T("");
    //-Patni-Abhishek/2010Dec14/Added/IR-180

    //+Patni-PP/2009Jun01/Modified/SU14 CPP test initialization corrections

    //Added by Hemant On 12/20/2008 1:18:44 PM
    SetWSEEditingEnum(WSE_None);
    SetAMBEditingEnum(AMB_NONE);

    InitializeParam();
    //Added by Hemant On 7/12/2008 2:08:41 PM
    //m_auto_voice_option = NONE_NONE;

    EncodeAutoVoiceNumberToStringAndsetAutoVoiceOption();

}
//****************************Method Header********************
//Method Name   : CSequence::CSequence(const CSequence& f_sequence)
//Author        : PATNI/DJ
//Purpose       : constructor for the class.
//*************************************************************
CSequence::CSequence(const CSequence* sequence):	//Patni-Sudhir/2011Mar03/Modified/Review Comment
    m_movecouch_method(NOMOVE),
    m_PrevMovecouch_Method(NOMOVE),
    m_auto_voice_option(NONE_NONE),
    m_Prev_Auto_Voice_Option(NONE_NONE),			//Patni-Abhishek/2010Dec14/Added/IR-180

    m_prescan_aut_voi_optn(NONE),

    m_child_thread_status(CT_NONE),
    m_cpascom_sequence(new CPASCOMSequence(*(sequence->m_cpascom_sequence))),
    m_auto_transfer(sequence->m_auto_transfer),
    m_exe_auto_voice(sequence->m_exe_auto_voice),
    m_scan_time_value(sequence->m_scan_time_value),
    m_canchange_modeoptionbreathhold(sequence->m_canchange_modeoptionbreathhold),
    m_wse_editingenum(WSE_None),
    m_cds_flag(0),
    m_amb_flag(0),
    m_mrs_eddy_axis(0),
    m_delay_time(sequence->m_delay_time),
    m_amb_mode_flag(0),
    m_sar_info(NULL)
{

    LPCTSTR FUNC_NAME = _T("CSequence::CSequence");



    m_pr_mode.m_anatomy = sequence->m_pr_mode.m_anatomy;
    m_pr_mode.m_rcv_coil_id = sequence->m_pr_mode.m_rcv_coil_id;
    m_pr_mode.m_recv_coil_label = sequence->m_pr_mode.m_recv_coil_label;
    m_pr_mode.m_couch_pos = sequence->m_pr_mode.m_couch_pos;
    m_pr_mode.m_home_pos = sequence->m_pr_mode.m_home_pos;
    m_pr_mode.m_couch_offset = sequence->m_pr_mode.m_couch_offset;
    m_pr_mode.m_contrast = sequence->m_pr_mode.m_contrast;
    m_pr_mode.m_str_contrast = sequence->m_pr_mode.m_str_contrast;
    m_pr_mode.m_scan_number = sequence->m_pr_mode.m_scan_number;
    m_pr_mode.m_view_dir = sequence->m_pr_mode.m_view_dir;
    //+Patni-Abhishek/2010Dec14/Added/IR-180
    m_pr_mode.m_Prev_Contrast = sequence->m_pr_mode.m_Prev_Contrast;
    m_pr_mode.m_Prev_Str_Contrast = sequence->m_pr_mode.m_Prev_Str_Contrast;
    //-Patni-Abhishek/2010Dec14/Added/IR-180
    m_pr_mode.m_str_trsc = sequence->m_pr_mode.m_str_trsc;

    //Added by Hemant On 11/19/2008 10:17:12 AM
    m_pr_mode.m_str_scan_anatomy = sequence->m_pr_mode.m_str_scan_anatomy;
    CPASParamStore* pparam = NULL;

    //+Patni-AD/2009Aug25/Modified/SU15 //CPP test corrections
    if (m_cpascom_sequence != NULL) {
        pparam = m_cpascom_sequence->GetSequenceParam();

    } else {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_cpascom_sequence pointer is NULL"));

        return;

    }

    //-Patni-AD/2009Aug25/Modified/SU15 //CPP test corrections
    CString str;
    str.Format(L" = %dx", pparam);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str);


    // To get patient orientation
    CString str_pat_ori = pparam->GetString(_T("PORI"));

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, L"After GetString");

    CString f_name(_T(""));
    CString f_key(_T(""));
    CString f_value(_T(""));
    CPQMUtility::GetInstance()->ParseLine(str_pat_ori, f_name, f_key, f_value);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, L"After ParseLine");

    //I8_ITC_44_saurabh_Gupta
    m_pr_mode.m_pat_ori.end_in = sequence->m_pr_mode.m_pat_ori.end_in;
    m_pr_mode.m_pat_ori.side_up = sequence->m_pr_mode.m_pat_ori.side_up;

    //I8_ITC_44_saurabh_Gupta
    //	m_pr_mode.m_pat_ori.end_in = _wtoi(f_key);
    //    m_pr_mode.m_pat_ori.side_up =  _wtoi(f_value);

    // To get scan offset values
    CString x_offset(_T(""));
    CString y_offset(_T(""));
    CString z_offset(_T(""));

    CString str_offset = pparam->GetString(_T("SCANOFFSET"));

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, L"After GetString SCANOFFSET");

    CPQMUtility::GetInstance()->GetScanOffsetValues(str_offset, x_offset, y_offset, z_offset);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, L"After GetScanOffSetValues");

    if (!str_offset.IsEmpty()) {

        m_scan_offset.x = (float)CPQMUtility::GetInstance()->ConvertStringToDouble(x_offset);
        m_scan_offset.y = (float)CPQMUtility::GetInstance()->ConvertStringToDouble(y_offset);
        m_scan_offset.z = (float)CPQMUtility::GetInstance()->ConvertStringToDouble(z_offset);

    } else {

        m_scan_offset.x = (float)0.0;
        m_scan_offset.y = (float)0.0;
        m_scan_offset.z = (float)0.0;
    }

    //Added by Hemant On 7/12/2008 2:08:41 PM
    //m_auto_voice_option = sequence->m_auto_voice_option;
    EncodeAutoVoiceNumberToStringAndsetAutoVoiceOption();

    //Modified by Hemant On 2/25/2009 11:08:38 AM
    //we need to remove the CURRENT from this seq
    //	//MRPdefect615
    //	////Added by Hemant On 12/20/2008 1:19:04 PM
    //	//SetWSEEditingEnum(WSE_None);
    //	SetWSEEditingEnum(sequence->GetWSEEditingEnum());
    //	//MRPdefect615
    SetWSEEditingEnum(sequence->GetWSEEditingEnum(), true);
    //End Of addition By Hemant On2/25/2009 11:08:42 AM

    SetAMBEditingEnum(AMB_NONE);
    //+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
    SetASCompassFlag(m_cpascom_sequence->GetSequenceParam()->GetInt(_T("AS-COMPASS")));
    //-Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
    SetAMBFlag(m_cpascom_sequence->GetSequenceParam()->GetInt(_T("AMB-FLAG")));
    SetAMBModeFlag(m_cpascom_sequence->GetSequenceParam()->GetInt(_T("AMB-MODE-FLAG")));
    //Patni-HAR/2010Apr26/Added/IR-134
    SetMRSEddyAxis(m_cpascom_sequence->GetSequenceParam()->GetInt(_T("EDDY")));

    //Patni-RUP/2010Jul27/Added/TMSC-REDMINE-466
    SetMoveCouchMethodOption(sequence->GetMoveCouchMethodOption());
}

void CSequence::InitPASCOMSequence()
{
    m_cpascom_sequence = new CPASCOMSequence();
}
//****************************Method Header********************
//Method Name   : CSequence::~CSequence()
//Author        : PATNI/DJ
//Purpose       : Destructor.
//*************************************************************

CSequence::~CSequence(
)
{
    DEL_PTR(m_cpascom_sequence)
}


CSequence::CSequence(const CSequence& sequence	//Patni-Sudhir/2011Mar03/Modified/Review Comment
                    ):	m_movecouch_method(NOMOVE),
    m_PrevMovecouch_Method(NOMOVE),				//Patni-Sudhir/2011Mar03/Added/Review Comment
    m_auto_voice_option(NONE_NONE),
    m_Prev_Auto_Voice_Option(NONE_NONE),		//Patni-Abhishek/2010Dec14/Added/IR-180

    m_prescan_aut_voi_optn(NONE),

    m_child_thread_status(CT_NONE),
    m_cpascom_sequence(new CPASCOMSequence(*(sequence.m_cpascom_sequence))),
    m_auto_transfer(sequence.m_auto_transfer),
    m_exe_auto_voice(sequence.m_exe_auto_voice),
    m_scan_time_value(sequence.m_scan_time_value),
    m_canchange_modeoptionbreathhold(sequence.m_canchange_modeoptionbreathhold),
    m_wse_editingenum(WSE_None),
    m_cds_flag(0),
    m_amb_flag(0),
    m_mrs_eddy_axis(0),
    m_amb_mode_flag(0),
    m_sar_info(NULL)
{


    m_pr_mode.m_anatomy = sequence.m_pr_mode.m_anatomy;
    m_pr_mode.m_rcv_coil_id = sequence.m_pr_mode.m_rcv_coil_id;
    m_pr_mode.m_recv_coil_label = sequence.m_pr_mode.m_recv_coil_label;
    m_pr_mode.m_couch_pos = sequence.m_pr_mode.m_couch_pos;
    m_pr_mode.m_home_pos = sequence.m_pr_mode.m_home_pos;
    m_pr_mode.m_couch_offset = sequence.m_pr_mode.m_couch_offset;
    m_pr_mode.m_contrast = sequence.m_pr_mode.m_contrast;
    m_pr_mode.m_str_contrast = sequence.m_pr_mode.m_str_contrast;
    m_pr_mode.m_scan_number = sequence.m_pr_mode.m_scan_number;
    m_pr_mode.m_view_dir = sequence.m_pr_mode.m_view_dir;
    //Patni-PJS/2010Mar12/Added/DeFT# MCM0229-00178
    //+Patni-Abhishek/2010Dec14/Added/IR-180
    m_pr_mode.m_Prev_Contrast = sequence.m_pr_mode.m_Prev_Contrast;
    m_pr_mode.m_Prev_Str_Contrast = sequence.m_pr_mode.m_Prev_Str_Contrast;
    //-Patni-Abhishek/2010Dec14/Added/IR-180
    m_pr_mode.m_str_trsc = sequence.m_pr_mode.m_str_trsc;	//Patni-Sudhir/2011Mar03/Added/Review Comment
    InitializeParam();
    //Added by Hemant On 7/12/2008 2:08:41 PM
    //m_auto_voice_option = sequence.m_auto_voice_option;
    EncodeAutoVoiceNumberToStringAndsetAutoVoiceOption();

    //Modified by Hemant On 2/25/2009 11:08:38 AM
    //we need to remove the CURRENT from this seq
    //	//Added by Hemant On 12/20/2008 1:19:04 PM
    //	SetWSEEditingEnum(sequence.GetWSEEditingEnum());
    SetWSEEditingEnum(sequence.GetWSEEditingEnum(), true);
    //End Of addition By Hemant On2/25/2009 11:09:13 AM
    SetAMBEditingEnum(AMB_NONE);

}




CSequence& CSequence::operator=(const CSequence& sequence)	//Patni-Sudhir/2011Mar03/Modified/Review Comment
{
    if (this == &sequence) {
        return *this;
    }

    m_movecouch_method = NOMOVE;

    //+Patni-Ravindra Acharya/2011Mar25/Modified/Review Comment
    m_PrevMovecouch_Method = NOMOVE;	//Patni-Sudhir/2011Mar03/Added/Review Comment
    //-Patni-Ravindra Acharya/2011Mar25/Modified/Review Comment

    //+Patni-PP/2009Jun01/Modified/SU14 CPP test initialization corrections
    m_auto_voice_option = NONE_NONE;
    //Patni-PJS/2010Jan19/Added/Phase 3 MVC004363
    //Patni-Abhishek/2010Dec14/Added/IR-180
    m_Prev_Auto_Voice_Option = NONE_NONE;
    m_prescan_aut_voi_optn = NONE;
    //Patni-HAR/2010Jan18/Added/DeFT#MVC006364
    m_child_thread_status = CT_NONE;	//Patni-Sudhir/2011Mar03/Added/Review Comment
    m_cpascom_sequence =  new CPASCOMSequence(*(sequence.m_cpascom_sequence));
    m_auto_transfer = sequence.m_auto_transfer;
    m_exe_auto_voice = sequence.m_exe_auto_voice;
    m_scan_time_value = sequence.m_scan_time_value;
    m_canchange_modeoptionbreathhold = sequence.m_canchange_modeoptionbreathhold;
    m_wse_editingenum = WSE_None;
    m_cds_flag = 0;
    m_amb_flag = 0;
    m_amb_mode_flag = 0;
    m_mrs_eddy_axis = 0;

    //-Patni-PP/2009Jun01/Modified/SU14 CPP test initialization corrections

    //Added by Hemant On 12/25/2008 9:24:32 PM
    //+Patni-HAR-SS/2009Apr20/Modified/Defect#112
    //m_canchange_modeoptionbreathhold = true ;

    //-Patni-HAR-SS/2009Apr20/Modified/Defect#112

    m_pr_mode.m_anatomy = sequence.m_pr_mode.m_anatomy;
    m_pr_mode.m_rcv_coil_id = sequence.m_pr_mode.m_rcv_coil_id;
    m_pr_mode.m_recv_coil_label = sequence.m_pr_mode.m_recv_coil_label;
    m_pr_mode.m_couch_pos = sequence.m_pr_mode.m_couch_pos;
    m_pr_mode.m_home_pos = sequence.m_pr_mode.m_home_pos;
    m_pr_mode.m_couch_offset = sequence.m_pr_mode.m_couch_offset;
    m_pr_mode.m_contrast = sequence.m_pr_mode.m_contrast;
    m_pr_mode.m_str_contrast = sequence.m_pr_mode.m_str_contrast;
    m_pr_mode.m_scan_number = sequence.m_pr_mode.m_scan_number;
    m_pr_mode.m_view_dir = sequence.m_pr_mode.m_view_dir;
    //+Patni-Abhishek/2010Dec14/Added/IR-180
    m_pr_mode.m_Prev_Contrast = sequence.m_pr_mode.m_Prev_Contrast;
    m_pr_mode.m_Prev_Str_Contrast = sequence.m_pr_mode.m_Prev_Str_Contrast;
    //-Patni-Abhishek/2010Dec14/Added/IR-180

    //Patni-PJS/2010Mar12/Added/DeFT# MCM0229-00178
    m_pr_mode.m_str_trsc = sequence.m_pr_mode.m_str_trsc;
    //Added by Hemant On 11/19/2008 10:17:51 AM
    m_pr_mode.m_str_scan_anatomy = sequence.m_pr_mode.m_str_scan_anatomy;

    m_delay_time = sequence.m_delay_time;
    m_sar_info = NULL;

    InitializeParam();

    //Added by Hemant On 7/12/2008 2:08:41 PM
    //m_auto_voice_option = sequence.m_auto_voice_option;
    EncodeAutoVoiceNumberToStringAndsetAutoVoiceOption();

    //Modified by Hemant On 2/25/2009 11:08:38 AM
    //we need to remove the CURRENT from this seq
    ////Added by Hemant On 12/20/2008 1:19:04 PM
    //SetWSEEditingEnum(sequence.GetWSEEditingEnum());
    SetWSEEditingEnum(sequence.GetWSEEditingEnum(), true);
    //End Of addition By Hemant On2/25/2009 11:09:32 AM
    SetAMBEditingEnum(AMB_NONE);
    return *this;
}




//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CSequence::EncodeAutoVoiceNumberToStringAndsetAutoVoiceOption(
)
{
    LPCTSTR FUNC_NAME = _T("CSequence::EncodeAutoVoiceNumberToStringAndsetAutoVoiceOption");

    CString str_auto_voice = _T("");

    //+Patni-AD/2009Aug25/Modified/SU15 //CPP test corrections
    if (auto_voice_code != NULL) {
        str_auto_voice =  auto_voice_code[m_exe_auto_voice >> 4];

        str_auto_voice += auto_voice_code[m_exe_auto_voice & 0x0F];

    } else {

        PQM_TRACE(USER_FUNC_MARKER,
                  FUNC_NAME,
                  _T("auto_voice_code pointer is NULL"));

        return;
    }

    //-Patni-AD/2009Aug25/Modified/SU15 //CPP test corrections
    CString auto_voice;

    auto_voice.LoadString(ID_MENUITEM_N_N);
    //Patni-PJS/2010Jan19/Added/Phase 3 MVC004363
    m_prescan_aut_voi_optn = NONE;

    if (str_auto_voice.Compare(auto_voice) == 0) {
        m_auto_voice_option = NONE_NONE;
        return ;
    }

    auto_voice.LoadString(ID_MENUITEM_N_B1);
    //Patni-PJS/2010Jan19/Added/Phase 3 MVC004363
    m_prescan_aut_voi_optn = NONE;

    if (str_auto_voice.Compare(auto_voice) == 0) {
        m_auto_voice_option = NONE_B1;
        return ;
    }

    auto_voice.LoadString(ID_MENUITEM_N_B2);
    //Patni-PJS/2010Jan19/Added/Phase 3 MVC004363
    m_prescan_aut_voi_optn = NONE;

    if (str_auto_voice.Compare(auto_voice) == 0) {
        m_auto_voice_option = NONE_B2;
        return ;
    }


    auto_voice.LoadString(ID_MENUITEM_N_B3);
    //Patni-PJS/2010Jan19/Added/Phase 3 MVC004363
    m_prescan_aut_voi_optn = NONE;

    if (str_auto_voice.Compare(auto_voice) == 0) {
        m_auto_voice_option = NONE_B3;
        return ;
    }

    auto_voice.LoadString(ID_MENUITEM_N_M1);
    //Patni-PJS/2010Jan19/Added/Phase 3 MVC004363
    m_prescan_aut_voi_optn = NONE;

    if (str_auto_voice.Compare(auto_voice) == 0) {
        m_auto_voice_option = NONE_MOTION;
        return ;
    }

    //-------------------------------------------------------------------------
    auto_voice.LoadString(ID_MENUITEM_B1_N);
    //Patni-PJS/2010Jan19/Added/Phase 3 MVC004363
    m_prescan_aut_voi_optn = B1;

    if (str_auto_voice.Compare(auto_voice) == 0) {
        m_auto_voice_option = B1_NONE;
        return ;
    }

    auto_voice.LoadString(ID_MENUITEM_B1_B1);
    //Patni-PJS/2010Jan19/Added/Phase 3 MVC004363
    m_prescan_aut_voi_optn = B1;

    if (str_auto_voice.Compare(auto_voice) == 0) {
        m_auto_voice_option = B1_B1;
        return ;
    }

    auto_voice.LoadString(ID_MENUITEM_B1_B2);
    //Patni-PJS/2010Jan19/Added/Phase 3 MVC004363
    m_prescan_aut_voi_optn = B1;

    if (str_auto_voice.Compare(auto_voice) == 0) {
        m_auto_voice_option = B1_B2;
        return ;
    }


    auto_voice.LoadString(ID_MENUITEM_B1_B3);
    //Patni-PJS/2010Jan19/Added/Phase 3 MVC004363
    m_prescan_aut_voi_optn = B1;

    if (str_auto_voice.Compare(auto_voice) == 0) {
        m_auto_voice_option = B1_B3;
        return ;
    }

    auto_voice.LoadString(ID_MENUITEM_B1_M1);
    //Patni-PJS/2010Jan19/Added/Phase 3 MVC004363
    m_prescan_aut_voi_optn = B1;

    if (str_auto_voice.Compare(auto_voice) == 0) {
        m_auto_voice_option = B1_MOTION;
        return ;
    }

    //-------------------------------------------------------------------------
    auto_voice.LoadString(ID_MENUITEM_B2_N);
    //Patni-PJS/2010Jan19/Added/Phase 3 MVC004363
    m_prescan_aut_voi_optn = B2;

    if (str_auto_voice.Compare(auto_voice) == 0) {
        m_auto_voice_option = B2_NONE;
        return ;
    }

    auto_voice.LoadString(ID_MENUITEM_B2_B1);
    //Patni-PJS/2010Jan19/Added/Phase 3 MVC004363
    m_prescan_aut_voi_optn = B2;

    if (str_auto_voice.Compare(auto_voice) == 0) {
        m_auto_voice_option = B2_B1;
        return ;
    }

    auto_voice.LoadString(ID_MENUITEM_B2_B2);
    //Patni-PJS/2010Jan19/Added/Phase 3 MVC004363
    m_prescan_aut_voi_optn = B2;

    if (str_auto_voice.Compare(auto_voice) == 0) {
        m_auto_voice_option = B2_B2;
        return ;
    }


    auto_voice.LoadString(ID_MENUITEM_B2_B3);
    //Patni-PJS/2010Jan19/Added/Phase 3 MVC004363
    m_prescan_aut_voi_optn = B2;

    if (str_auto_voice.Compare(auto_voice) == 0) {
        m_auto_voice_option = B2_B3;
        return ;
    }

    auto_voice.LoadString(ID_MENUITEM_B2_M1);
    //Patni-PJS/2010Jan19/Added/Phase 3 MVC004363
    m_prescan_aut_voi_optn = B2;

    if (str_auto_voice.Compare(auto_voice) == 0) {
        m_auto_voice_option = B2_MOTION;
        return ;
    }

    //-------------------------------------------------------------------------
    auto_voice.LoadString(ID_MENUITEM_B3_N);
    //Patni-PJS/2010Jan19/Added/Phase 3 MVC004363
    m_prescan_aut_voi_optn = B3;

    if (str_auto_voice.Compare(auto_voice) == 0) {
        m_auto_voice_option = B3_NONE;
        return ;
    }

    auto_voice.LoadString(ID_MENUITEM_B3_B1);
    //Patni-PJS/2010Jan19/Added/Phase 3 MVC004363
    m_prescan_aut_voi_optn = B3;

    if (str_auto_voice.Compare(auto_voice) == 0) {
        m_auto_voice_option = B3_B1;
        return ;
    }

    auto_voice.LoadString(ID_MENUITEM_B3_B2);
    //Patni-PJS/2010Jan19/Added/Phase 3 MVC004363
    m_prescan_aut_voi_optn = B3;

    if (str_auto_voice.Compare(auto_voice) == 0) {
        m_auto_voice_option = B3_B2;
        return ;
    }


    auto_voice.LoadString(ID_MENUITEM_B3_B3);
    //Patni-PJS/2010Jan19/Added/Phase 3 MVC004363
    m_prescan_aut_voi_optn = B3;

    if (str_auto_voice.Compare(auto_voice) == 0) {
        m_auto_voice_option = B3_B3;
        return ;
    }

    auto_voice.LoadString(ID_MENUITEM_B3_M1);
    //Patni-PJS/2010Jan19/Added/Phase 3 MVC004363
    m_prescan_aut_voi_optn = B3;

    if (str_auto_voice.Compare(auto_voice) == 0) {
        m_auto_voice_option = B3_MOTION;
        return ;
    }

    //-------------------------------------------------------------------------
    auto_voice.LoadString(ID_MENUITEM_M1_N);
    //Patni-PJS/2010Jan19/Added/Phase 3 MVC004363
    m_prescan_aut_voi_optn = MOTION;

    if (str_auto_voice.Compare(auto_voice) == 0) {
        m_auto_voice_option = MOTION_NONE;
        return ;
    }

    auto_voice.LoadString(ID_MENUITEM_M1_B1);
    //Patni-PJS/2010Jan19/Added/Phase 3 MVC004363
    m_prescan_aut_voi_optn = MOTION;

    if (str_auto_voice.Compare(auto_voice) == 0) {
        m_auto_voice_option = MOTION_B1;
        return ;
    }

    auto_voice.LoadString(ID_MENUITEM_M1_B2);
    //Patni-PJS/2010Jan19/Added/Phase 3 MVC004363
    m_prescan_aut_voi_optn = MOTION;

    if (str_auto_voice.Compare(auto_voice) == 0) {
        m_auto_voice_option = MOTION_B2;
        return ;
    }


    auto_voice.LoadString(ID_MENUITEM_M1_B3);
    //Patni-PJS/2010Jan19/Added/Phase 3 MVC004363
    m_prescan_aut_voi_optn = MOTION;

    if (str_auto_voice.Compare(auto_voice) == 0) {
        m_auto_voice_option = MOTION_B3;
        return ;
    }

    auto_voice.LoadString(ID_MENUITEM_M1_M1);
    //Patni-PJS/2010Jan19/Added/Phase 3 MVC004363
    m_prescan_aut_voi_optn = MOTION;

    if (str_auto_voice.Compare(auto_voice) == 0) {
        m_auto_voice_option = MOTION_MOTION;
        return ;
    }

}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
bool CSequence::CanChangeModeOptionBreathHold(
)const
{
    return m_canchange_modeoptionbreathhold ;
}


//****************************Method Header********************
//Method Name   : InitializeParam
//Author        : PATNI/DJ
//Purpose       : Initialize the sequence parameter.
//*************************************************************
void CSequence::InitializeParam(
)
{

    LPCTSTR FUNC_NAME = _T("CSequence::InitializeParam");
    LPCTSTR FUNC_NAME1 = _T("pparam") ;


    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
              _T("Trace of InitializeParam"));

    if (m_cpascom_sequence == NULL) {
        return;
    }

    CPASParamStore* pparam = m_cpascom_sequence->GetSequenceParam();

    CString str;
    str.Format(L" = %dx", pparam);
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME1, str);

    //To get anatomy
    m_pr_mode.m_anatomy = pparam->GetInt(_T("SA"));

    CString l_trace;
    l_trace.Format(_T("m_pr_mode.m_anatomy = %d"), m_pr_mode.m_anatomy);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace);

    m_pr_mode.m_str_scan_anatomy.m_scan_anatomy_name = pparam->GetString(_T("BODYPART"));

    //+Patni-Hemant/2010Apr27/Added/JaFT/IR-137
    m_pr_mode.m_str_scan_anatomy.SetLaterality((LateralityEnum)pparam->GetInt(_T("LATERALITY")));
    //-Patni-Hemant/2010Apr27/Added/JaFT/IR-137

    // To get patient orientation
    CString str_pat_ori = pparam->GetString(_T("PORI"));

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME1, L"After GetString");

    CString f_name(_T(""));
    CString f_key(_T(""));
    //+Patni-PJS/MODIFIED On 3/14/2009/ JFT#169
    CString f_value(_T("1"));
    CPQMUtility::GetInstance()->ParseLine(str_pat_ori, f_name, f_key, f_value);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME1, L"After ParseLine");

    m_pr_mode.m_pat_ori.end_in = _wtoi(f_key);
    m_pr_mode.m_pat_ori.side_up =  _wtoi(f_value);

    // To get patient view direction
    m_pr_mode.m_view_dir = pparam->GetInt(_T("VDIR"));
    l_trace.Empty();
    l_trace.Format(_T("m_pr_mode.m_view_dir = %d"), m_pr_mode.m_view_dir);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace);

    // To get receiver coil id
    m_pr_mode.m_rcv_coil_id = (unsigned long)pparam->GetFloat(_T("RC"));
    l_trace.Empty();
    //l_trace.Format(_T("m_pr_mode.m_rcv_coil_id = %f"), m_pr_mode.m_rcv_coil_id );

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace);

    // To get contrast agent
    m_pr_mode.m_str_contrast = pparam->GetString(_T("CONTRASTNAME"));

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, m_pr_mode.m_str_contrast);

    m_pr_mode.m_contrast = 0;

    //+Patni-HEMANT/MODIFIED On 3/28/2009 12:09:13 PM/ PSP1 / Internal Defect Fix
    //
    //	//Added by Hemant On 8/11/2008 7:06:29 PM
    //	if(m_pr_mode.m_str_contrast.Compare(_T("Gadolinium")) == 0)
    //		m_pr_mode.m_contrast = 1;
    //+Patni-Abhishek/2010Dec14/Added/IR-180
    m_pr_mode.m_Prev_Str_Contrast = pparam->GetString(_T("CONTRASTNAME"));
    m_pr_mode.m_Prev_Contrast = 0;
    //-Patni-Abhishek/2010Dec14/Added/IR-180
    CString l_contras_agent_str = m_pr_mode.m_str_contrast;
    l_contras_agent_str.TrimLeft();
    l_contras_agent_str.TrimRight();

    if (!l_contras_agent_str.IsEmpty()) {
        m_pr_mode.m_contrast = 1;
        //Patni-Abhishek/2010Dec14/Added/IR-180
        m_pr_mode.m_Prev_Contrast = 1;
    }

    //
    //-Patni-HEMANT/ ADDED On3/28/2009 12:09:23 PM/ PSP1 / Internal Defect Fix

    // To get auto transfer
    m_auto_transfer = pparam->GetInt(_T("APOST"));// 1;

    //+Patni-HEMANT/ADDED On 3/12/2009 6:36:21 PM/ GENERAL
    //when we have -ve values, we must also make it proper,
    //be'us we are using it like bool..
    //if you do, if(m_auto_transfer==0), then
    //(m_auto_transfer= -1), will give you false.
    m_auto_transfer = (m_auto_transfer > 0) ;
    //

    l_trace.Empty();
    l_trace.Format(_T("m_auto_transfer = %d"), m_auto_transfer);

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, l_trace);

    //Modified by Hemant
    //m_exe_auto_voice = 0;
    m_exe_auto_voice = pparam->GetInt(_T("AUTOVOICE"));// 1;

    // To get scan offset values
    CString x_offset(_T(""));
    CString y_offset(_T(""));
    CString z_offset(_T(""));

    CString str_offset = pparam->GetString(_T("SCANOFFSET"));

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_offset);

    CPQMUtility::GetInstance()->GetScanOffsetValues(str_offset, x_offset, y_offset, z_offset);


    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, L"After GetScanOffSetValues");

    if (!str_offset.IsEmpty()) {
        m_scan_offset.x = (float)CPQMUtility::GetInstance()->ConvertStringToDouble(x_offset);
        m_scan_offset.y = (float)CPQMUtility::GetInstance()->ConvertStringToDouble(y_offset);
        m_scan_offset.z = (float)CPQMUtility::GetInstance()->ConvertStringToDouble(z_offset);

    } else {
        m_scan_offset.x = (float)0.0;
        m_scan_offset.y = (float)0.0;
        m_scan_offset.z = (float)0.0;
    }

    CString str_trsc = pparam->GetString(_T("TRSC"));

    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str_trsc);

    m_pr_mode.m_str_trsc = _T("----");
    pparam->SetString(_T("TRSC"), m_pr_mode.m_str_trsc);

    int l_pln_stat = pparam->GetInt(_T("PLANSTATUS"));

    if (!((WSE_Current == GetWSEEditingEnum()) ||
          (WSE_Planned_AND_Current == GetWSEEditingEnum()))) {

        if (((pparam->GetInt(_T("PLANSTATUS")) & VFC_PLN_STATUS_GLOC) == VFC_PLN_STATUS_GLOC)
            //Patni-Sribanta/2010Sep17/Modified/MVC008802
            && (WSE_Planned_AND_Current != GetWSEEditingEnum())) {
            SetWSEEditingEnum(WSE_Planned);

        } else {
            SetWSEEditingEnum(WSE_None);
        }
    }




    //+Patni-HAR/2010Feb16/Added/MSA0251-00166

    if (pparam->GetInt(_T("DYN")) == TRUE ||
        (pparam->GetInt(_T("GROUPID")) && !pparam->GetInt(_T("MASTERFLAG")))) {
        m_canchange_modeoptionbreathhold = false;

    } else {
        m_canchange_modeoptionbreathhold = true;
    }


    //+Patni-HAR/2010Feb16/Added/MSA0251-00166
    //+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
    SetASCompassFlag(pparam->GetInt(_T("AS-COMPASS")));
    //-Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
    SetAMBFlag(pparam->GetInt(_T("AMB-FLAG")));
    SetAMBModeFlag(pparam->GetInt(_T("AMB-MODE-FLAG")));
    //Patni-HAR/2010Apr26/Added/IR-134
    SetMRSEddyAxis(pparam->GetInt(_T("EDDY")));

    //Patni-RUP/2010Jul27/Added/TMSC-REDMINE-466
    SetMoveCouchMethodOption((MOVECOUCH_METHOD)pparam->GetInt(_T("COUCHMETHOD")));
}

//****************************Method Header********************
//Method Name   : GetSequenceParamStruct
//Author        : PATNI/DJ
//Purpose       : Retrive seuquence parameter structure.
//*************************************************************
void CSequence::GetSequenceParamStruct(
    SEQUENCE_PARAM* f_param,
    Data_decl_list& f_sequence_param_name_list
)
{
    LPCTSTR FUNC_NAME = _T("CSequence::GetSequenceParamStruct");
    PQM_TRACE_SCOPED(FUNC_NAME);

    if (NULL == m_cpascom_sequence) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_cpascom_sequence pointer is NULL"));
        return;
    }

    CPASParamStore* l_pas_store = m_cpascom_sequence->GetSequenceParam();

    if (NULL == l_pas_store) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("l_pas_store pointer is NULL"));
        return;
    }

    CString l_value = _T(""), l_temp = _T("");

    POSITION l_pos = g_sequence_param_name_list.GetHeadPosition();
    DATA_DECL l_decl = {0};
    Data_decl_list l_temp_list;

    int l_counter = 0;

    try {

        CString log_param;

        while (l_pos) {

            l_counter ++;
            l_decl = g_sequence_param_name_list.GetNext(l_pos);

            switch (l_decl.m_type) {
                case DT_INT:
                    l_decl.m_value.integer = l_pas_store->GetInt(l_decl.m_name);
                    l_temp_list.AddTail(l_decl);
                    break;

                case DT_LONG:
                    l_decl.m_value.integer = l_pas_store->GetInt(l_decl.m_name);
                    l_temp_list.AddTail(l_decl);
                    break;

                case DT_STR: {

                    log_param = CString(_T("Param Name =")) + l_decl.m_name + CString(_T("  ; Param Value =")) + l_pas_store->GetString(l_decl.m_name) ;
                    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, log_param);

                    memset(&l_decl.m_value, 0, sizeof(l_decl.m_value));
                    CString param_val = l_pas_store->GetString(l_decl.m_name);

                    const int param_str_length = min(param_val.GetLength(), MAX_PATH - 1);

                    if (param_str_length > 0) {
                        _tcsncpy(l_decl.m_value.str, param_val.GetBuffer(param_str_length), param_str_length);
                        param_val.ReleaseBuffer();
                    }

                    l_temp_list.AddTail(l_decl);
                }
                break;

                case DT_BOOL:
                    l_decl.m_value.integer = l_pas_store->GetBool(l_decl.m_name);
                    l_temp_list.AddTail(l_decl);
                    break;

                case DT_DBL:
                case DT_FLT:

                    l_decl.m_value.dlb = l_pas_store->GetFloat(l_decl.m_name);
                    l_temp_list.AddTail(l_decl);
                    break;

                case DT_VECTOR: {
                    l_decl.m_value.var_data = l_pas_store->GetVector(l_decl.m_name, l_value);
                    l_temp_list.AddTail(l_decl);
                }
                break;
            }
        }

    } catch (...) {

        CString str;
        str.Format(_T("Param Index #%d Failed") , l_counter);
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str);

        str = CString(_T("Param Name =")) + l_decl.m_name ;
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str);

        str = CString(_T("Param Value =")) + l_pas_store->GetString(l_decl.m_name) ;
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, str);

        throw;
    }

    f_sequence_param_name_list.RemoveAll();
    f_sequence_param_name_list.AddTail(&l_temp_list);
}

//****************************Method Header********************
//Method Name   : SetValue
//Author        : PATNI/DJ
//Purpose       : Set the TAG value.
//*************************************************************
void CSequence::SetValue(
    const CString& tagName,
    const CString& Value
)
{

    LPCTSTR FUNC_NAME = _T("CSequence::SetValue");

    //+Patni-AD/2009Aug25/Modified/SU15 //CPP test corrections
    if (NULL == m_cpascom_sequence) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_cpascom_sequence pointer is NULL"));
        return;
    }

    CPASParamStore* pparamstore = m_cpascom_sequence->GetSequenceParam();

    if (NULL == pparamstore) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("pparamstore pointer is NULL"));
        return;
    }

    //-Patni-AD/2009Aug25/Modified/SU15 //CPP test corrections

    DATA_TYPE type = GetType(tagName.AllocSysString());

    long ivalue = 0L;
    float fvalue = 0.0;

    switch (type) {
        case DT_INT:
        case DT_LONG:
            StringToValue(Value, ivalue);
            pparamstore->SetInt(tagName, ivalue);
            break;

        case DT_STR:
            pparamstore->SetString(tagName, Value);
            break;

        case DT_BOOL:
            StringToValue(Value, ivalue);
            pparamstore->SetBool(tagName, ivalue ? true : false);
            break;

        case DT_DBL:
        case DT_FLT:
            StringToValue(Value, fvalue);
            pparamstore->SetFloat(tagName, fvalue);
            break;

        case DT_VECTOR:
            pparamstore->SetVector(tagName, Value);
            break;
    }
}

//****************************Method Header********************
//Method Name   : GetAutoTransferFlag
//Author        : PATNI/DJ
//Purpose       : Get Auto Transfer Flag.
//*************************************************************
int CSequence::GetAutoTransferFlag(
)const
{
    return m_auto_transfer;
}

//****************************Method Header********************
//Method Name   : SetAutoTransferFlag
//Author        : PATNI/DJ
//Purpose       : Set Auto Transfer Flag.
//*************************************************************
void CSequence::SetAutoTransferFlag(
    const int auto_transfer
)
{
    m_auto_transfer = auto_transfer;
}


//****************************Method Header********************
//Method Name   : GetScanOffset
//Author        : PATNI/DJ
//Purpose       : Get Scan Offset.
//*************************************************************
Vector_t& CSequence::GetScanOffset(
)
{
    return m_scan_offset;
}


//****************************Method Header********************
//Method Name   : SetScanOffset
//Author        : PATNI/DJ
//Purpose       : Set Scan Offset.
//*************************************************************
void  CSequence::SetScanOffset(
    Vector_t f_scanoffset
)
{
    m_scan_offset.x = f_scanoffset.x;
    m_scan_offset.y = f_scanoffset.y;
    m_scan_offset.z = f_scanoffset.z;

    CString l_scanoffset;
    l_scanoffset.Format(_T("%f,%f,%f"), m_scan_offset.x, m_scan_offset.y, m_scan_offset.z);
    GetCPASCOMSequence()->GetSequenceParam()->SetString(_T("SCANOFFSET"), l_scanoffset);
}


//****************************Method Header********************
//Method Name   : GetPrMode
//Author        : PATNI/DJ
//Purpose       : Get PR Mode.
//*************************************************************
CPqmPrMode& CSequence::GetPrMode(
)
{
    return m_pr_mode;
}


//****************************Method Header********************
//Method Name   : SetPrMode
//Author        : PATNI/DJ
//Purpose       : Set PR Mode.
//*************************************************************
void CSequence::SetPrMode(
    CPqmPrMode pr_mode
)
{
    m_pr_mode = pr_mode;
}


//****************************Method Header********************
//Method Name   : GetAcqStatus
//Author        : PATNI/DJ
//Purpose       : Return acquisition status.
//*************************************************************
int CSequence:: GetAcqStatus(
)const
{
    LPCTSTR FUNC_NAME = _T("CSequence::GetAcqStatus");

    if (NULL == m_cpascom_sequence) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_cpascom_sequence pointer is NULL"));
        return -1;
    }

    CPASParamStore* ppasparam = m_cpascom_sequence->GetSequenceParam();

    if (NULL == ppasparam) {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("ppasparam pointer is NULL"));

        return -1;
    }

    return ppasparam->GetInt(_T("ACQSTATUS2"));
}

//****************************Method Header********************
//Method Name   : SetAcqStatus
//Author        : PATNI/DJ
//Purpose       : Set acquisition status.
//*************************************************************
void CSequence::SetAcqStatus(
    const int status
)const
{
    LPCTSTR FUNC_NAME = _T("CSequence::SetAcqStatus");

    CPASParamStore* ppasparam = NULL;

    //+Patni-AD/2009Aug25/Modified/SU15 //CPP test corrections
    if (m_cpascom_sequence != NULL) {

        ppasparam = m_cpascom_sequence->GetSequenceParam();
        ppasparam->SetInt(_T("ACQSTATUS2"), status);

    } else {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("m_cpascom_sequence pointer is NULL"));

    }

    return;
    //-Patni-AD/2009Aug25/Modified/SU15 //CPP test corrections
}

//****************************Method Header********************
//Method Name   : GetCPASCOMSequence
//Author        : PATNI/DJ
//Purpose       : Return the reference of CPASCOMSequence object.
//*************************************************************
CPASCOMSequence* const CSequence::GetCPASCOMSequence()
{
    return m_cpascom_sequence;
}

void CSequence::SetCPASCOMSequence(CPASCOMSequence* passcom)
{
    m_cpascom_sequence = passcom;
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
AUTOVOICE_OPTIONS CSequence::GetAutoVoiceOption(
)const
{
    return m_auto_voice_option;
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CSequence::SetAutoVoiceOption(
    const AUTOVOICE_OPTIONS auto_voice_option
)
{
    m_auto_voice_option = auto_voice_option ;
}


//+Patni-PJS/2010Jan19/Added/Phase 3 MVC004363
//**************************************************************************
//Method Name   :
//Author        :  PATNI/PJS
//Purpose       :
//**************************************************************************
AUTOVOICE_PRESCAN_OPTION CSequence::GetPrescanAutoVoiceOptn(
)const
{
    return m_prescan_aut_voi_optn;
}

//**************************************************************************
//Method Name   :
//Author        :  PATNI/PJS
//Purpose       :
//**************************************************************************
void CSequence::SetPrescanAutoVoiceOptn(
    const AUTOVOICE_PRESCAN_OPTION prescan_auto_voice_opt
)
{
    m_prescan_aut_voi_optn = prescan_auto_voice_opt ;
}
//-Patni-PJS/2010Jan19/Added/Phase 3 MVC004363


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CString CSequence::GetScanTimeString(
)const
{
    return m_scan_time_value ;
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CSequence::SetScanTimeString(
    const CString& scan_time_value
)
{
    m_scan_time_value = scan_time_value;
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
WSE_EditingEnum CSequence::GetWSEEditingEnum(
)const
{
    return m_wse_editingenum ;
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CSequence::SetWSEEditingEnum(
    const WSE_EditingEnum wse_editingenum ,
    const bool remove_current /*= false*/ //Added by Hemant On 2/25/2009 11:05:22 AM
)
{
    m_wse_editingenum = wse_editingenum ;

    //Added by Hemant On 2/25/2009 11:07:16 AM
    //Remving the current from this seq..for wse..
    //Now WSE_Current will be WSE_None and WSE_Planned_AND_Current will be WSE_Planned only..
    if (remove_current) {

        //+Patni-HEMANT/ADDED On 3/29/2009 5:17:55 PM/ PSP / Internal Defect
        /*if(WSE_Planned_AND_Current == m_wse_editingenum) {*/
        if ((WSE_Planned_AND_Current == m_wse_editingenum)
            || (WSE_Planned == m_wse_editingenum)
           ) {
            //
            //-Patni-HEMANT/ ADDED On3/29/2009 5:18:04 PM/ PSP / Internal Defect

            m_wse_editingenum = WSE_Planned;

        } else {
            m_wse_editingenum = WSE_None;
        }
    }

    //End Of addition By Hemant On2/25/2009 11:07:20 AM

}
AMB_PlanningStateEnum CSequence::GetAMBEditingEnum()const
{
    return m_amb_planning_state;
}
void CSequence::SetAMBEditingEnum(const AMB_PlanningStateEnum amb_editingenum)
{
    m_amb_planning_state = amb_editingenum;
}


//+Patni-HAR/2010Jan18/Added/DeFT#MVC006364
//****************************Method Header************************************
//Method Name   : GetChildThreadStatus()
//Author        : PATNI\HAR
//Purpose       :
//*****************************************************************************
CHILD_THREAD_STATUS CSequence::GetChildThreadStatus()const
{
    return m_child_thread_status;
}


//****************************Method Header************************************
//Method Name   : SetChildThreadStatus()
//Author        : PATNI\HAR
//Purpose       :
//*****************************************************************************
void CSequence::SetChildThreadStatus(
    const CHILD_THREAD_STATUS f_child_thread_status)
{
    m_child_thread_status = f_child_thread_status;
}

//-Patni-HAR/2010Jan18/Added/DeFT#MVC006364


//+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : GetASCompassFlag
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
int CSequence::GetASCompassFlag(
) const  //Patni-Hemant/2010May6/Added/JaFT/SM_PH3_CDR-CDS_CDR_13
{
    return m_cds_flag ;
}
//-Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement


//+Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement
//************************************Method Header************************************
// Method Name  : SetASCompassFlag
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CSequence::SetASCompassFlag(
    const int f_cds_flag
)
{
    m_cds_flag = f_cds_flag ;

    if (GetCPASCOMSequence()) {
        GetCPASCOMSequence()->GetSequenceParam()->SetInt(_T("AS-COMPASS"), m_cds_flag) ;
    }
}
//-Patni-Hemant/2010Feb9/Added/ACE-Phase#3/CDS Requirement


//************************************Method Header************************************
// Method Name  : GetAMBFlag
// Author       : iGATE
// Purpose      :
//***********************************************************************************
int CSequence::GetAMBFlag(
) const  //Patni-Hemant/2010May6/Added/JaFT/SM_PH3_CDR-CDS_CDR_13
{
    return m_amb_flag ;
}
//************************************Method Header************************************
// Method Name  : SetAMBFlag
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CSequence::SetAMBFlag(
    const int f_amb_flag
)
{
    m_amb_flag = f_amb_flag ;

    if (GetCPASCOMSequence()) {
        GetCPASCOMSequence()->GetSequenceParam()->SetInt(_T("AMB-FLAG"), m_amb_flag) ;
    }
}

//************************************Method Header************************************
// Method Name  : GetAMBFlag
// Author       : iGATE
// Purpose      :
//***********************************************************************************
int CSequence::GetAMBModeFlag(
) const
{
    return m_amb_mode_flag ;
}
//************************************Method Header************************************
// Method Name  : SetAMBFlag
// Author       : iGATE
// Purpose      :
//***********************************************************************************
void CSequence::SetAMBModeFlag(
    const int f_amb_mode_flag
)
{
    m_amb_mode_flag = f_amb_mode_flag ;

    if (GetCPASCOMSequence()) {
        GetCPASCOMSequence()->GetSequenceParam()->SetInt(_T("AMB-MODE-FLAG"), m_amb_mode_flag) ;
    }
}

//+Patni-RUP/2010Jul22/Added/TMSC-REDMINE-466
//****************************Method Header************************************
//Method Name   : GetMoveCouchMethodOption
//Author        : PATNI\RUP
//Purpose       : To get current couch option sequence
//*****************************************************************************
MOVECOUCH_METHOD CSequence::GetMoveCouchMethodOption(
)const
{
    return m_movecouch_method;
}


//****************************Method Header************************************
//Method Name   : SetMoveCouchMethodOption
//Author        : PATNI\RUP
//Purpose       : To set current couch option sequence
//*****************************************************************************
void CSequence::SetMoveCouchMethodOption(const MOVECOUCH_METHOD f_movecouch_method
                                        )
{
    m_movecouch_method = f_movecouch_method;
    GetCPASCOMSequence()->GetSequenceParam()->SetInt(_T("COUCHMETHOD"), f_movecouch_method);

}
//-Patni-RUP/2010Jul22/Added/TMSC-REDMINE-466

//+Patni-Abhishek/2010Dec14/Added/IR-180
//************************************Method Header************************************
// Method Name  : GetPrevMoveCouchMethodOption
// Author       : PATNI/ ABHISHEK
// Purpose      : IR-180
//***********************************************************************************
MOVECOUCH_METHOD CSequence::GetPrevMoveCouchMethodOption(
)const
{
    return m_PrevMovecouch_Method;
}


//************************************Method Header************************************
// Method Name  : SetPrevMoveCouchMethodOption
// Author       : PATNI/ ABHISHEK
// Purpose      : IR-180
//***********************************************************************************
void CSequence::SetPrevMoveCouchMethodOption(const MOVECOUCH_METHOD f_movecouch_method
                                            )
{
    m_PrevMovecouch_Method = f_movecouch_method;

}

//************************************Method Header************************************
// Method Name  : GetPrevAutoVoiceOption
// Author       : PATNI/ ABHISHEK
// Purpose      : IR-180
//***********************************************************************************
AUTOVOICE_OPTIONS CSequence::GetPrevAutoVoiceOption(
)const
{
    return m_Prev_Auto_Voice_Option;
}

//************************************Method Header************************************
// Method Name  : SetPrevAutoVoiceOption
// Author       : PATNI/ ABHISHEK
// Purpose      : IR-180
//***********************************************************************************
void CSequence::SetPrevAutoVoiceOption(
    const AUTOVOICE_OPTIONS f_auto_voice_option
)
{
    m_Prev_Auto_Voice_Option = f_auto_voice_option ;
}

//-Patni-Abhishek/2010Dec14/Added/IR-180




//+Patni-Hemant/2010Apr27/Added/JaFT/IR-137
//************************************Method Header************************************
// Method Name  : GetLateralityShortStr
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
CString CSequence::GetLateralityShortStr(
)const
{
    switch (m_pr_mode.m_str_scan_anatomy.GetLaterality()) {

        case Laterality_None:
            return g_L_None_ShortName;

        case Laterality_Left:
            return g_L_Left_ShortName;

        case Laterality_Right:
            return g_L_Right_ShortName;

        default :
            return g_L_Invalid_ShortName;
    }

    return g_L_Invalid_ShortName;
}
//-Patni-Hemant/2010Apr27/Added/JaFT/IR-137



//****************************Method Header********************
//Method Name   : SetAutoVoiceCode
//Author        : PATNI/DJ
//Purpose       : Set Auto Voice code.
//*************************************************************
void CSequence::SetAutoVoiceCode(
    const int exe_auto_voice
)
{
    m_exe_auto_voice = exe_auto_voice;
    GetCPASCOMSequence()->GetSequenceParam()->SetInt(_T("AUTOVOICE"), m_exe_auto_voice);
}


//************************************Method Header************************************
// Method Name  :
// Author       : PATNI/ HEMANT
// Purpose      :
//***********************************************************************************
void CSequence::SetCanChangeModeOptionBreathHold(
    const bool canchange_modeoptionbreathhold
)
{
    m_canchange_modeoptionbreathhold = canchange_modeoptionbreathhold ;
}

//****************************Method Header************************************
//Method Name   : GetDelayTimeString
//Author        : PATNI\AKR
//Purpose       : To get delay time as string
//*****************************************************************************
CString CSequence::GetDelayTimeString() const
{
    return m_delay_time;
}
//****************************Method Header************************************
//Method Name   : SetDelayTimeString
//Author        : PATNI\AKR
//Purpose       : To Set delay time as string
//*****************************************************************************
void CSequence::SetDelayTimeString(const CString& delay_time_value)
{
    m_delay_time = delay_time_value;
}

//****************************Method Header********************
//Method Name   : SetSARInfoPtrForSeq
//Author        : iGate
//Purpose       :
//*************************************************************
void CSequence::SetSARInfoPtrForSeq(sarInfo_t* f_sar_info)
{
    m_sar_info = f_sar_info;
}

//****************************Method Header********************
//Method Name   : GetSARInfoPtrForSeq
//Author        : iGate
//Purpose       :
//*************************************************************
sarInfo_t* CSequence::GetSARInfoPtrForSeq() const
{
    return m_sar_info;
}

//****************************Method Header********************
//Method Name   : StringToValue
//Author        : PATNI/DJ
//Purpose       : Convert string to value, overloaded function.
//*************************************************************
inline void CSequence::StringToValue(
    const CString& f_seqParamIn,
    long& f_seqParamOut
)
{
    f_seqParamOut = _ttol(f_seqParamIn);
}


//****************************Method Header********************
//Method Name   : StringToValue
//Author        : PATNI/DJ
//Purpose       : Convert string to value.
//*************************************************************
void CSequence::StringToValue(
    const CString& f_seqParamIn,
    float& f_seqParamOut
)
{
    TCHAR* l_stop_string = NULL;
    f_seqParamOut = (float)_tcstod(f_seqParamIn, &l_stop_string);
}

//+Patni-HAR/2010Apr26/Added/IR-134
//****************************Method Header************************************
//Method Name   : GetMRSEddyAxis()
//Author        : PATNI\HAR
//Purpose       :
//*****************************************************************************
void CSequence::SetMRSEddyAxis(
    const int f_mrs_eddy_axis
)
{
    m_mrs_eddy_axis = f_mrs_eddy_axis;

    if (GetCPASCOMSequence()) {
        GetCPASCOMSequence()->GetSequenceParam()->SetInt(_T("EDDY"), m_mrs_eddy_axis) ;
    }
}
//-Patni-HAR/2010Apr26/Added/IR-134


//****************************Method Header********************
//Method Name   : GetType
//Author        : PATNI/DJ
//Purpose       : Get Data type
//*************************************************************
DATA_TYPE CSequence::GetType(
    _TCHAR* param_name
)
{
    POSITION pos = g_sequence_param_name_list.GetHeadPosition();
    DATA_DECL data;

    while (pos) {
        data = g_sequence_param_name_list.GetNext(pos);

        if (_tcscmp(data.m_name, param_name) == 0) {
            break;
        }
    }

    return data.m_type;
}

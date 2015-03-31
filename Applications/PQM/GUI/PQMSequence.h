//REDMINE_782_CPP_Test_Tool
//TMSC-REDMINE-783
//TMSC-REDMINE-780
//Redmine-777
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMSequence.h
 *  Overview: Structure of CPQMSequence class.
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
#ifndef __SM_PQM_SEQUENCE_H__
#define __SM_PQM_SEQUENCE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tami/libSAR/sar.h>
#include <PASCOMLib/PASCOMSequence.h>
#include <PQM/pqmcommon.h>
#include "PQMDefines.h"

enum AUTOVOICE_OPTIONS {

    NONE_NONE ,
    NONE_B1 ,
    NONE_B2 ,
    NONE_B3 ,
    NONE_MOTION ,

    B1_NONE ,
    B1_B1 ,
    B1_B2 ,
    B1_B3 ,
    B1_MOTION ,

    B2_NONE ,
    B2_B1 ,
    B2_B2 ,
    B2_B3 ,
    B2_MOTION ,

    B3_NONE ,
    B3_B1 ,
    B3_B2 ,
    B3_B3 ,
    B3_MOTION ,

    MOTION_NONE ,
    MOTION_B1 ,
    MOTION_B2 ,
    MOTION_B3 ,
    MOTION_MOTION ,

    AUTOVOICE_OPTIONS_INVALID
};

enum AUTOVOICE_PRESCAN_OPTION {
    NONE,
    B1,
    B2,
    B3,
    MOTION,
    AUTOVOICE_PRESCAN_OPTION_INVALID
};

enum WSE_EditingEnum {

    WSE_Current,
    WSE_Planned,
    WSE_Planned_AND_Current,
    WSE_None
};

enum AMB_PlanningStateEnum {
    AMB_NONE,
    AMB_PLAN,
    AMB_PLANNED
};

typedef enum {
    NOMOVE          = -1,
    CENTREPLAN      = 0,
    PARENTREFPOS    = 1
} MOVECOUCH_METHOD;

class CSequence
{
public:
    CSequence();
    explicit CSequence(CPASCOMSequence* f_cpascom_sequence);
    explicit CSequence(const CSequence* sequence);
    virtual ~CSequence();

    CSequence(const CSequence& sequence);
    CSequence& operator=(const CSequence& sequence);

    void EncodeAutoVoiceNumberToStringAndsetAutoVoiceOption();
    bool CanChangeModeOptionBreathHold()const;
    void InitializeParam();
    void GetSequenceParamStruct(SEQUENCE_PARAM* f_param, Data_decl_list& f_sequence_param_name_list);
    void SetValue(const CString& tagName, const CString& Value);

    int GetAutoTransferFlag()const;
    void SetAutoTransferFlag(const int auto_transfer);

    Vector_t& GetScanOffset();
    void  SetScanOffset(Vector_t f_scanoffset);

    CPqmPrMode& GetPrMode();
    void SetPrMode(CPqmPrMode pr_mode);

    int GetAcqStatus()const;
    void SetAcqStatus(const int status)const;

    CPASCOMSequence* const GetCPASCOMSequence();
    void SetCPASCOMSequence(CPASCOMSequence* passcom);

    AUTOVOICE_OPTIONS GetAutoVoiceOption()const;
    void SetAutoVoiceOption(const AUTOVOICE_OPTIONS auto_voice_option);

    AUTOVOICE_PRESCAN_OPTION GetPrescanAutoVoiceOptn()const;
    void SetPrescanAutoVoiceOptn(const AUTOVOICE_PRESCAN_OPTION prescan_auto_voice_opt);

    CString GetScanTimeString()const ;
    void SetScanTimeString(const CString& scan_time_value);

    WSE_EditingEnum GetWSEEditingEnum()const;
    void SetWSEEditingEnum(const WSE_EditingEnum wse_editingenum, const bool remove_current = false);


    AMB_PlanningStateEnum GetAMBEditingEnum()const;
    void SetAMBEditingEnum(const AMB_PlanningStateEnum wse_editingenum);

    CHILD_THREAD_STATUS GetChildThreadStatus()const;
    void SetChildThreadStatus(const CHILD_THREAD_STATUS f_child_thread_status);

    int GetASCompassFlag()const;
    void SetASCompassFlag(const int f_cds_flag);

    int GetAMBFlag()const;
    void SetAMBFlag(const int f_amb_flag);
    int GetAMBModeFlag()const;
    void SetAMBModeFlag(const int f_amb_mode_flag);
    MOVECOUCH_METHOD GetMoveCouchMethodOption()const;
    void SetMoveCouchMethodOption(const MOVECOUCH_METHOD f_movecouch_method);

    MOVECOUCH_METHOD GetPrevMoveCouchMethodOption()const;
    void SetPrevMoveCouchMethodOption(const MOVECOUCH_METHOD f_movecouch_method);

    AUTOVOICE_OPTIONS GetPrevAutoVoiceOption()const;
    void SetPrevAutoVoiceOption(const AUTOVOICE_OPTIONS f_auto_voice_option);

    CString GetLateralityShortStr()const;

    void SetAutoVoiceCode(const int exe_auto_voice);

    void SetCanChangeModeOptionBreathHold(const bool canchange_modeoptionbreathhold);


    CString GetDelayTimeString() const ;
    void SetDelayTimeString(const CString& delay_time_value);

    void InitPASCOMSequence();

    void SetSARInfoPtrForSeq(sarInfo_t* f_sar_info);
    sarInfo_t* GetSARInfoPtrForSeq() const;

private:
    MOVECOUCH_METHOD m_movecouch_method;
    MOVECOUCH_METHOD m_PrevMovecouch_Method;
    AUTOVOICE_OPTIONS m_auto_voice_option;
    AUTOVOICE_OPTIONS m_Prev_Auto_Voice_Option;
    AUTOVOICE_PRESCAN_OPTION m_prescan_aut_voi_optn;
    CHILD_THREAD_STATUS m_child_thread_status;

    CPASCOMSequence* m_cpascom_sequence;
    int m_auto_transfer;
    //stores auto voice setting number for prescan & scan
    int m_exe_auto_voice;
    CString m_scan_time_value;
    bool m_canchange_modeoptionbreathhold;
    WSE_EditingEnum m_wse_editingenum ;
    int m_cds_flag ;
    int m_mrs_eddy_axis;
    int m_amb_flag;
    int m_amb_mode_flag;
    AMB_PlanningStateEnum m_amb_planning_state;
    CPqmPrMode m_pr_mode;
    CString		m_delay_time;

    //Scan Offset
    Vector_t          m_scan_offset;
    sarInfo_t*		m_sar_info;

    void StringToValue(const CString& f_seqParamIn, long& f_seqParamOut);
    void StringToValue(const CString& f_seqParamIn, float& f_seqParamOut);

    void SetMRSEddyAxis(const int f_mrs_eddy_axis);

    DATA_TYPE GetType(_TCHAR* param_name);

};

#endif // #ifndef __SM_PQM_SEQUENCE_H__

//TMSC-REDMINE-777
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: PQMDefines.h
 *  Overview: Structure of Defines used in PQM.
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
#ifndef PQMDEFINES_H
#define PQMDEFINES_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <PQM/PQMCommonStructures.h> //Patni-Hemant/2010Apr27/Added/JaFT/IR-137
#include "PQMGUICommon.h"

typedef struct {

    LVITEM* ptr_lv_item;//lvi;

    CStringArray str_arr_col;

} slvitem_t, *p_slvitem_t;

typedef enum operation {

    scan_pause = 0,
    scan_resume,
} scan_operation;



//+Patni-Hemant/2010Apr26/Added/JaFT/IR-137
/*
struct SScanAantomy {

    int m_scan_anatomy_vf;
    CString m_scan_anatomy_name;

    SScanAantomy() {

        m_scan_anatomy_vf = -1 ;
        m_scan_anatomy_name	= _T("");
    }

    SScanAantomy(int scan_anatomy_vf, CString scan_anatomy_name) {

        m_scan_anatomy_vf = scan_anatomy_vf ;
        m_scan_anatomy_name	= scan_anatomy_name ;
    }

    bool operator == (const SScanAantomy& scanaantomy_data) {

        return ((m_scan_anatomy_name.CompareNoCase(scanaantomy_data.m_scan_anatomy_name) == 0)
                &&
                (m_scan_anatomy_vf == scanaantomy_data.m_scan_anatomy_vf)
               )  ;
    }
};
*/
//-Patni-Hemant/2010Apr26/Added/JaFT/IR-137

//! The class is used to store protocol mode.
struct  CPqmPrMode {

    //Receiver coil id
    unsigned long    m_rcv_coil_id;

    CString m_recv_coil_label;

    //couch position
    float  m_couch_pos;

    //home position
    float  m_home_pos;

    //couch offset
    float  m_couch_offset;

    //anantomy

    int  m_anatomy;

    SScanAantomy m_str_scan_anatomy;//SM3_Mangesh

    //contrast agent
    int  m_contrast;
    //Patni-Abhishek/2010Dec14/Added/IR-180
    int  m_Prev_Contrast;

    //contrast agent
    CString  m_str_contrast;
    //Patni-Abhishek/2010Dec14/Added/IR-180
    CString  m_Prev_Str_Contrast;

    //   Prot   Protocol_idx   scan number
    //   A      1              1
    //   B      2              1
    //   C      3              2
    //   D      4              2

    //It gets reset on reboot
    int  m_scan_number;

    //view direction
    int  m_view_dir;

    // Patient orientation
    Patori_t     m_pat_ori;

    CString m_str_trsc;

};

typedef struct TagColorsetting {
    TCHAR*    status;  // "Wait"/"Current"/"Failed"/"Done"
    long    FColor;
    long    BColor;
} Colorsetting;


#endif //PQMDEFINES_H
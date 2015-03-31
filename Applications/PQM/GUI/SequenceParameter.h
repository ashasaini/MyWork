/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: SequenceParameter.h
 *  Overview: Structure of CSequenceParameter class.
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
#ifndef _SEQ_PARAM_H
#define _SEQ_PARAM_H

#pragma warning(disable:4786)

// Include all the constants.
#include "stdlib.h"


#define	  WM_RECEIVE_SEQUENCE		WM_USER + 0x07


// Type of the folder
// It determines the hierarchy of the PAS DB
typedef enum tagFOLDER_TYPE_s {

    DATABASE        ,   //for UP/TP/etc path
    CONTAINER_FOLDER,   //for continer folder path within UP/TP folder
    CONTAINER_XML   ,   //for conainer.xml path within container folder
    PAS_FOLDER      ,   //for pas_unique_id folder path
    PAS_XML         ,   //for pas.xml within pas_unique_id folder
    SEQUENCE_XML        //fpr seq_unique_id within pas_unique_id folder

} FOLDER_TYPE;



// Data Type .
// it is divided into two parts...
// 1. System defined data type like , int, float etc.
// 2. User defined data types like, complex #, structure etc.
typedef enum tagDATA_TYPE_s {

    DT_INT = SYSTEM_DEFINED_DATATYPE,
    DT_FLT,
    DT_BOOL,
    DT_STR,
    DT_LONG,
    DT_DBL,

    DT_CHILD = USER_DEFINED_DATATTYPE,
    DT_VECTOR,
    DT_DATE

} DATA_TYPE;

typedef struct tagStringToData_s {

    TCHAR       string_data[MAX_PATH];
    DATA_TYPE   data_type;

} STRING_TO_DT;

static STRING_TO_DT String_to_datatype[] = {

    { _T("DT_STR"),   DT_STR},// all system defined datatypes.
    { _T("DT_INT"),   DT_INT},
    { _T("DT_FLT"),   DT_FLT},
    { _T("DT_BOOL"),  DT_BOOL},
    { _T("DT_STR"),   DT_STR},
    { _T("DT_LONG"),  DT_LONG},
    { _T("DT_DBL"),   DT_DBL},

    { _T("DT_CHILD"), DT_CHILD},// all user defined datatypes
    { _T("DT_VECTOR"), DT_VECTOR},
    { _T("DT_DATE"),  DT_DATE}
};


// Value structure
// A value container for each data type
typedef union tagVALUE_u {

    int			integer;
    float		flt;
    BOOL		boolean;
    TCHAR	 	str[MAX_PATH];
    long		lng;
    double		dlb;
    VARIANT     var_data;

} VALUE;


// Data Decleration, contains
// 1) variable name
// 2) data type of the variable
typedef struct tagDATA_DECL_s {

    _TCHAR		m_name[1024];
    DATA_TYPE	m_type;
    VALUE       m_value;

} DATA_DECL;


// Field is overall representation of the structure.
// It contains, DATA_DECL, which store data type and the name.
// It contains, value of the variable.
typedef struct tagFIELD_s {

    DATA_DECL	decl;
    VALUE		value;

} FIELD;

// Satya
//  Implementation, containing an array of fields and for each field containing
//	1. Variable Name
//	2. Variable Type
//	3. Variable Value
typedef struct tagSEQUENCE_PARAM_s {

    FIELD	fields;

} SEQUENCE_PARAM;



typedef struct tagPAS {

    PasLabel    label;
    PasUniqueId unique_id;

} PAS, *PASPtr;


//Component data types
typedef DATA_DECL seq_paramstore;

typedef CList <PAS, PAS&>		                        PasList;
typedef CList <CPAS, CPAS&>                             CPASList;
typedef CList <PasUniqueId, PasUniqueId&>               PasUniqueIdList;
typedef CList <SequenceUniqueId, SequenceUniqueId&>     SequenceUniqueIdList;
typedef CList <CPASCOMSequence*, CPASCOMSequence*>       SequenceList;
typedef CList <IPASCOMListener*, IPASCOMListener*>      PASCOMListenerList;
typedef CList <CString, CString&>						AttrList;
typedef CList <CString, CString&>						AttrValues;
typedef CList <DATA_DECL, DATA_DECL&>                   Data_decl_list;

static Data_decl_list                                    m_sequence_param_name_list;

#endif  //#ifndef _SEQ_PARAM_H



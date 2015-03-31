/*******************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name		: PASCOMConstants.h
 *  Overview		:
 *  Last Modified	: [6/6/2008]
 *******************************************************************************
 *                       Revision History
 *******************************************************************************
 *  Revision	Author		Date & Time			Changes
 *==============================================================================
 *   1.0		Patni       [6/6/2008]			Added file header
 *
 ******************************************************************************/

#ifndef __SM_PASCOM_SEQ_CONSTANTS_H__
#define __SM_PASCOM_SEQ_CONSTANTS_H__

//Header for CList
#include <afxtempl.h>
#include <map>

#define DllExport __declspec(dllexport)

const CString   PATH_CONNECTOR  =   _T("\\");
const TCHAR     SLASH			=   _T('/');
const CString   DOT_SLASH		=   _T("./");
const CString   DOT             =   _T(".");
const CString   CHILD_PARAM     =   _T("DT_CHILD");
const CString   COMMA           =   _T(",");

#define PAS_PATH  "/GP/PAS"

#define   START_FROM                1 // starting point of search in CString.

#define	  SYSTEM_DEFINED_DATATYPE	0
#define	  USER_DEFINED_DATATTYPE	100
#define	  WM_RECEIVE_SEQUENCE		WM_USER + 0x07

// Number of the Parametes in the structures.
#define SEQ_PARAM_CNT	    (sizeof(Seq_Param_Map)/sizeof(Seq_Param_Map[0]))
#define SPDRR_CNT			(sizeof(SPDRR)/sizeof(SPDRR[0]))


//Sequence related data types
typedef CString         ComponentName;
typedef CString         SequenceUniqueId;

//PAS related data types
typedef CString         PasUniqueId;
typedef CString         PasLabel;

//For return types
typedef UINT            ERROR_CODE;

#define DD_NAME(x)		x.m_name	// macro for getting the variable name
#define DD_TYPE(x)		x.m_type	// macro for getting the variable type

// Macro definition to access the Field's elements.
#define MS_NAME(x, i)	DD_NAME(x.fields[i].decl)
#define MS_TYPE(x, i)	DD_TYPE(x.fields[i].decl)
#define MS_VALUE_INT(x, i) x.fields[i].value.integer
#define MS_VALUE_FLT(x, i) x.fields[i].value.flt
#define MS_VALUE_BOOL(x, i) x.fields[i].value.boolean
#define TYPE_STR(x)		#x

//PASCOM Configuration files
const	CString	SEQ_PARA_XML					=	_T("seq_para.xml");
const	CString	SEQUENCEPARAMETER_XML			=	_T("sequenceparameter.xml");
const	CString	PASCOM_INI						=	_T("pascom.ini");
const	CString	COMMUNICATION_CHANNEL_SECTION	=	_T("COMMUNICATION CHANNEL");
const	CString DEFAULT_PGUID_XML				=	_T("PQM_Default_PGUID.xml");

// + MEM_PAS_021 AMT 07 Mar 07
//+ Patni-PJS/2009May30/Modify/JFT#118
/*typedef enum DISPLAY_ANATOMY_S{
    DONOT_DISPLAY_HEAD        = 1,
    DONOT_DISPLAY_MISC        = 2,
    DONOT_DISPLAY_CHEST       = 4,
    DONOT_DISPLAY_CSPINE      = 8,
    DONOT_DISPLAY_TLSPINE     = 16,
    DONOT_DISPLAY_ABDOMEN     = 32,
    DONOT_DISPLAY_PELVIS      = 64,
    DONOT_DISPLAY_LOWEREXT    = 128,
    DONOT_DISPLAY_UPPEREXT    = 256,
} Display_Anatomy_t;*/
typedef enum DISPLAY_ANATOMY_S {
    DONOT_DISPLAY_HEAD        = 1,
    DONOT_DISPLAY_OTHER		  = 2,
    //+Patni-PJS/2010Nov29/Modify/REDMINE-1039/Refactoring
    DONOT_DISPLAY_CHEST       = 4,
    DONOT_DISPLAY_CSPINE      = 8,
    DONOT_DISPLAY_TLSPINE     = 16,
    //-Patni-PJS/2010Nov29/Modify/REDMINE-1039/Refactoring
    DONOT_DISPLAY_ABDOMEN     = 32,
    DONOT_DISPLAY_PELVIS      = 64,
    DONOT_DISPLAY_LOWEREXT    = 128,
    DONOT_DISPLAY_UPPEREXT    = 256,
    DONOT_DISPLAY_MISC        = 512,
    DONOT_DISPLAY_SEQ		  = 1024,
    DONOT_DISPLAY_QA		  = 2048     //Patni-NP/2009Sep09/Modified/MVC004527
} Display_Anatomy_t;
//+ Patni-PJS/2009May30/Modify/JFT#118
//+Patni-PJS/2010Nov29/Modify/REDMINE-1039
const Display_Anatomy_t g_dis_anatomy[] = {
    DONOT_DISPLAY_HEAD ,
    DONOT_DISPLAY_OTHER ,
    DONOT_DISPLAY_CHEST,
    DONOT_DISPLAY_CSPINE,
    DONOT_DISPLAY_TLSPINE,
    DONOT_DISPLAY_ABDOMEN,
    DONOT_DISPLAY_PELVIS ,
    DONOT_DISPLAY_LOWEREXT,
    DONOT_DISPLAY_UPPEREXT,
    DONOT_DISPLAY_MISC,
    DONOT_DISPLAY_SEQ,
    DONOT_DISPLAY_QA         //Patni-NP/2009Sep09/Modified/MVC004527
};
//-Patni-PJS/2010Nov29/Modify/REDMINE-1039/

//Patni-KSS/2011Feb07/Added/Review Comments From TMSC
const int g_disp_ana_count = sizeof(g_dis_anatomy) / sizeof(g_dis_anatomy[0]);

// + MEM_PAS_041 AMT 14 Mar 07
typedef CMap<CString, LPCTSTR, int, int&> AnatomyBitMap;
typedef CMap<CString, LPCTSTR, int, int&> PASTypeBitMap;
typedef CMap<CString, LPCTSTR, int, int&> PASAnatomyMap;
typedef CList<CString, CString> AnatomyName;
typedef CList<CString, CString> AnatomyNumber;
// - MEM_PAS_041 AMT 14 Mar 07
// - MEM_PAS_021 AMT 07 Mar 07

///Usha M ( Dec/10/07)
// to remove dependency of PASCOM.ini file
const CString PASCOMM = _T("PASCOMM");

const CString ACQUIRE_PQM = _T("Acquire.PQM.PASCOMM");
const CString SCHEDULE_PQM = _T("Schedule.PQM.PASCOMM");
const CString ACQUIRE_MEMORY = _T("Acquire.Memory.PASCOMM");
const CString SCHEDULE_MEMORY = _T("Schedule.Memory.PASCOMM");
const CString UTILITY_MEMORY = _T("Utility_Close.Memory.PASCOMM");

///Usha M ( Dec/10/07)

//Patni-RSG/2009Aug13/Modified/PASACOM Performence Change
const int MAX_VF_CHAR_LEN  = 512;

#endif // #ifndef __SM_PASCOM_SEQ_CONSTANTS_H__



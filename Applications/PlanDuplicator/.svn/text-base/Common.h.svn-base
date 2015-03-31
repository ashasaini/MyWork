// Common.h: interface for the CCommon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMON_H__9B7DE819_601F_4377_8096_D114E28EDE90__INCLUDED_)
#define AFX_COMMON_H__9B7DE819_601F_4377_8096_D114E28EDE90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MPlusDicomTag/MPlusDicomTag.h"
//#include "MPlusDicomTag/MPlusMRTag.h"

#define EMPTY_STRING             _T("")

#define STUDY_PATH				_T("Study")			//MR-Recon2 Req#1
#define SITE					_T("SITE")			//MR-Recon2 Req#1
	
#ifdef _DEBUG  
#define BUILD_MODE DEBUGTEXT
#else
#define BUILD_MODE RELEASETEXT
#endif
//+Patni-AD/2009Oct22/Modified/SU15 //#MVC#4873 As per inputs from Tanoue-san
#define LOG_TRACE if(bl_trace_info) bl_trace_info->LoadLog
#define LOG_ERROR if(bl_error_info) bl_error_info->LogError
//-Patni-AD/2009Oct22/Modified/SU15 //#MVC#4873 As per inputs from Tanoue-san

//+Patni-AD/2009Mar25/Added/SU13 Code review comment from Tanoue-san
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&);               \
    TypeName& operator=(const TypeName&)

#define DISCONNECT_SYNC_EVNT	_T("Client Disconnect Sync Event")

//-Patni-AD/2009Mar25/Added/SU13 Code review comment from Tanoue-san

//+Patni-SGS/2010Jun14/Added/MR-Recon2 Req#1 implementation
#define STUDY_NUMBER_TAG 0x00200010
//-Patni-SGS/2010Jun14/Added/MR-Recon2 Req#1 implementation
//-----------------
//+Patni-NT/2010Aug09/Added/MR-Recon3 Req#1 Defect Fix implementation
#define QA_STUDY   _T("QAStudy")
//-Patni-NT/2010Aug09/Added/MR-Recon3 Req#1 Defect Fix implementation

//+Patni-VP/2012Feb09/Added/Design implementation
#define NUMBEROFROI 3
#define DOMAIN_PD     _T("MRI_PD")
#define PD_TRACE_CSSDK _T("PD_TRACE_CSSDK")
#define PD_ERROR_CSSDK _T("PD_ERROR_CSSDK")
#define PD_TRACE_FILE_PATH _T("PD_TRACE_FILE_PATH")
#define PD_ERROR_FILE_PATH _T("PD_ERROR_FILE_PATH")
#define PD_TRACE_LEVEL _T("TraceLevelPD")
#define PD_MARKER _T("MARKER")
#define MRMP_CONFIG		"MRMP_Config"
#define MRMP_LOGDIR		"MRMP_LOGDIR"
#define NUMBER_BASE 16
#define PD_INI_FILE _T("\\PD_Config.ini")
#define PD_TRACECONFIG_INI_FILE _T("\\PD_TraceConfig.ini")
#define PD_ERROR_LOG_FILE _T("\\PDErrorLog.txt")
#define PD_TRACE_LOG_FILE _T("\\PDTraceLog.txt")
#define PD_TOOL _T("\\PDTool")
//-Patni-VP/2012Feb09/Added/Design implementation
#define PD_APPLICATION "PlanDuplicator.exe"


#endif // !defined(AFX_COMMON_H__9B7DE819_601F_4377_8096_D114E28EDE90__INCLUDED_)

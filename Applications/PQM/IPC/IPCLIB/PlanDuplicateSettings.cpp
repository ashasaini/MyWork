//Redmine-783
//Redmine-779
//Redmine-780
//Redmine-781
// PlanDuplicateSettings.cpp: implementation of the CPlanDuplicateSettings class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PlanDuplicateSettings.h"
extern "C" {
#include <tami/libAcq/planDuplicate.h>
}
#include <tami/libAcq/AcqError.h>
#include <tami/libpl/plDuplicate.h>
#include <PQM/PQMLogMgr.h>
#include <PQM/PqmUtilities.h>
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//**************************************************************************
//Method Name   : CPlanDuplicateSettings
//Author        : PATNI\DKH
//Purpose       : Constructor
//**************************************************************************
CPlanDuplicateSettings::CPlanDuplicateSettings()
{
    m_plandupsettings.mask = PL_DUP_MASK_ORI_VEC | PL_DUP_MASK_OFF_VEC ;
    m_plandupsettings.defaultmask = PL_DUP_MASK_ORI_VEC | PL_DUP_MASK_OFF_VEC ;
}

//**************************************************************************
//Method Name   : ~CPlanDuplicateSettings()
//Author        : PATNI\DKH
//Purpose       : Destructor
//**************************************************************************
CPlanDuplicateSettings::~CPlanDuplicateSettings()
{

}

//**************************************************************************
//Method Name   : InitPlanDupSettingsFile
//Author        : PATNI\DKH
//Purpose       : To initialize plan duplicate settings
//**************************************************************************
bool CPlanDuplicateSettings::InitPlanDupSettingsFile(
    const CString& f_studyfile
)
{
    //Patni-AMT/2010Apr22/Added/JaFT# IR87_CDR_13-Code review
    LPCTSTR FUNC_NAME = _T("CPlanDuplicateSettings::InitPlanDupSettingsFile");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    int32_t l_icurrentmask = 0, l_idefaultmask = 0 , l_iflag = 0 ;
    char l_studyfile [MAX_PATH] = {0};
    int result = ACQ_ERROR_NONE;

    //Check whether file exists by reading the data for the flag
    result = AcqGetDuplicateAll(&l_icurrentmask, &l_iflag,
                                &l_idefaultmask, (char*) l_studyfile);

    //if file does not exist create new file with default settings
    if (ACQ_ERROR_FILE_OPEN == result) {
        if (!InitWithDefaultSettings(f_studyfile)) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("Failed to Initialize Plan Duplicate settings"));
            //Patni-AMT/2010Apr22/Added/JaFT# IR87_CDR_13
            return false;
        }

    } else {
        //Set the read value to member variables
        m_plandupsettings.mask = l_icurrentmask ;
        m_plandupsettings.defaultmask = l_idefaultmask ;
        m_plandupsettings.flag = l_iflag ;

        //File Exists so copy the current study path
        char* l_study = CPqmUtilities::ConvertCHAR(f_studyfile);

        //Patni-AMT/2010Apr22/Added/JaFT# IR87_CDR_15-Code review
        if (NULL != l_study) {
            strcpy(m_plandupsettings.studyfile, l_study);
            result =  AcqPutDuplicateStudy(l_study);

        } else {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("pointer l_study is NULL"));
            return false;
        }

        if (ACQ_ERROR_NONE != result) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("Error occurred while writing study path in file"));

            //Patni+RAJ/2010Apr20/Added/JaFT/IR87_CDR_15
            DEL_PTR_ARY(l_study);
        }

        //if flag value is 1 then copy default mask to current mask
        if (1 == l_iflag) {
            result =  AcqPutDuplicateMask(l_idefaultmask);

            if (ACQ_ERROR_NONE != result) {
                PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                          _T("Error occurred while setting current mask to default"));
            }

            m_plandupsettings.mask = l_idefaultmask ;
        }

        //Patni+RAJ/2010Apr20/Added/JaFT/IR87_CDR_15
        DEL_PTR_ARY(l_study);
    }

    return true;
}

//**************************************************************************
//Method Name   : ReadPlanDuplicateSettings
//Author        : PATNI\DKH
//Purpose       : To read plan duplicate settings
//**************************************************************************
bool CPlanDuplicateSettings::ReadPlanDuplicateSettings(
    VARIANT** f_data
)
{
    LPCTSTR FUNC_NAME = _T("CPlanDuplicateSettings::ReadPlanDuplicateSettings");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    PlanDupSettings* l_planduplicatesettings = new PlanDupSettings;

    if (NULL == l_planduplicatesettings) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("pointer l_planduplicatesettings is NULL"));
        *f_data = NULL;
        return false;
    }

    memset(l_planduplicatesettings, 0, sizeof(PlanDupSettings));

    l_planduplicatesettings->mask = m_plandupsettings.mask;
    l_planduplicatesettings->flag = m_plandupsettings.flag;
    l_planduplicatesettings->defaultmask = m_plandupsettings.defaultmask;

    strcpy(l_planduplicatesettings->studyfile, m_plandupsettings.studyfile);


    if (NULL != f_data) {
        (*f_data)->byref = l_planduplicatesettings;

    } else {

        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("pointer fdata is NULL"));
        return false;
    }

    return true;
}
//**************************************************************************
//Method Name   : WritePlanDuplicateMask
//Author        : PATNI\DKH
//Purpose       : To write plan duplicate settings
//**************************************************************************
bool CPlanDuplicateSettings::WritePlanDuplicateMask(const long f_imask , const BOOL f_bsaveflag)
{
    LPCTSTR FUNC_NAME = _T("CPlanDuplicateSettings::WritePlanDuplicateMask");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));

    int l_result = ACQ_ERROR_NONE ;

    // set the current mask in file
    m_plandupsettings.mask = f_imask;
    l_result = AcqPutDuplicateMask(f_imask);

    if (ACQ_ERROR_NONE != l_result) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Error occurred while writing current mask in file"));
        return false;
    }

    // save the default mask if f_bsaveflag id TRUE
    if (TRUE == f_bsaveflag) {
        m_plandupsettings.defaultmask = f_imask;
        l_result = AcqPutDuplicateDefaultMask(f_imask);

        if (ACQ_ERROR_NONE != l_result) {
            PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                      _T("Error occurred while writing default mask in file"));
            return false;
        }
    }

    return true ;
}

//**************************************************************************
//Method Name   : InitWithDefaultSettings
//Author        : PATNI\DKH
//Purpose       : To initialize default settings
//**************************************************************************
bool CPlanDuplicateSettings::InitWithDefaultSettings(const CString& f_studyfile)
{
    LPCTSTR FUNC_NAME = _T("CPlanDuplicateSettings::InitWithDefaultSettings");
    PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME, _T("Trace"));


    int32_t l_idefaultmask = PL_DUP_MASK_STANDARD;
    m_plandupsettings.mask = PL_DUP_MASK_ORI_VEC | PL_DUP_MASK_OFF_VEC ;
    m_plandupsettings.defaultmask = PL_DUP_MASK_ORI_VEC | PL_DUP_MASK_OFF_VEC ;
    int l_result             = ACQ_ERROR_NONE;

    //put the default mask in the file
    l_result = AcqPutDuplicateDefaultMask(l_idefaultmask);

    if (ACQ_ERROR_NONE != l_result) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Error occurred while writing default mask in file"));
        m_plandupsettings.defaultmask = PL_DUP_MASK_ORI_VEC | PL_DUP_MASK_OFF_VEC ;
        return false;
    }

    int32_t l_icurrentmask = PL_DUP_MASK_STANDARD;
    //put the current mask in the file
    l_result = AcqPutDuplicateMask(l_icurrentmask);

    if (ACQ_ERROR_NONE != l_result) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Error occurred while writing current mask in file"));
        m_plandupsettings.mask = PL_DUP_MASK_ORI_VEC | PL_DUP_MASK_OFF_VEC ;
        return false;
    }

    int32_t l_iflag = 1;
    //put the flag in the file
    l_result = AcqPutDuplicateFlag(l_iflag);

    if (ACQ_ERROR_NONE != l_result) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Error occurred while writing flag in file"));
        m_plandupsettings.flag = 1;
        return false;
    }

    //put the current study file in the path
    //+Patni+RAJ/2010Apr20/Added/JaFT/IR87_CDR_16
    /* char* l_study = NULL ;
     l_study = CPqmUtilities::ConvertCHAR(f_studyfile);*/
    char* l_study = CPqmUtilities::ConvertCHAR(f_studyfile);

    if (!l_study) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("pointer l_study is NULL"));
        return false;
    }

    //-Patni+RAJ/2010Apr20/Added/JaFT/IR87_CDR_16
    strcpy(m_plandupsettings.studyfile, l_study);
    l_result = AcqPutDuplicateStudy(l_study);

    if (ACQ_ERROR_NONE != l_result) {
        PQM_TRACE(USER_FUNC_MARKER, FUNC_NAME,
                  _T("Error occurred while writing study path in file"));
    }

    DEL_PTR_ARY(l_study);  //Patni+RAJ/2010Apr20/Added/JaFT/IR87_CDR_16

    return l_result == ACQ_ERROR_NONE ? true : false;
}
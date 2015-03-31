//Redmine-783
//Redmine-780
// PlanDuplicateSettings.h: interface for the CPlanDuplicateSettings class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_PLANDUPLICATE_SETTINGS_H__
#define __SM_IPCLIB_PLANDUPLICATE_SETTINGS_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>

class CPlanDuplicateSettings : private MR::ACQ::SM::COMMON::NonCopyable<CPlanDuplicateSettings>
{
public:
    //Constructor / Destrcutor
    CPlanDuplicateSettings();
    virtual ~CPlanDuplicateSettings();

    bool InitPlanDupSettingsFile(const CString& f_studyfile);
    bool ReadPlanDuplicateSettings(VARIANT** f_data);
    bool WritePlanDuplicateMask(const long f_imask , const BOOL f_bsaveflag = false);

private:

    PlanDupSettings m_plandupsettings;

    bool InitWithDefaultSettings(const CString& f_studyfile);

};

#endif // #ifndef __SM_IPCLIB_PLANDUPLICATE_SETTINGS_H__

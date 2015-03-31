//Redmine-780
// ILicenseInfo.h: interface for the ILicenseInfo class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SM_IPCLIB_ILICENSEINFO_H__
#define __SM_IPCLIB_ILICENSEINFO_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>

class ILicenseInfo : private MR::ACQ::SM::COMMON::NonCopyable<ILicenseInfo>
{
public:
    ILicenseInfo();
    virtual ~ILicenseInfo();

    virtual bool_t GetdBdtLicense() const = 0;
    virtual bool_t GetIECEdition() const = 0;
    virtual bool_t GetdBdt2002_2_License() const = 0;
    virtual bool_t GetdBdt2002_1_License() const = 0;
    virtual bool_t GetSAR02_01_License() const = 0;
    virtual bool_t GetSAR02_02_License() const = 0;
    //+ Patni-NP/2009Aug17/Added/MVC003302
    virtual bool_t GetNRFilter_License() const = 0;
    //- Patni-NP/2009Aug17/Added/MVC003302
    //Patni-NP/2009Aug21/Added/License Check
    virtual bool_t GetMrapv2002_License() const = 0;

};

#endif // #ifndef __SM_IPCLIB_ILICENSEINFO_H__ 

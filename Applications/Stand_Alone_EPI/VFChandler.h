// VFChandler.h: interface for the CVFChandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VFCHANDLER_H__51671668_1720_4107_BB2A_57D59AD4B0F7__INCLUDED_)
#define AFX_VFCHANDLER_H__51671668_1720_4107_BB2A_57D59AD4B0F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#ifdef V35M
extern "C" {
#include    <tami/vfStudy/vfstudy.h>
#include    <tami/vfStudy/vf_appcodes.h>
#include    <tami/libvf/vf_errors.h>
#include    <tami/common/study_id.h>
#include    <tami/common/errors.h>
#include <tami/libvf/varfields.h>
};
#endif

class CVFChandler
{
public:
    CVFChandler();
    virtual ~CVFChandler();
    CString StudyFileName() const;
    void StudyFileName(CString val);
    virtual int OpenDBFile();
    virtual int CloseDBFile();
protected:
    CString m_csStudyFileName;
    VfDBHandle_t m_vfd;
};

#endif // !defined(AFX_VFCHANDLER_H__51671668_1720_4107_BB2A_57D59AD4B0F7__INCLUDED_)

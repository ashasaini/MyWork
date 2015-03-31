// ImgFileHandler.h: interface for the CImgFileHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMGFILEHANDLER_H__86114DE7_B12D_40B6_BA6B_9E40953A20C4__INCLUDED_)
#define AFX_IMGFILEHANDLER_H__86114DE7_B12D_40B6_BA6B_9E40953A20C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#ifndef NORMAL
#define NORMAL          0
#endif

#ifndef ERROR
#define ERROR           -1
#endif
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
#include <IO.H>
#include <math.h>

class CImgFileHandler
{
public:
    CImgFileHandler();
    virtual ~CImgFileHandler();
    CString getStudyName() const;// { return m_csStudyName; }
    void setStudyName(CString val);// { m_csStudyName = val; }

protected:
    CString m_csStudyName;
};

#endif // !defined(AFX_IMGFILEHANDLER_H__86114DE7_B12D_40B6_BA6B_9E40953A20C4__INCLUDED_)

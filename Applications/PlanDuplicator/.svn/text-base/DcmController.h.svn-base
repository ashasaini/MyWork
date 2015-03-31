// DcmController.h: interface for the CDcmController class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCMCONTROLLER_H__4ECFE347_6F11_49BE_A043_163AC15FE1F1__INCLUDED_)
#define AFX_DCMCONTROLLER_H__4ECFE347_6F11_49BE_A043_163AC15FE1F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "DBWrapper.h"
#include "ImageInfo.h"
#include "PDDicomConverter.h"

#include<vector>
using namespace std;
typedef vector<CImageInfo> DCMDATAVECTOR;

class CDcmController  
{
public:
    CDcmController();
    virtual ~CDcmController();
    int startDICOMConversion(const char *f_cstudyName,const int& f_nportIndex,const CString& f_Dicom_filepath);
    const CImageInfo getImageInfo(const int& f_nImgNum);
    void setImageInfo(CImageInfo& val);
    int fragmentationStr(CString& f_csRefStr);

private:
    DCMDATAVECTOR m_vImageInfo;
    
};

#endif // !defined(AFX_DCMCONTROLLER_H__4ECFE347_6F11_49BE_A043_163AC15FE1F1__INCLUDED_)

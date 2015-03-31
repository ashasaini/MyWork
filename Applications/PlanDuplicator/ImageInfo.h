// ImageInfo.h: interface for the CImageInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEINFO_H__DED8C152_B542_473F_8591_8A83EB35512F__INCLUDED_)
#define AFX_IMAGEINFO_H__DED8C152_B542_473F_8591_8A83EB35512F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CImageInfo  
{
private:
    CString m_csStudyLOID;
    CString m_csSeriesLOID; 
    CString m_csImageLOID;
    CString m_csDcmFileName;
    
    
public:
    CImageInfo();
    virtual ~CImageInfo();
    const CString getImageLOID(void);
    void setImageLOID(CString& val);
    const CString getSeriesLOID(void);
    void setSeriesLOID(CString& val);
    const CString getStudyLOID(void);
    void setStudyLOID(CString& val);
    const CString getDcmFileName(void);
    void setDcmFileName(CString& val);

};

#endif // !defined(AFX_IMAGEINFO_H__DED8C152_B542_473F_8591_8A83EB35512F__INCLUDED_)

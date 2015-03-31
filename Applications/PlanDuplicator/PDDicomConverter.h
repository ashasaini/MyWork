// PDDicomConverter.h: interface for the CPDDicomConverter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PDDICOMCONVERTER_H__3E650BFD_10B1_411B_BB01_44BA6C67A0BF__INCLUDED_)
#define AFX_PDDICOMCONVERTER_H__3E650BFD_10B1_411B_BB01_44BA6C67A0BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "groupdefs.h"
#include "dataTypes.h"

class CvfcDBMgr;
class CPDDicomFileWriter;
class CImageInfo;
class CPDDicomConverter  
//vilas..rev..this class is renamed to CDCMController in New design. 
//both classes are currently present in code
{
public:
    CPDDicomConverter();
    virtual ~CPDDicomConverter();
    int GetDcmErrorcode(void);
    int WriteInDICOMFile(char *f_finalPath,CImageInfo * f_studyName);
    CPDDicomConverter(CPDDicomConverter&);
    CPDDicomConverter& operator = (CPDDicomConverter&);
private:
    void freeworkbuf(void);
    int freeMemoryOfGroup(dataElementT *f_groupElement,const int f_numElements);
    unsigned int GetGroupDataSize(dataElementT * f_groupElement,int f_numElements); //AN
    int initGroup0002(Group0002T &);
    int initGroup0008(Group0008T &);
    int initGroup0010(Group0010T &);
    int initGroup0018(Group0018T &);
    int initGroup0020(Group0020T &);
    int initGroup0028(Group0028T &);
    int initGroup0038(Group0038T &);
    int initGroup7FE0(Group7FE0T &);
    int m_numElementsInGroup02 ;
    int m_numElementsInGroup08 ;
    int m_numElementsInGroup10 ;
    int m_numElementsInGroup20 ;
    int m_numElementsInGroup28 ;
    int m_numElementsInGroup38 ;
    int m_numElementsInGroup18 ;
    int m_numElementsInGroup7FE0;
    int m_dcmError_code;
    CPDDicomFileWriter * m_dcm_writer;
};

#endif // !defined(AFX_PDDICOMCONVERTER_H__3E650BFD_10B1_411B_BB01_44BA6C67A0BF__INCLUDED_)

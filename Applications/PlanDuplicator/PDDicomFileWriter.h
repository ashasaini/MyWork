// PDDicomFileWriter.h: interface for the CPDDicomFileWriter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PDDICOMFILEWRITER_H__624AD3A9_35F4_49A5_B14E_D7A04F4A5DC8__INCLUDED_)
#define AFX_PDDICOMFILEWRITER_H__624AD3A9_35F4_49A5_B14E_D7A04F4A5DC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "groupdefs.h"
class CPDDicomFileWriter  
{
public:
	int AllocWorkBuff(unsigned int f_nsize); //AN
    CPDDicomFileWriter();
    virtual ~CPDDicomFileWriter();
    CPDDicomFileWriter(CPDDicomFileWriter&);
    CPDDicomFileWriter& operator = (CPDDicomFileWriter&);

    int freeworkbuf(void);
    int writeToFile(void); 
    int writeDICOMHeader(void);
    int WriteMetaElementsGroup(void);
    int SetFilePath(char *f_filePath);
    int WriteGroupToFile(dataElementT *f_grouptag,const int f_numElementsInGroup);
private:
    char *m_filePath;
    char* m_cpWorkbuf;
    int m_ndataSize;

};

#endif // !defined(AFX_PDDICOMFILEWRITER_H__624AD3A9_35F4_49A5_B14E_D7A04F4A5DC8__INCLUDED_)

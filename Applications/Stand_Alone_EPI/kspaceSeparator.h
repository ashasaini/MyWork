// kspaceSeparator.h: interface for the CkspaceSeparator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KSPACESEPARATOR_H__7B354863_FD48_461D_9F71_A6067534A058__INCLUDED_)
#define AFX_KSPACESEPARATOR_H__7B354863_FD48_461D_9F71_A6067534A058__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DataSeparator.h"

class CkspaceSeparator : public CDataSeparator
{
public:
    CkspaceSeparator();
    virtual ~CkspaceSeparator();
    int separateDataTemplateAndKspace( void *f_cEntireData,
                                       void *f_cRawDataBuffer,
                                       void *f_cTemplateDataBuffer1,
                                       void *f_cTemplateDataBuffer2,
                                      int &f_nWidth,
                                      int &f_nMultiplier,
                                      int &numBits, int &flag, int &f_nNumrepetition,int f_nDataTypeSize);
    int mergeDataTemplateAndKspace(double *f_cFinalBuffer,
                                   double *f_cRawDataBuffer,
                                   double *f_cTemplateDataBuffer1,
                                   double *f_cTemplateDataBuffer2,
                                   int f_nWidth,
                                   int f_nHeight);


};

#endif // !defined(AFX_KSPACESEPARATOR_H__7B354863_FD48_461D_9F71_A6067534A058__INCLUDED_)

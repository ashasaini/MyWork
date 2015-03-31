// EvenOddSeparator.h: interface for the CEvenOddSeparator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVENODDSEPARATOR_H__BBC6D2B1_8B03_4056_AFE6_4DB8CAD3AE4C__INCLUDED_)
#define AFX_EVENODDSEPARATOR_H__BBC6D2B1_8B03_4056_AFE6_4DB8CAD3AE4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DataSeparator.h"

class CEvenOddSeparator : public CDataSeparator  
{
public:
	CEvenOddSeparator();
	virtual ~CEvenOddSeparator();
    int separateDataEvenOdd(double *f_dDataBuffer, double *f_dEvenDataBuffer, double *f_dOddDataBuffer, int f_nWidth, int f_nHeight);
    int mergeDataEvenOdd(double *&f_dDataBuffer, double *f_dEvenDataBuffer, double *f_dOddDataBuffer, int f_nWidth, int f_nHeight);
};

#endif // !defined(AFX_EVENODDSEPARATOR_H__BBC6D2B1_8B03_4056_AFE6_4DB8CAD3AE4C__INCLUDED_)

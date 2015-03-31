// TimeShiftLineFit.h: interface for the CTimeShiftLineFit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMESHIFTLINEFIT_H__B11261EC_DE90_4FD9_A1DE_A1BD5972B0C7__INCLUDED_)
#define AFX_TIMESHIFTLINEFIT_H__B11261EC_DE90_4FD9_A1DE_A1BD5972B0C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ShiftPickStrategy.h"

class CTimeShiftLineFit : public CShiftPickStrategy  
{
public:
	CTimeShiftLineFit();
	virtual ~CTimeShiftLineFit();
    void linearLeastSqrFit (flt32_t *xP, /* to x coordinates */ 
                  flt32_t *yP, /* to y coordinates */ 
                  flt32_t *a0P, /* to 0-order coefficient of the fit */ 
                  flt32_t *a1P, /* to 1-order coefficient of the fit */ 
                  int32_t n /* number of points */ );
    int CalculateDataShift(float*x, float A, 
                           float B, void* f_nshiftA, 
                           int n, int* f_npeakIndex, 
                           int f_nwidth, int f_nflag);
};

#endif // !defined(AFX_TIMESHIFTLINEFIT_H__B11261EC_DE90_4FD9_A1DE_A1BD5972B0C7__INCLUDED_)

// TimeShift.h: interface for the CTimeShift class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMESHIFT_H__917F1D45_A426_4D09_8E93_EA2094111782__INCLUDED_)
#define AFX_TIMESHIFT_H__917F1D45_A426_4D09_8E93_EA2094111782__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ShiftPickStrategy.h"

class CTimeShift : public CShiftPickStrategy  
{
public:
	CTimeShift();
	virtual ~CTimeShift();
    int calculateDataShift(int f_nHeight,/*inP*/
                           int f_nWidth, /*inP*/
                           int *f_nPeakIndex,/*inP*/
                           int *f_nDataShift/*oP*/);
};

#endif // !defined(AFX_TIMESHIFT_H__917F1D45_A426_4D09_8E93_EA2094111782__INCLUDED_)

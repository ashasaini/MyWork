// ShiftPickStrategy.h: interface for the CShiftPickStrategy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHIFTPICKSTRATEGY_H__9B0E12E1_4512_4B6F_A8E2_402CCFD4F388__INCLUDED_)
#define AFX_SHIFTPICKSTRATEGY_H__9B0E12E1_4512_4B6F_A8E2_402CCFD4F388__INCLUDED_

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
#include <math.h>
#include "CommonDefinations.h"

class CShiftPickStrategy  
{
public:
	CShiftPickStrategy();
	virtual ~CShiftPickStrategy();
    int FindPeakAndPeakData(f64complex_t* DataBuffer,/*inP*/
                         int f_nHeight,/*inP*/ 
                         int l_nWidth,/*inP*/ 
                         f64complex_t* f_dPeakData, /*oP*/
                         int *f_nPeakIndex /*oP*/);
    int ShiftData( int f_nHeight,/*inP*/ 
                   int f_nWidth, /*inP*/ 
                   int *f_nDataShift,/*inP*/ 
                   f64complex_t *f_dKspaceDataBuffer/*oP*/ );
};

#endif // !defined(AFX_SHIFTPICKSTRATEGY_H__9B0E12E1_4512_4B6F_A8E2_402CCFD4F388__INCLUDED_)

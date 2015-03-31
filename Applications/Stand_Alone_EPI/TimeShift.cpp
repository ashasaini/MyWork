// TimeShift.cpp: implementation of the CTimeShift class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "epi.h"
#include "TimeShift.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTimeShift::CTimeShift()
{

}

CTimeShift::~CTimeShift()
{

}

int CTimeShift::calculateDataShift( int f_nHeight,/*inP*/ 
                                   int f_nWidth, /*inP*/ 
                                   int *f_nPeakIndex,/*inP*/ 
                                   int *f_nDataShift/*oP*/ )
{
    if (NULL == f_nPeakIndex ||
        NULL == f_nDataShift)
    {
        return EPI_ERROR_NULL_FUNCPARAMS;
    }
    int    l_nDataCenter = 0;
    int    i;
    l_nDataCenter = f_nWidth / 2;
    for(i = 0; i < f_nHeight; i++) {
        f_nDataShift[i] = -f_nPeakIndex [i] + (l_nDataCenter - 1);
    } 
    return E_NO_ERROR;
}


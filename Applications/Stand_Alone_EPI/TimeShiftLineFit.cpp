// TimeShiftLineFit.cpp: implementation of the CTimeShiftLineFit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "epi.h"
#include "TimeShiftLineFit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTimeShiftLineFit::CTimeShiftLineFit()
{

}

CTimeShiftLineFit::~CTimeShiftLineFit()
{

}

void CTimeShiftLineFit::linearLeastSqrFit(flt32_t *xP, /* to x coordinates */ 
                                          flt32_t *yP, /* to y coordinates */ 
                                          flt32_t *a0P, /* to 0-order coefficient of the fit */ 
                                          flt32_t *a1P, /* to 1-order coefficient of the fit */ 
                                          int32_t n /* number of points */ )
{
    flt32_t   s   = (flt32_t)n;
    flt32_t   sxs = 0.0;
    flt32_t   sys = 0.0;
    flt32_t   stt = 0.0;
    flt32_t   sty = 0.0;
    
    a0P[0] = 0.0;
    a1P[0] = 0.0;

    if (n <= 1)  return;

    /*
     * Sxs = Sum(xi)/S;
     */
     
    {
	flt32_t  *xiP = xP;
	int32_t   m   = n;
	
	do
	{
	    flt32_t xi= 0.0;

	    xi   = *xiP++;
	    
	    sxs += xi;
	} while (--m);
    }
    sxs /= s;
    
    /*
     * ti = (xi - Sx/S), Stt = Sum(ti*ti), Sys = Sum(yi)/S, Sty = Sum(ti*yi)
     */

    {
	flt32_t  *xiP = xP;
	flt32_t  *yiP = yP;
	int32_t   m   = n;
	
	do
	{
	    flt32_t ti;
	    flt32_t yi;

	    ti   = *xiP++;
	    yi   = *yiP++;

	    ti  -= sxs;
	    stt += ti * ti;
	    sys += yi;
	    sty += ti * yi;
	} while (--m);
    }
    sys /= s;
    
    /*
     * a1 = sty/Stt
     * a0 = (Sy/S) - (Sx/S)(sty/Stt),
     */
    
    a1P[0] = sty / stt;
    a0P[0] = sys - sxs * a1P[0];
}
int CTimeShiftLineFit::CalculateDataShift(float*x, 
                               float A, 
                               float B, 
                               void* f_nshiftA, 
                               int n, int* f_npeakIndex, 
                               int f_nwidth, int f_nflag)
{
    int i;
    int temp, dataCenter;
    float temp_float;
    int *ishiftA;
    float *fshiftA = NULL;
    
    
    for(i = 0; i < n; i++) {
        temp = (int)(A * x[i] + B);		
        if(0 == f_nflag) {
            ishiftA = (int *)f_nshiftA;
            temp = (int)(A * x[i] + B);
            dataCenter = -f_npeakIndex[i] + temp + (f_nwidth / 2) - 1; // For Correct A - method 2
            ishiftA[i] = -f_npeakIndex[i] + dataCenter;
        } else if(1 == f_nflag) {
            fshiftA = (float *)f_nshiftA;
            temp_float = (A * x[i] + B);
            dataCenter = f_nwidth / 2 - 1; //For Correct A - method 3
            fshiftA[i] = -temp_float + dataCenter;
        }
        
    }
    
#if 1
    
    if(1 == f_nflag) {
        float CenterDiff;
        
        CenterDiff = fshiftA[16];
        
        
        for(i = 0; i < n; i++) {
            fshiftA[i] = fshiftA[i] - CenterDiff;
            
        }
    }
    
#endif
    
    return 0;
}

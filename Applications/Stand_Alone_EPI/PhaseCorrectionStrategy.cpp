// PhaseCorrectionStrategy.cpp: implementation of the CPhaseCorrectionStrategy class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "epi.h"
#include "PhaseCorrectionStrategy.h"
#include "TimeShift.h"
#include "TimeShiftLineFit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPhaseCorrectionStrategy::CPhaseCorrectionStrategy()
{

}

CPhaseCorrectionStrategy::~CPhaseCorrectionStrategy()
{

}

int CPhaseCorrectionStrategy::performPhaseCorrection(f64complex_t *f_dTempDataBuff,/*inP*/ 
                                                     int f_nHeight,/*inP*/ 
                                                     int f_nWidth,/*inP*/ 
                                                     int f_nPhaseMethodSelectionFlag,/*inP*/ 
                                                     f64complex_t *f_dKspaceDataBuff /*oP*/ )
{
    f64complex_t  *l_dPeakData = new f64complex_t[f_nHeight];
    int *l_nPeakIndex = new int[f_nHeight];
    int *l_nDataShift = new int[f_nHeight];
    int l_nStatus = E_NO_ERROR;

    switch(f_nPhaseMethodSelectionFlag){
    case TIME_SHIFT:{
            CTimeShift l_TimeShift;
            l_nStatus = l_TimeShift.FindPeakAndPeakData(f_dTempDataBuff,
                                                        f_nHeight,f_nWidth,
                                                        l_dPeakData,l_nPeakIndex);
            if (E_NO_ERROR != l_nStatus)
            {
                return l_nStatus;
            }
            l_nStatus = l_TimeShift.calculateDataShift(f_nHeight,f_nWidth,
                                                       l_nPeakIndex,l_nDataShift);
            if (E_NO_ERROR != l_nStatus)
            {
                return l_nStatus;
            }

            l_nStatus = l_TimeShift.ShiftData(f_nHeight,f_nWidth,l_nDataShift,f_dKspaceDataBuff);
            if (E_NO_ERROR != l_nStatus)
            {
                return l_nStatus;
            }

        }
        break;
    case TIME_SHIFT_LINE_FIT:{
            CTimeShiftLineFit l_TimeLineFit;
            float peakIndex_float[64];
            float *l_xp = NULL;
            float l_A = 0.0;
            float l_B = 0.0;
            int status = 0;    
            int l_nFlag = 0;
            l_nStatus = l_TimeLineFit.FindPeakAndPeakData(f_dTempDataBuff,
                f_nHeight,f_nWidth,
                l_dPeakData,l_nPeakIndex);
            if (E_NO_ERROR != l_nStatus)
            {
                return l_nStatus;
            }
            for(int i = 0 ; i < f_nHeight; i++) {
                peakIndex_float[i] = (float)(l_nPeakIndex[i]);
            }
            l_xp = (float*) malloc(f_nHeight * sizeof(float));
            
            if(NULL == l_xp) {
                status = -1;
                return E_ERROR;
            }
            for(i = 0 ; i < f_nHeight ; i++) {
                l_xp[i] = (float) i + 1;
            }
            l_TimeLineFit.linearLeastSqrFit(l_xp,peakIndex_float,&l_B,&l_A,f_nHeight);
            l_nStatus = l_TimeLineFit.CalculateDataShift(l_xp,l_A,l_B,l_nDataShift,
                                                         f_nHeight,l_nPeakIndex,
                                                         f_nWidth,l_nFlag);
            if (E_NO_ERROR != l_nStatus)
            {
                return l_nStatus;
            }
            
            l_nStatus = l_TimeLineFit.ShiftData(f_nHeight,
                                                f_nWidth,l_nDataShift,
                                                f_dKspaceDataBuff);
            if (E_NO_ERROR != l_nStatus)
            {
                return l_nStatus;
            }

        }
        break;
    default:

        break;
    }

    return E_NO_ERROR;
}

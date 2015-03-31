// ShiftPickStrategy.cpp: implementation of the CShiftPickStrategy class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "epi.h"
#include "ShiftPickStrategy.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShiftPickStrategy::CShiftPickStrategy()
{

}

CShiftPickStrategy::~CShiftPickStrategy()
{

}

int CShiftPickStrategy::FindPeakAndPeakData(f64complex_t* DataBuffer,/*inP*/ 
                                         int f_nHeight,/*inP*/ 
                                         int l_nWidth,/*inP*/ 
                                         f64complex_t* f_dPeakData, /*oP*/ 
                                         int *f_nPeakIndex /*oP*/ )
{
    int l_nRow, l_nCol;
    double l_dTemp = 0.0;
    double l_dAbsValue;
    
    if(NULL == DataBuffer) {
        return EPI_ERROR_NULL_FUNCPARAMS;
    }
    
    for(l_nRow = 0; l_nRow < f_nHeight; l_nRow++) {
        l_dTemp = 0.0f;        
        for(l_nCol = 0; l_nCol < l_nWidth; l_nCol++) {
            l_dAbsValue = sqrt((DataBuffer[l_nRow * l_nWidth + l_nCol].real* 
                DataBuffer[l_nRow * l_nWidth + l_nCol].real)
                + (DataBuffer[l_nRow * l_nWidth + l_nCol].imgn * 
                DataBuffer[l_nRow * l_nWidth + l_nCol].imgn));
            
            
            if(l_dAbsValue > l_dTemp) {
                l_dTemp = l_dAbsValue;
                f_nPeakIndex[l_nRow] = l_nCol;
            }
        }
    }
    for(l_nRow = 0; l_nRow < f_nHeight; l_nRow++) {
        f_dPeakData[l_nRow].real = DataBuffer[l_nRow * l_nWidth + f_nPeakIndex[l_nRow]].real;
        f_dPeakData[l_nRow].imgn = DataBuffer[l_nRow * l_nWidth + f_nPeakIndex[l_nRow]].imgn;
    }
	
    return E_NO_ERROR;
}
int CShiftPickStrategy::ShiftData( int f_nHeight,/*inP*/ 
                          int f_nWidth, /*inP*/ 
                          int *f_nDataShift,/*inP*/ 
                          f64complex_t *f_dKspaceDataBuffer/*oP*/ )
{
    if (NULL == f_dKspaceDataBuffer ||
        NULL == f_nDataShift)
    {
        return EPI_ERROR_NULL_FUNCPARAMS;
    }
    int shift = 0;
    int l_nRow, l_nCol;
    int index = 0;
    
    int l_nRowSize = f_nWidth * sizeof(f64complex_t);
    
    f64complex_t *l_dTmpBuffer = NULL;
    l_dTmpBuffer = (f64complex_t*)malloc(l_nRowSize);
    if(NULL == l_dTmpBuffer) {
        return EPI_ERROR;
    }
    
    ZeroMemory(l_dTmpBuffer,l_nRowSize);
    
    
    for(l_nRow = 0; l_nRow < f_nHeight; l_nRow++) {
        shift = f_nDataShift[l_nRow];  
        ZeroMemory(l_dTmpBuffer,l_nRowSize);
        for(l_nCol = 0; l_nCol < f_nWidth; l_nCol++) {
            index = (l_nCol + shift + f_nWidth) % f_nWidth;
            l_dTmpBuffer[index].real = f_dKspaceDataBuffer[l_nRow * f_nWidth + l_nCol].real;
            l_dTmpBuffer[index].imgn = f_dKspaceDataBuffer[l_nRow * f_nWidth + l_nCol].imgn;
        }
        memcpy(f_dKspaceDataBuffer + (l_nRow * f_nWidth),l_dTmpBuffer,l_nRowSize);
    }
    
    free(l_dTmpBuffer);
    l_dTmpBuffer = NULL;
    
    return E_NO_ERROR;
}

// ImageDataProcessor.cpp: implementation of the CMapColorProcessor class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.

    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#include "stdafx.h"
#include "ImageDataProcessor.h"
#include "EPILogHelper.h"
#include <cmath>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CImageDataProcessor::CImageDataProcessor()
{
}

CImageDataProcessor::~CImageDataProcessor()
{
}

/*************************************************************
* Method:       SetColor
* Description:  This function is used
* Parameter:    short * f_dBckGndImgBuf :
* Parameter:    double * f_dMapImgBuf :
* Parameter:    void * f_dColoredImg :
* Parameter:    int f_nNumOfElements :
* Parameter:    double f_dT1Min :
* Parameter:    double f_dT1Max :
* Parameter:    bool f_bApplyPressed :
* Returns:      bool
*************************************************************/
bool CImageDataProcessor::ConvertTo4_D(double *f_dImgBuf, void *f_d4DImg, int f_nNumOfElements, int WW, int WL)
{
    try {
        if(NULL == f_dImgBuf ||  NULL == f_d4DImg) { //Dont check null for f_dMapImgBuf here
            //log error
            return(false);
        }

        if(NULL != f_dImgBuf) {
            ConvertToRange(f_dImgBuf, f_nNumOfElements, WW);
        }

        double low  = (((256.0 - 256.0 * (WW / 100.0)) / 2.0) / (WW / 100.0));
        double level    = (256.0 * ((WL - 50.0) / 100.0));
        double addval   = -low + level;
        short l_sValue = 0;
        unsigned long *l_ulColoredImg = NULL;
        l_ulColoredImg = (unsigned long*)f_d4DImg;

        for(int l_ni = 0; l_ni < f_nNumOfElements; l_ni++) {
            l_sValue = 0;
            l_sValue = (short)(f_dImgBuf[l_ni] + addval);

            if(l_sValue > UCHAR_MAX) {
                l_sValue = UCHAR_MAX ;
            }

            if(l_sValue <   0) {
                l_sValue = 0 ;
            }

            if(l_ulColoredImg) {
                l_ulColoredImg[l_ni] = (l_sValue << 24) | (l_sValue << 16) | (l_sValue << 8) | (l_sValue << 0) ;//gray scale
            }
        }

        l_ulColoredImg = NULL;//Do not delete this pointer only NULL
        return(true);
    } catch(...) {
        CEPILogHelper::WriteExceptionToLog(__LINE__, __FILE__);
        return false;
    }
}

/*************************************************************
* Method:       GetMinMaxT1ValForBckImage
* Description:  This function is used
* Parameter:    short * f_sImgBuf :
* Parameter:    int f_nNumOfElements :
* Parameter:    short & f_nMinT1 :
* Parameter:    short & f_nMaxT1 :
* Returns:      bool
*************************************************************/
bool CImageDataProcessor::GetMinMaxT1ValForImageShort(short *f_sImgBuf,
        int f_nNumOfElements,
        int &f_nMinT1, int &f_nMaxT1)
{
    try {
        int l_nMinT1 = 0;
        int l_nMaxT1 = 0;

        if(NULL == f_sImgBuf) {
            //log error
            return(false);
        }

        l_nMaxT1 = f_sImgBuf[0];
        l_nMinT1 = f_sImgBuf[0];

        for(int l_ni = 0; l_ni < f_nNumOfElements; l_ni++) {
            if(f_sImgBuf[l_ni] > l_nMaxT1) {
                l_nMaxT1 = f_sImgBuf[l_ni];
            }

            if(f_sImgBuf[l_ni] < l_nMinT1) {
                l_nMinT1 = f_sImgBuf[l_ni];
            }
        }

        f_nMinT1 = l_nMinT1;
        f_nMaxT1 = l_nMaxT1;
    } catch(...) {
        CEPILogHelper::WriteExceptionToLog(__LINE__, __FILE__);
        return false;
    }

    return(true);
}
/*************************************************************
* Method:       ConvertDoubleToShort
* Description:  This function is used
* Parameter:    short * f_sMapImgBufInShort :
* Parameter:    double * f_dMapImgBufInDouble :
* Parameter:    unsigned long f_NumOfElements :
* Returns:      bool
*************************************************************/
bool CImageDataProcessor::ConvertToRange(
    double *f_dImgBufInDouble,
    unsigned long f_NumOfElements,
    int WW
)
{
    try {
        if(NULL == f_dImgBufInDouble || f_NumOfElements <= 0) {
            //log error
            return false;
        }

        double f_dMin = 0.0, f_dMax = 0.0;
        GetMinMaxT1ValForImageDouble(f_dImgBufInDouble, f_NumOfElements, f_dMin, f_dMax);
        AdjustRange(f_dImgBufInDouble, f_NumOfElements, f_dMin, f_dMax, WW);
        return(true);
    } catch(...) {
        CEPILogHelper::WriteExceptionToLog(__LINE__, __FILE__);
        return false;
    }
}


/*************************************************************
* Method:       AdjustMapRange
* Description:  This function is used
* Parameter:    double * f_dMapImgBuf :
* Parameter:    short * f_sMapImgBufInShort :
* Parameter:    int f_nNumOfElements :
* Parameter:    double f_dT1Min :
* Parameter:    double f_dT1Max :
* Parameter:    bool f_bApplyPressed :
* Returns:      bool
*************************************************************/
bool CImageDataProcessor::AdjustRange(double *f_dMapImgBuf,
                                      int f_nNumOfElements,
                                      double f_dT1Min,
                                      double f_dT1Max,
                                      int WW
                                     )
{
    try {
        if((NULL == f_dMapImgBuf) || f_nNumOfElements <= 0) {
            //log error
            return(false);
        }

        int l_ni = 0;
        double dev  = (f_dT1Max - f_dT1Min) / (double)256.0;
        dev = dev * (WW / 100.0);

        if(fabs(dev) < 0.000000001) {
            dev = (f_dT1Max - f_dT1Min) / (double)256.0;
        }

        for(l_ni = 0; l_ni < f_nNumOfElements; l_ni++) {
            f_dMapImgBuf[l_ni] = (((f_dMapImgBuf[l_ni] - f_dT1Min)) / (dev)) ;
        }

        return(true);
    } catch(...) {
        CEPILogHelper::WriteExceptionToLog(__LINE__, __FILE__);
        return false;
    }
}

bool CImageDataProcessor::GetMinMaxT1ValForImageDouble(double *f_sImgBuf, int f_nNumOfElements, double &f_nMinT1, double &f_nMaxT1)
{
    try {
        double l_nMinT1 = 0;
        double l_nMaxT1 = 0;

        if(NULL == f_sImgBuf) {
            //log error
            return(false);
        }

        l_nMaxT1 = f_sImgBuf[0];
        l_nMinT1 = f_sImgBuf[0];

        for(int l_ni = 0; l_ni < f_nNumOfElements; l_ni++) {
            if(f_sImgBuf[l_ni] > l_nMaxT1) {
                l_nMaxT1 = f_sImgBuf[l_ni];
            }

            if(f_sImgBuf[l_ni] < l_nMinT1) {
                l_nMinT1 = f_sImgBuf[l_ni];
            }
        }

        f_nMinT1 = l_nMinT1;
        f_nMaxT1 = l_nMaxT1;
    } catch(...) {
        CEPILogHelper::WriteExceptionToLog(__LINE__, __FILE__);
        return false;
    }

    return(true);
}

///////////////////////////////////////////////////////////////////////////////
// End of file
//////////////
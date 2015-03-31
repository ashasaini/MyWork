// ImageDataProcessor.h: interface for the CMapColorProcessor class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.

    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#if !defined(AFX_IMAGEDATAPROCESSOR_H__84B2A3D9_FB04_4839_86C1_A69B6BC74A41__INCLUDED_)
#define AFX_IMAGEDATAPROCESSOR_H__84B2A3D9_FB04_4839_86C1_A69B6BC74A41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CImageDataProcessor
{
private:
    bool GetMinMaxT1ValForImageShort(short *f_sImgBuf, int f_nNumOfElements, int &f_nMinT1, int &f_nMaxT1);
    bool GetMinMaxT1ValForImageDouble(double *f_sImgBuf, int f_nNumOfElements, double &f_nMinT1, double &f_nMaxT1);

    bool AdjustRange(double *f_dMapImgBuf, int f_nNumOfElements, double f_dT1Min, double f_dT1Max, int);

public:
    CImageDataProcessor();
    virtual ~CImageDataProcessor();
    bool ConvertToRange(double *f_dMapImgBufInDouble, unsigned long f_NumOfElements, int);
    bool ConvertTo4_D(double *f_dImgBuf, void *f_d4DImg, int f_nNumOfElements, int, int);
};

#endif // !defined(AFX_MAPCOLORPROCESSOR_H__84B2A3D9_FB04_4839_86C1_A69B6BC74A41__INCLUDED_)

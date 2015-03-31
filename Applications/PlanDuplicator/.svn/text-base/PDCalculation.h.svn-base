// PDCalculation.h: interface for the CPDCalculation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PDCALCULATION_H__9347108B_9C86_4D18_B5F8_4554F0F1AE94__INCLUDED_)
#define AFX_PDCALCULATION_H__9347108B_9C86_4D18_B5F8_4554F0F1AE94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "dataTypes.h"
class CSmartVOIData;
class CPDCoordinates;
class CvfcDBMgr;
class CPDCalculation  
{
public:
    explicit CPDCalculation(CvfcDBMgr *);
    virtual ~CPDCalculation();
    CPDCalculation(CPDCalculation& f_calc);
    CPDCalculation& operator = (CPDCalculation& f_calc);
    int CalculateCouchOffset(const int &f_nprot_index,CPDCoordinates * f_pdata);
    int CalculateSliceThickness(PlaneAxis f_plane, const int &prot_index, CPDCoordinates* f_pdata);
    int CalculateSliceGap(PlaneAxis f_plane, const int &prot_index, CPDCoordinates* f_pdata);
    int CalculateSliceNum(PlaneAxis f_plane, const int &prot_index, CPDCoordinates* f_pdata);
private:
    CvfcDBMgr * m_vfcmgr;
};

#endif // !defined(AFX_PDCALCULATION_H__9347108B_9C86_4D18_B5F8_4554F0F1AE94__INCLUDED_)

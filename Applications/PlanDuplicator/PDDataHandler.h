// PDDataHandler.h: interface for the CPDDataHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PDDATAHANDLER_H__65068A97_81FF_40A2_83F0_8347546C0F81__INCLUDED_)
#define AFX_PDDATAHANDLER_H__65068A97_81FF_40A2_83F0_8347546C0F81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PDCoordinates.h"

#include<vector>
using namespace std;

typedef vector<CPDCoordinates*> DATAVECTOR;

class CPDDataHandler  
{
public:
    CPDDataHandler();
    CPDDataHandler(CPDDataHandler &f_handler);
    virtual ~CPDDataHandler();
    CPDDataHandler& operator = (CPDDataHandler& f_controller);

    int SetVantageDataVector(DATAVECTOR & f_vec_Vantage);
    int SetDicomDataVector(DATAVECTOR & f_vec_Dicom);
    
    f32vec3_t GetVantageData(int f_nSlicePlane,PDData_t f_nPlaneData) const;
    CPDCoordinates * GetVantageData(int f_nSlicePlane);
    f32vec3_t    GetDicomData(int f_nSlicePlane,PDData_t f_nPlaneData) const;
    /*char**/ CString GetVantageSlicePlane(int f_nSlicePlane);
    /*char**/ CString GetDicomSlicePlane(int f_nSlicePlane);
    int GetDataSize(void);
    void ConvertDICOMAxis(const f32vec3_t &f_pfin_vector,f32vec3_t& f_pfout_vector,int f_nflag);

private:
    DATAVECTOR m_vec_VantageData;
    DATAVECTOR m_vec_DicomData; 
    
};

#endif // !defined(AFX_PDDATAHANDLER_H__65068A97_81FF_40A2_83F0_8347546C0F81__INCLUDED_)



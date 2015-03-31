// PDDataFileReader.h: interface for the CPDDataFileReader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PDDATAFILEREADER_H__DC612C7E_CFBE_42B0_94F8_C028236834C6__INCLUDED_)
#define AFX_PDDATAFILEREADER_H__DC612C7E_CFBE_42B0_94F8_C028236834C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "PDCoordinates.h"
#include<vector>
using namespace std;
class CPDDataFileReader  
{
public:
    CPDDataFileReader();
    ~CPDDataFileReader();

    //Read Varfield file & store data in vector
    int ReadVarfieldDataFile(const char *f_ptr,vector<CPDCoordinates*>& f_VantageDataVector,
        PDAppMode_t f_app_mode);
    int getErrorCode(void);
    
private:
    int m_error_code;
};

#endif // !defined(AFX_PDDATAFILEREADER_H__DC612C7E_CFBE_42B0_94F8_C028236834C6__INCLUDED_)

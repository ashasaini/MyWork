// DBWrapper.h: interface for the CDBWrapper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBWRAPPER_H__275BA8A2_978B_463B_A9D6_244CE95EFCF4__INCLUDED_)
#define AFX_DBWRAPPER_H__275BA8A2_978B_463B_A9D6_244CE95EFCF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include<vector>
using namespace std;
class CDBWrapper  
{
public:
    CDBWrapper();
    virtual ~CDBWrapper();
    int getTotalNumImages(const char *f_cstudyName,const int& f_nportIndex, vector<CString> &f_csFileName,int *f_nNumImages);

};

#endif // !defined(AFX_DBWRAPPER_H__275BA8A2_978B_463B_A9D6_244CE95EFCF4__INCLUDED_)

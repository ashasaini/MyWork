// EPILogHelper.h: interface for the T1MapLogHelper class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.

    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#if !defined(AFX_EPILOGHELPER_H__03CBC99A_09CD_4F96_8075_1260CDCFE8EE__INCLUDED_)
#define AFX_EPILOGHELPER_H__03CBC99A_09CD_4F96_8075_1260CDCFE8EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <iostream>
#include <fstream>

class CEPILogHelper
{
public:
    static void WriteToErrorLog(CString f_smsg,
                                int f_Line_No = 0,
                                CString f_csFile_Name = _T(""));
    static void WriteExceptionToLog(int f_Line_No = 0,
                                    CString f_csFile_Name = _T(""));
    static void DeleteInstance();
    static bool CreateInstance();

    virtual ~CEPILogHelper();
    CEPILogHelper();//singleton
private:
    static FILE* m_pEPILogFile;
    static CEPILogHelper * m_EPILogHelper;
    static std::ofstream m_pEPIErrorLogFile;
   

};

#endif // !defined(AFX_EPILOGHELPER_H__03CBC99A_09CD_4F96_8075_1260CDCFE8EE__INCLUDED_)

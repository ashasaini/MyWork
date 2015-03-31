// Reconstruction.h: interface for the CReconstruction class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.
  
    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#if !defined(AFX_RECONSTRUCTION_H__738E2043_AF9A_4F29_AC7A_2C5DA0A91EE2__INCLUDED_)
#define AFX_RECONSTRUCTION_H__738E2043_AF9A_4F29_AC7A_2C5DA0A91EE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CReconstruction
{
private:
    CString m_csProcessName;
    int m_nProtocolNumber;
    CString m_csStudyFileName;
public:
    CReconstruction();
    virtual ~CReconstruction();
    int performReconstruction(CString &f_csStudyFileName, int f_nProtocolNum, int f_nDataForm);
    void setProtocolNumber(const int &f_nProtNo, const CString &f_csStudyFileName); // { m_csloids = val; }
    int getProtocolNumber(int &f_nProtNo, CString &f_csStudyFileName);
};

#endif // !defined(AFX_RECONSTRUCTION_H__738E2043_AF9A_4F29_AC7A_2C5DA0A91EE2__INCLUDED_)

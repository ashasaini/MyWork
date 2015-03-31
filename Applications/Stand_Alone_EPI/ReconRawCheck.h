// ReconRawCheck.h: interface for the CReconRawCheck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECONRAWCHECK_H__DFFF86EC_AB98_477E_9C19_0CCC282D0C70__INCLUDED_)
#define AFX_RECONRAWCHECK_H__DFFF86EC_AB98_477E_9C19_0CCC282D0C70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CReconRawCheck  
{

public:
	CReconRawCheck();
    CReconRawCheck& operator = (CReconRawCheck& f_RawCheck_ptr);
	virtual ~CReconRawCheck();
    BOOL RawCheck(CString& f_engineHost ,
        CString& f_csStudyFileName , 
        int f_nProtocolNo ,
        CString &f_RawCheckStatusFilePath); 

private:
    HANDLE m_hChildStd_IN_Rd ;
    HANDLE m_hChildStd_IN_Wr ;
    HANDLE m_hChildStd_OUT_Rd ;
    HANDLE m_hChildStd_OUT_Wr ;
    HANDLE m_hInputFile ;

    BOOL RawCheckCreateProcess(CString&f_engineHost ,
        CString& f_csStudyFileName , 
        int f_nProtocolNo); 
    void RawCheckWriteToPipe(void); 
    BOOL RawCheckReadFromPipe(void);
    void RawCheckCleanUp();
};

#endif // !defined(AFX_RECONRAWCHECK_H__DFFF86EC_AB98_477E_9C19_0CCC282D0C70__INCLUDED_)

// AutoLocatorAppController.h: interface for the CAutoLocatorAppController class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUTOLOCATORAPPCONTROLLER_H__6B99A84C_F6EF_49C6_81D7_6A6A250F9D52__INCLUDED_)
#define AFX_AUTOLOCATORAPPCONTROLLER_H__6B99A84C_F6EF_49C6_81D7_6A6A250F9D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <libStudyManager/NonCopyable.h>

class CAutoLocatorAppController: private MR::ACQ::SM::COMMON::NonCopyable<CAutoLocatorAppController>
{
    HANDLE m_processtool_thread;
    unsigned int  m_thread_id;
    int m_process_id;
    CString m_prot_index_workg_mode;
    bool m_load_gl_flag;			//flag to check whether load gl has been sent and reply is received
    HANDLE m_easytech_started_event;

    //functions
    unsigned int  WaitForAutoLocatorProcess();
public:
    CAutoLocatorAppController();
    virtual ~CAutoLocatorAppController();
    BOOL InvokeAutoLocatorApplication(const AUTOLOCATOR_TYPE f_autolocator_type, const bool f_cala_invoke_by_menu = false);
    int GetAutoLocatorProcessID() const;
    void SetAutoLocatorProcessID(const int f_cala_process_id);
    void WaitForAutoLocatorProcess(const int f_process_id);
    CString GetProtIndexAndWorkingMode()const;
    void SetProtIndexAndWorkingMode(const CString& f_cala_prot_index_workg_mode);
    void SetLoadGLFlag(const bool f_loadglflag);
    bool GetLoadGLFlag();
};

#endif // !defined(AFX_AUTOLOCATORAPPCONTROLLER_H__6B99A84C_F6EF_49C6_81D7_6A6A250F9D52__INCLUDED_)

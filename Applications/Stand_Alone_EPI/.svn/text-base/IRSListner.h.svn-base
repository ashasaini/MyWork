// IRSListner.h: interface for the CIRSListner class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IRSLISTNER_H__C91DD82A_3211_47FA_8527_70AD90DB5B32__INCLUDED_)
#define AFX_IRSLISTNER_H__C91DD82A_3211_47FA_8527_70AD90DB5B32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
const int INVALID_FD = -1;
#include <Winsock2.h>
#include "resource.h"
#include <tami/libvf/varfields.h>       /* Varfield definitions         */
#include <tami/vfStudy/vf_basictypes.h>
#include <tami/vfStudy/vfstudy.h>       /* Study hierarchy defs         */

#include <tami/common/study_id.h>
#include <tami/common/basicdefs.h>
#include "tami/libWNP/WNP_types.h"	


#include <tami/BDM/bdmDefs.h>
//#include <tami\libWNP\WNP_proto.h>
extern "C" {
#include <tami/libIrs/Irs.h>
#include <tami/libIrs/IrsEventFields.h>
}

class CIRSListner  
{
public:
	CIRSListner();
	virtual ~CIRSListner();

    // Initialize Create Listener thread and starts listening
    BOOL InitializeIRS();
    void CloseIrsNotifier();
    int GetIRSSocketFD();
    int getReconButtonClk() const { return m_nReconButtonClk; }
    void setReconButtonClk(int val) { m_nReconButtonClk = val; }
    int getReconCalled() const { return m_nReconCalled; }
    void setReconCalled(int val) { m_nReconCalled = val; }     
    
private:
    // Thread function which will continously listening
    static DWORD WINAPI IRSListnerThread(LPVOID f_parameter);
    
    int IRSRegisterReconDone();
    
    // call back function when IRS starts communicating with PQM
    void IRSEventHandler();
    
    
private:
    int                     m_bIndexToPass;
    int                     m_irs_socket;
    HANDLE                  m_irslisner_thread;
    int                     m_nReconCalled;
    int                     m_nReconButtonClk;
};

#endif // !defined(AFX_IRSLISTNER_H__C91DD82A_3211_47FA_8527_70AD90DB5B32__INCLUDED_)

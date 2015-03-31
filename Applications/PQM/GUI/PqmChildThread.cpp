//REDMINE_782_CPP_Test_Tool
//TMSC-REDMINE-782
//TMSC-REDMINE-781
//TMSC-REDMINE-777

/*****************************************************************************
*
*  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
*
*  File Name: PQMChildThread.cpp
*  Overview: Structure of CPQMChildThread class.
*  Creational Date: 2009/04/07
*****************************************************************************
*                       Revision History
*****************************************************************************
*  Revision      Author           Date & Time            Changes
*  ==========================================================================
*  1.0				PATNI/SS		2009/04/07			Created
*
*
 *****************************************************************************/
#include "stdafx.h"
#include "PqmChildThread.h"
#include "PQMView.h"
#include <PErrorHandler/ExceptionHandler.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


HANDLE CPqmChildThread::m_PQMCurrStateEvent = NULL;

CPqmChildThread::CPqmChildThread(
):	hThread(NULL),
    dThreadId(-1),
    //+Patni-Sribanta/2011Jan24/Added/TMSC-REDMINE-1026
    hScanThread(NULL),
    dScanThreadId(-1)
    //-Patni-Sribanta/2011Jan24/Added/TMSC-REDMINE-1026
{
}

CPqmChildThread::~CPqmChildThread()
{
    StopThread();
}

int CPqmChildThread::CreateChildThread(CPQMView* f_pqmview)
{

    hThread = CreateThread(NULL, 0, CPqmChildThread::PqmChlidThreadProc, f_pqmview, 0, &dThreadId);

    if (NULL == hThread) {
        return false;
    }

    //+Patni-Sribanta/2011Jan24/Added/TMSC-REDMINE-1026
    hScanThread = CreateThread(NULL, 0, CPqmChildThread::PqmChlidScanThreadProc, f_pqmview, 0, &dScanThreadId);

    if (NULL == hScanThread) {
        return false;
    }

    //-Patni-Sribanta/2011Jan24/Added/TMSC-REDMINE-1026


    CPqmChildThread::m_PQMCurrStateEvent = CreateEvent(NULL, TRUE, TRUE, PQM_CHILD_EVENT);

    if (NULL == CPqmChildThread::m_PQMCurrStateEvent) {
        StopThread();
        return false ;
    }

    return true;
}


void CPqmChildThread::PostMessageToThread(UINT fmsg, WPARAM fwParam, LPARAM flParam)const
{
    if (hThread != NULL) {
        PostThreadMessage(dThreadId, fmsg, fwParam, flParam);
    }
}

//+Patni-Sribanta/2011Jan24/Added/TMSC-REDMINE-1026
void CPqmChildThread::PostMessageToScanThread(UINT fmsg, WPARAM fwParam, LPARAM flParam)const
{
    if (hScanThread != NULL) {
        PostThreadMessage(dScanThreadId, fmsg, fwParam, flParam);
    }
}
//-Patni-Sribanta/2011Jan24/Added/TMSC-REDMINE-1026

bool CPqmChildThread::StopThread()
{
    if (hThread != NULL) {

        PostThreadMessage(dThreadId, WM_QUIT, NULL, NULL);
        CloseHandle(hThread);
        CloseHandle(m_PQMCurrStateEvent);
        hThread = NULL;
        m_PQMCurrStateEvent = NULL;
        dThreadId = -1;
    }

    if (NULL != hScanThread) {

        //+Patni-Sribanta/2011Jan24/Added/TMSC-REDMINE-1026
        PostThreadMessage(dScanThreadId, WM_QUIT, NULL, NULL);
        CloseHandle(hScanThread);
        hScanThread = NULL;
        dScanThreadId = -1;
        //-Patni-Sribanta/2011Jan24/Added/TMSC-REDMINE-1026
    }

    return true;
}



//+Patni-Sribanta/2011Jan11/Added/TMSC-REDMINE-1026
DWORD WINAPI CPqmChildThread::PqmChlidScanThreadProc(
    LPVOID lpParameter
)
{
    __try {
        if (NULL == lpParameter) return FALSE;

        CoInitialize(NULL);

        CPQMView* self = (CPQMView*)lpParameter;
        MSG msg;

        while (GetMessage(&msg, NULL, 0, 0)) {
            if (msg.message == WM_QUIT) {
                CoUninitialize();
                return TRUE;

            } else if (ON_SCAN_START == msg.message) {
                self->OnClickButtonScanStart();

            } else if (ON_SCAN_RESUME == msg.message) {
                self->OnClickButtonScanResume();

            } else if (ON_SCAN_PAUSE == msg.message) {
                self->OnClickButtonScanPause();

            } else if (ON_IRS_SCAN_MSG == msg.message) {
                self->OnHandleIRSMessageOnScanThread(msg.wParam, msg.lParam);
            }
        }

        CoUninitialize();

    } __except(RecordExceptionInfo(GetExceptionInformation(),
                                   _T("CPqmChildThread::PqmChlidScanThreadProc()"))) {

    }

    return 0;
}
//-Patni-Sribanta/2011Jan11/Added/TMSC-REDMINE-1026

DWORD WINAPI CPqmChildThread::PqmChlidThreadProc(LPVOID lpParameter)
{

    //
    __try {
        if (NULL == lpParameter) return false;

        CoInitialize(NULL);

        CPQMView* self = (CPQMView*)lpParameter;
        MSG msg;

        while (1) {
            GetMessage(&msg, NULL, 0, 0);

            if (msg.message == WM_QUIT) {
                //20091006/Tanoue-san for CoUninitialize
                CoUninitialize();
                return true;

            } else if (msg.message == ON_DUPLICATE) {
                if (NULL != m_PQMCurrStateEvent) {
                    ResetEvent(m_PQMCurrStateEvent);
                    self->OnClickButtonDuplicate();
                    SetEvent(m_PQMCurrStateEvent);
                }

            }

            //+Patni-KSS/2010Jul16/ADDED/IR-156_2
            else if (msg.message == ON_DUPLICATE_APPEND) {
                if (NULL != m_PQMCurrStateEvent) {
                    ResetEvent(m_PQMCurrStateEvent);
                    self->OnClickButtonDuplicate(true);
                    SetEvent(m_PQMCurrStateEvent);
                }

            }//-Patni-KSS/2010Jul16/ADDED/IR-156_2

            else if (msg.message == ON_AMB_DUPLCIATE) {
                if (NULL != m_PQMCurrStateEvent) {
                    ResetEvent(m_PQMCurrStateEvent);
                    self->AMBPerformSlaveDuplicate();
                    SetEvent(m_PQMCurrStateEvent);
                }
            }

            else if (msg.message == ON_DELETE) {
                if (NULL != m_PQMCurrStateEvent) {
                    ResetEvent(m_PQMCurrStateEvent);
                    self->OnClickButtonDelete();
                    SetEvent(m_PQMCurrStateEvent);
                }

            } else if (msg.message == ON_COILAPPLY) {

                if (NULL != m_PQMCurrStateEvent) {
                    ResetEvent(m_PQMCurrStateEvent);
                    self->ApplyCoilToProtocols();
                    SetEvent(m_PQMCurrStateEvent);
                }

            } else if (msg.message == ON_ANATOMYAPPLY) {
                if (NULL != m_PQMCurrStateEvent) {
                    ResetEvent(m_PQMCurrStateEvent);
                    self->OnClickButtonSarRegion((int)msg.wParam);
                    SetEvent(m_PQMCurrStateEvent);
                }

            } else if (msg.message == ON_SETALL) {
                if (NULL != m_PQMCurrStateEvent) {
                    ResetEvent(m_PQMCurrStateEvent);
                    self->OnClickButtonSetAll();
                    SetEvent(m_PQMCurrStateEvent);
                }

                //+Patni-SS/2009Nov16/Added/IR#90/SFT Menu

            } else if (msg.message == ON_END_EXAM) {
                if (NULL != m_PQMCurrStateEvent) {
                    ResetEvent(m_PQMCurrStateEvent);
                    self->OnClickButtonEndExam();
                    SetEvent(m_PQMCurrStateEvent);
                }
            }

            //+Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
            else if (msg.message == ON_SFT_MENU_COMMAND) {
                if (NULL != m_PQMCurrStateEvent) {

                    //+Patni-Hemant/2010Aug05/Modified/IR-152_ITT_DefectFix
                    //
                    //ResetEvent(m_PQMCurrStateEvent);
                    //self->HandleSFTMenuCommand((UINT)msg.wParam);
                    //SetEvent(m_PQMCurrStateEvent);
                    self->HandleSFTMenuCommand((UINT)msg.wParam);
                    //
                    //-Patni-Hemant/2010Aug05/Modified/IR-152_ITT_DefectFix
                }
            }

            //+Patni-Sribanta/2011Jan11/Added/TMSC-REDMINE-1026
            else if (ON_VOICE_MENUITEM_CLICK == msg.message) {
                self->OnClickVoiceMenuItem((int)msg.wParam);
            }

            //-Patni-Sribanta/2011Jan11/Modified/TMSC-REDMINE-1026
            else if (ON_SCAN_EDIT_OR_PLAN == msg.message) {
                self->StartScanEditOrPlan((ProconMessageEnum)msg.wParam, (bool)(msg.lParam > 0));
            }

            else if (ON_UPDATE_SARINFO == msg.message) {
                self->UpdateSarInfoOnPage();

            } else if (ON_CFA_OPERATION == msg.message) {
                self->DoCFAOperation(msg.wParam > 0);
            }

            //-Patni-Hemant/2010June28/Added/IR-152 Configurable SFTMenu
            else {
                // Unknown message....
            }
        }

        //20091006/Tanoue-san for CoUninitialize
        CoUninitialize();

    } __except(RecordExceptionInfo(GetExceptionInformation(),
                                   _T("CPqmChildThread::PqmChlidThreadProc()"))) {

    }

    return 0;
}


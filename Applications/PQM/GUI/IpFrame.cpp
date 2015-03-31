//TMSC-REDMINE-782
//TMSC-REDMINE-781
//TMSC-REDMINE-777
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: IpFrame.cpp
 *  Overview: Implementation of CIpFrame class.
 *  Last Modified: 2007/8/10 12:00:00
 *****************************************************************************
 *                       Revision History
 *****************************************************************************
 *  Revision      Author           Date & Time            Changes
 *  ==========================================================================
 *   1.0          Patni            2007/6/15 12:00:00     PQM5 Baselined code
 *   2.0          Patni            2007/8/10 12:00:00     Implemented PQM6 requirements & Formatted
 *
 *****************************************************************************/
#include "stdafx.h"
#include "IpFrame.h"
#include <libStudyManager/CFW/CFWSender.h>
#include <libStudyManager/WinAPIAdapter.h>

#include "PQM.h"
#include "PQMView.h"
#include "PQMUtility.h"
#include "DPSManager.h"

using MR::ACQ::SM::COMMON::CFW::CCFWSender;
using MR::ACQ::SM::COMMON::UTIL::GetCurProcessIdString;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CInPlaceFrame, COleDocIPFrameWnd)

BEGIN_MESSAGE_MAP(CInPlaceFrame, COleDocIPFrameWnd)
    //{{AFX_MSG_MAP(CInPlaceFrame)
    ON_WM_CREATE()
    ON_WM_COPYDATA()

    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

extern CPQMApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CInPlaceFrame construction/destruction

CInPlaceFrame::CInPlaceFrame()
//Patni-PJS/2010Apr20/Added/JaFT# MVC007170_CDR_01

{
}

CInPlaceFrame::~CInPlaceFrame()
{
}


int CInPlaceFrame::OnCreate(
    LPCREATESTRUCT lpCreateStruct
)
{
    if (COleDocIPFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    // CResizeBar implements in-place resizing.
    if (!m_wndResizeBar.Create(this)) {
        TRACE0("Failed to create resize bar\n");
        return -1;      // fail to create
    }

    // By default, it is a good idea to register a drop-target that does
    //  nothing with your frame window.  This prevents drops from
    //  "falling through" to a container that supports drag-drop.
    m_dropTarget.Register(this);

    return 0;
}

BOOL CInPlaceFrame::OnCopyData(
    CWnd* pWnd,
    COPYDATASTRUCT* pCopyDataStruct
)
{
    LPCTSTR FUNC_NAME = _T("CInPlaceFrame::OnCopyData");

    PQM_TRACE(SYSTEM_FUNC_MARKER, FUNC_NAME,
              _T("Trace of OnCopyData"));

    if (pCopyDataStruct->dwData == 0) {
        CPQMView* pView = (CPQMView*)GetActiveView();

        if (pView == NULL) {
            return FALSE;
        }

        GLInfo* pData = (GLInfo*)pCopyDataStruct->lpData;

        CString mode = pData->csPageName;

        CPQMUtility::GetInstance()->SetPQMParentName(mode);

        if (!CDPSManager::GetInstance()->Initialize()) {

            const CString NAME = CPQMUtility::GetInstance()->GetPQMParentName() + CString(_T(".")) + CPQMUtility::GetInstance()->GetApplicationName();
            CCFWSender sender;
            sender.Initialize(NAME, NAME, CPQMUtility::GetInstance()->GetPQMParentName());

            CPQMUtility::GetInstance()->SetPQMStatus(STATE_READY);
            sender.SendASyncMsgToParentPage(
                DPS_MSGID_REQ_REPORTSTATE, CPQMUtility::GetInstance()->GetPQMStatusString(),
                GetCurProcessIdString());

            sender.SendASyncMsgToParentPage(_T("FW_RequestClose"), _T("PQM"));



        }

        pView->PostMessage(WM_CMD_OPEN, (WPARAM)(mode.AllocSysString()), 0);
    }

    return COleDocIPFrameWnd::OnCopyData(pWnd, pCopyDataStruct);
}

/////////////////////////////////////////////////////////////////////////////
// CInPlaceFrame commands


BOOL CInPlaceFrame::PreCreateWindow(
    CREATESTRUCT& cs
)
{
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs

    return COleDocIPFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CInPlaceFrame diagnostics

#ifdef _DEBUG
void CInPlaceFrame::AssertValid() const
{
    COleDocIPFrameWnd::AssertValid();
}

void CInPlaceFrame::Dump(CDumpContext& dc) const
{
    COleDocIPFrameWnd::Dump(dc);
}
#endif //_DEBUG

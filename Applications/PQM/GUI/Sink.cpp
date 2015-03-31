//TMSC-REDMINE-782
//TMSC-REDMINE-779
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: Sink.cpp
 *  Overview: Implementation of CSink class.
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
#include "Sink.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/****************************************************************************************

    Copyright (C) 1994-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.

    Module: PQM

    Author: PATNI-HAR
*****************************************************************************************/


//************************************Method Header**************************************
//Method Name:  CSink

//Author:  PATNI/HAR

//Purpose: Constructor.

//**************************************************************************************/
CSink::CSink(
):	m_current_view(NULL),
    m_acqmsghandler(NULL),
    m_ipcmsghandler(NULL),
    m_srmsghandler(NULL)
{
    m_acqmsghandler = new CAcqmanMessageHandler();
    m_ipcmsghandler = new CIPCMessageHandler();
    m_srmsghandler = new CSRMessageHandler();
}

//************************************Method Header**************************************

//Method Name:  CSink

//Author:  PATNI/HAR

//Purpose: Set view pointer

//**************************************************************************************/
CSink::CSink(
    CPQMView* pView
):	m_current_view(pView),
    m_acqmsghandler(NULL),
    m_ipcmsghandler(NULL),
    m_srmsghandler(NULL)
{


    m_acqmsghandler = new CAcqmanMessageHandler();
    m_ipcmsghandler = new CIPCMessageHandler();
    m_srmsghandler = new CSRMessageHandler();
}

//************************************Method Header**************************************

//Method Name:  ~CSink

//Author:  PATNI/HAR

//Purpose: Destructor

//**************************************************************************************/
CSink::~CSink(
)
{
    DEL_PTR(m_acqmsghandler)

    DEL_PTR(m_ipcmsghandler)

    DEL_PTR(m_srmsghandler)
}
//+Patni-DKH/2010Jan28/Added/PH#3#/IR86/DebugWindow
//**************************************************************************
//Method Name   : DisplayDebugData
//Author        : PATNI\DKH
//Purpose       : Event handler of IPCCOM for DisplayDebugData
//**************************************************************************
HRESULT STDMETHODCALLTYPE CSink::DisplayDebugData(
    BSTR f_displaydata
)
{
    m_current_view->UpdateDebugWindow(f_displaydata);
    return TRUE ;
}
//-Patni-DKH/2010Jan28/Added/PH#3#/IR86/DebugWindow#

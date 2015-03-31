//Redmine-777
//REDMINE-780
//Redmine-782
/*******************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name		: IPASCOMListener.h
 *  Overview		:
 *  Last Modified	: [6/6/2008]
 *******************************************************************************
 *                       Revision History
 *******************************************************************************
 *  Revision	Author		Date & Time			Changes
 *==============================================================================
 *   1.0		Patni       [6/6/2008]			Added file header
 *
 ******************************************************************************/

#ifndef __SM_PASCOM_LISTENER_H__
#define __SM_PASCOM_LISTENER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PASCOM_COMM_Def.h"

class AFX_EXT_CLASS IPASCOMListener
{
public:

    IPASCOMListener();
    virtual ~IPASCOMListener();

    virtual bool HandlePASCOMMsg(
        const PASCOM_COMM_MSG& f_pascom_msg
    ) = 0; // pure virtual
};


#endif // #ifndef __SM_PASCOM_LISTENER_H__

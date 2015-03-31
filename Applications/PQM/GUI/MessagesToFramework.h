/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: MessagesToFramework.h
 *  Overview: Structure of Messages To Framework
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
typedef enum MsgToFrameworktag {

    RE_FW_CheckState,
    RE_FW_ReportState,
    RE_FW_Show,
    RE_FW_Hide,
    RE_FW_Close

} MsgToFramework;

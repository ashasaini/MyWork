//Redmine-783
//Redmine-780
/*****************************************************************************
 *
 *  (c) Copyright 2007 TOSHIBA Corporation. All Rights Reserved
 *
 *  File Name: ColorData.h
 *  Overview: Structure of CColorData class.
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
#ifndef __SM_IPCLIB_COLORDATA_H__
#define __SM_IPCLIB_COLORDATA_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//Patni-PJS /2010Nov23/Modified/TMSC-REDMINE-774
#include "PqmIpcLibConstants.h"
class CColorData : public CObject
{
public:
    DECLARE_SERIAL(CColorData);
    //Constrcutor / Destrcutor
    CColorData();
    virtual ~CColorData();

    CColorData(const CColorData& obj);
    CColorData& operator = (const CColorData& obj);

    void SetColorStatus(const int pos, const CString& status);
    void SetFcolor(const int pos, const COLORREF fcolor);
    void SetBcolor(const int pos, const COLORREF bcolor);

private:

    CString m_status[NUM_4];
    DWORD   m_foregroundcolor[NUM_4];
    DWORD   m_backgroundcolor[NUM_4];

    void    CColorCopy(const CColorData& obj);
    void	Serialize(CArchive& ar);

    CString GetStatus(const int pos) const;
    DWORD GetFcolor(const int pos) const;
    DWORD GetBcolor(const int pos) const;


};

#endif // #ifndef __SM_IPCLIB_COLORDATA_H__ 

// OpenGLDevice.h: interface for the OpenGLDevice class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.

    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#if !defined(AFX_OPENGLDEVICE_H__341A1C8E_F2CE_4443_95DE_AF8440C0B3EA__INCLUDED_)
#define AFX_OPENGLDEVICE_H__341A1C8E_F2CE_4443_95DE_AF8440C0B3EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <windows.h>

class OpenGLDevice
{
public:
    OpenGLDevice(HDC& deviceContext, int stencil = 0);
    OpenGLDevice(HWND& window, int stencil = 0);
    OpenGLDevice();

    bool create(HDC& deviceContext, int  stencil = 0);
    bool create(HWND& window, int stencil = 0);

    bool destroy();
    bool makeCurrent(bool disableOther = true);


    virtual ~OpenGLDevice();
protected:
    bool setDCPixelFormat(HDC& deviceContext, int stencil);
    bool LoadFont();
    HGLRC renderContext;
    HDC deviceContext;

};

#endif // !defined(AFX_OPENGLDEVICE_H__341A1C8E_F2CE_4443_95DE_AF8440C0B3EA__INCLUDED_)

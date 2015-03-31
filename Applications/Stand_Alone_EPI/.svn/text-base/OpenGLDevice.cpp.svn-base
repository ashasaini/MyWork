// OpenGLDevice.cpp: implementation of the OpenGLDevice class.
//
//////////////////////////////////////////////////////////////////////
/*******************************************************************************

  Copyright (C) 1999-2004 TOSHIBA MEDICAL SYSTEMS CORPORATION, All rights reserved.

    Module: TiPrep
    Author: IGATE-Advanced Cardiac Team
*******************************************************************************/
#include "stdafx.h"
#include "OpenGLDevice.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*************************************************************
* Method:       OpenGLDevice
* Description:  This function is used
* Returns:
*************************************************************/
OpenGLDevice::OpenGLDevice()
{
    renderContext = NULL;
    deviceContext = NULL;
}

/*************************************************************
* Method:       ~OpenGLDevice
* Description:  This function is used
* Returns:
*************************************************************/
OpenGLDevice::~OpenGLDevice()
{
    destroy();
}

/*************************************************************
* Method:       OpenGLDevice
* Description:  This function is used
* Parameter:    HWND & window :
* Parameter:    int stencil :
* Returns:
*************************************************************/
OpenGLDevice::OpenGLDevice(HWND& window, int stencil)
{
    create(window, stencil);
}

/*************************************************************
* Method:       OpenGLDevice
* Description:  This function is used
* Parameter:    HDC & deviceContext :
* Parameter:    int stencil :
* Returns:
*************************************************************/
OpenGLDevice::OpenGLDevice(HDC& deviceContext, int stencil)
{
    create(deviceContext, stencil);
}
//////////////////////////////////////////////////////////////////////
// Public Member Functions
//////////////////////////////////////////////////////////////////////
/*************************************************************
* Method:       create
* Description:  This function is used
* Parameter:    HWND & window :
* Parameter:    int stencil :
* Returns:      bool
*************************************************************/
bool OpenGLDevice::create(HWND& window, int stencil)
{
    try {
        HDC deviceContext = ::GetDC(window);

        if(!create(deviceContext, stencil)) {
            ::ReleaseDC(window, deviceContext);
            return false;
        }

        ::ReleaseDC(window, deviceContext);
        return true;
    } catch(...) {
        return false;
    }
}

/*************************************************************
* Method:       create
* Description:  This function is used
* Parameter:    HDC & deviceContext :
* Parameter:    int stencil :
* Returns:      bool
*************************************************************/
bool OpenGLDevice::create(HDC& deviceContext, int stencil)
{
    try {
        if(!deviceContext) {
            return false;
        }

        if(!setDCPixelFormat(deviceContext, stencil)) {
            return false;
        }

        destroy();
        renderContext = wglCreateContext(deviceContext);
        wglMakeCurrent(deviceContext, renderContext);
        OpenGLDevice::deviceContext = deviceContext;
        LoadFont();
        return true;
    } catch(...) {
        return false;
    }
}



/*************************************************************
* Method:       destroy
* Description:  This function is used
* Returns:      bool
*************************************************************/
bool OpenGLDevice::destroy()
{
    if(renderContext != NULL) {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(renderContext);
    } else {
        return false;
    }

    return true;
}

/*************************************************************
* Method:       makeCurrent
* Description:  This function is used
* Parameter:    bool disableOther :
* Returns:      bool
*************************************************************/
bool OpenGLDevice::makeCurrent(bool disableOther)
{
    if(renderContext != NULL) {
        //should all other device contexts
        //be disabled then?
        if(disableOther) {
            wglMakeCurrent(NULL, NULL);
        }

        wglMakeCurrent(deviceContext, renderContext);
    } else {
        return false;
    }

    return true;
}
//////////////////////////////////////////////////////////////////////
// Protected Member Functions
//////////////////////////////////////////////////////////////////////
/*************************************************************
* Method:       setDCPixelFormat
* Description:  This function is used
* Parameter:    HDC & deviceContext :
* Parameter:    int stencil :
* Returns:      bool
*************************************************************/
bool OpenGLDevice::setDCPixelFormat(HDC& deviceContext, int stencil)
{
    try {
        UNREFERENCED_PARAMETER(stencil);
        int pixelFormat;
        DEVMODE resolution;
        //PIXELFORMAT->BPP = DESKTOP->BPP
        EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &resolution);
        static PIXELFORMATDESCRIPTOR pixelFormatDesc = {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW |
            PFD_SUPPORT_OPENGL |
//                 PFD_GENERIC_FORMAT |
//                 PFD_SWAP_LAYER_BUFFERS|
            PFD_DOUBLEBUFFER,
            PFD_TYPE_RGBA,
            (BYTE)resolution.dmBitsPerPel,
            0, 0, 0, 0, 0, 0,
            0,
            0,
            0,
            0, 0, 0, 0,
            16,
            NULL,//stencil,
            0,
            0,
            0,
            0, 0, 0
        };
        pixelFormat = ChoosePixelFormat(deviceContext,
                                        &pixelFormatDesc);

        if(!SetPixelFormat(deviceContext, pixelFormat,
                           &pixelFormatDesc)) {
            return false ;
        }

        return true;
    } catch(...) {
        return false;
    }
}

/*************************************************************
* Method:       LoadFont
* Description:  This function is used
* Returns:      bool
*************************************************************/
bool OpenGLDevice::LoadFont()
{
    if(TRUE == wglUseFontBitmaps(deviceContext, 0, 255, 1000)) {
        return true;
    } else {
        return false;
    }
}
///////////////////////////////////////////////////////////////////////////////
// End of file
//////////////
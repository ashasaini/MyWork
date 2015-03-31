/**
 * Copyright(c) 2006 - 2009 TOSHIBA Medical Systems Corporation, All Rights
 * Reserved.
 *
 * AVICtrl.h - AVICtrl class declaration file
 *
 * @author :    Krishnakumar T G
 * @version:    1.0            Date:  2006-03-20
 * @author :    Vinod N N
 * @version:    2.0            Date:  2007-05-10
 * @author :    Naveen R.
 * @version:    2.1            Date:  2007-12-22
 * @author :    Shypa Padmanabhan     (Modified for MPlusV4)
 * @version:    2.2            Date:  2009-01-09
 */

#ifndef _AVICTRL_H_
#define _AVICTRL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/**
 * Added for Celeve PIMS Phase-5 requirement
 * @since  2.0
 */
#include <afxcmn.h>
/**
 * End of Change  for Celeve PIMS Phase-5 requirement
 * @since  2.0
 */
// Header file
#pragma warning ( push )
/**
 * Modified for MPlus V4
 * @since 2.2
 */
// As per TMSC comments ,removing warning disabling .
//#pragma warning ( disable: 4201 )
/**
  * End of Modification for MPlus V4
  */
#include <Vfw.h>
#pragma warning ( pop )

// Load flags
#define AVC_HALFSPEED           0x0000001 // Plays video with half speed
#define AVC_DOUBLESPEED         0x0000002 // Plays video with double speed

#define AVC_CENTERAVI           0x0000004 // Centers video inside the window
#define AVC_STRETCHAVI          0x0000008 // Stretches video to fit inside of the window
#define AVC_CENTERRECT          0x0000010 // Resizes window, center point stays the same

#define AVC_AUTOPLAY            0x0000020 // Starts playing automatically after Load()

#define AVC_MAPWINDOWCOLOR      0x0000040 // Background is COLOR_WINDOW instead of transparent (like CAnimateCtrl's AVS_"TRANSPARENT")
#define AVC_FIRSTPIXTRANSPRNT   0x0000080 // Ignore clrTransparent parameter and use color of firstframes first pixel for transparency

// Colors
/**
 * Modified for MPlus V4
 * @since 2.2
 */
//#define BLACK       RGB(0, 0, 0)
//#define WHITE       RGB(255, 255, 255)
//#define LTGREEN     RGB(0, 255, 0)
const COLORREF BLACK    =  RGB(0, 0, 0);
const COLORREF WHITE    =  RGB(255, 255, 255);
const COLORREF LTGREEN  =  RGB(0, 255, 0);
/**
 * End of Modification for MPlus V4
 */

// Timer
#define ID_TIMER    111

// Identifier for avi resources
#define RT_AVIVIDEO _T( "AVI" )

namespace MPlus
{
    namespace UICommon
    {
        /**
         * Copyright(c) 2006 - 2007 TOSHIBA Medical Systems Corporation, All Rights
         * Reserved.
         *
         * AVICtrl - AVICtrl class declaration this class is derived from CStatic
         *
         * @author :    Krishnakumar T G
         * @version:    1.0            Date:  2006-03-20
         * @author :    Vinod N N
         * @version:    2.0            Date:  2007-05-10
         * @author :    Naveen R.
         * @version:    2.1            Date:  2007-12-22
         */
        class AVICtrl : public CStatic
        {
        public:

            // Constructor
            AVICtrl();
            // Virtual Destructor
            virtual ~AVICtrl();

            // ClassWizard generated virtual function overrides
            //{{AFX_VIRTUAL(AVICtrl)
            //}}AFX_VIRTUAL

        protected:

            //{{AFX_MSG(AVICtrl)
            afx_msg void OnWindowPosChanging( WINDOWPOS FAR* lpwndpos_i );
            afx_msg void OnPaint();
            afx_msg BOOL OnEraseBkgnd( CDC* pDC_i );
            afx_msg void OnSysColorChange();
            afx_msg void OnDestroy();
           /**
            * Modified for MPlus V4
            * @since 2.2
            */
            /// Modified for correcting type conversion error.
            // afx_msg void OnTimer( UINT nIDEvent_i );
            afx_msg void OnTimer( UINT_PTR nIDEvent_i );
           /**
            * End of Modification for MPlus V4
            */
            //}}AFX_MSG

            DECLARE_MESSAGE_MAP()

        public:

            // To load the resource file
            HRESULT Load( UINT nIDResource_i, DWORD dwFlags_i = 0L, COLORREF clrTransparent_i = LTGREEN );
            // To load the AVI file
            HRESULT Load( LPCTSTR lpszFile_i, DWORD dwFlags_i = 0L, COLORREF clrTransparent_i = LTGREEN );
            // Reinitialize the background
            void ReinitBackground()
            {
                m_bInitBkg = TRUE;
            };
            // Play the AVI file 
            BOOL Play( BOOL bOnce_i = FALSE );
            // Stop the AVI file
            BOOL Stop( BOOL bResetToFirst_i = FALSE );
            // Seek the position in AVI
            void Seek( UINT nTo_i );
            // To check the file is whether playing or not
            BOOL IsPlaying()
            {
                return m_bPlaying;
            };

            /* Begin: Modification 2.1 for Bug ID #703, #704, #705 */
            // To get real width of the AVI frame.
            int GetRealWidth() const
            {
                return m_nRealWidth;
            }
            // To get the real height of the AVI frame.
            int GetRealHeight() const
            {
                return m_nRealHeight;
            }
            /* End: Modification 2.1 for Bug ID #703, #704, #705 */

        protected:

            // Draw the current AVI frame
            void DrawCurrentFrame( CDC* pDC_i );
            // To draw the DC with a transparent color 
            void DrawTransparent( int nX_i, int nY_i, int nWidth_i, int nHeight_i, CDC* pSrcDC_i, CDC* pDstDC_o );
            // To Initialize the background
            void InitBackground();
            // Get the first pixel color 
            COLORREF GetFirstPixelColor();
            // Get the temporary file name
            CString GetTempAVIFileName();

        private:

           /**
            * Modified for MPlus V4
            * @since 2.2
            */
            // To prevent default implementation of copy constructor.
            AVICtrl( const AVICtrl& AVICtrlObj_i );
            // To prevent Default implementation of = operator.
            AVICtrl& operator=( const AVICtrl& AVICtrlObj_i );
           /**
            * End of Modification for MPlus V4
            */

            // Initialization
            void CtrlInit();
            // Class Cleanup 
            void CtrlDestroy();

        private:

            // Holds the style flags
            DWORD m_dwFlags;
            // Holds the temporary AVI filename
            CString m_csTempFile;
            // Holds the control height
            int m_nRealHeight;
            // Holds the control width
            int m_nRealWidth;
            // Holds the vertical offset
            int m_nAVIOffsetY;
            // Holds the horizontal offset
            int m_nAVIOffsetX;
            // Holds the flag that whether the file is to be played once
            BOOL m_bOnce;
            // Holds the flag that whether the background is to be initialized
            BOOL m_bInitBkg;
            // Holds the old background bitmap
            CBitmap* m_pbmpBkgOld;
            // Holds the background bitmap
            CBitmap m_bmpBkg;
            // Holds the parent window device context
            CDC m_dcBkg;
            // Holds the transparent color
            COLORREF m_clrTransp;
            // Holds the handle to the  DrawDib DC
            HDRAWDIB m_hDrawDib;
            // Holds the frame interval
            UINT m_nTimerTime;
            // Holds the number of AVI frames
            LONG m_lFrames;
            // Holds the current frame number
            UINT m_nCurrentFrame;
            // Holds GetFrame object pointer
            PGETFRAME m_pGF;
            // Holds the flag that tells whether the file is playing or not
            BOOL m_bPlaying;
            // Holds the stream handle
            PAVISTREAM m_pAS;
        };

        //{{AFX_INSERT_LOCATION}}

    } // End of namespace UICommon
} // End of namespace MPlus

#endif // _AVICTRL_H_

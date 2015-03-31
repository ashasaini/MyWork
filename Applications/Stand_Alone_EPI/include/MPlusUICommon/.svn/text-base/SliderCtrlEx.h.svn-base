/**
 * Copyright(c) 2007-2010 TOSHIBA Medical Systems Corporation, All Rights
 * Reserved.
 *
 * SliderCtrlEx.h - Include file for SliderCtrlEx control.
 *
 * @author :    Praveen Kumar.C
 * @version:    1.0            Date:  2007-04-20
 * @author :    Nishad S
 * @version:    1.1            Date:  2007-06-21
 * @author :    Jijeesh K V
 * @version:    1.2            Date:  2008-04-09
 * @author :    Jijeesh K V
 * @version:    1.3            Date:  2008-05-22
 * @author :    Naveen R
 * @version:    1.4            Date:  2008-10-02
 * @author :    Naveen R
 * @version:    1.5            Date:  2008-12-09
 * @author :    Junaij M.
 * @version:    1.6            Date:  2008-12-24
 * @author :    Ajish Dharman U.
 * @version:    1.7            Date:  2009-02-09 
 * @author :    Shypa Padmanabhan     (Modified for MPlusV4)
 * @version:    1.8            Date:  2009-01-09
 * @author :    Jinesh P.P.
 * @version:    1.9            Date:  2010-03-09
 */

#ifndef _SLIDERCTRLEX_H_
#define _SLIDERCTRLEX_H_

#if _MSC_VER > 1000
#pragma once
#endif 

#ifndef _UNICODE
#error Unicode definition is necessary to use this class
#endif

#ifdef UI_CONTROLS_DLL
#define CONTROLS_CLASS_API __declspec( dllexport )
#else
#define CONTROLS_CLASS_API __declspec( dllimport )
#endif

#include "MPlusUICommon/GdiPlusCtrl.h"
#include <afxdtctl.h>
#include <afxcmn.h>

namespace MPlus
{
    namespace UICommon
    {

        /**
         * Copyright(c) 2007-2009 TOSHIBA Medical Systems Corporation, All Rights
         * Reserved.
         *
         * THUMB_COLORS_t - THUMB_COLORS_t structure declaration.
         *
         * @author :    Praveen Kumar. C
         * @version:    1.0            Date:  2007-04-23
         * @author :    Nishad S
         * @version:    1.1            Date:  2007-06-21
         * @author :    Jijeesh K V
         * @version:    1.2            Date:  2008-04-09
         * @author :    Jijeesh K V
         * @version:    1.3            Date:  2008-05-22
         */
        typedef struct THUMB_COLORS_tag THUMB_COLORS_t;
        struct THUMB_COLORS_tag
        {
            // Holds thumb gradient start color
            COLORREF clrGradientStart;
            // Holds thumb gradient end color
            COLORREF clrGradientEnd;
            // Holds color of the right cross border
            COLORREF clrRightCrossBorder;
            // Holds color of the right vertical border
            COLORREF clrRightVertBorder;
            // Holds color of the right corner point
            COLORREF clrRightCornerPoint;
            // Holds color of the bottom border
            COLORREF clrBottomBorder;
            // Holds color of the left corner point
            COLORREF clrLeftCornerPoint;
            // Holds color of the left vertical border
            COLORREF clrLeftVertBorder;
            // Holds color of the left cross border
            COLORREF clrLeftCrossBorder;
            // Holds color of the left cross shade
            COLORREF clrLeftCrossShade;
            // Holds color of the right cross shade
            COLORREF clrRightCrossShade;
            // Holds color of the left grip 
            COLORREF clrGripLeftLine;
            // Holds color of the right grip
            COLORREF clrGripRightLine;
            // Holds color of top corner point of the grip
            COLORREF clrGripTopCornerPoint;
            // Holds color of bottom corner point of the grip
            COLORREF clrGripBottomCornerPoint;
            // Holds color of top point of the thumb
            COLORREF clrTopPoint;
        };


        /**
         * Copyright(c) 2007 TOSHIBA Medical Systems Corporation, All Rights
         * Reserved.
         *
         * SLIDER_COLORS_t - SLIDER_COLORS_t structure declaration.
         *
         * @author :    Praveen Kumar.C
         * @version:    1.0            Date:  2007-04-20
         * @author :    Nishad S
         * @version:    1.1            Date:  2007-06-21
         * @author :    Jijeesh K V
         * @version:    1.2            Date:  2008-04-09
         */
         typedef struct SLIDER_COLORS_tag SLIDER_COLORS_t;
         struct SLIDER_COLORS_tag
         {
             
             // Holds disabled channel color
             COLORREF clrDisabledChannel;
             // Holds channel color
             COLORREF clrChannel;
             // Holds channel border color
             COLORREF clrChannelBorder;
             // Holds mouse over channel border color
             COLORREF clrMouseOverChannelBorder;
             // Holds disabled channel border color
             COLORREF clrDisabledChannelBorder;
             // Holds background color
             COLORREF clrBk;
             // Holds disabled background color
             COLORREF clrDisabledBk;
             // Holds pointer color
             COLORREF clrPointer;
             // Holds disabled pointer color
             COLORREF clrDisabledPointer;
             // Holds normal thumb colors
             THUMB_COLORS_t stNormalThumb;
             // Holds mouse over thumb colors
             THUMB_COLORS_t stMouseOverThumb;
             // Holds disabled thumb color
             COLORREF clrDisabledThumb;
             /* Begin: Added 1.2 */
             // Holds inactive channel color when range thumb exists
             COLORREF clrChannelInactive;
             /* End: Added 1.2 */
         };

         // Status of the scrolling
         enum SCROLLINFO_e
         {
             // Scrolling to left
             SCEX_SI_LEFT,
             // Scrolling to right
             SCEX_SI_RIGHT,
             // Idle 
             SCEX_SI_IDLE
         };

         /* Begin: Added 1.2 */
         // Thumb positioning status
         enum THUMB_POS_e
         {
             // Thumb at bottom
             TOP_OR_LEFT_THUMB = 0,
             // Thumb at top
             BOTTOM_OR_RIGHT_THUMB
         };
         /* End: Added 1.2 */
         
        /* Begin: Modification 1.6 for Bug ID #736 */
        // Notification code for changing left/top or right/bottom thumbs in SliderCtrlEx
        const int TBN_RANGECHANGE = ( WM_USER + 200 );

        // Notification structure for TBN_RANGECHANGE.
        struct NMTBRANGECHANGE
        {
            NMHDR stNMHDR;
            int   nThumbType; // Type of the thumb 0- Top/Left Thumb, 1 - Bottom/Right Thumb.
            int   nPos;       // Current Scroll position
            int   nMax;       // Max value of Range.
            int   nMin;       // Min value of Range
            int   nLeftPos;   // Current left/top Range position.
            int   nRightPos;  // Current right/Bottom range position.
        };
        /* Begin: Modification 1.6 for Bug ID #736 */

        /**
         * Copyright(c) 2007 TOSHIBA Medical Systems Corporation, All Rights
         * Reserved.
         *
         * SliderCtrlEx - The class creates the SliderCtrlEx control.
         *
         * Traceability Tags : TCOM_MP_CS_UI_SLIDERCTRLEX_SET_COLORS
         *                     TCOM_MP_CS_UI_SLIDERCTRLEX_SET_THUMBSIZE
         *                     TCOM_MP_CS_UI_SLIDERCTRLEX_SET_CHANNELTHICKNESS
         *
         * @author :    Praveen Kumar.C
         * @version:    1.0            Date:  2007-04-20
         * @author :    Nishad S
         * @version:    1.1            Date:  2007-06-21
         * @author :    Jijeesh K V
         * @version:    1.2            Date:  2008-04-09
         */
        class CONTROLS_CLASS_API SliderCtrlEx : public CSliderCtrl
        {

        public:

            // Constructor
            SliderCtrlEx();
            // Destructor
            virtual ~SliderCtrlEx();
            // Function to set thickness of the channel
            void SetChannelThickness( int nThickness_i );
            // Function to set height and width of the thumb
            void SetThumbSize( int nHeight_i, int nWidth_i );
            // Function to set colors of the control
            void SetColors( const SLIDER_COLORS_t &stColors_i );
            // Function to get thickness of the channel
            int GetChannelThickness();
            // Function to get height and width of the thumb
            void GetThumbSize( int &nHeight_o, int &nWidth_o );
            // Function to get colors of the control
            void GetColors( SLIDER_COLORS_t &stColors_o );
            // Enable/disable mouse hover effect.
            void EnableHoverEffect( bool bEnable_i = true )
            {
                m_bHoverEffect = bEnable_i;
            }

            /* Begin: Added 1.2 */
            // Function to set the knob style
            void SetKnobStyle( THUMB_POS_e eTop_i = BOTTOM_OR_RIGHT_THUMB );
            // Function to set left thumb position
            void SetLeftPos( int nLeft_i );
            // Function to set right thumb position
            void SetRightPos( int nRight_i );
            // To set the range
            void SetRange( int nMin_i, int nMax_i, BOOL bRedraw_i = FALSE );
            // To enable the range thumbs
            void EnableRangeThumbs( bool bShow_i = false );
            /* End: Added 1.2 */

            /* Begin: Added 1.4 */
            // This function will return the left position of the Limit range bar
            int GetLeftPos();
            // This function will return the right position of the Limit range bar
            int GetRightPos();
            /* End: Added 1.4 */
            /* Begin: Added 1.9 for Bug ID MPC06127 */
            // Overridden function to handle Invalidate.
            // This is required to avoid the call to Invalidate when window handle is invalid.
            void Invalidate( BOOL bErase_i = TRUE )
            {
                if( IsWindow( m_hWnd ))
                {
                    ::InvalidateRect( m_hWnd, 0, bErase_i );
                }
            }
            /* End: Added 1.9 for Bug ID MPC06127 */


            //{{AFX_VIRTUAL(SliderCtrlEx)
            protected:
            virtual void PreSubclassWindow();
            virtual LRESULT WindowProc( UINT uMessage_i, WPARAM wParam_i, LPARAM lParam_i );
            //}}AFX_VIRTUAL

        protected:

            //{{AFX_MSG(SliderCtrlEx)
            afx_msg void OnPaint();
            afx_msg void OnLButtonDown( UINT uFlags_i, CPoint point_i );
            afx_msg void OnLButtonUp( UINT uFlags_i, CPoint point_i );
            afx_msg void OnMouseMove( UINT uFlags_i, CPoint point_i );
            afx_msg BOOL OnMouseWheel( UINT uFlags_i, short sDelta_i, CPoint point_i );
            afx_msg void OnKeyDown( UINT uChar_i, UINT uRepCnt_i, UINT uFlags_i );
           /**
            * Modified for MPlus V4
            * @since 1.8
            */
            /// Modified for correcting type conversion error.
            // afx_msg void OnTimer( UINT uIDEvent_i );
            afx_msg void OnTimer( UINT_PTR uIDEvent_i );
           /**
            * End of Modification for MPlus V4
            */
            /* Begin: Added 1.3 */
            afx_msg void OnSize( UINT uType_i, int nCx_i, int nCy_i );
            /* End: Added 1.3 */
            //}}AFX_MSG
            afx_msg LRESULT OnMouseLeave( WPARAM wWParam_i, LPARAM lLParam_i );

            /* Begin: Modification 1.6 for Bug ID #723, #724, #725, #726, #727, #728*/
            afx_msg BOOL OnEraseBkgnd( CDC *pDC_i );
            afx_msg void OnCaptureChanged( CWnd* pWnd );
            /* Begin: Modification 1.6 for Bug ID #723, #724, #725, #726, #727, #728*/

            DECLARE_MESSAGE_MAP()

        private:

            // Function to draw control
            void DrawControl( CDC *pDC_i );
           /**
            * Modified for MPlus V4
            * @since 1.8
            */
            /* Beginning of Modification 1.7 for Bug ID #765*/
            // Function to draw horizontal thumb
            // void DrawHorizontalThumb( CDC& dc_i, CRect rect_i, THUMB_COLORS_t stColors_i );
           /**
            * Modified for MPlus V4
            * @since 1.8
            */
            //void DrawHorizontalThumb( CDC& dc_i, CRect rect_i, THUMB_COLORS_t stColors_i, bool bRangeThumb_i = false );
            void DrawHorizontalThumb( const CDC& dc_i, CRect rect_i, const THUMB_COLORS_t& stColors_i, bool bRangeThumb_i = false );
           /**
            * End of Modification for MPlus V4
            */
            // Function to draw vertical thumb
            // void DrawVerticalThumb( CDC& dc_i, CRect rect_i, THUMB_COLORS_t stColors_i );
           /**
            * Modified for MPlus V4
            * @since 1.8
            */
            //void DrawVerticalThumb( CDC& dc_i, CRect rect_i, THUMB_COLORS_t stColors_i, bool bRangeThumb_i = false );
            void DrawVerticalThumb( const CDC& dc_i, CRect rect_i, const THUMB_COLORS_t& stColors_i, bool bRangeThumb_i = false );
           /**
            * End of Modification for MPlus V4
            */
            /* End of Modification 1.7 for Bug ID #765*/
           /**
            * End of Modification for MPlus V4
            */
            // Function to end scroll
            void EndScroll();
            // Function to do thumb scroll
            void DoScroll();
            // Function to calculate channel rect
            CRect CalcChannelRect();
            // Function to calculate control rect
            void CalcControlRect();

            /* Begin: Added 1.2 */
            // Function to find the pixel position from position
            int Pos2Pixel( int nPos_i );
            // Function to find the position from pixel position
            int Pixel2Pos(int nPixel_i );
            /* End: Added 1.2 */

            /* Begin: Modification 1.5 #682 */
            void FillThumbColor( const COLORREF& ColorValue_i, THUMB_COLORS_t& stThumbColor_io );
            /* End: Modification 1.5 #682 */

        private:

            // Holds channel thickness
            int m_nChannelThickness;
            // Holds height of the thumb
            int m_nThumbHeight;
            // Holds width of the thumb
            int m_nThumbWidth;
            // Holds offset from the clicked position
            int m_nClickOffset;
            // Holds clicked point
            int m_nClickPoint;
            // Holds flag for mouse inside in the channel
            bool m_bMouseInsideChannel;
            // Holds flag for mouse inside int the thumb
            bool m_bMouseInsideThumb;
            // Holds flag for mouse inside in the control
            bool m_bMouseInside;
            // Thumb clicked flag 
            bool m_bThumbClicked;
            // LButtonDown flag
            bool m_bLButtonDown;
            // Holds colors of the control
            SLIDER_COLORS_t m_stColors;
            // Holds region for the thumb
            CRgn m_RgnThumb;
            // Holds status of scrolling
            SCROLLINFO_e m_eScrollInfo;
            // Hover effect flag
            bool m_bHoverEffect;

            /* Begin: Added 1.2 */
            // Flag to switch the bottom and top thumb
            THUMB_POS_e m_eKobTop;
            // Left Knob capturing flag
            bool m_bLeftCapture;
            // Right knob capturing flag
            bool m_bRightCapture;
            // Holds left thumb rect
            CRect m_leftRect;
            // Holds right thumb rect
            CRect m_rightRect;
            int m_nLeftPos; // Holds left thumb position
            int m_nRightPos; // Holds right thumb position
            // Holds the flag to show the range thumbs
            bool m_bRangeThumbVisible;
            /* End: Added 1.2 */
            /* Begin: Added 1.3 */
            bool m_bCalculateWidth; // Holds the flag to recalculate the control width
            int m_nControlWidth; // Holds the actual control width
            /* End: Added 1.3 */
            // Object to initialize the gdi plus
            GdiPlusCtrl m_GdiInit;
        };
    } // End of namespace UICommon
} // End of namespace MPlus

#endif // #ifndef _SLIDERCTRLEX_H_

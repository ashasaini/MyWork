/**
 * Copyright(c) 2006 - 2010 TOSHIBA Medical Systems Corporation, All Rights
 * Reserved.
 *
 * ScrollBarEx.h - ScrollBarEx class declaration file.
 *
 * @author :    Nishad S
 * @version:    1.0            Date:  2006-03-15
 * @author :    Vinod N N
 * @version:    2.0            Date:  2007-04-24
 * @author :    Nishad S
 * @version:    2.1            Date:  2007-06-21
 * @author :    Naveen R
 * @version:    2.2            Date:  2008-12-30
 * @author :    Shypa Padmanabhan     (Modified for MPlusV4)
 * @version:    2.3            Date:  2009-01-09
 * @author :    Jinesh P.P.  ( Modified for MPlusV4 BugFix Merging #MPC05902(Performance Improvement))
 * @version:    2.4            Date:  2010-02-15
 */

#ifndef _SCROLLBAREX_H_
#define _SCROLLBAREX_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "MPlusUICommon/ScrollBarGraphics.h"

/**
 * Added for Celeve PIMS Phase-5 requirement
 * @since  2.0
 */
static UINT RM_MOUSEOVER_SCROLLBAR = RegisterWindowMessage( _T( "MOUSE_OVER_SCROLLBAR_MSG" ));
/**
 * End of Change  for Celeve PIMS Phase-5 requirement
 * @since  2.0
 */

namespace MPlus
{
    namespace UICommon
    {
        /**
         * Copyright(c) 2006 - 2009 TOSHIBA Medical Systems Corporation, All Rights
         * Reserved.
         *
         * ScrollBarEx - This class is for custom scrollbar which is used by other GUI controls.
         *               In this scrollbar the scroll message is not posted to its parent window.
         *               But they are received by another window specified at the time of creation.
         *
         * Traceability Tags : TCOM_MP_CS_UI_SCROLLBAREX_TRACK_MOUSEMOVEMENT
         *                     TCOM_MP_CS_UI_SCROLLBAREX_MANAGE_SCROLLBAR_STATE
         *
         * @author :    Nishad S
         * @version:    1.0            Date:  2006-03-15
         * @author :    Vinod N N
         * @version:    2.0            Date:  2007-04-24
         */
        /**
         * Added for Celeve PIMS Phase-5 requirement
         * @since  2.0
         */
//         class CONTROLS_CLASS_API ScrollBarEx : public CWnd, ScrollBarGraphics
        /**
         * End of Change  for Celeve PIMS Phase-5 requirement
         * @since  2.0
         */

        /**
         * Added for Celeve PIMS Phase-5 requirement
         * @since  2.0
         */
         class CONTROLS_CLASS_API ScrollBarEx : public CWnd, public ScrollBarGraphics
        /**
         * End of Change  for Celeve PIMS Phase-5 requirement
         * @since  2.0
         */
        {
        public:

            // Constructor.
            ScrollBarEx();
            // Destructor.
            virtual ~ScrollBarEx();

            // ClassWizard generated virtual function overrides
            //{{AFX_VIRTUAL(ScrollBarEx)
            virtual void PreSubclassWindow();
            //}}AFX_VIRTUAL

        protected:

            //{{AFX_MSG(ScrollBarEx)
            afx_msg void OnPaint();
            afx_msg void OnSize( UINT uType_i, int nWidth_i, int nHeight_i );
            afx_msg void OnMouseMove( UINT uFlags_i, CPoint point_i );
            afx_msg void OnLButtonDown( UINT uFlags_i, CPoint point_i );
            afx_msg void OnLButtonUp( UINT uFlags_i, CPoint point_i );
           /**
            * Modified for MPlus V4
            * @since 2.3
            */
            /// Modified for correcting type conversion error.
            // afx_msg void OnTimer( UINT uIDEvent_i );
            afx_msg void OnTimer( UINT_PTR uIDEvent_i );
           /**
            * End of Modification for MPlus V4
            */
            //}}AFX_MSG
            afx_msg LRESULT OnMouseLeave( WPARAM nWParam_i, LPARAM lLParam_i );
            /* Begin: Modification 2.2 #711*/
            afx_msg void OnCaptureChanged( CWnd* pWnd );
            /* End: Modification 2.2 #711*/
            DECLARE_MESSAGE_MAP()

        public:

            // Creation.
            bool Create( CWnd* pWndParent_i, CWnd* pWndMessage_i, bool bVertical_i,
                         const CRect& rect_i = CRect( 0, 0, 0, 0 ),
                         int nID_i = 0, DWORD dwStyle_i = WS_CHILD | WS_VISIBLE, DWORD dwStyleEx_i = 0 );
            // Scroll info.
            void GetScrollInfo( LPSCROLLINFO lpScrollInfo_o, UINT uMask_i = SIF_ALL ) const;
            bool SetScrollInfo( LPSCROLLINFO lpScrollInfo_i, bool bRedraw_i = true );
            // Scroll limit.
            int  GetScrollLimit() const
            {
                return m_nMax - m_nPage;
            }
            // Scroll position.
            int  GetScrollPos() const
            {
                return m_nPos;
            }
            int  SetScrollPos( int nPos_i, bool bRedraw_i = true );
            // Scroll range.
            void GetScrollRange( int& nMinPos_o, int& nMaxPos_o ) const;
            void SetScrollRange( int nMinPos_i, int nMaxPos_i, bool bRedraw_i = true );
            // Show/hide.
            void ShowScrollBar( BOOL bShow_i = true );
            // Mouse message simulation functions.
            bool LButtonDown( const CPoint& point_i );
            bool LButtonUp( const CPoint& point_i );
            bool MouseMove( const CPoint& point_i );
            // Enable/disable scrollbar.
            BOOL EnableWindow( BOOL bEnable_i = TRUE );
            // Function to check the state of scroll bar
            BOOL IsWindowEnabled() const;
            // Overridden function to handle Invalidate.
            // This is required to avoid the call to Invalidate when window handle is invalid.
            void Invalidate( BOOL bErase_i = TRUE )
            {
                if( FALSE != IsWindow( m_hWnd ))
                {
                    ::InvalidateRect( m_hWnd, 0, bErase_i );
                }
            }
            // Enable/disable mouse hover effect.
            void EnableHoverEffect( bool bEnable_i = true )
            {
                m_bHoverEffect = bEnable_i;
            }

        private:

            // This enumerator is used to identify the components of the scrollbar.
            enum SB_ITEM_TYPE_e
            {
                SB_ITEM_NONE,
                SB_ITEM_MIN_BTN,
                SB_ITEM_MAX_BTN,
                SB_ITEM_MIN_CHAN,
                SB_ITEM_MAX_CHAN,
                SB_ITEM_THUMB
            };

        private:

           /**
            * Modified for MPlus V4
            * @since 2.3
            */
            ScrollBarEx( const ScrollBarEx& ScrollBarEx_i );
            ScrollBarEx& operator=( const ScrollBarEx& ScrollBarEx_i );
           /**
            * End of Modification for MPlus V4
            */

            // Function to calculate the rects of scrollbar components.
            void CalculateRects( int nWidth_i, int nHeight_i );
            // Function to calculate the thumb and channels rects from the scrollbar values.
            void CalculateThumbAndChannelRects( bool bForce_i = false, int nThumbPos_i = 0 );
            // Function to find the component under the given point.
            SB_ITEM_TYPE_e GetItemTypeFromPoint( const CPoint& point_i );
            // Function to initialize the thumb tracking.
            void BeginThumbTrack( const CPoint& point_i );
            // Function to track the thumb along with the mouse movement.
            void DoThumbTrack( const CPoint& point_i );
            // Function to end the thumb tracking.
            void EndThumbTrack();
            // Function to initialize the scrolling.
            void BeginScroll();
            // Function to continue the scrolling.
            void DoScroll();
            // Function to end the scrolling.
            void EndScroll();
            // Function to calculate the scroll position from the screen position of the thumb.
            int  GetPosFromThumbRect();
            // Function to check whether the given point is inside the selected component or not.
            bool IsPointInside( const CPoint& point_i );
            // Function to get the scrollbar message according to the selected component.
            int  GetScrollMsgType();
            // Function to get the message type of scrollbar.
            UINT GetScrollMsg()
            {
                return m_bVertical ? WM_VSCROLL : WM_HSCROLL;
            }
            // Drawing functions
            void UpdateItem( SB_ITEM_TYPE_e eItemType_i );
            void DrawMinBtn( CDC& dc_i );
            void DrawMaxBtn( CDC& dc_i );
            void DrawMinChan( CDC& dc_i );
            void DrawMaxChan( CDC& dc_i );
            void DrawThumb( CDC& dc_i );
            // Mouse capture functions.
            void SetCapture();
            void ReleaseCapture();
            // Function to get the visual state.
            SB_STATE_TYPE_e GetState( SB_ITEM_TYPE_e eItem_i );

        private:
            CWnd*  m_pWndMessage; // Pointer to the window that receives the scroll messages.
            bool   m_bVertical;   // Type of the scrollbar.
            // Data members.
            int    m_nMin;
            int    m_nMax;
            int    m_nPage;
            int    m_nPos;
            // Rects of the components.
            CRect  m_MinBtnRect;
            CRect  m_MaxBtnRect;
            CRect  m_ThumbRect;
            CRect  m_MinChanRect;
            CRect  m_MaxChanRect;
            SB_ITEM_TYPE_e m_eActiveItem;  // Active component.
            bool   m_bItemPressed;      // Component status.
            int    m_nTrackMouseOffset; // Offset of the mouse point in time of thumb tracking.
            int    m_nLastPos;          // Last scroll position.
            bool   m_bSimulateMouseCapture; // Flag that determines the mouse message simulation.
            CPoint m_LastMousePos;      // Mouse position at the last input.
            bool   m_bDisabled;
            // Border colors.
            COLORREF m_clrBorderHigh;
            COLORREF m_clrBorderLow;
            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            bool m_bMouseInside; // Holds mouse hover flag
            bool m_bWndEnabled;  // Hold window enable/disable virtual state
            /**
             * End of Change for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            bool m_bHoverEffect; // Hover effect flag
            SB_ITEM_TYPE_e m_eMouseInsideItem;  // Active component.
            /* Begin: Modification 2.4 for Bug ID #MPC05902(Performance Improvement) */
            bool m_bUseGDIOnly;
            /* End: Modification 2.4 for Bug ID #MPC05902(Performance Improvement) */
        };
    } // End of namespace UICommon
} // End of namespace MPlus

#endif // #ifndef _SCROLLBAREX_H_

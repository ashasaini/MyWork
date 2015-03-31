/**
 * Copyright(c) 2007 TOSHIBA Medical Systems Corporation, All Rights Reserved.
 *
 * TabSpinEx.h - TabSpinEx class declaration file.
 *
 * @author :    Nishad S
 * @version:    1.0            Date:  2007-09-04
 * @author :    Naveen R
 * @version:    1.1            Date:  2008-12-09
 */

#ifndef _TABSPINEX_H_
#define _TABSPINEX_H_

#include "MPlusUICommon/GdiPlusCtrl.h"
#include <afxcmn.h>

namespace MPlus
{
    namespace UICommon
    {
        /**
         * Copyright(c) 2007 TOSHIBA Medical Systems Corporation, All Rights Reserved.
         *
         * TabSpinEx - This class is used to customize the spin control of the tab control.
         *
         * @author :    Nishad S
         * @version:    1.0            Date:  2007-09-04
         */
        class TabSpinEx : public CSpinButtonCtrl
        {
        public:

            TabSpinEx();
            virtual ~TabSpinEx();

            // ClassWizard generated virtual function overrides
            //{{AFX_VIRTUAL(TabSpinEx)
            //}}AFX_VIRTUAL

        protected:

            //{{AFX_MSG(TabSpinEx)
            afx_msg void OnPaint();
            /* Begin: Modification 1.1 #683 */
            // afx_msg BOOL OnEraseBkgnd( CDC* pDC_i );
            /* End: Modification 1.1 #683 */
            afx_msg void OnLButtonDown( UINT nFlags_i, CPoint point_i );
            afx_msg void OnLButtonUp( UINT nFlags_i, CPoint point_i );
            afx_msg void OnMouseMove( UINT nFlags_i, CPoint point_i );
            //}}AFX_MSG
            DECLARE_MESSAGE_MAP()

        protected:

            afx_msg LRESULT OnMouseLeave( WPARAM, LPARAM );

        public:

            friend class TabEx;

        protected:

            // Button types
            enum TSEX_BUTTON_TYPE_e
            {
                TSEXBT_ROUND_LEFT,
                TSEXBT_ROUND_RIGHT
            };

            // Button states
            enum TSEX_BUTTON_STATE_e
            {
                TSEXBS_NORMAL,
                TSEXBS_HILIGHT,
                TSEXBS_PRESSED
            };

        protected:

            // Enable/disable mouse hover effect
            void EnableHoverEffect( bool bEnable_i )
            {
                m_bMouseHoverEnabled = bEnable_i;
            }

            // APIs for setting the colors
            void SetBKColor( COLORREF clr_i );
            void SetNormalFaceColors( COLORREF clrStart_i, COLORREF clrEnd_i );
            void SetPressedFaceColors( COLORREF clrCenter_i, COLORREF clrSurround_i );
            void SetOuterEdgeColors( COLORREF clrLeftTop_i, COLORREF clrRightBottom_i, COLORREF clrShadow_i );
            void SetInnerEdgeColors( COLORREF clrLeftTop_i, COLORREF clrRightBottom_i );
            void SetHighlightColor( COLORREF clr_i );
            void SetArrowColor( COLORREF clr_i );

            // Function to draw the button face
            void DrawButtonFace( HDC hDC_i, LPRECT lpRect_i, TSEX_BUTTON_TYPE_e eType_i, TSEX_BUTTON_STATE_e eState_i );
            // Function to draw the button arrow
            void DrawButtonArrow( HDC hDC_i, LPRECT lpRect_i, TSEX_BUTTON_TYPE_e eType_i, TSEX_BUTTON_STATE_e eState_i );
            // Function to get the client and button rectangles
            void GetRects( CRect& rectClient_o, CRect& rectLeftBtn_o, CRect& rectRightBtn_o );

            // Function to initialize
            void Initialize();

        protected:

            GdiPlusCtrl m_GdiInit;                // Object to initialize the GDI+

            TSEX_BUTTON_STATE_e m_eStateLeftBtn;  // State of the left button
            TSEX_BUTTON_STATE_e m_eStateRightBtn; // State of the right button

            bool m_bMouseInside;                  // Flag for the mouse cursor position
            bool m_bLeftPressed;                  // Flag for left button pressed status
            bool m_bRightPressed;                 // Flag for right button pressed status

            bool m_bMouseHoverEnabled;            // Mouse hover effect flag

            COLORREF m_clrBk;                     // Background color
            COLORREF m_clrFaceStart;              // Normal face gradient starting color
            COLORREF m_clrFaceEnd;                // Normal face gradient ending color
            COLORREF m_clrPressedCenter;          // Pressed face gradient center color
            COLORREF m_clrPressedSurround;        // Pressed face gradient surround color
            COLORREF m_clrOuterEdgeLeftTop;       // Left-top outer edge color
            COLORREF m_clrOuterEdgeRightBottom;   // Right-bottom outer edge color
            COLORREF m_clrOuterEdgeShadow;        // Outer edge shadow color
            COLORREF m_clrInnerEdgeLeftTop;       // Left-top inner edge color
            COLORREF m_clrInnerEdgeRightBottom;   // Right-bottom inner edge color
            COLORREF m_clrEdgeHighlight;          // Edge highlight color
            COLORREF m_clrArrow;                  // Arrow color
        };
    }
}

#endif // #ifndef _TABSPINEX_H_

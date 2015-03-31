/**
 * Copyright(c) 2006 - 2010 TOSHIBA Medical Systems Corporation, All Rights
 * Reserved.
 *
 * DialogEx.h - DialogEx class declaration file
 *
 * @author :    Krishnakumar T G
 * @version:    1.0            Date:  2006-03-10
 *
 * @author :    Praveen Kumar C
 * @version:    2.0            Date:  2007-04-26
 *
 * @author :    Naveen R
 * @version:    2.1            Date:  2007-06-20
 *
 * @author :   Nishad S
 * @version:    2.2            Date:  2007-08-09
 *
 * @author :    Jayasankar.S
 * @version:    2.3            Date:  2008-04-24
 *
 * @author :    Jijeesh K V
 * @version:    2.4            Date:  2008-06-25
 *
 * @author :    Jijeesh K V
 * @version:    2.5            Date:  2008-06-30
 * @author :    Naveen R.
 * @version:    2.6            Date:  2008-12-23
 * @author :    Shypa Padmanabhan     (Modified for MPlusV4)
 * @version:    2.7            Date:  2009-01-09
 * @author :    Asha (Modified for #MPC05061 - Painting issue in DEGL studyList
 *                    when display pages are switched continuously)
 * @version:    2.8            Date:  2009-10-23
 * @author :    Jinesh P.P.  ( Modified for MPlusV4 BugFix Merging #MPC06069 )
 * @version:    2.9           Date:  2010-02-15
 */

#ifndef _DIALOGEX_H_
#define _DIALOGEX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MPlusUICommon/GdiPlusCtrl.h"
/**
 * Added for Celeve PIMS Phase-5 requirement
 * @since  2.0
 */
#include <afxdtctl.h>
/**
 * End of Change  for Celeve PIMS Phase-5 requirement
 * @since  2.0
 */

#ifndef _UNICODE
#error Unicode definition is necessary to use this class
#endif

#ifdef UI_CONTROLS_DLL
#define CONTROLS_CLASS_API __declspec( dllexport )
#else
#define CONTROLS_CLASS_API __declspec( dllimport )
#endif

namespace MPlus
{
    namespace UICommon
    {
        /**
         * Copyright(c) 2006 - 2007  TOSHIBA Medical Systems Corporation, All Rights
         * Reserved.
         *
         * WINDOW_COLORS_t - Structure holding the window drawing colors
         *
         * @author :    Krishnakumar T G
         * @version:    1.0            Date:  2006-03-02
         *
         * @author :    Praveen Kumar C
         * @version:    2.0            Date:  2007-04-26
         *
         * @author :    Naveen R
         * @version:    2.1            Date:  2007-06-20
         *
         * @author :    Nishad S
         * @version:    2.2            Date:  2007-08-09
         *
         * @author :    Jayasankar.S
         * @version:    2.3            Date:  2008-04-24
         *
         * @author :    Jijeesh K V
         * @version:    2.4            Date:  2008-06-25
         *
         * @author :    Jijeesh K V
         * @version:    2.5            Date:  2008-06-30
         */
        typedef struct WINDOW_COLORS_tag WINDOW_COLORS_t;
        struct WINDOW_COLORS_tag
        {
            // Holds client area color
            COLORREF crClientColor;
            // Holds Title gradient start color
            COLORREF crTitleGradientStart;
            // Holds Title gradient end color
            COLORREF crTitleGradientEnd;
            // Holds the border shadow color
            COLORREF crBorderShadow;
            // Holds Caption color
            COLORREF crCaption;
            // Holds Line just beneath the title bar
            COLORREF crTopBorder; // This member variable is not using now.
            /* Begin: Added 2.5 */
            // Holds Title Blink start color
            COLORREF crTitleBlinkStart;
            // Holds Title Blink end color
            COLORREF crTitleBlinkEnd;
            /* End: Added 2.5 */
        };


        /**
         * Copyright(c) 2006 - 2007 TOSHIBA Medical Systems Corporation, All Rights
         * Reserved.
         *
         * DialogEx - DialogEx class declaration
         *
         * Traceability Tags : TCOM_MP_CS_UI_DIALOGEX_DEFAULT_CONSTRUCTOR
         *                     TCOM_MP_CS_UI_DIALOGEX_ADD_DIVIDER_LINE
         *
         * @author :    Krishnakumar T G
         * @version:    1.0            Date:  2006-03-10
         *
         * @author :    Praveen Kumar C
         * @version:    2.0            Date:  2007-04-26
         */
        class CONTROLS_CLASS_API DialogEx : public CDialog
        {

        public:

            // To change the close button visibility
            void ShowCloseButton( bool bFlag_i = true );
            // To change the title bars' visibility
            void ShowTitleBar( bool bFlag_i = true );
            // Gets the window border color
            COLORREF GetBorderColor();
            // Gets the window drawing colors
            void GetWindowColors( WINDOW_COLORS_t& stColors_o );
            // Sets the window colors
            void SetWindowColors( WINDOW_COLORS_t& stColors_i );
            // Sets the window border color
            void SetBorderColor( COLORREF crColor_i );
            // Gets the bottom ribbon height
            int GetRibbonHeight();
            // Sets the bottom ribbon height
            void SetRibbonHeight( int nRibbonHeight_i );
            // Sets the colors of the divider lines
            void SetTitleBarDividerLineColors( COLORREF crUpper_i, COLORREF crLower_i );
            void SetRibbonDividerLineColors( COLORREF crUpper_i, COLORREF crLower_i );

            // Function to set the font.
            /**
             * Took out for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            //bool SetFont( LPCTSTR lpctszFontName_i, int nPointSize_i );
            /**
             * End of Change  for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */

            /**
             * Took out for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            bool SetFont( LPCTSTR lpctszFontName_i, int nPointSize_i, bool bBold_i = false );
            /**
             * End of Change  for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */

            bool SetTitleBarHeight( int nHeight_i );
            int  GetTitlBarHeight();

            // Standard constructor
            DialogEx( UINT nIDD_i, CWnd* pParent_i = NULL );

            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            
            // Default constructor
            DialogEx();

            /**
            * End of Change for Celeve PIMS Phase-5 requirement
            * @since  2.0
            */

            // Enable/disable mouse hover effect.
            void EnableHoverEffect( bool bEnable_i = true )
            {
                m_bHoverEffect = bEnable_i;
            }

            ~DialogEx();
            // Overridden function to handle Invalidate.
            // This is required to avoid the call to Invalidate when window handle is invalid.
            void Invalidate( BOOL bErase_i = TRUE )
            {
                if( FALSE != IsWindow( m_hWnd ))
                {
                    ::InvalidateRect( m_hWnd, 0, bErase_i );
                }
            }

            /* Begin: Added 2.5 */
            // Override the DoMOdal function for setting blinking effect
           /**
            * Modified for MPlus V4
            * @since 2.7
            */
            /// Modified for removing warning "conversion from 'INT_PTR' to 'int', possible loss of data".
            // virtual int DoModal();
            virtual INT_PTR DoModal();
           /**
            * End of Modification for MPlus V4
            */
            /* End: Added 2.5 */

            // ClassWizard generated virtual function overrides
            //{{AFX_VIRTUAL(DialogEx)
            protected:
            virtual void DoDataExchange( CDataExchange* pDX_i );    // DDX/DDV support
            virtual void PreSubclassWindow();
            virtual LRESULT WindowProc( UINT nMessage_i, WPARAM wParam_i, LPARAM lParam_i );
            //}}AFX_VIRTUAL

        public:

            // Button states
            enum STATE_e
            {
                STATE_NORMAL,
                STATE_PRESSED,
                STATE_DISABLED
            };

        protected:

            // Generated message map functions
            //{{AFX_MSG(DialogEx)
            afx_msg void OnNcPaint();
            afx_msg void OnNcCalcSize( BOOL bCalcValidRects_i, NCCALCSIZE_PARAMS FAR* pstncsp_i );
           /**
            * Modified for MPlus V4
            * @since 2.7
            */
            /// Modified for correcting the error "'static_cast' :cannot convert from 'UINT (__thiscall 
            /// MPlus::UICommon::DialogEx::* )(CPoint)' to 'LRESULT (__thiscall CWnd::* )(CPoint)'"
            // afx_msg UINT OnNcHitTest( CPoint Point_i );
            afx_msg LRESULT OnNcHitTest( CPoint Point_i );
           /**
            * End of Modification for MPlus V4
            */
            afx_msg BOOL OnEraseBkgnd( CDC* pDC_i );
            afx_msg void OnNcLButtonUp( UINT nHitTest_i, CPoint Point_i );
            afx_msg void OnNcLButtonDown( UINT nHitTest_i, CPoint Point_i );
            virtual BOOL OnInitDialog();
            afx_msg void OnSize( UINT uType_i, int nWidth_i, int nHeight_i );
            afx_msg void OnSizing( UINT uSide_i, LPRECT pRect_o );
            /* Begin: Added 2.5 */
            afx_msg BOOL OnNcActivate( BOOL bActive_o );

            /* Begin: Modification 2.6 for Bug ID #695 */
            //afx_msg void OnKillFocus( CWnd* pNewWnd_o );
            //afx_msg void OnSetFocus( CWnd* pOldWnd_o );
            /* End: Modification 2.6 for Bug ID #695 */

            afx_msg void OnDestroy();
            /* End: Added 2.5 */
            //}}AFX_MSG
            afx_msg void OnStyleChanged( int nStyleType_i, LPSTYLESTRUCT pstStyleStruct_i );


            DECLARE_MESSAGE_MAP()

        private:

            // To track the mouse movement
            void TrackMouseMovement( bool bMouseOver_i );
            // Draw the button visuals
            //void DrawButton( HDC hDC_i, const RECT& rect_i, STATE_e eState_i = STATE_NORMAL );
            void DialogEx::DrawButton( HDC hDc_i );

        private:

            // Top Border line color
            COLORREF m_clrTopBorder;
            // Border line color
            COLORREF m_clrLine;

            /**
             * Tookout for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */

            // // Border line color
            // COLORREF m_clrRibbonTopLine;

            /**
             * End of Change for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */

            // Holds client area color
            COLORREF m_crClientColor;
            // Holds Title gradient start color
            COLORREF m_crTitleGradientStart;
            // Holds Title gradient end color
            COLORREF m_crTitleGradientEnd;
            // Holds the Border color
            COLORREF m_crBorderShadow;
            // Holds the Caption color
            COLORREF m_clrDialogText;
            // Holds upper edge Gradient starting color
            COLORREF m_crBrushEdgeStart;
            // Holds upper edge Gradient ending color
            COLORREF m_crBrushEdgeEnd;
            // Holds Normal face gradient starting color
            COLORREF m_crBrushFaceStart;
            // Holds Normal face gradient ending color
            COLORREF m_crBrushFaceEnd;
            // Holds Pushed face gradient starting color
            COLORREF m_crPushedFaceStart;
            // Holds Pushed face gradient ending color
            COLORREF m_crPushedFaceEnd;
            // Holds Circle gradient starting color
            COLORREF m_crbrushCircleStart;
            // Holds Circle gradient ending color
            COLORREF m_crbrushCircleEnd;
            // Holds Upper edge shadow color
            COLORREF m_crUpperEdgeShadow;
            // Holds Bottom edge color
            COLORREF m_crBottomEdge;
            // Holds pushed state bottom edge color
            COLORREF m_crBottomEdgePushed;
            // Holds bottom edge shadow color
            COLORREF m_crBottomEdgeShadow;
            // Holds Mouse hover color
            COLORREF m_crMouseHover;
            // Holds disabled button color
            COLORREF m_crDisabled;
            // Holds pressed state center color
            COLORREF m_crPressedCenter;
            // Holds pressed state Surround color
            COLORREF m_crPressedSurround;
            // Holds the close button cross colors
            COLORREF m_clrCloseBtnCroseHigh;
            // Holds the background color of the button
            COLORREF m_crBack;
            // Holds the close button status
            STATE_e m_CloseStatus;
            // Holds the bottom ribbon height
            int m_nBottomRibbon;
            // Holds the title bar height
            int m_nTitileHeight;
            // Holds the width of the border
            int m_nBorderwidth;
            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */

            // Holds divider line upper color
            COLORREF m_clrDividerLineUpper;
            // Holds divider line upper color
            COLORREF m_clrDividerLineLower;

            /**
             * End of Change for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */

            // Holds ribbon divider line upper color
            COLORREF m_clrRibbonDividerLineUpper;
            // Holds ribbon divider line upper color
            COLORREF m_clrRibbonDividerLineLower;

            /* Begin: Added 2.5 */
            // Holda normal title colors
            COLORREF m_clrNormalTitleStart;
            COLORREF m_clrNormalTitleEnd;
            // Holds color for blinking title bar
            COLORREF m_clrTitleBlinkColorStart;
            COLORREF m_clrTitleBlinkColorEnd;
            /* End: Added 2.5 */

            // Variable that holds the close button width;
            int m_nCloseButttonWidth;
            // Font to draw the caption of dialog
            CFont *m_pFont;
            // Object to initialize the gdi plus
            GdiPlusCtrl m_GdiInit;
            // flag that indicates the nccalcsize to ignore the current change in style
            bool m_bIgnoreStyleChange;
            // Holds mouse hover flag
            bool m_bHoverFlag;
            // Holds Caption status
            bool m_bCaptionFlag;
            // Holds the flag for close button visibility
            bool m_bShowCloseButton;
            bool    m_bHoverEffect; // Hover effect flag
            /* Begin: Added 2.3 */
            bool m_bResize; // Holds resizing flag
            /* End: Added 2.3 */
            /* Begin: Added 2.4 */
            CRect m_PreviousRect; // Holds the rectangle before resizing
            /* End: Added 2.4 */
            /* Begin: Added 2.5 */

            /* Begin: Modification 2.6 for Bug ID #695, m_bInactiveDlg removed */
            //bool m_bInactiveDlg; // flag to know inactive state
            /* End: Modification 2.6 for Bug ID #695, m_bInactiveDlg removed */

            bool m_bModalDialog; // Flag to know modal state
            /* End: Added 2.5 */
            /* Begin: Modification 2.8 for Bug ID #MPC05061 */
            bool m_bActivated;
            /* End: Modification 2.8 for Bug ID #MPC05061 */
            /* Begin: Modification 2.9 for Bug ID #MPC06069 */
            COLORREF m_clrRibbonGradientStart;
            COLORREF m_clrRibbonGradientEnd;
            /* End: Modification 2.9 for Bug ID #MPC06069 */
        };
    } // End of namespace UICommon
} // End of namespace MPlus

#endif // _DIALOGEX_H_

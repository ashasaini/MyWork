/**
 * Copyright(c) 2006 - 2007 TOSHIBA Medical Systems Corporation, All Rights
 * Reserved.
 *
 * TabEx.h - TabEx class declaration file. This class is derived from CTabCtrl
 *
 * @author :    Krishnakumar T G
 * @version:    1.0            Date:  2006-03-06
 *
 * @author :    Praveen Kumar C
 * @version:    2.0            Date:  2006-04-27
 * @author :    Nishad S
 * @version:    2.1            Date:  2007-06-21
 * @author :    Naveen R
 * @version:    2.2            Date:  2008-12-22
 */

#ifndef _TABEX_H_
#define _TABEX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef _UNICODE
#error Unicode definition is necessary to use this class
#endif

#ifdef UI_CONTROLS_DLL
#define CONTROLS_CLASS_API __declspec( dllexport )
#else
#define CONTROLS_CLASS_API __declspec( dllimport )
#endif

// Header files
#include <afxtempl.h>
#include "MPlusUICommon/SpinEx.h" // Kept for compatibility
#include "MPlusUICommon/TabSpinEx.h"

namespace MPlus
{
    namespace UICommon
    {
        /**
         * Copyright(c) 2006 - 2007 TOSHIBA Medical Systems Corporation, All Rights
         * Reserved.
         *
         * TAB_COLORS_t - Declaration of Tab drawing colors structure
         *
         * @author :    Krishnakumar T G
         * @version:    1.0            Date:  2006-03-07
         *
         * @author :    Praveen Kumar C
         * @version:    2.0            Date:  2006-04-27
         */
        typedef struct TAB_COLORS_tag TAB_COLORS_t;
        struct TAB_COLORS_tag
        {
            COLORREF clrBackGround;
            COLORREF clrParentBackGround;
            COLORREF clrActBorder;
            COLORREF clrActBorderShadow;
            COLORREF clrActTabGradientStart;
            COLORREF clrActTabGradientEnd;
            COLORREF clrInActBorder;
            COLORREF clrInActBorderShadow;
            COLORREF clrInActTabGradientStart;
            COLORREF clrInActTabGradientEnd;
            COLORREF clrMouseHover;

            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */

            COLORREF clrPageBorderShadow;
            COLORREF clrDisabled;

            /**
             * End of Change for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
        };


        /**
         * Copyright(c) 2006 - 2007 TOSHIBA Medical Systems Corporation, All Rights
         * Reserved.
         *
         * TabEx - Declaration of TabEx class
         *
         * Traceability Tags : TCOM_MP_CS_UI_TABEX_SET_COLORS
         *                     TCOM_MP_CS_UI_TABEX_SET_FONT
         *                     TCOM_MP_CS_UI_TABEX_SET_TEXTPOSITION
         *                     TCOM_MP_CS_UI_TABEX_SET_TAB_STATE
         *
         * @author :    Krishnakumar T G
         * @version:    1.0            Date:  2006-03-07
         *
         * @author :    Praveen Kumar C
         * @version:    2.0            Date:  2006-04-27
         */
        class CONTROLS_CLASS_API TabEx : public CTabCtrl
        {

        public:

            // Constructor
            TabEx();
            // Destructor
            virtual ~TabEx();

        public:

            // Button states
            enum TextAlignment_e
            {
                ALIGNLEFT = 0,
                ALIGNCENTER,
                ALIGNRIGHT
            };

            enum TextVertAlignment_e
            {
                ALIGN_NONE    = -1,
                ALIGN_TOP,
                ALIGN_VCENTER,
                ALIGN_BOTTOM,
                ALIGN_TOP_CENTER,
                ALIGN_BOTTOM_CENTER
            };

            // Hover effect flags
            enum HoverEffect_e
            {
                HOVER_CONTROL       = 0x0001,
                HOVER_CHILD_CONTROL = 0x0002
            };

        public:

            // ClassWizard generated virtual function overrides
            //{{AFX_VIRTUAL(TabEx)
            protected:
            virtual void PreSubclassWindow();
            virtual void DrawItem( LPDRAWITEMSTRUCT pstDrawItemStruct_i );
            virtual LRESULT WindowProc( UINT uMsg_i, WPARAM wParam_i, LPARAM lParam_i );
            //}}AFX_VIRTUAL

        public:

            // Set the primary text alignment
            void AlignPrimaryText( TextAlignment_e eAlign_i );
            bool AlignPrimaryText( int nIndex_i, TextAlignment_e eAlign_i );
            // Set the secondary text alignment
            void AlignSecondaryText( TextAlignment_e eAlign_i );
            bool AlignSecondaryText( int nIndex_i, TextAlignment_e eAlign_i );
            // Gets the Inactive text color
            COLORREF GetInActiveTextColor();
            // Sets the inactive text color
            void SetInActiveTextColor( const COLORREF clrColor_i );
            // Set the active text color for disabled tab
            void SetActiveTextColorDisabled( const COLORREF clrColor_i );
            // Set the inactive text color for disabled tab
            void SetInActiveTextColorDisabled( const COLORREF clrColor_i );
            // Gets Active text color
            COLORREF GetActiveTextColor();
            // Sets Active text color
            void SetActiveTextColor( const COLORREF clrColor_i );
            // Get Focus rectangle color
            COLORREF GetFocusRectColor();
            // Sets Focus rectangle color
            void SetFocusRectColor( const COLORREF clrColor_i );
            // To get the tab drawing colors
            void GetTabColors( TAB_COLORS_t& stColors_o );
            // to set the tab drawing colors
            void SetTabColors( const TAB_COLORS_t& stColors_i );

            /**
             * Took out for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            // To set the primary tab text font
            //void SetPrimaryTextFont( CString csFontName_i, int nSize_i );
            // To set the secondary tab text font
            //void SetSecondaryTextFont( CString csFontName_i, int nSize_i );
            /**
             * End of Change  for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */

            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            // To set the primary tab text font
            void SetPrimaryTextFont( CString csFontName_i, int nSize_i, bool bBold_i = false );
            // To set the secondary tab text font
            void SetSecondaryTextFont( CString csFontName_i, int nSize_i, bool bBold_i = false );
            /**
             * End of Change  for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */

            // Overridden function to handle Invalidate.
            // This is required to avoid the call to Invalidate when window handle is invalid.
            void Invalidate( BOOL bErase_i = TRUE )
            {
                if( FALSE != IsWindow( m_hWnd ))
                {
                    ::InvalidateRect( m_hWnd, 0, bErase_i );
                }
            }
            // To set the primary text position.
            void SetPrimaryTextPos( int nIndex_i, int nXPos_i, int nYPos_i );
            // To set the secondary text position.
            void SetSecondaryTextPos( int nIndex_i, int nXPos_i, int nYPos_i );

            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */

            bool    Attach( CTabCtrl *pTab_i );
            bool    SetTabEnable( int bIndex_i, BOOL bEnable_i = FALSE );
            BOOL    IsTabEnable( int nIndex_i );

            /**
             * End of Change for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */

            // Enable/disable mouse hover effect.
            void EnableHoverEffect( bool bEnable_i = true )
            {
                m_bHoverEffect = bEnable_i;
                m_bChildHoverEffect = bEnable_i;
                m_SpinCtrl.EnableHoverEffect( bEnable_i );
            }

            // Function To set the vertical alignment of the text;
            bool SetVerticalTextAlignment( int nIndex_i, bool bPrimaryText_i, TextVertAlignment_e eVertAlign_i );

            // APIs for setting the colors of spin control
            void SetSpinNormalFaceColors( COLORREF clrStart_i, COLORREF clrEnd_i );
            void SetSpinPressedFaceColors( COLORREF clrCenter_i, COLORREF clrSurround_i );
            void SetSpinOuterEdgeColors( COLORREF clrLeftTop_i, COLORREF clrRightBottom_i, COLORREF clrShadow_i );
            void SetSpinInnerEdgeColors( COLORREF clrLeftTop_i, COLORREF clrRightBottom_i );
            void SetSpinHighlightColor( COLORREF clr_i );
            void SetSpinArrowColor( COLORREF clr_i );

            // Hover effect
            void SetHoverEffect( int nApplyTo_i, bool bEnable_i );
            bool GetHoverEffect( int nApplyTo_i );

        protected:

            //{{AFX_MSG(TabEx)
            afx_msg BOOL OnEraseBkgnd( CDC* pDC_i );
            afx_msg void OnPaint();
            afx_msg void OnMouseMove(UINT nFlags, CPoint point);
            afx_msg LRESULT OnMouseLeave( WPARAM nWParam_i, LPARAM lLParam_i );
            afx_msg BOOL OnSelchanging( NMHDR* pNMHDR_i, LRESULT* pResult_i );
            afx_msg void OnEnable( BOOL bEnable_i );
            //}}AFX_MSG

            DECLARE_MESSAGE_MAP()

        private:

            // To find the Default spin control of the tab control
            HWND GetDefaultSpinControl();
            // To show the spin control
            void ShowSpinControl();

            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */

            // Function for send selection change notification
            // void SendSelChangeNotification( );
			bool SendSelChangeNotification( UINT uCode_i );

            /**
             * End of Change for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */

            // To set the position of the spin control
            void SetSpinPos();
            // To get a specified image from the image list.
            void GetBitmapImage( CDC *pBufferDC_i, CImageList *pImageList_i, int nImage_i, CBitmap* pDestBitmap_o );

        private:

            typedef struct
            {
                POINT PrimaryTextPos;
                POINT SecondaryTextPos;
                bool  bPrimaryTextAlignEnabled;
                bool  bSecondaryTextAlignEnabled;
                int   nPrimaryTextAlign;
                int   nSecondaryTextAlign;
                TextVertAlignment_e ePrimayVertAlign;
                TextVertAlignment_e eSecondaryVertAlign;
            } TEXTPOS_st;

        private:

            // Object to initialize the gdi plus
            GdiPlusCtrl m_GdiInit;
            // Holds the primary tab text alignment value
            int m_nPrimaryTextAlign;
            // Holds the secondary tab text alignment value
            int m_nSecondaryTextAlign;
            // Holds the inactive text color
            COLORREF m_clrInActText;
            // Holds the active text color
            COLORREF m_clrActText;
            // Holds the background color
            COLORREF m_clrBackGround;
            // Holds the background color of the Tab control.
            COLORREF m_clrTabBackground;
            // Holds the active tab border color
            COLORREF m_clrActBorder;
            // Holds the active border shadow color
            COLORREF m_clrActBorderShadow;
            // Holds the inactive border color
            COLORREF m_clrInActBorder;
            // Holds the inactive border shadow color
            COLORREF m_clrInActBorderShadow;
            // Holds the active tab gradient start color
            COLORREF m_clrActTabGradientStart;
            // Holds the active tab gradient end color
            COLORREF m_clrActTabGradientEnd;
            // Holds the inactive tab gradient start color
            COLORREF m_clrInActTabGradientStart;
            // Holds the inactive tab gradient end color
            COLORREF m_clrInActTabGradientEnd;
            // Holds the mouse hover color
            COLORREF m_clrMouseHover;
            // Holds the focus rectangle color
            COLORREF m_clrFocusRect;
            // Holds secondary tab text font size
            int m_nSecFontSize;
            // DIsabled text color for active tab
            COLORREF m_clrDisableActText;
            // Disabled text color for inactive tab
            COLORREF m_clrDisableInActText;

            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            // Holds font state, whether bold or not
            /* Begin: Modification 2.2 #718*/
            // Set primary text font directly to the control instead of keeping it as member.
            // bool m_bPrimaryFontBold;
            /* End: Modification 2.2 #718*/
            bool m_bSecondaryFontBold;
            /**
             * End of Change  for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */

            // Holds secondary tab text font name
            CString m_csSecFontName;
            
            /* Begin: Modification 2.2 #718*/
            // Set primary text font directly to the control instead of keeping it as member.
            // Holds primary tab text font size
            // int m_nPrimFontSize;
            // Holds primary tab text font name
            // CString m_csPrimFontName;
            /* End: Modification 2.2 #718*/

            // Holds Spin control
            TabSpinEx m_SpinCtrl;
            // Holds the index of the mouse hover tab
            int m_nHoverTab;
            // Holds the mouse hover flag
            bool m_bMouseHover;
            // Array to hold text positions.
            CArray<TEXTPOS_st, const TEXTPOS_st&> m_TextPosArray;
            
            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */

            COLORREF m_clrPageBorderShadow;
            COLORREF m_clrDisabled;
            CArray<bool,bool> m_StateArray;

            /**
             * End of Change for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            bool m_bHoverEffect;      // Hover effect flag for main control
            bool m_bChildHoverEffect; // Hover effect flag for child control
        };
    } // End of namespace UICommon
} // End of namespace MPlus

#endif // _TABEX_H_

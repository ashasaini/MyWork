/**
 * Copyright(c) 2006 - 2010 TOSHIBA Medical Systems Corporation, All Rights
 * Reserved.
 *
 * ButtonEx.h - ButtonEx class declaration file
 *
 * @author :    Krishnakumar T G
 * @version:    1.0            Date:  2006-02-14
 * @author :    Vinod N N
 * @version:    2.0            Date:  2007-04-24
 * @author :    Naveen R
 * @version:    2.1            Date:  2007-06-11
 * @author :    Jijeesh K V
 * @version:    2.2            Date:  2008-06-12
 * @author :    Naveen.R
 * @version:    2.3            Date:  2008-08-05
 * @author :    Naveen.R
 * @version:    2.4            Date:  2008-10-20
 * @author :    Shypa Padmanabhan     (Modified for MPlusV4)
 * @version:    2.5            Date:  2009-01-09
 * @author :    Asha (Modified for #MPC05368 - To handle Double click event in ButtonEx.)
 *                   (Modified for MR PIMS Performance improvement to avoid flickering.)
 * @version:    2.6            Date:  2009-10-23
 * @author :    Jinesh P.P.  ( Modified for MPlusV4 BugFix Merging MPC05949)
 * @version:    2.7            Date:  2010-01-19
 * @author :    Jinesh P.P.  ( Modified for MPlusV4 BugFix Merging #MPC05896(Performance Improvement))
 * @version:    2.8            Date:  2010-02-15
 * @author :    Jinesh P.P.  ( Modified for MPlusV4 BugFix Merging #MPC06152 )
 * @version:    2.9            Date:  2010-03-12
 */

#ifndef _BUTTONEX_H_
#define _BUTTONEX_H_

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

#include "MPlusUICommon/GdiPlusCtrl.h"

/**
 * Added for Celeve PIMS Phase-5 requirement
 * @since  2.0
 */
#include <afxcmn.h>
/**
 * End of Change  for Celeve PIMS Phase-5 requirement
 * @since  2.0
 */

/* Begin: Modification 2.3 */
// Forward Declaration
namespace Gdiplus
{
    class Graphics;
}
/* End: Modification 2.3 */

namespace MPlus
{
    namespace UICommon
    {
        // Forward declaration of class AVICtrl
        class AVICtrl;
        // Default color definitions
        const COLORREF CLR_AVIBACKCOLOR     = RGB( 255, 0, 255 );
        const COLORREF CLR_PICTUREBACKCOLOR = RGB( 255, 0, 255 );

        /**
         * Copyright(c) 2006 - 2007 TOSHIBA Medical Systems Corporation, All Rights
         * Reserved.
         *
         * BUTTON_COLORS_t - Holds the Button drawing colors
         *
         * @author :    Krishnakumar T G
         * @version:    1.0            Date:  2006-03-02
         * @author :    Vinod N N
         * @version:    2.0            Date:  2007-04-24
         */
        typedef struct BUTTON_COLORS_tag BUTTON_COLORS_t;
        struct BUTTON_COLORS_tag
        {
            // Background color
            COLORREF crBack;
            /**
             * Took out for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            // Holds the caption color
            // COLORREF crCaption;
            /**
             * End of Change  for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            // Holds the primary caption colors
            COLORREF crPrimaryCaptionNormal;
            COLORREF crSecondaryCaptionNormal;
            COLORREF crPrimaryCaptionPressed;
            COLORREF crSecondaryCaptionPressed;
            /**
             * End of Change  for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            // Holds the disable caption color
            COLORREF crDisabledCaption;
            // Holds upper edge Gradient starting color
            COLORREF crBrushEdgeStart;
            // Holds upper edge Gradient ending color
            COLORREF crBrushEdgeEnd;
            // Holds Normal face gradient starting color
            COLORREF crBrushFaceStart;
            // Holds Normal face gradient ending color
            COLORREF crBrushFaceEnd;
            // Holds Pushed face gradient starting color
            COLORREF crPushedFaceStart;
            // Holds Pushed face gradient ending color
            COLORREF crPushedFaceEnd;
            // Holds Circle gradient starting color
            COLORREF crbrushCircleStart;
            // Holds Circle gradient ending color
            COLORREF crbrushCircleEnd;
            // Holds Upper edge shadow color
            COLORREF crUpperEdgeShadow;
            // Holds Bottom edge color
            COLORREF crBottomEdge;
            // Holds pushed state bottom edge color
            COLORREF crBottomEdgePushed;
            // Holds bottom edge shadow color
            COLORREF crBottomEdgeShadow;
            // Holds Mouse hover color
            COLORREF crMouseHover;
            // Holds disabled button color
            COLORREF crDisabled;
            /* Begin: Modification 2.4 */
//             /* Begin: Modification 2.3 */
//             // Holds Edge color of button in disabled state
//             COLORREF crDisabledEdge;
//             /* End: Modification 2.3 */
            // Holds outer Bottom Edge color of button.
            COLORREF crOuterBtmEdge;
            /* End: Modification 2.4 */
            // Holds pressed state center color
            COLORREF crPressedCenter;
            //.Holds pressed state Surround color
            COLORREF crPressedSurround;
            // Holds the disabled state picture blend color
            COLORREF crPictureBlend; // This color is not using now.
            // Holds focus rect color
            COLORREF crFocusRect;
        };

        // The curve type
        enum CURVE_SIZE_e
        {
            CURVE_LARGE_5,
            CURVE_SMALL_3
        };


        /* Begin: Modification 2.3. Strcture no more used */
//         /**
//          * Copyright(c) 2006 TOSHIBA Medical Systems Corporation, All Rights
//          * Reserved.
//          *
//          * BUTTON_PARAMS_t - BUTTON_PARAMS_t structure declaration
//          *
//          * @author :    Krishnakumar T G
//          * @version:    1.0            Date:  2006-03-10
//          */
//         typedef struct BUTTON_PARAMS_tag BUTTON_PARAMS_t;
//         struct BUTTON_PARAMS_tag
//         {
//             // Holds the button edge curve type
//             int RoundType;
//             // Holds flag of push button enabled state
//             bool bCheckEnable;
//             // Holds the push button state of the button
//             bool bPushState;
//             // Holds the mouse hover flag for the button
//             bool bHoverFlag;
//             /* Begin: Added 2.2 */
//             // Holds focus flag
//             bool bFocusflag;
//             /* End: Added 2.2 */
//             // Holds Primary display text
//             CString csPrimaryText;
//             // Holds Primary text font name
//             CString csPrimFontName;
//             // Holds Primary text font size
//             int nPrimFontSize;
//             // Holds Primary text bold type
//             bool bPrimFontBold;
//             // Holds coordinate of primary text display position
//             CPoint PrimTextPos;
//             // Holds Secondary display text
//             CString csSecondaryText;
//             // Holds secondary text font name
//             CString csSecFontName;
//             // Holds secondary text bold type
//             bool bSecFontBold;
//             // Holds Secondary text font size
//             int nSecFontSize;
//             // Holds coordinate of Secondary text display position
//             CPoint SecTextPos;
//             // Holds coordinate of picture or animation
//             CPoint PicturePos;
//             // Holds flag of the AVI file open status
//             bool bFileOpen;
//             // Holds the window handle of the button.
//             HWND hButtonWnd;
//
//             CURVE_SIZE_e eCurve;
//         };
        /* End: Modification 2.3 */


        // Button types
        enum ButtonType
        {
            TUSBUTTON_ROUND_NONE = 0,
            TUSBUTTON_ROUND_ALL,
            TUSBUTTON_ROUND_RIGHT,
            TUSBUTTON_ROUND_LEFT,
            TUSBUTTON_ROUND_TOP,
            TUSBUTTON_ROUND_BTM,
            TUSBUTTON_ROUND_CIRCLE,
        };

        // Text align.
/* Begin: Modification 2.8 for Bug ID #MPC05896(Performance Improvement) */
// To avoid conflict between BitmapButtonEx and ButtonEx
#ifndef MPLUSUICOMMON_BUTTON_ALIGNTYPE_E_DEFINED
#define MPLUSUICOMMON_BUTTON_ALIGNTYPE_E_DEFINED
        enum ALIGNTYPE_e
        {
            TUSBUTTON_ALIGN_NONE,
            TUSBUTTON_ALIGN_LEFT_TOP,
            TUSBUTTON_ALIGN_LEFT_VCENTER,
            TUSBUTTON_ALIGN_LEFT_BOTTOM,
            TUSBUTTON_ALIGN_HCENTER_TOP,
            TUSBUTTON_ALIGN_HCENTER_VCENTER,
            TUSBUTTON_ALIGN_HCENTER_BOTTOM,
            TUSBUTTON_ALIGN_RIGHT_TOP,
            TUSBUTTON_ALIGN_RIGHT_VCENTER,
            TUSBUTTON_ALIGN_RIGHT_BOTTOM
        };
#endif
/* End: Modification 2.8 for Bug ID #MPC05896(Performance Improvement) */

        /**
         * Copyright(c) 2007 TOSHIBA Medical Systems Corporation, All Rights
         * Reserved.
         *
         * ButtonEx - Extended button class, derived from CButton.
         *
         * Traceability Tags : TCOM_MP_CS_UI_BUTTONEX_SET_COLORS
         *
         * @author :    Krishnakumar T G
         * @version:    1.0            Date:  2006-02-14
         * @author :    Vinod N N
         * @version:    2.0            Date:  2007-04-24
         */
        class CONTROLS_CLASS_API ButtonEx : public CButton
        {

        public:

            // Constructor
            ButtonEx();
            // virtual Destructor
            virtual ~ButtonEx();

        public:


            // Button states
            enum STATE_e
            {
                STATE_NORMAL,
                STATE_PRESSED,
                STATE_DISABLED
            };

        public:

            // ClassWizard generated virtual function overrides
            //{{AFX_VIRTUAL(ButtonEx)
            public:
            virtual void DrawItem( LPDRAWITEMSTRUCT pstDrawItemStruct_i );
            protected:
            virtual void PreSubclassWindow();
            //}}AFX_VIRTUAL

        public:

            // To set an icon to the button
            bool SetIcon( HICON hIcon_i );//Added by NeST for 2.1 ver requirment
            // To set the curve radius
            void SetCurveType( CURVE_SIZE_e eCurve_i );
            // To set the bitmap to the button
            void SetBitmap( HBITMAP hBitmap_i, COLORREF clrTransparent_i = CLR_PICTUREBACKCOLOR );
            // To get the button drawing colors
            void GetButtonColors( BUTTON_COLORS_t& stColors_o );
            // To set the button drawing colors
            void SetButtonColors( BUTTON_COLORS_t& stColors_i );
            // To set the secondary text position

           /**
            * Modified for MPlus V4
            * @since 2.5
            */
            //void SetSecondaryTextFont( CString csFontName_i, int nSize_i, bool bBold_i = false );
            void SetSecondaryTextFont( const CString& csFontName_i, int nSize_i, bool bBold_i = false );
            // to set the primary text font
            //void SetPrimaryTextFont( CString csFontName_i, int nSize_i, bool bBold_i = false );
            void SetPrimaryTextFont( const CString& csFontName_i, int nSize_i, bool bBold_i = false );
           /**
            * End of Modification for MPlus V4
            */

            // To set the picture position
            void SetPicturePos( int nPosX, int nPosY );
            // To get the secondary text position
            void GetSecondaryTextPos( CPoint& TextPos );
            // To set the secondary text position
            void SetSecondaryTextPos( int nPosX, int nPosY );
            // To get the primary text position
            void GetPrimaryTextPos( CPoint& TextPos );
            // To set primary text position
            void SetPrimaryTextPos( int nPosX, int nPosY );
            // To set primary text align
            void SetPrimaryTextAllign( ALIGNTYPE_e eAlign_i );
            // To set secondary text align
            void SetSecondaryTextAllign( ALIGNTYPE_e eAlign_i );
            // To get the secondary text
            CString GetSecondaryText();

           /**
            * Modified for MPlus V4
            * @since 2.5
            */
            // To set the secondary text
            //void SetSecondaryText( CString csText );
            void SetSecondaryText( const CString& csText );
           /**
            * End of Modification for MPlus V4
            */

            // To trigger to get the mouse leave message
            void TrackMouseMovement( bool bMouseOver_i );
            // To set the animation file
            void SetAnimation( CString& csFileName_i, COLORREF clrTransparent_i = CLR_AVIBACKCOLOR );
            // To stop or pause the animation
            void StopAnimation( BOOL bResetToFirst_i );
            // To play the animation
            void PlayAnimation();
            // To get the button push (Check) state
            bool GetCheckState();
            // To set the button push (Check) state
            void SetCheckState( bool bPushState_i );
            // To Enable/Disable the check box mode
            void EnableCheckBox( bool bEnable_i );
            // To set button type
            void SetButtonType( ButtonType Type_i );

            /* Begin: Modification 2.3. Functionality moved to the DrawItem() function*/
//            // To draw the button
//             static void DrawButton( HDC hDC_i,
//                                     const RECT& rect_i,
//                                     BUTTON_COLORS_t& stColors_i,
//                                     CImageList& ImageList_i,
//                                     BUTTON_PARAMS_t& stParams_i,
//                                     STATE_e eState_i = STATE_NORMAL,
//                                     ALIGNTYPE_e ePrimaryTextAlign_i = TUSBUTTON_ALIGN_HCENTER_VCENTER,
//                                     ALIGNTYPE_e eSecondaryTextAlign_i = TUSBUTTON_ALIGN_HCENTER_VCENTER );
            /* End: Modification 2.3 */

            // Overridden function to handle Invalidate.
            // This is required to avoid the call to Invalidate when window handle is invalid.
            void Invalidate( BOOL bErase_i = TRUE )
            {
                if( FALSE != IsWindow( m_hWnd ))
                {
                    ::InvalidateRect( m_hWnd, 0, bErase_i );
                }
            }

            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            void SetShadowColor( COLORREF clrShadow_i );
            /**
             * End of Change  for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            // Enable/disable mouse hover effect.
            void EnableHoverEffect( bool bEnable_i = true )
            {
                m_bHoverEffect = bEnable_i;
            }

            // Function to get the bitmap
            static void GetBitmapImage( CDC *pBufferDC_i, CImageList *pImageList_i, int nImage_i, CBitmap* pDestBitmap_o );
            /* Begin: Modification 2.8 for Bug ID #MPC05896(Performance Improvement) */
            void SetButtonBkgndColor( COLORREF clrBtnBkgnd_i );
            /* End: Modification 2.8 for Bug ID #MPC05896(Performance Improvement) */

        protected:

            //{{AFX_MSG(ButtonEx)
            afx_msg UINT OnGetDlgCode();
            /* Begin: Modification 2.6 for MPC05368 */
//            afx_msg void OnLButtonDblClk( UINT uFlags_i, CPoint point_i );
            /* End: Modification 2.6 for MPC05368*/
            afx_msg void OnMouseMove( UINT nFlags_i, CPoint Point_i );
            afx_msg void OnEnable( BOOL bEnable_i );
            afx_msg void OnSize( UINT nType_i, int nCX_i, int nCY_i );
            afx_msg BOOL OnEraseBkgnd( CDC* pDC_i );
            afx_msg void OnWindowPosChanging( WINDOWPOS* pWndPos_i );
            //}}AFX_MSG
            afx_msg LRESULT OnSetStyle( WPARAM wParam_i, LPARAM lParam_i );
            afx_msg LRESULT OnMouseLeave( WPARAM nWParam_i, LPARAM lLParam_i );

            DECLARE_MESSAGE_MAP()

        protected:

            /**
             * Modified for BugID MPC05949. Variable moved from private to protected level.
             * @since  2.7
             */
            /**
             * Modified for MR PIMS Performance improvement.
             * To avoid flickering of check box while mouse move
             * @since 2.6
             */
            bool m_bMousehoverStatus;
            /**
             * End of Change for MR PIMS Performance improvement.
             * @since 2.6
             */
            /**
             * End of Change for BugID MPC05949
             * @since  2.7
             */

        private:

           /**
            * Modified for MPlus V4
            * @since 2.5
            */
            // To prevent default implementation of copy constructor.
            ButtonEx( const ButtonEx& ButtonExObj_i );
            // To prevent Default implementation of = operator.
            ButtonEx& operator=( const ButtonEx& ButtonExObj_i );
           /**
            * End of Modification for MPlus V4
            */

            /* Begin: Modification 2.3 */
            // To draw the button visuals
           // void ButtonVisuals( LPDRAWITEMSTRUCT pstDrawItemStruct_i );

            // Function to draw the text and picture
            /* Begin: Modification 2.9 For Bug Id #MPC06152 */
           /**
            * Modified for MPlus V4
            * @since 2.5
            */
            //void DrawInnerObject( Gdiplus::Graphics& ObjGraphics_i, CDC& DC_i, STATE_e eState_i, CRect rect_i );
            //void DrawInnerObject( Gdiplus::Graphics& ObjGraphics_i, CDC& DC_i, STATE_e eState_i, CRect& rect_i );
           /**
            * End of Modification for MPlus V4
            */
            // Remove the Graphics Usage from this function
            void DrawInnerObject( CDC& DC_i, STATE_e eState_i, CRect& rect_i );
            /* End: Modification 2.9 For Bug Id #MPC06152 */


            /* End: Modification 2.3 */

           /**
            * Modified for MPlus V4
            * @since 2.5
            */
            // To get the alignment
            //static void AlignText( CDC& dc_i, const CString csCaption_i, ALIGNTYPE_e eAlign_i,
            static void AlignText( CDC& dc_i, const CString& csCaption_i, ALIGNTYPE_e eAlign_i,
           /**
            * End of Modification for MPlus V4
            */
                                   const CPoint& pos_i, RECT& Rect_io );
            // To track notification messages.
            BOOL OnChildNotify( UINT uMessage_i, WPARAM wParam_i, LPARAM lParam_i, LRESULT* pLResult_o );



        private:

            // Object to initialize the gdi plus
            GdiPlusCtrl m_GdiInit;
            // Holds check status
            bool m_bPushState;
            // Background color
            COLORREF m_crBack;
            /**
             * Took out for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            // Holds the caption color
            // COLORREF m_crCaption;
            /**
             * End of Change  for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            COLORREF m_crPrimaryCaptionNormal;
            COLORREF m_crSecondaryCaptionNormal;
            COLORREF m_crPrimaryCaptionPressed;
            COLORREF m_crSecondaryCaptionPressed;
            /**
             * End of Change  for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            // Holds the disable caption color
            COLORREF m_crDisabledCaption;
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

            /* Begin: Modification 2.4 */
//             /* Begin: Modification 2.3 */
//             // Holds Edge color of button in disabled state
//             // COLORREF m_crDisabledEdge;
//             /* End: Modification 2.3 */
            COLORREF m_crOuterBtmEdge;
            /* End: Modification 2.4 */


            // Holds pressed state center color
            COLORREF m_crPressedCenter;
            //.Holds pressed state Surround color
            COLORREF m_crPressedSurround;
            // Holds the disabled state picture blend color
            COLORREF m_crPictureBlend;
            // Holds focus rect color
            COLORREF m_crFocusRect;

            // Holds mouse hover flag
            bool m_bHoverFlag;
            /* Begin: Modification 2.3 */
//             /* Begin: Added 2.2 */
//             // Holds focus flag
//             bool m_bHasFocus;
//             /* End: Added 2.2 */
            /* End: Modification 2.3 */

            // Holds check box mode
            bool m_bCheckEnable;

            // Holds the button type
            ButtonType m_RoundType;
            // Holds the Primary text coordinate
            CPoint m_PrimTextPos;
            // Holds the Primary text font name
            CString m_csPrimFontName;
            // Holds the Primary text font size
            int m_nPrimFontSize;
            // Holds primary text bold type
            bool m_bPrimFontBold;
            // Alignment
            ALIGNTYPE_e m_ePrimaryTextAlign;

            // Holds secondary text
            CString m_csSecondaryText;
            // Holds secondary text position
            CPoint m_SecTextPos;
            // Holds Secondary text font name
            CString m_csSecFontName;
            // Holds secondary text font size
            int m_nSecFontSize;
            // Holds secondary text bold type
            bool m_bSecFontBold;
            // Alignment
            ALIGNTYPE_e m_eSecondaryTextAlign;

            // Holds the picture display coordinate
            CPoint m_PicturePos;

            // Animation control
            AVICtrl *m_pAnimation;
            // Holds animation file name
            CString m_csAnimationFile;
            // Holds animation file open status
            bool m_bFileOpen;

            /* Begin: Modification 2.6. Draw the Icon with alpha channel */
            // Image list
            // CImageList m_ImageList;
            HICON m_hPicIcon;
            int m_nPicsHeight;
            int m_nPicsWidth;
            /* End: Modification 2.6. Draw the Icon with alpha channel */

            // Default property.
            bool m_bDefPushBtn;

            // Previous state
            bool m_bPrevPush;
            STATE_e m_ePrevState;
            CURVE_SIZE_e m_eCurveSize;

            // Indicates whether window was repositioned or not.
            // Set to true if window is to be erased.
            bool m_bWindowMoved;
            bool m_bHoverEffect; // Hover effect flag
            /* Begin: Modification 2.8 for Bug ID #MPC05896(Performance Improvement) */
            bool m_bUseWndRgn;
            COLORREF m_clrBckGrnd;
            /* End: Modification 2.8 for Bug ID #MPC05896(Performance Improvement) */
        };
    } // End of namespace UICommon
} // End of namespace MPlus

#endif // _BUTTONEX_H_
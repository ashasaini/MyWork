/**
 * Copyright(c) 2006 - 2009 TOSHIBA Medical Systems Corporation, All Rights
 * Reserved.
 *
 * EditCtrlEx.h - Include FILE for the EditCtrlEx class
 *
 * @author :    Naveen.R
 * @version:    1.0            Date:  2006-02-17
 *
 * @author :    Praveen Kumar. C
 * @version:    2.0            Date:  2007-04-27
 *
 * @author :    Naveen.R
 * @version:    2.1            Date:  2007-06-20
 *
 * @author :    Nishad S
 * @version:    2.2            Date:  2007-08-10
 *
 * @author :    Nishad S
 * @version:    2.3            Date:  2007-09-28
 *
 * @author :    Naveen R
 * @version:    2.4            Date:  2008-09-04
 * @author :    Shypa Padmanabhan     (Modified for MPlusV4)
 * @version:    2.5            Date:  2009-01-09
 */

#ifndef _EDITCTRLEX_H_
#define _EDITCTRLEX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MPlusUICommon/ScrollBarEx.h"

/**
 * Added for Celeve PIMS Phase-5 requirement
 * @since  2.0
 */
#include <afxcmn.h>
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

        // Enum that defines various types of text alignment
        enum TEXT_ALIGN_e
        {
            TOP = 0,
            VCENTER,
            BOTTOM
        };

        /**
         * Added for Celeve PIMS Phase-5 requirement
         * @since  2.0
         */

        // Forward declaration
        class EditExOWnd;
            
        /**
        * End of Change for Celeve PIMS Phase-5 requirement
        * @since  2.0
        */


        /**
         * Copyright(c) 2006 - 2007 TOSHIBA Medical Systems Corporation, All Rights
         * Reserved.
         *
         * EditCtrlEx - The class customizes the edit box control.
         *
         * Traceability Tags : TCOM_MP_CS_UI_EDITCTRLEX_SET_COLORS
         *                     TCOM_MP_CS_UI_EDITCTRLEX_SET_ICON
         *                     TCOM_MP_CS_UI_EDITCTRLEX_MOUSE_OVER_EFFECT
         *
         * @author :    Naveen.R
         * @version:    1.0            Date:  2006-02-17
         *
         * @author :    Praveen Kumar. C
         * @version:    2.0            Date:  2007-04-27
         */
        class CONTROLS_CLASS_API EditCtrlEx : public CEdit
        {

        public:

            // Hover effect flags
            enum HoverEffect_e
            {
                HOVER_CONTROL       = 0x0001,
                HOVER_CHILD_CONTROL = 0x0002
            };

        public:

            // Constructor
            EditCtrlEx();

            // Function to set the background color
            void SetBkColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the background color of read-only edit box
            void SetBkColorReadOnly( COLORREF clrColor_i, bool bInvalidate_i = true );
            
            // Function to set the background color of disabled edit box
            void SetBkColorDisabled( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the text color
            void SetTextColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the text color of read-only edit box
            void SetTextColorReadOnly( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the text color of disabled edit box
            void SetTextColorDisabled( COLORREF clrColor_i, bool bInvalidate_i = false );

            // Function to set the border of edit box
            void SetBorderColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the border of disabled edit box
            void SetBorderColorDisabled( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the border of read-only edit box
            void SetBorderColorReadOnly( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the focus rect color
            void SetFocusRectColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the vertical alignment of the text in edit control
            bool SetTextAlignment( TEXT_ALIGN_e eAlign_i );
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
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            bool SetFont( LPCTSTR lpctszFontName_i, int nPointSize_i, bool bBold_i = false );
            /**
             * End of Change  for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */

            // Function to set the character list that must be filtered from displaying
            void SetFilterChars( LPCTSTR lpctszCharList_i );

            // Returns the character list that must be filtered from displaying
           /**
            * Modified for MPlus V4
            * @since 2.5
            */
            //LPCTSTR GetFilterChars(){ return m_csFilterCharList; }
            LPCTSTR GetFilterChars(){ return m_csFilterCharList.operator LPCTSTR(); }
           /**
            * End of Modification for MPlus V4
            */

            // Destructor
            virtual ~EditCtrlEx();

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

            // Function to set icon
            void SetIcon( HICON hIcon_i );
            // Function to set colors of the mouse over window
            void SetMouseOverWndColors( COLORREF clrTopLeftOuter_i,
                                        COLORREF clrTopLeftMid_i,
                                        COLORREF clrTopLeftInner_i,
                                        COLORREF clrBottomRightOuter_i,
                                        COLORREF clrBottomRightMid_i,
                                        COLORREF clrBottomRightInner_i );

            /**
             * End of Change for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            // Enable/disable mouse hover effect.
            void EnableHoverEffect( bool bEnable_i = true );

            // Hover effect
            void SetHoverEffect( int nApplyTo_i, bool bEnable_i );
            bool GetHoverEffect( int nApplyTo_i );

            BOOL Create( DWORD dwStyle_i, const RECT& rect_i, CWnd* pParentWnd_i, UINT nID_i );
            //{{AFX_VIRTUAL(EditCtrlEx)
            protected:
            virtual void PreSubclassWindow();
            //}}AFX_VIRTUAL

        protected:

            //{{AFX_MSG(EditCtrlEx)
            afx_msg void OnNcPaint();
            afx_msg HBRUSH CtlColor( CDC* pDC_i, UINT nCtlColor_i );
            afx_msg void OnChar( UINT uChar_i, UINT uRepCnt_i, UINT uFlags_i );
            afx_msg void OnKeyDown( UINT uChar_i, UINT uRepCnt_i, UINT uFlags_i );
            afx_msg void OnPaint();
            afx_msg void OnEnable( BOOL bEnable_i );
            afx_msg void OnNcCalcSize( BOOL bCalcValidRects_i, NCCALCSIZE_PARAMS FAR* lpncsp_i );
            afx_msg void OnSize( UINT uType_i, int nWidth_i, int nHeight_i );
            afx_msg void OnShowWindow( BOOL bShow_i, UINT uStatus_i );
            afx_msg void OnSetFocus(CWnd* pOldWnd);
            afx_msg void OnKillFocus(CWnd* pNewWnd);
            afx_msg void OnMouseMove(UINT uFlags_i, CPoint point_i );
            afx_msg void OnNcMouseMove(UINT uHitTest, CPoint point_i );
           /**
            * Modified for MPlus V4
            * @since 2.5
            */
            /// Modified for correcting the error "'static_cast' :cannot convert from 'UINT (__thiscall 
            /// MPlus::UICommon::EditCtrlEx::* )(CPoint)' to 'LRESULT (__thiscall CWnd::* )(CPoint)'
            // afx_msg UINT OnNcHitTest( CPoint point_i );
            afx_msg LRESULT OnNcHitTest( CPoint point_i );
           /**
            * End of Modification for MPlus V4
            */
            afx_msg void OnMove(int x, int y);
            //}}AFX_MSG
            afx_msg void OnStyleChanged( int nStyleType_i, LPSTYLESTRUCT lpStyleStruct_io );
            afx_msg LRESULT OnSetText( WPARAM wParam_i, LPARAM lParam_i );
            afx_msg LRESULT OnPaste( WPARAM wParam_i, LPARAM lParam_i );
            afx_msg LRESULT OnInitialize( WPARAM wParam_i, LPARAM lParam_i );

            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */

            afx_msg LRESULT OnMouseLeave( WPARAM wWParam_i, LPARAM lLParam_i );
            afx_msg LRESULT OnNcMouseLeave( WPARAM wWParam_i, LPARAM lLParam_i );
            afx_msg LRESULT OnMouseOverScrollBar( WPARAM wWParam_i, LPARAM lLParam_i );
            afx_msg void OnDestroy();
            /**
            * End of Change for Celeve PIMS Phase-5 requirement
            * @since  2.0
            */
            DECLARE_MESSAGE_MAP()

            // Function to set window region.
            void SetRgn( const CRect* pClientRect_i = 0, bool bHScroll_i = false, bool bVScroll_i = false );
            // Function to set the scrollbar values.
            void SetScrollBars();

           /**
            * Modified for MPlus V4
            * @since 2.5
            */
        private:
            EditCtrlEx( const EditCtrlEx& EditCtrlEx_i );
            EditCtrlEx& operator=( const EditCtrlEx& EditCtrlEx_i );
           /**
            * End of Modification for MPlus V4
            */


        protected:

            // Colors are needed for the spin control when it is buddy
            friend class SpinEx;

            // The brush to draw the background.
            CBrush m_brBackGround;
            // The color of text
            COLORREF m_clrText;
            // The color of read-only text
            COLORREF m_clrTextReadOnly;
            // The background color
            COLORREF m_clrBackGround;
            // The read-only background color
            COLORREF m_clrBackGroundReadOnly;
            // The disabled background color
            COLORREF m_clrBackGroundDisabled;
            // Border color
            COLORREF m_clrBorder;
            // Border color when disabled
            COLORREF m_clrBorderDisabled;
            // Border color when read-only
            COLORREF m_clrBorderReadOnly;
            // The border color in normal state
            COLORREF m_clrNormalBorderColor;
            // The border color in focused state
            COLORREF m_clrFocusBorderColor;
            // Disabled text color
            COLORREF m_clrTextDisabled;    
            // List of characters that must be filtered from display
            CString m_csFilterCharList;
            // Vertical scroll bar
            ScrollBarEx m_ScrollVert;
            // Horizontal scroll bar
            ScrollBarEx m_ScrollHorz;
            // The current window style
            DWORD m_dwOldStyle;
            // Bk color of the dialog, used to fill the right bottom box formed by scrollbars.
            COLORREF m_clrDlgBk;
            // Size of the scrollbar.
            int m_nScrollbarSize;

            /* Begin: Modification 2.3 */
            // Flag to check the font is set or not.
//            bool m_bFontSet;
            bool m_bInitilised;
            /* End: Modification 2.3 */

            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */

            COLORREF m_clrMouseOverTopLeftBorder;
            COLORREF m_clrMouseOverBottomRightBorder;

            TEXT_ALIGN_e m_eTextAlign;

        private:

            // To show mouse over window
            void ShowMouseOverRect( bool bShow_i = true );

        private:

            // Holds pointer to EditOver window
            EditExOWnd* m_pEditExOWnd;
            // Holds MouseOver flag
            bool m_bMouseInside;
            // Holds NcMouseOver flag
            bool m_bNcMouseInside;
            // Holds MouseOver flag in scroll bar
            bool m_bScrollBarMouseInside;
            // Space for Icon 
            int  m_nIconSpace;
            // For aligning text
            int m_nTextOffset;
            // Holds Images
            CImageList m_ImageList;

            /**
             * End of Change for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            bool m_bHoverEffect;      // Hover effect flag for main control
            bool m_bChildHoverEffect; // Hover effect flag for child control
        };
        //{{AFX_INSERT_LOCATION}}
    } // End of namespace UICommon
} // End of namespace MPlus

#endif // #ifndef _EDITCTRLEX_H_


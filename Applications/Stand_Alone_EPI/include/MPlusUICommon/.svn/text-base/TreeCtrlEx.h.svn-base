/**
 * Copyright(c) 2006 - 2009 TOSHIBA Medical Systems Corporation, All Rights
 * Reserved.
 *
 * TreeCtrlEx.h - TreeCtrlEx class declaration file.
 *
 * @author :    Nishad S
 * @version:    1.0            Date:  2006-03-14
 * @author :    Jijeesh K V
 * @version:    2.0            Date:  2007-05-11
 * @author :    Radhakrishnan G.
 * @version:    2.1            Date:  2007-06-22
 * @author :    Shypa Padmanabhan     (Modified for MPlusV4)
 * @version:    2.2            Date:  2009-01-09
 */

#ifndef _TREECTRLEX_H_
#define _TREECTRLEX_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "MPlusUICommon/ScrollBarEx.h"
#include "MPlusUICommon/PopupText.h"

/**
 * Added for Celeve PIMS Phase-5 requirement
 * @since  2.0
 */
#include <afxcmn.h>
/**
 * End of Change for Celeve PIMS Phase-5 requirement
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
         * Copyright(c) 2006 - 2009 TOSHIBA Medical Systems Corporation, All Rights
         * Reserved.
         *
         * TreeCtrlEx - This class is to customize the standard tree control.
         *
         * @author :    Nishad S
         * @version:    1.0            Date:  2006-03-14
         * @author :    Jijeesh K V
         * @version:    2.0            Date:  2007-05-11
         */
        class CONTROLS_CLASS_API TreeCtrlEx : public CTreeCtrl
        {
        public:

            // Constructor.
            TreeCtrlEx();
            // Destructor.
            virtual ~TreeCtrlEx();

            // ClassWizard generated virtual function overrides
            //{{AFX_VIRTUAL(TreeCtrlEx)
            protected:
            virtual void PreSubclassWindow();
            //}}AFX_VIRTUAL

        protected:

            //{{AFX_MSG(TreeCtrlEx)
            afx_msg void OnPaint();
            afx_msg void OnNcPaint();
            afx_msg void OnEnable( BOOL bEnable_i );
            afx_msg void OnSetFocus( CWnd* pOldWnd_i );
            afx_msg void OnNcCalcSize( BOOL bCalcValidRects_i, NCCALCSIZE_PARAMS FAR* lpncsp_i );
            afx_msg void OnMouseMove( UINT uFlags_i, CPoint point_i );
            afx_msg BOOL OnMouseWheel( UINT uFlags_i, short sDelta_i, CPoint point_i );
            afx_msg void OnShowWindow( BOOL bShow_i, UINT uStatus_i );
            afx_msg void OnMove(int x, int y);
            afx_msg void OnSize(UINT nType, int cx, int cy);
            afx_msg void OnNcDestroy();
            //}}AFX_MSG

            DECLARE_MESSAGE_MAP()

        public:

            // Function to create the tree ctrl
            BOOL Create( DWORD dwStyle_i, const RECT& Rect_i, CWnd* pParentWnd_i, UINT uID_i );
            // Functions to set colors. If redraw flag is true, the control will be repainted.
            void SetBorderColor( COLORREF clrBorder_i, bool bRedraw_i = true );
            void SetLineColor( COLORREF clrLine_i );
            void SetBkColor( COLORREF clrBk_i );
            void SetSelBkColor( COLORREF clrBk_i, bool bRedraw_i = true );
            void SetSelTextColor( COLORREF clrText_i, bool bRedraw_i = true );
            void SetDisabledBorderColor( COLORREF clrBorder_i, bool bRedraw_i = true );
            void SetDisabledBkColor( COLORREF clrBk_i );
            void SetDisabledTextColor( COLORREF clrText_i, bool bRedraw_i = true );
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

            // Overridden function to handle Invalidate.
            // This is required to avoid the call to Invalidate when window handle is invalid.
            void Invalidate( BOOL bErase_i = TRUE )
            {
                if( FALSE != IsWindow( m_hWnd ))
                {
                    ::InvalidateRect( m_hWnd, 0, bErase_i );
                }
            }

            // Get height of (+, - ) button icon in the tree control
            int GetConfButtonHeight();
            // Get width of ( +, - ) button icon in the tree control
            int GetConfButtonWidth();
            // Enable/disable mouse hover effect.
            void EnableHoverEffect( bool bEnable_i = true );

        private:

            // Function to initialize the tree control.
            void Initialize();
            // Function to set window region.
            void SetRgn( const CRect* pClientRect_i = 0, bool bHScroll_i = false, bool bVScroll_i = false );
            // Function to set the scrollbar values.
            void SetScrollBars();
            // Custom Draw notification handler.
            afx_msg void OnCustomDraw( NMHDR* pNMHDR_i, LRESULT* pResult_o );
            // Function to get the mouse leave notification.
            afx_msg LRESULT OnMouseLeave( WPARAM wParam_i, LPARAM lParam_i );
            // Function to handle the child notifications.
            BOOL OnChildNotify( UINT message_i, WPARAM wParam_i, LPARAM lParam_i, LRESULT* pLResult_o );
            // Function to show/hide pop up text under a point.
           /**
            * Modified for MPlus V4
            * @since 2.2
            */
            //void UpdatePopupText( CPoint point_i );
            void UpdatePopupText( const CPoint& point_i );
           /**
            * End of Modification for MPlus V4
            */

        private:

            ScrollBarEx m_ScrollHorz; // Horizontal scrollbar.
            ScrollBarEx m_ScrollVert; // Vertical scrollbar.
            // Old window style. The window region is updated
            // when the new style differ from the old style.
            DWORD     m_dwOldStyle;
            // Custom colors.
            COLORREF  m_clrBorder;
            COLORREF  m_clrBk;
            COLORREF  m_clrSelBk;
            COLORREF  m_clrSelText;
            COLORREF  m_clrDisBorder;
            COLORREF  m_clrDisBk;
            COLORREF  m_clrDisText;
            COLORREF  m_clrDlgBk;
            int       m_nScrollbarSize; // Size of the scrollbar.
            PopupText m_PopupText;      // For showing pop up text.
            bool      m_bMouseInside;   // Mouse inside/outside flag.
            HTREEITEM m_hLastHitItem;   // Keeps the last item that got mouse hit.
            bool      m_bFontSet;       // Flag to check the font is set or not.
            bool      m_bHoverEffect;   // Hover effect flag
            bool      m_bInitialized;   // Flag that specifies the member are initialized
        };
    } // End of namespace UICommon
} // End of namespace MPlus

#endif // #ifndef _TREECTRLEX_H_

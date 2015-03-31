/**
 * Copyright(c) 2007-2010 TOSHIBA Medical Systems Corporation, All Rights
 * Reserved.
 *
 * RichEditCtrlEx.h - RichEditCtrlEx class declaration file
 *
 * @author :    Vinod N N
 * @version:    1.0            Date:  2007-04-20
 * @author :    Radhakrishnan G
 * @version:    2.0            Date:  2007-06-22
 * @author :    Naveen R (Modified for bug ID #717)
 * @version:    2.1            Date:  2008-12-22
 * @author :    Jinesh P.P.
 * @version:    2.2            Date:  2010-03-09
 * @author :    Jinesh P.P.  ( Modified for MPlusV4 BugFix Merging MPC06199 )
 * @version:    2.3            Date:  2010-03-24
 */

#ifndef _RICHEDITCTRLEX_H_
#define _RICHEDITCTRLEX_H_

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
#include "MPlusUICommon/ScrollBarEx.h"
#include <afxcmn.h>

namespace MPlus
{
    namespace UICommon
    {

        /**
         * Copyright(c) 2007 TOSHIBA Medical Systems Corporation, All Rights
         * Reserved.
         *
         * RichEditCtrlEx - Extended Progress Bar class, derived from CRichEditCtrl.
         *
         * Traceability Tags : TCOM_MP_CS_UI_RICHEDITCTRLEX_SET_COLORS
         *
         * @author :    Vinod N N
         * @version:    1.0            Date:  2007-04-20
         */
        class CONTROLS_CLASS_API RichEditCtrlEx : public CRichEditCtrl
        {
        // Constructor
        public:
            RichEditCtrlEx();
            // virtual Destructor
            virtual ~RichEditCtrlEx();

        // Operations
        public:

        // Overrides
            // ClassWizard generated virtual function overrides
            //{{AFX_VIRTUAL(RichEditCtrlEx)
            protected:
            virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
            virtual void PreSubclassWindow();
            //}}AFX_VIRTUAL


            // Generated message map functions
        protected:
            //{{AFX_MSG( RichEditCtrlEx )
            afx_msg void OnPaint();
            afx_msg void OnEnable( BOOL bEnable_i );
            afx_msg void OnNcCalcSize( BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp );
            afx_msg void OnNcPaint();
            afx_msg void OnSetFocus(CWnd* pOldWnd);
            afx_msg void OnKillFocus(CWnd* pNewWnd);
            afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
            afx_msg void OnSize(UINT nType, int cx, int cy);
            afx_msg void OnMove(int x, int y);
            //}}AFX_MSG
            afx_msg LRESULT OnInitialize( WPARAM wParam_i, LPARAM lParam_i );  
            DECLARE_MESSAGE_MAP()

                // Implementation
        public:
            void SetTextColor( COLORREF clrText_i );
            void SetTextColorDisabled( COLORREF clrColor_i, bool bInvalidate_i = true );
            void SetBorderColor( COLORREF clrBorder_i );
            /* Begin: Modification 2.1 #717 - New function for updating background color*/
            void SetBkColor( COLORREF clrBkColor_i );
            /* End: Modification 2.1 #717*/
            // Setting disabled background color
            void SetDisabledBkColor( COLORREF clrBk_i );
            void SetBorderColorDisabled( COLORREF clrBorder_i );
            bool SetFont( LPCTSTR lpszFontName_i, int nPointSize_i, bool bBold_i = false );
            COLORREF GetTextColor(){return m_clrText;};
            COLORREF GetBorderColor(){return m_clrBorderNormal;};
            COLORREF GetBorderColorDisabled(){return m_clrBorderDisabled;};
            void EnableHoverEffect( bool bEnable_i = true );
            /* Begin: Added 2.2 for Bug ID MPC06127 */
            // Overridden function to handle Invalidate.
            // This is required to avoid the call to Invalidate when window handle is invalid.
            void Invalidate( BOOL bErase_i = TRUE )
            {
                if( IsWindow( m_hWnd ))
                {
                    ::InvalidateRect( m_hWnd, 0, bErase_i );
                }
            }
            /* End: Added 2.2 for Bug ID MPC06127 */
            
            /* Begin: Added 2.3 for Bug ID #MPC06199*/
            // Function for updating focussed border color
            void SetFocusBorderColor( COLORREF clrFocussedBorder_i );
            // Function for getting focussed border color.
            COLORREF GetFocusBorderColor(){ return m_clrBorderFocussed; };
            /* End: Added 2.3 for Bug ID #MPC06199*/

        protected:
            void SetScrollBars();
            void SetRgn( const CRect* pClientRect_i = 0, bool bHScroll_i = false, bool bVScroll_i = false );

        private:
            ScrollBarEx m_ScrollHorz;
            ScrollBarEx m_ScrollVert;
            DWORD       m_dwOldStyle;
            int         m_nScrollbarSize;
            COLORREF    m_clrBorderNormal;
            COLORREF    m_clrBorderDisabled;
            COLORREF    m_clrBorderFocussed;
            COLORREF    m_clrText;
            COLORREF    m_clrBackground;
            COLORREF    m_clrDlgBackground;
            COLORREF    m_clrBkDisabled;
            // Disabled text color
            COLORREF m_clrTextDisabled;    
            int m_nFontHeight; // Font size
            CString m_csFontName; // Font name
            bool m_bFontSet;
            bool m_bBold; // Bold or not
            bool m_bHoverEffect; // Hover effect flag

        };
    } // End of namespace UICommon
} // End of namespace MPlus

#endif // _RICHEDITCTRLEX_H

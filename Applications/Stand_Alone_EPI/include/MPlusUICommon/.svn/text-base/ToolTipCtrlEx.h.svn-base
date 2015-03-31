/**
 * Copyright(c) 2007 - 2010 TOSHIBA Medical Systems Corporation, All Rights
 * Reserved.
 *
 * ToolTipCtrlEx.h - ToolTipCtrlEx class declaration file
 *
 * @author :    Vinod N N
 * @version:    1.0            Date:  2007-04-25
 * @author :    Junaij M
 * @version:    1.1            Date:  2008-12-29
 * @author :    Rahul Kumar    ( Modified for MPlusV4 BugFix Merging MPC06200 )
 * @version:    1.2            Date:  2010-03-29
 */

#ifndef _TOOLTIPCTRLEX_H_
#define _TOOLTIPCTRLEX_H_

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
#include <afxcmn.h>

namespace MPlus
{
    namespace UICommon
    {

        /**
         * Copyright(c) 2007 TOSHIBA Medical Systems Corporation, All Rights
         * Reserved.
         *
         * ToolTipCtrlEx - Extended Tool Tip class, derived from CToolTipCtrl.
         *
         * Traceability Tags : TCOM_MP_CS_UI_TOOLTIPCTRLEX_SET_COLORS
         *                     TCOM_MP_CS_UI_TOOLTIPEX_SET_FONT
         *                     TCOM_MP_CS_UI_TOOLTIPEX_DRAW_TEXT
         *
         * @author :    Vinod N N
         * @version:    1.0            Date:  2007-04-25
         */
        class CONTROLS_CLASS_API ToolTipCtrlEx : public CToolTipCtrl
        {
        // Constructor
        public:
            ToolTipCtrlEx();
            // virtual Destructor
            virtual ~ToolTipCtrlEx();

        // Operations
        public:

        // Overrides
            // ClassWizard generated virtual function overrides
            //{{AFX_VIRTUAL(ToolTipCtrlEx)
            protected:
            virtual void PreSubclassWindow();
            virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
            //}}AFX_VIRTUAL

        // Implementation
        public:
            void SetBorderColor( COLORREF clrBorder_i );
            // Function to set the font.
            bool SetFont( LPCTSTR lpctszFontName_i, int nPointSize_i, bool bBold_i = false );
            COLORREF GetBorderColor(){return m_clrBorder;};

            // Generated message map functions
        protected:
            //{{AFX_MSG( ToolTipCtrlEx )
            afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
            afx_msg void OnPaint();
            afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
            //}}AFX_MSG
            afx_msg void OnTTNShow( NMHDR * pNotifyStruct, LRESULT* result );
            DECLARE_MESSAGE_MAP()

        protected:
            void Initialize();
            void UpdateRGBWithAlpha( CBitmap &bmp_i, LPBYTE pBitmapBits_i );
            void UpdateAlpha( int nWidth_i, int nHeight_i, LPBYTE pvBits_i );
            void DrawText( CDC& dc_i, CRect& rect_io, bool bCalcRect_i = false );

        private:
            /* Begin: Modification 1.1 #684 - Added for fixing margin issue. */
            int GetMarginWidth();
            /* End: Modification 1.1 #684*/

        protected:
            /* Begin: Modification 1.2 for Bug ID #MPC06200 */
            // variable not used
            // CSize m_size;
            /* End: Modification 1.2 for Bug ID #MPC06200 */
            // Object to initialize the gdi plus
            GdiPlusCtrl m_GdiInit;
            // Border color
            COLORREF m_clrBorder;
            /* Begin: Modification 1.2 for Bug ID #MPC06200 */
            // Hold status of cureent window( layered window or normal window )
            bool m_bLayeredWindow;
            /* End: Modification 1.2 for Bug ID #MPC06200 */
        };
    } // End of namespace UICommon
} // End of namespace MPlus
#endif // _TOOLTIPCTRLEX_H

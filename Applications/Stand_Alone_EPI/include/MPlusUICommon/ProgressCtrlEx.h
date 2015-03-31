/**
 * Copyright(c) 2007-2010 TOSHIBA Medical Systems Corporation, All Rights
 * Reserved.
 *
 * ProgressCtrlEx.h - ProgressCtrlEx class declaration file
 *
 * @author :    Vinod N N
 * @version:    1.0            Date:  2007-04-18
 * @author :    Jinesh P.P.
 * @version:    1.1            Date:  2010-03-09
 */

#ifndef _PROGRESSCTRLEX_H_
#define _PROGRESSCTRLEX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxdtctl.h>
#include <afxcmn.h>

#ifndef _UNICODE
#error Unicode definition is necessary to use this class
#endif

#ifdef UI_CONTROLS_DLL
#define CONTROLS_CLASS_API __declspec( dllexport )
#else
#define CONTROLS_CLASS_API __declspec( dllimport )
#endif

#include "MPlusUICommon/GdiPlusCtrl.h"

namespace MPlus
{
    namespace UICommon
    {

        /**
         * Copyright(c) 2007 TOSHIBA Medical Systems Corporation, All Rights
         * Reserved.
         *
         * Traceability Tags : TCOM_MP_CS_UI_PROGRESSCTRLEX_DRAW_PROGRESSBAR
         *                     TCOM_MP_CS_UI_PROGRESSCTRLEX_SET_COLORS
         *
         * ProgressCtrlEx - Extended Progress Bar class, derived from CProgressCtrl.
         *
         * @author :    Vinod N N
         * @version:    1.0            Date:  2007-04-18
         */
        class CONTROLS_CLASS_API ProgressCtrlEx : public CProgressCtrl
        {
        // Constructor
        public:
            ProgressCtrlEx();
            // virtual Destructor
            virtual ~ProgressCtrlEx();

        // Operations
        public:

        // Overrides
            // ClassWizard generated virtual function overrides
            //{{AFX_VIRTUAL(ProgressCtrlEx)
            protected:
            virtual void PreSubclassWindow();
            virtual LRESULT WindowProc( UINT uMsg_i, WPARAM wParam_i, LPARAM lParam_i );
            //}}AFX_VIRTUAL

        // Implementation
        public:
            void DrawProgressBar( CDC *pDC_i, const RECT &rectClient_i );
            void SetBorderColor( COLORREF clrBorder_i );
            void SetBkColor( COLORREF clrBk_i );
            void SetProgressColor( COLORREF clrCenter_i, COLORREF clrOuter_i );
            void SetDisabledBorderColor( COLORREF clrBorder_i );
            void SetDisabledBkColor( COLORREF clrBk_i );
            void SetDisabledProgressColor( COLORREF clrProgress_i );

            COLORREF GetDisabledProgressColor(){return m_clrDisabledProgress;};
            COLORREF GetBorderColor(){return m_clrNormalBorderColor;};
            COLORREF GetBkColor(){return m_clrBk;};
            void GetProgressColor( COLORREF& clrCenter_o, COLORREF& clrOuter_o );
            COLORREF GetDisabledBorderColor(){return m_clrDisabledBorder;};
            COLORREF GetDisabledBkColor(){return m_clrDisabledBk;};
            /* Begin: Added 1.1 for Bug ID MPC06127 */
            // Overridden function to handle Invalidate.
            // This is required to avoid the call to Invalidate when window handle is invalid.
            void Invalidate( BOOL bErase_i = TRUE )
            {
                if( IsWindow( m_hWnd ))
                {
                    ::InvalidateRect( m_hWnd, 0, bErase_i );
                }
            }
            /* End: Added 1.1 for Bug ID MPC06127 */

            // Generated message map functions
        protected:
            //{{AFX_MSG( ProgressCtrlEx )
            afx_msg void OnPaint();
            afx_msg void OnNcPaint();
            afx_msg void OnEnable( BOOL bEnable_i );
            //}}AFX_MSG

            DECLARE_MESSAGE_MAP()
        protected:
            void Resize();
            int m_nProgressRight;
            int m_nProgressLeft;
            int m_nProgressBottom;
            int m_nProgressTop;

            int m_nCurrentPosition;
            // Object to initialize the gdi plus
            GdiPlusCtrl m_GdiInit;

            // Normal border color
            COLORREF m_clrNormalBorderColor;
            // Disabled border color
            COLORREF m_clrDisabledBorder;
            //Disabled background color
            COLORREF m_clrDisabledBk;
            //Normal background color
            COLORREF m_clrBk;
            //Progress center color
            COLORREF m_clrProgressCenter;
            //Progress outer color
            COLORREF m_clrProgressOuter;
            //Disabled progress color
            COLORREF m_clrDisabledProgress;
            bool m_bDisabled;
        };
    } // End of namespace UICommon
} // End of namespace MPlus

#endif // _PROGRESSCTRLEX_H

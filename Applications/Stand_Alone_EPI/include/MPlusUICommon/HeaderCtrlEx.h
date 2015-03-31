/**
 * Copyright(c) 2006 - 2007 TOSHIBA Medical Systems Corporation, All Rights
 * Reserved.
 *
 * HeaderCtrlEx.h - Include file for the HeaderCtrlEx
 *
 * @author :    Naveen.R
 * @version:    1.0            Date:  2006-03-06
 * @author :    Jijeesh K V
 * @version:    2.0            Date:  2007-04-24
 * @author :    Jijeesh K V
 * @version:    2.1            Date:  2008-05-09
 */

#ifndef _HEADERCTRLEX_H_
#define _HEADERCTRLEX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MPlusUICommon/GdiPlusCtrl.h"
#include <afxtempl.h>

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
        struct MIN_MAX_t
        {
            int nMax;
            int nMin;
        };
        /* Begin: Added 2.1 */
        static const int EHCIM_DRAGBEGIN     = WM_USER + 16;
        static const int EHCIM_DRAGEND     = WM_USER + 17;
        /* End: Added 2.1 */
        /**
         * Copyright(c) 2006 - 2007 TOSHIBA Medical Systems Corporation, All Rights
         * Reserved.
         *
         * HeaderCtrlEx - This class customizes the header control
         *
         * Traceability Tags : TCOM_MP_CS_UI_HEADERCTRLEX_SET_COLORS
         *
         * @author :    Naveen.R
         * @version:    1.0            Date:  2006-03-06
         * @author :    Jijeesh K V
         * @version:    2.0            Date:  2007-04-24
         * @author :    Jijeesh K V
         * @version:    2.1            Date:  2008-05-09
         */
        class CONTROLS_CLASS_API HeaderCtrlEx : public CHeaderCtrl
        {

        public:

            // Constructor
            HeaderCtrlEx();

            // Function to set the text color
            void SetTextColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the text color
            void SetBkColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the text color
            void SetBorderColor( COLORREF clrTopLeft_i, COLORREF clrBottomRight_i, COLORREF clrInnerLine_i,
                bool bInvalidate_i = true );

            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            // Function to set grid line color
            void SetGridLineColor( COLORREF clrGrid_i );
            /**
             * End of Change for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */

            // Function set the min and max length of the column
            void SetColumnRange( int nItem_i, int nMin_i, int nMax_i );

            // Overridden function to handle Invalidate.
            // This is required to avoid the call to Invalidate when window handle is invalid.
            void Invalidate( BOOL bErase_i = TRUE )
            {
                if( FALSE != IsWindow( m_hWnd ))
                {
                    ::InvalidateRect( m_hWnd, 0, bErase_i );
                }
            }

            //{{AFX_VIRTUAL(HeaderCtrlEx)
            //}}AFX_VIRTUAL

            // Destructor
            virtual ~HeaderCtrlEx();

        protected:

            //{{AFX_MSG(HeaderCtrlEx)
            afx_msg BOOL OnEraseBkgnd( CDC* pDC_i );
            afx_msg void OnPaint();
            afx_msg void OnMouseMove(UINT nFlags, CPoint point);
            //}}AFX_MSG

            DECLARE_MESSAGE_MAP()
                // Function that draws the header control
                afx_msg void OnCustomDraw( NMHDR* pNMHDR_i, LRESULT* pResult_o );
            afx_msg void OnDragBegin( NMHDR* pNMHDR_i, LRESULT* pResult_o );
            afx_msg void OnDragEnd( NMHDR* pNMHDR_i, LRESULT* pResult_o );
            void PreSubclassWindow();
            BOOL OnChildNotify( UINT uMessage_i, WPARAM wParam_i , LPARAM lParam_i , LRESULT* pResult_i );
            // Draw a item in the control
            void DrawHeader( LPDRAWITEMSTRUCT pstDrawItem_i );

        protected:

            // Background color
            COLORREF m_clrBk;
            // Text color
            COLORREF m_clrText;
            // Top left color
            COLORREF m_clrTopLeft;
            // Bottom right color
            COLORREF m_clrBottomRight;
            // Inside line color
            COLORREF m_clrInsideLine;

            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            // Inner grid color
            COLORREF m_clrInnerGrid;
            /**
             * End of Change for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            // The sort column
            int m_nSortColumn;
            // The sort order
            bool m_bSortAscending;
            // Map that keeps the min and max length of each columns
            CMap< int,int, MIN_MAX_t, MIN_MAX_t> m_RangeMap;
            // The column which the user is currently dragging
            int m_nDragingColumn;

        private:

            // Object to initialize the gdi plus
            GdiPlusCtrl m_GdiInit;

            friend class ListCtrlEx;

        };

        //{{AFX_INSERT_LOCATION}}
    } // End of namespace UICommon
} // End of namespace MPlus
#endif // #ifndef _HEADERCTRLEX_H_

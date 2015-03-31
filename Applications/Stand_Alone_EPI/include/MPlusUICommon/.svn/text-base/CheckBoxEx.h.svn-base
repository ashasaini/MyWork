/**
 * Copyright(c) 2006 - 2007 TOSHIBA Medical Systems Corporation, All Rights
 * Reserved.
 *
 * CheckBoxEx.h - Include file for CheckBoxEx
 *
 * @author :    Naveen.R
 * @version:    1.0            Date:  2006-02-21
 * @author :    Vinod N N
 * @version:    2.0            Date:  2007-04-23
 * @author :    Nishad S
 * @version:    2.1            Date:  2007-06-21
 * @author :    Asha (Modified for MR PIMS Performance improvement to avoid flickering.)
 * @version:    2.2            Date:  2009-10-23
 */


#ifndef _CHECKBOXEX_H_
#define _CHECKBOXEX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MPlusUICommon/CheckBoxDraw.h"

namespace MPlus
{
    namespace UICommon
    {
        /**
         * Copyright(c) 2006 - 2007 TOSHIBA Medical Systems Corporation, All Rights
         * Reserved.
         *
         * CheckBoxEx - This class customizes the check box control
         *
         * Traceability Tags : TCOM_MP_CS_UI_CHECKBOXEX_SET_COLORS
         *                     TCOM_MP_CS_UI_CHECKBOXEX_TRACK_MOUSE_MOVEMENT
         *
         * @author :    Naveen.R
         * @version:    1.0            Date:  2006-02-21
         * @author :    Vinod N N
         * @version:    2.0            Date:  2007-04-23
         */
        class CONTROLS_CLASS_API CheckBoxEx : public CButton, public CheckBoxDraw
        {

        public:

            // Constructor
            CheckBoxEx();

            // Function to set the font.
            /**
             * Tookout for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            // bool SetFont( LPCTSTR lpctszFontName_i, int nPointSize_i );
            /**
             * End of Change for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */

            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            bool SetFont( LPCTSTR lpctszFontName_i, int nPointSize_i, bool m_bBold_i = false );
            /**
             * End of Change for Celeve PIMS Phase-5 requirement
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

            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            // Sets the border color of the check box when the mouse is over the control.
            void SetMouseOverBorderColor( COLORREF clrBorder_i );

            // Sets the shadow color of the check box.
            void SetShadowColor( COLORREF clrShadow_i );
            /**
             * End of Change for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */

            // Enable/disable mouse hover effect.
            void EnableHoverEffect( bool bEnable_i = true )
            {
                m_bHoverEffect = bEnable_i;
            }

            // Destructor
            virtual ~CheckBoxEx();

            //{{AFX_VIRTUAL(CheckBoxEx)
            public:
            virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
            protected:
            virtual void PreSubclassWindow();
            //}}AFX_VIRTUAL
            BOOL OnChildNotify( UINT uMessage_i, WPARAM wParam_i, LPARAM lParam_i, LRESULT* pResult_o );

        protected:

            //{{AFX_MSG(CheckBoxEx)
            afx_msg void OnLButtonDblClk( UINT uFlags_i, CPoint point_i );
            afx_msg void OnMouseMove( UINT nFlags_i, CPoint Point_i );
            afx_msg BOOL OnEraseBkgnd( CDC* pDC_i );
            //}}AFX_MSG
            afx_msg LRESULT AddCheck( WPARAM wParam_i, LPARAM lParam_i );
            afx_msg LRESULT ReturnCheck( WPARAM wParam_i, LPARAM lParam_i );
            afx_msg LRESULT OnInitialize( WPARAM wParam_i, LPARAM lParam_i );
            afx_msg LRESULT OnMouseLeave( WPARAM nWParam_i, LPARAM lLParam_i );
            DECLARE_MESSAGE_MAP()

        protected:

            // Check state
            bool m_bChecked;

        private:

            // Flag to check the font is set or not.
            bool m_bFontSet;
            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            // Holds mouse hover flag
            bool m_bMouseInside;
            /**
             * End of Change for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            bool m_bHoverEffect;
            /**
             * Modified for MR PIMS Performance improvement.
             * To avoid flickering of check box while mouse move
             * @since 2.2
             */
            bool m_bIsMinimalDraw;
            /**
             * End of Change for MR PIMS Performance improvement.
             * @since 2.2
             */
        };
        //{{AFX_INSERT_LOCATION}}
    } // End of namespace UICommon
} // End of namespace MPlus

#endif // #ifndef _CHECKBOXEX_H_

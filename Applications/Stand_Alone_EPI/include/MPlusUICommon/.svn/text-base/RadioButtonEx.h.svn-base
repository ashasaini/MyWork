/**
 * Copyright(c) 2006 - 2007 TOSHIBA Medical Systems Corporation, All Rights
 * Reserved.
 *
 * RadioButtonEx.h - Include file for the control RadioButtonEx
 *
 * @author :    Naveen.R
 * @version:    1.0            Date:  2006-02-22
 * @author :    Vinod N N
 * @version:    2.0            Date:  2007-04-20
 * @author :    Nishad S
 * @version:    2.1            Date:  2007-06-21
 * @author :    Jijeesh K v
 * @version:    2.2            Date:  2008-04-24
 * @author :    Asha (Modified for MR PIMS Performance improvement to avoid flickering.)
 * @version:    2.3            Date:  2009-10-23
 */

#ifndef _RADIOBUTTONEX_H_
#define _RADIOBUTTONEX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MPlusUICommon/RadioDraw.h"
#include <afxtempl.h>

namespace MPlus
{
    namespace UICommon
    {
        /**
         * Copyright(c) 2006 - 2007 TOSHIBA Medical Systems Corporation, All Rights
         * Reserved.
         *
         * RadioButtonEx - This class is use to customise the appearance of radio buttons
         *
         * Traceability Tags : TCOM_MP_CS_UI_RADIOBUTTONEX_SET_COLORS
         *                     TCOM_MP_CS_UI_RADIOBUTTONEX_TRACK_MOUSE_MOVEMENT
         *
         * @author :    Naveen.R
         * @version:    1.0            Date:  2006-02-22
         * @author :    Vinod N N
         * @version:    2.0            Date:  2007-04-20
         * @author :    Nishad S
         * @version:    2.1            Date:  2007-06-21
         * @author :    Jijeesh K v
         * @version:    2.2            Date:  2008-04-24
         */
        class CONTROLS_CLASS_API RadioButtonEx : public CButton, public RadioDraw
        {

        public:

            // Constructor
            RadioButtonEx();

            // Overridden function to handle Invalidate.
            // This is required to avoid the call to Invalidate when window handle is invalid.
            void Invalidate( BOOL bErase_i = TRUE )
            {
                if( FALSE != IsWindow( m_hWnd ))
                {
                    ::InvalidateRect( m_hWnd, 0, bErase_i );
                }
            }

            // Function to set the  back ground color of radio button
            void SetBkColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the  Fill color of radio button
            void SetCheckBkColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the text colorO
            void SetTextColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the disabled text color
            void SetTextColorDisabled( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the text color when un checked
            void SetTextColorUnChecked( COLORREF clrColor_i, bool bInvalidate_i = true );

             // Function to set the border of Check box
            void SetBorderColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the Check mark color
            void SetCheckColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set shadow color
            void SetShadowColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the check mark color when disabled
            void SetCheckColorDisabled( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the focus rectangle color
            void SetFocusRectColor( COLORREF clrColor_i, bool bInvalidate_i = true );

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

            // Sets the width of the radio button
            void SetWidth( int nWidth_i );

            BOOL OnChildNotify( UINT uMessage_i, WPARAM wParam_i, LPARAM lParam_i, LRESULT* pResult_o );

            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            // Sets the border color of the check box when the mouse is over the control.
            void SetMouseOverBorderColor( COLORREF clrBorder_i );
            /**
             * End of Change for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            // Enable/disable mouse hover effect.
            void EnableHoverEffect( bool bEnable_i = true );

            // Destructor
            virtual ~RadioButtonEx();

            //{{AFX_VIRTUAL(RadioButtonEx)
            public:
            virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct_i );            
            protected:
            virtual void PreSubclassWindow();
            //}}AFX_VIRTUAL

        protected:


            //{{AFX_MSG(RadioButtonEx)
            afx_msg UINT OnGetDlgCode();
            afx_msg void OnKeyDown( UINT uChar_i, UINT uRepCnt_i, UINT uFlags_i );
            afx_msg void OnNcDestroy();
            afx_msg void OnMouseMove( UINT nFlags_i, CPoint Point_i );
            afx_msg BOOL OnEraseBkgnd( CDC* pDC_i );
            //}}AFX_MSG
            afx_msg LRESULT AddCheck( WPARAM wParam_i, LPARAM lParam_i );
            afx_msg LRESULT ReturnCheck( WPARAM wParam_i, LPARAM lParam_i );
            afx_msg LRESULT OnInitialize( WPARAM wParam_i, LPARAM lParam_i );
            afx_msg LRESULT OnMouseLeave( WPARAM nWParam_i, LPARAM lLParam_i );
            DECLARE_MESSAGE_MAP()

            // Function that send uncheck message to all other radio buttons in the group
            void SendNotification();

        protected:

            // Array that keep a pointer to all the radio buttons in the group
            CArray<RadioButtonEx*,RadioButtonEx*>* m_parrRadioGroup;
            /* Begin: Added 2.2 */
            // Array that keeps the group members in case of dynamic group creation
            static CArray<RadioButtonEx*,RadioButtonEx*>* m_parrDynRadioGroup;
            static int m_nGrupCount; // Holds number of group items
            bool m_bStaticControl; // Holds the flag whether created dynamically or not
            /* End: Added 2.2 */
            // Keeps the check status of the control
            bool m_bChecked;

        private:

            // Flag to check the font is set or not.
            bool m_bFontSet;
            // Flag to check the WS_TABSTOP property set or not.
            bool m_bTabStop;
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
            bool m_bHoverEffect; // Hover effect flag
            /**
             * Modified for MR PIMS Performance Improvement
             * To avoid flickering of radio button while mouse move
             * @since 2.3
             */
            bool m_bMouseHoverSatus;
            /**
             * End of change for MR PIMS Performance Improvement
             * To avoid flickering of radio button while mouse move
             * @since 2.3
             */
        };

        //{{AFX_INSERT_LOCATION}}
    } // End of namespace UICommon
} // End of namespace MPlus

#endif // #ifndef _RADIOBUTTONEX_H_


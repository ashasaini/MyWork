/**
 * Copyright(c) 2006 - 2009 TOSHIBA Medical Systems Corporation, All Rights
 * Reserved.
 *
 * CheckBoxDraw.h - Include file for CheckBoxDraw
 *
 * @author :    Naveen.R
 * @version:    1.0            Date:  2006-03-01
 * @author :    Vinod N N
 * @version:    2.0            Date:  2007-04-23
 * @author :    Jayasankar.s
 * @version:    2.1            Date:  2008-05-02
 * @author :    Shypa Padmanabhan     (Modified for MPlusV4)
 * @version:    2.2            Date:  2009-01-09
 * @author :    Asha (Modified for MR PIMS Performance improvement to avoid flickering.)
 * @version:    2.3            Date:  2009-10-23
 */


#ifndef _CHECKBOXDRAW_H_
#define _CHECKBOXDRAW_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "MPlusUICommon/GdiPlusCtrl.h"

/**
 * Added for Celeve PIMS Phase-5 requirement
 * @since  2.0
 */
#include <afxdtctl.h>
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
        /**
         * Copyright(c) 2006 - 2009 TOSHIBA Medical Systems Corporation, All Rights
         * Reserved.
         *
         * CheckBoxDraw - This class is responsible for all the drawing function ins the check box
         *
         * Traceability Tags : TCOM_MP_CS_UI_CHECKBOXDRAW_SET_COLORS
         *
         * @author :    Naveen.R
         * @version:    1.0            Date:  2006-03-01
         * @author :    Vinod N N
         * @version:    2.0            Date:  2007-04-23
         * @author :    Jayasankar.s
         * @version:    2.1            Date:  2008-05-02
         */
        class CONTROLS_CLASS_API CheckBoxDraw
        {

        public:

            // Constructor
            CheckBoxDraw();

            // Function Initialize members.
            void Initialize();

            // Function to set the  back ground color of check box control
            void SetBkColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the  Fill color of check box
            void SetCheckBkColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the text color
            void SetTextColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the disabled text color
            void SetTextColorDisabled( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the text color when un checked
            void SetTextColorUnChecked( COLORREF clrColor_i, bool bInvalidate_i = true );

             // Function to set the border of Check box
            void SetBorderColor( COLORREF clrLeftColor_i, COLORREF clrRightColor_i,
                                 bool bInvalidate_i = true );

            // Function to set the Check mark color
            void SetCheckColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the check mark color when disabled
            void SetCheckColorDisabled( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the focus rect color
            void SetFocusRectColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            void SetMouseOverBorderColor( COLORREF clrColor_i );
            void SetShadowColor( COLORREF clrShadow_i );
            /**
             * End of Change  for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */

            // Sets the width of the check box
            bool SetWidth( int nWidth_i );

            // Return the width of the check box
            int GetWidth()
            {
                return m_nWidth;
            }

            // Abstract function
           /**
            * Modified for MPlus V4
            * @since 2.2
            */
            //virtual void Invalidate( BOOL bErase = TRUE ){ bErase = FALSE; }
            virtual void Invalidate( BOOL bErase = TRUE );
           /**
            * End of Modification for MPlus V4
            */

            /**
             * Modified for MR PIMS Performance improvement.
             * To avoid flickering of check box while mouse move
             * @since 2.3
             */
            // Draw the check box
            //void DrawCheckBox( LPDRAWITEMSTRUCT &lpDrawItemStruct_i, 
            //                   LPCTSTR lpctszText_i, bool bChecked_i,
            //                   bool bMouseOver_i = false // Added for Celeve PIMS Phase-5 requirement
            //                   );
            void DrawCheckBox( LPDRAWITEMSTRUCT &lpDrawItemStruct_i, 
                               LPCTSTR lpctszText_i, bool bChecked_i,
                               bool bMouseOver_i = false, // Added for Celeve PIMS Phase-5 requirement
                               bool bIsMinimalDraw_i = false // Flag for minimal drawing
                               );
            /**
             * End of Change for MR PIMS Performance improvement.
             * @since 2.3
             */

            // Draws the border of check box
            void DrawBox( HDC hDc_i, CRect& BorderRect_i,
                          bool bMouseOver_i = false, // Added for Celeve PIMS Phase-5 requirement
                          bool bDisable_i = false
                          );

            // Draws the check in the check box
            void DrawCheckMark( HDC hDc_i, CRect& BorderRect_i ,COLORREF clrCheckColor_i );

            // Destructor
            virtual ~CheckBoxDraw();

        protected:

            // The border color of left corner
            COLORREF m_clrBorderLeft;
            // The border color of right corner
            COLORREF m_clrBorderRight;
            // The disabled border color of left corner
            COLORREF m_clrBorderLeftDisable;
            // The disabled border color of right corner
            COLORREF m_clrBorderRightDisable;
            // The check mark color
            COLORREF m_clrCheck;
            // Check mark color when disabled
            COLORREF m_clrCheckDisabled;
            // Back color of check box
            COLORREF m_clrCheckBk;
            // Back color of check box when disabled
            COLORREF m_clrCheckBkDisabled;
            // Back color of check box control
            COLORREF m_clrBk;
            // Text color
            COLORREF m_clrText;
            // Text color when disabled
            COLORREF m_clrTextDisabled;
            // Text color when unchecked
            COLORREF m_clrTextUnChecked;
            // The focus rect color
            COLORREF m_clrFocusRect;
            // Width of check box
            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            // Border color on mouse over
            COLORREF m_clrMouseOverBorder;
            // Shadow color
            COLORREF m_clrShadow;
            /**
             * End of Change for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            int m_nWidth;
            // Object to initialize the gdi plus
            GdiPlusCtrl m_GdiInit;
        };
    } // End of namespace UICommon
} // End of namespace MPlus

#endif // #ifndef _CHECKBOXDRAW_H_

/**
 * Copyright(c) 2006 - 2009 TOSHIBA Medical Systems Corporation, All Rights
 * Reserved.
 *
 * RadioDraw.h - Include file for RadioDraw
 *
 * @author :    Naveen.R
 * @version:    1.0            Date:  2006-03-01
 * @author :    Vinod N N
 * @version:    2.0            Date:  2007-04-23
 * @author :    Shypa Padmanabhan     (Modified for MPlusV4)
 * @version:    2.1            Date:  2009-01-09
 * @author :    Asha (Modified for MR PIMS Performance improvement to avoid flickering.)
 * @version:    2.2            Date:  2009-10-23
 */

#ifndef _RADIODRAW_H_
#define _RADIODRAW_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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
         * RadioDraw - This class is responsible for the drawing of the radio button
         *
         * Traceability Tags : TCOM_MP_CS_UI_RADIODRAW_SET_COLORS
         *
         * @author :    Naveen.R
         * @version:    1.0            Date:  2006-03-01
         * @author :    Vinod N N
         * @version:    2.0            Date:  2007-04-23
         */
        class CONTROLS_CLASS_API RadioDraw
        {

        public:

            // Constructor
            RadioDraw();

            // Function Initialize members.
            void Initialize();

            // Function to set the  back ground color of the radio button
            void SetBkColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the  Fill color of radio button
            void SetCheckBkColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the text color
            void SetTextColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the shadow color
            void SetShadowColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the disabled text color
            void SetTextColorDisabled( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the text color when un checked
            void SetTextColorUnChecked( COLORREF clrColor_i, bool bInvalidate_i = true );

             // Function to set the border of Check box
            void SetBorderColor( COLORREF clrColor_i, bool bInvalidate_i = true );

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
            // Function to set the mouse over border color
            void SetMouseOverBorderColor( COLORREF clrBorder_i );
            /**
             * End of Change for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */

            // Sets the width of the radio button
            void SetWidth( int nWidth_i );

            // Returns the width of the radio button
            int GetWidth();
            
            /**
             * Modified for MR PIMS Performance Improvement
             * To avoid flickering of radio button while mouse move
             * @since 2.2
             */
            // Function to draw the radio button
            //void DrawRadio( LPDRAWITEMSTRUCT lpDrawItemStruct_i,CString* pcsText_i = 0 ,
                            //bool bChecked  = false,
                            //bool bMouseOver_i = false, // Added for Celeve PIMS Phase-5 requirement
                            //);
            void DrawRadio( LPDRAWITEMSTRUCT lpDrawItemStruct_i,CString* pcsText_i = 0 ,
                            bool bChecked  = false,
                            bool bMouseOver_i = false, // Added for Celeve PIMS Phase-5 requirement
                            bool bMinimalDraw_i = false );
            /**
             * End of change for MR PIMS Performance Improvement
             * To avoid flickering of radio button while mouse move
             * @since 2.2
             */

            // Abstract function
           /**
            * Modified for MPlus V4
            * @since 2.1
            */
            //virtual void Invalidate( BOOL bErase = TRUE ){ bErase = FALSE; }
            virtual void Invalidate( BOOL bErase = TRUE );
           /**
            * End of Modification for MPlus V4
            */

            // Destructor
            virtual ~RadioDraw();

        protected:

             // The check color
            COLORREF m_clrCheck;
            // Check color when disabled
            COLORREF m_clrCheckDisabled;
            // Back color of radio button
            COLORREF m_clrCheckBk;
            // Back color of radio button control
            COLORREF m_clrBk;
            // Border color
            COLORREF m_clrBorder;
            // Disabled Border color
            COLORREF m_clrBorderDisabled;
            // Disabled Back color
            COLORREF m_clrCheckBkDisabled;
            // Text color
            COLORREF m_clrText;
            // Text color when disabled
            COLORREF m_clrTextDisabled;
            // Text color when unchecked
            COLORREF m_clrTextUnChecked;
            // The focus rect color
            COLORREF m_clrFocusRect;
            // Holds the shadow color
            COLORREF m_clrShadow;
            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            // Border color on mouse over
            COLORREF m_clrMouseOverBorder;
            /**
             * End of Change for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            // The radius of radio button
            int m_nWidth;

        private:

            // Object to initialize the gdi plus
            GdiPlusCtrl m_GdiInit;
        };
    } // End of namespace UICommon
} // End of namespace MPlus

#endif // #ifdef _RADIODRAW_H_

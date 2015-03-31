/**
 * Copyright(c) 2006 - 2007 TOSHIBA Medical Systems Corporation, All Rights
 * Reserved.
 *
 * FrameEx.h - Include file for the FrameEx control
 *
 * @author :    Naveen.R
 * @version:    1.0            Date:  2006-03-07
 * @author :    Jijeesh K V
 * @version:    2.0            Date:  2007-04-24
 */

#ifndef _FRAMEEX_H_
#define _FRAMEEX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MPlusUICommon/GdiPlusCtrl.h"

/**
 * Added for Celeve PIMS Phase-5 requirement
 * @since  2.0
 */
#include <afxdtctl.h>
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
        // Represents different sorts of frame curves.
        enum CURVE_SHAPE_e
        {
            CURVE_NONE = 0,
            CURVE_VERY_SMALL,
            CURVE_SMALL,
            CURVE_MEDIUM,
            CURVE_LARGE,
            CURVE_EXTRA_LARGE
        };  
        /**
         * Copyright(c) 2006 - 2007 TOSHIBA Medical Systems Corporation, All Rights
         * Reserved.
         *
         * FrameEx - This class customizes the frame control
         *
         * Traceability Tags : TCOM_MP_CS_UI_FRAMEEX_SET_THICKNESS
         *
         * @author :    Naveen.R
         * @version:    1.0            Date:  2006-03-07
         * @author :    Jijeesh K V
         * @version:    2.0            Date:  2007-04-24
         */
        class CONTROLS_CLASS_API FrameEx : public CButton
        {

        public:

            // Constructor
            FrameEx();

            // Creating FrameEx
            BOOL Create( LPCTSTR lpctszCaption_i, DWORD dwStyle_i, const RECT& stRect_i, CWnd* pParentWnd_i, UINT nID_i );

            // Function to set the text color
            void SetTextColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the frame color
            void SetFrameColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            // Function to set the frame width
            void SetLineThickness( int nThickness_i );
            /**
             * End of Change for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */

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

            // Function to set the font of the dialog
            virtual void PreSubclassWindow();

            // Destructor
            virtual ~FrameEx();

            // Overridden function to handle Invalidate.
            // This is required to avoid the call to Invalidate when window handle is invalid.
            void Invalidate( BOOL bErase_i = TRUE );

            // Function to set the curve type.
            void SetCurveType( CURVE_SHAPE_e eCurve_i );

        protected:

            //{{AFX_MSG(FrameEx)
            afx_msg void OnPaint();
            //}}AFX_MSG
            DECLARE_MESSAGE_MAP()

        protected:

            // Text color
            COLORREF m_clrText;
            // Frame color
            COLORREF m_clrFrame;
        private:

            // Object to initialize the gdi plus
            GdiPlusCtrl m_GdiInit;
            // Flag to check the control is created.
            bool m_bInitialize;

            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            int m_nFrameThickness; // Frame thickness
            /**
             * End of Change for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            // Holds frame curve shape.
            CURVE_SHAPE_e m_eCurveSize;
        };

        //{{AFX_INSERT_LOCATION}}
    } // End of namespace UICommon
} // End of namespace MPlus
#endif // #ifndef _FRAMEEX_H_

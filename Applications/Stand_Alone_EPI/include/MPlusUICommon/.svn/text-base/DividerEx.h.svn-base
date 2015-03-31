/**
 * Copyright(c) 2007 TOSHIBA Medical Systems Corporation, All Rights
 * Reserved.
 *
 * DividerEx.h - Include file for the DividerEx control
 *
 * @author :    Praveen Kumar C
 * @version:    1.0            Date:  2007-04-19
 * @author :    Nishad S
 * @version:    1.1            Date:  2007-07-09
 */

#ifndef _DIVIDEREX_H_
#define _DIVIDEREX_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <afxdtctl.h>

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
         * Copyright(c) 2007 TOSHIBA Medical Systems Corporation, All Rights
         * Reserved.
         *
         * DividerEx - The class creates the Divider control
         *
         * Traceability Tags : TCOM_MP_CS_UI_DIVIDEREX_SET_COLOR
         *                     TCOM_MP_CS_UI_DIVIDEREX_SET_THICKNESS
         *
         * @author :    Praveen Kumar C
         * @version:    1.0            Date:  2007-04-19
         */

        class CONTROLS_CLASS_API DividerEx : public CStatic
        {

        public:

            // Constructor
            DividerEx( );
            // Destructor
            virtual ~DividerEx( );
            // Function to set lower and upper color
            void SetColor( COLORREF clrLower_i, COLORREF clrUpper_i );
            // Function to set lower and upper thickness
            void SetThickness( int nLower_i, int nUpper_i );
            // Function to get lower and upper color
            void GetColor( COLORREF &clrLower_o, COLORREF &clrUpper_o );
            // Function to get lower and upper thickness
            void GetThickness( int &nLower_o, int &nUpper_o );

        //{{AFX_VIRTUAL(DividerEx)
            protected:
                virtual void PreSubclassWindow( );
        //}}AFX_VIRTUAL

        protected:

            //{{AFX_MSG(DividerEx)
            afx_msg void OnPaint( );
            afx_msg void OnWindowPosChanging( WINDOWPOS FAR* lpwndpos_i );
            afx_msg void OnSize( UINT nType_i, int nCX_i, int nCY_i );
            //}}AFX_MSG
            afx_msg LRESULT OnInitialize( WPARAM wParam_i, LPARAM lParam_i );
            DECLARE_MESSAGE_MAP()

        private:

            // The thickness of the lower line of the Divider control
            int m_nLowerThickness;
            // The thickness of the upper line of the Divider control
            int m_nUpperThickness;
            // The color of the lower line of the Divider control
            COLORREF m_clrLowerColor;
            // The color of the upper line of the Divider control
            COLORREF m_clrUpperColor;
            // Holds the horizontal line or vertical line flag
            bool m_bHorizontal;
            bool m_bStyleGot;
        };
        //{{AFX_INSERT_LOCATION}}
    } // End of namespace UICommon
} // End of namespace MPlus

#endif // #ifndef _DIVIDEREX_H_

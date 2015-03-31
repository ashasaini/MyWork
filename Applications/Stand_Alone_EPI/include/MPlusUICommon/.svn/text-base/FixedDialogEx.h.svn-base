/**
 * Copyright(c) 2006 TOSHIBA Medical Systems Corporation, All Rights Reserved.
 *
 * FixedDialogEx.h - This File include the declaration of the class FixedDialogEx
 *
 * @author :    Naveen.R
 * @version:    1.0             Date:  2007-06-07
 */

#ifndef _FIXED_DIALOGEX_
#define _FIXED_DIALOGEX_

#if _MSC_VER > 1000
#pragma once
#endif

#include "MPlusUICommon/GdiPlusCtrl.h"
#include <afxwin.h>

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
        struct FIXED_DLG_COLORS_t
        {
            COLORREF clrBorder;
            COLORREF clrTitleStart;
            COLORREF clrTitleEnd;
            COLORREF clrRibbonStart;
            COLORREF clrRibbonEnd;
            COLORREF clrBk;
            COLORREF clrTxtColor;
            COLORREF clrRibbonTopLine;
            COLORREF clrTitleOuterRect;
            COLORREF clrTitleInnerRect;
            COLORREF clrTitle3DTopLeft;
            COLORREF clrTitle3DBtmRight;
        };



        /**
         * Copyright(c) 2006 TOSHIBA Medical Systems Corporation, All Rights Reserved.
         *
         * FixedDialogEx - This class implements the Fixed dialog control. This dialog is different from the 
         * DialogEx in features such as non movable title bar, dark background, Sharp corners etc.
         *
         * @author :    Naveen.R
         * @version:    1.0		    Date:  2007-06-12
         */
        class CONTROLS_CLASS_API FixedDialogEx : public CDialog
        {
        // Construction
        public:

            // Constructor
            FixedDialogEx( UINT nIDD_i, CWnd* pParent_i = NULL );
            // To change the close button visibility
            void ShowCloseButton( bool bFlag_i = true );
            // To change the title bars' visibility
            void ShowTitleBar( bool bFlag_i = true );
            // Gets the bottom ribbon height
            int GetRibbonHeight();
            // Sets the bottom ribbon height
            void SetRibbonHeight( int nRibbonHeight_i );

            // Function to set various colors of the dialog.
            FIXED_DLG_COLORS_t GetDlgColors();
            // Function to get the current dialog colors
            void SetDlgColors(FIXED_DLG_COLORS_t& stDlgColors_i );
            // Function to set the font of the dialog in the titlebar
            bool SetFont( LPCTSTR lpctszFontName_i, int nPointSize_i, bool bBold_i = false );
            
            // Enable/disable mouse hover effect.
            void EnableHoverEffect( bool bEnable_i = true )
            {
                m_bHoverEffect = bEnable_i;
            }
            //{{AFX_VIRTUAL(FixedDialogEx)
	        protected:
	        virtual LRESULT WindowProc( UINT uMessage_i, WPARAM wParam_i, LPARAM lParam_i );
	        //}}AFX_VIRTUAL

        // Implementation
        protected:

            //{{AFX_MSG(FixedDialogEx)
            virtual BOOL OnInitDialog();
            afx_msg void OnNcPaint();
	        afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	        afx_msg BOOL OnEraseBkgnd(CDC* pDC);
            afx_msg void OnSize( UINT uType_i, int nWidth_i, int nHeight_i );
	        //}}AFX_MSG
            afx_msg void FixedDialogEx::OnStyleChanged( int nStyleType_i, LPSTYLESTRUCT pstStyleStruct_i );
            DECLARE_MESSAGE_MAP()

        private:

            void DrawButton( HDC hDc_i );
        private:

            FIXED_DLG_COLORS_t m_stDlgColors;
            int m_nTitleBarHeight;
            int m_nBorderwidth;
            int m_nRibbonHeight;
            int m_nButtonState;

            bool m_bShowCloseButton;
            bool m_bCaptionFlag;
            bool m_bIgnoreStyleChange;


            GdiPlusCtrl m_GdiInit;
            CFont* m_pFont;

            bool    m_bHoverEffect; // Hover effect flag
        };
    }
}

#endif // #ifndef _FIXED_DIALOGEX_

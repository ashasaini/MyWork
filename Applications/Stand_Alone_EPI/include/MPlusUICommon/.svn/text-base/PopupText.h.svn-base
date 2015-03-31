/**
 * Copyright(c) 2006 - 2009 TOSHIBA Medical Systems Corporation, All Rights
 * Reserved.
 *
 * PopupText.h - PopupText class declaration file.
 *
 * @author :    Nishad S
 * @version:    1.0            Date:  2006-04-20
 * @author :    Vinod N N
 * @version:    2.0            Date:  2007-05-10
 * @author :    Shypa Padmanabhan     (Modified for MPlusV4)
 * @version:    2.1            Date:  2009-01-09
 */

#ifndef _POPUPTEXT_H_
#define _POPUPTEXT_H_

#if _MSC_VER > 1000
#pragma once
#endif

/**
 * Added for Celeve PIMS Phase-5 requirement
 * @since  2.0
 */
#include <afxdtctl.h>
/**
 * End of Change  for Celeve PIMS Phase-5 requirement
 * @since  2.0
 */
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
         * PopupText - This class is for displaying pop up text similar to tooltip.
         *
         * @author :    Nishad S
         * @version:    1.0            Date:  2006-04-20
         */
        class CONTROLS_CLASS_API PopupText : public CWnd
        {
        public:

            PopupText();
            virtual ~PopupText();

            // ClassWizard generated virtual function overrides
            //{{AFX_VIRTUAL(PopupText)
            virtual void PreSubclassWindow();
            //}}AFX_VIRTUAL

        protected:

            //{{AFX_MSG(PopupText)
           /**
            * Modified for MPlus V4
            * @since 2.1
            */
            /// Modified for correcting the error "'static_cast' :cannot convert from 'UINT (__thiscall 
            /// MPlus::UICommon::PopupText::* )(CPoint)' to 'LRESULT (__thiscall CWnd::* )(CPoint)'"
            // afx_msg UINT OnNcHitTest(CPoint point);
            afx_msg LRESULT OnNcHitTest( CPoint point );
           /**
            * End of Modification for MPlus V4
            */
            afx_msg void OnPaint();
            //}}AFX_MSG
            DECLARE_MESSAGE_MAP()

        public:

            bool Create();
            void Show( CWnd* pParent_i, LPCTSTR lpctszText_i, CFont* pFont_i,
                       COLORREF clrText_i, COLORREF clrBk_i, CRect rect_i );
            void Hide();
            void UpdateColors( COLORREF clrText_i, COLORREF clrBk_i );

       /**
        * Modified for MPlus V4
        * @since 2.1
        */
        private:
            PopupText( const PopupText& PopupText_i );
            PopupText& operator=( const PopupText& PopupText_i );
       /**
        * End of Modification for MPlus V4
        */

        private:

            CString  m_csText;  // Text to be displayed.
            COLORREF m_clrBk;   // Background color.
            COLORREF m_clrText; // Text color.
            CFont*   m_pFont;   // Font of the text.
        };
    } // End of namespace UICommon
} // End of namespace MPlus

#endif // #ifndef _POPUPTEXT_H_

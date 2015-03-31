/**
 * Copyright(c) 2007 TOSHIBA Medical Systems Corporation, All Rights
 * Reserved.
 *
 * SplitterEx.h - SplitterEx class declaration file
 *
 * @author :    Jijeesh K V
 * @version:    1.0            Date:  2007-05-07
 */

#ifndef _SPLITTEREX_H_
#define _SPLITTEREX_H_

#include <afxext.h> 

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
         * Traceability Tags : TCOM_MP_CS_UI_SPLITTEREX_DRAW_SPLITTER
         *
         * SplitterEx - This class used to draw the splitter window
         *
         * @author :    Jijeesh K V
         * @version:    1.0            Date:  2007-05-07
         */
        class CONTROLS_CLASS_API SplitterEx : public CSplitterWnd
        {
        // Construction
        public:

            SplitterEx();

        // Attributes
        public:

        // Operations
        public:

            // Overrides
            // ClassWizard generated virtual function overrides
            //{{AFX_VIRTUAL(SpEx)
            //}}AFX_VIRTUAL

        // Implementation
        public:

            virtual ~SplitterEx();

        // Generated message map functions
        protected:

            //{{AFX_MSG(SpEx)
            //}}AFX_MSG
            DECLARE_MESSAGE_MAP()

        public:

            // Function to set the border color
            void SetBorderColor( COLORREF clrBorder_i );
            // Function to get the border color
            COLORREF GetBorderColor();
            // Function to set the splitter bar color
            void SetSplitterBarColor( COLORREF clrLower_i, COLORREF clrUpper_i );
            // Function to get the splitter bar color
            void GetSplitterBarColor( COLORREF& clrLower_o, COLORREF& clrUpper_o );

        protected:

            // Function to paint the splitter
            void OnDrawSplitter( CDC* pDC_i, ESplitType nType_i, const CRect& rect_i );

        private:

            COLORREF m_clrBorder;        // Holds border color
            COLORREF m_clrSplitterLower; // Holds splitter lower color
            COLORREF m_clrSplitterUpper; // Holds splitter upper color
        };
    } // End of namespace UICommon
} // End of namespace MPlus
#endif // #ifndef _SPLITTEREX_H_

/**
 * Copyright(c) 2007 - 2010 TOSHIBA Medical Systems Corporation, All Rights
 * Reserved.
 *
 * ScrollViewEx.h - ScrollViewEx class declaration file
 *
 * @author :    Jijeesh K V
 * @version:    1.0            Date:  2007-04-24
 * @author :    Radhakrishnan G.
 * @version:    2.0            Date:  2007-06-22
 * @author :    Rahul Kumar    ( Modified for MPlusV4 BugFIx Merging MPC06250 )
 * @version:    2.1            Date:  2010-04-22
 */

#ifndef _SCROLLVIEWEX_H_
#define _SCROLLVIEWEX_H_

#ifdef UI_CONTROLS_DLL
#define CONTROLS_CLASS_API __declspec( dllexport )
#else
#define CONTROLS_CLASS_API __declspec( dllimport )
#endif

// Header includes
#include "ScrollBarEx.h"

namespace MPlus
{
    namespace UICommon
    {
        /**
         * Copyright(c) 2007 TOSHIBA Medical Systems Corporation, All Rights
         * Reserved.
         *
         * ScrollViewEx - This class customize Scroll View control
         *
         * Traceability Tags : TCOM_MP_CS_UI_SCROLLVIEWEX_SET_COLORS
         *
         * @author :    Jijeesh K V
         * @version:    1.0            Date:  2007-04-24
         */
        class CONTROLS_CLASS_API ScrollViewEx : public CScrollView
        {
        protected:
            ScrollViewEx(); // Protected constructor used by dynamic creation
            DECLARE_DYNCREATE(ScrollViewEx)

        // Attributes
        public:

        // Operations
        public:

            // Overrides
            // ClassWizard generated virtual function overrides
            //{{AFX_VIRTUAL(ScrollViewEx)
        protected:
            virtual void OnDraw(CDC* pDC); // Overridden to draw this view
            virtual void OnInitialUpdate(); // First time after construct
            virtual void PreSubclassWindow();
            //}}AFX_VIRTUAL

        // Implementation
        protected:
            virtual ~ScrollViewEx();
            #ifdef _DEBUG
            virtual void AssertValid() const;
            virtual void Dump(CDumpContext& dc) const;
            #endif

            // Generated message map functions
            //{{AFX_MSG(ScrollViewEx)
            afx_msg void OnPaint();
            afx_msg void OnNcPaint();
            afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
            afx_msg void OnSize(UINT nType, int cx, int cy);
            afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
            afx_msg void OnMove(int x, int y);
            //}}AFX_MSG
            DECLARE_MESSAGE_MAP()

        public:

            // Function to set the border color
            void SetBorderColor( COLORREF clrBorder_i );
            // Function to get the border color
            COLORREF GetBorderColor();
            // Enable/disable mouse hover effect.
            void EnableHoverEffect( bool bEnable_i = true );
            /* Begin: Modification 2.1 for Bug ID #MPC06250 */
            // To avoid scrollview's scroll bar painting issue.
            void SetScrollSizes( int nMapMode_i, SIZE stTotalSize_i, const SIZE& stPageSize_i = sizeDefault,
                                 const SIZE& stLineSize_i = sizeDefault );
            /* End: Modification 2.1 for Bug ID #MPC06250 */

        protected:

            // Functions to set the scroll bars
            void SetScrollBars();
            void SetRgn( const CRect* pClientRect_i = 0, bool bHScroll_i = false, bool bVScroll_i = false );

        private:

            ScrollBarEx m_ScrollHorz; // Horizontal Scroll
            ScrollBarEx m_ScrollVert; // Vertical Scroll
            DWORD m_dwOldStyle; // store style
            int m_nScrollbarSize; // Scroll size
            COLORREF m_clrBorder; // Holds border color
            bool     m_bHoverEffect;  // Hover effect flag
        };
    } // End of namespace UICommon
} // End of namespace MPlus
#endif // #ifndef _SCROLLVIEWEX_H_


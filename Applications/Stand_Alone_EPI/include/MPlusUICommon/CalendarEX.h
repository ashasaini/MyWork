/**
 * Copyright(c) 2006 - 2009 TOSHIBA Medical Systems Corporation, All Rights
 * Reserved.
 *
 * CalendarEx.h - CalendarEx class declaration file
 *
 * @author :    Krishnakumar T G
 * @version:    1.0            Date:  2006-03-07
 * @author :    Vinod N N
 * @version:    2.0            Date:  2007-05-10
 * @author :    Nishad S
 * @version:    2.1            Date:  2007-06-21
 * @author :    Naveen R
 * @version:    2.2            Date:  2008-12-15
 * @author :    Shypa Padmanabhan     (Modified for MPlusV4)
 * @version:    2.3            Date:  2009-01-09
 */

#ifndef _CALENDAREX_H_
#define _CALENDAREX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef _UNICODE
#error Unicode definition is necessary to use this class
#endif

#ifdef UI_CONTROLS_DLL
#define CONTROLS_CLASS_API __declspec( dllexport )
#else
#define CONTROLS_CLASS_API __declspec( dllimport )
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
namespace MPlus
{
    namespace UICommon
    {
        /**
         * Copyright(c) 2006 - 2009 TOSHIBA Medical Systems Corporation, All Rights
         * Reserved.
         *
         * CALENDAR_COLORS_t - Calendar colors structure declaration
         *
         * @author :    Krishnakumar T G
         * @version:    1.0            Date:  2006-03-07
         * @author :    Vinod N N
         * @version:    2.0            Date:  2007-05-10
         */
        typedef struct CALENDAR_COLORS_tag CALENDAR_COLORS_t;
        struct CALENDAR_COLORS_tag
        {
            // Holds Background color
            COLORREF clrBackground;
            // Holds border color
            COLORREF clrBorder;
            // Holds grid color
            COLORREF clrGrid;
            // Holds selected day back color
            COLORREF clrSelectedDayBack;
            // Holds title text color
            COLORREF clrTitle;
            // Holds weed day caption color
            COLORREF clrWeek;
            // Holds day text color
            COLORREF clrDay;
            // Title background gradient start color
            COLORREF clrTitleBackStart;
            // Title background gradient end color
            COLORREF clrTitleBackEnd;
            // Graying color for next and previous month
            COLORREF clrGray;
        };


        /**
         * Copyright(c) 2006 TOSHIBA Medical Systems Corporation, All Rights
         * Reserved.
         *
         * CalendarEx - CalendarEx class declaration
         *
         * @author :    Krishnakumar T G
         * @version:    1.0            Date:  2006-03-07
         */
        class CONTROLS_CLASS_API CalendarEx : public CMonthCalCtrl
        {

        public:

            // Constructor
            CalendarEx();
            // Destructor
            virtual ~CalendarEx();

        public:

            // ClassWizard generated virtual function overrides
            //{{AFX_VIRTUAL(CalendarEx)
            public:
            virtual BOOL PreTranslateMessage( MSG* pMsg_i );
            protected:
            virtual void PreSubclassWindow();
            //}}AFX_VIRTUAL

        public:

            // Sets the week day start
            void SetWeekDayStart( int nWeekDay_i );
            // Sets the show next/previous month flag
            void ShowNextPrevMonth( bool bFlag_i );
            // Sets the calendar drawing colors
            void SetCalendarColors( CALENDAR_COLORS_t& stColors_i );
            // Gets the calendar drawing colors
            void GetCalendarColors( CALENDAR_COLORS_t& stColors_o );
            // Set the month text.
            void SetMonthText( LPCTSTR lpctszText_i );
            // Set the day text.
            void SetWeekDayText( LPCTSTR lpctszText_i );
            // Show and hide month turn over button
            void ShowMonthTurnOverBtn( bool bShow_i = true );
            // Calculate the minimum required size.
            BOOL GetMinReqRect( RECT* pRect_o ) const;
            // Set the font.

            /**
             * Tookout for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            // void SetFont( LPCTSTR lpctszFontName_i, int nPointSize_i );
            /**
             * End of Change for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */

            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            void SetFont( LPCTSTR lpctszFontName_i, int nPointSize_i, bool bBold_i = false );
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
            // Function to show the drop down style calendar.
           /**
            * Modified for MPlus V4
            * @since 2.3
            */
            //bool ShowDropDown( const CPoint ScreenPos_i );
            bool ShowDropDown( const CPoint& ScreenPos_i );
           /**
            * End of Modification for MPlus V4
            */
            // Function to relay the message from parent to the calendar.
            void RelayEvent( MSG* pMsg_i );
            // This function is used to hide the drop down mode calendar.
            void HideDropDown();
            // Enable/disable mouse hover effect.
            void EnableHoverEffect( bool bEnable_i = true )
            {
                m_bHoverEffect = bEnable_i;
            }

        protected:

            //{{AFX_MSG(CalendarEx)
            afx_msg void OnPaint();
            afx_msg void OnLButtonDown( UINT nFlags_i, CPoint Point_i );
            afx_msg void OnRButtonDown( UINT nFlags_i, CPoint Point_i );
            afx_msg void OnMouseMove( UINT uFlags_i, CPoint Point_i );
            afx_msg BOOL OnEraseBkgnd( CDC* pDC_i );
            afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
            /* Begin: Modification 2.2 for Bug #690, #691*/
            afx_msg void OnSize( UINT nType_i, int nCX_i, int nCY_i );
            /* End: Modification 2.2 for Bug #690, #691*/
            //}}AFX_MSG
            afx_msg LRESULT OnSetCurSel( WPARAM wParam_i, LPARAM lParam_i );

            DECLARE_MESSAGE_MAP()

        private:

           /**
            * Modified for MPlus V4
            * @since 2.3
            */
            CalendarEx( const CalendarEx& CalendarEx_i );
            CalendarEx& operator=( const CalendarEx& CalendarEx_i );
           /**
            * End of Modification for MPlus V4
            */
            // This function will send the MCN_SELCHANGE notification message to the parent.
            void NotifySelChange();
            // This function will send the MCN_SELECT notification message to the parent.
            void NotifySelect();
            // This function is used to parse the string.
            void ParseString( LPCTSTR lpctszStr_i, CString arStr_o[], int nSize_i );
            // This function is used to calculate the different size values.
            void CalculateSize();
            // This function is used to set the button positions.
            void SetButtonPos();
            // This function is used to find the first week day and end day of the current month.
            void GetDayStartEndInfo( int& nWeekDayStart_i, int& nLastDay_i );
            // This function is used to draw the button.
            void DrawButton( CDC& dc_i, const CRect& rect_i, bool bRightArrow_i,
                             COLORREF clrBorder_i, COLORREF clrFill_i, COLORREF clrArrow_i );
            // Function to process click on the left button ( previous month ).
            void OnLeftButton();
            // Function to process click on the right button ( next month ).
            void OnRightButton();

        private:

            // Constants that specify the month and day counts.
            enum { TOTAL_MONTHS = 12, TOTAL_DAYS = 7 };

            // Object to initialize the gdi plus
            GdiPlusCtrl m_GdiInit;
            // Holds Title bar height
            int m_nTitleHeight;
            // Holds total caption height including title and week day head.
            int m_nCaptionHeight;
            // Holds cell width and height
            CSize   m_CellSize;
            // Holds the total size
            CSize   m_TotalSize;
            // Zero to base index
            int m_nWeekDayStart;
            // Holds show next/previous month flag
            bool m_bShowNextPrev;

            // Holds calendar background color
            COLORREF m_clrBackground;
            // Holds Border color
            COLORREF m_clrBorder;
            // Holds grid color
            COLORREF m_clrGrid;
            // Holds selected day back color
            COLORREF m_clrSelectedDayBack;
            // Holds title text color
            COLORREF m_clrTitle;
            // Holds week caption display color
            COLORREF m_clrWeek;
            // Holds Day text color
            COLORREF m_clrDay;
            // Holds title gradient start color
            COLORREF m_clrTitleBackStart;
            // Holds title gradient end color
            COLORREF m_clrTitleBackEnd;
            // Holds the graying color for previous & next month
            COLORREF m_clrGray;
            // Holds the button colors
            COLORREF m_clrBtnBorderNormal;
            COLORREF m_clrBtnBorderHighlight;
            COLORREF m_clrBtnBkNormal;
            COLORREF m_clrBtnBkHighlight;
            COLORREF m_clrBtnArrowNormal;
            COLORREF m_clrBtnArrowHighlight;

            // Months and days strings
            CString m_arMonth[TOTAL_MONTHS];
            CString m_arDay[TOTAL_DAYS];

            // Font settings.
            CString m_csFontName;
            int     m_nFontSize;

            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */

            bool m_bBold;

            /**
             * End of Change for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */

            // Parent window pointer.
            CWnd*   m_pParentWnd;
            // Drop down mode flag.
            bool    m_bDropDownMode;
            // Button rects.
            CRect   m_LeftButtonRect;
            CRect   m_RightButtonRect;
            // Mouse inside flags.
            bool    m_bMouseInsideLeft;
            bool    m_bMouseInsideRight;
            bool    m_bHoverEffect; // Hover effect flag
            bool m_bMonthTurnOverMode; // Month turn over button flag
        };
    } // End of namespace UICommon
} // End of namespace MPlus

#endif // _CALENDAREX_H_

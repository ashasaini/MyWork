/**
 * Copyright(c) 2006 - 2007 TOSHIBA Medical Systems Corporation, All Rights
 * Reserved.
 *
 * DateTimeCtrlEx.h - Include file for the DateTimeCtrlEx.
 *
 * @author :    Naveen.R
 * @version:    1.0            Date:  2006-10-31
 *
 * @author :    Praveen Kumar C
 * @version:    2.0            Date:  2007-05-04
 * @author :    Nishad S
 * @version:    2.1            Date:  2007-07-13
 * @author :    Jijeesh K V
 * @version:    2.2            Date:  2008-05-27
 */


#ifndef _DATETIMECTRLEX_H_
#define _DATETIMECTRLEX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <MPlusUICommon/GdiPlusCtrl.h>
#include <MPlusUICommon/CalendarEx.h>

namespace MPlus
{
    namespace UICommon
    {

        /**
         * Copyright(c) 2006 - 2007 TOSHIBA Medical Systems Corporation, All Rights
         * Reserved.
         *
         * DateTimeCtrlEx - This class implements the DateTimePicker control.
         *
         * Traceability Tags : TCOM_MP_CS_UI_DATETIMECTRLEX_SET_COLORS
         *                     TCOM_MP_CS_UI_DATETIMECTRLEX_SET_STAR
         *
         * @author :    Naveen.R
         * @version:    1.0            Date:  2006-10-31
         *
         * @author :    Praveen Kumar. C
         * @version:    2.0            Date:  2007-05-04
         */
        class CONTROLS_CLASS_API DateTimeCtrlEx : public CDateTimeCtrl
        {

        public:

            // Constructor
            DateTimeCtrlEx();
    
            // Destructor
            virtual ~DateTimeCtrlEx();
    
            // Functions returns the time in the DateTimeCtrlEx
            DWORD GetTime( CTime& timeDest_i ) const;
            BOOL  GetTime( COleDateTime& timeDest_i ) const;
            DWORD GetTime( LPSYSTEMTIME pTimeDest_i ) const;
    
            // Returns the CalendarEx object used by the DateTimeCtrlEx
            CalendarEx& GetCalenderCtrl();

            // Function to set the background color
            void SetBkColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the text color
            void SetTextColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the selection color
            void SetSelectedTextColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            void SetSelectionColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the border of edit box
            void SetBorderColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the border of disabled edit box
            void SetBorderColorDisabled( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the focus rect color
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

            // Function to enable or disable the start behavior of the control.
            void EnableStarMode( bool bEnable_i );

            /**
             * Added for Celeve PIMS Phase-5 requirement ( Merge with V 1.8 )
             * @since  2.0
             */

            // Function to enable or disable the start behavior of the control.
            void EnableDelMode( bool bEnable_i );

            // Function to set the disabled background color
            void SetBkColorDisabled( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the disabled text color
            void SetTextColorDisabled( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to show or hide star
            void SetStar( bool bShow_i = true );

            /**
             * End of Change for Celeve PIMS Phase-5 requirement ( Merge with V 1.8 )
             * @since  2.0
             */

            // Function to show/hide the calendar button
            void ShowButton( bool bShow_i = true );

            // Function to drop the calendar
            void DropCalendar();

            // Show/Hide month turn over button
            void ShowMonthTurnOverBtn( bool bShow_i );

            //{{AFX_VIRTUAL(DateTimeCtrlEx)
            public:
            virtual BOOL OnChildNotify( UINT uMessage_i, WPARAM wParam_i, LPARAM lParam_i, LRESULT* pLResult_o );
            protected:
            virtual void PreSubclassWindow();
            virtual LRESULT WindowProc( UINT uMessage_i, WPARAM wParam_i, LPARAM lParam_i );
            //}}AFX_VIRTUAL

        protected:

            //{{AFX_MSG(DateTimeCtrlEx)
            afx_msg void OnPaint();
            afx_msg BOOL OnEraseBkgnd(CDC* pDC);
            afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
            afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
            afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
            afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
            afx_msg void OnNcPaint();
            //}}AFX_MSG

            DECLARE_MESSAGE_MAP()

        protected:
    
            // Variable that keeps the color of the ctrl
            COLORREF m_clrText;
            COLORREF m_clrBk;
            COLORREF m_clrSelectionText;
            COLORREF m_clrSelectionBk;
            COLORREF m_clrBorder;
            COLORREF m_clrFocusBorder;
            COLORREF m_clrNormalBorder;
            COLORREF m_clrBorderDisabled;

            /**
             * Added for Celeve PIMS Phase-5 requirement ( Merge with V 1.8 )
             * @since  2.0
             */

            // Holds disabled text color
            COLORREF m_clrTextDisabled;
            // Holds disabled background color
            COLORREF m_clrBkDisabled;

            /**
             * End of Change for Celeve PIMS Phase-5 requirement ( Merge with V 1.8 )
             * @since  2.0
             */

            // Variables that keeps the position of the each item in ctrl
            int m_nDayPos;
            int m_nShortMonthPos;
            int m_nLongMonthPos;
            int m_nYearPos;
            int m_nSelectedField;
    
            // Variables that keeps the operation states of the ctrl
            bool m_bIgnoreAction;
            bool m_bIgnoreChange;
            bool m_bHaveFocus;
            bool m_bFontSet;
            bool m_bStarMode;
            bool m_bDelMode;

            // Variables that keeps intermediate Data's
            CString m_csEnteredText;
            int m_nEnteredNo;

            /* Begin: Took out 2.2 */
//             // The Drop Down Button
//             CBitmap m_bmpCalender;
//             CDC m_BmpDc;
            /* End: Took out 2.2 */
            int m_nButtonWidth ;
    
            // Object to initialize the gdi plus
            GdiPlusCtrl m_GdiInit;

            // The calender object used by the control
            CalendarEx m_DropCalender;

            // Flag for calendar button visibility
            bool m_bBtnVisible;
        };
    }
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}

#endif // #ifndef _DATETIMECTRLEX_H_

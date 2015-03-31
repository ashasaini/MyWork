/**
 * Copyright(c) 2006 - 2010 TOSHIBA Medical Systems Corporation, All Rights
 * Reserved.
 *
 * ListCtrlEx.h - Include file for list control
 *
 * @author :    Naveen.R
 * @version:    1.0            Date:  2006-03-06
 * @author :    Jijeesh K V
 * @version:    2.0            Date:  2007-04-24
 * @author :    Naveen.R
 * @version:    2.1            Date:  2007-06-07
 * @author :    Nishad S
 * @version:    2.2            Date:  2007-07-05
 * @author :    Jayasankar.S
 * @version:    2.3            Date:  2008-04-24
 * @author :    Naveen R
 * @version:    2.4            Date:  2008-12-17
 * @author :    Naveen R
 * @version:    2.5            Date:  2008-12-22
 * @author :    Shrijesh M N ( Modified for MPlusV4 BugFIx Merging MPC04751 )
 * @version:    2.6            Date:  2009-06-16
 * @author :    Jinesh P.P.  ( Modified for MPlusV4 BugFix Merging #MPC05898(Performance Improvement))
 * @version:    2.7            Date:  2010-02-15
 * @author :    Jinesh P.P.  ( Modified for MPlusV4 BugFix Merging #MPC04468 )
 * @version:    2.8            Date:  2010-03-23
 * @author :    Jinesh P.P.  ( Modified for MPlusV4 BugFix Merging #MPC06257 )
 * @version:    2.9            Date:  2010-04-13
 */

#ifndef _LISTCTRLEX_H_
#define _LISTCTRLEX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MPlusUICommon/ScrollBarEx.h"
#include "MPlusUICommon/HeaderCtrlEx.h"
#include <afxtempl.h>

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
        // List Align type
        enum LISTITEM_ALIGN_e
        {
            LIST_ALIGN_TOP    = DT_TOP,
            LIST_ALIGN_CENTER = DT_VCENTER,
            LIST_ALIGN_BOTTOM = DT_BOTTOM
        };

        // Sort type
        enum SORT_TYPE_e
        {
            SORT_TYPE_DATE = 0,
            SORT_TYPE_NUMBER,
            SORT_TYPE_DECIMAL,
            SORT_TYPE_STRING_NOCASE,
            SORT_TYPE_STRING_MATCHCASE
        };

        /**
         * Copyright(c) 2006 - 2007 TOSHIBA Medical Systems Corporation, All Rights
         * Reserved.
         *
         * ListCtrlEx - This class extends the list controls features
         *
         * Traceability Tags : TCOM_MP_CS_UI_LISTCTRLEX_SET_COLORS
         *
         * @author :    Naveen.R
         * @version:    1.0            Date:  2006-03-06
         * @author :    Jijeesh K V
         * @version:    2.0            Date:  2007-04-24
         * @author :    Naveen.R
         * @version:    2.1            Date:  2007-06-07
         * @author :    Nishad S
         * @version:    2.2            Date:  2007-07-05
         * @author :    Jayasankar.S
         * @version:    2.3            Date:  2008-04-24
         */
        class CONTROLS_CLASS_API ListCtrlEx : public CListCtrl
        {

        public:

            // Constructor
            ListCtrlEx();

            // Function to create the list ctrl
            BOOL Create( DWORD dwStyle_i, const RECT& Rect_i, CWnd* pParentWnd_i, UINT uID_i );

            // Set the image associated with a cell
            bool SetItemImage( int nItem_i, int nSubItem_i, int nImage_i );

            int InsertColumn( int nCol_i, LPCTSTR lpctszColumnHeading_i,
                              int nFormat_i = LVCFMT_LEFT, int nWidth_i = -1,
                              int nSubItem_i = -1 );

            // Just to get the default InsertColumn API in the base class.
            int InsertColumn( int nCol_i, const LVCOLUMN* pColumn_i )
            {
                return CListCtrl::InsertColumn( nCol_i, pColumn_i );
            }

            // Set the change the images showing mode
            void ShowIcons( bool bShow_i, bool bInavlidate_i = true );

            // Set the images associated with the header control items
            BOOL SetHeaderImage(int nItem_i, int nImage_i );

            // Draw the sort arrow in the specified header item
            void SetSortArrow( int iColumn_i = -1, bool bAscending_i = true );

            // Function calls the HeaderCtrlEx function to set the column range
            void SetColumnRange( int nItem_i, int nMin_i, int nMax_i );

            // Sets the border color of the list control
            void SetBorderColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Set the alternate row colors of the list control
            void SetRowColors( COLORREF clrColorOne_i, COLORREF clrColorTwo_i, bool bInvalidate_i = true );

            // Function to set the text color
            void SetTextColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the text selection color
            void SetTextSelColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the selection background color
            void SetBkSelColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Set the vertical grid color
            void SetGridColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the border color in the disabled state
            void SetBorderDisableColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Set the alternate row colors of the list control in the disabled state
            void SetRowDisableColors( COLORREF clrColorOne_i, COLORREF clrColorTwo_i, bool bInvalidate_i = true );

            // Function to set the background color in the disabled state
            void SetBkDisableColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the text color in the disabled state
            void SetTextDisableColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the grid color in the disabled state
            void SetGridDisableColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the focus rect color
            void SetFocusRectColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            // Function to set the focus rect colors
            void SetItemFocusRectColors( COLORREF clrBorder_i, COLORREF clrFill_i );
            // Function to set the color of non client rectangle in the right bottom side
            void SetNCColor( COLORREF clrColor_i, bool bInvalidate_i = true );
            
            // Function to set the horizontal grid color
            void SetItemHorzGridColor( COLORREF clrColor_i, bool bInvalidate_i = true );
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

            // Returns reference of HeaderCtrlEx of this list control
            HeaderCtrlEx& GetHeaderCtrlEx(){ return m_HeaderCtrl; }

            // Function to set the list empty message.
            void SetEmptyMessage( LPCTSTR lpctszMsg_i );

            // function to set row height
            void SetRowHeight( int nNewHeight_i );

            // Function to set vertical alignment of text in rows
            void SetItemAlign( LISTITEM_ALIGN_e eItemAlign_i, bool bInvalidate_i = true );

            // Sets sort type
            void SetSortType( int nColumn_i, SORT_TYPE_e eSortType_i = SORT_TYPE_STRING_NOCASE );

            // Remove Sort type from a column
            void RemoveSortType( int nColumn_i );

            /**
             * Added by NeST for 2.1 ver requirment
             * @since  2.1
             */
            // Function to set the border width of the List control
            bool SetBorderWidth( int nWidth_i );
            /**
             * End of Change for 2.1 ver requirment
             * @since  2.1
             */
            /* Begin: Added 2.3 */
            //Function used to set Multi-Property
            void SetMultiLine( bool bMultiline_i );
            /* End: Added 2.3 */
            // Destructor
            virtual ~ListCtrlEx();

            // Overridden function to handle Invalidate.
            // This is required to avoid the call to Invalidate when window handle is invalid.
            void Invalidate( BOOL bErase_i = TRUE )
            {
                if( FALSE != IsWindow( m_hWnd ))
                {
                    ::InvalidateRect( m_hWnd, 0, bErase_i );
                }
            }

            // Enable/disable mouse hover effect.
            void EnableHoverEffect( bool bEnable_i = true );

            /* Begin: Added 2.3 */
            // Overrided the CListCtrl::SetItemText()
            BOOL SetItemText( int nItem, int nSubItem, LPCTSTR lpszText );
            // Get the max text height
            int GetMaxTextHeight( const int nColumnId_i = -1, bool bRemove = false );
            /* End: Added 2.3 */

            /* Begin: Modification 2.4 #741 - New fucntions added for getting colors*/
            COLORREF GetBkDisableColor();
            COLORREF GetBkSelColor();
            COLORREF GetBorderColor();
            COLORREF GetBorderDisableColor();
            COLORREF GetFocusRectColor();
            COLORREF GetGridColor();
            COLORREF GetGridDisableColor();
            COLORREF GetItemFocusRectColor();
            COLORREF GetItemHorzGridColor();
            COLORREF GetNCColor();
            void GetRowColors( COLORREF& clrOddRowColor_o, COLORREF& clrEvenRowColor_o );
            void GetRowDisableColors( COLORREF& clrOddRowColor_o, COLORREF& clrEvenRowColor_o );
            COLORREF GetTextColor();
            COLORREF GetTextDisableColor();
            COLORREF GetTextSelColor();
            /* End: Modification 2.4 #741*/
            /* Begin: Modification 2.8 for Bug ID #MPC04468 */
            virtual void GetClientRect( LPRECT lpRect_o );
            /* End: Modification 2.8 for Bug ID #MPC04468 */
            /* Begin: Modification 2.9 for Bug ID #MPC06257 */
            void GetCellColors( int nRow_i, int /*nCol_i*/, bool bSelected_i,
                                COLORREF& clrFont_o, COLORREF& clrBackground_o );
            /* End: Modification 2.9 for Bug ID #MPC06257 */
            //{{AFX_VIRTUAL(ListCtrlEx)
            protected:
            virtual void PreSubclassWindow();
            //}}AFX_VIRTUAL

        protected:

            //{{AFX_MSG(ListCtrlEx)
            afx_msg BOOL OnEraseBkgnd( CDC* pDC_i );
            afx_msg void OnPaint();
            afx_msg void OnVScroll( UINT uSBCode_i, UINT uPos_i, CScrollBar* pScrollBar_i );
            afx_msg void OnHScroll( UINT uSBCode_i, UINT uPos_i, CScrollBar* pScrollBar_i );
            afx_msg void OnNcPaint();
            afx_msg void OnEnable( BOOL bEnable_i );
            afx_msg void OnNcCalcSize( BOOL bCalcValidRects_i, NCCALCSIZE_PARAMS FAR* lpncsp_i );
            afx_msg void OnSize( UINT uType_i, int nWidth_i, int nHeight_i );
            afx_msg void OnShowWindow( BOOL bShow_i, UINT uStatus_i );
            afx_msg void OnSetFocus( CWnd* pOldWnd_i );
            afx_msg void OnKillFocus( CWnd* pNewWnd_i );
            afx_msg void OnNcDestroy();
            afx_msg void OnMove(int x, int y);
            /* Begin: Modification 2.8 for Bug ID #MPC04468 */
            afx_msg LRESULT OnNcHitTest( CPoint Point_i );
            /* End: Modification 2.8 for Bug ID #MPC04468 */
            //}}AFX_MSG
            DECLARE_MESSAGE_MAP()

            // Function to initialize the list control
            void Initialize();

            // Function to identify the image parameters
            afx_msg LRESULT OnSetImgList( WPARAM wParam_i, LPARAM lParam_i );

            // Function to set the item height
            afx_msg void MeasureItem ( LPMEASUREITEMSTRUCT lpMeasureItemStruct_i );

            // Function to draw list control
            void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct_i );

            // Function called before drawing each subitem
            virtual bool PreSubItemDraw( const int nItem_i, const int nSubItem_i, const bool bSelected_i, COLORREF& clrText_o );

            /* Begin: Modification 2.4 #735 - Function made virtual for overriding in ComboListCtrl*/
            // Sets the window region
            // void SetRgn( const CRect* pClientRect_i = 0, bool bHScroll_i = false, bool bVScroll_i = false );
            virtual void SetRgn( const CRect* pClientRect_i = 0, bool bHScroll_i = false, bool bVScroll_i = false );
            /* End: Modification 2.4 #735*/

            // Sets the scroll bar positions
            void SetScrollBars();

            // Draws the message text when the list is empty.
            void ShowEmptyMessage( CDC& dc_i );

            // Draws the vertical column separator lines.
            /* Begin: Modification 2.6 for Bug ID MPC04751 */
            // void DrawColumnLines( CDC& dc_i, int nTop_i, int nBottom_i );
            void DrawColumnLines( CDC& dc_i, int nTop_i, int nBottom_i, int nRight_i );
            /* End: Modification 2.6 for Bug ID MPC04751 */

            // Standard GetHeaderCtrl function is overridden here as protected.
            // User should use GetHeaderCtrlEx.
            CHeaderCtrl* GetHeaderCtrl()
            {
                return CListCtrl::GetHeaderCtrl();
            }

            // Function to track the deletion.
            LRESULT OnDeleteItem( WPARAM wParam_i, LPARAM lParam_i );

            // Function to track the insertion.
            LRESULT OnInsertItem( WPARAM wParam_i, LPARAM lParam_i );

            // Function to process list ctrl messages
            LRESULT WindowProc( UINT uMessage_i, WPARAM wParam_i, LPARAM lParam_i );

            // Compare Procedure for Strings, No Case
            static int CALLBACK CompareStringNoCaseProc( LPARAM lParam1_i, LPARAM lParam2_i, LPARAM lParamSort_i );

            // Compare Procedure for Strings, Case Sensitive
            static int CALLBACK CompareStringMatchCaseProc( LPARAM lParam1_i, LPARAM lParam2_i, LPARAM lParamSort_i );

            // Compare Procedure for Number
            static int CALLBACK CompareNumberProc( LPARAM lParam1_i, LPARAM lParam2_i, LPARAM lParamSort_i );

            // Compare Procedure for Decimal Number
            static int CALLBACK CompareDecimalProc( LPARAM lParam1_i, LPARAM lParam2_i, LPARAM lParamSort_i );

            // Compare procedure for Date
            static int CALLBACK CompareDateProc( LPARAM lParam1_i, LPARAM lParam2_i, LPARAM lParamSort_i );

        private:

            /* Begin: Modification 2.7 for Bug ID #MPC05898(Performance Improvement) */
            void GetVisibleColumnRange( int nColumnCount_i, const int* pnColumnOrder_i,
                                        int& nLeftColumn_o, int& nRightColumn_o );
            /* End: Modification 2.7 for Bug ID #MPC05898(Performance Improvement) */

        protected:

            // Vertical scroll bar
            ScrollBarEx m_ScrollVert;
            // Horizontal scroll bar
            ScrollBarEx m_ScrollHorz;
            // The current window style
            DWORD m_dwOldStyle;
            /* Begin: Modification 2.4 #735 - Variable made protected to access from ComboListCtrl*/
            // Size of the scrollbar.
            int m_nScrollbarSize;
            /* End: Modification 2.4 #735*/

        private:

            // Text color
            COLORREF m_clrText;
            // Disabled text color
            COLORREF m_clrDisabledText;
            // Bk color
            COLORREF m_clrBk;
            // Disabled bk color
            COLORREF m_clrDisabledBk;
            // Text color when selected
            COLORREF m_clrSelText;
            // Background color of selected items
            COLORREF m_clrSelBk;
            // Background color of odd items in the list controls
            COLORREF m_clrOddRow;
            // Background color of odd items in the list controls in disabled state
            COLORREF m_clrDisabledOddRow;
            // Background color id the even items in the list control
            COLORREF m_clrEvenRow;
            // Background color id the even items in the list control in disabled state
            COLORREF m_clrDisabledEvenRow;
            // Border color
            COLORREF m_clrNormalBorderColor;
            COLORREF m_clrBorder;
            // Disabled border color
            COLORREF m_clrDisabledBorder;
            // Grid color
            COLORREF m_clrGrid;
            // Disabled grid color
            COLORREF m_clrDisabledGrid;
            // Border color in focused state
            COLORREF m_clrFocusBorderColor;

            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            // Horizontal grid color
            COLORREF m_clrItemHorzGrid;
            // Focus rect colors
            COLORREF m_clrItemFocusRectBorder;
            /* Begin: Modification 2.5 for Bug #739 & 740*/
            // COLORREF m_clrItemFocusRectFill;
            /* End: Modification 2.5 for Bug #739 & 740*/
            /**
             * End of Change for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */

            // Flag which specifies whether the icon should be drawn or not
            bool m_bDrawIcon;
            // The header control of the list control
            HeaderCtrlEx m_HeaderCtrl;
            // Bk color of the dialog, used to fill the right bottom box formed by scrollbars.
            COLORREF m_clrDlgBk;
            /* Begin: Modification 2.4 #735 - Variable made protected to access from ComboListCtrl*/
            // Size of the scrollbar.
            // int m_nScrollbarSize;
            /* End: Modification 2.4 #735*/

            // The width of icon in ListCtrl.
            int m_nImgOffset;
            // The offset position from top.
            int m_nImgTopPos;
            // Flag that specifies the member are initialized
            bool m_bInitialized;
            // Message that to be shown when the list is empty.
            CString m_csEmptyMsg;
            // Row height
            int m_nRowHeight;
            // Vertical text alignment in rows
            LISTITEM_ALIGN_e m_eItemAlign;
            // Array that keep sort type of each column
            CMap<int, int, SORT_TYPE_e, SORT_TYPE_e> m_SortTypeMap;

            /**
             * Added by NeST for 2.1 ver requirment
             * @since  2.1
             */
            // Additional Border Width 
            int m_nBorderWidth;
            /* Begin: Added 2.3 */
            // Keeps maximum Row Height 
            int m_nRowMaxHeight;
            bool m_bReCalculate; // Holds flag to know whethr the row height to be recalculated
            int m_nDragColumn; // The column id which is in drag condition
            CString csLongString; // Longest string in the dragging column
            int m_nSecMaxRowHt; // The maximum row height that is apart from current dragging column
            /* End: Added 2.3 */

            bool    m_bHoverEffect; // Hover effect flag
            /* Begin: Added 2.3 */
            // Multiline Property Flag
            bool    m_bMultiLine;  
            /* End: Added 2.3 */
            /**
             * End of Change for 2.1 ver requirment
             * @since  2.1
             */
        };

        //{{AFX_INSERT_LOCATION}}
    } // End of namespace UICommon
} // End of namespace MPlus

#endif // #ifndef _LISTCTRLEX_H_

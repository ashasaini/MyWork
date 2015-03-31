/**
 * Copyright(c) 2006 - 2007 TOSHIBA Medical Systems Corporation, All Rights
 * Reserved.
 *
 * ListBoxEx.h - Include file for the ListBoxEx control
 *
 * @author :    Naveen.R
 * @version:    1.0            Date:  2006-02-27
 * @author :    Jijeesh K V
 * @version:    2.0            Date:  2007-05-11
 * @author :    Nishad S
 * @version:    2.1            Date:  2007-08-10
 * @author :    Jijeesh K V
 * @version:    2.2            Date:  2008-06-25
 */

#ifndef _LISTBOXEX_H_
#define _LISTBOXEX_H_

#if _MSC_VER > 1000
#pragma once
#endif

/**
 * Added for Celeve PIMS Phase-5 requirement
 * @since  2.0
 */
#include <afxcmn.h>
/**
 * End of Change  for Celeve PIMS Phase-5 requirement
 * @since  2.0
 */
#include "MPlusUICommon/RadioDraw.h"
#include "MPlusUICommon/CheckBoxDraw.h"
#include "MPlusUICommon/ScrollBarEx.h"
#include <afxtempl.h>

namespace MPlus
{
    namespace UICommon
    {
        // Notification message used to send check/uncheck events.
        #define LBEX_NM_CHECKITEM 0x0001

        // Notification message structure.
        typedef struct
        {
            NMHDR hdr;
            int   nItem;
            bool  bChecked;
        } LBEXCHECKNM_t;
        
        // ListBoxEx Extended styles
        enum LIST_EXSTYLES_e
        {
            LIST_NONE = 0,
            LIST_RADIO,
            LIST_CHECK,
            LIST_ICON,
        };

        /**
         * Copyright(c) 2006 - 2007 TOSHIBA Medical Systems Corporation, All Rights
         * Reserved.
         *
         * ListBoxEx - This class extends the listbox control
         *
         * @author :    Naveen.R
         * @version:    1.0            Date:  2006-02-27
         * @author :    Jijeesh K V
         * @version:    2.0            Date:  2007-05-11
         */
        class CONTROLS_CLASS_API ListBoxEx : public CListBox
        {

        public:

            // Constructor
            ListBoxEx();

            // Set the image list associated with the list box
            void SetImageList( CImageList* pImageList_i );

            // Returns the pointer to the image list
            CImageList* GetImageList();

            // Function to set the background color
            void SetBorderColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the background color
            // Set the alternate row colors of the list control
            void SetRowColors( COLORREF clrColorOne_i, COLORREF clrColorTwo_i, bool bInvalidate_i = true );
            //void SetBkColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Set the alternate row colors of the list control
            void SetDisabledRowColors( COLORREF clrColorOne_i, COLORREF clrColorTwo_i, bool bInvalidate_i = true );

            // Function to set the text color
            void SetTextColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // function to set the text selection color
            void SetTextSelColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the selection background color
            void SetBkSelColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the text color in the disabled state
            void SetTextDisableColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the border color in the disabled state
            void SetBorderDisableColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Function to set the background color in the disabled state
            void SetBkDisableColor( COLORREF clrColor_i, bool bInvalidate_i = true );

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

            // Returns the tooltip control of the list
            CToolTipCtrl* GetToolTipCtrl(){ return &m_ToolTip; }

            // Function to add a string to the list box
            int AddString( LPCTSTR lpctszItem_i, int nImage_i = -1 );

            // Function to insert a string to the list box
            int InsertString( int nInsertPos_i , LPCTSTR lpctszItem_i, int nImage_i = -1 );

            // Function to delete a string from the list box
            int DeleteString( int nIndex_i );

            // Function to clear all the contents in the list box
            void ResetContent();

            // Function to set the extended list box styles
            bool SetExStyle( LIST_EXSTYLES_e eExStyle_i );

            // Function to set or reset the check state of the item.
            void SetCheck( int nIndex_i, bool bCheck_i = true );

            // Returns the no of items selected by check box or radio button
            int GetExSelCount();

            // Returns a buffer that is filled with selected items index
            bool GetExSelItems( UINT* puItems_i, int nSize_i );

            // Returns a pointer to the class that draws the radio button
            RadioDraw* GetRadio(){ return &m_RadioButton; }

            // Returns a pointer to the class that draws the check button
            CheckBoxDraw* GetCheckBox(){ return &m_CheckButton; }

            // Function to set the check selection mode.
            void EnableFullRowCheckSel( bool bEnable_i = true )
            {
                m_bFullRowCheckSel = bEnable_i;
            }

            // Enable/disable mouse hover effect.
            void EnableHoverEffect( bool bEnable_i = true );            

            // Function to set the horizontal grid color
            void SetItemHorzGridColor( COLORREF clrColor_i, bool bInvalidate_i = true );

            // Destructor
            virtual ~ListBoxEx();

            // Overridden function to handle Invalidate.
            // This is required to avoid the call to Invalidate when window handle is invalid.
            void Invalidate( BOOL bErase_i = TRUE )
            {
                if( FALSE != IsWindow( m_hWnd ))
                {
                    ::InvalidateRect( m_hWnd, 0, bErase_i );
                }
            }


        public:

            //{{AFX_VIRTUAL(ListBoxEx)
            public:
            /* Begin: Added 2.2 */
            virtual void MeasureItem( LPMEASUREITEMSTRUCT lpMeasureItemStruct_O );
            /* End: Added 2.2 */
            virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct_i );
            virtual BOOL PreTranslateMessage( MSG* pMsg_i );
            protected:
            virtual void PreSubclassWindow();
            virtual LRESULT WindowProc( UINT Message_i, WPARAM wParam_i, LPARAM lParam_i );
            //}}AFX_VIRTUAL

        protected:

            //{{AFX_MSG(ListBoxEx)
            afx_msg HBRUSH CtlColor( CDC* pDC_i, UINT uCtlColor_i );
            afx_msg void OnNcPaint();
            afx_msg void OnLButtonDown( UINT nFlags_i, CPoint point_i );
            afx_msg void OnPaint();
            afx_msg BOOL OnMouseWheel( UINT uFlags_i, short sDelta_i, CPoint pt_i );
            afx_msg void OnKeyDown( UINT uChar_i, UINT uRepCnt_i, UINT uFlags_i );
            afx_msg void OnMouseMove( UINT uFlags_i, CPoint point_i );
            afx_msg void OnVScroll( UINT uSBCode_i, UINT uPos_i, CScrollBar* pScrollBar_i );
            afx_msg void OnEnable( BOOL bEnable_i );
            afx_msg void OnNcCalcSize( BOOL bCalcValidRects_i, NCCALCSIZE_PARAMS FAR* lpncsp_i );
            afx_msg BOOL OnEraseBkgnd(CDC* pDC);
            afx_msg void OnShowWindow( BOOL bShow_i, UINT uStatus_i );
            afx_msg void OnSize(UINT nType, int cx, int cy);
            afx_msg void OnMove(int x, int y);
            //}}AFX_MSG
            DECLARE_MESSAGE_MAP()
            afx_msg LRESULT OnInitialize( WPARAM wParam_i, LPARAM lParam_i );

            // Function to set window region.
            void SetRgn( const CRect* pClientRect_i = 0, bool bHScroll_i = false, bool bVScroll_i = false );
            // Function to set the Scrollbar values.
            void SetScrollBars();

        protected:

            // Odd row colors
            COLORREF m_clrOddRowColor;
            // Even Row color
            COLORREF m_clrEvenRowColor;
            // Background color in disabled state.
            COLORREF m_clrBkColorDisabled;
            // Text color
            COLORREF m_clrText;
            // Text color when disabled
            COLORREF m_clrTextDisabled;
            // Border color
            COLORREF m_clrBorder;
            // Border color in disabled state
            COLORREF m_clrBorderDisabled;

            // Holds disabled even row color
            COLORREF m_clrDisabledEvenRow;

            // Holds disabled odd row color
            COLORREF m_clrDisabledOddRow;

            // Selection color
            COLORREF m_clrSelBk;
            // Text color when selected
            COLORREF m_clrSelText;
            // List box extended style
            LIST_EXSTYLES_e  m_eExStyle;
            // Object for drawing the radio button inside the list
            RadioDraw m_RadioButton;
            // Object for drawing the check box inside the list
            CheckBoxDraw m_CheckButton;
            // Map that keeps the checked items in the list
            CMap<int,int,bool,bool> m_CheckedMap;
            // Array that keeps the image corresponding to each item
            CArray<UINT,UINT> m_ImageNumer;
            // Variable that keeps the currently checked radio button
            int m_iCheckedRadio;
            // Pointer to the image list associated with the list box
            CImageList *m_pImageList;
            // Vertical scroll bar
            ScrollBarEx m_ScrollVert;
            // Horizontal scroll bar
            ScrollBarEx m_ScrollHorz;
            // Variable that keep the window style
            DWORD m_dwOldStyle;
            // Object for showing the tooltip
            CToolTipCtrl m_ToolTip;
            // Size of the scrollbar.
            int m_nScrollbarSize;
            // The width of the image in list box
            int m_nImageOffset;
            // The height of the images in image list
            int m_nImageHeight;
            // For tooltip handling
            int m_nPrevItem;
            // Discrete check selection mode flag.
            bool m_bFullRowCheckSel;
            // Flag to check the font is set or not.
            bool m_bFontSet;
            // flag that indicates whether the hover effect is enabled or disabled
            bool m_bHoverEffect;
            // Horizontal grid color
            COLORREF m_clrItemHorzGrid;
        };

        //{{AFX_INSERT_LOCATION}}
    } // End of namespace UICommon
} // End of namespace MPlus
#endif // #ifndef _LISTBOXEX_H_

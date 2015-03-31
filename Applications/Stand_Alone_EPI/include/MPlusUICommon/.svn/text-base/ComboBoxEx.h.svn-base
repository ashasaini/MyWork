/**
 * Copyright(c) 2006 - 2010 TOSHIBA Medical Systems Corporation, All Rights
 * Reserved.
 *
 * ComboBoxEx.h - ComboBoxEx class declaration file.
 *
 * @author :    Nishad S
 * @version:    1.0            Date:  2006-03-14
 * @author :    Jijeesh K V
 * @version:    2.0            Date:  2007-04-24
 * @author :    Nishad S
 * @version:    2.1            Date:  2007-06-21
 * @author :    Nishad S
 * @version:    2.2            Date:  2007-07-02
 * @author :    Jayasankar.s
 * @version:    2.3            Date:  2008-05-14
 * @author :    Jijeesh K V
 * @version:    2.4            Date:  2008-05-27
 * @author :    Jijeesh K V
 * @version:    2.5            Date:  2008-06-25
 * @author :    Naveen R
 * @version:    2.6            Date:  2008-07-10
 * @author :    Naveen R
 * @version:    2.7            Date:  2008-09-19
 * @author :    Shypa Padmanabhan     (Modified for MPlusV4)
 * @version:    2.8            Date:  2009-01-09
 * @author :    Jinesh P.P.   ( Modified for MPlusV4 BugFix Merging MPC05905 )
 * @version:    2.9            Date:  2010-01-11
 */

#ifndef _COMBOBOXEX_H_
#define _COMBOBOXEX_H_

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _UNICODE
#error Unicode definition is necessary to use this class
#endif

#ifdef UI_CONTROLS_DLL
#define CONTROLS_CLASS_API __declspec( dllexport )
#else
#define CONTROLS_CLASS_API __declspec( dllimport )
#endif

#include "MPlusUICommon/DropWnd.h"
#include "MPlusUICommon/ComboStaticCtrl.h"
#include "MPlusUICommon/ComboDropBtn.h"
/* Begin: Added 2.3 */
#include "MPlusUICommon/ComboEdit.h"
/* End: Added 2.3 */
/* Begin: Added 2.5 */
#include "MPlusUICommon/ComboEditEx.h"
/* End: Added 2.5 */

namespace MPlus
{
    namespace UICommon
    {
        /**
         * Copyright(c) 2006 - 2009 TOSHIBA Medical Systems Corporation, All Rights
         * Reserved.
         *
         * ComboBoxEx - This class is to customize the standard combo box.
         *
         * Traceability Tags : TCOM_MP_CS_UI_COMBOBOXEX_SET_COLORS
         *                     TCOM_MP_CS_UI_COMBOBOXEX_HANDLE_MOUSEMOVE
         *                     TCOM_MP_CS_UI_COMBOBOXEX_HANDLE_MOUSELEAVE
         *
         * @author :    Nishad S
         * @version:    1.0            Date:  2006-03-14
         * @author :    Jijeesh K V
         * @version:    2.0            Date:  2007-04-24
         * @author :    Jayasankar.s
         * @version:    2.3            Date:  2008-05-14
         */
        class CONTROLS_CLASS_API ComboBoxEx : public CComboBox
        {

        public:

            // Hover effect flags
            enum HoverEffect_e
            {
                HOVER_CONTROL       = 0x0001,
                HOVER_CHILD_CONTROL = 0x0002
            };

        public:

            // Constructor.
            ComboBoxEx();
            // Destructor.
            virtual ~ComboBoxEx();

            // ClassWizard generated virtual function overrides
            //{{AFX_VIRTUAL(ComboBoxEx)
            protected:
            virtual void PreSubclassWindow();
            virtual LRESULT WindowProc( UINT uMsg_i, WPARAM wParam_i, LPARAM lParam_i );
            //}}AFX_VIRTUAL

        protected:

            //{{AFX_MSG(ComboBoxEx)
            afx_msg void OnSize( UINT uType_i, int nWidth_i, int nHeight_i );
            afx_msg void OnPaint();
            afx_msg HBRUSH OnCtlColor( CDC* pDC_i, CWnd* pWnd_i, UINT nCtlColor_i );
            afx_msg void OnLButtonDown( UINT uFlags_i, CPoint point_i );
            afx_msg void OnLButtonDblClk( UINT uFlags_i, CPoint point_i );
            afx_msg void OnEditChange();
            afx_msg void OnSelChange();
            afx_msg void OnSetFocus( CWnd* pOldWnd_i );
            afx_msg void OnKillFocus( CWnd* pNewWnd_i );
            afx_msg void OnEnable( BOOL bEnable_i );
            //}}AFX_MSG
            DECLARE_MESSAGE_MAP()

        public:

            // Function to create thecombo box
            BOOL Create( DWORD dwStyle_i, const RECT& Rect_i, CWnd* pParentWnd_i, UINT uID_i );
            // Functions to set colors. If redraw flag is true the control will be updated.
            void SetBorderColor( COLORREF clrBorder_i, bool bRedraw_i = true );
            void SetFocusBorderColor( COLORREF clrBorder_i, bool bRedraw_i = true );
            void SetListBorderColor( COLORREF clrBorder_i );
            void SetTextColor( COLORREF clrText_i, bool bRedraw_i = true );
            void SetBkColor( COLORREF clrBk_i, bool bRedraw_i = true );
            void SetSelectedTextColor( COLORREF clrSelText_i, bool bRedraw_i = true );
            void SetSelectedBkColor( COLORREF clrSelBk_i, bool bRedraw_i = true );
            void SetDisabledBorderColor( COLORREF clrBorder_i, bool bRedraw_i = true );
            void SetDisabledTextColor( COLORREF clrText_i, bool bRedraw_i = true );
            void SetDisabledBkColor( COLORREF clrBk_i, bool bRedraw_i = true );

            /* Begin: Added 2.5 */
            bool SetMultiLine();
            bool GetMultiLine();
            /* End: Added 2.5 */

            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            // Function to set mouse over color
            void SetMouseOverColor( COLORREF clrBorder_i );
            /**
             * End of Change for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */

            // Function to set the font.
            /**
             * Tookout for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            // bool SetFont( LPCTSTR lpctszFontName_i, int nPointSize_i );
            /**
             * End of Change for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */

            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            bool SetFont( LPCTSTR lpctszFontName_i, int nPointSize_i, bool bBold_i = false );
            /**
             * End of Change for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */

            // Function to set the character list that must be filtered from displaying.
            void SetEditFilterChars( LPCTSTR lpctszChars_i );
            // Function to enable/disable copy-paste.
            void EnableCopyPaste( bool bEnable_i = true );
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

            // Hover effect
            void SetHoverEffect( int nApplyTo_i, bool bEnable_i );
            bool GetHoverEffect( int nApplyTo_i );
            // Function to set image to each item
            void SetItemImage( int nItem_i, int nImage_i );
            // Set the corresponding image list holding images
            void SetImageList( CImageList* pImageList_i, int nImageListType_i );

            // Return the image corresponding to the specified item
            int GetItemImage( int nItem_i );

        private:

           /**
            * Modified for MPlus V4
            * @since 2.8
            */
            ComboBoxEx( const ComboBoxEx& ComboBoxExObj_i );
            ComboBoxEx& operator=( const ComboBoxEx& ComboBoxEx_i );
           /**
            * End of Modification for MPlus V4
            */

            /* Begin: Took Out 2.3 */
//             // Static window proc hook function to handle the messages of edit box in the combo box.
//             static  LRESULT CALLBACK EditWndProcHook( HWND hWnd_i, UINT uMsg_i, WPARAM wParam_i, LPARAM lParam_i );
            /* End:  Took Out 2.3 */
             // User message handlers.
            // Function to handle ECBIM_INITIALIZE message.
            afx_msg LRESULT OnECBIMInitialize( WPARAM wParam_i, LPARAM lParam_i );
            // Function to handle ECBIM_DROPDOWN message.
            afx_msg LRESULT OnECBIMDropDown( WPARAM wParam_i, LPARAM lParam_i );
            // Function to handle ECBIM_DROPDOWNCLOSE message.
            afx_msg LRESULT OnECBIMDropDownClose( WPARAM wSetSelected_i, LPARAM lParam_i );
            // Function to handle ECBIM_SELECTITEM message.
            afx_msg LRESULT OnECBIMSelectItem( WPARAM wParam_i, LPARAM lParam_i );
            // Function to handle ECBIM_MOUSEWHEEL message.
            afx_msg LRESULT OnECBIMMouseWheel( WPARAM wParam_i, LPARAM lParam_i );
            // Function to handle ECBIM_KEYDOWN message.
            afx_msg LRESULT OnECBIMKeyDown( WPARAM wParam_i, LPARAM lParam_i );
            // Function to handle ECBIM_CHAR message.
            afx_msg LRESULT OnECBIMChar( WPARAM wParam_i, LPARAM lParam_i );
            // Function to handle ECBIM_SETTEXT message.
            afx_msg LRESULT OnECBIMSetText( WPARAM wParam_i, LPARAM lParam_i );
            // Function to handle ECBIM_CANCOPYPASTE message.
            afx_msg LRESULT OnECBIMCanCopyPaste( WPARAM wParam_i, LPARAM lParam_i );
            // Function to handle ECBIM_FILTER message.
            afx_msg LRESULT OnECBIMFilter( WPARAM wParam_i, LPARAM lParam_i );
            // Function to handle ECBIM_GETDLGCODE message.
            afx_msg LRESULT OnECBIMGetDlgCode( WPARAM wParam_i, LPARAM lParam_i );

            // Function to handle ECBIM_LBUTTONUP message.
            afx_msg LRESULT OnECBIMLButtonUp( WPARAM wParam_i, LPARAM lParam_i );

            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            // Function to handle ECBIM_MOUSEMOVE message.
            afx_msg LRESULT OnECBIMMouseMove( WPARAM wParam_i, LPARAM lParam_i );
            // Function to handle ECBIM_MOUSELEAVE message.
            afx_msg LRESULT OnECBIMMouseLeave( WPARAM wParam_i, LPARAM lParam_i );
            // Function to handle ECBIM_SYSKEYDOWN message.
            afx_msg LRESULT OnECBIMSysKeyDown( WPARAM wParam_i, LPARAM lParam_i );
            /**
             * End of Change for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */

            // Function to handle CB_SHOWDROPDOWN message.
            afx_msg LRESULT OnCBShowDropDown( WPARAM wParam_i, LPARAM lParam_i );
            // Function to handle CB_SELECTSTRING message.
            afx_msg LRESULT OnCBSelectString( WPARAM wParam_i, LPARAM lParam_i );
            // Function to handle CB_SETCURSEL message.
            afx_msg LRESULT OnCBSetCurSel( WPARAM wParam_i, LPARAM lParam_i );
            // Function to handle the child notifications.
            BOOL OnChildNotify( UINT message_i, WPARAM wParam_i, LPARAM lParam_i, LRESULT* pLResult_o );
            // Function to filter the string.
            void Filter( CString& csStr_io );
            // Function to handle sizing of the control.
            void SizeWindow( int nWidth_i, int nHeight_i );

        private:

            /* Begin: Took Out 2.3 */
//             // This CMap object is used to store the default window procs
//             // of edit boxes displaced by the EditWndProcHook.
//             static CMap< HWND, HWND, LONG, LONG > m_WndProcMap;
            /* End:  Took Out 2.3 */
            DWORD    m_dwStyle;     // Style of the combo box.
            CRect    m_DropBtnRect; // Rectangular position of the drop button.
            HWND     m_hWndItem;    // Handle of edit or static text.
            

            /* Begin: Modification 2.6 #BugId 575*/
            /*DropWnd  m_DropWnd;     // Drop down window.*/            
            //bool     m_bDropDown;   // Drop down window status.
            DropWnd*  m_pDropWnd;     // Drop down window.
            CMap<int, int, int, int> m_ImageMap; // Image to Id mapping
            CImageList* m_pImageList; // Holds the image list
             /* Begin: Modification 2.7 BugId #575*/
            // CRect m_RectList; // Rect of the drop down list
             /* Begin: Modification 2.7 BugId #575*/

            /* End: Modification 2.6 #BugId 575*/
            
            ComboStaticCtrl m_ComboStatic;  // Static text window for combo with Drop List style.
            ComboDropBtn    m_ComboDropBtn; // Drop button.
            
            // Custom colors of the combo box.
            COLORREF m_clrBorder;
            COLORREF m_clrBorderFocus;
            COLORREF m_clrListBorder;
            COLORREF m_clrText;
            COLORREF m_clrBk;
            COLORREF m_clrSelectedText;
            COLORREF m_clrSelectedBk;
            COLORREF m_clrDisabledBorder;
            COLORREF m_clrDisabledText;
            COLORREF m_clrDisabledBk;
            /* Begin: Added 2.4 */
            CBrush m_BkBrush; // Background brush
            /* End: Added 2.4 */

            int      m_nDropButtonWidth;  // Width of the drop button.
            bool     m_bInitialized;      // Flag that specifies the member are initialized
            CString  m_csFilterChars;     // Edit filter characters.
            bool     m_bCopyPasteEnabled; // Flag that determines copy-paste is enabled or not

            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            COLORREF m_clrMouseOver; // Mouse over color
            bool m_bMouseInside; // Mouse over flag for combo
            // bool m_bMouseEditInside; // Mouse over flag for edit
            /**
             * End of Change for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */

            bool m_bHoverEffect;      // Hover effect flag for main control
            bool m_bChildHoverEffect; // Hover effect flag for child control
            bool m_bDropBtnPressed; // Drop button state flag
            bool m_bDropBtnActive;  // Flag that indicate whether the drop button is in pressed state.
            bool m_bMouseInsideDropBtn; // Flag that indicate whether the cursor is inside the drop button.

            int m_nCharPos; // Gives caret position after a paste event

            /* Begin: Added 2.3 */
            ComboEdit m_ComboEdit;// 
            /* End: Added 2.3 */
            /* Begin: Added 2.5 */
            ComboEditEx m_MultiLineEdit; // Holds multi line edit
            bool m_bMultiline; // Set multiline combo
            
            /* Begin: Modification 2.6 */
//             CString m_csFontName; // Holds font
//             int m_nFontSize; // Holds font size
//             bool m_bBold; // Holds flag for boldness
            /* End: Modification 2.6 */

            /* End: Added 2.5 */
            /* Begin: Added 2.9 for Bug ID MPC05905 */
            bool m_bSelChange;
            /* End: Added 2.9 for Bug ID MPC05905 */
        };
    } // End of namespace UICommon
} // End of namespace MPlus

#endif // #ifndef _COMBOBOXEX_H_

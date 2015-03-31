/**
 * Copyright(c) 2006-2009 TOSHIBA Medical Systems Corporation, All Rights
 * Reserved.
 *
 * DropWnd.h - DropWnd class declaration file.
 *
 * @author :    Nishad S
 * @version:    1.0            Date:  2006-03-14
 * @author :    Shypa Padmanabhan     (Modified for MPlusV4)
 * @version:    1.1            Date:  2009-01-09
 */

#ifndef _DROPWND_H_
#define _DROPWND_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "MPlusUICommon/ComboListCtrl.h"

namespace MPlus
{
    namespace UICommon
    {
        class DropWnd : public CWnd
        {
        public:

            // Constructor.
            DropWnd();
            // Destructor.
            virtual ~DropWnd();

            // ClassWizard generated virtual function overrides
            //{{AFX_VIRTUAL(DropWnd)
            virtual void PreSubclassWindow();
            //}}AFX_VIRTUAL


        protected:
            //{{AFX_MSG(DropWnd)
            afx_msg void OnLButtonDown( UINT uFlags_i, CPoint point_i );
            afx_msg void OnLButtonUp( UINT uFlags_i, CPoint point_i );
            afx_msg void OnLButtonDblClk( UINT uFlags_i, CPoint point_i );
            afx_msg void OnMouseMove( UINT uFlags_i, CPoint point_i );
            //}}AFX_MSG
            DECLARE_MESSAGE_MAP()

        public:

            // Forward declaration of friend class.
            friend class ComboBoxEx;

        public:

            // Function to create the window.
            bool Create( ComboBoxEx* pWndParent_i, const CRect& rect_i );
            // Function to show/hide the window.
            void ShowDropDown( bool bShow_i = true );
            // Set the data window (list box of the combo box) of the list control.
            void SetDropDownList( HWND hWndDropDown_i )
            {
                m_ListCtrl.SetDropDownList( hWndDropDown_i );
            }
            // Function to handle UM_SELECTITEM message.
            afx_msg LRESULT OnECBIMSelectItem( WPARAM wParam_i, LPARAM lParam_i );

       /**
        * Modified for MPlus V4
        * @since 1.1
        */
        private:
            DropWnd( const DropWnd& DropWnd_i );
            DropWnd& operator=( const DropWnd& DropWnd_i );
       /**
        * End of Modification for MPlus V4
        */

        private:

            CWnd* m_pWndParent;       // Pointer to the parent window.
            ComboListCtrl m_ListCtrl; // List ctrl that shows the item list.
            int   m_nSetHeight;       // Initial height of the list box.
        };
    } // End of namespace UICommon
} // End of namespace MPlus

#endif // #ifndef _DROPWND_H_

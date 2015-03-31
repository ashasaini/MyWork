 /**
 * Copyright(c) 2006 - 2010 TOSHIBA Medical Systems Corporation, All Rights
 * Reserved.
 *
 * ComboListCtrl.h - ComboListCtrl class declaration file.
 *
 * @author :    Nishad S
 * @version:    1.0            Date:  2006-03-07
 * @author :    Jijeesh K V
 * @version:    2.0            Date:  2007-04-27
 * @author :    Nishad S
 * @version:    2.1            Date:  2007-07-23
 * @author :    Naveen
 * @version:    2.2            Date:  2008-09-05
 * @author :    Naveen R
 * @version:    2.3            Date:  2008-12-09
 * @author :    Naveen R
 * @version:    2.4            Date:  2008-12-16
 * @author :    Shypa Padmanabhan     (Modified for MPlusV4)
 * @version:    2.5            Date:  2009-01-09
 * @author :    Jinesh P.P.  ( Modified for MPlusV4 BugFix Merging #MPC05401 )
 * @version:    2.6            Date:  2010-03-23
 */

#ifndef _COMBOLISTCTRL_H_
#define _COMBOLISTCTRL_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "MPlusUICommon/ListCtrlEx.h"
/* Begin: Modification 2.4 #702*/
#include "MPlusUICommon/ComboStaticCtrl.h"
/* End: Modification 2.4 #702*/

namespace MPlus
{
    namespace UICommon
    {
        // User messages (ECBIM - Extended Combo Box Internal Message).
        static const int ECBIM_DROPDOWN      = WM_USER + 2;
        static const int ECBIM_DROPDOWNCLOSE = WM_USER + 3;
        static const int ECBIM_SELECTITEM    = WM_USER + 4;
        static const int ECBIM_MOUSEWHEEL    = WM_USER + 5;
        static const int ECBIM_KEYDOWN       = WM_USER + 6;
        static const int ECBIM_CHAR          = WM_USER + 7;
        static const int ECBIM_SETTEXT       = WM_USER + 8;
        static const int ECBIM_CANCOPYPASTE  = WM_USER + 9;
        static const int ECBIM_FILTER        = WM_USER + 10;
        static const int ECBIM_GETDLGCODE    = WM_USER + 11;

        /**
         * Added for Celeve PIMS Phase-5 requirement
         * @since  2.0
         */
        static const int ECBIM_MOUSEMOVE     = WM_USER + 12;
        static const int ECBIM_MOUSELEAVE    = WM_USER + 13;
        static const int ECBIM_SYSKEYDOWN    = WM_USER + 14;
        /**
         * End of Change for Celeve PIMS Phase-5 requirement
         * @since  2.0
         */
        static const int ECBIM_LBUTTONUP     = WM_USER + 15;
        /* Begin: Modification 2.2 #BugId 575*/
        static const int ECBIM_CLOSE         = WM_USER + 16;
        static const int ECBIM_GET_IMGLIST   = WM_USER + 17;
        /* End: Modification 2.2 #BugId 575*/
        
        class ComboBoxEx;
        /**
         * Copyright(c) 2006 - 2007 TOSHIBA Medical Systems Corporation, All Rights
         * Reserved.
         *
         * ComboListCtrl - This class is derived from ListCtrlEx to show the data
         *                 in the list box of combo box in customized view.
         *
         * Traceability Tags : TCOM_MP_CS_UI_COMBOLISTCTRL_HANDLE_MOUSEMOVE
         *                     TCOM_MP_CS_UI_COMBOLISTCTRL_HANDLE_MOUSELEAVE
         * @author :    Nishad S
         * @version:    1.0            Date:  2006-03-07
         * @author :    Jijeesh K V
         * @version:    2.0            Date:  2007-04-27
         */
        class ComboListCtrl : public ListCtrlEx
        {
        public:

            // Constructor.
            ComboListCtrl();
            // Destructor.
            virtual ~ComboListCtrl();

            // ClassWizard generated virtual function overrides
            //{{AFX_VIRTUAL(ComboListCtrl)
            virtual void PreSubclassWindow();
            //}}AFX_VIRTUAL

        protected:

            //{{AFX_MSG(ComboListCtrl)
            afx_msg void OnGetDispInfo( NMHDR* pstNMHDR_i, LRESULT* pResult_o );
            /* Begin: Modification 2.3 #683 */
            // afx_msg void OnSize( UINT uType_i, int nWidth_i, int nHeight_i );
            /* End: Modification 2.3 #683 */
            /* Begin: Modification 2.4 #735, #702*/
            afx_msg void OnNcCalcSize( BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp );
            afx_msg BOOL OnMouseWheel( UINT uFlags_i, short sDelta_i, CPoint point_i );
            /* End: Modification 2.4 #735, #702*/
            //}}AFX_MSG

            DECLARE_MESSAGE_MAP()

        public:

            // Function to set the item count from the list box of the combo box.
            int SetCountFromComboListBox();
            // Function to initialize the list ctrl before it is shown.
            void Init();
            // Function to set the drop down list box handle of the combo box.
            // The data that are shown in the list ctrl is taken from this list box.
            void SetDropDownList( HWND hWndDropDown_i )
            {
                m_hWndDropDownList = hWndDropDown_i;
            }

            // Function to handle virtual LButtonDown event.
            void LButtonDown( const CPoint& point_i );
            // Function to handle virtual LButtonUp event.
            void LButtonUp( const CPoint& point_i );
            // Function to handle virtual MouseMove event.
            void MouseMove( const CPoint& point_i );

            /* Begin: Modification 2.2 #BugId 575*/
            // Set the image map
//             void SetItemImageMap( int nItem_i, int nImage_i );
//            int GetItemImageIndex( int nItem_i );
            // Function to updated the scroll bars in list ctrl.
            void UpdateScrollBars();
            // Function to set the combobox pointer to the list ctrl
            void SetCombo( ComboBoxEx* pCombo_i );
            /* End: Modification 2.2 #BugId 575*/
            /* Begin: Modification 2.4 #735*/
            virtual void SetRgn( const CRect* pClientRect_i = 0, bool bHScroll_i = false, bool bVScroll_i = false );
            /* End: Modification 2.4 #735*/
            /* Begin: Modification 2.6 for Bug ID #MPC05401 */
            virtual void GetClientRect( LPRECT lpRect_o );
            /* End: Modification 2.6 for Bug ID #MPC05401 */

        private:

            /**
             * Modified for MPlus V4
             * @since 2.5
             */
            ComboListCtrl( const ComboListCtrl& ComboListCtrl_i );
            ComboListCtrl& operator=( const ComboListCtrl& ComboListCtrl_i );
           /**
            * End of Modification for MPlus V4
            */

            // Function to select an item at the specified point.
            void SelectItem( const CPoint& point_i );

        private:

            // Flag to identify whether the click is on list
            bool m_bLbuttonDown;
            HWND     m_hWndDropDownList; // Handle of the drop down list box of the combo box.
            
            /* Begin: Modification 2.2 #BugId 575*/
            ComboBoxEx* m_pCombo;
             //CMap<int, int, int, int> m_ImageMap; // Image to Id mapping
            /* End: Modification 2.2 #BugId 575*/
            /* Begin: Modification 2.4 #702 - ComboStaticCtrl is used as InlineToolTip control*/
            ComboStaticCtrl m_StaticTipCtrl;    // For displaying lengthy items.
            /* End: Modification 2.4 #702*/
        };
    } // End of namespace UICommon
} // End of namespace MPlus

#endif // #ifndef _COMBOLISTCTRL_H_

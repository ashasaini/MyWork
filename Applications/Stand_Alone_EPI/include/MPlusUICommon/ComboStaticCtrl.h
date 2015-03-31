/**
 * Copyright(c) 2006 - 2009 TOSHIBA Medical Systems Corporation, All Rights
 * Reserved.
 *
 * ComboStaticCtrl.h - ComboStaticCtrl class declaration file.
 *
 * @author :    Nishad S
 * @version:    1.0            Date:  2006-03-14
 *
 * @author :    Jijeesh K V
 * @version:    2.0            Date:  2007-04-27
 *
 * @author :    Naveen. R
 * @version:    2.1            Date:  2008-08-27
 * @author :    Naveen. R
 * @version:    2.2            Date:  2008-09-19
 * @author :    Naveen. R
 * @version:    2.3            Date:  2008-12-17
 * @author :    Shypa Padmanabhan     (Modified for MPlusV4)
 * @version:    2.4            Date:  2009-01-09
 */

#ifndef _COMBOSTATICCTRL_H_
#define _COMBOSTATICCTRL_H_

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
namespace MPlus
{
    namespace UICommon
    {
        /**
         * Copyright(c) 2006 - 2009 TOSHIBA Medical Systems Corporation, All Rights
         * Reserved.
         *
         * ComboStaticCtrl - This class is to show the text in the static area of the combo box
         *                   with Drop List style. The default text is hidden by this control.
         *
         * Traceability Tags : TCOM_MP_CS_UI_COMBOSTATICCTRL_SET_COLOR
         *                               TCOM_MP_CS_UI_COMBOSTATICCTRL_HANDLE_MOUSEOVER_EFFECT
         *
         * @author :    Nishad S
         * @version:    1.0            Date:  2006-03-14
         * @author :    Jijeesh K V
         * @version:    2.0            Date:  2007-04-27
         */
        class ComboStaticCtrl : public CStatic
        {
        public:

            // Constructor.
            ComboStaticCtrl();
            // Destructor.
            virtual ~ComboStaticCtrl();

            // ClassWizard generated virtual function overrides
            //{{AFX_VIRTUAL(ComboStaticCtrl)
            virtual void PreSubclassWindow();
            //}}AFX_VIRTUAL

        protected:

            //{{AFX_MSG(ComboStaticCtrl)
           /**
            * Modified for MPlus V4
            * @since 2.4
            */
            // afx_msg UINT OnNcHitTest( CPoint point_i );
            afx_msg LRESULT OnNcHitTest( CPoint point_i );
           /**
            * End of Modification for MPlus V4
            */
            afx_msg HBRUSH CtlColor( CDC* pDC_i, UINT nCtlColor_i );
            afx_msg void OnPaint();
            afx_msg void OnNcCalcSize( BOOL bCalcValidRects_i, NCCALCSIZE_PARAMS FAR* lpncsp_i );
            afx_msg void OnNcPaint();
            //}}AFX_MSG

            DECLARE_MESSAGE_MAP()

        public:

            // Color setting functions.
            void SetTextColor( COLORREF clrText_i );
            void SetBkColor( COLORREF clrBk_i );
            void SetFocusBkColor( COLORREF clrFocusBk_i ) { m_clrFocusBk = clrFocusBk_i; }
            // Set/reset the  focus flag.
            void SetFocusState( bool bFocus_i ) { m_bFocus = bFocus_i; }

            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            // Function to set the mouse over rect
            void SetMouseOverRect( bool bMouseInside_i );
            // Function to set mouse over color
            void SetMouseOverColor( COLORREF clrMouseOver_i );
            /**
             * End of Change for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            /* Begin: Modification 2.1 BugId #575*/
//             // Function to set the imgae list
//             void SetImageList( CImageList* pImageList_i );
            /* End: Modification 2.1 BugId #575*/
            
            // Function to set image correpondingly to the static control
            void SetImage( int nImage_i );
            /* Begin: Modification 2.3 #702*/
            void SetInlineToolTip( bool bEnable_i );
            /* End: Modification 2.3 #702*/

        private:

            COLORREF m_clrText; // Text color.
            COLORREF m_clrBk;   // Background color.
            COLORREF m_clrFocusBk; // Background color.
            /* Begin: Modification 2.2 BugId #575*/
            // CBrush   m_BkBrush; // Background brush.
            /* End: Modification 2.2 BugId #575*/

            bool     m_bFocus;  // Focus indication flag.

            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            COLORREF m_clrMouseOverBorder; // Mouse over color
            bool m_bMouseInside; // Mouse over flag
            /**
             * End of Change for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            
            /* Begin: Modification 2.1 BugId #575*/
            //CImageList* m_pImageList; // Holds image list
            /* Begin: Modification 2.1 BugId #575*/

            int m_nImage; // Holds the corresponding inmage index
            int m_nIconSpace; // Holds icon space
            /* Begin: Modification 2.3 #702*/
            bool m_bInlineToolTip;  // If it is true control will be acting as InlineToolTip.
            /* End: Modification 2.3 #702*/
        };
    } // End of namespace UICommon
} // End of namespace MPlus

#endif // #ifndef _COMBOSTATICCTRL_H_

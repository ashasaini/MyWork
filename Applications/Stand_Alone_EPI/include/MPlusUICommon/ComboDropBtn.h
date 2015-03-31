/**
 * Copyright(c) 2006 - 2010 TOSHIBA Medical Systems Corporation, All Rights
 * Reserved.
 *
 * ComboDropBtn.h - ComboDropBtn class declaration file.
 *
 * @author :    Nishad S
 * @version:    1.0            Date:  2006-05-30
 * @author :    Jijeesh K V
 * @version:    2.0            Date:  2007-04-27
 * @author :    Shypa Padmanabhan     (Modified for MPlusV4)
 * @version:    2.1            Date:  2009-01-09
 * @author :    Jinesh P.P.  ( Modified for MPlusV4 BugFix Merging #MPC05901(Performance Improvement))
 * @version:    2.2            Date:  2010-02-15
 */

#ifndef _COMBODROPBTN_H_
#define _COMBODROPBTN_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "MPlusUICommon/ScrollBarEx.h"

namespace MPlus
{
    namespace UICommon
    {
        /**
         * Copyright(c) 2006 - 2009 TOSHIBA Medical Systems Corporation, All Rights
         * Reserved.
         *
         * ComboDropBtn - This class is to implement the drop button of combo box.
         *
         * Traceability Tags : TCOM_MP_CS_UI_COMBODROPBTN_SET_COLORS
         *                     TCOM_MP_CS_UI_COMBODROPBTN_MOUSE_OVER_EFFECT
         *
         * @author :    Nishad S
         * @version:    1.0            Date:  2006-05-30
         * @author :    Jijeesh K V
         * @version:    2.0            Date:  2007-04-27
         */
        class ComboDropBtn : public CStatic
        {
        public:

            // Constructor.
            ComboDropBtn();
            // Destructor.
            virtual ~ComboDropBtn();

            // ClassWizard generated virtual function overrides
            //{{AFX_VIRTUAL(ComboDropBtn)
            virtual void PreSubclassWindow();
            //}}AFX_VIRTUAL

        protected:

            //{{AFX_MSG(ComboDropBtn)
           /**
            * Modified for MPlus V4
            * @since 2.1
            */
            /// Modified for correcting the error "'static_cast' :cannot convert from 'UINT (__thiscall 
            /// MPlus::UICommon::ComboDropBtn::* )(CPoint)' to 'LRESULT (__thiscall CWnd::* )(CPoint)'
            // afx_msg UINT OnNcHitTest( CPoint point_i );
            afx_msg LRESULT OnNcHitTest( CPoint point_i );
           /**
            * End of Modification for MPlus V4
            */
            afx_msg void OnPaint();
            //}}AFX_MSG

            DECLARE_MESSAGE_MAP()

        public:

            // Function to set the pressed state.
            void SetPressedState( bool bPressed_i );
            // Function to set the enabled state.
            void SetEnabledState( bool bEnabled_i );

            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            // Function to set the mouse over state to button
            void SetMouseOverState( bool bMouseInside_i );
            void SetMouseOverColor( COLORREF clrMouseOver_i );
            /**
             * End of Change for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            /* Begin: Modification 2.2 for Bug ID #MPC05901(Performance Improvement) */
            void SetDropButtonGdiFlags( bool bUseGDIOnly_i );
            /* End: Modification 2.2 for Bug ID #MPC05901(Performance Improvement) */

        private:

            bool     m_bPressed; // Pressed state flag.
            bool     m_bEnabled; // Enabled state flag.

            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            bool m_bMouseInside; // Holds mouse over status
            // To draw the button using scroll bar button
            ScrollBarGraphics m_Graph;
            COLORREF m_clrMouseOver; // Mouse over color
            /**
             * End of Change for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            /* Begin: Modification 2.2 for Bug ID #MPC05901(Performance Improvement) */
            bool m_bUseGDIOnly;
            /* End: Modification 2.2 for Bug ID #MPC05901(Performance Improvement) */
        };
    } // End of namespace UICommon
} // End of namespace MPlus

#endif // #ifndef _COMBODROPBTN_H_

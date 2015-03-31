/**
 * Copyright(c) 2006-2010 TOSHIBA Medical Systems Corporation, All Rights
 * Reserved.
 *
 * SpinEx.h - SpinEx class declaration file. This class is derived from CSpinButtonCtrl
 *
 * @author :    Krishnakumar T G
 * @version:    1.0            Date:  2006-03-09
 * @author :    Nishad S
 * @version:    2.1            Date:  2007-06-21
 * @author :    Jijeesh K V
 * @version:    2.2            Date:  2008-05-27
 * @author :    Naveen R
 * @version:    2.3            Date:  2008-08-07
 * @author :    Naveen R
 * @version:    2.4            Date:  2008-12-19
 * @author :    Shypa Padmanabhan     (Modified for MPlusV4)
 * @version:    2.5            Date:  2009-01-09
 * @author :    Jinesh P.P.  ( Modified for MPlusV4 BugFix Merging #MPC05900(Performance Improvement))
 * @version:    2.6           Date:  2010-02-18
 */

#ifndef _SPINEX_H_
#define _SPINEX_H_

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

#include "MPlusUICommon/ButtonEx.h"

namespace MPlus
{
    namespace UICommon
    {
        /**
         * Copyright(c) 2006-2009 TOSHIBA Medical Systems Corporation, All Rights
         * Reserved.
         *
         * SPIN_COLORS_t - Holds the spin drawing colors
         *
         * @author :    Krishnakumar T G
         * @version:    1.0            Date:  2006-03-09
         */
        typedef struct SPIN_COLORS_tag SPIN_COLORS_t;
        struct SPIN_COLORS_tag 
        {
            COLORREF clrDisabled;
            COLORREF clrDisabledBorder;
            COLORREF clrDisabledArrow;
            COLORREF clrBorder;
            COLORREF clrMouseHover;
            COLORREF clrDfltFaceStart;
            COLORREF clrDfltFaceEnd;
            COLORREF clrPressFaceStart;
            COLORREF clrPressFaceEnd;
            COLORREF clrArrow;
        };

        /**
         * Copyright(c) 2006 TOSHIBA Medical Systems Corporation, All Rights
         * Reserved.
         *
         * SpinEx - SpinEx class definition. This class is derived from CSpinButtonCtrl
         *
         * @author :    Krishnakumar T G
         * @version:    1.0            Date:  2006-03-09
         */
        class CONTROLS_CLASS_API SpinEx : public CSpinButtonCtrl
        {

        public:

            // Constructor
            SpinEx();
            // Destructor
            virtual ~SpinEx();

        public:

            // ClassWizard generated virtual function overrides
            //{{AFX_VIRTUAL(SpinEx)
            protected:
            virtual void PreSubclassWindow();
            virtual LRESULT WindowProc( UINT Message_i, WPARAM wParam_i, LPARAM lParam_i );
            //}}AFX_VIRTUAL

        public:

            // To create the spin control dynamically
            BOOL Create( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID );
            // To get the spin control drawing colors
            void GetSpinColors( SPIN_COLORS_t& stColors_i );
            // To set the spin control drawing colors
            void SetSpinColors( SPIN_COLORS_t& stColors_i );
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
            void EnableHoverEffect( bool bEnable_i = true )
            {
                m_bHoverEffect = bEnable_i;
            }

        protected:

            //{{AFX_MSG(SpinEx)
            afx_msg void OnPaint();
            afx_msg void OnLButtonDown( UINT nFlags_i, CPoint Point_i );
            afx_msg void OnLButtonUp( UINT nFlags_i, CPoint Point_i );
            afx_msg BOOL OnEraseBkgnd( CDC* pDC_i );
           /**
            * Modified for MPlus V4
            * @since 2.5
            */
            /// Modified for correcting the error "'static_cast' :cannot convert from 'UINT (__thiscall 
            /// MPlus::UICommon::SpinEx::* )(CPoint)' to 'LRESULT (__thiscall CWnd::* )(CPoint)'"
            // afx_msg UINT OnNcHitTest( CPoint Point_i );
            afx_msg LRESULT OnNcHitTest( CPoint Point_i );
           /**
            * End of Modification for MPlus V4
            */
            afx_msg void OnEnable( BOOL bEnable_i );
            afx_msg void OnNcDestroy();
            /* Begin: Modification 2.4 #711*/
            afx_msg void OnCaptureChanged( CWnd* pWnd );
            /* End: Modification 2.4 #711*/
            //}}AFX_MSG
            afx_msg void OnStyleChanged( int nStyleType_i, LPSTYLESTRUCT pstStyleStruct_i );

            DECLARE_MESSAGE_MAP()

        private:

           /**
            * Modified for MPlus V4
            * @since 2.5
            */
            SpinEx( const SpinEx& SpinEx_i );
            SpinEx& operator=( const SpinEx& SpinEx_i );
           /**
            * End of Modification for MPlus V4
            */
            // To track the mouse enter/leave
            void TrackMouseMovement( bool bMouseOver_i );
            // To draw the arrow inside the button.
            void DrawArrow( CDC& dc_i, const CRect &rect_i, bool bRightDown_i, COLORREF clr_i );
            /* Begin: Modification 2.6 for Bug ID #MPC05900(Performance Improvement) */
            // void DrawButton( HDC hDC_i, const CRect& rect_i, BUTTON_COLORS_t& stColors_i,
            //                 bool bMouseOver_i, ButtonEx::STATE_e eState_i );
            void DrawButton( CDC& dc_i, const CRect& rect_i, BUTTON_COLORS_t& stColors_i,
                             bool bMouseOver_i, ButtonEx::STATE_e eState_i );
            /* End: Modification 2.6 for Bug ID #MPC05900(Performance Improvement) */
            // Function to initialize the spin control.
            void InitBuddy();
            // Static window proc hook function to handle the messages of buddy edit box.
            static LRESULT CALLBACK EditWndSubClassProc( HWND hWnd_i, UINT uMsg_i, WPARAM wParam_i, 
                                LPARAM lParam_i, UINT_PTR uIdSubclass_i, DWORD_PTR dwRefData_i );

        private:

            // Holds the handle of buddy edit box.
            HWND m_hWndBuddy;
            // Object to initialize the gdi plus
            GdiPlusCtrl m_GdiInit;
            // Holds button drawing colors
            BUTTON_COLORS_t m_stButtonColors;
            /* Begin: Modification 2.3 . Unused Memeber variable*/
//             // Holds button parameters ( Font, Text, etc...)
//            BUTTON_PARAMS_t m_stButtonParams;
            /* End: Modification 2.3 */
            
            // Holds the Up button rectangle
            CRect m_UpRect;
            // Holds the down button rectangle
            CRect m_DownRect;
            // Holds the spin button direction
            bool m_bHorizontal;
            // Holds Left button pressed flag
            bool m_bLeftPressed;
            // Holds Right button pressed flag
            bool m_bRightPressed;
            // Holds Left button hover flag
            bool m_bLMouseHover;
            // Holds Right button hover flag
            bool m_bRMouseHover;
            // Holds the image list
            /* Begin: Took out 2.2 */
//             CImageList m_ImageList;
            /* End: Took out 2.2 */
            // Holds the flag that window is enable/disable
            bool m_bDisableFlag;
            // Hover effect flag
            bool m_bHoverEffect;
            // Mouse tarck flag
            bool m_bMouseInside;
            // Flag for buddy
            bool m_bBuddy;
            // Buddy edit control
            class EditCtrlEx* m_pEditEx;
            // Flag that specifies the member are initialized
            bool m_bBuddyInitialized;
            /* Begin: Modification 2.6 for Bug ID #MPC05900(Performance Improvement) */
            bool m_bUseGDIOnly;
            /* End: Modification 2.6 for Bug ID #MPC05900(Performance Improvement) */
        };
    } // End of namespace UICommon
} // End of namespace MPlus

#endif // _SPINEX_H_

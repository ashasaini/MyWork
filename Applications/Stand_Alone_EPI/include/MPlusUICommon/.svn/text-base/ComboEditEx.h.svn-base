/**
 * Copyright(c) 2008 - 2010 TOSHIBA Medical Systems Corporation, All Rights
 * Reserved.
 *
 * ComboEditEx.h - Multiline edit class declaration corresponding to combo box
 *
 * @author :    Jijeesh K V
 * @version:    1.0            Date:  2008-06-25
 *
 * @author :    Naveen. R
 * @version:    1.1            Date:  2008-09-04
 * @author :    Rahul Kumar  ( Modified for MPlusV4 BugFix #MPC06665 )
 * @version:    1.2            Date:  2010-08-31
 */

#ifndef _COMBOEDITEX_H_
#define _COMBOEDITEX_H_

#include "MPlusUICommon/EditCtrlEx.h"
#include <afxcmn.h>

#ifndef _UNICODE
#error Unicode definition is necessary to use this class
#endif

namespace MPlus
{
    namespace UICommon
    {
        /**
         * Copyright(c) 2008 TOSHIBA Medical Systems Corporation, All Rights
         * Reserved.
         *
         * ComboEditEx.h - Multiline edit class declaration corresponding to combo box
         *
         * @author :    Jijeesh K V
         * @version:    1.0            Date:  2008-06-25
         */
        class ComboEditEx : public EditCtrlEx
        {
        // Construction
        public:

            ComboEditEx();
            
        // Operations
        public:

            // Overrides
            // ClassWizard generated virtual function overrides
            //{{AFX_VIRTUAL(ComboEditEx)

        public:
            //}}AFX_VIRTUAL

        // Implementation
        public:

            virtual ~ComboEditEx();
            void SetImage( int nImage_i );
            //Function for setting Image list
            void SetImageList( CImageList* pImageList_i );

        // Generated message map functions
        protected:

            //{{AFX_MSG(ComboEditEx)
            afx_msg void OnSetFocus( CWnd* pOldWnd_i );
            afx_msg void OnKeyDown( UINT nChar_i, UINT nRepCnt_i, UINT nFlags_i );
            afx_msg void OnChar( UINT nChar_i, UINT nRepCnt_i, UINT nFlags_i );
            afx_msg void OnEditFocus( NMHDR* pNMHDR_i, LRESULT* pResult_i );
            afx_msg void OnKillFocus( CWnd* pNewWnd_i );
            /* Begin: Modification 1.2 for Bug ID MPC06665 */
            //afx_msg void OnKeyUp( UINT nChar_i, UINT nRepCnt_i, UINT nFlags_i );
            /* End: Modification 1.2 for Bug ID MPC06665 */
            afx_msg void OnSysKeyDown( UINT nChar_i, UINT nRepCnt_i, UINT nFlags_i );
            afx_msg void OnNcCalcSize( BOOL bCalcValidRects_i, NCCALCSIZE_PARAMS FAR* lpncsp_o );
            afx_msg void OnNcPaint();
            //}}AFX_MSG

            DECLARE_MESSAGE_MAP()

        private:

            int m_nIconSpace; // Holds the icon width
            int m_nImage; // Holds the image
            
            /* Begin: Modification 1.1 BugId #575*/
            // CImageList* m_pImageList; // Holds the image list
            /* Begin: Modification 1.1 BugId #575*/
        };
    } // End of namespace UICommon
} // End of namespace MPlus
#endif // #ifndef _COMBOEDITEX_H_

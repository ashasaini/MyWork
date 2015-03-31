/**
 * Copyright(c) 2007-2009 TOSHIBA Medical Systems Corporation, All Rights
 * Reserved.
 *
 * ComboEdit.h - Declaration file for class ComboEdit.
 *
 * @author :    Junaij M
 * @version:    1.0            Date:  2008-05-14
 * @author :    Naveen.R
 * @version:    1.1            Date:  2008-07-10
 * @author :    Shypa Padmanabhan     (Modified for MPlusV4)
 * @version:    1.2            Date:  2009-01-09
 */

#ifndef _COMBOEDIT_H_
#define _COMBOEDIT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MPlusUICommon/ComboListCtrl.h"

namespace MPlus
{
    namespace UICommon
    {


        /* Begin: Modification 1.1 #578*/
        // Subclass function of edit control in combo
        LRESULT CALLBACK EditProc( HWND hWnd_i, UINT uMsg_i, WPARAM wParam_i, 
                                   LPARAM lParam_i, UINT_PTR uIdSubclass_i,
                                   DWORD_PTR dwRefData_i );
        /* End: Modification 1.1 #578*/

        /**
         * Copyright(c) 2007-2009 TOSHIBA Medical Systems Corporation, All Rights
         * Reserved.
         *
         * ComboEdit - Class for sub classing the drop down combo box.
         *
         * @author :    Junaij M
         * @version:    1.0            Date:  2008-05-14
         */
        /* Begin: Modification 1.1.for item #578 */
        class ComboEdit /*: public CEdit*/
        /* End: Modification 1.1 */
        {

        public:

            //Constructor
            ComboEdit();
            //Destructor
            virtual ~ComboEdit();
            //Fucntion for setting Image
            void SetImage( int nImage_i );
            //Function for setting Image list
            void SetImageList( CImageList* pImageList_i );

             /* Begin: Modification 1.1 #578*/
//        protected:
             /* Begin: Modification 1.1 #578*/

            //{{AFX_VIRTUAL(ComboEdit)
            virtual LRESULT WindowProc( UINT uMsg_i, WPARAM wParam_i, LPARAM lParam_i );
            //}}AFX_VIRTUAL

           /**
            * Modified for MPlus V4
            * @since 1.2
            */
            int GetIconSpace() const
            {
                return m_nIconSpace;
            }
            void SetIconSpace( int nIconSpace_i )
            {
                m_nIconSpace = nIconSpace_i;
            }
            int GetDropButtonLeft() const
            {
                return m_nDropButtonLeft;
            }
            void SetDropButtonLeft( int nDropBtnLeft_i )
            {
                m_nDropButtonLeft = nDropBtnLeft_i;
            }
            void GetPoint( CPoint& PointObj_o ) const
            {
                PointObj_o = m_point;
            }
            void SetPoint( const CPoint& PointObj_i )
            {
                m_point = PointObj_i;
            }
            int GetImage() const
            {
                return m_nImage;
            }
            HWND GetHwnd()
            {
                return m_hWnd;
            }
            void SetHwnd( HWND hWnd_i )
            {
                m_hWnd = hWnd_i;
            }

        private:
            ComboEdit( const ComboEdit& ComboEdit_i );
            ComboEdit& operator =( const ComboEdit& ComboEdit_i );

        //public:
        private:
            
            /* Begin: Modification 1.1 BugId #575*/
            // CImageList* m_pImageList; // Holds the image list
            /* Begin: Modification 1.1 BugId  #575*/
            int m_nIconSpace; // Holds the icon width
            int m_nDropButtonLeft; // Left co-ordinate of the Drop button
            CPoint m_point; // point to store the mouse click
            int m_nImage; // Holds the image

            /* Begin: Modification 1.1 */
            int m_nTopOffset;
            HWND m_hWnd;
            /* End: Modification 1.1 */
           /**
            * End of Modification for MPlus V4
            */
        };
    }
}

#endif // _COMBOEDIT_H_

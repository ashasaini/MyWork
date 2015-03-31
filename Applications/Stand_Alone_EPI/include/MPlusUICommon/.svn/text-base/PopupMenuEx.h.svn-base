/**
 * Copyright(c) 2007-2009 TOSHIBA Medical Systems Corporation, All Rights
 * Reserved.
 *
 * PopupMenuEx.h - Include file for the control PopupMenuEx
 *
 * @author :    Jijeesh K V
 * @version:    1.0            Date:  2007-04-19
 * @author :    Jijeesh K V
 * @version:    1.1            Date:  2008-05-07
 * @author :    Naveen R
 * @version:    1.2            Date:  2008-12-18
 * @author :    Shypa Padmanabhan     (Modified for MPlusV4)
 * @version:    1.3            Date:  2009-01-09
 * @author :    Asha (Modified for #MPC05207 - Context menu overflowing the screen)
 * @version:    1.4            Date:  2009-10-23
 */

#ifndef _POPUPMENUEX_H_
#define _POPUPMENUEX_H_

#include <afxtempl.h>
#include <afxcmn.h>

#ifdef UI_CONTROLS_DLL
#define CONTROLS_CLASS_API __declspec( dllexport )
#else
#define CONTROLS_CLASS_API __declspec( dllimport )
#endif

namespace MPlus
{
    namespace UICommon
    {
        /**
         * Copyright(c) 2007-2009 TOSHIBA Medical Systems Corporation, All Rights
         * Reserved.
         *
         * PopupMenuEx - This class customizes pop up menus
         *
         * Traceability Tags : TCOM_MP_CS_UI_POPUPMENUEX_SET_COLORS
         *                     TCOM_MP_CS_UI_POPUPMENUEX_SET_IMAGELIST
         *                     TCOM_MP_CS_UI_POPUPMENUEX_SET_IMAGE
         *                     TCOM_MP_CS_UI_POPUPMENUEX_SET_FONT
         *                     TCOM_MP_CS_UI_POPUPMENUEX_SET_MENUITEM_HEIGHT
         *                     TCOM_MP_CS_UI_POPUPMENUEX_TRACK_POPUPMENU
         *
         * @author :    Jijeesh K V
         * @version:    1.0            Date:  2007-04-19
         * @author :    Jijeesh K V
         * @version:    1.1            Date:  2008-05-07
         */
        class CONTROLS_CLASS_API PopupMenuEx : public CMenu
        {

            // Declaring pop up window class as friend class
            friend class PopupWindow;
            /* Begin: Added 1.1 */
            friend class MenuWnd;
            /* End: Added 1.1 */

        public:

            // Constructor
            PopupMenuEx();
            virtual ~PopupMenuEx();

        public:

            // Function to show a context menu
            BOOL TrackPopupMenu( UINT uFlags_i, int nX_i, int nY_i, CWnd* pWnd_i, LPCRECT lpRect_i = NULL, int nSubMenu_i = -1 );
            // Function to set the image list
            void SetImageList( CImageList* pImageList_i );
            // Function to set image to an item
            void SetImage( int nImagePos_i, UINT uMenuId_i );
            // Function to set the border color
            void SetBorderColor( COLORREF clrBorder_i );
            // Function to set the background color
            void SetBkColor( COLORREF clrBk_i );
            // Function to set the selected background color
            void SetSelBkColor( COLORREF clrBk_i );
            // Function to set the text color
            void SetTextColor( COLORREF clrText_i );
            // Function to set the selected text color
            void SetSelTextColor( COLORREF clrText_i );
            // Function to set disabled text color
            void SetDisTextColor( COLORREF clrDisText_i );
            // Function to set the font
            bool SetFont( LPCTSTR lpctszFontName_i, int nPointSize_i, bool bBold_i = false );
            // Function to set the menu item height
            void SetMenuItemHeight( int nHeight_i );
            // Function to get the border color
            COLORREF GetBorderColor();
            // Function to set the background color
            COLORREF GetBkColor();
            // Function to get the text color
            COLORREF GetTextColor();
            // Function to get the selected background color
            COLORREF GetSelBkColor();
            // Function to get the selected text color
            COLORREF GetSelTextColor();
            // Function to get the disabled text color
            COLORREF GetDisTextColor();
            // Function to get the menu item height]
            int GetMenuItemHeight();
            /* Begin: Added 1.1 */
            // function to set the menu bat
            BOOL SetMenuBar( CWnd* pWnd_i );
            void DeleteAll();
            /* End: Added 1.1 */
            /* Begin: Modification 1.2 #731, #732*/
            void CopyStyle( const PopupMenuEx& PopupMenu_i );
            /* End: Modification 1.2 #731, #732*/

        protected:

            // Just for disable the standard API
            BOOL TrackPopupMenuEx();
            // Function to read font settings from configuration
            CFont* GetFont();

        private:
           /**
            * Modified for MPlus V4
            * @since 1.3
            */
            PopupMenuEx( const PopupMenuEx& PopupMenuEx_i );
            PopupMenuEx& operator=( const PopupMenuEx& PopupMenuEx_i );
           /**
            * End of Modification for MPlus V4
            */

        private:


            CString m_csFontName; // Font name
            int m_nFontSize; // Font size
            bool m_bBold;
            UINT m_uFlags; // Holds flags for pop up menu
            CImageList* m_pImageList; // Holds the image list
            CMap<UINT, UINT, int, int> m_ImageMap; // Image to Id mapping
            // Color members
            COLORREF m_clrBorder; // Holds border color
            COLORREF m_clrBkNormal; // Holds normal background color
            COLORREF m_clrBkSelected; // Holds selected background color
            COLORREF m_clrTextNormal; // Holds normal text color
            COLORREF m_clrTextSelected; // Holds selected text color
            COLORREF m_clrTextDisabled; // Holds disabled text color
            int m_nItemHeight; // Holds menu item height
            bool m_bRightArrowPressed; // Indicate menu pop up on right arrow
            CImageList m_CheckImageList; // Check mark image list
            PopupWindow* m_popWnd;
            /* Begin: Added 1.1 */
            bool m_bMenuBar; // Holds menu bar flag
            MenuWnd* m_MenuBar; // Holds the menu bar window
            /* End: Added 1.1 */
            /* Begin: Added 1.4 for #MPC05207 */
            CMap<HMENU, HMENU, int, int> m_PopWndMap;
            /* End: Added 1.4 for #MPC05207 */
        };
    } // End of namespace UICommon
} // End of namespace MPlus
#endif // #ifndef _POPUPMENUEX_H_

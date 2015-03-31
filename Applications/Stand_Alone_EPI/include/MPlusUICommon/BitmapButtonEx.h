/**
 * Copyright(c) 2009-2010 TOSHIBA Medical Systems Corporation, All Rights Reserved.
 *
 * BitmapButtonEx.h - BitmapButtonEx class declaration file.
 *
 * @author :    Rahul Kumar 
 * @version:    1.0            Date:  2009-12-29
 * @info   :    Initial version
 * @author :    Jinesh P.P.
 * @version:    1.1                 Date:  2010-03-09
 * @author :    Jinesh P.P.         ( Modified for MPlusV4 BugFix Merging MPC06134 )
 * @version:    1.2                 Date:  2010-03-10
 */

#ifndef _BITMAPBUTTONEX_H_
#define _BITMAPBUTTONEX_H_

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

#include "MPlusUICommon/GdiPlusCtrl.h"
#include <afxtempl.h>

namespace Gdiplus
{
    class Graphics;
}

namespace MPlus
{
    namespace UICommon
    {
        
        // Default color definitions.
        const COLORREF CLR_PICTUREBACK = RGB( 255, 0, 255 );

        // Text align structure.
#ifndef MPLUSUICOMMON_BUTTON_ALIGNTYPE_E_DEFINED
#define MPLUSUICOMMON_BUTTON_ALIGNTYPE_E_DEFINED
        enum ALIGNTYPE_e
        {
            TUSBUTTON_ALIGN_NONE,
            TUSBUTTON_ALIGN_LEFT_TOP,
            TUSBUTTON_ALIGN_LEFT_VCENTER,
            TUSBUTTON_ALIGN_LEFT_BOTTOM,
            TUSBUTTON_ALIGN_HCENTER_TOP,
            TUSBUTTON_ALIGN_HCENTER_VCENTER,
            TUSBUTTON_ALIGN_HCENTER_BOTTOM,
            TUSBUTTON_ALIGN_RIGHT_TOP,
            TUSBUTTON_ALIGN_RIGHT_VCENTER,
            TUSBUTTON_ALIGN_RIGHT_BOTTOM
        };
#endif

        struct BUTTON_ATTRIBUTES_t;

        // Typedef declaration of structure.
        typedef struct BITMAP_BUTTON_SETTINGS_tag BITMAP_BUTTON_SETTINGS_t;
        struct BITMAP_BUTTON_SETTINGS_tag
        {          
            bool bConfigFileRead;
            // Holds bitmap settings informations.
            CMap<CString, LPCTSTR, BUTTON_ATTRIBUTES_t*, BUTTON_ATTRIBUTES_t*> BitmapSetMap; 
            // Holds the count of bitmap settings in ini file.
            int nBitmapSetCount;
            /* Begin: Modified 1.2 for Bug ID #MPC06134 */
            // To Synchronize the Shared Dc
            CRITICAL_SECTION SharedDcCS;
            /* End: Modified 1.2 for Bug ID #MPC06134 */
           
            BITMAP_BUTTON_SETTINGS_tag();
            ~BITMAP_BUTTON_SETTINGS_tag();
        };
        

        /**
         * Copyright(c) 2009 TOSHIBA Medical Systems Corporation, All Rights Reserved.
         *
         * BitmapButtonEx - Extended button class, derived from CButton.
         *
         * @traceability         : #MPC05897
         *
         * @author :    Rahul Kumar
         * @version:    1.0            Date:  2009-12-29
         * @info   :    Initial version
         */
        class CONTROLS_CLASS_API BitmapButtonEx : public CButton
        {

        public:

            BitmapButtonEx();
            virtual ~BitmapButtonEx();
        
        public:
            
            // Button states.
            enum BUTTONSTATE_e
            {
                STATE_NORMAL,
                STATE_PRESSED,
                STATE_DISABLED
            };
            
            //{{AFX_VIRTUAL(ButtonEx)
            public:
            virtual void DrawItem( LPDRAWITEMSTRUCT pstDrawItemStruct_i );
            protected:
            virtual void PreSubclassWindow();
            //}}AFX_VIRTUAL

        public:

            // Enable/disable mouse hover effect.
            void EnableHoverEffect( bool bEnable_i = true )
            {
                m_bHoverEffect = bEnable_i;
            }
            // To get the primary text position.
            void GetPrimaryTextPos( CPoint& TextPoint_o );
            // To get the secondary text.
            CString GetSecondaryText();
            // To get the secondary text position.
            void GetSecondaryTextPos( CPoint& TextPoint_o );
            // To set the bitmap to the button.
            void SetBitmap( HBITMAP hBitmap_i, COLORREF clrTransparent_i = CLR_PICTUREBACK );
            // To set an icon to the button.
            bool SetIcon( HICON hIcon_i );
            // To set the picture position.
            void SetPicturePos( int nPosX_i, int nPosY_i );
            // To set primary text align.
            void SetPrimaryTextAllign( ALIGNTYPE_e eAlign_i );
            // To set the primary text font.
            void SetPrimaryTextFont( const CString& csFontName_i, int nSize_i, bool bBold_i = false );
            // To set primary text position.
            void SetPrimaryTextPos( int nPosX_i, int nPosY_i );
            // To set the secondary text.
            void SetSecondaryText( const CString& csText_i );
            // To set secondary text align.
            void SetSecondaryTextAllign( ALIGNTYPE_e eAlign_i );
            // To set the secondary text position.
            void SetSecondaryTextFont( const CString& csFontName_i, int nSize_i, bool bBold_i = false );
            // To set the secondary text position.
            void SetSecondaryTextPos( int nPosX_i, int nPosY_i );
            // To get the button push (Check) state.
            bool GetCheckState();
            // To set the button push (Check) state.
            void SetCheckState( bool bPushState_i );
            // To Enable/Disable the check box mode.
            void EnableCheckBox( bool bEnable_i );
            // To set the BitmapSet to be used for drawing.
            void UseBitmapSet( const CString& csBitmapSet_i );
            /* Begin: Added 1.1 for Bug ID MPC06127 */
            // Overridden function to handle Invalidate.
            // This is required to avoid the call to Invalidate when window handle is invalid.
            void Invalidate( BOOL bErase_i = TRUE )
            {
                if( IsWindow( m_hWnd ))
                {
                    ::InvalidateRect( m_hWnd, 0, bErase_i );
                }
            }
            /* End: Added 1.1 for Bug ID MPC06127 */

        protected:
            
            //{{AFX_MSG( BitmapButtonEx )            
            afx_msg BOOL OnEraseBkgnd( CDC* pDC_i );
            afx_msg void OnMouseMove( UINT nFlags_i, CPoint Point_i );
            afx_msg UINT OnGetDlgCode();
            afx_msg void OnSize( UINT nType_i, int nCX_i, int nCY_i );
            afx_msg LRESULT OnMouseLeave( WPARAM wParam_i, LPARAM lParam_i );
            afx_msg LRESULT OnSetStyle( WPARAM wParam_i, LPARAM lParam_i );  
            //}}AFX_MSG                      
            DECLARE_MESSAGE_MAP()
           
        private:

            // Create bitmaps of button sets.
            void CreateButtonBitmaps( BUTTON_ATTRIBUTES_t* stBitmapAttributes_o, int nSizeIndex_i );
            // Get correct bitmap index whose size approximately equal to actual button size.
            int GetBitmapIndex( CSize sizeActual );
            /* Begin: Modified 1.2 for Bug ID #MPC06134 */
            // Function to draw the text and picture.
            // void DrawInnerObject( Gdiplus::Graphics& ObjGraphics_i, CDC& DC_i, BUTTONSTATE_e eState_i, 
            //                      CRect rect_i );
            void DrawInnerObject( CDC& DC_i, BUTTONSTATE_e eState_i, 
                                  CRect rect_i );
            /* End: Modified 1.2 for Bug ID #MPC06134 */
            // To get the alignment.
            static void AlignText( CDC& dc_i, const CString& csCaption_i, ALIGNTYPE_e eAlign_i,
                                   const CPoint& pos_i, RECT& Rect_io );
            // Scale bitmap to actual button.
            bool Scale9Slice( CDC* pDCDst_o, CDC* pDCSrc_i, const CRect& rectDst_o, const CRect& rectSrc_i,
                              const CRect& rect9Slice_i );
            /* Begin: Modified 1.2 for Bug ID #MPC06134 */
            // Draw focus rect over button.
            // void DrawRectFocus( Gdiplus::Graphics& ObjGraphics_o, CRect rect_i );
            void DrawRectFocus( CDC& DC_i, CRect rect_i );
            /* End: Modified 1.2 for Bug ID #MPC06134 */
            // To track notification messages.
            BOOL OnChildNotify( UINT uMessage_i, WPARAM wParam_i, LPARAM lParam_i, LRESULT* pLResult_o );
                
        private:

            // Object to initialize the gdi plus.
            GdiPlusCtrl m_GdiInit;
            // Holds check status of bitmap button.
            bool m_bPushState;
            // Holds mouse hover flag.
            bool m_bHoverFlag;
            // Holds check box mode.
            bool m_bCheckEnable;
            // Holds the Primary text coordinate.
            CPoint m_PrimTextPoint;
            // Holds the Primary text font name.
            CString m_csPrimFontName;
            // Holds the Primary text font size.
            int m_nPrimFontSize;
            // Holds primary text bold type.
            bool m_bPrimFontBold;
            // Holds primary text alignment.
            ALIGNTYPE_e m_ePrimaryTextAlign;
            // Holds secondary text.
            CString m_csSecondaryText;
            // Holds secondary text position.
            CPoint m_SecTextPoint;
            // Holds Secondary text font name.
            CString m_csSecFontName;
            // Holds secondary text font size.
            int m_nSecFontSize;
            // Holds secondary text bold type.
            bool m_bSecFontBold;
            // Holds secondary text alignment.
            ALIGNTYPE_e m_eSecondaryTextAlign;
            // Holds the picture display coordinate.
            CPoint m_PicturePoint;
            // Hold Icon handle
            HICON m_hPicIcon;
            int m_nPicsHeight;
            int m_nPicsWidth;
            // Default property.
            bool m_bDefPushBtn;
            // Holds Enable HoverEffect status.
            bool m_bHoverEffect; 
            // Holds currently used bitmap set 
            CString m_csUsedBitmapSet;
            // Hold buttons Settings in ini file and its bitmaps.
            static BITMAP_BUTTON_SETTINGS_t m_stButtonSettings;
            /* Begin: Modified 1.2 for Bug ID #MPC06134 */
            // // Holds currently used bitmap.
            // CBitmap* m_pCurrentSetBitmap;
            // Holds currently used DC;
            CDC* m_pCurrentSetDC;
            /* End: Modified 1.2 for Bug ID #MPC06134 */
            // Holds Size index of currently used bitmap set.
            int m_nBitmapSizeIndex;
        };
        
   } // End of namespace UICommon
} // End of namespace MPlus
#endif // _BITMAPBUTTONEX_H_
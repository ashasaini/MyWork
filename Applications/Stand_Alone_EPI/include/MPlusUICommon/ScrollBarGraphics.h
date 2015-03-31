/**
 * Copyright(c) 2006 - 2010 TOSHIBA Medical Systems Corporation, All Rights
 * Reserved.
 *
 * ScrollBarGraphics.h - ScrollBarGraphics class declaration file.
 *
 * @author :    Nishad S
 * @version:    1.0            Date:  2006-03-09
 * @author :    Vinod N N
 * @version:    2.0            Date:  2007-04-24
 * @author :    Shypa Padmanabhan     (Modified for MPlusV4)
 * @version:    2.1            Date:  2009-01-09
 * @author :    Jinesh P.P.  ( Modified for MPlusV4 BugFix Merging #MPC05902(Performance Improvement))
 * @version:    2.2           Date:  2010-02-15
 */
#ifndef _SCROLLBARGRAPHICS_H_
#define _SCROLLBARGRAPHICS_H_

#include "MPlusUICommon/GdiPlusCtrl.h"

#ifdef UI_CONTROLS_DLL
#define CONTROLS_CLASS_API __declspec( dllexport )
#else
#define CONTROLS_CLASS_API __declspec( dllimport )
#endif

// Forward declaration.
namespace Gdiplus
{
    class GraphicsPath;
}

namespace MPlus
{
    namespace UICommon
    {
        // State types to specifiy the colors.
        enum SB_STATE_TYPE_e
        {
            SB_STATE_NORMAL = 0, // Normal state.
            SB_STATE_PRESSED,    // Pressed or selected state.
            SB_STATE_DISABLED,   // Disabled state.
            SB_STATE_TYPE_COUNT  // Total number of the states.
        };

        /**
         * Added for Celeve PIMS Phase-5 requirement
         * @since  2.0
         */
         // Scrollbar colors.
         struct SCROLLBAR_COLORS_t
         {
             // Button border colors.
             COLORREF clrOuterLineHigh;
             COLORREF clrOuterLineLow;
             COLORREF clrInnerLineHigh; // This member variable is not using now.
             COLORREF clrInnerLineLow; // This member variable is not using now.
             // Face colors.
             COLORREF clrFaceHigh;
             COLORREF clrFaceLow;
             // Arrow color.
             COLORREF clrArrow;
             // Thumb line colors.
             COLORREF clrThumbLine;
             COLORREF clrThumbLineShadow;
             // Channel colors.
             COLORREF clrChanHigh;
             COLORREF clrChanLow;
         };
        /**
         * End of Change  for Celeve PIMS Phase-5 requirement
         * @since  2.0
         */

        /**
         * Copyright(c) 2006 - 2007 TOSHIBA Medical Systems Corporation, All Rights
         * Reserved.
         *
         * ScrollBarGraphics - This class contains the functions required
         *                      for drawing the scrollbar.
         *
         * Traceability Tags : TCOM_MP_CS_UI_SCROLLBARGRAPHICSEX_SET_COLORS
         *
         * @author :    Nishad S
         * @version:    1.0            Date:  2006-03-09
         * @author :    Vinod N N
         * @version:    2.0            Date:  2007-04-24
         */
        class CONTROLS_CLASS_API ScrollBarGraphics
        {
        public:

            // Constructor.
            ScrollBarGraphics();

           /**
            * Modified for MPlus V4
            * @since 2.1
            */
            virtual ~ScrollBarGraphics();
           /**
            * End of Modification for MPlus V4
            */

            // Function to initialize members.
            void Initialize();
            // Function to draw scrollbar button.
            void DrawButton( HDC hDC_i, const CRect& rect_i, SB_STATE_TYPE_e eState_i = SB_STATE_NORMAL,
                             bool bMouseOver_i = false // Added for Celeve PIMS Phase-5 requirement
                             );
            // Function to draw left button.
            void DrawLeftButton( HDC hDC_i, const CRect& rect_i, SB_STATE_TYPE_e eState_i = SB_STATE_NORMAL,
                                 bool bMouseOver_i = false // Added for Celeve PIMS Phase-5 requirement
                                 );
            // Function to draw right button.
            void DrawRightButton( HDC hDC_i, const CRect& rect_i, SB_STATE_TYPE_e eState_i = SB_STATE_NORMAL,
                                  bool bMouseOver_i = false // Added for Celeve PIMS Phase-5 requirement
                                  );
            // Function to draw up button.
            void DrawUpButton( HDC hDC_i, const CRect& rect_i, SB_STATE_TYPE_e eState_i = SB_STATE_NORMAL,
                               bool bMouseOver_i = false // Added for Celeve PIMS Phase-5 requirement
                               );
            // Function to draw down button.
            void DrawDownButton( HDC hDC_i, const CRect& rect_i, SB_STATE_TYPE_e eState_i = SB_STATE_NORMAL,
                                 bool bMouseOver_i = false // Added for Celeve PIMS Phase-5 requirement
                                 );
            // Function to draw horizontal thumb.
            void DrawHorizontalThumb( HDC hDC_i, const CRect& rect_i, SB_STATE_TYPE_e eState_i = SB_STATE_NORMAL,
                                      bool bMouseOver_i = false // Added for Celeve PIMS Phase-5 requirement
                                      );
            // Function to draw vertical thumb.
            void DrawVerticalThumb( HDC hDC_i, const CRect& rect_i, SB_STATE_TYPE_e eState_i = SB_STATE_NORMAL,
                                    bool bMouseOver_i = false // Added for Celeve PIMS Phase-5 requirement
                                    );
            // Function to draw horizontal channel.
            void DrawHorizontalChannel( HDC hDC_i, const CRect& rect_i, SB_STATE_TYPE_e eState_i = SB_STATE_NORMAL );
            // Function to draw vertical channel.
            void DrawVerticalChannel( HDC hDC_i, const CRect& rect_i, SB_STATE_TYPE_e eState_i = SB_STATE_NORMAL );

            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            // Mouse Over Outer line color
            void SetMouseOverColor( COLORREF clrMouseOver_i );
            COLORREF GetMouseOverColor(){return m_clrOuterLineMouseOver;};

            //Sets the Scrollbar colors at a particular state
            void SetStateColors( int nSBState_i, SCROLLBAR_COLORS_t stColors_i );

            //Gets the Scrollbar colors at a particular state
            SCROLLBAR_COLORS_t GetStateColors( int nSBState_i ){return m_stColors[nSBState_i];};

            // Abstract function
           /**
            * Modified for MPlus V4
            * @since 2.1
            */
            //virtual void Invalidate( BOOL bErase = TRUE ){ bErase = FALSE; }
            virtual void Invalidate( BOOL bErase_i = TRUE );
           /**
            * End of Modification for MPlus V4
            */
            /**
             * End of Change  for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */

            /* Begin: Modification 2.2 for Bug ID #MPC05902(Performance Improvement) */            
            // Function to draw scrollbar button.
            void GdiDrawButton( CDC& dc_i, const CRect& rect_i,
                                SB_STATE_TYPE_e eState_i = SB_STATE_NORMAL, bool bMouseOver_i = false );
            // Function to draw left button.
            void GdiDrawLeftButton( CDC& dc_i, const CRect& rect_i,
                                    SB_STATE_TYPE_e eState_i = SB_STATE_NORMAL, bool bMouseOver_i = false );
            // Function to draw right button.
            void GdiDrawRightButton( CDC& dc_i, const CRect& rect_i,
                                     SB_STATE_TYPE_e eState_i = SB_STATE_NORMAL, bool bMouseOver_i = false );
            // Function to draw up button.
            void GdiDrawUpButton( CDC& dc_i, const CRect& rect_i,
                                  SB_STATE_TYPE_e eState_i = SB_STATE_NORMAL, bool bMouseOver_i = false );
            // Function to draw down button.
            void GdiDrawDownButton( CDC& dc_i, const CRect& rect_i,
                                    SB_STATE_TYPE_e eState_i = SB_STATE_NORMAL, bool bMouseOver_i = false );
            // Function to draw horizontal thumb.
            void GdiDrawHorizontalThumb( CDC& dc_i, const CRect& rect_i,
                                         SB_STATE_TYPE_e eState_i = SB_STATE_NORMAL, bool bMouseOver_i = false );
            // Function to draw vertical thumb.
            void GdiDrawVerticalThumb( CDC& dc_i, const CRect& rect_i,
                                       SB_STATE_TYPE_e eState_i = SB_STATE_NORMAL, bool bMouseOver_i = false );
            // Function to draw horizontal channel.
            void GdiDrawHorizontalChannel( CDC& dc_i, const CRect& rect_i,
                                           SB_STATE_TYPE_e eState_i = SB_STATE_NORMAL );
            // Function to draw vertical channel.
            void GdiDrawVerticalChannel( CDC& dc_i, const CRect& rect_i,
                                         SB_STATE_TYPE_e eState_i = SB_STATE_NORMAL );
            /* End: Modification 2.2 for Bug ID #MPC05902(Performance Improvement) */

        private:

            // Function to create roundrect path.
            void GetRoundRectPath( const CRect& rect_i, int nRad_i, Gdiplus::GraphicsPath& path_o );

        private:

            /**
             * Took out for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
//             // Scrollbar colors.
//             struct COLORS_t
//             {
//                 // Button border colors.
//                 COLORREF clrOuterLineHigh;
//                 COLORREF clrOuterLineLow;
//                 COLORREF clrInnerLineHigh;
//                 COLORREF clrInnerLineLow;
//                 // Face colors.
//                 COLORREF clrFaceHigh;
//                 COLORREF clrFaceLow;
//                 // Arrow color.
//                 COLORREF clrArrow;
//                 // Thumb line colors.
//                 COLORREF clrThumbLine;
//                 COLORREF clrThumbLineShadow;
//                 // Channel colors.
//                 COLORREF clrChanHigh;
//                 COLORREF clrChanLow;
//             };
            /**
             * End of Change  for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */

            COLORREF m_clrBk;    // Background color.
            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            // Mouse Over Outerline color
            COLORREF m_clrOuterLineMouseOver;
            /**
             * End of Change  for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */

            /**
             * Took out for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
//            COLORS_t m_stColors[SB_STATE_TYPE_COUNT]; // Color array to store all the state colors.
            /**
             * End of Change  for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */

            /**
             * Added for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            SCROLLBAR_COLORS_t m_stColors[SB_STATE_TYPE_COUNT]; // Color array to store all the state colors.
            /**
             * End of Change  for Celeve PIMS Phase-5 requirement
             * @since  2.0
             */
            // Object to initialize the gdi plus
            GdiPlusCtrl m_GdiInit;
        };
    } // End of namespace UICommon
} // End of namespace MPlus

#endif // #ifndef _SCROLLBARGRAPHICS_H_

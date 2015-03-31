/**
 * Copyright(c) 2008 TOSHIBA Medical Systems Corporation, All Rights Reserved.
 *
 * TreeListCtrl.h - Class declaration file for TreeListCtrl
 *
 * @author :    Jijeesh K V
 * @version:    1.0            Date:  2008-07-03
 * @author :    Naveen R
 * @version:    1.1            Date:  2008-12-22
 */

#ifndef _TREELIST_CTRL_H_
#define _TREELIST_CTRL_H_

#include <afxtempl.h>
#include "MPlusUICommon/ListCtrlEx.h"

#ifndef _UNICODE
#error Unicode definition is necessary to use this class
#endif

#ifdef UI_CONTROLS_DLL
#define CONTROLS_CLASS_API __declspec( dllexport )
#else
#define CONTROLS_CLASS_API __declspec( dllimport )
#endif

namespace MPlus
{
    namespace UICommon
    {
        // Constant definition
        const UINT DEFAULT_TREE_COL_WIDTH = 90; // Default width of the column
        const LPCTSTR TLC_ITEMCHANGED_STRING = _T( "TreeListItemChanged" ); // Message string
        // Messages
        const UINT TLM_ITEMCHANGED = RegisterWindowMessage( TLC_ITEMCHANGED_STRING );
        const int DEPTH = -1;
        const int INDX = -1;

        // Pre-declarations of classes
        class TreeListItem;

        /**
         * Copyright(c) 2008 TOSHIBA Medical Systems Corporation, All Rights Reserved.
         *
         * TreeListCtrl.h - Class declaration file for TreeListCtrl
         *
         * @author :    Jijeesh K V
         * @version:    1.0            Date:  2008-07-03
         */
        class CONTROLS_CLASS_API TreeListCtrl : public ListCtrlEx
        {

        public:
            // Constructor
            TreeListCtrl();
            // Destructor
            virtual ~TreeListCtrl();
            // Insert an item to the TreeListCtrl
            void InsertItem( TreeListItem* ParentItem_i,
                             TreeListItem* NewItem_i,
                             CString& csKey_i );
            // Look up if an item exists in the TreeListCtrl
            TreeListItem* LookUp( CString& csKey_i,
                                  int& nIndex_io,
                                  bool bLookAll = true );
            // Find an Item in the TreeListCtrl on the basis of Index
            TreeListItem* FindIndex( int nIndex,
                        bool bLookAll = true );
            // Get number of Items in the TreeListCtrl
            int GetCount( bool bLookAll_i = true );
            // Remove an item with given Key
            void RemoveItem( CString& csKey_i, bool bLokAll_i = true );
            // Remove an Item at the given Index
            void RemoveItem( int nIndex_i, bool bLookAll_i = true );
            // Set whether to exclude root node from tree list control
            void SetExcludeRootNode( bool bExclude = true );
            //  Get whether root node is excluded from tree list control
            bool GetExcludeRootNode();
            // Shows the TreeList control
            void ShowWindow( int nComdShow_i );
            // Inserts a column to tree list control
            int InsertColumn( const int nCol_i,
                              const CString& csColumnHeading_i,
                              const int nFormat_i = LVCFMT_LEFT ,
                              const int nWidth_i = DEFAULT_TREE_COL_WIDTH );
            // This function will update GUI with data
            void SyncDataToGUI();
            // This function will return a pointer list of root nodes
            CPtrList* GetParentNodes();
            // Sets indentation of special case on second level
            void SetSecondLevelIndent( const int nSecondLevelIndent_i );
            // Gets the indentation of special case on second level
            int GetSecondLevelIndent();

        protected:

            //{{AFX_VIRTUAL( TreeListCtrl )
            virtual void PreSubclassWindow();
            virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct_i );
            virtual BOOL PreTranslateMessage( MSG* pMsg_i );
         //}}AFX_VIRTUAL

            //{{AFX_MSG(TreeListCtrl)
            afx_msg void OnGetdispinfo( NMHDR* pNMHDR_i, LRESULT* pResult_i );
            afx_msg void OnDblclk( NMHDR* pNMHDR_i, LRESULT* pResult_i );
            afx_msg void OnClick( NMHDR* pNMHDR_i, LRESULT* pResult_i );
            afx_msg void OnLButtonDown( UINT uFlags_i, CPoint Point_i );
            afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
            //}}AFX_MSG
            DECLARE_MESSAGE_MAP()

        private:

            // Copy Constructor
            TreeListCtrl( const TreeListCtrl& TreeListCtrl_i );
            // Assignment operator
            const TreeListCtrl& operator=( const TreeListCtrl& TreeListCtrl_i );
            // Search an Item in the TreeListCtrl
            TreeListItem* Search( CPtrList* pList_i,
                                  CString& csKey_i,
                                  int& nIndex_io,
                                  bool bLookAll_i = true,
                                  bool bFindByKey_i = true,
                                  int nIndex_i = INDX );

            // Count items in the TreeListCtrl
            void Count( CPtrList* pList_i,
                                  int& nCount_i,
                                  bool bLookAll_i = true );
            // Get number of columns
            int GetColumnCount();
            // Calculate rectangles
            void AdjustRect( int nDepth_i, int nChildCount_i, CRect& TextRect_i,
                             CRect& SwitchRect_i );
            // Draw Switch - Plus/Minus
            void DrawSwitchRect( CDC* pDC_i, CRect& SwitchRect_i, bool bExpanded_i,
                                 int nDepth_i, COLORREF BackColor_i );
            // Drawing Horizontal line connecting SwitchRect and TextRect
            void DrawConnectorLine( CDC* pDC_i,
                                    CRect& SwitchRect_i,
                                    CRect& TextRect_i,
                                    int nDepth_i,
                                    bool bExcludeRootNode_i = true );
            // Draw Vertical root line
            void DrawVerticalRootLines( CDC* pDC_i, int nIndex_i,
                                        CRect& SwitchRect_i,
                                        CRect& TextRect_i, int nDepth_i,
                                        int nChildCount_i, bool bExpanded_i );

            // Draw an item text
            /* Begin: Modification 1.1 #719 - Unused COLORREF values removed from argument list.*/
            // void DrawItemText( CDC* pDC_i, CString& csItemText_i, CRect& TextRect_i,
            //                    COLORREF& TextColor_i, COLORREF& BackColor_i );
            // New parameter for specifying text alignment is added.
            void DrawItemText( CDC* pDC_i, CString& csItemText_i, CRect& TextRect_i, UINT uTextAlignment_i );
            /* End: Modification 1.1 #719*/

            // Empties the child List
            void EmptyList( CPtrList* pList_i );
            // Redraw the vertical root lines
            void ReDrawAllLines( CDC *pDC_i );

        private:

            // List of all items in the TreeListCtrl
            CPtrList m_ItemList;
            // Array of TreeList items on visible index basis
            CPtrArray m_VisibleItemArray;
            // TreeListCtrl Indentation
            int m_nIndent;
            // If it was plus or minus
            bool  m_bIsPlus;
            // Exclude root of SwitchRect drawing
            bool  m_bExcludeRoot;
            // Holds remove flags
            bool m_bRemoveItem;
            // Stack of Items for drawing
            int m_nScrollHW;                // Scroll measure
            int m_nSecondLevelIndent;       // To manage special case of second level
            CPoint m_DownPoint;             // To handle single click expand
            int m_nReDrawCount;             // Redraw count

            COLORREF m_clrRootLines;       // Holds root lines color
            COLORREF m_clrOddRow;          // Holds odd row color
            COLORREF m_clrEvenRow;         // Holds even row color
            COLORREF m_clrText;            // Holds text color
            COLORREF m_clrDisabledText;    // Holds disabled text color
            COLORREF m_clrSelBk;           // Holds selected Background color
            COLORREF m_clrSelText;         // Holds selected text color
            COLORREF m_clrGrid;            // Holds normal grid color
            COLORREF m_clrDisabledGrid;    // Holds disabled grid color
            COLORREF m_clrDisabledOddRow;  // Holds disabled odd row color
            COLORREF m_clrDisabledEvenRow; // Holds disabled even row color
            COLORREF m_clrItemHorzGrid;    // Holds the horizontal grid color
        };
    } // End of namespace UICommon
} // End of namespace MPlus
#endif // #ifndef _TREELIST_CTRL_H_


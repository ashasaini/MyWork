/**
 * Copyright(c) 2008 TOSHIBA Medical Systems Corporation, All Rights
 * Reserved.
 *
 * TreeListItem.h - Implementation file of TreeListItem class
 *
 * @author :    Jijeesh K V
 * @version:    1.0            Date:  2008-07-03
 */
#ifndef _TREELIST_ITEM_H_
#define _TREELIST_ITEM_H_

#ifndef _UNICODE
#error Unicode definition is necessary to use this class
#endif

#ifdef UI_CONTROLS_DLL
#define CONTROLS_CLASS_API __declspec( dllexport )
#else
#define CONTROLS_CLASS_API __declspec( dllimport )
#endif

const int MAX_TREELISTITEM = 17;

namespace MPlus
{
    namespace UICommon
    {
        // enum for managing list item state
        enum TREE_LIST_ITEM_STATE_e
        {
            TLIS_EXPANDED = 2,
                TLIS_COLLAPSED = 1
        };

        /**
         * Copyright(c) 2008 TOSHIBA Medical Systems Corporation, All Rights
         * Reserved.
         *
         * TreeListItem - This class represent a single item in TreeListCtrl
         *
         * @author :    Jijeesh K V
         * @version:    1.0            Date:  2008-07-03
         */
        class CONTROLS_CLASS_API TreeListItem
        {
        public:

            // Default constructor
            TreeListItem();
            // Copy constructor
            TreeListItem( const TreeListItem& TreeListItemObj_i );
            // Destructor
            virtual ~TreeListItem();
            // To set the state of an item, expand or collapse
            void SetItemState( TREE_LIST_ITEM_STATE_e eItemState_i );
            // To get the state of an item, expand or collapse
            TREE_LIST_ITEM_STATE_e GetItemState();
            // To get the list of child items
            CPtrList* GetChildItems();
            // To set the parent node item
            void SetParentNode( TreeListItem* pParentNode_i );
            // To get parent node item
            TreeListItem* GetParentNode();
            // To set key
            void SetKey( CString& csKey );
            // To get key
            CString GetKey();
            // Index operator
            CString& operator[]( int nIndex );
            // To set the text
            void SetText( int nIndex, CString& csItem );
            // To get the Text
            CString& GetText( int nIndex_i );
            // To set the depth
            void SetDepth( int nDepth_i );
            // To get the depth
            int GetDepth();
            // Assignment operator
            const TreeListItem& operator =( TreeListItem& TItem_i );
            // Sets Switch rectangle of a tree list item
            void SetSwitchRect( CRect& SwitchRect_i );
            // Get switch rectangle of a tree list item
            CRect& GetSwitchRect();
            // Sets visible index of an item
            void SetVisibleIndex( int nVIndex_i );
            // Get visible index of an item
            int GetVisibleIndex();
            // Get Array of all visible items
            CPtrArray& GetVisibleItemArray();

        protected:

            // Current state of an item
            TREE_LIST_ITEM_STATE_e m_eItemState;
            CString m_csKey;            // Key of an item
            CPtrList* m_pChildList;     // List of child items
            TreeListItem* m_pParent;    // Parent Item
            int m_nDepth;               // Depth of an item
            CRect m_SwitchRect;         // Switch rect
            int m_nVisibleIndex;        // Visible Index
            CPtrArray m_VisibleItemArray; // Array of all visible items
            CString m_csItemArray[MAX_TREELISTITEM]; // List of Items
        };
    } // End of namespace UICommon
} // End of namespace MPlus
#endif // ifndef _TREELIST_ITEM_H

/****************************************************************************
*  File Name:		dll.h	($Revision: 1.2 $)
*  Last Modified	$Date: 2010/02/17 02:30:01 $
*  Revision Log:	$Log: dll.h,v $
*  Revision Log:	Revision 1.2  2010/02/17 02:30:01  karube
*  Revision Log:	MCM0252-00020 Phase3 libWNP,Native
*  Revision Log:	
*  Revision Log:	Revision 1.1.1.1  2010/01/06 06:45:42  ohba
*  Revision Log:	no message
*  Revision Log:	
*  Revision Log:	Revision 1.1.1.1.28.1.2.3.56.1  2006/06/20 12:42:18  matsuo
*  Revision Log:	MCM0177-00001 Recon Engine porting to FreeBSD-64Bit
*  Revision Log:	
*  Revision Log:	Revision 1.1.1.1.28.1.2.3  2003/03/31 07:25:26  ohba
*  Revision Log:	from WIPRO MAR 31
*  Revision Log:	
*  Revision Log:	Revision 1.4  2002/08/07 07:55:09  purandar
*  Revision Log:	Merging new code with ported code
*  Revision Log:	
*  Revision Log:	Revision 1.1.1.1  2000/06/08 12:36:52  full
*  Revision Log:	Change Include Path
*  Revision Log:	
*  Revision Log:	Revision 1.1.1.1  2000/04/21 08:59:21  ohba
*  Revision Log:	From Persia
*  Revision Log:	
 * Revision 1.3  93/03/10  15:44:46  johnr
 * added reference to new functions dllMoveNodeAfter and dllMoveNodeBefore
 * 
 * Revision 1.2  92/09/29  22:32:08  lib
 * Converted to the new directory structure
 * 
 * Revision 1.1  92/06/09  13:51:12  jiang
 * Initial revision
 * 
*
*     File Overview:
*       This file contains include definitions necessary for using functions
*	manipulating doubly-linked lists. The functions are defined in
*	dll.c
*
****************************************************************************/
/* Revision History:
 * yy-mm-dd	modified by	modifications
 * 91-09-05	arkady		Added int dllCountTypes() prototype
 * 91-08-27	arkady		Added dllWhatNodeNum() prototype
 * 91-07-15	sdm		Walkthrough modifications; removed #defines
 *                              of FAILURE and SUCCESS.  Redefined function
 *				prototypes for dllAppendNode and dllInsert*
 * 91-05-28	jwu		Added dllDeleteNode
 * 01-05-16	arkady		Added several functions,
 *                              get NULL from stddef.h
 * 91-05-06	jwu		Created
 * End of History */   

#ifndef tami_libdll_dll_H__
#define tami_libdll_dll_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Need definition for NULL in traverse macros. */
#include <stddef.h>
#include <stdlib.h>

/* Define the Doubly-Linked List data structure. */
typedef struct DllS
{
    long   type;		/* The type of data in the item */
    void   *item;		/* The item at this node*/
    struct DllS *next;		/* The next node in the list */
    struct DllS *prev;		/* The previous node in the list*/
} DllT;

/* Define a dummy Doubly-Linked List type */
#define DLL_NO_TYPE	(0l)

/* A note on macro and function parameter names: anyNode vs. node

   When anyNode is specified, a pointer to any node in a dll is required.
   In this case, the node pointed to is not of particular interest; ANY
   node in the dll would do.

   When node is specified, then a pointer to a particular node in a dll
   is required.  In this case, the node pointed to is special in some way. */

/*** Dll macros ***/

/* These macros can be used directly, or their functionality can be
   accessed using the functions dllFindHead, dllFindTail,
   dllTraverseNodesNext, and dllTraverseNodesPrev.  Using the macros
   directly results in quicker execution; using the functions saves
   space and preserves the value of the node pointer passed.

   Note that DLL_FIND_HEAD and DLL_FIND_TAIL cannot be used as
   expressions, only as statements.  If they are used as the target of
   an if statement, then braces must be used. */

#define DLL_FIND_HEAD(anyNode) {if (anyNode)\
				    while((anyNode)->prev)\
					anyNode = (anyNode)->prev;\
					}

#define DLL_FIND_TAIL(anyNode) {if (anyNode)\
				    while((anyNode)->next)\
					anyNode = (anyNode)->next;\
					}

#define DLL_TRAVERSE_NODES_NEXT(node,func,data) \
    ((node) ? dllTraverseNodesForward((node)->next, (func), (data) ) : NULL)

#define DLL_TRAVERSE_NODES_PREV(node,func,data) \
    ((node) ? dllTraverseNodesReverse((node)->prev, (func), (data) ) : NULL)


/*** Dll function prototypes ***/

/*
 *	Return the number of nodes in the dll pointed to by dllP.
 */
int	dllCount(DllT *dllP);

/*
 *	Return the number of nodes with given type in the dll pointed to by dllP.
 */
int dllCountTypes(DllT *anyNodeP, long type);

/*	Insert an item in ascending order acording to comparison function.
 *	Return a pointer to the new node.
 */
DllT  	*dllInsertNodeAscend(DllT *nodeP, void *itemP, long type,
			     int  (*cmpFuncP)(void *ItemP, void *KeyItemP),
			     DllT **newNodeP);

/*	Insert an item before/after node nodeBefore/nodeAfter.
 *	Return a pointer to the new node.
 */
DllT  	*dllInsertBeforeNode(DllT *nodeBefore, void *item, long type,
			     DllT **newNodeP);
DllT  	*dllInsertAfterNode(DllT *nodeAfter, void *item, long type,
			    DllT **newNodeP);

/*
 *	Append a new node to the END of doubly-linked list.
 */
DllT    *dllAppendNode(DllT *node, void *item, long type, DllT **newNodeP);

/*
 *	Given any node in a dll, return the address of the first node.
 */
DllT    *dllFindHead(DllT *anyNode);

/*
 *	Given any node in a dll, return the address of the last node.
 */
DllT    *dllFindTail(DllT *anyNode);

/*	Given any node in a dll, return the address of node number num,
 *	assuming the first node (head) is number 0.
 */
DllT    *dllNodeNum(DllT *anyNode, int num);
DllT    *dllNodeType(DllT *anyNode, long type);

/*	Given any node in a dll, return the item from node number num,
 *	assuming the first node (head) is number 0
 */
void    *dllItemNum(DllT *anyNode, int num);

/*	Given dll node return node sequence number
 *	First node is 0. If dll is empty -1 is returned
 */
int dllWhatNodeNum(DllT *nodeP);

/*
 *	Given two dll's (any nodes), concatenate them into one.
 */
DllT    *dllConcat(DllT *anyNode1, DllT *anyNode2);

/*	Free each node of a dll.
 *	Optionally call a user function for each item.
 */
void    dllDeleteAll(DllT *anyNode, void (*userFreeItemFunc)(void *item));


/*	Delete a node pointed to by node from a dll.
 *	Return a pointer to the next node if exits,
 *	otherwise returns prev node or NULL.
 */
DllT    *dllDeleteNode(DllT *node, void (*userFreeItemFunc)(void *item));

/*
 *	Given a dll, traverse it and assign a given value to node->type.
 */
DllT    *dllSetAllTypes(DllT *anyNode, long newType);

/*
*  move node before or after a specified node 
*  return a pointer to the moved node
*/
DllT    *dllMoveNodeAfter(DllT *moveNode, DllT *afterNode);
DllT    *dllMoveNodeBefore(DllT *moveNode, DllT *beforeNode);

/*
 *	Dll Traverse functions:
 *
 *	Each of these functions will stop and return node/item pointer
 *	when the user function called at each node has a zero
 *	return value.
 */
DllT    *dllTraverseNodesForward(DllT *firstNode, 
				 int (*userFunc)(DllT *node, void *userData),
				 void *userData);
DllT    *dllTraverseNodesNext(DllT *leadingNode, 
			      int (*userFunc)(DllT *node, void *userData),
			      void *userData);
DllT 	*dllTraverseNodes(DllT *anyNode, 
			  int (*userFunc)(DllT *node, void *userData),
			  void *userData);
void    *dllTraverseItemsForward(DllT *firstNode, 
				 int (*userFunc)(void *item, void *userData),
				 void *userData);
void    *dllTraverseItems(DllT *anyNode, 
			  int (*userFunc)(void *item, void *userData),
			  void *userData);
DllT    *dllTraverseNodesReverse(DllT *firstNode, 
				 int (*userFunc)(DllT *node, void *userData),
				 void *userData);
DllT    *dllTraverseNodesPrev(DllT *trailingNode, 
			      int (*userFunc)(DllT *node, void *userData),
			      void *userData);
void    *dllTraverseItemsReverse(DllT *node, 
				 int (*userFunc)(void *item, void *userData),
				 void *userData);
  
#ifdef __cplusplus
} // extern "C"
#endif

#endif /* tami_libdll_dll_H__ */

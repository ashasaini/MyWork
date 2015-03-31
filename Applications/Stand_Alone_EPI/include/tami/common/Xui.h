/******************************************************************************
*   File Name:		$Source: /mri/cvsroot/develop/src.include/common/Xui.h,v $	($Revision: 1.1.28.1.2.3 $)
*   Last Modified:	$Date: 2003/03/31 07:25:09 $
*   Revision Log:	$Log: Xui.h,v $
*   Revision Log:	Revision 1.1.28.1.2.3  2003/03/31 07:25:09  ohba
*   Revision Log:	from WIPRO MAR 31
*   Revision Log:	
*   Revision Log:	Revision 1.3  2002/08/07 06:14:41  purandar
*   Revision Log:	Merging new code with ported code
*   Revision Log:	
*   Revision Log:	Revision 1.1  2000/05/16 10:15:03  full
*   Revision Log:	Change include/common
*   Revision Log:	
*   Revision Log:	Revision 1.2  2000/05/16 08:37:51  full
*   Revision Log:	Symbolic Link
*   Revision Log:	
 * Revision 1.2  92/09/29  22:03:19  lib
 * Converted to the new directory structure
 * 
 * Revision 1.1  92/04/13  10:24:18  rickm
 * Initial revision
 * 
*
*	File Overview:
*		Xui.h - include file for Xui.h.  This file contains
*		definitions for running X User Interface Library routines.
*
*	Exported Functions and Procedures:
*
******************************************************************************/
/* Revision Histroy:
   yy-mm-dd modified-by	modification-done
   90-12-17 hbw		Created.
End of History */

/* Definitions */

/* Structures */

/* Macros */

/* External functions */
extern Widget	XuiCreateManagedScrollBar();
extern void 	XuiSB_ScrollArrow();
extern void	XuiNoOp();
extern char	*XuiDecmpXmStr();


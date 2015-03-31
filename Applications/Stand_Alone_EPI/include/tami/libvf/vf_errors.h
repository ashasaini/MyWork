/******************************************************************************
 *  File Name:		$Source: /mri/cvsroot/develop/src.include/libvf/vf_errors.h,v $  ($Revision: 1.1.1.1.28.1.2.3 $)
 *  Last Modified:	$Date: 2003/03/31 07:25:40 $
 *  Revision Log:	$Log: vf_errors.h,v $
 *  Revision Log:	Revision 1.1.1.1.28.1.2.3  2003/03/31 07:25:40  ohba
 *  Revision Log:	from WIPRO MAR 31
 *  Revision Log:	
 *  Revision Log:	Revision 1.1.1.1  2002/04/16 07:15:05  cvsadmin
 *  Revision Log:	Imported
 *  Revision Log:	
 *  Revision Log:	Revision 1.1.1.1  2000/06/08 11:01:57  full
 *  Revision Log:	Change Include Path
 *  Revision Log:	
 *  Revision Log:	Revision 1.1.1.1  2000/04/21 08:59:23  ohba
 *  Revision Log:	From Persia
 *  Revision Log:	
 * Revision 1.4  92/10/22  16:46:55  park
 * Changed incorrect header comment.
 * 
 * Revision 1.3  92/10/22  16:33:25  park
 * Bracketed with #ifndef/#endif for once-only include
 * 
 *  
 *    File Overview:
 *	This file defines error codes returned by various Varfields
 *	function calls, as well as strings for Vf{Func,Field}ErrorStr()
 *	use.
 *
 *****************************************************************************/
/* Revision History:
 * yy-mm-dd	modified-by	modifications-done
 * 92-10-22	jyp		Created
 * End of History */

#ifndef tami_libvf_vf_errors_H__
#define tami_libvf_vf_errors_H__


#define	E_NO_MEM		2
#define	E_NO_SPACE		3
#define	E_BAD_SYSCALL		4
#define	E_BAD_DESCRIPTOR	5
#define	E_FILE_NOT_FOUND	6
#define	E_BAD_FORMAT		7
#define	E_BAD_OPTS		8
#define	E_BAD_PATH		9
#define	E_NOT_IMPLEMENTED	10
#define	E_EXISTS		11
#define	E_LOCK_WAIT		12
#define	E_BAD_SIZE		13

#define	E_MAX_ERRNUM		13	/* Maintain this! */

#ifdef INCLUDE_ERRSTRS
static const char *vfFuncErrorStrs[] = {
    "no error",
    "error",
    "out of memory",
    "out of space",
    "bad system call",
    "bad descriptor",
    "file not found",
    "bad format",
    "bad options",
    "bad path",
    "not implemented",
    "exists",
    "lock wait",
    "bad size",
    0};

/*
 * STUBSTUB: should remove this as soon as we can get people who are
 * "illegally" using this to convert over to the Vf{Func,Field}ErrorStr()
 * utility functions.
 */
char **Status_strings = (char **)vfFuncErrorStrs;
#else
extern char **Status_strings;
#endif


#endif /* !tami_libvf_vf_errors_H__ */

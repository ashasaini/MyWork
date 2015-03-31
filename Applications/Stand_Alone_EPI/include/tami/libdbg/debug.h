/**********************************************************************
*  File Name:		$Source: /mri/cvsroot/develop/src.include/libdbg/debug.h,v $  ($Revision: 1.1.1.1.28.1.2.3 $)
*  Last Modified:	$Date: 2003/03/31 07:25:25 $
*  Revision Log:	$Log: debug.h,v $
*  Revision Log:	Revision 1.1.1.1.28.1.2.3  2003/03/31 07:25:25  ohba
*  Revision Log:	from WIPRO MAR 31
*  Revision Log:	
*  Revision Log:	Revision 1.1.1.1  2002/04/16 07:14:55  cvsadmin
*  Revision Log:	Imported
*  Revision Log:	
*  Revision Log:	Revision 1.1.1.1  2000/06/08 12:36:51  full
*  Revision Log:	Change Include Path
*  Revision Log:	
*  Revision Log:	Revision 1.1.1.1  2000/04/21 08:59:21  ohba
*  Revision Log:	From Persia
*  Revision Log:	
 * Revision 1.5  92/09/29  22:30:22  lib
 * Converted to the new directory structure
 * 
 * Revision 1.4  92/08/21  09:07:22  arkady
 * included stdio.h
 * 
 * Revision 1.3  92/05/08  15:21:30  emmett
 * Added Prototypes
 * 
 * Revision 1.2  92/04/28  18:07:22  ilya
 * no change
 * 
 * Revision 1.1  92/04/13  10:24:25  rickm
 * Initial revision
 * 
*  
*    File Overview:
*	This file defines a complete DEBUG macro package.
*	It allows function traces, diagnostic printouts, and
*	conditional function evaluations.  It needs the companion
*	debug.c library routines if activated by "DEBUG".
*
*	The package has three levels of intrusiveness.  If the
*	preprocessor symbol "DEBUG" is not defined, all DEBUG
*	package macros become invisible to the compiler.  Even
*	if "DEBUG" is defined, each DEBUG feature invocation is
*	controlled by individual feature flags, detailed further
*	below.
*
*	There is no reason to physically edit out the DEBUG calls
*	for production systems -- not defining DEBUG is sufficient.
*
**********************************************************************/


/* Revision History:
yy-mm-dd	modified-by	modifications-done
92-08-21	arkady		included stdio.h
92-05-08        eac              Added prototypes
91-04-11	park		Created
End of History */


#ifndef NGP_DEBUG_H__
#define NGP_DEBUG_H__


/* static char	rcsid[] = "$Header: /mri/cvsroot/develop/src.include/libdbg/debug.h,v 1.1.1.1.28.1.2.3 2003/03/31 07:25:25 ohba Exp $"; */


/* None of this package is included if DEBUG is not defined. */
#ifdef DEBUG

/* Need the bitfields package to manage the debug flags mask */

#include <tami/common/bitfields.h>

#include <stdio.h>		/* Need stdout */


/*
 * Note that in the following macros, we use the form:
 *   (expr ? actions : 0)
 * instead of
 *   if(expr) actions
 * This is because the latter causes strange side effects if embedded
 * inside another if statement, due to the C if-the-else reduction rules,
 * whereas the former can be used anywhere a real function might.
 */


extern char		*Db_func_desc;

void			db_change_debug_mask(void);
void			db_set_mask(register char *);
int			db_option_on(register char *);


#define	DB_DO(opts, action)	(db_option_on(opts)?(void)action:(void)0)

#define	DB_DO_IF(expr, opts, action) \
	((expr)&&db_option_on(opts)?(void)action:(void)0)

/*
 * DB_FUNC_ENTER must be immediately after var declarations and
 * before code, since it includes a declaration.
 */
#define	DB_FUNC_ENTER(func_desc_str) \
	static char		*Db_func_desc = func_desc_str; \
	DB_PRINTF("f", ("Entered.\n"))

#define DB_FUNC_EXIT() \
	DB_PRINTF("f", ("Exit.\n"))

#define	DB_PRINT_LABEL()	fputs(Db_func_desc, stdout)

#define	DB_PRINTF(opts, arglist) \
	DB_DO(opts, (DB_PRINT_LABEL()+printf arglist))

#define	DB_PRINTF2(opts, al1, al2) \
	DB_DO(opts, (DB_PRINT_LABEL()+printf al1+printf al2))

#define	DB_PRINTF3(opts, al1, al2, al3) \
	DB_DO(opts, (DB_PRINT_LABEL()+printf al1+printf al2+printf al3))


#else /* !DEBUG */

#define	DB_DO(opts, action)
#define	DB_DO_IF(expr, opts, action)
#define	DB_FUNC_ENTER(func_desc_str)
#define	DB_FUNC_EXIT()
#define	DB_PRINTF(opts, arglist)	 
#define	DB_PRINTF2(opts, al1, al2)
#define	DB_PRINTF3(opts, al1, al2, al3)

#endif /* !DEBUG */


#endif /* !NGP_DEBUG_H__ */

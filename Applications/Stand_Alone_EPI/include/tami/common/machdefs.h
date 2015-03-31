/**********************************************************************
*  File Name:		$Source: /mri/cvsroot/develop/src.include/common/machdefs.h,v $  ($Revision: 1.1.28.1.2.3.56.2 $)
*  Last Modified:	$Date: 2006/10/16 08:02:58 $
*  Revision Log:	$Log: machdefs.h,v $
*  Revision Log:	Revision 1.1.28.1.2.3.56.2  2006/10/16 08:02:58  matsuo
*  Revision Log:	MCM0177-00002 Recon Host Windows Native Porting
*  Revision Log:	
*  Revision Log:	Revision 1.1.28.1.2.3.56.1  2006/06/20 12:38:40  matsuo
*  Revision Log:	MCM0177-00001 Recon Engine porting to FreeBSD-64Bit
*  Revision Log:	
*  Revision Log:	Revision 1.1.28.1.2.3  2003/03/31 07:25:10  ohba
*  Revision Log:	from WIPRO MAR 31
*  Revision Log:	
*  Revision Log:	Revision 1.5  2002/08/07 06:14:49  purandar
*  Revision Log:	Merging new code with ported code
*  Revision Log:	
*  Revision Log:	Revision 1.1  2000/05/16 10:15:03  full
*  Revision Log:	Change include/common
*  Revision Log:	
*  Revision Log:	Revision 1.2  2000/05/16 08:37:51  full
*  Revision Log:	Symbolic Link
*  Revision Log:	
 * Revision 1.3  92/09/29  22:03:45  lib
 * Converted to the new directory structure
 * 
 * Revision 1.2  92/08/27  10:47:02  ilya
 * *** empty log message ***
 * 
 * Revision 1.1  92/04/13  10:24:39  rickm
 * Initial revision
 * 
*  
*    File Overview:
*	This file contains typedefs that explicitly encode the
*	bit-sizes of the data types in the name.  By making this
*	file heavily CPU architecture-dependent, code that uses this
*	should be substantially machine-INDEPENDENT.
*
*	In addition to the typedefs, the ANSI-specified macro symbols
*	for specifying the host-dependent word value limits has been
*	extended to cover our new types (see the ANSI Specification
*	for the requirements for limits.h and float.h).

*    NB: Obviously, this file is heavily machine-dependent, and should
*	be rewritten if our code is ported to a new host.
*
**********************************************************************/


/* Revision History:
yy-mm-dd	modified-by	modifications-done
91-04-11	park		Created
End of History */

#ifndef NGP_MACHDEFS_H__
#define NGP_MACHDEFS_H__

/* static char	rcsid[] = "$Header: /mri/cvsroot/develop/src.include/common/machdefs.h,v 1.1.28.1.2.3.56.2 2006/10/16 08:02:58 matsuo Exp $"; */


#include <limits.h>

/*
 * The following are defined in the EXTENSIONS part of SGI's limits.h,
 * which is inconvenient (and wrong), since they are defined in float.h,
 * as per ANSI.
 * NB: I did not do the usual #ifdef--#undef--#endif sequence, because
 * if SGI ever fixes this, I want to know via a cpp complaint, so that
 * I can finally remove these onerous lines.
 */
#ifndef __NUTC__
#undef FLT_DIG
#undef FLT_MIN
#undef FLT_MAX
#undef DBL_DIG
#undef DBL_MIN
#undef DBL_MAX
#endif

#ifdef V35M
#include <float.h>
#elif __NUTC__
#include <float.h>
#else
#include </usr/include/float.h>
#endif
/*
#ifdef V35M
#include </usr/include/float.h>
#else
#include </usr/include/float.h>
#endif
*/

#ifdef V35M
#  ifdef _XtIntrinsic_h
typedef Cardinal NxCardinal_t;
#  else /* _XtIntrinsic_h */
typedef unsigned int	NxCardinal_t;
#  endif /* _XtIntrinsic_h */
typedef char*		NxPointer_t;	/* for MrmFetchLiteral */
#else /* V35M */
typedef int		NxCardinal_t;
#  if NeedFunctionPrototypes
typedef void*		NxPointer_t;	/* XtPointer */
#  else /* NeedFunctionPrototypes */
typedef char*		NxPointer_t;	/* XtPointer */
#  endif /* NeedFunctionPrototypes */
#endif /* V35M */

#ifdef V35M
typedef	signed char	int8_t;
#define	INT8_MIN	SCHAR_MIN
#define	INT8_MAX	SCHAR_MAX

typedef	short		int16_t;
#define	INT16_MIN	SHRT_MIN
#define	INT16_MAX	SHRT_MAX

typedef long		int32_t;
#define	INT32_MIN	LONG_MIN
#define	INT32_MAX	LONG_MAX


typedef	unsigned char	uint8_t;
#define	UINT8_MAX	UCHAR_MAX

typedef	unsigned short	uint16_t;
#define	UINT16_MAX	USHRT_MAX

typedef unsigned long	uint32_t;
#define	UINT32_MAX	ULONG_MAX

#else
#ifndef _SYS_TYPES_H
#ifndef __WIN_NATIVE_PORTING__
#include <inttypes.h>
#endif /* __WIN_NATIVE_PORTING__ */
#ifdef __FREE_BSD__
#include <stdint.h>
#endif
#else	/* ! _SYS_TYPES_H */
/*	Added by Wipro,	Rajani D, July 6, start		*/
#ifndef IRIX65
typedef unsigned char           uint8_t;
typedef unsigned short          uint16_t;
typedef unsigned int            uint32_t;
#endif	/* IRIX65 */
/*	Added by Wipro,	Rajani D, July 6, end 		*/
#endif	/* ! _SYS_TYPES_H */
#endif	/* V35M */


typedef	float		flt32_t;

#define	FLT32_MANT_DIG		FLT_MANT_DIG
#define	FLT32_EPSILON		FLT_EPSILON
#define	FLT32_DIG		FLT_DIG
#define	FLT32_MIN_EXP		FLT_MIN_EXP
#define	FLT32_MIN		FLT_MIN
#define	FLT32_MIN_10_EXP	FLT_MIN_10_EXP
#define	FLT32_MAX_EXP		FLT_MAX_EXP
#define	FLT32_MAX		FLT_MAX
#define	FLT32_MAX_10_EXP	FLT_MAX_10_EXP


typedef	double		flt64_t;

#define	FLT64_MANT_DIG		DBL_MANT_DIG
#define	FLT64_EPSILON		DBL_EPSILON
#define	FLT64_DIG		DBL_DIG
#define	FLT64_MIN_EXP		DBL_MIN_EXP
#define	FLT64_MIN		DBL_MIN
#define	FLT64_MIN_10_EXP	DBL_MIN_10_EXP
#define	FLT64_MAX_EXP		DBL_MAX_EXP
#define	FLT64_MAX		DBL_MAX
#define	FLT64_MAX_10_EXP	DBL_MAX_10_EXP


#endif /* !NGP_MACHDEFS_H__ */













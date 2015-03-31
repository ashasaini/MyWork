/**********************************************************************
*  File Name:		$Source: /mri/cvsroot/develop/src.include/common/basicdefs.h,v $  ($Revision: 1.1.28.1.2.3 $)
*  Last Modified:	$Date: 2003/03/31 07:25:09 $
*  Revision Log:	$Log: basicdefs.h,v $
*  Revision Log:	Revision 1.1.28.1.2.3  2003/03/31 07:25:09  ohba
*  Revision Log:	from WIPRO MAR 31
*  Revision Log:	
*  Revision Log:	Revision 1.4  2002/08/07 06:14:44  purandar
*  Revision Log:	Merging new code with ported code
*  Revision Log:	
*  Revision Log:	Revision 1.1  2000/05/16 10:15:03  full
*  Revision Log:	Change include/common
*  Revision Log:	
*  Revision Log:	Revision 1.2  2000/05/16 08:37:51  full
*  Revision Log:	Symbolic Link
*  Revision Log:	
 * Revision 1.5  92/09/29  22:03:28  lib
 * Converted to the new directory structure
 * 
 * Revision 1.4  92/08/25  12:19:34  rickm
 * Added patient coordinate vector.
 * 
 * Revision 1.3  92/07/14  10:14:34  scottm
 * Changed vector types
 * 
 * Revision 1.2  92/05/20  12:58:25  arkady
 * Added f32vector_t
 * 
 * Revision 1.1  92/04/13  10:24:20  rickm
 * Initial revision
 * 
*  
*    File Overview:
*	This include file will be used by all NGP applications.
*	It has nested includes (I know, slightly violates standards...)
*	for the minimal set of NGP header files to establish a nice
*	environment.  It then defines some miscellaneous useful macros,
*	data types, and declarations, for manipulating bit fields,
*	complex numbers, timeval structures, etc.
*
**********************************************************************/


/* Revision History:
 * yy-mm-dd	modified-by	modifications-done
 * 92-07-13	scottm		Changed f32vector_t to f32vec3_t;
                                added f32vec2_t.
 * 92-05-20	arkady		Added f32vector_t
 *				use tami_common_basicdefs_H__
 *				put <> around nested includes
 * 91-04-11	park		Created
 * End of History */

#ifndef tami_common_basicdefs_H__
#define tami_common_basicdefs_H__


/* static char	rcsid[] = "$Header: /mri/cvsroot/develop/src.include/common/basicdefs.h,v 1.1.28.1.2.3 2003/03/31 07:25:09 ohba Exp $"; */


/**********************************************************************
*
*  The minimal standard include files for NGP:
*
*    machdefs.h	- defines machine-independent data types to make code
*		as portable as possible, such as "int32_t" and "flt64_t".
*
*    errors.h	- defines the basic function return status type status_t,
*		as well as macros for dealing with them mnemonically.
*
*    debug.h	- A comprehensive debugging macro package, to be used
*		copiously in our code.
*
**********************************************************************/


#include <tami/common/machdefs.h>
#include <tami/common/bitfields.h>
#include <tami/common/errors.h>
#include <tami/libdbg/debug.h>

/**********************************************************************
*
* The following is for boolean operations.  "bool_t" is for use in
* code, and is more speed-efficient than sbool_t, which is for use in
* structures where every byte counts; bool_t is preferred.
**********************************************************************/

typedef	int		bool_t;
typedef	int8_t		sbool_t;

#ifndef	TRUE
#define	TRUE	1
#define	FALSE	0
#endif


/**********************************************************************
*
*  A few more often-used types and "abbrevs":
*
**********************************************************************/

typedef	char *		string_t;


/**********************************************************************
*
*  Some "complex" math struct definitions, to standardize what will
*  probably be a frequently used data type.
*
**********************************************************************/

typedef struct {
    flt32_t	real;
    flt32_t	imgn;
} f32complex_t;

typedef struct {
    flt64_t	real;
    flt64_t	imgn;
} f64complex_t;

typedef struct {
    int8_t	real;
    int8_t	imgn;
} i8complex_t;

typedef struct {
    int16_t	real;
    int16_t	imgn;
} i16complex_t;

typedef struct {
    int32_t	real;
    int32_t	imgn;
} i32complex_t;

/**********************************************************************
*
*  Vector struct definitions.
*
**********************************************************************/
#ifdef __NUTC__
#ifndef dtype_H__
typedef struct {
    flt32_t     x;
    flt32_t     y;
} f32vec2_t;

typedef struct {
    flt32_t     x;
    flt32_t     y;
    flt32_t     z;
} f32vec3_t;
#endif
#else

typedef struct {
    flt32_t	x;
    flt32_t	y;
} f32vec2_t;

typedef struct {
    flt32_t	x;
    flt32_t	y;
    flt32_t	z;
} f32vec3_t;
#endif
/**********************************************************************
*  The standard way to communicate orientation is to use the patient's
*  frame of reference. The patient coordinate system is defined by the
*  right-handed system (LR, PA, IS), where LR is the component in the
*  direction left to right, PA is the direction posterior to anterior
*  (back to front), and IS is the direction inferior to superior
*  (feet to head). The following is a vector with meaningful component
*  names for this coordinate system.
*
**********************************************************************/

typedef struct {
    flt32_t lr;	 /* left to right */
    flt32_t pa;  /* posterior to anterior (back to front) */
    flt32_t is;  /* inferior to superior (feet to head) */
} f32patvec3_t;

/**********************************************************************
*
*  Macro to make a bi-byte constant.  The arguments should be true
*  char type or char constants, or it might break.
*
**********************************************************************/

#define	BYTE_PACK2(b0, b1)	((((uint16_t) b0) << 8) | (b1))



/**********************************************************************
*
*  Macros to round values up or down to the nearest multiple of the
*    modulo value.  These macros try to avoid multiplication, and
*    especially division, as much as possible, but multiple evaluation
*    couldn't be avoided.  If you have a more efficient macro, please
*    contribute.
*
**********************************************************************/

#define	ROUND_UP(val, mod)	ROUND_DOWN((val) + (mod) - 1,mod)
#define	ROUND_DOWN(val, mod)	((val) - ((val) % (mod)))


/**********************************************************************
*
*  These macros had better be self-explanatory.  Why aren't they in
*    a ANSI-standard file?  Note that both give preference to the
*    first value in the case of a tie.
*
**********************************************************************/

#ifndef MAX
#define	MIN(a,b)	((b) < (a) ? (b) : (a))
#define	MAX(a,b)	((b) > (a) ? (b) : (a))
#endif


/**********************************************************************
*
*  Macros to add and subtract struct timevals.
*    In the macros, the result can be stored into the same struct
*    as either of the args.
*
**********************************************************************/

#define	TIMEVAL_ADD(sum, t1, t2)					\
    ((sum)->tv_sec = (t1)->tv_sec + (t2)->tv_sec +			\
	(((sum)->tv_usec = (t1)->tv_usec + (t2)->tv_usec) > 1000000L ?	\
	    ((sum)->tv_usec -= 1000000L, 1) : 0))

#define	TIMEVAL_SUB(diff, t1, t2)					\
    ((diff)->tv_sec = (t1)->tv_sec - (t2)->tv_sec -			\
	(((diff)->tv_usec = (t1)->tv_usec - (t2)->tv_usec) < 0 ?	\
	    ((diff)->tv_usec += 1000000L, 1) : 0))


#endif /* tami_common_basicdefs_H__ */

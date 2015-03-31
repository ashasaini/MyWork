/**********************************************************************
*  
*    File Overview:
*	This include file defines varfield types for all the
*	"universal" types defined in basicdefs.h and machdefs.h
*	It places them in the VFT_{MIN,MAX}_BASIC_RSRVD range
*	reserved for this purpose.
*
**********************************************************************/


/* Revision History:
yy-mm-dd	modified-by	modifications-done
91-11-06	park		Created
End of History */

#ifndef tami_vfStudy_vf_basictypes_H__
#define tami_vfStudy_vf_basictypes_H__


/*
 * First, varfield types for all the standard C types.
 * Using these are actually dangerous, since the sizes are not
 * portable across all platforms, but caveat user:
 * (besides, sizeof(type) can be stored in the size field,
 * so all is not lost)
 */

#define	VFT_SIGNED_CHAR		(VFT_MIN_BASIC_RSRVD + 0)
#define	VFT_UNSIGNED_CHAR	(VFT_MIN_BASIC_RSRVD + 1)
#define	VFT_SHORT		(VFT_MIN_BASIC_RSRVD + 2)
#define	VFT_UNSIGNED_SHORT	(VFT_MIN_BASIC_RSRVD + 3)
#define	VFT_INT			(VFT_MIN_BASIC_RSRVD + 4)
#define	VFT_UNSIGNED_INT	(VFT_MIN_BASIC_RSRVD + 5)
#define	VFT_LONG		(VFT_MIN_BASIC_RSRVD + 6)
#define	VFT_UNSIGNED_LONG	(VFT_MIN_BASIC_RSRVD + 7)
#define	VFT_FLOAT		(VFT_MIN_BASIC_RSRVD + 8)
#define	VFT_DOUBLE		(VFT_MIN_BASIC_RSRVD + 9)
#define	VFT_LONG_DOUBLE		(VFT_MIN_BASIC_RSRVD + 10)

/*
 * Reserve slots 11-31 for more basic types and combinations
 * thereof, most likely for 64-bit architectures to come.
 */


/*
 * Now, some basic fixed-size types from machdefs.h:
 */

#define	VFT_INT8		(VFT_MIN_BASIC_RSRVD + 32)
#define	VFT_UINT8		(VFT_MIN_BASIC_RSRVD + 33)
#define	VFT_INT16		(VFT_MIN_BASIC_RSRVD + 34)
#define	VFT_UINT16		(VFT_MIN_BASIC_RSRVD + 35)
#define	VFT_INT32		(VFT_MIN_BASIC_RSRVD + 36)
#define	VFT_UINT32		(VFT_MIN_BASIC_RSRVD + 37)
#define	VFT_INT64		(VFT_MIN_BASIC_RSRVD + 38)
#define	VFT_UINT64		(VFT_MIN_BASIC_RSRVD + 39)

#define	VFT_FLT32		(VFT_MIN_BASIC_RSRVD + 40)
#define	VFT_FLT64		(VFT_MIN_BASIC_RSRVD + 41)


/*
 * And finally, the ancilliary types from basicdefs.h:
 */

#define	VFT_BOOL		(VFT_MIN_BASIC_RSRVD + 42)
#define	VFT_SBOOL		(VFT_MIN_BASIC_RSRVD + 43)
#define	VFT_STRING		(VFT_MIN_BASIC_RSRVD + 44)
#define VFT_F32COMPLEX		(VFT_MIN_BASIC_RSRVD + 45)
#define VFT_F64COMPLEX		(VFT_MIN_BASIC_RSRVD + 46)
#define VFT_I8COMPLEX		(VFT_MIN_BASIC_RSRVD + 47)
#define VFT_I16COMPLEX		(VFT_MIN_BASIC_RSRVD + 48)
#define VFT_I32COMPLEX		(VFT_MIN_BASIC_RSRVD + 49)
#define VFT_BITFIELD		(VFT_MIN_BASIC_RSRVD + 50)



#endif /* !tami_vfStudy_vf_basictypes_H__ */






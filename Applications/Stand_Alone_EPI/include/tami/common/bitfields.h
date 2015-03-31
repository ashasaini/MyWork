/**********************************************************************
*  File Name:		$Source: /mri/cvsroot/develop/src.include/common/bitfields.h,v $  ($Revision: 1.1.28.1.2.3 $)
*  Last Modified:	$Date: 2003/03/31 07:25:10 $
*  Revision Log:	$Log: bitfields.h,v $
*  Revision Log:	Revision 1.1.28.1.2.3  2003/03/31 07:25:10  ohba
*  Revision Log:	from WIPRO MAR 31
*  Revision Log:	
*  Revision Log:	Revision 1.3  2002/08/07 06:14:45  purandar
*  Revision Log:	Merging new code with ported code
*  Revision Log:	
*  Revision Log:	Revision 1.1  2000/05/16 10:15:03  full
*  Revision Log:	Change include/common
*  Revision Log:	
*  Revision Log:	Revision 1.2  2000/05/16 08:37:51  full
*  Revision Log:	Symbolic Link
*  Revision Log:	
 * Revision 1.2  92/09/29  22:03:31  lib
 * Converted to the new directory structure
 * 
 * Revision 1.1  92/04/13  10:24:21  rickm
 * Initial revision
 * 
*  
*  File Overview:
*	This file defines routines defining and manipulating bitfields.
*
*  Provided macros:
*
*    BITFLD_DECL(var, nbits)	- used in the definitions part of the
*				code, creates a bitfield variable "var"
*				that can hold at least "nbits" bits.
*				NB: nbits must eval to a constant.
*
*    Following macros are used to init the bitfield.
*	Use a for-loop, 0..BITFLD_SIZE,
*	assigning BITFLD_WORD_[01] as appropriate.
*    BITFLD_SIZE(nbits)		- number of words in the bitfield.
*    BITFLD_WORD_0		- a word constant with all bits off
*    BITFLD_WORD_1		- a word constant with all bits on
*
*    BITFLD_ON(bf_array, bpos)	- evals to true (nonzero) if bit is on.
*    BITFLD_SET(bf_array, bpos)	- sets requested bit.
*    BITFLD_RESET(bf_array, bpos) - clears requested bit.
*
*  Implementation Note:
*    the package works on arrays of ints, instead of bytes, because
*    many architectures are much more efficient with word-sized
*    operand operations.
*
**********************************************************************/


/* Revision History:
yy-mm-dd	modified-by	modifications-done
91-04-11	park		Created
End of History */


#ifndef NGP_BITFIELDS_H__
#define NGP_BITFIELDS_H__

/* static char	rcsid[] = "$Header: /mri/cvsroot/develop/src.include/common/bitfields.h,v 1.1.28.1.2.3 2003/03/31 07:25:10 ohba Exp $"; */

#include <limits.h>		/* Needed for CHAR_BIT */

typedef unsigned int bitfield_t;	/* A nice efficient data type */

/* Size of the bitfield basic word.  Shouldn't need touching. */
#define	BITFLD_WORD_NBITS	(CHAR_BIT * sizeof (bitfield_t))
#define	BITFLD_WORD_ALL_0	((bitfield_t) 0)
#define	BITFLD_WORD_ALL_1	(~BITFLD_WORD_ALL_0)

#define	BITFLD_WORD(bpos)	((bpos) / BITFLD_WORD_NBITS)
#define	BITFLD_BIT(bpos)	(0x1 << ((bpos) % BITFLD_WORD_NBITS))

/* Size of bitfield *in words* needed to hold n bits. */
#define	BITFLD_SIZE(nbits)	(BITFLD_WORD((nbits) - 1) + 1)

#define	BITFLD_DECL(var, nbits)	bitfield_t var[BITFLD_SIZE(nbits)]

#define	BITFLD_INIT(var, nbits, set)			\
	{ int i;					\
	  for (i = 0; i < BITFLD_SIZE(nbits); i++)	\
	    (var)[i] = ((set) ? BITFLD_WORD_ALL_1 : BITFLD_WORD_ALL_0);	\
	}

#define	BITFLD_ON(bf_array, bpos)	\
		((bf_array)[BITFLD_WORD(bpos)] & BITFLD_BIT(bpos))
#define	BITFLD_SET(bf_array, bpos)	\
		((bf_array)[BITFLD_WORD(bpos)] |= BITFLD_BIT(bpos))
#define	BITFLD_RESET(bf_array, bpos)	\
		((bf_array)[BITFLD_WORD(bpos)] &= ~BITFLD_BIT(bpos))


#endif /* !NGP_BITFIELDS_H__ */

/******************************************************************************
 *  File Name:		$Source: /mplus/cvsroot/Theta/develop/src.include/rb/rbctl.h,v $  ($Revision: 1.1.1.1 $)
 *  Last Modified:	$Date: 2010/01/06 06:45:43 $
 *  Revision Log:	$Log: rbctl.h,v $
 *  Revision Log:	Revision 1.1.1.1  2010/01/06 06:45:43  ohba
 *  Revision Log:	no message
 *  Revision Log:	
 *  Revision Log:	Revision 1.2.12.1.2.3  2003/03/31 07:25:48  ohba
 *  Revision Log:	from WIPRO MAR 31
 *  Revision Log:	
 *  Revision Log:	Revision 1.3  2002/08/29 12:40:59  purandar
 *  Revision Log:	Merge changes
 *  Revision Log:	
 *  Revision Log:	Revision 1.2  2002/02/18 06:58:12  ohiwa
 *  Revision Log:	for EX octane
 *  Revision Log:	
 *  Revision Log:	Revision 1.1.14.1  2001/09/21 01:47:00  ohiwa
 *  Revision Log:	for octane RB
 *  Revision Log:	
 * Revision 1.3  92/10/22  18:29:40  park
 * GP-50 Phase 2
 * 
 *  
 *    File Overview:
 *	This file defines constants for the RB driver's ioctl requests.
 *
 *****************************************************************************/
/* Revision History:
 * yy-mm-dd	modified-by	modifications-done
 * 91-??-??	jyp		Created
 * 92-10-22	jyp		Added the CANCEL_IO define
 * 00-01-217	Y.Ohiwa		for OCTANE.
 * End of History */



#ifndef tami_rb_rbctl_H__
#define tami_rb_rbctl_H__

#ifdef	IRIX65
#include <sys/types.h>
#endif	/* IRIX65 */

/*
 * Command options for ioctl() call on the rb i/o or ctl device:
 */

#define	RBCTL_STAT	1
#define	RBCTL_RESET	2
#define	RBCTL_RMSTAT	3
#define	RBCTL_RMRESET	4
#define	RBCTL_DIAG	5
#define	RBCTL_GSTATS	6
#define	RBCTL_MKERR	7
#define	RBCTL_CANCEL_IO	8
#ifdef	IRIX65
#define	RBCTL_INIT	9
#endif	/* IRIX65 */

#ifdef	IRIX65
typedef	struct {
	uint32_t	base;
	uint32_t	size;
	uint32_t	vector;
	uint32_t	ipl;
	uint32_t	vme_base;
	uint32_t	vme_size;
}rb_conf_t;
#endif	/* IRIX65 */

/*
 * Structure returned by the RBCTL_GSTATS call:
 */

struct rbstats {
    int			intrs;
    int			errcnt;
    unsigned int	errflags;
    int			fepracecnt;	/* Temporary, for debugging */
};


/*
 * Bitfield meanings of rbstats.errflags field:
 */

#define	RBSTAT_NODONE	0x0001
#define	RBSTAT_NOERRBIT	0x0002
#define	RBSTAT_BERR	0x0004
#define	RBSTAT_NFE	0x0008
#define	RBSTAT_CERR	0x0010
#define	RBSTAT_BARERR	0x0020
#define	RBSTAT_ERRPROB	0x0040
#define	RBSTAT_STILLARM	0x0080
#define	RBSTAT_STRAY	0x0100
#define	RBSTAT_WCNZERO	0x0200
#define	RBSTAT_FEPRACE	0x0400


#ifndef _KERNEL

/*
 * String names of above bits, for use by applications to
 * dump the bit fields.
 */

static char *rbstat_flags[11] = {
    "NODONE",
    "NOERRBIT",
    "BERR",
    "NFE",
    "CERR",
    "BARERR",
    "ERRPROB",
    "STILLARM",
    "STRAY",
    "WCNZERO",
    "FEPRACE"
};

#endif /* !_KERNEL */


/*
 * Option arguments for the RBCTL_MKERR ioctl command:
 */

#define	RB_DBOP_BERR	1
#define	RB_DBOP_BARERR	2


#endif /* ! tami_rb_rbctl_H__ */

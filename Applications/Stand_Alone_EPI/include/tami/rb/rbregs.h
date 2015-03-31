/******************************************************************************
 *  File Name:		$Source: /mplus/cvsroot/Theta/develop/src.include/rb/rbregs.h,v $  ($Revision: 1.1.1.1 $)
 *  Last Modified:	$Date: 2010/01/06 06:45:43 $
 *  Revision Log:	$Log: rbregs.h,v $
 *  Revision Log:	Revision 1.1.1.1  2010/01/06 06:45:43  ohba
 *  Revision Log:	no message
 *  Revision Log:	
 *  Revision Log:	Revision 1.2.12.1.2.3  2003/03/31 07:25:48  ohba
 *  Revision Log:	from WIPRO MAR 31
 *  Revision Log:	
 *  Revision Log:	Revision 1.3  2002/08/29 12:41:01  purandar
 *  Revision Log:	Merge changes
 *  Revision Log:	
 *  Revision Log:	Revision 1.2  2002/02/18 06:58:12  ohiwa
 *  Revision Log:	for EX octane
 *  Revision Log:	
 *  Revision Log:	Revision 1.1.14.1  2001/09/21 01:47:00  ohiwa
 *  Revision Log:	for octane RB
 *  Revision Log:	
 * Revision 1.4  93/02/19  20:35:37  park
 * Version with working CANCEL_IO function.
 * 
 * Revision 1.3  92/10/22  18:30:13  park
 * GP-50 Phase 2
 * 
 *  
 *    File Overview:
 *	This file defines macros, constants and offsets for the RB
 *	VME device registers.
 *
 *****************************************************************************/
/* Revision History:
 * yy-mm-dd	modified-by	modifications-done
 * 91-??-??	jyp		Created
 * 92-10-22	jyp		Bracketed with tami_rb_rbregs_H__
 * 00-01-17	Y.Ohiwa		for OCTANE.
 * End of History */



#ifndef tami_rb_rbregs_H__
#define tami_rb_rbregs_H__

#ifdef	IRIX65
#include <sys/sema.h>
#endif	/* IRIX65 */


/*
 * Some basic parameters of the RB.
 * We define them here (instead of in system config file) because
 * it is highly unlikely to change.  (Famous last words...)
 * In fact, it would probably require a major rewrite of this driver --
 * it is certain that simply redefining the following will not be enough.
 */
#define	RB_NUNITS	2
#define	RB_NCHANS	4
#define	RB_REGSIZE	256	/* Non-LUT space at end of RB address. */
#define	RB_FIFOSIZE	1024	/* Size of RB ping-pong fifos, in words */


/* CSR1 bits: */
/* Write: */
#define RBC_CLRERR	0x80
#define RBC_NFLUSH	0x40
	/* 0x20,0x10 not used */
#define RBC_SCH1	0x08
#define RBC_SCH0	0x04
#define RBC_DNCLR	0x02
#define RBC_ARMEN	0x01
/* Read: */
#define RBC_ERROR	0x80
#define RBC_LBERR	0x40
#define RBC_LNFE	0x20
#define RBC_LCERR	0x10
#define RBC_ARM3	0x08
#define RBC_ARM2	0x04
#define RBC_ARM1	0x02
#define RBC_ARM0	0x01

/* CSR2 bits: */
#define RBC_DONE	0x80
#define RBC_DONE3	0x40
#define RBC_DONE2	0x20
#define RBC_DONE1	0x10
#define RBC_DONE0	0x08
#define RBC_INTEN	0x04
#define RBC_ARM		0x02
#define RBC_RSRVD	0x01

/* CSR3 bits: */
#define RBC_RBRDY	0x80
#define RBC_RMRDY	0x40
#define RBC_LCH1	0x20
#define RBC_LCH0	0x10
#define RBC_DIAG	0x08
#define RBC_STEP	0x04
#define RBC_RMRST	0x02
#define RBC_HRST	0x01


/* #define	RBC_ERRBITS	(RBC_ERROR | RBC_LBERR | RBC_LCERR) */

#define	RBC_LCHSHIFT	4
#define	RBC_SCHSHIFT	2
#define	RBC_LCHTOC(csrb3)	(((csrb3)&(RBC_LCH0|RBC_LCH1)) >> RBC_LCHSHIFT)
#define	RBC_CTOSCH(n)	((n) << RBC_SCHSHIFT)
    
#define	RBC_DONEN(c)	(0x1 << ((c) + 3))
#define	RBC_ARMN(c)	(0x1 << (c))

#define	RB_WDSIZE	4
#define	RB_WDSHIFT	2
#define	RB_WTOB(w)	((w) << RB_WDSHIFT)
#define	RB_BTOW(b)	(((unsigned) (b)) >> RB_WDSHIFT)
#define	RB_WALIGNED(p)	((((unsigned) (p)) & (RB_WDSIZE - 1)) == 0x0)

#define	RB_REGMASK(val)		((val) & 0xffffff)

#define	RB_PAGESIZE	4096
#define	RB_PAGESHIFT	12

#define RB_BTOCT(addr)	(((unsigned) (addr)) >> RB_PAGESHIFT)
#define RB_CTOB(addr)	(((unsigned) (addr)) << RB_PAGESHIFT)

/*
 * Following are distinct from RB_BTOCT/RB_CTOB, in case SGI changes
 * VME->phys mapping by not placing phys. mem at VME addr. 0
 */
#define	RB_ATOLUTT(p)	(((unsigned) (p)) >> RB_PAGESHIFT)
#define	RB_LUTTOA(l)	(RB_REGMASK(l) << RB_PAGESHIFT)


#define RB_DEVTOUNIT(min)	((min) / (RB_NCHANS * 2))
#define RB_DEVTOCHAN(min)	((min) & (RB_NCHANS - 1))
#define RB_ISCHANCTL(min)	(((min) & (RB_NCHANS)) != 0)


#ifndef TRUE
#define TRUE	1
#define FALSE	0
#endif


struct chan_regs {
    unsigned int	wcnt;
    unsigned int	bar;
};

struct wordbytes {
    unsigned char	b0, b1, b2, b3;
};

struct rbregs {
    struct chan_regs	chan[RB_NCHANS];
    unsigned int	scratch[6];
    unsigned int	intvec;
    struct wordbytes	csr;
};

struct rblutspan {
    struct rblutspan	*prev;
    struct rblutspan	*next;
    unsigned int	start;
    unsigned int	size;
    int			inuse;
};

/*
 * This is the limit for necessary free/alloc list elements under
 * dynamic load.  Notice: static example needing 2n+1 will never form.
 */
#define	RB_NLUTFREE	(2 * RB_NCHANS)


struct chaninfo {
    unsigned int	status;
    int			nrdopen;
    int			nctlopen;
    struct rblutspan	*lutspan;
    sema_t		chansema;	/* For sequencing actual I/O */
#ifndef	IRIX65
    lock_t		chaninfolock;	/* For critical updates to struct */
    lock_t		chandevlock;	/*  "    "         "    "  dev */
#endif	/* IRIX65 */
    unsigned char	ctlopenrw;
    struct buf		*curbp;
    int			dboptype;
};

#define	RB_CHAN_OK	0x01
#define	RB_CHAN_BUSY	0x02

/*
 * Currently, code in the driver has been designed, or shown to be,
 * structured so that there should be no examine-then-set race conditions,
 * with any fields of any globals.  In some cases, this is with the
 * help of locks and/or semaphores.
 * If the code is restructured so that this is not so, the globals, esp.
 * rbinfo, must be examined to see if more of the subset of fields must
 * be declared "volatile".  We're being conservative with membership
 * due to the assumption that "volatiles" will slow down the code.
 */


struct rbinfo {
    volatile unsigned int	*lut;
    volatile unsigned int	*lutend;
    volatile struct rbregs	*regs;
#ifdef	IRIX65
    uint32_t			base;
    uint32_t			size;
    uint32_t			lutsize;
#endif
    unsigned char		intvec;
#ifdef	IRIX65
    uint32_t			ipl;
    uint32_t			vme_base;
    uint32_t			vme_size;
#endif
    unsigned int		maxbar;
    struct chaninfo		chaninfo[RB_NCHANS];
    unsigned int		rbstat;
#ifdef	IRIX65
    mutex_t			rbcsrlock;
#else
    lock_t			rbcsrlock;
    lock_t			rblutlock;
    struct rblutspan		*rblutspan;
    struct rblutspan		*rblutspanfree;
#endif
    int				suspended_chan;
    struct rbstats		stats;
};


#define	RBS_NO_BOARD	0x00
#define	RBS_DEAD	0x01
#define	RBS_READY	0x02
#define	RBS_INIT	0x03

/* In microseconds: */
#define RB_INIT_TIMEOUT	500000
#define RB_INIT_TIMEOUT_LIMIT	500000

/* In microseconds: */
#define RM_INIT_TIMEOUT	100000
#define RM_INIT_TIMEOUT_LIMIT	6400000


#ifdef _KERNEL

/* GLOBALS: */
struct rbinfo		rbinfo[RB_NUNITS];
#ifndef	IRIX65
struct rblutspan	rblutspanarray[RB_NUNITS][RB_NLUTFREE];
#endif	/* IRIX65 */
unsigned int		rbboardstatus[RB_NUNITS] = {RBS_NO_BOARD,RBS_NO_BOARD};

#endif /* _KERNEL */

#endif /* ! tami_rb_rbregs_H__ */

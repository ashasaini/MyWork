/******************************************************************************
**
**      Copyright (c) 1995 by TOSHIBA Corporation. All Rights Reserved.
**
**      Filename:       rb_conf.h
**
**      Purpose:        Include file for RB
**
**      Revision:       1.0     S.IKEDA     1995.12.12     original
**      Revision:       1.01    Y.Ohiwa     1996.05.23     add VME address information
**
******************************************************************************/

/*
 * Some basic parameters of the RB.
 * We define them here (instead of in system config file) because
 * it is highly unlikely to change.  (Famous last words...)
 * In fact, it would probably require a major rewrite of this driver --
 * it is certain that simply redefining the following will not be enough.
 */
#define	RB_NUNITS	8
#define RB_NCHANS       4
#define RB_REGSIZE      256     /* Non-LUT space at end of RB address. */
#define RB_FIFOSIZE     1024    /* Size of RB ping-pong fifos, in words */

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
#define RBCTL_GET_MMAP	9

/*
 * Bitfield meanings of rbstats.errflags field:
 */

#define	RBSTAT_NODONE		0x0001
#define	RBSTAT_NOERRBIT		0x0002
#define	RBSTAT_BERR		0x0004
#define	RBSTAT_NFE		0x0008
#define	RBSTAT_CERR		0x0010
#define	RBSTAT_BARERR		0x0020
#define	RBSTAT_ERRPROB		0x0040
#define	RBSTAT_STILLARM		0x0080
#define	RBSTAT_STRAY		0x0100
#define	RBSTAT_WCNZERO		0x0200
#define	RBSTAT_FEPRACE		0x0400
#define	RBSTAT_EIO		0x0800
#define RBSTAT_CANCELIO		0x1000

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


#define	RB_SLEEP_INTERVAL   	500000  /* usec */
#define	RB_SLEEP_RETRY     	10
#define	PROBE_SIZE		1024

#define	NORMAL			0	/* Normal status */

#define RBC_LCHSHIFT    4
#define RBC_SCHSHIFT    2
#define RBC_LCHTOC(csrb3)       (((csrb3)&(RBC_LCH0|RBC_LCH1)) >> RBC_LCHSHIFT)
#define RBC_CTOSCH(n)   ((n) << RBC_SCHSHIFT)

#define RBC_DONEN(c)    (0x1 << ((c) + 3))
#define RBC_ARMN(c)     (0x1 << (c))

#define	RB_REGMASK(val)		((val) & 0xffffff)

#define	RB_PAGESIZE	4096
#define	RB_PAGESHIFT	12

#define RB_WDSIZE       4
#define RB_WDSHIFT      2
#define RB_WTOB(w)      ((w) << RB_WDSHIFT)
#define RB_BTOW(b)      (((unsigned) (b)) >> RB_WDSHIFT)
#define RB_WALIGNED(p)  ((((unsigned) (p)) & (RB_WDSIZE - 1)) == 0x0)

#define RB_BTOCT(addr)	(((unsigned) (addr)) >> RB_PAGESHIFT)
#define RB_CTOB(addr)	(((unsigned) (addr)) << RB_PAGESHIFT)

/*
 * Following are distinct from RB_BTOCT/RB_CTOB, in case SGI changes
 * VME->phys mapping by not placing phys. mem at VME addr. 0
 */
#define	RB_ATOLUTT(p)	(((unsigned) (p)) >> RB_PAGESHIFT)
#define	RB_LUTTOA(l)	(RB_REGMASK(l) << RB_PAGESHIFT)

#define	RB_DEVTOUNIT(min)       ((min) / (RB_NCHANS * 2))
#define RB_DEVTOCHAN(min)       ((min) & (RB_NCHANS - 1))
#define RB_ISCHANCTL(min)       (((min) & (RB_NCHANS)) != 0)

#define RB_LUT_SIZE             0x7c00
#define	RB_REGS_OFFSET		0x7fc0
#define	USESIG1			SIGUSR1
#define	USESIG2			SIGUSR2
#define	LUT_MASK		0x00ffffff
#define	UPPER_MASK		0x01	/* for bebug */
#define	CSR_UNIT_SIZE		8	/* for debug */

#define	IOCTL_READY		1
#define	IOCTL_NOTREADY		0
#define	INTR_DEV		"/dev/rbint"

#define RB_CHAN_OK      0x01
#define RB_CHAN_BUSY    0x02

struct chan_regs {
    unsigned long	wcnt;
    unsigned long	bar;
};

struct wordbytes {
    unsigned char	b0, b1, b2, b3;
};

struct rbregs {
    struct chan_regs	chan[RB_NCHANS];
    unsigned long	scratch[6];
    unsigned long	intvec;
    struct wordbytes	csr;
};

typedef	struct {
	unsigned long	max_cnt;
	unsigned long	cnt;
	unsigned long	break_flag;
}rb_lock_t;

#define	RB_LOCK_SHM_KEY	0x1111
#define	RB_LOCK_SEM_KEY	0x2222

/***********************************************************************
**
**      Copyright (c) 1996 by TOSHIBA Corporation. All Rights Reserved.
**
**      Filename:       rbint_ctl.h
**
**      Purpose:        Include file for RB's interrupt driver.
**
**      Revision:       1.00     T.Tanaka     1996.01.19     original
**      Revision:       1.01     Y.Ohiwa      1996.06.05     add semaphore command
**      Revision:       1.02     Y.Ohiwa      1998.09.09     add RM wait intr timeout
**
***********************************************************************/

/*********************************************************** 
**
**	command code for IOCTL of RB's interrupt driver 
**
***********************************************************/

#define	RB_CLR_INTR		0x1	/* Clear interrupt */
#define	RB_GET_INTR		0x2	/* Get interrupt */
#define	RB_CAN_INTR		0x3	/* Cancel interrupt waiting */
#define	RB_DEV_LOCK		0x4	/* Lock device of RB's */
#define	RB_DEV_FREE		0x5	/* Free device of RB's */
#define	RB_SET_INIT_CONF 	0x6	/* Set rb_config */
#define	RB_GET_INIT_CONF 	0x7	/* Get rb_config */
#define	RB_SET_ACTION_CONF 	0x8	/* Set rb_comdata */
#define	RB_GET_ACTION_CONF 	0x9	/* Get rb_comdata */

/**********************************************************
**
**	structure for RB configration 
**
***********************************************************/

typedef struct {
    int			intrs;
    int			errcnt;
    unsigned int	errflags;
    int			fepracecnt;	/* Temporary, for debugging */
} rbstats_t;

typedef struct {
        unsigned long   base;
        unsigned long   size;
        long            vector;
        long            ipl;
        struct {
                unsigned long   base;
                unsigned long   size;
        }vmebuf[RB_NCHANS];
} rb_config_t;

typedef struct{
	int	   suspended_chan;
        rbstats_t  stats[RB_NCHANS];
        int        dboptype[RB_NCHANS];
        int        chan_busy[RB_NCHANS];
} rb_comdata_t;

/*********************************************************** 
**
**	structure for RB's unit, cannel (and error status)
**
***********************************************************/

typedef struct {
    int unit;		/* request unit number */
    int chan;		/* request channel number */
} rb_uch_t;

typedef struct {
    int 	  unit;		/* request unit number */
    int 	  chan;		/* request channel number */
    unsigned long err_stat;	/* error status (return status) */
} rb_uchs_t;

typedef struct {
    int 	  unit;		/* request unit number */
    int 	  chan;		/* request channel number(dummy only 0) */
    unsigned long base;		/* RB base address */
    unsigned long size;		/* RB size */
    long	  vector;	/* RB vector */
    long	  ipl;		/* RB ipl */
    struct {
    	unsigned long base;	/* VME buffer base address  */
    	unsigned long size;	/* VME buffer memory size  */
    }vmebuf[RB_NCHANS];
} rb_uchconfig_t;

typedef	struct{
    int		unit;		/* request unit number */
    int		chan;		/* request channel number(dummy only 0) */
    int		suspended_chan;
    rbstats_t	stats[RB_NCHANS];
    int		dboptype[RB_NCHANS];
    int		chan_busy[RB_NCHANS];
} rb_uchcomdata_t;

typedef struct {
    int 	unit;		/* request unit number */
    int 	chan;		/* request channel number */
    long 	timeout;	/* timeout */
} rb_uwait_t;

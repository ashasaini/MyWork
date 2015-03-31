/*********************************************************************
 *
 *  (C) Copyright 1998 TOSHIBA Corporation All Rights Reserved
 *
 *  File Name:       $Source: /mplus/cvsroot/Theta/develop/src.include/librbvap/rbvap.h,v $  ($Revision: 1.2 $)
 *  Last Modified:   $Date: 2010/02/17 02:32:29 $
 *  Revision Log:    $Log: rbvap.h,v $
 *  Revision Log:    Revision 1.2  2010/02/17 02:32:29  karube
 *  Revision Log:    MCM0252-00020 Phase3 libWNP,Native
 *  Revision Log:
 *  Revision Log:    Revision 1.1.1.1  2010/01/06 06:45:42  ohba
 *  Revision Log:    no message
 *  Revision Log:
 *  Revision Log:    Revision 1.1.28.1.2.3.64.1  2009/02/02 08:37:20  ohiwa
 *  Revision Log:    MCM0229-00010 RBVAP_NFILE is FD_SETSIZE
 *  Revision Log:
 *  Revision Log:    Revision 1.1.28.1.2.3.96.1  2008/12/16 10:41:12  ohiwa
 *  Revision Log:    MCM0228-00030 RBVAP_NFILE is FD_SETSIZE
 *  Revision Log:
 *  Revision Log:    Revision 1.1.28.1.2.3  2003/03/31 07:25:35  ohba
 *  Revision Log:    from WIPRO MAR 31
 *  Revision Log:
 *  Revision Log:    Revision 1.1.1.1  2002/04/16 07:14:43  cvsadmin
 *  Revision Log:    Imported
 *  Revision Log:
 *  Revision Log:    Revision 1.1  2000/06/07 08:49:00  ohiwa
 *  Revision Log:    create librbvap
 *  Revision Log:
 *  Revision Log:    Revision 1.1.1.1  2000/04/21 08:59:19  ohba
 *  Revision Log:    From Persia
 *  Revision Log:
 *
 *    File Overview:
 *      This file defines routines for...
 *
 *    Objects with external linkage defined in this file:
 *
 *********************************************************************/
/* Revision History:
 * Sep.24.1998  H.Tsurumaki  Created
 * Jul.02.1999  H.Tsurumaki  Added define RBVAP_NFILE
 * End of History */

#ifndef	__librbvap_h__
#define	__librbvap_h__

#ifdef __cplusplus
extern "C" {
#endif

#include <tami/common/basicdefs.h>

#ifdef __FREE_BSD__
#define RBVAP_NFILE FD_SETSIZE
#else
#ifndef RBVAP_NFILE
#ifdef _NFILE
#define	RBVAP_NFILE _NFILE
#endif
#endif

#ifndef RBVAP_NFILE
#ifdef FOPEN_MAX
#define RBVAP_NFILE FOPEN_MAX
#endif
#endif

#ifndef RBVAP_NFILE
#ifdef FD_SETSIZE
#define RBVAP_NFILE FD_SETSIZE
#endif
#endif

#ifndef RBVAP_NFILE
#define	RBVAP_NFILE 256
#endif
#endif /* __FREE_BSD__ */

extern long RBVAPerrcode;

enum {
	UNKNOWN_DEVICE	= 0,
	RB_DEVICE	,
	SHC_DEVICE	,
	VAP_DEVICE	,
	NumOfWhichDeviceRBVAP
};

typedef	struct {
	int	which_device;
}   rbvap_info_t;

extern	int	RBVAPopen (char *path, int oflag);
extern	int	RBVAPjob (int fd, uint32_t jobid);
extern	int	RBVAPclose (int fd);
extern	int	RBVAPread (int fd, void *buf, int len);
extern	int	RBVAPioctl (int fd, int cmd, int arg);
extern	int	RBVAPinfo (int fd, rbvap_info_t *infop);

extern	long	RBVAPerrcode;
extern	int 	RBVAPdemo;

#ifdef __cplusplus
} // extern "C"
#endif

#endif	/* __librbvap_h__ */

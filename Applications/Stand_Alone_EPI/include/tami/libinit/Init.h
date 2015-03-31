/*********************************************************************
 *
 *  (c) Copyright 1996 TOSHIBA Corporation All Rights Reserved
 *
 *  File Name:       $Source: /mplus/cvsroot/Theta/develop/src.include/libinit/Init.h,v $  ($Revision: 1.2 $)
 *  Last Modified:   $Date: 2010/02/17 02:30:25 $
 *  Revision Log:    $Log: Init.h,v $
 *  Revision Log:    Revision 1.2  2010/02/17 02:30:25  karube
 *  Revision Log:    MCM0252-00020 Phase3 libWNP,Native
 *  Revision Log:
 *  Revision Log:    Revision 1.1.1.1  2010/01/06 06:45:42  ohba
 *  Revision Log:    no message
 *  Revision Log:
 *  Revision Log:    Revision 1.2.12.1.2.3.64.1  2008/01/25 04:56:07  takab
 *  Revision Log:    MCM0209-00007 add InitGetDispType
 *  Revision Log:
 *  Revision Log:    Revision 1.2.12.1.2.3  2003/03/31 07:25:28  ohba
 *  Revision Log:    from WIPRO MAR 31
 *  Revision Log:
 *  Revision Log:    Revision 1.2  2002/08/07 07:56:53  purandar
 *  Revision Log:    Merging new code with ported code
 *  Revision Log:
 *  Revision Log:    Revision 1.2  2002/05/30 23:59:31  full
 *  Revision Log:    for new libSAR
 *  Revision Log:
 *  Revision Log:    Revision 1.1.1.1  2000/06/08 11:01:54  full
 *  Revision Log:    Change Include Path
 *  Revision Log:
 *  Revision Log:    Revision 1.1.1.1  2000/04/21 08:59:19  ohba
 *  Revision Log:    From Persia
 *  Revision Log:
 *
 *    File Overview:
 *      This file defines ...
 *
 *********************************************************************/
/* Revision History:
 * May.16.96	Naoyuki FURUDATE	Created
 * End of History */



#ifndef Init_H__

typedef struct {
    int32_t	NumChannel;
} InitAcqInfo_t, *InitAcqInfo_s;

#define Init_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <tami/initmgr/common.h>

/*******************************************
 * 	File : common.c
 *******************************************/
int		InitCreateCommon	(Common_t **);
int		InitOpenCommon		(Common_t **);
status_t	InitDestroyCommon	(void);
status_t	InitCloseCommon		(Common_t *);

/*******************************************
 * 	File : fileman.c
 *******************************************/
status_t	InitGetFilemanBusy	(int *);
status_t	InitSetFilemanBusy	(int);

/*******************************************
 * 	File : smconf.c
 *******************************************/
status_t	InitReadSmConf		(FILE *,Common_t *);
status_t	InitWriteSmConf		(FILE *,Common_t *);
status_t	InitGetAppCommon	(AppCommon_t **);
status_t	InitReadSmAdd		(FILE *,char **);
char *		InitGetSysName		(int);
int		InitGetSysId		(char *);
char *		InitGetAreaName		(int);
#ifdef __WIN_NATIVE_PORTING__
int			InitGetAreaId		(char *);
#else
int		InitGetAreaCode		(char *);
#endif
/*******************************************
 * 	File : system_type.c
 *******************************************/
status_t	InitGetSystemType	(int *);
status_t	InitGetVersion		(char *,char *);
status_t	InitGetCpuType		(int *);
#ifdef __WIN_NATIVE_PORTING__
status_t	InitGetAreaCode		(int *);
#else
status_t	InitGetAreaId		(int *);
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* Init_H__ */


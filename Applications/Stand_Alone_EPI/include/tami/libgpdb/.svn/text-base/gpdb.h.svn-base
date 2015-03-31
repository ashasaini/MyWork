/*********************************************************************
 *
 *  (c) Copyright 1994 TOSHIBA Corporation All Rights Reserved
 *
 *  File Name:       $Source: /mplus/cvsroot/Theta/develop/src.include/libgpdb/gpdb.h,v $  ($Revision: 1.2 $)
 *  Last Modified:   $Date: 2010/02/17 02:30:13 $
 *  Revision Log:    $Log: gpdb.h,v $
 *  Revision Log:    Revision 1.2  2010/02/17 02:30:13  karube
 *  Revision Log:    MCM0252-00020 Phase3 libWNP,Native
 *  Revision Log:
 *  Revision Log:    Revision 1.1.1.1  2010/01/06 06:45:42  ohba
 *  Revision Log:    no message
 *  Revision Log:
 *  Revision Log:    Revision 1.2.22.1.2.3  2003/03/31 07:25:27  ohba
 *  Revision Log:    from WIPRO MAR 31
 *  Revision Log:
 *  Revision Log:    Revision 1.3  2002/08/07 07:56:01  purandar
 *  Revision Log:    Merging new code with ported code
 *  Revision Log:
 *  Revision Log:    Revision 1.2  2001/02/01 12:09:34  full
 *  Revision Log:    Add PathDB and AjustF1F2 Param
 *  Revision Log:
 *  Revision Log:    Revision 1.1  2000/05/17 09:03:15  tel
 *  Revision Log:    *** empty log message ***
 *  Revision Log:
 *
 *    File Overview:
 *      This file defines ...
 *
 *********************************************************************/
/* Revision History:
 * Nov. 9.94	Naoyuki FURUDATE	Created
 * End of History */



#ifndef libgpdb_gpdb_H__
#define libgpdb_gpdb_H__

#ifdef __cplusplus
extern "C" {
#endif

status_t	GpdbGetFilePath			(char *,char **,char **);

typedef enum gpdb_on_off_e {
    GPDB_OFF,
    GPDB_ON
    } gpdb_on_off_t;

typedef enum gpdb_mode_e {
    GPDB_MODE_INSERT = 1,
    GPDB_MODE_REPLACE
    } gpdb_mode_t;

typedef void *gpdb_handle_p;
typedef char *gpdb_descr_t;

/*******************************************
 *      File : gpdb.c
 *******************************************/
gpdb_handle_p	GPDBOpen		(char *,char *,int);
status_t	GPDBClose		(gpdb_handle_p);

#ifdef USE_GPDB_PACK
status_t	GPDBPackData		(void *,void *,
					 void (*)(void *,datum *),
                                         int,gpdb_descr_t,int,
                                         datum *,datum *,datum *);
status_t	GPDBUnpackData		(datum *,datum *,bool_t,void *,
                                         int,gpdb_descr_t *,int *);
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* libgpdb_gpdb_H__ */

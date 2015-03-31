/*********************************************************************
 *
 *  (c) Copyright 1994 TOSHIBA Corporation All Rights Reserved
 *
 *  File Name:       $Source: /mplus/cvsroot/Theta/develop/src.include/libgpdb/interlock.h,v $  ($Revision: 1.2 $)
 *  Last Modified:   $Date: 2010/02/17 02:30:13 $
 *  Revision Log:    $Log: interlock.h,v $
 *  Revision Log:    Revision 1.2  2010/02/17 02:30:13  karube
 *  Revision Log:    MCM0252-00020 Phase3 libWNP,Native
 *  Revision Log:
 *  Revision Log:    Revision 1.1.1.1  2010/01/06 06:45:42  ohba
 *  Revision Log:    no message
 *  Revision Log:
 *  Revision Log:    Revision 1.1.28.1.2.3.56.1.2.1  2008/01/10 08:13:35  koku
 *  Revision Log:    MCM0209-00063 : GPDB for V9.25
 *  Revision Log:
 *  Revision Log:    Revision 1.1.28.1.2.3.56.1  2006/06/20 12:43:44  matsuo
 *  Revision Log:    MCM0177-00001 Recon Engine porting to FreeBSD-64Bit
 *  Revision Log:
 *  Revision Log:    Revision 1.1.28.1.2.3  2003/03/31 07:25:27  ohba
 *  Revision Log:    from WIPRO MAR 31
 *  Revision Log:
 *  Revision Log:    Revision 1.3  2002/08/07 07:56:03  purandar
 *  Revision Log:    Merging new code with ported code
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
 * Oct.27.94	Naoyuki FURUDATE	Created from filter.h
 * Dec.19.2007	K.Kokubun               added for IDB_RF_ACTION_LIMIT_KEY,IDB_RF_DUTY_KEY
 * End of History */



#ifndef libgpdb_interlock_H__
#define libgpdb_interlock_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <tami/libgpdb/gpdb.h>

#define IDB_DB_NAME		"interlockDB"
#define IDB_RF_MAX_KEY		0x1000

/*** Added for V9.25 K.Kokubun     ***/
#define IDB_RF_ACTION_LIMIT_KEY	0x2000
#define IDB_RF_POWER_DUTY_KEY	0x2001
#define IDB_F2_FIX_KEY	        0x2002

/* data-structures and data-types */

typedef void 	*idb_handle_p;
#ifdef __FREE_BSD__
typedef uint64_t idb_db_key_t;
#else
typedef int32_t  idb_db_key_t;
#endif

typedef enum idb_keys_e {
    IDB_KEY_TYPE        = 1,
    IDB_KEY_INSTALLED,
    IDB_KEY_ID,
    IDB_KEY_DESCR,
    IDB_KEY_SORT_ORDER
    } idb_keys_t;

typedef enum idb_on_off_e {
    IDB_OFF,
    IDB_ON
    } idb_on_off_t;

typedef enum idb_interlock_type_e {
    IDB_INTERLOCK = 1,
    IDB_WEIGHT_RATIO,
    IDB_RF_MAX,

    /* added for V9.25 K.Kokubun */
    IDB_RF_ACTION_LIMIT,  
    IDB_RF_POWER_DUTY,
    IDB_F2_FIX
    } idb_type_t;

typedef enum idb_mode_e {
    IDB_MODE_INSERT = 1,
    IDB_MODE_REPLACE
    } idb_mode_t;

typedef char *idb_descr_t;


typedef struct idb_rec_s *idb_rec_p;
typedef struct idb_rec_s {
    int			interlock_id;
    idb_type_t		type;
    idb_on_off_t        is_installed;

    unsigned long	value[6];
    unsigned long	max_value;
    unsigned long	cf_max;

    int			sort_order;
    int			len_of_descr;
    idb_descr_t		description;
} idb_rec_t;

#define IDB_VERSION_KEY		"_version"

/* Error Severity Constants */

/* function prototypes */

idb_handle_p	IdbOpen		(char *,char *,int);
status_t	IdbClose	(idb_handle_p);
status_t	IdbGet		(idb_handle_p,void *,idb_rec_p);
status_t	IdbGetAll	(idb_handle_p,void (*)(void *,idb_rec_p));
status_t	IdbSearch	(idb_handle_p,void (*)(void *,idb_rec_p),...);
status_t	IdbPut		(idb_handle_p,idb_mode_t,void *,idb_rec_p);
status_t	IdbDelete	(idb_handle_p,void *);

/* MACRO definitions */

#define IDB_REC_KEY(type,id)		((type) << 16 | (id) & 0x0000ffff)
#define IDB_DECODE_KEY(key,type,id)     \
    (type) = (key) >> 16; \
    (id) = 0x0000ffff & (key)

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* libgpdb_interlock_H__ */


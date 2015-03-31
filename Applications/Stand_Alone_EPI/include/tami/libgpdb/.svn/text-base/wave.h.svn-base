/*********************************************************************
 *
 *  (c) Copyright 1994 TOSHIBA Corporation All Rights Reserved
 *
 *  File Name:       $Source: /mplus/cvsroot/Theta/develop/src.include/libgpdb/wave.h,v $  ($Revision: 1.3 $)
 *  Last Modified:   $Date: 2010/03/23 01:25:35 $
 *  Revision Log:    $Log: wave.h,v $
 *  Revision Log:    Revision 1.3  2010/03/23 01:25:35  sagae
 *  Revision Log:    MCM0252-00104
 *  Revision Log:    Waveform for adding new types.
 *  Revision Log:
 *  Revision Log:    Revision 1.2  2010/02/17 02:30:13  karube
 *  Revision Log:    MCM0252-00020 Phase3 libWNP,Native
 *  Revision Log:
 *  Revision Log:    Revision 1.1.1.1  2010/01/06 06:45:42  ohba
 *  Revision Log:    no message
 *  Revision Log:
 *  Revision Log:    Revision 1.1.28.1.2.3.56.1  2006/06/20 12:43:44  matsuo
 *  Revision Log:    MCM0177-00001 Recon Engine porting to FreeBSD-64Bit
 *  Revision Log:
 *  Revision Log:    Revision 1.1.28.1.2.3  2003/03/31 07:25:27  ohba
 *  Revision Log:    from WIPRO MAR 31
 *  Revision Log:
 *  Revision Log:    Revision 1.3  2002/08/07 07:56:10  purandar
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
 * End of History */



#ifndef libgpdb_wave_H__
#define libgpdb_wave_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <tami/libgpdb/gpdb.h>

#define WDB_DB_NAME	"waveDB"

/* data-structures and data-types */

typedef void 	*wdb_handle_p;
#ifdef __FREE_BSD__
typedef uint64_t wdb_db_key_t;
#else
typedef int32_t  wdb_db_key_t;
#endif

typedef enum wdb_keys_e {
    WDB_KEY_TYPE        = 1,
    WDB_KEY_INSTALLED,
    WDB_KEY_ID,
    WDB_KEY_DESCR,
    WDB_KEY_SORT_ORDER
    } wdb_keys_t;

typedef enum wdb_on_off_e {
    WDB_OFF,
    WDB_ON
    } wdb_on_off_t;

typedef enum wdb_wave_type_e {
    WDB_TYPE_SINC = 1,
    WDB_TYPE_SINC_OPT,
    WDB_TYPE_SINC_MADO,
    WDB_TYPE_FE_I,
    WDB_TYPE_FE_Q,
    WDB_TYPE_FLIP_I,
    WDB_TYPE_FLIP_Q,
    WDB_TYPE_FLOP_I,
    WDB_TYPE_FLOP_Q,
    WDB_TYPE_P,
    WDB_TYPE_NSEP,
    WDB_TYPE_HS,
    WDB_TYPE_ZERO,
    WDB_TYPE_EXCITE_AMPM,
    WDB_TYPE_REFOCUS_AMPM,
    WDB_TYPE_FUNCTION_AMPM,
    WDB_TYPE_EXCITE_FIR,
    WDB_TYPE_REFOCUS_FIR,
    WDB_TYPE_FUNCTION_FIR
    } wdb_type_t;

typedef enum wdb_mode_e {
    WDB_MODE_INSERT = 1,
    WDB_MODE_REPLACE
    } wdb_mode_t;

typedef char *wdb_descr_t;

typedef struct wdb_rec_s *wdb_rec_p;
typedef struct wdb_rec_s {
    int			wave_id;
    wdb_type_t		type;
    wdb_on_off_t        is_installed;
    
    char		wave_name[32];
    double		power;
    
    int			sort_order;
    
    int			len_of_descr;
    wdb_descr_t		description;
} wdb_rec_t;

#define WDB_VERSION_KEY		"_version"

/* Error Severity Constants */

/* function prototypes */

wdb_handle_p	WdbOpen		(char *,char *,int);
status_t	WdbClose	(wdb_handle_p);
status_t	WdbGet		(wdb_handle_p,void *,wdb_rec_p);
status_t	WdbGetAll	(wdb_handle_p,void (*)(void *,wdb_rec_p));
status_t	WdbSearch	(wdb_handle_p,void (*)(void *,wdb_rec_p),...);
status_t	WdbPut		(wdb_handle_p,wdb_mode_t,void *,wdb_rec_p);
status_t	WdbDelete	(wdb_handle_p,void *);

/* MACRO definitions */

#define WDB_REC_KEY(type,id)		((type) << 16 | (id) & 0x0000ffff)
#define WDB_DECODE_KEY(key,type,id)     \
    (type) = (key) >> 16; \
    (id) = 0x0000ffff & (key)

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* libgpdb_wave_H__ */


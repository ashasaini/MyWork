/*********************************************************************
 *
 *  (C) Copyright 1995 TOSHIBA Corporation All Rights Reserved
 *
 *  File Name:       $Source: /mplus/cvsroot/Theta/develop/src.include/libgpdb/rfl.h,v $  ($Revision: 1.2 $)
 *  Last Modified:   $Date: 2010/02/17 02:30:13 $
 *  Revision Log:    $Log: rfl.h,v $
 *  Revision Log:    Revision 1.2  2010/02/17 02:30:13  karube
 *  Revision Log:    MCM0252-00020 Phase3 libWNP,Native
 *  Revision Log:
 *  Revision Log:    Revision 1.1.1.1  2010/01/06 06:45:42  ohba
 *  Revision Log:    no message
 *  Revision Log:
 *  Revision Log:    Revision 1.1.28.1.2.3.56.1.2.1  2009/01/12 07:00:13  hanaki
 *  Revision Log:    MCM0229-00093 WB_yaps
 *  Revision Log:
 *  Revision Log:    Revision 1.1.28.1.2.3.56.1  2006/06/20 12:43:44  matsuo
 *  Revision Log:    MCM0177-00001 Recon Engine porting to FreeBSD-64Bit
 *  Revision Log:
 *  Revision Log:    Revision 1.1.28.1.2.3  2003/03/31 07:25:27  ohba
 *  Revision Log:    from WIPRO MAR 31
 *  Revision Log:
 *  Revision Log:    Revision 1.3  2002/08/07 07:56:06  purandar
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
 * Aug.07.95	Masakatsu OGIWARA	Created from interlock.h
 * End of History */



#ifndef libgpdb_rfl_H__
#define libgpdb_rfl_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <tami/libgpdb/gpdb.h>

#define RDB_DB_NAME		"rflDB"

/* data-structures and data-types */

typedef void 			*rdb_handle_p;
#ifdef __FREE_BSD__
typedef uint64_t		rdb_db_key_t;
#else
typedef int32_t			rdb_db_key_t;
#endif

typedef enum rdb_keys_e {
    RDB_KEY_PORT	= 1,
    RDB_KEY_COIL_ID,
    RDB_KEY_REGION_ID,
    RDB_KEY_CHANNEL,
    RDB_KEY_INSTALLED,
    RDB_KEY_DESCR,
    RDB_KEY_SORT_ORDER
    } rdb_keys_t;

typedef enum rdb_on_off_e {
    RDB_OFF,
    RDB_ON
    } rdb_on_off_t;

typedef enum rdb_coil_port_e {
    RDB_PORT_A = 'A',
    RDB_PORT_B = 'B',
    RDB_PORT_C = 'C',
    RDB_PORT_D = 'D',
    RDB_PORT_E = 'E',
    RDB_PORT_F = 'F'
    } rdb_coil_port_t;

typedef enum rdb_channels_e {
    RDB_CHANNEL_A = 'A',
    RDB_CHANNEL_B = 'B',
    RDB_CHANNEL_C = 'C',
    RDB_CHANNEL_L = 'L',
    RDB_CHANNEL_M = 'M',
    RDB_CHANNEL_X = 'X',
    RDB_CHANNEL_Y = 'Y',
    RDB_CHANNEL_Z = 'Z' 
    } rdb_channels_t;

typedef enum rdb_mode_e {
    RDB_MODE_INSERT = 1,
    RDB_MODE_REPLACE
    } rdb_mode_t;

typedef char 		*rdb_descr_t;

typedef struct rdb_rec_s 	*rdb_rec_p;
typedef struct rdb_rec_s {			/* content of a record in the rfl DB */
    rdb_coil_port_t		port;		/* port is also key to record */
    int				coil_id;	/* coil id is also key to record */
    int				region_id;	/* resion id is also key to record */
    rdb_channels_t		channel;	/* channel is also key to record */
    rdb_on_off_t        	is_installed;
    
    flt32_t			slice_thickness;
    flt32_t			slice_gap;
    flt32_t			field_of_view;
    int				num_slices;
    int				step_value;
    int				wb_yaps;			/* V9.50 */
    int				axis;		/* V9.30 */
#ifdef __WNP_MPLUS__
    int				B1ch;		/* Venus Ph.1  2ch transmit power */
    int				B1Amp;		/* Venus Ph.1  2ch transmit power */
    int				B1Phase;	/* Venus Ph.1  2ch transmit power */
#endif
    
    
    int				sort_order;
    
    int				len_of_descr;
    rdb_descr_t			description;

} rdb_rec_t;

#define RDB_VERSION_KEY		"_version"

/* Error Severity Constants */

/* function prototypes */

rdb_handle_p	RdbOpen		(char *,char *,int);
status_t	RdbClose	(rdb_handle_p);
status_t	RdbGet		(rdb_handle_p,void *,rdb_rec_p);
status_t	RdbGetAll	(rdb_handle_p,void (*)(void *,rdb_rec_p));
status_t	RdbSearch	(rdb_handle_p,void (*)(void *,rdb_rec_p),...);
status_t	RdbPut		(rdb_handle_p,rdb_mode_t,void *,rdb_rec_p);
status_t	RdbDelete	(rdb_handle_p,void *);

/* MACRO definitions */

#define RDB_REC_KEY(port, channel, coil_id, region_id)	( ((((((port) << 8) | (channel)) << 8) | (coil_id)) << 8) | (region_id) )
#define RDB_DECODE_KEY(rdb_rec_key, port, channel, coil_id, region_id)     \
    (port)      = (rdb_rec_key) >> 24; \
    (channel)   = (rdb_rec_key) >> 16 & 0x00ff; \
    (coil_id)   = (rdb_rec_key) >> 8  & 0x00ff; \
    (region_id) = 0x00ff & (rdb_rec_key)

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* libgpdb_rfl_H__ */


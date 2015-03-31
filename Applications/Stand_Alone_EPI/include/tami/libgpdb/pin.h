/***********************************************************************
 *
 *  (c) Copyright 2002 TOSHIBA Corporation All Rights Reserved
 *
 *    File Overview:
 *      This file defines ...
 *
 ***********************************************************************/
/* Revision History:
 * Jul.07.2006  K.Kokubun                  Created for SATURN
 * 
 * End of History */

#ifndef pin_H__
#define pin_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __NUTC__
#include <tami/common/machdefs.h>
#endif
#include <tami/libgpdb/gpdb.h>
#include <tami/libgpdb/coil.h>

#define PIN_DB_NAME		"pinDB"

typedef void    *pindb_handle_p;
typedef int32_t	pindb_key_t;

typedef enum pindb_keys_e {
  PIN_DB_KEY_TYPE	= 1,
  PIN_DB_KEY_INSTALLED,
  PIN_DB_KEY_ID,
  PIN_DB_KEY_CHANNEL,
  PIN_DB_KEY_PORT,
  PIN_DB_KEY_SID,
  PIN_DB_KEY_SIG,
  PIN_DB_KEY_HYBSIG,
  PIN_DB_KEY_BLOCKNO,
  PIN_DB_KEY_NUMDRIVE,
  PIN_DB_KEY_PINDEF,
  PIN_DB_KEY_RF_ON,
  PIN_DB_KEY_RF_OFF,
  PIN_DB_KEY_DESCR,
  PIN_DB_KEY_SORT_ORDER
} pindb_keys_t;


typedef enum pindb_mode_e {
  PINDB_MODE_INSERT = 1,
  PINDB_MODE_REPLACE
} pindb_mode_t;

typedef enum pindb_on_off_e {
  PINDB_OFF,
  PINDB_ON
} pindb_on_off_t;


typedef struct pindb_rec_s 	*pindb_rec_p;
typedef struct pindb_rec_s {
  
  int             coil_id;         /* coil ID             */
  cdb_channels_t  channel;         /* channel             */
  cdb_coil_port_t port;            /* port                */
  int             section_id;      /* Seciton ID          */
  int             sig;             /* Signal No.          */
  int             hybSig;          /* Hybrid Signal       */
  int             blockNo;         /* Block No            */
  int             numDrive;        /* Number of pin drive */
  int             rx_sel;          /* Rx_sel              */
  int             rf_on;           /* RF_ON               */
  int             rx_Nsel;         /* Rx_Nsel             */
  
  int			sort_order;
  int			len_of_descr;
  
  gpdb_descr_t	description;
  
} pindb_rec_t;

/* function prototypes */

#define PinDBOpen  GPDBOpen
#define PinDBClose GPDBClose

pindb_handle_p	PinDBOpen	( char *,char *,int );
status_t	PinDBGet		(gpdb_handle_p,void *,pindb_rec_p);
status_t	PinDBGet		(gpdb_handle_p,void *,pindb_rec_p);
status_t	PinDBGetAll	(gpdb_handle_p,void (*)(void *,pindb_rec_p));
status_t	PinDBSearch	(gpdb_handle_p,void (*)(void *,pindb_rec_p),...);
status_t	PinDBPut		(gpdb_handle_p,gpdb_mode_t,void *,pindb_rec_p);
status_t	PinDBDelete	(gpdb_handle_p,void *);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* pin_H__ */

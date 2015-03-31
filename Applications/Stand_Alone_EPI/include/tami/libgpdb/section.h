/***********************************************************************
 *
 *  (c) Copyright 2002 TOSHIBA Corporation All Rights Reserved
 *
 *    File Overview:
 *      This file defines ...
 *
 ***********************************************************************/
/* Revision History:
 * Jul.05.2006  K.Kokubun                  Created for SATURN
 * 
 * End of History */

#ifndef section_H__
#define section_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __NUTC__
#include <tami/common/machdefs.h>
#endif
#include <tami/libgpdb/gpdb.h>
#include <tami/libgpdb/coil.h>

#define SECTION_DB_NAME		"sectionDB"

typedef void    *sedb_handle_p;
typedef int32_t	sedb_key_t;

typedef enum sedb_keys_e {
    SECTION_DB_KEY_TYPE	= 1,
    SECTION_DB_KEY_INSTALLED,
    SECTION_DB_KEY_ID,
    SECTION_DB_KEY_CHANNEL,
    SECTION_DB_KEY_PORT,
    SECTION_DB_KEY_SID,
    SECTION_DB_KEY_SIG,
    SECTION_DB_KEY_HYBSIG,
    SECTION_DB_KEY_MAXCH,
    SECTION_DB_KEY_MINCH,
    SECTION_DB_KEY_DISTPRI,
    SECTION_DB_KEY_INSTPRI,
    SECTION_DB_KEY_DIST,
    SECTION_DB_KEY_ELEMENT,
    SECTION_DB_KEY_LINK,
    SECTION_DB_KEY_COILDETECT,
    SECTION_DB_KEY_DESCR,
    SECTION_DB_KEY_SORT_ORDER
    } sedb_keys_t;


typedef enum sedb_mode_e {
    SEDB_MODE_INSERT = 1,
    SEDB_MODE_REPLACE
    } sedb_mode_t;

typedef enum sedb_on_off_e {
    SEDB_OFF,
    SEDB_ON
    } sedb_on_off_t;


typedef struct sedb_rec_s 	*sedb_rec_p;
typedef struct sedb_rec_s {

  int             coil_id;         /* coil ID                */
  cdb_channels_t  channel;         /* channel                */
  cdb_coil_port_t port;            /* port                   */
  int             section_id;      /* Seciton ID             */
  int             sig;             /* Signal No.             */
  int             hybSig;          /* Hybrid Signal          */
  int             maxChannel;      /* Max. ch of section     */
  int             minChannel;      /* Min. ch of section     */
  int             sectionPri;      /* Section priority       */
  int             insectionPri;    /* InSection priority     */
  float           dist;            /* Distance of section   [mm] */
  int             elemXYZ[3];      /* length of element(XYZ) */
  int             coilDetect;      /*                        */
  int             link;            /*                        */  

  int			sort_order;
  int			len_of_descr;
  gpdb_descr_t	description;

} sedb_rec_t;

/* function prototypes */

#define SeDBOpen	GPDBOpen
#define SeDBClose GPDBClose

sedb_handle_p	SeDBOpen	( char *,char *,int );
status_t	SeDBGet		(gpdb_handle_p,void *,sedb_rec_p);
status_t	SeDBGet		(gpdb_handle_p,void *,sedb_rec_p);
status_t	SeDBGetAll	      (gpdb_handle_p,void (*)(void *,sedb_rec_p));
status_t	SeDBSearch	      (gpdb_handle_p,void (*)(void *,sedb_rec_p),...);
status_t	SeDBPut		(gpdb_handle_p,gpdb_mode_t,void *,sedb_rec_p);
status_t	SeDBDelete	      (gpdb_handle_p,void *);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* section_H__ */

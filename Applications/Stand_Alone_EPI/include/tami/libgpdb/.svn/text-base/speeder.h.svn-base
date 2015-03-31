/***********************************************************************
 *
 *  (c) Copyright 2002 TOSHIBA Corporation All Rights Reserved
 *
 *    File Overview:
 *      This file defines ...
 *
 ***********************************************************************/
/* Revision History:
 * Jan.15.2002  •Ÿ“‡ –L		Created
 * Sep.19.2003  •Ÿ“‡ –L     V6.20  add. SE_SPEEDER_RATIO
 * Jun.30.2006  K.Kokubun                  added for SATURN 
 * 
 * End of History */

#ifndef speeder_H__
#define speeder_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __NUTC__
#include <tami/common/machdefs.h>
#endif
#include <tami/libgpdb/gpdb.h>

/* Added for SATURN Jul.05.2006 K.Kokubun
 */
#include <tami/libgpdb/coil.h>

#define SPEEDER_DB_NAME		"speederDB"

typedef void    *spdb_handle_p;
typedef int32_t	spdb_key_t;

typedef enum spdb_keys_e {
  SPEEDER_DB_KEY_TYPE	= 1,
  SPEEDER_DB_KEY_INSTALLED,
  SPEEDER_DB_KEY_ID,
  SPEEDER_DB_KEY_DESCR,
  SPEEDER_DB_KEY_SORT_ORDER,

  /* Added for V9.00 Jul.20.2006 K.Kokubun */
  SPEEDER_DB_KEY_SID,  
  SPEEDER_DB_KEY_HYBMODE

} spdb_keys_t;

typedef enum spdb_mode_e {
  SPDB_MODE_INSERT = 1,
  SPDB_MODE_REPLACE
} spdb_mode_t;

typedef enum spdb_on_off_e {
  SPDB_OFF,
  SPDB_ON
} spdb_on_off_t;


typedef struct spdb_rec_s 	*spdb_rec_p;
typedef struct spdb_rec_s {
  int			speeder_id;

  /* Addef for Saturn Jun.30.2006
   */
  int            section_id;      /* Seciton ID          */
  int            hybMod;          /* Hybrid Mode         */
  
  
  int			s_MaxFov_dx ;	/* Maximum FOV -X (mm) */
  int			s_MaxFov_dy ;	/* Maximum FOV -Y (mm) */
  int			s_MaxFov_dz ;	/* Maximum FOV -Z (mm) */
  int			s_MaxFov_ix ;	/* Maximum FOV +X (mm) */
  int			s_MaxFov_iy ;	/* Maximum FOV +Y (mm) */
  int			s_MaxFov_iz ;	/* Maximum FOV +Z (mm) */

  int			s_MinFov_dx ;	/* Minimum FOV -X (mm) */
  int			s_MinFov_dy ;	/* Minimum FOV -Y (mm) */
  int			s_MinFov_dz ;	/* Minimum FOV -Z (mm) */
  int			s_MinFov_ix ;	/* Minimum FOV +X (mm) */
  int			s_MinFov_iy ;	/* Minimum FOV +Y (mm) */
  int			s_MinFov_iz ;	/* Minimum FOV +Z (mm) */

  int			s_MaxRatio ;	/* Maximum Speeder Total Ratio */
  int			s_PEMaxRatio ;	/* Maximum Speeder PE Ratio V6.20 */
  int			s_SEMaxRatio ;	/* Maximum Speeder SE Ratio V6.20 */
  int			rfsLimit ;	/* Limit of Relative Couch position for Speeder (mm) */

  spdb_on_off_t	is_installed;
  
  /* Addef for Saturn Jun.30.2006
   */
  int            s_Use_EnableXYZ;     /* Use Speeder Enable Flag in XYZ */

  int            s_EnableX;           /* Speeder Enable Flag in X */
  int            s_Pe_RatioX;         /* Maximum PE Speeder Ratio in X */
  int            s_Se_RatioX;         /* Maximum SE Speeder Ratio in X */

  int            s_EnableY;           /* Speeder Enable Flag in Y */
  int            s_Pe_RatioY;         /* Maximum PE Speeder Ratio in Y */
  int            s_Se_RatioY;         /* Maximum SE Speeder Ratio in Y */

  int            s_EnableZ;           /* Speeder Enable Flag in Z */  
  int            s_Pe_RatioZ;         /* Maximum PE Speeder Ratio in Z */
  int            s_Se_RatioZ;         /* Maximum SE Speeder Ratio in Z */

  int			sort_order;
    
  int			len_of_descr;
  gpdb_descr_t	description;

} spdb_rec_t;

/* function prototypes */

#define SpDBOpen	GPDBOpen
#define SpDBClose	GPDBClose

spdb_handle_p	SpDBOpen	( char *,char *,int );
status_t	SpDBGet		(gpdb_handle_p,void *,spdb_rec_p);
status_t	SpDBGet		(gpdb_handle_p,void *,spdb_rec_p);
status_t	SpDBGetAll	(gpdb_handle_p,void (*)(void *,spdb_rec_p));
status_t	SpDBSearch	(gpdb_handle_p,void (*)(void *,spdb_rec_p),...);
status_t	SpDBPut		(gpdb_handle_p,gpdb_mode_t,void *,spdb_rec_p);
status_t	SpDBDelete	(gpdb_handle_p,void *);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* speeder_H__ */

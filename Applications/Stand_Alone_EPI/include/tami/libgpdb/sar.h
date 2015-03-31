/*********************************************************************
 *
 *  (c) Copyright 1994 TOSHIBA Corporation All Rights Reserved
 *
 *  File Name:       $Source: /mplus/cvsroot/Theta/develop/src.include/libgpdb/sar.h,v $  ($Revision: 1.2 $)
 *  Last Modified:   $Date: 2010/02/17 02:30:13 $
 *  Revision Log:    $Log: sar.h,v $
 *  Revision Log:    Revision 1.2  2010/02/17 02:30:13  karube
 *  Revision Log:    MCM0252-00020 Phase3 libWNP,Native
 *  Revision Log:
 *  Revision Log:    Revision 1.1.1.1  2010/01/06 06:45:42  ohba
 *  Revision Log:    no message
 *  Revision Log:
 *  Revision Log:    Revision 1.2.22.1.2.3.56.1.2.1  2007/05/09 04:41:52  koku
 *  Revision Log:    MCM0200-00001 : GPDB for V9.01R245
 *  Revision Log:
 *  Revision Log:    Revision 1.2.22.1.2.3.56.1  2006/09/07 11:27:50  koku
 *  Revision Log:    MCM0177-00110 : GPDB for V9.00
 *  Revision Log:
 *  Revision Log:    Revision 1.2.22.1.2.3  2003/03/31 07:25:27  ohba
 *  Revision Log:    from WIPRO MAR 31
 *  Revision Log:
 *  Revision Log:    Revision 1.3  2002/08/07 07:56:07  purandar
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



#ifndef libgpdb_sar_H__
#define libgpdb_sar_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <tami/libgpdb/gpdb.h>

#define SDB_DB_NAME		"sarDB"
#define SDB_MAX_ANATOMY		10
#define SDB_OTHER_COIL_ID	(((('O' << 8) | 'L') << 16) | 0x0000)

/* data-structures and data-types */

typedef void *sdb_handle_p;

typedef enum sdb_keys_e {
  SDB_KEY_TYPE        = 1,
  SDB_KEY_INSTALLED,
  SDB_KEY_ID,
  SDB_KEY_DESCR,
  SDB_KEY_SORT_ORDER
} sdb_keys_t;

typedef enum sdb_on_off_e {
  SDB_OFF,
  SDB_ON
} sdb_on_off_t;

typedef enum sdb_sar_type_e {
  SDB_SAR_LIMIT = 1,
  SDB_SAR_MATRIX,
  SDB_SAR_FACTOR,
  SDB_RF_POWER,
  SDB_MASS_RATIO,
  SDB_MASS_RATIO_WHOLEBODY,    /* W */
  SDB_MASS_RATIO_HEAD,         /* H */
  SDB_MASS_RATIO_PARTIAL,      /* P */
  SDB_F1F2_PARAM,
  SDB_SAR_VERSION
} sdb_sar_type_t;

typedef enum sdb_mode_e {
  SDB_MODE_INSERT = 1,
  SDB_MODE_REPLACE
} sdb_mode_t;

typedef struct sdb_db_key_s {
  sdb_sar_type_t	type;
  int32_t		id;
} sdb_db_key_t;

/* added for V9.01 Vantage */
typedef struct sdb_sar_version_s {
  int32_t	version;
  int32_t	revision;
} sdb_sar_version_t;


typedef struct sdb_sar_limit_s {
  int32_t	limit_id;
  flt32_t	value[4];
} sdb_sar_limit_t;

typedef struct sdb_sar_matrix_s {
  int32_t	coil_id;
  flt32_t       dummy_height;
  flt32_t       dummy_weight;
  int32_t	value[SDB_MAX_ANATOMY];
} sdb_sar_matrix_t;

typedef struct sdb_sar_factor_s {
  int32_t	coil_id;
  flt32_t	pcoil;
  flt32_t	k_value;
  flt32_t	pmax[2];
  int32_t       ef;
  int32_t       ie;
  int32_t       min;
  int32_t       max;
  
} sdb_sar_factor_t;

typedef struct sdb_rf_power_s {
  flt32_t	value[3];	/* 0:LOW 1:MID 2:HIGHT */
} sdb_rf_power_t;

typedef struct sdb_mass_ratio_s {
  int32_t	coil_id;

  sdb_sar_type_t  sar_type;
  flt32_t         height;
  struct {
    flt32_t       ratio;
    flt32_t       fixed_value;
  } weight;
  
    struct {
	flt32_t	ratio;
	flt32_t	fixed_value;
    } value[SDB_MAX_ANATOMY]; /* ID is depend on SVN_PLN_SCAN_ANATOMY */
} sdb_mass_ratio_t;

typedef struct sdb_f1f2_param_s {
    int32_t	F2Ratio;	/* 891/1000 */
    int32_t	TxLoadW;	/* 942/1000 */
} sdb_f1f2_param_t;

typedef char *sdb_descr_t;

typedef struct sdb_rec_s *sdb_rec_p;
typedef struct sdb_rec_s {
    int			sar_id;
    sdb_sar_type_t	type;
    sdb_on_off_t        is_installed;
    
    union {
      sdb_sar_version_t sar_version;
      sdb_sar_limit_t	sar_limit;
      sdb_sar_matrix_t	sar_matrix;
      sdb_sar_factor_t	sar_factor;
      sdb_rf_power_t	rf_power;
      sdb_mass_ratio_t	mass_ratio;
      sdb_f1f2_param_t	f1f2_param;
    } param;

    int			sort_order;
    char		identifier;
    
    int			len_of_descr;
    sdb_descr_t		description;
} sdb_rec_t;

#define SDB_VERSION_KEY		"_version"

/* Error Severity Constants */

/* function prototypes */

sdb_handle_p	SdbOpen		(char *,char *,int);
status_t	SdbClose	(sdb_handle_p);
status_t	SdbGet		(sdb_handle_p,void *,sdb_rec_p);
status_t	SdbGetAll	(sdb_handle_p,void (*)(void *,sdb_rec_p));
status_t	SdbSearch	(sdb_handle_p,void (*)(void *,sdb_rec_p),...);
status_t	SdbPut		(sdb_handle_p,sdb_mode_t,void *,sdb_rec_p);
status_t	SdbDelete	(sdb_handle_p,void *);

/* MACRO definitions */

#define SDB_REC_KEY(k,t,i)	(k.type = t,k.id = i)
#define SDB_DECODE_KEY(k,t,i)     ((t) = k.type, (i) = k.id)

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* libgpdb_sar_H__ */


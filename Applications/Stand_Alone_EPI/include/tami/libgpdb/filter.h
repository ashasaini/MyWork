/*********************************************************************
 *
 *  (c) Copyright 1994 TOSHIBA Corporation All Rights Reserved
 *
 *    File Overview:
 *      This file defines ...
 *
 *********************************************************************/
/* Revision History:
 * May.31.1994	Naoyuki Furudate	Created
 * Sep.21.1995	Yoshiteru Watanabe	Added LSI & BEST
 * Dec.09.1997	Yoshiteru Watanabe	Added IES & Zoom In Plane
 * Jun.03.2002	Yoshiteru Watanabe	Added Spike Noise Killer
 * Mar.10.2003	Yoshiteru Watanabe	Added GDC
 * Oct.06.2004	Yoshiteru Watanabe	Added GOP
 * End of History */



#ifndef libgpdb_filter_H__
#define libgpdb_filter_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <tami/libgpdb/gpdb.h>
#include <tami/msAP/msAP.h>

#define FDB_DB_NAME	"filterDB"

/* data-structures and data-types */

typedef void 	*fdb_handle_p;
#ifdef __FREE_BSD__
typedef uint64_t fdb_db_key_t;
#else
typedef int32_t  fdb_db_key_t;
#endif

typedef enum fdb_keys_e {
  FDB_KEY_TYPE        = 1,
  FDB_KEY_INSTALLED,
  FDB_KEY_ID,
  FDB_KEY_DESCR,
  FDB_KEY_SORT_ORDER
} fdb_keys_t;

typedef enum fdb_on_off_e {
  FDB_OFF,
  FDB_ON
} fdb_on_off_t;

typedef enum fdb_filter_type_e {
  FDB_REFINE = 1,
  FDB_K_SPACE,
  FDB_FILTER_SET,
  FDB_REFINE2,
  FDB_LSI,
  FDB_BEST,
  FDB_IES,
  FDB_ZIP,
  FDB_SNK,
  FDB_GDC,
  FDB_GOP,
  FDB_FILTER_SET2,
  FDB_EMTONE
} fdb_filter_type_t;

typedef enum fdb_mode_e {
  FDB_MODE_INSERT = 1,
  FDB_MODE_REPLACE
} fdb_mode_t;

typedef RfltRefinePars_t 	fdb_refine_param_t;
typedef RfltRefine2Pars_t 	fdb_refine2_param_t;
typedef RfltLSIPars_t 		fdb_lsi_param_t;
typedef RfltBESTPars_t		fdb_best_param_t;

typedef struct fdb_k_space_param_s {
  int32_t	type; 
  flt32_t	dump;
  flt32_t	rhw;
  flt32_t	r;
  flt32_t	a;
  flt32_t	b;
  flt32_t	dumpRo;
  flt32_t	rhwRo;
  flt32_t	rRo;
  flt32_t	aRo;
  flt32_t	bRo;
  flt32_t	cRo;
  flt32_t	dumpPe;
  flt32_t	rhwPe;
  flt32_t	rPe;
  flt32_t	aPe;
  flt32_t	bPe;
  flt32_t	cPe;
  flt32_t	dumpSe;
  flt32_t	rhwSe;
  flt32_t	rSe;
  flt32_t	aSe;
  flt32_t	bSe;
  flt32_t	cSe;

  /* New K-space */
  flt32_t	alpha;
  flt32_t	p;
  flt32_t	radius;
  flt32_t	width;
} fdb_k_space_param_t;

typedef struct fdb_ies_param_s {
  int32_t number;
} fdb_ies_param_t;

typedef struct fdb_zip_param_s {
  flt32_t wsgDump;
  flt32_t wsgRhw;
} fdb_zip_param_t;

typedef struct fdb_snk_param_s {
  flt32_t a;
  flt32_t b;
  int32_t k;
} fdb_snk_param_t;

typedef struct fdb_gdc_param_s {
  int32_t number;
} fdb_gdc_param_t;

typedef struct fdb_gop_param_s {
  int32_t number;
} fdb_gop_param_t;

typedef struct fdb_filter_set2_s {
  int32_t	k_space;
  int32_t	gop;
} fdb_filter_set2_t;

typedef struct fdb_filter_set_s {
  int32_t	k_space;
  int32_t	refine;
} fdb_filter_set_t;

typedef struct fdb_emtone_param_s {
  flt32_t LSIstrengthHP;
  flt32_t HpMaskSoftThreshold;
  flt32_t EMthresholdRate;
  int32_t LowpassFilterType;
  int32_t BandpassFilterType;
} fdb_emtone_param_t;

typedef char *fdb_descr_t;

typedef struct fdb_rec_s *fdb_rec_p;
typedef struct fdb_rec_s {
  int			filter_id;
  fdb_filter_type_t	type;
  fdb_on_off_t        	is_installed;
    
  union {
    fdb_refine_param_t	refine;
    fdb_refine2_param_t	refine2;
    fdb_lsi_param_t    	lsi;
    fdb_best_param_t	best;
    fdb_ies_param_t 	ies;
    fdb_zip_param_t 	zip;
    fdb_snk_param_t 	snk;
    fdb_gdc_param_t 	gdc;
    fdb_gop_param_t 	gop;
    fdb_k_space_param_t	k_space;
    fdb_filter_set_t	filter_set;
    fdb_filter_set2_t	filter_set2;
    fdb_emtone_param_t  emtone;
  } param;
    
  int			sort_order;
  char		identifier;
    
  int			len_of_descr;
  fdb_descr_t		description;
} fdb_rec_t;

#define FDB_VERSION_KEY		"_version"

/* Error Severity Constants */

/* function prototypes */

fdb_handle_p	FdbOpen		(char *,char *,int);
status_t	FdbClose	(fdb_handle_p);
status_t	FdbGet		(fdb_handle_p,void *,fdb_rec_p);
status_t	FdbGetAll	(fdb_handle_p,void (*)(void *,fdb_rec_p));
status_t	FdbSearch	(fdb_handle_p,void (*)(void *,fdb_rec_p),...);
status_t	FdbPut		(fdb_handle_p,fdb_mode_t,void *,fdb_rec_p);
status_t	FdbDelete	(fdb_handle_p,void *);

/* MACRO definitions */

#define FDB_REC_KEY(type,id)		((type) << 16 | (id) & 0x0000ffff)
#define FDB_DECODE_KEY(key,type,id)     \
    (type) = (key) >> 16; \
    (id) = 0x0000ffff & (key)

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* libgpdb_filter_H__ */

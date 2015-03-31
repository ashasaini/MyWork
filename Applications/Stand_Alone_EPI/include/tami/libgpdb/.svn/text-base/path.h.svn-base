/***********************************************************************
 *
 *  (c) Copyright 2001 TOSHIBA Corporation All Rights Reserved
 *
 *    File Overview:
 *      This file defines ...
 *
 ***********************************************************************/
/* Revision History:
 * Feb. 1.01	å√ä⁄ íºçK	Created
 * Jan.12.2002  ïüìá ñL
 *              COILÉGÉåÉÅÉìÉgÇÃ16chëŒâûÇÃà◊Ç…PATHÇägí£ÇµÇΩÅB
 * End of History */

#ifndef path_H__
#define path_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <tami/libgpdb/gpdb.h>

#define PATH_DB_NAME		"pathDB"

#ifdef __FREE_BSD__
typedef uint64_t path_db_key_t;
#else
typedef int32_t  path_db_key_t;
#endif

typedef enum path_db_keys_e {
    PATH_DB_KEY_TYPE	= 1,
    PATH_DB_KEY_INSTALLED,
    PATH_DB_KEY_ID,
    PATH_DB_KEY_DESCR,
    PATH_DB_KEY_SORT_ORDER
    } path_db_keys_t;

typedef struct path_db_rec_s 	*path_db_rec_p;
typedef struct path_db_rec_s {
    int			path_id;
    unsigned long	path[2];		/* V5.00 */
    unsigned long	path_data[8];

    gpdb_on_off_t	is_installed;
    
    int			sort_order;
    
    int			len_of_descr;
    gpdb_descr_t	description;

} path_db_rec_t;

/* function prototypes */

#define PathDBOpen	GPDBOpen
#define PathDBClose	GPDBClose

status_t	PathDBGet	(gpdb_handle_p,void *,path_db_rec_p);
status_t	PathDBGetAll	(gpdb_handle_p,void (*)(void *,path_db_rec_p));
status_t	PathDBSearch	(gpdb_handle_p,void (*)(void *,path_db_rec_p),...);
status_t	PathDBPut	(gpdb_handle_p,gpdb_mode_t,void *,path_db_rec_p);
status_t	PathDBDelete	(gpdb_handle_p,void *);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* path_H__ */

/******************************************************************************
 *  
 *    File Overview:
 *      This file defines routines for...
 *
 *    Objects with external linkage defined in this file:
 *      
 *
 *****************************************************************************/
/* Revision History:
 * yyyy-mm-dd     modified-by     modifications-done
 * 2004-09-07     T.Kohketsu      Created.
 * 2005-01-08     M.Ogiwara       added OPEN_RETRY, modified OPEN_RETRY_WAIT_TIME 
 * 2005-01-25     M.Ogiwara       The writing addition of the log file 
 * End of History */



#ifndef msOpen_H__
#define msOpen_H__

#ifdef __cplusplus
extern "C" {
#endif

#define OPEN_RETRY		50
#define OPEN_RETRY_WAIT_TIME	200000

#define ERROR_LOG_PATH		"/usr/tmp"
#define MS_OPEN_LOG_NAME	"msOpen_failed_log"
#define MS_FOPEN_LOG_NAME	"msFopen_failed_log"

enum {
     MS_OPEN = 0,
     MS_FOPEN,
     NumOfOpenTyps
};

int msOpen(char *pathname, int oflag, ...);

FILE *msFopen(char *pathname, char *type);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* msOpen_H__ */

